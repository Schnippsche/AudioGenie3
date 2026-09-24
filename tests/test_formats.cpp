// Data-driven tests over the files created by fixtures\generate.bat.
// Expectations = what ffmpeg wrote (1 s sine, 44.1 kHz, stereo or 22.05 kHz mono).
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
const char* kFieldName[FieldCount] = { "Title", "Artist", "Album", "Year", "Track", "Genre", "Comment" };

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
    double margin;           // tolerance (encoder padding, estimate from file size)
    bool tagged;             // enthaelt die Standard-Tags (Testtitel, ...)
    bool payloadMd5Stable;   // AUDIOGetMD5ValueW does not change when tags are written (for WAV and WavPack
                             // the tag is part of the MD5, for AAC the file size)
};

const Fixture kFixtures[] = {
    { "mp3/id3v1_only.mp3",   MPEG,      44100, 2, 1.071, 0.15, true,  true  },   // ID3v1 tag at the end of the file only
    { "mp3/id3v23_comm.mp3",  MPEG,      44100, 2, 1.045, 0.15, true,  true  },   // ID3v2.3 built by hand: TYER, COMM
    { "mp3/id3v24_comm.mp3",  MPEG,      44100, 2, 1.045, 0.15, true,  true  },   // ID3v2.4 built by hand: TDRC timestamp, COMM
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
    // Raw ADTS stream: the duration is estimated from file size and bit rate, hence the large tolerance;
    // when an ID3v2 tag is written the estimate changes (the tag counts too).
    { "aac/no_tags.aac",      AAC,       44100, 2, 1.06,  0.1,  false, false },
    // APE (Monkey's Audio 13.26 from the WAV fixtures); the DLL reports the file version as 3.99. The MD5 includes the tag.
    { "ape/no_tags_c1000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/no_tags_c2000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/no_tags_c4000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/no_tags_c5000.ape",   MONKEY, 44100, 2, 1.0, 0.01, false, false },
    { "ape/mono_22k.ape",        MONKEY, 22050, 1, 1.0, 0.01, false, false },
    { "ape/tagged.ape",          MONKEY, 44100, 2, 1.0, 0.01, true,  false },
    { "ape/tagged_id3v1.ape",    MONKEY, 44100, 2, 1.0, 0.01, true,  false },   // ID3v1 instead of an APE tag at the end of the file
    // Musepack SV8 (mpcenc 1.30): 1 s, Samplezahl im SH-Paket
    { "mpc/sv8_thumb.mpc",      MPEGPLUS, 44100, 2, 1.0, 0.001, false, true  },
    { "mpc/sv8_standard.mpc",   MPEGPLUS, 44100, 2, 1.0, 0.001, false, true  },
    { "mpc/sv8_insane.mpc",     MPEGPLUS, 44100, 2, 1.0, 0.001, false, true  },
    { "mpc/sv8_mono_44k.mpc",   MPEGPLUS, 44100, 1, 1.0, 0.001, false, true  },
    { "mpc/sv8_tagged_ape.mpc", MPEGPLUS, 44100, 2, 1.0, 0.001, true,  false },
    { "mpc/sv8_tagged_id3v2.mpc", MPEGPLUS, 44100, 2, 1.0, 0.001, true, false },
    // Musepack SV7 real (mppenc 1.16): 39 or 28/42 frames; duration according to header and ffprobe 1.0188 s or 1.008 s
    { "mpc/sv7_thumb.mpc",      MPEGPLUS, 44100, 2, 1.0188, 0.001, false, true  },
    { "mpc/sv7_radio.mpc",      MPEGPLUS, 44100, 2, 1.0188, 0.001, false, true  },
    { "mpc/sv7_standard.mpc",   MPEGPLUS, 44100, 2, 1.0188, 0.001, false, true  },
    { "mpc/sv7_extreme.mpc",    MPEGPLUS, 44100, 2, 1.0188, 0.001, false, true  },
    { "mpc/sv7_insane.mpc",     MPEGPLUS, 44100, 2, 1.0188, 0.001, false, true  },
    { "mpc/sv7_thumb_48k.mpc",  MPEGPLUS, 48000, 2, 1.008,  0.001, false, true  },
    { "mpc/sv7_thumb_32k.mpc",  MPEGPLUS, 32000, 2, 1.008,  0.001, false, true  },
    { "mpc/sv7_mono_44k.mpc",   MPEGPLUS, 44100, 2, 1.0188, 0.001, false, true  },   // SV7 has no mono: encoded as stereo
    { "mpc/sv7_tagged_ape.mpc", MPEGPLUS, 44100, 2, 1.0188, 0.001, true,  false },
    { "mpc/sv7_tagged_id3v2.mpc", MPEGPLUS, 44100, 2, 1.0188, 0.001, true, false },
    // Musepack SV7 (synthetische Header, siehe make_mpc_fixtures.py): Dauer = Frames * 1152 / Samplerate
    { "mpc/sv7_synthetic_standard.mpc",         MPEGPLUS, 44100, 2, 2.612, 0.01, false, true  },
    { "mpc/sv7_synthetic_thumb_joint_48k.mpc",  MPEGPLUS, 48000, 2, 4.8,   0.01, false, true  },
    { "mpc/sv7_synthetic_insane_32k.mpc",       MPEGPLUS, 32000, 2, 1.8,   0.01, false, true  },
    { "mpc/sv7_synthetic_tagged_ape.mpc",       MPEGPLUS, 44100, 2, 2.612, 0.01, true,  false },
    { "mpc/sv7_synthetic_tagged_id3v2.mpc",     MPEGPLUS, 44100, 2, 2.612, 0.01, true,  false },
    // Raw ADTS stream from the real samples (generate.bat); duration estimated from file size and bit rate.
    { "aac/adts_sample-1.aac",     AAC,  44100, 2, 5.06,  0.2,  false, false },
    { "aac/adts_id3_sample-2.aac", AAC,  44100, 2, 5.06,  0.2,  true,  false },
    { "aac/adts_id3v24_comm.aac",  AAC,  44100, 2, 5.06,  0.2,  true,  false },   // ID3v2.4 by hand, real COMM frame
    // Real MP4 files (AAC-LC, 320 kbit) with the extension .aac, cut to 5 s (originals: tests/fixtures_local/aac).
    // Dauer laut DLL 5,062 s, ffprobe meldet 5,015 s (Encoder-Priming).
    { "aac/sample-1.aac",     MP4M4A,    44100, 2, 5.05,  0.1,  false, true  },
    { "aac/sample-2.aac",     MP4M4A,    44100, 2, 5.05,  0.1,  false, true  },
    { "aac/sample-5.aac",     MP4M4A,    44100, 2, 5.05,  0.1,  false, true  },
};

