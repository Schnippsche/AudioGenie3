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

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include "io.h"
#include <fcntl.h>
#include "lyrics.h"
#include "Blob.h"
#include "share.h"
#include "sys\stat.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLyrics::CLyrics()
{
	FStartPosition = 0;
	FEndPosition = 0;
	FFileSize = 0;
	FVersion = LYRICS_VERSION_UNKNOWN;
	ID3v1AreaSize = 0;
	memset(ID3v1Area, 0, sizeof(ID3v1Area));
	memset(FHeader, 0, sizeof(FHeader));
}
CLyrics::~CLyrics()
{
}

/* -------------------------------------------------------------------------- */

void CLyrics::ResetData()
{
	/* Reset all variables */
	for (int i = 0; i < LYRICS_FRAME_COUNT; i++)
		FField[i].Empty();

	Data.Clear();
	FUnknown.Clear();
	memset(ID3v1Area, 0, sizeof(ID3v1Area));
	ID3v1AreaSize = 0;
	FVersion = LYRICS_VERSION_UNKNOWN;
	FStartPosition = 0;
	FEndPosition = 0;
	CTools::LyricsSize = 0;
}

/* -------------------------------------------------------------------------- */

// Writes the region [start, start + oldLength) of the file again with the data (may be NULL). The lyrics tag is usually directly in
// front of the ID3v1 data: then only the end of the file is written, otherwise (an APE tag is behind it) the file is rewritten.
bool CLyrics::WriteRegion(LPCWSTR FileName, __int64 start, __int64 oldLength, CBlob *data)
{
	const long dataLength = (data != NULL) ? (long)data->GetLength() : 0;
	if (start + oldLength != FFileSize - ID3v1AreaSize)
		return CTools::rewriteRegion(FileName, start, oldLength, data);
	int fh;
	if (_wsopen_s(&fh, FileName, O_RDWR | _O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE ) != 0)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	bool ok = (_lseeki64(fh, start, SEEK_SET) >= 0);
	if (ok && dataLength > 0)
		ok = (_write(fh, data->m_pData, dataLength) == dataLength);
	/* the ID3 tag (with the enhanced tag in front of it, if there is one) follows the lyrics tag */
	if (ok)
		ok = (_write(fh, ID3v1Area, ID3v1AreaSize) == ID3v1AreaSize);
	if (ok)
		ok = (_chsize_s(fh, start + dataLength + ID3v1AreaSize) == 0);
	_close(fh);
	if (!ok)
		CTools::instance().setLastError(errno != 0 ? errno : EIO);
	return ok;
}

/* -------------------------------------------------------------------------- */

// stores the data of a defined field; returns false if the field is not defined
bool CLyrics::SetTagItem(const char ID[], long Pos, long DataSize)
{
	for (int Iterator = 0; Iterator < LYRICS_FRAME_COUNT; Iterator++)
	{
		if (memcmp(ID, FIELD_LIST[Iterator], 3) == 0)
		{
			FField[Iterator] = Data.GetStringAt(Pos, DataSize);
			return true;
		}
	}
	return false;
}

/* -------------------------------------------------------------------------- */

