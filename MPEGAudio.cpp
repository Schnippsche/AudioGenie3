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

// reads a 32 bit big endian number
long CMPEGAudio::Get4B(BYTE v[])
{
	return v[0] * 0x1000000 + v[1] * 0x10000 + v[2] * 0x100 + v[3];
}

void CMPEGAudio::ResetData()
{
	/* Reset all variables */
	memset(&FVBR, 0, sizeof(FVBR));
	memset(&FLame, 0, sizeof(FLame));
	lameHeaderStart = 0;
	lameHeaderSize = 0;
	memset(&Frame, 0, sizeof(Frame));
	memset(&Data, 0, sizeof(Data));
	memset(VendorID, 0, 10);
	totalBitrate = 0;
	secPerFrame = 0;
	scannedFrames = 0;
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

// checks the 4 bytes of a frame header: sync bits and the reserved or unsupported values of the fields
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

// coefficient for the frame length: frame length = coefficient * bit rate / sample rate + padding
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
			return 48;   // 384 samples per frame in every version
		else if (Frame.LayerID == MPEG_LAYER_II)
			return 144;
		else
			return 72;
	}
}

// bit rate in kbit/s from the table of the version, layer and bit rate index
WORD CMPEGAudio::GetBitRateID()
{
	/* Get bit rate */
	return MPEG_BIT_RATE[Frame.VersionID][Frame.LayerID][Frame.BitRateID];
}

long CMPEGAudio::GetSampleRate()
{
	/* Get sample rate */
	return MPEG_SAMPLE_RATE[Frame.VersionID][Frame.SampleRateID];
}

// size of the padding slot: 4 bytes in layer 1, 1 byte in the other layers
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

// samples per frame: layer 1 has 384, layer 2 has 1152, layer 3 has 1152 (MPEG 1) or 576 (MPEG 2 and 2.5)
long CMPEGAudio::GetSamplesPerFrame()
{
	if (Frame.LayerID == MPEG_LAYER_I)
		return 384;
	if (Frame.LayerID == MPEG_LAYER_II || Frame.VersionID == MPEG_VERSION_1)
		return 1152;
	return 576;
}

// length of the current frame in bytes, including the header
long CMPEGAudio::GetFrameLength()
{
	long Coefficient, BitRate, SampleRate, Padding;
	/* Calculate MPEG frame length */
	// layer 1: the slots have 4 bytes, the number of slots is rounded down before it is multiplied by 4
	if (Frame.LayerID == MPEG_LAYER_I)
	{
		BitRate = GetBitRateID();
		SampleRate = GetSampleRate();
		return SampleRate > 0 ? ((12000l * BitRate / SampleRate) + (Frame.PaddingBit ? 1 : 0)) * 4 : 0;
	}
	Coefficient = GetCoefficient();
	BitRate = GetBitRateID();
	SampleRate = GetSampleRate();
	Padding = GetPadding();
	return ((long) (Coefficient * BitRate * 1000l / SampleRate)) + Padding;
}

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

// true if a valid frame header is at the given position of the buffer
bool CMPEGAudio::ValidFrameAt(long Index, BYTE Data[])
{
	/* Check for frame at given position */
	BYTE HeaderData[4];
	memcpy(&HeaderData, &Data[Index], 4);
	if (IsFrameHeader(HeaderData))
		return true;
	return false;
}

bool CMPEGAudio::IsXing(long Index, BYTE Data[])
{
	/* Get true if Xing encoder */
	return (memcmp(&Data[Index], &ZERO, 6) == 0);
}

