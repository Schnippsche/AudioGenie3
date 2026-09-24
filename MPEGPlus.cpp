/* AudioGenie is a Library for analyzing and tagging audio files.
   Copyright (C) 2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011
   Free Software Foundation, Inc.
   This file is part of the AudioGenie Library.
   Contributed by Stefan Toengi.

   The AudioGenie Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see <http://www.gnu.org/licenses/> 
*/

#include "stdafx.h"
#include "MPEGPlus.h"
#include "io.h"
#include <fcntl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMPEGPlus::CMPEGPlus()
{
  FIsSV8 = false;
  FChannels = 2;
  FSamples = 0;
  FBeginSilence = 0;
}

CMPEGPlus::~CMPEGPlus()
{
}

/* -------------------------------------------------------------------------- */

bool CMPEGPlus::ReadHeader(FILE *Stream)
{
  FIsSV8 = false;
  _fseeki64(Stream, CTools::ID3v2Size, SEEK_SET);
  /* Read header and get file size */
  memset(Header.ByteArray, 0, sizeof(Header.ByteArray));
  fread(Header.ByteArray, 1, 12, Stream);
  /* if transfer is not complete */
  if (memcmp(Header.ByteArray, "MP+", 3) == 0)
  {
    memcpy(&Header.IntegerArray, &Header.ByteArray, 12);
    return true;
  }
  if (memcmp(Header.ByteArray, "MPCK", 4) == 0)
    return ReadHeaderSV8(Stream);
  return false;
}

/* -------------------------------------------------------------------------- */

// variable-length integer from a buffer: 7 bits per byte (high bit = another byte follows), most significant group first.
// returns false at the end of the buffer or on overflow (more than 9 bytes).
static bool ReadVarInt(const BYTE *data, size_t length, size_t &pos, unsigned __int64 &value)
{
  value = 0;
  for (int i = 0; i < 9; i++)
  {
    if (pos >= length)
      return false;
    BYTE b = data[pos++];
    value = (value << 7) | (b & 0x7F);
    if ((b & 0x80) == 0)
      return true;
  }
  return false;
}

// Stream version 8: after "MPCK" packets follow: 2 byte key, length as variable-length integer (including key
// and length field itself), then the payload. "SH" (stream header) contains sample count, sample rate and channels, "EI" (encoder
// info) the profile. As soon as audio data ("AP") or the end ("SE") begins, the header is finished.
bool CMPEGPlus::ReadHeaderSV8(FILE *Stream)
{
  static const long SAMPLE_RATES[4] = { 44100, 48000, 37800, 32000 };
  const size_t MAX_PACKET_PAYLOAD = 4096;   // header packets are tiny; larger ones are skipped
  bool haveSH = false, midSideUsed = false;
  BYTE profileIndex = 0;

  _fseeki64(Stream, CTools::ID3v2Size + 4, SEEK_SET);
  for (int packet = 0; packet < 32; packet++)
  {
    BYTE head[2 + 9];
    size_t got = fread(head, 1, sizeof(head), Stream);
    if (got < 4)
      break;
    size_t pos = 2;
    unsigned __int64 packetSize = 0;
    if (!ReadVarInt(head, got, pos, packetSize) || packetSize < pos || packetSize > 0x7FFFFFFF)
      break;
    const size_t payloadLen = (size_t)(packetSize - pos);
    const char *key = (const char *)head;
    if (memcmp(key, "AP", 2) == 0 || memcmp(key, "SE", 2) == 0)
      break;
    // back to the start of the payload (after the header bytes that were read)
    _fseeki64(Stream, (__int64)pos - (__int64)got, SEEK_CUR);
    if (payloadLen > MAX_PACKET_PAYLOAD || (memcmp(key, "SH", 2) != 0 && memcmp(key, "EI", 2) != 0))
    {
      _fseeki64(Stream, (__int64)payloadLen, SEEK_CUR);
      continue;
    }
    BYTE payload[MAX_PACKET_PAYLOAD];
    if (fread(payload, 1, payloadLen, Stream) != payloadLen)
      break;
    if (memcmp(key, "SH", 2) == 0)
    {
      // CRC32 (4), stream version (1), sample count (VarInt), beginning silence (VarInt), 2 bytes frequency/bands and channels
      size_t p = 4;
      unsigned __int64 samples = 0, silence = 0;
      if (payloadLen < 9 || payload[p++] != 8 || !ReadVarInt(payload, payloadLen, p, samples) ||
          !ReadVarInt(payload, payloadLen, p, silence) || p + 2 > payloadLen)
        return false;
      const BYTE b1 = payload[p], b2 = payload[p + 1];
      FSampleRate = SAMPLE_RATES[b1 >> 5 & 3];
      FChannels = (b2 >> 4) + 1;
      midSideUsed = ((b2 >> 3) & 1) != 0;
      FSamples = (__int64)samples;
      FBeginSilence = (__int64)(silence < samples ? silence : samples);
      haveSH = true;
    }
    else if (payloadLen >= 1)
      profileIndex = payload[0] >> 4;   // profile in the upper 7 bits of byte 0; profile/8 corresponds to the index of the SV7 profiles
  }
  if (!haveSH || FSampleRate <= 0 || FChannels < 1 || FChannels > 2)
    return false;
  FIsSV8 = true;
  FStreamVersion = 8;
  FChannelModeID = (FChannels == 1) ? MPP_CM_MONO : (midSideUsed ? MPP_CM_JOINT_STEREO : MPP_CM_STEREO);
  FProfileID = ProfileFromIndex(profileIndex);
  FFrameCount = (long)((FSamples - FBeginSilence) / 1152);
  FBitRate = 0;
  FValid = true;
  return true;
}

