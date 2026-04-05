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
extern PTK_SURFACE *Main_Screen;
extern PTK_SURFACE *Temp_PFONT;
extern PTK_SURFACE *Temp_LARGEPFONT;
extern PTK_SURFACE *Temp_SMALLPFONT;
extern PTK_SURFACE *Temp_NOTEPFONT;
extern PTK_SURFACE *Temp_NOTELARGEPFONT;
extern PTK_SURFACE *Temp_NOTESMALLPFONT;
// ---
extern PTK_SURFACE *Temp_PFONT_DOUBLE;
extern PTK_SURFACE *Temp_LARGEPFONT_DOUBLE;
extern PTK_SURFACE *Temp_SMALLPFONT_DOUBLE;
extern PTK_SURFACE *Temp_NOTEPFONT_DOUBLE;
extern PTK_SURFACE *Temp_NOTELARGEPFONT_DOUBLE;
extern PTK_SURFACE *Temp_NOTESMALLPFONT_DOUBLE;
// ---
extern char *Font_Ascii;
extern int Nbr_Letters;
extern int Font_Pos[256];
extern int Font_Size[256];
extern int pattern_double;
int FgColor;
int BkColor;
unsigned int RGBTexture[TEXTURES_SIZE * TEXTURES_SIZE * sizeof(unsigned int)];
PTK_PALETTE *Current_Palette;
PTK_COLOR GLPalette[256];
extern PTK_TEXTURE FONT_GL[WORK_FONTS_NBR];
extern PTK_TEXTURE FONT_LOW_GL[WORK_FONTS_NBR];

// ------------------------------------------------------
// Functions
int Get_Char_Position(char *Ascii_Letters, int Max_Letters, char Letter);

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// OpenGL stuff

struct Vertex
{
    GLfloat x, y;
};
struct Color
{
    GLfloat r, g, b, a;
};

// This may be changed in the future
#define TEXTURES_MAX 37
// Pretty big but who knows...
#define VERTICES_MAX 50000
#define TX_VERTICES_MAX 20000
// One for background and one for requesters
#define DRAW_PRIORITIES 2

Vertex *Vertices;
Color *Colors;
int Num_Vertices[DRAW_PRIORITIES];
int Max_Vertices[DRAW_PRIORITIES];

Vertex *Vertices_Tx[TEXTURES_MAX];
Vertex *Coords_Tx[TEXTURES_MAX];
int Num_Tx[DRAW_PRIORITIES][TEXTURES_MAX];
int Max_Tx[DRAW_PRIORITIES][TEXTURES_MAX];
int Num_Textures;
int Ids_Tx[TEXTURES_MAX];

int Drawing_Priority = 0;

// Background indexes for the different fonts
int text_bk_color_offsets[] =
{
    0,   // (phony)
    0,   // COL_BACKGROUND 1
    0,   // COL_STATIC_HI 2
    1,   // COL_STATIC_MED 3
    0,   // COL_STATIC_LO 4
    0,   // COL_HI 5
    2,   // COL_MED 6
    0,   // COL_LO 7
    4,   // COL_PUSHED_HI 8
    3,   // COL_PUSHED_MED 9
    0,   // COL_PUSHED_LO 10
    0,   // COL_SLIDER_HI 11
    10,  // COL_SLIDER_MED 12
    0,   // COL_SLIDER_LO 13
    0,   // COL_VUMETER 14
    0,   // COL_VUMETERPEAK 15
    0,   // COL_SCOPESSAMPLES 16
    0,   // COL_FONT_HI 17
    0,   // COL_FONT_LO 18
    0,   // COL_PATTERN_LO_BACK 19
    0,   // COL_PATTERN_LO_FORE 20
    0,   // COL_PATTERN_HI_BACK 21
    0,   // COL_PATTERN_HI_FORE 22
    0,   // COL_PATTERN_SEL_BACK 23
    0,   // COL_PATTERN_SEL_FORE 24
    0,   // (calculated) 25
    0,   // COL_NOTE_LO_FORE 26
    0,   // (calculated) 27
    0,   // COL_NOTE_HI_FORE 28
    0,   // (calculated) 29
    0,   // COL_NOTE_SEL_FORE 30
    0,   // COL_MUTE 31
    0,   // COL_PLAY 32
    0,   // COL_MUTE_PLAY_INVERT 33
    0,   // COL_PATTERN_LO_BACK_SHADOW 34
    0,   // COL_PATTERN_LO_FORE_SHADOW 35
    0,   // COL_PATTERN_HI_BACK_SHADOW 36
    0,   // COL_PATTERN_HI_FORE_SHADOW 37
    0,   // COL_PATTERN_SEL_BACK_SHADOW 38
    0,   // COL_PATTERN_SEL_FORE_SHADOW 29
    0,   // COL_NOTE_LO_BACK_SHADOW 40
    0,   // COL_NOTE_LO_FORE_SHADOW 41
    0,   // COL_NOTE_HI_BACK_SHADOW 42
    0,   // COL_NOTE_HI_FORE_SHADOW 43
    0,   // COL_NOTE_SEL_BACK_SHADOW 44
    0,   // COL_NOTE_SEL_FORE_SHADOW 45
    0,   // COL_DOUBLE_HI 46
    5,   // COL_DOUBLE_MED 47
    0,   // COL_DOUBLE_LO 48
    0,   // COL_DOUBLE_PUSHED_HI 49
    6,   // COL_DOUBLE_PUSHED_MED 50
    0,   // COL_DOUBLE_PUSHED_LO 51
    0,   // COL_INPUT_HI 52
    7,   // COL_INPUT_MED 53
    0,   // COL_INPUT_LO 54
    0,   // COL_INPUT_PUSHED_HI 55
    8,   // COL_INPUT_PUSHED_MED 56
    0,   // COL_INPUT_PUSHED_LO 57
    9    // COL_BLACK 58
};

