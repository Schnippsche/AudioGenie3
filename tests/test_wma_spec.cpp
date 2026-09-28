// WMA checks with synthetic files (ASF specification): header object, file properties, stream properties, content description, extended content
// description, header extension with metadata library and padding, data object. The files that are written are checked with an own reader of the
// objects: sizes, the number of the objects, the file size in the file properties, the data object.
#include "id3v2_support.h"
#include <cmath>
#include <string>

using namespace ag3test;

namespace {

void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
void le16(Bytes& b, uint32_t v) { b.push_back(static_cast<uint8_t>(v)); b.push_back(static_cast<uint8_t>(v >> 8)); }
void le32(Bytes& b, uint32_t v) { le16(b, v & 0xFFFF); le16(b, v >> 16); }
void le64(Bytes& b, uint64_t v) { le32(b, static_cast<uint32_t>(v)); le32(b, static_cast<uint32_t>(v >> 32)); }

Bytes guid(uint32_t d1, uint16_t d2, uint16_t d3, std::initializer_list<uint8_t> d4)
{
    Bytes b;
    le32(b, d1); le16(b, d2); le16(b, d3);
    b.insert(b.end(), d4.begin(), d4.end());
    return b;
}

const Bytes G_HEADER = guid(0x75B22630, 0x668E, 0x11CF, { 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C });
const Bytes G_DATA = guid(0x75B22636, 0x668E, 0x11CF, { 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C });
const Bytes G_FILEPROP = guid(0x8CABDCA1, 0xA947, 0x11CF, { 0x8E, 0xE4, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65 });
const Bytes G_STREAM = guid(0xB7DC0791, 0xA9B7, 0x11CF, { 0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65 });
const Bytes G_AUDIO = guid(0xF8699E40, 0x5B4D, 0x11CF, { 0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B });
const Bytes G_VIDEO = guid(0xBC19EFC0, 0x5B4D, 0x11CF, { 0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B });
const Bytes G_NOECC = guid(0x20FB5700, 0x5B55, 0x11CF, { 0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B });
const Bytes G_CONTENT = guid(0x75B22633, 0x668E, 0x11CF, { 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C });
const Bytes G_EXTCONTENT = guid(0xD2D0A440, 0xE307, 0x11D2, { 0x97, 0xF0, 0x00, 0xA0, 0xC9, 0x5E, 0xA8, 0x50 });
const Bytes G_HEADEREXT = guid(0x5FBF03B5, 0xA92E, 0x11CF, { 0x8E, 0xE3, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65 });
const Bytes G_RESERVED1 = guid(0xABD3D211, 0xA9BA, 0x11CF, { 0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65 });
const Bytes G_METALIB = guid(0x44231C94, 0x9498, 0x49D1, { 0xA1, 0x41, 0x1D, 0x13, 0x4E, 0x45, 0x70, 0x54 });
const Bytes G_PADDING = guid(0x1806D474, 0xCADF, 0x4509, { 0xA4, 0xBA, 0x9A, 0xAB, 0xCB, 0x96, 0xAA, 0xE8 });
const Bytes G_UNKNOWN = guid(0x11223344, 0x5566, 0x7788, { 1, 2, 3, 4, 5, 6, 7, 8 });

Bytes object(const Bytes& id, const Bytes& payload)
{
    Bytes b = id;
    le64(b, payload.size() + 24);
    put(b, payload);
    return b;
}

Bytes utf16(const std::wstring& s, bool terminator = true)
{
    Bytes b;
    for (wchar_t c : s) le16(b, c);
    if (terminator) le16(b, 0);
    return b;
}

struct Props {
    uint64_t fileSize = 0;          // 0: set to the real size
    uint64_t playDuration = 50000000;   // 5 s (100 ns)
    uint64_t preroll = 0;               // ms
    uint32_t flags = 2;
    uint32_t maxBitrate = 128000;
};

Bytes fileProperties(const Props& p, uint64_t fileSize)
{
    Bytes b;
    for (int i = 0; i < 16; i++) b.push_back(static_cast<uint8_t>(0xF0 + i));   // file id
    le64(b, p.fileSize ? p.fileSize : fileSize);
    le64(b, 0x01D0000000000000ull);   // creation date
    le64(b, 1000);                     // data packets
    le64(b, p.playDuration);
    le64(b, p.playDuration);           // send duration
    le64(b, p.preroll);
    le32(b, p.flags);
    le32(b, 3200); le32(b, 3200);      // packet sizes
    le32(b, p.maxBitrate);
    return b;
}

Bytes streamProperties(bool audio, int streamNumber, int channels, uint32_t rate)
{
    Bytes b;
    put(b, audio ? G_AUDIO : G_VIDEO);
    put(b, G_NOECC);
    le64(b, 0);                        // time offset
    Bytes specific;
    if (audio) {
        le16(specific, 0x0161);        // WMA v2
        le16(specific, channels);
        le32(specific, rate);
        le32(specific, 16000);         // bytes per second
        le16(specific, 4096);          // block align
        le16(specific, 16);            // bits per sample
        le16(specific, 10);            // size of the codec data
        specific.resize(specific.size() + 10, 0x0A);
    } else {
        specific.resize(51, 0x0B);     // video data (not interpreted)
    }
    le32(b, static_cast<uint32_t>(specific.size()));
    le32(b, 0);                        // error correction data length
    le16(b, static_cast<uint32_t>(streamNumber));   // flags: stream number
    le32(b, 0);                        // reserved
    put(b, specific);
    return b;
}

Bytes contentDescription(const std::wstring& title, const std::wstring& author, const std::wstring& copyright, const std::wstring& description, const std::wstring& rating)
{
    Bytes t = title.empty() ? Bytes() : utf16(title), a = author.empty() ? Bytes() : utf16(author), c = copyright.empty() ? Bytes() : utf16(copyright),
          d = description.empty() ? Bytes() : utf16(description), r = rating.empty() ? Bytes() : utf16(rating);
    Bytes b;
    le16(b, static_cast<uint32_t>(t.size())); le16(b, static_cast<uint32_t>(a.size())); le16(b, static_cast<uint32_t>(c.size()));
    le16(b, static_cast<uint32_t>(d.size())); le16(b, static_cast<uint32_t>(r.size()));
    put(b, t); put(b, a); put(b, c); put(b, d); put(b, r);
    return b;
}

struct Attr { std::wstring name; int type; Bytes value; };

Bytes extAttr(const Attr& a)
{
    Bytes b;
    const Bytes n = utf16(a.name);
    le16(b, static_cast<uint32_t>(n.size())); put(b, n);
    le16(b, static_cast<uint32_t>(a.type));
    le16(b, static_cast<uint32_t>(a.value.size())); put(b, a.value);
    return b;
}

Bytes extContent(const std::vector<Attr>& attrs)
{
    Bytes b;
    le16(b, static_cast<uint32_t>(attrs.size()));
    for (const Attr& a : attrs) put(b, extAttr(a));
    return b;
}

Bytes metaLibrary(const std::vector<Attr>& attrs, int stream = 0)
{
    Bytes b;
    le16(b, static_cast<uint32_t>(attrs.size()));
    for (const Attr& a : attrs) {
        const Bytes n = utf16(a.name);
        le16(b, 0);                                          // language list index
        le16(b, static_cast<uint32_t>(stream));
        le16(b, static_cast<uint32_t>(n.size()));
        le16(b, static_cast<uint32_t>(a.type));
        le32(b, static_cast<uint32_t>(a.value.size()));
        put(b, n); put(b, a.value);
    }
    return b;
}

struct WmaSpec {
    Props props;
    bool videoFirst = false;
    std::wstring title = L"Song", author = L"Band", copyright = L"(c) Me", description = L"Comment", rating = L"5";
    std::vector<Attr> ext = { { L"WM/AlbumTitle", 0, utf16(L"Record") }, { L"WM/Genre", 0, utf16(L"Rock") }, { L"WM/TrackNumber", 3, { 7, 0, 0, 0 } } };
    std::vector<Attr> meta;
    size_t padding = 3000;                  // payload of the padding object in the header extension (0: none)
    bool unknownObject = true;
    int dataPackets = 40;
    size_t tailBytes = 0;                   // an index object behind the data object
};

Bytes wmaFile(const WmaSpec& s)
{
    // the objects of the header
    Bytes objects;
    int count = 0;
    auto add = [&](const Bytes& o) { put(objects, o); count++; };
    add(object(G_FILEPROP, fileProperties(s.props, 0)));   // the file size is put in below
    if (s.videoFirst) add(object(G_STREAM, streamProperties(false, 1, 0, 0)));
    add(object(G_STREAM, streamProperties(true, s.videoFirst ? 2 : 1, 2, 44100)));
    if (s.unknownObject) add(object(G_UNKNOWN, Bytes(37, 0x5A)));
    add(object(G_CONTENT, contentDescription(s.title, s.author, s.copyright, s.description, s.rating)));
    if (!s.ext.empty()) add(object(G_EXTCONTENT, extContent(s.ext)));
    // the header extension: reserved fields, data size, objects
    Bytes ext;
    if (!s.meta.empty()) put(ext, object(G_METALIB, metaLibrary(s.meta)));
    if (s.padding) put(ext, object(G_PADDING, Bytes(s.padding, 0)));
    Bytes extPayload = G_RESERVED1;
    le16(extPayload, 6);
    le32(extPayload, static_cast<uint32_t>(ext.size()));
    put(extPayload, ext);
    add(object(G_HEADEREXT, extPayload));
    Bytes header = G_HEADER;
    le64(header, objects.size() + 30);
    le32(header, static_cast<uint32_t>(count));
    header.push_back(1); header.push_back(2);
    put(header, objects);
    // the data object
    Bytes data = G_DATA;
    Bytes packets;
    for (int i = 0; i < s.dataPackets; i++) packets.resize(packets.size() + 3200, static_cast<uint8_t>(0x30 + i));
    le64(data, 50 + packets.size());
    for (int i = 0; i < 16; i++) data.push_back(static_cast<uint8_t>(0xF0 + i));
    le64(data, static_cast<uint64_t>(s.dataPackets)); le16(data, 0x0101);
    put(data, packets);
    Bytes out = header;
    put(out, data);
    if (s.tailBytes) put(out, object(guid(0x33000890, 0xE5B1, 0x11CF, { 0x89, 0xF4, 0x00, 0xA0, 0xC9, 0x03, 0x49, 0xCB }), Bytes(s.tailBytes, 0x77)));
    // the file size in the file properties (offset: header 30 + object header 24 + file id 16)
    if (!s.props.fileSize) {
        const uint64_t fs = out.size();
        for (int i = 0; i < 8; i++) out[30 + 24 + 16 + static_cast<size_t>(i)] = static_cast<uint8_t>(fs >> (8 * i));
    }
    return out;
}

struct Obj { Bytes id; size_t offset, size; };

// the objects of the header and the position of the data object; checks the sizes and the number of the objects
struct Layout {
    std::vector<Obj> objects;
    size_t headerSize = 0;
    uint32_t declaredCount = 0;
    size_t dataOffset = 0;
    uint64_t fileSizeField = 0;
};

uint64_t rd64(const Bytes& f, size_t p) { uint64_t v = 0; for (int i = 7; i >= 0; i--) v = (v << 8) | f[p + static_cast<size_t>(i)]; return v; }
uint32_t rd32(const Bytes& f, size_t p) { return static_cast<uint32_t>(f[p] | (f[p + 1] << 8) | (f[p + 2] << 16) | (static_cast<uint32_t>(f[p + 3]) << 24)); }

Layout layout(const Bytes& f)
{
    Layout l;
    REQUIRE(f.size() > 30);
    REQUIRE(Bytes(f.begin(), f.begin() + 16) == G_HEADER);
    l.headerSize = static_cast<size_t>(rd64(f, 16));
    l.declaredCount = rd32(f, 24);
    size_t pos = 30;
    while (pos + 24 <= l.headerSize) {
        Obj o;
        o.id = Bytes(f.begin() + static_cast<std::ptrdiff_t>(pos), f.begin() + static_cast<std::ptrdiff_t>(pos + 16));
        o.offset = pos;
        o.size = static_cast<size_t>(rd64(f, pos + 16));
        REQUIRE(o.size >= 24);
        l.objects.push_back(o);
        pos += o.size;
    }
    CHECK(pos == l.headerSize);                          // the objects fill the header exactly
    CHECK(l.objects.size() == l.declaredCount);          // the number of the objects in the header is right
    l.dataOffset = l.headerSize;
    REQUIRE(f.size() >= l.dataOffset + 50);
    CHECK(Bytes(f.begin() + static_cast<std::ptrdiff_t>(l.dataOffset), f.begin() + static_cast<std::ptrdiff_t>(l.dataOffset + 16)) == G_DATA);
    for (const Obj& o : l.objects)
        if (o.id == G_FILEPROP) l.fileSizeField = rd64(f, o.offset + 24 + 16);
    return l;
}

const Obj* find(const Layout& l, const Bytes& id)
{
    for (const Obj& o : l.objects) if (o.id == id) return &o;
    return nullptr;
}

Bytes tail(const Bytes& f, const Layout& l) { return Bytes(f.begin() + static_cast<std::ptrdiff_t>(l.dataOffset), f.end()); }

// header, data, an unknown object: the rules for every written file
void checkWritten(const Bytes& written, const Bytes& original, bool sameSize = false)
{
    const Layout l = layout(written);
    const Layout l0 = layout(original);
    CHECK(tail(written, l) == tail(original, l0));               // the data object and everything behind it are unchanged
    CHECK(l.fileSizeField == written.size());                    // the file size in the file properties
    if (sameSize) CHECK(written.size() == original.size());
    const Obj* u = find(l, G_UNKNOWN);
    const Obj* u0 = find(l0, G_UNKNOWN);
    if (u0 != nullptr) {
        REQUIRE(u != nullptr);                                   // objects that the library does not know stay
        CHECK(Bytes(written.begin() + static_cast<std::ptrdiff_t>(u->offset), written.begin() + static_cast<std::ptrdiff_t>(u->offset + u->size)) ==
              Bytes(original.begin() + static_cast<std::ptrdiff_t>(u0->offset), original.begin() + static_cast<std::ptrdiff_t>(u0->offset + u0->size)));
    }
    const Obj* fp = find(l, G_FILEPROP);
    const Obj* fp0 = find(l0, G_FILEPROP);
    REQUIRE(fp != nullptr);
    // the rest of the file properties is unchanged
    Bytes a(written.begin() + static_cast<std::ptrdiff_t>(fp->offset), written.begin() + static_cast<std::ptrdiff_t>(fp->offset + fp->size));
    Bytes b(original.begin() + static_cast<std::ptrdiff_t>(fp0->offset), original.begin() + static_cast<std::ptrdiff_t>(fp0->offset + fp0->size));
    std::fill(a.begin() + 40, a.begin() + 48, 0); std::fill(b.begin() + 40, b.begin() + 48, 0);
    CHECK(a == b);
}

}  // namespace

