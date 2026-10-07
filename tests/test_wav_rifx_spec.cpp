// WAV / RIFX checks: RIFX is the big endian variant of RIFF used by old Mac/SGI tools (e.g. QuickTime on PowerPC). The outer chunk is
// 'RIFX' instead of 'RIFF' and every size field in the file (the outer length, every chunk/subchunk header, the 'fmt ' payload fields
// SampleRate/BytesPerSecond/etc.) is big endian instead of the usual RIFF little endian; the audio sample bytes themselves are passed
// through unchanged either way, so they need no special handling here.
#include "id3v2_support.h"
#include <cmath>
#include <cstring>

using namespace ag3test;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
void be16(Bytes& b, uint16_t v) { b.push_back(static_cast<uint8_t>(v >> 8)); b.push_back(static_cast<uint8_t>(v)); }
void be32(Bytes& b, uint32_t v) { for (int i = 3; i >= 0; i--) b.push_back(static_cast<uint8_t>(v >> (8 * i))); }

Bytes fmtChunkBE(uint16_t channels, uint32_t rate, uint16_t bits)
{
    Bytes b;
    put(b, "fmt "); be32(b, 16);
    be16(b, 1);                                               // PCM
    be16(b, channels);
    be32(b, rate);
    const uint32_t blockAlign = channels * (bits / 8);
    be32(b, rate * blockAlign);                                // bytes per second
    be16(b, static_cast<uint16_t>(blockAlign));
    be16(b, bits);
    return b;
}

Bytes pcmSamples(size_t sampleFrames, uint16_t channels, uint16_t bits)
{
    Bytes b;
    const size_t bytesPerSample = bits / 8;
    for (size_t i = 0; i < sampleFrames * channels * bytesPerSample; i++) b.push_back(static_cast<uint8_t>((i * 37) & 0xFF));
    return b;
}

struct RifxSpec {
    uint16_t channels = 2;
    uint32_t rate = 44100;
    uint16_t bits = 16;
    size_t sampleFrames = 1000;
    bool infoTag = false;
};

Bytes rifxFile(const RifxSpec& s)
{
    const Bytes fmt = fmtChunkBE(s.channels, s.rate, s.bits);
    const Bytes pcm = pcmSamples(s.sampleFrames, s.channels, s.bits);
    Bytes chunks;
    put(chunks, fmt);
    if (s.infoTag) {
        Bytes list;
        put(list, "INFO");
        put(list, "INAM"); be32(list, 5); put(list, "Song"); list.push_back(0);
        put(chunks, "LIST"); be32(chunks, static_cast<uint32_t>(list.size())); put(chunks, list);
        if (list.size() % 2 == 1) chunks.push_back(0);   // RIFF/RIFX chunks are padded to an even length
    }
    put(chunks, "data"); be32(chunks, static_cast<uint32_t>(pcm.size()));
    put(chunks, pcm);
    Bytes out;
    put(out, "RIFX"); be32(out, static_cast<uint32_t>(4 + chunks.size()));   // 'WAVE' plus all chunks
    put(out, "WAVE");
    put(out, chunks);
    return out;
}

}  // namespace

TEST_CASE("RIFX: the builder produces a big endian RIFF header", "[wav][rifx][spec][selftest]")
{
    const Bytes f = rifxFile(RifxSpec());
    REQUIRE(f.size() > 44);
    CHECK(std::memcmp(f.data(), "RIFX", 4) == 0);
    CHECK(std::memcmp(f.data() + 8, "WAVEfmt ", 8) == 0);
}

