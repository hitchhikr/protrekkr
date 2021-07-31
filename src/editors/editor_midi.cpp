// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2021 Franck Charlet.
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
#include "include/editor_midi.h"
#include "../midi/include/midi.h"
#include "../midi/include/midi_dispatch.h"
#include "../files/include/files.h"
#include "../ui/include/requesters.h"

#define POS_X_MIDI1 100
#define POS_X_MIDI2 335
#define POS_X_MIDI3 569
#define GAP_X_MIDI 70

// ------------------------------------------------------
// Variables
extern REQUESTER Overwrite_Requester;

extern int Midi_Current_Notes[MAX_TRACKS][MAX_POLYPHONY];

char Midi_Name[20];

SCREEN_COORD Pos_Midi_Automation[] =
{
    { POS_X_MIDI1, 99 },
    { POS_X_MIDI1, 81 },
    { POS_X_MIDI1, 63 },
    { POS_X_MIDI1, 45 },
    
    { POS_X_MIDI2, 135 },
    { POS_X_MIDI2, 117 },
    { POS_X_MIDI2, 99 },
    { POS_X_MIDI2, 81 },
    { POS_X_MIDI2, 63 },
    { POS_X_MIDI2, 45 },
    
    { POS_X_MIDI3, 117 },
    { POS_X_MIDI3, 99 },
    { POS_X_MIDI3, 81 },
    { POS_X_MIDI3, 63 },
    { POS_X_MIDI3, 45 }
};

// ------------------------------------------------------
// Functions
void Display_Midi_Automation(int Idx);
void Mod_Midi_Automation_Value(int Amount);

