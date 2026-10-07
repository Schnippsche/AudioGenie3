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

// CMP4.cpp: implementation of class CMP4.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include "mp4.h"
#include <memory.h>
#include "resource.h"
#include "id3v1taginfo.h"
#include "mp4_atomfactory.h"
#include "mp4_soun.h"
#include "mp4_mdhd.h"
#include "mp4_mdat.h"
#include "mp4_stco.h"
#include "ID3V1.h"
#include <vector>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMP4::CMP4()
{
	mainContainer = new CMP4_MainContainer();
}

CMP4::~CMP4()
{
	mainContainer->remove();
	delete mainContainer;
}

void CMP4::ResetData()
{
	/* Reset variables */
	mainContainer->remove();
	CMP4_AtomFactory::lastOffset = 0;
	CMP4_AtomFactory::mediaLength = 0;
	CMP4_AtomFactory::lastMDHDAtom = NULL;
	CMP4_AtomFactory::firstAudioPos = 0;
	CMP4_AtomFactory::lastAudioPos = 0;
}

bool CMP4::ReadFromFile(CFile *Stream)
{
	//ResetData();
	CMP4_AtomFactory::lastAudioPos = CTools::FileSize;
	/* Read file data */
	//Stream->seek(CTools::ID3v2Size);
	mainContainer->load(Stream, CTools::ID3v2Size, (u64)(CTools::FileSize - CTools::ID3v1Size - CTools::ID3v2Size));
	return (mainContainer->find(FTYP_PFAD) != NULL);	
}

CAtlString CMP4::GetFileVersion()
{
	// get the ftyp atom
	CMP4Atom* atom = mainContainer->find(FTYP_PFAD);
	if (atom == NULL)
		return UNKNOWN;
	int majortype = atom->_blob.Get4B(0); // major type
	for (int i = 0; i < 49; i++)
	{
		if (MP4_FORMATCODES[i].id == majortype)
			return MP4_FORMATCODES[i].text;
	}
	return UNKNOWN;
}

int CMP4::GetPictureCount()
{ 
	CMP4Atom* atom = mainContainer->find(COVR_PFAD);
	if (atom == NULL)
		return 0;
	CMP4_Container *cont = static_cast<CMP4_Container*>(atom);
	return (int)cont->_children.GetCount();
}

CAtlString CMP4::GetPictureMime(int Index)
{
	if (Index < 1)
		return L"";
	CMP4Atom* atom = mainContainer->find(COVR_PFAD);
	if (atom == NULL)
		return L"";
	CMP4_Container *cont = static_cast<CMP4_Container*>(atom);
	if (Index <= (int)cont->_children.GetCount())
		atom = cont->_children[Index - 1];
	else
		return L"";
	// type of the data box: 13 JPEG, 14 PNG, 12 GIF, 27 BMP
	switch (atom->_blob.Get4B(0))
	{
	case 14: return _T("png");
	case 12: return _T("gif");
	case 27: return _T("bmp");
	default: return _T("jpg");
	}
}

long CMP4::GetPictureSize(int Index)
{
	if (Index < 1)
		return 0;
	CMP4Atom* atom = mainContainer->find(COVR_PFAD);
	if (atom == NULL)
		return 0;
	CMP4_Container *cont = static_cast<CMP4_Container*>(atom);
	if (Index <= (int)cont->_children.GetCount())
	{
		atom = cont->_children[Index - 1];
		return (long)atom->_blob.GetLength() - 8;
	}
	return 0;	
}

bool CMP4::GetPicture(LPCWSTR file, int Index)
{
	if (Index < 1)
		return false;
	CMP4Atom* atom = mainContainer->find(COVR_PFAD);
	if (atom == NULL)
		return false;
	CMP4_Container *cont = static_cast<CMP4_Container*>(atom);
	if (Index <= (int)cont->_children.GetCount())
		atom = cont->_children[Index - 1];
	else
		return false;
	// write from memory to file: the picture behind the 8 bytes of type and locale (an atom with fewer bytes has no picture; before, the
	// negative length became a huge size_t and the write read behind the buffer)
	if (atom->_blob.GetLength() < 8)
	{
		CTools::instance().setLastError(ERR_INVALID_FORMAT);
		return false;
	}
	return CTools::writeFile(file, atom->_blob.m_pData + 8, atom->_blob.GetLength() - 8);
}

