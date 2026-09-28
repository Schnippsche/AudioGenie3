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

#include "StdAfx.h"
#include "MP4_container.h"
#include "mp4_atomfactory.h"
#include "Tools.h"

CMP4_Container::CMP4_Container()
{
	init(0);
}

CMP4_Container::CMP4_Container(u32 frameID)
{
	init(frameID);	
}
CMP4Atom* CMP4_Container::copy()
{
	CMP4Atom *atom;
	atom = new CMP4_Container(getFrameID());
	atom->_blob.Clear();
	atom->_blob.AddBlob(_blob);
	atom->_parent = _parent;
	// copy the children as well
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		atom->addAtom(_children[i]->copy());
	}
	return atom;
}

void CMP4_Container::init(u32 frameID)
{
	setFrameID(frameID);
	_children.SetCount(0, 10);
	setParent(EMPTY);
}

CMP4_Container::~CMP4_Container(void)
{
	remove();
}
void CMP4_Container::replaceAtom(CMP4Atom* atom)
{
	CAtlString path = getPath();
	CAtlString atomID = atom->getPath();
	int pathLen = path.GetLength();
	if (atomID.Left(pathLen).CompareNoCase(path) == 0)
	{
		// is the wanted right branch already at the end?
		CAtlString node;
		if (pathLen == 0)
			node = atomID.Left(4);
		else
			node = atomID.Mid(pathLen + 1, 4);

		bool isEnde = (atomID.GetLength() - pathLen <= 5);
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			if (_children[i]->getId().CompareNoCase(node) == 0)
			{
				if (isEnde)
				{
					delete _children[i];
					_children[i] = atom;
					atom->setParent(this);
					return;
				}
				_children[i]->replaceAtom(atom);
				return;
			}			
		}
		// not found: append at the end if necessary
		if (isEnde)
		{
			_children.Add(atom);
			atom->setParent(this);
			return;
		}
		// somewhere in the middle, create a new container recursively 
		u32 id = (node.GetAt(0) << 24) + (node.GetAt(1) << 16) + (node.GetAt(2) << 8) + node.GetAt(3);
		CMP4Atom* container = CMP4_AtomFactory::instance()->createAtom(id);
		container->setParent(getPath());
		_children.Add(container);
		container->replaceAtom(atom);
		return;
	}		
}

void CMP4_Container::addAtom(CMP4Atom* atom)
{
	CAtlString path = getPath();
	CAtlString atomID = atom->getPath();
	int pathLen = path.GetLength();
	if (atomID.Left(pathLen).CompareNoCase(path) == 0)
	{
		// is the wanted right branch already at the end?
		CAtlString node;
		if (pathLen == 0)
			node = atomID.Left(4);
		else
			node = atomID.Mid(pathLen + 1, 4);

		bool isEnde = (atomID.GetLength() - pathLen <= 5);
		if (isEnde)
		{
			atom->setParent(this);
			_children.Add(atom);
			return;
		}
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			if (_children[i]->getId().CompareNoCase(node) == 0)
			{
				_children[i]->addAtom(atom);
				return;
			}
		}
		// somewhere in the middle, create a new container recursively 
		u32 id = (node.GetAt(0) << 24) + (node.GetAt(1) << 16) + (node.GetAt(2) << 8) + node.GetAt(3);
		CMP4Atom* container = CMP4_AtomFactory::instance()->createAtom(id);
		container->setParent(getPath());
		_children.Add(container);
		container->addAtom(atom);
		return;
	}		
}

CMP4Atom* CMP4_Container::find(CAtlString atomID)
{
	CAtlString path = getPath();
	if (path.CompareNoCase(atomID) == 0)
		return (--CMP4_AtomFactory::count == 0) ? this : NULL;
	CMP4Atom* atom = NULL;
	if (atomID.Left(path.GetLength()).CompareNoCase(path) == 0)
	{
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			atom = _children[i]->find(atomID);
			if (atom != NULL)
				return atom;
		}
	}	
	return NULL;
}

