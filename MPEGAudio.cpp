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
#include "MPEGAudio.h"
#include "stdio.h"
#include "io.h"
#include <fcntl.h>
#include <share.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMPEGAudio::CMPEGAudio()
{
}

CMPEGAudio::~CMPEGAudio()
{
}

/* -------------------------------------------------------------------------- */

long CMPEGAudio::Get4B(BYTE v[])
{
	return v[0] * 0x1000000 + v[1] * 0x10000 + v[2] * 0x100 + v[3];
}

/* -------------------------------------------------------------------------- */

void CMPEGAudio::ResetData()
{
	/* Reset all variables */
	memset(&FVBR, 0, sizeof(FVBR));
	memset(&Frame, 0, sizeof(Frame));
	memset(&Data, 0, sizeof(Data));
	memset(VendorID, 0, 10);
	totalBitrate = 0;
	secPerFrame = 0;
	firstAudioPos = 0;
	lastAudioPos = 0;
	Encoder.Empty();
	StartPosition = 0;
	Frame.Found = false;
	Frame.VersionID = MPEG_VERSION_UNKNOWN;
	Frame.SampleRateID = MPEG_SAMPLE_RATE_UNKNOWN;
	Frame.ModeID = MPEG_CM_UNKNOWN;
	Frame.ModeExtensionID = MPEG_CM_EXTENSION_UNKNOWN;
	Frame.EmphasisID = MPEG_EMPHASIS_UNKNOWN;
	Frame.FramePosition = 0;
}

/* -------------------------------------------------------------------------- */

bool CMPEGAudio::IsFrameHeader(BYTE HeaderData[])
{
	/* Check for valid frame header       AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM */
	if ( ( HeaderData[0] != 0xFF)         ||  // AAAAAAAA
		(( HeaderData[1] & 0xE0) != 0xE0) ||  // AAA00000 header must be 111
		(( HeaderData[1] & 0x18) == 0x08) ||  // 000BB000 version ID must not be 01
		(( HeaderData[1] & 0x06) == 0x00) ||  // 00000CC0 layer must not be 00
		(( HeaderData[2] & 0xF0) == 0xF0) ||  // EEEE0000 bit rate must not be 1111 (bad)
		(( HeaderData[2] & 0xF0) == 0x00) ||  // EEEE0000 bit rate must not be 0000 (free format is not supported)
		(( HeaderData[2] & 0x0C) == 0x0C) ||  // 0000FF00 sampling id must not be 11
		(( HeaderData[3] & 0x03) == 0x02))    // 000000MM emphasis must not be 10
		return false;
	else
		return true;

}

/* -------------------------------------------------------------------------- */

WORD CMPEGAudio::GetCoefficient()
{
	/* Get frame size coefficient */
	if (Frame.VersionID == MPEG_VERSION_1)
	{
		if (Frame.LayerID == MPEG_LAYER_I)
			return 48;
		else
			return 144;
	}
	else
	{
		if (Frame.LayerID == MPEG_LAYER_I)
			return 24;
		else if (Frame.LayerID == MPEG_LAYER_II)
			return 144;
		else
			return 72;
	}
}

/* -------------------------------------------------------------------------- */

WORD CMPEGAudio::GetBitRateID()
{
	/* Get bit rate */
	return MPEG_BIT_RATE[Frame.VersionID][Frame.LayerID][Frame.BitRateID];
}

/* -------------------------------------------------------------------------- */

long CMPEGAudio::GetSampleRate()
{
	/* Get sample rate */
	return MPEG_SAMPLE_RATE[Frame.VersionID][Frame.SampleRateID];
}

/* -------------------------------------------------------------------------- */

int CMPEGAudio::GetPadding()
{
	/* Get frame padding */
	if (Frame.PaddingBit)
	{
		if (Frame.LayerID == MPEG_LAYER_I)
			return 4;
		else
			return 1;
	}
	else
		return 0;
}

/* -------------------------------------------------------------------------- */

long CMPEGAudio::GetFrameLength()
{
	long Coefficient, BitRate, SampleRate, Padding;
	/* Calculate MPEG frame length */
	Coefficient = GetCoefficient();
	BitRate = GetBitRateID();
	SampleRate = GetSampleRate();
	Padding = GetPadding();
	return ((long) (Coefficient * BitRate * 1000l / SampleRate)) + Padding;
}

