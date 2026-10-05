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
  6 ms); MD5 throughput is up 32% (560 -> 745 MB/s); analyzing a file needs less than a third of the read calls of 2.0.4 (2.4 instead
  of 8.9 per file), takes 39 % less time than 2.0.4, and is two and a half times as fast as TagLib, four times as fast as JAudioTagger and
  almost eight times as fast as mutagen (see "Performance"; a profile showed that 93 % of the former time was spent in system calls).
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

All file access (reading and writing) goes through one small class, `CFile` (`File.h`, `File.cpp`): the rest of the library reads and writes with
`read`, `write`, `seek`, `size` and positioned reads, and only `File.cpp` uses system functions (`CreateFileW`, `ReadFile`, `WriteFile`). A port to
another platform has to replace that file and the Windows-specific parts (ATL strings, COM export).

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

Measured on an AMD Ryzen 7 7700 with a Samsung 990 Pro (NVMe), with a library of 22,919 audio files (103 GB: 22,666 MP3, 127 WMA, 80 M4A, 28 Monkey's Audio, 7 WAV, 5 WavPack, 3 Ogg Vorbis, 2 FLAC and 1 AAC), and on a
network drive (SMB) with about 16000 MP3 files (measured with 3.0.0). The numbers depend on the hardware; the tool `tests/tools/run_scan.bat` repeats the measurement on your own library (see `tests/README.md`).

### Analyzing files (`AUDIOAnalyzeFileW`)

The analysis reads only the beginning and the end of a file, so its time does not depend on the file size, and it is limited by the
first access to the file, not by the CPU.

#### Comparison with other libraries (local SSD, warm cache)

All libraries analyzed the same 22,919 files of the local library (103 GB: 22,666 MP3, 127 WMA, 80 M4A, 28 Monkey's Audio, 7 WAV, 5 WavPack, 3 Ogg Vorbis,
2 FLAC and 1 AAC) from the NVMe drive with a warm file system cache. For every file a small program reads what an application that shows a library
needs: the format, the duration, bit rate, sample rate, channels and the tags title, artist, album, year, track, genre and comment. The time is the
median of 3 to 7 passes (fewer for the slow libraries; the first pass is a warm-up); two rounds, the second in the reverse order, gave the same values
within 1 %. The table shows the mean of the two rounds, for the 64 bit versions of the libraries (AudioGenie 2.0.4 only exists as a 32 bit DLL); the first
column is the time of one pass over all 22,919 files.

| Library | total time for 22,919 files | time per file | read calls per file | CPU time per file |
|---|---|---|---|---|
| **AudioGenie3, current** | **1.2 s** | **0.053 ms** | **2.4** | **0.052 ms** |
| AudioGenie 2.0.4 (32 bit only) | 2.0 s | 0.087 ms | 8.9 | 0.087 ms |
| tagparser 12.5.3 (C++) | 2.4 s | 0.105 ms | 8.8 | 0.104 ms |
| TagLib 2.3.2 (C++) | 3.0 s | 0.129 ms | 21.6 | 0.128 ms |
| JAudioTagger 3.0.1 (Java 23) | 4.8 s | 0.210 ms | 4.9 | 0.213 ms |
| mutagen 1.48.1 (Python 3.11) | 9.5 s | 0.414 ms | 6.3 | 0.413 ms |
| tinytag 2.3.2 (Python 3.11) | 9.6 s | 0.418 ms | 3.8 | 0.418 ms |
| music-metadata 12.0.0 (Node.js 22) | 16.9 s | 0.739 ms | 24.3 | 0.750 ms |
| FFmpeg libavformat 62.3 (PyAV 17, Python 3.11) | 16.8 s | 0.732 ms | 1.6 | 0.731 ms |
| MediaInfoLib 26.05 | 33.1 s | 1.443 ms | 4.7 | 1.424 ms |

The current state needs 50 % less time than tagparser, 59 % less than TagLib, a quarter of the time of JAudioTagger, one eighth of mutagen and tinytag,
one fourteenth of music-metadata and FFmpeg and one twenty-seventh of MediaInfoLib, and 39 % less than 2.0.4. The duration and the bit rate of
MP3 files are more exact than in 2.0.4 (see the release notes of 3.0.1 and 3.0.2); that costs a little time, which the fewer system calls more than make up.
A second 2.0.4 build in the old source tree needs 0.093 ms and 13.0 read calls per file. MediaInfoLib with the option `ParseSpeed` 0 (headers only) needs 1.17 ms and
4.2 read calls. JAudioTagger is measured after the warm-up of the JIT compiler (the first pass is not counted), its CPU time includes the compiler
and garbage collector threads. FFmpeg and tinytag could not open 33 of the 22,919 files, JAudioTagger 35, mutagen 2 and tagparser 137 (it does not read WMA, which accounts for 127 of them).
tagparser was built with MSVC for the test (with win-iconv instead of GNU libiconv, without Boost) and given the paths in the ANSI code page;
music-metadata ran with its default options (its option `duration`, which parses the whole file to get the duration, is off by default).

The libraries do not return the same: MediaInfoLib and FFmpeg analyze the streams in depth and return far more than the fields above, which is
why they need more time for this task. TagLib and mutagen estimate the duration of an MP3 file without a Xing header from the file size:
on the 1,728 files (almost all MP3) where TagLib, mutagen, pymediainfo and AudioGenie3 3.0.2 differed by more than 0.5 s in the duration, a complete
decoding by ffmpeg agreed within 0.1 s with the duration of AudioGenie3 for 99.3 % of the files, with that of TagLib for 12.6 % and with that of
mutagen for 7.5 %.

#### Other formats (local SSD, warm cache)

The comparison above is 99 % MP3. The other formats of the test library (the FLAC files are the test files of the FLAC project, `flac-test-files`,
with deliberately extreme metadata: tags of 17 MB, 1,000 comments, 13 blocks of several MB, and 6 invalid files that no library reads) with the 64 bit
DLL (2.0.4: 32 bit, the only one there is), the median of 40 passes; TagLib 2.3.2 for comparison:

| Format | files | 2.0.4 (32 bit): time, read calls | current: time, read calls | TagLib: time, read calls |
|---|---|---|---|---|
| WMA | 127 | 0.088 ms, 7.1 | **0.040 ms, 2.0** | 0.44 ms, 147.5 |
| M4A | 80 | 0.157 ms, 10.4 | **0.081 ms, 3.7** | 0.29 ms, 53.0 |
| WAV | 7 | 0.066 ms, 6.0 | **0.032 ms, 2.0** | 0.066 ms, 10.1 |
| WavPack | 5 | 0.061 ms, 6.0 | **0.033 ms, 2.0** | 0.057 ms, 5.0 |
| Ogg Vorbis | 3 | 0.095 ms, 15.3 | **0.041 ms, 3.0** | 0.148 ms, 24.7 |
| Monkey's Audio | 28 | 0.054 ms, 5.0 | **0.032 ms, 2.0** | 0.054 ms, 6.0 |
| FLAC | 140 | 0.87 ms, 10.6 | **0.81 ms, 2.6** | 2.05 ms, 22.9 |

Two read calls (the start and the end of the file) are the minimum; the files with larger metadata need one more for every block that does not fit into
the cache. The time of the FLAC test files is dominated by the few files with metadata of many MB (the median of the FLAC files is 0.075 ms per file). The current
version recognizes 134 of the files, 2.0.4 136: the two files that 2.0.4 accepts in addition have no STREAMINFO block or other blocks in front of it, which
the FLAC format does not allow. AAC without a header (one file) reads the whole file in 64 KB blocks and counts the frames, because the duration is not known otherwise (TagLib:
1,505 read calls for the same file). No sample files of Ogg Opus, Musepack and TTA were available; they use the same readers.

On a network drive (SMB, about 16,000 files, first access, measured with 3.0.0) the analysis took about 33 ms per file with 10.2 read calls
(2.0.4: about 32 ms, 12.2 read calls). The cost of the first access to each file dominates there, so fewer system calls change
little. The same holds for a mechanical drive: 600 random files (590 MP3) of a SATA hard disk on USB 3 (16,500 files), first access
(the file system cache evicted before each run, a warm-up pass over other files not counted, two rounds in the reverse order), needed
24.5 to 27.7 ms per file with all libraries, AudioGenie3 24.8 ms with 2.7 read calls (differences of the order of the noise); the exception is FFmpeg with 18.2 ms, which does not read the end of an MP3 file and estimates the duration. The first access
of an AudioGenie 2.0.4 run was 55.8 ms (the drive was spinning up), 25.1 ms in the second round. The CPU time per file was 0.25 ms for AudioGenie3,
0.4 ms for TagLib, 0.8 ms for mutagen, 1.2 ms for tinytag and FFmpeg and 2.0 ms for MediaInfoLib.

The 32 and the 64 bit DLL return identical results. The tags and the technical data are the same as in version 2.0.4, except
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
| Analysis (`AUDIOAnalyzeFileW`): system calls | The length of the file is read with one call instead of about four (for the stream of the analysis not at all); absolute positions instead of seeks relative to the end of the file; a read buffer of 8 KB instead of 4 KB; the last 8 KB of the file are read once for ID3v1, Lyrics3, the APE footer and the last MPEG block instead of one seek and read each | local SSD, 22,876 files, cached, `tests/tools/run_scan.bat` (analysis only): 2.14 s -> 1.69 s (-21 %), kernel time 1.94 s -> 1.50 s, read calls per file 8.8 -> 3.9; including the reading of the fields (comparison above): -24 % |
| Text conversion (fields of many KB) | The end of a text is searched with `memchr`; a text that cannot fit into the text buffer (256 KB characters) is not converted at all (more than three times the buffer, or ASCII characters: one character per byte), where the system first looked at all of the text, twice (UTF-8, then ANSI) | the FLAC test file with 39 fields of 468 KB each: 25 -> 10 ms |
| Analysis: start of the file | The first 8 KB of the file are read once for the format check, the ID3v2 header and the first MPEG block (no seek on a freshly opened file); behind an ID3v2 tag larger than about 4.7 KB the cache is extended with one read to the end of the tag plus 8 KB (at most 256 KB), so tag, header and first block come from memory | same files, cached: read calls per file 3.9 -> 2.4 (-38 %), time per file 0.065 -> 0.058 ms (-11 %) |
| File access: class `CFile` (File.h) | The readers and the functions that write a file use a `CFile` and no longer the C library: the file is opened with `CreateFileW` and read with `ReadFile` with the offset in the request (one system call, no seek, no file pointer), written with `WriteFile`, with an own read buffer of 8 KB for the sequential reads; the writes are not buffered | local SSD, 22,876 files, cached: time per file 0.059 -> 0.054 ms (-7 %) |
| Other formats: the start of the file | FLAC, M4A (atoms), WAV (chunks), WavPack, TTA and the header of Monkey's Audio read from absolute positions through the cache of the start of the file, WMA, Musepack, the APE tag (also at the end of MP3 files, from the cache of the end of the file) and the Ogg pages read in sequence through the caches (`CSequentialRead`: no `ftell`, no second read of the start); an item or block that is not inside of the cache extends it with one read; reads of 8 KB and more go to the file directly (one read instead of two) | read calls per file: WMA 3.0 -> 2.0, M4A 6.7 -> 3.7, WAV 3.7 -> 1.9, WavPack 5 -> 2, Ogg 6 -> 3, Monkey's Audio 3 -> 2, FLAC 3.7 -> 2.6; time -10 to -22 %, FLAC unchanged |

A sampling profile of the analysis before the cache of the start of the file (warm cache, 22,876 files) shows where the time goes: 91 % is spent in system calls (opening the file 32 %,
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
