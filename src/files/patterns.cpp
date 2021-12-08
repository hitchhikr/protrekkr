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
#include "include/patterns.h"

// ------------------------------------------------------
// Load the data from a pattern file
void Load_Pattern_Data(int (*Read_Function)(void *, int ,int, FILE *),
                      int (*Read_Function_Swap)(void *, int ,int, FILE *),
                      FILE *in,
                      int version)
{
    int Cur_Position = Get_Song_Position();

    int Old_Curr_Buff_Block = Curr_Buff_Block;
    Curr_Buff_Block = NBR_COPY_BLOCKS - 1;

    Read_Function(Buff_MultiNotes[Curr_Buff_Block], sizeof(char), MAX_TRACKS, in);
    if(version == 2)
    {
        Read_Function(Buff_Effects[Curr_Buff_Block], sizeof(char), MAX_TRACKS, in);
    }
    Read_Function_Swap(&b_buff_xsize[Curr_Buff_Block], sizeof(int), 1, in);
    Read_Function_Swap(&b_buff_ysize[Curr_Buff_Block], sizeof(int), 1, in);
    Read_Function_Swap(&start_buff_nibble[Curr_Buff_Block], sizeof(int), 1, in);

    int Size_In;
    int Size_Out = PATTERN_LEN;
    Read_Function_Swap(&Size_In, sizeof(int), 1, in);

    unsigned char *Pack_Mem = (unsigned char *) malloc(Size_In);
    if(Pack_Mem)
    {
        Read_Function(Pack_Mem, sizeof(char), Size_In, in);
        unsigned char *Final_Mem_Out = Depack_Data(Pack_Mem, Size_In, Size_Out);
        if(Final_Mem_Out)
        {
            Buff_Full[Curr_Buff_Block] = TRUE;
            memcpy(BuffBlock[Curr_Buff_Block], Final_Mem_Out, Size_Out);
            Paste_Block(Cur_Position, Paste_Across, FALSE);
            free(Final_Mem_Out);
        }
        free(Pack_Mem);
    }
    Curr_Buff_Block = Old_Curr_Buff_Block;
    Actupated(0);
}

// ------------------------------------------------------
// Save the data to a pattern file
void Save_Pattern_Data(int (*Write_Function)(void *, int ,int, FILE *),
                      int (*Write_Function_Swap)(void *, int ,int, FILE *),
                      FILE *in)
{
    int Cur_Position = Get_Song_Position();

    int Old_Curr_Buff_Block = Curr_Buff_Block;

    Copy_Buff(NBR_COPY_BLOCKS - 1, Curr_Buff_Block);
    Curr_Buff_Block = NBR_COPY_BLOCKS - 1;
    Copy_Selection_To_Buffer(Cur_Position);

    Calc_selection();

    Write_Function(Buff_MultiNotes[Curr_Buff_Block], sizeof(char), MAX_TRACKS, in);
    Write_Function(Buff_Effects[Curr_Buff_Block], sizeof(char), MAX_TRACKS, in);
    Write_Function_Swap(&b_buff_xsize[Curr_Buff_Block], sizeof(int), 1, in);
    Write_Function_Swap(&b_buff_ysize[Curr_Buff_Block], sizeof(int), 1, in);
    Write_Function_Swap(&start_buff_nibble[Curr_Buff_Block], sizeof(int), 1, in);
    int Block_Len = PATTERN_LEN;

    unsigned char *Final_Mem_Out = Pack_Data(BuffBlock[Curr_Buff_Block], &Block_Len);
    if(Final_Mem_Out)
    {
        Write_Function_Swap(&Block_Len, sizeof(int), 1, in);
        Write_Function(Final_Mem_Out, sizeof(char), Block_Len, in);
        free(Final_Mem_Out);
    }
    Curr_Buff_Block = Old_Curr_Buff_Block;
    Actupated(0);
}

// ------------------------------------------------------
// Load a pattern file
void Load_Pattern(char *FileName)
{
    FILE *in;
    int version = 0;

    if(!is_editing)
    {
        Status_Box("Edit mode isn't turned on.");
        return;
    }

    in = fopen(FileName, "rb");

    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];
        fread(extension, sizeof(char), 9, in);

        if(strcmp(extension, "TWNNBLK1") == 0) version = 1;
        if(strcmp(extension, "PROTBLK2") == 0) version = 2;
        if(version)
        {
            // Ok, extension matched!
            Status_Box("Loading Pattern data...");

            Read_Data(Selection_Name, sizeof(char), 20, in);
            Load_Pattern_Data(Read_Data, Read_Data_Swap, in, version);
            Actupated(0);

            Status_Box("Pattern data loaded ok.");
        }
        else
        {
            Status_Box("That file is not a " TITLE " Pattern file...");
        }
        fclose(in);
    }
    else
    {
        Status_Box("Pattern data loading failed. (Possible cause: file not found)");
    }
}

// ------------------------------------------------------
// Save a pattern block file
void Save_Pattern(void)
{
    FILE *in;
    char Temph[96];
    char extension[10];

    sprintf(extension, "PROTBLK2");
    sprintf(Temph, "Saving '%s.ppb' data in patterns directory...", Selection_Name);
    Status_Box(Temph);
    sprintf(Temph, "%s" SLASH "%s.ppb", Dir_Patterns, Selection_Name);

    in = fopen(Temph, "wb");
    if(in != NULL)
    {
        Write_Data(extension, sizeof(char), 9, in);
        Write_Data(Selection_Name, sizeof(char), 20, in);

        Save_Pattern_Data(Write_Data, Write_Data_Swap, in);

        fclose(in);
        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Status_Box("Pattern data saved succesfully.");   
    }
    else
    {
        Status_Box("Pattern data save failed.");
    }

    Clear_Input();
}
