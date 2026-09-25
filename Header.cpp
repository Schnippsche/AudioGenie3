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

// Header.cpp: implementation of class CHeader.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Header.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeader::CHeader()
{

}

CHeader::~CHeader()
{

}
// check the file header for known data types
BYTE CHeader::ReadFromFile(FILE *Stream)
{
  // _fseeki64(Stream, 0, SEEK_SET);
  fread(Buf, 1, 8, Stream); 
  for (int i = 0; i < MAX_FILETYPES; i++)
  {
    if ( memcmp(Buf, filetypes[i].kennung, filetypes[i].laenge) == 0)
      return filetypes[i].result;
  }
  // nothing found yet, check bytes 5 - 8
  if (Buf[4] == 0x66 && Buf[5] == 0x74 && Buf[6] == 0x79 && Buf[7] ==  0x70)  // mp4    "ftyp"
	  return AUDIO_FORMAT_MP4;
  return AUDIO_FORMAT_UNKNOWN;  
}