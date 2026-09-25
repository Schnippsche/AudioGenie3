/* 
* hashlib++ - a simple hash library for C++
* 
* Copyright (c) 2007,2008 Benjamin Grüdelbach
* 
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 
* 	1)     Redistributions of source code must retain the above copyright
* 	       notice, this list of conditions and the following disclaimer.
* 
* 	2)     Redistributions in binary form must reproduce the above copyright
* 	       notice, this list of conditions and the following disclaimer in
* 	       the documentation and/or other materials provided with the
* 	       distribution.
* 	     
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//----------------------------------------------------------------------	

/*
* The hashlib++ MD5 implementation is derivative from the sourcecode
* published in RFC 1321 
* 
* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
* rights reserved.
* 
* License to copy and use this software is granted provided that it
* is identified as the "RSA Data Security, Inc. MD5 Message-Digest
* Algorithm" in all material mentioning or referencing this software
* or this function.
* 
* License is also granted to make and use derivative works provided
* that such works are identified as "derived from the RSA Data
* Security, Inc. MD5 Message-Digest Algorithm" in all material
* mentioning or referencing the derived work.
* 
* RSA Data Security, Inc. makes no representations concerning either
* the merchantability of this software or the suitability of this
* software for any particular purpose. It is provided "as is"
* without express or implied warranty of any kind.
* 
* These notices must be retained in any copies of any part of this
* documentation and/or software.
*/

//----------------------------------------------------------------------	

/**
*  @file 	hl_md5.cpp
*  @brief	This file contains the implementation of the MD5 class
*  @date 	Mo 17 Sep 2007
*/  

//----------------------------------------------------------------------
//hashlib++ includes
#include "stdafx.h"
#include <intrin.h>
#include "md5.h"

//----------------------------------------------------------------------
// defines

// Constants for MD5Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/*
* ROTATE_LEFT rotates x left n bits. 
* cast to unsigned int to guarantee support for 64Bit System
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | (( (unsigned int) x) >> (32-(n))))

/*
FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
  (a) += F ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }

#define GG(a, b, c, d, x, s, ac) { \
  (a) += G ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
  (a) += H ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
  (a) += I ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }

//----------------------------------------------------------------------	
//private member-functions

/**
*  @brief 	Processes nblocks consecutive 64 byte blocks and updates state. The state stays in registers for all blocks and
*  		the words are read directly (little endian host), which is faster than transforming block by block.
*  @param	state	state to transform
*  @param	data	the blocks
*  @param	nblocks	number of blocks
*/  
#define FAST_F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define FAST_G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define FAST_H(x, y, z) ((x) ^ (y) ^ (z))
#define FAST_I(x, y, z) ((y) ^ ((x) | ~(z)))
#define FAST_STEP(f, a, b, c, d, x, t, s) { (a) += f((b), (c), (d)) + (x) + (t); (a) = _rotl((a), (s)); (a) += (b); }

static_assert(sizeof(unsigned long int) == 4, "the state of MD5 must consist of 32 bit words");

