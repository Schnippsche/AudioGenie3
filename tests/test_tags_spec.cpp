// ID3v1 / ID3v1.1 and APE tag v1 / v2 checks with hand made tags.
#include "id3v2_support.h"
#include <string>

using namespace ag3test;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
Bytes bytesOf(const char* s) { Bytes b; put(b, s); return b; }
Bytes le32(uint32_t v) { return { static_cast<uint8_t>(v), static_cast<uint8_t>(v >> 8), static_cast<uint8_t>(v >> 16), static_cast<uint8_t>(v >> 24) }; }

Bytes audio() { return makeMp3(30); }

size_t findBytes(const Bytes& hay, const Bytes& needle)
{
    auto it = std::search(hay.begin(), hay.end(), needle.begin(), needle.end());
    return it == hay.end() ? static_cast<size_t>(-1) : static_cast<size_t>(it - hay.begin());
}

// 128 bytes ID3v1: fields are padded with $00
Bytes id3v1(const std::string& title, const std::string& artist, const std::string& album, const std::string& year,
            const Bytes& commentAndTrack /* exactly 30 bytes */, uint8_t genre)
{
    Bytes t = bytesOf("TAG");
    auto field = [&t](const std::string& s, size_t n) { for (size_t i = 0; i < n; i++) t.push_back(i < s.size() ? static_cast<uint8_t>(s[i]) : 0); };
    field(title, 30); field(artist, 30); field(album, 30); field(year, 4);
    REQUIRE(commentAndTrack.size() == 30);
    put(t, commentAndTrack);
    t.push_back(genre);
    REQUIRE(t.size() == 128);
    return t;
}

Bytes pad30(const std::string& s)
{
    Bytes b;
    for (size_t i = 0; i < 30; i++) b.push_back(i < s.size() ? static_cast<uint8_t>(s[i]) : 0);
    return b;
}

fs::path writeWithTail(const char* name, const Bytes& tail)
{
    Bytes b = audio();
    put(b, tail);
    return writeTemp(name, b);
}

// APE item: value size, flags, key, $00, value
Bytes apeItem(const std::string& key, const Bytes& value, uint32_t flags = 0)
{
    Bytes i = le32(static_cast<uint32_t>(value.size()));
    put(i, le32(flags));
    put(i, key.c_str());
    i.push_back(0);
    put(i, value);
    return i;
}

// APE tag: [header] items footer; version 1000 has no header and no flags
Bytes apeTag(uint32_t version, const std::vector<Bytes>& items, bool header = true)
{
    Bytes body;
    for (const Bytes& i : items) put(body, i);
    const uint32_t size = static_cast<uint32_t>(body.size()) + 32;
    auto block = [&](uint32_t flags) {
        Bytes h = bytesOf("APETAGEX");
        put(h, le32(version)); put(h, le32(size)); put(h, le32(static_cast<uint32_t>(items.size()))); put(h, le32(flags));
        h.insert(h.end(), 8, 0);
        return h;
    };
    Bytes t;
    const bool v2 = version >= 2000;
    if (v2 && header) put(t, block(0xA0000000u));
    put(t, body);
    put(t, block(v2 ? (header ? 0x80000000u : 0x40000000u) : 0u));
    return t;
}

Bytes text(const char* s) { return bytesOf(s); }

// index of the first difference between the start of f and the audio data, -1 if the audio data are intact (prefix)
long audioDiff(const Bytes& f)
{
    const Bytes a = audio();
    if (f.size() < a.size()) return static_cast<long>(f.size());
    for (size_t i = 0; i < a.size(); i++) if (f[i] != a[i]) return static_cast<long>(i);
    return -1;
}

}  // namespace

