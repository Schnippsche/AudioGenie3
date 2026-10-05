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
#include "WavPack.h"

// documentation at: http://www.wavpack.com/file_format.txt
//
// A WavPack file is a sequence of blocks. A block has a header of 32 bytes: "wvpk", size of the block (without the first 8 bytes), version (0x402 to
// 0x410), the upper 8 bits of the block index and of the total number of the samples (from version 5), total number of the samples (32 bit; valid in
// the first block; 0xFFFFFFFF: unknown), block index, number of the samples of the block, flags, CRC. The flags contain the bytes per sample (bits 0-1),
// the mono flag (bit 2), the initial and the final block of a frame (bits 11 and 12) and the sample rate index (bits 23-26). A stream with more than 2
// channels has several blocks (a channel pair or a channel each) for the same samples. After the header follow the metadata sub-blocks.

namespace {

const unsigned long FLAG_MONO = 0x4;
const unsigned long FLAG_FINAL_BLOCK = 0x1000;
const unsigned long RATE_MASK = 0xFUL << 23;
const int RATE_SHIFT = 23;
const int RATE_CUSTOM = 15;         // the sample rate is in a metadata sub-block
const BYTE ID_SAMPLE_RATE = 0x27;   // 3 bytes, little endian
const BYTE ID_LARGE = 0x80;         // the size of the sub-block has 3 bytes
const BYTE ID_ODD_SIZE = 0x40;      // the data have an odd number of bytes (one byte of padding)

struct BlockHeader
{
	unsigned long size;             // without the first 8 bytes
	unsigned int version;
	unsigned long totalLow, blockIndexLow, blockSamples, flags;
	BYTE totalHigh, indexHigh;
};

unsigned long Le32(const BYTE *p) { return (unsigned long)p[0] | ((unsigned long)p[1] << 8) | ((unsigned long)p[2] << 16) | ((unsigned long)p[3] << 24); }

bool ParseHeader(const BYTE *h, BlockHeader &b)
{
	if (memcmp(h, "wvpk", 4) != 0)
		return false;
	b.size = Le32(h + 4);
	b.version = h[8] | (h[9] << 8);
	b.indexHigh = h[10];
	b.totalHigh = h[11];
	b.totalLow = Le32(h + 12);
	b.blockIndexLow = Le32(h + 16);
	b.blockSamples = Le32(h + 20);
	b.flags = Le32(h + 24);
	// versions 0x402 to 0x410 can be decoded; a block has at least the rest of its header
	return b.version >= 0x402 && b.version <= 0x410 && b.size >= 24;
}

}  // namespace

CWavPack::CWavPack(void)
{
	ResetData();
}


CWavPack::~CWavPack(void)
{
}

void CWavPack::ResetData()
{
	/* Reset data */
	_header.Clear();
	_samples = 0;
	_majorversion = 0;
	_minorversion = 0;
	_flags = 0;
	_channels = 0;
	_sampleRate = 0;
}

bool CWavPack::IsValid()
{
	if (_header.GetLength() >= 32)
	{
		if (_header.GetAt(0) == 'w' && _header.GetAt(1) == 'v' && _header.GetAt(2) == 'p' && _header.GetAt(3) == 'k')
		{
			if (GetSampleRate() > 0 && _channels > 0)
			{
				return true;
			}
		}
	}
	return false;
}

CAtlString CWavPack::GetFileVersion()
{
	if (IsValid())
	{
		CAtlString tmp;
		tmp.Format(L"wavpack v%i.%i", _majorversion, _minorversion);
		return tmp;
	}
	return L"";
}

CAtlString CWavPack::GetChannelMode()
{
	/* Get channel mode */
	switch (_channels)
	{
	case 0:
		return UNKNOWN;
	case 1:
		return MONO;
	case 2:
		return STEREO;
	default:
		return MULTICHANNEL;
	}
}

float CWavPack::GetDuration()
{
	/* Get song duration */
	if (!IsValid())
		return 0.0f;
	long sampleRate = GetSampleRate();
	return (sampleRate > 0 && _samples > 0) ? (float)((double)_samples / sampleRate) : 0.0f;
}

long CWavPack::GetSampleRate()
{
	return _sampleRate;
}

long CWavPack::GetChannels()
{
	return _channels;
}

