// ID3v2-API: Round-Trips je Frame-Typ ueber Tag-Version (2.2/2.3/2.4) und Textkodierung.
// Tests auf synthetischen MP3-Daten (support.cpp), keine Fixture-Dateien noetig.
#include "id3v2_support.h"

using namespace ag3test;
namespace fs = std::filesystem;



// =============================================================== Textframes

TEST_CASE("ID3v2: Textframe setzen/lesen ueber alle Versionen und Kodierungen", "[id3v2][text]")
{
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            const std::wstring& text = (cfg.enc == 0) ? kLatin1 : kUnicode;
            ID3V2SetTextFrameW(ID3F_TIT2, text.c_str());
            ID3V2SetTextFrameW(ID3F_TPE1, L"Interpret");
            ID3V2SetTextFrameW(ID3F_TALB, kLatin1.c_str());
            ID3V2SetTextFrameW(ID3F_TRCK, L"3/12");
            ID3V2SetTextFrameW(ID3F_TCOM, L"Komponist");
            s.reload();
            CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == text);
            CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"Interpret");
            CHECK(take(ID3V2GetTextFrameW(ID3F_TALB)) == kLatin1);
            CHECK(take(ID3V2GetTextFrameW(ID3F_TRCK)) == L"3/12");
            CHECK(take(ID3V2GetTextFrameW(ID3F_TCOM)) == L"Komponist");
            CHECK(ID3V2GetFrameCountW(ID3F_TIT2) == 1);
            CHECK(ID3V2GetEncodingW(ID3F_TIT2) == (cfg.format == 1 ? 0 : cfg.enc) );
        }
    }
}

TEST_CASE("ID3v2: Jahr (TYER in v2.3, TDRC in v2.4) und Genre", "[id3v2][text]")
{
    SECTION("v2.3: TYER") {
        Session s(kV23);
        ID3V2SetTextFrameW(ID3F_TYER, L"1999");
        ID3V2SetGenreW(L"Rock");
        s.reload();
        CHECK(take(ID3V2GetTextFrameW(ID3F_TYER)) == L"1999");
        CHECK(take(ID3V2GetGenreW()) == L"Rock");
        CHECK(take(AUDIOGetYearW()) == L"1999");
        CHECK(take(AUDIOGetGenreW()) == L"Rock");
    }
    SECTION("v2.4: TDRC") {
        Session s(kV24);
        ID3V2SetTextFrameW(ID3F_TDRC, L"1999");
        ID3V2SetGenreW(L"Rock");
        s.reload();
        CHECK(take(ID3V2GetTextFrameW(ID3F_TDRC)) == L"1999");
        CHECK(take(ID3V2GetGenreW()) == L"Rock");
        CHECK(take(AUDIOGetGenreW()) == L"Rock");
    }
    SECTION("v2.4: TYER ist dort kein gueltiges Frame und wird beim Schreiben verworfen") {
        Session s(kV24);
        ID3V2SetTextFrameW(ID3F_TYER, L"1999");
        ID3V2SetTextFrameW(ID3F_TIT2, L"Titel");
        s.reload();
        INFO("Frame-IDs: " << Catch::StringMaker<std::string>::convert([&] { auto w = take(ID3V2GetAllFrameIDsW()); return std::string(w.begin(), w.end()); }()));
        CHECK(take(ID3V2GetTextFrameW(ID3F_TYER)) == L"");
    }
}

TEST_CASE("ID3v2: leerer Text loescht das Frame", "[id3v2][text]")
{
    Session s(kV24);
    ID3V2SetTextFrameW(ID3F_TIT2, L"Titel");
    ID3V2SetTextFrameW(ID3F_TPE1, L"Interpret");
    s.reload();
    REQUIRE(ID3V2GetFrameCountW(ID3F_TIT2) == 1);
    ID3V2SetTextFrameW(ID3F_TIT2, L"");
    s.reload();
    CHECK(ID3V2GetFrameCountW(ID3F_TIT2) == 0);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"");
    CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"Interpret");
}

TEST_CASE("ID3v2: sehr langer Text und Sonderzeichen", "[id3v2][text]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            std::wstring lang(3000, L'x');
            lang += L"ä€";
            ID3V2SetTextFrameW(ID3F_TIT2, lang.c_str());
            ID3V2SetTextFrameW(ID3F_TALB, L"mit /Schraegstrich\\ und \"Anfuehrung\" & <tag>");
            s.reload();
            CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == lang);
            CHECK(take(ID3V2GetTextFrameW(ID3F_TALB)) == L"mit /Schraegstrich\\ und \"Anfuehrung\" & <tag>");
        }
    }
}

