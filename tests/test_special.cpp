// Sonderfaelle: Dateinamen (Unicode, Sonderzeichen, Endungen, lange Pfade), Dateiattribute und Sperren,
// ungueltige Pfade, sehr grosse (sparse) Dateien.
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <winioctl.h>
#include <algorithm>
#include <cstring>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

fs::path fixture(const char* rel) { return fs::path(AG3_FIXTURES_DIR) / rel; }

// frisches, leeres Unterverzeichnis von %TEMP%/ag3tests
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

// analysieren, Titel aendern, speichern, neu lesen
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

// ============================================================ Dateinamen

TEST_CASE("Dateinamen: Umlaute, CJK, Kyrillisch, Emoji, Leerzeichen und Sonderzeichen", "[special][names]")
{
    const fs::path dir = freshDir(L"namen_\u00e4\u00f6\u00fc \u65e5\u672c");   // auch das Verzeichnis ist Unicode
    const wchar_t* stems[] = {
        L"Gr\u00fc\u00dfe \u00e4\u00f6\u00fc \u00df",
        L"\u65e5\u672c\u8a9e\u306e\u30d5\u30a1\u30a4\u30eb",
        L"\u0444\u0430\u0439\u043b \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0430",
        L"emoji \U0001F3B5 test",                       // ausserhalb der BMP (Surrogatpaar)
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

TEST_CASE("Dateiendung in Grossbuchstaben und gemischt", "[special][names]")
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
    SECTION("Endung nur mit Punkt / ohne Endung: MP3-Inhalt wird nicht erkannt (Endung ist Vorfilter), Formate mit Signatur schon") {
        copyTo(fixture("mp3/tagged.mp3"), dir / L"ohne_endung");
        CHECK(AUDIOAnalyzeFileW((dir / L"ohne_endung").c_str()) == UNKNOWN);
        copyTo(fixture("flac/tagged.flac"), dir / L"flac_ohne_endung");
        CHECK(AUDIOAnalyzeFileW((dir / L"flac_ohne_endung").c_str()) == FLAC);
        copyTo(fixture("flac/tagged.flac"), dir / L"flac.mp3");
        CHECK(AUDIOAnalyzeFileW((dir / L"flac.mp3").c_str()) == FLAC);   // Inhalt zaehlt, nicht die Endung
    }
}

// ============================================================ Lange Pfade

namespace {
// \\?\-Pfad mit Verzeichniskette, dessen Gesamtlaenge 'target' Zeichen erreicht
fs::path longPath(const std::wstring& root, size_t target, const std::wstring& fileName)
{
    std::wstring p = L"\\\\?\\" + root;
    while (p.size() + 1 + fileName.size() < target)
        p += L"\\" + std::wstring(std::min<size_t>(200, target - p.size() - fileName.size() - 2), L'd');
    return fs::path(p + L"\\" + fileName);
}
}  // namespace

TEST_CASE("Lange Pfade: Grenzbereich um MAX_PATH (260) und weit darueber (\\\\?\\-Praefix)", "[special][longpath]")
{
    const fs::path base = freshDir(L"lang");
    for (size_t target : { 240u, 259u, 300u, 600u, 1500u }) {
        DYNAMIC_SECTION("Gesamtlaenge ca. " << target << " Zeichen") {
            const fs::path p = longPath(base.wstring(), target, L"lied.mp3");
            std::error_code ec;
            fs::create_directories(p.parent_path(), ec);
            if (ec) SKIP("Verzeichnis nicht anlegbar: " << ec.message());
            copyTo(fixture("mp3/tagged.mp3"), p);
            // getrennt von roundTrip: bei langen Pfaden ist entscheidend, dass nichts abstuerzt und ein Fehler sauber gemeldet wird
            const long fmt = AUDIOAnalyzeFileW(p.c_str());
            INFO("Pfadlaenge " << p.wstring().size());
            REQUIRE(fmt == MPEG);                       // mit \?\-Praefix funktionieren Pfade bis mindestens 1500 Zeichen
            AUDIOSetTitleW(L"Lang");
            CHECK(AUDIOSaveChangesW() != 0);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(AUDIOGetTitleW()) == L"Lang");
            std::error_code ec2;
            fs::remove_all(base, ec2);
        }
    }
}

// ============================ Regressionen aus dem Missbrauchstest

TEST_CASE("Nach fehlgeschlagener Analyse darf Speichern die vorher analysierte Datei nicht veraendern", "[special][regression]")
{
    const fs::path p = tempDir() / "vorher.mp3";
    copyTo(fixture("mp3/tagged.mp3"), p);
    const Bytes original = readFile(p);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(AUDIOAnalyzeFileW(L"Z:\\gibt\\es\\nicht.mp3") == UNKNOWN);   // leert die Felder, oeffnet aber keine Datei
    AUDIOSetTitleW(L"Anderer Titel");
    CHECK(AUDIOSaveChangesW() == 0);                                       // frueher: schrieb die leeren Felder in die alte Datei
    CHECK(AUDIOGetLastFileW() != nullptr);
    CHECK(take(AUDIOGetLastFileW()) == L"");
    CHECK(readFile(p) == original);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"Testtitel");
}

