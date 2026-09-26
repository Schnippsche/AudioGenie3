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
        // encoding $01, BOM + 'A' + $00 $00, BOM + 'B'
        const Bytes expected = { 0x01, 0xFF, 0xFE, 'A', 0x00, 0x00, 0x00, 0xFF, 0xFE, 'B', 0x00 };
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
        // only one value: encoding $01, BOM, 'A'
        CHECK(f[i + 7] == 0x05);   // frame size: encoding (1) + BOM (2) + 'A' (2)
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
    SECTION("v2.4: a frame with the unsynchronisation flag is resynchronised and written without the flag") {
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
        CHECK(f[i + 9] == 0x00);   // no unsynchronisation flag
        CHECK(findBytes(f, Bytes{ 'o', 'w', 'n', 0x00, 0xFF, 0x00, 0x12 }) != static_cast<size_t>(-1));
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
