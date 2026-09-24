// Gemeinsame Hilfen fuer die ID3v2-Tests (Session, Blob, fetch).
#pragma once
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <algorithm>
#include <cstring>

namespace fs = std::filesystem;

namespace ag3test {

struct Cfg { short format; short enc; const char* name; };   // format: 1=v2.2 2=v2.3 3=v2.4; enc: 0=ISO-8859-1 1=UTF-16 2=UTF-16BE 3=UTF-8
inline const Cfg kAllCfgs[] = {
    { 3, 0, "v2.4 ISO-8859-1" }, { 3, 1, "v2.4 UTF-16" }, { 3, 2, "v2.4 UTF-16BE" }, { 3, 3, "v2.4 UTF-8" },
    { 2, 0, "v2.3 ISO-8859-1" }, { 2, 1, "v2.3 UTF-16" },
    { 1, 0, "v2.2 ISO-8859-1" },
};
inline const Cfg kV24 = { 3, 3, "v2.4 UTF-8" };
inline const Cfg kV23 = { 2, 1, "v2.3 UTF-16" };

inline const std::wstring kLatin1 = L"Grüße äöü éè Test";       // in ISO-8859-1 darstellbar
inline const std::wstring kUnicode = L"Grüße € 日本語 Ж";           // nur mit UTF-Kodierungen

// Frischer MP3-Testfall mit gesetztem Format/Kodierung.
struct Session {
    fs::path path;
    Cfg cfg;
    explicit Session(const Cfg& c, const char* name = "id3v2_rt.mp3") : cfg(c)
    {
        path = writeTemp(name, makeMp3(40));
        REQUIRE(AUDIOAnalyzeFileW(path.c_str()) == MPEG);
        REQUIRE(ID3V2SetFormatAndEncodingW(c.format, c.enc) != 0);
    }
    // speichern und neu einlesen; prueft die Tag-Version in der Datei
    void reload()
    {
        REQUIRE(ID3V2SaveChangesW() != 0);
        const Bytes b = readFile(path);
        REQUIRE(b.size() > 10);
        REQUIRE(std::memcmp(b.data(), "ID3", 3) == 0);
        CHECK(b[3] == static_cast<uint8_t>(cfg.format + 1));            // 1->2, 2->3, 3->4
        REQUIRE(AUDIOAnalyzeFileW(path.c_str()) == MPEG);
    }
};

// Bytes -> BYTE* Hilfen
struct Blob {
    Bytes data;
    explicit Blob(std::initializer_list<uint8_t> l) : data(l) {}
    explicit Blob(size_t n, uint8_t start = 1) { for (size_t i = 0; i < n; i++) data.push_back(static_cast<uint8_t>(start + i * 7)); }
    BYTE* ptr() { return data.data(); }
    u32 len() const { return static_cast<u32>(data.size()); }
};
inline Bytes fetch(long (__stdcall *fn)(BYTE*, u32, short), short index)
{
    Bytes out(4096);
    const long n = fn(out.data(), static_cast<u32>(out.size()), index);
    out.resize(n > 0 ? static_cast<size_t>(n) : 0);
    return out;
}

}  // namespace ag3test
