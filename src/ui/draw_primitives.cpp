// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2026 Franck Charlet.
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
#include "../include/ptk.h"

// ------------------------------------------------------
// Variables
extern PTK_TEXTURE *Main_Screen;
extern PTK_TEXTURE *Temp_PFONT;
extern PTK_TEXTURE *Temp_LARGEPFONT;
extern PTK_TEXTURE *Temp_SMALLPFONT;
extern PTK_TEXTURE *Temp_NOTEPFONT;
extern PTK_TEXTURE *Temp_NOTELARGEPFONT;
extern PTK_TEXTURE *Temp_NOTESMALLPFONT;
// ---
extern PTK_TEXTURE *Temp_PFONT_DOUBLE;
extern PTK_TEXTURE *Temp_LARGEPFONT_DOUBLE;
extern PTK_TEXTURE *Temp_SMALLPFONT_DOUBLE;
extern PTK_TEXTURE *Temp_NOTEPFONT_DOUBLE;
extern PTK_TEXTURE *Temp_NOTELARGEPFONT_DOUBLE;
extern PTK_TEXTURE *Temp_NOTESMALLPFONT_DOUBLE;
// ---
extern char *Font_Ascii;
extern int Nbr_Letters;
extern int Font_Pos[256];
extern int Font_Size[256];
extern int pattern_double;
int FgColor;
unsigned int RGBTexture[TEXTURES_SIZE * TEXTURES_SIZE * sizeof(unsigned int)];
PTK_PALETTE *Current_Palette;
PTK_COLOR GLPalette[256];
extern GLuint FONT_GL;
extern GLuint FONT_LOW_GL;

// ------------------------------------------------------
// Functions
int Get_Char_Position(char *Ascii_Letters, int Max_Letters, char Letter);

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
void Draw_HLine(int x, int y, int Width, int Color)
{
    glPushMatrix();
        glTranslatef((GLfloat) x, (GLfloat) y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].a / 255.0f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, 1.0f);
            glVertex2f((GLfloat) Width, 1.0f);
            glVertex2f((GLfloat) Width, 0.0f);
        glEnd();
    glPopMatrix();
}

// ------------------------------------------------------
// Draw a vertical line
void Draw_VLine(int x, int y, int Height, int Color)
{
    glPushMatrix();
        glTranslatef((GLfloat) x, (GLfloat) y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].a / 255.0f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, (GLfloat) Height);
            glVertex2f(1.0f, (GLfloat) Height);
            glVertex2f(1.0f, 0.0f);
        glEnd();
    glPopMatrix();
}

// ------------------------------------------------------
// Draw a vertical line
void Draw_Pixel(int x, int y, int Color)
{
    glPushMatrix();
        glTranslatef((GLfloat) x, (GLfloat) y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].a / 255.0f);
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
void Draw_Flat_Rectangle(int x, int y,
                         int Width, int Height, int Color)
{
    glPushMatrix();
        glTranslatef((GLfloat) x, (GLfloat) y, 0.0f);
        glColor4f((float) Ptk_Palette[Color].r / 255.0f,
                  (float) Ptk_Palette[Color].g / 255.0f,
                  (float) Ptk_Palette[Color].b / 255.0f,
                  (float) Ptk_Palette[Color].a / 255.0f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, (GLfloat) Height);
            glVertex2f((GLfloat) Width, (GLfloat) Height);
            glVertex2f((GLfloat) Width, 0.0f);
        glEnd();
    glPopMatrix();
}

// ------------------------------------------------------
// Create an OpenGL texture from a surface
GLuint Create_OGL_Texture(PTK_TEXTURE *Source)
{
    GLuint txId = 0;
    unsigned char *SrcPic;
    int was_locked;
    int i;
    int j;
    int index;

    was_locked = Lock_Texture(Source);

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
        Unlock_Texture(Source);
    }
    return(txId);
}

