// Hilfsfunktionen fuer die AudioGenie3-Tests: synthetische Testdateien,
// temporaere Dateien und BSTR-Handling. Keine Fixture-Dateien im Repo noetig.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <oleauto.h>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace ag3test {

using Bytes = std::vector<uint8_t>;

// PCM-WAV (16 Bit, Sinuston 440 Hz), Dauer in Sekunden.
Bytes makeWav(int sampleRate, int channels, double seconds);

// MPEG-1 Layer 3, 128 kbit/s, 44,1 kHz, Stereo, Stille: 'frames' Frames a 417 Byte.
Bytes makeMp3(int frames);

// Eigenes Verzeichnis unter %TEMP%, wird bei Bedarf angelegt.
std::filesystem::path tempDir();
std::filesystem::path writeTemp(const std::string& name, const Bytes& data);
Bytes readFile(const std::filesystem::path& p);

// Uebernimmt einen von der DLL gelieferten BSTR (gibt ihn frei).
std::wstring take(BSTR b);

// Groesse des ID3v2-Tags am Dateianfang inkl. 10-Byte-Header, 0 wenn keiner.
size_t id3v2TotalSize(const Bytes& file);

}  // namespace ag3test
