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

// ApeTagItem.cpp: implementation of class CApeTagItem.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ApeTagItem.h"
#include "Tools.h"
// Format:  http://web.archive.org/web/20041026140532/www.personal.uni-jena.de/~pfk/mpp/sv8/apetagitem.html

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CApeTagItem::CApeTagItem()
{

}

CApeTagItem::~CApeTagItem()
{

}

bool CApeTagItem::ReadFromFile(FILE *Stream)
{
  errno = 0;
  CBlob tmp;
  tmp.FileRead(8, Stream);
  Flags = 0;
  Size = (long)tmp.GetR4B(0);   // size of the item value in bytes
  if (Size < 0 || ((__int64)_ftelli64(Stream) + Size + CTools::ID3v1Size) > (__int64)CTools::FileSize )
	  return false;
  Flags = (long)tmp.GetR4B(4);  // item flags
  Key.Empty();
  int keyChar;
  while ((keyChar = fgetc(Stream)) != EOF && keyChar != 0)
  {
    Key+= (char)keyChar;
    if (Key.GetLength() > 255) // keys are short, anything else is corrupt
      return false;
  }
  if (keyChar == EOF)
    return false;
  Value.FileRead(Size, Stream);  
  return (errno == 0);
}

bool CApeTagItem::WriteToFile(FILE *Stream)
{
  errno = 0;
  CBlob tmp;
  tmp.AddR4B(Size);        // size of the item value in bytes
  tmp.AddR4B(Flags);       // item flags
  tmp.FileWrite(8, Stream);
  return (errno == 0);
}

void CApeTagItem::Reset()
{
  Size = 0;
  Flags = 0;
  Key.Empty();
  Value.Clear();
}

bool CApeTagItem::isBinary()
{
  return ((Flags >> 1) != 0);
}