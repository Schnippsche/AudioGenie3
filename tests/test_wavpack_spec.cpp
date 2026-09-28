// WavPack checks (file format of wavpack.com): blocks with the header of 32 bytes and the metadata sub-blocks, built byte by byte, and real files
// of ffmpeg: several channels (a block per channel pair), sample rates that are not in the table, a total number of samples that is unknown.
#include "id3v2_support.h"
#include <cmath>
#include <filesystem>
#include <string>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

void le16(Bytes& b, uint32_t v) { b.push_back(static_cast<uint8_t>(v)); b.push_back(static_cast<uint8_t>(v >> 8)); }
void le32(Bytes& b, uint32_t v) { le16(b, v & 0xFFFF); le16(b, v >> 16); }

const uint32_t MONO = 0x4, FINAL = 0x1000, INITIAL = 0x800;

struct Block {
    uint32_t version = 0x410;
    uint32_t totalSamples = 44100;      // 0xFFFFFFFF: unknown
    uint32_t totalHigh = 0;             // the upper 8 bits (version 5)
    uint64_t blockIndex = 0;
    uint32_t blockSamples = 44100;
    uint32_t flags = 0;
    int rateIndex = 9;                  // 44100
    bool stereo = true;
    Bytes subBlocks;
    size_t audioBytes = 300;
};

Bytes block(const Block& b, bool first, bool last)
{
    Bytes body;
    body.insert(body.end(), b.subBlocks.begin(), b.subBlocks.end());
    body.resize(body.size() + b.audioBytes, 0x6D);
    Bytes out = { 'w', 'v', 'p', 'k' };
    le32(out, static_cast<uint32_t>(body.size() + 24));
    le16(out, b.version);
    out.push_back(static_cast<uint8_t>(b.blockIndex >> 32));
    out.push_back(static_cast<uint8_t>(b.totalHigh));
    le32(out, first ? b.totalSamples : 0);
    le32(out, static_cast<uint32_t>(b.blockIndex));
    le32(out, b.blockSamples);
    uint32_t flags = b.flags | (b.stereo ? 0 : MONO) | (static_cast<uint32_t>(b.rateIndex) << 23) | 1;   // 16 bit
    if (first) flags |= INITIAL;
    if (last) flags |= FINAL;
    le32(out, flags);
    le32(out, 0x12345678);   // crc
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

// a metadata sub-block: id, size in words (1 byte, or 3 bytes with the large flag), data padded to whole words
Bytes subBlock(uint8_t id, const Bytes& data, bool large = false)
{
    Bytes b;
    Bytes padded = data;
    uint8_t flags = id;
    if (padded.size() & 1) { padded.push_back(0); flags |= 0x40; }   // an odd size
    const size_t words = padded.size() / 2;
    if (large) { flags |= 0x80; b.push_back(flags); b.push_back(static_cast<uint8_t>(words)); b.push_back(static_cast<uint8_t>(words >> 8)); b.push_back(static_cast<uint8_t>(words >> 16)); }
    else { b.push_back(flags); b.push_back(static_cast<uint8_t>(words)); }
    b.insert(b.end(), padded.begin(), padded.end());
    return b;
}

// a file: frames of blocks; each frame has one block per entry of `layout` (true: stereo block, false: mono block)
Bytes wavpackFile(const Block& proto, const std::vector<bool>& layout, int frames)
{
    Bytes out;
    for (int f = 0; f < frames; f++) {
        for (size_t i = 0; i < layout.size(); i++) {
            Block b = proto;
            b.stereo = layout[i];
            b.blockIndex = proto.blockIndex + static_cast<uint64_t>(f) * proto.blockSamples;
            const Bytes blk = block(b, f == 0 && i == 0, i + 1 == layout.size());
            out.insert(out.end(), blk.begin(), blk.end());
        }
    }
    return out;
}

fs::path copyFixture(const char* rel)
{
    const fs::path dst = tempDir() / fs::path(rel).filename();
    fs::copy_file(fs::path(AG3_FIXTURES_DIR) / rel, dst, fs::copy_options::overwrite_existing);
    return dst;
}

}  // namespace

