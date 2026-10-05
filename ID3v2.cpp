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
#include "ID3v2.h"
#include "id3_framefactory.h"
#include "Tools.h"
#include "id3v1.h"
#include "io.h"

static void resyncTag(CBlob* data);


CID3V2::CID3V2(void)
{
	_frames.SetCount(0, 10);	
}

CID3V2::~CID3V2(void)
{
	ResetData();
}

short CID3V2::getEncoding(u32 frameID)
{
	size_t counts = _frames.GetCount();
	for (size_t i = 0; i < counts; i++)
	{
		if (_frames[i]->_frameID == frameID)
		{
			_frames[i]->decode();
			return _frames[i]->encodingID;
		}
	}
	return -1;
}

bool CID3V2::setTargetFormatAndEncoding(BYTE newFormat, BYTE newEncoding)
{
	ATLASSERT(newEncoding >= 0 && newEncoding <= 3);
	ATLASSERT(newFormat >= 0 && newFormat <= 3);
	if (newFormat > 0)
		newFormat++;
	// id3v2.2 and v2.3 know only the encodings ISO-8859-1 and UTF-16 with BOM; UTF-16BE and UTF-8 were introduced with v2.4
	const BYTE targetVersion = (newFormat != 0) ? newFormat : CTools::ID3V2newTagVersion;
	if (targetVersion != TAG_VERSION_2_4 && targetVersion != 0 && newEncoding >= 2)
	{
		CTools::instance().setLastError(ERR_UTF8_NOT_ALLOWED); 
		return false;
	}
	// 0 = use the default tag format
	if (newFormat != 0)
		CTools::ID3V2newTagVersion = newFormat;
	CTools::ID3V2defaultEncodingID = newEncoding;	
	return true;
}

bool CID3V2::ReadHeader(CFile *Stream)
{
	// An ID3v2 tag can be detected with the following pattern:
	// $49 44 33 yy yy xx zz zz zz zz
	// Where yy is less than $FF, xx is the 'flags' byte and zz is less than $80.
	memset(buf, 0, 10);
	TagDataSize = 0;
	CTools::readAt(Stream, 0, buf, 10);
	Size = 0;
	if (buf[0] == 'I' && buf[1] == 'D' && buf[2] == '3' && buf[3] < 0xFF && buf[4] < 0xFF)
	{
		Version = buf[3];
		Revision = buf[4];
		CTools::ID3V2Flags = buf[5];
		// the size field contains the extended header, the frames and the padding, but not the header and the footer
		TagDataSize = (buf[6] << 21) + ( buf[7] << 14) + (buf[8] << 7) + buf[9];
		Size = TagDataSize + 10;
		if (Version == TAG_VERSION_2_4 && (CTools::ID3V2Flags & 0x10) == 0x10)
			Size+= 10; // footer present (id3v2.4)
		if (Size > CTools::FileSize)
		{
			CTools::instance().writeError(L"ID3V2 tag is corrupt, because the ID3V2 size is bigger than filesize!");
			Size = 0;			
		}
		buf[3] = 0; buf[4] = 0;
		TagID = (buf[0] << 16) + (buf[1] << 8) + buf[2]; 
		return true;
	}
	return false;
}

void CID3V2::ResetData()
{
	Version = 0;
	Revision = 0;
	CTools::ID3V2Flags = 0;
	Size = 0;
	TagID = 0;
	size_t cnt = _frames.GetCount();
	if (cnt > 0)
	{
		for (size_t i = 0; i < cnt; i++)
			delete _frames.GetAt(i);	
	}
	_frames.RemoveAll();
}

void CID3V2::WriteHeader(CBlob *blob, long tagSize, bool unsynchronised)
{
	blob->AddValue('I');
	blob->AddValue('D');
	blob->AddValue('3');
	blob->AddValue(CTools::ID3V2newTagVersion);
	blob->AddValue(0);
	// flags: no extended header, no footer; the unsynchronisation bit (v2.2/v2.3 only - v2.4 flags it per frame instead,
	// see CID3_Frame::storeFrame) is set by SaveTag once it actually had to unsynchronise the tag body
	blob->AddValue(unsynchronised ? 128 : 0);
	blob->AddS4B(tagSize); // SyncSafe Integer
}

