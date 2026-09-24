// ID3v2-API, Teil 2: Binaer-, Zahlen- und Strukturframes (Round-Trips).
#include "id3v2_support.h"

using namespace ag3test;

namespace {
Bytes fetch2(long (__stdcall *fn)(BYTE*, u32))
{
    Bytes out(4096);
    const long n = fn(out.data(), static_cast<u32>(out.size()));
    out.resize(n > 0 ? static_cast<size_t>(n) : 0);
    return out;
}
}  // namespace

// ============================================================ Binaerframes

TEST_CASE("ID3v2: Private Frames (PRIV)", "[id3v2][binary]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob d1(40), d2(5, 200);
            CHECK(ID3V2AddPrivateFrameW(d1.ptr(), d1.len(), L"owner1@example.com") == 0);
            CHECK(ID3V2AddPrivateFrameW(d2.ptr(), d2.len(), L"owner2@example.com") == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_PRIV) == 2);
            for (short i = 1; i <= 2; i++) {
                const std::wstring url = take(ID3V2GetPrivateFrameURLW(i));
                const Bytes data = fetch(ID3V2GetPrivateFrameDataW, i);
                if (url == L"owner1@example.com") CHECK(data == d1.data); else { CHECK(url == L"owner2@example.com"); CHECK(data == d2.data); }
            }
        }
    }
}

TEST_CASE("ID3v2: Eindeutige Datei-ID (UFID)", "[id3v2][binary]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob d(24);
            CHECK(ID3V2AddUniqueFileIdentifierW(d.ptr(), d.len(), L"http://musicbrainz.org") == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_UFID) == 1);
            CHECK(take(ID3V2GetUniqueFileIdentifierOwnerW(1)) == L"http://musicbrainz.org");
            CHECK(fetch(ID3V2GetUniqueFileIdentifierDataW, 1) == d.data);
        }
    }
}

TEST_CASE("ID3v2: Allgemeines Objekt (GEOB)", "[id3v2][binary]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob d(300);
            CHECK(ID3V2AddGeneralObjectW(d.ptr(), d.len(), L"application/octet-stream", L"datei.bin", L"Beschreibung") == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_GEOB) == 1);
            CHECK(take(ID3V2GetGeneralObjectMimeW(1)) == L"application/octet-stream");
            CHECK(take(ID3V2GetGeneralObjectFilenameW(1)) == L"datei.bin");
            CHECK(take(ID3V2GetGeneralObjectDescriptionW(1)) == L"Beschreibung");
            CHECK(fetch(ID3V2GetGeneralObjectDataW, 1) == d.data);
        }
    }
}

TEST_CASE("ID3v2: Gruppen-ID (GRID), Verschluesselung (ENCR), Signatur (SIGN, nur v2.4)", "[id3v2][binary]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob g(10), e(16), sg(32);
            CHECK(ID3V2AddGroupIdentificationW(g.ptr(), g.len(), L"http://example.com/grp", 0x81) == 0);
            CHECK(ID3V2AddEncryptionW(e.ptr(), e.len(), L"http://example.com/enc", 0x82) == 0);
            CHECK(ID3V2AddSignatureFrameW(sg.ptr(), sg.len(), 0x83) == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_GRID) == 1);
            CHECK(take(ID3V2GetGroupIdentificationURLW(1)) == L"http://example.com/grp");
            CHECK(ID3V2GetGroupIdentificationSymbolW(1) == 0x81);
            CHECK(fetch(ID3V2GetGroupIdentificationDataW, 1) == g.data);
            REQUIRE(ID3V2GetFrameCountW(ID3F_ENCR) == 1);
            CHECK(take(ID3V2GetEncryptionURLW(1)) == L"http://example.com/enc");
            CHECK(ID3V2GetEncryptionSymbolW(1) == 0x82);
            CHECK(fetch(ID3V2GetEncryptionDataW, 1) == e.data);
            if (cfg.format == 3) {
                REQUIRE(ID3V2GetFrameCountW(ID3F_SIGN) == 1);
                CHECK(ID3V2GetSignatureFrameGroupSymbolW(1) == 0x83);
                CHECK(fetch(ID3V2GetSignatureFrameDataW, 1) == sg.data);
            } else {
                CHECK(ID3V2GetFrameCountW(ID3F_SIGN) == 0);   // SIGN gibt es erst ab v2.4 und wird in v2.3 verworfen
            }
        }
    }
}

