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
#include "include/reverbs.h"

// ------------------------------------------------------
// Load the data from a reverb file (or a module)
void Load_Reverb_Data(int (*Read_Function)(void *, int ,int, FILE *),
                      int (*Read_Function_Swap)(void *, int ,int, FILE *),
                      FILE *in, int New)
{
    int i;

    Read_Function(&num_echoes, sizeof(char), 1, in);

    for(i = 0; i < MAX_COMB_FILTERS; i++)
    {
        delays[i] = 1000;
        if(New) delays[i] /= 2;
        decays[i] = 0.0f;
    }

    for(i = 0; i < num_echoes; i++)
    {
        Read_Function_Swap(&delays[i], sizeof(int), 1, in);
        if(New) delays[i] /= 2;
    }
    for(i = 0; i < num_echoes; i++)
    {
        Read_Function_Swap(&decays[i], sizeof(float), 1, in);
    }
}

#if !defined(__WINAMP__)
// ------------------------------------------------------
// Save the data to a reverb file (or a module)
void Save_Reverb_Data(int (*Write_Function)(void *, int ,int, FILE *),
                      int (*Write_Function_Swap)(void *, int ,int, FILE *),
                      FILE *out)
{
    int i;

    Write_Function(&num_echoes, sizeof(char), 1, out);

    // Save the reverb data
    for(i = 0; i < num_echoes; i++)
    {
        Write_Function_Swap(&delays[i], sizeof(int), 1, out);
    }
    for(i = 0; i < num_echoes; i++)
    {
        Write_Function_Swap(&decays[i], sizeof(float), 1, out);
    }
}

// ------------------------------------------------------
// Load a reverb file
void Load_Reverb(char *FileName)
{
    FILE *in;
    int New_Reverb = FALSE;
    in = fopen(FileName, "rb");

    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];
        fread(extension, sizeof(char), 9, in);

        if(strcmp(extension, "TWNNREV1") == 0 ||
           strcmp(extension, "PROTREV2") == 0
          )
        {
            switch(extension[7])
            {
                case '2':
                    New_Reverb = TRUE;
                    break;
            }

            // Ok, extension matched!
            Status_Box("Loading Reverb Data...");

            Read_Data(Reverb_Name, sizeof(char), 20, in);
            Load_Reverb_Data(Read_Data, Read_Data_Swap, in, New_Reverb);
            Initreverb();
            Actualize_Reverb_Ed(0);

            Status_Box("Reverb Data Loaded Successfully.");
        }
        else
        {
            Status_Box("That File Is Not A " TITLE " Reverb File...");
        }
        fclose(in);
    }
    else
    {
        Status_Box("Reverb Data Loading Failed. (Possible Cause: File Not Found)");
    }
}

// ------------------------------------------------------
// Save a reverb file
void Save_Reverb(void)
{
    FILE *in;
    char Temph[96];
    char extension[10];

    sprintf(extension, "TWNNREV1");
    sprintf(Temph, "Saving '%s.prv' Data In Reverbs Directory...", Reverb_Name);
    Status_Box(Temph);
    sprintf(Temph, "%s" SLASH "%s.prv", Dir_Reverbs, Reverb_Name);

    in = fopen(Temph, "wb");
    if(in != NULL)
    {
        Write_Data(extension, sizeof(char), 9, in);
        Write_Data(Reverb_Name, sizeof(char), 20, in);

        Save_Reverb_Data(Write_Data, Write_Data_Swap, in);

        fclose(in);
        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Status_Box("Reverb Data Saved Successfully.");   
    }
    else
    {
        Status_Box("Reverb Data Saving Failed.");
    }

    Clear_Input();
}
#endif