TEST_CASE("RIFX: sample rate, channels, duration and bit rate come from the big endian fmt chunk", "[wav][rifx][spec]")
{
    SECTION("stereo, 16 bit") {
        RifxSpec s; s.sampleFrames = 44100;
        auto p = writeTemp("rifx_stereo.wav", rifxFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
        CHECK(WAVGetBitsPerSampleW() == 16);
    }
    SECTION("mono, 8 bit, another sample rate") {
        RifxSpec s; s.channels = 1; s.bits = 8; s.rate = 22050; s.sampleFrames = 22050 * 2;
        auto p = writeTemp("rifx_mono8.wav", rifxFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(AUDIOGetChannelsW() == 1);
        CHECK(WAVGetBitsPerSampleW() == 8);
        CHECK(std::fabs(AUDIOGetDurationW() - 2.0) < 0.001);
    }
    SECTION("24 bit") {
        RifxSpec s; s.bits = 24; s.sampleFrames = 48000;
        auto p = writeTemp("rifx_24bit.wav", rifxFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(WAVGetBitsPerSampleW() == 24);
        CHECK(std::fabs(AUDIOGetDurationW() - 48000.0 / 44100) < 0.001);
    }
}

TEST_CASE("RIFX: the LIST/INFO tag is read despite the big endian chunk sizes", "[wav][rifx][spec]")
{
    RifxSpec s; s.infoTag = true;
    auto p = writeTemp("rifx_info.wav", rifxFile(s));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"Song");
}

TEST_CASE("RIFX: a data chunk that reaches past the real end of the file ends at the end of the file", "[wav][rifx][spec]")
{
    RifxSpec s; s.sampleFrames = 1000;
    Bytes f = rifxFile(s);
    // corrupt the (big endian) 'data' size to claim more bytes than the file actually has
    const size_t dataSizePos = f.size() - pcmSamples(1000, 2, 16).size() - 4;
    REQUIRE(std::memcmp(f.data() + dataSizePos - 4, "data", 4) == 0);
    f[dataSizePos] = 0x7F; f[dataSizePos + 1] = 0xFF; f[dataSizePos + 2] = 0xFF; f[dataSizePos + 3] = 0xFF;
    // the audio data reach to the end of the file (a recording that was cut off), as players read them
    auto p = writeTemp("rifx_huge_promise.wav", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(AUDIOIsValidFormatW() != 0);
    CHECK(std::fabs(AUDIOGetDurationW() - 1000.0 / 44100) < 0.001);
}

TEST_CASE("RIFX: writing tags keeps the format RIFX and the audio data untouched", "[wav][rifx][spec][write]")
{
    RifxSpec s; s.sampleFrames = 20000; s.infoTag = true;
    const Bytes f = rifxFile(s);
    auto p = writeTemp("rifx_write.wav", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"Song");
    WAVSetTextFrameW(WAV_IART, std::wstring(2000, L'a').c_str());
    REQUIRE(WAVSaveChangesW() != 0);
    const Bytes g = readFile(p);
    // still RIFX (not downgraded to plain little endian RIFF), still analyzable, with the new tag
    CHECK(std::memcmp(g.data(), "RIFX", 4) == 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetTextFrameW(WAV_IART)).size() == 2000);
    CHECK(std::fabs(AUDIOGetDurationW() - 20000.0 / 44100) < 0.001);
    // the audio bytes themselves are unchanged, wherever they ended up
    auto find = [](const Bytes& hay, const Bytes& needle) {
        return std::search(hay.begin(), hay.end(), needle.begin(), needle.end()) != hay.end();
    };
    const Bytes pcm = pcmSamples(20000, 2, 16);
    CHECK(find(g, pcm));
}

TEST_CASE("RIFX: a plain little endian RIFF file right after a RIFX one is still read correctly", "[wav][rifx][spec]")
{
    // the endianness flag is a static shared across all WAV chunk objects (RIFX/RIFF sibling chunks have no back reference to the
    // container that first detects it); make sure a RIFF file analyzed right after a RIFX one is not misread as big endian
    auto rifx = writeTemp("rifx_then_riff_a.wav", rifxFile(RifxSpec()));
    REQUIRE(AUDIOAnalyzeFileW(rifx.c_str()) == WAV);
    CHECK(AUDIOGetSampleRateW() == 44100);

    Bytes little;
    put(little, "RIFF");
    // little endian equivalent, built directly (not reusing the RF64 test's helpers to keep this file self contained)
    Bytes fmt;
    put(fmt, "fmt "); for (int i = 0; i < 4; i++) fmt.push_back(static_cast<uint8_t>((16 >> (8 * i)) & 0xFF));
    fmt.push_back(1); fmt.push_back(0);
    fmt.push_back(2); fmt.push_back(0);
    for (int i = 0; i < 4; i++) fmt.push_back(static_cast<uint8_t>((22050 >> (8 * i)) & 0xFF));
    const uint32_t bps = 22050 * 4;
    for (int i = 0; i < 4; i++) fmt.push_back(static_cast<uint8_t>((bps >> (8 * i)) & 0xFF));
    fmt.push_back(4); fmt.push_back(0);
    fmt.push_back(16); fmt.push_back(0);
    const Bytes pcm = pcmSamples(22050, 2, 16);
    Bytes body;
    put(body, fmt);
    put(body, "data"); for (int i = 0; i < 4; i++) body.push_back(static_cast<uint8_t>((pcm.size() >> (8 * i)) & 0xFF));
    put(body, pcm);
    for (int i = 0; i < 4; i++) little.push_back(static_cast<uint8_t>(((4 + body.size()) >> (8 * i)) & 0xFF));
    put(little, "WAVE");
    put(little, body);
    auto p = writeTemp("rifx_then_riff_b.wav", little);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(AUDIOGetSampleRateW() == 22050);
    CHECK(AUDIOGetChannelsW() == 2);
    CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
}
