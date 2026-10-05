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
#include "wavchunk.h"

class CWAVDataChunk :
	public CWAVChunk
{
public:
	CWAVDataChunk(void);
	~CWAVDataChunk(void);
	bool load(CFile *Stream, u64 offset, u64 size);
	void save(CBlob *blob);
	bool save(FILE* Source, FILE *Destination);
	u64 getSize();
	u64 getOffset() { return _offset; };
	u64 getPayloadSize() { return _size; };   // the size of the audio data alone, without the chunk header (the 64 bit value from 'ds64' for RF64 files)
	void copyPositionFrom(const CWAVDataChunk &other) { _offset = other._offset; _size = other._size; };
protected:
	u64 _size;
	u64 _offset{};
};
