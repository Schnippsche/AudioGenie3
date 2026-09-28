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
#include "mp4_atomfactory.h"
#include "mp4_ftyp.h"
#include "mp4_container.h"
#include "mp4_mdhd.h"
#include "mp4_soun.h"
#include "mp4_mdat.h"
#include "mp4_hdlr.h"
#include "Tools.h"
#include "id3v1taginfo.h"
#include "MP4_STCO.h"

CMP4_AtomFactory *CMP4_AtomFactory::factory = 0;
int CMP4_AtomFactory::count = 0;
__int64 CMP4_AtomFactory::firstAudioPos = 0;
__int64 CMP4_AtomFactory::lastAudioPos = 0;
__int64 CMP4_AtomFactory::mediaLength = 0;
__int64 CMP4_AtomFactory::lastOffset = 0;
CMP4_MDHD* CMP4_AtomFactory::lastMDHDAtom = NULL;

CMP4_AtomFactory::CMP4_AtomFactory(void)
{
}

CMP4_AtomFactory::~CMP4_AtomFactory(void)
{
}

CMP4_AtomFactory *CMP4_AtomFactory::instance()
{
	if(!factory)
	{
		factory = new CMP4_AtomFactory;		
		factory->ebene = 0;	
	}
	return factory;
}

CMP4Atom* CMP4_AtomFactory::createAtom(u32 atomID) const
{
	switch (atomID)
	{
	case MP4_MOOV: return new CMP4_Container(MP4_MOOV);
	case MP4_UDTA: return new CMP4_Container(MP4_UDTA);
	case MP4_META: return new CMP4_Container(MP4_META);
	case MP4_FTYP: return new CMP4_FTYP();
	case MP4_ILST: return new CMP4_Container(MP4_ILST);
	case MP4_MDAT: return new CMP4_MDAT();
	case MP4_MDHD: 
		{
			CMP4_MDHD* mdhd = new CMP4_MDHD();
			factory->lastMDHDAtom = mdhd;
			return mdhd; 
		}
	case MP4_MDIA: return new CMP4_Container(MP4_MDIA);	
	case MP4_MINF: return new CMP4_Container(MP4_MINF);
	case MP4_COVR: return new CMP4_Container(MP4_COVR);
	case MP4_HDLR: return new CMP4_HDLR();
	case MP4_SOUN: return new CMP4_SOUN();
	case MP4_STBL: return new CMP4_Container(MP4_STBL);
	case MP4_STCO: return new CMP4_STCO();
	case MP4_CO64: return new CMP4_STCO(MP4_CO64);
	case MP4_STSD:
		{
			CMP4_STSD* stsd = new CMP4_STSD();
			stsd->mdhd = factory->lastMDHDAtom;   // the media header of the same track is in front of the sample description
			return stsd;
		}
	case MP4_TRAK: return new CMP4_Container(MP4_TRAK);
	case MP4_VMHD: return new CMP4_Container(MP4_VMHD);
	case MP4_SMHD:
		{
			if (factory->lastMDHDAtom != NULL)
				factory->lastMDHDAtom->isSoundAtom = true;
			return new CMP4Atom(MP4_SMHD);
		}
	//case MP4_ITUN: return new CMP4_Tunes(); 
	default:
		return new CMP4Atom(atomID);
	};
}

