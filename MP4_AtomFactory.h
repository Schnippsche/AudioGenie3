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
#include "mp4atom.h"
#include "atlcoll.h"
#include "mp4_mdhd.h"

#define cFTYP static_cast<CMP4_FTYP*>
#define cMDAT static_cast<CMP4_MDAT*>
#define cMDHD static_cast<CMP4_MDHD*>
#define cSOUN static_cast<CMP4_SOUN*>
#define cSTCO static_cast<CMP4_STCO*>
#define cSTSD static_cast<CMP4_STSD*>

enum MP4_FRAMETYPES {
	MP4_NONE = 0,
	MP4_FTYP = 'ftyp',
	MP4_HDLR = 'hdlr',
	MP4_ILST = 'ilst',
	MP4_MDAT = 'mdat',
	MP4_MDHD = 'mdhd',
	MP4_MDIA = 'mdia',
	MP4_META = 'meta',
	MP4_MINF = 'minf',
	MP4_MOOV = 'moov',
	MP4_NMHD = 'nmhd',
	MP4_SMHD = 'smhd',
	MP4_SOUN = 'soun',
	MP4_STBL = 'stbl',
	MP4_STCO = 'stco',
	MP4_CO64 = 'co64',
	MP4_STSD = 'stsd',
	MP4_TRAK = 'trak',
	MP4_UDTA = 'udta',
	MP4_VMHD = 'vmhd',
	MP4_COVR = 'covr',
	MP4_ITUN = '----'
};

class CMP4_AtomFactory
{
public:
	static CMP4_AtomFactory *instance();
	int ebene;
	static int count;
	static __int64 firstAudioPos, lastAudioPos, lastOffset;
	static __int64 mediaLength;
	static CMP4_MDHD* lastMDHDAtom;
	CMP4Atom *createAtom(const u32 atomID) const;
	CAtlString getText(CMP4Atom *atom);
	CAtlString getiTuneText(CMP4Atom *atom);
	CAtlString getiTuneFrame(CMP4Atom *atom);
	void setText(CMP4Atom *atom, CAtlString newText);
	void setItuneText(CMP4Atom *atom, CAtlString frame, CAtlString newText, CAtlString mean = _T(""));
	CAtlString getiTuneMean(CMP4Atom *atom);
	void setTrack(CMP4Atom *atom, WORD von, WORD bis);
	void buildData(CMP4Atom *atom, int length, BYTE flag);	
protected:
	/*!
	* Constructs a factory.  Because this is a singleton this method is
	* protected, but may be used for subclasses.
	*/
	CMP4_AtomFactory();
	virtual ~CMP4_AtomFactory();	
private:
	CMP4_AtomFactory(const CMP4_AtomFactory &);
	CMP4_AtomFactory &operator=(const CMP4_AtomFactory &);
	static CMP4_AtomFactory *factory;
	
};