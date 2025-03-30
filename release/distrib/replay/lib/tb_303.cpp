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
#include "include/tb_303.h"
#include "include/replay.h"

#if defined(PTK_303)

// ------------------------------------------------------
// Constructor
gear303::gear303()
{
    tbVolume = 0.5f;
    reset();
}

#define DENORMAL_303 1

#if DENORMAL_303
__inline float denormal_303(float sample)
{
    unsigned int isample;
    *(unsigned int *) &isample = *(unsigned int *) &sample;

    int abs_mantissa = isample & 0x007FFFFF;
    int biased_exponent = isample & 0x7F800000;
    if (biased_exponent == 0 && abs_mantissa != 0) 
    {
        return 0;
    }
    return sample;
}
#endif

// ------------------------------------------------------
// 303 initialization
void gear303::reset(void)
{
    int i;
    tbPattern = 255;
    tbLine = 255;
    tbCurrentVolume = 0.0f;
    tbRealVolume = 0.0f;
    tbInnertime = 0.0f;
    tbTargetVolume = 0.0f;
    tbRealCutoff = 0.0f;
    tbRealEnvmod = 0.0f;
    tbRealResonance = 0.0f;
    tbRealAccent = 0.0f;
    tbLastCutoff = 0.0f;
    tbLastEnvmod = 0.0f;
    tbLastResonance = 0.0f;
    tbAutoSlidect = 1;
    tbAutoSlideres = 1;
    tbAutoSlideenv = 1;
    tbAutoMod = 1;
    tbFirstRow = 1;
    tbTargetRealVolume = 1.0f;
    tbTargetRealVolumeRamp = 0.0f;
    tbCurMultiple = 0;

    tbCutoff = 0.5f;
    tbResonance = 0.5f;
    tbEnvmod = 0.5f;
    tbDecay = 0.5f;
    tbAccent = 0.0f;
    Note_Off = FALSE;

    for(i = 0 ; i < 5; i++)
    { 
        tbBuf[i] = 0.0f;
    }
    tbOscSpeedFreak = 0.0f;

    tbSample = 0.0f;
    tbOscPosition = 0.0f;
    tbOscSpeed = 0.0f;
    tbWaveform = TB303_WAVEFORM_SAW;
    RampVolume = 0.0f;
    tbRampVolume = 0.0f;
}

// ------------------------------------------------------
// Render 1 32bit-float sample
float gear303::tbGetSample(para303 *PARAT303)
{
    // Get Oscillator values
    switch(tbWaveform)
    {
        // SawTooth
        case TB303_WAVEFORM_SAW:
            tbSample = tbOscPosition * (1.0f + tbAccent);
            break;

        // Square
        case TB303_WAVEFORM_SQUARE:
            if(tbOscPosition < 0) tbSample = -16384.0f * (1.0f + tbAccent);
            else tbSample = 16384.0f * (1.0f + tbAccent);
            break;
    }

    // Run Oscillator
    tbOscPosition += tbOscSpeed;

    if(tbInnertime > 0)
    {
        tbInnertime--;
        tbOscSpeed += tbOscSpeedFreak;
    }

    if(tbOscPosition >= 16384.0f) tbOscPosition -= 32768.0f;

    // The sliding operation reduces the cutoff freq of 1/4
    // by the speed tbDecay.
    TickPos = (tbDecay / (float) PARAT303->scale) / (float) SamplesPerTick;

    if(tbAutoMod)
    {
        if(tbAutoSlidect)
        {
            tbLastCutoff = tbRealCutoff;
            tbRealCutoff -= TickPos;
            if(tbRealCutoff < -0.25f)
            {
                tbRealCutoff = -0.25f;
                tbAutoSlidect = 0;
            }
        }
     
        if(tbAutoSlideres)
        {   
            tbLastResonance = tbRealResonance;
            tbRealResonance -= TickPos;
            if(tbRealResonance < -0.25f)
            {
                tbRealResonance = -0.25f;
                tbAutoSlideres = 0;
            }
        }

        if(tbAutoSlideenv)
        {   
            tbLastEnvmod = tbRealEnvmod;
            tbRealEnvmod -= TickPos;
            if(tbRealEnvmod < tbEnvmod - 1.0f)
            {
                tbRealEnvmod = tbEnvmod - 1.0f;
                tbAutoSlideenv = 0;
            }
        }
    }

    tbRealCutoff2 = tbRealCutoff;
    tbRealEnvmod2 = tbRealEnvmod;
    tbRealResonance2 = tbRealResonance;
    tbTargetVolume = tbRealVolume;

    if(tbRealCutoff2 < -0.25f) tbRealCutoff2 = -0.25f;
    if(tbRealEnvmod2 < -0.25f) tbRealEnvmod2 = -0.25f;
    if(tbRealResonance2 < 0.0f) tbRealResonance2 = 0.0f;

    if(tbRealCutoff2 > 1.0f) tbRealCutoff2 = 1.0f;
    if(tbRealResonance2 > 1.0f) tbRealResonance2 = 1.0f;
    if(tbRealEnvmod2 > 1.0f) tbRealEnvmod2 = 1.0f;
    if(tbTargetVolume > 1.0f) tbTargetVolume = 1.0f;

    if(tbCurrentVolume < tbTargetVolume)
    {
        tbCurrentVolume += 0.001f;
        if(tbCurrentVolume > tbTargetVolume) tbCurrentVolume = tbTargetVolume;
    }
    else
    {
        tbCurrentVolume -= 0.001f;
        if(tbCurrentVolume < tbTargetVolume) tbCurrentVolume = tbTargetVolume;
    }

    float output = tbFilter() * (1.0f + tbAccent);
    if(output < -32767.0f) output = -32767.0f;
    if(output > 32767.0f) output = 32767.0f;

    if(tbAccent > 0.0f)
    {
        tbAccent -= 0.0001f;
        if(tbAccent < 0.0f) tbAccent = 0.0f;
    }

    if(tbRampVolume > RampVolume)
    {
        // That's for an explicit note off
        tbRampVolume -= 0.0005f;
        if(tbRampVolume < RampVolume) tbRampVolume = RampVolume;
    }
    else
    {
        tbRampVolume += 0.05f;
        if(tbRampVolume > RampVolume) tbRampVolume = RampVolume;
    }

    if(tbTargetRealVolumeRamp > tbTargetRealVolume)
    {
        tbTargetRealVolumeRamp -= 0.1f;
        if(tbTargetRealVolumeRamp < tbTargetRealVolume) tbTargetRealVolumeRamp = tbTargetRealVolume;
    }
    else
    {
        tbTargetRealVolumeRamp += 0.1f;
        if(tbTargetRealVolumeRamp > tbTargetRealVolume) tbTargetRealVolumeRamp = tbTargetRealVolume;
    }
    return(output * tbTargetRealVolumeRamp * tbRampVolume * tbCurrentVolume);
}

