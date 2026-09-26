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
#include "stdio.h"
#include "io.h"
#include "flac.h"
#include "FlacCover.h"
#include "Blob.h"
#include <share.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFLAC::CFLAC()
{
	covers.SetCount(0, 2);
	ResetData();
}

CFLAC::~CFLAC()
{
	DeletePictures();
}

void CFLAC::ResetData()
{
	/* Reset data */
	Channels = 0;
	SampleRate = 0;
	BitsPerSample = 0;
	Samples = 0;
	minBlockSize = 0;
	maxBlockSize = 0;
	minFrameSize = 0;
	maxFrameSize = 0;
	oldLen = 0;
	anzComments = 0;
	metadataComplete = false;
	commentRead = false;
	BlockComment.Clear();
	BlockOther.Clear();
	BlockStreamInfo.Clear();
	Daten.Clear();
	BlockHeader.lastBlock = false;
	BlockHeader.Type = 0;
	BlockHeader.Size = 0;
	firstAudioPosition = 0;
	CVorbisComment::ResetData();
	DeletePictures();
}

bool CFLAC::IsValid()
{
	/* Check for right FLAC's Audio file data */
	return (Channels > 0 && SampleRate > 0 && BitsPerSample > 0 && Samples > 0);
}

float CFLAC::GetDuration()
{
	/* Get song duration */
	if (IsValid())
		return (float)((double)Samples / (double)SampleRate);

	return 0.0f;
}

float CFLAC::GetRatio()
{
	/* Get compression ratio */
	if (IsValid())
		return (float)((double)CTools::FileSize / ((double)Samples * Channels * BitsPerSample / 8.0) * 100.0);

	return 0.0f;
}

long CFLAC::GetBitRate()
{
	// the bytes of the frames (without the metadata and the tags at the start and at the end) in kbit/s
	const double duration = GetDuration();
	const __int64 audio = CTools::FileSize - CTools::ID3v2Size - oldLen - CTools::ID3v1Size - CTools::APESize - CTools::LyricsSize;
	if (duration > 0 && audio > 0)
		return (long) ((double)audio / 125.0 / duration + .5);
	return 0;
}

// CRC-8 of the frame header (polynomial x^8 + x^2 + x + 1)
static BYTE FrameHeaderCrc(const BYTE *data, size_t length)
{
	BYTE crc = 0;
	for (size_t i = 0; i < length; i++)
	{
		crc ^= data[i];
		for (int k = 0; k < 8; k++)
			crc = (crc & 0x80) ? (BYTE)((crc << 1) ^ 0x07) : (BYTE)(crc << 1);
	}
	return crc;
}

