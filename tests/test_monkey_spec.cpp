// Monkey's Audio checks (MAC SDK): the header of the old format (before 3.98: optional peak level and seek elements) and of the new format
// (descriptor and header), built bit by bit, and real files of MAC.exe with 8, 16 and 24 bits and 6 channels.
#include "id3v2_support.h"
#include <cmath>
#include <filesystem>
#include <string>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void le16(Bytes& b, uint32_t v) { b.push_back(static_cast<uint8_t>(v)); b.push_back(static_cast<uint8_t>(v >> 8)); }
void le32(Bytes& b, uint32_t v) { le16(b, v & 0xFFFF); le16(b, v >> 16); }

struct Ape {
    uint32_t version = 3990;
    uint32_t compression = 2000;
    uint32_t flags = 0;
    uint32_t channels = 2;
    uint32_t rate = 44100;
    uint32_t bits = 16;              // new format
    uint32_t blocksPerFrame = 73728;
    uint32_t frames = 10;
    uint32_t finalBlocks = 12345;
    uint32_t descriptorBytes = 52;   // new format: the header is behind the descriptor
    uint32_t peak = 16384;           // old format, with the flag 4
    uint32_t seekElements = 10;      // old format, with the flag 16
    size_t dataBytes = 20000;
};

Bytes apeFile(const Ape& a)
{
    Bytes b;
    put(b, "MAC ");
    le16(b, a.version);
    if (a.version < 3980) {
        le16(b, a.compression); le16(b, a.flags); le16(b, a.channels); le32(b, a.rate);
        le32(b, 44); le32(b, 0);                      // header bytes, terminating bytes
        le32(b, a.frames); le32(b, a.finalBlocks);
        if (a.flags & 4) le32(b, a.peak);
        if (a.flags & 16) le32(b, a.seekElements);
    } else {
        le16(b, 0);                                   // padding
        le32(b, a.descriptorBytes);
        le32(b, 24); le32(b, 40); le32(b, 44);        // header bytes, seek table bytes, header data bytes
        le32(b, static_cast<uint32_t>(a.dataBytes)); le32(b, 0); le32(b, 0);
        for (int i = 0; i < 16; i++) b.push_back(static_cast<uint8_t>(0xC0 + i));   // MD5
        while (b.size() < a.descriptorBytes) b.push_back(0x99);   // an extended descriptor
        le16(b, a.compression); le16(b, a.flags);
        le32(b, a.blocksPerFrame); le32(b, a.finalBlocks); le32(b, a.frames);
        le16(b, a.bits); le16(b, a.channels); le32(b, a.rate);
    }
    b.resize(b.size() + a.dataBytes, 0x5C);
    return b;
}

fs::path copyFixture(const char* rel)
{
    const fs::path dst = tempDir() / fs::path(rel).filename();
    fs::copy_file(fs::path(AG3_FIXTURES_DIR) / rel, dst, fs::copy_options::overwrite_existing);
    return dst;
}

}  // namespace