// Corresponding to Font_Ascii in misc_draw.cpp
char *Font_Ascii_Small = 
    "00000000000000000000000000"
    "00000000000000000000000000"
    //           é        è    ç   à         ù    µ                  /\  \/  <   >   <<  >>
    "000000000000000000000000000000000000000000000000000000000"
    // ---- Small letters
    "11111111111111111111111111"
    //0   1   2   3   4   5   6   7   8   9
    "1111111111"
    ///   (   )   :
    "1111"
    //.   -
    "11"
    // ---- Small letters
    "000000011"
    "1";

// ------------------------------------------------------
// Init OpenGL working stuff
int Init_Open_GL_Buffers()
{
    int i;
    int j;

    Vertices = (Vertex *) calloc(VERTICES_MAX, sizeof(Vertex));
    if (!Vertices)
    {
        return FALSE;
    }
    Colors = (Color *) calloc(VERTICES_MAX, sizeof(Color));
    if (!Colors)
    {
        return FALSE;
    }
    for (i = 0; i < DRAW_PRIORITIES; i++)
    {
        Max_Vertices[i] = VERTICES_MAX;
    }
    for (i = 0; i < TEXTURES_MAX; i++)
    {
        Vertices_Tx[i] = (Vertex *) calloc(TX_VERTICES_MAX, sizeof(Vertex));
        if (!Vertices_Tx[i])
        {
            return FALSE;
        }
        Coords_Tx[i] = (Vertex *) calloc(TX_VERTICES_MAX, sizeof(Vertex));
        if (!Coords_Tx[i])
        {
            return FALSE;
        }
        for (j = 0; j < DRAW_PRIORITIES; j++)
        {
            Max_Tx[j][i] = TX_VERTICES_MAX;
        }
    }
    return TRUE;
}

// ------------------------------------------------------
// Release allocated OpenGL working stuff
void Release_Open_GL_Buffers()
{
    int i;

    for (i = 0; i < TEXTURES_MAX; i++)
    {
        if(Vertices_Tx[i])
        {
            free(Vertices_Tx[i]);
            Vertices_Tx[i] = NULL;
        }
        if (Coords_Tx[i])
        {
            free(Coords_Tx[i]);
            Coords_Tx[i] = NULL;
        }
    }
    if (Vertices)
    {
        free(Vertices);
        Vertices = NULL;
    }
    if (Colors)
    {
        free(Colors);
        Colors = NULL;
    }
}

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
    memset(Num_Vertices, 0, sizeof(Num_Vertices));
    memset(Num_Tx, 0, sizeof(Num_Tx));
}

