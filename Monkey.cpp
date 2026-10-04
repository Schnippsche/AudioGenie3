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
#include "io.h"
#include "monkey.h"
#include "Blob.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMonkey::CMonkey()
{
}
CMonkey::~CMonkey()
{
}

void CMonkey::ResetData()
{
  /* Reset data */
  Header.Reset();
}

bool CMonkey::IsValid()
{
  /* Check for right Monkey"s Audio file data */
  return (Header.exists() &&
          Header.SampleRate > 0 &&
          Header.Channels > 0);
}

CAtlString CMonkey::GetFileVersion()
{
  _TCHAR Buf[16];

  /* Get encoder version */
  if (Header.VersionID == 0)
    return EMPTY;
  else
    _stprintf_s(Buf, 16, _T("%4.2f"), Header.VersionID / 1000.0);

  return CAtlString(Buf);
}

CAtlString CMonkey::GetCompression()
{
  /* Get compression level: 1000 fast, 2000 normal, 3000 high, 4000 extra high, 5000 insane */
  const int level = Header.CompressionID / 1000;
  return (level >= 0 && level < (int)(sizeof(MONKEY_COMPRESSION) / sizeof(MONKEY_COMPRESSION[0]))) ? MONKEY_COMPRESSION[level] : UNKNOWN;
}

BYTE CMonkey::GetBits()
{
  /* Get number of bits per sample (the new format has the value in the header, the old one in the format flags) */
  if (!IsValid())
    return 0;
  return (Header.BitsPerSample > 0 && Header.BitsPerSample <= 255) ? (BYTE)Header.BitsPerSample : 16;
}

CAtlString CMonkey::GetChannelMode()
{
  /* Get channel mode */
  switch (Header.Channels)
  {
    case 0:
      return UNKNOWN;
    case 1:
      return MONO;
    case 2:
      return STEREO;
    default:
      return MULTICHANNEL;
  }
}

float CMonkey::GetPeak()
{
  /* Get peak level ratio */
  if (IsValid() && ((Header.Flags & MONKEY_FLAG_PEAK_LEVEL) > 0) && Header.VersionID < 3980)
  {
    if (GetBits() == 16)
      return Header.PeakLevel / 32768.0f * 100.0f;
    if (GetBits() == 24)
      return Header.PeakLevel / 8388608.0f * 100.0f;
    return Header.PeakLevel / 128.0f * 100.0f;
  }
  return 0.0f;
}

long CMonkey::GetSamplesPerFrame()
{
  /* Get number of samples (blocks) in a frame */
  if (!IsValid() )
    return 0;
  return Header.BlocksPerFrame;
}

__int64 CMonkey::GetSamples64()
{
  /* Get number of samples: all frames have the blocks per frame, the final frame has its own number */
  if (!IsValid() || Header.Frames <= 0)
    return 0;
  return (__int64)(Header.Frames - 1) * (unsigned long)Header.BlocksPerFrame + (unsigned long)Header.FinalFrameBlocks;
}

long CMonkey::GetSamples()
{
  const __int64 samples = GetSamples64();
  return samples > 0x7FFFFFFF ? 0x7FFFFFFF : (long)samples;
}

float CMonkey::GetDuration()
{
  /* Get song duration */
  if (!IsValid())
    return 0.0f;
  return (float)((double)GetSamples64() / Header.SampleRate);
}

// the size of the compressed data: the file without the tags
static __int64 CompressedSize()
{
  return CTools::FileSize - CTools::audioStart() - CTools::ID3v1Size - CTools::APESize - CTools::LyricsSize;
}

float CMonkey::GetCompressionRatio()
{
  /* Get compression ratio: the compressed size divided by the size of the WAV data */
  if (!IsValid())
    return 0.0f;
  const double uncompressed = (double)GetSamples64() * GetChannels() * GetBits() / 8.0 + Header.HeaderDataBytes;
  return uncompressed > 0 ? (float)((double)CompressedSize() / uncompressed) : 0.0f;
}

long CMonkey::GetBitRate()
{
  /* the compressed data per second */
  const double duration = GetDuration();
  if (duration > 0 && CompressedSize() > 0)
    return (long)((double)CompressedSize() * 8.0 / duration / 1000.0 + 0.5);
  return 0;
}

bool CMonkey::ReadFromFile(FILE *Stream)
{
  /* Read Monkey"s Audio header data */
  CSequentialRead sequence(Stream, CTools::audioStart());   // read from the cache of the start of the file
  ResetData();
  if (Header.ReadFromFile(Stream))
    return true;
  ResetData();
  return false;
}

