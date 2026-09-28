// Long texts in the tags: the getters limit the text to MAXTEXTBUFFER (256 KB by default, SetConfigValueW key 4). A text that is longer must not be
// lost or changed when another field of the file is written, and the formats with a 16 bit length must not write a length that wraps around.
#include "id3v2_support.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <string>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

const long KEY_MAXTEXTBUFFER = 4;
const long DEFAULT_BUFFER = 0x40000;

// the text buffer is set for a scope and restored afterwards (the setting is global)
struct TextBuffer {
    explicit TextBuffer(long size) { SetConfigValueW(KEY_MAXTEXTBUFFER, size); }
    ~TextBuffer() { SetConfigValueW(KEY_MAXTEXTBUFFER, DEFAULT_BUFFER); }
};

fs::path copyFixture(const char* rel)
{
    const fs::path dst = tempDir() / fs::path(rel).filename();
    fs::copy_file(fs::path(AG3_FIXTURES_DIR) / rel, dst, fs::copy_options::overwrite_existing);
    return dst;
}

// a text with non-ASCII characters at its start and its end
std::wstring longText(size_t n)
{
    std::wstring s(n, L'L');
    s.front() = 0x00C4;
    s.back() = 0x20AC;
    return s;
}

std::wstring longestOf(BSTR (__stdcall* get)(short))
{
    std::wstring best;
    for (short i = 1; i <= 8; i++) {
        const std::wstring v = take(get(i));
        if (v.size() > best.size()) best = v;
    }
    return best;
}

// writes the long text (with a large buffer), then changes another field with the default buffer and checks the long text with a large buffer
template <class Write, class Save, class Read, class Other>
void keepsLongText(const char* fixture, Write write, Save save, Read read, Other other, size_t size = 300000)
{
    const fs::path p = copyFixture(fixture);
    const std::wstring text = longText(size);
    {
        TextBuffer big(0x1000000);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) != UNKNOWN);
        write(text);
        REQUIRE(save(p.c_str()) != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) != UNKNOWN);
        REQUIRE(read() == text);
    }
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) != UNKNOWN);   // the default buffer: the text is shortened when it is read
    other();
    REQUIRE(save(p.c_str()) != 0);
    TextBuffer big(0x1000000);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) != UNKNOWN);
    CHECK(read() == text);
}

}  // namespace

TEST_CASE("Texts over the text buffer stay unchanged: ID3v2 comment, lyrics and user text", "[texts][spec][write]")
{
    const uint32_t TIT2 = 0x54495432;
    SECTION("COMM") {
        keepsLongText("mp3/tagged.mp3", [](const std::wstring& t) { ID3V2AddCommentW(L"eng", L"long", t.c_str()); }, ID3V2SaveChangesToFileW,
                      [] { return longestOf(ID3V2GetCommentW); }, [&] { ID3V2SetTextFrameW(TIT2, L"other"); });
    }
    SECTION("USLT") {
        keepsLongText("mp3/tagged.mp3", [](const std::wstring& t) { ID3V2AddLyricW(L"eng", L"lyr", t.c_str()); }, ID3V2SaveChangesToFileW,
                      [] { return longestOf(ID3V2GetLyricW); }, [&] { ID3V2SetTextFrameW(TIT2, L"other"); });
    }
    SECTION("TXXX") {
        keepsLongText("mp3/tagged.mp3", [](const std::wstring& t) { ID3V2AddUserTextW(L"usr", t.c_str()); }, ID3V2SaveChangesToFileW,
                      [] { return longestOf(ID3V2GetUserTextW); }, [&] { ID3V2SetTextFrameW(TIT2, L"other"); });
    }
}

