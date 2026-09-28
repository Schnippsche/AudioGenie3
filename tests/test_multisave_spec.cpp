// Saving a tag twice in a row without AUDIOAnalyzeFileW in between, for every writable format. Several *SaveChangesToFileW functions
// re-read the "AUDIOGetLastFileW() == FileName" case (calling analyze themselves) and hide problems that only show up when the caller
// passes a path that is textually different from the last analyzed one (a relative path, a different drive letter mapping, a path with
// an extra "." segment) but names the very same file - which native _wfsopen() resolves identically, so the file is still opened. Since
// two spellings compare unequal, the wrapper's "reload after saving" convenience does not fire, exercising the underlying SaveToFile()
// on its own: a save that leaves cached positions or sizes from the earlier analysis around damages the audio data of the second save.
// (FLAC and WMA had exactly this problem; both are fixed and covered here so a regression comes back with a red test.)
#include "id3v2_support.h"
#include <cmath>
#include <filesystem>
#include <string>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

// a path that a plain string comparison sees as different from "p", but that opens the identical file (Windows resolves the "."
// segment); used instead of p itself so the *SaveChangesToFileW wrapper's "reload if FileName == last analyzed file" shortcut,
// which would silently paper over a stale position/size, does not apply.
std::wstring otherSpelling(const fs::path& p)
{
    return p.parent_path().wstring() + L"\\.\\" + p.filename().wstring();
}

fs::path copyFixture(const char* rel, const char* newName)
{
    const fs::path dst = tempDir() / newName;
    fs::copy_file(fs::path(AG3_FIXTURES_DIR) / rel, dst, fs::copy_options::overwrite_existing);
    return dst;
}

}  // namespace

TEST_CASE("Multi-save without a fresh analysis: WAV", "[multisave][spec][write]")
{
    const Bytes original = makeWav(44100, 2, 0.5);
    auto p = writeTemp("multisave.wav", original);
    const std::wstring p2 = otherSpelling(p);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    WAVSetTextFrameW(WAV_INAM, L"Title one");
    REQUIRE(WAVSaveChangesToFileW(p2.c_str()) != 0);
    WAVSetTextFrameW(WAV_IART, std::wstring(500, L'b').c_str());
    REQUIRE(WAVSaveChangesToFileW(p2.c_str()) != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"Title one");
    CHECK(take(WAVGetTextFrameW(WAV_IART)).size() == 500);
    CHECK(AUDIOGetDurationW() == Catch::Approx(0.5).margin(0.01));
    // the PCM samples (the 44 byte header aside) must still be exactly the original ones
    const Bytes after = readFile(p);
    const size_t dataStart = 44;
    REQUIRE(after.size() >= original.size());
    CHECK(std::equal(original.begin() + static_cast<std::ptrdiff_t>(dataStart), original.end(), after.end() - static_cast<std::ptrdiff_t>(original.size() - dataStart)));
}

