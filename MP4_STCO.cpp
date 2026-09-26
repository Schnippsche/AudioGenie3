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
#include "MP4_STCO.h"
#include "mp4_atomfactory.h"

CMP4_STCO::CMP4_STCO(u32 id)
{
	setFrameID(id);
	_is64 = (id == MP4_CO64);
	_position = 0;
}

CMP4_STCO::~CMP4_STCO(void)
{
}

bool CMP4_STCO::move(__int64 delta, FILE* Destination)
{
	if (_blob.GetLength() < 8 || delta == 0)
		return true;
	const size_t entrySize = _is64 ? 8 : 4;
	u32 count = _blob.Get4B(4);
	// entry_count comes from the file: at most as many entries as the atom content really holds
	// (otherwise the calculation overflows and the loop takes practically forever)
	const u32 maxCount = (u32)((_blob.GetLength() - 8) / entrySize);
	if (count > maxCount)
		count = maxCount;
	CBlob tmp(count * entrySize + 8);
	tmp.AddMemory(_blob.m_pData, 8);
	for (u32 lfd = 0; lfd < count; lfd++)
	{
		if (_is64)
		{
			const __int64 entry = (__int64)(((u64)_blob.Get4B(8 + (size_t)lfd * 8) << 32) | _blob.Get4B(12 + (size_t)lfd * 8)) + delta;
			if (entry < 0)
				return false;
			tmp.Add4B((u32)((u64)entry >> 32));
			tmp.Add4B((u32)entry);
		}
		else
		{
			const __int64 entry = (__int64)_blob.Get4B(8 + (size_t)lfd * 4) + delta;
			if (entry < 0 || entry > 0xFFFFFFFFll)
				return false;   // a 32 bit table cannot hold the offset
			tmp.Add4B((u32)entry);
		}
	}
	// data behind the entries stay as they are
	const size_t used = 8 + (size_t)count * entrySize;
	if (_blob.GetLength() > used)
		tmp.AddMemory(_blob.m_pData + used, _blob.GetLength() - used);
	_blob.Clear();
	_blob.AddBlob(tmp);
	_fseeki64(Destination, _position, SEEK_SET);
	CMP4Atom::save(Destination);
	return true;
}

void CMP4_STCO::save(FILE *Destination)
{
	// if source and destination position are identical and it is the same file, do nothing
	_position = _ftelli64(Destination);
	CMP4Atom::save(Destination);
}