// Frame header (FLAC format specification): sync code 11111111111110 (14 bits), reserved (0), blocking strategy (0 fixed, 1 variable),
// block size code (4 bits), sample rate code (4), channel assignment (4), sample size code (3), reserved (0), the frame or sample number
// (UTF-8 coded, 1 to 7 bytes), the block size and the sample rate if the codes say so, the CRC-8 of the header.
// Returns the number of the first sample of the frame and its block size.
static bool ParseFrameHeader(const BYTE *h, size_t available, int channels, int bitsPerSample, long sampleRate, int fixedBlockSize, __int64 &firstSample, long &blockSize)
{
	if (available < 6 || h[0] != 0xFF || (h[1] & 0xFE) != 0xF8)
		return false;
	const bool variable = (h[1] & 1) != 0;
	const int sizeCode = h[2] >> 4, rateCode = h[2] & 0x0F;
	const int assignment = h[3] >> 4, sampleSizeCode = (h[3] >> 1) & 7;
	if ((h[3] & 1) || sizeCode == 0 || rateCode == 15 || sampleSizeCode == 3 || sampleSizeCode == 7 || assignment > 10)
		return false;
	// the channels and the sample size have to be those of the stream
	if (assignment < 8 && assignment + 1 != channels)
		return false;
	if (assignment >= 8 && channels != 2)
		return false;
	static const int SAMPLE_SIZES[8] = { 0, 8, 12, 0, 16, 20, 24, 32 };
	if (sampleSizeCode != 0 && SAMPLE_SIZES[sampleSizeCode] != bitsPerSample)
		return false;
	// UTF-8 coded number
	size_t pos = 4;
	const BYTE lead = h[pos];
	int extra;
	unsigned __int64 number;
	if (lead < 0x80) { extra = 0; number = lead; }
	else if ((lead & 0xE0) == 0xC0) { extra = 1; number = lead & 0x1F; }
	else if ((lead & 0xF0) == 0xE0) { extra = 2; number = lead & 0x0F; }
	else if ((lead & 0xF8) == 0xF0) { extra = 3; number = lead & 0x07; }
	else if ((lead & 0xFC) == 0xF8) { extra = 4; number = lead & 0x03; }
	else if ((lead & 0xFE) == 0xFC) { extra = 5; number = lead & 0x01; }
	else if (lead == 0xFE) { extra = 6; number = 0; }
	else return false;
	if (available < pos + 1 + extra + 4)
		return false;
	for (int i = 1; i <= extra; i++)
	{
		if ((h[pos + i] & 0xC0) != 0x80)
			return false;
		number = (number << 6) | (h[pos + i] & 0x3F);
	}
	pos += 1 + extra;
	if (sizeCode == 1) blockSize = 192;
	else if (sizeCode >= 2 && sizeCode <= 5) blockSize = 576L << (sizeCode - 2);
	else if (sizeCode == 6) { blockSize = h[pos] + 1; pos += 1; }
	else if (sizeCode == 7) { blockSize = ((h[pos] << 8) | h[pos + 1]) + 1; pos += 2; }
	else blockSize = 256L << (sizeCode - 8);
	if (rateCode == 12) pos += 1;
	else if (rateCode == 13 || rateCode == 14) pos += 2;
	if (available < pos + 1 || FrameHeaderCrc(h, pos) != h[pos])
		return false;
	if (variable)
		firstSample = (__int64)number;
	else
		firstSample = (__int64)number * fixedBlockSize;   // a fixed block size: the frame number
	(void)sampleRate;
	return true;
}

// The total number of the samples is unknown (0) in the STREAMINFO of a stream that was written to a pipe. It is the end of the last
// frame: the file is searched from its end for the last frame header.
__int64 CFLAC::SamplesOfLastFrame(FILE *Stream)
{
	const __int64 low = firstAudioPosition;
	const long BLOCK = 256 * 1024;
	CBlob block;
	__int64 end = CTools::FileSize;
	// no more than 8 MB from the end (the last frame is not larger than the maximal frame size, but this value is often 0)
	while (end > low && CTools::FileSize - end < 8 * 1024 * 1024)
	{
		__int64 start = end - BLOCK;
		if (start < low)
			start = low;
		_fseeki64(Stream, start, SEEK_SET);
		block.FileRead((size_t)(end - start) + 32, Stream);
		const long length = (long)block.GetLength();
		for (long i = (long)(end - start) - 1; i >= 0; i--)
		{
			if (block.m_pData[i] != 0xFF)
				continue;
			__int64 first;
			long size;
			if (ParseFrameHeader(block.m_pData + i, (size_t)(length - i), Channels, BitsPerSample, SampleRate, maxBlockSize, first, size))
				return first + size;
		}
		if (start <= low)
			break;
		end = start;
	}
	return 0;
}

bool CFLAC::ReadFromFile(FILE *Stream)
{
	/* Read FLAC's Audio header data */
	ResetData();
	_fseeki64(Stream, CTools::ID3v2Size, SEEK_SET);
	memset(tmpHdr, 0, 4);
	if (fread(tmpHdr, 1, 4, Stream) == 4 && memcmp(tmpHdr, FLAC_ID, 4) == 0)
	{
		// metadata blocks: the first one is the STREAMINFO, the last one has the flag in its header
		bool first = true;
		bool ok = true;
		while (ok && !BlockHeader.lastBlock)
		{
			ok = ReadBlock(Stream, first);
			first = false;
		}
		if (!ok && Samples == 0 && SampleRate == 0)
		{
			ResetData();
			return false;   // no STREAMINFO
		}
		metadataComplete = ok;
		firstAudioPosition = _ftelli64(Stream);
		oldLen = (long)(firstAudioPosition - CTools::ID3v2Size);
		if (ok && Samples == 0 && SampleRate > 0)
			Samples = SamplesOfLastFrame(Stream);
		return true;
	}
	ResetData();
	return false;
}

