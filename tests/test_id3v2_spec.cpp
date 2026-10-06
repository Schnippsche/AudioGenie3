// ID3v2.4 specification checks with hand made tags (id3v2.4.0-structure and id3v2.4.0-frames): tag header, extended header,
// footer, frame flags, text encodings and text frames with several strings.
#include "id3v2_support.h"
#include <string>

using namespace ag3test;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
Bytes bytesOf(const char* s) { Bytes b; put(b, s); return b; }

Bytes synchsafe(uint32_t v)
{
    return { static_cast<uint8_t>((v >> 21) & 0x7f), static_cast<uint8_t>((v >> 14) & 0x7f), static_cast<uint8_t>((v >> 7) & 0x7f), static_cast<uint8_t>(v & 0x7f) };
}

Bytes be32(uint32_t v)
{
    return { static_cast<uint8_t>(v >> 24), static_cast<uint8_t>(v >> 16), static_cast<uint8_t>(v >> 8), static_cast<uint8_t>(v) };
}

// frame of the given tag version (2.3: plain size, 2.4: synchsafe size)
Bytes frame(const char* id, const Bytes& data, uint16_t flags = 0, int version = 4)
{
    Bytes f;
    put(f, id);
    put(f, version == 4 ? synchsafe(static_cast<uint32_t>(data.size())) : be32(static_cast<uint32_t>(data.size())));
    f.push_back(static_cast<uint8_t>(flags >> 8));
    f.push_back(static_cast<uint8_t>(flags));
    put(f, data);
    return f;
}

// header + body (extended header, frames) + padding + optional footer
Bytes tagBytes(int version, uint8_t flags, const Bytes& body, size_t padding = 0, const Bytes& footer = Bytes())
{
    Bytes t;
    put(t, "ID3");
    t.push_back(static_cast<uint8_t>(version));
    t.push_back(0);
    t.push_back(flags);
    put(t, synchsafe(static_cast<uint32_t>(body.size() + padding)));
    put(t, body);
    t.insert(t.end(), padding, 0);
    put(t, footer);
    return t;
}

Bytes audioFrames() { return makeMp3(30); }

// writes tag + audio; the audio is the reference for "the audio data was not touched"
fs::path writeTagged(const char* name, const Bytes& tag)
{
    Bytes b = tag;
    put(b, audioFrames());
    return writeTemp(name, b);
}

bool audioIntact(const fs::path& p)
{
    const Bytes file = readFile(p);
    const Bytes audio = audioFrames();
    return file.size() >= audio.size() && std::equal(audio.begin(), audio.end(), file.end() - static_cast<std::ptrdiff_t>(audio.size()));
}

size_t findBytes(const Bytes& hay, const Bytes& needle, size_t from = 0)
{
    if (from >= hay.size()) return static_cast<size_t>(-1);
    auto it = std::search(hay.begin() + static_cast<std::ptrdiff_t>(from), hay.end(), needle.begin(), needle.end());
    return it == hay.end() ? static_cast<size_t>(-1) : static_cast<size_t>(it - hay.begin());
}

u32 frameId(const char* s)
{
    return (static_cast<u32>(static_cast<uint8_t>(s[0])) << 24) | (static_cast<u32>(static_cast<uint8_t>(s[1])) << 16) |
           (static_cast<u32>(static_cast<uint8_t>(s[2])) << 8) | static_cast<u32>(static_cast<uint8_t>(s[3]));
}

// changes a frame and saves the tag as v2.4 with the given encoding
void saveAsV24(short encoding)
{
    REQUIRE(ID3V2SetFormatAndEncodingW(3, encoding) != 0);
    ID3V2SetTextFrameW(ID3F_TPE1, L"Changed");
    REQUIRE(ID3V2SaveChangesW() != 0);
}

}  // namespace

TEST_CASE("ID3v2.4: encoding $02 is UTF-16BE without BOM", "[id3v2][spec][encoding]")
{
    SECTION("read") {
        auto p = writeTagged("spec_be_read.mp3", tagBytes(4, 0, frame("TIT2", { 0x02, 0x00, 0x41, 0x00, 0x42 }), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"AB");
    }
    SECTION("write") {
        auto p = writeTagged("spec_be_write.mp3", tagBytes(4, 0, frame("TIT2", { 0x03, 'x' }), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 2) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"AB");
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("TIT2"));
        REQUIRE(i != static_cast<size_t>(-1));
        const Bytes expected = { 0x02, 0x00, 0x41, 0x00, 0x42 };
        CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 15) == expected);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"AB");
    }
}

TEST_CASE("ID3v2.4: text frames with several strings keep all values", "[id3v2][spec][text]")
{
    const Bytes twoValues = { 0x03, 'A', 0x00, 'B' };
    auto p = writeTagged("spec_multi.mp3", tagBytes(4, 0, frame("TPE1", twoValues), 100));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"A");   // the API returns the first value

    SECTION("re-encoded as UTF-16 in a v2.4 tag") {
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 1) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("TPE1"));
        REQUIRE(i != static_cast<size_t>(-1));
        // encoding $01, BOM + 'A' + $00 $00, BOM + 'B'; each BOM's FF is followed by FE (>= 0xE0, i.e. it looks like the
        // start of an MPEG frame sync), so unsynchronisation inserts a $00 right after both of them
        const Bytes expected = { 0x01, 0xFF, 0x00, 0xFE, 'A', 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFE, 'B', 0x00 };
        CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 10 + static_cast<std::ptrdiff_t>(expected.size())) == expected);
    }
    SECTION("re-encoded as UTF-8 in a v2.4 tag") {
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 0) != 0);   // forces a rebuild of the text frames
        ID3V2SetTextFrameW(ID3F_TIT2, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 3) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"y");
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes f = readFile(p);
        CHECK(findBytes(f, twoValues) != static_cast<size_t>(-1));
    }
    SECTION("a v2.3 tag joins the values with a slash") {
        REQUIRE(ID3V2SetFormatAndEncodingW(2, 0) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"A/B");
    }
    SECTION("a final terminator does not create a second value") {
        auto q = writeTagged("spec_multi_term.mp3", tagBytes(4, 0, frame("TPE1", { 0x03, 'A', 0x00 }), 100));
        REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 1) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"A");
        const Bytes f = readFile(q);
        const size_t i = findBytes(f, bytesOf("TPE1"));
        REQUIRE(i != static_cast<size_t>(-1));
        // only one value: encoding $01, BOM, 'A'; the BOM's FF is followed by FE (>= 0xE0), so unsynchronisation adds one
        // more byte than the plain content (encoding 1 + BOM 2 + 'A' 2 = 5) would otherwise need
        CHECK(f[i + 7] == 0x06);   // frame size: encoding (1) + unsynchronised BOM (3) + 'A' (2)
        CHECK(findBytes(f, Bytes{ 0xFF, 0xFE, 'A', 0x00, 0x00, 0x00, 0xFF, 0xFE }) == static_cast<size_t>(-1));
    }
}

