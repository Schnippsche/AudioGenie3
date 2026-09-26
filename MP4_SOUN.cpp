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
#include "MP4_SOUN.h"
#include "mp4_atomfactory.h"

CMP4_SOUN::CMP4_SOUN(void)
{
	setFrameID(MP4_SOUN);
	version = 0;
	revision = 0;
	vendor = 0;
	channels = 0;
	sampleSize = 0;
	compressionID = 0;
	packetSize = 0;
	sampleRate = 0;
	samplesPerPacket = 0;
	bytesPerPacket = 0;
	bytesPerFrame = 0;
	bytesPerSample = 0;
}

CMP4_SOUN::~CMP4_SOUN(void)
{
}

void CMP4_SOUN::load(FILE *Stream, u64 offset, u64 size)
{
	offset;
	_blob.FileRead((size_t)size, Stream);
	if (_blob.GetLength() >= 20)
	{
		version = _blob.Get2B(0); // usually version 0
		revision = _blob.Get2B(2);
		vendor = _blob.Get4B(4);
		channels = _blob.Get2B(8);
		sampleSize = _blob.Get2B(10);
		compressionID = _blob.Get2B(12);
		packetSize = _blob.Get2B(14);
		sampleRate = _blob.Get4B(16);
		if (version == 1 && _blob.GetLength() >= 36) // Version 1
		{
			samplesPerPacket = _blob.Get4B(20);
			bytesPerPacket = _blob.Get4B(24);
			bytesPerFrame = _blob.Get4B(28);
			bytesPerSample = _blob.Get4B(32);
		}
		else
		{
			samplesPerPacket = 0;
			bytesPerPacket = 0;
			bytesPerFrame = 0;
			bytesPerSample = 0;
		}
	}
}

CMP4_STSD::CMP4_STSD(void)
{
	setFrameID(MP4_STSD);
	mdhd = NULL;
	channels = 0;
	sampleRate = 0;
}

CMP4_STSD::~CMP4_STSD(void)
{
}

void CMP4_STSD::load(FILE *Stream, u64 offset, u64 size)
{
	offset;
	_blob.FileRead((size_t)size, Stream);
	channels = 0;
	sampleRate = 0;
	// version and flags (4), number of entries (4), then the first entry: size (4), format (4), 6 reserved bytes, data reference index (2),
	// version (2), revision (2), vendor (4), channel count (2), sample size (2), compression ID (2), packet size (2), sample rate (4)
	if (_blob.GetLength() >= 8 + 36 && _blob.Get4B(4) >= 1)
	{
		const size_t entry = 8;
		const u32 entrySize = _blob.Get4B(entry);
		const WORD entryVersion = _blob.Get2B(entry + 16);
		if (entrySize >= 36 && entryVersion < 2)   // version 2 (QuickTime) has another layout
		{
			channels = _blob.Get2B(entry + 24);
			sampleRate = (long)((u32)_blob.Get4B(entry + 32) >> 16);   // the integer part of the 16.16 number
		}
	}
}
