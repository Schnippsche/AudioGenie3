"""Compares AudioGenie3 (x64 DLL) with the Python libraries mutagen and pymediainfo: technical data (format, duration,
bit rate, sample rate, channels), the common tags (title, artist) and which tag containers are physically present in the
file (ID3v1/ID3v2/APEv2 for MP3, detected independently by reading the raw bytes), plus the time each library takes.

Usage:
    python tests/tools/compare_libs.py <file_list.txt> [--limit N] [--seed N] [--out prefix]

<file_list.txt>: one path per line (UTF-8), e.g. tests/out/scan_all/x64/files.txt (the full drive listing scan_all.cpp
writes). Only files whose extension is one AudioGenie3 recognizes as audio are used from that list. --limit takes a
random sample of that size (stratified: proportional to how common each extension is) instead of all files.
"""
import argparse
import ctypes
import json
import os
import random
import struct
import sys
import time

AUDIO_EXT = ['.mp3', '.mp2', '.mp1', '.wma', '.m4a', '.ogg', '.opus', '.flac', '.wav', '.ape', '.mpc', '.tta', '.wv', '.aac']
DLL_PATH = r"D:\Entwicklung\AudioGenie3\x64\Release\AudioGenie3.dll"

FORMAT_NAMES = {0: 'unknown', 1: 'MPEG', 2: 'WMA', 3: 'MONKEY', 4: 'FLAC', 5: 'WAV', 6: 'OGGVORBIS', 7: 'MPEGPLUS',
                8: 'AAC', 9: 'MP4M4A', 10: 'TTA', 11: 'WAVPACK', 12: 'ID3V2', 13: 'OGGOPUS'}


# ---------------------------------------------------------------------------------------------------------------------
# AudioGenie3
# ---------------------------------------------------------------------------------------------------------------------

class AudioGenie:
    def __init__(self):
        self.dll = ctypes.WinDLL(DLL_PATH)
        W = ctypes.c_wchar_p
        d = self.dll
        d.AUDIOAnalyzeFileW.argtypes = [W]; d.AUDIOAnalyzeFileW.restype = ctypes.c_long
        d.AUDIOGetDurationW.restype = ctypes.c_float
        d.AUDIOGetBitrateW.restype = ctypes.c_long
        d.AUDIOGetSampleRateW.restype = ctypes.c_long
        d.AUDIOGetChannelsW.restype = ctypes.c_long
        d.AUDIOIsValidFormatW.restype = ctypes.c_short
        d.AUDIOGetTitleW.restype = ctypes.c_wchar_p
        d.AUDIOGetArtistW.restype = ctypes.c_wchar_p
        d.AUDIOGetLastErrorNumberW.restype = ctypes.c_long
        d.ID3V2GetVersionW.restype = ctypes.c_wchar_p
        d.APEExistsW.restype = ctypes.c_short
        d.ID3V1ExistsW.restype = ctypes.c_short

    def analyze(self, path):
        d = self.dll
        fmt = d.AUDIOAnalyzeFileW(path)
        out = {
            'format': FORMAT_NAMES.get(fmt, str(fmt)),
            'valid': bool(d.AUDIOIsValidFormatW()),
            'duration': d.AUDIOGetDurationW(),
            'bitrate': d.AUDIOGetBitrateW(),
            'samplerate': d.AUDIOGetSampleRateW(),
            'channels': d.AUDIOGetChannelsW(),
            'title': d.AUDIOGetTitleW() or '',
            'artist': d.AUDIOGetArtistW() or '',
            'lasterror': d.AUDIOGetLastErrorNumberW(),
        }
        tags = []
        if fmt == 1:  # MPEG: several tag types can coexist
            if (d.ID3V2GetVersionW() or ''):
                tags.append('ID3v2')
            if d.APEExistsW():
                tags.append('APE')
            if d.ID3V1ExistsW():
                tags.append('ID3v1')
        elif out['title'] or out['artist']:
            tags.append(out['format'])
        out['tags'] = tags
        return out


# ---------------------------------------------------------------------------------------------------------------------
# raw-byte ground truth for MP3 tag containers (independent of any library, to check discrepancies)
# ---------------------------------------------------------------------------------------------------------------------

def raw_tag_presence(path):
    present = set()
    try:
        with open(path, 'rb') as f:
            head = f.read(10)
            if head[:3] == b'ID3':
                present.add('ID3v2')
            size = os.path.getsize(path)
            tail_len = min(size, 256 * 1024)
            f.seek(-tail_len, os.SEEK_END)
            tail = f.read()
            if tail[-128:-125] == b'TAG':
                present.add('ID3v1')
            if b'APETAGEX' in tail:
                present.add('APE')
            if b'LYRICSBEGIN' in tail or b'LYRICS200' in tail:
                present.add('Lyrics3')
    except OSError:
        pass
    return present


# ---------------------------------------------------------------------------------------------------------------------
# mutagen
# ---------------------------------------------------------------------------------------------------------------------