void *ptk_realloc(void *buffer, size_t old_size, size_t size)
{
    void *Tmp_Buffer;
 
    Tmp_Buffer = malloc(size);
    if(!Tmp_Buffer)
    {
        return NULL;
    }
    memset(Tmp_Buffer, 0, size);
    memcpy(Tmp_Buffer, buffer, old_size);
    free(buffer);
    return Tmp_Buffer;
}

// Failsafe checks
int check_vertices_buffer()
{
    if((Num_Vertices[Drawing_Priority] + 4) > (Max_Vertices[Drawing_Priority] - 1))
    {
        Vertices = (Vertex *) realloc(Vertices, (Num_Vertices[Drawing_Priority] + 10000) * sizeof(Vertex));
        if(!Vertices)
        {
            return FALSE;
        }
        Colors = (Color *) realloc(Colors, (Num_Vertices[Drawing_Priority] + 10000) * sizeof(Color));
        if(!Colors)
        {
            return FALSE;
        }
        Max_Vertices[Drawing_Priority] = Num_Vertices[Drawing_Priority] + 10000;
    }
    return TRUE;
}

int check_vertices_tx_buffer(int index)
{
    if((Num_Tx[Drawing_Priority][index] + 4) > (Max_Tx[Drawing_Priority][index] - 1))
    {
        Vertices_Tx[index] = (Vertex *) realloc(Vertices_Tx[index], (Num_Tx[Drawing_Priority][index] + 10000) * sizeof(Vertex));
        if(!Vertices_Tx[index])
        {
            return FALSE;
        }
        Coords_Tx[index] = (Vertex *) realloc(Coords_Tx[index], (Num_Tx[Drawing_Priority][index] + 10000) * sizeof(Vertex));
        if(!Coords_Tx[index])
        {
            return FALSE;
        }
        Max_Tx[Drawing_Priority][index] = Num_Tx[Drawing_Priority][index] + 10000;
    }
    return TRUE;
}

// ------------------------------------------------------
// Restore previous matrices mode
void Leave_2d_Mode(void)
{
    int i;
    int j;

    for(j = 0; j < DRAW_PRIORITIES; j++)
    {
        if(Num_Vertices[j])
        {
            // Draw the flat elements
            glEnableClientState(GL_COLOR_ARRAY);
            glVertexPointer(2, GL_FLOAT, sizeof(Vertex), Vertices);
	        glColorPointer(4, GL_FLOAT, sizeof(Color), Colors);
            glDrawArrays(GL_QUADS, 0, Num_Vertices[j]);
            glDisableClientState(GL_COLOR_ARRAY);
        }

        // Now draw the non alpha blended textured ones
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // For each different texture
        for(i = 0; i < Num_Textures; i++)
        {
            // Anything to draw for this one ?
            if(Ids_Tx[i] != -1)
            {
                if(Num_Tx[j][i])
                {
                    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), Vertices_Tx[i]);
                    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), Coords_Tx[i]);
                    glBindTexture(GL_TEXTURE_2D, Ids_Tx[i]);
                    glDrawArrays(GL_QUADS, 0, Num_Tx[j][i]);
                }
            }
        }
	    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ------------------------------------------------------
// Draw a horizontal line
void DrawHLine(int y, int x, int x2, int Color)
{
    GLfloat r = (GLfloat) Ptk_Palette[Color].r / 255.0f;
    GLfloat g = (GLfloat) Ptk_Palette[Color].g / 255.0f;
    GLfloat b = (GLfloat) Ptk_Palette[Color].b / 255.0f;
    GLfloat a = (GLfloat) Ptk_Palette[Color].a / 255.0f;
    GLfloat Width = (GLfloat) ((x2 - x) + 1);

    if(!check_vertices_buffer())
    {
        return;
    }
    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = 1.0f + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) Width + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = 1.0f + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) Width + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;
}

// ------------------------------------------------------
// Draw a vertical line
void DrawVLine(int x, int y, int y2, int Color)
{
    GLfloat r = (GLfloat) Ptk_Palette[Color].r / 255.0f;
    GLfloat g = (GLfloat) Ptk_Palette[Color].g / 255.0f;
    GLfloat b = (GLfloat) Ptk_Palette[Color].b / 255.0f;
    GLfloat a = (GLfloat) Ptk_Palette[Color].a / 255.0f;
    GLfloat Height = (GLfloat) ((y2 - y) + 1);

    if(!check_vertices_buffer())
    {
        return;
    }
    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) Height + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = 1.0f + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) Height + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = 1.0f + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;
}