// ------------------------------------------------------
// Load the old presets
void Load_Old_Reverb_Presets(int Type)
{
    int i;

    switch(Type)
    {
        case 0:
            decays[0] = 20.0f;
            decays[1] =  0.0f;
            decays[2] = 15.0f;
            decays[3] =  0.0f;
            decays[4] =  9.0f;
            decays[5] =  1.0f;
            decays[6] =  8.0f;
            decays[7] =  1.0f;
            decays[8] =  4.0f;
            decays[9] =  1.0f;

            delays[0] = 100;
            delays[1] = 300; 
            delays[2] = 500;
            delays[3] = 200;
            delays[4] = 800;
            delays[5] = 650;
            delays[6] = 720;
            delays[7] = 450;
            delays[8] = 2100;
            delays[9] = 1500;

            num_echoes = 10;
            break;

        case 1:
            decays[0] =   7.0f;
            decays[1] = -13.0f;
            decays[2] =  25.0f;
            decays[3] =  31.0f;
            decays[4] =  20.0f;
            decays[5] =  28.0f;
            decays[6] = -21.0f;
            decays[7] =  18.0f;
            decays[8] = -13.0f;
            decays[9] =   9.0f;

            delays[0] = 1000 / 2;
            delays[1] = 1600 / 2;
            delays[2] = 2100 / 2;
            delays[3] = 2400 / 2;
            delays[4] = 2290 / 2;
            delays[5] = 2350 / 2;
            delays[6] = 2400 / 2;
            delays[7] = 2500 / 2;
            delays[8] = 2680 / 2;
            delays[9] = 3410 / 2;

            num_echoes = 10;
            break;

        case 2:
            decays[0] =  1.0f;
            decays[1] =  1.0f;
            decays[2] =  9.0f;
            decays[3] = 12.0f;
            decays[4] = 22.0f;
            decays[5] =  1.0f;
            decays[6] = 15.0f;
            decays[7] =  1.0f;
            decays[8] =  7.0f;
            decays[9] =  2.0f;

            delays[0] =  100 / 2;
            delays[1] =  200 / 2;
            delays[2] =  300 / 2;
            delays[3] = 1000 / 2;
            delays[4] = 1190 / 2;
            delays[5] = 1250 / 2;
            delays[6] = 1300 / 2;
            delays[7] = 1400 / 2;
            delays[8] = 1580 / 2;
            delays[9] = 1610 / 2;

            num_echoes = 10;
            break;

        case 3:
            decays[0] = 22.0f;
            decays[1] =  5.0f;
            decays[2] = 12.0f;
            decays[3] =  3.0f;

            delays[0] = 2000 / 2;
            delays[1] = 4400 / 2;
            delays[2] = 5000 / 2;
            delays[3] = 6200 / 2;

            num_echoes = 4;
            break;

        case 4:
            decays[0] = 11.0f;
            decays[1] =  0.0f;
            decays[2] = 31.0f;
            decays[3] =  0.0f;

            delays[0] = 3012 / 2;
            delays[1] = 4012 / 2;
            delays[2] = 4022 / 2;
            delays[3] = 5232 / 2;

            num_echoes = 4;
            break;

        case 5:
            decays[0] =   7.0f;
            decays[1] = -13.0f;
            decays[2] =  25.0f;
            decays[3] =  31.0f;
            decays[4] =  20.0f;
            decays[5] =  28.0f;
            decays[6] = -21.0f;
            decays[7] =  18.0f;
            decays[8] = -13.0f;
            decays[9] =   9.0f;

            delays[0] = 20 / 2;
            delays[1] = 600 / 2;
            delays[2] = 100 / 2;
            delays[3] = 400 / 2;
            delays[4] = 290 / 2;
            delays[5] = 1350 / 2;
            delays[6] = 400 / 2;
            delays[7] = 1500 / 2;
            delays[8] = 1680 / 2;
            delays[9] = 1410 / 2;

            num_echoes = 10;
            break;

        case 6:
            decays[0] =   7.0f;
            decays[1] = -13.0f;
            decays[2] =  25.0f;
            decays[3] =  31.0f;
            decays[4] =  20.0f;
            decays[5] =  28.0f;
            decays[6] = -21.0f;
            decays[7] =  18.0f;
            decays[8] = -13.0f;
            decays[9] =  12.0f;

            delays[0] = 20 / 2;
            delays[1] = 600 / 2; 
            delays[2] = 700 / 2;
            delays[3] = 800 / 2;
            delays[4] = 990 / 2;
            delays[5] = 1350 / 2;
            delays[6] = 1400 / 2;
            delays[7] = 1500 / 2;
            delays[8] = 1680 / 2;
            delays[9] = 1910 / 2;

            num_echoes = 10;
            break;
    }
    // / 64.0f
    for(i = 0; i < num_echoes; i++)
    {
        decays[i] = decays[i] * 0.015625f;
    }
}
