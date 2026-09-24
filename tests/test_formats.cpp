// Datengetriebene Tests ueber die mit fixtures\generate.bat erzeugten Dateien.
// Erwartungen = was ffmpeg geschrieben hat (1 s Sinus, 44,1 kHz, Stereo bzw. 22,05 kHz Mono).
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <future>
#include <memory>
#include <process.h>
#include <thread>
#include <cstring>

using namespace ag3test;
namespace fs = std::filesystem;

namespace {

enum Field { Title, Artist, Album, Year, Track, Genre, Comment, FieldCount };
const char* kFieldName[FieldCount] = { "Titel", "Interpret", "Album", "Jahr", "Track", "Genre", "Kommentar" };

std::wstring getField(Field f)
{
    switch (f) {
    case Title:   return take(AUDIOGetTitleW());
    case Artist:  return take(AUDIOGetArtistW());
    case Album:   return take(AUDIOGetAlbumW());
    case Year:    return take(AUDIOGetYearW());
    case Track:   return take(AUDIOGetTrackW());
    case Genre:   return take(AUDIOGetGenreW());
    case Comment: return take(AUDIOGetCommentW());
    default:      return L"";
    }
}

struct Fixture {
    const char* file;        // relativ zu tests\fixtures
    AudioFormatID format;
    long sampleRate;
    long channels;
    double duration;         // Sekunden
    double margin;           // Toleranz (Encoder-Padding, Schaetzung aus Dateigroesse)
    bool tagged;             // enthaelt die Standard-Tags (Testtitel, ...)
    bool payloadMd5Stable;   // AUDIOGetMD5ValueW aendert sich beim Tag-Schreiben nicht (bei WAV und WavPack
                             // fliesst der Tag in den MD5 ein, bei AAC die Dateigroesse)
};

const Fixture kFixtures[] = {
    { "mp3/id3v1_only.mp3",   MPEG,      44100, 2, 1.071, 0.15, true,  true  },   // nur ID3v1-Tag am Dateiende
    { "mp3/id3v23_comm.mp3",  MPEG,      44100, 2, 1.045, 0.15, true,  true  },   // ID3v2.3 von Hand: TYER, COMM
    { "mp3/id3v24_comm.mp3",  MPEG,      44100, 2, 1.045, 0.15, true,  true  },   // ID3v2.4 von Hand: TDRC-Zeitstempel, COMM
    { "mp3/no_tags_cbr.mp3",  MPEG,      44100, 2, 1.045, 0.15, false, true  },
    { "mp3/no_tags_xing.mp3", MPEG,      44100, 2, 1.071, 0.15, false, true  },
    { "mp3/tagged.mp3",       MPEG,      44100, 2, 1.071, 0.15, true,  true  },
    { "mp3/with_cover.mp3",   MPEG,      44100, 2, 1.071, 0.15, true,  true  },
    { "mp3/mono_22k.mp3",     MPEG,      22050, 1, 1.097, 0.15, false, true  },
    { "wav/no_tags.wav",      WAV,       44100, 2, 1.0,   0.01, false, false },
    { "wav/tagged.wav",       WAV,       44100, 2, 1.0,   0.01, true,  false },
    { "wav/mono_22k.wav",     WAV,       22050, 1, 1.0,   0.01, false, false },
    { "flac/no_tags.flac",    FLAC,      44100, 2, 1.0,   0.01, false, true  },
    { "flac/tagged.flac",     FLAC,      44100, 2, 1.0,   0.01, true,  true  },
    { "flac/with_cover.flac", FLAC,      44100, 2, 1.0,   0.01, true,  true  },
    { "flac/mono_22k.flac",   FLAC,      22050, 1, 1.0,   0.01, false, true  },
    { "ogg/no_tags.ogg",      OGGVORBIS, 44100, 2, 1.0,   0.05, false, true  },
    { "ogg/tagged.ogg",       OGGVORBIS, 44100, 2, 1.0,   0.05, true,  true  },
    { "ogg/mono_22k.ogg",     OGGVORBIS, 22050, 1, 1.0,   0.05, false, true  },
    { "m4a/no_tags.m4a",      MP4M4A,    44100, 2, 1.023, 0.05, false, true  },
    { "m4a/tagged.m4a",       MP4M4A,    44100, 2, 1.023, 0.05, true,  true  },
    { "m4a/with_cover.m4a",   MP4M4A,    44100, 2, 1.023, 0.05, true,  true  },
    { "m4a/alac_tagged.m4a",  MP4M4A,    44100, 2, 1.0,   0.05, true,  true  },
    { "wma/no_tags.wma",      WMA,       44100, 2, 1.0,   0.05, false, true  },
    { "wma/tagged.wma",       WMA,       44100, 2, 1.0,   0.05, true,  true  },
    { "wv/no_tags.wv",        WAVPACK,   44100, 2, 1.0,   0.01, false, false },
    { "wv/tagged.wv",         WAVPACK,   44100, 2, 1.0,   0.01, true,  false },
    { "tta/no_tags.tta",      TTA,       44100, 2, 1.0,   0.01, false, true  },
    { "tta/tagged.tta",       TTA,       44100, 2, 1.0,   0.01, true,  true  },
    // Roher ADTS-Strom: Dauer wird aus Dateigroesse und Bitrate geschaetzt, daher grosse Toleranz;
    // beim Schreiben eines ID3v2-Tags aendert sich die Schaetzung (Tag zaehlt mit).
    { "aac/no_tags.aac",      AAC,       44100, 2, 1.06,  0.1,  false, false },
    // APE (Monkey's Audio 13.26 aus den WAV-Fixtures); die Datei-Version meldet die DLL als 3.99. Der MD5 schliesst den Tag ein.
    { "ape/no_tags_c1000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/no_tags_c2000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/no_tags_c4000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/no_tags_c5000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/mono_22k.ape",        MONKEY, 22050, 1, 1.0, 0.01, false, false },
    { "ape/tagged.ape",          MONKEY, 44100, 2, 1.0, 0.01, true,  false },
    { "ape/tagged_id3v1.ape",    MONKEY, 44100, 2, 1.0, 0.01, true,  false },   // ID3v1 statt APE-Tag am Dateiende
    // Musepack SV7 (synthetische Header, siehe make_mpc_fixtures.py): Dauer = Frames * 1152 / Samplerate
    { "mpc/sv7_synthetic_standard.mpc",         MPEGPLUS, 44100, 2, 2.612, 0.01, false, true  },
    { "mpc/sv7_synthetic_thumb_joint_48k.mpc",  MPEGPLUS, 48000, 2, 4.8,   0.01, false, true  },
    { "mpc/sv7_synthetic_insane_32k.mpc",       MPEGPLUS, 32000, 2, 1.8,   0.01, false, true  },
    { "mpc/sv7_synthetic_tagged_ape.mpc",       MPEGPLUS, 44100, 2, 2.612, 0.01, true,  false },
    { "mpc/sv7_synthetic_tagged_id3v2.mpc",     MPEGPLUS, 44100, 2, 2.612, 0.01, true,  false },
    // Roher ADTS-Strom aus den realen Samples (generate.bat); Dauer aus Dateigroesse und Bitrate geschaetzt.
    { "aac/adts_sample-1.aac",     AAC,  44100, 2, 5.06,  0.2,  false, false },
    { "aac/adts_id3_sample-2.aac", AAC,  44100, 2, 5.06,  0.2,  true,  false },
    { "aac/adts_id3v24_comm.aac",  AAC,  44100, 2, 5.06,  0.2,  true,  false },   // ID3v2.4 von Hand, echter COMM-Frame
    // Reale MP4-Dateien (AAC-LC, 320 kbit) mit der Endung .aac, auf 5 s gekuerzt (Originale: tests/fixtures_local/aac).
    // Dauer laut DLL 5,062 s, ffprobe meldet 5,015 s (Encoder-Priming).
    { "aac/sample-1.aac",     MP4M4A,    44100, 2, 5.05,  0.1,  false, true  },
    { "aac/sample-2.aac",     MP4M4A,    44100, 2, 5.05,  0.1,  false, true  },
    { "aac/sample-5.aac",     MP4M4A,    44100, 2, 5.05,  0.1,  false, true  },
};

const wchar_t* kStdTags[FieldCount] = { L"Testtitel", L"Testkuenstler", L"Testalbum", L"2024", L"3", L"Rock", L"Kommentar" };

// Eigenheiten der Encoder: ein Standardfeld bleibt bewusst leer, weil der Encoder es woanders ablegt.
// ffmpeg schreibt den Kommentar in ID3v2-Tags als TXXX:comment statt als COMM-Frame; die DLL liest COMM
// (Fixtures mit echtem COMM: mp3/id3v23_comm.mp3, mp3/id3v24_comm.mp3, aac/adts_id3v24_comm.aac).
struct Quirk { const char* file; Field field; const char* why; };
const Quirk kQuirks[] = {
    { "mp3/tagged.mp3",            Comment, "ffmpeg: TXXX:comment statt COMM" },
    { "mp3/with_cover.mp3",        Comment, "ffmpeg: TXXX:comment statt COMM" },
    { "aac/adts_id3_sample-2.aac", Comment, "ffmpeg: TXXX:comment statt COMM" },
};

bool isQuirk(const char* file, Field f)
{
    for (const Quirk& q : kQuirks) if (!strcmp(q.file, file) && q.field == f) return true;
    return false;
}

// Fixtures, bei denen der Standard-Round-Trip nicht gilt: AAC wird als APE-Tag geschrieben (siehe Doku von
// AUDIOSaveChangesW); ein vorhandener ID3v2-Tag am Dateianfang hat beim Lesen Vorrang (eigener Test unten).
bool skipWriteTests(const char* file) { return !strcmp(file, "aac/adts_id3_sample-2.aac") || !strcmp(file, "aac/adts_id3v24_comm.aac"); }

// Bricht den Prozess mit Exitcode 98 ab, wenn f nicht innerhalb von 'seconds' fertig wird (Haenger statt Endlosschleife melden).
template <class F> void mustFinishWithin(int seconds, const char* what, F f)
{
    auto done = std::make_shared<std::promise<void>>();
    auto fut = done->get_future();
    std::thread([done, f]() mutable { f(); done->set_value(); }).detach();
    if (fut.wait_for(std::chrono::seconds(seconds)) != std::future_status::ready) {
        fprintf(stderr, "\nHAENGER: %s dauert laenger als %d s\n", what, seconds);
        fflush(stderr);
        _exit(98);
    }
}

fs::path fixturePath(const char* rel) { return fs::path(AG3_FIXTURES_DIR) / rel; }

// Kopie im Temp-Verzeichnis, damit die Fixtures unveraendert bleiben.
fs::path copyToTemp(const char* rel)
{
    fs::path dst = tempDir() / fs::path(rel).filename();
    fs::copy_file(fixturePath(rel), dst, fs::copy_options::overwrite_existing);
    return dst;
}

}  // namespace