TEST_CASE("ID3v1: the comment of a v1.0 tag is not taken for a track number", "[tags][spec][id3v1]")
{
    // 30 characters of comment; the 29th is a blank and the 30th a letter: no v1.1 tag, since byte 29 is not $00
    const std::string comment = "0123456789012345678901234567 x";
    REQUIRE(comment.size() == 30);
    Bytes cm(comment.begin(), comment.end());
    auto p = writeWithTail("v1_comment30.mp3", id3v1("T", "A", "B", "2001", cm, 17));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V1GetCommentW()) == L"0123456789012345678901234567 x");
    CHECK(take(ID3V1GetTrackW()).empty());
    CHECK(take(ID3V1GetVersionW()) == L"1.0");
}

TEST_CASE("ID3v1.1: layout of the tag", "[tags][spec][id3v1]")
{
    Bytes cm = pad30("a comment that is longer than 28 chars");
    cm[28] = 0; cm[29] = 7;
    auto p = writeWithTail("v11_read.mp3", id3v1("Title", "Artist", "Album", "1999", cm, 9));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V1GetTrackW()) == L"7");
    CHECK(take(ID3V1GetVersionW()) == L"1.1");
    CHECK(take(ID3V1GetGenreW()) == L"Metal");

    SECTION("write") {
        ID3V1SetTrackW(L"12");
        ID3V1SetCommentW(L"0123456789012345678901234567890123");
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        REQUIRE(f.size() == audio().size() + 128);
        const Bytes tag(f.end() - 128, f.end());
        CHECK(Bytes(tag.begin(), tag.begin() + 3) == bytesOf("TAG"));
        CHECK(Bytes(tag.begin() + 97, tag.begin() + 125) == bytesOf("0123456789012345678901234567"));   // comment cut to 28
        CHECK(tag[125] == 0);
        CHECK(tag[126] == 12);
        CHECK(tag[127] == 9);
        CHECK(audioDiff(f) == -1);
    }
    SECTION("v1.0 when there is no track") {
        ID3V1SetTrackW(L"");
        ID3V1SetCommentW(L"0123456789012345678901234567890123");
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        CHECK(Bytes(f.end() - 31, f.end() - 1) == bytesOf("012345678901234567890123456789"));
    }
}

TEST_CASE("ID3v1: the track number is one byte", "[tags][spec][id3v1]")
{
    auto p = writeWithTail("v11_track.mp3", id3v1("T", "A", "B", "2001", pad30("c"), 255));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    ID3V1SetTrackW(L"255");
    CHECK(take(ID3V1GetTrackW()) == L"255");
    ID3V1SetTrackW(L"256");
    CHECK(take(ID3V1GetTrackW()).empty());   // does not fit: no track, and not 0 by wrapping around
    ID3V1SetTrackW(L"300");
    CHECK(take(ID3V1GetTrackW()).empty());
}

TEST_CASE("ID3v1: genre list and the value 255", "[tags][spec][id3v1]")
{
    CHECK(ID3V1GetGenresW() == 148);
    CHECK(take(ID3V1GetGenreItemW(0)) == L"Blues");
    CHECK(take(ID3V1GetGenreItemW(60)) == L"Top 40");
    CHECK(take(ID3V1GetGenreItemW(62)) == L"Pop/Funk");
    CHECK(take(ID3V1GetGenreItemW(79)) == L"Hard Rock");
    CHECK(take(ID3V1GetGenreItemW(80)) == L"Folk");
    CHECK(take(ID3V1GetGenreItemW(125)) == L"Dance Hall");
    CHECK(take(ID3V1GetGenreItemW(144)) == L"Thrash Metal");
    CHECK(take(ID3V1GetGenreItemW(147)) == L"Synthpop");
    auto p = writeWithTail("v1_genre.mp3", id3v1("T", "A", "B", "2001", pad30("c"), 200));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V1GetGenreW()).empty());
    CHECK(ID3V1GetGenreIDW() == 200);
    SECTION("names of the previous versions are still accepted") {
        ID3V1SetGenreW(L"Trash Metal");
        CHECK(ID3V1GetGenreIDW() == 144);
        ID3V1SetGenreW(L"Pop & Funk");
        CHECK(ID3V1GetGenreIDW() == 62);
        ID3V1SetGenreW(L"Top");
        CHECK(ID3V1GetGenreIDW() == 60);
    }
    SECTION("unknown name: 255") {
        ID3V1SetGenreW(L"No such genre");
        CHECK(ID3V1GetGenreIDW() == 255);
        REQUIRE(ID3V1SaveChangesW() != 0);
        CHECK(readFile(p).back() == 255);
    }
}

