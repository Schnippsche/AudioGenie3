// Musepack checks: the stream version 7 header (24 bytes) and the packets of the stream version 8, built byte by byte, plus the real files of
// mppenc and mpcenc in the fixtures.
#include "id3v2_support.h"
#include <cmath>
#include <filesystem>
#include <string>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }

// SV7: signature and version, frames (32 bit), max level (16), profile (4), link (2), sample frequency (2), intensity stereo, MS stereo, max band (6), ...
// the word at offset 20: the last frame length in its upper 11 bits
struct Sv7 {
    uint8_t version = 0x07;
    uint32_t frames = 100;
    int profile = 10;                 // standard
    int rateIndex = 0;                // 44100
    bool joint = true;
    uint32_t lastFrame = 0;           // 0: not set
    size_t audioBytes = 10000;
};

Bytes sv7(const Sv7& s)
{
    Bytes b(24, 0);
    b[0] = 'M'; b[1] = 'P'; b[2] = '+'; b[3] = s.version;
    for (int i = 0; i < 4; i++) b[4 + static_cast<size_t>(i)] = static_cast<uint8_t>(s.frames >> (8 * i));
    b[10] = static_cast<uint8_t>((s.profile << 4) | s.rateIndex);
    b[11] = s.joint ? 0x40 : 0x00;
    const uint32_t word = s.lastFrame << 20;
    for (int i = 0; i < 4; i++) b[20 + static_cast<size_t>(i)] = static_cast<uint8_t>(word >> (8 * i));
    b.resize(b.size() + s.audioBytes, 0x37);
    return b;
}

// variable length integer of SV8: 7 bits per byte, the upper bit says that another byte follows
Bytes varint(uint64_t v, size_t minBytes = 1)
{
    Bytes groups;
    do { groups.insert(groups.begin(), static_cast<uint8_t>(v & 0x7F)); v >>= 7; } while (v);
    while (groups.size() < minBytes) groups.insert(groups.begin(), 0);
    for (size_t i = 0; i + 1 < groups.size(); i++) groups[i] |= 0x80;
    return groups;
}

Bytes packet(const char* key, const Bytes& payload, size_t sizeBytes = 1)
{
    // the size includes the key and the size field itself
    Bytes size;
    for (size_t total = 2 + sizeBytes + payload.size();; ) {
        size = varint(total, sizeBytes);
        if (2 + size.size() + payload.size() == total) break;
        total = 2 + size.size() + payload.size();
    }
    Bytes b;
    put(b, key);
    b.insert(b.end(), size.begin(), size.end());
    b.insert(b.end(), payload.begin(), payload.end());
    return b;
}

struct Sv8 {
    uint64_t samples = 44100;
    uint64_t silence = 0;
    int rateIndex = 0;
    int channels = 2;
    bool midSide = false;
    int profileByte = 0xA0;           // profile 10 in the upper 4 bits of byte 0 of the EI packet: standard
    bool withEI = true;
    bool withSH = true;
    size_t samplesBytes = 1;
    size_t audioBytes = 8000;
    bool noiseFirst = false;          // a packet that is not known in front of the SH packet
};

Bytes sv8(const Sv8& s)
{
    Bytes out;
    put(out, "MPCK");
    if (s.noiseFirst) out.insert(out.end(), { 'R', 'G', 6, 0, 0, 0 });    // key, size (2 + 1 + 3 bytes), 3 bytes payload
    if (s.withSH) {
        Bytes sh = { 0, 0, 0, 0, 8 };                                    // crc (not checked), stream version
        const Bytes n = varint(s.samples, s.samplesBytes), sil = varint(s.silence);
        sh.insert(sh.end(), n.begin(), n.end());
        sh.insert(sh.end(), sil.begin(), sil.end());
        sh.push_back(static_cast<uint8_t>(s.rateIndex << 5));            // sample frequency (3 bits), max used bands (5 bits)
        sh.push_back(static_cast<uint8_t>(((s.channels - 1) << 4) | (s.midSide ? 8 : 0) | 4));   // channels - 1 (4 bits), MS, audio block frames
        const Bytes p = packet("SH", sh);
        out.insert(out.end(), p.begin(), p.end());
    }
    if (s.withEI) {
        const Bytes p = packet("EI", { static_cast<uint8_t>(s.profileByte), 1, 30, 0 });
        out.insert(out.end(), p.begin(), p.end());
    }
    const Bytes ap = packet("AP", Bytes(s.audioBytes, 0x5A), 2);
    out.insert(out.end(), ap.begin(), ap.end());
    const Bytes se = packet("SE", {});
    out.insert(out.end(), se.begin(), se.end());
    return out;
}