// ============================================================ URL-Frames

TEST_CASE("ID3v2: URL-Frames (WOAR) und benutzerdefinierte URL (WXXX)", "[id3v2][url]")
{
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            ID3V2SetURLFrameW(ID3F_WOAR, L"http://www.example.com/kuenstler");
            REQUIRE(ID3V2AddUserURLW(L"Beschreibung", L"http://www.example.com/x?a=1&b=2") == 0);   // 0 = neuer Eintrag
            REQUIRE(ID3V2AddUserURLW(L"Zweite", L"http://www.example.com/zwei") == 0);
            s.reload();
            CHECK(take(ID3V2GetURLFrameW(ID3F_WOAR)) == L"http://www.example.com/kuenstler");
            REQUIRE(ID3V2GetFrameCountW(ID3F_WXXX) == 2);
            CHECK(take(ID3V2GetUserURLDescriptionW(1)) == L"Beschreibung");
            CHECK(take(ID3V2GetUserURLW(1)) == L"http://www.example.com/x?a=1&b=2");
            CHECK(take(ID3V2GetUserURLDescriptionW(2)) == L"Zweite");
            CHECK(take(ID3V2GetUserURLW(2)) == L"http://www.example.com/zwei");
        }
    }
}

// ===================================================== Kommentar, Lyrics, User

TEST_CASE("ID3v2: Kommentare (COMM) mehrfach mit Sprache und Beschreibung", "[id3v2][comment]")
{
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            const std::wstring& text = (cfg.enc == 0) ? kLatin1 : kUnicode;
            CHECK(ID3V2AddCommentW(L"eng", L"", text.c_str()) == 0);
            CHECK(ID3V2AddCommentW(L"deu", L"Anmerkung", L"zweiter Kommentar") == 0);
            CHECK(ID3V2AddCommentW(L"eng", L"", L"ersetzt den ersten") == -1);          // gleiche Sprache+Beschreibung -> ersetzt
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_COMM) == 2);
            // Reihenfolge nicht festgelegt: nach Sprache zuordnen
            for (short i = 1; i <= 2; i++) {
                const std::wstring lang = take(ID3V2GetCommentLanguageW(i));
                if (lang == L"eng") {
                    CHECK(take(ID3V2GetCommentW(i)) == L"ersetzt den ersten");
                    CHECK(take(ID3V2GetCommentDescriptionW(i)) == L"");
                } else {
                    CHECK(lang == L"deu");
                    CHECK(take(ID3V2GetCommentW(i)) == L"zweiter Kommentar");
                    CHECK(take(ID3V2GetCommentDescriptionW(i)) == L"Anmerkung");
                }
            }
            // abstraktes Feld liefert den ersten COMM
            CHECK(!take(AUDIOGetCommentW()).empty());
        }
    }
}

TEST_CASE("ID3v2: Lyrics (USLT)", "[id3v2][lyrics]")
{
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            const std::wstring text = (cfg.enc == 0) ? L"Zeile 1\r\nZeile 2 äö" : L"Zeile 1\r\nZeile 2 € 日";
            CHECK(ID3V2AddLyricW(L"deu", L"Liedtext", text.c_str()) == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_USLT) == 1);
            CHECK(take(ID3V2GetLyricLanguageW(1)) == L"deu");
            CHECK(take(ID3V2GetLyricDescriptionW(1)) == L"Liedtext");
            CHECK(take(ID3V2GetLyricW(1)) == text);
        }
    }
}

TEST_CASE("ID3v2: benutzerdefinierter Text (TXXX)", "[id3v2][text]")
{
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            const std::wstring& text = (cfg.enc == 0) ? kLatin1 : kUnicode;
            CHECK(ID3V2AddUserTextW(L"MOOD", text.c_str()) == 0);
            CHECK(ID3V2AddUserTextW(L"ORIGIN", L"anderes Feld") == 0);
            CHECK(ID3V2AddUserTextW(L"MOOD", L"ueberschrieben") == -1);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_TXXX) == 2);
            bool mood = false, origin = false;
            for (short i = 1; i <= 2; i++) {
                const std::wstring d = take(ID3V2GetUserTextDescriptionW(i));
                if (d == L"MOOD") { mood = true; CHECK(take(ID3V2GetUserTextW(i)) == L"ueberschrieben"); }
                if (d == L"ORIGIN") { origin = true; CHECK(take(ID3V2GetUserTextW(i)) == L"anderes Feld"); }
            }
            CHECK(mood);
            CHECK(origin);
        }
    }
}

