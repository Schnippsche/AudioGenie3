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
#include "stdio.h"
#include "io.h"
#include "Blob.h"
#include "Tools.h"
#include "ape.h"
#include <fcntl.h>
#include <ctype.h>
#include "sys/stat.h"
#include "share.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAPE::CAPE()
{
	_items.SetCount(0, 20);
}
CAPE::~CAPE()
{
	ResetData();
}

/* -------------------------------------------------------------------------- */

void CAPE::ResetData()
{ /* Reset all variables */
	for (size_t i = 0; i < _items.GetCount(); i++)
		delete _items.GetAt(i);
	_items.RemoveAll();
	TagInfo.Reset();
	CTools::APESize  = 0;
	FVersion = 0;
	Data.Clear();
}

/* -------------------------------------------------------------------------- */

bool CAPE::ReadFooter(FILE *Stream)
{
	/* Read footer data */
	_fseeki64(Stream, - CTools::ID3v1Size - APE_TAG_FOOTER_SIZE, SEEK_END);
	return TagInfo.ReadFromFile(Stream);  
}

/* -------------------------------------------------------------------------- */

// APE item keys: 2 to 255 characters in the range $20 to $7E; ID3, TAG, OggS and MP+ are not allowed
static bool isValidKey(LPCWSTR key)
{
	const size_t length = (key == NULL) ? 0 : wcslen(key);
	if (length < 2 || length > 255)
		return false;
	for (size_t i = 0; i < length; i++)
		if (key[i] < 0x20 || key[i] > 0x7E)
			return false;
	static const LPCWSTR forbidden[] = { L"ID3", L"TAG", L"OggS", L"MP+" };
	for (int i = 0; i < 4; i++)
		if (_wcsicmp(key, forbidden[i]) == 0)
			return false;
	return true;
}