TEST_CASE("Formate: Eckdaten aller Fixtures", "[formats]")
{
    for (const Fixture& fx : kFixtures) {
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("Fixture fehlt: " << fx.file);
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(fx.file).c_str()) == fx.format);
            CHECK(AUDIOGetSampleRateW() == fx.sampleRate);
            CHECK(AUDIOGetChannelsW() == fx.channels);
            CHECK(AUDIOGetDurationW() == Catch::Approx(fx.duration).margin(fx.margin));
            CHECK(AUDIOGetLastErrorNumberW() == 0);
            CHECK(AUDIOGetFileSizeW() == static_cast<long>(fs::file_size(fixturePath(fx.file))));
        }
    }
}

TEST_CASE("Formate: Tags lesen", "[formats][tags]")
{
    for (const Fixture& fx : kFixtures) {
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("Fixture fehlt: " << fx.file);
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(fx.file).c_str()) == fx.format);
            for (int i = 0; i < FieldCount; i++) {
                const Field f = static_cast<Field>(i);
                INFO("Feld " << kFieldName[i]);
                const wchar_t* expected = fx.tagged && !isQuirk(fx.file, f) ? kStdTags[i] : L"";
                CHECK(getField(f) == expected);
            }
        }
    }
}

TEST_CASE("Musepack SV8 (MPCK, mpcenc 1.30) wird erkannt", "[formats][mpc][!shouldfail]")
{
    // Erwartet fehlschlagend: die DLL kennt nur Stream-Version 4-7 ('MP+'), der aktuelle Musepack-Encoder schreibt SV8 ('MPCK').
    // Wird SV8 unterstuetzt, schlaegt dieser Test um: dann in kFixtures aufnehmen und diesen Test entfernen.
    for (const char* rel : { "mpc/sv8_thumb.mpc", "mpc/sv8_standard.mpc", "mpc/sv8_insane.mpc", "mpc/sv8_mono_44k.mpc" }) {
        INFO(rel);
        REQUIRE(fs::exists(fixturePath(rel)));
        CHECK(AUDIOAnalyzeFileW(fixturePath(rel).c_str()) == MPEGPLUS);
    }
}

