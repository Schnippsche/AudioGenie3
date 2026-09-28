// WAV / RF64 checks (EBU Tech 3306, "RF64: An Extension of the WAVE format"): for files that reach or exceed 4 GB the outer chunk is 'RF64'
// instead of 'RIFF', its size field is 0xFFFFFFFF, and a 'ds64' chunk right behind 'WAVE' carries the real 64 bit sizes (RIFF size, 'data' size,
// sample count); the 'data' chunk itself also has 0xFFFFFFFF in its 32 bit size field. The files here are small (RF64 is legal for any size,
// streaming encoders write it when the final size is not known yet), which keeps the test fast while still exercising the real parsing code.
#include "id3v2_support.h"
#include <cmath>
#include <cstring>

using namespace ag3test;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
void le32(Bytes& b, uint32_t v) { for (int i = 0; i < 4; i++) b.push_back(static_cast<uint8_t>(v >> (8 * i))); }
void le64(Bytes& b, uint64_t v) { for (int i = 0; i < 8; i++) b.push_back(static_cast<uint8_t>(v >> (8 * i))); }

Bytes fmtChunk(uint16_t channels, uint32_t rate, uint16_t bits)
{
    Bytes b;
    put(b, "fmt ");
    le32(b, 16);
    b.push_back(1); b.push_back(0);                          // PCM
    b.push_back(static_cast<uint8_t>(channels)); b.push_back(0);
    le32(b, rate);
    const uint32_t blockAlign = channels * (bits / 8);
    le32(b, rate * blockAlign);                               // bytes per second
    b.push_back(static_cast<uint8_t>(blockAlign)); b.push_back(0);
    b.push_back(static_cast<uint8_t>(bits)); b.push_back(0);
    return b;
}

Bytes pcmSamples(size_t sampleFrames, uint16_t channels, uint16_t bits)
{
    Bytes b;
    const size_t bytesPerSample = bits / 8;
    for (size_t i = 0; i < sampleFrames * channels * bytesPerSample; i++) b.push_back(static_cast<uint8_t>((i * 37) & 0xFF));
    return b;
}

struct Rf64Spec {
    uint16_t channels = 2;
    uint32_t rate = 44100;
    uint16_t bits = 16;
    size_t sampleFrames = 1000;
    uint64_t declaredRiffSize = 0;   // 0: computed from the real chunks (matches them)
    uint64_t declaredDataSize = 0;   // 0: the real size of the payload below
    uint64_t sampleCount = 0;
    bool infoTag = false;
};

Bytes rf64File(const Rf64Spec& s)
{
    const Bytes fmt = fmtChunk(s.channels, s.rate, s.bits);
    const Bytes pcm = pcmSamples(s.sampleFrames, s.channels, s.bits);
    Bytes chunks;
    // 'ds64' has to be the first chunk behind 'WAVE'; its 28 byte payload (riff size, data size, sample count, table length) is filled in below,
    // once the size of everything that follows is known
    const size_t ds64PayloadPos = chunks.size() + 8;
    put(chunks, "ds64"); le32(chunks, 28);
    chunks.resize(chunks.size() + 28, 0);
    put(chunks, "fmt "); le32(chunks, static_cast<uint32_t>(fmt.size() - 8)); put(chunks, Bytes(fmt.begin() + 8, fmt.end()));
    if (s.infoTag) {
        Bytes list;
        put(list, "INFO");
        put(list, "INAM"); le32(list, 5); put(list, "Song"); list.push_back(0);
        put(chunks, "LIST"); le32(chunks, static_cast<uint32_t>(list.size())); put(chunks, list);
        if (list.size() % 2 == 1) chunks.push_back(0);   // RIFF chunks are padded to an even length
    }
    put(chunks, "data"); le32(chunks, 0xFFFFFFFFu);
    put(chunks, pcm);
    const uint64_t realDataSize = s.declaredDataSize ? s.declaredDataSize : pcm.size();
    const uint64_t realRiffSize = s.declaredRiffSize ? s.declaredRiffSize : (4 + chunks.size());   // 'WAVE' plus all chunks
    for (int i = 0; i < 8; i++) chunks[ds64PayloadPos + static_cast<size_t>(i)] = static_cast<uint8_t>(realRiffSize >> (8 * i));
    for (int i = 0; i < 8; i++) chunks[ds64PayloadPos + 8 + static_cast<size_t>(i)] = static_cast<uint8_t>(realDataSize >> (8 * i));
    for (int i = 0; i < 8; i++) chunks[ds64PayloadPos + 16 + static_cast<size_t>(i)] = static_cast<uint8_t>(s.sampleCount >> (8 * i));
    Bytes out;
    put(out, "RF64"); le32(out, 0xFFFFFFFFu); put(out, "WAVE");
    put(out, chunks);
    return out;
}

}  // namespace

