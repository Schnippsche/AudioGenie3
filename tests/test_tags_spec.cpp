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

// a hand made ID3v2.3 frame: 4 byte ID, 4 byte size (plain, not synchsafe: that is only for the outer tag size in v2.3), 2 byte flags
Bytes id3v2Frame(const char* id, const Bytes& body)
{
    Bytes f = bytesOf(id);
    const uint32_t size = static_cast<uint32_t>(body.size());
    f.push_back(static_cast<uint8_t>(size >> 24)); f.push_back(static_cast<uint8_t>(size >> 16));
    f.push_back(static_cast<uint8_t>(size >> 8));  f.push_back(static_cast<uint8_t>(size));
    f.push_back(0); f.push_back(0);
    put(f, body);
    return f;
}

// a hand made ID3v2.3 tag: "ID3", version (3,0), flags $00, synchsafe size, then the frames
Bytes id3v2Tag(const std::vector<Bytes>& frames)
{
    Bytes body;
    for (const Bytes& f : frames) put(body, f);
    Bytes t = bytesOf("ID3");
    t.push_back(3); t.push_back(0); t.push_back(0);
    const uint32_t size = static_cast<uint32_t>(body.size());
    for (int shift = 21; shift >= 0; shift -= 7) t.push_back(static_cast<uint8_t>((size >> shift) & 0x7F));
    put(t, body);
    return t;
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

TEST_CASE("ID3v1: a file of 128 to 130 bytes that holds only the tag", "[tags][spec][id3v1]")
{
    const Bytes tag = id3v1("Title", "Artist", "Album", "1999", pad30("comment"), 9);
    SECTION("130 bytes: the tag is replaced, not added a second time") {
        Bytes b = { 0x01, 0x02 };
        put(b, tag);
        auto p = writeTemp("only_tag_130.mp3", b);
        AUDIOAnalyzeFileW(p.c_str());
        ID3V1SetTitleW(L"New");
        REQUIRE(ID3V1SaveChangesToFileW(p.c_str()) != 0);
        const Bytes f = readFile(p);
        REQUIRE(f.size() == 130);
        CHECK(f[0] == 0x01);
        CHECK(Bytes(f.begin() + 2, f.begin() + 5) == bytesOf("TAG"));
        CHECK(Bytes(f.begin() + 5, f.begin() + 9) == Bytes({ 'N', 'e', 'w', 0 }));
    }
    SECTION("128 bytes: removing the tag leaves an empty file") {
        auto p = writeTemp("only_tag_128.mp3", tag);
        AUDIOAnalyzeFileW(p.c_str());
        REQUIRE(ID3V1RemoveTagFromFileW(p.c_str()) != 0);
        CHECK(readFile(p).empty());
    }
    SECTION("fewer than 8 bytes: no tag") {
        auto p = writeTemp("tiny.mp3", bytesOf("TAGx"));
        AUDIOAnalyzeFileW(p.c_str());
        CHECK(ID3V1RemoveTagFromFileW(p.c_str()) == 0);
        CHECK(readFile(p).size() == 4);
    }
}

TEST_CASE("APE v2: a file that holds only the tag becomes empty when the tag is removed", "[tags][spec][ape]")
{
    auto p = writeTemp("only_ape.mp3", apeTag(2000, { apeItem("Title", text("T")) }));
    AUDIOAnalyzeFileW(p.c_str());
    REQUIRE(APERemoveTagFromFileW(p.c_str()) != 0);
    CHECK(readFile(p).empty());
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

TEST_CASE("ID3v1 enhanced tag: speed, genre text and times through the API", "[tags][spec][id3v1][enhanced]")
{
    const Bytes v1 = id3v1("T", "A", "B", "2001", pad30("c"), 17);
    auto p = writeParts("enh_api.mp3", Bytes(), v1);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(ID3V1GetSpeedW() == 0);
    CHECK(take(ID3V1GetEnhancedGenreW()).empty());
    CHECK(take(ID3V1GetStartTimeW()).empty());
    CHECK(take(ID3V1GetEndTimeW()).empty());

    SECTION("values are written and read again") {
        ID3V1SetSpeedW(3);
        ID3V1SetEnhancedGenreW(L"Progressive Rock");
        ID3V1SetStartTimeW(L"001:05");
        ID3V1SetEndTimeW(L"123:59");
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        REQUIRE(f.size() == audio().size() + 355);
        CHECK(audioDiff(f) == -1);
        const Bytes rest(f.end() - 355 + 184, f.end() - 128);
        CHECK(rest == concat({ Bytes{ 3 }, field("Progressive Rock", 30), field("001:05", 6), field("123:59", 6) }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(ID3V1GetSpeedW() == 3);
        CHECK(take(ID3V1GetEnhancedGenreW()) == L"Progressive Rock");
        CHECK(take(ID3V1GetStartTimeW()) == L"001:05");
        CHECK(take(ID3V1GetEndTimeW()) == L"123:59");
        CHECK(take(ID3V1GetTitleW()) == L"T");
        // everything removed: the enhanced tag is not written again
        ID3V1SetSpeedW(0);
        ID3V1SetEnhancedGenreW(L"");
        ID3V1SetStartTimeW(L"");
        ID3V1SetEndTimeW(L"");
        REQUIRE(ID3V1SaveChangesW() != 0);
        CHECK(readFile(p).size() == audio().size() + 128);
    }
    SECTION("invalid values are not accepted") {
        ID3V1SetSpeedW(5);
        CHECK(ID3V1GetSpeedW() == 0);
        ID3V1SetSpeedW(-1);
        CHECK(ID3V1GetSpeedW() == 0);
        ID3V1SetStartTimeW(L"1:05");
        ID3V1SetStartTimeW(L"001:65");
        ID3V1SetStartTimeW(L"00a:10");
        ID3V1SetStartTimeW(L"001-05");
        CHECK(take(ID3V1GetStartTimeW()).empty());
        ID3V1SetEndTimeW(L"002:30");
        ID3V1SetEndTimeW(L"bad");
        CHECK(take(ID3V1GetEndTimeW()) == L"002:30");   // the invalid value did not replace it
        ID3V1SetEnhancedGenreW(L"0123456789012345678901234567890123456789");
        CHECK(take(ID3V1GetEnhancedGenreW()) == L"012345678901234567890123456789");   // 30 characters
    }
}

namespace {
struct MaxTextLength {
    long old;
    MaxTextLength() : old(GetConfigValueW(9)) {}
    ~MaxTextLength() { SetConfigValueW(9, old); }
};
}  // namespace

TEST_CASE("ID3v1 enhanced tag: configuration value ID3V1MAXTEXTLENGTH", "[tags][spec][id3v1][enhanced][config]")
{
    MaxTextLength guard;
    const std::string longTitle = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";   // 100 characters
    REQUIRE(longTitle.size() == 100);
    CHECK(GetConfigValueW(9) == 90);   // the default
    auto p = writeParts("enh_config.mp3", Bytes(), id3v1("T", "A", "B", "2001", pad30("c"), 17));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);

    SECTION("the default is 90 characters") {
        ID3V1SetTitleW(wide(longTitle).c_str());
        REQUIRE(ID3V1SaveChangesW() != 0);
        CHECK(readFile(p).size() == audio().size() + 355);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == wide(longTitle.substr(0, 90)));
    }
    SECTION("30: no enhanced tag for long texts") {
        SetConfigValueW(9, 30);
        ID3V1SetTitleW(wide(longTitle).c_str());
        REQUIRE(ID3V1SaveChangesW() != 0);
        CHECK(readFile(p).size() == audio().size() + 128);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == wide(longTitle.substr(0, 30)));
    }
    SECTION("30: the enhanced tag is written for a speed") {
        SetConfigValueW(9, 30);
        ID3V1SetTitleW(wide(longTitle).c_str());
        ID3V1SetSpeedW(2);
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        REQUIRE(f.size() == audio().size() + 355);
        CHECK(Bytes(f.end() - 355 + 4, f.end() - 355 + 64) == field("", 60));   // the title continues nowhere
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == wide(longTitle.substr(0, 30)));
        CHECK(ID3V1GetSpeedW() == 2);
    }
    SECTION("50 characters") {
        SetConfigValueW(9, 50);
        ID3V1SetTitleW(wide(longTitle).c_str());
        REQUIRE(ID3V1SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == wide(longTitle.substr(0, 50)));
    }
    SECTION("values outside of 30 to 90 are set to the limit") {
        SetConfigValueW(9, 10);
        CHECK(GetConfigValueW(9) == 30);
        SetConfigValueW(9, 500);
        CHECK(GetConfigValueW(9) == 90);
        SetConfigValueW(9, 61);
        CHECK(GetConfigValueW(9) == 61);
    }
}

