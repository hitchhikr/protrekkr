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
#include "include/editor_sequencer.h"
#include "include/editor_pattern.h"
#include "include/patterns_blocks.h"

#include "../files/include/files.h"
#include "../ui/include/requesters.h"

// ------------------------------------------------------
// Structures
typedef struct
{
    int pattern;
    char active_state[16];
} SEQ_POS, *LPSEQ_POS;

// ------------------------------------------------------
// Variables
extern REQUESTER Overwrite_Requester;

int transpose_semitones;
int Cur_Seq_Buffer = 0;

char Selection_Name[20];

int Seq_Buffers_Full[4];

SEQ_POS Seq_Buffers[4] =
{
    { 0, { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, } },
    { 0, { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, } },
    { 0, { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, } },
    { 0, { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, } },
};

int cur_seq_buffer[] =
{
    BUTTON_PUSHED,
    BUTTON_NORMAL,
    BUTTON_NORMAL,
    BUTTON_NORMAL
};

int Remap_From;
int Remap_To;
int Remap_Swap;
int Action_Range;

int Ext_Pos_Switch;
int Ext_Track_Switch;

// ------------------------------------------------------
// Functions
void Seq_Fill(int st, int en, char n);
void Seq_Delete(int st);
void Seq_Insert(int st);
void Seq_Move_Up(int st);
void Seq_Move_Down(int st);
void Seq_Copy(int st);
void Display_Seq_Buffers(void);
void Seq_Paste(int st);
void Bound_Patt_Pos(void);
void Display_Seq_Edit_Buttons(void);

