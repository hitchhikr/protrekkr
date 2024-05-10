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

// ------------------------------------------------------
// Includes
#include "include/editor_pattern.h"
#include "include/editor_diskio.h"
#include "include/patterns_blocks.h"
#include "../support/include/timer.h"
#include "include/editor_instrument.h"
#include "include/editor_sequencer.h"

// ------------------------------------------------------
// Constants
#define POS_HORIZ_SLIDER 716

// ------------------------------------------------------
// Variables
int VIEWLINE;
char is_editing = 0;
char is_recording = 0;
char is_recording_2 = 0;
char is_record_key = 0;

// Min 1 Max 16
int patt_highlight = 4;
int Continuous_Scroll;
int Last_Pixel;
int Last_Pixel_Complete;

int DISPLAYED_LINES;

int MAX_PATT_SCREEN_X;

MARKER Patterns_Marker;

extern char Use_Shadows;

char Global_Patterns_Font = TRACK_MEDIUM;

int Current_Edit_Steps = 1;
int Current_Octave = 4;

int Real_Displayed_Tracks;

char sr_isrecording = 0;

int color_1b[11];
int color_2b[11];

int shadow_tracks[MAX_TRACKS];

int pos_effects_icons[MAX_TRACKS];

char table_decimal[] =
{
    20, 20, 0,
    20, 20, 1,
    20, 20, 2,
    20, 20, 3,
    20, 20, 4,
    20, 20, 5,
    20, 20, 6,
    20, 20, 7,
    20, 20, 8,
    20, 20, 9,

    20, 1, 0,
    20, 1, 1,
    20, 1, 2,
    20, 1, 3,
    20, 1, 4,
    20, 1, 5,
    20, 1, 6,
    20, 1, 7,
    20, 1, 8,
    20, 1, 9,

    20, 2, 0,
    20, 2, 1,
    20, 2, 2,
    20, 2, 3,
    20, 2, 4,
    20, 2, 5,
    20, 2, 6,
    20, 2, 7,
    20, 2, 8,
    20, 2, 9,

    20, 3, 0,
    20, 3, 1,
    20, 3, 2,
    20, 3, 3,
    20, 3, 4,
    20, 3, 5,
    20, 3, 6,
    20, 3, 7,
    20, 3, 8,
    20, 3, 9,

    20, 4, 0,
    20, 4, 1,
    20, 4, 2,
    20, 4, 3,
    20, 4, 4,
    20, 4, 5,
    20, 4, 6,
    20, 4, 7,
    20, 4, 8,
    20, 4, 9,

    20, 5, 0,
    20, 5, 1,
    20, 5, 2,
    20, 5, 3,
    20, 5, 4,
    20, 5, 5,
    20, 5, 6,
    20, 5, 7,
    20, 5, 8,
    20, 5, 9,

    20, 6, 0,
    20, 6, 1,
    20, 6, 2,
    20, 6, 3,
    20, 6, 4,
    20, 6, 5,
    20, 6, 6,
    20, 6, 7,
    20, 6, 8,
    20, 6, 9,

    20, 7, 0,
    20, 7, 1,
    20, 7, 2,
    20, 7, 3,
    20, 7, 4,
    20, 7, 5,
    20, 7, 6,
    20, 7, 7,
    20, 7, 8,
    20, 7, 9,

    20, 8, 0,
    20, 8, 1,
    20, 8, 2,
    20, 8, 3,
    20, 8, 4,
    20, 8, 5,
    20, 8, 6,
    20, 8, 7,
    20, 8, 8,
    20, 8, 9,

    20, 9, 0,
    20, 9, 1,
    20, 9, 2,
    20, 9, 3,
    20, 9, 4,
    20, 9, 5,
    20, 9, 6,
    20, 9, 7,
    20, 9, 8,
    20, 9, 9,

    1, 0, 0,
    1, 0, 1,
    1, 0, 2,
    1, 0, 3,
    1, 0, 4,
    1, 0, 5,
    1, 0, 6,
    1, 0, 7,
    1, 0, 8,
    1, 0, 9,

    1, 1, 0,
    1, 1, 1,
    1, 1, 2,
    1, 1, 3,
    1, 1, 4,
    1, 1, 5,
    1, 1, 6,
    1, 1, 7,
    1, 1, 8,
    1, 1, 9,

    1, 2, 0,
    1, 2, 1,
    1, 2, 2,
    1, 2, 3,
    1, 2, 4,
    1, 2, 5,
    1, 2, 6,
    1, 2, 7,
    1, 2, 8,
    1, 2, 9,
};

int Cur_Char_size[MAX_TRACKS];

LETTER_FUNCTION Cur_Char_Function[MAX_TRACKS];
NOTE_FUNCTION Cur_Note_Function[MAX_TRACKS];

PtkTimer Pattern_Timer_Horiz_Left;
PtkTimer Pattern_Timer_Horiz_Right;
PtkTimer Pattern_Timer_Horiz_Right_Slow;
int Pattern_Scrolling_Horiz_Left;
int Pattern_Scrolling_Horiz_Right;
int Pattern_Scrolling_Horiz_Right_Slow;
float Pattern_First_Delay_Horiz_Left;
float Pattern_First_Delay_Horiz_Right;
float Pattern_First_Delay_Horiz_Right_Slow;
float Pattern_Delay_Horiz_Left;
float Pattern_Delay_Horiz_Right;
float Pattern_Delay_Horiz_Right_Slow;

PtkTimer Pattern_Timer_Vert;
int Pattern_Scrolling_Vert;
float Pattern_First_Delay_Vert;
float Pattern_Delay_Vert;

PtkTimer Select_Timer_Vert;
int Select_Scrolling_Vert;
float Select_First_Delay_Vert;
float Select_Delay_Vert;

PtkTimer Select_Timer_Horz;
int Select_Scrolling_Horz;
float Select_First_Delay_Horz;
float Select_Delay_Horz;

char Large_Patterns;
int Patterns_Lines_Offset = 0;

// ------------------------------------------------------
// Functions
int Get_Nibble_Color(int In_Prev_Next, int row, int column, int multi, int Shadow);
int Get_Nibble_Color_Highlight(int row, int column);
void Display_Patt_Line(int In_Prev_Next, int Shadow_Pattern, int y, int rel, int track, int tVisible_Columns, int pattern);
void Solo_Track(int track_to_solo);

// ------------------------------------------------------
// Draw a pattern on screen
void draw_pated(int track, int line, int petrack, int row)
{
    int rel = 0;
    int liner;

    int Cur_Position;
    int Cur_Position2;
    int Shadow_Pattern;
    int pattern;
    int In_Prev_Next;
    int In_Prev_Next2;
    int rel2;
    int i;
    int rel2_size;
    int rel2_size2;
    int dover;
    int cur_track;
    int save_dover;
    int y;
    int idx_icons_fx;

    Cur_Position = Get_Song_Position();

    int tVisible_Columns = Visible_Columns;

    int max_size = Get_Visible_Tracks_Size((gui_track + Visible_Columns));
    if(max_size < MAX_PATT_SCREEN_X)
    {
        if((gui_track + tVisible_Columns) < Songtracks)
        {
            tVisible_Columns++;
        }
    }

    y = 184;

    // Clear headers line
    SetColor(COL_PATTERN_LO_BACK);
    bjbox(1, y, CONSOLE_WIDTH - 20, 12 + 8);

    dover = PAT_COL_NOTE;

    idx_icons_fx = 0;

    // Tracks headers
    for(liner = 0; liner < tVisible_Columns; liner++)
    {
        cur_track = track + liner;

        if(Track_Under_Caret == cur_track) SetColor(COL_PUSHED_MED);
        else SetColor(COL_PATTERN_LO_BACK);

        if(dover + 2 >= MAX_PATT_SCREEN_X) break;
        dover += 2;

        if(dover + 4 >= MAX_PATT_SCREEN_X) break;
        dover += 4;

        shadow_tracks[cur_track] = FALSE;
        if(!Chan_Active_State[Cur_Position][cur_track])
        {
            if(Song_Playing)
            {
                shadow_tracks[cur_track] = Use_Shadows;
            }
        }

        save_dover = dover;

        // Draw the track header

        // Channel number
        if((dover + (Cur_Char_size[cur_track] + 1)) >= MAX_PATT_SCREEN_X) goto Skip_Header;
        Cur_Char_Function[cur_track].Fnc(dover, y, cur_track, 71, 71 + 6);
        dover += Cur_Char_size[cur_track] + 1;

        // Mute on/off
        if((dover + 29) >= MAX_PATT_SCREEN_X) goto Skip_Header;
        if(Chan_Mute_State[cur_track]) Cur_Char_Function[cur_track].Fnc(dover, y, 25, 0, 0);
        else Cur_Char_Function[cur_track].Fnc(dover, y, 26, 0, 0);
        dover += 29;

        // Active on/off
        if((dover + 29) >= MAX_PATT_SCREEN_X) goto Skip_Header;
        if(Chan_Active_State[Cur_Position][cur_track]) Cur_Char_Function[cur_track].Fnc(dover, y, 23, 0, 0);
        else Cur_Char_Function[cur_track].Fnc(dover, y, 24, 0, 0);
        dover += 29;

        // Zoom on/off
        if((dover + 17) >= MAX_PATT_SCREEN_X) goto Skip_Header;
        if(Get_Track_Zoom(cur_track) != TRACK_MEDIUM) Cur_Char_Function[cur_track].Fnc(dover, y, 27, 0, 0);
        else Cur_Char_Function[cur_track].Fnc(dover, y, 28, 0, 0);
        dover += 17;

Skip_Header:

        dover = save_dover;

        if((dover + 1) >= MAX_PATT_SCREEN_X) goto Skip_Header2;
        dover += 1;

        // Reduce notes
        if((dover + 9) >= MAX_PATT_SCREEN_X) goto Skip_Header2;
        if(Channels_MultiNotes[cur_track] == 1) Cur_Char_Function[cur_track].Fnc(dover, y + 8, 37, 0, 0);
        else Cur_Char_Function[cur_track].Fnc(dover, y + 8, 35, 0, 0);
        dover += 9;

        // Expand notes
        if((dover + 20) >= MAX_PATT_SCREEN_X) goto Skip_Header2;
        if(Channels_MultiNotes[cur_track] == MAX_POLYPHONY) Cur_Char_Function[cur_track].Fnc(dover, y + 8, 38, 0, 0);
        else Cur_Char_Function[cur_track].Fnc(dover, y + 8, 36, 0, 0);
        dover += 20;

Skip_Header2:

        dover = save_dover;

        if((dover + 1) >= MAX_PATT_SCREEN_X) break;
        dover += 1;

        // Calculate the place holder for the tracks data
        for(i = 0; i < Channels_MultiNotes[cur_track]; i++)
        {
            // Note
            dover += Cur_Char_size[cur_track] * 3;
            if(dover >= MAX_PATT_SCREEN_X) break;

            // Gap
            dover += PAT_COL_SHIFT - 2;
            if(dover >= MAX_PATT_SCREEN_X) break;

            // Instrument
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
            
            // Gap
            dover += 2;
            if(dover >= MAX_PATT_SCREEN_X) break;
        }
        if(dover >= MAX_PATT_SCREEN_X) break;

        // Gap
        dover += PAT_COL_SHIFT;
        if(dover >= MAX_PATT_SCREEN_X) break;

        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;

        dover += PAT_COL_SHIFT;
        if(dover >= MAX_PATT_SCREEN_X) break;

        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;

        // Gap
        dover += PAT_COL_SHIFT;
        if(dover >= MAX_PATT_SCREEN_X) break;

        pos_effects_icons[idx_icons_fx] = dover;

        // Reduce fx
        if((dover + 9) >= MAX_PATT_SCREEN_X) break;
        if(Channels_Effects[cur_track] == 1) Cur_Char_Function[cur_track].Fnc(dover, y + 8, 37, 0, 0);
        else Cur_Char_Function[cur_track].Fnc(dover, y + 8, 35, 0, 0);
        dover += 9;

        // Expand fx
        if((dover + 9) >= MAX_PATT_SCREEN_X) break;
        if(Channels_Effects[cur_track] == MAX_FX) Cur_Char_Function[cur_track].Fnc(dover, y + 8, 38, 0, 0);
        else Cur_Char_Function[cur_track].Fnc(dover, y + 8, 36, 0, 0);
        dover += 9;

        dover = save_dover;

        // Calculate the place holder for the tracks data
        for(i = 0; i < Channels_MultiNotes[cur_track]; i++)
        {
            // Note
            dover += Cur_Char_size[cur_track] * 3;
            if(dover >= MAX_PATT_SCREEN_X) break;

            // Gap
            dover += PAT_COL_SHIFT - 2;
            if(dover >= MAX_PATT_SCREEN_X) break;

            // Instrument
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
            
            // Gap
            dover += 2;
            if(dover >= MAX_PATT_SCREEN_X) break;
        }
        if(dover >= MAX_PATT_SCREEN_X) break;

        // Gap
        dover += PAT_COL_SHIFT;
        if(dover >= MAX_PATT_SCREEN_X) break;

        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;

        dover += PAT_COL_SHIFT;
        if(dover >= MAX_PATT_SCREEN_X) break;

        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) break;

        // Gap
        dover += PAT_COL_SHIFT;
        if(dover >= MAX_PATT_SCREEN_X) break;
       
        for(i = 0; i < Channels_Effects[cur_track]; i++)
        {
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
            dover += Cur_Char_size[cur_track];
            if(dover >= MAX_PATT_SCREEN_X) break;
        }

        idx_icons_fx++;
    }

    y = 195 + 5;
    Cur_Position2 = Cur_Position;
    Shadow_Pattern = 0;
    In_Prev_Next = FALSE;
    In_Prev_Next2 = FALSE;

    SetColor(COL_PATTERN_LO_BACK);
    bjbox(1,
          y,
          CONSOLE_WIDTH - 20,
          (Cur_Height - 372) + Patterns_Lines_Offset
         );

    for(liner = -VIEWLINE; liner < VIEWLINE + (DISPLAYED_LINES % 2); liner++)
    {
        rel2 = 0;
        rel = liner + line;
        In_Prev_Next = In_Prev_Next2;

        if(See_Prev_Next_Pattern)
        {
            // Can't see past the first & last positions
            if(Cur_Position2)
            {
                if(rel <= -1)
                {
                    // See previous pattern
                    rel2 = patternLines[pSequence[Cur_Position2 - 1]] + rel;
                    if(rel2 < 0)
                    {
                        rel2_size = -rel;
                        rel2_size2 = rel2_size;
                        // Find the first line to display
                        if(rel2 < 0)
                        {
                            rel2 = 0;
                            while(rel2_size2 != 0)
                            {
                                Cur_Position2--;
                                rel2_size2 -= patternLines[pSequence[Cur_Position2]];
                                if(rel2_size2 < 0)
                                {
                                    rel2 += -rel2_size2;
                                    rel2_size2 = 0;
                                }
                                if(!Cur_Position2) break;
                            }
                            y += rel2_size2 * 8;
                            liner += rel2_size2;
                            rel2_size -= rel2_size2;
                        }

                        // Display the lines before our position
                        rel = rel2;
                        In_Prev_Next = TRUE;
                        for(i = 0; i < rel2_size; i++)
                        {
                            pattern = pSequence[Cur_Position2];
                            Display_Patt_Line(In_Prev_Next, Shadow_Pattern, y, rel, track, tVisible_Columns, pattern);
                            y += 8;
                            rel++;
                            if(rel >= patternLines[pSequence[Cur_Position2]])
                            {
                                Cur_Position2++;
                                if(Cur_Position == Cur_Position2)
                                {
                                    In_Prev_Next = FALSE;
                                }
                                rel = 0;
                            }
                            liner++;
                        }
                        liner--;
                        In_Prev_Next2 = FALSE;
                        In_Prev_Next = FALSE;
                        goto Disp;
                    }
                    rel = rel2;
                    In_Prev_Next = TRUE;
                    pattern = pSequence[Cur_Position2 - 1];
                    goto Go_Display;
                } 
            }
            if(Cur_Position2 < Song_Length - 1)
            {
                if(rel >= patternLines[pSequence[Cur_Position2]])
                {
                    // See next pattern
                    line -= patternLines[pSequence[Cur_Position2]];
                    rel = liner + line;
                    Cur_Position2++;
                    In_Prev_Next2 = TRUE;
                    In_Prev_Next = In_Prev_Next2;
                } 
            } 
        }

        if(rel > -1 && rel < patternLines[pSequence[Cur_Position2]])
        {
            pattern = pSequence[Cur_Position2];
Go_Display:
            // Display the highlight line at approx the middle of the pattern
            if(liner != 0)
            {
                Display_Patt_Line(In_Prev_Next, Shadow_Pattern, y, rel, track, tVisible_Columns, pattern);
            }
            else
            {
                draw_pated_highlight(gui_track, line, Track_Under_Caret, Column_Under_Caret, y);
                y += 8;
            }
        }
        y += 8;
Disp:;
    }
}

