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

// ID3V1.cpp: implementation of class CID3V1. ( no Unicode )
// writes ANSI only
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ID3V1.h"
#include "Blob.h"
#include "id3v1taginfo.h"
#include "io.h"
#include <fcntl.h>
#include "sys/stat.h"
#include "share.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CID3V1::CID3V1()
{
	Stream = NULL;
	fileEnhanced = false;
}

CID3V1::~CID3V1()
{
}

void CID3V1::ResetData()
{
	id3v1tag.Reset();
	CTools::ID3v1Size = 0;
}

int CID3V1::DetectSize(FILE *Stream)
{
	const __int64 fileSize = _filelengthi64(_fileno(Stream));
	char id[4];
	if (fileSize < ID3V1_TAG_SIZE || _fseeki64(Stream, fileSize - ID3V1_TAG_SIZE, SEEK_SET) != 0 || fread(id, 1, 3, Stream) != 3 || memcmp(id, ID3V1_ID, 3) != 0)
		return 0;
	if (fileSize >= ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE && _fseeki64(Stream, fileSize - ID3V1_TAG_SIZE - ID3V1_ENHANCED_SIZE, SEEK_SET) == 0
		&& fread(id, 1, 4, Stream) == 4 && memcmp(id, "TAG+", 4) == 0)
		return ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE;
	return ID3V1_TAG_SIZE;
}

bool CID3V1::OpenFile(LPCWSTR FileName, bool WriteModus)
{
	CAtlString modus;
	if (WriteModus)
		modus = READ_AND_WRITE;
	else
		modus = READ_ONLY;
	id3v1tag._exists = false;			
	fileEnhanced = false;
	if ( (Stream = _wfsopen(FileName, modus, _SH_DENYNO)) != NULL)
  {
		/* Read tag */
		char ap[10];
		_fseeki64(Stream, -131, SEEK_END);
		fread(ap, 1, 8, Stream);
		if (ap[0] == 'A' && ap[1] == 'P' && ap[2] == 'E' && ap[3] == 'T' && ap[4] == 'A'&& ap[5] == 'G' && ap[6] == 'E' && ap[7] == 'X')
		{
			id3v1tag._exists = false;			
		}
		else if(ap[3] == 'T' && ap[4] == 'A' && ap[5] == 'G')
		{
			id3v1tag._exists = true;
		}
		// enhanced tag "TAG+" in front of the id3v1 tag
		if (id3v1tag._exists && _fseeki64(Stream, -(ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE), SEEK_END) == 0)
		{
			char plus[4];
			fileEnhanced = (fread(plus, 1, 4, Stream) == 4 && memcmp(plus, "TAG+", 4) == 0);
		}
		_fseeki64(Stream, -(fileEnhanced ? ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE : ID3V1_TAG_SIZE), SEEK_END);
		return true;
	}
	CTools::instance().setLastError(errno);
	return false;
}

void CID3V1::CloseFile()
{
	if (Stream != NULL)
		fclose(Stream);
}

void CID3V1::ReadFromFile(FILE *Stream)
{
	/* Reset and load tag data from file to variable */
	//_fseeki64(Stream, -ID3V1_TAG_SIZE, SEEK_END);
	if (id3v1tag.ReadFromFile(Stream))
	{
		CTools::ID3v1Size = id3v1tag._enhanced ? ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE : ID3V1_TAG_SIZE;
		return;
	}
	//ResetData();
}

CAtlString CID3V1::GetGenre()
{
	if (id3v1tag.Genre >= 0 && id3v1tag.Genre < MAX_MUSIC_GENRES)
		return MUSIC_GENRE[id3v1tag.Genre];
	else
		return EMPTY;
}

CAtlString CID3V1::GetGenreItem(int i)
{
	if (i >= 0 && i < MAX_MUSIC_GENRES)
		return MUSIC_GENRE[i];
	else
		return EMPTY;
}

bool CID3V1::RemoveTag(LPCWSTR FileName)
{
	int fh;
	/* Open a file */
	if (_wsopen_s(&fh, FileName, _O_WRONLY | _O_BINARY, _SH_DENYWR, _S_IWRITE) == 0)
	{
		long ln = _filelength(fh) - (fileEnhanced ? ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE : ID3V1_TAG_SIZE);
		if (ln > 0)
			_chsize_s(fh, ln);
		_close(fh);
		return true;
	}
	else
		CTools::instance().setLastError(errno);
	return false;
};

bool CID3V1::RemoveFromFile(LPCWSTR FileName)
{
	/* Find tag */
	bool Result = OpenFile(FileName, true);
	CloseFile();
	if (!Result)
		return false;
	/* Delete tag if loaded and tag header OK */
	if (id3v1tag.exists())
		return RemoveTag(FileName);

	CTools::instance().setLastError(ERR_TAG_NOT_EXIST);
	return false;
};

bool CID3V1::AddTag(LPCWSTR FileName)
{
	bool result = false;
	/* Write tag */
	if ( (Stream = _wfsopen(FileName, APPEND, _SH_DENYWR)) != NULL)
  {
		result = id3v1tag.WriteToFile(Stream);
		fclose(Stream);
	}
	else
		CTools::instance().setLastError(errno);
	return result;
};

bool CID3V1::SaveToFile(LPCWSTR FileName)
{
	bool Result = OpenFile(FileName, true);
	// replace if it exists
	if (Result && id3v1tag.exists())
	{
		if (id3v1tag.needsEnhanced() == fileEnhanced)
		{
			_fseeki64(Stream, -id3v1tag.GetSize(), SEEK_END);
			Result = id3v1tag.WriteToFile(Stream);
			CloseFile();
		}
		else
		{
			// the size of the tag changes (the enhanced tag is added or removed): write it again at the end of the file
			CloseFile();
			Result = RemoveTag(FileName) && AddTag(FileName);
		}
	}
	else
	{
		CloseFile();
		Result = AddTag(FileName);
	}
	return Result;
};
