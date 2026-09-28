// FLAC checks with synthetic files (format specification of xiph.org): the STREAMINFO block with all fields, the metadata block headers,
// the frame header with its CRC-8 and the rules for writing (STREAMINFO first, last block flag, padding, blocks of 24 bit length). The
// files that are written are checked with an own reader of the metadata blocks.
#include "id3v2_support.h"
#include <cmath>
#include <map>
#include <string>

using namespace ag3test;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
void be16(Bytes& b, uint32_t v) { b.push_back(static_cast<uint8_t>(v >> 8)); b.push_back(static_cast<uint8_t>(v)); }
void be24(Bytes& b, uint32_t v) { b.push_back(static_cast<uint8_t>(v >> 16)); be16(b, v); }
void be32(Bytes& b, uint32_t v) { be16(b, v >> 16); be16(b, v & 0xFFFF); }
void le32(Bytes& b, uint32_t v) { for (int i = 0; i < 4; i++) b.push_back(static_cast<uint8_t>(v >> (8 * i))); }
std::wstring wide(const std::string& s) { return std::wstring(s.begin(), s.end()); }

// STREAMINFO: 16 + 16 + 24 + 24 + 20 + 3 + 5 + 36 + 128 bits
Bytes streamInfo(uint32_t minBs, uint32_t maxBs, uint32_t minFs, uint32_t maxFs, uint32_t rate, int channels, int bps, uint64_t samples)
{
    Bytes b;
    be16(b, minBs); be16(b, maxBs); be24(b, minFs); be24(b, maxFs);
    const uint64_t packed = (static_cast<uint64_t>(rate) << 44) | (static_cast<uint64_t>(channels - 1) << 41) | (static_cast<uint64_t>(bps - 1) << 36) | samples;
    for (int i = 7; i >= 0; i--) b.push_back(static_cast<uint8_t>(packed >> (8 * i)));
    for (int i = 0; i < 16; i++) b.push_back(static_cast<uint8_t>(0xA0 + i));   // MD5
    return b;
}

Bytes block(int type, bool last, const Bytes& data)
{
    Bytes b;
    b.push_back(static_cast<uint8_t>(type | (last ? 0x80 : 0)));
    be24(b, static_cast<uint32_t>(data.size()));
    put(b, data);
    return b;
}

Bytes vorbisComments(const std::string& vendor, const std::vector<std::string>& list)
{
    Bytes b;
    le32(b, static_cast<uint32_t>(vendor.size())); put(b, vendor.c_str());
    le32(b, static_cast<uint32_t>(list.size()));
    for (const std::string& c : list) { le32(b, static_cast<uint32_t>(c.size())); put(b, c.c_str()); }
    return b;
}

uint8_t crc8(const uint8_t* d, size_t n)
{
    uint8_t crc = 0;
    for (size_t i = 0; i < n; i++) {
        crc ^= d[i];
        for (int k = 0; k < 8; k++) crc = (crc & 0x80) ? static_cast<uint8_t>((crc << 1) ^ 0x07) : static_cast<uint8_t>(crc << 1);
    }
    return crc;
}

