// Runs AUDIOAnalyzeFileW over EVERY file of a directory tree (any extension) and calls the generic getters afterwards. It is made to find crashes,
// hangs, wrong results for files that are no audio files and audio files that are not recognized.
//
//   scan_all list <root directory> <list file>
//       writes the list of all files (UTF-8, one path per line, sorted)
//   scan_all scan <list file> <result file> <progress file> <first index> [last index]
//       analyzes the files of the list from <first index> on; the result file is appended (one line per file, flushed at once), the index of the file
//       that is analyzed at the moment is in the progress file: after a crash, the driver script (run_scan_all.ps1) knows the culprit.
//
// The DLL is taken from the PATH. Not part of the Catch2 test run.
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

static std::string utf8(const std::wstring& w)
{
    if (w.empty()) return {};
    const int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), n, nullptr, nullptr);
    return s;
}

static std::wstring fromUtf8(const std::string& s)
{
    if (s.empty()) return {};
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), n);
    return w;
}

// takes over a BSTR of the DLL (frees it) and makes it safe for a tab separated line
static std::string field(BSTR b, size_t maxChars = 60)
{
    if (!b) return {};
    std::wstring w(b, SysStringLen(b));
    SysFreeString(b);
    if (w.size() > maxChars) w.resize(maxChars);
    for (wchar_t& c : w)
        if (c == L'\t' || c == L'\r' || c == L'\n' || c < 0x20) c = L' ';
    return utf8(w);
}

static int listFiles(const wchar_t* root, const wchar_t* listFile)
{
    std::vector<std::wstring> files;
    std::error_code ec;
    fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end;
    while (!ec && it != end) {
        const fs::directory_entry& e = *it;
        std::error_code e2;
        // no directory links (a junction can lead to a loop)
        if (e.is_symlink(e2) || (e.is_directory(e2) && (GetFileAttributesW(e.path().c_str()) & FILE_ATTRIBUTE_REPARSE_POINT))) {
            if (e.is_directory(e2)) it.disable_recursion_pending();
        } else if (e.is_regular_file(e2)) {
            files.push_back(e.path().wstring());
        }
        it.increment(ec);
        if (ec) { ec.clear(); it.pop(ec); if (ec) break; }
    }
    std::sort(files.begin(), files.end());
    std::ofstream out(listFile, std::ios::binary);
    for (const std::wstring& f : files) out << utf8(f) << "\n";
    printf("%zu files\n", files.size());
    return 0;
}

int wmain(int argc, wchar_t** argv)
{
    if (argc >= 4 && std::wstring(argv[1]) == L"list")
        return listFiles(argv[2], argv[3]);
    if (argc < 6 || std::wstring(argv[1]) != L"scan") {
        fprintf(stderr, "usage: scan_all list <root> <list file> | scan <list file> <result file> <progress file> <first index> [last index]\n");
        return 2;
    }
    std::vector<std::wstring> files;
    {
        std::ifstream in(argv[2], std::ios::binary);
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            files.push_back(fromUtf8(line));
        }
    }
    size_t first = (size_t)_wtoi64(argv[5]);
    size_t last = files.size();
    if (argc >= 7) last = std::min(last, (size_t)_wtoi64(argv[6]));
    FILE* result = _wfopen(argv[3], L"ab");
    FILE* progress = _wfopen(argv[4], L"wb");
    if (!result || !progress) { fprintf(stderr, "cannot open the output files\n"); return 2; }
    fseek(result, 0, SEEK_END);
    if (ftell(result) == 0) {
        fprintf(result, "index\tpath\tsize\tformat\tvalid\tduration\tbitrate\tsamplerate\tchannels\tmode\tversion\ttitle\tartist\talbum\tyear\tgenre\tcomment\tlasterror\tms\n");
    }
    for (size_t i = first; i < last; i++) {
        // the index of the file that is analyzed now
        rewind(progress);
        fprintf(progress, "%-12zu", i);
        fflush(progress);
        const std::wstring& path = files[i];
        std::error_code ec;
        const unsigned long long size = fs::file_size(path, ec);
        const auto t0 = Clock::now();
        const long format = AUDIOAnalyzeFileW(path.c_str());
        const double ms = std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
        const short valid = AUDIOIsValidFormatW();
        const float duration = AUDIOGetDurationW();
        const long bitrate = AUDIOGetBitrateW();
        const long rate = AUDIOGetSampleRateW();
        const long channels = AUDIOGetChannelsW();
        const std::string mode = field(AUDIOGetChannelModeW());
        const std::string version = field(AUDIOGetVersionW());
        const std::string title = field(AUDIOGetTitleW());
        const std::string artist = field(AUDIOGetArtistW());
        const std::string album = field(AUDIOGetAlbumW());
        const std::string year = field(AUDIOGetYearW());
        const std::string genre = field(AUDIOGetGenreW());
        const std::string comment = field(AUDIOGetCommentW(), 30);
        const long lastError = AUDIOGetLastErrorNumberW();
        fprintf(result, "%zu\t%s\t%llu\t%ld\t%d\t%.3f\t%ld\t%ld\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%ld\t%.2f\n", i, utf8(path).c_str(), ec ? 0ull : size, format,
                (int)valid, duration, bitrate, rate, channels, mode.c_str(), version.c_str(), title.c_str(), artist.c_str(), album.c_str(), year.c_str(),
                genre.c_str(), comment.c_str(), lastError, ms);
        fflush(result);
    }
    fclose(result);
    fclose(progress);
    return 0;
}
