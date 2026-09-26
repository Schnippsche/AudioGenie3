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

#include "stdafx.h"
#include "aac.h"
#include "io.h"
#include "Blob.h"
#include <fcntl.h>
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAAC::CAAC()
{

}

CAAC::~CAAC()
{
}

// AAC files without container: ADTS (a header in front of every frame) and ADIF (one header at the start of the file), both defined in
// ISO/IEC 13818-7 and ISO/IEC 14496-3.

namespace {

// number of the channels for the channel configuration of ADTS (0: the program config element defines them, 7: 7.1)
const BYTE CHANNELS_OF_CONFIG[8] = { 0, 1, 2, 3, 4, 5, 6, 8 };

// reads bits from a buffer (most significant bit first); reading behind the end gives zero bits and sets the overflow flag
struct BitReader
{
	const BYTE *data;
	size_t length;
	size_t position;
	bool overflow;
	BitReader(const BYTE *d, size_t n, size_t start = 0) : data(d), length(n), position(start), overflow(false) {}
	unsigned long Get(int count)
	{
		unsigned long result = 0;
		for (int i = 0; i < count; i++)
		{
			if (position >= length * 8)
			{
				overflow = true;
				result <<= 1;
			}
			else
				result = (result << 1) | ((data[position / 8] >> (7 - position % 8)) & 1);
			position++;
		}
		return result;
	}
};

struct ProgramConfig
{
	int profile;          // object type - 1 (0 = Main, 1 = LC, 2 = SSR, 3 = LTP)
	int sampleRateIndex;
	int channels;
};

// program_config_element() (ISO/IEC 14496-3, table 4.2): the channels are counted from the lists of the elements, a channel pair element
// (is_cpe) has 2 channels; the coupling channels and the data elements are no audio channels of the output
bool ReadProgramConfig(BitReader &bits, ProgramConfig &pce)
{
	bits.Get(4);   // element_instance_tag
	pce.profile = (int)bits.Get(2);
	pce.sampleRateIndex = (int)bits.Get(4);
	const int front = (int)bits.Get(4);
	const int side = (int)bits.Get(4);
	const int back = (int)bits.Get(4);
	const int lfe = (int)bits.Get(2);
	bits.Get(3);   // number of assoc data elements
	bits.Get(4);   // number of valid coupling channels
	if (bits.Get(1))
		bits.Get(4);   // mono mixdown
	if (bits.Get(1))
		bits.Get(4);   // stereo mixdown
	if (bits.Get(1))
		bits.Get(3);   // matrix mixdown
	pce.channels = lfe;
	const int counts[3] = { front, side, back };
	for (int list = 0; list < 3; list++)
		for (int i = 0; i < counts[list]; i++)
		{
			const bool pair = bits.Get(1) != 0;   // is_cpe
			bits.Get(4);                          // tag
			pce.channels += pair ? 2 : 1;
		}
	return !bits.overflow && pce.channels > 0;
}

// the fields of an ADTS header (7 bytes, 9 with the checksum)
struct AdtsHeader
{
	int id;                // 0: MPEG-4, 1: MPEG-2
	int profile;           // object type - 1
	int sampleRateIndex;
	int channelConfig;
	long frameLength;      // with the header
	int headerLength;
	int blocks;            // number of raw data blocks in the frame (each has 1024 samples)
	bool variableRate;     // buffer fullness 0x7FF
};

bool ParseAdtsHeader(const BYTE *h, size_t available, AdtsHeader &f)
{
	if (available < 7)
		return false;
	// syncword 12 bits, layer 00 (else it is an MPEG audio header of layer 1 to 3)
	if (h[0] != 0xFF || (h[1] & 0xF0) != 0xF0 || (h[1] & 0x06) != 0)
		return false;
	f.id = (h[1] >> 3) & 1;
	f.headerLength = (h[1] & 1) ? 7 : 9;   // protection_absent: without checksum
	f.profile = (h[2] >> 6) & 3;
	f.sampleRateIndex = (h[2] >> 2) & 0x0F;
	if (SAMPLE_RATE[f.sampleRateIndex] == 0)
		return false;
	f.channelConfig = ((h[2] & 1) << 2) | (h[3] >> 6);
	f.frameLength = ((h[3] & 3) << 11) | (h[4] << 3) | (h[5] >> 5);
	f.variableRate = (((h[5] & 0x1F) << 6) | (h[6] >> 2)) == 0x7FF;
	f.blocks = (h[6] & 3) + 1;
	return f.frameLength >= f.headerLength;
}

// reads a file in blocks: At() gives a pointer to the bytes at a position of the file (NULL behind the end)
class FileWindow
{
	FILE *file;
	__int64 start;
	size_t length;
	BYTE buffer[64 * 1024];
public:
	FileWindow(FILE *f) : file(f), start(0), length(0) {}
	const BYTE* At(__int64 position, size_t need, size_t &available)
	{
		if (position < start || position + (__int64)need > start + (__int64)length)
		{
			_fseeki64(file, position, SEEK_SET);
			start = position;
			length = fread(buffer, 1, sizeof(buffer), file);
		}
		if (length == 0 || position >= start + (__int64)length)
		{
			available = 0;
			return NULL;
		}
		available = (size_t)(start + length - position);
		return buffer + (position - start);
	}
};

}  // namespace