// ---- Lyrics3 v1.00 and v2.00 ----

namespace {

// index of the first difference of two byte vectors (the length if one is shorter), -1 if they are equal; keeps the output short
long diffIndex(const Bytes& a, const Bytes& b)
{
    const size_t n = std::min(a.size(), b.size());
    for (size_t i = 0; i < n; i++) if (a[i] != b[i]) return static_cast<long>(i);
    return a.size() == b.size() ? -1 : static_cast<long>(n);
}

// Lyrics3 v2.00 field: ID (3), size (5 digits), data
Bytes lyricsField(const char* id, const std::string& data)
{
    char size[8];
    snprintf(size, sizeof(size), "%05u", static_cast<unsigned>(data.size()));
    Bytes f = bytesOf(id);
    put(f, size);
    put(f, data.c_str());
    return f;
}

// Lyrics3 v2.00 tag: "LYRICSBEGIN", fields, size (6 digits, LYRICSBEGIN and the fields), "LYRICS200"
Bytes lyrics200(const std::vector<Bytes>& fields)
{
    Bytes t = bytesOf("LYRICSBEGIN");
    for (const Bytes& f : fields) put(t, f);
    char size[8];
    snprintf(size, sizeof(size), "%06u", static_cast<unsigned>(t.size()));
    put(t, size);
    put(t, "LYRICS200");
    return t;
}

Bytes lyrics100(const std::string& text)
{
    Bytes t = bytesOf("LYRICSBEGIN");
    put(t, text.c_str());
    put(t, "LYRICSEND");
    return t;
}

// built on first use: the helper uses REQUIRE, which must not run during static initialization
const Bytes& kV1() { static const Bytes t = id3v1("Title", "Artist", "Album", "2001", pad30("comment"), 17); return t; }

}  // namespace