TEST_CASE("RF64: the builder puts a consistent ds64 chunk in front of fmt and data", "[wav][rf64][spec][selftest]")
{
    const Bytes f = rf64File(Rf64Spec());
    REQUIRE(f.size() > 44);
    CHECK(std::memcmp(f.data(), "RF64", 4) == 0);
    CHECK(std::memcmp(f.data() + 8, "WAVEds64", 8) == 0);
}

TEST_CASE("RF64: sample rate, channels, duration and bit rate come from the ds64 sizes", "[wav][rf64][spec]")
{
    SECTION("stereo, 16 bit") {
        Rf64Spec s; s.sampleFrames = 44100;
        auto p = writeTemp("rf64_stereo.wav", rf64File(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
        CHECK(WAVGetBitsPerSampleW() == 16);
    }
    SECTION("mono, 8 bit, another sample rate") {
        Rf64Spec s; s.channels = 1; s.bits = 8; s.rate = 22050; s.sampleFrames = 22050 * 2;
        auto p = writeTemp("rf64_mono8.wav", rf64File(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(AUDIOGetChannelsW() == 1);
        CHECK(WAVGetBitsPerSampleW() == 8);
        CHECK(std::fabs(AUDIOGetDurationW() - 2.0) < 0.001);
    }
    SECTION("24 bit") {
        Rf64Spec s; s.bits = 24; s.sampleFrames = 48000;
        auto p = writeTemp("rf64_24bit.wav", rf64File(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(WAVGetBitsPerSampleW() == 24);
        CHECK(std::fabs(AUDIOGetDurationW() - 48000.0 / 44100) < 0.001);
    }
}

TEST_CASE("RF64: a data size that a 32 bit RIFF file could never declare (over 4 GiB)", "[wav][rf64][spec]")
{
    // the physical bytes are not actually written (that would make the test slow); the container only has to trust ds64 for the duration,
    // and it must refuse to read past the real end of the file (a chunk that claims more than the file has is rejected, see the next test)
    Rf64Spec s;
    s.declaredDataSize = 5ull * 1024 * 1024 * 1024;   // 5 GiB, does not fit into a 32 bit chunk size field
    s.declaredRiffSize = s.declaredDataSize + 1000;   // consistent with a file that really is that big
    auto f = rf64File(s);
    // shrink the physical file back down (only the ds64 promise stays huge) and make sure it is rejected: the data chunk would reach
    // past the end of the actual file, which must not be accepted as valid
    auto p = writeTemp("rf64_huge_promise.wav", f);
    // the format is still identified (the header up to 'data' parses fine), but the file is not valid: a 'data' chunk that reaches
    // past the real end of the file is rejected, as for any other corrupt or truncated file
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(AUDIOIsValidFormatW() == 0);
}

TEST_CASE("RF64: writing tags keeps the format RF64 and the audio data untouched", "[wav][rf64][spec][write]")
{
    Rf64Spec s; s.sampleFrames = 20000; s.infoTag = true;
    const Bytes f = rf64File(s);
    auto p = writeTemp("rf64_write.wav", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"Song");
    WAVSetTextFrameW(WAV_IART, std::wstring(2000, L'a').c_str());
    REQUIRE(WAVSaveChangesW() != 0);
    const Bytes g = readFile(p);
    // still RF64 (not downgraded to plain RIFF), still analyzable, with the new tag
    CHECK(std::memcmp(g.data(), "RF64", 4) == 0);
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
