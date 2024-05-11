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
int FgColor;
#if defined(__USE_OPENGL__)
unsigned int *RGBTexture;
SDL_Color GLPalette[256];
extern GLuint FONT_GL;
extern GLuint FONT_LOW_GL;
#endif

int Nbr_Update_Rects;
SDL_Rect Update_Stack[UPDATE_STACK_SIZE];

// ------------------------------------------------------
// Functions
int Get_Char_Position(char *Ascii_Letters, int Max_Letters, char Letter);

#if defined(__USE_OPENGL__)
// ------------------------------------------------------
// Set 2d ortho mode
void Enter_2D_Mode(float Width, float Height)
{
    double _Width = Width;
    double _Height = Height;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, _Width, _Height, 0.0, 0.0, 1.0);
}

// ------------------------------------------------------
// Restore previous matrices mode
void Leave_2d_Mode(void)
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ------------------------------------------------------
// Draw a horizontal line
void Draw_HLine_(int x, int y, int Width, int Color)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].unused / 255.0f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, 1.0f);
            glVertex2f(Width, 1.0f);
            glVertex2f(Width, 0.0f);
        glEnd();
    glPopMatrix();
}

// ------------------------------------------------------
// Draw a vertical line
void Draw_VLine_(int x, int y, int Height, int Color)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].unused / 255.0f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, Height);
            glVertex2f(1.0f, Height);
            glVertex2f(1.0f, 0.0f);
        glEnd();
    glPopMatrix();
}

// ------------------------------------------------------
// Draw a vertical line
void Draw_Pixel_(int x, int y, int Color)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].unused / 255.0f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, 1.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(1.0f, 0.0f);
        glEnd();
    glPopMatrix();
}

// ------------------------------------------------------
// Draw a one colored rectangle
void Draw_Flat_Rectangle(float x, float y,
                         int Width, int Height, int Color)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].unused / 255.0f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, Height);
            glVertex2f(Width, Height);
            glVertex2f(Width, 0.0f);
        glEnd();
    glPopMatrix();
}

// ------------------------------------------------------
// Create a texture
GLuint Create_Texture(SDL_Surface *Source)
{
    GLuint txId = 0;
    unsigned char *SrcPic;
    int was_locked = FALSE;
    int i;
    int j;
    int index;

    if(SDL_MUSTLOCK(Source))
    {
        if(!SDL_LockSurface(Source)) was_locked = TRUE;
    }

    memset(RGBTexture, 0, TEXTURES_SIZE * TEXTURES_SIZE * sizeof(unsigned int));
    SrcPic = (unsigned char *) Source->pixels;
    for(j = 0; j < Source->h; j++)
    {
        for(i = 0; i < Source->w; i++)
        {
            index = SrcPic[(j * Source->pitch) + i];
#if defined(__BIG_ENDIAN__)
            RGBTexture[(j * TEXTURES_SIZE) + i] = (GLPalette[index].r << 24) | (GLPalette[index].g << 16) | (GLPalette[index].b << 8);
            if(index)
            {
                RGBTexture[(j * TEXTURES_SIZE) + i] |= 0xff;
            }
#else
            RGBTexture[(j * TEXTURES_SIZE) + i] = (GLPalette[index].r) | (GLPalette[index].g << 8) | (GLPalette[index].b << 16);
            if(index)
            {
                RGBTexture[(j * TEXTURES_SIZE) + i] |= 0xff000000;
            }
#endif
        }
    }
    glGenTextures(1, &txId);
    if(txId)
    {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURES_SIZE, TEXTURES_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, RGBTexture);
    }
    if(was_locked)
    {
        SDL_UnlockSurface(Source);
    }
    return(txId);
}

// ------------------------------------------------------
// Delete a previously created texture
void Destroy_Texture(GLuint *txId)
{
    glDeleteTextures(1, txId);
    glFlush();
}