// Xing and Info header: ID, flags (4 bytes), then in this order the fields whose flag is set: frames (bit 0), bytes (bit 1),
// table of contents with 100 bytes (bit 2), quality (bit 3, 4 bytes); the encoder string (for example LAME3.99r) follows
void CMPEGAudio::GetXingInfo(long Index, BYTE Data[], bool info)
{
	memset(&FVBR, 0, sizeof(FVBR));
	FVBR.Found = true;
	FVBR.Cbr = info;
	memcpy(&FVBR.ID, info ? "Info" : VBR_ID_XING, 4);
	const long flags = Get4B(Data + Index + 4);
	long pos = Index + 8;
	if (flags & 1)
	{
		FVBR.Frames = Get4B(Data + pos);
		pos += 4;
	}
	if (flags & 2)
	{
		FVBR.Bytes = Get4B(Data + pos);
		pos += 4;
	}
	if (flags & 4)
		pos += 100;
	if (flags & 8)
	{
		FVBR.Scale = Data[pos + 3];
		pos += 4;
	}
	/* the encoder string may not be present */
	memcpy(&FVBR.VendorID, &Data[pos], 8);
}

// reads the VBRI header of Fraunhofer: number of bytes, number of frames and scale
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

void CMPEGAudio::FindVBR(long Index, BYTE Data[])
{
	/* Check for a Xing or Info header at given position: behind the header, the CRC and the side information */
	if (memcmp(&Data[Index], VBR_ID_XING, 4) == 0)
		GetXingInfo(Index, Data, false);
	else if (memcmp(&Data[Index], "Info", 4) == 0)
		GetXingInfo(Index, Data, true);
}

// The frames and bytes of a Xing, Info or VBRI header describe the file at the time of the encoding. If the file was cut, extended or
// joined afterwards they do not match the audio data any more: then the header is not used (the values are estimated from the size).
bool CMPEGAudio::IsHeaderPlausible()
{
	const __int64 audioBytes = CTools::FileSize - CTools::ID3v1Size - CTools::LyricsSize - CTools::APESize - Frame.FramePosition;
	if (audioBytes <= 0)
		return true;   // the tags are not known: no statement
	// the number of bytes (the audio data with the header frame). Constant bit rate (Info): the size gives the duration as well as the header,
	// so at most 0.5 % of difference. Variable bit rate: the size says little (the bit rate of the header frame is not the average),
	// so the header is only refused if the file is very different (25 %).
	if (FVBR.Bytes > 0)
	{
		const __int64 difference = (FVBR.Bytes > audioBytes) ? (FVBR.Bytes - audioBytes) : (audioBytes - FVBR.Bytes);
		if (difference * (FVBR.Cbr ? 200 : 4) > audioBytes)
			return false;
	}
	// constant bit rate (Info): all frames have the length of the first one
	if (FVBR.Cbr && FVBR.Frames > 0 && GetSampleRate() > 0)
	{
		const double expected = (double)FVBR.Frames * GetCoefficient() * GetBitRateID() * 1000.0 / GetSampleRate();
		// the frames of the header do not contain the header frame itself (or they do): both are fine
		const double withoutHeader = (double)(audioBytes - Frame.FrameSize);
		const bool matches = (expected <= audioBytes * 1.02 && expected >= audioBytes * 0.98) || (expected <= withoutHeader * 1.02 && expected >= withoutHeader * 0.98);
		if (!matches)
			return false;
	}
	return true;
}

// CRC-16 as LAME writes it: polynomial $8005, start value 0, most significant bit first
static unsigned short LameCrc16(unsigned short crc, const BYTE *data, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		crc ^= (unsigned short)(data[i] << 8);
		for (int bit = 0; bit < 8; bit++)
			crc = (unsigned short)((crc & 0x8000) ? ((crc << 1) ^ 0x8005) : (crc << 1));
	}
	return crc;
}

static unsigned long BigEndian32(const BYTE *p)
{
	return ((unsigned long)p[0] << 24) | ((unsigned long)p[1] << 16) | ((unsigned long)p[2] << 8) | p[3];
}

