// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2025 Franck Charlet.
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
#include "include/editor_setup.h"
#include "include/editor_sequencer.h"
#include "include/editor_pattern.h"
#include "../midi/include/midi.h"

// ------------------------------------------------------
// Variables
extern int Song_Playing_Pattern;
extern int patt_highlight;
extern char FullScreen;
extern int Cur_Width;
extern int Cur_Height;
extern char AutoSave;
extern char AutoBackup;
extern char AutoReload;
extern char SplashScreen;
extern int Beveled;
extern char Use_Shadows;
extern int Continuous_Scroll;
extern int wait_AutoSave;
extern char Global_Patterns_Font;
extern int leading_zeroes;
extern int leading_zeroes_char;
extern int leading_zeroes_char_row;

extern int metronome_magnify;

extern int Nbr_Keyboards;
extern int Keyboard_Idx;
extern char Jazz_Edit;

extern char Accidental;

int current_palette_idx;

char Paste_Across;

char *Labels_PatSize[] =
{
    "Small",
    "Medium",
    "Large"
};

char *Labels_AutoSave[] =
{
    "Off",
    "1 min",
    "2 mins",
    "4 mins",
    "8 mins",
    "10 mins",
    "15 mins",
    "30 mins"
};

// ------------------------------------------------------
// Functions
char *Get_Keyboard_Label(void);
char *Get_Keyboard_FileName(void);
void Load_Keyboard_Def(char *FileName);

