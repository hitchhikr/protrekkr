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
#include "include/editor_diskio.h"
#include "../files/include/files.h"
#include "../ui/include/requesters.h"

#if defined(__AROS__)
#include <string.h>
#endif

// ------------------------------------------------------
// Variables
extern REQUESTER Overwrite_Requester;
extern REQUESTER Zzaapp_Requester;

int allow_save = TRUE;
extern int song_Seconds;
extern int song_Minutes;
extern int song_Hours;
int Allow_32bit = 0;
int Allow_Single_Render = 0;

int Tracks_To_Render[MAX_TRACKS];
char *Tracks_Labels[MAX_TRACKS] =
{
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F"
};

typedef struct
{
    int x;
    int y;
} TRACK_POS, *LPTRACK_POS;

TRACK_POS Tracks_Position[MAX_TRACKS] =
{
    { 350 + (0 * 18), 0 },
    { 350 + (1 * 18), 0 },
    { 350 + (2 * 18), 0 },
    { 350 + (3 * 18), 0 },
    { 350 + (4 * 18), 0 },
    { 350 + (5 * 18), 0 },
    { 350 + (6 * 18), 0 },
    { 350 + (7 * 18), 0 },

    { 350 + (0 * 18), 18 },
    { 350 + (1 * 18), 18 },
    { 350 + (2 * 18), 18 },
    { 350 + (3 * 18), 18 },
    { 350 + (4 * 18), 18 },
    { 350 + (5 * 18), 18 },
    { 350 + (6 * 18), 18 },
    { 350 + (7 * 18), 18 },
};

// ------------------------------------------------------
// Functions
int Calc_Length(void);
void Reset_Song_Length(void);
void Display_Song_Length(void);
void Display_Tracks_To_Render(void);
void Check_Tracks_To_Render(void);
int Is_Track_To_Render_Solo(int nbr);
void Check_Tracks_To_Render_To_Solo(void);