// gain field of the replay gain: 3 bits name (1 = radio, 2 = audiophile), 3 bits originator, 1 bit sign, 9 bits gain in 0.1 dB
static float LameGain(const BYTE *p, int name)
{
	const int value = (p[0] << 8) | p[1];
	if (((value >> 13) & 7) != name)
		return 0.0f;
	const float gain = (value & 0x1FF) / 10.0f;
	return (value & 0x200) ? -gain : gain;
}

// The LAME tag follows the fields of the Xing/Info header (which all have to be present, flags $0F): 9 characters encoder version,
// revision and method, lowpass, replay gain, flags, bit rate, delay and padding, misc, MP3 gain, preset, music length, music CRC and
// the CRC of the frame in front of it; together 36 bytes 120 bytes behind the beginning of the Xing/Info header.
void CMPEGAudio::ParseLameTag(long frameIndex, long xingIndex, BYTE Data[])
{
	memset(&FLame, 0, sizeof(FLame));
	if (!FVBR.Found || memcmp(FVBR.ID, VBR_ID_FHG, 4) == 0 || (Get4B(Data + xingIndex + 4) & 0x0F) != 0x0F)
		return;
	const long ext = xingIndex + 120;
	if (ext + 36 > frameIndex + Frame.FrameSize)
		return;
	const BYTE *p = Data + ext;
	if (memcmp(p, "LAME", 4) != 0 && memcmp(p, "Lavf", 4) != 0 && memcmp(p, "Lavc", 4) != 0 && memcmp(p, "GOGO", 4) != 0 && memcmp(p, "L3.9", 4) != 0)
		return;
	if ((p[9] >> 4) > 1)
		return;   // unknown revision
	FLame.Found = true;
	for (int i = 0; i < 9; i++)
		FLame.Version[i] = (p[i] >= 0x20 && p[i] < 0x7F) ? (char)p[i] : ' ';
	for (int i = 8; i >= 0 && FLame.Version[i] == ' '; i--)
		FLame.Version[i] = 0;
	FLame.Revision = (BYTE)(p[9] >> 4);
	FLame.VbrMethod = (BYTE)(p[9] & 0x0F);
	FLame.Lowpass = p[10] * 100;
	FLame.PeakSignal = (float)(BigEndian32(p + 11) / 8388608.0);   // fixed point with 23 fractional bits
	FLame.RadioGain = LameGain(p + 15, 1);
	FLame.AudiophileGain = LameGain(p + 17, 2);
	FLame.Bitrate = p[20];
	FLame.EncoderDelay = (p[21] << 4) | (p[22] >> 4);
	FLame.EncoderPadding = ((p[22] & 0x0F) << 8) | p[23];
	FLame.Mp3Gain = (short)((p[25] & 0x80) ? -(p[25] & 0x7F) : (p[25] & 0x7F));
	FLame.Preset = ((p[26] & 0x07) << 8) | p[27];
	FLame.MusicLength = (long)BigEndian32(p + 28);
	FLame.MusicCrc = (unsigned short)((p[32] << 8) | p[33]);
	// the CRC of the tag covers the frame from its first byte up to the CRC
	FLame.TagCrcValid = (LameCrc16(0, Data + frameIndex, (size_t)(ext + 34 - frameIndex)) == (unsigned short)((p[34] << 8) | p[35]));
	lameHeaderStart = Frame.FramePosition;
	lameHeaderSize = Frame.FrameSize;
}

