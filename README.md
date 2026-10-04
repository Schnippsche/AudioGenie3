# AudioGenie3

AudioGenie3 is a Windows DLL for analyzing audio files and for reading and writing their tags (metadata).
It exports a plain `__stdcall` C interface with Unicode (`...W`) functions, so it can be used from C/C++, C#, VB.NET,
Delphi, VB6 and XProfan alike. Ready-made wrappers for these languages are in `Wrapper/`.

**Version 3.x** is the successor of version 2.0.4 (3.0.0 was the first release as open source). Earlier versions of the library were called AudioGenie and
AudioGenie2; the 2.0.x version numbers were kept for a while after the DLL had been renamed to AudioGenie3, and the
version number now follows the name. The `@since` remarks in the API documentation (for example `2.0.1.0`) name the
2.0.x version in which a function was introduced.

## What's new since 2.0.4

2.0.4 (2011) was the last release before the DLL lay dormant for about 15 years. Version 3.x is a ground-up revision:
every format was checked against its actual specification (not just against whatever a handful of sample files
happened to need), several serious data-loss bugs were found and fixed, and formats and capabilities were added that
did not exist before. The 4,000-file library comparison in "Performance" below is against 2.0.4 itself; every 3.x
release since is additionally checked against real-world collections of thousands of files (14,385 MP3, 596 Ogg, and
more) to catch what synthetic tests alone miss.

- **New formats and capabilities**: OGG Opus (RFC 7845); WAV files of 4 GB or more (RF64, EBU Tech 3306) and the big
  endian RIFX variant used by old Mac/SGI tools; the LAME tag (encoder settings, gapless-playback delay/padding,
  replay gain, two independently verifiable checksums); decompression of ID3v2 frames that use zlib compression, and
  writing ID3v2 unsynchronisation (protects tag data that could otherwise be misread as an MPEG frame sync) - both
  were read-only or entirely unsupported before; the enhanced ID3v1 tag (up to 90 characters, speed, genre text,
  start/end time) and APE tags at the beginning of a file; an XProfan wrapper.
- **Every tag and container format re-verified against its specification**: ID3v1, ID3v2 (v2.2/2.3/2.4), APE, Lyrics3,
  MPEG frame headers plus Xing/Info/VBRI, FLAC, AAC (ADTS and ADIF), WMA, MP4/M4A, Ogg Vorbis, Monkey's Audio, WavPack
  and TTA. Each turned up genuine, real-world deviations - wrong durations, valid files rejected, fields read
  incorrectly - not hypothetical edge cases.
- **Serious bugs fixed that could lose data or corrupt a file**: saving an APE tag next to an ID3v1 tag cut 128 bytes
  of audio; an in-place MP4 save could move the media data without updating the chunk table; a WMA save with a write
  block size of 0 lost the audio data and reported success anyway; a second FLAC or WMA save without re-analyzing the
  file first could damage it or lose extended tag fields and pictures.
- **Real-world quirks handled**: ID3v2.3 tags of old iTunes versions (the 3 character frame IDs of v2.2 with a zero byte in a v2.3
  header), ID3v2.4 tags with ordinary instead of synchsafe frame sizes, v2.4 tags that set the unsynchronisation flag only in the
  tag header, junk that looks like an MPEG frame in front of the audio, and MP3 files with more than 128 KB of junk behind
  the audio or with a padding bit that behaves differently at the start and at the end (joined files). Found by comparing
  the library with other libraries and with ffmpeg on 22,876 files; the duration of an MP3 file without a Xing header is now
  within 0.1 s of the decoded duration for 99 % of the files where the libraries disagree.
- **Tag reading made more correct, not just more permissive**: the format-independent `AUDIO*` fields now fall back
  from ID3v2 to APE to ID3v1 to Lyrics3 per field instead of per tag (see "Tag priority for the abstract fields"
  below), so an old file tagged by a tool that wrote only a private, non-standard ID3v2 frame (RealJukebox and
  similar) is no longer read as having no title or artist at all.
- **Faster**: the MPEG frame scan reads in 64 KB blocks instead of one read per frame (40 MB of frames: 181 ms ->
  6 ms); MD5 throughput is up 32% (560 -> 745 MB/s); analyzing a file needs about a quarter of the read calls it used to
  (3.9 instead of 15.7) and 21 % less time than in 3.0.2 (a profile showed that 93 % of the time is spent in system calls).
