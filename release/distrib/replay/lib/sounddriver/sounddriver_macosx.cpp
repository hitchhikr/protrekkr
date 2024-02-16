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
#include "include/sounddriver_macosx.h"

// ------------------------------------------------------
// Variables
unsigned int AUDIO_To_Fill;
int AUDIO_Samples;
int AUDIO_Play_Flag;
float AUDIO_Timer;

int volatile AUDIO_Acknowledge;
AudioDeviceID AUDIO_Device;
UInt32 Amount;
AudioStreamBasicDescription	Desc;

int AUDIO_SoundBuffer_Size;
int AUDIO_Latency;
int AUDIO_Milliseconds = 10;
int AUDIO_16Bits;

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
static OSStatus AUDIO_Callback(AudioDeviceID device,
                               const AudioTimeStamp* current_time,
	                           const AudioBufferList* data_in,
                               const AudioTimeStamp* time_in,
	                           AudioBufferList *data_out,
                               const AudioTimeStamp *time_out,
                               void *client_data)
{
    if(AUDIO_Play_Flag)
    {
        AUDIO_Mixer((Uint8 *) data_out->mBuffers[0].mData, data_out->mBuffers[0].mDataByteSize);
    }
    else
    {
        unsigned int i;
        char *pSamples = (char *) data_out->mBuffers[0].mData;
        for(i = 0; i < data_out->mBuffers[0].mDataByteSize; i++)
        {
            pSamples[i] = 0;
        }
    }
    AUDIO_Samples += data_out->mBuffers[0].mDataByteSize;
    AUDIO_Timer = ((((float) AUDIO_Samples) * (1.0f / (float) AUDIO_Latency)) * 1000.0f);
    return(kAudioHardwareNoError);
}

// ------------------------------------------------------
// Name: AUDIO_Init_Driver()
// Desc: Init the audio driver
int AUDIO_Init_Driver(void (*Mixer)(Uint8 *, Uint32))
{
    AUDIO_Mixer = Mixer;

    AUDIO_Device = kAudioDeviceUnknown;
    Amount = sizeof(AudioDeviceID);
    if(AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice,
                                &Amount,
                                (void *) &AUDIO_Device) == noErr)
    {
        if(AudioDeviceAddIOProc(AUDIO_Device,
                                AUDIO_Callback,
                                NULL) == noErr)
        {
            return(AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds));
        }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        else
        {
            Message_Error("Error while calling AudioDeviceAddIOProc()");
        }
#endif

    }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    else
    {
        Message_Error("Error while calling AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice)");
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
    int found_pcmformat = TRUE;

    if(milliseconds < 10) milliseconds = 10;
    if(milliseconds > 250) milliseconds = 250;

    num_fragments = 6;
    frag_size = (int) (AUDIO_PCM_FREQ * (milliseconds / 1000.0f));

    AudioValueRange Frame_Range;

    AUDIO_16Bits = TRUE;

    Amount = sizeof(AudioStreamBasicDescription);
    if(AudioDeviceGetProperty(AUDIO_Device,
                              0,
                              FALSE,
                              kAudioDevicePropertyStreamFormat, &Amount, &Desc) == noErr)
    {
        Desc.mSampleRate = AUDIO_PCM_FREQ;
        Desc.mChannelsPerFrame = AUDIO_DBUF_CHANNELS;
        Desc.mBitsPerChannel = sizeof(short) << 3;
        Desc.mFormatFlags = kAudioFormatFlagIsSignedInteger;//kLinearPCMFormatFlagIsPacked | 
        Desc.mFormatID = kAudioFormatLinearPCM;
        Desc.mFramesPerPacket = 1;
        Desc.mBytesPerFrame = (Desc.mBitsPerChannel * Desc.mChannelsPerFrame) >> 3;
        Desc.mBytesPerPacket = Desc.mBytesPerFrame * Desc.mFramesPerPacket;

#if defined(__BIG_ENDIAN__)
        Desc.mFormatFlags |= kLinearPCMFormatFlagIsBigEndian;
#endif

        // Try with 16 bit integers
        if(AudioDeviceSetProperty(AUDIO_Device,
                                  NULL,
                                  0,
                                  FALSE,
                                  kAudioDevicePropertyStreamFormat, sizeof(AudioStreamBasicDescription), &Desc) != noErr)
        {
            // Try with 32 bit floating points
            AUDIO_16Bits = FALSE;
            Desc.mSampleRate = AUDIO_PCM_FREQ;
            Desc.mChannelsPerFrame = AUDIO_DBUF_CHANNELS;
            Desc.mBitsPerChannel = sizeof(float) << 3;
            Desc.mFormatFlags = kLinearPCMFormatFlagIsPacked | kAudioFormatFlagIsFloat;
            Desc.mFormatID = kAudioFormatLinearPCM;
            Desc.mFramesPerPacket = 1;
            Desc.mBytesPerFrame = (Desc.mBitsPerChannel * Desc.mChannelsPerFrame) >> 3;
            Desc.mBytesPerPacket = Desc.mBytesPerFrame * Desc.mFramesPerPacket;

#if defined(__BIG_ENDIAN__)
            Desc.mFormatFlags |= kLinearPCMFormatFlagIsBigEndian;
#endif

            if(AudioDeviceSetProperty(AUDIO_Device,
                                      NULL,
                                      0,
                                      FALSE,
                                      kAudioDevicePropertyStreamFormat, sizeof(AudioStreamBasicDescription), &Desc) != noErr)
            {
                found_pcmformat = FALSE;
            }
        }

        if(found_pcmformat)
        {
	        Amount = sizeof(AudioValueRange);

            AUDIO_SoundBuffer_Size = frag_size << 2;

            if(AudioDeviceGetProperty(AUDIO_Device,
                                      0,
                                      FALSE,
                                      kAudioDevicePropertyBufferFrameSizeRange,
                                      &Amount,
                                      &Frame_Range) == noErr)
            {
                if(AUDIO_SoundBuffer_Size > (int) Frame_Range.mMaximum)
                {
                    AUDIO_SoundBuffer_Size = Frame_Range.mMaximum;
                }
            }

            AUDIO_Latency = AUDIO_SoundBuffer_Size;

            if(AudioDeviceSetProperty(AUDIO_Device,
                                      0,
                                      0,
                                      0,
                                      kAudioDevicePropertyBufferFrameSize,
                                      sizeof(AUDIO_SoundBuffer_Size),
                                      &AUDIO_SoundBuffer_Size) == noErr)
            {
                AudioDeviceStart(AUDIO_Device, AUDIO_Callback);
                return(TRUE);
            }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
            else
            {
                Message_Error("Error while calling AudioDeviceSetProperty(kAudioDevicePropertyBufferFrameSize)");
            }
#endif

        }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        else
        {
            Message_Error("Error while calling AudioDeviceSetProperty(kAudioDevicePropertyStreamFormat)");
        }
#endif

    }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    else
    {
        Message_Error("Error while calling AudioDeviceGetProperty(kAudioDevicePropertyStreamFormat)");
    }
#endif

    return(FALSE);
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
    AUDIO_Stop();
    AudioDeviceStop(AUDIO_Device, AUDIO_Callback);
}

// ------------------------------------------------------
// Name: AUDIO_Stop_Driver()
// Desc: Stop everything
void AUDIO_Stop_Driver(void)
{
    AUDIO_Stop_Sound_Buffer();
    AudioDeviceRemoveIOProc(AUDIO_Device, AUDIO_Callback);
    AUDIO_Device = 0;
}
