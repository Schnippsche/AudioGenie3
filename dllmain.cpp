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


#include "stdafx.h"
#include "resource.h"
#include "AudioGenie3_i.h"
#include "dllmain.h"
#include "dlldatax.h"
#include "io.h"
#include <winver.h>
#pragma comment(lib, "version.lib")


CAudioGenie3Module _AtlModule;

// DLL-Einstiegspunkt
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}

// ******************************************************************************

BYTE GetFileFormat(FILE *Stream)
{
	CTools::APEHeadSize = 0;
	BYTE result = header.ReadFromFile(Stream);
	if (result == AUDIO_FORMAT_ID3V2)
	{
		CID3V2 v2;
		if (v2.ReadHeader(Stream))
		{
			CTools::ID3v2Size = v2.Size;
			_fseeki64(Stream, v2.Size, SEEK_SET);
			result = header.ReadFromFile(Stream);
			if (result == AUDIO_FORMAT_INVALID)
				result = AUDIO_FORMAT_UNKNOWN;   // bugfix for mpeg headers that do not directly follow the id3v2 tag
		}
		v2.ResetData();
	}
	// an APE tag at the beginning of the file (at the start or behind an ID3v2 tag): the audio data follow it
	__int64 apeOffset, apeLength;
	if (CAPE::FindHeadTag(Stream, apeOffset, apeLength))
	{
		CTools::APEHeadSize = (int)apeLength;
		_fseeki64(Stream, apeOffset + apeLength, SEEK_SET);
		result = header.ReadFromFile(Stream);
		if (result == AUDIO_FORMAT_INVALID)
			result = AUDIO_FORMAT_UNKNOWN;
	}
	return result;
}

BYTE GetFormat(LPCWSTR FileName)
{
	FILE *Source;
	BYTE result = AUDIO_FORMAT_UNKNOWN;
	FileName = getValidPointer(FileName);
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) != NULL)
	{
		result = GetFileFormat(Source);
		if (result == AUDIO_FORMAT_UNKNOWN || result == AUDIO_FORMAT_WAV) // either AAC or MPEG or MPEG with a WAV header in front 
		{
			endung = FileName;
			endung = endung.Trim();			
			if (endung.GetLength() > 3)
				endung = endung.Right(4);
			// now the uncertain candidates, check the file extension
			if (endung.CompareNoCase(_T(".AAC")) == 0)
			{
				CAAC tmpAac;
				if (tmpAac.ReadFromFile(Source))
					result = AUDIO_FORMAT_AAC;
			}
			// now MP3
			else if ( (endung.CompareNoCase(_T(".MP3")) == 0  || 
				endung.CompareNoCase(_T(".MP2")) == 0  ||
				endung.CompareNoCase(_T(".MP1")) == 0  ||
				endung.CompareNoCase(_T("MP3~")) == 0))
			{
				CMPEGAudio tmpMpeg;
				if (tmpMpeg.ReadFromFile(Source))
					result = AUDIO_FORMAT_MPEG;
				tmpMpeg.ResetData();
			}
		}
		fclose(Source);
	}
	return result;
}

void ClearAllTags()
{
	endung.Empty();
	Title.Empty();
	Artist.Empty();
	Album.Empty();
	Comment.Empty();
	Genre.Empty();
	Track.Empty();
	Year.Empty();
	Composer.Empty();
	aac.ResetData();
	ape.ResetData();
	mpp.ResetData();
	flac.ResetData();
	wav.ResetData();
	ogg.ResetData();
	wma.ResetData();
	mp4.ResetData();
	mpeg.ResetData();
	id3v1.ResetData();
	id3v2.ResetData();
	monkey.ResetData();  
	lyrics.ResetData();
	tta.ResetData();
	wavpack.ResetData();
	CTools::instance().reset();
	Format = 0;
	audio = &defaultaudio;
	Format = AUDIO_FORMAT_UNKNOWN;
	CTools::instance().setLastError(0);
}
/**
 * @brief read an audio-file and all known tags
 *
 * The format is detected from the file content (signatures). Only for AAC (extension .aac) and MPEG audio (.mp1, .mp2, .mp3, .msf, .mp3~)
 * the extension is checked first as a pre-filter, so an MP3 file with another extension is not recognized.
 *
 * the tags are read in this way:
 * files with format WMA, MP4, FLAC and OGG use their own fields. All other formats follow these rules:
 * first the ID3v2 tag, if it does not exist the APE tag, then the ID3v1 tag and finally the Lyrics tag.
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return return value from 0 to 11 with the following meaning:
 * @retval 0 unknown Format
 * @retval 1 MP3
 * @retval 2 WMA
 * @retval 3 MONKEY
 * @retval 4 FLAC
 * @retval 5 WAV
 * @retval 6 OGG VORBIS
 * @retval 7 MPP
 * @retval 8 AAC
 * @retval 9 MP4/M4A
 * @retval 10 TTA
 * @retval 11 wavpack
 */
extern "C" long __stdcall AUDIOAnalyzeFileW(LPCWSTR FileName)
{
	ClearAllTags();
	FILE *Source;
	// copy, because FileName may point to lastFile (internal calls AUDIOAnalyzeFileW(lastFile))
	CAtlString requestedFile(getValidPointer(FileName));
	// "last analyzed file" only counts again once this analysis was able to open the file. Otherwise a following
	// AUDIOSaveChangesW would write the fields emptied by ClearAllTags into the previous file and delete its tags.
	lastFile.Empty();
	FileName = requestedFile;
	errno = 0;
	ATLTRACE(_T("Analyzing %s\n"), FileName);
	if ( (Source = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) != NULL)
	{
		CTools::FileSize = _filelengthi64(_fileno(Source));
		CTools::instance().writeInfo(L"parse file '%s' [%I64d bytes]...", FileName, CTools::FileSize);
		CTools::instance().doEvents();
		lastFile = FileName;
		if (CTools::FileSize < 32)
		{
			CTools::instance().writeInfo(_T("no valid audio format detected"));
			goto ende;
		}
		BYTE possibleFormat = GetFileFormat(Source);
		// distinguish cases by format
		if (possibleFormat == AUDIO_FORMAT_INVALID) // clearly invalid format
		{
			fclose(Source);
			CTools::instance().writeInfo(_T("no valid audio format detected"));
			return AUDIO_FORMAT_UNKNOWN;
		}
		// take a closer look
		
		// get the file extension, last 4 bytes
		endung = FileName;
		if (endung.GetLength() > 3)
			endung = endung.Right(4);
		// parse ID3V2 tag if present
		if (CTools::ID3v2Size > 0)
			id3v2.ReadFromFile(Source);
		// load tags at the end of the file
		if (CTools::FileSize > 132)
		{
			id3v1.ReadFromFile(Source);
			lyrics.ReadFromFile(Source);
		}
		ape.ReadFromFile(Source);
		// first the formats without tags
		if (possibleFormat == AUDIO_FORMAT_WMA && wma.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_WMA;
			CTools::instance().writeInfo(_T("identified as wma file"));
			audio = &wma;
			Album = wma.GetUserItem(WM_ALBUMTITLE);
			Artist = wma.GetUserItem(WM_AUTHOR);
			Comment = wma.GetUserItem(WM_DESCRIPTION);
			Genre = wma.GetUserItem(WM_GENRE);
			Title = wma.GetUserItem(WM_TITLE);
			Track = wma.GetUserItem(WM_TRACKNUMBER);
			Year = wma.GetUserItem(WM_YEAR);
			if (Year.IsEmpty())
				Year = wma.GetUserItem(_T("date"));   // e.g. written by ffmpeg
			Composer = wma.GetUserItem(WM_COMPOSER);
			goto ende;
		}
		if (possibleFormat == AUDIO_FORMAT_MP4 && mp4.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_MP4;
			CTools::instance().writeInfo(_T("identified as mp4/m4a file"));
			audio = &mp4;
			Album = mp4.GetILSTText(MP4_ALBUM);
			Artist = mp4.GetILSTText(MP4_ARTIST);
			Comment = mp4.GetILSTText(MP4_COMMENT);
			Genre = mp4.GetGenre();
			Title = mp4.GetILSTText(MP4_TITLE);
			Track = mp4.GetTrack();
			Year = mp4.GetILSTText(ILST_DAY);
			Composer = mp4.GetILSTText(MP4_COMPOSER);
			goto ende;
		}
		// do not test for WAV with mp* files, because some MP3s also have WAV headers
		if (  possibleFormat == AUDIO_FORMAT_WAV && endung.CompareNoCase(_T(".MP3")) != 0  && 
			endung.CompareNoCase(_T(".MP2")) != 0  &&
			endung.CompareNoCase(_T(".MP1")) != 0  &&
			endung.CompareNoCase(_T("MP3~")) != 0  && 
			wav.ReadFromFile(Source) )
		{
			Format = AUDIO_FORMAT_WAV;
			CTools::instance().writeInfo(_T("identified as wav file"));
			audio = &wav;
			Album = wav.getTextFrame(WAV_IPRD);
			Artist = wav.getTextFrame(WAV_IART);
			Title = wav.getTextFrame(WAV_INAM);
			Comment = wav.getTextFrame(WAV_ICMT);
			Year = wav.getTextFrame(WAV_ICRD);
			Genre = wav.getTextFrame(WAV_IGNR);
			Track = wav.getTextFrame(WAV_ITRK);	
			if (Track.IsEmpty())
				Track = wav.getTextFrame(0x49505254);   // "IPRT" (part), used by ffmpeg for the track number
			Composer = wav.getTextFrame(WAV_IMUS);
			goto ende;
		}
		// first the clearly identifiable formats
		if (possibleFormat == AUDIO_FORMAT_FLAC && flac.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_FLAC;
			CTools::instance().writeInfo(_T("identified as flac file"));
			audio = &flac;
			Album = flac.GetUserItem(VORBIS_ALBUM);
			Artist = flac.GetUserItem(VORBIS_ARTIST);
			Comment = flac.GetUserItem(VORBIS_COMMENT);
			if (Comment.IsEmpty())
				Comment = flac.GetUserItem(VORBIS_DESCRIPTION);   // Vorbis standard field, e.g. written by ffmpeg
			Genre = flac.GetUserItem(VORBIS_GENRE);
			Title = flac.GetUserItem(VORBIS_TITLE);
			Track = flac.GetUserItem(VORBIS_TRACKNUMBER);
			Year = flac.GetUserItem(VORBIS_DATE);
			Composer = flac.GetUserItem(VORBIS_COMPOSER);
			goto ende;
		}
		if (possibleFormat == AUDIO_FORMAT_OGGVORBIS && ogg.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_OGGVORBIS;
			CTools::instance().writeInfo(_T("identified as ogg vorbis file"));
			audio = &ogg;
			Album = ogg.GetUserItem(VORBIS_ALBUM);
			Artist = ogg.GetUserItem(VORBIS_ARTIST);
			Comment = ogg.GetUserItem(VORBIS_COMMENT);
			if (Comment.IsEmpty())
				Comment = ogg.GetUserItem(VORBIS_DESCRIPTION);   // Vorbis standard field, e.g. written by ffmpeg
			Genre = ogg.GetUserItem(VORBIS_GENRE);
			Title = ogg.GetUserItem(VORBIS_TITLE);
			Track = ogg.GetUserItem(VORBIS_TRACKNUMBER);
			Year = ogg.GetUserItem(VORBIS_DATE); 
			Composer = ogg.GetUserItem(VORBIS_COMPOSER);
			goto ende;
		}

		

		// load tags, an ID3v2 tag is present
		if (CTools::ID3v2Size > 0)
		{			
			Album = id3v2.GetText(F_TALB);
			Artist = id3v2.GetText(F_TPE1);
			CID3_Frame *frame = id3v2.findFrame(F_COMM);
			if (frame != NULL)
				Comment = cCOMM(frame)->getText();
			else
				Comment.Empty();

			Genre = id3v2.GetGenre();
			Title = id3v2.GetText(F_TIT2);
			Track = id3v2.GetText(F_TRCK);
			Year = id3v2.GetText(F_TYER);
			if (Year.IsEmpty())
			{
				// ID3v2.4: the year is in the timestamp TDRC (ISO 8601, e.g. "2024" or "2024-05-01")
				CAtlString timestamp = id3v2.GetText(F_TDRC);
				Year = timestamp.Left(4);
			}
			Composer = id3v2.GetText(F_TCOM);
		}
		else if (ape.Exists())
		{
			Album = ape.GetTagItem(APE_ALBUM);
			Artist = ape.GetTagItem(APE_ARTIST);
			Comment = ape.GetTagItem(APE_COMMENT);
			Genre = ape.GetTagItem(APE_GENRE);
			Title = ape.GetTagItem(APE_TITLE);
			Track = ape.GetTagItem(APE_TRACK);
			Year = ape.GetTagItem(APE_YEAR);
			if (Year.IsEmpty())
				Year = ape.GetTagItem(_T("DATE"));   // e.g. written by ffmpeg
			Composer = ape.GetTagItem(APE_COMPOSER);
		}
		else if (CTools::ID3v1Size > 0)
		{
			Album = id3v1.GetAlbum();
			Artist = id3v1.GetArtist();
			Comment = id3v1.GetComment();
			Genre = id3v1.GetGenre();
			Title = id3v1.GetTitle();
			Track = id3v1.GetTrack(); 
			Year = id3v1.GetYear();         
		}
		else if (CTools::LyricsSize > 0)
		{
			Album = lyrics.GetAlbum();
			Artist = lyrics.GetArtist();
			Genre = lyrics.GetGenre();
			Title = lyrics.GetTitle();      
		}

		if (possibleFormat == AUDIO_FORMAT_MONKEY && monkey.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_MONKEY;
			CTools::instance().writeInfo(_T("identified as monkey file"));
			audio = &monkey;
			goto ende;
		}

		if (possibleFormat == AUDIO_FORMAT_TTA && tta.ReadFromFile(Source)) 
		{
			Format = AUDIO_FORMAT_TTA;
			CTools::instance().writeInfo(_T("identified as tta file"));
			audio = &tta;
			goto ende;
		}
		if (possibleFormat == AUDIO_FORMAT_WAVPACK && wavpack.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_WAVPACK;
			CTools::instance().writeInfo(_T("identified as wavpack file"));
			audio = &wavpack; 
			goto ende;
		}
		if (possibleFormat == AUDIO_FORMAT_MPP && mpp.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_MPP;
			CTools::instance().writeInfo(_T("identified as mpeg plus file"));
			audio = &mpp;
			goto ende;
		}
		// now the uncertain candidates, check the file extension
		if (endung.CompareNoCase(_T(".AAC")) == 0 && aac.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_AAC;
			CTools::instance().writeInfo(_T("identified as aac file"));
			audio = &aac;
			goto ende;
		}

		// now MP3
		if ( (endung.CompareNoCase(_T(".MP3")) == 0  || 
			endung.CompareNoCase(_T(".MP2")) == 0  ||
			endung.CompareNoCase(_T(".MP1")) == 0  ||
			endung.CompareNoCase(_T(".MSF")) == 0  ||
			endung.CompareNoCase(_T("MP3~")) == 0) && 
			mpeg.ReadFromFile(Source))
		{
			Format = AUDIO_FORMAT_MPEG;
			CTools::instance().writeInfo(_T("identified as mpeg file"));
			audio = &mpeg;
			// determine the last audio position from the tags
			__int64 last = CTools::FileSize - 1;
			if (CTools::APESize > 0)
				last = min(last, CTools::FileSize - CTools::APESize - 32 - CTools::ID3v1Size - 1);
			if (lyrics.Exists() )
				last = min(last, lyrics.GetStartPosition() - 1);
			if (id3v1.Exists() )
				last = min(last, CTools::FileSize - CTools::ID3v1Size - 1);
			mpeg.setLastAudioPosition(last);
		}

ende:
		fclose(Source);
		if (Format == 0)
			CTools::instance().writeInfo(_T("unknown or invalid audio format detected"));					
	}
	else
		CTools::instance().setLastError(errno); 
	return Format;
}


/**
 * @brief store the changes in a file, depending on format of the destination file:
 *
 * | audio format | changes saved as |
 * |---|---|
 * | MP3 | ID3v2 tag |
 * | WMA | WMA Fields |
 * | MONKEY | APE tag |
 * | FLAC | Vorbis Comment |
 * | WAV | wav chunk |
 * | OGG | Vorbis Comment |
 * | MPP | ID3v2 tag |
 * | AAC | APE tag (an existing ID3v2 tag is left unchanged and takes precedence when reading) |
 * | MP4 | MP4 atoms |
 * | WavPack | APE tag |
 * | TTA | ID3v2 tag |
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall AUDIOSaveChangesToFileW(LPCWSTR FileName) 
{
	CTools::instance().setLastError(0);
	FileName = getValidPointer(FileName);
	Format = (short)GetFormat(FileName);
	bool result;
	switch (Format)
	{
	case AUDIO_FORMAT_WMA:
		wma.SetUserItem(WM_ALBUMTITLE, Album);
		wma.SetUserItem(WM_AUTHOR, Artist);
		wma.SetUserItem(WM_DESCRIPTION, Comment);
		wma.SetUserItem(WM_GENRE, Genre);
		wma.SetUserItem(WM_TITLE, Title);
		wma.SetUserItem(WM_TRACKNUMBER, Track);
		wma.SetUserItem(WM_YEAR, Year);      
		wma.SetUserItem(WM_COMPOSER, Composer);
		result = wma.SaveToFile(FileName);
		if (result && !lastFile.IsEmpty() && lastFile.CompareNoCase(FileName) == 0)
			AUDIOAnalyzeFileW(FileName);
		return b2s(result);		
	case AUDIO_FORMAT_FLAC:
		flac.SetUserItem(VORBIS_ALBUM, Album);
		flac.SetUserItem(VORBIS_ARTIST, Artist);
		flac.SetUserItem(VORBIS_COMMENT, Comment);
		flac.SetUserItem(VORBIS_GENRE, Genre);
		flac.SetUserItem(VORBIS_TITLE, Title);
		flac.SetUserItem(VORBIS_TRACKNUMBER, Track);
		flac.SetUserItem(VORBIS_DATE, Year); 
		flac.SetUserItem(VORBIS_COMPOSER, Composer); 
		return b2s(flac.SaveToFile(FileName));
	case AUDIO_FORMAT_OGGVORBIS:
		ogg.SetUserItem(VORBIS_ALBUM, Album);
		ogg.SetUserItem(VORBIS_ARTIST, Artist);
		ogg.SetUserItem(VORBIS_COMMENT, Comment);
		ogg.SetUserItem(VORBIS_GENRE, Genre);
		ogg.SetUserItem(VORBIS_TITLE, Title);
		ogg.SetUserItem(VORBIS_TRACKNUMBER, Track);
		ogg.SetUserItem(VORBIS_DATE, Year);  
		ogg.SetUserItem(VORBIS_COMPOSER, Composer);  
		return b2s(ogg.SaveTag(FileName));
	case AUDIO_FORMAT_MP4:
		mp4.SetILSTText(MP4_ALBUM, Album);
		mp4.SetILSTText(MP4_ARTIST, Artist);
		mp4.SetILSTText(MP4_COMMENT, Comment);
		mp4.SetGenre(Genre); 
		mp4.SetILSTText(MP4_TITLE, Title);
		mp4.SetTrack(Track);
		mp4.SetILSTText(ILST_DAY, Year);
		mp4.SetILSTText(MP4_COMPOSER, Composer);
		result = mp4.SaveToFile(FileName);
		if (result && !lastFile.IsEmpty() && lastFile.CompareNoCase(FileName) == 0)
			AUDIOAnalyzeFileW(FileName);
		return b2s(result);	
	case AUDIO_FORMAT_MONKEY:
		ape.SetTagItem(APE_ALBUM, Album);
		ape.SetTagItem(APE_ARTIST, Artist);
		ape.SetTagItem(APE_COMMENT, Comment);
		ape.SetTagItem(APE_GENRE, Genre);
		ape.SetTagItem(APE_TITLE, Title);
		ape.SetTagItem(APE_TRACK, Track);
		ape.SetTagItem(APE_YEAR, Year);
		ape.SetTagItem(APE_COMPOSER, Composer);
		return b2s(ape.SaveToFile(FileName));
	case AUDIO_FORMAT_WAVPACK:
		ape.SetTagItem(APE_ALBUM, Album);
		ape.SetTagItem(APE_ARTIST, Artist);
		ape.SetTagItem(APE_COMMENT, Comment);
		ape.SetTagItem(APE_GENRE, Genre);
		ape.SetTagItem(APE_TITLE, Title);
		ape.SetTagItem(APE_TRACK, Track);
		ape.SetTagItem(APE_YEAR, Year);
		ape.SetTagItem(APE_COMPOSER, Composer);
		return b2s(ape.SaveToFile(FileName));
	case AUDIO_FORMAT_AAC:
		ape.SetTagItem(APE_ALBUM, Album);
		ape.SetTagItem(APE_ARTIST, Artist);
		ape.SetTagItem(APE_COMMENT, Comment);
		ape.SetTagItem(APE_GENRE, Genre);
		ape.SetTagItem(APE_TITLE, Title);
		ape.SetTagItem(APE_TRACK, Track);
		ape.SetTagItem(APE_YEAR, Year);
		ape.SetTagItem(APE_COMPOSER, Composer);
		return b2s(ape.SaveToFile(FileName));
	case AUDIO_FORMAT_UNKNOWN:
		CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
		return b2s(false);
	case AUDIO_FORMAT_WAV:
		wav.setTextFrame(WAV_IPRD, Album);
		wav.setTextFrame(WAV_IART, Artist);
		wav.setTextFrame(WAV_INAM, Title);
		wav.setTextFrame(WAV_ICMT, Comment);
		wav.setTextFrame(WAV_ICRD, Year);
		wav.setTextFrame(WAV_IGNR, Genre);
		wav.setTextFrame(WAV_ITRK, Track);
		wav.setTextFrame(WAV_IMUS, Composer);
		result = wav.SaveToFile(FileName);
		if (result && !lastFile.IsEmpty() && lastFile.CompareNoCase(FileName) == 0)
			AUDIOAnalyzeFileW(FileName);
		return b2s(result);		
	default:
		id3v2.SetText(F_TALB, Album);
		id3v2.SetText(F_TPE1, Artist);
		id3v2.replaceFrame(new CID3F_COMM(_T("eng"), EMPTY, Comment));
		id3v2.SetText(F_TCON, Genre);
		id3v2.SetText(F_TIT2, Title);
		id3v2.SetText(F_TRCK, Track);
		// ID3v2.4 has no TYER, the year is in TDRC there (the other way round in v2.2/v2.3); the other frame is dropped on writing
		if (CTools::ID3V2newTagVersion == TAG_VERSION_2_4)
			id3v2.SetText(F_TDRC, Year);
		else
			id3v2.SetText(F_TYER, Year);
		id3v2.SetText(F_TCOM, Composer);
		return b2s(id3v2.SaveToFile(FileName));		
	}  
}

/**
 * @brief store the changes in the last analyzed file, depending on format of the destination file:
 *
 * | audio format | changes saved as |
 * |---|---|
 * | MP3 | ID3v2 tag |
 * | WMA | WMA Fields |
 * | MONKEY | APE tag |
 * | FLAC | Vorbis Comment |
 * | WAV | wav chunk |
 * | OGG | Vorbis Comment |
 * | MPP | ID3v2 tag |
 * | AAC | APE tag (an existing ID3v2 tag is left unchanged and takes precedence when reading) |
 * | MP4 | MP4 atoms |
 * | WavPack | APE tag |
 * | TTA | ID3v2 tag |
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall AUDIOSaveChangesW()
{
	return AUDIOSaveChangesToFileW(lastFile);
}

/**
 * @brief get the number of channels
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return number of channels
 */