// Checks the CRC-16 of the music data: from the frame behind the LAME tag frame up to the music length of the tag
bool CMPEGAudio::IsLameMusicCrcValid(LPCWSTR FileName)
{
	if (!FLame.Found || FLame.MusicLength <= lameHeaderSize)
		return false;
	FILE *Stream = _wfsopen(FileName, READ_ONLY, _SH_DENYNO);
	if (Stream == NULL)
		return false;
	const __int64 start = lameHeaderStart + lameHeaderSize;
	__int64 remaining = (__int64)FLame.MusicLength - lameHeaderSize;
	bool ok = (_fseeki64(Stream, 0, SEEK_END) == 0 && _ftelli64(Stream) >= start + remaining && _fseeki64(Stream, start, SEEK_SET) == 0);
	unsigned short crc = 0;
	BYTE *block = new BYTE[64 * 1024];
	while (ok && remaining > 0)
	{
		const size_t want = (remaining > 64 * 1024) ? 64 * 1024 : (size_t)remaining;
		const size_t got = fread(block, 1, want, Stream);
		if (got != want)
			ok = false;
		crc = LameCrc16(crc, block, got);
		remaining -= (__int64)got;
		CTools::instance().doEvents();
	}
	delete [] block;
	fclose(Stream);
	return ok && crc == FLame.MusicCrc;
}

void CMPEGAudio::FindVBRI(long Index, BYTE Data[])
{
	/* the VBRI header of Fraunhofer is always 32 bytes behind the frame header */
	if (memcmp(&Data[Index], VBR_ID_FHG, 4) == 0)
		GetFhgInfo(Index, Data);
}

// offset of the Xing header behind the frame header (side information: 36 or 21 bytes for MPEG 1 stereo or mono, 21 or 13 for MPEG 2)
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

long CMPEGAudio::GetBitRate()
{
	if (!Frame.Found)
		return 0;
	float Res1 = 0, Res2 = 0;

	/* Get bit rate, calculate average bit rate if VBR header found */
	// an Info header belongs to a file with a constant bit rate, without the number of bytes there is no average
	if (FVBR.Found && FVBR.Cbr)
		return GetBitRateID();
	// without the number of bytes the average is calculated from the size of the audio data (without the header frame)
	if (FVBR.Found && FVBR.Bytes <= 0 && scannedFrames == 0)
	{
		const __int64 audioBytes = CTools::FileSize - CTools::ID3v1Size - CTools::LyricsSize - CTools::APESize - Frame.FramePosition - Frame.FrameSize;
		if (FVBR.Frames > 0 && audioBytes > 0 && GetSampleRate() > 0)
			return (long)((double)audioBytes * 8.0 * GetSampleRate() / ((double)FVBR.Frames * GetSamplesPerFrame()) / 1000.0 + 0.5);
		return GetBitRateID();
	}
	if (FVBR.Found && FVBR.Frames > 0)
	{
		if (CTools::configValues[CONFIG_MPEGEXACTREAD] != 0)
		{
			if (totalBitrate > 0 && FVBR.Frames > 0)
				return totalBitrate / FVBR.Frames;
			else
				return 0;
		}

		// the number of bytes contains the header frame, the number of frames does not
		Res1 = (float(FVBR.Bytes) - (float)Frame.FrameSize) / (float) FVBR.Frames;
		Res1 -= GetPadding();
		Res2 = float(GetSampleRate() / GetCoefficient() / 1000.0f);
		Res1 *= Res2;
		Res1 += 0.5f;
	}
	else
		return GetBitRateID();
	return long(Res1);
}

long CMPEGAudio::GetFrames()
{
	__int64 MPEGSize;
	float tmp;
	if (!Frame.Found)
		return 0;
	/* the frames were counted (MPEGEXACTREAD): exact, independent of data after the last frame */
	if (scannedFrames > 0)
		return scannedFrames;
	/* Get total number of frames, calculate if VBR header not found */
	if (FVBR.Found && FVBR.Frames > 0)
		return FVBR.Frames;
	else
	{
		MPEGSize = CTools::FileSize - CTools::ID3v1Size - StartPosition - CTools::LyricsSize - CTools::APESize;
		tmp = (float)MPEGSize / float(GetFrameLength()) + 0.9f;
		return long( tmp );
	}
}

