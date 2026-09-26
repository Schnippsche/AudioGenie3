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

class CMP4_STCO :
	public CMP4Atom
{
public:
	CMP4_STCO(u32 id = 'stco');	// 'stco' (32 bit offsets) or 'co64' (64 bit offsets)
	~CMP4_STCO(void);
	// adds delta to all offsets; false if an offset does not fit into the table any more
	bool move(__int64 delta, FILE *Destination);
	void save(FILE *Destination);
private:
	__int64 _position;
	bool _is64;
};
