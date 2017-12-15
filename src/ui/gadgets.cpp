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
#include "include/gadgets.h"

// ------------------------------------------------------
// Allocate the memory array for all the gaddets
Gadgets::Gadgets(int Amount_Gadgets)
{
    GadgetsArray = (LPGADGET *) malloc(Amount_Gadgets * sizeof(GADGET));
    Nbr_Gadgets = 0;
}

// ------------------------------------------------------
// Allocate the memory array for all the gaddets
Gadgets::~Gadgets()
{
    int i;

    for(i = 0; i < Nbr_Gadgets; i++)
    {
        if(GadgetsArray[i]) Free(GadgetsArray[i]->id);
    }
}

// ------------------------------------------------------
// Create a gadget
GADGETID Gadgets::Create(GADGET_TYPE Type, char *string,
                         int x, int y,
                         int width, int height,
                         GADGET_ALIGN align, int user,
                         int enabled,
                         void (*button_event)(GADGETID id,
                                              int button, int wheel,
                                              int x, int y),
                         void (*draw_event)(GADGETID id,
                                            int x, int y,
                                            int width, int height,
                                            int user_dat))
{

    LPGADGET New_Gadget;
    int *Gadget_Value;
    int String_Len;

    New_Gadget = (LPGADGET) malloc(sizeof(GADGET)); 
    New_Gadget->type = Type;
    String_Len = strlen(string);
    if(string)
    {
        New_Gadget->string = (char *) malloc(String_Len + 1);
        memset(New_Gadget->string, 0, String_Len + 1);
        memcpy(New_Gadget->string, string, String_Len);
    }
    else
    {
        New_Gadget->string = NULL;
    }
    New_Gadget->x = x + 1;
    New_Gadget->y = y + 1;
    New_Gadget->width = width + 1;
    New_Gadget->align = align;
    New_Gadget->button_event = button_event;
    New_Gadget->draw_event = draw_event;
    New_Gadget->enabled = enabled;
    New_Gadget->scrollbar_pos = 0;
    New_Gadget->scrollbar_grip = 0;

    Gadget_Value = &New_Gadget->vint;
    *Gadget_Value = user;
    New_Gadget->over = FALSE;
    New_Gadget->height = height + 1;
    New_Gadget->clicked = FALSE;
    New_Gadget->id = Nbr_Gadgets + 1;

    Set_ScrollBar_Amount(New_Gadget->id, 100, 100);

    GadgetsArray[Nbr_Gadgets] = New_Gadget;
    Nbr_Gadgets++;
    return(New_Gadget->id);
}

// ------------------------------------------------------
// Store the dimension of the global container
void Gadgets::Set_Screen_Dimensions(int Width, int Height)
{
    Screen_Width = Width;
    Screen_Height = Height;
}

