# AudioGenie3

AudioGenie3 is a Windows DLL for analyzing audio files and for reading and writing their tags (metadata).
It exports a plain `__stdcall` C interface with Unicode (`...W`) functions, so it can be used from C/C++, C#, VB.NET,
Delphi, VB6 and XProfan alike. Ready-made wrappers for these languages are in `Wrapper/`.

**Version 3.x** is the successor of version 2.0.4 (3.0.0 was the first release as open source). Earlier versions of the library were called AudioGenie and
AudioGenie2; the 2.0.x version numbers were kept for a while after the DLL had been renamed to AudioGenie3, and the
version number now follows the name. The `@since` remarks in the API documentation (for example `2.0.1.0`) name the
2.0.x version in which a function was introduced. For historical reasons the C# wrapper is still called `AudioGenie2.cs`
(class `AudioGenie2`), so that existing code keeps working.

## What it does

- **Analyzes** a file and reports technical data: format, duration, bit rate, sample rate, channels, file size,
  MD5 value of the audio data.
- **Reads and writes the common tag fields** (title, artist, album, track, year, genre, comment, composer) through one
  format-independent set of `AUDIO*` functions. The DLL stores them in the tag type that fits the format.
- **Gives full access to the format-specific tags**: ID3v1, ID3v2 (v2.2/2.3/2.4, all frame types including chapters,
  synchronized lyrics and pictures), APE, Lyrics3, Vorbis comments, WMA fields, MP4 atoms and WAV chunks.
- **Handles cover art** (embedded pictures) in ID3v2, FLAC, WMA and MP4.

### Supported formats

| Format | Tags |
|---|---|
| MP3 (MPEG audio) | ID3v1 (also the enhanced tag), ID3v2, APE (at the end or at the beginning of the file), Lyrics3 |
| MPC (Musepack) | ID3v2, APE |
| AAC (ADTS) | APE, ID3v2, ID3v1 |
| MP4 / M4A | MP4 atoms |
| WMA | WMA fields |
| FLAC | Vorbis comment, pictures |
| OGG Vorbis | Vorbis comment |
| Monkey's Audio | APE |
| WavPack | APE |
| TTA | ID3v2 |
| WAV | RIFF chunks (LIST/INFO, BEXT, CART, DISP) |

The format is detected from the file content. Only for MP3/MP2/MP1 and AAC the file extension (`.mp3`, `.mp2`, `.mp1`,
`.msf`, `.mp3~`, `.aac`) is checked first, so those files need their proper extension.

## How it works

The DLL is **single-threaded by design** and keeps the data of the last analyzed file in memory:

1. `AUDIOAnalyzeFileW(path)` reads the file and returns the format ID (0 = unknown, 1 = MP3, 2 = WMA, 3 = Monkey,
   4 = FLAC, 5 = WAV, 6 = OGG, 7 = MPC, 8 = AAC, 9 = MP4/M4A, 10 = TTA, 11 = WavPack).
2. All `AUDIOGet...W` / `ID3V2Get...W` / ... functions read from that remembered state.
3. `AUDIOSet...W` and the other setters change the state in memory only.
4. `AUDIOSaveChangesW()` writes the changes back into the analyzed file, and returns 0 on error, otherwise -1.
   `AUDIOSaveChangesToFileW(path)` writes into another file.

Because of this you must not call the DLL from several threads at the same time. Analyze the file again
before you access a different one.

Note that `AUDIOSaveChangesW` writes the abstract fields (title, artist, ...) and thereby overwrites the matching
ID3v2 frames. If you edit ID3v2 frames directly, save with `ID3V2SaveChangesW` instead.

Strings returned by the DLL are `BSTR`s. When you declare the functions yourself (for example with P/Invoke), the return
type must be marshalled as `BStr`, otherwise the process crashes on x64. The wrappers in `Wrapper/` already do this.

### MP3 files with a variable bit rate (VBR)

The duration, the number of frames and the average bit rate of an MP3 file are exact if the file has a Xing or VBRI
header, and for files with a constant bit rate (CBR) that have no additional data after the audio. A VBR file **without**
such a header can only be estimated from its size and the bit rate of the first frame, and is then treated as CBR; the
same estimate is too long for a CBR file with junk after the last frame.

For such files set the configuration value `MPEGEXACTREAD` **before** the analysis:

