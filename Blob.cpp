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

#include "stdafx.h"
#include <new>
#include <io.h>
#include "Blob.h"
#include "Tools.h"

// upper limit for a single blob. Protects against size values from
// manipulated files and against overflows with a 32 bit size_t.
static const size_t BLOB_MAX_SIZE = 0x40000000; // 1 GB
// From this size on, a requested read size is checked against the remaining size of the file
static const size_t BLOB_CHECK_FILE_LIMIT = 65536;

static inline bool blobSizeOverflow(size_t current, size_t add)
{
	return (add > BLOB_MAX_SIZE) || (current > BLOB_MAX_SIZE - add);
}

// limits nLen to the number of bytes that are left in the file from the current position
static size_t limitToFileRest(size_t nLen, FILE *Stream)
{
	if (nLen <= BLOB_CHECK_FILE_LIMIT || Stream == NULL)
		return nLen;
	__int64 pos = _ftelli64(Stream);
	if (pos < 0)
		return nLen;
	__int64 fileLen = _filelengthi64(_fileno(Stream));
	if (fileLen < 0)
		return nLen;
	unsigned __int64 rest = (fileLen > pos) ? (unsigned __int64)(fileLen - pos) : 0;
	if ((unsigned __int64)nLen > rest)
		return (size_t)rest;
	return nLen;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBlob::CBlob()
{
	m_CurrentLength = 0;
	m_BufferSize = 0;
	m_pData = NULL;
}

CBlob::CBlob(size_t pufferSize)
{
	m_CurrentLength = 0;
	m_BufferSize = 0;
	m_pData = NULL;
	AllocNewBuffer(pufferSize);
}

CBlob::CBlob(const CBlob& src)
{
	m_CurrentLength = 0;
	m_BufferSize = 0;
	m_pData = NULL;
	if (src.m_CurrentLength > 0 && src.m_pData != NULL)
		AssignCopy(src.m_CurrentLength, (LPCSTR) src.m_pData);
}

CBlob::~CBlob()
{
	Free();
}

void CBlob::Free()
{
	if (m_pData != NULL)
		delete [] m_pData;

	m_pData = NULL;
	m_CurrentLength = 0;
	m_BufferSize = 0;
}

bool CBlob::AllocNewBuffer(size_t nLen)
{
	if (nLen < m_BufferSize && m_pData != NULL)
		return true;
	// free the old memory
	Free();
	if (nLen > BLOB_MAX_SIZE)
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, nLen);
		return false;
	}
	// allocate new memory + reserve for null bytes
	size_t newSize = (((nLen + 4) / BLOCKSIZE) + 1) * BLOCKSIZE;
	BYTE *p = new (std::nothrow) BYTE[newSize + 8];
	if (p == NULL)
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, nLen);
		return false;
	}
	m_pData = p;
	m_BufferSize = newSize;
	m_CurrentLength = 0;
	return true;
}

void CBlob::AssignCopy(size_t nSrcLen, LPCSTR lpszSrcData)
{
	if (!AllocNewBuffer(nSrcLen))
		return;
	if (nSrcLen > 0 && lpszSrcData != NULL)
		memcpy(m_pData, lpszSrcData, nSrcLen);
	m_CurrentLength = nSrcLen;
}

void CBlob::AssignCopy(size_t nSrcLen, LPCWSTR lpszSrcData)
{
	if (nSrcLen > BLOB_MAX_SIZE / 2)
		return;
	if (!AllocNewBuffer(nSrcLen * 2))
		return;
	if (nSrcLen > 0 && lpszSrcData != NULL)
		memcpy(m_pData, lpszSrcData, nSrcLen * 2);
	m_CurrentLength = nSrcLen * 2;
}


const CBlob& CBlob::operator=(const CBlob& stringSrc)
{
	if (this != &stringSrc)
	{
		if (stringSrc.m_CurrentLength == 0 || stringSrc.m_pData == NULL)
			m_CurrentLength = 0;
		else
			AssignCopy(stringSrc.m_CurrentLength, (LPCSTR) stringSrc.m_pData);
	}
	return *this;
}

