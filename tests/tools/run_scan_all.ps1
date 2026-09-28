# Runs scan_all.exe over a whole directory tree with the 32 or 64 bit DLL. A crash or a hang of the DLL does not stop the run: the file that
# was analyzed at that moment is recorded in <out>\problems.txt and the run goes on behind it.
#
#   powershell -File tests\tools\run_scan_all.ps1 -Arch x64 -Root D:\ -Out tests\out\scan_all\x64 [-DllDir <directory of another DLL>] [-HangSeconds 120] [-List]
#
# -List: make the file list new (else an existing <out>\files.txt is used).
param(
    [string]$Arch = 'x64',
    [string]$Root = 'D:\',
    [string]$Out = 'tests\out\scan_all\x64',
    [string]$DllDir = '',
    [int]$HangSeconds = 120,
    [switch]$List
)
$ErrorActionPreference = 'Stop'
# the worker has the progress file open for writing: read it with a share mode that allows this
function Read-Progress {
    $fs = New-Object System.IO.FileStream($progress, 'Open', 'Read', 'ReadWrite')
    try { (New-Object System.IO.StreamReader($fs)).ReadToEnd().Trim() } finally { $fs.Dispose() }
}
$repo = Split-Path (Split-Path $PSScriptRoot)
Set-Location $repo
New-Item -ItemType Directory -Force $Out | Out-Null
$Out = (Resolve-Path $Out).Path
if (-not $DllDir) { $DllDir = if ($Arch -eq 'x86') { "$repo\Release" } else { "$repo\x64\Release" } }

# build scan_all.exe
$vc = 'C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat'
$exe = "$Out\scan_all.exe"
$build = "call `"$vc`" $Arch >nul 2>nul && cd /d `"$Out`" && cl /nologo /EHsc /utf-8 /wd4828 /std:c++17 /O2 /MD /Fe:scan_all.exe `"$repo\tests\tools\scan_all.cpp`" /link `"$DllDir\AudioGenie3.lib`" oleaut32.lib user32.lib"
cmd /c $build | Out-Null
if (-not (Test-Path $exe)) { throw 'scan_all.exe was not built' }
$env:PATH = "$DllDir;$env:PATH"

$fileList = "$Out\files.txt"
if ($List -or -not (Test-Path $fileList)) { & $exe list $Root $fileList }
$total = (Get-Content $fileList -ReadCount 100000 | Measure-Object -Property Count -Sum).Sum
"$total files to analyze"
$result = "$Out\result.tsv"
$progress = "$Out\progress.txt"
$problems = "$Out\problems.txt"
Remove-Item $result, $problems -ErrorAction SilentlyContinue
$paths = [System.IO.File]::ReadAllLines($fileList)
$start = 0
$sw = [Diagnostics.Stopwatch]::StartNew()
while ($start -lt $total) {
    $p = Start-Process -FilePath $exe -ArgumentList @('scan', $fileList, $result, $progress, $start) -PassThru -NoNewWindow
    $null = $p.Handle   # else ExitCode stays empty
    $lastIndex = -1; $lastChange = [Diagnostics.Stopwatch]::StartNew()
    while (-not $p.HasExited) {
        Start-Sleep -Seconds 2
        try { $now = [int64](Read-Progress) } catch { $now = $lastIndex }
        if ($now -ne $lastIndex) { $lastIndex = $now; $lastChange.Restart() }
        elseif ($lastChange.Elapsed.TotalSeconds -gt $HangSeconds) {
            "HANG at index $now : $($paths[$now])" | Tee-Object -FilePath $problems -Append
            $p.Kill(); $p.WaitForExit(); break
        }
    }
    $p.WaitForExit()
    if ($p.ExitCode -eq 0) { break }
    $idx = [int64](Read-Progress)
    if ($p.ExitCode -ne -1) { "CRASH (exit code $($p.ExitCode)) at index $idx : $($paths[$idx])" | Tee-Object -FilePath $problems -Append }
    $start = $idx + 1
    "restart at $start ($([int]$sw.Elapsed.TotalSeconds) s)"
}
"finished after $([int]$sw.Elapsed.TotalSeconds) s"
if (Test-Path $problems) { "problems:"; Get-Content $problems } else { "no crash and no hang" }