TEST_CASE("ID3v2: frames with the grouping flag", "[id3v2][spec][flags]")
{
    SECTION("v2.4: group byte in front of the data") {
        const Bytes data = { 0x80, 0x03, 'T', 'i', 't', 'l', 'e' };
        auto p = writeTagged("spec_group24.mp3", tagBytes(4, 0, frame("TIT2", data, 0x0040), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Title");
        saveAsV24(3);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("TIT2"));
        REQUIRE(i != static_cast<size_t>(-1));
        CHECK(f[i + 8] == 0x00);
        CHECK(f[i + 9] == 0x40);                                            // flag is kept
        CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 10 + static_cast<std::ptrdiff_t>(data.size())) == data);   // with the group byte
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Title");
    }
    SECTION("v2.3: group byte in front of the data") {
        const Bytes data = { 0x80, 0x00, 'T', 'i', 't', 'l', 'e' };
        auto p = writeTagged("spec_group23.mp3", tagBytes(3, 0, frame("TIT2", data, 0x0020, 3), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Title");
    }
}

TEST_CASE("ID3v2: extended header is skipped and removed", "[id3v2][spec][header]")
{
    SECTION("v2.4, 6 bytes") {
        Bytes body = { 0x00, 0x00, 0x00, 0x06, 0x01, 0x00 };
        put(body, frame("TIT2", { 0x03, 'E', 'x', 't' }));
        auto p = writeTagged("spec_ext24.mp3", tagBytes(4, 0x40, body, 0));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Ext");
        saveAsV24(3);
        CHECK(audioIntact(p));
        CHECK(readFile(p)[5] == 0);   // no extended header in the new tag
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Ext");
    }
    SECTION("v2.4, 12 bytes with a CRC") {
        // size (4), number of flag bytes, flags ($20 = CRC present), data length $05, CRC (5 bytes)
        Bytes body = { 0x00, 0x00, 0x00, 0x0C, 0x01, 0x20, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 };
        put(body, frame("TIT2", { 0x03, 'C', 'r', 'c' }));
        auto p = writeTagged("spec_ext24_crc.mp3", tagBytes(4, 0x40, body, 0));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Crc");
        saveAsV24(3);
        CHECK(audioIntact(p));
    }
    SECTION("v2.3, size field without its own 4 bytes") {
        Bytes body = { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };   // size 6: flags (2) + padding size (4)
        put(body, frame("TIT2", { 0x00, 'E', 'x', 't' }, 0, 3));
        auto p = writeTagged("spec_ext23.mp3", tagBytes(3, 0x40, body, 0));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Ext");
        saveAsV24(3);
        CHECK(audioIntact(p));
    }
}

TEST_CASE("ID3v2.4: footer is part of the tag and removed when the tag is rewritten", "[id3v2][spec][header]")
{
    const Bytes body = frame("TIT2", { 0x03, 'F', 'o', 'o' });
    Bytes footer;
    put(footer, "3DI");
    footer.push_back(4);
    footer.push_back(0);
    footer.push_back(0x10);
    put(footer, synchsafe(static_cast<uint32_t>(body.size())));
    auto p = writeTagged("spec_footer.mp3", tagBytes(4, 0x10, body, 0, footer));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Foo");
    CHECK(ID3V2GetSizeW() == static_cast<long>(10 + body.size() + 10));   // header + frames + footer
    saveAsV24(3);
    CHECK(audioIntact(p));
    CHECK(findBytes(readFile(p), bytesOf("3DI")) == static_cast<size_t>(-1));
}

TEST_CASE("ID3v2: text frames the DLL does not know and foreign frame IDs", "[id3v2][spec][text]")
{
    SECTION("an unknown text frame is read as text") {
        auto p = writeTagged("spec_unknown_t.mp3", tagBytes(4, 0, frame("TXYZ", { 0x03, 'a', 'b' }), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(frameId("TXYZ"))) == L"ab");
    }
    SECTION("the text of a frame that is not a text frame is empty") {
        Bytes apic = { 0x00 };
        put(apic, "image/png");
        apic.push_back(0);
        apic.push_back(3);
        apic.push_back(0);
        apic.push_back(0x89);
        auto p = writeTagged("spec_foreign.mp3", tagBytes(4, 0, frame("APIC", apic), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2GetFrameCountW(ID3F_APIC) == 1);
        CHECK(take(ID3V2GetTextFrameW(ID3F_APIC)).empty());
    }
}

TEST_CASE("ID3v2: frame flags and additional header fields are kept and converted", "[id3v2][spec][flags]")
{
    SECTION("v2.4: a frame with the unsynchronisation flag is resynchronised, then re-unsynchronised on write since its decoded content still needs it") {
        Bytes d = bytesOf("own");
        d.push_back(0);
        for (uint8_t b : { 0xFF, 0x00, 0x00, 0x12 }) d.push_back(b);   // unsynchronised form of FF 00 12
        auto p = writeTagged("spec_unsync.mp3", tagBytes(4, 0, frame("PRIV", d, 0x0002), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        unsigned char buf[16] = {};
        REQUIRE(ID3V2GetPrivateFrameDataW(buf, 16, 1) == 3);
        CHECK((buf[0] == 0xFF && buf[1] == 0x00 && buf[2] == 0x12));
        saveAsV24(3);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("PRIV"));
        REQUIRE(i != static_cast<size_t>(-1));
        // the decoded payload (FF 00 12) has FF directly followed by 00, one of the byte pairs unsynchronisation exists to
        // protect (it would otherwise be ambiguous with the stuffing marker itself): the flag is set again on write, and
        // the bytes on disk are the same unsynchronised form as the ones this test started from
        CHECK(f[i + 9] == 0x02);
        CHECK(findBytes(f, Bytes{ 'o', 'w', 'n', 0x00, 0xFF, 0x00, 0x00, 0x12 }) != static_cast<size_t>(-1));
    }
    SECTION("v2.3 to v2.4: status flags and group byte") {
        // v2.3: %abc00000 %ijk00000: a (discard on tag alter) $8000, b $4000, c (read only) $2000, k (grouping) $0020
        const Bytes data = { 0x80, 0x00, 'T', 'i', 't', 'l', 'e' };
        auto p = writeTagged("spec_flags_23_24.mp3", tagBytes(3, 0, frame("TIT2", data, 0xE020, 3), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 0) != 0);
        ID3V2SetTextFrameW(ID3F_TPE1, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("TIT2"));
        REQUIRE(i != static_cast<size_t>(-1));
        // v2.4: %0abc0000 %0h00kmnp: a $4000, b $2000, c $1000, h (grouping) $0040
        CHECK(((f[i + 8] << 8) | f[i + 9]) == 0x7040);
        CHECK(f[i + 10] == 0x80);   // group byte first, then the data
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Title");
    }
    SECTION("v2.4 to v2.3: status flags and group byte") {
        const Bytes data = { 0x80, 0x03, 'T', 'i', 't', 'l', 'e' };
        auto p = writeTagged("spec_flags_24_23.mp3", tagBytes(4, 0, frame("TIT2", data, 0x7040), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(2, 1) != 0);
        ID3V2SetTextFrameW(ID3F_TPE1, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("TIT2"));
        REQUIRE(i != static_cast<size_t>(-1));
        CHECK(((f[i + 8] << 8) | f[i + 9]) == 0xE020);
        CHECK(f[i + 10] == 0x80);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Title");
    }
    SECTION("v2.4: an encrypted frame keeps its method byte and data length") {
        // flags: encryption $0004 and data length indicator $0001; method $81, length 5 (synchsafe), then the (encrypted) data
        const Bytes data = { 0x81, 0x00, 0x00, 0x00, 0x05, 0x11, 0x22, 0x33, 0x44, 0x55 };
        auto p = writeTagged("spec_encrypted.mp3", tagBytes(4, 0, frame("PRIV", data, 0x0005), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        saveAsV24(3);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("PRIV"));
        REQUIRE(i != static_cast<size_t>(-1));
        CHECK(f[i + 9] == 0x05);
        CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 10 + static_cast<std::ptrdiff_t>(data.size())) == data);
    }
}

// ---------------------------------------------------------------- conversion between the tag versions

TEST_CASE("ID3v2: dates are converted between the tag versions", "[id3v2][spec][convert]")
{
    SECTION("v2.3 to v2.4: TYER, TDAT, TIME and TORY become TDRC and TDOR") {
        Bytes body = frame("TYER", { 0x00, '1', '9', '9', '9' }, 0, 3);
        put(body, frame("TDAT", { 0x00, '1', '7', '0', '5' }, 0, 3));   // DDMM
        put(body, frame("TIME", { 0x00, '2', '1', '3', '0' }, 0, 3));   // HHMM
        put(body, frame("TORY", { 0x00, '1', '9', '9', '8' }, 0, 3));
        auto p = writeTagged("spec_dates_23_24.mp3", tagBytes(3, 0, body, 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        saveAsV24(3);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TDRC)) == L"1999-05-17T21:30");
        CHECK(take(ID3V2GetTextFrameW(ID3F_TDOR)) == L"1998");
        CHECK(ID3V2GetFrameCountW(ID3F_TYER) == 0);
        CHECK(ID3V2GetFrameCountW(ID3F_TDAT) == 0);
        CHECK(ID3V2GetFrameCountW(ID3F_TIME) == 0);
        CHECK(ID3V2GetFrameCountW(ID3F_TORY) == 0);
    }
    SECTION("v2.4 to v2.3: TDRC and TDOR become TYER, TDAT, TIME and TORY") {
        Bytes body = frame("TDRC", { 0x03, '1', '9', '9', '9', '-', '0', '5', '-', '1', '7', 'T', '2', '1', ':', '3', '0' });
        put(body, frame("TDOR", { 0x03, '1', '9', '9', '8' }));
        auto p = writeTagged("spec_dates_24_23.mp3", tagBytes(4, 0, body, 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(2, 1) != 0);
        ID3V2SetTextFrameW(ID3F_TPE1, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TYER)) == L"1999");
        CHECK(take(ID3V2GetTextFrameW(ID3F_TDAT)) == L"1705");
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIME)) == L"2130");
        CHECK(take(ID3V2GetTextFrameW(ID3F_TORY)) == L"1998");
    }
    SECTION("v2.4 to v2.3: a year alone gives TYER only") {
        auto p = writeTagged("spec_dates_year.mp3", tagBytes(4, 0, frame("TDRC", { 0x03, '2', '0', '0', '1' }), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(2, 0) != 0);
        ID3V2SetTextFrameW(ID3F_TPE1, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TYER)) == L"2001");
        CHECK(ID3V2GetFrameCountW(ID3F_TDAT) == 0);
        CHECK(ID3V2GetFrameCountW(ID3F_TIME) == 0);
    }
}

