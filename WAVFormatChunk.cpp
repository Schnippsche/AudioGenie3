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
#include "WAVFormatChunk.h"

CWAVFormatChunk::CWAVFormatChunk(void)
{
	_chunkID = 'fmt ';
	FormatID = 0;
	ChannelNumber = 0;
	SampleRate = 0;
	BytesPerSecond = 0;
	BlockAlign = 0;
	BitsPerSample = 0;
}

CWAVFormatChunk::~CWAVFormatChunk(void)
{
}

bool CWAVFormatChunk::load(FILE *Stream, u64 offset, u64 size)
{
	_data.FileReadAt(Stream, (__int64)offset, (size_t)size);
	if (_data.GetLength() < 16)
		return false;
	// in a RIFX file (big endian RIFF variant) these fields are big endian too, like every other size/number field in the file
	if (s_bigEndian)
	{
		FormatID = _data.Get2B(0);
		ChannelNumber = _data.Get2B(2);
		SampleRate = _data.Get4B(4);
		BytesPerSecond = _data.Get4B(8);
		BlockAlign = _data.Get2B(12);
		BitsPerSample = _data.Get2B(14);
	}
	else
	{
		FormatID = _data.GetR2B(0);
		ChannelNumber = _data.GetR2B(2);
		SampleRate = _data.GetR4B(4);
		BytesPerSecond = _data.GetR4B(8);
		BlockAlign = _data.GetR2B(12);
		BitsPerSample = _data.GetR2B(14);
	}
	return true;
}