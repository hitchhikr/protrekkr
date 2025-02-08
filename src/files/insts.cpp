// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2025 Franck Charlet.
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
#include "include/insts.h"

// ------------------------------------------------------
// Load an instrument
void Load_Inst(char *FileName)
{
    int old_bug = FALSE;
    int Pack_Scheme = FALSE;
    int Mp3_Scheme = FALSE;
    int new_adsr = FALSE;
    int tight = FALSE;
    int Env_Modulation = FALSE;
    int New_Env = FALSE;
    int Glob_Vol = FALSE;
    int Combine = FALSE;
    int Long_Midi_Prg = FALSE;
    int Var_Disto = FALSE;

    Status_Box("Attempting To Load An Instrument File...", TRUE);
    FILE *in;
    in = fopen(FileName, "rb");

    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];
        Read_Data(extension, sizeof(char), 9, in);

        switch(extension[7])
        {
            case 'B':
                Var_Disto = TRUE;
            case 'A':
                Long_Midi_Prg = TRUE;
            case '9':
                Combine = TRUE;
            case '8':
                Glob_Vol = TRUE;
            case '7':
                New_Env = TRUE;
            case '6':
                Env_Modulation = TRUE;
            case '5':
                Mp3_Scheme = TRUE;
            case '4':
                tight = TRUE;
            case '3':
                new_adsr = TRUE;
            case '2':
                Pack_Scheme = TRUE;
            case '1':
                break;
            case '0':
                old_bug = TRUE;
                break;
        }
        Kill_Instrument(Current_Instrument, TRUE);
        Status_Box("Loading Instrument -> Header...", TRUE); 
        Read_Data(&nameins[Current_Instrument], sizeof(char), 20, in);

        // Reading sample data
        Status_Box("Loading Instrument -> Sample Data...", TRUE);

        int swrite = Current_Instrument;

        if(Long_Midi_Prg)
        {
            Read_Data_Swap(&Midiprg[swrite], sizeof(int), 1, in);
        }
        else
        {
            char OldMidiPrg;

            Read_Data(&OldMidiPrg, sizeof(char), 1, in);
            Midiprg[swrite] = OldMidiPrg;
        }
            
        Read_Data(&Synthprg[swrite], sizeof(char), 1, in);

        PARASynth[swrite].disto = 0;

        PARASynth[swrite].lfo_1_attack = 0;
        PARASynth[swrite].lfo_1_decay = 0;
        PARASynth[swrite].lfo_1_sustain = 128;
        PARASynth[swrite].lfo_1_release = 0x10000;

        PARASynth[swrite].lfo_2_attack = 0;
        PARASynth[swrite].lfo_2_decay = 0;
        PARASynth[swrite].lfo_2_sustain = 128;
        PARASynth[swrite].osc_combine = COMBINE_ADD;

        PARASynth[swrite].lfo_2_release = 0x10000;

        Read_Synth_Params(Read_Data, Read_Data_Swap, in, swrite,
                          !old_bug, new_adsr, tight,
                          Env_Modulation, New_Env, FALSE, Combine, Var_Disto);
        // Gsm by default
        if(Pack_Scheme)
        {
            Read_Data(&SampleCompression[swrite], sizeof(char), 1, in);
        }
        else
        {
            SampleCompression[swrite] = SMP_PACK_WAVPACK;
        }
        // Load the bitrate
        if(Mp3_Scheme)
        {
            switch(SampleCompression[swrite])
            {
                case SMP_PACK_MP3:
                    Read_Data(&Mp3_BitRate[swrite], sizeof(char), 1, in);
                    break;

                case SMP_PACK_AT3:
                    Read_Data(&At3_BitRate[swrite], sizeof(char), 1, in);
                    break;
            }
        }
        SampleCompression[swrite] = Fix_Codec(SampleCompression[swrite]);

        Sample_Vol[swrite] = 1.0f;
        if(Glob_Vol)
        {
            Read_Data_Swap(&Sample_Vol[swrite], sizeof(float), 1, in);
        }

        for(int slwrite = 0; slwrite < MAX_INSTRS_SPLITS; slwrite++)
        {
            Read_Data(&SampleType[swrite][slwrite], sizeof(char), 1, in);
            if(SampleType[swrite][slwrite])
            {
                if(old_bug) Read_Data(&SampleName[swrite][slwrite], sizeof(char), 256, in);
                else Read_Data(&SampleName[swrite][slwrite], sizeof(char), 64, in);
                
                Read_Data(&Basenote[swrite][slwrite], sizeof(char), 1, in);

                Read_Data_Swap(&LoopStart[swrite][slwrite], sizeof(int), 1, in);
                Read_Data_Swap(&LoopEnd[swrite][slwrite], sizeof(int), 1, in);
                Read_Data(&LoopType[swrite][slwrite], sizeof(char), 1, in);
                
                Read_Data_Swap(&Sample_Length[swrite][slwrite], sizeof(int), 1, in);
                Read_Data(&Finetune[swrite][slwrite], sizeof(char), 1, in);
                Read_Data_Swap(&Sample_Amplify[swrite][slwrite], sizeof(float), 1, in);
                Read_Data_Swap(&FDecay[swrite][slwrite], sizeof(float), 1, in);

                Allocate_Wave(swrite, slwrite, Sample_Length[swrite][slwrite], 1, FALSE, NULL, NULL);

                // Samples data
                Read_Data(RawSamples[swrite][0][slwrite], sizeof(short), Sample_Length[swrite][slwrite], in);
                Swap_Sample(RawSamples[swrite][0][slwrite], swrite, slwrite);
                *RawSamples[swrite][0][slwrite] = 0;

                // Number of channel(s)
                Read_Data(&Sample_Channels[swrite][slwrite], sizeof(char), 1, in);
                if(Sample_Channels[swrite][slwrite] == 2)
                {
                    // Stereo
                    RawSamples[swrite][1][slwrite] = (short *) malloc(Sample_Length[swrite][slwrite] * sizeof(short) + 8);
                    memset(RawSamples[swrite][1][slwrite], 0, Sample_Length[swrite][slwrite] * sizeof(short) + 8);
                    Read_Data(RawSamples[swrite][1][slwrite], sizeof(short), Sample_Length[swrite][slwrite], in);
                    Swap_Sample(RawSamples[swrite][1][slwrite], swrite, slwrite);
                    *RawSamples[swrite][1][slwrite] = 0;
                }
            } // Exist Sample
            Recalculate_Sample_Size(swrite, slwrite, TRUE, 0, 0, FALSE, FALSE);
        }
        fclose(in);
        Actualize_Pattern_Ed();
        Actualize_Instrument_Ed(2, 0);
        Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        Status_Box("Instrument Loaded Successfully.", TRUE);
    }
    else
    {
        Status_Box("Instrument Loading Failed. (Possible Cause: File Not Found)", TRUE);
    }
    Clear_Input();
}

