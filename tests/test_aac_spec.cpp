// AAC checks with synthetic files: ADTS (header in front of every frame) and ADIF (one header at the start) as defined in
// ISO/IEC 13818-7 and ISO/IEC 14496-3. The files are built bit by bit from the fields of the specification.
#include "id3v2_support.h"
#include <cmath>
#include <string>

using namespace ag3test;

namespace {

// writes bits, most significant bit first
struct BitWriter {
    Bytes bytes;
    size_t bits = 0;
    void put(uint32_t value, int count)
    {
        for (int i = count - 1; i >= 0; i--) {
            if (bits % 8 == 0) bytes.push_back(0);
            if ((value >> i) & 1) bytes.back() |= static_cast<uint8_t>(0x80 >> (bits % 8));
            bits++;
        }
    }
};

const int RATES[13] = { 96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350 };

struct Frame {
    int id = 0;                  // 0: MPEG-4, 1: MPEG-2
    bool crc = false;            // with a checksum: the header has 9 bytes
    int profile = 1;             // object type - 1: 0 Main, 1 LC, 2 SSR, 3 LTP
    int rate = 4;                // sampling frequency index
    int config = 2;              // channel configuration
    int blocks = 1;              // raw data blocks in the frame
    int length = 371;            // with the header
    bool vbr = false;            // buffer fullness 0x7FF
    Bytes payload;               // put right behind the header (the program config element for the configuration 0)
};

Bytes adtsFrame(const Frame& f)
{
    const int headerLength = f.crc ? 9 : 7;
    BitWriter w;
    w.put(0xFFF, 12);
    w.put(static_cast<uint32_t>(f.id), 1);
    w.put(0, 2);                                            // layer
    w.put(f.crc ? 0 : 1, 1);                                // protection_absent
    w.put(static_cast<uint32_t>(f.profile), 2);
    w.put(static_cast<uint32_t>(f.rate), 4);
    w.put(0, 1);                                            // private
    w.put(static_cast<uint32_t>(f.config), 3);
    w.put(0, 4);                                            // original, home, copyright id bit and start
    w.put(static_cast<uint32_t>(f.length), 13);
    w.put(f.vbr ? 0x7FF : 0x123, 11);                       // buffer fullness
    w.put(static_cast<uint32_t>(f.blocks - 1), 2);
    Bytes out = w.bytes;
    if (f.crc) { out.push_back(0xAB); out.push_back(0xCD); }
    out.insert(out.end(), f.payload.begin(), f.payload.end());
    while (static_cast<int>(out.size()) < f.length) out.push_back(static_cast<uint8_t>(0x30 + (out.size() & 0x0F)));
    REQUIRE(static_cast<int>(out.size()) == f.length);
    (void)headerLength;
    return out;
}

Bytes adtsFile(const Frame& f, int frames)
{
    Bytes out;
    for (int i = 0; i < frames; i++) {
        Frame g = f;
        g.length = f.length + (i % 5) * 7;    // frames of different sizes
        const Bytes b = adtsFrame(g);
        out.insert(out.end(), b.begin(), b.end());
    }
    return out;
}

// program_config_element(): tag, object type, sampling frequency index, the numbers of the elements, then the lists
void programConfig(BitWriter& w, int profile, int rate, const std::vector<int>& front, const std::vector<int>& side, const std::vector<int>& back, int lfe)
{
    w.put(0, 4);
    w.put(static_cast<uint32_t>(profile), 2);
    w.put(static_cast<uint32_t>(rate), 4);
    w.put(static_cast<uint32_t>(front.size()), 4);
    w.put(static_cast<uint32_t>(side.size()), 4);
    w.put(static_cast<uint32_t>(back.size()), 4);
    w.put(static_cast<uint32_t>(lfe), 2);
    w.put(0, 3);                                            // assoc data
    w.put(0, 4);                                            // coupling channels
    w.put(0, 1); w.put(0, 1); w.put(0, 1);                  // no mixdowns
    for (const auto* list : { &front, &side, &back })
        for (int cpe : *list) { w.put(static_cast<uint32_t>(cpe), 1); w.put(0, 4); }
    for (int i = 0; i < lfe; i++) w.put(0, 4);
    while (w.bits % 8) w.put(0, 1);                         // byte alignment
    w.put(0, 8);                                            // comment field bytes
}

struct Adif {
    bool copyright = false;
    bool variable = false;
    uint32_t bitrate = 128000;
    int profile = 1;
    int rate = 4;
    std::vector<int> front = { 1 }, side, back;
    int lfe = 0;
    size_t audioSize = 64000;
};

Bytes adifFile(const Adif& a)
{
    BitWriter w;
    for (char c : std::string("ADIF")) w.put(static_cast<uint32_t>(c), 8);
    w.put(a.copyright ? 1 : 0, 1);
    if (a.copyright) for (int i = 0; i < 9; i++) w.put(0x41 + i, 8);   // copyright_id: 72 bits
    w.put(0, 1); w.put(0, 1);                                          // original_copy, home
    w.put(a.variable ? 1 : 0, 1);                                      // bitstream_type
    w.put(a.bitrate, 23);
    w.put(0, 4);                                                       // one program config element
    if (!a.variable) w.put(0x12345, 20);                               // buffer fullness
    programConfig(w, a.profile, a.rate, a.front, a.side, a.back, a.lfe);
    Bytes out = w.bytes;
    out.resize(out.size() + a.audioSize, 0x55);
    return out;
}

}  // namespace