void CID3V2::ReadFromFile(CFile *Stream)
{
	CTools::ID3V2oldTagVersion = 0;
	size_t count = _frames.GetCount();
	for (size_t i = 0; i < count; i++)
		delete _frames.GetAt(i);
	_frames.RemoveAll();
	CTools::instance().writeDebug(_T("Analyze id3v2 tag"));
	ReadHeader(Stream);	
	oldTagSize = Size;
	CTools::ID3v2Size = Size;
	if (Size > 0 && TagID == ID3V2_TAGID && Version >= TAG_VERSION_2_2 && Version <= TAG_VERSION_2_4 )
	{
		CTools::instance().writeDebug(_T("id3v2.%i tag found"), Version);
		CTools::ID3V2oldTagVersion = Version;
		CBlob data(TagDataSize + 10);
		data.FileReadAt(Stream, 10, TagDataSize);   // behind the header of 10 bytes
		if (Version == TAG_VERSION_2_2 && (CTools::ID3V2Flags & 0x40) == 0x40)
			CTools::instance().writeWarning(L"id3v2.2 tag is compressed: no compression scheme is defined, the tag is ignored");
		else
		{
			// v2.2 and v2.3: the unsynchronisation applies to the whole tag (frame sizes are those of the original data)
			if (Version < TAG_VERSION_2_4 && (CTools::ID3V2Flags & 0x80) == 0x80)
				resyncTag(&data);
			parseTags(&data);
		}
	}
	else
		CTools::instance().writeDebug(_T("no id3v2 tag present"));
}

// Unsynchronisation (ID3v2 spec), applied to the whole tag body at once for v2.2/v2.3 (v2.4 does it per frame instead, see
// CID3_Frame::storeFrame): after every 0xFF byte that is followed by nothing, by 0x00, or by a byte >= 0xE0 (i.e. that could
// look like the start of an MPEG frame sync to a decoder that scans for one without respecting tag boundaries), an extra
// 0x00 byte is inserted - exactly what resyncTag() below undoes again when such a tag is read. Returns whether 'data'
// actually needed it (and was rewritten in place); leaves 'data' untouched if not, so an ordinary tag is not needlessly
// grown by this.
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
		return false;
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

// Transforms all FF 00 sequences into FF
static void resyncTag(CBlob* data)
{
	const size_t length = data->GetLength();
	BYTE *dest = new BYTE[length + 1];
	const BYTE *src = data->m_pData;
	const BYTE *end = src + length;
	size_t ln = 0;
	while (src < end)
	{
		dest[ln++] = *src;
		src += (src[0] == 0xFF && (src + 1 < end) && src[1] == 0) ? 2 : 1;
	}
	data->Clear();
	data->AddMemory(dest, (long)ln);
	delete [] dest;
}

// true if a frame that ends at 'end' (position in the tag body) is followed by something that can follow a frame: the end of the tag, the
// padding (a zero byte) or the header of a next frame. Used to decide how the size of a v2.4 frame is to be read, see parseTags().
// Four capital letters alone are not enough for a frame header: they are also found in the text of a frame (for example "ABBA"). The size
// in the header has to fit into the tag as well, as a synchsafe integer or as an ordinary number.
static bool frameEndsCleanly(CBlob* data, u32 dataSize, unsigned __int64 end)
{
	if (end > dataSize)
		return false;
	if (end == dataSize || data->GetAt((size_t)end) == 0)
		return true;
	if (end + 10 > dataSize)
		return false;
	const size_t pos = (size_t)end;
	if (!(ALLOWED[data->GetAt(pos)] && ALLOWED[data->GetAt(pos + 1)] && ALLOWED[data->GetAt(pos + 2)] && ALLOWED[data->GetAt(pos + 3)]))
		return false;
	const unsigned __int64 afterNext = end + 10;
	return afterNext + (u32)data->GetS4B(pos + 4) <= dataSize || afterNext + (u32)data->Get4B(pos + 4) <= dataSize;
}