TEST_CASE("ID3v2: volume adjustment and equalisation are not written in the layout of another version", "[id3v2][spec][convert]")
{
    const Bytes rvad = { 0x03, 0x10, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };   // v2.3 layout
    const Bytes equa = { 0x10, 0x00, 0x64, 0x01, 0x00 };
    Bytes rva2 = bytesOf("track");                                                         // v2.4 layout
    rva2.push_back(0);
    for (uint8_t b : { 0x01, 0x04, 0x00, 0x00 }) rva2.push_back(b);
    Bytes equ2 = { 0x01 };
    put(equ2, "eq");
    equ2.push_back(0);
    for (uint8_t b : { 0x00, 0x64, 0x04, 0x00 }) equ2.push_back(b);

    SECTION("v2.3 to v2.4: dropped") {
        Bytes body = frame("RVAD", rvad, 0, 3);
        put(body, frame("EQUA", equa, 0, 3));
        put(body, frame("TIT2", { 0x00, 'k', 'e', 'e', 'p' }, 0, 3));
        auto p = writeTagged("spec_rva_23_24.mp3", tagBytes(3, 0, body, 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        saveAsV24(3);
        const Bytes f = readFile(p);
        CHECK(findBytes(f, bytesOf("RVA2")) == static_cast<size_t>(-1));
        CHECK(findBytes(f, bytesOf("RVAD")) == static_cast<size_t>(-1));
        CHECK(findBytes(f, bytesOf("EQU2")) == static_cast<size_t>(-1));
        CHECK(findBytes(f, bytesOf("EQUA")) == static_cast<size_t>(-1));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"keep");
    }
    SECTION("v2.3 to v2.3: kept unchanged") {
        Bytes body = frame("RVAD", rvad, 0, 3);
        put(body, frame("EQUA", equa, 0, 3));
        auto p = writeTagged("spec_rva_23_23.mp3", tagBytes(3, 0, body, 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(2, 1) != 0);
        ID3V2SetTextFrameW(ID3F_TPE1, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes f = readFile(p);
        CHECK(findBytes(f, rvad) != static_cast<size_t>(-1));
        CHECK(findBytes(f, equa) != static_cast<size_t>(-1));
    }
    SECTION("v2.4 to v2.3: dropped, v2.4 to v2.4: kept") {
        Bytes body = frame("RVA2", rva2);
        put(body, frame("EQU2", equ2));
        auto p = writeTagged("spec_rva_24.mp3", tagBytes(4, 0, body, 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        saveAsV24(3);
        Bytes f = readFile(p);
        CHECK(findBytes(f, rva2) != static_cast<size_t>(-1));
        CHECK(findBytes(f, equ2) != static_cast<size_t>(-1));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(2, 1) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"y");
        REQUIRE(ID3V2SaveChangesW() != 0);
        f = readFile(p);
        CHECK(findBytes(f, bytesOf("RVAD")) == static_cast<size_t>(-1));
        CHECK(findBytes(f, bytesOf("EQUA")) == static_cast<size_t>(-1));
    }
}

// ---------------------------------------------------------------- text encodings

namespace {

// sets the code page for ISO-8859-1 / ANSI strings (configuration key 7), restores the default afterwards
struct CodePage {
    explicit CodePage(long codePage) { SetConfigValueW(7, codePage); }
    ~CodePage() { SetConfigValueW(7, 1252); }
};

}  // namespace

TEST_CASE("ID3v2: text that ISO-8859-1 cannot represent is stored as Unicode", "[id3v2][spec][encoding]")
{
    auto p = writeTagged("spec_lossy.mp3", tagBytes(4, 0, frame("TIT2", { 0x03, 'x' }), 100));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2SetFormatAndEncodingW(3, 0) != 0);
    ID3V2SetTextFrameW(ID3F_TIT2, L"\x041F\x0440\x0438\x0432\x0435\x0442 \x20AC");
    ID3V2SetTextFrameW(ID3F_TALB, L"Gr\x00FC\x00DF" L"e");   // fits into ISO-8859-1
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes f = readFile(p);
    const size_t title = findBytes(f, bytesOf("TIT2"));
    const size_t album = findBytes(f, bytesOf("TALB"));
    REQUIRE(title != static_cast<size_t>(-1));
    REQUIRE(album != static_cast<size_t>(-1));
    CHECK(f[title + 10] != 0x00);   // not ISO-8859-1
    CHECK(f[album + 10] == 0x00);   // stays ISO-8859-1
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"\x041F\x0440\x0438\x0432\x0435\x0442 \x20AC");
    CHECK(take(ID3V2GetTextFrameW(ID3F_TALB)) == L"Gr\x00FC\x00DF" L"e");
}

TEST_CASE("ID3v2: encoding $00 uses the configured code page", "[id3v2][spec][encoding]")
{
    auto p = writeTagged("spec_codepage.mp3", tagBytes(4, 0, frame("TIT2", { 0x00, 0xE4, 0x93 }), 100));
    SECTION("default: Windows-1252, independent of the system settings") {
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        const std::wstring t = take(ID3V2GetTextFrameW(ID3F_TIT2));
        REQUIRE(t.size() == 2);
        CHECK(t[0] == 0x00E4);
        CHECK(t[1] == 0x201C);
    }
    SECTION("28591: strict ISO-8859-1") {
        CodePage cp(28591);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        const std::wstring t = take(ID3V2GetTextFrameW(ID3F_TIT2));
        REQUIRE(t.size() == 2);
        CHECK(t[0] == 0x00E4);
        CHECK(t[1] == 0x0093);
        REQUIRE(ID3V2SetFormatAndEncodingW(3, 0) != 0);
        ID3V2SetTextFrameW(ID3F_TIT2, L"\x00E4");
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("TIT2"));
        REQUIRE(i != static_cast<size_t>(-1));
        CHECK(f[i + 10] == 0x00);
        CHECK(f[i + 11] == 0xE4);
    }
    SECTION("1251: Cyrillic") {
        CodePage cp(1251);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        const std::wstring t = take(ID3V2GetTextFrameW(ID3F_TIT2));
        REQUIRE(!t.empty());
        CHECK(t[0] == 0x0434);
    }
    SECTION("0: the code page of the system") {
        CodePage cp(0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)).size() == 2);
    }
}

