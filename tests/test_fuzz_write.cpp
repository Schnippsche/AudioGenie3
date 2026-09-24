// Fuzzing der Schreibpfade: beschaedigte Kopien aller Fixtures analysieren und danach Tags/Bilder/Frames aendern,
// speichern und erneut lesen. Erwartung: kein Absturz (unter ASan: kein Speicherfehler).
// Anzahl der Durchlaeufe: Umgebungsvariable AG3_FUZZ_ROUNDS (Standard 1).
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <algorithm>
#include <cstdlib>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include <set>
#include <string>
#ifdef _WIN32
#include <process.h>
#endif

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

struct Rng {
    uint32_t seed;
    uint32_t next() { seed = seed * 1664525u + 1013904223u; return seed >> 8; }
    size_t below(size_t n) { return n ? next() % n : 0; }
};

int rounds()
{
    const char* env = std::getenv("AG3_FUZZ_ROUNDS");
    return env ? std::max(1, std::atoi(env)) : 1;
}

// alle Audio-Fixtures (rekursiv), auch die unter broken/
std::vector<fs::path> allFixtures()
{
    static const std::set<std::string> ext = { ".mp3", ".wav", ".flac", ".ogg", ".m4a", ".wma", ".wv", ".tta", ".aac", ".ape", ".mpc" };
    std::vector<fs::path> out;
    if (!fs::exists(AG3_FIXTURES_DIR)) return out;
    for (const auto& e : fs::recursive_directory_iterator(AG3_FIXTURES_DIR)) {
        if (!e.is_regular_file()) continue;
        std::string x = e.path().extension().string();
        std::transform(x.begin(), x.end(), x.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (ext.count(x)) out.push_back(e.path());
    }
    std::sort(out.begin(), out.end());
    return out;
}

// Ablaufprotokoll fuer die Suche nach Haengern: AG3_FUZZ_TRACE=1 schreibt jeden Schritt nach %TEMP%/ag3tests/trace.log
// und legt vor jedem Zyklus eine Kopie der Eingabe als last_cycle_input.<ext> ab.
void trace(const char* step)
{
    if (!std::getenv("AG3_FUZZ_TRACE")) return;
    FILE* f = nullptr;
    if (fopen_s(&f, (tempDir() / "trace.log").string().c_str(), "a") == 0 && f) { fprintf(f, "%s\n", step); fclose(f); }
}

fs::path coverPath() { return fs::path(AG3_FIXTURES_DIR) / "cover_64x64.jpg"; }

void readAll()
{
    AUDIOGetDurationW(); AUDIOGetBitrateW(); AUDIOGetChannelsW(); AUDIOGetSampleRateW();
    take(AUDIOGetTitleW()); take(AUDIOGetArtistW()); take(AUDIOGetAlbumW()); take(AUDIOGetYearW());
    take(AUDIOGetTrackW()); take(AUDIOGetGenreW()); take(AUDIOGetCommentW()); take(AUDIOGetComposerW());
    take(AUDIOGetMD5ValueW());
}

const wchar_t* pickText(Rng& r)
{
    static const wchar_t* texts[] = {
        L"", L"a", L"Titel", L"Grüße äöü", L"€ 日本語 Ж",
        L"Zeile1\r\nZeile2", L"mit;Semikolon=und\"Anfuehrung\"",
        L"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
        L"1999", L"7", L"3/12", L"Rock",
    };
    return texts[r.below(sizeof(texts) / sizeof(texts[0]))];
}

// Waechter gegen Haenger: dauert ein einzelner Bearbeitungszyklus laenger als 30 s, wird die Eingabedatei gesichert
// (%TEMP%/ag3tests/hang_input.<ext>), eine Meldung ausgegeben und der Prozess mit Exitcode 99 beendet, statt minutenlang zu warten.
struct Watchdog {
    static std::mutex& mtx() { static std::mutex m; return m; }
    static std::chrono::steady_clock::time_point& started() { static std::chrono::steady_clock::time_point t; return t; }
    static std::string& current() { static std::string s; return s; }
    static std::atomic<bool>& active() { static std::atomic<bool> a{ false }; return a; }
    static void ensureThread()
    {
        static std::once_flag once;
        std::call_once(once, [] {
            std::thread([] {
                for (;;) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (!active()) continue;
                    std::lock_guard<std::mutex> g(mtx());
                    if (std::chrono::steady_clock::now() - started() < std::chrono::seconds(30)) continue;
                    fprintf(stderr, "\nHAENGER: Zyklus laeuft seit >30 s auf %s (Kopie der Eingabe: %s/ag3tests/hang_input.*), Abbruch\n",
                        current().c_str(), tempDir().parent_path().string().c_str());
                    fflush(stderr);
                    std::error_code ec;
                    for (const char* x : { ".aac", ".mp3", ".wav", ".flac", ".ogg", ".m4a", ".wma", ".wv", ".tta", ".ape", ".mpc" }) {
                        const fs::path in = tempDir() / (std::string("last_cycle_input") + x);
                        if (fs::exists(in, ec)) fs::copy_file(in, tempDir() / (std::string("hang_input") + x), fs::copy_options::overwrite_existing, ec);
                    }
                    _exit(99);
                }
            }).detach();
        });
    }
    explicit Watchdog(const fs::path& f)
    {
        ensureThread();
        std::lock_guard<std::mutex> g(mtx());
        started() = std::chrono::steady_clock::now();
        current() = f.filename().string();
        active() = true;
    }
    ~Watchdog() { active() = false; }
};