TEST_CASE("WMA: the builder and the checker of the tests agree", "[wma][spec][selftest]")
{
    const Bytes f = wmaFile(WmaSpec());
    const Layout l = layout(f);
    CHECK(l.fileSizeField == f.size());
    CHECK(l.objects.size() == 6);
}

TEST_CASE("WMA: duration with the preroll, sample rate and channels of the audio stream, live streams", "[wma][spec]")
{
    SECTION("a plain file") {
        auto p = writeTemp("wma_plain.wma", wmaFile(WmaSpec()));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(std::fabs(AUDIOGetDurationW() - 5.0) < 0.001);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(AUDIOGetBitrateW() == 128);
    }
    SECTION("the play duration contains the preroll") {
        WmaSpec s; s.props.preroll = 3000;
        auto p = writeTemp("wma_preroll.wma", wmaFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(std::fabs(AUDIOGetDurationW() - 2.0) < 0.001);
    }
    SECTION("a preroll that is longer than the play duration gives no negative duration") {
        WmaSpec s; s.props.preroll = 9000;
        auto p = writeTemp("wma_preroll_big.wma", wmaFile(s));
        AUDIOAnalyzeFileW(p.c_str());
        CHECK(AUDIOGetDurationW() >= 0.0f);
    }
    SECTION("a video stream in front of the audio stream") {
        WmaSpec s; s.videoFirst = true;
        auto p = writeTemp("wma_video_first.wmv", wmaFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
    }
    SECTION("a live stream (broadcast flag) has no play duration: it is estimated from the size and the bit rate") {
        WmaSpec s; s.props.flags = 1; s.props.playDuration = 0; s.props.maxBitrate = 64000;
        const Bytes f = wmaFile(s);
        auto p = writeTemp("wma_live.wma", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        const Layout l = layout(f);
        CHECK(std::fabs(AUDIOGetDurationW() - 8.0 * static_cast<double>(f.size() - l.dataOffset) / 64000) < 0.01);
    }
}

TEST_CASE("WMA: the content description and the attribute types of the extended content description and the metadata library", "[wma][spec]")
{
    WmaSpec s;
    const std::vector<uint8_t> raw = { 1, 2, 3 };
    s.ext = { { L"WM/AlbumTitle", 0, utf16(L"Record") }, { L"WM/TrackNumber", 3, { 7, 0, 0, 0 } }, { L"IsVBR", 2, { 1, 0, 0, 0 } },
              { L"WM/Small", 5, { 9, 1 } }, { L"WM/Big", 4, { 0, 0, 0, 0, 1, 0, 0, 0 } }, { L"WM/Raw", 1, raw } };
    s.meta = { { L"WM/Flag", 2, { 1, 0 } }, { L"WM/Guid", 6, { 0x44, 0x33, 0x22, 0x11, 0x66, 0x55, 0x88, 0x77, 1, 2, 3, 4, 5, 6, 7, 8 } } };
    auto p = writeTemp("wma_types.wma", wmaFile(s));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
    CHECK(take(WMAGetUserItemW(L"Title")) == L"Song");
    CHECK(take(WMAGetUserItemW(L"Author")) == L"Band");
    CHECK(take(WMAGetUserItemW(L"Description")) == L"Comment");
    CHECK(take(WMAGetUserItemW(L"Copyright")) == L"(c) Me");
    CHECK(take(WMAGetUserItemW(L"WM/AlbumTitle")) == L"Record");
    CHECK(take(WMAGetUserItemW(L"WM/TrackNumber")) == L"7");
    CHECK(take(WMAGetUserItemW(L"IsVBR")) == L"true");
    CHECK(take(WMAGetUserItemW(L"WM/Small")) == L"265");
    CHECK(take(WMAGetUserItemW(L"WM/Big")) == L"4294967296");     // 64 bit
    CHECK(take(WMAGetUserItemW(L"WM/Flag")) == L"true");           // a BOOL of the metadata library has 16 bit
    CHECK(take(WMAGetUserItemW(L"WM/Guid")) == L"11223344-5566-7788-0102-030405060708");
}

TEST_CASE("WMA: writing keeps the objects, the data and the number of the objects; the file size is updated", "[wma][spec][write]")
{
    WmaSpec s; s.meta = { { L"WM/Flag", 2, { 1, 0 } } };
    const Bytes f = wmaFile(s);
    auto p = writeTemp("wma_write.wma", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
    SECTION("a change that fits into the padding: the file has the same size") {
        WMASetUserItemW(L"WM/Genre", L"Jazz");
        REQUIRE(WMASaveChangesW() != 0);
        const Bytes g = readFile(p);
        checkWritten(g, f, true);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(take(WMAGetUserItemW(L"WM/Genre")) == L"Jazz");
        CHECK(take(WMAGetUserItemW(L"WM/Flag")) == L"true");
    }
    SECTION("a change that does not fit: the header grows, the file size field grows with it") {
        WMASetUserItemW(L"WM/Composer", std::wstring(9000, L'c').c_str());
        REQUIRE(WMASaveChangesW() != 0);
        const Bytes g = readFile(p);
        CHECK(g.size() > f.size());
        checkWritten(g, f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(take(WMAGetUserItemW(L"WM/Composer")).size() == 9000);
    }
    SECTION("several saves in a row without analyzing again") {
        for (size_t size : { size_t(9000), size_t(100), size_t(30000), size_t(50) }) {
            INFO("size " << size);
            WMASetUserItemW(L"WM/Composer", std::wstring(size, L'c').c_str());
            REQUIRE(WMASaveChangesW() != 0);
            checkWritten(readFile(p), f);
        }
    }
    SECTION("the standard fields of the content description") {
        WMASetUserItemW(L"Title", L"New title");
        WMASetUserItemW(L"Copyright", L"");
        REQUIRE(WMASaveChangesW() != 0);
        checkWritten(readFile(p), f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(take(WMAGetUserItemW(L"Title")) == L"New title");
        CHECK(take(WMAGetUserItemW(L"Copyright")).empty());
        CHECK(take(WMAGetUserItemW(L"Author")) == L"Band");
    }
    SECTION("all fields of the extended content description are removed: the object is removed too") {
        WMASetUserItemW(L"WM/AlbumTitle", L"");
        WMASetUserItemW(L"WM/Genre", L"");
        WMASetUserItemW(L"WM/TrackNumber", L"");
        REQUIRE(WMASaveChangesW() != 0);
        const Bytes g = readFile(p);
        checkWritten(g, f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        CHECK(take(WMAGetUserItemW(L"WM/Genre")).empty());
        CHECK(take(WMAGetUserItemW(L"WM/AlbumTitle")).empty());
    }
    SECTION("without padding wanted (size 0) the audio data are kept") {
        SetConfigValueW(5, 0);   // WMAPADDINGSIZE
        SetConfigValueW(2, 0);   // ID3V2WRITEBLOCKSIZE: has no influence on WMA
        WMASetUserItemW(L"WM/Composer", std::wstring(9000, L'c').c_str());
        const short ok = WMASaveChangesW();
        SetConfigValueW(5, 4096);
        SetConfigValueW(2, 524288);
        REQUIRE(ok != 0);
        checkWritten(readFile(p), f);
    }
}

TEST_CASE("WMA: files without a padding object, with an index behind the data and with a live stream", "[wma][spec][write]")
{
    SECTION("no padding in the file: rebuilt, a padding object is added") {
        WmaSpec s; s.padding = 0;
        const Bytes f = wmaFile(s);
        auto p = writeTemp("wma_nopad.wma", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        WMASetUserItemW(L"WM/Genre", L"Jazz");
        REQUIRE(WMASaveChangesW() != 0);
        checkWritten(readFile(p), f);
    }
    SECTION("an index object behind the data object is copied") {
        WmaSpec s; s.tailBytes = 5000;
        const Bytes f = wmaFile(s);
        auto p = writeTemp("wma_index.wma", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        WMASetUserItemW(L"WM/Composer", std::wstring(9000, L'c').c_str());
        REQUIRE(WMASaveChangesW() != 0);
        checkWritten(readFile(p), f);
    }
    SECTION("a live stream keeps the zero in the file size field") {
        WmaSpec s; s.props.flags = 1; s.props.playDuration = 0; s.props.fileSize = 0;
        Bytes f = wmaFile(s);
        for (int i = 0; i < 8; i++) f[30 + 24 + 16 + static_cast<size_t>(i)] = 0;
        auto p = writeTemp("wma_live_write.wma", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WMA);
        WMASetUserItemW(L"WM/Composer", std::wstring(9000, L'c').c_str());
        REQUIRE(WMASaveChangesW() != 0);
        const Bytes g = readFile(p);
        const Layout l = layout(g);
        CHECK(l.fileSizeField == 0);
        CHECK(tail(g, l) == tail(f, layout(f)));
    }
}

TEST_CASE("WMA: damaged headers are not written", "[wma][spec][write]")
{
    const Bytes f = wmaFile(WmaSpec());
    SECTION("the file ends inside the header") {
        const Bytes cut(f.begin(), f.begin() + 400);
        auto p = writeTemp("wma_cut_header.wma", cut);
        if (AUDIOAnalyzeFileW(p.c_str()) == WMA) {
            WMASetUserItemW(L"WM/Genre", L"x");
            CHECK(WMASaveChangesW() == 0);
        }
        CHECK(readFile(p) == cut);
    }
    SECTION("a huge number of objects does not hang") {
        Bytes g = f;
        g[24] = 0xFF; g[25] = 0xFF; g[26] = 0xFF; g[27] = 0x7F;
        auto p = writeTemp("wma_many_objects.wma", g);
        AUDIOAnalyzeFileW(p.c_str());
        SUCCEED();
    }
    SECTION("an object that is larger than the header") {
        Bytes g = f;
        for (int i = 0; i < 8; i++) g[30 + 16 + static_cast<size_t>(i)] = 0x7F;
        auto p = writeTemp("wma_big_object.wma", g);
        AUDIOAnalyzeFileW(p.c_str());
        SUCCEED();
    }
}
