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
#include "WAVRIFFContainer.h"
#include "WAVDataChunk.h"
#include "Tools.h"
#include "WAVBEXTChunk.h"


CWAVRIFFContainer::CWAVRIFFContainer(void)
{
	formatChunk = NULL;
	_chunkID = 0;
	_isRF64 = false;
	_isRIFX = false;
	_sampleCount64 = 0;
}

CWAVRIFFContainer::~CWAVRIFFContainer(void)
{
	Remove();
	delete formatChunk;
	formatChunk = NULL;
}

bool CWAVRIFFContainer::save(CFile *Source, CFile *Destination)
{
	s_bigEndian = _isRIFX;   // reassert: sibling chunks read this static flag while they save their own size fields below
	u64 totalSize = 0;
	const bool rf64 = needsRF64(totalSize);
	if (totalSize > 0)
	{
		CBlob *blob = new CBlob(16);
		blob->Add4B(_isRIFX ? 'RIFX' : (rf64 ? 'RF64' : 'RIFF'));
		if (_isRIFX)
			blob->Add4B((u32)(totalSize - 8));
		else
			blob->AddR4B(rf64 ? 0xFFFFFFFFu : (u32)(totalSize - 8));
		blob->Add4B('WAVE');
		blob->FileWrite(blob->GetLength(), Destination);
		delete blob;
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
			_children[i]->save(Source, Destination);
	}
	return true;
}

void CWAVRIFFContainer::save(CBlob *blob)
{
	// RIFF (or RF64/RIFX) ( 4 Bytes ), datasize ( 4 Bytes ), 'WAVE' ( 4 Bytes ), then the chunks
	s_bigEndian = _isRIFX;   // reassert: sibling chunks read this static flag while they save their own size fields below
	u64 totalSize = 0;
	const bool rf64 = needsRF64(totalSize);
	if (totalSize > 0)
	{
		blob->Add4B(_isRIFX ? 'RIFX' : (rf64 ? 'RF64' : 'RIFF'));
		if (_isRIFX)
			blob->Add4B((u32)(totalSize - 8));
		else
			blob->AddR4B(rf64 ? 0xFFFFFFFFu : (u32)(totalSize - 8));
		blob->Add4B('WAVE');
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			_children[i]->save(blob);
		}
	}
}

bool CWAVRIFFContainer::load(CFile *Stream, u64 offset, u64 size)
{
	// RIFF container, structure is mandatory:
	// 4 bytes 'RIFF', or 'RF64' for files of 4 GB or more (EBU Tech 3306): the size field is 0xFFFFFFFF then and the real sizes are in the
	// 'ds64' chunk that comes right behind the 'WAVE' text (RIFF size, size of the 'data' chunk, number of samples), or 'RIFX', the big
	// endian variant used by old Mac/SGI tools: identical structure, but the length field here and every chunk/subchunk size field
	// further down (and the 'fmt ' chunk's own numeric fields) are big endian instead of the usual RIFF little endian
	// 4 bytes length
	// 4 bytes 'WAVE' as text
	// then the remaining chunks/subchunks
	CBlob header;
	header.FileReadAt(Stream, (__int64)offset, 12);
	if (header.GetLength() != 12)
		return false;

	const u32 outerID = (u32)header.Get4B(0);
	if ((outerID != 'RIFF' && outerID != 'RF64' && outerID != 'RIFX') || header.Get4B(8) != 'WAVE')
		return false;
	_isRF64 = (outerID == 'RF64');
	_isRIFX = (outerID == 'RIFX');
	s_bigEndian = _isRIFX;
	_chunkID = 'WAVE';
	offset+=12;
	const u64 endPos = size;
	u64 realDataSize = 0;
	bool haveRealDataSize = false;
	u32 chunkID;
	u64 dataSize, nextReadPos;
	while (offset + 8 < endPos)
	{
		header.FileReadAt(Stream, (__int64)offset, 8);
		if (header.GetLength() < 8)
			break;
		chunkID = (u32)header.Get4B(0);
		dataSize = _isRIFX ? (u32)header.Get4B(4) : (u32)header.GetR4B(4);
		nextReadPos = offset + 8;
		if (chunkID == 'ds64' && dataSize >= 28 && nextReadPos + dataSize <= endPos)
		{
			// riff size (8), data size (8), sample count (8), table length (4), then the table (not used: none of our chunks reaches 4 GB besides 'data')
			CBlob ds64;
			ds64.FileReadAt(Stream, (__int64)nextReadPos, 28);
			if (ds64.GetLength() == 28)
			{
				realDataSize = (u64)ds64.GetR8B(8);
				_sampleCount64 = (u64)ds64.GetR8B(16);
				haveRealDataSize = true;
			}
		}
		else if (chunkID == 'data' && dataSize == 0xFFFFFFFFull && haveRealDataSize)
			dataSize = realDataSize;
		if (nextReadPos + dataSize > endPos)
		{
			CTools::instance().writeWarning(L"corrupt or invalid chunk '%c%c%c%c' at position %I64u ignored", BYTE(chunkID >> 24), BYTE(chunkID >> 16), BYTE(chunkID >> 8), BYTE(chunkID), offset);
			offset = endPos;
		}
		else
		{
			CWAVChunk *chunk;
			if (chunkID == 'LIST')
				chunk = new CWAVContainer(chunkID);
			else if (chunkID == 'data')
				chunk = new CWAVDataChunk();
			else if (chunkID =='cart')
				chunk = new CWAVCARTChunk();
			else if (chunkID =='bext' )
				chunk = new CWAVBEXTChunk();
			else if (chunkID =='DISP')
				chunk = new CWAVDISPChunk();
			else if (chunkID == 'fmt ')
			{
				formatChunk = new CWAVFormatChunk();
				chunk = formatChunk;
			}
			else
				chunk = new CWAVChunk(chunkID);

			_children.Add(chunk);
			CTools::instance().doEvents();
			chunk->load(Stream, nextReadPos, dataSize);
			offset = nextReadPos + dataSize;
			if (offset % 2 == 1)
				offset++;

		}
	}
	return true;
}

