#!/usr/bin/env python3
"""Contract check: the line endings of the working tree match the rules of .gitattributes.

The repository stores text files with LF; files with `eol=crlf` (sources, batch files, project files) must
have CRLF in the working tree, files with `eol=lf` must have LF. Files with mixed line endings are always an
error. LF-only sources cause the git warning "LF will be replaced by CRLF" and complaints of Visual Studio.

Usage:  python tests/contract/check_line_endings.py          check, exit code 1 on deviations
        python tests/contract/check_line_endings.py --fix    convert the files in the working tree
"""
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def tracked_files():
    """(path, working tree eol, attributes) of all tracked files, as git analyzes them."""
    out = subprocess.run(["git", "ls-files", "--eol"], cwd=ROOT, capture_output=True, text=True, check=True).stdout
    for line in out.splitlines():
        meta, _, path = line.partition("\t")
        fields = meta.split(None, 2)          # i/lf  w/crlf  attr/text eol=crlf
        yield path, fields[1][2:], (fields[2] if len(fields) > 2 else "")


def expected(attr):
    if "eol=crlf" in attr:
        return "crlf"
    if "eol=lf" in attr:
        return "lf"
    return None


def convert(path, eol):
    p = ROOT / path
    data = p.read_bytes()
    lf = data.replace(b"\r\n", b"\n")
    new = lf.replace(b"\n", b"\r\n") if eol == "crlf" else lf
    if new != data:
        p.write_bytes(new)


def main():
    fix = "--fix" in sys.argv[1:]
    try:
        files = list(tracked_files())
    except (OSError, subprocess.CalledProcessError) as e:
        print(f"line ending check skipped (git not available): {e}")
        return 0
    bad = []
    for path, actual, attr in files:
        if actual in ("-text", "none", "binary"):     # binary or empty
            continue
        want = expected(attr)
        if actual == "mixed" or (want and actual != want):
            bad.append((path, actual, want or "one kind"))
    if not bad:
        print(f"line endings ok ({len(files)} tracked files)")
        return 0
    for path, actual, want in bad:
        print(f"{path}: {actual}, expected {want}")
        if fix:
            convert(path, want if want in ("crlf", "lf") else "lf")
    if fix:
        print(f"{len(bad)} file(s) converted")
        return 0
    print(f"{len(bad)} file(s) with wrong line endings; fix with: python tests/contract/check_line_endings.py --fix")
    return 1


if __name__ == "__main__":
    sys.exit(main())
