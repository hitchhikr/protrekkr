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
#define WAVEFORM_TRI 6

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
#if defined(__HAIKU__) || defined(__LINUX__) || defined(__AROS__)
#include <stdint.h>
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

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
    #if !defined(__GCC__)
    #pragma pack(push)
    #pragma pack(1)
    #else
    #pragma pack(push, 1)
    #endif
#endif

struct Synth_Parameters
{
    char Preset_Name[20];

    unsigned char osc_1_waveform;
    unsigned char osc_2_waveform;
    char osc_combine;

    int osc_1_pw;
    int osc_2_pw;

    unsigned char osc_2_detune;
    unsigned char osc_2_finetune;

    unsigned char vcf_cutoff;
    unsigned char vcf_resonance;
    unsigned char vcf_type;

    /* Envelopes and LFO's properties */

    int env_1_attack;
    int env_1_decay;
    unsigned char env_1_sustain;
    int env_1_release;

    int env_2_attack;
    int env_2_decay;
    unsigned char env_2_sustain;
    int env_2_release;

    int lfo_1_period;
    int lfo_2_period;

    /* Envelopes and LFO's modulation variables */

    unsigned char lfo_1_osc_1_pw;
    unsigned char lfo_1_osc_2_pw;
    unsigned char lfo_1_osc_1_pitch;
    unsigned char lfo_1_osc_2_pitch;
    unsigned char lfo_1_osc_1_volume;
    unsigned char lfo_1_osc_2_volume;
    unsigned char lfo_1_vcf_cutoff;
    unsigned char lfo_1_vcf_resonance;

    unsigned char lfo_2_osc_1_pw;
    unsigned char lfo_2_osc_2_pw;
    unsigned char lfo_2_osc_1_pitch;
    unsigned char lfo_2_osc_2_pitch;
    unsigned char lfo_2_osc_1_volume;
    unsigned char lfo_2_osc_2_volume;
    unsigned char lfo_2_vcf_cutoff;
    unsigned char lfo_2_vcf_resonance;

    unsigned char env_1_osc_1_pw;
    unsigned char env_1_osc_2_pw;
    unsigned char env_1_osc_1_pitch;
    unsigned char env_1_osc_2_pitch;
    unsigned char env_1_osc_1_volume;
    unsigned char env_1_osc_2_volume;  
    unsigned char env_1_vcf_cutoff;
    unsigned char env_1_vcf_resonance;

    unsigned char env_2_osc_1_pw;
    unsigned char env_2_osc_2_pw;
    unsigned char env_2_osc_1_pitch;
    unsigned char env_2_osc_2_pitch;
    unsigned char env_2_osc_1_volume;
    unsigned char env_2_osc_2_volume;  
    unsigned char env_2_vcf_cutoff;
    unsigned char env_2_vcf_resonance;

    unsigned char osc_3_volume;
    unsigned char osc_3_switch;

    unsigned char ptc_glide;
    unsigned char glb_volume;

    unsigned char disto;

    int lfo_1_attack;
    int lfo_1_decay;
    unsigned char lfo_1_sustain;
    int lfo_1_release;

    int lfo_2_attack;
    int lfo_2_decay;
    unsigned char lfo_2_sustain;
    int lfo_2_release;

    unsigned char lfo_1_disto;
    unsigned char lfo_2_disto;
    unsigned char env_1_disto;
    unsigned char env_2_disto;

    char osc_sync;
    char osc_3_interval;
};

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
    #pragma pack(pop)
#endif

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

#if defined(PTK_SYNTH_TRI)
extern short STOCK_TRI[SIZE_WAVEFORMS_SPACE];
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
        char OSC_1_WAVEFORM;
        char OSC_2_WAVEFORM;

        float OSC_1_PW;
        float OSC_2_PW; 
        
        float OSC_2_DETUNE;
        float OSC_2_FINETUNE;
        
        float VCF_CUTOFF;
        float VCF_RESONANCE;

        char VCF_TYPE;
        
        float ENV_1_ATTACK;
        float ENV_1_DECAY;
        float ENV_1_SUSTAIN;
        float ENV_1_RELEASE;
        
        float ENV_2_ATTACK;
        float ENV_2_DECAY;
        float ENV_2_SUSTAIN;
        float ENV_2_RELEASE;
        
        float LFO_1_PERIOD;
        int LFO_1_SUBGRMAX;
        float LFO_2_PERIOD;
        int LFO_2_SUBGRMAX;

        float LFO_1_OSC_1_PW;
        float LFO_1_OSC_2_PW;
        float LFO_1_OSC_1_PITCH;
        float LFO_1_OSC_2_PITCH;
        float LFO_1_OSC_1_VOLUME;
        float LFO_1_OSC_2_VOLUME; 
        float LFO_1_VCF_CUTOFF;
        float LFO_1_VCF_RESONANCE;  
        float LFO_1_DISTO;
        
        float LFO_2_OSC_1_PW;
        float LFO_2_OSC_2_PW;
        float LFO_2_OSC_1_PITCH;
        float LFO_2_OSC_2_PITCH;
        float LFO_2_OSC_1_VOLUME;
        float LFO_2_OSC_2_VOLUME;
        float LFO_2_VCF_CUTOFF;
        float LFO_2_VCF_RESONANCE;  
        float LFO_2_DISTO;
        
        float ENV_1_OSC_1_PW;
        float ENV_1_OSC_2_PW;
        float ENV_1_OSC_1_PITCH;
        float ENV_1_OSC_2_PITCH;
        float ENV_1_OSC_1_VOLUME;
        float ENV_1_OSC_2_VOLUME;
        float ENV_1_VCF_CUTOFF;
        float ENV_1_VCF_RESONANCE;
        float ENV_1_DISTO;
        
        float ENV_2_OSC_1_PW;
        float ENV_2_OSC_2_PW;
        float ENV_2_OSC_1_PITCH;
        float ENV_2_OSC_2_PITCH;
        float ENV_2_OSC_1_VOLUME;
        float ENV_2_OSC_2_VOLUME; 
        float ENV_2_VCF_CUTOFF;
        float ENV_2_VCF_RESONANCE;  
        float ENV_2_DISTO;
        
        float OSC_3_VOLUME;
        unsigned char OSC_3_SWITCH;
        float PTC_GLIDE;
        int64 PTC_GLIDE_64;
        float GLB_VOLUME;
        float DISTO;
        float LFO_1_ATTACK;
        float LFO_1_DECAY;
        float LFO_1_SUSTAIN;
        float LFO_1_RELEASE;
        float LFO_2_ATTACK;
        float LFO_2_DECAY;
        float LFO_2_SUSTAIN;
        float LFO_2_RELEASE;

        char OSC_COMBINE;
        char OSC_SYNC;
        char OSC_3_INTERVAL;

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

        char ENV_1_STAGE;
        char ENV_2_STAGE;

        void Reset(void);

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
        void ChangeParameters(LPSYNTH_DATA TSP);
