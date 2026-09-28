"""Summary of a scan_all result (tests/out/scan_all/<arch>/result.tsv): formats by extension, files that are recognized although they are no audio
files, audio files that are not recognized, implausible values, errors and the slowest files.

python tests/tools/analyze_scan_all.py <result.tsv> [problems.txt]
"""
import csv, collections, os, sys

FORMATS = {0: 'unknown', 1: 'MPEG', 2: 'WMA', 3: 'MONKEY', 4: 'FLAC', 5: 'WAV', 6: 'OGGVORBIS', 7: 'MPEGPLUS', 8: 'AAC', 9: 'MP4M4A', 10: 'TTA',
           11: 'WAVPACK', 12: 'ID3V2', 13: 'OGGOPUS'}
# extensions that a file of the format normally has
EXPECTED = {
    'MPEG': {'.mp3', '.mp2', '.mp1', '.mpa', '.msf'}, 'WMA': {'.wma', '.wmv', '.asf'}, 'MONKEY': {'.ape', '.mac'}, 'FLAC': {'.flac', '.fla'},
    'WAV': {'.wav', '.wave', '.bwf', '.mp3'}, 'OGGVORBIS': {'.ogg', '.oga', '.ogv'}, 'MPEGPLUS': {'.mpc', '.mp+', '.mpp'}, 'AAC': {'.aac'},
    'MP4M4A': {'.m4a', '.mp4', '.m4b', '.m4v', '.mov', '.aac', '.3gp'}, 'TTA': {'.tta'}, 'WAVPACK': {'.wv'}, 'OGGOPUS': {'.opus', '.ogg', '.oga'},
}
AUDIO_EXT = {'.mp3', '.mp2', '.mp1', '.wma', '.m4a', '.ogg', '.opus', '.flac', '.wav', '.ape', '.mpc', '.tta', '.wv', '.aac', '.oga', '.m4b'}


def main():
    path = sys.argv[1]
    rows = []
    with open(path, encoding='utf-8', errors='replace', newline='') as f:
        rd = csv.reader(f, delimiter='\t', quoting=csv.QUOTE_NONE)
        head = next(rd)
        for r in rd:
            if len(r) == len(head):
                rows.append(dict(zip(head, r)))
    print('files analyzed:', len(rows))
    total_ms = sum(float(r['ms']) for r in rows)
    total_bytes = sum(int(r['size']) for r in rows)
    print('time of AUDIOAnalyzeFileW: %.1f s (%.3f ms per file), %.1f GB in the files' % (total_ms / 1000, total_ms / max(1, len(rows)), total_bytes / 1e9))
    by_format = collections.Counter(FORMATS.get(int(r['format']), r['format']) for r in rows)
    print('\nformats:')
    for k, v in by_format.most_common():
        print('  %-10s %d' % (k, v))

    print('\nrecognized formats by extension (top 15 extensions per format):')
    table = collections.defaultdict(collections.Counter)
    for r in rows:
        fmt = FORMATS.get(int(r['format']), r['format'])
        if fmt != 'unknown':
            table[fmt][os.path.splitext(r['path'])[1].lower()] += 1
    for fmt, c in sorted(table.items()):
        print('  %-10s %s' % (fmt, ', '.join('%s:%d' % (e or '(none)', n) for e, n in c.most_common(15))))

    print('\nrecognized as audio, but the extension is not one of the format (false positives?):')
    shown = collections.Counter()
    for r in rows:
        fmt = FORMATS.get(int(r['format']), r['format'])
        if fmt == 'unknown':
            continue
        ext = os.path.splitext(r['path'])[1].lower()
        if ext not in EXPECTED.get(fmt, set()):
            shown[fmt] += 1
            if shown[fmt] <= 12:
                print('  %-10s %s  (%s bytes, dur %s, valid %s)' % (fmt, r['path'], r['size'], r['duration'], r['valid']))
    for fmt, n in shown.items():
        print('  ... %s: %d files in total' % (fmt, n))

    print('\nfiles with an audio extension that are not recognized:')
    n = 0
    by_ext = collections.Counter()
    for r in rows:
        ext = os.path.splitext(r['path'])[1].lower()
        if ext in AUDIO_EXT and int(r['format']) == 0:
            by_ext[ext] += 1
            n += 1
            if n <= 25:
                print('  %s  (%s bytes, error %s)' % (r['path'], r['size'], r['lasterror']))
    print('  total %d: %s' % (n, dict(by_ext)))

    print('\nrecognized, but not valid (AUDIOIsValidFormat = 0):')
    n = 0
    for r in rows:
        if int(r['format']) != 0 and r['valid'] == '0':
            n += 1
            if n <= 15:
                print('  %-10s %s (%s bytes)' % (FORMATS.get(int(r['format'])), r['path'], r['size']))
    print('  total', n)

    print('\nimplausible values of recognized files:')
    counts = collections.Counter()
    for r in rows:
        fmt = int(r['format'])
        if fmt == 0:
            continue
        dur = float(r['duration']); br = int(r['bitrate']); sr = int(r['samplerate']); ch = int(r['channels'])
        msgs = []
        if dur < 0 or dur != dur:
            msgs.append('negative duration')
        if dur > 360000:
            msgs.append('duration over 100 hours')
        if br < 0 or br > 30000:
            msgs.append('bit rate %d' % br)
        if sr < 0 or sr > 800000:
            msgs.append('sample rate %d' % sr)
        if ch < 0 or ch > 32:
            msgs.append('channels %d' % ch)
        if r['valid'] == '-1' and (dur == 0 or sr == 0):
            msgs.append('valid without duration or sample rate')
        for m in msgs:
            counts[m] += 1
            if counts[m] <= 6:
                print('  %s: %s %s (dur %s, br %s, sr %s, ch %s)' % (m, FORMATS.get(fmt), r['path'], r['duration'], r['bitrate'], r['samplerate'], r['channels']))
    print('  counts:', dict(counts))

    print('\nlast error numbers:')
    errs = collections.Counter(r['lasterror'] for r in rows)
    for k, v in errs.most_common(12):
        print('  %s: %d' % (k, v))

    print('\nslowest files:')
    for r in sorted(rows, key=lambda r: -float(r['ms']))[:12]:
        print('  %8.1f ms  %-10s %s (%s bytes)' % (float(r['ms']), FORMATS.get(int(r['format'])), r['path'], r['size']))
    if len(sys.argv) > 2 and os.path.exists(sys.argv[2]):
        print('\nproblems (crashes and hangs):')
        print(open(sys.argv[2], encoding='utf-8', errors='replace').read())


main()
