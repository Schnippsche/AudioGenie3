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

class CID3F_GEOB:public CID3_Frame
{
public:
	CID3F_GEOB(void);	
	CID3F_GEOB(LPCWSTR mime, LPCWSTR filename , LPCWSTR description);
	void init(LPCWSTR mime, LPCWSTR filename , LPCWSTR description);
	~CID3F_GEOB(void);	
	bool equals(CID3_Frame *frame);
	void decode();
	void encode();
	CAtlString getMime();
	CAtlString getDescription();
	CAtlString getFilename();	
private:
	CAtlString _mime, _filename, _description;	
};
