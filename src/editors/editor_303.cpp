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

// ------------------------------------------------------
// Includes
#include "include/editor_303.h"
#include "../files/include/files.h"
#include "../ui/include/requesters.h"

// ------------------------------------------------------
// Variables
SDL_Surface *SKIN303;

int Current_copy_buffer;
int Copied_Buffer[4];

int tb303_pattern_buffer_full[4];
flag303 tb303_pattern_buffer[4][16];
unsigned char tb303_buffer_tone[4][16];

unsigned char editsteps[2][32];

int Refresh_Unit;

extern REQUESTER Overwrite_Requester;

// ------------------------------------------------------
// Fucntions
void tb303_copy_pattern(void);
void tb303_paste_pattern(void);
void tb303_notes_up(void);
void tb303_notes_down(void);
void Display_Cur_copy_Buffer(void);

void Draw_303_Ed(void)
{
    Draw_Editors_Bar(USER_SCREEN_TB303_EDIT);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, "", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("303 Units");

#ifndef __LITE__
    Gui_Draw_Button_Box(8, (Cur_Height - 134), 64, 16, "Reset Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(8, (Cur_Height - 116), 64, 16, "Clear Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(8, (Cur_Height - 98), 64, 16, "Rnd.NoteOn", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(8, (Cur_Height - 80), 64, 16, "Rand.Tones", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(8, (Cur_Height - 62), 64, 16, "Rand.Flags", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(600, (Cur_Height - 98), 64, 16, "All Notes Up", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(668, (Cur_Height - 98), 64, 16, "All Notes Dn", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(600, (Cur_Height - 78), 64, 16, "Copy Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(600, (Cur_Height - 60), 64, 16, "Paste Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(600, (Cur_Height - 138), 56, 16, "Patt. Name", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(658, (Cur_Height - 138), 34, 16, "Save", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Skincopy(80, (Cur_Height - 140), 0, 0, 510, 114);

    Gui_Draw_Button_Box(668, (Cur_Height - 60), 64, 16, "Scale", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_NO_BORDER | BUTTON_TEXT_CENTERED);
#else
    Gui_Draw_Button_Box(8, (Cur_Height - 134), 64, 16, "Reset Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(8, (Cur_Height - 116), 64, 16, "Clear Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(8, (Cur_Height - 98), 64, 16, "Rnd.NoteOn", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(8, (Cur_Height - 80), 64, 16, "Rand.Tones", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(8, (Cur_Height - 62), 64, 16, "Rand.Flags", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);

    Gui_Draw_Button_Box(600, (Cur_Height - 98), 64, 16, "All Notes Up", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(668, (Cur_Height - 98), 64, 16, "All Notes Dn", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(600, (Cur_Height - 78), 64, 16, "Copy Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(600, (Cur_Height - 60), 64, 16, "Paste Patt.", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);

    Gui_Draw_Button_Box(600, (Cur_Height - 138), 56, 16, "Patt. Name", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_DISABLED);
    Gui_Draw_Button_Box(658, (Cur_Height - 138), 34, 16, "Save", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);

    Skincopy(80, (Cur_Height - 140), 0, 0, 510, 114);

    Gui_Draw_Button_Box(668, (Cur_Height - 60), 64, 16, "Scale", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_NO_BORDER | BUTTON_TEXT_CENTERED);
#endif

    Actualize_303_Ed(0);
}

void Actualize_303_Ed(char gode)
{
    if(userscreen == USER_SCREEN_TB303_EDIT)
    {
        Refresh_303_Unit(sl3, gode);

        if(gode == 0 || gode == 17)
        {
            Display_Cur_copy_Buffer();
        }
    }
}

void Refresh_303_Unit(int Unit, int gode)
{
    char tcp[40];

    if(userscreen == USER_SCREEN_TB303_EDIT)
    {
    
        if(gode == 0 ||
           gode == 1)
        {
            number303(tb303[Unit].patternlength[tb303[Unit].selectedpattern], 118, (Cur_Height - 44));
        }

        // Selected bassline
        if(gode == 0)
        {
            if(Unit)
            {
                Skincopy(577, (Cur_Height - 58), 138, 119, 3, 3);
                Skincopy(558, (Cur_Height - 58), 143, 119, 3, 3);
#ifndef __LITE__
                Gui_Draw_Button_Box(668, (Cur_Height - 78), 64, 16, "Tune to 1", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
#else
                Gui_Draw_Button_Box(668, (Cur_Height - 78), 64, 16, "Tune to 1", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
#endif
            }
            else
            {
                Skincopy(558, (Cur_Height - 58), 138, 119, 3, 3);
                Skincopy(577, (Cur_Height - 58), 143, 119, 3, 3);
#ifndef __LITE__
                Gui_Draw_Button_Box(668, (Cur_Height - 78), 64, 16, "Tune to 2", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
#else
                Gui_Draw_Button_Box(668, (Cur_Height - 78), 64, 16, "Tune to 2", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
#endif
            }

            // Make sure we display the right boundaries
            if(editsteps[Unit][tb303[Unit].selectedpattern] > tb303[Unit].patternlength[tb303[Unit].selectedpattern] - 1)
            {
                editsteps[Unit][tb303[Unit].selectedpattern] = tb303[Unit].patternlength[tb303[Unit].selectedpattern] - 1;
            }

            number303(editsteps[Unit][tb303[Unit].selectedpattern] + 1, 486, (Cur_Height - 114));
            // Displaying pattern selection leds
            // Bank [A-D]
            int tbank303 = tb303[Unit].selectedpattern / 8;
            tbank303 *= 15;
            // Restoring background
            Skincopy(86, (Cur_Height - 74), 6, 66, 59, 14);
            // Light the bank
            Skincopy(86 + tbank303, (Cur_Height - 74), 15 + tbank303, 117, 14, 14);
            // Displaying pattern selection leds
            // Pattern [1-8]
            int tpat303 = tb303[Unit].selectedpattern - (tb303[Unit].selectedpattern / 8) * 8;
            // Restoring background
            Skincopy(86, (Cur_Height - 116), 6, 24, 59, 30);
            // Light the bank
            if(tpat303 < 4)
            {
                Skincopy(86 + tpat303 * 15, (Cur_Height - 116), 75 + tpat303 * 15, 117, 14, 14);
            }
            else
            {
                tpat303 -= 4;
                Skincopy(86 + tpat303 * 15, (Cur_Height - 101), 75 + tpat303 * 15, 132, 14, 14);
            }
        }

        // Displaying waveform switch
        if(gode == 0 || gode == 2)
        {
            if(tb303[Unit].waveform) Skincopy(180, (Cur_Height - 128), 137, 135, 13, 8);
            else Skincopy(180, (Cur_Height - 128), 137, 125, 13, 8);
        }

        // Draw 303 Knobs
        if(gode == 0 || gode == 3) knob(229, (Cur_Height - 124), tb303[Unit].tune / 2);
        if(gode == 0 || gode == 4) knob(262, (Cur_Height - 124), tb303[Unit].cutoff / 2);
        if(gode == 0 || gode == 5) knob(295, (Cur_Height - 124), tb303[Unit].resonance / 2);
        if(gode == 0 || gode == 6) knob(328, (Cur_Height - 124), tb303[Unit].envmod / 2);
        if(gode == 0 || gode == 7) knob(361, (Cur_Height - 124), tb303[Unit].decay / 2);
        if(gode == 0 || gode == 8) knob(394, (Cur_Height - 124), tb303[Unit].accent / 2);

        // Restoring notes background
        if(gode == 0 || gode == 9)
        {
            // Restore it
            Skincopy(88 + 80, 66 + (Cur_Height - 140), 88, 66, 195, 40);

            // Light a note
            switch(tb303[Unit].tone[tb303[Unit].selectedpattern][editsteps[Unit][tb303[Unit].selectedpattern]])
            {
                case 0: Skincopy(88 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 1: Skincopy(101 + 80, 66 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 2: Skincopy(114 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 3: Skincopy(127 + 80, 66 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 4: Skincopy(140 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 5: Skincopy(166 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 6: Skincopy(179 + 80, 66 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 7: Skincopy(191 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 8: Skincopy(204 + 80, 66 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 9: Skincopy(217 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 10: Skincopy(230 + 80, 66 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 11: Skincopy(243 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
                case 12: Skincopy(269 + 80, 91 + (Cur_Height - 140), 301, 119, 13, 13); break;
            }
        }

        // Light pause/note led
        if(gode == 0 || gode == 10)
        {
            if(tb303[Unit].flag[tb303[Unit].selectedpattern][editsteps[Unit][tb303[Unit].selectedpattern]].pause)
            {
                Skincopy(402, (Cur_Height - 79), 138, 119, 3, 3);
                Skincopy(439, (Cur_Height - 79), 143, 119, 3, 3);
            }
            else
            {
                Skincopy(402, (Cur_Height - 79), 143, 119, 3, 3);
                Skincopy(439, (Cur_Height - 79), 138, 119, 3, 3);
            }
        }

        // Light slide/off led
        if(gode == 0 || gode == 11)
        {
            if(tb303[Unit].flag[tb303[Unit].selectedpattern][editsteps[Unit][tb303[Unit].selectedpattern]].slide_flag)
            {
                Skincopy(456, (Cur_Height - 57), 138, 119, 3, 3);
            }
            else
            {
                Skincopy(456, (Cur_Height - 57), 143, 119, 3, 3);
            }
        }

        // Light accent/off led
        if(gode == 0 || gode == 12)
        {
            if(tb303[Unit].flag[tb303[Unit].selectedpattern][editsteps[Unit][tb303[Unit].selectedpattern]].accent_flag)
            {
                Skincopy(431, (Cur_Height - 57), 138, 119, 3, 3);
            }
            else
            {
                Skincopy(431, (Cur_Height - 57), 143, 119, 3, 3);
            }
        }

        // Transpose up flag
        if(gode == 0 || gode == 13)
        {
            if(tb303[Unit].flag[tb303[Unit].selectedpattern][editsteps[Unit][tb303[Unit].selectedpattern]].transposeup_flag)
            {
                Skincopy(406, (Cur_Height - 57), 138, 119, 3, 3);
            }
            else
            {
                Skincopy(406, (Cur_Height - 57), 143, 119, 3, 3);
            }
        }

        // Transpose down flag
        if(gode == 0 || gode == 14)
        {
            if(tb303[Unit].flag[tb303[Unit].selectedpattern][editsteps[Unit][tb303[Unit].selectedpattern]].transposedown_flag)
            {
                Skincopy(381, (Cur_Height - 57), 138, 119, 3, 3);
            }
            else
            {
                Skincopy(381, (Cur_Height - 57), 143, 119, 3, 3);
            }
        }

        // Volume
        if(gode == 0 || gode == 15)
        {
            // volume background
            Skincopy(529, (Cur_Height - 115), 449, 25, 19, 88);
            int tb303v = (int) (tb303engine[Unit].tbVolume * 72.0f);
            // Volume slider
            Skincopy(531, (Cur_Height - 42) - tb303v, 0, 116, 13, 11);
        }

        if(gode == 0 || gode == 18)
        {
            sprintf(tcp, "%s_", tb303[Unit].pattern_name[tb303[Unit].selectedpattern]);

            if(snamesel == INPUT_303_PATTERN)
            {
#ifndef __LITE__
                Gui_Draw_Button_Box(600, (Cur_Height - 120), 164, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT);
#else
                Gui_Draw_Button_Box(600, (Cur_Height - 120), 164, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT | BUTTON_DISABLED);
#endif
            }
            else
            {
#ifndef __LITE__
                Gui_Draw_Button_Box(600, (Cur_Height - 120), 164, 16, tb303[Unit].pattern_name[tb303[Unit].selectedpattern], BUTTON_NORMAL | BUTTON_INPUT);
#else
                Gui_Draw_Button_Box(600, (Cur_Height - 120), 164, 16, tb303[Unit].pattern_name[tb303[Unit].selectedpattern], BUTTON_NORMAL | BUTTON_INPUT | BUTTON_DISABLED);
#endif
            }
        }

        if(gode == 0 || gode == 19)
        {
#ifndef __LITE__
            Gui_Draw_Arrows_Number_Box2(670, (Cur_Height - 42), tb303[Unit].scale, BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
#else
            Gui_Draw_Arrows_Number_Box2(670, (Cur_Height - 42), tb303[Unit].scale, BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
#endif
        }

    }
}

void number303(unsigned char number, int x, int y)
{
    switch(number)
    {
        case 0: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 156, 118, 7, 14); break;
        case 1: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 164, 118, 7, 14); break;
        case 2: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 172, 118, 7, 14); break;
        case 3: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 180, 118, 7, 14); break;
        case 4: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 188, 118, 7, 14); break;
        case 5: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 196, 118, 7, 14); break;
        case 6: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 204, 118, 7, 14); break;
        case 7: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 212, 118, 7, 14); break;
        case 8: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 220, 118, 7, 14); break;
        case 9: Skincopy(x, y, 156, 118, 7, 14); Skincopy(x + 8, y, 228, 118, 7, 14); break;
        case 10: Skincopy(x, y, 164, 118, 7, 14); Skincopy(x + 8, y, 156, 118, 7, 14); break;
        case 11: Skincopy(x, y, 164, 118, 7, 14); Skincopy(x + 8, y, 164, 118, 7, 14); break;
        case 12: Skincopy(x, y, 164, 118, 7, 14); Skincopy(x + 8, y, 172, 118, 7, 14); break;
        case 13: Skincopy(x, y, 164, 118, 7, 14); Skincopy(x + 8, y, 180, 118, 7, 14); break;
        case 14: Skincopy(x, y, 164, 118, 7, 14); Skincopy(x + 8, y, 188, 118, 7, 14); break;
        case 15: Skincopy(x, y, 164, 118, 7, 14); Skincopy(x + 8, y, 196, 118, 7, 14); break;
        case 16: Skincopy(x, y, 164, 118, 7, 14); Skincopy(x + 8, y, 204, 118, 7, 14); break;
    }
}

void knob(int x, int y, unsigned char number)
{
    if(number > 62) number = 62;
    Skincopy(x, y, number * 25, 147, 24, 24);
}

void Mouse_Right_303_Ed(void )
{
#ifndef __LITE__
    if(userscreen == USER_SCREEN_TB303_EDIT)
    {
        // Volume Slider
        if(zcheckMouse(529, (Cur_Height - 115), 19, 88))
        {
            tb303engine[sl3].tbVolume = 0.5f;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 15;
            liveparam = LIVE_PARAM_303_1_VOLUME + sl3;
            livevalue = (int) (tb303engine[sl3].tbVolume * 255.0f);
        }

        // Tune Knob
        if(zcheckMouse(229, (Cur_Height - 124), 24, 24))
        {
            int tempz = 64;
            tb303[sl3].tune = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 3;
            liveparam = LIVE_PARAM_303_1_TUNE + sl3;
            livevalue = tempz << 1;
        }

        // CutOff Knob
        if(zcheckMouse(262, (Cur_Height - 124), 24, 24))
        {
            int tempz = 64;
            tb303[sl3].cutoff = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 4;
            liveparam = LIVE_PARAM_303_1_CUTOFF + sl3;
            livevalue = tempz << 1;
        }

        // Reso knob
        if(zcheckMouse(295, (Cur_Height - 124), 24, 24))
        {
            int tempz = 64;
            tb303[sl3].resonance = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 5;
            liveparam = LIVE_PARAM_303_1_RESONANCE + sl3;
            livevalue = tempz << 1;
        }

        // Envmod knob
        if(zcheckMouse(328, (Cur_Height - 124), 24, 24))
        {
            int tempz = 64;
            tb303[sl3].envmod = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 6;
            liveparam = LIVE_PARAM_303_1_ENVMOD + sl3;
            livevalue = tempz << 1;
        }

        // Decay knob
        if(zcheckMouse(361, (Cur_Height - 124), 24, 24))
        {
            int tempz = 64;
            tb303[sl3].decay = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 7;
            liveparam = LIVE_PARAM_303_1_DECAY + sl3;
            livevalue = tempz << 1;
        }

        // Accent knob
        if(zcheckMouse(394, (Cur_Height - 124), 24, 24))
        {
            int tempz = 64;
            tb303[sl3].accent = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 8;
            liveparam = LIVE_PARAM_303_1_ACCENT + sl3;
            livevalue = tempz << 1;
        }

        // Scale
        if(zcheckMouse(670, (Cur_Height - 42), 16, 16))
        {
            int value = tb303[sl3].scale;
            value -= 4;
            if(value < 1) value = 1;
            tb303[sl3].scale = value;
            tb303engine[sl3].tbCurMultiple = tb303[sl3].scale;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 19;
        }

        // Scale
        if(zcheckMouse(670 + 44, (Cur_Height - 42), 16, 16))
        {
            tb303[sl3].scale += 4;
            if(tb303[sl3].scale > 16) tb303[sl3].scale = 16;
            tb303engine[sl3].tbCurMultiple = tb303[sl3].scale;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 19;
        }
    }
#endif
}

void Mouse_Wheel_303_Ed(int roll_amount)
{
#ifndef __LITE__
    if(userscreen == USER_SCREEN_TB303_EDIT)
    {
        // Volume Knob
        if(zcheckMouse(529, (Cur_Height - 115), 19, 88))
        {
            float froll = roll_amount / 86.0f;
            float breakvol = tb303engine[sl3].tbVolume + froll;
            if(breakvol < 0.0f) breakvol = 0.0f;
            if(breakvol > 1.0f) breakvol = 1.0f;
            tb303engine[sl3].tbVolume = breakvol;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            liveparam = LIVE_PARAM_303_1_VOLUME + sl3;
            livevalue = (int) (breakvol * 255.0f);
            teac = 15;
        }

        // Tune Knob
        if(zcheckMouse(229, (Cur_Height - 124), 24, 24))
        {
            int tempz = tb303[sl3].tune + roll_amount;
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].tune = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 3;
            liveparam = LIVE_PARAM_303_1_TUNE + sl3;
            livevalue = tempz << 1;
        }

        // CutOff Knob
        if(zcheckMouse(262, (Cur_Height - 124), 24, 24))
        {
            int tempz = tb303[sl3].cutoff + roll_amount;
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].cutoff = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 4;
            liveparam = LIVE_PARAM_303_1_CUTOFF + sl3;
            livevalue = tempz << 1;
        }

        // Reso knob
        if(zcheckMouse(295, (Cur_Height - 124), 24, 24))
        {
            int tempz = tb303[sl3].resonance + roll_amount;
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].resonance = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 5;
            liveparam = LIVE_PARAM_303_1_RESONANCE + sl3;
            livevalue = tempz << 1;
        }

        // Envmod knob
        if(zcheckMouse(328, (Cur_Height - 124), 24, 24))
        {
            int tempz = tb303[sl3].envmod + roll_amount;
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].envmod = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 6;
            liveparam = LIVE_PARAM_303_1_ENVMOD + sl3;
            livevalue = tempz << 1;
        }

        // Decay knob
        if(zcheckMouse(361, (Cur_Height - 124), 24, 24))
        {
            int tempz = tb303[sl3].decay + roll_amount;
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].decay = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 7;
            liveparam = LIVE_PARAM_303_1_DECAY + sl3;
            livevalue = tempz << 1;
        }

        // Accent knob
        if(zcheckMouse(394, (Cur_Height - 124), 24, 24))
        {
            int tempz = tb303[sl3].accent + roll_amount;
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].accent = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 8;
            liveparam = LIVE_PARAM_303_1_ACCENT + sl3;
            livevalue = tempz << 1;
        }
    }
#endif
}

void Mouse_Left_303_Ed(void)
{
#ifndef __LITE__
    if(userscreen == USER_SCREEN_TB303_EDIT)
    {
        // Step Forward
        if(zcheckMouse(479, (Cur_Height - 50), 25, 16))
        {
            editsteps[sl3][tb303[sl3].selectedpattern]++;
            if(editsteps[sl3][tb303[sl3].selectedpattern] > tb303[sl3].patternlength[tb303[sl3].selectedpattern] - 1)
            {
                editsteps[sl3][tb303[sl3].selectedpattern] = 0;
            }
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }

        // Step Back
        if(zcheckMouse(484, (Cur_Height - 78), 15, 10))
        {
            if(editsteps[sl3][tb303[sl3].selectedpattern] == 0) editsteps[sl3][tb303[sl3].selectedpattern] = tb303[sl3].patternlength[tb303[sl3].selectedpattern] - 1;
            else editsteps[sl3][tb303[sl3].selectedpattern]--;
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Pattern Steps +1
        if(zcheckMouse(135, (Cur_Height - 45), 11, 9) &&
           tb303[sl3].patternlength[tb303[sl3].selectedpattern] < 16)
        {
            tb303[sl3].patternlength[tb303[sl3].selectedpattern]++;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }

        // Pattern Steps -1
        if(zcheckMouse(135, (Cur_Height - 36), 11, 9) &&
           tb303[sl3].patternlength[tb303[sl3].selectedpattern] > 1)
        {
            tb303[sl3].patternlength[tb303[sl3].selectedpattern]--;
            if(editsteps[sl3][tb303[sl3].selectedpattern] > tb303[sl3].patternlength[tb303[sl3].selectedpattern] - 1)
            {
                editsteps[sl3][tb303[sl3].selectedpattern] = tb303[sl3].patternlength[tb303[sl3].selectedpattern] - 1;
            }
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }

        // Pause/Note button
        if(zcheckMouse(450, (Cur_Height - 82), 15, 10))
        {
            if(tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].pause)
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].pause = 0;
            }
            else
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].pause = 1;
            }
            teac = 10;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Glide button
        if(zcheckMouse(454, (Cur_Height - 50), 10, 15))
        {
            if(tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].slide_flag)
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].slide_flag = 0;
            }
            else
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].slide_flag = 1;
            }
            teac = 11;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Accent button
        if(zcheckMouse(428, (Cur_Height - 50), 10, 15))
        {
            if(tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].accent_flag)
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].accent_flag = 0;
            }
            else
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].accent_flag = 1;
            }
            teac = 12;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Transpose up button
        if(zcheckMouse(403, (Cur_Height - 50), 10, 15))
        {
            if(tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].transposeup_flag)
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].transposeup_flag = 0;
            }
            else
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].transposeup_flag = 1;
            }
            teac = 13;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Transpose down button
        if(zcheckMouse(378, (Cur_Height - 50), 10, 15))
        {
            if(tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].transposedown_flag)
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].transposedown_flag = 0;
            }
            else
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]].transposedown_flag = 1;
            }
            teac = 14;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Sawtooth/Square switching
        if(zcheckMouse(177, (Cur_Height - 129), 19, 10))
        {
            tb303[sl3].waveform ^= 1;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 2;
        }

        // NoteOn/Off buttons
        if(zcheckMouse(87 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(100 + 80, (Cur_Height - 76), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 1;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(113 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 2;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(126 + 80, (Cur_Height - 76), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 3;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(139 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 4;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(165 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 5;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(178 + 80, (Cur_Height - 76), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 6;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(190 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 7;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(203 + 80, (Cur_Height - 76), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 8;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(216 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 9;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }

        if(zcheckMouse(229 + 80, (Cur_Height - 76), 87, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 10;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(242 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 11;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }
        if(zcheckMouse(268 + 80, (Cur_Height - 50), 17, 15))
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][editsteps[sl3][tb303[sl3].selectedpattern]] = 12;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 9;
        }

        // Pattern selection buttons
        if(zcheckMouse(86, (Cur_Height - 117), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(100, (Cur_Height - 117), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp + 1;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(114, (Cur_Height - 117), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp + 2;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(130, (Cur_Height - 117), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp + 3;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(86, (Cur_Height - 102), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp + 4;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(100, (Cur_Height - 102), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp + 5;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(114, (Cur_Height - 102), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp + 6;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(130, (Cur_Height - 102), 16, 16))
        {
            char gcp = (tb303[sl3].selectedpattern / 8) * 8;
            tb303[sl3].selectedpattern = gcp + 7;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }

        // Bank selection buttons
        if(zcheckMouse(86, (Cur_Height - 75), 16, 16))
        {
            tb303[sl3].selectedpattern = (tb303[sl3].selectedpattern - (tb303[sl3].selectedpattern / 8) * 8);
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(100, (Cur_Height - 75), 16, 16))
        {
            tb303[sl3].selectedpattern = ((tb303[sl3].selectedpattern - (tb303[sl3].selectedpattern / 8) * 8)) + 8;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(114, (Cur_Height - 75), 16, 16))
        {
            tb303[sl3].selectedpattern = ((tb303[sl3].selectedpattern - (tb303[sl3].selectedpattern / 8) * 8)) + 16;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        if(zcheckMouse(130, (Cur_Height - 75), 16, 16))
        {
            tb303[sl3].selectedpattern = ((tb303[sl3].selectedpattern - (tb303[sl3].selectedpattern / 8) * 8)) + 24;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }

        // 303 #1
        if(zcheckMouse(553, (Cur_Height - 71), 15, 12))
        {
            sl3 = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }
        // 303 #2
        if(zcheckMouse(573, (Cur_Height - 71), 15, 12))
        {
            sl3 = 1;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 0;
        }

        // Rand tones
        if(zcheckMouse(8, (Cur_Height - 80), 64, 16))
        {
            for(char alter = 0; alter < 16; alter++)
            {
                tb303[sl3].tone[tb303[sl3].selectedpattern][alter] = (rand() % 13);
            }
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // rand flags
        if(zcheckMouse(8, (Cur_Height - 62), 64, 16))
        {
            for(char alter = 0; alter < 16; alter++)
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].slide_flag = rand() % 2;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposeup_flag = rand() % 2;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposedown_flag = rand() % 2;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].accent_flag = rand() % 2;
            }
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Rand noteon
        if(zcheckMouse(8, (Cur_Height - 98), 64, 16))
        {
            for(char alter = 0; alter < 16; alter++)
            {
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].pause = rand() % 2;
            }
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Clear pattern
        if(zcheckMouse(8, (Cur_Height - 116), 64, 16))
        {
            for(char alter = 0; alter < 16; alter++)
            {
                tb303[sl3].tone[tb303[sl3].selectedpattern][alter] = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].pause = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].slide_flag = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposeup_flag = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposedown_flag = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].accent_flag = 0;
            }
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Reset pattern
        if(zcheckMouse(8, (Cur_Height - 134), 64, 16))
        {
            for(char alter = 0; alter < 16; alter++)
            {
                tb303[sl3].tone[tb303[sl3].selectedpattern][alter] = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].pause = 1;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].slide_flag = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposeup_flag = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposedown_flag = 0;
                tb303[sl3].flag[tb303[sl3].selectedpattern][alter].accent_flag = 0;
            }
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // All notes up
        if(zcheckMouse(600, (Cur_Height - 98), 64, 16))
        {
            tb303_notes_up();
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // All notes down
        if(zcheckMouse(668, (Cur_Height - 98), 64, 16))
        {
            tb303_notes_down();
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Copy pattern
        if(zcheckMouse(600, (Cur_Height - 78), 64, 16))
        {
            tb303_copy_pattern();
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Paste pattern
        if(zcheckMouse(600, (Cur_Height - 60), 64, 16))
        {
            tb303_paste_pattern();
            teac = 0;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Select the copy buffers
        if(zcheckMouse(600, (Cur_Height - 42), 15, 16))
        {
            Current_copy_buffer = 0;
            teac = 17;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }
        if(zcheckMouse(616, (Cur_Height - 42), 15, 16))
        {
            Current_copy_buffer = 1;
            teac = 17;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }
        if(zcheckMouse(633, (Cur_Height - 42), 15, 16))
        {
            Current_copy_buffer = 2;
            teac = 17;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }
        if(zcheckMouse(649, (Cur_Height - 42), 15, 16))
        {
            Current_copy_buffer = 3;
            teac = 17;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
        }

        // Start name input
        if(zcheckMouse(600, (Cur_Height - 120), 164, 16) && snamesel == INPUT_NONE)
        {
            snamesel = INPUT_303_PATTERN;
            strcpy(cur_input_name, tb303[sl3].pattern_name[tb303[sl3].selectedpattern]);
            namesize = 0;
            sprintf(tb303[sl3].pattern_name[tb303[sl3].selectedpattern], "");
            teac = 18;
            gui_action = GUI_CMD_UPDATE_MIDI_303_ED;
        }

        // Save the data
        if(zcheckMouse(658, (Cur_Height - 138), 34, 16))
        {
            if(File_Exist_Req("%s"SLASH"%s.303", Dir_Patterns, tb303[sl3].pattern_name[tb303[sl3].selectedpattern]))
            {
                Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_303_PATTERN);
            }
            else
            {
                gui_action = GUI_CMD_SAVE_303_PATTERN;
            }
        }

        // Tune to the other unit
        if(zcheckMouse(668, (Cur_Height - 78), 64, 16))
        {
            if(sl3)
            {
                tb303[1].tune = tb303[0].tune;
            }
            else
            {
                tb303[0].tune = tb303[1].tune;
            }
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 3;
        }

        // Scale
        if(zcheckMouse(670, (Cur_Height - 42), 16, 16))
        {
            tb303[sl3].scale--;
            if(tb303[sl3].scale < 1) tb303[sl3].scale = 1;
            tb303engine[sl3].tbCurMultiple = tb303[sl3].scale;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 19;
        }

        // Scale
        if(zcheckMouse(670 + 44, (Cur_Height - 42), 16, 16))
        {
            tb303[sl3].scale++;
            if(tb303[sl3].scale > 16) tb303[sl3].scale = 16;
            tb303engine[sl3].tbCurMultiple = tb303[sl3].scale;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 19;
        }

    }
#endif
}

void Mouse_Sliders_303_Ed(void)
{
#ifndef __LITE__
    if(userscreen == USER_SCREEN_TB303_EDIT)
    {
        // Volume Knob
        if(zcheckMouse(529, (Cur_Height - 115), 19, 88))
        {
            float breakvol = (float) (80 - (Mouse.y - (Cur_Height - 115)));
            breakvol /= 72.0f;
            if(breakvol < 0.0f) breakvol = 0.0f;
            if(breakvol > 1.0f) breakvol = 1.0f;
            tb303engine[sl3].tbVolume = breakvol;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 15;
            liveparam = LIVE_PARAM_303_1_VOLUME + sl3;
            livevalue = (int) (breakvol * 255.0f);
        }

        // Tune Knob
        if(zcheckMouse(229, (Cur_Height - 124), 24, 24))
        {
            if(fluzy == -1) fluzy = (Mouse.y * 3) + tb303[sl3].tune;

            int tempz = fluzy - (Mouse.y * 3);
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].tune = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 3;
            liveparam = LIVE_PARAM_303_1_TUNE + sl3;
            livevalue = tempz << 1;
        }

        // CutOff Knob
        if(zcheckMouse(262, (Cur_Height - 124), 24, 24))
        {
            if(fluzy == -1) fluzy = (Mouse.y * 3) + tb303[sl3].cutoff;

            int tempz = fluzy - (Mouse.y * 3);
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].cutoff = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 4;
            liveparam = LIVE_PARAM_303_1_CUTOFF + sl3;
            livevalue = tempz << 1;
        }

        // Reso knob
        if(zcheckMouse(295, (Cur_Height - 124), 24, 24))
        {
            if(fluzy == -1) fluzy = (Mouse.y * 3) + tb303[sl3].resonance;

            int tempz = fluzy - (Mouse.y * 3);
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].resonance = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 5;
            liveparam = LIVE_PARAM_303_1_RESONANCE + sl3;
            livevalue = tempz << 1;
        }

        // Envmod knob
        if(zcheckMouse(328, (Cur_Height - 124), 24, 24))
        {
            if(fluzy == -1) fluzy = (Mouse.y * 3) + tb303[sl3].envmod;

            int tempz = fluzy - (Mouse.y * 3);
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].envmod = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 6;
            liveparam = LIVE_PARAM_303_1_ENVMOD + sl3;
            livevalue = tempz << 1;
        }

        // Decay knob
        if(zcheckMouse(361, (Cur_Height - 124), 24, 24))
        {
            if(fluzy == -1) fluzy = (Mouse.y * 3) + tb303[sl3].decay;

            int tempz = fluzy - (Mouse.y * 3);
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].decay = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 7;
            liveparam = LIVE_PARAM_303_1_DECAY + sl3;
            livevalue = tempz << 1;
        }

        // Accent knob
        if(zcheckMouse(394, (Cur_Height - 124), 24, 24))
        {
            if(fluzy == -1) fluzy = (Mouse.y * 3) + tb303[sl3].accent;

            int tempz = fluzy - (Mouse.y * 3);
            if(tempz < 0) tempz = 0;
            if(tempz > 127) tempz = 127;
            tb303[sl3].accent = tempz;
            gui_action = GUI_CMD_REFRESH_TB303_PARAMS;
            teac = 8;
            liveparam = LIVE_PARAM_303_1_ACCENT + sl3;
            livevalue = tempz << 1;
        }
    }
#endif
}

void Skincopy(int xd, int yd, int xs, int ys, int w, int h)
{
    Copy(SKIN303, xd, yd, xs, ys, xs + w, ys + h);
}

void tb303_copy_pattern(void)
{
#ifndef __LITE__
    tb303_pattern_buffer_full[Current_copy_buffer] = TRUE;
    for(char alter = 0; alter < 16; alter++)
    {
        tb303_buffer_tone[Current_copy_buffer][alter] = tb303[sl3].tone[tb303[sl3].selectedpattern][alter];
        tb303_pattern_buffer[Current_copy_buffer][alter].pause = tb303[sl3].flag[tb303[sl3].selectedpattern][alter].pause;
        tb303_pattern_buffer[Current_copy_buffer][alter].slide_flag = tb303[sl3].flag[tb303[sl3].selectedpattern][alter].slide_flag;
        tb303_pattern_buffer[Current_copy_buffer][alter].transposeup_flag = tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposeup_flag;
        tb303_pattern_buffer[Current_copy_buffer][alter].transposedown_flag = tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposedown_flag;
        tb303_pattern_buffer[Current_copy_buffer][alter].accent_flag = tb303[sl3].flag[tb303[sl3].selectedpattern][alter].accent_flag;
    }
    Copied_Buffer[Current_copy_buffer] = TRUE;
    Display_Cur_copy_Buffer();
#endif
}

void tb303_paste_pattern(void)
{
#ifndef __LITE__
    if(Copied_Buffer[Current_copy_buffer])
    {
        for(char alter = 0; alter < 16; alter++)
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][alter] = tb303_buffer_tone[Current_copy_buffer][alter];
            tb303[sl3].flag[tb303[sl3].selectedpattern][alter].pause = tb303_pattern_buffer[Current_copy_buffer][alter].pause;
            tb303[sl3].flag[tb303[sl3].selectedpattern][alter].slide_flag = tb303_pattern_buffer[Current_copy_buffer][alter].slide_flag;
            tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposeup_flag = tb303_pattern_buffer[Current_copy_buffer][alter].transposeup_flag;
            tb303[sl3].flag[tb303[sl3].selectedpattern][alter].transposedown_flag = tb303_pattern_buffer[Current_copy_buffer][alter].transposedown_flag;
            tb303[sl3].flag[tb303[sl3].selectedpattern][alter].accent_flag = tb303_pattern_buffer[Current_copy_buffer][alter].accent_flag;
        }
    }
#endif
}

void tb303_notes_up(void)
{
#ifndef __LITE__
    for(char alter = 0; alter < 16; alter++)
    {
        if(tb303[sl3].tone[tb303[sl3].selectedpattern][alter] != 12)
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][alter]++;
        }
    }
#endif
}

void tb303_notes_down(void)
{
#ifndef __LITE__
    for(char alter = 0; alter < 16; alter++)
    {
        if(tb303[sl3].tone[tb303[sl3].selectedpattern][alter])
        {
            tb303[sl3].tone[tb303[sl3].selectedpattern][alter]--;
        }
    }
#endif
}

void Display_Cur_copy_Buffer(void)
{
#ifndef __LITE__
    int highlight[4] =
    {
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL
    };
    highlight[Current_copy_buffer] = BUTTON_PUSHED;
    Gui_Draw_Button_Box(600, (Cur_Height - 42), 15, 16, "1", highlight[0] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[0] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(616, (Cur_Height - 42), 15, 16, "2", highlight[1] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[1] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(633, (Cur_Height - 42), 15, 16, "3", highlight[2] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[2] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(649, (Cur_Height - 42), 15, 16, "4", highlight[3] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[3] ? 0 : BUTTON_LOW_FONT));
#else
    int highlight[4] =
    {
        BUTTON_NORMAL | BUTTON_DISABLED,
        BUTTON_NORMAL | BUTTON_DISABLED,
        BUTTON_NORMAL | BUTTON_DISABLED,
        BUTTON_NORMAL | BUTTON_DISABLED
    };
    highlight[Current_copy_buffer] = BUTTON_PUSHED;
    Gui_Draw_Button_Box(600, (Cur_Height - 42), 15, 16, "1", highlight[0] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[0] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(616, (Cur_Height - 42), 15, 16, "2", highlight[1] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[1] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(633, (Cur_Height - 42), 15, 16, "3", highlight[2] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[2] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(649, (Cur_Height - 42), 15, 16, "4", highlight[3] | BUTTON_TEXT_CENTERED | (tb303_pattern_buffer_full[3] ? 0 : BUTTON_LOW_FONT));
#endif
}
