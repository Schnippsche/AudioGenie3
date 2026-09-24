# AudioGenie3 Tests

Tests laufen gegen die gebaute DLL ueber deren C-Schnittstelle (Catch2 v3, Amalgamation in `third_party/`).
Testdateien werden im Test erzeugt (WAV, MP3-Frames aus Stille) – es sind keine Fixtures im Repo.

```
tests\run.bat            # x86 und x64: Wrapper-Vertragspruefung + Catch2-Tests
tests\run.bat x64        # nur x64
tests\run.bat x64 "[wav]"   # nur Tests mit Tag [wav]
```

Voraussetzung: `Release\AudioGenie3.dll/.lib` (Win32) bzw. `x64\Release\...` sind gebaut
(`MSBuild AudioGenie3.vcxproj /p:Configuration=Release /p:Platform=Win32|x64`).

| Datei | Inhalt |
|---|---|
| `test_exports.cpp` | alle Exporte der `.def` sind in der DLL aufloesbar |
| `test_wav.cpp` | WAV-Analyse, LIST/INFO-Round-Trip, PCM-Daten unveraendert |
| `test_mpeg.cpp` | MP3-Analyse, ID3v2-Round-Trip, `AUDIOSetTitleW`, Tag entfernen |
| `test_robustness.cpp` | fehlende/leere/abgeschnittene/beschaedigte Dateien (synthetisch) |
| `test_formats.cpp` | alle Formate aus `fixtures/`: Eckdaten, Tags lesen/schreiben, Cover, kaputte Dateien, Fuzzing echter Dateien |
| `test_id3v2.cpp`, `test_id3v2_frames.cpp` | ID3v2-API: Round-Trips je Frame-Typ ueber v2.2/2.3/2.4 und die Kodierungen ISO-8859-1/UTF-16/UTF-16BE/UTF-8 (Text, URL, Kommentar, Lyrics, Bilder, Kapitel, Binaer- und Zahlenframes) |
| `test_fuzz_write.cpp` | Fuzzing der Schreibpfade: zufaellige Operationsfolgen (Tags, Bilder, Frames, Speichern) auf Fixture-Kopien und auf beschaedigten Kopien; Haenger-Waechter |
| `contract/check_wrappers.py` | Signaturen von C++, C#, VB.NET, Delphi, VB6 gegen `dllmain.cpp` |

Hinweise zur API (aus dem Code): `AUDIOSaveChangesW` schreibt die abstrakten Felder (Titel, Interpret ...)
und ueberschreibt dabei ID3v2-Frames; fuer Frame-Tests `ID3V2SaveChangesW` nehmen.
`ID3V2RemoveTagW` wirkt sofort auf die Datei.

## Encoder-Eigenheiten (`kQuirks`)

Die frueheren "Lese-Luecken" (Jahr `TDRC`, Vorbis `DESCRIPTION`, APE `date`, WMA `Description`/`date`, WAV `IPRT`) sind in der DLL behoben
(Fallbacks beim Lesen, siehe Commit). Uebrig ist eine Eigenheit von ffmpeg, keine DLL-Luecke: ffmpeg schreibt den Kommentar in ID3v2-Tags als
`TXXX:comment` statt als `COMM`-Frame, die DLL liest `COMM`. Diese Faelle stehen in `kQuirks` (Kommentar erwartet leer); fuer echte
`COMM`-Frames erzeugt `fixtures/make_id3_fixtures.py` von Hand gebaute ID3v2.3-/2.4-Tags (`mp3/id3v23_comm.mp3`, `mp3/id3v24_comm.mp3`,
`aac/adts_id3v24_comm.aac`).

## AddressSanitizer (`run_asan.bat`)

