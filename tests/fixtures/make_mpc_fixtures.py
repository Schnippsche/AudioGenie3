#!/usr/bin/env python3
"""Musepack-Fixtures.

* SV8 (current format, 'MPCK'): created by mpcenc 1.30 (see generate.bat) - only renamed/present here.
* SV7 ('MP+' + 0x07): real files are created by mppenc 1.16 (see generate.bat). In addition, *synthetic* SV7 files are built:
  a 24-byte header with known values (frames, sample rate, profile, stereo/joint) and dummy frames. They test the
  header parsing of the DLL, not real audio decoding. Optionally with an APEv2 or ID3v2 tag.

Aufruf: python make_mpc_fixtures.py   (im Ordner tests/fixtures)
"""
import struct
from pathlib import Path

from make_id3_fixtures import tag as id3_tag   # ID3v2 tag with standard fields (title, artist, ..., COMM)

HERE = Path(__file__).resolve().parent

SAMPLE_RATE_IDX = {44100: 0, 48000: 1, 37800: 2, 32000: 3}
PROFILE_IDX = {'thumb': 8, 'radio': 9, 'standard': 10, 'xtreme': 11, 'insane': 12}


def sv7_header(frames, sample_rate, profile, joint):
    """24 Byte: 'MP+' 0x07, Frames(LE32), MaxLevel(16), Byte10 = Profil<<4 | Samplerate-Index, Byte11 = Joint-Stereo-Flag."""
    h = bytearray(24)
    h[0:3] = b'MP+'
    h[3] = 0x07
    struct.pack_into('<I', h, 4, frames)
    h[10] = (PROFILE_IDX[profile] << 4) | SAMPLE_RATE_IDX[sample_rate]
    h[11] = 0x40 if joint else 0x00
    return bytes(h)


def ape_tag(items):
    """APEv2 tag with header and footer; items = [(key, text)]."""
    body = b''
    for key, value in items:
        v = value.encode('utf-8')
        body += struct.pack('<II', len(v), 0) + key.encode('ascii') + b'\x00' + v
    size = len(body) + 32                      # Items + Footer
    def block(flags):
        return b'APETAGEX' + struct.pack('<IIII', 2000, size, len(items), flags) + b'\x00' * 8
    return block(0xA0000000) + body + block(0x80000000)


STD_ITEMS = [('Title', 'Testtitel'), ('Artist', 'Testkuenstler'), ('Album', 'Testalbum'), ('Year', '2024'),
             ('Track', '3'), ('Genre', 'Rock'), ('Comment', 'Kommentar')]


def payload(frames):
    return bytes((i * 31) & 0xFF for i in range(frames * 8))   # dummy data, no real MPC frames


def write(name, data):
    (HERE / 'mpc' / name).write_bytes(data)
    print('created: mpc/' + name)


if __name__ == '__main__':
    (HERE / 'mpc').mkdir(exist_ok=True)
    frames = 100                                    # 100 * 1152 / 44100 = 2,612 s
    base = sv7_header(frames, 44100, 'standard', joint=False) + payload(frames)
    write('sv7_synthetic_standard.mpc', base)
    write('sv7_synthetic_thumb_joint_48k.mpc', sv7_header(200, 48000, 'thumb', joint=True) + payload(200))
    write('sv7_synthetic_insane_32k.mpc', sv7_header(50, 32000, 'insane', joint=False) + payload(50))
    write('sv7_synthetic_tagged_ape.mpc', base + ape_tag(STD_ITEMS))
    write('sv7_synthetic_tagged_id3v2.mpc', id3_tag(3, 'TYER', '2024') + base)
    # SV7 real: from the file created by mppenc 1.16 (see generate.bat) with an appended APEv2 or a prepended ID3v2 tag
    sv7 = HERE / 'mpc' / 'sv7_standard.mpc'
    if sv7.exists():
        write('sv7_tagged_ape.mpc', sv7.read_bytes() + ape_tag(STD_ITEMS))
        write('sv7_tagged_id3v2.mpc', id3_tag(3, 'TYER', '2024') + sv7.read_bytes())
    # SV8: from the file created by mpcenc (see generate.bat) with an appended APEv2 tag
    sv8 = HERE / 'mpc' / 'sv8_standard.mpc'
    if sv8.exists():
        write('sv8_tagged_ape.mpc', sv8.read_bytes() + ape_tag(STD_ITEMS))
        write('sv8_tagged_id3v2.mpc', id3_tag(3, 'TYER', '2024') + sv8.read_bytes())
