// Special cases: file names (Unicode, special characters, extensions, long paths), file attributes and locks,
// invalid paths, very large (sparse) files.
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <winioctl.h>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>

#pragma comment(lib, "user32.lib")   // PeekMessageW, PostThreadMessageW, PostQuitMessage

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

fs::path fixture(const char* rel) { return fs::path(AG3_FIXTURES_DIR) / rel; }

// fresh, empty subdirectory of %TEMP%/ag3tests
fs::path freshDir(const std::wstring& name)
{
    fs::path d = tempDir() / name;
    std::error_code ec;
    fs::remove_all(d, ec);
    fs::create_directories(d);
    return d;
}

void copyTo(const fs::path& src, const fs::path& dst)
{
    fs::create_directories(dst.parent_path());
    fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
}

struct Sample { const char* rel; AudioFormatID format; };
const Sample kSamples[] = {
    { "mp3/tagged.mp3", MPEG }, { "flac/tagged.flac", FLAC }, { "ogg/tagged.ogg", OGGVORBIS }, { "m4a/tagged.m4a", MP4M4A },
    { "wav/tagged.wav", WAV }, { "wma/tagged.wma", WMA }, { "wv/tagged.wv", WAVPACK }, { "tta/tagged.tta", TTA },
    { "ape/tagged.ape", MONKEY }, { "mpc/sv7_tagged_ape.mpc", MPEGPLUS }, { "mpc/sv8_tagged_ape.mpc", MPEGPLUS },
};

// analyze, change title, save, read again
void roundTrip(const fs::path& p, AudioFormatID fmt)
{
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fmt);
    const std::wstring before = take(AUDIOGetTitleW());
    CHECK(!before.empty());
    AUDIOSetTitleW(L"Neuer Titel \u00e4\u00f6\u00fc");
    REQUIRE(AUDIOSaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fmt);
    CHECK(take(AUDIOGetTitleW()) == L"Neuer Titel \u00e4\u00f6\u00fc");
}

}  // namespace

// ============================================================ File names

TEST_CASE("File names: umlauts, CJK, Cyrillic, emoji, blanks and special characters", "[special][names]")
{
    const fs::path dir = freshDir(L"namen_\u00e4\u00f6\u00fc \u65e5\u672c");   // the directory is Unicode as well
    const wchar_t* stems[] = {
        L"Gr\u00fc\u00dfe \u00e4\u00f6\u00fc \u00df",
        L"\u65e5\u672c\u8a9e\u306e\u30d5\u30a1\u30a4\u30eb",
        L"\u0444\u0430\u0439\u043b \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0430",
        L"emoji \U0001F3B5 test",                       // outside the BMP (surrogate pair)
        L"mehrere.punkte.im.namen",
        L"Sonder #%&;,'=+[]{}() $~!@",
        L"a",
    };
    for (const Sample& s : kSamples) {
        if (!fs::exists(fixture(s.rel))) continue;
        for (size_t si = 0; si < sizeof(stems) / sizeof(stems[0]); si++) {
            const wchar_t* stem = stems[si];
            const fs::path p = dir / (std::wstring(stem) + fs::path(s.rel).extension().wstring());
            DYNAMIC_SECTION(s.rel << " als Name Nr. " << si) {
                copyTo(fixture(s.rel), p);
                roundTrip(p, s.format);
            }
        }
    }
}

TEST_CASE("File extension in upper case and mixed case", "[special][names]")
{
    const fs::path dir = freshDir(L"endungen");
    for (const wchar_t* ext : { L".MP3", L".Mp3", L".mP3" }) {
        DYNAMIC_SECTION(fs::path(ext).string()) {
            const fs::path p = dir / (std::wstring(L"lied") + ext);
            copyTo(fixture("mp3/tagged.mp3"), p);
            roundTrip(p, MPEG);
        }
    }
    for (const wchar_t* ext : { L".AAC", L".Aac" }) {
        DYNAMIC_SECTION(fs::path(ext).string()) {
            const fs::path p = dir / (std::wstring(L"lied") + ext);
            copyTo(fixture("aac/adts_sample-1.aac"), p);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
        }
    }
    SECTION("Extension only a dot / no extension: MP3 content is not recognized (the extension is a pre-filter), formats with a signature are") {
        copyTo(fixture("mp3/tagged.mp3"), dir / L"ohne_endung");
        CHECK(AUDIOAnalyzeFileW((dir / L"ohne_endung").c_str()) == UNKNOWN);
        copyTo(fixture("flac/tagged.flac"), dir / L"flac_ohne_endung");
        CHECK(AUDIOAnalyzeFileW((dir / L"flac_ohne_endung").c_str()) == FLAC);
        copyTo(fixture("flac/tagged.flac"), dir / L"flac.mp3");
        CHECK(AUDIOAnalyzeFileW((dir / L"flac.mp3").c_str()) == FLAC);   // the content counts, not the extension
    }
}