// the UTF-8 like coding of the frame or sample number (up to 36 bits, 1 to 7 bytes)
Bytes utf8Number(uint64_t v)
{
    Bytes b;
    if (v < 0x80) { b.push_back(static_cast<uint8_t>(v)); return b; }
    int extra = 1;
    while (extra < 6 && v >= (1ull << (5 * extra + 6))) extra++;
    static const uint8_t LEAD[7] = { 0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    b.push_back(static_cast<uint8_t>(LEAD[extra] | (v >> (6 * extra))));
    for (int i = extra - 1; i >= 0; i--) b.push_back(static_cast<uint8_t>(0x80 | ((v >> (6 * i)) & 0x3F)));
    return b;
}

// a frame: header with CRC-8, then a body of the given size (no real audio: the checks only need the header)
Bytes frame(bool variable, uint64_t number, int blockSizeCode, int blockSizeValue, int channelAssignment, int bodySize)
{
    Bytes h = { 0xFF, static_cast<uint8_t>(0xF8 | (variable ? 1 : 0)) };
    h.push_back(static_cast<uint8_t>((blockSizeCode << 4) | 0));   // sample rate: from the STREAMINFO
    h.push_back(static_cast<uint8_t>((channelAssignment << 4) | 0));   // sample size: from the STREAMINFO
    put(h, utf8Number(number));
    if (blockSizeCode == 6) h.push_back(static_cast<uint8_t>(blockSizeValue - 1));
    if (blockSizeCode == 7) be16(h, static_cast<uint32_t>(blockSizeValue - 1));
    h.push_back(crc8(h.data(), h.size()));
    for (int i = 0; i < bodySize; i++) h.push_back(static_cast<uint8_t>(0x11 + (i * 7 & 0x3F)));   // never a sync code: no 0xFF
    return h;
}

struct FlacSpec {
    uint32_t rate = 44100;
    int channels = 2;
    int bps = 16;
    uint64_t samples = 441000;              // 10 s
    uint32_t blockSize = 4096;
    std::string vendor = "reference libFLAC 1.3.3 20190804";
    std::vector<std::string> comments = { "TITLE=Song", "ARTIST=Band" };
    size_t padding = 2000;                  // payload of the padding block (behind the comment block)
    bool seekTable = false;
    bool application = false;
    bool cueSheet = false;
    size_t frames = 20;
    int frameBody = 300;
    bool variable = false;
    int lastBlockSize = 0;                  // a shorter last frame (code 7)
    size_t id3v2 = 0;                       // an ID3v2 tag of this size in front
};

Bytes flacFile(const FlacSpec& s)
{
    Bytes out;
    if (s.id3v2) {
        put(out, "ID3"); out.push_back(3); out.push_back(0); out.push_back(0);
        const uint32_t n = static_cast<uint32_t>(s.id3v2 - 10);
        out.push_back(static_cast<uint8_t>((n >> 21) & 0x7F)); out.push_back(static_cast<uint8_t>((n >> 14) & 0x7F));
        out.push_back(static_cast<uint8_t>((n >> 7) & 0x7F)); out.push_back(static_cast<uint8_t>(n & 0x7F));
        out.resize(s.id3v2, 0);
    }
    put(out, "fLaC");
    put(out, block(0, false, streamInfo(s.blockSize, s.blockSize, 100, 900, s.rate, s.channels, s.bps, s.samples)));
    put(out, block(4, false, vorbisComments(s.vendor, s.comments)));
    if (s.seekTable) { Bytes st(18 * 3, 0x5A); put(out, block(3, false, st)); }
    if (s.application) { Bytes ap = { 'x', 'M', 'P', '3' }; ap.resize(40, 0x33); put(out, block(2, false, ap)); }
    if (s.cueSheet) { Bytes cs(396, 0x44); put(out, block(5, false, cs)); }
    put(out, block(1, true, Bytes(s.padding, 0)));
    for (size_t i = 0; i < s.frames; i++) {
        const bool last = (i + 1 == s.frames) && s.lastBlockSize > 0;
        const uint64_t number = s.variable ? i * s.blockSize : i;
        put(out, last ? frame(s.variable, number, 7, s.lastBlockSize, 1, s.frameBody) : frame(s.variable, number, 12, 4096, 1, s.frameBody));
    }
    return out;
}

struct Block { int type; bool last; size_t size; size_t offset; };

// the metadata blocks of a file: the offset is the one of the block header
std::vector<Block> readBlocks(const Bytes& f, size_t& audioStart, size_t start = 0)
{
    std::vector<Block> blocks;
    REQUIRE(f.size() >= start + 4);
    REQUIRE(std::memcmp(&f[start], "fLaC", 4) == 0);
    size_t pos = start + 4;
    while (pos + 4 <= f.size()) {
        Block b;
        b.type = f[pos] & 0x7F;
        b.last = (f[pos] & 0x80) != 0;
        b.size = (static_cast<size_t>(f[pos + 1]) << 16) | (static_cast<size_t>(f[pos + 2]) << 8) | f[pos + 3];
        b.offset = pos;
        blocks.push_back(b);
        pos += 4 + b.size;
        if (b.last) break;
    }
    audioStart = pos;
    return blocks;
}

Bytes slice(const Bytes& f, size_t from, size_t to) { return Bytes(f.begin() + static_cast<std::ptrdiff_t>(from), f.begin() + static_cast<std::ptrdiff_t>(to)); }

// all rules of the metadata: STREAMINFO first with 34 bytes, the last flag on the last block only, the padding block is the last one
void checkMetadata(const Bytes& f, const Bytes& original, size_t id3 = 0)
{
    size_t audio = 0, audio0 = 0;
    const auto blocks = readBlocks(f, audio, id3);
    const auto blocks0 = readBlocks(original, audio0, id3);
    REQUIRE(!blocks.empty());
    CHECK(blocks[0].type == 0);
    CHECK(blocks[0].size == 34);
    for (size_t i = 0; i < blocks.size(); i++) CHECK(blocks[i].last == (i + 1 == blocks.size()));
    CHECK(blocks.back().type == 1);
    // the STREAMINFO is unchanged (with the MD5 of the audio data), the audio data are unchanged
    CHECK(slice(f, blocks[0].offset, blocks[0].offset + 38) == slice(original, blocks0[0].offset, blocks0[0].offset + 38));
    CHECK(slice(f, audio, f.size()) == slice(original, audio0, original.size()));
    // blocks that are not written by the library stay as they are
    for (const Block& b0 : blocks0)
        if (b0.type == 2 || b0.type == 3 || b0.type == 5) {
            bool found = false;
            for (const Block& b : blocks)
                if (b.type == b0.type && b.size == b0.size && slice(f, b.offset + 4, b.offset + 4 + b.size) == slice(original, b0.offset + 4, b0.offset + 4 + b0.size)) found = true;
            CHECK(found);
        }
}

std::vector<std::string> commentsOf(const Bytes& f)
{
    size_t audio;
    std::vector<std::string> list;
    for (const Block& b : readBlocks(f, audio)) {
        if (b.type != 4) continue;
        size_t pos = b.offset + 4;
        auto rd = [&](size_t at) { return static_cast<uint32_t>(f[at] | (f[at + 1] << 8) | (f[at + 2] << 16) | (static_cast<uint32_t>(f[at + 3]) << 24)); };
        const uint32_t vl = rd(pos); pos += 4 + vl;
        const uint32_t n = rd(pos); pos += 4;
        for (uint32_t i = 0; i < n; i++) {
            const uint32_t l = rd(pos); pos += 4;
            list.emplace_back(f.begin() + static_cast<std::ptrdiff_t>(pos), f.begin() + static_cast<std::ptrdiff_t>(pos + l));
            pos += l;
        }
    }
    return list;
}

}  // namespace

