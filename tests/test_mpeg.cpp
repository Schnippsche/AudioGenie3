#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <chrono>

using namespace ag3test;

TEST_CASE("MPEG: analysis returns format and basic data", "[mpeg]")
{
    auto p = writeTemp("silence.mp3", makeMp3(100));   // 100 Frames * 26,12 ms
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(AUDIOGetChannelsW() == 2);
    CHECK(AUDIOGetBitrateW() == 128);
    CHECK(AUDIOGetDurationW() == Catch::Approx(2.612).margin(0.05));
}

TEST_CASE("MPEG: ID3v2 round trip, audio data stays unchanged", "[mpeg][id3v2][roundtrip]")
{
    const Bytes audio = makeMp3(50);
    auto p = writeTemp("id3_tag.mp3", audio);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    ID3V2SetTextFrameW(ID3F_TIT2, L"Titel äöü €");
    ID3V2SetTextFrameW(ID3F_TPE1, L"Kuenstler");
    REQUIRE(ID3V2SaveChangesW() != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(ID3V2ExistsW() != 0);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Titel äöü €");
    CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"Kuenstler");

    const Bytes after = readFile(p);
    const size_t tag = id3v2TotalSize(after);
    REQUIRE(tag > 10);
    REQUIRE(after.size() >= tag + audio.size());
    // The frames follow the tag; an appended ID3v1 tag (128 bytes) is allowed.
    CHECK(std::equal(audio.begin(), audio.end(), after.begin() + tag));
}

TEST_CASE("MPEG: AUDIOSetTitleW + AUDIOSaveChangesW writes ID3v2 (TIT2)", "[mpeg][id3v2][roundtrip]")
{
    auto p = writeTemp("audio_title.mp3", makeMp3(50));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    AUDIOSetTitleW(L"Abstrakter Titel");
    REQUIRE(AUDIOSaveChangesW() != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"Abstrakter Titel");
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Abstrakter Titel");
}

TEST_CASE("MPEG: ID3V2RemoveTagW removes the tag from the file immediately", "[mpeg][id3v2]")
{
    const Bytes audio = makeMp3(50);
    auto p = writeTemp("id3_remove.mp3", audio);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    ID3V2SetTextFrameW(ID3F_TIT2, L"weg damit");
    REQUIRE(ID3V2SaveChangesW() != 0);
    REQUIRE(id3v2TotalSize(readFile(p)) > 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2RemoveTagW() != 0);   // takes effect immediately, no save needed

    const Bytes after = readFile(p);
    CHECK(id3v2TotalSize(after) == 0);
    CHECK(after == audio);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(ID3V2ExistsW() == 0);
}

// ---------------------------------------------------------------- MPEGEXACTREAD (SetConfigValueW key 0)

namespace {

constexpr long kExactRead = 0;   // configuration key MPEGEXACTREAD

// switches MPEGEXACTREAD on for the lifetime of the object and back off afterwards
struct ExactRead {
    explicit ExactRead(bool on) { SetConfigValueW(kExactRead, on ? 1 : 0); }
    ~ExactRead() { SetConfigValueW(kExactRead, 0); }
};

// MPEG-1 Layer 3, 44.1 kHz, stereo. bitrate index: 5 = 64, 9 = 128, 11 = 192 kbit/s.
void appendFrame(Bytes& out, int bitrateIndex, bool padding)
{
    static const int kbit[] = { 0, 0, 0, 0, 0, 64, 0, 0, 0, 128, 0, 192 };
    const size_t len = 144 * kbit[bitrateIndex] * 1000 / 44100 + (padding ? 1 : 0);
    const size_t at = out.size();
    out.resize(at + len, 0);
    out[at] = 0xFF;
    out[at + 1] = 0xFB;
    out[at + 2] = static_cast<uint8_t>((bitrateIndex << 4) | (padding ? 2 : 0));
    out[at + 3] = 0x00;
}

// VBR file without Xing header: 64, 128, 192 kbit/s in turn; the first frame has the padding bit set, the last one not
Bytes makeVbrWithoutHeader(int rounds)
{
    Bytes b;
    for (int i = 0; i < rounds; i++) {
        appendFrame(b, 5, i == 0);
        appendFrame(b, 9, false);
        appendFrame(b, 11, false);
    }
    return b;
}

}  // namespace

TEST_CASE("MPEG: VBR file without header: exact read counts all frames", "[mpeg][exactread]")
{
    const int rounds = 400;   // 1200 frames
    auto p = writeTemp("vbr_noheader.mp3", makeVbrWithoutHeader(rounds));
    const double exactDuration = 3 * rounds * 1152.0 / 44100.0;

    {   // normal: treated as CBR with the bit rate of the first frame (64 kbit/s)
        ExactRead off(false);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(AUDIOGetBitrateW() == 64);
    }
    {
        ExactRead on(true);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(MPEGGetFramesW() == 3 * rounds);
        CHECK(AUDIOGetBitrateW() == 128);   // average of 64, 128 and 192
        CHECK(AUDIOGetDurationW() == Catch::Approx(exactDuration).margin(0.05));
    }
}

