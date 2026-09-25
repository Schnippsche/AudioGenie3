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

// MPEGPlus.h: interface for class CMPEGPlus.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Blob.h"
#include "Audio.h"

/* Used with ChannelModeID property */
#define MPP_CM_STEREO  1                                // Index for stereo mode
#define MPP_CM_JOINT_STEREO 2                     // Index for joint-stereo mode
#define MPP_CM_MONO 3                             // Index for mono (SV8 only)

/* Used with ProfileID property */
#define MPP_PROFILE_UNKNOWN 0                                   /* Unknown profile */
#define MPP_PROFILE_THUMB 1                              /* 'Thumb' (poor) quality */
#define MPP_PROFILE_RADIO 2                             /*'Radio' (normal) quality */
#define MPP_PROFILE_STANDARD 3                         /*'Standard' (good) quality */
#define MPP_PROFILE_XTREME 4                       /* 'Xtreme' (very good) quality */
#define MPP_PROFILE_INSANE 5                       /* 'Insane' (excellent) quality */
#define MPP_PROFILE_BRAINDEAD 6                 /* 'BrainDead' (excellent) quality */
#define MPP_PROFILE_QUALITY9 7                /* '--quality 9' (excellent) quality */
#define MPP_PROFILE_QUALITY10 8              /* '--quality 10' (excellent) quality */
#define MPP_PROFILE_QUALITY0 9                            /* '--quality 0' profile */
#define MPP_PROFILE_QUALITY1 10                           /* '--quality 1' profile */
#define MPP_PROFILE_TELEPHONE 11                            /* 'Telephone' profile */
#define MPP_PROFILE_EXPERIMENTAL 12
/* Profile names */
static const LPCWSTR MPP_PROFILE[] =
{ UNKNOWN, _T("Thumb"), _T("Radio"), _T("Standard"), _T("Xtreme"), _T("Insane"), _T("BrainDead"), _T("--quality 9"),
_T("--quality 10"), _T("--quality 0"), _T("--quality 1"), _T("Telephone"), _T("Experimental") };

/* ID code for stream version 7 */
static const long STREAM_VERSION_7_ID = 120279117;    // 120279117 = 'MP+' + #7
static const long STREAM_VERSION_71_ID = 388714573;   // 388714573 = 'MP+' + #23

/* File header data - for internal use */
struct tagape
{ BYTE ByteArray[12];                                     // Data as byte array
long IntegerArray[3];                                   // Data as integer array
};


class CMPEGPlus: public CAudio
{
private:
	tagape Header;
	bool FValid;
	BYTE FChannelModeID;
	long FFrameCount;
	long FSampleRate;
	int  FBitRate;
	BYTE FStreamVersion;
	BYTE FProfileID;
	// Stream version 8 ('MPCK'): packet-based header, values come from the SH packet
	bool FIsSV8;
	long FChannels;
	__int64 FSamples;
	__int64 FBeginSilence;
	bool ReadHeaderSV8(FILE *Stream);
	static BYTE ProfileFromIndex(BYTE index);
	bool FIsCorrupted();
	bool ReadHeader(FILE *Stream);
	int GetBitRateIntern();
	BYTE GetChannelModeID();
	BYTE GetProfileID();

public:
	CMPEGPlus();
	virtual ~CMPEGPlus();
	void ResetData();
	bool ReadFromFile(FILE *Stream);
	bool IsValid()             { return FValid; };
	long GetChannels()         { return FChannels; }
	CAtlString GetChannelMode();
	long GetFrameCount();
	long  GetBitRate();
	BYTE GetStreamVersion();
	CAtlString GetFileVersion();
	float  GetDuration();
	long GetSampleRate();
};

