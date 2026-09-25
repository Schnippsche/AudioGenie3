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
#include "WMA_File.h"
#include "share.h"
#include "io.h"

CWMA_File::CWMA_File(void)
{	
	_valid = false;
}

CWMA_File::~CWMA_File(void)
{
	ResetData();
}

bool CWMA_File::ReadFromFile(FILE *Stream)
{
	ResetData();
	/* Read file data */
	/* Check for existing header */
	_fseeki64(Stream, CTools::ID3v2Size, SEEK_SET);
	if (header.load(Stream, toSizeClamped(CTools::FileSize)))
	{
		_valid = true;
		DataPosition = _ftelli64(Stream); 
		return true;
	}
	_valid = false;
	return false;
}

CAtlString CWMA_File::GetUserItem(CAtlString key)
{
	// look in the normal content first, then in ExtContent
	CWMA_Object *obj = header.findObject(WMA_CONTENT_DESCRIPTION_ID);
	if (obj != NULL)
	{
		CWMA_ContentDescription *cd = static_cast<CWMA_ContentDescription*>(obj);
		// standard fields are stored in the ContentDescription object
		// An empty value in the standard object does not count as a hit: some encoders (e.g. ffmpeg) put fields
		// like "Description" in the ExtContentDescription object instead.
		CAtlString value;
		if (key.CompareNoCase(WM_TITLE) == 0 || key.CompareNoCase(WM_TITLE2) == 0)
			value = cd->GetTitle();
		else if (key.CompareNoCase(WM_AUTHOR) == 0 || key.CompareNoCase(WM_AUTHOR2) == 0)
			value = cd->GetAuthor();
		else if (key.CompareNoCase(WM_DESCRIPTION) == 0 || key.CompareNoCase(WM_DESCRIPTION2) == 0)
			value = cd->GetComment();
		else if (key.CompareNoCase(WM_PROVIDERRATING) == 0 || key.CompareNoCase(WM_PROVIDERRATING2) == 0)
			value = cd->GetRating();
		else if (key.CompareNoCase(WM_COPYRIGHT) == 0 || key.CompareNoCase(WM_COPYRIGHT2) == 0)
			value = cd->GetCopyright();
		if (!value.IsEmpty())
			return value;
	}

	// now look in the ExtContentDescription object, if present
	int pos = CWMA_ObjectFactory::instance().FindField(key);
	// description: can be stored under "WM/Description" as well as under "Description"
	if (pos == -1 && key.CompareNoCase(WM_DESCRIPTION) == 0)
		pos = CWMA_ObjectFactory::instance().FindField(WM_DESCRIPTION2);
	if (pos == -1 && key.CompareNoCase(WM_DESCRIPTION2) == 0)
		pos = CWMA_ObjectFactory::instance().FindField(WM_DESCRIPTION);
	if (pos != -1)
		return CWMA_ObjectFactory::instance().GetValue(pos);
	// nothing found
	return EMPTY;
}

void CWMA_File::SetUserItem(LPCWSTR key, LPCWSTR item)
{
	// where to write, standard or ExtContent?	
	CAtlString tmp(key);
	int art = 0;
	if (tmp.CompareNoCase(WM_TITLE) == 0 || tmp.CompareNoCase(WM_TITLE2) == 0)
		art = 1;
	if (tmp.CompareNoCase(WM_AUTHOR) == 0 || tmp.CompareNoCase(WM_AUTHOR2) == 0)
		art = 2;
	if (tmp.CompareNoCase(WM_DESCRIPTION) == 0 || tmp.CompareNoCase(WM_DESCRIPTION2) == 0)
		art = 3;
	if (tmp.CompareNoCase(WM_PROVIDERRATING) == 0 || tmp.CompareNoCase(WM_PROVIDERRATING2) == 0)
		art = 4;
	if (tmp.CompareNoCase(WM_COPYRIGHT) == 0 || tmp.CompareNoCase(WM_COPYRIGHT2) == 0)
		art = 5;

	if (art > 0)
	{
		// write values into ContentDescription
		CWMA_Object *obj = header.findObject(WMA_CONTENT_DESCRIPTION_ID);
		CWMA_ContentDescription *cd;
		if (obj == NULL)
		{
			obj = CWMA_ObjectFactory::instance().createObject(WMA_CONTENT_DESCRIPTION_ID);
			header.insertObject(obj, 0);
		}
		cd = static_cast<CWMA_ContentDescription*>(obj);
		switch (art)
		{
		case 1:
			return cd->SetTitle(item);
		case 2:
			return cd->SetAuthor(item);
		case 3:
			return cd->SetComment(item);
		case 4:
			return cd->SetRating(item);
		case 5:
			return cd->SetCopyright(item);
		}
	}
	// write into ExtContentDescription
	CWMA_ObjectFactory::instance().SetValue(key, item);

}