// the header of a metadata block: last-metadata-block flag (1 bit), block type (7 bits, 127 is invalid), length (24 bits, without the header)
bool CFLAC::ReadBlockHeader(FILE *Stream)
{
	memset(tmpHdr, 0, 4);
	if (fread(tmpHdr, 1, 4, Stream) != 4)
	{
		BlockHeader.lastBlock = true;   // the file ends inside the metadata
		return false;
	}
	BlockHeader.lastBlock = ((tmpHdr[0] & 0x80) == 0x80);
	// clear last flag
	tmpHdr[0] &= 0x7F;
	BlockHeader.Type = tmpHdr[0];
	BlockHeader.Size = (tmpHdr[1] << 16) + (tmpHdr[2] << 8) + tmpHdr[3];
	return BlockHeader.Type != 127;
}

void CFLAC::BuildBlockHeader(int Len, BYTE typ)
{
	tmpHdr[0] = typ;
	tmpHdr[1] = (BYTE) (Len >> 16);
	tmpHdr[2] = (BYTE) (Len >> 8);
	tmpHdr[3] = (BYTE) (Len & 0xFF);
}

// false if the block is damaged: the header is invalid or the block reaches over the end of the file (a block of length 0 is valid)
bool CFLAC::ReadBlock(FILE *Stream, bool first)
{
	if (!ReadBlockHeader(Stream))
	{
		BlockHeader.lastBlock = true;
		return false;
	}
	const int size = BlockHeader.Size;
	if (_ftelli64(Stream) + size > CTools::FileSize)
	{
		BlockHeader.lastBlock = true;
		return false;
	}
	if (first && BlockHeader.Type != METADATA_BLOCK_STREAMINFO)
	{
		BlockHeader.lastBlock = true;   // the STREAMINFO block has to be the first block
		return false;
	}
	ATLTRACE(L"Block:%i  size:%i\n", BlockHeader.Type, size);
	switch (BlockHeader.Type)
	{
	case METADATA_BLOCK_STREAMINFO:
		if (!first || size != 34)
		{
			BlockHeader.lastBlock = true;
			return false;
		}
		BlockStreamInfo.FileRead(size, Stream);
		AnalyzeStreamInfo();
		break;
	case METADATA_BLOCK_PADDING:
		_fseeki64(Stream, size, SEEK_CUR);
		break;
	case METADATA_BLOCK_COMMENT:
		{
			// the Vorbis comments of the block (no framing bit); only the first block counts
			CBlob comments;
			comments.FileRead(size, Stream);
			if (!commentRead)
				AnalyzeVorbisComments(comments.m_pData, comments.GetLength());
			commentRead = true;
		}
		break;
	case METADATA_BLOCK_PICTURE:
		BlockCover.Clear();
		BlockCover.FileRead(size, Stream);
		if (BlockCover.GetLength() > 32)
			covers.Add(new CFlacCover(&BlockCover));
		break;
	default:
		// seek table, application, cue sheet and the block types that are not known: written back as they are
		Daten.Clear();
		Daten.FileRead(size, Stream);
		BlockOther.AddMemory(tmpHdr, 4);
		BlockOther.AddBlob(Daten);
	};
	return true;
}

