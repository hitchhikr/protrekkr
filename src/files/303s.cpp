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
#include "include/303s.h"

// ------------------------------------------------------
// Read a 303 pattern data
void Load_303_Data(int (*Read_Function)(void *, int ,int, FILE *),
                   int (*Read_Function_Swap)(void *, int ,int, FILE *),
                   FILE *in, int unit, int pattern)
{
    int i;

    Read_Function(&tb303[unit].patternlength[pattern], sizeof(char), 1, in);
    Read_Function(&tb303[unit].tone[pattern], sizeof(char), 16, in);
    for(i = 0; i < 16; i++)
    {
        Read_Function_Swap(&tb303[unit].flag[pattern][i], sizeof(struct flag303), 1, in);
    }
    Read_Function(&tb303[unit].pattern_name[pattern], sizeof(char), 20, in);
}

// ------------------------------------------------------
// Write a 303 pattern data
void Save_303_Data(int (*Write_Function)(void *, int ,int, FILE *),
                   int (*Write_Function_Swap)(void *, int ,int, FILE *),
                   FILE *in, int unit, int pattern)
{
    int i;

    Write_Function(&tb303[unit].patternlength[pattern], sizeof(char), 1, in);
    Write_Function(&tb303[unit].tone[pattern], sizeof(char), 16, in);
    for(i = 0; i < 16; i++)
    {
        Write_Function_Swap(&tb303[unit].flag[pattern][i], sizeof(struct flag303), 1, in);
    }
    Write_Function(&tb303[unit].pattern_name[pattern], sizeof(char), 20, in);
}

// ------------------------------------------------------
// Load a 303 pattern
#if !defined(__WINAMP__)
void Load_303(char *FileName)
{
    FILE *in;
    in = fopen(FileName, "rb");

    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];
        fread(extension, sizeof(char), 9, in);

        if(strcmp(extension, "TWNN3031") == 0)
        {
            // Ok, extension matched!
            Status_Box("Loading 303 pattern...");

            Load_303_Data(Read_Data, Read_Data_Swap, in, sl3, tb303[sl3].selectedpattern);

            Actualize_303_Ed(0);
            Status_Box("303 pattern loaded ok.");
        }
        else
        {
            Status_Box("That file is not a " TITLE " 303 pattern file...");
        }
        fclose(in);
    }
    else
    {
        Status_Box("303 pattern loading failed. (Possible cause: file not found)");
    }
}

// ------------------------------------------------------
// Save a 303 pattern
void Save_303(void)
{
    FILE *in;
    char Temph[MAX_PATH];
    char extension[10];

    sprintf(extension, "TWNN3031");
    sprintf(Temph, "Saving '%s.303' pattern in patterns directory...",
            tb303[sl3].pattern_name[tb303[sl3].selectedpattern]);
    Status_Box(Temph);
    sprintf(Temph, "%s" SLASH "%s.303", Dir_Patterns,
            tb303[sl3].pattern_name[tb303[sl3].selectedpattern]);
    in = fopen(Temph, "wb");

    if(in != NULL)
    {
        Write_Data(extension, sizeof(char), 9, in);

        Save_303_Data(Write_Data, Write_Data_Swap, in, sl3, tb303[sl3].selectedpattern);

        fclose(in);
        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Status_Box("303 pattern saved succesfully.");   
    }
    else
    {
        Status_Box("303 pattern save failed.");
    }

    Clear_Input();
}
#endif