TEST_CASE("Texts over the text buffer stay unchanged: APE, MP4, WAV", "[texts][spec][write]")
{
    SECTION("APE item in an APE file") {
        keepsLongText("ape/tagged.ape", [](const std::wstring& t) { APESetUserItemW(L"LONGITEM", t.c_str()); }, APESaveChangesToFileW,
                      [] { return take(APEGetUserItemW(L"LONGITEM")); }, [] { APESetUserItemW(L"OTHER", L"x"); });
    }
    SECTION("APE item in an MP3 file") {
        keepsLongText("mp3/no_tags_cbr.mp3", [](const std::wstring& t) { APESetUserItemW(L"LONGITEM", t.c_str()); }, APESaveChangesToFileW,
                      [] { return take(APEGetUserItemW(L"LONGITEM")); }, [] { APESetUserItemW(L"OTHER", L"x"); });
    }
    SECTION("MP4 free form item") {
        keepsLongText("m4a/tagged.m4a", [](const std::wstring& t) { MP4SetiTuneFrameW(L"LONGITEM", t.c_str()); }, MP4SaveChangesToFileW,
                      [] { return take(MP4GetiTuneFrameW(L"LONGITEM")); }, [] { MP4SetTextFrameW(MP4_TITLE, L"other"); });
    }
    SECTION("MP4 comment") {
        keepsLongText("m4a/tagged.m4a", [](const std::wstring& t) { MP4SetTextFrameW(MP4_COMMENT, t.c_str()); }, MP4SaveChangesToFileW,
                      [] { return take(MP4GetTextFrameW(MP4_COMMENT)); }, [] { MP4SetTextFrameW(MP4_TITLE, L"other"); });
    }
    SECTION("WAV comment chunk") {
        keepsLongText("wav/tagged.wav", [](const std::wstring& t) { WAVSetTextFrameW(WAV_ICMT, t.c_str()); }, WAVSaveChangesToFileW,
                      [] { return take(WAVGetTextFrameW(WAV_ICMT)); }, [] { WAVSetTextFrameW(WAV_INAM, L"other"); });
    }
}

TEST_CASE("Texts over the text buffer stay unchanged: Vorbis comments of Ogg and FLAC", "[texts][spec][write]")
{
    SECTION("FLAC") {
        keepsLongText("flac/tagged.flac", [](const std::wstring& t) { FLACSetUserItemW(L"LONGITEM", t.c_str()); }, FLACSaveChangesToFileW,
                      [] { return take(FLACGetUserItemW(L"LONGITEM")); }, [] { FLACSetUserItemW(L"TITLE", L"other"); });
    }
    SECTION("Ogg Vorbis") {
        keepsLongText("ogg/tagged.ogg", [](const std::wstring& t) { OGGSetUserItemW(L"LONGITEM", t.c_str()); }, OGGSaveChangesToFileW,
                      [] { return take(OGGGetUserItemW(L"LONGITEM")); }, [] { OGGSetUserItemW(L"TITLE", L"other"); });
    }
}

TEST_CASE("WMA: a text of more than 65535 bytes is shortened, it does not damage the attribute", "[texts][spec][write]")
{
    for (size_t n : { size_t(1000), size_t(32766), size_t(32767), size_t(40000), size_t(300000) }) {
        INFO("length " << n);
        const fs::path p = copyFixture("wma/tagged.wma");
        const std::wstring text = longText(n);
        TextBuffer big(0x1000000);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        WMASetUserItemW(L"LongItem", text.c_str());
        REQUIRE(WMASaveChangesToFileW(p.c_str()) != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        const std::wstring got = take(WMAGetUserItemW(L"LongItem"));
        CHECK(got == text.substr(0, std::min<size_t>(n, 32766)));   // the text up to 32766 characters (65534 bytes and the terminating zero)
        // the other attributes are still there
        CHECK(take(WMAGetUserItemW(L"WM/AlbumTitle")).size() > 0);
    }
    SECTION("the content description: title, author, comment") {
        const fs::path p = copyFixture("wma/tagged.wma");
        const std::wstring text = longText(50000);
        TextBuffer big(0x1000000);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        WMASetUserItemW(L"Title", text.c_str());
        WMASetUserItemW(L"Description", text.c_str());
        REQUIRE(WMASaveChangesToFileW(p.c_str()) != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(take(WMAGetUserItemW(L"Title")) == text.substr(0, 32766));
        CHECK(take(WMAGetUserItemW(L"Description")) == text.substr(0, 32766));
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 100);   // the file is still readable
    }
}
