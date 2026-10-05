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
#include "share.h"
#include <errno.h>
#include <new>

CFile::CFile() : m_handle(INVALID_HANDLE_VALUE), m_append(false), m_failed(false), m_pos(0), m_bufferStart(0), m_bufferLength(0)
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
	case ERROR_FILE_EXISTS:
	case ERROR_ALREADY_EXISTS:
		return EEXIST;
	case ERROR_TOO_MANY_OPEN_FILES:
		return EMFILE;
	case ERROR_DISK_FULL:
	case ERROR_HANDLE_DISK_FULL:
		return ENOSPC;
	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
		return ENOMEM;
	default:
		return EINVAL;
	}
}

bool CFile::open(LPCWSTR fileName, LPCWSTR mode, int share)
{
	close();
	DWORD access, creation;
	bool append = false;
	if (wcscmp(mode, L"rb") == 0)
	{
		access = GENERIC_READ;
		creation = OPEN_EXISTING;
	}
	else if (wcscmp(mode, L"wb") == 0)
	{
		access = GENERIC_WRITE;
		creation = CREATE_ALWAYS;
	}
	else if (wcscmp(mode, L"r+b") == 0)
	{
		access = GENERIC_READ | GENERIC_WRITE;
		creation = OPEN_EXISTING;
	}
	else if (wcscmp(mode, L"w+b") == 0)
	{
		access = GENERIC_READ | GENERIC_WRITE;
		creation = CREATE_ALWAYS;
	}
	else if (wcscmp(mode, L"ab") == 0)
	{
		access = FILE_APPEND_DATA | FILE_READ_ATTRIBUTES | SYNCHRONIZE;   // every write goes to the end of the file
		creation = OPEN_ALWAYS;
		append = true;
	}
	else
	{
		errno = EINVAL;
		return false;
	}
	const DWORD sharing = (share == _SH_DENYWR) ? FILE_SHARE_READ : (FILE_SHARE_READ | FILE_SHARE_WRITE);
	m_handle = CreateFileW(fileName, access, sharing, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		errno = errnoOf(GetLastError());
		return false;
	}
	m_append = append;
	m_failed = false;
	m_pos = 0;
	m_bufferStart = 0;
	m_bufferLength = 0;
	return true;
}

bool CFile::openRead(LPCWSTR fileName)
{
	return open(fileName, L"rb", _SH_DENYNO);
}

void CFile::close()
{
	if (m_handle != INVALID_HANDLE_VALUE)
		CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
	m_bufferLength = 0;
}

CFile *CFile::openFile(LPCWSTR fileName, LPCWSTR mode, int share)
{
	CFile *file = new (std::nothrow) CFile();
	if (file == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}
	if (!file->open(fileName, mode, share))
	{
		delete file;
		return NULL;
	}
	return file;
}

void CFile::closeFile(CFile *file)
{
	delete file;
}

__int64 CFile::size()
{
	LARGE_INTEGER length;
	if (m_handle == INVALID_HANDLE_VALUE || !GetFileSizeEx(m_handle, &length))
		return -1;
	return length.QuadPart;
}

bool CFile::seek(__int64 pos)
{
	if (m_handle == INVALID_HANDLE_VALUE || pos < 0)
		return false;
	m_pos = pos;
	return true;
}

bool CFile::seekEnd(__int64 offset)
{
	const __int64 length = size();
	if (length < 0 || length + offset < 0)
		return false;
	m_pos = length + offset;
	return true;
}

__int64 CFile::tell()
{
	return m_handle != INVALID_HANDLE_VALUE ? m_pos : -1;
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
		if (!ReadFile(m_handle, (BYTE *)destination + done, part, &got, &where))
		{
			if (GetLastError() != ERROR_HANDLE_EOF)
				m_failed = true;
			break;
		}
		if (got == 0)
			break;   // the end of the file
		done += got;
	}
	return done;
}

size_t CFile::read(void *destination, size_t length)
{
	if (m_handle == INVALID_HANDLE_VALUE)
		return 0;
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
	return read(&b, 1) == 1 ? b : -1;
}

size_t CFile::write(const void *source, size_t length)
{
	if (m_handle == INVALID_HANDLE_VALUE)
		return 0;
	m_bufferLength = 0;   // the bytes in the buffer may be out of date
	size_t done = 0;
	while (done < length)
	{
		const DWORD part = (DWORD)(length - done < 0x40000000 ? length - done : 0x40000000);
		OVERLAPPED where;
		memset(&where, 0, sizeof(where));
		where.Offset = (DWORD)(m_pos & 0xFFFFFFFF);
		where.OffsetHigh = (DWORD)(m_pos >> 32);
		DWORD written = 0;
		if (!WriteFile(m_handle, (const BYTE *)source + done, part, &written, m_append ? NULL : &where) || written == 0)
		{
			m_failed = true;
			errno = errnoOf(GetLastError());
			break;
		}
		done += written;
		m_pos += (__int64)written;
	}
	if (m_append)
		m_pos = size();   // the writes went to the end of the file
	return done;
}

bool CFile::flush()
{
	return m_handle != INVALID_HANDLE_VALUE && !m_failed;
}

size_t CFile::readDirect(__int64 pos, void *destination, size_t length)
{
	if (m_handle == INVALID_HANDLE_VALUE)
		return 0;
	const size_t got = readRaw(pos, destination, length);
	m_pos = pos + (__int64)got;
	return got;
}

size_t CFile::readDirectHere(void *destination, size_t length)
{
	return readDirect(m_pos, destination, length);
}
