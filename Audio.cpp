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

// Audio.cpp: implementation of class CAudio.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Audio.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAudio::CAudio()
{
}

CAudio::~CAudio()
{

}
// default implementation
long CAudio::GetBitRate()
{
  float Duration = GetDuration();
  if (Duration > 0)
    return (long)((CTools::FileSize - CTools::ID3v1Size - CTools::audioStart() - CTools::LyricsSize - CTools::APESize ) / 125 / Duration + 0.5) ;
  else
    return 0;
}

// default implementation
CAtlString CAudio::GetChannelMode()
{
  switch (GetChannels())
  {
    case 1: return MONO;
    case 2: return STEREO;
    default: return UNKNOWN;
  }
}
