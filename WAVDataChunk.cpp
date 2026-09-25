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

size_t CWAVDataChunk::getSize()
{
	if (_size == 0)
		return 0;
	if (_size % 2 == 1)
		return _size + 9;
	return _size + 8;	
}

bool CWAVDataChunk::load(FILE *Stream, u32 offset, u32 size)
{
	Stream;
	ATLTRACE(_T("Data Chunk with %i bytes\n"), size);
	_offset = offset;
	_size = size;
	return true;
}

void CWAVDataChunk::save(CBlob *blob)
{
	blob->Add4B(_chunkID);
	blob->AddR4B(_size);
}

bool CWAVDataChunk::save(FILE* Source, FILE *Destination)
{
	if (_size > 0)
	{
		long blockSize = CTools::configValues[CONFIG_ID3V2WRITEBLOCKSIZE];
		CBlob *blob = new CBlob(blockSize);
		blob->Add4B('data');
		blob->AddR4B(_size);
		blob->FileWrite(8, Destination);
		// Copy Block
		_fseeki64(Source, _offset, SEEK_SET);
		long tmpSize = 0;
		__int64 dataLen = _size;
		while (dataLen > 0)
		{
			tmpSize = (long)((dataLen > blockSize) ? blockSize : dataLen);
			ATLTRACE(_T("write %i data bytes\n"), tmpSize);
			blob->FileRead(tmpSize, Source);
			CTools::instance().doEvents();
			blob->FileWrite(tmpSize, Destination);
			CTools::instance().doEvents();
			dataLen-=tmpSize;
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
