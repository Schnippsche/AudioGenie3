// MP4 (ISO/IEC 14496-12, 14496-14 and the iTunes metadata) checks with synthetic files. The written files are checked with an own
// box reader: sizes, the chunk offsets of all tracks (stco and co64) and the data of the media.
#include "id3v2_support.h"
#include <cmath>
#include <map>
#include <string>

using namespace ag3test;

namespace {

void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
Bytes be16(uint32_t v) { return { static_cast<uint8_t>(v >> 8), static_cast<uint8_t>(v) }; }
Bytes be32(uint32_t v) { return { static_cast<uint8_t>(v >> 24), static_cast<uint8_t>(v >> 16), static_cast<uint8_t>(v >> 8), static_cast<uint8_t>(v) }; }
Bytes be64(uint64_t v) { Bytes b; put(b, be32(static_cast<uint32_t>(v >> 32))); put(b, be32(static_cast<uint32_t>(v))); return b; }
Bytes zeros(size_t n) { return Bytes(n, 0); }
std::wstring wide(const std::string& s) { return std::wstring(s.begin(), s.end()); }

Bytes box(const char* type, const Bytes& payload)
{
    Bytes b = be32(static_cast<uint32_t>(8 + payload.size()));
    put(b, type);
    put(b, payload);
    return b;
}

// a box with an extended size (16 byte header), which is allowed for every box
Bytes box64(const char* type, const Bytes& payload)
{
    Bytes b = be32(1);
    put(b, type);
    put(b, be64(16 + payload.size()));
    put(b, payload);
    return b;
}

Bytes fullBox(const char* type, uint8_t version, uint32_t flags, const Bytes& payload)
{
    Bytes p = { version, static_cast<uint8_t>(flags >> 16), static_cast<uint8_t>(flags >> 8), static_cast<uint8_t>(flags) };
    put(p, payload);
    return box(type, p);
}

Bytes ftyp()
{
    Bytes p;
    put(p, "M4A "); put(p, be32(0)); put(p, "M4A "); put(p, "mp42"); put(p, "isom");
    return box("ftyp", p);
}

Bytes mvhd(uint32_t timescale, uint32_t duration)
{
    Bytes p;
    put(p, be32(0)); put(p, be32(0)); put(p, be32(timescale)); put(p, be32(duration));
    put(p, be32(0x00010000)); put(p, be16(0x0100)); put(p, zeros(10));
    const uint32_t matrix[9] = { 0x00010000, 0, 0, 0, 0x00010000, 0, 0, 0, 0x40000000 };
    for (uint32_t m : matrix) put(p, be32(m));
    put(p, zeros(24)); put(p, be32(3));
    return fullBox("mvhd", 0, 0, p);
}

Bytes mdhd(int version, uint32_t timescale, uint64_t duration)
{
    Bytes p;
    if (version == 0) { put(p, be32(0)); put(p, be32(0)); put(p, be32(timescale)); put(p, be32(static_cast<uint32_t>(duration))); }
    else { put(p, be64(0)); put(p, be64(0)); put(p, be32(timescale)); put(p, be64(duration)); }
    put(p, be16(0x55C4)); put(p, be16(0));
    return fullBox("mdhd", static_cast<uint8_t>(version), 0, p);
}

Bytes hdlr(const char* type, const char* name)
{
    Bytes p;
    put(p, be32(0)); put(p, type); put(p, zeros(12)); put(p, name); p.push_back(0);
    return fullBox("hdlr", 0, 0, p);
}

// sound sample entry 'mp4a': channels, sample size, sample rate (16.16)
Bytes stsd(int channels, uint32_t sampleRate)
{
    Bytes e;
    put(e, zeros(6)); put(e, be16(1));                       // reserved, data reference index
    put(e, be16(0)); put(e, be16(0)); put(e, be32(0));        // version, revision, vendor
    put(e, be16(static_cast<uint32_t>(channels))); put(e, be16(16)); put(e, be16(0)); put(e, be16(0));
    put(e, be32(sampleRate << 16));
    Bytes p = be32(1);
    put(p, box("mp4a", e));
    return fullBox("stsd", 0, 0, p);
}

struct Track {
    int channels = 2;
    uint32_t sampleRate = 44100;
    uint32_t timescale = 44100;
    uint64_t duration = 441000;
    int mdhdVersion = 0;
    bool co64 = false;
    std::vector<size_t> chunkSizes = { 4000, 3000, 5000 };
    uint8_t marker = 0x10;    // the first byte of chunk k is marker + k
};

// the chunks of all tracks are in the mdat one behind the other; offsets are relative to the beginning of the mdat payload
Bytes trak(const Track& t, uint64_t mdatPayloadPos, uint64_t firstChunkInMdat)
{
    Bytes stco;
    uint64_t at = firstChunkInMdat;
    Bytes offsets = be32(static_cast<uint32_t>(t.chunkSizes.size()));
    for (size_t s : t.chunkSizes) {
        if (t.co64) put(offsets, be64(mdatPayloadPos + at)); else put(offsets, be32(static_cast<uint32_t>(mdatPayloadPos + at)));
        at += s;
    }
    Bytes stbl;
    put(stbl, stsd(t.channels, t.sampleRate));
    put(stbl, fullBox("stts", 0, 0, [&] { Bytes p = be32(1); put(p, be32(static_cast<uint32_t>(t.chunkSizes.size()))); put(p, be32(1024)); return p; }()));
    put(stbl, fullBox("stsc", 0, 0, [&] { Bytes p = be32(1); put(p, be32(1)); put(p, be32(1)); put(p, be32(1)); return p; }()));
    put(stbl, fullBox("stsz", 0, 0, [&] { Bytes p = be32(0); put(p, be32(static_cast<uint32_t>(t.chunkSizes.size()))); return p; }()));
    put(stbl, fullBox(t.co64 ? "co64" : "stco", 0, 0, offsets));
    Bytes minf;
    put(minf, fullBox("smhd", 0, 0, zeros(4)));
    put(minf, box("stbl", stbl));
    Bytes mdia;
    put(mdia, mdhd(t.mdhdVersion, t.timescale, t.duration));
    put(mdia, hdlr("soun", "SoundHandler"));
    put(mdia, box("minf", minf));
    Bytes tk;
    put(tk, fullBox("tkhd", 0, 7, zeros(80)));
    put(tk, box("mdia", mdia));
    return box("trak", tk);
}

Bytes dataBox(uint32_t type, const Bytes& payload)
{
    Bytes p = be32(type); put(p, be32(0)); put(p, payload);
    return box("data", p);
}
Bytes textItem(const char* id, const std::string& value)
{
    Bytes v; put(v, value.c_str());
    return box(id, dataBox(1, v));
}
Bytes freeform(const std::string& mean, const std::string& name, const std::string& value)
{
    Bytes m = be32(0); put(m, mean.c_str());
    Bytes n = be32(0); put(n, name.c_str());
    Bytes v; put(v, value.c_str());
    Bytes p = box("mean", m); put(p, box("name", n)); put(p, dataBox(1, v));
    return box("----", p);
}

Bytes udta(const std::vector<Bytes>& items)
{
    Bytes ilst;
    for (const Bytes& i : items) put(ilst, i);
    Bytes meta = be32(0);   // version and flags of the meta box
    put(meta, hdlr("mdir", ""));
    put(meta, box("ilst", ilst));
    return box("udta", box("meta", meta));
}

struct Mp4 {
    std::vector<Track> tracks = { Track() };
    std::vector<Bytes> items = { textItem("\xA9" "nam", "Song"), textItem("\xA9" "ART", "Band") };
    bool moovLast = false;        // moov behind the mdat
    bool extendedMdat = false;    // mdat with an extended size
    bool sizeZeroMdat = false;    // the size of the mdat is 0: up to the end of the file
    size_t padding = 0;           // a free box in front of the mdat
    uint32_t movieTimescale = 1000;
    uint32_t movieDuration = 10000;
};

// the chunk data: chunk k of track t starts with the marker, the rest is a pattern
Bytes mediaData(const Mp4& m)
{
    Bytes d;
    for (const Track& t : m.tracks)
        for (size_t k = 0; k < t.chunkSizes.size(); k++)
            for (size_t i = 0; i < t.chunkSizes[k]; i++) d.push_back(i == 0 ? static_cast<uint8_t>(t.marker + k) : static_cast<uint8_t>((i * 7 + k) & 0xFF));
    return d;
}

Bytes moovOf(const Mp4& m, uint64_t mdatPayloadPos)
{
    Bytes body = mvhd(m.movieTimescale, m.movieDuration);
    uint64_t at = 0;
    for (const Track& t : m.tracks) {
        put(body, trak(t, mdatPayloadPos, at));
        for (size_t s : t.chunkSizes) at += s;
    }
    put(body, udta(m.items));
    return box("moov", body);
}

Bytes mp4File(const Mp4& m)
{
    const Bytes media = mediaData(m);
    const size_t mdatHeader = m.extendedMdat ? 16 : 8;
    Bytes mdat;
    if (m.sizeZeroMdat) { mdat = be32(0); put(mdat, "mdat"); }
    else mdat = m.extendedMdat ? box64("mdat", media) : box("mdat", media);
    if (m.sizeZeroMdat) put(mdat, media);
    Bytes f = ftyp();
    if (!m.moovLast) {
        const size_t moovSize = moovOf(m, 0).size();
        const uint64_t mdatPayloadPos = f.size() + moovSize + (m.padding ? m.padding : 0) + mdatHeader;
        put(f, moovOf(m, mdatPayloadPos));
        if (m.padding) put(f, box("free", zeros(m.padding - 8)));
        put(f, mdat);
    } else {
        const uint64_t mdatPayloadPos = f.size() + mdatHeader;
        put(f, mdat);
        put(f, moovOf(m, mdatPayloadPos));
    }
    return f;
}

// ---- box reader
struct Box { std::string type; uint64_t offset, headerSize, size; };

std::vector<Box> readBoxes(const Bytes& f, uint64_t from, uint64_t to)
{
    std::vector<Box> out;
    uint64_t pos = from;
    auto rd32 = [&](uint64_t p) { return (static_cast<uint32_t>(f[p]) << 24) | (static_cast<uint32_t>(f[p + 1]) << 16) | (static_cast<uint32_t>(f[p + 2]) << 8) | f[p + 3]; };
    while (pos + 8 <= to) {
        Box b;
        b.offset = pos;
        uint64_t size = rd32(pos);
        b.type.assign(reinterpret_cast<const char*>(&f[pos + 4]), 4);
        b.headerSize = 8;
        if (size == 1) { size = (static_cast<uint64_t>(rd32(pos + 8)) << 32) | rd32(pos + 12); b.headerSize = 16; }
        else if (size == 0) size = to - pos;
        b.size = size;
        if (size < b.headerSize || pos + size > to) break;
        out.push_back(b);
        pos += size;
    }
    return out;
}

const Box* find(const std::vector<Box>& v, const char* type)
{
    for (const Box& b : v) if (b.type == type) return &b;
    return nullptr;
}

// all chunk offsets of all tracks: (stco or co64)
std::vector<std::vector<uint64_t>> chunkOffsets(const Bytes& f)
{
    std::vector<std::vector<uint64_t>> result;
    auto top = readBoxes(f, 0, f.size());
    const Box* moov = find(top, "moov");
    if (!moov) return result;
    auto rd32 = [&](uint64_t p) { return (static_cast<uint32_t>(f[p]) << 24) | (static_cast<uint32_t>(f[p + 1]) << 16) | (static_cast<uint32_t>(f[p + 2]) << 8) | f[p + 3]; };
    for (const Box& t : readBoxes(f, moov->offset + moov->headerSize, moov->offset + moov->size)) {
        if (t.type != "trak") continue;
        const Box* cur = &t;
        const char* path[] = { "mdia", "minf", "stbl" };
        std::vector<Box> level = readBoxes(f, cur->offset + cur->headerSize, cur->offset + cur->size);
        bool ok = true;
        Box holder = t;
        for (const char* step : path) {
            const Box* b = find(level, step);
            if (!b) { ok = false; break; }
            holder = *b;
            level = readBoxes(f, b->offset + b->headerSize, b->offset + b->size);
        }
        if (!ok) continue;
        for (const Box& b : level) {
            if (b.type != "stco" && b.type != "co64") continue;
            std::vector<uint64_t> offsets;
            const uint32_t n = rd32(b.offset + b.headerSize + 4);
            for (uint32_t i = 0; i < n; i++) {
                if (b.type == "stco") offsets.push_back(rd32(b.offset + b.headerSize + 8 + 4ull * i));
                else offsets.push_back((static_cast<uint64_t>(rd32(b.offset + b.headerSize + 8 + 8ull * i)) << 32) | rd32(b.offset + b.headerSize + 12 + 8ull * i));
            }
            result.push_back(offsets);
        }
    }
    return result;
}

// the media data of the file: the payload of the mdat
Bytes mdatPayload(const Bytes& f)
{
    auto top = readBoxes(f, 0, f.size());
    const Box* m = find(top, "mdat");
    REQUIRE(m != nullptr);
    return Bytes(f.begin() + static_cast<std::ptrdiff_t>(m->offset + m->headerSize), f.begin() + static_cast<std::ptrdiff_t>(m->offset + m->size));
}

// every chunk offset has to point to the first byte of its chunk (the marker) in the file
void checkChunks(const Bytes& f, const Mp4& m)
{
    const auto all = chunkOffsets(f);
    REQUIRE(all.size() == m.tracks.size());
    for (size_t t = 0; t < m.tracks.size(); t++) {
        REQUIRE(all[t].size() == m.tracks[t].chunkSizes.size());
        for (size_t k = 0; k < all[t].size(); k++) {
            INFO("track " << t << " chunk " << k);
            REQUIRE(all[t][k] < f.size());
            CHECK(f[all[t][k]] == static_cast<uint8_t>(m.tracks[t].marker + k));
            CHECK(f[all[t][k] + 1] == static_cast<uint8_t>((1 * 7 + k) & 0xFF));
        }
    }
}

}  // namespace