// ------------------------------------------------------
// Display a gadget with the given id
int Gadgets::Display(GADGETID id, int highlight)
{
    int x;
    int y;
    int j;
    int Color_Hi;
    int Color_Mid;
    int Color_Lo;
    int Color_Slider;
    COORDINATE String_Pos;
    char Value[256];
    int Remaining_Chars;
    LPGADGET Refresh_Highlight;
    LPGADGET Cur_Gadget;
    LPGADGETSTRING CurString;

    if(id < 0 || id > Nbr_Gadgets) return(FALSE);

    Cur_Gadget = __Get_From_Id(id);

    if(highlight == -1)
    {
        Refresh_Highlight = __Check_Mouse_Over(&Mouse);
        
        if(Refresh_Highlight)
        {
            highlight = Refresh_Highlight->id == id ? TRUE: FALSE;
        }
        else
        {
            highlight = FALSE;
        }
    }

    __Set_Mouse_Over_State(id, highlight);
    
    if(Cur_Gadget)
    {
        if(Cur_Gadget->enabled)
        {
            Color_Hi = highlight ? COL_INPUT_HI : COL_HI;
            Color_Mid = highlight ? COL_INPUT_MED : COL_MED;
            Color_Lo = highlight ? COL_INPUT_LO : COL_LO;
        }
        else
        {
            Color_Hi = COL_STATIC_HI;
            Color_Mid = COL_STATIC_MED;
            Color_Lo = COL_STATIC_LO;
        }
        Color_Slider = highlight ? COL_SLIDER_HI : COL_SCOPESSAMPLES;

        x = Cur_Gadget->x; 
        y = Cur_Gadget->y;
        if(x < 0) x = Screen_Width + x;
        if(y < 0) y = Screen_Height + y;

        switch(Cur_Gadget->type)
        {
            case TYPE_STATIC:
                __Get_Align(Cur_Gadget, Cur_Gadget->string, &String_Pos);
                if(Cur_Gadget->vint == GADGET_STATIC_BEVEL)
                {
                    Draw_Box(x, y, Cur_Gadget->width,
                             Cur_Gadget->height, Cur_Gadget->clicked,
                             COL_STATIC_HI, COL_STATIC_MED, COL_STATIC_LO, TRUE);
                    SetColor(COL_LO);
                    PrintString(x + String_Pos.x,
                                y + 1 + String_Pos.y,
                                USE_FONT,
                                Cur_Gadget->string);
                }
                else
                {
                    SetColor(COL_FONT_HI);
                    PrintString(x + String_Pos.x,
                                y + 1 + String_Pos.y,
                                USE_FONT,
                                Cur_Gadget->string);
                }
                if(Cur_Gadget->draw_event)
                {
                    Cur_Gadget->draw_event(Cur_Gadget->id, x, y,
                                           Cur_Gadget->width, Cur_Gadget->height,
                                           Cur_Gadget->vint);
                }
                break;

            case TYPE_BUTTON:
                Draw_Box(x, y, Cur_Gadget->width,
                         Cur_Gadget->height, Cur_Gadget->clicked,
                         Color_Hi, Color_Mid, Color_Lo, TRUE);
                __Get_Align(Cur_Gadget, Cur_Gadget->string, &String_Pos);
                SetColor(Color_Lo);
                PrintString(x + String_Pos.x,
                            y + 1 + String_Pos.y,
                            USE_FONT,
                            Cur_Gadget->string);
                break;

            case TYPE_TEXT:
                __Get_Align(Cur_Gadget, Cur_Gadget->string, &String_Pos);
                SetColor(COL_HI);
                PrintString(x + 2 + String_Pos.x,
                            y + 2,
                            USE_FONT,
                            Cur_Gadget->string);
                SetColor(COL_LO);
                PrintString(x + String_Pos.x,
                            y,
                            USE_FONT,
                            Cur_Gadget->string);
                break;

            case TYPE_NUMBER:
                memset(Value, 0, sizeof(Value));
                Draw_Box(x, y,
                         Cur_Gadget->width, Cur_Gadget->height,
                         Cur_Gadget->clicked,
                         COL_LO, COL_MED, COL_HI, TRUE);
                sprintf(Value, Cur_Gadget->string, Cur_Gadget->vint);
                __Get_Align(Cur_Gadget, Value, &String_Pos);
                SetColor(COL_FONT_HI);
                PrintString(x + String_Pos.x + 1,
                            y + String_Pos.y + 1,
                            USE_FONT,
                            Value);
                break;

            case TYPE_TIMER:
                memset(Value, 0, sizeof(Value));
                Draw_Box(x, y, Cur_Gadget->width, 
                         Cur_Gadget->height, Cur_Gadget->clicked,
                         COL_LO, COL_MED, COL_HI, TRUE);
                sprintf(Value, "%.2d:%.2d", Cur_Gadget->vint / 1000 / 60, (Cur_Gadget->vint / 1000) % 60);
                __Get_Align(Cur_Gadget, Value, &String_Pos);
                SetColor(COL_FONT_HI);
                PrintString(x + String_Pos.x + 1,
                            y + String_Pos.y + 1,
                            USE_FONT,
                            Value);
                break;

            case TYPE_NAKED_STRING:
                memset(Value, 0, sizeof(Value));

                CurString = Cur_Gadget->vstr;
                sprintf(Value, "%s", CurString->String);
                Remaining_Chars = (CurString->Max - strlen(CurString->String));
                if(Remaining_Chars > 0)
                {
                    for(j = 0; j < Remaining_Chars; j++)
                    {
                        strcat(Value, "_");
                    }
                }
                __Get_Align(Cur_Gadget, Value, &String_Pos);
                String_Pos.x = Get_Size_Text(Cur_Gadget->string) + 2;
                SetColor(COL_FONT_HI);
                PrintString(x + String_Pos.x + 1,
                            y + String_Pos.y + 1,
                            USE_FONT,
                            Value);
                break;

            case TYPE_STRING:
                memset(Value, 0, sizeof(Value));
                Draw_Box(x, y,
                         Cur_Gadget->width, Cur_Gadget->height,
                         Cur_Gadget->clicked,
                         Color_Hi, Color_Mid, Color_Lo, TRUE);
                __Get_String_Left(Cur_Gadget, Cur_Gadget->string, &String_Pos);
                SetColor(Color_Lo);
                PrintString(x + 2 + String_Pos.x,
                            y + 2 + String_Pos.y,
                            USE_FONT,
                            Cur_Gadget->string);
                SetColor(Color_Hi);
                PrintString(x + String_Pos.x,
                            y + String_Pos.y,
                            USE_FONT,
                            Cur_Gadget->string);
                
                CurString = Cur_Gadget->vstr;
                sprintf(Value, "%s", CurString->String);
                Remaining_Chars = (CurString->Max - strlen(CurString->String));
                if(Remaining_Chars > 0)
                {
                    for(j = 0; j < Remaining_Chars; j++)
                    {
                        strcat(Value, "_");
                    }
                }
                __Get_Align(Cur_Gadget, Value, &String_Pos);
                String_Pos.x = Get_Size_Text(Cur_Gadget->string) + 2;
                SetColor(COL_FONT_HI);
                PrintString(x + String_Pos.x + 1,
                            y + String_Pos.y + 1,
                            USE_FONT,
                            Value);
                break;

            case TYPE_HSCROLLBAR:
                Draw_Box(x, y,
                         Cur_Gadget->width, Cur_Gadget->height,
                         FALSE,
                         COL_LO, COL_MED, COL_HI, TRUE);
                Draw_Box(x + 4 + Cur_Gadget->scrollbar_pos, y + 4,
                         Cur_Gadget->scrollbar_innersize,
                         Cur_Gadget->height - 8, Cur_Gadget->clicked,
                         0, Color_Slider, 0, FALSE);
                break;

            case TYPE_VSCROLLBAR:
                Draw_Box(x, y, Cur_Gadget->width,
                         Cur_Gadget->height, TRUE,
                         COL_LO, COL_MED, COL_HI, TRUE);
                Draw_Box(x + 4, y + 4 + Cur_Gadget->scrollbar_pos,
                         Cur_Gadget->width - 8,
                         Cur_Gadget->scrollbar_innersize, Cur_Gadget->clicked,
                         0, Color_Slider, 0, FALSE);
                break;

            case TYPE_ARRAY:
                Draw_Box(x, y,
                         Cur_Gadget->width, Cur_Gadget->height,
                         TRUE,
                         COL_LO, COL_BACKGROUND, COL_HI, TRUE);
                if(Cur_Gadget->draw_event)
                {
                    Cur_Gadget->draw_event(Cur_Gadget->id,
                                           x + 2, y + 2,
                                           Cur_Gadget->width - 4, Cur_Gadget->height - 4,
                                           Cur_Gadget->vint);
                }
                break;
        }
        return(TRUE);
    }
    return(FALSE);
}