TEST_CASE("ID3v2: genre (TCON) in all notations", "[id3v2][spec][text]")
{
    struct Form { const char* tcon; const wchar_t* genre; };
    const Form forms[] = {
        { "(21)", L"Ska" }, { "21", L"Ska" }, { "(RX)", L"Remix" }, { "(CR)", L"Cover" }, { "(21)Custom", L"Custom" },
        { "(21)(22)", L"Ska" }, { "((Text", L"(Text" }, { "Eurodisco", L"Eurodisco" }, { "", L"" },
    };
    for (const Form& f : forms) {
        DYNAMIC_SECTION("TCON " << f.tcon) {
            Bytes d = { 0x00 };
            put(d, f.tcon);
            auto p = writeTagged("spec_tcon.mp3", tagBytes(4, 0, frame("TCON", d), 100));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            CHECK(take(AUDIOGetGenreW()) == f.genre);
        }
    }
}

// ---------------------------------------------------------------- counters, buffer size, language, linked pictures

TEST_CASE("ID3v2: counters of any length and optional fields", "[id3v2][spec][frames]")
{
    SECTION("PCNT longer than four bytes") {
        auto p = writeTagged("spec_pcnt.mp3", tagBytes(4, 0, frame("PCNT", { 0x00, 0x00, 0x00, 0x00, 0x00, 0x07 }), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(ID3V2GetPlayCounterW() == 7);
    }
    SECTION("POPM without a counter") {
        Bytes d = bytesOf("a@b.c");
        d.push_back(0);
        d.push_back(200);
        auto p = writeTagged("spec_popm0.mp3", tagBytes(4, 0, frame("POPM", d), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(ID3V2GetPopularimeterRatingW(1) == 200);
        CHECK(ID3V2GetPopularimeterCounterW(1) == 0);
    }
    SECTION("POPM with a five byte counter") {
        Bytes d = bytesOf("a@b.c");
        d.push_back(0);
        d.push_back(100);
        for (uint8_t b : { 0x00, 0x00, 0x00, 0x00, 0x09 }) d.push_back(b);
        auto p = writeTagged("spec_popm5.mp3", tagBytes(4, 0, frame("POPM", d), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(ID3V2GetPopularimeterCounterW(1) == 9);
    }
    SECTION("RBUF without the offset to the next tag") {
        auto p = writeTagged("spec_rbuf.mp3", tagBytes(4, 0, frame("RBUF", { 0x00, 0x10, 0x00, 0x01 }), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(ID3V2GetRecommendedBufferSizeValueW() == 4096);
        CHECK(ID3V2GetRecommendedBufferSizeFlagW() == 1);
        CHECK(ID3V2GetRecommendedBufferSizeOffsetW() == 0);
    }
}

TEST_CASE("ID3v2: an unknown language is written as XXX", "[id3v2][spec][frames]")
{
    auto p = writeTagged("spec_lang.mp3", tagBytes(4, 0, frame("TIT2", { 0x03, 'x' }), 100));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2SetFormatAndEncodingW(3, 3) != 0);
    ID3V2AddCommentW(L"", L"d", L"text");
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes f = readFile(p);
    const size_t i = findBytes(f, bytesOf("COMM"));
    REQUIRE(i != static_cast<size_t>(-1));
    CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 11, f.begin() + static_cast<std::ptrdiff_t>(i) + 14) == bytesOf("XXX"));
}

TEST_CASE("ID3v2: linked pictures are not read from disk unless configured", "[id3v2][spec][frames]")
{
    auto pic = writeTemp("spec_linked_picture.bin", Bytes(10, 0x42));
    Bytes apic = { 0x00 };
    put(apic, "-->");
    apic.push_back(0);
    apic.push_back(3);
    apic.push_back(0);
    put(apic, pic.string().c_str());
    auto p = writeTagged("spec_link.mp3", tagBytes(4, 0, frame("APIC", apic), 100));
    SECTION("default") {
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2GetFrameCountW(ID3F_APIC) == 1);
        CHECK(ID3V2GetPictureSizeW(1) == 0);
    }
    SECTION("configured") {
        SetConfigValueW(8, 1);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(ID3V2GetPictureSizeW(1) == 10);
        SetConfigValueW(8, 0);
    }
    SECTION("configured: a network path or a file larger than 64 MB is not loaded") {
        const fs::path big = writeTemp("spec_linked_big.bin", Bytes(1, 0));
        fs::resize_file(big, 64ull * 1024 * 1024 + 1);
        SetConfigValueW(8, 1);
        for (const std::string& link : { std::string("\\\\gibt.es.nicht.example\\share\\cover.jpg"), "\\\\?\\" + pic.string(), big.string() }) {
            INFO(link);
            Bytes a = { 0x00 };
            put(a, "-->");
            a.push_back(0);
            a.push_back(3);
            a.push_back(0);
            put(a, link.c_str());
            auto q = writeTagged("spec_link.mp3", tagBytes(4, 0, frame("APIC", a), 100));
            CHECK(AUDIOAnalyzeFileW(q.c_str()) == MPEG);
            CHECK(ID3V2GetPictureSizeW(1) == 0);
        }
        SetConfigValueW(8, 0);
        fs::remove(big);
    }
}

// ---------------------------------------------------------------- frames the DLL does not know

TEST_CASE("ID3v2: unknown frames are preserved when the tag is saved", "[id3v2][spec][flags]")
{
    const Bytes payload = { 0x01, 0x02, 0x03, 0x04 };
    SECTION("v2.4 to v2.4") {
        Bytes body = frame("XABC", payload);
        put(body, frame("TXYZ", { 0x03, 'a', 'b' }));
        auto p = writeTagged("spec_unknown_keep.mp3", tagBytes(4, 0, body, 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        saveAsV24(3);
        const Bytes f = readFile(p);
        const size_t i = findBytes(f, bytesOf("XABC"));
        REQUIRE(i != static_cast<size_t>(-1));
        CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 14) == payload);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(frameId("TXYZ"))) == L"ab");
    }
    SECTION("v2.4 to v2.3 and back") {
        auto p = writeTagged("spec_unknown_23.mp3", tagBytes(4, 0, frame("XABC", payload), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(2, 1) != 0);
        ID3V2SetTextFrameW(ID3F_TPE1, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        CHECK(findBytes(readFile(p), bytesOf("XABC")) != static_cast<size_t>(-1));
    }
    SECTION("a frame with the flag 'discard if the tag is altered' is dropped") {
        auto p = writeTagged("spec_unknown_drop.mp3", tagBytes(4, 0, frame("XABC", payload, 0x4000), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        saveAsV24(3);
        CHECK(findBytes(readFile(p), bytesOf("XABC")) == static_cast<size_t>(-1));
    }
    SECTION("v2.2 tags cannot hold a four character frame ID") {
        auto p = writeTagged("spec_unknown_22.mp3", tagBytes(4, 0, frame("XABC", payload), 100));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(1, 0) != 0);
        ID3V2SetTextFrameW(ID3F_TPE1, L"x");
        REQUIRE(ID3V2SaveChangesW() != 0);
        CHECK(findBytes(readFile(p), bytesOf("XABC")) == static_cast<size_t>(-1));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"x");
    }
}


// ---- ID3v2.2 and ID3v2.3 (id3v2-00 and id3v2.3.0) ----

namespace {

// v2.2 frame: three character ID, three byte size
Bytes frame22(const char* id, const Bytes& data)
{
    Bytes f;
    put(f, id);
    f.push_back(static_cast<uint8_t>(data.size() >> 16));
    f.push_back(static_cast<uint8_t>(data.size() >> 8));
    f.push_back(static_cast<uint8_t>(data.size()));
    put(f, data);
    return f;
}

// unsynchronisation scheme: $FF is followed by $00 if the next byte is $00 or a sync ($E0 and above), and at the end of the data
Bytes unsynchronise(const Bytes& in)
{
    Bytes out;
    for (size_t i = 0; i < in.size(); i++) {
        out.push_back(in[i]);
        if (in[i] == 0xFF && (i + 1 == in.size() || in[i + 1] == 0x00 || in[i + 1] >= 0xE0))
            out.push_back(0x00);
    }
    return out;
}

}  // namespace

TEST_CASE("ID3v2.2 and v2.3: unsynchronisation of the whole tag", "[id3v2][spec][header]")
{
    // frame sizes are those of the data before unsynchronisation, the tag size is the size after it
    const Bytes title = { 0x00, 'a', 0xFF, 0xE0, 0xFF, 0xFF, 0xE1, 'z' };
    const Bytes artist = { 0x00, 'B' };
    SECTION("v2.3") {
        Bytes body = frame("TIT2", title, 0, 3);
        put(body, frame("TPE1", artist, 0, 3));
        auto p = writeTagged("spec_unsync23.mp3", tagBytes(3, 0x80, unsynchronise(body), 20));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"a\xFF\xE0\xFF\xFF\xE1z");
        CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"B");
    }
    SECTION("v2.2") {
        Bytes body = frame22("TT2", title);
        put(body, frame22("TP1", artist));
        auto p = writeTagged("spec_unsync22.mp3", tagBytes(2, 0x80, unsynchronise(body), 20));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"a\xFF\xE0\xFF\xFF\xE1z");
        CHECK(take(ID3V2GetTextFrameW(ID3F_TPE1)) == L"B");
    }
}

TEST_CASE("ID3v2.2: a tag with the compression bit is ignored", "[id3v2][spec][header]")
{
    auto p = writeTagged("spec_compr22.mp3", tagBytes(2, 0x40, frame22("TT2", { 0x00, 'X' }), 20));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)).empty());
}