bool CAPE::SetTagItem(LPCWSTR FieldName, LPCWSTR Value)
{
	
	/* Set tag item if supported field found */
	for (size_t Iterator = 0; Iterator < _items.GetCount(); Iterator++)
	{
		item = _items[Iterator];
		if (item->Key.CompareNoCase(FieldName) == 0)
		{
			// If the value is empty, delete the key
			if (Value == 0 || wcslen(Value) == 0)
			{
				delete item;
				_items.RemoveAt(Iterator);
				return true;
			}
			item->Value.Clear();
			item->Flags = 0x00;
			if (TagInfo.Version > APE_VERSION_1_0)
				item->Value.AddEncodedString(TEXT_ENCODED_UTF8, Value, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
			else
				item->Value.AddEncodedString(TEXT_ENCODED_ANSI, Value, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
			return true;
		}
	}
	/* not found, make a new entry if new Value != blank */
	if (Value == 0 || wcslen(Value) == 0)
		return true;
	if (!isValidKey(FieldName))
	{
		CTools::instance().writeWarning(L"APE item '%s' ignored: a key has 2 to 255 characters from $20 to $7E and is not ID3, TAG, OggS or MP+", FieldName);
		return false;
	}

	item = new CApeTagItem();
	item->Key = FieldName;
	item->Value.Clear();
	item->Flags = 0x00;
	if (TagInfo.Version == 0)
		TagInfo.Version = APE_VERSION_2_0;

	if (TagInfo.Version > APE_VERSION_1_0)
		item->Value.AddEncodedString(TEXT_ENCODED_UTF8, Value, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	// TagItem[newEntry].Value.AddAsUTF8(Value);
	else
		item->Value.AddEncodedString(TEXT_ENCODED_ANSI, Value, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	_items.Add(item);
	return true;
}

/* -------------------------------------------------------------------------- */

CAtlString CAPE::GetTagItem(LPCWSTR FieldName)
{
	/* search all stored fields for key */
	for (size_t Iterator = 0; Iterator < _items.GetCount(); Iterator++)
	{
		item = _items.GetAt(Iterator);
		if ( item->Key.CompareNoCase(FieldName) == 0 && !item->isBinary() )
		{
			if (TagInfo.Version > APE_VERSION_1_0)
				return item->Value.ConvertToUnicodeString(TEXT_ENCODED_UTF8);
			else
				return item->Value.ConvertToUnicodeString(TEXT_ENCODED_ANSI);
		}
	}
	return EMPTY;
}

/* -------------------------------------------------------------------------- */

bool CAPE::ReadFields(FILE *Stream, __int64 headOffset)
{
	CAtlString FieldName;
	long Iterator;
	// the items of a tag at the beginning follow its header, the items of a tag at the end are counted back from the end of the file
	if (headOffset >= 0)
		_fseeki64(Stream, headOffset + APE_TAG_HEADER_SIZE, SEEK_SET);
	else
		_fseeki64(Stream, -CTools::ID3v1Size - TagInfo.Size, SEEK_END);
	/* Read all stored fields */
	for (Iterator = 0; Iterator < TagInfo.Fields; Iterator++)
	{
		CTools::instance().doEvents();
		item = new CApeTagItem();
		if (item->ReadFromFile(Stream) == false)
		{
			ResetData();
			return false;
		}
		_items.Add(item);
		if (headOffset >= 0 && _ftelli64(Stream) > headOffset + APE_TAG_HEADER_SIZE + TagInfo.Size)
		{
			ResetData();	// the items reach beyond the tag
			return false;
		}
	}
	return true;
}

/* -------------------------------------------------------------------------- */

bool CAPE::FindHeadTag(FILE *Stream, __int64 &offset, __int64 &length)
{
	BYTE id3[10];
	offset = 0;
	length = 0;
	const __int64 fileSize = _filelengthi64(_fileno(Stream));
	if (_fseeki64(Stream, 0, SEEK_SET) != 0)
		return false;
	// behind an ID3v2 tag: the size field is synchsafe, a footer (v2.4) has 10 bytes
	if (fread(id3, 1, 10, Stream) == 10 && id3[0] == 'I' && id3[1] == 'D' && id3[2] == '3' && id3[3] < 0xFF && id3[4] < 0xFF
		&& ((id3[6] | id3[7] | id3[8] | id3[9]) & 0x80) == 0)
	{
		offset = 10 + ((__int64)id3[6] << 21) + ((__int64)id3[7] << 14) + ((__int64)id3[8] << 7) + id3[9];
		if (id3[3] == 4 && (id3[5] & 0x10) != 0)
			offset += 10;
	}
	if (offset + APE_TAG_HEADER_SIZE > fileSize || _fseeki64(Stream, offset, SEEK_SET) != 0)
		return false;
	CBlob tmp;
	tmp.FileRead(APE_TAG_HEADER_SIZE, Stream);
	if (tmp.GetLength() != APE_TAG_HEADER_SIZE || memcmp(tmp.m_pData, APE_ID, 8) != 0)
		return false;
	const DWORD version = (DWORD)tmp.GetR4B(8);
	const DWORD size = (DWORD)tmp.GetR4B(12);
	const DWORD flags = (DWORD)tmp.GetR4B(20);
	// only a tag with a header (bit 29 of the flags) can be at the beginning; the size includes the footer, but not the header
	if (version < APE_VERSION_2_0 || (flags & 0x20000000u) == 0 || size < APE_TAG_FOOTER_SIZE || offset + APE_TAG_HEADER_SIZE + (__int64)size > fileSize
		|| (__int64)size + APE_TAG_HEADER_SIZE > 0x7FFFFFFF)
		return false;
	length = APE_TAG_HEADER_SIZE + (__int64)size;
	return true;
}

/* -------------------------------------------------------------------------- */

bool CAPE::ReadHeadTag(FILE *Stream, __int64 offset, __int64 length)
{
	_fseeki64(Stream, offset, SEEK_SET);
	if (!TagInfo.ReadFromFile(Stream))
	{
		ResetData();
		return false;
	}
	FVersion = TagInfo.Version;
	CTools::APEHeadSize = (int)length;
	return ReadFields(Stream, offset);
}

/* -------------------------------------------------------------------------- */

// writes the file again: the data before the region, the new data (may be NULL) and the data behind the old region
bool CAPE::RewriteRegion(LPCWSTR FileName, __int64 offset, __int64 oldLength, CBlob *data)
{
	FILE *Source;
	FILE *Destination;
	CAtlString NewFileName(FileName);
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	NewFileName += TILDE;
	if ( (Destination = _wfsopen(NewFileName, READ_AND_WRITENEW, _SH_DENYWR)) == NULL)
	{
		CTools::instance().setLastError(errno);
		fclose(Source);
		return false;
	}
	bool ok = CTools::copyStream(Source, Destination, offset);
	if (ok && data != NULL && data->GetLength() > 0)
		ok = (data->FileWrite(data->GetLength(), Destination) == data->GetLength());
	if (ok)
		ok = (_fseeki64(Source, offset + oldLength, SEEK_SET) == 0) && CTools::copyStream(Source, Destination, -1);
	if (!ok)
	{
		fclose(Destination);
		fclose(Source);
		_wremove(NewFileName);
		CTools::instance().setLastError(EIO);
		return false;
	}
	return CTools::finishRewrite(Source, Destination, NewFileName, FileName);
}

/* -------------------------------------------------------------------------- */

bool CAPE::TruncateFile(LPCWSTR FileName, int Offset)
{
	int fh;
	if( _wsopen_s(&fh,  FileName, _O_RDWR | _O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE )  == 0 )
	{
		long ln = _filelength(fh) - Offset;
		if (ln > 0)
			_chsize_s(fh, ln);
		_close( fh );
		return true;
	}
	CTools::instance().setLastError(errno);
	return false;
}

/* -------------------------------------------------------------------------- */

bool CAPE::AddToFile(LPCWSTR FileName)
{ /* Add tag data to file */
	FILE * Stream;
	if ( (Stream = _wfsopen(FileName, APPEND, _SH_DENYWR)) != NULL)
	{
		Data.FileWrite(Data.GetLength(), Stream);
		fflush(Stream);
		fclose(Stream);
		return true;
	}
	CTools::instance().setLastError(errno);
	return false;
}

/* -------------------------------------------------------------------------- */

void CAPE::BuildFooter()
{
	CBlob tmpBlob;
	size_t Iterator;
	/* Build tag footer */
	// a v1 tag stays a v1 tag (its values are ANSI), everything else is written as v2
	const long version = (TagInfo.Version == APE_VERSION_1_0 || FVersion == APE_VERSION_1_0) ? APE_VERSION_1_0 : APE_VERSION_2_0;
	TagInfo.Reset();
	TagInfo.Version = version;
	TagInfo.Size = APE_TAG_FOOTER_SIZE;
	for (Iterator = 0; Iterator < _items.GetCount(); Iterator++)
	{
		item = _items.GetAt(Iterator);
		TagInfo.Size+=(long)item->Value.GetLength();
		TagInfo.Size+=(long)item->Key.GetLength();
		TagInfo.Size+= 9; // 4 + 4 + 1		

	}
	TagInfo.Fields = (long)_items.GetCount();
}


/* -------------------------------------------------------------------------- */

void CAPE::BuildTagData()
{
	size_t Iterator;
	long ValueSize, Flags;
	/* Build tag header, fields and footer */
	Data.Clear();
	BuildFooter();
	// v2: header and footer; v1: footer only, all flags are zero
	const bool v2 = (TagInfo.Version > APE_VERSION_1_0);
	if (v2)
	{
		/* set flags to footer start */
		TagInfo.Flags = (0xA0 << 24);
		TagInfo.WriteToBlob(Data);
	}
	Flags = 0;
	CBlob tmpBlob;
	for (Iterator = 0; Iterator < _items.GetCount(); Iterator++)
	{
		item = _items.GetAt(Iterator);
		ValueSize = (long)item->Value.GetLength();
		Data.AddR4B(ValueSize);
		Data.AddR4B(item->Flags); // Flag = 0;
		Data.AddEncodedString(TEXT_ENCODED_ANSI, item->Key, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
		Data.AddValue(0);
		Data.AddBlob(item->Value);		
	}
	/* set flags to footer end */
	TagInfo.Flags = v2 ? (0x80 << 24) : 0;
	TagInfo.WriteToBlob(Data);
}

/* -------------------------------------------------------------------------- */

bool CAPE::SaveTag(LPCWSTR FileName)
{
	BuildTagData();
	/* Add created tag to file */
	bool result = AddToFile(FileName);
	// take an ID3v1 tag into account if present
	if (tmpid3v1.GetSize() > 0)
		tmpid3v1.SaveToFile(FileName);
	return result;
}

/* -------------------------------------------------------------------------- */

bool CAPE::ReadFromFile(FILE *Stream)
{
	// a tag at the beginning of the file has a header and is found first
	__int64 headOffset, headLength;
	if (FindHeadTag(Stream, headOffset, headLength))
		return ReadHeadTag(Stream, headOffset, headLength);
	/* Process data if loaded and footer is valid */
	if (ReadFooter(Stream))
	{
		CTools::APESize = TagInfo.Size;
		FVersion = TagInfo.Version;
		/* Get information from fields */
		return ReadFields(Stream, -1);
	}
	ResetData();
	return false;
}

/* -------------------------------------------------------------------------- */

bool CAPE::RemoveFromFile(LPCWSTR FileName, bool saveID3v1Tag)
{ /* Remove tag from file if found */
	FILE *Source;
	// a tag at the beginning of the file: the rest of the file is written again
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) != NULL)
	{
		__int64 headOffset, headLength;
		const bool head = FindHeadTag(Source, headOffset, headLength);
		fclose(Source);
		if (head)
		{
			const bool removed = RewriteRegion(FileName, headOffset, headLength, NULL);
			TagInfo.Reset();
			return removed;
		}
	}
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYWR)) != NULL)
	{
		// read and remember the ID3v1 tag
		tmpid3v1.ReadFromFile(Source);

		bool result = ReadFooter(Source);
		fclose(Source);
		if (result)
		{
			if ( (TagInfo.Flags >> 31) != 0 )
				TagInfo.Size+= APE_TAG_HEADER_SIZE;
			// delete APE + ID3v1 tag!
			result = TruncateFile(FileName, tmpid3v1.GetSize() + TagInfo.Size);
			// write the ID3v1 tag back if it exists and is wanted
			if (saveID3v1Tag && tmpid3v1.GetSize() > 0)
				tmpid3v1.SaveToFile(FileName);
			TagInfo.Reset();
			return result;
		}
		else 
		{ 
			CTools::instance().setLastError(ERR_TAG_NOT_EXIST);
			TagInfo.Reset();
			return false;
		}
	}
	CTools::instance().setLastError(errno);
	return false;
}