extern "C" long __stdcall AUDIOGetChannelsW()
{
	return audio->GetChannels();
}


/**
 * @brief get the duration in seconds
 *
 * For MPEG audio files with a variable bit rate (VBR) the value is only exact if the file has a Xing or VBRI header
 * or if the configuration value MPEGEXACTREAD was set before the analysis, see SetConfigValueW().
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return duration in seconds
 */
extern "C" float __stdcall AUDIOGetDurationW()
{
	return audio->GetDuration();  
}

/**
 * @brief get the duration in milliseconds
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return duration in milliseconds
 */
extern "C" long __stdcall AUDIOGetDurationMillisW()
{
	return (long)(audio->GetDuration() * 1000.0);
}

/**
 * @brief get the sample rate in Hz
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return sample rate in Hz
 */
extern "C" long __stdcall AUDIOGetSampleRateW()
{
	return audio->GetSampleRate();
}


/**
 * @brief get the bit rate in kbps
 *
 * For MPEG audio files with a variable bit rate (VBR) this is the average bit rate. It is only exact if the file has a
 * Xing or VBRI header or if the configuration value MPEGEXACTREAD was set before the analysis, see SetConfigValueW().
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return bit rate in kbps
 */
extern "C" long __stdcall AUDIOGetBitrateW()
{
	return audio->GetBitRate();
}


/**
 * @brief get the channel mode (mono / stereo)
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return channel mode
 */
extern "C" BSTR __stdcall AUDIOGetChannelModeW()
{
	return audio->GetChannelMode().AllocSysString();
}


/**
 * @brief get the error number of the last error
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return error number
 */
extern "C" long __stdcall AUDIOGetLastErrorNumberW()
{
	return CTools::instance().getLastError();
}


/**
 * @brief get the description of the last error
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return error string
 */
extern "C" BSTR __stdcall AUDIOGetLastErrorTextW()
{
	return CTools::instance().GetLastErrorText().AllocSysString();
}

/**
 * @brief get the last analyzed file name
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return file name of the last analyzed file
 */
extern "C" BSTR __stdcall AUDIOGetLastFileW()
{
	return lastFile.AllocSysString();
}

/**
 * @brief get the format version of the analyzed file
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return version
 */
extern "C" BSTR __stdcall AUDIOGetVersionW()
{
	return audio->GetFileVersion().AllocSysString();
}

/**
 * @brief returns -1 if the file is valid
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return -1 if file is valid, otherwise 0
 */
extern "C" short __stdcall AUDIOIsValidFormatW()
{
	return b2s(audio->IsValid());
}

/**
 * @brief get the filesize in bytes
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return filesize; from 2 GiB on 2147483647 is returned (the value does not fit into a long)
 */
extern "C" long __stdcall AUDIOGetFileSizeW()
{
	return toLongClamped(CTools::FileSize); // from 2 GB on, 2147483647 is returned
}

/**
 * @brief get the title
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return title
 */
extern "C" BSTR __stdcall AUDIOGetTitleW() 
{
	return Title.AllocSysString();
}

/**
 * @brief set the title
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString title
 */
extern "C" void __stdcall AUDIOSetTitleW(LPCWSTR textString) 
{
	textString = getValidPointer(textString);
	Title = textString;  
}

/**
 * @brief get the artist
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return artist
 */
extern "C" BSTR __stdcall AUDIOGetArtistW() 
{
	return Artist.AllocSysString();	
}

/**
 * @brief set the artist
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString artist
 */
extern "C" void __stdcall AUDIOSetArtistW(LPCWSTR textString) 
{ 
	Artist = getValidPointer(textString);
}

/**
 * @brief get the album
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return album
 */
extern "C" BSTR __stdcall AUDIOGetAlbumW() 
{
	return Album.AllocSysString();
}

/**
 * @brief set the album
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString album
 */
extern "C" void __stdcall AUDIOSetAlbumW(LPCWSTR textString) 
{
	Album = getValidPointer(textString);  
}

/**
 * @brief get the comment
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return Comment
 */
extern "C" BSTR __stdcall AUDIOGetCommentW() 
{
	return Comment.AllocSysString();
}

/**
 * @brief set the comment
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString comment
 */
extern "C" void __stdcall AUDIOSetCommentW(LPCWSTR textString) 
{
	Comment = getValidPointer(textString);
}

/**
 * @brief get the composer
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return Composer
 */
extern "C" BSTR __stdcall AUDIOGetComposerW() 
{
	return Composer.AllocSysString();
}

/**
 * @brief set the composer
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString composer
 */
extern "C" void __stdcall AUDIOSetComposerW(LPCWSTR textString) 
{
	Composer = getValidPointer(textString);
}

/**
 * @brief get the genre
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return genre
 */
extern "C" BSTR __stdcall AUDIOGetGenreW() 
{
	return Genre.AllocSysString();
}

/**
 * @brief set the genre
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString genre
 */
extern "C" void __stdcall AUDIOSetGenreW(LPCWSTR textString) 
{
	Genre = getValidPointer(textString);  
}

/**
 * @brief get the track
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return track
 */
extern "C" BSTR __stdcall AUDIOGetTrackW() 
{
	return Track.AllocSysString();
}

/**
 * @brief set the track
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString track
 */
extern "C" void __stdcall AUDIOSetTrackW(LPCWSTR textString) 
{
	Track = getValidPointer(textString);  
}

/**
 * @brief get the year
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return year
 */
extern "C" BSTR __stdcall AUDIOGetYearW() 
{
	return Year.AllocSysString();
}

/**
 * @brief set the year
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @param textString year
 */
extern "C" void __stdcall AUDIOSetYearW(LPCWSTR textString) 
{
	Year = getValidPointer(textString);
}


/* ------------------------------------------------------------------------------------- */

/**
 * @brief get the Header Type
 *
 * @ingroup AAC
 * @since 2.0.1.0
 * @return Header Type
 */
extern "C" BSTR __stdcall AACGetHeaderTypeW()
{
	return aac.GetHeaderType().AllocSysString();
}


/**
 * @brief get the Profile
 *
 * @ingroup AAC
 * @since 2.0.1.0
 * @return Profile
 */
extern "C" BSTR __stdcall AACGetProfileW()
{
	return aac.GetProfile().AllocSysString();
}


/**
 * @brief get the bit rate type (CBR or VBR)
 *
 * @ingroup AAC
 * @since 2.0.1.0
 * @return bit rate type
 */
extern "C" BSTR __stdcall AACGetBitRateTypeW()
{
	return aac.GetBitRateType().AllocSysString();
}

/**
 * @brief get the bits per sample
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return bits per Sample
 */
extern "C" long __stdcall FLACGetBitsPerSampleW()
{
	return flac.GetBitsPerSample();
}


/**
 * @brief get the compression ratio
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return compression rate
 */
extern "C" float __stdcall FLACGetCompressionRatioW()
{
	return flac.GetRatio();
}


/**
 * @brief get the number of samples
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return number of samples
 */
extern "C" long __stdcall FLACGetSamplesW()
{
	return flac.GetSamples();
}

/**
 * @brief get the minimum block size
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return minimum block size
 */
extern "C" long __stdcall FLACGetMinBlockSizeW()
{
	return flac.GetMinBlockSize();
}


/**
 * @brief get the maximum block size in bytes
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return maximum block size
 */
extern "C" long __stdcall FLACGetMaxBlockSizeW()
{
	return flac.GetMaxBlockSize();
}


/**
 * @brief get the minimum framesize in bytes
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return minimum framesize
 */
extern "C" long __stdcall FLACGetMinFrameSizeW()
{
	return flac.GetMinFrameSize();
}


/**
 * @brief get the maximum framesize in bytes
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return maximum frame size
 */
extern "C" long __stdcall FLACGetMaxFrameSizeW()
{
	return flac.GetMaxFrameSize();
}

/**
 * @brief get the vendor
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return vendor
 */
extern "C" BSTR __stdcall FLACGetVendorW() 
{
	return flac.GetVendor().AllocSysString();
}


/**
 * @brief set the vendor
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @param textString Vendor
 */
extern "C" void __stdcall FLACSetVendorW(LPCWSTR textString) 
{
	flac.SetVendor(getValidPointer(textString));
}

/**
 * @brief get a user defined field
 *
 * use the method FLACGetItemKeysW for a list of all existing keys.
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @param ItemKey user-defined key
 * @return field user-defined item or empty if key not present
 */
extern "C" BSTR __stdcall FLACGetUserItemW(LPCWSTR ItemKey) 
{
	return flac.GetUserItem(getValidPointer(ItemKey)).AllocSysString();
}

/**
 * @brief set a user defined field
 *
 * use the method FLACGetItemKeysW for a list of all existing keys.
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @param ItemKey the key you want to set
 * @param textString new value for the key, an empty string removes this item
 */
extern "C" void __stdcall FLACSetUserItemW(LPCWSTR ItemKey, LPCWSTR textString) 
{
	flac.SetUserItem(getValidPointer(ItemKey), getValidPointer(textString));
}
/**
 * @brief get all field names (comma separated)
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return fieldnames
 */
extern "C" BSTR __stdcall FLACGetItemKeysW() 
{
	return flac.GetAllKeys().AllocSysString();
}

/**
 * @brief store a picture from a file in the tag
 *
 * There may be more than one picture frame in each tag, but only one with the same content descriptor.
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param FileName name of the picture file
 * @param Description a description of the picture
 * @param PictureType type from 0 to 20, see @ref picturetypes
 * @param asLink shows how the picture will be stored
 *
 * - -1 = only a link to the image will be stored
 * - 0 = the image data will be stored
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall FLACAddPictureFileW(LPCWSTR FileName, LPCWSTR Description, short PictureType, short asLink)
{
	CFlacCover *pic = new CFlacCover( (BYTE)PictureType, Description);
	bool result = (asLink) ? pic->setFileLink(getValidPointer(FileName)) : pic->setPictureFile(getValidPointer(FileName));
	if (result)
		flac.replaceCover(pic);
	else
		delete pic;
	return b2s(result);
}


/**
 * @brief store a picture from a byte array in the FLAC tag
 *
 * There may be more than one picture in each tag, but only one with the same content descriptor.
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param arr pointer to a byte array with the picture data
 * @param Length the size of the array
 * @param Description a description of the picture
 * @param PictureType picture type from 0 to 20, see @ref picturetypes
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall FLACAddPictureArrayW(BYTE *arr, u32 Length, LPCWSTR Description, short PictureType)
{
	CFlacCover *pic = new CFlacCover( (BYTE)PictureType, getValidPointer(Description));
	pic->data.AddMemory(arr, Length);
	return b2s(flac.replaceCover(pic));
}


/**
 * @brief remove all pictures from the FLAC tag
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 */
extern "C" void __stdcall FLACDeletePicturesW() 
{
	flac.DeletePictures();
}

/**
 * @brief remove a picture from the FLAC tag
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param Index index from 1 to FLACGetPictureCountW
 * @return normally -1, 0 on error or picture not present
 */
extern "C" short __stdcall FLACDeletePictureW(short Index) 
{
	return b2s(flac.DeletePicture(Index));
}


/**
 * @brief get the description from a picture
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param Index index from 1 to FLACGetPictureCountW
 * @return the description of the picture
 */
extern "C" BSTR __stdcall FLACGetPictureDescriptionW(short Index) 
{
	CFlacCover* cover = flac.GetCover(Index);
	return (cover != NULL) ? cover->description.AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the size from a picture in bytes
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param Index index from 1 to FLACGetPictureCountW
 * @return the size of the picture in bytes
 */
extern "C" long __stdcall FLACGetPictureSizeW(short Index) 
{
	CFlacCover* cover = flac.GetCover(Index);
	return (cover != NULL) ? cover->getPictureSize() : 0;
}

/**
 * @brief get the picture type from a picture
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param Index index from 1 to FLACGetPictureCountW
 * @return picture type from 0 to 20, see @ref picturetypes
 */
extern "C" short __stdcall FLACGetPictureTypeW(short Index) 
{
	CFlacCover* cover = flac.GetCover(Index);
	return (cover != NULL) ? (short)cover->type : 0;
}



/**
 * @brief get the picture type from a picture
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param Index index from 1 to FLACGetPictureCountW
 * @return picture type as text
 */
extern "C" BSTR __stdcall FLACGetPictureTypeTextW(short Index) 
{
	CFlacCover* cover = flac.GetCover(Index);
	return (cover != NULL) ? cover->getPictureTypeAsText().AllocSysString() : CTools::instance().GetEmptyBSTR();
}


/**
 * @brief get the mime type from a picture, e.g. "image/jpeg" or "image/bmp" or "XXX" for unknown format
 *
 * If the file was stored as a Link then this method returns the file name starting with "-->", e.g. "-->C:\test.jpg"
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param Index index from 1 to FLACGetPictureCountW
 * @return the mime type of the picture
 */
extern "C" BSTR __stdcall FLACGetPictureMimeW(short Index) 
{
	CFlacCover* cover = flac.GetCover(Index);
	return (cover != NULL) ? cover->getMime().AllocSysString() : CTools::instance().GetEmptyBSTR();
}



/**
 * @brief get the number of pictures
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @return number of pictures
 */
extern "C" short __stdcall FLACGetPictureCountW()
{
	return flac.GetPictureCount();
}

/**
 * @brief get a picture from the tag and store it in the specified file
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param FileName Name of the file where the picture will be stored
 * @param Index index from 1 to FLACGetPictureCountW
 * @return normally -1, 0 on error or index not present
 */
extern "C" short __stdcall FLACGetPictureFileW(LPCWSTR FileName, short Index) 
{
	CFlacCover* cover = flac.GetCover(Index);
	return (cover != NULL) ? b2s(cover->getPictureFile(getValidPointer(FileName))) : b2s(0);	
}

/**
 * @brief get a picture from the tag and copy it in a byte array
 *
 * @ingroup FLAC
 * @since 2.0.2.0
 * @param arr to a Byte array
 * @param maxLen maximum size of the byte array
 * @param Index index from 1 to FLACGetPictureCountW
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall FLACGetPictureArrayW(BYTE *arr, u32 maxLen, short Index)
{
	CFlacCover* cover = flac.GetCover(Index);
	return (cover != NULL) ? cover->getPictureArray(arr, maxLen) : -1;
}

/**
 * @brief store the comments in Vorbis format in a file
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall FLACSaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) == AUDIO_FORMAT_FLAC)
		return b2s(flac.SaveToFile(FileName));
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false);  
}

/**
 * @brief store the comments in Vorbis format in the last analyzed file
 *
 * @ingroup FLAC
 * @since 2.0.1.0
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall FLACSaveChangesW()
{
	return FLACSaveChangesToFileW(lastFile);
}

/* ------------------------------------------------------------------------------------- */

/**
 * @brief get the bits per sample
 *
 * @ingroup MONKEY
 * @since 2.0.1.0
 * @return bits per sample
 */
extern "C" short __stdcall MONKEYGetBitsW()
{
	return monkey.GetBits();
}


/**
 * @brief get the compression
 *
 * @ingroup MONKEY
 * @since 2.0.1.0
 * @return compression
 */
extern "C" BSTR __stdcall MONKEYGetCompressionW()
{
	return monkey.GetCompression().AllocSysString();
}


/**
 * @brief get the number of frames
 *
 * @ingroup MONKEY
 * @since 2.0.1.0
 * @return number of frames
 */
extern "C" long __stdcall MONKEYGetFramesW()
{
	return monkey.GetFrames();
}


/**
 * @brief get the peak level
 *
 * @ingroup MONKEY
 * @since 2.0.1.0
 * @return peak level
 */
extern "C" float __stdcall MONKEYGetPeakW()
{
	return monkey.GetPeak();
}


/**
 * @brief get the compression ratio
 *
 * @ingroup MONKEY
 * @since 2.0.1.0
 * @return compression ratio
 */
extern "C" float __stdcall MONKEYGetCompressionRatioW()
{
	return monkey.GetCompressionRatio();
}


/**
 * @brief get the number of samples
 *
 * @ingroup MONKEY
 * @since 2.0.1.0
 * @return samples
 */
extern "C" long __stdcall MONKEYGetSamplesW()
{
	return monkey.GetSamples();
}


/**
 * @brief get the number of samples per frame
 *
 * @ingroup MONKEY
 * @since 2.0.1.0
 * @return samples per frame
 */
extern "C" long __stdcall MONKEYGetSamplesPerFrameW()
{
	return monkey.GetSamplesPerFrame();
}

/* ------------------------------------------------------------------------------------- */

/**
 * @brief get a comma-separated list of all unique frame ids
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @return a comma-separated text string with Frame IDs
 */
extern "C" BSTR __stdcall MP4GetAllFrameIDsW()
{
	return mp4.getILSTFrameIDs().AllocSysString();
}

/**
 * @brief get the text entry of an MP4 ILST text frame
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param FrameID the frame id
 * @return frame text
 */
extern "C" BSTR __stdcall MP4GetTextFrameW(u32 FrameID)
{
	return mp4.GetILSTText(FrameID).AllocSysString();
}

/**
 * @brief set the text entry of an MP4 ILST text frame
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @param textString the new text
 */
extern "C" void __stdcall MP4SetTextFrameW(u32 FrameID, LPCWSTR textString)
{
	mp4.SetILSTText(FrameID, getValidPointer(textString));
}

/**
 * @brief get the text entry of an MP4 iTune text frame
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param FrameID the frame id
 * @return frame text
 */
extern "C" BSTR __stdcall MP4GetiTuneFrameW(LPCWSTR FrameID)
{
	return mp4.GetItuneText(getValidPointer(FrameID)).AllocSysString();
}

/**
 * @brief set the text entry of an MP4 iTune text frame
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @param textString the new text
 */
extern "C" void __stdcall MP4SetiTuneFrameW(LPCWSTR FrameID, LPCWSTR textString)
{
	mp4.SetItuneText(getValidPointer(FrameID), getValidPointer(textString));
}

/**
 * @brief get the format / version of the file
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @return format
 */
extern "C" BSTR __stdcall MP4GetVersionW()
{
	return mp4.GetFileVersion().AllocSysString();
}

/**
 * @brief get the genre
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @return genre
 */
extern "C" BSTR __stdcall MP4GetGenreW()
{
	return mp4.GetGenre().AllocSysString();	
}

/**
 * @brief set the genre
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param textString genre
 */
extern "C" void __stdcall MP4SetGenreW(LPCWSTR textString)
{
	mp4.SetGenre(getValidPointer(textString)); 
}


/**
 * @brief get the number of pictures
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @return number of pictures
 */
extern "C" long __stdcall MP4GetPictureCountW()
{
	return mp4.GetPictureCount();
}

/**
 * @brief get the track
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @return track
 */
extern "C" BSTR __stdcall MP4GetTrackW()
{
	return mp4.GetTrack().AllocSysString();	
}


/**
 * @brief set the track
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param textString track
 */
extern "C" void __stdcall MP4SetTrackW(LPCWSTR textString)
{
	mp4.SetTrack(getValidPointer(textString));
}


/**
 * @brief get a Picture with a specific Index from the Frame and store it in a file
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param FileName name of the file
 * @param Index index from 1 to MP4GetPictureCountW
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall MP4GetPictureFileW(LPCWSTR FileName, short Index)
{
	return b2s(mp4.GetPicture(getValidPointer(FileName), Index));
}

/**
 * @brief get a picture from the tag and copy it in a byte array
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param arr to a Byte array
 * @param maxLen maximum size of the byte array
 * @param Index index from 1 to MP4GetPictureCountW
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall MP4GetPictureArrayW(BYTE *arr, u32 maxLen, short Index)
{
	return mp4.GetPictureArray(arr, maxLen, Index);		
}


/**
 * @brief get the mime type from a picture, e.g. "jpg" or "png"
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param Index index from 1 to MP4GetPictureCountW
 * @return the mime type of the picture
 */
extern "C" BSTR __stdcall MP4GetPictureMimeW(short Index) 
{
	return mp4.GetPictureMime(Index).AllocSysString();
}

/**
 * @brief get the size from a picture in bytes
 *
 * @ingroup MP4
 * @since 2.0.2.0
 * @param Index index from 1 to MP4GetPictureCountW
 * @return the size of the picture in bytes
 */
extern "C" long __stdcall MP4GetPictureSizeW(short Index) 
{
	return mp4.GetPictureSize(Index);
}

/**
 * @brief store a picture from a byte array in the tag
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param arr to a byte array with the picture data
 * @param Length the size of the array
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall MP4AddPictureArrayW(BYTE *arr, u32 Length)
{
	return  b2s(mp4.AddPictureArray(arr, Length));
}

/**
 * @brief store a picture from a file in the tag
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param FileName name of the picture file
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall MP4AddPictureFileW(LPCWSTR FileName)
{
	return b2s(mp4.AddPictureFile(getValidPointer(FileName)));
}


/**
 * @brief remove all pictures
 *
 * @ingroup MP4
 * @since 2.0.1.0
 */
extern "C" void __stdcall MP4DeletePicturesW() 
{
	mp4.DeletePictures();
}

/**
 * @brief remove a picture
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param Index index from 1 to MP4GetPictureCountW
 * @return normally -1, 0 on error or picture not present
 */
extern "C" short __stdcall MP4DeletePictureW(short Index) 
{
	return b2s(mp4.DeletePictureFrame(Index));
}


/**
 * @brief remove all entries of the MP4 tag (title, pictures etc.) from memory; the file is changed with the next save
 *
 * @ingroup MP4
 * @since 2.0.1.0
 */
extern "C" void __stdcall MP4DeleteEntriesW()
{
	mp4.RemoveTag();
}

/**
 * @brief store the MP4 changes in a file
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall MP4SaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) != AUDIO_FORMAT_INVALID)
	{
		bool result = mp4.SaveToFile(FileName);
		if (result && !lastFile.IsEmpty() && lastFile.CompareNoCase(FileName) == 0)
			AUDIOAnalyzeFileW(FileName);
		return b2s(result);
	}
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false);   
}

/**
 * @brief store the MP4 changes in the last analyzed file
 *
 * @ingroup MP4
 * @since 2.0.1.0
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall MP4SaveChangesW()
{
	return MP4SaveChangesToFileW(lastFile);
}

/* ------------------------------------------------------------------------------------- */


/**
 * @brief shows the copyright state of a file
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return -1 if copyright protected, otherwise 0
 */
extern "C" short __stdcall MPEGIsCopyrightedW()
{
	return b2s(mpeg.GetCopyrightBit());
}


/**
 * @brief shows the original state of the file
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return -1 if the file is the original, otherwise 0
 */
extern "C" short __stdcall MPEGIsOriginalW()
{
	return b2s(mpeg.GetOriginalBit());
}


/**
 * @brief shows the state of padding
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return -1 if padding used, otherwise 0
 */
extern "C" short __stdcall MPEGIsPaddingW()
{
	return b2s(mpeg.GetPaddingBit());
}


/**
 * @brief shows the state of the private bit
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return -1 if set, otherwise 0
 */
extern "C" short __stdcall MPEGIsPrivateW()
{
	return b2s(mpeg.GetPrivateBit());
}

/**
 * @brief returns -1 if the frames are protected by a CRC checksum (protection bit)
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return -1 if CRC protected, otherwise 0
 */
extern "C" short __stdcall MPEGIsProtectedW() 
{
	return b2s(!mpeg.GetProtectionBit());
}

/**
 * @brief get the emphasis
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return emphasis
 */
extern "C" BSTR __stdcall MPEGGetEmphasisW()
{
	return mpeg.GetEmphasis().AllocSysString();
}


/**
 * @brief get the encoder
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return encoder
 */
extern "C" BSTR __stdcall MPEGGetEncoderW()
{
	return mpeg.GetEncoder().AllocSysString();
}

/**
 * @brief get the mpeg version e.g. "MPEG 2.0"
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return version
 */
extern "C" BSTR __stdcall MPEGGetVersionW() 
{
	return mpeg.GetFileVersion().AllocSysString();
}

/**
 * @brief get the position of the first valid frame
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return position of the frame
 */
extern "C" long __stdcall MPEGGetFramePositionW()
{
	return mpeg.GetFramePosition();
}


/**
 * @brief get the frame size in bytes
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return frame size
 */
extern "C" long __stdcall MPEGGetFrameSizeW()
{
	return mpeg.GetFrameSize();
}


/**
 * @brief get the number of Frames
 *
 * The value comes from the Xing or VBRI header. Without a header it is calculated from the file size (exact for
 * constant bit rate files without additional data after the audio). If the configuration value MPEGEXACTREAD was set
 * before the analysis, the frames are counted one by one and the value is exact in every case, see SetConfigValueW().
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return number of frames
 */
extern "C" long __stdcall MPEGGetFramesW()
{
	return mpeg.GetFrames();
}


/**
 * @brief get the layer
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return layer
 */
extern "C" BSTR __stdcall MPEGGetLayerW()
{
	return mpeg.GetLayer().AllocSysString();
}


/**
 * @brief returns -1 if it is variable bitrate
 *
 * A file is recognized as VBR if it has a Xing or VBRI header. A VBR file without such a header is only recognized
 * if the configuration value MPEGEXACTREAD was set before the analysis, see SetConfigValueW().
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @return -1 if variable bitrate, otherwise 0
 */
extern "C" short __stdcall MPEGIsVBRW()
{
	return b2s(mpeg.IsVBR());
}


/**
 * @brief modify the state of the copyright bit
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @param FileName name of the MPEG file, which is modified immediately
 * @param newValue 1 set and 0 reset the Copyright Bit
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall MPEGSetCopyrightedW(LPCWSTR FileName, short newValue)
{
	return b2s(mpeg.SetCopyrightBit(getValidPointer(FileName), (newValue != 0) ));
}


/**
 * @brief modify the state of the original bit
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @param FileName name of the MPEG file, which is modified immediately
 * @param newValue 1 set and 0 reset the Original Bit
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall MPEGSetOriginalW(LPCWSTR FileName, short newValue)
{
	return b2s(mpeg.SetOriginalBit(getValidPointer(FileName), (newValue != 0) ));
}


/**
 * @brief modify the state of the private bit
 *
 * @ingroup MPEG
 * @since 2.0.1.0
 * @param FileName name of the MPEG file, which is modified immediately
 * @param newValue 1 set and 0 reset the Private Bit
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall MPEGSetPrivateW(LPCWSTR FileName, short newValue)
{
	return b2s(mpeg.SetPrivateBit(getValidPointer(FileName), (newValue != 0) ));
}


/**
 * @brief get the number of frames
 *
 * @ingroup MPP
 * @since 2.0.1.0
 * @return number of Frames
 */
extern "C" long __stdcall MPPGetFramesW()
{
	return mpp.GetFrameCount();
}


/**
 * @brief get the stream version
 *
 * @ingroup MPP
 * @since 2.0.1.0
 * @return version
 */
extern "C" short __stdcall MPPGetStreamVersionW()
{
	return mpp.GetStreamVersion();
}

/* ------------------------------------------------------------------------------------- */

/**
 * @brief get the nominal bitrate
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return nominal bitrate
 */
extern "C" long __stdcall OGGGetBitRateNominalW()
{
	return ogg.GetBitRateNominal();
}


/**
 * @brief get the album
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return album
 */
extern "C" BSTR __stdcall OGGGetAlbumW()
{
	return ogg.GetUserItem(VORBIS_ALBUM).AllocSysString();
}


/**
 * @brief set the album
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString album
 */
extern "C" void __stdcall OGGSetAlbumW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_ALBUM, getValidPointer(textString));
}


