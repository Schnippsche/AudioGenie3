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

#include "StdAfx.h"
#include "Tools.h"
#include "share.h"
#include <io.h>
#include <memory>
#include <new>

int CTools::lastError;
__int64 CTools::FileSize;
FILE *CTools::analysisStream = NULL;
static std::unique_ptr<BYTE[]> g_headBuffer;   // the cache of the start of the file
static size_t g_headCapacity = 0;
size_t CTools::headCacheLength = 0;
bool CTools::streamAtStart = false;
bool CTools::streamAtHeadEnd = false;
BYTE CTools::tailCache[CTools::TAIL_CACHE_SIZE];
size_t CTools::tailCacheLength = 0;

// makes room for count bytes in the cache of the start of the file, the bytes that are in it stay
static bool reserveHead(size_t count)
{
	if (count <= g_headCapacity)
		return true;
	std::unique_ptr<BYTE[]> bigger(new (std::nothrow) BYTE[count]);
	if (!bigger)
		return false;
	if (CTools::headCacheLength != 0)
		memcpy(bigger.get(), g_headBuffer.get(), CTools::headCacheLength);
	g_headBuffer = std::move(bigger);
	g_headCapacity = count;
	return true;
}

size_t CTools::readAt(FILE *Stream, __int64 pos, void *destination, size_t length)
{
	if (Stream == NULL || length == 0)
		return 0;
	if (Stream == analysisStream && pos >= 0)
	{
		// the start of the file: the first HEAD_CACHE_SIZE bytes (the whole file if it is small), or what extendHeadCache has read
		const __int64 headEnd = headCacheLength != 0 ? (__int64)headCacheLength : (FileSize < (__int64)HEAD_CACHE_SIZE ? FileSize : (__int64)HEAD_CACHE_SIZE);
		if (pos + (__int64)length <= headEnd)
		{
			if (headCacheLength == 0 && reserveHead((size_t)headEnd))
			{
				const size_t count = (size_t)headEnd;
				// a stream that was just opened is at the start of the file: no seek
				if ((streamAtStart || _fseeki64(Stream, 0, SEEK_SET) == 0) && fread(g_headBuffer.get(), 1, count, Stream) == count)
				{
					headCacheLength = count;
					streamAtHeadEnd = true;
				}
				streamAtStart = false;
			}
			if (headCacheLength != 0)
			{
				memcpy(destination, g_headBuffer.get() + (size_t)pos, length);
				return length;
			}
		}
		// the end of the file
		if (length <= TAIL_CACHE_SIZE)
		{
			const __int64 cacheStart = FileSize > (__int64)TAIL_CACHE_SIZE ? FileSize - (__int64)TAIL_CACHE_SIZE : 0;
			if (pos >= cacheStart && pos + (__int64)length <= FileSize)
			{
				if (tailCacheLength == 0)
				{
					const size_t count = (size_t)(FileSize - cacheStart);
					streamAtStart = streamAtHeadEnd = false;
					if (_fseeki64(Stream, cacheStart, SEEK_SET) == 0 && fread(tailCache, 1, count, Stream) == count)
						tailCacheLength = count;
				}
				if (tailCacheLength != 0)
				{
					memcpy(destination, tailCache + (size_t)(pos - cacheStart), length);
					return length;
				}
			}
		}
	}
	streamAtStart = streamAtHeadEnd = false;
	if (_fseeki64(Stream, pos, SEEK_SET) != 0)
		return 0;
	return fread(destination, 1, length, Stream);
}

void CTools::extendHeadCache(FILE *Stream, __int64 end)
{
	if (Stream == NULL || Stream != analysisStream || headCacheLength == 0)
		return;   // only after the first read of the start
	if (end > FileSize)
		end = FileSize;
	if (end <= (__int64)headCacheLength || end > (__int64)HEAD_CACHE_MAX)
		return;
	if (!reserveHead((size_t)end))
		return;
	const size_t have = headCacheLength;
	const size_t count = (size_t)end - have;
	// directly behind the first read of the start the stream is at the right position
	const bool seek = !streamAtHeadEnd;
	streamAtStart = streamAtHeadEnd = false;
	if (seek && _fseeki64(Stream, (__int64)have, SEEK_SET) != 0)
		return;
	// One read of exactly count bytes: fread would split it into a read of a multiple of the buffer size (8 KB) and one more read of 8 KB for the
	// rest. The buffer of the stream is empty here (directly behind the first read of the start, or behind the seek), the position of the file is the
	// one of the stream, and every other read of the stream is done behind a seek.
	const int got = _read(_fileno(Stream), g_headBuffer.get() + have, (unsigned int)count);
	if (got > 0)
		headCacheLength = have + (size_t)got;
}