`tests\run_asan.bat [x64|x86] [Catch2-Argumente]` baut die DLL (nach `build-asan\<arch>\`, die normalen Builds bleiben
unberuehrt, COM-Registrierung abgeschaltet) **und** die Tests mit `/fsanitize=address` und fuehrt sie aus. Ein Speicherfehler
bricht mit Stacktrace samt Zeile ab. Laengerer Fuzz-Lauf: `set AG3_FUZZ_ROUNDS=40` (Standard 1).

Per ASan gefundene und behobene Fehler (Regressionsdateien in `fixtures/broken/`, Test `[asan-regression]`):

| Befund | Ort | Regression |
|---|---|---|
| FLAC: PICTURE-Block las `ln` Bytes ohne Pruefung der Blocklaenge (heap-buffer-overflow) | `FlacCover.cpp` | `broken/flac_cover_length_overflow.flac` |
| WavPack: Sample-Rate-Index 15 ausserhalb der Tabelle (global-buffer-overflow) | `WavPack.cpp` `GetSampleRate` | `broken/wavpack_samplerate_index.wv` |
| `CMP4Atom` ohne virtuellen Destruktor (`delete` ueber Basiszeiger) | `MP4Atom.h` | jede M4A-Datei (zweite Analyse) |

Hinweis: Am Ende eines Testlaufs setzt ein Catch2-Listener (`support.cpp`) die DLL zurueck. Ohne das bricht der Prozess unter ASan
beim Beenden ab (0xC0000409), wenn zuletzt eine OGG-/FLAC-Datei analysiert wurde: die statischen Objekte der DLL geben ihren Speicher
erst nach dem Abbau der ASan-Laufzeit frei. Das ist ein Artefakt der Kombination aus statischer CRT und ASan, kein Fehler im Code.

## Bewusste Designentscheidungen (keine Fehler)

Diese Verhaeltnisse sind gewollt und in den Tests so festgehalten; sie nicht als Befund melden.

- **Erkennung: Endung nur als Vorfilter (Performance).** Die meisten Formate werden am Dateiinhalt erkannt (Signaturen in
  `Header.h`), auch bei falscher Endung (z. B. MP4 mit Endung `.aac` wird als M4A erkannt). Nur bei den "Wackelkandidaten"
  ohne eindeutigen Header wird die Endung vorab geprueft (`dllmain.cpp`, `AUDIOAnalyzeFileW`): **AAC** (`.aac`) und
  **MPEG-Audio** (`.mp3`, `.mp2`, `.mp1`, `.msf`, `.mp3~`). Eine MP3-Datei mit anderer Endung wird deshalb nicht erkannt.
- **`AUDIOSaveChangesW` schreibt je nach Format unterschiedlich** (Tabelle in der Doku): AAC, WavPack und MONKEY als APE-Tag,
  MP3/MPC/TTA als ID3v2. Ein vorhandener ID3v2-Tag am Dateianfang einer AAC-Datei bleibt dabei unveraendert und hat beim Lesen
  Vorrang (Test "AAC mit vorhandenem ID3v2-Tag ...").
- **`AUDIOSaveChangesW` ueberschreibt ID3v2-Frames** aus den abstrakten Feldern (Titel, Interpret ...); fuer Frame-Tests
  `ID3V2SaveChangesW` verwenden. `ID3V2RemoveTagW` wirkt sofort auf die Datei.
- **Einzelthread-DLL mit globalem Zustand:** Erst `AUDIOAnalyzeFileW`, dann arbeiten alle anderen Funktionen auf dem
  gemerkten Zustand. Die Tests analysieren deshalb vor jedem Zugriff die betroffene Datei neu.

## Fuzzing und Haenger

- `AG3_FUZZ_ROUNDS=N` vervielfacht die Durchlaeufe der Fuzz-Tests (`[fuzz]`, `[robust]`); Standard 1. Fuer lange Laeufe z. B. 20.
- Fuzz-Dateien behalten ihre Endung: MP3 und AAC werden nur damit erkannt (Endung als Vorfilter, siehe oben).
- Jeder Bearbeitungszyklus in `test_fuzz_write.cpp` steht unter einem Waechter: dauert er laenger als 30 s, wird die Eingabe als
  `%TEMP%\ag3tests\hang_input.<ext>` gesichert und der Prozess mit Exitcode 99 beendet. `AG3_FUZZ_TRACE=1` protokolliert
  die Schritte nach `%TEMP%\ag3tests\trace.log`.
- `mustFinishWithin` (test_formats.cpp) macht dasselbe fuer einzelne Aufrufe (Exitcode 98).
- Bisher gefundener Haenger (behoben): `stco`-Atom mit ueberlangem `entry_count` liess `AUDIOSaveChangesW` bei M4A praktisch endlos
  laufen (`MP4_STCO.cpp`); Regression `broken/mp4_stco_count_overflow.m4a`.

## Musepack

- **SV7** (`MP+`): mpcenc 1.30 schreibt kein SV7 mehr; `fixtures/make_mpc_fixtures.py` baut synthetische SV7-Dateien (24-Byte-Header mit
  bekannten Werten, Dummy-Frames, optional APEv2-/ID3v2-Tag). Sie pruefen das Header-Parsing (Samplerate, Dauer, Tags), keine Dekodierung.
- **SV8** (`MPCK`): `generate.bat` erzeugt echte Dateien mit `mpcenc.exe` (Pfad ueber `MPCENC`, Standard
  `D:\Entwicklung\Musepack\64bit\mpcenc.exe`); `make_mpc_fixtures.py` haengt APEv2- bzw. ID3v2-Tags an. Die DLL liest SV8 (Paket-Parser fuer
  SH/EI in `MPEGPlus.cpp`): Samplerate, Kanaele inkl. Mono, Dauer aus der Samplezahl, Profil, Bitrate.