- **Open source and far more thoroughly tested**: LGPL-2.1-or-later; a Catch2 test suite that grew from about 5,400
  assertions (3.0.0) to over 20,000 today, run on 32 and 64 bit with AddressSanitizer and fuzzing; a contract check
  that every wrapper (C/C++, C#, VB.NET, Delphi, VB6, XProfan) matches the exports; tools to scan and compare whole
  real-world music libraries between versions and against other libraries (TagLib, mutagen, pymediainfo, Mp3tag) and a complete
  decoding by ffmpeg.

## What it does

- **Analyzes** a file and reports technical data: format, duration, bit rate, sample rate, channels, file size,
  MD5 value of the audio data.
- **Reads and writes the common tag fields** (title, artist, album, track, year, genre, comment, composer) through one
  format-independent set of `AUDIO*` functions. The DLL stores them in the tag type that fits the format. For MP3,
  Monkey's Audio, TTA, WavPack and Musepack, which can carry several of ID3v2, APE, ID3v1 and Lyrics3 side by side,
  reading takes each field from the first of them, in that priority order, that has it (see the table below); an
  ID3v2 tag that exists but lacks one particular frame - e.g. from an old ripping tool that wrote only a private,
  non-standard frame and none of the usual text frames - no longer blocks that one field from falling back to APE,
  ID3v1 or Lyrics3. Writing keeps every one of these tags that already exists in the file consistent with each other
  (not creating a new one), so a field cleared through this API (write an empty value, then save) is cleared in all
  of them and does not resurface on the next read from a tag the save left untouched.
- **Gives full access to the format-specific tags**: ID3v1, ID3v2 (v2.2/2.3/2.4, all frame types including chapters,
  synchronized lyrics and pictures), APE, Lyrics3, Vorbis comments (FLAC, OGG Vorbis, OGG Opus), WMA fields, MP4 atoms
  and WAV chunks.
- **Handles cover art** (embedded pictures) in ID3v2, FLAC, WMA and MP4.
- **Reads OGG Opus** (format ID 12) with the same `OGG*` functions as OGG Vorbis; an OGG file that also contains a
  video stream (Theora, VP8) is read but not written.

### Supported formats

| Format | Tags |
|---|---|
| MP3 (MPEG audio) | ID3v1 (also the enhanced tag), ID3v2, APE (at the end or at the beginning of the file), Lyrics3 |
| MPC (Musepack, stream versions 7, 7.1 and 8) | ID3v2, APE |
| AAC (ADTS and ADIF) | APE, ID3v2, ID3v1 |
| MP4 / M4A | MP4 atoms |
| WMA | WMA fields |
| FLAC | Vorbis comment, pictures |
| OGG Vorbis, OGG Opus | Vorbis comment |
| Monkey's Audio | APE |
| WavPack | APE |
| TTA | ID3v2 |
| WAV (including RF64 for files of 4 GB or more, and the big endian variant RIFX) | RIFF chunks (LIST/INFO, BEXT, CART, DISP) |

The format is detected from the file content. Only for MP3/MP2/MP1 and AAC the file extension (`.mp3`, `.mp2`, `.mp1`,
`.msf`, `.mp3~`, `.aac`) is checked first, so those files need their proper extension.

### Tag priority for the abstract fields

For the formats that can carry more than one of ID3v2, APE, ID3v1 and Lyrics3 (MP3, Monkey's Audio, TTA, WavPack,
Musepack), the format-independent `AUDIOGet...W` functions take each field, independently, from the first tag in this
order that has it. Not every tag type has every field:

| Field | ID3v2 | APE | ID3v1 | Lyrics3 |
|---|---|---|---|---|
| Title, Artist, Album, Genre | yes | yes | yes | yes |
| Comment, Track | yes | yes | yes | - |
| Year | yes | yes | yes | - |
| Composer | yes | yes | - | - |

The format-specific functions (`ID3V2Get...W`, `APEGet...W`, `ID3V1Get...W`, `LYRICSGet...W`) always read only their
own tag, unaffected by this priority order, so they still show exactly what is really stored in that one tag.

`AUDIOSaveChangesW`/`AUDIOSaveChangesToFileW` write the primary tag for the format (see "How it works" below) and, in
addition, keep any of the other tags above in sync if it already exists in the file - none is created from nothing.
This is what keeps the read side above safe from stale data: clearing a field through this API clears it everywhere
it was writable, instead of leaving an old value in a tag the save did not otherwise touch. The one exception is an
APE tag on a TTA or Musepack file: some other tool's incidental APE tag there (e.g. just an "encoder" item written by
ffmpeg) is not kept in sync, only ID3v1 is - APE's own tail rewrite assumes the MPEG-style layout it was built for.
Lyrics3 is never synced either, since only MP3 realistically carries it and it always coexists with ID3v1 there.