// ------------------------------------------------------
// Draw a pattern row
void Display_Patt_Line(int In_Prev_Next, int Shadow_Pattern,
                       int y, int rel,
                       int track, int tVisible_Columns,
                       int pattern)
{
    int cur_column;
    int offset_t;
    int cur_color;
    char *ptr_table_decimal;
    int high_color;
    char multip;
    int dover;
    int synchro_fx = FALSE;
    int Fx_Color;
    int tracky;
    int cur_track;
    int dover_break;
    int i;

    if(!In_Prev_Next) Shadow_Pattern = FALSE;
    else Shadow_Pattern = Use_Shadows;

    multip = FALSE;
    if(patt_highlight > 1)
    {
        if(rel % patt_highlight == 0) multip = TRUE;
    }

    cur_column = Get_Track_Nibble_Start(Channels_MultiNotes, Channels_Effects, track) + track;

    // Browse all tracks to seek synchro markers
    for(tracky = 0; tracky < Songtracks; tracky++)
    {
        offset_t = Get_Pattern_Offset(pattern, tracky, rel);
        unsigned char p_e_sync;

        for(i = 0; i < Channels_Effects[tracky]; i++)
        {
            p_e_sync = *(RawPatterns + offset_t + PATTERN_FX + (i * 2));
            if(p_e_sync == 0x7) synchro_fx = TRUE;
        }
    }
    
    dover = PAT_COL_NOTE;

    for(tracky = 0; tracky < tVisible_Columns; tracky++)
    {
        cur_track = track + tracky;

        // Read the datas
        offset_t = Get_Pattern_Offset(pattern, cur_track, rel);

        unsigned char p_a;
        unsigned char p_b;
        unsigned char p_bh;
        
        unsigned char p_c = *(RawPatterns + offset_t + PATTERN_VOLUME);
        unsigned char p_ch = p_c & 0xf;
        unsigned char p_d = *(RawPatterns + offset_t + PATTERN_PANNING);
        unsigned char p_dh = p_d & 0xf;
        
        unsigned char p_e[MAX_FX];
        unsigned char p_eh[MAX_FX];
        unsigned char p_f[MAX_FX];
        unsigned char p_fh[MAX_FX];
         
         for(i = 0 ; i < Channels_Effects[cur_track]; i++)
         {
            p_e[i] = *(RawPatterns + offset_t + PATTERN_FX + (i * 2));
            p_eh[i] = p_e[i] & 0xf;
            p_f[i] = *(RawPatterns + offset_t + PATTERN_FXDATA + (i * 2));
            p_fh[i] = p_f[i] & 0xf;
        }

        // Don't draw the 2 gaps if the next char can't be drawn
        if(dover + 6 + (Cur_Char_size[cur_track] * 3) >= MAX_PATT_SCREEN_X) break;
        cur_color = Get_Nibble_Color(In_Prev_Next, rel, cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
        Letter(dover, y, 20, cur_color, cur_color + 7);
        dover += 2;
        Letter(dover, y, 30, cur_color, cur_color + 7);
        dover += 4;

        // Notes/Instruments
        for(i = 0; i < Channels_MultiNotes[cur_track]; i++)
        {
            p_a = *(RawPatterns + offset_t + PATTERN_NOTE1 + (i * 2));
            p_b = *(RawPatterns + offset_t + PATTERN_INSTR1 + (i * 2));
            p_bh = p_b & 0xf;

            dover_break = dover + (Cur_Char_size[cur_track] * 3);
            if(dover_break >= MAX_PATT_SCREEN_X) break;
            Cur_Note_Function[cur_track].Fnc(dover, y, p_a, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track] * 3;

            // Don't draw the gap if the next char can't be drawn
            dover_break = (dover + PAT_COL_SHIFT - 2) + Cur_Char_size[cur_track];
            if(dover_break >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
            Cur_Char_Function[cur_track].Fnc(dover, y, 29, cur_color, cur_color + 7);
            dover += PAT_COL_SHIFT - 2;

            // Instrument
            dover_break = dover + Cur_Char_size[cur_track];
            if(dover_break >= MAX_PATT_SCREEN_X) break;
            if(p_b != 255)
            {
                if(p_b >> 4) Cur_Char_Function[cur_track].Fnc(dover, y, p_b >> 4, cur_color, cur_color + 7);
                else Cur_Char_Function[cur_track].Fnc(dover, y, 20, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
                
                dover_break = dover + Cur_Char_size[cur_track];
                if(dover_break >= MAX_PATT_SCREEN_X) break;
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, p_bh, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
            }
            else
            {
                Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];

                dover_break = dover + Cur_Char_size[cur_track];
                if(dover_break >= MAX_PATT_SCREEN_X) break;
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
            }

            // Don't draw the gap if the next char can't be drawn
            dover_break = dover + 2;
            // Is there at least 1 note after this ?
            // Then check if it can be drawn properly
            if(i < Channels_MultiNotes[cur_track] - 1) dover_break += (Cur_Char_size[cur_track] * 3);
            if(dover_break >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
            Cur_Char_Function[cur_track].Fnc(dover, y, 29, cur_color, cur_color + 7);
            dover += 2;
        }
        if(dover_break >= MAX_PATT_SCREEN_X) break;

        // Don't draw the gap if the next char can't be drawn
        if(dover + (PAT_COL_SHIFT + Cur_Char_size[cur_track]) >= MAX_PATT_SCREEN_X) break;
        Cur_Char_Function[cur_track].Fnc(dover, y, 29, cur_color, cur_color + 7);
        dover += PAT_COL_SHIFT;

        // Volume
        if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
        if(p_c != 255)
        {
            if(p_c >> 4) Cur_Char_Function[cur_track].Fnc(dover, y, p_c >> 4, cur_color, cur_color + 7);
            else Cur_Char_Function[cur_track].Fnc(dover, y, 20, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];

            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
            Cur_Char_Function[cur_track].Fnc(dover, y, p_ch, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];
        }
        else
        {
            Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];

            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
            Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];
        }

        // Don't draw the gap if the next char can't be drawn
        if(dover + (PAT_COL_SHIFT + Cur_Char_size[cur_track]) >= MAX_PATT_SCREEN_X) break;
        cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
        Cur_Char_Function[cur_track].Fnc(dover, y, 29, cur_color, cur_color + 7);
        dover += PAT_COL_SHIFT;

        // Panning
        if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
        if(p_d != 255)
        {
            if(p_d >> 4) Cur_Char_Function[cur_track].Fnc(dover, y, p_d >> 4, cur_color, cur_color + 7);
            else Cur_Char_Function[cur_track].Fnc(dover, y, 20, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];

            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
            Cur_Char_Function[cur_track].Fnc(dover, y, p_dh, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];
        }
        else
        {
            Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];

            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
            Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
            dover += Cur_Char_size[cur_track];
        }

        // Don't draw the gap if the next char can't be drawn
        if(dover + (PAT_COL_SHIFT + Cur_Char_size[cur_track]) >= MAX_PATT_SCREEN_X) break;
        
        // The first column of the effect is highlighted together with the gap.
        cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
        Cur_Char_Function[cur_track].Fnc(dover, y, 29, cur_color, cur_color + 7);
        dover += PAT_COL_SHIFT;

        // Effects
        int exit_tracks = FALSE;
        cur_column--;
        for(i = 0; i < Channels_Effects[cur_track]; i++)
        {
            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
            {
                exit_tracks = TRUE;
                break;
            }
            if(!p_e[i] && !p_f[i])
            {
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
        
                if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                {
                    exit_tracks = TRUE;
                    break;
                }
                // Second nibble
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
           
                if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                {
                    exit_tracks = TRUE;
                    break;
                }
                // Third nibble
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
        
                if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                {
                    exit_tracks = TRUE;
                    break;
                }
                // Fourth nibble
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, 21, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
            }
            else
            {
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, p_e[i] >> 4, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
        
                if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                {
                    exit_tracks = TRUE;
                    break;
                }
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, p_eh[i], cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
        
                if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                {
                    exit_tracks = TRUE;
                    break;
                }
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, p_f[i] >> 4, cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
        
                if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                {
                    exit_tracks = TRUE;
                    break;
                }
                cur_color = Get_Nibble_Color(In_Prev_Next, rel, ++cur_column, multip, Shadow_Pattern | shadow_tracks[cur_track]);
                Cur_Char_Function[cur_track].Fnc(dover, y, p_fh[i], cur_color, cur_color + 7);
                dover += Cur_Char_size[cur_track];
            }
        }
        //dover += Cur_Char_size[cur_track];
        if(exit_tracks) break;

        cur_column++;
    } // Track

    // Display the rows numbers
    if(Shadow_Pattern)
    {
        if(multip)
        {
            high_color = 71 + 87;
            Fx_Color = 32;
            SetColor(COL_PATTERN_LO_BACK_SHADOW);
        }
        else
        {
            high_color = 0 + 87;
            Fx_Color = 31;
            SetColor(COL_PATTERN_LO_BACK_SHADOW);
        }
    }
    else
    {
        if(multip)
        {
            high_color = 71;
            Fx_Color = 32;
            SetColor(COL_PATTERN_LO_BACK);
        }
        else
        {
            high_color = 0;
            Fx_Color = 31;
            SetColor(COL_PATTERN_LO_BACK);
        }
    }

    ptr_table_decimal = table_decimal + (rel * 3);

    // Display the row index
    if(Rows_Decimal)
    {
        Letter(1, y, ptr_table_decimal[0], high_color, high_color + 7);
        Letter(9, y, ptr_table_decimal[1], high_color, high_color + 7);
        Letter(17, y, ptr_table_decimal[2], high_color, high_color + 7);
    }
    else
    {
        Letter(1, y, 20, high_color, high_color + 7);
        Letter(9, y, rel >> 4, high_color, high_color + 7);
        Letter(17, y, rel & 0xf, high_color, high_color + 7);
    }

    // Display the synchro marker
    if(synchro_fx)
    {
        Letter(24, y, Fx_Color, 0, 0);
    }
}

