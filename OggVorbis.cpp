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
#include "OGGVORBIS.h"
#include "Blob.h"
#include "io.h"
#include <fcntl.h>
#include "OggHeader.h"
#include "vorbisheader.h"
#include <share.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COggVorbis::COggVorbis()
{
	ResetData();
}

COggVorbis::~COggVorbis()
{
}

// Ogg (RFC 3533): a stream consists of pages with a header (27 bytes + the lacing values) and the segments of the packets. A packet
// ends with a segment of less than 255 bytes. Vorbis: the first packet is the identification header (it has a page of its own), then
// the comment header and the setup header follow; the audio packets start on a new page.

static unsigned __int64 GranulePosition(COGGHeader &page)
{
	return ((unsigned __int64)(unsigned int)page.Abs1 << 32) | (unsigned int)page.AbsolutePosition;
}

/* ********************* Auxiliary functions & procedures ******************** */

// Reads the last page of the stream that has a granule position (the number of the samples): the file is searched from its end
// backwards. A page is only accepted if it is complete and belongs to the stream.
__int64 COggVorbis::GetSamples(FILE *Source)
{
	const __int64 fileSize = CTools::FileSize;
	const long BLOCK = 65536 + 512;
	const __int64 low = (firstAudioPos > 0) ? firstAudioPos : 0;
	CBlob block;
	__int64 end = fileSize;
	while (end > low)
	{
		__int64 start = end - BLOCK;
		if (start < low)
			start = low;
		_fseeki64(Source, start, SEEK_SET);
		block.FileRead((size_t)(end - start), Source);
		const long length = (long)block.GetLength();
		for (long i = length - 27; i >= 0; i--)
		{
			if (memcmp(block.m_pData + i, OGG_PAGE_ID, 4) != 0)
				continue;
			// a page: the version has to be 0 and the page has to fit into the file
			if (block.m_pData[i + 4] != 0)
				continue;
			COGGHeader page;
			_fseeki64(Source, start + i, SEEK_SET);
			if (!page.ReadFromFile(Source) || (unsigned int)page.Serial != serial)
				continue;
			__int64 pageLength = 27 + page.Segments;
			for (int k = 0; k < page.Segments; k++)
				pageLength += page.LacingValues[k];
			if (start + i + pageLength > fileSize)
				continue;
			const unsigned __int64 granule = GranulePosition(page);
			if (granule == 0xFFFFFFFFFFFFFFFFULL)
				continue;   // no packet ends on this page: the previous page counts
			return (__int64)granule;
		}
		if (start <= low)
			break;
		end = start + 65307;   // a page has at most 65307 bytes: the pages that begin at the start of this block are checked again completely
	}
	return 0;
}