// ------------------------------------------------------
// Draw a vertical line
void DrawPixel(int x, int y, int Color)
{
    GLfloat r = (GLfloat) Ptk_Palette[Color].r / 255.0f;
    GLfloat g = (GLfloat) Ptk_Palette[Color].g / 255.0f;
    GLfloat b = (GLfloat) Ptk_Palette[Color].b / 255.0f;
    GLfloat a = (GLfloat) Ptk_Palette[Color].a / 255.0f;

    if(!check_vertices_buffer())
    {
        return;
    }
    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = 1.0f + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = 1.0f + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = 1.0f + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = 1.0f + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;
}

// ------------------------------------------------------
// Draw a one colored rectangle
void Fill_Rect(int x, int y,
               int x2, int y2)
{
    GLfloat r = (GLfloat) Ptk_Palette[FgColor].r / 255.0f;
    GLfloat g = (GLfloat) Ptk_Palette[FgColor].g / 255.0f;
    GLfloat b = (GLfloat) Ptk_Palette[FgColor].b / 255.0f;
    GLfloat a = (GLfloat) Ptk_Palette[FgColor].a / 255.0f;
    GLfloat Width = (GLfloat) (x2 - x);
    GLfloat Height = (GLfloat) (y2 - y); 

    if(!check_vertices_buffer())
    {
        return;
    }
    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) Height + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) Width + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) Height + (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;

    Vertices[Num_Vertices[Drawing_Priority]].x = (GLfloat) Width + (GLfloat) x; Vertices[Num_Vertices[Drawing_Priority]].y = (GLfloat) y;
    Colors[Num_Vertices[Drawing_Priority]].r = r; Colors[Num_Vertices[Drawing_Priority]].g = g; Colors[Num_Vertices[Drawing_Priority]].b = b; Colors[Num_Vertices[Drawing_Priority]].a = a;
    Num_Vertices[Drawing_Priority]++;
}

// ------------------------------------------------------
// Draw a picture
void Draw_Textured_Rectangle(int x, int y, int x1, int y1, int Width, int Height, int index, int Blend)
{
    if(index != -1)
    {
        if(!check_vertices_tx_buffer(index))
        {
            return;
        }
        // Store the vertices and coords for this one texture index
        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].x = (x1 / (float) TEXTURES_SIZE) + (GLfloat) x;
        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].y = (y1 / (float) TEXTURES_SIZE) + (GLfloat) y;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].x = 0.0f + (GLfloat) x;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].y = 0.0f + (GLfloat) y;
        Num_Tx[Drawing_Priority][index]++;

        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].x = (x1 / (float) TEXTURES_SIZE) + (GLfloat) x;
        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].y = (Height / (float) TEXTURES_SIZE) + (y1 / (float) TEXTURES_SIZE) + (GLfloat) y;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].x = 0.0f + (GLfloat) x;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].y = (GLfloat) Height + (GLfloat) y;
        Num_Tx[Drawing_Priority][index]++;

        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].x = (Width / (float) TEXTURES_SIZE) + (x1 / (float) TEXTURES_SIZE) + (GLfloat) x;
        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].y = (Height / (float) TEXTURES_SIZE) + (y1 / (float) TEXTURES_SIZE) + (GLfloat) y;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].x = (GLfloat) Width + (GLfloat) x;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].y = (GLfloat) Height + (GLfloat) y;
        Num_Tx[Drawing_Priority][index]++;

        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].x = (Width / (float) TEXTURES_SIZE) + (x1 / (float) TEXTURES_SIZE) + (GLfloat) x;
        Coords_Tx[index][Num_Tx[Drawing_Priority][index]].y = (y1 / (float) TEXTURES_SIZE) + (GLfloat) y;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].x = (GLfloat) Width + (GLfloat) x;
        Vertices_Tx[index][Num_Tx[Drawing_Priority][index]].y = 0.0f + (GLfloat) y;
        Num_Tx[Drawing_Priority][index]++;
    }
}

// ------------------------------------------------------
// Create an OpenGL texture from a surface
void Create_OGL_Texture(PTK_SURFACE *Source, PTK_TEXTURE *Dest)
{
    GLuint txId = 0;
    unsigned char *SrcPic;
    int was_locked;
    int i;
    int j;
    int index;

    was_locked = Lock_Surface(Source);

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
        Dest->index = Num_Textures;
        Dest->txid = txId;
        Ids_Tx[Num_Textures] = txId;
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURES_SIZE, TEXTURES_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, RGBTexture);
        Num_Textures++;
    }
    if(was_locked)
    {
        Unlock_Surface(Source);
    }
}

