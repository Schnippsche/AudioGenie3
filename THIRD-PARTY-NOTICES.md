# Third-party notices

AudioGenie3 itself is licensed under the GNU Lesser General Public License,
version 2.1 or (at your option) any later version (see `LICENSE`).
The following third-party code is included in the source tree and remains
under its own license.

## XProfan wrapper (`Wrapper/Profan/prfwrapper.inc`)

Written by Dieter Zornow and included in this repository with his permission. It was checked and adapted to the current API
by the AudioGenie3 project.

## hashlib++ (`md5.h`, `md5.cpp`)

Copyright (c) 2007, 2008 Benjamin Grüdelbach

Licensed under the BSD 2-clause license:

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## RSA Data Security, Inc. MD5 Message-Digest Algorithm (`md5.h`, `md5.cpp`)

The hashlib++ MD5 implementation is derived from the RSA Data Security, Inc.
MD5 Message-Digest Algorithm, published in RFC 1321.

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All rights
reserved.

License to copy and use this software is granted provided that it is identified
as the "RSA Data Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing this software or this function.

License is also granted to make and use derivative works provided that such
works are identified as "derived from the RSA Data Security, Inc. MD5
Message-Digest Algorithm" in all material mentioning or referencing the derived
work.

RSA Data Security, Inc. makes no representations concerning either the
merchantability of this software or the suitability of this software for any
particular purpose. It is provided "as is" without express or implied warranty
of any kind.

These notices must be retained in any copies of any part of this documentation
and/or software.

## puff (`puff.c`, `puff.h`)

Copyright (C) 2002-2013 Mark Adler, all rights reserved. Version 2.3, 21 Jan 2013. Obtained unmodified from the zlib
project's `contrib/puff/` (https://github.com/madler/zlib). Used to decompress ID3v2 frames that have the compression
flag set (the frame data is a zlib stream, RFC 1950/1951); no other part of the DLL uses it.

This software is provided 'as-is', without any express or implied warranty. In no event will the author be held liable
for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter
it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is
   not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original
   software.
3. This notice may not be removed or altered from any source distribution.

Mark Adler    madler@alumni.caltech.edu

## Catch2 (`tests/third_party/catch2/`, tests only)

Copyright Catch2 Authors. Distributed under the Boost Software License,
Version 1.0. The license text is in `tests/third_party/catch2/LICENSE.txt`.
Catch2 is used only to build the test suite and is not part of the DLL.