void CID3V2::parseTags(CBlob* data)
{
	u32 DataPosition = 0;
	int headerSize = ( Version == TAG_VERSION_2_2) ? 6 : 10;
	u32 dataSize = (u32)data->GetLength();
	// extended header (v2.3 and v2.4): skip it. v2.4: the size field includes itself; v2.3: it does not
	if ((CTools::ID3V2Flags & 0x40) == 0x40 && dataSize >= 4)
	{
		u32 extendedSize = 0;
		if (Version == TAG_VERSION_2_4)
			extendedSize = data->GetS4B(0);
		else if (Version == TAG_VERSION_2_3)
			extendedSize = data->Get4B(0) + 4;
		if (extendedSize >= 6 && extendedSize <= dataSize)
			DataPosition = extendedSize;
	}
	// v2.4: the size of a frame is a synchsafe integer (7 bits per byte). Some taggers (e.g. old iTunes versions) write an ordinary 32 bit
	// number instead, which is only the same up to a size of 127 bytes. Once a frame of a tag has shown this, all frames of the tag are read that way.
	bool plainSizes = false;
	while ((DataPosition + headerSize) < dataSize)
	{
		ATLTRACE(_T("Reading at Pos:%d \n"), DataPosition);
		if (Version == TAG_VERSION_2_2)
		{
			FrameID = data->Get3B(DataPosition);
			FrameSize = data->Get3B(DataPosition + 3);
			FrameFlags = 0;
			isValid = ( ALLOWED[data->GetAt(DataPosition)] && ALLOWED[data->GetAt(DataPosition+1)] && ALLOWED[data->GetAt(DataPosition+2)]);			
		}
		else
		{
			FrameID = data->Get4B(DataPosition);
			//Stream->read(buf, 10);
			if (CTools::ID3V2oldTagVersion == TAG_VERSION_2_4)
			{
				// the size as a synchsafe integer (as the specification says) and as an ordinary number (wrong, but written by some taggers)
				const u32 synchsafeSize = (u32)data->GetS4B(DataPosition + 4);
				const u32 plainSize = (u32)data->Get4B(DataPosition + 4);
				FrameSize = synchsafeSize;
				if (!plainSizes && synchsafeSize != plainSize)
				{
					// The ordinary number is used if the bytes cannot be a synchsafe integer (a size byte with bit 7 set), or if the synchsafe
					// reading does not end in front of the next frame (or the padding, or the end of the tag) but the ordinary one does.
					// A size below 128 is the same in both and is never in doubt.
					const bool sevenBitBytes = ((data->GetAt(DataPosition + 4) | data->GetAt(DataPosition + 5) | data->GetAt(DataPosition + 6) | data->GetAt(DataPosition + 7)) & 0x80) == 0;
					const unsigned __int64 afterSynchsafe = (unsigned __int64)DataPosition + headerSize + synchsafeSize;
					const unsigned __int64 afterPlain = (unsigned __int64)DataPosition + headerSize + plainSize;
					if (!sevenBitBytes || (!frameEndsCleanly(data, dataSize, afterSynchsafe) && frameEndsCleanly(data, dataSize, afterPlain)))
					{
						plainSizes = true;
						CTools::instance().writeWarning(L"id3v2.4 tag with frame sizes that are not synchsafe, read as ordinary numbers");
					}
				}
				if (plainSizes)
					FrameSize = plainSize;
			}
			else // v2.3: ordinary number
				FrameSize = data->Get4B(DataPosition + 4);
			FrameFlags = data->Get2B(DataPosition + 8);
			// is Data Length Indicator set?

			isValid = ( ALLOWED[data->GetAt(DataPosition)] && ALLOWED[data->GetAt(DataPosition+1)] && ALLOWED[data->GetAt(DataPosition+2)] && ALLOWED[data->GetAt(DataPosition+3)] );			
			// some taggers (e.g. old iTunes versions) write the 3 character IDs of v2.2 into the 10 byte frame header of v2.3, padded with a
			// zero byte ("TT2" + zero byte): the frame is read with its v2.2 ID
			if (!isValid && (FrameID & 0xFF) == 0 && ALLOWED[data->GetAt(DataPosition)] && ALLOWED[data->GetAt(DataPosition+1)] && ALLOWED[data->GetAt(DataPosition+2)])
			{
				FrameID >>= 8;
				isValid = true;
			}
		}
		if (FrameID == 0) // 4 zero bytes: padding area
			break;
		// if the frame is invalid, continue analyzing at the next byte  
		if (!isValid)
		{
			DataPosition++;
		}
		else
		{
			ATLTRACE(_T("Size:%i "), FrameSize);
			if ((unsigned __int64)DataPosition + headerSize + FrameSize > dataSize) // 64 bit so that the check cannot overflow
			{
				
				CTools::instance().setLastError(ERR_FRAME_CORRUPT, BYTE(FrameID >> 24), BYTE(FrameID >> 16), BYTE(FrameID >> 8), BYTE(FrameID), DataPosition + headerSize);
				DataPosition = dataSize;
			}
			else
			{
				if (FrameSize > oldTagSize)
					FrameSize = oldTagSize;
				CID3_Frame *frame = CID3_FrameFactory::instance().createFrame(FrameID);
				frame->flags = FrameFlags;
				// v2.4: the unsynchronisation flag in the tag header says that unsynchronisation is applied on all frames. Many taggers set only this
				// flag and not the flag of the frames, so every frame is read as if it had the flag (as TagLib and mutagen do). If the frame has the
				// flag as well, nothing changes: the data are decoded only once. Only the frames of the tag get it, not the frames inside of a
				// chapter frame (they are part of the data of the chapter frame, which is decoded as a whole).
				if (Version == TAG_VERSION_2_4 && (CTools::ID3V2Flags & 0x80) == 0x80)
					frame->flags |= 0x0002;
				frame->load(data->m_pData + DataPosition + headerSize, FrameSize);
#ifdef DEBUG
				frame->print();
#endif
				_frames.Add(frame);
				DataPosition+= headerSize + FrameSize;
			}
		}		
	}
}

