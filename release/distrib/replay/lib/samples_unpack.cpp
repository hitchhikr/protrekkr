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
#if defined(PTK_TRUESPEECH)
TRUESPEECHWAVEFORMAT TrueSpeech_Format;
#endif
#if defined(PTK_ADPCM)
IMAADPCMWAVEFORMAT ADPCM_Format;
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
#if defined(__TRUESPEECH_CODEC__)
extern TRUESPEECHWAVEFORMAT TrueSpeech_Format;
#else
#undef PTK_TRUESPEECH
#endif
#if defined(__ADPCM_CODEC__)
extern IMAADPCMWAVEFORMAT ADPCM_Format;
#else
#undef PTK_ADPCM
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
void UnpackGSM(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
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
void UnpackAT3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate)
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
// Unpack a TrueSpeech sample
#if defined(PTK_TRUESPEECH)
void UnpackTrueSpeech(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
{
    int i;
    int Real_Size;

    memset(&Wave_Format, 0, sizeof(WAVEFORMATEX));
    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;

    TrueSpeech_Format.wfx.wFormatTag = WAVE_FORMAT_DSPGROUP_TRUESPEECH;
    TrueSpeech_Format.wfx.nChannels = 1;
    TrueSpeech_Format.wfx.nSamplesPerSec = 0x1f40;
    TrueSpeech_Format.wfx.nAvgBytesPerSec = 0x42b;
    TrueSpeech_Format.wfx.nBlockAlign = 0x20;
    TrueSpeech_Format.wfx.wBitsPerSample = 1;
    TrueSpeech_Format.wfx.cbSize = 0x20;
    TrueSpeech_Format.wRevision = 1;
    TrueSpeech_Format.nSamplesPerBlock = 0xf0;

    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &TrueSpeech_Format, (LPWAVEFORMATEX) &Wave_Format, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Unpack_Stream, NULL, (LPWAVEFORMATEX) &TrueSpeech_Format, (LPWAVEFORMATEX) &Wave_Format, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

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
        Dest[i] = dwDest[i + 0x3e];
    }

    if(dwDest) free(dwDest);
    acmStreamUnprepareHeader(Unpack_Stream, &Unpack_Stream_Head, 0);
    acmStreamClose(Unpack_Stream, 0);
}
#endif

// ------------------------------------------------------
// Unpack a MP3 sample
#if defined(PTK_MP3)
void UnpackMP3(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size, int BitRate)
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
void UnpackADPCM(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
{
    int i;
    int Real_Size;

    memset(&Wave_Format, 0, sizeof(WAVEFORMATEX));
    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;

    ADPCM_Format.wfx.wFormatTag = WAVE_FORMAT_IMA_ADPCM;
    ADPCM_Format.wfx.nChannels = 1;
    ADPCM_Format.wfx.nSamplesPerSec = 44100;
    ADPCM_Format.wfx.nAvgBytesPerSec = 0x566d;
    ADPCM_Format.wfx.nBlockAlign = 0x400;
    ADPCM_Format.wfx.wBitsPerSample = 4;
    ADPCM_Format.wfx.cbSize = 2;
    ADPCM_Format.wSamplesPerBlock = 0x7f9;
    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &ADPCM_Format, (LPWAVEFORMATEX) &Wave_Format, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Unpack_Stream, NULL, (LPWAVEFORMATEX) &ADPCM_Format, (LPWAVEFORMATEX) &Wave_Format, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

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
// Unpack a Bit 8 sample
#if defined(PTK_8BIT)
void Unpack8Bit(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
{
    int i;

    for(i = 0; i < Src_Size; i++)
    {
        Dest[i] = Source[i] << 8;
    }
}
#endif