TEST_CASE("Lyrics3 v2.00: reading all defined fields", "[tags][spec][lyrics]")
{
    const std::string lyricsText = "[00:01]First line\r\n[00:05]Second line";
    const Bytes tag = lyrics200({ lyricsField("IND", "11"), lyricsField("LYR", lyricsText), lyricsField("INF", "some\r\ninformation"),
                                  lyricsField("AUT", "the author"), lyricsField("EAL", "extended album"), lyricsField("EAR", "extended artist"),
                                  lyricsField("ETT", "extended title"), lyricsField("IMG", "cover.jpg||the cover||[00:00]") });
    const std::wstring md5 = md5OfAudio(writeParts("lyr_plain.mp3", Bytes(), kV1()));
    auto p = writeParts("lyr_read.mp3", Bytes(), concat({ tag, kV1() }));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(LYRICSExistsW() != 0);
    CHECK(take(LYRICSGetVersionW()) == L"2.00");
    CHECK(take(LYRICSGetIndicationW()) == L"11");
    CHECK(take(LYRICSGetLyricsW()) == wide(lyricsText));
    CHECK(take(LYRICSGetInformationW()) == L"some\r\ninformation");
    CHECK(take(LYRICSGetAuthorW()) == L"the author");
    CHECK(take(LYRICSGetAlbumW()) == L"extended album");
    CHECK(take(LYRICSGetArtistW()) == L"extended artist");
    CHECK(take(LYRICSGetTitleW()) == L"extended title");
    CHECK(take(LYRICSGetImageLinkW()) == L"cover.jpg||the cover||[00:00]");
    CHECK(LYRICSGetSizeW() == static_cast<long>(tag.size()));   // the whole tag: LYRICSBEGIN, fields, size and LYRICS200
    CHECK(LYRICSGetStartPositionW() == static_cast<long>(audio().size()));
    CHECK(take(ID3V1GetTitleW()) == L"Title");
    CHECK(take(AUDIOGetMD5ValueW()) == md5);
}

