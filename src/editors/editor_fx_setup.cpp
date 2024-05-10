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
#include "include/editor_fx_setup.h"

// ------------------------------------------------------
// Variables
int Ticks_Synchro_Left = 1;
int Ticks_Synchro_Right = 1;
extern char Use_Cubic;
extern float mas_attack;
extern float mas_release;

int Table_Interpolation_Dec[] = { 0, SPLINE_INT, 0 };
int Table_Interpolation_Inc[] = { SPLINE_INT, CUBIC_INT, CUBIC_INT };

// ------------------------------------------------------
// Functions
void Display_Reverb_Cutoff(void);
void Display_Reverb_Resonance(void);

void Draw_Fx_Ed(void)
{
    Draw_Editors_Bar(USER_SCREEN_FX_SETUP_EDIT);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("FX Setup (Global)");
    Gui_Draw_Button_Box(8, (Cur_Height - 136), 224, 110, "Reverb Setup", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_VTOP);
    Gui_Draw_Button_Box(18, (Cur_Height - 120), 56, 16, "Switch", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(18, (Cur_Height - 102), 56, 16, "Feedback", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(18, (Cur_Height - 84), 56, 16, "Room Size", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(18, (Cur_Height - 66), 56, 16, "Filter", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(18, (Cur_Height - 48), 56, 16, "Stereo", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(240, (Cur_Height - 136), 288, 96, "Stereo Delay Settings", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_VTOP);
    Gui_Draw_Button_Box(248, (Cur_Height - 120), 56, 16, "L.Delay", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(248, (Cur_Height - 102), 56, 16, "R.Delay", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(248, (Cur_Height - 84), 56, 16, "L.Decay", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(248, (Cur_Height - 66), 56, 16, "R.Decay", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(530, (Cur_Height - 136), 104, 56, "Ticks Delay Synchro", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_VTOP);

    Gui_Draw_Button_Box(596, (Cur_Height - 120), 32, 16, "Set", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(596, (Cur_Height - 102), 32, 16, "Set", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(640, (Cur_Height - 136), 64, 16, "Interpolation", BUTTON_NORMAL | BUTTON_DISABLED);

//    Gui_Draw_Button_Box(640, 484, 144, 66, "Compressor", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_VTOP);
}

void Actualize_Fx_Ed(char gode)
{
    if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
    {
        if(gode == 0 || gode == 2)
        {
            Realslider(77, (Cur_Height - 102), (int) (Feedback * 127.0f), compressor);
        }

        if(gode == 0 || gode == 3)
        {
            if(lchorus_delay > 22100) lchorus_delay = 22100;
            if(lchorus_delay < 1) lchorus_delay = 1;
            if(gode == 3)
            {
                lchorus_counter = MIX_RATE;
                lchorus_counter2 = MIX_RATE - lchorus_delay;
            }
            Realslider(307, (Cur_Height - 120), lchorus_delay / 175, TRUE);
            outlong(458, (Cur_Height - 120), (lchorus_delay * 1000) / MIX_RATE, 2);
        }

        if(gode == 0 || gode == 4)
        {
            if(rchorus_delay > 22100) rchorus_delay = 22100;
            if(rchorus_delay < 1) rchorus_delay = 1;
            if(gode == 4)
            {
                rchorus_counter = MIX_RATE;
                rchorus_counter2 = MIX_RATE - rchorus_delay;
            }
            Realslider(307, (Cur_Height - 102), rchorus_delay / 175, TRUE);
            outlong(458, (Cur_Height - 102), (rchorus_delay * 1000) / MIX_RATE, 2);
        }

        if(gode == 0 || gode == 5)
        {
            if(lchorus_feedback > 0.95f) lchorus_feedback = 0.95f;
            if(lchorus_feedback < 0) lchorus_feedback = 0;
            Realslider(307, (Cur_Height - 84), (int) (lchorus_feedback * 127.0f), TRUE);
            outlong(458, (Cur_Height - 84), (int) (lchorus_feedback * 100.0f), 1);
        }

        if(gode == 0 || gode == 6)
        {
            if(rchorus_feedback > 0.95f) rchorus_feedback = 0.95f;
            if(rchorus_feedback < 0) rchorus_feedback = 0;
            Realslider(307, (Cur_Height - 66), (int) (rchorus_feedback * 127), TRUE);
            outlong(458, (Cur_Height - 66), (int) (rchorus_feedback * 100), 1);
        }

        if(gode == 0 || gode == 7)
        {
            if(c_threshold < 10) c_threshold = 10;
            if(c_threshold > 127) c_threshold = 127;
            Realslider(77, (Cur_Height - 84), c_threshold, compressor);
            // Re-generate
            if(gode == 7) InitRevervbFilter();
        }

        if(gode == 0 || gode == 8)
        {
            if(compressor == FALSE)
            {
                Gui_Draw_Button_Box(79, (Cur_Height - 120), 32, 16, "On", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(113, (Cur_Height - 120), 32, 16, "Off", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(79, (Cur_Height - 120), 32, 16, "On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(113, (Cur_Height - 120), 32, 16, "Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            }
        }

        if(gode == 0 || gode == 9)
        {
            Display_Reverb_Cutoff();
        }

        if(gode == 0 || gode == 10)
        {
            if(Ticks_Synchro_Left < 1) Ticks_Synchro_Left = 1;
            while((SamplesPerTick * Ticks_Synchro_Left) > 22100)
            {
                Ticks_Synchro_Left--;
            }
            Gui_Draw_Arrows_Number_Box2(534, (Cur_Height - 120), Ticks_Synchro_Left, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        }
        if(gode == 0 || gode == 11)
        {
            if(Ticks_Synchro_Right < 1) Ticks_Synchro_Right = 1;
            while((SamplesPerTick * Ticks_Synchro_Right) > 22100)
            {
                Ticks_Synchro_Right--;
            }
            Gui_Draw_Arrows_Number_Box2(534, (Cur_Height - 102), Ticks_Synchro_Right, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        }

        if(gode == 0 || gode == 12)
        {
            Gui_Draw_Button_Box(706, (Cur_Height - 136), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

            switch(Use_Cubic)
            {
                case CUBIC_INT:
                    Gui_Draw_Button_Box(706 + 16 + 2, (Cur_Height - 136), 42, 16, "Cubic", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                case SPLINE_INT:
                    Gui_Draw_Button_Box(706 + 16 + 2, (Cur_Height - 136), 42, 16, "Linear", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                default:
                    Gui_Draw_Button_Box(706 + 16 + 2, (Cur_Height - 136), 42, 16, "None", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
            }
            Gui_Draw_Button_Box(706 + (18 + 42) + 2, (Cur_Height - 136), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        }

        if(gode == 0 || gode == 13)
        {
            if(Reverb_Stereo_Amount < 0) Reverb_Stereo_Amount = 0;
            if(Reverb_Stereo_Amount > 127) Reverb_Stereo_Amount = 127;
            Realslider(77, (Cur_Height - 48), Reverb_Stereo_Amount, compressor);
            if(gode == 13) Initreverb();
        }

        if(gode == 0 || gode == 14)
        {
            Display_Reverb_Resonance();
        }

/*
        if(gode == 0 || gode == 13)
        {
            char string[64];

            Gui_Draw_Button_Box(648, 505, 56, 16, "Attack", BUTTON_NORMAL | BUTTON_DISABLED);
            Realslider_Size(705, 505, 55, (int) (mas_attack * 55.0f), TRUE);
            sprintf(string, "%d%%", (int) (mas_attack * 100.0f));
            Print_String(string, 705, 507, 72, BUTTON_TEXT_CENTERED);

            Gui_Draw_Button_Box(648, 523, 56, 16, "Release", BUTTON_NORMAL | BUTTON_DISABLED);
            Realslider_Size(705, 523, 55, (int) (mas_release * 55.0f), TRUE);
            sprintf(string, "%d%%", (int) (mas_release * 100.0f));
            Print_String(string, 705, 525, 72, BUTTON_TEXT_CENTERED);
        }*/
    }
}

void Mouse_Sliders_Fx_Ed(void)
{
    if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
    {
        if(zcheckMouse(77, (Cur_Height - 102), 148, 16) && compressor)
        {
            Feedback = float(float(Mouse.x - 87) / 127.0f);
            if(Feedback < 0) Feedback = 0;
            if(Feedback > 0.88f) Feedback = 0.88f;

            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 2;
        }
        
        if(zcheckMouse(77, (Cur_Height - 84), 148, 16) && compressor)
        {
            c_threshold = Mouse.x - 87;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 7;
        }
        // Reverb filter cutoff
        if(zcheckMouse(77, (Cur_Height - 66), 67, 16) && compressor)
        {
            Reverb_Filter_Cutoff = (float)(Mouse.x - 87) / 50.0f;
            if(Reverb_Filter_Cutoff < 0.02f) Reverb_Filter_Cutoff = 0.02f;
            if(Reverb_Filter_Cutoff > 0.99f) Reverb_Filter_Cutoff = 0.99f;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 9;
        }  
        // Reverb filter resonance
        if(zcheckMouse(150, (Cur_Height - 66), 67, 16) && compressor)
        {
            Reverb_Filter_Resonance = (float)(Mouse.x - 160) / 50.0f;
            if(Reverb_Filter_Resonance < 0.02f) Reverb_Filter_Resonance = 0.02f;
            if(Reverb_Filter_Resonance > 0.99f) Reverb_Filter_Resonance = 0.99f;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 14;
        }  
        // Reverb stereo
        if(zcheckMouse(77, (Cur_Height - 48), 148, 16) && compressor)
        {
            int value = (Mouse.x - 87);
            if(value < 0) value = 0;
            if(value > 127) value = 127;
            Reverb_Stereo_Amount = value;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 13;
        }  
        if(zcheckMouse(307, (Cur_Height - 120), 148, 16))
        {
            lchorus_delay = (Mouse.x - 317) * 174;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 3;
        }
        if(zcheckMouse(307, (Cur_Height - 102), 148, 16))
        {
            rchorus_delay = (Mouse.x - 317) * 174;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 4;
        }
        if(zcheckMouse(307, (Cur_Height - 84), 148, 16))
        {
            lchorus_feedback = float(Mouse.x - 317) / 127;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 5;
        }
        if(zcheckMouse(307, (Cur_Height - 66), 148, 16))
        {
            rchorus_feedback = float(Mouse.x - 317) / 127;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 6;
        }

/*        // Compressor attack
        if(zcheckMouse(706, 505, 72, 18))
        {
            mas_attack = float(Mouse.x - 716.0f) / 55;
            if(mas_attack < 0.0f) mas_attack = 0.0f;
            if(mas_attack > 0.99f) mas_attack = 0.99f;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 14;
        }

        // Compressor release
        if(zcheckMouse(706, 523, 72, 18))
        {
            mas_release = float(Mouse.x - 716.0f) / 55;
            if(mas_release < 0.0f) mas_release = 0.0f;
            if(mas_release > 0.99f) mas_release = 0.99f;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 14;
        }
*/
    } // userscreen
}

void Mouse_Right_Fx_Ed(void)
{
    if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
    {
        // Ticks synchro left
        if(zcheckMouse(534, (Cur_Height - 120), 16, 16) == 1)
        {
            Ticks_Synchro_Left -= 10;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 10;
        }
        if(zcheckMouse(578, (Cur_Height - 120), 16, 16) == 1)
        {
            Ticks_Synchro_Left += 10;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 10;
        }

        // Ticks synchro right
        if(zcheckMouse(534, (Cur_Height - 102), 16, 16) == 1)
        {
            Ticks_Synchro_Right -= 10;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 11;
        }
        if(zcheckMouse(578, (Cur_Height - 102), 16, 16) == 1)
        {
            Ticks_Synchro_Right += 10;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 11;
        }
    }
}

void Mouse_Left_Fx_Ed(void)
{
    if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
    {
        // Delay type
        if(zcheckMouse(79, (Cur_Height - 84), 16, 16) && compressor)
        {
            DelayType--;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 1;
        }
        // Delay type
        if(zcheckMouse(79 + 44, (Cur_Height - 84), 16, 16) && compressor)
        {
            DelayType++;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 1;
        }

        // Ticks synchro left
        if(zcheckMouse(534, (Cur_Height - 120), 16, 16))
        {
            Ticks_Synchro_Left--;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 10;
        }
        if(zcheckMouse(578, (Cur_Height - 120), 16, 16))
        {
            Ticks_Synchro_Left++;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 10;
        }

        // Ticks synchro right
        if(zcheckMouse(534, (Cur_Height - 102), 16, 16))
        {
            Ticks_Synchro_Right--;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 11;
        }
        if(zcheckMouse(578, (Cur_Height - 102), 16, 16))
        {
            Ticks_Synchro_Right++;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 11;
        }

        if(zcheckMouse(596, (Cur_Height - 120), 32, 16))
        {
            lchorus_delay = SamplesPerTick * Ticks_Synchro_Left;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 3;
        }
        if(zcheckMouse(596, (Cur_Height - 102), 32, 16))
        {
            rchorus_delay = SamplesPerTick * Ticks_Synchro_Right;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 4;
        }

        if(compressor == FALSE && zcheckMouse(79, (Cur_Height - 120), 32, 16))
        {
            compressor = TRUE;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 0;
        }
        if(compressor == TRUE && zcheckMouse(113, (Cur_Height - 120), 32, 16))
        {
            compressor = FALSE;
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 0;
        }  

        if(zcheckMouse(706, (Cur_Height - 136), 16, 16))
        {
            if(Use_Cubic > 0)
            {
                Use_Cubic = Table_Interpolation_Dec[Use_Cubic];
            }
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 12;
        }
        if(zcheckMouse(706 + (18 + 42) + 2, (Cur_Height - 136), 16, 16))
        {
            if(Use_Cubic <= SPLINE_INT)
            {
                Use_Cubic = Table_Interpolation_Inc[Use_Cubic];
            }
            gui_action = GUI_CMD_UPDATE_FX_ED;
            teac = 12;
        }

    }
}

void Display_Reverb_Cutoff(void)
{
    char string[64];

    if(Reverb_Filter_Cutoff < 0.02f) Reverb_Filter_Cutoff = 0.02f;
    if(Reverb_Filter_Cutoff > 0.99f) Reverb_Filter_Cutoff = 0.99f;

    Realslider_Size(77, (Cur_Height - 66), 55, (int) (Reverb_Filter_Cutoff * 55.0f), compressor);

    sprintf(string, "%d%%", (int) (Reverb_Filter_Cutoff * 102.0f));
    Print_String(string, 77, (Cur_Height - (66 - 2)), 55 + 17, BUTTON_TEXT_CENTERED);
}

void Display_Reverb_Resonance(void)
{
    char string[64];

    if(Reverb_Filter_Resonance < 0.02f) Reverb_Filter_Resonance = 0.02f;
    if(Reverb_Filter_Resonance > 0.99f) Reverb_Filter_Resonance = 0.99f;

    Realslider_Size(150, (Cur_Height - 66), 55, (int) (Reverb_Filter_Resonance * 55.0f), compressor);

    sprintf(string, "%d%%", (int) (Reverb_Filter_Resonance * 102.0f));
    Print_String(string, 150, (Cur_Height - (66 - 2)), 55 + 17, BUTTON_TEXT_CENTERED);
}