bool CMP4::AddPictureFile(LPCWSTR FileName)
{
	CFile *Stream;
	if ( (Stream = CFile::openFile(FileName, CFile::Mode::Read, CFile::Share::All)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	long length = (long)toSizeClamped(Stream->size());
	CBlob arr(length);
	arr.FileRead(length, Stream);
	CFile::closeFile(Stream);
	return AddPictureArray(arr.m_pData, length);
}

bool CMP4::AddPictureArray(BYTE *arr, u32 length)
{
	// the picture type is recognized by the first 4 bytes: no pointer or fewer bytes is not a picture
	if (arr == NULL || length < 4)
	{
		CTools::instance().setLastError(ERR_PICTUREARRAY_TOO_SMALL);
		return false;
	}
	mainContainer->checkMetaBox();
	CMP4Atom* atom = mainContainer->find(COVR_PFAD);
	CMP4_Container *cont;
	// no picture yet, create a new container
	if (atom == NULL)
	{
		cont = new CMP4_Container(MP4_COVR);
		cont->setParent(ILST_PFAD);
		mainContainer->addAtom(cont);
	}
	else
		cont = static_cast<CMP4_Container*>(atom);
	atom = new CMP4Atom('data');
	atom->setParent(COVR_PFAD);
	// find out the picture type, JPEG or png
	BYTE picType = 13; // JPEG = Default
	if (arr[0] ==  0x89 && arr[1] == 0x50 && arr[2] == 0x4E && arr[3] == 0x47)
		picType = 14;
	else if (arr[0] == 'G' && arr[1] == 'I' && arr[2] == 'F')
		picType = 12;
	else if (arr[0] == 'B' && arr[1] == 'M')
		picType = 27;
	atom->_blob.Clear();
	atom->_blob.Add4B(picType);
	atom->_blob.Add4B(0);
	atom->_blob.AddMemory(arr, length);
	cont->_children.Add(atom);
	return true;
}

bool CMP4::DeletePictureFrame(short Index)
{
	CMP4Atom* atom = mainContainer->find(COVR_PFAD);
	if (atom == NULL)
		return false;
	CMP4_Container *cont = static_cast<CMP4_Container*>(atom);
	if (Index > 0 && Index <= (int)cont->_children.GetCount())
	{
		delete cont->_children[Index - 1];
		cont->_children.RemoveAt(Index - 1);
		return true;
	}
	return false;
}

void CMP4::DeletePictures()
{
	mainContainer->removeAtom(COVR_PFAD);
}

long CMP4::GetPictureArray(BYTE *destination, long maxLen, short Index)
{
	if (Index < 1)
		return -1;
	CMP4Atom* atom = mainContainer->find(COVR_PFAD);
	if (atom == NULL)
		return -1;
	CMP4_Container *cont = static_cast<CMP4_Container*>(atom);
	if (Index > 0 && Index <= (int)cont->_children.GetCount())
		atom = cont->_children[Index - 1];
	else
		return -1;
	// 4 bytes version/flags = byte hex version + 24-bit hex flags
	// 0D (13) = jpeg ,  0E (14) = png
	// 4 bytes reserved = 32-bit value set to zero
	// an atom with fewer than 8 bytes has no picture (before, the negative length was returned as the size)
	long ln = (atom->_blob.GetLength() > 8) ? (long)atom->_blob.GetLength() - 8 : 0;
	if (ln > maxLen || (destination == NULL && ln > 0))   // no array: as if it were too small
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY, (unsigned)ln);   // the array of the caller is too small: the size it needs
		return -1;
	}
	if (ln > 0)
		memcpy(destination, atom->_blob.m_pData + 8, ln);
	return ln;
}

long CMP4::GetBitRate()
{
	float duration = GetDuration();
	if (duration > 0)
	{
		if (CMP4_AtomFactory::mediaLength > 0)
			return (long)(CMP4_AtomFactory::mediaLength * 8.0 / duration / 1000.0 + 0.5);
		return (long)((CTools::FileSize - CMP4_AtomFactory::lastOffset) * 8.0 / duration / 1000.0 + 0.5);
	}
	return 0;
}