/* -------------------------------------------------------------------------- */

void CMPEGAudio::DecodeHeader(BYTE HeaderData[])
{
	/* Decode frame header data */
	Frame.VersionID = (BYTE) ((HeaderData[1] >> 3) & 3);
	Frame.LayerID = (BYTE) ((HeaderData[1] >> 1) & 3);
	Frame.ProtectionBit = ((HeaderData[1] & 1) != 0);
	Frame.BitRateID = (BYTE) ((HeaderData[2] >> 4));
	Frame.SampleRateID = (BYTE) ((HeaderData[2] >> 2) & 3);
	Frame.PaddingBit = ((HeaderData[2] & 2) != 0);
	Frame.PrivateBit = ((HeaderData[2] & 1) != 0);
	Frame.ModeID = (BYTE) ((HeaderData[3] >> 6) & 3);
	Frame.ModeExtensionID = (BYTE) ((HeaderData[3] >> 4) & 3);
	Frame.CopyrightBit = ((HeaderData[3] & 8) != 0);
	Frame.OriginalBit = ((HeaderData[3] & 4) != 0);
	Frame.EmphasisID = (BYTE) ((HeaderData[3] & 3));
}

/* -------------------------------------------------------------------------- */

bool CMPEGAudio::ValidFrameAt(long Index, BYTE Data[])
{
	/* Check for frame at given position */
	BYTE HeaderData[4];
	memcpy(&HeaderData, &Data[Index], 4);
	if (IsFrameHeader(HeaderData))
		return true;
	return false;
}

/* -------------------------------------------------------------------------- */

bool CMPEGAudio::IsXing(long Index, BYTE Data[])
{
	/* Get true if Xing encoder */
	return (memcmp(&Data[Index], &ZERO, 6) == 0);
}

/* -------------------------------------------------------------------------- */

void CMPEGAudio::GetXingInfo(long Index, BYTE Data[])
{
	memset(&FVBR, 0, sizeof(FVBR));
	/* Extract Xing VBR info at given position */
	FVBR.Found = true;
	memcpy(&FVBR.ID, VBR_ID_XING, 4);
	FVBR.Frames = Get4B(Data + Index + 8);
	FVBR.Bytes = Get4B(Data + Index + 12);
	FVBR.Scale = Data[Index + 119];
	/*{ Vendor ID may not be present */
	memcpy(&FVBR.VendorID, &Data[Index + 120], 8);
}

/* -------------------------------------------------------------------------- */

void CMPEGAudio::GetFhgInfo(long Index, BYTE Data[])
{
	memset(&FVBR, 0, sizeof(FVBR));
	/* Extract FhG VBR info at given position */
	FVBR.Found = true;
	memcpy(&FVBR.ID, VBR_ID_FHG, 4);
	FVBR.Frames = Get4B(Data + Index + 14);
	FVBR.Bytes = Get4B(Data + Index + 10);
	FVBR.Scale = Data[Index + 9];
}

/* -------------------------------------------------------------------------- */

void CMPEGAudio::FindVBR(long Index, BYTE Data[])
{
	/* Check for VBR header at given position */
	if (memcmp(&Data[Index], VBR_ID_XING, 4) == 0)
		GetXingInfo(Index, Data);

	if (memcmp(&Data[Index], VBR_ID_FHG, 4) == 0)
		GetFhgInfo(Index, Data);
}

/* -------------------------------------------------------------------------- */

BYTE CMPEGAudio::GetVBRDeviation()
{
	/* Calculate VBR deviation */
	if (Frame.VersionID == MPEG_VERSION_1)
		if (Frame.ModeID != MPEG_CM_MONO)
			return 36;
		else
			return 21;
	else if (Frame.ModeID != MPEG_CM_MONO)
		return 21;
	else
		return 13;
}

/* -------------------------------------------------------------------------- */