TEST_CASE("WavPack: duration with fractions of a second, sample rate and channel mode", "[wavpack][spec]")
{
    SECTION("stereo, 6.3 seconds") {
        Block b; b.totalSamples = 277830; b.blockSamples = 22050;
        auto p = writeTemp("wv_stereo.wv", wavpackFile(b, { true }, 13));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(std::fabs(AUDIOGetDurationW() - 6.3) < 0.001);      // the duration was cut to whole seconds
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(take(AUDIOGetChannelModeW()) == L"Stereo");
        CHECK(take(AUDIOGetVersionW()) == L"wavpack v4.16");
    }
    SECTION("every entry of the sample rate table") {
        const int rates[15] = { 6000, 8000, 9600, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000, 192000 };
        for (int i = 0; i < 15; i++) {
            INFO("index " << i);
            Block b; b.rateIndex = i; b.totalSamples = static_cast<uint32_t>(rates[i] * 3 / 2); b.blockSamples = static_cast<uint32_t>(rates[i] / 2);
            auto p = writeTemp("wv_rate.wv", wavpackFile(b, { true }, 3));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
            CHECK(AUDIOGetSampleRateW() == rates[i]);
            CHECK(std::fabs(AUDIOGetDurationW() - 1.5) < 0.001);
        }
    }
    SECTION("mono") {
        Block b;
        auto p = writeTemp("wv_mono.wv", wavpackFile(b, { false }, 2));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(AUDIOGetChannelsW() == 1);
        CHECK(take(AUDIOGetChannelModeW()) == L"Mono");
    }
}

TEST_CASE("WavPack: the channels of all blocks of a frame", "[wavpack][spec]")
{
    const struct { const char* name; std::vector<bool> layout; int channels; } cases[] = {
        { "3 channels: a pair and a single channel", { true, false }, 3 },
        { "4 channels", { true, true }, 4 },
        { "5.1: two pairs and two single channels", { true, true, false, false }, 6 },
        { "7.1: three pairs and two single channels", { true, true, true, false, false }, 8 },
    };
    for (const auto& c : cases) {
        INFO(c.name);
        Block b; b.blockSamples = 20000; b.totalSamples = 60000;
        auto p = writeTemp("wv_channels.wv", wavpackFile(b, c.layout, 3));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(AUDIOGetChannelsW() == c.channels);
        CHECK(take(AUDIOGetChannelModeW()) == L"Multi Channel");
        CHECK(std::fabs(AUDIOGetDurationW() - 60000.0 / 44100) < 0.001);
    }
}

TEST_CASE("WavPack: a sample rate that is not in the table is in a sub-block", "[wavpack][spec]")
{
    SECTION("the rate in the sub-block ID_SAMPLE_RATE (0x27)") {
        Block b; b.rateIndex = 15; b.totalSamples = 37000 * 2; b.blockSamples = 37000;
        b.subBlocks = subBlock(0x27, { 0x88, 0x90, 0x00 });   // 37000 = 0x009088
        auto p = writeTemp("wv_custom_rate.wv", wavpackFile(b, { true }, 2));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(AUDIOGetSampleRateW() == 37000);
        CHECK(std::fabs(AUDIOGetDurationW() - 2.0) < 0.001);
    }
    SECTION("other sub-blocks in front of it, one of them large and one with an odd size") {
        Block b; b.rateIndex = 15; b.totalSamples = 50000; b.blockSamples = 25000;
        b.subBlocks = subBlock(0x25, Bytes(9, 0x11));
        Bytes big = subBlock(0x21, Bytes(600, 0x22), true);
        b.subBlocks.insert(b.subBlocks.end(), big.begin(), big.end());
        Bytes rate = subBlock(0x27, { 0x80, 0xBB, 0x00 });   // 48000
        b.subBlocks.insert(b.subBlocks.end(), rate.begin(), rate.end());
        auto p = writeTemp("wv_custom_rate2.wv", wavpackFile(b, { true }, 2));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(AUDIOGetSampleRateW() == 48000);
    }
    SECTION("index 15 without the sub-block: no sample rate, no crash") {
        Block b; b.rateIndex = 15;
        auto p = writeTemp("wv_no_rate.wv", wavpackFile(b, { true }, 2));
        AUDIOAnalyzeFileW(p.c_str());
        CHECK(AUDIOGetDurationW() == 0.0f);
    }
}

