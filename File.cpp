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
#include <errno.h>
#include <limits.h>
#include <new>
#include <string>

// WinBase.h defines it only for _WIN32_WINNT >= 0x0600 (targetver.h has 0x0500); a system without it falls back to MoveFileExW
#ifndef REPLACEFILE_IGNORE_ACL_ERRORS
#define REPLACEFILE_IGNORE_ACL_ERRORS 0x00000004
#endif

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

bool CFile::open(LPCWSTR fileName, Mode mode, Share share)
{
	close();
	if (fileName == NULL)
	{
		errno = EINVAL;
		return false;
	}
	DWORD access = GENERIC_READ, creation = OPEN_EXISTING;
	bool append = false;
	switch (mode)
	{
	case Mode::Read:
		access = GENERIC_READ;
		creation = OPEN_EXISTING;
		break;
	case Mode::Write:
		access = GENERIC_WRITE;
		creation = CREATE_ALWAYS;
		break;
	case Mode::ReadWrite:
		access = GENERIC_READ | GENERIC_WRITE;
		creation = OPEN_EXISTING;
		break;
	case Mode::ReadWriteNew:
		access = GENERIC_READ | GENERIC_WRITE;
		creation = CREATE_ALWAYS;
		break;
	case Mode::Append:
		access = FILE_APPEND_DATA | FILE_READ_ATTRIBUTES | SYNCHRONIZE;   // every write goes to the end of the file
		creation = OPEN_ALWAYS;
		append = true;
		break;  
	}
	const DWORD sharing = (share == Share::Read) ? FILE_SHARE_READ : (FILE_SHARE_READ | FILE_SHARE_WRITE);
	m_handle = CreateFileW(fileName, access, sharing, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		errno = errnoOf(GetLastError());
		return false;
	}
	if (mode != Mode::Read && !isFile())
	{
		close();
		errno = ENODEV;
		return false;
	}
	m_append = append;
	m_failed = false;
	m_pos = 0;
	m_bufferStart = 0;
	m_bufferLength = 0;
	return true;
}