// ============================================================ Long paths

namespace {
// \\?\ path with a chain of directories whose total length reaches 'target' characters
fs::path longPath(const std::wstring& root, size_t target, const std::wstring& fileName)
{
    std::wstring p = L"\\\\?\\" + root;
    while (p.size() + 1 + fileName.size() < target)
        p += L"\\" + std::wstring(std::min<size_t>(200, target - p.size() - fileName.size() - 2), L'd');
    return fs::path(p + L"\\" + fileName);
}
}  // namespace

TEST_CASE("Long paths: around MAX_PATH (260) and far beyond (\\\\?\\ prefix)", "[special][longpath]")
{
    const fs::path base = freshDir(L"lang");
    for (size_t target : { 240u, 259u, 300u, 600u, 1500u }) {
        DYNAMIC_SECTION("total length approx. " << target << " characters") {
            const fs::path p = longPath(base.wstring(), target, L"lied.mp3");
            std::error_code ec;
            fs::create_directories(p.parent_path(), ec);
            if (ec) SKIP("directory cannot be created: " << ec.message());
            copyTo(fixture("mp3/tagged.mp3"), p);
            // separate from roundTrip: for long paths it matters that nothing crashes and an error is reported cleanly
            const long fmt = AUDIOAnalyzeFileW(p.c_str());
            INFO("path length " << p.wstring().size());
            REQUIRE(fmt == MPEG);                       // with the \\?\ prefix paths work up to at least 1500 characters
            AUDIOSetTitleW(L"Lang");
            CHECK(AUDIOSaveChangesW() != 0);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(AUDIOGetTitleW()) == L"Lang");
            std::error_code ec2;
            fs::remove_all(base, ec2);
        }
    }
}

namespace {
// a path of exactly 'total' characters below root (directories of at most 200 characters)
std::wstring exactPath(const std::wstring& root, size_t total, const std::wstring& fileName)
{
    std::wstring p = root;
    while (p.size() + 1 + fileName.size() < total) {
        size_t left = total - p.size() - 1 - fileName.size();   // characters for "\dir" components in front of the file name
        size_t n = std::min<size_t>(200, left - 1);
        if (left - 1 - n == 1) n--;                                // never leave exactly one character (a component needs a backslash too)
        p += L"\\" + std::wstring(n, L'd');
    }
    return p + L"\\" + fileName;
}
}  // namespace

TEST_CASE("Long paths without the \\\\?\\ prefix: analysis and saving that writes the file again", "[special][longpath]")
{
    const fs::path base = freshDir(L"lang_plain");
    // 258: the backup (~~) has 260 characters, 259: the temporary file (~), 260 and more: the file itself
    for (size_t total : { 250u, 258u, 259u, 260u, 300u, 600u }) {
        DYNAMIC_SECTION("total length " << total << " characters") {
            const std::wstring plain = exactPath(base.wstring(), total, L"lied.mp3");   // as an application passes it
            REQUIRE(plain.size() == total);
            const fs::path prefixed = L"\\\\?\\" + plain;
            std::error_code ec;
            fs::create_directories(prefixed.parent_path(), ec);
            if (ec) SKIP("directory cannot be created: " << ec.message());
            copyTo(fixture("mp3/tagged.mp3"), prefixed);
            REQUIRE(AUDIOAnalyzeFileW(plain.c_str()) == MPEG);
            AUDIOSetTitleW(L"Lang");
            AUDIOSetCommentW(std::wstring(40000, L'x').c_str());   // the file is written again: temporary file and backup are longer still
            CHECK(AUDIOSaveChangesW() != 0);
            REQUIRE(AUDIOAnalyzeFileW(plain.c_str()) == MPEG);
            CHECK(take(AUDIOGetTitleW()) == L"Lang");
            size_t files = 0;
            for (const auto& e : fs::directory_iterator(prefixed.parent_path())) { (void)e; files++; }
            CHECK(files == 1);   // no temporary file or backup is left
            std::error_code ec2;
            fs::remove_all(base, ec2);
        }
    }
}