void CBlob::ConcatInPlace(size_t nSrcLen, LPCSTR lpszSrcData)
{
	// concatenating an empty string is a no-op!
	if (nSrcLen == 0 || lpszSrcData == NULL)
		return;
	if (blobSizeOverflow(m_CurrentLength, nSrcLen))
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, nSrcLen);
		return;
	}
	size_t nNewLen = m_CurrentLength + nSrcLen;
	if (!GrowBuffer(nNewLen))
		return;
	memcpy(m_pData + m_CurrentLength, lpszSrcData, nSrcLen);
	m_CurrentLength += nSrcLen;
}

// nSrcLen is the number of BYTES here, as in all callers
void CBlob::ConcatInPlace(size_t nSrcLen, LPCWSTR lpszSrcData)
{
	ConcatInPlace(nSrcLen, (LPCSTR)lpszSrcData);
}

// returns true if nBytes starting at nIndex lie inside the blob
#define BLOB_IN_RANGE(nIndex, nBytes) (m_pData != NULL && (nIndex) <= m_CurrentLength && (nBytes) <= m_CurrentLength - (nIndex))

BYTE CBlob::GetAt(size_t nIndex)
{
	if (!BLOB_IN_RANGE(nIndex, 1))
		return 0;
	return m_pData[nIndex];
}

CAtlString CBlob::GetStringAt(size_t nPos, size_t nLength)
{
	if (m_CurrentLength == 0 || m_pData == NULL || nPos >= m_CurrentLength)
		return EMPTY;
	if (nLength > m_CurrentLength - nPos)
		nLength = m_CurrentLength - nPos;
	CAtlString tmp((LPCSTR) (m_pData + nPos), (int)nLength);
	return tmp;
}

long CBlob::Get4B(size_t nIndex) // MSB Big Endian
{
	if (!BLOB_IN_RANGE(nIndex, 4))
		return 0;
	return (long)(((unsigned long)m_pData[nIndex] << 24) | ((unsigned long)m_pData[nIndex + 1] << 16) | ((unsigned long)m_pData[nIndex + 2] << 8) | m_pData[nIndex + 3]);
}

long CBlob::GetR4B(size_t nIndex) // LSB little Endian
{
	if (!BLOB_IN_RANGE(nIndex, 4))
		return 0;
	long value;
	memcpy(&value, m_pData + nIndex, 4);
	return value;
}

__int64 CBlob::GetR8B(size_t nIndex)
{
	if (!BLOB_IN_RANGE(nIndex, 8))
		return 0;
	__int64 value;
	memcpy(&value, m_pData + nIndex, 8);
	return value;
}

long CBlob::GetS4B(size_t nIndex)
{
	if (!BLOB_IN_RANGE(nIndex, 4))
		return 0;
	return (long)(((unsigned long)m_pData[nIndex] << 21) + ((unsigned long)m_pData[nIndex + 1] << 14) + ((unsigned long)m_pData[nIndex + 2] << 7) + m_pData[nIndex + 3]);
}

unsigned __int16 CBlob::Get2B(size_t nIndex)
{
	if (!BLOB_IN_RANGE(nIndex, 2))
		return 0;
	return (unsigned __int16)((m_pData[nIndex] << 8) + m_pData[nIndex + 1]);
}

unsigned __int16 CBlob::GetR2B(size_t nIndex)
{
	if (!BLOB_IN_RANGE(nIndex, 2))
		return 0;
	unsigned __int16 value;
	memcpy(&value, m_pData + nIndex, 2);
	return value;
}

unsigned __int16 CBlob::GetS2B(size_t nIndex)
{
	if (!BLOB_IN_RANGE(nIndex, 2))
		return 0;
	return (unsigned __int16)((m_pData[nIndex] << 7) + m_pData[nIndex + 1]);
}

long CBlob::Get3B(size_t nIndex)
{
	if (!BLOB_IN_RANGE(nIndex, 3))
		return 0;
	return (m_pData[nIndex] << 16) + (m_pData[nIndex + 1] << 8) + m_pData[nIndex + 2];
}