long CMPEGAudio::GetBitRate()
{
	if (!Frame.Found)
		return 0;
	float Res1 = 0, Res2 = 0;

	/* Get bit rate, calculate average bit rate if VBR header found */
	if (FVBR.Found && FVBR.Frames > 0)
	{
		if (CTools::configValues[CONFIG_MPEGEXACTREAD] != 0)
		{
			if (totalBitrate > 0 && FVBR.Frames > 0)
				return totalBitrate / FVBR.Frames;
			else
				return 0;
		}

		Res1 = float(FVBR.Bytes) / (float) FVBR.Frames;
		Res1 -= GetPadding();
		Res2 = float(GetSampleRate() / GetCoefficient() / 1000.0f);
		Res1 *= Res2;
		Res1 += 0.5f;
	}
	else
		return GetBitRateID();
	return long(Res1);
}

/* -------------------------------------------------------------------------- */

long CMPEGAudio::GetFrames()
{
	__int64 MPEGSize;
	float tmp;
	if (!Frame.Found)
		return 0;
	/* Get total number of frames, calculate if VBR header not found */
	if (FVBR.Found)
		return FVBR.Frames;
	else
	{
		MPEGSize = CTools::FileSize - CTools::ID3v1Size - StartPosition - CTools::LyricsSize - CTools::APESize;
		tmp = (float)MPEGSize / float(GetFrameLength()) + 0.9f;
		return long( tmp );
	}
}

/* -------------------------------------------------------------------------- */

float CMPEGAudio::GetDuration()
{
	__int64 MPEGSize;
	/* Calculate song duration */
	if (!Frame.Found)
		return 0.0f;
	if (FVBR.Found && FVBR.Frames > 0)
	{
		if (CTools::configValues[CONFIG_MPEGEXACTREAD] != 0)
			return (float)(FVBR.Frames * secPerFrame);
		else
			return (float) FVBR.Frames * GetCoefficient() * 8.0f / GetSampleRate();
	}
	MPEGSize = CTools::FileSize - CTools::ID3v1Size - Frame.FramePosition - CTools::LyricsSize - CTools::APESize;
	return float(MPEGSize) / float(GetBitRate()) / 125.0f;
}

/* -------------------------------------------------------------------------- */

BYTE CMPEGAudio::GetVBREncoderID()
{
	BYTE result = 0;
	/* Guess VBR encoder and get ID */  
	if (memcmp(&FVBR.VendorID, VENDOR_ID_LAME, 4) == 0)
		result = MPEG_ENCODER_LAME;

	if (memcmp(&FVBR.VendorID, VENDOR_ID_GOGO_NEW, 4) == 0)
		result = MPEG_ENCODER_GOGO;

	if (memcmp(&FVBR.VendorID, VENDOR_ID_GOGO_OLD, 4) == 0)
		result = MPEG_ENCODER_GOGO;

	if (memcmp(&FVBR.VendorID, VBR_ID_XING, 4) == 0)
		result = MPEG_ENCODER_XING;

	if (memcmp(&FVBR.VendorID, VBR_ID_FHG, 4) == 0)
		result = MPEG_ENCODER_FHG;
	return result;
}

/* -------------------------------------------------------------------------- */

BYTE CMPEGAudio::GetCBREncoderID()
{
	FindVendorID();	
	BYTE result = MPEG_ENCODER_FHG;
	/* Guess CBR encoder and get ID */
	if (Frame.OriginalBit && Frame.ProtectionBit)
		result = MPEG_ENCODER_LAME;
	if (GetBitRate() <= 160 && Frame.ModeID == MPEG_CM_STEREO)
		result = MPEG_ENCODER_BLADE;
	if (Frame.CopyrightBit && Frame.OriginalBit && !Frame.ProtectionBit)
		result =  MPEG_ENCODER_XING;
	if (Frame.Xing && Frame.OriginalBit)
		result =  MPEG_ENCODER_XING;
	if (Frame.LayerID == MPEG_LAYER_II)
		result =  MPEG_ENCODER_QDESIGN;
	if (Frame.ModeID == MPEG_CM_DUAL_CHANNEL && Frame.ProtectionBit)
		result =  MPEG_ENCODER_SHINE;
	if (memcmp(VendorID, VENDOR_ID_LAME, 4) == 0)
		result =  MPEG_ENCODER_LAME;
	if (memcmp(VendorID, VENDOR_ID_GOGO_NEW, 4) == 0)
		result =  MPEG_ENCODER_GOGO;
	return result;
}