// ------------------------------------------------------
// Do Note On
void gear303::tbNoteOn(int tbNote, para303 *PARAT303)
{
    if(!Note_Off)
    {
        if(PARAT303->flag[tbPattern][tbLine].transposeup_flag) tbNote += 12;
        if(PARAT303->flag[tbPattern][tbLine].transposedown_flag) tbNote -= 12;

        RampVolume = 1.0f;

        tbWaveform = PARAT303->waveform;
        tbOscSpeedFreak = 0;
        float ftune = float(tbNote) - 17;

        // 0.1889763f seems to be about right
        ftune += (float) PARAT303->tune * 0.1889763f;
        float tbOscSpeedTmp = POWF2(ftune / 12.0f) * 64.0f;

        int tbLine2 = (tbLine - 1);
        if(tbLine2 < 0) tbLine2 = PARAT303->patternlength[tbPattern] - 1;
        if(tbLine2 < 0) tbLine2 = 0;
        if(tbLine == 255) tbLine = PARAT303->patternlength[tbPattern] - 1;

        // Are we sliding ?
        if(PARAT303->flag[tbPattern][tbLine].slide_flag)
        {
            // Coming from a slide ?
            if(PARAT303->flag[tbPattern][tbLine2].slide_flag && !tbFirstRow)
            {
                tbRealCutoff = tbLastCutoff;
                tbRealEnvmod = tbLastEnvmod;
                tbRealResonance = tbLastResonance;
            }
            else
            {
                // Re-init & prepare
                tbFirstRow = 0;
                tbAutoSlidect = 1;
                tbAutoSlideres = 1;
                tbAutoSlideenv = 1;
                tbAutoMod = 1;
                tbRealCutoff = (float(PARAT303->cutoff) / 127.0f);
                tbRealEnvmod = (((float) PARAT303->envmod) / 127.0f);
                tbRealResonance = (((float) PARAT303->resonance) / 127.0f);          
                tbDecay = (1.0f - (((float) PARAT303->decay) / 127.0f));
            }
        }
        else
        {
            // coming from a slide ?
            if(PARAT303->flag[tbPattern][tbLine2].slide_flag)
            {
                // First time played ?
                if(tbFirstRow)
                {
                    // Re-init the sliding
                    tbAutoSlidect = 1;
                    tbAutoSlideres = 1;
                    tbAutoSlideenv = 1;
                    tbAutoMod = 1;
                    tbRealCutoff = (float(PARAT303->cutoff) / 127.0f);
                    tbRealEnvmod = (((float) PARAT303->envmod) / 127.0f);
                    tbRealResonance = (((float) PARAT303->resonance) / 127.0f);          
                    tbDecay = (1.0f - (((float) PARAT303->decay) / 127.0f));
                }
                else
                {
                    // Continue...
                    tbRealCutoff = tbLastCutoff;
                    tbRealEnvmod = tbLastEnvmod;
                    tbRealResonance = tbLastResonance;
                }
            }
            else
            {
                // Re-init
                tbAutoSlidect = 1;
                tbAutoSlideres = 1;
                tbAutoSlideenv = 1;
                tbAutoMod = 1;
                tbRealCutoff = (float(PARAT303->cutoff) / 127.0f);
                tbRealEnvmod = (((float) PARAT303->envmod) / 127.0f);
                tbRealResonance = (((float) PARAT303->resonance) / 127.0f);          
                tbDecay = (1.0f - (((float) PARAT303->decay) / 127.0f));
            }
        }

        char forcefault = 1;
        // Don't use slide when the pattern is starting
        if(PARAT303->flag[tbPattern][tbLine2].slide_flag && !tbFirstRow)
        {
            // New speed to reach
            tbOscSpeed = tbOscSpeedTmp;
            forcefault = 0;
            // Retrieve the previous speed
            ftune = float(PARAT303->tone[tbPattern][tbLine2]) - 17;
            if(PARAT303->flag[tbPattern][tbLine2].transposeup_flag) ftune += 12;
            if(PARAT303->flag[tbPattern][tbLine2].transposedown_flag) ftune -= 12;
            ftune += (float) PARAT303->tune * 0.1889763f;
            tbInnertime = SamplesPerTick * 0.5f;
            float tbDestiny = tbOscSpeed;                            // Dest
            float tbSource = POWF2(ftune / 12.0f) * 64.0f;
            // Previous speed
            tbOscSpeed = tbSource;
            tbOscSpeedFreak = (tbDestiny - tbSource) / tbInnertime;  // Get glide coefficient
        }

        // Start volume ramping if there's a pause and we're not sliding
        if(!PARAT303->flag[tbPattern][tbLine].pause && forcefault)
        {
            tbRealVolume = 0;
            tbAutoSlidect = 1;
            tbAutoSlideres = 1;
            tbAutoSlideenv = 1;
            tbAutoMod = 1;
            tbRealCutoff = tbLastCutoff;
            tbRealEnvmod = tbLastEnvmod;
            tbRealResonance = tbLastResonance;
        }
        else
        {
            if(forcefault)
            {
                tbOscSpeed = tbOscSpeedTmp;
                tbOscPosition = 0.0f;
                tbCutoff = (float(PARAT303->cutoff) / 127.0f);
                tbRealCutoff = tbCutoff;
                tbEnvmod = (((float) PARAT303->envmod) / 127.0f);
                tbRealEnvmod = tbEnvmod;
                tbResonance = ((float) PARAT303->resonance) / 127.0f;
                tbRealResonance = tbResonance;
                tbDecay = (1.0f - (((float) PARAT303->decay) / 127.0f));
            }

            // There's a slight deprecation when sliding
            if(PARAT303->flag[tbPattern][tbLine].slide_flag)
            {
                tbDecay += 0.05f;
            }
            tbRealVolume = tbVolume;

            // Accent is only used if there's a note to play
            if(PARAT303->flag[tbPattern][tbLine].accent_flag)
            {
                tbAccent = (((float) PARAT303->accent / 127.0f)) * 2.0f;
                // Decay is automatically turned on
                tbDecay = 1.0f;
            }
        }
    }
}

