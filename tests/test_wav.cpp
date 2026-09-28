#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include "../Wrapper/C C++/audiogenie3.h"

using namespace ag3test;

TEST_CASE("WAV: analysis returns format and basic data", "[wav]")
{
    auto p = writeTemp("sine_stereo.wav", makeWav(44100, 2, 1.0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(AUDIOGetSampleRateW() == 44100);
    CHECK(AUDIOGetChannelsW() == 2);
    CHECK(AUDIOGetDurationW() == Catch::Approx(1.0).margin(0.01));
}

TEST_CASE("WAV: mono and another sample rate", "[wav]")
{
    auto p = writeTemp("sine_mono.wav", makeWav(22050, 1, 2.0));
    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(AUDIOGetSampleRateW() == 22050);
    CHECK(AUDIOGetChannelsW() == 1);
    CHECK(AUDIOGetDurationW() == Catch::Approx(2.0).margin(0.01));
}

TEST_CASE("WAV: LIST/INFO tag round trip, audio data stays unchanged", "[wav][roundtrip]")
{
    const Bytes original = makeWav(44100, 2, 0.5);
    auto p = writeTemp("wav_tag.wav", original);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    WAVSetTextFrameW(WAV_INAM, L"Testtitel äöü");
    WAVSetTextFrameW(WAV_IART, L"Testkuenstler");
    REQUIRE(WAVSaveChangesW() != 0);

    REQUIRE(AUDIOAnalyzeFileW(p.c_str()) == WAV);
    CHECK(take(WAVGetTextFrameW(WAV_INAM)) == L"Testtitel äöü");
    CHECK(take(WAVGetTextFrameW(WAV_IART)) == L"Testkuenstler");
    CHECK(AUDIOGetDurationW() == Catch::Approx(0.5).margin(0.01));

    // PCM data (chunk 'data' without header) must stay byte-for-byte identical.
    const Bytes after = readFile(p);
    const size_t dataStart = 44;
    REQUIRE(after.size() > original.size());
    CHECK(std::equal(original.begin() + dataStart, original.end(), after.end() - (original.size() - dataStart)));
}
