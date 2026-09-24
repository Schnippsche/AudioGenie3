# AudioGenie3 Tests

The tests run against the built DLL through its C interface (Catch2 v3, amalgamation in `third_party/`).
Some test files are generated inside the tests (WAV, MP3 frames from silence); the audio fixtures for all formats are in `fixtures/`.

```
tests\run.bat            # x86 and x64: wrapper contract check + Catch2 tests
tests\run.bat x64        # x64 only
tests\run.bat x64 "[wav]"   # only tests with the tag [wav]
```

Prerequisite: `Release\AudioGenie3.dll/.lib` (Win32) and `x64\Release\...` are built
(`MSBuild AudioGenie3.vcxproj /p:Configuration=Release /p:Platform=Win32|x64`).

| File | Content |
|---|---|
| `test_exports.cpp` | all exports of the `.def` can be resolved in the DLL |
| `test_wav.cpp` | WAV analysis, LIST/INFO round trip, PCM data unchanged |
| `test_mpeg.cpp` | MP3 analysis, ID3v2 round trip, `AUDIOSetTitleW`, remove tag |
| `test_robustness.cpp` | missing/empty/truncated/damaged files (synthetic) |
| `test_formats.cpp` | all formats from `fixtures/`: basic data, read/write tags, cover, broken files, fuzzing of real files |
| `test_id3v2.cpp`, `test_id3v2_frames.cpp` | ID3v2 API: round trips per frame type across v2.2/2.3/2.4 and the encodings ISO-8859-1/UTF-16/UTF-16BE/UTF-8 (text, URL, comment, lyrics, pictures, chapters, binary and numeric frames) |
| `test_special.cpp` | special cases: Unicode/special-character file names, upper/lower case extensions, long paths (up to 1500 characters with `\\?\`), invalid paths, tiny files, read-only files and locks, sparse files of 3 and 5 GB |
| `test_api_misuse.cpp` | every exported function with random, partly invalid arguments (NULL, huge strings, index 0/-1/32767, foreign frame IDs) on one file per format; the call list `misuse_calls.inc` is generated from the header by `gen_misuse.py` |
| `test_fuzz_write.cpp` | fuzzing of the write paths: random operation sequences (tags, pictures, frames, saving) on fixture copies and on damaged copies; hang watchdog |
| `contract/check_wrappers.py` | signatures of C++, C#, VB.NET, Delphi, VB6 against `dllmain.cpp` |

Notes on the API (from the code): `AUDIOSaveChangesW` writes the abstract fields (title, artist ...)
and thereby overwrites ID3v2 frames; use `ID3V2SaveChangesW` for frame tests.
`ID3V2RemoveTagW` takes effect on the file immediately.

The test data values (`Testtitel`, `Testkuenstler`, `Kommentar` ...) are deliberately kept as they are: they are written into the fixtures
by `fixtures/generate.bat` and `fixtures/make_*.py` and read back by the tests.

## Encoder quirks (`kQuirks`)

The earlier "read gaps" (year `TDRC`, Vorbis `DESCRIPTION`, APE `date`, WMA `Description`/`date`, WAV `IPRT`) are fixed in the DLL
(fallbacks when reading, see the commit). What remains is a quirk of ffmpeg, not a DLL gap: ffmpeg writes the comment in ID3v2 tags as
`TXXX:comment` instead of a `COMM` frame, while the DLL reads `COMM`. These cases are listed in `kQuirks` (comment expected to be empty); for real
`COMM` frames `fixtures/make_id3_fixtures.py` builds ID3v2.3/2.4 tags by hand (`mp3/id3v23_comm.mp3`, `mp3/id3v24_comm.mp3`,
`aac/adts_id3v24_comm.aac`).

## AddressSanitizer (`run_asan.bat`)

`tests\run_asan.bat [x64|x86] [Catch2 arguments]` builds the DLL (into `build-asan\<arch>\`, the normal builds stay
untouched, COM registration is switched off) **and** the tests with `/fsanitize=address` and runs them. A memory error
aborts with a stack trace including the line. Longer fuzz run: `set AG3_FUZZ_ROUNDS=40` (default 1).

Errors found by ASan and fixed (regression files in `fixtures/broken/`, test `[asan-regression]`):

| Finding | Location | Regression |
|---|---|---|
| FLAC: PICTURE block read `ln` bytes without checking the block length (heap-buffer-overflow) | `FlacCover.cpp` | `broken/flac_cover_length_overflow.flac` |
| WavPack: sample rate index 15 outside the table (global-buffer-overflow) | `WavPack.cpp` `GetSampleRate` | `broken/wavpack_samplerate_index.wv` |
| `CMP4Atom` without a virtual destructor (`delete` through a base pointer) | `MP4Atom.h` | every M4A file (second analysis) |

Note: at the end of a test run a Catch2 listener (`support.cpp`) resets the DLL. Without it the process aborts under ASan
on exit (0xC0000409) if an OGG/FLAC file was analyzed last: the static objects of the DLL only release their memory
after the ASan runtime has been torn down. This is an artifact of the combination of static CRT and ASan, not an error in the code.

## Deliberate design decisions (not errors)

These conditions are intended and recorded in the tests as such; do not report them as findings.

- **Detection: the extension is only a pre-filter (performance).** Most formats are recognized by the file content (signatures in
  `Header.h`), even with a wrong extension (e.g. MP4 with the extension `.aac` is recognized as M4A). Only for the "uncertain candidates"
  without an unambiguous header is the extension checked first (`dllmain.cpp`, `AUDIOAnalyzeFileW`): **AAC** (`.aac`) and
  **MPEG audio** (`.mp3`, `.mp2`, `.mp1`, `.msf`, `.mp3~`). An MP3 file with another extension is therefore not recognized.
- **`AUDIOSaveChangesW` writes differently depending on the format** (table in the documentation): AAC, WavPack and MONKEY as APE tag,
  MP3/MPC/TTA as ID3v2. An existing ID3v2 tag at the start of an AAC file stays unchanged and takes precedence when reading
  (test "AAC with an existing ID3v2 tag ...").
- **`AUDIOSaveChangesW` overwrites ID3v2 frames** from the abstract fields (title, artist ...); use `ID3V2SaveChangesW`
  for frame tests. `ID3V2RemoveTagW` takes effect on the file immediately.
- **Single-threaded DLL with global state:** first `AUDIOAnalyzeFileW`, then all other functions work on the remembered
  state. The tests therefore analyze the affected file again before each access.

## Fuzzing and hangs

- `AG3_FUZZ_ROUNDS=N` multiplies the iterations of the fuzz tests (`[fuzz]`, `[robust]`); default 1. For long runs e.g. 20.
- Fuzz files keep their extension: MP3 and AAC are only recognized with it (extension as pre-filter, see above).
- Every edit cycle in `test_fuzz_write.cpp` is guarded by a watchdog: if it takes longer than 30 s, the input is saved as
  `%TEMP%\ag3tests\hang_input.<ext>` and the process ends with exit code 99. `AG3_FUZZ_TRACE=1` logs the steps
  to `%TEMP%\ag3tests\trace.log`.
- `mustFinishWithin` (test_formats.cpp) does the same for single calls (exit code 98).
- Hang found so far (fixed): a `stco` atom with an oversized `entry_count` made `AUDIOSaveChangesW` run practically forever on M4A
  (`MP4_STCO.cpp`); regression `broken/mp4_stco_count_overflow.m4a`.

## Musepack

- **SV7** (`MP+`): real files from `mppenc.exe` 1.16 (path via `MPPENC`, default `D:\Entwicklung\Musepack7\mppenc.exe`): all five profiles,
  44.1/48/32 kHz, stream version 7 (`0x07`) and 7.1 (`0x17`); duration and sample rate match ffprobe. SV7 has no mono
  (a mono input is encoded as stereo). In addition `fixtures/make_mpc_fixtures.py` builds *synthetic* SV7 headers (known values,
  dummy frames) and appends APEv2/ID3v2 tags to real files.
- **SV8** (`MPCK`): `generate.bat` creates real files with `mpcenc.exe` 1.30 (path via `MPCENC`, default
  `D:\Entwicklung\Musepack\64bit\mpcenc.exe`). The DLL reads SV8 (packet parser for SH/EI in `MPEGPlus.cpp`): sample rate,
  channels including mono, duration from the sample count, profile, bit rate.

## Special cases: what applies

- **Paths:** Unicode (umlauts, CJK, Cyrillic, emoji), special characters and several dots in the name work. Long paths work with
  the prefix `\\?\` (tested up to 1500 characters); without the prefix the Windows limit of 260 characters applies.
- **Extension:** checked as a pre-filter for MP3/AAC, also in upper and mixed case (`.MP3`, `.Aac`).
- **Read-only/locks:** reading works, `AUDIOSaveChangesW` returns 0 and leaves the file unchanged; an exclusive lock already prevents
  reading (format 0).
- **Large files:** analysis works with 3 GB (WAV, MP3 with ID3v1 at the end) and 5 GB (FLAC, WavPack with APE tag at the end);
  `AUDIOGetFileSizeW` returns a `long` and is clamped to 2147483647 from 2 GiB on (intended).
- **`AUDIOSaveChangesW` after a failed analysis:** writes nothing (formerly: the emptied fields into the previously analyzed file).
- `AG3_MISUSE_TRACE=1` logs every call of the misuse test to `%TEMP%/ag3tests/misuse_trace.log` (last line = crash cause),
  `AG3_MISUSE_SKIP=Fn1,Fn2` skips functions, `AG3_FUZZ_ROUNDS` increases the calls per function (default 30).

## Encoding and line endings

`.editorconfig` defines the encoding per area: DLL source code, wrappers and example projects are Windows-1252 (`latin1`; VB6 and older
Delphi versions expect ANSI), tests and scripts UTF-8 (`tests/run.bat` compiles with `/utf-8`). The generated Doxygen documentation
is UTF-8. `.gitattributes` stores text with LF in the repository and checks out Windows sources and batch files with CRLF.
String literals in the DLL sources are ASCII; comments in all sources are English (the only non-ASCII characters are the
`(c)` symbol in the MP4 atom names and a name in the MD5 copyright).

## API documentation (Doxygen)

The API documentation is in Doxygen comments in `dllmain.cpp` (English only, groups in `doxygen_groups.dox`). Regenerate it with:
`doxygen Doxyfile` (output to `doku/`, warnings to `doku/doxygen-warnings.txt`). `doku/` is generated and must not be edited by hand.
