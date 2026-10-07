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
#include <vector>

// an mdat atom of the old file (its bytes start to end) and how far it has moved in the new file
struct CMP4_Move
{
	__int64 start, end, delta;
};

class CMP4_STCO :
	public CMP4Atom
{
public:
	CMP4_STCO(u32 id = 'stco');	// 'stco' (32 bit offsets) or 'co64' (64 bit offsets)
	~CMP4_STCO(void);
	// moves every offset by the delta of the mdat atom it points into (an offset outside of all of them by the delta of the first one) and
	// writes the table again at its place; false if an offset does not fit into the table any more
	bool move(const std::vector<CMP4_Move> &moves, CFile *Destination);
	void save(CFile *Destination);
private:
	__int64 _position;
	bool _is64;
};