TEST_CASE("WavPack: the total number of the samples has 40 bits; unknown is taken from the last block", "[wavpack][spec]")
{
    SECTION("a total of more than 2^32 samples (version 5)") {
        Block b; b.rateIndex = 10; b.totalSamples = 0x00001000; b.totalHigh = 0x02; b.blockSamples = 48000;   // 2 * 2^32 + 4096 - 2
        auto p = writeTemp("wv_40bit.wv", wavpackFile(b, { true }, 2));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        const double samples = 4096.0 + 2.0 * 4294967296.0 - 2.0;
        CHECK(std::fabs(AUDIOGetDurationW() / (samples / 48000.0) - 1.0) < 1e-5);
    }
    SECTION("unknown (0xFFFFFFFF): the end of the last block") {
        Block b; b.totalSamples = 0xFFFFFFFFu; b.blockSamples = 22050;
        auto p = writeTemp("wv_unknown.wv", wavpackFile(b, { true }, 7));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(std::fabs(AUDIOGetDurationW() - 7 * 22050.0 / 44100) < 0.001);
    }
    SECTION("unknown, with several blocks per frame and a shorter last frame") {
        Block b; b.totalSamples = 0xFFFFFFFFu; b.blockSamples = 20000;
        Bytes f = wavpackFile(b, { true, false }, 4);
        Block last = b; last.blockSamples = 5000; last.blockIndex = 4 * 20000;
        for (size_t i = 0; i < 2; i++) { last.stereo = (i == 0); const Bytes blk = block(last, false, i == 1); f.insert(f.end(), blk.begin(), blk.end()); }
        auto p = writeTemp("wv_unknown_multi.wv", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(AUDIOGetChannelsW() == 3);
        CHECK(std::fabs(AUDIOGetDurationW() - 85000.0 / 44100) < 0.001);
    }
    SECTION("a tag behind the last block is not part of it") {
        Block b; b.totalSamples = 0xFFFFFFFFu; b.blockSamples = 22050;
        Bytes f = wavpackFile(b, { true }, 5);
        Bytes tag(128, 0); tag[0] = 'T'; tag[1] = 'A'; tag[2] = 'G';
        f.insert(f.end(), tag.begin(), tag.end());
        auto p = writeTemp("wv_unknown_tag.wv", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(std::fabs(AUDIOGetDurationW() - 5 * 22050.0 / 44100) < 0.001);
    }
}

TEST_CASE("WavPack: invalid headers", "[wavpack][spec]")
{
    SECTION("versions that cannot be decoded") {
        for (uint32_t v : { 0x401u, 0x411u, 0x0u, 0x300u }) {
            INFO("version " << v);
            Block b; b.version = v;
            auto p = writeTemp("wv_version.wv", wavpackFile(b, { true }, 2));
            CHECK(AUDIOAnalyzeFileW(p.c_str()) != WAVPACK);
        }
        for (uint32_t v : { 0x402u, 0x403u, 0x410u }) {
            INFO("version " << v);
            Block b; b.version = v;
            auto p = writeTemp("wv_version_ok.wv", wavpackFile(b, { true }, 2));
            CHECK(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        }
    }
    SECTION("a block size below the header") {
        Bytes f = wavpackFile(Block(), { true }, 2);
        f[4] = 10; f[5] = 0; f[6] = 0; f[7] = 0;
        auto p = writeTemp("wv_small_block.wv", f);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != WAVPACK);
    }
    SECTION("the file ends inside the header") {
        const Bytes f = wavpackFile(Block(), { true }, 2);
        auto p = writeTemp("wv_cut.wv", Bytes(f.begin(), f.begin() + 20));
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != WAVPACK);
    }
}

TEST_CASE("WavPack: real files of ffmpeg", "[wavpack][spec]")
{
    SECTION("6 channels") {
        const fs::path p = copyFixture("wv/ch6.wv");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(AUDIOGetChannelsW() == 6);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(std::fabs(AUDIOGetDurationW() - 0.3) < 0.001);
    }
    SECTION("37000 Hz is not in the table") {
        const fs::path p = copyFixture("wv/rate37000.wv");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(AUDIOGetSampleRateW() == 37000);
        CHECK(std::fabs(AUDIOGetDurationW() - 0.5) < 0.001);
    }
    SECTION("a file from a pipe has no total number of samples") {
        const fs::path p = copyFixture("wv/no_total_samples.wv");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(std::fabs(AUDIOGetDurationW() - 0.5) < 0.001);
    }
    SECTION("a fraction of a second") {
        const fs::path p = copyFixture("wv/no_tags.wv");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAVPACK);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
    }
}
