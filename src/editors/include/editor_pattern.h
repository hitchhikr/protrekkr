// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2014 Franck Charlet.
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

#ifndef _EDITOR_PATTERN_H_
#define _EDITOR_PATTERN_H_

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
#define PAT_COL_SHIFT 4
#define PAT_COL_NOTE 23

#define PAT_COL_CHAR 8
#define PAT_COL_CHAR_LARGE 11
#define PAT_COL_CHAR_SMALL 6

extern int DISPLAYED_LINES;

extern int MAX_PATT_SCREEN_X;

// ------------------------------------------------------
// Enumerations
enum TRACK_TYPE
{
    TRACK_SMALL,
    TRACK_MEDIUM,
    TRACK_LARGE
};

// ------------------------------------------------------
// Structures
typedef struct
{
    void (*Fnc)(int, int, char, int, int);
} LETTER_FUNCTION, *LPLETTER_FUNCTION;

typedef struct
{
    void (*Fnc)(int, int, int, int, int);
    void (*Fnc_Lo)(int, int, char, int, int);
} NOTE_FUNCTION, *LPNOTE_FUNCTION;

// ------------------------------------------------------
// Variables
extern char Large_Patterns;
extern int Patterns_Lines_Offset;

// ------------------------------------------------------
// Functions
void draw_pated(int track, int line, int petrack, int row);
void Actupated(int modac);
void draw_pated_highlight(int track, int line, int petrack, int row, int ypos);
void Actualize_Patterned(void);
void Bound_Patt_Pos(void);
void Reset_Pattern_Scrolling_Horiz(void);
void Mouse_Wheel_Pattern_Ed(int roll_amount, int allow);
void Mouse_Sliders_Pattern_Ed(void);
void Mouse_Sliders_Right_Pattern_Ed(void);
void Mouse_Left_Pattern_Ed(void);
void Mouse_Right_Pattern_Ed(void);
int Get_Visible_Tracks_Size(int max_tracks);
int Get_Visible_Partial_Tracks(void);
int Get_Song_Position(void);
int Get_Pattern_Line(void);
int Get_Song_Line(void);
void Goto_Previous_Row(void);
void Goto_Next_Row(void);
void Goto_Previous_Page(void);
void Goto_Next_Page(void);
void Goto_Previous_Column(void);
void Goto_Next_Column(void);
void Goto_Top_Left(void);
void Goto_Bottom_Right(void);
void Goto_Row(int row);
void Goto_Previous_Position(void);
void Goto_Song_Position(int Position);
int Get_Track_Size(int Track, int *Column);
int Get_Track_Real_Size(int Track);
int Get_Last_Track_Column(int track);
int Get_Visible_Complete_Tracks(void);
int Get_Pattern_Offset(int track, int row, int pattern);
void Reset_Patterns_Zoom(void);
void Toggle_Track_Zoom(int track, int large);
void Clear_Track_Zoom(int track);
TRACK_TYPE Get_Track_Zoom(int track);
void Set_Track_Zoom(int track, TRACK_TYPE type);
void Draw_Pattern_Right_Stuff();
void Set_Pattern_Size();
void Draw_Blocks_Buffers_Status();

#endif
