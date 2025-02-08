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
#if defined(__WIN32__)
#include <windows.h>
#if defined(__GCC__)
#include "include/mmreg.h"
#include "include/msacm.h"
#else
#include <mmreg.h>
#include <msacm.h>
#endif
#endif

#include "include/samples_unpack.h"

// ------------------------------------------------------
// Variables
#if defined(__STAND_ALONE__)

#if defined(PTK_GSM)
GSM610WAVEFORMAT GSM_Format;
#endif
#if defined(PTK_AT3)
TRUESPEECHWAVEFORMAT At3_Format;
#endif
#if defined(PTK_MP3)
MPEGLAYER3WAVEFORMAT MP3_Format;
#endif

#else

#if defined(__MP3_CODEC__)
extern MPEGLAYER3WAVEFORMAT MP3_Format;
#else
#undef PTK_MP3
#endif
#if defined(__GSM_CODEC__)
extern GSM610WAVEFORMAT GSM_Format;
#else
#undef PTK_GSM
#endif
#if defined(__AT3_CODEC__)
extern TRUESPEECHWAVEFORMAT At3_Format;
#else
#undef PTK_AT3
#endif

#endif

#if defined(__WIN32__)
ACMSTREAMHEADER Unpack_Stream_Head;
HACMSTREAM Unpack_Stream;
WAVEFORMATEX Wave_Format;
#endif

// ------------------------------------------------------
// Unpack a GSM sample
#if defined(PTK_GSM)
void Unpack_GSM(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
{
    int i;
    int Real_Size;

    memset(&Wave_Format, 0, sizeof(WAVEFORMATEX));
    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;

    GSM_Format.wfx.wFormatTag = WAVE_FORMAT_GSM610;
    GSM_Format.wfx.nChannels = 1;
    GSM_Format.wfx.nSamplesPerSec = 0xac44;
    GSM_Format.wfx.nAvgBytesPerSec = 0x22fd;
    GSM_Format.wfx.nBlockAlign = 0x41;
    GSM_Format.wfx.wBitsPerSample = 0;
    GSM_Format.wfx.cbSize = 2;
    GSM_Format.wSamplesPerBlock = 0x140;
    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &GSM_Format, (LPWAVEFORMATEX) &Wave_Format, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Unpack_Stream, NULL, (LPWAVEFORMATEX) &GSM_Format, (LPWAVEFORMATEX) &Wave_Format, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

    acmStreamSize(Unpack_Stream, Src_Size, (DWORD *) &Real_Size, ACM_STREAMSIZEF_SOURCE);
    short *dwDest = (short *) malloc((Real_Size * 2) + 8);
    memset(dwDest, 0, (Real_Size * 2) + 8);
    Unpack_Stream_Head.cbStruct = sizeof(ACMSTREAMHEADER);
    Unpack_Stream_Head.pbSrc = Source;
    Unpack_Stream_Head.cbSrcLength = Src_Size;
    Unpack_Stream_Head.pbDst = (Uint8 *) dwDest;
    Unpack_Stream_Head.cbDstLength = Real_Size;
    acmStreamPrepareHeader(Unpack_Stream, &Unpack_Stream_Head, 0);
    acmStreamConvert(Unpack_Stream, &Unpack_Stream_Head, 0);

    for(i = 0; i < Dst_Size; i++)
    {
        Dest[i] = dwDest[i];
    }

    if(dwDest) free(dwDest);
    acmStreamUnprepareHeader(Unpack_Stream, &Unpack_Stream_Head, 0);
    acmStreamClose(Unpack_Stream, 0);
}
#endif

// ------------------------------------------------------
// Unpack an AT3 sample
#if defined(PTK_AT3)
void Unpack_AT3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate)
{
    int i;
    int Real_Size;

    memset(&Wave_Format, 0, sizeof(WAVEFORMATEX));
    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;

    At3_Format.wfx.wFormatTag = 0x270;
    At3_Format.wfx.nChannels = 2;
    At3_Format.wfx.nSamplesPerSec = 44100;
    At3_Format.wfx.nAvgBytesPerSec = BitRate * 125;
    switch(BitRate)
    {
        case 66:
            At3_Format.wfx.nBlockAlign = 192;
            break;

        case 105:
            At3_Format.wfx.nBlockAlign = 304;
            break;

        case 132:
            At3_Format.wfx.nBlockAlign = 384;
            break;
    }
    At3_Format.wfx.wBitsPerSample = 0;
    At3_Format.wfx.cbSize = 0xe;
    At3_Format.wRevision = 1;
    At3_Format.nSamplesPerBlock = 0x800;
    At3_Format.abReserved[2] = 1;
    At3_Format.abReserved[4] = 1;
    At3_Format.abReserved[6] = 1;

    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &At3_Format, (LPWAVEFORMATEX) &Wave_Format, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Unpack_Stream, NULL, (LPWAVEFORMATEX) &At3_Format, (LPWAVEFORMATEX) &Wave_Format, NULL, 0, 0, 0);

    acmStreamSize(Unpack_Stream, Src_Size, (DWORD *) &Real_Size, ACM_STREAMSIZEF_SOURCE);
    short *dwDest = (short *) malloc((Real_Size * 4) + 8);
    memset(dwDest, 0, (Real_Size * 4) + 8);
    Unpack_Stream_Head.cbStruct = sizeof(ACMSTREAMHEADER);
    Unpack_Stream_Head.pbSrc = Source;
    Unpack_Stream_Head.cbSrcLength = Src_Size;
    Unpack_Stream_Head.pbDst = (Uint8 *) dwDest;
    Unpack_Stream_Head.cbDstLength = Real_Size;
    acmStreamPrepareHeader(Unpack_Stream, &Unpack_Stream_Head, 0);
    acmStreamConvert(Unpack_Stream, &Unpack_Stream_Head, 0);

    for(i = 0; i < Dst_Size; i++)
    {
        Dest[i] = dwDest[i * 2];
    }

    if(dwDest) free(dwDest);
    acmStreamUnprepareHeader(Unpack_Stream, &Unpack_Stream_Head, 0);
    acmStreamClose(Unpack_Stream, 0);
}
#endif