/**
 * @brief get the artist
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return artist
 */
extern "C" BSTR __stdcall OGGGetArtistW()
{
	return ogg.GetUserItem(VORBIS_ARTIST).AllocSysString();
}


/**
 * @brief set the artist
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString artist
 */
extern "C" void __stdcall OGGSetArtistW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_ARTIST, getValidPointer(textString));
}


/**
 * @brief get the comment
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return comment
 */
extern "C" BSTR __stdcall OGGGetCommentW()
{
	return ogg.GetUserItem(VORBIS_COMMENT).AllocSysString();
}


/**
 * @brief set the comment
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString comment
 */
extern "C" void __stdcall OGGSetCommentW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_COMMENT, getValidPointer(textString));
}


/**
 * @brief get the date
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return date
 */
extern "C" BSTR __stdcall OGGGetDateW()
{
	return ogg.GetUserItem(VORBIS_DATE).AllocSysString();
}


/**
 * @brief set the date
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString date
 */
extern "C" void __stdcall OGGSetDateW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_DATE, getValidPointer(textString));
}


/**
 * @brief get the description
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return description
 */
extern "C" BSTR __stdcall OGGGetDescriptionW()
{
	return ogg.GetUserItem(VORBIS_DESCRIPTION).AllocSysString();
}


/**
 * @brief set the description
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString description
 */
extern "C" void __stdcall OGGSetDescriptionW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_DESCRIPTION, getValidPointer(textString));
}


/**
 * @brief get the genre
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return genre
 */
extern "C" BSTR __stdcall OGGGetGenreW()
{
	return ogg.GetUserItem(VORBIS_GENRE).AllocSysString();
}


/**
 * @brief set the genre
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString genre
 */
extern "C" void __stdcall OGGSetGenreW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_GENRE, getValidPointer(textString));
}


/**
 * @brief get the Performer
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return Performer
 */
extern "C" BSTR __stdcall OGGGetPerformerW()
{
	return ogg.GetUserItem(VORBIS_PERFORMER).AllocSysString();
}


/**
 * @brief set the Performer
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString performer
 */
extern "C" void __stdcall OGGSetPerformerW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_PERFORMER, getValidPointer(textString));
}


/**
 * @brief get the title
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return title
 */
extern "C" BSTR __stdcall OGGGetTitleW()
{
	return ogg.GetUserItem(VORBIS_TITLE).AllocSysString();
}


/**
 * @brief set the title
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString title
 */
extern "C" void __stdcall OGGSetTitleW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_TITLE, getValidPointer(textString));
}


/**
 * @brief get the track
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return track
 */
extern "C" BSTR __stdcall OGGGetTrackW()
{
	return ogg.GetUserItem(VORBIS_TRACKNUMBER).AllocSysString();
}


/**
 * @brief set the track
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString track
 */
extern "C" void __stdcall OGGSetTrackW(LPCWSTR textString)
{
	ogg.SetUserItem(VORBIS_TRACKNUMBER, getValidPointer(textString));
}


/**
 * @brief get the vendor
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return vendor
 */
extern "C" BSTR __stdcall OGGGetVendorW()
{
	return ogg.GetVendor().AllocSysString();
}


/**
 * @brief set the vendor
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString vendor
 */
extern "C" void __stdcall OGGSetVendorW(LPCWSTR textString)
{
	ogg.SetVendor(getValidPointer(textString));
}

/**
 * @brief get the version
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return version
 */
extern "C" BSTR __stdcall OGGGetVersionW() 
{
	return ogg.GetUserItem(VORBIS_VERSION).AllocSysString();
}

/**
 * @brief set the version
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString version
 */
extern "C" void __stdcall OGGSetVersionW(LPCWSTR textString) 
{
	ogg.SetUserItem(VORBIS_VERSION, getValidPointer(textString));
}

/**
 * @brief get the copyright
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return copyright
 */
extern "C" BSTR __stdcall OGGGetCopyrightW() 
{
	return ogg.GetUserItem(VORBIS_COPYRIGHT).AllocSysString();
}

/**
 * @brief set the copyright
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString copyright
 */
extern "C" void __stdcall OGGSetCopyrightW(LPCWSTR textString) 
{
	ogg.SetUserItem(VORBIS_COPYRIGHT, getValidPointer(textString));
}

/**
 * @brief get the license
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return license
 */
extern "C" BSTR __stdcall OGGGetLicenseW() 
{
	return ogg.GetUserItem(VORBIS_LICENSE).AllocSysString();
}

/**
 * @brief set the license
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString license
 */
extern "C" void __stdcall OGGSetLicenseW(LPCWSTR textString) 
{
	ogg.SetUserItem(VORBIS_LICENSE, getValidPointer(textString));
}

/**
 * @brief get the organization
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return organization
 */
extern "C" BSTR __stdcall OGGGetOrganizationW() 
{
	return ogg.GetUserItem(VORBIS_ORGANIZATION).AllocSysString();
}

/**
 * @brief set the organization
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString organization
 */
extern "C" void __stdcall OGGSetOrganizationW(LPCWSTR textString) 
{
	ogg.SetUserItem(VORBIS_ORGANIZATION, getValidPointer(textString));
}

/**
 * @brief get the location
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return location
 */
extern "C" BSTR __stdcall OGGGetLocationW() 
{
	return ogg.GetUserItem(VORBIS_LOCATION).AllocSysString();
}

/**
 * @brief set the location
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString location
 */
extern "C" void __stdcall OGGSetLocationW(LPCWSTR textString) 
{
	ogg.SetUserItem(VORBIS_LOCATION, getValidPointer(textString));
}

/**
 * @brief get the contact
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return contact
 */
extern "C" BSTR __stdcall OGGGetContactW() 
{
	return ogg.GetUserItem(VORBIS_CONTACT).AllocSysString();
}

/**
 * @brief set the contact
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString contact
 */
extern "C" void __stdcall OGGSetContactW(LPCWSTR textString) 
{
	ogg.SetUserItem(VORBIS_CONTACT, getValidPointer(textString));
}

/**
 * @brief get the <a href="http://en.wikipedia.org/wiki/International_Standard_Recording_Code" target="_blank">International Standard Recording Code</a>
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return International Standard Recording Code
 */
extern "C" BSTR __stdcall OGGGetISRCW() 
{
	return ogg.GetUserItem(VORBIS_ISRC).AllocSysString();
}

/**
 * @brief set the <a href="http://en.wikipedia.org/wiki/International_Standard_Recording_Code" target="_blank">International Standard Recording Code</a>
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param textString International Standard Recording Code
 */
extern "C" void __stdcall OGGSetISRCW(LPCWSTR textString) 
{
	ogg.SetUserItem(VORBIS_ISRC, getValidPointer(textString));
}

/**
 * @brief get all field names (comma separated)
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return fieldnames
 */
extern "C" BSTR __stdcall OGGGetItemKeysW() 
{
	return ogg.GetAllKeys().AllocSysString();
}

/**
 * @brief get a user defined field
 *
 * use the method OGGGetItemKeysW for a list of all existing keys.
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param ItemKey user-defined key
 * @return field user-defined item or empty if key not present
 */
extern "C" BSTR __stdcall OGGGetUserItemW(LPCWSTR ItemKey) 
{
	return ogg.GetUserItem(getValidPointer(ItemKey)).AllocSysString();
}

/**
 * @brief set a user defined field
 *
 * use the method OGGGetItemKeysW for a list of all existing keys.
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param ItemKey the key you want to set
 * @param textString new value for the key, an empty string removes this item
 */
extern "C" void __stdcall OGGSetUserItemW(LPCWSTR ItemKey, LPCWSTR textString) 
{
	ogg.SetUserItem(getValidPointer(ItemKey), getValidPointer(textString));
}


/**
 * @brief stores the Ogg comments in a file
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall OGGSaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) == AUDIO_FORMAT_OGGVORBIS)
		return b2s(ogg.SaveTag(FileName));
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false);
}

/**
 * @brief stores the Ogg comments in the last analyzed file
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall OGGSaveChangesW()
{
	return OGGSaveChangesToFileW(lastFile);
}


/**
 * @brief remove the Ogg comment from a file. Attention: This function removes the tag immediately!
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return -1 if removed, otherwise 0
 */
extern "C" short __stdcall OGGRemoveTagFromFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) == AUDIO_FORMAT_OGGVORBIS)
		return b2s(ogg.RemoveTag(FileName));
	return b2s(false);
}

/**
 * @brief remove the Ogg comment from the last analyzed file. Attention: This function removes the tag immediately!
 *
 * @ingroup OGG
 * @since 2.0.1.0
 * @return -1 if removed, otherwise 0
 */
extern "C" short __stdcall OGGRemoveTagW()
{
	if (GetFormat(lastFile) == AUDIO_FORMAT_OGGVORBIS)
		return b2s(ogg.RemoveTag(lastFile));
	return b2s(false);
}

/* ------------------------------------------------------------------------------------- */

/**
 * @brief get the bits per sample
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return bits per sample
 */
extern "C" long __stdcall WAVGetBitsPerSampleW()
{
	return wav.GetBitsPerSample();
}


/**
 * @brief get the block align
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return block align
 */
extern "C" long __stdcall WAVGetBlockAlignW()
{
	return wav.GetBlockAlign();
}


/**
 * @brief get the number of bytes per second
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return number of bytes
 */
extern "C" long __stdcall WAVGetBytesPerSecondW()
{
	return wav.GetBytesPerSecond();
}


/**
 * @brief get the number of channels
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return number of channels
 */
extern "C" long __stdcall WAVGetChannelsW()
{
	return wav.GetChannels();
}


/**
 * @brief get the size of the header
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return header size
 */
extern "C" long __stdcall WAVGetHeaderSizeW()
{
	return wav.GetHeaderSize();
}

/**
 * @brief get the format as text string
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return format
 */
extern "C" BSTR __stdcall WAVGetFormatW() 
{
	return wav.GetFormat().AllocSysString();
}

/**
 * @brief get the format-id of the file
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return format number with the following meaning
 * @retval 0 unknown
 * @retval 1 PCM/uncompressed
 * @retval 2 Microsoft ADPCM
 * @retval 6 ITU G.711 a-law
 * @retval 7 ITU G.711 µ-law
 * @retval 17 IMA ADPCM
 * @retval 20 ITU G.723 ADPCM (Yamaha)
 * @retval 49 GSM 6.10
 * @retval 64 ITU G.721 ADPCM
 * @retval 80 MPEG
 */
extern "C" short __stdcall WAVGetFormatIDW() 
{
	return wav.GetFormatID();
}

/**
 * @brief get a comma-separated list of all unique info chunk ids
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return a comma-separated text string with info chunk IDs
 */
extern "C" BSTR __stdcall WAVGetInfoChunkIDsW()
{
	return wav.getINFOChunkIDs().AllocSysString();
}

/**
 * @brief get the AudioGenie version
 *
 * @ingroup UNIVERSAL
 * @since 2.0.1.0
 * @return version number from AudioGenie
 */
extern "C" BSTR __stdcall GetAudioGenieVersionW() 
{
	// The version is read from the VERSIONINFO resource of this DLL (AudioGenie3.rc), the single place to change it.
	CAtlString version;
	HMODULE module = NULL;
	if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&GetAudioGenieVersionW), &module))
	{
		HRSRC res = FindResourceW(module, MAKEINTRESOURCEW(VS_VERSION_INFO), RT_VERSION);
		HGLOBAL data = res ? LoadResource(module, res) : NULL;
		VS_FIXEDFILEINFO* info = NULL;
		UINT len = 0;
		if (data && VerQueryValueW(LockResource(data), L"\\", reinterpret_cast<LPVOID*>(&info), &len) && info)
		{
			version.Format(L"%u.%u.%u.%u", HIWORD(info->dwFileVersionMS), LOWORD(info->dwFileVersionMS),
				HIWORD(info->dwFileVersionLS), LOWORD(info->dwFileVersionLS));
		}
	}
	return version.AllocSysString();
}

/* ------------------------------------------------------------------------------------- */

/**
 * @brief get a picture from the tag and store it in a file
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @param FileName Name of the file where the picture will be stored
 * @param Index index from 1 to WMAGetPictureCountW
 * @return normally -1, 0 on error or index not present
 */
extern "C" short __stdcall WMAGetPictureFileW(LPCWSTR FileName, short Index) 
{
	return b2s(wma.GetPicture(Index, getValidPointer(FileName)));
}

/**
 * @brief get a picture from the tag and store it in a byte array
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/PICTURE
 * @param arr to a Byte array
 * @param maxLen maximum size of the byte array
 * @param Index index from 1 to WMAGetPictureCountW
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall WMAGetPictureArrayW(BYTE *arr, u32 maxLen, short Index)
{
	return (long)wma.GetPictureArray(arr, maxLen, Index);	
}

/**
 * @brief get the description from a picture
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @param Index index from 1 to WMAGetPictureCountW
 * @return the description of the picture
 */
extern "C" BSTR __stdcall WMAGetPictureDescriptionW(short Index) 
{
	return wma.GetPictureDescription(Index).AllocSysString();  
}

/**
 * @brief get the mime type from a picture
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @param Index index from 1 to WMAGetPictureCountW
 * @return the mime type of the picture
 */
extern "C" BSTR __stdcall WMAGetPictureMimeW(short Index) 
{
	return wma.GetPictureMime(Index).AllocSysString();
}

/**
 * @brief get the size from a picture in bytes
 *
 * @ingroup WMA
 * @since 2.0.2.0
 * @param Index index from 1 to WMAGetPictureCountW
 * @return the size of the picture in bytes
 */
extern "C" long __stdcall WMAGetPictureSizeW(short Index) 
{
	return wma.GetPictureSize(Index);
}

/**
 * @brief get the picture type from a picture
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @param Index index from 1 to WMAGetPictureCountW
 * @return picture type from 0 to 20, see @ref picturetypes
 */
extern "C" short __stdcall WMAGetPictureTypeW(short Index) 
{
	return (short)wma.GetPictureType(Index);
}

/**
 * @brief get the number of pictures
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @return number of pictures
 */
extern "C" short __stdcall WMAGetPictureCountW() 
{
	return (short)wma.GetPictures();
}

/**
 * @brief store a picture from a file in the tag
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @param FileName name of the picture file
 * @param Description a description of the picture
 * @param PictureType type from 0 to 20, see @ref picturetypes
 * @param Index index from 1 to n; if n is not present a new entry will be created
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall WMAAddPictureFileW(LPCWSTR FileName, LPCWSTR Description, short PictureType, short Index) 
{
	return b2s(wma.SetPicture(getValidPointer(FileName), getValidPointer(Description), Index, PictureType));	
}

/**
 * @brief store a picture from a byte array in the tag
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @param arr to a byte array with the picture data
 * @param Length the size of the array
 * @param Description a description of the picture
 * @param PictureType type from 0 to 20, see @ref picturetypes
 * @param index index from 1 to n; if n is not present a new entry will be created
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall WMAAddPictureArrayW(BYTE *arr, u32 Length, LPCWSTR Description, short PictureType, short index )
{
  return b2s(wma.SetPictureArray(arr, Length, getValidPointer(Description), index, PictureType));  
} 


/**
 * @brief get a user defined field
 *
 * use the method WMAGetItemKeysW for a list of all existing keys.
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @param ItemKey user-defined key
 * @return field user-defined item or empty if key not present
 */
