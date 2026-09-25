@echo off
rem Smoke test of the C# and VB.NET wrappers against the 32 and 64 bit DLL. Usage: tests\wrappers\run_wrappers.bat [-Arch x86,x64]
"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File "%~dp0run_wrappers.ps1" %*
exit /b %ERRORLEVEL%