/* ********************** Private functions & procedures ********************* */

void CAAC::ResetData()
{
	/* Reset all variables */
	FHeaderTypeID = AAC_HEADER_TYPE_UNKNOWN;
	FMPEGVersionID = AAC_MPEG_VERSION_UNKNOWN;
	FProfileID = AAC_PROFILE_UNKNOWN;
	FChannels = 0;
	FSampleRate = 0;
	FBitRate = 0;
	FBitRateTypeID = AAC_BITRATE_TYPE_UNKNOWN;
	FDuration = 0.0;
}

CAtlString CAAC::GetHeaderType()
{
	/* Get header type name */
	return AAC_HEADER_TYPE[FHeaderTypeID];
}

CAtlString CAAC::GetFileVersion()
{
	/* Get MPEG version name */
	return AAC_MPEG_VERSION[FMPEGVersionID];
}

CAtlString CAAC::GetProfile()
{
	/* Get profile name */
	return AAC_PROFILE[FProfileID];
}

CAtlString CAAC::GetBitRateType()
{
	/* Get bit rate type name */
	return AAC_BITRATE_TYPE[FBitRateTypeID];
}

float CAAC::GetDuration()
{
	/* ADTS: the sum of the frames */
	if (FHeaderTypeID == AAC_HEADER_TYPE_ADTS)
		return (float)FDuration;
	/* ADIF has no frame headers: the duration is estimated from the bit rate of the header and the size of the audio data (without the
	   tags at the end of the file) */
	if (FBitRate == 0)
		return 0.0f;
	const __int64 audio = CTools::FileSize - CTools::audioStart() - CTools::ID3v1Size - CTools::APESize - CTools::LyricsSize;
	return audio > 0 ? (float)(8.0 * (double)audio / FBitRate) : 0.0f;
}

bool CAAC::IsValid()
{
	/* Check for file correctness */
	return (FHeaderTypeID != AAC_HEADER_TYPE_UNKNOWN &&
		FChannels > 0 &&
		FSampleRate > 0 &&
		FBitRate > 0);
}

BYTE CAAC::RecognizeHeaderType(FILE *Source)
{
	BYTE Header[9] = { 0 };
	/* Get header type of the file */
	_fseeki64(Source, CTools::audioStart(), SEEK_SET);
	const size_t count = fread(Header, 1, sizeof(Header), Source);
	if (count >= 4 && memcmp(Header, ADIF, 4) == 0)
		return AAC_HEADER_TYPE_ADIF;
	AdtsHeader adts;
	if (ParseAdtsHeader(Header, count, adts))
		return AAC_HEADER_TYPE_ADTS;
	return AAC_HEADER_TYPE_UNKNOWN;
}

