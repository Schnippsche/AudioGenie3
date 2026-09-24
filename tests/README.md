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
| `contract/check_wrappers.py` | Signaturen von C++, C#, VB.NET, Delphi, VB6 gegen `dllmain.cpp` |

Hinweise zur API (aus dem Code): `AUDIOSaveChangesW` schreibt die abstrakten Felder (Titel, Interpret ...)
und ueberschreibt dabei ID3v2-Frames; fuer Frame-Tests `ID3V2SaveChangesW` nehmen.
`ID3V2RemoveTagW` wirkt sofort auf die Datei.

## Bekannte Luecken (`[gaps]`)

`test_formats.cpp` fuehrt in `kGaps` Felder auf, die ffmpeg schreibt, die DLL aber nicht liefert (z. B. ID3v2.4-Jahr `TDRC`,
Vorbis-`COMMENT`, APE-`date`). Der Test `[gaps]` ist mit `[!shouldfail]` markiert: er gilt als bestanden, solange die Luecken
bestehen, und schlaegt um, sobald eine geschlossen wird – dann den Eintrag aus `kGaps` entfernen.

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