TEST_CASE("Kapitel-ID als Elternelement (kein CTOC), NULL-/Kurz-Bildarrays fuehren nicht zum Absturz", "[special][regression]")
{
    const fs::path p = tempDir() / "regress.mp3";
    copyTo(fixture("mp3/id3v24_comm.mp3"), p);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2AddChapterW(L"chp", L"Kapitel", L"", 0, 1000) == 0);
    CHECK(ID3V2AddChildElementW(L"chp", L"x") == 0);        // Kapitel hat keine Unterelemente (frueher: Absturz durch falschen Cast)
    CHECK(ID3V2DeleteChildElementW(L"chp", L"x") == 0);
    CHECK(ID3V2AddChildElementW(L"", L"x") == 0);
    BYTE tiny[3] = { 1, 2, 3 };
    CHECK(MP4AddPictureArrayW(nullptr, 0) == 0);            // frueher: Absturz beim Lesen von arr[0]
    CHECK(MP4AddPictureArrayW(tiny, 3) == 0);
}

// ================================= Ungueltige Pfade und Nicht-Dateien

TEST_CASE("Ungueltige Pfade: NULL, leer, Verzeichnis, Platzhalter, Laufwerk, sehr lang", "[special][invalid]")
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
    CHECK(AUDIOAnalyzeFileW(tempDir().c_str()) == UNKNOWN);            // Verzeichnis
    CHECK(AUDIOAnalyzeFileW(L"C:\\") == UNKNOWN);
    CHECK(AUDIOAnalyzeFileW(std::wstring(100000, L'x').c_str()) == UNKNOWN);   // 100 000 Zeichen
    CHECK(AUDIOAnalyzeFileW((std::wstring(50000, L'x') + L".mp3").c_str()) == UNKNOWN);

    // nach lauter Fehlern funktioniert die naechste gueltige Analyse
    REQUIRE(AUDIOAnalyzeFileW(fixture("mp3/tagged.mp3").c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"Testtitel");

    // Speichern ohne vorherige gueltige Analyse
    AUDIOAnalyzeFileW(L"Z:\\gibt\\es\\nicht.mp3");
    CHECK(AUDIOSaveChangesW() == 0);
    CHECK(AUDIOSaveChangesToFileW(nullptr) == 0);
    CHECK(AUDIOSaveChangesToFileW(L"") == 0);
    CHECK(AUDIOSaveChangesToFileW(L"Z:\\gibt\\es\\nicht.mp3") == 0);
}

TEST_CASE("Leere und winzige Dateien mit jeder Endung", "[special][invalid]")
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
            INFO("Endung " << fs::path(ext).string() << ", Groesse " << size);
            CHECK(fmt == UNKNOWN);
            AUDIOGetDurationW(); take(AUDIOGetTitleW());
            AUDIOSetTitleW(L"x");
            CHECK(AUDIOSaveChangesW() == 0);
        }
    }
}

// ========================================= Schreibschutz und Sperren

namespace {
bool setReadOnly(const fs::path& p, bool ro)
{
    DWORD a = GetFileAttributesW(p.c_str());
    if (a == INVALID_FILE_ATTRIBUTES) return false;
    return SetFileAttributesW(p.c_str(), ro ? (a | FILE_ATTRIBUTE_READONLY) : (a & ~FILE_ATTRIBUTE_READONLY)) != 0;
}
}  // namespace

TEST_CASE("Schreibgeschuetzte Dateien: Lesen geht, Speichern schlaegt sauber fehl und aendert nichts", "[special][readonly]")
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
            CHECK(readFile(p) == original);            // Datei unveraendert

            // Attribut entfernt: dasselbe Speichern klappt jetzt
            REQUIRE(setReadOnly(p, false));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == s.format);
            AUDIOSetTitleW(L"Jetzt schon");
            CHECK(AUDIOSaveChangesW() != 0);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == s.format);
            CHECK(take(AUDIOGetTitleW()) == L"Jetzt schon");
        }
    }
}

