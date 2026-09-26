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

class CID3F_EQUA:public CID3_Frame
{
public:
	CID3F_EQUA(long oldID);	
	CID3F_EQUA(BYTE format, LPCWSTR identification);
	void init(BYTE format, LPCWSTR identification);
	~CID3F_EQUA(void);	
	bool equals(CID3_Frame *frame);
	void decode();
	void encode();
	BYTE getFormat();
	CAtlString getIdentification();
	bool canStoreFor(BYTE version);
	BYTE getAdjustmentBits();
private:
	BYTE _format, _bits;
	CAtlString _identification;
	long _oldID;
	BYTE lastTag;
};