TEST_CASE("MP4: the builder and the checker of the tests agree", "[mp4][spec][selftest]")
{
    Mp4 m;
    const Bytes f = mp4File(m);
    checkChunks(f, m);
    CHECK(mdatPayload(f) == mediaData(m));
}

TEST_CASE("MP4: duration, sample rate, channels and bit rate", "[mp4][spec]")
{
    SECTION("a plain stereo file") {
        Mp4 m;
        auto p = writeTemp("mp4_plain.m4a", mp4File(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(std::fabs(AUDIOGetDurationW() - 10.0) < 0.001);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(AUDIOGetBitrateW() == static_cast<long>(12000 * 8.0 / 10.0 / 1000.0 + 0.5));
    }
    SECTION("the sample rate is in the sample entry, the time scale of the media can be another one") {
        Mp4 m;
        m.tracks[0].sampleRate = 48000;
        m.tracks[0].timescale = 90000;
        m.tracks[0].duration = 900000 + 45000;   // 10.5 s
        auto p = writeTemp("mp4_timescale.m4a", mp4File(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(std::fabs(AUDIOGetDurationW() - 10.5) < 0.001);
        CHECK(AUDIOGetSampleRateW() == 48000);
    }
    SECTION("mono and six channels") {
        for (int channels : { 1, 6 }) {
            Mp4 m;
            m.tracks[0].channels = channels;
            auto p = writeTemp("mp4_channels.m4a", mp4File(m));
            REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
            CHECK(AUDIOGetChannelsW() == channels);
        }
    }
    SECTION("the media header with 64 bit values (version 1)") {
        Mp4 m;
        m.tracks[0].mdhdVersion = 1;
        m.tracks[0].duration = 44100ull * 5000;   // 5000 s
        auto p = writeTemp("mp4_mdhd1.m4a", mp4File(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(std::fabs(AUDIOGetDurationW() - 5000.0) < 0.5);
        CHECK(AUDIOGetSampleRateW() == 44100);
    }
    SECTION("two audio tracks: the file is as long as the longest track") {
        Mp4 m;
        m.tracks.push_back(Track());
        m.tracks[1].duration = 441000 / 2;
        m.tracks[1].marker = 0x60;
        auto p = writeTemp("mp4_twotracks.m4a", mp4File(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(std::fabs(AUDIOGetDurationW() - 10.0) < 0.001);
    }
}

TEST_CASE("MP4: boxes with an extended size, a size of 0 and the movie behind the media", "[mp4][spec]")
{
    SECTION("mdat with an extended size") {
        Mp4 m; m.extendedMdat = true;
        auto p = writeTemp("mp4_ext.m4a", mp4File(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(take(MP4GetTextFrameW(MP4_TITLE)) == L"Song");
        CHECK(AUDIOGetSampleRateW() == 44100);
    }
    SECTION("mdat with the size 0 (up to the end of the file)") {
        Mp4 m; m.sizeZeroMdat = true;
        auto p = writeTemp("mp4_size0.m4a", mp4File(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(take(MP4GetTextFrameW(MP4_TITLE)) == L"Song");
    }
    SECTION("moov behind the mdat") {
        Mp4 m; m.moovLast = true;
        auto p = writeTemp("mp4_moovlast.m4a", mp4File(m));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(take(MP4GetTextFrameW(MP4_TITLE)) == L"Song");
        CHECK(std::fabs(AUDIOGetDurationW() - 10.0) < 0.001);
    }
}

TEST_CASE("MP4: metadata items", "[mp4][spec][tags]")
{
    Mp4 m;
    Bytes trkn = { 0, 0, 0x01, 0x2C, 0x07, 0xD0, 0, 0 };       // track 300 of 2000
    Bytes disk = { 0, 0, 0, 2, 0, 3 };                          // disk 2 of 3
    Bytes tmpo = { 0x01, 0x2C };                                // 300 bpm
    Bytes gnre = { 0, 18 };                                     // ID3v1 genre 17 (+1)
    m.items = { textItem("\xA9" "nam", "Gr\xC3\xBC\xC3\x9F" "e"), textItem("\xA9" "alb", "Album"), textItem("\xA9" "day", "2004"),
                box("trkn", dataBox(0, trkn)), box("disk", dataBox(0, disk)), box("tmpo", dataBox(21, tmpo)), box("gnre", dataBox(0, gnre)),
                freeform("com.apple.iTunes", "MYKEY", "value one"), freeform("org.example", "OTHER", "value two") };
    auto p = writeTemp("mp4_items.m4a", mp4File(m));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
    CHECK(take(MP4GetTextFrameW(MP4_TITLE)) == L"Gr\u00FC\u00DF" L"e");
    CHECK(take(MP4GetTextFrameW(MP4_ALBUM)) == L"Album");
    CHECK(take(MP4GetTrackW()) == L"300/2000");
    CHECK(take(MP4GetTextFrameW(MP4_CDSETNUMBER)) == L"2/3");
    CHECK(take(MP4GetTextFrameW(MP4_BEATSPERMINUTE)) == L"300");
    CHECK(take(MP4GetGenreW()) == wide("Rock"));   // ID3v1 genre 17 is "Rock"; 18 - 1 = 17 -> see the genre list
    CHECK(take(MP4GetiTuneFrameW(L"MYKEY")) == L"value one");
    CHECK(take(MP4GetiTuneFrameW(L"OTHER")) == L"value two");   // the meaning of a free form item can be any other than com.apple.iTunes
}

TEST_CASE("MP4: writing tags keeps the media and the chunk offsets of all tracks", "[mp4][spec][write]")
{
    struct Case { const char* name; bool co64; bool moovLast; bool ext; size_t padding; int tracks; };
    const Case cases[] = {
        { "stco, moov in front", false, false, false, 0, 1 },
        { "stco, moov in front, two tracks", false, false, false, 0, 2 },
        { "co64, moov in front, two tracks", true, false, false, 0, 2 },
        { "moov behind the media", false, true, false, 0, 1 },
        { "extended mdat size", false, false, true, 0, 1 },
        { "padding in front of the mdat", false, false, false, 4096, 1 },
    };
    for (const Case& c : cases) {
        INFO(c.name);
        Mp4 m;
        m.moovLast = c.moovLast; m.extendedMdat = c.ext; m.padding = c.padding;
        m.tracks[0].co64 = c.co64;
        if (c.tracks == 2) { m.tracks.push_back(Track()); m.tracks[1].co64 = c.co64; m.tracks[1].marker = 0x60; m.tracks[1].chunkSizes = { 2500, 2500 }; }
        const Bytes f = mp4File(m);
        checkChunks(f, m);
        const Bytes media = mediaData(m);
        auto p = writeTemp("mp4_write.m4a", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);

                MP4SetTextFrameW(MP4_TITLE, L"A much longer title than before, so that the movie box grows and the media data have to move");
        MP4SetTextFrameW(MP4_COMMENT, wide(std::string(5000, 'c')).c_str());
        REQUIRE(MP4SaveChangesW() != 0);
        const Bytes g = readFile(p);
        CHECK(mdatPayload(g) == media);
        checkChunks(g, m);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
        CHECK(take(MP4GetTextFrameW(MP4_TITLE)).substr(0, 15) == L"A much longer t");
        CHECK(take(MP4GetTextFrameW(MP4_COMMENT)).size() == 5000);
        CHECK(std::fabs(AUDIOGetDurationW() - 10.0) < 0.001);

        // and shorter again
        MP4SetTextFrameW(MP4_COMMENT, L"");
        MP4SetTextFrameW(MP4_TITLE, L"S");
        REQUIRE(MP4SaveChangesW() != 0);
        const Bytes h = readFile(p);
        CHECK(mdatPayload(h) == media);
        checkChunks(h, m);
    }
}

TEST_CASE("MP4: track and disk numbers above 255, tempo above 255", "[mp4][spec][write]")
{
    Mp4 m;
    auto p = writeTemp("mp4_numbers.m4a", mp4File(m));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
    MP4SetTrackW(L"300/2000");
    MP4SetTextFrameW(MP4_CDSETNUMBER, L"12/300");
    MP4SetTextFrameW(MP4_BEATSPERMINUTE, L"300");
    REQUIRE(MP4SaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
    CHECK(take(MP4GetTrackW()) == L"300/2000");
    CHECK(take(MP4GetTextFrameW(MP4_CDSETNUMBER)) == L"12/300");
    CHECK(take(MP4GetTextFrameW(MP4_BEATSPERMINUTE)) == L"300");
    // the bytes as the specification defines them: trkn 8 bytes (2 reserved, track, total, 2 reserved), disk 6 bytes
    const Bytes f = readFile(p);
    Bytes want = { 0, 0, 0x01, 0x2C, 0x07, 0xD0, 0, 0 };
    CHECK(std::search(f.begin(), f.end(), want.begin(), want.end()) != f.end());
    Bytes disk = { 0, 0, 0, 12, 0x01, 0x2C };
    CHECK(std::search(f.begin(), f.end(), disk.begin(), disk.end()) != f.end());
}

TEST_CASE("MP4: free form items keep their meaning", "[mp4][spec][write]")
{
    Mp4 m;
    m.items = { textItem("\xA9" "nam", "Song"), freeform("org.example", "OTHER", "value two") };
    auto p = writeTemp("mp4_freeform.m4a", mp4File(m));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
    MP4SetTextFrameW(MP4_TITLE, L"Changed");
    MP4SetiTuneFrameW(L"NEWKEY", L"new value");
    REQUIRE(MP4SaveChangesW() != 0);
    const Bytes f = readFile(p);
    Bytes mean = be32(0); put(mean, "org.example");
    Bytes wantMean = box("mean", mean);
    CHECK(std::search(f.begin(), f.end(), wantMean.begin(), wantMean.end()) != f.end());   // the meaning of the other item is unchanged
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MP4M4A);
    CHECK(take(MP4GetiTuneFrameW(L"OTHER")) == L"value two");
    CHECK(take(MP4GetiTuneFrameW(L"NEWKEY")) == L"new value");
}

TEST_CASE("MP4: damaged files", "[mp4][spec]")
{
    Mp4 m;
    const Bytes f = mp4File(m);
    SECTION("cut inside the moov box") {
        Bytes t(f.begin(), f.begin() + 200);
        auto p = writeTemp("mp4_cut.m4a", t);
        AUDIOAnalyzeFileW(p.c_str());   // must not crash
        SUCCEED();
    }
    SECTION("a box that reaches beyond its container") {
        Bytes g = f;
        // the size of the ftyp box is followed by the moov box: make the size of the moov box far too large
        auto top = readBoxes(g, 0, g.size());
        const Box* moov = find(top, "moov");
        REQUIRE(moov != nullptr);
        g[moov->offset] = 0x7F;
        auto p = writeTemp("mp4_bigbox.m4a", g);
        AUDIOAnalyzeFileW(p.c_str());   // must not crash or hang
        SUCCEED();
    }
}