/* -------------------------------------------------------------------------- */

BYTE CMPEGPlus::GetStreamVersion()
{
  if (FIsSV8)
    return 8;
  /* Get MPEGplus stream version */
  if (Header.IntegerArray[0] == STREAM_VERSION_7_ID)
    return 7;
  if (Header.IntegerArray[0] == STREAM_VERSION_71_ID)
    return 71;
  switch ((Header.ByteArray[1] % 32) / 2)
  {
    case  3: return 4;
    case  7: return 5;
    case 11: return 6;
  }
  return 0;
}

/* -------------------------------------------------------------------------- */

long CMPEGPlus::GetSampleRate()
{
  if (FIsSV8)
    return FSampleRate;
  switch (Header.ByteArray[10] & 3)
  {
    case 0: return 44100l;
    case 1: return 48000l;
    case 2: return 37800l;
    case 3: return 32000l;
  }
  return 0l;
}
/* -------------------------------------------------------------------------- */

BYTE CMPEGPlus::GetChannelModeID()
{
  /* Get channel mode for stream version 7 */
  BYTE ver= GetStreamVersion();
  if (ver == 7 || ver == 71)
  {
    if ((Header.ByteArray[11] % 128) < 64)
      return MPP_CM_STEREO;
    else
      return MPP_CM_JOINT_STEREO;
  }
  else
  {
    /* Get channel mode for stream version 4-6 */
    if ((Header.ByteArray[2] % 128) == 0)
      return MPP_CM_STEREO;
    else
      return MPP_CM_JOINT_STEREO;
  }
}

/* -------------------------------------------------------------------------- */

long CMPEGPlus::GetFrameCount()
{
  if (FIsSV8)
    return FFrameCount;
  /* Get frame count */
  BYTE Version = GetStreamVersion();
  if (Version == 4)
    return Header.IntegerArray[1] >> 16;
  if (Version == 5 || Version == 6 || Version == 7 || Version == 71)
    return Header.IntegerArray[1];
  return 0;
}

/* -------------------------------------------------------------------------- */

int CMPEGPlus::GetBitRateIntern()
{
  /*{ Try to get bit rate */
  BYTE Version = GetStreamVersion();
  if (Version == 4 || Version == 5)
    return (int)(Header.IntegerArray[0] >> 23);

  return 0;
}

/* -------------------------------------------------------------------------- */

BYTE CMPEGPlus::GetProfileID()
{
  BYTE ver = GetStreamVersion();
  /* Get MPEGplus profile (exists for stream version 7 only) */
  if (ver == 7 || ver == 71)
    return ProfileFromIndex((Header.ByteArray[10] & 0xF0) >> 4);
  return MPP_PROFILE_UNKNOWN;
}

/* -------------------------------------------------------------------------- */