bool CID3V2::SaveToFile(LPCWSTR FileName)
{
	CFile *Stream;
	/* Check for existing tag */
	ATLTRACE(_T("open %s CFile::Mode::Read\n"), FileName); 
	if ( (Stream = CFile::openFile(FileName, CFile::Mode::Read, CFile::Share::All)) != NULL)
	{
		ReadHeader(Stream);
		oldTagSize = Size;
		CFile::closeFile(Stream);
		return SaveTag(FileName);
	}
	CTools::instance().setLastError(errno);
	return false;
}


// The date frames differ between the versions: v2.2/v2.3 TYER (year), TDAT (DDMM), TIME (HHMM), TORY (original year); v2.4 TDRC
// (timestamp yyyy-MM-ddTHH:mm) and TDOR. The frames of the other version are created before the tag is written.
void CID3V2::convertFramesForVersion()
{
	if (CTools::ID3V2newTagVersion == TAG_VERSION_2_4)
	{
		const CAtlString tyer = GetText(F_TYER), tdat = GetText(F_TDAT), tim = GetText(F_TIME), tory = GetText(F_TORY);
		if (findFrame(F_TDRC) == NULL && tyer.GetLength() >= 4)
		{
			CAtlString timestamp = tyer.Left(4);
			if (tdat.GetLength() >= 4)
			{
				timestamp += _T("-") + tdat.Mid(2, 2) + _T("-") + tdat.Left(2);
				if (tim.GetLength() >= 4)
					timestamp += _T("T") + tim.Left(2) + _T(":") + tim.Mid(2, 2);
			}
			SetText(F_TDRC, timestamp);
		}
		if (findFrame(F_TDOR) == NULL && tory.GetLength() >= 4)
			SetText(F_TDOR, tory.Left(4));
	}
	else
	{
		const CAtlString tdrc = GetText(F_TDRC), tdor = GetText(F_TDOR);
		if (findFrame(F_TYER) == NULL && tdrc.GetLength() >= 4)
		{
			SetText(F_TYER, tdrc.Left(4));
			if (tdrc.GetLength() >= 10 && tdrc.GetAt(4) == '-' && tdrc.GetAt(7) == '-')
			{
				SetText(F_TDAT, tdrc.Mid(8, 2) + tdrc.Mid(5, 2));
				if (tdrc.GetLength() >= 16 && tdrc.GetAt(10) == 'T' && tdrc.GetAt(13) == ':')
					SetText(F_TIME, tdrc.Mid(11, 2) + tdrc.Mid(14, 2));
			}
		}
		if (findFrame(F_TORY) == NULL && tdor.GetLength() >= 4)
			SetText(F_TORY, tdor.Left(4));
	}
}

