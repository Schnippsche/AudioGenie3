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
#include "MD5Tool.h"
#include "io.h"
#include "Tools.h"

CMD5Tool::CMD5Tool(void)
{
	buffer = new BYTE[MAX_MD5_BUFFER];
	md5 = new MD5();
}

CMD5Tool::~CMD5Tool(void)
{
	delete [] buffer;
	delete md5;
}

bool CMD5Tool::calcHashFromFile(LPCWSTR FileName, __int64 startPos, __int64 endPos)
{
	md5->MD5Init(&ctx);
	CFile *Stream;
	__int64 end, start, maxLoad;
	hash.Empty();
	if ( (Stream = CFile::openFile(FileName, CFile::Mode::Read, CFile::Share::Read)) != NULL)
	{
		int len;
		end = (endPos == 0) ? CTools::fileLength(Stream) - 1 : endPos;		
		start = min(startPos, end);
		maxLoad = end - start + 1;
		ATLTRACE(_T("MD5calc, start=%I64d ende=%I64d maxLoad=%I64d\n"), start, end, maxLoad);
		if (Stream->seek(start))
		{
			while ( (len = (int)Stream->read(buffer, (size_t)min((__int64)MAX_MD5_BUFFER, maxLoad))) )
			{
				//ATLTRACE(_T("Old StartPos: %d "), (long)Stream->tell() - len);
				//ATLTRACE(_T("Next StartPos: %d "), (long)Stream->tell());
				//ATLTRACE(_T("maxLoad: %d len:%d \n"), maxLoad, len);
				md5->MD5Update(&ctx, buffer, len);
				CTools::instance().doEvents();
				maxLoad-=len;				
			}		
			hashIt();
			Stream->flush();
			CFile::closeFile(Stream);
			ATLTRACE(_T("MD5: %s\n"), hash);
			return true;
		}
		CFile::closeFile(Stream);		
	}
	CTools::instance().setLastError(errno);
	return false;	
}

void CMD5Tool::hashIt()
{
	unsigned char buff[16] = "";
	hash.Empty();
	md5->MD5Final((unsigned char*)buff,&ctx);
	for(int i = 0; i < 16; ++i)
		hash.AppendFormat(_T("%02x"), buff[i]);	
}