TEST_CASE("ID3v2.2: frames are converted into the IDs of v2.3 and v2.4", "[id3v2][spec][convert]")
{
    Bytes body = frame22("TT2", { 0x00, 'T' });
    put(body, frame22("TYE", { 0x00, '1', '9', '9', '9' }));
    put(body, frame22("IPL", { 0x00, 'g', 0x00, 'p', 0x00 }));
    put(body, frame22("CNT", { 0x00, 0x00, 0x00, 0x07 }));
    put(body, frame22("POP", { 'a', '@', 'b', 0x00, 0x80, 0x00, 0x00, 0x00, 0x03 }));
    put(body, frame22("PIC", { 0x00, 'J', 'P', 'G', 0x03, 0x00, 0xFF, 0xD8, 0xFF, 0xE0, 0x01, 0x02 }));
    auto p = writeTagged("spec_conv22.mp3", tagBytes(2, 0, body, 50));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2SetFormatAndEncodingW(2, 0) != 0);
    ID3V2SetTextFrameW(ID3F_TPE1, L"x");
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes f = readFile(p);
    for (const char* id : { "TIT2", "TYER", "IPLS", "PCNT", "POPM", "APIC" })
        CHECK(findBytes(f, bytesOf(id)) != static_cast<size_t>(-1));
    CHECK(findBytes(f, bytesOf("image/jpeg")) != static_cast<size_t>(-1));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"T");
    CHECK(take(ID3V2GetPictureMimeW(1)) == L"image/jpeg");
    // and back to v2.2
    REQUIRE(ID3V2SetFormatAndEncodingW(1, 0) != 0);
    ID3V2SetTextFrameW(ID3F_TPE1, L"y");
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes g = readFile(p);
    for (const char* id : { "TT2", "TYE", "IPL", "CNT", "POP", "PIC" })
        CHECK(findBytes(g, bytesOf(id)) != static_cast<size_t>(-1));
    CHECK(audioIntact(p));
}

TEST_CASE("ID3v2.2 and v2.3: only the encodings $00 and $01 exist", "[id3v2][spec][encoding]")
{
    CHECK(ID3V2SetFormatAndEncodingW(1, 2) == 0);   // v2.2 with UTF-16BE
    CHECK(ID3V2SetFormatAndEncodingW(2, 2) == 0);   // v2.3 with UTF-16BE
    CHECK(ID3V2SetFormatAndEncodingW(1, 3) == 0);
    CHECK(ID3V2SetFormatAndEncodingW(2, 3) == 0);
    CHECK(ID3V2SetFormatAndEncodingW(3, 2) != 0);   // v2.4 accepts it
    CHECK(ID3V2SetFormatAndEncodingW(2, 1) != 0);
}

TEST_CASE("ID3v2.2 and v2.3: linked information has a three or four character frame ID", "[id3v2][spec][convert]")
{
    Bytes lnk = { 'T', 'T', '2' };
    put(lnk, "http://x/a.mp3");
    lnk.push_back(0);
    auto p = writeTagged("spec_link22.mp3", tagBytes(2, 0, frame22("LNK", lnk), 50));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2SetFormatAndEncodingW(2, 0) != 0);
    ID3V2SetTextFrameW(ID3F_TPE1, L"x");
    REQUIRE(ID3V2SaveChangesW() != 0);
    Bytes f = readFile(p);
    size_t i = findBytes(f, bytesOf("LINK"));
    REQUIRE(i != static_cast<size_t>(-1));
    CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 14) == bytesOf("TIT2"));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2SetFormatAndEncodingW(1, 0) != 0);
    ID3V2SetTextFrameW(ID3F_TPE1, L"y");
    REQUIRE(ID3V2SaveChangesW() != 0);
    f = readFile(p);
    i = findBytes(f, bytesOf("LNK"));
    REQUIRE(i != static_cast<size_t>(-1));
    CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 6, f.begin() + static_cast<std::ptrdiff_t>(i) + 9) == bytesOf("TT2"));
}

// zlib.compress(b"\x00Hello World", 9) (encoding byte 0 = ISO-8859-1, then the text): 2 byte zlib header, DEFLATE data,
// 4 byte Adler-32 of the plain 12 bytes (0x180c041d, the last 4 bytes here) - verified against Python's zlib module.
static const Bytes kCompressedHelloWorld = { 0x78, 0xda, 0x63, 0xf0, 0x48, 0xcd, 0xc9, 0xc9, 0x57, 0x08, 0xcf, 0x2f, 0xca, 0x49,
                                              0x01, 0x00, 0x18, 0x0c, 0x04, 0x1d };