bool CID3V2::SaveTag(LPCWSTR FileName)
{
	convertFramesForVersion();
	// build the frame data first (not just its size): v2.2/v2.3 unsynchronise the whole tag body as one afterwards, which
	// can grow it, so the exact final size has to come from what was actually built rather than from calcTagSize()'s
	// analytical estimate (still correct: identical to it whenever nothing needs unsynchronising, see below)
	CBlob body;
	storeFrames(&body);
	const bool unsynchronised = (CTools::ID3V2newTagVersion != TAG_VERSION_2_4) && applyUnsynchronisation(body);
	u32 tagSize = 10 + (u32)body.GetLength();
	bool needRebuild = false;
	long paddingSize = 0;
	long paddingBlockSize = CTools::configValues[CONFIG_ID3V2PADDINGSIZE];
	u32 optimalSize = tagSize;
	if (paddingBlockSize > 0)
		optimalSize = ((u32)(tagSize / paddingBlockSize) + 1) * paddingBlockSize;

	if (tagSize > oldTagSize || optimalSize < oldTagSize || paddingBlockSize == 0)
	{
		long newTagSize = optimalSize;
		paddingSize = newTagSize - tagSize;
		tagSize = newTagSize;
		needRebuild = true;
	}
	else
	{
		paddingSize = oldTagSize - tagSize;
		tagSize = oldTagSize;
		needRebuild = false;
	}
	CBlob *newData = new CBlob(tagSize);
	WriteHeader(newData, tagSize - 10, unsynchronised);
	newData->AddBlob(body);
	if (paddingSize > 0)
		newData->AddValue(0, paddingSize);
	bool result;
	if (needRebuild)
		result = RebuildFile(FileName, newData);
	else
		result = ReplaceTag(FileName, newData);
	delete newData;
	return result;
}

bool CID3V2::ReplaceTag(LPCWSTR FileName, CBlob* data)
{
	CFile *Stream;	
	CTools::instance().writeDebug(_T("Replace id3v2 Tag")); 
	if ( (Stream = CFile::openFile(FileName, CFile::Mode::ReadWrite, CFile::Share::Read)) != NULL)
	{
		data->FileWrite(data->GetLength(), Stream);
		Stream->flush();
		CFile::closeFile(Stream);
		return true;
	}
	CTools::instance().setLastError(errno);
	return false;
}

