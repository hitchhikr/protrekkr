// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2021 Franck Charlet.
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

#ifndef _SYNTH_H_
#define _SYNTH_H_

// ------------------------------------------------------
// Includes
#include <math.h>
#include "replay.h"

// ------------------------------------------------------
// Constants
#define SIZE_WAVEFORMS_SPACE 88273

#define WAVEFORM_SIN 0
#define WAVEFORM_SAW 1
#define WAVEFORM_PULSE 2
#define WAVEFORM_WHITE 3
#define WAVEFORM_NONE 4
#define WAVEFORM_WAV 5
#define WAVEFORM_PINK 6

#define COMBINE_ADD 0
#define COMBINE_SUB 1
#define COMBINE_MUL 2
#define COMBINE_DIV 3

// ------------------------------------------------------
// Types
#if defined(__WIN32__) && !defined(__GCC__)
typedef __int64 int64;
typedef unsigned __int64 Uint64;
#else
typedef long long int64;
#if defined(__LINUX__) && !defined(__FREEBSD__)
#include <stdint.h>
//typedef unsigned long long uint64_t;
typedef uint64_t Uint64;
#else
typedef unsigned long long Uint64;
#endif
#endif

extern float SIN[360];

#if defined(PTK_SYNTH)

// ------------------------------------------------------
// Structures

/* Struct used to store/update synthesizer parameters */

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)

struct SynthParameters
{
    char presetname[20];

    unsigned char osc1_waveform;
    unsigned char osc2_waveform;
    unsigned char osc_combine;

    int osc1_pw;
    int osc2_pw;

    unsigned char osc2_detune;
    unsigned char osc2_finetune;

    unsigned char vcf_cutoff;
    unsigned char vcf_resonance;
    unsigned char vcf_type;

    /* Envelopes and LFO's properties */

    int env1_attack;
    int env1_decay;
    unsigned char env1_sustain;
    int env1_release;

    int env2_attack;
    int env2_decay;
    unsigned char env2_sustain;
    int env2_release;

    int lfo1_period;
    int lfo2_period;

    /* Envelopes and LFO's modulation variables */

    unsigned char lfo1_osc1_pw;
    unsigned char lfo1_osc2_pw;
    unsigned char lfo1_osc1_pitch;
    unsigned char lfo1_osc2_pitch;
    unsigned char lfo1_osc1_volume;
    unsigned char lfo1_osc2_volume;
    unsigned char lfo1_vcf_cutoff;
    unsigned char lfo1_vcf_resonance;

    unsigned char lfo2_osc1_pw;
    unsigned char lfo2_osc2_pw;
    unsigned char lfo2_osc1_pitch;
    unsigned char lfo2_osc2_pitch;
    unsigned char lfo2_osc1_volume;
    unsigned char lfo2_osc2_volume;
    unsigned char lfo2_vcf_cutoff;
    unsigned char lfo2_vcf_resonance;

    unsigned char env1_osc1_pw;
    unsigned char env1_osc2_pw;
    unsigned char env1_osc1_pitch;
    unsigned char env1_osc2_pitch;
    unsigned char env1_osc1_volume;
    unsigned char env1_osc2_volume;  
    unsigned char env1_vcf_cutoff;
    unsigned char env1_vcf_resonance;

    unsigned char env2_osc1_pw;
    unsigned char env2_osc2_pw;
    unsigned char env2_osc1_pitch;
    unsigned char env2_osc2_pitch;
    unsigned char env2_osc1_volume;
    unsigned char env2_osc2_volume;  
    unsigned char env2_vcf_cutoff;
    unsigned char env2_vcf_resonance;

    unsigned char osc3_volume;
    unsigned char osc3_switch;

    unsigned char ptc_glide;
    unsigned char glb_volume;

    unsigned char disto;

    int lfo1_attack;
    int lfo1_decay;
    unsigned char lfo1_sustain;
    int lfo1_release;

    int lfo2_attack;
    int lfo2_decay;
    unsigned char lfo2_sustain;
    int lfo2_release;
};

#endif      // !defined(__STAND_ALONE__) || defined(__WINAMP__)

// ------------------------------------------------------
// Variables
extern int SamplesPerTick;