bool CWMA_File::SaveToFile(LPCWSTR FileName)
{
	FILE* Stream;
	if ( (Stream = _wfsopen(FileName, READ_AND_WRITE, _SH_DENYWR)) == NULL)
	{
		CTools::instance().setLastError(errno);
		return false;
	}

	_fseeki64(Stream, CTools::ID3v2Size, SEEK_SET);
	u32 oldHeaderSize = (u32)header.loadHeaderOnly(Stream);
	if (oldHeaderSize == 0) 
	{
		fclose(Stream);
		return false;
	}
	long DataPosition = CTools::ID3v2Size + oldHeaderSize;
	CWMA_Object *obj = NULL;
	// save the current objects, if present
	CWMA_MetadataLibrary *newMetas = NULL;
	CWMA_ContentDescription *newContent = NULL;
	CWMA_ExtContentDescription *newExtContent = NULL;
	if ((obj = header.findObject(WMA_CONTENT_DESCRIPTION_ID)) != NULL)
	{
		if (obj->getData()->GetLength() > 0)
			newContent = new CWMA_ContentDescription(obj->getData());
	}
	if ((obj = header.findObject(WMA_EXTENDED_CONTENT_DESCRIPTION_ID)) != NULL)
	{
		if (obj->getData()->GetLength() > 0)
			newExtContent = new CWMA_ExtContentDescription(obj->getData());
	}
	else
	{
		// what if ExtContent does not exist, but new entries were added?
		newExtContent = new CWMA_ExtContentDescription();
		newExtContent->buildData();
		if (newExtContent->getData()->GetLength() == 0)
		{
			// nothing new was there
			delete newExtContent;
			newExtContent = NULL;
		}
	}
	if ((obj = header.findObject(WMA_HEADER_EXTENSION_ID)) != NULL)
	{
		CWMA_Header_Extension* he = static_cast<CWMA_Header_Extension*>(obj);
		if ((obj = he->findObject(WMA_METADATA_LIBRARY_ID)) != NULL)
		{
			// is there anything in it at all?
			if (obj->getData()->GetLength() > 0)
				newMetas = new CWMA_MetadataLibrary(obj->getData());

		}
		else
		{
			// what if metadata does not exist, but new entries were added?
			newMetas = new CWMA_MetadataLibrary();
			newMetas->buildData();
			if (newMetas->getData()->GetLength() == 0)
			{
				delete newMetas;
				newMetas = NULL;				
			}
		}
	}

	ResetData();
	_fseeki64(Stream, CTools::ID3v2Size, SEEK_SET);
	header.load(Stream, toSizeClamped(CTools::FileSize));
	// throw out all paddings	
	size_t oldPadding = 0;
	while ((obj = header.findObject(WMA_PADDING_ID)) != NULL)
	{
		oldPadding += obj->getDataSize();
		header.deleteObject(WMA_PADDING_ID);
	}
	obj = header.findObject(WMA_HEADER_EXTENSION_ID);
	if (obj != NULL)
	{
		CWMA_Header_Extension* he = static_cast<CWMA_Header_Extension*>(obj);
		while ((obj = he->findObject(WMA_PADDING_ID)) != NULL)
		{
			oldPadding += obj->getDataSize();
			he->deleteObject(WMA_PADDING_ID);
		}
		if (newMetas != NULL)
		{
			he->replaceObject(newMetas);
		}
		else
			he->deleteObject(WMA_METADATA_LIBRARY_ID);
	}
	// replace new elements
	if (newContent != NULL)
		header.replaceObject(newContent);
	if (newExtContent != NULL)
		header.replaceObject(newExtContent);

	size_t blockSize = CTools::configValues[CONFIG_ID3V2WRITEBLOCKSIZE];
	CBlob newHeader(blockSize);
	header.save(&newHeader);
	CWMA_Padding *newPadding = NULL;
	long paddingBlockSize = CTools::configValues[CONFIG_WMAPADDINGSIZE];
	if (newHeader.GetLength() + 26 > oldHeaderSize) // the padding object needs 24 bytes for itself and at least 1 byte of padding
	{   
		ATLTRACE(L"rebuild file...\n");
		// Rebuild
		// padding wanted?
		if (blockSize > 0)
		{
			newPadding = new CWMA_Padding(paddingBlockSize);
			header.replaceObject(newPadding);
		}
		else
			header.deleteObject(WMA_PADDING_ID);

		newHeader.Clear();
		header.save(&newHeader);
		FILE *Destination;
		CAtlString NewFileName(FileName);
		NewFileName+=TILDE;
		if ( (Destination = _wfsopen(NewFileName, WRITE_ONLY, _SH_DENYWR)) != NULL)
		{
			newHeader.FileWrite(newHeader.GetLength(), Destination);
			// Copy the rest of the File in Blocks
			_fseeki64(Stream, DataPosition, SEEK_SET);
			CBlob tmp(blockSize);
			do
			{
				ATLTRACE(_T("Write WMAData Block\n"));
				tmp.FileRead(blockSize, Stream);
				tmp.FileWrite(tmp.GetLength(), Destination);
			} while (tmp.GetLength() == blockSize);
			if (!CTools::finishRewrite(Stream, Destination, NewFileName, FileName))
				return false;
		}
		else
		{
			CTools::instance().setLastError(errno);
		}
	}
	else
	{
		ATLTRACE(L"rewrite file...\n");
		// insert suitable padding
		u32 newPaddingSize = oldHeaderSize - newHeader.GetLength() - 24;
		if (newPaddingSize > 0)
		{
			newPadding = new CWMA_Padding(oldHeaderSize - newHeader.GetLength() - 24);
			header.replaceObject(newPadding);
		}
		newHeader.Clear();
		header.save(&newHeader);
		_fseeki64(Stream, CTools::ID3v2Size, SEEK_SET);	
		newHeader.FileWrite(newHeader.GetLength(), Stream);
		_flushall();
		fclose(Stream);	
	}
	return true;
}

