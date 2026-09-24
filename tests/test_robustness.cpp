// Kaputte Eingaben duerfen die DLL nicht zum Absturz bringen (Absturz = Prozessabbruch = Testfehler).
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"
#include <algorithm>

using namespace ag3test;

static void analyzeAndReadAll(const std::filesystem::path& p)
{
    AUDIOAnalyzeFileW(p.c_str());
    AUDIOGetDurationW(); AUDIOGetBitrateW(); AUDIOGetChannelsW(); AUDIOGetSampleRateW();
    take(AUDIOGetTitleW());
    take(AUDIOGetVersionW());
}

TEST_CASE("Nicht existierende Datei und Datei ohne Audio", "[robust]")
{
    CHECK(AUDIOAnalyzeFileW(L"Z:\\gibt\\es\\nicht.mp3") == UNKNOWN);
    auto p = writeTemp("leer.bin", Bytes{});
    CHECK(AUDIOAnalyzeFileW(p.c_str()) == UNKNOWN);
    auto q = writeTemp("text.mp3", Bytes{ 'h', 'a', 'l', 'l', 'o' });
    CHECK(AUDIOAnalyzeFileW(q.c_str()) == UNKNOWN);
}

TEST_CASE("Abgeschnittene Dateien", "[robust]")
{
    const Bytes wav = makeWav(44100, 2, 0.1);
    const Bytes mp3 = makeMp3(20);
    for (size_t len = 0; len <= 100; len += 7) {
        analyzeAndReadAll(writeTemp("trunc.wav", Bytes(wav.begin(), wav.begin() + std::min(len, wav.size()))));
        analyzeAndReadAll(writeTemp("trunc.mp3", Bytes(mp3.begin(), mp3.begin() + std::min(len, mp3.size()))));
    }
    SUCCEED("kein Absturz");
}

TEST_CASE("Zufaellig beschaedigte Dateien (deterministisch)", "[robust]")
{
    uint32_t seed = 12345;
    auto rnd = [&] { seed = seed * 1664525u + 1013904223u; return seed >> 8; };
    int which = 0;
    for (const Bytes& base : { makeWav(44100, 2, 0.05), makeMp3(10) }) {
        const std::string name = (which++ == 0) ? "fuzz.wav" : "fuzz.mp3";   // MP3 wird nur mit passender Endung erkannt
        for (int round = 0; round < 200; round++) {
            Bytes b = base;
            for (int k = 0; k < 4; k++) b[rnd() % std::min<size_t>(b.size(), 200)] = static_cast<uint8_t>(rnd());
            analyzeAndReadAll(writeTemp(name, b));
        }
    }
    SUCCEED("kein Absturz");
}