TEST_CASE("Multi-save without a fresh analysis: FLAC", "[multisave][spec][write]")
{
    auto p = copyFixture("flac/tagged.flac", "multisave.flac");
    const std::wstring p2 = otherSpelling(p);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
    FLACSetUserItemW(L"TITLE", L"Title one");
    REQUIRE(FLACSaveChangesToFileW(p2.c_str()) != 0);
    FLACSetUserItemW(L"ARTIST", std::wstring(9000, L'c').c_str());   // large enough to force a rebuild of the file
    REQUIRE(FLACSaveChangesToFileW(p2.c_str()) != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
    CHECK(take(FLACGetUserItemW(L"TITLE")) == L"Title one");
    CHECK(take(FLACGetUserItemW(L"ARTIST")).size() == 9000);
    CHECK(AUDIOIsValidFormatW() != 0);
    CHECK(AUDIOGetDurationW() > 0.0f);
}

TEST_CASE("Multi-save without a fresh analysis: WMA", "[multisave][spec][write]")
{
    auto p = copyFixture("wma/tagged.wma", "multisave.wma");
    const std::wstring p2 = otherSpelling(p);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
    WMASetUserItemW(L"Title", L"Title one");
    REQUIRE(WMASaveChangesToFileW(p2.c_str()) != 0);
    WMASetUserItemW(L"WM/Composer", std::wstring(9000, L'c').c_str());
    REQUIRE(WMASaveChangesToFileW(p2.c_str()) != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
    CHECK(take(WMAGetUserItemW(L"Title")) == L"Title one");
    CHECK(take(WMAGetUserItemW(L"WM/Composer")).size() == 9000);
    CHECK(AUDIOGetDurationW() > 0.0f);
}

TEST_CASE("Multi-save without a fresh analysis: MP4/M4A", "[multisave][spec][write]")
{
    auto p = copyFixture("m4a/tagged.m4a", "multisave.m4a");
    const std::wstring p2 = otherSpelling(p);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
    MP4SetTextFrameW(MP4_TITLE, L"Title one");
    REQUIRE(MP4SaveChangesToFileW(p2.c_str()) != 0);
    MP4SetTextFrameW(MP4_COMMENT, std::wstring(5000, L'c').c_str());
    REQUIRE(MP4SaveChangesToFileW(p2.c_str()) != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
    CHECK(take(MP4GetTextFrameW(MP4_TITLE)) == L"Title one");
    CHECK(take(MP4GetTextFrameW(MP4_COMMENT)).size() == 5000);
    CHECK(AUDIOGetDurationW() > 0.0f);
}

TEST_CASE("Multi-save without a fresh analysis: Ogg Vorbis", "[multisave][spec][write]")
{
    auto p = copyFixture("ogg/tagged.ogg", "multisave.ogg");
    const std::wstring p2 = otherSpelling(p);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
    OGGSetTitleW(L"Title one");
    REQUIRE(OGGSaveChangesToFileW(p2.c_str()) != 0);
    OGGSetUserItemW(L"COMMENT", std::wstring(9000, L'c').c_str());
    REQUIRE(OGGSaveChangesToFileW(p2.c_str()) != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
    CHECK(take(OGGGetTitleW()) == L"Title one");
    CHECK(take(OGGGetUserItemW(L"COMMENT")).size() == 9000);
    CHECK(AUDIOGetDurationW() > 0.0f);
}

TEST_CASE("Multi-save without a fresh analysis: MP3 (ID3v2)", "[multisave][spec][write]")
{
    auto p = copyFixture("mp3/tagged.mp3", "multisave_id3v2.mp3");
    const std::wstring p2 = otherSpelling(p);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    ID3V2SetTextFrameW(0x54495432 /* TIT2 */, L"Title one");
    REQUIRE(ID3V2SaveChangesToFileW(p2.c_str()) != 0);
    ID3V2SetTextFrameW(0x54504531 /* TPE1 */, std::wstring(3000, L'c').c_str());
    REQUIRE(ID3V2SaveChangesToFileW(p2.c_str()) != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(0x54495432)) == L"Title one");
    CHECK(take(ID3V2GetTextFrameW(0x54504531)).size() == 3000);
    CHECK(AUDIOGetDurationW() > 0.0f);
}

TEST_CASE("Multi-save without a fresh analysis: APE tag", "[multisave][spec][write]")
{
    auto p = copyFixture("ape/tagged.ape", "multisave.ape");
    const std::wstring p2 = otherSpelling(p);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
    APESetUserItemW(L"Title", L"Title one");
    REQUIRE(APESaveChangesToFileW(p2.c_str()) != 0);
    APESetUserItemW(L"Comment", std::wstring(3000, L'c').c_str());
    REQUIRE(APESaveChangesToFileW(p2.c_str()) != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MONKEY);
    CHECK(take(APEGetUserItemW(L"Title")) == L"Title one");
    CHECK(take(APEGetUserItemW(L"Comment")).size() == 3000);
    CHECK(AUDIOGetDurationW() > 0.0f);
}