bool CID3V2::RebuildFile(LPCWSTR FileName, CBlob* data)
{
	CFile *Source;
	CFile *Destination;
	CAtlString NewFileName(FileName);
	long FrameOldSize = 0;
	CTools::instance().writeDebug(_T("Rebuild id3v2 tag")); 
	if ( (Source = CFile::openFile(FileName, CFile::Mode::Read, CFile::Share::All)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}

	/* Rebuild file with old file data and new tag data (optional) */
	ReadHeader(Source);
	FrameOldSize = Size;
	if (data == NULL && FrameOldSize == 0)
	{
		CTools::instance().setLastError(ERR_TAG_NOT_EXIST);
		CFile::closeFile(Source);
		return false;
	}

	/* Create file streams */
	NewFileName+=TILDE;
	if ( (Destination = CFile::openFile(NewFileName, CFile::Mode::Write, CFile::Share::Read)) == NULL)
	{
		CTools::instance().setLastError(errno);
		CFile::closeFile(Source);
		return false;
	};

	size_t blockSize = (size_t)CTools::configValues[CONFIG_ID3V2WRITEBLOCKSIZE];
	if (blockSize < 4096)
		blockSize = 4096;
	CBlob tmp(blockSize);
	bool ok = true;
	/* Copy data blocks */
	if (!Source->seek(FrameOldSize))
	{
		CTools::instance().writeError(L"ID3V2 fseek(%i) failed, aborting save", FrameOldSize);
		ok = false;
	}
	if (ok && data != NULL)
	{
		size_t dataLen = data->GetLength();
		if (data->FileWrite(dataLen, Destination) != dataLen)
			ok = false;
	}

	// Copy data in Blocks for large files
	while (ok)
	{
		ATLTRACE(_T("write %u bytes\n"), (unsigned)blockSize);
		tmp.FileRead(blockSize, Source);
		size_t got = tmp.GetLength();
		if (got > 0 && tmp.FileWrite(got, Destination) != got)
		{
			ok = false; // write error, e.g. disk full
			break;
		}
		CTools::instance().doEvents();
		if (got < blockSize)
		{
			if (Source->failed())
				ok = false;
			break;
		}
	}
	if (ok && (Destination->failed() || !Destination->flush()))
		ok = false;
	int writeErr = errno;
	CFile::closeFile(Destination);
	CFile::closeFile(Source);
	if (!ok)
	{
		// the original stays unchanged, only the temporary file is removed
		CFile::removeFile(NewFileName);
		CTools::instance().setLastError(writeErr != 0 ? writeErr : EIO);
		return false;
	}
	CTools::instance().doEventsNow();
	/* replace the old file with the new file in one step */
	if (!MoveFileExW(NewFileName, FileName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		CFile::removeFile(NewFileName);
		CTools::instance().setLastError(EACCES);
		return false;
	}
	return true;
}

bool CID3V2::RemoveFromFile(LPCWSTR FileName)
{
	CTools::instance().writeDebug(_T("try to remove id3v2 tag")); 
	return RebuildFile(FileName, NULL);
}

CAtlString CID3V2::GetText(u32 ID)
{
	if (_frames.GetCount() == 0)
		return EMPTY;
	CID3_Frame *f = findFrame(ID);
	return (f != NULL && f->isTextFrame()) ? cT000(f)->getText() : EMPTY;
}

CAtlString CID3V2::GetURL(u32 ID)
{
	if (_frames.GetCount() == 0)
		return EMPTY;
	CID3_Frame *f = findFrame(ID);
	return (f != NULL && f->isUrlFrame()) ? cW000(f)->getURL() : EMPTY;
}

void CID3V2::SetText(u32 ID, LPCWSTR newText)
{
	replaceFrame(new CID3F_T000(ID, newText));	
}

void CID3V2::SetURL(u32 ID, LPCWSTR newURL)
{
	replaceFrame(new CID3F_W000(ID, newURL));
}

CAtlString CID3V2::getAllFrameIDs()
{
	CAtlString result;
	CSimpleArray<unsigned int> ids;
	size_t count = _frames.GetCount();
	u32 frame;
	for (size_t i = 0; i < count; i++)
	{
		frame = _frames[i]->_frameID;
		if (ids.Find(frame) == -1)
		{
			ids.Add(frame);
			if (result.GetLength() > 0)
				result.Append(_T(","));
			result.Append(_frames[i]->getFrameIDString());
		}		
	}
	return result;
}

CID3_Frame* CID3V2::findFrame(u32 ID, int index)
{
	int found = 0;
	size_t count = _frames.GetCount();
	for (size_t i = 0; i < count; i++)
	{
		if (_frames[i]->equals(ID) )
		{		
			found++;
			if (found == index)
				return _frames[i];
		}
	}
	return NULL;
}

CID3_Frame* CID3V2::findFrame(CAtlString id)
{
	size_t count = _frames.GetCount();
	for (size_t i = 0; i < count; i++)
	{
		if (_frames[i]->equals(F_CTOC))
		{		
			CID3F_CTOC* result = cCTOC(_frames[i]);
			if (result->getID().Compare(id) == 0)
				return result;
		}
		if (_frames[i]->equals(F_CHAP))
		{		
			CID3F_CHAP* result = cCHAP(_frames[i]);
			if (result->getID().Compare(id) == 0)
				return result;
		}
	}
	return NULL;
}

CID3_Frame* CID3V2::findNextFrame(u32 ID, size_t& index)
{
	size_t count = _frames.GetCount();
	while (index < count)
	{
		if (_frames[index]->equals(ID) )
			return _frames[index++];
		index++;
	}
	return NULL;
}

short CID3V2::deleteAllFrames(u32 ID)
{
	int count = (int) _frames.GetCount();
	short result = 0;
	while (--count >= 0)
	{
		if (_frames[count]->equals(ID) )
		{
			delete _frames[count];
			_frames.RemoveAt(count);
			result++;
		}
	}
	return result;
}

short CID3V2::deleteAllFrames(CAtlString ID)
{
	int count = (int)_frames.GetCount();
	short result = 0;
	while (--count >= 0)
	{
		if (_frames[count]->equals(F_CTOC) || _frames[count]->equals(F_CHAP))
		{		
			CID3F_Chapter* chapter = cCHAPTER(_frames[count]);
			if (chapter->getID().Compare(ID) == 0)
			{
				delete _frames[count];
				_frames.RemoveAt(count);
				result++;
			}
		}		
	}
	return result;
}


bool CID3V2::deleteFrame(u32 ID, int index)
{
	int found = 0;
	size_t count = _frames.GetCount();
	for (size_t i = 0; i < count; i++)
	{
		if (_frames[i]->equals(ID) )
		{		
			found++;
			if (found == index)
			{
				delete _frames[i];
				_frames.RemoveAt(i);	
				return true;
			}
		}
	}	
	return false;
}
short CID3V2::countFrame(u32 ID)
{
	short result = 0;
	size_t count = _frames.GetCount();
	for (size_t i = 0; i < count; i++)
	{
		if (_frames[i]->equals(ID) )
			result++;
	}
	return result;
}

bool CID3V2::replaceFrame(CID3_Frame *frame)
{
	size_t count = _frames.GetCount();
	for (size_t i = 0; i < count; i++)
	{
		if (frame->equals(_frames[i]))
		{
			delete _frames[i];
			_frames.SetAt(i, frame);
			return true;
		}
	}
	_frames.Add(frame);
	return false;
}

void CID3V2::storeFrames(CBlob *blob)
{
	size_t counts = _frames.GetCount();
	for (size_t i = 0; i < counts; i++)
		_frames[i]->storeFrame(blob);
}

u32 CID3V2::calcTagSize()
{
	long wide = CTools::ID3V2newTagVersion == TAG_VERSION_2_2 ? 6 : 10;
	/* Calculate new tag size (without padding) */
	u32 TagSize = 10;
	CID3_Frame* frame;
	u32 id;
	size_t count = _frames.GetCount();
	for (size_t i = 0; i < count; i++)
	{
		frame = _frames[i];
		id = CID3_FrameFactory::instance().findTagForVersion(frame->_frameID);
		if (id != F_NONE && frame->canStoreFor(CTools::ID3V2newTagVersion))
			TagSize+= frame->getStoredSize() + wide;		
	}
	return TagSize;
}

// One reference of the genre frame: a number of the ID3v1 list, the keywords RX (remix) and CR (cover) or free text
static CAtlString resolveGenreReference(const CAtlString &reference)
{
	if (reference.Compare(_T("RX")) == 0)
		return CAtlString(_T("Remix"));
	if (reference.Compare(_T("CR")) == 0)
		return CAtlString(_T("Cover"));
	if (reference.GetLength() > 0 && reference.GetLength() <= 3 && reference.SpanIncluding(_T("0123456789")).GetLength() == reference.GetLength())
	{
		const int number = _wtoi(reference);
		if (number >= 0 && number < MAX_MUSIC_GENRES)
			return CAtlString(MUSIC_GENRE[number]);
	}
	return reference;
}

// v2.3: "(21)", "(RX)", "(21)(22)Refinement", "((" is a literal "("; v2.4: numbers and keywords as separate strings or free text
CAtlString CID3V2::GetGenre()
{
	CAtlString genre = GetText(F_TCON);
	if (genre.GetLength() == 0)
		return genre;
	if (genre.GetLength() >= 2 && genre.GetAt(0) == '(' && genre.GetAt(1) == '(')
		return genre.Mid(1);
	int pos = 0;
	CAtlString first;
	while (pos < genre.GetLength() && genre.GetAt(pos) == '(' && !(pos + 1 < genre.GetLength() && genre.GetAt(pos + 1) == '('))
	{
		const int end = genre.Find(')', pos + 1);
		if (end < 0)
			break;
		if (first.IsEmpty())
			first = resolveGenreReference(genre.Mid(pos + 1, end - pos - 1));
		pos = end + 1;
	}
	if (pos == 0)
		return resolveGenreReference(genre);
	CAtlString refinement = genre.Mid(pos);
	if (refinement.GetLength() >= 2 && refinement.GetAt(0) == '(' && refinement.GetAt(1) == '(')
		refinement = refinement.Mid(1);
	return refinement.IsEmpty() ? first : refinement;
}
bool CID3V2::parseCueFile(LPCWSTR FileName)
{
	CFile *Source;
	CTools::instance().writeDebug(_T("parse cue file")); 
	if ( (Source = CFile::openFile(FileName, CFile::Mode::Read, CFile::Share::All)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	// read everything into the blob
	int ln = (int)toSizeClamped(Source->size());
	CBlob dummy(ln + 1);
	dummy.FileRead(ln, Source);
	CFile::closeFile(Source);
	// split the blob into pieces; separators are 0D, 0A, EOL and blanks
	int pos = 0, start = 0;
	CAtlArray<CAtlString> tokens;
	dummy.AddValue(13);
	CID3F_CTOC *toc = new CID3F_CTOC(_T("toc1"), EMPTY, EMPTY, true);
	CID3F_CHAP *chap = NULL, *lastChap = NULL;
	long lastTime = 0;
	toc->setRoot(true);
	replaceFrame(toc);
	while (pos < ln)
	{
		if (dummy.GetAt(pos) == '"') // quotation marks
		{
			do // read up to the next quotation mark or the end of the line
			{
				pos++;
			}
			while (!(dummy.GetAt(pos) == '"' || dummy.GetAt(pos) == 13 || dummy.GetAt(pos) == 10));
			if (pos - start > 0)
				tokens.Add(dummy.GetStringAt(start + 1, pos - start - 1));
			start = pos + 1;
		}
		if (dummy.GetAt(pos) == ' ') // blank	
		{
			if (pos - start > 0)
				tokens.Add(dummy.GetStringAt(start, pos - start));
			start = pos + 1;
		}
		else if (dummy.GetAt(pos) == 13 || dummy.GetAt(pos) == 10)
		{
			if (pos - start > 0)
				tokens.Add(dummy.GetStringAt(start, pos - start));
			start = pos + 1;
			// now there is a line with all relevant words
			// evaluate; if an error occurs, false is returned
			if (tokens.GetCount() > 1)
			{
				CAtlString key(tokens.GetAt(0));
				CAtlString tmp;
				if (key.CompareNoCase(_T("INDEX")) == 0)
				{
					// int nr = _wtoi(tokens.GetAt(1));
					tmp = tokens.GetAt(2); // the time is given in minutes (mm), seconds (ss) and frames (ff), where each second is divided into 75(!) frames.
					// parse time
					int p1, p2;
					p1 = tmp.Find(':', 0);
					p2 = tmp.Find(':', p1 + 1);
					long startTime = 0;
					if (p1 != -1 && p2 != -1)
						startTime = ( _wtoi(tmp.Left(p1)) * 60000l) + (_wtoi(tmp.Mid(p1 + 1, p2 - p1 - 1)) * 1000l) + (_wtoi(tmp.Mid(p2 + 1)) * 1000l / 76l);
					lastTime = startTime;			
					if (chap != NULL)
						chap->setTimes(startTime, startTime);
					if (lastChap != NULL)
						lastChap->setTimes(lastChap->getStartTime(), lastTime );
					lastChap = chap;
				}
				else if (key.CompareNoCase(_T("PERFORMER")) == 0)
				{
					if (chap != NULL)
						chap->addFrame(new CID3F_T000(F_TPE1, tokens.GetAt(1)));
					else
						toc->addFrame(new CID3F_T000(F_TPE1, tokens.GetAt(1)));
				}
				else if (key.CompareNoCase(_T("TITLE")) == 0)
				{
					if (chap != NULL )
						chap->setTitle(tokens.GetAt(1));
					else
						toc->setTitle(tokens.GetAt(1));

				}
				else if (key.CompareNoCase(_T("TRACK")) == 0)
				{
						tmp.Format(_T("ch%i"), _wtoi(tokens.GetAt(1)));
						chap = new CID3F_CHAP(tmp, EMPTY, EMPTY);
						replaceFrame(chap);
						toc->addChildElement(tmp);										
				}				
			}
			// reset the line
			tokens.RemoveAll();
		}
		pos++;
	}
	return true;
}