const wchar_t* kStdTags[FieldCount] = { L"Testtitel", L"Testkuenstler", L"Testalbum", L"2024", L"3", L"Rock", L"Kommentar" };

// Encoder quirks: a standard field intentionally stays empty because the encoder stores it elsewhere.
// ffmpeg writes the comment in ID3v2 tags as TXXX:comment instead of a COMM frame; the DLL reads COMM
// (fixtures with a real COMM: mp3/id3v23_comm.mp3, mp3/id3v24_comm.mp3, aac/adts_id3v24_comm.aac).
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

// Fixtures for which the standard round trip does not apply: AAC is written as an APE tag (see the documentation of
// AUDIOSaveChangesW); an existing ID3v2 tag at the start of the file takes precedence when reading (separate test below).
bool skipWriteTests(const char* file) { return !strcmp(file, "aac/adts_id3_sample-2.aac") || !strcmp(file, "aac/adts_id3v24_comm.aac"); }

// Aborts the process with exit code 98 if f does not finish within 'seconds' (report a hang instead of an endless loop).
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

// Copy in the temp directory so that the fixtures stay unchanged.
fs::path copyToTemp(const char* rel)
{
    fs::path dst = tempDir() / fs::path(rel).filename();
    fs::copy_file(fixturePath(rel), dst, fs::copy_options::overwrite_existing);
    return dst;
}

}  // namespace

