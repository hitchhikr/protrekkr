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

#ifndef _PTK_REPLAY_H_
#define _PTK_REPLAY_H_

// ------------------------------------------------------
// Includes
#if defined(__WIN32__)
#include <windows.h>
#define PTKEXPORT __stdcall
#else
#define PTKEXPORT
#endif

// ------------------------------------------------------
// Functions
#if !defined(__WINAMP__)
#if defined(__WIN32__)
int PTKEXPORT Ptk_InitDriver(HWND hWnd, int Latency);
#else
int PTKEXPORT Ptk_InitDriver(int Latency);
#endif // __WIN32__
#else
int PTKEXPORT Ptk_InitDriver(void);
#endif // __WINAMP__
int PTKEXPORT Ptk_InitModule(unsigned char *Module, int start_position);
int PTKEXPORT Ptk_GetRow(void);
int PTKEXPORT Ptk_GetPosition(void);
void PTKEXPORT Ptk_SetPosition(int position);
void PTKEXPORT Ptk_Play(void);
void PTKEXPORT Ptk_Stop(void);
void PTKEXPORT Ptk_ReleaseDriver(void);

#endif
