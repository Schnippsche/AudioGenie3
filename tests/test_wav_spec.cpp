// WAV checks with synthetic files: damaged and streamed 'data' chunks, data behind the RIFF chunk, ID3 tags around it, saving into
// another file and short 'cart' chunks. The audio data must never get lost by a save.
#include "id3v2_support.h"
#include <chrono>
#include <cmath>
#include <cstring>

using namespace ag3test;

namespace {

void put(Bytes& b, const char* s) { while (*s) b.push_back(static_cast<uint8_t>(*s++)); }
void put(Bytes& b, const Bytes& o) { b.insert(b.end(), o.begin(), o.end()); }
void le16(Bytes& b, uint32_t v) { for (int i = 0; i < 2; i++) b.push_back(static_cast<uint8_t>(v >> (8 * i))); }
void le32(Bytes& b, uint32_t v) { for (int i = 0; i < 4; i++) b.push_back(static_cast<uint8_t>(v >> (8 * i))); }

// a chunk; size: the value of the size field (the length of the payload if it is -1)
Bytes chunk(const char* id, const Bytes& payload, int64_t size = -1)
{
    Bytes b;
    put(b, id);
    le32(b, static_cast<uint32_t>(size < 0 ? static_cast<int64_t>(payload.size()) : size));
    put(b, payload);
    if (payload.size() % 2 == 1) b.push_back(0);
    return b;
}

Bytes fmtChunk(uint32_t channels, uint32_t rate)
{
    Bytes p;
    le16(p, 1); le16(p, channels); le32(p, rate); le32(p, rate * channels * 2); le16(p, channels * 2); le16(p, 16);
    return chunk("fmt ", p);
}

Bytes riff(const std::vector<Bytes>& chunks)
{
    Bytes body;
    put(body, "WAVE");
    for (const Bytes& c : chunks) put(body, c);
    Bytes b;
    put(b, "RIFF");
    le32(b, static_cast<uint32_t>(body.size()));
    put(b, body);
    return b;
}

Bytes pcm(size_t bytes, int seed = 0)
{
    Bytes b(bytes);
    for (size_t i = 0; i < bytes; i++) b[i] = static_cast<uint8_t>((i * 7 + static_cast<size_t>(seed)) & 0xFF);
    return b;
}

bool contains(const Bytes& hay, const Bytes& needle)
{
    return std::search(hay.begin(), hay.end(), needle.begin(), needle.end()) != hay.end();
}

// the payload of the 'data' chunk as the size field says (at most up to the end of the file)
Bytes dataPayload(const Bytes& f)
{
    const Bytes id = { 'd', 'a', 't', 'a' };
    auto it = std::search(f.begin(), f.end(), id.begin(), id.end());
    REQUIRE(it != f.end());
    const size_t pos = static_cast<size_t>(it - f.begin());
    const uint32_t size = f[pos + 4] | (f[pos + 5] << 8) | (f[pos + 6] << 16) | (static_cast<uint32_t>(f[pos + 7]) << 24);
    const size_t end = std::min(f.size(), pos + 8 + static_cast<size_t>(size));
    return Bytes(f.begin() + static_cast<std::ptrdiff_t>(pos + 8), f.begin() + static_cast<std::ptrdiff_t>(end));
}

}  // namespace