extern "C" BSTR __stdcall WMAGetUserItemW(LPCWSTR ItemKey) 
{
	return wma.GetUserItem(getValidPointer(ItemKey)).AllocSysString();
}

/**
 * @brief set a user defined field
 *
 * use the method WMAGetItemKeysW for a list of all existing keys.
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @param ItemKey the key you want to set
 * @param textString new value for the key, an empty string removes this item
 */
extern "C" void __stdcall WMASetUserItemW(LPCWSTR ItemKey, LPCWSTR textString) 
{
	wma.SetUserItem(getValidPointer(ItemKey), getValidPointer(textString));
}

/**
 * @brief get all field names (comma separated)
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @return fieldnames
 */
extern "C" BSTR __stdcall WMAGetItemKeysW() 
{
	return wma.GetItemKeys().AllocSysString();
}

/**
 * @brief remove a picture with WM/Picture Tag
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 * @param Index index from 1 to WMAGetPictureCountW
 * @return normally -1, 0 on error or picture not present
 */
extern "C" short __stdcall WMADeletePictureW(short Index) 
{
	return b2s(wma.DeletePictureFrame(Index));
}

/**
 * @brief remove all pictures with WM/Picture tag
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @par WMA attribute
 *   WM/Picture
 */
extern "C" void __stdcall WMADeletePicturesW() 
{
	wma.DeletePictures();
}

/**
 * @brief returns -1 if it is variable bitrate
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @return -1 if variable bitrate, otherwise 0
 */
extern "C" short __stdcall WMAIsVBRW()
{
	return b2s(wma.IsVBR());
}

/**
 * @brief store the WMA tag in a file
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall WMASaveChangesToFileW(LPCWSTR FileName) 
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) == AUDIO_FORMAT_WMA)
	{
		bool result = wma.SaveToFile(FileName);
		if (result && !lastFile.IsEmpty() && lastFile.CompareNoCase(FileName) == 0)
		{
			CTools::instance().writeInfo(L"Re-reading %s:", FileName);
			AUDIOAnalyzeFileW(FileName);
		}
		return b2s(result);
	}
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false); 
}

/**
 * @brief store the WMA tag in the last analyzed file
 *
 * @ingroup WMA
 * @since 2.0.1.0
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall WMASaveChangesW()
{
	return WMASaveChangesToFileW(lastFile);
}
/* ------------------------------------------------------------------------------------- */

/**
 * @brief get the album
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return album
 */
extern "C" BSTR __stdcall APEGetAlbumW()
{
	return ape.GetTagItem(APE_ALBUM).AllocSysString();
}


/**
 * @brief set the album
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString album
 */
extern "C" void __stdcall APESetAlbumW(LPCWSTR textString)
{
	ape.SetTagItem(APE_ALBUM, getValidPointer(textString));
}


/**
 * @brief get the artist
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return artist
 */
extern "C" BSTR __stdcall APEGetArtistW()
{
	return ape.GetTagItem(APE_ARTIST).AllocSysString();
}


/**
 * @brief set the artist
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString artist
 */
extern "C" void __stdcall APESetArtistW(LPCWSTR textString)
{
	ape.SetTagItem(APE_ARTIST, getValidPointer(textString));
}


/**
 * @brief get the comment
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return comment
 */
extern "C" BSTR __stdcall APEGetCommentW()
{
	return ape.GetTagItem(APE_COMMENT).AllocSysString();
}


/**
 * @brief set the comment
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString comment
 */
extern "C" void __stdcall APESetCommentW(LPCWSTR textString)
{
	ape.SetTagItem(APE_COMMENT, getValidPointer(textString));
}


/**
 * @brief get the copyright
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return copyright
 */
extern "C" BSTR __stdcall APEGetCopyrightW()
{
	return ape.GetTagItem(APE_COPYRIGHT).AllocSysString();
}


/**
 * @brief set the copyright
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString Copyright
 */
extern "C" void __stdcall APESetCopyrightW(LPCWSTR textString)
{
	ape.SetTagItem(APE_COPYRIGHT, getValidPointer(textString));
}


/**
 * @brief get the genre
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return genre
 */
extern "C" BSTR __stdcall APEGetGenreW()
{
	return ape.GetTagItem(APE_GENRE).AllocSysString();
}


/**
 * @brief set the genre
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString genre
 */
extern "C" void __stdcall APESetGenreW(LPCWSTR textString)
{
	ape.SetTagItem(APE_GENRE, getValidPointer(textString));
}


/**
 * @brief get the title
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return title
 */
extern "C" BSTR __stdcall APEGetTitleW()
{
	return ape.GetTagItem(APE_TITLE).AllocSysString();
}


/**
 * @brief set the title
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString title
 */
extern "C" void __stdcall APESetTitleW(LPCWSTR textString)
{
	ape.SetTagItem(APE_TITLE, getValidPointer(textString));
}


/**
 * @brief get the track
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return track
 */
extern "C" BSTR __stdcall APEGetTrackW()
{
	return ape.GetTagItem(APE_TRACK).AllocSysString();
}


/**
 * @brief set the track
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString track
 */
extern "C" void __stdcall APESetTrackW(LPCWSTR textString)
{
	ape.SetTagItem(APE_TRACK, getValidPointer(textString));
}


/**
 * @brief get the year
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return year
 */
extern "C" BSTR __stdcall APEGetYearW()
{
	return ape.GetTagItem(APE_YEAR).AllocSysString();
}


/**
 * @brief set the year
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param textString year
 */
extern "C" void __stdcall APESetYearW(LPCWSTR textString)
{
	ape.SetTagItem(APE_YEAR, getValidPointer(textString));
}


/**
 * @brief get the size of the tag in bytes
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return tag size
 */
extern "C" long __stdcall APEGetSizeW()
{
	return ape.GetSize();
}


/**
 * @brief returns -1 if the APE tag exists
 *
 * The tag is looked for at the end of the file (in front of an ID3v1 tag) and at the beginning of the file (at the start or
 * behind an ID3v2 tag, then it begins with a header). A tag at the beginning is written again at the same place when it is
 * saved or removed, which rewrites the file. A new tag is always written at the end of the file.
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return -1 if APE tag exists, otherwise 0
 */
extern "C" short __stdcall APEExistsW()
{
	return b2s(ape.Exists());
}


/**
 * @brief get the ape version
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return version
 */
extern "C" BSTR __stdcall APEGetVersionW()
{
	return ape.GetTagVersion().AllocSysString();
}


/**
 * @brief get a user defined field
 *
 * use the method APEGetItemKeysW for a list of all existing keys.
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param Key user-defined key
 * @return field user-defined item or empty if key not present
 */
extern "C" BSTR __stdcall APEGetUserItemW(LPCWSTR Key) 
{
	return ape.GetUserDefined(getValidPointer(Key)).AllocSysString();
}


/**
 * @brief set a user defined field
 *
 * use the method APEGetItemKeysW for a list of all existing keys.
 *
 * A key has 2 to 255 characters from the range space (0x20) to tilde (0x7E); ID3, TAG, OggS and MP+ are not allowed. A new item
 * with another key is ignored (the log contains a warning). An APE tag of version 1 stays a version 1 tag and its values are
 * ANSI; all other tags are written as version 2 with UTF-8 values.
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param Key user-defined key
 * @param textString new value, an empty string removes this item
 */
extern "C" void __stdcall APESetUserItemW(LPCWSTR Key, LPCWSTR textString) 
{
	ape.SetUserDefined(getValidPointer(Key), getValidPointer(textString));
}


/**
 * @brief get all field names (comma separated) with text values
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return fieldnames
 */
extern "C" BSTR __stdcall APEGetItemKeysW() 
{
	return ape.GetAllKeys().AllocSysString();
}


/**
 * @brief store the APE tag in a file
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall APESaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) != AUDIO_FORMAT_INVALID)
		return b2s(ape.SaveToFile(FileName));
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false);  
}

/**
 * @brief store the APE tag in the last analyzed file
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall APESaveChangesW()
{
	return APESaveChangesToFileW(lastFile);
}

/**
 * @brief remove the APE tag from a file. Attention: This function removes the tag immediately!
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return -1 if removed, otherwise 0
 */
extern "C" short __stdcall APERemoveTagFromFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (ape.RemoveFromFile(FileName, true))
	{
		AUDIOAnalyzeFileW(FileName);
		return b2s(true);
	}
	return b2s(false);
}

/**
 * @brief remove the APE tag from the last analyzed file. Attention: This function removes the tag immediately!
 *
 * @ingroup APE
 * @since 2.0.1.0
 * @return -1 if removed, otherwise 0
 */
extern "C" short __stdcall APERemoveTagW()
{
	if (ape.RemoveFromFile(lastFile, true))
	{
		AUDIOAnalyzeFileW(lastFile);
		return b2s(true);
	}
	return b2s(false);
}
/* ------------------------------------------------------------------------------------- */

/**
 * @brief get the genre
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return genre
 */
extern "C" BSTR __stdcall ID3V1GetGenreW()
{
	return id3v1.GetGenre().AllocSysString();
}


/**
 * @brief set the genre
 *
 * The name is compared with the list of ID3V1GetGenreItemW; an unknown name sets the value 255 (no genre).
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param textString genre
 */
extern "C" void __stdcall ID3V1SetGenreW(LPCWSTR textString)
{
	id3v1.SetGenre(getValidPointer(textString));
}


/**
 * @brief get the album
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return album
 */
extern "C" BSTR __stdcall ID3V1GetAlbumW()
{
	return id3v1.GetAlbum().AllocSysString();
}


/**
 * @brief set the album
 *
 * The id3v1 tag has 30 characters for this field. A longer text (up to 90 characters) is stored with the enhanced tag
 * ("TAG+", 227 bytes in front of the id3v1 tag, the file then has 355 bytes of id3v1 data); reading joins both parts. Speed,
 * genre and start and end time of an existing enhanced tag are kept when the tag is written.
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param textString album
 */
extern "C" void __stdcall ID3V1SetAlbumW(LPCWSTR textString)
{
	id3v1.SetAlbum(getValidPointer(textString));
}


/**
 * @brief get the artist
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return artist
 */
extern "C" BSTR __stdcall ID3V1GetArtistW()
{
	return id3v1.GetArtist().AllocSysString();
}


/**
 * @brief set the artist
 *
 * The id3v1 tag has 30 characters for this field. A longer text (up to 90 characters) is stored with the enhanced tag
 * ("TAG+", 227 bytes in front of the id3v1 tag, the file then has 355 bytes of id3v1 data); reading joins both parts. Speed,
 * genre and start and end time of an existing enhanced tag are kept when the tag is written.
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param textString artist
 */
extern "C" void __stdcall ID3V1SetArtistW(LPCWSTR textString)
{
	id3v1.SetArtist(getValidPointer(textString));
}


/**
 * @brief get the comment
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return comment
 */
extern "C" BSTR __stdcall ID3V1GetCommentW()
{
	return id3v1.GetComment().AllocSysString();
}


/**
 * @brief set the comment
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param textString comment
 */
extern "C" void __stdcall ID3V1SetCommentW(LPCWSTR textString)
{
	id3v1.SetComment(getValidPointer(textString));
}


/**
 * @brief get the title
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return title
 */
extern "C" BSTR __stdcall ID3V1GetTitleW()
{
	return id3v1.GetTitle().AllocSysString();
}


/**
 * @brief set the title
 *
 * The id3v1 tag has 30 characters for this field. A longer text (up to 90 characters) is stored with the enhanced tag
 * ("TAG+", 227 bytes in front of the id3v1 tag, the file then has 355 bytes of id3v1 data); reading joins both parts. Speed,
 * genre and start and end time of an existing enhanced tag are kept when the tag is written.
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param textString title
 */
extern "C" void __stdcall ID3V1SetTitleW(LPCWSTR textString)
{
	id3v1.SetTitle(getValidPointer(textString));
}


/**
 * @brief get the track
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return track
 */
extern "C" BSTR __stdcall ID3V1GetTrackW()
{
	return id3v1.GetTrack().AllocSysString();
}


/**
 * @brief set the track
 *
 * The track is one byte (1 to 255). Another value, an empty text or 0 means no track: the tag is then written as id3v1.0.
 * An id3v1.1 tag (with a track) is recognized by the byte $00 before the track in the comment field; the comment has
 * 28 characters then.
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param textString track
 */
extern "C" void __stdcall ID3V1SetTrackW(LPCWSTR textString)
{
	id3v1.SetTrack(getValidPointer(textString));
}


/**
 * @brief get the year
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return year
 */
extern "C" BSTR __stdcall ID3V1GetYearW()
{
	return id3v1.GetYear().AllocSysString();
}


/**
 * @brief set the year
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param textString year
 */
extern "C" void __stdcall ID3V1SetYearW(LPCWSTR textString)
{
	id3v1.SetYear(getValidPointer(textString));
}

/**
 * @brief get the genre number
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return genre number
 */
extern "C" short __stdcall ID3V1GetGenreIDW() 
{
	return (short)id3v1.GetGenreID();
}

/**
 * @brief set the genre number
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param nNewValue genre number
 */
extern "C" void __stdcall ID3V1SetGenreIDW(short nNewValue) 
{
	id3v1.SetGenreID(nNewValue);
}

/**
 * @brief get a genre for a specific number (0 to ID3V1GetGenresW - 1)
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param number number
 * @return genre string
 */
extern "C" BSTR __stdcall ID3V1GetGenreItemW(short number) 
{
	return id3v1.GetGenreItem(number).AllocSysString();
}

/**
 * @brief get the number of supported genres
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return number of genres
 */
extern "C" short __stdcall ID3V1GetGenresW() 
{
	return (short)id3v1.NumberOfGenres();
}

/**
 * @brief get the speed of the enhanced ID3v1 tag
 *
 * The enhanced tag ("TAG+") is in front of the ID3v1 tag, see ID3V1SetTitleW.
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @return 0=not set  1=slow  2=medium  3=fast  4=hardcore
 */
extern "C" short __stdcall ID3V1GetSpeedW()
{
	return (short)id3v1.GetSpeed();
}


/**
 * @brief set the speed of the enhanced ID3v1 tag
 *
 * The enhanced tag is written if a speed, a genre or a time is set, or if a text is longer than 30 characters (see the
 * configuration value ID3V1MAXTEXTLENGTH in SetConfigValueW).
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @param speed 0=not set  1=slow  2=medium  3=fast  4=hardcore; another value means 0
 */
extern "C" void __stdcall ID3V1SetSpeedW(short speed)
{
	id3v1.SetSpeed(speed);
}


/**
 * @brief get the genre text of the enhanced ID3v1 tag
 *
 * This is a free text of up to 30 characters, not the genre number of the ID3v1 tag (see ID3V1GetGenreW).
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @return genre text
 */
extern "C" BSTR __stdcall ID3V1GetEnhancedGenreW()
{
	return id3v1.GetEnhancedGenre().AllocSysString();
}


/**
 * @brief set the genre text of the enhanced ID3v1 tag
 *
 * This is a free text of up to 30 characters, not the genre number of the ID3v1 tag (see ID3V1SetGenreW).
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @param textString genre text, an empty text removes it
 */
extern "C" void __stdcall ID3V1SetEnhancedGenreW(LPCWSTR textString)
{
	id3v1.SetEnhancedGenre(getValidPointer(textString));
}


/**
 * @brief get the start time of the enhanced ID3v1 tag
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @return start time in the form mmm:ss or empty
 */
extern "C" BSTR __stdcall ID3V1GetStartTimeW()
{
	return id3v1.GetTime(false).AllocSysString();
}


/**
 * @brief set the start time of the enhanced ID3v1 tag
 *
 * The start time is the position at which the music starts, for example to skip a lead-in.
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @param textString time in the form mmm:ss (three digits, colon, seconds from 00 to 59), an empty text removes it;
 *        another text is ignored (the log contains a warning)
 */
extern "C" void __stdcall ID3V1SetStartTimeW(LPCWSTR textString)
{
	if (!id3v1.SetTime(false, getValidPointer(textString)))
		CTools::instance().writeWarning(L"ID3v1 start time '%s' ignored: the form is mmm:ss", getValidPointer(textString));
}


/**
 * @brief get the end time of the enhanced ID3v1 tag
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @return end time in the form mmm:ss or empty
 */
extern "C" BSTR __stdcall ID3V1GetEndTimeW()
{
	return id3v1.GetTime(true).AllocSysString();
}


/**
 * @brief set the end time of the enhanced ID3v1 tag
 *
 * The end time is the position at which the music ends, for example to skip a fade-out.
 *
 * @ingroup ID3V1
 * @since 3.1.0.0
 * @param textString time in the form mmm:ss (three digits, colon, seconds from 00 to 59), an empty text removes it;
 *        another text is ignored (the log contains a warning)
 */
extern "C" void __stdcall ID3V1SetEndTimeW(LPCWSTR textString)
{
	if (!id3v1.SetTime(true, getValidPointer(textString)))
		CTools::instance().writeWarning(L"ID3v1 end time '%s' ignored: the form is mmm:ss", getValidPointer(textString));
}


/**
 * @brief returns -1 if ID3v1 tag exists
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return -1 if Tag exists, otherwise 0
 */
extern "C" short __stdcall ID3V1ExistsW()
{
	return b2s(id3v1.Exists());
}


/**
 * @brief get the version of the id3v1 tag
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return version
 */
extern "C" BSTR __stdcall ID3V1GetVersionW()
{
	return id3v1.GetTagVersion().AllocSysString();
}


/**
 * @brief remove the ID3v1 tag from a file. Attention: This function removes the tag immediately! An existing Lyrics tag is removed as well.
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param FileName the name of the file
 * @return -1 if removed, 0 on error
 */
extern "C" short __stdcall ID3V1RemoveTagFromFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (lyrics.Exists())
		lyrics.RemoveFromFile(FileName);
	if (id3v1.RemoveFromFile(FileName))
	{
		AUDIOAnalyzeFileW(FileName);
		return b2s(true);	
	}
	return b2s(false);
}

/**
 * @brief remove the ID3v1 tag from the last analyzed file. Attention: This function removes the tag immediately! An existing Lyrics tag is removed as well.
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return -1 if removed, 0 on error
 */
extern "C" short __stdcall ID3V1RemoveTagW()
{
	if (lyrics.Exists())
		lyrics.RemoveFromFile(lastFile);
	if (id3v1.RemoveFromFile(lastFile))
	{ 
		AUDIOAnalyzeFileW(lastFile);
		return b2s(true);
	}
	return b2s(false);	
}

/**
 * @brief stores the ID3v1 tag into a file
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @param FileName the name of the file
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V1SaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) != AUDIO_FORMAT_INVALID)
		return b2s(id3v1.SaveToFile(FileName));
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false);   
}

/**
 * @brief stores the ID3v1 tag into the last analyzed file
 *
 * @ingroup ID3V1
 * @since 2.0.1.0
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V1SaveChangesW()
{
	return ID3V1SaveChangesToFileW(lastFile);
}

/* ------------------------------------------------------------------------------------- */

/**
 * @brief returns -1 if the Lyrics tag exists
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return -1 if a Lyrics tag exists, otherwise 0
 */
extern "C" short __stdcall LYRICSExistsW()
{
	return b2s(lyrics.Exists());
}


/**
 * @brief get the size of the tag in bytes
 *
 * This is the size of the whole tag: "LYRICSBEGIN", the fields, the size and "LYRICS200" (version 2.00) or "LYRICSBEGIN", the
 * text and "LYRICSEND" (version 1.00). Up to version 3.1.0 the size did not contain the six digits and "LYRICS200" (15 bytes) or
 * "LYRICSEND" (9 bytes).
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return size in bytes
 */
extern "C" long __stdcall LYRICSGetSizeW()
{
	return lyrics.GetSize();
}


/**
 * @brief get the start position of the tag
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return start position
 */
extern "C" long __stdcall LYRICSGetStartPositionW()
{
	return toLongClamped(lyrics.GetStartPosition());
}


/**
 * @brief get the version
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return version
 */
extern "C" BSTR __stdcall LYRICSGetVersionW()
{
	return lyrics.GetTagVersion().AllocSysString();
}


/**
 * @brief get the album
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return album
 */
extern "C" BSTR __stdcall LYRICSGetAlbumW()
{
	return lyrics.GetAlbum().AllocSysString();
}


/**
 * @brief set the album
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString album
 */
extern "C" void __stdcall LYRICSSetAlbumW(LPCWSTR textString)
{
	lyrics.SetAlbum(getValidPointer(textString));
}


/**
 * @brief get the artist
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return artist
 */
extern "C" BSTR __stdcall LYRICSGetArtistW()
{
	return lyrics.GetArtist().AllocSysString();
}


/**
 * @brief set the artist
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString artist
 */
extern "C" void __stdcall LYRICSSetArtistW(LPCWSTR textString)
{
	lyrics.SetArtist(getValidPointer(textString));
}


/**
 * @brief get the author
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return author
 */
extern "C" BSTR __stdcall LYRICSGetAuthorW()
{
	return lyrics.GetAuthor().AllocSysString();
}


/**
 * @brief set the author
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString author
 */
extern "C" void __stdcall LYRICSSetAuthorW(LPCWSTR textString)
{
	lyrics.SetAuthor(getValidPointer(textString));
}


/**
 * @brief get the genre
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return genre
 */
extern "C" BSTR __stdcall LYRICSGetGenreW()
{
	return lyrics.GetGenre().AllocSysString();
}


/**
 * @brief set the genre
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString genre
 */
extern "C" void __stdcall LYRICSSetGenreW(LPCWSTR textString)
{
	lyrics.SetGenre(getValidPointer(textString));
}


/**
 * @brief get a link to an image
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return image link
 */
extern "C" BSTR __stdcall LYRICSGetImageLinkW()
{
	return lyrics.GetImageLink().AllocSysString();
}


