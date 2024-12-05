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

#ifndef _TB303_H_
#define _TB303_H_

// ------------------------------------------------------
// Includes
#include <math.h>

#if defined(__STAND_ALONE__)
    #if defined(__WINAMP__)
        #include "ptk_def_properties.h"
    #else
        #if defined(__GCC__)
        #include "../../ptk_properties.h"
        #else
        #include "../ptk_properties.h"
        #endif
    #endif
#else
    #include "ptk_def_properties.h"
#endif

#if defined(PTK_303)

// ------------------------------------------------------
// Constants
#define TB303_WAVEFORM_SAW 0
#define TB303_WAVEFORM_SQUARE 1

// ------------------------------------------------------
// Structures

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
    #if !defined(__GCC__)
    #pragma pack(push)
    #pragma pack(1)
    #else
    #pragma pack(push, 1)
    #endif
#endif

#if defined(__BIG_ENDIAN__)
    struct flag303
    {
        unsigned reserved_flag : 27;
        unsigned pause : 1;
        unsigned transposedown_flag : 1;
        unsigned transposeup_flag : 1;
        unsigned accent_flag : 1;
        unsigned slide_flag : 1;
    };
#else
    struct flag303
    {
        unsigned slide_flag : 1;
        unsigned accent_flag : 1;
        unsigned transposeup_flag : 1;
        unsigned transposedown_flag : 1;
        unsigned pause : 1;
        unsigned reserved_flag : 27;
    };
#endif

struct para303
{
    unsigned char enabled;           //Enabled                 UBYTE           0       0x00 = off, 0x01 = on (pattern mode)
    unsigned char selectedpattern;   //Selected pattern        UBYTE           1       0x00 to 0x20 (pattern mode)
    unsigned char tune;              //Tune                    UBYTE           2       0x00 to 0x7f (pattern mode)
    unsigned char cutoff;            //Cutoff                  UBYTE           3       0x00 to 0x7f (pattern mode)
    unsigned char resonance;         //Resonance               UBYTE           4       0x00 to 0x7f (pattern mode)
    unsigned char envmod;            //EnvMod                  UBYTE           5       0x00 to 0x7f (pattern mode)
    unsigned char decay;             //Decay                   UBYTE           6       0x00 to 0x7f (pattern mode)
    unsigned char accent;            //Accent                  UBYTE           7       0x00 to 0x7f (pattern mode)
    unsigned char waveform;          //Waveform                UBYTE           8       0x00 = triangle, 0x01 = square (pattern mode)
    unsigned char scale;
    
    unsigned char patternlength[32]; // 32 = 4 bank * 8 patterns
    unsigned char tone[32][16];
    struct flag303 flag[32][16];

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    char pattern_name[32][20];
#endif

    //    32*Pattern (1088 bytes)             9,9+(1*34),9+(2*34),9+(3*34)...
    //    |   Shuffle         UBYTE                   0x00 = off, 0x01 = on
    //    |   Pattern length  UBYTE                   0x01 to 0x10
    //    |
    //    |   16*Step (32 bytes)              11,11+(1*34),11+(2*34),11+(3*34)...
    //    |   |   Tone/pitch  UBYTE                   0x00 to 0x0c
    //    |   |   Flags       BITMASK8                bit 0 = No slide/Slide (0x01)
    //    |   |                                       bit 1 = No accent/Accent (0x02)
    //    |   |                                       bit 2 = Normal/Transpose up (0x04)
    //    |   |                                       bit 3 = Normal/Transpose down (0x08)
    //    \   \                                       bit 4 = Pause/Note (0x10)
};

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
    #pragma pack(pop)
#endif

// ------------------------------------------------------
// Variables
extern int SamplesPerTick;
extern int PosInTick;
extern int PosInTick_Delay;

// ------------------------------------------------------
// Classes
class gear303
{
    public:

        unsigned int tbPattern;     // From 0 to 31, 255 - Off
        unsigned char tbLine;       // From 0 to 15, 255 - Off

        gear303();

        void reset(void);

        void tbNoteOn(int tbNote, para303 *PARAT303);

        float tbGetSample(para303 *PARAT303);
        float tbBuf[5];
        float tbVolume;
        float TickPos;
        int Note_Off;
        int tbCurMultiple;

        float RampVolume;
        float tbTargetRealVolume;

    private:

        float tbFilter(void);

        // 303 Parameters
        float tbTargetRealVolumeRamp;
        float tbCutoff;
        float tbResonance;
        float tbEnvmod;
        float tbDecay;
        float tbAccent;
        float tbSample;
        float tbRealAccent;
        float tbRealCutoff;
        float tbRealCutoff2;
        float tbRealResonance;
        float tbRealResonance2;
        float tbLastCutoff;
        float tbLastEnvmod;
        float tbLastResonance;
        float tbRealEnvmod;
        float tbRealEnvmod2;
        float tbRealVolume;
        float tbOscSpeedFreak;
        float tbTargetVolume;
        float tbCurrentVolume;
        float tbInnertime;
        // Oscillator variables
        float tbOscPosition;
        float tbOscSpeed;
        float tbFadeCutOffTime;
        float tbFadeCutOffInterval;

        float tbRampVolume;
        float tbRampCutOff;

        int tbAutoSlidect;
        int tbAutoSlideres;
        int tbAutoSlideenv;
        int tbAutoMod;
        int tbFirstRow;

        // Waveform Type
        char tbWaveform;
};

// ------------------------------------------------------
// Functions
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
void ReInit_303_Parameters(para303 *tbpars);
void Reset_303_Parameters(para303 *tbpars);
#endif

#endif

#endif
