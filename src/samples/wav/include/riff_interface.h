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

#ifndef _RIFF_INTERFACE_H_
#define _RIFF_INTERFACE_H_

// ------------------------------------------------------
// Includes
#include "ddc.h"
#include "../../../../release/ptk/replay/lib/include/endianness.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void *WaveHandle;
WaveHandle create_wave(void);
void free_wave(WaveHandle);

DDCRET OpenForWrite(WaveHandle, const char  *Filename,
                    UINT32 SamplingRate,
                    UINT16 BitsPerSample,
                    UINT16 NumChannels
                   );

DDCRET OpenForRead(WaveHandle, const char *Filename);

DDCRET SeekToSample(WaveHandle, unsigned long SampleIndex);

DDCRET WriteData_void(WaveHandle, const void *data, UINT32 numData);
DDCRET WriteData_int32(WaveHandle, const INT32 *data, UINT32 numData);
// The following work only with 16-bit audio
DDCRET WriteData_int16(WaveHandle, const INT16 *data, UINT32 numData);
// The following work only with 8-bit audio
DDCRET WriteData_uint8(WaveHandle, const UINT8 *data, UINT32 numData);

DDCRET WriteMonoSample(WaveHandle, INT16 ChannelData);
DDCRET WriteStereoSample(WaveHandle, INT16 LeftChannelData, INT16 RightChannelData);
DDCRET WriteStereoFloatSample(WaveHandle, float LeftSample, float RightSample);

DDCRET ReadMonoSample(WaveHandle, INT16 *ChannelData);
DDCRET ReadStereoSample(WaveHandle, INT16 *LeftSampleData, INT16 *RightSampleData);

DDCRET CloseWave(WaveHandle);

UINT32   SamplingRate(WaveHandle);
UINT16   BitsPerSample(WaveHandle);
UINT16   NumChannels(WaveHandle);
UINT32   NumSamples(WaveHandle);

UINT32   Wave_LoopType(WaveHandle);
UINT32   Wave_LoopStart(WaveHandle);
UINT32   Wave_LoopEnd(WaveHandle);

// Open for write using another wave file's parameters...

long CurrentFilePosition(WaveHandle);

#ifdef __cplusplus
}
#endif

#endif /* _RIFF_H_ */