// Aenderungen ueber die formatunabhaengige API
void abstractEdits(Rng& r)
{
    AUDIOSetTitleW(pickText(r));
    AUDIOSetArtistW(pickText(r));
    AUDIOSetAlbumW(pickText(r));
    AUDIOSetYearW(pickText(r));
    AUDIOSetTrackW(pickText(r));
    AUDIOSetGenreW(pickText(r));
    AUDIOSetCommentW(pickText(r));
    AUDIOSetComposerW(pickText(r));
}

// formatspezifische Schreiboperationen; liefert nichts, es zaehlt nur das Ueberleben
void formatEdits(long fmt, Rng& r)
{
    const std::wstring cover = coverPath().wstring();
    const bool haveCover = fs::exists(coverPath());
    switch (fmt) {
    case FLAC:
        if (haveCover) FLACAddPictureFileW(cover.c_str(), pickText(r), static_cast<short>(r.below(21)), 0);
        if (r.below(3) == 0) FLACDeletePicturesW();
        else if (r.below(3) == 0) FLACDeletePictureW(static_cast<short>(1 + r.below(3)));
        FLACSetUserItemW(L"CUSTOM", pickText(r));
        AUDIOSaveChangesW();
        break;
    case MP4M4A:
        if (haveCover) MP4AddPictureFileW(cover.c_str());
        if (r.below(3) == 0) MP4DeletePicturesW();
        MP4SetiTuneFrameW(L"----:com.example:X", pickText(r));
        AUDIOSaveChangesW();
        break;
    case WMA:
        if (haveCover) WMAAddPictureFileW(cover.c_str(), pickText(r), static_cast<short>(r.below(21)), 1);
        if (r.below(3) == 0) WMADeletePicturesW();
        WMASetUserItemW(L"WM/Custom", pickText(r));
        AUDIOSaveChangesW();
        break;
    case WAV:
        WAVSetTextFrameW(WAV_INAM, pickText(r));
        WAVSetTextFrameW(WAV_ICMT, pickText(r));
        WAVSetDisplayTextW(pickText(r));
        WAVSaveChangesW();
        break;
    case OGGVORBIS:
        OGGSetUserItemW(L"CUSTOM", pickText(r));
        AUDIOSaveChangesW();
        break;
    case MONKEY: case WAVPACK:
        APESetUserItemW(L"Custom", pickText(r));
        AUDIOSaveChangesW();
        break;
    case MPEG: case AAC: case TTA: case MPEGPLUS: default:
        ID3V2SetFormatAndEncodingW(static_cast<short>(r.below(4)), static_cast<short>(r.below(4)));
        ID3V2SetTextFrameW(ID3F_TIT2, pickText(r));
        ID3V2AddCommentW(L"eng", pickText(r), pickText(r));
        ID3V2AddUserTextW(pickText(r), pickText(r));
        ID3V2AddChapterW(L"c1", pickText(r), pickText(r), 0, 1000);
        if (haveCover) ID3V2AddPictureFileW(cover.c_str(), pickText(r), static_cast<short>(r.below(21)), 0);
        if (r.below(3) == 0) ID3V2DeleteAllFramesW(ID3F_APIC);
        if (r.below(4) == 0) ID3V2RemoveTagW();
        else if (r.below(2) == 0) ID3V2SaveChangesW();
        else AUDIOSaveChangesW();
        break;
    }
}