// ------------------------------------------------------
// Record a FX on a track using various stratagems
void Record_Live_Fx(int Track,
                    int Pattern,
                    int Line,
                    int Cmd,
                    int Data,
                    int Free)
{
    int i;
    int Cur_Track;
    int offset_t;
    int Track_Tried;

    Track_Tried = 0;
    Cur_Track = Track;

Write_Fx:

    offset_t = Get_Pattern_Offset(Pattern, Cur_Track, Line);
    for(i = 0; i < Channels_Effects[Cur_Track]; i++)
    {
        // A fx similar is already there: let's replace it with a new value
        if(*(RawPatterns + offset_t + PATTERN_FX + (i * 2)) == Cmd)
        {
            *(RawPatterns + offset_t + PATTERN_FXDATA + (i * 2)) = Data;
            return;
        }
        // Place it on the first empty column otherwise
        if(*(RawPatterns + offset_t + PATTERN_FX + (i * 2)) == 0 &&
           *(RawPatterns + offset_t + PATTERN_FXDATA + (i * 2)) == 0)
        {
            *(RawPatterns + offset_t + PATTERN_FX + (i * 2)) = Cmd;
            *(RawPatterns + offset_t + PATTERN_FXDATA + (i * 2)) = Data;
            return;
        }
    }
    // No free slot have been found on the row
    if(Free)
    {
        // 303 units aren't bounded to any track so we can try another one for those
        Cur_Track++;
        if(Cur_Track >= Songtracks) Cur_Track = 0;
        // Try with the current amount of tracks
        Track_Tried++;
        if(Track_Tried < Songtracks) goto Write_Fx;
        // Back to where we were
        Cur_Track = Track;
    }
    // We couldn't find any suitable slot
    // We'll try to increase the number of effects if possible
    if(Channels_Effects[Cur_Track] < MAX_FX)
    {
        Channels_Effects[Cur_Track]++;
        *(RawPatterns + offset_t + PATTERN_FX + ((Channels_Effects[Cur_Track] - 1) * 2)) = Cmd;
        *(RawPatterns + offset_t + PATTERN_FXDATA + ((Channels_Effects[Cur_Track] - 1) * 2)) = Data;
        return;
    }
    // We couldn't record the command
}