TEST_CASE("ID3v2: Audio-Verschluesselung (AENC) und Musik-CD-ID (MCDI)", "[id3v2][binary]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob a(20), cd(60);
            CHECK(ID3V2AddAudioEncryptionW(a.ptr(), a.len(), L"http://example.com/aenc", 100, 2000) == 0);
            CHECK(ID3V2AddMusicCdIdentifierW(cd.ptr(), cd.len()) == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_AENC) == 1);
            CHECK(take(ID3V2GetAudioEncryptionURLW(1)) == L"http://example.com/aenc");
            CHECK(ID3V2GetAudioEncryptionPreviewStartW(1) == 100);
            CHECK(ID3V2GetAudioEncryptionPreviewLengthW(1) == 2000);
            CHECK(fetch(ID3V2GetAudioEncryptionDataW, 1) == a.data);
            REQUIRE(ID3V2GetFrameCountW(ID3F_MCDI) == 1);
            CHECK(fetch2(ID3V2GetMusicCdIdentifierDataW) == cd.data);
        }
    }
}

TEST_CASE("ID3v2: Ereigniszeiten (ETCO) und Tempo (SYTC)", "[id3v2][binary]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob ev(30), tempo(12);
            CHECK(ID3V2AddEventTimingCodesW(ev.ptr(), ev.len(), 2) == 0);
            CHECK(ID3V2AddSynchronizedTempoW(tempo.ptr(), tempo.len(), 2) == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_ETCO) == 1);
            CHECK(ID3V2GetEventTimingCodesTimestampFormatW() == 2);
            CHECK(fetch2(ID3V2GetEventTimingCodesDataW) == ev.data);
            REQUIRE(ID3V2GetFrameCountW(ID3F_SYTC) == 1);
            CHECK(ID3V2GetSynchronizedTempoFormatW() == 2);
            CHECK(fetch2(ID3V2GetSynchronizedTempoDataW) == tempo.data);
        }
    }
}

TEST_CASE("ID3v2: Lautstaerkeanpassung (RVAD) und Entzerrung (EQUA) in v2.3", "[id3v2][binary]")
{
    // Die API erzeugt die v2.3-Frames RVAD und EQUA (siehe Doku von ID3V2AddRelativeVolumeAdjustmentW/AddEqualisationW).
    Session s(kV23);
    Blob rv(18), eq(24);
    CHECK(ID3V2AddRelativeVolumeAdjustmentW(rv.ptr(), rv.len(), L"track") == 0);
    CHECK(ID3V2AddEqualisationW(eq.ptr(), eq.len(), 1, L"eqid") == 0);
    s.reload();
    // Bezeichner und Interpolationsmethode gibt es erst in RVA2/EQU2 (v2.4); RVAD/EQUA speichern nur die Daten.
    REQUIRE(ID3V2GetFrameCountW(ID3F_RVAD) == 1);
    CHECK(fetch(ID3V2GetRelativeVolumeAdjustmentDataW, 1) == rv.data);
    REQUIRE(ID3V2GetFrameCountW(ID3F_EQUA) == 1);
    CHECK(fetch(ID3V2GetEqualisationDataW, 1) == eq.data);
}

TEST_CASE("ID3v2: Lautstaerkeanpassung und Entzerrung in v2.4 (RVA2/EQU2 mit Bezeichner)", "[id3v2][binary]")
{
    Session s(kV24);
    Blob rv(18), eq(24);
    CHECK(ID3V2AddRelativeVolumeAdjustmentW(rv.ptr(), rv.len(), L"track") == 0);
    CHECK(ID3V2AddEqualisationW(eq.ptr(), eq.len(), 1, L"eqid") == 0);
    s.reload();
    const std::wstring wids = take(ID3V2GetAllFrameIDsW());
    const std::string ids(wids.begin(), wids.end());
    INFO("Frame-IDs nach dem Speichern: " << ids);
    const int volume = ID3V2GetFrameCountW(ID3F_RVAD) + ID3V2GetFrameCountW(ID3F_RVA2);
    const int equal = ID3V2GetFrameCountW(ID3F_EQUA) + ID3V2GetFrameCountW(ID3F_EQU2);
    CHECK(volume == 1);
    CHECK(equal == 1);
    // In v2.4 werden die Frames als RVA2/EQU2 geschrieben und tragen Bezeichner und Interpolationsmethode.
    CHECK(take(ID3V2GetRelativeVolumeAdjustmentIdentifierW(1)) == L"track");
    CHECK(fetch(ID3V2GetRelativeVolumeAdjustmentDataW, 1) == rv.data);
    CHECK(take(ID3V2GetEqualisationIdentificationW(1)) == L"eqid");
    CHECK(ID3V2GetEqualisationInterpolationW(1) == 1);
    CHECK(fetch(ID3V2GetEqualisationDataW, 1) == eq.data);
}

