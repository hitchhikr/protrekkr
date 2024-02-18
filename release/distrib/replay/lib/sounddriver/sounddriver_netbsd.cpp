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
#include "include/sounddriver_netbsd.h"

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples;
int AUDIO_Play_Flag;
float AUDIO_Timer;

int volatile AUDIO_Acknowledge;
int AUDIO_Device;
short *AUDIO_SoundBuffer[2];
audio_buf_info AUDIO_Info_Buffer;
pthread_t hThread;
int volatile Thread_Running;
int AUDIO_SoundBuffer_Size;

int AUDIO_Latency;
int AUDIO_Milliseconds = 10;
int AUDIO_FlipFlop = 0;

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
    while(Thread_Running)
    {
        if(AUDIO_SoundBuffer[0])
        {
            AUDIO_Acknowledge = FALSE;
            if(AUDIO_Play_Flag)
            {
                AUDIO_Mixer((Uint8 *) AUDIO_SoundBuffer[AUDIO_FlipFlop], AUDIO_SoundBuffer_Size);
            }
            else
            {
                unsigned int i;
                char *pSamples = (char *) AUDIO_SoundBuffer[AUDIO_FlipFlop];
                for(i = 0; i < AUDIO_SoundBuffer_Size; i++)
                {
                    pSamples[i] = 0;
                }
                AUDIO_Acknowledge = TRUE;
            }
            write(AUDIO_Device, AUDIO_SoundBuffer[AUDIO_FlipFlop], AUDIO_SoundBuffer_Size);
            AUDIO_FlipFlop ^= 1;

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
    AUDIO_Mixer = Mixer;

    char *Mixer_Name;
    int8 Mixer_Volume[4];
    struct sched_param p;

    AUDIO_Device = open("/dev/audio", O_WRONLY, 0);
    if(AUDIO_Device >= 0)
    {
        p.sched_priority = 1;
        pthread_setschedparam(pthread_self(), SCHED_FIFO, &p);
        return(AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds));
    }
    
#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    else
    {
        Message_Error("Error while calling open(\"/dev/audio\")");
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

    int Dsp_Val;
    struct sched_param p;

	frag_size = 10 + (int) (logf((float) (frag_size >> 9)) / logf(2.0f));

    Dsp_Val = (num_fragments << 16) | frag_size;
    ioctl(AUDIO_Device, SNDCTL_DSP_SETFRAGMENT, &Dsp_Val);
    Dsp_Val = AUDIO_DBUF_CHANNELS;
    ioctl(AUDIO_Device, SNDCTL_DSP_CHANNELS, &Dsp_Val);
    Dsp_Val = AFMT_S16_NE;
    ioctl(AUDIO_Device, SNDCTL_DSP_SETFMT, &Dsp_Val);
    Dsp_Val = AUDIO_PCM_FREQ;
    ioctl(AUDIO_Device, SNDCTL_DSP_SPEED, &Dsp_Val);

    if((ioctl(AUDIO_Device, SNDCTL_DSP_GETOSPACE, &AUDIO_Info_Buffer) < 0))
    {
        ioctl(AUDIO_Device, SNDCTL_DSP_GETBLKSIZE, &AUDIO_Info_Buffer.fragsize);
    }

    AUDIO_SoundBuffer_Size = AUDIO_Info_Buffer.fragsize;
    AUDIO_Latency = AUDIO_SoundBuffer_Size;

    AUDIO_SoundBuffer[0] = (short *) malloc(AUDIO_SoundBuffer_Size << 1);
    if(!AUDIO_SoundBuffer[0])
    {

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Message_Error("Can't allocate audio buffer 1");
#endif

        return(FALSE);
    }
    AUDIO_SoundBuffer[1] = (short *) malloc(AUDIO_SoundBuffer_Size << 1);
    if(!AUDIO_SoundBuffer[1])
    {

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Message_Error("Can't allocate audio buffer 2");
#endif

        return(FALSE);
    }
    memset(AUDIO_SoundBuffer[0], 0, AUDIO_SoundBuffer_Size << 1);
    memset(AUDIO_SoundBuffer[1], 0, AUDIO_SoundBuffer_Size << 1);

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
            }
        }
        else
        {
            if(hThread)
            {
                while(!AUDIO_Acknowledge)
                {
                    usleep(10);
                }
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
        hThread = NULL;
    }
    if(AUDIO_SoundBuffer[0]) free(AUDIO_SoundBuffer[0]);
    AUDIO_SoundBuffer[0] = NULL;
    if(AUDIO_SoundBuffer[1]) free(AUDIO_SoundBuffer[1]);
    AUDIO_SoundBuffer[1] = NULL;
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();
    if(AUDIO_Device) close(AUDIO_Device);
    AUDIO_Device = NULL;
}
