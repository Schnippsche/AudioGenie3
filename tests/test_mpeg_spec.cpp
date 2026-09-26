// MPEG audio frame header and Xing / Info / VBRI header checks with synthetic files. The expected values are calculated from the
// formulas of the MPEG audio specification (ISO/IEC 11172-3 and 13818-3), not with the code of the DLL.
#include "id3v2_support.h"
#include <cmath>
#include <string>

using namespace ag3test;

namespace {

enum Version { V1, V2, V25 };


// bit rates in kbit/s for the indices 1 to 14
int bitrateOf(Version v, int layer, int index)
{
    static const int v1l1[14] = { 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 };
    static const int v1l2[14] = { 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384 };
    static const int v1l3[14] = { 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 };
    static const int v2l1[14] = { 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 };
    static const int v2l23[14] = { 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 };
    if (v == V1) return layer == 1 ? v1l1[index - 1] : (layer == 2 ? v1l2[index - 1] : v1l3[index - 1]);
    return layer == 1 ? v2l1[index - 1] : v2l23[index - 1];
}

int sampleRateOf(Version v, int index)
{
    static const int r[3][3] = { { 44100, 48000, 32000 }, { 22050, 24000, 16000 }, { 11025, 12000, 8000 } };
    return r[v][index];
}

int samplesPerFrame(Version v, int layer)
{
    if (layer == 1) return 384;
    if (layer == 2) return 1152;
    return v == V1 ? 1152 : 576;
}

// frame length in bytes (padding: 0 or 1 slot); layer 1 has slots of 4 bytes
int frameLength(Version v, int layer, int bitrate, int sampleRate, int padding)
{
    if (layer == 1) return (12 * bitrate * 1000 / sampleRate + padding) * 4;
    const int coefficient = (layer == 3 && v != V1) ? 72 : 144;
    return coefficient * bitrate * 1000 / sampleRate + padding;
}

struct Spec {
    Version version = V1;
    int layer = 3;
    int bitrateIndex = 9;    // 1..14
    int sampleRateIndex = 0; // 0..2
    int mode = 0;            // 0 stereo, 1 joint stereo, 2 dual channel, 3 mono
    bool crc = false;        // protection bit 0 = 16 bit CRC follows the header
    int paddingPattern = 0;  // 0 never, 1 always, 2 every second frame
    bool priv = false, copyright = false, original = false;
    int emphasis = 0;
};

Bytes headerOf(const Spec& s, int padding)
{
    const int verBits = s.version == V1 ? 3 : (s.version == V2 ? 2 : 0);
    const int layerBits = s.layer == 1 ? 3 : (s.layer == 2 ? 2 : 1);
    Bytes h(4);
    h[0] = 0xFF;
    h[1] = static_cast<uint8_t>(0xE0 | (verBits << 3) | (layerBits << 1) | (s.crc ? 0 : 1));
    h[2] = static_cast<uint8_t>((s.bitrateIndex << 4) | (s.sampleRateIndex << 2) | (padding << 1) | (s.priv ? 1 : 0));
    h[3] = static_cast<uint8_t>((s.mode << 6) | ((s.copyright ? 1 : 0) << 3) | ((s.original ? 1 : 0) << 2) | s.emphasis);
    return h;
}

int paddingOf(const Spec& s, int frameNumber)
{
    return s.paddingPattern == 0 ? 0 : (s.paddingPattern == 1 ? 1 : frameNumber % 2);
}

int lengthOf(const Spec& s, int frameNumber)
{
    return frameLength(s.version, s.layer, bitrateOf(s.version, s.layer, s.bitrateIndex), sampleRateOf(s.version, s.sampleRateIndex), paddingOf(s, frameNumber));
}

Bytes frameOf(const Spec& s, int frameNumber)
{
    Bytes f = headerOf(s, paddingOf(s, frameNumber));
    if (s.crc) { f.push_back(0); f.push_back(0); }
    f.resize(static_cast<size_t>(lengthOf(s, frameNumber)), 0);
    return f;
}

Bytes framesOf(const Spec& s, int count, int firstNumber = 0)
{
    Bytes b;
    for (int i = 0; i < count; i++) {
        const Bytes f = frameOf(s, firstNumber + i);
        b.insert(b.end(), f.begin(), f.end());
    }
    return b;
}

std::wstring versionText(Version v) { return v == V1 ? L"MPEG 1.0" : (v == V2 ? L"MPEG 2.0" : L"MPEG 2.5"); }

// exact frame scan (configuration MPEGEXACTREAD)
struct ExactRead {
    long old;
    explicit ExactRead(bool on) : old(GetConfigValueW(0)) { SetConfigValueW(0, on ? 1 : 0); }
    ~ExactRead() { SetConfigValueW(0, old); }
};

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void putBE32(Bytes& b, uint32_t v) { b.push_back(static_cast<uint8_t>(v >> 24)); b.push_back(static_cast<uint8_t>(v >> 16)); b.push_back(static_cast<uint8_t>(v >> 8)); b.push_back(static_cast<uint8_t>(v)); }
void putBE16(Bytes& b, uint32_t v) { b.push_back(static_cast<uint8_t>(v >> 8)); b.push_back(static_cast<uint8_t>(v)); }

// side information of layer 3 (bytes between the header and the Xing/Info tag)
int sideInfoSize(Version v, int mode)
{
    if (v == V1) return mode == 3 ? 17 : 32;
    return mode == 3 ? 9 : 17;
}

}  // namespace

