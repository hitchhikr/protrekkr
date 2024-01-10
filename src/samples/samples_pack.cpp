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

// ------------------------------------------------------
// Includes
#if defined(__WIN32__)
#include <windows.h>
#if defined(__GCC__)
#include "../../release/distrib/replay/lib/include/mmreg.h"
#include "../../release/distrib/replay/lib/include/msacm.h"
#else
#include <mmreg.h>
#include <msacm.h>
#endif
#endif

#include "include/samples_pack.h"
#include "../../release/distrib/replay/lib/include/samples_unpack.h"

int pack_audio(short *Source, short *Dest, int Size);

// ------------------------------------------------------
// Variables
#if defined(STAND_ALONE)

#if defined(PTK_MP3)
MPEGLAYER3WAVEFORMAT MP3_Format;
#endif
#if defined(PTK_GSM)
GSM610WAVEFORMAT GSM_Format;
#endif
#if defined(PTK_AT3)
TRUESPEECHWAVEFORMAT At3_Format;
#endif

#if defined(__WIN32__)
WAVEFORMATEX Wave_Format;
ACMSTREAMHEADER Pack_Stream_Head;
HACMSTREAM Pack_Stream;
#endif

#else

#if defined(__MP3_CODEC__)
MPEGLAYER3WAVEFORMAT MP3_Format;
#endif
#if defined(__GSM_CODEC__)
GSM610WAVEFORMAT GSM_Format;
#endif
#if defined(__AT3_CODEC__)
TRUESPEECHWAVEFORMAT At3_Format;
#endif

#if defined(__WIN32__)
extern WAVEFORMATEX Wave_Format;
ACMSTREAMHEADER Pack_Stream_Head;
HACMSTREAM Pack_Stream;
#endif

#endif

// ------------------------------------------------------
// Pack a sample to GSM
#if defined(__GSM_CODEC__)
int ToGSM(short *Source, short *Dest, int Size)
{
    int Src_size;
    int Dest_Size;

    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;
    Wave_Format.nChannels = 1;
    Wave_Format.cbSize = 0;
    Wave_Format.wBitsPerSample = 16;
    Wave_Format.nSamplesPerSec = 44100;
    Wave_Format.nBlockAlign = Wave_Format.nChannels * Wave_Format.wBitsPerSample / 8;
    Wave_Format.nAvgBytesPerSec = Wave_Format.nSamplesPerSec * Wave_Format.nBlockAlign;

    GSM_Format.wfx.wFormatTag = WAVE_FORMAT_GSM610;
    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &GSM_Format, sizeof(GSM_Format), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Pack_Stream, NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &GSM_Format, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

    Src_size = Size;
    unsigned long rawbufsize = 0;
    acmStreamSize(Pack_Stream, Src_size, &rawbufsize, ACM_STREAMSIZEF_SOURCE);
    Uint8 *Pack_Buf = (Uint8 *) malloc(Src_size + 8);
    memset(Pack_Buf, 0, Src_size + 8);
    Uint8 *rawbuf = (Uint8 *) malloc(rawbufsize + 8);
    memset(rawbuf, 0, rawbufsize + 8);

    ACMSTREAMHEADER Pack_Stream_Head;
    ZeroMemory(&Pack_Stream_Head, sizeof(ACMSTREAMHEADER));
    Pack_Stream_Head.cbStruct = sizeof(ACMSTREAMHEADER);
    Pack_Stream_Head.pbSrc = (Uint8 *) Pack_Buf;
    Pack_Stream_Head.cbSrcLength = Src_size;
    Pack_Stream_Head.pbDst = rawbuf;
    Pack_Stream_Head.cbDstLength = rawbufsize;
    acmStreamPrepareHeader(Pack_Stream, &Pack_Stream_Head, 0);

    memcpy(Pack_Buf, Source, Src_size);

    acmStreamConvert(Pack_Stream, &Pack_Stream_Head, 0);
    Dest_Size = Pack_Stream_Head.cbDstLengthUsed;
    if(Dest_Size < Src_size)
    {
        memcpy(Dest, rawbuf, Dest_Size);
    }
    else
    {
        Dest_Size = 0;
    }

    acmStreamUnprepareHeader(Pack_Stream, &Pack_Stream_Head, 0);
    if(rawbuf) free(rawbuf);
    if(Pack_Buf) free(Pack_Buf);
    acmStreamClose(Pack_Stream, 0);

    return(Dest_Size);
}
#endif