// ------------------------------------------------------
// Draw the caret pattern row
void draw_pated_highlight(int track, int line, int petrack, int row, int ypos)
{
    int offset_t;
    int dover;
    int cur_column;
    int cur_color;
    char *ptr_table_decimal;
    int pattern;
    int Cur_Position;
    int synchro_fx = FALSE;
    int tracky;
    int high_col;
    int i;
    int dover_break;
    int Real_visible;

    int tVisible_Columns = Visible_Columns;

    int max_size = Get_Visible_Tracks_Size((gui_track + Visible_Columns));
    if(max_size < MAX_PATT_SCREEN_X)
    {
        if((gui_track + tVisible_Columns) < Songtracks)
        {
            tVisible_Columns++;
        }
    }

    Cur_Position = Get_Song_Position();

    pattern = pSequence[Cur_Position];

    if(RawPatterns)
    {
        if(tVisible_Columns > Songtracks) tVisible_Columns = Songtracks;

        // Browse all tracks to seek synchro markers
        for(tracky = 0; tracky < Songtracks; tracky++)
        {
            offset_t = Get_Pattern_Offset(pattern, tracky, line);
            unsigned char p_e_sync;

            for(i = 0; i < Channels_Effects[tracky]; i++)
            {
                p_e_sync = *(RawPatterns + offset_t + PATTERN_FX + (i * 2));
                if(p_e_sync == 0x7) synchro_fx = TRUE;
            }
        }

        SetColor(COL_PATTERN_LO_BACK);
        bjbox(1, ypos, CHANNELS_WIDTH - 1, 16);

        dover = PAT_COL_NOTE;
        Last_Pixel = dover;
        Last_Pixel_Complete = dover;
        cur_column = Get_Track_Nibble_Start(Channels_MultiNotes, Channels_Effects, track) + track;

        Real_visible = 0;
        
        for(tracky = 0; tracky < tVisible_Columns; tracky++)
        {
            int cur_track = track + tracky;

            Real_visible++;
            offset_t = Get_Pattern_Offset(pattern, cur_track, line);

            unsigned char p_a;
            unsigned char p_b;
            unsigned char p_bh;

            unsigned char p_c = *(RawPatterns + offset_t + PATTERN_VOLUME);
            unsigned char p_ch = p_c & 0xf;
            unsigned char p_d = *(RawPatterns + offset_t + PATTERN_PANNING);
            unsigned char p_dh = p_d & 0xf;
            
            unsigned char p_e[MAX_FX];
            unsigned char p_eh[MAX_FX];
            unsigned char p_f[MAX_FX];
            unsigned char p_fh[MAX_FX];
         
            for(i = 0 ; i < Channels_Effects[cur_track]; i++)
            {
                p_e[i] = *(RawPatterns + offset_t + PATTERN_FX + (i * 2));
                p_eh[i] = p_e[i] & 0xf;
                p_f[i] = *(RawPatterns + offset_t + PATTERN_FXDATA + (i * 2));
                p_fh[i] = p_f[i] & 0xf;
            }

            // Don't draw the gap if there's nothing to draw after that
            if(dover + 2 + 4 + (Cur_Char_size[cur_track] * 3) >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color_Highlight(line, cur_column);
            Letter(dover, ypos, 20, cur_color, cur_color + 15);
            dover += 2;
            Last_Pixel = dover;

            if(dover + 4 >= MAX_PATT_SCREEN_X) break;
            Letter(dover, ypos, 30, cur_color, cur_color + 15);
            dover += 4;
            Last_Pixel = dover;

            high_col = 0;

            // Notes/Instruments
            for(i = 0; i < Channels_MultiNotes[cur_track]; i++)
            {
                p_a = *(RawPatterns + offset_t + PATTERN_NOTE1 + (i * 2));
                p_b = *(RawPatterns + offset_t + PATTERN_INSTR1 + (i * 2));
                p_bh = p_b & 0xf;

                // Note
                dover_break = dover + (Cur_Char_size[cur_track] * 3);
                Last_Pixel = dover;
                if(dover_break >= MAX_PATT_SCREEN_X) break;
                if(row == high_col && cur_track == petrack) Cur_Note_Function[cur_track].Fnc(dover, ypos, p_a, 48, 48 + 15);
                else Cur_Note_Function[cur_track].Fnc(dover, ypos, p_a, cur_color, cur_color + 15);
                dover += Cur_Char_size[cur_track] * 3;
                Last_Pixel = dover;
                high_col++;

                // Instrument hi
                dover_break = (dover + PAT_COL_SHIFT - 2) + Cur_Char_size[cur_track];
                if(dover_break >= MAX_PATT_SCREEN_X) break;
                cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                Cur_Char_Function[cur_track].Fnc(dover, ypos, 29, cur_color, cur_color + 15);
                dover += PAT_COL_SHIFT - 2;
                Last_Pixel = dover;
                
                dover_break = dover + Cur_Char_size[cur_track];
                if(dover_break >= MAX_PATT_SCREEN_X) break;
                if(row == high_col && cur_track == petrack)
                {
                    if(p_b != 255)
                    {
                        if(p_b >> 4) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_b >> 4, 48, 48 + 15);
                        else Cur_Char_Function[cur_track].Fnc(dover, ypos, 20, 48, 48 + 15);
                    }
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                }
                else
                {
                    if(p_b != 255)
                    {
                        if(p_b >> 4) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_b >> 4, cur_color, cur_color + 15);
                        else Cur_Char_Function[cur_track].Fnc(dover, ypos, 20, cur_color, cur_color + 15);
                    }
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                }
                dover += Cur_Char_size[cur_track];
                Last_Pixel = dover;
                high_col++;

                // Instrument lo
                dover_break = dover + Cur_Char_size[cur_track];
                if(dover_break >= MAX_PATT_SCREEN_X) break;
                cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                if(row == high_col && cur_track == petrack)
                {
                    if(p_b != 255) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_bh, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                }
                else
                {
                    if(p_b != 255) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_bh, cur_color, cur_color + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                }
                dover += Cur_Char_size[cur_track];
                Last_Pixel = dover;

                // More notes to draw ?
                dover_break = dover + 2;
                if(i < Channels_MultiNotes[cur_track] - 1) dover_break += (Cur_Char_size[cur_track] * 3);
                if(dover_break >= MAX_PATT_SCREEN_X) break;
                cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                Cur_Char_Function[cur_track].Fnc(dover, ypos, 29, cur_color, cur_color + 15);
                dover += 2;
                Last_Pixel = dover;

                high_col++;
            }
            if(dover_break >= MAX_PATT_SCREEN_X) break;

            // Volume hi
            if(dover + PAT_COL_SHIFT + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            Cur_Char_Function[cur_track].Fnc(dover, ypos, 29, cur_color, cur_color + 15);
            dover += PAT_COL_SHIFT;
            Last_Pixel = dover;

            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            if(row == high_col && cur_track == petrack)
            {
                if(p_c != 255)
                {
                    if(p_c >> 4) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_c >> 4, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 20, 48, 48 + 15);
                }
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                dover += Cur_Char_size[cur_track];
            }
            else
            {
                if(p_c != 255)
                {
                    if(p_c >> 4) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_c >> 4, cur_color, cur_color + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 20, cur_color, cur_color + 15);
                }
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                dover += Cur_Char_size[cur_track];
            }
            Last_Pixel = dover;
            high_col++;

            // Volume lo
            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
            if(row == high_col && cur_track == petrack)
            {
                if(p_c != 255) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_ch, 48, 48 + 15);
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                dover += Cur_Char_size[cur_track];
            }
            else
            {
                if(p_c != 255) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_ch, cur_color, cur_color + 15);
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                dover += Cur_Char_size[cur_track];
            }
            Last_Pixel = dover;
            high_col++;

            // Panning hi
            if(dover + PAT_COL_SHIFT + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
            Cur_Char_Function[cur_track].Fnc(dover, ypos, 29, cur_color, cur_color + 15);
            dover += PAT_COL_SHIFT;
            Last_Pixel = dover;

            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            if(row == high_col && cur_track == petrack)
            {
                if(p_d != 255)
                {
                    if(p_d >> 4) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_d >> 4, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 20, 48, 48 + 15);
                }
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                dover += Cur_Char_size[cur_track];
            }
            else
            {
                if(p_d != 255)
                {
                    if(p_d >> 4) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_d >> 4, cur_color, cur_color + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 20, cur_color, cur_color + 15);
                }
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                dover += Cur_Char_size[cur_track];
            }
            Last_Pixel = dover;
            high_col++;

            // Panning lo
            if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
            if(row == high_col && cur_track == petrack)
            {
                if(p_d != 255) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_dh, 48, 48 + 15);
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                dover += Cur_Char_size[cur_track];
            }
            else
            {
                if(p_d != 255) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_dh, cur_color, cur_color + 15);
                else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                dover += Cur_Char_size[cur_track];
            }
            Last_Pixel = dover;
            high_col++;

            if(dover + PAT_COL_SHIFT + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X) break;
            cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
            Cur_Char_Function[cur_track].Fnc(dover, ypos, 29, cur_color, cur_color + 15);
            dover += PAT_COL_SHIFT;
            Last_Pixel = dover;
 
            //  Effects
            int exit_tracks = FALSE;
            cur_column--;
            for(i = 0 ; i < Channels_Effects[cur_track]; i++)
            {
                if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                {
                    exit_tracks = TRUE;
                    break;
                }
                if(!p_e[i] && !p_f[i])
                {
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;

                    //  Effect lo
                    if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                    {
                        exit_tracks = TRUE;
                        break;
                    }
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;

                    //  Effect data hi
                    if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                    {
                        exit_tracks = TRUE;
                        break;
                    }
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if((p_e[i] >> 4) == 0) if(p_eh[i] == 0x07) synchro_fx = TRUE;
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;

                    //  Effect data lo
                    if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                    {
                        exit_tracks = TRUE;
                        break;
                    }
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, 21, cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;
                }
                else
                {
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_e[i] >> 4, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, p_e[i] >> 4, cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;

                    //  Effect lo
                    if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                    {
                        exit_tracks = TRUE;
                        break;
                    }
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_eh[i], 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, p_eh[i], cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;

                    //  Effect data hi
                    if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                    {
                        exit_tracks = TRUE;
                        break;
                    }
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if((p_e[i] >> 4) == 0) if(p_eh[i] == 0x07) synchro_fx = TRUE;
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_f[i] >> 4, 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, p_f[i] >> 4, cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;

                    //  Effect data lo
                    if(dover + Cur_Char_size[cur_track] >= MAX_PATT_SCREEN_X)
                    {
                        exit_tracks = TRUE;
                        break;
                    }
                    cur_color = Get_Nibble_Color_Highlight(line, ++cur_column);
                    if(row == high_col && cur_track == petrack) Cur_Char_Function[cur_track].Fnc(dover, ypos, p_fh[i], 48, 48 + 15);
                    else Cur_Char_Function[cur_track].Fnc(dover, ypos, p_fh[i], cur_color, cur_color + 15);
                    dover += Cur_Char_size[cur_track];
                    Last_Pixel = dover;
                    high_col++;
                }
            }
            //dover += Cur_Char_size[cur_track];

            Last_Pixel = dover;
            Last_Pixel_Complete = dover;
            high_col++;
            ++cur_column;

            // ------------------------------------------------
            // Record live events
            if(sr_isrecording)
            {
                if(liveparam > 0 && Track_Under_Caret == cur_track && Song_Playing)
                {
                    if(livevalue < 0) livevalue = 0;
                    if(livevalue > 255) livevalue = 255;

                    switch(liveparam)
                    {
                        case LIVE_PARAM_SONG_VOLUME:
                            Record_Live_Fx(cur_track, pattern, line, 0x1c, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_SONG_BPM:
                            Record_Live_Fx(cur_track, pattern, line, 0xf0, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_TRACK_CUTOFF:
                            Record_Live_Fx(cur_track, pattern, line, 8, livevalue, FALSE);
                            break;
                        case LIVE_PARAM_TRACK_RESONANCE:
                            Record_Live_Fx(cur_track, pattern, line, 0x14, livevalue, FALSE);
                            break;
                        case LIVE_PARAM_TRACK_REVERB_SEND:
                            Record_Live_Fx(cur_track, pattern, line, 0x11, livevalue, FALSE);
                            break;
                        case LIVE_PARAM_TRACK_THRESHOLD:
                            Record_Live_Fx(cur_track, pattern, line, 0x12, livevalue, FALSE);
                            break;
                        case LIVE_PARAM_TRACK_CLAMP:
                            Record_Live_Fx(cur_track, pattern, line, 0x13, livevalue, FALSE);
                            break;
                        case LIVE_PARAM_TRACK_LFO_CARRIER:
                            Record_Live_Fx(cur_track, pattern, line, 0x16, livevalue, FALSE);
                            break;
                        case LIVE_PARAM_TRACK_PANNING:
                            if(livevalue > 0x80) livevalue = 0x80;
                            *(RawPatterns + offset_t + PATTERN_PANNING) = livevalue;
                            break;
                        case LIVE_PARAM_TRACK_VOLUME:
                            *(RawPatterns + offset_t + PATTERN_VOLUME) = livevalue;
                            break;
                        case LIVE_PARAM_303_1_CUTOFF:
                            Record_Live_Fx(cur_track, pattern, line, 0x33, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_2_CUTOFF:
                            Record_Live_Fx(cur_track, pattern, line, 0x34, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_1_RESONANCE:
                            Record_Live_Fx(cur_track, pattern, line, 0x35, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_2_RESONANCE:
                            Record_Live_Fx(cur_track, pattern, line, 0x36, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_1_ENVMOD:
                            Record_Live_Fx(cur_track, pattern, line, 0x37, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_2_ENVMOD:
                            Record_Live_Fx(cur_track, pattern, line, 0x38, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_1_DECAY:
                            Record_Live_Fx(cur_track, pattern, line, 0x39, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_2_DECAY:
                            Record_Live_Fx(cur_track, pattern, line, 0x3a, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_1_ACCENT:
                            Record_Live_Fx(cur_track, pattern, line, 0x3b, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_2_ACCENT:
                            Record_Live_Fx(cur_track, pattern, line, 0x3c, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_1_TUNE:
                            Record_Live_Fx(cur_track, pattern, line, 0x3d, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_2_TUNE:
                            Record_Live_Fx(cur_track, pattern, line, 0x3e, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_1_VOLUME:
                            Record_Live_Fx(cur_track, pattern, line, 0x41, livevalue, TRUE);
                            break;
                        case LIVE_PARAM_303_2_VOLUME:
                            Record_Live_Fx(cur_track, pattern, line, 0x42, livevalue, TRUE);
                            break;
                    } // Close switch

                    liveparam = 0;
                } // Close liveparam updated
            } // Close is recording
            // ------------------------------------------------

        }

        Visible_Columns = Real_visible;

        // Row number
        ptr_table_decimal = table_decimal + (line * 3);
        if(Rows_Decimal)
        {
            Letter(1, ypos, ptr_table_decimal[0], 16, 16 + 15);
            Letter(9, ypos, ptr_table_decimal[1], 16, 16 + 15);
            Letter(17, ypos, ptr_table_decimal[2], 16, 16 + 15);
        }
        else
        {
            Letter(1, ypos, 20, 16, 16 + 15);
            Letter(9, ypos, line >> 4, 16, 16 + 15);
            Letter(17, ypos, line & 0xf, 16, 16 + 15);
        }

        // Synchro marker
        if(synchro_fx)
        {
            Letter(24, ypos + 4, 33, 0, 0);
        }
    }
}

// ------------------------------------------------------
// Update the displayed pattern data
void Actupated(int modac)
{
    int nlines;
    int i;
    int Cur_Position;
    int Cur_Line;
    int Max_Lines_Song;

    // Buffers blocks
    if(is_editing > 1)
    {
        is_editing = 0;
        Notify_Edit();
    }

    if(!Song_Playing)
    {
        Pattern_Line_Visual = Pattern_Line;
    }

    Cur_Position = Get_Song_Position();
    nlines = patternLines[pSequence[Cur_Position]];

    if(Cur_Position != 0 || Cur_Position < (Song_Length - 1))
    {
        if(Continuous_Scroll)
        {
            if(Pattern_Line < 0)
            {
                nlines = patternLines[pSequence[Cur_Position - 1]];
                Pattern_Line = nlines + Pattern_Line;
                Unselect_Selection();
                gui_action = GUI_CMD_PREVIOUS_POSITION;
                return;
            }
            else
            {
                if(Pattern_Line >= nlines)
                {
                    Pattern_Line -= nlines;
                    Unselect_Selection();
                    gui_action = GUI_CMD_NEXT_POSITION;
                    return;
                }
            }
        }
        else
        {
            if(Pattern_Line < 0) Pattern_Line += nlines;
            if(Pattern_Line >= nlines) Pattern_Line -= nlines;
            if(Pattern_Line >= nlines) Pattern_Line = nlines - 1;
        }
    }
    else
    {
        if(Pattern_Line < 0) Pattern_Line += nlines;
        if(Pattern_Line >= nlines) Pattern_Line -= nlines;
        if(Pattern_Line >= nlines) Pattern_Line = nlines - 1;
    }

    int max_channel_dat = Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, Track_Under_Caret) - 1;
    if(Column_Under_Caret > max_channel_dat)
    {
        if(Track_Under_Caret < Songtracks - 1)
        {
            Column_Under_Caret = 0;
            Track_Under_Caret++;
        }
        else
        {
            Column_Under_Caret = 0;
            Track_Under_Caret = 0;
        }
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }
    if(Column_Under_Caret < 0)
    {
        Track_Under_Caret--;
        if(Track_Under_Caret < 0)
        {
            max_channel_dat = Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, Songtracks - 1) - 1;
        }
        else
        {
            max_channel_dat = Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, Track_Under_Caret) - 1;
        }
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
        Column_Under_Caret = max_channel_dat;
    }
    if(Track_Under_Caret > Songtracks - 1)
    {
        while(Track_Under_Caret > Songtracks - 1)
        {
            Track_Under_Caret--;
        }
        gui_track = 0;
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }
    Visible_Columns = Get_Visible_Complete_Tracks(NULL);

    // Keep the caret in focus
    if(Track_Under_Caret < 0)
    {
        Track_Under_Caret = Songtracks - 1;
        gui_track = Songtracks - (Visible_Columns);
        if(gui_track < 0) gui_track = 0;
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }

    if(!modac)
    {
        // Right
        if(Track_Under_Caret >= gui_track + Visible_Columns)
        {
            gui_track = Track_Under_Caret - (Visible_Columns - 1);
        }

        // Left
        if(Track_Under_Caret < gui_track)
        {
            gui_track -= gui_track - Track_Under_Caret;
        }
    }
    else
    {
        // Right
        if(Track_Under_Caret >= gui_track + Visible_Columns)
        {
            Track_Under_Caret = gui_track + (Visible_Columns - 1);
            gui_action = GUI_CMD_SET_FOCUS_TRACK;
        }
        // Left
        if(Track_Under_Caret < gui_track)
        {
            Track_Under_Caret = gui_track;
            gui_action = GUI_CMD_SET_FOCUS_TRACK;
        }
    }

    // ----
    Set_Track_Slider(gui_track);

    // We need to check if the column where Track_Under_Caret is can't be displayed
    // after the correction
    if(!modac)
    {
        if(Track_Under_Caret >= gui_track + Visible_Columns)
        {
            gui_track = Track_Under_Caret - (Visible_Columns - 1);
            Set_Track_Slider(gui_track);
        }
    }

    Cur_Line = Get_Pattern_Line();

    draw_pated(gui_track, Cur_Line, Track_Under_Caret, Column_Under_Caret);

    if(Continuous_Scroll)
    {
        // Count the number of rows in the song
        Max_Lines_Song = 0;
        for(i = 0; i < Song_Length; i++)
        {   
            Max_Lines_Song += patternLines[pSequence[i]];
        }
        // Slider for the entire song
        Realslider_Vert(MAX_PATT_SCREEN_X + 1, 200, Get_Song_Line(),
                        DISPLAYED_LINES,
                        DISPLAYED_LINES + Max_Lines_Song,
                        (Cur_Height - 452) + Patterns_Lines_Offset,
                        TRUE,
                        TRUE);
    }
    else
    {
        // Just for the pattern
        Realslider_Vert(MAX_PATT_SCREEN_X + 1, 200, Cur_Line,
                        DISPLAYED_LINES,
                        patternLines[pSequence[Cur_Position]] + DISPLAYED_LINES,
                        (Cur_Height - 452) + Patterns_Lines_Offset,
                        TRUE,
                        TRUE);
    }
}

// ------------------------------------------------------
// Draw the slider and the buttons located at the right of the pattern
void Draw_Pattern_Right_Stuff()
{
    int cur_line;

    cur_line = Get_Pattern_Line();

    SetColor(COL_BLACK);
    Fillrect(MAX_PATT_SCREEN_X, 184, MAX_PATT_SCREEN_X + 20, (Cur_Height - 251) + (16 * 5) + Patterns_Lines_Offset);

    DrawVLine(0, 184, (Cur_Height - 251) + (16 * 5) + Patterns_Lines_Offset, COL_BLACK);

    // 195
    Gui_Draw_Button_Box(MAX_PATT_SCREEN_X + 1, 184, 16, 14, "\01", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    Gui_Draw_Button_Box(MAX_PATT_SCREEN_X + 1, (Cur_Height - 251) + Patterns_Lines_Offset, 16, 14, "\02", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);

    Draw_Blocks_Buffers_Status();
}

// ------------------------------------------------------
// Display the status of the selection buffers
void Draw_Blocks_Buffers_Status()
{
    int High[] =
    {
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL
    };

    High[Curr_Buff_Block] = BUTTON_PUSHED;

    Gui_Draw_Button_Box(MAX_PATT_SCREEN_X + 1,
                        (Cur_Height - 251) + (16 * 1) + Patterns_Lines_Offset, 16, 14, "1",
                        High[0] | BUTTON_TEXT_CENTERED | (Buff_Full[0] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(MAX_PATT_SCREEN_X + 1,
                        (Cur_Height - 251) + (16 * 2) + Patterns_Lines_Offset, 16, 14, "2",
                        High[1] | BUTTON_TEXT_CENTERED | (Buff_Full[1] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(MAX_PATT_SCREEN_X + 1,
                        (Cur_Height - 251) + (16 * 3) + Patterns_Lines_Offset, 16, 14, "3",
                        High[2] | BUTTON_TEXT_CENTERED | (Buff_Full[2] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(MAX_PATT_SCREEN_X + 1,
                        (Cur_Height - 251) + (16 * 4) + Patterns_Lines_Offset, 16, 14, "4",
                        High[3] | BUTTON_TEXT_CENTERED | (Buff_Full[3] ? 0 : BUTTON_LOW_FONT));
}

// ------------------------------------------------------
// Actually the extra data associated with the pattern
void Actualize_Patterned(void)
{
    if(Current_Edit_Steps < 0) Current_Edit_Steps = 16;
    if(Current_Edit_Steps > 16) Current_Edit_Steps = 0;
    if(Current_Octave < 0) Current_Octave = 0;
    if(Current_Octave > 8) Current_Octave = 8;
    if(Current_Instrument < 0) Current_Instrument = 127;
    if(Current_Instrument > 127) Current_Instrument = 0;

    char tcp[30];
    sprintf(tcp, "%s_", nameins[Current_Instrument]);

    if(snamesel == INPUT_INSTRUMENT_NAME) Gui_Draw_Button_Box(90, 108, 166, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT);
    else Gui_Draw_Button_Box(90, 108, 166, 16, nameins[Current_Instrument], BUTTON_NORMAL | BUTTON_INPUT);

    Gui_Draw_Arrows_Number_Box2(90, 126, Current_Edit_Steps, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    value_box(258, 126, Current_Octave, BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    value_box(258, 108, Current_Instrument, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    Actualize_Instruments_Synths_List(2);
}

// ------------------------------------------------------
// Return the correct color of a character (select / highlighted or not)
int Get_Nibble_Color(int In_Prev_Next, int row, int column, int multi, int Shadow)
{
    int color = (Shadow * 87);
    
    if(In_Prev_Next == 0)
    {
        if(column >= block_start_track[Curr_Buff_Block] &&
           column <= block_end_track[Curr_Buff_Block] &&
           row >= block_start[Curr_Buff_Block] && row <= block_end[Curr_Buff_Block])
        {
            color = 8;
        }
    }
    if(multi) color += 71;
    return(color);
}

// ------------------------------------------------------
// Return the color of a character in the caret row
int Get_Nibble_Color_Highlight(int row, int column)
{
    int color = 0;

    if(column >= block_start_track[Curr_Buff_Block] &&
       column <= block_end_track[Curr_Buff_Block] &&
       row >= block_start[Curr_Buff_Block] && row <= block_end[Curr_Buff_Block])
    {
        color = 16;
    }
    return(color + 16);
}

// ------------------------------------------------------
// Return the size in pixel of a given track
int Get_Track_Size(int Track, int *Column)
{
    int i;
    int dover = 0;
    int old_dover = 0;
    int Phony;

    if(Column == NULL) Column = &Phony;
    *Column = 0;

    // Gap
    old_dover = dover;
    dover += 2;
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);

    old_dover = dover;
    dover += 4;
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);

    // On / off
    for(i = 0; i < Channels_MultiNotes[Track]; i++)
    {
        // Note
        old_dover = dover;
        dover += Cur_Char_size[Track] * 3;
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
        *Column += 1;

        // Gap
        old_dover = dover;
        dover += PAT_COL_SHIFT - 2;
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);

        // Instrument
        old_dover = dover;
        dover += Cur_Char_size[Track];
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
        *Column += 1;

        old_dover = dover;
        dover += Cur_Char_size[Track];
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
        *Column += 1;

        // Gap
        old_dover = dover;
        dover += 2;
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
    }

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);

    // Volume
    old_dover = dover;
    dover += Cur_Char_size[Track];
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
    *Column += 1;

    old_dover = dover;
    dover += Cur_Char_size[Track];
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
    *Column += 1;

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);

    // Panning
    old_dover = dover;
    dover += Cur_Char_size[Track];
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
    *Column += 1;

    old_dover = dover;
    dover += Cur_Char_size[Track];
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
    *Column += 1;

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(dover >= MAX_PATT_SCREEN_X) return(old_dover);

    for(i = 0; i < Channels_Effects[Track]; i++)
    {
        // Effect
        old_dover = dover;
        dover += Cur_Char_size[Track];
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
        *Column += 1;

        old_dover = dover;
        dover += Cur_Char_size[Track];
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
        *Column += 1;

        old_dover = dover;
        dover += Cur_Char_size[Track];
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
        *Column += 1;

        old_dover = dover;
        dover += Cur_Char_size[Track];
        if(dover >= MAX_PATT_SCREEN_X) return(old_dover);
        *Column += 1;
    }
    return(dover);
}

// ------------------------------------------------------
// Return the real size (with all elements) in pixel of a given track
int Get_Track_Real_Size(int Track)
{
    int i;
    int dover = 0;
    int old_dover = 0;

    // Gap
    old_dover = dover;
    dover += 2;

    old_dover = dover;
    dover += 4;

    // On / off
    for(i = 0; i < Channels_MultiNotes[Track]; i++)
    {
        // Note
        old_dover = dover;
        dover += Cur_Char_size[Track] * 3;

        // Gap
        old_dover = dover;
        dover += PAT_COL_SHIFT - 2;

        // Instrument
        old_dover = dover;
        dover += Cur_Char_size[Track];

        old_dover = dover;
        dover += Cur_Char_size[Track];

        // Gap
        old_dover = dover;
        dover += 2;
    }

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;

    // Volume
    old_dover = dover;
    dover += Cur_Char_size[Track];

    old_dover = dover;
    dover += Cur_Char_size[Track];

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;

    // Panning
    old_dover = dover;
    dover += Cur_Char_size[Track];

    old_dover = dover;
    dover += Cur_Char_size[Track];

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;

    for(i = 0; i < Channels_Effects[Track]; i++)
    {
        // Effect
        old_dover = dover;
        dover += Cur_Char_size[Track];

        old_dover = dover;
        dover += Cur_Char_size[Track];

        old_dover = dover;
        dover += Cur_Char_size[Track];

        old_dover = dover;
        dover += Cur_Char_size[Track];
    }
    return(dover);
}

// ------------------------------------------------------
// Return the size in pixel of all visible tracks
int Get_Visible_Tracks_Size(int max_tracks)
{
    int i;
    int size = 0;
    int column = 0;

    for(i = gui_track; i < max_tracks; i++)
    {
        size += Get_Track_Size(i, &column);
        if(size >= MAX_PATT_SCREEN_X) break;
    }
    return(size);
}

// ------------------------------------------------------
// Return the index of the track located under the mouse pointer
int Get_Track_Over_Mouse(int Mouse, int *Was_Scrolling, int Left)
{
    int i;
    int bound_left = 0;
    int bound_right = 0;
    int under_mouse = 0;
    int mouse_coord = Mouse;
    int found_track;
    int track_size;

    if(Was_Scrolling) *Was_Scrolling = FALSE;

    if(mouse_coord >= Last_Pixel && !Left)
    {
        // Right scrolling
        if(Pattern_Scrolling_Horiz_Right)
        {
            Pattern_Delay_Horiz_Right += Pattern_Timer_Horiz_Right.Get_Frames_Delay();
            if(Pattern_Delay_Horiz_Right < Pattern_First_Delay_Horiz_Right)
            {
                // Wait before scrolling
                mouse_coord = Last_Pixel_Complete - 1;
            }
            else
            {
                under_mouse = gui_track + Visible_Columns - 1;
                // Scroll it
                Pattern_Delay_Horiz_Right = 0;
                Pattern_First_Delay_Horiz_Right = 200.0f;
                if(under_mouse > (Songtracks - 1)) under_mouse = Songtracks - 1;
                if(!Left) Column_Under_Caret = Get_Last_Track_Column(under_mouse);
                if(Was_Scrolling) *Was_Scrolling = TRUE;
            }
        }
        else
        {
            Pattern_Timer_Horiz_Right.Set_Frames_Counter();
            Pattern_Scrolling_Horiz_Right = TRUE;
            Pattern_Delay_Horiz_Right = 0;
            Pattern_First_Delay_Horiz_Right = 150.0f;
            mouse_coord = Last_Pixel_Complete - 1;
        }
    }
    else
    {
        if(mouse_coord >= Last_Pixel_Complete && !Left)
        {
            // Right scrolling
            if(Pattern_Scrolling_Horiz_Right_Slow)
            {
                Pattern_Delay_Horiz_Right_Slow += Pattern_Timer_Horiz_Right_Slow.Get_Frames_Delay();
                if(Pattern_Delay_Horiz_Right_Slow < Pattern_First_Delay_Horiz_Right_Slow)
                {
                    // Wait before scrolling
                    mouse_coord = Last_Pixel_Complete - 1;
                }
                else
                {
                    under_mouse = gui_track + Visible_Columns - 1;
                    // Scroll it
                    Pattern_Delay_Horiz_Right_Slow = 0;
                    Pattern_First_Delay_Horiz_Right_Slow = 200.0f;
                    if(under_mouse > (Songtracks - 1)) under_mouse = Songtracks - 1;
                    if(!Left) Column_Under_Caret = Get_Last_Track_Column(under_mouse);
                    if(Was_Scrolling) *Was_Scrolling = TRUE;
                }
            }
            else
            {
                Pattern_Timer_Horiz_Right_Slow.Set_Frames_Counter();
                Pattern_Scrolling_Horiz_Right_Slow = TRUE;
                Pattern_Delay_Horiz_Right_Slow = 0;
                Pattern_First_Delay_Horiz_Right_Slow = 150.0f;
                mouse_coord = Last_Pixel_Complete - 1;
            }
        }
    }
    if(mouse_coord < Last_Pixel)
    {
        // From 0
        mouse_coord -= PAT_COL_NOTE;
        found_track = FALSE;
        int max_size = Get_Visible_Tracks_Size((gui_track + Visible_Columns));
        int tVisible_Columns = Visible_Columns;
        if(max_size < Last_Pixel)
        {
            if((gui_track + tVisible_Columns) < Songtracks)
            {
                tVisible_Columns++;
            }
        }
        for(i = gui_track; i < (gui_track + tVisible_Columns); i++)
        {
            track_size = Get_Track_Size(i, NULL);
            bound_right += track_size;
            if(mouse_coord >= bound_left && mouse_coord < bound_right)
            {
                under_mouse = i;
                found_track = TRUE;
                break;
            }
            bound_left = bound_right;
        }
        if(found_track == FALSE && mouse_coord >= 0)
        {
            under_mouse = Songtracks - 1;
        }
    }

    // Temporary fix before adding auto scroll during blocks selection
    if(Left)
    {
        if(mouse_coord >= Last_Pixel)
        {
            mouse_coord = Last_Pixel;
            under_mouse = gui_track + Visible_Columns - 1;
        }
    }

    // Left scrolling
    if(under_mouse < gui_track && !Left)
    {
        if(Pattern_Scrolling_Horiz_Left)
        {
            Pattern_Delay_Horiz_Left += Pattern_Timer_Horiz_Left.Get_Frames_Delay();
            if(Pattern_Delay_Horiz_Left < Pattern_First_Delay_Horiz_Left)
            {
                // Wait before scrolling
                under_mouse = gui_track;
            }
            else
            {
                under_mouse = gui_track - 1;
                // Scroll it
                Pattern_Delay_Horiz_Left = 0;
                Pattern_First_Delay_Horiz_Left = 200.0f;
            }
        }
        else
        {
            Pattern_Timer_Horiz_Left.Set_Frames_Counter();
            Pattern_Scrolling_Horiz_Left = TRUE;
            Pattern_Delay_Horiz_Left = 0;
            Pattern_First_Delay_Horiz_Left = 150.0f;
            under_mouse = gui_track;
        }
        if(Was_Scrolling) *Was_Scrolling = TRUE;
        Column_Under_Caret = 0;
    }

    if(under_mouse > Songtracks - 1) under_mouse = Songtracks - 1;
    if(under_mouse < 0) under_mouse = 0;
    return(under_mouse);
}

// ------------------------------------------------------
// Return the index of a track column according to a coordinate
int Get_Column_Idx(int track, int mouse_coord)
{
    int ret_value = 0;
    int old_dover = 0;
    int dover = 0;
    int i;
    
    if(mouse_coord < 0) return(0);

    // Gap
    old_dover = dover;
    dover += 2;
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
    
    old_dover = dover;
    dover += 4;
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);

    // On / off
    for(i = 0; i < Channels_MultiNotes[track]; i++)
    {
        // Note
        old_dover = dover;
        dover += Cur_Char_size[track] * 3;
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
        ret_value++;

        // Gap
        old_dover = dover;
        dover += PAT_COL_SHIFT - 2;
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);

        // Instrument
        old_dover = dover;
        dover += Cur_Char_size[track];
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
        ret_value++;

        old_dover = dover;
        dover += Cur_Char_size[track];
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
        ret_value++;

        // Gap
        old_dover = dover;
        dover += 2;
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
    }

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);

    // Volume
    old_dover = dover;
    dover += Cur_Char_size[track];
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
    ret_value++;

    old_dover = dover;
    dover += Cur_Char_size[track];
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
    ret_value++;

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);

    // Panning
    old_dover = dover;
    dover += Cur_Char_size[track];
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
    ret_value++;

    old_dover = dover;
    dover += Cur_Char_size[track];
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
    ret_value++;

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);

    for(i = 0; i < Channels_Effects[track]; i++)
    {
        // Effect
        old_dover = dover;
        dover += Cur_Char_size[track];
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
        ret_value++;

        old_dover = dover;
        dover += Cur_Char_size[track];
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
        ret_value++;

        old_dover = dover;
        dover += Cur_Char_size[track];
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
        ret_value++;

        old_dover = dover;
        dover += Cur_Char_size[track];
        if(mouse_coord >= old_dover && mouse_coord < dover) return(ret_value);
        ret_value++;
    }
    int Last_Col = Get_Last_Track_Column(track);
    if(ret_value > Last_Col) ret_value = Last_Col;
    return(ret_value);
}

// ------------------------------------------------------
// Return the last column of a track
int Get_Last_Track_Column(int track)
{
    // Notes + Volume + Panning + Fx
    return(((Channels_MultiNotes[track] * 3) +
           EXTRA_NIBBLE_DAT +
           (Channels_Effects[track] * 4)) - 1);
}

// ------------------------------------------------------
// Return the last visible column and track
int Get_Last_Column_And_Track(int *track)
{
    int i;
    int dover = 0;
    int old_dover = 0;
    int column = 0;
    int pixel_visible_tracks = PAT_COL_NOTE;
    int channel_size;
    int cur_track;

    *track = gui_track;
    for(i = gui_track; i < Songtracks; i++)
    {
        channel_size = Get_Track_Size(i, NULL);
        if((pixel_visible_tracks + channel_size - 1) >= MAX_PATT_SCREEN_X)
        {
            break;
        }
        *track += 1;
        pixel_visible_tracks += channel_size;
    }

    cur_track = *track;

    // Start at last track
    dover = pixel_visible_tracks;

    // Gap
    old_dover = dover;
    dover += 2;
    if(dover >= MAX_PATT_SCREEN_X) return(column);

    old_dover = dover;
    dover += 4;
    if(dover >= MAX_PATT_SCREEN_X) return(column);

    // On / off
    for(i = 0; i < Channels_MultiNotes[cur_track]; i++)
    {
        // Note
        old_dover = dover;
        dover += Cur_Char_size[cur_track] * 3;
        if(dover >= MAX_PATT_SCREEN_X) return(column);
        column++;

        // Gap
        old_dover = dover;
        dover += PAT_COL_SHIFT - 2;
        if(dover >= MAX_PATT_SCREEN_X) return(column);

        // Instrument
        old_dover = dover;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) return(column);
        column++;

        old_dover = dover;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) return(column);
        column++;

        // Gap
        old_dover = dover;
        dover += 2;
        if(dover >= MAX_PATT_SCREEN_X) return(column);
    }

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(dover >= MAX_PATT_SCREEN_X) return(column);

    // Volume
    old_dover = dover;
    dover += Cur_Char_size[cur_track];
    if(dover >= MAX_PATT_SCREEN_X) return(column);
    column++;

    old_dover = dover;
    dover += Cur_Char_size[cur_track];
    if(dover >= MAX_PATT_SCREEN_X) return(column);
    column++;

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(dover >= MAX_PATT_SCREEN_X) return(column);

    // Panning
    old_dover = dover;
    dover += Cur_Char_size[cur_track];
    if(dover >= MAX_PATT_SCREEN_X) return(column);
    column++;

    old_dover = dover;
    dover += Cur_Char_size[cur_track];
    if(dover >= MAX_PATT_SCREEN_X) return(column);
    column++;

    // Gap
    old_dover = dover;
    dover += PAT_COL_SHIFT;
    if(dover >= MAX_PATT_SCREEN_X) return(column);

    for(i = 0; i < Channels_Effects[cur_track]; i++)
    {
        // Effect
        old_dover = dover;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) return(column);
        column++;

        old_dover = dover;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) return(column);
        column++;

        old_dover = dover;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) return(column);
        column++;

        old_dover = dover;
        dover += Cur_Char_size[cur_track];
        if(dover >= MAX_PATT_SCREEN_X) return(column);
        column++;
    }
    return(column);
}

// ------------------------------------------------------
// Return the index of the column located under the mouse pointer
int Get_Column_Over_Mouse(int *track, int *column,
                          int check_boundaries,
                          int *Was_Scrolling,
                          int Left)
{
    int i;
    int max_tr = 6;
    int dover = 0;
    int old_dover = 0;
    int track_size;
    int last_column;
    int last_track;
    int mouse;
    int tmp_track;
    int mouse_coord;
    int tmp_column;
    int need_scroll;
    int Visible_Columns;
    int max_mouse_coord;
    int pixels;

    need_scroll = 0;
    mouse = Mouse.x;
    max_mouse_coord = mouse - PAT_COL_NOTE;

get_tracks_boundaries:
    tmp_track = Get_Track_Over_Mouse(mouse, Was_Scrolling, Left);
    mouse_coord = mouse - PAT_COL_NOTE;
    for(i = tmp_track - 1; i >= gui_track; i--)
    {
        track_size = Get_Track_Size(i, NULL);
        mouse_coord -= track_size;
    }
    tmp_column = Get_Column_Idx(tmp_track, mouse_coord);

    if(check_boundaries)
    {
        last_column = Get_Last_Column_And_Track(&last_track);
        if(tmp_track >= last_track && tmp_column >= last_column)
        {
            mouse--;
            goto get_tracks_boundaries;
        }
    }
    if(Was_Scrolling)
    {
        if(!*Was_Scrolling)
        {
            *column = tmp_column;
        }
    }
    else
    {
        *column = tmp_column;
    }
    int mouse_line = (Mouse.y - (183 + 17));
    if(mouse_line >= 0)
    {
        Visible_Columns = Get_Visible_Complete_Tracks(&pixels);
        if(mouse_coord < 0)
        {
            need_scroll = -1;
        }
        if(max_mouse_coord > pixels && (Visible_Columns + gui_track) < Songtracks)
        {
            need_scroll = 1;
        }
    }
    *track = tmp_track;
    return need_scroll;
}

// ------------------------------------------------------
// return the index of the row located under the mouse pointer
int Get_Line_Over_Mouse(int *Need_Scroll)
{
    int Cur_Position = Get_Song_Position();

    int mouse_line = (Mouse.y - (183 + 17));
    *Need_Scroll = 0;
    // (Highlight line is doubled)
    if(mouse_line >= ((VIEWLINE + 1) * 8)) mouse_line -= 8;
    mouse_line /= 8;
    mouse_line -= VIEWLINE;
    mouse_line += Pattern_Line;
    if(mouse_line > (VIEWLINE + Pattern_Line))
    {
        if(mouse_line >= patternLines[pSequence[Cur_Position]])
        {
            mouse_line = patternLines[pSequence[Cur_Position]];
        }
        *Need_Scroll = mouse_line - (VIEWLINE + Pattern_Line);
    }
    if(mouse_line < -(VIEWLINE - Pattern_Line))
    {
        if((VIEWLINE - Pattern_Line) < 0)
        {
            *Need_Scroll = -(-(VIEWLINE - Pattern_Line) - mouse_line);
        }
    }
    if(mouse_line > patternLines[pSequence[Cur_Position]] - 1) mouse_line = patternLines[pSequence[Cur_Position]] - 1;
    if(mouse_line < 0)
    {
        mouse_line = 0;
    }
    return(mouse_line);
}

// ------------------------------------------------------
// Return the number of tracks completly displayed on screen
int Get_Visible_Complete_Tracks(int *pixels)
{
    int pixel_visible_tracks = PAT_COL_NOTE;
    int channel_size;
    int i;
    int nbr_tracks = 0;

    for(i = gui_track; i < Songtracks; i++)
    {
        channel_size = Get_Track_Size(i, NULL);
        if((pixel_visible_tracks + channel_size - 1) >= MAX_PATT_SCREEN_X)
        {
            break;
        }
        nbr_tracks++;
        pixel_visible_tracks += channel_size;
    }
    if(pixels != NULL)
    {
        *pixels = pixel_visible_tracks;
    }
    return(nbr_tracks);
}

// ------------------------------------------------------
// Return the number of tracks partially displayed on screen
int Get_Visible_Partial_Tracks(void)
{
    int tracks = Visible_Columns;
    int max_size = Get_Visible_Tracks_Size((gui_track + Visible_Columns));
    if(max_size < MAX_PATT_SCREEN_X)
    {
        if((gui_track + tracks) < Songtracks)
        {
            tracks++;
        }
    }
    return(tracks);
}

// ------------------------------------------------------
// Make sure the position isn't beyond current pattern lines range
void Bound_Patt_Pos(void)
{
    if(Pattern_Line >= patternLines[pSequence[Song_Position]])
    {
        Pattern_Line = patternLines[pSequence[Song_Position]] - 1;
    }
}

// ------------------------------------------------------
// Reset the mouse scrolling timing
// Each time the right mouse button is pressed
void Reset_Pattern_Scrolling_Horiz(void)
{
    Pattern_Delay_Horiz_Left = 0;
    Pattern_Delay_Horiz_Right = 0;
    Pattern_Delay_Horiz_Right_Slow = 0;
    Pattern_Scrolling_Horiz_Left = FALSE;
    Pattern_Scrolling_Horiz_Right = FALSE;
    Pattern_Scrolling_Horiz_Right_Slow = FALSE;
    Pattern_Delay_Vert = 0.0f;
    Pattern_Scrolling_Vert = FALSE;
    Select_Delay_Vert = 0.0f;
    Select_Scrolling_Vert = FALSE;
    Select_Delay_Horz = 0.0f;
    Select_Scrolling_Horz = FALSE;
}

// ------------------------------------------------------
// Set the layout of the horizontal tracks slider and bound the caret
void Set_Track_Slider(int pos)
{
    Visible_Columns = Get_Visible_Complete_Tracks(NULL);
    if(Track_Under_Caret >= pos + Visible_Columns)
    {
        Track_Under_Caret = pos + Visible_Columns;
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }
    if(Track_Under_Caret < pos)
    {
        Track_Under_Caret = pos;
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }
    float fpos = (float) pos;

    if(!(Songtracks - Visible_Columns))
    {
        fpos = 0.0f;
    }
    Realslider_Horiz(POS_HORIZ_SLIDER,
                     (Cur_Height - 171) + Patterns_Lines_Offset,
                     (int) fpos,
                     Visible_Columns,
                     Songtracks,
                     Cur_Width - (POS_HORIZ_SLIDER + 1),
                     TRUE);
}

// ------------------------------------------------------
// Handle the mouse wheel event
void Mouse_Wheel_Pattern_Ed(int roll_amount, int allow)
{
    int Cur_Position = Get_Song_Position();

    if(allow)
    {
        // Scroll the patterns
        if(zcheckMouse_nobutton(0, 182, CONSOLE_WIDTH, (Cur_Height - 354) + Patterns_Lines_Offset) == 1)
        {
            Pattern_Line += roll_amount;
            if(Continuous_Scroll && !Cur_Position) if(Pattern_Line < 0) Pattern_Line = 0;
            if(Continuous_Scroll && (Cur_Position == Song_Length - 1))
            {
                if(Pattern_Line >= patternLines[pSequence[Cur_Position]])
                {
                    Pattern_Line = patternLines[pSequence[Cur_Position]] - 1;
                }
            }
            Actupated(0);
        }
    }

    // Current track slider (horizontal)
    if(zcheckMouse(POS_HORIZ_SLIDER - 1, (Cur_Height - 171) + Patterns_Lines_Offset,
                   (Cur_Width - (POS_HORIZ_SLIDER + 1)), 16))
    {
        Visible_Columns = Get_Visible_Complete_Tracks(NULL);

        gui_track += -roll_amount;
        if(gui_track < 0)
        {
            gui_track = 0;
        }
        if(gui_track >= ((Songtracks - Visible_Columns)))
        {
            gui_track = ((Songtracks - Visible_Columns));
        }
        Actupated(1);
    }
}

// ------------------------------------------------------
// Handle the sliders event (right mouse button)
void Mouse_Sliders_Right_Pattern_Ed(void)
{
    int sched_line;

    // Position the caret on the specified track/column with the mouse
    if(zcheckMouse(1, 183 + 15, CHANNELS_WIDTH, (Cur_Height - 371) + Patterns_Lines_Offset))
    {
        int In_Scrolling = FALSE;
        Get_Column_Over_Mouse(&Track_Under_Caret,
                              &Column_Under_Caret,
                              FALSE,
                              &In_Scrolling,
                              FALSE);
        Actupated(0);
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }

    // Go to the row selected with the mouse
    if(!Song_Playing)
    {
        if(zcheckMouse(1, 183 + 15, CHANNELS_WIDTH, (Cur_Height - 371) + Patterns_Lines_Offset))
        {
            if(!is_recording)
            {
                int Need_Scroll;
                sched_line = Get_Line_Over_Mouse(&Need_Scroll);

                if(Pattern_Scrolling_Vert)
                {
                    Pattern_Delay_Vert += Pattern_Timer_Vert.Get_Frames_Delay();
                    if(Pattern_Delay_Vert >= Pattern_First_Delay_Vert)
                    {
                        // Scroll it
                        Pattern_Delay_Vert = 0;
                        Pattern_First_Delay_Vert = 250.0f;
                        Pattern_Line = sched_line;
                    }
                }
                else
                {
                    Pattern_Timer_Vert.Set_Frames_Counter();
                    Pattern_Scrolling_Vert = TRUE;
                    Pattern_Delay_Vert = 0;
                    Pattern_First_Delay_Vert = 250.0f;
                    Pattern_Line = sched_line;
                }
            }
            Actupated(0);
        }
    }
}

// ------------------------------------------------------
// Handle the sliders event (left mouse button)
void Mouse_Sliders_Pattern_Ed(void)
{
    // Current track slider (horizontal)
    if(zcheckMouse(POS_HORIZ_SLIDER - 1, (Cur_Height - 171) + Patterns_Lines_Offset,
                   (Cur_Width - (POS_HORIZ_SLIDER + 1)), 16))
    {
        float Pos_Mouse = (float) (Mouse.x - (POS_HORIZ_SLIDER - 1));
        if(Pos_Mouse < 0) Pos_Mouse = 0;
        // Normalize and scale
        Pos_Mouse = Pos_Mouse / ((float) (Cur_Width - (POS_HORIZ_SLIDER - 1)));
        if(Pos_Mouse > 1.0f) Pos_Mouse = 1.0f;
        Visible_Columns = Get_Visible_Complete_Tracks(NULL);

        Pos_Mouse = Pos_Mouse * ((Songtracks - Visible_Columns) + 1);
        gui_track = (int) Pos_Mouse;
        Actupated(1);
    }

    if(Continuous_Scroll)
    {
        // Rows slider (vertical) (whole song)
        if(zcheckMouse(MAX_PATT_SCREEN_X + 1, 200, 16 + 1, (Cur_Height - 452) + Patterns_Lines_Offset) & !Song_Playing)
        {
            int max_length;
            int Max_Lines_Song;
            int final_row;
            int i;

            // Max rows
            Max_Lines_Song = 0;
            for(i = 0; i < Song_Length; i++)
            {
                Max_Lines_Song += patternLines[pSequence[i]];
            }

            // Use linear position
            max_length = Max_Lines_Song + DISPLAYED_LINES;
        
            int Center = Slider_Get_Center(DISPLAYED_LINES, max_length, (Cur_Height - 452) + Patterns_Lines_Offset);
            float Pos_Mouse = ((float) ((Mouse.y - 200) - (Center / 2))) / ((float) (Cur_Height - 452) + (float) Patterns_Lines_Offset);
            if(Pos_Mouse > 1.0f) Pos_Mouse = 1.0f;
            float s_offset = (Pos_Mouse * max_length);
            if(s_offset > (float) (max_length - DISPLAYED_LINES))
            {
                s_offset = (float) (max_length - DISPLAYED_LINES);
            }
            final_row = (int32) s_offset;
            if(final_row < 0) final_row = 0;
            if(final_row > Max_Lines_Song - 1) final_row = Max_Lines_Song - 1;
            // Convert it back to segmented pattern/position structure
            i = 0;
            while(final_row >= patternLines[pSequence[i]])
            {
                final_row -= patternLines[pSequence[i]];
                i++;
            }
            Pattern_Line = final_row;
            Goto_Song_Position(i);
            Actupated(0);
        }
    }
    else
    {
        // Rows slider (vertical) (pattern only)
        if(zcheckMouse(MAX_PATT_SCREEN_X + 1, 200 + (Continuous_Scroll * 80), 16 + 1, (Cur_Height - 452) - (Continuous_Scroll * 80) + Patterns_Lines_Offset) & !Song_Playing)
        {
            int final_row;
            int Cur_Position = Get_Song_Position();
            int max_length = patternLines[pSequence[Cur_Position]] + DISPLAYED_LINES;
            int Center = Slider_Get_Center(DISPLAYED_LINES, max_length, ((Cur_Height - 452) - (Continuous_Scroll * 80)) + Patterns_Lines_Offset);
            float Pos_Mouse = ((float) ((Mouse.y - (200 + (Continuous_Scroll * 80))) - 
                              (Center / 2))) / (((float) (Cur_Height - 452) - (Continuous_Scroll * 80)) + (float) Patterns_Lines_Offset);
            if(Pos_Mouse > 1.0f) Pos_Mouse = 1.0f;
            float s_offset = (Pos_Mouse * max_length);
            if(s_offset > (float) (max_length - DISPLAYED_LINES))
            {
                s_offset = (float) (max_length - DISPLAYED_LINES);
            }
            final_row = (int32) s_offset;
            if(final_row < 0) final_row = 0;
            if(final_row > patternLines[pSequence[Cur_Position]] - 1) final_row = patternLines[pSequence[Cur_Position]] - 1;
            Pattern_Line = final_row;
            Actupated(0);
        }
    }

    // End of the marking stuff
    if(zcheckMouse(1, 183 + 15, MAX_PATT_SCREEN_X, (Cur_Height - 371) + Patterns_Lines_Offset) && !Song_Playing)
    {
        int track;
        int column;
        int track2;
        int column2;
        int line;
        int Need_Scroll;
        int Visible_Columns;
        int was_scrolling = 0;
        int need_horz_scroll = Get_Column_Over_Mouse(&track2, &column2, TRUE, NULL, TRUE);

        if(need_horz_scroll)
        {
                // Scroll the pattern upward or downward if the mouse is out of bounds
                if(Select_Scrolling_Horz)
                {
                    Select_Delay_Horz += Select_Timer_Horz.Get_Frames_Delay();
                    if(Select_Delay_Horz >= Select_First_Delay_Horz)
                    {
                        // Scroll it
                        Select_Delay_Horz = 0;
                        Select_First_Delay_Horz = 100.0f;
                        gui_track += need_horz_scroll;
                        if(gui_track < 0)
                        {
                            gui_track = 0;
                        }
                        if(Track_Under_Caret < gui_track)
                        {
                            Track_Under_Caret = gui_track;
                        }
                        Visible_Columns = Get_Visible_Complete_Tracks(NULL);

                        if(Track_Under_Caret > (Visible_Columns + gui_track - 1))
                        {
                            Track_Under_Caret = Visible_Columns + gui_track - 1;
                        }
                        Actupated(0);
                        gui_action = GUI_CMD_SET_FOCUS_TRACK;
                    }
                }
                else
                {
                    Select_Timer_Horz.Set_Frames_Counter();
                    Select_Scrolling_Horz = TRUE;
                    Select_Delay_Horz = 0;
                    Select_First_Delay_Horz = 100.0f;
                }
        }

        need_horz_scroll = Get_Column_Over_Mouse(&track, &column, TRUE, &was_scrolling, TRUE);
        line = Get_Line_Over_Mouse(&Need_Scroll);
        if(Need_Scroll)
        {
                // Scroll the pattern upward or downward if the mouse is out of bounds
                if(Select_Scrolling_Vert)
                {
                    Select_Delay_Vert += Select_Timer_Vert.Get_Frames_Delay();
                    if(Select_Delay_Vert >= Select_First_Delay_Vert)
                    {
                        // Scroll it
                        Select_Delay_Vert = 0;
                        Select_First_Delay_Vert = 100.0f;
                        Pattern_Line += Need_Scroll;
                    }
                }
                else
                {
                    Select_Timer_Vert.Set_Frames_Counter();
                    Select_Scrolling_Vert = TRUE;
                    Select_Delay_Vert = 0;
                    Select_First_Delay_Vert = 100.0f;
                }
        }
        Mark_Block_End(column, track, line, 3);
    }
}

// ------------------------------------------------------
// Handle the left mouse button event
void Mouse_Left_Pattern_Ed(void)
{
    int i;
    int start_mute_check_x;
    int tracks;
    int Need_Scroll;

    // Start of the marking block
    if(zcheckMouse(1, 183 + 15, MAX_PATT_SCREEN_X, (Cur_Height - 371) + Patterns_Lines_Offset) && !Song_Playing)
    {
        int track;
        int column;
        int In_Scrolling = FALSE;
        Get_Column_Over_Mouse(&track, &column, FALSE, NULL, TRUE);
        Mark_Block_Start(column, track, Get_Line_Over_Mouse(&Need_Scroll));
    }

    // Prev row
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, 184, 16 + 1, 14) & !Song_Playing)
    {
        Goto_Previous_Row(FALSE);
    }

    // Next row
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, (Cur_Height - 251) + Patterns_Lines_Offset, 16 + 1, 14) & !Song_Playing)
    {
        Goto_Next_Row(FALSE);
    }

    // Set buffer 1
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, (Cur_Height - 251) + (16 * 1) + Patterns_Lines_Offset, 16 + 1, 14))
    {
        Curr_Buff_Block = 0;
        Draw_Pattern_Right_Stuff();
        Actupated(0);
        teac = 0;
        gui_action = GUI_CMD_UPDATE_SEQUENCER;
    }
    // Set buffer 2
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, (Cur_Height - 251) + (16 * 2) + Patterns_Lines_Offset, 16 + 1, 14))
    {
        Curr_Buff_Block = 1;
        Draw_Pattern_Right_Stuff();
        Actupated(0);
        teac = 0;
        gui_action = GUI_CMD_UPDATE_SEQUENCER;
    }
    // Set buffer 3
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, (Cur_Height - 251) + (16 * 3) + Patterns_Lines_Offset, 16 + 1, 14))
    {
        Curr_Buff_Block = 2;
        Draw_Pattern_Right_Stuff();
        Actupated(0);
        teac = 0;
        gui_action = GUI_CMD_UPDATE_SEQUENCER;
    }
    // Set buffer 4
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, (Cur_Height - 251) + (16 * 4) + Patterns_Lines_Offset, 16 + 1, 14))
    {
        Curr_Buff_Block = 3;
        Draw_Pattern_Right_Stuff();
        Actupated(0);
        teac = 0;
        gui_action = GUI_CMD_UPDATE_SEQUENCER;
    }

    // Track mute
    start_mute_check_x = PAT_COL_NOTE + 1 + 4 + 1;
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(zcheckMouse(start_mute_check_x + Cur_Char_size[i], 183, 28, 7))
        {
            gui_action = GUI_CMD_SWITCH_TRACK_MUTE_STATE;
            break;
        }
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // Track on/off
    start_mute_check_x = PAT_COL_NOTE + 1 + 4 + 29 + 1;
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(zcheckMouse(start_mute_check_x + Cur_Char_size[i], 183, 28, 7))
        {
            int Cur_Position = Get_Song_Position();
            int tmp_track = Get_Track_Over_Mouse(Mouse.x, NULL, FALSE);
            Toggle_Track_On_Off_Status(Cur_Position, tmp_track);
            break;
        }
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // Track zoom > large
    start_mute_check_x = PAT_COL_NOTE + 1 + 4 + (29 * 2) + 1;
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(zcheckMouse(start_mute_check_x + Cur_Char_size[i], 183, 16, 7))
        {
            gui_action = GUI_CMD_SWITCH_TRACK_LARGE_STATE;
            break;
        }
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // Reduce notes
    start_mute_check_x = PAT_COL_NOTE + 4 + 1;
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(zcheckMouse(start_mute_check_x , 183 + 8, 8, 7))
        {
            gui_action = GUI_CMD_REDUCE_TRACK_NOTES;
            break;
        }
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // Expand notes
    start_mute_check_x = PAT_COL_NOTE + 4 + 1 + 9;
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(zcheckMouse(start_mute_check_x, 183 + 8, 8, 7))
        {
            gui_action = GUI_CMD_EXPAND_TRACK_NOTES;
            break;
        }
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // Reduce effects
    tracks = Get_Visible_Partial_Tracks();
    for(i = 0; i < tracks; i++)
    {
        start_mute_check_x = pos_effects_icons[i] - 1;
        if(zcheckMouse(start_mute_check_x, 183 + 8, 8, 7))
        {
            gui_action = GUI_CMD_REDUCE_TRACK_EFFECTS;
            break;
        }
    }

    // Expand effects
    tracks = Get_Visible_Partial_Tracks();
    for(i = 0; i < tracks; i++)
    {
        start_mute_check_x = pos_effects_icons[i] + 9 - 1;
        if(zcheckMouse(start_mute_check_x, 183 + 8, 8, 7))
        {
            gui_action = GUI_CMD_EXPAND_TRACK_EFFECTS;
            break;
        }
    }
}