// ------------------------------------------------------
// Draw a bevelled box on the screen
void Gadgets::Draw_Box(int x, int y,
                       int width, int height,
                       int reversed,
                       int colhi, int colmid, int collo,
                       int bevel)
{
    int Color;

    SetColor(colmid);
    if(bevel)
    {
        Fillrect(x - 1 + (reversed ? 2 : 0), y - 1 + (reversed ? 2 : 0),
                 x + width - 1 - (reversed ? 2 : 0),
                 y + height - 1 - (reversed ? 2 : 0));
        Color = reversed ? colhi : collo;
        DrawVLine(x + width - 2, y, y + height - 2, Color);
        DrawHLine(y + height - 2, x, x + width - 2, Color);
        Color = reversed ? collo : colhi;
        DrawVLine(x - 1, y - 1, y + height - 3, Color);
        DrawHLine(y - 1, x - 1, x + width - 3, Color);
    }
    else
    {
        Fillrect(x, y, x + width - 2, y + height - 2);
    }
}

// ------------------------------------------------------
// Set the enabled state of a gadget
int Gadgets::Set_Enabled(GADGETID id, int enabled)
{
    int old_state = FALSE;
    LPGADGET Cur_Gadget = __Get_From_Id(id);
    
    if(Cur_Gadget)
    {
        old_state = Cur_Gadget->enabled;
        Cur_Gadget->enabled = enabled;
        Display(id, GADGET_NOHIGHLIGHT);
    }
    return(old_state);
}