// STREAMINFO: minimal and maximal block size (16 bit each), minimal and maximal frame size (24 bit each), sample rate (20 bits), channels - 1 (3),
// bits per sample - 1 (5), the total number of samples (36 bits, 0 if unknown), MD5 of the audio data (128 bits)
void CFLAC::AnalyzeStreamInfo()
{
	minBlockSize = BlockStreamInfo.Get2B(0);
	maxBlockSize = BlockStreamInfo.Get2B(2);
	minFrameSize = (long)BlockStreamInfo.Get3B(4);
	maxFrameSize = (long)BlockStreamInfo.Get3B(7);
	SampleRate = (long)(BlockStreamInfo.GetAt(10) << 12) + (BlockStreamInfo.GetAt(11) << 4) + (BlockStreamInfo.GetAt(12) >> 4);
	Channels = ((BlockStreamInfo.GetAt(12) >> 1) & 0x7) + 1;
	BitsPerSample = ((BlockStreamInfo.GetAt(12) & 1) << 4) + (BlockStreamInfo.GetAt(13) >> 4) + 1;
	Samples = ((__int64)(BlockStreamInfo.GetAt(13) & 0x0F) << 32) + (unsigned long)BlockStreamInfo.Get4B(14);
}

void CFLAC::BuildComment()
{
	int Len = 0;
	anzComments = 0;
	BlockComment.Clear();
	neu.Clear();
	BuildVorbisComments(neu);
	Len = (long)neu.GetLength();
	BuildBlockHeader(Len, METADATA_BLOCK_COMMENT);
	BlockComment.AddMemory(tmpHdr, 4);
	BlockComment.AddBlob(neu);
}

// false if a block does not fit into the 24 bit length of the block header
bool CFLAC::BuildFrame(bool withComment)
{
	int Len = 0, padding = 0;
	if (withComment)
		BuildComment();
	else
		BlockComment.Clear();
	if (BlockComment.GetLength() > 0xFFFFFF + 4)
		return false;
	// reassemble the whole block
	// first fLaC, then StreamInfo, then Comment, then Pictures, then the rest
	Daten.Clear();
	Daten.AddMemory(FLAC_ID, 4);
	BuildBlockHeader((int)BlockStreamInfo.GetLength(), METADATA_BLOCK_STREAMINFO);
	Daten.AddMemory(tmpHdr, 4);
	Daten.AddBlob(BlockStreamInfo);
	Daten.AddBlob(BlockComment);

	for (size_t i = 0; i < covers.GetCount(); i++)
	{
		BlockCover.Clear();
		covers[i]->Save(&BlockCover);
		if (BlockCover.GetLength() > 0xFFFFFF)
			return false;
		BuildBlockHeader((int)BlockCover.GetLength(), METADATA_BLOCK_PICTURE);
		Daten.AddMemory(tmpHdr, 4);
		Daten.AddBlob(BlockCover);
	}

	Daten.AddBlob(BlockOther);
	Len = (int)Daten.GetLength();
	// the padding block (4 bytes header, it may be empty) fills the old size of the metadata; without an old size that is big enough or
	// with a padding of more than the 24 bit of the length the file is rebuilt with a small padding
	padding = (int)oldLen - Len - 4;
	if (padding >= 0 && padding <= 0xFFFFFF)
	{
		mustRebuild = false;
	}
	else
	{
		mustRebuild = true;
		padding = 1000;
	}
	// add padding as the last block
	BuildBlockHeader(padding, METADATA_BLOCK_PADDING + 0x80);
	Daten.AddMemory(tmpHdr, 4);
	Daten.AddValue(0, padding);
	return true;
}

