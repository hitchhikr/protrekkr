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
#if defined(PTK_TRUESPEECH)
TRUESPEECHWAVEFORMAT TrueSpeech_Format;
#endif
#if defined(PTK_ADPCM)
IMAADPCMWAVEFORMAT ADPCM_Format;
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
#if defined(__TRUESPEECH_CODEC__)
TRUESPEECHWAVEFORMAT TrueSpeech_Format;
#endif
#if defined(__ADPCM_CODEC__)
IMAADPCMWAVEFORMAT ADPCM_Format;
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
#if defined(__MP3_CODEC__)
int ToMP3(short *Source, short *Dest, int Size, int BitRate)
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

    MP3_Format.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
    MP3_Format.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
    MP3_Format.wfx.nChannels = 1;
    MP3_Format.wfx.nSamplesPerSec = 44100;
    MP3_Format.wfx.nAvgBytesPerSec = BitRate * (1000 / 8);
    MP3_Format.wfx.wBitsPerSample = 0;
    MP3_Format.wfx.nBlockAlign = 1;
    MP3_Format.wID = MPEGLAYER3_ID_MPEG;
    MP3_Format.fdwFlags = MPEGLAYER3_FLAG_PADDING_OFF;
    MP3_Format.nBlockSize = 0;
    MP3_Format.nFramesPerBlock = 0;
    MP3_Format.nCodecDelay = 0;

    acmStreamOpen(&Pack_Stream, NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &MP3_Format, NULL, 0, 0, 0);

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
    if(rawbuf) free(rawbuf);
    if(Pack_Buf) free(Pack_Buf);

    return(Dest_Size);
}
#endif

// ------------------------------------------------------
// Pack a sample to TrueSpeech
#if defined(__TRUESPEECH_CODEC__)
int ToTrueSpeech(short *Source, short *Dest, int Size)
{
    int Src_size;
    int Dest_Size;

    Wave_Format.wFormatTag = WAVE_FORMAT_PCM;
    Wave_Format.nChannels = 1;
    Wave_Format.cbSize = 0;
    Wave_Format.wBitsPerSample = 16;
    Wave_Format.nSamplesPerSec = 8000;
    Wave_Format.nBlockAlign = Wave_Format.nChannels * Wave_Format.wBitsPerSample / 8;
    Wave_Format.nAvgBytesPerSec = Wave_Format.nSamplesPerSec * Wave_Format.nBlockAlign;

    TrueSpeech_Format.wfx.wFormatTag = WAVE_FORMAT_DSPGROUP_TRUESPEECH;
    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &TrueSpeech_Format, sizeof(TrueSpeech_Format), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Pack_Stream, NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &TrueSpeech_Format, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

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
// Pack a sample to ADPCM
#if defined(__ADPCM_CODEC__)
int ToADPCM(short *Source, short *Dest, int Size)
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

    ADPCM_Format.wfx.wFormatTag = WAVE_FORMAT_IMA_ADPCM;
    acmFormatSuggest(NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &ADPCM_Format, sizeof(TrueSpeech_Format), ACM_FORMATSUGGESTF_WFORMATTAG);
    acmStreamOpen(&Pack_Stream, NULL, (LPWAVEFORMATEX) &Wave_Format, (LPWAVEFORMATEX) &ADPCM_Format, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME);

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
// Pack a sample to internal format
int ToInternal(short *Source, short *Dest, int Size)
{
    int Dest_Size = pack_audio(Source, Dest, Size);
    return Dest_Size >= Size ? 0 : Dest_Size;
}