// ------------------------------------------------------
// Retrieve the enabled state of a gadget
int Gadgets::Get_Enabled(GADGETID id)
{
    LPGADGET Cur_Gadget = __Get_From_Id(id);

    return(Cur_Gadget->enabled);
}

// ------------------------------------------------------
// Retrieve the enabled state of a gadget
LPGADGET Gadgets::__Get_From_Id(GADGETID id)
{
    int i;
    LPGADGET Cur_Gadget;

    for(i = 0; i < Nbr_Gadgets; i++)
    {
        Cur_Gadget = GadgetsArray[i];
        if(Cur_Gadget->id == id)
        {
            return(Cur_Gadget);
        }
    }
    return(NULL);
}

// ------------------------------------------------------
// Return the left position of a string in a gadget
void Gadgets::__Get_String_Left(LPGADGET gadget,
                                char *string, LPCOORDINATE pos)
{
    pos->x = 3;
    pos->y = (((gadget->height - 1) - Get_Font_Height()) / 2);
    if(pos->y < 1) pos->y = 1;
}

// ------------------------------------------------------
// Return the left position of a string in a gadget
void Gadgets::__Get_String_Center(LPGADGET gadget,
                                  char *string, LPCOORDINATE pos)
{
    pos->x = (((gadget->width - 1) - Get_Size_Text(string)) / 2) - 1;
    if(pos->x < -1) pos->x = -1;
    pos->y = (((gadget->height - 1) - Get_Font_Height()) / 2);
    if(pos->y < 0) pos->y = 0;
}

// ------------------------------------------------------
// Return the right justified position of a string in a gadget
void Gadgets::__Get_String_Right(LPGADGET gadget,
                                 char *string, LPCOORDINATE pos)
{
    pos->x = (((gadget->width - 1) - Get_Size_Text(string))) - 3;
    if(pos->x < 1) pos->x = 1;
    pos->y = (((gadget->height - 1) - Get_Font_Height()) / 2);
    if(pos->y < 1) pos->y = 1;
}

// ------------------------------------------------------
// Retrieve the assigned alignement of a gadget
void Gadgets::__Get_Align(LPGADGET gadget,
                          char *string, LPCOORDINATE pos)
{
    switch(gadget->align)
    {
        case ALIGN_CENTER:
            __Get_String_Center(gadget, string, pos);
            break;
        case ALIGN_RIGHT:
            __Get_String_Right(gadget, string, pos);
            break;
        default:
            __Get_String_Left(gadget, string, pos);
            break;
    }
}

