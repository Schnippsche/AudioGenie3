/* AudioGenie is a Library for analyzing and tagging audio files.
   Copyright (C) 2001-2026
   Stefan Toengi.
   This file is part of the AudioGenie Library.
   Contributed by Stefan Toengi.

   The AudioGenie Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The AudioGenie Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the AudioGenie Library; if not, see <http://www.gnu.org/licenses/>.
*/

// ApeTagItem.cpp: implementation of class CApeTagItem.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ApeTagItem.h"
#include "Tools.h"
// Format:  http://web.archive.org/web/20041026140532/www.personal.uni-jena.de/~pfk/mpp/sv8/apetagitem.html

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CApeTagItem::CApeTagItem()
{

}

CApeTagItem::~CApeTagItem()
{

}

bool CApeTagItem::ReadFromFile(CFile *Stream, bool readValue)
{
  errno = 0;
  CBlob tmp;
  Flags = 0;
  if (!tmp.FileRead(8, Stream) || tmp.GetLength() != 8)
	  return false;
  // the size of the file that is read (CTools::FileSize is the size of the analyzed file; a tag is also checked in another file before it is removed)
  const __int64 fileLength = CTools::fileLength(Stream);
  Size = (long)tmp.GetR4B(0);   // size of the item value in bytes
  if (Size < 0 || (__int64)CTools::seqTell(Stream) + Size > fileLength)
	  return false;
  Flags = (long)tmp.GetR4B(4);  // item flags
  Key.Empty();
  // the key ends with a zero byte; keys are short, anything else is corrupt. It is read as a block (not byte by byte, up to the end of the file).
  const __int64 keyStart = CTools::seqTell(Stream);
  const __int64 left = fileLength - keyStart;
  BYTE keyBlock[257];
  const size_t got = (left > 0) ? CTools::seqRead(Stream, keyBlock, left < (__int64)sizeof(keyBlock) ? (size_t)left : sizeof(keyBlock)) : 0;
  size_t keyLength = 0;
  while (keyLength < got && keyBlock[keyLength] != 0)
    keyLength++;
  if (keyLength == got || keyLength > 255)   // the end of the file or no terminator within 256 characters
    return false;
  // a key is ASCII; a byte outside of it (not allowed) is read with the code page the key is written with, so it is written back unchanged
  CBlob keyBytes;
  keyBytes.AddMemory(keyBlock, keyLength);
  Key = keyBytes.GetAnsiStringAt(0, keyLength);
  const __int64 valueStart = keyStart + (__int64)keyLength + 1;
  if (!readValue)
  {
    // only the layout of the items is checked: the value is skipped
    CTools::seqSeek(Stream, valueStart + Size);
    return true;
  }
  CTools::seqSeek(Stream, valueStart);
  // the whole value (no memory or a file that ends too early: no item, instead of one with an empty value that a save would write)
  if (!Value.FileRead(Size, Stream) || (long)Value.GetLength() != Size)
    return false;
  return (errno == 0);
}

void CApeTagItem::Reset()
{
  Size = 0;
  Flags = 0;
  Key.Empty();
  Value.Clear();
}

bool CApeTagItem::isBinary()
{
  // bits 2..1 of the flags: 0 text, 1 binary, 2 locator (UTF-8 text as well), 3 reserved
  const int type = (Flags >> 1) & 3;
  return (type == 1 || type == 3);
}