/* -------------------------------------------------------------------------- */

BYTE CMPEGAudio::GetEncoderID()
{
	/* Get guessed encoder ID */
	if (Frame.Found)
		if (FVBR.Found)
			return GetVBREncoderID();
		else
			return GetCBREncoderID();
	return 0;
}

/* -------------------------------------------------------------------------- */
void CMPEGAudio::GetInternEncoder()
{
	BYTE temp[10];
	Encoder.Empty();
	memset(temp, 0, 10);

	BYTE EncoderID = GetEncoderID();
	/* Get guessed encoder name and encoder version for LAME */
	Encoder = MPEG_ENCODER[EncoderID];
	if (memcmp(&FVBR.VendorID, ZERO, 6) != 0)
		memcpy(temp, &FVBR.VendorID, 8);

	if (memcmp(VendorID, ZERO, 6) != 0)
		memcpy(temp, &VendorID, 8);

	if (EncoderID == MPEG_ENCODER_LAME && temp[5] == '.')
	{
		Encoder+=_T(" ");
		Encoder+= (char)temp[4];	
		Encoder+= (char)temp[5];	
		Encoder+= (char)temp[6];
		Encoder+= (char)temp[7];	
	}
}

/* -------------------------------------------------------------------------- */

CAtlString CMPEGAudio::GetChannelMode()
{
	switch (Frame.ModeID)
	{
	case MPEG_CM_MONO:
		return MONO;
	case MPEG_CM_STEREO:
		return STEREO;
	case MPEG_CM_JOINT_STEREO:
		return JOINTSTEREO;
	case MPEG_CM_DUAL_CHANNEL:
		return DUALCHANNEL;
	default:
		return UNKNOWN;
	}
}

/* -------------------------------------------------------------------------- */

long CMPEGAudio::GetChannels()
{
	if (Frame.ModeID == MPEG_CM_MONO)
		return 1;
	if (Frame.ModeID == MPEG_CM_UNKNOWN)
		return 0;
	return 2;
}
/* -------------------------------------------------------------------------- */

CAtlString CMPEGAudio::GetLayer()
{
	return MPEG_LAYER[Frame.LayerID];
}

/* -------------------------------------------------------------------------- */

CAtlString CMPEGAudio::GetEmphasis()
{
	return MPEG_EMPHASIS[Frame.EmphasisID];
}

/* -------------------------------------------------------------------------- */

CAtlString CMPEGAudio::GetFileVersion()
{
	return MPEG_VERSION[Frame.VersionID];
}

/* -------------------------------------------------------------------------- */

void CMPEGAudio::FindVendorID()
{
	int Iterator;
	BYTE* point = vendorValues.m_pData;
	for (Iterator = (int)vendorValues.GetLength() - 8; Iterator > 0; Iterator--)
	{
		if (   point[Iterator] == 'L'
			&& point[Iterator + 1] == 'A'
			&& point[Iterator + 2] == 'M'
			&& point[Iterator + 3] == 'E') 
		/*
			if (vendorValues.GetAt(Iterator) == 'L'
			&& vendorValues.GetAt(Iterator + 1) == 'A'
			&& vendorValues.GetAt(Iterator + 2) == 'M'
			&& vendorValues.GetAt(Iterator + 3) == 'E' ) */
		{
			memcpy(VendorID, vendorValues.m_pData + Iterator, 8);
			return;
		}
		if (point[Iterator] == 'G'
			&& point[Iterator + 1] == 'O'
			&& point[Iterator + 2] == 'G'
			&& point[Iterator + 3] == 'O')
		/*if (vendorValues.GetAt(Iterator) == 'G'
			&& vendorValues.GetAt(Iterator + 1) == 'O'
			&& vendorValues.GetAt(Iterator + 2) == 'G'
			&& vendorValues.GetAt(Iterator + 3) == 'O' ) */
		{
			memcpy(VendorID, vendorValues.m_pData + Iterator, 4);
			return;
		}	
	}
	memset(VendorID, 0, 8);
}

