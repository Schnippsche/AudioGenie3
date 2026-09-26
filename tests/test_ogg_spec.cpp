// Ogg Vorbis checks with synthetic files: pages and packets as in RFC 3533, the identification, comment and setup headers of the
// Vorbis I specification and the Vorbis comment specification. The checks of the written files are made with an own page reader
// that verifies the checksums, the page sequence numbers, the flags and the packets.
#include "id3v2_support.h"
#include <cmath>
#include <map>
#include <string>

using namespace ag3test;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
void le32(Bytes& b, uint32_t v) { for (int i = 0; i < 4; i++) b.push_back(static_cast<uint8_t>(v >> (8 * i))); }
std::wstring wide(const std::string& s) { return std::wstring(s.begin(), s.end()); }

// CRC-32 of Ogg: polynomial $04C11DB7, start value 0, not reflected, no final xor; the checksum field is zero
uint32_t oggCrc(const uint8_t* d, size_t n)
{
    static uint32_t table[256];
    static bool init = false;
    if (!init) {
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t r = i << 24;
            for (int k = 0; k < 8; k++) r = (r & 0x80000000u) ? (r << 1) ^ 0x04C11DB7u : (r << 1);
            table[i] = r;
        }
        init = true;
    }
    uint32_t crc = 0;
    for (size_t i = 0; i < n; i++) crc = (crc << 8) ^ table[((crc >> 24) & 0xFF) ^ d[i]];
    return crc;
}

struct Packet {
    Bytes data;
    int64_t granule = -1;     // granule position of the page if this is the last packet that ends on it
    bool flushAfter = false;  // the page ends behind this packet (as at the end of the headers)
};

// pages: up to 255 segments, a packet can continue on the next page. A page ends behind a packet with flushAfter; its granule position
// is the one of the packet that ends it (-1 if the page ends inside a packet).
Bytes paginate(const std::vector<Packet>& packets, uint32_t serial, uint32_t firstSeq, bool bos, bool eos)
{
    struct Seg { Bytes data; bool endsPacket; int64_t granule; bool flush; };
    std::vector<Seg> segs;
    for (const Packet& p : packets) {
        size_t pos = 0;
        while (true) {
            const size_t n = std::min<size_t>(255, p.data.size() - pos);
            Seg s;
            s.data.assign(p.data.begin() + static_cast<std::ptrdiff_t>(pos), p.data.begin() + static_cast<std::ptrdiff_t>(pos + n));
            pos += n;
            s.endsPacket = n < 255;
            if (n == 255 && pos == p.data.size()) {   // a packet whose size is a multiple of 255 ends with a segment of 0
                s.endsPacket = false;
                s.granule = -1; s.flush = false;
                segs.push_back(s);
                Seg z; z.endsPacket = true; z.granule = p.granule; z.flush = p.flushAfter;
                segs.push_back(z);
                break;
            }
            s.granule = s.endsPacket ? p.granule : -1;
            s.flush = s.endsPacket && p.flushAfter;
            segs.push_back(s);
            if (s.endsPacket) break;
        }
    }
    Bytes out;
    uint32_t seq = firstSeq;
    size_t i = 0;
    bool first = true, continued = false;
    while (i < segs.size()) {
        Bytes body, lacing;
        while (i < segs.size() && lacing.size() < 255) {
            lacing.push_back(static_cast<uint8_t>(segs[i].data.size()));
            put(body, segs[i].data);
            const bool flush = segs[i].flush;
            i++;
            if (flush) break;
        }
        const Seg& last = segs[i - 1];
        const int64_t granule = last.endsPacket ? last.granule : -1;
        Bytes h;
        put(h, "OggS");
        h.push_back(0);
        h.push_back(static_cast<uint8_t>((continued ? 1 : 0) | ((first && bos) ? 2 : 0) | ((i >= segs.size() && eos) ? 4 : 0)));
        for (int k = 0; k < 8; k++) h.push_back(static_cast<uint8_t>(static_cast<uint64_t>(granule) >> (8 * k)));
        le32(h, serial);
        le32(h, seq);
        le32(h, 0);
        h.push_back(static_cast<uint8_t>(lacing.size()));
        put(h, lacing);
        put(h, body);
        const uint32_t crc = oggCrc(h.data(), h.size());
        for (int k = 0; k < 4; k++) h[22 + k] = static_cast<uint8_t>(crc >> (8 * k));
        put(out, h);
        seq++;
        first = false;
        continued = !last.endsPacket;
    }
    return out;
}

