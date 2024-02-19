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
#include "include/draw_primitives.h"
#include "../extralibs/sdl_draw/include/sdl_draw.h"
#include "../include/ptk.h"

// ------------------------------------------------------
// Variables
extern SDL_Surface *Main_Screen;
extern SDL_Surface *Temp_PFONT;
extern SDL_Surface *Temp_LARGEPFONT;
extern SDL_Surface *Temp_SMALLPFONT;
extern SDL_Surface *Temp_NOTEPFONT;
extern SDL_Surface *Temp_NOTELARGEPFONT;
extern SDL_Surface *Temp_NOTESMALLPFONT;
extern char *Font_Ascii;
extern int Nbr_Letters;
extern int Font_Pos[256];
extern int Font_Size[256];
#if defined(__MACOSX_PPC__)
extern unsigned char *Pointer_BackBuf;
#endif
int FgColor;

int Nbr_Update_Rects;
SDL_Rect Update_Stack[UPDATE_STACK_SIZE];

// ------------------------------------------------------
// Functions
int Get_Char_Position(char *Ascii_Letters, int Max_Letters, char Letter);

// ------------------------------------------------------
// Draw a pixel
void DrawPixel(int x, int y, int Color)
{
    Draw_Pixel(Main_Screen, x, y, Color);
}

// ------------------------------------------------------
// Draw an horizontal line
void DrawHLine(int y, int x1, int x2, int Color)
{
    Draw_HLine(Main_Screen, x1, y, x2, Color);
    Push_Update_Rect(x1, y, x2 - x1, y + 1);
}

// ------------------------------------------------------
// Draw a vertical line
void DrawVLine(int x, int y1, int y2, int Color)
{
    Draw_VLine(Main_Screen, x, y1, y2, Color);
    Push_Update_Rect(x, y1, x + 1, y2 - y1);
}

// ------------------------------------------------------
// Set the current color
void SetColor(int color)
{
    FgColor = color;
}

// ------------------------------------------------------
// Fill a rectangle with the current color
void Fillrect(int x1, int y1, int x2, int y2)
{
    SDL_Rect Dst_Rect;
    Dst_Rect.x = x1;
    Dst_Rect.y = y1;
    Dst_Rect.w = x2 - x1;
    Dst_Rect.h = y2 - y1;
    SDL_FillRect(Main_Screen, &Dst_Rect, FgColor);
    Push_Update_Rect(x1, y1, x2 - x1, y2 - y1);
}