TEST_CASE("APE v2: layout of a written tag", "[tags][spec][ape]")
{
    auto p = writeTemp("ape_layout.mp3", audio());
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    APESetTitleW(L"Titel");
    APESetUserItemW(L"Custom Key", L"v");
    REQUIRE(APESaveChangesW() != 0);
    const Bytes f = readFile(p);
    const Bytes a = audio();
    REQUIRE(f.size() > a.size() + 64);
    CHECK(audioDiff(f) == -1);
    // header directly after the audio data
    const size_t h = a.size();
    CHECK(Bytes(f.begin() + h, f.begin() + h + 8) == bytesOf("APETAGEX"));
    auto rd = [&](size_t pos) { return static_cast<uint32_t>(f[pos]) | (static_cast<uint32_t>(f[pos + 1]) << 8) | (static_cast<uint32_t>(f[pos + 2]) << 16) | (static_cast<uint32_t>(f[pos + 3]) << 24); };
    CHECK(rd(h + 8) == 2000);
    const uint32_t size = rd(h + 12);
    CHECK(rd(h + 16) == 2);
    CHECK(rd(h + 20) == 0xA0000000u);   // has header, has footer, this is the header
    CHECK(size == f.size() - h - 32);   // size = items + footer, without the header
    const size_t foot = f.size() - 32;
    CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(foot), f.begin() + static_cast<std::ptrdiff_t>(foot) + 8) == bytesOf("APETAGEX"));
    CHECK(rd(foot + 8) == 2000);
    CHECK(rd(foot + 12) == size);
    CHECK(rd(foot + 16) == 2);
    CHECK(rd(foot + 20) == 0x80000000u);   // has header, has footer, this is the footer
    for (size_t i = 24; i < 32; i++) { CHECK(f[h + i] == 0); CHECK(f[foot + i] == 0); }
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetTitleW()) == L"Titel");
    CHECK(take(APEGetUserItemW(L"Custom Key")) == L"v");
}

TEST_CASE("APE v2: an ID3v1 tag behind the APE tag keeps its place and the audio data stay intact", "[tags][spec][ape]")
{
    const Bytes v1 = id3v1("T", "A", "B", "2001", pad30("c"), 17);
    Bytes tail = apeTag(2000, { apeItem("Title", text("old")) });
    put(tail, v1);
    auto p = writeWithTail("ape_with_v1.mp3", tail);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetTitleW()) == L"old");
    CHECK(take(ID3V1GetTitleW()) == L"T");
    APESetTitleW(L"new");
    REQUIRE(APESaveChangesW() != 0);
    const Bytes f = readFile(p);
    const Bytes a = audio();
    CHECK(audioDiff(f) == -1);
    CHECK(Bytes(f.end() - 128, f.end()) == v1);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetTitleW()) == L"new");
    CHECK(take(ID3V1GetTitleW()) == L"T");
}

TEST_CASE("APE v1: the values are ANSI, a changed tag keeps its version or converts the values", "[tags][spec][ape]")
{
    // "Mueller" with u umlaut in ISO-8859-1
    const Bytes value = { 'M', 0xFC, 'l', 'l', 'e', 'r' };
    auto p = writeWithTail("ape_v1.mp3", apeTag(1000, { apeItem("Artist", value), apeItem("Title", text("t")) }));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetVersionW()) == L"1000");
    CHECK(take(APEGetArtistW()) == L"M\xFC" L"ller");
    APESetTitleW(L"changed");
    REQUIRE(APESaveChangesW() != 0);
    const Bytes saved = readFile(p);
    CHECK(findBytes(saved, apeItem("Artist", value)) != static_cast<size_t>(-1));
    CHECK(findBytes(saved, apeItem("Title", text("changed"))) != static_cast<size_t>(-1));
    CHECK(saved.size() == audio().size() + 32 + apeItem("Artist", value).size() + apeItem("Title", text("changed")).size());
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetTitleW()) == L"changed");
    CHECK(take(APEGetArtistW()) == L"M\xFC" L"ller");   // the untouched item must survive the save
}

