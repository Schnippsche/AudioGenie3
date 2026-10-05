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
#include <errno.h>
#include <io.h>
#include <new>

CFile::CFile() : m_file(NULL), m_handle(INVALID_HANDLE_VALUE), m_pos(0), m_bufferStart(0), m_bufferLength(0)
{
}

CFile::CFile(FILE *file) : m_file(file), m_handle(INVALID_HANDLE_VALUE), m_pos(0), m_bufferStart(0), m_bufferLength(0)
{
}

// the errno that the C library would have set for an error of the system
static int errnoOf(DWORD error)
{
	switch (error)
	{
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
	case ERROR_INVALID_NAME:
	case ERROR_BAD_NETPATH:
	case ERROR_BAD_PATHNAME:
	case ERROR_INVALID_DRIVE:
	case ERROR_BAD_NET_NAME:
	case ERROR_DIRECTORY:
		return ENOENT;
	case ERROR_ACCESS_DENIED:
	case ERROR_SHARING_VIOLATION:
	case ERROR_LOCK_VIOLATION:
	case ERROR_NETWORK_ACCESS_DENIED:
		return EACCES;
	case ERROR_TOO_MANY_OPEN_FILES:
		return EMFILE;
	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
		return ENOMEM;
	default:
		return EINVAL;
	}
}

bool CFile::openRead(LPCWSTR fileName)
{
	close();
	// like _wfsopen(READ_ONLY, _SH_DENYNO): other programs may read and write the file
	m_handle = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		errno = errnoOf(GetLastError());
		return false;
	}
	m_pos = 0;
	m_bufferStart = 0;
	m_bufferLength = 0;
	return true;
}

void CFile::close()
{
	if (m_handle != INVALID_HANDLE_VALUE)
		CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
	m_file = NULL;   // a C stream of the caller stays open
	m_bufferLength = 0;
}

__int64 CFile::size()
{
	HANDLE handle = m_handle;
	if (handle == INVALID_HANDLE_VALUE)
	{
		if (m_file == NULL)
			return -1;
		handle = (HANDLE)_get_osfhandle(_fileno(m_file));
	}
	LARGE_INTEGER length;
	if (!GetFileSizeEx(handle, &length))
		return -1;
	return length.QuadPart;
}

bool CFile::seek(__int64 pos)
{
	if (m_handle != INVALID_HANDLE_VALUE)
	{
		if (pos < 0)
			return false;
		m_pos = pos;
		return true;
	}
	return m_file != NULL && _fseeki64(m_file, pos, SEEK_SET) == 0;
}

bool CFile::seekEnd(__int64 offset)
{
	if (m_handle != INVALID_HANDLE_VALUE)
	{
		const __int64 length = size();
		if (length < 0 || length + offset < 0)
			return false;
		m_pos = length + offset;
		return true;
	}
	return m_file != NULL && _fseeki64(m_file, offset, SEEK_END) == 0;
}

__int64 CFile::tell()
{
	if (m_handle != INVALID_HANDLE_VALUE)
		return m_pos;
	return m_file != NULL ? _ftelli64(m_file) : -1;
}

// a positioned read: the file pointer is not used (the system takes the offset from the request)
size_t CFile::readRaw(__int64 pos, void *destination, size_t length)
{
	size_t done = 0;
	while (done < length)
	{
		const DWORD part = (DWORD)(length - done < 0x40000000 ? length - done : 0x40000000);
		OVERLAPPED where;
		memset(&where, 0, sizeof(where));
		const __int64 at = pos + (__int64)done;
		where.Offset = (DWORD)(at & 0xFFFFFFFF);
		where.OffsetHigh = (DWORD)(at >> 32);
		DWORD got = 0;
		if (!ReadFile(m_handle, (BYTE *)destination + done, part, &got, &where) || got == 0)
			break;   // the end of the file or an error
		done += got;
	}
	return done;
}

size_t CFile::read(void *destination, size_t length)
{
	if (m_handle == INVALID_HANDLE_VALUE)
		return m_file != NULL ? fread(destination, 1, length, m_file) : 0;
	size_t done = 0;
	BYTE *to = (BYTE *)destination;
	while (done < length)
	{
		// from the buffer
		if (m_bufferLength != 0 && m_pos >= m_bufferStart && m_pos < m_bufferStart + (__int64)m_bufferLength)
		{
			const size_t offset = (size_t)(m_pos - m_bufferStart);
			size_t part = m_bufferLength - offset;
			if (part > length - done)
				part = length - done;
			memcpy(to + done, m_buffer.get() + offset, part);
			done += part;
			m_pos += (__int64)part;
			continue;
		}
		const size_t left = length - done;
		if (left >= BUFFER_SIZE)
		{
			// a block as large as the buffer: directly into the destination
			const size_t got = readRaw(m_pos, to + done, left);
			done += got;
			m_pos += (__int64)got;
			break;
		}
		// fill the buffer from the position
		if (!m_buffer)
			m_buffer.reset(new (std::nothrow) BYTE[BUFFER_SIZE]);
		if (!m_buffer)
		{
			// no memory for the buffer: the request is read directly
			const size_t got = readRaw(m_pos, to + done, left);
			done += got;
			m_pos += (__int64)got;
			break;
		}
		m_bufferStart = m_pos;
		m_bufferLength = readRaw(m_pos, m_buffer.get(), BUFFER_SIZE);
		if (m_bufferLength == 0)
			break;
	}
	return done;
}

int CFile::getByte()
{
	BYTE b;
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		if (m_file == NULL)
			return -1;
		const int c = fgetc(m_file);
		return c == EOF ? -1 : c;
	}
	return read(&b, 1) == 1 ? b : -1;
}

size_t CFile::readDirect(__int64 pos, void *destination, size_t length)
{
	if (m_handle != INVALID_HANDLE_VALUE)
	{
		const size_t got = readRaw(pos, destination, length);
		m_pos = pos + (__int64)got;
		return got;
	}
	if (m_file == NULL || _lseeki64(_fileno(m_file), pos, SEEK_SET) < 0)
		return 0;
	const size_t done = readDirectHere(destination, length);
	// the C library may keep older data in the buffer of the stream (a seek to a position inside of the buffer does not move the file) and takes
	// the position of the next refill from the file: the stream is brought to a known state (an empty buffer)
	_fseeki64(m_file, 0, SEEK_END);
	return done;
}

size_t CFile::readDirectHere(void *destination, size_t length)
{
	if (m_handle != INVALID_HANDLE_VALUE)
		return readDirect(m_pos, destination, length);
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