void CBlob::FileRead(size_t nLen, FILE *Stream)
{
	if (nLen < 1 || Stream == NULL)
	{
		m_CurrentLength = 0;
		return;
	}
	nLen = limitToFileRest(nLen, Stream);
	if (nLen < 1)
	{
		m_CurrentLength = 0;
		return;
	}
	if (!AllocNewBuffer(nLen))
	{
		m_CurrentLength = 0;
		return;
	}
	m_CurrentLength = fread(m_pData, 1, nLen, Stream);
}

size_t CBlob::FileWrite(size_t nLen, FILE *Stream)
{
	if (nLen > m_CurrentLength)
		nLen = m_CurrentLength;
	if (nLen == 0 || Stream == NULL || m_pData == NULL)
		return 0;
	return fwrite(m_pData, 1, nLen, Stream);
}

void CBlob::AddFile(size_t nLen, FILE* Stream)
{
	if (nLen < 1 || Stream == NULL)
		return;
	nLen = limitToFileRest(nLen, Stream);
	if (nLen < 1 || blobSizeOverflow(m_CurrentLength, nLen))
		return;
	if (!GrowBuffer(m_CurrentLength + nLen))
		return;
	// fast reading
	size_t tmpLen = fread(&m_pData[m_CurrentLength], 1, nLen, Stream);
	m_CurrentLength += tmpLen;
}

void CBlob::AddMemory(const void* src, size_t nLen)
{
	ConcatInPlace(nLen, (LPCSTR)src);
}

void CBlob::Add2B(int Value)
{
	if (!GrowBuffer(m_CurrentLength + 2))
		return;
	m_pData[m_CurrentLength++] = (Value >> 8) & 0xFF;
	m_pData[m_CurrentLength++] = Value & 0xFF;
}

void CBlob::AddR2B(int Value)
{
	if (!GrowBuffer(m_CurrentLength + 2))
		return;
	memcpy(m_pData + m_CurrentLength, &Value, 2);
	m_CurrentLength+=2;
}

void CBlob::Add3B(int Value)
{
	if (!GrowBuffer(m_CurrentLength + 3))
		return;
	m_pData[m_CurrentLength++] = (Value >> 16) & 0xFF;
	m_pData[m_CurrentLength++] = (Value >> 8) & 0xFF;
	m_pData[m_CurrentLength++] = Value & 0xFF;
}

void CBlob::Add4B(unsigned int Value)
{
	if (!GrowBuffer(m_CurrentLength + 4))
		return;
	m_pData[m_CurrentLength++] = (Value >> 24) & 0xFF;
	m_pData[m_CurrentLength++] = (Value >> 16) & 0xFF;
	m_pData[m_CurrentLength++] = (Value >> 8) & 0xFF;
	m_pData[m_CurrentLength++] = Value & 0xFF;
}

void CBlob::AddR4B(unsigned int Value)
{
	if (!GrowBuffer(m_CurrentLength + 4))
		return;
	memcpy(m_pData + m_CurrentLength, &Value, 4);
	m_CurrentLength+=4;
}

void CBlob::AddR8B(__int64 Value)
{
	if (!GrowBuffer(m_CurrentLength + 8))
		return;
	memcpy(m_pData + m_CurrentLength, &Value, 8);
	m_CurrentLength+=8;
}

void CBlob::AddS4B(int Value)
{
	if (!GrowBuffer(m_CurrentLength + 4))
		return;
	m_pData[m_CurrentLength++] = (Value >> 21) & 0x7F;
	m_pData[m_CurrentLength++] = (Value >> 14) & 0x7F;
	m_pData[m_CurrentLength++] = (Value >> 7) & 0x7F;
	m_pData[m_CurrentLength++] = Value & 0x7F;
}

void CBlob::AddValue(BYTE ch)
{
	AddValue(ch, 1);
}

void CBlob::AddNullByte()
{
	AddValue(0, 1);
}

void CBlob::AddValue(BYTE ch, size_t nRepeat)
{
	if (nRepeat == 0)
		return;
	if (blobSizeOverflow(m_CurrentLength, nRepeat))
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, nRepeat);
		return;
	}
	if (!GrowBuffer(m_CurrentLength + nRepeat))
		return;
	memset(&m_pData[m_CurrentLength], ch, nRepeat);
	m_CurrentLength += nRepeat;
}