// ============================================================ Zahlen-Frames

TEST_CASE("ID3v2: Puffergroesse (RBUF), Positionssynchronisation (POSS), Nachhall (RVRB)", "[id3v2][numeric]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            CHECK(ID3V2AddRecommendedBufferSizeW(65536, 1, 4096) == 0);
            CHECK(ID3V2AddPositionSynchronisationW(2, 123456) == 0);
            CHECK(ID3V2AddReverbW(100, 200, 3, 4, 10, 20, 30, 40, 50, 60) == 0);
            s.reload();
            CHECK(ID3V2GetRecommendedBufferSizeValueW() == 65536);
            CHECK(ID3V2GetRecommendedBufferSizeFlagW() == 1);
            CHECK(ID3V2GetRecommendedBufferSizeOffsetW() == 4096);
            CHECK(ID3V2GetPositionSynchronisationTimestampFormatW() == 2);
            CHECK(ID3V2GetPositionSynchronisationValueW() == 123456);
            CHECK(ID3V2GetReverbLeftW() == 100);
            CHECK(ID3V2GetReverbRightW() == 200);
            CHECK(ID3V2GetReverbBouncesLeftW() == 3);
            CHECK(ID3V2GetReverbBouncesRightW() == 4);
            CHECK(ID3V2GetReverbFeedbackLeftToLeftW() == 10);
            CHECK(ID3V2GetReverbFeedbackLeftToRightW() == 20);
            CHECK(ID3V2GetReverbFeedbackRightToRightW() == 30);
            CHECK(ID3V2GetReverbFeedbackRightToLeftW() == 40);
            CHECK(ID3V2GetReverbPremixLeftToRightW() == 50);
            CHECK(ID3V2GetReverbPremixRightToLeftW() == 60);
        }
    }
}

TEST_CASE("ID3v2: MPEG-Positionstabelle (MLLT)", "[id3v2][numeric]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob t(24);
            CHECK(ID3V2AddMpegLocationLookupTableW(t.ptr(), t.len(), 1000, 20000, 500, 8, 8) == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_MLLT) == 1);
            CHECK(ID3V2GetMpegLocationLookupTableFramesW() == 1000);
            CHECK(ID3V2GetMpegLocationLookupTableBytesW() == 20000);
            CHECK(ID3V2GetMpegLocationLookupTableMillisecondsW() == 500);
            CHECK(ID3V2GetMpegLocationLookupTableBytesDeviationW() == 8);
            CHECK(ID3V2GetMpegLocationLookupTableMillisecondsDeviationW() == 8);
            CHECK(fetch2(ID3V2GetMpegLocationLookupTableDataW) == t.data);
        }
    }
}

TEST_CASE("ID3v2: Seek-Offset (SEEK) und Audio-Suchpunkte (ASPI), nur v2.4", "[id3v2][numeric]")
{
    Session s(kV24);
    Blob pts(16);
    CHECK(ID3V2AddSeekOffsetW(2048) == 0);
    CHECK(ID3V2AddAudioSeekPointW(pts.ptr(), pts.len(), 100, 5000, 8, 16) == 0);
    s.reload();
    CHECK(ID3V2GetSeekOffsetW() == 2048);
    REQUIRE(ID3V2GetFrameCountW(ID3F_ASPI) == 1);
    CHECK(ID3V2GetAudioSeekPointStartW() == 100);
    CHECK(ID3V2GetAudioSeekPointLengthW() == 5000);
    CHECK(ID3V2GetAudioSeekPointNumberW() == 8);
    CHECK(ID3V2GetAudioSeekPointBitsPerIndexpointW() == 16);
    CHECK(fetch2(ID3V2GetAudioSeekPointDataW) == pts.data);
}

// ====================================== Eigentum, Kommerz, Verknuepfung

