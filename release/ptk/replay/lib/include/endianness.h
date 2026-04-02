// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2026 Franck Charlet.
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

// ------------------------------------------------------
// Includes
#include <ptk_types.h>

// ------------------------------------------------------
// Functions
#if defined(__BIG_ENDIAN__)
UINT64 __Swap_64(UINT64 Value);
UINT32 __Swap_32(UINT32 Value);
UINT16 __Swap_16(UINT16 Value);
#define Swap_64(x) __Swap_64((UINT64) x)
#define Swap_32(x) __Swap_32((UINT32) x)
#define Swap_16(x) __Swap_16((UINT16) x)
#else
#define Swap_64(x) (UINT64) x
#define Swap_32(x) (UINT32) x
#define Swap_16(x) (UINT16) x
#endif

// Only used inside the tracker to load Audio IFF files
#if defined(__MOT_SWAP__)
#if defined(__BIG_ENDIAN__)
#define Mot_Swap_64(x) (UINT64) x
#define Mot_Swap_32(x) (UINT32) x
#define Mot_Swap_16(x) (UINT16) x
#else
UINT64 __Swap_64(UINT64 Value);
UINT32 __Swap_32(UINT32 Value);
UINT16 __Swap_16(UINT16 Value);
#define Mot_Swap_64(x) __Swap_64((UINT64) x)
#define Mot_Swap_32(x) __Swap_32((UINT32) x)
#define Mot_Swap_16(x) __Swap_16((UINT16) x)
#endif
#endif

#endif
