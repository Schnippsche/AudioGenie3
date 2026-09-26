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
/* Lyrics.h          for use with Lyrics.cpp                                  */
/* by Stefan Toengi (c) 2003                                                  */
/* -------------------------------------------------------------------------- */
#pragma once

#include "Blob.h"
#include "id3v1taginfo.h"
#include "resource.h"
#include "Tools.h"

static const char *LYRICS_BEGIN = "LYRICSBEGIN";
static const char *LYRICS_OLD   = "LYRICSEND";
static const char *LYRICS_NEW   = "LYRICS200";

#define LYRICS_VERSION_UNKNOWN 0
#define LYRICS_VERSION_100     1
#define LYRICS_VERSION_200     2

#define LYRICS_FRAME_COUNT     9

static const char *FIELD_LIST[] = {
	"IND","LYR","INF","AUT","EAL","EAR","ETT","IMG","GRE" };

	class CLyrics
	{
	private:
		__int64 FStartPosition;
		__int64 FEndPosition;
		__int64 FFileSize;
		CAtlString FField[LYRICS_FRAME_COUNT];
		CBlob Data;
		BYTE ID3v1Area[ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE];	// the id3v1 data at the end of the file: the id3v1 tag and the enhanced tag in front of it, if there is one
		int ID3v1AreaSize;
		CBlob FUnknown;		// fields that are not defined, as they were read
		BYTE FHeader[12];/* 9 Bytes Version 3 Bytes 'TAG' */
		BYTE FVersion;
		bool WriteRegion(LPCWSTR FileName, __int64 start, __int64 oldLength, CBlob *data);
		bool SetTagItem(const char ID[], long Pos, long DataSize);
		bool ReadHeader(FILE *Stream);
		bool ReadFramesOld(FILE *Stream, bool isDeleting);
		bool ReadFramesNew(FILE *Stream, bool isDeleting);
		void BuildTagData();
	public:
		CLyrics();
		virtual ~CLyrics();
		void ReadFromFile(FILE *Stream);
		bool RemoveFromFile(LPCWSTR FileName);
		bool SaveToFile(LPCWSTR FileName);
		CAtlString GetTagVersion();
		bool Exists()                        { return (CTools::LyricsSize > 0); };
		long GetSize()                       { return CTools::LyricsSize;};
		void ResetData();
		CAtlString GetIndication()              { return FField[0]; };
		CAtlString GetLyrics()                  { return FField[1]; };
		CAtlString GetInformation()             { return FField[2]; };
		CAtlString GetAuthor()                  { return FField[3]; };
		CAtlString GetAlbum()                   { return FField[4]; };
		CAtlString GetArtist()                  { return FField[5]; };
		CAtlString GetTitle()                   { return FField[6]; };
		CAtlString GetImageLink()               { return FField[7]; };
		CAtlString GetGenre()                   { return FField[8]; };
		void SetIndication(LPCWSTR NewText)  { FField[0] = NewText;   };
		void SetLyrics(LPCWSTR NewText)      { FField[1] = NewText;   };
		void SetInformation(LPCWSTR NewText) { FField[2] = NewText;   };
		void SetAuthor(LPCWSTR NewText)      { FField[3] = NewText;   };
		void SetAlbum(LPCWSTR NewText)       { FField[4] = NewText;   };
		void SetArtist(LPCWSTR NewText)      { FField[5] = NewText;   };
		void SetTitle(LPCWSTR NewText)       { FField[6] = NewText;   };
		void SetImageLink(LPCWSTR NewText)   { FField[7] = NewText;   };
		void SetGenre(LPCWSTR NewText)       { FField[8] = NewText;   };
		__int64 GetStartPosition()              { return FStartPosition; };
	};