TEST_CASE("APE v2: item keys", "[tags][spec][ape]")
{
    auto p = writeTemp("ape_keys.mp3", audio());
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    // keys of 2 to 255 characters from $20 to $7E; ID3, TAG, OggS and MP+ are not allowed
    APESetUserItemW(L"A", L"one character");
    APESetUserItemW(L"ID3", L"x");
    APESetUserItemW(L"TAG", L"x");
    APESetUserItemW(L"OggS", L"x");
    APESetUserItemW(L"MP+", L"x");
    APESetUserItemW(L"Na\xEF" L"ve", L"x");
    APESetUserItemW(std::wstring(256, L'k').c_str(), L"x");
    APESetUserItemW(L"AB", L"two characters");
    APESetUserItemW(std::wstring(255, L'k').c_str(), L"long");
    REQUIRE(APESaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    const std::wstring keys = take(APEGetItemKeysW());
    CHECK(keys.find(L"ID3") == std::wstring::npos);
    CHECK(keys.find(L"TAG") == std::wstring::npos);
    CHECK(keys.find(L"OggS") == std::wstring::npos);
    CHECK(keys.find(L"MP+") == std::wstring::npos);
    CHECK(take(APEGetUserItemW(L"A")).empty());
    CHECK(take(APEGetUserItemW(L"AB")) == L"two characters");
    CHECK(take(APEGetUserItemW(std::wstring(255, L'k').c_str())) == L"long");
    // every key in the file is valid
    const Bytes f = readFile(p);
    CHECK(findBytes(f, bytesOf("Na")) == static_cast<size_t>(-1));
}

TEST_CASE("APE v2: item types", "[tags][spec][ape]")
{
    // flags bits 2..1: 0 text, 1 binary, 2 locator (UTF-8 too)
    const Bytes bin = { 0x00, 0x01, 0x02 };
    auto p = writeWithTail("ape_types.mp3", apeTag(2000, { apeItem("Title", text("text")), apeItem("Cover Art (Front)", bin, 2),
                                                           apeItem("Weblink", text("http://x/y"), 4), apeItem("Artist", text("ro"), 1) }));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetTitleW()) == L"text");
    CHECK(take(APEGetArtistW()) == L"ro");                  // read only is a flag, not a type
    CHECK(take(APEGetUserItemW(L"Cover Art (Front)")).empty());   // binary
    CHECK(take(APEGetUserItemW(L"Weblink")) == L"http://x/y");    // a locator is text
    const std::wstring keys = take(APEGetItemKeysW());
    CHECK(keys.find(L"Cover Art") == std::wstring::npos);
    // the binary item survives a save
    APESetTitleW(L"changed");
    REQUIRE(APESaveChangesW() != 0);
    const Bytes f = readFile(p);
    CHECK(findBytes(f, apeItem("Cover Art (Front)", bin, 2)) != static_cast<size_t>(-1));
    CHECK(findBytes(f, apeItem("Weblink", text("http://x/y"), 4)) != static_cast<size_t>(-1));
}

TEST_CASE("APE v2: a list of values is separated by $00", "[tags][spec][ape]")
{
    Bytes two = { 'A', 'B', 0, 'C', 'D' };
    auto p = writeWithTail("ape_list.mp3", apeTag(2000, { apeItem("Artist", two) }));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetArtistW()).substr(0, 2) == L"AB");
    // an untouched list is written back as it was
    APESetTitleW(L"t");
    REQUIRE(APESaveChangesW() != 0);
    CHECK(findBytes(readFile(p), apeItem("Artist", two)) != static_cast<size_t>(-1));
}