bool CLyrics::ReadHeader(FILE *Stream)
{
	FVersion = LYRICS_VERSION_UNKNOWN;
	ID3v1AreaSize = 0;
	memset(ID3v1Area, 0, sizeof(ID3v1Area));
	const __int64 fileSize = _filelengthi64(_fileno(Stream));
	/* the lyrics tag is in front of the id3v1 tag, which has to exist */
	BYTE last[ID3V1_TAG_SIZE];
	if (fileSize < ID3V1_TAG_SIZE || _fseeki64(Stream, fileSize - ID3V1_TAG_SIZE, SEEK_SET) != 0)
		return false;
	if (fread(last, 1, ID3V1_TAG_SIZE, Stream) != ID3V1_TAG_SIZE || memcmp(last, ID3V1_ID, 3) != 0)
		return false;
	/* an enhanced tag (TAG+) is part of the id3v1 data: the lyrics tag is in front of it */
	int area = ID3V1_TAG_SIZE;
	char plus[4];
	if (fileSize >= ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE && _fseeki64(Stream, fileSize - ID3V1_TAG_SIZE - ID3V1_ENHANCED_SIZE, SEEK_SET) == 0
		&& fread(plus, 1, 4, Stream) == 4 && memcmp(plus, "TAG+", 4) == 0)
		area = ID3V1_TAG_SIZE + ID3V1_ENHANCED_SIZE;
	if (_fseeki64(Stream, fileSize - area, SEEK_SET) != 0 || fread(ID3v1Area, 1, area, Stream) != (size_t)area)
		return false;
	ID3v1AreaSize = area;
	FFileSize = fileSize;
	/* an APE tag (footer and items, header) can be between the lyrics tag and the id3v1 data */
	__int64 end = fileSize - area;
	BYTE footer[32];
	if (end >= 32 && _fseeki64(Stream, end - 32, SEEK_SET) == 0 && fread(footer, 1, 32, Stream) == 32 && memcmp(footer, "APETAGEX", 8) == 0)
	{
		const unsigned long version = footer[8] | (footer[9] << 8) | (footer[10] << 16) | ((unsigned long)footer[11] << 24);
		const unsigned long size = footer[12] | (footer[13] << 8) | (footer[14] << 16) | ((unsigned long)footer[15] << 24);
		const unsigned long flags = footer[20] | (footer[21] << 8) | (footer[22] << 16) | ((unsigned long)footer[23] << 24);
		const __int64 total = (__int64)size + ((version >= 2000 && (flags & 0x80000000ul) != 0) ? 32 : 0);
		if (total >= 32 && total <= end)
			end -= total;
	}
	/* Read header and get Version & Size */
	FEndPosition = end - 9;
	if (FEndPosition < 0)
		return false;
	_fseeki64(Stream, FEndPosition, SEEK_SET);
	if (fread(FHeader, 1, 9, Stream) != 9)
		return false;
	/* check if Lyrics-Tag exists */
	if (memcmp(FHeader, LYRICS_BEGIN, 6) != 0)
		return false;
	/* check for Version */
	if (memcmp(FHeader, LYRICS_OLD, 9) == 0)
	{
		FVersion = LYRICS_VERSION_100;
		return true;
	}
	if (memcmp(FHeader, LYRICS_NEW, 9) == 0)
	{
		FVersion = LYRICS_VERSION_200;
		return true;
	}

	return false;
}

/* -------------------------------------------------------------------------- */

// Lyrics3 v1.00: "LYRICSBEGIN", the text (up to 5100 bytes), "LYRICSEND"
bool CLyrics::ReadFramesOld(FILE *Stream, bool isDeleting)
{
	__int64 StartPosition = FEndPosition - (5100 + 11);
	if (StartPosition < 0)
		StartPosition = 0;
	const long DataSize = (long)(FEndPosition - StartPosition);
	Data.Clear();
	if (!isDeleting)
		FUnknown.Clear();
	_fseeki64(Stream, StartPosition, SEEK_SET);
	Data.FileRead(DataSize, Stream);
	/* Search for Begin: the last one belongs to the tag */
	for (long Iterator = (long)Data.GetLength() - 11; Iterator >= 0; Iterator--)
	{
		if (memcmp(Data.m_pData + Iterator, LYRICS_BEGIN, 11) == 0)
		{
			FStartPosition = StartPosition + Iterator;
			if (!isDeleting)
			{
				CTools::LyricsSize = (int)(FEndPosition + 9 - FStartPosition);   // the whole tag including LYRICSBEGIN and LYRICSEND
				FField[1] = Data.GetStringAt(Iterator + 11, DataSize - Iterator - 11);
			}
			return true;
		}
	}
	return false;
}

/* -------------------------------------------------------------------------- */