bool CWMA_File::DeletePictureFrame(short index)
{
	int found = CWMA_ObjectFactory::instance().FindPicture(index);
	if (found != -1)
	{
		CWMA_ObjectFactory::instance().DeleteField(found);
		return true;
	}
	return false;
}

void CWMA_File::DeletePictures()
{	
	for (int index = (int)CWMA_ObjectFactory::instance().tagdatas.GetCount() - 1; index >= 0; index--)
	{
		if (CWMA_ObjectFactory::instance().tagdatas.GetAt(index)->IsPicture() )
			CWMA_ObjectFactory::instance().DeleteField(index);
	}
}

int CWMA_File::GetPictures()
{
	int count = 0;
	size_t counts = CWMA_ObjectFactory::instance().tagdatas.GetCount();
	for (size_t i = 0; i < counts; i++)
	{	
		if (CWMA_ObjectFactory::instance().tagdatas.GetAt(i)->IsPicture() )
			count++;
	}
	return count;
}

CAtlString CWMA_File::GetPictureDescription(int index)
{
	TagPictureStruct *tps = getPictureTag(index);
	return (tps == NULL) ? EMPTY : tps->Description;
}

TagPictureStruct* CWMA_File::getPictureTag(int index)
{
	int result = -1;
	result = CWMA_ObjectFactory::instance().FindPicture(index);
	if (result != -1)
	{
		CBlob *dt = CWMA_ObjectFactory::instance().tagdatas[result]->getData();
		return CWMA_ObjectFactory::instance().ExtractPicture(dt);			
	}	
	return NULL;
}

int CWMA_File::GetPictureType(int index)
{
	TagPictureStruct *tps = getPictureTag(index);
	return (tps == NULL) ? 0 : tps->Type;
}

long CWMA_File::GetPictureSize(int index)
{
	TagPictureStruct *tps = getPictureTag(index);
	return (tps == NULL) ? 0 : tps->PicSize;
}

bool CWMA_File::GetPicture(int index, LPCWSTR fileName)
{
	TagPictureStruct *tps = getPictureTag(index);
	if (tps == NULL)
		return false;
	FILE *Stream;
	ATLTRACE(_T("open %s WRITE_ONLY\n"), fileName);
	if ( (Stream = _wfsopen(fileName, WRITE_ONLY, _SH_DENYNO)) != NULL)
	{
		fwrite(tps->PicDaten, 1, tps->PicSize, Stream);
		fflush(Stream);
		fclose(Stream);
		return true;
	}
	CTools::instance().setLastError(errno);
	return false;
}