CAtlString CMP4::GetILSTText(u32 lastFrameID)
{
	CAtlString suchPfad;
	suchPfad.Format(_T("%s.%c%c%c%c"), ILST_PFAD, BYTE(lastFrameID >> 24), BYTE(lastFrameID >> 16), BYTE(lastFrameID >> 8), BYTE(lastFrameID)); 
	CMP4Atom* atom = mainContainer->find(suchPfad);
	if (atom == NULL)
		return EMPTY;
	return CMP4_AtomFactory::instance()->getText(atom);
}

void CMP4::SetILSTText(u32 FrameID, CAtlString newText)
{
	if (newText.IsEmpty())
	{
		CAtlString tmp;
		tmp.Format(_T("%s.%c%c%c%c"), ILST_PFAD, BYTE(FrameID >> 24), BYTE(FrameID >> 16), BYTE(FrameID >> 8), BYTE(FrameID)); 
		mainContainer->removeAtom(tmp);
		return;
	}
	CMP4Atom* atom = new CMP4Atom(FrameID);
	atom->setParent(ILST_PFAD);
	CMP4_AtomFactory::instance()->setText(atom, newText);
	mainContainer->checkMetaBox();
	mainContainer->replaceAtom(atom);
}

CAtlString CMP4::GetItuneText(CAtlString frame)
{
	CAtlString suchPfad;
	suchPfad.Format(_T("%s.%s"), ITUN_PFAD, (LPCTSTR)frame);
	CMP4Atom* atom = mainContainer->find(suchPfad);
	if (atom == NULL)
		return EMPTY;
	return CMP4_AtomFactory::instance()->getiTuneText(atom);	
}
void CMP4::SetItuneText(CAtlString frame, CAtlString newText)
{
	CAtlString suchPfad;
	suchPfad.Format(_T("%s.%s"), ITUN_PFAD, (LPCTSTR)frame); 
	if (newText.IsEmpty())
	{
		mainContainer->removeAtom(suchPfad);
		return;
	}
	mainContainer->checkMetaBox();
	CMP4Atom* atom = mainContainer->find(suchPfad);
	if (atom == NULL) // create new
	{
		atom = new CMP4Atom('----');
		atom->setParent(ILST_PFAD);
		CMP4_AtomFactory::instance()->setItuneText(atom, frame, newText);
		mainContainer->addAtom(atom);
	}
	else // replace: the owner (mean) of the item stays
	{
		const CAtlString mean = CMP4_AtomFactory::instance()->getiTuneMean(atom);
		CMP4_AtomFactory::instance()->setItuneText(atom, frame, newText, mean);
	}
}
CAtlString CMP4::getILSTFrameIDs()
{
	CMP4Atom* atom = mainContainer->find(ILST_PFAD);
	if (atom == NULL)
		return EMPTY;
	CAtlString result;
	CSimpleArray<CAtlString> ids;
	CMP4_Container *cont = static_cast<CMP4_Container*>(atom);
	size_t count = cont->_children.GetCount();
	u32 frame;
	CAtlString tmp;
	for (size_t i = 0; i < count; i++)
	{
		atom = cont->_children[i];
		frame = atom->getFrameID();
		if (frame == '----')
		{
			tmp = _T("---- ");		
			tmp.Append(CMP4_AtomFactory::instance()->getiTuneFrame(atom));
		}
		else
			tmp = atom->getId();

		if (ids.Find(tmp) == -1)
		{
			ids.Add(tmp);
			if (result.GetLength() > 0)
				result.Append(_T(","));
			result.Append(tmp);
		}		
	}
	return result;
}

CAtlString CMP4::GetTrack()
{
	CMP4Atom* atom = mainContainer->find(TRKN_PFAD);
	if (atom == NULL || atom->_blob.GetLength() < 20)
		return L"";
	CAtlString tmp;
	// 2 bytes reserved, track number (16 bit), total (16 bit)
	if (atom->getDataLen() >= 22 && atom->_blob.Get2B(20) > 0)
		tmp.Format(_T("%i/%i"), (int)atom->_blob.Get2B(18), (int)atom->_blob.Get2B(20));
	else
		tmp.Format(_T("%i"), (int)atom->_blob.Get2B(18));
	return tmp;
}