// ------------------------------------------------------
// Check if a gadget is located within a gadget's boundaries
LPGADGET Gadgets::__Check_Mouse_Over(LPMOUSE mouse)
{
    int i;
    LPGADGET Cur_Gadget;
    int Gadget_Type;
    LPGADGET Clicked_Gadget;
    int x = mouse->x + 1;
    int y = mouse->y + 1;
    int Gadget_X;
    int Gadget_Y;
    int Bound_Width;
    int Bound_Height;
    int Bound_Left;
    int Bound_Top;

    Clicked_Gadget = __Check_Clicked();
    if(Clicked_Gadget)
    {
        if((mouse->button != MOUSE_LEFT_BUTTON) &&
           (mouse->button != MOUSE_RIGHT_BUTTON) &&
           (mouse->button != MOUSE_MIDDLE_BUTTON))
        {
            Clicked_Gadget->clicked = FALSE;
            
            Gadget_X = Clicked_Gadget->x;
            Gadget_Y = Clicked_Gadget->y;
            if(Gadget_X < 0) Gadget_X = Screen_Width + Gadget_X;
            if(Gadget_Y < 0) Gadget_Y = Screen_Height + Gadget_Y;
            
            // Check if the mouse is still over the gadget
            if(Check_Coordinates(mouse->x, mouse->y,
                                 BOUNDRECT(Gadget_X, Gadget_Y,
                                           Clicked_Gadget->width,
                                           Clicked_Gadget->height
                                          )
                                )
              )
            {
                // Button up event sent to the gadget
                __Run_Event(mouse, Clicked_Gadget, TRUE);
            }
        }
        if(Clicked_Gadget->type == TYPE_ARRAY) return(NULL);
        return(Clicked_Gadget);
    }
    for(i = 0; i < Nbr_Gadgets; i++)
    {
        Cur_Gadget = GadgetsArray[i];
        Gadget_Type = __Get_Type(Cur_Gadget->id);
        switch(Gadget_Type)
        {
            case TYPE_HSCROLLBAR:
                Bound_Left = 5 + Cur_Gadget->scrollbar_pos;
                Bound_Top = 5;
                Bound_Width = Cur_Gadget->scrollbar_innersize - (GADGET_STATIC_BEVEL * 2);
                Bound_Height = Cur_Gadget->height;
                break;

            case TYPE_VSCROLLBAR:
                Bound_Left = 5;
                Bound_Top = 5 + Cur_Gadget->scrollbar_pos;
                Bound_Width = Cur_Gadget->width;
                Bound_Height = Cur_Gadget->scrollbar_innersize - (GADGET_STATIC_BEVEL * 2);
                break;

            default:
                Bound_Left = 0;
                Bound_Top = 0;
                Bound_Width = Cur_Gadget->width;
                Bound_Height = Cur_Gadget->height;
                break;
        }

        Gadget_X = Cur_Gadget->x;
        Gadget_Y = Cur_Gadget->y;
        if(Gadget_X < 0) Gadget_X = Screen_Width + Gadget_X;
        if(Gadget_Y < 0) Gadget_Y = Screen_Height + Gadget_Y;

        if(Check_Coordinates(x, y,
                             BOUNDRECT(Gadget_X + Bound_Left,
                                       Gadget_Y + Bound_Top,
                                       Bound_Width - (Bound_Left * 2),
                                       Bound_Height)))
        {
            if(Gadget_Type == TYPE_BUTTON ||
               Gadget_Type == TYPE_STRING ||
               Gadget_Type == TYPE_HSCROLLBAR ||
               Gadget_Type == TYPE_VSCROLLBAR)
            {
                return(Cur_Gadget);
            }
        }
    }
    return(NULL);
}

// ------------------------------------------------------
// Check if a gadget is located within boundaries
void Gadgets::__End_Mouse_Over()
{
    int i;
    LPGADGET Cur_Gadget;

    Cur_Gadget = __Check_Clicked();
    if(Cur_Gadget)
    {
        if(Cur_Gadget->type != TYPE_ARRAY)
        {
            Display(Cur_Gadget->id, GADGET_NOHIGHLIGHT);
        }
        return;
    }
    for(i = 0; i < Nbr_Gadgets; i++)
    {
        Cur_Gadget = GadgetsArray[i];
        if(Cur_Gadget->over)
        {
            Display(Cur_Gadget->id, GADGET_NOHIGHLIGHT);
            return;
        }
    }
}

// ------------------------------------------------------
// Set the mouse over status of a gadget from a given id
void Gadgets::__Set_Mouse_Over_State(GADGETID id, int status)
{
    LPGADGET Cur_Gadget;

    Cur_Gadget = __Get_From_Id(id);
    if(Cur_Gadget)
    {
        Cur_Gadget->over = status;
    }
}

// ------------------------------------------------------
// Return the type of a gadget
int Gadgets::__Get_Type(GADGETID id)
{
    LPGADGET Cur_Gadget;

    Cur_Gadget = __Get_From_Id(id);
    if(Cur_Gadget)
    {
        return(Cur_Gadget->type);
    }
    return(-1);
}

// ------------------------------------------------------
// Return the id of a gadget
int Gadgets::__Get_Id(LPGADGET gadget)
{
    if(gadget) return(gadget->id);
    return(-1);
}

