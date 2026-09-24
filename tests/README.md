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
| `test_robustness.cpp` | fehlende/leere/abgeschnittene/beschaedigte Dateien |
| `contract/check_wrappers.py` | Signaturen von C++, C#, VB.NET, Delphi, VB6 gegen `dllmain.cpp` |

Hinweise zur API (aus dem Code): `AUDIOSaveChangesW` schreibt die abstrakten Felder (Titel, Interpret ...)
und ueberschreibt dabei ID3v2-Frames; fuer Frame-Tests `ID3V2SaveChangesW` nehmen.
`ID3V2RemoveTagW` wirkt sofort auf die Datei.
