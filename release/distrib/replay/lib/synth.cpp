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
#include "include/replay.h"

// ------------------------------------------------------
// Variables
float SIN[360]; // Sine float-precalculated table, in absolute degrees.

#if defined(PTK_SYNTH)

int SIZE_WAVEFORMS;
extern char Use_Cubic;

#if defined(PTK_SYNTH_SIN)
short STOCK_SIN[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_SAW)
short STOCK_SAW[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_PULSE)
short STOCK_PULSE[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_WHITE)
short STOCK_WHITE[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_PINK)
    short STOCK_PINK[SIZE_WAVEFORMS_SPACE];
#endif

// ------------------------------------------------------
// This next function resets the synthesizer to default values
void CSynth::Reset(void)
{

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    // Synthesizer General Reset
    Data.DISTO = 0.0f;
    Data.OSC1_WAVEFORM = WAVEFORM_SAW;      /* Sawtooth */
    Data.OSC2_WAVEFORM = WAVEFORM_PULSE;    /* Pulse */

    T_OSC1_VOLUME = 0;
    T_OSC2_VOLUME = 0;

    Data.OSC2_DETUNE = 0;        /* No Semitone Detune */
    Data.OSC2_FINETUNE = 0.1f;   /* 1/10 Semitone detune */

    Data.VCF_CUTOFF = 0.5f;      /* 10000Hz Cutoff */
    Data.VCF_RESONANCE = 0.5f;   /* Not very weird =] */
    Data.VCF_TYPE = 0;           /* LowPass filter */

    Data.ENV1_ATTACK = 0.0f;
    Data.ENV1_DECAY = 0.1f;
    Data.ENV1_SUSTAIN = 0.3f;    /* Sustain volume at 1/3 */
    Data.ENV1_RELEASE = 0.0f;

    Data.ENV2_ATTACK = 0.0f;
    Data.ENV2_DECAY = 0.1f;
    Data.ENV2_SUSTAIN = 0.3f;    /* Sustain volume at 1/3 */
    Data.ENV2_RELEASE = 0.0f;

    Data.LFO1_PERIOD = 16;
    Data.LFO2_PERIOD = 16;

    LFO1_GR = 0;
    LFO2_GR = 0;

    LFO1_SUBGRCOUNTER = 0;
    LFO2_SUBGRCOUNTER = 0;

    Data.LFO1_SUBGRMAX = 200;
    Data.LFO2_SUBGRMAX = 200;

    Data.LFO1_OSC1_PW = 0;
    Data.LFO1_OSC2_PW = 0;
    Data.LFO1_OSC1_PITCH = 0;
    Data.LFO1_OSC2_PITCH = 0;
    Data.LFO1_OSC1_VOLUME = 0;
    Data.LFO1_OSC2_VOLUME = 0;   
    Data.LFO1_VCF_CUTOFF = 0;
    Data.LFO1_VCF_RESONANCE = 0; 

    Data.LFO2_OSC1_PW = 0;
    Data.LFO2_OSC2_PW = 0;
    Data.LFO2_OSC1_PITCH = 0;
    Data.LFO2_OSC2_PITCH = 0;
    Data.LFO2_OSC1_VOLUME = 0;
    Data.LFO2_OSC2_VOLUME = 0;   
    Data.LFO2_VCF_CUTOFF = 0;
    Data.LFO2_VCF_RESONANCE = 0; 

    Data.ENV1_OSC1_PW = 0;
    Data.ENV1_OSC2_PW = 0;
    Data.ENV1_OSC1_PITCH = 0;
    Data.ENV1_OSC2_PITCH = 0;
    Data.ENV1_OSC1_VOLUME = 1.0f;
    Data.ENV1_OSC2_VOLUME = 1.0f;   
    Data.ENV1_VCF_CUTOFF = 0;
    Data.ENV1_VCF_RESONANCE = 0; 

    Data.ENV2_OSC1_PW = 0;
    Data.ENV2_OSC2_PW = 0;
    Data.ENV2_OSC1_PITCH = 0;
    Data.ENV2_OSC2_PITCH = 0;
    Data.ENV2_OSC1_VOLUME = 0;
    Data.ENV2_OSC2_VOLUME = 0;   
    Data.ENV2_VCF_CUTOFF = 0;
    Data.ENV2_VCF_RESONANCE = 0; 

    Data.OSC3_SWITCH = FALSE;

    OSC1_STEP = 0;
    OSC2_STEP = 0;

    ENV1_VOLUME = 0;
    ENV2_VOLUME = 0;

    ENV1_MIN = 0;
    ENV2_MIN = 0;

    Data.LFO1_ATTACK = 0.0f;
    Data.LFO1_DECAY = 0.1f;
    Data.LFO1_SUSTAIN = 0.3f;
    Data.LFO1_RELEASE = 0.0f;

    Data.LFO2_ATTACK = 0.0f;
    Data.LFO2_DECAY = 0.1f;
    Data.LFO2_SUSTAIN = 0.3f;
    Data.LFO2_RELEASE = 0.0f;

    Data.OSC3_VOLUME = 0;

    Data.PTC_GLIDE64 = 0;

#endif

#if defined(PTK_SYNTH_LFO1)
    LFO1_COUNTER = 0;
    LFO1_STAGE = 0;
    LFO1_ADSR_VALUE = 0.0f;
    LFO1_VALUE = 0;
#endif

#if defined(PTK_SYNTH_LFO2)
    LFO2_COUNTER = 0;
    LFO2_STAGE = 0;
    LFO2_ADSR_VALUE = 0.0f;
    LFO2_VALUE = 0;
#endif

    ENV1_STAGE = 0;
    ENV2_STAGE = 0;

    ENV1_COUNTER = 0;
    ENV2_COUNTER = 0;

    ENV1_VALUE = 0;
    ENV2_VALUE = 0;

    ENV1_A_COEF = 0.0f;
    ENV2_A_COEF = 0.0f;

    OSC1_SPEED = 0;
    OSC2_SPEED = 0;
    OSC3_SPEED = 0;

    sbuf0L = 0.0f;
    sbuf1L = 0.0f;
    sbuf0R = 0.0f;
    sbuf1R = 0.0f;

#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
    MoogBufferL[0] = 0.0f;
    MoogBufferL[1] = 0.0f;
    MoogBufferL[2] = 0.0f;
    MoogBufferL[3] = 0.0f;
    MoogBufferL[4] = 0.0f;

    MoogBufferR[0] = 0.0f;
    MoogBufferR[1] = 0.0f;
    MoogBufferR[2] = 0.0f;
    MoogBufferR[3] = 0.0f;
    MoogBufferR[4] = 0.0f;
#endif
}

// ------------------------------------------------------
// This function is for internal use only. Makes the LFO's run.
void CSynth::LfoAdvance(void)
{
#if defined(PTK_SYNTH_LFO1)
    LFO1_SUBGRCOUNTER++;
#endif

#if defined(PTK_SYNTH_LFO2)
    LFO2_SUBGRCOUNTER++;
#endif

#if defined(PTK_SYNTH_LFO1)
    switch(LFO1_STAGE)
    {
        case SYNTH_ATTACK:
            LFO1_ADSR_VALUE += LFO1_A_COEF;
            LFO1_COUNTER++;
            if(LFO1_COUNTER >= LFO1b_ATTACK)
            {
                if(LFO1_ADSR_VALUE > 1.0f) LFO1_ADSR_VALUE = 1.0f;
                LFO1_COUNTER = 0;
                LFO1_STAGE = 2;
            }
            break;
        case SYNTH_DECAY:
            LFO1_ADSR_VALUE -= LFO1_D_COEF;
            LFO1_COUNTER++;
            if(LFO1_COUNTER >= LFO1b_DECAY)
            {
                if(LFO1_ADSR_VALUE < 0.0f) LFO1_ADSR_VALUE = 0.0f;
                LFO1_ADSR_VALUE = Data.LFO1_SUSTAIN;
                LFO1_COUNTER = 0;
                LFO1_STAGE = 3;
            }
            break;
        case SYNTH_SUSTAIN:
            LFO1_ADSR_VALUE = Data.LFO1_SUSTAIN;
            break;
        case SYNTH_RELEASE:
            LFO1_ADSR_VALUE -= LFO1_R_COEF;
            LFO1_COUNTER++;
            if(LFO1_COUNTER > LFO1b_RELEASE)
            {
                LFO1_ADSR_VALUE = 0.0f;
                LFO1_COUNTER = 0;
                LFO1_STAGE = 0;
            }
            break;
    }

    if(LFO1_SUBGRCOUNTER > Data.LFO1_SUBGRMAX)
    {
        LFO1_SUBGRCOUNTER = 0;
        LFO1_GR++;
        if(LFO1_GR > 359) LFO1_GR = 0;
    }

    LFO1_VALUE = SIN[LFO1_GR] * LFO1_ADSR_VALUE;
#endif

#if defined(PTK_SYNTH_LFO2)
    switch(LFO2_STAGE)
    {
        case SYNTH_ATTACK:
            LFO2_ADSR_VALUE += LFO2_A_COEF;
            LFO2_COUNTER++;
            if(LFO2_COUNTER >= LFO2b_ATTACK)
            {
                if(LFO2_ADSR_VALUE > 1.0f) LFO2_ADSR_VALUE = 1.0f;
                LFO2_COUNTER = 0;
                LFO2_STAGE = 2;
            }
            break;
        case SYNTH_DECAY:
            LFO2_ADSR_VALUE -= LFO2_D_COEF;
            LFO2_COUNTER++;
            if(LFO2_COUNTER >= LFO2b_DECAY)
            {
                if(LFO2_ADSR_VALUE < 0.0f) LFO2_ADSR_VALUE = 0.0f;
                LFO2_ADSR_VALUE = Data.LFO2_SUSTAIN;
                LFO2_COUNTER = 0;
                LFO2_STAGE = 3;
            }
            break;
        case SYNTH_SUSTAIN:
            LFO2_ADSR_VALUE = Data.LFO2_SUSTAIN;
            break;
        case SYNTH_RELEASE:
            LFO2_ADSR_VALUE -= LFO2_R_COEF;
            LFO2_COUNTER++;
            if(LFO2_COUNTER > LFO2b_RELEASE)
            {
                LFO2_ADSR_VALUE = 0.0f;
                LFO2_COUNTER = 0;
                LFO2_STAGE = 0;
            }
            break;
    }

    if(LFO2_SUBGRCOUNTER > Data.LFO2_SUBGRMAX)
    {
        LFO2_SUBGRCOUNTER = 0;
        LFO2_GR++;
        if(LFO2_GR > 359) LFO2_GR = 0;
    }

    LFO2_VALUE = SIN[LFO2_GR] * LFO2_ADSR_VALUE;
#endif

}

