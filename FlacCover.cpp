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
#include "FlacCover.h"
#include "Tools.h"
#include "io.h"
#include "ID3F_APIC.h"

/** 
<32>   	 The picture type according to the ID3v2 APIC frame:
Others are reserved and should not be used. There may only be one each of picture type 1 and 2 in a file.
<32> 	The length of the MIME type string in bytes.
<n*8> 	The MIME type string, in printable ASCII characters 0x20-0x7e. The MIME type may also be --> to signify that the data part is a URL of the picture instead of the picture data itself.
<32> 	The length of the description string in bytes.
<n*8> 	The description of the picture, in UTF-8.
<32> 	The width of the picture in pixels.
<32> 	The height of the picture in pixels.
<32> 	The color depth of the picture in bits-per-pixel.
<32> 	For indexed-color pictures (e.g. GIF), the number of colors used, or 0 for non-indexed pictures.
<32> 	The length of the picture data in bytes.
<n*8> 	The binary picture data.  
*/

CFlacCover::CFlacCover(void)
{
	type = 0;
	width = 0;
	height = 0;
	colordepth = 0;
	colornumbers = 0;
	_isLink = false;
}

CFlacCover::CFlacCover(CBlob *tmpData)
{
	// all length values come from the file and must not leave the block
	const size_t total = tmpData->GetLength();
	auto clampLen = [total](size_t pos, long value) -> size_t {
		const size_t avail = (pos < total) ? total - pos : 0;
		const size_t ln = (size_t)(unsigned long)value;
		return (ln < avail) ? ln : avail;
	};
	type = tmpData->Get4B(0);
	int start = 8;
	size_t ln = clampLen(start, tmpData->Get4B(4)); // Length
	_mime = tmpData->GetAnsiStringAt(start, ln); // ASCII Mime (with the code page it is written with)
	start+=(int)ln;
	ln = clampLen(start + 4, tmpData->Get4B(start)); // Length
	start+=4;
	data.Clear();
	data.AddMemory(tmpData->m_pData + start, ln);
	// UTF-8; a description that old programs wrote in the ANSI code page is not lost (it is written back as UTF-8)
	description = data.ConvertUtf8OrAnsi();
	start+=(int)ln;
	width = tmpData->Get4B(start); // width
	height = tmpData->Get4B(start + 4); // height
	colordepth = tmpData->Get4B(start + 8); // color depth
	colornumbers = tmpData->Get4B(start + 12); // numbers of colors
	data.Clear();
	start+=20;
	ln = clampLen(start, tmpData->Get4B(start - 4)); //picture length
	// if the mime type is -->, read the picture from a file
	if (_mime.Compare(_T("-->")) == 0)
	{
		_isLink = true;
		// UTF-8 like the description (and as the link is written)
		_pictureLink = tmpData->getNextString(TEXT_ENCODED_UTF8, start);
		// the link comes from the file: it is only followed if this is configured (LINKEDPICTURES)
		CTools::readLinkedPicture(_pictureLink, data);
	}
	else
	{
		_isLink = false;
		data.AddMemory(tmpData->m_pData + start, ln);
	}
}

CFlacCover::CFlacCover(BYTE picType, LPCWSTR Description)
{
	type = picType;
	description = Description;
	width = 0;
	height = 0;
	colordepth = 0;
	colornumbers = 0;
}


CFlacCover::~CFlacCover(void)
{

}

bool CFlacCover::getPictureFile(LPCWSTR fileName)
{
	return CTools::writeFile(fileName, data.m_pData, data.GetLength());
}

CAtlString CFlacCover::getMime()
{
	return (_isLink) ?  MIME_LINK + _pictureLink : _mime;	
}

long CFlacCover::getPictureArray(BYTE *arr, size_t maxLen)
{
	size_t ln = data.GetLength();
	if (ln > maxLen || (arr == NULL && ln > 0))   // no array: as if it were too small
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, (unsigned)ln);   // the array of the caller is too small: the size it needs
		return -1;
	}
	if (ln > 0)
		memcpy(arr, data.m_pData, ln);
	return (long)ln;
}

long CFlacCover::getPictureSize()
{
	return (long)data.GetLength();
}

CAtlString CFlacCover::getPictureTypeAsText()
{
	if (type >= 0 && type <= 20)
		return APIC_PICTURE_TYPES[type];

	CAtlString dummy;	
	dummy.Format(_T("unknown PictureType %i"), type);	
	return dummy;
}

bool CFlacCover::setPictureFile(LPCWSTR fileName)
{
	_isLink = false;
	_pictureLink.Empty();
	// a file that cannot be read completely gives no picture (FLACAddPictureFileW would otherwise replace a picture with an empty one)
	if (!CTools::readWholeFile(fileName, data, BLOCK_MAX))   // a larger picture does not fit into a metadata block
		return false;
	// rebuild the MIME type
	_mime = CTools::instance().ExtractMimeFromPicture(data.m_pData, data.GetLength());
	calcInfos();
	return true;
}

