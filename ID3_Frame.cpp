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

#include "StdAfx.h"
#include "ID3_Frame.h"
#include "ID3v2.h"
#include "Tools.h"
#include "ID3_FrameFactory.h"
#include <vector>
extern "C" {
#include "puff.h"   // third-party (Mark Adler, zlib license, see THIRD-PARTY-NOTICES.md): raw DEFLATE decoder, no dependencies
}

bool CID3_Frame::equals(const unsigned int ID)
{
	return (this->_frameID == ID);
}

bool CID3_Frame::equals(CID3_Frame *frame)
{
	return (this->_frameID == frame->_frameID);
}

CID3_Frame::CID3_Frame(void)
{
	init(0);
}

CID3_Frame::CID3_Frame(unsigned int frameID)
{
	init(frameID);
}

void CID3_Frame::init(unsigned int frameID)
{
	_frameID = frameID;	
	isDecoded = false;
	mustRebuild = true;
	encodingID = 255;
	useTextEncoding = false;
	flags = 0;
	_discardOnTagAlter = _discardOnFileAlter = _readOnly = _grouped = _compressed = _encrypted = _hasDataLength = _unsyncResolved = _loaded = false;
	_unsyncChecked = _unsyncOnWrite = false;
	_groupId = _encryptionMethod = 0;
	_dataLength = 0;
}

CID3_Frame::~CID3_Frame(void)
{
	_blob.Clear();
	_data.Clear();
}

// length of the additional header fields of this frame in a tag of the given version
static long extraFieldsSize(CID3_Frame *f, BYTE version)
{
	if (version != TAG_VERSION_2_3 && version != TAG_VERSION_2_4)
		return 0;
	return (f->_grouped ? 1 : 0) + (f->_encrypted ? 1 : 0) + ((f->_compressed || (f->_encrypted && f->_hasDataLength)) ? 4 : 0);
}

// An unknown frame is written again with its ID, unless its flag says that it is to be discarded when the tag is altered
bool CID3_Frame::canStoreFor(BYTE /*version*/)
{
	return !(_discardOnTagAlter && !CID3_FrameFactory::instance().isKnownFrameID(_frameID));
}

long CID3_Frame::getStoredSize()
{
	return getSize() + extraFieldsSize(this, CTools::ID3V2newTagVersion);
}

// Unsynchronisation (ID3v2 spec): after every 0xFF byte that is followed by nothing, by 0x00, or by a byte >= 0xE0 (i.e.
// that could look like the start of an MPEG frame sync to a decoder that scans for one without respecting tag boundaries),
// an extra 0x00 byte is inserted. This is exactly what CID3_Frame::resync()/CID3V2::resyncTag() undo again when such a tag
// is read. Returns whether 'data' actually needed it (and was rewritten in place); leaves 'data' untouched if not, so a
// frame/tag whose content never contains a risky sequence is not needlessly grown by this.
static bool needsUnsync(const BYTE *data, size_t length, size_t pos)
{
	return data[pos] == 0xFF && (pos + 1 == length || data[pos + 1] == 0 || data[pos + 1] >= 0xE0);
}

static bool applyUnsynchronisation(CBlob &data)
{
	const BYTE *src = data.m_pData;
	const size_t length = data.GetLength();
	size_t i = 0;
	for (; i < length; i++)
		if (needsUnsync(src, length, i))
			break;
	if (i == length)
		return false;   // nothing in here ever looks like a frame sync: leave the data exactly as it was
	CBlob stuffed;
	for (i = 0; i < length; i++)
	{
		stuffed.AddValue(src[i]);
		if (needsUnsync(src, length, i))
			stuffed.AddValue(0);
	}
	data.Clear();
	data.AddBlob(stuffed);
	return true;
}

long CID3_Frame::getSize()
{
	bool rebuilt = mustRebuild;
	// check the encoding
	if (useTextEncoding)
	{
		decode();
		if (CTools::ID3V2defaultEncodingID != encodingID)
		{
			encodingID = CTools::ID3V2defaultEncodingID;
			mustRebuild = true;
			rebuilt = true;
		}
	}
	CTools::lossyText = false;
	encode();
	// ISO-8859-1 (code page) cannot represent every character: store such a frame as UTF-16 instead of replacing characters by ?
	if (useTextEncoding && encodingID == TEXT_ENCODED_ANSI && CTools::lossyText)
	{
		encodingID = TEXT_ENCODED_UTF16BOM;
		mustRebuild = true;
		rebuilt = true;
		encode();
	}
	if (rebuilt)
		_unsyncChecked = false;
	// v2.2/v2.3 unsynchronise the whole tag at once (CID3V2::SaveTag), not per frame
	if (CTools::ID3V2newTagVersion == TAG_VERSION_2_4 && !_unsyncChecked)
	{
		_unsyncOnWrite = applyUnsynchronisation(_blob);
		_unsyncChecked = true;
	}
	return (long)_blob.GetLength();
}