void CMP4_Container::load(FILE *Stream, u64 offset, u64 size)
{
	_size = size;
	_offset = offset;
	u64 endPos = _offset + _size;
	if (getFrameID() == MP4_META)
		_offset+=4;
	CAtlString parent(getPath());
	CBlob header(16);
	u64 nextReadPos = 0;
	u64 atomSize = 0;
	u32 atomID = 0;
	u64 dataSize = 0;
	u32 headerLength = 8;
	CMP4Atom *atom;
	while (_offset + 8 <= endPos)
	{
		_fseeki64(Stream, (__int64)_offset, SEEK_SET);
		header.FileRead(8, Stream);
		if (header.GetLength() < 8)
			return;
		atomSize = header.Get4B(0);
		atomID = header.Get4B(4);
		if (atomID == 0)
			return;
		headerLength = 8;
		if (atomSize == 1) // the size follows as a 64 bit value (extended size)
		{
			header.FileRead(8, Stream);
			if (header.GetLength() < 8)
				return;
			atomSize = ((u64)header.Get4B(0) << 32) | header.Get4B(4);
			headerLength = 16;
		}
		else if (atomSize == 0) // This is the last atom, it reaches to the end of the file
		{
			if ((u64)CTools::FileSize <= _offset)
				return;
			atomSize = (u64)CTools::FileSize - _offset;
		}
		// an atom is at least as large as its header and must lie inside the container
		if (atomSize < headerLength || _offset + atomSize > endPos)
			return;
		dataSize = atomSize - headerLength;
		// the data of an atom are read into memory, only the media data (mdat) are not
		if (dataSize > 0x7FFFFFFFull && atomID != MP4_MDAT)
		{
			CTools::instance().setLastError(ERR_FRAME_TOO_BIG);
			return;
		}
		// protection against atoms nested too deeply (stack overflow)
		if (CMP4_AtomFactory::instance()->ebene > 100)
		{
			CTools::instance().setLastError(ERR_FRAME_TOO_BIG);
			return;
		}
		atom = CMP4_AtomFactory::instance()->createAtom(atomID);
		atom->_extended = (headerLength == 16);
		CMP4_AtomFactory::instance()->ebene+=2;
		_children.Add(atom);
		atom->setParent(parent);
#ifdef DEBUG
		atom->print();
#endif
		CTools::instance().doEvents();
		nextReadPos = _offset + headerLength;
		atom->load(Stream, nextReadPos, dataSize);
		CMP4_AtomFactory::instance()->ebene-=2;
		_offset+=atomSize;
	}
}

u64 CMP4_Container::getSize()
{
	u64 size = headerSize();
	if (getFrameID() == MP4_META)
		size+=4;

	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		size+=_children[i]->getSize();
	}
	return size;
}

void CMP4_Container::remove()
{
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		_children[i]->remove();
		delete _children[i];
	}
	_children.RemoveAll();
}

bool CMP4_Container::removeAtom(CAtlString atomID)
{
	CAtlString path = getPath();
	if (path.CompareNoCase(atomID) == 0)
	{
		// delete myself
		remove();
		return true;
	}
	if (atomID.Left(path.GetLength()).CompareNoCase(path) == 0)
	{
		size_t counts = _children.GetCount();
		for (size_t i = 0; i < counts; i++)
		{
			if (_children[i]->removeAtom(atomID))
			{
				delete _children[i];
				_children.RemoveAt(i);
				return false;
			}
		}
	}	
	return false;
}

bool CMP4_Container::removeAtom(CMP4Atom* atom)
{
	CAtlString path = getPath();
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		if (_children[i]->removeAtom(atom))
		{
			delete _children[i];
			_children.RemoveAt(i);				
			return false;
		}
	}
	return false;
}
void CMP4_Container::save(FILE *stream)
{
	u64 frameSize = getSize();
	CBlob tmp(20);
	if (_extended)
	{
		tmp.Add4B(1);
		tmp.Add4B(getFrameID());
		tmp.Add4B((u32)(frameSize >> 32));
		tmp.Add4B((u32)frameSize);
	}
	else
	{
		tmp.Add4B((u32)frameSize);
		tmp.Add4B(getFrameID());
	}
	if (getFrameID() == MP4_META)
		tmp.Add4B(0);
	tmp.FileWrite(tmp.GetLength(), stream);
	size_t counts = _children.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		_children[i]->save(stream);		
	}	
}
