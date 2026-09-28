"""Analyzes the result of compare_libs.py: per-library format/valid/error counts, how often duration, bit rate, sample
rate, channels, title and artist agree between AudioGenie3 and each of mutagen/pymediainfo, and which tag containers
each library reports for MP3 versus what is actually in the file (checked independently by reading the raw bytes).

Usage: python tests/tools/analyze_complib.py <result.jsonl>
"""
import json
import re
import sys
from collections import Counter, defaultdict


def norm_text(s):
    if not s:
        return ''
    s = str(s).strip()
    s = re.sub(r'\s+', ' ', s)
    return s


def close(a, b, tol):
    try:
        return abs(float(a) - float(b)) <= tol
    except (TypeError, ValueError):
        return False


def main():
    path = sys.argv[1]
    rows = [json.loads(l) for l in open(path, encoding='utf-8')]
    print(f'{len(rows)} files\n')

    print('=== format detected (AudioGenie3) ===')
    fmt_counts = Counter(r['ag'].get('format', '?') for r in rows if isinstance(r.get('ag'), dict))
    for k, v in fmt_counts.most_common():
        print(f'  {k:12s} {v}')

    print('\n=== "unsupported" / parse errors, by library ===')
    for lib in ('ag', 'mu', 'mi'):
        unsupported = sum(1 for r in rows if isinstance(r.get(lib), dict) and r[lib].get('format') == 'unsupported')
        errored = sum(1 for r in rows if isinstance(r.get(lib), dict) and 'error' in r[lib])
        invalid = sum(1 for r in rows if isinstance(r.get(lib), dict) and r[lib].get('valid') is False and r[lib].get('format') not in ('unsupported',))
        print(f'  {lib:3s} unsupported={unsupported:5d} errors={errored:5d} recognized-but-invalid={invalid:5d}')

    print('\n  unsupported by extension:')
    for lib in ('mu', 'mi'):
        by_ext = Counter()
        for r in rows:
            d = r.get(lib)
            if isinstance(d, dict) and (d.get('format') == 'unsupported' or 'error' in d):
                ext = r['path'].rsplit('.', 1)[-1].lower()
                by_ext[ext] += 1
        print(f'    {lib}: {dict(by_ext.most_common())}')

    # only compare rows where all three actually produced technical data
    def ok(r, lib):
        d = r.get(lib)
        return isinstance(d, dict) and d.get('format') not in ('unsupported', None) and 'error' not in d and d.get('duration', 0)

    comparable = [r for r in rows if ok(r, 'ag') and ok(r, 'mu') and ok(r, 'mi')]
    print(f'\n=== files where AudioGenie, mutagen and pymediainfo all returned data: {len(comparable)} of {len(rows)} ===')

    print('\n=== agreement with AudioGenie3 (tolerances: duration 0.5 s, bit rate 5 kbit/s or 5 %, sample rate exact, channels exact) ===')
    for other in ('mu', 'mi'):
        name = 'mutagen' if other == 'mu' else 'pymediainfo'
        n = len(comparable)
        dur_ok = sum(1 for r in comparable if close(r['ag']['duration'], r[other]['duration'], 0.5))
        br_ok = sum(1 for r in comparable if close(r['ag']['bitrate'], r[other]['bitrate'], max(5, 0.05 * r['ag']['bitrate'])))
        sr_ok = sum(1 for r in comparable if r['ag']['samplerate'] == r[other]['samplerate'])
        ch_ok = sum(1 for r in comparable if r['ag']['channels'] == r[other]['channels'])
        ti_ok = sum(1 for r in comparable if norm_text(r['ag']['title']) == norm_text(r[other]['title']))
        ar_ok = sum(1 for r in comparable if norm_text(r['ag']['artist']) == norm_text(r[other]['artist']))
        print(f'  {name:12s} duration {dur_ok}/{n} ({100*dur_ok/n:.1f}%)  bitrate {br_ok}/{n} ({100*br_ok/n:.1f}%)  '
              f'samplerate {sr_ok}/{n} ({100*sr_ok/n:.1f}%)  channels {ch_ok}/{n} ({100*ch_ok/n:.1f}%)  '
              f'title {ti_ok}/{n} ({100*ti_ok/n:.1f}%)  artist {ar_ok}/{n} ({100*ar_ok/n:.1f}%)')

    # detailed discrepancy dump for manual inspection, grouped by field
    def dump_discrepancies(field, tol_fn, n=8):
        for other in ('mu', 'mi'):
            name = 'mutagen' if other == 'mu' else 'pymediainfo'
            print(f'\n  --- {field}: AudioGenie3 vs {name} (first {n}) ---')
            shown = 0
            for r in comparable:
                a, b = r['ag'].get(field), r[other].get(field)
                if not tol_fn(a, b):
                    print(f'    {r["path"][-70:]}  ag={a!r}  {name}={b!r}')
                    shown += 1
                    if shown >= n:
                        break

    print('\n=== example discrepancies ===')
    dump_discrepancies('duration', lambda a, b: close(a, b, 0.5))
    dump_discrepancies('bitrate', lambda a, b: close(a, b, max(5, 0.05 * (a or 1))))
    dump_discrepancies('samplerate', lambda a, b: a == b)
    dump_discrepancies('channels', lambda a, b: a == b)
    dump_discrepancies('title', lambda a, b: norm_text(a) == norm_text(b))
    dump_discrepancies('artist', lambda a, b: norm_text(a) == norm_text(b))

    # MP3 tag container presence: AudioGenie's own detection vs the raw byte ground truth
    print('\n=== MP3 tag containers: AudioGenie3 report vs. raw bytes (ID3v2 signature / ID3v1 "TAG" footer / "APETAGEX") ===')
    mp3_rows = [r for r in rows if isinstance(r.get('ag'), dict) and r['ag'].get('format') == 'MPEG' and 'raw_tags' in r]
    print(f'  {len(mp3_rows)} MP3 files checked')
    mismatch = 0
    combo_counts = Counter()
    for r in mp3_rows:
        ag_tags = set(r['ag'].get('tags', []))
        raw_tags = set(r.get('raw_tags', []))
        combo_counts[tuple(sorted(raw_tags))] += 1
        if ag_tags != raw_tags:
            mismatch += 1
            if mismatch <= 10:
                print(f'    {r["path"][-70:]}  AudioGenie reports={sorted(ag_tags)}  raw bytes have={sorted(raw_tags)}')
    print(f'  mismatches: {mismatch} / {len(mp3_rows)}')
    print('  combinations of tag containers actually present (raw bytes):')
    for combo, n in combo_counts.most_common():
        print(f'    {combo or "(none)"}: {n}')

    print('\n=== what mutagen reports as the tag container class, for MP3 files with more than one raw tag container ===')
    multi = [r for r in mp3_rows if len(r.get('raw_tags', [])) > 1]
    print(f'  {len(multi)} MP3 files with 2 or more tag containers physically present')
    mu_class_counts = Counter()
    for r in multi:
        mu = r.get('mu', {})
        mu_class_counts[tuple(mu.get('tags', []))] += 1
    for combo, n in mu_class_counts.most_common():
        print(f'    mutagen sees {combo or "(none)"}: {n}')


if __name__ == '__main__':
    main()
