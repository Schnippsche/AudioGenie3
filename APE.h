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



/* -------------------------------------------------------------------------- */
/* APE.h              for use with APE.cpp                                    */
/* by Stefan Toengi (c) 2003                                                  */
/* -------------------------------------------------------------------------- */
#pragma once

#include "Tools.h"
#include "Blob.h"
#include "ApeTagInfo.h"
#include "id3v1taginfo.h"
#include "ApeTagItem.h"
#include "resource.h"
#include "id3v1.h"
#include "atlcoll.h"

/* First version of APE tag */
#define APE_VERSION_1_0    1000
/* Second version of APE tag */
#define APE_VERSION_2_0    2000

static const LPCWSTR APE_TITLE(_T("TITLE"));
static const LPCWSTR APE_ARTIST(_T("ARTIST"));
static const LPCWSTR APE_ALBUM(_T("ALBUM"));
static const LPCWSTR APE_TRACK(_T("TRACK"));
static const LPCWSTR APE_YEAR(_T("YEAR"));
static const LPCWSTR APE_GENRE(_T("GENRE"));
static const LPCWSTR APE_COMMENT(_T("COMMENT"));
static const LPCWSTR APE_COPYRIGHT(_T("COPYRIGHT"));
static const LPCWSTR APE_COMPOSER(_T("COMPOSER"));

/* Max. number of unsupported tag fields */

class CAPE 
{
private:
	CApeTagInfo TagInfo;
	int FVersion;
	CID3V1 tmpid3v1;
	CApeTagItem* item;
	CBlob Data;
	bool ReadFooter(FILE *Stream);
	__int64 _footerPos;	// position of the footer of an APE tag at the end of the file
	bool LocateTail(LPCWSTR FileName, __int64 &start, __int64 &total);
	bool ReadFields(FILE *Stream, __int64 headOffset);
	bool ReadHeadTag(FILE *Stream, __int64 offset, __int64 length);
	bool RewriteRegion(LPCWSTR FileName, __int64 offset, __int64 oldLength, CBlob *data);
	void BuildTagData();
	bool TruncateFile(LPCWSTR FileName, int Offset);
	bool SaveTag(LPCWSTR FileName);
	void BuildFooter();
	bool AddToFile(LPCWSTR FileName);
	CAtlArray<CApeTagItem *> _items;
public:
	CAPE();
	virtual ~CAPE();
	bool SetTagItem(LPCWSTR FieldName, LPCWSTR Value);
	CAtlString GetTagItem(LPCWSTR key);
	// a tag at the end of the file or at the beginning (behind an ID3v2 tag)
	bool Exists()          { return (CTools::APESize > 0 || (CTools::APEHeadSize > 0 && TagInfo.Version != 0)); };
	// looks for a tag at the beginning of the file: directly at the start or behind an ID3v2 tag; it begins with a header
	static bool FindHeadTag(FILE *Stream, __int64 &offset, __int64 &length);
	// looks for the footer of a tag at the end of the file: directly in front of the ID3v1 data or in front of a Lyrics3 v2.00 tag
	// that is between them (lyricsAfter = its size)
	static bool FindTailFooter(FILE *Stream, int id3v1Size, bool checkLyrics, __int64 &footerPos, __int64 &lyricsAfter);
	void ResetData();
	void Print();
	bool ReadFromFile(FILE *Stream);
	bool RemoveFromFile(LPCWSTR FileName, bool saveID3v1Tag);
	bool SaveToFile(LPCWSTR FileName);
	CAtlString GetTagVersion();  
	CAtlString GetAllKeys();
	long GetSize()         { return TagInfo.Size; };
	
	CAtlString GetUserDefined(CAtlString key)     { return GetTagItem(key); };
	void SetUserDefined(LPCWSTR key, LPCWSTR value) { SetTagItem(key, value); };
};


