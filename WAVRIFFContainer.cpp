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
}

CWAVRIFFContainer::~CWAVRIFFContainer(void)
{
	Remove();
	delete formatChunk;
	formatChunk = NULL;
}

bool CWAVRIFFContainer::save(FILE* Source, FILE *Destination)
{
	if (getSize() > 0)
	{
		CBlob *blob = new CBlob(getSize() + 12);
		blob->Add4B('RIFF');
		blob->AddR4B((u32)getSize() - 8);
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
	// LIST ( 4 Bytes )
	// datasize ( 4 Bytes )
	// chunkID ( 4 Bytes )
	if (getSize() > 0)
	{
		blob->Add4B('RIFF');
		blob->AddR4B((u32)getSize() - 8);
		blob->Add4B('WAVE');
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			_children[i]->save(blob);		
		}
	}
}

bool CWAVRIFFContainer::load(FILE *Stream, u32 offset, u32 size)
{
	// RIFF container, structure is mandatory:
	// 4 Bytes 'RIFF'
	// 4 bytes length
	// 4 bytes 'WAVE' as text
	// then the remaining chunks/subchunks
	CBlob header;
	_fseeki64(Stream, offset, SEEK_SET);
	header.FileRead(12, Stream);
	if (header.GetLength() != 12)
		return false;

	if (header.Get4B(0) != 'RIFF' || header.Get4B(8) != 'WAVE')
		return false;
	_chunkID = 'WAVE';
	offset+=12;
	u32 endPos = size;
	u32 chunkID, dataSize;
	u32 nextReadPos;
	while (offset + 8 < endPos)
	{
		_fseeki64(Stream, offset, SEEK_SET);
		header.FileRead(8, Stream);
		if (header.GetLength() < 8)
			break;
		chunkID = header.Get4B(0);
		dataSize = header.GetR4B(4);
		nextReadPos = offset + 8;
		if ((unsigned __int64)nextReadPos + dataSize > endPos)
		{
			CTools::instance().writeWarning(L"corrupt or invalid chunk '%c%c%c%c' at position %i ignored", BYTE(chunkID >> 24), BYTE(chunkID >> 16), BYTE(chunkID >> 8), BYTE(chunkID), offset);
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
	chunk->getData()->AddEncodedString(TEXT_ENCODED_ANSI, newText, false, true);
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
