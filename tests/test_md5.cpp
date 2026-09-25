// MD5 of files: known values, comparison with the Windows implementation for many sizes (block and buffer boundaries).
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <bcrypt.h>
#include <string>

#pragma comment(lib, "bcrypt.lib")

using namespace ag3test;

namespace {

// MD5 with the CNG of Windows as independent reference
std::wstring referenceMd5(const Bytes& data)
{
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    REQUIRE(BCryptOpenAlgorithmProvider(&alg, BCRYPT_MD5_ALGORITHM, nullptr, 0) == 0);
    REQUIRE(BCryptCreateHash(alg, &hash, nullptr, 0, nullptr, 0, 0) == 0);
    if (!data.empty())
        REQUIRE(BCryptHashData(hash, const_cast<PUCHAR>(data.data()), (ULONG)data.size(), 0) == 0);
    UCHAR digest[16];
    REQUIRE(BCryptFinishHash(hash, digest, sizeof digest, 0) == 0);
    BCryptDestroyHash(hash);
    BCryptCloseAlgorithmProvider(alg, 0);
    static const wchar_t* hex = L"0123456789abcdef";
    std::wstring s;
    for (UCHAR b : digest) { s += hex[b >> 4]; s += hex[b & 15]; }
    return s;
}

Bytes pseudoRandom(size_t n, unsigned seed)
{
    Bytes b(n);
    for (auto& x : b) { seed = seed * 1664525u + 1013904223u; x = (uint8_t)(seed >> 24); }
    return b;
}

Bytes text(const char* s) { return Bytes(s, s + strlen(s)); }

}  // namespace

TEST_CASE("MD5: test values of RFC 1321", "[md5]")
{
    struct V { const char* text; const wchar_t* md5; };
    const V values[] = {
        { "a", L"0cc175b9c0f1b6a831c399e269772661" },
        { "abc", L"900150983cd24fb0d6963f7d28e17f72" },
        { "message digest", L"f96b697d7cb7938d525a2f31aaf161d0" },
        { "abcdefghijklmnopqrstuvwxyz", L"c3fcd3d76192e4007dfb496cca67e13b" },
        { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", L"d174ab98d277d9f5a5611c2c9f419d9f" },
        { "12345678901234567890123456789012345678901234567890123456789012345678901234567890", L"57edf4a22be3c955ac49da2e2107b67a" },
    };
    for (const V& v : values) {
        auto p = writeTemp("md5_vector.bin", text(v.text));
        CHECK(take(GetMD5ValueFromFileW(p.c_str())) == v.md5);
    }
    // one million times 'a'
    auto p = writeTemp("md5_million.bin", Bytes(1000000, 'a'));
    CHECK(take(GetMD5ValueFromFileW(p.c_str())) == L"7707d6ae4e027c70eea2a935c2296f21");
}

TEST_CASE("MD5: same result as the Windows implementation for many sizes", "[md5]")
{
    // sizes around the 64 byte MD5 blocks and the read buffer (16 KB, 64 KB, 256 KB)
    const size_t sizes[] = { 1, 2, 55, 56, 57, 63, 64, 65, 119, 120, 127, 128, 129, 1000, 4095, 4096, 4097, 16383, 16384, 16385, 32768,
                             65535, 65536, 65537, 65599, 65600, 65601, 131072, 262143, 262144, 262145, 1000003, 3 * 65536 + 17 };
    for (size_t n : sizes) {
        DYNAMIC_SECTION(n << " bytes") {
            const Bytes data = pseudoRandom(n, (unsigned)n);
            auto p = writeTemp("md5_size.bin", data);
            CHECK(take(GetMD5ValueFromFileW(p.c_str())) == referenceMd5(data));
        }
    }
}

TEST_CASE("MD5: hash of the audio data does not depend on the tags", "[md5][mpeg]")
{
    const Bytes audio = makeMp3(300);
    auto p = writeTemp("md5_audio.mp3", audio);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    const std::wstring plain = take(AUDIOGetMD5ValueW());
    REQUIRE(plain.size() == 32);
    CHECK(plain == referenceMd5(audio));   // without tags the audio data is the whole file

    // other tests leave settings in the DLL (single instance); write a defined ID3v2 tag
    REQUIRE(ID3V2SetFormatAndEncodingW(2, 1) != 0);   // v2.3, UTF-16
    ID3V2SetTextFrameW(ID3F_TIT2, L"a title");
    REQUIRE(ID3V2SaveChangesW() != 0);
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    ID3V1SetTitleW(L"v1 title");
    REQUIRE(ID3V1SaveChangesW() != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == MPEG);
    const Bytes tagged = readFile(p);
    const size_t v2 = id3v2TotalSize(tagged);
    INFO("ID3v2 tag " << v2 << " bytes, file " << tagged.size() << " bytes, audio " << audio.size() << " bytes");
    REQUIRE(v2 > 0);
    REQUIRE(tagged.size() == v2 + audio.size() + 128);   // ID3v2 + audio + ID3v1
    CHECK(take(AUDIOGetMD5ValueW()) == plain);
    CHECK(take(GetMD5ValueFromFileW(p.c_str())) != plain);   // the whole file differs
}