Bytes idHeader(int channels, uint32_t rate, int32_t maxBr, int32_t nominal, int32_t minBr)
{
    Bytes b = { 0x01 };
    put(b, "vorbis");
    le32(b, 0);
    b.push_back(static_cast<uint8_t>(channels));
    le32(b, rate);
    le32(b, static_cast<uint32_t>(maxBr)); le32(b, static_cast<uint32_t>(nominal)); le32(b, static_cast<uint32_t>(minBr));
    b.push_back(0xB8);   // blocksizes 256 and 2048
    b.push_back(1);      // framing bit
    return b;
}

Bytes commentHeader(const std::string& vendor, const std::vector<std::string>& comments)
{
    Bytes b = { 0x03 };
    put(b, "vorbis");
    le32(b, static_cast<uint32_t>(vendor.size()));
    put(b, vendor.c_str());
    le32(b, static_cast<uint32_t>(comments.size()));
    for (const std::string& c : comments) { le32(b, static_cast<uint32_t>(c.size())); put(b, c.c_str()); }
    b.push_back(1);      // framing bit
    return b;
}

Bytes setupHeader(size_t size)
{
    Bytes b = { 0x05 };
    put(b, "vorbis");
    for (size_t i = 0; i < size; i++) b.push_back(static_cast<uint8_t>((i * 31 + 7) & 0xFF) | 0x02);
    b.push_back(1);
    return b;
}

struct OggSpec {
    int channels = 2;
    uint32_t rate = 44100;
    int32_t nominal = 128000;
    std::string vendor = "Xiph.Org libVorbis I 20200704";
    std::vector<std::string> comments = { "TITLE=Song", "ARTIST=Band", "album=Record", "DATE=2001" };
    size_t setupSize = 3000;
    int audioPackets = 200;
    int packetsPerPage = 20;
    int64_t samplesPerPacket = 1000;
    int64_t lastGranule = -100;     // -100: the sum of the packets
    bool lastPageWithoutGranule = false;
};

Bytes oggFile(const OggSpec& s)
{
    const uint32_t serial = 0x1234ABCD;
    Bytes out = paginate({ Packet{ idHeader(s.channels, s.rate, -1, s.nominal, -1), 0, true } }, serial, 0, true, false);
    // the comment header and the setup header end on a page (the audio starts on a new page)
    std::vector<Packet> hdr = { Packet{ commentHeader(s.vendor, s.comments), -1, false }, Packet{ setupHeader(s.setupSize), 0, true } };
    Bytes h2 = paginate(hdr, serial, 1, false, false);
    put(out, h2);
    uint32_t seq = 0;
    for (size_t i = 0; i + 27 <= out.size();) {   // count the pages
        const size_t segs = out[i + 26];
        size_t len = 27 + segs;
        for (size_t k = 0; k < segs; k++) len += out[i + 27 + k];
        i += len; seq++;
    }
    std::vector<Packet> audio;
    int64_t granule = 0;
    for (int i = 0; i < s.audioPackets; i++) {
        Packet p;
        p.data.assign(300 + (i % 7) * 40, static_cast<uint8_t>(0x40 + (i & 0x3F)));
        granule += s.samplesPerPacket;
        const bool endOfPage = ((i + 1) % s.packetsPerPage == 0) || i + 1 == s.audioPackets;
        p.granule = endOfPage ? ((i + 1 == s.audioPackets && s.lastGranule != -100) ? s.lastGranule : granule) : -1;
        p.flushAfter = endOfPage;
        audio.push_back(p);
    }
    if (s.lastPageWithoutGranule && !audio.empty()) {
        // an additional last page whose packet does not end on it: granule position -1
        Packet extra; extra.data.assign(700, 0x11); extra.granule = -1; extra.flushAfter = true;
        audio.push_back(extra);
    }
    put(out, paginate(audio, serial, seq, false, true));
    return out;
}

// ---- page reader with all checks of RFC 3533
struct PageInfo { uint8_t flags; int64_t granule; uint32_t serial, seq; size_t offset, length; bool crcOk; std::vector<std::vector<uint8_t>> segments; std::vector<uint8_t> lacing; };