/* Sine float-precalculated table, in absolute degrees. */
#if defined(PTK_SYNTH_SIN)
extern short STOCK_SIN[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_SAW)
extern short STOCK_SAW[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_PULSE)
extern short STOCK_PULSE[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_WHITE)
extern short STOCK_WHITE[SIZE_WAVEFORMS_SPACE];
#endif

#if defined(PTK_SYNTH_PINK)
    extern short STOCK_PINK[SIZE_WAVEFORMS_SPACE];
#endif

extern int SIZE_WAVEFORMS;

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
    #if !defined(__GCC__)
    #pragma pack(push)
    #pragma pack(1)
    #else
    #pragma pack(push, 1)
    #endif
#endif

typedef struct
{
        char OSC1_WAVEFORM;
        char OSC2_WAVEFORM;

        float OSC1_PW;
        float OSC2_PW; 
        float OSC2_DETUNE;
        float OSC2_FINETUNE; 
        float VCF_CUTOFF;
        float VCF_RESONANCE;
        char VCF_TYPE;
        float ENV1_ATTACK;
        float ENV1_DECAY;
        float ENV1_SUSTAIN;
        float ENV1_RELEASE;
        float ENV2_ATTACK;
        float ENV2_DECAY;
        float ENV2_SUSTAIN;
        float ENV2_RELEASE;
        float LFO1_PERIOD;
        int LFO1_SUBGRMAX;
        float LFO2_PERIOD;
        int LFO2_SUBGRMAX;
        float LFO1_OSC1_PW;
        float LFO1_OSC2_PW;
        float LFO1_OSC1_PITCH;
        float LFO1_OSC2_PITCH;
        float LFO1_OSC1_VOLUME;
        float LFO1_OSC2_VOLUME; 
        float LFO1_VCF_CUTOFF;
        float LFO1_VCF_RESONANCE;  
        float LFO2_OSC1_PW;
        float LFO2_OSC2_PW;
        float LFO2_OSC1_PITCH;
        float LFO2_OSC2_PITCH;
        float LFO2_OSC1_VOLUME;
        float LFO2_OSC2_VOLUME;
        float LFO2_VCF_CUTOFF;
        float LFO2_VCF_RESONANCE;  
        float ENV1_OSC1_PW;
        float ENV1_OSC2_PW;
        float ENV1_OSC1_PITCH;
        float ENV1_OSC2_PITCH;
        float ENV1_OSC1_VOLUME;
        float ENV1_OSC2_VOLUME; 
        float ENV1_VCF_CUTOFF;
        float ENV1_VCF_RESONANCE;  
        float ENV2_OSC1_PW;
        float ENV2_OSC2_PW;
        float ENV2_OSC1_PITCH;
        float ENV2_OSC2_PITCH;
        float ENV2_OSC1_VOLUME;
        float ENV2_OSC2_VOLUME; 
        float ENV2_VCF_CUTOFF;
        float ENV2_VCF_RESONANCE;  
        float OSC3_VOLUME;
        unsigned char OSC3_SWITCH;
        float PTC_GLIDE;
        int64 PTC_GLIDE64;
        float GLB_VOLUME;
        float DISTO;
        float LFO1_ATTACK;
        float LFO1_DECAY;
        float LFO1_SUSTAIN;
        float LFO1_RELEASE;
        float LFO2_ATTACK;
        float LFO2_DECAY;
        float LFO2_SUSTAIN;
        float LFO2_RELEASE;

        char OSC_COMBINE;
} SYNTH_DATA, *LPSYNTH_DATA;

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
    #pragma pack(pop)
#endif

// ------------------------------------------------------
// Classes
class CSynth
{
    public:
   
        SYNTH_DATA Data;

        char ENV1_STAGE;
        char ENV2_STAGE;

        void Reset(void);

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
        void ChangeParameters(LPSYNTH_DATA TSP);
#else
        void ChangeParameters(SynthParameters TSP);
#endif

        /* Work functions */

        float GetSample(short *Left_Samples,
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
                        float Ampli_Vol);