## How it works

The DLL is **single-threaded by design** and keeps the data of the last analyzed file in memory:

1. `AUDIOAnalyzeFileW(path)` reads the file and returns the format ID (0 = unknown, 1 = MP3, 2 = WMA, 3 = Monkey,
   4 = FLAC, 5 = WAV, 6 = OGG Vorbis, 7 = MPC, 8 = AAC, 9 = MP4/M4A, 10 = TTA, 11 = WavPack, 12 = OGG Opus).
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
header, and for files with a constant bit rate (CBR). Data after the last frame (up to 128 KB of junk or tags that are not
recognized) is found without reading the whole file and does not count; if there is more of it, or if the padding bit behaves
differently at the start and at the end of the file (joined files), the frames are counted automatically. If a Xing or VBRI header does not match the file
(for example after cutting), the frames are counted. A VBR file **without** such a header can only be estimated from its
size and the bit rate of the first frame; such a file is noticed if the bit rate differs between the first and the last frames
(they are read anyway): then all frames are counted and `MPEGIsVBRW()` returns -1. A bit rate that changes only in the middle
of the file is not noticed (the file is treated as CBR).

To get exact values for every file set the configuration value `MPEGEXACTREAD` **before** the analysis:

```cpp
SetConfigValueW(0, 1);                    // key 0 = MPEGEXACTREAD
AUDIOAnalyzeFileW(L"C:\\Music\\vbr.mp3");   // reads all frames: exact duration, frames, average bit rate, MPEGIsVBRW()
```

The analysis then reads the whole file frame by frame. That takes about 6 ms for a 40 MB file on a fast SSD, but
noticeably longer on a slow disk or a network drive, so leave it off when you scan large collections. Details are in the
documentation of `SetConfigValueW`.

## Performance

Measured on an AMD Ryzen 7 7700 with a Samsung 990 Pro (NVMe), with a library of 22,876 audio files (101 GB, 22,666 of them MP3), and on a
network drive (SMB) with about 16000 MP3 files (measured with 3.0.0). The numbers depend on the hardware; the tool `tests/tools/run_scan.bat` repeats the measurement on your own library (see `tests/README.md`).

### Analyzing files (`AUDIOAnalyzeFileW`)

The analysis reads only the beginning and the end of a file, so its time does not depend on the file size, and it is limited by the
first access to the file, not by the CPU.

| | local SSD (22,876 files, cached) | network drive (16000 files, first access) |
|---|---|---|
| time per file | 0.074 ms (3.0.2: 0.093 ms) | about 33 ms (2.0.4: about 32 ms) |
| read calls per file | 3.9 (3.0.2: 8.8) | 10.2 (2.0.4: 12.2) |

On an earlier 7,339-file subset of the local library AudioGenie 2.0.4 needed 0.14 ms and 15.7 read calls per file. The network values were
not measured again for 3.0.3; the cost of the first access to each file dominates there, so fewer system calls are expected to change little.

The 32 and the 64 bit DLL are equally fast and return identical results. The tags and the technical data are the same as in version 2.0.4, except
where the duration and bit rate of MP3 files are deliberately more accurate now (data behind the last frame, encoders without the padding bit,
VBR files without a header; see the release notes of 3.0.1 to 3.0.3). On a network drive the cost of the first access to each file dominates; if you scan large libraries
repeatedly, keep the results in your application and analyze only new or changed files.

