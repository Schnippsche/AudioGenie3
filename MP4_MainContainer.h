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
#include "mp4_container.h"
class CMP4_MainContainer:public CMP4_Container
{
public:
	CMP4_MainContainer(void);
	~CMP4_MainContainer(void);
	CMP4Atom* find(CAtlString atomID, int count = 1);
	void save(CFile *stream);
	void adjustPadding(__int64 size);
	// true if the atoms, written from position base on, leave every mdat atom where it is (they can be written into the same file then)
	bool mdatsKeepPositions(__int64 base);
	// a fragmented file (moof, mfra, sidx or moov.mvex): its offsets in the fragments are not adjusted when data move
	bool isFragmented();
	void checkMetaBox();
};