// Reads the identification header and the packets of the comment header and the setup header.
bool COggVorbis::GetInfo(FILE *Source, bool withComments)
{
	commentPacket.Clear();
	setupLacing.Clear();
	setupData.Clear();
	headerPages = 0;
	multiplexed = false;
	valid = false;
	__int64 pos = CTools::ID3v2Size;
	_fseeki64(Source, pos, SEEK_SET);
	if (!FPage.ReadFromFile(Source))
		return false;
	// the identification header alone on the first page (bos)
	long bodyLength = 0;
	for (int k = 0; k < FPage.Segments; k++)
		bodyLength += FPage.LacingValues[k];
	const __int64 bodyPos = pos + 27 + FPage.Segments;
	_fseeki64(Source, bodyPos, SEEK_SET);
	Parameters.Reset();
	Parameters.ReadFromFile(Source);
	if (memcmp(Parameters.ID, VORBIS_PARAMETERS_ID, 7) != 0 || bodyLength < 30)
		return false;
	serial = (unsigned int)FPage.Serial;
	firstPagePos = pos;
	pos = bodyPos + bodyLength;
	secondPagePos = pos;
	// the pages of the comment header and the setup header
	int packetsDone = 0;
	while (packetsDone < 2)
	{
		_fseeki64(Source, pos, SEEK_SET);
		COGGHeader page;
		if (!page.ReadFromFile(Source))
			break;
		long length = 0;
		for (int k = 0; k < page.Segments; k++)
			length += page.LacingValues[k];
		const __int64 pageEnd = pos + 27 + page.Segments + length;
		if (pageEnd > CTools::FileSize)
			break;
		if ((unsigned int)page.Serial != serial)
		{
			multiplexed = true;   // a page of another stream between the headers
			pos = pageEnd;
			continue;
		}
		CBlob body;
		body.FileRead(length, Source);
		if ((long)body.GetLength() != length)
			break;
		long offset = 0;
		for (int k = 0; k < page.Segments; k++)
		{
			const BYTE lace = page.LacingValues[k];
			if (packetsDone == 0)
			{
				commentPacket.AddMemory(body.m_pData + offset, lace);
				if (lace < 255)
					packetsDone = 1;
			}
			else
			{
				// the setup header and the rest of the page are kept as they are (segments with their lacing values)
				setupLacing.AddValue(lace);
				setupData.AddMemory(body.m_pData + offset, lace);
				if (packetsDone == 1 && lace < 255)
					packetsDone = 2;
			}
			offset += lace;
			if (commentPacket.GetLength() > (size_t)CTools::FileSize)
				break;
		}
		headerPages++;
		lastHeaderGranule = (__int64)GranulePosition(page);
		lastHeaderFlags = page.Byte;
		pos = pageEnd;
	}
	// an incomplete file (cut inside the headers) can be read, but not written
	headerEndPos = pos;
	valid = (packetsDone == 2);
	if (withComments && packetsDone >= 1 && commentPacket.GetLength() >= 7 && memcmp(commentPacket.m_pData, VORBIS_TAG_ID, 7) == 0)
		ReadTag(Source);
	firstAudioPos = headerEndPos;
	Samples = GetSamples(Source);
	return true;
}

void COggVorbis::ReadTag(FILE *Source)
{
	Source;
	AnalyzeVorbisComments(commentPacket.m_pData + 7, commentPacket.GetLength() - 7);
}

void COggVorbis::BuildTag()
{
	Data.Clear();
	Data.AddMemory(VORBIS_TAG_ID, 7);
	BuildVorbisComments(Data);
	Data.AddValue(1);   // the framing bit
}

unsigned long COggVorbis::CalculateCRC(unsigned long CRC, BYTE Data[], long Size)
{
	long Index;
	// Calculate CRC through data
	for (Index = 0; Index < Size; Index++)
		CRC = (CRC << 8) XOR CRC_TABLE[((CRC >> 24) &0xff ) XOR Data[Index]];

	return CRC;
}

// the pages of the headers behind the identification header: the new comment header and the old setup header. A page has at most 255
// segments, the headers end on a page. Sequence numbers start with 1, the checksums are set.
int COggVorbis::BuildHeaderPages(CBlob &out)
{
	CBlob lacing, content;
	long n = (long)Data.GetLength();
	while (n >= 255)
	{
		lacing.AddValue(255);
		n -= 255;
	}
	lacing.AddValue((BYTE)n);
	content.AddBlob(Data);
	lacing.AddBlob(setupLacing);
	content.AddBlob(setupData);
	const size_t total = lacing.GetLength();
	size_t index = 0;
	size_t dataPos = 0;
	unsigned int seq = 1;
	bool continued = false;
	int pages = 0;
	while (index < total)
	{
		const size_t count = (total - index > 255) ? 255 : (total - index);
		long bodyLength = 0;
		for (size_t k = 0; k < count; k++)
			bodyLength += lacing.m_pData[index + k];
		const bool last = (index + count >= total);
		const bool endsPacket = (lacing.m_pData[index + count - 1] < 255);
		const __int64 granule = last ? lastHeaderGranule : (endsPacket ? 0 : -1);
		CBlob page;
		page.AddMemory(OGG_PAGE_ID, 4);
		page.AddValue(0);
		page.AddValue((BYTE)((continued ? 1 : 0) | (last ? (lastHeaderFlags & 4) : 0)));
		for (int k = 0; k < 8; k++)
			page.AddValue((BYTE)(((unsigned __int64)granule >> (8 * k)) & 0xFF));
		page.AddR4B((int)serial);
		page.AddR4B((int)seq);
		page.AddR4B(0);   // the checksum is set below
		page.AddValue((BYTE)count);
		page.AddMemory(lacing.m_pData + index, count);
		page.AddMemory(content.m_pData + dataPos, bodyLength);
		const unsigned long crc = CalculateCRC(0, page.m_pData, (long)page.GetLength());
		page.m_pData[22] = (BYTE)(crc & 0xFF);
		page.m_pData[23] = (BYTE)((crc >> 8) & 0xFF);
		page.m_pData[24] = (BYTE)((crc >> 16) & 0xFF);
		page.m_pData[25] = (BYTE)((crc >> 24) & 0xFF);
		out.AddBlob(page);
		index += count;
		dataPos += bodyLength;
		seq++;
		pages++;
		continued = !endsPacket;
	}
	return pages;
}

