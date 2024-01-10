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

#ifndef _AIFF_H_
#define _AIFF_H_

// ------------------------------------------------------
// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../../../release/distrib/replay/lib/include/endianness.h"

// ------------------------------------------------------
// Constants
#if defined(__BIG_ENDIAN__)
#define FormID 'FORM'
#define AiffID 'AIFF'
#define AifcID 'AIFC'
#define SoundDataID 'SSND'
#else
#define FormID 'MROF'
#define AiffID 'FFIA'
#define AifcID 'CFIA'
#define SoundDataID 'DNSS'
#endif

#define NoLooping 0
#define ForwardLooping 1
#define ForwardBackwardLooping 2

typedef short MarkerId;
typedef long ID;

typedef unsigned int UINT32;
typedef int INT32;

#if defined(__WIN32__) && !defined(__GCC__)
typedef unsigned __int64 Uint64;
#else
#if defined(__HAIKU__) || defined(__LINUX__)
#include <stdint.h>
//typedef unsigned long long uint64_t;
typedef uint64_t Uint64;
#else
typedef unsigned long long Uint64;
#endif
#endif

// ------------------------------------------------------
// Structures
#if !defined(__GCC__)
#pragma pack(push)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif
typedef struct
{
    unsigned char count;
    char *string;
} pstring;

typedef struct
{
    short numChannels;
    unsigned long numSampleFrames;
    short sampleSize;
    short sampleRate;
} CommonChunk;

typedef struct
{
    unsigned long offset;
	unsigned long blockSize;
	unsigned char *WaveformData;
} SoundDataChunk;

typedef struct
{
    MarkerId id;
    unsigned long position;
    pstring markerName;
} Marker;

typedef struct
{
    unsigned short numMarkers;
    Marker *Markers;
} MarkerChunk;

typedef struct
{
    short PlayMode;
	MarkerId beginLoop;
	MarkerId endLoop;
} Loop;

typedef struct
{
    char baseNote;
    char detune;
    char lowNote;
    char highNote;
    char lowvelocity;
    char highvelocity;
    short gain;
    Loop sustainLoop;
    Loop releaseLoop;
} InstrumentChunk;

typedef struct
{
    unsigned char *MIDIdata;
} MIDIDataChunk;

#pragma pack(pop)

class AIFFFile
{
    public:
        AIFFFile();
        ~AIFFFile();
        int Open(const char *Filename);
        void Close();
        int BitsPerSample();
        int NumChannels();
        unsigned long NumSamples();
        int BaseNote();
        int LoopType();
        unsigned long LoopStart();
        unsigned long LoopEnd();

        int ReadMonoSample(short *Sample);
        int ReadStereoSample(short *L, short *R);

    private:
        unsigned long FourCC(const char *ChunkName);
        int Read(void *Data, unsigned NumBytes);
        long CurrentFilePosition();
        int Seek(long offset);
        int SeekChunk(const char *ChunkName);
        int Get_Marker(int Marker_Id);
        void IntToFloat(int *Dest, int Source);
        void Int64ToDouble(Uint64 *Dest, Uint64 Source);

        FILE *file;
        CommonChunk CommDat;
        MarkerChunk Markers;
        Marker CurMarker;
        int Use_Floats;
        int Block_Size;
        int Loop_Start;
        int Loop_End;
        char Base_Note;
        Loop SustainLoop;
};

#endif