// the picture data from memory: the MIME type, the size and the colors are determined from the data as for a picture from a file
void CFlacCover::setPictureData(const BYTE *arr, size_t length)
{
	data.Clear();
	_isLink = false;
	_pictureLink.Empty();
	_mime.Empty();
	if (arr != NULL && length > 0)
		data.AddMemory(arr, length);
	if (length >= 4)
		_mime = CTools::instance().ExtractMimeFromPicture(data.m_pData, data.GetLength());
	calcInfos();
}

bool CFlacCover::setFileLink(LPCWSTR fileName)
{
	data.Clear();
	_isLink = true;
	_pictureLink.Empty();
	if (fileName != NULL && wcslen(fileName) > 0)
	{
		_pictureLink = fileName;
		_mime = MIME_LINK;
		// the picture behind the link is loaded for the getters only, up to the size of a linked picture; the link is stored anyway
		const int lastError = CTools::getLastError();
		const CAtlString lastErrorText = CTools::GetLastErrorText();
		if (!CTools::readWholeFile(fileName, data, CTools::LINKED_PICTURE_MAX))
			CTools::restoreLastError(lastError, lastErrorText);
		calcInfos();
		return true;
	}
	CTools::instance().setLastError(ERR_INVALID_FILENAME);
	return false;
}

// width, height, color depth (bits per pixel) and the number of the colors of an indexed picture from the header of the picture data
void CFlacCover::calcInfos()
{
	width = 0;
	height = 0;
	colordepth = 24;
	colornumbers = 0;
	const size_t length = data.GetLength();
	if (length <= 4)
		return;
	switch (CTools::instance().CalcMimeFromPicture(data.m_pData, data.GetLength()))
	{
	case IMAGE_JPG:
		{
			// the segments follow each other: marker (0xFF and a code), length (16 bit, with the length field); the start of frame
			// (SOF0 to SOF15 without DHT 0xC4, JPG 0xC8 and DAC 0xCC) has precision (1 byte), height, width (2 bytes each) and the number of components
			size_t i = 2;
			while (i + 4 <= length && data.GetAt(i) == 0xFF)
			{
				const BYTE marker = data.GetAt(i + 1);
				if (marker == 0xFF)
				{
					i++;   // fill byte
					continue;
				}
				if (marker >= 0xC0 && marker <= 0xCF && marker != 0xC4 && marker != 0xC8 && marker != 0xCC)
				{
					if (i + 10 <= length)
					{
						height = data.Get2B(i + 5);
						width = data.Get2B(i + 7);
						colordepth = data.GetAt(i + 9) * data.GetAt(i + 4);   // components x precision
					}
					break;
				}
				if (marker == 0xD8 || (marker >= 0xD0 && marker <= 0xD7) || marker == 0x01)
					i += 2;   // markers without a length
				else if (marker == 0xDA)
					break;    // the scan data follow: no start of frame found
				else
					i += 2 + data.Get2B(i + 2);
			}
		}
		break;
	case IMAGE_GIF:
		// "GIF89a", the logical screen: width and height (16 bit little endian), flags: global color table (bit 7), its size 2^(n+1) (bits 0 to 2)
		if (length >= 11)
		{
			width = (unsigned short)data.GetR2B(6);
			height = (unsigned short)data.GetR2B(8);
			const BYTE flags = data.GetAt(10);
			colordepth = ((flags >> 4) & 7) + 1;   // bits per primary color of the palette
			if (flags & 0x80)
				colornumbers = 1 << ((flags & 7) + 1);
		}
		break;
	case IMAGE_PNG:
		// IHDR: width, height (32 bit big endian), bit depth, color type (0 gray, 2 RGB, 3 palette, 4 gray with alpha, 6 RGB with alpha)
		if (length >= 29)
		{
			width = data.Get4B(16);
			height = data.Get4B(20);
			const int depth = data.GetAt(24);
			const int colorType = data.GetAt(25);
			const int channels = (colorType == 2) ? 3 : (colorType == 4) ? 2 : (colorType == 6) ? 4 : 1;
			colordepth = depth * channels;
			if (colorType == 3)
			{
				// the number of the colors is the length of the PLTE chunk / 3
				size_t pos = 8;
				while (pos + 12 <= length)
				{
					const size_t chunk = (size_t)(unsigned long)data.Get4B(pos);
					if (memcmp(data.m_pData + pos + 4, "PLTE", 4) == 0)
					{
						colornumbers = (unsigned int)(chunk / 3);
						break;
					}
					if (chunk > length)
						break;
					pos += 12 + chunk;
				}
			}
		}
		break;
	case IMAGE_TIFF:
		// "II" little endian or "MM" big endian, the offset of the first directory, the entries: tag, type, count, value (12 bytes each)
		if (length > 8)
		{
			const bool little = data.GetAt(0) == 'I';
			auto read2 = [&](size_t pos) -> long { return little ? (unsigned short)data.GetR2B(pos) : (unsigned short)data.Get2B(pos); };
			auto read4 = [&](size_t pos) -> long { return little ? data.GetR4B(pos) : data.Get4B(pos); };
			const long offset = read4(4);
			if (offset > 8 && (size_t)offset + 2 < length)
			{
				size_t pos = (size_t)offset;
				long entries = read2(pos);
				pos += 2;
				while (entries > 0 && pos + 12 <= length)
				{
					const long tag = read2(pos);
					const long valueType = read2(pos + 2);
					const long value = (valueType == 3) ? read2(pos + 8) : read4(pos + 8);   // SHORT or LONG
					if (tag == 256)
						width = value;
					else if (tag == 257)
						height = value;
					pos += 12;
					entries--;
				}
			}
		}
		break;
	case IMAGE_BMP:
		// BITMAPINFOHEADER: width, height (32 bit little endian, the height is negative for a picture stored from the top), bits per pixel,
		// the number of the colors used
		if (length >= 50)
		{
			width = data.GetR4B(18);
			const long h = data.GetR4B(22);
			height = h < 0 ? -h : h;
			colordepth = (unsigned short)data.GetR2B(28);
			if (colordepth <= 8)
				colornumbers = data.GetR4B(46) != 0 ? data.GetR4B(46) : (1u << colordepth);
		}
		break;
	case IMAGE_WEBP:
		// "RIFF", size, "WEBP", then the first chunk (name and size, its data from byte 20 on): VP8 (lossy: frame tag, start code 9D 01 2A,
		// width and height with 14 bits), VP8L (lossless: signature 2F, width - 1 and height - 1 with 14 bits each, the alpha bit) or VP8X
		// (extended: flags with the alpha bit 0x10, 3 bytes reserved, width - 1 and height - 1 of the canvas with 24 bits each)
		if (length >= 30)
		{
			const BYTE *p = data.m_pData;
			if (memcmp(p + 12, "VP8 ", 4) == 0 && p[23] == 0x9D && p[24] == 0x01 && p[25] == 0x2A)
			{
				width = (p[26] | (p[27] << 8)) & 0x3FFF;
				height = (p[28] | (p[29] << 8)) & 0x3FFF;
			}
			else if (memcmp(p + 12, "VP8L", 4) == 0 && p[20] == 0x2F)
			{
				const u32 bits = (u32)p[21] | ((u32)p[22] << 8) | ((u32)p[23] << 16) | ((u32)p[24] << 24);
				width = (int)(bits & 0x3FFF) + 1;
				height = (int)((bits >> 14) & 0x3FFF) + 1;
				colordepth = (bits & (1u << 28)) ? 32 : 24;
			}
			else if (memcmp(p + 12, "VP8X", 4) == 0)
			{
				width = (int)((u32)p[24] | ((u32)p[25] << 8) | ((u32)p[26] << 16)) + 1;
				height = (int)((u32)p[27] | ((u32)p[28] << 8) | ((u32)p[29] << 16)) + 1;
				colordepth = (p[20] & 0x10) ? 32 : 24;
			}
		}
		break;
	}
}