TEST_CASE("AAC ADTS: header fields, duration and bit rate from all frames", "[aac][spec]")
{
    Frame f;
    const Bytes data = adtsFile(f, 200);
    auto p = writeTemp("aac_basic.aac", data);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
    CHECK(take(AACGetHeaderTypeW()) == L"ADTS");
    CHECK(take(AACGetProfileW()) == L"AAC LC");
    CHECK(take(AUDIOGetVersionW()) == L"MPEG-4");
    CHECK(take(AACGetBitRateTypeW()) == L"CBR");
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(AUDIOGetChannelsW() == 2);
    const double seconds = 200 * 1024.0 / 44100;
    CHECK(std::fabs(AUDIOGetDurationW() - seconds) < 0.0005);   // exact: 1024 samples per frame
    CHECK(AUDIOGetBitrateW() == static_cast<long>(data.size() * 8.0 / seconds / 1000.0 + 0.5));
}

TEST_CASE("AAC ADTS: every sampling frequency index, channel configurations 1 to 7, profiles and the MPEG-2 id", "[aac][spec]")
{
    for (int r = 0; r < 13; r++) {
        INFO("rate index " << r);
        Frame f; f.rate = r;
        auto p = writeTemp("aac_rate.aac", adtsFile(f, 50));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(AUDIOGetSampleRateW() == RATES[r]);
        CHECK(std::fabs(AUDIOGetDurationW() - 50 * 1024.0 / RATES[r]) < 0.001);
    }
    const int channels[8] = { -1, 1, 2, 3, 4, 5, 6, 8 };   // configuration 7 has 8 channels (7.1)
    for (int c = 1; c <= 7; c++) {
        INFO("configuration " << c);
        Frame f; f.config = c;
        auto p = writeTemp("aac_channels.aac", adtsFile(f, 20));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(AUDIOGetChannelsW() == channels[c]);
    }
    const wchar_t* profiles[4] = { L"AAC Main", L"AAC LC", L"AAC SSR", L"AAC LTP" };
    for (int pr = 0; pr < 4; pr++) {
        Frame f; f.profile = pr; f.id = 1;
        auto p = writeTemp("aac_profile.aac", adtsFile(f, 20));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(take(AACGetProfileW()) == profiles[pr]);
        CHECK(take(AUDIOGetVersionW()) == L"MPEG-2");
    }
}

