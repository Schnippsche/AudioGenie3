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
#include "mp4atom.h"

class CMP4_Container:public CMP4Atom
{
public:
	CMP4_Container(void);
	CMP4_Container(u32 frameID);
	CMP4Atom* copy();
	~CMP4_Container(void);
	void init(u32 frameID);
	virtual CMP4Atom* find(CAtlString atomID);
	virtual void replaceAtom(CMP4Atom* atom);
	virtual void addAtom(CMP4Atom* atom);
	virtual bool removeAtom(CAtlString atomID);
	void remove();
	bool removeAtom(CMP4Atom* atom);
	void load(FILE *stream, u64 offset, u64 size);
	virtual void save(FILE *stream);
	u64 getSize();	
protected:
	u64 _offset, _size;
};