TEST_CASE("AAC mit vorhandenem ID3v2-Tag: Schreiben landet im APE-Tag, ID3v2 hat beim Lesen Vorrang", "[formats][tags]")
{
    // Dokumentiertes Verhalten (AUDIOSaveChangesW: AAC -> APE-Tag): der vorhandene ID3v2-Tag bleibt unveraendert,
    // die neuen Werte stehen in einem angehaengten APE-Tag und werden erst gelesen, wenn kein ID3v2-Tag existiert.
    const fs::path p = copyToTemp("aac/adts_id3_sample-2.aac");
    const Bytes before = readFile(p);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
    AUDIOSetTitleW(L"Neuer Titel");
    REQUIRE(AUDIOSaveChangesW() != 0);

    const Bytes after = readFile(p);
    REQUIRE(after.size() > before.size());
    CHECK(std::equal(before.begin(), before.end(), after.begin()));   // ID3v2-Tag und Audio unveraendert davor
    const std::string tail(after.begin() + before.size(), after.end());
    CHECK(tail.find("APETAGEX") != std::string::npos);                // APE-Tag angehaengt

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
    CHECK(getField(Title) == L"Testtitel");                           // ID3v2 hat Vorrang
}

TEST_CASE("Formate: Tags schreiben (Round-Trip), Audiodaten bleiben unveraendert", "[formats][tags][roundtrip]")
{
    const std::wstring title = L"Neuer Titel äöü €";
    for (const Fixture& fx : kFixtures) {
        if (skipWriteTests(fx.file)) continue;
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("Fixture fehlt: " << fx.file);
            const fs::path p = copyToTemp(fx.file);

            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fx.format);
            const std::wstring md5 = take(AUDIOGetMD5ValueW());

            AUDIOSetTitleW(title.c_str());
            AUDIOSetArtistW(L"Neuer Kuenstler");
            AUDIOSetAlbumW(L"Neues Album");
            AUDIOSetYearW(L"1999");
            AUDIOSetTrackW(L"7");
            AUDIOSetGenreW(L"Jazz");
            AUDIOSetCommentW(L"Neuer Kommentar");
            AUDIOSetComposerW(L"Komponist");
            REQUIRE(AUDIOSaveChangesW() != 0);

            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fx.format);
            CHECK(getField(Title) == title);
            CHECK(getField(Artist) == L"Neuer Kuenstler");
            CHECK(getField(Album) == L"Neues Album");
            CHECK(getField(Year) == L"1999");
            CHECK(getField(Track) == L"7");
            CHECK(getField(Genre) == L"Jazz");
            CHECK(getField(Comment) == L"Neuer Kommentar");
            CHECK(take(AUDIOGetComposerW()) == L"Komponist");

            CHECK(AUDIOGetSampleRateW() == fx.sampleRate);
            CHECK(AUDIOGetChannelsW() == fx.channels);
            CHECK(AUDIOGetDurationW() == Catch::Approx(fx.duration).margin(fx.margin));
            if (fx.payloadMd5Stable) CHECK(take(AUDIOGetMD5ValueW()) == md5);
        }
    }
}