TEST_CASE("AAC ADTS: checksum in the header, several raw data blocks per frame, variable bit rate", "[aac][spec]")
{
    SECTION("with a checksum the header has 9 bytes") {
        Frame f; f.crc = true;
        auto p = writeTemp("aac_crc.aac", adtsFile(f, 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(std::fabs(AUDIOGetDurationW() - 100 * 1024.0 / 44100) < 0.0005);
    }
    SECTION("each raw data block has 1024 samples") {
        Frame f; f.blocks = 3;
        auto p = writeTemp("aac_blocks.aac", adtsFile(f, 60));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(std::fabs(AUDIOGetDurationW() - 60 * 3 * 1024.0 / 44100) < 0.0005);
    }
    SECTION("buffer fullness 0x7FF means a variable bit rate") {
        Frame f; f.vbr = true;
        auto p = writeTemp("aac_vbr.aac", adtsFile(f, 20));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(take(AACGetBitRateTypeW()) == L"VBR");
    }
}

TEST_CASE("AAC ADTS: channel configuration 0 takes the channels from the program config element", "[aac][spec]")
{
    struct Case { const char* name; std::vector<int> front, side, back; int lfe; int channels; };
    const Case cases[] = {
        { "stereo pair", { 1 }, {}, {}, 0, 2 },
        { "mono", { 0 }, {}, {}, 0, 1 },
        { "5.1: center, front pair, back pair, lfe", { 0, 1 }, {}, { 1 }, 1, 6 },
        { "7.1", { 0, 1 }, { 1 }, { 1 }, 1, 8 },
    };
    for (const Case& c : cases) {
        INFO(c.name);
        BitWriter w;
        w.put(5, 3);                                      // id_syn_ele: ID_PCE
        programConfig(w, 1, 4, c.front, c.side, c.back, c.lfe);
        Frame f; f.config = 0; f.payload = w.bytes; f.length = 400;
        auto p = writeTemp("aac_pce.aac", adtsFile(f, 20));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(AUDIOGetChannelsW() == c.channels);
    }
}

TEST_CASE("AAC ADTS: no MPEG audio, no reserved values, damaged data and tags", "[aac][spec]")
{
    SECTION("an MPEG audio header (layer 3) has no AAC frame") {
        Bytes mp3 = { 0xFF, 0xFB, 0x90, 0x64 };
        mp3.resize(4000, 0x11);
        auto p = writeTemp("aac_is_mp3.aac", mp3);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != AAC);
    }
    SECTION("the sampling frequency indices 13 to 15 are reserved") {
        Frame f; f.rate = 13;
        auto p = writeTemp("aac_reserved.aac", adtsFile(f, 20));
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != AAC);
    }
    SECTION("a frame length below the header length is no frame") {
        Frame f; f.length = 6;
        Bytes b = adtsFrame(Frame());
        b[3] = static_cast<uint8_t>((b[3] & 0xFC) | 0); b[4] = 0; b[5] = static_cast<uint8_t>((b[5] & 0x1F) | (6 << 5));
        b.resize(3000, 0x22);
        auto p = writeTemp("aac_short.aac", b);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != AAC);
    }
    SECTION("damaged data in the middle: the next frame is found") {
        Frame f;
        Bytes b = adtsFile(f, 100);
        Bytes junk(777, 0x00);
        Bytes c(b.begin(), b.begin() + static_cast<std::ptrdiff_t>(b.size() / 2));
        // cut at a frame boundary: rebuild from two files and put the junk between
        Bytes first = adtsFile(f, 50), second = adtsFile(f, 50);
        c = first; c.insert(c.end(), junk.begin(), junk.end()); c.insert(c.end(), second.begin(), second.end());
        auto p = writeTemp("aac_junk.aac", c);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(std::fabs(AUDIOGetDurationW() - 100 * 1024.0 / 44100) < 0.0005);
    }
    SECTION("an ID3v1 tag behind the last frame is not counted") {
        Frame f;
        Bytes b = adtsFile(f, 100);
        Bytes tag(128, 0);
        tag[0] = 'T'; tag[1] = 'A'; tag[2] = 'G';
        b.insert(b.end(), tag.begin(), tag.end());
        auto p = writeTemp("aac_id3v1.aac", b);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(std::fabs(AUDIOGetDurationW() - 100 * 1024.0 / 44100) < 0.0005);
    }
    SECTION("a file cut inside the last frame: the incomplete frame is not counted") {
        Frame f;
        Bytes b = adtsFile(f, 100);
        b.resize(b.size() - 100);
        auto p = writeTemp("aac_cut.aac", b);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(std::fabs(AUDIOGetDurationW() - 99 * 1024.0 / 44100) < 0.0005);
    }
}

TEST_CASE("AAC ADIF: header fields, channels from the program config element, duration from the bit rate", "[aac][spec]")
{
    SECTION("stereo, constant rate") {
        Adif a;
        const Bytes b = adifFile(a);
        auto p = writeTemp("adif_stereo.aac", b);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(take(AACGetHeaderTypeW()) == L"ADIF");
        CHECK(take(AACGetBitRateTypeW()) == L"CBR");
        CHECK(take(AACGetProfileW()) == L"AAC LC");
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);       // one channel pair element is two channels
        CHECK(AUDIOGetBitrateW() == 128);
        CHECK(std::fabs(AUDIOGetDurationW() - 8.0 * static_cast<double>(b.size()) / 128000) < 0.001);
    }
    SECTION("variable rate with a copyright id") {
        Adif a; a.variable = true; a.copyright = true; a.rate = 3; a.profile = 0; a.bitrate = 96000;
        auto p = writeTemp("adif_vbr.aac", adifFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(take(AACGetBitRateTypeW()) == L"VBR");
        CHECK(take(AACGetProfileW()) == L"AAC Main");
        CHECK(AUDIOGetSampleRateW() == 48000);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(AUDIOGetBitrateW() == 96);
    }
    SECTION("5.1") {
        Adif a; a.front = { 0, 1 }; a.back = { 1 }; a.lfe = 1;
        auto p = writeTemp("adif_51.aac", adifFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(AUDIOGetChannelsW() == 6);
    }
    SECTION("mono") {
        Adif a; a.front = { 0 };
        auto p = writeTemp("adif_mono.aac", adifFile(a));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        CHECK(AUDIOGetChannelsW() == 1);
    }
    SECTION("a cut header is no AAC file") {
        Bytes b = adifFile(Adif());
        b.resize(12);
        auto p = writeTemp("adif_cut.aac", b);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != AAC);
    }
}