### Optimizations

| Area | Change | Effect |
|---|---|---|
| MPEG frame scan (`MPEGEXACTREAD`) | The file is read in 64 KB blocks instead of one read per frame; the scan stops in front of the tags at the end of the file (data that is not a frame was skipped byte by byte, including a large APE tag); the properties of the first frame are kept | 40 MB of frames: 181 ms -> 6 ms; 1 MB of non-frame data at the end: 446 ms -> about 1 ms |
| MPEG frame scan | With `MPEGEXACTREAD` the counted frames are used for CBR files too | correct length of files with data after the last frame |
| MPEG duration without Xing/Info header | Encoders that never set the padding bit (11 % of the test library, frames 417 instead of 417.96 bytes) are recognized in the first frames; the frames are counted by their length instead of the bit rate. The end of the last frames is searched in the block at the end of the file that was read anyway (up to 128 KB more only if it has no frames); a VBR header that does not match the file counts all frames; a different bit rate in the blocks at the start and at the end of the file (VBR without header, 0.2 % of the files) counts all frames too; the frame scan only counts frames that are followed by the next frame, so junk no longer adds frames or makes a constant bit rate file VBR (70 of 7335 files); a single frame with a damaged header between two valid frames does not interrupt the frames but is not counted | files with up to 128 KB of data after the last frame no longer 0.5 s too long (1.7 % of a 7300 file library); 7335 MP3 files: 910 -> 37 files with a duration error above 0.1 s, 26 -> 5 above 1 s; about 1.8 us (2.5 %) more per file |
| MD5 (`AUDIOGetMD5ValueW`, `GetMD5ValueFromFileW`) | All blocks of a read are processed in one call, the words are read directly, 64 KB read blocks | 560 -> 670 MB/s |
| MD5 | Round 2 with delayed addition (shorter dependency chain) | 670 -> 745 MB/s (5 MB song: about 7 ms) |
| Analysis (`AUDIOAnalyzeFileW`): system calls | The length of the file is read with one call instead of about four (for the stream of the analysis not at all); absolute positions instead of seeks relative to the end of the file; a read buffer of 8 KB instead of 4 KB; the last 8 KB of the file are read once for ID3v1, Lyrics3, the APE footer and the last MPEG block instead of one seek and read each | local SSD, 22,876 files, cached: 2.14 s -> 1.69 s (-21 %), kernel time 1.94 s -> 1.50 s, read calls per file 8.8 -> 3.9 |

A sampling profile of the analysis (warm cache, 22,876 files) shows where the time goes: 91 % is spent in system calls (opening the file 32 %,
reading 36 %, of that the start of the file 16 % and the end 9 %, closing 7 %, the file size and positioning 11 %), only 8 % in the code of the library.
The C library's file functions are the floor; going lower would mean replacing them with direct `ReadFile` calls.

### Tried without a gain (not adopted)

- **Reading the end of the file once** for ID3v1, Lyrics3, APE and the MPEG vendor block: on the network drive fewer read calls
  (10.2 -> 6.7 per file) did not change the time (3.0.0). On a local drive the same idea, as a cache of the last 8 KB of the file, saves
  about 8 % of the analysis time and is part of 3.0.3 (see above). The order of the reads matters: reading the end of the file before
  the ID3v2 tag made the analysis 50 % slower on the network drive.
- **Larger read buffers** than 8 KB for the analysis (time relative to 3.0.2, together with the other system call changes): 6 KB -12.0 %,
  8 KB -12.5 %, 12 KB -11.2 %, 16 KB -10.9 %, 32 KB -6.9 %, 64 KB -2.1 %: more bytes are copied than calls are saved.
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
| C# | `Wrapper/C #/AudioGenie3.cs` |
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

var format = AudioGenie3.AUDIOAnalyzeFile(@"C:\Music\song.flac");
if (format != AudioFormatID.UNKNOWN)
{
    Console.WriteLine($"{AudioGenie3.AUDIOArtist} - {AudioGenie3.AUDIOTitle}");
    Console.WriteLine($"{AudioGenie3.AUDIOGetDuration()} s");

    AudioGenie3.AUDIOTitle = "New title";
    AudioGenie3.AUDIOSaveChanges();
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