TEST_CASE("Formate: Tags loeschen durch Leerwerte", "[formats][tags][roundtrip]")
{
    for (const Fixture& fx : kFixtures) {
        if (!fx.tagged || skipWriteTests(fx.file)) continue;
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("Fixture fehlt: " << fx.file);
            const fs::path p = copyToTemp(fx.file);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fx.format);
            AUDIOSetTitleW(L""); AUDIOSetArtistW(L""); AUDIOSetAlbumW(L"");
            REQUIRE(AUDIOSaveChangesW() != 0);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fx.format);
            CHECK(getField(Title) == L"");
            CHECK(getField(Artist) == L"");
            CHECK(getField(Album) == L"");
            CHECK(getField(Genre) == L"Rock");   // nicht geleerte Felder bleiben erhalten
        }
    }
}

TEST_CASE("ID3v1: Textfelder werden ohne NUL-/Leerzeichen-Auffuellung geliefert", "[formats][tags][id3v1]")
{
    // ID3v1-Felder haben feste Laenge (30 Byte), aufgefuellt meist mit NUL-Bytes.
    for (const char* rel : { "mp3/id3v1_only.mp3", "ape/tagged_id3v1.ape" }) {
        DYNAMIC_SECTION(rel) {
            if (!fs::exists(fixturePath(rel))) SKIP("Fixture fehlt: " << rel);
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(rel).c_str()) != UNKNOWN);
            CHECK(ID3V1ExistsW() != 0);
            CHECK(take(ID3V1GetTitleW()) == L"Testtitel");
            CHECK(take(ID3V1GetArtistW()) == L"Testkuenstler");
            CHECK(take(ID3V1GetAlbumW()) == L"Testalbum");
            CHECK(take(ID3V1GetYearW()) == L"2024");
            CHECK(take(ID3V1GetCommentW()) == L"Kommentar");
            CHECK(take(ID3V1GetTrackW()) == L"3");
            // dieselben Werte ueber die abstrakten Felder
            CHECK(getField(Title) == L"Testtitel");
            CHECK(getField(Comment) == L"Kommentar");
        }
    }
}