/**
 * @brief set a link to an image
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString image link
 */
extern "C" void __stdcall LYRICSSetImageLinkW(LPCWSTR textString)
{
	lyrics.SetImageLink(getValidPointer(textString));
}


/**
 * @brief get the internal Indication
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return indication
 */
extern "C" BSTR __stdcall LYRICSGetIndicationW()
{
	return lyrics.GetIndication().AllocSysString();
}


/**
 * @brief set the internal Indication
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString Indication
 */
extern "C" void __stdcall LYRICSSetIndicationW(LPCWSTR textString)
{
	lyrics.SetIndication(getValidPointer(textString));
}


/**
 * @brief get the information
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return information
 */
extern "C" BSTR __stdcall LYRICSGetInformationW()
{
	return lyrics.GetInformation().AllocSysString();
}


/**
 * @brief set the information
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString information
 */
extern "C" void __stdcall LYRICSSetInformationW(LPCWSTR textString)
{
	lyrics.SetInformation(getValidPointer(textString));
}


/**
 * @brief get the lyrics
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return lyrics
 */
extern "C" BSTR __stdcall LYRICSGetLyricsW()
{
	return lyrics.GetLyrics().AllocSysString();
}


/**
 * @brief set the lyrics
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString Lyrics
 */
extern "C" void __stdcall LYRICSSetLyricsW(LPCWSTR textString)
{
	lyrics.SetLyrics(getValidPointer(textString));
}


/**
 * @brief get the title
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return title
 */
extern "C" BSTR __stdcall LYRICSGetTitleW()
{
	return lyrics.GetTitle().AllocSysString();
}


/**
 * @brief set the title
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param textString title
 */
extern "C" void __stdcall LYRICSSetTitleW(LPCWSTR textString)
{
	lyrics.SetTitle(getValidPointer(textString));
}


/**
 * @brief remove the Lyrics tag from a file. Attention: This function removes the tag immediately!
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return -1 if removed, otherwise 0
 */
extern "C" short __stdcall LYRICSRemoveTagFromFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (lyrics.RemoveFromFile(FileName))
	{    
		AUDIOAnalyzeFileW(FileName);
		return b2s(true);	
	}
	return b2s(false); 
}

/**
 * @brief remove the Lyrics tag from the last analyzed file. Attention: This function removes the tag immediately!
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return -1 if removed, otherwise 0
 */
extern "C" short __stdcall LYRICSRemoveTagW()
{
	if (lyrics.RemoveFromFile(lastFile))
	{    
		AUDIOAnalyzeFileW(lastFile);
		return b2s(true);	
	}
	return b2s(false); 
}

/**
 * @brief stores the Lyrics tag in a file
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall LYRICSSaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);
	if (GetFormat(FileName) != AUDIO_FORMAT_INVALID)
		return b2s(lyrics.SaveToFile(FileName));
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false); 
}

/**
 * @brief stores the Lyrics tag in the last analyzed file
 *
 * The tag is always written as Lyrics3 v2.00 in front of the ID3v1 tag (and its enhanced tag), which has to exist. Fields
 * that this library does not know are kept. The indication has two characters (0 or 1, otherwise it is not written); the lyrics,
 * the information and the image links have at most 99999 bytes, the other fields 250; longer texts are cut. Line breaks are
 * written as CR LF. If all fields are empty the tag is removed.
 *
 * @ingroup LYRICS
 * @since 2.0.1.0
 * @return 0 on error, otherwise -1
 */
extern "C" short __stdcall LYRICSSaveChangesW()
{
	return LYRICSSaveChangesToFileW(lastFile);
}
/* ------------------------------------------------------------------------------------- */

/**
 * @brief get the text entry of an ID3v2 frame
 *
 * The frame must be a text frame (T000 to TZZZ, except TXXX), also one that this library does not know. An ID3v2.4 text
 * frame can contain several strings; this function returns the first one. All strings are kept when the tag is saved
 * again (in an ID3v2.2 or ID3v2.3 tag they are separated by a slash).
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FrameID the frame id
 * @return frame text, empty if the frame does not exist or is not a text frame
 */
extern "C" BSTR __stdcall ID3V2GetTextFrameW(u32 FrameID)
{
	return id3v2.GetText(FrameID).AllocSysString();
}

/**
 * @brief set the text entry of a frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * An empty text removes the frame.
 *
 * @param FrameID the ID of the frame
 * @param textString the new text
 */
extern "C" void __stdcall ID3V2SetTextFrameW(u32 FrameID, LPCWSTR textString)
{
	if (textString == NULL || wcslen(textString) == 0)
		id3v2.deleteFrame(FrameID);
	else
		id3v2.SetText(FrameID, getValidPointer(textString));
}

/**
 * @brief get the text of an url frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetURLFrameW(u32 FrameID)
{
	return id3v2.GetURL(FrameID).AllocSysString();
}

/**
 * @brief set the text of an url frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * An empty text removes the frame.
 *
 * @param FrameID the ID of the frame
 * @param textString the new text
 */
extern "C" void __stdcall ID3V2SetURLFrameW(u32 FrameID, LPCWSTR textString)
{
	if (textString == NULL || wcslen(textString) == 0)
		id3v2.deleteFrame(FrameID);
	else
		id3v2.SetURL(FrameID, getValidPointer(textString));
}
/**
 * @brief get the frame count of a frame ID
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @return frame count
 */
extern "C" short __stdcall ID3V2GetFrameCountW(u32 FrameID)
{
	return (short)id3v2.countFrame(FrameID);
}

/**
 * @brief deletes all frames with the frame id
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @return normally -1, 0 if frame not found
 */
extern "C" short __stdcall ID3V2DeleteAllFramesW(u32 FrameID)
{
	return (id3v2.deleteAllFrames(FrameID) == 0) ? 0 : -1 ;
}

/**
 * @brief deletes a specific frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @param Index from 1 to frame count
 * @return normally -1, 0 if frame not found
 */
extern "C" short __stdcall ID3V2DeleteSelectedFrameW(u32 FrameID, short Index)
{
	return b2s(id3v2.deleteFrame(FrameID, Index));
}

/**
 * @brief set the format and text encoding of the ID3v2 tag
 *
 * The encoding applies to all text frames when the tag is saved. Text that ISO-8859-1 cannot represent is not replaced by
 * question marks: such a frame is stored as UTF-16 with BOM instead. The encodings 2 (UTF-16BE) and 3 (UTF-8) exist only in
 * id3v2.4; with the format id3v2.2 or id3v2.3 they are rejected.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param format 0=existing format 1=id3v2.2  2=id3v2.3 3=id3v2.4
 * @param encoding 0= ISO-8859-1  1=UTF-16 with BOM  2=UTF-16BE without BOM  3=UTF-8
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2SetFormatAndEncodingW(short format, short encoding)
{
	return b2s(id3v2.setTargetFormatAndEncoding((BYTE)format, (BYTE)encoding));
}

/**
 * @brief get the encoding of the selected frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @return Encoding -1= FrameID not found  0= ISO-8859-1  1=UTF-16 with BOM  2=UTF-16BE without BOM  3=UTF-8
 */
extern "C" short __stdcall ID3V2GetEncodingW(u32 FrameID)
{
	return id3v2.getEncoding(FrameID);
}

/**
 * @brief set a configuration value.
 *
 * The following keys are currently supported:
 *
 * | Key | ID | Defaultvalue | Description |
 * |---|---|---|---|
 * | 0 | MPEGEXACTREAD | 0 | a non-zero value activates reading all MPEG frames (see below); replaces SetMPEGAnalyzeAllFrames |
 * | 1 | ID3V2PADDINGSIZE | 4096 | the padding size in bytes for an ID3v2 tag |
 * | 2 | WRITEBLOCKSIZE | 524288 | the block size in bytes for internal file copy |
 * | 3 | DOEVENTSMILLIS | 250 | milliseconds after which AudioGenie fires a DoEvent |
 * | 4 | MAXTEXTBUFFER | 262144 | the maximum text size in bytes |
 * | 5 | WMAPADDINGSIZE | 4096 | the padding size in bytes for a WMA tag |
 * | 6 | MP4PADDINGSIZE | 4096 | the padding size in bytes for an MP4 tag |
 * | 7 | ANSICODEPAGE | 1252 | the code page of ISO-8859-1 / ANSI strings, 0 = the code page of the system (see below) |
 * | 8 | ID3V2LINKEDPICTURES | 0 | 1 = read the picture that an ID3v2 APIC frame links to (MIME type <tt>--></tt>) from disk |
 * | 9 | ID3V1MAXTEXTLENGTH | 90 | 30 to 90 = the longest title, artist and album that is written to an ID3v1 tag (see below) |
 *
 * <b>ANSICODEPAGE</b>
 *
 * Text with the encoding ISO-8859-1 (ID3v2 encoding 0, ID3v1 and other ANSI strings) is converted with this Windows code page. The
 * default 1252 contains all characters of ISO-8859-1 and gives the same result on every computer. Use 28591 for strict
 * ISO-8859-1 (the bytes $80 to $9F are then control characters), or a code page such as 1251 for tags that were written with
 * that code page. 0 selects the code page of the system (the behaviour of version 2.0.4). Set the value before the analysis.
 *
 * <b>ID3V1MAXTEXTLENGTH</b>
 *
 * The ID3v1 tag has 30 characters for title, artist and album. A longer text is stored in the enhanced tag ("TAG+", 227 bytes in
 * front of the ID3v1 tag) up to 90 characters; a longer text is cut. With the value 30 a longer text is cut at 30 characters and the
 * enhanced tag is written only if a speed, a genre text or a time is set (ID3V1SetSpeedW, ID3V1SetEnhancedGenreW,
 * ID3V1SetStartTimeW, ID3V1SetEndTimeW). Values outside of 30 to 90 are set to the nearest limit. The value does not
 * change reading: an existing enhanced tag is always read.
 *
 * <b>ID3V2LINKEDPICTURES</b>
 *
 * An ID3v2 picture frame can contain a link to a file instead of the picture (MIME type <tt>--></tt>). The link comes from the
 * tag of the audio file, so it is not followed by default (the picture size is 0); otherwise a manipulated file could make
 * an application read any file of the computer. Set the value to 1 to load such pictures.
 *
 * <b>MPEGEXACTREAD</b>
 *
 * Set this value <b>before</b> AUDIOAnalyzeFileW(); it only affects MPEG audio files (MP1/MP2/MP3).
 *
 * By default the duration, the number of frames and the bit rate are taken from a Xing or VBRI header, if the file has one.
 * For files without such a header they are calculated from the file size and the bit rate of the first frame. This is exact
 * for files with a constant bit rate (CBR) unless there is additional data after the last frame, which makes the duration
 * too long. For files with a variable bit rate (VBR) that lack the header, the values are only an estimate, and the file
 * is treated as CBR (MPEGIsVBRW() returns 0).
 *
 * With MPEGEXACTREAD not equal to 0 the analysis reads the whole file frame by frame instead:
 * - MPEGGetFramesW(), AUDIOGetDurationW() and AUDIOGetBitrateW() are exact for VBR files, even without a header,
 *   and the average bit rate is calculated from all frames. The values of the Xing/VBRI header are ignored.
 * - MPEGIsVBRW() returns -1 if the bit rate changes between the frames.
 * - For files with a constant bit rate (CBR) the number of frames and the duration are counted as well, so data after the
 *   last frame (e.g. junk or a tag that is not recognized) does not make the duration too long.
 * - The analysis takes as long as it takes to read the file: about 6 ms for a 40 MB file on a fast SSD, but seconds
 *   for a large collection on a slow disk or a network drive. Without the setting only a few small blocks at the
 *   beginning and at the end of the file are read, regardless of the file size.
 * - Data that is not an MPEG frame (for example damaged parts) is skipped byte by byte; the tags at the end of the file
 *   (ID3v1, Lyrics3, APE) are not scanned.
 * - The properties of the first frame (padding, private, copyright and original bit, channel mode extension) are
 *   kept; they are not overwritten by the last frame.
 *
 * Recommendation: leave the setting off for scanning large collections. Switch it on if the duration of VBR files
 * without a header must be correct, e.g. before you show or compare durations.
 *
 * @ingroup UNIVERSAL
 * @since 2.0.1.0
 * @param key the configuration key (see the table)
 * @param value the new value
 */
extern "C" void __stdcall SetConfigValueW(long key, long value)
{
	CTools::instance().setConfigValue(key, value);
}
/**
 * @brief get a configuration value.
 *
 * The following keys are currently supported:
 *
 * | Key | ID | Description |
 * |---|---|---|
 * | 0 | MPEGEXACTREAD | a non-zero value means that all MPEG frames are read |
 * | 1 | ID3V2PADDINGSIZE | the padding size in bytes for an ID3v2 tag |
 * | 2 | WRITEBLOCKSIZE | the block size in bytes for internal file copy |
 * | 3 | DOEVENTSMILLIS | milliseconds after which AudioGenie fires a DoEvent |
 * | 4 | MAXTEXTBUFFER | the maximum text size in bytes |
 * | 5 | WMAPADDINGSIZE | the padding size in bytes for a WMA tag |
 * | 6 | MP4PADDINGSIZE | the padding size in bytes for an MP4 tag |
 * | 7 | ANSICODEPAGE | the code page of ISO-8859-1 / ANSI strings |
 * | 8 | ID3V2LINKEDPICTURES | 1 if linked pictures are read from disk |
 * | 9 | ID3V1MAXTEXTLENGTH | the longest text that is written to an ID3v1 tag |
 *
 * @ingroup UNIVERSAL
 * @since 2.0.1.0
 * @param key the configuration key (see SetConfigValueW)
 * @return the value
 */
extern "C" long __stdcall GetConfigValueW(long key)
{
	return CTools::instance().getConfigValue(key);
}

/**
 * @brief "TCON" get the content type / genre
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   TCON
 * @return genre
 */
extern "C" BSTR __stdcall ID3V2GetGenreW()
{
	return id3v2.GetGenre().AllocSysString();
}


/**
 * @brief "TCON" set the content type / genre
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   TCON
 * @param textString genre
 */
extern "C" void __stdcall ID3V2SetGenreW(LPCWSTR textString)
{
	id3v2.SetText(F_TCON, getValidPointer(textString));
}



/**
 * @brief "PCNT" get a counter of the number of times a file has been played
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   PCNT
 * @return counter
 */
extern "C" long __stdcall ID3V2GetPlayCounterW() 
{
	id3frame = id3v2.findFrame(F_PCNT);
	return (id3frame != NULL) ? cPCNT(id3frame)->getCounter() : 0;
}

/**
 * @brief "PCNT" set a counter of the number of times a file has been played
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   PCNT
 * @param counter counter
 */
extern "C" void __stdcall ID3V2SetPlayCounterW(long counter) 
{
	id3v2.replaceFrame(new CID3F_PCNT(counter));	
}


/**
 * @brief returns -1 if the ID3v2 tag exists
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @return -1 if ID3v2 tag exists, otherwise 0
 */
extern "C" short __stdcall ID3V2ExistsW()
{
	return b2s(id3v2.Size > 0);
}


/**
 * @brief get the size of the tag in bytes
 *
 * The size includes the header, the extended header, the frames, the padding and the footer (ID3v2.4).
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @return the size in bytes
 */
extern "C" long __stdcall ID3V2GetSizeW()
{
	return id3v2.Size;
}


/**
 * @brief get the ID3v2 version number
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @return "2.2", "2.3" or "2.4", empty if there is no ID3v2 tag
 */
extern "C" BSTR __stdcall ID3V2GetVersionW()
{
	CAtlString result;
	switch (id3v2.Version)
	{
	case 2:	result = _T("2.2"); break;
	case 3:	result = _T("2.3"); break;
	case 4: result = _T("2.4"); break;
	default: result.Empty();
	}
	return result.AllocSysString();
}


/**
 * @brief "COMM" set a comment
 *
 * There may be more than one 'COMM' frame in each tag, but only one with the same language and content descriptor.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMM
 * @param Language id of the entry, e.g. ENG for English (must be 3 bytes long)
 * @param Description the description of the entry
 * @param Text the comment
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddCommentW(LPCWSTR Language, LPCWSTR Description, LPCWSTR Text)
{
	return b2s(id3v2.replaceFrame(new CID3F_COMM(getValidPointer(Language), getValidPointer(Description), getValidPointer(Text))));	
}


/**
 * @brief get the description from a comment
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMM
 * @param Index index from 1 to comment frame count
 * @return the description of the comment
 */