TEST_CASE("Formats: basic data of all fixtures", "[formats]")
{
    for (const Fixture& fx : kFixtures) {
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("fixture missing: " << fx.file);
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(fx.file).c_str()) == fx.format);
            CHECK(AUDIOGetSampleRateW() == fx.sampleRate);
            CHECK(AUDIOGetChannelsW() == fx.channels);
            CHECK(AUDIOGetDurationW() == Catch::Approx(fx.duration).margin(fx.margin));
            CHECK(AUDIOGetLastErrorNumberW() == 0);
            CHECK(AUDIOGetFileSizeW() == static_cast<long>(fs::file_size(fixturePath(fx.file))));
        }
    }
}

TEST_CASE("Formats: read tags", "[formats][tags]")
{
    for (const Fixture& fx : kFixtures) {
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("fixture missing: " << fx.file);
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(fx.file).c_str()) == fx.format);
            for (int i = 0; i < FieldCount; i++) {
                const Field f = static_cast<Field>(i);
                INFO("field " << kFieldName[i]);
                const wchar_t* expected = fx.tagged && !isQuirk(fx.file, f) ? kStdTags[i] : L"";
                CHECK(getField(f) == expected);
            }
        }
    }
}

TEST_CASE("Musepack SV7 (mppenc 1.16): profile and channel mode", "[formats][mpc]")
{
    struct Case { const char* file; const wchar_t* profile; };
    const Case cases[] = {
        { "mpc/sv7_thumb.mpc", L"Thumb" }, { "mpc/sv7_radio.mpc", L"Radio" }, { "mpc/sv7_standard.mpc", L"Standard" },
        { "mpc/sv7_extreme.mpc", L"Xtreme" }, { "mpc/sv7_insane.mpc", L"Insane" }, { "mpc/sv7_thumb_48k.mpc", L"Thumb" },
    };
    for (const Case& c : cases) {
        DYNAMIC_SECTION(c.file) {
            if (!fs::exists(fixturePath(c.file))) SKIP("fixture missing");
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(c.file).c_str()) == MPEGPLUS);
            CHECK(take(AUDIOGetVersionW()) == c.profile);
            CHECK(take(AUDIOGetChannelModeW()) == L"Joint Stereo");   // mppenc nutzt Mid/Side
            CHECK(AUDIOGetBitrateW() > 20);
            CHECK(AUDIOGetBitrateW() < 200);
        }
    }
}

TEST_CASE("Musepack SV8: profile, channel mode, bit rate and stream version", "[formats][mpc]")
{
    struct Case { const char* file; long channels; const wchar_t* profile; const wchar_t* mode; };
    const Case cases[] = {
        { "mpc/sv8_thumb.mpc",     2, L"Thumb",    L"Joint Stereo" },
        { "mpc/sv8_standard.mpc",  2, L"Standard", L"Joint Stereo" },
        { "mpc/sv8_insane.mpc",    2, L"Insane",   L"Joint Stereo" },
        { "mpc/sv8_mono_44k.mpc",  1, L"Standard", L"Mono" },
    };
    for (const Case& c : cases) {
        DYNAMIC_SECTION(c.file) {
            if (!fs::exists(fixturePath(c.file))) SKIP("fixture missing");
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(c.file).c_str()) == MPEGPLUS);
            CHECK(AUDIOGetChannelsW() == c.channels);
            CHECK(take(AUDIOGetVersionW()) == c.profile);
            CHECK(take(AUDIOGetChannelModeW()) == c.mode);
            CHECK(AUDIOGetDurationW() == Catch::Approx(1.0).margin(0.001));
            CHECK(AUDIOGetBitrateW() > 20);
            CHECK(AUDIOGetBitrateW() < 200);
        }
    }
}