__int64 CTools::fileLength(FILE *Stream)
{
	if (Stream == NULL)
		return -1;
	if (Stream == analysisStream)
		return FileSize;
	LARGE_INTEGER size;
	if (!GetFileSizeEx((HANDLE)_get_osfhandle(_fileno(Stream)), &size))
		return -1;
	return size.QuadPart;
}
int CTools::ID3v1Size;
long CTools::ID3v2Size;
int CTools::LyricsSize;
int CTools::APESize;
int CTools::APEHeadSize;
__int64 CTools::firstMpegAudioPos;
long CTools::configValues[MAX_CONFIG_VALUES];
bool CTools::lossyText = false;
wchar_t *CTools::wcTextPuffer = 0;
char *CTools::cTextPuffer = 0;
CAtlString CTools::lastErrorText;
MSG CTools::msg;
CAtlString CTools::logFile;
FILE* CTools::log;
SYSTEMTIME CTools::stTime; //To contain the date/time
CAtlString CTools::logOutput;
CBlob CTools::output;
DWORD CTools::oldTickCount;
BYTE CTools::ID3V2oldTagVersion = 0;
BYTE CTools::ID3V2newTagVersion = 3; // TAG_VERSION_2_3
BYTE CTools::ID3V2defaultEncodingID = TEXT_ENCODED_ANSI;
BYTE CTools::ID3V2Flags = 0;

CTools::CTools(void)
{
	// the following initialization happens only once!
	oldTickCount = 0;
	logFile.Empty();
	log = NULL;
	configValues[CONFIG_MPEGEXACTREAD] = 0;
	configValues[CONFIG_ID3V2PADDINGSIZE] = 4096;
	configValues[CONFIG_ID3V2WRITEBLOCKSIZE] = 524288l; // default 0.5MB
	configValues[CONFIG_WMAPADDINGSIZE] = 4096;
	configValues[CONFIG_DOEVENTSMILLIS] = 250;
	configValues[CONFIG_MP4PADDINGSIZE] = 4096;
	configValues[CONFIG_ANSICODEPAGE] = 1252;         // Windows-1252, a superset of ISO-8859-1
	configValues[CONFIG_ID3V2LINKEDPICTURES] = 0;
	configValues[CONFIG_ID3V1MAXTEXTLENGTH] = 90;
	setConfigValue(CONFIG_MAXTEXTBUFFER, 0x40000l); // default 256 KB
}

CTools::~CTools(void)
{
	delete [] wcTextPuffer;
	delete [] cTextPuffer;
}

CTools &CTools::instance()
{
	static CTools factory;
	return factory;
}

void CTools::reset()
{
	FileSize = 0;
	ID3v1Size = 0;
	ID3v2Size = 0;
	LyricsSize = 0;
	APESize = 0;
	APEHeadSize = 0;
	lastError = 0;
	lastErrorText.Empty();
	firstMpegAudioPos = 0;
}