TEST_CASE("ID3v2: Eigentum (OWNE), Kommerz (COMR), Verknuepfung (LINK)", "[id3v2][text]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            Blob logo(50);
            CHECK(ID3V2AddOwnershipW(L"EUR9.99", L"20240131", L"Verkaeufer") == 0);
            CHECK(ID3V2AddLinkedInformationW(ID3F_TIT2, L"http://example.com/frame", L"extra") == 0);
            CHECK(ID3V2AddCommercialFrameW(logo.ptr(), logo.len(), L"EUR1.00", L"20301231", L"http://example.com/kontakt", 1, L"Anbieter", L"Produktbeschreibung") == 0);
            s.reload();
            CHECK(take(ID3V2GetOwnershipPriceW()) == L"EUR9.99");
            CHECK(take(ID3V2GetOwnershipDateW()) == L"20240131");
            CHECK(take(ID3V2GetOwnershipSellerW()) == L"Verkaeufer");
            REQUIRE(ID3V2GetFrameCountW(ID3F_LINK) == 1);
            CHECK(ID3V2GetLinkedInformationFrameIdentifierW(1) == static_cast<long>(ID3F_TIT2));
            CHECK(take(ID3V2GetLinkedInformationURLW(1)) == L"http://example.com/frame");
            CHECK(take(ID3V2GetLinkedInformationAdditionalDataW(1)) == L"extra");
            REQUIRE(ID3V2GetFrameCountW(ID3F_COMR) == 1);
            CHECK(take(ID3V2GetCommercialFramePriceW(1)) == L"EUR1.00");
            CHECK(take(ID3V2GetCommercialFrameValidUntilW(1)) == L"20301231");
            CHECK(take(ID3V2GetCommercialFrameContactURLW(1)) == L"http://example.com/kontakt");
            CHECK(ID3V2GetCommercialFrameReceivedAsW(1) == 1);
            CHECK(take(ID3V2GetCommercialFrameSellerNameW(1)) == L"Anbieter");
            CHECK(take(ID3V2GetCommercialFrameDescriptionW(1)) == L"Produktbeschreibung");
            CHECK(fetch(ID3V2GetCommercialFramePictureW, 1) == logo.data);
        }
    }
}

// ================================================== Synchronisierte Lyrics

TEST_CASE("ID3v2: Synchronisierte Lyrics (SYLT)", "[id3v2][lyrics]")
{
    for (const Cfg& cfg : { kV23, kV24 }) {
        DYNAMIC_SECTION(cfg.name) {
            Session s(cfg);
            CHECK(ID3V2AddSyncLyricW(L"deu", L"Karaoke", L"[00000010]Zeile eins\r\n[00000050]Zeile zwei", 1, 2) == 0);
            s.reload();
            REQUIRE(ID3V2GetFrameCountW(ID3F_SYLT) == 1);
            CHECK(take(ID3V2GetSyncLyricLanguageW(1)) == L"deu");
            CHECK(take(ID3V2GetSyncLyricDescriptionW(1)) == L"Karaoke");
            CHECK(ID3V2GetSyncLyricContentTypeW(1) == 1);
            CHECK(ID3V2GetSyncLyricTimeFormatW(1) == 2);
            // Eintraege sind mit CRLF getrennt, nach dem letzten Eintrag folgt ein einzelnes LF (so liefert es die DLL)
            CHECK(take(ID3V2GetSyncLyricW(1)) == L"[00000010]Zeile eins\r\n[00000050]Zeile zwei\n");
        }
    }
}

// ==================================================== Kapitel und Inhaltsverzeichnis