namespace {
void writeText(const fs::path& p, const std::string& text, DWORD attributes = FILE_ATTRIBUTE_NORMAL)
{
    std::ofstream(p, std::ios::binary).write(text.data(), static_cast<std::streamsize>(text.size()));
    SetFileAttributesW(p.c_str(), attributes);
}

std::string readText(const fs::path& p)
{
    const Bytes b = readFile(p);
    return std::string(b.begin(), b.end());
}
}  // namespace

TEST_CASE("Writing a file again: files with the names of the temporary file and of the backup stay untouched", "[special][replace]")
{
    const fs::path dir = freshDir(L"tempnames");
    const fs::path p = dir / L"song.mp3";
    copyTo(fixture("mp3/tagged.mp3"), p);
    struct Other { std::wstring suffix; DWORD attributes; };
    const Other others[] = {
        { L"~", FILE_ATTRIBUTE_NORMAL }, { L"~1", FILE_ATTRIBUTE_HIDDEN }, { L"~2", FILE_ATTRIBUTE_READONLY },
        { L"~~", FILE_ATTRIBUTE_NORMAL }, { L"~~1", FILE_ATTRIBUTE_NORMAL },
    };
    for (const Other& o : others)
        writeText(p.wstring() + o.suffix, "a file of the user: song.mp3" + ascii(o.suffix), o.attributes);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    AUDIOSetTitleW(L"Neu");
    AUDIOSetCommentW(std::wstring(40000, L'x').c_str());   // the file is written again
    REQUIRE(AUDIOSaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"Neu");

    for (const Other& o : others) {
        const fs::path q = p.wstring() + o.suffix;
        INFO(q.filename().string());
        CHECK(readText(q) == "a file of the user: song.mp3" + ascii(o.suffix));
        CHECK(GetFileAttributesW(q.c_str()) == o.attributes);
    }
    size_t files = 0;
    for (const auto& e : fs::directory_iterator(dir)) { (void)e; files++; }
    CHECK(files == 1 + std::size(others));   // the temporary file (~3) and the backup (~~2) are gone
    for (const Other& o : others)
        SetFileAttributesW((p.wstring() + o.suffix).c_str(), FILE_ATTRIBUTE_NORMAL);
}

// ============================ Regressions from the misuse test

TEST_CASE("After a failed analysis, saving must not change the previously analyzed file", "[special][regression]")
{
    const fs::path p = tempDir() / "before.mp3";
    copyTo(fixture("mp3/tagged.mp3"), p);
    const Bytes original = readFile(p);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(AUDIOAnalyzeFileW(L"Z:\\gibt\\es\\nicht.mp3") == UNKNOWN);   // clears the fields, but opens no file
    AUDIOSetTitleW(L"Anderer Titel");
    CHECK(AUDIOSaveChangesW() == 0);                                       // formerly: wrote the empty fields into the old file
    CHECK(AUDIOGetLastFileW() != nullptr);
    CHECK(take(AUDIOGetLastFileW()) == L"");
    CHECK(readFile(p) == original);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"Testtitel");
}

TEST_CASE("Chapter ID as parent element (not a CTOC), NULL/short picture arrays do not crash", "[special][regression]")
{
    const fs::path p = tempDir() / "regress.mp3";
    copyTo(fixture("mp3/id3v24_comm.mp3"), p);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2AddChapterW(L"chp", L"Kapitel", L"", 0, 1000) == 0);
    CHECK(ID3V2AddChildElementW(L"chp", L"x") == 0);        // a chapter has no child elements (formerly: crash due to a wrong cast)
    CHECK(ID3V2DeleteChildElementW(L"chp", L"x") == 0);
    CHECK(ID3V2AddChildElementW(L"", L"x") == 0);
    BYTE tiny[3] = { 1, 2, 3 };
    CHECK(MP4AddPictureArrayW(nullptr, 0) == 0);            // formerly: crash when reading arr[0]
    CHECK(MP4AddPictureArrayW(tiny, 3) == 0);
}