void CTools::doEvents()
{
	// GetTickCount (not GetTickCount64, which needs Windows Vista; the minimum platform is Windows 2000) wraps around after 49 days, which is
	// harmless here: the difference of two DWORD values is correct across the wrap-around, and it is only compared with a few hundred milliseconds
#pragma warning(suppress: 28159)
	DWORD ticks = GetTickCount();
	if ((DWORD)(ticks - oldTickCount) < (DWORD)CTools::configValues[CONFIG_DOEVENTSMILLIS]) // only every eventMillis milliseconds
		return;
	oldTickCount = ticks;
	doEventsNow();
}
// Lets the host process the messages of its thread (user interface, progress bar) while the library works on a long operation, like DoEvents
// of Visual Basic: all messages that are waiting are dispatched, at most MAX_MESSAGES, so that a flood of messages (mouse moves) cannot hold up
// the operation. A WM_QUIT is not for the library: it is posted again for the message loop of the host, which would otherwise never see it
// (the host would keep running after it asked to quit).
void CTools::doEventsNow()
{
	const int MAX_MESSAGES = 100;
	// a message handler of the host can call the library again: that inner call does not process messages itself
	static bool processing = false;
	if (processing)
		return;
	processing = true;
	MSG message;   // local: the handlers can use the library again, which would overwrite a member
	for (int i = 0; i < MAX_MESSAGES && PeekMessageW(&message, (HWND) NULL, 0, 0, PM_REMOVE); i++)
	{
		if (message.message == WM_QUIT)
		{
			PostQuitMessage((int)message.wParam);
			break;
		}
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
	processing = false;
}

void CTools::setLastError(int error, ...)
{
	lastError = error;
	if (error == 0)
		return;
	if (error > 200 && error < 250 && (size_t)(error - 201) < _countof(ERR_TEXT)) // user-defined error text
	{
		va_list vlist;
		va_start(vlist, error);
		_vsnwprintf_s(wcTextPuffer, 16000, 15000, ERR_TEXT[error - 201], vlist);	
		lastErrorText = wcTextPuffer;
		va_end(vlist);
	}
	else // system error
	{
		_wcserror_s(wcTextPuffer, 16000, lastError);  
	}
	lastErrorText = wcTextPuffer;
	writeError(L"%s", (LPCWSTR)lastErrorText);
}

bool CTools::copyStream(FILE *source, FILE *destination, __int64 count)
{
	size_t blockSize = (size_t)configValues[CONFIG_ID3V2WRITEBLOCKSIZE];
	if (blockSize < 4096)
		blockSize = 4096;
	CBlob tmp(blockSize);
	while (count != 0)
	{
		size_t want = (count < 0 || count > (__int64)blockSize) ? blockSize : (size_t)count;
		tmp.FileRead(want, source);
		size_t got = tmp.GetLength();
		if (got > 0 && tmp.FileWrite(got, destination) != got)
			return false; // write error
		instance().doEvents();
		if (count > 0)
			count -= (__int64)got;
		if (got < want)
			return (count < 0) ? (ferror(source) == 0) : false; // end of file (with count > 0: source too short)
	}
	return true;
}

bool CTools::rewriteRegion(LPCWSTR FileName, __int64 offset, __int64 oldLength, CBlob *data)
{
	FILE *Source;
	FILE *Destination;
	CAtlString NewFileName(FileName);
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) == NULL)
	{
		instance().setLastError(errno);
		return false;
	}
	NewFileName += TILDE;
	if ( (Destination = _wfsopen(NewFileName, READ_AND_WRITENEW, _SH_DENYWR)) == NULL)
	{
		instance().setLastError(errno);
		fclose(Source);
		return false;
	}
	bool ok = copyStream(Source, Destination, offset);
	if (ok && data != NULL && data->GetLength() > 0)
		ok = (data->FileWrite(data->GetLength(), Destination) == data->GetLength());
	if (ok)
		ok = (_fseeki64(Source, offset + oldLength, SEEK_SET) == 0) && copyStream(Source, Destination, -1);
	if (!ok)
	{
		fclose(Destination);
		fclose(Source);
		_wremove(NewFileName);
		instance().setLastError(EIO);
		return false;
	}
	return finishRewrite(Source, Destination, NewFileName, FileName);
}