// ------------------------------------------------------
// Check if a gadget gas been clicked and trigger the associated event
void Gadgets::Process_Click(LPMOUSE mouse)
{
    int i;
    LPGADGET Cur_Gadget;
    int Gadget_Type;
    int Gadget_X;
    int Gadget_Y;
    int x = mouse->x + 1;
    int y = mouse->y + 1;

    __End_Mouse_Over();
    Display(__Get_Id(__Check_Mouse_Over(&Mouse)), GADGET_HIGHLIGHT);

    for(i = 0; i < Nbr_Gadgets; i++)
    {
        Cur_Gadget = GadgetsArray[i];
        if(Cur_Gadget->enabled)
        {
            Gadget_X = Cur_Gadget->x;
            Gadget_Y = Cur_Gadget->y;
            if(Gadget_X < 0) Gadget_X = Screen_Width + Gadget_X;
            if(Gadget_Y < 0) Gadget_Y = Screen_Height + Gadget_Y;

            if(Cur_Gadget->clicked &&
               (Cur_Gadget->type == TYPE_HSCROLLBAR ||
                Cur_Gadget->type == TYPE_VSCROLLBAR))
            {
                if(mouse->button == MOUSE_LEFT_BUTTON ||
                   mouse->button == MOUSE_RIGHT_BUTTON ||
                   mouse->button == MOUSE_MIDDLE_BUTTON)
                {
                    Cur_Gadget->last_button = mouse->button;
                    Cur_Gadget->clicked = TRUE;
                    switch(Cur_Gadget->type)
                    {
                        case TYPE_HSCROLLBAR:
                        case TYPE_VSCROLLBAR:
                            __Run_Event(mouse, Cur_Gadget, FALSE);
                            break;
                        default:
                            break;
                    }
                }
                if(mouse->wheel)
                {
                    __Run_Event(mouse, Cur_Gadget, FALSE);
                }
            }
            else if(Check_Coordinates(x, y,
                                      BOUNDRECT(Gadget_X, Gadget_Y,
                                                Cur_Gadget->width,
                                                Cur_Gadget->height)))
            {
                Gadget_Type = __Get_Type(Cur_Gadget->id);
                if(Gadget_Type == TYPE_BUTTON || Gadget_Type == TYPE_STRING ||
                   Gadget_Type == TYPE_ARRAY || Gadget_Type == TYPE_HSCROLLBAR ||
                   Gadget_Type == TYPE_VSCROLLBAR)
                {
                    if(!__Check_Clicked())
                    {
                        if(mouse->button == MOUSE_LEFT_BUTTON ||
                           mouse->button == MOUSE_RIGHT_BUTTON ||
                           mouse->button == MOUSE_MIDDLE_BUTTON)
                        {
                            Cur_Gadget->last_button = mouse->button;
                            Cur_Gadget->clicked = TRUE;
                            switch(Cur_Gadget->type)
                            {
                                case TYPE_HSCROLLBAR:
                                case TYPE_VSCROLLBAR:
                                    // Set the grip coordinates for first event
                                    __Calc_Scrollbar_Grip(mouse, Cur_Gadget);
                                    __Run_Event(mouse, Cur_Gadget, FALSE);
                                    break;
                                default:
                                    break;
                            }
                        }
                        if(mouse->wheel)
                        {
                            __Run_Event(mouse, Cur_Gadget, FALSE);
                        }
                    }
                }
            }
        }
    }
}

// ------------------------------------------------------
// Check if a gadget has already been clicked
LPGADGET Gadgets::__Check_Clicked(void)
{
    LPGADGET Cur_Gadget;
    int i;

    for(i = 0; i < Nbr_Gadgets; i++)
    {
        Cur_Gadget = GadgetsArray[i];
        if(Cur_Gadget->clicked)
        {
            return(Cur_Gadget);
        }
    }
    return(NULL);
}

// ------------------------------------------------------
// Check if a coordinate is within boundaries
int Gadgets::Check_Coordinates(int srcx, int srcy,
                               BOUNDRECT dst)
{
    if(srcx >= dst.x && srcx < dst.x + dst.width)
    {
        if(srcy >= dst.y && srcy < dst.y + dst.height)
        {
            return(TRUE);
        }
    }
    return(FALSE);
}

// ------------------------------------------------------
// Run the associated event of a gadget
void Gadgets::__Run_Event(LPMOUSE mouse,
                          LPGADGET gadget,
                          int Last_Button)
{
    LOCALPOINT local;

    if(gadget->button_event != NULL)
    {
        local.Load(mouse, gadget);
        gadget->button_event(gadget->id,
                             Last_Button ? gadget->last_button : mouse->button,
                             mouse->wheel,
                             local.x, local.y);
    }
}

