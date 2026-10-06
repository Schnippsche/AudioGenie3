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
#include <memory>

// The access of the library to a file: the readers (ReadFromFile, load, ...) and the functions that write a file use a CFile and not a C stream
// or a handle of the system, so the system functions are used in one place only (File.cpp), the one that has to be written again for another
// platform. The sequential functions (read, write, seek, tell, getByte) have the meaning of fread, fwrite, fseek, ftell and fgetc; the positioned
// reads with the caches of the analysis are CTools::readAt and CBlob::FileReadAt.
class CFile
{
public:
	CFile();
	~CFile() { close(); }
	// how a file is opened
	enum class Mode
	{
		Read,           // read, the file has to exist
		Write,          // write, the file is empty afterwards (it is created if it does not exist)
		ReadWrite,      // read and write, the file has to exist
		ReadWriteNew,   // read and write, the file is empty afterwards (it is created if it does not exist)
		Append          // write at the end of the file (the file is created if it does not exist)
	};
	// what other programs may do with the file while it is open
	enum class Share
	{
		All,            // read and write
		Read            // only read
	};
	// false if the file cannot be opened (errno is set; ENODEV if a mode for writing opens a device such as CON, NUL, a pipe, a volume or a disk)
	bool open(LPCWSTR fileName, Mode mode, Share share);
	bool openRead(LPCWSTR fileName);   // Mode::Read and Share::All
	void close();
	bool isOpen() const { return m_handle != INVALID_HANDLE_VALUE; }
	// for the functions that keep a pointer to the file: NULL if the file cannot be opened, closeFile closes and deletes (NULL is allowed)
	static CFile *openFile(LPCWSTR fileName, Mode mode, Share share);
	static void closeFile(CFile *file);
	// deletes a file; false if it cannot be deleted
	static bool removeFile(LPCWSTR fileName);
	// replaces the file origFileName with the file newFileName (ReplaceFileW: the creation time, the attributes, the alternate data streams
	// and the ACL of the original stay; origFileName + "~~" is the backup during the replace); false if that is not possible.
	// The rename does not write the data of the new file to the disk: sync() it before it is closed, otherwise it can be incomplete after a crash of the system.
	static bool replaceFile(LPCWSTR newFileName, LPCWSTR origFileName);
	// size of the file in bytes, -1 on an error (errno is set)
	__int64 size();
	// sequential access
	bool seek(__int64 pos);            // from the start of the file
	bool seekEnd(__int64 offset);      // offset (<= 0) from the end of the file
	__int64 tell();
	size_t read(void *destination, size_t length);
	int getByte();                     // -1 at the end of the file
	size_t write(const void *source, size_t length);   // the bytes written (less than length on an error)
	bool flush();                      // the writes are not buffered here: nothing is left to write (the data is not forced to the disk)
	bool sync();                       // writes the data from the cache of the system to the disk (FlushFileBuffers); false on an error
	bool truncate(__int64 length);     // the file gets this length (shorter: the end is cut off, longer: zeros)
	bool failed() const { return m_failed; }   // a read or write error happened (like ferror)
	// Reads length bytes from the position pos directly from the file (no buffer, one system call); the position is behind the last byte.
	// For large blocks and for filling the caches of CTools::readAt. A negative pos reads nothing and leaves the position unchanged.
	size_t readDirect(__int64 pos, void *destination, size_t length);
	// like readDirect, directly at the current position of the file
	size_t readDirectHere(void *destination, size_t length);

private:
	CFile(const CFile &);
	CFile &operator=(const CFile &);
	static const size_t BUFFER_SIZE = 8192;
	size_t readRaw(__int64 pos, void *destination, size_t length);   // positioned read of the system
	bool isFile() const;   // the handle is a file and not a device, a volume or a disk
	HANDLE m_handle;
	bool m_append;              // the writes go to the end of the file
	bool m_failed;
	__int64 m_pos;              // the position of the sequential functions
	__int64 m_bufferStart;      // the buffer holds the bytes of the file from m_bufferStart on
	size_t m_bufferLength;
	std::unique_ptr<BYTE[]> m_buffer;   // allocated with the first sequential read
};
