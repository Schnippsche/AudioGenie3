# Testdateien

Ein Unterordner pro Format; kaputte oder abgeschnittene Dateien in `broken/`.

- Klein halten (wenige Sekunden, moeglichst < 200 KB): jede Datei bleibt in der Git-Historie.
- Nur Dateien ablegen, die weitergegeben werden duerfen.
- Sprechende Namen, z. B. `stereo_44k_tagged.flac`, `no_tags.m4a`, `with_cover.mp3`.
- Sinnvolle Varianten je Format: ohne Tags, mit Tags, mit Cover, Mono/Stereo.

Die `.gitkeep`-Dateien koennen entfernt werden, sobald ein Ordner Dateien enthaelt.

## Erzeugen

`generate.bat` erzeugt alle ffmpeg-faehigen Dateien reproduzierbar (1 s Sinuston, bitexact) inklusive
abgeschnittener Varianten in `broken/`. Nicht damit erzeugbar: APE (`mac.exe`) und MPC (`mpcenc.exe`) –
diese Dateien von Hand in `ape\` bzw. `mpc\` ablegen.
