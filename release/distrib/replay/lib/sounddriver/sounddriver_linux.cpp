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
#include "include/sounddriver_linux.h"

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples;
int AUDIO_Play_Flag;
float AUDIO_Timer;

int volatile AUDIO_Acknowledge;
int AUDIO_Device;
short *AUDIO_SoundBuffer;
pthread_t hThread;
int volatile Thread_Running;
int AUDIO_SoundBuffer_Size;

snd_pcm_t *playback_handle;
snd_pcm_sframes_t latency;

int AUDIO_Latency;
int AUDIO_Milliseconds = 40;

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
void *AUDIO_Thread(void *arg)
{
    int32_t volatile insize;
    int32_t volatile frames_to_deliver;

    while(Thread_Running)
    {
        if(AUDIO_SoundBuffer)
        {
            AUDIO_Acknowledge = FALSE;
            if(AUDIO_Play_Flag)
            {
                AUDIO_Mixer((Uint8 *) AUDIO_SoundBuffer, AUDIO_SoundBuffer_Size);
            }
            else
            {
                memset(AUDIO_SoundBuffer, 0, AUDIO_SoundBuffer_Size);
                AUDIO_Acknowledge = TRUE;
            }

            snd_pcm_writei(playback_handle, AUDIO_SoundBuffer, AUDIO_SoundBuffer_Size >> 2);
            
            AUDIO_Samples += AUDIO_SoundBuffer_Size;
            AUDIO_Timer = ((((float) AUDIO_Samples) * (1.0f / (float) AUDIO_Latency)) * 1000.0f);
        }
        usleep(10);
    }
    Thread_Running = 1;
    pthread_exit(0);
    return(0);
}

// ------------------------------------------------------
// Name: AUDIO_Init_Driver()
// Desc: Init the audio driver
int AUDIO_Init_Driver(void (*Mixer)(Uint8 *, Uint32))
{
    struct sched_param p;

    AUDIO_Mixer = Mixer;

    p.sched_priority = 1;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &p);

    return(AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds));
}

// ------------------------------------------------------
// Name: AUDIO_Create_Sound_Buffer()
// Desc: Create an audio buffer of given milliseconds
int AUDIO_Create_Sound_Buffer(int milliseconds)
{
    const char *device;
    unsigned int frag_size;
    unsigned int bitrate = AUDIO_PCM_FREQ;

    device = getenv("ALSA_DEVICE");
    if(!device)
    {
        device = "default";
    }

    if(snd_pcm_open(&playback_handle, device, SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Message_Error("Error while calling open \"snd_pcm_open()\"");
#endif

        return(FALSE);
    }
    
    if(milliseconds < 40) milliseconds = 40;
    if(milliseconds > 250) milliseconds = 250;
    // US = MS * 1000
    frag_size = (int) (AUDIO_PCM_FREQ * (milliseconds / 1000.0f));
    if(snd_pcm_set_params(playback_handle,
                          SND_PCM_FORMAT_S16_LE,
                          SND_PCM_ACCESS_RW_INTERLEAVED,
                          AUDIO_DBUF_CHANNELS,
                          AUDIO_PCM_FREQ,
                          0,
                          milliseconds * 1000) < 0)
    {

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Message_Error("Error while calling snd_pcm_set_params()");
#endif

        return(FALSE);
    }

    AUDIO_SoundBuffer_Size = frag_size * ((AUDIO_DBUF_RESOLUTION * AUDIO_DBUF_CHANNELS) >> 3);
    AUDIO_Latency = AUDIO_SoundBuffer_Size;
    AUDIO_SoundBuffer = (short *) malloc(AUDIO_SoundBuffer_Size);

    Thread_Running = 1;
    if(pthread_create(&hThread, NULL, AUDIO_Thread, NULL) == 0)
    {
        return(TRUE);
    }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    Message_Error("Error while calling pthread_create()");
#endif

    Thread_Running = 0;

    return(FALSE);
}

// ------------------------------------------------------
// Name: AUDIO_Wait_For_Thread()
// Desc: Wait for a command acknowledgment from the thread
void AUDIO_Wait_For_Thread(void)
{
    if(Thread_Running)
    {
        if(AUDIO_Play_Flag)
        {
            while(AUDIO_Acknowledge)
            {
                usleep(10);
            };
        }
        else
        {
            if(hThread)
            {
                while(!AUDIO_Acknowledge)
                {
                    usleep(10);
                };
            }
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
    AUDIO_Stop();

    if(hThread)
    {
        Thread_Running = 0;
        while(!Thread_Running)
        {
            usleep(10);
        }
        hThread = 0;
    }
    if(AUDIO_SoundBuffer)
    {
        free(AUDIO_SoundBuffer);
        AUDIO_SoundBuffer = NULL;
    }
    if(playback_handle)
    {
        snd_pcm_drain(playback_handle);
        snd_pcm_close(playback_handle);
        playback_handle = NULL;
    }
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();
    playback_handle = NULL;
}