TEST_CASE("WAV: a 'data' chunk whose size is wrong keeps its audio data on a save", "[wav][spec][write]")
{
    // 1 s of 44.1 kHz stereo; the size field claims more (a cut recording), 0xFFFFFFFF or 0 (written while streaming). Before, the
    // 'data' chunk was ignored and the save wrote the file without the audio data.
    const Bytes audio = pcm(176400);
    struct Case { const char* name; int64_t size; };
    const Case cases[] = { { "larger than the file", 176400 + 100000 }, { "0xFFFFFFFF", 0xFFFFFFFFll }, { "0", 0 } };
    for (const Case& c : cases) {
        INFO(c.name);
        const Bytes f = riff({ fmtChunk(2, 44100), chunk("data", audio, c.size) });
        auto p = writeTemp("wav_datasize.wav", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
        WAVSetTextFrameW(WAV_INAM, L"New title");
        REQUIRE(WAVSaveChangesW() != 0);
        const Bytes g = readFile(p);
        const bool sameAudio = (dataPayload(g) == audio);   // the size field is the real size now
        CHECK(sameAudio);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
        CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"New title");
    }
    SECTION("an empty 'data' chunk followed by another chunk stays empty") {
        Bytes list;
        put(list, "INFO");
        put(list, chunk("INAM", { 'O', 'l', 'd', 0 }));
        const Bytes f = riff({ fmtChunk(2, 44100), chunk("data", {}), chunk("LIST", list) });
        auto p = writeTemp("wav_emptydata.wav", f);
        REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
        CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"Old");
        CHECK(AUDIOGetDurationW() == 0.0f);
    }
}

TEST_CASE("WAV: an ID3v2 tag of odd size in front and an ID3v1 tag behind are kept", "[wav][spec][write]")
{
    // the pad byte behind a chunk of odd size is counted from the chunk, not from the start of the file
    const Bytes audio = pcm(176400);
    Bytes v2 = { 'I', 'D', '3', 3, 0, 0, 0, 0, 0, 0x15 };   // 21 bytes of padding: 31 bytes
    v2.resize(31, 0);
    Bytes v1 = { 'T', 'A', 'G' };
    put(v1, "ID3v1 title");
    v1.resize(128, 0);
    Bytes f = v2;
    put(f, riff({ fmtChunk(2, 44100), chunk("JUNK", { 'a', 'b', 'c' }), chunk("data", audio) }));
    put(f, v1);
    auto p = writeTemp("wav_id3.wav", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
    WAVSetTextFrameW(WAV_INAM, L"New title");
    REQUIRE(WAVSaveChangesW() != 0);
    const Bytes g = readFile(p);
    REQUIRE(g.size() > v2.size() + v1.size());
    const bool v2Kept = std::equal(v2.begin(), v2.end(), g.begin());
    CHECK(v2Kept);
    const bool v1Kept = std::equal(v1.begin(), v1.end(), g.end() - 128);
    CHECK(v1Kept);
    const bool sameAudio = (dataPayload(Bytes(g.begin(), g.end() - 128)) == audio);
    CHECK(sameAudio);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"New title");
    CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
}

TEST_CASE("WAV: data behind the RIFF chunk are kept and read quickly", "[wav][spec][write]")
{
    // 1 MB of zeros behind the chunks: before, every 8 bytes became an empty chunk (about 500 ms) and the save dropped them
    const Bytes audio = pcm(176400);
    Bytes f = riff({ fmtChunk(2, 44100), chunk("data", audio) });
    const size_t riffSize = f.size();
    Bytes tail(1024 * 1024, 0);
    put(tail, "some text behind");
    put(f, tail);
    auto p = writeTemp("wav_tail.wav", f);
    const auto t0 = std::chrono::steady_clock::now();
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    INFO("analysis took " << ms << " ms");
    CHECK(ms < 100.0);
    WAVSetTextFrameW(WAV_INAM, L"New title");
    REQUIRE(WAVSaveChangesW() != 0);
    const Bytes g = readFile(p);
    REQUIRE(g.size() > riffSize);
    const bool tailKept = g.size() >= tail.size() && std::equal(tail.begin(), tail.end(), g.end() - static_cast<std::ptrdiff_t>(tail.size()));
    CHECK(tailKept);
    const bool sameAudio = (dataPayload(g) == audio);
    CHECK(sameAudio);
}