// ---- ID3v1 enhanced tag (TAG+) and APE tags at the beginning of the file ----

namespace {

Bytes field(const std::string& s, size_t n)
{
    Bytes b;
    for (size_t i = 0; i < n; i++) b.push_back(i < s.size() ? static_cast<uint8_t>(s[i]) : 0);
    return b;
}

// 227 bytes: "TAG+", title/artist/album (60 each), speed, genre (30), start time, end time
Bytes enhancedTag(const std::string& title, const std::string& artist, const std::string& album, uint8_t speed, const std::string& genre,
                  const std::string& start, const std::string& end)
{
    Bytes t = bytesOf("TAG+");
    put(t, field(title, 60)); put(t, field(artist, 60)); put(t, field(album, 60));
    t.push_back(speed);
    put(t, field(genre, 30)); put(t, field(start, 6)); put(t, field(end, 6));
    REQUIRE(t.size() == 227);
    return t;
}

Bytes concat(std::initializer_list<Bytes> parts)
{
    Bytes r;
    for (const Bytes& p : parts) put(r, p);
    return r;
}

// tag at the beginning of the file, the audio data and a tail
fs::path writeParts(const char* name, const Bytes& head, const Bytes& tail = Bytes())
{
    return writeTemp(name, concat({ head, audio(), tail }));
}

Bytes id3v2Tag(const char* title)
{
    // header (ID3v2.4, no flags), one TIT2 frame (UTF-8), 20 bytes of padding
    Bytes frame = bytesOf("TIT2");
    const uint32_t n = static_cast<uint32_t>(1 + std::string(title).size());
    frame.insert(frame.end(), { 0, 0, static_cast<uint8_t>(n >> 7), static_cast<uint8_t>(n & 0x7F), 0, 0, 3 });
    put(frame, title);
    frame.insert(frame.end(), 20, 0);
    const uint32_t size = static_cast<uint32_t>(frame.size());
    Bytes t = bytesOf("ID3");
    t.insert(t.end(), { 4, 0, 0, static_cast<uint8_t>((size >> 21) & 0x7F), static_cast<uint8_t>((size >> 14) & 0x7F), static_cast<uint8_t>((size >> 7) & 0x7F), static_cast<uint8_t>(size & 0x7F) });
    put(t, frame);
    return t;
}

std::wstring md5OfAudio(const fs::path& p)
{
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    return take(AUDIOGetMD5ValueW());
}

std::wstring wide(const std::string& s) { return std::wstring(s.begin(), s.end()); }

}  // namespace

TEST_CASE("ID3v1 enhanced tag (TAG+): reading", "[tags][spec][id3v1][enhanced]")
{
    const std::string longTitle = "Title part one 30 characters.." "and part two of the title";   // 30 + 25 characters
    REQUIRE(longTitle.size() == 55);
    const Bytes v1 = id3v1(longTitle.substr(0, 30), "Short", std::string(30, 'a'), "2005", pad30("comment"), 17);
    const Bytes plus = enhancedTag(longTitle.substr(30), "must be ignored", "bcd", 2, "Rock", "001:05", "003:10");
    auto p = writeParts("enh_read.mp3", Bytes(), concat({ plus, v1 }));
    const std::wstring md5 = md5OfAudio(writeParts("enh_plain.mp3", Bytes()));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V1GetTitleW()) == wide(longTitle));
    CHECK(take(ID3V1GetArtistW()) == L"Short");   // this field of the id3v1 tag is not full: the enhanced part does not belong to it
    CHECK(take(ID3V1GetAlbumW()) == wide(std::string(30, 'a') + "bcd"));
    CHECK(take(ID3V1GetCommentW()) == L"comment");
    CHECK(ID3V1ExistsW() != 0);
    CHECK(take(AUDIOGetMD5ValueW()) == md5);   // the enhanced tag does not belong to the audio data
}