bool CBlob::GrowBuffer(size_t newLen)
{
	// Buffer is big enough ?
	if (newLen < m_BufferSize && m_pData != NULL)
		return true;
	if (newLen > BLOB_MAX_SIZE)
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, newLen);
		return false;
	}
	// grow geometrically (factor 1.5) so that many small appends do not become quadratically expensive
	size_t tmpSize = (size_t((newLen + 1) / BLOCKSIZE) + 1) * BLOCKSIZE;
	size_t bigger = m_BufferSize + m_BufferSize / 2;
	if (bigger > tmpSize && bigger <= BLOB_MAX_SIZE + BLOCKSIZE)
		tmpSize = bigger;
	BYTE* tmpData = new (std::nothrow) BYTE[tmpSize + 8];
	if (tmpData == NULL)
	{
		ATLTRACE(_T("Cannot allocate %u bytes...\n"), (unsigned)tmpSize);
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, tmpSize + 8);
		return false;
	}
	if (m_pData != NULL)
	{
		memcpy(tmpData, m_pData, m_CurrentLength);
		delete [] m_pData;
	}
	m_BufferSize = tmpSize;
	m_pData = tmpData;
	return true;
}

void CBlob::AddBlob(const CBlob& blob, size_t start)
{
	if (start > blob.m_CurrentLength || blob.m_pData == NULL)
		return;
	if (blob.m_CurrentLength > 0)
		ConcatInPlace(blob.m_CurrentLength - start, (LPCSTR) blob.m_pData + start);
}

void CBlob::AddString(const LPCSTR string)
{
	if (string == NULL)
		return;
	ConcatInPlace(strlen(string), string);
}

/* adds a Unicode string to the blob */
void CBlob::AddString(const LPCWSTR string)
{
	if (string == NULL)
		return;
	ConcatInPlace(wcslen(string) * 2, string);
}

CAtlString CBlob::getNextString(BYTE encoding, int& startPos)
{
	if (m_CurrentLength == 0 || m_pData == NULL || startPos < 0 || (size_t)startPos > m_CurrentLength)
		return EMPTY;
	// the reserve at the end of the buffer guarantees termination
	m_pData[m_CurrentLength] = 0;
	m_pData[m_CurrentLength + 1] = 0;

	// find next nullbyte
	size_t endPos = startPos;
	if (encoding == TEXT_ENCODED_UTF16BOM || encoding == TEXT_ENCODED_UTF16)
	{
		while (endPos < m_CurrentLength && (m_pData[endPos] + m_pData[endPos + 1] != 0) )
			endPos+=2;
		endPos+=2;
	}
	else
	{
		while (endPos < m_CurrentLength && m_pData[endPos] != 0)
			endPos++;
		endPos++;
	}
	// set end position to the next value
	if (endPos > m_CurrentLength)
		endPos = m_CurrentLength;
	size_t size = endPos - startPos;
	// size of the text buffer in wchar_t
	size_t maxBuffer = (size_t)CTools::instance().configValues[CONFIG_MAXTEXTBUFFER];
	CAtlString result;
	wchar_t *po = CTools::instance().wcTextPuffer;
	size_t n;
	switch (encoding) // Encoding ID
	{
	case TEXT_ENCODED_ANSI: // ANSI or ISO-8859-1
		if (MultiByteToWideChar(CP_ACP, 0, (const char*)(m_pData + startPos), -1, po, (int)maxBuffer) > 0)
			result = CAtlString(po);
		else
		{
			result.Empty();
			CTools::instance().setLastError(ERR_TEXTCONVERT, L"ANSI");
		}
		break;
	case TEXT_ENCODED_UTF16BOM: // UTF-16 with BOM
		if (size >= 2 && m_pData[startPos] == 0xFE && m_pData[startPos + 1] == 0xFF) // big endian, swap bytes
		{
			n = 0;
			for (size_t i = startPos + 2; i + 1 < endPos && n < maxBuffer - 1; i+=2)
				po[n++] = (wchar_t)((m_pData[i] << 8) + m_pData[i + 1]);
			po[n] = 0;
		}
		else if (size >= 2 && m_pData[startPos] == 0xFF && m_pData[startPos + 1] == 0xFE ) // little endian
		{
			n = (size - 2) / 2;
			if (n > maxBuffer - 1)
				n = maxBuffer - 1;
			memcpy(po, m_pData + startPos + 2, n * 2);
			po[n] = 0;
		}
		else // unknown BOM or corrupt
		{
			result.Empty();
			break;
		}
		result = CAtlString(po);
		break;
	case TEXT_ENCODED_UTF16: // UTF-16 without BOM
		n = size / 2;
		if (n > maxBuffer - 1)
			n = maxBuffer - 1;
		memcpy(po, m_pData + startPos, n * 2);
		po[n] = 0;
		result = CAtlString(po);
		break;
	case TEXT_ENCODED_UTF8:   // UTF-8
		if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char*)(m_pData + startPos), -1, po, (int)maxBuffer) > 0)
			result = CAtlString(po);
		else
		{
			CTools::instance().setLastError(ERR_TEXTCONVERT, L"UTF-8");
			result.Empty();
		}
		break;
	}
	startPos = (int)endPos;
	return result;
}

