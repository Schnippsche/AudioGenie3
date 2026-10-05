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
#include "Audio.h"
#include "Tools.h"

class CTTA : public CAudio
{
	private:
	__int64 _samples;                 // number of the samples of a channel (32 bit in TTA1, 64 bit in TTA2)
	long _samplerate, _channels, _bitspersample;
	CBlob _header;
	bool CheckValid();
public:
	CTTA(void);
	virtual ~CTTA();
	bool ReadFromFile(CFile *Stream);
	void ResetData();
	bool IsValid();
	CAtlString GetFileVersion();
	CAtlString GetChannelMode();
	long GetSamples()        { return _samples > 0x7FFFFFFF ? 0x7FFFFFFF : (long)_samples; };
	long GetSampleRate()     { return _samplerate;  };     
	long GetChannels()       { return _channels;    };
	float GetDuration();
};