TEST_CASE("Musepack SV8: systematically modify each of the first 48 header bytes (no crash)", "[formats][mpc][robust]")
{
    const auto src = fixturePath("mpc/sv8_standard.mpc");
    if (!fs::exists(src)) SKIP("fixture missing");
    const Bytes orig = readFile(src);
    REQUIRE(orig.size() > 200);
    int recognized = 0;
    for (size_t pos = 0; pos < 48; pos++) {
        for (uint8_t v : { 0x00, 0x01, 0x7F, 0x80, 0xFF }) {
            Bytes b = orig;
            b[pos] = v;
            const long fmt = AUDIOAnalyzeFileW(writeTemp("sv8_mut.mpc", b).c_str());
            AUDIOGetDurationW(); AUDIOGetBitrateW(); AUDIOGetChannelsW(); AUDIOGetSampleRateW();
            take(AUDIOGetVersionW()); take(AUDIOGetChannelModeW());
            if (fmt == MPEGPLUS) recognized++;
        }
    }
    CHECK(recognized > 0);   // changes outside the SH packet must not destroy the recognition
    // truncated at every position of the header
    for (size_t len = 0; len <= 64; len++) {
        AUDIOAnalyzeFileW(writeTemp("sv8_cut.mpc", Bytes(orig.begin(), orig.begin() + len)).c_str());
        AUDIOGetDurationW();
    }
    SUCCEED("no crash");
}

TEST_CASE("AAC with an existing ID3v2 tag: writing ends up in the APE tag, ID3v2 takes precedence when reading", "[formats][tags]")
{
    // Documented behavior (AUDIOSaveChangesW: AAC -> APE tag): the existing ID3v2 tag stays unchanged,
    // the new values are in an appended APE tag and are only read when no ID3v2 tag exists.
    const fs::path p = copyToTemp("aac/adts_id3_sample-2.aac");
    const Bytes before = readFile(p);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
    AUDIOSetTitleW(L"Neuer Titel");
    REQUIRE(AUDIOSaveChangesW() != 0);

    const Bytes after = readFile(p);
    REQUIRE(after.size() > before.size());
    CHECK(std::equal(before.begin(), before.end(), after.begin()));   // ID3v2 tag and audio in front unchanged
    const std::string tail(after.begin() + before.size(), after.end());
    CHECK(tail.find("APETAGEX") != std::string::npos);                // APE-Tag angehaengt

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == AAC);
    CHECK(getField(Title) == L"Testtitel");                           // ID3v2 takes precedence
}

TEST_CASE("Formats: write tags (round trip), audio data stays unchanged", "[formats][tags][roundtrip]")
{
    const std::wstring title = L"Neuer Titel äöü €";
    for (const Fixture& fx : kFixtures) {
        if (skipWriteTests(fx.file)) continue;
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("fixture missing: " << fx.file);
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

TEST_CASE("Formats: delete tags by writing empty values", "[formats][tags][roundtrip]")
{
    for (const Fixture& fx : kFixtures) {
        if (!fx.tagged || skipWriteTests(fx.file)) continue;
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("fixture missing: " << fx.file);
            const fs::path p = copyToTemp(fx.file);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fx.format);
            AUDIOSetTitleW(L""); AUDIOSetArtistW(L""); AUDIOSetAlbumW(L"");
            REQUIRE(AUDIOSaveChangesW() != 0);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == fx.format);
            CHECK(getField(Title) == L"");
            CHECK(getField(Artist) == L"");
            CHECK(getField(Album) == L"");
            CHECK(getField(Genre) == L"Rock");   // fields that were not cleared are kept
        }
    }
}