TEST_CASE("WAV: saving into another file keeps its format and its other chunks", "[wav][spec][write]")
{
    // only the tag chunks (LIST INFO, cart, DISP) come from the analyzed file; before, its format chunk was written into the other file
    const Bytes a = riff({ fmtChunk(2, 44100), chunk("data", pcm(176400)) });
    const Bytes otherAudio = pcm(96000, 3);
    const Bytes marker = chunk("cue ", pcm(28, 5));
    const Bytes b = riff({ fmtChunk(1, 48000), marker, chunk("data", otherAudio) });
    auto pa = writeTemp("wav_save_a.wav", a);
    auto pb = writeTemp("wav_save_b.wav", b);
    REQUIRE(AUDIOAnalyzeFileW(pa.c_str()) == WAV);
    WAVSetTextFrameW(WAV_INAM, L"Title of A");
    WAVSetCartChunkEntryW(1, L"Cart title");
    WAVSetDisplayTextW(L"Display text");
    REQUIRE(WAVSaveChangesToFileW(pb.c_str()) != 0);
    const Bytes g = readFile(pb);
    CHECK(contains(g, fmtChunk(1, 48000)));
    CHECK(contains(g, marker));
    const bool sameAudio = (dataPayload(g) == otherAudio);
    CHECK(sameAudio);
    REQUIRE(AUDIOAnalyzeFileW(pb.c_str()) == WAV);
    CHECK(AUDIOGetSampleRateW() == 48000);
    CHECK(AUDIOGetChannelsW() == 1);
    CHECK(std::fabs(AUDIOGetDurationW() - 1.0) < 0.001);
    CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"Title of A");
    CHECK(take(WAVGetCartChunkEntryW(1)) == L"Cart title");
    CHECK(take(WAVGetDisplayTextW()) == L"Display text");
}

TEST_CASE("WAV: a short 'cart' chunk is read only up to its end and keeps its fields", "[wav][spec][write]")
{
    // version, title and the start of the artist (71 bytes instead of 2048): before, the artist was read beyond the end of the chunk and
    // setting another field deleted the title
    Bytes cart;
    put(cart, "0101");
    Bytes title;
    put(title, "Cart title");
    title.resize(64, 0);
    put(cart, title);
    put(cart, "Art");
    const Bytes f = riff({ fmtChunk(2, 44100), chunk("cart", cart), chunk("data", pcm(1000)) });
    auto p = writeTemp("wav_shortcart.wav", f);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetCartChunkEntryW(1)) == L"Cart title");
    CHECK(take(WAVGetCartChunkEntryW(2)) == L"Art");
    CHECK(take(WAVGetCartChunkEntryW(3)) == L"");
    WAVSetCartChunkEntryW(3, L"CUT1");
    CHECK(take(WAVGetCartChunkEntryW(1)) == L"Cart title");
    CHECK(take(WAVGetCartChunkEntryW(2)) == L"Art");
    CHECK(take(WAVGetCartChunkEntryW(3)) == L"CUT1");
    REQUIRE(WAVSaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetCartChunkEntryW(1)) == L"Cart title");
    CHECK(take(WAVGetCartChunkEntryW(3)) == L"CUT1");
}

TEST_CASE("WAV: the MD5 of the audio data is the one of the payload of the 'data' chunk", "[wav][spec][write]")
{
    // before, the whole file was hashed (with the header and the tag chunks): every save of a tag changed it
    const Bytes audio = pcm(176400);
    auto p = writeTemp("wav_md5.wav", riff({ fmtChunk(2, 44100), chunk("data", audio) }));
    auto q = writeTemp("wav_md5_payload.raw", audio);
    const std::wstring payloadHash = take(GetMD5ValueFromFileW(q.c_str()));   // the hash of the whole file: the audio data alone
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    const std::wstring before = take(AUDIOGetMD5ValueW());
    CHECK(before == payloadHash);
    WAVSetTextFrameW(WAV_INAM, L"A title that changes the file");
    REQUIRE(WAVSaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(AUDIOGetMD5ValueW()) == before);
}