// ------------------------------------------------------
// Handle the right mouse button event
void Mouse_Right_Pattern_Ed(void)
{
    int start_mute_check_x;
    int tracks;
    int i;

    // Decrease/Increase steps
    if(zcheckMouse(90, 126, 16, 16))
    {
        Current_Edit_Steps = 0;
        gui_action = GUI_CMD_UPDATE_PATTERN_ED;
    }
    if(zcheckMouse(134, 126, 16, 16))
    {
        Current_Edit_Steps = 16;
        gui_action = GUI_CMD_UPDATE_PATTERN_ED;
    }

    // Solo track
    start_mute_check_x = PAT_COL_NOTE + 1 + 4;
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(start_mute_check_x + Cur_Char_size[i] >= MAX_PATT_SCREEN_X) break;
        if(zcheckMouse(start_mute_check_x + Cur_Char_size[i], 183, 28, 7))
        {
            int tmp_track = Get_Track_Over_Mouse(Mouse.x, NULL, FALSE);
            Solo_Track(tmp_track);
            // Will unmute the correct track
            gui_action = GUI_CMD_SWITCH_TRACK_MUTE_STATE;
        }
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // All tracks off but the selected one
    start_mute_check_x = PAT_COL_NOTE + 1 + 4 + 29;
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(start_mute_check_x + Cur_Char_size[i] >= MAX_PATT_SCREEN_X) break;
        if(zcheckMouse(start_mute_check_x + Cur_Char_size[i], 183, 28, 7))
        {
            int Cur_Position = Get_Song_Position();
            int tmp_track = Get_Track_Over_Mouse(Mouse.x, NULL, FALSE);
            Solo_Track_On_Off(Cur_Position, tmp_track);
            break;
        }
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // Track zoom > small
    start_mute_check_x = PAT_COL_NOTE + 1 + 4 + (29 * 2);
    tracks = Get_Visible_Partial_Tracks();
    for(i = gui_track; i < gui_track + tracks; i++)
    {
        if(start_mute_check_x + Cur_Char_size[i] >= MAX_PATT_SCREEN_X) break;
        if(zcheckMouse(start_mute_check_x + Cur_Char_size[i], 183, 16, 7)) gui_action = GUI_CMD_SWITCH_TRACK_SMALL_STATE;
        start_mute_check_x += Get_Track_Size(i, NULL);
    }

    // Prev page
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, 184, 16 + 1, 14) & !Song_Playing)
    {
        Goto_Previous_Page(FALSE);
    }

    // Next page
    if(zcheckMouse(MAX_PATT_SCREEN_X + 1, (Cur_Height - 251) + Patterns_Lines_Offset, 16 + 1, 14) & !Song_Playing)
    {
        Goto_Next_Page(FALSE);
    }
}

