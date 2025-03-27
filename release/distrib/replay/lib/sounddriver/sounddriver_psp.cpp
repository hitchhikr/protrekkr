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
#include "include/sounddriver_psp.h"

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples __attribute__((aligned(64)));
int AUDIO_Play_Flag __attribute__((aligned(64)));
float AUDIO_Timer __attribute__((aligned(64)));

int volatile AUDIO_Acknowledge;

SceUID AUDIO_thid;
int AUDIO_HWChannel;
int AUDIO_SoundBuffer_Size;
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
short *ptrAudio_BufferPlay1;
short *ptrAudio_BufferPlay2;
int AUDIO_FlipFlop = FALSE;

SceInt32 AUDIO_Thread(SceSize args, ScePVoid argp)
{
    volatile int done;

    done = FALSE;
    for(;;)
    {
        volatile short *ptrBuffer;

        if(AUDIO_FlipFlop)
        {
            ptrBuffer = ptrAudio_BufferPlay1;
        }
        else
        {
            ptrBuffer = ptrAudio_BufferPlay2;
        }
        if(AUDIO_Play_Flag)
        {
            if(sceAudioGetChannelRestLen(AUDIO_HWChannel) <= 0)
            {
                sceAudioOutput(AUDIO_HWChannel, PSP_AUDIO_VOLUME_MAX, (void *) ptrBuffer);
                AUDIO_FlipFlop ^= TRUE;
                done = FALSE;
                AUDIO_Samples += AUDIO_SoundBuffer_Size;
                AUDIO_Timer = ((((float) AUDIO_Samples) * (1.0f / (float) AUDIO_Latency)) * 1000.0f);
            }
            else
            {
                if(done == FALSE)
                {
                    done = TRUE;
                    AUDIO_Mixer((Uint8 *) ptrBuffer, AUDIO_SoundBuffer_Size);
                }
            }
        }
        else
        {
            memset(ptrBuffer, 0, AUDIO_SoundBuffer_Size);
        }
        sceKernelDelayThread(10);
    }
    return(0);
}

// ------------------------------------------------------
// Name: AUDIO_Init_Driver()
// Desc: Init the audio driver
int AUDIO_Init_Driver(void (*Mixer)(Uint8 *, Uint32))
{
    AUDIO_Mixer = Mixer;
    return(AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds));
}

// ------------------------------------------------------
// Name: AUDIO_malloc_64()
// Desc: Allocate aligned memory
void *AUDIO_malloc_64(int *size)
{
    int mod_64 = *size & 0x3f;
    if(mod_64 != 0) *size += 64 - mod_64;
    return((void *) memalign(64, *size));
}

// ------------------------------------------------------
// Name: AUDIO_Create_Sound_Buffer()
// Desc: Create an audio buffer of given milliseconds
int AUDIO_Create_Sound_Buffer(int milliseconds)
{
    int frag_size;

    if(milliseconds < 10) milliseconds = 10;
    if(milliseconds > 250) milliseconds = 250;

    frag_size = (int) (AUDIO_PCM_FREQ * (milliseconds / 1000.0f));

    AUDIO_SoundBuffer_Size = frag_size * ((AUDIO_DBUF_RESOLUTION * AUDIO_DBUF_CHANNELS) >> 3);
    AUDIO_SoundBuffer_Size = PSP_AUDIO_SAMPLE_ALIGN(AUDIO_SoundBuffer_Size);

    AUDIO_HWChannel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, AUDIO_SoundBuffer_Size, PSP_AUDIO_FORMAT_STEREO);
    AUDIO_SoundBuffer_Size <<= 2;

    AUDIO_Latency = AUDIO_SoundBuffer_Size;
    int buf_size = AUDIO_SoundBuffer_Size;
    ptrAudio_BufferPlay1 = (short *) (((int) AUDIO_malloc_64(&buf_size)));
    sceKernelDcacheWritebackInvalidateAll();

    if((int) ptrAudio_BufferPlay1)
    {
        memset((void *) ptrAudio_BufferPlay1, 0, buf_size);
        ptrAudio_BufferPlay2 = (short *) (((int) AUDIO_malloc_64(&buf_size)));
        sceKernelDcacheWritebackInvalidateAll();

        if((int) ptrAudio_BufferPlay2)
        {
            memset((void *) ptrAudio_BufferPlay2, 0, buf_size);
            AUDIO_thid = sceKernelCreateThread("Ptk", AUDIO_Thread, AUDIO_THREAD_PRIORITY, AUDIO_THREAD_STACKSIZE, PSP_THREAD_ATTR_VFPU, NULL);
            if(AUDIO_thid > 0)
            {
                sceKernelStartThread(AUDIO_thid, 0, NULL);
                return(TRUE);
            }
        }
    }

    return(FALSE);
}

// ------------------------------------------------------
// Name: AUDIO_Play()
// Desc: Play the sound buffer endlessly
void AUDIO_Play(void)
{
    AUDIO_ResetTimer();
    AUDIO_Play_Flag = TRUE;
    sceKernelDcacheWritebackInvalidateAll();
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
    sceKernelDcacheWritebackInvalidateAll();
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
// Desc: Return the played time in milliseconds
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
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Sound_Buffer()
// Desc: Release the audio buffer
void AUDIO_Stop_Sound_Buffer(void)
{
    AUDIO_Stop();
    if(AUDIO_thid > 0) sceKernelDeleteThread(AUDIO_thid);
    AUDIO_thid = NULL;
    if(AUDIO_HWChannel) sceAudioChRelease(AUDIO_HWChannel);
    AUDIO_HWChannel = NULL;
    if(ptrAudio_BufferPlay1) free((void *) ptrAudio_BufferPlay1);
    ptrAudio_BufferPlay1 = NULL;
    if(ptrAudio_BufferPlay2) free((void *) ptrAudio_BufferPlay2);
    ptrAudio_BufferPlay2 = NULL;
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();
}