extern "C" BSTR __stdcall ID3V2GetCommentDescriptionW(short Index)
{
	id3frame = id3v2.findFrame(F_COMM, Index);
	return (id3frame != NULL) ? cCOMM(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();
}


/**
 * @brief get the language from a comment
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMM
 * @param Index index from 1 to comment frame count
 * @return the language id of the comment, e.g. ENG for English
 */
extern "C" BSTR __stdcall ID3V2GetCommentLanguageW(short Index)
{
	id3frame = id3v2.findFrame(F_COMM, Index);
	return (id3frame != NULL) ? cCOMM(id3frame)->getLanguage().AllocSysString() : CTools::instance().GetEmptyBSTR();
}


/**
 * @brief get a comment
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMM
 * @param Index index from 1 to comment frame count
 * @return the real comment
 */
extern "C" BSTR __stdcall ID3V2GetCommentW(short Index)
{
	id3frame = id3v2.findFrame(F_COMM, Index);
	return (id3frame != NULL) ? cCOMM(id3frame)->getText().AllocSysString() : CTools::instance().GetEmptyBSTR();
}


/**
 * @brief imports a .cue file and creates CTOC / CHAP frames
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FileName the name of the import file
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2ImportCueFileW(LPCWSTR FileName)
{
	return b2s(id3v2.parseCueFile(getValidPointer(FileName)));
}

/**
 * @brief "USLT" set a unsynchronised lyric
 *
 * There may be more than one 'USLT' frame in each tag, but only one with the same language and content descriptor.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   USLT
 * @param Language ID of the entry, e.g. ENG for English (must be 3 bytes long)
 * @param Description the description of the entry
 * @param Text the lyric
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddLyricW(LPCWSTR Language, LPCWSTR Description, LPCWSTR Text)
{
	return b2s(id3v2.replaceFrame(new CID3F_USLT(getValidPointer(Language), getValidPointer(Description), getValidPointer(Text))));	
}


/**
 * @brief get the description from a lyric
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   USLT
 * @param Index index from 1 to lyric frame count
 * @return the description of the lyric
 */
extern "C" BSTR __stdcall ID3V2GetLyricDescriptionW(short Index)
{
	id3frame = id3v2.findFrame(F_USLT, Index);
	return (id3frame != NULL) ? cUSLT(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}


/**
 * @brief get the language from a lyrics
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   USLT
 * @param Index index from 1 to lyric frame count
 * @return the language id of the lyric, e.g. ENG for English
 */
extern "C" BSTR __stdcall ID3V2GetLyricLanguageW(short Index)
{
	id3frame = id3v2.findFrame(F_USLT, Index);
	return (id3frame != NULL) ? cUSLT(id3frame)->getLanguage().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}


/**
 * @brief get a Lyric
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   USLT
 * @param Index index from 1 to Lyric frame count
 * @return the text of the lyric
 */
extern "C" BSTR __stdcall ID3V2GetLyricW(short Index)
{
	id3frame = id3v2.findFrame(F_USLT, Index);
	return (id3frame != NULL) ? cUSLT(id3frame)->getText().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}

/**
 * @brief get the description from a picture
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param Index index from 1 to picture frame count
 * @return the description of the picture
 */
extern "C" BSTR __stdcall ID3V2GetPictureDescriptionW(short Index) 
{
	id3frame = id3v2.findFrame(F_APIC, Index);
	return (id3frame != NULL) ? cAPIC(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the size from a picture in bytes
 *
 * @ingroup ID3V2
 * @since 2.0.2.0
 * @par ID3v2 frame
 *   APIC
 * @param Index index from 1 to picture frame count
 * @return the size of the picture in bytes
 */
extern "C" long __stdcall ID3V2GetPictureSizeW(short Index) 
{
	id3frame = id3v2.findFrame(F_APIC, Index);
	return (id3frame != NULL) ? cAPIC(id3frame)->getPictureSize() : 0;
}

/**
 * @brief get the picture type from a picture
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param Index index from 1 to picture frame count
 * @return picture type from 0 to 20, see @ref picturetypes
 */
extern "C" short __stdcall ID3V2GetPictureTypeW(short Index) 
{
	id3frame = id3v2.findFrame(F_APIC, Index);
	return (id3frame != NULL) ? cAPIC(id3frame)->getPictureType() : 0;
}

/**
 * @brief get the picture type from a picture
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param Index index from 1 to picture frame count
 * @return picture type as text
 */
extern "C" BSTR __stdcall ID3V2GetPictureTypeTextW(short Index) 
{
	id3frame = id3v2.findFrame(F_APIC, Index);
	return (id3frame != NULL) ? cAPIC(id3frame)->getPictureTypeAsText().AllocSysString() : CTools::instance().GetEmptyBSTR();
}


/**
 * @brief get the mime type from a picture, e.g. "image/jpeg" or "image/bmp" or "XXX" for unknown format
 *
 * If the file was stored as a Link then this method returns the file name starting with "-->", e.g. "-->C:\test.jpg"
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param Index index from 1 to picture frame count
 * @return the mime type of the picture
 */
extern "C" BSTR __stdcall ID3V2GetPictureMimeW(short Index) 
{
	id3frame = id3v2.findFrame(F_APIC, Index);
	return (id3frame != NULL) ? cAPIC(id3frame)->getMime().AllocSysString() : CTools::instance().GetEmptyBSTR();
}


/**
 * @brief get a picture from the tag and store it in the specified file
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param FileName Name of the file where the picture will be stored
 * @param Index index from 1 to picture frame count
 * @return normally -1, 0 on error or index not present
 */
extern "C" short __stdcall ID3V2GetPictureFileW(LPCWSTR FileName, short Index) 
{
	id3frame = id3v2.findFrame(F_APIC, Index);
	return (id3frame != NULL) ? b2s(cAPIC(id3frame)->getPictureFile(getValidPointer(FileName))) : b2s(false);
}

/**
 * @brief get a picture from the tag and copy it in a byte array
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param arr to a Byte array
 * @param maxLen maximum size of the byte array
 * @param Index index from 1 to picture frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetPictureArrayW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_APIC, Index);
	return (id3frame != NULL) ? cAPIC(id3frame)->getData(arr, maxLen) : -1;	
}

/**
 * @brief "APIC" store a picture from a file in the tag
 *
 * There may be more than one 'APIC' frame in each tag, but only one with the same content descriptor.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param FileName name of the picture file
 * @param Description a description of the picture
 * @param PictureType type from 0 to 20, see @ref picturetypes
 * @param asLink shows how the picture will be stored
 *
 * - -1 = only a link to the image will be stored
 * - 0 = the image data will be stored
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2AddPictureFileW(LPCWSTR FileName, LPCWSTR Description, short PictureType, short asLink)
{
	CID3F_APIC *pic = new CID3F_APIC((BYTE)PictureType, getValidPointer(Description));
	bool result = (asLink) ? pic->setFileLink(getValidPointer(FileName)) : pic->setPictureFile(getValidPointer(FileName));
	if (result)
		id3v2.replaceFrame(pic);
	else
		delete pic;
	return b2s(result);
}


/**
 * @brief "APIC" store a picture from a byte array in the tag
 *
 * There may be more than one 'APIC' frame in each tag, but only one with the same content descriptor.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   APIC
 * @param arr to a byte array with the picture data
 * @param Length the size of the array
 * @param Description a description of the picture
 * @param PictureType type from 0 to 20, see @ref picturetypes
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddPictureArrayW(BYTE *arr, u32 Length, LPCWSTR Description, short PictureType)
{
	CID3F_APIC *pic = new CID3F_APIC((BYTE)PictureType, getValidPointer(Description));
	pic->setData(arr, Length);	
	return b2s(id3v2.replaceFrame(pic));
}



// AENC
/**
 * @brief add an audio encryption frame
 *
 * There may be more than one 'AENC' frame in a tag, but only one with the same 'Owner identifier'.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   AENC
 * @param arr pointer to the byte array
 * @param maxLen of the array in bytes
 * @param URL owner identifier (an URL or email address)
 * @param PreviewStart start of the unencrypted preview, in MPEG frames
 * @param PreviewLength length of the unencrypted preview, in MPEG frames
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddAudioEncryptionW(BYTE *arr, u32 maxLen, LPCWSTR URL, short PreviewStart, short PreviewLength)
{
	CID3F_AENC *f = new CID3F_AENC(getValidPointer(URL), PreviewStart, PreviewLength);
	f->setData(arr, maxLen);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the encryption info
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   AENC
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to AudioEncryptionFrame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetAudioEncryptionDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_AENC, Index);
	return (id3frame != NULL) ? cAENC(id3frame)->getData(arr, maxLen) : -1;	
}

/**
 * @brief get the owner identifier (an URL or email address)
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   AENC
 * @param Index from 1 to AudioEncryptionFrame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetAudioEncryptionURLW(short Index) 
{
	id3frame = id3v2.findFrame(F_AENC, Index);
	return (id3frame != NULL) ? cAENC(id3frame)->getIdentifier().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the preview start frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   AENC
 * @param Index from 1 to AudioEncryptionFrame count
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetAudioEncryptionPreviewStartW(short Index) 
{
	id3frame = id3v2.findFrame(F_AENC, Index);
	return (short)((id3frame != NULL) ? cAENC(id3frame)->getStart() : -1);
}

/**
 * @brief get the preview length count
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   AENC
 * @param Index from 1 to AudioEncryptionFrame count
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetAudioEncryptionPreviewLengthW(short Index) 
{
	id3frame = id3v2.findFrame(F_AENC, Index);
	return (short)((id3frame != NULL) ? cAENC(id3frame)->getLength() : -1);
}

// ASPI
/**
 * @brief add an audio seek point
 *
 * There may only be one 'audio seek point index' frame in a tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ASPI
 * @param arr pointer to the byte array with the index points
 * @param maxLen of the array in bytes
 * @param start start of the indexed data (byte offset)
 * @param length length of the indexed data in bytes
 * @param numbers number of index points
 * @param BitsPerPoint bits per index point (8 or 16)
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddAudioSeekPointW(BYTE *arr, u32 maxLen, long start, long length, short numbers, BYTE BitsPerPoint)
{
	CID3F_ASPI *f = new CID3F_ASPI(start, length, numbers, BitsPerPoint);
	f->setData(arr, maxLen);
	return b2s(id3v2.replaceFrame(f));
}
/**
 * @brief get the start of the indexed data
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ASPI
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetAudioSeekPointStartW() 
{
	id3frame = id3v2.findFrame(F_ASPI);
	return (id3frame != NULL) ? cASPI(id3frame)->getStart() : -1;
}
/**
 * @brief get the length of the indexed data
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ASPI
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetAudioSeekPointLengthW() 
{
	id3frame = id3v2.findFrame(F_ASPI);
	return (id3frame != NULL) ? cASPI(id3frame)->getLength() : -1;
}
/**
 * @brief get the number of index points
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ASPI
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetAudioSeekPointNumberW() 
{
	id3frame = id3v2.findFrame(F_ASPI);
	return (id3frame != NULL) ? cASPI(id3frame)->getNumber() : -1;
}
/**
 * @brief get the bits per index point
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ASPI
 * @return value or 0 if the frame does not exist
 */
extern "C" long __stdcall ID3V2GetAudioSeekPointBitsPerIndexpointW() 
{
	id3frame = id3v2.findFrame(F_ASPI);
	return (id3frame != NULL) ? cASPI(id3frame)->getBpi() : 0;
}
/**
 * @brief get the data array with the index points
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ASPI
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetAudioSeekPointDataW(BYTE *arr, u32 maxLen)
{
	id3frame = id3v2.findFrame(F_ASPI);
	return (id3frame != NULL) ? cASPI(id3frame)->getData(arr, maxLen) : -1;	
}

// COMR
/**
 * @brief add a commercial frame
 *
 * There may be more than one 'COMR' in a tag, but no two may be identical.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param arr pointer to the byte array with the seller logo
 * @param maxLen the size of the array in bytes
 * @param Price price, e.g. "EUR9.99" (currency code followed by the amount)
 * @param validUntil in format YYYYMMDD (8 chars)
 * @param contactUrl URL for contacting the seller
 * @param receivedAs how the audio is delivered, 0 to 8 (see ID3V2GetCommercialFrameReceivedAsW)
 * @param seller name of the seller
 * @param description short description of the product
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddCommercialFrameW(BYTE *arr, u32 maxLen, LPCWSTR Price, LPCWSTR validUntil, LPCWSTR contactUrl , short receivedAs , LPCWSTR seller, LPCWSTR description)
{
	CID3F_COMR *f = new CID3F_COMR(getValidPointer(Price), getValidPointer(validUntil), getValidPointer(contactUrl), (BYTE)receivedAs, getValidPointer(seller), getValidPointer(description));
	f->setPicture(arr, maxLen);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the seller logo
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to Commercial Frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetCommercialFramePictureW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getData(arr, maxLen) : -1;	
}

/**
 * @brief get the price
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param Index from 1 to Commercial Frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetCommercialFramePriceW(short Index) 
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getPrice().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the valid until field
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param Index from 1 to Commercial Frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetCommercialFrameValidUntilW(short Index) 
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getValidUntil().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the contact url
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param Index from 1 to Commercial Frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetCommercialFrameContactURLW(short Index) 
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getContactUrl().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the seller name
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param Index from 1 to Commercial Frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetCommercialFrameSellerNameW(short Index) 
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getSellerName().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the description
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param Index from 1 to Commercial Frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetCommercialFrameDescriptionW(short Index) 
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the mime type of the logo
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param Index from 1 to Commercial Frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetCommercialFramePictureMimeW(short Index) 
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getPictureMime().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the "received as" field
 *
 * 0=Other, 1=Standard CD Album with other songs 2=Compressed audio on CD  3=File over the Internet  4=Stream over the Internet  5=As note sheets  6=As note sheets in a book with other sheets  7=Music on other media  8=Non-musical merchandise
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   COMR
 * @param Index from 1 to Commercial Frame count
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetCommercialFrameReceivedAsW(short Index) 
{
	id3frame = id3v2.findFrame(F_COMR, Index);
	return (id3frame != NULL) ? cCOMR(id3frame)->getReceivedAs() : -1;
}

// CTOC/CHAP
/**
 * @brief CTOC add a table of content
 *
 * There may be more than one frame of this type in a tag but each must have an Element ID that is unique with respect to any other "CTOC" or "CHAP" frame in the tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CTOC
 * @param ID the unique id of the element
 * @param Title the title of the element
 * @param Description the description of the element
 * @param isOrdered non-zero (e.g. -1) if the child elements are ordered, 0 if they are not ordered
 * @return -1 if the frame was replaced, 0 if it was added
 */
extern "C" short __stdcall ID3V2AddTableOfContentW(LPCWSTR ID, LPCWSTR Title, LPCWSTR Description, short isOrdered)
{
	// if no CTOC exists yet, set the ROOT flag to true
	CID3F_CTOC *f = new CID3F_CTOC(getValidPointer(ID), getValidPointer(Title), getValidPointer(Description), (isOrdered != 0));
	if (id3v2.findFrame(F_CTOC) == NULL)
		f->setRoot(true);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief CHAP add a chapter
 *
 * There may be more than one frame of this type in a tag but each must have an Element ID that is unique with respect to any other "CTOC" or "CHAP" frame in the tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CHAP
 * @param ID the unique id
 * @param Title the title
 * @param Description the description
 * @param startTime the start time in milliseconds
 * @param endTime the end time in milliseconds
 * @return -1 if the frame was replaced, 0 if it was added
 */
extern "C" short __stdcall ID3V2AddChapterW(LPCWSTR ID, LPCWSTR Title, LPCWSTR Description, u32 startTime, u32 endTime)
{
	CID3F_CHAP *f = new CID3F_CHAP(getValidPointer(ID), getValidPointer(Title), getValidPointer(Description));
	f->setTimes(startTime, endTime);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief add a child element to a parent element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ParentTocID the ID of the parent CTOC element
 * @param ChildID the ID of the child element (CHAP or CTOC)
 * @return normally -1, 0 if the parent was not found or is not a CTOC element
 */
extern "C" short __stdcall ID3V2AddChildElementW(LPCWSTR ParentTocID, LPCWSTR ChildID)
{
	id3frame = id3v2.findFrame(getValidPointer(ParentTocID));
	// findFrame(ID) also returns chapters (CHAP); only a table of contents (CTOC) has child elements
	if (id3frame == NULL || !cCHAPTER(id3frame)->isCTOC())
		return b2s(false);
	cCTOC(id3frame)->addChildElement(getValidPointer(ChildID));
	return b2s(true);
}

/**
 * @brief delete the child element and all references
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ParentTocID the parent ID
 * @param ChildID the ID to remove
 * @return normally -1, 0 if the parent was not found or is not a CTOC element
 */
extern "C" short __stdcall ID3V2DeleteChildElementW(LPCWSTR ParentTocID, LPCWSTR ChildID)
{
	id3frame = id3v2.findFrame(getValidPointer(ParentTocID));
	if (id3frame == NULL || !cCHAPTER(id3frame)->isCTOC())
		return b2s(false);
	return cCTOC(id3frame)->deleteChildElement(getValidPointer(ChildID));	
}

/**
 * @brief deletes a CTOC or CHAP Element and all of the child elements
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the element
 * @return normally -1, 0 if ID not found
 */
extern "C" short __stdcall ID3V2DeleteAddendumW(LPCWSTR ID)
{
	ATLTRACE(_T("deleting %s\n"), ID);
	CID3_Frame *delFrame = id3v2.findFrame(getValidPointer(ID));
	if (delFrame == NULL)
		return-b2s(false);
	CID3F_Chapter *chap = cCHAPTER(delFrame);
	// first delete all subframes
	chap->deleteAllFrames();
	// if it is a CTOC element: remove subordinate branches recursively
	if (chap->isCTOC())
	{
		CID3F_CTOC *toc = cCTOC(chap);
		for (u16 i = toc->getNumberOfEntries(); i > 0; i--)
		{
			ATLTRACE(_T(" recursively deleting %s\n"), toc->getChildElementID(i));
			ID3V2DeleteAddendumW(toc->getChildElementID(i));
		}
	}
	// delete references from the remaining TOCs
	size_t pos = 0;
	CID3_Frame *found;
	do
	{
		found = id3v2.findNextFrame(F_CTOC, pos);
		if (found != NULL)
			cCTOC(found)->deleteChildElement(ID);

	} while (found != NULL);
	// finally delete the element itself
	id3v2.deleteAllFrames(getValidPointer(ID));
	return b2s(true);
}

/**
 * @brief get the title of a CTOC or CHAP Element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the element
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetAddendumTitleW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return CTools::instance().GetEmptyBSTR();
	return cCHAPTER(id3frame)->getTitle().AllocSysString();
}

/**
 * @brief set the title of a CHAP or CTOC element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the element
 * @param newTitle the new title
 * @return normally -1, 0 if ID not found
 */
extern "C" short __stdcall ID3V2SetAddendumTitleW(LPCWSTR ID, LPCWSTR newTitle)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	cCHAPTER(id3frame)->setTitle(getValidPointer(newTitle));
	return b2s(true);
}

/**
 * @brief set the times of a CHAP element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CHAP
 * @param ID the ID of the element
 * @param startTime the start time in milliseconds
 * @param endTime the end time in milliseconds
 * @return normally -1, 0 if ID not found or element is not a CHAP element
 */
extern "C" short __stdcall ID3V2SetChapterTimesW(LPCWSTR ID, u32 startTime, u32 endTime)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	if (cCHAPTER(id3frame)->isCHAP())
	{
		cCHAP(id3frame)->setTimes(startTime, endTime);
		return b2s(true);
	}
	return b2s(false);
}

/**
 * @brief get the description of a CHAP or CTOC element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the element
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetAddendumDescriptionW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return CTools::instance().GetEmptyBSTR();
	return cCHAPTER(id3frame)->getDescription().AllocSysString();
}
/**
 * @brief set the description of a CTOC or CHAP element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the id of the element
 * @param newDescription the new description
 * @return normally -1, 0 if ID not found
 */
extern "C" short __stdcall ID3V2SetAddendumDescriptionW(LPCWSTR ID, LPCWSTR newDescription)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	cCHAPTER(id3frame)->setDescription(getValidPointer(newDescription));
	return b2s(true);
}

/**
 * @brief get the type of the element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the element
 * @return 0=element is a CHAP element, 1=element is a CTOC element, -1=ID not found
 */
extern "C" short __stdcall ID3V2GetAddendumTypeW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return -1;
	return (cCHAPTER(id3frame)->isCTOC()) ? 1 : 0;
}

/**
 * @brief get a comma-separated list of all unique frame ids
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @return a comma-separated text string with Frame IDs
 */
extern "C" BSTR __stdcall ID3V2GetAllFrameIDsW()
{
	return id3v2.getAllFrameIDs().AllocSysString();
}

/**
 * @brief get the start time of a CHAP element in milliseconds
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CHAP
 * @param ID the ID of the Chapter
 * @return time in milliseconds
 */
extern "C" u32 __stdcall ID3V2GetChapterStartTimeW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return 0;
	return (cCHAPTER(id3frame)->isCHAP()) ? cCHAP(id3frame)->getStartTime() : 0;
}

/**
 * @brief get the end time of a CHAP element in milliseconds
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CHAP
 * @param ID the ID of the Chapter
 * @return time in milliseconds
 */
extern "C" u32 __stdcall ID3V2GetChapterEndTimeW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return 0;
	return (cCHAPTER(id3frame)->isCHAP()) ? cCHAP(id3frame)->getEndTime() : 0;
}

/**
 * @brief get the sort order of a CTOC element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CTOC
 * @param ID the ID of the CTOC element
 * @return -1 if the child elements are ordered, otherwise 0 (also if the ID was not found or is not a CTOC element)
 */
extern "C" short __stdcall ID3V2GetTOCIsOrderedW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return 0;
	return (cCHAPTER(id3frame)->isCTOC()) ? b2s(cCTOC(id3frame)->isOrdered()) : 0;
} 
/**
 * @brief set the sort order of a CTOC element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CTOC
 * @param ID the ID of the CTOC element
 * @param status non-zero (e.g. -1) if the child elements are ordered, 0 if they are not ordered
 * @return normally -1, 0 if ID not found
 */
extern "C" short __stdcall ID3V2SetTOCIsOrderedW(LPCWSTR ID, short status)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	if (cCHAPTER(id3frame)->isCTOC())
		cCTOC(id3frame)->setOrdered(status != 0);
	return b2s(true);
} 


/**
 * @brief get the count of subframes from an ID
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the Parent frame
 * @return subframe count or -1 if error
 */
extern "C" short __stdcall ID3V2GetSubFramesW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return -1;
	return cCHAPTER(id3frame)->getFrameCount();
}
/**
 * @brief get the ID of a subframe from a parent frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param Index the index from 1 to ID3V2GetSubFramesW
 * @return Frame ID or 0 if parent frame not found
 */
extern "C" u32 __stdcall ID3V2GetSubFrameIDW(LPCWSTR ID, short Index)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return 0;
	return cCHAPTER(id3frame)->getFrameID(Index);
}

/**
 * @brief get the type of a subframe from a parent frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param Index the index from 1 to ID3V2GetSubFramesW
 * @return 1=normal text frame T000, 2=user-defined text frame TXXX, 3=normal URL frame W000, 4=user-defined URL frame WXXX, 5=Picture APIC, -1=ID not found
 */
extern "C" short __stdcall ID3V2GetSubFrameTypeW(LPCWSTR ID, short Index)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return -1;
	return cCHAPTER(id3frame)->getFrameTyp(Index);
}

/**
 * @brief get the text of a subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param Index the index from 1 to ID3V2GetSubFramesW
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetSubFrameTextW(LPCWSTR ID, short Index)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return CTools::instance().GetEmptyBSTR();
	return cCHAPTER(id3frame)->getFrameText(Index).AllocSysString();
}

/**
 * @brief get the description of a subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param Index the index from 1 to ID3V2GetSubFramesW
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetSubFrameDescriptionW(LPCWSTR ID, short Index)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return CTools::instance().GetEmptyBSTR();
	return cCHAPTER(id3frame)->getFrameDescription(Index).AllocSysString();
}

/**
 * @brief get a byte array from a picture subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param arr pointer to a byte array
 * @param maxLen maximum size of the array in bytes
 * @param ID the ID of the parent frame
 * @param Index the index from 1 to ID3V2GetSubFramesW
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetSubFrameImageW(BYTE *arr, u32 maxLen, LPCWSTR ID, short Index)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return -1;
	CID3F_Chapter *chap = cCHAPTER(id3frame);
	if (chap->getFrameTyp(Index) != 5) // element is not an APIC frame
		return -1;
	return cAPIC(chap->getFrame(Index))->getData(arr, maxLen);	
}

/**
 * @brief get the picture type of picture subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param Index the index from 1 to ID3V2GetSubFramesW
 * @return picture type from 0 to 20, see @ref picturetypes; -2 if the subframe is not a picture, 0 if the parent frame was not found
 */
extern "C" short __stdcall ID3V2GetSubFrameImageTypeW(LPCWSTR ID, short Index)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	CID3F_Chapter *chap = cCHAPTER(id3frame);
	if (chap->getFrameTyp(Index) != 5) // element is not an APIC frame
		return -2;
	return cAPIC(chap->getFrame(Index))->getPictureType();	
}

/**
 * @brief get a free ID for a CTOC frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CTOC
 * @return the free ID
 */
extern "C" BSTR __stdcall ID3V2GetPossibleTOCIDW()
{
	size_t index = 0;
	int lfdNr = 0;
	CAtlString tmp, defaultString;
	defaultString = _T("toc");
	tmp.Format(_T("%s%i"), (LPCTSTR)defaultString, lfdNr);
	do 
	{
		id3frame = id3v2.findNextFrame(F_CTOC, index);
		if (id3frame != NULL && cCHAPTER(id3frame)->getID().CompareNoCase(tmp) == 0)
		{
			index = 0;
			tmp.Format(_T("%s%i"), (LPCTSTR)defaultString, ++lfdNr);
		}
	} while (id3frame != NULL);
	return tmp.AllocSysString();
}

/**
 * @brief get a free ID for a CHAP frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CHAP
 * @return the free ID
 */
extern "C" BSTR __stdcall ID3V2GetPossibleCHAPIDW()
{
	size_t index = 0;
	int lfdNr = 0;
	CAtlString tmp, defaultString;
	defaultString = _T("ch");
	tmp.Format(_T("%s%i"), (LPCTSTR)defaultString, lfdNr);
	do 
	{
		id3frame = id3v2.findNextFrame(F_CHAP, index);
		if (id3frame != NULL && cCHAPTER(id3frame)->getID().CompareNoCase(tmp) == 0)
		{
			index = 0;
			tmp.Format(_T("%s%i"), (LPCTSTR)defaultString, ++lfdNr);
		}
	} while (id3frame != NULL);
	return tmp.AllocSysString();
}

/**
 * @brief get the ID of the root CTOC element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   CTOC
 * @return the ID of the root CTOC element
 */
extern "C" BSTR __stdcall ID3V2GetTOCRootIDW()
{
	size_t pos = 0;
	do
	{
		id3frame = id3v2.findNextFrame(F_CTOC, pos);		

	} while (id3frame != NULL && !cCTOC(id3frame)->isRoot());
	return (id3frame != NULL) ? cCTOC(id3frame)->getID().AllocSysString() : CTools::instance().GetEmptyBSTR(); 
}
/**
 * @brief get the count of the child elements from a CTOC element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the parent frame ID
 * @return count of the elements, -1 if the ID was not found or is not a CTOC element
 */