static void MD5Blocks(unsigned long int state[4], const unsigned char *data, size_t nblocks)
{
  unsigned int a = (unsigned int)state[0], b = (unsigned int)state[1], c = (unsigned int)state[2], d = (unsigned int)state[3];
  while (nblocks--)
  {
    unsigned int x[16];
    memcpy(x, data, 64);
    const unsigned int sa = a, sb = b, sc = c, sd = d;
    FAST_STEP(FAST_F, a, b, c, d, x[0], 0xd76aa478u, 7);
    FAST_STEP(FAST_F, d, a, b, c, x[1], 0xe8c7b756u, 12);
    FAST_STEP(FAST_F, c, d, a, b, x[2], 0x242070dbu, 17);
    FAST_STEP(FAST_F, b, c, d, a, x[3], 0xc1bdceeeu, 22);
    FAST_STEP(FAST_F, a, b, c, d, x[4], 0xf57c0fafu, 7);
    FAST_STEP(FAST_F, d, a, b, c, x[5], 0x4787c62au, 12);
    FAST_STEP(FAST_F, c, d, a, b, x[6], 0xa8304613u, 17);
    FAST_STEP(FAST_F, b, c, d, a, x[7], 0xfd469501u, 22);
    FAST_STEP(FAST_F, a, b, c, d, x[8], 0x698098d8u, 7);
    FAST_STEP(FAST_F, d, a, b, c, x[9], 0x8b44f7afu, 12);
    FAST_STEP(FAST_F, c, d, a, b, x[10], 0xffff5bb1u, 17);
    FAST_STEP(FAST_F, b, c, d, a, x[11], 0x895cd7beu, 22);
    FAST_STEP(FAST_F, a, b, c, d, x[12], 0x6b901122u, 7);
    FAST_STEP(FAST_F, d, a, b, c, x[13], 0xfd987193u, 12);
    FAST_STEP(FAST_F, c, d, a, b, x[14], 0xa679438eu, 17);
    FAST_STEP(FAST_F, b, c, d, a, x[15], 0x49b40821u, 22);
    FAST_STEP(FAST_G, a, b, c, d, x[1], 0xf61e2562u, 5);
    FAST_STEP(FAST_G, d, a, b, c, x[6], 0xc040b340u, 9);
    FAST_STEP(FAST_G, c, d, a, b, x[11], 0x265e5a51u, 14);
    FAST_STEP(FAST_G, b, c, d, a, x[0], 0xe9b6c7aau, 20);
    FAST_STEP(FAST_G, a, b, c, d, x[5], 0xd62f105du, 5);
    FAST_STEP(FAST_G, d, a, b, c, x[10], 0x02441453u, 9);
    FAST_STEP(FAST_G, c, d, a, b, x[15], 0xd8a1e681u, 14);
    FAST_STEP(FAST_G, b, c, d, a, x[4], 0xe7d3fbc8u, 20);
    FAST_STEP(FAST_G, a, b, c, d, x[9], 0x21e1cde6u, 5);
    FAST_STEP(FAST_G, d, a, b, c, x[14], 0xc33707d6u, 9);
    FAST_STEP(FAST_G, c, d, a, b, x[3], 0xf4d50d87u, 14);
    FAST_STEP(FAST_G, b, c, d, a, x[8], 0x455a14edu, 20);
    FAST_STEP(FAST_G, a, b, c, d, x[13], 0xa9e3e905u, 5);
    FAST_STEP(FAST_G, d, a, b, c, x[2], 0xfcefa3f8u, 9);
    FAST_STEP(FAST_G, c, d, a, b, x[7], 0x676f02d9u, 14);
    FAST_STEP(FAST_G, b, c, d, a, x[12], 0x8d2a4c8au, 20);
    FAST_STEP(FAST_H, a, b, c, d, x[5], 0xfffa3942u, 4);
    FAST_STEP(FAST_H, d, a, b, c, x[8], 0x8771f681u, 11);
    FAST_STEP(FAST_H, c, d, a, b, x[11], 0x6d9d6122u, 16);
    FAST_STEP(FAST_H, b, c, d, a, x[14], 0xfde5380cu, 23);
    FAST_STEP(FAST_H, a, b, c, d, x[1], 0xa4beea44u, 4);
    FAST_STEP(FAST_H, d, a, b, c, x[4], 0x4bdecfa9u, 11);
    FAST_STEP(FAST_H, c, d, a, b, x[7], 0xf6bb4b60u, 16);
    FAST_STEP(FAST_H, b, c, d, a, x[10], 0xbebfbc70u, 23);
    FAST_STEP(FAST_H, a, b, c, d, x[13], 0x289b7ec6u, 4);
    FAST_STEP(FAST_H, d, a, b, c, x[0], 0xeaa127fau, 11);
    FAST_STEP(FAST_H, c, d, a, b, x[3], 0xd4ef3085u, 16);
    FAST_STEP(FAST_H, b, c, d, a, x[6], 0x04881d05u, 23);
    FAST_STEP(FAST_H, a, b, c, d, x[9], 0xd9d4d039u, 4);
    FAST_STEP(FAST_H, d, a, b, c, x[12], 0xe6db99e5u, 11);
    FAST_STEP(FAST_H, c, d, a, b, x[15], 0x1fa27cf8u, 16);
    FAST_STEP(FAST_H, b, c, d, a, x[2], 0xc4ac5665u, 23);
    FAST_STEP(FAST_I, a, b, c, d, x[0], 0xf4292244u, 6);
    FAST_STEP(FAST_I, d, a, b, c, x[7], 0x432aff97u, 10);
    FAST_STEP(FAST_I, c, d, a, b, x[14], 0xab9423a7u, 15);
    FAST_STEP(FAST_I, b, c, d, a, x[5], 0xfc93a039u, 21);
    FAST_STEP(FAST_I, a, b, c, d, x[12], 0x655b59c3u, 6);
    FAST_STEP(FAST_I, d, a, b, c, x[3], 0x8f0ccc92u, 10);
    FAST_STEP(FAST_I, c, d, a, b, x[10], 0xffeff47du, 15);
    FAST_STEP(FAST_I, b, c, d, a, x[1], 0x85845dd1u, 21);
    FAST_STEP(FAST_I, a, b, c, d, x[8], 0x6fa87e4fu, 6);
    FAST_STEP(FAST_I, d, a, b, c, x[15], 0xfe2ce6e0u, 10);
    FAST_STEP(FAST_I, c, d, a, b, x[6], 0xa3014314u, 15);
    FAST_STEP(FAST_I, b, c, d, a, x[13], 0x4e0811a1u, 21);
    FAST_STEP(FAST_I, a, b, c, d, x[4], 0xf7537e82u, 6);
    FAST_STEP(FAST_I, d, a, b, c, x[11], 0xbd3af235u, 10);
    FAST_STEP(FAST_I, c, d, a, b, x[2], 0x2ad7d2bbu, 15);
    FAST_STEP(FAST_I, b, c, d, a, x[9], 0xeb86d391u, 21);
    a += sa; b += sb; c += sc; d += sd;
    data += 64;
  }
  state[0] = a; state[1] = b; state[2] = c; state[3] = d;
}