void Draw_Midi_Ed(void)
{
    char middev[80];

    Draw_Editors_Bar(USER_SCREEN_SETUP_MIDI);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, "", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("Midi Setup");

#if defined(__NO_MIDI__)
    sprintf(middev, "In (%d)", 0);
#else
    sprintf(middev, "In (%d)", n_midiindevices);
#endif
    Gui_Draw_Button_Box(12, (Cur_Height - 135), 56, 16, middev, BUTTON_NORMAL | BUTTON_DISABLED);

#if defined(__NO_MIDI__)
    sprintf(middev, "Out (%d)", 0);
#else
    sprintf(middev, "Out (%d)", n_midioutdevices);
#endif
    Gui_Draw_Button_Box(12, (Cur_Height - 117), 56, 16, middev, BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(12, (Cur_Height - 99), 82, 16, "Track Notes Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED
#if defined(__NO_MIDI__)
    | BUTTON_DISABLED
#endif
    );
    Gui_Draw_Button_Box(12, (Cur_Height - 81), 82, 16, "Song Notes Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED
#if defined(__NO_MIDI__)
    | BUTTON_DISABLED
#endif
    );


    Gui_Draw_Button_Box(749, (Cur_Height - 142), 34, 16, "Save", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
}

void Actualize_Midi_Ed(char gode)
{
    if(userscreen == USER_SCREEN_SETUP_MIDI)
    {
        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC0)
        {
            Display_Midi_Automation(0);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC1)
        {
            Display_Midi_Automation(1);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC2)
        {
            Display_Midi_Automation(2);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC3)
        {
            Display_Midi_Automation(3);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC4)
        {
            Display_Midi_Automation(4);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC5)
        {
            Display_Midi_Automation(5);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC6)
        {
            Display_Midi_Automation(6);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC7)
        {
            Display_Midi_Automation(7);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC8)
        {
            Display_Midi_Automation(8);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC9)
        {
            Display_Midi_Automation(9);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC10)
        {
            Display_Midi_Automation(10);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC11)
        {
            Display_Midi_Automation(11);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC12)
        {
            Display_Midi_Automation(12);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC13)
        {
            Display_Midi_Automation(13);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CC14)
        {
            Display_Midi_Automation(14);
        }

        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_CHANGE_NAME)
        {
            char tcp[30];
            sprintf(tcp, "%s_", Midi_Name);

            if(snamesel == INPUT_MIDI_NAME)
            {
                Gui_Draw_Button_Box(583, (Cur_Height - 142), 164, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT);
            }
            else
            {
                Gui_Draw_Button_Box(583, (Cur_Height - 142), 164, 16, Midi_Name, BUTTON_NORMAL | BUTTON_INPUT);
            }
        }

        // -----------
#if !defined(__NO_MIDI__)
        Midi_InitIn();
        Midi_InitOut();
#endif

        // Select midi in device
        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_SEL_IN)
        {
#if defined(__NO_MIDI__)
            value_box(70, (Cur_Height - 134), 0, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
#else
            value_box(70, (Cur_Height - 134), c_midiin + 1, BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
#endif
#if !defined(__NO_MIDI__)
            if(c_midiin != -1)
            {
                Gui_Draw_Button_Box(132, (Cur_Height - 134), 182, 16, Midi_GetInName(), BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
#endif
                Gui_Draw_Button_Box(132, (Cur_Height - 134), 182, 16, "None", BUTTON_NORMAL | BUTTON_DISABLED);
#if !defined(__NO_MIDI__)
            }
#endif
        }

        // Select midi out device
        if(gode == UPDATE_MIDI_ED_ALL || gode == UPDATE_MIDI_ED_SEL_OUT)
        {
#if defined(__NO_MIDI__)
            value_box(70, (Cur_Height - 117), 0, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_DISABLED);
#else
            value_box(70, (Cur_Height - 117), c_midiout + 1, BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
#endif
#if !defined(__NO_MIDI__)
            if(c_midiout != -1)
            {
                Gui_Draw_Button_Box(132, (Cur_Height - 117), 182, 16, Midi_GetOutName(), BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
#endif
                Gui_Draw_Button_Box(132, (Cur_Height - 117), 182, 16, "None", BUTTON_NORMAL | BUTTON_DISABLED);
#if !defined(__NO_MIDI__)
            }
#endif
        }
    }
}

void Mouse_Right_Midi_Ed(void)
{
    if(userscreen == USER_SCREEN_SETUP_MIDI)
    {
        Mod_Midi_Automation_Value(10);
    }
}

void Mouse_Left_Midi_Ed(void)
{
    if(userscreen == USER_SCREEN_SETUP_MIDI)
    {
        // Save the data
        if(zcheckMouse(749, (Cur_Height - 142), 34, 16))
        {
            if(File_Exist_Req("%s" SLASH "%s.pmi", Dir_MidiCfg, Midi_Name))
            {
                Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_MIDI_CFG);
            }
            else
            {
                gui_action = GUI_CMD_SAVE_MIDI_CFG;
            }
        }

        // Start midi name input
        if(zcheckMouse(583, (Cur_Height - 142), 164, 16) && snamesel == INPUT_NONE)
        {
            snamesel = INPUT_MIDI_NAME;
            strcpy(cur_input_name, Midi_Name);
            namesize = 0;
            sprintf(Midi_Name, "");
            teac = UPDATE_MIDI_ED_CHANGE_NAME;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
        }

#if !defined(__NO_MIDI__)
        // Previous midi in device
        if(zcheckMouse(70, (Cur_Height - 134), 16, 16))
        {
            c_midiin--;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            midiin_changed = 1;
            teac = UPDATE_MIDI_ED_SEL_IN;
        }
        // Next midi in device
        if(zcheckMouse(114, (Cur_Height - 134), 16, 16))
        {
            c_midiin++;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            midiin_changed = 1;
            teac = UPDATE_MIDI_ED_SEL_IN;
        }

        // Previous midi out device
        if(zcheckMouse(70, (Cur_Height - 117), 16, 16))
        {
            c_midiout--;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            midiout_changed = TRUE;
            teac = UPDATE_MIDI_ED_SEL_OUT;
        }
        // Next midi out device
        if(zcheckMouse(114, (Cur_Height - 117), 16, 16))
        {
            c_midiout++;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            midiout_changed = TRUE;
            teac = UPDATE_MIDI_ED_SEL_OUT;
        }
#endif

        // Midi track notes off
#if !defined(__NO_MIDI__)
        if(zcheckMouse(12, (Cur_Height - 99), 82, 16) == 1 && c_midiout != -1)
        {
            Midi_NoteOff(Track_Under_Caret, -1);
            int i;
            for(i = 0; i < MAX_POLYPHONY; i++)
            {
                Midi_Current_Notes[Chan_Midi_Prg[Track_Under_Caret]][i] = 0;
            }
            gui_action = GUI_CMD_MIDI_NOTE_OFF_1_TRACK;
        }
#endif

        // All Midi notes off
#if !defined(__NO_MIDI__)
        if(zcheckMouse(12, (Cur_Height - 81), 82, 16) == 1 && c_midiout != -1)
        {
            Midi_AllNotesOff();
            gui_action = GUI_CMD_MIDI_NOTE_OFF_ALL_TRACKS;
        }
#endif

        Mod_Midi_Automation_Value(1);
    }
}

// ------------------------------------------------------
// Display an automation data
void Display_Midi_Automation(int Idx)
{
    Gui_Draw_Arrows_Number_Box(Pos_Midi_Automation[Idx].x,
                               Cur_Height - Pos_Midi_Automation[Idx].y,
                               Midi_Dispatch_Table[Idx].CC,
                               BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    Gui_Draw_Button_Box(Pos_Midi_Automation[Idx].x + GAP_X_MIDI,
                        (Cur_Height - Pos_Midi_Automation[Idx].y),
                        16,
                        16,
                        "\03",
                        BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    Gui_Draw_Button_Box(Pos_Midi_Automation[Idx].x + GAP_X_MIDI + 18,
                        (Cur_Height - Pos_Midi_Automation[Idx].y),
                        108,
                        16,
                        Str_Midi_Commands[Midi_Dispatch_Table[Idx].Automation].Name,
                        BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);
    Gui_Draw_Button_Box(Pos_Midi_Automation[Idx].x + GAP_X_MIDI + (18 + 110),
                        (Cur_Height - Pos_Midi_Automation[Idx].y),
                        16,
                        16,
                        "\04",
                        BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
}

// ------------------------------------------------------
// Modify a cc value or relative automation one
void Mod_Midi_Automation_Value(int Amount)
{
    int i;

    for(i = 0; i < NBR_MIDI_DISPATCH_MSG; i++)
    {
        if(zcheckMouse(Pos_Midi_Automation[i].x,
                       (Cur_Height - Pos_Midi_Automation[i].y),
                       16, 16) && snamesel == INPUT_NONE)
        {
            Midi_Dispatch_Table[i].CC -= Amount;
            if(Midi_Dispatch_Table[i].CC < 0) Midi_Dispatch_Table[i].CC = 0;
            teac = UPDATE_MIDI_ED_CC0 + i;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            break;
        }

        if(zcheckMouse(Pos_Midi_Automation[i].x + 44,
                       (Cur_Height - Pos_Midi_Automation[i].y),
                       16, 16) && snamesel == INPUT_NONE)
        {
            Midi_Dispatch_Table[i].CC += Amount;
            if(Midi_Dispatch_Table[i].CC > 255) Midi_Dispatch_Table[i].CC = 255;
            teac = UPDATE_MIDI_ED_CC0 + i;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            break;
        }

        if(zcheckMouse(Pos_Midi_Automation[i].x + GAP_X_MIDI,
                       (Cur_Height - Pos_Midi_Automation[i].y),
                       16, 16) && snamesel == INPUT_NONE)
        {
            Midi_Dispatch_Table[i].Automation = (MIDI_AUTOMATION) (Midi_Dispatch_Table[i].Automation - Amount);
            if(Midi_Dispatch_Table[i].Automation < (MIDI_AUTOMATION) 0) Midi_Dispatch_Table[i].Automation = (MIDI_AUTOMATION) 0;
            teac = UPDATE_MIDI_ED_CC0 + i;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            break;
        }

        if(zcheckMouse(Pos_Midi_Automation[i].x + GAP_X_MIDI + (18 + 110),
                       (Cur_Height - Pos_Midi_Automation[i].y),
                       16, 16) && snamesel == INPUT_NONE)
        {
            Midi_Dispatch_Table[i].Automation = (MIDI_AUTOMATION) (Midi_Dispatch_Table[i].Automation + Amount);
            if(Midi_Dispatch_Table[i].Automation > (MIDI_AUTOMATION) (NBR_MIDI_AUTOMATION - 1)) Midi_Dispatch_Table[i].Automation = (MIDI_AUTOMATION) (NBR_MIDI_AUTOMATION - 1);
            teac = UPDATE_MIDI_ED_CC0 + i;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            break;
        }
    }
}