TEST_CASE("Gesperrte Dateien: Lesesperre erlaubt Lesen, Schreiben scheitert; exklusive Sperre verhindert schon das Lesen", "[special][lock]")
{
    const fs::path dir = freshDir(L"lock");
    for (const Sample& s : { kSamples[0], kSamples[1], kSamples[3], kSamples[4] }) {
        DYNAMIC_SECTION(s.rel) {
            const fs::path p = dir / fs::path(s.rel).filename();
            copyTo(fixture(s.rel), p);
            const Bytes original = readFile(p);

            SECTION("andere Anwendung liest (Lesezugriff, Teilen nur zum Lesen)") {
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
                CHECK(AUDIOAnalyzeFileW(p.c_str()) == s.format);     // nach dem Freigeben geht es wieder
            }
        }
    }
}

// ================================================ Grosse (sparse) Dateien

namespace {

// Legt eine sparse-Datei an: 'head' am Anfang, 'tail' am Ende, dazwischen ungespeicherte Nullen. Liefert false, wenn das
// Dateisystem keine Sparse-Dateien kann.
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
        Bytes b = makeWav(sampleRate, channels, 0.0);        // 44-Byte-Header ohne Daten
        auto put32 = [&](size_t off, uint32_t v) { for (int i = 0; i < 4; i++) b[off + i] = static_cast<uint8_t>(v >> (8 * i)); };
        put32(4, 36 + dataSize);
        put32(40, dataSize);
        return b;
    }
};

}  // namespace

TEST_CASE("Grosse Dateien: 3 GB WAV (sparse) - Dauer ohne 32-Bit-Ueberlauf", "[special][large]")
{
    const fs::path p = tempDir() / "gross3gb.wav";
    const uint64_t dataSize = 3ull * 1024 * 1024 * 1024;                    // 3 GiB PCM = 3 * 2^30 / 176400 s
    if (!makeSparse(p, Wav::header(static_cast<uint32_t>(dataSize), 44100, 2), 44 + dataSize)) SKIP("Sparse-Dateien nicht moeglich");
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(AUDIOGetChannelsW() == 2);
    CHECK(AUDIOGetDurationW() == Catch::Approx(static_cast<double>(dataSize) / 176400.0).epsilon(0.001));   // ~18 256 s
    CHECK(AUDIOGetDurationMillisW() > 0);
    std::error_code ec;
    fs::remove(p, ec);
}

TEST_CASE("Grosse Dateien: 3 GB MP3 (sparse) mit ID3v1-Tag am Ende", "[special][large]")
{
    const fs::path p = tempDir() / "gross3gb.mp3";
    const Bytes head = makeMp3(50);
    Bytes tail(128, 0);
    std::memcpy(tail.data(), "TAG", 3);
    std::memcpy(tail.data() + 3, "Grosser Titel", 13);
    const uint64_t total = 3ull * 1024 * 1024 * 1024;
    if (!makeSparse(p, head, total, tail)) SKIP("Sparse-Dateien nicht moeglich");
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(take(ID3V1GetTitleW()) == L"Grosser Titel");
    AUDIOGetDurationW(); AUDIOGetBitrateW();
    CHECK(AUDIOGetFileSizeW() == 2147483647);      // AUDIOGetFileSizeW liefert ein long: ab 2 GiB wird auf LONG_MAX begrenzt
    std::error_code ec;
    fs::remove(p, ec);
}

TEST_CASE("Grosse Dateien: 5 GB FLAC und WavPack (sparse) - Kopf lesen", "[special][large]")
{
    const uint64_t total = 5ull * 1024 * 1024 * 1024;
    SECTION("FLAC: STREAMINFO steht am Anfang") {
        Bytes flac = readFile(fixture("flac/no_tags.flac"));
        REQUIRE(flac.size() > 1000);
        flac.resize(1000);                                                       // Kopf mit STREAMINFO und Metadaten genuegt
        const fs::path p = tempDir() / "gross5gb.flac";
        if (!makeSparse(p, flac, total)) SKIP("Sparse-Dateien nicht moeglich");
        CHECK(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(AUDIOGetSampleRateW() == 44100);
        std::error_code ec;
        fs::remove(p, ec);
    }
    SECTION("APE-Tag am Ende einer 5-GB-Datei wird gefunden") {
        Bytes head = readFile(fixture("wv/no_tags.wv"));
        head.resize(2000);
        const Bytes tag = readFile(fixture("wv/tagged.wv"));
        const size_t pos = std::string(tag.begin(), tag.end()).rfind("APETAGEX");   // Footer
        REQUIRE(pos != std::string::npos);
        const fs::path p = tempDir() / "gross5gb.wv";
        // Footer allein reicht nicht; ganze Tag-Region (letzte 400 Byte) uebernehmen
        Bytes tail(tag.end() - 400, tag.end());
        if (!makeSparse(p, head, total, tail)) SKIP("Sparse-Dateien nicht moeglich");
        const long fmt = AUDIOAnalyzeFileW(p.c_str());
        INFO("Format " << fmt);
        CHECK(fmt == WAVPACK);
        std::error_code ec;
        fs::remove(p, ec);
    }
}
