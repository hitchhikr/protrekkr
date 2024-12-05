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

#ifndef _SAMPLES_UNPACK_H_
#define _SAMPLES_UNPACK_H_

// ------------------------------------------------------
// Includes
#if defined(__STAND_ALONE__)
    #if defined(__WINAMP__)
        #include "ptk_def_properties.h"
    #else
        #if defined(__GCC__)
        #include "../../ptk_properties.h"
        #else
        #include "../ptk_properties.h"
        #endif
    #endif
#else
    #include "ptk_def_properties.h"
#endif

// ------------------------------------------------------
// Types
typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;

// ------------------------------------------------------
// Constants
#define MP3_FRAMES_DELAG (1460 - 355)

// ------------------------------------------------------
// Functions
#if defined(__STAND_ALONE__)
#if defined(PTK_AT3)
void Unpack_AT3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate);
#endif
#if defined(PTK_GSM)
void Unpack_GSM(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size);
#endif
#if defined(PTK_MP3)
void Unpack_MP3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate);
#endif
#else
#if defined(__AT3_CODEC__)
void Unpack_AT3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate);
#endif
#if defined(__GSM_CODEC__)
void Unpack_GSM(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size);
#endif
#if defined(__MP3_CODEC__)
void Unpack_MP3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate);
#endif
#endif
void Unpack_ADPCM(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size);
void Unpack_8Bit(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size);
void Unpack_WavPack(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size);

#endif
