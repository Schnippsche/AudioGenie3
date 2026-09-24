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

Bekannte Befunde (Stand jetzt, noch nicht behoben):

| Befund | Ort | Reproduktion |
|---|---|---|
| FLAC: PICTURE-Block liest `ln` Bytes ohne Pruefung der Blocklaenge (heap-buffer-overflow, Lesen) | `FlacCover.cpp` (`AddMemory(..., ln)`) | `broken/flac_cover_length_overflow.flac`, Tag `[known-asan]` |
| WavPack: Sample-Rate-Index 15 liegt ausserhalb der Tabelle (global-buffer-overflow) | `WavPack.cpp` `GetSampleRate` | `broken/wavpack_samplerate_index.wv`, Tag `[known-asan]` |
| `CMP4Atom` hat keinen virtuellen Destruktor (`delete` ueber Basiszeiger, new-delete-type-mismatch) | `MP4Atom.h`, `MP4_Container.cpp:252` | jede M4A-Datei, zweite Analyse; per `ASAN_OPTIONS=new_delete_type_mismatch=0` ausgeblendet |

Die `[known-asan]`-Tests sind im Standardlauf ausgeschlossen; `tests\run_asan.bat x64 [known-asan]` fuehrt sie aus und bricht ab, bis der
Fehler behoben ist. Den Ausschluss (und die `ASAN_OPTIONS`-Zeile in `run_asan.bat`) nach dem Beheben entfernen.

Hinweis: Am Ende eines Testlaufs setzt ein Catch2-Listener (`support.cpp`) die DLL zurueck. Ohne das bricht der Prozess unter ASan
beim Beenden ab (0xC0000409), wenn zuletzt eine OGG-/FLAC-Datei analysiert wurde: die statischen Objekte der DLL geben ihren Speicher
erst nach dem Abbau der ASan-Laufzeit frei. Das ist ein Artefakt der Kombination aus statischer CRT und ASan, kein Fehler im Code.
