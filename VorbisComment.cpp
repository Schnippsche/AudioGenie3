/* AudioGenie is a Library for analyzing and tagging audio files.
   Copyright (C) 2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011
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

void CVorbisComment::SetUserItem(LPCWSTR key, LPCWSTR value)
{
	size_t counts = _items.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		item = _items.GetAt(i);
		if (item->key.CompareNoCase(key) == 0)
		{
			item->value = value;
			// if the value is empty, also remove the key
			if (value == 0 || wcslen(value) == 0)
			{
				delete item;
				_items.RemoveAt(i);
			}
			return ;
		}
	}
	// if the value is empty, leave the procedure
	if (value == 0 || wcslen(value) == 0)
		return ;
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