void CMP4::SetTrack(LPCWSTR newTrack)
{
	if (wcslen(newTrack) == 0)
	{
		CAtlString id;
		id.Format(L"%s.trkn", ILST_PFAD);
		mainContainer->removeAtom(id);
		return;
	}
	// format either as one number or as two numbers separated by /
	CAtlString info(newTrack);
	int von = 0, bis = 0;
	int pos = info.Find('/');
	if (pos == -1) // not found
		von = _wtoi(newTrack) ;	
	else
	{
		von = _wtoi(info.Left(pos));
		bis = _wtoi(info.Mid(pos + 1));
	}
	// the numbers have 16 bit
	von = (von < 0) ? 0 : ((von > 65535) ? 65535 : von);
	bis = (bis < 0) ? 0 : ((bis > 65535) ? 65535 : bis);
	CMP4Atom* atom = new CMP4Atom(ILST_TRKN);
	atom->setParent(ILST_PFAD);
	CMP4_AtomFactory::instance()->setTrack(atom, (WORD)von, (WORD)bis);
	mainContainer->replaceAtom(atom);
}

CAtlString CMP4::GetGenre()
{
	// genre either as ©gen (text) or as gnre (number)
	// look for a text frame first
	CMP4Atom* atom = mainContainer->find(TEXT_GENRE_PFAD);
	if (atom != NULL)
		return CMP4_AtomFactory::instance()->getText(atom);
	// not present as text, maybe as a number??
	atom = mainContainer->find(ZAHL_GENRE_PFAD);
	if (atom == NULL || atom->getDataLen() < 18)
		return L"";
	// found, get the number and build the text from it
	BYTE genreID = atom->_blob.GetAt(17);
	if (genreID > 0 && genreID <= MAX_MUSIC_GENRES)
		return MUSIC_GENRE[genreID - 1];
	// number outside the range, just output the number
	CAtlString ausgabe;
	ausgabe.Format(_T("%i"), genreID);
	return ausgabe;
}

void CMP4::SetGenre(LPCWSTR newgenre)
{
	// first delete all genres
	mainContainer->removeAtom(TEXT_GENRE_PFAD);
	mainContainer->removeAtom(ZAHL_GENRE_PFAD);
	if (newgenre == NULL || wcslen(newgenre) == 0)
		return;

	CAtlString info(newgenre);
	// first find out whether the genre is defined in the standards
	for (BYTE i = 0; i < MAX_MUSIC_GENRES; i++)
	{
		if (info.CompareNoCase(MUSIC_GENRE[i]) == 0)
		{
			CMP4Atom* atom = new CMP4Atom(ILST_ZAHL_GENRE);
			atom->setParent(ILST_PFAD);
			CMP4_AtomFactory::instance()->buildData(atom, 2, 0);
			atom->_blob.AddNullByte();
			atom->_blob.AddValue(i + 1);
			mainContainer->replaceAtom(atom); // set new genre
			return;
		}
	}
	// no standard genre found, set custom genre
	CMP4Atom* atom = new CMP4Atom(ILST_TEXT_GENRE);
	atom->setParent(ILST_PFAD);
	CMP4_AtomFactory::instance()->setText(atom, info);
	mainContainer->replaceAtom(atom); // set new genre
}

// the first sample description of a sound track
static CMP4_STSD* FirstSoundSampleEntry(CMP4_MainContainer *container)
{
	int start = 1;
	CMP4_STSD* stsd;
	while ( (stsd = cSTSD(container->find(STSD_PFAD, start))) != NULL)
	{
		start++;
		if (stsd->mdhd != NULL && stsd->mdhd->isSoundAtom && stsd->channels > 0)
			return stsd;
	}
	return NULL;
}

long CMP4::GetChannels()
{
	// number of the channels in the sample description of the sound track; 2 without a description
	CMP4_STSD* stsd = FirstSoundSampleEntry(mainContainer);
	return (stsd != NULL) ? stsd->channels : 2;
}

long CMP4::GetSampleRate()
{
	// the sample rate is in the sample description; the time scale of the media header is the fall back (it is often the sample rate too)
	CMP4_STSD* stsd = FirstSoundSampleEntry(mainContainer);
	if (stsd != NULL && stsd->sampleRate > 0)
		return stsd->sampleRate;
	int start = 1;
	CMP4_MDHD* atom;
	long tmpSamplerate = 0;
	int count = 0;
	while ( (atom = cMDHD(mainContainer->find(MDHD_PFAD, start))) != NULL)
	{
		start++;
		if (atom->isSoundAtom)
		{
			tmpSamplerate+=atom->timeScale;
			count++;
		}
	}
	return (count > 0 ) ? tmpSamplerate / count : 0;
}