extern "C" short __stdcall ID3V2GetChildElementsW(LPCWSTR ID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return -1;
	CID3F_Chapter *chap = cCHAPTER(id3frame);
	return (chap->isCTOC()) ? (cCTOC(chap))->getNumberOfEntries() : -1;
}
/**
 * @brief get the ID of a child element
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param Index from 1 to ID3V2GetChildElementsW
 * @return the ID of the element
 */
extern "C" BSTR __stdcall ID3V2GetChildElementIDW(LPCWSTR ID, short Index)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return CTools::instance().GetEmptyBSTR();
	CID3F_Chapter *chap = cCHAPTER(id3frame);
	return (chap->isCTOC()) ? (cCTOC(chap))->getChildElementID(Index).AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief set a text subframe; an empty text removes the subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param FrameID the ID of the new frame
 * @param textString the new text
 * @param description the new description
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2SetTextSubFrameW(LPCWSTR ID, u32 FrameID, LPCWSTR textString, LPCWSTR description)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	CID3F_Chapter *chap = cCHAPTER(id3frame);
	if (textString == NULL || wcslen(textString) == 0)
		chap->deleteFrame(FrameID);
	else
	{
		if (FrameID == F_TXXX)
			chap->addFrame(new CID3F_TXXX(getValidPointer(description), getValidPointer(textString)));
		else
			chap->addFrame(new CID3F_T000(FrameID, getValidPointer(textString)));
	}
	return b2s(true);
}

/**
 * @brief set a URL subframe; an empty URL removes the subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param FrameID the ID of the new frame
 * @param urlString the new url
 * @param description the new description
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2SetURLSubFrameW(LPCWSTR ID, u32 FrameID, LPCWSTR urlString, LPCWSTR description)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	CID3F_Chapter *chap = cCHAPTER(id3frame);
	if (urlString == NULL || wcslen(urlString) == 0)
		chap->deleteFrame(FrameID);
	else
	{
		if (FrameID == F_WXXX)
			chap->addFrame(new CID3F_WXXX(getValidPointer(description), getValidPointer(urlString)));
		else
			chap->addFrame(new CID3F_W000(FrameID, getValidPointer(urlString)));
	}
	return b2s(true);
}

/**
 * @brief add a picture subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param arr to a byte array with picture data
 * @param maxLen maximum size of the array in bytes
 * @param ID the ID of the parent frame
 * @param Description the description of the image
 * @param PictureType type from 0 to 20, see @ref picturetypes
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2SetImageSubFrameW(BYTE *arr, u32 maxLen, LPCWSTR ID, LPCWSTR Description, short PictureType)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	CID3F_APIC *pic = new CID3F_APIC((BYTE)PictureType, getValidPointer(Description));
	pic->setData(arr, maxLen);
	cCHAPTER(id3frame)->addFrame(pic);
	return b2s(true);
}

/**
 * @brief deletes a subframe
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param ID the ID of the parent frame
 * @param FrameID the ID of the frame to remove
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2DeleteSubFrameW(LPCWSTR ID, u32 FrameID)
{
	id3frame = id3v2.findFrame(getValidPointer(ID));
	if (id3frame == NULL)
		return b2s(false);
	cCHAPTER(id3frame)->deleteFrame(FrameID);
	return b2s(true);
}

// ENCR
/**
 * @brief ENCR add an encryption frame
 * There may be more than one 'ENCR' frame in each tag, but only one with the same symbol and owner identifier.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ENCR
 * @param arr pointer to the byte array with the encryption data
 * @param maxLen maximum size of the array in bytes
 * @param URL owner identifier
 * @param Symbol Method symbol
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddEncryptionW(BYTE *arr, u32 maxLen, LPCWSTR URL, short Symbol)
{
	CID3F_ENCR *f = new CID3F_ENCR(getValidPointer(URL), (BYTE)Symbol);
	f->setData(arr, maxLen);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the Method Symbol
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ENCR
 * @param Index from 1 to encryption frame count
 * @return Symbol or -1 if not found
 */
extern "C" short __stdcall ID3V2GetEncryptionSymbolW(short Index) 
{
	id3frame = id3v2.findFrame(F_ENCR, Index);
	return (id3frame != NULL) ? cENCR(id3frame)->getSymbol() : -1;
}

/**
 * @brief get the encryption url
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ENCR
 * @param Index from 1 to encryption frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetEncryptionURLW(short Index) 
{
	id3frame = id3v2.findFrame(F_ENCR, Index);
	return (id3frame != NULL) ? cENCR(id3frame)->getOwner().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the encryption data
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ENCR
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to encryption frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetEncryptionDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_ENCR, Index);
	return (id3frame != NULL) ? cENCR(id3frame)->getData(arr, maxLen) : -1;	
}
// EQUA
/**
 * @brief EQUA add an equalization frame
 *
 * There may be more than one 'EQUA' frame in each tag, but only one with the same identification string.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   EQUA
 * @param arr pointer to the byte array with the equalization data
 * @param maxLen of the array in bytes
 * @param Interpolationmethod 0=Band (no interpolation) 1=Linear
 * @param Identification identification string of the equalization
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddEqualisationW(BYTE *arr, u32 maxLen, BYTE Interpolationmethod, LPCWSTR Identification)
{
	CID3F_EQUA *f = new CID3F_EQUA(Interpolationmethod, getValidPointer(Identification));
	f->setData(arr, maxLen);
	return b2s(id3v2.replaceFrame(f));
}
/**
 * @brief get the interpolation value 0=Band (no interpolation) 1=Linear
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   EQUA
 * @param Index from 1 to Equalisation frame count
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetEqualisationInterpolationW(short Index) 
{
	id3frame = id3v2.findFrame(F_EQUA, Index);
	return (id3frame != NULL) ? cEQUA(id3frame)->getFormat() : -1;
}
/**
 * @brief get the number of bits used for representation of the adjustment
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   EQUA
 * @param Index from 1 to Equalisation frame count
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetEqualisationAdjustmentBitsW(short Index) 
{
	id3frame = id3v2.findFrame(F_EQUA, Index);
	return (id3frame != NULL) ? cEQUA(id3frame)->getAdjustmentBits() : -1;
}
/**
 * @brief get the identifier
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   EQUA
 * @param Index from 1 to Equalisation frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetEqualisationIdentificationW(short Index) 
{
	id3frame = id3v2.findFrame(F_EQUA, Index);
	return (id3frame != NULL) ? cEQUA(id3frame)->getIdentification().AllocSysString() : CTools::instance().GetEmptyBSTR();
}
/**
 * @brief get the data array with the equalization points
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   EQUA
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to Equalisation frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetEqualisationDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_EQUA, Index);
	return (id3frame != NULL) ? cEQUA(id3frame)->getData(arr, maxLen) : -1;	
}

// ETCO
/**
 * @brief ETCO add an event timing code frame
 *
 * There may only be one 'ETCO' frame in each tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ETCO
 * @param arr pointer to the byte array with the event codes
 * @param maxLen of the array in bytes with the event codes
 * @param TimestampFormat 1=frames as unit  2=milliseconds as unit
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddEventTimingCodesW(BYTE *arr, u32 maxLen, BYTE TimestampFormat)
{
	CID3F_ETCO *f = new CID3F_ETCO(TimestampFormat);
	f->setData(arr, maxLen);
	return b2s(id3v2.replaceFrame(f));
}
/**
 * @brief get the timestamp format  1=frames as unit  2=milliseconds as unit
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ETCO
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetEventTimingCodesTimestampFormatW() 
{
	id3frame = id3v2.findFrame(F_ETCO);
	return (id3frame != NULL) ? cETCO(id3frame)->getFormat() :-1;
}
/**
 * @brief get the event data
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   ETCO
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetEventTimingCodesDataW(BYTE *arr, u32 maxLen)
{
	id3frame = id3v2.findFrame(F_ETCO);
	return (id3frame != NULL) ? cETCO(id3frame)->getData(arr, maxLen) : -1;	
}


// GEOB
/**
 * @brief GEOB add a general encapsulated object
 *
 * There may be more than one 'GEOB' frame in each tag, but only one with the same content descriptor.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GEOB
 * @param arr pointer to the byte array with the object data
 * @param maxLen of the array in bytes
 * @param Mime Type of the object
 * @param FileName file name of the object
 * @param Description the descriptor
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddGeneralObjectW(BYTE *arr, u32 maxLen, LPCWSTR Mime, LPCWSTR FileName, LPCWSTR Description)
{
	CID3F_GEOB *f = new CID3F_GEOB(getValidPointer(Mime), getValidPointer(FileName), getValidPointer(Description));
	f->setData(arr, maxLen);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the mime type of the object
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GEOB
 * @param Index from 1 to GeneralObject frames
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetGeneralObjectMimeW(short Index) 
{
	id3frame = id3v2.findFrame(F_GEOB, Index);
	return (id3frame != NULL) ? cGEOB(id3frame)->getMime().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the filename
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GEOB
 * @param Index from 1 to GeneralObject frames
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetGeneralObjectFilenameW(short Index) 
{
	id3frame = id3v2.findFrame(F_GEOB, Index);
	return (id3frame != NULL) ? cGEOB(id3frame)->getFilename().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the description of the general object
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GEOB
 * @param Index from 1 to GeneralObject frames
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetGeneralObjectDescriptionW(short Index) 
{
	id3frame = id3v2.findFrame(F_GEOB, Index);
	return (id3frame != NULL) ? cGEOB(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the object data
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GEOB
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to GeneralObject frames
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetGeneralObjectDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_GEOB, Index);
	return (id3frame != NULL) ? cGEOB(id3frame)->getData(arr, maxLen) : -1;	
}
// GRID
/**
 * @brief add a GRID frame
 *
 * There may be more than one 'GRID' frame in each tag, but only one with the same owner identifier and group symbol.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GRID
 * @param arr pointer to the byte array
 * @param length size of the array in bytes
 * @param Url owner identifier
 * @param symbol symbol in the range 128 - 240
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddGroupIdentificationW(BYTE *arr, u32 length, LPCWSTR Url, BYTE symbol)
{
	CID3F_GRID *f = new CID3F_GRID(getValidPointer(Url), symbol);
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the group identification url
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GRID
 * @param Index from 1 to GroupIdentification frames
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetGroupIdentificationURLW(short Index) 
{
	id3frame = id3v2.findFrame(F_GRID, Index);
	return (id3frame != NULL) ? cGRID(id3frame)->getURL().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the group identifiation symbol
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GRID
 * @param Index from 1 to GroupIdentification frames
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetGroupIdentificationSymbolW(short Index) 
{
	id3frame = id3v2.findFrame(F_GRID, Index);
	return (id3frame != NULL) ? cGRID(id3frame)->getSymbol() : -1;
}

/**
 * @brief get the data of the group identification frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   GRID
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to GroupIdentification frames
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetGroupIdentificationDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_GRID, Index);
	return (id3frame != NULL) ? cGRID(id3frame)->getData(arr, maxLen) : -1;	
}
// LINK
/**
 * @brief LINK add a LINK frame
 *
 * There may be more than one 'LINK' frame in each tag, but only one with the same contents.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   LINK
 * @param FrameIdentifier the ID of the linked frame
 * @param URL the url to the file with the frame
 * @param additionalData additional Data
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddLinkedInformationW(long FrameIdentifier, LPCWSTR URL, LPCWSTR additionalData)
{
	return b2s(id3v2.replaceFrame(new CID3F_LINK(FrameIdentifier, getValidPointer(URL), getValidPointer(additionalData))));	
}
/**
 * @brief get the frame id of the linked frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   LINK
 * @param Index from 1 to LinkedInformation frame count
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetLinkedInformationFrameIdentifierW(short Index) 
{
	id3frame = id3v2.findFrame(F_LINK, Index);
	return (id3frame != NULL) ? cLINK(id3frame)->getIdentifier() : -1;
}
/**
 * @brief get the url to the linked frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   LINK
 * @param Index from 1 to LinkedInformation frame count
 * @return the text string with the url
 */
extern "C" BSTR __stdcall ID3V2GetLinkedInformationURLW(short Index) 
{
	id3frame = id3v2.findFrame(F_LINK, Index);
	return (id3frame != NULL) ? cLINK(id3frame)->getURL().AllocSysString() : CTools::instance().GetEmptyBSTR();
}
/**
 * @brief get the additional data of the linked frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   LINK
 * @param Index from 1 to LinkedInformation frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetLinkedInformationAdditionalDataW(short Index) 
{
	id3frame = id3v2.findFrame(F_LINK, Index);
	return (id3frame != NULL) ? cLINK(id3frame)->getAdditional().AllocSysString() : CTools::instance().GetEmptyBSTR();
}
// MCDI
/**
 * @brief MCDI add a Music CD Identifier frame
 *
 * There may only be one 'MCDI' frame in each tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MCDI
 * @param arr pointer to the byte array with the CD TOC
 * @param length of the array
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddMusicCdIdentifierW(BYTE *arr, u32 length)
{
	CID3F_MCDI *f = new CID3F_MCDI();
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}
/**
 * @brief get the CD TOC data
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MCDI
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetMusicCdIdentifierDataW(BYTE *arr, u32 maxLen)
{
	id3frame = id3v2.findFrame(F_MCDI);
	return (id3frame != NULL) ? cMCDI(id3frame)->getData(arr, maxLen) : -1;	
}
// MLLT
/**
 * @brief MLLT add a Mpeg Location Lookuptable frame
 *
 * There may only be one 'MLLT' frame in each tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MLLT
 * @param arr pointer to the byte array with the deviation in bytes and milliseconds
 * @param length of the array
 * @param Frames mpeg frames between reference
 * @param Bytes Bytes between reference
 * @param Milliseconds milliseconds between reference
 * @param BytesDeviation bits for bytes deviation
 * @param MillisecondsDeviation bits for milliseconds deviation
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddMpegLocationLookupTableW(BYTE *arr, u32 length, long Frames, long Bytes, long Milliseconds, BYTE BytesDeviation, BYTE MillisecondsDeviation)
{
	CID3F_MLLT *f = new CID3F_MLLT(Frames, Bytes, Milliseconds, BytesDeviation, MillisecondsDeviation);
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}
/**
 * @brief get the data of an MLLT Frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MLLT
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetMpegLocationLookupTableDataW(BYTE *arr, u32 maxLen)
{
	id3frame = id3v2.findFrame(F_MLLT);
	return (id3frame != NULL) ? cMLLT(id3frame)->getData(arr, maxLen) : -1;	
}
/**
 * @brief get the mpeg frames
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MLLT
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetMpegLocationLookupTableFramesW()
{
	id3frame = id3v2.findFrame(F_MLLT);
	return (id3frame != NULL) ? cMLLT(id3frame)->getFrames() : -1;	
}
/**
 * @brief get the bytes between reference
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MLLT
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetMpegLocationLookupTableBytesW()
{
	id3frame = id3v2.findFrame(F_MLLT);
	return (id3frame != NULL) ? cMLLT(id3frame)->getBytes() : -1;	
}
/**
 * @brief get the milliseconds between reference
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MLLT
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetMpegLocationLookupTableMillisecondsW()
{
	id3frame = id3v2.findFrame(F_MLLT);
	return (id3frame != NULL) ? cMLLT(id3frame)->getMillis() : -1;	
}
/**
 * @brief get the bits for byte deviation
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MLLT
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetMpegLocationLookupTableBytesDeviationW()
{
	id3frame = id3v2.findFrame(F_MLLT);
	return (id3frame != NULL) ? cMLLT(id3frame)->getBdev() : -1;	
}
/**
 * @brief get the bits for milliseconds deviation
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   MLLT
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetMpegLocationLookupTableMillisecondsDeviationW()
{
	id3frame = id3v2.findFrame(F_MLLT);
	return (id3frame != NULL) ? cMLLT(id3frame)->getMdev() : -1;	
}

// OWNE
/**
 * @brief OWNE add an Ownership frame
 *
 * There may be only one 'OWNE' frame in each tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   OWNE
 * @param Price the price paid
 * @param dateString date of purchase
 * @param Seller the seller
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddOwnershipW(LPCWSTR Price, LPCWSTR dateString, LPCWSTR Seller)
{
	return b2s(id3v2.replaceFrame(new CID3F_OWNE(getValidPointer(Price), getValidPointer(dateString), getValidPointer(Seller))));	
}

/**
 * @brief get the price paid
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   OWNE
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetOwnershipPriceW() 
{
	id3frame = id3v2.findFrame(F_OWNE);
	return (id3frame != NULL) ? cOWNE(id3frame)->getPrice().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the date of purchase
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   OWNE
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetOwnershipDateW() 
{
	id3frame = id3v2.findFrame(F_OWNE);
	return (id3frame != NULL) ? cOWNE(id3frame)->getDate().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the seller
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   OWNE
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetOwnershipSellerW() 
{
	id3frame = id3v2.findFrame(F_OWNE);
	return (id3frame != NULL) ? cOWNE(id3frame)->getSeller().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

// POSS
/**
 * @brief add a position synchron frame
 *
 * There may be only one 'POSS' frame in each tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   POSS
 * @param TimestampFormat 1=mpeg frames as unit  2=milliseconds as unit
 * @param Position the position where the listener starts to receive
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddPositionSynchronisationW(BYTE TimestampFormat, long Position)
{
	return b2s(id3v2.replaceFrame(new CID3F_POSS(TimestampFormat, Position)));	
}
/**
 * @brief get the timestamp format, 1=mpeg frames as unit  2=milliseconds as unit
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   POSS
 * @return value or -1 if error
 */
extern "C" short __stdcall ID3V2GetPositionSynchronisationTimestampFormatW()
{
	id3frame = id3v2.findFrame(F_POSS);
	return (id3frame != NULL) ? cPOSS(id3frame)->getFormat() : -1;	
}
/**
 * @brief get the position
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   POSS
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetPositionSynchronisationValueW()
{
	id3frame = id3v2.findFrame(F_POSS);
	return (id3frame != NULL) ? cPOSS(id3frame)->getPosition() : -1;	
}

// PRIV
/**
 * @brief add a private frame
 *
 * There may be more than one 'PRIV' frame in each tag, but only one with the same owner identifier.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   PRIV
 * @param arr pointer to the byte array
 * @param length size of array in bytes
 * @param URL owner identifier
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddPrivateFrameW(BYTE *arr, u32 length, LPCWSTR URL)
{
	CID3F_PRIV *f = new CID3F_PRIV(getValidPointer(URL));
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the owner identifier
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   PRIV
 * @param Index from 1 to Private Frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetPrivateFrameURLW(short Index) 
{
	id3frame = id3v2.findFrame(F_PRIV, Index);
	return (id3frame != NULL) ? cPRIV(id3frame)->getURL().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the data of the private frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   PRIV
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to Private Frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetPrivateFrameDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_PRIV, Index);
	return (id3frame != NULL) ? cPRIV(id3frame)->getData(arr, maxLen) : -1;	
}

// RBUF
/**
 * @brief add a recommended buffer frame
 *
 * There may be only one 'RBUF' frame in each tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RBUF
 * @param BufferSize the buffer size
 * @param EmbeddedInfoFlag 0=false 1=true
 * @param Offset offset to next tag
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddRecommendedBufferSizeW(long BufferSize, BYTE EmbeddedInfoFlag, long Offset)
{
	return b2s(id3v2.replaceFrame(new CID3F_RBUF(BufferSize, EmbeddedInfoFlag, Offset )));
}
/**
 * @brief get the buffer size
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RBUF
 * @return buffer size or -1 on error
 */
extern "C" long __stdcall ID3V2GetRecommendedBufferSizeValueW()
{
	id3frame = id3v2.findFrame(F_RBUF);
	return (id3frame != NULL) ? cRBUF(id3frame)->getBufferSize() : -1;
}
/**
 * @brief get the embedded info flag 0=false 1=true
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RBUF
 * @return 0=false 1=true, -1 on error
 */
extern "C" short __stdcall ID3V2GetRecommendedBufferSizeFlagW()
{
	id3frame = id3v2.findFrame(F_RBUF);
	return (id3frame != NULL) ? cRBUF(id3frame)->getFlag() : -1;
}
/**
 * @brief get the offset to next tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RBUF
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetRecommendedBufferSizeOffsetW()
{
	id3frame = id3v2.findFrame(F_RBUF);
	return (id3frame != NULL) ? cRBUF(id3frame)->getOffset() : -1;
}
// RVAD
/**
 * @brief add a relative volume adjustment frame
 *
 * There may be more than one 'RVAD' frame in each tag, but only one with the same identification string.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVAD
 * @param arr pointer to the byte array
 * @param length the size of the array in bytes
 * @param Identifier the unique identifier
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddRelativeVolumeAdjustmentW(BYTE *arr, u32 length, LPCWSTR Identifier)
{
	CID3F_RVAD *f = new CID3F_RVAD(getValidPointer(Identifier));
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the data from the relative volume adjustment frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVAD
 * @param arr pointer to the byte array
 * @param maxLen the size of the array in bytes
 * @param Index from 1 to relative volume adjustment frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetRelativeVolumeAdjustmentDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_RVAD, Index);
	return (id3frame != NULL) ? cRVAD(id3frame)->getData(arr, maxLen) : -1;
}

/**
 * @brief get the identifier of the relative volume adjustment frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVAD
 * @param Index from 1 to relative volume adjustment frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetRelativeVolumeAdjustmentIdentifierW(short Index) 
{
	id3frame = id3v2.findFrame(F_RVAD, Index);
	return (id3frame != NULL) ? cRVAD(id3frame)->getIdentification().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

// RVRB
/**
 * @brief add a reverb frame
 *
 * There may be only one 'RVRB' frame in each tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @param reverbLeft reverb left in milliseconds
 * @param reverbRight reverb right in milliseconds
 * @param bouncesLeft reverb bounces left
 * @param bouncesRight reverb bounces right
 * @param feedbackLeftToLeft reverb feedback left to left
 * @param feedbackLeftToRight reverb feedback left to right
 * @param feedbackRightToRight reverb feedback right to right
 * @param feedbackRightToLeft reverb feedback right to left
 * @param premixLeftToRight premix left to right
 * @param premixRightToLeft premix right to left
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddReverbW(short reverbLeft, short reverbRight, BYTE bouncesLeft, BYTE bouncesRight, BYTE feedbackLeftToLeft, BYTE feedbackLeftToRight, BYTE feedbackRightToRight, BYTE feedbackRightToLeft, BYTE premixLeftToRight, BYTE premixRightToLeft)
{
	return b2s(id3v2.replaceFrame(new CID3F_RVRB(reverbLeft, reverbRight, bouncesLeft, bouncesRight, feedbackLeftToLeft, feedbackLeftToRight, feedbackRightToRight, feedbackRightToLeft,  premixLeftToRight, premixRightToLeft)));
}
/**
 * @brief get the value for reverb left
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb left in ms or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbLeftW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(0) : -1;
}
/**
 * @brief get the value for reverb right
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb right in ms or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbRightW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(1) : -1;
}
/**
 * @brief get the value for reverb bounces left
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb bounces left or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbBouncesLeftW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(2) : -1;
}
/**
 * @brief get the value for reverb bounces right
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb bounces right or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbBouncesRightW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(3) : -1;
}
/**
 * @brief get the value for reverb feedback left to left
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb feedback left to left or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbFeedbackLeftToLeftW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(4) : -1;
}
/**
 * @brief get the value for reverb feedback left to right
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb feedback left to right or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbFeedbackLeftToRightW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(5) : -1;
}
/**
 * @brief get the value for reverb feedback right to right
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb feedback right to right or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbFeedbackRightToRightW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(6) : -1;
}
/**
 * @brief get the value for reverb feedback right to left
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return reverb feedback right to left or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbFeedbackRightToLeftW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(7) : -1;
}
/**
 * @brief get the value for premix left to right
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return premix left to right or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbPremixLeftToRightW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(8) : -1;
}
/**
 * @brief get the value for premix right to left
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   RVRB
 * @return premix right to left or -1 if error
 */
