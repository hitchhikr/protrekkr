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

#ifndef _REQUESTERS_H_
#define _REQUESTERS_H_

// ------------------------------------------------------
// Constants
#define BUTTON_DEFAULT 1
#define BUTTON_CANCEL 2

// ------------------------------------------------------
// Structures
typedef struct
{
    void *Next;
    char *Text;
    int Type;
    int Key;
} REQUESTER_BUTTON, *LPREQUESTER_BUTTON;

typedef struct
{
    char *Text;
    LPREQUESTER_BUTTON Buttons;
    SDL_Surface **Picture;
    int TimeOut;
} REQUESTER, *LPREQUESTER;

// ------------------------------------------------------
// Variables
extern int In_Requester;
extern int Requester_Action;
extern int Req_Timer;
extern int Req_TimeOut;
extern int Req_Pressed_Button;
extern int Req_Default_Button;
extern LPREQUESTER Current_Requester;

// ------------------------------------------------------
// Functions
void Mouse_Handler_Requester(void);
void Keyboard_Handler_Requester(void);
int Display_Requester(LPREQUESTER Requester, int Action, char *Text, int Center);
int Check_Requester(LPREQUESTER Requester);

#endif