void Draw_Master_Ed(void)
{
    Get_Phony_Palette();

    Draw_Editors_Bar(USER_SCREEN_SETUP_EDIT);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("UI Setup");

    Gui_Draw_Button_Box(8, (Cur_Height - 125), 110, 16, "Metronome (Rows)", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(8, (Cur_Height - 105), 110, 16, "Latency (Milliseconds)", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(330, (Cur_Height - 125), 114, 16, "Mousewheel Multiplier", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(330, (Cur_Height - 105), 114, 16, "Rows Highlight", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(330, (Cur_Height - 85), 114, 16, "Decimal Rows", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(330, (Cur_Height - 65), 114, 16, "Show Prev/Next Patt.", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(330, (Cur_Height - 45), 114, 16, "Continuous Scroll", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(8, (Cur_Height - 85), 110, 16, "Auto Save", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(670, (Cur_Height - 145), 60, 16, "Full Screen", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(520, (Cur_Height - 125), 60, 16, "Keyboard", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66, (Cur_Height - 105), 60, 16, "Themes", BUTTON_NO_BORDER | BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    
    Gui_Draw_Button_Box(520 + (18 + 108) + 1 + 66, (Cur_Height - 85), 18, 16, "\214", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 1 + 66, (Cur_Height - 65), 18, 16, "\005", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 1 + 66, (Cur_Height - 45), 18, 16, "\006", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66, (Cur_Height - 85), 18, 16, "1", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66 + 21, (Cur_Height - 85), 18, 16, "2", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66 + 42, (Cur_Height - 85), 18, 16, "3", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66, (Cur_Height - 65), 18, 16, "4", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66 + 21, (Cur_Height - 65), 18, 16, "5", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66 + 42, (Cur_Height - 65), 18, 16, "6", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66, (Cur_Height - 45), 18, 16, "7", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66 + 21, (Cur_Height - 45), 18, 16, "8", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20 + 66 + 42, (Cur_Height - 45), 18, 16, "9", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(8, (Cur_Height - 65), 110, 16, "Default Pattern Font", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(8, (Cur_Height - 45), 110, 16, "Paste Across Patterns", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(184, (Cur_Height - 125), 72, 16, "Play While Edit", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(184, (Cur_Height - 105), 72, 16, "Auto Backup", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(205, (Cur_Height - 65), 51, 16, "Splash S.", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(184, (Cur_Height - 45), 72, 16, "Load Last Ptk", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(734, (Cur_Height - 125), 42, 16, "Accid.", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(205, (Cur_Height - 85), 51, 16, "Lead 0s", BUTTON_NORMAL | BUTTON_DISABLED);
}

void Actualize_Master_Ed(char gode)
{
    int Real_Palette_Idx;
    int RefreshTex = FALSE;

    if(userscreen == USER_SCREEN_SETUP_EDIT)
    {
        // Latency
        if(gode == 0 || gode == 5)
        {

#if defined(__AMIGAOS4__)
            if(AUDIO_Milliseconds < 20) AUDIO_Milliseconds = 20;
#else
#if defined(__LINUX_ALSASEQ__)
            if(AUDIO_Milliseconds < 40) AUDIO_Milliseconds = 40;
#else
            if(AUDIO_Milliseconds < 10) AUDIO_Milliseconds = 10;
#endif
#endif

            if(AUDIO_Milliseconds > 250) AUDIO_Milliseconds = 250;
            Gui_Draw_Arrows_Number_Box(8 + 112, (Cur_Height - 105), AUDIO_Milliseconds, BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        }

        // Create a new sound buffer with the new latency amount
        if(gode == 5)
        {
            Song_Stop();
            AUDIO_Stop_Sound_Buffer();
            AUDIO_Create_Sound_Buffer(AUDIO_Milliseconds);
            Init_Scopes_VuMeters_Buffers();
            AUDIO_Play();
        }

        // Mouse wheel
        if(gode == 0 || gode == 6)
        {
            if(MouseWheel_Multiplier < 1) MouseWheel_Multiplier = 1;
            if(MouseWheel_Multiplier > 16) MouseWheel_Multiplier = 16;
            Gui_Draw_Arrows_Number_Box2(446, (Cur_Height - 125), MouseWheel_Multiplier, BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        }

        // Pattern highlight
        if(gode == 0 || gode == 7)
        {
            if(patt_highlight < 1) patt_highlight = 1;
            if(patt_highlight > 16) patt_highlight = 16;
            if(patt_highlight == 1)
            {
                Gui_Draw_Button_Box(446, (Cur_Height - 105), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 18, (Cur_Height - 105), 24, 16, "Off", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 44, (Cur_Height - 105), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                if(Rows_Decimal)
                {
                    Gui_Draw_Arrows_Number_Box2(446, (Cur_Height - 105), patt_highlight, BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                }
                else
                {
                    value_box(446, (Cur_Height - 105), patt_highlight, BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                }
            }
        }
        if(gode == 7)
        {
            Update_Pattern(0);
        }

        // Use decimal numbering for rows
        if(gode == 0 || gode == 8)
        {
            if(Rows_Decimal)
            {
                Gui_Draw_Button_Box(446, (Cur_Height - 85), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 31, (Cur_Height - 85), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(446, (Cur_Height - 85), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 31, (Cur_Height - 85), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Show Prev. next pattern
        if(gode == 0 || gode == 13)
        {
            if(See_Prev_Next_Pattern)
            {
                Gui_Draw_Button_Box(446, (Cur_Height - 65), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 31, (Cur_Height - 65), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(446, (Cur_Height - 65), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 31, (Cur_Height - 65), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Don't stop at the bottom of a pattern
        if(gode == 0 || gode == 14)
        {
            if(Continuous_Scroll)
            {
                Gui_Draw_Button_Box(446, (Cur_Height - 45), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 31, (Cur_Height - 45), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(446, (Cur_Height - 45), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(446 + 31, (Cur_Height - 45), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
            Draw_Pattern_Right_Stuff();
            Update_Pattern(0);
        }

        // Full screen
        if(gode == 0 || gode == 9)
        {
            if(FullScreen)
            {
                Gui_Draw_Button_Box(734, (Cur_Height - 145), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(734 + 31, (Cur_Height - 145), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(734, (Cur_Height - 145), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(734 + 31, (Cur_Height - 145), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Refresh palette infos
        if(gode == 0 || gode == 10)
        {
            if(current_palette_idx < 0) current_palette_idx = 0;
            if(current_palette_idx > (NUMBER_COLORS - 1)) current_palette_idx = NUMBER_COLORS - 1;
            Gui_Draw_Button_Box(520, (Cur_Height - 105), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(520 + 16 + 2, (Cur_Height - 105), 108, 16, Labels_Palette[current_palette_idx], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(520 + (18 + 108) + 2, (Cur_Height - 105), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Real_Palette_Idx = Idx_Palette[current_palette_idx];
            if(Phony_Palette[Real_Palette_Idx].r < 0) Phony_Palette[Real_Palette_Idx].r = 0;
            if(Phony_Palette[Real_Palette_Idx].r > 255) Phony_Palette[Real_Palette_Idx].r = 255;
            if(Phony_Palette[Real_Palette_Idx].g < 0) Phony_Palette[Real_Palette_Idx].g = 0;
            if(Phony_Palette[Real_Palette_Idx].g > 255) Phony_Palette[Real_Palette_Idx].g = 255;
            if(Phony_Palette[Real_Palette_Idx].b < 0) Phony_Palette[Real_Palette_Idx].b = 0;
            if(Phony_Palette[Real_Palette_Idx].b > 255) Phony_Palette[Real_Palette_Idx].b = 255;
            Real_Slider(518, (Cur_Height - 85), Ptk_Palette[Real_Palette_Idx].r / 2, TRUE);
            Print_Long_Small(668, (Cur_Height - 85), Ptk_Palette[Real_Palette_Idx].r, 0, 41, BUTTON_NORMAL | BUTTON_DISABLED);
            Real_Slider(518, (Cur_Height - 65), Ptk_Palette[Real_Palette_Idx].g / 2, TRUE);
            Print_Long_Small(668, (Cur_Height - 65), Ptk_Palette[Real_Palette_Idx].g, 0, 41, BUTTON_NORMAL | BUTTON_DISABLED);
            Real_Slider(518, (Cur_Height - 45), Ptk_Palette[Real_Palette_Idx].b / 2, TRUE);
            Print_Long_Small(668, (Cur_Height - 45), Ptk_Palette[Real_Palette_Idx].b, 0, 41, BUTTON_NORMAL | BUTTON_DISABLED);
            Set_Phony_Palette();
            if(gode) 
            {
                RefreshTex = TRUE;
            }
        }

        // Bevel on/off
        if(gode == 0 || gode == 10 || gode == 13)
        {
            switch(Beveled)
            {
                case 2:
                    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20, (Cur_Height - 105), 14, 16, "B", BUTTON_PUSHED | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
                    break;
                case 1:
                    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20, (Cur_Height - 105), 14, 16, "B", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                    break;
                case 0:
                    Gui_Draw_Button_Box(520 + (18 + 108) + 2 + 20, (Cur_Height - 105), 14, 16, "B", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
                    break;
            }
            Set_Phony_Palette();
            if(gode) 
            {
                RefreshTex = TRUE;
            }
        }

        // Set auto save interval
        if(gode == 0 || gode == 15)
        {
            if(AutoSave < 0) AutoSave = 0;
            if(AutoSave >= sizeof(Labels_AutoSave) / sizeof(char *)) AutoSave = sizeof(Labels_AutoSave) / sizeof(char *) - 1;
            Gui_Draw_Button_Box(8 + 112, (Cur_Height - 85), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(8 + 112 + 18, (Cur_Height - 85), 46, 16, Labels_AutoSave[AutoSave], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(8 + 112 + 48 + 18, (Cur_Height - 85), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        }

        // Keyboard layout
        if(gode == 0 || gode == 16)
        {
            if(Keyboard_Idx < 0) Keyboard_Idx = 0;
            if(Keyboard_Idx >= (Nbr_Keyboards - 1)) Keyboard_Idx = Nbr_Keyboards - 1;
            if(gode == 16)
            {
                Load_Keyboard_Def(Get_Keyboard_FileName());
            }

#if defined(__WIN32__)
            Gui_Draw_Button_Box(520 + 62 + 2, (Cur_Height - 125), 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
#else
            Gui_Draw_Button_Box(520 + 62 + 2, (Cur_Height - 125), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
#endif            

            Gui_Draw_Button_Box(520 + 62 + 2 + 18, (Cur_Height - 125), 106, 16, Get_Keyboard_Label(), BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

#if defined(__WIN32__)
            Gui_Draw_Button_Box(520 + 62 + 2 + 108 + 18, (Cur_Height - 125), 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
#else
            Gui_Draw_Button_Box(520 + 62 + 2 + 108 + 18, (Cur_Height - 125), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
#endif

        }

        // Paste across patterns
        if(gode == 0 || gode == 17)
        {
            if(Paste_Across)
            {
                Gui_Draw_Button_Box(120, (Cur_Height - 45), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(120 + 31, (Cur_Height - 45), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(120, (Cur_Height - 45), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(120 + 31, (Cur_Height - 45), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Play instruments while editing
        if(gode == 0 || gode == 18)
        {
            if(Jazz_Edit)
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 125), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 125), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 125), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 125), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Notes type
        if(gode == 0 || gode == 19)
        {
            if(Accidental)
            {
                Gui_Draw_Button_Box(780, (Cur_Height - 125), 14, 16, "b", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(780, (Cur_Height - 125), 14, 16, "#", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            if(gode == 19)
            {
                Update_Pattern(0);
            }
        }

        // Bevel on/off
        if(gode == 0 || gode == 20)
        {
            switch(Use_Shadows)
            {
                case 0:
                    Gui_Draw_Button_Box(520 + 18 + (18 + 108) + 2 + 20, (Cur_Height - 105), 40, 16, "Shades", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                    break;
                case 1:
                    Gui_Draw_Button_Box(520 + 18 + (18 + 108) + 2 + 20, (Cur_Height - 105), 40, 16, "Shades", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                    break;
            }
            Update_Pattern(0);
        }

        // Set default size of patterns
        if(gode == 0 || gode == 21)
        {
            if(Global_Patterns_Font < TRACK_SMALL) Global_Patterns_Font = TRACK_SMALL;
            if(Global_Patterns_Font >= TRACK_LARGE) Global_Patterns_Font = TRACK_LARGE;
            Gui_Draw_Button_Box(120, (Cur_Height - 65), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(120 + 18, (Cur_Height - 65), 46, 16, Labels_PatSize[Global_Patterns_Font], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(120 + 48 + 18, (Cur_Height - 65), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        }

        // Milliseconds
        if(gode == 0 || gode == 22)
        {
            if(metronome_magnify < 0) metronome_magnify = 0;
            if(metronome_magnify > 128) metronome_magnify = 128;
            Gui_Draw_Arrows_Number_Box(8 + 112, (Cur_Height - 125), metronome_magnify, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            if(!metronome_magnify)
            {
                Gui_Draw_Button_Box(8 + 112 + 18, (Cur_Height - 125), 24, 16, "Off", BUTTON_DISABLED | BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
        }

        // Auto backup
        if(gode == 0 || gode == 23)
        {
            if(AutoBackup)
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 105), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 105), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 105), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 105), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Splash screen
        if(gode == 0 || gode == 24)
        {
            if(SplashScreen)
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 65), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 65), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 65), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 65), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Load last used ptk
        if(gode == 0 || gode == 25)
        {
            if(AutoReload)
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 45), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 45), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 45), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 45), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // Show leading 0s
        if(gode == 0 || gode == 26)
        {
            if(leading_zeroes)
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 85), 29, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 85), 29, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(258, (Cur_Height - 85), 29, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(258 + 31, (Cur_Height - 85), 29, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }
        
        // There was a palette change
        if(RefreshTex)
        {
            Set_Pictures_And_Palettes(FALSE);
        }
    }
}

void Mouse_Right_Master_Ed(void)
{
    if(userscreen == USER_SCREEN_SETUP_EDIT)
    {
        // Previous color
        if(Check_Mouse(520, (Cur_Height - 105), 16, 16) == 1)
        {
            current_palette_idx -= 10;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Next color
        if(Check_Mouse(520 + (18 + 108) + 2, (Cur_Height - 105), 16, 16) == 1)
        {
            current_palette_idx += 10;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Turn beveling type 2 on/off
        if(Check_Mouse(520 + (18 + 108) + 2 + 20, (Cur_Height - 105), 14, 16))
        {
            if(Beveled == 2) Beveled = 0;
            else Beveled = 2;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 13;
        }

        // Metronome
        if(Check_Mouse(8 + 112, (Cur_Height - 125), 16, 16))
        {
            metronome_magnify -= 10;
            if(metronome_magnify < 0) metronome_magnify = 0;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 22;
        }

        // Metronome
        if(Check_Mouse(8 + 112 + 44, (Cur_Height - 125), 16, 16))
        {
            metronome_magnify += 10;
            if(metronome_magnify > 128) metronome_magnify = 128;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 22;
        }
    }
}

void Mouse_Left_Master_Ed(void)
{
    if(userscreen == USER_SCREEN_SETUP_EDIT)
    {
        // Latency
        if(Check_Mouse(8 + 112, (Cur_Height - 105), 16, 16))
        {

#if defined(__AMIGAOS4__)
            if(AUDIO_Milliseconds > 20)
#else
#if defined(__LINUX_ALSASEQ__)
            if(AUDIO_Milliseconds > 40)
#else
            if(AUDIO_Milliseconds > 10)
#endif
#endif

            {
                AUDIO_Milliseconds -= 10;
                gui_action = GUI_CMD_UPDATE_SETUP_ED;
                teac = 5;
            }
        }

        // Latency
        if(Check_Mouse(8 + 112 + 44, (Cur_Height - 105), 16, 16))
        {
            if(AUDIO_Milliseconds < 250)
            {
                AUDIO_Milliseconds += 10;
                gui_action = GUI_CMD_UPDATE_SETUP_ED;
                teac = 5;
            }
        }

        // Mousewheel
        if(Check_Mouse(446, (Cur_Height - 125), 16, 16))
        {
            MouseWheel_Multiplier--;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 6;
        }

        // Mousewheel
        if(Check_Mouse(446 + 44, (Cur_Height - 125), 16, 16))
        {
            MouseWheel_Multiplier++;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 6;
        }

        // Patterns sep.
        if(Check_Mouse(446, (Cur_Height - 105), 16, 16))
        {
            patt_highlight--;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 7;
        }

        // Patterns sep.
        if(Check_Mouse(446 + 44, (Cur_Height - 105), 16, 16))
        {
            patt_highlight++;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 7;
        }

        // Rows decimal on
        if(Check_Mouse(446, (Cur_Height - 85), 29, 16))
        {
            Rows_Decimal = TRUE;
            teac = 0;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        // Rows decimal off
        if(Check_Mouse(446 + 31, (Cur_Height - 85), 29, 16))
        {
            Rows_Decimal = FALSE;
            teac = 0;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        // See prev/next pattern
        if(Check_Mouse(446, (Cur_Height - 65), 29, 16))
        {
            See_Prev_Next_Pattern = TRUE;
            teac = 13;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            Update_Pattern(0);
        }

        // See prev/next pattern
        if(Check_Mouse(446 + 31, (Cur_Height - 65), 29, 16))
        {
            See_Prev_Next_Pattern = FALSE;
            teac = 13;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            Update_Pattern(0);
        }

        // Continuous scroll
        if(Check_Mouse(446, (Cur_Height - 45), 29, 16))
        {
            Continuous_Scroll = 1;
            teac = 14;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Continuous scroll
        if(Check_Mouse(446 + 31, (Cur_Height - 45), 29, 16))
        {
            Continuous_Scroll = 0;
            teac = 14;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Paste across pattern
        if(Check_Mouse(120, (Cur_Height - 45), 29, 16))
        {
            Paste_Across = TRUE;
            teac = 17;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Paste across pattern
        if(Check_Mouse(120 + 31, (Cur_Height - 45), 29, 16))
        {
            Paste_Across = FALSE;
            teac = 17;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Play while editing
        if(Check_Mouse(258, (Cur_Height - 125), 29, 16))
        {
            Jazz_Edit = TRUE;
            teac = 18;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Play while editing
        if(Check_Mouse(258 + 31, (Cur_Height - 125), 29, 16))
        {
            Jazz_Edit = FALSE;
            teac = 18;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Accidental type
        if(Check_Mouse(780, (Cur_Height - 125), 14, 16))
        {
            Accidental ^= TRUE;
            teac = 19;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Full screen on
        if(Check_Mouse(734, (Cur_Height - 145), 29, 16))
        {
            if(!FullScreen)
            {
                FullScreen = TRUE;
                teac = 9;
                Switch_FullScreen(Cur_Width, Cur_Height, TRUE, FALSE);
            }
        }

        // Full screen off
        if(Check_Mouse(734 + 31, (Cur_Height - 145), 29, 16))
        {
            if(FullScreen)
            {
                FullScreen = FALSE;
                teac = 9;
                Switch_FullScreen(Cur_Width, Cur_Height, TRUE, TRUE);
            }
        }

        // Previous color
        if(Check_Mouse(520, (Cur_Height - 105), 16, 16))
        {
            current_palette_idx--;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Next color
        if(Check_Mouse(520 + (18 + 108) + 2, (Cur_Height - 105), 16, 16))
        {
            current_palette_idx++;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Negate the palette
        if(Check_Mouse(520 + (18 + 108) +1 + 66, (Cur_Height - 85), 18, 16))
        {
            Negate_Palette();
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Rotate the palette components to the left
        if(Check_Mouse(520 + (18 + 108) +1 + 66, (Cur_Height - 65), 18, 16))
        {
            Rotate_Palette_Left();
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Rotate the palette components to the right
        if(Check_Mouse(520 + (18 + 108) +1 + 66, (Cur_Height - 45), 18, 16))
        {
            Rotate_Palette_Right();
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // set default palette 1
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66, (Cur_Height - 85), 18, 16))
        {
            Restore_Default_Palette(Default_Palette1, Default_Beveled1);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 2
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66 + 21, (Cur_Height - 85), 18, 16))
        {
            Restore_Default_Palette(Default_Palette2, Default_Beveled2);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 3
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66 + 42, (Cur_Height - 85), 18, 16))
        {
            Restore_Default_Palette(Default_Palette3, Default_Beveled3);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 4
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66, (Cur_Height - 65), 18, 16))
        {
            Restore_Default_Palette(Default_Palette4, Default_Beveled4);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 5
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66 + 21, (Cur_Height - 65), 18, 16))
        {
            Restore_Default_Palette(Default_Palette5, Default_Beveled5);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 6
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66 + 42, (Cur_Height - 65), 18, 16))
        {
            Restore_Default_Palette(Default_Palette6, Default_Beveled6);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 7
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66, (Cur_Height - 45), 18, 16))
        {
            Restore_Default_Palette(Default_Palette7, Default_Beveled7);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 8
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66 + 21, (Cur_Height - 45), 18, 16))
        {
            Restore_Default_Palette(Default_Palette8, Default_Beveled8);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Set default palette 9
        if(Check_Mouse(520 + (18 + 108) + 2 + 20 + 66 + 42, (Cur_Height - 45), 18, 16))
        {
            Restore_Default_Palette(Default_Palette9, Default_Beveled9);
            Get_Phony_Palette();
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Turn beveling type 1 on/off
        if(Check_Mouse(520 + (18 + 108) + 2 + 20, (Cur_Height - 105), 14, 16))
        {
            if(Beveled == 1) Beveled = 0;
            else Beveled = 1;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 13;
        }

        // Turn shadows on/off
        if(Check_Mouse(520 + 18 + (18 + 108) + 2 + 20, (Cur_Height - 105), 40, 16))
        {
            Use_Shadows ^= TRUE;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 20;
        }

        // Default patterns zoom
        if(Check_Mouse(120, (Cur_Height - 65), 16, 16))
        {
            Global_Patterns_Font--;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 21;
        }

        // Default patterns zoom
        if(Check_Mouse(120 + 48 + 18, (Cur_Height - 65), 16, 16))
        {
            Global_Patterns_Font++;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 21;
        }

        // Autosave
        if(Check_Mouse(8 + 112, (Cur_Height - 85), 16, 16))
        {
            AutoSave--;
            wait_AutoSave = 0;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 15;
        }

        // Autosave
        if(Check_Mouse(8 + 112 + 48 + 18, (Cur_Height - 85), 16, 16))
        {
            AutoSave++;
            wait_AutoSave = 0;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 15;
        }

#if !defined(__WIN32__)
        // Keyboard
        if(Check_Mouse(520 + 62 + 2, (Cur_Height - 125), 16, 16))
        {
            Keyboard_Idx--;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 16;
        }

        // Keyboard
        if(Check_Mouse(520 + 62 + 2 + 108 + 18, (Cur_Height - 125), 16, 16))
        {
            Keyboard_Idx++;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 16;
        }
#endif

        // Metronome
        if(Check_Mouse(8 + 112, (Cur_Height - 125), 16, 16))
        {
            if(metronome_magnify > 0)
            {
                metronome_magnify--;
                gui_action = GUI_CMD_UPDATE_SETUP_ED;
                teac = 22;
            }
        }

        // Metronome
        if(Check_Mouse(8 + 112 + 44, (Cur_Height - 125), 16, 16))
        {
            if(metronome_magnify < 128)
            {
                metronome_magnify++;
                gui_action = GUI_CMD_UPDATE_SETUP_ED;
                teac = 22;
            }
        }

        // Auto backup on
        if(Check_Mouse(258, (Cur_Height - 105), 29, 16))
        {
            AutoBackup = TRUE;
            teac = 23;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Auto backup off
        if(Check_Mouse(258 + 31, (Cur_Height - 105), 29, 16))
        {
            AutoBackup = FALSE;
            teac = 23;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Leading 0s on
        if(Check_Mouse(258, (Cur_Height - 85), 29, 16))
        {
            leading_zeroes = TRUE;
            leading_zeroes_char = 0;
            leading_zeroes_char_row = 0;
            teac = 26;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            Update_Pattern(0);
        }

        // Leading 0s off
        if(Check_Mouse(258 + 31, (Cur_Height - 85), 29, 16))
        {
            leading_zeroes = FALSE;
            leading_zeroes_char = 21;
            leading_zeroes_char_row = 20;
            teac = 26;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            Update_Pattern(0);
        }

        // Splash Screen on
        if(Check_Mouse(258, (Cur_Height - 65), 29, 16))
        {
            SplashScreen = TRUE;
            teac = 24;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Splash Screen off
        if(Check_Mouse(258 + 31, (Cur_Height - 65), 29, 16))
        {
            SplashScreen = FALSE;
            teac = 24;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Load Last Ptk on
        if(Check_Mouse(258, (Cur_Height - 45), 29, 16))
        {
            AutoReload = TRUE;
            teac = 25;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }

        // Load Last Ptk off
        if(Check_Mouse(258 + 31, (Cur_Height - 45), 29, 16))
        {
            AutoReload = FALSE;
            teac = 25;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
        }
    }
}

void Mouse_Sliders_Master_Ed(void)
{
    int Real_Palette_Idx;

    if(userscreen == USER_SCREEN_SETUP_EDIT)
    {
        // Red component
        if(Check_Mouse(518, (Cur_Height - 81), 148, 16))
        {
            Real_Palette_Idx = Idx_Palette[current_palette_idx];
            Phony_Palette[Real_Palette_Idx].r = (int) ((Mouse.x - 10 - 518.0f)) * 2;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Green component
        if(Check_Mouse(518, (Cur_Height - 81) + 18, 148, 16))
        {
            Real_Palette_Idx = Idx_Palette[current_palette_idx];
            Phony_Palette[Real_Palette_Idx].g = (int) ((Mouse.x - 10 - 518.0f)) * 2;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }

        // Blue component
        if(Check_Mouse(518, (Cur_Height - 81) + 18 + 18, 148, 16))
        {
            Real_Palette_Idx = Idx_Palette[current_palette_idx];
            Phony_Palette[Real_Palette_Idx].b = (int) ((Mouse.x - 10 - 518.0f)) * 2;
            gui_action = GUI_CMD_UPDATE_SETUP_ED;
            teac = 10;
        }
    }
}
