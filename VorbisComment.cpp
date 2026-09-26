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

// VorbisComment.cpp: implementation of class CVorbisComment.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VorbisComment.h"
#include "Tools.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVorbisComment::CVorbisComment()
{
	_items.SetCount(0, 20);	
}

CVorbisComment::~CVorbisComment()
{
	ResetData();
}

void CVorbisComment::ResetData()
{
	Fields = 0;
	VendorInfo = DEFAULT_VENDOR;
	for (size_t i = 0; i < _items.GetCount(); i++)
		delete _items.GetAt(i);
	_items.RemoveAll();	
}

void CVorbisComment::BuildVorbisComments(CBlob &Data)
{
	/* Build Comments into Blob */
	CBlob str;
	int Index;
	// Build Vorbis tag
	Fields = 0;
	content.Empty();
	//int maxLen = VendorInfo.GetLength();
	Fields = (int)_items.GetCount();
	// vendor info and number of fields
	str.AddEncodedString(TEXT_ENCODED_UTF8, VendorInfo, false,false);
	Data.AddR4B((int)str.GetLength());
	Data.AddBlob(str); 
	Data.AddR4B(Fields);
	// Write tag fields
	if (Fields == 0)
		return;
	for (Index = 0; Index < Fields; Index++)
	{
		item = _items.GetAt(Index);	
		content = item->key + L"=" + item->value;
		//content = item->key;
		//content+=_T("=");
		//content+=item->value;
		str.Clear();
		str.AddEncodedString(TEXT_ENCODED_UTF8, content, false, false);
		Data.AddR4B((int)str.GetLength());
		Data.AddBlob(str);		
	}
}

// the list of the comments as the specification defines it: vendor (length, UTF-8), number of the comments, the comments (length, UTF-8 "NAME=value")
void CVorbisComment::AnalyzeVorbisComments(const BYTE *data, size_t length)
{
	auto read32 = [&](size_t pos) -> size_t { return (size_t)data[pos] | ((size_t)data[pos + 1] << 8) | ((size_t)data[pos + 2] << 16) | ((size_t)data[pos + 3] << 24); };
	size_t pos = 0;
	if (length < 4)
		return;
	size_t len = read32(0);
	pos = 4;
	if (len > length - pos)
		return;
	CBlob tmpBlob;
	tmpBlob.AddMemory(data + pos, len);
	VendorInfo = tmpBlob.ConvertToUnicodeString(TEXT_ENCODED_UTF8);
	pos += len;
	if (length - pos < 4)
		return;
	const size_t count = read32(pos);
	pos += 4;
	Fields = (int)count;
	for (size_t i = 0; i < count; i++)
	{
		if (length - pos < 4)
			return;   // the data end
		len = read32(pos);
		pos += 4;
		const bool truncated = (len > length - pos);
		if (truncated)
			len = length - pos;
		tmpBlob.Clear();
		tmpBlob.AddMemory(data + pos, len);
		pos += len;
		CAtlString temp = tmpBlob.ConvertToUnicodeString(TEXT_ENCODED_UTF8);
		const int separator = temp.Find(_T("="));
		if (separator > 0)
		{
			item = new structField;
			item->key = temp.Left(separator);
			item->value = temp.Mid(separator + 1);
			_items.Add(item);
		}
		if (truncated)
			return;
	}
}

bool CVorbisComment::IsValidKey(LPCWSTR key)
{
	if (key == NULL || key[0] == 0)
		return false;
	for (const wchar_t *c = key; *c != 0; c++)
		if (*c < 0x20 || *c > 0x7D || *c == L'=')
			return false;
	return true;
}

void CVorbisComment::AnalyzeVorbisComments(FILE *Stream)
{
	long i, len = 0, Separator;
	CBlob tmpBlob;
	CAtlString temp;
	// Vendor
	tmpBlob.FileRead(4, Stream);
	if (tmpBlob.GetLength() < 4)
		return;
	len = tmpBlob.GetR4B(0);
	if (len < 0 || len > CTools::FileSize)
		return;
	tmpBlob.FileRead(len, Stream);
	VendorInfo = tmpBlob.ConvertToUnicodeString(TEXT_ENCODED_UTF8);  
	// continue with the number of fields
	tmpBlob.FileRead(4, Stream);
	if (tmpBlob.GetLength() < 4)
		return;
	Fields = tmpBlob.GetR4B(0);
	if (Fields <= 0)
		return;

	for (i = 0; i < Fields ; i++)
	{
		tmpBlob.FileRead(4, Stream);
		if (tmpBlob.GetLength() < 4) // end of file reached
			return;
		len = tmpBlob.GetR4B(0);
		if (len < 0 || len > CTools::FileSize)
			return;
		tmpBlob.FileRead(len, Stream);
		bool truncated = (tmpBlob.GetLength() < (size_t)len);
		temp = tmpBlob.ConvertToUnicodeString(TEXT_ENCODED_UTF8);
		if ((Separator = temp.Find(_T("="))) > 0)
		{
			item = new structField;
			item->key = temp.Left(Separator);
			item->value = temp.Mid(Separator + 1);
			_items.Add(item);
		}
		if (truncated)
			return;
	} 
}

CAtlString CVorbisComment::GetUserItem(LPCWSTR key)
{
	CAtlString result;
	size_t counts = _items.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		item = _items.GetAt(i);
		if (item->key.CompareNoCase(key) == 0)
		{
			if (result.GetLength() > 0)
				result+=_T("|");
			result+=item->value;
		}
	}
	return result;
}

// Fields with the same name are allowed by the specification; setting a value replaces all of them, an empty value removes all of them.
void CVorbisComment::SetUserItem(LPCWSTR key, LPCWSTR value)
{
	if (!IsValidKey(key))
	{
		CTools::instance().writeWarning(L"Vorbis comment '%s' ignored: a field name has the characters $20 to $7D without '='", key == NULL ? L"" : key);
		return;
	}
	const bool empty = (value == 0 || wcslen(value) == 0);
	bool found = false;
	for (size_t i = 0; i < _items.GetCount(); )
	{
		item = _items.GetAt(i);
		if (item->key.CompareNoCase(key) == 0)
		{
			if (!found && !empty)
			{
				item->value = value;   // the first field is kept
				found = true;
				i++;
				continue;
			}
			delete item;   // an empty value or another field of the same name
			_items.RemoveAt(i);
			continue;
		}
		i++;
	}
	if (found || empty)
		return;
	item = new structField;
	item->key = key;
	item->value = value;
	_items.Add(item);
}

CAtlString CVorbisComment::GetAllKeys()
{
	CAtlString result;
	/* search all stored fields for key */
	size_t counts = _items.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		if (result.GetLength() > 0)
			result+=",";
		result+=_items.GetAt(i)->key;		
	}
	return result;
} 