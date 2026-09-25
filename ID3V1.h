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

// ID3V1.h: interface for class CID3V1.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Tools.h"
#include "id3v1taginfo.h"
#include "resource.h"

class CID3V1
{
	// Construction
public:
	CID3V1();
	~CID3V1();
	void ResetData();
	void ReadFromFile(FILE *Stream);
	bool Exists()              { return (CTools::ID3v1Size > 0);          };
	/* Getter */
	CAtlString GetAlbum()         { return id3v1tag.Album;           };
	CAtlString GetArtist()        { return id3v1tag.Artist;          };
	CAtlString GetComment()       { return id3v1tag.Comment;         };
	CAtlString GetGenre();
	CAtlString GetTrack()         { return id3v1tag.GetTrack();      };
	CAtlString GetTitle()         { return id3v1tag.Title;           };
	CAtlString GetYear()          { return id3v1tag.Year;      };
	CAtlString GetTagVersion()    { return id3v1tag.GetTagVersion(); };
	/* Setter */
	void SetGenre(LPCWSTR value)        { id3v1tag.SetGenre(value);  };
	void SetGenreID(int value)          { id3v1tag.Genre = (BYTE)value;  };
	void SetTrack(LPCWSTR value)        { id3v1tag.SetTrack(value);  };
	void SetYear(LPCWSTR newYear)       { id3v1tag.Year = newYear; };
	void SetTitle(LPCWSTR newTitle)     { id3v1tag.Title = newTitle; };
	void SetArtist(LPCWSTR newArtist)   { id3v1tag.Artist = newArtist; };
	void SetAlbum(LPCWSTR newAlbum)     { id3v1tag.Album = newAlbum; };
	void SetComment(LPCWSTR newComment) { id3v1tag.Comment = newComment; };
	// general
	int GetGenreID()            { return id3v1tag.Genre;   };
	int NumberOfGenres()        { return MAX_MUSIC_GENRES; };
	CAtlString GetGenreItem(int i);
	long GetSize()              { return CTools::ID3v1Size; };
	bool RemoveFromFile(LPCWSTR FileName);
	bool SaveToFile(LPCWSTR FileName);
	bool OpenFile(LPCWSTR FileName, bool WriteModus);

private:
	bool RemoveTag(LPCWSTR FileName);
	bool AddTag(LPCWSTR FileName);
	void CloseFile();
	FILE *Stream;
	CID3V1TagInfo id3v1tag;
};

