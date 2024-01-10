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
#include "include/sounddriver_windows.h"

#define DIRECTSOUND_VERSION 0x0900
#if defined(__GCC__)
#include "include/DSound.h"
#else
#include <DSound.h>
#endif

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples;
int AUDIO_Play_Flag;
float AUDIO_Timer;

int volatile AUDIO_Acknowledge;

int AUDIO_SoundBuffer_Size;

LPDIRECTSOUND AUDIO_DSound_Context;
DSBUFFERDESC AUDIO_Sound_Buffer_Desc;
LPDIRECTSOUNDBUFFER AUDIO_Sound_Buffer;
WAVEFORMATEX AUDIO_Wave_Format;

DWORD AUDIO_Buffer_Pos;
DWORD AUDIO_Old_Buffer_Pos;
LPVOID AUDIO_Audio_Ptr1;
DWORD AUDIO_Audio_Bytes1;
LPVOID AUDIO_Audio_Ptr2;
DWORD AUDIO_Audio_Bytes2;
DWORD AUDIO_ThreadId;
HANDLE AUDIO_hReplayThread;

int AUDIO_Latency;

int AUDIO_Milliseconds = 20;

// ------------------------------------------------------
// Functions
int AUDIO_Create_Sound_Buffer(int milliseconds);
void AUDIO_Stop_Sound_Buffer(void);
void (STDCALL *AUDIO_Mixer)(Uint8 *, Uint32);
void AUDIO_Mixer_Fill_Buffer(void *, Uint32);
void AUDIO_Synth_Play(void);

// ------------------------------------------------------
// Name: AUDIO_Thread()
// Desc: Audio rendering
DWORD WINAPI AUDIO_Thread(LPVOID lpParameter)
{
    int Bytes_To_Lock;

    for(;;)
    {
        if(AUDIO_Sound_Buffer)
        {
            AUDIO_Acknowledge = FALSE;

            AUDIO_Sound_Buffer->GetCurrentPosition(&AUDIO_Buffer_Pos, NULL);
            Bytes_To_Lock = AUDIO_Buffer_Pos - AUDIO_Old_Buffer_Pos;
            if(Bytes_To_Lock < 0) Bytes_To_Lock += AUDIO_Latency;
            AUDIO_Sound_Buffer->Lock(AUDIO_Old_Buffer_Pos, Bytes_To_Lock, &AUDIO_Audio_Ptr1, &AUDIO_Audio_Bytes1, &AUDIO_Audio_Ptr2, &AUDIO_Audio_Bytes2, 0);
            AUDIO_Old_Buffer_Pos = AUDIO_Buffer_Pos;

            if(AUDIO_Play_Flag)
            {
                AUDIO_Mixer((Uint8 *) AUDIO_Audio_Ptr1, AUDIO_Audio_Bytes1);
                AUDIO_Mixer((Uint8 *) AUDIO_Audio_Ptr2, AUDIO_Audio_Bytes2);
            }
            else
            {
                unsigned int i;
                char *pSamples = (char *) AUDIO_Audio_Ptr1;
                for(i = 0; i < AUDIO_Audio_Bytes1; i++)
                {
                    pSamples[i] = 0;
                }
                pSamples = (char *) AUDIO_Audio_Ptr2;
                for(i = 0; i < AUDIO_Audio_Bytes2; i++)
                {
                    pSamples[i] = 0;
                }
                AUDIO_Acknowledge = TRUE;
            }

            AUDIO_Sound_Buffer->Unlock(AUDIO_Audio_Ptr1, AUDIO_Audio_Bytes1, AUDIO_Audio_Ptr2, AUDIO_Audio_Bytes2);

            AUDIO_Samples += (AUDIO_Audio_Bytes1 + AUDIO_Audio_Bytes2);
            AUDIO_Timer = ((((float) AUDIO_Samples) * (1.0f / (float) AUDIO_Latency)) * 1000.0f);
        }
        Sleep(10);
    }
    return(FALSE);
}

// ------------------------------------------------------
// Name: AUDIO_Init_Driver()
// Desc: Init the audio driver
int AUDIO_Init_Driver(HWND hWnd, void (STDCALL *Mixer)(Uint8 *, Uint32))
{
    AUDIO_Mixer = Mixer;

    if(DirectSoundCreate(NULL, &AUDIO_DSound_Context, NULL) == DS_OK)
    {
        if(AUDIO_DSound_Context->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE) == DS_OK)
        {
            return(AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds));
        }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        else
        {
            Message_Error("Error while calling AUDIO_DSound_Context->SetCooperativeLevel()");
        }
#endif

    }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    else
    {
        Message_Error("Error while calling DirectSoundCreate()");
    }
#endif

    return(FALSE);
}