CAtlString CMP4_AtomFactory::getText(CMP4Atom* atom)
{
	if (atom->_blob.GetLength() <= 16)
		return EMPTY;
	// 4 bytes length
	// 4 bytes 'data' 
	// 4 bytes version/flags = byte hex version + 24-bit hex flags
	// (current version = 0 ; contains text flag = 0x000001)
	// 4 bytes reserved = 32-bit value set to zero
	size_t ln = atom->_blob.GetLength() - 16;
	// long data = atom->_blob.Get4B(4);
	long version = atom->getVersion();
	// Version: 1=text, 21=uint8 0=uint8
	if (version == 1)
	{
		CBlob tmp;
		tmp.AddMemory(atom->_blob.m_pData + 16, ln);
		return tmp.ConvertToUnicodeString(TEXT_ENCODED_UTF8);
	}

	if (version == 0 || version == 21) // Binary Data
	{
		CAtlString result;
		if (ln == 1)
			result.Format(_T("%i"), (BYTE)atom->_blob.m_pData[16]);
		else if (ln == 2 && atom->getFrameID() == 'gnre')
		{
			BYTE genreID = (BYTE)atom->_blob.m_pData[17];
			if (genreID > 0 && genreID <= MAX_MUSIC_GENRES)
				result = MUSIC_GENRE[genreID - 1];
		}
		else if (ln == 2)
			result.Format(_T("%i"), (int)atom->_blob.Get2B(16));   // 16 bit number (tempo)
		else if (ln == 4)		
			result.Format(_T("%i"), atom->_blob.Get4B(16)); // (BYTE)atom->_blob.m_pData[19]);
		else if (ln ==6 || ln == 8)
		{
			// track or disk: 2 bytes reserved, number (16 bit), total (16 bit), 2 bytes reserved (only trkn)
			if (atom->_blob.Get2B(20) >  0)
				result.Format(_T("%i/%i"), (int)atom->_blob.Get2B(18), (int)atom->_blob.Get2B(20));
			else
				result.Format(_T("%i"), (int)atom->_blob.Get2B(18));
		}
		else  // unknown length, what now?
		{
			CBlob tmp;
			tmp.AddMemory(atom->_blob.m_pData + 16, ln);
			result = tmp.ConvertToUnicodeString(TEXT_ENCODED_UTF8);
		}		
		return result;
	}

	// oops, unknown format
	CTools::instance().setLastError(ERR_ATOM_IS_NO_TEXTCLASS);
	return EMPTY;
}