TEST_CASE("Lyrics3 v1.00: reading", "[tags][spec][lyrics]")
{
    SECTION("a text") {
        const std::string text = "Line one\r\nLine two";
        auto p = writeParts("lyr_v1.mp3", Bytes(), concat({ lyrics100(text), kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetVersionW()) == L"1.00");
        CHECK(take(LYRICSGetLyricsW()) == wide(text));
        CHECK(LYRICSGetSizeW() == static_cast<long>(11 + text.size() + 9));
        CHECK(LYRICSGetStartPositionW() == static_cast<long>(audio().size()));
    }
    SECTION("the longest text of 5100 bytes") {
        const std::string text(5100, 'x');
        auto p = writeParts("lyr_v1_max.mp3", Bytes(), concat({ lyrics100(text), kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK((take(LYRICSGetLyricsW()) == wide(text)));
        CHECK(LYRICSGetStartPositionW() == static_cast<long>(audio().size()));
    }
    SECTION("a change writes a v2.00 tag and keeps the ID3v1 tag") {
        auto p = writeParts("lyr_v1_upgrade.mp3", Bytes(), concat({ lyrics100("old text"), kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        LYRICSSetLyricsW(L"new text");
        REQUIRE(LYRICSSaveChangesW() != 0);
        CHECK(diffIndex(readFile(p), concat({ audio(), lyrics200({ lyricsField("LYR", "new text") }), kV1() })) == -1);
    }
}

TEST_CASE("Lyrics3 v1.00: an APE tag between it and the ID3v1 tag is handled", "[tags][spec][lyrics][ape]")
{
    const std::string lyricsText = "Line one\r\nLine two";
    const Bytes ape = apeTag(2000, { apeItem("Title", text("APE title")) });
    SECTION("reading") {
        auto p = writeParts("lyr_v1_ape_read.mp3", Bytes(), concat({ lyrics100(lyricsText), ape, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetVersionW()) == L"1.00");
        CHECK(take(LYRICSGetLyricsW()) == wide(lyricsText));
        CHECK(LYRICSGetSizeW() == static_cast<long>(11 + lyricsText.size() + 9));
        CHECK(take(APEGetTitleW()) == L"APE title");   // the APE tag behind it is still readable on its own
        CHECK(take(ID3V1GetTitleW()) == L"Title");
    }
    SECTION("a change upgrades to v2.00, keeping the APE and ID3v1 tags intact") {
        auto p = writeParts("lyr_v1_ape_write.mp3", Bytes(), concat({ lyrics100(lyricsText), ape, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        LYRICSSetLyricsW(L"new text");
        REQUIRE(LYRICSSaveChangesW() != 0);
        CHECK(diffIndex(readFile(p), concat({ audio(), lyrics200({ lyricsField("LYR", "new text") }), ape, kV1() })) == -1);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetVersionW()) == L"2.00");
        CHECK(take(LYRICSGetLyricsW()) == L"new text");
        CHECK(take(APEGetTitleW()) == L"APE title");
        CHECK(take(ID3V1GetTitleW()) == L"Title");
    }
    SECTION("removing keeps the APE and ID3v1 tags intact") {
        auto p = writeParts("lyr_v1_ape_remove.mp3", Bytes(), concat({ lyrics100(lyricsText), ape, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(LYRICSRemoveTagFromFileW(p.c_str()) != 0);
        CHECK(diffIndex(readFile(p), concat({ audio(), ape, kV1() })) == -1);
    }
}

TEST_CASE("Lyrics3 v2.00: layout of a written tag", "[tags][spec][lyrics]")
{
    auto p = writeParts("lyr_write.mp3", Bytes(), kV1());
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(LYRICSExistsW() == 0);
    // the fields are written in the order IND, LYR, INF, AUT, EAL, EAR, ETT, IMG: the indication is the first field
    LYRICSSetImageLinkW(L"cover.jpg");
    LYRICSSetTitleW(L"title");
    LYRICSSetLyricsW(L"lyrics");
    LYRICSSetIndicationW(L"10");
    LYRICSSetAuthorW(L"author");
    REQUIRE(LYRICSSaveChangesW() != 0);
    const Bytes expected = concat({ audio(), lyrics200({ lyricsField("IND", "10"), lyricsField("LYR", "lyrics"), lyricsField("AUT", "author"),
                                                          lyricsField("ETT", "title"), lyricsField("IMG", "cover.jpg") }), kV1() });
    CHECK(diffIndex(readFile(p), expected) == -1);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(LYRICSGetLyricsW()) == L"lyrics");
    CHECK(take(ID3V1GetTitleW()) == L"Title");
}

TEST_CASE("Lyrics3 v2.00: limits of the fields, line breaks and the indication", "[tags][spec][lyrics]")
{
    auto p = writeParts("lyr_limits.mp3", Bytes(), kV1());
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);

    SECTION("a field has at most 99999 characters (LYR, INF, IMG) or 250 characters (AUT, EAL, EAR, ETT)") {
        LYRICSSetLyricsW(wide(std::string(100000, 'l')).c_str());
        LYRICSSetAuthorW(wide(std::string(300, 'a')).c_str());
        REQUIRE(LYRICSSaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK((take(LYRICSGetLyricsW()) == wide(std::string(99999, 'l'))));
        CHECK(take(LYRICSGetAuthorW()) == wide(std::string(250, 'a')));
        CHECK(take(ID3V1GetTitleW()) == L"Title");
        CHECK(audioDiff(readFile(p)) == -1);
    }
    SECTION("line breaks are CR LF") {
        LYRICSSetLyricsW(L"one\ntwo\rthree\r\nfour");
        REQUIRE(LYRICSSaveChangesW() != 0);
        CHECK(diffIndex(readFile(p), concat({ audio(), lyrics200({ lyricsField("LYR", "one\r\ntwo\r\nthree\r\nfour") }), kV1() })) == -1);
    }
    SECTION("the indication has two characters, 0 or 1") {
        LYRICSSetLyricsW(L"text");
        LYRICSSetIndicationW(L"1x");
        REQUIRE(LYRICSSaveChangesW() != 0);
        CHECK(diffIndex(readFile(p), concat({ audio(), lyrics200({ lyricsField("LYR", "text") }), kV1() })) == -1);
    }
}

TEST_CASE("Lyrics3 v2.00: unknown fields are kept", "[tags][spec][lyrics]")
{
    const Bytes tag = lyrics200({ lyricsField("LYR", "text"), lyricsField("XYZ", "unknown data"), lyricsField("AUT", "me") });
    auto p = writeParts("lyr_unknown.mp3", Bytes(), concat({ tag, kV1() }));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(LYRICSGetAuthorW()) == L"me");
    LYRICSSetAuthorW(L"you");
    REQUIRE(LYRICSSaveChangesW() != 0);
    const Bytes f = readFile(p);
    CHECK(findBytes(f, lyricsField("XYZ", "unknown data")) != static_cast<size_t>(-1));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(LYRICSGetAuthorW()) == L"you");
    CHECK(take(LYRICSGetLyricsW()) == L"text");
}

TEST_CASE("Lyrics3: removing the tag and a missing ID3v1 tag", "[tags][spec][lyrics]")
{
    SECTION("remove") {
        auto p = writeParts("lyr_remove.mp3", Bytes(), concat({ lyrics200({ lyricsField("LYR", "text") }), kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(LYRICSRemoveTagFromFileW(p.c_str()) != 0);
        CHECK(diffIndex(readFile(p), concat({ audio(), kV1() })) == -1);
    }
    SECTION("a Lyrics3 tag needs an ID3v1 tag") {
        auto p = writeParts("lyr_nov1.mp3", Bytes());
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        LYRICSSetLyricsW(L"text");
        CHECK(LYRICSSaveChangesW() == 0);
        CHECK(diffIndex(readFile(p), audio()) == -1);
    }
}

TEST_CASE("Lyrics3: together with the enhanced ID3v1 tag", "[tags][spec][lyrics][enhanced]")
{
    const Bytes plus = enhancedTag("more title", "", "", 2, "Rock", "000:10", "003:00");
    const Bytes v1 = id3v1(std::string(30, 't'), "A", "B", "2001", pad30("c"), 17);
    const Bytes tag = lyrics200({ lyricsField("LYR", "text"), lyricsField("AUT", "me") });
    auto p = writeParts("lyr_enh.mp3", Bytes(), concat({ tag, plus, v1 }));
    const std::wstring md5 = md5OfAudio(writeParts("lyr_enh_plain.mp3", Bytes()));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(LYRICSGetLyricsW()) == L"text");
    CHECK(take(ID3V1GetTitleW()) == wide(std::string(30, 't') + "more title"));
    CHECK(take(AUDIOGetMD5ValueW()) == md5);
    LYRICSSetAuthorW(L"you");
    REQUIRE(LYRICSSaveChangesW() != 0);
    CHECK(diffIndex(readFile(p), concat({ audio(), lyrics200({ lyricsField("LYR", "text"), lyricsField("AUT", "you") }), plus, v1 })) == -1);
}

TEST_CASE("Lyrics3 v2.00: damaged tags", "[tags][spec][lyrics]")
{
    SECTION("the size of a field reaches beyond the tag") {
        Bytes tag = lyrics200({ lyricsField("LYR", "text"), lyricsField("AUT", "me") });
        // the size of the last field (5 digits behind its ID) is changed to 99999; the size of the tag stays the same
        const size_t i = findBytes(tag, bytesOf("AUT"));
        REQUIRE(i != static_cast<size_t>(-1));
        for (size_t k = 0; k < 5; k++) tag[i + 3 + k] = '9';
        auto p = writeParts("lyr_bad1.mp3", Bytes(), concat({ tag, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetLyricsW()) == L"text");
        CHECK(take(LYRICSGetAuthorW()).empty());
    }
    SECTION("a field size that is not a number does not stop the analysis") {
        Bytes tag = lyrics200({ lyricsField("LYR", "text"), lyricsField("AUT", "me") });
        const size_t i = findBytes(tag, bytesOf("AUT"));
        tag[i + 3] = '-'; tag[i + 4] = '1'; tag[i + 5] = 'x'; tag[i + 6] = 'x'; tag[i + 7] = 'x';
        auto p = writeParts("lyr_bad2.mp3", Bytes(), concat({ tag, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetLyricsW()) == L"text");
    }
    SECTION("the size of the tag does not lead to LYRICSBEGIN") {
        Bytes tag = lyrics200({ lyricsField("LYR", "text") });
        const size_t n = tag.size();
        tag[n - 15] = '0'; tag[n - 14] = '0'; tag[n - 13] = '0'; tag[n - 12] = '0'; tag[n - 11] = '9'; tag[n - 10] = '9';   // 000099: too small
        auto p = writeParts("lyr_bad3.mp3", Bytes(), concat({ tag, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(LYRICSExistsW() == 0);
        CHECK(take(LYRICSGetLyricsW()).empty());
    }
}

// ---- order of the tags at the end of the file: APE, Lyrics3 and ID3v1 (with the enhanced tag) ----

TEST_CASE("Tags at the end of the file: APE, Lyrics3 and ID3v1 in every order", "[tags][spec][order]")
{
    const Bytes ape = apeTag(2000, { apeItem("Title", text("ape title")) });
    const Bytes apeChanged = apeTag(2000, { apeItem("Title", text("changed")) });
    const Bytes lyr = lyrics200({ lyricsField("LYR", "lyrics text"), lyricsField("AUT", "author") });
    const Bytes lyrChanged = lyrics200({ lyricsField("LYR", "lyrics text"), lyricsField("AUT", "changed") });
    const Bytes v1 = id3v1("Title", "Artist", "Album", "2001", pad30("comment"), 17);
    const Bytes v1Long = id3v1(std::string(30, 't'), "Artist", "Album", "2001", pad30("comment"), 17);   // a full title continues in the enhanced tag
    const Bytes plus = enhancedTag("more", "", "", 2, "Rock", "000:10", "003:00");
    const Bytes idTags = concat({ plus, v1Long });
    const Bytes apeNew = apeTag(2000, { apeItem("TITLE", text("changed")) });   // the key of a new item is TITLE
    const std::wstring md5 = md5OfAudio(writeParts("order_plain.mp3", Bytes()));

    struct Order { const char* name; bool apeFirst; bool hasApe, hasLyr, enhanced; };
    const Order orders[] = {
        { "APE, ID3v1", true, true, false, false },
        { "Lyrics3, ID3v1", true, false, true, false },
        { "APE, Lyrics3, ID3v1", true, true, true, false },
        { "Lyrics3, APE, ID3v1", false, true, true, false },
        { "APE, Lyrics3, TAG+, ID3v1", true, true, true, true },
        { "Lyrics3, APE, TAG+, ID3v1", false, true, true, true },
        { "Lyrics3, TAG+, ID3v1", true, false, true, true },
    };
    for (const Order& o : orders) {
        INFO(o.name);
        const Bytes& id = o.enhanced ? idTags : v1;
        auto build = [&](const Bytes& a, const Bytes& l) {
            Bytes tail;
            if (o.apeFirst) { if (o.hasApe) put(tail, a); if (o.hasLyr) put(tail, l); }
            else { if (o.hasLyr) put(tail, l); if (o.hasApe) put(tail, a); }
            put(tail, id);
            return concat({ audio(), tail });
        };

        {
            auto p = writeTemp("order_read.mp3", build(ape, lyr));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(ID3V1GetTitleW()) == (o.enhanced ? wide(std::string(30, 't') + "more") : L"Title"));
            CHECK((APEExistsW() != 0) == o.hasApe);
            CHECK((LYRICSExistsW() != 0) == o.hasLyr);
            if (o.hasApe) CHECK(take(APEGetTitleW()) == L"ape title");
            if (o.hasLyr) {
                CHECK(take(LYRICSGetLyricsW()) == L"lyrics text");
                CHECK(take(LYRICSGetAuthorW()) == L"author");
                const long expectedStart = static_cast<long>(audio().size()) + ((o.apeFirst && o.hasApe) ? static_cast<long>(ape.size()) : 0);
                CHECK(LYRICSGetStartPositionW() == expectedStart);
                CHECK(LYRICSGetSizeW() == static_cast<long>(lyr.size()));
            }
            CHECK(take(AUDIOGetMD5ValueW()) == md5);   // the audio data end in front of the first tag
        }
        if (o.hasApe) {
            auto p = writeTemp("order_ape_save.mp3", build(ape, lyr));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            APESetTitleW(L"changed");
            REQUIRE(APESaveChangesW() != 0);
            CHECK(diffIndex(readFile(p), build(apeChanged, lyr)) == -1);   // the APE tag is written again at its place
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(APEGetTitleW()) == L"changed");
            CHECK(take(AUDIOGetMD5ValueW()) == md5);

            auto q = writeTemp("order_ape_remove.mp3", build(ape, lyr));
            REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == MPEG);
            REQUIRE(APERemoveTagFromFileW(q.c_str()) != 0);
            Bytes expected = audio();
            if (o.hasLyr) put(expected, lyr);
            put(expected, id);
            CHECK(diffIndex(readFile(q), expected) == -1);
        }
        if (o.hasLyr) {
            auto p = writeTemp("order_lyr_save.mp3", build(ape, lyr));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            LYRICSSetAuthorW(L"changed");
            REQUIRE(LYRICSSaveChangesW() != 0);
            CHECK(diffIndex(readFile(p), build(ape, lyrChanged)) == -1);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(LYRICSGetAuthorW()) == L"changed");
            CHECK(take(AUDIOGetMD5ValueW()) == md5);

            auto q = writeTemp("order_lyr_remove.mp3", build(ape, lyr));
            REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == MPEG);
            REQUIRE(LYRICSRemoveTagFromFileW(q.c_str()) != 0);
            Bytes expected = audio();
            if (o.hasApe) put(expected, ape);
            put(expected, id);
            CHECK(diffIndex(readFile(q), expected) == -1);
            REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == MPEG);
            if (o.hasApe) CHECK(take(APEGetTitleW()) == L"ape title");
        }
        if (!o.hasApe) {
            // a new APE tag is written in front of the ID3v1 data, behind a Lyrics3 tag
            auto p = writeTemp("order_ape_new.mp3", build(ape, lyr));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            APESetTitleW(L"changed");
            REQUIRE(APESaveChangesW() != 0);
            Bytes expected = audio();
            put(expected, lyr);
            put(expected, apeNew);
            put(expected, id);
            CHECK(diffIndex(readFile(p), expected) == -1);
        }
        if (!o.hasLyr) {
            // a new Lyrics3 tag is written in front of the ID3v1 data, behind an APE tag
            auto p = writeTemp("order_lyr_new.mp3", build(ape, lyr));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            LYRICSSetLyricsW(L"lyrics text");
            LYRICSSetAuthorW(L"changed");
            REQUIRE(LYRICSSaveChangesW() != 0);
            Bytes expected = audio();
            put(expected, ape);
            put(expected, lyrChanged);
            put(expected, id);
            CHECK(diffIndex(readFile(p), expected) == -1);
        }
    }
}

TEST_CASE("Tags: the abstract getters fall back to ID3v1 when the ID3v2 tag has none of the tracked fields", "[tags][spec][fallback]")
{
    // an ID3v2 tag that exists but holds only a private, non-standard frame (as some old ripping tools did, e.g. RealJukebox's
    // "RealJukebox:Metadata" GEOB frame): none of TIT2/TPE1/TALB/TCON/COMM/TRCK/TYER/TCOM are present at all
    Bytes priv = bytesOf("test-owner"); priv.push_back(0); priv.push_back(0xAB);
    Bytes b = id3v2Tag({ id3v2Frame("PRIV", priv) });
    put(b, audio());
    put(b, id3v1("Old Title", "Old Artist", "Old Album", "1998", pad30("Old Comment"), 17));   // 17 = Rock
    auto p = writeTemp("fallback_priv_id3v1.mp3", b);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    // the ID3v2 tag itself really has no TIT2/TPE1 frame at all
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"");
    CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"");
    // but the abstract getters, meant to give the best available data, fall through to the ID3v1 tag underneath
    CHECK(take(AUDIOGetTitleW()) == L"Old Title");
    CHECK(take(AUDIOGetArtistW()) == L"Old Artist");
    CHECK(take(AUDIOGetAlbumW()) == L"Old Album");
    CHECK(take(AUDIOGetGenreW()) == L"Rock");
    CHECK(take(AUDIOGetYearW()) == L"1998");
}

TEST_CASE("Tags: a field missing from an otherwise partially filled ID3v2 tag still falls back to ID3v1", "[tags][spec][fallback]")
{
    // the ID3v2 tag has only TCON ("Rock") and TIT2 ("Dancing with Myself"), no TPE1 - a common real-world pattern from old
    // rippers that only ever wrote a couple of frames; an ID3v1 tag underneath carries the missing artist. The fallback is per
    // field, not per tag: Title/Genre come from ID3v2 (where they are genuinely present), Artist/Album from ID3v1 underneath.
    Bytes tcon; tcon.push_back(0); put(tcon, bytesOf("Rock"));
    Bytes tit2; tit2.push_back(0); put(tit2, bytesOf("Dancing with Myself"));
    Bytes b = id3v2Tag({ id3v2Frame("TCON", tcon), id3v2Frame("TIT2", tit2) });
    put(b, audio());
    put(b, id3v1("Stale Title", "Old Artist", "Old Album", "1998", pad30("Old Comment"), 17));
    auto p = writeTemp("fallback_partial_v2.mp3", b);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(AUDIOGetGenreW()) == L"Rock");                   // from ID3v2
    CHECK(take(AUDIOGetTitleW()) == L"Dancing with Myself");    // from ID3v2, not the stale ID3v1 title underneath
    CHECK(take(AUDIOGetArtistW()) == L"Old Artist");            // ID3v2 has no TPE1 at all: falls back to ID3v1
    CHECK(take(AUDIOGetAlbumW()) == L"Old Album");              // ID3v2 has no TALB at all: falls back to ID3v1
}

TEST_CASE("Tags: clearing a field through the abstract API also clears it in an ID3v1 tag that already exists", "[tags][spec][fallback][write]")
{
    // the file has both an ID3v2 and an ID3v1 tag with real, matching values (as a normal, previously tagged file would).
    // AUDIOSetTitleW(L"")+AUDIOSetArtistW(L"")+AUDIOSaveChangesW clears the ID3v2 frames as usual, and must also clear the
    // ID3v1 fields - otherwise the next read would fall back to the now-stale "Old Title"/"Old Artist" in ID3v1 underneath.
    Bytes tit2; tit2.push_back(0); put(tit2, bytesOf("Old Title"));
    Bytes tpe1; tpe1.push_back(0); put(tpe1, bytesOf("Old Artist"));
    Bytes tcon; tcon.push_back(0); put(tcon, bytesOf("Rock"));
    Bytes b = id3v2Tag({ id3v2Frame("TIT2", tit2), id3v2Frame("TPE1", tpe1), id3v2Frame("TCON", tcon) });
    put(b, audio());
    put(b, id3v1("Old Title", "Old Artist", "Old Album", "1998", pad30("Old Comment"), 17));
    auto p = writeTemp("clear_syncs_id3v1.mp3", b);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(take(AUDIOGetTitleW()) == L"Old Title");
    REQUIRE(take(ID3V1GetTitleW()) == L"Old Title");

    AUDIOSetTitleW(L""); AUDIOSetArtistW(L"");
    REQUIRE(AUDIOSaveChangesW() != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"");
    CHECK(take(AUDIOGetArtistW()) == L"");
    CHECK(take(AUDIOGetGenreW()) == L"Rock");           // untouched field, kept
    // the literal ID3v1 tag itself was updated too, not just the abstract view of it
    CHECK(take(ID3V1GetTitleW()) == L"");
    CHECK(take(ID3V1GetArtistW()) == L"");
}

TEST_CASE("Tags at the end of the file that are larger than the cache of the end of the file", "[tags][spec][lyrics][ape][tail]")
{
    // the tags at the end and the last MPEG frames are read from a cache of the last 8192 bytes of the file; a tag that is larger than that has to be
    // found, and the values of the audio data (frames, duration, MD5) must not depend on it
    auto plain = writeParts("tail_plain.mp3", Bytes(), kV1());
    REQUIRE(AUDIOAnalyzeFileW(plain.c_str()) == MPEG);
    const long frames = MPEGGetFramesW();
    const float duration = AUDIOGetDurationW();
    const std::wstring md5 = take(AUDIOGetMD5ValueW());
    REQUIRE(frames > 0);

    SECTION("Lyrics3 v2.00 of 12000 bytes in front of the ID3v1 tag") {
        const std::string longLyrics(12000, 'x');
        const Bytes tag = lyrics200({ lyricsField("IND", "10"), lyricsField("LYR", longLyrics) });
        auto p = writeParts("tail_lyrics.mp3", Bytes(), concat({ tag, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetLyricsW()) == wide(longLyrics));
        CHECK(LYRICSGetStartPositionW() == static_cast<long>(audio().size()));
        CHECK(take(ID3V1GetTitleW()) == L"Title");
        CHECK(MPEGGetFramesW() == frames);
        CHECK(AUDIOGetDurationW() == duration);
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
    }
    SECTION("APE v2 of 10000 bytes in front of the ID3v1 tag") {
        const Bytes big(10000, 0x41);
        auto p = writeParts("tail_ape.mp3", Bytes(), concat({ apeTag(2000, { apeItem("Title", text("ape title")), apeItem("Cover Art (Front)", big, 2) }), kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(APEGetTitleW()) == L"ape title");
        CHECK(take(ID3V1GetTitleW()) == L"Title");
        CHECK(MPEGGetFramesW() == frames);
        CHECK(AUDIOGetDurationW() == duration);
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
    }
    SECTION("a file that is smaller than the cache, with all three tags") {
        const Bytes tag = lyrics200({ lyricsField("LYR", "short") });
        auto p = writeParts("tail_small.mp3", Bytes(), concat({ apeTag(2000, { apeItem("Title", text("ape title")) }), tag, kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(APEGetTitleW()) == L"ape title");
        CHECK(take(LYRICSGetLyricsW()) == L"short");
        CHECK(take(ID3V1GetTitleW()) == L"Title");
        CHECK(MPEGGetFramesW() == frames);
        CHECK(take(AUDIOGetMD5ValueW()) == md5);
    }
}

TEST_CASE("APE v2: tags around the size of the cache of the end (8 KB) and with long keys", "[tags][spec][ape]")
{
    // The items are read in sequence from the cache of the end of the file, or from the file if the tag is larger. Between the large item and the
    // end there are items with a key of 255 characters and one that ends exactly at the end of the tag.
    for (size_t n : std::vector<size_t>{ 0, 100, 4000, 8000, 8100, 8160, 8192, 9000, 70000 }) {
        INFO("size of the first item: " << n);
        const std::string longKey(255, 'k');
        const Bytes tag = apeTag(2000, { apeItem("Filler", Bytes(n, 'x')), apeItem("Title", text("after the filler")), apeItem(longKey, text("long key")), apeItem("Artist", text("last item")) });
        for (bool v1 : { false, true }) {
            Bytes tail = tag;
            if (v1)
                put(tail, id3v1("T", "A", "B", "2001", pad30("c"), 17));
            auto p = writeWithTail("ape_sizes.mp3", tail);
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(APEGetTitleW()) == L"after the filler");
            CHECK(take(APEGetArtistW()) == L"last item");
            CHECK(take(APEGetUserItemW(std::wstring(255, L'k').c_str())) == L"long key");
            CHECK(AUDIOGetDurationW() > 0);
        }
    }
}

namespace {
// ANSICODEPAGE (7) for one test, back to the default 1252 also if a REQUIRE ends it
struct AnsiCodePage {
    explicit AnsiCodePage(long codePage) { SetConfigValueW(7, codePage); }
    ~AnsiCodePage() { SetConfigValueW(7, 1252); }
};
}  // namespace

TEST_CASE("ID3v1 and Lyrics3: the texts are read with the code page they are written with (ANSICODEPAGE)", "[tags][spec][id3v1][lyrics][codepage]")
{
    // the byte 0xE9 is U+0439 (Cyrillic short i) in code page 1251, U+00E9 (e acute) in 1252
    AnsiCodePage cp(1251);
    SECTION("ID3v1") {
        auto p = writeParts("cp_v1.mp3", Bytes(), id3v1("Caf\xE9", "Artist", "Album", "2001", pad30("comment"), 17));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == L"Cafй");
        ID3V1SetTitleW(L"йц");   // 0xE9 0xF6 in 1251
        REQUIRE(ID3V1SaveChangesW() != 0);
        const Bytes f = readFile(p);
        CHECK(Bytes(f.end() - 125, f.end() - 122) == Bytes({ 0xE9, 0xF6, 0x00 }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V1GetTitleW()) == L"йц");
    }
    SECTION("Lyrics3") {
        auto p = writeParts("cp_lyr.mp3", Bytes(), concat({ lyrics200({ lyricsField("LYR", "text"), lyricsField("ETT", "Caf\xE9") }), kV1() }));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetTitleW()) == L"Cafй");
        LYRICSSetTitleW(L"йц");
        REQUIRE(LYRICSSaveChangesW() != 0);
        CHECK(findBytes(readFile(p), Bytes({ 'E', 'T', 'T', '0', '0', '0', '0', '2', 0xE9, 0xF6 })) != static_cast<size_t>(-1));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(LYRICSGetTitleW()) == L"йц");
    }
}