// Copies the pages behind the headers. If the number of the header pages has changed, the sequence numbers of the pages of the stream
// change by delta and their checksums are calculated again.
bool COggVorbis::CopyPages(FILE *Source, FILE *Destination, int delta)
{
	_fseeki64(Source, headerEndPos, SEEK_SET);
	if (delta == 0)
		return CTools::copyStream(Source, Destination, -1);
	CBlob page;
	while (true)
	{
		page.FileRead(27, Source);
		if (page.GetLength() == 0)
			return true;
		if (page.GetLength() < 27 || memcmp(page.m_pData, OGG_PAGE_ID, 4) != 0)
			return (page.FileWrite(page.GetLength(), Destination) == page.GetLength()) && CTools::copyStream(Source, Destination, -1);
		const int segments = page.m_pData[26];
		CBlob lacing;
		lacing.FileRead(segments, Source);
		if ((int)lacing.GetLength() != segments)
			return false;
		long length = 0;
		for (int k = 0; k < segments; k++)
			length += lacing.m_pData[k];
		CBlob body;
		body.FileRead(length, Source);
		page.AddBlob(lacing);
		page.AddBlob(body);
		if ((long)body.GetLength() != length)
			return page.FileWrite(page.GetLength(), Destination) == page.GetLength();   // a truncated page at the end of the file
		unsigned int pageSerial = 0, seq = 0;
		memcpy(&pageSerial, page.m_pData + 14, 4);
		memcpy(&seq, page.m_pData + 18, 4);
		if (pageSerial == serial)
		{
			seq += (unsigned int)delta;
			memcpy(page.m_pData + 18, &seq, 4);
			memset(page.m_pData + 22, 0, 4);
			const unsigned long crc = CalculateCRC(0, page.m_pData, (long)page.GetLength());
			memcpy(page.m_pData + 22, &crc, 4);
		}
		if (page.FileWrite(page.GetLength(), Destination) != page.GetLength())
			return false;
		CTools::instance().doEvents();
		page.Clear();
	}
}

bool COggVorbis::RebuildFile(LPCWSTR FileName)
{
	FILE *Source;
	FILE *Destination;
	CAtlString NewFileName(FileName);
	/* Rebuild the file with the new Vorbis tag */
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	/* Create file streams */
	// read and write permissions are required!
	NewFileName+=TILDE;
	if ( (Destination = _wfsopen(NewFileName, READ_AND_WRITENEW, _SH_DENYWR)) == NULL)
	{
		CTools::instance().setLastError(errno);
		fclose(Source);
		return false;
	};
	// everything in front of the header pages (the first page), the new header pages, the pages of the audio
	CBlob newHeader;
	const int newPages = BuildHeaderPages(newHeader);
	_fseeki64(Source, 0, SEEK_SET);
	bool ok = CTools::copyStream(Source, Destination, secondPagePos);
	if (ok)
		ok = (newHeader.FileWrite(newHeader.GetLength(), Destination) == newHeader.GetLength());
	if (ok)
		ok = CopyPages(Source, Destination, newPages - this->headerPages);
	if (!ok)
	{
		fclose(Destination);
		fclose(Source);
		_wremove(NewFileName);
		CTools::instance().setLastError(EIO);
		return false;
	}
	return CTools::finishRewrite(Source, Destination, NewFileName, FileName);
}

