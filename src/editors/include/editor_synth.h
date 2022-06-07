// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2022 Franck Charlet.
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

#ifndef _EDITOR_SYNTH_H_
#define _EDITOR_SYNTH_H_

// ------------------------------------------------------
// Includes
#if defined(__GCC__)
#include "../../include/variables.h"
#include "../../ui/include/misc_draw.h"
#else
#include "../include/variables.h"
#include "../ui/include/misc_draw.h"
#endif

// ------------------------------------------------------
// Constants
#define UPDATE_SYNTH_ED_ALL 0
#define UPDATE_SYNTH_ED_VALUES 1
#define UPDATE_SYNTH_ED_OSC1_PARAMS 2
#define UPDATE_SYNTH_ED_OSC1_WAVEFORM 3
#define UPDATE_SYNTH_ED_OSC2_WAVEFORM 4
#define UPDATE_SYNTH_ED_OSC3_SWITCH 5
#define UPDATE_SYNTH_ED_VALIDATE_PARAMS 6
#define UPDATE_SYNTH_ED_VCF_TYPE 7
#define UPDATE_SYNTH_ED_OSC2_PARAMS 8
#define UPDATE_SYNTH_ED_VCF_PARAMS 9
#define UPDATE_SYNTH_ED_LFO1_PARAMS 10
#define UPDATE_SYNTH_ED_LFO2_PARAMS 11
#define UPDATE_SYNTH_ED_ENV1_PARAMS 12
#define UPDATE_SYNTH_ED_ENV2_PARAMS 13
#define UPDATE_SYNTH_ED_Misc_PARAMS 14
#define UPDATE_SYNTH_CHANGE_NAME 15
#define UPDATE_SYNTH_CHANGE_ASSOCIATED_SAMPLE 16
#define UPDATE_SYNTH_ED_COMBINER 17

// ------------------------------------------------------
// Functions
void Draw_Synth_Ed(void);
void Actualize_Synth_Ed(char gode);
void Mouse_Sliders_Synth_Ed(void);
void Mouse_Right_Synth_Ed(void);
void Mouse_Left_Synth_Ed(void);
void Actualize_SynthParSlider(void);
void IniCsParNames(void);

#endif