/**
*  @brief 	Basic transformation. Transforms state based on block.
*  @param	state	state to transform
*  @param	block	block to transform
*/  
void MD5::MD5Transform (unsigned long int state[4], unsigned char block[64])
{
  MD5Blocks(state, block, 1);
}

/**
*  @brief 	Encodes input data
*  @param	output Encoded data as OUT parameter
*  @param	input Input data
*  @param	len The length of the input assuming it is a
*  		multiple of 4
*/  
void MD5::Encode (unsigned char *output, unsigned long int *input, unsigned int len)
{
  size_t i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
    output[j] = (unsigned char)(input[i] & 0xff);
    output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
    output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
    output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
  }
}

/**
*  @brief 	Decodes input data into output
*  @param	output Decoded data as OUT parameter
*  @param	input Input data
*  @param	len The length of the input assuming it is a
*  		multiple of 4
*/  
void MD5::Decode (unsigned long int *output, unsigned char *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
    output[i] = ((unsigned long int)input[j]) | 
    (((unsigned long int)input[j+1]) << 8) |
    (((unsigned long int)input[j+2]) << 16) |
    (((unsigned long int)input[j+3]) << 24);
}

//----------------------------------------------------------------------	
//public member-functions

/**
*  @brief 	Initialization begins an operation,
*  		writing a new context
*  @param 	context	The MD5_CTX context to initialize
*/  
void MD5::MD5Init (MD5_CTX *context)
{
  context->count[0] = context->count[1] = 0;
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

/**
*  @brief 	Block update operation. Continues an md5
*  		message-digest operation, processing another
*  		message block, and updating the context.
*  @param	context The MD5_CTX context to update
*  @param	input The data to write into the context
*  @param	inputLen The length of the input data
*/  
void MD5::MD5Update (MD5_CTX *context, unsigned char *input, unsigned int inputLen)
{
  unsigned int index, partLen;
  size_t i;
  /* Compute number of bytes mod 64 */
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);

  /* Update number of bits */
  if ( (context->count[0] += ((unsigned long int)inputLen << 3))
    < ((unsigned long int)inputLen << 3))
    context->count[1]++;

  context->count[1] += ((unsigned long int)inputLen >> 29);
  partLen = 64 - index;

  /*
  * Transform as many times as possible.
  */
  if (inputLen >= partLen) 
  {
    memcpy((POINTER)&context->buffer[index], (POINTER)input, partLen);
    MD5Transform (context->state, context->buffer);

    // all complete blocks in one call
    const size_t blocks = (inputLen - partLen) / 64;
    MD5Blocks (context->state, &input[partLen], blocks);
    i = partLen + blocks * 64;

    index = 0;
  }
  else 
    i = 0;

  /* Buffer remaining input */
  memcpy((POINTER)&context->buffer[index], (POINTER)&input[i],  inputLen-i);
}

/**
*  @brief 	Finalization ends the md5 message-digest 
*  		operation, writing the the message digest and
*  		zeroizing the context.
*  @param	digest This is an OUT parameter which contains
*  		the created hash after the method returns
*  @param	context The context to finalize
*/  
void MD5::MD5Final (unsigned char digest[16], MD5_CTX *context)
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  Encode (bits, context->count, 8);

  /* 
  * Pad out to 56 mod 64.
  */
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  MD5Update (context, PADDING, padLen);

  /* Append length (before padding) */
  MD5Update (context, bits, 8);

  /* Store state in digest */
  Encode (digest, context->state, 16);

  /*
  * Zeroize sensitive information.
  */
  memset((POINTER)context, 0, sizeof (*context));  
}
