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

#ifndef _EDITOR_MIDI_H_
#define _EDITOR_MIDI_H_

// ------------------------------------------------------
// Includes
#if defined(__GCC__)
#include "../../include/variables.h"
#include "../../ui/include/misc_draw.h"
#elif !defined(BZR2)
#include "../include/variables.h"
#include "../ui/include/misc_draw.h"
#else
#include "../include/variables.h"
#endif

// ------------------------------------------------------
// Constants
#define UPDATE_MIDI_ED_ALL 0
#define UPDATE_MIDI_ED_CC0 UPDATE_MIDI_ED_ALL + 1
#define UPDATE_MIDI_ED_CC1 UPDATE_MIDI_ED_CC0 + 1
#define UPDATE_MIDI_ED_CC2 UPDATE_MIDI_ED_CC1 + 1
#define UPDATE_MIDI_ED_CC3 UPDATE_MIDI_ED_CC2 + 1
#define UPDATE_MIDI_ED_CC4 UPDATE_MIDI_ED_CC3 + 1
#define UPDATE_MIDI_ED_CC5 UPDATE_MIDI_ED_CC4 + 1
#define UPDATE_MIDI_ED_CC6 UPDATE_MIDI_ED_CC5 + 1
#define UPDATE_MIDI_ED_CC7 UPDATE_MIDI_ED_CC6 + 1
#define UPDATE_MIDI_ED_CC8 UPDATE_MIDI_ED_CC7 + 1
#define UPDATE_MIDI_ED_CC9 UPDATE_MIDI_ED_CC8 + 1
#define UPDATE_MIDI_ED_CC10 UPDATE_MIDI_ED_CC9 + 1
#define UPDATE_MIDI_ED_CC11 UPDATE_MIDI_ED_CC10 + 1
#define UPDATE_MIDI_ED_CC12 UPDATE_MIDI_ED_CC11 + 1
#define UPDATE_MIDI_ED_CC13 UPDATE_MIDI_ED_CC12 + 1
#define UPDATE_MIDI_ED_CC14 UPDATE_MIDI_ED_CC13 + 1
#define UPDATE_MIDI_ED_SEL_IN UPDATE_MIDI_ED_CC14 + 1
#define UPDATE_MIDI_ED_SEL_OUT UPDATE_MIDI_ED_SEL_IN + 1

#define UPDATE_MIDI_ED_CHANGE_NAME UPDATE_MIDI_ED_SEL_OUT + 1

// ------------------------------------------------------
// Variables
extern char Midi_Name[20];

// ------------------------------------------------------
// Functions
void Draw_Midi_Ed(void);
void Actualize_Midi_Ed(char gode);
void Mouse_Left_Midi_Ed(void);
void Mouse_Right_Midi_Ed(void);

#endif
