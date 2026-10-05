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

// MonkeyTagInfo.cpp: implementation of class CMonkeyTagInfo.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonkeyTagInfo.h"
#include "Tools.h"
#include "Blob.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonkeyTagInfo::CMonkeyTagInfo()
{
	Reset();
}

CMonkeyTagInfo::~CMonkeyTagInfo()
{

}

// Monkey's Audio header (MAC SDK). Versions before 3.98: "MAC ", version, compression level, format flags, channels, sample rate, header bytes,
// terminating bytes, frames, blocks of the final frame; the peak level (32 bit) follows only with the flag 4 and the number of the seek elements
// (32 bit) only with the flag 16. From 3.98: a descriptor ("MAC ", version, padding, descriptor bytes, header bytes, seek table bytes, header data
// bytes, frame data bytes, high part, terminating bytes, MD5) and behind it, at the offset descriptor bytes, the header: compression level,
// format flags, blocks per frame, blocks of the final frame, frames, bits per sample, channels, sample rate.
bool CMonkeyTagInfo::ReadFromFile(CFile *Stream)
{
	const __int64 start = CTools::seqTell(Stream);
	CBlob tmp;
	tmp.FileRead(6, Stream);
	if (tmp.GetLength() != 6)
		return false;
	memcpy(ID, tmp.m_pData, 4);                                             /* Always "MAC " */
	if (!exists())
		return false;
	VersionID = tmp.GetR2B(4);                                             /* Version number * 1000 (3.91 = 3910) */
	if (VersionID < 3980)
	{
		tmp.FileRead(26, Stream);
		if (tmp.GetLength() != 26)
			return false;
		CompressionID = tmp.GetR2B(0);                                      /* Compression level code */
		Flags = tmp.GetR2B(2);                                              /* Any format flags */
		Channels = tmp.GetR2B(4);                                           /* Number of channels */
		SampleRate = tmp.GetR4B(6);                                         /* Sample rate (hz) */
		HeaderBytes = tmp.GetR4B(10);                                       /* Header length (without header ID) */
		TerminatingBytes = tmp.GetR4B(14);                                  /* Extended data */
		Frames = tmp.GetR4B(18);                                            /* Number of frames in the file */
		FinalSamples = tmp.GetR4B(22);                                      /* Number of samples in the final frame */
		FinalFrameBlocks = FinalSamples;
		if (Flags & 4)                                                      /* peak level (if stored) */
		{
			tmp.FileRead(4, Stream);
			if (tmp.GetLength() != 4)
				return false;
			PeakLevel = tmp.GetR4B(0);
		}
		if (Flags & 16)                                                     /* number of seek elements (if stored) */
		{
			tmp.FileRead(4, Stream);
			if (tmp.GetLength() != 4)
				return false;
			SeekElements = tmp.GetR4B(0);
		}
		FormatFlags = Flags;
		BitsPerSample = (Flags & 8) ? 24 : ((Flags & 1) ? 8 : 16);
		// the number of the blocks of a frame depends on the version and the compression level
		if (VersionID >= 3950)
			BlocksPerFrame = 73728l * 4l;
		else if (VersionID >= 3900 || (VersionID >= 3800 && CompressionID == 4000))
			BlocksPerFrame = 73728l;
		else
			BlocksPerFrame = 9216l;
	}
	else
	{
		// the descriptor
		tmp.FileRead(46, Stream);   // padding (2) and the rest of the descriptor (44)
		if (tmp.GetLength() != 46)
			return false;
		DescriptorBytes = tmp.GetR4B(2);
		HeaderBytes = tmp.GetR4B(6);
		SeekTableBytes = tmp.GetR4B(10);
		HeaderDataBytes = tmp.GetR4B(14);
		APEFrameDataBytes = tmp.GetR4B(18);
		APEFrameDataBytesHigh = tmp.GetR4B(22);
		TerminatingDataBytes = tmp.GetR4B(26);
		memcpy(FileMD5, tmp.m_pData + 30, 16);
		// the header follows the descriptor (the descriptor can be larger than 52 bytes)
		if (DescriptorBytes < 52)
			return false;
		CTools::seqSeek(Stream, start + DescriptorBytes);
		tmp.FileRead(24, Stream);
		if (tmp.GetLength() != 24)
			return false;
		CompressionID = tmp.GetR2B(0);
		FormatFlags = tmp.GetR2B(2);
		Flags = FormatFlags;
		BlocksPerFrame = tmp.GetR4B(4);
		FinalFrameBlocks = tmp.GetR4B(8);
		Frames = tmp.GetR4B(12);
		BitsPerSample = tmp.GetR2B(16);
		Channels = tmp.GetR2B(18);
		SampleRate = tmp.GetR4B(20);
	}
	return true;
}
void CMonkeyTagInfo::Reset()
{
	memset(ID,0, 4);
	VersionID = 0;
	CompressionID = 0;
	Flags = 0;
	Channels = 0;
	SampleRate = 0;
	HeaderBytes = 0;
	TerminatingBytes = 0;
	Frames = 0;
	FinalSamples = 0;
	PeakLevel = 0;
	SeekElements = 0;
	FormatFlags = 0;
	DescriptorBytes = 0;
	SeekTableBytes = 0;
	HeaderDataBytes = 0;
	APEFrameDataBytes = 0;
	APEFrameDataBytesHigh = 0;
	TerminatingDataBytes = 0;
	memset(FileMD5, 0, 16);
	BlocksPerFrame = 0;
	FinalFrameBlocks = 0;
	BitsPerSample = 0;
}
bool CMonkeyTagInfo::exists()
{
	return (memcmp(ID, MONKEY_TAG_ID, 4) == 0);
}
