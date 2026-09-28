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


// Audio.h: interface for class CAudio.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Tools.h"

class CAudio
{
public:
	CAudio();
	virtual ~CAudio();
	// virtual methods with default values
	virtual long GetChannels()       { return 0; };
	virtual float GetDuration()      { return 0; };
	virtual long GetSampleRate()     { return 0; };
	virtual long GetBitRate();
	virtual CAtlString GetChannelMode();
	virtual CAtlString GetFileVersion() { return L"";   };
	virtual bool IsValid()           { return false;     };
	virtual __int64 GetFirstAudioPosition() { return CTools::audioStart(); };
	virtual __int64 GetLastAudioPosition()  { return CTools::FileSize - 1; };
};


