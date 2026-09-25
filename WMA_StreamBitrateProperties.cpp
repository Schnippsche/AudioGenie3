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
#include "WMA_StreamBitrateProperties.h"
#include "Tools.h"

CWMA_StreamBitrateProperties::CWMA_StreamBitrateProperties(void)
{
	_objectID = WMA_STREAM_BITRATE_PROPERTIES_ID;
}

CWMA_StreamBitrateProperties::~CWMA_StreamBitrateProperties(void)
{
}

void CWMA_StreamBitrateProperties::Empty()
{
	_data.Clear();
	Flags = 0;
	AverageBitrate = 0;
}
bool CWMA_StreamBitrateProperties::load(FILE *Stream, size_t maxLen)
{
	ATLTRACE(_T("StreamBitrate Properties with %i bytes\n"), maxLen);
	_data.FileRead(6, Stream);
	if (_data.GetLength() != 6 || maxLen < 6)
	{
		 CTools::instance().setLastError(ERR_WMA_PARSE);
		_data.Clear();
		return false;
	}
	Flags = _data.GetR2B(0);
	AverageBitrate = _data.GetR4B(2);
	return true;
}