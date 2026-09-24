# Test files

One subfolder per format; broken or truncated files in `broken/`.

- Keep them small (a few seconds, preferably < 200 KB): every file stays in the Git history.
- Only add files that may be redistributed.
- Use descriptive names, e.g. `stereo_44k_tagged.flac`, `no_tags.m4a`, `with_cover.mp3`.
- Useful variants per format: without tags, with tags, with cover, mono/stereo.

The `.gitkeep` files can be removed as soon as a folder contains files.

## Creating them

`generate.bat` creates all files reproducibly (1 s sine tone, bitexact) including truncated variants in `broken/`:
ffmpeg for MP3/WAV/FLAC/OGG/M4A/WMA/WavPack/TTA/AAC, `mac.exe` (Monkey's Audio) for APE, `mppenc.exe`/`mpcenc.exe` for Musepack
SV7/SV8, and the Python scripts `make_id3_fixtures.py` and `make_mpc_fixtures.py` for hand-built tags and synthetic SV7 headers.
Missing tools are skipped with a note. The OGG files cannot be reproduced bit-identically (ffmpeg randomizes the stream serial number),
so only regenerate them when necessary. The originals of the long AAC samples are kept unversioned in `tests/fixtures_local/`.
