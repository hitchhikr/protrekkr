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

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

// ------------------------------------------------------
// Includes
#include <SDL/SDL.h>
#if defined(__WIN32__)
#include <SDL/SDL_syswm.h>
#endif

#include "../../include/version.h"

// ------------------------------------------------------
// Constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 8

#define MOUSE_LEFT_BUTTON 1
#define MOUSE_MIDDLE_BUTTON 2
#define MOUSE_RIGHT_BUTTON 4

#define SDL_MOUSE_LEFT_BUTTON 1
#define SDL_MOUSE_MIDDLE_BUTTON 2
#define SDL_MOUSE_RIGHT_BUTTON 3

#define MAX_EVENTS 16

#define SDL_GO_FULLSCREEN (SDL_USEREVENT + 1)

// ------------------------------------------------------
// Structure
typedef struct
{
    int x;
    int y;
    int button;
    int button_oneshot;
    int wheel;
} MOUSE, *LPMOUSE;

// ------------------------------------------------------
// Variables
extern unsigned short Keys[SDLK_LAST];
extern unsigned short Keys_Sym[SDLK_LAST];
extern int Key_Unicode;
extern unsigned short Keys_Raw[65535];
extern unsigned short Keys_Raw_Off[65535];
extern unsigned short Keys_Raw_Repeat[65535];
extern unsigned short Keys_Unicode[65535];
extern int Keyboard_Nbr_Events;
extern int Keyboard_Events[256];
extern int Keyboard_Notes_Type[256];
extern int Keyboard_Notes_Bound[256];
extern int Env_Change;
extern int key_on;
extern char Keyboard_Name[];

// ------------------------------------------------------
// Functions
void Run_Interface(void);
void Message_Error(char *Message);
int Switch_FullScreen(int Width, int Height, int Refresh, int Force_Window_Mode);
int Get_LShift(void);
int Get_RShift(void);
int Get_Caps(void);
int Get_LAlt(void);
int Get_RAlt(void);
int Get_LCtrl(void);
int Get_RCtrl(void);
int Redraw_Screen(void);

#endif
