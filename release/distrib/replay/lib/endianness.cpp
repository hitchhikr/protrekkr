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

// ------------------------------------------------------
// Includes
#include "include/endianness.h"

// ------------------------------------------------------
// Functions
#if defined(__BIG_ENDIAN__) || defined(__MOT_SWAP__)

Uint64 __Swap_64(Uint64 Value)
{
#if defined(__GCC__)
    return(((Value & 0xffll) << 56) |
           ((Value & 0xff00ll) << 40) |
           ((Value & 0xff0000ll) << 24) |
           ((Value & 0xff000000ll) << 8) |
           ((Value & 0xff00000000ll) >> 8) |
           ((Value & 0xff0000000000ll) >> 24) |
           ((Value & 0xff000000000000ll) >> 40) |
           ((Value & 0xff00000000000000ll) >> 56)
          );
#else
    return(((Value & 0xff) << 56) |
           ((Value & 0xff00) << 40) |
           ((Value & 0xff0000) << 24) |
           ((Value & 0xff000000) << 8) |
           ((Value & 0xff00000000) >> 8) |
           ((Value & 0xff0000000000) >> 24) |
           ((Value & 0xff000000000000) >> 40) |
           ((Value & 0xff00000000000000) >> 56)
          );
#endif
}

int __Swap_32(int Value)
{
    return(((Value & 0xff) << 24) |
           ((Value & 0xff00) << 8) |
           ((Value & 0xff0000) >> 8) |
           ((Value & 0xff000000) >> 24)
          );
}

short __Swap_16(short Value)
{
    return(((Value & 0xff) << 8) |
           ((Value & 0xff00) >> 8)
          );
}

#endif
