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

// Fixed field of the id3v1 tag; a field that is full continues in the enhanced tag (ext, may be NULL)
static CAtlString ReadFieldWithExtension(CBlob *blob, size_t pos, size_t len, CBlob *ext, size_t extPos, size_t extLen)
{
  CAtlString raw = blob->GetStringAt(pos, len);
  bool full = ((size_t)raw.GetLength() >= len);
  for (int i = 0; full && i < raw.GetLength(); i++)
    if (raw[i] == 0)
      full = false;
  CAtlString text((LPCTSTR)raw);
  if (full && ext != NULL)
    text += CAtlString((LPCTSTR)ext->GetStringAt(extPos, extLen));
  return text.TrimRight();
}

bool CID3V1TagInfo::ReadFromFile(FILE *Stream)
{
  errno = 0;
  _exists = false;
  // one read of the end of the file: the id3v1 tag (128 bytes) and the enhanced tag (227 bytes) in front of it
  CBlob tail;
  CBlob enhanced;
  _enhanced = false;
  memset(_enhancedRest, 0, sizeof(_enhancedRest));
  size_t want = ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE;
  if (_fseeki64(Stream, -(__int64)want, SEEK_END) != 0)
  {
    want = 131;   // a smaller file: the id3v1 tag and the 3 bytes in front of it
    _fseeki64(Stream, -(__int64)want, SEEK_END);
  }
  tail.FileRead(want, Stream);
  const size_t length = tail.GetLength();
  if (length < 131)
    return false;
  // the last 131 bytes: check that TAG does not point to APETAGEX (BUG 27.08.06)
  tmp->Clear();
  tmp->AddMemory(tail.m_pData + length - 131, 131);
  if ( (memcmp(tmp->m_pData, _T("APETAGEX"), 8) == 0)
	  || (tmp->GetAt(3) != 'T' || tmp->GetAt(4) != 'A' || tmp->GetAt(5) != 'G') )
	  return false;
  // the enhanced tag "TAG+" is in front of the id3v1 tag
  if (length == ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE && memcmp(tail.m_pData, "TAG+", 4) == 0)
  {
    _enhanced = true;
    enhanced.AddMemory(tail.m_pData, ID3V1_ENHANCED_SIZE);
    memcpy(_enhancedRest, tail.m_pData + 4 + 180, ID3V1_ENHANCED_REST);
  }
  CBlob *ext = _enhanced ? &enhanced : NULL;
  Title = ReadFieldWithExtension(tmp, 6, 30, ext, 4, 60);
  Artist = ReadFieldWithExtension(tmp, 36, 30, ext, 64, 60);
  Album = ReadFieldWithExtension(tmp, 66, 30, ext, 124, 60);
  Year = ReadFixedField(tmp, 96, 4);
  Comment = ReadFixedField(tmp, 100, 30);
  // id3v1.1: the last byte of the comment is a track number if the byte before it is $00
  // (a comment of 30 characters with a blank at position 29 is not a v1.1 tag)
  if (tmp->GetAt(128) == 0 && tmp->GetAt(129) != 0)
    Track = tmp->GetAt(129);
  else
    Track = 0;

  Genre = tmp->GetAt(130);
  _exists = true;
  return true;
}

// the longest text that is written: 30 to 90 characters (configuration ID3V1MAXTEXTLENGTH)
static int MaxTextLength()
{
  const long value = CTools::configValues[CONFIG_ID3V1MAXTEXTLENGTH];
  return (value < 30) ? 30 : ((value > 90) ? 90 : (int)value);
}

// enhanced tag: speed (0 = not set, 1 slow, 2 medium, 3 fast, 4 hardcore), genre (free text of 30 characters),
// start and end time ("mmm:ss"); the 43 bytes are _enhancedRest[0], [1..30], [31..36] and [37..42]
int CID3V1TagInfo::GetSpeed()
{
  return _enhancedRest[0];
}

