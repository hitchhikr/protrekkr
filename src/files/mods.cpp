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
#include "include/mods.h"

// ------------------------------------------------------
// Variables
#if !defined(__WINAMP__)
int mt_tags[16] =
{
    'NHC1', 'NHC2', 'NHC3', '.K.M',
    'NHC5', 'NHC6', 'NHC7', 'NHC8',
    'NHC9', 'HC01', 'HC11', 'HC21',
    'HC31', 'HC41', 'HC51', 'HC61'
};

char mt_channels[16] =
{
     1,  2,  3,  4,
     5,  6,  7,  8,
     9, 10, 11, 12,
    13, 14, 15, 16
};

float mt_pannels[16] =
{
    0.30f, 0.70f, 0.70f, 0.30f,
    0.30f, 0.70f, 0.70f, 0.30f,
    0.30f, 0.70f, 0.70f, 0.30f,
    0.30f, 0.70f, 0.70f, 0.30f
};

AMIGA_NOTE mt_period_conv[] =
{
    { 13696, 0 },
    { 12928, 1 },
    { 12192, 2 },
    { 11520, 3 },
    { 10848, 4 },
    { 10240, 5 },
    { 9664,  6 },
    { 9120,  7 },
    { 8608,  8 },
    { 8128,  9 },
    { 7680, 10 },
    { 7248, 11 },

    { 6848, 12 },
    { 6464, 13 },
    { 6096, 14 },
    { 5760, 15 },
    { 5424, 16 },
    { 5120, 17 },
    { 4832, 18 },
    { 4560, 19 },
    { 4304, 20 },
    { 4064, 21 },
    { 3840, 22 },
    { 3624, 23 },

    { 3424, 24 },
    { 3232, 25 },
    { 3048, 26 },
    { 2880, 27 },
    { 2712, 28 },
    { 2560, 29 },
    { 2416, 30 },
    { 2280, 31 },
    { 2152, 32 },
    { 2032, 33 },
    { 1920, 34 },
    { 1812, 35 },

    { 1712, 36 },
    { 1616, 37 },
    { 1524, 38 },
    { 1440, 39 },
    { 1356, 40 },
    { 1280, 41 },
    { 1208, 42 },
    { 1140, 43 },
    { 1076, 44 },
    { 1016, 45 },
    {  960, 46 },
    {  906, 47 },

    {  856, 48 },
    {  808, 49 },
    {  762, 50 },
    {  720, 51 },
    {  678, 52 },
    {  640, 53 },
    {  604, 54 },
    {  570, 55 },
    {  538, 56 },
    {  508, 57 },
    {  480, 58 },
    {  453, 59 },
    
    {  428, 60 },
    {  404, 61 },
    {  381, 62 },
    {  360, 63 },
    {  339, 64 },
    {  320, 65 },
    {  302, 66 },
    {  285, 67 },
    {  269, 68 },
    {  254, 69 },
    {  240, 70 },
    {  226, 71 },
    
    {  214, 72 },
    {  202, 73 },
    {  190, 74 },
    {  180, 75 },
    {  170, 76 },
    {  160, 77 },
    {  151, 78 },
    {  143, 79 },
    {  135, 80 },
    {  127, 81 },
    {  120, 82 },
    {  113, 83 },

    {  107, 84 },
    {  101, 85 },
    {   95, 86 },
    {   90, 87 },
    {   85, 88 },
    {   80, 89 },
    {   75, 90 },
    {   71, 91 },
    {   67, 92 },
    {   63, 93 },
    {   60, 94 },
    {   57, 95 },

    {   53, 96 },
    {   50, 97 },
    {   47, 98 },
    {   45, 99 },
    {   42, 100 },
    {   40, 101 },
    {   38, 102 },
    {   36, 103 },
    {   34, 104 },
    {   32, 105 },
    {   30, 106 },
    {   28, 107 },

    {   26, 108 },
    {   25, 109 },
    {   23, 110 },
    {   22, 111 },
    {   21, 112 },
    {   20, 113 },
    {   18, 114 },
    {   17, 115 },
    {   16, 116 },
    {   15, 117 },
    {   15, 118 },
    {   14, 119 },
}; 

unsigned char *Mod_Dat;
int Pos_Mod_Dat;
int Mod_Size;

// ------------------------------------------------------
// Retrieve data from the .mod data
void Read_Mod(void *Dest, int Size)
{
    memcpy(Dest, Mod_Dat + Pos_Mod_Dat, Size);
    Pos_Mod_Dat += Size;
}

