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

#ifndef _EDITOR_REVERB_ED_H_
#define _EDITOR_REVERB_ED_H_

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
#define UPDATE_REVERB_ED_ALL 0
#define UPDATE_REVERB_ED_ECHOES 1

#define UPDATE_REVERB_ED_DELAY1 2
#define UPDATE_REVERB_ED_DELAY2 3
#define UPDATE_REVERB_ED_DELAY3 4
#define UPDATE_REVERB_ED_DELAY4 5
#define UPDATE_REVERB_ED_DELAY5 6
#define UPDATE_REVERB_ED_DELAY6 7
#define UPDATE_REVERB_ED_DELAY7 8
#define UPDATE_REVERB_ED_DELAY8 9
#define UPDATE_REVERB_ED_DELAY9 10
#define UPDATE_REVERB_ED_DELAY10 11

#define UPDATE_REVERB_ED_DECAY1 12
#define UPDATE_REVERB_ED_DECAY2 13
#define UPDATE_REVERB_ED_DECAY3 14
#define UPDATE_REVERB_ED_DECAY4 15
#define UPDATE_REVERB_ED_DECAY5 16
#define UPDATE_REVERB_ED_DECAY6 17
#define UPDATE_REVERB_ED_DECAY7 18
#define UPDATE_REVERB_ED_DECAY8 19
#define UPDATE_REVERB_ED_DECAY9 20
#define UPDATE_REVERB_ED_DECAY10 21

#define UPDATE_REVERB_ED_ARROWS 22

#define UPDATE_REVERB_ED_CHANGE_NAME 23

// ------------------------------------------------------
// Variables
extern char Reverb_Name[20];

// ------------------------------------------------------
// Functions
void Draw_Reverb_Ed(void);
void Actualize_Reverb_Ed(int gode);
void Mouse_Right_Reverb_Ed(void);
void Mouse_Left_Reverb_Ed(void);
void Mouse_Sliders_Reverb_Ed(void);
void Mouse_Sliders_Right_Reverb_Ed(void);
void Mouse_Reverb_Ed();

#endif
