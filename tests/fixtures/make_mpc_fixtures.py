#!/usr/bin/env python3
"""Musepack-Fixtures.

* SV8 (aktuelles Format, 'MPCK'): erzeugt mpcenc 1.30 (siehe generate.bat) - hier nur umbenannt/vorhanden.
* SV7 ('MP+' + 0x07): mpcenc 1.30 schreibt kein SV7 mehr. Es werden daher *synthetische* SV7-Dateien gebaut:
  ein 24-Byte-Header mit bekannten Werten (Frames, Samplerate, Profil, Stereo/Joint) und Dummy-Frames. Sie pruefen das
  Header-Parsing der DLL, keine echte Audio-Dekodierung. Optional mit APEv2- oder ID3v2-Tag.

Aufruf: python make_mpc_fixtures.py   (im Ordner tests/fixtures)
"""
import struct
from pathlib import Path

from make_id3_fixtures import tag as id3_tag   # ID3v2-Tag mit Standardfeldern (Titel, Interpret, ..., COMM)

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
    """APEv2-Tag mit Header und Footer; items = [(Schluessel, Text)]."""
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
    return bytes((i * 31) & 0xFF for i in range(frames * 8))   # Dummy-Daten, keine echten MPC-Frames


def write(name, data):
    (HERE / 'mpc' / name).write_bytes(data)
    print('erzeugt: mpc/' + name)


if __name__ == '__main__':
    (HERE / 'mpc').mkdir(exist_ok=True)
    frames = 100                                    # 100 * 1152 / 44100 = 2,612 s
    base = sv7_header(frames, 44100, 'standard', joint=False) + payload(frames)
    write('sv7_synthetic_standard.mpc', base)
    write('sv7_synthetic_thumb_joint_48k.mpc', sv7_header(200, 48000, 'thumb', joint=True) + payload(200))
    write('sv7_synthetic_insane_32k.mpc', sv7_header(50, 32000, 'insane', joint=False) + payload(50))
    write('sv7_synthetic_tagged_ape.mpc', base + ape_tag(STD_ITEMS))
    write('sv7_synthetic_tagged_id3v2.mpc', id3_tag(3, 'TYER', '2024') + base)