std::vector<PageInfo> readPages(const Bytes& f)
{
    std::vector<PageInfo> pages;
    size_t pos = 0;
    while (pos + 27 <= f.size() && std::memcmp(&f[pos], "OggS", 4) == 0) {
        PageInfo p;
        p.offset = pos;
        p.flags = f[pos + 5];
        p.granule = 0;
        for (int k = 7; k >= 0; k--) p.granule = (p.granule << 8) | f[pos + 6 + static_cast<size_t>(k)];
        p.serial = f[pos + 14] | (f[pos + 15] << 8) | (f[pos + 16] << 16) | (static_cast<uint32_t>(f[pos + 17]) << 24);
        p.seq = f[pos + 18] | (f[pos + 19] << 8) | (f[pos + 20] << 16) | (static_cast<uint32_t>(f[pos + 21]) << 24);
        const uint32_t stored = f[pos + 22] | (f[pos + 23] << 8) | (f[pos + 24] << 16) | (static_cast<uint32_t>(f[pos + 25]) << 24);
        const size_t segs = f[pos + 26];
        size_t len = 27 + segs;
        size_t data = pos + 27 + segs;
        for (size_t k = 0; k < segs; k++) {
            p.lacing.push_back(f[pos + 27 + k]);
            p.segments.emplace_back(f.begin() + static_cast<std::ptrdiff_t>(data), f.begin() + static_cast<std::ptrdiff_t>(data + f[pos + 27 + k]));
            data += f[pos + 27 + k];
            len += f[pos + 27 + k];
        }
        p.length = len;
        if (pos + len > f.size()) break;
        Bytes copy(f.begin() + static_cast<std::ptrdiff_t>(pos), f.begin() + static_cast<std::ptrdiff_t>(pos + len));
        std::fill(copy.begin() + 22, copy.begin() + 26, 0);
        p.crcOk = (oggCrc(copy.data(), copy.size()) == stored);
        pages.push_back(p);
        pos += len;
    }
    return pages;
}

// the packets of the file (a packet ends with a segment below 255)
std::vector<Bytes> readPackets(const std::vector<PageInfo>& pages)
{
    std::vector<Bytes> packets;
    Bytes cur;
    for (const PageInfo& p : pages)
        for (size_t k = 0; k < p.segments.size(); k++) {
            put(cur, p.segments[k]);
            if (p.lacing[k] < 255) { packets.push_back(cur); cur.clear(); }
        }
    return packets;
}

// all requirements of the container: sequence numbers, checksums, flags, granule positions, packets
void checkContainer(const Bytes& f, size_t expectedHeaderPackets = 3)
{
    const auto pages = readPages(f);
    REQUIRE(!pages.empty());
    size_t total = 0;
    for (const PageInfo& p : pages) total += p.length;
    CHECK(total == f.size());   // no data behind the last page
    for (size_t i = 0; i < pages.size(); i++) {
        INFO("page " << i);
        CHECK(pages[i].crcOk);
        CHECK(pages[i].seq == i);
        CHECK(pages[i].serial == pages[0].serial);
        CHECK(((pages[i].flags & 2) != 0) == (i == 0));                    // BOS on the first page only
        CHECK(((pages[i].flags & 4) != 0) == (i + 1 == pages.size()));      // EOS on the last page only
        if (i > 0) {
            // the continued packet flag: the previous page ends with a segment of 255
            const bool prevContinues = pages[i - 1].lacing.back() == 255;
            CHECK(((pages[i].flags & 1) != 0) == prevContinues);
        }
    }
    const auto packets = readPackets(pages);
    REQUIRE(packets.size() >= expectedHeaderPackets);
    CHECK(packets[0].size() == 30);
    CHECK(packets[0][0] == 1);
    CHECK(std::memcmp(&packets[0][1], "vorbis", 6) == 0);
    CHECK(packets[1][0] == 3);
    CHECK(std::memcmp(&packets[1][1], "vorbis", 6) == 0);
    CHECK(packets[1].back() == 1);   // framing bit of the comment header
    CHECK(packets[2][0] == 5);
    CHECK(std::memcmp(&packets[2][1], "vorbis", 6) == 0);
    // the first audio packet starts on a new page: the headers end at the end of a page
    size_t headerPackets = 0;
    for (size_t i = 0; i < pages.size(); i++)
        for (size_t k = 0; k < pages[i].segments.size(); k++)
            if (pages[i].lacing[k] < 255) {
                headerPackets++;
                if (headerPackets == 3) CHECK(k + 1 == pages[i].segments.size());
            }
}