// ------------------------------------------------------
// Unpack a MP3 sample
#if defined(PTK_MP3)
void Unpack_MP3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate)
{
    int i;
    int Real_Size;

    memset(&Wave_Format, 0, sizeof(WAVEFORMATEX));
    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;
    MP3_Format.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
    MP3_Format.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
    MP3_Format.wfx.nChannels = 1;
    MP3_Format.wfx.nSamplesPerSec = 44100;
    MP3_Format.wfx.nAvgBytesPerSec = BitRate * (1000 / 8);
    MP3_Format.wfx.nBlockAlign = 1;
    MP3_Format.wID = MPEGLAYER3_ID_MPEG;
    MP3_Format.fdwFlags = MPEGLAYER3_FLAG_PADDING_OFF;
    MP3_Format.nBlockSize = 0x68;
    MP3_Format.nFramesPerBlock = 0;
    MP3_Format.nCodecDelay = 0;
    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &MP3_Format, (LPWAVEFORMATEX) &Wave_Format, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Unpack_Stream, NULL, (LPWAVEFORMATEX) &MP3_Format, (LPWAVEFORMATEX) &Wave_Format, NULL, 0, 0, 0 );

    acmStreamSize(Unpack_Stream, Src_Size, (DWORD *) &Real_Size, ACM_STREAMSIZEF_SOURCE);
    short *dwDest = (short *) malloc((Real_Size * 2) + (MP3_FRAMES_DELAG * 4) + 8);
    memset(dwDest, 0, (Real_Size * 2) + (MP3_FRAMES_DELAG * 4) + 8);
    Unpack_Stream_Head.cbStruct = sizeof(ACMSTREAMHEADER);
    Unpack_Stream_Head.pbSrc = Source;
    Unpack_Stream_Head.cbSrcLength = Src_Size;
    Unpack_Stream_Head.pbDst = (Uint8 *) dwDest;
    Unpack_Stream_Head.cbDstLength = Real_Size;
    acmStreamPrepareHeader(Unpack_Stream, &Unpack_Stream_Head, 0);
    acmStreamConvert(Unpack_Stream, &Unpack_Stream_Head, 0);

    for(i = 0; i < Dst_Size; i++)
    {
        Dest[i] = dwDest[i + MP3_FRAMES_DELAG];
    }

    if(dwDest) free(dwDest);
    acmStreamUnprepareHeader(Unpack_Stream, &Unpack_Stream_Head, 0);
    acmStreamClose(Unpack_Stream, 0);
}
#endif

// ------------------------------------------------------
// Unpack an ADPCM sample
#if defined(PTK_ADPCM)
static int indexTable[16] =
{
	-1, -1, -1, -1, 2, 4, 6, 8,
	-1, -1, -1, -1, 2, 4, 6, 8
};
static int stepsizeTable[89] =
{
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
	19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
	50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
	130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
	337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
	876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
	2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
	5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};
void Unpack_ADPCM(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
{
    signed char *inp;   /* Input buffer pointer */
    short *outp;        /* output buffer pointer */
    int sign;           /* Current adpcm sign bit */
    int delta;          /* Current adpcm output value */
    int step;           /* Stepsize */
    int valpred;        /* Predicted value */
    int vpdiff;         /* Current change to valpred */
    int index;          /* Current step change index */
    int inputbuffer;    /* place to keep next 4-bit value */
    int bufferstep;     /* toggle between inputbuffer/input */
    outp = Dest;
    inp = (signed char *) Source;

    valpred = 0;
    index = 0;
    step = stepsizeTable[index];

    bufferstep = 0;

    for (; Dst_Size > 0; Dst_Size--) {

        /* Step 1 - get the delta value */
        if (bufferstep)
        {
            delta = inputbuffer & 0xf;
        }
        else
        {
            inputbuffer = *inp++;
            delta = (inputbuffer >> 4) & 0xf;
        }
        bufferstep = !bufferstep;

        /* Step 2 - Find new index value (for later) */
        index += indexTable[delta];
        if (index < 0) index = 0;
        if (index > 88) index = 88;

        /* Step 3 - Separate sign and magnitude */
        sign = delta & 8;
        delta = delta & 7;

        /* Step 4 - Compute difference and new predicted value */
        /*
        ** Computes 'vpdiff = (delta+0.5)*step/4', but see comment
        ** in adpcm_coder.
        */
        vpdiff = step >> 3;
        if (delta & 4) vpdiff += step;
        if (delta & 2) vpdiff += step >> 1;
        if (delta & 1) vpdiff += step >> 2;

        if (sign)
            valpred -= vpdiff;
        else
            valpred += vpdiff;

        /* Step 5 - clamp output value */
        if (valpred > 32767) valpred = 32767;
        else if (valpred < -32768) valpred = -32768;

        /* Step 6 - Update step value */
        step = stepsizeTable[index];

        /* Step 7 - Output value */
        *outp++ = valpred;
    }
}
#endif

// ------------------------------------------------------
// Unpack a Bit 8 sample
#if defined(PTK_8BIT)
void Unpack_8Bit(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
{
    int i;

    for(i = 0; i < Src_Size; i++)
    {
        Dest[i] = Source[i] << 8;
    }
}
#endif
