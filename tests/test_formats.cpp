// Datengetriebene Tests ueber die mit fixtures\generate.bat erzeugten Dateien.
// Erwartungen = was ffmpeg geschrieben hat (1 s Sinus, 44,1 kHz, Stereo bzw. 22,05 kHz Mono).
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <algorithm>
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
};

const wchar_t* kStdTags[FieldCount] = { L"Testtitel", L"Testkuenstler", L"Testalbum", L"2024", L"3", L"Rock", L"Kommentar" };

// Bekannte Luecken: ffmpeg schreibt das Feld, die DLL liefert es nicht (leer). Diese Felder werden im
// Test "Tags lesen" nicht geprueft, sondern im Test "[gaps]" als erwartet fehlschlagend.
struct Gap { const char* file; Field field; const char* why; };
const Gap kGaps[] = {
    { "mp3/tagged.mp3",     Year,    "ID3v2.4-Frame TDRC wird nicht gelesen (nur TYER aus v2.3)" },
    { "mp3/tagged.mp3",     Comment, "COMM-Frame aus ffmpeg wird nicht als Kommentar geliefert" },
    { "mp3/with_cover.mp3", Comment, "COMM-Frame aus ffmpeg wird nicht als Kommentar geliefert" },
    { "flac/tagged.flac",   Comment, "Vorbis-Feld COMMENT wird nicht gelesen" },
    { "flac/with_cover.flac", Comment, "Vorbis-Feld COMMENT wird nicht gelesen" },
    { "ogg/tagged.ogg",     Comment, "Vorbis-Feld COMMENT wird nicht gelesen" },
    { "wma/tagged.wma",     Year,    "WM/Year wird nicht gelesen (ffmpeg schreibt WM/Year)" },
    { "wma/tagged.wma",     Comment, "Description wird nicht als Kommentar geliefert" },
    { "wv/tagged.wv",       Year,    "APE-Feld 'date' statt 'Year'" },
    { "tta/tagged.tta",     Year,    "APE-Feld 'date' statt 'Year'" },
    { "wav/tagged.wav",     Track,   "INFO-Feld ITRK wird nicht als Track geliefert" },
};

bool isGap(const char* file, Field f)
{
    for (const Gap& g : kGaps) if (!strcmp(g.file, file) && g.field == f) return true;
    return false;
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
                if (fx.tagged && isGap(fx.file, f)) continue;
                INFO("Feld " << kFieldName[i]);
                CHECK(getField(f) == (fx.tagged ? kStdTags[i] : L""));
            }
        }
    }
}

TEST_CASE("Formate: bekannte Luecken beim Lesen", "[formats][gaps][!shouldfail]")
{
    // Schlaegt absichtlich fehl (Catch2: !shouldfail). Wird eine Luecke in der DLL geschlossen, schlaegt dieser
    // Test um -> Eintrag aus kGaps entfernen, dann wird das Feld im normalen Test mitgeprueft.
    for (const Gap& g : kGaps) {
        INFO(g.file << ": " << g.why);
        REQUIRE(AUDIOAnalyzeFileW(fixturePath(g.file).c_str()) != UNKNOWN);
        CHECK(getField(g.field) == kStdTags[g.field]);
    }
}

TEST_CASE("Formate: Tags schreiben (Round-Trip), Audiodaten bleiben unveraendert", "[formats][tags][roundtrip]")
{
    const std::wstring title = L"Neuer Titel äöü €";
    for (const Fixture& fx : kFixtures) {
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
        if (!fx.tagged) continue;
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
    uint32_t seed = 4711;
    auto rnd = [&] { seed = seed * 1664525u + 1013904223u; return seed >> 8; };

    for (const Fixture& fx : kFixtures) {
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("Fixture fehlt: " << fx.file);
            const Bytes orig = readFile(fixturePath(fx.file));
            REQUIRE(!orig.empty());
            const size_t head = std::min<size_t>(orig.size(), 4096);   // Header und Tags liegen am Anfang

            // 1) abgeschnitten an verschiedenen Stellen
            for (int i = 0; i < 12; i++) {
                const size_t len = rnd() % orig.size();
                Bytes b(orig.begin(), orig.begin() + len);
                AUDIOAnalyzeFileW(writeTemp("cut.bin", b).c_str());
                take(AUDIOGetTitleW()); AUDIOGetDurationW();
            }
            // 2) einzelne Bytes im Kopfbereich veraendert
            for (int i = 0; i < 40; i++) {
                Bytes b = orig;
                for (int k = 0; k < 3; k++) b[rnd() % head] = static_cast<uint8_t>(rnd());
                AUDIOAnalyzeFileW(writeTemp("mut.bin", b).c_str());
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
