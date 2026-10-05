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

// The access of the library to a file that is read: the readers (ReadFromFile, load, ...) read from a CFile and not from a C stream or a handle of
// the system, so the system functions are used in one place only (File.cpp). The sequential functions (read, seek, tell, getByte) have the meaning of
// fread, fseek, ftell and fgetc; the positioned reads with the caches of the analysis are CTools::readAt and CBlob::FileReadAt.
// A CFile either opens the file itself (openRead) or wraps a C stream of the caller (the functions that write a file read the old one that way).
class CFile
{
public:
	CFile() : m_file(NULL), m_owned(false) {}
	explicit CFile(FILE *file) : m_file(file), m_owned(false) {}   // a C stream that the caller has opened, it stays open
	~CFile() { close(); }
	// opens the file for reading, other programs may read and write it at the same time; false if it cannot be opened (errno is set)
	bool openRead(LPCWSTR fileName);
	void close();
	bool isOpen() const { return m_file != NULL; }
	FILE *stream() const { return m_file; }   // the C stream, for the functions that copy or write
	// size of the file in bytes, -1 on an error
	__int64 size();
	// sequential access
	bool seek(__int64 pos);            // from the start of the file
	bool seekEnd(__int64 offset);      // offset (<= 0) from the end of the file
	__int64 tell();
	size_t read(void *destination, size_t length);
	int getByte();                     // -1 at the end of the file
	// Reads length bytes from the position pos directly from the file (no buffer): afterwards the buffer of the stream is empty, the position is
	// behind the last byte. For large blocks and for filling the caches of CTools::readAt.
	size_t readDirect(__int64 pos, void *destination, size_t length);
	// like readDirect, directly at the current position of the file; only valid if the buffer of the stream is empty (behind readDirect or a seek)
	size_t readDirectHere(void *destination, size_t length);
	void setBuffer(size_t size);       // buffer of the sequential reads, before the first read

private:
	CFile(const CFile &);
	CFile &operator=(const CFile &);
	FILE *m_file;
	bool m_owned;
};