TEST_CASE("ID3v1 enhanced tag (TAG+): writing", "[tags][spec][id3v1][enhanced]")
{
    const Bytes v1 = id3v1("T", "A", "B", "2001", pad30("c"), 17);
    const std::string longTitle = "0123456789012345678901234567890123456789012345678901234567890123456789012345";   // 76 characters
    REQUIRE(longTitle.size() == 76);
    SECTION("a long text adds the enhanced tag") {
        auto p = writeParts("enh_write.mp3", Bytes(), v1);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        ID3V1SetTitleW(wide(longTitle).c_str());
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        REQUIRE(f.size() == audio().size() + 355);
        CHECK(audioDiff(f) == -1);
        const Bytes plus(f.end() - 355, f.end() - 128);
        CHECK(Bytes(plus.begin(), plus.begin() + 4) == bytesOf("TAG+"));
        CHECK(Bytes(plus.begin() + 4, plus.begin() + 64) == field(longTitle.substr(30), 60));   // 46 characters continue the title
        CHECK(Bytes(f.end() - 125, f.end() - 95) == field(longTitle.substr(0, 30), 30));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == wide(longTitle));
        CHECK(take(ID3V1GetArtistW()) == L"A");
    }
    SECTION("speed, genre and times are kept, the text may become short again") {
        const Bytes plus = enhancedTag("more", "", "", 3, "Metal", "000:10", "002:00");
        auto p = writeParts("enh_keep.mp3", Bytes(), concat({ plus, id3v1(std::string(30, 't'), "A", "B", "2001", pad30("c"), 17) }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == wide(std::string(30, 't') + "more"));
        ID3V1SetTitleW(L"short");
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        REQUIRE(f.size() == audio().size() + 355);
        const Bytes rest(f.end() - 355 + 184, f.end() - 128);
        CHECK(rest == Bytes(plus.begin() + 184, plus.end()));
        CHECK(audioDiff(f) == -1);
    }
    SECTION("an enhanced tag without data and with short texts is not written again") {
        auto p = writeParts("enh_drop.mp3", Bytes(), concat({ enhancedTag("", "", "", 0, "", "", ""), v1 }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        ID3V1SetTitleW(L"changed");
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        CHECK(f.size() == audio().size() + 128);
        CHECK(audioDiff(f) == -1);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == L"changed");
    }
    SECTION("removing the tag removes both parts") {
        auto p = writeParts("enh_remove.mp3", Bytes(), concat({ enhancedTag("more", "", "", 1, "Pop", "", ""), v1 }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V1RemoveTagFromFileW(p.c_str()) != 0);
        CHECK(readFile(p) == audio());
    }
}

TEST_CASE("ID3v1 enhanced tag (TAG+): together with an APE tag", "[tags][spec][id3v1][enhanced][ape]")
{
    const Bytes plus = enhancedTag("more", "", "", 1, "Pop", "", "");
    const Bytes v1 = id3v1(std::string(30, 't'), "A", "B", "2001", pad30("c"), 17);
    auto p = writeParts("enh_ape.mp3", Bytes(), concat({ apeTag(2000, { apeItem("Title", text("ape")) }), plus, v1 }));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetTitleW()) == L"ape");
    CHECK(take(ID3V1GetTitleW()) == wide(std::string(30, 't') + "more"));
    APESetTitleW(L"changed");
    REQUIRE(APESaveChangesW() != 0);
    const Bytes f = readFile(p);
    CHECK(audioDiff(f) == -1);
    CHECK(Bytes(f.end() - 355, f.end() - 128) == plus);
    CHECK(Bytes(f.end() - 128, f.end()) == v1);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(APEGetTitleW()) == L"changed");
}