// ------------------------------------------------------
// The cooler NoteOn typical message for this Class CSynth Objects =].
// (used at tick 0)
void CSynth::NoteOn(int note, float speed, int Looping, unsigned int Length,
                    unsigned int Loop_Length
#if defined(PTK_INSTRUMENTS)
                    ,float note_smp
#endif
                    ,int glide
                   )
{

#if defined(PTK_INSTRUMENTS)
    float note_1 = Data.OSC1_WAVEFORM == WAVEFORM_WAV ? note_smp: (float) note;
    float note_2 = Data.OSC2_WAVEFORM == WAVEFORM_WAV ? note_smp: (float) note;
#else
    float note_1 = (float) note;
    float note_2 = (float) note;
#endif

    float smp_freq;
    float adsr_ratio;

    OSC1_STEP = POWF2(note_1 / 12.0f);
    OSC2_STEP = POWF2((note_2 + Data.OSC2_FINETUNE + Data.OSC2_DETUNE) / 12.0f);

    if(!glide)
    {
        ENV1_STAGE = SYNTH_ATTACK; /* '0' is off, '1' starts the attack */
        ENV2_STAGE = SYNTH_ATTACK;

        ENV1_COUNTER = 0; /* Envelope stage counter, in samples */
        ENV2_COUNTER = 0;

        ENV1_VALUE = 0;
        ENV2_VALUE = 0;

        ENV1_MIN = 0;
        ENV2_MIN = 0;

        ENV1_VOLUME = 0;
        ENV2_VOLUME = 0;

        sbuf0L = 0.0f;
        sbuf1L = 0.0f;
        sbuf0R = 0.0f;
        sbuf1R = 0.0f;

#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
        MoogBufferL[0] = 0.0f;
        MoogBufferL[1] = 0.0f;
        MoogBufferL[2] = 0.0f;
        MoogBufferL[3] = 0.0f;
        MoogBufferL[4] = 0.0f;

        MoogBufferR[0] = 0.0f;
        MoogBufferR[1] = 0.0f;
        MoogBufferR[2] = 0.0f;
        MoogBufferR[3] = 0.0f;
        MoogBufferR[4] = 0.0f;
#endif

    }

    if(Data.OSC1_WAVEFORM != WAVEFORM_WAV)
    {
        Length = SIZE_WAVEFORMS;
        Loop_Length = SIZE_WAVEFORMS;
    }
    smp_freq = ((float) Length) / OSC1_STEP;
    adsr_ratio = smp_freq / 1024.0f;
    ENV1b_ATTACK = (Data.ENV1_ATTACK / adsr_ratio) * smp_freq;
    ENV1b_DECAY = (Data.ENV1_DECAY / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO1)
    LFO1b_ATTACK = (Data.LFO1_ATTACK / adsr_ratio) * smp_freq;
    LFO1b_DECAY = (Data.LFO1_DECAY / adsr_ratio) * smp_freq;
#endif

    if(Looping)
    {
        smp_freq = ((float) Loop_Length) / OSC1_STEP;
        adsr_ratio = smp_freq / 1024.0f;
    }
    ENV1b_RELEASE = (Data.ENV1_RELEASE / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO1)
    LFO1b_RELEASE = (Data.LFO1_RELEASE / adsr_ratio) * smp_freq;
#endif

    if(Data.OSC2_WAVEFORM != WAVEFORM_WAV)
    {
        Length = SIZE_WAVEFORMS;
        Loop_Length = SIZE_WAVEFORMS;
    }
    smp_freq = ((float) Length) / OSC2_STEP;
    adsr_ratio = smp_freq / 1024.0f;
    ENV2b_ATTACK = (Data.ENV2_ATTACK / adsr_ratio) * smp_freq;
    ENV2b_DECAY = (Data.ENV2_DECAY / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO2)
    LFO2b_ATTACK = (Data.LFO2_ATTACK / adsr_ratio) * smp_freq;
    LFO2b_DECAY = (Data.LFO2_DECAY / adsr_ratio) * smp_freq;
#endif

    if(Looping)
    {
        smp_freq = ((float) Loop_Length) / OSC2_STEP;
        adsr_ratio = smp_freq / 1024.0f;
    }
    ENV2b_RELEASE = (Data.ENV2_RELEASE / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO2)
    LFO2b_RELEASE = (Data.LFO2_RELEASE / adsr_ratio) * smp_freq;
#endif

    if(ENV1b_ATTACK < 1.0f) ENV1b_ATTACK = 1.0f;
    if(ENV1b_DECAY < 1.0f) ENV1b_DECAY = 1.0f;
    if(ENV1b_RELEASE < 1.0f) ENV1b_RELEASE = 1.0f;
    if(ENV2b_ATTACK < 1.0f) ENV2b_ATTACK = 1.0f;
    if(ENV2b_DECAY < 1.0f) ENV2b_DECAY = 1.0f;
    if(ENV2b_RELEASE < 1.0f) ENV2b_RELEASE = 1.0f;

    /* Update ENV1 */
    ENV1_D_COEF = (1.0f - Data.ENV1_SUSTAIN) / (float) ENV1b_DECAY;
    ENV1_R_COEF = Data.ENV1_SUSTAIN / (float) ENV1b_RELEASE;

    /* Update ENV2 */
    ENV2_D_COEF = (1.0f - Data.ENV2_SUSTAIN) / (float) ENV2b_DECAY;
    ENV2_R_COEF = Data.ENV2_SUSTAIN / (float) ENV2b_RELEASE;

    ENV1_A_COEF = (1.0f - ENV1_VALUE) / ENV1b_ATTACK;
    ENV2_A_COEF = (1.0f - ENV2_VALUE) / ENV2b_ATTACK;

    if(!glide)
    {
        ENV1_LOOP_BACKWARD = FALSE;
        ENV2_LOOP_BACKWARD = FALSE;
        ENV3_LOOP_BACKWARD = FALSE;

#if defined(PTK_SYNTH_LFO1)
        LFO1_STAGE = SYNTH_ATTACK;
        LFO1_GR = 0;
        LFO1_VALUE = 0;
        LFO1_ADSR_VALUE = 0;
        LFO1_COUNTER = 0;
        LFO1_SUBGRCOUNTER = 0;
#endif

#if defined(PTK_SYNTH_LFO2)
        LFO2_STAGE = SYNTH_ATTACK;
        LFO2_GR = 0;
        LFO2_VALUE = 0;
        LFO2_ADSR_VALUE = 0;
        LFO2_COUNTER = 0;
        LFO2_SUBGRCOUNTER = 0;
#endif
    }

#if defined(PTK_SYNTH_LFO1)
    if(LFO1b_ATTACK < 1.0f) LFO1b_ATTACK = 1.0f;
    if(LFO1b_DECAY < 1.0f) LFO1b_DECAY = 1.0f;
    if(LFO1b_RELEASE < 1.0f) LFO1b_RELEASE = 1.0f;
#endif

#if defined(PTK_SYNTH_LFO2)
    if(LFO2b_ATTACK < 1.0f) LFO2b_ATTACK = 1.0f;
    if(LFO2b_DECAY < 1.0f) LFO2b_DECAY = 1.0f;
    if(LFO2b_RELEASE < 1.0f) LFO2b_RELEASE = 1.0f;
#endif

#if defined(PTK_SYNTH_LFO1)
    LFO1_D_COEF = (1.0f - Data.LFO1_SUSTAIN) / (float) LFO1b_DECAY;
    LFO1_R_COEF = Data.LFO1_SUSTAIN / (float) LFO1b_RELEASE;
    LFO1_A_COEF = (1.0f - LFO1_ADSR_VALUE) / LFO1b_ATTACK;
#endif

#if defined(PTK_SYNTH_LFO2)
    LFO2_D_COEF = (1.0f - Data.LFO2_SUSTAIN) / (float) LFO2b_DECAY;
    LFO2_R_COEF = Data.LFO2_SUSTAIN / (float) LFO2b_RELEASE;
    LFO2_A_COEF = (1.0f - LFO2_ADSR_VALUE) / LFO2b_ATTACK;
#endif
}