// ------------------------------------------------------
// Mute all tracks but a given one
void Solo_Track(int track_to_solo)
{
    // Unmute all if the user clicked on a solo track
    if(Chan_Mute_State[track_to_solo] == 0)
    {
        int Was_Solo = FALSE;
        
        // Check if all other tracks are muted
        for(int solify = 0; solify < MAX_TRACKS; solify++)
        {
            if((solify != track_to_solo) && Chan_Mute_State[solify] == 0)
            {
                Was_Solo = TRUE;
            }
        }
        if(!Was_Solo)
        {
            // Unmute all
            for(int solify = 0; solify < MAX_TRACKS; solify++)
            {
                Chan_Mute_State[solify] = 0;
            }
            Chan_Mute_State[track_to_solo] = 1;
        }
        else
        {
            // Else mute all
            for(int solify = 0; solify < MAX_TRACKS; solify++)
            {
                Chan_Mute_State[solify] = 1;
            }
        }
    }
    else
    {
        // Else mute all
        for(int solify = 0; solify < MAX_TRACKS; solify++)
        {
            Chan_Mute_State[solify] = 1;
        }
    }
}

// ------------------------------------------------------
// Move one row above
void Goto_Previous_Row(int Remove_Sel)
{
    int Cur_Position = Get_Song_Position();

    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
    Pattern_Line--;
    if(Continuous_Scroll && !Cur_Position) if(Pattern_Line < 0) Pattern_Line = 0;
    Actupated(0);
    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
}