// ------------------------------------------------------
// Filter routine.
// That's a custom moog like filter
float gear303::tbFilter(void)
{
    float f;
    float p;
    float q;
    float in;
    float t[4];
    float cut = tbRealCutoff2;
    float res = tbRealResonance2;
    float envcut;
    float envcut2;

    envcut = 1.0f + (tbRealEnvmod2 * 0.85f);
    envcut2 = 1.0f - (tbRealEnvmod2 * 0.85f);
    cut += 0.55f;
    cut *= envcut;
    cut *= 0.50f;
#if DENORMAL_303
    cut = denormal_303(cut);
#endif
    q = (0.85f - cut);
    p = ((cut * cut) * 0.45f);
    f = (p + p) - 1.0f;
    res *= 5.0f;
    q = res * ((1.0f + (0.5f * q) * (1.0f - q + (5.6f * q * q))));
    if(q > 2.42f) q = 2.42f;
#if DENORMAL_303
    q = denormal_303(q);
    tbBuf[0] = denormal_303(tbBuf[0]);
    tbBuf[1] = denormal_303(tbBuf[1]);
    tbBuf[2] = denormal_303(tbBuf[2]);
    tbBuf[3] = denormal_303(tbBuf[3]);
    tbBuf[4] = denormal_303(tbBuf[4]);
#endif
    in = (tbSample / 16384.0f) - (q * tbBuf[4] * envcut2);
#if DENORMAL_303
    in = denormal_303(in);
#endif
    t[1] = tbBuf[1];
    t[2] = tbBuf[2];
    t[3] = tbBuf[3];
    tbBuf[1] = ((in + tbBuf[0]) * p) - (tbBuf[1] * f);
    tbBuf[2] = ((tbBuf[1] + t[1]) * p) - (tbBuf[2] * f);
    tbBuf[3] = ((tbBuf[2] + t[2]) * p) - (tbBuf[3] * f);
    tbBuf[4] = ((tbBuf[3] + t[3]) * p) - (tbBuf[4] * f);
    tbBuf[0] = in;
    return(tbBuf[4] * 16384.0f);
}

#endif
