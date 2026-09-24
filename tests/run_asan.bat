@echo off
rem AddressSanitizer-Lauf: baut die DLL UND das Testprogramm mit /fsanitize=address und startet die Tests.
rem Aufruf: tests\run_asan.bat [x64|x86] [Catch2-Argumente]      (Standard: x64)
rem
rem Die ASan-DLL landet in build-asan\<arch>\ (nicht in Release\), damit die normalen Builds unberuehrt bleiben.
rem Die COM-Registrierung wird dabei abgeschaltet (Kopie des Projekts mit RegisterOutput=false,
rem sonst wuerde die per-User-Registrierung auf die ASan-DLL umgebogen).
setlocal
set "TESTS=%~dp0"
set "ROOT=%~dp0.."
set "A=%~1"
if "%A%"=="" set "A=x64"
shift
if /i "%A%"=="x86" (set "PLAT=Win32") else (set "PLAT=x64")
set "MSBUILD=C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
set "BUILD=%ROOT%\build-asan\%A%"
set "OUT=%TESTS%out\asan-%A%"
if not exist "%BUILD%" mkdir "%BUILD%"
if not exist "%OUT%" mkdir "%OUT%"

call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" %A% >nul 2>nul

rem --- 1. DLL mit ASan bauen (Debug-Symbole fuer lesbare Stacktraces)
pushd "%ROOT%"
powershell -NoProfile -Command "(Get-Content -Raw -Encoding UTF8 AudioGenie3.vcxproj) -replace '<RegisterOutput>true</RegisterOutput>','<RegisterOutput>false</RegisterOutput>' | Set-Content -Encoding UTF8 AudioGenie3.asan.vcxproj"
set "CL=-Zi"
set "LINK=-DEBUG"
"%MSBUILD%" AudioGenie3.asan.vcxproj -nologo -v:m -m -p:Configuration=Release -p:Platform=%PLAT% -p:EnableASAN=true -p:WholeProgramOptimization=false -p:LinkTimeCodeGeneration=Default -p:OutDir="%BUILD%\\" -p:IntDir="%BUILD%\obj\\" > "%BUILD%\build.log" 2>&1
set BUILDRC=%errorlevel%
set "CL="
set "LINK="
del AudioGenie3.asan.vcxproj >nul 2>nul
popd
if not %BUILDRC%==0 (echo ASan-Build der DLL fehlgeschlagen, siehe %BUILD%\build.log & exit /b 1)

rem --- 2. Tests mit ASan bauen
pushd "%OUT%"
set "OPTS=/nologo /EHsc /utf-8 /wd4828 /std:c++17 /Zi /Od /MD /fsanitize=address /I"%TESTS%third_party""
if not exist catch2.obj cl %OPTS% /c /Fo:catch2.obj "%TESTS%third_party\catch2\catch_amalgamated.cpp" || goto :fail
cl %OPTS% /DAG3_FIXTURES_DIR=\"%TESTS:\=/%fixtures\" /DAG3_DEF_PATH=\"%ROOT:\=/%/AudioGenie3.def\" /Fe:ag3tests.exe "%TESTS%support.cpp" "%TESTS%test_exports.cpp" "%TESTS%test_wav.cpp" "%TESTS%test_mpeg.cpp" "%TESTS%test_robustness.cpp" "%TESTS%test_formats.cpp" catch2.obj /link /DEBUG "%BUILD%\AudioGenie3.lib" oleaut32.lib || goto :fail

rem --- 3. Ausfuehren (DLL-Verzeichnis zuerst im Suchpfad; die ASan-Laufzeit liefert vcvarsall im PATH)
set "PATH=%BUILD%;%PATH%"
.\ag3tests.exe %1 %2 %3 %4 || goto :fail
popd
endlocal
exit /b 0
:fail
popd
endlocal
exit /b 1
