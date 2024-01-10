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

#ifndef _GADGETS_H_
#define _GADGETS_H_

// ------------------------------------------------------
// Includes
#if defined(__GCC__)
#include "../../support/include/main.h"
#include "../../include/variables.h"
#else
#include "../support/include/main.h"
#include "../include/variables.h"
#endif
#include "misc_draw.h"

// ------------------------------------------------------
// Constants
enum GADGET_TYPE
{
    TYPE_STATIC,
    TYPE_BUTTON,
    TYPE_TEXT,
    TYPE_NUMBER,
    TYPE_TIMER,
    TYPE_STRING,
    TYPE_NAKED_STRING,
    TYPE_HSCROLLBAR,
    TYPE_VSCROLLBAR,
    TYPE_ARRAY
};

enum GADGET_ALIGN
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

#define GADGET_MOUSE_OVER 1

#define GADGET_STATIC_NOBEVEL 0
#define GADGET_STATIC_BEVEL -1

#define GADGET_REFRESH -1
#define GADGET_HIGHLIGHT TRUE
#define GADGET_NOHIGHLIGHT FALSE

typedef int GADGETID;

// ------------------------------------------------------
// Structures
typedef struct
{
    int x;
    int y;
} COORDINATE, *LPCOORDINATE;

typedef struct
{
    int Max;
    char *String;
} GADGETSTRING, *LPGADGETSTRING;

typedef struct
{
    int Amount;
    GADGETID *Id;
} GADGETPOOL, *LPGADGETPOOL;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
    GADGET_TYPE type;
    GADGETID id;
    void (*button_event)(GADGETID id,
                         int button, int wheel,
                         int x, int y);
    void (*draw_event)(GADGETID id,
                       int x, int y,
                       int width, int height,
                       int user_dat);
    union
    {
        int vint;
        float vflt;
        LPGADGETSTRING vstr;
    };
    GADGET_ALIGN align;
    char *string;
    int clicked;
    int over;
    int enabled;
    int last_button;
    int scrollbar_amount;
    int scrollbar_max;
    int scrollbar_innersize;
    int scrollbar_pos;
    int scrollbar_grip;
} GADGET, *LPGADGET;

typedef struct
{
    GADGET_TYPE type;
    char *string;
    int x;
    int y;
    int width;
    int height;
    GADGET_ALIGN align;
    int user;
    int enabled;
    void (*button_event)(GADGETID id,
                         int button, int wheel,
                         int x, int y);
    void (*draw_event)(GADGETID id,
                       int x, int y,
                       int width, int height,
                       int user_dat);
} USERGADGET, *LPUSERGADGET;

// ------------------------------------------------------
// Classes
class BOUNDRECT
{
    public:
        BOUNDRECT(int x, int y,
                  int width, int height)
        {
            this->x  = x;
            this->y  = y;
            this->width = width;
            this->height = height;
        }
        ~BOUNDRECT()
        {
        }
        int x;
        int y;
        int width;
        int height;
};

class LOCALPOINT
{
    public:
        LOCALPOINT()
        {
        }
        ~LOCALPOINT()
        {
        }
        void Load(LPMOUSE mouse, LPGADGET gadget)
        {
            this->x = mouse->x - gadget->x;
            this->y = mouse->y - gadget->y;
            if(this->x < 0) this->x = 0;
            if(this->y < 0) this->y = 0;
            if(this->x > gadget->width) this->x = gadget->width;
            if(this->y > gadget->height) this->y = gadget->height;
        }
        int x;
        int y;
};

class Gadgets
{
    public:

        Gadgets(int Amount_Gadgets);
        ~Gadgets();

        GADGETID Create(GADGET_TYPE Type,
                        char *string,
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
                                           int user_dat)
                       );
        int Display(GADGETID id, int Highlight);
        void Display_List(LPGADGETPOOL Pool);
        int Get_Enabled(GADGETID id);
        int Set_Enabled(GADGETID id, int enabled);
        void Process_Click(LPMOUSE Mouse);
        int Create_List(USERGADGET Gadgets[], LPGADGETPOOL Pool);
        void Free_List(LPGADGETPOOL Pool);
        void Set_ScrollBar_Amount(GADGETID id, int value, int max);
        int Set_ScrollBar_By_Pos(GADGETID id, int value);
        void Set_ScrollBar_By_Value(GADGETID id, int value);
        void Set_Timer(GADGETID id, int Value);
        void Set_Screen_Dimensions(int Width, int Height);

    private:

        LPGADGET __Get_From_Id(GADGETID id);
        void __Get_String_Left(LPGADGET gadget, char *string, LPCOORDINATE pos);
        void __Get_String_Center(LPGADGET gadget, char *string, LPCOORDINATE pos);
        void __Get_String_Right(LPGADGET gadget, char *string, LPCOORDINATE pos);
        void __Get_Align(LPGADGET gadget, char *string, LPCOORDINATE pos);
        LPGADGET __Check_Mouse_Over(LPMOUSE mouse);
        void __End_Mouse_Over();
        void __Set_Mouse_Over_State(GADGETID id, int status);
        int __Get_Type(GADGETID id);
        int __Get_Id(LPGADGET gadget);
        void __Calc_Scrollbar_Grip(LPMOUSE mouse, LPGADGET gadget);
        int __Calc_ScrollBar_Value(GADGETID id, int value, int max);
        float __Get_ScrollBar_Max(LPGADGET gadget);
        int __Calc_ScrollBar_Pos(GADGETID id, int value);
        LPGADGET __Check_Clicked();
        void __Run_Event(LPMOUSE mouse, LPGADGET gadget, int Last_Button);
        void Draw_Box(int x, int y,
                      int width, int height,
                      int reversed,
                      int colhi, int colmid, int collo,
                      int bevel);
        void Free(GADGETID id);
        int Check_Coordinates(int srcx, int srcy,
                              BOUNDRECT dst);
        int Nbr_Gadgets;
        LPGADGET *GadgetsArray;
        int Screen_Width;
        int Screen_Height;
};

// ------------------------------------------------------
// Functions
LPGADGET *Gadget_Array_Init(int Amount_Gadgets);

#endif
