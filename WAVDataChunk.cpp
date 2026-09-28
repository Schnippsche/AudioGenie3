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
#include "WAVDataChunk.h"
#include "Tools.h"

CWAVDataChunk::CWAVDataChunk(void)
{
	_chunkID = 'data';
	_size = 0;
}

CWAVDataChunk::~CWAVDataChunk(void)
{
}

u64 CWAVDataChunk::getSize()
{
	if (_size == 0)
		return 0;
	if (_size % 2 == 1)
		return _size + 9;
	return _size + 8;	
}

bool CWAVDataChunk::load(FILE *Stream, u64 offset, u64 size)
{
	Stream;
	_offset = offset;
	_size = size;
	return true;
}

void CWAVDataChunk::save(CBlob *blob)
{
	blob->Add4B(_chunkID);
	// the size field of a chunk has 32 bit; a file of 4 GB or more (RF64) puts the real size into the 'ds64' chunk and 0xFFFFFFFF here
	const u32 sizeField = _size >= 0xFFFFFFFFull ? 0xFFFFFFFFu : (u32)_size;
	if (s_bigEndian)
		blob->Add4B(sizeField);
	else
		blob->AddR4B(sizeField);
}

bool CWAVDataChunk::save(FILE* Source, FILE *Destination)
{
	if (_size > 0)
	{
		long blockSize = CTools::configValues[CONFIG_ID3V2WRITEBLOCKSIZE];
		CBlob *blob = new CBlob(blockSize);
		blob->Add4B('data');
		const u32 sizeField = _size >= 0xFFFFFFFFull ? 0xFFFFFFFFu : (u32)_size;
		if (s_bigEndian)
			blob->Add4B(sizeField);
		else
			blob->AddR4B(sizeField);
		blob->FileWrite(8, Destination);
		// Copy Block
		_fseeki64(Source, (__int64)_offset, SEEK_SET);
		long tmpSize = 0;
		unsigned __int64 dataLen = _size;
		while (dataLen > 0)
		{
			tmpSize = (long)((dataLen > (unsigned __int64)blockSize) ? blockSize : dataLen);
			blob->FileRead(tmpSize, Source);
			CTools::instance().doEvents();
			blob->FileWrite(tmpSize, Destination);
			CTools::instance().doEvents();
			dataLen-=(unsigned __int64)tmpSize;
		}
		if (_size % 2 == 1)
		{
			blob->Clear();
			blob->AddNullByte();
			blob->FileWrite(1, Destination);
		}
		delete blob;			
	}
	return true;
}