TEST_CASE("APE tag at the beginning of the file", "[tags][spec][ape][head]")
{
    const std::wstring md5 = md5OfAudio(writeParts("head_plain.mp3", Bytes()));
    const Bytes tag = apeTag(2000, { apeItem("Title", text("head title")), apeItem("Artist", text("head artist")) });
    auto audioAtEnd = [](const Bytes& f, size_t skipAtEnd = 0) {
        return Bytes(f.end() - static_cast<std::ptrdiff_t>(skipAtEnd + audio().size()), f.end() - static_cast<std::ptrdiff_t>(skipAtEnd)) == audio();
    };

    SECTION("at the start of the file") {
        auto p = writeParts("head_start.mp3", tag);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(APEGetTitleW()) == L"head title");
        CHECK(take(APEGetArtistW()) == L"head artist");
        CHECK(APEExistsW() != 0);
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
        CHECK(take(AUDIOGetTitleW()) == L"head title");

        SECTION("saving keeps the tag at the start") {
            APESetTitleW(L"a much longer changed title");
            APESetUserItemW(L"Custom", L"value");
            REQUIRE(APESaveChangesW() != 0);
            const Bytes f = readFile(p);
            CHECK(Bytes(f.begin(), f.begin() + 8) == bytesOf("APETAGEX"));
            CHECK(audioAtEnd(f));   // nothing was added at the end
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(APEGetTitleW()) == L"a much longer changed title");
            CHECK(take(APEGetUserItemW(L"Custom")) == L"value");
            CHECK(take(APEGetArtistW()) == L"head artist");
            CHECK(take(AUDIOGetMD5ValueW()) == md5);
        }
        SECTION("removing the tag") {
            REQUIRE(APERemoveTagFromFileW(p.c_str()) != 0);
            CHECK(readFile(p) == audio());
            CHECK(APEExistsW() == 0);
        }
    }
    SECTION("behind an ID3v2 tag") {
        const Bytes v2 = id3v2Tag("v2 title");
        auto p = writeParts("head_v2.mp3", concat({ v2, tag }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"v2 title");
        CHECK(take(APEGetTitleW()) == L"head title");
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
        APESetTitleW(L"changed");
        REQUIRE(APESaveChangesW() != 0);
        const Bytes f = readFile(p);
        CHECK(Bytes(f.begin(), f.begin() + static_cast<std::ptrdiff_t>(v2.size())) == v2);
        CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(v2.size()), f.begin() + static_cast<std::ptrdiff_t>(v2.size()) + 8) == bytesOf("APETAGEX"));
        CHECK(audioAtEnd(f));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(APEGetTitleW()) == L"changed");
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"v2 title");
    }
    SECTION("with an ID3v1 tag at the end") {
        const Bytes v1 = id3v1("T", "A", "B", "2001", pad30("c"), 17);
        auto p = writeParts("head_v1.mp3", tag, v1);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(APEGetTitleW()) == L"head title");
        CHECK(take(ID3V1GetTitleW()) == L"T");
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
        APESetTitleW(L"changed");
        REQUIRE(APESaveChangesW() != 0);
        const Bytes f = readFile(p);
        CHECK(Bytes(f.end() - 128, f.end()) == v1);
        CHECK(audioAtEnd(f, 128));
    }
    SECTION("an ID3v2 tag written into a file with an APE tag at the start goes in front of it") {
        auto p = writeParts("head_write_v2.mp3", tag);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 3) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"new v2 title");
        REQUIRE(ID3V2SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"new v2 title");
        CHECK(take(APEGetTitleW()) == L"head title");
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
    }
    SECTION("a corrupt tag at the start is not taken for an APE tag") {
        Bytes bad = tag;
        bad[12] = 0xFF; bad[13] = 0xFF; bad[14] = 0xFF; bad[15] = 0x7F;   // size of the tag larger than the file
        auto p = writeParts("head_bad.mp3", bad);
        AUDIOAnalyzeFileW(p.c_str());   // the tag is not recognized; the frames are found by searching, the result is not important here
        CHECK(APEExistsW() == 0);
    }
}