// the end of the metadata blocks in the file as it is now (an earlier save may have changed its size)
bool CFLAC::CurrentMetadataSize(LPCWSTR FileName, long &size)
{
	FILE *Stream = _wfsopen(FileName, READ_ONLY, _SH_DENYNO);
	if (Stream == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	bool ok = false;
	BYTE header[4];
	const __int64 fileLength = _filelengthi64(_fileno(Stream));   // an earlier save may have changed the size of the file
	__int64 pos = CTools::ID3v2Size;
	_fseeki64(Stream, pos, SEEK_SET);
	if (fread(header, 1, 4, Stream) == 4 && memcmp(header, FLAC_ID, 4) == 0)
	{
		pos += 4;
		while (true)
		{
			_fseeki64(Stream, pos, SEEK_SET);
			if (fread(header, 1, 4, Stream) != 4 || (header[0] & 0x7F) == 127)
				break;
			pos += 4 + ((header[1] << 16) + (header[2] << 8) + header[3]);
			if (pos > fileLength)
				break;
			if (header[0] & 0x80)
			{
				ok = true;
				break;
			}
		}
	}
	fclose(Stream);
	if (ok)
		size = (long)(pos - CTools::ID3v2Size);
	return ok;
}

bool CFLAC::SaveToFile(LPCWSTR FileName)
{
	// the end of the metadata has to be known exactly
	long currentSize = 0;
	if (!metadataComplete || !CurrentMetadataSize(FileName, currentSize))
	{
		CTools::instance().setLastError(ERR_INVALID_FORMAT);
		return false;
	}
	oldLen = currentSize;
	firstAudioPosition = CTools::ID3v2Size + oldLen;
	if (!BuildFrame(true))
	{
		CTools::instance().setLastError(ERR_FRAME_TOO_BIG);
		return false;
	}
	if (mustRebuild)
		return RebuildFile(FileName);
	return ReplaceTag(FileName);
}

bool CFLAC::RebuildFile(LPCWSTR FileName)
{
	FILE *Source;
	FILE *Destination;
	CAtlString NewFileName(FileName);
	CBlob tmp;
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}

	/* Create file streams */
	NewFileName+=TILDE;
	if ( (Destination = _wfsopen(NewFileName, WRITE_ONLY, _SH_DENYWR)) == NULL)
	{
		fclose(Source);
		CTools::instance().setLastError(errno);
		return false;
	};
	// if an ID3v2 tag is present, carry it along as well
	if (CTools::ID3v2Size > 0)
	{
		tmp.FileRead(CTools::ID3v2Size, Source);
		tmp.FileWrite(CTools::ID3v2Size, Destination);
	}
	// skip old block
	_fseeki64(Source, oldLen, SEEK_CUR);
	Daten.FileWrite(Daten.GetLength(), Destination);
	// copy the rest of the file block by block, do not load it completely into memory
	if (!CTools::copyStream(Source, Destination, -1))
	{
		fclose(Destination);
		fclose(Source);
		_wremove(NewFileName);
		CTools::instance().setLastError(EIO);
		return false;
	}
	return CTools::finishRewrite(Source, Destination, NewFileName, FileName);
}

bool CFLAC::ReplaceTag(LPCWSTR FileName)
{
	FILE *Stream;
	if ( (Stream = _wfsopen(FileName, READ_AND_WRITE, _SH_DENYWR)) != NULL)
	{
		_fseeki64(Stream, CTools::ID3v2Size, SEEK_SET);
		Daten.FileWrite(Daten.GetLength(), Stream);
		fflush(Stream);
		fclose(Stream);
		return true;
	}
	CTools::instance().setLastError(errno);
	return false;
}

CFlacCover* CFLAC::GetCover(short Index)
{
	if (covers.GetCount() == 0 || (size_t)Index > covers.GetCount() || Index < 1)
		return NULL;
	return covers.GetAt(Index - 1);
}

bool CFLAC::DeletePicture(short Index)
{
	if (covers.GetCount() == 0 || (size_t)Index > covers.GetCount() || Index < 1)
		return false;
	delete covers.GetAt(Index -1);
	covers.RemoveAt(Index - 1);
	return true;
}

void CFLAC::DeletePictures()
{
	for (size_t i = 0; i < covers.GetCount(); i++)
		delete covers[i];	
	covers.RemoveAll();
}

bool CFLAC::replaceCover(CFlacCover *cover)
{
	for (size_t i = 0; i < covers.GetCount(); i++)
	{
		if (cover->description.Compare(covers[i]->description) == 0)
		{
			delete covers[i];
			covers[i] = cover;
			return true;
		}
	}
	// newly added
	covers.Add(cover);
	return false;
}