TEST_CASE("ID3v1: text fields are returned without NUL/blank padding", "[formats][tags][id3v1]")
{
    // ID3v1 fields have a fixed length (30 bytes), usually padded with NUL bytes.
    for (const char* rel : { "mp3/id3v1_only.mp3", "ape/tagged_id3v1.ape" }) {
        DYNAMIC_SECTION(rel) {
            if (!fs::exists(fixturePath(rel))) SKIP("fixture missing: " << rel);
            REQUIRE(AUDIOAnalyzeFileW(fixturePath(rel).c_str()) != UNKNOWN);
            CHECK(ID3V1ExistsW() != 0);
            CHECK(take(ID3V1GetTitleW()) == L"Testtitel");
            CHECK(take(ID3V1GetArtistW()) == L"Testkuenstler");
            CHECK(take(ID3V1GetAlbumW()) == L"Testalbum");
            CHECK(take(ID3V1GetYearW()) == L"2024");
            CHECK(take(ID3V1GetCommentW()) == L"Kommentar");
            CHECK(take(ID3V1GetTrackW()) == L"3");
            // the same values via the abstract fields
            CHECK(getField(Title) == L"Testtitel");
            CHECK(getField(Comment) == L"Kommentar");
        }
    }
}

// ------------------------------------------------------------------ Cover

TEST_CASE("Cover: read existing pictures", "[formats][cover]")
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
    SECTION("FLAC (PICTURE block)") {
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("flac/with_cover.flac").c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 1);
        CHECK(FLACGetPictureSizeW(1) == jpgSize);
        CHECK(take(FLACGetPictureMimeW(1)) == L"image/jpeg");
    }
    SECTION("M4A (covr atom)") {
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("m4a/with_cover.m4a").c_str()) == MP4M4A);
        CHECK(MP4GetPictureCountW() == 1);
        CHECK(MP4GetPictureSizeW(1) == jpgSize);
    }
    SECTION("Files without cover") {
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("mp3/tagged.mp3").c_str()) == MPEG);
        CHECK(ID3V2GetFrameCountW(ID3F_APIC) == 0);
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("flac/tagged.flac").c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 0);
        REQUIRE(AUDIOAnalyzeFileW(fixturePath("m4a/tagged.m4a").c_str()) == MP4M4A);
        CHECK(MP4GetPictureCountW() == 0);
    }
}

TEST_CASE("Cover: add a picture, round trip, audio data unchanged", "[formats][cover][roundtrip]")
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
    SECTION("FLAC: delete picture") {
        const fs::path p = copyToTemp("flac/with_cover.flac");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        REQUIRE(FLACGetPictureCountW() == 1);
        FLACDeletePicturesW();
        REQUIRE(AUDIOSaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 0);
    }
}

// ------------------------------------------------------------ Broken files

TEST_CASE("Broken files: recognition and no crash", "[formats][broken]")
{
    struct Case { const char* file; AudioFormatID expected; };
    // Characterization of the current behavior: truncated files are still recognized as their format,
    // a 128 byte MP3 remnant is not.
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
            if (!fs::exists(fixturePath(c.file))) SKIP("fixture missing: " << c.file);
            CHECK(AUDIOAnalyzeFileW(fixturePath(c.file).c_str()) == c.expected);
            // no getter may crash on half-read data
            for (int i = 0; i < FieldCount; i++) getField(static_cast<Field>(i));
            AUDIOGetDurationW(); AUDIOGetBitrateW(); AUDIOGetChannelsW(); AUDIOGetSampleRateW();
            take(AUDIOGetMD5ValueW());
            take(AUDIOGetLastErrorTextW());
        }
    }
}

TEST_CASE("Broken files: saving tags into a truncated file does not crash", "[formats][broken]")
{
    for (const char* rel : { "broken/tagged_half.mp3", "broken/tagged_half.flac", "broken/tagged_half.ogg",
                             "broken/tagged_half.m4a", "broken/tagged_half.wma" }) {
        DYNAMIC_SECTION(rel) {
            if (!fs::exists(fixturePath(rel))) SKIP("fixture missing: " << rel);
            const fs::path p = copyToTemp(rel);
            AUDIOAnalyzeFileW(p.c_str());
            AUDIOSetTitleW(L"Reparaturversuch");
            AUDIOSaveChangesW();   // result is open (success or error), it just must not crash
            AUDIOAnalyzeFileW(p.c_str());
            SUCCEED("no crash");
        }
    }
}

// -------------------------------------------- Random damage to real files

