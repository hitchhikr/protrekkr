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

#ifndef _EDITOR_303_H_
#define _EDITOR_303_H_

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
// Variables
extern int snamesel;
extern SDL_Surface *Main_Screen;
extern gear303 tb303engine[2];
extern para303 tb303[2];
extern unsigned char sl3;
extern int Refresh_Unit;
extern void Copy_303_Skin(int xd, int yd, int xs, int ys, int w, int h);

// ------------------------------------------------------
// Functions
void Draw_303_Ed(void);
void Actualize_303_Ed(char gode);
void Refresh_303_Unit(int Unit, int gode);
void Copy_303_Number(unsigned char number,int x,int y);
void Copy_303_Skin(int xd, int yd, int xs, int ys, int w, int h);
void Copy_303_Knob(int x, int y, unsigned char number);
void Mouse_Right_303_Ed(void);

void Mouse_Left_303_Ed(void);
void Mouse_Sliders_303_Ed(void);
void Mouse_Wheel_303_Ed(int roll_amount);

#endif