void CID3V1TagInfo::SetSpeed(int value)
{
  _enhancedRest[0] = (value >= 0 && value <= 4) ? (BYTE)value : 0;
}

CAtlString CID3V1TagInfo::GetEnhancedGenre()
{
  CBlob field;
  field.AddMemory(_enhancedRest + 1, 30);
  return ReadFixedField(&field, 0, 30);
}

void CID3V1TagInfo::SetEnhancedGenre(LPCWSTR value)
{
  CBlob field;
  field.AddFixedAnsiString(value, 30);
  memset(_enhancedRest + 1, 0, 30);
  memcpy(_enhancedRest + 1, field.m_pData, min(field.GetLength(), (size_t)30));
}

CAtlString CID3V1TagInfo::GetTime(bool end)
{
  const BYTE *time = _enhancedRest + (end ? 37 : 31);
  CAtlString result;
  for (int i = 0; i < 6 && time[i] != 0; i++)
    result += (TCHAR)time[i];
  return result;
}

// a time has the form mmm:ss (seconds below 60) or is empty; anything else is not accepted
bool CID3V1TagInfo::SetTime(bool end, LPCWSTR value)
{
  BYTE *time = _enhancedRest + (end ? 37 : 31);
  const size_t length = (value == NULL) ? 0 : wcslen(value);
  if (length == 0)
  {
    memset(time, 0, 6);
    return true;
  }
  if (length != 6 || !iswdigit(value[0]) || !iswdigit(value[1]) || !iswdigit(value[2]) || value[3] != L':'
    || value[4] < L'0' || value[4] > L'5' || !iswdigit(value[5]))
    return false;
  for (int i = 0; i < 6; i++)
    time[i] = (BYTE)value[i];
  return true;
}

bool CID3V1TagInfo::needsEnhanced()
{
  const int maxLength = MaxTextLength();
  if (min(Title.GetLength(), maxLength) > 30 || min(Artist.GetLength(), maxLength) > 30 || min(Album.GetLength(), maxLength) > 30)
    return true;
  for (int i = 0; i < ID3V1_ENHANCED_REST; i++)
    if (_enhancedRest[i] != 0)
      return true;
  return false;
}

bool CID3V1TagInfo::WriteToFile(FILE *Stream)
{
  errno = 0;
  tmp->Clear();
  if (needsEnhanced())
  {
    // the parts of title, artist and album behind the 30th character (up to 60 more), then speed, genre and times as they were
    tmp->AddMemory("TAG+", 4);
    const int maxLength = MaxTextLength();
    tmp->AddFixedAnsiString(Title.Left(maxLength).Mid(30), 60);
    tmp->AddFixedAnsiString(Artist.Left(maxLength).Mid(30), 60);
    tmp->AddFixedAnsiString(Album.Left(maxLength).Mid(30), 60);
    tmp->AddMemory(_enhancedRest, ID3V1_ENHANCED_REST);
  }
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
  tmp->FileWrite(tmp->GetLength(), Stream);
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
  _enhanced = false;
  memset(_enhancedRest, 0, sizeof(_enhancedRest));
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
  // the track number is one byte: a value that does not fit means 'no track'
  const int value = _wtoi(newValue);
  Track = (value > 0 && value <= 255) ? (BYTE)value : 0;
}

void CID3V1TagInfo::SetGenre(LPCWSTR newValue)
{
  CAtlString vgl = newValue;
  Genre = DEFAULT_GENRE;
  for (int i=0; i < MAX_MUSIC_GENRES; i++)
    if (vgl.CompareNoCase(MUSIC_GENRE[i]) == 0)
      Genre = (BYTE)i;
  // names that version 3.0 used for three genres
  if (Genre == DEFAULT_GENRE)
  {
    if (vgl.CompareNoCase(_T("Top")) == 0) Genre = 60;
    else if (vgl.CompareNoCase(_T("Pop & Funk")) == 0) Genre = 62;
    else if (vgl.CompareNoCase(_T("Trash Metal")) == 0) Genre = 144;
  }
}
