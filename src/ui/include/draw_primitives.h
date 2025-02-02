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
#if defined(__USE_OPENGL__)
#if defined(__WIN32__)
#include <windows.h>
#endif
#include <SDL/SDL_opengl.h>
#define TEXTURES_SIZE 1024
#endif

// ------------------------------------------------------
// Constants
#define UPDATE_STACK_SIZE 2048

// ------------------------------------------------------
// Variables
extern int Cur_Width;
extern int Cur_Height;

// ------------------------------------------------------
// Functions
void DrawPixel(int x, int y, int Color);
void DrawHLine(int y, int x1, int x2, int Color);
void DrawVLine(int x, int y1, int y2, int Color);
void SetColor(int color);
void Fillrect(int x1, int y1, int x2, int y2);
void UISetPalette(SDL_Color *Palette, int Amount);
#if defined(__USE_OPENGL__)
GLuint Create_Texture(SDL_Surface *Source);
void Destroy_Texture(GLuint *txId);
void Draw_Tx_Quad(float x, float y, float x1, float y1, float Width, float Height, GLuint TexID, int Blend);
void Copy(GLuint Source, int x, int y, int x1, int y1, int x2, int y2);
void Copy_No_Refresh(GLuint Source, int x, int y, int x1, int y1, int x2, int y2, int remainder);
#else
void Copy(SDL_Surface *Source, int x, int y, int x1, int y1, int x2, int y2);
void Copy_No_Refresh(SDL_Surface *Source, int x, int y, int x1, int y1, int x2, int y2, int remainder);
#endif
void Copy_To_Surface(SDL_Surface *Source, SDL_Surface *dest,
                     int dest_x, int dest_y, int src_start_x, int src_start_y, int src_end_x, int src_end_y);
void Print_String(int x, int y, int Font_Type, char *String, int max_x = -1);
void Push_Update_Rect(int x, int y, int width, int height);
#if defined(__USE_OPENGL__)
void Enter_2D_Mode(float Width, float Height);
void Leave_2d_Mode(void);
#endif

#endif