/* -------------------------------------------------------------------------- */

bool CMPEGAudio::FindFrame()
{
	BYTE HeaderData[4];
	long Iterator;

	/* Search for valid frame */
	memcpy(&HeaderData, &Data, 4);

	for (Iterator = 0; Iterator < MAX_MPEG_FRAME_LENGTH; Iterator++)
	{
		/* Decode data if frame header found */
		if (IsFrameHeader(HeaderData))
		{
			DecodeHeader(HeaderData);
			/* Check for next frame and try to find VBR header */
			if (ValidFrameAt((long)Iterator + GetFrameLength(), Data))
			{
				Frame.Found = true;
				Frame.FramePosition = StartPosition + (long)Iterator;
				Frame.FrameSize = GetFrameLength();
				Frame.Xing = IsXing(Iterator + 4, Data);
				FindVBR(Iterator + GetVBRDeviation(), Data);				
				break;
			}
		}

		/* Prepare next data block */
		HeaderData[0] = HeaderData[1];
		HeaderData[1] = HeaderData[2];
		HeaderData[2] = HeaderData[3];
		HeaderData[3] = Data[Iterator + 4];
	}
	return true;
}

/* -------------------------------------------------------------------------- */

bool CMPEGAudio::ReadFromFile(FILE *Stream)
{
	long Transferred;
	bool result;
	__int64 maxSearch;
	maxSearch = CTools::ID3v2Size + CTools::FileSize / 2;
	/* Open file, read first block of data and search for a frame */
	ResetData();
	StartPosition = CTools::ID3v2Size;
	_fseeki64(Stream, StartPosition, SEEK_SET);
	Transferred = (long)fread(Data, 1, DATASIZE, Stream);
	if (Transferred < 0)
		Transferred = 0;
	memset(Data + Transferred, 0, sizeof(Data) - Transferred); // do not evaluate remains of the last block
	result = FindFrame();
	firstAudioPos = Frame.FramePosition;
	CTools::firstMpegAudioPos = firstAudioPos; 
	/* Try to search Block by Block if no frame at the beginning found */
	/* But only 50 % of the file */
	while ( (!Frame.Found) && (Transferred == DATASIZE) && (result == true) && (StartPosition < maxSearch))
	{
		CTools::instance().doEvents();
		StartPosition += MAX_MPEG_FRAME_LENGTH;
		_fseeki64(Stream, StartPosition, SEEK_SET);
		Transferred = (long)fread(Data, 1, DATASIZE, Stream);
	if (Transferred < 0)
		Transferred = 0;
	memset(Data + Transferred, 0, sizeof(Data) - Transferred); // do not evaluate remains of the last block
		result = FindFrame();
		firstAudioPos = Frame.FramePosition;
	}

	/* Search for vendor ID at the end if CBR encoded */
	if (Frame.Found)
	{ 
		if (!FVBR.Found)
		{
			vendorValues.Clear();
			if (_fseeki64(Stream, -DATASIZE - CTools::ID3v1Size - CTools::LyricsSize, SEEK_END) != 0)
				_fseeki64(Stream, 0, SEEK_SET); // file is smaller than the search range
			vendorValues.FileRead(DATASIZE, Stream);
		}
		GetInternEncoder();

		if (CTools::configValues[CONFIG_MPEGEXACTREAD] != 0)
			ReadAllFrames(Stream);
		return true;
	}
	ResetData();
	return false;
}

/* -------------------------------------------------------------------------- */

bool CMPEGAudio::SetPrivateBit(LPCWSTR FileName, bool neu)
{
	return SetBit(FileName, 2, 1, neu);
}
/* -------------------------------------------------------------------------- */

bool CMPEGAudio::SetCopyrightBit(LPCWSTR FileName, bool neu)
{
	return SetBit(FileName, 3, 8, neu);
}
/* -------------------------------------------------------------------------- */

bool CMPEGAudio::SetOriginalBit(LPCWSTR FileName, bool neu)
{
	return SetBit(FileName, 3, 4, neu);
}

/* -------------------------------------------------------------------------- */

