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
#include "include/editor_reverb.h"
#include "../files/include/files.h"
#include "../ui/include/requesters.h"
#include "../ui/include/gadgets.h"

// ------------------------------------------------------
// Variables
extern REQUESTER Overwrite_Requester;

char Reverb_Name[20];
SCREEN_COORD Table_Sliders[10] = 
{
    { 442, 0 },
    { 442, 18 },
    { 442, 18 * 2 },
    { 442, 18 * 3 },
    { 442, 18 * 4 },
    { 442 + 205, 0 },
    { 442 + 205, 18 },
    { 442 + 205, 18 * 2 },
    { 442 + 205, 18 * 3 },
    { 442 + 205, 18 * 4 }
};

SCREEN_COORD Table_Arrows[10] = 
{
    { 72, 0 },
    { 72, 18 },
    { 72, 18 * 2 },
    { 72, 18 * 3 },
    { 72, 18 * 4 },
    { 72 + 185, 0 },
    { 72 + 185, 18 },
    { 72 + 185, 18 * 2 },
    { 72 + 185, 18 * 3 },
    { 72 + 185, 18 * 4 }
};

int Update_Slider[] = 
{
    UPDATE_REVERB_ED_DECAY1,
    UPDATE_REVERB_ED_DECAY2,
    UPDATE_REVERB_ED_DECAY3,
    UPDATE_REVERB_ED_DECAY4,
    UPDATE_REVERB_ED_DECAY5,
    UPDATE_REVERB_ED_DECAY6,
    UPDATE_REVERB_ED_DECAY7,
    UPDATE_REVERB_ED_DECAY8,
    UPDATE_REVERB_ED_DECAY9,
    UPDATE_REVERB_ED_DECAY10
};
/*
void Pos_Exit(GADGETID id, int button, int wheel, int x, int y)
{

}

USERGADGET Gadgets_Reverb_List[] = 
{
    { BUTTON, "Echoes", 10, -135, 60, 16, ALIGN_LEFT, GADGET_STATIC_BEVEL, TRUE, Pos_Exit, NULL },
    { BUTTON, NULL, NULL, NULL, NULL, NULL, ALIGN_CENTER, NULL, NULL, NULL, NULL }
};
GADGETPOOL Reverb_IDs;

Gadgets Gadgets_Reverb(1);
*/
// ------------------------------------------------------
// Functions
void Display_Delays_Arrows(void);
void Display_Decay_Slider(int Index);

