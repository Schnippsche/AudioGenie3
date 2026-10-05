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
#include "File.h"
#include "Blob.h"
#include "share.h"
#include <io.h>

bool CFile::openRead(LPCWSTR fileName)
{
	close();
	m_file = _wfsopen(fileName, READ_ONLY, _SH_DENYNO);
	m_owned = (m_file != NULL);
	return m_file != NULL;
}

void CFile::close()
{
	if (m_file != NULL && m_owned)
		fclose(m_file);
	m_file = NULL;
	m_owned = false;
}

__int64 CFile::size()
{
	if (m_file == NULL)
		return -1;
	LARGE_INTEGER length;
	if (!GetFileSizeEx((HANDLE)_get_osfhandle(_fileno(m_file)), &length))
		return -1;
	return length.QuadPart;
}

bool CFile::seek(__int64 pos)
{
	return m_file != NULL && _fseeki64(m_file, pos, SEEK_SET) == 0;
}

bool CFile::seekEnd(__int64 offset)
{
	return m_file != NULL && _fseeki64(m_file, offset, SEEK_END) == 0;
}

__int64 CFile::tell()
{
	return m_file != NULL ? _ftelli64(m_file) : -1;
}

size_t CFile::read(void *destination, size_t length)
{
	return m_file != NULL ? fread(destination, 1, length, m_file) : 0;
}

int CFile::getByte()
{
	if (m_file == NULL)
		return -1;
	const int c = fgetc(m_file);
	return c == EOF ? -1 : c;
}

size_t CFile::readDirectHere(void *destination, size_t length)
{
	if (m_file == NULL)
		return 0;
	size_t done = 0;
	while (done < length)
	{
		const size_t part = length - done < 0x40000000 ? length - done : 0x40000000;
		const int got = _read(_fileno(m_file), (BYTE *)destination + done, (unsigned int)part);
		if (got <= 0)
			break;
		done += (size_t)got;
	}
	return done;
}

size_t CFile::readDirect(__int64 pos, void *destination, size_t length)
{
	if (m_file == NULL || _lseeki64(_fileno(m_file), pos, SEEK_SET) < 0)
		return 0;
	const size_t done = readDirectHere(destination, length);
	// the C library may keep older data in the buffer of the stream (a seek to a position inside of the buffer does not move the file) and takes
	// the position of the next refill from the file: the stream is brought to a known state (an empty buffer)
	_fseeki64(m_file, 0, SEEK_END);
	return done;
}

void CFile::setBuffer(size_t size)
{
	if (m_file != NULL)
		setvbuf(m_file, NULL, _IOFBF, size);
}