// ================================= Invalid paths and non-files

TEST_CASE("Invalid paths: NULL, empty, directory, wildcards, drive, very long", "[special][invalid]")
{
    CHECK(AUDIOAnalyzeFileW(nullptr) == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L"") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L" ") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L".mp3") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L"*.mp3") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L"C:\\nicht\\vorhanden\\<>|?.mp3") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L"\\\\gibt.es.nicht.example\\share\\x.mp3") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L"NUL") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(L"CON.mp3") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(tempDir().c_str()) == UNKNOWN);            // directory
    CHECK(AUDIOAnalyzeFileW(L"C:\\") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(std::wstring(100000, L'x').c_str()) == UNKNOWN);   // 100,000 characters
    CHECK(AUDIOAnalyzeFileW((std::wstring(50000, L'x') + L".mp3").c_str()) == UNKNOWN);

    // after many errors the next valid analysis works
    REQUIRE(AUDIOAnalyzeFileW(fixture("mp3/tagged.mp3").c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"Testtitel");

    // saving without a preceding valid analysis
    AUDIOAnalyzeFileW(L"Z:\\gibt\\es\\nicht.mp3");
    CHECK(AUDIOSaveChangesW() == 0);
    CHECK(AUDIOSaveChangesToFileW(nullptr) == 0);
    CHECK(AUDIOSaveChangesToFileW(L"") == 0);
    CHECK(AUDIOSaveChangesToFileW(L"Z:\\gibt\\es\\nicht.mp3") == 0);
}

namespace {
struct PipeCall { std::wstring path; short analyzed; short saved; long error; };

DWORD WINAPI analyzeAndSave(LPVOID p)
{
    PipeCall* c = static_cast<PipeCall*>(p);
    c->analyzed = AUDIOAnalyzeFileW(c->path.c_str());
    ID3V1SetTitleW(L"darf nicht in die Pipe");
    c->saved = ID3V1SaveChangesToFileW(c->path.c_str());
    c->error = AUDIOGetLastErrorNumberW();
    return 0;
}
}  // namespace

TEST_CASE("Devices are not files: nothing is written into a named pipe", "[special][invalid][device]")
{
    PipeCall call{ L"\\\\.\\pipe\\ag3test_" + std::to_wstring(GetCurrentProcessId()) + L".mp3", -1, -1, 0 };
    HANDLE server = CreateNamedPipeW(call.path.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, nullptr);
    REQUIRE(server != INVALID_HANDLE_VALUE);
    HANDLE thread = CreateThread(nullptr, 0, analyzeAndSave, &call, 0, nullptr);
    REQUIRE(thread != nullptr);
    const bool finished = WaitForSingleObject(thread, 10000) == WAIT_OBJECT_0;
    DWORD written = 0;
    PeekNamedPipe(server, nullptr, 0, nullptr, &written, nullptr);   // what the library wrote into the pipe
    CloseHandle(server);   // a read that blocks returns now, so the test does not hang
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    CHECK(finished);
    CHECK(call.analyzed == UNKNOWN);
    CHECK(call.saved == 0);
    CHECK(call.error != 0);
    CHECK(written == 0);
}

TEST_CASE("Empty and tiny files with every extension", "[special][invalid]")
{
    const fs::path dir = freshDir(L"winzig");
    for (const wchar_t* ext : { L".mp3", L".aac", L".flac", L".ogg", L".m4a", L".wma", L".wav", L".wv", L".tta", L".ape", L".mpc" }) {
        for (size_t size : { 0u, 1u, 3u, 4u, 7u, 8u, 11u, 12u, 15u, 16u, 31u, 43u, 44u }) {
            const fs::path p = dir / (L"x" + std::wstring(ext));
            Bytes b(size);
            for (size_t i = 0; i < size; i++) b[i] = static_cast<uint8_t>(i * 37 + 1);
            writeTemp("winzig_tmp", b);
            fs::copy_file(tempDir() / "winzig_tmp", p, fs::copy_options::overwrite_existing);
            const long fmt = AUDIOAnalyzeFileW(p.c_str());
            INFO("extension " << fs::path(ext).string() << ", size " << size);
            CHECK(fmt == UNKNOWN);
            AUDIOGetDurationW(); take(AUDIOGetTitleW());
            AUDIOSetTitleW(L"x");
            CHECK(AUDIOSaveChangesW() == 0);
        }
    }
}