// profile index (1..15): in SV7 the upper 4 bits of byte 10, in SV8 the profile of the EI packet divided by 8
BYTE CMPEGPlus::ProfileFromIndex(BYTE index)
{
  {
    switch (index)
    {
      case  1: return MPP_PROFILE_EXPERIMENTAL;
      case  5: return MPP_PROFILE_QUALITY0;
      case  6: return MPP_PROFILE_QUALITY1;
      case  7: return MPP_PROFILE_TELEPHONE;
      case  8: return MPP_PROFILE_THUMB;
      case  9: return MPP_PROFILE_RADIO;
      case 10: return MPP_PROFILE_STANDARD;
      case 11: return MPP_PROFILE_XTREME;
      case 12: return MPP_PROFILE_INSANE;
      case 13: return MPP_PROFILE_BRAINDEAD;
      case 14: return MPP_PROFILE_QUALITY9;
      case 15: return MPP_PROFILE_QUALITY10;
    }
  }
  return MPP_PROFILE_UNKNOWN;
}

/* -------------------------------------------------------------------------- */

void CMPEGPlus::ResetData()
{
  FIsSV8 = false;
  FChannels = 2;
  FSamples = 0;
  FBeginSilence = 0;
  FValid = false;
  FChannelModeID = 0;
  FFrameCount = 0;
  FBitRate = 0;
  FStreamVersion = 0;
  FSampleRate = 0;
  FProfileID = MPP_PROFILE_UNKNOWN;
  memset(&Header, 0, sizeof(Header));
}

/* -------------------------------------------------------------------------- */

CAtlString CMPEGPlus::GetChannelMode()
{
  switch (FChannelModeID)
  {
    case MPP_CM_STEREO:
      return STEREO;
    case MPP_CM_JOINT_STEREO:
      return JOINTSTEREO;
    case MPP_CM_MONO:
      return _T("Mono");
    default:
      return UNKNOWN;
  }  
}

/* -------------------------------------------------------------------------- */

long CMPEGPlus::GetBitRate()
{
  __int64 CompressedSize;
  /* Calculate bit rate if not given */
  CompressedSize = CTools::FileSize - CTools::ID3v2Size - CTools::ID3v1Size - CTools::APESize;
  if (FIsSV8)
  {
    // SV8: exact duration from the sample count, bit rate = compressed size * 8 / duration
    const double seconds = (FSampleRate > 0) ? (double)(FSamples - FBeginSilence) / FSampleRate : 0.0;
    return (seconds > 0.0) ? (long)(CompressedSize * 8.0 / seconds / 1000.0 + 0.5) : 0;
  }
  if (FBitRate == 0 && FFrameCount > 0)
    return (long) (CompressedSize / 144l * (FSampleRate/1000l) / FFrameCount); // Neu ab V2.3.2
    /* ALT: return (long) (CompressedSize * 8l * (FSampleRate/1000l) / FFrameCount / 1152l); ALTE Berechnung */
  return FBitRate;
}

/* -------------------------------------------------------------------------- */

CAtlString CMPEGPlus::GetFileVersion()
{
  return MPP_PROFILE[FProfileID];
}

/* -------------------------------------------------------------------------- */

float CMPEGPlus::GetDuration()
{
  /* Calculate duration time */
  if (FIsSV8)
    return (FSampleRate > 0) ? (float)((double)(FSamples - FBeginSilence) / FSampleRate) : 0.0f;
  if (FSampleRate > 0)
    return (float) (FFrameCount * 1152.0f / FSampleRate);
  else
    return 0.0f;
}

/* -------------------------------------------------------------------------- */

bool CMPEGPlus::ReadFromFile(FILE *Stream)
{
  /* Process data if loaded and file valid */
  if (ReadHeader(Stream))
  {
    if (FIsSV8)
      return true;   // ReadHeaderSV8 has set all values
    FValid = (GetStreamVersion() > 0);
    /* Fill properties with header data */
    FSampleRate = GetSampleRate();
    FChannelModeID = GetChannelModeID();
    FFrameCount = GetFrameCount();
    FBitRate = GetBitRateIntern();
    FStreamVersion = GetStreamVersion();
    FProfileID = GetProfileID();
    return true;
  }
  ResetData();
  return false;
}