TEST_CASE("ID3v2.3: a compressed TIT2 frame (zlib/DEFLATE) is decompressed", "[id3v2][spec][compression]")
{
    // v2.3: a compressed frame unconditionally starts with the plain (uncompressed) size, then the zlib stream
    Bytes body = be32(12);
    put(body, kCompressedHelloWorld);
    auto p = writeTagged("spec_compressed_v23.mp3", tagBytes(3, 0, frame("TIT2", body, 0x0080, 3), 0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Hello World");
}

TEST_CASE("ID3v2.4: a compressed TIT2 frame with a data length indicator is decompressed", "[id3v2][spec][compression]")
{
    // flags 0x08 (compressed) | 0x01 (data length indicator): the synchsafe plain size comes first, then the zlib stream
    Bytes body = synchsafe(12);
    put(body, kCompressedHelloWorld);
    auto p = writeTagged("spec_compressed_v24_len.mp3", tagBytes(4, 0, frame("TIT2", body, 0x0009, 4), 0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Hello World");
}

TEST_CASE("ID3v2.4: a compressed TIT2 frame without a data length indicator still decompresses", "[id3v2][spec][compression]")
{
    // flags 0x08 only: no size field at all, the frame content is directly the zlib stream; the plain size is not known
    // ahead of time and has to be discovered with puff()'s output-less scanning mode
    auto p = writeTagged("spec_compressed_v24_nolen.mp3", tagBytes(4, 0, frame("TIT2", kCompressedHelloWorld, 0x0008, 4), 0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"Hello World");
}

TEST_CASE("ID3v2.3: a compressed frame with a corrupted DEFLATE stream does not crash and yields no text", "[id3v2][spec][compression]")
{
    Bytes corrupted = kCompressedHelloWorld;
    corrupted[10] ^= 0xFF;   // flip a byte in the middle of the DEFLATE data itself
    Bytes body = be32(12);
    put(body, corrupted);
    auto p = writeTagged("spec_compressed_corrupt.mp3", tagBytes(3, 0, frame("TIT2", body, 0x0080, 3), 0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) != L"Hello World");
}

TEST_CASE("ID3v2.3: a compressed frame with a wrong Adler-32 checksum is rejected, not silently accepted", "[id3v2][spec][compression]")
{
    Bytes corrupted = kCompressedHelloWorld;
    corrupted.back() ^= 0xFF;   // flip a byte of the trailing Adler-32 only; the DEFLATE data itself stays valid
    Bytes body = be32(12);
    put(body, corrupted);
    auto p = writeTagged("spec_compressed_badsum.mp3", tagBytes(3, 0, frame("TIT2", body, 0x0080, 3), 0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) != L"Hello World");
}

TEST_CASE("ID3v2.3: a compressed and encrypted frame is left alone (unknown encryption method)", "[id3v2][spec][compression]")
{
    // flags 0x80 (compressed) | 0x40 (encrypted): decompression must not even be attempted, since the bytes are not a
    // zlib stream yet (they would need decrypting first, with a vendor specific method this library does not know).
    // v2.3 additional header field order: [decompressed size, 4 bytes][encryption method, 1 byte][group, 1 byte]
    Bytes body = be32(12);
    body.push_back(0x2A);          // encryption method byte
    put(body, kCompressedHelloWorld);
    auto p = writeTagged("spec_compressed_encrypted.mp3", tagBytes(3, 0, frame("TIT2", body, 0x00C0, 3), 0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) != L"Hello World");
}

TEST_CASE("ID3v2.4: writing a frame whose content looks like a frame sync applies unsynchronisation", "[id3v2][spec][unsync]")
{
    // ISO-8859-1 0xFF followed by 0xE0 or higher is exactly the byte pair a naive decoder could mistake for an MPEG
    // frame sync; the encoding byte (0x00) in front of it is untouched (not 0xFF), the plain content is 3 bytes:
    // 00 FF E0. after unsynchronisation it must be 00 FF 00 E0 (a 0x00 inserted right after the FF).
    Session s(Cfg{ 3, 0, "v2.4 ISO-8859-1" }, "unsync_v24.mp3");
    ID3V2SetTextFrameW(ID3F_TIT2, L"ÿà");
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes f = readFile(s.path);
    const size_t i = findBytes(f, bytesOf("TIT2"));
    REQUIRE(i != static_cast<size_t>(-1));
    // frame header: 4 byte ID, 4 byte synchsafe size, 2 byte flags
    const Bytes sizeBytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 4, f.begin() + static_cast<std::ptrdiff_t>(i) + 8);
    const uint32_t size = ((sizeBytes[0] & 0x7Fu) << 21) | ((sizeBytes[1] & 0x7Fu) << 14) | ((sizeBytes[2] & 0x7Fu) << 7) | (sizeBytes[3] & 0x7Fu);
    CHECK(size == 4);
    const uint16_t flagBits = static_cast<uint16_t>((f[i + 8] << 8) | f[i + 9]);
    CHECK((flagBits & 0x0002) != 0);   // n - unsynchronisation
    const Bytes payload(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 10 + 4);
    CHECK(payload == Bytes{ 0x00, 0xFF, 0x00, 0xE0 });

    REQUIRE(AUDIOAnalyzeFileW(s.path.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"ÿà");   // round trip: the inserted 0x00 was correctly removed again
}

TEST_CASE("ID3v2.4: writing an ordinary frame does not apply unsynchronisation", "[id3v2][spec][unsync]")
{
    Session s(Cfg{ 3, 0, "v2.4 ISO-8859-1" }, "unsync_v24_plain.mp3");
    ID3V2SetTextFrameW(ID3F_TIT2, L"Ordinary Title");
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes f = readFile(s.path);
    const size_t i = findBytes(f, bytesOf("TIT2"));
    REQUIRE(i != static_cast<size_t>(-1));
    const uint16_t flagBits = static_cast<uint16_t>((f[i + 8] << 8) | f[i + 9]);
    CHECK((flagBits & 0x0002) == 0);   // nothing in "Ordinary Title" ever looks like a frame sync: not flagged, not grown
}

TEST_CASE("ID3v2.3: writing a tag whose frame data looks like a frame sync applies whole-tag unsynchronisation", "[id3v2][spec][unsync]")
{
    // v2.3 unsynchronisation is a tag level flag (byte 5 of the 10 byte header, bit 0x80), applied to the concatenated
    // bytes of every frame together, not per frame (see CID3V2::SaveTag).
    Session s(Cfg{ 2, 0, "v2.3 ISO-8859-1" }, "unsync_v23.mp3");
    ID3V2SetTextFrameW(ID3F_TIT2, L"ÿà");
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes f = readFile(s.path);
    REQUIRE(std::memcmp(f.data(), "ID3", 3) == 0);
    CHECK((f[5] & 0x80) != 0);   // tag header flags: unsynchronisation bit set
    const size_t i = findBytes(f, bytesOf("TIT2"));
    REQUIRE(i != static_cast<size_t>(-1));
    // v2.3 frame size fields stay the plain (pre-stuffing) size: the comment in ID3V2::ReadFromFile documents this
    const Bytes sizeBytes(f.begin() + static_cast<std::ptrdiff_t>(i) + 4, f.begin() + static_cast<std::ptrdiff_t>(i) + 8);
    const uint32_t size = (sizeBytes[0] << 24) | (sizeBytes[1] << 16) | (sizeBytes[2] << 8) | sizeBytes[3];
    CHECK(size == 3);   // 00 FF E0, plain, before the whole-tag stuffing pass adds the extra 0x00
    const Bytes payload(f.begin() + static_cast<std::ptrdiff_t>(i) + 10, f.begin() + static_cast<std::ptrdiff_t>(i) + 10 + 4);
    CHECK(payload == Bytes{ 0x00, 0xFF, 0x00, 0xE0 });   // but the bytes actually on disk are stuffed

    REQUIRE(AUDIOAnalyzeFileW(s.path.c_str()) == MPEG);
    CHECK(take(ID3V2GetTextFrameW(ID3F_TIT2)) == L"ÿà");
}

TEST_CASE("ID3v2.3: writing an ordinary tag does not set the unsynchronisation flag", "[id3v2][spec][unsync]")
{
    Session s(Cfg{ 2, 0, "v2.3 ISO-8859-1" }, "unsync_v23_plain.mp3");
    ID3V2SetTextFrameW(ID3F_TIT2, L"Ordinary Title");
    REQUIRE(ID3V2SaveChangesW() != 0);
    const Bytes f = readFile(s.path);
    CHECK((f[5] & 0x80) == 0);
}

TEST_CASE("ID3v2.3: 3 character v2.2 frame IDs padded with a zero byte are read", "[id3v2][spec][header]")
{
    // written by old iTunes versions: a v2.3 header and 10 byte frame headers, but the IDs are those of v2.2 ("TT2", "TP1") plus a zero byte
    auto oldFrame = [](const char* id, const char* text) {
        Bytes data = { 0x00 };
        put(data, text);
        Bytes f = frame(id, data, 0, 3);
        f.insert(f.begin() + 3, 0);
        return f;
    };
    Bytes body = oldFrame("TT2", "Black Or White");
    put(body, oldFrame("TP1", "Michael Jackson"));
    put(body, oldFrame("TAL", "Dangerous"));
    put(body, frame("TRCK", { 0x00, '3' }, 0, 3));   // a regular frame in the same tag
    auto p = writeTagged("spec_v22_ids_in_v23.mp3", tagBytes(3, 0, body, 20));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(AUDIOGetTitleW()) == L"Black Or White");
    CHECK(take(AUDIOGetArtistW()) == L"Michael Jackson");
    CHECK(take(AUDIOGetAlbumW()) == L"Dangerous");
    CHECK(take(AUDIOGetTrackW()) == L"3");

    SECTION("saving writes regular v2.3 frame IDs and keeps all frames and the audio") {
        REQUIRE(AUDIOSaveChangesW() != 0);
        const Bytes saved = readFile(p);
        CHECK(audioIntact(p));
        CHECK(findBytes(saved, { 'T', 'T', '2', 0 }) == static_cast<size_t>(-1));
        CHECK(findBytes(saved, { 'T', 'P', '1', 0 }) == static_cast<size_t>(-1));
        CHECK(findBytes(saved, { 'T', 'A', 'L', 0 }) == static_cast<size_t>(-1));
        CHECK(findBytes(saved, { 'T', 'I', 'T', '2' }) != static_cast<size_t>(-1));
        CHECK(findBytes(saved, { 'T', 'P', 'E', '1' }) != static_cast<size_t>(-1));
        CHECK(findBytes(saved, { 'T', 'A', 'L', 'B' }) != static_cast<size_t>(-1));
        CHECK(findBytes(saved, { 'T', 'R', 'C', 'K' }) != static_cast<size_t>(-1));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(AUDIOGetTitleW()) == L"Black Or White");
        CHECK(take(AUDIOGetArtistW()) == L"Michael Jackson");
        CHECK(take(AUDIOGetAlbumW()) == L"Dangerous");
        CHECK(take(AUDIOGetTrackW()) == L"3");
    }
    SECTION("a changed field is saved with the regular ID, the other fields stay") {
        AUDIOSetTitleW(L"Changed");
        REQUIRE(AUDIOSaveChangesW() != 0);
        CHECK(audioIntact(p));
        CHECK(findBytes(readFile(p), { 'T', 'T', '2', 0 }) == static_cast<size_t>(-1));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(AUDIOGetTitleW()) == L"Changed");
        CHECK(take(AUDIOGetArtistW()) == L"Michael Jackson");
        CHECK(take(AUDIOGetAlbumW()) == L"Dangerous");
    }
}

TEST_CASE("ID3v2.4: frame sizes written as ordinary numbers (old iTunes versions) are read", "[id3v2][spec][header]")
{
    // v2.4 says: the size of a frame is a synchsafe integer. Some taggers write an ordinary 32 bit number, which is the same only below
    // 128 bytes. frame(..., 3) writes the ordinary number, frame(..., 4) the synchsafe integer.
    auto utf8 = [](const std::string& s) {
        Bytes b = { 0x03 };
        b.insert(b.end(), s.begin(), s.end());
        return b;
    };
    auto picture = [](size_t n) {
        Bytes b = { 0x00, 'i', 'm', 'a', 'g', 'e', '/', 'j', 'p', 'e', 'g', 0x00, 0x03, 0x00 };
        for (size_t i = 0; i < n; i++)
            b.push_back(static_cast<uint8_t>((i * 7 + 1) % 200 + 1));
        return b;
    };
    const std::string longText(300, 'A');   // capital letters: also look like a frame ID
    auto check = [](const fs::path& p, const wchar_t* title, const wchar_t* artist) {
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(AUDIOGetTitleW()) == title);
        CHECK(take(AUDIOGetArtistW()) == artist);
    };

    SECTION("a text frame of 300 bytes") {
        Bytes body = frame("TIT2", utf8("Title"), 0, 3);
        put(body, frame("TALB", utf8(longText), 0, 3));
        put(body, frame("TPE1", utf8("Artist"), 0, 3));
        auto p = writeTagged("spec_v24_plain_text.mp3", tagBytes(4, 0, body));
        check(p, L"Title", L"Artist");
        CHECK(take(AUDIOGetAlbumW()) == std::wstring(longText.begin(), longText.end()));
        CHECK(AUDIOGetLastErrorNumberW() == 0);
    }
    SECTION("a picture of 20000 bytes in the middle") {
        Bytes body = frame("TIT2", utf8("Title"), 0, 3);
        put(body, frame("APIC", picture(20000), 0, 3));
        put(body, frame("TPE1", utf8("Artist"), 0, 3));
        check(writeTagged("spec_v24_plain_pic.mp3", tagBytes(4, 0, body)), L"Title", L"Artist");
    }
    SECTION("a picture of 300000 bytes in front of the text frames") {
        Bytes body = frame("APIC", picture(300000), 0, 3);
        put(body, frame("TIT2", utf8("Title"), 0, 3));
        put(body, frame("TPE1", utf8("Artist"), 0, 3));
        check(writeTagged("spec_v24_plain_pic_first.mp3", tagBytes(4, 0, body)), L"Title", L"Artist");
    }
    SECTION("saving writes synchsafe sizes and keeps the audio") {
        Bytes body = frame("TIT2", utf8("Title"), 0, 3);
        put(body, frame("APIC", picture(20000), 0, 3));
        put(body, frame("TPE1", utf8("Artist"), 0, 3));
        auto p = writeTagged("spec_v24_plain_save.mp3", tagBytes(4, 0, body));
        check(p, L"Title", L"Artist");
        saveAsV24(3);   // changes the artist and writes the tag as v2.4
        CHECK(audioIntact(p));
        check(p, L"Title", L"Changed");
        // the picture is still there: the tag is at least as large as the picture
        CHECK(id3v2TotalSize(readFile(p)) > 20000);
    }
    SECTION("synchsafe sizes with capital letters in the text are not taken for ordinary numbers") {
        Bytes body = frame("TALB", utf8(longText), 0, 4);   // 301 bytes: synchsafe 00 00 02 2D, as an ordinary number 557
        put(body, frame("TIT2", utf8("Title"), 0, 4));
        put(body, frame("TPE1", utf8("Artist"), 0, 4));
        check(writeTagged("spec_v24_synchsafe_caps.mp3", tagBytes(4, 0, body)), L"Title", L"Artist");
        CHECK(take(AUDIOGetAlbumW()) == std::wstring(longText.begin(), longText.end()));
    }
}

TEST_CASE("ID3v2.4: the unsynchronisation flag in the tag header applies to all frames", "[id3v2][spec][unsync]")
{
    // Unsynchronisation puts a 0x00 byte behind every 0xFF byte that is followed by 0x00 or by a byte >= 0xE0. UTF-16 text with a byte
    // order mark (FF FE) and a character with the code unit 00FF (bytes FF 00) needs it twice. Many taggers set only the flag in the tag
    // header and not the flag of the frames (0x0002); decoding twice would remove the 0x00 of the second kind again.
    auto unsync = [](const Bytes& in) {
        Bytes out;
        for (size_t i = 0; i < in.size(); i++) {
            out.push_back(in[i]);
            if (in[i] == 0xFF && (i + 1 == in.size() || in[i + 1] == 0 || in[i + 1] >= 0xE0))
                out.push_back(0);
        }
        return out;
    };
    const Bytes title = { 0x01, 0xFF, 0xFE, 'T', 0, 0xFF, 0, 'x', 0 };     // UTF-16LE with BOM: "T" U+00FF "x"
    const Bytes artist = { 0x01, 0xFF, 0xFE, 'A', 0, 'r', 0, 't', 0 };     // "Art"
    REQUIRE(unsync(title) != title);
    auto check = [](const fs::path& p) {
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(AUDIOGetTitleW()) == L"T\u00ffx");
        CHECK(take(AUDIOGetArtistW()) == L"Art");
    };
    auto build = [&](uint8_t tagFlags, uint16_t frameFlags, bool unsynchronised) {
        Bytes body = frame("TIT2", unsynchronised ? unsync(title) : title, frameFlags, 4);
        put(body, frame("TPE1", unsynchronised ? unsync(artist) : artist, frameFlags, 4));
        return tagBytes(4, tagFlags, body);
    };

    SECTION("only the flag of the frames (the specification)") {
        check(writeTagged("spec_v24_unsync_frames.mp3", build(0, 0x0002, true)));
    }
    SECTION("only the flag of the tag header") {
        check(writeTagged("spec_v24_unsync_header.mp3", build(0x80, 0, true)));
    }
    SECTION("both flags: the data are decoded only once") {
        check(writeTagged("spec_v24_unsync_both.mp3", build(0x80, 0x0002, true)));
    }
    SECTION("no flag and data that are not unsynchronised") {
        check(writeTagged("spec_v24_unsync_none.mp3", build(0, 0, false)));
    }
    SECTION("saving keeps the text and the audio") {
        auto p = writeTagged("spec_v24_unsync_save.mp3", build(0x80, 0, true));
        check(p);
        ID3V2SetTextFrameW(ID3F_TALB, L"Album");
        REQUIRE(ID3V2SaveChangesW() != 0);
        CHECK(audioIntact(p));
        check(p);
        CHECK(take(AUDIOGetAlbumW()) == L"Album");
    }
}

