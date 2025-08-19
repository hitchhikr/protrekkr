// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2025 Franck Charlet.
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

#ifndef _S3M_H_
#define _S3M_H_

// ------------------------------------------------------
// Includes
#include "files.h"
#include "reverbs.h"
#include "../../editors/include/patterns_blocks.h"
#include "../../../release/ptk/replay/lib/include/endianness.h"

#if !defined(BZR2)
#include "../../midi/include/midi.h"
#include "../../ui/include/misc_draw.h"
#endif

// ------------------------------------------------------
// Structures
typedef struct
{
    int old_note;
    int new_note;
} S3M_NOTE, *LPS3M_NOTE;

#if !defined(__GCC__)
#pragma pack(push)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

typedef struct
{
    char SongName[28];
    unsigned char EndSongMarker;
    unsigned char FileType;
    unsigned short Expansion;
    unsigned short SongLength;
    unsigned short SamplesEntries;
    unsigned short PatternsEntries;
    unsigned short Flags;
    unsigned short TrackerVersion;
    unsigned short SignedSamples;
    unsigned int Signature;
    unsigned char DefaultGlobalVolume;
    unsigned char DefaultSpeed;
    unsigned char DefaultTempo;
    unsigned char MasterVolume;
    unsigned char UltraClick;
    unsigned char DefaultPanningFlag;
    unsigned char ExpansionDatas[8];
    unsigned short SpecialPointer;
} S3M_MODULEHEADER, *LPS3M_MODULEHEADER;

#pragma pack(pop)

// ------------------------------------------------------
// Functions
int Check_S3M(int s3m_tag);
void Load_S3M(char *Name, const char *FileName);

#endif
