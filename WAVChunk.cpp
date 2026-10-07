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

#include "StdAfx.h"
#include "WAVChunk.h"

bool CWAVChunk::s_bigEndian = false;

CWAVChunk::CWAVChunk(void)
{
	_data.Clear();
	_chunkID = 0;
}

CWAVChunk::CWAVChunk(u32 ID)
{
	_data.Clear();
	_chunkID = ID;
}
CWAVChunk::~CWAVChunk(void)
{
}
void CWAVChunk::Remove()
{
	_data.Clear();
}
u64 CWAVChunk::getSize()
{
	if (_data.GetLength() == 0)
		return 0;
	u64 size = (u64)_data.GetLength() + 8;
	if (_data.GetLength() % 2 == 1)
		size++;
	return size;
}

CWAVChunk* CWAVChunk::clone()
{
	CWAVChunk *copy = new CWAVChunk(_chunkID);
	copy->_data.AddBlob(_data);
	return copy;
}

CWAVChunk* CWAVChunk::find(u32 ID)
{
	return (ID == _chunkID) ? this : NULL;
}

bool CWAVChunk::load(CFile *Stream, u64 offset, u64 size)
{
	_data.FileReadAt(Stream, (__int64)offset, (size_t)size);
	return ((u64)_data.GetLength() == size);
}

void CWAVChunk::save(CBlob *blob)
{
	if (_data.GetLength() > 0)
	{
		blob->Add4B(_chunkID);
		if (s_bigEndian)
			blob->Add4B((int)_data.GetLength());
		else
			blob->AddR4B((int)_data.GetLength());
		blob->AddBlob(_data);
		if (_data.GetLength() % 2 == 1)
			blob->AddNullByte();
	}

}

bool CWAVChunk::save(CFile *Source, CFile *Destination)
{
	Source;
	if (_data.GetLength() > 0)
	{
		CBlob *blob = new CBlob(_data.GetLength() + 10);
		save(blob);
		blob->FileWrite(blob->GetLength(), Destination);
		delete blob;
	}
	return true;
}

