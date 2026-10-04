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

// OGGHeader.cpp: implementation of class COGGHeader.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OGGHeader.h"
#include "Tools.h"
#include "Blob.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COGGHeader::COGGHeader()
{
  Reset();
}

COGGHeader::~COGGHeader()
{

}
bool COGGHeader::ReadFromFile(FILE *Stream)
{
  errno = 0;
  // the first 27 bytes of the page in one read (the identification "OggS", then 23 bytes)
  BYTE head[27];
  const size_t got = CTools::seqRead(Stream, head, sizeof(head));
  memcpy(ID, head, 4);                                /* Always "OggS" */
  if (got < 4 || memcmp(ID, OGG_PAGE_ID, 4) != 0)
  {
	  memset(ID, 0, 4);
	  return false;
  }
  if (got != sizeof(head))
	  return false;
  CBlob tmp;
  tmp.AddMemory(head + 4, 23);
  StreamVersion = tmp.GetAt(0);
  Byte = tmp.GetAt(1);                         /* Header type flag */
  AbsolutePosition = tmp.GetR4B(2);
  Abs1 = tmp.GetR4B(6);
  //   __int64 AbsolutePosition;               /* Absolute granule position */
  Serial = tmp.GetR4B(10);             /* Stream serial number */
  PageNumber = tmp.GetR4B(14);         /* Page sequence number */
  Checksum = tmp.GetR4B(18);           /* Page checksum */
  Segments = tmp.GetAt(22);                    /* Number of page segments */
  CTools::seqRead(Stream, LacingValues, Segments);    /* Lacing values - segment sizes */
  return (errno == 0);
}

bool COGGHeader::ReadFromMemory(const BYTE *data, size_t length)
{
  if (length < 27 || memcmp(data, OGG_PAGE_ID, 4) != 0 || length < (size_t)27 + data[26])
  {
	  memset(ID, 0, 4);
	  return false;
  }
  memcpy(ID, data, 4);
  CBlob tmp;
  tmp.AddMemory(data + 4, 23);
  StreamVersion = tmp.GetAt(0);
  Byte = tmp.GetAt(1);
  AbsolutePosition = tmp.GetR4B(2);
  Abs1 = tmp.GetR4B(6);
  Serial = tmp.GetR4B(10);
  PageNumber = tmp.GetR4B(14);
  Checksum = tmp.GetR4B(18);
  Segments = tmp.GetAt(22);
  memcpy(LacingValues, data + 27, Segments);
  return true;
}

bool COGGHeader::WriteToFile(FILE *Stream)
{
  errno = 0;
  CBlob tmp;
  tmp.AddMemory(ID, 4);						   /* Always "OggS" */
  tmp.AddValue(StreamVersion);                 /* Stream structure version */
  tmp.AddValue(Byte);                          /* Header type flag */
  tmp.AddR4B(AbsolutePosition);
  tmp.AddR4B(Abs1);
  //   __int64 AbsolutePosition;               /* Absolute granule position */
  tmp.AddR4B(Serial);                  /* Stream serial number */
  tmp.AddR4B(PageNumber);              /* Page sequence number */
  tmp.AddR4B(Checksum);                /* Page checksum */
  tmp.AddValue(Segments);                      /* Number of page segments */
  tmp.AddMemory(LacingValues, Segments);       /* Lacing values - segment sizes */
  tmp.FileWrite(tmp.GetLength(), Stream);
  return (errno == 0);
}

void COGGHeader::CopyToChar(BYTE buf[])
{
  memcpy(&buf[0], ID, 4);
  memcpy(&buf[4], &StreamVersion, 1);
  memcpy(&buf[5], &Byte, 1);
  memcpy(&buf[6], &AbsolutePosition, 4);
  memcpy(&buf[10], &Abs1, 4);
  memcpy(&buf[14], &Serial, 4);
  memcpy(&buf[18], &PageNumber, 4);
  memcpy(&buf[22], &Checksum, 4);
  memcpy(&buf[26], &Segments, 1);
  memcpy(&buf[27], LacingValues, Segments);
}

void COGGHeader::Reset()
{
  memset(ID, 0, 4);
  StreamVersion = 0;
  Byte = 0;
  AbsolutePosition = 0;
  Abs1 = 0;
  Serial = 0;
  PageNumber = 0;
  Checksum = 0;
  Segments = 0;
  memset(LacingValues, 0, sizeof(LacingValues));
}