// duration in seconds: from the scan of all frames if it was made, else from the number of frames of the Xing/VBRI header, else estimated from the size and the bit rate
float CMPEGAudio::GetDuration()
{
	__int64 MPEGSize;
	/* Calculate song duration */
	if (!Frame.Found)
		return 0.0f;
	if (scannedFrames > 0)
		return (float)(scannedFrames * secPerFrame);
	if (FVBR.Found && FVBR.Frames > 0)
		return (float) FVBR.Frames * GetSamplesPerFrame() / GetSampleRate();
	MPEGSize = CTools::FileSize - CTools::ID3v1Size - Frame.FramePosition - CTools::LyricsSize - CTools::APESize;
	return float(MPEGSize) / float(GetBitRate()) / 125.0f;
}

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

// guessed encoder (see the tables of the VBR and CBR encoders)
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

long CMPEGAudio::GetChannels()
{
	if (Frame.ModeID == MPEG_CM_MONO)
		return 1;
	if (Frame.ModeID == MPEG_CM_UNKNOWN)
		return 0;
	return 2;
}

CAtlString CMPEGAudio::GetLayer()
{
	return MPEG_LAYER[Frame.LayerID];
}

CAtlString CMPEGAudio::GetEmphasis()
{
	return MPEG_EMPHASIS[Frame.EmphasisID];
}

CAtlString CMPEGAudio::GetFileVersion()
{
	return MPEG_VERSION[Frame.VersionID];
}

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

// searches the first frame in the data at the start of the audio; it counts only if the next frame follows at the calculated distance
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
				// a CRC (2 bytes) is between the header and the side information if the protection bit is 0; some encoders clear the bit
				// without writing a CRC, so the position without the CRC is checked as well
				long xingIndex = Iterator + GetVBRDeviation();
				if (!Frame.ProtectionBit)
				{
					FindVBR(xingIndex + 2, Data);
					if (FVBR.Found)
						xingIndex += 2;
				}
				if (!FVBR.Found)
					FindVBR(xingIndex, Data);
				if (FVBR.Found)
					ParseLameTag(Iterator, xingIndex, Data);
				else
					FindVBRI(Iterator + 4 + 32, Data);
				// a header that does not match the audio data is not used
				if (FVBR.Found && !IsHeaderPlausible())
					memset(&FVBR, 0, sizeof(FVBR));
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

bool CMPEGAudio::ReadFromFile(FILE *Stream)
{
	long Transferred;
	bool result;
	__int64 maxSearch;
	maxSearch = CTools::audioStart() + CTools::FileSize / 2;
	/* Open file, read first block of data and search for a frame */
	ResetData();
	StartPosition = CTools::audioStart();
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

bool CMPEGAudio::SetPrivateBit(LPCWSTR FileName, bool neu)
{
	return SetBit(FileName, 2, 1, neu);
}

bool CMPEGAudio::SetCopyrightBit(LPCWSTR FileName, bool neu)
{
	return SetBit(FileName, 3, 8, neu);
}

bool CMPEGAudio::SetOriginalBit(LPCWSTR FileName, bool neu)
{
	return SetBit(FileName, 3, 4, neu);
}

// sets or clears one bit of the first frame header directly in the file
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
	// Measured (SSD, file in the cache): 40 MB of frames 181 ms -> 6 ms; 1 MB of non-frame data at the end 446 ms -> about 1 ms.
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
	// the first frame is the frame of a Xing, Info or VBRI header, not an audio frame
	const bool headerFrame = FVBR.Found;
	FVBR.Found = false;
	FVBR.Cbr = false;
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
	if (headerFrame && Count > 0)
	{
		Count--;
		totalBitrate -= MPEG_BIT_RATE[firstFrame.VersionID][firstFrame.LayerID][firstFrame.BitRateID];
	}
	FVBR.Frames = Count;
	scannedFrames = Count;
	secPerFrame = (float)GetSamplesPerFrame() / (float)GetSampleRate(); // sec per frame
	ATLTRACE(_T("Counts: %d Lost:%d Duration:%f sec\n"), Count, Lost, (float)(Count * secPerFrame));
}

