@echo off
rem Scans a music library with the 32 or 64 bit DLL and measures the time.
rem Usage: tests\tools\run_scan.bat <x86|x64> <root directory> [limit] [k/n]
rem Optional: set AG3_DLLDIR to use the DLL of another directory (e.g. a baseline build for a comparison).
rem Result: tests\out\scan\<arch>\result.tsv and result.tsv.summary.txt
rem Prerequisite: the DLLs are built (Release\ or x64\Release\).
setlocal
set "TOOLS=%~dp0"
set "ROOT=%~dp0..\.."
set "A=%~1"
set "DIR=%~2"
set "LIMIT=%~3"
set "PART=%~4"
if "%A%"=="" goto :usage
if "%DIR%"=="" goto :usage
if /i "%A%"=="x86" (set "DLLDIR=%ROOT%\Release") else (set "DLLDIR=%ROOT%\x64\Release")
if defined AG3_DLLDIR set "DLLDIR=%AG3_DLLDIR%"
set "OUT=%ROOT%\tests\out\scan\%A%"
if not exist "%OUT%" mkdir "%OUT%"
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" %A% >nul 2>nul
pushd "%OUT%"
cl /nologo /EHsc /utf-8 /wd4828 /std:c++17 /O2 /MD /Fe:scan_library.exe "%TOOLS%scan_library.cpp" /link "%DLLDIR%\AudioGenie3.lib" oleaut32.lib || goto :fail
set "PATH=%DLLDIR%;%PATH%"
.\scan_library.exe "%DIR%" result.tsv %LIMIT% %PART% || goto :fail
popd
endlocal
exit /b 0
:usage
echo Usage: run_scan.bat ^<x86^|x64^> ^<root directory^> [limit] [k/n]
exit /b 2
:fail
popd
endlocal
exit /b 1