// ------------------------------------------------------
// Retrieve the current position in a .mod
int Mod_Pos()
{
    return Pos_Mod_Dat;
}

// ------------------------------------------------------
// Retrieve a byte from the .mod data
unsigned char Getc_Mod()
{
    unsigned char ret;
    ret = Mod_Dat[Pos_Mod_Dat];
    Pos_Mod_Dat++;
    return ret;
}

// ------------------------------------------------------
// Retrieve a word from the .mod data
unsigned short Getc_Mod_Word()
{
    return (int) (Getc_Mod() << 8) + (int) Getc_Mod();
}

// ------------------------------------------------------
// Retrieve a double word from the .mod data
unsigned int Getc_Mod_Dword()
{
    return (int) (Getc_Mod() << 24) + 
           (int) (Getc_Mod() << 16) + 
           (int) (Getc_Mod() << 8) + 
           (int) Getc_Mod();
}

// ------------------------------------------------------
// Retrieve data from the .mod data
void Seek_Mod(int Pos, int type)
{
    switch(type)
    {
        case SEEK_SET:
            Pos_Mod_Dat = Pos;
            break;
        case SEEK_CUR:
            Pos_Mod_Dat += Pos;
            break;
        case SEEK_END:
            Pos_Mod_Dat = Mod_Size - Pos;
            break;
    }
}

// ------------------------------------------------------
// Convert an amiga note for our own purpose
int Conv_Amiga_Note(int note)
{
    int i;
    for(i = 0; i < sizeof(mt_period_conv) / sizeof(AMIGA_NOTE); i++)
    {
        if(note == mt_period_conv[i].old_note)
        {
            return(mt_period_conv[i].new_note);
        }
    }
    return(121);
}

// ------------------------------------------------------
// Scale an amiga protracker .mod effect data
float Scale_Amiga_Mod_Value(int value, float scale1, float scale2)
{
    float newvalue = (float) value;
    newvalue /= scale1;
    newvalue *= scale2;
    if(newvalue > scale2) newvalue = scale2;
    return(newvalue);
}