extern "C" short __stdcall ID3V2GetReverbPremixRightToLeftW()
{
	id3frame = id3v2.findFrame(F_RVRB);
	return (id3frame != NULL) ? cRVRB(id3frame)->getDatas(9) : -1;
}

// SEEK
/**
 * @brief add a seek frame
 *
 * There may only be one 'SEEK' frame in a tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SEEK
 * @param offset the offset to next tag
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddSeekOffsetW(long offset)
{
	return b2s(id3v2.replaceFrame(new CID3F_SEEK(offset)));
}

/**
 * @brief get the offset to next tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SEEK
 * @return value or -1 if error
 */
extern "C" long __stdcall ID3V2GetSeekOffsetW()
{
	id3frame = id3v2.findFrame(F_SEEK);
	return (id3frame != NULL) ? cSEEK(id3frame)->getOffset() : -1;
}
// SIGN
/**
 * @brief add a signature frame
 *
 * There may be more than one 'SIGN' frame in a tag, but no two with the same group symbol
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SIGN
 * @param arr pointer to the byte array
 * @param length of the array in bytes
 * @param GroupSymbol symbol
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddSignatureFrameW(BYTE *arr, u32 length, short GroupSymbol)
{
	CID3F_SIGN *f = new CID3F_SIGN((BYTE)GroupSymbol);
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the data of a signature frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SIGN
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to signature frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetSignatureFrameDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_SIGN, Index);
	return (id3frame != NULL) ? cSIGN(id3frame)->getData(arr, maxLen) : -1;	
}

/**
 * @brief get the symbol of a signature frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SIGN
 * @param Index from 1 to signature frame count
 * @return Value or -1 if error
 */
extern "C" short __stdcall ID3V2GetSignatureFrameGroupSymbolW(short Index)
{
	id3frame = id3v2.findFrame(F_SIGN, Index);
	return (id3frame != NULL) ? cSIGN(id3frame)->getSymbol() : -1;	
}

// SYTC
/**
 * @brief set or replace a synchronized Tempo frame
 *
 * There may only be one 'SYTC' frame in each tag.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYTC
 * @param arr pointer to the byte array
 * @param length of array in bytes
 * @param Format Timestamp format 1=frames as unit  2=milliseconds as unit
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddSynchronizedTempoW(BYTE *arr, u32 length, short Format)
{
	CID3F_SYTC *f = new CID3F_SYTC((BYTE)Format);
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}

/**
 * @brief get the data of a Synchronized tempo frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYTC
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetSynchronizedTempoDataW(BYTE *arr, u32 maxLen)
{
	id3frame = id3v2.findFrame(F_SYTC);
	return (id3frame != NULL) ? cSYTC(id3frame)->getData(arr, maxLen) : -1;	
}

/**
 * @brief get the timestamp format
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYTC
 * @return 1=frames as unit  2=milliseconds as unit  -1=error
 */
extern "C" short __stdcall ID3V2GetSynchronizedTempoFormatW()
{
	id3frame = id3v2.findFrame(F_SYTC);
	return (id3frame != NULL) ? cSYTC(id3frame)->getFormat() : -1;	
}

// UFID
/**
 * @brief set or replace a unique file identifier frame
 *
 * There may be more than one 'UFID' frame in a tag, but only one with the same Owner identifier.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   UFID
 * @param arr pointer to the byte array
 * @param length the size of the array
 * @param Owner owner identifier
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddUniqueFileIdentifierW(BYTE *arr, u32 length, LPCWSTR Owner)
{
	CID3F_UFID *f = new CID3F_UFID(getValidPointer(Owner));
	f->setData(arr, length);
	return b2s(id3v2.replaceFrame(f));
}
/**
 * @brief get the owner of the frame identifier
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   UFID
 * @param Index from 1 to Unique File Identifier frame count
 * @return the text string
 */
extern "C" BSTR __stdcall ID3V2GetUniqueFileIdentifierOwnerW(short Index)
{
	id3frame = id3v2.findFrame(F_UFID, Index);
	return (id3frame != NULL) ? cUFID(id3frame)->getOwner().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief get the data of the unique file identifier frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   UFID
 * @param arr pointer to the byte array
 * @param maxLen maximum size of the array in bytes
 * @param Index from 1 to Unique File Identifier frame count
 * @return the real length of the array in bytes or -1 if error
 */
extern "C" long __stdcall ID3V2GetUniqueFileIdentifierDataW(BYTE *arr, u32 maxLen, short Index)
{
	id3frame = id3v2.findFrame(F_UFID, Index);
	return (id3frame != NULL) ? cUFID(id3frame)->getData(arr, maxLen) : -1;	
}


/**
 * @brief "USER" set a user-defined frame
 *
 * There may be more than one 'USER' frame in a tag, but only one with the same Language.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   USER
 * @param Language ID of the entry, e.g. ENG for English (must be 3 bytes long)
 * @param Text the user frame
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddUserFrameW(LPCWSTR Language, LPCWSTR Text)
{
	return b2s(id3v2.replaceFrame(new CID3F_USER(getValidPointer(Text), getValidPointer(Language))));	
}


/**
 * @brief get the language from a user-defined frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   USER
 * @param Index index from 1 to user frame count
 * @return the language id of the user frame, e.g. ENG for English
 */
extern "C" BSTR __stdcall ID3V2GetUserFrameLanguageW(short Index)
{
	id3frame = id3v2.findFrame(F_USER, Index);
	return (id3frame != NULL) ? cUSER(id3frame)->getLanguage().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}


/**
 * @brief get a user-defined Frame
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   USER
 * @param Index index from 1 to user frame count
 * @return the user-defined text
 */
extern "C" BSTR __stdcall ID3V2GetUserFrameW(short Index)
{
	id3frame = id3v2.findFrame(F_USER, Index);
	return (id3frame != NULL) ? cUSER(id3frame)->getText().AllocSysString() : CTools::instance().GetEmptyBSTR();
}

/**
 * @brief "TXXX" set a user-defined Text
 *
 * There may be more than one 'TXXX' frame in each tag, but only one with the same description.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   TXXX
 * @param Description the description of the entry
 * @param Text the new text
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddUserTextW(LPCWSTR Description, LPCWSTR Text)
{
	return b2s(id3v2.replaceFrame(new CID3F_TXXX(getValidPointer(Description), getValidPointer(Text))));	
}



/**
 * @brief get a Description from a user-defined Text
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   TXXX
 * @param Index index from 1 to user text frame count
 * @return the description of the user text
 */
extern "C" BSTR __stdcall ID3V2GetUserTextDescriptionW(short Index)
{
	id3frame = id3v2.findFrame(F_TXXX, Index);
	return (id3frame != NULL) ? cTXXX(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}


/**
 * @brief get the user-defined text
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   TXXX
 * @param Index index from 1 to user text frame count
 * @return the user-defined text
 */
extern "C" BSTR __stdcall ID3V2GetUserTextW(short Index)
{
	id3frame = id3v2.findFrame(F_TXXX, Index);
	return (id3frame != NULL) ? cTXXX(id3frame)->getText().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}


/**
 * @brief "WXXX" set a user-defined url
 *
 * There may be more than one 'WXXX' frame in each tag, but only one with the same description.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   WXXX
 * @param Description the description of the entry
 * @param URL the new url
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddUserURLW(LPCWSTR Description, LPCWSTR URL)
{
	return b2s(id3v2.replaceFrame(new CID3F_WXXX(getValidPointer(Description), getValidPointer(URL))));	
}


/**
 * @brief get the description from a user-defined url
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   WXXX
 * @param Index index from 1 to user url frame count
 * @return the description of the user-defined url
 */
extern "C" BSTR __stdcall ID3V2GetUserURLDescriptionW(short Index)
{
	id3frame = id3v2.findFrame(F_WXXX, Index);
	return (id3frame != NULL) ? cWXXX(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}


/**
 * @brief get a user-defined url
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   WXXX
 * @param Index index from 1 to user URL frame count
 * @return the user-defined url
 */
extern "C" BSTR __stdcall ID3V2GetUserURLW(short Index)
{
	id3frame = id3v2.findFrame(F_WXXX, Index);
	return (id3frame != NULL) ? cWXXX(id3frame)->getURL().AllocSysString() : CTools::instance().GetEmptyBSTR();	

}


/**
 * @brief remove the ID3v2 tag from a file. Attention: This function removes the tag immediately!
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return -1 if removed, 0 on error
 */
extern "C" short __stdcall ID3V2RemoveTagFromFileW(LPCWSTR FileName)
{
	if (id3v2.RemoveFromFile(getValidPointer(FileName)))
	{
		AUDIOAnalyzeFileW(getValidPointer(FileName));
		return b2s(true);
	}
	return b2s(false);	
}
/**
 * @brief remove the ID3v2 tag from the last analyzed file. Attention: This function removes the tag immediately!
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @return -1 if removed, 0 on error
 */
extern "C" short __stdcall ID3V2RemoveTagW()
{
	if (id3v2.RemoveFromFile(lastFile))
	{
		return b2s(true);	
	}
	return b2s(false);  
}


/**
 * @brief store the ID3v2 tag in a file
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2SaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);	
	if (GetFormat(FileName) != AUDIO_FORMAT_INVALID)
		return b2s(id3v2.SaveToFile(FileName));
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false);   
}

/**
 * @brief store the ID3v2 tag in the last analyzed file
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall ID3V2SaveChangesW()
{
	return ID3V2SaveChangesToFileW(lastFile);
}

/**
 * @brief get the text from a synchronized lyrics
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYLT
 * @param Index index from 1 to sync lyric frame count
 * @return the text value
 */
extern "C" BSTR __stdcall ID3V2GetSyncLyricW(short Index) 
{
	id3frame = id3v2.findFrame(F_SYLT, Index);
	return (id3frame != NULL) ? cSYLT(id3frame)->getText().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}

/**
 * @brief get the description from a synchronized lyric
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYLT
 * @param Index index from 1 to sync lyric frame count
 * @return the description of the synchronized lyric
 */
extern "C" BSTR __stdcall ID3V2GetSyncLyricDescriptionW(short Index) 
{
	id3frame = id3v2.findFrame(F_SYLT, Index);
	return (id3frame != NULL) ? cSYLT(id3frame)->getDescription().AllocSysString() : CTools::instance().GetEmptyBSTR();	  
}

/**
 * @brief get the language from a synchronized lyrics
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYLT
 * @param Index index from 1 to sync lyric frame count
 * @return the language id of the synchronized lyric, e.g. ENG for English
 */
extern "C" BSTR __stdcall ID3V2GetSyncLyricLanguageW(short Index) 
{
	id3frame = id3v2.findFrame(F_SYLT, Index);
	return (id3frame != NULL) ? cSYLT(id3frame)->getLanguage().AllocSysString() : CTools::instance().GetEmptyBSTR();	 
}

/**
 * @brief get the time stamp format from a synchronized lyrics
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYLT
 * @param Index index from 1 to sync lyric frame count
 * @return return value with the following meaning:
 * @retval 0 entry not found
 * @retval 1 mpeg frames as unit
 * @retval 2 milliseconds as unit
 */
extern "C" short __stdcall ID3V2GetSyncLyricTimeFormatW(short Index) 
{
	id3frame = id3v2.findFrame(F_SYLT, Index);
	return (id3frame != NULL) ? cSYLT(id3frame)->getTimestampFormat() : 0;	
}

/**
 * @brief get the content type from a synchronized lyrics
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYLT
 * @param Index index from 1 to sync lyrics frame count
 * @return return value with the following meaning:
 * @retval 0 other (also returned if the entry was not found)
 * @retval 1 lyrics
 * @retval 2 text transcription
 * @retval 3 movement/part name
 * @retval 4 events
 * @retval 5 chord
 * @retval 6 trivia/'pop up' information
 * @retval 7 URLs to webpages
 * @retval 8 URL to images
 */
extern "C" short __stdcall ID3V2GetSyncLyricContentTypeW(short Index) 
{
	id3frame = id3v2.findFrame(F_SYLT, Index);
	return (id3frame != NULL) ? cSYLT(id3frame)->getContentType() : 0;		
}


/**
 * @brief "SYLT" set a synchronized lyric
 *
 * There may be more than one 'SYLT' frame in each tag, but only one with the same language and content descriptor.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   SYLT
 * @param Language language of the entry, e.g. ENG for English (must be 3 bytes long)
 * @param Description the description of the entry
 * @param Text multi entries separated with line break (crlf). use the following format:
 *
 * @verbatim
 * [xxxxxxxx]entry1
 * [xxxxxxxx]entry2 etc.
 * @endverbatim
 *
 * xxxxxxxx = formatted absolute time stamp 8 digits
 * entry = any text entry
 *
 * Example:
 * @verbatim
 * [00000010]Strang
 * [00000020]ers
 * [00000080]in
 * @endverbatim
 * @param ContentType set the content type. Possible values are:
 *
 * - 0 = other
 * - 1 = lyrics
 * - 2 = text transcription
 * - 3 = movement/part name
 * - 4 = events
 * - 5 = chord
 * - 6 = trivia/'pop up' information
 * - 7 = URLs to webpages
 * - 8 = URL to images
 * @param TimeStampFormat set the timestamp format units
 *
 * - 1 = using mpeg frames as unit
 * - 2 = using milliseconds as unit
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddSyncLyricW(LPCWSTR Language, LPCWSTR Description, LPCWSTR Text, short ContentType, short TimeStampFormat)
{
	return b2s(id3v2.replaceFrame(new CID3F_SYLT(Language, (BYTE)TimeStampFormat, (BYTE)ContentType,getValidPointer(Description), getValidPointer(Text))));
}


/**
 * @brief "POPM" set a popularimeter
 *
 * There may be more than one 'POPM' frame in each tag, but only one with the same email address.
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   POPM
 * @param Email a personal email address
 * @param Rating rating from 1 (worst) to 255 (best), 0=no rating
 * @param Counter play counter
 * @return -1 if frame was replaced, 0 if frame was added
 */
extern "C" short __stdcall ID3V2AddPopularimeterW(LPCWSTR Email, short Rating, long Counter) 
{
	return b2s(id3v2.replaceFrame(new CID3F_POPM(getValidPointer(Email), (BYTE)Rating, Counter)));
}

/**
 * @brief get the email from a popularimeter tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   POPM
 * @param Index index from 1 to popularimeter frame count
 * @return email address
 */
extern "C" BSTR __stdcall ID3V2GetPopularimeterEmailW(short Index) 
{
	id3frame = id3v2.findFrame(F_POPM, Index);
	return (id3frame != NULL) ? cPOPM(id3frame)->getEmail().AllocSysString() : CTools::instance().GetEmptyBSTR();	
}

/**
 * @brief get the rating from a popularimeter tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   POPM
 * @param Index from 1 to popularimeter frame count
 * @return rating from 1 (worst) to 255 (best) or 0 if not present
 */
extern "C" short __stdcall ID3V2GetPopularimeterRatingW(short Index) 
{
	id3frame = id3v2.findFrame(F_POPM, Index);
	return (id3frame != NULL) ? cPOPM(id3frame)->getRating() : 0;	
}

/**
 * @brief get the counter from a popularimeter tag
 *
 * @ingroup ID3V2
 * @since 2.0.1.0
 * @par ID3v2 frame
 *   POPM
 * @param Index from 1 to popularimeter frame count
 * @return counter or -1 if not present
 */
extern "C" long __stdcall ID3V2GetPopularimeterCounterW(short Index) 
{
	id3frame = id3v2.findFrame(F_POPM, Index);
	return (id3frame != NULL) ? cPOPM(id3frame)->getCounter() : -1;	

}

/**
 * @brief set a file name for audiogenie logging
 *
 * @ingroup UNIVERSAL
 * @since 2.0.1.0
 * @param fileName name of the logging file
 */
extern "C" void __stdcall SetLogFileW(LPCWSTR fileName)
{
	CTools::instance().setLogFile(getValidPointer(fileName));
}

/**
 * @brief get a wav text frame (info chunk)
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @return the text string
 */
extern "C" BSTR __stdcall WAVGetTextFrameW(u32 FrameID)
{
	return wav.getTextFrame(FrameID).AllocSysString();
}

/**
 * @brief set a wav text frame (info chunk)
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @param FrameID the ID of the frame
 * @param textString the new text
 */
extern "C" void __stdcall WAVSetTextFrameW(u32 FrameID, LPCWSTR textString)
{
	wav.setTextFrame(FrameID, getValidPointer(textString));	
}

/**
 * @brief get the text of a display chunk
 *
 * @ingroup WAV
 * @since 2.0.3.0
 * @return the text string or empty if not present
 */
extern "C" BSTR __stdcall WAVGetDisplayTextW()
{
	return wav.getDisplayText().AllocSysString();
}

/**
 * @brief set the text of a DISPLAY chunk
 *
 * @ingroup WAV
 * @since 2.0.3.0
 * @param textString the new text
 */
extern "C" void __stdcall WAVSetDisplayTextW(LPCWSTR textString)
{
	wav.setDisplayText(getValidPointer(textString));	
}

/**
 * @brief store the WAV tag in a file
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall WAVSaveChangesToFileW(LPCWSTR FileName)
{
	FileName = getValidPointer(FileName);	
	if (GetFormat(FileName) != AUDIO_FORMAT_INVALID)
	{
		bool result = wav.SaveToFile(FileName);
		if (result && !lastFile.IsEmpty() && lastFile.CompareNoCase(FileName) == 0)
			AUDIOAnalyzeFileW(FileName);
		return b2s(result);
	}
	CTools::instance().setLastError(ERR_TAG_NOT_ALLOWED);
	return b2s(false);   
}

/**
 * @brief stores the WAV tag into the last analyzed file
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @return normally -1, 0 on error
 */
extern "C" short __stdcall WAVSaveChangesW()
{
	return WAVSaveChangesToFileW(lastFile);
}


/**
 * @brief get a wav cart chunk entry
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @param Index the index of the entry from 0 to 26
 * @return the text string
 */
extern "C" BSTR __stdcall WAVGetCartChunkEntryW(short Index)
{
	return wav.getCartText((BYTE)Index).AllocSysString();
}

/**
 * @brief set or replace a cartchunk entry
 *
 * @ingroup WAV
 * @since 2.0.1.0
 * @param Index the index of the entry from 0 to 26
 * @param textString the new text
 */
extern "C" void __stdcall WAVSetCartChunkEntryW(short Index, LPCWSTR textString)
{
	wav.setCartText((BYTE)Index, getValidPointer(textString));
}

/* ----------------------------------------------------------------------------------------- */

/**
 * @brief get an MD5 hash only from the audio data and without tag information from the last analyzed file
 *
 * You can use this for comparing audio files.
 *
 * The file is read completely from the first to the last audio byte. The calculation itself runs at about 0.7 GB/s on a
 * current desktop CPU (a 5 MB song takes about 7 ms); on a slow disk or a network drive the transfer rate is the limit.
 * The hash is calculated with every call, keep the result if you need it more than once.
 *
 * @ingroup AUDIO
 * @since 2.0.1.0
 * @return MD5 hash as 32 hexadecimal characters (empty if no file was analyzed)
 */
extern "C" BSTR __stdcall AUDIOGetMD5ValueW() 
{	
	if (Format != AUDIO_FORMAT_UNKNOWN)
	{
		md5Tool.calcHashFromFile(lastFile, audio->GetFirstAudioPosition(), audio->GetLastAudioPosition());
		return md5Tool.getHashValue().AllocSysString();	
	}
	return CTools::instance().GetEmptyBSTR();	
}

/**
 * @brief get an MD5 hash from the complete file
 *
 * You can use this for comparing files.
 *
 * The calculation itself runs at about 0.7 GB/s on a current desktop CPU; on a slow disk or a network drive the transfer
 * rate is the limit.
 *
 * @ingroup UNIVERSAL
 * @since 2.0.1.0
 * @param FileName name of the file
 * @return MD5 hash as 32 hexadecimal characters
 */
extern "C" BSTR __stdcall GetMD5ValueFromFileW(LPCWSTR FileName) 
{	
	md5Tool.calcHashFromFile(getValidPointer(FileName), 0, 0);
	return md5Tool.getHashValue().AllocSysString();	
}