// Flag bits of the frame header. Status flags: id3v2.3 %abc00000, id3v2.4 %0abc0000 (first byte of the flags)
static bool statusFlag(u16 flags, u16 mask23, u16 mask24)
{
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_3)
		return (flags & mask23) != 0;
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4)
		return (flags & mask24) != 0;
	return false; // id3v2.2 has no flags
}

static u32 readSynchsafe(const BYTE *p)
{
	return ((u32)(p[0] & 0x7F) << 21) | ((u32)(p[1] & 0x7F) << 14) | ((u32)(p[2] & 0x7F) << 7) | (u32)(p[3] & 0x7F);
}

// RFC 1950 (zlib format) checksum of the decompressed data, to confirm puff() decoded a genuine zlib stream rather than
// something that merely happened to parse as one
static u32 adler32(const BYTE *data, size_t len)
{
	u32 a = 1, b = 0;
	for (size_t i = 0; i < len; i++)
	{
		a = (a + data[i]) % 65521u;
		b = (b + a) % 65521u;
	}
	return (b << 16) | a;
}

// A compressed ID3v2 frame is a zlib stream (RFC 1950): 2 byte header, raw DEFLATE data (RFC 1951), 4 byte Adler-32 of the
// decompressed data. puff() (third-party, see THIRD-PARTY-NOTICES.md) only implements the DEFLATE part, so the header is
// skipped here and the checksum is verified separately. The exact decompressed size is not trusted from the frame's own
// "data length" field (present but not authoritative if the frame is corrupt); it is instead determined with a first,
// output-less call to puff(), the same way its own documentation describes for this purpose. A generous but finite cap
// guards against a corrupt or adversarial frame claiming an enormous size (a "decompression bomb").
static const size_t MAX_DECOMPRESSED_FRAME_SIZE = 64 * 1024 * 1024;

static bool inflateZlibFrame(const BYTE *source, long size, CBlob &result)
{
	if (size < 6)   // shorter than a zlib header (2) + Adler-32 trailer (4) can ever legitimately be
		return false;
	const BYTE *deflateSrc = source + 2;
	unsigned long deflateSize = (unsigned long)(size - 2);

	unsigned long destLen = 0;
	unsigned long srcLen = deflateSize;
	if (puff(NIL, &destLen, deflateSrc, &srcLen) != 0)
		return false;
	if (destLen > MAX_DECOMPRESSED_FRAME_SIZE)
		return false;

	CBlob decompressed;
	decompressed.AddValue(0, destLen);   // reserve exactly destLen bytes to decompress into
	unsigned long outLen = destLen;
	srcLen = deflateSize;
	if (puff(decompressed.m_pData, &outLen, deflateSrc, &srcLen) != 0 || outLen != destLen)
		return false;
	// srcLen is now how many of the deflateSize bytes puff() actually consumed; the Adler-32 trailer follows right behind
	if (srcLen + 4 > deflateSize)
		return false;
	const BYTE *trailer = deflateSrc + srcLen;
	const u32 storedAdler = ((u32)trailer[0] << 24) | ((u32)trailer[1] << 16) | ((u32)trailer[2] << 8) | (u32)trailer[3];
	if (adler32(decompressed.m_pData, destLen) != storedAdler)
		return false;

	result.Clear();
	result.AddMemory(decompressed.m_pData, destLen);
	return true;
}