// Lyrics3 v2.00: "LYRICSBEGIN", fields (ID, size with 5 digits, data), size of "LYRICSBEGIN" and the fields (6 digits), "LYRICS200"
bool CLyrics::ReadFramesNew(FILE *Stream, bool isDeleting)
{
	char Buffer[8];
	memset(Buffer, 0, sizeof(Buffer));
	Data.Clear();
	if (!isDeleting)
		FUnknown.Clear();
	/* Get information from Lyrics (Version 2.00) */
	_fseeki64(Stream, FEndPosition - 6, SEEK_SET);
	if (fread(Buffer, 1, 6, Stream) != 6)
		return false;
	/* Convert from Char into Size: six digits */
	long DataSize = 0;
	for (int i = 0; i < 6; i++)
	{
		if (Buffer[i] < '0' || Buffer[i] > '9')
			return false;
		DataSize = DataSize * 10 + (Buffer[i] - '0');
	}
	if (DataSize < 11 || (FEndPosition - 6 - DataSize) < 0)
		return false;
	FStartPosition = FEndPosition - 6 - DataSize;
	_fseeki64(Stream, FStartPosition, SEEK_SET);
	Data.FileRead(DataSize, Stream);
	/* the tag has to begin with LYRICSBEGIN */
	if ((long)Data.GetLength() != DataSize || memcmp(Data.m_pData, LYRICS_BEGIN, 11) != 0)
	{
		Data.Clear();
		return false;
	}
	if (isDeleting)
		return true;
	CTools::LyricsSize = DataSize + 6 + 9;   // the whole tag: LYRICSBEGIN, fields, size and LYRICS200
	long Iterator = 11;
	while (Iterator + 8 <= DataSize)
	{
		const char *field = (const char *)Data.m_pData + Iterator;
		long fieldSize = 0;
		int digits = 0;
		while (digits < 5 && field[3 + digits] >= '0' && field[3 + digits] <= '9')
		{
			fieldSize = fieldSize * 10 + (field[3 + digits] - '0');
			digits++;
		}
		/* a damaged field: the fields read so far are kept */
		if (digits < 5 || Iterator + 8 + fieldSize > DataSize)
			break;
		if (!SetTagItem(field, Iterator + 8, fieldSize))
			FUnknown.AddMemory(field, 8 + fieldSize);   // unknown fields are written again when the tag is saved
		Iterator += 8 + fieldSize;
	}
	return true;
}

/* -------------------------------------------------------------------------- */

void CLyrics::ReadFromFile(FILE *Stream)
{
	/* Process data if loaded and header valid */
	if (ReadHeader(Stream))
	{
		/* Get information from frames if version supported */
		const bool valid = (FVersion == LYRICS_VERSION_200) ? ReadFramesNew(Stream, false) : ReadFramesOld(Stream, false);
		if (valid)
			return;
	}
	ResetData();
}

/* -------------------------------------------------------------------------- */

bool CLyrics::RemoveFromFile(LPCWSTR FileName)
{
	FILE *Stream;
	if ( (Stream = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) != NULL)
	{
		bool Result = ReadHeader(Stream);
		if (Result)
			Result = (FVersion == LYRICS_VERSION_200) ? ReadFramesNew(Stream, true) : ReadFramesOld(Stream, true);
		fclose(Stream);
		if (!Result)
		{
			CTools::instance().setLastError(ERR_TAG_NOT_EXIST);
			return false;
		}
		return WriteRegion(FileName, FStartPosition, FEndPosition + 9 - FStartPosition, NULL);
	}
	CTools::instance().setLastError(errno);
	return false;
}

/* -------------------------------------------------------------------------- */

// the longest field in bytes; the indication has two characters, the lyrics, the information and the image links have 99999
static const int FIELD_MAX_SIZE[LYRICS_FRAME_COUNT] = { 2, 99999, 99999, 250, 250, 250, 250, 99999, 250 };
// fields with several lines: the lines are separated by CR LF
static const bool FIELD_MULTILINE[LYRICS_FRAME_COUNT] = { false, true, true, false, false, false, false, true, false };

static CAtlString NormalizeLineBreaks(const CAtlString &text)
{
	CAtlString result;
	const int length = text.GetLength();
	for (int i = 0; i < length; i++)
	{
		const TCHAR c = text[i];
		if (c == _T('\r'))
		{
			result += _T("\r\n");
			if (i + 1 < length && text[i + 1] == _T('\n'))
				i++;
		}
		else if (c == _T('\n'))
			result += _T("\r\n");
		else
			result += c;
	}
	return result;
}

