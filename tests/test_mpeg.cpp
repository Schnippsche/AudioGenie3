#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"

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