// converts a BYTE array of any format into a Unicode string
// only works with a UNICODE build
// encoding: specifies the encoding
// rest is data ( 0-terminated )
CAtlString CBlob::ConvertToUnicodeString( BYTE encoding)
{
	if (m_CurrentLength < 1)
		return EMPTY;
	int pos = 0;
	return getNextString(encoding, pos);
}

void CBlob::AddFixedAnsiString(const CAtlString source, size_t maxLen)
{
	CAtlString tmp = source.Left((int)maxLen);
	size_t nLen = tmp.GetLength();
	AddEncodedString(TEXT_ENCODED_ANSI, tmp, false, false);
	if (nLen < maxLen)
		AddValue(0,maxLen - nLen);
}

/**
adds a string depending on the encoding
**/
void CBlob::AddEncodedString(BYTE encoding, const CAtlString source, bool withEncodingByte, bool withNullBytes)
{
	if (withEncodingByte)
		AddValue(encoding);
	// if the string is empty, do not convert
	if (source.GetLength() == 0)
	{
		if (withNullBytes)
		{
			if (encoding == TEXT_ENCODED_UTF16BOM || encoding == TEXT_ENCODED_UTF16)
				AddValue(0, 2);
			else
				AddNullByte();
		}
		return;
	}

	// the buffer is not cleared beforehand: WideCharToMultiByte returns the exact length
	int maxBuffer = CTools::instance().getConfigValue(CONFIG_MAXTEXTBUFFER);
	char* buf = CTools::instance().cTextPuffer;
	size_t Size;

	switch (encoding) // Encoding ID
	{
	case TEXT_ENCODED_ANSI: // ANSI or ISO-8859-1
		Size = WideCharToMultiByte(CP_ACP,0, source, source.GetLength(), buf, maxBuffer, 0, 0);
		if (Size > 0)
			ConcatInPlace(Size, buf);
		else
			CTools::instance().setLastError(ERR_TEXTCONVERT, L"ANSI");
		if (withNullBytes)
			AddNullByte();
		break;
	case TEXT_ENCODED_UTF16BOM: // UTF-16 with BOM
		AddValue(0xFF);
		AddValue(0xFE);
		ConcatInPlace(wcslen(source) * 2, source);
		if (withNullBytes)
			AddValue(0, 2);
		break;
	case TEXT_ENCODED_UTF16: // UTF-16 without BOM
		ConcatInPlace(wcslen(source) * 2, source);
		if (withNullBytes)
			AddValue(0, 2);
		break;
	case TEXT_ENCODED_UTF8:   // UTF-8
		Size = WideCharToMultiByte(CP_UTF8,0, source, source.GetLength(), buf, maxBuffer, 0, 0);
		if (Size > 0)
			ConcatInPlace(Size, buf);
		else
			CTools::instance().setLastError(ERR_TEXTCONVERT, L"UTF-8");
		if (withNullBytes)
			AddNullByte();
		break;
	}
}