void Draw_Sequencer_Ed(void)
{
    Draw_Editors_Bar(USER_SCREEN_SEQUENCER);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("Sequencer");

    Gui_Draw_Button_Box(396 + 34, (Cur_Height - 134), 32, 16, "Copy", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Display_Seq_Buffers();

    Gui_Draw_Button_Box(288, (Cur_Height - 134), 16, 16, "\07", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE  | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(288, (Cur_Height - 116), 16, 16, "\01", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(288, (Cur_Height - 78), 16, 16, "\02", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(288, (Cur_Height - 60), 16, 16, "\10", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE  | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(89, (Cur_Height - 134), 24, 90, NULL, BUTTON_NORMAL);
    Gui_Draw_Button_Box(257, (Cur_Height - 134), 24, 90, NULL, BUTTON_NORMAL);
    Gui_Draw_Button_Box(120, (Cur_Height - 134), 132, 90, NULL, BUTTON_NORMAL);

    Gui_Draw_Button_Box(480, (Cur_Height - 134), 306, 28, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(480, (Cur_Height - 128), 190, 26, "Save Selection :", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_NO_BORDER | BUTTON_TEXT_VTOP);
    Gui_Draw_Button_Box(745, (Cur_Height - 128), 34, 16, "Save", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(480, (Cur_Height - 99), 306, 64, "Instrument", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_VTOP);
    Gui_Draw_Button_Box(538, (Cur_Height - 99 + 3), 42, 16, "Range", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(590, (Cur_Height - 99 + 3), 60, 16, "Remap", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(652, (Cur_Height - 99 + 3), 60, 16, "Swap", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(720, (Cur_Height - 99), 60, 18, "Transpose", BUTTON_NO_BORDER | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(720, (Cur_Height - 56), 60, 18, "Semitones", BUTTON_NO_BORDER | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(480, (Cur_Height - 77), 60, 26, "From", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_NO_BORDER | BUTTON_TEXT_VTOP);
    Gui_Draw_Button_Box(480, (Cur_Height - 56), 60, 26, "To", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_NO_BORDER | BUTTON_TEXT_VTOP);

}

void Actualize_Seq_Ed(char gode)
{
    if(userscreen == USER_SCREEN_SEQUENCER)
    {
        SetColor(COL_BLACK);
        bjbox(91, (Cur_Height - 132), 21, 87);
        bjbox(122, (Cur_Height - 132), 129, 87);
        bjbox(259, (Cur_Height - 132), 21, 87);
        SetColor(COL_BACKGROUND);
        bjbox(91 + 1, (Cur_Height - 132) + 1, 21 - 2, 87 - 2);
        bjbox(122 + 1, (Cur_Height - 132) + 1, 129 - 2, 87 - 2);
        bjbox(259 + 1, (Cur_Height - 132) + 1, 21 - 2, 87 - 2);

        // Current pos bar
        SetColor(COL_PUSHED_MED);
        bjbox(123, (Cur_Height - 95), 127, 12);

        for(int lseq = -3; lseq < 4; lseq++)
        {
            int rel;
            int Cur_Position = Get_Song_Position();
            rel = lseq + Cur_Position;
            if(rel > -1 && rel < Song_Length)
            {
                out_decchar(93, (Cur_Height - 95) + lseq * 12, rel, 0);
                out_decchar(261, (Cur_Height - 95) + lseq * 12, pSequence[rel], 0);

                for(int rel2 = 0; rel2 < Song_Tracks; rel2++)
                {
                    if(Chan_Active_State[rel][rel2]) out_nibble(124 + rel2 * 8, (Cur_Height - 95) + lseq * 12, USE_FONT, rel2);
                    else out_nibble(124 + rel2 * 8, (Cur_Height - 95) + lseq * 12, USE_FONT_LOW, rel2);
                } // sub for
            }
            else
            {   // rel range OK
                Print_String(93, (Cur_Height - 95) + lseq * 12, USE_FONT, "000");
                Print_String(261, (Cur_Height - 95) + lseq * 12, USE_FONT, "000");
            }
        } // for end
        Update_Pattern(0);

        // From instrument
        if(gode == 0 || gode == 1)
        {
            if(Remap_From < 0) Remap_From = 0;
            if(Remap_From > 0x7f) Remap_From = 0x7f;
            value_box(520, (Cur_Height - 76), Remap_From, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        }

        // To instrument
        if(gode == 0 || gode == 2)
        {
            if(Remap_To < 0) Remap_To = 0;
            if(Remap_To > 0x7f) Remap_To = 0x7f;
            value_box(520, (Cur_Height - 56), Remap_To, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        }

        if(gode == 0 || gode == 3)
        {
            char tcp[30];
            sprintf(tcp, "%s_", Selection_Name);

            if(snamesel == INPUT_SELECTION_NAME)
            {
                Gui_Draw_Button_Box(579, (Cur_Height - 128), 164, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT);
            }
            else
            {
                Gui_Draw_Button_Box(579, (Cur_Height - 128), 164, 16, Selection_Name, BUTTON_NORMAL | BUTTON_INPUT);
            }
        }

        // Transpose
        if(gode == 0 || gode == 4)
        {
            if(transpose_semitones < -120) transpose_semitones = -120;
            if(transpose_semitones > 120) transpose_semitones = 120;
            value_box_format(720, (Cur_Height - 76), transpose_semitones, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE, "%d");
        }

        if(gode == 0 || gode == 5 || gode == 6)
        {
            int dis = 0;
            if(Action_Range)
            {
                dis = BUTTON_DISABLED;
            }
            if(Remap_Swap)
            {
                Gui_Draw_Button_Box(590, (Cur_Height - 99 + 3), 60, 16, "Remap", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | dis);
                Gui_Draw_Button_Box(652, (Cur_Height - 99 + 3), 60, 16, "Swap", BUTTON_PUSHED | BUTTON_TEXT_CENTERED | dis);
            }
            else
            {
                Gui_Draw_Button_Box(590, (Cur_Height - 99 + 3), 60, 16, "Remap", BUTTON_PUSHED | BUTTON_TEXT_CENTERED | dis);
                Gui_Draw_Button_Box(652, (Cur_Height - 99 + 3), 60, 16, "Swap", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | dis);
            }
        }

        if(gode == 0 || gode == 6)
        {
            if(Action_Range)
            {
                Gui_Draw_Button_Box(538, (Cur_Height - 99 + 3), 42, 16, "Range", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(538, (Cur_Height - 99 + 3), 42, 16, "Range", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
        }
    }
    Display_Seq_Edit_Buttons();
}

void Mouse_Left_Sequencer_Ed(void)
{
    int i;
    int j;
    int k;
    int l;
    int Trans_From;
    int Trans_To;
    int Cur_Position = Get_Song_Position();

    if(userscreen == USER_SCREEN_SEQUENCER)
    {
        // Remap Selection
        if(Check_Mouse(590, (Cur_Height - 76), 60, 16) && can_modify_song)
        {
            if(!Action_Range)
            {
                Reset_Transpose_Block();
                if(transpose_semitones)
                {
                    if(transpose_semitones > 0)
                    {
                        for(k = 0; k < transpose_semitones; k++)
                        {
                            Instrument_Semitone_Up_Sel(Cur_Position, Get_Real_Selection(FALSE), 1, Remap_From);
                        }
                    }
                    else
                    {
                        for(k = 0; k < -transpose_semitones; k++)
                        {
                            Instrument_Semitone_Down_Sel(Cur_Position, Get_Real_Selection(FALSE), 1, Remap_From);
                        }
                    }
                }
                Instrument_Remap_Sel(Cur_Position, Get_Real_Selection(FALSE), Remap_From, Remap_To, Remap_Swap);
                Update_Pattern(0);
            }
            else
            {
                if(transpose_semitones)
                {
                    if(Remap_From > Remap_To)
                    {
                        Trans_From = Remap_To;
                        Trans_To = Remap_From;
                    }
                    else
                    {
                        Trans_From = Remap_From;
                        Trans_To = Remap_To;
                    }
                    Reset_Transpose_Block();
                    if(transpose_semitones > 0)
                    {
                        for(i = Trans_From; i < (Trans_To + 1); i++)
                        {
                            for(k = 0; k < transpose_semitones; k++)
                            {
                                Instrument_Semitone_Up_Sel(Cur_Position, Get_Real_Selection(FALSE), 1, i);
                            }
                        }
                    }
                    else
                    {
                        for(i = Trans_From; i < (Trans_To + 1); i++)
                        {
                            for(k = 0; k < -transpose_semitones; k++)
                            {
                                Instrument_Semitone_Down_Sel(Cur_Position, Get_Real_Selection(FALSE), 1, i);
                            }
                        }
                    }
                    Update_Pattern(0);
                }
            }
        }
        
        // Remap Track
        if(Check_Mouse(590, (Cur_Height - 56), 60, 16) && can_modify_song)
        {
            if(!Action_Range)
            {
                Reset_Transpose_Block();
                if(transpose_semitones)
                {
                    if(transpose_semitones > 0)
                    {
                        for(k = 0; k < transpose_semitones; k++)
                        {
                            Instrument_Semitone_Up_Sel(Cur_Position, Select_Track(Track_Under_Caret), 1, Remap_From);
                        }
                    }
                    else
                    {
                        for(k = 0; k < -transpose_semitones; k++)
                        {
                            Instrument_Semitone_Down_Sel(Cur_Position, Select_Track(Track_Under_Caret), 1, Remap_From);
                        }
                    }
                }
                Instrument_Remap_Sel(Cur_Position, Select_Track(Track_Under_Caret), Remap_From, Remap_To, Remap_Swap);
                Update_Pattern(0);
            }
            else
            {
                if(transpose_semitones)
                {
                    if(Remap_From > Remap_To)
                    {
                        Trans_From = Remap_To;
                        Trans_To = Remap_From;
                    }
                    else
                    {
                        Trans_From = Remap_From;
                        Trans_To = Remap_To;
                    }
                    Reset_Transpose_Block();
                    if(transpose_semitones > 0)
                    {
                        for(i = Trans_From; i < (Trans_To + 1); i++)
                        {
                            for(k = 0; k < transpose_semitones; k++)
                            {
                                Instrument_Semitone_Up_Sel(Cur_Position, Select_Track(Track_Under_Caret), 1, i);
                            }
                        }
                    }
                    else
                    {
                        for(i = Trans_From; i < (Trans_To + 1); i++)
                        {
                            for(k = 0; k < -transpose_semitones; k++)
                            {
                                Instrument_Semitone_Down_Sel(Cur_Position, Select_Track(Track_Under_Caret), 1, i);
                            }
                        }
                    }
                    Update_Pattern(0);
                }
            }
        }

        // Remap Pattern
        if(Check_Mouse(652, (Cur_Height - 76), 60, 16) && can_modify_song)
        {
            if(!Action_Range)
            {
                Reset_Transpose_Block();
                for(i = 0; i < Song_Tracks; i++)
                {
                    if(transpose_semitones)
                    {
                        if(transpose_semitones > 0)
                        {
                            for(k = 0; k < transpose_semitones; k++)
                            {
                                Instrument_Semitone_Up_Sel(Cur_Position, Select_Track(i), 1, Remap_From);
                            }
                        }
                        else
                        {
                            for(k = 0; k < -transpose_semitones; k++)
                            {
                                Instrument_Semitone_Down_Sel(Cur_Position, Select_Track(i), 1, Remap_From);
                            }
                        }
                    }
                    Instrument_Remap_Sel(Cur_Position, Select_Track(i), Remap_From, Remap_To, Remap_Swap);
                }
                Update_Pattern(0);
            }
            else
            {
                if(Remap_From > Remap_To)
                {
                    Trans_From = Remap_To;
                    Trans_To = Remap_From;
                }
                else
                {
                    Trans_From = Remap_From;
                    Trans_To = Remap_To;
                }
                Reset_Transpose_Block();
                for(i = 0; i < Song_Tracks; i++)
                {
                    if(transpose_semitones)
                    {
                        for(j = Trans_From; j < (Trans_To + 1); j++)
                        {
                            if(transpose_semitones > 0)
                            {
                                for(k = 0; k < transpose_semitones; k++)
                                {
                                    Instrument_Semitone_Up_Sel(Cur_Position, Select_Track(i), 1, j);
                                }
                            }
                            else
                            {
                                for(k = 0; k < -transpose_semitones; k++)
                                {
                                    Instrument_Semitone_Down_Sel(Cur_Position, Select_Track(i), 1, j);
                                }
                            }
                        }
                    }
                }
                Update_Pattern(0);
            }
        }

        // Remap Song
        if(Check_Mouse(652, (Cur_Height - 56), 60, 16) && can_modify_song)
        {
            char *Done_Pattern;
            int nbr_patterns;

            if(!Action_Range)
            {
                // We need an array to mark the patterns we already transposed
                // otherwise we could transpose them several times and that's not what we want.
                nbr_patterns = 0;
                for(i = 0; i < Song_Length; i++)
                {
                    if(pSequence[i] > nbr_patterns)
                    {
                        nbr_patterns = pSequence[i];
                    }
                }
                Done_Pattern = (char *) malloc(nbr_patterns + 1);
                memset(Done_Pattern, 0, nbr_patterns + 1);
                if(Done_Pattern)
                {
                    Status_Box("I'm working there...", TRUE);
                    Reset_Transpose_Block();
                    for(j = 0; j < Song_Length; j++)
                    {
                        if(!Done_Pattern[pSequence[j]])
                        {
                            for(i = 0; i < Song_Tracks; i++)
                            {
                                if(transpose_semitones)
                                {
                                    if(transpose_semitones > 0)
                                    {
                                        for(k = 0; k < transpose_semitones; k++)
                                        {
                                            Instrument_Semitone_Up_Sel(j, Select_Track(i), 1, Remap_From);
                                        }
                                    }
                                    else
                                    {
                                        for(k = 0; k < -transpose_semitones; k++)
                                        {
                                            Instrument_Semitone_Down_Sel(j, Select_Track(i), 1, Remap_From);
                                        }
                                    }
                                }
                                Instrument_Remap_Sel(j, Select_Track(i), Remap_From, Remap_To, Remap_Swap);
                            }
                            Done_Pattern[pSequence[j]] = TRUE;
                        }
                    }
                    Update_Pattern(0);
                    free(Done_Pattern);
                }
            }
            else
            {
                if(Remap_From > Remap_To)
                {
                    Trans_From = Remap_To;
                    Trans_To = Remap_From;
                }
                else
                {
                    Trans_From = Remap_From;
                    Trans_To = Remap_To;
                }

                // We need an array to mark the patterns we already transposed
                // otherwise we could transpose them several times and that's not what we want.
                nbr_patterns = 0;
                for(i = 0; i < Song_Length; i++)
                {
                    if(pSequence[i] > nbr_patterns)
                    {
                        nbr_patterns = pSequence[i];
                    }
                }
                Done_Pattern = (char *) malloc(nbr_patterns + 1);
                memset(Done_Pattern, 0, nbr_patterns + 1);
                if(Done_Pattern)
                {
                    Status_Box("I'm working there...", TRUE);
                    Reset_Transpose_Block();
                    for(j = 0; j < Song_Length; j++)
                    {
                        if(!Done_Pattern[pSequence[j]])
                        {
                            for(i = 0; i < Song_Tracks; i++)
                            {
                                if(transpose_semitones)
                                {
                                    for(l = Trans_From; l < (Trans_To + 1); l++)
                                    {
                                        if(transpose_semitones > 0)
                                        {
                                            for(k = 0; k < transpose_semitones; k++)
                                            {
                                                Instrument_Semitone_Up_Sel(j, Select_Track(i), 1, l);
                                            }
                                        }
                                        else
                                        {
                                            for(k = 0; k < -transpose_semitones; k++)
                                            {
                                                Instrument_Semitone_Down_Sel(j, Select_Track(i), 1, l);
                                            }
                                        }
                                    }
                                }
                            }
                            Done_Pattern[pSequence[j]] = TRUE;
                        }
                    }
                    Update_Pattern(0);
                    free(Done_Pattern);
                }
            }
        }

        if(Check_Mouse(538, (Cur_Height - 99 + 3), 42, 16))
        {
            Action_Range ^= TRUE;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 6;
        }

        if(Check_Mouse(590, (Cur_Height - 99 + 3), 60, 16) && !Action_Range)
        {
            Remap_Swap = FALSE;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 5;
        }

        if(Check_Mouse(652, (Cur_Height - 99 + 3), 60, 16) && !Action_Range)
        {
            Remap_Swap = TRUE;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 5;
        }

        // From Instrument
        if(Check_Mouse(520, (Cur_Height - 76), 16, 16))
        {
            Remap_From--;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // From Instrument
        if(Check_Mouse(520 + 44, (Cur_Height - 76), 16, 16))
        {
            Remap_From++;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // To Instrument
        if(Check_Mouse(520, (Cur_Height - 56), 16, 16))
        {
            Remap_To--;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // To Instrument
        if(Check_Mouse(520 + 44, (Cur_Height - 56), 16, 16))
        {
            Remap_To++;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // Transpose
        if(Check_Mouse(720, (Cur_Height - 76), 16, 16))
        {
            transpose_semitones--;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 4;
        }

        // Transpose
        if(Check_Mouse(720 + 44, (Cur_Height - 76), 16, 16))
        {
            transpose_semitones++;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 4;
        }

        // Clear all
        if(Check_Mouse(4, (Cur_Height - 134), 80, 16) && can_modify_song)
        {
            Seq_Fill(0, 256, FALSE);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Clear position
        if(Check_Mouse(4, (Cur_Height - 116), 80, 16) && can_modify_song)
        {
            Seq_Fill(Cur_Position, Cur_Position + 1, FALSE);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Reset all
        if(Check_Mouse(4, (Cur_Height - 78), 80, 16) && can_modify_song)
        {
            Seq_Fill(0, 256, TRUE);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Reset position
        if(Check_Mouse(4, (Cur_Height - 60), 80, 16) && can_modify_song)
        {
            Seq_Fill(Cur_Position, Cur_Position + 1, TRUE);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Insert position
        if(Check_Mouse(308, (Cur_Height - 78), 80, 16) && can_modify_song)
        {
            Seq_Insert(Cur_Position);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Delete position
        if(Check_Mouse(308, (Cur_Height - 60), 80, 16) && can_modify_song)
        {
            Seq_Delete(Cur_Position);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Move position up
        if(Check_Mouse(308, (Cur_Height - 97), 16, 16) && can_modify_song && (Cur_Position != 0))
        {
            Seq_Move_Up(Cur_Position);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Move position down
        if(Check_Mouse(308 + 18, (Cur_Height - 97), 16, 16) && can_modify_song && (Cur_Position < (Song_Length - 1)))
        {
            Seq_Move_Down(Cur_Position);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Select the copy buffers
        if(Check_Mouse(396, (Cur_Height - 78), 15, 16))
        {
            cur_seq_buffer[0] = BUTTON_PUSHED;
            cur_seq_buffer[1] = BUTTON_NORMAL;
            cur_seq_buffer[2] = BUTTON_NORMAL;
            cur_seq_buffer[3] = BUTTON_NORMAL;
            Display_Seq_Buffers();
            Cur_Seq_Buffer = 0;
        }
        if(Check_Mouse(396 + 17, (Cur_Height - 78), 15, 16))
        {
            cur_seq_buffer[0] = BUTTON_NORMAL;
            cur_seq_buffer[1] = BUTTON_PUSHED;
            cur_seq_buffer[2] = BUTTON_NORMAL;
            cur_seq_buffer[3] = BUTTON_NORMAL;
            Display_Seq_Buffers();
            Cur_Seq_Buffer = 1;
        }
        if(Check_Mouse(396 + (17 * 2), (Cur_Height - 78), 15, 16))
        {
            cur_seq_buffer[0] = BUTTON_NORMAL;
            cur_seq_buffer[1] = BUTTON_NORMAL;
            cur_seq_buffer[2] = BUTTON_PUSHED;
            cur_seq_buffer[3] = BUTTON_NORMAL;
            Display_Seq_Buffers();
            Cur_Seq_Buffer = 2;
        }
        if(Check_Mouse(396 + (17 * 3), (Cur_Height - 78), 15, 16))
        {
            cur_seq_buffer[0] = BUTTON_NORMAL;
            cur_seq_buffer[1] = BUTTON_NORMAL;
            cur_seq_buffer[2] = BUTTON_NORMAL;
            cur_seq_buffer[3] = BUTTON_PUSHED;
            Display_Seq_Buffers();
            Cur_Seq_Buffer = 3;
        }

        // Cut
        if(Check_Mouse(396, (Cur_Height - 134), 32, 16) && can_modify_song)
        {
            Seq_Copy(Cur_Position);
            Seq_Delete(Cur_Position);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Copy
        if(Check_Mouse(396 + 34, (Cur_Height - 134), 32, 16))
        {
            Seq_Copy(Cur_Position);
        }

        // Paste
        if(Check_Mouse(396, (Cur_Height - 116), 66, 16) && can_modify_song)
        {
            Seq_Paste(Cur_Position);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Insert/Paste
        if(Check_Mouse(396, (Cur_Height - 98), 66, 16) && can_modify_song)
        {
            Seq_Insert(Cur_Position);
            Seq_Paste(Cur_Position);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Ptn->Pos[Cur]
        if(Check_Mouse(308, (Cur_Height - 134), 80, 16) && can_modify_song)
        {
            if(Cur_Position < 128)
            {
                pSequence[Cur_Position] = Cur_Position;
                Seq_Bound_Pattern(Cur_Position);
                gui_action = GUI_CMD_UPDATE_SEQUENCER;
            }
        }

        // Ptn->Pos[Sng]
        if(Check_Mouse(308, (Cur_Height - 116), 80, 16) && can_modify_song)
        {
            for(int xpos = 0; xpos < 128; xpos++)
            {
                pSequence[xpos] = xpos;
            }
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Up 10 pos
        if(Check_Mouse(288, (Cur_Height - 134), 16, 16))
        {
            gui_action = GUI_CMD_REDUCE_POSITIONS_10;
        }
        // Up 1 pos
        if(Check_Mouse(288, (Cur_Height - 116), 16, 16))
        {
            gui_action = GUI_CMD_PREVIOUS_POSITION;
        }
        // Down 1 pos
        if(Check_Mouse(288, (Cur_Height - 78), 16, 16))
        {
            gui_action = GUI_CMD_NEXT_POSITION;
        }
        // Down 10 pos
        if(Check_Mouse(288, (Cur_Height - 60), 16, 16))
        {
            gui_action = GUI_CMD_INCREASE_POSITIONS_10;
        }

        // Add 100 to the selected pattern
        if(Check_Mouse(260, (Cur_Height - 132), 7, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 132) + 1)) / 12) - 3;
            posindex += Cur_Position;
            if(posindex >= 0 && posindex < Song_Length)
            {
                if(pSequence[posindex] < 127)
                {
                    pSequence[posindex] += 100;   
                    if(pSequence[posindex] >= 128) pSequence[posindex] = 127;
                    Seq_Bound_Pattern(posindex);

                    if(posindex != Cur_Position) gui_action = GUI_CMD_UPDATE_SEQ_ED;
                    else gui_action = GUI_CMD_UPDATE_SEQUENCER;

                }
                else
                {
                    gui_action = GUI_CMD_PATTERNS_POOL_EXHAUSTED;
                }
            }
        }

        // Add 10 to the selected pattern
        if(Check_Mouse(266, (Cur_Height - 132), 7, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 132) + 1)) / 12) - 3;
            posindex += Cur_Position;
            if(posindex >= 0 && posindex < Song_Length)
            {
                if(pSequence[posindex] < 127)
                {
                    pSequence[posindex] += 10; 
                    if(pSequence[posindex] >= 128) pSequence[posindex] = 127;
                    Seq_Bound_Pattern(posindex);

                    if(posindex != Cur_Position) gui_action = GUI_CMD_UPDATE_SEQ_ED;
                    else gui_action = GUI_CMD_UPDATE_SEQUENCER;

                }
                else
                {
                    gui_action = GUI_CMD_PATTERNS_POOL_EXHAUSTED;
                }
            }
        }

        // Add 1 to the selected pattern
        if(Check_Mouse(272, (Cur_Height - 132), 7, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 132) + 1)) / 12) - 3;
            posindex += Cur_Position;
            if(posindex >= 0 && posindex < Song_Length)
            {
                if(pSequence[posindex] < 127)
                {
                    pSequence[posindex]++;
                    Seq_Bound_Pattern(posindex);
                    if(posindex != Cur_Position) gui_action = GUI_CMD_UPDATE_SEQ_ED;
                    else gui_action = GUI_CMD_UPDATE_SEQUENCER;
                }
                else
                {
                    gui_action = GUI_CMD_PATTERNS_POOL_EXHAUSTED;
                }
            }
        }

        // Enable/Disable channels
        if(Check_Mouse(123, (Cur_Height - 131), 129, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 131) + 1)) / 12) - 3;
            posindex += Cur_Position;
            Toggle_Track_On_Off_Status(posindex, (Mouse.x - 123) / 8);
        }

        // Scroll the positions
        if(Check_Mouse(89, (Cur_Height - 131), 24, 84))
        {
            int posindex = ((Mouse.y - (Cur_Height - 131)) / 12) - 3;
            posindex += Cur_Position;
            if(posindex >= 0 && posindex < Song_Length && posindex != Cur_Position)
            {
                Song_Position = posindex;
                gui_action = GUI_CMD_UPDATE_SEQUENCER;
            }
        }

        // Start selection name input
        if(Check_Mouse(579, (Cur_Height - 128), 164, 16) && snamesel == INPUT_NONE)
        {
            snamesel = INPUT_SELECTION_NAME;
            strcpy(cur_input_name, Selection_Name);
            namesize = 0;
            sprintf(Selection_Name, "");
            teac = 3;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }
        
        // Save the data
        if(Check_Mouse(745, (Cur_Height - 128), 34, 16))
        {
            if(File_Exist_Req("%s" SLASH "%s.ppb", Dir_Patterns, Selection_Name))
            {
                Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_PATTERN, NULL, TRUE);
            }
            else
            {
                gui_action = GUI_CMD_SAVE_PATTERN;
            }
        }
    }
}

void Mouse_Right_Sequencer_Ed(void)
{
    int Cur_Position = Get_Song_Position();
    int i;

    if(userscreen == USER_SCREEN_SEQUENCER)
    {
        // Goto start position
        if(Check_Mouse(288, (Cur_Height - 134), 16, 16))
        {
            gui_action = GUI_CMD_GOTO_START_POSITION;
        }
        // Goto end position
        if(Check_Mouse(288, (Cur_Height - 60), 16, 16))
        {
            gui_action = GUI_CMD_GOTO_END_POSITION;
        }

        // From Instrument -10
        if(Check_Mouse(520, (Cur_Height - 76), 16, 16))
        {
            Remap_From -= 10;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // From Instrument +10
        if(Check_Mouse(520 + 44, (Cur_Height - 76), 16, 16))
        {
            Remap_From += 10;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // To Instrument -10
        if(Check_Mouse(520, (Cur_Height - 56), 16, 16))
        {
            Remap_To -= 10;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // To Instrument +10
        if(Check_Mouse(520 + 44, (Cur_Height - 56), 16, 16))
        {
            Remap_To += 10;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 1;
        }

        // Sub 100 to the selected pattern
        if(Check_Mouse(260, (Cur_Height - 132), 7, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 132) + 1)) / 12) - 3;
            posindex += Cur_Position;
            if(posindex >= 0 && posindex < Song_Length)
            {
                int reak = pSequence[posindex];
                reak -= 100;
                if(reak < 0) reak = 0;
                pSequence[posindex] = reak;
                Seq_Bound_Pattern(posindex);
                if(posindex != Cur_Position) gui_action = GUI_CMD_UPDATE_SEQ_ED;
                else gui_action = GUI_CMD_UPDATE_SEQUENCER;
            }
        }

        // Sub 10 to the selected pattern
        if(Check_Mouse(266, (Cur_Height - 132), 7, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 132) + 1)) / 12) - 3;
            posindex += Cur_Position;
            if(posindex >= 0 && posindex < Song_Length)
            {
                int reak = pSequence[posindex];
                reak -= 10;
                if(reak < 0) reak = 0;
                pSequence[posindex] = reak;
                Seq_Bound_Pattern(posindex);

                if(posindex != Cur_Position) gui_action = GUI_CMD_UPDATE_SEQ_ED;
                else gui_action = GUI_CMD_UPDATE_SEQUENCER;
            }
        }

        // Sub 1 to the selected pattern
        if(Check_Mouse(272, (Cur_Height - 132), 7, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 132) + 1)) / 12) - 3;
            posindex += Cur_Position;
            if(posindex >= 0 && posindex < Song_Length)
            {
                if(pSequence[posindex] > 0)
                {
                    pSequence[posindex]--;
                    Seq_Bound_Pattern(posindex);

                    if(posindex != Cur_Position) gui_action = GUI_CMD_UPDATE_SEQ_ED;
                    else gui_action = GUI_CMD_UPDATE_SEQUENCER;
                }
            }
        }

        // Solo a track
        if(Check_Mouse(123, (Cur_Height - 131), 129, 84) && can_modify_song)
        {
            int posindex = ((Mouse.y - ((Cur_Height - 131) + 1)) / 12) - 3;
            posindex += Cur_Position;
            Solo_Track_On_Off(posindex, (Mouse.x - 123) / 8);
        }

        // Insert 10 positions
        if(Check_Mouse(308, (Cur_Height - 78), 80, 16) && can_modify_song)
        {
            for(i = 0; i < 10; i++)
            {
                Seq_Insert(Cur_Position);
            }
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Delete 10 positions
        if(Check_Mouse(308, (Cur_Height - 60), 80, 16) && can_modify_song)
        {
            for(i = 0; i < 10; i++)
            {
                Seq_Delete(Cur_Position);
            }
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // Transpose
        if(Check_Mouse(720, (Cur_Height - 76), 16, 16))
        {
            transpose_semitones -= 12;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 4;
        }

        // Transpose
        if(Check_Mouse(720 + 44, (Cur_Height - 76), 16, 16))
        {
            transpose_semitones += 12;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            teac = 4;
        }
    }
}

void Actualize_Sequencer(void)
{
    int value;
    int Cur_Position;
    int i;

    if(Song_Playing)
    {
        if(Song_Position < 0) Song_Position = 0;
        if(Song_Position > Song_Length - 1)
        {
            Song_Position = Song_Length - 1;
            Bound_Patt_Pos();
            Update_Pattern(0);
        }
        for(i = 0; i < MAX_TRACKS; i++)
        {
            Chan_History_State[Song_Position][i] = FALSE;
        }
    }
    else
    {
        if(Song_Position < 0) Song_Position = 0;
        if(Song_Position > Song_Length - 1)
        {
            Song_Position = Song_Length - 1;
            Bound_Patt_Pos();
            Update_Pattern(0);
        }
        // Keep the coherency
        Song_Position_Visual = Song_Position;
        Pattern_Line_Visual = Pattern_Line;
    }
    Cur_Position = Get_Song_Position();

    value = pSequence[Cur_Position];
    if(value > 127) pSequence[Cur_Position] = 127;
    if(value < 0) pSequence[Cur_Position] = 0;

    Gui_Draw_Arrows_Number_Box(188, 28, Cur_Position, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    Gui_Draw_Arrows_Number_Box(188, 46, pSequence[Cur_Position], BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
    Seq_Bound_Pattern(Cur_Position);
    if(Rows_Decimal)
    {
        Gui_Draw_Arrows_Number_Box(188, 82, patternLines[pSequence[Cur_Position]], BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
    }
    else
    {
        value_box(188, 82, patternLines[pSequence[Cur_Position]], BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
    }
    Gui_Draw_Arrows_Number_Box(188, 64, Song_Length, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
    Actualize_Seq_Ed(0);
}

void Seq_Fill(int st, int en, char n)
{
    for(int cl = st; cl < en; cl++)
    {
        for(char trk = 0; trk < Song_Tracks; trk++)
        {
            Chan_Active_State[cl][trk] = n;
            Chan_History_State[cl][trk] = FALSE;
        }
    }
}     

// ------------------------------------------------------
// Delete a position
void Seq_Delete(int st)
{
    int cl;

    if(Song_Length > 1)
    {
        for(cl = st; cl < Song_Length - 1; cl++)
        {
            pSequence[cl] = pSequence[cl + 1];
            for(char trk = 0; trk < Song_Tracks; trk++)
            {
                Chan_Active_State[cl][trk] = Chan_Active_State[cl + 1][trk];
                Chan_History_State[cl][trk] = Chan_History_State[cl + 1][trk];
            }
        }
        // Clean the last entry
        pSequence[cl] = 0;
        for(char trk = 0; trk < Song_Tracks; trk++)
        {
            Chan_Active_State[cl][trk] = TRUE;
            Chan_History_State[cl][trk] = FALSE;
        }
        Song_Length--;
    }
}

// ------------------------------------------------------
// Insert a position
void Seq_Insert(int st)
{
    int cl;

    if(Song_Length < 255)
    {
        for(cl = Song_Length - 1; cl >= st; cl--)
        {
            pSequence[cl + 1] = pSequence[cl];
            for(char trk = 0; trk < Song_Tracks; trk++)
            {
                Chan_Active_State[cl + 1][trk] = Chan_Active_State[cl][trk];
                Chan_History_State[cl + 1][trk] = Chan_History_State[cl][trk];
            }
        }
        // Insert it at original position
        pSequence[st] = 0;
        for(char trk = 0; trk < Song_Tracks; trk++)
        {
            Chan_Active_State[st][trk] = TRUE;
            Chan_History_State[st][trk] = FALSE;
        }
        Song_Length++;
    }
}

// ------------------------------------------------------
// Move a position up
void Seq_Move_Up(int cl)
{
    int value;

    if(cl > 0)
    {
        value = pSequence[cl - 1];
        pSequence[cl - 1] = pSequence[cl];
        pSequence[cl] = value;
        for(char trk = 0; trk < Song_Tracks; trk++)
        {
            value = Chan_Active_State[cl - 1][trk];
            Chan_Active_State[cl - 1][trk] = Chan_Active_State[cl][trk];
            Chan_Active_State[cl][trk] = value;
            value = Chan_History_State[cl - 1][trk];
            Chan_History_State[cl - 1][trk] = Chan_History_State[cl][trk];
            Chan_History_State[cl][trk] = value;
        }
        Song_Position--;
    }
}

// ------------------------------------------------------
// Move a position down
void Seq_Move_Down(int cl)
{
    int value;

    if(Song_Length > 1)
    {
        value = pSequence[cl + 1];
        pSequence[cl + 1] = pSequence[cl];
        pSequence[cl] = value;
        for(char trk = 0; trk < Song_Tracks; trk++)
        {
            value = Chan_Active_State[cl + 1][trk];
            Chan_Active_State[cl + 1][trk] = Chan_Active_State[cl][trk];
            Chan_Active_State[cl][trk] = value;
            value = Chan_History_State[cl + 1][trk];
            Chan_History_State[cl + 1][trk] = Chan_History_State[cl + 1][trk];
            Chan_History_State[cl][trk] = value;
        }
        Song_Position++;
    }
}

// ------------------------------------------------------
// Set pattern number to last position
void Seq_Bound_Pattern(int posil)
{
    if(pSequence[posil] >= nPatterns)
    {
        nPatterns = pSequence[posil] + 1;
    }
}

// ------------------------------------------------------
// Copy a position
void Seq_Copy(int st)
{
    Seq_Buffers_Full[Cur_Seq_Buffer] = TRUE;
    Seq_Buffers[Cur_Seq_Buffer].pattern = pSequence[st];
    for(char trk = 0; trk < Song_Tracks; trk++)
    {
        Seq_Buffers[Cur_Seq_Buffer].active_state[trk] = Chan_Active_State[st][trk];
    }
    Display_Seq_Buffers();
}

// ------------------------------------------------------
// Paste a position
void Seq_Paste(int st)
{
    pSequence[st] = Seq_Buffers[Cur_Seq_Buffer].pattern;
    for(char trk = 0; trk < Song_Tracks; trk++)
    {
        Chan_Active_State[st][trk] = Seq_Buffers[Cur_Seq_Buffer].active_state[trk];
        Chan_History_State[st][trk] = FALSE;
    }
}     

// ------------------------------------------------------
// Notify the user selected buffer visually
void Display_Seq_Buffers(void)
{
    Gui_Draw_Button_Box(396, (Cur_Height - 78), 15, 16, "1", cur_seq_buffer[0] | BUTTON_TEXT_CENTERED | (Seq_Buffers_Full[0] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(396 + 17, (Cur_Height - 78), 15, 16, "2", cur_seq_buffer[1] | BUTTON_TEXT_CENTERED | (Seq_Buffers_Full[1] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(396 + (17 * 2), (Cur_Height - 78), 15, 16, "3", cur_seq_buffer[2] | BUTTON_TEXT_CENTERED | (Seq_Buffers_Full[2] ? 0 : BUTTON_LOW_FONT));
    Gui_Draw_Button_Box(396 + (17 * 3), (Cur_Height - 78), 15, 16, "4", cur_seq_buffer[3] | BUTTON_TEXT_CENTERED | (Seq_Buffers_Full[3] ? 0 : BUTTON_LOW_FONT));
}

// ------------------------------------------------------
// Turn a channel active state on/off
void Toggle_Track_On_Off_Status(int posindex, int seqindex)
{
    if(posindex >= 0 && posindex < Song_Length)
    {
        if(seqindex < 0) seqindex = 0;
        if(seqindex > Song_Tracks - 1) seqindex = Song_Tracks - 1;
        if(!Chan_Active_State[posindex][seqindex])
        {
            Chan_Active_State[posindex][seqindex] = TRUE;
            Chan_History_State[posindex][seqindex] = FALSE;
        }
        else
        {
            Chan_Active_State[posindex][seqindex] = FALSE;
            Chan_History_State[posindex][seqindex] = FALSE;
        }
        Update_Pattern(0);
        gui_action = GUI_CMD_UPDATE_SEQUENCER;
    }
}

// ------------------------------------------------------
// Turn all channel active states on/off but one
void Solo_Track_On_Off(int posindex, int seqindex)
{
    int Already_Solo;

    if(posindex >= 0 && posindex < Song_Length)
    {
        if(seqindex < 0) seqindex = 0;
        if(seqindex > Song_Tracks - 1) seqindex = Song_Tracks - 1;

        if(Chan_Active_State[posindex][seqindex])
        {
            // Check if it was the only track turned on
            Already_Solo = 0;
            for(int alphac = 0; alphac < Song_Tracks; alphac++)
            {
                if(Chan_Active_State[posindex][alphac] == TRUE) Already_Solo++;
            }
            if(Already_Solo == 1)
            {
                // Was already soloed: turn'em all on
                for(int alphac = 0; alphac < Song_Tracks; alphac++)
                {
                    Chan_Active_State[posindex][alphac] = TRUE;
                    Chan_History_State[posindex][alphac] = FALSE;
                }
            }
            else
            {
                // Solo it
                for(int alphac = 0; alphac < Song_Tracks; alphac++)
                {
                    Chan_Active_State[posindex][alphac] = FALSE;
                    Chan_History_State[posindex][alphac] = FALSE;
                }
            }
        }
        else
        {
            Already_Solo = 0;
            for(int alphac = 0; alphac < Song_Tracks; alphac++)
            {
                if(Chan_Active_State[posindex][alphac] == TRUE) Already_Solo++;
            }
            if(!Already_Solo)
            {
                // Active all
                for(int alphac = 0; alphac < Song_Tracks; alphac++)
                {
                    Chan_Active_State[posindex][alphac] = TRUE;
                    Chan_History_State[posindex][alphac] = FALSE;
                }
                Update_Pattern(0);
                gui_action = GUI_CMD_UPDATE_SEQUENCER;
                return;
            }
            else
            {
                // Solo it
                for(int alphac = 0; alphac < Song_Tracks; alphac++)
                {
                    Chan_Active_State[posindex][alphac] = FALSE;
                    Chan_History_State[posindex][alphac] = FALSE;
                }
            }
        }
        // Active it
        Chan_Active_State[posindex][seqindex] = TRUE;
        Chan_History_State[posindex][seqindex] = FALSE;
        Update_Pattern(0);
        gui_action = GUI_CMD_UPDATE_SEQUENCER;
    }
}

// ------------------------------------------------------
// Display the edition related buttons
void Display_Seq_Edit_Buttons(void)
{
    int editing_mode;
    
    if(userscreen == USER_SCREEN_SEQUENCER)
    {
        editing_mode = can_modify_song ? 0 : BUTTON_DISABLED;
        Gui_Draw_Button_Box(4, (Cur_Height - 134), 80, 16, "Clear All", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(4, (Cur_Height - 116), 80, 16, "Clear Position", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(4, (Cur_Height - 78), 80, 16, "Reset All", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(4, (Cur_Height - 60), 80, 16, "Reset Position", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);

        Gui_Draw_Button_Box(396, (Cur_Height - 134), 32, 16, "Cut", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(396, (Cur_Height - 116), 66, 16, "Paste", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(396, (Cur_Height - 98), 66, 16, "Insert/Paste", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);

        Gui_Draw_Button_Box(308, (Cur_Height - 134), 80, 16, "Ptn->Pos [Cur]", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(308, (Cur_Height - 116), 80, 16, "Ptn->Pos [Sng]", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(308, (Cur_Height - 78), 80, 16, "Insert Position", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(308, (Cur_Height - 60), 80, 16, "Delete Position", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED | editing_mode);

        Gui_Draw_Button_Box(308, (Cur_Height - 97), 16, 16, "\01", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode | (Get_Song_Position() == 0 ? BUTTON_DISABLED : 0));
        Gui_Draw_Button_Box(308 + 18, (Cur_Height - 97), 16, 16, "\02", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode | (Get_Song_Position() < (Song_Length - 1) ? 0 : BUTTON_DISABLED));

        Gui_Draw_Button_Box(590, (Cur_Height - 76), 60, 16, "Selection", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(590, (Cur_Height - 56), 60, 16, "Track", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(652, (Cur_Height - 76), 60, 16, "Pattern", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);
        Gui_Draw_Button_Box(652, (Cur_Height - 56), 60, 16, "Song", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | editing_mode);


    }
}