// ------------------------------------------------------
// Envelopes run function
void CSynth::EnvRun(int *track, int *track2)
{
    /* ENV1 */
    switch(ENV1_STAGE)
    {
        /* Attack */
        case SYNTH_ATTACK:
            ENV1_VALUE += ENV1_A_COEF;
            ENV1_COUNTER++;
            if(ENV1_COUNTER >= ENV1b_ATTACK)
            {
                if(ENV1_VALUE > 1.0f) ENV1_VALUE = 1.0f;
                ENV1_COUNTER = 0;
                ENV1_STAGE = SYNTH_DECAY;
            }
            break;

        /* Decay */
        case SYNTH_DECAY:
            ENV1_VALUE -= ENV1_D_COEF;
            ENV1_COUNTER++;
            if(ENV1_COUNTER >= ENV1b_DECAY)
            {
                if(ENV1_VALUE < 0.0f) ENV1_VALUE = 0.0f;
                ENV1_VALUE = Data.ENV1_SUSTAIN;
                ENV1_COUNTER = 0;
                ENV1_STAGE = SYNTH_SUSTAIN;
            }
            break;

        /* Sustain */
        case SYNTH_SUSTAIN:
            ENV1_VALUE = Data.ENV1_SUSTAIN;
            break;

        /* Release */
        case SYNTH_RELEASE:
            ENV1_VALUE -= ENV1_R_COEF;
            ENV1_COUNTER++;
            if(ENV1_COUNTER > ENV1b_RELEASE)
            {
                ENV1_VALUE = 0.0f;
                ENV1_COUNTER = 0;
                ENV1_STAGE = 0; /* Stop the rock ENV1 */
                OSC1_SPEED = 0;
                OSC3_SPEED = 0;
                *track = PLAYING_NOSAMPLE;
            }
            break;
    }

    /* ENV2 */
    switch(ENV2_STAGE)
    {
        /* Attack */
        case SYNTH_ATTACK:
            ENV2_VALUE += ENV2_A_COEF;
            ENV2_COUNTER++;
            if(ENV2_COUNTER >= ENV2b_ATTACK)
            {
                if(ENV2_VALUE > 1.0f) ENV2_VALUE = 1.0f;
                ENV2_COUNTER = 0;
                ENV2_STAGE = SYNTH_DECAY;
            }
            break;

        /* Decay */
        case SYNTH_DECAY:
            ENV2_VALUE -= ENV2_D_COEF;
            ENV2_COUNTER++;
            if(ENV2_COUNTER >= ENV2b_DECAY)
            {
                if(ENV2_VALUE < 0.0f) ENV2_VALUE = 0.0f;
                ENV2_VALUE = Data.ENV2_SUSTAIN;
                ENV2_COUNTER = 0;
                ENV2_STAGE = SYNTH_SUSTAIN;
            }
            break;

        /* Sustain */
        case SYNTH_SUSTAIN:
            ENV2_VALUE = Data.ENV2_SUSTAIN;
            break;

        /* Release */
        case SYNTH_RELEASE:
            ENV2_VALUE -= ENV2_R_COEF;
            ENV2_COUNTER++;
            if(ENV2_COUNTER > ENV2b_RELEASE)
            {
                ENV2_VALUE = 0.0f;
                ENV2_COUNTER = 0;
                ENV2_STAGE = 0; /* Stop the rock ENV2 */
                OSC2_SPEED = 0;
                *track2 = PLAYING_NOSAMPLE;
            }
            break;
    }

    ENV1_VOLUME = ENV1_VALUE;
#if defined(PTK_SYNTH_ENV1)
    ENV1_VOLUME *= Data.ENV1_OSC1_VOLUME;
#endif
#if defined(PTK_SYNTH_ENV2)
    ENV1_VOLUME *= Data.ENV2_OSC1_VOLUME;
#endif

    ENV2_VOLUME = ENV2_VALUE;
#if defined(PTK_SYNTH_ENV1)
    ENV2_VOLUME *= Data.ENV1_OSC2_VOLUME;
#endif
#if defined(PTK_SYNTH_ENV2)
    ENV2_VOLUME *= Data.ENV2_OSC2_VOLUME;
#endif

#if !defined(PTK_SYNTH_LFO1) && !defined(PTK_SYNTH_LFO2)
    ENV1_MIN = 1.0f;
#else
    ENV1_MIN = 0.0f;
    if(
#if defined(PTK_SYNTH_LFO1)
    Data.LFO1_OSC1_VOLUME == 0.0f
#else
    TRUE
#endif
    &&
#if defined(PTK_SYNTH_LFO2)
    Data.LFO2_OSC1_VOLUME == 0.0f
#else
    TRUE
#endif
    )
    {
        ENV1_MIN = 1.0f;
    }
#endif

#if !defined(PTK_SYNTH_LFO1) && !defined(PTK_SYNTH_LFO2)
    ENV2_MIN = 1.0f;
#else
    ENV2_MIN = 0.0f;
    if(
#if defined(PTK_SYNTH_LFO1)
    Data.LFO1_OSC2_VOLUME == 0.0f
#else
    TRUE
#endif
    &&
#if defined(PTK_SYNTH_LFO2)
    Data.LFO2_OSC2_VOLUME == 0.0f
#else
    TRUE
#endif
    )
    {
        ENV2_MIN = 1.0f;
    }
#endif
}

// ------------------------------------------------------
// 'Note Off' message for CSynth class objects
void CSynth::NoteOff(void)
{
    if(ENV1_STAGE > PLAYING_NOSAMPLE && ENV1_STAGE < SYNTH_RELEASE)
    {
        ENV1_R_COEF = ENV1_VALUE / (float) ENV1b_RELEASE;
        ENV1_COUNTER = 0;
        ENV1_STAGE = SYNTH_RELEASE;
    }

    if(ENV2_STAGE > PLAYING_NOSAMPLE && ENV2_STAGE < SYNTH_RELEASE)
    {
        ENV2_R_COEF = ENV2_VALUE / (float) ENV2b_RELEASE;
        ENV2_COUNTER = 0;
        ENV2_STAGE = SYNTH_RELEASE;
    }

#if defined(PTK_SYNTH_LFO1)
    if(LFO1_STAGE > PLAYING_NOSAMPLE && LFO1_STAGE < SYNTH_RELEASE)
    {
        LFO1_R_COEF = LFO1_ADSR_VALUE / (float) LFO1b_RELEASE;
        LFO1_COUNTER = 0;
        LFO1_STAGE = SYNTH_RELEASE;
    }
#endif

#if defined(PTK_SYNTH_LFO2)
    if(LFO2_STAGE > PLAYING_NOSAMPLE && LFO2_STAGE < SYNTH_RELEASE)
    {
        LFO2_R_COEF = LFO2_ADSR_VALUE / (float) LFO2b_RELEASE;
        LFO2_COUNTER = 0;
        LFO2_STAGE = SYNTH_RELEASE;
    }
#endif

}