TEST_CASE("MPEG frame headers: version, layer, bit rate, sample rate, length and duration", "[mpeg][spec][frames]")
{
    int combinations = 0;
    for (Version v : { V1, V2, V25 })
        for (int layer = 1; layer <= 3; layer++)
            for (int sr = 0; sr < 3; sr++)
                for (int bi : { 1, 5, 14 })
                    for (int pad = 0; pad < 3; pad++) {
                        Spec s;
                        s.version = v; s.layer = layer; s.sampleRateIndex = sr; s.bitrateIndex = bi; s.paddingPattern = pad;
                        s.mode = (layer == 1 || bi == 5) ? 1 : 0;
                        const int bitrate = bitrateOf(v, layer, bi);
                        const int rate = sampleRateOf(v, sr);
                        const int frames = 24;
                        const std::string label = "MPEG " + std::string(v == V1 ? "1" : (v == V2 ? "2" : "2.5")) + " layer " + std::to_string(layer) + " " +
                                                  std::to_string(bitrate) + " kbit/s " + std::to_string(rate) + " Hz padding " + std::to_string(pad);
                        INFO(label);
                        auto p = writeTemp("mpegspec_frames.mp3", framesOf(s, frames));
                        combinations++;

                        for (bool exact : { false, true }) {
                            ExactRead er(exact);
                            const bool recognized = (AUDIOAnalyzeFileW(p.c_str()) == MPEG);
                            CHECK(recognized);
                            if (!recognized) continue;
                            CHECK(AUDIOGetSampleRateW() == rate);
                            CHECK(AUDIOGetBitrateW() == bitrate);
                            CHECK(take(MPEGGetVersionW()) == versionText(v));
                            CHECK(take(MPEGGetLayerW()) == L"Layer " + std::to_wstring(layer));
                            CHECK(MPEGGetFrameSizeW() == lengthOf(s, 0));
                            CHECK(MPEGGetFramePositionW() == 0);
                            CHECK(AUDIOGetChannelsW() == 2);
                            const double expected = frames * static_cast<double>(samplesPerFrame(v, layer)) / rate;
                            if (exact) {
                                CHECK(MPEGGetFramesW() == frames);
                                CHECK(std::fabs(AUDIOGetDurationW() - expected) < 0.0005);
                                CHECK(MPEGIsVBRW() == 0);
                            } else {
                                // without a VBR header the duration is calculated from the size and the bit rate
                                const double estimate = static_cast<double>(fs::file_size(p)) * 8.0 / (bitrate * 1000.0);
                                CHECK(std::fabs(AUDIOGetDurationW() - estimate) < 0.0005);
                                CHECK(std::fabs(AUDIOGetDurationW() - expected) < expected * 0.15);   // the frames are rounded down to slots: layer 1 at 32 kbit/s is 12 % off
                            }
                        }
                    }
    CHECK(combinations == 3 * 3 * 3 * 3 * 3);
}