TEST_CASE("ID3v2: Kapitel (CHAP) und Inhaltsverzeichnis (CTOC)", "[id3v2][chapters]")
{
    Session s(kV24);
    CHECK(ID3V2AddTableOfContentW(L"toc1", L"Inhalt", L"Beschreibung des Inhalts", 1) == 0);
    CHECK(ID3V2AddChapterW(L"chp1", L"Kapitel 1", L"Einleitung", 0, 60000) == 0);
    CHECK(ID3V2AddChapterW(L"chp2", L"Kapitel 2", L"Hauptteil", 60000, 180000) == 0);
    CHECK(ID3V2AddChildElementW(L"toc1", L"chp1") != 0);
    CHECK(ID3V2AddChildElementW(L"toc1", L"chp2") != 0);
    s.reload();

    REQUIRE(ID3V2GetFrameCountW(ID3F_CHAP) == 2);
    REQUIRE(ID3V2GetFrameCountW(ID3F_CTOC) == 1);
    CHECK(ID3V2GetChapterStartTimeW(L"chp2") == 60000);
    CHECK(ID3V2GetChapterEndTimeW(L"chp2") == 180000);
    CHECK(take(ID3V2GetAddendumTitleW(L"chp1")) == L"Kapitel 1");
    CHECK(take(ID3V2GetAddendumDescriptionW(L"chp2")) == L"Hauptteil");
    CHECK(take(ID3V2GetAddendumTitleW(L"toc1")) == L"Inhalt");
    CHECK(ID3V2GetTOCIsOrderedW(L"toc1") != 0);
    CHECK(take(ID3V2GetTOCRootIDW()) == L"toc1");
    REQUIRE(ID3V2GetChildElementsW(L"toc1") == 2);
    CHECK(take(ID3V2GetChildElementIDW(L"toc1", 1)) == L"chp1");
    CHECK(take(ID3V2GetChildElementIDW(L"toc1", 2)) == L"chp2");

    // aendern
    CHECK(ID3V2SetChapterTimesW(L"chp1", 1000, 61000) != 0);
    CHECK(ID3V2SetAddendumTitleW(L"chp1", L"Neuer Titel") != 0);
    CHECK(ID3V2SetTOCIsOrderedW(L"toc1", 0) != 0);
    s.reload();
    CHECK(ID3V2GetChapterStartTimeW(L"chp1") == 1000);
    CHECK(ID3V2GetChapterEndTimeW(L"chp1") == 61000);
    CHECK(take(ID3V2GetAddendumTitleW(L"chp1")) == L"Neuer Titel");
    CHECK(ID3V2GetTOCIsOrderedW(L"toc1") == 0);

    // loeschen
    CHECK(ID3V2DeleteChildElementW(L"toc1", L"chp2") != 0);
    CHECK(ID3V2DeleteAddendumW(L"chp2") != 0);
    s.reload();
    CHECK(ID3V2GetFrameCountW(ID3F_CHAP) == 1);
    CHECK(ID3V2GetChildElementsW(L"toc1") == 1);
}

TEST_CASE("ID3v2: Unterframes in Kapiteln (Text, URL, Bild)", "[id3v2][chapters]")
{
    Session s(kV24);
    Blob img(80);
    CHECK(ID3V2AddChapterW(L"chp1", L"Kapitel", L"", 0, 1000) == 0);
    CHECK(ID3V2SetTextSubFrameW(L"chp1", ID3F_TALB, L"Sub-Album", L"") != 0);
    CHECK(ID3V2SetURLSubFrameW(L"chp1", ID3F_WXXX, L"http://example.com/sub", L"Link") != 0);
    CHECK(ID3V2SetImageSubFrameW(img.ptr(), img.len(), L"chp1", L"Sub-Bild", 3) != 0);
    s.reload();
    const short n = ID3V2GetSubFramesW(L"chp1");
    CHECK(n >= 3);
    bool text = false, url = false, image = false;
    for (short i = 1; i <= n; i++) {
        const u32 id = ID3V2GetSubFrameIDW(L"chp1", i);
        if (id == ID3F_TALB) { text = true; CHECK(take(ID3V2GetSubFrameTextW(L"chp1", i)) == L"Sub-Album"); }
        if (id == ID3F_WXXX) { url = true; CHECK(take(ID3V2GetSubFrameTextW(L"chp1", i)) == L"http://example.com/sub"); CHECK(take(ID3V2GetSubFrameDescriptionW(L"chp1", i)) == L"Link"); }
        if (id == ID3F_APIC) {
            image = true;
            Bytes out(4096);
            const long len = ID3V2GetSubFrameImageW(out.data(), static_cast<u32>(out.size()), L"chp1", i);
            out.resize(len > 0 ? static_cast<size_t>(len) : 0);
            CHECK(out == img.data);
            CHECK(ID3V2GetSubFrameImageTypeW(L"chp1", i) == 3);
        }
    }
    CHECK(text);
    CHECK(url);
    CHECK(image);
    CHECK(ID3V2DeleteSubFrameW(L"chp1", ID3F_TALB) != 0);
    s.reload();
    bool stillText = false;
    for (short i = 1; i <= ID3V2GetSubFramesW(L"chp1"); i++) if (ID3V2GetSubFrameIDW(L"chp1", i) == ID3F_TALB) stillText = true;
    CHECK_FALSE(stillText);
}