// ------------------------------------------------------
// Delete a previously created texture
void Destroy_OGL_Texture(GLuint *txId)
{
    if(*txId != -1)
    {
        glDeleteTextures(1, txId);
        *txId = -1;
    }
}

// ------------------------------------------------------
// Draw bitmap
void Draw_Tx_Quad(int x, int y, int x1, int y1, int Width, int Height, GLuint TexID, int Blend)
{
    if(TexID != -1)
    {
        glPushMatrix();
            glTranslatef((GLfloat) x, (GLfloat) y, 0.0f);
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
                glTexCoord2f((x1 / (float) TEXTURES_SIZE), (Height / (float) TEXTURES_SIZE) + (y1 / (float) TEXTURES_SIZE)); glVertex2f(0.0f, (GLfloat) Height);
                glTexCoord2f((Width / (float) TEXTURES_SIZE) + (x1 / (float) TEXTURES_SIZE), (Height / (float) TEXTURES_SIZE) + (y1 / (float) TEXTURES_SIZE)); glVertex2f((GLfloat) Width, (GLfloat) Height);
                glTexCoord2f((Width / (float) TEXTURES_SIZE) + (x1 / (float) TEXTURES_SIZE), y1 / (float) TEXTURES_SIZE); glVertex2f((GLfloat) Width, 0.0f);
            glEnd();
            if(Blend)
            {
                glDisable(GL_BLEND);
            }
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

// ------------------------------------------------------
// Load a .bmp picture into a SDL surface
PTK_TEXTURE *Load_Picture(char *FileName)
{
    return(SDL_LoadBMP(FileName));
}

// ------------------------------------------------------
//  Create a texture
PTK_TEXTURE *Create_Texture(int width, int height)
{
    return SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0xff);
}

// ------------------------------------------------------
// Delete a previously created texture
void Destroy_Texture(PTK_TEXTURE *texture)
{
    SDL_FreeSurface(texture);
}

// ------------------------------------------------------
// Lock a texture
int Lock_Texture(PTK_TEXTURE *texture)
{
    if(SDL_MUSTLOCK(texture))
    {
        if(!SDL_LockSurface(texture)) return(TRUE);
    }
    return(FALSE);
}

// ------------------------------------------------------
// Unlock a texture
void Unlock_Texture(PTK_TEXTURE *texture)
{
    SDL_UnlockSurface(texture);
}

// ------------------------------------------------------
// Draw a pixel
void DrawPixel(int x, int y, int Color)
{
    Draw_Pixel(x, y, Color);
}

// ------------------------------------------------------
// Draw an horizontal line
void DrawHLine(int y, int x1, int x2, int Color)
{
    Draw_HLine(x1, y, (x2 - x1) + 1, Color);
}

// ------------------------------------------------------
// Draw a vertical line
void DrawVLine(int x, int y1, int y2, int Color)
{
    Draw_VLine(x, y1, (y2 - y1) + 1, Color);
}

// ------------------------------------------------------
// Set the current color
void Set_Color(int color)
{
    FgColor = color;
}

// ------------------------------------------------------
// Copy a surface onto the main screen without recording the rect
void Copy_No_Refresh(GLuint Source,
          int dest_x, int dest_y,
          int source_start_x, int source_start_y,
          int src_end_x, int src_end_y,
          int remainder)
{
    SDL_Rect Src_Rect;
    SDL_Rect Dst_Rect;

    if(pattern_double)
    {
        source_start_y <<= 1;
        src_end_y <<= 1;
        src_end_y += remainder;
    }

    Dst_Rect.x = dest_x;
    Dst_Rect.y = dest_y;
    Dst_Rect.w = (src_end_x - source_start_x) + 1;
    Dst_Rect.h = (src_end_y - source_start_y) + 1;

    Src_Rect.x = source_start_x;
    Src_Rect.y = source_start_y;
    Src_Rect.w = Dst_Rect.w;
    Src_Rect.h = Dst_Rect.h;

    Draw_Tx_Quad(dest_x, dest_y,
                 source_start_x, source_start_y,
                 Dst_Rect.w, 
                 Dst_Rect.h,
                 Source, FALSE);
}

// ------------------------------------------------------
// Fill a rectangle with the current color
void Fill_Rect(int x1, int y1, int x2, int y2)
{
    Draw_Flat_Rectangle(x1, y1, x2 - x1, y2 - y1, FgColor);
}

// ------------------------------------------------------
// Copy a surface onto the main screen
void Copy(GLuint Source,
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

    Draw_Tx_Quad(x, y,
                 x1, y1,
                 Dst_Rect.w,
                 Dst_Rect.h,
                 Source, FALSE);
}

// ------------------------------------------------------
// Copy a rectangle onto a given surface
// (Only used to create the fonts and destroy the requesters)
void Copy_To_Surface(PTK_TEXTURE *source, PTK_TEXTURE *dest,
                     int dst_y, int src_y, int width, int height)
{
    int src_was_locked;
    int dst_was_locked;
    unsigned char *src_pix;
    unsigned char *dst_pix;
    int i;
    int j;

    src_was_locked = Lock_Texture(source);
    dst_was_locked = Lock_Texture(dest);

    src_pix = (unsigned char *) source->pixels;
    dst_pix = (unsigned char *) dest->pixels;
    src_pix += (src_y * source->pitch);
    dst_pix += (dst_y * dest->pitch);

    for(j = 0; j < height; j++)
    {
        for(i = 0; i < width; i++)
        {
            dst_pix[(j * dest->pitch) + i] = src_pix[(j * source->pitch) + i];
        }
    }

    if(src_was_locked)
    {
        Unlock_Texture(source);
    }
    if(dst_was_locked)
    {
        Unlock_Texture(dest);
    }
}

// ------------------------------------------------------
// Print a string on the screen
void Print_String(int x,
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

        x += Font_Size[Idx];
        if(early_exit) break;
    }
}

