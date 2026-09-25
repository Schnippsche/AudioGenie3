# Loads a compiled smoke test library and calls its Main(string[] args). Started by run_wrappers.ps1 in a PowerShell of the
# bitness of the DLL, because a 32 bit DLL can only be loaded by a 32 bit process (and vice versa).
param(
    [Parameter(Mandatory = $true)][string]$Library,
    [Parameter(Mandatory = $true)][string]$Type,
    [Parameter(Mandatory = $true)][string]$Fixtures
)
$dir = Split-Path -Parent $Library
$env:PATH = "$dir;" + $env:PATH          # AudioGenie3.dll is found next to the library
$asm = [Reflection.Assembly]::LoadFrom($Library)
$rc = $asm.GetType($Type).GetMethod('Main').Invoke($null, @(, [string[]]@($Fixtures)))
exit [int]$rc