float CMP4::GetDuration()
{
	// the file is as long as its longest sound track
	int start = 1;
	double duration = 0.0;
	CMP4_MDHD* atom;
	while ( (atom = cMDHD(mainContainer->find(MDHD_PFAD, start))) != NULL)
	{
		if (atom->isSoundAtom && atom->duration > 0 && atom->timeScale > 0)
		{
			const double trackDuration = (double)atom->duration / (double)atom->timeScale;
			if (trackDuration > duration)
				duration = trackDuration;
		}
		start++;
	}
	return (float)duration;
}

void CMP4::RemoveTag()
{
	mainContainer->removeAtom(_T("moov.udta"));	
}

// the size of an ID3v2 tag at the start of the file (0 if there is none): the atoms begin behind it
static __int64 id3v2SizeOf(CFile *Stream)
{
	BYTE h[10];
	if (CTools::readAt(Stream, 0, h, 10) != 10 || h[0] != 'I' || h[1] != 'D' || h[2] != '3' || h[3] == 0xFF || h[4] == 0xFF || ((h[6] | h[7] | h[8] | h[9]) & 0x80) != 0)
		return 0;
	__int64 size = 10 + ((__int64)h[6] << 21) + ((__int64)h[7] << 14) + ((__int64)h[8] << 7) + h[9];
	if (h[3] == 4 && (h[5] & 0x10) != 0)
		size += 10;   // footer (v2.4)
	return size;
}