def mutagen_analyze(path):
    import mutagen
    mf = mutagen.File(path, easy=True)
    if mf is None:
        return {'format': 'unsupported', 'valid': False}
    info = mf.info
    length = getattr(info, 'length', 0.0) or 0.0
    bitrate = getattr(info, 'bitrate', 0) or 0
    sr = getattr(info, 'sample_rate', None)
    if sr is None:
        sr = getattr(info, 'samplerate', 0) or 0
    channels = getattr(info, 'channels', 0) or 0
    title = ''
    artist = ''
    tagclass = ''
    if mf.tags is not None:
        tagclass = type(mf.tags).__name__
        try:
            title = str((mf.tags.get('title') or [''])[0])
        except Exception:
            title = ''
        try:
            artist = str((mf.tags.get('artist') or [''])[0])
        except Exception:
            artist = ''
    out = {
        'format': type(mf).__name__,
        'valid': True,
        'duration': float(length),
        'bitrate': int(round(bitrate / 1000.0)),
        'samplerate': int(sr),
        'channels': int(channels),
        'title': title,
        'artist': artist,
        'tags': [tagclass] if tagclass else [],
    }
    # for MP3 check APEv2 co-existence explicitly (mutagen.File only exposes ID3 through .tags)
    if isinstance(mf, __import__('mutagen.mp3', fromlist=['MP3']).MP3):
        try:
            from mutagen.apev2 import APEv2
            APEv2(path)
            out['tags'].append('APEv2')
        except Exception:
            pass
    return out


# ---------------------------------------------------------------------------------------------------------------------
# pymediainfo
# ---------------------------------------------------------------------------------------------------------------------

def pymediainfo_analyze(path, mi_cls):
    media = mi_cls.parse(path)
    audio = next((t for t in media.tracks if t.track_type == 'Audio'), None)
    general = next((t for t in media.tracks if t.track_type == 'General'), None)
    if audio is None:
        return {'format': 'unsupported', 'valid': False}
    duration_s = (float(audio.duration) / 1000.0) if audio.duration else (float(general.duration) / 1000.0 if general and general.duration else 0.0)
    bitrate = audio.bit_rate or (general.overall_bit_rate if general else 0) or 0
    sr = audio.sampling_rate or 0
    channels = audio.channel_s or 0
    title = (general.title if general else None) or ''
    artist = (general.performer if general else None) or ''
    tag_fields = []
    if general:
        data = general.to_data()
        for key in ('title', 'performer', 'album', 'track_name', 'genre', 'recorded_date'):
            if data.get(key):
                tag_fields.append(key)
    return {
        'format': (general.format if general else '') or audio.format or '',
        'valid': True,
        'duration': duration_s,
        'bitrate': int(round(float(bitrate) / 1000.0)),
        'samplerate': int(sr),
        'channels': int(channels),
        'title': str(title),
        'artist': str(artist),
        'tags': tag_fields,
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('list_file')
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('--seed', type=int, default=42)
    ap.add_argument('--out', default=r'tests\out\complib')
    args = ap.parse_args()

    files = []
    with open(args.list_file, encoding='utf-8', errors='replace') as f:
        for line in f:
            p = line.rstrip('\n')
            ext = os.path.splitext(p)[1].lower()
            if ext in AUDIO_EXT:
                files.append(p)
    print(f'{len(files)} audio files in the list', file=sys.stderr)

    if args.limit and args.limit < len(files):
        rnd = random.Random(args.seed)
        # stratified: proportional sample per extension, at least 1 per extension present
        by_ext = {}
        for p in files:
            by_ext.setdefault(os.path.splitext(p)[1].lower(), []).append(p)
        sample = []
        for ext, plist in by_ext.items():
            k = max(1, round(len(plist) / len(files) * args.limit))
            k = min(k, len(plist))
            sample.extend(rnd.sample(plist, k))
        rnd.shuffle(sample)
        files = sample[:args.limit]
    print(f'{len(files)} files selected for this run', file=sys.stderr)

    ag = AudioGenie()
    from pymediainfo import MediaInfo

    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    result_path = args.out + '.jsonl'
    timing = {'audiogenie': 0.0, 'mutagen': 0.0, 'pymediainfo': 0.0}
    counts = {'audiogenie': 0, 'mutagen': 0, 'pymediainfo': 0}
    errors = {'mutagen': 0, 'pymediainfo': 0}

    with open(result_path, 'w', encoding='utf-8') as out:
        for i, path in enumerate(files):
            if i % 2000 == 0:
                print(f'{i}/{len(files)}', file=sys.stderr)
            row = {'path': path}

            t0 = time.perf_counter()
            try:
                row['ag'] = ag.analyze(path)
            except Exception as e:
                row['ag'] = {'error': str(e)}
            timing['audiogenie'] += time.perf_counter() - t0
            counts['audiogenie'] += 1

            t0 = time.perf_counter()
            try:
                row['mu'] = mutagen_analyze(path)
            except Exception as e:
                row['mu'] = {'error': str(e)}
                errors['mutagen'] += 1
            timing['mutagen'] += time.perf_counter() - t0
            counts['mutagen'] += 1

            t0 = time.perf_counter()
            try:
                row['mi'] = pymediainfo_analyze(path, MediaInfo)
            except Exception as e:
                row['mi'] = {'error': str(e)}
                errors['pymediainfo'] += 1
            timing['pymediainfo'] += time.perf_counter() - t0
            counts['pymediainfo'] += 1

            if row['ag'].get('format') == 'MPEG' if isinstance(row.get('ag'), dict) else False:
                row['raw_tags'] = sorted(raw_tag_presence(path))

            out.write(json.dumps(row, ensure_ascii=False, default=str) + '\n')

    print('\n=== timing ===', file=sys.stderr)
    for k in timing:
        print(f'{k}: {timing[k]:.2f} s total, {timing[k]/max(1,counts[k])*1000:.3f} ms/file, {counts[k]} files, {errors.get(k,0)} errors', file=sys.stderr)
    print(f'wrote {result_path}', file=sys.stderr)


if __name__ == '__main__':
    main()