void Draw_DiskIO_Ed(void)
{
    Draw_Editors_Bar(USER_SCREEN_DISKIO_EDIT);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("Disk Operations / Module Credits");

    Gui_Draw_Button_Box(254, (Cur_Height - 112), 80, 16, "Calc .ptp Size", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    outlong(254, (Cur_Height - 94), Final_Mod_Length, 7);

    Gui_Draw_Button_Box(254, (Cur_Height - 76), 80, 16, "Calc Length", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Display_Song_Length();

    Gui_Draw_Button_Box(8, (Cur_Height - 94), 80, 16, "Title", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(8, (Cur_Height - 76), 80, 16, "Produced By", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(8, (Cur_Height - 58), 80, 16, "Message", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(8, (Cur_Height - 130), 80, 16, "Zzaapp", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(90, (Cur_Height - 112), 80, 16, "WAV Render", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(90, (Cur_Height - 130), 80, 16, "Show Info", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(342, (Cur_Height - 130), 404, 102, NULL, BUTTON_NORMAL | BUTTON_DISABLED);

    PrintString(350, (Cur_Height - 128), USE_FONT, "Tracks To Render :");
    Display_Tracks_To_Render();

    PrintString(654, (Cur_Height - 124), USE_FONT, "Render To :");

    Gui_Draw_Button_Box(350, (Cur_Height - 68), 106, 16, "Output Bits Quality", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(350, (Cur_Height - 49), 106, 16, "One File Per Track", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(532, (Cur_Height - 86), 60, 26, "From", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_NO_BORDER | BUTTON_TEXT_VTOP);
    Gui_Draw_Button_Box(532, (Cur_Height - 66), 60, 26, "To", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_NO_BORDER | BUTTON_TEXT_VTOP);
}

// ------------------------------------------------------
// Refresh function
void Actualize_DiskIO_Ed(int gode)
{
    if(userscreen == USER_SCREEN_DISKIO_EDIT)
    {
        char tname[32];

        if(gode == 0 || gode == 5)
        {
            switch(rawrender_target)
            {
                case RENDER_TO_FILE:
                    Gui_Draw_Button_Box(654, (Cur_Height - 106), 80, 16, "Wav File", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                    Gui_Draw_Button_Box(654, (Cur_Height - 88), 80, 16, "Mono Sample", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                    Gui_Draw_Button_Box(654, (Cur_Height - 70), 80, 16, "Stereo Sample", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                    Allow_32bit = 0;
                    Allow_Single_Render = 0;
                    break;
                case RENDER_TO_MONO:
                    Gui_Draw_Button_Box(654, (Cur_Height - 106), 80, 16, "Wav File", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                    Gui_Draw_Button_Box(654, (Cur_Height - 88), 80, 16, "Mono Sample", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                    Gui_Draw_Button_Box(654, (Cur_Height - 70), 80, 16, "Stereo Sample", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                    Allow_32bit = BUTTON_DISABLED;
                    Allow_Single_Render = BUTTON_DISABLED;
                    break;
                case RENDER_TO_STEREO:
                    Gui_Draw_Button_Box(654, (Cur_Height - 106), 80, 16, "Wav File", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                    Gui_Draw_Button_Box(654, (Cur_Height - 88), 80, 16, "Mono Sample", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                    Gui_Draw_Button_Box(654, (Cur_Height - 70), 80, 16, "Stereo Sample", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                    Allow_32bit = BUTTON_DISABLED;
                    Allow_Single_Render = BUTTON_DISABLED;
                    break;
            }
        }

        if(gode == 0 || gode == 1)
        {
            if(rawrender_32float)
            {
                Gui_Draw_Button_Box(458, (Cur_Height - 68), 29, 16, "32", Allow_32bit | BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(458 + 31, (Cur_Height - 68), 29, 16, "16", Allow_32bit | BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(458, (Cur_Height - 68), 29, 16, "32", Allow_32bit | BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(458 + 31, (Cur_Height - 68), 29, 16, "16", Allow_32bit | BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        if(gode == 0 || gode == 2)
        {
            if(!rawrender_range)
            {
                Gui_Draw_Button_Box(534, (Cur_Height - 112), 40, 16, "Whole", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(534 + 42, (Cur_Height - 112), 40, 16, "Range", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(534, (Cur_Height - 112), 40, 16, "Whole", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(534 + 42, (Cur_Height - 112), 40, 16, "Range", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }

        // From position
        if(gode == 0 || gode == 3)
        {
            if(rawrender_from < 0) rawrender_from = 0;
            if(rawrender_from > (Song_Length - 1)) rawrender_from = (Song_Length - 1);
            if(rawrender_from > rawrender_to)
            {
                rawrender_to = rawrender_from;
                Gui_Draw_Arrows_Number_Box(572, (Cur_Height - 66), rawrender_to, BUTTON_NORMAL | (rawrender_range ? 0 : BUTTON_DISABLED) | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
            }
            Gui_Draw_Arrows_Number_Box(572, (Cur_Height - 86), rawrender_from, BUTTON_NORMAL | (rawrender_range ? 0 : BUTTON_DISABLED) | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
        }

        // To position
        if(gode == 0 || gode == 4)
        {
            if(rawrender_to < 0) rawrender_to = 0;
            if(rawrender_to > (Song_Length - 1)) rawrender_to = (Song_Length - 1);
            if(rawrender_to < rawrender_from)
            {
                rawrender_from = rawrender_to;
                Gui_Draw_Arrows_Number_Box(572, (Cur_Height - 86), rawrender_from, BUTTON_NORMAL | (rawrender_range ? 0 : BUTTON_DISABLED) | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
            }
            Gui_Draw_Arrows_Number_Box(572, (Cur_Height - 66), rawrender_to, BUTTON_NORMAL | (rawrender_range ? 0 : BUTTON_DISABLED) | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
        }

        if(allow_save)
        {
            Gui_Draw_Button_Box(8, (Cur_Height - 112), 80, 16, "Save Module", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(254, (Cur_Height - 130), 80, 16, "Save .ptp", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        }
        else
        {
            Gui_Draw_Button_Box(8, (Cur_Height - 112), 80, 16, "Save Module", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(254, (Cur_Height - 130), 80, 16, "Save .ptp", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }

        if(gode == 0 || gode == 5)
        {
            if(rawrender_multi)
            {
                Gui_Draw_Button_Box(458, (Cur_Height - 49), 29, 16, "On", Allow_Single_Render | BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(458 + 31, (Cur_Height - 49), 29, 16, "Off", Allow_Single_Render | BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(458, (Cur_Height - 49), 29, 16, "On", Allow_Single_Render | BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(458 + 31, (Cur_Height - 49), 29, 16, "Off", Allow_Single_Render | BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
        }


        if(snamesel == INPUT_MODULE_NAME)
        {
            sprintf(tname, "%s_", name);
            Gui_Draw_Button_Box(90, (Cur_Height - 94), 162, 16, tname, BUTTON_PUSHED | BUTTON_INPUT);
        }
        else
        {
            sprintf(tname, "%s", name);
            Gui_Draw_Button_Box(90, (Cur_Height - 94), 162, 16, tname, BUTTON_NORMAL | BUTTON_INPUT);
        }

        if(snamesel == INPUT_MODULE_ARTIST)
        {
            sprintf(tname, "%s_", artist);
            Gui_Draw_Button_Box(90, (Cur_Height - 76), 162, 16, tname, BUTTON_PUSHED | BUTTON_INPUT);
        }
        else
        {
            sprintf(tname, "%s", artist);
            Gui_Draw_Button_Box(90, (Cur_Height - 76), 162, 16, tname, BUTTON_NORMAL | BUTTON_INPUT);
        }

        if(snamesel == INPUT_MODULE_STYLE)
        {
            sprintf(tname, "%s_", style);
            Gui_Draw_Button_Box(90, (Cur_Height - 58), 162, 16, tname, BUTTON_PUSHED | BUTTON_INPUT);
        }
        else
        {
            sprintf(tname, "%s", style);
            Gui_Draw_Button_Box(90, (Cur_Height - 58), 162, 16, tname, BUTTON_NORMAL | BUTTON_INPUT);
        }
    }
}

// ------------------------------------------------------
// Handle right mouse button
void Mouse_Right_DiskIO_Ed(void)
{
    if(userscreen == USER_SCREEN_DISKIO_EDIT)
    {
        Check_Tracks_To_Render_To_Solo();

        if(rawrender_range)
        {
            // From position
            if(zcheckMouse(572, (Cur_Height - 86), 16, 16) == 1)
            {
                rawrender_from -= 10;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 3;
            }

            // From position
            if(zcheckMouse(572 + 44, (Cur_Height - 86), 16, 16) == 1)
            {
                rawrender_from += 10;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 3;
            }

            // To position
            if(zcheckMouse(572, (Cur_Height - 66), 16, 16) == 1)
            {
                rawrender_to -= 10;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 4;
            }

            // To position
            if(zcheckMouse(572 + 44, (Cur_Height - 66), 16, 16) == 1)
            {
                rawrender_to += 10;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 4;
            }
        }
    }
}

// ------------------------------------------------------
// Handle left mouse button
void Mouse_Left_DiskIO_Ed(void)
{
    int i;
    char WavFileName[MAX_PATH];

    if(userscreen == USER_SCREEN_DISKIO_EDIT)
    {
        // Save song
        if(zcheckMouse(8, (Cur_Height - 112), 80, 16))
        {
            if(File_Exist_Req("%s" SLASH "%s.ptk", Dir_Mods, name))
            {
                Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_MODULE, NULL, TRUE);
            }
            else
            {
                gui_action = GUI_CMD_SAVE_MODULE;
            }
        }
        // Save final
        if(zcheckMouse(254, (Cur_Height - 130), 80, 16))
        {
            if(File_Exist_Req("%s" SLASH "%s.ptp", Dir_Mods, name))
            {
                Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_FINAL, NULL, TRUE);
            }
            else
            {
                gui_action = GUI_CMD_SAVE_FINAL;
            }
        }
        // Calc final
        if(zcheckMouse(254, (Cur_Height - 112), 80, 16))
        {
            gui_action = GUI_CMD_CALC_FINAL;
        }
        // Calc length
        if(zcheckMouse(254, (Cur_Height - 76), 80, 16))
        {
            Calc_Length();
        }

        if(zcheckMouse(90, (Cur_Height - 130), 80, 16))
        {
            gui_action = GUI_CMD_MODULE_INFOS;
        }

        // Start module name input
        if(zcheckMouse(90, (Cur_Height - 94), 162, 16) && snamesel == INPUT_NONE)
        {
            strcpy(cur_input_name, name);
            sprintf(name, "");
            namesize = 0;
            snamesel = INPUT_MODULE_NAME;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Start artist name input
        if(zcheckMouse(90, (Cur_Height - 76), 162, 16) && snamesel == INPUT_NONE)
        {
            strcpy(cur_input_name, artist);
            sprintf(artist, "");
            namesize = 0;
            snamesel = INPUT_MODULE_ARTIST;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Start module style input
        if(zcheckMouse(90, (Cur_Height - 58), 162, 16) && snamesel == INPUT_NONE)
        {
            strcpy(cur_input_name, style);
            sprintf(style, "");
            namesize = 0;
            snamesel = INPUT_MODULE_STYLE;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Zzaapp
        if(zcheckMouse(8, (Cur_Height - 130), 80, 16))
        {
            Display_Requester(&Zzaapp_Requester, GUI_CMD_NEW_MODULE, NULL, TRUE);
        }

        if(zcheckMouse(90, (Cur_Height - 112), 80, 16))
        {
            if(rawrender_target == RENDER_TO_FILE)
            {
                if(rawrender_multi &&
                   rawrender_target == RENDER_TO_FILE)
                {
                    int any_file = FALSE;
                    for(i = 0; i < Songtracks; i++)
                    {
                        sprintf(WavFileName, "%%s" SLASH "%%s_%x.wav", i);
                        if(File_Exist(WavFileName, Dir_Mods, name))
                        {
                            any_file = TRUE;
                            break;
                        }
                    }
                    if(any_file)
                    {
                        Overwrite_Requester.Text = "Some .wav files are about to be overwritten, is that ok ?";
                        Display_Requester(&Overwrite_Requester, GUI_CMD_RENDER_WAV, NULL, TRUE);
                    }
                    else
                    {
                        gui_action = GUI_CMD_RENDER_WAV;
                    }
                }
                else
                {
                    if(File_Exist_Req("%s" SLASH "%s.wav", Dir_Mods, name))
                    {
                        Display_Requester(&Overwrite_Requester, GUI_CMD_RENDER_WAV, NULL, TRUE);
                    }
                    else
                    {
                        gui_action = GUI_CMD_RENDER_WAV;
                    }
                }
            }
            else
            {
                gui_action = GUI_CMD_RENDER_WAV;
            }
        }

        // Render as 32 bit on
        if(zcheckMouse(458, (Cur_Height - 68), 29, 16) && !Allow_32bit)
        {
            rawrender_32float = TRUE;
            teac = 1;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Render as 32 bit off
        if(zcheckMouse(458 + 31, (Cur_Height - 68), 29, 16) && !Allow_32bit)
        {
            rawrender_32float = FALSE;
            teac = 1;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Render entire song
        if(zcheckMouse(534, (Cur_Height - 112), 40, 16))
        {
            rawrender_range = FALSE;
            teac = 0;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Render a range
        if(zcheckMouse(534 + 42, (Cur_Height - 112), 40, 16))
        {
            rawrender_range = TRUE;
            teac = 0;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        if(rawrender_range)
        {
            // From position
            if(zcheckMouse(572, (Cur_Height - 86), 16, 16) == 1)
            {
                rawrender_from--;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 3;
            }

            // From position
            if(zcheckMouse(572 + 44, (Cur_Height - 86), 16, 16) == 1)
            {
                rawrender_from++;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 3;
            }

            // To position
            if(zcheckMouse(572, (Cur_Height - 66), 16, 16) == 1)
            {
                rawrender_to--;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 4;
            }

            // To position
            if(zcheckMouse(572 + 44, (Cur_Height - 66), 16, 16) == 1)
            {
                rawrender_to++;
                gui_action = GUI_CMD_UPDATE_DISKIO_ED;
                teac = 4;
            }
        }

        // Render to wav file
        if(zcheckMouse(654, (Cur_Height - 106), 80, 16))
        {
            rawrender_target = RENDER_TO_FILE;
            teac = 0;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Render to mono sample
        if(zcheckMouse(654, (Cur_Height - 88), 80, 16))
        {
            rawrender_target = RENDER_TO_MONO;
            teac = 0;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Render to stereo sample
        if(zcheckMouse(654, (Cur_Height - 70), 80, 16))
        {
            rawrender_target = RENDER_TO_STEREO;
            teac = 0;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Render as multiple file
        if(zcheckMouse(458, (Cur_Height - 49), 29, 16) && !Allow_Single_Render)
        {
            rawrender_multi = TRUE;
            teac = 5;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }

        // Render as single files
        if(zcheckMouse(458 + 31, (Cur_Height - 49), 29, 16) && !Allow_Single_Render)
        {
            rawrender_multi = FALSE;
            teac = 5;
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
        }


        Check_Tracks_To_Render();
    }
}

void Display_Song_Length(void)
{
    char ms[64];

    if(userscreen == USER_SCREEN_DISKIO_EDIT)
    {
        sprintf(ms, "%.2d:%.2d:%.2d", song_Hours, song_Minutes, song_Seconds);
        Gui_Draw_Button_Box(254, (Cur_Height - 58), 80, 16, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
        PrintString(274, (Cur_Height - 56), USE_FONT, ms);
    }
}

// ------------------------------------------------------
// Check if a track to render has been solo'ed already
int Is_Track_To_Render_Solo(int nbr)
{
    int i;
    int found_tracks = FALSE;

    for(i = 0; i < Songtracks; i++)
    {
        if(i != nbr)
        {
            if(!Tracks_To_Render[i]) found_tracks = TRUE;
        }
    }
    // Yes: unmute all
    if(found_tracks == FALSE) return(TRUE);
    // No: solo it
    return(FALSE);
}

// ------------------------------------------------------
// Display a track gadget with it's number
void Display_1_Track_To_Render(int nbr)
{
    if(Tracks_To_Render[nbr])
    {
        if(nbr < Songtracks)
        {
            Gui_Draw_Button_Box(Tracks_Position[nbr].x, (Cur_Height - 112) + Tracks_Position[nbr].y, 16, 16, Tracks_Labels[nbr], BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        }
        else
        {
            Gui_Draw_Button_Box(Tracks_Position[nbr].x, (Cur_Height - 112) + Tracks_Position[nbr].y, 16, 16, Tracks_Labels[nbr], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
    }
    else
    {
        if(nbr < Songtracks)
        {
            Gui_Draw_Button_Box(Tracks_Position[nbr].x, (Cur_Height - 112) + Tracks_Position[nbr].y, 16, 16, Tracks_Labels[nbr], BUTTON_PUSHED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        }
        else
        {
            Gui_Draw_Button_Box(Tracks_Position[nbr].x, (Cur_Height - 112) + Tracks_Position[nbr].y, 16, 16, Tracks_Labels[nbr], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
    }
}

// ------------------------------------------------------
// Display the list of track to render
void Display_Tracks_To_Render(void)
{
    int i;

    if(userscreen == USER_SCREEN_DISKIO_EDIT)
    {
        for(i = 0; i < MAX_TRACKS; i++)
        {
            Display_1_Track_To_Render(i);
        }
    }
}

// ------------------------------------------------------
// Clear tracks to render status
void Reset_Tracks_To_Render(void)
{
    int i;
    for(i = 0; i < Songtracks; i++)
    {
        Tracks_To_Render[i] = FALSE;
    }
    Display_Tracks_To_Render();
}

// ------------------------------------------------------
// Check if a track can be solo'ed
void Check_Tracks_To_Render_To_Solo(void)
{
    int i;
    int j;

    for(i = 0; i < Songtracks; i++)
    {
        if(zcheckMouse(Tracks_Position[i].x, (Cur_Height - 112) + Tracks_Position[i].y, 17, 16))
        {
            if(Is_Track_To_Render_Solo(i))
            {
                for(j = 0; j < Songtracks; j++)
                {
                    Tracks_To_Render[j] = FALSE;
                }
            }
            else
            {
                for(j = 0; j < Songtracks; j++)
                {
                    Tracks_To_Render[j] = TRUE;
                }
                Tracks_To_Render[i] = FALSE;
            }
        }
    }
    Display_Tracks_To_Render();
}

// ------------------------------------------------------
// Handle clicks on the tracks gadgets
void Check_Tracks_To_Render(void)
{
    int i;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        if(i < Songtracks)
        {
            if(zcheckMouse(Tracks_Position[i].x, (Cur_Height - 112) + Tracks_Position[i].y, 17, 16))
            {
                Tracks_To_Render[i] ^= TRUE;
                Display_1_Track_To_Render(i);
            }
        }
    }
}
