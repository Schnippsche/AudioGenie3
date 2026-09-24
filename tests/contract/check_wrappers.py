#!/usr/bin/env python3
"""Contract check: signatures of the wrappers (C/C++, C#, VB.NET, Delphi, VB6) against the
extern "C" functions in dllmain.cpp.  Exit code 1 on deviations.

Aufruf:  python tests/contract/check_wrappers.py
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / "Wrapper"


def read(p):
    return Path(p).read_text(encoding="latin-1")


def split_params(s):
    s = s.strip()
    return [] if s in ("", "void") else [x.strip() for x in s.split(",")]


# Normalized type classes: i16, i32, f32, u8, ptr (pointer/buffer), wstr (LPCWSTR), bstr, void
DLL_TYPES = {"short": "i16", "long": "i32", "u32": "i32", "LPCWSTR": "wstr", "BSTR": "bstr",
             "BYTE *": "ptr", "BYTE": "u8", "float": "f32", "void": "void"}
ENUMS = {"ID3V2FRAMES": "i32", "MP4FRAMES": "i32", "WAVFRAMES": "i32", "PictureType": "i16"}


def parse_dll():
    out = {}
    for m in re.finditer(r'extern "C"\s+(.+?)\s+__stdcall\s+(\w+)\s*\(([^)]*)\)', read(ROOT / "dllmain.cpp")):
        ret, name, ps = m.groups()
        params = []
        for p in split_params(ps):
            p = re.sub(r"\s*\b\w+\s*$", "", p) if re.search(r"[\w*]\s+\**\w+$", p) else p
            params.append(DLL_TYPES[p.strip()])
        out[name] = (DLL_TYPES[ret.strip()], params)
    return out


def parse_cpp():
    m2 = {"short": "i16", "long": "i32", "u32": "i32", "uint": "i32", "int": "i32", "LPCWSTR": "wstr",
          "BSTR": "bstr", "BYTE": "u8", "float": "f32", "void": "void"}
    out = {}
    for m in re.finditer(r"DllImport\s+(.+?)\s+__stdcall\s+(\w+)\s*\(([^)]*)\)", read(W / "C C++/audiogenie3.h")):
        r, n, ps = m.groups()
        toks = []
        for p in split_params(ps):
            toks.append("ptr" if "*" in p else m2.get(p.split()[0], "?" + p.split()[0]))
        out[n] = (m2.get(r.strip(), "?" + r), toks)
    return out


def parse_cs():
    m2 = {"short": "i16", "int": "i32", "uint": "i32", "long": "i64!", "string": "wstr", "String": "wstr", "byte[]": "ptr",
          "IntPtr": "ptr", "byte": "u8", "float": "f32", "void": "void"}
    m2.update(ENUMS)
    out = {}
    pat = r"((?:\[[^\]\n]*\]\s*)*)\s*(?:private|public|internal)\s+static\s+extern\s+([\w\[\]]+)\s+(\w+)\s*\(([^)]*)\)"
    for m in re.finditer(pat, read(W / "C #/AudioGenie2.cs")):
        attrs, r, n, ps = m.groups()
        toks = []
        for p in split_params(ps):
            p = re.sub(r"\b(ref|out|in)\s+", "", p).strip()
            toks.append(m2.get(p.split()[0], "?" + p.split()[0]))
        rt = m2.get(r, "?" + r)
        if rt == "wstr":
            rt = "bstr" if "UnmanagedType.BStr" in attrs else "wstr(without BStr marshalling)"
        out[n] = (rt, toks)
    return out


def parse_vb():
    m2 = {"Short": "i16", "Integer": "i32", "UInteger": "i32", "Long": "i64!", "String": "wstr",
          "Byte()": "ptr", "IntPtr": "ptr", "Byte": "u8", "Single": "f32"}
    m2.update(ENUMS)
    out = {}
    pat = (r"(<DllImport\([^\n]*\)>\s*_\r?\n\s*)Private\s+(Function|Sub)\s+(\w+)\s*"
           r"\(((?:[^()]|\(\))*)\)(?:\s+As\s+(<[^>]*>\s*)?([\w\[\]]+))?")
    for m in re.finditer(pat, read(W / "DotNET/audiogenie3.vb")):
        _, kind, n, ps, ret_attr, r = m.groups()
        toks = []
        for p in split_params(re.sub(r"_\r?\n\s*", "", ps)):
            t = re.search(r"(?<!\w)As\s+([\w\[\]()]+)", p.split(" As ", 1)[0] + " As " + p.split(" As ", 1)[1])
            t = t.group(1).replace("[", "").replace("]", "")
            toks.append(m2.get(t, "?" + t))
        r = (r or "").replace("[", "").replace("]", "")
        rt = "void" if kind == "Sub" else m2.get(r, "?" + r)
        if rt == "wstr":
            rt = "bstr" if ret_attr and "BStr" in ret_attr else "wstr(without BStr marshalling)"
        out[n] = (rt, toks)
    return out


def parse_delphi():
    m2 = {"smallint": "i16", "longint": "i32", "integer": "i32", "cardinal": "i32", "longword": "i32",
          "pwidechar": "wstr", "widestring": "wstr", "pointer": "ptr", "pbyte": "ptr", "byte": "u8",
          "single": "f32"}
    out = {}
    pat = r"(function|procedure)\s+AG3(\w+)\s*(?:\(([^)]*)\))?\s*(?::\s*(\w+))?\s*;\s*stdcall;\s*external\s+\w+\s+name\s+'(\w+)'"
    for m in re.finditer(pat, read(W / "Delphi/AudioGenie3.pas")):
        kind, _, ps, r, n = m.groups()
        toks = []
        for grp in (ps or "").split(";"):
            if grp.strip():
                names, ty = grp.split(":")
                toks += [m2.get(ty.strip().lower(), "?" + ty.strip())] * len([x for x in names.replace("const", "").split(",") if x.strip()])
        rt = "void" if kind == "procedure" else ("bstr" if (r or "").lower() == "widestring" else m2.get((r or "").lower(), "?" + str(r)))
        out[n] = (rt, toks)
    return out


def parse_vb6():
    m2 = {"Integer": "i16", "Long": "i32", "String": "bstr", "Byte": "u8", "Single": "f32", "Any": "ptr"}
    out = {}
    pat = r'Private Declare (Function|Sub) (\w+) Lib "[^"]+"(?: Alias "\w+")? \(([^)]*)\)(?: As (\w+))?'
    for m in re.finditer(pat, read(W / "VB6/clsAudioGenie.cls")):
        kind, n, ps, r = m.groups()
        toks = []
        for p in split_params(ps):
            toks.append(m2.get(re.search(r"(?<!\w)As (\w+)", p).group(1), "?"))
        out[n] = ("void" if kind == "Sub" else m2.get(r, "?" + str(r)), toks)
    return out


# Known, harmless deviations (identical stack/register usage, value fits into 16 bits / is ignored).
HARMLOS_RET = {"AUDIOAnalyzeFileW", "MP4DeletePictureW"}


def compatible(wrapper_lang, w, d, is_ret):
    if w == d:
        return True
    if wrapper_lang == "vb6" and not is_ret and w == "i32" and d in ("wstr", "ptr"):
        return True                      # ByVal Long = StrPtr / Zeiger
    if wrapper_lang == "vb6" and is_ret and w == "bstr" and d == "bstr":
        return True
    if wrapper_lang == "delphi" and is_ret and w == "wstr" and d == "bstr":
        return True                      # PWideChar return value; ConvertString frees the BSTR
    if not is_ret and {w, d} <= {"wstr", "bstr"}:
        return True                      # WideString/BSTR als Parameter = LPCWSTR
    return False


def main():
    dll = parse_dll()
    parsers = {"cpp": parse_cpp, "cs": parse_cs, "vbnet": parse_vb, "delphi": parse_delphi, "vb6": parse_vb6}
    problems = 0
    print(f"{len(dll)} exports in dllmain.cpp")
    for lang, fn in parsers.items():
        decl = fn()
        issues = []
        for n in sorted(dll):
            if n not in decl:
                issues.append(f"{n}: missing")
                continue
            (dr, dp), (wr, wp) = dll[n], decl[n]
            if n not in HARMLOS_RET and not compatible(lang, wr, dr, True):
                issues.append(f"{n}: return type {wr} instead of {dr}")
            if len(wp) != len(dp):
                issues.append(f"{n}: {len(wp)} instead of {len(dp)} parameters")
                continue
            for i, (a, b) in enumerate(zip(wp, dp)):
                if not compatible(lang, a, b, False):
                    issues.append(f"{n}: parameter {i + 1} {a} instead of {b}")
        extra = sorted(set(decl) - set(dll))
        issues += [f"{n}: not in the DLL" for n in extra]
        print(f"  {lang:7s} {len(decl):4d} declarations, {len(issues)} deviations")
        for i in issues:
            print("     -", i)
        problems += len(issues)
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