void CID3_Frame::load(BYTE* source, long size)
{
	const BYTE version = CTools::ID3V2oldTagVersion;
	_loaded = true;
	_discardOnTagAlter = statusFlag(flags, 0x8000, 0x4000);
	_discardOnFileAlter = statusFlag(flags, 0x4000, 0x2000);
	_readOnly = statusFlag(flags, 0x2000, 0x1000);
	_grouped = isGrouped();
	_compressed = isCompressed();
	_encrypted = isEncrypted();
	// v2.4: data length indicator flag; v2.3: a compressed frame starts with the decompressed size
	_hasDataLength = (version == TAG_VERSION_2_4) ? isDataLenIndicator() : (version == TAG_VERSION_2_3 && _compressed);
	// additional header fields in the order of the flags: v2.4 group, encryption, data length; v2.3 decompressed size, encryption, group
	long extraSize = (_grouped ? 1 : 0) + (_encrypted ? 1 : 0) + (_hasDataLength ? 4 : 0);
	if (extraSize > size)
	{
		// corrupt frame: the additional fields do not fit into the frame, use the frame as it is
		_grouped = _encrypted = _hasDataLength = false;
	}
	long pos = 0;
	if (version == TAG_VERSION_2_4)
	{
		if (_grouped)
			_groupId = source[pos++];
		if (_encrypted)
			_encryptionMethod = source[pos++];
		if (_hasDataLength)
		{
			_dataLength = readSynchsafe(source + pos);
			pos+=4;
		}
	}
	else if (version == TAG_VERSION_2_3)
	{
		if (_hasDataLength)
		{
			_dataLength = ((u32)source[pos] << 24) | ((u32)source[pos + 1] << 16) | ((u32)source[pos + 2] << 8) | (u32)source[pos + 3];
			pos+=4;
		}
		if (_encrypted)
			_encryptionMethod = source[pos++];
		if (_grouped)
			_groupId = source[pos++];
	}
	// a compressed frame that is also encrypted cannot be decompressed here: the encryption method is vendor specific and
	// unknown to this library, so the bytes are not even valid DEFLATE data yet
	if (_compressed && !_encrypted && inflateZlibFrame(source + pos, size - pos, _blob))
	{
		// _blob now holds the decompressed data; nothing else to do
	}
	else
	{
		if (_compressed)
			CTools::instance().setLastError(ERR_COMPRESSED_FRAME_CORRUPT);
		_blob.AddMemory(source + pos, size - pos);
	}
	isDecoded = false;
	mustRebuild = false;
	// unsynchronisation is reversed here for every frame, so that the data are plain from now on and the frame is written without the flag
	if (isUnsynchronized())
	{
		resync();
		mustRebuild = false;
	}
	_unsyncResolved = true;
}

void CID3_Frame::save(CFile *stream)
{
	encode();
	_blob.FileWrite(_blob.GetLength(), stream);	
}

void CID3_Frame::print()
{
	ATLTRACE(_T("Frame:%c%c%c%c Size:%u\n"), BYTE(_frameID >> 24), BYTE(_frameID >> 16), BYTE(_frameID >> 8), BYTE(_frameID), _blob.GetLength());
}
CAtlString CID3_Frame::getFrameIDString()
{
	CAtlString dummy;
	dummy.Format(_T("%c%c%c%c"), BYTE(_frameID >> 24), BYTE(_frameID >> 16), BYTE(_frameID >> 8), BYTE(_frameID));
	return dummy;
}
long CID3_Frame::getData(BYTE *destination, long maxLen)
{
	decode();
	long ln = (long)_data.GetLength();
	if (ln > maxLen)
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY);
		return 0;
	}
	if (ln > 0)
		memcpy(destination, _data.m_pData, ln);
	return ln;
}

bool CID3_Frame::setData(BYTE *source, unsigned int maxLen)
{
	decode();
	_data.Clear();
	_data.AddMemory(source, maxLen);
	mustRebuild = true;
	return true;
}

void CID3_Frame::storeFrame(CBlob *tmp)
{
	u32 id = CID3_FrameFactory::instance().findTagForVersion(_frameID);
	if (id != F_NONE && !canStoreFor(CTools::ID3V2newTagVersion))
	{
		CTools::instance().writeWarning(L"frame '%s' ignored because its data cannot be converted into id3v2.%i", (LPCTSTR)getFrameIDString(), CTools::ID3V2newTagVersion);
		return;
	}
	if (id != F_NONE)
	{
		const BYTE version = CTools::ID3V2newTagVersion;
		// encode first, so that the size of the frame data is known
		const long dataSize = getSize();
		// flags and additional header fields in the layout of the new version
		u16 newFlags = 0;
		CBlob extra;
		if (version == TAG_VERSION_2_4)
		{
			// %0abc0000 %0h00kmnp
			if (_discardOnTagAlter) newFlags |= 0x4000;
			if (_discardOnFileAlter) newFlags |= 0x2000;
			if (_readOnly) newFlags |= 0x1000;
			if (_grouped) newFlags |= 0x0040;
			if (_compressed) newFlags |= 0x0008;
			if (_encrypted) newFlags |= 0x0004;
			const bool lengthField = _compressed || (_encrypted && _hasDataLength);
			if (lengthField) newFlags |= 0x0001;
			if (_unsyncOnWrite) newFlags |= 0x0002;   // n - Unsynchronisation
			if (_grouped) extra.AddValue(_groupId);
			if (_encrypted) extra.AddValue(_encryptionMethod);
			if (lengthField) extra.AddS4B((int)_dataLength);
		}
		else if (version == TAG_VERSION_2_3)
		{
			// %abc00000 %ijk00000
			if (_discardOnTagAlter) newFlags |= 0x8000;
			if (_discardOnFileAlter) newFlags |= 0x4000;
			if (_readOnly) newFlags |= 0x2000;
			if (_compressed) newFlags |= 0x0080;
			if (_encrypted) newFlags |= 0x0040;
			if (_grouped) newFlags |= 0x0020;
			if (_compressed) extra.Add4B(_dataLength);
			if (_encrypted) extra.AddValue(_encryptionMethod);
			if (_grouped) extra.AddValue(_groupId);
		}
		const long frameSize = dataSize + (long)extra.GetLength();
		switch (version)
		{
		case TAG_VERSION_2_2:
			{
				tmp->Add3B(id);
				tmp->Add3B(dataSize);
				break;
			}
		case TAG_VERSION_2_3:
			{
				tmp->Add4B(id);
				tmp->Add4B(frameSize);
				tmp->Add2B(newFlags);
				break;
			}
		case TAG_VERSION_2_4:
			{
				tmp->Add4B(id);
				tmp->AddS4B(frameSize);
				tmp->Add2B(newFlags);
				break;
			}
		}
		if (version != TAG_VERSION_2_2 && extra.GetLength() > 0)
			tmp->AddBlob(extra);
		tmp->AddBlob(_blob);
	}
	else
	{
		CTools::instance().writeWarning(L"frame '%s' ignored because it is not supported in id3v2.%i", (LPCTSTR)getFrameIDString(), CTools::ID3V2newTagVersion);
	}
}