// comment header of the file: vendor and the list of the comments
struct Comments { std::string vendor; std::vector<std::string> list; };
Comments parseComments(const Bytes& f)
{
    const auto packets = readPackets(readPages(f));
    Comments c;
    const Bytes& p = packets.at(1);
    size_t pos = 7;
    auto rd = [&](size_t at) { return static_cast<uint32_t>(p[at] | (p[at + 1] << 8) | (p[at + 2] << 16) | (static_cast<uint32_t>(p[at + 3]) << 24)); };
    const uint32_t vl = rd(pos); pos += 4;
    c.vendor.assign(p.begin() + static_cast<std::ptrdiff_t>(pos), p.begin() + static_cast<std::ptrdiff_t>(pos + vl)); pos += vl;
    const uint32_t n = rd(pos); pos += 4;
    for (uint32_t i = 0; i < n; i++) {
        const uint32_t l = rd(pos); pos += 4;
        c.list.emplace_back(p.begin() + static_cast<std::ptrdiff_t>(pos), p.begin() + static_cast<std::ptrdiff_t>(pos + l));
        pos += l;
    }
    return c;
}

// the audio packets (all packets behind the three headers)
std::vector<Bytes> audioPackets(const Bytes& f)
{
    auto packets = readPackets(readPages(f));
    packets.erase(packets.begin(), packets.begin() + 3);
    return packets;
}

std::vector<int64_t> audioGranules(const Bytes& f)
{
    std::vector<int64_t> g;
    const auto pages = readPages(f);
    // the granule positions of the pages behind the header pages
    size_t headerPackets = 0, firstAudioPage = 0;
    for (size_t i = 0; i < pages.size() && headerPackets < 3; i++) {
        for (uint8_t l : pages[i].lacing) if (l < 255) headerPackets++;
        firstAudioPage = i + 1;
    }
    for (size_t i = firstAudioPage; i < pages.size(); i++) g.push_back(pages[i].granule);
    return g;
}

}  // namespace

TEST_CASE("Ogg Vorbis: the builder and the checker of the tests agree", "[ogg][spec][selftest]")
{
    const Bytes f = oggFile(OggSpec());
    writeTemp("ogg_selftest.ogg", f);
    checkContainer(f);
    CHECK(parseComments(f).list.size() == 4);
}