// ------------------------------------------------------
// Move one row below
void Goto_Next_Row(int Remove_Sel)
{
    int Cur_Position = Get_Song_Position();

    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
    Pattern_Line++;
    if(Continuous_Scroll && (Cur_Position == Song_Length - 1)) if(Pattern_Line >= patternLines[pSequence[Cur_Position]]) Pattern_Line = patternLines[pSequence[Cur_Position]] - 1;
    Actupated(0);
    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
}

// ------------------------------------------------------
// Move one page above
void Goto_Previous_Page(int Remove_Sel)
{
    int Cur_Position = Get_Song_Position();

    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
    Pattern_Line -= 16;
    if(!is_recording && !Continuous_Scroll) if(Pattern_Line < 0) Pattern_Line = 0;
    if(Continuous_Scroll && !Cur_Position) if(Pattern_Line < 0) Pattern_Line = 0;
    Actupated(0);
    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
}

// ------------------------------------------------------
// Move one page below
void Goto_Next_Page(int Remove_Sel)
{
    int Cur_Position = Get_Song_Position();

    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
    Pattern_Line += 16;
    if(!is_recording && !Continuous_Scroll) if(Pattern_Line >= patternLines[pSequence[Cur_Position]]) Pattern_Line = patternLines[pSequence[Cur_Position]] - 1;
    if(Continuous_Scroll && (Cur_Position == Song_Length - 1)) if(Pattern_Line >= patternLines[pSequence[Cur_Position]]) Pattern_Line = patternLines[pSequence[Cur_Position]] - 1;
    Actupated(0);
    if(Remove_Sel) Select_Block_Keyboard(BLOCK_MARK_ROWS);
}

