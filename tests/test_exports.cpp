// Contract: every export declared in the .def must be resolvable in the DLL.
#include "catch2/catch_amalgamated.hpp"
#include "support.h"
#include <fstream>
#include <regex>

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
    CHECK(count == 451);  // 446 API-Funktionen + 5 COM-Standardexporte
    FreeLibrary(h);
}
