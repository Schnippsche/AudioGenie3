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


/* Blob.h          for use with String-like BYTE-Fields                       */
/* by Stefan Toengi (c) 2003                                                  */
#pragma once

class CFile;
#define BLOCKSIZE 256
static const LPCWSTR EMPTY(_T(""));
static const LPCWSTR UNKNOWN(_T("Unknown"));
static const LPCWSTR MONO(_T("Mono"));
static const LPCWSTR STEREO(_T("Stereo"));
static const LPCWSTR JOINTSTEREO(_T("Joint Stereo"));
static const LPCWSTR DUALCHANNEL(_T("Dual Channel"));
static const LPCWSTR MULTICHANNEL(_T("Multi Channel"));

// the encoding of a text (the first byte of most ID3v2 text frames; the other tags use the ones they need)
enum class TextEncoding : BYTE
{
	Ansi = 0,       // ISO-8859-1 or ANSI
	Utf16Bom = 1,   // UTF-16 with BOM
	Utf16 = 2,      // UTF-16BE without BOM (encoding $02 of ID3v2.4)
	Utf8 = 3,       // UTF-8
	Utf16LE = 4,    // internal: UTF-16 little endian without BOM (WMA, SYLT), not an ID3v2 encoding
	Unset = 255     // no encoding chosen yet
};
inline TextEncoding textEncodingOf(BYTE value) { return static_cast<TextEncoding>(value); }   // from the byte of a file or of the API
inline BYTE encodingByte(TextEncoding encoding) { return static_cast<BYTE>(encoding); }       // the byte for a file or the API
static const TextEncoding TEXT_ENCODED_ANSI = TextEncoding::Ansi;
static const TextEncoding TEXT_ENCODED_UTF16BOM = TextEncoding::Utf16Bom;
static const TextEncoding TEXT_ENCODED_UTF16 = TextEncoding::Utf16;
static const TextEncoding TEXT_ENCODED_UTF16LE = TextEncoding::Utf16LE;
static const TextEncoding TEXT_ENCODED_UTF8 = TextEncoding::Utf8;
// AddEncodedString: is the byte of the encoding written in front of the text, are the terminating zero bytes written behind it
enum class EncodingByte { Without, With };
enum class NullBytes { Without, With };
static const EncodingByte TEXT_WITH_ENCODING = EncodingByte::With;
static const EncodingByte TEXT_WITHOUT_ENCODING = EncodingByte::Without;
static const NullBytes TEXT_WITH_NULLBYTES = NullBytes::With;
static const NullBytes TEXT_WITHOUT_NULLBYTES = NullBytes::Without;

class CBlob
{
private:
	size_t m_CurrentLength;
	size_t m_BufferSize;
	void Free();
	bool AllocNewBuffer(size_t nLen);
	// functions for ANSI characters
	void AssignCopy(size_t nSrcLen, LPCSTR lpszSrcData);
	void ConcatInPlace(size_t nSrcLen, LPCSTR lpszSrcData);
	// functions for Unicode characters
	void AssignCopy(size_t nSrcLen, LPCWSTR lpszSrcData);
	void ConcatInPlace(size_t nSrcLen, LPCWSTR lpszSrcData);
	bool GrowBuffer(size_t newLen);
public:
	BYTE *m_pData;
	CBlob();
	CBlob(size_t pufferSize);
	CBlob(const CBlob& src);
	virtual ~CBlob();
	const CBlob& operator=(const CBlob& stringSrc);
	// ANSI methods
	void AddString(const LPCSTR string);
	// Unicode methods
	void AddString(const LPCWSTR string);
	// general methods
	void AddMemory(const void *src, size_t nLen);
	// replaces the content with the nLen bytes (at most) at the position pos of the file, see CTools::readAt()
	void FileReadAt(CFile *Stream, __int64 pos, size_t nLen);
	void Add2B(int value);
	void Add3B(int value);
	void Add4B(unsigned int value);
	void AddR8B(__int64 value);
	void AddR4B(unsigned int value);
	void AddR2B(int value);
	void AddS4B(int value);
	void AddValue(BYTE ch);
	void AddValue(BYTE ch, size_t nRepeat);
	void AddNullByte();
	void AddBlob(const CBlob& blob, size_t start = 0);
	void AddFile(size_t nLen, CFile *Stream);
	BYTE GetAt(size_t nIndex);
	long Get4B(size_t nIndex);
	long GetR4B(size_t nIndex);
	unsigned __int16 Get2B(size_t nIndex);
	unsigned __int16 GetR2B(size_t nIndex);
	__int64 GetR8B(size_t nIndex);
	long Get3B(size_t nIndex);
	long GetS4B(size_t nIndex);
	unsigned __int16 GetS2B(size_t nIndex);
	
	CAtlString GetStringAt(size_t nPos, size_t nLength);
	void Clear() { Free(); };
	size_t GetLength() { return m_CurrentLength; };
	void FileRead(size_t nLen, CFile *Stream);
	size_t FileWrite(size_t nLen, CFile *Stream);
	bool isEmpty()    { return (m_CurrentLength == 0); };
	bool isNotEmpty() { return (m_CurrentLength > 0); };
	CAtlString ConvertToUnicodeString(TextEncoding encoding);
	void AddFixedAnsiString(const CAtlString source, size_t maxLen);
	void AddEncodedString(TextEncoding encoding, const CAtlString source, EncodingByte withEncodingByte = EncodingByte::With, NullBytes withNullBytes = NullBytes::With);
	CAtlString getNextString(TextEncoding encoding, int& startPos);
};