TEST_CASE("MPEG: exact read keeps the properties of the first frame", "[mpeg][exactread]")
{
    auto p = writeTemp("vbr_flags.mp3", makeVbrWithoutHeader(50));
    bool padding;
    {
        ExactRead off(false);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        padding = MPEGIsPaddingW() != 0;
    }
    CHECK(padding);   // the first frame has the padding bit set
    ExactRead on(true);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK((MPEGIsPaddingW() != 0) == padding);   // not the state of the last frame
}

TEST_CASE("MPEG: exact read gives the correct length of a CBR file with trailing data", "[mpeg][exactread]")
{
    Bytes withTail = makeMp3(1000);
    withTail.insert(withTail.end(), 1 << 20, 'A');   // 1 MB non-frame data after the frames
    auto p = writeTemp("cbr_trailing_data.mp3", withTail);

    ExactRead on(true);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(MPEGGetFramesW() == 1000);
    CHECK(AUDIOGetDurationW() == Catch::Approx(1000 * 1152.0 / 44100.0).margin(0.01));
    CHECK(AUDIOGetBitrateW() == 128);
}

TEST_CASE("MPEG: exact read ignores a trailing tag and is fast with it", "[mpeg][exactread]")
{
    const Bytes audio = makeVbrWithoutHeader(100);   // 300 frames, VBR is detected by the frame scan
    auto plain = writeTemp("exact_plain.mp3", audio);
    Bytes withTail = audio;
    withTail.insert(withTail.end(), 1 << 20, 'A');   // 1 MB non-frame data after the frames
    auto tail = writeTemp("exact_tail.mp3", withTail);

    ExactRead on(true);
    REQUIRE(AUDIOAnalyzeFileW(plain.c_str()) == MPEG);
    const long frames = MPEGGetFramesW();
    const float duration = AUDIOGetDurationW();
    CHECK(frames == 300);

    const auto t0 = std::chrono::steady_clock::now();
    REQUIRE(AUDIOAnalyzeFileW(tail.c_str()) == MPEG);
    const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    CHECK(MPEGGetFramesW() == frames);
    CHECK(AUDIOGetDurationW() == Catch::Approx(duration).margin(0.05));
    INFO("analysis with 1 MB trailing data took " << ms << " ms");
    CHECK(ms < 150.0);   // byte by byte it took about 450 ms
}

// ---------------------------------------------------------------- tags at the end of the file (read cache)

namespace {

// tags at the end of an MP3: ID3v1, APE with an item of the given size, Lyrics3 (optional)
void writeTailTags(const std::filesystem::path& p, size_t apeItemChars, bool lyrics)
{
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    ID3V1SetTitleW(L"v1 title");
    ID3V1SetArtistW(L"v1 artist");
    REQUIRE(ID3V1SaveChangesW() != 0);
    if (apeItemChars > 0) {
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        APESetTitleW(L"ape title");
        APESetUserItemW(L"Big", std::wstring(apeItemChars, L'x').c_str());
        REQUIRE(APESaveChangesW() != 0);
    }
    if (lyrics) {
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        LYRICSSetLyricsW(L"la la la, the lyrics of the song");
        LYRICSSetTitleW(L"lyrics title");
        REQUIRE(LYRICSSaveChangesW() != 0);
    }
}

}  // namespace

TEST_CASE("MPEG: tags at the end of the file are read completely (read cache)", "[mpeg][tailcache]")
{
    struct Case { const char* name; int frames; size_t apeChars; bool lyrics; };
    const Case cases[] = {
        { "small file, all tags inside the cache", 20, 200, false },
        { "APE tag larger than the cache", 200, 40000, false },
        { "APE tag between the frames and ID3v1, 10 KB", 200, 10000, false },
        { "Lyrics3 and ID3v1", 200, 0, true },
    };
    for (const Case& c : cases) {
        DYNAMIC_SECTION(c.name) {
            auto p = writeTemp(std::string("tail_") + std::to_string(c.frames) + "_" + std::to_string(c.apeChars) + ".mp3", makeMp3(c.frames));
            writeTailTags(p, c.apeChars, c.lyrics);

            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(ID3V1ExistsW() != 0);
            CHECK(take(ID3V1GetTitleW()) == L"v1 title");
            CHECK(take(ID3V1GetArtistW()) == L"v1 artist");
            CHECK(MPEGGetFramesW() > 0);
            if (c.apeChars > 0) {
                CHECK(take(APEGetTitleW()) == L"ape title");
                CHECK(take(APEGetUserItemW(L"Big")).size() == c.apeChars);
            }
            if (c.lyrics) {
                CHECK(LYRICSExistsW() != 0);
                CHECK(take(LYRICSGetTitleW()) == L"lyrics title");
                CHECK(take(LYRICSGetLyricsW()) == L"la la la, the lyrics of the song");
            }
            // the duration does not include the tags
            CHECK(AUDIOGetDurationW() == Catch::Approx(c.frames * 1152.0 / 44100.0).margin(0.1));
        }
    }
}