TEST_CASE("ID3v2: Nutzungsbedingungen (USER)", "[id3v2][text]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            CHECK(ID3V2AddUserFrameW(L"eng", L"Nur fuer den privaten Gebrauch") == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_USER) == 1);
            CHECK(take(ID3V2GetUserFrameLanguageW(1)) == L"eng");
            CHECK(take(ID3V2GetUserFrameW(1)) == L"Nur fuer den privaten Gebrauch");
        }
    }
}

TEST_CASE("ID3v2: Play Counter (PCNT) und Popularimeter (POPM)", "[id3v2][counter]")
{
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            ID3V2SetPlayCounterW(123456);
            CHECK(ID3V2AddPopularimeterW(L"user@example.com", 200, 42) == 0);
            CHECK(ID3V2AddPopularimeterW(L"other@example.com", 5, 70000) == 0);
            s.reload();
            CHECK(ID3V2GetPlayCounterW() == 123456);
            REQUIRE(ID3V2GetFrameCountW(ID3F_POPM) == 2);
            for (short i = 1; i <= 2; i++) {
                const std::wstring mail = take(ID3V2GetPopularimeterEmailW(i));
                if (mail == L"user@example.com") { CHECK(ID3V2GetPopularimeterRatingW(i) == 200); CHECK(ID3V2GetPopularimeterCounterW(i) == 42); }
                else { CHECK(mail == L"other@example.com"); CHECK(ID3V2GetPopularimeterRatingW(i) == 5); CHECK(ID3V2GetPopularimeterCounterW(i) == 70000); }
            }
        }
    }
}

// ============================================================ Frames loeschen

TEST_CASE("ID3v2: Frames einzeln und alle loeschen, Tag entfernen", "[id3v2][delete]")
{
    Session s(kV24);
    ID3V2AddCommentW(L"eng", L"a", L"eins");
    ID3V2AddCommentW(L"eng", L"b", L"zwei");
    ID3V2AddCommentW(L"eng", L"c", L"drei");
    ID3V2SetTextFrameW(ID3F_TIT2, L"Titel");
    s.reload();
    REQUIRE(ID3V2GetFrameCountW(ID3F_COMM) == 3);

    CHECK(ID3V2DeleteSelectedFrameW(ID3F_COMM, 2) != 0);
    CHECK(ID3V2GetFrameCountW(ID3F_COMM) == 2);
    CHECK(ID3V2DeleteSelectedFrameW(ID3F_COMM, 9) == 0);     // Index ausserhalb: nicht gefunden
    s.reload();
    CHECK(ID3V2GetFrameCountW(ID3F_COMM) == 2);

    CHECK(ID3V2DeleteAllFramesW(ID3F_COMM) != 0);
    s.reload();
    CHECK(ID3V2GetFrameCountW(ID3F_COMM) == 0);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Titel");

    const std::wstring ids = take(ID3V2GetAllFrameIDsW());
    CHECK(ids.find(L"TIT2") != std::wstring::npos);

    CHECK(ID3V2RemoveTagW() != 0);
    CHECK(AUDIOAnalyzeFileW(s.path.c_str()) == MPEG);
    CHECK(ID3V2ExistsW() == 0);
}

TEST_CASE("ID3v2: Tag-Version und Groesse", "[id3v2][version]")
{
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            ID3V2SetTextFrameW(ID3F_TIT2, L"Titel");
            s.reload();
            CHECK(ID3V2ExistsW() != 0);
            const std::wstring ver = take(ID3V2GetVersionW());
            const wchar_t* expect = cfg.format == 1 ? L"2.2" : cfg.format == 2 ? L"2.3" : L"2.4";
            INFO("Version laut DLL: " << Catch::StringMaker<std::string>::convert(std::string(ver.begin(), ver.end())));
            CHECK(ver.find(expect) != std::wstring::npos);
            CHECK(ID3V2GetSizeW() > 0);
            CHECK(static_cast<size_t>(ID3V2GetSizeW()) <= readFile(s.path).size());
        }
    }
}

TEST_CASE("ID3v2: Audiodaten bleiben bei jedem Format/jeder Kodierung unveraendert", "[id3v2][roundtrip]")
{
    const Bytes audio = makeMp3(40);
    for (const Cfg& cfg : kAllCfgs) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            ID3V2SetTextFrameW(ID3F_TIT2, L"Titel");
            ID3V2AddCommentW(L"eng", L"", L"Kommentar");
            s.reload();
            const Bytes after = readFile(s.path);
            const size_t tag = id3v2TotalSize(after);
            REQUIRE(after.size() >= tag + audio.size());
            CHECK(std::equal(audio.begin(), audio.end(), after.begin() + tag));
        }
    }
}