bool CMP4::SaveToFile(LPCWSTR FileName)
{
	// determine the start of the data area
	CFile *Source;
	CFile *Destination;
	CAtlString NewFileName(FileName);
	if ( (Source = CFile::openFile(FileName, CFile::Mode::ReadWrite, CFile::Share::All)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	// the tags in front of and behind the atoms of this file (FileName is not always the analyzed file: its sizes and those of the analysis are
	// not used, CTools::FileSize stays the size of the analyzed file)
	const __int64 fileSize = Source->size();
	const __int64 atomsStart = id3v2SizeOf(Source);
	const __int64 atomsEnd = fileSize - CID3V1::DetectSize(Source);
	if (fileSize < 0 || atomsEnd <= atomsStart)
	{
		CTools::instance().setLastError(ERR_INVALID_FORMAT);
		CFile::closeFile(Source);
		return false;
	}
	// save old taggings
	CMP4Atom *oldTaggings = NULL, *atom = NULL;
	atom = mainContainer->find(ILST_PFAD);
	if (atom != NULL)
	{
		oldTaggings = atom->copy();
	}
	Source->seek(atomsStart);
	CMP4_MainContainer *newData = new CMP4_MainContainer();
	newData->load(Source, (u64)atomsStart, (u64)(atomsEnd - atomsStart));
	// all mdat atoms (a file can have more than one): the positions and sizes of the old file, the source of their data
	std::vector<CMP4_MDAT*> mdats;
	for (int index = 1; ; index++)
	{
		CMP4Atom *found = newData->find(MDAT_PFAD, index);
		if (found == NULL)
			break;
		mdats.push_back(cMDAT(found));
	}
	// a fragmented file has offsets in its fragments (moof, mfra, sidx) that are not adjusted when the data move: it is not written
	// (before, every save destroyed the audio data of such a file)
	const bool fragmented = newData->isFragmented();
	if (mdats.empty() || fragmented)
	{
		CTools::instance().setLastError(fragmented ? ERR_MP4WRITE_NOT_SUPPORTED : ERR_INVALID_FORMAT);
		delete newData;
		delete oldTaggings;
		CFile::closeFile(Source);
		return false;
	}
	std::vector<CMP4_Move> moves;
	for (CMP4_MDAT *m : mdats)
	{
		m->setSourceFile(FileName);
		moves.push_back({ m->getPosition(), m->getPosition() + (__int64)m->getSize(), 0 });
	}
	u64 sizeBefore = newData->getSize();
	// delete wrong paddings
	atom = newData->find(_T("moov.udta.meta.free"));
	if (atom != NULL)
		newData->removeAtom(atom);
	newData->adjustPadding(-1);
	if (oldTaggings != NULL)
	{
		newData->checkMetaBox();
		newData->replaceAtom(oldTaggings);
	}
	else
	{
		// no tagging data wanted
		newData->removeAtom(_T("moov.udta"));
	}
	u64 sizeAfter = newData->getSize();
	long paddingBlockSize = CTools::configValues[CONFIG_MP4PADDINGSIZE];
	u64 optimalSize = sizeAfter;
	if (paddingBlockSize > 0)
		optimalSize = ((sizeAfter / (u64)paddingBlockSize) + 1) * (u64)paddingBlockSize;

	// a padding box has 8 bytes of header: a gap of 1..7 bytes cannot be filled in place
	bool rebuild = (sizeAfter > sizeBefore || (sizeBefore - sizeAfter > 0 && sizeBefore - sizeAfter < 8) || optimalSize < sizeBefore || paddingBlockSize == 0);
	if (!rebuild)
	{
		// the padding fills the space that the smaller tag has left (in front of the mdat atom). If the atoms in front of an mdat atom change their
		// size (the metadata are behind the mdat atom, or between two of them), the mdat atom would move inside of the file: the data cannot be
		// copied inside of the same file (the new padding overwrites the start of the old data) and the chunk offsets would be wrong, so the file is rebuilt
		newData->adjustPadding((sizeBefore > sizeAfter) ? (__int64)(sizeBefore - sizeAfter) - 8 : -1);
		if (!newData->mdatsKeepPositions(atomsStart))
		{
			newData->adjustPadding(-1);
			rebuild = true;
		}
	}
	if (rebuild)
	{
		CTools::instance().writeDebug(_T("Rebuild mp4 tag"));
		for (CMP4_MDAT *m : mdats)
			m->setSameFile(false);
		// rebuild File
		/* Create file streams */
		if ( (Destination = CTools::createTemporary(FileName, CFile::Mode::Write, NewFileName)) == NULL)
		{
			delete newData;
			CTools::instance().setLastError(errno);
			CFile::closeFile(Source);
			return false;
		};
		/* adjust padding  */
		newData->adjustPadding((paddingBlockSize > 0) ? (__int64)paddingBlockSize - 8 : -1);
		// the tags in front of and behind the atoms (ID3v2, ID3v1) are kept (before, they were lost when the file was rebuilt)
		bool tagsCopied = Source->seek(0) && CTools::copyStream(Source, Destination, atomsStart);
		/* Copy atom blocks */
		newData->save(Destination);
		tagsCopied = tagsCopied && Source->seek(atomsEnd) && CTools::copyStream(Source, Destination, fileSize - atomsEnd);
		if (!tagsCopied)
			Destination->setFailed(EIO);   // the new file must not replace the original
		// the chunk offsets of all tracks (32 bit tables stco and 64 bit tables co64) move with the mdat atom they point into
		bool offsetsOk = true;
		for (size_t i = 0; i < mdats.size(); i++)
			moves[i].delta = mdats[i]->getPosition() - moves[i].start;
		for (int track = 1; offsetsOk; track++)
		{
			CMP4_STCO* table = cSTCO(newData->find(STCO_PFAD, track));
			if (table == NULL)
				break;
			offsetsOk = table->move(moves, Destination);
		}
		for (int track = 1; offsetsOk; track++)
		{
			CMP4_STCO* table = cSTCO(newData->find(CO64_PFAD, track));
			if (table == NULL)
				break;
			offsetsOk = table->move(moves, Destination);
		}
		delete newData;
		if (!offsetsOk)
		{
			// an offset does not fit into a 32 bit table: the file is not changed
			CFile::closeFile(Destination);
			CFile::closeFile(Source);
			CFile::removeFile(NewFileName);
			CTools::instance().setLastError(ERR_FRAME_TOO_BIG);
			return false;
		}
		return CTools::finishRewrite(Source, Destination, NewFileName, FileName);
	}
	CTools::instance().writeDebug(_T("Rewrite mp4 tag"));
	// the mdat atoms stay where they are: the audio data are not copied
	for (CMP4_MDAT *m : mdats)
		m->setSameFile(true);
	Source->seek(atomsStart);
	newData->save(Source);
	// the atoms are written over the old ones: a write error is reported, and the bytes go to the disk
	const bool ok = Source->sync();
	const int error = errno;
	CFile::closeFile(Source);
	newData->remove();
	delete newData;
	if (!ok)
	{
		CTools::instance().setLastError(error != 0 ? error : EIO);
		return false;
	}
	return true;
}
