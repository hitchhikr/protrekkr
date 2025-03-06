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
#include "include/riff.h"
#include "include/riff_interface.h"

extern "C"
{

WaveHandle create_wave(void)
{
    return (WaveFile *) new WaveFile();
};

void free_wave(WaveHandle p)
{
    if(p)
    {
        delete (WaveFile *) p;
        p = NULL;
    }
}

DDCRET OpenForWrite(WaveHandle p, const char *Filename,
                UINT32 SamplingRate,
                UINT16 BitsPerSample,
                UINT16 NumChannels
               )
{
    return((WaveFile *) p)->OpenForWrite(Filename, SamplingRate, BitsPerSample, NumChannels);
}

DDCRET OpenForRead(WaveHandle p, const char *Filename)
{
    return((WaveFile *) p)->OpenForRead(Filename);
}

DDCRET SeekToSample(WaveHandle p, unsigned long SampleIndex)
{
    return((WaveFile *) p)->SeekToSample(SampleIndex);
}

DDCRET WriteData_void(WaveHandle p, const void *data, UINT32 numData)
{
    return((WaveFile *) p)->WriteData_void(data, numData);
}

DDCRET WriteData_int32(WaveHandle p, const INT32 *data, UINT32 numData)
{
    return((WaveFile *) p)->WriteData_int32(data, numData);
}

DDCRET WriteData_int16(WaveHandle p, const INT16 *data, UINT32 numData)
{
    return((WaveFile *) p)->WriteData_int16(data, numData);
}

DDCRET WriteData_uint8(WaveHandle p, const UINT8 *data, UINT32 numData)
{
    return((WaveFile *) p)->WriteData_uint8(data, numData);
}

DDCRET WriteMonoSample(WaveHandle p, INT16 ChannelData)
{
    return((WaveFile *) p)->WriteMonoSample(ChannelData);
}

DDCRET WriteStereoSample(WaveHandle p, INT16 LeftChannelData, INT16 RightChannelData)
{
    return((WaveFile *) p)->WriteStereoSample(LeftChannelData, RightChannelData);
}

DDCRET WriteStereoFloatSample(WaveHandle p, float LeftSample, float RightSample)
{
    return((WaveFile *) p)->WriteStereoFloatSample(LeftSample, RightSample);
}

DDCRET ReadMonoSample(WaveHandle p, INT16 *ChannelData)
{
    return((WaveFile *) p)->ReadMonoSample(ChannelData);
}

DDCRET ReadStereoSample(WaveHandle p, INT16 *LeftSampleData, INT16 *RightSampleData)
{
    return((WaveFile *) p)->ReadStereoSample(LeftSampleData, RightSampleData);
}

DDCRET CloseWave(WaveHandle p)
{
    return((WaveFile *) p)->Close();
}

UINT32 SamplingRate(WaveHandle p)
{
    return((WaveFile *) p)->SamplingRate();
}

UINT16 BitsPerSample(WaveHandle p)
{
    return((WaveFile *) p)->BitsPerSample();
}

UINT16 NumChannels(WaveHandle p)
{
    return((WaveFile *) p)->NumChannels();
}

UINT32 NumSamples(WaveHandle p)
{
    return((WaveFile *) p)->NumSamples();
}

UINT32 Wave_LoopType(WaveHandle p)
{
    return((WaveFile *) p)->Wave_LoopType();
}

UINT32 Wave_LoopStart(WaveHandle p)
{
    return((WaveFile *) p)->Wave_LoopStart();
}

UINT32 Wave_LoopEnd(WaveHandle p)
{
    return((WaveFile *) p)->Wave_LoopEnd();
}

long CurrentFilePosition(WaveHandle p)
{
    return((WaveFile *) p)->CurrentFilePosition();
}

}