```cpp
SetConfigValueW(0, 1);                    // key 0 = MPEGEXACTREAD
AUDIOAnalyzeFileW(L"C:\\Music\\vbr.mp3");   // reads all frames: exact duration, frames, average bit rate, MPEGIsVBRW()
```

The analysis then reads the whole file frame by frame. That takes about 6 ms for a 40 MB file on a fast SSD, but
noticeably longer on a slow disk or a network drive, so leave it off when you scan large collections. Details are in the
documentation of `SetConfigValueW`.

## Performance

Measured on an AMD Ryzen 7 7700 with a Samsung 990 Pro (NVMe) and on a network drive (SMB) with about 16000 MP3 files. The numbers
depend on the hardware; the tool `tests/tools/run_scan.bat` repeats the measurement on your own library (see `tests/README.md`).

### Analyzing files (`AUDIOAnalyzeFileW`)

The analysis reads only the beginning and the end of a file, so its time does not depend on the file size, and it is limited by the
first access to the file, not by the CPU.

| | local SSD (7339 files, cached) | network drive (16000 files, first access) |
|---|---|---|
| time per file | 0.11 ms (AudioGenie 2.0.4: 0.14 ms) | about 33 ms (2.0.4: about 32 ms) |
| read calls per file | 8.4 (2.0.4: 15.7) | 10.2 (2.0.4: 12.2) |

The 32 and the 64 bit DLL are equally fast and return identical results. For 4000 files the length, bit rate and all tags are identical to
the results of version 2.0.4. On a network drive the cost of the first access to each file dominates; if you scan large libraries
repeatedly, keep the results in your application and analyze only new or changed files.

### Optimizations

| Area | Change | Effect |
|---|---|---|
| MPEG frame scan (`MPEGEXACTREAD`) | The file is read in 64 KB blocks instead of one read per frame; the scan stops in front of the tags at the end of the file (data that is not a frame was skipped byte by byte, including a large APE tag); the properties of the first frame are kept | 40 MB of frames: 181 ms -> 6 ms; 1 MB of non-frame data at the end: 446 ms -> about 1 ms |
| MPEG frame scan | With `MPEGEXACTREAD` the counted frames are used for CBR files too | correct length of files with data after the last frame |
| MD5 (`AUDIOGetMD5ValueW`, `GetMD5ValueFromFileW`) | All blocks of a read are processed in one call, the words are read directly, 64 KB read blocks | 560 -> 670 MB/s |
| MD5 | Round 2 with delayed addition (shorter dependency chain) | 670 -> 745 MB/s (5 MB song: about 7 ms) |

### Tried without a gain (not adopted)

- **Reading the end of the file once** for ID3v1, Lyrics3, APE and the MPEG vendor block: fewer read calls (10.2 -> 6.7 per file) but the
  same time on the network drive. The order of the reads matters: reading the end of the file before the ID3v2 tag made the analysis
  50 % slower.
- **`CBlob` growth strategy:** the buffer already grows by a factor of 1.5; the whole CPU time of the analysis is below 0.5 % of the
  time per file on a network drive.
- **Several processes in parallel** on the network drive: the same total throughput as one process.
- **Larger MD5 read blocks and overlapped reads:** no gain above 64 KB; on a network drive the transfer rate (50 to 70 MB/s) is the limit.

The details of the MD5 and the frame scan changes are documented in `md5.cpp`, `MD5Tool.h` and `MPEGAudio.cpp`.

## Getting started

### Build

```
MSBuild AudioGenie3.vcxproj /p:Configuration=Release /p:Platform=Win32
MSBuild AudioGenie3.vcxproj /p:Configuration=Release /p:Platform=x64
```

The result is `AudioGenie3.dll` (and `AudioGenie3.lib` for C/C++) in `Release\` or `x64\Release\`. Pick the bitness that
matches your application. The Visual Studio build registers the DLL for the current user automatically.

### Register the DLL

The DLL is a COM server and must be registered before use. Use the `regsvr32` of the same bitness as the DLL
(`C:\Windows\SysWOW64egsvr32.exe` for the 32-bit DLL on 64-bit Windows):

```
regsvr32 AudioGenie3.dll                   (per machine, needs administrator rights)
regsvr32 /n /i:user AudioGenie3.dll        (for the current user only)
regsvr32 /u AudioGenie3.dll                (unregister a per-machine registration)
regsvr32 /u /n /i:user AudioGenie3.dll     (unregister a per-user registration)
```

Keep the DLL in a fixed folder, because the registration stores its path.

### Wrappers

| Language | File |
|---|---|
| C / C++ | `Wrapper/C C++/audiogenie3.h` (link `AudioGenie3.lib`) |
| C# | `Wrapper/C #/AudioGenie2.cs` |
| VB.NET | `Wrapper/DotNET/audiogenie3.vb` |
| Delphi | `Wrapper/Delphi/AudioGenie3.pas` |
| VB6 | `Wrapper/VB6/clsAudioGenie.cls` |
| XProfan | `Wrapper/Profan/prfwrapper.inc` (by Dieter Zornow; the procedures call the DLL functions by name after `ImportDLL`) |