// ------------------------------------------------------
// Delete a previously created texture
void Destroy_OGL_Texture(PTK_TEXTURE *texture)
{
    if(texture->txid != -1 && Num_Textures)
    {
        glDeleteTextures(1, &texture->txid);
        Ids_Tx[Num_Textures] = -1;
        Num_Textures--;
    }
}

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

// ------------------------------------------------------
// Load a .bmp picture into a SDL surface
PTK_SURFACE *Load_Picture(char *FileName)
{
    return(SDL_LoadBMP(FileName));
}

// ------------------------------------------------------
//  Create a surface
PTK_SURFACE *Create_Surface(int width, int height)
{
    return SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0xff);
}

// ------------------------------------------------------
// Delete a previously created surface
void Destroy_Surface(PTK_SURFACE *texture)
{
    SDL_FreeSurface(texture);
}

// ------------------------------------------------------
// Lock a surface
int Lock_Surface(PTK_SURFACE *texture)
{
    if(SDL_MUSTLOCK(texture))
    {
        if(!SDL_LockSurface(texture)) return(TRUE);
    }
    return(FALSE);
}

// ------------------------------------------------------
// Unlock a surface
void Unlock_Surface(PTK_SURFACE *texture)
{
    SDL_UnlockSurface(texture);
}

// ------------------------------------------------------
// Set the current foreground color
void Set_Color(int color)
{
    FgColor = color;
}

// ------------------------------------------------------
// Set the current background color
void Set_Bk_Color(int color)
{
    BkColor = color;
}

// ------------------------------------------------------
// Copy a surface onto the main screen without recording the rect
// - Only used for the patterns -
void Copy_No_Refresh(PTK_TEXTURE *Source,
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

    Draw_Textured_Rectangle(dest_x, dest_y,
                 source_start_x, source_start_y,
                 Dst_Rect.w, 
                 Dst_Rect.h,
                 Source->index, FALSE);
}

// ------------------------------------------------------
// Copy a surface onto the main screen
// - Only used for the splash screen and the 303s picture -
void Copy(PTK_TEXTURE *Source,
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

    Draw_Textured_Rectangle(x, y,
                 x1, y1,
                 Dst_Rect.w,
                 Dst_Rect.h,
                 Source->index, FALSE);
}

// ------------------------------------------------------
// Copy a rectangle onto a given surface
// - Only used to create the patterns fonts -
void Copy_To_Surface(PTK_SURFACE *source, PTK_SURFACE *dest,
                     int dst_y, int src_y, int width, int height)
{
    int src_was_locked;
    int dst_was_locked;
    unsigned char *src_pix;
    unsigned char *dst_pix;
    int i;
    int j;

    src_was_locked = Lock_Surface(source);
    dst_was_locked = Lock_Surface(dest);

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
        Unlock_Surface(source);
    }
    if(dst_was_locked)
    {
        Unlock_Surface(dest);
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
    Src_Rect.y = 0;

    start_x = x;
    // current color
    Set_Bk_Color(FgColor);

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
        if(Font_Ascii_Small[Idx] == '0')
        {
            Dst_Rect.h = Font_Height;
            Src_Rect.h = Font_Height;
        }
        else
        {
            Dst_Rect.h = Font_Height_Small;
            Src_Rect.h = Font_Height_Small;
        }

        if(Font_Type == USE_FONT)
        {
            Draw_Textured_Rectangle(Dst_Rect.x, Dst_Rect.y,
                         Src_Rect.x, Src_Rect.y,
                         Dst_Rect.w, Dst_Rect.h,
                         FONT_GL[text_bk_color_offsets[BkColor]].index, TRUE);
        }
        else
        {
            Draw_Textured_Rectangle(Dst_Rect.x, Dst_Rect.y,
                         Src_Rect.x, Src_Rect.y,
                         Dst_Rect.w, Dst_Rect.h,
                         FONT_LOW_GL[text_bk_color_offsets[BkColor]].index, TRUE);
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
void Set_Texture_Palette(PTK_SURFACE *texture)
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