// ------------------------------------------------------
// Name: AUDIO_Create_Sound_Buffer()
// Desc: Create an audio buffer of given milliseconds
int AUDIO_Create_Sound_Buffer(int milliseconds)
{
    int num_fragments;
    int frag_size;

    if(milliseconds < 10) milliseconds = 10;
    if(milliseconds > 250) milliseconds = 250;

    num_fragments = 6;
    frag_size = (int) (AUDIO_PCM_FREQ * (milliseconds / 1000.0f));

    AUDIO_Sound_Buffer_Desc.dwSize = sizeof(AUDIO_Sound_Buffer_Desc);
    AUDIO_Sound_Buffer_Desc.dwFlags = DSBCAPS_GLOBALFOCUS;

    AUDIO_Wave_Format.wFormatTag = WAVE_FORMAT_PCM;
    // Stereo
    AUDIO_Wave_Format.nChannels = AUDIO_DBUF_CHANNELS;
    AUDIO_Wave_Format.nSamplesPerSec = AUDIO_PCM_FREQ;
    AUDIO_Wave_Format.nAvgBytesPerSec = (AUDIO_PCM_FREQ * ((AUDIO_DBUF_RESOLUTION * AUDIO_DBUF_CHANNELS) >> 3));
    AUDIO_Wave_Format.nBlockAlign = ((AUDIO_DBUF_RESOLUTION * AUDIO_DBUF_CHANNELS) >> 3);
    AUDIO_Sound_Buffer_Desc.dwBufferBytes = frag_size * AUDIO_Wave_Format.nBlockAlign * num_fragments;
    AUDIO_Latency = AUDIO_Sound_Buffer_Desc.dwBufferBytes;
    // 16 bits
    AUDIO_Wave_Format.wBitsPerSample = AUDIO_DBUF_RESOLUTION;
    AUDIO_Sound_Buffer_Desc.lpwfxFormat = &AUDIO_Wave_Format;
    if(AUDIO_DSound_Context->CreateSoundBuffer(&AUDIO_Sound_Buffer_Desc, &AUDIO_Sound_Buffer, NULL) == DS_OK)
    {
        AUDIO_hReplayThread = CreateThread(NULL, 0, &AUDIO_Thread, 0, 0, &AUDIO_ThreadId);
        SetThreadPriority(AUDIO_hReplayThread, THREAD_PRIORITY_TIME_CRITICAL);
        AUDIO_Sound_Buffer->Play(0, 0, DSBPLAY_LOOPING);
        return(TRUE);
    }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    Message_Error("Error while calling AUDIO_DSound_Context->CreateSoundBuffer()");
#endif

    return(FALSE);
}

// ------------------------------------------------------
// Name: AUDIO_Wait_For_Thread()
// Desc: Wait for a command acknowledgment from the thread
void AUDIO_Wait_For_Thread(void)
{
    if(AUDIO_Sound_Buffer)
    {
        if(AUDIO_Play_Flag)
        {
            while(AUDIO_Acknowledge)
            {
                Sleep(10);
            };
        }
        else
        {
            while(!AUDIO_Acknowledge)
            {
                Sleep(10);
            };
        }
    }
}

// ------------------------------------------------------
// Name: AUDIO_Play()
// Desc: Play the sound buffer endlessly
void AUDIO_Play(void)
{
    AUDIO_ResetTimer();
    AUDIO_Play_Flag = TRUE;
    AUDIO_Wait_For_Thread();
}

// ------------------------------------------------------
// Name: AUDIO_IsPlaying()
// Desc: Return the playing state of the sound buffer
int AUDIO_IsPlaying(void)
{
    return(AUDIO_Play_Flag);
}

// ------------------------------------------------------
// Name: AUDIO_ResetTimer()
// Desc: Reset the samples counter
void AUDIO_ResetTimer(void)
{
    AUDIO_Samples = 0;
    AUDIO_Timer = 0.0f;
}

// ------------------------------------------------------
// Name: AUDIO_GetTime()
// Desc: Return the played time in milliseconds
float AUDIO_GetTime(void)
{
    return(AUDIO_Timer);
}

// ------------------------------------------------------
// Name: AUDIO_GetSamples()
// Desc: Return the played time in samples
int AUDIO_GetSamples(void)
{
    return(AUDIO_Samples);
}

// ------------------------------------------------------
// Name: AUDIO_Stop()
// Desc: Stop the sound buffer
void AUDIO_Stop(void)
{
    AUDIO_Play_Flag = FALSE;
    AUDIO_Wait_For_Thread();
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Sound_Buffer()
// Desc: Release the audio buffer
void AUDIO_Stop_Sound_Buffer(void)
{
    AUDIO_Stop();
    if(AUDIO_Sound_Buffer) AUDIO_Sound_Buffer->Stop();
    if(AUDIO_hReplayThread)
    {
        TerminateThread(AUDIO_hReplayThread, 0);
        CloseHandle(AUDIO_hReplayThread);
        AUDIO_hReplayThread = 0;
    }
    if(AUDIO_Sound_Buffer)
    {
        AUDIO_Sound_Buffer->Release();
        AUDIO_Sound_Buffer = NULL;
    }
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();
    if(AUDIO_DSound_Context) AUDIO_DSound_Context->Release();
}