// ------------------------------------------------------
// Move one column on the left
void Goto_Previous_Column(void)
{
    Select_Block_Keyboard(BLOCK_MARK_TRACKS);
    Column_Under_Caret--;
    Actupated(0);
    Select_Block_Keyboard(BLOCK_MARK_TRACKS);
    gui_action = GUI_CMD_SET_FOCUS_TRACK;
}

// ------------------------------------------------------
// Move one column on the right
void Goto_Next_Column(void)
{
    Select_Block_Keyboard(BLOCK_MARK_TRACKS);
    Column_Under_Caret++;
    Actupated(0);
    Select_Block_Keyboard(BLOCK_MARK_TRACKS);
    gui_action = GUI_CMD_SET_FOCUS_TRACK;
}

// ------------------------------------------------------
// Move to the top left of the pattern
void Goto_Top_Left(void)
{
    Select_Block_Keyboard(BLOCK_MARK_ROWS | BLOCK_MARK_TRACKS);
    if(!Get_LCtrl()) Pattern_Line = 0;
    else
    {
        Column_Under_Caret = 0;
        Track_Under_Caret = 0;
    }
    Actupated(0);
    Select_Block_Keyboard(BLOCK_MARK_ROWS | BLOCK_MARK_TRACKS);
    gui_action = GUI_CMD_SET_FOCUS_TRACK;
}

// ------------------------------------------------------
// Move to the bottom right of the pattern
void Goto_Bottom_Right(void)
{
    Select_Block_Keyboard(BLOCK_MARK_ROWS | BLOCK_MARK_TRACKS);
    if(!Get_LCtrl()) Pattern_Line = patternLines[pSequence[Get_Song_Position()]] - 1;
    else
    {
        Column_Under_Caret = 0;
        Track_Under_Caret = Songtracks - 1;
    }
    Actupated(0);
    gui_action = GUI_CMD_SET_FOCUS_TRACK;
    Select_Block_Keyboard(BLOCK_MARK_ROWS | BLOCK_MARK_TRACKS);
}

// ------------------------------------------------------
// Move to a given row of the pattern
void Goto_Row(int row)
{
    Select_Block_Keyboard(BLOCK_MARK_ROWS);
    Pattern_Line = row;
    Actupated(0);
    Select_Block_Keyboard(BLOCK_MARK_ROWS);
}

// ------------------------------------------------------
// Go to a given song position
void Goto_Song_Position(int Position)
{
    Song_Position = Position;
    Bound_Patt_Pos();
    Actualize_Sequencer();
    Actupated(0);
}

// ------------------------------------------------------
// Return the current sequence position
int Get_Song_Position(void)
{
    if(Song_Playing) return(Song_Position_Visual);
    else return(Song_Position);
}

// ------------------------------------------------------
// Return the current pattern line
int Get_Pattern_Line(void)
{
    if(Song_Playing) return(Pattern_Line_Visual);
    else return(Pattern_Line);
}

// ------------------------------------------------------
// Return the current row in the entire song
int Get_Song_Line(void)
{
    int i;
    int Cur_Lines_Song;

    int Start_Pos = Get_Song_Position();
    Cur_Lines_Song = 0;
    for(i = 0; i < Start_Pos; i++)
    {
        Cur_Lines_Song += patternLines[pSequence[i]];
    }
    if(Song_Playing) return(Cur_Lines_Song + Pattern_Line_Visual);
    else return(Cur_Lines_Song + Pattern_Line);
}

// ------------------------------------------------------
// Set all the tracks to small size font
void Reset_Patterns_Zoom(void)
{
    int i;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        Set_Track_Zoom(i, (TRACK_TYPE) Global_Patterns_Font);
    }
}

// ------------------------------------------------------
// Toggle the zoom status of a track
void Toggle_Track_Zoom(int track, int large)
{
    if(large)
    {
        if(Get_Track_Zoom(track) == TRACK_LARGE)
        {
            Set_Track_Zoom(track, TRACK_MEDIUM);
        }
        else
        {
            Set_Track_Zoom(track, TRACK_LARGE);
        }
    }
    else
    {
        if(Get_Track_Zoom(track) == TRACK_SMALL)
        {
            Set_Track_Zoom(track, TRACK_MEDIUM);
        }
        else
        {
            Set_Track_Zoom(track, TRACK_SMALL);
        }
    }
}

// ------------------------------------------------------
// Return the type of a track
TRACK_TYPE Get_Track_Zoom(int track)
{
    switch(Cur_Char_size[track])
    {
        case PAT_COL_CHAR:
            return(TRACK_MEDIUM);

        case PAT_COL_CHAR_LARGE:
            return(TRACK_LARGE);

        case PAT_COL_CHAR_SMALL:
            return(TRACK_SMALL);

        default:
            return(TRACK_MEDIUM);
    }
}

// ------------------------------------------------------
// Force a track to be zoomed out
void Clear_Track_Zoom(int track)
{
    Cur_Char_size[track] = PAT_COL_CHAR;
    Cur_Char_Function[track].Fnc = Letter;
    Cur_Note_Function[track].Fnc = blitnote;
}

// ------------------------------------------------------
// Set the zoom status of a track
void Set_Track_Zoom(int track, TRACK_TYPE type)
{
    switch(type)
    {
        case TRACK_SMALL:
            Cur_Char_size[track] = PAT_COL_CHAR_SMALL;
            Cur_Char_Function[track].Fnc = Small_Letter;
            Cur_Note_Function[track].Fnc = blitsmallnote;
            break;

        case TRACK_MEDIUM:
            Clear_Track_Zoom(track);
            if(Get_Track_Real_Size(track) >= TRACKS_WIDTH)
            {
                // Track is too damn large to be displayed with medium font
                Set_Track_Zoom(track, TRACK_SMALL);
            }
            break;

        case TRACK_LARGE:
            Cur_Char_size[track] = PAT_COL_CHAR_LARGE;
            Cur_Char_Function[track].Fnc = Large_Letter;
            Cur_Note_Function[track].Fnc = blitlargenote;
            if(Get_Track_Real_Size(track) >= TRACKS_WIDTH)
            {
                // Track is too damn large to be displayed with large font
                Set_Track_Zoom(track, TRACK_MEDIUM);
            }
            break;
    }
}

// ------------------------------------------------------
// Define the number of lines to display for a pattern
void Set_Pattern_Size()
{
    if(Large_Patterns)
    {
        Patterns_Lines_Offset = 132;
        DISPLAYED_LINES = (Cur_Height - 256) / 8 + 1;
    }
    else
    {
        Patterns_Lines_Offset = 0;
        DISPLAYED_LINES = (Cur_Height - 388) / 8 + 1;
    }
    VIEWLINE = (DISPLAYED_LINES / 2);
}