// Decides whether the file has to be written as RF64 (its total size is 4 GB or more, or it was read as RF64: once RF64 it stays RF64, the
// 'data' chunk never shrinks below 4 GB again on a tag save) and brings a 'ds64' chunk with the current sizes into the child list if so;
// totalSize is set to the size of the whole file (RIFF/RF64 header, 'WAVE' and all chunks).
bool CWAVRIFFContainer::needsRF64(u64 &totalSize)
{
	totalSize = getSize();
	if (totalSize == 0)
		return false;
	if (_isRIFX)
		return false;   // RIFX has no 'ds64'/large-file convention of its own; leave it as a plain (big endian) RIFX file
	if (!_isRF64 && totalSize <= 0xFFFFFFF0ull)
		return false;
	_isRF64 = true;
	CWAVChunk *ds64 = NULL;
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts && ds64 == NULL; i++)
		if (_children[i]->getID() == 'ds64')
			ds64 = _children[i];
	if (ds64 == NULL)
	{
		ds64 = new CWAVChunk('ds64');
		_children.InsertAt(0, ds64);
	}
	CWAVChunk *dataChunk = find('data');
	const u64 dataSize = (dataChunk != NULL) ? static_cast<CWAVDataChunk*>(dataChunk)->getPayloadSize() : 0;
	CBlob *d = ds64->getData();
	d->Clear();
	totalSize = getSize();   // the (possibly new) 'ds64' chunk changed the total size: recompute before it is written into itself
	d->AddR8B((__int64)(totalSize - 8));
	d->AddR8B((__int64)dataSize);
	d->AddR8B((__int64)_sampleCount64);
	d->AddR4B(0);            // table length: no further 64 bit sizes of other chunks
	totalSize = getSize();
	return true;
}

void CWAVRIFFContainer::Remove()
{
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
		delete _children[i];
	_children.RemoveAll();
	_children.SetCount(0,10);
	formatChunk = NULL;
}

void CWAVRIFFContainer::addInfoChunk(u32 FrameID, CAtlString newText)
{
	// look for the LIST/INFO chunk
	CWAVContainer* container = getInfoChunk();
	// create new? Insert before the data chunk
	if (container == NULL)
	{
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			if (_children[i]->getID() == 'data')
			{
				container = new CWAVContainer('INFO');
				_children.InsertAt(i,container);
				i = 999;
			}
		}
	}
	if (container == NULL)
	{
		CTools::instance().setLastError(ERR_NO_DATACHUNK); 
		return; 
	}
	CWAVChunk* chunk = new CWAVChunk(FrameID);
	chunk->getData()->Clear();
	chunk->getData()->AddEncodedString(TEXT_ENCODED_ANSI, newText, TEXT_WITHOUT_ENCODING, TEXT_WITH_NULLBYTES);
	container->_children.Add(chunk);
}

CWAVContainer* CWAVRIFFContainer::getInfoChunk()
{
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		if (_children[i]->getID() == 'INFO')
			return static_cast<CWAVContainer*>(_children[i]);
	}
	return NULL;
}

CWAVDISPChunk* CWAVRIFFContainer::getDispChunk()
{
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		// only look for DISP chunks with text content
		if (_children[i]->getID() == 'DISP')
		{
			CWAVDISPChunk* result = static_cast<CWAVDISPChunk*>(_children[i]);
			if (result->getType() == CF_TEXT)
				return result;
		}
	}
	return NULL;
}

CWAVCARTChunk* CWAVRIFFContainer::getCartChunk()
{
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		if (_children[i]->getID() == 'cart')
			return static_cast<CWAVCARTChunk*>(_children[i]);
	}
	return NULL;
}

CWAVCARTChunk* CWAVRIFFContainer::addCartChunk()
{
	// look for the CART chunk
	CWAVCARTChunk* chunk = getCartChunk();
	// create new? Insert before the data chunk
	if (chunk == NULL)
	{
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			if (_children[i]->getID() == 'data')
			{
				chunk = new CWAVCARTChunk();
				_children.InsertAt(i, chunk);
				i = 999;
			}
		}
	}
	if (chunk == NULL)
		CTools::instance().setLastError(ERR_NO_DATACHUNK); 
	return chunk;
}

CWAVBEXTChunk* CWAVRIFFContainer::getBextChunk()
{
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		if (_children[i]->getID() == 'bext')
			return static_cast<CWAVBEXTChunk*>(_children[i]);
	}
	return NULL;
}

CWAVBEXTChunk* CWAVRIFFContainer::addBextChunk()
{
	// look for the BEXT chunk
	CWAVBEXTChunk* chunk = getBextChunk();
	// create new? Insert before the data chunk
	if (chunk == NULL)
	{
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			if (_children[i]->getID() == 'data')
			{
				chunk = new CWAVBEXTChunk();
				_children.InsertAt(i, chunk);
				i = 999;
			}
		}
	}
	if (chunk == NULL)
		CTools::instance().setLastError(ERR_NO_DATACHUNK); 
	return chunk;
}

CWAVDISPChunk* CWAVRIFFContainer::addDispChunk()
{
	// look for the DISP chunk with text
	CWAVDISPChunk* chunk = getDispChunk();
	// create new? Append at the end
	if (chunk == NULL)
	{
		chunk = new CWAVDISPChunk();
		_children.Add(chunk);			
	}
	return chunk;
}