TEST_CASE("MPEG frame header: all bits of the header", "[mpeg][spec][frames]")
{
    Spec s;
    s.mode = 3;                       // mono
    s.crc = true;
    s.priv = true; s.copyright = true; s.original = true; s.emphasis = 1;   // 50/15 ms
    s.paddingPattern = 1;
    auto p = writeTemp("mpegspec_bits.mp3", framesOf(s, 20));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(AUDIOGetChannelsW() == 1);
    CHECK(take(AUDIOGetChannelModeW()) == L"Mono");
    CHECK(MPEGIsProtectedW() != 0);   // protection bit 0: protected by a CRC
    CHECK(MPEGIsPrivateW() != 0);
    CHECK(MPEGIsCopyrightedW() != 0);
    CHECK(MPEGIsOriginalW() != 0);
    CHECK(MPEGIsPaddingW() != 0);
    CHECK(take(MPEGGetEmphasisW()) == L"50/15 ms");

    Spec t;
    t.mode = 2;                        // dual channel
    auto q = writeTemp("mpegspec_bits2.mp3", framesOf(t, 20));
    REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == MPEG);
    CHECK(take(AUDIOGetChannelModeW()) == L"Dual Channel");
    CHECK(MPEGIsProtectedW() == 0);
    CHECK(MPEGIsPrivateW() == 0);
    CHECK(MPEGIsCopyrightedW() == 0);
    CHECK(MPEGIsOriginalW() == 0);
    CHECK(MPEGIsPaddingW() == 0);
}

TEST_CASE("MPEG frame header: invalid headers are not frames", "[mpeg][spec][frames]")
{
    // a run of frames whose header has a reserved value is not recognized: version 01, layer 00, bit rate 1111, sample rate 11, emphasis 10
    const struct { const char* name; int byte; uint8_t value; } bad[] = {
        { "reserved version 01", 1, 0xEB },      // 111 01 01 1 -> version bits 01, layer 01
        { "reserved layer 00", 1, 0xF9 },        // 111 11 00 1
        { "bit rate index 1111", 2, 0xF0 },
        { "sample rate index 11", 2, 0x9C },
        { "reserved emphasis 10", 3, 0x02 },
    };
    for (const auto& b : bad) {
        INFO(b.name);
        Spec s;
        Bytes f = framesOf(s, 20);
        // change the header of every frame
        const int len = lengthOf(s, 0);
        for (size_t i = 0; i + 4 <= f.size(); i += static_cast<size_t>(len))
            f[i + static_cast<size_t>(b.byte)] = (b.byte == 3) ? static_cast<uint8_t>((f[i + 3] & 0xFC) | b.value) : b.value;
        auto p = writeTemp("mpegspec_bad.mp3", f);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != MPEG);
    }
}