/* -------------------------------------------------------------------------- */

bool CAPE::SaveToFile(LPCWSTR FileName)
{
	/* Delete old tag if exists and write new tag */
	CTools::instance().setLastError(0);
	// a tag at the beginning of the file is written again at the same place
	FILE *Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO);
	if (Source != NULL)
	{
		__int64 headOffset, headLength;
		const bool head = FindHeadTag(Source, headOffset, headLength);
		fclose(Source);
		if (head)
		{
			BuildTagData();
			return RewriteRegion(FileName, headOffset, headLength, &Data);
		}
	}
	// delete APE and ID3v1 tag
	bool result = RemoveFromFile(FileName, false);
	// a missing tag is not an error in this case
	if (result == false && CTools::instance().getLastError() != ERR_TAG_NOT_EXIST)
		return false;
	CTools::instance().setLastError(0);
	// the ID3v1 tag is in front of the end of the file only if there was no APE tag: then it is moved behind the new APE tag.
	// Otherwise RemoveFromFile has removed it together with the old APE tag.
	if (!result && tmpid3v1.GetSize() > 0)
		TruncateFile(FileName, tmpid3v1.GetSize());
	return SaveTag(FileName);  
}

/* -------------------------------------------------------------------------- */

CAtlString CAPE::GetTagVersion()
{
	if (FVersion == 0)
		return EMPTY;
	CAtlString tmp;
	tmp.Format(_T("%i"), FVersion);
	return tmp;	
}

/* -------------------------------------------------------------------------- */

CAtlString CAPE::GetAllKeys()
{
	CAtlString result;
	/* search all stored fields for key */
	for (size_t Iterator = 0; Iterator < _items.GetCount(); Iterator++)
	{
		item = _items.GetAt(Iterator);
		if ( !item->isBinary() )
		{ 
			if (result.GetLength() > 0)
				result+=_T(",");
			result+=item->Key;     
		}
	}
	return result;
}