// ------------------------------------------------------
// Pack a sample to AT3
#if defined(__AT3_CODEC__)
int ToAT3(short *Source, short *Dest, int Size, int BitRate)
{
    int Src_size;
    int Dest_Size;
    int i;

    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;
    Wave_Format.nChannels = 2;
    Wave_Format.cbSize = 0;
    Wave_Format.wBitsPerSample = 16;
    Wave_Format.nSamplesPerSec = 44100;
    Wave_Format.nBlockAlign = Wave_Format.nChannels * Wave_Format.wBitsPerSample / 8;
    Wave_Format.nAvgBytesPerSec = Wave_Format.nSamplesPerSec * Wave_Format.nBlockAlign;

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
    acmStreamOpen(&Pack_Stream, NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &At3_Format, NULL, 0, 0, 0);

    short *dwSource = (short *) malloc(Size * 2 + 8);
    memset(dwSource, 0, Size * 2 + 8);
    for(i = 0; i < Size / 2; i++)
    {
        dwSource[(i * 2)] = Source[i];
        dwSource[(i * 2) + 1] = 0;
    }

    Source = dwSource;
    Size *= 2;

    Src_size = Size;
    unsigned long rawbufsize = 0;
    acmStreamSize(Pack_Stream, Src_size, &rawbufsize, ACM_STREAMSIZEF_SOURCE);
    Uint8 *Pack_Buf = (Uint8 *) malloc(Src_size + 8);
    memset(Pack_Buf, 0, Src_size + 8);
    Uint8 *rawbuf = (Uint8 *) malloc(rawbufsize + 8);
    memset(rawbuf, 0, rawbufsize + 8);

    ACMSTREAMHEADER Pack_Stream_Head;
    ZeroMemory(&Pack_Stream_Head, sizeof(ACMSTREAMHEADER));
    Pack_Stream_Head.cbStruct = sizeof(ACMSTREAMHEADER);
    Pack_Stream_Head.pbSrc = (Uint8 *) Pack_Buf;
    Pack_Stream_Head.cbSrcLength = Src_size;
    Pack_Stream_Head.pbDst = rawbuf;
    Pack_Stream_Head.cbDstLength = rawbufsize;
    acmStreamPrepareHeader(Pack_Stream, &Pack_Stream_Head, 0);

    memcpy(Pack_Buf, Source, Src_size);

    acmStreamConvert(Pack_Stream, &Pack_Stream_Head, 0);
    Dest_Size = Pack_Stream_Head.cbDstLengthUsed;
    if(Dest_Size < Src_size)
    {
        memcpy(Dest, rawbuf, Dest_Size);
    }
    else
    {
        Dest_Size = 0;
    }

    acmStreamUnprepareHeader(Pack_Stream, &Pack_Stream_Head, 0);
    if(rawbuf) free(rawbuf);
    if(Pack_Buf) free(Pack_Buf);
    if(dwSource) free(dwSource);
    acmStreamClose(Pack_Stream, 0);

    return(Dest_Size);
}
#endif

// ------------------------------------------------------
// Pack a sample to MP3

// !!! Currently BROKEN !!!