TEST_CASE("FLAC: the builder and the checker of the tests agree", "[flac][spec][selftest]")
{
    const Bytes f = flacFile(FlacSpec());
    checkMetadata(f, f);
    CHECK(commentsOf(f).size() == 2);
}

TEST_CASE("FLAC: STREAMINFO fields", "[flac][spec]")
{
    SECTION("a standard file") {
        FlacSpec s;
        auto p = writeTemp("flac_read.flac", flacFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(AUDIOGetSampleRateW() == 44100);
        CHECK(AUDIOGetChannelsW() == 2);
        CHECK(FLACGetBitsPerSampleW() == 16);
        CHECK(FLACGetSamplesW() == 441000);
        CHECK(FLACGetMinBlockSizeW() == 4096);
        CHECK(FLACGetMaxBlockSizeW() == 4096);
        CHECK(FLACGetMinFrameSizeW() == 100);
        CHECK(FLACGetMaxFrameSizeW() == 900);
        CHECK(std::fabs(AUDIOGetDurationW() - 10.0) < 0.0005);
        CHECK(take(FLACGetUserItemW(L"title")) == L"Song");
        CHECK(take(FLACGetVendorW()) == L"reference libFLAC 1.3.3 20190804");
    }
    SECTION("the largest values of the fields: 655350 Hz (20 bits), 8 channels, 32 bits") {
        FlacSpec s; s.rate = 655350; s.channels = 8; s.bps = 32; s.samples = 655350ull * 3;
        auto p = writeTemp("flac_max.flac", flacFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(AUDIOGetSampleRateW() == 655350);
        CHECK(AUDIOGetChannelsW() == 8);
        CHECK(FLACGetBitsPerSampleW() == 32);
        CHECK(std::fabs(AUDIOGetDurationW() - 3.0) < 0.0005);
    }
    SECTION("the counter of the samples has 36 bits: more than 4 billion samples") {
        FlacSpec s; s.rate = 192000; s.samples = (1ull << 33) + 12345;   // 12.4 hours at 192 kHz
        auto p = writeTemp("flac_long.flac", flacFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(std::fabs(AUDIOGetDurationW() - static_cast<double>((1ull << 33) + 12345) / 192000) < 0.01);
        CHECK(FLACGetSamplesW() > 0);   // the function has 32 bits: it is limited, not negative
    }
    SECTION("the bit rate is the size of the frames per second") {
        FlacSpec s;
        const Bytes f = flacFile(s);
        auto p = writeTemp("flac_rate.flac", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        size_t audio;
        readBlocks(f, audio);
        CHECK(AUDIOGetBitrateW() == static_cast<long>((f.size() - audio) * 8.0 / 10.0 / 1000.0 + 0.5));
    }
}

TEST_CASE("FLAC: metadata blocks of length 0, block type 127, missing STREAMINFO and truncated metadata", "[flac][spec]")
{
    SECTION("an empty padding block in the middle does not end the metadata") {
        FlacSpec s;
        Bytes f;
        put(f, "fLaC");
        put(f, block(0, false, streamInfo(4096, 4096, 100, 900, 44100, 2, 16, 441000)));
        put(f, block(1, false, Bytes()));   // padding of length 0: allowed
        put(f, block(4, false, vorbisComments("v", { "TITLE=After empty" })));
        put(f, block(1, true, Bytes(500, 0)));
        for (int i = 0; i < 10; i++) put(f, frame(false, static_cast<uint64_t>(i), 12, 4096, 1, 200));
        auto p = writeTemp("flac_empty_block.flac", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(take(FLACGetUserItemW(L"TITLE")) == L"After empty");
        FLACSetUserItemW(L"ARTIST", L"New");
        REQUIRE(FLACSaveChangesW() != 0);
        const Bytes g = readFile(p);
        checkMetadata(g, f);   // the frames start behind the last block: the audio data are intact
    }
    SECTION("the first block has to be the STREAMINFO") {
        Bytes f;
        put(f, "fLaC");
        put(f, block(4, false, vorbisComments("v", { "TITLE=x" })));
        put(f, block(0, true, streamInfo(4096, 4096, 100, 900, 44100, 2, 16, 441000)));
        f.resize(f.size() + 3000, 0x11);
        auto p = writeTemp("flac_no_first_info.flac", f);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != FLAC);
    }
    SECTION("a STREAMINFO with another length than 34 is invalid") {
        Bytes f;
        put(f, "fLaC");
        put(f, block(0, true, Bytes(20, 0)));
        f.resize(f.size() + 3000, 0x11);
        auto p = writeTemp("flac_short_info.flac", f);
        CHECK(AUDIOAnalyzeFileW(p.c_str()) != FLAC);
    }
    SECTION("block type 127 is invalid: the metadata end there and the file is not written") {
        Bytes f;
        put(f, "fLaC");
        put(f, block(0, false, streamInfo(4096, 4096, 100, 900, 44100, 2, 16, 441000)));
        put(f, block(127, true, Bytes(10, 0)));
        f.resize(f.size() + 3000, 0x11);
        auto p = writeTemp("flac_type127.flac", f);
        if (AUDIOAnalyzeFileW(p.c_str()) == FLAC) {
            FLACSetUserItemW(L"TITLE", L"x");
            CHECK(FLACSaveChangesW() == 0);
            CHECK(readFile(p) == f);
        }
    }
    SECTION("the file ends inside the metadata: readable, but it is not written") {
        const Bytes full = flacFile(FlacSpec());
        const Bytes cut = slice(full, 0, 4 + 38 + 20);   // inside the comment block
        auto p = writeTemp("flac_cut_meta.flac", cut);
        AUDIOAnalyzeFileW(p.c_str());
        FLACSetUserItemW(L"TITLE", L"x");
        CHECK(FLACSaveChangesW() == 0);
        CHECK(readFile(p) == cut);
    }
}

TEST_CASE("FLAC: the number of the samples is unknown (0): the end of the last frame gives it", "[flac][spec]")
{
    SECTION("a fixed block size: frame numbers") {
        FlacSpec s; s.samples = 0; s.frames = 12; s.lastBlockSize = 1000;
        auto p = writeTemp("flac_unknown_fixed.flac", flacFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(FLACGetSamplesW() == 11 * 4096 + 1000);
        CHECK(std::fabs(AUDIOGetDurationW() - (11 * 4096 + 1000) / 44100.0) < 0.0005);
    }
    SECTION("a variable block size: sample numbers") {
        FlacSpec s; s.samples = 0; s.frames = 12; s.lastBlockSize = 777; s.variable = true;
        auto p = writeTemp("flac_unknown_variable.flac", flacFile(s));
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(FLACGetSamplesW() == 11 * 4096 + 777);
    }
    SECTION("a frame header with a wrong checksum is not a frame") {
        FlacSpec s; s.samples = 0; s.frames = 12;
        Bytes f = flacFile(s);
        // damage the CRC-8 of the last frame header
        size_t pos = f.size() - static_cast<size_t>(s.frameBody) - 1;
        while (!(f[pos - 1] == 0xFF && (f[pos] & 0xFE) == 0xF8) && pos > 0) pos--;
        f[pos + 4 + 0] ^= 0x01;
        auto p = writeTemp("flac_unknown_badcrc.flac", f);
        AUDIOAnalyzeFileW(p.c_str());
        CHECK(FLACGetSamplesW() != 12 * 4096);   // the damaged frame is skipped: the frame before it is the last one
    }
}

TEST_CASE("FLAC: writing keeps STREAMINFO, the other blocks and the audio; padding fills the old size", "[flac][spec][write]")
{
    FlacSpec s; s.seekTable = true; s.application = true; s.cueSheet = true;
    const Bytes f = flacFile(s);
    auto p = writeTemp("flac_write.flac", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
    SECTION("a change that fits into the padding: the file has the same size") {
        FLACSetUserItemW(L"ARTIST", L"Somebody else");
        REQUIRE(FLACSaveChangesW() != 0);
        const Bytes g = readFile(p);
        CHECK(g.size() == f.size());
        checkMetadata(g, f);
        bool found = false;
        for (const std::string& c : commentsOf(g)) if (c == "ARTIST=Somebody else") found = true;
        CHECK(found);
    }
    SECTION("a change that does not fit: the file is rebuilt") {
        FLACSetUserItemW(L"COMMENT", wide(std::string(9000, 'c')).c_str());
        REQUIRE(FLACSaveChangesW() != 0);
        const Bytes g = readFile(p);
        CHECK(g.size() > f.size());
        checkMetadata(g, f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(take(FLACGetUserItemW(L"COMMENT")).size() == 9000);
    }
    SECTION("every size of the tag up to the padding gives a valid file (a padding of 0 bytes is valid)") {
        for (int extra = 0; extra < 40; extra++) {
            INFO("extra " << extra);
            Bytes h = f;
            auto q = writeTemp("flac_sizes.flac", h);
            REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == FLAC);
            FLACSetUserItemW(L"COMMENT", wide(std::string(1985 + static_cast<size_t>(extra), 'x')).c_str());
            REQUIRE(FLACSaveChangesW() != 0);
            checkMetadata(readFile(q), f);
        }
    }
    SECTION("several saves in a row without analyzing the file again") {
        // the size of the metadata changes with every save: the next one must start from the size the file has now
        const size_t sizes[] = { 9000, 100, 30000, 50, 2500, 2500 };
        for (size_t size : sizes) {
            INFO("size " << size);
            FLACSetUserItemW(L"COMMENT", wide(std::string(size, 'k')).c_str());
            REQUIRE(FLACSaveChangesW() != 0);
            checkMetadata(readFile(p), f);
        }
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(take(FLACGetUserItemW(L"COMMENT")).size() == 2500);
    }
    SECTION("an ID3v2 tag in front is carried along") {
        FlacSpec t = s; t.id3v2 = 300;
        const Bytes with = flacFile(t);
        auto q = writeTemp("flac_id3.flac", with);
        REQUIRE(AUDIOAnalyzeFileW(q.c_str()) == FLAC);
        FLACSetUserItemW(L"COMMENT", wide(std::string(9000, 'c')).c_str());
        REQUIRE(FLACSaveChangesW() != 0);
        const Bytes g = readFile(q);
        CHECK(slice(g, 0, 300) == slice(with, 0, 300));
        checkMetadata(g, with, 300);
    }
}

TEST_CASE("FLAC: padding of more than 24 bit, blocks that do not fit into 24 bit", "[flac][spec][write]")
{
    SECTION("old padding of 2 x 9 MB: the new padding does not fit into one block, the file is rebuilt") {
        FlacSpec s; s.padding = 0;
        Bytes f;
        put(f, "fLaC");
        put(f, block(0, false, streamInfo(4096, 4096, 100, 900, 44100, 2, 16, 441000)));
        put(f, block(4, false, vorbisComments("v", { "TITLE=Big padding" })));
        put(f, block(1, false, Bytes(9u << 20, 0)));
        put(f, block(1, true, Bytes(9u << 20, 0)));
        for (int i = 0; i < 10; i++) put(f, frame(false, static_cast<uint64_t>(i), 12, 4096, 1, 200));
        auto p = writeTemp("flac_bigpad.flac", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        FLACSetUserItemW(L"ARTIST", L"X");
        REQUIRE(FLACSaveChangesW() != 0);
        const Bytes g = readFile(p);
        size_t audio, audio0;
        const auto blocks = readBlocks(g, audio);
        readBlocks(f, audio0);
        CHECK(blocks.back().type == 1);
        CHECK(blocks.back().size <= 0xFFFFFF);
        CHECK(slice(g, audio, g.size()) == slice(f, audio0, f.size()));
    }
    SECTION("a picture of more than 16 MB: saving fails, the file stays as it is") {
        FlacSpec s;
        const Bytes f = flacFile(s);
        auto p = writeTemp("flac_bigpic.flac", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        Bytes png = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
        png.resize(17u << 20, 0x77);
        FLACAddPictureArrayW(png.data(), static_cast<uint32_t>(png.size()), L"big", 3);
        CHECK(FLACSaveChangesW() == 0);
        CHECK(readFile(p) == f);
    }
}

namespace {

struct Picture { int type; std::string mime; std::string description; uint32_t width, height, depth, colors; size_t dataSize; };

// the picture block of the file: the fields of the specification
bool readPicture(const Bytes& f, Picture& pic)
{
    size_t audio;
    for (const Block& b : readBlocks(f, audio)) {
        if (b.type != 6) continue;
        size_t pos = b.offset + 4;
        auto rd = [&](size_t at) { return (static_cast<uint32_t>(f[at]) << 24) | (static_cast<uint32_t>(f[at + 1]) << 16) | (static_cast<uint32_t>(f[at + 2]) << 8) | f[at + 3]; };
        pic.type = static_cast<int>(rd(pos)); pos += 4;
        const uint32_t ml = rd(pos); pos += 4;
        pic.mime.assign(f.begin() + static_cast<std::ptrdiff_t>(pos), f.begin() + static_cast<std::ptrdiff_t>(pos + ml)); pos += ml;
        const uint32_t dl = rd(pos); pos += 4;
        pic.description.assign(f.begin() + static_cast<std::ptrdiff_t>(pos), f.begin() + static_cast<std::ptrdiff_t>(pos + dl)); pos += dl;
        pic.width = rd(pos); pic.height = rd(pos + 4); pic.depth = rd(pos + 8); pic.colors = rd(pos + 12);
        pic.dataSize = rd(pos + 16);
        return pos + 20 + pic.dataSize == b.offset + 4 + b.size;
    }
    return false;
}

Bytes pngHeader(uint32_t w, uint32_t h, int depth, int colorType, size_t paletteEntries = 0)
{
    Bytes b = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    be32(b, 13); put(b, "IHDR"); be32(b, w); be32(b, h);
    b.push_back(static_cast<uint8_t>(depth)); b.push_back(static_cast<uint8_t>(colorType)); b.push_back(0); b.push_back(0); b.push_back(0);
    be32(b, 0);   // crc (not checked)
    if (paletteEntries) { be32(b, static_cast<uint32_t>(paletteEntries * 3)); put(b, "PLTE"); b.resize(b.size() + paletteEntries * 3, 0x10); be32(b, 0); }
    b.resize(b.size() + 100, 0x22);
    return b;
}

}  // namespace

TEST_CASE("FLAC: the picture block has the width, height, depth and colors of the picture", "[flac][spec][write][picture]")
{
    struct Case { const char* name; Bytes data; const char* mime; uint32_t w, h, depth, colors; };
    Bytes gif = { 'G', 'I', 'F', '8', '9', 'a', 0x2C, 0x01, 0x90, 0x00, 0xF7, 0, 0 };   // 300 x 144, global table of 256 colors, 8 bits
    gif.resize(200, 0x33);
    Bytes bmp(200, 0);
    bmp[0] = 'B'; bmp[1] = 'M'; bmp[14] = 40; bmp[18] = 0x2C; bmp[19] = 0x01; bmp[22] = 0x90; bmp[28] = 24;   // 300 x 144, 24 bits
    Bytes bmpTop = bmp; bmpTop[22] = 0x70; bmpTop[23] = 0xFF; bmpTop[24] = 0xFF; bmpTop[25] = 0xFF;             // height -144: from the top
    Bytes jpegProgressive = { 0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 'J', 'F', 'I', 'F', 0, 1, 1, 0, 0, 1, 0, 1, 0, 0,
                              0xFF, 0xC2, 0x00, 0x11, 0x08, 0x00, 0x90, 0x01, 0x2C, 0x03, 1, 0x22, 0, 2, 0x11, 1, 3, 0x11, 1, 0xFF, 0xDA };
    Bytes jpegBaseline = jpegProgressive; jpegBaseline[21] = 0xC0;
    const Case cases[] = {
        { "PNG RGBA", pngHeader(300, 144, 8, 6), "image/png", 300, 144, 32, 0 },
        { "PNG gray 16 bit", pngHeader(300, 144, 16, 0), "image/png", 300, 144, 16, 0 },
        { "PNG palette", pngHeader(300, 144, 8, 3, 17), "image/png", 300, 144, 8, 17 },
        { "GIF", gif, "image/gif", 300, 144, 8, 256 },
        { "BMP", bmp, "image/bmp", 300, 144, 24, 0 },
        { "BMP from the top", bmpTop, "image/bmp", 300, 144, 24, 0 },
        { "JPEG baseline", jpegBaseline, "image/jpeg", 300, 144, 24, 0 },
        { "JPEG progressive", jpegProgressive, "image/jpeg", 300, 144, 24, 0 },
    };
    for (const Case& c : cases) {
        INFO(c.name);
        const Bytes f = flacFile(FlacSpec());
        auto p = writeTemp("flac_pic.flac", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        Bytes data = c.data;
        CHECK(FLACAddPictureArrayW(data.data(), static_cast<uint32_t>(data.size()), L"cover", 3) == 0);   // 0: added, -1: replaced
        REQUIRE(FLACSaveChangesW() != 0);
        const Bytes g = readFile(p);
        checkMetadata(g, f);
        Picture pic;
        REQUIRE(readPicture(g, pic));
        CHECK(pic.type == 3);
        CHECK(pic.mime == c.mime);
        CHECK(pic.description == "cover");
        CHECK(pic.width == c.w);
        CHECK(pic.height == c.h);
        CHECK(pic.depth == c.depth);
        CHECK(pic.colors == c.colors);
        CHECK(pic.dataSize == data.size());
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
        CHECK(FLACGetPictureCountW() == 1);
        CHECK(FLACGetPictureSizeW(1) == static_cast<long>(data.size()));
    }
}

TEST_CASE("FLAC: a Vorbis comment block does not read into the next block", "[flac][spec]")
{
    // the count of the comments says 50, the block only has 2: the following picture block must still be read
    Bytes vc = vorbisComments("v", { "TITLE=One", "ARTIST=Two" });
    vc[5] = 50;   // the count at offset 4 + 1 (vendor of length 1): bytes 5..8
    Bytes f;
    put(f, "fLaC");
    put(f, block(0, false, streamInfo(4096, 4096, 100, 900, 44100, 2, 16, 441000)));
    put(f, block(4, false, vc));
    Bytes pic;
    be32(pic, 3); be32(pic, 9); put(pic, "image/png"); be32(pic, 1); put(pic, "d");
    be32(pic, 1); be32(pic, 2); be32(pic, 24); be32(pic, 0);
    const Bytes png = pngHeader(1, 2, 8, 2);
    be32(pic, static_cast<uint32_t>(png.size())); put(pic, png);
    put(f, block(6, false, pic));
    put(f, block(1, true, Bytes(100, 0)));
    for (int i = 0; i < 10; i++) put(f, frame(false, static_cast<uint64_t>(i), 12, 4096, 1, 200));
    auto p = writeTemp("flac_short_comment.flac", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
    CHECK(take(FLACGetUserItemW(L"TITLE")) == L"One");
    CHECK(take(FLACGetUserItemW(L"ARTIST")) == L"Two");
    CHECK(FLACGetPictureCountW() == 1);
    CHECK(std::fabs(AUDIOGetDurationW() - 10.0) < 0.0005);
}

TEST_CASE("FLAC: comment fields larger than the text buffer are not lost when other fields are written", "[flac][spec][write]")
{
    // MAXTEXTBUFFER (256 KB) limits the text that the getters return; the field itself has to stay in the file as it was
    const std::string big(400000, 'L');
    const std::string medium(70000, 'M');
    FlacSpec s;
    s.comments = { "TITLE=Song", "COMMENT=" + big, "LYRICS=" + medium, "ARTIST=Band" };
    s.padding = 0;
    const Bytes f = flacFile(s);
    auto p = writeTemp("flac_big_fields.flac", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
    FLACSetUserItemW(L"TITLE", L"Changed");
    REQUIRE(FLACSaveChangesW() != 0);
    const Bytes g = readFile(p);
    checkMetadata(g, f);
    const auto list = commentsOf(g);
    REQUIRE(list.size() == 4);
    CHECK(list[0] == "TITLE=Changed");
    CHECK(list[1] == "COMMENT=" + big);
    CHECK(list[2] == "LYRICS=" + medium);
    CHECK(list[3] == "ARTIST=Band");
    // a field that is set is replaced
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == FLAC);
    FLACSetUserItemW(L"COMMENT", L"short");
    REQUIRE(FLACSaveChangesW() != 0);
    bool found = false;
    for (const std::string& c : commentsOf(readFile(p))) if (c == "COMMENT=short") found = true;
    CHECK(found);
}
