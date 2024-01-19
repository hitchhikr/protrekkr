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
#include "include/config.h"

// ------------------------------------------------------
// Variables
extern int metronome_magnify;
extern int patt_highlight;
extern char FullScreen;
extern int Cur_Left;
extern int Cur_Top;
extern int Cur_Width;
extern int Cur_Height;
extern int Continuous_Scroll;
extern char AutoSave;
extern char AutoBackup;
extern char Scopish_LeftRight;
extern char Jazz_Edit;
extern char Accidental;
extern char Use_Shadows;
extern char Global_Patterns_Font;
extern char *cur_dir;

// ------------------------------------------------------
// Save the configuration file
void Save_Config(void)
{
    FILE *out;
    char extension[10];
    char FileName[MAX_PATH];
    int i;
    int Real_Palette_Idx;
    char KeyboardName[MAX_PATH];
    signed char phony = -1;

    sprintf(extension, "PROTCFGF");
    Status_Box("Saving 'ptk.cfg'...");

    sprintf(FileName, "%s" SLASH "ptk.cfg", ExePath);

    memset(KeyboardName, 0, sizeof(KeyboardName));
    sprintf(KeyboardName, "%s", Keyboard_Name);

    out = fopen(FileName, "wb");
    if(out != NULL)
    {
        Write_Data(extension, sizeof(char), 9, out);
        Write_Data_Swap(&Current_Edit_Steps, sizeof(Current_Edit_Steps), 1, out);
        Write_Data_Swap(&patt_highlight, sizeof(patt_highlight), 1, out);
        Write_Data_Swap(&AUDIO_Milliseconds, sizeof(AUDIO_Milliseconds), 1, out);

#if defined(__NO_MIDI__)
        Write_Data(&phony, sizeof(phony), 1, out);
#else
        Write_Data(&c_midiin, sizeof(c_midiin), 1, out);
#endif

#if defined(__NO_MIDI__)
        Write_Data(&phony, sizeof(phony), 1, out);
#else
        Write_Data(&c_midiout, sizeof(c_midiout), 1, out);
#endif

        Write_Data_Swap(&MouseWheel_Multiplier, sizeof(MouseWheel_Multiplier), 1, out);
        Write_Data(&Rows_Decimal, sizeof(Rows_Decimal), 1, out);
        Write_Data(&FullScreen, sizeof(FullScreen), 1, out);

        for(i = 0; i < NUMBER_COLORS; i++)
        {
            Real_Palette_Idx = Idx_Palette[i];
            Write_Data(&Ptk_Palette[Real_Palette_Idx].r, sizeof(char), 1, out);
            Write_Data(&Ptk_Palette[Real_Palette_Idx].g, sizeof(char), 1, out);
            Write_Data(&Ptk_Palette[Real_Palette_Idx].b, sizeof(char), 1, out);
        }
        Write_Data(&See_Prev_Next_Pattern, sizeof(See_Prev_Next_Pattern), 1, out);
        Write_Data_Swap(&Beveled, sizeof(Beveled), 1, out);
        Write_Data_Swap(&Continuous_Scroll, sizeof(Continuous_Scroll), 1, out);
        Write_Data(&AutoSave, sizeof(AutoSave), 1, out);
        Write_Data(&AutoBackup, sizeof(AutoBackup), 1, out);
        
        Write_Data(&Dir_Mods, sizeof(Dir_Mods), 1, out);
        Write_Data(&Dir_Instrs, sizeof(Dir_Instrs), 1, out);
        Write_Data(&Dir_Presets, sizeof(Dir_Presets), 1, out);
        Write_Data(&Dir_Reverbs, sizeof(Dir_Reverbs), 1, out);
        Write_Data(&Dir_MidiCfg, sizeof(Dir_MidiCfg), 1, out);
        Write_Data(&Dir_Patterns, sizeof(Dir_Patterns), 1, out);
        Write_Data(&Dir_Samples, sizeof(Dir_Samples), 1, out);
        Write_Data(KeyboardName, MAX_PATH, 1, out);

        Write_Data(&rawrender_32float, sizeof(char), 1, out);
        Write_Data(&rawrender_multi, sizeof(char), 1, out);
        Write_Data(&rawrender_target, sizeof(char), 1, out);
        Write_Data(&Large_Patterns, sizeof(char), 1, out);
        Write_Data(&Scopish_LeftRight, sizeof(char), 1, out);
 
        Write_Data(&Paste_Across, sizeof(char), 1, out);
        Write_Data(&Jazz_Edit, sizeof(char), 1, out);
        Write_Data(&Accidental, sizeof(char), 1, out);

        Write_Data(&Use_Shadows, sizeof(char), 1, out);
        Write_Data(&Global_Patterns_Font, sizeof(char), 1, out);

        Write_Data(&metronome_magnify, sizeof(int), 1, out);

        // Save the compelte midi automation config
        Save_Midi_Cfg_Data(Write_Data, Write_Data_Swap, out);

        Write_Data_Swap(&Cur_Width, sizeof(int), 1, out);
        Write_Data_Swap(&Cur_Height, sizeof(int), 1, out);

        Cur_Left = -1;
        Cur_Top = -1;
        Write_Data_Swap(&Cur_Left, sizeof(int), 1, out);
        Write_Data_Swap(&Cur_Top, sizeof(int), 1, out);

        fclose(out);

        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Status_Box("Configuration file saved succesfully.");  
    }
    else
    {
        Status_Box("Configuration file save failed.");
    }
}

