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
#include "Blob.h"
typedef unsigned __int32  u32;
typedef unsigned __int64  u64;

class CWAVChunk
{
public:
	CWAVChunk(void);
	CWAVChunk(u32 ID);
	virtual ~CWAVChunk(void);
	virtual CWAVChunk* find(u32 ID);
	virtual u64 getSize();
	virtual bool isID(u32 ID) { return ( _chunkID == ID ); };
	virtual bool load(CFile *Stream, u64 offset, u64 size);
	virtual void save(CBlob *blob);	
	virtual bool save(FILE* Source, FILE *Destination);
	virtual void Remove();
	u32 getID() { return _chunkID; };
	CBlob* getData() { return &_data; };
	// true while the current WAV file is a 'RIFX' file (big endian RIFF variant used by old Mac/SGI tools): the outer header and every
	// chunk's size field are big endian then, instead of the usual RIFF little endian; chunk payload bytes are unaffected by this flag,
	// so any chunk that is never rebuilt from parsed fields on save still round-trips correctly regardless of its value. Kept as a static
	// flag (like CTools::FileSize/ID3v2Size) because sibling chunk objects have no reference back to the top-level RIFF container that
	// first detects it; CWAVRIFFContainer::load/save always (re)set it before it is read from.
	static bool s_bigEndian;
protected:
	u32 _chunkID;
	CBlob _data;
};