#if defined(__MP3_CODEC__)
/*BOOL CALLBACK enum_drivers(
  HACMDRIVERID hadid,
  LPACMFORMATDETAILS pafd,
  DWORD_PTR dwInstance,
  DWORD fdwSupport
)
{
    return TRUE;
}

ACMFORMATENUMCB (Acmformatenumcb2) = &enum_drivers;
ACMFORMATDETAILS acmFormatDetails2;
*/
int ToMP3(short *Source, short *Dest, int Size, int BitRate)
{
    int Src_size;
    int Dest_Size;
    int ret;
  //  BYTE *pFormat = NULL;   // Caller allocated.
//    DWORD cbMaxSize = 0;
 
    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;
    Wave_Format.nChannels = 1;
    Wave_Format.cbSize = 0;
    Wave_Format.wBitsPerSample = 16;
    Wave_Format.nSamplesPerSec = 44100;
    Wave_Format.nBlockAlign = Wave_Format.nChannels * Wave_Format.wBitsPerSample / 8;
    Wave_Format.nAvgBytesPerSec = Wave_Format.nSamplesPerSec * Wave_Format.nBlockAlign;

    MP3_Format.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
    MP3_Format.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
    MP3_Format.wfx.nChannels = 1;
    MP3_Format.wfx.nSamplesPerSec = 44100;
    MP3_Format.wfx.nAvgBytesPerSec = (BitRate * 1000) / 8;
    MP3_Format.wfx.wBitsPerSample = 0;
    MP3_Format.wfx.nBlockAlign = 1;

    MP3_Format.wID = MPEGLAYER3_ID_MPEG;
    MP3_Format.fdwFlags = MPEGLAYER3_FLAG_PADDING_OFF;
    MP3_Format.nBlockSize = 0;
    MP3_Format.nFramesPerBlock = 0;
    MP3_Format.nCodecDelay = 0;

  // Ask for WAVE_FORMAT_MPEGLAYER3 formats.
/*    ret = acmMetrics(NULL, ACM_METRIC_MAX_SIZE_FORMAT, &cbMaxSize);
 
    pFormat = new BYTE[cbMaxSize];
    ZeroMemory(pFormat, cbMaxSize);

    WAVEFORMATEX *pWaveFormat = (WAVEFORMATEX *) pFormat;
    pWaveFormat->wFormatTag = WAVE_FORMAT_MPEGLAYER3;
    pWaveFormat->nChannels = 1;
    pWaveFormat->nSamplesPerSec = 44100;
    pWaveFormat->wBitsPerSample = 16;
    pWaveFormat->nBlockAlign = (pWaveFormat->nChannels * pWaveFormat->wBitsPerSample) / 8;
    pWaveFormat->nAvgBytesPerSec = pWaveFormat->nSamplesPerSec * pWaveFormat->nBlockAlign;

    // Set up the acmFormatDetails structure.
    ZeroMemory(&acmFormatDetails2, sizeof(acmFormatDetails2));
    acmFormatDetails2.cbStruct = sizeof(ACMFORMATDETAILS);
    acmFormatDetails2.pwfx = pWaveFormat;
    acmFormatDetails2.cbwfx = cbMaxSize;

    // For the ACM_FORMATENUMF_WFORMATTAG request, the format
    // tag in acmFormatDetails must match the format tag in
    // the pFormat buffer.//
    acmFormatDetails2.dwFormatTag = WAVE_FORMAT_MPEGLAYER3;
    //
    ret = acmFormatEnum(NULL, &acmFormatDetails2, Acmformatenumcb2, 0, ACM_FORMATENUMF_WFORMATTAG);
*/
    ret = acmFormatSuggest(NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &MP3_Format, sizeof(MP3_Format), ACM_FORMATSUGGESTF_WFORMATTAG);
    ret = acmStreamOpen(&Pack_Stream, NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &MP3_Format, NULL, 0, 0, 0);

    Src_size = Size;
    unsigned long rawbufsize = 0;
    acmStreamSize(Pack_Stream, Src_size, &rawbufsize, ACM_STREAMSIZEF_SOURCE);
    rawbufsize += MP3_FRAMES_LAG * 2;
    Uint8 *Pack_Buf = (Uint8 *) malloc(Src_size + (MP3_FRAMES_LAG * 4) + 8);
    memset(Pack_Buf, 0, Src_size + (MP3_FRAMES_LAG * 4) + 8);
    Uint8 *rawbuf = (Uint8 *) malloc(rawbufsize + (MP3_FRAMES_LAG * 4) + 8);
    memset(rawbuf, 0, rawbufsize + (MP3_FRAMES_LAG * 4) + 8);

    ACMSTREAMHEADER Pack_Stream_Head;
    ZeroMemory(&Pack_Stream_Head, sizeof(ACMSTREAMHEADER));
    Pack_Stream_Head.cbStruct = sizeof(ACMSTREAMHEADER);
    Pack_Stream_Head.pbSrc = (Uint8 *) Pack_Buf;
    Pack_Stream_Head.cbSrcLength = Src_size + (MP3_FRAMES_LAG * 2);
    Pack_Stream_Head.pbDst = rawbuf;
    Pack_Stream_Head.cbDstLength = rawbufsize;
    acmStreamPrepareHeader(Pack_Stream, &Pack_Stream_Head, 0);

    memcpy(Pack_Buf, Source, Src_size);

    acmStreamConvert(Pack_Stream, &Pack_Stream_Head, 0);
    Dest_Size = Pack_Stream_Head.cbDstLengthUsed;
    if(Dest_Size < Src_size)
    {
        memcpy(Dest, rawbuf, Dest_Size);
    }
    else
    {
        Dest_Size = 0;
    }

    acmStreamUnprepareHeader(Pack_Stream, &Pack_Stream_Head, 0);
    acmStreamClose(Pack_Stream, 0);
//    delete [] pFormat;
    if(rawbuf) free(rawbuf);
    if(Pack_Buf) free(Pack_Buf);

    return(Dest_Size);
}
#endif

