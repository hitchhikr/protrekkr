// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2021 Franck Charlet.
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

#ifndef _SOUNDDRIVER_PSP_H_
#define _SOUNDDRIVER_PSP_H_

// ------------------------------------------------------
// Includes
#include <pspkernel.h>
#include <pspaudio.h>
#include <malloc.h>
#include <string.h>

#define STDCALL

#define TRUE 1
#define FALSE 0

// ------------------------------------------------------
// Constants
#define AUDIO_PCM_FREQ 44100

#define AUDIO_THREAD_STACKSIZE (1024 * 4)
#define AUDIO_THREAD_PRIORITY 2

#define AUDIO_DBUF_CHANNELS 2
#define AUDIO_DBUF_RESOLUTION 16

// ------------------------------------------------------
// Types
typedef unsigned int Uint32;
typedef unsigned char Uint8;
typedef int int32;
typedef char int8;

// ------------------------------------------------------
// Functions
extern int AUDIO_Latency;
extern int AUDIO_Milliseconds;

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
void Message_Error(char *Message);
#endif

int AUDIO_Init_Driver(void (STDCALL *Mixer)(Uint8 *, Uint32));
int AUDIO_Create_Sound_Buffer(int milliseconds);
void AUDIO_Stop_Sound_Buffer(void);
void AUDIO_Stop_Driver(void);
void AUDIO_Play(void);
void AUDIO_Stop(void);
void *AUDIO_malloc_64(int *size);
int AUDIO_IsPlaying(void);
float AUDIO_GetTime(void);
int AUDIO_GetSamples(void);
void AUDIO_ResetTimer(void);

#endif