void CMP4_AtomFactory::setText(CMP4Atom *atom, CAtlString newText)
{
	u32 id = atom->getFrameID();
	CBlob tmp;
	BYTE type = 1;
	if (id == 'trkn' || id == 'gnre')
	{
		CTools::instance().writeError(_T("don't use MP4SetTextFrameW with trkn frame or gnre frame. Use MP4SetTrackW or MP4SetGenreW instead!"));
		return;
	}
	if (id == 'covr')
	{
		CTools::instance().writeError(_T("don't use MP4SetTextFrameW with 'covr' frame. Use MP4AddPictureArrayW or MP4AddPictureFileW instead!"));
		return;
	}
	if (id == 'cpil' || id == 'pgap' || id == 'pcst') // 1 Databyte
	{
		type = 21;	
		tmp.AddValue((BYTE)_wtoi(newText));
	}
	else if (id == 'tmpo') // 2 Databyte (16 bit number)
	{
		type = 21;
		int tempo = _wtoi(newText);
		if (tempo < 0) tempo = 0;
		if (tempo > 65535) tempo = 65535;
		tmp.AddValue((BYTE)(tempo >> 8));
		tmp.AddValue((BYTE)(tempo & 0xFF));
	}
	else if (id == 'disk') // 6 bytes: 2 reserved, disk number (16 bit), total (16 bit)
	{
		type = 0;	
		int pos = newText.Find('/');
		int disk = 0, total = 0;
		if (pos == -1) // not found
			disk = _wtoi(newText);
		else
		{
			disk = _wtoi(newText.Left(pos));
			total = _wtoi(newText.Mid(pos + 1));
		}
		disk = (disk < 0) ? 0 : ((disk > 65535) ? 65535 : disk);
		total = (total < 0) ? 0 : ((total > 65535) ? 65535 : total);
		tmp.AddValue(0, 2);
		tmp.AddValue((BYTE)(disk >> 8));
		tmp.AddValue((BYTE)(disk & 0xFF));
		tmp.AddValue((BYTE)(total >> 8));
		tmp.AddValue((BYTE)(total & 0xFF));
	}
	else if (id == 'purl' || id == 'egid')
	{
		type = 0;
		tmp.AddEncodedString(TEXT_ENCODED_UTF8, newText, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	}
	else
	{
		type = 1;
		tmp.AddEncodedString(TEXT_ENCODED_UTF8, newText, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	}
		
	buildData(atom,(int)tmp.GetLength(), type);
	atom->_blob.AddMemory(tmp.m_pData, tmp.GetLength());
}

// trkn: 8 bytes: 2 reserved, track number (16 bit), total (16 bit), 2 reserved
void CMP4_AtomFactory::setTrack(CMP4Atom *atom, WORD von, WORD bis)
{
	buildData(atom, 8, 0);
	atom->_blob.AddValue(0, 2);
	atom->_blob.AddValue((BYTE)(von >> 8));
	atom->_blob.AddValue((BYTE)(von & 0xFF));
	atom->_blob.AddValue((BYTE)(bis >> 8));
	atom->_blob.AddValue((BYTE)(bis & 0xFF));
	atom->_blob.AddValue(0, 2);
}

void CMP4_AtomFactory::buildData(CMP4Atom *atom, int length, BYTE flag)
{
	atom->_blob.Clear();
	atom->_blob.Add4B(16 + length);
	atom->_blob.Add4B('data');
	atom->_blob.Add4B(flag);
	atom->_blob.Add4B(0);
}
// A free form item ("----") consists of the boxes "mean" (the owner of the name, for example com.apple.iTunes), "name" and "data"
// (ISO/IEC 14496-12 boxes with their own sizes; mean and name have version and flags, data has the type and a locale).
static bool FindSubAtom(CMP4Atom *atom, u32 wanted, size_t &start, size_t &length)
{
	size_t pos = 0;
	const size_t total = atom->_blob.GetLength();
	while (pos + 8 <= total)
	{
		const u32 size = atom->_blob.Get4B(pos);
		const u32 type = atom->_blob.Get4B(pos + 4);
		if (size < 8 || pos + size > total)
			return false;
		if (type == wanted)
		{
			start = pos;
			length = size;
			return true;
		}
		pos += size;
	}
	return false;
}

static CAtlString SubAtomText(CMP4Atom *atom, u32 wanted, size_t skip)
{
	size_t start = 0, length = 0;
	if (!FindSubAtom(atom, wanted, start, length) || length <= skip)
		return EMPTY;
	CBlob tmp;
	tmp.AddMemory(atom->_blob.m_pData + start + skip, length - skip);
	return tmp.ConvertToUnicodeString(TEXT_ENCODED_UTF8);
}

void CMP4_AtomFactory::setItuneText(CMP4Atom *atom, CAtlString frame, CAtlString newText, CAtlString mean)
{
	CBlob tmp;
	if (mean.IsEmpty())
		mean = _T("com.apple.iTunes");
	atom->_blob.Clear();
	// Atom "mean"
	tmp.Clear();
	tmp.AddEncodedString(TEXT_ENCODED_UTF8, mean, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	atom->_blob.Add4B((u32)tmp.GetLength() + 12);
	atom->_blob.Add4B('mean');
	atom->_blob.Add4B(0); // version and flags
	atom->_blob.AddBlob(tmp);
	// Atom "name"
	tmp.Clear();
	tmp.AddEncodedString(TEXT_ENCODED_UTF8, frame, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	atom->_blob.Add4B((u32)tmp.GetLength() + 12);
	atom->_blob.Add4B('name');
	atom->_blob.Add4B(0); // version and flags
	atom->_blob.AddBlob(tmp);
	// Atom "data"
	tmp.Clear();
	tmp.AddEncodedString(TEXT_ENCODED_UTF8, newText, TEXT_WITHOUT_ENCODING, TEXT_WITHOUT_NULLBYTES);
	atom->_blob.Add4B((u32)tmp.GetLength() + 16);
	atom->_blob.Add4B('data');
	atom->_blob.Add4B(1); // type: text
	atom->_blob.Add4B(0); // locale
	atom->_blob.AddBlob(tmp);
}
CAtlString CMP4_AtomFactory::getiTuneText(CMP4Atom *atom)
{
	return SubAtomText(atom, 'data', 16);
}
CAtlString CMP4_AtomFactory::getiTuneFrame(CMP4Atom *atom)
{
	return SubAtomText(atom, 'name', 12);
}
CAtlString CMP4_AtomFactory::getiTuneMean(CMP4Atom *atom)
{
	return SubAtomText(atom, 'mean', 12);
}
