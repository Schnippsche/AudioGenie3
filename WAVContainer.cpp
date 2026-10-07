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
#include "WAVContainer.h"
#include "Tools.h"

CWAVContainer::CWAVContainer(void)
{
	_data.Clear();
	_chunkID = 0;
	_children.SetCount(0, 10);
}
CWAVContainer::CWAVContainer(u32 ID)
{
	_data.Clear();
	_chunkID = ID;
	_children.SetCount(0, 10);
}

CWAVContainer::~CWAVContainer(void)
{
	Remove();
}
u64 CWAVContainer::getSize()
{
	u64 size = 12;
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		size+=_children[i]->getSize();
	}
	return size;
}

CWAVChunk* CWAVContainer::clone()
{
	CWAVContainer *copy = new CWAVContainer(_chunkID);
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
		copy->_children.Add(_children[i]->clone());
	return copy;
}

CWAVChunk* CWAVContainer::find(u32 ID)
{
	CWAVChunk* chunk = NULL;
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		chunk = _children[i]->find(ID);
		if (chunk != NULL)
			return chunk;
	}
	return NULL;
}

bool CWAVContainer::load(CFile *Stream, u64 offset, u64 size)
{
	const u64 endPos = offset + size;
	CBlob header;
	// container has 4 extra bytes at the start, take them as the ID
	header.FileReadAt(Stream, (__int64)offset, 4);
	_chunkID = header.Get4B(0);
	offset+=4;
	while (offset < endPos)
	{
		header.FileReadAt(Stream, (__int64)offset, 8); 
		if (header.GetLength() < 8)
			return false;
		u32 chunkID = header.Get4B(0);
		u64 dataSize = s_bigEndian ? (u32)header.Get4B(4) : (u32)header.GetR4B(4);

		if (offset + 8 + dataSize > endPos)
			return false;

		CWAVChunk *chunk = new CWAVChunk(chunkID);
		_children.Add(chunk);
		CTools::instance().doEvents();
		u64 nextReadPos = offset + 8;
		chunk->load(Stream, nextReadPos, dataSize);
		if (dataSize % 2 == 1)
			nextReadPos++;
		offset = nextReadPos + dataSize;
	}
	return true;
}

void CWAVContainer::save(CBlob *blob)
{
	// LIST ( 4 Bytes )
	// datasize ( 4 Bytes )
	// chunkID ( 4 Bytes )
	if (getSize() > 0)
	{
		blob->Add4B('LIST');
		if (s_bigEndian)
			blob->Add4B((u32)(getSize() - 8));
		else
			blob->AddR4B((u32)(getSize() - 8));
		blob->Add4B(_chunkID);
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			_children[i]->save(blob);
		}
	}
}

bool CWAVContainer::save(CFile *Source, CFile *Destination)
{
	if (getSize() > 0)
	{
		CBlob *blob = new CBlob((size_t)getSize() + 12);
		blob->Add4B('LIST');
		if (s_bigEndian)
			blob->Add4B((u32)(getSize() - 8));
		else
			blob->AddR4B((u32)(getSize() - 8));
		blob->Add4B(_chunkID);
		blob->FileWrite(blob->GetLength(), Destination);
		delete blob;
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
			_children[i]->save(Source, Destination);		
	}
	return true;
}


void CWAVContainer::Remove()
{
	size_t counts = _children.GetCount();
	ATLTRACE(L"WavContainer.Remove(%i)", counts);
	for (size_t i = 0; i < counts; i++)
		delete _children.GetAt(i);
	_children.RemoveAll();
	_children.SetCount(0,10);
}