// ------------------------------------------------------
// Load a .mod, .ft or .digi file
void Load_Amiga_Mod(char *Name, const char *FileName, int channels, int digibooster)
{
    FILE *in;
    int t_hi;
    int t_lo;
    int swrite;
    int tps;
    int pwrite;
    int li2;
    int pw2;
    //float volume;
    unsigned int x;
    //int ramp;
    //float ramp_vol;
    int i;
    int j;
    int k;
    int vib_chan[16];
    int vib_last[16];
    int arp_chan[16];
    int last_vol_fade[16];
    int cur_tempo = 125;
    int cur_speed = 6;
    int found_tempo;
    int found_speed;
    int last_speed;
    int last_tempo;

    char FineTune_Table[] =
    {
         0,   16,  32,  48,  64,  80,  96, 112,
       -112, -96, -80, -64, -48, -32, -24, -16
    };

    for(i = 0; i < 16; i++)
    {
        vib_chan[i] = 0;
        vib_last[i] = 0;
        arp_chan[i] = 0;
        last_vol_fade[i] = 0;
    }

    in = fopen(FileName, "rb");
    if(in != NULL)
    {
        fseek(in, 0, SEEK_END);
        Mod_Size = ftell(in);
        fseek(in, 0, SEEK_SET);

        Mod_Dat = (unsigned char *) malloc(Mod_Size);
        if(Mod_Dat)
        {
            fread(Mod_Dat, 1, Mod_Size, in);
            fclose(in);
            Pos_Mod_Dat = 0;

            Song_Playing = FALSE;

            Free_Samples();
            Clear_Patterns_Pool();

#if !defined(__NO_MIDI__) && !defined(__WINAMP__)
            Midi_Reset();
#endif

            init_sample_bank();
            Pre_Song_Init();
            int Patterns;

            if(digibooster)
            {
                // Convert the digi booster pro module to a protracker one
                // Determine the amount of patterns and samples
                unsigned char *Digi_Mod;
                int Smps_Length = 0;
                int PattSize;
                int Old_Pos;
                int Packed;
                unsigned int Version;
                unsigned char BitMasks[64];
                unsigned char *bDigi_Mod;
                unsigned short *wDigi_Mod;
                unsigned short *wDigi_Mod2;
                
                // Check the version
                Seek_Mod(24, SEEK_SET);
                Version = Getc_Mod();
                Seek_Mod(26, SEEK_SET);
                Packed = Getc_Mod();
                Seek_Mod(46, SEEK_SET);
                // Total length of patterns
                if(Version < 0x12)
                {
                    Patterns = Getc_Mod();
                }
                else
                {
                    Patterns = Getc_Mod() + 1;
                }
                Seek_Mod(176, SEEK_SET);
                // Total length of samples
                for(i = 0; i < 31; i++)
                {
                    Smps_Length += Getc_Mod_Dword();
                }
                Digi_Mod = (unsigned char *) malloc((Patterns * 64 * 4 * channels) + Smps_Length + 1084);
                if(!Digi_Mod)
                {
                    Status_Box("Not enough memory.");
                    Clear_Input();
                    return;
                }
                memset(Digi_Mod, 0, (Patterns * 64 * 4 * channels) + Smps_Length + 1084);
                
                Seek_Mod(47, SEEK_SET);
                int Length = Getc_Mod() + 1;
                Digi_Mod[950] = Length;
                for(i = 0; i < 128; i++)
                {
                    Digi_Mod[952 + i] = Getc_Mod();
                }
                for(i = 0; i < 31; i++)
                {
                    Sample_Length[i][0] = Getc_Mod_Dword();
                }
                for(i = 0; i < 31; i++)
                {
                    LoopStart[i][0] = Getc_Mod_Dword();
                }
                /* (Loop length) */
                for(i = 0; i < 31; i++)
                {
                    LoopEnd[i][0] = Getc_Mod_Dword();
                }
                for(i = 0; i < 31; i++)
                {
                    Sample_Vol[i] = Scale_Amiga_Mod_Value(Getc_Mod(), 63.0f, 0.99f);
                }
                for(i = 0; i < 31; i++)
                {
                    Finetune[i][0] = FineTune_Table[Getc_Mod() & 0xf];
                }

                memset(Name, 0, 21);
                Read_Mod((unsigned char *) Name, 20);

                Seek_Mod(642, SEEK_SET);
                
                for(i = 0; i < 31; i++)
                {
                    memset(nameins[i], 0, 20);
                    Read_Mod(&nameins[i], 19);
                    Seek_Mod(30 - 19, SEEK_CUR);
                }

                wDigi_Mod = (unsigned short *) (Digi_Mod + 1084);

                // Decode the pattern data
                if(Packed)
                {
                    // Packed patterns
                    for(k = 0; k < Patterns; k++)
                    {
                        Old_Pos = Mod_Pos();
                        PattSize = Getc_Mod_Word() + 2;
                        Read_Mod(BitMasks, 64);
                        for(i = 0; i < 64; i++)
                        {
                            for(j = 7; j >= 0; j--)
                            {
                                // Check bit mask
                                if(BitMasks[i] & (1 << j))
                                {
                                    int value;
                                    value = (int) Getc_Mod();
                                    value |= (int) (Getc_Mod() << 8);
                                    *wDigi_Mod++ = value;
                                    value = (int) Getc_Mod();
                                    value |= (int) (Getc_Mod() << 8);
                                    *wDigi_Mod++ = value;
                                }
                                else
                                {
                                    *wDigi_Mod++ = 0;
                                    *wDigi_Mod++ = 0;
                                }
                            }
                        }
                        Seek_Mod(Old_Pos + PattSize, SEEK_SET);
                    }
                }
                else
                {
                    // Non-packed patterns
                    for(k = 0; k < Patterns; k++)
                    {
                        for(j = 0; j < channels; j++)
                        {
                            wDigi_Mod2 = &wDigi_Mod[j * 2];
                            for(i = 0; i < 64; i++)
                            {
                                int value;
                                value = (int) Getc_Mod();
                                value |= (int) (Getc_Mod() << 8);
                                wDigi_Mod2[0] = value;
                                value = (int) Getc_Mod();
                                value |= (int) (Getc_Mod() << 8);
                                wDigi_Mod2[1] = value;
                                wDigi_Mod2 += (channels * 2);
                            }
                        }
                        wDigi_Mod += (channels * 2 * 64);
                    }
                }

                bDigi_Mod = (unsigned char *) wDigi_Mod;

                // Copy the samples
                for(i = 0; i < 31; i++)
                {
                    if(Sample_Length[i][0])
                    {
                        Read_Mod(bDigi_Mod, Sample_Length[i][0]);
                        bDigi_Mod += Sample_Length[i][0];
                    }
                }

                free(Mod_Dat);
                // This is the new mod
                Mod_Dat = Digi_Mod;
            }

            Seek_Mod(0, SEEK_SET);

            // Read the title
            sprintf(style, "Converted");
            if(!digibooster)
            {
                memset(Name, 0, 21);
                Read_Mod(Name, 20);
            }
            else
            {
                Seek_Mod(20, SEEK_CUR);
            }
            if(!strlen(Name))
            {
                strcpy(Name, FileName);
                i = strlen(Name) - 1;
                while(i)
                {
                    if(Name[i] == '.')
                    {
                        for(j = i; Name[j]; j++)
                        {
                            Name[j] = 0;
                        }
                        break;
                    }
                    i--;
                }
            }

            // Remove suspicious chars
            for(j = 0; Name[j]; j++)
            {
                if(!isspace(Name[j]))
                {
                    if(!isalnum(Name[j]))
                    {
                        Name[j] = '_';
                    }
                }
            }
        
            Songtracks = channels;

            for(swrite = 0; swrite < 31; swrite++)
            {
                SampleType[swrite][0] = 1;

                if(!digibooster)
                {
                    memset(nameins[swrite], 0, 20);
                    Read_Mod(&nameins[swrite], 19);

                    // Jump over 3 unhandled bytes for PTK samplename.
                }
                else
                {
                    Seek_Mod(19, SEEK_CUR);
                }
                Seek_Mod(3, SEEK_CUR);

                // Remove suspicious chars
                for(j = 0; nameins[swrite][j]; j++)
                {
                    if(!isalnum(nameins[swrite][j])) nameins[swrite][j] = '_';
                }
    
                sprintf(SampleName[swrite][0], "Untitled.wav");
                    
                if(!digibooster)
                {
                    Clear_Instrument_Dat(swrite, 0, 0);

                    Sample_Length[swrite][0] = Getc_Mod_Word();
                    Sample_Length[swrite][0] *= 2;

                    Finetune[swrite][0] = FineTune_Table[Getc_Mod() & 0xf];
                    Sample_Vol[swrite] = Scale_Amiga_Mod_Value(Getc_Mod(), 63.0f, 0.99f);

                    // Calculate/Adapt AMIGA loop points to ptk LoopPoints
                    LoopStart[swrite][0] = Getc_Mod_Word();
                    LoopStart[swrite][0] *= 2;
                    LoopEnd[swrite][0] = Getc_Mod_Word();
                    LoopEnd[swrite][0] *= 2;
                }
                else
                {
                    Seek_Mod(2 + 1 + 1 + 2 + 2, SEEK_CUR);
                }

                Sample_Amplify[swrite][0] = 0.5f;
                Basenote[swrite][0] = DEFAULT_BASE_NOTE - 5 + 12 + 12 + 12 + 12 - 2;
                if(LoopEnd[swrite][0] > 2)
                {
                    LoopEnd[swrite][0] = LoopStart[swrite][0] + LoopEnd[swrite][0];
                    LoopType[swrite][0] = SMP_LOOP_FORWARD;
                }
                else
                {
                    LoopEnd[swrite][0] = Sample_Length[swrite][0];
                    LoopType[swrite][0] = SMP_LOOP_NONE;
                }

                if(LoopEnd[swrite][0] > Sample_Length[swrite][0])
                {
                    LoopEnd[swrite][0] = LoopEnd[swrite][0] - Sample_Length[swrite][0];
                }
            } // FOR

            Read_Mod(&Song_Length, 1);
            Getc_Mod();

            last_speed = 4;
            last_tempo = 125;

            // PATTERN PROGRESSION;
            nPatterns = 1;

            for(tps = 0; tps < 128; tps++)
            {
                pSequence[tps] = Getc_Mod();
                if(pSequence[tps] >= nPatterns) nPatterns = pSequence[tps] + 1;
                patternLines[tps] = 64;
            }
            if(digibooster)
            {
                nPatterns = Patterns;
            }

            // JUMP OVER THE HEADER
            Seek_Mod(4, SEEK_CUR);

            for(pwrite = 0; pwrite < Song_Length; pwrite++)
            {
                // All 64 rows of the .mod
                for(li2 = 0; li2 < 64; li2++)
                {
                    Seek_Mod(1084 + (pSequence[pwrite] * (4 * channels * 64) + (4 * channels * li2)), SEEK_SET);

                    // Precalc the speed for that row
                    for(pw2 = 0; pw2 < channels; pw2++)
                    {
                        int tmo = Get_Pattern_Offset(pSequence[pwrite], pw2, li2);
                        int f_byte = Getc_Mod();
                        int t_sample = f_byte >> 4;
                        int t_period = ((f_byte - (t_sample << 4)) << 8) + (int) Getc_Mod();
                        int t_byte = Getc_Mod();
                        int t_sample2 = t_byte >> 4;
                        int Cmd = t_byte - (t_sample2 << 4);
                        int Cmd_Dat = Getc_Mod();

                        switch(Cmd)
                        {
                            // SPEED
                            case 0xf:
                                if(Cmd_Dat > 31) cur_tempo = Cmd_Dat;
                                else cur_speed = Cmd_Dat;

                                // Get the speed to find
                                int found_exact_st;
                                int found_free_chan;
                                float speed_to_find = cur_tempo / 125.0f * 50.0f / (float) cur_speed;
                                speed_to_find *= 60.0f;
                                speed_to_find = (float) ((int) speed_to_find);

                                // Now we need to find the exact match for that speed
                                // the ugly big bad brute force way >:D
                
                                Cmd = 0;
                                Cmd_Dat = 0;

                                // Loop over speeds
                                found_exact_st = FALSE;
                                for(j = 1; j < 32; j++)
                                {
                                    // Loop over tempos
                                    for(i = 20; i < 255; i++)
                                    {
                                        if(speed_to_find == (((float) i * (float) j)))
                                        {
                                            found_exact_st = TRUE;
                                            found_tempo = i;
                                            found_speed = j;
                                            // Always use the 2nd fx to set the tempo
                                        
                                            // We can put this on any track so look for a free one

                                            found_free_chan = FALSE;
                                            // Check if we already have recorded it
                                            for(k = 0; k < channels; k++)
                                            {
                                                int free_chan = Get_Pattern_Offset(pSequence[pwrite], k, li2);
                                                if(*(RawPatterns + free_chan + PATTERN_FX2) == 0xf0 &&
                                                   *(RawPatterns + free_chan + PATTERN_FXDATA2) == found_tempo)
                                                {
                                                    found_free_chan = TRUE;
                                                }
                                            }

                                            if(!found_free_chan)
                                            {
                                                for(k = 0; k < channels; k++)
                                                {
                                                    int free_chan = Get_Pattern_Offset(pSequence[pwrite], k, li2);
                                                    if(!*(RawPatterns + free_chan + PATTERN_FX2))
                                                    {
                                                        found_free_chan = TRUE;
                                                        //if(last_tempo != found_tempo)
                                                        {
                                                            *(RawPatterns + free_chan + PATTERN_FX2) = 0xf0;
                                                            *(RawPatterns + free_chan + PATTERN_FXDATA2) = found_tempo;
                                                            Channels_Effects[k] = 2;
                                                            last_tempo = found_tempo;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                            if(!found_free_chan)
                                            {
                                                // Extend more (now we can do that)
                                                //if(last_tempo != found_tempo)
                                                {
                                                    *(RawPatterns + tmo + PATTERN_FX3) = 0xf0;
                                                    *(RawPatterns + tmo + PATTERN_FXDATA3) = found_tempo;
                                                    Channels_Effects[pw2] = 3;
                                                    last_tempo = found_tempo;
                                                }
                                            }
                                            Cmd = 0xf;
                                            Cmd_Dat = found_speed;
                                            break;
                                        }
                                    }
                                    if(found_exact_st) break;
                                }
                                //if(last_speed != Cmd_Dat)
                                {
                                    *(RawPatterns + tmo + PATTERN_FX) = Cmd;
                                    *(RawPatterns + tmo + PATTERN_FXDATA) = Cmd_Dat;
                                    last_speed = Cmd_Dat;
                                }
                                break;
                        } // Pattern FX adapter end.
                    }

                    // Jump to the pattern to retrieve
                    Seek_Mod(1084 + (pSequence[pwrite] * (4 * channels * 64) + (4 * channels * li2)), SEEK_SET);
                    for(pw2 = 0; pw2 < channels; pw2++)
                    {
                        int tmo = Get_Pattern_Offset(pSequence[pwrite], pw2, li2);
                        int f_byte = Getc_Mod();
                        int t_sample = f_byte >> 4;
                        int t_period = ((f_byte - (t_sample << 4)) << 8) + (int) Getc_Mod();
                        int t_byte = Getc_Mod();
                        int t_sample2 = t_byte >> 4;
                        int Cmd = t_byte - (t_sample2 << 4);
                        t_sample = (t_sample << 4) + t_sample2;
                        if(t_sample == 0) t_sample = 255;
                        if(t_sample != 255) t_sample--;

                        int Note = 121;                     // No note
                        int Cmd_Dat = Getc_Mod();

                        // Period Conversion Table
                        Note = Conv_Amiga_Note(t_period);

                        *(RawPatterns + tmo + PATTERN_NOTE1) = Note;
                        *(RawPatterns + tmo + PATTERN_INSTR1) = t_sample;

                        // There was a vibrato
                        if(vib_chan[pw2] == TRUE)
                        {
                            // But no more
                            if(Cmd != 4 && Cmd != 6)
                            {
                                if(*(RawPatterns + tmo + PATTERN_FX2) == 0x1d)
                                {
                                    vib_chan[pw2] = FALSE;
                                }
                                else
                                {
                                    // Stop it or delay it
                                    if(!*(RawPatterns + tmo + PATTERN_FX2))
                                    {
                                        *(RawPatterns + tmo + PATTERN_FX2) = 0x1d;
                                        *(RawPatterns + tmo + PATTERN_FXDATA2) = 0;
                                        Channels_Effects[pw2] = 2;
                                        vib_chan[pw2] = FALSE;
                                    }
                                }
                            }
                        }

                        // There was an arpeggio
                        if(arp_chan[pw2] == TRUE)
                        {
                            // But no more
                            if(Cmd != 0 || (Cmd == 0 && Cmd_Dat == 0))
                            {
                                if(*(RawPatterns + tmo + PATTERN_FX2) == 0x1b)
                                {
                                    arp_chan[pw2] = FALSE;
                                }
                                else
                                {
                                    // Stop it or delay it
                                    if(!*(RawPatterns + tmo + PATTERN_FX2))
                                    {
                                        *(RawPatterns + tmo + PATTERN_FX2) = 0x1b;
                                        *(RawPatterns + tmo + PATTERN_FXDATA2) = 0;
                                        Channels_Effects[pw2] = 2;
                                        arp_chan[pw2] = FALSE;
                                    }
                                }
                            }
                        }

                        // (Speed has already been converted)
                        if(Cmd != 0xf)
                        {
                            // Pattern effect adapter:
                            switch(Cmd)
                            {
                                // ARPEGGIO
                                case 0:
                                    if(Cmd_Dat)
                                    {
                                        // Our arpeggios are different from theirs
                                        Cmd = 0x1b;
                                        arp_chan[pw2] = TRUE;
                                    }
                                    else Cmd = 0;
                                    break;

                                // TODO: effects like 1 2 3 5 6 a depend on the current speed in .mods.
                                //       (the faster the speed, the less effect).

                                // PITCH UP
                                case 1:
                                    Cmd_Dat = (int) Scale_Amiga_Mod_Value(Cmd_Dat, (31 - (cur_speed - 1)) * 2.5f, 255.0f);
                                    break;

                                // PITCH DOWN
                                case 2:
                                    Cmd_Dat = (int) Scale_Amiga_Mod_Value(Cmd_Dat, (31 - (cur_speed - 1)) * 2.5f, 255.0f);
                                    break;

                                // TONE PORTAMENTO
                                case 3:
                                    Cmd = 5;
                                    break;

                                // VIBRATO
                                case 4:
                                    Cmd = 0x1d;
                                    // Our vibratos are different from theirs
                                    if(Cmd_Dat == 0) Cmd_Dat = vib_last[pw2];
                                    else vib_last[pw2] = Cmd_Dat;
                                    vib_chan[pw2] = TRUE;
                                    break;

                                // TONE PORTAMENTO + VOL SLIDE
                                case 5:
                                    // Use the 2nd fx slot to mix both commands
                                    if(Cmd_Dat >= 16)
                                    {
                                        // Vol SlideUp
                                        *(RawPatterns + tmo + PATTERN_FX2) = 0x19;
                                        *(RawPatterns + tmo + PATTERN_FXDATA2) = (int) Scale_Amiga_Mod_Value(Cmd_Dat >> 4,
                                                                                                             (31 - (cur_speed - 1)) * 2.5f,
                                                                                                             255.0f);
                                        Channels_Effects[pw2] = 2;
                                    }
                                    else
                                    {
                                        *(RawPatterns + tmo + PATTERN_FX2) = 0x1a;
                                        *(RawPatterns + tmo + PATTERN_FXDATA2) = (int) Scale_Amiga_Mod_Value(Cmd_Dat & 0xf,
                                                                                                             (31 - (cur_speed - 1)) * 2.5f,
                                                                                                             255.0f);
                                        Channels_Effects[pw2] = 2;
                                    }
                                    // (The portamento retains the value initiated with command 3)
                                    Cmd = 5;
                                    Cmd_Dat = 0;
                                    break;

                                // VIBRATO + VOL SLIDE
                                case 6:
                                    // Use the 2nd fx slot to mix both commands
                                    if(Cmd_Dat >= 16)
                                    {
                                        // Vol SlideUp
                                        *(RawPatterns + tmo + PATTERN_FX2) = 0x19;
                                        *(RawPatterns + tmo + PATTERN_FXDATA2) = (int) Scale_Amiga_Mod_Value(Cmd_Dat >> 4,
                                                                                                             (31 - (cur_speed - 1)) * 2.5f,
                                                                                                             255.0f);
                                        Channels_Effects[pw2] = 2;
                                    }
                                    else
                                    {
                                        *(RawPatterns + tmo + PATTERN_FX2) = 0x1a;
                                        *(RawPatterns + tmo + PATTERN_FXDATA2) = (int) Scale_Amiga_Mod_Value(Cmd_Dat & 0xf,
                                                                                                             (31 - (cur_speed - 1)) * 2.5f,
                                                                                                             255.0f);
                                        Channels_Effects[pw2] = 2;
                                    }
                                    // (The vibrato retains the value initiated with command 4)
                                    Cmd = 0x1d;
                                    Cmd_Dat = vib_last[pw2];
                                    vib_chan[pw2] = TRUE;
                                    break;

                                // DEMOSYNCHRO SIGNAL ?
                                case 8:
                                    // Let's assume it's a fasttracker 1 module
                                    // Which could handle panning.
                                    if(channels > 4)
                                    {
                                        Cmd_Dat >>= 1;
                                        *(RawPatterns + tmo + PATTERN_PANNING) = Cmd_Dat;
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }
                                    else
                                    {
                                        Cmd = 7;
                                    }
                                    break;

                                // SAMPLE OFFSET
                                case 9:
                                    // We use 256 bytes instead of 128
                                    //Cmd_Dat <<= 1;
                                    break;

                                // VOLSLIDE
                                case 0xa:
                                    if(Cmd_Dat >= 16)
                                    {
                                        Cmd = 0x19; // Vol SlideUp
                                        if(digibooster)
                                        {
                                            if(Cmd_Dat == 0) Cmd_Dat = (last_vol_fade[pw2]);
                                            else last_vol_fade[pw2] = Cmd_Dat;
                                        }
                                        Cmd_Dat = (int) Scale_Amiga_Mod_Value(Cmd_Dat >> 4,
                                                                              (31 - (cur_speed - 1)) * 2.5f,
                                                                              255.0f);

                                    }
                                    else
                                    {
                                        Cmd = 0x1a; // Vol Slide Down
                                        if(digibooster)
                                        {
                                            if(Cmd_Dat == 0) Cmd_Dat = last_vol_fade[pw2];
                                            else last_vol_fade[pw2] = Cmd_Dat;
                                        }
                                        Cmd_Dat = (int) Scale_Amiga_Mod_Value(Cmd_Dat & 0xf,
                                                                              (31 - (cur_speed - 1)) * 2.5f,
                                                                              255.0f);
                                    }
                                    break;

                                // POSITION JUMP
                                case 0xb:
                                    Cmd = 0x1f;
                                    break;

                                // VOLUME
                                case 0xc:
                                    Cmd = 3;
                                    Cmd_Dat = (int) Scale_Amiga_Mod_Value(Cmd_Dat, 63.0f, 255.0f);
                                    break;

                                // PATTERN BREAK
                                case 0xd:
                                    // Protracker handles this as BCD
                                    // So convert it to hex
                                    t_hi = (Cmd_Dat >> 4);
                                    t_lo = Cmd_Dat - (t_hi << 4);
                                    Cmd_Dat = (t_hi * 10) + t_lo;
                                    break;

                                // EXX Special Effects
                                // [not a full implementation of all Protracker MOD Exx commands]
                                case 0xe:
                                    Cmd = 0;

                                    // FILTER (not supported)
                                    if(Cmd_Dat >= 0x00 && Cmd_Dat < 0x10)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // FINE PORTAMENTO UP
                                    if(Cmd_Dat >= 0x10 && Cmd_Dat < 0x20)
                                    {
                                        Cmd = 0x22;
                                        Cmd_Dat = (int) Scale_Amiga_Mod_Value(Cmd_Dat - 0x10, 15.0f, 255.0f);
                                    }

                                    // FINE PORTAMENTO DOWN
                                    if(Cmd_Dat >= 0x20 && Cmd_Dat < 0x30)
                                    {
                                        Cmd = 0x23;
                                        Cmd_Dat = (int) Scale_Amiga_Mod_Value(Cmd_Dat - 0x20, 15.0f, 255.0f);
                                    }

                                    // SET GLISS CONTROL (not supported)
                                    if(Cmd_Dat >= 0x30 && Cmd_Dat < 0x40)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // SET VIBRATO CONTROL (not supported)
                                    if(Cmd_Dat >= 0x40 && Cmd_Dat < 0x50)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // SET FINETUNE (not supported)
                                    if(Cmd_Dat >= 0x50 && Cmd_Dat < 0x60)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // PATTERN LOOP
                                    if(Cmd_Dat >= 0x60 && Cmd_Dat < 0x70)
                                    {
                                        Cmd = 0x06;
                                        Cmd_Dat = Cmd_Dat - 0x60;
                                    }

                                    // SET TREMOLO CONTROL (not supported)
                                    if(Cmd_Dat >= 0x70 && Cmd_Dat < 0x80)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // NOTHING
                                    if(Cmd_Dat >= 0x80 && Cmd_Dat < 0x90)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // NOTE RETRIGGER
                                    if(Cmd_Dat >= 0x90 && Cmd_Dat < 0xa0)
                                    {
                                        Cmd = 0xe;
                                        Cmd_Dat = Cmd_Dat - 0x90;
                                    }

                                    // FINEVOLUME SLIDEUP
                                    if(Cmd_Dat >= 0xa0 && Cmd_Dat < 0xb0)
                                    {
                                        Cmd = 0x20;
                                        Cmd_Dat = (int) (Scale_Amiga_Mod_Value(Cmd_Dat - 0xa0, 16.0f, 255.0f) / 2.33333f);
                                    }

                                    // FINEVOLUME SLIDEDOWN
                                    if(Cmd_Dat >= 0xb0 && Cmd_Dat < 0xc0)
                                    {
                                        Cmd = 0x21;
                                        Cmd_Dat = (int) (Scale_Amiga_Mod_Value(Cmd_Dat - 0xb0, 16.0f, 255.0f) / 2.33333f);
                                    }

                                    // NOTE CUT
                                    if(Cmd_Dat >= 0xc0 && Cmd_Dat < 0xd0)
                                    {
                                        *(RawPatterns + tmo + PATTERN_VOLUME) = (Cmd_Dat & 0xf) | 0xf0;
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // NOTE DELAY (not supported)
                                    if(Cmd_Dat >= 0xd0 && Cmd_Dat < 0xe0)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // PATTERN DELAY (not supported)
                                    if(Cmd_Dat >= 0xe0 && Cmd_Dat < 0xf0)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }

                                    // FUNKIT (not supported)
                                    if(Cmd_Dat >= 0xf0 && Cmd_Dat <= 0xff)
                                    {
                                        Cmd = 0;
                                        Cmd_Dat = 0;
                                    }
                                    break;

                            } // Pattern FX adapter end.

                            *(RawPatterns + tmo + PATTERN_FX) = Cmd;
                            *(RawPatterns + tmo + PATTERN_FXDATA) = Cmd_Dat;
                        }
                    }
                }
            }

            Seek_Mod(1084 + (nPatterns * (4 * channels * 64)), SEEK_SET);

            // Load the samples
            for(swrite = 0; swrite < 31; swrite++)
            {
                if(Sample_Length[swrite][0] > 8)
                {
                    // Reserving space for 16-Bit Signed Short Data.
                    Allocate_Wave(swrite, 0, Sample_Length[swrite][0], 1, FALSE, NULL, NULL);
                    for(x = 0; x < Sample_Length[swrite][0]; x++)
                    {
                        *(RawSamples[swrite][0][0] + x) = ((short) Getc_Mod() << 8);
                    }
                    *(RawSamples[swrite][0][0]) = 0;
                }
                else
                {
                    // NO SAMPLE
                    SampleType[swrite][0] = 0;
                }
            }

            for(i = 0; i < channels; i++)
            {
                TPan[i] = mt_pannels[i];
                ComputeStereo(i);
                FixStereo(i);
            }

            Use_Cubic = NONE_INT;
            Beats_Per_Min = 125;
            Ticks_Per_Beat = 4;
            mas_vol = 0.75f;
            compressor = FALSE;

            free(Mod_Dat);
            Mod_Dat = NULL;

            Load_Old_Reverb_Presets(0);
            Init_Tracker_Context_After_ModLoad();

        }
        else
        {
            Status_Box("Not enough memory.");
        }
    }
    else
    {
        Status_Box("Protracker module loading failed. (Possible cause: file not found)");
    }

    Clear_Input();
}
#endif // __WINAMP__
