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
#include "include/sounddriver_aos4.h"

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples;
int AUDIO_Play_Flag;
float AUDIO_Timer;

int volatile AUDIO_Acknowledge;

struct MsgPort *AHImp;
struct AHIRequest *AHIio;
struct AHIRequest *AHIio2;
struct AHIRequest *join;
short *AHIbuf;
short *AHIbuf2;
int volatile Thread_Running;
int32 old_sigbit;
void *old_sigtask;
pthread_t hThread;

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
void *AUDIO_Thread(void *arg)
{
    struct sched_param p;
    
    old_sigbit = AHImp->mp_SigBit;
    old_sigtask = AHImp->mp_SigTask;
    AHImp->mp_SigBit = IExec->AllocSignal(-1);
    AHImp->mp_SigTask = IExec->FindTask(NULL);

    memset(&p, 0, sizeof(p));
    p.sched_priority = 1;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &p);

    while(Thread_Running)
    {
        if(AHIbuf)
        {
            struct AHIRequest *io = AHIio;
            short *buf = AHIbuf;
    
            if(AUDIO_Play_Flag)
            {
                AUDIO_Mixer((Uint8 *) buf, AUDIO_SoundBuffer_Size);
                AUDIO_Acknowledge = FALSE;
            }
            else
            {
                unsigned int i;
                char *pSamples = (char *) buf;
                for(i = 0; i < AUDIO_SoundBuffer_Size; i++)
                {
                    pSamples[i] = 0;
                }
                AUDIO_Acknowledge = TRUE;
            }    

            io->ahir_Std.io_Message.mn_Node.ln_Pri = 0;
            io->ahir_Std.io_Command = CMD_WRITE;
            io->ahir_Std.io_Data = buf;
            io->ahir_Std.io_Length = AUDIO_SoundBuffer_Size;
            io->ahir_Std.io_Offset = 0;
            io->ahir_Frequency = AUDIO_PCM_FREQ;
            io->ahir_Type = AHIST_S16S;
            io->ahir_Volume = 0x10000;
            io->ahir_Position = 0x8000;
            io->ahir_Link = join;
            IExec->SendIO((struct IORequest *) io);
            if(join)
            {
                IExec->WaitIO((struct IORequest *) join);
            }
            join = io;
            AHIio = AHIio2;
            AHIio2 = io;
            AHIbuf = AHIbuf2;
            AHIbuf2 = buf;

            AUDIO_Samples += AUDIO_SoundBuffer_Size;
            AUDIO_Timer = ((((float) AUDIO_Samples) * (1.0f / (float) AUDIO_Latency)) * 1000.0f);
        }
        usleep(10);
    }
    Thread_Running = 1;
    return(NULL);
}

// ------------------------------------------------------
// Name: AUDIO_Init_Driver()
// Desc: Init the audio driver
int AUDIO_Init_Driver(void (*Mixer)(Uint8 *, Uint32))
{
    AUDIO_Mixer = Mixer;

    AHImp = (struct MsgPort *) IExec->AllocSysObject(ASOT_PORT, NULL);

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    if(!AHImp)
    {
        Message_Error("Error while calling IExec->AllocSysObject()");
        return(FALSE);
    }
#endif

    AHIio = (struct AHIRequest *) IExec->AllocSysObjectTags(ASOT_IOREQUEST,
                                                            ASOIOR_ReplyPort, AHImp,
                                                            ASOIOR_Size, sizeof(struct AHIRequest),
                                                            TAG_END);
    AHIio2 = (struct AHIRequest *) IExec->AllocSysObjectTags(ASOT_IOREQUEST,
                                                             ASOIOR_ReplyPort, AHImp,
                                                             ASOIOR_Size, sizeof(struct AHIRequest),
                                                             TAG_END);
    join = NULL;
    if(!AHIio || !AHIio2)
    {

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Message_Error("Error while calling IExec->AllocSysObjectTags()");
#endif

        return(FALSE);
    }
    AHIio->ahir_Version = 4;

    // Open ahi AHI_NO_UNIT
    if(IExec->OpenDevice(AHINAME, AHI_DEFAULT_UNIT, (struct IORequest *) AHIio, 0))
    {
        AHIio->ahir_Std.io_Device = NULL;

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Message_Error("Error while calling IExec->OpenDevice()");
#endif

        return(FALSE);
    }

    // Copy for double buffering
    IExec->CopyMem(AHIio, AHIio2, sizeof(struct AHIRequest));

    // Create audio buffer
    return(AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds));
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

    AUDIO_SoundBuffer_Size = frag_size << 2;
    AUDIO_Latency = AUDIO_SoundBuffer_Size;
    
    AHIbuf = (short *) IExec->AllocVecTags(AUDIO_SoundBuffer_Size << 1, AVT_Type, MEMF_SHARED, TAG_END);
    AHIbuf2 = (short *) IExec->AllocVecTags(AUDIO_SoundBuffer_Size << 1, AVT_Type, MEMF_SHARED, TAG_END);
    
    if(!AHIbuf || !AHIbuf2)
    {

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Message_Error("Error while calling IExec->AllocVecTagList()");
#endif

        return(FALSE);
    }
    
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
        hThread = 0;
        if(join)
        {
            IExec->AbortIO((struct IORequest *) join);
            IExec->WaitIO((struct IORequest *) join);
        }
        IExec->FreeSignal(AHImp->mp_SigBit);
        AHImp->mp_SigBit = old_sigbit;
        AHImp->mp_SigTask = old_sigtask;
    }
    IExec->FreeVec(AHIbuf);
    IExec->FreeVec(AHIbuf2);
    AHIbuf = NULL;
    AHIbuf2 = NULL;
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();

    if(AHIio && AHIio->ahir_Std.io_Device)
    {
        IExec->CloseDevice((struct IORequest *) AHIio);
    }
    IExec->FreeSysObject(ASOT_IOREQUEST, AHIio);
    IExec->FreeSysObject(ASOT_IOREQUEST, AHIio2);
    IExec->FreeSysObject(ASOT_PORT, AHImp);
    AHIio = NULL;
    AHIio2 = NULL;
    AHImp = NULL;
}