// ------------------------------------------------------------------ Cover

TEST_CASE("Cover: vorhandene Bilder lesen", "[formats][cover]")
{
    const fs::path jpg = fixturePath("cover_64x64.jpg");
    if (!fs::exists(jpg)) SKIP("cover_64x64.jpg fehlt");
    const long jpgSize = static_cast<long>(fs::file_size(jpg));

    SECTION("MP3 (ID3v2 APIC)") {
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("mp3/with_cover.mp3").c_str()) == MPEG);
        CHECK(ID3V2GetFrameCountW(ID3F_APIC) == 1);
        CHECK(ID3V2GetPictureSizeW(1) == jpgSize);
        CHECK(take(ID3V2GetPictureMimeW(1)) == L"image/jpeg");
    }
    SECTION("FLAC (PICTURE-Block)") {
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("flac/with_cover.flac").c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 1);
        CHECK(FLACGetPictureSizeW(1) == jpgSize);
        CHECK(take(FLACGetPictureMimeW(1)) == L"image/jpeg");
    }
    SECTION("M4A (covr-Atom)") {
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("m4a/with_cover.m4a").c_str()) == MP4M4A);
        CHECK(MP4GetPictureCountW() == 1);
        CHECK(MP4GetPictureSizeW(1) == jpgSize);
    }
    SECTION("Dateien ohne Cover") {
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("mp3/tagged.mp3").c_str()) == MPEG);
        CHECK(ID3V2GetFrameCountW(ID3F_APIC) == 0);
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("flac/tagged.flac").c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 0);
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("m4a/tagged.m4a").c_str()) == MP4M4A);
        CHECK(MP4GetPictureCountW() == 0);
    }
}

