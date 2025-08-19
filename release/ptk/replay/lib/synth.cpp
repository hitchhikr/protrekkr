// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2025 Franck Charlet.
// All rights reserFed.
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
float SIN[360];     // Sine float-precalculated table, in absolute degrees.

#if defined(PTK_SYNTH)

int SIZE_WAVEFORMS;

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

#if defined(PTK_SYNTH_TRI)
short STOCK_TRI[SIZE_WAVEFORMS_SPACE];
#endif

#define WAVEFORM_ADJUST_FREQ 0.5325f

// ------------------------------------------------------
// This next function resets the synthesizer to default values
void CSynth::Reset(void)
{

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    // Synthesizer General Reset
    Data.DISTO = 0.0f;
    Data.OSC_1_WAVEFORM = WAVEFORM_SAW;      /* Sawtooth */
    Data.OSC_2_WAVEFORM = WAVEFORM_PULSE;    /* Pulse */

    T_OSC_1_VOLUME = 0;
    T_OSC_2_VOLUME = 0;

    Data.OSC_2_DETUNE = 0;                  /* No Semitone Detune */
    Data.OSC_2_FINETUNE = 0.1f;             /* 1/10 Semitone detune */

    Data.VCF_CUTOFF = 0.5f;                 /* 10000Hz Cutoff */
    Data.VCF_RESONANCE = 0.5f;              /* Not very weird =] */
    Data.VCF_TYPE = 0;                      /* LowPass filter */

    Data.ENV_1_ATTACK = 0.0f;
    Data.ENV_1_DECAY = 0.1f;
    Data.ENV_1_SUSTAIN = 0.3f;              /* Sustain volume at 1/3 */
    Data.ENV_1_RELEASE = 0.0f;

    Data.ENV_2_ATTACK = 0.0f;
    Data.ENV_2_DECAY = 0.1f;
    Data.ENV_2_SUSTAIN = 0.3f;              /* Sustain volume at 1/3 */
    Data.ENV_2_RELEASE = 0.0f;

    Data.LFO_1_PERIOD = 16;
    Data.LFO_2_PERIOD = 16;

    LFO_1_GR = 0;
    LFO_2_GR = 0;

    LFO_1_SUBGRCOUNTER = 0;
    LFO_2_SUBGRCOUNTER = 0;

    Data.LFO_1_SUBGRMAX = 200;
    Data.LFO_2_SUBGRMAX = 200;

    Data.LFO_1_OSC_1_PW = 0;
    Data.LFO_1_OSC_2_PW = 0;
    Data.LFO_1_OSC_1_PITCH = 0;
    Data.LFO_1_OSC_2_PITCH = 0;
    Data.LFO_1_OSC_1_VOLUME = 0;
    Data.LFO_1_OSC_2_VOLUME = 0;   
    Data.LFO_1_VCF_CUTOFF = 0;
    Data.LFO_1_VCF_RESONANCE = 0; 
    Data.LFO_1_DISTO = 0; 

    Data.LFO_2_OSC_1_PW = 0;
    Data.LFO_2_OSC_2_PW = 0;
    Data.LFO_2_OSC_1_PITCH = 0;
    Data.LFO_2_OSC_2_PITCH = 0;
    Data.LFO_2_OSC_1_VOLUME = 0;
    Data.LFO_2_OSC_2_VOLUME = 0;   
    Data.LFO_2_VCF_CUTOFF = 0;
    Data.LFO_2_VCF_RESONANCE = 0; 
    Data.LFO_2_DISTO = 0; 

    Data.ENV_1_OSC_1_PW = 0;
    Data.ENV_1_OSC_2_PW = 0;
    Data.ENV_1_OSC_1_PITCH = 0;
    Data.ENV_1_OSC_2_PITCH = 0;
    Data.ENV_1_OSC_1_VOLUME = 1.0f;
    Data.ENV_1_OSC_2_VOLUME = 1.0f;   
    Data.ENV_1_VCF_CUTOFF = 0;
    Data.ENV_1_VCF_RESONANCE = 0; 
    Data.ENV_1_DISTO = 0; 

    Data.ENV_2_OSC_1_PW = 0;
    Data.ENV_2_OSC_2_PW = 0;
    Data.ENV_2_OSC_1_PITCH = 0;
    Data.ENV_2_OSC_2_PITCH = 0;
    Data.ENV_2_OSC_1_VOLUME = 0;
    Data.ENV_2_OSC_2_VOLUME = 0;   
    Data.ENV_2_VCF_CUTOFF = 0;
    Data.ENV_2_VCF_RESONANCE = 0; 
    Data.ENV_2_DISTO = 0; 

    Data.OSC_3_SWITCH = FALSE;

    OSC_1_STEP = 0;
    OSC_2_STEP = 0;

    ENV_1_VOLUME = 0;
    ENV_2_VOLUME = 0;

    ENV_1_MIN = 0;
    ENV_2_MIN = 0;

    Data.LFO_1_ATTACK = 0.0f;
    Data.LFO_1_DECAY = 0.1f;
    Data.LFO_1_SUSTAIN = 0.3f;
    Data.LFO_1_RELEASE = 0.0f;

    Data.LFO_2_ATTACK = 0.0f;
    Data.LFO_2_DECAY = 0.1f;
    Data.LFO_2_SUSTAIN = 0.3f;
    Data.LFO_2_RELEASE = 0.0f;

    Data.OSC_3_VOLUME = 0;
    Data.PTC_GLIDE_64 = 0;

    GLIDE_64_ALIGN = 0;

    Data.GLB_VOLUME = 1.0f;
    Data.OSC_COMBINE = COMBINE_ADD;

    Data.OSC_SYNC = FALSE;
    Data.OSC_3_INTERVAL = 12;

#endif

#if defined(PTK_SYNTH_LFO_1)
    LFO_1_COUNTER = 0;
    LFO_1_STAGE = 0;
    LFO_1_ADSR_VALUE = 0.0f;
    LFO_1_VALUE = 0;
#endif

#if defined(PTK_SYNTH_LFO_2)
    LFO_2_COUNTER = 0;
    LFO_2_STAGE = 0;
    LFO_2_ADSR_VALUE = 0.0f;
    LFO_2_VALUE = 0;
#endif

    ENV_1_STAGE = 0;
    ENV_2_STAGE = 0;

    ENV_1_COUNTER = 0;
    ENV_2_COUNTER = 0;

    ENV_1_VALUE = 0;
    ENV_2_VALUE = 0;

    ENV_1_A_COEF = 0.0f;
    ENV_2_A_COEF = 0.0f;

    OSC_1_SPEED = 0;
    OSC_2_SPEED = 0;
    OSC_3_SPEED = 0;

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

#define DENORMAL_SYNTH 0

#if DENORMAL_SYNTH
__inline float denormal_synth(float sample)
{
    unsigned int isample;
    *(unsigned int *) &isample = *(unsigned int *) &sample;

    int abs_mantissa = isample & 0x007FFFFF;
    int biased_exponent = isample & 0x7F800000;
    if(biased_exponent == 0 && abs_mantissa != 0) 
    {
        return 0;
    }
    return sample;
}
#endif

// ------------------------------------------------------
// This function is for internal use only. Makes the LFO's run.
void CSynth::Lfo_Advance(void)
{
#if defined(PTK_SYNTH_LFO_1)
    LFO_1_SUBGRCOUNTER++;
#endif

#if defined(PTK_SYNTH_LFO_2)
    LFO_2_SUBGRCOUNTER++;
#endif

#if defined(PTK_SYNTH_LFO_1)
    switch(LFO_1_STAGE)
    {
        case SYNTH_ATTACK:
            LFO_1_ADSR_VALUE += LFO_1_A_COEF;
            LFO_1_COUNTER++;
            if(LFO_1_COUNTER >= LFO_1b_ATTACK)
            {
                if(LFO_1_ADSR_VALUE > 1.0f) LFO_1_ADSR_VALUE = 1.0f;
                LFO_1_COUNTER = 0;
                LFO_1_STAGE = 2;
            }
            break;
        case SYNTH_DECAY:
            LFO_1_ADSR_VALUE -= LFO_1_D_COEF;
            LFO_1_COUNTER++;
            if(LFO_1_COUNTER >= LFO_1b_DECAY)
            {
                if(LFO_1_ADSR_VALUE < 0.0f) LFO_1_ADSR_VALUE = 0.0f;
                LFO_1_ADSR_VALUE = Data.LFO_1_SUSTAIN;
                LFO_1_COUNTER = 0;
                LFO_1_STAGE = 3;
            }
            break;
        case SYNTH_SUSTAIN:
            LFO_1_ADSR_VALUE = Data.LFO_1_SUSTAIN;
            break;
        case SYNTH_RELEASE:
            LFO_1_ADSR_VALUE -= LFO_1_R_COEF;
            LFO_1_COUNTER++;
            if(LFO_1_COUNTER > LFO_1b_RELEASE)
            {
                LFO_1_ADSR_VALUE = 0.0f;
                LFO_1_COUNTER = 0;
                LFO_1_STAGE = 0;
            }
            break;
    }

    if(LFO_1_SUBGRCOUNTER > Data.LFO_1_SUBGRMAX)
    {
        LFO_1_SUBGRCOUNTER = 0;
        LFO_1_GR++;
        if(LFO_1_GR > 359) LFO_1_GR = 0;
    }

    LFO_1_VALUE = SIN[LFO_1_GR] * LFO_1_ADSR_VALUE;
#endif

#if defined(PTK_SYNTH_LFO_2)
    switch(LFO_2_STAGE)
    {
        case SYNTH_ATTACK:
            LFO_2_ADSR_VALUE += LFO_2_A_COEF;
            LFO_2_COUNTER++;
            if(LFO_2_COUNTER >= LFO_2b_ATTACK)
            {
                if(LFO_2_ADSR_VALUE > 1.0f) LFO_2_ADSR_VALUE = 1.0f;
                LFO_2_COUNTER = 0;
                LFO_2_STAGE = 2;
            }
            break;
        case SYNTH_DECAY:
            LFO_2_ADSR_VALUE -= LFO_2_D_COEF;
            LFO_2_COUNTER++;
            if(LFO_2_COUNTER >= LFO_2b_DECAY)
            {
                if(LFO_2_ADSR_VALUE < 0.0f) LFO_2_ADSR_VALUE = 0.0f;
                LFO_2_ADSR_VALUE = Data.LFO_2_SUSTAIN;
                LFO_2_COUNTER = 0;
                LFO_2_STAGE = 3;
            }
            break;
        case SYNTH_SUSTAIN:
            LFO_2_ADSR_VALUE = Data.LFO_2_SUSTAIN;
            break;
        case SYNTH_RELEASE:
            LFO_2_ADSR_VALUE -= LFO_2_R_COEF;
            LFO_2_COUNTER++;
            if(LFO_2_COUNTER > LFO_2b_RELEASE)
            {
                LFO_2_ADSR_VALUE = 0.0f;
                LFO_2_COUNTER = 0;
                LFO_2_STAGE = 0;
            }
            break;
    }

    if(LFO_2_SUBGRCOUNTER > Data.LFO_2_SUBGRMAX)
    {
        LFO_2_SUBGRCOUNTER = 0;
        LFO_2_GR++;
        if(LFO_2_GR > 359) LFO_2_GR = 0;
    }

    LFO_2_VALUE = SIN[LFO_2_GR] * LFO_2_ADSR_VALUE;
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
    float note_1 = Data.OSC_1_WAVEFORM == WAVEFORM_WAV ? note_smp: (float) note;
    float note_2 = Data.OSC_2_WAVEFORM == WAVEFORM_WAV ? note_smp: (float) note;
#else
    float note_1 = (float) note;
    float note_2 = (float) note;
#endif

    float smp_freq;
    float adsr_ratio;

    OSC_1_STEP = POWF2((note_1) / 12.0f);
    OSC_2_STEP = POWF2(((note_2) + Data.OSC_2_FINETUNE + Data.OSC_2_DETUNE) / 12.0f);
    
    if(!glide)
    {
        ENV_1_STAGE = SYNTH_ATTACK; /* '0' is off, '1' starts the attack */
        ENV_2_STAGE = SYNTH_ATTACK;

        ENV_1_COUNTER = 0; /* Envelope stage counter, in samples */
        ENV_2_COUNTER = 0;

        ENV_1_VALUE = 0;
        ENV_2_VALUE = 0;

        ENV_1_MIN = 0;
        ENV_2_MIN = 0;

        ENV_1_VOLUME = 0;
        ENV_2_VOLUME = 0;

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

    if(Data.OSC_1_WAVEFORM != WAVEFORM_WAV)
    {
        Length = SIZE_WAVEFORMS;
        Loop_Length = SIZE_WAVEFORMS;
    }
    smp_freq = ((float) Length) / OSC_1_STEP;
    adsr_ratio = smp_freq / 1024.0f;
    ENV_1b_ATTACK = (Data.ENV_1_ATTACK / adsr_ratio) * smp_freq;
    ENV_1b_DECAY = (Data.ENV_1_DECAY / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO_1)
    LFO_1b_ATTACK = (Data.LFO_1_ATTACK / adsr_ratio) * smp_freq;
    LFO_1b_DECAY = (Data.LFO_1_DECAY / adsr_ratio) * smp_freq;
#endif

    if(Looping)
    {
        smp_freq = ((float) Loop_Length) / OSC_1_STEP;
        adsr_ratio = smp_freq / 1024.0f;
    }
    ENV_1b_RELEASE = (Data.ENV_1_RELEASE / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO_1)
    LFO_1b_RELEASE = (Data.LFO_1_RELEASE / adsr_ratio) * smp_freq;
#endif

    if(Data.OSC_2_WAVEFORM != WAVEFORM_WAV)
    {
        Length = SIZE_WAVEFORMS;
        Loop_Length = SIZE_WAVEFORMS;
    }
    smp_freq = ((float) Length) / OSC_2_STEP;
    adsr_ratio = smp_freq / 1024.0f;
    ENV_2b_ATTACK = (Data.ENV_2_ATTACK / adsr_ratio) * smp_freq;
    ENV_2b_DECAY = (Data.ENV_2_DECAY / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO_2)
    LFO_2b_ATTACK = (Data.LFO_2_ATTACK / adsr_ratio) * smp_freq;
    LFO_2b_DECAY = (Data.LFO_2_DECAY / adsr_ratio) * smp_freq;
#endif

    if(Looping)
    {
        smp_freq = ((float) Loop_Length) / OSC_2_STEP;
        adsr_ratio = smp_freq / 1024.0f;
    }
    ENV_2b_RELEASE = (Data.ENV_2_RELEASE / adsr_ratio) * smp_freq;

#if defined(PTK_SYNTH_LFO_2)
    LFO_2b_RELEASE = (Data.LFO_2_RELEASE / adsr_ratio) * smp_freq;
#endif

    if(ENV_1b_ATTACK < 1.0f) ENV_1b_ATTACK = 1.0f;
    if(ENV_1b_DECAY < 1.0f) ENV_1b_DECAY = 1.0f;
    if(ENV_1b_RELEASE < 1.0f) ENV_1b_RELEASE = 1.0f;
    if(ENV_2b_ATTACK < 1.0f) ENV_2b_ATTACK = 1.0f;
    if(ENV_2b_DECAY < 1.0f) ENV_2b_DECAY = 1.0f;
    if(ENV_2b_RELEASE < 1.0f) ENV_2b_RELEASE = 1.0f;

    /* Update ENV_1 */
    ENV_1_D_COEF = (1.0f - Data.ENV_1_SUSTAIN) / (float) ENV_1b_DECAY;
    ENV_1_R_COEF = Data.ENV_1_SUSTAIN / (float) ENV_1b_RELEASE;

    /* Update ENV_2 */
    ENV_2_D_COEF = (1.0f - Data.ENV_2_SUSTAIN) / (float) ENV_2b_DECAY;
    ENV_2_R_COEF = Data.ENV_2_SUSTAIN / (float) ENV_2b_RELEASE;

    ENV_1_A_COEF = (1.0f - ENV_1_VALUE) / ENV_1b_ATTACK;
    ENV_2_A_COEF = (1.0f - ENV_2_VALUE) / ENV_2b_ATTACK;

    if(!glide)
    {
        ENV_1_LOOP_BACKWARD = FALSE;
        ENV_2_LOOP_BACKWARD = FALSE;
        ENV_3_LOOP_BACKWARD = FALSE;

#if defined(PTK_SYNTH_LFO_1)
        LFO_1_STAGE = SYNTH_ATTACK;
        LFO_1_GR = 0;
        LFO_1_VALUE = 0;
        LFO_1_ADSR_VALUE = 0;
        LFO_1_COUNTER = 0;
        LFO_1_SUBGRCOUNTER = 0;
#endif

#if defined(PTK_SYNTH_LFO_2)
        LFO_2_STAGE = SYNTH_ATTACK;
        LFO_2_GR = 0;
        LFO_2_VALUE = 0;
        LFO_2_ADSR_VALUE = 0;
        LFO_2_COUNTER = 0;
        LFO_2_SUBGRCOUNTER = 0;
#endif
    }

#if defined(PTK_SYNTH_LFO_1)
    if(LFO_1b_ATTACK < 1.0f) LFO_1b_ATTACK = 1.0f;
    if(LFO_1b_DECAY < 1.0f) LFO_1b_DECAY = 1.0f;
    if(LFO_1b_RELEASE < 1.0f) LFO_1b_RELEASE = 1.0f;
#endif

#if defined(PTK_SYNTH_LFO_2)
    if(LFO_2b_ATTACK < 1.0f) LFO_2b_ATTACK = 1.0f;
    if(LFO_2b_DECAY < 1.0f) LFO_2b_DECAY = 1.0f;
    if(LFO_2b_RELEASE < 1.0f) LFO_2b_RELEASE = 1.0f;
#endif

#if defined(PTK_SYNTH_LFO_1)
    LFO_1_D_COEF = (1.0f - Data.LFO_1_SUSTAIN) / (float) LFO_1b_DECAY;
    LFO_1_R_COEF = Data.LFO_1_SUSTAIN / (float) LFO_1b_RELEASE;
    LFO_1_A_COEF = (1.0f - LFO_1_ADSR_VALUE) / LFO_1b_ATTACK;
#endif

#if defined(PTK_SYNTH_LFO_2)
    LFO_2_D_COEF = (1.0f - Data.LFO_2_SUSTAIN) / (float) LFO_2b_DECAY;
    LFO_2_R_COEF = Data.LFO_2_SUSTAIN / (float) LFO_2b_RELEASE;
    LFO_2_A_COEF = (1.0f - LFO_2_ADSR_VALUE) / LFO_2b_ATTACK;
#endif
}

// ------------------------------------------------------
// Envelopes run function
void CSynth::Env_Run(int *track, int *track2)
{
    /* ENV_1 */
    switch(ENV_1_STAGE)
    {
        /* Attack */
        case SYNTH_ATTACK:
            ENV_1_VALUE += ENV_1_A_COEF;
            ENV_1_COUNTER++;
            if(ENV_1_COUNTER >= ENV_1b_ATTACK)
            {
                if(ENV_1_VALUE > 1.0f) ENV_1_VALUE = 1.0f;
                ENV_1_COUNTER = 0;
                ENV_1_STAGE = SYNTH_DECAY;
            }
            break;

        /* Decay */
        case SYNTH_DECAY:
            ENV_1_VALUE -= ENV_1_D_COEF;
            ENV_1_COUNTER++;
            if(ENV_1_COUNTER >= ENV_1b_DECAY)
            {
                if(ENV_1_VALUE < 0.0f) ENV_1_VALUE = 0.0f;
                ENV_1_VALUE = Data.ENV_1_SUSTAIN;
                ENV_1_COUNTER = 0;
                ENV_1_STAGE = SYNTH_SUSTAIN;
            }
            break;

        /* Sustain */
        case SYNTH_SUSTAIN:
            ENV_1_VALUE = Data.ENV_1_SUSTAIN;
            break;

        /* Release */
        case SYNTH_RELEASE:
            ENV_1_VALUE -= ENV_1_R_COEF;
            ENV_1_COUNTER++;
            if(ENV_1_COUNTER > ENV_1b_RELEASE)
            {
                ENV_1_VALUE = 0.0f;
                ENV_1_COUNTER = 0;
                ENV_1_STAGE = 0; /* Stop the rock ENV_1 */
                OSC_1_SPEED = 0;
                OSC_3_SPEED = 0;
                *track = PLAYING_NOSAMPLE;
            }
            break;
    }

    /* ENV_2 */
    switch(ENV_2_STAGE)
    {
        /* Attack */
        case SYNTH_ATTACK:
            ENV_2_VALUE += ENV_2_A_COEF;
            ENV_2_COUNTER++;
            if(ENV_2_COUNTER >= ENV_2b_ATTACK)
            {
                if(ENV_2_VALUE > 1.0f) ENV_2_VALUE = 1.0f;
                ENV_2_COUNTER = 0;
                ENV_2_STAGE = SYNTH_DECAY;
            }
            break;

        /* Decay */
        case SYNTH_DECAY:
            ENV_2_VALUE -= ENV_2_D_COEF;
            ENV_2_COUNTER++;
            if(ENV_2_COUNTER >= ENV_2b_DECAY)
            {
                if(ENV_2_VALUE < 0.0f) ENV_2_VALUE = 0.0f;
                ENV_2_VALUE = Data.ENV_2_SUSTAIN;
                ENV_2_COUNTER = 0;
                ENV_2_STAGE = SYNTH_SUSTAIN;
            }
            break;

        /* Sustain */
        case SYNTH_SUSTAIN:
            ENV_2_VALUE = Data.ENV_2_SUSTAIN;
            break;

        /* Release */
        case SYNTH_RELEASE:
            ENV_2_VALUE -= ENV_2_R_COEF;
            ENV_2_COUNTER++;
            if(ENV_2_COUNTER > ENV_2b_RELEASE)
            {
                ENV_2_VALUE = 0.0f;
                ENV_2_COUNTER = 0;
                ENV_2_STAGE = 0; /* Stop the rock ENV_2 */
                OSC_2_SPEED = 0;
                *track2 = PLAYING_NOSAMPLE;
            }
            break;
    }

    ENV_1_VOLUME = ENV_1_VALUE;
#if defined(PTK_SYNTH_ENV_1)
    ENV_1_VOLUME *= Data.ENV_1_OSC_1_VOLUME;
#endif
#if defined(PTK_SYNTH_ENV_2)
    ENV_1_VOLUME *= Data.ENV_2_OSC_1_VOLUME;
#endif

    ENV_2_VOLUME = ENV_2_VALUE;
#if defined(PTK_SYNTH_ENV_1)
    ENV_2_VOLUME *= Data.ENV_1_OSC_2_VOLUME;
#endif
#if defined(PTK_SYNTH_ENV_2)
    ENV_2_VOLUME *= Data.ENV_2_OSC_2_VOLUME;
#endif

#if !defined(PTK_SYNTH_LFO_1) && !defined(PTK_SYNTH_LFO_2)
    ENV_1_MIN = 1.0f;
#else
    ENV_1_MIN = 0.0f;
    if(
#if defined(PTK_SYNTH_LFO_1)
    Data.LFO_1_OSC_1_VOLUME == 0.0f
#else
    TRUE
#endif
    &&
#if defined(PTK_SYNTH_LFO_2)
    Data.LFO_2_OSC_1_VOLUME == 0.0f
#else
    TRUE
#endif
    )
    {
        ENV_1_MIN = 1.0f;
    }
#endif

#if !defined(PTK_SYNTH_LFO_1) && !defined(PTK_SYNTH_LFO_2)
    ENV_2_MIN = 1.0f;
#else
    ENV_2_MIN = 0.0f;
    if(
#if defined(PTK_SYNTH_LFO_1)
    Data.LFO_1_OSC_2_VOLUME == 0.0f
#else
    TRUE
#endif
    &&
#if defined(PTK_SYNTH_LFO_2)
    Data.LFO_2_OSC_2_VOLUME == 0.0f
#else
    TRUE
#endif
    )
    {
        ENV_2_MIN = 1.0f;
    }
#endif
}

// ------------------------------------------------------
// 'Note Off' message for CSynth class objects
void CSynth::Note_Off(void)
{
    if(ENV_1_STAGE > PLAYING_NOSAMPLE && ENV_1_STAGE < SYNTH_RELEASE)
    {
        ENV_1_R_COEF = ENV_1_VALUE / (float) ENV_1b_RELEASE;
        ENV_1_COUNTER = 0;
        ENV_1_STAGE = SYNTH_RELEASE;
    }

    if(ENV_2_STAGE > PLAYING_NOSAMPLE && ENV_2_STAGE < SYNTH_RELEASE)
    {
        ENV_2_R_COEF = ENV_2_VALUE / (float) ENV_2b_RELEASE;
        ENV_2_COUNTER = 0;
        ENV_2_STAGE = SYNTH_RELEASE;
    }

#if defined(PTK_SYNTH_LFO_1)
    if(LFO_1_STAGE > PLAYING_NOSAMPLE && LFO_1_STAGE < SYNTH_RELEASE)
    {
        LFO_1_R_COEF = LFO_1_ADSR_VALUE / (float) LFO_1b_RELEASE;
        LFO_1_COUNTER = 0;
        LFO_1_STAGE = SYNTH_RELEASE;
    }
#endif

#if defined(PTK_SYNTH_LFO_2)
    if(LFO_2_STAGE > PLAYING_NOSAMPLE && LFO_2_STAGE < SYNTH_RELEASE)
    {
        LFO_2_R_COEF = LFO_2_ADSR_VALUE / (float) LFO_2b_RELEASE;
        LFO_2_COUNTER = 0;
        LFO_2_STAGE = SYNTH_RELEASE;
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
                        s_access *position_osc_1,
                        s_access *position_osc_2,

#if defined(PTK_SYNTH_OSC_3)
                        s_access *position_osc_3,
#endif

                        float osc_speed,
                        float Ampi_Vol)
    {

    s_access *pos_osc_1;
    pos_osc_1 = position_osc_1;
    
    s_access *pos_osc_2;
    pos_osc_2 = position_osc_2;

#if defined(PTK_SYNTH_OSC_3)
    s_access *pos_osc_3;
    pos_osc_3 = position_osc_3;
#endif

    short *Left_Samples1 = NULL;
    short *Right_Samples1 = NULL;
    unsigned int res_dec;

#if defined(PTK_SYNTH_PITCH)
    float osc_speed1;
    float osc_speed1b;
#endif

    float osc_speed2;
    float osc_speed_dbl;

#if defined(PTK_SYNTH_OSC_2)
    float osc_speed_tune;
#endif

    float mul_datL;
    float mul_datR;
    unsigned int i_POSITION;

#if defined(PTK_SYNTH_SYNC)
    int do_sync = FALSE;
#endif

    GS_VAL_L = 0;
    GS_VAL_R = 0;

    char Loop_Type1 = Loop_Type;
    char Loop_Type2 = Loop_Type;

    int Loop_Sub1 = Loop_Sub;
    int Loop_Sub2 = Loop_Sub;
    int Length1 = Length;
    int Length2 = Length;

// ------------------------------------------------
// Oscillator 1

    if(ENV_1_STAGE)
    {
        // Oscillator1 On
        if(Data.OSC_1_WAVEFORM != WAVEFORM_NONE)
        {
            T_OSC_1_VOLUME = (
#if defined(PTK_SYNTH_LFO_1)
                             LFO_1_VALUE * Data.LFO_1_OSC_1_VOLUME
#endif
#if defined(PTK_SYNTH_LFO_2)
                             + LFO_2_VALUE * Data.LFO_2_OSC_1_VOLUME
#endif
                             + ENV_1_MIN
                            )
                            * ENV_1_VOLUME
                           ;
            if(Data.OSC_1_WAVEFORM == WAVEFORM_NONE) T_OSC_1_VOLUME *= Ampi_Vol;

            if(*track)
            {
                if(GLIDE_64_ALIGN != 0 && OSC_1_SPEED != 0)
                {
                    if(osc_speed > OSC_1_SPEED)
                    {
                        OSC_1_SPEED += GLIDE_64_ALIGN;
                        if(OSC_1_SPEED > osc_speed) OSC_1_SPEED = osc_speed;
                    }
                    else
                    {
                        OSC_1_SPEED -= GLIDE_64_ALIGN;
                        if(OSC_1_SPEED < osc_speed) OSC_1_SPEED = osc_speed;
                    }
                }
                else
                {
                    OSC_1_SPEED = osc_speed;
                }

#if defined(PTK_SYNTH_PITCH)
                osc_speed1 = (float)
                             (
#if defined(PTK_SYNTH_LFO_1_PITCH)
                              LFO_1_VALUE * Data.LFO_1_OSC_1_PITCH
#endif
#if defined(PTK_SYNTH_LFO_2_PITCH)
                            + LFO_2_VALUE * Data.LFO_2_OSC_1_PITCH
#endif
#if defined(PTK_SYNTH_ENV_1_PITCH)
                            + ENV_1_VALUE * Data.ENV_1_OSC_1_PITCH
#endif
#if defined(PTK_SYNTH_ENV_2_PITCH)
                            + ENV_2_VALUE * Data.ENV_2_OSC_1_PITCH
#endif
                           );
#endif

                osc_speed2 = OSC_1_SPEED;
                Left_Samples1 = Left_Samples;
                Right_Samples1 = Right_Samples;

#if defined(PTK_INSTRUMENTS)
                if(Data.OSC_1_WAVEFORM != WAVEFORM_WAV)
#endif
                {
                    Length1 = SIZE_WAVEFORMS;
                    Loop_Sub1 = SIZE_WAVEFORMS;
                    
#if defined(PTK_SYNTH_SIN) || defined(PTK_SYNTH_SAW) || defined(PTK_SYNTH_PULSE) || defined(PTK_SYNTH_WHITE) || defined(PTK_SYNTH_TRI)
                    switch(Data.OSC_1_WAVEFORM)
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
                            Length1 = SIZE_WAVEFORMS_SPACE;
                            Loop_Sub1 = SIZE_WAVEFORMS_SPACE;
                            break;
#endif

#if defined(PTK_SYNTH_TRI)
                        case WAVEFORM_TRI:
                            Left_Samples1 = STOCK_TRI;
                            Right_Samples1 = STOCK_TRI;
                            break;
#endif

                    }

#endif

                    osc_speed_dbl = (float) osc_speed1;
                    osc_speed_dbl *= WAVEFORM_ADJUST_FREQ;
                    osc_speed1 = osc_speed_dbl;

                    osc_speed_dbl = (float) osc_speed2;
                    osc_speed_dbl *= WAVEFORM_ADJUST_FREQ;
                    osc_speed2 = osc_speed_dbl;

                    // Those always loop
                    Loop_Type1 = SMP_LOOP_FORWARD;
                }

                if(Left_Samples1)
                {
                    // ******
                    res_dec = pos_osc_1->int_pos;
                    
                    Set_Carrier_Boundaries(pos_osc_1->int_pos,
                                           &i_POSITION,
                                           Loop_Type1,
                                           ENV_1_LOOP_BACKWARD,
                                           Length1,
                                           Length1,
                                           Length1 - Loop_Sub1);

                    mul_datL = 1.0f;
                    mul_datR = 1.0f;

#if defined(PTK_SYNTH_PHASE1)
                    if(Data.OSC_1_PW)
                    {
                        T_OSC_PW = Data.OSC_1_PW * 
                                (
#if defined(PTK_SYNTH_LFO_1)
                                   (LFO_1_VALUE * Data.LFO_1_OSC_1_PW) 
#endif
#if defined(PTK_SYNTH_LFO_2)
                                 + (LFO_2_VALUE * Data.LFO_2_OSC_1_PW)
#endif
#if defined(PTK_SYNTH_ENV_1)
                                 + (ENV_1_VALUE * Data.ENV_1_OSC_1_PW)
#endif
#if defined(PTK_SYNTH_ENV_2)
                                 + (ENV_2_VALUE * Data.ENV_2_OSC_1_PW)
#endif
                                );
                        if(*(Left_Samples1 + i_POSITION) > 0) mul_datL = T_OSC_PW * 2.0f;
                        if(Stereo == 2) if(*(Right_Samples1 + i_POSITION) > 0) mul_datR = T_OSC_PW * 2.0f;
                    }
#endif

                    GS_VAL_L = (*(Left_Samples1 + i_POSITION) * mul_datL) * T_OSC_1_VOLUME;

                    // Stereo sample
                    if(Stereo == 2)
                    {
                        GS_VAL_R = (*(Right_Samples1 + i_POSITION) * mul_datR) * T_OSC_1_VOLUME;
                    }

#if defined(PTK_SYNTH_PITCH)
                    osc_speed2 += osc_speed1;
#endif

                    if(ENV_1_LOOP_BACKWARD == TRUE)
                    {
                        if(pos_osc_1->int_pos > 0)
                        {
                            pos_osc_1->flt_pos -= osc_speed2;
                            pos_osc_1->int_pos = (int32) pos_osc_1->flt_pos;
                        }
                    }
                    else
                    {
                        pos_osc_1->flt_pos += osc_speed2;
                        pos_osc_1->int_pos = (int32) pos_osc_1->flt_pos;
                    }

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    switch(Loop_Type1)
                    {
                        case SMP_LOOP_NONE:
#endif
                            if(ENV_1_LOOP_BACKWARD)
                            {
                                if((int) pos_osc_1->int_pos <= 0)
                                {
                                    pos_osc_1->int_pos = 0;
                                    pos_osc_1->flt_pos = 0;
                                    *track = PLAYING_NOSAMPLE;
                                }
                            }
                            else
                            {
                                if(pos_osc_1->int_pos >= Length1)
                                {
                                    pos_osc_1->int_pos = Length1;
                                    pos_osc_1->flt_pos = (float) Length1;
                                    *track = PLAYING_NOSAMPLE;
                                }
                            }

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                            break;
#endif

#if defined(PTK_LOOP_FORWARD)
                        case SMP_LOOP_FORWARD:
                            if(ENV_1_LOOP_BACKWARD)
                            {
                                if((int) pos_osc_1->int_pos <= (int) (Length1 - Loop_Sub1))
                                {
                                    pos_osc_1->int_pos += Length1;
                                    pos_osc_1->flt_pos += Length1;
                                }
                            }
                            else
                            {
                                if(pos_osc_1->int_pos >= Length1)
                                {
                                    pos_osc_1->int_pos -= Loop_Sub1;
                                    pos_osc_1->flt_pos -= Loop_Sub1;
                                }
                            }
                            break;
#endif

#if defined(PTK_LOOP_PINGPONG)
                        case SMP_LOOP_PINGPONG:
                            if(ENV_1_LOOP_BACKWARD)
                            {
                                if((int) pos_osc_1->int_pos <= (int) (Length1 - Loop_Sub1))
                                {
                                    pos_osc_1->int_pos = Length1 - Loop_Sub1;
                                    pos_osc_1->flt_pos = Length1 - (float) Loop_Sub1;
                                    ENV_1_LOOP_BACKWARD = FALSE;
                                }
                            }
                            else
                            {
                                if(pos_osc_1->int_pos >= Length1)
                                {
                                    pos_osc_1->int_pos = Length1;
                                    pos_osc_1->flt_pos = (float) Length1;
                                    ENV_1_LOOP_BACKWARD = TRUE;
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

#if defined(PTK_SYNTH_OSC_3)
    if(Data.OSC_3_SWITCH)
    {
        // SubOscillator On
        if(Data.OSC_1_WAVEFORM != WAVEFORM_NONE)
        {
            if(*track)
            {
                float semitones_intervals[] =
                {
                    2.0f,
                    1.89072f,
                    1.78615f, 
                    1.68631f, 
                    1.59118f, 
                    1.50076f, 
                    1.41508f, 
                    1.33410f, 
                    1.25784f, 
                    1.18630f, 
                    1.11949f, 
                    1.05738f, 
                    1.0f
                };
                osc_speed_dbl = (float) osc_speed2 / 2.0f;
                osc_speed_dbl *= semitones_intervals[Data.OSC_3_INTERVAL];
                osc_speed2 = osc_speed_dbl;

                if(Left_Samples1)
                {
                    // ******
                    res_dec = pos_osc_3->int_pos;

                    Set_Carrier_Boundaries(pos_osc_3->int_pos,
                                           &i_POSITION,
                                           Loop_Type1,
                                           ENV_3_LOOP_BACKWARD,
                                           Length1,
                                           Length1,
                                           Length1 - Loop_Sub1);

                    mul_datL = 1.0f;
                    mul_datR = 1.0f;

#if defined(PTK_SYNTH_PHASE1)
                    if(Data.OSC_1_PW)
                    {
                        T_OSC_PW = Data.OSC_1_PW * 
                               (
#if defined(PTK_SYNTH_LFO_1)
                                  (LFO_1_VALUE * Data.LFO_1_OSC_1_PW) 
#endif
#if defined(PTK_SYNTH_LFO_2)
                                + (LFO_2_VALUE * Data.LFO_2_OSC_1_PW)
#endif
#if defined(PTK_SYNTH_ENV_1)
                                + (ENV_1_VALUE * Data.ENV_1_OSC_1_PW)
#endif
#if defined(PTK_SYNTH_ENV_2)
                                + (ENV_2_VALUE * Data.ENV_2_OSC_1_PW)
#endif
                                );

                        if(*(Left_Samples1 + i_POSITION) > 0) mul_datL = T_OSC_PW * 2.0f;
                        if(Stereo == 2) if(*(Right_Samples1 + i_POSITION) > 0) mul_datR = T_OSC_PW * 2.0f;
                    }
#endif

                    GS_VAL_L = Math_Func(GS_VAL_L, (*(Left_Samples1 + i_POSITION) * mul_datL)
                                                    * T_OSC_1_VOLUME * Data.OSC_3_VOLUME);

                    if(Stereo == 2)
                    {
                        GS_VAL_R = Math_Func(GS_VAL_R, (*(Right_Samples1 + i_POSITION) * mul_datR)
                                                        * T_OSC_1_VOLUME * Data.OSC_3_VOLUME);
                    }
                }

                if(ENV_3_LOOP_BACKWARD == TRUE)
                {
                    if(pos_osc_3->int_pos > 0)
                    {
                        pos_osc_3->flt_pos -= osc_speed2;
                        pos_osc_3->int_pos = (int32) pos_osc_3->flt_pos;
                    }
                }
                else
                {
                    pos_osc_3->flt_pos += osc_speed2;
                    pos_osc_3->int_pos = (int32) pos_osc_3->flt_pos;
                }

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                switch(Loop_Type1)
                {
                    case SMP_LOOP_NONE:
#endif
                        if(ENV_3_LOOP_BACKWARD)
                        {
                            if((int) pos_osc_3->int_pos <= 0)
                            {
                                pos_osc_3->int_pos = 0;
                                pos_osc_3->flt_pos = 0;
                                *track = PLAYING_NOSAMPLE;
                            }
                        }
                        else
                        {
                            if(pos_osc_3->int_pos >= Length1)
                            {
                                pos_osc_3->int_pos = Length1;
                                pos_osc_3->flt_pos = (float) Length1;
                                *track = PLAYING_NOSAMPLE;
                            }
                        }
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                        break;
#endif

#if defined(PTK_LOOP_FORWARD)
                    case SMP_LOOP_FORWARD:
                        if(ENV_3_LOOP_BACKWARD)
                        {
                            if((int) pos_osc_3->int_pos <= (int) (Length1 - Loop_Sub1))
                            {
                                // ******
                                pos_osc_3->int_pos += Length1;
                                pos_osc_3->flt_pos += Length1;
                            }
                        }
                        else
                        {
                            if(pos_osc_3->int_pos >= Length1)
                            {
                                pos_osc_3->int_pos -= Loop_Sub1;
                                pos_osc_3->flt_pos -= Loop_Sub1;
                            }
                        }
                        break;
#endif

#if defined(PTK_LOOP_PINGPONG)
                    case SMP_LOOP_PINGPONG:
                        if(ENV_3_LOOP_BACKWARD)
                        {
                            if((int) pos_osc_3->int_pos <= (int) (Length1 - Loop_Sub1))
                            {
                                pos_osc_3->int_pos = Length1 - Loop_Sub1;
                                pos_osc_3->flt_pos = (float) (Length1 - Loop_Sub);
                                ENV_3_LOOP_BACKWARD = FALSE;
                            }
                        }
                        else
                        {
                            if(pos_osc_3->int_pos >= Length1)
                            {
                                pos_osc_3->int_pos = Length1;
                                pos_osc_3->flt_pos = (float) Length1;
                                ENV_3_LOOP_BACKWARD = TRUE;
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
#endif // PTK_SYNTH_OSC_3

// ------------------------------------------------
// Oscillator 2

#if defined(PTK_SYNTH_OSC_2)
    if(ENV_2_STAGE)
    {
        // Oscillator2 On 
        if(Data.OSC_2_WAVEFORM != WAVEFORM_NONE)
        {
            if(*track2)
            {
                osc_speed_tune = osc_speed + ((float) Data.OSC_2_FINETUNE * 0.125f)
                                           + ((float) Data.OSC_2_DETUNE  * 0.125f);
                if(GLIDE_64_ALIGN != 0 && OSC_2_SPEED != 0)
                {
                    if(osc_speed_tune > OSC_2_SPEED)
                    {
                        OSC_2_SPEED += GLIDE_64_ALIGN;
                        if(OSC_2_SPEED > osc_speed_tune) OSC_2_SPEED = osc_speed_tune;
                    }
                    else
                    {
                        OSC_2_SPEED -= GLIDE_64_ALIGN;
                        if(OSC_2_SPEED < osc_speed_tune) OSC_2_SPEED = osc_speed_tune;
                    }
                }
                else
                {
                    OSC_2_SPEED = osc_speed_tune;
                }

#if defined(PTK_SYNTH_PITCH)
                osc_speed1b = ((float)
                               (
#if defined(PTK_SYNTH_LFO_1_PITCH)
                                  LFO_1_VALUE * Data.LFO_1_OSC_2_PITCH
#endif
#if defined(PTK_SYNTH_LFO_2_PITCH)
                                + LFO_2_VALUE * Data.LFO_2_OSC_2_PITCH
#endif
#if defined(PTK_SYNTH_ENV_1_PITCH)
                                + ENV_1_VALUE * Data.ENV_1_OSC_2_PITCH
#endif
#if defined(PTK_SYNTH_ENV_2_PITCH)
                                + ENV_2_VALUE * Data.ENV_2_OSC_2_PITCH
#endif
                               ))
                              ;
#endif

                osc_speed2 = OSC_2_SPEED;

#if defined(PTK_INSTRUMENTS)
                if(Data.OSC_2_WAVEFORM != WAVEFORM_WAV)
#endif
                {
                    Length2 = SIZE_WAVEFORMS;
                    Loop_Sub2 = SIZE_WAVEFORMS;
                    
                    switch(Data.OSC_2_WAVEFORM)
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
                            Length2 = SIZE_WAVEFORMS_SPACE;
                            Loop_Sub2 = SIZE_WAVEFORMS_SPACE;
                            break;
#endif

#if defined(PTK_SYNTH_TRI)
                        case WAVEFORM_TRI:
                            Left_Samples = STOCK_TRI;
                            Right_Samples = STOCK_TRI;
                            break;
#endif

                    }

                    osc_speed_dbl = (float) osc_speed1b;
                    osc_speed_dbl *= WAVEFORM_ADJUST_FREQ;
                    osc_speed1b = osc_speed_dbl;

                    osc_speed_dbl = (float) osc_speed2;
                    osc_speed_dbl *= WAVEFORM_ADJUST_FREQ;
                    osc_speed2 = osc_speed_dbl;

                    // Those always loop
                    Loop_Type2 = SMP_LOOP_FORWARD;
                }

                if(Left_Samples)
                {
                    T_OSC_2_VOLUME = ((
#if defined(PTK_SYNTH_LFO_1)
                                    LFO_1_VALUE * Data.LFO_1_OSC_2_VOLUME
#endif
#if defined(PTK_SYNTH_LFO_2)
                                    + LFO_2_VALUE * Data.LFO_2_OSC_2_VOLUME
#endif
                                    + 0) + ENV_2_MIN)
                                    * ENV_2_VOLUME;

                    if(Data.OSC_2_WAVEFORM == WAVEFORM_NONE) T_OSC_2_VOLUME *= Ampi_Vol;

                    // ******
                    res_dec = pos_osc_2->int_pos;

                    Set_Carrier_Boundaries(pos_osc_2->int_pos,
                                           &i_POSITION,
                                           Loop_Type2,
                                           ENV_2_LOOP_BACKWARD,
                                           Length2,
                                           Length2,
                                           Length2 - Loop_Sub2);

                    mul_datL = 1.0f;
                    mul_datR = 1.0f;

#if defined(PTK_SYNTH_PHASE2)
                    if(Data.OSC_2_PW)
                    {
                        T_OSC_PW = Data.OSC_2_PW * 
                                  (
#if defined(PTK_SYNTH_LFO_1)
                                    (LFO_1_VALUE * Data.LFO_1_OSC_2_PW) 
#endif
#if defined(PTK_SYNTH_LFO_2)
                                    + (LFO_2_VALUE * Data.LFO_2_OSC_2_PW)
#endif
#if defined(PTK_SYNTH_ENV_1)
                                    + (ENV_1_VALUE * Data.ENV_1_OSC_2_PW)
#endif
#if defined(PTK_SYNTH_ENV_2)
                                    + (ENV_2_VALUE * Data.ENV_2_OSC_2_PW)
#endif
                                   );
                        if(*(Left_Samples + i_POSITION) > 0) mul_datL = T_OSC_PW * 2.0f;
                        if(Stereo == 2) if(*(Right_Samples + i_POSITION) > 0) mul_datR = T_OSC_PW * 2.0f;
                    }
#endif

                    GS_VAL_L = Math_Func(GS_VAL_L, (*(Left_Samples + i_POSITION) * mul_datL) * T_OSC_2_VOLUME);

                    // Stereo sample
                    if(Stereo == 2)
                    {

                        GS_VAL_R = Math_Func(GS_VAL_R, (*(Right_Samples + i_POSITION) * mul_datR) * T_OSC_2_VOLUME);
                    }

#if defined(PTK_SYNTH_PITCH)
                    osc_speed2 += osc_speed1b;
#endif

                    if(ENV_2_LOOP_BACKWARD == TRUE)
                    {
                        // ******
                        if(pos_osc_2->int_pos > 0)
                        {
                            pos_osc_2->flt_pos -= osc_speed2;
                            pos_osc_2->int_pos = (int32) pos_osc_2->flt_pos;
                        }
                    }
                    else
                    {
                        pos_osc_2->flt_pos += osc_speed2;
                        pos_osc_2->int_pos = (int32) pos_osc_2->flt_pos;
                    }

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    switch(Loop_Type2)
                    {
                        case SMP_LOOP_NONE:
#endif
                            if(ENV_2_LOOP_BACKWARD)
                            {
                                if((int) pos_osc_2->int_pos <= 0)
                                {
                                    pos_osc_2->int_pos = 0;
                                    pos_osc_2->flt_pos = 0;
                                    *track2 = PLAYING_NOSAMPLE;
#if defined(PTK_SYNTH_SYNC)
                                    do_sync = TRUE;
#endif
                                }
                            }
                            else
                            {
                                if(pos_osc_2->int_pos >= Length2)
                                {
                                    pos_osc_2->int_pos = Length2;
                                    pos_osc_2->flt_pos = (float) Length2;
                                    *track2 = PLAYING_NOSAMPLE;
#if defined(PTK_SYNTH_SYNC)
                                    do_sync = TRUE;
#endif
                                }
                            }

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                            break;
#endif

#if defined(PTK_LOOP_FORWARD)
                        case SMP_LOOP_FORWARD:
                            if(ENV_2_LOOP_BACKWARD)
                            {
                                // ******
                                if((int) pos_osc_2->int_pos <= (int) (Length2 - Loop_Sub2))
                                {
                                    pos_osc_2->int_pos += Length2;
                                    pos_osc_2->flt_pos += Length2;
#if defined(PTK_SYNTH_SYNC)
                                    do_sync = TRUE;
#endif
                                }
                            }
                            else
                            {
                                if(pos_osc_2->int_pos >= Length2)
                                {
                                    pos_osc_2->int_pos -= Loop_Sub2;
                                    pos_osc_2->flt_pos -= Loop_Sub2;
#if defined(PTK_SYNTH_SYNC)
                                    do_sync = TRUE;
#endif
                                }
                            }
                            break;
#endif

#if defined(PTK_LOOP_PINGPONG)
                        case SMP_LOOP_PINGPONG:
                            if(ENV_2_LOOP_BACKWARD)
                            {
                                if((int) pos_osc_2->int_pos <= (int) (Length2 - Loop_Sub2))
                                {
                                    pos_osc_2->int_pos = Length2 - Loop_Sub2;
                                    pos_osc_2->flt_pos = (float) (Length2 - Loop_Sub2);
                                    ENV_2_LOOP_BACKWARD = FALSE;
#if defined(PTK_SYNTH_SYNC)
                                    do_sync = TRUE;
#endif
                                }
                            }
                            else
                            {
                                if(pos_osc_2->int_pos >= Length2)
                                {
                                    pos_osc_2->int_pos = Length2;
                                    pos_osc_2->flt_pos = (float) Length2;
                                    ENV_2_LOOP_BACKWARD = TRUE;
#if defined(PTK_SYNTH_SYNC)
                                    do_sync = TRUE;
#endif
                                }
                            }
                            break;
#endif

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    }
#endif

#if defined(PTK_SYNTH_SYNC)
                    if(Data.OSC_SYNC)
                    {
                        if(do_sync)
                        {
                            pos_osc_1->int_pos = 0;
                            pos_osc_1->flt_pos = 0;
                            pos_osc_3->int_pos = 0;
                            pos_osc_3->flt_pos = 0;
                        }
                    }
#endif

                }
            }
        }
    }
#endif // PTK_SYNTH_OSC_2

// ------------------------------------------------
// Left signal

#if defined(PTK_SYNTH_DISTO)
    if(Data.DISTO != 0.0f)
    {
        float Disto;
 
        Disto = Data.DISTO
#if defined(PTK_SYNTH_LFO_1)
                    + (LFO_1_VALUE * Data.LFO_1_DISTO)
#endif
#if defined(PTK_SYNTH_LFO_2)
                    + (LFO_2_VALUE * Data.LFO_2_DISTO)
#endif
#if defined(PTK_SYNTH_ENV_1)
                    + (ENV_1_VALUE * Data.ENV_1_DISTO)
#endif
#if defined(PTK_SYNTH_ENV_2)
                    + (ENV_2_VALUE * Data.ENV_2_DISTO)
#endif
                   ;
        GS_VAL_L /= 32767.0f;
        GS_VAL_L = GS_VAL_L * (absf(GS_VAL_L) + Disto) / ((GS_VAL_L * GS_VAL_L) + (Disto - 1.0f) * absf(GS_VAL_L) + 1.0f);
        GS_VAL_L *= 32767.0f;
    }
#endif

#if defined(PTK_SYNTH_FILTER)
    if(Data.VCF_TYPE < 2)
    {
        FILT_CUTO = Data.VCF_CUTOFF
#if defined(PTK_SYNTH_LFO_1)
                    + LFO_1_VALUE * Data.LFO_1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_LFO_2)
                    + LFO_2_VALUE * Data.LFO_2_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV_1)
                    + ENV_1_VALUE * Data.ENV_1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV_2)
                    + ENV_2_VALUE * Data.ENV_2_VCF_CUTOFF
#endif
                   ;

        FILT_RESO = Data.VCF_RESONANCE
#if defined(PTK_SYNTH_LFO_1)
                    + LFO_1_VALUE * Data.LFO_1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_LFO_2)
                    + LFO_2_VALUE * Data.LFO_2_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV_1)
                    + ENV_1_VALUE * Data.ENV_1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV_2)
                    + ENV_2_VALUE * Data.ENV_2_VCF_RESONANCE
#endif
                   ;
        if(FILT_CUTO < 0.05f) FILT_CUTO = 0.05f;
        if(FILT_CUTO > 0.90f) FILT_CUTO = 0.90f;
        if(FILT_RESO < 0.05f) FILT_RESO = 0.05f;
        if(FILT_RESO > 0.98f) FILT_RESO = 0.98f;
        FILT_A = float(1.0f - FILT_CUTO); 
        FILT_B = float(FILT_RESO * (1.0f + (1.0f / FILT_A)));
        GS_VAL_L = Filter_L();
    }

#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
    else if(Data.VCF_TYPE > 2)
    {
            FILT_CUTO = Data.VCF_CUTOFF
#if defined(PTK_SYNTH_LFO_1)
                        + LFO_1_VALUE * Data.LFO_1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_LFO_2)
                        + LFO_2_VALUE * Data.LFO_2_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV_1)
                        + ENV_1_VALUE * Data.ENV_1_VCF_CUTOFF
#endif
#if defined(PTK_SYNTH_ENV_2)
                        + ENV_2_VALUE * Data.ENV_2_VCF_CUTOFF
#endif
                       ;

            FILT_RESO = Data.VCF_RESONANCE
#if defined(PTK_SYNTH_LFO_1)
                        + LFO_1_VALUE * Data.LFO_1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_LFO_2)
                        + LFO_2_VALUE * Data.LFO_2_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV_1)
                        + ENV_1_VALUE * Data.ENV_1_VCF_RESONANCE
#endif
#if defined(PTK_SYNTH_ENV_2)
                        + ENV_2_VALUE * Data.ENV_2_VCF_RESONANCE
#endif
                    ;
            if(FILT_CUTO < 0.0f) FILT_CUTO = 0.0f;
            FILT_CUTO += 0.55f;
            if(FILT_CUTO > 1.55f) FILT_CUTO = 1.55f;
            if(FILT_RESO < 0.0f) FILT_RESO = 0.0f;
            if(FILT_RESO > 1.0f) FILT_RESO = 1.0f;
            GS_VAL_L = Moog_Filter_L();
    }
#endif      // defined(PTK_SYNTH_FILTER_MOOG)

#endif      // defined(PTK_SYNTH_FILTER)

// ------------------------------------------------
// Right signal

    if(Stereo == 2)
    {

#if defined(PTK_SYNTH_DISTO)
        if(Data.DISTO != 0.0f)
        {
            float Disto;

            Disto = Data.DISTO
#if defined(PTK_SYNTH_LFO_1)
                        + (LFO_1_VALUE * Data.LFO_1_DISTO)
#endif
#if defined(PTK_SYNTH_LFO_2)
                        + (LFO_2_VALUE * Data.LFO_2_DISTO)
#endif
#if defined(PTK_SYNTH_ENV_1)
                        + (ENV_1_VALUE * Data.ENV_1_DISTO)
#endif
#if defined(PTK_SYNTH_ENV_2)
                        + (ENV_2_VALUE * Data.ENV_2_DISTO)
#endif
                       ;
            GS_VAL_R /= 32767.0f;
            GS_VAL_R = GS_VAL_R * (absf(GS_VAL_R) + Disto) / ((GS_VAL_R * GS_VAL_R) + (Disto - 1.0f) * absf(GS_VAL_R) + 1.0f);
            GS_VAL_R *= 32767.0f;
        }
#endif

#if defined(PTK_SYNTH_FILTER)
        if(Data.VCF_TYPE < 2)
        {
            GS_VAL_R = Filter_R();
        }
#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
        else if(Data.VCF_TYPE > 2)
        {
            GS_VAL_R = Moog_Filter_R();
        }
#endif
#endif

        // Return right value
        *Right_Signal += GS_VAL_R * vol;
    }

    // Advance all, oscillator, envelopes, and lfo's
    Env_Run(track, track2);
    Lfo_Advance();

    // Return value
    return GS_VAL_L * vol;
}

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
void CSynth::ChangeParameters(LPSYNTH_DATA TSP)
{
    memcpy(&Data, TSP, sizeof(SYNTH_DATA));
}

#else

void CSynth::ChangeParameters(Synth_Parameters TSP)
{
    Data.OSC_1_WAVEFORM =       TSP.osc_1_waveform;
    Data.OSC_2_WAVEFORM =       TSP.osc_2_waveform;
    Data.OSC_COMBINE =          TSP.osc_combine;
    Data.OSC_SYNC =             TSP.osc_sync;
    Data.OSC_3_INTERVAL =       TSP.osc_3_interval;

    Data.OSC_1_PW =             (float) (TSP.osc_1_pw - 256) / 256.0f;
    Data.OSC_2_PW =             (float) (TSP.osc_2_pw - 256) / 256.0f;

    Data.OSC_2_DETUNE =         (float) (TSP.osc_2_detune - 64.0f) * 0.0625f;
    Data.OSC_2_FINETUNE =       (float) TSP.osc_2_finetune * 0.0078125f;

    Data.VCF_CUTOFF =           (float) TSP.vcf_cutoff * 0.0078125f;
    Data.VCF_RESONANCE =        (float) TSP.vcf_resonance * 0.0078125f;

    Data.VCF_TYPE = TSP.vcf_type;

    // Sustain = 0 128
    Data.ENV_1_ATTACK =         ((float) (TSP.env_1_attack + 1)) / 512.0f;
    Data.ENV_1_DECAY =          ((float) (TSP.env_1_decay + 1)) / 512.0f;
    Data.ENV_1_SUSTAIN =        (float) TSP.env_1_sustain * 0.0078125f;          // / 128
    Data.ENV_1_RELEASE =        ((float) (TSP.env_1_release + 1)) / 512.0f;
    if(Data.ENV_1_ATTACK < 0.1f) Data.ENV_1_ATTACK = 0.1f;
    if(Data.ENV_1_RELEASE < 0.15f) Data.ENV_1_RELEASE = 0.15f;

    Data.ENV_2_ATTACK =         ((float) (TSP.env_2_attack + 1)) / 512.0f;
    Data.ENV_2_DECAY =          ((float) (TSP.env_2_decay + 1)) / 512.0f;
    Data.ENV_2_SUSTAIN =        (float) (TSP.env_2_sustain * 0.0078125f);        // / 128
    Data.ENV_2_RELEASE =        ((float) (TSP.env_2_release + 1)) / 512.0f;
    if(Data.ENV_2_ATTACK < 0.1f) Data.ENV_2_ATTACK = 0.1f;
    if(Data.ENV_2_RELEASE < 0.15f) Data.ENV_2_RELEASE = 0.15f;
  
    Data.LFO_1_PERIOD =         (float) (TSP.lfo_1_period * 2) + 1;
    Data.LFO_1_SUBGRMAX =       (int) (((float) SamplesPerTick * 0.000277f * Data.LFO_1_PERIOD));

    Data.LFO_2_PERIOD =         (float) (TSP.lfo_2_period * 2) + 1;
    Data.LFO_2_SUBGRMAX =       (int) ((float) SamplesPerTick * 0.000277f * Data.LFO_2_PERIOD);

    /* Envelopes and LFO's matrix modulation variables */
    Data.LFO_1_OSC_1_PW =       ((float) TSP.lfo_1_osc_1_pw - 64) * 0.015625f;
    Data.LFO_1_OSC_2_PW =       ((float) TSP.lfo_1_osc_2_pw - 64) * 0.015625f;
    Data.LFO_1_OSC_1_PITCH =    ((float) TSP.lfo_1_osc_1_pitch - 64) * 0.015625f;
    Data.LFO_1_OSC_2_PITCH =    ((float) TSP.lfo_1_osc_2_pitch - 64) * 0.015625f;
    Data.LFO_1_OSC_1_VOLUME =   ((float) TSP.lfo_1_osc_1_volume - 64) * 0.015625f;
    Data.LFO_1_OSC_2_VOLUME =   ((float) TSP.lfo_1_osc_2_volume - 64) * 0.015625f;
    Data.LFO_1_VCF_CUTOFF =     ((float) TSP.lfo_1_vcf_cutoff - 64) * 0.015625f;
    Data.LFO_1_VCF_RESONANCE =  ((float) TSP.lfo_1_vcf_resonance - 64) * 0.015625f;
    Data.LFO_1_DISTO =          ((float) TSP.lfo_1_disto - 64) * 0.015625f;

    Data.LFO_2_OSC_1_PW =       ((float) TSP.lfo_2_osc_1_pw - 64) * 0.015625f;
    Data.LFO_2_OSC_2_PW =       ((float) TSP.lfo_2_osc_2_pw - 64) * 0.015625f;
    Data.LFO_2_OSC_1_PITCH =    ((float) TSP.lfo_2_osc_1_pitch - 64) * 0.015625f;
    Data.LFO_2_OSC_2_PITCH =    ((float) TSP.lfo_2_osc_2_pitch - 64) * 0.015625f;
    Data.LFO_2_OSC_1_VOLUME =   ((float) TSP.lfo_2_osc_1_volume - 64) * 0.015625f;
    Data.LFO_2_OSC_2_VOLUME =   ((float) TSP.lfo_2_osc_2_volume - 64) * 0.015625f;
    Data.LFO_2_VCF_CUTOFF =     ((float) TSP.lfo_2_vcf_cutoff - 64) * 0.015625f;
    Data.LFO_2_VCF_RESONANCE =  ((float) TSP.lfo_2_vcf_resonance - 64) * 0.015625f;
    Data.LFO_2_DISTO =          ((float) TSP.lfo_2_disto - 64) * 0.015625f;

    Data.ENV_1_OSC_1_PW =       ((float) TSP.env_1_osc_1_pw - 64) * 0.015625f;
    Data.ENV_1_OSC_2_PW =       ((float) TSP.env_1_osc_2_pw - 64) * 0.015625f;
    Data.ENV_1_OSC_1_PITCH =    ((float) TSP.env_1_osc_1_pitch - 64) * 0.015625f;
    Data.ENV_1_OSC_2_PITCH =    ((float) TSP.env_1_osc_2_pitch - 64) * 0.015625f;
    Data.ENV_1_OSC_1_VOLUME =   ((float) TSP.env_1_osc_1_volume - 64) * 0.015625f;
    Data.ENV_1_OSC_2_VOLUME =   ((float) TSP.env_1_osc_2_volume - 64) * 0.015625f;
    Data.ENV_1_VCF_CUTOFF =     ((float) TSP.env_1_vcf_cutoff - 64) * 0.015625f;
    Data.ENV_1_VCF_RESONANCE =  ((float) TSP.env_1_vcf_resonance - 64) * 0.015625f;
    Data.ENV_1_DISTO =          ((float) TSP.env_1_disto - 64) * 0.015625f;

    Data.ENV_2_OSC_1_PW =       ((float) TSP.env_2_osc_1_pw - 64) * 0.015625f;
    Data.ENV_2_OSC_2_PW =       ((float) TSP.env_2_osc_2_pw - 64) * 0.015625f;
    Data.ENV_2_OSC_1_PITCH =    ((float) TSP.env_2_osc_1_pitch - 64) * 0.015625f;
    Data.ENV_2_OSC_2_PITCH =    ((float) TSP.env_2_osc_2_pitch - 64) * 0.015625f;
    Data.ENV_2_OSC_1_VOLUME =   ((float) TSP.env_2_osc_1_volume - 64) * 0.015625f;
    Data.ENV_2_OSC_2_VOLUME =   ((float) TSP.env_2_osc_2_volume - 64) * 0.015625f;
    Data.ENV_2_VCF_CUTOFF =     ((float) TSP.env_2_vcf_cutoff - 64) * 0.015625f;
    Data.ENV_2_VCF_RESONANCE =  ((float) TSP.env_2_vcf_resonance - 64) * 0.015625f;
    Data.ENV_2_DISTO =          ((float) TSP.env_2_disto - 64) * 0.015625f;

    Data.OSC_3_VOLUME =         ((float) TSP.osc_3_volume - 64) * 0.015625f;
    Data.OSC_3_SWITCH =         TSP.osc_3_switch;

    Data.PTC_GLIDE =            ((float) TSP.ptc_glide * (float) TSP.ptc_glide) * 0.0000015625f;
    GLIDE_64_ALIGN =            ((float) Data.PTC_GLIDE);

    Data.DISTO =                ((float) TSP.disto - 64) * 0.015625f;

    Data.LFO_1_ATTACK =         ((float) (TSP.lfo_1_attack + 1)) / 512.0f;
    Data.LFO_1_DECAY =          ((float) (TSP.lfo_1_decay + 1)) / 512.0f;
    Data.LFO_1_SUSTAIN =        (float) TSP.lfo_1_sustain * 0.0078125f;
    Data.LFO_1_RELEASE =        ((float) (TSP.lfo_1_release + 1)) / 512.0f;
    if(Data.LFO_1_ATTACK < 0.1f) Data.LFO_1_ATTACK = 0.1f;
    if(Data.LFO_1_RELEASE < 0.15f) Data.LFO_1_RELEASE = 0.15f;

    Data.LFO_2_ATTACK =        ((float) (TSP.lfo_2_attack + 1)) / 512.0f;
    Data.LFO_2_DECAY =         ((float) (TSP.lfo_2_decay + 1)) / 512.0f;
    Data.LFO_2_SUSTAIN =       (float) TSP.lfo_2_sustain * 0.0078125f;
    Data.LFO_2_RELEASE =       ((float) (TSP.lfo_2_release + 1)) / 512.0f;
    if(Data.LFO_2_ATTACK < 0.1f) Data.LFO_2_ATTACK = 0.1f;
    if(Data.LFO_2_RELEASE < 0.15f) Data.LFO_2_RELEASE = 0.15f;
}
#endif // defined(__STAND_ALONE__) && !defined(__WINAMP__)

#if defined(PTK_SYNTH_FILTER)
#if defined(PTK_SYNTH_FILTER_LO) || defined(PTK_SYNTH_FILTER_HI)

float CSynth::Filter_L(void)
{
    sbuf0L = FILT_A * sbuf0L + FILT_CUTO * (GS_VAL_L + FILT_B * (sbuf0L - sbuf1L)); 
    sbuf1L = FILT_A * sbuf1L + FILT_CUTO * sbuf0L;

#if DENORMAL_SYNTH
    sbuf0L = denormal_synth(sbuf0L);
    sbuf1L = denormal_synth(sbuf1L);
#endif

#if defined(PTK_SYNTH_FILTER_LO) && defined(PTK_SYNTH_FILTER_HI)
    return(Data.VCF_TYPE == 0 ? sbuf1L : GS_VAL_L - sbuf1L);
#else
    #if defined(PTK_SYNTH_FILTER_LO)
        return(sbuf1L);
    #endif
    #if defined(PTK_SYNTH_FILTER_HI)
        return(GS_VAL_L - sbuf1L);
    #endif
#endif
}

float CSynth::Filter_R(void)
{
    sbuf0R = FILT_A * sbuf0R + FILT_CUTO * (GS_VAL_R + FILT_B * (sbuf0R - sbuf1R));
    sbuf1R = FILT_A * sbuf1R + FILT_CUTO * sbuf0R;

#if DENORMAL_SYNTH
    sbuf0R = denormal_synth(sbuf0R);
    sbuf1R = denormal_synth(sbuf1R);
#endif

#if defined(PTK_SYNTH_FILTER_LO) && defined(PTK_SYNTH_FILTER_HI)
    return(Data.VCF_TYPE == 0 ? sbuf1R : GS_VAL_R - sbuf1R);
#else
    #if defined(PTK_SYNTH_FILTER_LO)
        return(sbuf1R);
    #endif
    #if defined(PTK_SYNTH_FILTER_HI)
        return(GS_VAL_R - sbuf1R);
    #endif
#endif
}
#endif      // defined(PTK_SYNTH_FILTER_LO) || defined(PTK_SYNTH_FILTER_HI)

#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
float CSynth::Moog_Filter_L(void)
{
    float f;
    float p;
    float q;
    float in;
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

#if DENORMAL_SYNTH
    q = denormal_synth(q);
    MoogBufferL[1] = denormal_synth(MoogBufferL[1]);
    MoogBufferL[2] = denormal_synth(MoogBufferL[2]);
    MoogBufferL[3] = denormal_synth(MoogBufferL[3]);
    MoogBufferL[4] = denormal_synth(MoogBufferL[4]);
#endif

    in = (GS_VAL_L / 32767.0f) - (q * MoogBufferL[4]);

    t[1] = MoogBufferL[1];
    t[2] = MoogBufferL[2];
    t[3] = MoogBufferL[3];
    MoogBufferL[1] = ((in + MoogBufferL[0]) * p) - (MoogBufferL[1] * f);
    MoogBufferL[2] = ((MoogBufferL[1] + t[1]) * p) - (MoogBufferL[2] * f);
    MoogBufferL[3] = ((MoogBufferL[2] + t[2]) * p) - (MoogBufferL[3] * f);
    MoogBufferL[4] = ((MoogBufferL[3] + t[3]) * p) - (MoogBufferL[4] * f);
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

float CSynth::Moog_Filter_R(void)
{
    float f;
    float p;
    float q;
    float t[4];
    float in;
    float cut = FILT_CUTO;
    float res = FILT_RESO;

    cut *= 0.50f;
    q = (0.85f - cut);
    p = ((cut * cut) * 0.45f);
    f = (p + p) - 1.0f;
    res *= 5.0f;
    q = res * ((1.0f + (0.5f * q) * (1.0f - q + (5.6f * q * q))));
    if(q > 2.42f) q = 2.42f;

#if DENORMAL_SYNTH
    q = denormal_synth(q);
    MoogBufferR[1] = denormal_synth(MoogBufferR[1]);
    MoogBufferR[2] = denormal_synth(MoogBufferR[2]);
    MoogBufferR[3] = denormal_synth(MoogBufferR[3]);
    MoogBufferR[4] = denormal_synth(MoogBufferR[4]);
#endif

    in = (GS_VAL_L / 32767.0f) - (q * MoogBufferR[4]);

    t[1] = MoogBufferR[1];
    t[2] = MoogBufferR[2];
    t[3] = MoogBufferR[3];
    MoogBufferR[1] = ((in + MoogBufferR[0]) * p) - (MoogBufferR[1] * f);
    MoogBufferR[2] = ((MoogBufferR[1] + t[1]) * p) - (MoogBufferR[2] * f);
    MoogBufferR[3] = ((MoogBufferR[2] + t[2]) * p) - (MoogBufferR[3] * f);
    MoogBufferR[4] = ((MoogBufferR[3] + t[3]) * p) - (MoogBufferR[4] * f);
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
