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

#include "ID3_frame.h"
#include "atlcoll.h"

class CID3F_T000 : public CID3_Frame
{
public:
	CID3F_T000(void);
	CID3F_T000(unsigned int frameID);
	CID3F_T000(unsigned int frameID, LPCWSTR newText);
	~CID3F_T000(void);	
	void init(unsigned int frameID, LPCWSTR newText);
	void decode();
	void encode();
	CAtlString getText();
	bool isTextFrame() { return true; }
	void print();
private:
	CAtlString _text;                    // the first string
	CAtlArray<CAtlString> _more;         // further strings (id3v2.4 allows several strings in a text frame)
	BYTE _lastTag;                       // tag version of the data in the blob
};
