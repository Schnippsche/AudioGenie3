// Helper functions for the AudioGenie3 tests: synthetic test files,
// temporary files and BSTR handling. No fixture files needed in the repo.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <oleauto.h>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace ag3test {

using Bytes = std::vector<uint8_t>;

// PCM-WAV (16 Bit, Sinuston 440 Hz), Dauer in Sekunden.
Bytes makeWav(int sampleRate, int channels, double seconds);

// MPEG-1 Layer 3, 128 kbit/s, 44,1 kHz, Stereo, Stille: 'frames' Frames a 417 Byte.
Bytes makeMp3(int frames);

// Own directory under %TEMP%, created on demand.
std::filesystem::path tempDir();
std::filesystem::path writeTemp(const std::string& name, const Bytes& data);
Bytes readFile(const std::filesystem::path& p);

// Takes over a BSTR returned by the DLL (frees it).
std::wstring take(BSTR b);
// narrows an ASCII-only wstring (e.g. frame IDs, version text) for log output
inline std::string ascii(const std::wstring& w) { std::string s; for (wchar_t c : w) s += static_cast<char>(c); return s; }

// Value of an environment variable, empty if it is not set (getenv replacement without the C4996 warning).
inline std::string envValue(const char* name)
{
    char* buf = nullptr;
    size_t len = 0;
    std::string v;
    if (_dupenv_s(&buf, &len, name) == 0 && buf) v = buf;
    free(buf);
    return v;
}
inline bool envSet(const char* name) { char* buf = nullptr; size_t len = 0; const bool set = _dupenv_s(&buf, &len, name) == 0 && buf; free(buf); return set; }

// Size of the ID3v2 tag at the start of the file including the 10-byte header, 0 if there is none.
size_t id3v2TotalSize(const Bytes& file);

}  // namespace ag3test