TEST_CASE("Damaged fixtures: no crash when analyzing", "[formats][robust]")
{
    // The environment variable AG3_FUZZ_ROUNDS multiplies the iterations (e.g. 30 for a long ASan run).
    const char* env = std::getenv("AG3_FUZZ_ROUNDS");
    const int rounds = env ? std::max(1, std::atoi(env)) : 1;
    uint32_t seed = 4711;
    auto rnd = [&] { seed = seed * 1664525u + 1013904223u; return seed >> 8; };

    for (const Fixture& fx : kFixtures) {
        DYNAMIC_SECTION(fx.file) {
            if (!fs::exists(fixturePath(fx.file))) SKIP("fixture missing: " << fx.file);
            const Bytes orig = readFile(fixturePath(fx.file));
            REQUIRE(!orig.empty());
            const size_t head = std::min<size_t>(orig.size(), 4096);   // header and tags are at the start

            // 1) truncated at various positions
            for (int i = 0; i < 12 * rounds; i++) {
                const size_t len = rnd() % orig.size();
                Bytes b(orig.begin(), orig.begin() + len);
                AUDIOAnalyzeFileW(writeTemp("cut" + fs::path(fx.file).extension().string(), b).c_str());   // keep the extension (MP3/AAC are only recognized with it)
                take(AUDIOGetTitleW()); AUDIOGetDurationW();
            }
            // 2) individual bytes in the header area changed
            for (int i = 0; i < 40 * rounds; i++) {
                Bytes b = orig;
                for (int k = 0; k < 3; k++) b[rnd() % head] = static_cast<uint8_t>(rnd());
                AUDIOAnalyzeFileW(writeTemp("mut" + fs::path(fx.file).extension().string(), b).c_str());
                for (int f = 0; f < FieldCount; f++) getField(static_cast<Field>(f));
                AUDIOGetDurationW(); take(AUDIOGetMD5ValueW());
            }
            // 3) file extension does not match the content
            AUDIOAnalyzeFileW(writeTemp("falsche_endung.mp3", orig).c_str());
            AUDIOAnalyzeFileW(writeTemp("falsche_endung.flac", orig).c_str());
            SUCCEED("no crash");
        }
    }
}

// ------------------------------------------- Regressionen aus ASan-Funden (Fuzzing)

// These files triggered memory errors in the DLL that were only noticed under AddressSanitizer
// (FlacCover.cpp, WavPack.cpp); they are kept as regressions.
TEST_CASE("ASan regressions: broken files", "[formats][robust][asan-regression]")
{
    SECTION("FLAC: PICTURE block with a wrong length value (read beyond the end of the buffer, FlacCover.cpp)") {
        const auto p = fixturePath("broken/flac_cover_length_overflow.flac");
        if (!fs::exists(p)) SKIP("fixture missing");
        AUDIOAnalyzeFileW(p.c_str());
        FLACGetPictureCountW(); FLACGetPictureSizeW(1);
        SUCCEED("no crash");
    }
    SECTION("MP4: entry_count of the stco atom larger than the atom content (near-endless loop when saving, MP4_STCO.cpp)") {
        const auto src = fixturePath("broken/mp4_stco_count_overflow.m4a");
        if (!fs::exists(src)) SKIP("fixture missing");
        const fs::path p = copyToTemp("broken/mp4_stco_count_overflow.m4a");
        mustFinishWithin(30, "AUDIOSaveChangesW on an M4A with a broken stco", [&] {
            AUDIOAnalyzeFileW(p.c_str());
            AUDIOSetTitleW(L"Reparaturversuch");
            AUDIOSaveChangesW();
            AUDIOAnalyzeFileW(p.c_str());
        });
        SUCCEED("finished");
    }
    SECTION("WavPack: Sample-Rate-Index 15 (Tabellenzugriff ausserhalb, WavPack.cpp GetSampleRate)") {
        const auto p = fixturePath("broken/wavpack_samplerate_index.wv");
        if (!fs::exists(p)) SKIP("fixture missing");
        AUDIOAnalyzeFileW(p.c_str());
        AUDIOGetDurationW(); AUDIOGetSampleRateW();
        SUCCEED("no crash");
    }
}