TEST_CASE("ID3v2 tag sizes around the cache of the start of the file give the same result", "[id3v2][spec][head]")
{
    // The analysis reads the first 8192 bytes of the file once and answers the reads of the format, the ID3v2 header and tag and the first MPEG
    // block (3460 bytes) from that block, as far as they are completely inside of it. A tag that ends at 4732 or before leaves the whole first MPEG
    // block inside, behind a larger tag the cache is extended (up to 256 KB: tag and 8 KB behind it), a tag that is still larger is read from the file.
    // The results must not depend on it.
    const Bytes title = { 0x03, 'T', 'i', 't', 'l', 'e' };
    const Bytes body = frame("TIT2", title);
    long frames = 0;
    float duration = 0;
    for (size_t total : std::vector<size_t>{ 40, 100, 4000, 4732, 4733, 5000, 8000, 8181, 8192, 8193, 8300, 12000, 20000, 60000, 131072, 253951, 253952, 262144, 300000 }) {
        INFO("tag of " << total << " bytes");
        auto p = writeTagged("spec_head_cache.mp3", tagBytes(4, 0, body, total - 10 - body.size()));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(AUDIOGetTitleW()) == L"Title");
        CHECK(MPEGGetFramePositionW() == static_cast<long>(total));
        CHECK(AUDIOGetBitrateW() > 0);
        if (frames == 0) {
            frames = MPEGGetFramesW();
            duration = AUDIOGetDurationW();
            REQUIRE(frames > 0);
        }
        CHECK(MPEGGetFramesW() == frames);
        CHECK(AUDIOGetDurationW() == duration);
        CHECK(id3v2TotalSize(readFile(p)) == total);
    }
    SECTION("a file that is smaller than the cache: tag and frames are read from the cache of the start") {
        Bytes shortFile = tagBytes(4, 0, body, 200 - 10 - body.size());
        const Bytes a = makeMp3(4);
        shortFile.insert(shortFile.end(), a.begin(), a.end());
        REQUIRE(shortFile.size() < 8192);
        auto p = writeTemp("spec_head_cache_small.mp3", shortFile);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(AUDIOGetTitleW()) == L"Title");
        CHECK(MPEGGetFramePositionW() == 200);
        CHECK(std::abs(MPEGGetFramesW() - 4) <= 1);
    }
    SECTION("saving after the analysis, then analyzing again") {
        auto p = writeTagged("spec_head_cache_save.mp3", tagBytes(4, 0, body, 3000));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        AUDIOSetTitleW(L"Changed");
        REQUIRE(AUDIOSaveChangesW() != 0);
        CHECK(audioIntact(p));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
        CHECK(take(AUDIOGetTitleW()) == L"Changed");
    }
}

