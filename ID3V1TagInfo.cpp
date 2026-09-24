/* AudioGenie is a Library for analyzing and tagging audio files.
   Copyright (C) 2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011
   Free Software Foundation, Inc.
   This file is part of the AudioGenie Library.
   Contributed by Stefan Toengi.

   The AudioGenie Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see <http://www.gnu.org/licenses/> 
*/

// ID3V1TagInfo.cpp: implementation of class CID3V1TagInfo.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ID3V1TagInfo.h"
#include "Tools.h"
#include "io.h"

/*
Blob
0      3     possibly APE 
3 	  3 	identifier TAG marking an ID3v1 block
6 	 30 	Songtitel
36 	 30 	Artist/Performer
66 	 30 	Album
96 	  4 	Erscheinungsjahr
100 	 30 	Beliebiger Kommentar
130   1 	Genre


*/



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CID3V1TagInfo::CID3V1TagInfo()
{
  tmp = new CBlob(132);
  Reset();
}

CID3V1TagInfo::~CID3V1TagInfo()
{
	delete tmp;
}

// Reads a fixed-length text field: the padding usually consists of NUL bytes (blanks occur as well),
// so cut off at the first NUL and then remove trailing blanks.
static CAtlString ReadFixedField(CBlob *blob, size_t pos, size_t len)
{
	CAtlString raw = blob->GetStringAt(pos, len);
	CAtlString text((LPCTSTR)raw);   // stops at the first NUL
	return text.TrimRight();
}

bool CID3V1TagInfo::ReadFromFile(FILE *Stream)
{
  errno = 0;
  _exists = false;
  // first check that TAG does not point to APETAGEX (BUG 27.08.06)
  _fseeki64(Stream, -131, SEEK_END);
  tmp->FileRead(131, Stream);
  if ( (tmp->GetLength() != 131)
	  || (memcmp(tmp->m_pData, _T("APETAGEX"), 8) == 0)
	  || (tmp->GetAt(3) != 'T' || tmp->GetAt(4) != 'A' || tmp->GetAt(5) != 'G') )
	  return false;
  
  /*  old version
  CAtlString st = tmp->GetStringAt(6, 124);
  Title = st.Mid(0, 30).TrimRight();
  Artist = st.Mid(30, 30).TrimRight();
  Album = st.Mid(60, 30).TrimRight();
  Year = st.Mid(90, 4).TrimRight(); 
  Comment = st.Mid(94, 30).TrimRight(); 
  */
  Title = ReadFixedField(tmp, 6, 30);
  Artist = ReadFixedField(tmp, 36, 30);
  Album = ReadFixedField(tmp, 66, 30);
  Year = ReadFixedField(tmp, 96, 4);
  Comment = ReadFixedField(tmp, 100, 30);
  if ((tmp->GetAt(128) == 0 && tmp->GetAt(129) != 0) ||
      (tmp->GetAt(128) == 32 && tmp->GetAt(129) != 32))
    Track = tmp->GetAt(129);
  else
    Track = 0;

  Genre = tmp->GetAt(130);
  _exists = true;
  return true;
}

bool CID3V1TagInfo::WriteToFile(FILE *Stream)
{
  errno = 0;
  tmp->Clear();
  tmp->AddMemory(ID3V1_ID, 3);
  tmp->AddFixedAnsiString(Title ,30);
  tmp->AddFixedAnsiString(Artist, 30);
  tmp->AddFixedAnsiString(Album, 30);
  tmp->AddFixedAnsiString(Year, 4);
  if (Track > 0)
  {
    tmp->AddFixedAnsiString(Comment, 28);
    tmp->AddNullByte();
    tmp->AddValue(Track);    
  }
  else
    tmp->AddFixedAnsiString(Comment, 30);
  tmp->AddValue(Genre);
  tmp->FileWrite(ID3V1_TAG_SIZE, Stream);
  return (errno == 0);
}

void CID3V1TagInfo::Reset()
{
  _exists = false;
  Album.Empty();
  Artist.Empty();
  Comment.Empty();
  Title.Empty();
  Year.Empty();
  Track = 0;
  Genre = DEFAULT_GENRE;
}

bool CID3V1TagInfo::exists()
{
  return _exists;
}

CAtlString CID3V1TagInfo::GetTagVersion()
{
  if (!_exists)
    return EMPTY;
  if (Track > 0)
    return CAtlString(TAG_VERSION_1_1);
  else
    return CAtlString(TAG_VERSION_1_0);
}

CAtlString CID3V1TagInfo::GetTrack()
{
  if (Track == 0 || Track > 255)
    return EMPTY;
  CAtlString result;
  result.Format(_T("%i"), Track);
  return result;
}

void CID3V1TagInfo::SetTrack(LPCWSTR newValue)
{
  Track = (BYTE)_wtoi(newValue);
  if (Track < 0 || Track > 255) Track = 0;
}

void CID3V1TagInfo::SetGenre(LPCWSTR newValue)
{
  CAtlString vgl = newValue;
  Genre = DEFAULT_GENRE;
  for (int i=0; i < MAX_MUSIC_GENRES; i++)
    if (vgl.CompareNoCase(MUSIC_GENRE[i]) == 0)
      Genre = (BYTE)i;
}
