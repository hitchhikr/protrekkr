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

#ifndef __DRAW_PRIMITIVES_H_
#define __DRAW_PRIMITIVES_H_

// ------------------------------------------------------
// Include
#include <SDL/SDL.h>

// ------------------------------------------------------
// Constants
#define UPDATE_STACK_SIZE 2048

// ------------------------------------------------------
// Variables
extern int Cur_Width;
extern int Cur_Height;

// ------------------------------------------------------
// Functions
void DrawLine(int x1, int y1, int x2, int y2);
void DrawPixel(int x, int y, int Color);
void DrawHLine(int y, int x1, int x2, int Color);
void DrawVLine(int x, int y1, int y2, int Color);
void SetColor(int color);
void Fillrect(int x1, int y1, int x2, int y2);
void UISetPalette(SDL_Color *Palette, int Amount);
void Copy(SDL_Surface *Source, int x, int y, int x1, int y1, int x2, int y2);
void Copy_To_Surface(SDL_Surface *Source, SDL_Surface *dest, int x, int y, int x1, int y1, int x2, int y2);
void PrintString(int x, int y, int Font_Type, char *String, int max_x = -1);
void Push_Update_Rect(int x, int y, int width, int height);

#endif
