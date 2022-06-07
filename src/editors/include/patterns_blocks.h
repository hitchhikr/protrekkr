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

#ifndef _PATTERNS_BLOCKS_H_
#define _PATTERNS_BLOCKS_H_

// ------------------------------------------------------
// Includes
#if defined(__AROS__)
#include <string.h>
#endif

// ------------------------------------------------------
// Constants
#define NBR_COPY_BLOCKS 5
#define EXTRA_NIBBLE_DAT 4

// ------------------------------------------------------
// Enumerations
enum COLUMN_TYPE
{
    NOTE,
    INSTRHI,
    INSTRLO,
    VOLUMEHI,
    VOLUMELO,
    PANNINGHI,
    PANNINGLO,
    EFFECTHI,
    EFFECTLO,
    EFFECTDATHI,
    EFFECTDATLO,
    EFFECT2HI,
    EFFECT2LO,
    EFFECT2DATHI,
    EFFECT2DATLO,
    EFFECT3HI,
    EFFECT3LO,
    EFFECT3DATHI,
    EFFECT3DATLO,
    EFFECT4HI,
    EFFECT4LO,
    EFFECT4DATHI,
    EFFECT4DATLO
};

// ------------------------------------------------------
// Structures
typedef struct
{
    int x_start;
    int y_start;
    int x_end;
    int y_end;
} SELECTION, *LPSELECTION;

// ------------------------------------------------------
// Variables
extern int Curr_Buff_Block;
extern int Buff_Full[NBR_COPY_BLOCKS];
extern int b_buff_xsize[NBR_COPY_BLOCKS];
extern int b_buff_ysize[NBR_COPY_BLOCKS];
extern int start_buff_nibble[NBR_COPY_BLOCKS];
extern unsigned char *BuffBlock[NBR_COPY_BLOCKS];
extern int block_start_track_nibble[NBR_COPY_BLOCKS];
extern int block_end_track_nibble[NBR_COPY_BLOCKS];
extern int block_start_track[NBR_COPY_BLOCKS];
extern int block_end_track[NBR_COPY_BLOCKS];
extern int block_start[NBR_COPY_BLOCKS];
extern int block_end[NBR_COPY_BLOCKS];
extern char Buff_MultiNotes[NBR_COPY_BLOCKS][MAX_TRACKS];
extern char Buff_Effects[NBR_COPY_BLOCKS][MAX_TRACKS];

// ------------------------------------------------------
// Functions
int Init_Block_Work(void);
void Mark_Block_Start(int start_nibble, int start_track, int start_line);
void Mark_Block_End(int end_nibble, int start_track, int start_line, int Modif);
int Delete_Selection(int Position);
void Copy_Selection_To_Buffer(int Position);
void Paste_Selection_From_Buffer(int Position, int Go_Across);
void Cut_Selection(int Position);
void Copy_Selection(int Position);
void Copy_Buff(int dst, int src);
SELECTION Get_Real_Selection(int Default);
SELECTION Select_Track(int Track);
void Paste_Block(int Position, int Go_Across, int Refresh);
void Interpolate_Block(int Position);
void Fill_Block(int Position, int step);
void Randomize_Block(int Position, int step);
void Semitone_Up_Block(int Position);
void Semitone_Down_Block(int Position);
void Instrument_Semitone_Up_Block(int Position);
void Instrument_Semitone_Up_Sel(int Position, SELECTION Sel, int Amount, int Instr);
void Instrument_Semitone_Down_Block(int Position);
void Instrument_Semitone_Down_Sel(int Position, SELECTION Sel, int Amount, int Instr);
void Octave_Up_Block(int Position);
void Octave_Down_Block(int Position);
void Instrument_Octave_Up_Block(int Position);
void Instrument_Octave_Down_Block(int Position);
void Instrument_Remap_Sel(int Position, SELECTION Sel, int From, int To);
void Select_Track_Block(void);
void Select_Pattern_Block(void);
void Select_Note_Block(void);
void Calc_selection(void);
void Unselect_Selection(void);
void Select_Block_Keyboard(int Type);
void Insert_Pattern_Line(int Position);
void Insert_Track_Line(int track, int Position);
void Remove_Pattern_Line(int Position);
void Remove_Track_Line(int track, int Position);
void Clear_Patterns_Pool(void);
void Clear_Track_Data(int offset);
int Alloc_Patterns_Pool(void);
int Get_Max_Nibble_Track(char *Buffer_MultiNotes, char *Buffer_Effects, int track);
int Get_Track_Nibble_Start(char *Buffer_MultiNotes, char *Buffer_Effects, int track);
int Get_Track_From_Nibble(char *Buffer_MultiNotes, char *Buffer_Effects, int nibble);
void Select_All_Notes_Block(void);
int Get_Max_Nibble_Track_From_Nibble(char *Buffer_MultiNotes, char *Buffer_Effects, int nibble);
int Get_Max_Nibble_Effects_From_Nibble(char *Buffer_MultiNotes, char *Buffer_Effects, int nibble);
int Get_Byte_From_Column(char *Buffer_MultiNotes, char *Buffer_Effects, int column);
COLUMN_TYPE Get_Column_Type(char *Buffer_MultiNotes, char *Buffer_Effects, int column);
int Get_Track_Relative_Column(char *Buffer_MultiNotes, char *Buffer_Effects, int column);
int Get_Max_Nibble_All_Tracks(void);
void Clear_Buff(int Idx);
int Are_Columns_Compatible(int type_src, int type_dst);
void Delete_Track(void);
void Insert_Track(void);
void Reset_Track(int Position, int Track);

#endif