// ------------------------------------------------------
// Load the configuration file
void Load_Config(void)
{
    FILE *in;
    int i;
    int Real_Palette_Idx;
    char FileName[MAX_PATH];
    char KeyboardName[MAX_PATH];
    signed char phony = -1;
    char Win_Coords[64];
    SDL_Surface *Desktop = NULL;

    sprintf(FileName, "%s" SLASH "ptk.cfg", ExePath);

    memset(KeyboardName, 0, sizeof(KeyboardName));

    in = fopen(FileName, "rb");
    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];

        Read_Data(extension, sizeof(char), 9, in);
        if(strcmp(extension, "PROTCFGF") == 0)
        {
            Read_Data_Swap(&Current_Edit_Steps, sizeof(Current_Edit_Steps), 1, in);
            Read_Data_Swap(&patt_highlight, sizeof(patt_highlight), 1, in);
            Read_Data_Swap(&AUDIO_Milliseconds, sizeof(AUDIO_Milliseconds), 1, in);

#if defined(__NO_MIDI__)
            Read_Data(&phony, sizeof(phony), 1, in);
#else
            Read_Data(&c_midiin, sizeof(c_midiin), 1, in);
#endif

#if defined(__NO_MIDI__)
            Read_Data(&phony, sizeof(phony), 1, in);
#else
            Read_Data(&c_midiout, sizeof(c_midiout), 1, in);
#endif

            Read_Data_Swap(&MouseWheel_Multiplier, sizeof(MouseWheel_Multiplier), 1, in);
            Read_Data(&Rows_Decimal, sizeof(Rows_Decimal), 1, in);
            Read_Data(&FullScreen, sizeof(FullScreen), 1, in);

            for(i = 0; i < NUMBER_COLORS; i++)
            {
                Real_Palette_Idx = Idx_Palette[i];
                Read_Data(&Ptk_Palette[Real_Palette_Idx].r, sizeof(char), 1, in);
                Read_Data(&Ptk_Palette[Real_Palette_Idx].g, sizeof(char), 1, in);
                Read_Data(&Ptk_Palette[Real_Palette_Idx].b, sizeof(char), 1, in);
                Ptk_Palette[Real_Palette_Idx].unused = 0;
            }
            Read_Data(&See_Prev_Next_Pattern, sizeof(See_Prev_Next_Pattern), 1, in);
            Read_Data_Swap(&Beveled, sizeof(Beveled), 1, in);
            Read_Data_Swap(&Continuous_Scroll, sizeof(Continuous_Scroll), 1, in);
            Read_Data(&AutoSave, sizeof(AutoSave), 1, in);
            Read_Data(&AutoBackup, sizeof(AutoBackup), 1, in);
            Read_Data(&Dir_Mods, sizeof(Dir_Mods), 1, in);
            Read_Data(&Dir_Instrs, sizeof(Dir_Instrs), 1, in);
            Read_Data(&Dir_Presets, sizeof(Dir_Presets), 1, in);
            Read_Data(&Dir_Reverbs, sizeof(Dir_Reverbs), 1, in);
            Read_Data(&Dir_MidiCfg, sizeof(Dir_MidiCfg), 1, in);
            Read_Data(&Dir_Patterns, sizeof(Dir_Patterns), 1, in);
            Read_Data(&Dir_Samples, sizeof(Dir_Samples), 1, in);
            Read_Data(KeyboardName, MAX_PATH, 1, in);

            Read_Data(&rawrender_32float, sizeof(char), 1, in);
            Read_Data(&rawrender_multi, sizeof(char), 1, in);
            Read_Data(&rawrender_target, sizeof(char), 1, in);
            Read_Data(&Large_Patterns, sizeof(char), 1, in);
            Read_Data(&Scopish_LeftRight, sizeof(char), 1, in);

            Read_Data(&Paste_Across, sizeof(char), 1, in);
            Read_Data(&Jazz_Edit, sizeof(char), 1, in);
            Read_Data(&Accidental, sizeof(char), 1, in);

            Read_Data(&Use_Shadows, sizeof(char), 1, in);
            Read_Data(&Global_Patterns_Font, sizeof(char), 1, in);

            Read_Data(&metronome_magnify, sizeof(int), 1, in);

            if(Large_Patterns)
            {
                Set_Pattern_Size();
                userscreen = USER_SCREEN_LARGE_PATTERN;
                curr_tab_highlight = USER_SCREEN_DISKIO_EDIT;
            }
            else
            {
                Set_Pattern_Size();
                userscreen = USER_SCREEN_DISKIO_EDIT;
                curr_tab_highlight = USER_SCREEN_DISKIO_EDIT;
            }

            // Reload the compelte midi automation config
            Load_Midi_Cfg_Data(Read_Data, Read_Data_Swap, in);

            Read_Data_Swap(&Cur_Width, sizeof(int), 1, in);
            Read_Data_Swap(&Cur_Height, sizeof(int), 1, in);

            Read_Data_Swap(&Cur_Left, sizeof(int), 1, in);
            Read_Data_Swap(&Cur_Top, sizeof(int), 1, in);
            Desktop = SDL_SetVideoMode(0, 0, 0, 0);
            // Check if the coords are too big
            if(Cur_Width > SDL_GetVideoSurface()->w)
            {
                Cur_Left = 0;
                Cur_Width = SDL_GetVideoSurface()->w;
            }
            if(Cur_Height > SDL_GetVideoSurface()->h)
            {
                Cur_Top = 0;
                Cur_Height = SDL_GetVideoSurface()->h;
            }
            if(Cur_Left == -1 ||
               Cur_Top == -1)
            {
                Cur_Left = SDL_GetVideoSurface()->w;
                Cur_Top = SDL_GetVideoSurface()->h;
                Cur_Left = (Cur_Left - Cur_Width) / 2;
                Cur_Top = (Cur_Top - Cur_Height) / 2;
            }
            SDL_FreeSurface(Desktop);

#ifndef __MORPHOS__
            sprintf(Win_Coords,
                    "SDL_VIDEO_WINDOW_POS=%d,%d",
                    Cur_Left,
                    Cur_Top);
            SDL_putenv(Win_Coords);
#endif

        }
        fclose(in);
    }
    sprintf(Keyboard_Name, "%s", KeyboardName);

    // Set default dirs if nothing
    if(!strlen(Dir_Mods))
    {
        GETCWD(Dir_Mods, MAX_PATH);

#if defined(__WIN32__)
        strcat(Dir_Mods, "\\modules");
#else
        strcat(Dir_Mods, "/modules");
#endif

    }
    if(!strlen(Dir_Instrs))
    {
        GETCWD(Dir_Instrs, MAX_PATH);

#if defined(__WIN32__)
        strcat(Dir_Instrs, "\\instruments");
#else
        strcat(Dir_Instrs, "/instruments");
#endif

    }
    if(!strlen(Dir_Presets))
    {
        GETCWD(Dir_Presets, MAX_PATH);

#if defined(__WIN32__)
        strcat(Dir_Presets, "\\presets");
#else
        strcat(Dir_Presets, "/presets");
#endif

    }

    if(!strlen(Dir_Reverbs))
    {
        GETCWD(Dir_Reverbs, MAX_PATH);

#if defined(__WIN32__)
        strcat(Dir_Reverbs, "\\reverbs");
#else
        strcat(Dir_Reverbs, "/reverbs");
#endif

    }

    if(!strlen(Dir_MidiCfg))
    {
        GETCWD(Dir_MidiCfg, MAX_PATH);

#if defined(__WIN32__)
        strcat(Dir_MidiCfg, "\\midicfgs");
#else
        strcat(Dir_MidiCfg, "/midicfgs");
#endif

    }

    if(!strlen(Dir_Patterns))
    {
        GETCWD(Dir_Patterns, MAX_PATH);

#if defined(__WIN32__)
        strcat(Dir_Patterns, "\\patterns");
#else
        strcat(Dir_Patterns, "/patterns");
#endif

    }

    if(!strlen(Dir_Samples))
    {
        GETCWD(Dir_Samples, MAX_PATH);

#if defined(__WIN32__)
        strcat(Dir_Samples, "\\samples");
#else
        strcat(Dir_Samples, "/samples");
#endif

    }

    cur_dir = Dir_Mods;
}