// ------------------------------------------------------
// Create a pool of gadgets from a list
int Gadgets::Create_List(USERGADGET GadgetsList[],
                         LPGADGETPOOL Pool)
{
    int i;
    int Nbr_Gadgets_To_Create;

    Nbr_Gadgets_To_Create = 0;
    while(GadgetsList[Nbr_Gadgets_To_Create].string)
    {
        Nbr_Gadgets_To_Create++;
    }

    Pool->Amount = Nbr_Gadgets_To_Create;
    Pool->Id = (GADGETID *) malloc(Nbr_Gadgets * sizeof(GADGETID));
    memset(Pool->Id, 0, Nbr_Gadgets * sizeof(GADGETID));

    if(Pool->Id)
    {
        for(i = 0; i < Nbr_Gadgets_To_Create; i++)
        {
            Pool->Id[i] = Create(GadgetsList->type,
                                 GadgetsList->string,
                                 GadgetsList->x,
                                 GadgetsList->y,
                                 GadgetsList->width,
                                 GadgetsList->height,
                                 GadgetsList->align,
                                 GadgetsList->user,
                                 GadgetsList->enabled,
                                 GadgetsList->button_event,
                                 GadgetsList->draw_event);
            GadgetsList++;
        }
        return(TRUE);
    }
    return(FALSE);
}

// ------------------------------------------------------
// Free a gadget
void Gadgets::Free(GADGETID id)
{
    LPGADGET Gadget = __Get_From_Id(id);
    if(Gadget)
    {
        if(Gadget->string) free(Gadget->string);
    }
}

// ------------------------------------------------------
// Free a list of gadgets
void Gadgets::Free_List(LPGADGETPOOL Pool)
{
    int i;

    for(i = 0; i < Pool->Amount; i++)
    {
        Free(Pool->Id[i]);
    }

    if(Pool->Id) free(Pool->Id);
}

// ------------------------------------------------------
// Display a pool of gadgets
void Gadgets::Display_List(LPGADGETPOOL Pool)
{
    int i;

    for(i = 0; i < Pool->Amount; i++)
    {
        Display(Pool->Id[i], GADGET_NOHIGHLIGHT);
    }
}

// ------------------------------------------------------
// Set the value of a scrollbar gadget
int Gadgets::__Calc_ScrollBar_Value(GADGETID id,
                                    int value, int max)
{
    float Scrollbar_Max_Size;
    float Scrollbar_Size;
    LPGADGET Gadget = __Get_From_Id(id);

    if(Gadget)
    {
        if(value > max) value = max;
        Gadget->scrollbar_amount = value;
        Gadget->scrollbar_max = max;
        Scrollbar_Size = (float) Gadget->scrollbar_amount / (float) Gadget->scrollbar_max;

        switch(Gadget->type)
        {
            case TYPE_HSCROLLBAR:
                Scrollbar_Max_Size = (float) (Gadget->width - 8 - Gadget->vint);
                Scrollbar_Max_Size *= Scrollbar_Size;
                Gadget->scrollbar_innersize = (int) Scrollbar_Max_Size;
                return(TRUE);

            case TYPE_VSCROLLBAR:
                Scrollbar_Max_Size = (float) (Gadget->height - 8 - Gadget->vint);
                Scrollbar_Max_Size *= Scrollbar_Size;
                Gadget->scrollbar_innersize = (int) Scrollbar_Max_Size;
                return(TRUE);

            default:
                break;
        }
    }
    return(FALSE);
}


// ------------------------------------------------------
// Return the size of a scrollbar gadget
float Gadgets::__Get_ScrollBar_Max(LPGADGET gadget)
{
    switch(gadget->type)
    {
        case TYPE_HSCROLLBAR:
            return((float) (gadget->width - 8 - gadget->vint));
            break;

        case TYPE_VSCROLLBAR:
            return((float) (gadget->height - 8 - gadget->vint));
            break;

        default:
            break;
    }
    return(0.0f);
}