bool CMPEGAudio::SetBit(LPCWSTR FileName, int HdrPos, BYTE BitPos, bool neu)
{
	BYTE HeaderData[4];
	bool result = false;
	FILE *Stream;
	if ( (Stream = _wfsopen(FileName, READ_AND_WRITE, _SH_DENYWR)) != NULL)
	{
		result = ReadFromFile(Stream);
		if (Frame.Found == false)
		{
			fclose(Stream);
			return false;
		}
		// the header is at StartPosition
		_fseeki64(Stream, Frame.FramePosition, SEEK_SET);
		fread(HeaderData, 1, 4, Stream);
		if (neu)
			HeaderData[HdrPos] |= BitPos;
		else
			HeaderData[HdrPos] &= ~BitPos;

		_fseeki64(Stream, Frame.FramePosition, SEEK_SET);
		fwrite(HeaderData, 1, 4, Stream);
		fflush(Stream);
		fclose(Stream);
		return true;
	}
	CTools::instance().setLastError(errno);
	return result;
}

bool CMPEGAudio::IsValid()
{
	return (Frame.Found == true && 
		Frame.VersionID != MPEG_VERSION_UNKNOWN &&
		Frame.SampleRateID != MPEG_SAMPLE_RATE_UNKNOWN &&
		Frame.ModeID != MPEG_CM_UNKNOWN &&
		Frame.ModeExtensionID != MPEG_CM_EXTENSION_UNKNOWN && 
		Frame.EmphasisID != MPEG_EMPHASIS_UNKNOWN);
}

void CMPEGAudio::ReadAllFrames(FILE *Stream)
{
	// Scans the audio data frame by frame. The file is read in blocks (one read per frame would be very slow, especially
	// on network drives). The scan stops in front of the tags at the end of the file; data that is not a frame is skipped byte by byte.
	const size_t SCAN_BLOCK_SIZE = 64 * 1024;
	long FrameLength, Count = 0, Lost = 0;
	__int64 StartPos = Frame.FramePosition;
	__int64 audioEnd = CTools::FileSize - CTools::ID3v1Size - CTools::LyricsSize - CTools::APESize;
	long oldBitrate = 0, newBitrate = 0;
	// DecodeHeader overwrites Frame with every frame; the properties of the first frame are restored after the scan
	const tagFrameData firstFrame = Frame;
	BYTE *block = new BYTE[SCAN_BLOCK_SIZE];
	__int64 blockStart = 0;
	size_t blockLen = 0;
	ATLTRACE(_T("Start Scanning at %I64d...\n"), StartPos);
	totalBitrate = 0;
	FVBR.Found = false;
	while (StartPos < audioEnd)
	{
		if (StartPos < blockStart || StartPos + 4 > blockStart + (__int64)blockLen)
		{
			// the 4 header bytes are not in the current block: read the next block starting at this position
			blockStart = StartPos;
			blockLen = 0;
			if (_fseeki64(Stream, StartPos, SEEK_SET) == 0)
				blockLen = fread(block, 1, SCAN_BLOCK_SIZE, Stream);
			if (blockLen < 4)
				break; // end of the file or read error
		}
		BYTE *HeaderData = block + (StartPos - blockStart);
		if (IsFrameHeader(HeaderData))
		{
			Count++;
			DecodeHeader(HeaderData);
			FrameLength = GetFrameLength();
			if (FrameLength < 1)
				FrameLength = 1; // a frame length of 0 would never advance
			newBitrate = GetBitRateID();
			totalBitrate += newBitrate;
			if (oldBitrate != 0 && newBitrate != oldBitrate)
				FVBR.Found = true;
			oldBitrate = newBitrate;
			CTools::instance().doEvents();
		}
		else
		{
			Lost++;
			FrameLength = 1;
		}
		StartPos += FrameLength;
	}
	delete [] block;
	Frame = firstFrame;
	FVBR.Frames = Count;
	secPerFrame = (float)GetCoefficient() * 8.0f / (float)GetSampleRate(); // sec per frame
	ATLTRACE(_T("Counts: %d Lost:%d Duration:%f sec\n"), Count, Lost, (float)(Count * secPerFrame));
}