size_t CWMA_File::GetPictureArray(BYTE *arr, u32 maxLen, short index)
{
	TagPictureStruct *tps = getPictureTag(index);
	if (tps == NULL)
		return 0;
	if (tps->PicSize > maxLen)
	{
		CTools::instance().setLastError(ERR_NOT_ENOUGH_MEMORY);
		return 0;
	}
	memcpy(arr, tps->PicDaten, tps->PicSize);
	return tps->PicSize;	
}
CAtlString CWMA_File::GetPictureMime(int index)
{
	TagPictureStruct *tps = getPictureTag(index);
	return (tps == NULL) ? EMPTY : tps->Mime;
}

CAtlString CWMA_File::GetItemKeys()
{
	return CWMA_ObjectFactory::instance().GetItemKeys();
}

bool CWMA_File::SetPicture(LPCWSTR FileName, LPCWSTR Description, short Index, short PictureType)
{
	if (FileName == NULL || wcslen(FileName) == 0)
	{
		CTools::instance().setLastError(errno);
		return false;
	}
	FILE *Stream;
	long ln = 0;
	if ( (Stream = _wfsopen(FileName, READ_ONLY, _SH_DENYNO)) != NULL)
	{
		ln = _filelength(_fileno(Stream));
		CBlob tmp(ln);
		tmp.FileRead(ln, Stream);
		fclose(Stream);

		return SetPictureArray(tmp.m_pData, ln, Description, Index, PictureType);
	}
	CTools::instance().setLastError(errno);
	return false;
}
bool CWMA_File::SetPictureArray(BYTE *arr, u32 len, LPCWSTR Description, short index, short picType)
{
	if (index < 1)
	{
		CTools::instance().setLastError(ERR_ID_OUT_OF_RANGE);
		return false;
	}
	if (len < 4)
	{
		CTools::instance().setLastError(ERR_PICTUREARRAY_TOO_SMALL);
		return false;
	}
	// create a new picture if smaller than 65500 in ExtContentDescription, otherwise in MetadataLibrary
	CWMA_TagData *item;
	if (len < 65500)
		item = new CWMA_TagData(EXTCONTENT_ART);
	else
		item = new CWMA_TagData(METADATA_ART);
	item->setNewPicture(arr, len, Description, (BYTE)picType);

	int it = CWMA_ObjectFactory::instance().FindPicture(index);
	if (it != -1) // replace existing picture
	{
		delete CWMA_ObjectFactory::instance().tagdatas.GetAt(it);
		CWMA_ObjectFactory::instance().tagdatas.SetAt(it, item);
	}
	else
	{
		CWMA_ObjectFactory::instance().tagdatas.Add(item);
	}
	return true;
}

void CWMA_File::ResetData()
{
	header.Remove();
	DataPosition = 0;	
}

bool CWMA_File::IsVBR()
{
	int pos = CWMA_ObjectFactory::instance().FindField(_T("IsVBR"));
	if (pos != -1)
		return (CWMA_ObjectFactory::instance().GetValue(pos).Compare(_T("true")) == 0);
	return false;
}

long CWMA_File::GetBitRate()
{
	CWMA_Object *obj = header.findObject(WMA_FILE_PROPERTIES_ID);
	if (obj == NULL)
		return 0;
	CWMA_FileProperties *fd = static_cast<CWMA_FileProperties*>(obj);
	return fd->getBitRate();
}

long CWMA_File::GetChannels()
{
	CWMA_Object *obj = header.findObject(WMA_STREAM_PROPERTIES_ID);
	if (obj == NULL)
		return 0;
	CWMA_StreamProperties *sp = static_cast<CWMA_StreamProperties*>(obj);
	return sp->getNumberOfChannels();	
}

float CWMA_File::GetDuration()
{
	CWMA_Object *obj = header.findObject(WMA_FILE_PROPERTIES_ID);
	if (obj == NULL)
		return 0;
	CWMA_FileProperties *fd = static_cast<CWMA_FileProperties*>(obj);
	return fd->getDuration();	
}

long CWMA_File::GetSampleRate()
{ 
	CWMA_Object *obj = header.findObject(WMA_STREAM_PROPERTIES_ID);
	if (obj == NULL)
		return 0;
	CWMA_StreamProperties *sp = static_cast<CWMA_StreamProperties*>(obj);
	return sp->getSamplesPerSecond();
}

bool CWMA_File::IsValid()
{
	return _valid;
}