TEST_CASE("MPEG: frames are found behind junk and behind a false sync", "[mpeg][spec][frames]")
{
    Spec s;
    Bytes junk;
    for (int i = 0; i < 700; i++) junk.push_back(static_cast<uint8_t>(i * 7 + 3) == 0xFF ? 0 : static_cast<uint8_t>(i * 7 + 3));
    // a sync pattern in the junk that has no frame behind it
    junk[100] = 0xFF; junk[101] = 0xFB; junk[102] = 0x90; junk[103] = 0x00;
    Bytes f = junk;
    const Bytes frames = framesOf(s, 20);
    f.insert(f.end(), frames.begin(), frames.end());
    auto p = writeTemp("mpegspec_junk.mp3", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(MPEGGetFramePositionW() == 700);
    CHECK(AUDIOGetBitrateW() == 128);
}

TEST_CASE("Xing and Info header: position and fields", "[mpeg][spec][xing]")
{
    ExactRead defaultRead(false);   // the header is evaluated when the frames are not counted; do not depend on other tests
    struct Layout { Version v; int mode; const char* name; };
    const Layout layouts[] = { { V1, 0, "MPEG 1 stereo" }, { V1, 3, "MPEG 1 mono" }, { V2, 0, "MPEG 2 stereo" }, { V2, 3, "MPEG 2 mono" }, { V25, 3, "MPEG 2.5 mono" } };
    for (const Layout& lay : layouts)
        for (bool crc : { false, true })
            for (const char* id : { "Xing", "Info" })
                for (uint32_t flags : { 0x1u, 0x3u, 0xFu }) {
                    Spec s;
                    s.version = lay.v; s.layer = 3; s.mode = lay.mode; s.crc = crc;
                    s.sampleRateIndex = 0; s.bitrateIndex = lay.v == V1 ? 9 : 10;   // 128 kbit/s (MPEG 1) or 96 kbit/s
                    const int bitrate = bitrateOf(s.version, 3, s.bitrateIndex);
                    const int rate = sampleRateOf(s.version, 0);
                    const int audioFrames = 30;
                    const uint32_t bytes = static_cast<uint32_t>(lengthOf(s, 0)) * audioFrames;
                    INFO(std::string(lay.name) + (crc ? " CRC " : " ") + id + " flags " + std::to_string(flags));

                    // the first frame is the header frame: header, [CRC], side information (zero), Xing/Info tag
                    Bytes first = headerOf(s, 0);
                    if (crc) { first.push_back(0); first.push_back(0); }
                    first.resize(first.size() + static_cast<size_t>(sideInfoSize(s.version, s.mode)), 0);
                    put(first, id);
                    putBE32(first, flags);
                    if (flags & 1) putBE32(first, audioFrames);
                    if (flags & 2) putBE32(first, bytes);
                    if (flags & 4) first.insert(first.end(), 100, 0);
                    if (flags & 8) putBE32(first, 78);
                    if (flags == 0xF) put(first, "LAME3.99r");
                    first.resize(static_cast<size_t>(lengthOf(s, 0)), 0);
                    Bytes file = first;
                    const Bytes audioData = framesOf(s, audioFrames, 1);
                    file.insert(file.end(), audioData.begin(), audioData.end());
                    auto p = writeTemp("mpegspec_xing.mp3", file);

                    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
                    CHECK(MPEGGetFramePositionW() == 0);
                    const double expected = audioFrames * static_cast<double>(samplesPerFrame(s.version, 3)) / rate;
                    CHECK(MPEGGetFramesW() == audioFrames);
                    CHECK(std::fabs(AUDIOGetDurationW() - expected) < 0.0005);
                    // the tag "Xing" marks a file with a variable bit rate, "Info" one with a constant bit rate
                    CHECK((MPEGIsVBRW() != 0) == (std::string(id) == "Xing"));
                    CHECK(std::abs(AUDIOGetBitrateW() - bitrate) <= 1);
                    if (flags == 0xF) CHECK(take(MPEGGetEncoderW()) == L"LAME 3.99");
                    {
                        // the frames are counted: the header frame is not an audio frame
                        ExactRead exact(true);
                        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
                        CHECK(MPEGGetFramesW() == audioFrames);
                        CHECK(std::fabs(AUDIOGetDurationW() - expected) < 0.0005);
                        CHECK(std::abs(AUDIOGetBitrateW() - bitrate) <= 1);
                    }
                }
}

TEST_CASE("Xing header: a flag that is not set means the field is missing", "[mpeg][spec][xing]")
{
    ExactRead defaultRead(false);   // the header is evaluated when the frames are not counted; do not depend on other tests
    // flags 2: only the number of bytes; the frame count has to come from the size of the file
    Spec s;
    const int audioFrames = 40;
    Bytes first = headerOf(s, 0);
    first.resize(first.size() + 32, 0);
    put(first, "Xing");
    putBE32(first, 2);
    putBE32(first, 123456);   // bytes
    first.resize(static_cast<size_t>(lengthOf(s, 0)), 0);
    Bytes file = first;
    const Bytes a = framesOf(s, audioFrames, 1);
    file.insert(file.end(), a.begin(), a.end());
    auto p = writeTemp("mpegspec_xing_nofr.mp3", file);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    const double expected = (audioFrames + 1) * 1152.0 / 44100;   // estimated from the size of the file
    CHECK(std::fabs(AUDIOGetDurationW() - expected) < expected * 0.05);
    CHECK(MPEGGetFramesW() >= audioFrames);
    CHECK(MPEGGetFramesW() <= audioFrames + 2);
}

TEST_CASE("VBRI header: fixed position 32 bytes behind the header", "[mpeg][spec][vbri]")
{
    ExactRead defaultRead(false);   // the header is evaluated when the frames are not counted; do not depend on other tests
    for (int mode : { 0, 3 }) {
        Spec s;
        s.mode = mode;
        s.bitrateIndex = 9;
        const int audioFrames = 50;
        const uint32_t bytes = static_cast<uint32_t>(lengthOf(s, 0)) * audioFrames;
        Bytes first = headerOf(s, 0);
        first.resize(4 + 32, 0);   // the VBRI tag is always at offset 36
        put(first, "VBRI");
        putBE16(first, 1);         // version
        putBE16(first, 0x0B44);    // delay
        putBE16(first, 75);        // quality
        putBE32(first, bytes);
        putBE32(first, audioFrames);
        putBE16(first, 0); putBE16(first, 1); putBE16(first, 2); putBE16(first, 1);   // TOC entries, scale, entry size, frames per entry
        first.resize(static_cast<size_t>(lengthOf(s, 0)), 0);
        Bytes file = first;
        const Bytes a = framesOf(s, audioFrames, 1);
        file.insert(file.end(), a.begin(), a.end());
        INFO((mode == 0 ? "stereo" : "mono"));
        auto p = writeTemp("mpegspec_vbri.mp3", file);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(MPEGGetFramesW() == audioFrames);
        CHECK(std::fabs(AUDIOGetDurationW() - audioFrames * 1152.0 / 44100) < 0.0005);
        CHECK(MPEGIsVBRW() != 0);
    }
}

TEST_CASE("MPEG: a real VBR file is recognized when the frame scan is on", "[mpeg][spec][frames]")
{
    // frames with changing bit rates (no VBR header): the exact scan finds every frame and the average bit rate
    Bytes file;
    Spec s;
    const int rates[] = { 3, 7, 11, 9, 5 };
    int sum = 0, count = 0;
    for (int i = 0; i < 50; i++) {
        s.bitrateIndex = rates[i % 5];
        const Bytes f = frameOf(s, 0);
        file.insert(file.end(), f.begin(), f.end());
        sum += bitrateOf(V1, 3, s.bitrateIndex);
        count++;
    }
    auto p = writeTemp("mpegspec_vbr.mp3", file);
    ExactRead er(true);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(MPEGGetFramesW() == count);
    CHECK(MPEGIsVBRW() != 0);
    CHECK(AUDIOGetBitrateW() == sum / count);
    CHECK(std::fabs(AUDIOGetDurationW() - count * 1152.0 / 44100) < 0.0005);
}
