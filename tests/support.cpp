#include "support.h"
#include <cmath>
#include <fstream>

namespace ag3test {

static void put16(Bytes& b, uint16_t v) { b.push_back(v & 0xFF); b.push_back(v >> 8); }
static void put32(Bytes& b, uint32_t v) { for (int i = 0; i < 4; i++) b.push_back((v >> (8 * i)) & 0xFF); }
static void putTag(Bytes& b, const char* t) { b.insert(b.end(), t, t + 4); }

Bytes makeWav(int sampleRate, int channels, double seconds)
{
    const uint32_t frames = static_cast<uint32_t>(sampleRate * seconds);
    const uint32_t dataSize = frames * channels * 2;
    Bytes b;
    putTag(b, "RIFF"); put32(b, 36 + dataSize); putTag(b, "WAVE");
    putTag(b, "fmt "); put32(b, 16); put16(b, 1); put16(b, static_cast<uint16_t>(channels));
    put32(b, sampleRate); put32(b, sampleRate * channels * 2);
    put16(b, static_cast<uint16_t>(channels * 2)); put16(b, 16);
    putTag(b, "data"); put32(b, dataSize);
    for (uint32_t i = 0; i < frames; i++) {
        const int16_t s = static_cast<int16_t>(12000 * std::sin(2.0 * 3.14159265358979 * 440.0 * i / sampleRate));
        for (int c = 0; c < channels; c++) put16(b, static_cast<uint16_t>(s));
    }
    return b;
}

Bytes makeMp3(int frames)
{
    Bytes b;
    for (int i = 0; i < frames; i++) {
        const uint8_t header[4] = { 0xFF, 0xFB, 0x90, 0x00 };  // MPEG1 L3, 128k, 44.1k, kein Padding, Stereo
        b.insert(b.end(), header, header + 4);
        b.resize(b.size() + 417 - 4, 0);
    }
    return b;
}

std::filesystem::path tempDir()
{
    auto d = std::filesystem::temp_directory_path() / "ag3tests";
    std::filesystem::create_directories(d);
    return d;
}

std::filesystem::path writeTemp(const std::string& name, const Bytes& data)
{
    auto p = tempDir() / name;
    std::ofstream f(p, std::ios::binary | std::ios::trunc);
    f.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
    return p;
}

Bytes readFile(const std::filesystem::path& p)
{
    std::ifstream f(p, std::ios::binary);
    return Bytes(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

std::wstring take(BSTR b)
{
    std::wstring s = b ? std::wstring(b, SysStringLen(b)) : std::wstring();
    SysFreeString(b);
    return s;
}

size_t id3v2TotalSize(const Bytes& f)
{
    if (f.size() < 10 || f[0] != 'I' || f[1] != 'D' || f[2] != '3') return 0;
    return 10 + ((f[6] & 0x7F) << 21 | (f[7] & 0x7F) << 14 | (f[8] & 0x7F) << 7 | (f[9] & 0x7F));
}

}  // namespace ag3test