// ------------------------------------------------------
// Pack a sample to ADPCM
static int pack_stepsizeTable[] =
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

/* Intel ADPCM step variation table */
int pack_indextable[16] =
{
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};

int ToADPCM(short *indata, short *outdata, int len)
{
    short *inp;			/* Input buffer pointer */
    signed char *outp;  /* output buffer pointer */
    int val;			/* Current input sample value */
    int sign;			/* Current adpcm sign bit */
    int delta;			/* Current adpcm output value */
    int diff;			/* Difference between val and valprev */
    int step;			/* Stepsize */
    int valpred;		/* Predicted output value */
    int vpdiff;			/* Current change to valpred */
    int index;			/* Current step change index */
    int outputbuffer;   /* place to keep previous 4-bit value */
    int bufferstep;		/* toggle between outputbuffer/output */
    int ret_len = 0;
    outp = (signed char *) outdata;
    inp = indata;

    valpred = 0;
    index = 0;
    step = pack_stepsizeTable[index];

    bufferstep = 1;

    for(; len > 0; len--)
    {
		val = (*inp++) ;
		//if(val & 0x8000) val |= 0xffff0000;
		/* Step 1 - compute difference with previous value */
		/* Step 4 - Clamp previous value to 16 bits */

		diff = val - valpred;
		sign = (diff < 0) ? 8 : 0;
		if(sign) diff = (-diff);

		/* Step 2 - Divide and clamp */
		/* Note:
		** This code *approximately* computes:
		**    delta = diff*4/step;
		**    vpdiff = (delta+0.5)*step/4;
		** but in shift step bits are dropped. The net result of this is
		** that even if you have fast mul/div hardware you cannot put it to
		** good use since the fixup would be too expensive.
		*/
		delta = 0;
		vpdiff = (step >> 3);

		if(diff >= step)
        {
			delta = 4;
			diff -= step;
			vpdiff += step;
		}
		step >>= 1;
		if(diff >= step)
        {
			delta |= 2;
			diff -= step;
			vpdiff += step;
		}
		step >>= 1;
		if(diff >= step)
        {
			delta |= 1;
			vpdiff += step;
		}

		/* Step 3 - Update previous value */
		if(sign) valpred -= vpdiff;
		else valpred += vpdiff;

		if(valpred > 32767) valpred = 32767;
		else if(valpred < -32768) valpred = -32768;

		/* Step 5 - Assemble value, update index and step values */
		delta |= sign;

		index += pack_indextable[delta];
		if(index < 0) index = 0;
		if(index > 88) index = 88;
		step = pack_stepsizeTable[index];

		if(bufferstep)
        {
            outputbuffer = (delta << 4) & 0xf0;
        }
        else
        {
            *outp++ = (delta & 0x0f) | outputbuffer;
            ret_len++;
	    }
        bufferstep = !bufferstep;
    }

    /* Output last step, if needed */
    if(!bufferstep)
    {
		*outp++ = outputbuffer;
		ret_len++;
	}
	return(ret_len);
}

// ------------------------------------------------------
// Pack a sample to 8 Bit
int To8Bit(short *Source, short *Dest, int Size)
{
    int i;
    unsigned char *bDest = (unsigned char *) Dest;

    for(i = 0; i < Size; i++)
    {
        bDest[i] = Source[i] >> 8;
    }

    return(Size);
}

// ------------------------------------------------------
// Pack a sample to wavpack format
int ToWavPack(short *Source, short *Dest, int Size)
{
    int Dest_Size = pack_audio(Source, Dest, Size);
    return Dest_Size >= Size ? 0 : Dest_Size;
}
