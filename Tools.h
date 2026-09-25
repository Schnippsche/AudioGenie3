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

#pragma once

#include "Blob.h"

enum ERR_NUMBERS {
	ERR_V1TAG_MISSING = 201,
	ERR_WRONG_LANG_ID = 202,
	ERR_TOO_MUCH_FRAMES = 203,
	ERR_TAG_NOT_EXIST = 204,
	ERR_WMAWRITE_NOT_SUPPORTED = 205,
	ERR_MP4WRITE_NOT_SUPPORTED = 206,
	ERR_ID_OUT_OF_RANGE = 207,
	ERR_SYNCPARSE_ERROR = 208,
	ERR_TAG_NOT_ALLOWED = 209,
	ERR_WMA_PARSE = 210,
	ERR_WMA_PICTURE_TOO_BIG = 211,
	ERR_WAVWRITE_NOT_SUPPORTED = 212,
	ERR_PICTUREARRAY_TOO_SMALL = 213,
	ERR_IMAGEURL_NOT_FOUND = 214,
	ERR_WRONG_FORMAT = 215,
	ERR_FRAME_NOT_FOUND = 216,
	ERR_INVALID_FILENAME = 217,
	ERR_INVALID_FORMAT = 218,
	ERR_NOT_ENOUGH_MEMORY = 219,
	ERR_FRAME_TOO_BIG = 220,
	ERR_ATOM_IS_NO_TEXTCLASS = 221,
	ERR_COMPRESSED_FRAME_NOT_SUPPORTED = 222,
	ERR_CHAPTER_FRAME_CORRUPT = 223,
	ERR_UTF8_NOT_ALLOWED = 224,
	ERR_FRAME_CORRUPT = 225,
	ERR_TEXTCONVERT = 226,
	ERR_NO_DATACHUNK = 227
};

static const LPCWSTR IMAGE_SHORT[] = { L"XXX", L"JPG", L"GIF", L"PNG", L"BMP",L"TIF",L"-->"} ;

#define IMAGE_UNKNOWN 0
#define IMAGE_JPG 1
#define IMAGE_GIF 2
#define IMAGE_PNG 3
#define IMAGE_BMP 4
#define IMAGE_TIFF 5
#define IMAGE_LINK 6


static const LPCWSTR IMAGE_LONG[] = { L"XXX", L"image/jpeg", L"image/gif", L"image/png", L"image/bmp",L"image/tiff",L"-->"} ;

static const LPCWSTR ERR_TEXT[] = {  
	_T("id3v1-tag missing! must be present before you can save a lyrics-tag!"), 
	_T("the language id must be 3 bytes long!"), 
	_T(""), // _T("too much frames!"), 
	_T("tag doesn't exist!"), 
	_T(""),//_T("wma tag writing not supported in this version!"), 
	_T(""),//_T("mp4 tag writing not supported in this version!"), 
	_T("id is out of range!"), 
	_T("parser Error: wrong format for id3v2 sync lyrics!"), 
	_T("tag not allowed in this audio format!"), 
	_T("error while parsing wma file! object size is wrong."), 
	_T(""),//_T("the picture tag is greater then 65535 bytes! it must be less than 65536 bytes!"), 
	_T(""),//_T("wav tag writing not supported in this version!"),
	_T("array must be minimum 4 bytes long!"),
	_T("id3v2 image uri '%s' not found!"),
	_T("wma parse error! the file seems to be corrupt!"),
	_T("specific frame not found"),
	_T("invalid file name"),
	_T("invalid frame format"),
	_T("not enough memory for allocating %u bytes"),
	_T("frame is too big "),
	_T("atom data is unknown format"),
	_T("id3v2 compressed frames are not supported"),
	_T("chapter frame corrupt, framesize is bigger than id3v2 size"),
	_T("utf-8 is not allowed in this id3v2 format!"),
	_T("id3v2 frame '%c%c%c%c' at pos %u is corrupt, framesize is bigger than id3v2 size"),
	_T("text can not convert into %s, use an empty string instead"),
	_T("no data chunk found; this is not a valid wav file or you forgot to analyze the file first")
};

static const CAtlString MIME_LINK = _T("-->");

// public interfaces and 32 bit formats (WAV, MP4) clamp 64 bit values instead of truncating them
inline long toLongClamped(__int64 value)
{
	if (value > 0x7FFFFFFFLL)
		return 0x7FFFFFFF;
	if (value < -0x7FFFFFFFLL - 1)
		return (-0x7FFFFFFF - 1);
	return (long)value;
}
inline unsigned int toU32Clamped(__int64 value)
{
	if (value < 0)
		return 0;
	if (value > 0xFFFFFFFFLL)
		return 0xFFFFFFFFu;
	return (unsigned int)value;
}
inline size_t toSizeClamped(__int64 value)
{
#ifdef _WIN64
	return (value < 0) ? 0 : (size_t)value;
#else
	return (size_t)toU32Clamped(value);
#endif
}

enum CONFIGKEYS {
	CONFIG_MPEGEXACTREAD = 0,
	CONFIG_ID3V2PADDINGSIZE,
	CONFIG_ID3V2WRITEBLOCKSIZE,
	CONFIG_DOEVENTSMILLIS,
	CONFIG_MAXTEXTBUFFER,
	CONFIG_WMAPADDINGSIZE,
	CONFIG_MP4PADDINGSIZE
};

#define MAX_CONFIG_VALUES 7

class CTools
{
public:
	static CTools &instance();
	// closes both files, checks for write errors and only then replaces the original with the new file
	// copies count bytes block by block (count < 0: until the end of the file). Returns false on a read or write error
	static bool copyStream(FILE *source, FILE *destination, __int64 count);
	static bool finishRewrite(FILE *source, FILE *destination, LPCWSTR newFileName, LPCWSTR origFileName);
	static __int64 FileSize;
	static int ID3v1Size;
	static long ID3v2Size;
	static int LyricsSize;
	static int APESize;
	static __int64 firstMpegAudioPos;
	static long configValues[MAX_CONFIG_VALUES];
	static wchar_t* wcTextPuffer;
	static char* cTextPuffer;
	static void doEvents();
	static void doEventsNow();
	static void setLogFile(LPCWSTR file);
	static void writeInfo(LPCWSTR entry, ...);
	static void writeDebug(LPCWSTR entry, ...);
	static void writeError(LPCWSTR entry, ...);
	static void writeWarning(LPCWSTR entry, ...);
	static void setLastError(int error, ...);
	void reset();
	static CAtlString ExtractMimeFromPicture(const BYTE *buf);
	static CAtlString ExtractSmallMimeFromPicture(const BYTE *buf);
	static int CalcMimeFromPicture(const BYTE *buf);
	static CAtlString GetLastErrorText() { return lastErrorText; };
	static int getLastError() { return lastError; };
	static BSTR GetEmptyBSTR() { return CAtlString().AllocSysString(); };
	static void setConfigValue(long key, long value);
	static long getConfigValue(long key);
	static BYTE ID3V2oldTagVersion, ID3V2newTagVersion, ID3V2defaultEncodingID, ID3V2Flags;
protected:
	CTools(void);
	virtual ~CTools(void);	
private:
	CTools(const CTools &);
	CTools &operator=(const CTools &);
	static void write(LPCWSTR art, LPCWSTR entry);
	static int lastError;
	static CAtlString lastErrorText;
	static DWORD oldTickCount;
	static FILE* log;
	static CAtlString logFile;
	static MSG msg;
	static SYSTEMTIME stTime; //To contain the date/time
	static CAtlString logOutput;
	static CBlob output;
};