TEST_CASE("ID3v2: a CUE sheet becomes chapters; an INDEX without a time and an open quotation mark at the end do not crash", "[id3v2][spec][chapter]")
{
    const std::string cue =
        "PERFORMER \"Band\"\r\nTITLE \"Album\"\r\nFILE \"x.mp3\" MP3\r\n"
        "  TRACK 01 AUDIO\r\n    TITLE \"One\"\r\n    INDEX 01 00:00:00\r\n"
        "  TRACK 02 AUDIO\r\n    TITLE \"Two\"\r\n    INDEX 01 01:02:30\r\n"
        "  TRACK 03 AUDIO\r\n    INDEX 01\r\n    TITLE \"Three";   // no time; quotation mark not closed, no line end
    auto c = writeTemp("spec_chapters.cue", Bytes(cue.begin(), cue.end()));
    auto p = writeTemp("spec_chapters.mp3", makeMp3(40));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    REQUIRE(ID3V2ImportCueFileW(c.c_str()) != 0);
    CHECK(ID3V2GetFrameCountW(ID3F_CHAP) == 3);
    CHECK(ID3V2GetChapterStartTimeW(L"ch1") == 0);
    CHECK(ID3V2GetChapterEndTimeW(L"ch1") == 62400);     // the start of track 2
    CHECK(ID3V2GetChapterStartTimeW(L"ch2") == 62400);   // 1 min 2 s and 30 of 75 frames = 400 ms
}

TEST_CASE("ID3v2: a CUE sheet in UTF-8 (with or without byte order mark) or in the code page of the system", "[id3v2][spec][chapter][codepage]")
{
    const std::string head = "  TRACK 01 AUDIO\r\n    TITLE \"";
    const std::string tail = "\"\r\n    INDEX 01 00:00:00\r\n";
    // what the code page of this computer makes of the byte 0xE9 (U+00E9 with 1252)
    wchar_t systemChar = 0;
    MultiByteToWideChar(CP_ACP, 0, "\xE9", 1, &systemChar, 1);
    struct Case { const char* name; std::string cue; std::wstring title; };
    const Case cases[] = {
        { "UTF-8 with byte order mark", "\xEF\xBB\xBF" + head + "Caf\xC3\xA9" + tail, L"Café" },
        { "UTF-8 without byte order mark", head + "Caf\xC3\xA9" + tail, L"Café" },
        { "not UTF-8: the code page of the system", head + "Caf\xE9" + tail, std::wstring(L"Caf") + systemChar },
    };
    for (const Case& c : cases) {
        DYNAMIC_SECTION(c.name) {
            auto cue = writeTemp("spec_chapters_enc.cue", Bytes(c.cue.begin(), c.cue.end()));
            auto p = writeTemp("spec_chapters_enc.mp3", makeMp3(40));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
            REQUIRE(ID3V2ImportCueFileW(cue.c_str()) != 0);
            REQUIRE(ID3V2GetFrameCountW(ID3F_CHAP) == 1);
            CHECK(take(ID3V2GetSubFrameTextW(L"ch1", 1)) == c.title);
        }
    }
}

TEST_CASE("ID3v2: the language of COMM, USER, USLT and SYLT is read with the code page it is written with", "[id3v2][spec][codepage]")
{
    CodePage cp(1251);   // the byte 0xE9 is U+0439 in 1251
    const Bytes lang = { 'd', 0xE9, 'u' };
    Bytes comm = { 0x00 }; comm.insert(comm.end(), lang.begin(), lang.end()); comm.push_back(0); comm.push_back('c');
    Bytes uslt = comm;
    Bytes user = { 0x00 }; user.insert(user.end(), lang.begin(), lang.end()); user.push_back('u');
    Bytes sylt = { 0x00 }; sylt.insert(sylt.end(), lang.begin(), lang.end()); sylt.push_back(2); sylt.push_back(1); sylt.push_back(0);
    Bytes body = frame("COMM", comm);
    const Bytes more[] = { frame("USLT", uslt), frame("USER", user), frame("SYLT", sylt) };
    for (const Bytes& f : more) body.insert(body.end(), f.begin(), f.end());
    auto p = writeTagged("spec_language_cp.mp3", tagBytes(4, 0, body, 100));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    CHECK(take(ID3V2GetCommentLanguageW(1)) == L"dйu");
    CHECK(take(ID3V2GetLyricLanguageW(1)) == L"dйu");
    CHECK(take(ID3V2GetUserFrameLanguageW(1)) == L"dйu");
    CHECK(take(ID3V2GetSyncLyricLanguageW(1)) == L"dйu");
}