// ========================================= Read-only files and locks

namespace {
bool setReadOnly(const fs::path& p, bool ro)
{
    DWORD a = GetFileAttributesW(p.c_str());
    if (a == INVALID_FILE_ATTRIBUTES) return false;
    return SetFileAttributesW(p.c_str(), ro ? (a | FILE_ATTRIBUTE_READONLY) : (a & ~FILE_ATTRIBUTE_READONLY)) != 0;
}
}  // namespace

TEST_CASE("Read-only files: reading works, saving fails cleanly and changes nothing", "[special][readonly]")
{
    const fs::path dir = freshDir(L"readonly");
    for (const Sample& s : kSamples) {
        if (!fs::exists(fixture(s.rel))) continue;
        DYNAMIC_SECTION(s.rel) {
            const fs::path p = dir / fs::path(s.rel).filename();
            copyTo(fixture(s.rel), p);
            const Bytes original = readFile(p);
            REQUIRE(setReadOnly(p, true));

            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == s.format);
            AUDIOSetTitleW(L"Darf nicht geschrieben werden");
            CHECK(AUDIOSaveChangesW() == 0);
            CHECK(AUDIOGetLastErrorNumberW() != 0);
            CHECK(readFile(p) == original);            // file unchanged

            // attribute removed: the same save works now
            REQUIRE(setReadOnly(p, false));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == s.format);
            AUDIOSetTitleW(L"Jetzt schon");
            CHECK(AUDIOSaveChangesW() != 0);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == s.format);
            CHECK(take(AUDIOGetTitleW()) == L"Jetzt schon");
        }
    }
}

