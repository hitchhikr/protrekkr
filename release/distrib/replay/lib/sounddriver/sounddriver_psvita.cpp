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
#include "include/sounddriver_psvita.h"

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples;
int AUDIO_Play_Flag;
float AUDIO_Timer;

SceUID AUDIO_thid;
int AUDIO_HWChannel;
int AUDIO_SoundBuffer_Size;
int AUDIO_Latency;
int AUDIO_Milliseconds = 10;

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
short *AUDIO_SoundBuffer[2];
int AUDIO_FlipFlop = 0;

SceInt32 AUDIO_Thread(SceSize args, ScePVoid argp)
{
	int volumes[2] = { SCE_AUDIO_VOLUME_0DB, SCE_AUDIO_VOLUME_0DB };

    for(;;)
    {
        if(AUDIO_SoundBuffer[0])
        {
            if(AUDIO_Play_Flag)
            {
                AUDIO_Mixer((Uint8 *) AUDIO_SoundBuffer[AUDIO_FlipFlop], AUDIO_SoundBuffer_Size);
            }
            else
            {
                memset(AUDIO_SoundBuffer[AUDIO_FlipFlop], 0, AUDIO_SoundBuffer_Size);
            }
            sceAudioOutOutput(AUDIO_HWChannel, (void *) AUDIO_SoundBuffer[AUDIO_FlipFlop]);
            sceAudioOutSetVolume(AUDIO_HWChannel, (SceAudioOutChannelFlag) (SCE_AUDIO_VOLUME_FLAG_L_CH | SCE_AUDIO_VOLUME_FLAG_R_CH), volumes);
            AUDIO_FlipFlop ^= 1;

            AUDIO_Samples += AUDIO_SoundBuffer_Size;
            AUDIO_Timer = ((((float) AUDIO_Samples) * (1.0f / (float) AUDIO_Latency)) * 1000.0f);
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
// Name: PSVITA_malloc()
// Desc: Allocate a memory block
#define ALIGN(x, a)	(((x) + ((a) - 1)) & ~((a) - 1))
void *PSVITA_malloc(int size)
{
    SceUID ret;
    int *mem_block;

    size = ALIGN(size + 4, 4096);
	ret = sceKernelAllocMemBlock("", SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_NC_RW, size, 0);
    if(ret < 0) return(NULL);
    sceKernelGetMemBlockBase(ret, (void **) &mem_block);
    memset((void *) mem_block, 0, size);
    mem_block[0] = ret;
    return((void *) &mem_block[1]);
}

// ------------------------------------------------------
// Name: PSVITA_free()
// Desc: Free a memory block
void PSVITA_free(void *mem_block)
{
    int *block = (int *) mem_block;
    if(block)
    {
        sceKernelFreeMemBlock(block[-1]);
    }
}

// ------------------------------------------------------
// Name: AUDIO_Create_Sound_Buffer()
// Desc: Create an audio buffer of given milliseconds
int AUDIO_Create_Sound_Buffer(int milliseconds)
{
    int frag_size;
	int volumes[2] = { 0, 0 };

    if(milliseconds < 10) milliseconds = 10;
    if(milliseconds > 250) milliseconds = 250;

    frag_size = (int) (AUDIO_PCM_FREQ * (milliseconds / 1000.0f));

    AUDIO_SoundBuffer_Size = frag_size * ((AUDIO_DBUF_RESOLUTION * AUDIO_DBUF_CHANNELS) >> 3);
    AUDIO_SoundBuffer_Size = AUDIO_SAMPLE_ALIGN(AUDIO_SoundBuffer_Size);

    AUDIO_HWChannel = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, AUDIO_SoundBuffer_Size, AUDIO_PCM_FREQ, SCE_AUDIO_OUT_MODE_STEREO);
    sceAudioOutSetVolume(AUDIO_HWChannel, (SceAudioOutChannelFlag) (SCE_AUDIO_VOLUME_FLAG_L_CH | SCE_AUDIO_VOLUME_FLAG_R_CH), volumes);
    AUDIO_SoundBuffer_Size <<= 2;

    AUDIO_Latency = AUDIO_SoundBuffer_Size;
	AUDIO_SoundBuffer[0] = (short *) PSVITA_malloc(AUDIO_SoundBuffer_Size);
    if(AUDIO_SoundBuffer[0])
    {
        AUDIO_SoundBuffer[1] = (short *) PSVITA_malloc(AUDIO_SoundBuffer_Size);
        if(AUDIO_SoundBuffer[1])
        {
            AUDIO_thid = sceKernelCreateThread("Ptk", AUDIO_Thread, AUDIO_THREAD_PRIORITY, AUDIO_THREAD_STACKSIZE, 0, SCE_KERNEL_CPU_MASK_USER_ALL, NULL);
            if(AUDIO_thid >= 0)
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
    if(AUDIO_HWChannel) sceAudioOutReleasePort(AUDIO_HWChannel);
    AUDIO_HWChannel = NULL;
    if(AUDIO_SoundBuffer[0]) PSVITA_free((void *) AUDIO_SoundBuffer[0]);
    if(AUDIO_SoundBuffer[1]) PSVITA_free((void *) AUDIO_SoundBuffer[1]);
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();
}
