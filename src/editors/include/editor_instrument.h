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

#ifndef _EDITOR_INSTRUMENT_H_
#define _EDITOR_INSTRUMENT_H_

// ------------------------------------------------------
// Includes
#if defined(__GCC__)
#include "../../include/variables.h"
#include "../../ui/include/misc_draw.h"
#include "../../samples/wav/include/riff.h"
#else
#include "../include/variables.h"
#include "../ui/include/misc_draw.h"
#include "../samples/wav/include/riff.h"
#endif

// ------------------------------------------------------
// VAriables
extern int Instrs_index;
extern int Instrs_ykar;

// ------------------------------------------------------
// Functions
void Draw_Instrument_Ed(void);
void Actualize_Instrument_Ed(int typex, char gode);
void Mouse_Sliders_Instrument_Ed(void);
void Mouse_Left_Instrument_Ed(void);
void Mouse_Right_Instrument_Ed(void);
void Mouse_Left_Repeat_Instrument_Ed(void);
void Mouse_Sliders_Right_Instrument_Ed(void);
void Afloop(void);
void Dump_Instruments_Synths_List(int xr, int yr);
void Actualize_Instruments_Synths_List(int modeac);
void Save_WaveForm(int Instr_Nbr, int Channel, int Split);
void Restore_WaveForm(int Instr_Nbr, int Channel, int Split);

#endif
