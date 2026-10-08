// The save functions of dllmain with another file than the analyzed one: the values of the analysis stay, the tags of the written file
// decide which tags are kept in sync, and a file of another format keeps its own fields.
#include "id3v2_support.h"
#include <filesystem>
#include <string>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

fs::path copyFixture(const char* rel, const char* newName)
{
    const fs::path dst = tempDir() / newName;
    fs::copy_file(fs::path(AG3_FIXTURES_DIR) / rel, dst, fs::copy_options::overwrite_existing);
    return dst;
}

Bytes id3v2Padding(size_t size)
{
    Bytes tag = { 'I', 'D', '3', 3, 0, 0, 0, 0, static_cast<uint8_t>((size >> 7) & 0x7F), static_cast<uint8_t>(size & 0x7F) };
    tag.resize(10 + size, 0);
    return tag;
}

Bytes id3v1(const char* title)
{
    Bytes tag = { 'T', 'A', 'G' };
    for (const char* c = title; *c; c++) tag.push_back(static_cast<uint8_t>(*c));
    tag.resize(128, 0);
    tag[127] = 0xFF;
    return tag;
}

bool hasID3v1(const Bytes& f) { return f.size() >= 128 && f[f.size() - 128] == 'T' && f[f.size() - 127] == 'A' && f[f.size() - 126] == 'G'; }

}  // namespace

TEST_CASE("Save flow: a save into another file does not change the tag sizes of the analysis", "[saveflow][spec][write]")
{
    // the analyzed FLAC file has no ID3v2 tag, the other one has: before, the analyzed file could not be saved afterwards (error 218)
    const Bytes flac = readFile(fs::path(AG3_FIXTURES_DIR) / "flac/tagged.flac");
    auto pa = writeTemp("saveflow_a.flac", flac);
    Bytes withTag = id3v2Padding(1000);
    withTag.insert(withTag.end(), flac.begin(), flac.end());
    auto pb = writeTemp("saveflow_b.flac", withTag);
    REQUIRE(AUDIOAnalyzeFileW(pa.c_str()) == FLAC);
    AUDIOSetTitleW(L"Title of the save");
    REQUIRE(AUDIOSaveChangesToFileW(pb.c_str()) != 0);
    REQUIRE(AUDIOSaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(pa.c_str()) == FLAC);
    CHECK(take(AUDIOGetTitleW()) == L"Title of the save");
    const Bytes g = readFile(pb);
    const Bytes tag = id3v2Padding(1000);
    const bool tagKept = std::equal(tag.begin(), tag.end(), g.begin());
    CHECK(tagKept);
    REQUIRE(AUDIOAnalyzeFileW(pb.c_str()) == FLAC);
    CHECK(take(AUDIOGetTitleW()) == L"Title of the save");
}

TEST_CASE("Save flow: the format of the analysis stays after a save into another file", "[saveflow][spec][write]")
{
    // before, AUDIOSaveChangesToFileW put the format of the written file into the analysis: the MD5 of the analyzed file was empty after
    // an attempt to save into a file of no known format
    auto p = writeTemp("saveflow_md5.mp3", makeMp3(40));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    const std::wstring md5 = take(AUDIOGetMD5ValueW());
    REQUIRE(!md5.empty());
    auto text = writeTemp("saveflow.txt", Bytes(200, 'x'));
    CHECK(AUDIOSaveChangesToFileW(text.c_str()) == 0);
    CHECK(take(AUDIOGetMD5ValueW()) == md5);
}

TEST_CASE("Save flow: the ID3v1 tag of the written file decides whether it is kept in sync", "[saveflow][spec][write]")
{
    // before, the ID3v1 tag of the analyzed file decided: a save into a file without one added one, a file with one kept its old values
    Bytes withV1 = makeMp3(40);
    const Bytes v1 = id3v1("Old title");
    withV1.insert(withV1.end(), v1.begin(), v1.end());
    const Bytes withoutV1 = makeMp3(40);
    SECTION("the analyzed file has an ID3v1 tag, the written one has none") {
        auto pa = writeTemp("saveflow_v1a.mp3", withV1);
        auto pb = writeTemp("saveflow_v1b.mp3", withoutV1);
        REQUIRE(AUDIOAnalyzeFileW(pa.c_str()) == MPEG);
        AUDIOSetTitleW(L"New title");
        REQUIRE(AUDIOSaveChangesToFileW(pb.c_str()) != 0);
        CHECK(!hasID3v1(readFile(pb)));
    }
    SECTION("the analyzed file has none, the written one has an ID3v1 tag") {
        auto pa = writeTemp("saveflow_v1a.mp3", withoutV1);
        auto pb = writeTemp("saveflow_v1b.mp3", withV1);
        REQUIRE(AUDIOAnalyzeFileW(pa.c_str()) == MPEG);
        AUDIOSetTitleW(L"New title");
        REQUIRE(AUDIOSaveChangesToFileW(pb.c_str()) != 0);
        const Bytes g = readFile(pb);
        REQUIRE(hasID3v1(g));
        const std::string title(reinterpret_cast<const char*>(&g[g.size() - 125]), 9);
        CHECK(title == "New title");
    }
}

TEST_CASE("Save flow: a file of another format keeps its own fields", "[saveflow][spec][write]")
{
    // an MP3 is analyzed, the general fields are saved into an M4A: before, its other fields (here a free form item) were lost
    auto m4a = copyFixture("m4a/tagged.m4a", "saveflow.m4a");
    REQUIRE(AUDIOAnalyzeFileW(m4a.c_str()) == MP4M4A);
    MP4SetiTuneFrameW(L"MYKEY", L"my value");
    REQUIRE(MP4SaveChangesW() != 0);
    auto mp3 = writeTemp("saveflow_general.mp3", makeMp3(40));
    REQUIRE(AUDIOAnalyzeFileW(mp3.c_str()) == MPEG);
    AUDIOSetTitleW(L"Title of the MP3");
    AUDIOSetArtistW(L"Artist of the MP3");
    REQUIRE(AUDIOSaveChangesToFileW(m4a.c_str()) != 0);
    // the analysis is the MP3 with the fields that were set
    CHECK(take(AUDIOGetTitleW()) == L"Title of the MP3");
    CHECK(MPEGGetFramesW() > 0);
    REQUIRE(AUDIOAnalyzeFileW(m4a.c_str()) == MP4M4A);
    CHECK(take(AUDIOGetTitleW()) == L"Title of the MP3");
    CHECK(take(AUDIOGetArtistW()) == L"Artist of the MP3");
    CHECK(take(MP4GetiTuneFrameW(L"MYKEY")) == L"my value");
}