// ------------------------------------------------------
// Set the current palette
void UISetPalette(SDL_Color *Palette, int Amount)
{
    if(FONT_LOW)
    {
        SDL_SetPalette(FONT_LOW, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(FONT)
    {
        SDL_SetPalette(FONT, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(SKIN303)
    {
        SDL_SetPalette(SKIN303, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(LOGOPIC)
    {
        SDL_SetPalette(LOGOPIC, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(Temp_PFONT)
    {
        SDL_SetPalette(Temp_PFONT, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(Temp_LARGEPFONT)
    {
        SDL_SetPalette(Temp_LARGEPFONT, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(Temp_SMALLPFONT)
    {
        SDL_SetPalette(Temp_SMALLPFONT, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(Temp_NOTEPFONT)
    {
        SDL_SetPalette(Temp_NOTEPFONT, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(Temp_NOTELARGEPFONT)
    {
        SDL_SetPalette(Temp_NOTELARGEPFONT, SDL_LOGPAL, Palette, 0, Amount);
    }
    if(Temp_NOTESMALLPFONT)
    {
        SDL_SetPalette(Temp_NOTESMALLPFONT, SDL_LOGPAL, Palette, 0, Amount);
    }

#if defined(__MACOSX_PPC__)
    if(POINTER)
    {
        SDL_SetPalette(POINTER, SDL_LOGPAL, Palette, 0, Amount);
    }
#endif

    SDL_SetPalette(Main_Screen, SDL_PHYSPAL, Palette, 0, Amount);
    SDL_SetPalette(Main_Screen, SDL_LOGPAL, Palette, 0, Amount);
}

// ------------------------------------------------------
// Copy a surface onto the main screen
void Copy(SDL_Surface *Source,
          int x, int y,
          int x1, int y1,
          int x2, int y2)
{
    SDL_Rect Src_Rect;
    SDL_Rect Dst_Rect;

    Dst_Rect.x = x;
    Dst_Rect.y = y;
    Dst_Rect.w = (x2 - x1) + 1;
    Dst_Rect.h = (y2 - y1) + 1;

    Src_Rect.x = x1;
    Src_Rect.y = y1;
    Src_Rect.w = Dst_Rect.w;
    Src_Rect.h = Dst_Rect.h;

    SDL_BlitSurface(Source, &Src_Rect, Main_Screen, &Dst_Rect);
    Push_Update_Rect(x, y, Dst_Rect.w, Dst_Rect.h);
}

// ------------------------------------------------------
// Copy a rectangle onto a given surface
void Copy_To_Surface(SDL_Surface *Source, SDL_Surface *dest,
                     int x, int y, int x1, int y1, int x2, int y2)
{
    SDL_Rect Src_Rect;
    SDL_Rect Dst_Rect;

    Src_Rect.x = x1;
    Src_Rect.y = y1;
    Src_Rect.w = (x2 - x1);
    Src_Rect.h = (y2 - y1);

    Dst_Rect.x = x;
    Dst_Rect.y = y;
    Dst_Rect.w = (x2 - x1);
    Dst_Rect.h = (y2 - y1);

    SDL_BlitSurface(Source, &Src_Rect, dest, &Dst_Rect);
    Push_Update_Rect(x, y, x2 - x1, y2 - y1);
}

// ------------------------------------------------------
// Print a string on the screen
void PrintString(int x,
                 int y,
                 int Font_Type,
                 char *String,
                 int max_x)
{
    int Idx;
    int Idx2;
    int i;
    int pos_x;
    int start_x;
    int early_exit = FALSE;
    SDL_Rect Src_Rect;
    SDL_Rect Dst_Rect;

    /* FIX: this should be compensated by the gadget class */
    y += 2;

    Dst_Rect.y = y;
    Dst_Rect.h = Font_Height;
    Src_Rect.y = 0;
    Src_Rect.h = Font_Height;

    start_x = x;

    for(i = 0; i < (int) strlen(String); i++)
    {
        Idx = Get_Char_Position(Font_Ascii, Nbr_Letters, String[i]);
        pos_x = x;
        
        if(max_x != -1)
        {
            Idx2 = Get_Char_Position(Font_Ascii, Nbr_Letters, '\015');
            if((pos_x + Font_Size[Idx2]) >= ((start_x + max_x) - (Font_Size[Idx2]) - 4))
            {
                Idx = Idx2;
                early_exit = TRUE;
            }
        }

        Src_Rect.x = Font_Pos[Idx];
        Src_Rect.w = Font_Size[Idx];
        Dst_Rect.x = pos_x;
        Dst_Rect.w = Src_Rect.w;

        if(Font_Type == USE_FONT)
        {
            SDL_BlitSurface(FONT, &Src_Rect, Main_Screen, &Dst_Rect);
        }
        else
        {
            SDL_BlitSurface(FONT_LOW, &Src_Rect, Main_Screen, &Dst_Rect);
        }
        x += Font_Size[Idx];
        if(early_exit) break;
    }
}

// ------------------------------------------------------
// Display or clear the mouse pointer at given coordinates
#if defined(__MACOSX_PPC__)
void Display_Mouse_Pointer(int x, int y, int clear)
{
    int was_locked;
    int main_was_locked;

    if(x >= Cur_Width) return;
    if(y >= Cur_Height) return;
    
    main_was_locked = FALSE;
    if(SDL_MUSTLOCK(Main_Screen))
    {
        if(!SDL_LockSurface(Main_Screen)) main_was_locked = TRUE;
    }

    was_locked = FALSE;
    if(SDL_MUSTLOCK(POINTER))
    {
        if(!SDL_LockSurface(POINTER)) was_locked = TRUE;
    }

    int i;
    int j;
    int Src_offset;
    int Dst_offset;
    int Len_Dst = Main_Screen->pitch * Main_Screen->h;
    unsigned char *SrcPix = (unsigned char *) POINTER->pixels;
    unsigned char *DstPix = (unsigned char *) Main_Screen->pixels;

    for(j = 0; j < POINTER->h; j++)
    {
        for(i = 0; i < POINTER->w; i++)
        {
            Src_offset = (j * POINTER->pitch) + i;
            Dst_offset = ((j + y) * Main_Screen->pitch) + (i + x);
            if(Dst_offset >= 0)
            {
                if(((i + x) < (Cur_Width - 1)) &&
                   ((j + y) < (Cur_Height - 1)))
                {
                    if(clear)
                    {
                        if(SrcPix[Src_offset])
                        {
                            DrawPixel((i + x), (j + y), Pointer_BackBuf[Src_offset]);
                        }
                    } 
                    else
                    {
                        if(SrcPix[Src_offset])
                        {
                            Pointer_BackBuf[Src_offset] = DstPix[Dst_offset];
                            DrawPixel((i + x), (j + y), SrcPix[Src_offset]);
                        }
                    }
                }
            }
        }
    }

    if(was_locked)
    {
        SDL_UnlockSurface(POINTER);
    }

    if(main_was_locked)
    {
        SDL_UnlockSurface(Main_Screen);
    }

    Push_Update_Rect(x, y, POINTER->w, POINTER->h);
}
#endif

// ------------------------------------------------------
// See if a rect have to be scheduled or not
int Check_Rect(int x, int y, int width, int height)
{
    int i;

    for(i = 0; i < Nbr_Update_Rects; i++)
    {
        if(x >= Update_Stack[i].x &&
           y >= Update_Stack[i].y &&
           (width + x) <= (Update_Stack[i].x + Update_Stack[i].w) &&
           (height + y) <= (Update_Stack[i].y + Update_Stack[i].h))
        {
            return(FALSE);
        }
    }
    return(TRUE);
}

// ------------------------------------------------------
// Schedule a rectangle to be refreshed
void Push_Update_Rect(int x, int y, int width, int height)
{
    if(width < 0)
    {
        width = -width;
        x -= width;
    }
    if(height < 0)
    {
        height = -height;
        y -= height;
    }
    if(Nbr_Update_Rects < (UPDATE_STACK_SIZE - 1))
    {
        if(Check_Rect(x, y, width + 1, height + 1))
        {
            if(x < 0) x = 0;
            if(y < 0) y = 0;
            if((x + width) > Cur_Width - 1) width = (Cur_Width - 1) - x;
            if((y + height) > Cur_Height - 1) height = (Cur_Height - 1) - y;
            Update_Stack[Nbr_Update_Rects].x = x;
            Update_Stack[Nbr_Update_Rects].y = y;
            Update_Stack[Nbr_Update_Rects].w = width + 1;
            Update_Stack[Nbr_Update_Rects].h = height + 1;
            Nbr_Update_Rects++;
        }
    }
}
