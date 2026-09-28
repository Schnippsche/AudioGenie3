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
#include "mp4_mdhd.h"

class CMP4_SOUN:public CMP4Atom
{
public:
	CMP4_SOUN(void);
	~CMP4_SOUN(void);
	void load(FILE *stream, u64 offset, u64 size);
	int version;
	int revision, vendor, channels, sampleSize, compressionID, packetSize, sampleRate;
	int samplesPerPacket, bytesPerPacket, bytesPerFrame, bytesPerSample;
};

// Sample description box (stsd) of a track: the first sample entry of a sound track has the number of the channels and the sample rate
// (ISO/IEC 14496-12, AudioSampleEntry: 6 bytes reserved, data reference index, 8 bytes reserved, channel count, sample size, 4 bytes
// reserved, sample rate as 16.16 fixed point number)
class CMP4_STSD : public CMP4Atom
{
public:
	CMP4_STSD(void);
	~CMP4_STSD(void);
	void load(FILE *stream, u64 offset, u64 size);
	CMP4_MDHD *mdhd;		// media header of the track: it knows whether the track is a sound track
	int channels;
	long sampleRate;		// Hz, 0 if the entry does not have one
};