TEST_CASE("Locked files: a read lock allows reading, writing fails; an exclusive lock already prevents reading", "[special][lock]")
{
    const fs::path dir = freshDir(L"lock");
    for (const Sample& s : { kSamples[0], kSamples[1], kSamples[3], kSamples[4] }) {
        DYNAMIC_SECTION(s.rel) {
            const fs::path p = dir / fs::path(s.rel).filename();
            copyTo(fixture(s.rel), p);
            const Bytes original = readFile(p);

            SECTION("another application reads (read access, sharing for reading only)") {
                HANDLE h = CreateFileW(p.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
                REQUIRE(h != INVALID_HANDLE_VALUE);
                CHECK(AUDIOAnalyzeFileW(p.c_str()) == s.format);
                AUDIOSetTitleW(L"gesperrt");
                CHECK(AUDIOSaveChangesW() == 0);
                CloseHandle(h);
                CHECK(readFile(p) == original);
            }
            SECTION("andere Anwendung sperrt exklusiv") {
                HANDLE h = CreateFileW(p.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
                REQUIRE(h != INVALID_HANDLE_VALUE);
                CHECK(AUDIOAnalyzeFileW(p.c_str()) == UNKNOWN);
                CHECK(AUDIOGetLastErrorNumberW() != 0);
                CloseHandle(h);
                CHECK(AUDIOAnalyzeFileW(p.c_str()) == s.format);     // works again after the lock is released
            }
        }
    }
}

// ======================================== Replacing the file when it is written again

namespace {
FILETIME creationTime(const fs::path& p)
{
    WIN32_FILE_ATTRIBUTE_DATA a{};
    GetFileAttributesExW(p.c_str(), GetFileExInfoStandard, &a);
    return a.ftCreationTime;
}
}  // namespace

TEST_CASE("A file that is written again keeps its creation time, attributes and alternate data streams", "[special][replace]")
{
    const fs::path dir = freshDir(L"replace");
    for (const Sample& s : kSamples) {
        if (!fs::exists(fixture(s.rel))) continue;
        DYNAMIC_SECTION(s.rel) {
            const fs::path p = dir / fs::path(s.rel).filename();
            copyTo(fixture(s.rel), p);
            const auto sizeBefore = fs::file_size(p);
            // creation time 2001-01-01, hidden, a stream of another program
            SYSTEMTIME st{ 2001, 1, 1, 1, 12, 0, 0, 0 };
            FILETIME old{};
            SystemTimeToFileTime(&st, &old);
            HANDLE h = CreateFileW(p.c_str(), FILE_WRITE_ATTRIBUTES, 0, nullptr, OPEN_EXISTING, 0, nullptr);
            REQUIRE(h != INVALID_HANDLE_VALUE);
            REQUIRE(SetFileTime(h, &old, nullptr, nullptr));
            CloseHandle(h);
            const fs::path stream = p.wstring() + L":ag3test";
            h = CreateFileW(stream.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
            REQUIRE(h != INVALID_HANDLE_VALUE);
            DWORD written = 0;
            WriteFile(h, "stream", 6, &written, nullptr);
            CloseHandle(h);
            REQUIRE(SetFileAttributesW(p.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_ARCHIVE));

            // a long comment: the tag does not fit into the room of the old one, the file is written again
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == s.format);
            AUDIOSetCommentW(std::wstring(40000, L'x').c_str());
            REQUIRE(AUDIOSaveChangesW() != 0);
            CHECK(fs::file_size(p) != sizeBefore);

            const FILETIME now = creationTime(p);
            CHECK(CompareFileTime(&now, &old) == 0);
            CHECK((GetFileAttributesW(p.c_str()) & FILE_ATTRIBUTE_HIDDEN) != 0);
            h = CreateFileW(stream.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
            CHECK(h != INVALID_HANDLE_VALUE);
            if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
            // neither the temporary file nor the backup is left
            CHECK(!fs::exists(p.wstring() + L"~"));
            CHECK(!fs::exists(p.wstring() + L"~~"));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == s.format);
            CHECK(take(AUDIOGetCommentW()).size() > 1000);
        }
    }
}

// ================================================ Large (sparse) files

namespace {

// Creates a sparse file: 'head' at the start, 'tail' at the end, unstored zeros in between. Returns false if the
// file system does not support sparse files.
bool makeSparse(const fs::path& p, const Bytes& head, uint64_t totalSize, const Bytes& tail = {})
{
    HANDLE h = CreateFileW(p.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD ret = 0;
    if (!DeviceIoControl(h, FSCTL_SET_SPARSE, nullptr, 0, nullptr, 0, &ret, nullptr)) { CloseHandle(h); return false; }
    bool ok = WriteFile(h, head.data(), static_cast<DWORD>(head.size()), &ret, nullptr) != 0;
    LARGE_INTEGER pos;
    pos.QuadPart = static_cast<LONGLONG>(totalSize - tail.size());
    ok = ok && SetFilePointerEx(h, pos, nullptr, FILE_BEGIN);
    if (!tail.empty()) ok = ok && WriteFile(h, tail.data(), static_cast<DWORD>(tail.size()), &ret, nullptr);
    pos.QuadPart = static_cast<LONGLONG>(totalSize);
    ok = ok && SetFilePointerEx(h, pos, nullptr, FILE_BEGIN) && SetEndOfFile(h);
    CloseHandle(h);
    return ok;
}

struct Wav {
    static Bytes header(uint32_t dataSize, int sampleRate, int channels)
    {
        Bytes b = makeWav(sampleRate, channels, 0.0);        // 44-byte header without data
        auto put32 = [&](size_t off, uint32_t v) { for (int i = 0; i < 4; i++) b[off + i] = static_cast<uint8_t>(v >> (8 * i)); };
        put32(4, 36 + dataSize);
        put32(40, dataSize);
        return b;
    }
};

}  // namespace

TEST_CASE("Large files: 3 GB WAV (sparse) - duration without 32-bit overflow", "[special][large]")
{
    const fs::path p = tempDir() / "gross3gb.wav";
    const uint64_t dataSize = 3ull * 1024 * 1024 * 1024;                    // 3 GiB PCM = 3 * 2^30 / 176400 s
    if (!makeSparse(p, Wav::header(static_cast<uint32_t>(dataSize), 44100, 2), 44 + dataSize)) SKIP("sparse files not possible");
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(AUDIOGetChannelsW() == 2);
    CHECK(AUDIOGetDurationW() == Catch::Approx(static_cast<double>(dataSize) / 176400.0).epsilon(0.001));   // ~18 256 s
    CHECK(AUDIOGetDurationMillisW() > 0);
    std::error_code ec;
    fs::remove(p, ec);
}

TEST_CASE("Large files: 3 GB MP3 (sparse) with an ID3v1 tag at the end", "[special][large]")
{
    const fs::path p = tempDir() / "gross3gb.mp3";
    const Bytes head = makeMp3(50);
    Bytes tail(128, 0);
    std::memcpy(tail.data(), "TAG", 3);
    std::memcpy(tail.data() + 3, "Grosser Titel", 13);
    const uint64_t total = 3ull * 1024 * 1024 * 1024;
    if (!makeSparse(p, head, total, tail)) SKIP("sparse files not possible");
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(take(ID3V1GetTitleW()) == L"Grosser Titel");
    AUDIOGetDurationW(); AUDIOGetBitrateW();
    CHECK(AUDIOGetFileSizeW() == 2147483647);      // AUDIOGetFileSizeW returns a long: from 2 GiB on it is clamped to LONG_MAX
    std::error_code ec;
    fs::remove(p, ec);
}

TEST_CASE("Large files: 5 GB FLAC and WavPack (sparse) - read the head", "[special][large]")
{
    const uint64_t total = 5ull * 1024 * 1024 * 1024;
    SECTION("FLAC: STREAMINFO is at the start") {
        Bytes flac = readFile(fixture("flac/no_tags.flac"));
        REQUIRE(flac.size() > 1000);
        flac.resize(1000);                                                       // head with STREAMINFO and metadata is enough
        const fs::path p = tempDir() / "gross5gb.flac";
        if (!makeSparse(p, flac, total)) SKIP("sparse files not possible");
        CHECK(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(AUDIOGetSampleRateW() == 44100);
        std::error_code ec;
        fs::remove(p, ec);
    }
    SECTION("APE tag at the end of a 5 GB file is found") {
        Bytes head = readFile(fixture("wv/no_tags.wv"));
        head.resize(2000);
        const Bytes tag = readFile(fixture("wv/tagged.wv"));
        const size_t pos = std::string(tag.begin(), tag.end()).rfind("APETAGEX");   // Footer
        REQUIRE(pos != std::string::npos);
        const fs::path p = tempDir() / "gross5gb.wv";
        // the footer alone is not enough; take over the whole tag region (last 400 bytes)
        Bytes tail(tag.end() - 400, tag.end());
        if (!makeSparse(p, head, total, tail)) SKIP("sparse files not possible");
        const long fmt = AUDIOAnalyzeFileW(p.c_str());
        INFO("Format " << fmt);
        CHECK(fmt == WAVPACK);
        std::error_code ec;
        fs::remove(p, ec);
    }
}

TEST_CASE("Message processing during an operation (DOEVENTSMILLIS)", "[special][doevents]")
{
    // the value 0: the messages are processed at every opportunity (by default only every 250 ms)
    struct EventsEveryTime {
        long old;
        EventsEveryTime() : old(GetConfigValueW(3)) { SetConfigValueW(3, 0); }
        ~EventsEveryTime() { SetConfigValueW(3, old); }
    } guard;
    auto p = writeTemp("doevents.mp3", makeMp3(40));
    const DWORD thread = GetCurrentThreadId();
    MSG m;
    auto emptyQueue = [&m]() { while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE)) {} };   // also takes a WM_QUIT of an earlier test
    emptyQueue();

    // the messages that are still in the queue (the quit message with its exit code, if there is one)
    auto remaining = [&m]() {
        std::vector<std::pair<UINT, WPARAM>> left;
        while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE))
            left.push_back({ m.message, m.wParam });
        return left;
    };

    SECTION("all waiting messages are dispatched, not one per call") {
        for (int i = 0; i < 10; i++)
            PostThreadMessageW(thread, WM_APP + 1, 0, 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(remaining().empty());
    }
    SECTION("WM_QUIT is posted again for the message loop of the host") {
        PostQuitMessage(42);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        const auto left = remaining();
        REQUIRE(left.size() == 1);
        CHECK(left[0].first == WM_QUIT);
        CHECK(left[0].second == 42);
    }
    SECTION("the messages in front of a WM_QUIT are dispatched, the quit message stays") {
        for (int i = 0; i < 5; i++)
            PostThreadMessageW(thread, WM_APP + 2, 0, 0);
        PostQuitMessage(7);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        const auto left = remaining();
        REQUIRE(left.size() == 1);
        CHECK(left[0].first == WM_QUIT);
        CHECK(left[0].second == 7);
    }
    emptyQueue();
}

namespace {
// what a message handler of the host does while the library saves a file (it runs in CTools::doEvents)
struct HandlerCalls {
    std::wstring otherFile;
    int calls = 0;
    long analyzed = -1, analyzeError = 0;
    long setConfigError = 0;
    std::wstring title;   // a getter is allowed
};
HandlerCalls* g_handler = nullptr;

LRESULT CALLBACK handlerProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    if (msg == WM_APP && g_handler != nullptr && g_handler->calls++ == 0) {
        g_handler->analyzed = AUDIOAnalyzeFileW(g_handler->otherFile.c_str());
        g_handler->analyzeError = AUDIOGetLastErrorNumberW();
        SetConfigValueW(4, 1 << 20);   // the size of the text buffer: it must not change while the library uses it
        g_handler->setConfigError = AUDIOGetLastErrorNumberW();
        g_handler->title = take(AUDIOGetTitleW());
        return 0;
    }
    return DefWindowProcW(h, msg, w, l);
}
}  // namespace

