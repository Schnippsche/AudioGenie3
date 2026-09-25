# Smoke test of the C# and VB.NET wrappers: compiles each wrapper together with a small test program and runs it against
# the 32 and the 64 bit DLL (analysis, tags with special characters, MD5, version, missing file).
#
#   powershell -ExecutionPolicy Bypass -File tests\wrappers\run_wrappers.ps1 [-Arch x86,x64]
#
# Prerequisites: the DLLs are built (Release\ and x64\Release\); the compilers csc.exe and vbc.exe of the .NET Framework 4
# (part of Windows). The test programs are compiled as libraries and loaded by a PowerShell of the same bitness: freshly
# built executables are sometimes removed by virus scanners. Output: tests\out\wrappers\.
param([string[]]$Arch = @('x86', 'x64'))

$ErrorActionPreference = 'Stop'
$here = $PSScriptRoot
$root = (Resolve-Path (Join-Path $here '..\..')).Path
$fixtures = Join-Path $root 'tests\fixtures'
$is64 = [Environment]::Is64BitProcess

$languages = @(
    @{ Name = 'C#';     Compiler = 'csc.exe'; Wrapper = (Join-Path $root 'Wrapper\C #\AudioGenie2.cs');      Check = (Join-Path $here 'CsCheck.cs'); Type = 'CsCheck' },
    @{ Name = 'VB.NET'; Compiler = 'vbc.exe'; Wrapper = (Join-Path $root 'Wrapper\DotNET\audiogenie3.vb'); Check = (Join-Path $here 'VbCheck.vb'); Type = 'VbCheck' }
)

$failed = 0
foreach ($a in $Arch) {
    $dllDir = if ($a -eq 'x86') { Join-Path $root 'Release' } else { Join-Path $root 'x64\Release' }
    if (-not (Test-Path (Join-Path $dllDir 'AudioGenie3.dll'))) {
        Write-Host "missing $dllDir\AudioGenie3.dll, build the DLL first"
        $failed++
        continue
    }
    $framework = if ($a -eq 'x86') { 'Framework' } else { 'Framework64' }
    $compilerDir = Join-Path $env:WINDIR "Microsoft.NET\$framework\v4.0.30319"
    # PowerShell of the same bitness as the DLL
    if ($a -eq 'x86') { $hostDir = if ($is64) { 'SysWOW64' } else { 'System32' } }
    else { $hostDir = if ($is64) { 'System32' } else { 'Sysnative' } }
    $hostExe = Join-Path $env:WINDIR "$hostDir\WindowsPowerShell\v1.0\powershell.exe"

    foreach ($lang in $languages) {
        Write-Host "=== $($lang.Name), $a"
        $out = Join-Path $root "tests\out\wrappers\$($lang.Type)\$a"
        New-Item -ItemType Directory -Force $out | Out-Null
        Copy-Item (Join-Path $dllDir 'AudioGenie3.dll') $out -Force
        $lib = Join-Path $out "$($lang.Type)Lib.dll"
        & (Join-Path $compilerDir $lang.Compiler) -nologo -codepage:1252 "-platform:$a" -target:library "-out:$lib" $lang.Wrapper $lang.Check
        if ($LASTEXITCODE -ne 0) {
            Write-Host "compilation failed"
            $failed++
            continue
        }
        & $hostExe -NoProfile -ExecutionPolicy Bypass -File (Join-Path $here 'host.ps1') -Library $lib -Type $lang.Type -Fixtures $fixtures
        if ($LASTEXITCODE -ne 0) { $failed++ }
    }
}

if ($failed -eq 0) { Write-Host 'wrapper tests: all passed' } else { Write-Host "wrapper tests: $failed run(s) failed" }
exit $failed
