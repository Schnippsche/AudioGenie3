// Scans a music library with AudioGenie3.dll: analyzes every audio file, writes length, bit rate and tags to a
// tab separated file (UTF-8) and prints timing statistics. Used to compare the 32 and 64 bit DLL.
//
//   scan_library <root directory> <result file> [limit]
//
// The DLL is taken from the PATH (see run_scan.bat). Not part of the Catch2 test run.
#define NOMINMAX
#include "../../Wrapper/C C++/audiogenie3.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using Clock = std::chrono::steady_clock;

static double msSince(Clock::time_point t) { return std::chrono::duration<double, std::milli>(Clock::now() - t).count(); }

static std::string utf8(const std::wstring& w)
{
    if (w.empty()) return {};
    const int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), n, nullptr, nullptr);
    return s;
}

// takes over a BSTR of the DLL (frees it) and makes it safe for a tab separated line
static std::string field(BSTR b)
{
    if (!b) return {};
    std::wstring w(b, SysStringLen(b));
    SysFreeString(b);
    for (wchar_t& c : w)
        if (c == L'\t' || c == L'\r' || c == L'\n') c = L' ';
    return utf8(w);
}

static bool isAudio(const fs::path& p)
{
    static const wchar_t* ext[] = { L".mp3", L".mp2", L".mp1", L".wma", L".m4a", L".ogg", L".flac", L".wav", L".ape", L".mpc", L".tta", L".wv", L".aac" };
    std::wstring e = p.extension().wstring();
    std::transform(e.begin(), e.end(), e.begin(), ::towlower);
    for (const wchar_t* x : ext)
        if (e == x) return true;
    return false;
}

static double filetimeMs(const FILETIME& f) { return (((unsigned long long)f.dwHighDateTime << 32) | f.dwLowDateTime) / 10000.0; }

int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "usage: scan_library <root directory> <result file> [limit]\n");
        return 2;
    }
    const fs::path root = argv[1];
    const fs::path result = argv[2];
    const size_t limit = argc > 3 ? (size_t)atoll(argv[3]) : 0;

    const auto wall0 = Clock::now();
    std::vector<fs::path> files;
    for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied); it != fs::recursive_directory_iterator(); ++it)
        if (it->is_regular_file() && isAudio(it->path())) files.push_back(it->path());
    std::sort(files.begin(), files.end());
    if (limit && files.size() > limit) files.resize(limit);
    const double listMs = msSince(wall0);

    std::ofstream out(result, std::ios::binary);
    out << "path\tsize\tformat\tduration_s\tbitrate_kbps\tsamplerate\tchannels\ttitle\tartist\talbum\tyear\ttrack\tgenre\tcomment\tanalysis_ms\n";

    IO_COUNTERS io0;
    GetProcessIoCounters(GetCurrentProcess(), &io0);
    std::vector<double> times;
    times.reserve(files.size());
    std::vector<std::pair<double, size_t>> slowest;
    size_t recognized = 0;
    double analysisMs = 0, readMs = 0;
    long long bytes = 0;
    const auto scan0 = Clock::now();
    for (size_t i = 0; i < files.size(); i++) {
        const auto t0 = Clock::now();
        const short format = AUDIOAnalyzeFileW(files[i].c_str());
        const double ms = msSince(t0);
        analysisMs += ms;
        times.push_back(ms);
        slowest.push_back({ ms, i });

        const auto r0 = Clock::now();
        std::error_code ec;
        const auto size = fs::file_size(files[i], ec);
        bytes += ec ? 0 : (long long)size;
        out << utf8(files[i].wstring()) << '\t' << (ec ? 0 : size) << '\t' << format << '\t';
        if (format != UNKNOWN) {
            recognized++;
            char num[64];
            snprintf(num, sizeof num, "%.2f", AUDIOGetDurationW());
            out << num << '\t' << AUDIOGetBitrateW() << '\t' << AUDIOGetSampleRateW() << '\t' << AUDIOGetChannelsW() << '\t'
                << field(AUDIOGetTitleW()) << '\t' << field(AUDIOGetArtistW()) << '\t' << field(AUDIOGetAlbumW()) << '\t'
                << field(AUDIOGetYearW()) << '\t' << field(AUDIOGetTrackW()) << '\t' << field(AUDIOGetGenreW()) << '\t'
                << field(AUDIOGetCommentW());
        } else {
            out << "\t\t\t\t\t\t\t\t\t\t" << field(AUDIOGetLastErrorTextW());
        }
        char t[32];
        snprintf(t, sizeof t, "%.3f", ms);
        out << '\t' << t << '\n';
        readMs += msSince(r0);
    }
    out.close();
    const double scanMs = msSince(scan0);
    const double wallMs = msSince(wall0);

    IO_COUNTERS io1;
    GetProcessIoCounters(GetCurrentProcess(), &io1);
    FILETIME c, e, k, u;
    GetProcessTimes(GetCurrentProcess(), &c, &e, &k, &u);

    std::sort(times.begin(), times.end());
    auto pct = [&](double p) { return times.empty() ? 0.0 : times[std::min(times.size() - 1, (size_t)(p * times.size()))]; };
    std::sort(slowest.rbegin(), slowest.rend());

    char buf[512];
    std::string s;
    auto add = [&](const char* fmt, auto... a) { snprintf(buf, sizeof buf, fmt, a...); s += buf; s += '\n'; };
    add("DLL:                %d bit", (int)(sizeof(void*) * 8));
    add("root:               %s", root.string().c_str());
    add("files:              %zu (recognized %zu, unknown %zu), %.1f GB", files.size(), recognized, files.size() - recognized, bytes / 1073741824.0);
    add("listing:            %.0f ms", listMs);
    add("analysis (sum):     %.0f ms   = %.2f ms/file", analysisMs, files.empty() ? 0.0 : analysisMs / files.size());
    add("  median %.2f ms, p95 %.2f ms, p99 %.2f ms, max %.1f ms", pct(0.5), pct(0.95), pct(0.99), times.empty() ? 0.0 : times.back());
    add("reading results:    %.0f ms (getters and writing the result file)", readMs);
    add("scan loop:          %.0f ms", scanMs);
    add("wall time:          %.0f ms", wallMs);
    add("CPU time:           user %.0f ms, kernel %.0f ms", filetimeMs(u), filetimeMs(k));
    add("read operations:    %.1f per file, %.1f KB per file", (double)(io1.ReadOperationCount - io0.ReadOperationCount) / std::max<size_t>(1, files.size()),
        (double)(io1.ReadTransferCount - io0.ReadTransferCount) / 1024 / std::max<size_t>(1, files.size()));
    add("slowest files:");
    for (size_t i = 0; i < std::min<size_t>(5, slowest.size()); i++)
        add("  %8.1f ms  %s", slowest[i].first, files[slowest[i].second].string().c_str());
    fputs(s.c_str(), stdout);
    std::ofstream(result.string() + ".summary.txt") << s;
    return 0;
}