// ------------------------------------------------------
// Display the panel
void Draw_Reverb_Ed(void)
{
    //Gadgets_Reverb.Create_List(Gadgets_Reverb_List, &Reverb_IDs);

    Draw_Editors_Bar(USER_SCREEN_REVERB_EDIT);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, "", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("Reverb customizer");

    Gui_Draw_Button_Box(10, (Cur_Height - 135), 60, 16, "Echoes", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(10, (Cur_Height - 115), 60, 16, "Delay 1", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10, (Cur_Height - 97), 60, 16, "Delay 2", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10, (Cur_Height - 79), 60, 16, "Delay 3", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10, (Cur_Height - 61), 60, 16, "Delay 4", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10, (Cur_Height - 43), 60, 16, "Delay 5", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(10 + 185, (Cur_Height - 115), 60, 16, "Delay 6", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10 + 185, (Cur_Height - 97), 60, 16, "Delay 7", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10 + 185, (Cur_Height - 79), 60, 16, "Delay 8", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10 + 185, (Cur_Height - 61), 60, 16, "Delay 9", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(10 + 185, (Cur_Height - 43), 60, 16, "Delay 10", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(380, (Cur_Height - 115), 60, 16, "Decay 1", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(380, (Cur_Height - 97), 60, 16, "Decay 2", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(380, (Cur_Height - 79), 60, 16, "Decay 3", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(380, (Cur_Height - 61), 60, 16, "Decay 4", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(380, (Cur_Height - 43), 60, 16, "Decay 5", BUTTON_NORMAL | BUTTON_DISABLED);
    
    Gui_Draw_Button_Box(585, (Cur_Height - 115), 60, 16, "Decay 6", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(585, (Cur_Height - 97), 60, 16, "Decay 7", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(585, (Cur_Height - 79), 60, 16, "Decay 8", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(585, (Cur_Height - 61), 60, 16, "Decay 9", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Button_Box(585, (Cur_Height - 43), 60, 16, "Decay 10", BUTTON_NORMAL | BUTTON_DISABLED);

    Gui_Draw_Button_Box(749, (Cur_Height - 142), 34, 16, "Save", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
}

// ------------------------------------------------------
// Redraw the elements on the screen
void Mouse_Reverb_Ed()
{
    if(userscreen == USER_SCREEN_REVERB_EDIT)
    {
      //  Gadgets_Reverb.Set_Screen_Dimensions(Cur_Width, Cur_Height);
        //Gadgets_Reverb.Process_Click(&Mouse);
    }
}

// ------------------------------------------------------
// Redraw the elements on the screen
void Actualize_Reverb_Ed(int gode)
{
    if(userscreen == USER_SCREEN_REVERB_EDIT)
    {
    //    Gadgets_Reverb.Set_Screen_Dimensions(Cur_Width, Cur_Height);
      //  Gadgets_Reverb.Display_List(&Reverb_IDs);

        // Echoes
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_ECHOES)
        {
            Gui_Draw_Arrows_Number_Box2(72, (Cur_Height - 135), num_echoes, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        }

        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY1)
        {
            Display_Decay_Slider(0);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY2)
        {
            Display_Decay_Slider(1);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY3)
        {
            Display_Decay_Slider(2);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY4)
        {
            Display_Decay_Slider(3);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY5)
        {
            Display_Decay_Slider(4);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY6)
        {
            Display_Decay_Slider(5);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY7)
        {
            Display_Decay_Slider(6);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY8)
        {
            Display_Decay_Slider(7);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY9)
        {
            Display_Decay_Slider(8);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DECAY10)
        {
            Display_Decay_Slider(9);
        }

        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY1)
        {
            Display_Delays_Arrows();
        }

        // 0..24999
        // -64.0f..64.0f

        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY1)
        {
            outlong_small(108, (Cur_Height - 115), delays[0], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY2)
        {
            outlong_small(108, (Cur_Height - 97), delays[1], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY3)
        {
            outlong_small(108, (Cur_Height - 79), delays[2], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY4)
        {
            outlong_small(108, (Cur_Height - 61), delays[3], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY5)
        {
            outlong_small(108, (Cur_Height - 43), delays[4], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY6)
        {
            outlong_small(108 + 185, (Cur_Height - 115), delays[5], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY7)
        {
            outlong_small(108 + 185, (Cur_Height - 97), delays[6], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY8)
        {
            outlong_small(108 + 185, (Cur_Height - 79), delays[7], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY9)
        {
            outlong_small(108 + 185, (Cur_Height - 61), delays[8], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }
        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_DELAY10)
        {
            outlong_small(108 + 185, (Cur_Height - 43), delays[9], 0, 45,
                          BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
        }

        if(gode == UPDATE_REVERB_ED_ALL || gode == UPDATE_REVERB_ED_CHANGE_NAME)
        {
            char tcp[30];
            sprintf(tcp, "%s_", Reverb_Name);

            if(snamesel == INPUT_REVERB_NAME)
            {
                Gui_Draw_Button_Box(583, (Cur_Height - 142), 164, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT);
            }
            else
            {
                Gui_Draw_Button_Box(583, (Cur_Height - 142), 164, 16, Reverb_Name, BUTTON_NORMAL | BUTTON_INPUT);
            }
        }
    }
}

// ------------------------------------------------------
// Handle right mouse buttons events
void Mouse_Right_Reverb_Ed(void)
{
    if(userscreen == USER_SCREEN_REVERB_EDIT)
    {
        if(zcheckMouse(72, (Cur_Height - 135), 16, 16))
        {
            num_echoes -= 2;
            if(num_echoes < 1) num_echoes = 1;
            Initreverb();
            gui_action = GUI_CMD_UPDATE_REVERB_ED;
            teac = UPDATE_REVERB_ED_ALL;
        }

        // Number of echoes
        if(zcheckMouse(72 + 44, (Cur_Height - 135), 16, 16))
        {
            num_echoes += 2;
            if(num_echoes > 10) num_echoes = 10;
            Initreverb();
            gui_action = GUI_CMD_UPDATE_REVERB_ED;
            teac = UPDATE_REVERB_ED_ALL;
        }

        int i;

        for(i = 0; i < num_echoes; i++)
        {
            // Arrows left
            if(zcheckMouse(Table_Arrows[i].x + 18, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i] -= 50;
                if(delays[i] < 83) delays[i] = 83;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }

            // Arrows right
            if(zcheckMouse(Table_Arrows[i].x + 82, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i] += 50;
                if(delays[i] > 15000) delays[i] = 15000;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }
        }

    }
}

// ------------------------------------------------------
// Handle left mouse buttons events
void Mouse_Left_Reverb_Ed(void)
{
    if(userscreen == USER_SCREEN_REVERB_EDIT)
    {

        // Number of echoes
        if(zcheckMouse(72, (Cur_Height - 135), 16, 16))
        {
            num_echoes--;
            if(num_echoes < 1) num_echoes = 1;
            Initreverb();
            gui_action = GUI_CMD_UPDATE_REVERB_ED;
            teac = UPDATE_REVERB_ED_ALL;
        }

        // Number of echoes
        if(zcheckMouse(72 + 44, (Cur_Height - 135), 16, 16))
        {
            num_echoes++;
            if(num_echoes > 10) num_echoes = 10;
            Initreverb();
            gui_action = GUI_CMD_UPDATE_REVERB_ED;
            teac = UPDATE_REVERB_ED_ALL;
        }

        // Save the data
        if(zcheckMouse(749, (Cur_Height - 142), 34, 16))
        {
            if(File_Exist_Req("%s" SLASH "%s.prv", Dir_Reverbs, Reverb_Name))
            {
                Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_REVERB, NULL, TRUE);
            }
            else
            {
                gui_action = GUI_CMD_SAVE_REVERB;
            }
        }

        // Start reverb name input
        if(zcheckMouse(583, (Cur_Height - 142), 164, 16) && snamesel == INPUT_NONE)
        {
            snamesel = INPUT_REVERB_NAME;
            strcpy(cur_input_name, Reverb_Name);
            namesize = 0;
            sprintf(Reverb_Name, "");
            teac = UPDATE_REVERB_ED_CHANGE_NAME;
            gui_action = GUI_CMD_UPDATE_REVERB_ED;
        }
        
        int i;

        for(i = 0; i < num_echoes; i++)
        {
            // Center buttons
            if(zcheckMouse(Table_Sliders[i].x + 120, (Cur_Height - 115) + Table_Sliders[i].y, 16, 16))
            {
                decays[i] = 0.0f;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }
        }

        for(i = 0; i < num_echoes; i++)
        {
            // Arrows left
            if(zcheckMouse(Table_Arrows[i].x + 18, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i]--;
                if(delays[i] < 83) delays[i] = 83;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }

            // Arrows right
            if(zcheckMouse(Table_Arrows[i].x + 82, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i]++;
                if(delays[i] > 15000) delays[i] = 15000;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }
        }
    }
}

// ------------------------------------------------------
// Handle sliders events
void Mouse_Sliders_Reverb_Ed(void)
{
    if(userscreen == USER_SCREEN_REVERB_EDIT)
    {
        int i;

        for(i = 0; i < num_echoes; i++)
        {
            // Rows slider
            if(zcheckMouse(Table_Sliders[i].x, (Cur_Height - 115) + Table_Sliders[i].y, 119, 16))
            {
                int max_length = 128 + 16;
                int Center = Slider_Get_Center(16, max_length, 119);
                float Pos_Mouse = ((float) ((Mouse.x - Table_Sliders[i].x) - (Center / 2) + 7)) / 119.0f;
                if(Pos_Mouse > 1.0f) Pos_Mouse = 1.0f;
                Pos_Mouse -= 0.5f;
                float s_offset = (Pos_Mouse * (float) max_length);
                if(s_offset > 63.0f) s_offset = 63.0f;
                if(s_offset < -63.0f) s_offset = -63.0f;
                decays[i] = s_offset * 0.015625f;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }
        }

        for(i = 0; i < num_echoes; i++)
        {
            // Arrows left
            if(zcheckMouse(Table_Arrows[i].x, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i]--;
                if(delays[i] < 83) delays[i] = 83;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }

            // Arrows right
            if(zcheckMouse(Table_Arrows[i].x + 82 + 18, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i]++;
                if(delays[i] > 15000) delays[i] = 15000;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }
        }
    }
}

// ------------------------------------------------------
// Handle right mouse button sliders events
void Mouse_Sliders_Right_Reverb_Ed(void)
{
    if(userscreen == USER_SCREEN_REVERB_EDIT)
    {
        int i;

        for(i = 0; i < num_echoes; i++)
        {
            // Arrows left
            if(zcheckMouse(Table_Arrows[i].x, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i] -= 50;
                if(delays[i] < 83) delays[i] = 83;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }

            // Arrows right
            if(zcheckMouse(Table_Arrows[i].x + 82 + 18, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16))
            {
                delays[i] += 50;
                if(delays[i] > 15000) delays[i] = 15000;
                teac = 0;
                gui_action = GUI_CMD_UPDATE_REVERB_ED;
                Initreverb();
                break;
            }
        }
    }
}

// ------------------------------------------------------
// Display the arrows controls
void Display_Delays_Arrows(void)
{
    int i;
    int Enabled[10] =
    {
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED,
        BUTTON_DISABLED
    };

    for(i = 0; i < num_echoes; i++)
    {
        Enabled[i] = 0;
    }

    for(i = 0; i < 10; i++)
    {
        Gui_Draw_Button_Box(Table_Arrows[i].x, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16, "\05", BUTTON_NORMAL | Enabled[i] | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        Gui_Draw_Button_Box(Table_Arrows[i].x + 18, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16, "\03", BUTTON_NORMAL | Enabled[i] | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        Gui_Draw_Button_Box(Table_Arrows[i].x + 82, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16, "\04", BUTTON_NORMAL | Enabled[i] | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
        Gui_Draw_Button_Box(Table_Arrows[i].x + 82 + 18, (Cur_Height - 115) + Table_Arrows[i].y, 16, 16, "\06", BUTTON_NORMAL | Enabled[i] | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    }
}

// ------------------------------------------------------
// Display a decay slider
void Display_Decay_Slider(int Index)
{
    Realslider_Horiz(Table_Sliders[Index].x, (Cur_Height - 115) + Table_Sliders[Index].y,
                     (int) ((decays[Index] * 64.0f) + 64.0f), 16, 128 + 16, 119, num_echoes > Index);
    Gui_Draw_Button_Box(Table_Sliders[Index].x + 120, (Cur_Height - 115) + Table_Sliders[Index].y, 16, 16, "C", BUTTON_NORMAL | (num_echoes > Index ? 0 : BUTTON_DISABLED) | BUTTON_TEXT_CENTERED);
}
