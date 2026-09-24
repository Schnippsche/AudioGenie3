#!/usr/bin/env python3
"""Erzeugt tests/misuse_calls.inc aus dem C++-Wrapper-Header: fuer jede exportierte Funktion (ausser Speichern/Entfernen/Analyse)
ein Block, der sie mit zufaelligen, teils ungueltigen Argumenten aufruft (NULL, leer, riesig, Index 0/-1/32767, Frame-IDs
ausserhalb ...). Aufruf: python tests/gen_misuse.py
"""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / "Wrapper" / "C C++" / "audiogenie3.h"
OUT = Path(__file__).resolve().parent / "misuse_calls.inc"

SKIP = re.compile(r"(Save|Remove|Analyze|ImportCue)")     # veraendern Dateien oder sind eigener Test
FUNC = re.compile(r"DllImport\s+(.+?)\s+__stdcall\s+(\w+)\s*\(([^)]*)\)\s*;")


def params(text):
    text = text.strip()
    return [] if text in ("", "void") else [p.strip() for p in text.split(",")]


def main():
    lines = ["// GENERIERT von tests/gen_misuse.py - nicht von Hand aendern", ""]
    count = 0
    for m in FUNC.finditer(HEADER.read_text(encoding="latin-1")):
        ret, name, ps = m.group(1).strip(), m.group(2), params(m.group(3))
        if SKIP.search(name):
            continue
        args, decls = [], []
        i = 0
        while i < len(ps):
            p = ps[i]
            typ = p.split()[0] if "*" not in p else "BYTE*"
            pname = p.split()[-1].lower()
            v = f"a{i}"
            if typ == "BYTE*":
                # Puffer + Laenge: Laenge nie groesser als der Puffer (das waere ein Aufruferfehler)
                decls.append(f"auto {v} = pickBuffer(r, buf);")
                args.append(f"{v}.ptr")
                if i + 1 < len(ps) and ps[i + 1].split()[0] in ("u32", "uint"):
                    args.append(f"{v}.len")
                    i += 1
            elif typ == "LPCWSTR":
                decls.append(f"const wchar_t* {v} = pickString(r, {'true' if 'file' in pname else 'false'});")
                args.append(v)
            elif typ == "short":
                decls.append(f"short {v} = pickShort(r);")
                args.append(v)
            elif typ in ("u32", "uint"):
                decls.append(f"u32 {v} = pickId(r);")
                args.append(v)
            elif typ in ("long", "int"):
                decls.append(f"long {v} = pickLong(r);")
                args.append(v)
            elif typ == "BYTE":
                decls.append(f"BYTE {v} = static_cast<BYTE>(r.next());")
                args.append(v)
            else:
                raise SystemExit(f"unbekannter Typ {typ} in {name}")
            i += 1
        call = f"{name}({', '.join(args)})"
        if ret == "BSTR":
            call = f"take({call})"
        lines.append(f'if (!skipped("{name}") && (name.empty() || name == "{name}")) {{ for (int n = 0; n < rounds; n++) {{ {" ".join(decls)} traceCall("{name}", n); (void){call}; }} ++called; }}')
        count += 1
    OUT.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"{count} Funktionen -> {OUT.name}")


if __name__ == "__main__":
    main()