#else
        void ChangeParameters(Synth_Parameters TSP);
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
                        Uint64 *position_osc_1,
                        Uint64 *position_osc_2,

#if defined(PTK_SYNTH_OSC_3)
                        Uint64 *position_osc_3,
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
        void Note_Off(void);

        float Filter_L(void);
        float Filter_R(void);

        float Moog_Filter_L(void);
        float Moog_Filter_R(void);

        char ENV_1_LOOP_BACKWARD;
        char ENV_2_LOOP_BACKWARD;
        char ENV_3_LOOP_BACKWARD;

    private:

        /* Internal Use */

        void Lfo_Advance(void);
        void Env_Run(int *track, int *track2);
        float Math_Func(float in_old, float in_new);

        /* Synthesizer properties */
#if defined(PTK_SYNTH_FILTER)
        float FILT_CUTO;
        float FILT_RESO;
        float FILT_A;
        float FILT_B;
#endif

        float T_OSC_PW;
        float T_OSC_1_VOLUME;
        float T_OSC_2_VOLUME;
        
        int64 OSC_1_SPEED
#if defined(__GCC__)
        __attribute__((aligned(8)))
#endif
        ;
        int64 OSC_2_SPEED
#if defined(__GCC__)
        __attribute__((aligned(8)))
#endif
        ;
        int64 OSC_3_SPEED
#if defined(__GCC__)
        __attribute__((aligned(8)))
#endif
        ;
        int64 GLIDE_64_ALIGN
#if defined(__GCC__)
        __attribute__((aligned(8)))
#endif
        ;

        /* Envelopes and LFO's properties */
        float ENV_1b_ATTACK;
        float ENV_1b_DECAY;
        float ENV_1b_RELEASE;

        float ENV_2b_ATTACK;
        float ENV_2b_DECAY;
        float ENV_2b_RELEASE;

        float ENV_1_A_COEF;
        float ENV_1_D_COEF;
        float ENV_1_R_COEF;

        float ENV_2_A_COEF;
        float ENV_2_D_COEF;
        float ENV_2_R_COEF;

#if defined(PTK_SYNTH_LFO_1)
        float LFO_1_COUNTER;
        char LFO_1_STAGE;
        float LFO_1b_ATTACK;
        float LFO_1b_DECAY;
        float LFO_1b_RELEASE;
        float LFO_1_A_COEF;
        float LFO_1_D_COEF;
        float LFO_1_R_COEF;
        float LFO_1_ADSR_VALUE;
#endif

#if defined(PTK_SYNTH_LFO_2)
        float LFO_2_COUNTER;
        char LFO_2_STAGE;
        float LFO_2b_ATTACK;
        float LFO_2b_DECAY;
        float LFO_2b_RELEASE;
        float LFO_2_A_COEF;
        float LFO_2_D_COEF;
        float LFO_2_R_COEF;
        float LFO_2_ADSR_VALUE;
#endif

#if defined(PTK_SYNTH_LFO_1)
        int LFO_1_GR;
#endif

#if defined(PTK_SYNTH_LFO_2)
        int LFO_2_GR;
#endif

#if defined(PTK_SYNTH_LFO_1)
        int LFO_1_SUBGRCOUNTER;
#endif

#if defined(PTK_SYNTH_LFO_2)
        int LFO_2_SUBGRCOUNTER;
#endif

#if defined(PTK_SYNTH_LFO_1)
        float LFO_1_VALUE;
#endif

#if defined(PTK_SYNTH_LFO_2)
        float LFO_2_VALUE;
#endif

        /* Internal rendering variables */
#if defined(PTK_SYNTH_FILTER_MOOG_LO) || defined(PTK_SYNTH_FILTER_MOOG_BAND)
        float MoogBufferL[5];
        float MoogBufferR[5];
#endif

        float OSC_1_STEP;
        float OSC_2_STEP;

        float ENV_1_COUNTER;
        float ENV_2_COUNTER;

        float ENV_1_VOLUME;
        float ENV_2_VOLUME;

        float ENV_1_VALUE;
        float ENV_2_VALUE;

        float ENV_1_MIN;
        float ENV_2_MIN;

        float sbuf0L;
        float sbuf1L;

        float sbuf0R;
        float sbuf1R;

        float GS_VAL_L;
        float GS_VAL_R;
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
