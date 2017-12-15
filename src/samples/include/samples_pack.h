// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2014 Franck Charlet.
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

#ifndef _SAMPLES_PACK_H_
#define _SAMPLES_PACK_H_

// ------------------------------------------------------
// Constants
#define MP3_FRAMES_LAG 65536

// ------------------------------------------------------
// Functions
#if defined(__GSM_CODEC__)
int ToGSM(short *Source, short *Dest, int Size);
#endif
#if defined(__AT3_CODEC__)
int ToAT3(short *Source, short *Dest, int Size, int BitRate);
#endif
#if defined(__MP3_CODEC__)
int ToMP3(short *Source, short *Dest, int Size, int BitRate);
#endif
#if defined(__TRUESPEECH_CODEC__)
int ToTrueSpeech(short *Source, short *Dest, int Size);
#endif
#if defined(__ADPCM_CODEC__)
int ToADPCM(short *Source, short *Dest, int Size);
#endif
int To8Bit(short *Source, short *Dest, int Size);
int ToInternal(short *Source, short *Dest, int Size);

#endif