TEST_CASE("Cover: Bild hinzufuegen, Round-Trip, Audiodaten unveraendert", "[formats][cover][roundtrip]")
{
    const fs::path jpg = fixturePath("cover_64x64.jpg");
    if (!fs::exists(jpg)) SKIP("cover_64x64.jpg fehlt");
    const long jpgSize = static_cast<long>(fs::file_size(jpg));

    SECTION("MP3") {
        const fs::path p = copyToTemp("mp3/tagged.mp3");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        const std::wstring md5 = take(AUDIOGetMD5ValueW());
        REQUIRE(ID3V2AddPictureFileW(jpg.c_str(), L"Front", COVER_FRONT, 0) != 0);
        REQUIRE(ID3V2SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(ID3V2GetFrameCountW(ID3F_APIC) == 1);
        CHECK(ID3V2GetPictureSizeW(1) == jpgSize);
        CHECK(ID3V2GetPictureTypeW(1) == COVER_FRONT);
        CHECK(take(ID3V2GetPictureDescriptionW(1)) == L"Front");
        CHECK(take(AUDIOGetTitleW()) == L"Testtitel");
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
    }
    SECTION("FLAC") {
        const fs::path p = copyToTemp("flac/tagged.flac");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        const std::wstring md5 = take(AUDIOGetMD5ValueW());
        REQUIRE(FLACAddPictureFileW(jpg.c_str(), L"Front", COVER_FRONT, 0) != 0);
        REQUIRE(AUDIOSaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 1);
        CHECK(FLACGetPictureSizeW(1) == jpgSize);
        CHECK(FLACGetPictureTypeW(1) == COVER_FRONT);
        CHECK(take(AUDIOGetTitleW()) == L"Testtitel");
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
    }
    SECTION("M4A") {
        const fs::path p = copyToTemp("m4a/tagged.m4a");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        const std::wstring md5 = take(AUDIOGetMD5ValueW());
        REQUIRE(MP4AddPictureFileW(jpg.c_str()) != 0);
        REQUIRE(AUDIOSaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(MP4GetPictureCountW() == 1);
        CHECK(MP4GetPictureSizeW(1) == jpgSize);
        CHECK(take(AUDIOGetTitleW()) == L"Testtitel");
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
    }
    SECTION("FLAC: Bild loeschen") {
        const fs::path p = copyToTemp("flac/with_cover.flac");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        REQUIRE(FLACGetPictureCountW() == 1);
        FLACDeletePicturesW();
        REQUIRE(AUDIOSaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 0);
    }
}

// ------------------------------------------------------------ Kaputte Dateien

TEST_CASE("Kaputte Dateien: Erkennung und kein Absturz", "[formats][broken]")
{
    struct Case { const char* file; AudioFormatID expected; };
    // Charakterisierung des aktuellen Verhaltens: abgeschnittene Dateien werden noch als ihr Format
    // erkannt, ein 128-Byte-MP3-Rest nicht.
    const Case cases[] = {
        { "broken/tagged_half.mp3",        MPEG },
        { "broken/tagged_header_only.mp3", UNKNOWN },
        { "broken/tagged_half.flac",       FLAC },
        { "broken/tagged_header_only.flac", FLAC },
        { "broken/tagged_half.ogg",        OGGVORBIS },
        { "broken/tagged_half.m4a",        MP4M4A },
        { "broken/tagged_half.wma",        WMA },
        { "broken/tagged_header_only.wav", WAV },
    };
    for (const Case& c : cases) {
        DYNAMIC_SECTION(c.file) {
            if (!fs::exists(fixturePath(c.file))) SKIP("Fixture fehlt: " << c.file);
            CHECK(AUDIOAnalyzeFileW(fixturePath(c.file).c_str()) == c.expected);
            // alle Getter duerfen auf halb gelesenen Daten nicht abstuerzen
            for (int i = 0; i < FieldCount; i++) getField(static_cast<Field>(i));
            AUDIOGetDurationW(); AUDIOGetBitrateW(); AUDIOGetChannelsW(); AUDIOGetSampleRateW();
            take(AUDIOGetMD5ValueW());
            take(AUDIOGetLastErrorTextW());
        }
    }
}

TEST_CASE("Kaputte Dateien: Tags speichern in abgeschnittene Datei stuerzt nicht ab", "[formats][broken]")
{
    for (const char* rel : { "broken/tagged_half.mp3", "broken/tagged_half.flac", "broken/tagged_half.ogg",
                             "broken/tagged_half.m4a", "broken/tagged_half.wma" }) {
        DYNAMIC_SECTION(rel) {
            if (!fs::exists(fixturePath(rel))) SKIP("Fixture fehlt: " << rel);
            const fs::path p = copyToTemp(rel);
            AUDIOAnalyzeFileW(p.c_str());
            AUDIOSetTitleW(L"Reparaturversuch");
            AUDIOSaveChangesW();   // Ergebnis offen (Erfolg oder Fehler), es darf nur nicht abstuerzen
            AUDIOAnalyzeFileW(p.c_str());
            SUCCEED("kein Absturz");
        }
    }
}

// -------------------------------------------- Zufaellige Beschaedigung echter Dateien

TEST_CASE("Beschaedigte Fixtures: kein Absturz beim Analysieren", "[formats][robust]")
{
    // Umgebungsvariable AG3_FUZZ_ROUNDS vervielfacht die Durchlaeufe (z. B. 30 fuer einen langen ASan-Lauf).
    const char* env = std::getenv("AG3_FUZZ_ROUNDS");
    const int rounds = env ? std::max(1, std::atoi(env)) : 1;
    uint32_t seed = 4711;
    auto rnd = [&] { seed = seed * 1664525u + 1013904223u; return seed >> 8; };

    for (const Fixture& fx : kFixtures) {
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("Fixture fehlt: " << fx.file);
            const Bytes orig = readFile(fixturePath(fx.file));
            REQUIRE(!orig.empty());
            const size_t head = std::min<size_t>(orig.size(), 4096);   // Header und Tags liegen am Anfang

            // 1) abgeschnitten an verschiedenen Stellen
            for (int i = 0; i < 12 * rounds; i++) {
                const size_t len = rnd() % orig.size();
                Bytes b(orig.begin(), orig.begin() + len);
                AUDIOAnalyzeFileW(writeTemp("cut" + fs::path(fx.file).extension().string(), b).c_str());   // Endung erhalten (MP3/AAC werden nur damit erkannt)
                take(AUDIOGetTitleW()); AUDIOGetDurationW();
            }
            // 2) einzelne Bytes im Kopfbereich veraendert
            for (int i = 0; i < 40 * rounds; i++) {
                Bytes b = orig;
                for (int k = 0; k < 3; k++) b[rnd() % head] = static_cast<uint8_t>(rnd());
                AUDIOAnalyzeFileW(writeTemp("mut" + fs::path(fx.file).extension().string(), b).c_str());
                for (int f = 0; f < FieldCount; f++) getField(static_cast<Field>(f));
                AUDIOGetDurationW(); take(AUDIOGetMD5ValueW());
            }
            // 3) Dateiendung passt nicht zum Inhalt
            AUDIOAnalyzeFileW(writeTemp("falsche_endung.mp3", orig).c_str());
            AUDIOAnalyzeFileW(writeTemp("falsche_endung.flac", orig).c_str());
            SUCCEED("kein Absturz");
        }
    }
}

// ------------------------------------------- Regressionen aus ASan-Funden (Fuzzing)

// Diese Dateien haben in der DLL Speicherfehler ausgeloest, die nur unter AddressSanitizer auffielen
// (FlacCover.cpp, WavPack.cpp); sie bleiben als Regressionen erhalten.
TEST_CASE("ASan-Regressionen: kaputte Dateien", "[formats][robust][asan-regression]")
{
    SECTION("FLAC: PICTURE-Block mit falscher Laengenangabe (Lesen ueber das Pufferende, FlacCover.cpp)") {
        const auto p = fixturePath("broken/flac_cover_length_overflow.flac");
        if (!fs::exists(p)) SKIP("Fixture fehlt");
        AUDIOAnalyzeFileW(p.c_str());
        FLACGetPictureCountW(); FLACGetPictureSizeW(1);
        SUCCEED("kein Absturz");
    }
    SECTION("MP4: entry_count des stco-Atoms groesser als der Atom-Inhalt (Beinahe-Endlosschleife beim Speichern, MP4_STCO.cpp)") {
        const auto src = fixturePath("broken/mp4_stco_count_overflow.m4a");
        if (!fs::exists(src)) SKIP("Fixture fehlt");
        const fs::path p = copyToTemp("broken/mp4_stco_count_overflow.m4a");
        mustFinishWithin(30, "AUDIOSaveChangesW auf M4A mit kaputtem stco", [&] {
            AUDIOAnalyzeFileW(p.c_str());
            AUDIOSetTitleW(L"Reparaturversuch");
            AUDIOSaveChangesW();
            AUDIOAnalyzeFileW(p.c_str());
        });
        SUCCEED("beendet");
    }
    SECTION("WavPack: Sample-Rate-Index 15 (Tabellenzugriff ausserhalb, WavPack.cpp GetSampleRate)") {
        const auto p = fixturePath("broken/wavpack_samplerate_index.wv");
        if (!fs::exists(p)) SKIP("Fixture fehlt");
        AUDIOAnalyzeFileW(p.c_str());
        AUDIOGetDurationW(); AUDIOGetSampleRateW();
        SUCCEED("kein Absturz");
    }
}
