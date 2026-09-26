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
