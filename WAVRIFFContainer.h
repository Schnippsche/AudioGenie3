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
#include "wavcontainer.h"
#include "wavformatchunk.h"
#include "wavcartchunk.h"
#include "WAVDISPChunk.h"
#include "WAVBEXTChunk.h"

class CWAVRIFFContainer :
	public CWAVContainer
{
public:
	CWAVRIFFContainer(void);
	~CWAVRIFFContainer(void);
	// the RIFF chunk at offset; endPos: the end of the chunks (in front of an ID3v1 tag at the end of the file)
	bool load(CFile *Stream, u64 offset, u64 endPos);
	void save(CBlob *blob);
	bool save(CFile *Source, CFile *Destination);
	CWAVFormatChunk* getFormatChunk() { return formatChunk; };
	void Remove();
	CWAVCARTChunk* addCartChunk();
	CWAVCARTChunk* getCartChunk();	
	void addInfoChunk(u32 FrameID, CAtlString newText);
	CWAVContainer* getInfoChunk();
	CWAVDISPChunk* addDispChunk();
	CWAVDISPChunk* getDispChunk();
	CWAVBEXTChunk* addBextChunk();
	CWAVBEXTChunk* getBextChunk();	
	// the position behind the last chunk that was read: the bytes from there to the end (data behind the RIFF chunk, a chunk that does not fit
	// into the file) are not chunks of the file and are copied unchanged when it is saved
	u64 getTailStart() { return _tailStart; };
	// false if a chunk could not be read into memory: it would be missing in a saved file
	bool isComplete() { return _complete; };
	// replaces the tag chunks (LIST INFO, cart, bext, DISP with text) by copies of those of other; the other chunks stay
	void takeTagChunks(CWAVRIFFContainer &other);
private:
	CWAVFormatChunk *formatChunk;
	bool _isRF64;             // the file was read as RF64 (EBU Tech 3306, for files of 4 GB or more) or has grown into one
	bool _isRIFX;             // the file was read as RIFX (big endian RIFF variant used by old Mac/SGI tools); kept once detected
	u64 _sampleCount64;       // the sample count of the 'ds64' chunk (only meaningful with a 'fact' chunk; kept as it was read)
	u64 _tailStart;           // see getTailStart
	bool _complete;           // see isComplete
	// makes sure a 'ds64' chunk with the current sizes exists if the file needs to be RF64; returns whether it does
	bool needsRF64(u64 &totalSize);
};
