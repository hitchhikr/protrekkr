// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2026 Franck Charlet.
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
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#if defined(__WIN32__)
#include <windows.h>
#endif
#ifndef __MORPHOS__
#include <SDL2/SDL_opengl.h>
#else
#include <GL/gl.h>
#endif
#define TEXTURES_SIZE 1024
#if defined(__WIN32__)
#include <SDL2/SDL_syswm.h>
#endif
#include <SDL2/SDL_types.h>

// ------------------------------------------------------
// Constants
#define GET_SURFACE(s) (s##_GL)

// ------------------------------------------------------
// Variables
extern int Cur_Width;
extern int Cur_Height;

// ------------------------------------------------------
// Functions
#define PTK_TEXTURE SDL_Surface
#define PTK_COLOR SDL_Color
#define PTK_PALETTE SDL_Palette

// ------------------------------------------------------
// Functions
void DrawPixel(int x, int y, int Color);
void DrawHLine(int y, int x1, int x2, int Color);
void DrawVLine(int x, int y1, int y2, int Color);
void Set_Color(int color);
void Fill_Rect(int x1, int y1, int x2, int y2);
void UI_Set_Palette(PTK_COLOR *Palette, int Amount);
GLuint Create_OGL_Texture(PTK_TEXTURE *Source);
void Destroy_OGL_Texture(GLuint *txId);
void Draw_Tx_Quad(int x, int y, int x1, int y1, int Width, int Height, GLuint TexID, int Blend);
void Copy(GLuint Source, int x, int y, int x1, int y1, int x2, int y2);
void Copy_No_Refresh(GLuint Source, int x, int y, int x1, int y1, int x2, int y2, int remainder);
void Copy_To_Surface(PTK_TEXTURE *source, PTK_TEXTURE *dest, int dst_y, int src_y, int width, int height);
void Print_String(int x, int y, int Font_Type, char *String, int max_x = -1);
void Enter_2D_Mode(float Width, float Height);
void Leave_2d_Mode(void);
PTK_TEXTURE *Create_Texture(int width, int height);
void Destroy_Texture(PTK_TEXTURE *texture);
PTK_TEXTURE *Load_Picture(char *FileName);
int Lock_Texture(PTK_TEXTURE *texture);
void Unlock_Texture(PTK_TEXTURE *texture);
void Free_Palette(void);

#endif