TEST_CASE("Monkey's Audio new format: bits per sample, samples, duration, compression level", "[monkey][spec]")
{
    SECTION("16 bit stereo: 9 frames of 73728 blocks and a final frame") {
        Ape a;
        auto p = writeTemp("ape_new16.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(MONKEYGetBitsW() == 16);
        const long samples = 9 * 73728 + 12345;
        CHECK(MONKEYGetSamplesW() == samples);
        CHECK(MONKEYGetFramesW() == 10);
        CHECK(MONKEYGetSamplesPerFrameW() == 73728);
        CHECK(std::fabs(AUDIOGetDurationW() - samples / 44100.0) < 0.001);
        CHECK(take(AUDIOGetVersionW()) == L"3.99");
        CHECK(take(MONKEYGetCompressionW()) == L"Normal");
    }
    SECTION("the bits per sample come from the header: 8 and 24 bit") {
        for (uint32_t bits : { 8u, 24u }) {
            Ape a; a.bits = bits;
            auto p = writeTemp("ape_new_bits.ape", apeFile(a));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
            CHECK(MONKEYGetBitsW() == static_cast<short>(bits));
        }
    }
    SECTION("all compression levels and the blocks per frame of the header") {
        const struct { uint32_t level; const wchar_t* name; uint32_t blocks; } levels[] = {
            { 1000, L"Fast", 73728 }, { 2000, L"Normal", 73728 }, { 3000, L"High", 73728 }, { 4000, L"Extra High", 294912 }, { 5000, L"Insane", 1179648 } };
        for (const auto& l : levels) {
            INFO("level " << l.level);
            Ape a; a.compression = l.level; a.blocksPerFrame = l.blocks; a.frames = 3;
            auto p = writeTemp("ape_level.ape", apeFile(a));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
            CHECK(take(MONKEYGetCompressionW()) == l.name);
            CHECK(MONKEYGetSamplesPerFrameW() == static_cast<long>(l.blocks));
            CHECK(MONKEYGetSamplesW() == static_cast<long>(2 * l.blocks + 12345));
        }
        Ape a; a.compression = 9000;   // not a known level: no access outside of the table
        auto p = writeTemp("ape_level_unknown.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(take(MONKEYGetCompressionW()) == L"Unknown");
    }
    SECTION("the header can be behind a larger descriptor") {
        Ape a; a.descriptorBytes = 76; a.bits = 24; a.rate = 96000;
        auto p = writeTemp("ape_big_descriptor.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(AUDIOGetSampleRateW() == 96000);
        CHECK(MONKEYGetBitsW() == 24);
        CHECK(MONKEYGetFramesW() == 10);
    }
    SECTION("more than 2 billion samples: the duration is right, the function limits the number") {
        Ape a; a.frames = 100000; a.blocksPerFrame = 294912; a.finalBlocks = 1000; a.compression = 4000;
        auto p = writeTemp("ape_long.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        const double samples = 99999.0 * 294912 + 1000;
        CHECK(std::fabs(AUDIOGetDurationW() / (samples / 44100.0) - 1.0) < 1e-5);
        CHECK(MONKEYGetSamplesW() == 2147483647);
    }
    SECTION("a file without frames has the duration 0") {
        Ape a; a.frames = 0;
        auto p = writeTemp("ape_no_frames.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(AUDIOGetDurationW() == 0.0f);
        CHECK(MONKEYGetSamplesW() == 0);
    }
    SECTION("channels") {
        Ape a; a.channels = 6;
        auto p = writeTemp("ape_6ch.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(AUDIOGetChannelsW() == 6);
        CHECK(take(AUDIOGetChannelModeW()) == L"Multi Channel");
        Ape m; m.channels = 1;
        p = writeTemp("ape_1ch.ape", apeFile(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(take(AUDIOGetChannelModeW()) == L"Mono");
    }
}

TEST_CASE("Monkey's Audio old format (before 3.98): the peak level and the seek elements are optional", "[monkey][spec]")
{
    const struct { uint32_t flags; const char* name; } cases[] = {
        { 0, "no optional field" }, { 4, "peak level" }, { 16, "seek elements" }, { 4 | 16, "both" }, { 4 | 16 | 2 | 32, "both, CRC and no WAV header" } };
    for (const auto& c : cases) {
        INFO(c.name);
        Ape a; a.version = 3970; a.flags = c.flags; a.compression = 2000; a.frames = 5; a.finalBlocks = 30000;
        auto p = writeTemp("ape_old.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(MONKEYGetBitsW() == 16);
        CHECK(MONKEYGetSamplesPerFrameW() == 73728 * 4);   // from 3.95: 4 x 73728 blocks
        CHECK(MONKEYGetSamplesW() == 4 * 73728 * 4 + 30000);
        CHECK(take(AUDIOGetVersionW()) == L"3.97");
        if (c.flags & 4) CHECK(std::fabs(MONKEYGetPeakW() - 16384 / 32768.0f * 100.0f) < 0.01f);
        else CHECK(MONKEYGetPeakW() == 0.0f);
    }
    SECTION("bits from the format flags: 8 bit (flag 1) and 24 bit (flag 8)") {
        Ape a; a.version = 3970; a.flags = 1;
        auto p = writeTemp("ape_old8.ape", apeFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(MONKEYGetBitsW() == 8);
        Ape b; b.version = 3970; b.flags = 8 | 4;
        p = writeTemp("ape_old24.ape", apeFile(b));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(MONKEYGetBitsW() == 24);
        CHECK(std::fabs(MONKEYGetPeakW() - 16384 / 8388608.0f * 100.0f) < 0.001f);
    }
    SECTION("blocks per frame by version and level") {
        const struct { uint32_t version, level, blocks; } v[] = { { 3990, 2000, 0 }, { 3950, 2000, 294912 }, { 3900, 2000, 73728 }, { 3820, 4000, 73728 }, { 3820, 2000, 9216 }, { 3700, 2000, 9216 } };
        for (const auto& x : v) {
            if (x.blocks == 0) continue;
            INFO("version " << x.version << " level " << x.level);
            Ape a; a.version = x.version; a.compression = x.level; a.frames = 3;
            auto p = writeTemp("ape_old_blocks.ape", apeFile(a));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
            CHECK(MONKEYGetSamplesPerFrameW() == static_cast<long>(x.blocks));
        }
    }
}

TEST_CASE("Monkey's Audio: damaged and cut headers", "[monkey][spec]")
{
    const Bytes f = apeFile(Ape());
    SECTION("the file ends inside the header") {
        for (size_t n : { size_t(5), size_t(30), size_t(60), size_t(75) }) {
            INFO("length " << n);
            const Bytes cut(f.begin(), f.begin() + static_cast<std::ptrdiff_t>(n));
            auto p = writeTemp("ape_cut.ape", cut);
            CHECK(AUDIOAnalyzeFileW(p.c_str()) != MONKEY);
        }
    }
    SECTION("a descriptor of less than 52 bytes is invalid") {
        Bytes g = f;
        g[8] = 40;
        auto p = writeTemp("ape_short_descriptor.ape", g);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != MONKEY);
    }
    SECTION("a descriptor that reaches over the end of the file") {
        Bytes g = f;
        g[8] = 0xFF; g[9] = 0xFF; g[10] = 0xFF; g[11] = 0x7F;
        auto p = writeTemp("ape_far_descriptor.ape", g);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != MONKEY);
    }
}

TEST_CASE("Monkey's Audio: real files of MAC.exe with 8 bit, 24 bit and 6 channels", "[monkey][spec]")
{
    const struct { const char* file; int rate, channels, bits; double seconds; } files[] = {
        { "ape/bits24_c2000.ape", 48000, 2, 24, 0.5 }, { "ape/bits8_c2000.ape", 11025, 1, 8, 0.5 },
        { "ape/ch6_c2000.ape", 44100, 6, 16, 0.3 }, { "ape/no_tags_c5000.ape", 44100, 2, 16, 1.0 } };
    for (const auto& x : files) {
        INFO(x.file);
        const fs::path p = copyFixture(x.file);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(AUDIOGetSampleRateW() == x.rate);
        CHECK(AUDIOGetChannelsW() == x.channels);
        CHECK(MONKEYGetBitsW() == x.bits);
        CHECK(std::fabs(AUDIOGetDurationW() - x.seconds) < 0.01);
        // the bit rate is the size of the file per second
        CHECK(AUDIOGetBitrateW() == static_cast<long>(fs::file_size(p) * 8.0 / AUDIOGetDurationW() / 1000.0 + 0.5));
    }
    SECTION("the insane level has a name") {
        const fs::path p = copyFixture("ape/no_tags_c5000.ape");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
        CHECK(take(MONKEYGetCompressionW()) == L"Insane");
    }
}
