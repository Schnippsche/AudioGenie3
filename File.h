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
#include <stdio.h>
#include <memory>

// The access of the library to a file that is read: the readers (ReadFromFile, load, ...) read from a CFile and not from a C stream or a handle of
// the system, so the system functions are used in one place only (File.cpp). The sequential functions (read, seek, tell, getByte) have the meaning of
// fread, fseek, ftell and fgetc; the positioned reads with the caches of the analysis are CTools::readAt and CBlob::FileReadAt.
// A CFile either opens the file itself (openRead) or wraps a C stream of the caller (the functions that write a file read the old one that way).
class CFile
{
public:
	CFile();
	explicit CFile(FILE *file);   // a C stream that the caller has opened, it stays open
	~CFile() { close(); }
	// opens the file for reading with the functions of the system, other programs may read and write it at the same time; false if it cannot be
	// opened (errno is set). The reads are positioned reads (one system call each, no seek) and are buffered for the sequential functions.
	bool openRead(LPCWSTR fileName);
	void close();
	bool isOpen() const { return m_handle != INVALID_HANDLE_VALUE || m_file != NULL; }
	// size of the file in bytes, -1 on an error
	__int64 size();
	// sequential access
	bool seek(__int64 pos);            // from the start of the file
	bool seekEnd(__int64 offset);      // offset (<= 0) from the end of the file
	__int64 tell();
	size_t read(void *destination, size_t length);
	int getByte();                     // -1 at the end of the file
	// Reads length bytes from the position pos directly from the file (no buffer, one system call); the position is behind the last byte.
	// For large blocks and for filling the caches of CTools::readAt.
	size_t readDirect(__int64 pos, void *destination, size_t length);
	// like readDirect, directly at the current position of the file
	size_t readDirectHere(void *destination, size_t length);

private:
	CFile(const CFile &);
	CFile &operator=(const CFile &);
	static const size_t BUFFER_SIZE = 8192;
	size_t readRaw(__int64 pos, void *destination, size_t length);   // positioned read of the system
	FILE *m_file;               // a C stream of the caller (the functions that write a file read the old one that way)
	HANDLE m_handle;            // the file opened by openRead
	__int64 m_pos;              // the position of the sequential functions (handle)
	__int64 m_bufferStart;      // the buffer holds the bytes of the file from m_bufferStart on (handle)
	size_t m_bufferLength;
	std::unique_ptr<BYTE[]> m_buffer;   // allocated with the first sequential read
};