//	To be used when reading an ID3v2-tag
//	Transforms all FF 00 sequences into FF
void CID3_Frame::resync()
{
	std::vector<BYTE> dest;
	dest.reserve(_blob.GetLength());
	const BYTE *src = _blob.m_pData;
	const BYTE *end = src + _blob.GetLength();
	while (src < end)
	{
		dest.push_back(*src);
		src += (src[0] == 0xFF && (src + 1 < end) && src[1] == 0) ? 2 : 1;
	}
	_blob.Clear();
	if (!dest.empty())
		_blob.AddMemory(dest.data(), dest.size());
	mustRebuild = true;
}

//	To be used when writing an ID3v2-tag
//	Transforms:
//	11111111 111xxxxx -> 11111111 00000000 111xxxxx
//	11111111 00000000 -> 11111111 00000000 00000000 
//	11111111 <EOF> -> 11111111 00000000 <EOF>

/*void CID3_Frame::unsync()
{
	BYTE *dest = new BYTE[_blob.GetLength() * 2];
	BYTE *src = _blob.m_pData;
	BYTE *end = src + _blob.GetLength();
	int ln = 0;
	while (src < end)
	{
		dest[ln++] = *src;
		if (src[0] == 0xFF && ((src+1 == end) || (src[1] == 0) || src[1] >= 0xE0))
			dest[ln++] = 0;
		src++;
	}
	_blob.Clear();
	_blob.AddMemory(dest, ln);
	delete [] dest;	
} */

/*
id3v2.3: %abc00000 %ijk00000
a - Tag alter preservation
b - File alter preservation
c - Read only
i - Compression
j - Encryption
k - Grouping identity

id3v2.4: %0abc0000 %0h00kmnp
a - Tag alter preservation
b - File alter preservation
c - Read only
h - Grouping identity
k - Compression
m - Encryption
n - Unsynchronisation
p - Data length indicator
*/

bool CID3_Frame::isUnsynchronized()
{
	if (_unsyncResolved) // already reversed in load()
		return false;
	// v2.2 and v2.3: the flag of the tag applies to the whole tag, which is reversed before the frames are read
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4) // v2.4: flag of the frame (the flag of the tag only says that all frames have it)
		return ((flags & 2) == 2);
	return false;
};

bool CID3_Frame::isReadOnly() 
{ 
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_3) // flags apply globally to all frames in v2.3
		return ((flags & 8192) == 8192); 
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4) 
		return ((flags & 4096) == 4096); 
	return 0; // id3v2.2 has no flags	
};   
bool CID3_Frame::isGrouped() 
{ 
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_3) // flags apply globally to all frames in v2.3
		return ((flags & 32) == 32); 
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4) 
		return ((flags & 64) == 64); 
	return 0; // id3v2.2 has no flags
};
bool CID3_Frame::isCompressed() 
{ 
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_3) // flags apply globally to all frames in v2.3
		return ((flags & 128) == 128); 
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4) 
		return ((flags & 8) == 8); 
	return 0; // id3v2.2 has no flags
}; 
bool CID3_Frame::isEncrypted() 
{ 
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_3) // flags apply globally to all frames in v2.3
		return ((flags & 64) == 64);
	if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4) 
		return ((flags & 4) == 4); 
	return 0; // id3v2.2 has no flags	
};
bool CID3_Frame::isDataLenIndicator() // v2.4 only
{ 
	return (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4) ? ((flags & 1) == 1) : 0;	
};