/* AudioGenie is a Library for analyzing and tagging audio files.
   Copyright (C) 2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011
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
#include "MP4_MDAT.h"
#include "mp4_atomfactory.h"
#include "Tools.h"
#include <share.h>

CMP4_MDAT::CMP4_MDAT(void)
{
	setFrameID(MP4_MDAT);
	_size = 0;
	_sameFile = false;
}

CMP4_MDAT::~CMP4_MDAT(void)
{
}
void CMP4_MDAT::load(FILE *Stream, u32 offset, u32 size)
{
	offset;
	Stream;
	//_fseeki64(Stream, offset, SEEK_SET);
	// remember position and length but do not read the data
	// is only read when saving
	_position = offset - 8;
	_size = size + 8;
	CMP4_AtomFactory::mediaLength+= size;
	CMP4_AtomFactory::firstAudioPos = offset;
	CMP4_AtomFactory::lastAudioPos = offset + size - 8;
	_sourcefile.Empty();
}
u32 CMP4_MDAT::getSize()
{
	return _size;
}

void CMP4_MDAT::save(FILE *Destination)
{
	// if source and destination position are identical and it is the same file, do nothing
	__int64 newPos = _ftelli64(Destination);
	if (newPos == (__int64)_position && _sameFile)
	{
		// move to the end of the block
		_fseeki64(Destination, _position + _size, SEEK_SET);	
		return;
	}
	// different positions, so copy from old to new
	__int64 frameSize = _size;
	long blockSize = CTools::configValues[CONFIG_ID3V2WRITEBLOCKSIZE];
	if (!_sourcefile.IsEmpty())
	{
		CBlob tmp(blockSize);	
		FILE *Source;
		/* Copy data blocks */
		if ( (Source = _wfsopen(_sourcefile, READ_ONLY, _SH_DENYNO)) != NULL)
		{
			_fseeki64(Source, _position, SEEK_SET);
			long tmpSize = 0;
			while (frameSize > 0)
			{
				tmpSize = (long)((frameSize > blockSize) ? blockSize : frameSize);
				ATLTRACE(_T("write %i bytes\n"), tmpSize);
				tmp.FileRead(tmpSize, Source);
				CTools::instance().doEvents();
				tmp.FileWrite(tmpSize, Destination);
				CTools::instance().doEvents();
				frameSize-=tmpSize;
			}
			fclose(Source);
			_position = (u32)newPos;
		} 
		else
			CTools::instance().setLastError(errno);
	}	
}
