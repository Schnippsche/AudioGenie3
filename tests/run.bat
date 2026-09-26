@echo off
rem Builds and runs the AudioGenie3 tests.  Usage: tests\run.bat [x86|x64|both] [Catch2 arguments]
rem Prerequisite: the DLLs are built (Release\ or x64\Release\).
setlocal
set "TESTS=%~dp0"
set "ROOT=%~dp0.."
set "ARCHS=%~1"
if "%ARCHS%"=="" set "ARCHS=both"
if /i "%ARCHS%"=="both" set "ARCHS=x86 x64"
shift
set FAILED=0
where python >nul 2>nul
if not errorlevel 1 (python "%TESTS%contract\check_wrappers.py" || set FAILED=1)
if not errorlevel 1 (python "%TESTS%contract\check_line_endings.py" || set FAILED=1)
for %%A in (%ARCHS%) do call :run %%A %1 %2 %3 %4
exit /b %FAILED%

:run
setlocal
set "A=%1"
if /i "%A%"=="x86" (set "DLLDIR=%ROOT%\Release") else (set "DLLDIR=%ROOT%\x64\Release")
set "OUT=%TESTS%out\%A%"
if not exist "%OUT%" mkdir "%OUT%"
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" %A% >nul 2>nul
pushd "%OUT%"
rem compile Catch2 only once per architecture
if not exist catch2.obj cl /nologo /EHsc /utf-8 /std:c++17 /O1 /MD /I"%TESTS%third_party" /c /Fo:catch2.obj "%TESTS%third_party\catch2\catch_amalgamated.cpp" || goto :fail
cl /nologo /EHsc /utf-8 /wd4828 /std:c++17 /O1 /W3 /MD /I"%TESTS%third_party" /DAG3_FIXTURES_DIR=\"%TESTS:\=/%fixtures\" /DAG3_DEF_PATH=\"%ROOT:\=/%/AudioGenie3.def\" /Fe:ag3tests.exe "%TESTS%support.cpp" "%TESTS%test_exports.cpp" "%TESTS%test_wav.cpp" "%TESTS%test_mpeg.cpp" "%TESTS%test_robustness.cpp" "%TESTS%test_formats.cpp" "%TESTS%test_id3v2.cpp" "%TESTS%test_id3v2_frames.cpp" "%TESTS%test_fuzz_write.cpp" "%TESTS%test_special.cpp" "%TESTS%test_api_misuse.cpp" "%TESTS%test_md5.cpp" "%TESTS%test_id3v2_spec.cpp" "%TESTS%test_tags_spec.cpp" "%TESTS%test_mpeg_spec.cpp" "%TESTS%test_ogg_spec.cpp" "%TESTS%test_mp4_spec.cpp" "%TESTS%test_aac_spec.cpp" catch2.obj /link "%DLLDIR%\AudioGenie3.lib" oleaut32.lib || goto :fail
set "PATH=%DLLDIR%;%PATH%"
echo === Tests %A% ===
rem ".\" is needed: the current directory may not be in the search path (NoDefaultCurrentDirectoryInExePath)
.\ag3tests.exe %2 %3 %4 %5 || goto :fail
popd
endlocal
exit /b 0
:fail
popd
endlocal & set FAILED=1
exit /b 1
