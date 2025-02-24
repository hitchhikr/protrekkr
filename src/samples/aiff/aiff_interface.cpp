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

// ------------------------------------------------------
// Includes
#include "include/aiff.h"
#include "include/aiff_interface.h"

extern "C"
{

AIFFHandle create_aiff(void)
{
    return (AIFFFile *) new AIFFFile();
}

void free_aiff(AIFFHandle p)
{
    if(p)
    {
        delete (AIFFFile *) p;
        p = NULL;
    }
}

int AIFF_Open(AIFFHandle p, const char *Filename)
{
    return((AIFFFile *) p)->AIFF_Open(Filename);
}

void AIFF_Close(AIFFHandle p)
{
    ((AIFFFile *) p)->AIFF_Close();
}

int AIFF_BitsPerSample(AIFFHandle p)
{
    return((AIFFFile *) p)->AIFF_BitsPerSample();
}

int AIFF_NumChannels(AIFFHandle p)
{
    return((AIFFFile *) p)->AIFF_NumChannels();
}

unsigned long AIFF_NumSamples(AIFFHandle p)
{
    return((AIFFFile *) p)->AIFF_NumSamples();
}

int AIFF_BaseNote(AIFFHandle p)
{
    return((AIFFFile *) p)->AIFF_BaseNote();
}

int AIFF_LoopType(AIFFHandle p)
{
    return((AIFFFile *) p)->AIFF_LoopType();
}

unsigned long AIFF_LoopStart(AIFFHandle p)
{
    return((AIFFFile *) p)->AIFF_LoopStart();
}

unsigned long AIFF_LoopEnd(AIFFHandle p)
{
    return((AIFFFile *) p)->AIFF_LoopEnd();
}

int AIFF_ReadMonoSample(AIFFHandle p, short *Sample)
{
    return((AIFFFile *) p)->AIFF_ReadMonoSample(Sample);
}

int AIFF_ReadStereoSample(AIFFHandle p, short *L, short *R)
{
    return((AIFFFile *) p)->AIFF_ReadStereoSample(L, R);
}

}
