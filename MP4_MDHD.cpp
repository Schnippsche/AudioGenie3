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
#include "MP4_MDHD.h"
#include "mp4_atomfactory.h"

CMP4_MDHD::CMP4_MDHD(void)
{
	setFrameID(MP4_MDHD);
	isSoundAtom = false;
}

CMP4_MDHD::~CMP4_MDHD(void)
{
}

void CMP4_MDHD::load(FILE *Stream, u64 offset, u64 size)
{
	offset;
	_blob.FileRead((size_t)size, Stream);
	version = 0;
	timeScale = 0;
	duration = 0;
	// version 0: 32 bit times and duration; version 1: 64 bit times and duration
	if (_blob.GetLength() >= 1 && _blob.GetAt(0) == 1)
	{
		if (_blob.GetLength() >= 36)
		{
			version = 1;
			flags = _blob.Get3B(1);
			creationTime = ((u64)_blob.Get4B(4) << 32) | _blob.Get4B(8);
			modifyTime = ((u64)_blob.Get4B(12) << 32) | _blob.Get4B(16);
			timeScale = _blob.Get4B(20);
			duration = ((u64)_blob.Get4B(24) << 32) | _blob.Get4B(28);
			language = _blob.Get2B(32);
			quality = _blob.Get2B(34);
		}
	}
	else if (_blob.GetLength() >= 24)
	{
		version = _blob.GetAt(0);
		flags = _blob.Get3B(1);
		creationTime = _blob.Get4B(4);
		modifyTime = _blob.Get4B(8);
		timeScale = _blob.Get4B(12);
		duration = _blob.Get4B(16);
		language = _blob.Get2B(20);
		quality = _blob.Get2B(22);
	}
}