TEST_CASE("Ogg Vorbis: reading the identification header, the duration and the comments", "[ogg][spec]")
{
    OggSpec s;
    const Bytes f = oggFile(s);
    auto p = writeTemp("ogg_read.ogg", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(AUDIOGetChannelsW() == 2);
    CHECK(OGGGetBitRateNominalW() == 128);
    // the duration is the granule position of the last page divided by the sample rate: 200 packets * 1000 samples = 200000 samples
    CHECK(std::fabs(AUDIOGetDurationW() - 200000.0 / 44100) < 0.0005);
    CHECK(take(OGGGetTitleW()) == L"Song");
    CHECK(take(OGGGetArtistW()) == L"Band");
    CHECK(take(OGGGetAlbumW()) == L"Record");          // a field name is not case sensitive
    CHECK(take(OGGGetDateW()) == L"2001");
    CHECK(take(OGGGetVendorW()) == wide(s.vendor));
    CHECK(take(OGGGetUserItemW(L"title")) == L"Song");
}

TEST_CASE("Ogg Vorbis: sample rates, channels and long files", "[ogg][spec]")
{
    SECTION("48 kHz, mono") {
        OggSpec s; s.rate = 48000; s.channels = 1; s.samplesPerPacket = 1200;
        auto p = writeTemp("ogg_mono.ogg", oggFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(AUDIOGetSampleRateW() == 48000);
        CHECK(AUDIOGetChannelsW() == 1);
        CHECK(std::fabs(AUDIOGetDurationW() - 240000.0 / 48000) < 0.0005);
    }
    SECTION("six channels are a valid stream") {
        OggSpec s; s.channels = 6;
        auto p = writeTemp("ogg_51.ogg", oggFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(AUDIOGetChannelsW() == 6);
        CHECK(AUDIOGetDurationW() > 4.0f);
    }
    SECTION("a granule position of more than 32 bit") {
        OggSpec s; s.lastGranule = 6000000000LL;   // 136054 seconds at 44100 Hz
        auto p = writeTemp("ogg_long.ogg", oggFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(std::fabs(AUDIOGetDurationW() - 6000000000.0 / 44100) < 1.0);
    }
    SECTION("the last page has no granule position (-1): the previous page counts") {
        OggSpec s; s.lastPageWithoutGranule = true;
        auto p = writeTemp("ogg_lastneg.ogg", oggFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(std::fabs(AUDIOGetDurationW() - 200000.0 / 44100) < 0.0005);
    }
}

TEST_CASE("Ogg Vorbis: writing keeps a valid container", "[ogg][spec][write]")
{
    OggSpec s;
    const Bytes f = oggFile(s);
    const auto audioBefore = audioPackets(f);
    const auto granulesBefore = audioGranules(f);
    auto p = writeTemp("ogg_write.ogg", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);

    SECTION("a short change") {
        OGGSetTitleW(L"A new title");
        OGGSetArtistW(L"New artist");
        REQUIRE(OGGSaveChangesW() != 0);
        const Bytes g = readFile(p);
        checkContainer(g);
        CHECK(audioPackets(g) == audioBefore);
        CHECK(audioGranules(g) == granulesBefore);
        const Comments c = parseComments(g);
        CHECK(c.vendor == s.vendor);
        auto has = [&](const char* x) { return std::find(c.list.begin(), c.list.end(), x) != c.list.end(); };
        CHECK(has("TITLE=A new title"));
        CHECK(has("ARTIST=New artist"));
        CHECK(has("album=Record"));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(take(OGGGetTitleW()) == L"A new title");
        CHECK(std::fabs(AUDIOGetDurationW() - 200000.0 / 44100) < 0.0005);
    }
    SECTION("a comment header of more than 64 KB spans several pages") {
        const std::wstring big(100000, L'x');
        OGGSetUserItemW(L"BIGFIELD", big.c_str());
        REQUIRE(OGGSaveChangesW() != 0);
        const Bytes g = readFile(p);
        checkContainer(g);
        CHECK(audioPackets(g) == audioBefore);   // the audio packets are unchanged
        // the audio pages keep their granule positions, their sequence numbers follow the new header pages
        CHECK(audioGranules(g) == granulesBefore);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(take(OGGGetUserItemW(L"BIGFIELD")).size() == 100000);
        CHECK(take(OGGGetTitleW()) == L"Song");
        CHECK(std::fabs(AUDIOGetDurationW() - 200000.0 / 44100) < 0.0005);
        // and back to a short header
        OGGSetUserItemW(L"BIGFIELD", L"");
        REQUIRE(OGGSaveChangesW() != 0);
        const Bytes h = readFile(p);
        checkContainer(h);
        CHECK(audioPackets(h) == audioBefore);
        CHECK(audioGranules(h) == granulesBefore);
    }
    SECTION("removing the tag keeps the vendor and the container") {
        REQUIRE(OGGRemoveTagW() != 0);
        const Bytes g = readFile(p);
        checkContainer(g);
        const Comments c = parseComments(g);
        CHECK(c.vendor == s.vendor);
        CHECK(c.list.empty());
        CHECK(audioPackets(g) == audioBefore);
    }
}

TEST_CASE("Ogg Vorbis: a file with a comment header on several pages", "[ogg][spec][write]")
{
    OggSpec s;
    s.comments.push_back("COVER=" + std::string(90000, 'c'));
    s.setupSize = 6000;
    const Bytes f = oggFile(s);
    checkContainer(f);
    const auto audioBefore = audioPackets(f);
    auto p = writeTemp("ogg_multipage.ogg", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
    CHECK(take(OGGGetUserItemW(L"COVER")).size() == 90000);
    CHECK(take(OGGGetTitleW()) == L"Song");
    OGGSetTitleW(L"Changed");
    REQUIRE(OGGSaveChangesW() != 0);
    const Bytes g = readFile(p);
    checkContainer(g);
    CHECK(audioPackets(g) == audioBefore);
    CHECK(audioGranules(g) == audioGranules(f));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
    CHECK(take(OGGGetTitleW()) == L"Changed");
    CHECK(take(OGGGetUserItemW(L"COVER")).size() == 90000);
}

TEST_CASE("Vorbis comments: field names and several values", "[ogg][spec][comments]")
{
    OggSpec s;
    s.comments = { "ARTIST=First", "artist=Second", "GENRE=Rock", "TITLE=Song" };
    auto p = writeTemp("ogg_fields.ogg", oggFile(s));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
    // several fields with the same name are allowed; the getter returns all values
    const std::wstring artists = take(OGGGetArtistW());
    CHECK(artists.find(L"First") != std::wstring::npos);
    CHECK(artists.find(L"Second") != std::wstring::npos);

    SECTION("setting a value replaces all fields of the name") {
        OGGSetArtistW(L"Only");
        REQUIRE(OGGSaveChangesW() != 0);
        const Comments c = parseComments(readFile(p));
        int n = 0;
        for (const std::string& x : c.list) if (x.size() > 7 && (x.compare(0, 7, "ARTIST=") == 0 || x.compare(0, 7, "artist=") == 0)) n++;
        CHECK(n == 1);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(take(OGGGetArtistW()) == L"Only");
    }
    SECTION("an empty value removes all fields of the name") {
        OGGSetArtistW(L"");
        REQUIRE(OGGSaveChangesW() != 0);
        const Comments c = parseComments(readFile(p));
        for (const std::string& x : c.list) CHECK(x.compare(0, 7, "ARTIST=") != 0);
        for (const std::string& x : c.list) CHECK(x.compare(0, 7, "artist=") != 0);
    }
    SECTION("a field name has the characters $20 to $7D without '='") {
        OGGSetUserItemW(L"BAD=NAME", L"x");
        OGGSetUserItemW(L"", L"x");
        OGGSetUserItemW(L"N\u00E4me", L"x");
        OGGSetUserItemW(L"GOOD NAME", L"kept");
        REQUIRE(OGGSaveChangesW() != 0);
        const Comments c = parseComments(readFile(p));
        for (const std::string& x : c.list) {
            const size_t eq = x.find('=');
            REQUIRE(eq != std::string::npos);
            CHECK(eq >= 1);
            for (size_t i = 0; i < eq; i++) CHECK((static_cast<uint8_t>(x[i]) >= 0x20 && static_cast<uint8_t>(x[i]) <= 0x7D));
        }
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
        CHECK(take(OGGGetUserItemW(L"GOOD NAME")) == L"kept");
    }
    SECTION("the values are UTF-8") {
        OGGSetTitleW(L"Gr\u00FC\u00DFe \u65E5\u672C\u8A9E");
        REQUIRE(OGGSaveChangesW() != 0);
        const Comments c = parseComments(readFile(p));
        bool found = false;
        for (const std::string& x : c.list) if (x == "TITLE=Gr\xC3\xBC\xC3\x9F" "e \xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E") found = true;
        CHECK(found);
    }
}

TEST_CASE("Ogg Vorbis: damaged and foreign files", "[ogg][spec]")
{
    const Bytes f = oggFile(OggSpec());
    SECTION("truncated inside the headers") {
        Bytes t(f.begin(), f.begin() + 200);
        auto p = writeTemp("ogg_trunc.ogg", t);
        AUDIOAnalyzeFileW(p.c_str());   // must not crash
        CHECK(AUDIOGetDurationW() < 1.0f);
    }
    SECTION("an Ogg stream of another codec is not Vorbis") {
        Bytes g = f;
        // the identification header of Vorbis is changed into an Opus header: "OpusHead"
        const auto pages = readPages(g);
        const size_t at = pages[0].offset + 27 + 1;   // the packet starts behind the lacing values
        g[at] = 'O'; g[at + 1] = 'p'; g[at + 2] = 'u'; g[at + 3] = 's'; g[at + 4] = 'H'; g[at + 5] = 'e'; g[at + 6] = 'a';
        auto p = writeTemp("ogg_opus.ogg", g);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != OGGVORBIS);
    }
    SECTION("a comment header with a wrong number of comments") {
        OggSpec s;
        Bytes g = oggFile(s);
        const auto pages = readPages(g);
        // the count of the comments is behind the vendor string: set it far too large; the comments that exist are read
        const size_t pos = pages[1].offset + 27 + pages[1].lacing.size() + 7 + 4 + s.vendor.size();
        g[pos] = 0xFF; g[pos + 1] = 0xFF;
        auto p = writeTemp("ogg_badcount.ogg", g);
        AUDIOAnalyzeFileW(p.c_str());   // must not crash or hang
        SUCCEED();
    }
}

// ---------------------------------------------------------------------------------------------------------------------------
// Ogg Opus (RFC 7845): OpusHead on the first page, OpusTags (Vorbis comments without framing bit) ending a page, the audio behind it;
// the granule positions count samples of 48 kHz, the first pre-skip samples are not played.

namespace {

struct OpusSpec {
    int channels = 2;
    int preSkip = 312;
    uint32_t inputRate = 44100;
    std::string vendor = "libopus 1.3.1";
    std::vector<std::string> comments = { "TITLE=Voice", "ARTIST=Speaker", "ENCODER=Lavc" };
    int audioPackets = 300;         // 20 ms each
    int packetsPerPage = 50;
    int mappingFamily = 0;
};

Bytes opusHead(const OpusSpec& s)
{
    Bytes b;
    put(b, "OpusHead");
    b.push_back(1);
    b.push_back(static_cast<uint8_t>(s.channels));
    b.push_back(static_cast<uint8_t>(s.preSkip & 0xFF)); b.push_back(static_cast<uint8_t>(s.preSkip >> 8));
    le32(b, s.inputRate);
    b.push_back(0); b.push_back(0);   // output gain
    b.push_back(static_cast<uint8_t>(s.mappingFamily));
    if (s.mappingFamily != 0) {
        b.push_back(static_cast<uint8_t>(s.channels));   // streams
        b.push_back(0);                                   // coupled streams
        for (int i = 0; i < s.channels; i++) b.push_back(static_cast<uint8_t>(i));
    }
    return b;
}

Bytes opusTags(const std::string& vendor, const std::vector<std::string>& comments)
{
    Bytes b;
    put(b, "OpusTags");
    le32(b, static_cast<uint32_t>(vendor.size()));
    put(b, vendor.c_str());
    le32(b, static_cast<uint32_t>(comments.size()));
    for (const std::string& c : comments) { le32(b, static_cast<uint32_t>(c.size())); put(b, c.c_str()); }
    return b;
}

Bytes opusFile(const OpusSpec& s)
{
    const uint32_t serial = 0x0BADCAFE;
    Bytes out = paginate({ Packet{ opusHead(s), 0, true } }, serial, 0, true, false);
    put(out, paginate({ Packet{ opusTags(s.vendor, s.comments), 0, true } }, serial, 1, false, false));
    std::vector<Packet> audio;
    int64_t granule = s.preSkip;
    for (int i = 0; i < s.audioPackets; i++) {
        Packet p;
        p.data.assign(80 + (i % 5) * 20, static_cast<uint8_t>(0x50 + (i & 0x1F)));
        granule += 960;
        const bool endOfPage = ((i + 1) % s.packetsPerPage == 0) || i + 1 == s.audioPackets;
        p.granule = endOfPage ? granule : -1;
        p.flushAfter = endOfPage;
        audio.push_back(p);
    }
    put(out, paginate(audio, serial, 2, false, true));
    return out;
}

// all container requirements of an Opus file: checksums, sequence numbers, flags, OpusHead first, OpusTags ends a page
void checkOpusContainer(const Bytes& f)
{
    const auto pages = readPages(f);
    REQUIRE(pages.size() >= 3);
    size_t total = 0;
    for (const PageInfo& p : pages) total += p.length;
    CHECK(total == f.size());
    for (size_t i = 0; i < pages.size(); i++) {
        INFO("page " << i);
        CHECK(pages[i].crcOk);
        CHECK(pages[i].seq == i);
        CHECK(((pages[i].flags & 2) != 0) == (i == 0));
        CHECK(((pages[i].flags & 4) != 0) == (i + 1 == pages.size()));
    }
    const auto packets = readPackets(pages);
    REQUIRE(packets.size() >= 3);
    CHECK(std::memcmp(packets[0].data(), "OpusHead", 8) == 0);
    CHECK(std::memcmp(packets[1].data(), "OpusTags", 8) == 0);
    // the first audio packet starts on a new page
    size_t headerPackets = 0;
    for (size_t i = 0; i < pages.size(); i++)
        for (size_t k = 0; k < pages[i].segments.size(); k++)
            if (pages[i].lacing[k] < 255) {
                headerPackets++;
                if (headerPackets == 2) CHECK(k + 1 == pages[i].segments.size());
            }
}

// the audio packets (everything behind OpusHead and OpusTags)
std::vector<Bytes> opusAudio(const Bytes& f)
{
    auto packets = readPackets(readPages(f));
    packets.erase(packets.begin(), packets.begin() + 2);
    return packets;
}

Comments parseOpusComments(const Bytes& f)
{
    const auto packets = readPackets(readPages(f));
    const Bytes& p = packets.at(1);
    Comments c;
    size_t pos = 8;
    auto rd = [&](size_t at) { return static_cast<uint32_t>(p[at] | (p[at + 1] << 8) | (p[at + 2] << 16) | (static_cast<uint32_t>(p[at + 3]) << 24)); };
    const uint32_t vl = rd(pos); pos += 4;
    c.vendor.assign(p.begin() + static_cast<std::ptrdiff_t>(pos), p.begin() + static_cast<std::ptrdiff_t>(pos + vl)); pos += vl;
    const uint32_t n = rd(pos); pos += 4;
    for (uint32_t i = 0; i < n; i++) {
        const uint32_t l = rd(pos); pos += 4;
        c.list.emplace_back(p.begin() + static_cast<std::ptrdiff_t>(pos), p.begin() + static_cast<std::ptrdiff_t>(pos + l));
        pos += l;
    }
    return c;
}

}  // namespace

TEST_CASE("Ogg Opus: the builder and the checker of the tests agree", "[ogg][opus][spec][selftest]")
{
    const Bytes f = opusFile(OpusSpec());
    checkOpusContainer(f);
    CHECK(parseOpusComments(f).list.size() == 3);
}

TEST_CASE("Ogg Opus: format, sample rate, channels, duration without pre-skip and comments", "[ogg][opus][spec]")
{
    OpusSpec s;
    auto p = writeTemp("opus_read.ogg", opusFile(s));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGOPUS);
    CHECK(AUDIOGetSampleRateW() == 48000);       // Opus is always decoded with 48 kHz, the input rate of the header is only information
    CHECK(AUDIOGetChannelsW() == 2);
    CHECK(std::fabs(AUDIOGetDurationW() - 300 * 960.0 / 48000) < 0.0005);   // 6 s, the pre-skip is not counted
    CHECK(AUDIOGetBitrateW() > 0);
    CHECK(take(OGGGetTitleW()) == L"Voice");
    CHECK(take(OGGGetArtistW()) == L"Speaker");
    CHECK(take(OGGGetVendorW()) == L"libopus 1.3.1");
    CHECK(take(AUDIOGetTitleW()) == L"Voice");
}

TEST_CASE("Ogg Opus: mono, six channels (mapping family 1) and other pre-skip values", "[ogg][opus][spec]")
{
    OpusSpec s; s.channels = 1; s.preSkip = 3840;
    auto p = writeTemp("opus_mono.ogg", opusFile(s));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGOPUS);
    CHECK(AUDIOGetChannelsW() == 1);
    CHECK(std::fabs(AUDIOGetDurationW() - 6.0) < 0.0005);
    OpusSpec m; m.channels = 6; m.mappingFamily = 1;
    p = writeTemp("opus_51.ogg", opusFile(m));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGOPUS);
    CHECK(AUDIOGetChannelsW() == 6);
}

TEST_CASE("Ogg Opus: writing tags keeps the header and the audio, pages are numbered and checksummed", "[ogg][opus][spec][write]")
{
    OpusSpec s;
    const Bytes f = opusFile(s);
    const auto audio0 = opusAudio(f);
    auto p = writeTemp("opus_write.ogg", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGOPUS);
    SECTION("a shorter and a longer tag") {
        OGGSetTitleW(L"A");
        REQUIRE(OGGSaveChangesW() != 0);
        Bytes g = readFile(p);
        checkOpusContainer(g);
        CHECK(readPackets(readPages(g)).at(0) == readPackets(readPages(f)).at(0));
        CHECK(opusAudio(g) == audio0);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGOPUS);
        CHECK(take(OGGGetTitleW()) == L"A");
        // a comment header over several pages (a cover as a Vorbis comment is a long base64 text)
        const std::wstring big(70000, L'b');
        OGGSetUserItemW(L"METADATA_BLOCK_PICTURE", big.c_str());
        REQUIRE(OGGSaveChangesW() != 0);
        g = readFile(p);
        checkOpusContainer(g);
        CHECK(opusAudio(g) == audio0);
        CHECK(parseOpusComments(g).vendor == "libopus 1.3.1");
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == OGGOPUS);
        CHECK(std::fabs(AUDIOGetDurationW() - 6.0) < 0.0005);
        CHECK(take(OGGGetUserItemW(L"METADATA_BLOCK_PICTURE")).size() == 70000);
    }
    SECTION("the tag is removed, the vendor stays") {
        REQUIRE(OGGRemoveTagW() != 0);
        const Bytes g = readFile(p);
        checkOpusContainer(g);
        const Comments c = parseOpusComments(g);
        CHECK(c.vendor == "libopus 1.3.1");
        CHECK(c.list.empty());
        CHECK(opusAudio(g) == audio0);
    }
    SECTION("the generic function writes too") {
        AUDIOSetTitleW(L"Generic");
        REQUIRE(AUDIOSaveChangesToFileW(p.c_str()) != 0);
        const Bytes g = readFile(p);
        checkOpusContainer(g);
        bool found = false;
        for (const std::string& line : parseOpusComments(g).list) if (line == "TITLE=Generic") found = true;
        CHECK(found);
    }
}

TEST_CASE("Ogg Opus: a cut file is no Vorbis file and a Vorbis file stays one", "[ogg][opus][spec]")
{
    const Bytes f = opusFile(OpusSpec());
    const Bytes cut(f.begin(), f.begin() + 40);   // inside the first page
    auto p = writeTemp("opus_cut.ogg", cut);
    CHECK(AUDIOAnalyzeFileW(p.c_str()) != OGGVORBIS);
    p = writeTemp("vorbis_after.ogg", oggFile(OggSpec()));
    CHECK(AUDIOAnalyzeFileW(p.c_str()) == OGGVORBIS);
}