TEST_CASE("A message handler cannot change the data or analyze another file while a file is saved", "[special][doevents]")
{
    struct EventsEveryTime {
        long old;
        EventsEveryTime() : old(GetConfigValueW(3)) { SetConfigValueW(3, 0); }
        ~EventsEveryTime() { SetConfigValueW(3, old); }
    } guard;
    const fs::path dir = freshDir(L"reentry");
    // Before the lock the analysis in the handler changed the name of the last file that the running save works with; depending on the
    // lengths of the paths the save then replaced b with the new a, failed, or read the freed memory of the old name.
    const fs::path a = dir / L"a.mp3";
    const fs::path b = dir / L"b.mp3";
    copyTo(fixture("mp3/tagged.mp3"), a);
    copyTo(fixture("mp3/tagged.mp3"), b);
    const Bytes bBefore = readFile(b);
    const long textBuffer = GetConfigValueW(4);

    WNDCLASSW wc = {};
    wc.lpfnWndProc = handlerProc;
    wc.lpszClassName = L"ag3tests_reentry";
    RegisterClassW(&wc);
    HWND wnd = CreateWindowW(L"ag3tests_reentry", L"", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr);
    REQUIRE(wnd != nullptr);
    HandlerCalls calls;
    calls.otherFile = b.wstring();
    g_handler = &calls;

    REQUIRE(AUDIOAnalyzeFileW(a.c_str()) == MPEG);
    const std::wstring titleOfA = take(AUDIOGetTitleW());
    AUDIOSetCommentW(std::wstring(40000, L'x').c_str());   // a is written again: the copy loop processes the messages
    PostMessageW(wnd, WM_APP, 0, 0);
    const short saved = AUDIOSaveChangesW();
    g_handler = nullptr;
    DestroyWindow(wnd);

    CHECK(saved != 0);
    REQUIRE(calls.calls == 1);
    CHECK(calls.analyzed == 0);
    CHECK(calls.analyzeError == 228);
    CHECK(calls.setConfigError == 228);
    CHECK(GetConfigValueW(4) == textBuffer);
    CHECK(calls.title == titleOfA);   // the getter sees the data of the running save
    CHECK(readFile(b) == bBefore);    // the other file is untouched
    REQUIRE(AUDIOAnalyzeFileW(a.c_str()) == MPEG);
    CHECK(take(AUDIOGetCommentW()).size() == 40000);
}

TEST_CASE("A successful call does not leave the text of an earlier error", "[special][error]")
{
    auto p = writeTemp("error_text.mp3", makeMp3(40));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(AUDIOSaveChangesToFileW(L"") == 0);
    CHECK(!take(AUDIOGetLastErrorTextW()).empty());
    REQUIRE(AUDIOSaveChangesW() != 0);
    CHECK(AUDIOGetLastErrorNumberW() == 0);
    CHECK(take(AUDIOGetLastErrorTextW()).empty());
}