// builds the tag in Data; Data is empty if there is no field
void CLyrics::BuildTagData()
{
	char Buffer[16];
	Data.Clear();
	Data.AddMemory(LYRICS_BEGIN, 11);
	for (long Iterator = 0; Iterator < LYRICS_FRAME_COUNT; Iterator++)
	{
		CAtlString text = FField[Iterator];
		if (text.GetLength() == 0)
			continue;
		/* the indication field consists of two characters, 0 or 1 */
		if (Iterator == 0 && !((text == _T("00")) || (text == _T("01")) || (text == _T("10")) || (text == _T("11"))))
		{
			CTools::instance().writeWarning(L"lyrics3 indication '%s' ignored: it has two characters, 0 or 1", (LPCTSTR)text);
			continue;
		}
		if (FIELD_MULTILINE[Iterator])
			text = NormalizeLineBreaks(text);
		CBlob encoded;
		encoded.AddEncodedString(TEXT_ENCODED_ANSI, text, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
		/* a field is limited: cut the text */
		while ((int)encoded.GetLength() > FIELD_MAX_SIZE[Iterator] && text.GetLength() > 0)
		{
			text = text.Left(text.GetLength() - max(1, (int)encoded.GetLength() - FIELD_MAX_SIZE[Iterator]));
			encoded.Clear();
			encoded.AddEncodedString(TEXT_ENCODED_ANSI, text, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
		}
		if (encoded.GetLength() == 0)
			continue;
		/* the byte value 255 must not occur in the text */
		for (size_t i = 0; i < encoded.GetLength(); i++)
			if (encoded.m_pData[i] == 0xFF)
				encoded.m_pData[i] = '?';
		Data.AddMemory(FIELD_LIST[Iterator], 3);
		sprintf_s(Buffer, 16, "%05i", (int)encoded.GetLength());
		Data.AddMemory(&Buffer, 5);
		Data.AddBlob(encoded);
	}
	/* fields that are not defined are written as they were read */
	if (FUnknown.GetLength() > 0 && Data.GetLength() + FUnknown.GetLength() <= 999999)
		Data.AddBlob(FUnknown);
	/* no field: a lyrics tag needs at least one, so there is no tag */
	if (Data.GetLength() == 11)
	{
		Data.Clear();
		return;
	}
	sprintf_s(Buffer, 16, "%06iLYRICS200", (int)Data.GetLength());
	Data.AddMemory(Buffer, 15);
}

/* -------------------------------------------------------------------------- */

bool CLyrics::SaveToFile(LPCWSTR FileName)
{
	/* Prepare tag record */
	FILE *Stream;
	bool hadTag = false;
	if ( (Stream = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	if (ReadHeader(Stream))
		hadTag = (FVersion == LYRICS_VERSION_200) ? ReadFramesNew(Stream, true) : ReadFramesOld(Stream, true);
	fclose(Stream);
	/*  ID3v1-Tag must exist! */
	if (ID3v1AreaSize == 0)
	{
		CTools::instance().setLastError(ERR_V1TAG_MISSING);
		return false;
	}
	BuildTagData();
	/* an existing tag is replaced at its place (an APE tag can be behind it), a new tag is written in front of the ID3v1 data */
	__int64 start = FFileSize - ID3v1AreaSize;
	__int64 oldLength = 0;
	if (hadTag)
	{
		start = FStartPosition;
		oldLength = FEndPosition + 9 - FStartPosition;
	}
	if (Data.GetLength() == 0)
	{
		if (!hadTag)
			return true;
		FVersion = LYRICS_VERSION_UNKNOWN;
		CTools::LyricsSize = 0;
		return WriteRegion(FileName, start, oldLength, NULL);
	}
	const bool ok = WriteRegion(FileName, start, oldLength, &Data);
	if (ok)
	{
		FVersion = LYRICS_VERSION_200;
		CTools::LyricsSize = (int)Data.GetLength();
	}
	return ok;
}

CAtlString CLyrics::GetTagVersion()
{
	_TCHAR Buf[16];
	if (FVersion == LYRICS_VERSION_UNKNOWN)
		return EMPTY;
	_stprintf_s(Buf, 16, _T("%i.00"), FVersion);
	return CAtlString(Buf);
}