// CreateFileW also opens devices: the console (CON), NUL, COM ports, pipes and, with a path like \\.\C: or \\.\PhysicalDrive0, a whole
// volume or disk. Files on a local, removable or network drive are FILE_TYPE_DISK; so are volumes and disks, but the system has
// no file information for them. Only for writing, where a tag written to a disk would destroy it: for reading, the check would cost
// 1.2 % of the analysis, and a device is harmless there (its size is not known, nothing is read).
bool CFile::isFile() const
{
	if (GetFileType(m_handle) != FILE_TYPE_DISK)
		return false;
	BY_HANDLE_FILE_INFORMATION info;
	return GetFileInformationByHandle(m_handle, &info) && (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool CFile::openRead(LPCWSTR fileName)
{
	return open(fileName, Mode::Read, Share::All);
}

void CFile::close()
{
	if (m_handle != INVALID_HANDLE_VALUE)
		CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
	m_bufferLength = 0;
}

CFile *CFile::openFile(LPCWSTR fileName, Mode mode, Share share)
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

bool CFile::removeFile(LPCWSTR fileName)
{
	if (fileName == NULL)
	{
		errno = EINVAL;
		return false;
	}
	if (!DeleteFileW(fileName))
	{
		errno = errnoOf(GetLastError());
		return false;
	}
	return true;
}

bool CFile::replaceFile(LPCWSTR newFileName, LPCWSTR origFileName)
{
	if (newFileName == NULL || origFileName == NULL)
	{
		errno = EINVAL;
		return false;
	}
	// ReplaceFileW keeps what belongs to the original: the creation time, the attributes, the alternate data streams and the ACL.
	// It is several steps: with a backup name the original is never lost if one of them fails.
	std::wstring backupFileName(origFileName);
	backupFileName += L"~~";
	if (ReplaceFileW(origFileName, newFileName, backupFileName.c_str(), REPLACEFILE_IGNORE_MERGE_ERRORS | REPLACEFILE_IGNORE_ACL_ERRORS, NULL, NULL))
	{
		DeleteFileW(backupFileName.c_str());
		return true;
	}
	const DWORD error = GetLastError();
	if (error == ERROR_UNABLE_TO_MOVE_REPLACEMENT_2)
	{
		// the original has the backup name, the new file still has its own name: the original gets its name back
		MoveFileExW(backupFileName.c_str(), origFileName, 0);
		errno = errnoOf(error);
		return false;
	}
	// Both files are unchanged (e.g. a file system without ReplaceFileW): the rename, which loses what belongs to the original.
	if (!MoveFileExW(newFileName, origFileName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		errno = errnoOf(GetLastError());
		return false;
	}
	return true;
}

__int64 CFile::size()
{
	LARGE_INTEGER length;
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		errno = EBADF;
		return -1;
	}
	if (!GetFileSizeEx(m_handle, &length))
	{
		errno = errnoOf(GetLastError());
		return -1;
	}
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
	if (length < 0 || offset > 0 || length + offset < 0)
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
	// A negative offset is not a position: the system reads -2 (FILE_USE_FILE_POINTER_POSITION) at its file pointer.
	if (pos < 0)
		return 0;
	// pos + length must not overflow (a position from a damaged file)
	if ((unsigned __int64)length > (unsigned __int64)(_I64_MAX - pos))
		length = (size_t)(_I64_MAX - pos);
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
			// The end of the file is not an error; a real error sets errno like fread (the readers check errno).
			// A position far behind the end (from a damaged file, larger than the file system allows) is the end of the file as well.
			const DWORD error = GetLastError();
			bool endOfFile = (error == ERROR_HANDLE_EOF);
			if (error == ERROR_INVALID_PARAMETER)
			{
				const __int64 fileLength = size();
				endOfFile = (fileLength >= 0 && at >= fileLength);
			}
			if (!endOfFile)
			{
				m_failed = true;
				errno = errnoOf(error);
			}
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
		if (!WriteFile(m_handle, (const BYTE *)source + done, part, &written, m_append ? NULL : &where))
		{
			m_failed = true;
			errno = errnoOf(GetLastError());
			break;
		}
		if (written == 0)
		{
			// no error of the system, but nothing written: GetLastError holds an old value
			m_failed = true;
			errno = EIO;
			break;
		}
		done += written;
		m_pos += (__int64)written;
	}
	if (m_append)
	{
		// the writes went to the end of the file; if its size is not known, the position stays behind the bytes written
		const __int64 end = size();
		if (end >= 0)
			m_pos = end;
	}
	return done;
}

bool CFile::truncate(__int64 length)
{
	if (m_handle == INVALID_HANDLE_VALUE || length < 0)
		return false;
	LARGE_INTEGER where;
	where.QuadPart = length;
	m_bufferLength = 0;
	if (!SetFilePointerEx(m_handle, where, NULL, FILE_BEGIN) || !SetEndOfFile(m_handle))
	{
		m_failed = true;
		errno = errnoOf(GetLastError());
		return false;
	}
	return true;
}

bool CFile::flush()
{
	return m_handle != INVALID_HANDLE_VALUE && !m_failed;
}

bool CFile::sync()
{
	if (m_handle == INVALID_HANDLE_VALUE || m_failed)
		return false;
	if (!FlushFileBuffers(m_handle))
	{
		m_failed = true;
		errno = errnoOf(GetLastError());
		return false;
	}
	return true;
}

size_t CFile::readDirect(__int64 pos, void *destination, size_t length)
{
	if (m_handle == INVALID_HANDLE_VALUE || pos < 0)
		return 0;
	const size_t got = readRaw(pos, destination, length);
	m_pos = pos + (__int64)got;
	return got;
}

size_t CFile::readDirectHere(void *destination, size_t length)
{
	return readDirect(m_pos, destination, length);
}