// Analyse -> Aenderungen -> Speichern -> Neu einlesen -> alle Getter
void editCycle(const fs::path& file, Rng& r)
{
    // die Eingabe immer sichern (klein), damit der Waechter sie bei einem Haenger ablegen kann
    fs::copy_file(file, tempDir() / ("last_cycle_input" + file.extension().string()), fs::copy_options::overwrite_existing);
    Watchdog wd(file);
    trace(("--- Zyklus " + file.filename().string() + " seed=" + std::to_string(r.seed)).c_str());
    trace("analyze");
    const long fmt = AUDIOAnalyzeFileW(file.c_str());
    readAll();
    if (fmt == UNKNOWN) return;
    trace("abstractEdits");
    abstractEdits(r);
    trace("save1");
    AUDIOSaveChangesW();
    trace("reanalyze1");
    AUDIOAnalyzeFileW(file.c_str());
    readAll();
    trace("formatEdits");
    formatEdits(fmt, r);
    trace("reanalyze2");
    AUDIOAnalyzeFileW(file.c_str());
    readAll();
    trace("fertig");
}

fs::path tempWithExt(const char* stem, const fs::path& like, const Bytes& data)
{
    return writeTemp(std::string(stem) + like.extension().string(), data);   // Endung erhalten: MP3/AAC werden nur damit erkannt
}

}  // namespace

TEST_CASE("Fuzz: Schreibpfade auf unveraenderten Fixture-Kopien (zufaellige Operationsfolgen)", "[fuzz][write]")
{
    const auto files = allFixtures();
    if (files.empty()) SKIP("keine Fixtures gefunden");
    Rng r{ 20240924u };
    for (const fs::path& f : files) {
        DYNAMIC_SECTION(fs::relative(f, AG3_FIXTURES_DIR).generic_string()) {
            const Bytes orig = readFile(f);
            for (int i = 0; i < 6 * rounds(); i++) {
                const fs::path p = tempWithExt("wfuzz_ok", f, orig);
                editCycle(p, r);
                editCycle(p, r);   // zweiter Durchlauf auf dem bereits veraenderten File
            }
            SUCCEED("kein Absturz");
        }
    }
}

TEST_CASE("Fuzz: Schreibpfade auf beschaedigten Fixture-Kopien", "[fuzz][write]")
{
    const auto files = allFixtures();
    if (files.empty()) SKIP("keine Fixtures gefunden");
    Rng r{ 4711u };
    for (const fs::path& f : files) {
        DYNAMIC_SECTION(fs::relative(f, AG3_FIXTURES_DIR).generic_string()) {
            const Bytes orig = readFile(f);
            if (orig.empty()) continue;
            const size_t head = std::min<size_t>(orig.size(), 4096);
            for (int i = 0; i < 20 * rounds(); i++) {
                Bytes b = orig;
                switch (r.below(4)) {
                case 0: for (int k = 0; k < 3; k++) b[r.below(head)] = static_cast<uint8_t>(r.next()); break;          // Kopfbereich
                case 1: for (int k = 0; k < 3; k++) b[b.size() - 1 - r.below(std::min<size_t>(b.size(), 512))] = static_cast<uint8_t>(r.next()); break;  // Dateiende (APE/ID3v1)
                case 2: b.resize(r.below(b.size())); break;                                                              // abgeschnitten
                default: b[r.below(b.size())] = static_cast<uint8_t>(r.next()); b[r.below(head)] ^= 0xFF; break;         // verstreut
                }
                const fs::path p = tempWithExt("wfuzz_bad", f, b);
                editCycle(p, r);
            }
            SUCCEED("kein Absturz");
        }
    }
}
