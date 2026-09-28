// True Audio checks: the header of TTA1 (22 bytes with the CRC-32) and of TTA2 (36 bytes, 64 bit sample counter), built byte by byte, and real
// files of ffmpeg with 6 channels and a sample rate that is not usual.
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
void le64(Bytes& b, uint64_t v) { le32(b, static_cast<uint32_t>(v)); le32(b, static_cast<uint32_t>(v >> 32)); }

uint32_t crc32(const uint8_t* d, size_t n)
{
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < n; i++) {
        crc ^= d[i];
        for (int k = 0; k < 8; k++) crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320u : (crc >> 1);
    }
    return ~crc;
}

Bytes tta1(uint32_t channels, uint32_t bits, uint32_t rate, uint32_t samples, size_t audio = 4000)
{
    Bytes b;
    put(b, "TTA1");
    le16(b, 1);   // simple format
    le16(b, channels); le16(b, bits); le32(b, rate); le32(b, samples);
    le32(b, crc32(b.data(), b.size()));
    b.resize(b.size() + audio, 0x4B);
    return b;
}

Bytes tta2(uint32_t channels, uint32_t bits, uint32_t rate, uint64_t samples, size_t audio = 4000)
{
    Bytes b;
    put(b, "TTA2");
    le16(b, channels); le16(b, bits); le32(b, rate);
    le32(b, 0x3F);        // channel mask
    le64(b, samples);
    le64(b, 0);           // data block size
    le32(b, crc32(b.data(), b.size()));
    b.resize(b.size() + audio, 0x4B);
    return b;
}

fs::path copyFixture(const char* rel)
{
    const fs::path dst = tempDir() / fs::path(rel).filename();
    fs::copy_file(fs::path(AG3_FIXTURES_DIR) / rel, dst, fs::copy_options::overwrite_existing);
    return dst;
}

}  // namespace

TEST_CASE("TTA1: channels, sample rate, duration and channel mode", "[tta][spec]")
{
    SECTION("stereo, 6.3 seconds") {
        auto p = writeTemp("tta1_stereo.tta", tta1(2, 16, 44100, 277830));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(std::fabs(AUDIOGetDurationW() - 6.3) < 0.001);
        CHECK(take(AUDIOGetChannelModeW()) == L"Stereo");
        CHECK(take(AUDIOGetVersionW()) == L"TTA1");
    }
    SECTION("mono, 6 channels and 8 channels") {
        auto p = writeTemp("tta1_mono.tta", tta1(1, 16, 48000, 48000));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(take(AUDIOGetChannelModeW()) == L"Mono");
        for (uint32_t ch : { 6u, 8u }) {
            p = writeTemp("tta1_multi.tta", tta1(ch, 24, 44100, 44100));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
            CHECK(AUDIOGetChannelsW() == static_cast<long>(ch));
            CHECK(take(AUDIOGetChannelModeW()) == L"Multi Channel");
        }
    }
    SECTION("more than 2^31 samples: 32 bit without sign") {
        auto p = writeTemp("tta1_long.tta", tta1(2, 16, 44100, 3000000000u));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(std::fabs(AUDIOGetDurationW() / (3000000000.0 / 44100) - 1.0) < 1e-5);
    }
}

TEST_CASE("TTA2: 64 bit sample counter, zero means unknown", "[tta][spec]")
{
    SECTION("a counter of more than 2^32") {
        auto p = writeTemp("tta2_long.tta", tta2(2, 24, 96000, (1ull << 33) + 1234));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(AUDIOGetSampleRateW() == 96000);
        CHECK(std::fabs(AUDIOGetDurationW() / (((1ull << 33) + 1234) / 96000.0) - 1.0) < 1e-5);
        CHECK(take(AUDIOGetVersionW()) == L"TTA2");
    }
    SECTION("a normal file") {
        auto p = writeTemp("tta2_normal.tta", tta2(2, 16, 44100, 88200));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(std::fabs(AUDIOGetDurationW() - 2.0) < 0.001);
        CHECK(AUDIOGetChannelsW() == 2);
    }
    SECTION("the length 0 (unknown): the duration is 0, no crash") {
        auto p = writeTemp("tta2_unknown.tta", tta2(2, 16, 44100, 0));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(AUDIOGetDurationW() == 0.0f);
    }
}

TEST_CASE("TTA: invalid headers", "[tta][spec]")
{
    SECTION("zero values") {
        const Bytes files[] = { tta1(0, 16, 44100, 1000), tta1(2, 0, 44100, 1000), tta1(2, 16, 0, 1000), tta1(2, 16, 44100, 0), tta1(2, 33, 44100, 1000) };
        for (const Bytes& f : files) {
            auto p = writeTemp("tta_zero.tta", f);
            CHECK(AUDIOAnalyzeFileW(p.c_str()) != TTA);
        }
    }
    SECTION("the file ends inside the header") {
        const Bytes f = tta1(2, 16, 44100, 1000);
        for (size_t n : { size_t(3), size_t(10), size_t(21) }) {
            auto p = writeTemp("tta_cut.tta", Bytes(f.begin(), f.begin() + static_cast<std::ptrdiff_t>(n)));
            CHECK(AUDIOAnalyzeFileW(p.c_str()) != TTA);
        }
    }
    SECTION("another version") {
        Bytes f = tta1(2, 16, 44100, 1000);
        f[3] = '3';
        auto p = writeTemp("tta_v3.tta", f);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != TTA);
    }
}

TEST_CASE("TTA: real files of ffmpeg", "[tta][spec]")
{
    SECTION("6 channels, 24 bit") {
        const fs::path p = copyFixture("tta/ch6.tta");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(AUDIOGetChannelsW() == 6);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(std::fabs(AUDIOGetDurationW() - 0.3) < 0.001);
        CHECK(take(AUDIOGetChannelModeW()) == L"Multi Channel");
    }
    SECTION("37000 Hz") {
        const fs::path p = copyFixture("tta/rate37000.tta");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == TTA);
        CHECK(AUDIOGetSampleRateW() == 37000);
        CHECK(std::fabs(AUDIOGetDurationW() - 0.5) < 0.001);
    }
}