// ------------------------------------------------------
// Save the current instrument
void Save_Inst(void)
{
    FILE *in;
    char Temph[MAX_PATH];
    char extension[10];
    char synth_prg;
    int synth_save;

    sprintf(extension, "PROTINSB");

    if(!strlen(nameins[Current_Instrument])) sprintf(nameins[Current_Instrument], "Untitled");
    sprintf (Temph, "Saving '%s.pti' Instrument In Instruments Directory...", nameins[Current_Instrument]);
    Status_Box(Temph, TRUE);
    sprintf(Temph, "%s" SLASH "%s.pti", Dir_Instrs, nameins[Current_Instrument]);

    in = fopen(Temph, "wb");
    if(in != NULL)
    {
        // Writing header & name...
        Write_Data(extension, sizeof(char), 9, in);
        rtrim_string(nameins[Current_Instrument], 20);
        Write_Data(nameins[Current_Instrument], sizeof(char), 20, in);

        // Writing sample data
        int swrite = Current_Instrument;

        Write_Data_Swap(&Midiprg[swrite], sizeof(int), 1, in);

        switch(Synthprg[swrite])
        {
            case SYNTH_WAVE_OFF:
            case SYNTH_WAVE_CURRENT:
                synth_prg = Synthprg[swrite];
                synth_save = swrite;
                break;
            default:
                synth_prg = SYNTH_WAVE_CURRENT;
                synth_save = Synthprg[swrite] - 2;
                break;
        }

        Write_Data(&synth_prg, sizeof(char), 1, in);

        Write_Synth_Params(Write_Data, Write_Data_Swap, in, swrite);

        Write_Data(&SampleCompression[swrite], sizeof(char), 1, in);
        switch(SampleCompression[swrite])
        {
            case SMP_PACK_MP3:
                Write_Data(&Mp3_BitRate[swrite], sizeof(char), 1, in);
                break;

            case SMP_PACK_AT3:
                Write_Data(&At3_BitRate[swrite], sizeof(char), 1, in);
                break;
        }

        Write_Data_Swap(&Sample_Vol[swrite], sizeof(float), 1, in);

        swrite = synth_save;
        for(int slwrite = 0; slwrite < MAX_INSTRS_SPLITS; slwrite++)
        {
            Write_Data(&SampleType[swrite][slwrite], sizeof(char), 1, in);
            if(SampleType[swrite][slwrite])
            {
                rtrim_string(SampleName[swrite][slwrite], 64);
                Write_Data(SampleName[swrite][slwrite], sizeof(char), 64, in);
                Write_Data(&Basenote[swrite][slwrite], sizeof(char), 1, in);
                
                Write_Data_Swap(&LoopStart[swrite][slwrite], sizeof(int), 1, in);
                Write_Data_Swap(&LoopEnd[swrite][slwrite], sizeof(int), 1, in);
                Write_Data(&LoopType[swrite][slwrite], sizeof(char), 1, in);
                
                Write_Data_Swap(&Sample_Length[swrite][slwrite], sizeof(int), 1, in);
                Write_Data(&Finetune[swrite][slwrite], sizeof(char), 1, in);
                Write_Data_Swap(&Sample_Amplify[swrite][slwrite], sizeof(float), 1, in);
                Write_Data_Swap(&FDecay[swrite][slwrite], sizeof(float), 1, in);

                Write_Unpacked_Sample(Write_Data, in, swrite, slwrite);

            } // Exist Sample
        }
        fclose(in);

        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Actualize_Pattern_Ed();
        Status_Box("Instrument Saved Successfully.", TRUE);
    }
    else
    {
        Status_Box("Instrument Saving Failed.", TRUE);
    }

    Clear_Input();
}