fs::path copyFixture(const char* rel)
{
    const fs::path dst = tempDir() / fs::path(rel).filename();
    fs::copy_file(fs::path(AG3_FIXTURES_DIR) / rel, dst, fs::copy_options::overwrite_existing);
    return dst;
}

}  // namespace

TEST_CASE("Musepack SV7: frames, last frame length, sample rate, channel mode", "[mpc][spec]")
{
    SECTION("the duration is (frames - 1) * 1152 + the last frame") {
        for (uint32_t last : { 0u, 1u, 324u, 1152u }) {
            INFO("last frame " << last);
            Sv7 s; s.frames = 39; s.lastFrame = last;
            auto p = writeTemp("mpc_sv7_last.mpc", sv7(s));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
            const double samples = 38 * 1152.0 + (last == 0 ? 1152 : last);
            CHECK(std::fabs(AUDIOGetDurationW() - samples / 44100.0) < 0.0001);
        }
    }
    SECTION("sample rates 44100, 48000, 37800 and 32000") {
        const int rates[4] = { 44100, 48000, 37800, 32000 };
        for (int i = 0; i < 4; i++) {
            Sv7 s; s.rateIndex = i; s.frames = 100; s.lastFrame = 500;
            auto p = writeTemp("mpc_sv7_rate.mpc", sv7(s));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
            CHECK(AUDIOGetSampleRateW() == rates[i]);
            CHECK(std::fabs(AUDIOGetDurationW() - (99 * 1152.0 + 500) / rates[i]) < 0.0001);
        }
    }
    SECTION("the bit rate is the size of the audio data per second, also with 37800 Hz") {
        for (int i : { 0, 2 }) {
            Sv7 s; s.rateIndex = i; s.frames = 500; s.audioBytes = 200000;
            const Bytes f = sv7(s);
            auto p = writeTemp("mpc_sv7_bitrate.mpc", f);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
            const double bits = static_cast<double>(f.size()) * 8.0;
            CHECK(AUDIOGetBitrateW() == static_cast<long>(bits / AUDIOGetDurationW() / 1000.0 + 0.5));
        }
    }
    SECTION("stereo, joint stereo and the stream versions 7 and 7.1") {
        Sv7 s; s.joint = false;
        auto p = writeTemp("mpc_sv7_stereo.mpc", sv7(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(MPPGetStreamVersionW() == 7);
        Sv7 j; j.joint = true; j.version = 0x17;
        p = writeTemp("mpc_sv71_joint.mpc", sv7(j));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(MPPGetStreamVersionW() == 71);
    }
    SECTION("all profiles") {
        const struct { int index; const wchar_t* name; } profiles[] = { { 1, L"Experimental" }, { 5, L"--quality 0" }, { 6, L"--quality 1" }, { 7, L"Telephone" },
            { 8, L"Thumb" }, { 9, L"Radio" }, { 10, L"Standard" }, { 11, L"Xtreme" }, { 12, L"Insane" }, { 13, L"BrainDead" }, { 14, L"--quality 9" }, { 15, L"--quality 10" } };
        for (const auto& pr : profiles) {
            INFO("profile " << pr.index);
            Sv7 s; s.profile = pr.index;
            auto p = writeTemp("mpc_sv7_profile.mpc", sv7(s));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
            CHECK(take(AUDIOGetVersionW()) == pr.name);
        }
    }
    SECTION("the file ends inside the header") {
        const Bytes f = sv7(Sv7());
        auto p = writeTemp("mpc_sv7_cut.mpc", Bytes(f.begin(), f.begin() + 8));
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != MPEGPLUS);
    }
}

TEST_CASE("Musepack SV8: stream header packet, encoder info, variable length integers", "[mpc][spec]")
{
    SECTION("samples, sample rate, channels") {
        Sv8 s; s.samples = 44100 * 3 + 500;
        auto p = writeTemp("mpc_sv8.mpc", sv8(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(MPPGetStreamVersionW() == 8);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(std::fabs(AUDIOGetDurationW() - (44100 * 3 + 500) / 44100.0) < 0.0001);
        CHECK(take(AUDIOGetVersionW()) == L"Standard");
    }
    SECTION("the beginning silence is not counted") {
        Sv8 s; s.samples = 100000; s.silence = 1481;
        auto p = writeTemp("mpc_sv8_silence.mpc", sv8(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(std::fabs(AUDIOGetDurationW() - (100000 - 1481) / 44100.0) < 0.0001);
    }
    SECTION("sample rate indices 0 to 3; 4 to 7 are reserved") {
        const int rates[4] = { 44100, 48000, 37800, 32000 };
        for (int i = 0; i < 8; i++) {
            INFO("index " << i);
            Sv8 s; s.rateIndex = i;
            auto p = writeTemp("mpc_sv8_rate.mpc", sv8(s));
            if (i < 4) {
                REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
                CHECK(AUDIOGetSampleRateW() == rates[i]);
            } else {
                CHECK(AUDIOAnalyzeFileW(p.c_str()) != MPEGPLUS);
            }
        }
    }
    SECTION("channels: 1 and 2 are valid, 3 is not") {
        for (int ch : { 1, 2, 3 }) {
            INFO("channels " << ch);
            Sv8 s; s.channels = ch;
            auto p = writeTemp("mpc_sv8_channels.mpc", sv8(s));
            if (ch <= 2) {
                REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
                CHECK(AUDIOGetChannelsW() == ch);
            } else {
                CHECK(AUDIOAnalyzeFileW(p.c_str()) != MPEGPLUS);
            }
        }
    }
    SECTION("the sample counter is a variable length integer of several bytes") {
        Sv8 s; s.samples = 5000000000ull;   // 5 bytes
        auto p = writeTemp("mpc_sv8_long.mpc", sv8(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(std::fabs(AUDIOGetDurationW() / (5000000000.0 / 44100) - 1.0) < 1e-5);
        Sv8 t; t.samples = 44100; t.samplesBytes = 4;           // padded with leading zero groups
        p = writeTemp("mpc_sv8_padded.mpc", sv8(t));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.0001);
    }
    SECTION("unknown packets in front of the stream header are skipped") {
        Sv8 s; s.noiseFirst = true;
        auto p = writeTemp("mpc_sv8_noise.mpc", sv8(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(AUDIOGetSampleRateW() == 44100);
    }
    SECTION("no stream header: no Musepack file") {
        Sv8 s; s.withSH = false;
        auto p = writeTemp("mpc_sv8_nosh.mpc", sv8(s));
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != MPEGPLUS);
    }
    SECTION("the bit rate is the size of the audio data per second") {
        Sv8 s; s.samples = 44100 * 10; s.audioBytes = 400000;
        const Bytes f = sv8(s);
        auto p = writeTemp("mpc_sv8_bitrate.mpc", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(AUDIOGetBitrateW() == static_cast<long>(static_cast<double>(f.size()) * 8.0 / 10.0 / 1000.0 + 0.5));
    }
}

TEST_CASE("Musepack: real files of mppenc (SV7) and mpcenc (SV8)", "[mpc][spec]")
{
    SECTION("SV7: the last frame is 324 samples, the file has exactly 44100 samples") {
        const fs::path p = copyFixture("mpc/sv7_standard.mpc");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.0001);
    }
    SECTION("SV7 with other sample rates") {
        const fs::path a = copyFixture("mpc/sv7_thumb_32k.mpc");
        REQUIRE(AUDIOAnalyzeFileW(a.c_str()) == MPEGPLUS);
        CHECK(AUDIOGetSampleRateW() == 32000);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.0001);
        const fs::path b = copyFixture("mpc/sv7_thumb_48k.mpc");
        REQUIRE(AUDIOAnalyzeFileW(b.c_str()) == MPEGPLUS);
        CHECK(AUDIOGetSampleRateW() == 48000);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.0001);
    }
    SECTION("SV8") {
        const fs::path p = copyFixture("mpc/sv8_standard.mpc");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEGPLUS);
        CHECK(MPPGetStreamVersionW() == 8);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.01);
    }
}