// ------------------------------------------------------
// Draw bitmap
void Draw_Tx_Quad(float x, float y, float x1, float y1, float Width, float Height, GLuint TexID, int Blend)
{
    if(TexID != -1)
    {
        glPushMatrix();
            glTranslatef(x, y, 0.0f);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            if(Blend)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            glBindTexture(GL_TEXTURE_2D, TexID);
            glEnable(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glBegin(GL_QUADS);
                glTexCoord2f((x1 / (float) TEXTURES_SIZE), (y1 / (float) TEXTURES_SIZE)); glVertex2f(0.0f, 0.0f);
                glTexCoord2f((x1 / (float) TEXTURES_SIZE), (Height / (float) TEXTURES_SIZE) + (y1 / (float) TEXTURES_SIZE)); glVertex2f(0.0f, Height);
                glTexCoord2f((Width / (float) TEXTURES_SIZE) + (x1 / (float) TEXTURES_SIZE), (Height / (float) TEXTURES_SIZE) + (y1 / (float) TEXTURES_SIZE)); glVertex2f(Width, Height);
                glTexCoord2f((Width / (float) TEXTURES_SIZE) + (x1 / (float) TEXTURES_SIZE), y1 / (float) TEXTURES_SIZE); glVertex2f(Width, 0.0f);
            glEnd();
            if(Blend)
            {
                glDisable(GL_BLEND);
            }
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}
#endif

// ------------------------------------------------------
// Draw a pixel
void DrawPixel(int x, int y, int Color)
{

#if defined(__USE_OPENGL__)
    Draw_Pixel_(x, y, Color);
#else
    Draw_Pixel(Main_Screen, x, y, Color);
#endif

}

// ------------------------------------------------------
// Draw an horizontal line
void DrawHLine(int y, int x1, int x2, int Color)
{

#if defined(__USE_OPENGL__)
    Draw_HLine_(x1, y, (x2 - x1) + 1, Color);
#else
    Draw_HLine(Main_Screen, x1, y, x2, Color);
    Push_Update_Rect(x1, y, x2 - x1, y + 1);
#endif

}

// ------------------------------------------------------
// Draw a vertical line
void DrawVLine(int x, int y1, int y2, int Color)
{

#if defined(__USE_OPENGL__)
    Draw_VLine_(x, y1, (y2 - y1) + 1, Color);
#else
    Draw_VLine(Main_Screen, x, y1, y2, Color);
    Push_Update_Rect(x, y1, x + 1, y2 - y1);
#endif

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

#if defined(__USE_OPENGL__)
    Draw_Flat_Rectangle(x1, y1, x2 - x1, y2 - y1, FgColor);
#else
    SDL_Rect Dst_Rect;
    Dst_Rect.x = x1;
    Dst_Rect.y = y1;
    Dst_Rect.w = x2 - x1;
    Dst_Rect.h = y2 - y1;
    SDL_FillRect(Main_Screen, &Dst_Rect, FgColor);
    Push_Update_Rect(x1, y1, x2 - x1, y2 - y1);
#endif

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

#if !defined(__USE_OPENGL__)
    SDL_SetPalette(Main_Screen, SDL_PHYSPAL, Palette, 0, Amount);
    SDL_SetPalette(Main_Screen, SDL_LOGPAL, Palette, 0, Amount);
#else
    int i;

    for(i = 0; i < Amount; i++)
    {
        GLPalette[i].r = Palette[i].r;
        GLPalette[i].g = Palette[i].g;
        GLPalette[i].b = Palette[i].b;
    }
#endif

}

// ------------------------------------------------------
// Copy a surface onto the main screen
#if defined(__USE_OPENGL__)
void Copy(GLuint Source,
#else
void Copy(SDL_Surface *Source,
#endif
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

#if defined(__USE_OPENGL__)
    Draw_Tx_Quad(x, y,
                 x1, y1,
                 Dst_Rect.w,
                 Dst_Rect.h,
                 Source, FALSE);
#else
    SDL_BlitSurface(Source, &Src_Rect, Main_Screen, &Dst_Rect);
    Push_Update_Rect(x, y, Dst_Rect.w, Dst_Rect.h);
#endif

}

// ------------------------------------------------------
// Copy a surface onto the main screen without recording the rect
#if defined(__USE_OPENGL__)
void Copy_No_Refresh(GLuint Source,
#else
void Copy_No_Refresh(SDL_Surface *Source,
#endif
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

#if defined(__USE_OPENGL__)
    Draw_Tx_Quad(x, y,
                 x1, y1,
                 Dst_Rect.w, 
                 Dst_Rect.h,
                 Source, FALSE);
#else
    SDL_BlitSurface(Source, &Src_Rect, Main_Screen, &Dst_Rect);
#endif
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

#if !defined(__USE_OPENGL__)
        if(Font_Type == USE_FONT)
        {
            SDL_BlitSurface(FONT, &Src_Rect, Main_Screen, &Dst_Rect);
        }
        else
        {
            SDL_BlitSurface(FONT_LOW, &Src_Rect, Main_Screen, &Dst_Rect);
        }
#else
        if(Font_Type == USE_FONT)
        {
            Draw_Tx_Quad(Dst_Rect.x, Dst_Rect.y,
                         Src_Rect.x, Src_Rect.y,
                         Dst_Rect.w, Dst_Rect.h,
                         FONT_GL, TRUE);
        }
        else
        {
            Draw_Tx_Quad(Dst_Rect.x, Dst_Rect.y,
                         Src_Rect.x, Src_Rect.y,
                         Dst_Rect.w, Dst_Rect.h,
                         FONT_LOW_GL, TRUE);
        }

#endif

        x += Font_Size[Idx];
        if(early_exit) break;
    }
}

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