// the size of the picture block (without its header) as Save writes it: type, the lengths and the 4 values (32 bytes), MIME type,
// description, the picture or the link
size_t CFlacCover::storedSize()
{
	CBlob text;
	text.AddEncodedString(TEXT_ENCODED_ANSI, _mime, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	size_t size = 32 + text.GetLength();
	text.Clear();
	text.AddEncodedString(TEXT_ENCODED_UTF8, description, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	size += text.GetLength();
	if (_isLink)
	{
		text.Clear();
		text.AddEncodedString(TEXT_ENCODED_UTF8, _pictureLink, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
		size += text.GetLength();
	}
	else
		size += data.GetLength();
	return size;
}

void CFlacCover::Save(CBlob * destination)
{
	CBlob tmp;
	destination->Add4B(type);
	tmp.AddEncodedString(TEXT_ENCODED_ANSI, _mime, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	destination->Add4B((unsigned int)tmp.GetLength());
	destination->AddBlob(tmp);
	tmp.Clear();
	tmp.AddEncodedString(TEXT_ENCODED_UTF8, description, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	destination->Add4B((unsigned int)tmp.GetLength());
	destination->AddBlob(tmp);
	destination->Add4B(width);
	destination->Add4B(height);
	destination->Add4B(colordepth);
	destination->Add4B(colornumbers);
	if (_isLink)
	{
		tmp.Clear();
		tmp.AddEncodedString(TEXT_ENCODED_UTF8, _pictureLink, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
		destination->Add4B((unsigned int)tmp.GetLength());
		destination->AddBlob(tmp);
	}
	else
	{
		destination->Add4B((unsigned int)data.GetLength());
		destination->AddBlob(data);
	}
}