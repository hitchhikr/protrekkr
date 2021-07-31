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
#include "include/midi_cfg.h"
#include "../midi/include/midi_dispatch.h"

#if !defined(__WINAMP__)
// ------------------------------------------------------
// Load the data from a midi config file
void Load_Midi_Cfg_Data(int (*Read_Function)(void *, int ,int, FILE *),
                        int (*Read_Function_Swap)(void *, int ,int, FILE *),
                        FILE *in)
{
    int i;

    for(i = 0; i < NBR_MIDI_DISPATCH_MSG; i++)
    {
        Read_Function_Swap(&Midi_Dispatch_Table[i].CC, sizeof(int), 1, in);
        Read_Function_Swap(&Midi_Dispatch_Table[i].Automation, sizeof(int), 1, in);
    }
}

// ------------------------------------------------------
// Save the data to a midi config file
void Save_Midi_Cfg_Data(int (*Write_Function)(void *, int ,int, FILE *),
                        int (*Write_Function_Swap)(void *, int ,int, FILE *),
                        FILE *out)
{
    int i;

    for(i = 0; i < NBR_MIDI_DISPATCH_MSG; i++)
    {
        Write_Function_Swap(&Midi_Dispatch_Table[i].CC, sizeof(int), 1, out);
        Write_Function_Swap(&Midi_Dispatch_Table[i].Automation, sizeof(int), 1, out);
    }
}

// ------------------------------------------------------
// Load a reverb file
void Load_Midi_Cfg(char *FileName)
{
    FILE *in;
    in = fopen(FileName, "rb");

    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];
        fread(extension, sizeof(char), 9, in);

        if(strcmp(extension, "PROTMID1") == 0)
        {
            // Ok, extension matched!
            Status_Box("Loading midi config data...");

            Read_Data(Midi_Name, sizeof(char), 20, in);
            Load_Midi_Cfg_Data(Read_Data, Read_Data_Swap, in);
            Actualize_Midi_Ed(0);

            Status_Box("Midi config data loaded ok.");
        }
        else
        {
            Status_Box("That file is not a " TITLE " midi config file...");
        }
        fclose(in);
    }
    else
    {
        Status_Box("Midi config data loading failed. (Possible cause: file not found)");
    }
}

// ------------------------------------------------------
// Save a midi config file
void Save_Midi_Cfg(void)
{
    FILE *in;
    char Temph[96];
    char extension[10];

    sprintf(extension, "PROTMID1");
    sprintf(Temph, "Saving '%s.pmi' data in midi configs directory...", Midi_Name);
    Status_Box(Temph);
    sprintf(Temph, "%s" SLASH "%s.pmi", Dir_MidiCfg, Midi_Name);

    in = fopen(Temph, "wb");
    if(in != NULL)
    {
        Write_Data(extension, sizeof(char), 9, in);
        Write_Data(Midi_Name, sizeof(char), 20, in);

        Save_Midi_Cfg_Data(Write_Data, Write_Data_Swap, in);

        fclose(in);
        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Status_Box("Midi config data saved succesfully.");   
    }
    else
    {
        Status_Box("Midi config data save failed.");
    }

    Clear_Input();
}
#endif
