// Robustheit gegen ungueltige Argumente: jede exportierte Funktion (ausser Speichern/Entfernen) wird mit zufaelligen,
// teils ungueltigen Werten aufgerufen (NULL, leere/riesige Strings, Index 0/-1/32767, unbekannte Frame-IDs), auf jeweils einer
// analysierten Datei jedes Formats. Erwartung: kein Absturz (unter ASan: kein Speicherfehler).
// Die Aufrufliste steht in misuse_calls.inc (erzeugt von gen_misuse.py); AG3_FUZZ_ROUNDS erhoeht die Aufrufe je Funktion.
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <cstdlib>
#include <string>
#include <vector>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

struct Rng {
    uint32_t seed;
    uint32_t next() { seed = seed * 1664525u + 1013904223u; return seed >> 8; }
    size_t below(size_t n) { return n ? next() % n : 0; }
};

struct Buffer { BYTE* ptr; u32 len; };

// AG3_MISUSE_SKIP=Fn1,Fn2: diese Funktionen auslassen (bekannte Absturzkandidaten, um weitere zu finden)
bool skipped(const char* fn)
{
    static const std::string list = [] { const char* e = std::getenv("AG3_MISUSE_SKIP"); return std::string(",") + (e ? e : "") + ","; }();
    return list.find(std::string(",") + fn + ",") != std::string::npos;
}

// AG3_MISUSE_TRACE=1: jeden Aufruf vor der Ausfuehrung nach %TEMP%/ag3tests/misuse_trace.log schreiben (Suche nach Absturzursachen)
std::string g_currentFile;
void traceCall(const char* fn, int n)
{
    static const bool on = std::getenv("AG3_MISUSE_TRACE") != nullptr;
    if (!on) return;
    static FILE* f = nullptr;
    if (!f) fopen_s(&f, (tempDir() / "misuse_trace.log").string().c_str(), "w");
    if (f) { fprintf(f, "%s %s #%d\n", g_currentFile.c_str(), fn, n); fflush(f); }
}

const wchar_t* pickString(Rng& r, bool isFileName)
{
    static const std::wstring longText(5000, L'x');
    static const std::wstring unicode = L"\u00e4\u00f6\u00fc \u20ac \u65e5\u672c\u8a9e \U0001F3B5";
    // Dateinamen: nur solche, die nirgends etwas anlegen koennen (einige Funktionen schreiben Dateien)
    static const wchar_t* fileNames[] = { nullptr, L"", L"Z:\\gibt\\es\\nicht\\x.bin", L"<>|?*.bin", L"NUL\\x" };
    static const wchar_t* texts[] = { nullptr, L"", L"a", L"Titel", L"eng", L"http://example.com", nullptr };
    if (isFileName) return fileNames[r.below(sizeof(fileNames) / sizeof(fileNames[0]))];
    switch (r.below(4)) {
    case 0: return longText.c_str();
    case 1: return unicode.c_str();
    default: return texts[r.below(sizeof(texts) / sizeof(texts[0]))];
    }
}

short pickShort(Rng& r)
{
    static const short v[] = { 0, -1, 1, 2, 3, 4, 7, 100, 255, 256, 32767, -32768 };
    return v[r.below(sizeof(v) / sizeof(v[0]))];
}

long pickLong(Rng& r)
{
    static const long v[] = { 0, 1, -1, 2, 1000, 65536, 2147483647L, -2147483647L - 1 };
    return v[r.below(sizeof(v) / sizeof(v[0]))];
}

u32 pickId(Rng& r)
{
    static const u32 v[] = { 0, 1, 0xFFFFFFFFu, 0x80000000u, ID3F_TIT2, ID3F_APIC, ID3F_TXXX, ID3F_COMM, ID3F_CHAP, ID3F_CTOC, ID3F_SYLT,
                             ID3F_PRIV, ID3F_POPM, ID3F_TYER, ID3F_TDRC, 0x41424344u, 0x54494D45u, WAV_INAM, WAV_IART };
    return v[r.below(sizeof(v) / sizeof(v[0]))];
}

Buffer pickBuffer(Rng& r, std::vector<BYTE>& buf)
{
    // Puffer ist immer 4096 Byte gross; Laenge nie darueber (sonst waere es ein Aufruferfehler). NULL nur mit Laenge 0.
    static const u32 lens[] = { 0, 1, 2, 7, 64, 1000, 4096 };
    const u32 len = lens[r.below(sizeof(lens) / sizeof(lens[0]))];
    if (len == 0 && r.below(2) == 0) return { nullptr, 0 };
    for (size_t i = 0; i < 64; i++) buf[i] = static_cast<BYTE>(r.next());
    return { buf.data(), len };
}

// Einige Frames setzen, damit die Getter etwas finden
void prepareId3v2()
{
    ID3V2SetTextFrameW(ID3F_TIT2, L"Titel");
    ID3V2AddCommentW(L"eng", L"", L"Kommentar");
    ID3V2AddUserTextW(L"MOOD", L"froh");
    ID3V2AddChapterW(L"chp1", L"Kapitel", L"", 0, 1000);
    ID3V2AddTableOfContentW(L"toc1", L"Inhalt", L"", 1);
    std::vector<BYTE> d(20, 7);
    ID3V2AddPrivateFrameW(d.data(), 20, L"owner");
    ID3V2AddPopularimeterW(L"a@b.c", 100, 5);
    ID3V2AddSyncLyricW(L"eng", L"x", L"[00000010]Zeile\r\n", 1, 2);
}

void battery(const std::string& name, const fs::path& file, uint32_t seed)
{
    g_currentFile = name;
    Rng r{ seed };
    const int rounds = std::getenv("AG3_FUZZ_ROUNDS") ? std::max(1, std::atoi(std::getenv("AG3_FUZZ_ROUNDS"))) * 30 : 30;
    std::vector<BYTE> buf(4096);
    if (AUDIOAnalyzeFileW(file.c_str()) == UNKNOWN) FAIL("nicht analysierbar: " << file.string());
    prepareId3v2();
    int called = 0;
    const std::string only;   // leer = alle Funktionen
    (void)only;
#define name only
#include "misuse_calls.inc"
#undef name
    REQUIRE(called > 300);   // die Liste deckt (fast) die ganze API ab
    (void)name;
}

}  // namespace

TEST_CASE("Ungueltige Argumente: alle Funktionen auf einer Datei je Format", "[misuse]")
{
    struct F { const char* rel; };
    const F files[] = { {"mp3/id3v24_comm.mp3"}, {"mp3/id3v23_comm.mp3"}, {"flac/with_cover.flac"}, {"ogg/tagged.ogg"}, {"m4a/with_cover.m4a"},
                        {"wma/tagged.wma"}, {"wav/tagged.wav"}, {"wv/tagged.wv"}, {"tta/tagged.tta"}, {"ape/tagged.ape"},
                        {"mpc/sv8_tagged_ape.mpc"}, {"mpc/sv7_tagged_ape.mpc"}, {"aac/adts_id3v24_comm.aac"}, {"ape/tagged_id3v1.ape"} };
    uint32_t seed = 99;
    for (const F& f : files) {
        DYNAMIC_SECTION(f.rel) {
            const fs::path src = fs::path(AG3_FIXTURES_DIR) / f.rel;
            if (!fs::exists(src)) SKIP("Fixture fehlt");
            const fs::path p = tempDir() / ("misuse" + src.extension().string());
            fs::copy_file(src, p, fs::copy_options::overwrite_existing);
            battery(f.rel, p, seed++);
            SUCCEED("kein Absturz");
        }
    }
}