// ------------------------------------------------------
// The cool/render function, gets the next synth sample.
// (Used at all ticks)
float CSynth::GetSample(short *Left_Samples,
                        short *Right_Samples,
                        char Stereo,
                        char Loop_Type,
                        unsigned int Length,
                        unsigned int Loop_Sub,
                        float *Right_Signal,
                        float vol,
                        int *track,
                        int *track2,
                        Uint64 *position_osc1,
                        Uint64 *position_osc2,
#if defined(PTK_SYNTH_OSC3)
                        Uint64 *position_osc3,
#endif
                        int64 osc_speed,
                        float Ampi_Vol)
    {

    s_access *pos_osc1 = (s_access *) position_osc1;
    s_access *pos_osc2 = (s_access *) position_osc2;

#if defined(PTK_SYNTH_OSC3)
    s_access *pos_osc3 = (s_access *) position_osc3;
#endif

    short *Left_Samples1 = NULL;
    short *Right_Samples1 = NULL;
    unsigned int res_dec;

#if defined(PTK_SYNTH_PITCH)
    int64 osc_speed1;
    int64 osc_speed1b;
#endif
    int64 osc_speed2;

#if defined(PTK_SYNTH_OSC2)
    int64 osc_speed_tune;
#endif

    float mul_datL;
    float mul_datR;
    unsigned int i_POSITION[4];

    GS_VAL = 0;
    GS_VAL2 = 0;

    char Loop_Type1 = Loop_Type;
    char Loop_Type2 = Loop_Type;

    unsigned int Loop_Sub1 = Loop_Sub;
    unsigned int Loop_Sub2 = Loop_Sub;
    unsigned int Length1 = Length;
    unsigned int Length2 = Length;

// ------------------------------------------------
// Oscillator 1

    if(ENV1_STAGE)
    {
        // Oscillator1 On
        if(Data.OSC1_WAVEFORM != WAVEFORM_NONE)
        {
            T_OSC1_VOLUME = (
#if defined(PTK_SYNTH_LFO1)
                             LFO1_VALUE * Data.LFO1_OSC1_VOLUME
#endif
#if defined(PTK_SYNTH_LFO2)
                             + LFO2_VALUE * Data.LFO2_OSC1_VOLUME
#endif
                             + ENV1_MIN
                            )
                            * ENV1_VOLUME
                           ;
            if(Data.OSC1_WAVEFORM == WAVEFORM_NONE) T_OSC1_VOLUME *= Ampi_Vol;

            if(*track)
            {
                if(Data.PTC_GLIDE64 != 0 && OSC1_SPEED != 0)
                {
                    if(osc_speed > OSC1_SPEED)
                    {
                        OSC1_SPEED += Data.PTC_GLIDE64;
                        if(OSC1_SPEED > osc_speed) OSC1_SPEED = osc_speed;
                    }
                    else
                    {
                        OSC1_SPEED -= Data.PTC_GLIDE64;
                        if(OSC1_SPEED < osc_speed) OSC1_SPEED = osc_speed;
                    }
                }
                else
                {
                    OSC1_SPEED = osc_speed;
                }

#if defined(PTK_SYNTH_PITCH)
                osc_speed1 = (int64) ((double)
                             (
#if defined(PTK_SYNTH_LFO1_PITCH)
                              LFO1_VALUE * Data.LFO1_OSC1_PITCH
#endif
#if defined(PTK_SYNTH_LFO2_PITCH)
                            + LFO2_VALUE * Data.LFO2_OSC1_PITCH
#endif
#if defined(PTK_SYNTH_ENV1_PITCH)
                            + ENV1_VALUE * Data.ENV1_OSC1_PITCH
#endif
#if defined(PTK_SYNTH_ENV2_PITCH)
                            + ENV2_VALUE * Data.ENV2_OSC1_PITCH
#endif
                           ) * 4294967296.0);
#endif

                osc_speed2 = OSC1_SPEED;
                Left_Samples1 = Left_Samples;
                Right_Samples1 = Right_Samples;

#if defined(PTK_INSTRUMENTS)
                if(Data.OSC1_WAVEFORM != WAVEFORM_WAV)
#endif
                {

#if defined(PTK_SYNTH_SIN) || defined(PTK_SYNTH_SAW) || defined(PTK_SYNTH_PULSE) || defined(PTK_SYNTH_WHITE) || defined(PTK_SYNTH_PINK)

                    switch(Data.OSC1_WAVEFORM)
                    {

#if defined(PTK_SYNTH_SIN)
                        case WAVEFORM_SIN:
                            Left_Samples1 = STOCK_SIN;
                            Right_Samples1 = STOCK_SIN;
                            break;
#endif

#if defined(PTK_SYNTH_SAW)
                        case WAVEFORM_SAW:
                            Left_Samples1 = STOCK_SAW;
                            Right_Samples1 = STOCK_SAW;
                            break;
#endif

#if defined(PTK_SYNTH_PULSE)
                        case WAVEFORM_PULSE:
                            Left_Samples1 = STOCK_PULSE;
                            Right_Samples1 = STOCK_PULSE;
                            break;
#endif

#if defined(PTK_SYNTH_WHITE)
                        case WAVEFORM_WHITE:
                            Left_Samples1 = STOCK_WHITE;
                            Right_Samples1 = STOCK_WHITE;
                            break;
#endif

#if defined(PTK_SYNTH_PINK)
                        case WAVEFORM_PINK:
                            Left_Samples1 = STOCK_PINK;
                            Right_Samples1 = STOCK_PINK;
                            break;
#endif

                    }

#endif

#if defined(PTK_SYNTH_PITCH)
                    osc_speed1 *= 65;
#endif
                    osc_speed2 *= 65;

                    Length1 = SIZE_WAVEFORMS;
                    Loop_Sub1 = SIZE_WAVEFORMS;
                    Loop_Type1 = SMP_LOOP_FORWARD;
                }

                if(Left_Samples1)
                {
                    res_dec = pos_osc1->half.last;
                    
                    Set_Spline_Boundaries(pos_osc1->half.first,
                                          i_POSITION,
                                          Loop_Type1,
                                          ENV1_LOOP_BACKWARD,
                                          Length1,
                                          Length1,
                                          Length1 - Loop_Sub1);

                    mul_datL = 1.0f;
                    mul_datR = 1.0f;

#if defined(PTK_SYNTH_PHASE1)
                    if(Data.OSC1_PW)
                    {
                        T_OSC_PW = Data.OSC1_PW * 
                                (
#if defined(PTK_SYNTH_LFO1)
                                   (LFO1_VALUE * Data.LFO1_OSC1_PW) 
#endif
#if defined(PTK_SYNTH_LFO2)
                                 + (LFO2_VALUE * Data.LFO2_OSC1_PW)
#endif
#if defined(PTK_SYNTH_ENV1)
                                 + (ENV1_VALUE * Data.ENV1_OSC1_PW)
#endif
#if defined(PTK_SYNTH_ENV2)
                                 + (ENV2_VALUE * Data.ENV2_OSC1_PW)
#endif
                                );
                        if(*(Left_Samples1 + i_POSITION[0]) > 0) mul_datL = T_OSC_PW * 2.0f;
                        if(Stereo == 2) if(*(Right_Samples1 + i_POSITION[0]) > 0) mul_datR = T_OSC_PW * 2.0f;
                    }
#endif

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
#if defined(PTK_USE_CUBIC)
                    GS_VAL = (Cubic_Work(
                                (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                res_dec)) * T_OSC1_VOLUME;
#elif defined(PTK_USE_SPLINE)
                    GS_VAL = (Spline_Work(
                                (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                res_dec)) * T_OSC1_VOLUME;
#else
                    GS_VAL = (*(Left_Samples1 + i_POSITION[0]) * mul_datL)
                              * T_OSC1_VOLUME;
#endif

#else
                    switch(Use_Cubic)
                    {
                        case CUBIC_INT:
                            GS_VAL = (Cubic_Work(
                                        (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                        (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                        (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                        (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                        res_dec)) * T_OSC1_VOLUME;
                            break;
                        case SPLINE_INT:
                            GS_VAL = (Spline_Work(
                                        (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                        (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                        (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                        (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                        res_dec)) * T_OSC1_VOLUME;
                            break;
                        default:
                            GS_VAL = (*(Left_Samples1 + i_POSITION[0]) * mul_datL)
                                      * T_OSC1_VOLUME;
                            break;
                    }
#endif

                    // Stereo sample
                    if(Stereo == 2)
                    {

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
#if defined(PTK_USE_CUBIC)
                        GS_VAL2 = (Cubic_Work(
                                     (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                     (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                     (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                     (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                     res_dec)) * T_OSC1_VOLUME;
#elif defined(PTK_USE_SPLINE)
                        GS_VAL2 = (Spline_Work(
                                     (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                     (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                     (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                     (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                     res_dec)) * T_OSC1_VOLUME;
#else
                        GS_VAL2 = (*(Right_Samples1 + i_POSITION[0]) * mul_datR)
                                   * T_OSC1_VOLUME;
#endif

#else
                        switch(Use_Cubic)
                        {
                            case CUBIC_INT:
                                GS_VAL2 = (Cubic_Work(
                                             (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                             (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                             (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                             (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                             res_dec)) * T_OSC1_VOLUME;
                                break;
                            case SPLINE_INT:
                                GS_VAL2 = (Spline_Work(
                                             (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                             (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                             (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                             (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                             res_dec)) * T_OSC1_VOLUME;
                                break;
                            default:
                                GS_VAL2 = (*(Right_Samples1 + i_POSITION[0]) * mul_datR)
                                           * T_OSC1_VOLUME;
                                break;
                        }
#endif
                    }


#if defined(PTK_SYNTH_PITCH)
                    osc_speed2 += osc_speed1;
                    if(osc_speed2 < 16) osc_speed2 = 16;
#endif

                    if(ENV1_LOOP_BACKWARD == TRUE)
                    {
                        if(pos_osc1->half.first > 0)
                        {
                            pos_osc1->absolu -= osc_speed2;
                        }
                    }
                    else pos_osc1->absolu += osc_speed2;

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    switch(Loop_Type1)
                    {
                        case SMP_LOOP_NONE:
#endif
                            if(ENV1_LOOP_BACKWARD)
                            {
                                if((int) pos_osc1->half.first <= 0)
                                {
                                    pos_osc1->half.first = 0;
                                    *track = PLAYING_NOSAMPLE;
                                }
                            }
                            else
                            {
                                if(pos_osc1->half.first >= Length1)
                                {
                                    pos_osc1->half.first = Length1;
                                    *track = PLAYING_NOSAMPLE;
                                }
                            }
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                            break;
#endif

#if defined(PTK_LOOP_FORWARD)
                        case SMP_LOOP_FORWARD:
                            if(ENV1_LOOP_BACKWARD)
                            {
                                if((int) pos_osc1->half.first <= (int) (Length1 - Loop_Sub1))
                                {
                                    pos_osc1->half.first += Length1;
                                }
                            }
                            else
                            {
                                if(pos_osc1->half.first >= Length1)
                                {
                                    pos_osc1->half.first -= Loop_Sub1;
                                }
                            }
                            break;
#endif

#if defined(PTK_LOOP_PINGPONG)
                        case SMP_LOOP_PINGPONG:
                            if(ENV1_LOOP_BACKWARD)
                            {
                                if((int) pos_osc1->half.first <= (int) (Length1 - Loop_Sub1))
                                {
                                    pos_osc1->half.first = Length1 - Loop_Sub1;
                                    ENV1_LOOP_BACKWARD = FALSE;
                                }
                            }
                            else
                            {
                                if(pos_osc1->half.first >= Length1)
                                {
                                    pos_osc1->half.first = Length1;
                                    ENV1_LOOP_BACKWARD = TRUE;
                                }
                            }
                            break;
#endif
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    }
#endif

               }
            }
        }
    }

// ------------------------------------------------
// Oscillator 3 (sub)

#if defined(PTK_SYNTH_OSC3)
    if(Data.OSC3_SWITCH)
    {
        // SubOscillator On
        if(Data.OSC1_WAVEFORM != WAVEFORM_NONE)
        {
            if(*track)
            {
                osc_speed2 = OSC1_SPEED / 2;

#if defined(PTK_SYNTH_PITCH)
                osc_speed1 = ((int64) ((double) (
#if defined(PTK_SYNTH_LFO1_PITCH)
                                + LFO1_VALUE * Data.LFO1_OSC1_PITCH
#endif
#if defined(PTK_SYNTH_LFO2_PITCH)
                                + LFO2_VALUE * Data.LFO2_OSC1_PITCH
#endif
#if defined(PTK_SYNTH_ENV1_PITCH)
                                + ENV1_VALUE * Data.ENV1_OSC1_PITCH
#endif
#if defined(PTK_SYNTH_ENV2_PITCH)
                                + ENV2_VALUE * Data.ENV2_OSC1_PITCH
#endif
                               ) * 4294967296.0)) / 2;
#endif

#if defined(PTK_INSTRUMENTS)
                if(Data.OSC1_WAVEFORM != WAVEFORM_WAV)
#endif
                {
                    osc_speed2 *= 65;

#if defined(PTK_SYNTH_PITCH)
                    osc_speed1 *= 65;
#endif
                    Length = SIZE_WAVEFORMS;
                    Loop_Sub = SIZE_WAVEFORMS;
                    Loop_Type = SMP_LOOP_FORWARD;
                    Stereo = 1;
                }

                if(Left_Samples1)
                {
                    res_dec = pos_osc3->half.last;

                    Set_Spline_Boundaries(pos_osc3->half.first,
                                          i_POSITION,
                                          Loop_Type,
                                          ENV2_LOOP_BACKWARD,
                                          Length,
                                          Length,
                                          Length - Loop_Sub);

                    mul_datL = 1.0f;
                    mul_datR = 1.0f;

#if defined(PTK_SYNTH_PHASE1)
                    if(Data.OSC1_PW)
                    {
                        T_OSC_PW = Data.OSC1_PW * 
                               (
#if defined(PTK_SYNTH_LFO1)
                                  (LFO1_VALUE * Data.LFO1_OSC1_PW) 
#endif
#if defined(PTK_SYNTH_LFO2)
                                + (LFO2_VALUE * Data.LFO2_OSC1_PW)
#endif
#if defined(PTK_SYNTH_ENV1)
                                + (ENV1_VALUE * Data.ENV1_OSC1_PW)
#endif
#if defined(PTK_SYNTH_ENV2)
                                + (ENV2_VALUE * Data.ENV2_OSC1_PW)
#endif
                                );

                        if(*(Left_Samples1 + i_POSITION[0]) > 0) mul_datL = T_OSC_PW * 2.0f;
                        if(Stereo == 2) if(*(Right_Samples1 + i_POSITION[0]) > 0) mul_datR = T_OSC_PW * 2.0f;
                    }
#endif

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
#if defined(PTK_USE_CUBIC)
                    GS_VAL += (Cubic_Work(
                                    (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                    (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                    (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                    (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                    res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
#elif defined(PTK_USE_SPLINE)
                    GS_VAL += (Spline_Work(
                                    (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                    (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                    (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                    (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                    res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
#else
                    GS_VAL += (*(Left_Samples1 + i_POSITION[0]) * mul_datL)
                               * T_OSC1_VOLUME * Data.OSC3_VOLUME;
#endif

#else
                    switch(Use_Cubic)
                    {
                        case CUBIC_INT:
                            GS_VAL += (Cubic_Work(
                                            (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                            (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                            (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                            (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                            res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
                            break;
                        case SPLINE_INT:
                            GS_VAL += (Spline_Work(
                                            (float) (*(Left_Samples1 + i_POSITION[3])) * mul_datL,
                                            (float) (*(Left_Samples1 + i_POSITION[0])) * mul_datL,
                                            (float) (*(Left_Samples1 + i_POSITION[1])) * mul_datL,
                                            (float) (*(Left_Samples1 + i_POSITION[2])) * mul_datL,
                                            res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
                            break;
                        default:
                            GS_VAL += (*(Left_Samples1 + i_POSITION[0]) * mul_datL)
                                       * T_OSC1_VOLUME * Data.OSC3_VOLUME;
                            break;
                    }
#endif

                    if(Stereo == 2)
                    {

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
#if defined(PTK_USE_CUBIC)
                        GS_VAL2 += (Cubic_Work(
                                        (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                        (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                        (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                        (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                        res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
#elif defined(PTK_USE_SPLINE)
                        GS_VAL2 += (Spline_Work(
                                        (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                        (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                        (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                        (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                        res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
#else
                        GS_VAL2 += (*(Right_Samples1 + i_POSITION[0]) * mul_datR)
                                    * T_OSC1_VOLUME * Data.OSC3_VOLUME;
#endif

#else
                        switch(Use_Cubic)
                        {
                            case CUBIC_INT:
                                GS_VAL2 += (Cubic_Work(
                                                (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                                (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                                (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                                (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                                res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
                                break;
                            case SPLINE_INT:
                                GS_VAL2 += (Spline_Work(
                                                (float) (*(Right_Samples1 + i_POSITION[3])) * mul_datR,
                                                (float) (*(Right_Samples1 + i_POSITION[0])) * mul_datR,
                                                (float) (*(Right_Samples1 + i_POSITION[1])) * mul_datR,
                                                (float) (*(Right_Samples1 + i_POSITION[2])) * mul_datR,
                                                res_dec)) * T_OSC1_VOLUME * Data.OSC3_VOLUME;
                                break;
                            default:
                                GS_VAL2 += (*(Right_Samples1 + i_POSITION[0]) * mul_datR)
                                            * T_OSC1_VOLUME * Data.OSC3_VOLUME;
                                break;
                        }
#endif

                    }
                }

#if defined(PTK_SYNTH_PITCH)
                osc_speed2 += osc_speed1;
                if(osc_speed2 < 16) osc_speed2 = 16;
#endif

                if(ENV3_LOOP_BACKWARD == TRUE)
                {
                    if(pos_osc3->half.first > 0)
                    {
                        pos_osc3->absolu -= osc_speed2;
                    }
                }
                else pos_osc3->absolu += osc_speed2;

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                switch(Loop_Type)
                {
                    case SMP_LOOP_NONE:
#endif
                        if(ENV3_LOOP_BACKWARD)
                        {
                            if((int) pos_osc3->half.first <= 0)
                            {
                                pos_osc3->half.first = 0;
                                *track = PLAYING_NOSAMPLE;
                            }
                        }
                        else
                        {
                            if(pos_osc3->half.first >= Length)
                            {
                                pos_osc3->half.first = Length;
                                *track = PLAYING_NOSAMPLE;
                            }
                        }
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                        break;
#endif

#if defined(PTK_LOOP_FORWARD)
                    case SMP_LOOP_FORWARD:
                        if(ENV3_LOOP_BACKWARD)
                        {
                            if((int) pos_osc3->half.first <= (int) (Length - Loop_Sub))
                            {
                                pos_osc3->half.first += Length;
                            }
                        }
                        else
                        {
                            if(pos_osc3->half.first >= Length)
                            {
                                pos_osc3->half.first -= Loop_Sub;
                            }
                        }
                        break;
#endif

#if defined(PTK_LOOP_PINGPONG)
                    case SMP_LOOP_PINGPONG:
                        if(ENV3_LOOP_BACKWARD)
                        {
                            if((int) pos_osc3->half.first <= (int) (Length - Loop_Sub))
                            {
                                pos_osc3->half.first = Length - Loop_Sub;
                                ENV3_LOOP_BACKWARD = FALSE;
                            }
                        }
                        else
                        {
                            if(pos_osc3->half.first >= Length)
                            {
                                pos_osc3->half.first = Length;
                                ENV3_LOOP_BACKWARD = TRUE;
                            }
                        }
                        break;
#endif
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                }
#endif
            }
        }
    }
#endif // PTK_SYNTH_OSC3

// ------------------------------------------------
// Oscillator 2

#if defined(PTK_SYNTH_OSC2)
    if(ENV2_STAGE)
    {
        // Oscillator2 On 
        if(Data.OSC2_WAVEFORM != WAVEFORM_NONE)
        {
            if(*track2)
            {
                osc_speed_tune = osc_speed + (int64) ((double) Data.OSC2_FINETUNE * 536870912.0)
                                           + (int64) ((double) Data.OSC2_DETUNE * 536870912.0);
                if(Data.PTC_GLIDE64 != 0 && OSC2_SPEED != 0)
                {
                    if(osc_speed_tune > OSC2_SPEED)
                    {
                        OSC2_SPEED += Data.PTC_GLIDE64;
                        if(OSC2_SPEED > osc_speed_tune) OSC2_SPEED = osc_speed_tune;
                    }
                    else
                    {
                        OSC2_SPEED -= Data.PTC_GLIDE64;
                        if(OSC2_SPEED < osc_speed_tune) OSC2_SPEED = osc_speed_tune;
                    }
                }
                else
                {
                    OSC2_SPEED = osc_speed_tune;
                }

#if defined(PTK_SYNTH_PITCH)
                osc_speed1b = ((int64) ((double)
                               (
#if defined(PTK_SYNTH_LFO1_PITCH)
                                  LFO1_VALUE * Data.LFO1_OSC2_PITCH
#endif
#if defined(PTK_SYNTH_LFO2_PITCH)
                                + LFO2_VALUE * Data.LFO2_OSC2_PITCH
#endif
#if defined(PTK_SYNTH_ENV1_PITCH)
                                + ENV1_VALUE * Data.ENV1_OSC2_PITCH
#endif
#if defined(PTK_SYNTH_ENV2_PITCH)
                                + ENV2_VALUE * Data.ENV2_OSC2_PITCH
#endif
                               ) * 4294967296.0));
#endif

                osc_speed2 = OSC2_SPEED;

#if defined(PTK_INSTRUMENTS)
                if(Data.OSC2_WAVEFORM != WAVEFORM_WAV)
#endif
                {
                    switch(Data.OSC2_WAVEFORM)
                    {

#if defined(PTK_SYNTH_SIN)
                        case WAVEFORM_SIN:
                            Left_Samples = STOCK_SIN;
                            Right_Samples = STOCK_SIN;
                            break;
#endif

#if defined(PTK_SYNTH_SAW)
                        case WAVEFORM_SAW:
                            Left_Samples = STOCK_SAW;
                            Right_Samples = STOCK_SAW;
                            break;
#endif

#if defined(PTK_SYNTH_PULSE)
                        case WAVEFORM_PULSE:
                            Left_Samples = STOCK_PULSE;
                            Right_Samples = STOCK_PULSE;
                            break;
#endif

#if defined(PTK_SYNTH_WHITE)
                        case WAVEFORM_WHITE:
                            Left_Samples = STOCK_WHITE;
                            Right_Samples = STOCK_WHITE;
                            break;
#endif

#if defined(PTK_SYNTH_PINK)
                        case WAVEFORM_PINK:
                            Left_Samples = STOCK_PINK;
                            Right_Samples = STOCK_PINK;
                            break;
#endif

                    }

#if defined(PTK_SYNTH_PITCH)
                    osc_speed1b *= 65;
#endif

                    osc_speed2 *= 65;
                    Length2 = SIZE_WAVEFORMS;
                    Loop_Sub2 = SIZE_WAVEFORMS;
                    Loop_Type2 = SMP_LOOP_FORWARD;
                }

                if(Left_Samples)
                {
                    T_OSC2_VOLUME = ((
#if defined(PTK_SYNTH_LFO1)
                                    LFO1_VALUE * Data.LFO1_OSC2_VOLUME
#endif
#if defined(PTK_SYNTH_LFO2)
                                    + LFO2_VALUE * Data.LFO2_OSC2_VOLUME
#endif
                                    + 0) + ENV2_MIN)
                                    * ENV2_VOLUME;

                    if(Data.OSC2_WAVEFORM == WAVEFORM_NONE) T_OSC2_VOLUME *= Ampi_Vol;

                    res_dec = pos_osc2->half.last;

                    Set_Spline_Boundaries(pos_osc2->half.first,
                                          i_POSITION,
                                          Loop_Type2,
                                          ENV2_LOOP_BACKWARD,
                                          Length2,
                                          Length2,
                                          Length2 - Loop_Sub2);

                    mul_datL = 1.0f;
                    mul_datR = 1.0f;

#if defined(PTK_SYNTH_PHASE2)
                    if(Data.OSC2_PW)
                    {
                        T_OSC_PW = Data.OSC2_PW * 
                                  (
#if defined(PTK_SYNTH_LFO1)
                                    (LFO1_VALUE * Data.LFO1_OSC2_PW) 
#endif
#if defined(PTK_SYNTH_LFO2)
                                    + (LFO2_VALUE * Data.LFO2_OSC2_PW)
#endif
#if defined(PTK_SYNTH_ENV1)
                                    + (ENV1_VALUE * Data.ENV1_OSC2_PW)
#endif
#if defined(PTK_SYNTH_ENV2)
                                    + (ENV2_VALUE * Data.ENV2_OSC2_PW)
#endif
                                   );
                        if(*(Left_Samples + i_POSITION[0]) > 0) mul_datL = T_OSC_PW * 2.0f;
                        if(Stereo == 2) if(*(Right_Samples + i_POSITION[0]) > 0) mul_datR = T_OSC_PW * 2.0f;
                    }
#endif

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
#if defined(PTK_USE_CUBIC)
                    GS_VAL = Math_Func(GS_VAL, Cubic_Work((float) (*(Left_Samples + i_POSITION[3])) * mul_datL,
                                                          (float) (*(Left_Samples + i_POSITION[0])) * mul_datL,
                                                          (float) (*(Left_Samples + i_POSITION[1])) * mul_datL,
                                                          (float) (*(Left_Samples + i_POSITION[2])) * mul_datL,
                                                          res_dec) * T_OSC2_VOLUME);
#elif defined(PTK_USE_SPLINE)
                    GS_VAL = Math_Func(GS_VAL, Spline_Work((float) (*(Left_Samples + i_POSITION[3])) * mul_datL,
                                                           (float) (*(Left_Samples + i_POSITION[0])) * mul_datL,
                                                           (float) (*(Left_Samples + i_POSITION[1])) * mul_datL,
                                                           (float) (*(Left_Samples + i_POSITION[2])) * mul_datL,
                                                           res_dec) * T_OSC2_VOLUME);
#else
                    GS_VAL = Math_Func(GS_VAL, *(Left_Samples + i_POSITION[0]) * mul_datL) * T_OSC2_VOLUME;
#endif

#else
                    switch(Use_Cubic)
                    {
                        case CUBIC_INT:
                            GS_VAL = Math_Func(GS_VAL, Cubic_Work((float) (*(Left_Samples + i_POSITION[3])) * mul_datL,
                                                                  (float) (*(Left_Samples + i_POSITION[0])) * mul_datL,
                                                                  (float) (*(Left_Samples + i_POSITION[1])) * mul_datL,
                                                                  (float) (*(Left_Samples + i_POSITION[2])) * mul_datL,
                                                                  res_dec) * T_OSC2_VOLUME);
                            break;
                        case SPLINE_INT:
                            GS_VAL = Math_Func(GS_VAL, Spline_Work((float) (*(Left_Samples + i_POSITION[3])) * mul_datL,
                                                                   (float) (*(Left_Samples + i_POSITION[0])) * mul_datL,
                                                                   (float) (*(Left_Samples + i_POSITION[1])) * mul_datL,
                                                                   (float) (*(Left_Samples + i_POSITION[2])) * mul_datL,
                                                                   res_dec) * T_OSC2_VOLUME);
                            break;
                        default:
                            GS_VAL = Math_Func(GS_VAL, (*(Left_Samples + i_POSITION[0]) * mul_datL) * T_OSC2_VOLUME);
                            break;
                    }
#endif

                    // Stereo sample
                    if(Stereo == 2)
                    {

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
#if defined(PTK_USE_CUBIC)
                        GS_VAL2 = Math_Func(GS_VAL2, Cubic_Work((float) (*(Right_Samples + i_POSITION[3])) * mul_datR,
                                                                (float) (*(Right_Samples + i_POSITION[0])) * mul_datR,
                                                                (float) (*(Right_Samples + i_POSITION[1])) * mul_datR,
                                                                (float) (*(Right_Samples + i_POSITION[2])) * mul_datR,
                                                                res_dec) * T_OSC2_VOLUME);
#elif defined(PTK_USE_SPLINE)
                        GS_VAL2 = Math_Func(GS_VAL2, Spline_Work((float) (*(Right_Samples + i_POSITION[3])) * mul_datR,
                                                                 (float) (*(Right_Samples + i_POSITION[0])) * mul_datR,
                                                                 (float) (*(Right_Samples + i_POSITION[1])) * mul_datR,
                                                                 (float) (*(Right_Samples + i_POSITION[2])) * mul_datR,
                                                                 res_dec) * T_OSC2_VOLUME);
#else
                        GS_VAL2 = Math_Func(GS_VAL2, (*(Right_Samples + i_POSITION[0]) * mul_datR) * T_OSC2_VOLUME);
#endif

#else
                        switch(Use_Cubic)
                        {
                            case CUBIC_INT:
                                GS_VAL2 = Math_Func(GS_VAL2, Cubic_Work((float) (*(Right_Samples + i_POSITION[3])) * mul_datR,
                                                                        (float) (*(Right_Samples + i_POSITION[0])) * mul_datR,
                                                                        (float) (*(Right_Samples + i_POSITION[1])) * mul_datR,
                                                                        (float) (*(Right_Samples + i_POSITION[2])) * mul_datR,
                                                                        res_dec) * T_OSC2_VOLUME);
                                break;
                            case SPLINE_INT:
                                GS_VAL2 = Math_Func(GS_VAL2, Spline_Work((float) (*(Right_Samples + i_POSITION[3])) * mul_datR,
                                                                         (float) (*(Right_Samples + i_POSITION[0])) * mul_datR,
                                                                         (float) (*(Right_Samples + i_POSITION[1])) * mul_datR,
                                                                         (float) (*(Right_Samples + i_POSITION[2])) * mul_datR,
                                                                         res_dec) * T_OSC2_VOLUME);
                                break;
                            default:
                                GS_VAL2 = Math_Func(GS_VAL2, (*(Right_Samples + i_POSITION[0]) * mul_datR) * T_OSC2_VOLUME);
                                break;
                        }
#endif
                    }

#if defined(PTK_SYNTH_PITCH)
                    osc_speed2 += osc_speed1b;
                    if(osc_speed2 < 16) osc_speed2 = 16;
#endif

                    if(ENV2_LOOP_BACKWARD == TRUE)
                    {
                        if(pos_osc2->half.first > 0)
                        {
                            pos_osc2->absolu -= osc_speed2;
                        }
                    }
                    else
                    {
                        pos_osc2->absolu += osc_speed2;
                    }

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    switch(Loop_Type2)
                    {
                        case SMP_LOOP_NONE:
#endif
                            if(ENV2_LOOP_BACKWARD)
                            {
                                if((int) pos_osc2->half.first <= 0)
                                {
                                    pos_osc2->half.first = 0;
                                    *track2 = PLAYING_NOSAMPLE;
                                }
                            }
                            else
                            {
                                if(pos_osc2->half.first >= Length2)
                                {
                                    pos_osc2->half.first = Length2;
                                    *track2 = PLAYING_NOSAMPLE;
                                }
                            }
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                            break;
#endif

#if defined(PTK_LOOP_FORWARD)
                        case SMP_LOOP_FORWARD:
                            if(ENV2_LOOP_BACKWARD)
                            {
                                if((int) pos_osc2->half.first <= (int) (Length2 - Loop_Sub2))
                                {
                                    pos_osc2->half.first += Length2;
                                }
                            }
                            else
                            {
                                if(pos_osc2->half.first >= Length2)
                                {
                                    pos_osc2->half.first -= Loop_Sub2;
                                }
                            }
                            break;
#endif

#if defined(PTK_LOOP_PINGPONG)
                        case SMP_LOOP_PINGPONG:
                            if(ENV2_LOOP_BACKWARD)
                            {
                                if((int) pos_osc2->half.first <= (int) (Length2 - Loop_Sub2))
                                {
                                    pos_osc2->half.first = Length2 - Loop_Sub2;
                                    ENV2_LOOP_BACKWARD = FALSE;
                                }
                            }
                            else
                            {
                                if(pos_osc2->half.first >= Length2)
                                {
                                    pos_osc2->half.first = Length2;
                                    ENV2_LOOP_BACKWARD = TRUE;
                                }
                            }
                            break;
#endif
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    }
#endif

                }
            }
        }
    }
#endif // PTK_SYNTH_OSC2

// ------------------------------------------------
// Left signal

#if defined(PTK_SYNTH_DISTO)
    if(Data.DISTO != 1.0f)
    {
        GS_VAL /= 32767.0f;
        GS_VAL = GS_VAL * (fabsf(GS_VAL) + Data.DISTO) / ((GS_VAL * GS_VAL) + (Data.DISTO - 1) *
                 fabsf(GS_VAL) + 1);
        GS_VAL *= 32767.0f;
    }
#endif

#if defined(PTK_SYNTH_FILTER)
    if(Data.VCF_TYPE < 2)
    {
        FILT_CUTO = Data.VCF_CUTOFF
#if defined(PTK_SYNTH_LFO1)
                    + LFO1_VALUE * Data.LFO1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_LFO2)
                    + LFO2_VALUE * Data.LFO2_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV1)
                    + ENV1_VALUE * Data.ENV1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV2)
                    + ENV2_VALUE * Data.ENV2_VCF_CUTOFF
#endif
                   ;

        FILT_RESO = Data.VCF_RESONANCE
#if defined(PTK_SYNTH_LFO1)
                    + LFO1_VALUE * Data.LFO1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_LFO2)
                    + LFO2_VALUE * Data.LFO2_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV1)
                    + ENV1_VALUE * Data.ENV1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV2)
                    + ENV2_VALUE * Data.ENV2_VCF_RESONANCE
#endif
                   ;
        if(FILT_CUTO < 0.05f) FILT_CUTO = 0.05f;
        if(FILT_CUTO > 0.90f) FILT_CUTO = 0.90f;
        if(FILT_RESO < 0.05f) FILT_RESO = 0.05f;
        if(FILT_RESO > 0.98f) FILT_RESO = 0.98f;
        FILT_A = float(1.0f - FILT_CUTO); 
        FILT_B = float(FILT_RESO * (1.0f + (1.0f / FILT_A)));
        GS_VAL = FilterL();
    }

#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
    else if(Data.VCF_TYPE > 2)
    {
            FILT_CUTO = Data.VCF_CUTOFF
#if defined(PTK_SYNTH_LFO1)
                        + LFO1_VALUE * Data.LFO1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_LFO2)
                        + LFO2_VALUE * Data.LFO2_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV1)
                        + ENV1_VALUE * Data.ENV1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV2)
                        + ENV2_VALUE * Data.ENV2_VCF_CUTOFF
#endif
                       ;

            FILT_RESO = Data.VCF_RESONANCE
#if defined(PTK_SYNTH_LFO1)
                        + LFO1_VALUE * Data.LFO1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_LFO2)
                        + LFO2_VALUE * Data.LFO2_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV1)
                        + ENV1_VALUE * Data.ENV1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV2)
                        + ENV2_VALUE * Data.ENV2_VCF_RESONANCE
#endif
                    ;
            if(FILT_CUTO < 0.0f) FILT_CUTO = 0.0f;
            FILT_CUTO += 0.55f;
            if(FILT_CUTO > 1.55f) FILT_CUTO = 1.55f;
            if(FILT_RESO < 0.0f) FILT_RESO = 0.0f;
            if(FILT_RESO > 1.0f) FILT_RESO = 1.0f;
            GS_VAL = MoogFilterL();
    }
#endif      // defined(PTK_SYNTH_FILTER_MOOG)

#endif      // defined(PTK_SYNTH_FILTER)


// ------------------------------------------------
// Right signal

    if(Stereo == 2)
    {

#if defined(PTK_SYNTH_DISTO)
        if(Data.DISTO != 1.0f)
        {
            GS_VAL2 /= 32767.0f;
            GS_VAL2 = GS_VAL2 * (fabsf(GS_VAL2) + Data.DISTO) / ((GS_VAL2 * GS_VAL2) + (Data.DISTO - 1) *
                      fabsf(GS_VAL2) + 1);
            GS_VAL2 *= 32767.0f;
        }
#endif

#if defined(PTK_SYNTH_FILTER)
        if(Data.VCF_TYPE < 2) GS_VAL2 = FilterR();
#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
        else if(Data.VCF_TYPE > 2) GS_VAL2 = MoogFilterR();
#endif
#endif

        // Return right value
        *Right_Signal += GS_VAL2 * vol;
    }

    // Advance all, oscillator, envelopes, and lfo's
    EnvRun(track, track2);
    LfoAdvance();

    // Return value
    return GS_VAL * vol;
}

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
void CSynth::ChangeParameters(LPSYNTH_DATA TSP)
{
    memcpy(&Data, TSP, sizeof(SYNTH_DATA));
}

#else

void CSynth::ChangeParameters(SynthParameters TSP)
{
    Data.OSC1_WAVEFORM = TSP.osc1_waveform;
    Data.OSC2_WAVEFORM = TSP.osc2_waveform;
    Data.OSC_COMBINE = TSP.osc_combine;

    Data.OSC1_PW = (float) (TSP.osc1_pw - 256) / 256.0f;
    Data.OSC2_PW = (float) (TSP.osc2_pw - 256) / 256.0f;

    Data.OSC2_DETUNE = (float) (TSP.osc2_detune - 64.0f) * 0.0625f;
    Data.OSC2_FINETUNE = (float) TSP.osc2_finetune * 0.0078125f;

    Data.VCF_CUTOFF = (float) TSP.vcf_cutoff * 0.0078125f;
    Data.VCF_RESONANCE = (float) TSP.vcf_resonance * 0.0078125f;

    Data.VCF_TYPE = TSP.vcf_type;

    // Sustain = 0 128
    Data.ENV1_ATTACK = ((float) (TSP.env1_attack + 1)) / 512.0f;
    Data.ENV1_DECAY = ((float) (TSP.env1_decay + 1)) / 512.0f;
    Data.ENV1_SUSTAIN = (float) TSP.env1_sustain * 0.0078125f;          // / 128
    Data.ENV1_RELEASE = ((float) (TSP.env1_release + 1)) / 512.0f;
    if(Data.ENV1_ATTACK < 0.1f) Data.ENV1_ATTACK = 0.1f;
    if(Data.ENV1_RELEASE < 0.15f) Data.ENV1_RELEASE = 0.15f;

    Data.ENV2_ATTACK = ((float) (TSP.env2_attack + 1)) / 512.0f;
    Data.ENV2_DECAY = ((float) (TSP.env2_decay + 1)) / 512.0f;
    Data.ENV2_SUSTAIN = (float) (TSP.env2_sustain * 0.0078125f);        // / 128
    Data.ENV2_RELEASE = ((float) (TSP.env2_release + 1)) / 512.0f;
    if(Data.ENV2_ATTACK < 0.1f) Data.ENV2_ATTACK = 0.1f;
    if(Data.ENV2_RELEASE < 0.15f) Data.ENV2_RELEASE = 0.15f;
  
    Data.LFO1_PERIOD = (float) (TSP.lfo1_period * 2) + 1;
    Data.LFO1_SUBGRMAX = (int) (((float) SamplesPerTick * 0.000277f * Data.LFO1_PERIOD));

    Data.LFO2_PERIOD = (float) (TSP.lfo2_period * 2) + 1;
    Data.LFO2_SUBGRMAX = (int) ((float) SamplesPerTick * 0.000277f * Data.LFO2_PERIOD);

    /* Envelopes and LFO's matrix modulation variables */
    Data.LFO1_OSC1_PW =       ((float) TSP.lfo1_osc1_pw - 64) * 0.015625f;
    Data.LFO1_OSC2_PW =       ((float) TSP.lfo1_osc2_pw - 64) * 0.015625f;
    Data.LFO1_OSC1_PITCH =    ((float) TSP.lfo1_osc1_pitch - 64) * 0.015625f;
    Data.LFO1_OSC2_PITCH =    ((float) TSP.lfo1_osc2_pitch - 64) * 0.015625f;
    Data.LFO1_OSC1_VOLUME =   ((float) TSP.lfo1_osc1_volume - 64) * 0.015625f;
    Data.LFO1_OSC2_VOLUME =   ((float) TSP.lfo1_osc2_volume - 64) * 0.015625f;
    Data.LFO1_VCF_CUTOFF =    ((float) TSP.lfo1_vcf_cutoff - 64) * 0.015625f;
    Data.LFO1_VCF_RESONANCE = ((float) TSP.lfo1_vcf_resonance - 64) * 0.015625f;

    Data.LFO2_OSC1_PW =       ((float) TSP.lfo2_osc1_pw - 64) * 0.015625f;
    Data.LFO2_OSC2_PW =       ((float) TSP.lfo2_osc2_pw - 64) * 0.015625f;
    Data.LFO2_OSC1_PITCH =    ((float) TSP.lfo2_osc1_pitch - 64) * 0.015625f;
    Data.LFO2_OSC2_PITCH =    ((float) TSP.lfo2_osc2_pitch - 64) * 0.015625f;
    Data.LFO2_OSC1_VOLUME =   ((float) TSP.lfo2_osc1_volume - 64) * 0.015625f;
    Data.LFO2_OSC2_VOLUME =   ((float) TSP.lfo2_osc2_volume - 64) * 0.015625f;
    Data.LFO2_VCF_CUTOFF =    ((float) TSP.lfo2_vcf_cutoff - 64) * 0.015625f;
    Data.LFO2_VCF_RESONANCE = ((float) TSP.lfo2_vcf_resonance - 64) * 0.015625f;

    Data.ENV1_OSC1_PW =       ((float) TSP.env1_osc1_pw - 64) * 0.015625f;
    Data.ENV1_OSC2_PW =       ((float) TSP.env1_osc2_pw - 64) * 0.015625f;
    Data.ENV1_OSC1_PITCH =    ((float) TSP.env1_osc1_pitch - 64) * 0.015625f;
    Data.ENV1_OSC2_PITCH =    ((float) TSP.env1_osc2_pitch - 64) * 0.015625f;
    Data.ENV1_OSC1_VOLUME =   ((float) TSP.env1_osc1_volume - 64) * 0.015625f;
    Data.ENV1_OSC2_VOLUME =   ((float) TSP.env1_osc2_volume - 64) * 0.015625f;
    Data.ENV1_VCF_CUTOFF =    ((float) TSP.env1_vcf_cutoff - 64) * 0.015625f;
    Data.ENV1_VCF_RESONANCE = ((float) TSP.env1_vcf_resonance - 64) * 0.015625f;

    Data.ENV2_OSC1_PW =       ((float) TSP.env2_osc1_pw - 64) * 0.015625f;
    Data.ENV2_OSC2_PW =       ((float) TSP.env2_osc2_pw - 64) * 0.015625f;
    Data.ENV2_OSC1_PITCH =    ((float) TSP.env2_osc1_pitch - 64) * 0.015625f;
    Data.ENV2_OSC2_PITCH =    ((float) TSP.env2_osc2_pitch - 64) * 0.015625f;
    Data.ENV2_OSC1_VOLUME =   ((float) TSP.env2_osc1_volume - 64) * 0.015625f;
    Data.ENV2_OSC2_VOLUME =   ((float) TSP.env2_osc2_volume - 64) * 0.015625f;
    Data.ENV2_VCF_CUTOFF =    ((float) TSP.env2_vcf_cutoff - 64) * 0.015625f;
    Data.ENV2_VCF_RESONANCE = ((float) TSP.env2_vcf_resonance - 64) * 0.015625f;

    Data.OSC3_VOLUME =        ((float) TSP.osc3_volume - 64) * 0.015625f;
    Data.OSC3_SWITCH =        TSP.osc3_switch;

    Data.PTC_GLIDE =          ((float) TSP.ptc_glide * (float) TSP.ptc_glide) * 0.0000015625f;
    Data.PTC_GLIDE64 =         (int64) ((double) Data.PTC_GLIDE * 4294967296.0);

    Data.DISTO =              (((float) TSP.disto)) + 1.0f;

    Data.LFO1_ATTACK =        ((float) (TSP.lfo1_attack + 1)) / 512.0f;
    Data.LFO1_DECAY =         ((float) (TSP.lfo1_decay + 1)) / 512.0f;
    Data.LFO1_SUSTAIN =       (float) TSP.lfo1_sustain * 0.0078125f;
    Data.LFO1_RELEASE =       ((float) (TSP.lfo1_release + 1)) / 512.0f;
    if(Data.LFO1_ATTACK < 0.1f) Data.LFO1_ATTACK = 0.1f;
    if(Data.LFO1_RELEASE < 0.15f) Data.LFO1_RELEASE = 0.15f;

    Data.LFO2_ATTACK =        ((float) (TSP.lfo2_attack + 1)) / 512.0f;
    Data.LFO2_DECAY =         ((float) (TSP.lfo2_decay + 1)) / 512.0f;
    Data.LFO2_SUSTAIN =       (float) TSP.lfo2_sustain * 0.0078125f;
    Data.LFO2_RELEASE =       ((float) (TSP.lfo2_release + 1)) / 512.0f;
    if(Data.LFO2_ATTACK < 0.1f) Data.LFO2_ATTACK = 0.1f;
    if(Data.LFO2_RELEASE < 0.15f) Data.LFO2_RELEASE = 0.15f;
}
#endif // defined(__STAND_ALONE__) && !defined(__WINAMP__)

#if defined(PTK_SYNTH_FILTER)
#if defined(PTK_SYNTH_FILTER_LO) || defined(PTK_SYNTH_FILTER_HI)

float CSynth::FilterL(void)
{
//    GS_VAL++;
    sbuf0L = FILT_A * sbuf0L + FILT_CUTO * (GS_VAL + FILT_B * (sbuf0L - sbuf1L)); 
    sbuf1L = FILT_A * sbuf1L + FILT_CUTO * sbuf0L;
#if defined(PTK_SYNTH_FILTER_LO) && defined(PTK_SYNTH_FILTER_HI)
    return(Data.VCF_TYPE == 0 ? sbuf1L : GS_VAL - sbuf1L);
#else
    #if defined(PTK_SYNTH_FILTER_LO)
        return(sbuf1L);
    #endif
    #if defined(PTK_SYNTH_FILTER_HI)
        return(GS_VAL - sbuf1L);
    #endif
#endif
}

float CSynth::FilterR(void)
{
  //  GS_VAL2++;
    sbuf0R = FILT_A * sbuf0R + FILT_CUTO * (GS_VAL2 + FILT_B * (sbuf0R - sbuf1R));
    sbuf1R = FILT_A * sbuf1R + FILT_CUTO * sbuf0R;
#if defined(PTK_SYNTH_FILTER_LO) && defined(PTK_SYNTH_FILTER_HI)
    return(Data.VCF_TYPE == 0 ? sbuf1R : GS_VAL2 - sbuf1R);
#else
    #if defined(PTK_SYNTH_FILTER_LO)
        return(sbuf1R);
    #endif
    #if defined(PTK_SYNTH_FILTER_HI)
        return(GS_VAL2 - sbuf1R);
    #endif
#endif
}
#endif      // defined(PTK_SYNTH_FILTER_LO) || defined(PTK_SYNTH_FILTER_HI)

#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
float CSynth::MoogFilterL(void)
{
    float f;
    float p;
    float q;
    float t[4];
    float cut = FILT_CUTO;
    float res = FILT_RESO;

    cut *= 0.50f;
    q = (0.85f - cut);
    p = ((cut * cut) * 0.45f);
    f = (p + p) - 1.0f;
    res *= 5.0f;
    q = res * ((1.0f + (0.5f * q) * (1.0f - q + (5.6f * q * q))));
    if(q > 2.42f) q = 2.42f;
    float in = (GS_VAL / 32767.0f) - (q * MoogBufferL[4]);
    t[1] = MoogBufferL[1];
    t[2] = MoogBufferL[2];
    t[3] = MoogBufferL[3];
    MoogBufferL[1] = ((in + MoogBufferL[0]) * p ) - (MoogBufferL[1] * f);
    MoogBufferL[2] = ((MoogBufferL[1] + t[1]) * p ) - (MoogBufferL[2] * f);
    MoogBufferL[3] = ((MoogBufferL[2] + t[2]) * p ) - (MoogBufferL[3] * f);
    MoogBufferL[4] = ((MoogBufferL[3] + t[3]) * p ) - (MoogBufferL[4] * f);
    MoogBufferL[0] = in;

#if defined(PTK_SYNTH_FILTER_MOOG_LO) && defined(PTK_SYNTH_FILTER_MOOG_BAND)
    return((Data.VCF_TYPE == 3 ? MoogBufferL[4] : (3.0f * (MoogBufferL[3] - MoogBufferL[4]))) * 32767.0f);
#else
    #if defined(PTK_SYNTH_FILTER_MOOG_LO)
        return(MoogBufferL[4] * 32767.0f);
    #endif
    #if defined(PTK_SYNTH_FILTER_MOOG_BAND)
        return((3.0f * (MoogBufferL[3] - MoogBufferL[4])) * 32767.0f);
    #endif
#endif
}

float CSynth::MoogFilterR(void)
{
    float f;
    float p;
    float q;
    float t[4];
    float cut = FILT_CUTO;
    float res = FILT_RESO;

    cut *= 0.50f;
    q = (0.85f - cut);
    p = ((cut * cut) * 0.45f);
    f = (p + p) - 1.0f;
    res *= 5.0f;
    q = res * ((1.0f + (0.5f * q) * (1.0f - q + (5.6f * q * q))));
    if(q > 2.42f) q = 2.42f;
    float in = (GS_VAL / 32767.0f) - (q * MoogBufferR[4]);
    t[1] = MoogBufferR[1];
    t[2] = MoogBufferR[2];
    t[3] = MoogBufferR[3];
    MoogBufferR[1] = ((in + MoogBufferR[0]) * p ) - (MoogBufferR[1] * f);
    MoogBufferR[2] = ((MoogBufferR[1] + t[1]) * p ) - (MoogBufferR[2] * f);
    MoogBufferR[3] = ((MoogBufferR[2] + t[2]) * p ) - (MoogBufferR[3] * f);
    MoogBufferR[4] = ((MoogBufferR[3] + t[3]) * p ) - (MoogBufferR[4] * f);
    MoogBufferR[0] = in;

#if defined(PTK_SYNTH_FILTER_MOOG_LO) && defined(PTK_SYNTH_FILTER_MOOG_BAND)
    return((Data.VCF_TYPE == 3 ? MoogBufferR[4] : (3.0f * (MoogBufferR[3] - MoogBufferR[4]))) * 32767.0f);
#else
    #if defined(PTK_SYNTH_FILTER_MOOG_LO)
        return(MoogBufferR[4] * 32767.0f);
    #endif
    #if defined(PTK_SYNTH_FILTER_MOOG_BAND)
        return((3.0f * (MoogBufferR[3] - MoogBufferR[4])) * 32767.0f);
    #endif
#endif

}
#endif

#endif // defined(PTK_SYNTH_FILTER)

// Combine both oscillators with a given function
float CSynth::Math_Func(float in_old, float in_new)
{
    float sec_value;
    switch(Data.OSC_COMBINE)
    {
        default:
        case COMBINE_ADD:
            return in_old + in_new;
        case COMBINE_SUB:
            return in_old - in_new;
        case COMBINE_MUL:
            sec_value = (((in_new / 32767.0f) * 0.5f) + 0.5f);
            return (in_old * sec_value);
        case COMBINE_DIV:
            sec_value = (((in_new / 32767.0f) * 0.5f) + 0.5f) + 1.0f;
            return (in_old / sec_value);
    }
}

#endif // PTK_SYNTH
