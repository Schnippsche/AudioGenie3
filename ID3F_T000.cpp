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

#include "StdAfx.h"
#include "ID3F_T000.h"
#include "ID3v2.h"

/*
<Header for 'Text information frame', ID: "T000" - "TZZZ", excluding "TXXX">
Text encoding                $xx
Information                  <text string according to encoding>
*/

CID3F_T000::CID3F_T000(void) 	
{
	init(0, EMPTY);	
}

CID3F_T000::CID3F_T000(unsigned int frameID) 	
{
	init(frameID, EMPTY);
}

CID3F_T000::CID3F_T000(unsigned int frameID, LPCWSTR newText) 	
{
	init(frameID, newText);	
}

void CID3F_T000::init(unsigned int frameID, LPCWSTR newText)
{
	_frameID = frameID;
	if (newText != NULL)
		_text = newText;
	else
		_text.Empty();
	useTextEncoding = true;
	mustRebuild = true;
	isDecoded = true;
	_more.RemoveAll();
	_lastTag = 99;
}


CID3F_T000::~CID3F_T000(void)
{
	_text.Empty();
}

void CID3F_T000::decode()
{
	if (!isDecoded)
	{	
		if (isUnsynchronized())
			resync();
		_text.Empty();
		_more.RemoveAll();
		_lastTag = CTools::ID3V2oldTagVersion;
		if (_blob.GetLength() > 0)
		{
			encodingID = _blob.GetAt(0);
			if (_blob.GetLength() > 1)
			{
				int start = 1;
				_text = _blob.getNextString(encodingID, start);
				// further strings, separated by the terminator of the encoding
				while (start < (int)_blob.GetLength())
					_more.Add(_blob.getNextString(encodingID, start));
				// a terminator after the last string does not add a value
				while (_more.GetCount() > 0 && _more[_more.GetCount() - 1].IsEmpty())
					_more.RemoveAt(_more.GetCount() - 1);
			}
		}
		isDecoded = true;
	}	
}

void CID3F_T000::encode()
{
	if (mustRebuild || (_more.GetCount() > 0 && _lastTag != CTools::ID3V2newTagVersion))
	{
		decode();
		_blob.Clear();
		if (_more.GetCount() == 0)
			_blob.AddEncodedString(encodingID, _text, TEXT_WITH_ENCODING, TEXT_WITHOUT_NULLBYTES);
		else if (CTools::ID3V2newTagVersion == TAG_VERSION_2_4)
		{
			// id3v2.4: null separated list of strings
			_blob.AddEncodedString(encodingID, _text, TEXT_WITH_ENCODING, TEXT_WITH_NULLBYTES);
			for (size_t i = 0; i < _more.GetCount(); i++)
				_blob.AddEncodedString(encodingID, _more[i], TEXT_WITHOUT_ENCODING, (i + 1 < _more.GetCount()) ? TEXT_WITH_NULLBYTES : TEXT_WITHOUT_NULLBYTES);
		}
		else
		{
			// id3v2.2 and id3v2.3 know only one string; several values are separated by a slash
			CAtlString joined(_text);
			for (size_t i = 0; i < _more.GetCount(); i++)
			{
				joined += _T("/");
				joined += _more[i];
			}
			_blob.AddEncodedString(encodingID, joined, TEXT_WITH_ENCODING, TEXT_WITHOUT_NULLBYTES);
		}
		_lastTag = CTools::ID3V2newTagVersion;
		mustRebuild = false;
	}
}

CAtlString CID3F_T000::getText()
{
	decode();
	return _text;
}

void CID3F_T000::print()
{
	ATLTRACE(_T("Frame:%c%c%c%c %u "), BYTE(_frameID >> 24), BYTE(_frameID >> 16), BYTE(_frameID >> 8), BYTE(_frameID), _frameID);
	decode();
	ATLTRACE(_T(" Encoding:%i Text:%s\n"), encodingID,_text);
}