// ------------------------------------------------------
// Set the position of a scrollbar gadget
int Gadgets::__Calc_ScrollBar_Pos(GADGETID id, int value)
{
    float Scrollbar_Max_Size;
    LPGADGET Gadget = __Get_From_Id(id);
    int Center;
    int maximum;

    if(Gadget)
    {
        Scrollbar_Max_Size = __Get_ScrollBar_Max(Gadget);

        switch(Gadget->type)
        {
            case TYPE_HSCROLLBAR:
            case TYPE_VSCROLLBAR:
                Center = (Gadget->scrollbar_innersize / 2) + Gadget->scrollbar_grip;
                Gadget->scrollbar_pos = value - Center;
                if(Gadget->scrollbar_pos < 0)
                {
                    Gadget->scrollbar_pos = 0;
                }
                maximum = (int) Scrollbar_Max_Size - Gadget->scrollbar_innersize;
                if(Gadget->scrollbar_pos > maximum)
                {
                    Gadget->scrollbar_pos = maximum;
                }
                return(TRUE);

            default:
                break;
        }
    }
    return(FALSE);
}

// ------------------------------------------------------
// Set the mouse grip position inside a scrollbar
void Gadgets::__Calc_Scrollbar_Grip(LPMOUSE mouse,
                                    LPGADGET gadget)
{
    LOCALPOINT local;
    int local_coord;
    int gap = 0;

    local.Load(mouse, gadget);

    switch(gadget->type)
    {
        case TYPE_HSCROLLBAR:
            gap = 4;
            local_coord = local.x;
            break;

        case TYPE_VSCROLLBAR:
            gap = 4;
            local_coord = local.y;
            break;
    }
    
    if((local_coord - gap) < gadget->scrollbar_pos)
    {
        gadget->scrollbar_grip = 0;
        return;
    }
    if((local_coord - (gap / 2)) >= gadget->scrollbar_pos + gadget->scrollbar_innersize)
    {
        gadget->scrollbar_grip = 0;
        return;
    }

    gadget->scrollbar_grip = local_coord - gadget->scrollbar_pos;
    gadget->scrollbar_grip -= (gadget->scrollbar_innersize / 2);
}

// ------------------------------------------------------
// Set the current and maximum values of a scrollbar and re-display it
void Gadgets::Set_ScrollBar_Amount(GADGETID id,
                                   int value, int max)
{
    if(__Calc_ScrollBar_Value(id, value, max))
    {
        Display(id, GADGET_NOHIGHLIGHT);
    }
}

// ------------------------------------------------------
// Set the position of a scrollbar and re-display it
int Gadgets::Set_ScrollBar_By_Pos(GADGETID id, int value)
{
    float Pos;
    float Max;
    float Amount;
    float Max_Amount;
    LPGADGET Gadget = __Get_From_Id(id);

    if(Gadget)
    {
        if(__Calc_ScrollBar_Pos(id, value))
        {
            Display(id, Gadget->clicked);
        }

        Pos = (float) Gadget->scrollbar_pos;
        Max = __Get_ScrollBar_Max(Gadget) - Gadget->scrollbar_innersize;
        Amount = (float) Gadget->scrollbar_amount;
        Max_Amount = (float) Gadget->scrollbar_max;
    
        Pos = Pos / Max;
        Amount = (Max_Amount - Amount);

        return((int) (Pos * Amount));
    }
    return(0);
}

// ------------------------------------------------------
// Set the position of a scrollbar and re-display it
void Gadgets::Set_ScrollBar_By_Value(GADGETID id, int value)
{
    float Max_Amount;
    float fltValue;
    float Scrollbar_Max_Size;
    float Maximum;
    LPGADGET Gadget = __Get_From_Id(id);

    if(Gadget)
    {
        Max_Amount = (float) Gadget->scrollbar_max - (float) Gadget->scrollbar_amount;

        if(value > (int) Max_Amount) value = (int) Max_Amount;

        fltValue = (float) value;
        fltValue /= Max_Amount;     // Normalize

        Scrollbar_Max_Size = __Get_ScrollBar_Max(Gadget);
        Maximum = Scrollbar_Max_Size - Gadget->scrollbar_innersize;
        fltValue *= Maximum;
        Gadget->scrollbar_pos = (int) fltValue;

        Display(id, Gadget->clicked);
    }
}

// ------------------------------------------------------
// Set the position of a scrollbar and re-display it
void Gadgets::Set_Timer(GADGETID id, int value)
{
    LPGADGET Gadget = __Get_From_Id(id);
   
    if(Gadget)
    {
        Gadget->vint = value;
        Display(id, GADGET_NOHIGHLIGHT);
    }
}