void COggVorbis::ResetData()
{
	/* Reset variables */
	FPage.Reset();
	LPage.Reset();
	Parameters.Reset();
	Samples = 0;                                   /* Total number of samples */
	FChannelModeID = 0;
	FSampleRate = 0;
	FBitRateNominal = 0;
	FSamples = 0;
	Data.Clear();
	commentPacket.Clear();
	setupLacing.Clear();
	setupData.Clear();
	firstAudioPos = 0;
	firstPagePos = 0;
	secondPagePos = 0;
	headerEndPos = 0;
	headerPages = 0;
	serial = 0;
	lastHeaderGranule = 0;
	lastHeaderFlags = 0;
	multiplexed = false;
	valid = false;
	CVorbisComment::ResetData();
}

float COggVorbis::FGetDuration()
{
	/* Calculate duration time: the granule position of the last page is the number of the samples */
	if (FSamples > 0)
	{
		if (FSampleRate > 0)
			return (float) ((double)FSamples / (double)FSampleRate);
		return 0.0f;
	}
	// without a granule position the nominal bit rate (kbit/s) and the size of the file give the duration
	if (FBitRateNominal > 0)
		return (float) ((double)(CTools::FileSize - CTools::ID3v2Size) / ((double)FBitRateNominal * 125.0));
	return 0.0f;
}

int COggVorbis::FGetBitRate()
{
	/* Calculate average bit rate */
	float Duration = FGetDuration();
	if (Duration > 0)
		return (int) ((CTools::FileSize - CTools::ID3v2Size) / Duration / 125);
	else
		return 0;
}

bool COggVorbis::FIsValid()
{
	/* Check for file correctness: Vorbis allows 1 to 255 channels */
	return (FChannelModeID >= 1 &&
		FSampleRate > 0 &&
		FGetDuration() > 0.1f &&
		FGetBitRate() > 0);
}

bool COggVorbis::ReadFromFile(FILE *Stream)
{
	/* Read data from file */
	if (GetInfo(Stream, true))
	{
		/* Fill variables */
		FChannelModeID = Parameters.ChannelMode;
		FSampleRate = Parameters.SampleRate;
		FBitRateNominal = int(Parameters.BitRateNominal / 1000);
		FSamples = Samples;
		return true;
	}
	return false;
}

bool COggVorbis::SaveTag(LPCWSTR FileName)
{
	// Save Vorbis tag
	bool Result = false;
	FILE *Source;
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYWR)) != NULL)
	{
		Result = GetInfo(Source, false);
		fclose(Source);
		if (Result && (multiplexed || !valid))
		{
			// the headers of several streams are mixed: the pages cannot be written without the other streams
			CTools::instance().setLastError(ERR_INVALID_FORMAT);
			return false;
		}
		if (Result)
		{
			FChannelModeID = Parameters.ChannelMode;
			FSampleRate = Parameters.SampleRate;
			FBitRateNominal = int(Parameters.BitRateNominal / 1000);
			FSamples = Samples;
			/* Prepare tag data and save to file */
			BuildTag();
			Result = RebuildFile(FileName);
		}
	}
	else
		CTools::instance().setLastError(errno);

	return Result;
}

bool COggVorbis::RemoveTag(LPCWSTR FileName)
{
	// Clear Vorbis tag except the vendor (the name of the encoder)
	const CAtlString vendor = GetVendor();
	CVorbisComment::ResetData();
	SetVendor(vendor);
	return SaveTag(FileName);
}

CAtlString COggVorbis::GetChannelMode()
{
	switch (FChannelModeID)
	{
	case 0:
		return UNKNOWN;
	case VORBIS_CM_MONO:
		return MONO;
	case VORBIS_CM_STEREO:
		return STEREO;
	default:
		return MULTICHANNEL;
	}
};
