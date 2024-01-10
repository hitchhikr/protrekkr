// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2024 Franck Charlet.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
// ------------------------------------------------------

#ifndef __ENDIANNESS_H_
#define __ENDIANNESS_H_

#if defined(__WIN32__) && !defined(__GCC__)
typedef unsigned __int64 Uint64;
#else
#if defined(__HAIKU__) || defined(__LINUX__)
#include <stdint.h>
typedef uint64_t Uint64;
#else
typedef unsigned long long Uint64;
#endif
#endif

// ------------------------------------------------------
// Functions

#if defined(__BIG_ENDIAN__)
Uint64 __Swap_64(Uint64 Value);
int __Swap_32(int Value);
short __Swap_16(short Value);
#define Swap_64(x) __Swap_64((Uint64) x)
#define Swap_32(x) __Swap_32((int) x)
#define Swap_16(x) __Swap_16((int) x)
#else
#define Swap_64(x) (Uint64) x
#define Swap_32(x) (int) x
#define Swap_16(x) (int) x
#endif

// Only used inside the tracker to load Audio IFF files
#if defined(__MOT_SWAP__)
#if defined(__BIG_ENDIAN__)
#define Mot_Swap_64(x) (Uint64) x
#define Mot_Swap_32(x) (int) x
#define Mot_Swap_16(x) (int) x
#else
Uint64 __Swap_64(Uint64 Value);
int __Swap_32(int Value);
short __Swap_16(short Value);
#define Mot_Swap_64(x) __Swap_64((Uint64) x)
#define Mot_Swap_32(x) __Swap_32((int) x)
#define Mot_Swap_16(x) __Swap_16((int) x)
#endif
#endif

#endif