        void NoteOn(int noten, float speed, int Looping, unsigned int Length,
                    unsigned int Loop_Length
#if defined(PTK_INSTRUMENTS)
                    ,float note_smp
#endif
                    , int glide
                   );
        void NoteOff(void);

        float FilterL(void);
        float FilterR(void);

        float MoogFilterL(void);
        float MoogFilterR(void);

        char ENV1_LOOP_BACKWARD;
        char ENV2_LOOP_BACKWARD;
        char ENV3_LOOP_BACKWARD;

    private:

        /* Internal Use */

        void LfoAdvance(void);
        void EnvRun(int *track, int *track2);
        float Math_Func(float in_old, float in_new);

        /* Synthesizer properties */
#if defined(PTK_SYNTH_FILTER)
        float FILT_CUTO;
        float FILT_RESO;
        float FILT_A;
        float FILT_B;
#endif

        float T_OSC_PW;
        float T_OSC1_VOLUME;
        float T_OSC2_VOLUME;

        int64 OSC1_SPEED;
        int64 OSC2_SPEED;
        int64 OSC3_SPEED;

        /* Envelopes and LFO's properties */
        float ENV1b_ATTACK;
        float ENV1b_DECAY;
        float ENV1b_RELEASE;

        float ENV2b_ATTACK;
        float ENV2b_DECAY;
        float ENV2b_RELEASE;

        float ENV1_A_COEF;
        float ENV1_D_COEF;
        float ENV1_R_COEF;

        float ENV2_A_COEF;
        float ENV2_D_COEF;
        float ENV2_R_COEF;

#if defined(PTK_SYNTH_LFO1)
        float LFO1_COUNTER;
        char LFO1_STAGE;
        float LFO1b_ATTACK;
        float LFO1b_DECAY;
        float LFO1b_RELEASE;
        float LFO1_A_COEF;
        float LFO1_D_COEF;
        float LFO1_R_COEF;
        float LFO1_ADSR_VALUE;
#endif

#if defined(PTK_SYNTH_LFO2)
        float LFO2_COUNTER;
        char LFO2_STAGE;
        float LFO2b_ATTACK;
        float LFO2b_DECAY;
        float LFO2b_RELEASE;
        float LFO2_A_COEF;
        float LFO2_D_COEF;
        float LFO2_R_COEF;
        float LFO2_ADSR_VALUE;
#endif

#if defined(PTK_SYNTH_LFO1)
        int LFO1_GR;
#endif

#if defined(PTK_SYNTH_LFO2)
        int LFO2_GR;
#endif

#if defined(PTK_SYNTH_LFO1)
        int LFO1_SUBGRCOUNTER;
#endif

#if defined(PTK_SYNTH_LFO2)
        int LFO2_SUBGRCOUNTER;
#endif

#if defined(PTK_SYNTH_LFO1)
        float LFO1_VALUE;
#endif

#if defined(PTK_SYNTH_LFO2)
        float LFO2_VALUE;
#endif

        /* Internal rendering variables */
#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
        float MoogBufferL[5];
        float MoogBufferR[5];
#endif

        float OSC1_STEP;
        float OSC2_STEP;

        float ENV1_COUNTER;
        float ENV2_COUNTER;

        float ENV1_VOLUME;
        float ENV2_VOLUME;

        float ENV1_VALUE;
        float ENV2_VALUE;

        float ENV1_MIN;
        float ENV2_MIN;

        float sbuf0L;
        float sbuf1L;

        float sbuf0R;
        float sbuf1R;

        float GS_VAL;
        float GS_VAL2;
};

#endif // PTK_SYNTH

#if defined(PTK_COMPRESSOR)
class rFilter
{
    private: 

        float buffy0;
        float buffy1;

    public:

        rFilter()
        {
            Reset();
        };

        void Reset()
        {
            buffy0 = 0.0f;
            buffy1 = 0.0f;
        };

        float fWork(float input, float f, float q)
        {
            float fa = 1.0f - f;
            float fb = float(q * (1.0f + (1.0f / fa)));

            buffy0 = fa * buffy0 + f * (input + fb * (buffy0 - buffy1));
            buffy1 = fa * buffy1 + f * buffy0;
            return buffy1;
        };
};
#endif

#endif
