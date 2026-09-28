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
#include "md5.h"

/*
* Size of the read blocks for the hash calculation. 64 KB instead of 16 KB reads about 9 % faster from the file cache (560 -> 610
* MB/s); 256 KB and 1 MB, ReadFile with SEQUENTIAL_SCAN and overlapped reads showed no further gain. On a network drive the
* transfer rate (50 to 70 MB/s measured) limits the speed regardless of the block size. See the notes in md5.cpp.
*/
#define MAX_MD5_BUFFER (64*1024)

class CMD5Tool
{
protected:
	MD5 *md5;
	MD5_CTX ctx;
	BYTE *buffer;
	CAtlString hash;
private:
	void hashIt();
public:
	CMD5Tool(void);
	~CMD5Tool(void);
	CAtlString getHashValue()	{ return hash; };
	bool calcHashFromFile(LPCWSTR FileName, __int64 startPos, __int64 endPos);	
};