// ADIF: "ADIF", copyright_id_present (1 bit, then 72 bits), original_copy, home, bitstream_type (0: constant rate, 1: variable rate),
// bitrate (23 bits), number_of_program_config_elements - 1 (4 bits), the buffer fullness (20 bits, only with a constant rate), then the
// program config elements
bool CAAC::ReadADIF(FILE *Source)
{
	BYTE header[512];
	_fseeki64(Source, CTools::audioStart(), SEEK_SET);
	const size_t count = fread(header, 1, sizeof(header), Source);
	BitReader bits(header, count, 32);
	if (bits.Get(1))
		bits.Get(24), bits.Get(24), bits.Get(24);   // copyright_id, 72 bits
	bits.Get(2);                                      // original_copy, home
	const bool variable = bits.Get(1) != 0;
	FBitRate = (long)bits.Get(23);
	bits.Get(4);
	if (!variable)
		bits.Get(20);
	ProgramConfig pce = { 0, 0, 0 };
	if (!ReadProgramConfig(bits, pce))
		return false;
	FBitRateTypeID = variable ? AAC_BITRATE_TYPE_VBR : AAC_BITRATE_TYPE_CBR;
	FMPEGVersionID = AAC_MPEG_VERSION_4;
	FProfileID = (BYTE)(pce.profile + 1);
	FSampleRate = SAMPLE_RATE[pce.sampleRateIndex];
	FChannels = (BYTE)pce.channels;
	return FSampleRate > 0;
}

// ADTS: all frames are counted, the duration is the number of the samples (1024 per raw data block) divided by the sample rate. After
// damaged data the next frame is looked for: it has to be followed by a frame or by the end of the file.
bool CAAC::ReadADTS(FILE *Source)
{
	FileWindow window(Source);
	const __int64 end = CTools::FileSize;
	__int64 position = CTools::audioStart();
	__int64 audioBytes = 0;
	long frames = 0;
	int rateIndex = -1;      // the sample rate index of the stream (all frames have the same)
	bool variable = false;
	// a frame at the position: valid header, the frame is inside the file, the sample rate of the stream
	auto frameAt = [&](__int64 pos, AdtsHeader &f) -> bool
	{
		size_t available;
		const BYTE *p = window.At(pos, 9, available);
		return p != NULL && ParseAdtsHeader(p, available, f) && pos + f.frameLength <= end && (rateIndex < 0 || f.sampleRateIndex == rateIndex);
	};
	while (position + 7 <= end)
	{
		AdtsHeader f, next;
		if (!frameAt(position, f))
		{
			if (rateIndex < 0)
				return false;   // the file has to start with a frame
			bool found = false;
			while (!found && ++position + 7 <= end)
				found = frameAt(position, f) && (position + f.frameLength == end || frameAt(position + f.frameLength, next));
			if (!found)
				break;
		}
		if (rateIndex < 0)
		{
			rateIndex = f.sampleRateIndex;
			FMPEGVersionID = f.id ? AAC_MPEG_VERSION_2 : AAC_MPEG_VERSION_4;
			FProfileID = (BYTE)(f.profile + 1);
			FSampleRate = SAMPLE_RATE[rateIndex];
			FChannels = CHANNELS_OF_CONFIG[f.channelConfig];
			// channel configuration 0: the program config element at the start of the first raw data block (without the block positions
			// that a frame with several raw data blocks has)
			if (f.channelConfig == 0 && f.blocks == 1)
			{
				BYTE raw[256];
				_fseeki64(Source, position + f.headerLength, SEEK_SET);
				BitReader bits(raw, fread(raw, 1, sizeof(raw), Source));
				ProgramConfig pce = { 0, 0, 0 };
				if (bits.Get(3) == 5 && ReadProgramConfig(bits, pce))   // id_syn_ele: ID_PCE
					FChannels = (BYTE)pce.channels;
			}
		}
		if (f.variableRate)
			variable = true;
		frames++;
		audioBytes += f.frameLength;
		FDuration += 1024.0 * f.blocks / SAMPLE_RATE[f.sampleRateIndex];
		position += f.frameLength;
	}
	if (frames == 0)
		return false;
	FBitRateTypeID = variable ? AAC_BITRATE_TYPE_VBR : AAC_BITRATE_TYPE_CBR;
	if (FDuration > 0)
		FBitRate = (long)(8.0 * audioBytes / FDuration + 0.5);
	return true;
}

bool CAAC::ReadFromFile(FILE *Source)
{
	/* Read data from file */
	ResetData();
	FHeaderTypeID = RecognizeHeaderType(Source);
	/* Read header data */
	if (FHeaderTypeID == AAC_HEADER_TYPE_ADIF)
		return ReadADIF(Source) && IsValid();
	if (FHeaderTypeID == AAC_HEADER_TYPE_ADTS)
		return ReadADTS(Source) && IsValid();   // ADTS looks almost like MPEG, so validate further
	ResetData();
	return false; // not an AAC file
}
