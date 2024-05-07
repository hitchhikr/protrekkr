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
#include "include/sounddriver_haiku.h"
#include <game/StreamingGameSound.h>
#include <media/MediaDefs.h>

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples;
int AUDIO_Play_Flag;
float AUDIO_Timer;

int volatile AUDIO_Acknowledge;
BStreamingGameSound *AUDIO_Device;
Uint32 Amount;

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
static void AUDIO_Callback(void *cookie, void *inBuffer, size_t byteCount, BStreamingGameSound *device)
{
    if(AUDIO_Play_Flag)
    {
        AUDIO_Mixer((Uint8 *) inBuffer, byteCount);
    }
    else
    {
        unsigned int i;
        char *pSamples = (char *) inBuffer;
        for(i = 0; i < byteCount; i++)
        {
            pSamples[i] = 0;
        }
    }
    AUDIO_Samples += byteCount;
    AUDIO_Timer = ((((float) AUDIO_Samples) * (1.0f / (float) AUDIO_Latency)) * 1000.0f);
}

// ------------------------------------------------------
// Name: AUDIO_Init_Driver()
// Desc: Init the audio driver
int AUDIO_Init_Driver(void (*Mixer)(Uint8 *, Uint32))
{
    AUDIO_Mixer = Mixer;

    AUDIO_Device = 0;
    return(AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds));
}

// ------------------------------------------------------
// Name: AUDIO_Create_Sound_Buffer()
// Desc: Create an audio buffer of given milliseconds
int AUDIO_Create_Sound_Buffer(int milliseconds)
{
    int num_fragments;
    int frag_size;
    int found_pcmformat = TRUE;

    if(milliseconds < 10) milliseconds = 10;
    if(milliseconds > 250) milliseconds = 250;

    num_fragments = 6;
    frag_size = (int) (AUDIO_PCM_FREQ * (milliseconds / 1000.0f));

	struct gs_audio_format format;
	format.frame_rate = AUDIO_PCM_FREQ;
	format.channel_count = AUDIO_DBUF_CHANNELS;
	format.format = gs_audio_format::B_GS_S16;
	format.byte_order = B_MEDIA_LITTLE_ENDIAN;
	format.buffer_size = 4096 * num_fragments;

	AUDIO_Device = new BStreamingGameSound(4096, &format);
	if(AUDIO_Device->InitCheck() != B_OK)
    {
        return(FALSE);
    }

    AUDIO_SoundBuffer_Size = format.buffer_size;

    AUDIO_Latency = AUDIO_SoundBuffer_Size;
	if(AUDIO_Latency == 0)
    {
        return(FALSE);
    }

	AUDIO_Device->SetStreamHook(&AUDIO_Callback, NULL);
	AUDIO_Device->StartPlaying();
    return(TRUE);
}

// ------------------------------------------------------
// Name: AUDIO_Wait_For_Thread()
// Desc: Wait for a command acknowledgment from the thread
void AUDIO_Wait_For_Thread(void)
{
    usleep(10);
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
    AUDIO_Wait_For_Thread();
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Sound_Buffer()
// Desc: Release the audio buffer
void AUDIO_Stop_Sound_Buffer(void)
{
    if(AUDIO_Device)
    {
        AUDIO_Stop();
        AUDIO_Device->StopPlaying();
        AUDIO_Wait_For_Thread();
        delete AUDIO_Device;
        AUDIO_Device = NULL;
    }
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();
}