bool CTools::finishRewrite(FILE *source, FILE *destination, LPCWSTR newFileName, LPCWSTR origFileName)
{
	bool ok = true;
	int err = 0;
	if (destination != NULL)
	{
		if (ferror(destination) != 0 || fflush(destination) != 0)
		{
			ok = false;
			err = errno;
		}
		if (fclose(destination) != 0)
		{
			ok = false;
			if (err == 0)
				err = errno;
		}
	}
	if (source != NULL)
	{
		if (ferror(source) != 0)
			ok = false;
		fclose(source);
	}
	if (!ok)
	{
		// the original stays unchanged, only the temporary file is removed
		_wremove(newFileName);
		instance().setLastError(err != 0 ? err : EIO);
		return false;
	}
	instance().doEventsNow();
	// replace the old file with the new file in one step
	if (!MoveFileExW(newFileName, origFileName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		_wremove(newFileName);
		instance().setLastError(EACCES);
		return false;
	}
	return true;
}

void CTools::setLogFile(LPCWSTR file)
{
	logFile = file;
}

void CTools::writeInfo(LPCWSTR entry, ...)
{
	if (logFile.IsEmpty())
		return;
	va_list vlist;
	va_start(vlist, entry);
	_vsnwprintf_s(wcTextPuffer, 16000, 15000, entry, vlist);	
	write( _T("INFO"), wcTextPuffer);
	va_end(vlist);
}

void CTools::writeError(LPCWSTR entry, ...)
{
	if (logFile.IsEmpty())
		return;
	va_list vlist;
	va_start(vlist, entry);
	_vsnwprintf_s(wcTextPuffer, 16000, 15000, entry, vlist);	
	write( _T("ERROR"), wcTextPuffer);
	va_end(vlist);
}

void CTools::writeWarning(LPCWSTR entry, ...)
{
	if (logFile.IsEmpty())
		return;
	va_list vlist;
	va_start(vlist, entry);
	_vsnwprintf_s(wcTextPuffer, 16000, 15000, entry, vlist);
	write( _T("WARNING"), wcTextPuffer);
	va_end(vlist);
}

void CTools::writeDebug(LPCWSTR entry, ...)
{
	entry;
#ifdef _DEBUG
	va_list vlist;
	va_start(vlist, entry);
	_vsnwprintf_s(wcTextPuffer, 16000, 15000, entry, vlist);
	write( _T("DEBUG"), wcTextPuffer);
	va_end(vlist);	
#endif
}


void CTools::write(LPCWSTR art, LPCWSTR entry)
{
	if (!logFile.IsEmpty())
	{
		log = _wfsopen(logFile, _T("ab"), _SH_DENYNO);
		if (log != NULL)
		{
			GetLocalTime(&stTime);
			output.Clear();		
			logOutput.Format(_T("%4i-%02i-%02i %02i:%02i:%02i.%04i %s: %s\r\n"), stTime.wYear, stTime.wMonth,
				stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, art, entry);
			output.AddEncodedString(TEXT_ENCODED_ANSI, logOutput, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
			output.FileWrite(output.GetLength(), log);
			fflush(log);
			fclose(log);
		}	
	}
}

CAtlString CTools::ExtractMimeFromPicture(const BYTE *buf)
{
	return IMAGE_LONG[CalcMimeFromPicture(buf)];
}

CAtlString CTools::ExtractSmallMimeFromPicture(const BYTE *buf)
{
	return IMAGE_SHORT[CalcMimeFromPicture(buf)];	
}

int CTools::CalcMimeFromPicture(const BYTE *buf)
{
	if (buf == NULL)
		return IMAGE_UNKNOWN; 
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
		return IMAGE_JPG; 
	if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
		return IMAGE_GIF; 
	if (buf[0] == 0x89 && buf[1] == 0x50 && buf[2] == 0x4E && buf[3] == 0x47)
		return IMAGE_PNG; 
	if (buf[0] == 'B' && buf[1] == 'M')
		return IMAGE_BMP; 
	if (buf[0] == 0x49 && buf[1] == 0x49 && buf[2] == 0x2A && buf[3] == 0x00)
		return IMAGE_TIFF;   // little endian
	if (buf[0] == 0x4D && buf[1] == 0x4D && buf[2] == 0x00 && buf[3] == 0x2A)
		return IMAGE_TIFF;   // big endian
	if (buf[0] == '-' && buf[1] == '-' && buf[2] == '>')
		return IMAGE_LINK; 
	return IMAGE_UNKNOWN;
}

void CTools::setConfigValue(long key, long value)
{
	if (key == CONFIG_MAXTEXTBUFFER)
	{
		if (value > 0x1000000l)  // max. 16 M characters
			value = 0x1000000l;
		int newValue = ((int)value / 2) * 2;
		if (newValue < 32768)
			newValue = 32768;
		wchar_t *newWc = new (std::nothrow) wchar_t[newValue + 2];
		char *newC = new (std::nothrow) char[newValue + 2];
		const bool failedWc = (newWc == NULL);
		const bool failedC = (newC == NULL);
		if (failedWc || failedC)
		{
			delete [] newWc;
			delete [] newC;
			return; // keep the old buffers
		}
		delete [] wcTextPuffer;
		delete [] cTextPuffer;
		wcTextPuffer = newWc;
		cTextPuffer = newC;
		configValues[key] = newValue;
	}
	else if (key >= 0 && key < MAX_CONFIG_VALUES)
	{
		// block size must be > 0, otherwise the copy loops run forever
		if (key == CONFIG_ID3V2WRITEBLOCKSIZE)
		{
			if (value < 4096)
				value = 4096;
			else if (value > 0x4000000l)
				value = 0x4000000l;
		}
		if (key == CONFIG_ANSICODEPAGE)
		{
			// 0 = code page of the system, otherwise an installed single byte code page (no Unicode code pages)
			if (value != 0 && (value < 0 || !IsValidCodePage((UINT)value) || value == 1200 || value == 1201 || value == 12000 || value == 12001 || value == 65000 || value == 65001))
				return;
		}
		if (key == CONFIG_ID3V2LINKEDPICTURES)
			value = (value != 0) ? 1 : 0;
		// the id3v1 tag has 30 characters, the enhanced tag adds 60 more
		if (key == CONFIG_ID3V1MAXTEXTLENGTH)
			value = (value < 30) ? 30 : ((value > 90) ? 90 : value);
		configValues[key] = value;
	}
}
long CTools::getConfigValue(long key)
{
	return (key >= 0 && key < MAX_CONFIG_VALUES) ? configValues[key] : 0;
}