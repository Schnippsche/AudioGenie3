#!/usr/bin/env python3
"""Erzeugt MP3-/AAC-Fixtures mit von Hand gebauten ID3v2-Tags (unabhaengig von ffmpeg und von der DLL).

ffmpeg schreibt den Kommentar als TXXX:comment; hier wird ein echter COMM-Frame verwendet, und das Jahr steht wie in
der jeweiligen Tag-Version ueblich in TYER (v2.3) bzw. als Zeitstempel in TDRC (v2.4).

Aufruf: python make_id3_fixtures.py   (im Ordner tests/fixtures; Audio stammt aus mp3/no_tags_cbr.mp3 und aac/adts_sample-1.aac)
"""
import struct
from pathlib import Path

HERE = Path(__file__).resolve().parent


def syncsafe(n):
    return bytes([(n >> 21) & 0x7F, (n >> 14) & 0x7F, (n >> 7) & 0x7F, n & 0x7F])


def frame(version, fid, payload):
    size = struct.pack('>I', len(payload)) if version == 3 else syncsafe(len(payload))
    return fid.encode('ascii') + size + b'\x00\x00' + payload


def text(fid, value, version):
    return frame(version, fid, b'\x00' + value.encode('latin-1'))          # Kodierung 0 = ISO-8859-1


def comm(version, lang, desc, value):
    return frame(version, 'COMM', b'\x00' + lang.encode('ascii') + desc.encode('latin-1') + b'\x00' + value.encode('latin-1'))


def tag(version, year_id, year_value):
    frames = b''.join([
        text('TIT2', 'Testtitel', version), text('TPE1', 'Testkuenstler', version), text('TALB', 'Testalbum', version),
        text(year_id, year_value, version), text('TRCK', '3', version), text('TCON', 'Rock', version),
        comm(version, 'eng', '', 'Kommentar'), text('TCOM', 'Komponist', version),
    ])
    padding = b'\x00' * 64
    body = frames + padding
    return b'ID3' + bytes([version, 0, 0]) + syncsafe(len(body)) + body


def build(name, audio_file, version, year_id, year_value):
    src = HERE / audio_file
    if not src.exists():
        print('uebersprungen (Quelle fehlt):', name)
        return
    (HERE / name).write_bytes(tag(version, year_id, year_value) + src.read_bytes())
    print('erzeugt:', name)


if __name__ == '__main__':
    build('mp3/id3v23_comm.mp3', 'mp3/no_tags_cbr.mp3', 3, 'TYER', '2024')
    build('mp3/id3v24_comm.mp3', 'mp3/no_tags_cbr.mp3', 4, 'TDRC', '2024-05-01')
    build('aac/adts_id3v24_comm.aac', 'aac/adts_sample-1.aac', 4, 'TDRC', '2024')
