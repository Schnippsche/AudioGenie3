// Contract: every export declared in the .def must be resolvable in the DLL.
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include <fstream>
#include <regex>
#include <vector>
#pragma comment(lib, "version.lib")

TEST_CASE("All exports of the .def are present in the DLL", "[exports]")
{
    HMODULE h = LoadLibraryW(L"AudioGenie3.dll");
    REQUIRE(h != nullptr);

    std::ifstream def(AG3_DEF_PATH);
    REQUIRE(def.good());
    std::regex re(R"(^\s*([A-Za-z]\w*)\s*(PRIVATE)?\s*\r?$)");
    std::string line;
    int count = 0;
    while (std::getline(def, line)) {
        std::smatch m;
        if (!std::regex_match(line, m, re)) continue;
        const std::string name = m[1];
        if (name == "LIBRARY" || name == "EXPORTS") continue;
        INFO("Export " << name);
        CHECK(GetProcAddress(h, name.c_str()) != nullptr);
        count++;
    }
    CHECK(count == 459);  // 454 API-Funktionen + 5 COM-Standardexporte
    FreeLibrary(h);
}

TEST_CASE("GetAudioGenieVersionW returns the version of the VERSIONINFO resource", "[exports][version]")
{
    HMODULE h = LoadLibraryW(L"AudioGenie3.dll");
    REQUIRE(h != nullptr);
    auto fn = reinterpret_cast<BSTR(__stdcall*)()>(GetProcAddress(h, "GetAudioGenieVersionW"));
    REQUIRE(fn != nullptr);
    const std::wstring version = ag3test::take(fn());
    CHECK(std::regex_match(version, std::wregex(LR"(\d+\.\d+\.\d+\.\d+)")));

    // compare with the FileVersion string of the DLL file
    wchar_t path[MAX_PATH];
    REQUIRE(GetModuleFileNameW(h, path, MAX_PATH) > 0);
    DWORD dummy = 0;
    const DWORD size = GetFileVersionInfoSizeW(path, &dummy);
    REQUIRE(size > 0);
    std::vector<char> block(size);
    REQUIRE(GetFileVersionInfoW(path, 0, size, block.data()));
    wchar_t* value = nullptr;
    UINT len = 0;
    REQUIRE(VerQueryValueW(block.data(), L"\\StringFileInfo\\040704e4\\FileVersion", reinterpret_cast<LPVOID*>(&value), &len));
    CHECK(version == value);
    FreeLibrary(h);
}