// The blocks of the first frame (up to the block with the final flag): the number of the channels (a mono block has 1 channel, another block 2)
// and the sample rate if it is in a sub-block. A frame has at most 255 blocks.
void CWavPack::ReadFrame(CFile *Stream, __int64 position)
{
	_channels = 0;
	for (int block = 0; block < 255; block++)
	{
		BYTE header[32];
		if (CTools::readAt(Stream, position, header, 32) != 32)
			return;
		BlockHeader b;
		if (!ParseHeader(header, b) || position + 8 + (__int64)b.size > CTools::FileSize)
			return;
		_channels += (b.flags & FLAG_MONO) ? 1 : 2;
		if (block == 0 && ((b.flags & RATE_MASK) >> RATE_SHIFT) == RATE_CUSTOM)
		{
			// the sample rate of a stream with a rate that is not in the table: a sub-block of the first block
			CBlob data;
			data.FileReadAt(Stream, position + 32, b.size - 24);
			size_t pos = 0;
			while (pos + 2 <= data.GetLength())
			{
				const BYTE id = data.GetAt(pos);
				size_t words, headerLength = 2;
				if (id & ID_LARGE)
				{
					if (pos + 4 > data.GetLength())
						break;
					words = data.GetAt(pos + 1) | (data.GetAt(pos + 2) << 8) | (data.GetAt(pos + 3) << 16);
					headerLength = 4;
				}
				else
					words = data.GetAt(pos + 1);
				size_t bytes = words * 2;
				if (id & ID_ODD_SIZE)
					bytes--;
				if (pos + headerLength + bytes > data.GetLength())
					break;
				if ((id & 0x3F) == ID_SAMPLE_RATE && bytes >= 3)
					_sampleRate = data.GetAt(pos + headerLength) | (data.GetAt(pos + headerLength + 1) << 8) | (data.GetAt(pos + headerLength + 2) << 16);
				pos += headerLength + words * 2;
			}
		}
		if (b.flags & FLAG_FINAL_BLOCK)
			return;
		position += 8 + (__int64)b.size;
	}
}

// The total number of the samples is unknown in a file that was written to a pipe: it is the end of the last block (block index + samples).
// The file is searched from its end for the last block header.
__int64 CWavPack::SamplesOfLastBlock(CFile *Stream)
{
	const long BLOCK = 256 * 1024;
	CBlob block;
	__int64 end = CTools::FileSize;
	const __int64 low = CTools::audioStart();
	// no more than 16 MB from the end (a block has at most that size)
	while (end > low && CTools::FileSize - end < 16 * 1024 * 1024)
	{
		__int64 start = end - BLOCK;
		if (start < low)
			start = low;
		block.FileReadAt(Stream, start, (size_t)(end - start) + 32);
		const long length = (long)block.GetLength();
		for (long i = (long)(end - start) - 1; i >= 0; i--)
		{
			if (block.m_pData[i] != 'w' || length - i < 32)
				continue;
			BlockHeader b;
			if (!ParseHeader(block.m_pData + i, b) || start + i + 8 + (__int64)b.size > CTools::FileSize)
				continue;
			const __int64 index = (__int64)b.blockIndexLow + ((__int64)b.indexHigh << 32);
			return index + b.blockSamples;
		}
		if (start <= low)
			break;
		end = start;
	}
	return 0;
}

bool CWavPack::ReadFromFile(CFile *Stream)
{
	/* Read header data */
	ResetData();
	_header.FileReadAt(Stream, CTools::audioStart(), 32);
	BlockHeader b;
	if (_header.GetLength() == 32 && ParseHeader(_header.m_pData, b))
	{
		/*
		0 char ckID [4];              // "wvpk"
		4 uint32_t ckSize;            // size of entire block (minus 8, of course)
		8 uint16_t version;           // 0x402 to 0x410 are currently valid for decode
		10 uchar block_index_u8;      // upper 8 bits of the block index (version 5)
		11 uchar total_samples_u8;    // upper 8 bits of the total samples (version 5)
		12 uint32_t total_samples;    // total samples for entire file, but this is only valid if block_index == 0 and a value of -1 indicates
		                              // unknown length
		16 uint32_t block_index;      // index of first sample in block relative to beginning of file
		20 uint32_t block_samples;    // number of samples in this block (0 = no audio)
		24 uint32_t flags;            // various flags for id and decoding
		28 uint32_t crc;              // crc for actual decoded data */
		_majorversion = _header.GetAt(9);
		_minorversion = _header.GetAt(8);
		_flags = (long)b.flags;
		// the counter has 40 bits from version 5; 0xFFFFFFFF in the lower part is still "unknown"
		if (b.totalLow != 0xFFFFFFFFUL)
			_samples = (__int64)b.totalLow + ((__int64)b.totalHigh << 32) - b.totalHigh;
		const size_t index = (size_t)((b.flags & RATE_MASK) >> RATE_SHIFT);
		if (index < sizeof(sample_rates) / sizeof(sample_rates[0]))   // index 15 is a rate in a sub-block
			_sampleRate = (long)sample_rates[index];
		ReadFrame(Stream, CTools::audioStart());
		if (_samples <= 0)
			_samples = SamplesOfLastBlock(Stream);
		return true;
	}
	ResetData();
	return false;
}