## Examples

### C++: read tags and technical data

```cpp
#include "audiogenie3.h"
#include <cstdio>

int main()
{
    short format = AUDIOAnalyzeFileW(L"C:\\Music\\song.mp3");
    if (format == UNKNOWN) {
        BSTR err = AUDIOGetLastErrorTextW();
        wprintf(L"Not readable: %s\n", err);
        SysFreeString(err);
        return 1;
    }

    BSTR title  = AUDIOGetTitleW();
    BSTR artist = AUDIOGetArtistW();
    wprintf(L"%s - %s\n", artist, title);
    wprintf(L"%.1f s, %ld kbit/s, %ld Hz\n",
            AUDIOGetDurationW(), AUDIOGetBitrateW(), AUDIOGetSampleRateW());
    SysFreeString(title);
    SysFreeString(artist);
    return 0;
}
```

### C++: change tags and save

```cpp
if (AUDIOAnalyzeFileW(L"C:\\Music\\song.mp3") != UNKNOWN) {
    AUDIOSetTitleW(L"New title");
    AUDIOSetArtistW(L"New artist");
    AUDIOSetYearW(L"2024");
    if (!AUDIOSaveChangesW())
        wprintf(L"Saving failed\n");
}
```

### C++: add a cover picture to an MP3

```cpp
AUDIOAnalyzeFileW(L"C:\\Music\\song.mp3");
// asLink = 0: embed the image data (1 would store only a link to the file)
ID3V2AddPictureFileW(L"C:\\Music\\cover.jpg", L"Front cover", COVER_FRONT, 0);
ID3V2SaveChangesW();
```

### C#

```csharp
using AudioGenie;

var format = AudioGenie2.AUDIOAnalyzeFile(@"C:\Music\song.flac");
if (format != AudioFormatID.UNKNOWN)
{
    Console.WriteLine($"{AudioGenie2.AUDIOArtist} - {AudioGenie2.AUDIOTitle}");
    Console.WriteLine($"{AudioGenie2.AUDIOGetDuration()} s");

    AudioGenie2.AUDIOTitle = "New title";
    AudioGenie2.AUDIOSaveChanges();
}
```

### Delphi

```delphi
var
  AG: TAudioGenie3;
begin
  AG := TAudioGenie3.Create;
  try
    if AG.AUDIOAnalyzeFileW('C:\Music\song.mp3') <> UNKNOWN then
    begin
      ShowMessage(AG.AUDIOArtistW + ' - ' + AG.AUDIOTitleW);
      AG.AUDIOTitleW := 'New title';
      AG.AUDIOSaveChangesW;
    end;
  finally
    AG.Free;
  end;
end;
```

### VB6

```vb
Dim ag As New clsAudioGenie

If ag.AUDIOAnalyzeFile("C:\Music\song.mp3") <> 0 Then
    Debug.Print ag.AUDIOArtist & " - " & ag.AUDIOTitle
    ag.AUDIOTitle = "New title"
    ag.AUDIOSaveChanges
End If
```

## Documentation

The API reference (all 470 exported functions, grouped by format) is available online at
<https://schnippsche.github.io/AudioGenie3/>. It is generated with Doxygen:

```
doxygen Doxyfile
```

The output goes to `docs/` (open `docs/index.html`). In the generated help, the functions are grouped by tag
or format on the *Topics* page. This README is used as the start page of the help.

## Tests

The Catch2 test suite exercises the DLL through its C interface, with fixtures for every format, fuzzing and
AddressSanitizer runs. See `tests/README.md`.

## License

Copyright (C) 2001-2026 Stefan Toengi.

AudioGenie3 is free software, licensed under the GNU Lesser General Public
License, version 2.1 or (at your option) any later version. See `LICENSE`.

Third-party components and their licenses are listed in
`THIRD-PARTY-NOTICES.md`.