// ------------------------------------------------------
// Free the palette
void Free_Palette(void)
{
    SDL_FreePalette(Current_Palette);
    Current_Palette = NULL;
}

// ------------------------------------------------------
// Set the current palette on a surface
void Set_Texture_Palette(PTK_TEXTURE *texture)
{
    if(texture)
    {
        if(Current_Palette)
        {
            SDL_SetSurfacePalette(texture, Current_Palette);
        }
    }
}

// ------------------------------------------------------
// Set the current palette
void UI_Set_Palette(PTK_COLOR *Palette, int Amount)
{
    if(Current_Palette)
    {
        Free_Palette();
        Current_Palette = SDL_AllocPalette(Amount);
        SDL_SetPaletteColors(Current_Palette, Palette, 0, Amount);
    }

    Set_Texture_Palette(FONT_LOW);
    Set_Texture_Palette(FONT);

    // ---
    Set_Texture_Palette(Temp_PFONT);
    Set_Texture_Palette(Temp_LARGEPFONT);
    Set_Texture_Palette(Temp_SMALLPFONT);
    Set_Texture_Palette(Temp_NOTEPFONT);
    Set_Texture_Palette(Temp_NOTELARGEPFONT);
    Set_Texture_Palette(Temp_NOTESMALLPFONT);

    // ---
    Set_Texture_Palette(Temp_PFONT_DOUBLE);
    Set_Texture_Palette(Temp_LARGEPFONT_DOUBLE);
    Set_Texture_Palette(Temp_SMALLPFONT_DOUBLE);
    Set_Texture_Palette(Temp_NOTEPFONT_DOUBLE);
    Set_Texture_Palette(Temp_NOTELARGEPFONT_DOUBLE);
    Set_Texture_Palette(Temp_NOTESMALLPFONT_DOUBLE);
    // ---

    int i;

    for(i = 0; i < Amount; i++)
    {
        GLPalette[i].r = Palette[i].r;
        GLPalette[i].g = Palette[i].g;
        GLPalette[i].b = Palette[i].b;
        GLPalette[i].a = Palette[i].a;
    }

}
