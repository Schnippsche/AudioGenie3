
/* AudioGenie is a Library for analyzing and tagging audio files.
   Copyright (C) 2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011
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

// The following macros define the minimum required platform. The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the features required to run 
// your application. The macros enable all features that are available on the platform versions up to 
// and including the specified version.

// Change the following definitions for platforms older than those specified below.
// See MSDN for the latest information about the corresponding values for the different platforms.
#ifndef WINVER                          // Specifies that Windows 2000 is the minimum required platform.
#define WINVER 0x0500           // Change the corresponding value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that Windows 2000 is the minimum required platform.
#define _WIN32_WINNT 0x0500     // Change the corresponding value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that Windows 98 is the minimum required platform.
#define _WIN32_WINDOWS 0x0410 // Change the corresponding value to target at least Windows Me.
#endif

#ifndef _WIN32_IE                       // Specifies that Internet Explorer 7.0 is the minimum required platform.
#define _WIN32_IE 0x0700        // Change the corresponding value to target other versions of IE.
#endif

