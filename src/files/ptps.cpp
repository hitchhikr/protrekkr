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
#include "include/ptps.h"

// ------------------------------------------------------
// Variables
FILE *Out_constants;
FILE *Out_FX;
int Rec_Fx = 0;
LPSYNC_FX Sync_Fx;
unsigned char Old_pSequence[MAX_SEQUENCES];
int Nbr_Muted_Tracks;
int Muted_Tracks[MAX_TRACKS];
int Nbr_Used_Instr;
extern EQSTATE EqDat[MAX_TRACKS];
INSTR_ORDER Used_Instr[MAX_INSTRS];
INSTR_ORDER Used_Instr2[MAX_INSTRS];
INSTR_ORDER Used_Instr3[MAX_INSTRS];

// ------------------------------------------------------
// Save a constant into a properties include file
void Save_Constant(char *Name, int value)
{
    if(Out_constants)
    {
        if(value) fprintf(Out_constants, "#define %s\n", Name);
        else fprintf(Out_constants, "// #define %s\n", Name);
    }
}

// ------------------------------------------------------
// Look for a fx already recorded for that pos/row
int Check_FX(int pos, int row)
{
    int i;

    for(i = 0; i < Rec_Fx; i++)
    {
        if(pos == Sync_Fx[i].Pos &&
           row == Sync_Fx[i].Row)
        {
            return(i);
        }
    }
    return(-1);
}

// ------------------------------------------------------
// Save a synchro marker
void Save_FX(int pos, int row, int data)
{
    if(Out_FX)
    {
        fwrite(&pos, 1, 1, Out_FX);
        fwrite(&row, 1, 1, Out_FX);
        fwrite(&data, 1, 1, Out_FX);
    }
}

// ------------------------------------------------------
// Look for a sequence index
int Search_Sequence(int sequence_idx)
{
    int i;

    for(i = 0; i < MAX_SEQUENCES; i++)
    {
        if(Old_pSequence[i] == sequence_idx)
        {
            return(i);
        }
    }
    return(-1);
}

// ------------------------------------------------------
// Return TRUE if a given track is muted
int Track_Is_Muted(int track)
{
    int i;

    for(i = 0; i < Nbr_Muted_Tracks; i++)
    {
        if(Muted_Tracks[i] == track) return(TRUE);
    }
    return(FALSE);
}

// ------------------------------------------------------
// Return the new index of an instrument for an old one
int Get_Instr_New_Order(int instr)
{
    int i;

    for(i = 0; i < MAX_INSTRS; i++)
    {
        if(Used_Instr[i].old_order == instr) return(Used_Instr[i].new_order);
    }
    return(-1);
}

// ------------------------------------------------------
// Check if an index is within range of channel's variable data
int Check_Range(int Idx, int Bound, int Start)
{
    int i;
    int Val = Start;

    for(i = 0; i < Bound; i++)
    {
        if(Idx == Val)
        {
            return TRUE;
        }
        Val += 2;
    }
    return FALSE;
}

// ------------------------------------------------------
// Save a packed (.ptp) module
// (Only the samples are actually (if requested) packed,
//  the rest is just "demangled" to ease packers compression ratio).
int Save_Ptp(FILE *in, int Simulate, char *FileName)
{
    unsigned char *TmpPatterns;
    unsigned char *TmpPatterns_Tracks;
    unsigned char *TmpPatterns_Notes;
    unsigned char *TmpPatterns_Rows;
    unsigned int New_Extension = 'KTRP';
    char char_value;
    char FileName_FX[MAX_PATH];
    int i;
    int j;
    int k;
    int l;
    int twrite;
    int tps_trk;
    char Store_303_1 = FALSE;
    char Store_303_2 = FALSE;

    int Store_Gsm = FALSE;
    int Store_Mp3 = FALSE;
    int Store_ADPCM = FALSE;
    int Store_At3 = FALSE;
    int Store_8Bit = FALSE;
    int Store_WavPack = FALSE;

    int Store_Track_Volume = FALSE;
    int Store_Track_Eq = FALSE;

    int Store_Flanger = FALSE;
    int Store_Disclap = FALSE;
    int Store_LFO = FALSE;
    int Store_Instr_Waveform_Osc_1 = FALSE; 
    int Store_Instr_Waveform_Osc_2 = FALSE;
    int Store_Synth_Disto = FALSE;
    int Store_Synth_Osc_2 = FALSE;
    int Store_Synth_Osc_3 = FALSE;

    int Store_Synth_Phase_Osc_1 = FALSE;
    int Store_Synth_Phase_Osc_2 = FALSE;

    int Store_Synth_Lfo_1 = FALSE;
    int Store_Synth_Lfo_2 = FALSE;
    int Store_Synth_Lfo_1_Pitch = FALSE;
    int Store_Synth_Lfo_2_Pitch = FALSE;
    
    int Store_Synth_Env_1 = FALSE;
    int Store_Synth_Env_2 = FALSE;
    int Store_Synth_Env_1_Pitch = FALSE;
    int Store_Synth_Env_2_Pitch = FALSE;
    
    int Store_Instruments = FALSE;
    int Store_Loop_Forward = FALSE;
    int Store_Loop_PingPong = FALSE;

    int Store_Volume_Column = FALSE;
    int Store_FX_NoteCut = FALSE;
    int Store_FX_PitchUp = FALSE;
    int Store_FX_PitchDown = FALSE;
    int Store_FX_SetVolume = FALSE;
    int Store_FX_TranceSlicer = FALSE;
    int Store_FX_TranceGlider = FALSE;
    int Store_FX_PatternLoop = FALSE;
    int Store_FX_SetCutOff = FALSE;
    int Store_FX_SetSampleOffset = FALSE;
    int Store_FX_SetRandomCutOff = FALSE;
    int Store_FX_SlideUpCutOff = FALSE;
    int Store_FX_SlideDownCutOff = FALSE;
    int Store_FX_PatternBreak = FALSE;
    int Store_FX_NoteRetrigger = FALSE;
    int Store_FX_SetBPM = FALSE;
    int Store_FX_SetSpeed = FALSE;
    int Store_FX_SendToDelayCommand = FALSE;
    int Store_FX_SendToReverbCommand = FALSE;
    int Store_FX_SetDistortionThreshold = FALSE;
    int Store_FX_SetDistortionClamp = FALSE;
    int Store_FX_SetFilterResonance = FALSE;
    int Store_FX_SetFilterType = FALSE;
    int Store_FX_ResetFilterLfo = FALSE;
    
    int Store_FX_SetLfoRate = FALSE;
    int Store_FX_SetLfoScale = FALSE;
    int Store_FX_SetFilterLfo = FALSE;
    int Store_FX_SetVolumeLfo = FALSE;
    int Store_FX_SetPanningLfo = FALSE;
    
    int Store_FX_SetLfoFrequency= FALSE;
    int Store_FX_AutoFadeIn = FALSE;
    int Store_FX_AutoFadeOut = FALSE;
    int Store_FX_VolumeSlideUp = FALSE;
    int Store_FX_VolumeSlideDown = FALSE;
    int Store_FX_SetGlobalVolume = FALSE;
    int Store_FX_Arpeggio = FALSE;
    int Store_FX_Vibrato = FALSE;
    int Store_FX_Reverse = FALSE;
    int Store_FX_PosJump = FALSE;
    int Store_FX_FineVolumeSlideUp = FALSE;
    int Store_FX_FineVolumeSlideDown = FALSE;
    int Store_FX_FinePitchUp = FALSE;
    int Store_FX_FinePitchDown = FALSE;
    int Store_FX_SwitchFlanger = FALSE;
    int Store_FX_SwitchTrackLFO = FALSE;
    int Store_FX_SwitchTrackCompression = FALSE;
    int Store_FX_Shuffle = FALSE;
    int Store_FX_RevCuto = FALSE;
    int Store_FX_RevReso = FALSE;
    int Store_FX_RevDamp = FALSE;

    int Store_Synth = FALSE;

    int Store_TrackFilters = FALSE;

    int Store_Filter_LoHiBand = FALSE;
    int Store_Filter_Lo24 = FALSE;
    int Store_Filter_Lo48 = FALSE;
    int Store_Filter_Lp24 = FALSE;
    int Store_Filter_AModM = FALSE;
    int Store_Filter_AModS = FALSE;
    int Store_Filter_SingleM = FALSE;
    int Store_Filter_SingleS = FALSE;
    int Store_Filter_Eqm15 = FALSE;
    int Store_Filter_Eqm6 = FALSE;
    int Store_Filter_Eqp6 = FALSE;
    int Store_Filter_Eqp15 = FALSE;
    int Store_Filter_Delta = FALSE;
    int Store_Filter_DistL = FALSE;
    int Store_Filter_DistM = FALSE;
    int Store_Filter_DistH = FALSE;
    int Store_Filter_Dist = FALSE;
    int Store_Filter_Hp12M = FALSE;
    int Store_Filter_Hp12S = FALSE;
    int Store_Filter_Hp24M = FALSE;

    int Store_Synth_Filter = FALSE;
    int Store_Synth_Filter_Lo = FALSE;
    int Store_Synth_Filter_Hi = FALSE;
    int Store_Synth_Filter_Moog_Lo = FALSE;
    int Store_Synth_Filter_Moog_Band = FALSE;
    int Store_Synth_Sin = FALSE;
    int Store_Synth_Saw = FALSE;
    int Store_Synth_Pulse = FALSE;
    int Store_Synth_WhiteNoise = FALSE;
    int Store_Synth_Tri = FALSE;

    int Empty_Fx = FALSE;
    int Number_Fx = 0;
    int Empty_Var = 0;

    unsigned char New_pSequence[256];
    unsigned short New_patternLines[256];
    unsigned char *New_RawPatterns;
    unsigned char done_pattern[256];
    int int_pattern;
    int Real_Song_Tracks;
    int pwrite;
    int swrite;
    char Constant_Filename[MAX_PATH];
    char Comp_Flag_Main = FALSE;
    char Comp_Flag_Tracks = FALSE;

    Out_constants = NULL;
    Out_FX = NULL;
    wait_AutoSave = 0;

    if(!Simulate)
    {
        sprintf(Constant_Filename, "%s" SLASH "ptk_properties.h", Dir_Mods);
        Out_constants = fopen(Constant_Filename, "w");
        fprintf(Out_constants, "// Constants for %s.ptp module\n", FileName);
        fprintf(Out_constants, "// Generated for %s replay routine\n", VERSION);
        fprintf(Out_constants, "// Use this file to compile a custom PtkReplay library\n\n", VERSION);
    }

    New_RawPatterns = (unsigned char *) malloc(PATTERN_POOL_SIZE);
    if(!New_RawPatterns) return(FALSE);

    // Writing header & name...
    Write_Mod_Data(&New_Extension, sizeof(char), 4, in);

    // Re-arrange the patterns sequence
    int_pattern = 0;
    memset(done_pattern, 0, sizeof(done_pattern));
    memset(Old_pSequence, -1, sizeof(Old_pSequence));

    for(i = 0; i < Song_Length; i++)
    {
        if(!done_pattern[pSequence[i]])
        {
            memcpy(New_RawPatterns + (int_pattern * PATTERN_LEN),
                   RawPatterns + (pSequence[i] * PATTERN_LEN),
                   PATTERN_LEN);
            New_patternLines[int_pattern] = patternLines[pSequence[i]];
            Old_pSequence[i] = pSequence[i];
            New_pSequence[i] = int_pattern;
        }
        else
        {
            // Pattern is already known
            New_pSequence[i] = New_pSequence[Search_Sequence(pSequence[i])];
        }
        if(!done_pattern[pSequence[i]])
        {
            int_pattern++;
            done_pattern[pSequence[i]] = TRUE;
        }
    }

    Nbr_Muted_Tracks = 0;
    for(i = 0; i < MAX_TRACKS; i++)
    {
        Muted_Tracks[i] = -1;
    }

    // Check which tracks are muted
    for(tps_trk = 0; tps_trk < Song_Tracks; tps_trk++)
    {
        if(Chan_Mute_State[tps_trk])
        {
            Muted_Tracks[Nbr_Muted_Tracks] = tps_trk;
            Nbr_Muted_Tracks++;
        }
    }
    Real_Song_Tracks = Song_Tracks - Nbr_Muted_Tracks;

    char_value = (char) int_pattern;
    Write_Mod_Data(&char_value, sizeof(char), 1, in);
    
    // Number of tracks is stored here in .ptp format
    char_value = (char) Real_Song_Tracks;
    Write_Mod_Data(&char_value, sizeof(char), 1, in);

    Write_Mod_Data(&Song_Length, sizeof(char), 1, in);
    Write_Mod_Data(&Use_Cubic, sizeof(char), 1, in);

    // Patterns sequence
    Write_Mod_Data(New_pSequence, sizeof(char), Song_Length, in);

    for(i = 0; i < int_pattern; i++)
    {
        char_value = (char) New_patternLines[i];
        Write_Mod_Data(&char_value, sizeof(char), 1, in);
    }

    char_value = (char) Real_Song_Tracks;
    Write_Mod_Data(Channels_MultiNotes, sizeof(char), char_value, in);

    Write_Mod_Data(Channels_Effects, sizeof(char), char_value, in);

    for(i = 0; i < char_value; i++)
    {
        if(Track_Volume[i] <= 0.99f)
        {
            Store_Track_Volume = TRUE;
        }
        Write_Mod_Data(&Track_Volume[i], sizeof(float), 1, in);
    }

    for(i = 0; i < char_value; i++)
    {
        Write_Mod_Data(&Track_Surround[i], sizeof(char), 1, in);
    }

    for(i = 0; i < char_value; i++)
    {
        if(EqDat[i].lg != 1.0f ||
           EqDat[i].mg != 1.0f ||
           EqDat[i].hg != 1.0f
          )
        {
            Store_Track_Eq = TRUE;
        }
        Write_Mod_Data(&EqDat[i].lg, sizeof(float), 1, in);
        Write_Mod_Data(&EqDat[i].mg, sizeof(float), 1, in);
        Write_Mod_Data(&EqDat[i].hg, sizeof(float), 1, in);
    }

    // Check the instruments
    Nbr_Used_Instr = 0;
    TmpPatterns = New_RawPatterns;
    for(i = 0; i < MAX_INSTRS; i++)
    {
        Used_Instr[i].new_order = -1;
        Used_Instr[i].old_order = -1;
        Used_Instr2[i].old_order = -1;
    }
    for(pwrite = 0; pwrite < int_pattern; pwrite++)
    {
        TmpPatterns_Rows = TmpPatterns + (pwrite * PATTERN_LEN);
        for(i = 0; i < PATTERN_BYTES; i++)
        {   // Data
            for(k = 0; k < Song_Tracks; k++)
            {   // Tracks
                if(!Track_Is_Muted(k))
                {
                    TmpPatterns_Tracks = TmpPatterns_Rows + (k * PATTERN_BYTES);
                    for(j = 0; j < New_patternLines[pwrite]; j++)
                    {   // Rows
                        TmpPatterns_Notes = TmpPatterns_Tracks + (j * PATTERN_ROW_LEN);
                        // Store the used instrument
                        switch(i)
                        {
                            case  PATTERN_INSTR1:
                            case  PATTERN_INSTR2:
                            case  PATTERN_INSTR3:
                            case  PATTERN_INSTR4:
                            case  PATTERN_INSTR5:
                            case  PATTERN_INSTR6:
                            case  PATTERN_INSTR7:
                            case  PATTERN_INSTR8:
                            case  PATTERN_INSTR9:
                            case  PATTERN_INSTR10:
                            case  PATTERN_INSTR11:
                            case  PATTERN_INSTR12:
                            case  PATTERN_INSTR13:
                            case  PATTERN_INSTR14:
                            case  PATTERN_INSTR15:
                            case  PATTERN_INSTR16:
                                if(TmpPatterns_Notes[i] < MAX_INSTRS)
                                {
                                    if(Used_Instr[TmpPatterns_Notes[i]].new_order == -1)
                                    {
                                        Used_Instr[TmpPatterns_Notes[i]].new_order = Nbr_Used_Instr;
                                        Used_Instr[TmpPatterns_Notes[i]].old_order = TmpPatterns_Notes[i];
                                        Used_Instr2[Nbr_Used_Instr].old_order = TmpPatterns_Notes[i];
                                        Nbr_Used_Instr++;
                                    }
                                }
                                break;
                            
                            case PATTERN_FX:
                            case PATTERN_FX2:
                            case PATTERN_FX3:
                            case PATTERN_FX4:
                                // Count the number of synchro fxs
                                if(TmpPatterns_Notes[i] == 0x7)
                                {
                                    Number_Fx++;
                                }
                        }
                    }
                }
            }
        }
    }

    // Add the prgsynth instruments
    int synth_instr_remap;
    for(i = 0; i < MAX_INSTRS; i++)
    {
        switch(Synthprg[i])
        {
            case SYNTH_WAVE_OFF:
                break;
            case SYNTH_WAVE_CURRENT:
                Store_Synth = TRUE;
                synth_instr_remap = i;
                break;
            default:
                Store_Synth = TRUE;
                synth_instr_remap = Synthprg[i] - 2;
                break;
        }
        if(Synthprg[i] != SYNTH_WAVE_OFF)
        {
            if(Used_Instr[synth_instr_remap].new_order == -1)
            {
                Used_Instr[synth_instr_remap].new_order = Nbr_Used_Instr;
                Used_Instr[synth_instr_remap].old_order = synth_instr_remap;
                Used_Instr2[Nbr_Used_Instr].old_order = synth_instr_remap;
                Nbr_Used_Instr++;
            }
        }
    }

    if(!Simulate)
    {
        // Create the sync fx file data
        Sync_Fx = NULL;
        if(Number_Fx)
        {
            Rec_Fx = 0;
            sprintf(FileName_FX, "%s.psm", FileName);
            Out_FX = fopen(FileName_FX, "wb");

            // Save the FX data
            for(l = 0; l < Song_Length; l++)
            {
                TmpPatterns_Rows = New_RawPatterns + (New_pSequence[l] * PATTERN_LEN);
                for(i = 0; i < PATTERN_BYTES; i++)
                {   // Data
                    for(k = 0; k < Song_Tracks; k++)
                    {   // Tracks
                        if(!Track_Is_Muted(k))
                        {
                            TmpPatterns_Tracks = TmpPatterns_Rows + (k * PATTERN_BYTES);
                            for(j = 0; j < New_patternLines[New_pSequence[l]]; j++)
                            {   // Rows
                                TmpPatterns_Notes = TmpPatterns_Tracks + (j * PATTERN_ROW_LEN);
                                if(i == PATTERN_FX ||
                                   i == PATTERN_FX2 ||
                                   i == PATTERN_FX3 ||
                                   i == PATTERN_FX4
                                  )
                                {
                                    // Don't save FX 7
                                    if(TmpPatterns_Notes[i] == 0x7)
                                    {
                                        Rec_Fx++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Sync_Fx = (LPSYNC_FX) malloc(Rec_Fx * sizeof(SYNC_FX));
            if(Sync_Fx)
            {
                memset(Sync_Fx, -1, Rec_Fx * sizeof(SYNC_FX));
                LPSYNC_FX TmpSync_Fx = Sync_Fx;
                int cur_fx_nbr;
                int real_fx_nbr = 0;

                // Save the FX data
                for(l = 0; l < Song_Length; l++)
                {
                    TmpPatterns_Rows = New_RawPatterns + (New_pSequence[l] * PATTERN_LEN);
                    for(i = 0; i < PATTERN_BYTES; i++)
                    {   // Data
                        for(k = 0; k < Song_Tracks; k++)
                        {   // Tracks
                            if(!Track_Is_Muted(k))
                            {
                                TmpPatterns_Tracks = TmpPatterns_Rows + (k * PATTERN_BYTES);
                                for(j = 0; j < New_patternLines[New_pSequence[l]]; j++)
                                {   // Rows
                                    TmpPatterns_Notes = TmpPatterns_Tracks + (j * PATTERN_ROW_LEN);
                                    if(i == PATTERN_FX ||
                                       i == PATTERN_FX2 ||
                                       i == PATTERN_FX3 ||
                                       i == PATTERN_FX4
                                      )
                                    {
                                        // Don't save FX 7
                                        if(TmpPatterns_Notes[i] == 0x7)
                                        {
                                            // Discard multiple fxs / line
                                            cur_fx_nbr = Check_FX(l, j);
                                            if(cur_fx_nbr == -1)
                                            {
                                                TmpSync_Fx->Pos = l;
                                                TmpSync_Fx->Row = j;
                                                TmpSync_Fx->Data = TmpPatterns_Notes[i + 1];
                                                TmpSync_Fx++;
                                                real_fx_nbr++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Write_Data(&real_fx_nbr, sizeof(short), 1, Out_FX);
                for(i = 0; i < real_fx_nbr; i++)
                {
                    Save_FX(Sync_Fx[i].Pos, Sync_Fx[i].Row, Sync_Fx[i].Data);
                }
                free(Sync_Fx);
            }
        }
    }

    // Look for the used fx
    TmpPatterns = New_RawPatterns;
    for(pwrite = 0; pwrite < int_pattern; pwrite++)
    {
        TmpPatterns_Rows = TmpPatterns + (pwrite * PATTERN_LEN);
        for(i = 0; i < PATTERN_BYTES; i++)
        {   // Data
            for(k = 0; k < Song_Tracks; k++)
            {   // Tracks
                if(!Track_Is_Muted(k))
                {
                    TmpPatterns_Tracks = TmpPatterns_Rows + (k * PATTERN_BYTES);
                    for(j = 0; j < New_patternLines[pwrite]; j++)
                    {   // Rows
                        TmpPatterns_Notes = TmpPatterns_Tracks + (j * PATTERN_ROW_LEN);

                        // Check the volume column
                        if(i == PATTERN_VOLUME)
                        {
                            if(TmpPatterns_Notes[i] != 0xff)
                            {
                                if(TmpPatterns_Notes[i] & 0xf0 == 0xf0)
                                {
                                    Store_FX_NoteCut = TRUE;
                                }
                                if(TmpPatterns_Notes[i] <= 64)
                                {
                                    Store_Volume_Column = TRUE;
                                }
                            }
                        }

                        Empty_Fx = FALSE;
                        // Check the effects column
                        if(Check_Range(i, Channels_Effects[k], PATTERN_FX))
                        {
                            switch(TmpPatterns_Notes[i])
                            {
                                // $00 No Effect
                                case 0x00:
                                    Empty_Fx = TRUE;
                                    break;

                                    // $01 Pitch Up
                                case 0x1:
                                    Store_FX_PitchUp = TRUE;
                                    break;

                                // $02 Pitch Down
                                case 0x2:
                                    Store_FX_PitchDown = TRUE;
                                    break;

                                // $03 Set volume
                                case 0x3:
                                    Store_FX_SetVolume = TRUE;
                                    break;

                                // $04 Trance slicer
                                case 0x4:
                                    Store_FX_TranceSlicer = TRUE;
                                    break;

                                // $05 Glider
                                case 0x5:
                                    Store_FX_TranceGlider = TRUE;
                                    break;

                                // $06 Pattern loop
                                case 0x6:
                                    Store_FX_PatternLoop = TRUE;
                                    break;

                                // $08 SetCutOff
                                case 0x8:
                                    Store_FX_SetCutOff = TRUE;
                                    break;

                                // $09 SetSampleOffset
                                case 0x9:
                                    Store_FX_SetSampleOffset = TRUE;
                                    break;

                                // $0a SetRandomCutOff
                                case 0xa:
                                    Store_FX_SetRandomCutOff = TRUE;
                                    break;

                                // $0b SlideUpCutOff
                                case 0xb:
                                    Store_FX_SlideUpCutOff = TRUE;
                                    break;

                                // $0c SlideDownCutOff
                                case 0xc:
                                    Store_FX_SlideDownCutOff = TRUE;
                                    break;

                                // $0d PatternBreak
                                case 0xd:
                                    Store_FX_PatternBreak = TRUE;
                                    break;

                                // $0e Note retrigger
                                case 0xe:
                                    Store_FX_NoteRetrigger = TRUE;
                                    break;

                                // $f0 Set BPM
                                case 0xf0:
                                    Store_FX_SetBPM = TRUE;
                                    break;

                                // $0f Set speed
                                case 0xf:
                                    Store_FX_SetSpeed = TRUE;
                                    break;

                                // $10 Send to delay Command
                                case 0x10:
                                    Store_FX_SendToDelayCommand = TRUE;
                                    break;

                                // $11 Send to reverb Command
                                case 0x11:
                                    Store_FX_SendToReverbCommand = TRUE;
                                    break;

                                // $12 Set distortion Threshold
                                case 0x12:
                                    Store_FX_SetDistortionThreshold = TRUE;
                                    break;

                                // $13 Set distortion clamp
                                case 0x13:
                                    Store_FX_SetDistortionClamp = TRUE;
                                    break;

                                // $14 Set filter resonance
                                case 0x14:
                                    Store_FX_SetFilterResonance = TRUE;
                                    break;

                                // $15 Set filter Type
                                case 0x15:
                                    Store_FX_SetFilterType = TRUE;
                                    switch(TmpPatterns_Notes[i + 1])
                                    {
                                        case 0:
                                        case 1:
                                        case 2:
                                        case 3:
                                            Store_Filter_LoHiBand = TRUE;
                                            break;
                                        case 4:
                                            break;
                                        case 5:
                                            Store_Filter_Lo24 = TRUE;
                                            break;
                                        case 6:
                                            Store_Filter_Lo48 = TRUE;
                                            break;
                                        case 7:
                                            Store_Filter_Lp24 = TRUE;
                                            break;
                                        case 8:
                                            Store_Filter_AModM = TRUE;
                                            break;
                                        case 9:
                                            Store_Filter_AModS = TRUE;
                                            break;
                                        case 10:
                                            Store_Filter_SingleM = TRUE;
                                            break;
                                        case 11:
                                            Store_Filter_SingleS = TRUE;
                                            break;
                                        case 12:
                                            Store_Filter_Eqm15 = TRUE;
                                            break;
                                        case 13:
                                            Store_Filter_Eqm6 = TRUE;
                                            break;
                                        case 14:
                                            Store_Filter_Eqp6 = TRUE;
                                            break;
                                        case 15:
                                            Store_Filter_Eqp15 = TRUE;
                                            break;
                                        case 16:
                                            Store_Filter_Delta = TRUE;
                                            break;
                                        case 17:
                                            Store_Filter_DistL = TRUE;
                                            break;
                                        case 18:
                                            Store_Filter_DistM = TRUE;
                                            break;
                                        case 19:
                                            Store_Filter_DistH = TRUE;
                                            break;
                                        case 20:
                                            Store_Filter_Dist = TRUE;
                                            break;
                                        case 21:
                                            Store_Filter_Hp12M = TRUE;
                                            break;
                                        case 22:
                                            Store_Filter_Hp12S = TRUE;
                                            break;
                                        case 23:
                                            Store_Filter_Hp24M = TRUE;
                                            break;
                                    }
                                    break;

                                // $16 Reset channel filter lfo
                                case 0x16:
                                    Store_FX_ResetFilterLfo = TRUE;
                                    break;

                                // $17 Auto fade in xx ticks
                                case 0x17:
                                    Store_FX_AutoFadeIn = TRUE;
                                    break;

                                // $18 Auto fade out xx ticks
                                case 0x18:
                                    Store_FX_AutoFadeOut = TRUE;
                                    break;

                                // $19 Volume slide up
                                case 0x19:
                                    Store_FX_VolumeSlideUp = TRUE;
                                    break;

                                // $1a Volume slide down
                                case 0x1a:
                                    Store_FX_VolumeSlideDown = TRUE;
                                    break;

                                // $1c Arpeggio
                                case 0x1b:
                                    Store_FX_Arpeggio = TRUE;
                                    break;

                                // $1c Set global volume
                                case 0x1c:
                                    Store_FX_SetGlobalVolume = TRUE;
                                    break;

                                // $1d Vibrato
                                case 0x1d:
                                    Store_FX_Vibrato = TRUE;
                                    break;

                                // $1e Reverse playing way
                                case 0x1e:
                                    Store_FX_Reverse = TRUE;
                                    break;

                                // $1f Position jump
                                case 0x1f:
                                    Store_FX_PosJump = TRUE;
                                    break;

                                // $20 Fine volume slide up
                                case 0x20:
                                    Store_FX_FineVolumeSlideUp = TRUE;
                                    break;

                                // $20 Fine volume slide down
                                case 0x21:
                                    Store_FX_FineVolumeSlideDown = TRUE;
                                    break;

                                // $22 Fine pitch up
                                case 0x22:
                                    Store_FX_FinePitchUp = TRUE;
                                    break;

                                // $23 Fine pitch down
                                case 0x23:
                                    Store_FX_FinePitchDown = TRUE;
                                    break;

                                // $24 Switch flanger
                                case 0x24:
                                    Store_FX_SwitchFlanger = TRUE;
                                    break;

                                // $25 Set shuffle
                                case 0x25:
                                    Store_FX_Shuffle = TRUE;
                                    break;

                                // $26 Set reverb cutoff
                                case 0x26:
                                    Store_FX_RevCuto = TRUE;
                                    break;

                                // $27 Set reverb resonance
                                case 0x27:
                                    Store_FX_RevReso = TRUE;
                                    break;

                                // $28 Switch track LFO
                                case 0x28:
                                    Store_FX_SwitchTrackLFO = TRUE;
                                    break;

                                // $29 Switch track compressor
                                case 0x29:
                                    // Check if the compressor is activated
                                    if(TmpPatterns_Notes[i + 1])
                                    {
                                        Comp_Flag_Tracks = TRUE;
                                    }
                                    break;

                                // $2c Set reverb damp
                                case 0x2C:
                                    Store_FX_RevDamp = TRUE;
                                    break;
                                
                                // $43 Set channel lfo frequency value
                                case 0x43:
                                    Store_FX_SetLfoRate = TRUE;
                                    break;

                                // $44 Set channel lfo multiplier value
                                case 0x44:
                                    Store_FX_SetLfoScale = TRUE;
                                    break;

                                // $45 Set channel filter lfo value
                                case 0x45:
                                    Store_FX_SetFilterLfo = TRUE;
                                    break;

                                // $46 Set channel volume lfo value
                                case 0x46:
                                    Store_FX_SetVolumeLfo = TRUE;
                                    break;

                                // $47 Set channel panning lfo value
                                case 0x47:
                                    Store_FX_SetPanningLfo = TRUE;
                                    break;

                                // $31 First TB303 control
                                case 0x31:
                                    Store_303_1 = TRUE;
                                    break;

                                // $32 Second TB303 control
                                case 0x32:
                                    Store_303_2 = TRUE;
                                    break;
                            }
                        }

                        // Is it a legal fx command column ?
                        if(Check_Range(i, Channels_Effects[k], PATTERN_FX))
                        {
                            // Don't save FX 7
                            if(TmpPatterns_Notes[i] == 0x7)
                            {
                                Write_Mod_Data(&Empty_Var, sizeof(char), 1, in);
                            }
                            else
                            {
                                Write_Mod_Data(TmpPatterns_Notes + i, sizeof(char), 1, in);
                            }
                        }
                        else
                        {
                            // Is it a legal fx data column ?
                            if(Check_Range(i, Channels_Effects[k], PATTERN_FXDATA))
                            {
                                // Don't save Fx 7 data or non-zero data for empty an effect
                                if(TmpPatterns_Notes[i - 1] == 0x7 || Empty_Fx)
                                {
                                    Write_Mod_Data(&Empty_Var, sizeof(char), 1, in);
                                }
                                else
                                {
                                    Write_Mod_Data(TmpPatterns_Notes + i, sizeof(char), 1, in);
                                }
                            }
                            else
                            {
                                // Is it a legal instrument column ?
                                if(Check_Range(i, Channels_MultiNotes[k], PATTERN_INSTR1))
                                {
                                    // Replace the instrument order
                                    if(TmpPatterns_Notes[i] < MAX_INSTRS)
                                    {
                                        TmpPatterns_Notes[i] = Get_Instr_New_Order(TmpPatterns_Notes[i]);
                                    }
                                    Write_Mod_Data(TmpPatterns_Notes + i, sizeof(char), 1, in);
                                }
                                else
                                {
                                    // Is it a legal note column ?
                                    if(Check_Range(i, Channels_MultiNotes[k], PATTERN_NOTE1))
                                    {
                                        Write_Mod_Data(TmpPatterns_Notes + i, sizeof(char), 1, in);
                                    }
                                    else
                                    {
                                        // We reach this point if we're not in the range of notes or
                                        // fx for this track
                                        switch(i)
                                        {
                                            case PATTERN_VOLUME:
                                            case PATTERN_PANNING:
                                                Write_Mod_Data(TmpPatterns_Notes + i, sizeof(char), 1, in);
                                                break;
                                            default:
                                                Write_Mod_Data(&Empty_Var, sizeof(char), 1, in);
                                                break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    switch(Use_Cubic)
    {
        case CUBIC_INT:
            Save_Constant("PTK_USE_CUBIC", TRUE);
            Save_Constant("PTK_USE_SPLINE", FALSE);
            break;
        case SPLINE_INT:
            Save_Constant("PTK_USE_CUBIC", FALSE);
            Save_Constant("PTK_USE_SPLINE", TRUE);
            break;
        default:
            Save_Constant("PTK_USE_CUBIC", FALSE);
            Save_Constant("PTK_USE_SPLINE", FALSE);
            break;
    }

    Save_Constant("PTK_303", Store_303_1 | Store_303_2);

    Save_Constant("PTK_FX_0", Store_FX_PitchUp | Store_FX_PitchDown |
                              Store_FX_TranceSlicer |
                              Store_FX_TranceGlider |
                              Store_FX_ResetFilterLfo |
                              Store_FX_SetLfoRate |
                              Store_FX_SetLfoScale |
                              Store_FX_SetFilterLfo |
                              Store_FX_SetVolumeLfo |
                              Store_FX_SetPanningLfo
                              );

    Save_Constant("PTK_FX_X", Store_FX_SetCutOff |
                              Store_FX_SetRandomCutOff |
                              Store_FX_SlideUpCutOff |
                              Store_FX_SlideDownCutOff |
                              Store_FX_NoteRetrigger |
                              Store_FX_SetBPM |
                              Store_FX_SetSpeed |
                              Store_FX_SendToDelayCommand |
                              Store_FX_SendToReverbCommand |
                              Store_FX_SetDistortionThreshold |
                              Store_FX_SetDistortionClamp |
                              Store_FX_SetFilterResonance |
                              Store_FX_SetFilterType |
                              Store_FX_AutoFadeIn |
                              Store_FX_AutoFadeOut |
                              Store_FX_VolumeSlideUp |
                              Store_FX_VolumeSlideDown |
                              Store_FX_SetGlobalVolume
                             );

    Save_Constant("PTK_VOLUME_COLUMN", Store_Volume_Column);

    Save_Constant("PTK_FX_NOTECUT", Store_FX_NoteCut);
    Save_Constant("PTK_FX_PITCHUP", Store_FX_PitchUp);
    Save_Constant("PTK_FX_PITCHDOWN", Store_FX_PitchDown);
    Save_Constant("PTK_FX_SETVOLUME", Store_FX_SetVolume);
    Save_Constant("PTK_FX_TRANCESLICER", Store_FX_TranceSlicer);
    Save_Constant("PTK_FX_TRANCEGLIDER", Store_FX_TranceGlider);

    Save_Constant("PTK_FX_PATTERNLOOP", Store_FX_PatternLoop);
    Save_Constant("PTK_FX_SETCUTOFF", Store_FX_SetCutOff);
    Save_Constant("PTK_FX_SETSAMPLEOFFSET", Store_FX_SetSampleOffset);
    Save_Constant("PTK_FX_SETRANDOMCUTOFF", Store_FX_SetRandomCutOff);
    Save_Constant("PTK_FX_SLIDEUPCUTOFF", Store_FX_SlideUpCutOff);
    Save_Constant("PTK_FX_SLIDEDOWNCUTOFF", Store_FX_SlideDownCutOff);
    Save_Constant("PTK_FX_PATTERNBREAK", Store_FX_PatternBreak);
    Save_Constant("PTK_FX_NOTERETRIGGER", Store_FX_NoteRetrigger);
    Save_Constant("PTK_FX_SETBPM", Store_FX_SetBPM);
    Save_Constant("PTK_FX_SETSPEED", Store_FX_SetSpeed);
    Save_Constant("PTK_FX_SENDTODELAYCOMMAND", Store_FX_SendToDelayCommand);
    Save_Constant("PTK_FX_SENDTOREVERBCOMMAND", Store_FX_SendToReverbCommand);
    Save_Constant("PTK_FX_SETDISTORTIONTHRESHOLD", Store_FX_SetDistortionThreshold);
    Save_Constant("PTK_FX_SETDISTORTIONCLAMP", Store_FX_SetDistortionClamp);
    Save_Constant("PTK_FX_SETFILTERRESONANCE", Store_FX_SetFilterResonance);
    Save_Constant("PTK_FX_SETFILTERTYPE", Store_FX_SetFilterType);
    Save_Constant("PTK_FX_RESETFILTERLFO", Store_FX_ResetFilterLfo);

    Save_Constant("PTK_FX_SETLFORATE", Store_FX_SetLfoRate);
    Save_Constant("PTK_FX_SETLFOSCALE", Store_FX_SetLfoScale);
    Save_Constant("PTK_FX_SETFILTERLFO", Store_FX_SetFilterLfo);
    Save_Constant("PTK_FX_SETVOLUMELFO", Store_FX_SetVolumeLfo);
    Save_Constant("PTK_FX_SETPANNINGLFO", Store_FX_SetPanningLfo);
    
    Save_Constant("PTK_FX_AUTOFADEIN", Store_FX_AutoFadeIn);
    Save_Constant("PTK_FX_AUTOFADEOUT", Store_FX_AutoFadeOut);
    Save_Constant("PTK_FX_AUTOFADEMODE", Store_FX_AutoFadeIn | Store_FX_AutoFadeOut);
    Save_Constant("PTK_FX_VOLUMESLIDEUP", Store_FX_VolumeSlideUp);
    Save_Constant("PTK_FX_VOLUMESLIDEDOWN", Store_FX_VolumeSlideDown);
    Save_Constant("PTK_FX_SETGLOBALVOLUME", Store_FX_SetGlobalVolume);
    Save_Constant("PTK_FX_ARPEGGIO", Store_FX_Arpeggio);
    Save_Constant("PTK_FX_VIBRATO", Store_FX_Vibrato);
    Save_Constant("PTK_FX_REVERSE", Store_FX_Reverse);
    Save_Constant("PTK_FX_POSJUMP", Store_FX_PosJump);
    Save_Constant("PTK_FX_FINEVOLUMESLIDEUP", Store_FX_FineVolumeSlideUp);
    Save_Constant("PTK_FX_FINEVOLUMESLIDEDOWN", Store_FX_FineVolumeSlideDown);
    Save_Constant("PTK_FX_FINEPITCHUP", Store_FX_FinePitchUp);
    Save_Constant("PTK_FX_FINEPITCHDOWN", Store_FX_FinePitchDown);
    Save_Constant("PTK_FX_SWITCHFLANGER", Store_FX_SwitchFlanger);
    Save_Constant("PTK_FX_TRACK_FILTER_LFO", Store_FX_SwitchTrackLFO);

    Save_Constant("PTK_FX_SETREVCUTO", Store_FX_RevCuto);
    Save_Constant("PTK_FX_SETREVRESO", Store_FX_RevReso);
    Save_Constant("PTK_FX_SETREVDAMP", Store_FX_RevDamp);

    // Special but only at tick 0
    Save_Constant("PTK_FX_TICK0", Store_FX_Vibrato | Store_FX_Arpeggio |
                                  Store_FX_PatternLoop | Store_FX_Reverse |
                                  Store_FX_RevCuto | Store_FX_RevReso | Store_FX_RevDamp);

    // Remap the used instruments
    for(i = 0; i < MAX_INSTRS; i++)
    {
        switch(Synthprg[i])
        {
            case SYNTH_WAVE_OFF:
            case SYNTH_WAVE_CURRENT:
                break;

            default:
                Synthprg[i] = Get_Instr_New_Order(Synthprg[i] - 2) + 2;
                break;
        }
    }

    Write_Mod_Data(&Nbr_Used_Instr, sizeof(int), 1, in);

    // Writing sample data
    for(i = 0; i < MAX_INSTRS; i++)
    {
        // Check if it was used at pattern level
        swrite = Used_Instr2[i].old_order;
        if(swrite != -1)
        {
            Write_Mod_Data(&Synthprg[swrite], sizeof(char), 1, in);

            Write_Mod_Data(&Beat_Sync[swrite], sizeof(char), 1, in);
            Write_Mod_Data(&Beat_Lines[swrite], sizeof(short), 1, in);
            Write_Mod_Data(&Sample_Vol[swrite], sizeof(float), 1, in);

            if(Synthprg[swrite])
            {
                // Forward as least for synths
                Store_Loop_Forward = TRUE;

                Write_Mod_Data(&PARASynth[swrite].osc_1_waveform, sizeof(char), 1, in);
                Write_Mod_Data(&PARASynth[swrite].osc_2_waveform, sizeof(char), 1, in);
                if(PARASynth[swrite].osc_1_waveform == WAVEFORM_WAV) Store_Instr_Waveform_Osc_1 = TRUE;
                if(PARASynth[swrite].osc_2_waveform == WAVEFORM_WAV) Store_Instr_Waveform_Osc_2 = TRUE;

                if(PARASynth[swrite].osc_1_waveform == WAVEFORM_SIN) Store_Synth_Sin = TRUE;
                if(PARASynth[swrite].osc_2_waveform == WAVEFORM_SIN) Store_Synth_Sin = TRUE;
                if(PARASynth[swrite].osc_1_waveform == WAVEFORM_SAW) Store_Synth_Saw = TRUE;
                if(PARASynth[swrite].osc_2_waveform == WAVEFORM_SAW) Store_Synth_Saw = TRUE;
                if(PARASynth[swrite].osc_1_waveform == WAVEFORM_PULSE) Store_Synth_Pulse = TRUE;
                if(PARASynth[swrite].osc_2_waveform == WAVEFORM_PULSE) Store_Synth_Pulse = TRUE;
                if(PARASynth[swrite].osc_1_waveform == WAVEFORM_WHITE) Store_Synth_WhiteNoise = TRUE;
                if(PARASynth[swrite].osc_2_waveform == WAVEFORM_WHITE) Store_Synth_WhiteNoise = TRUE;
                if(PARASynth[swrite].osc_1_waveform == WAVEFORM_TRI) Store_Synth_Tri = TRUE;
                if(PARASynth[swrite].osc_2_waveform == WAVEFORM_TRI) Store_Synth_Tri = TRUE;

                if(PARASynth[swrite].osc_2_waveform != WAVEFORM_NONE) Store_Synth_Osc_2 = TRUE;

                if(PARASynth[swrite].osc_1_pw != 256) Store_Synth_Phase_Osc_1 = TRUE;
                if(PARASynth[swrite].osc_2_pw != 256) Store_Synth_Phase_Osc_2 = TRUE;

                float fvalue;
                int ivalue;
                int64 i64value;

                fvalue = (float) (PARASynth[swrite].osc_1_pw - 256) / 256.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = (float) (PARASynth[swrite].osc_2_pw - 256) / 256.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = (float) (PARASynth[swrite].osc_2_detune - 64.0f) * 0.0625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
    
                fvalue = (float) PARASynth[swrite].osc_2_finetune * 0.0078125f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = (float) PARASynth[swrite].vcf_cutoff * 0.0078125f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                
                fvalue = (float) PARASynth[swrite].vcf_resonance * 0.0078125f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                Write_Mod_Data(&PARASynth[swrite].vcf_type, sizeof(char), 1, in);
                if(PARASynth[swrite].vcf_type != 2) Store_Synth_Filter = TRUE;
                if(PARASynth[swrite].vcf_type == 0) Store_Synth_Filter_Lo = TRUE;
                if(PARASynth[swrite].vcf_type == 1) Store_Synth_Filter_Hi = TRUE;
                if(PARASynth[swrite].vcf_type == 3) Store_Synth_Filter_Moog_Lo = TRUE;
                if(PARASynth[swrite].vcf_type == 4) Store_Synth_Filter_Moog_Band = TRUE;

                fvalue = ((float) (PARASynth[swrite].env_1_attack + 1)) / 512.0f;
                if(fvalue < 0.1f) fvalue = 0.1f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                
                fvalue = ((float) (PARASynth[swrite].env_1_decay + 1)) / 512.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = (float) PARASynth[swrite].env_1_sustain * 0.0078125f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = ((float) (PARASynth[swrite].env_1_release + 1)) / 512.0f;
                if(fvalue < 0.15f) fvalue = 0.15f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = ((float) (PARASynth[swrite].env_2_attack + 1)) / 512.0f;
                if(fvalue < 0.1f) fvalue = 0.1f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = ((float) (PARASynth[swrite].env_2_decay + 1)) / 512.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = (float) (PARASynth[swrite].env_2_sustain * 0.0078125f);
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = ((float) (PARASynth[swrite].env_2_release + 1)) / 512.0f;
                if(fvalue < 0.15f) fvalue = 0.15f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = (float) (PARASynth[swrite].lfo_1_period * 2) + 1;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                ivalue = (int) (((float) SamplesPerTick * 0.000277f * fvalue));
                Write_Mod_Data(&ivalue, sizeof(int), 1, in);

                fvalue = (float) (PARASynth[swrite].lfo_2_period * 2) + 1;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                ivalue = (int) (((float) SamplesPerTick * 0.000277f * fvalue));
                Write_Mod_Data(&ivalue, sizeof(int), 1, in);

                if(Store_Synth_Phase_Osc_1)
                {
                    if(PARASynth[swrite].lfo_1_osc_1_pw != 64) Store_Synth_Lfo_1 = TRUE;
                }
                if(Store_Synth_Phase_Osc_2)
                {
                    if(PARASynth[swrite].lfo_1_osc_2_pw != 64) Store_Synth_Lfo_1 = TRUE;
                }
                if(PARASynth[swrite].lfo_1_osc_1_pitch != 64) Store_Synth_Lfo_1 = TRUE;
                if(PARASynth[swrite].lfo_1_osc_2_pitch != 64) Store_Synth_Lfo_1 = TRUE;
                if(PARASynth[swrite].lfo_1_osc_1_pitch != 64) Store_Synth_Lfo_1_Pitch = TRUE;
                if(PARASynth[swrite].lfo_1_osc_2_pitch != 64) Store_Synth_Lfo_1_Pitch = TRUE;
                if(PARASynth[swrite].lfo_1_osc_1_volume != 64) Store_Synth_Lfo_1 = TRUE;
                if(PARASynth[swrite].lfo_1_osc_2_volume != 64) Store_Synth_Lfo_1 = TRUE;
                if(PARASynth[swrite].lfo_1_vcf_cutoff != 64) Store_Synth_Lfo_1 = TRUE;
                if(PARASynth[swrite].lfo_1_vcf_resonance != 64) Store_Synth_Lfo_1 = TRUE;
                if(PARASynth[swrite].lfo_1_disto != 64) Store_Synth_Lfo_1 = TRUE;

                fvalue = ((float) PARASynth[swrite].lfo_1_osc_1_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_osc_2_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_osc_1_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_osc_2_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_osc_1_volume - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_osc_2_volume - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_vcf_cutoff - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_vcf_resonance - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_1_disto - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                if(Store_Synth_Phase_Osc_1)
                {
                    if(PARASynth[swrite].lfo_2_osc_1_pw != 64) Store_Synth_Lfo_2 = TRUE;
                }
                if(Store_Synth_Phase_Osc_2)
                {
                    if(PARASynth[swrite].lfo_2_osc_2_pw != 64) Store_Synth_Lfo_2 = TRUE;
                }
                if(PARASynth[swrite].lfo_2_osc_1_pitch != 64) Store_Synth_Lfo_2 = TRUE;
                if(PARASynth[swrite].lfo_2_osc_2_pitch != 64) Store_Synth_Lfo_2 = TRUE;
                if(PARASynth[swrite].lfo_2_osc_1_pitch != 64) Store_Synth_Lfo_2_Pitch = TRUE;
                if(PARASynth[swrite].lfo_2_osc_2_pitch != 64) Store_Synth_Lfo_2_Pitch = TRUE;
                if(PARASynth[swrite].lfo_2_osc_1_volume != 64) Store_Synth_Lfo_2 = TRUE;
                if(PARASynth[swrite].lfo_2_osc_2_volume != 64) Store_Synth_Lfo_2 = TRUE;
                if(PARASynth[swrite].lfo_2_vcf_cutoff != 64) Store_Synth_Lfo_2 = TRUE;
                if(PARASynth[swrite].lfo_2_vcf_resonance != 64) Store_Synth_Lfo_2 = TRUE;
                if(PARASynth[swrite].lfo_2_disto != 64) Store_Synth_Lfo_2 = TRUE;

                fvalue = ((float) PARASynth[swrite].lfo_2_osc_1_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_osc_2_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_osc_1_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_osc_2_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_osc_1_volume - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_osc_2_volume - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_vcf_cutoff - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_vcf_resonance - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].lfo_2_disto - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                if(Store_Synth_Phase_Osc_1)
                {
                    if(PARASynth[swrite].env_1_osc_1_pw != 64) Store_Synth_Env_1 = TRUE;
                }
                if(Store_Synth_Phase_Osc_2)
                {
                    if(PARASynth[swrite].env_1_osc_2_pw != 64) Store_Synth_Env_1 = TRUE;
                }
                if(PARASynth[swrite].env_1_osc_1_pitch != 64) Store_Synth_Env_1 = TRUE;
                if(PARASynth[swrite].env_1_osc_2_pitch != 64) Store_Synth_Env_1 = TRUE;
                if(PARASynth[swrite].env_1_osc_1_pitch != 64) Store_Synth_Env_1_Pitch = TRUE;
                if(PARASynth[swrite].env_1_osc_2_pitch != 64) Store_Synth_Env_1_Pitch = TRUE;
                if(PARASynth[swrite].env_1_osc_1_volume != 127) Store_Synth_Env_1 = TRUE;
                if(PARASynth[swrite].env_1_osc_2_volume != 127) Store_Synth_Env_1 = TRUE;
                if(PARASynth[swrite].env_1_vcf_cutoff != 64) Store_Synth_Env_1 = TRUE;
                if(PARASynth[swrite].env_1_vcf_resonance != 64) Store_Synth_Env_1 = TRUE;
                if(PARASynth[swrite].env_1_disto != 64) Store_Synth_Env_1 = TRUE;

                fvalue = ((float) PARASynth[swrite].env_1_osc_1_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_osc_2_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_osc_1_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_osc_2_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_osc_1_volume - 64) * 0.015625f;
                //if((PARASynth[swrite].env_1_osc_1_volume - 64) == 0) fvalue = 1.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_osc_2_volume - 64) * 0.015625f;
                //if((PARASynth[swrite].env_1_osc_2_volume - 64) == 0) fvalue = 1.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_vcf_cutoff - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_vcf_resonance - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_1_disto - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                if(Store_Synth_Phase_Osc_1)
                {
                    if(PARASynth[swrite].env_2_osc_1_pw != 64) Store_Synth_Env_2 = TRUE;
                }
                if(Store_Synth_Phase_Osc_2)
                {
                    if(PARASynth[swrite].env_2_osc_2_pw != 64) Store_Synth_Env_2 = TRUE;
                }
                if(PARASynth[swrite].env_2_osc_1_pitch != 64) Store_Synth_Env_2 = TRUE;
                if(PARASynth[swrite].env_2_osc_2_pitch != 64) Store_Synth_Env_2 = TRUE;
                if(PARASynth[swrite].env_2_osc_1_pitch != 64) Store_Synth_Env_2_Pitch = TRUE;
                if(PARASynth[swrite].env_2_osc_2_pitch != 64) Store_Synth_Env_2_Pitch = TRUE;
                if(PARASynth[swrite].env_2_osc_1_volume != 127) Store_Synth_Env_2 = TRUE;
                if(PARASynth[swrite].env_2_osc_2_volume != 127) Store_Synth_Env_2 = TRUE;
                if(PARASynth[swrite].env_2_vcf_cutoff != 64) Store_Synth_Env_2 = TRUE;
                if(PARASynth[swrite].env_2_vcf_resonance != 64) Store_Synth_Env_2 = TRUE;
                if(PARASynth[swrite].env_2_disto != 64) Store_Synth_Env_2 = TRUE;

                fvalue = ((float) PARASynth[swrite].env_2_osc_1_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_osc_2_pw - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_osc_1_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_osc_2_pitch - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_osc_1_volume - 64) * 0.015625f;
                //if((PARASynth[swrite].env_2_osc_1_volume - 64) == 0) fvalue = 1.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_osc_2_volume - 64) * 0.015625f;
                //if((PARASynth[swrite].env_2_osc_2_volume - 64) == 0) fvalue = 1.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_vcf_cutoff - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_vcf_resonance - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) PARASynth[swrite].env_2_disto - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = ((float) PARASynth[swrite].osc_3_volume - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                Write_Mod_Data(&PARASynth[swrite].osc_3_switch, sizeof(char), 1, in);
                if(PARASynth[swrite].osc_3_switch) Store_Synth_Osc_3 = TRUE;

                fvalue = ((float) PARASynth[swrite].ptc_glide * (float) PARASynth[swrite].ptc_glide) * 0.0000015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                i64value = (int64) ((double) fvalue * 4294967296.0f);
                Write_Mod_Data(&i64value, sizeof(int64), 1, in);

                fvalue = ((float) PARASynth[swrite].glb_volume) * 0.0078125f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                if(PARASynth[swrite].disto != 64) Store_Synth_Disto = TRUE;

                fvalue = ((float) PARASynth[swrite].disto - 64) * 0.015625f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = ((float) (PARASynth[swrite].lfo_1_attack + 1)) / 512.0f;
                if(fvalue < 0.1f) fvalue = 0.1f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) (PARASynth[swrite].lfo_1_decay + 1)) / 512.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = (float) PARASynth[swrite].lfo_1_sustain * 0.0078125f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) (PARASynth[swrite].lfo_1_release + 1)) / 512.0f;
                if(fvalue < 0.15f) fvalue = 0.15f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                fvalue = ((float) (PARASynth[swrite].lfo_2_attack + 1)) / 512.0f;
                if(fvalue < 0.1f) fvalue = 0.1f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) (PARASynth[swrite].lfo_2_decay + 1)) / 512.0f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = (float) PARASynth[swrite].lfo_2_sustain * 0.0078125f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);
                fvalue = ((float) (PARASynth[swrite].lfo_2_release + 1)) / 512.0f;
                if(fvalue < 0.15f) fvalue = 0.15f;
                Write_Mod_Data(&fvalue, sizeof(float), 1, in);

                Write_Mod_Data(&PARASynth[swrite].osc_combine, sizeof(char), 1, in);
                Write_Mod_Data(&PARASynth[swrite].osc_sync, sizeof(char), 1, in);
                Write_Mod_Data(&PARASynth[swrite].osc_3_interval, sizeof(char), 1, in);
            }

            // Compression type
            Write_Mod_Data(&SampleCompression[swrite], sizeof(char), 1, in);
            switch(SampleCompression[swrite])
            {
                case SMP_PACK_MP3:
                    Write_Mod_Data(&Mp3_BitRate[swrite], sizeof(char), 1, in);
                    break;

                case SMP_PACK_AT3:
                    Write_Mod_Data(&At3_BitRate[swrite], sizeof(char), 1, in);
                    break;
            }

            // 16 splits / instrument
            for(int slwrite = 0; slwrite < MAX_INSTRS_SPLITS; slwrite++)
            {
                Write_Mod_Data(&SampleType[swrite][slwrite], sizeof(char), 1, in);
                if(SampleType[swrite][slwrite])
                {
                    Store_Instruments = TRUE;
                    int Apply_Interpolation = FALSE;

                    // Check if any of the packing scheme has been used
                    switch(SampleCompression[swrite])
                    {
#if defined(__AT3_CODEC__)
                        case SMP_PACK_AT3:
                            Store_At3 = TRUE;
                            Apply_Interpolation = TRUE;
                            break;
#endif
#if defined(__GSM_CODEC__)
                        case SMP_PACK_GSM:
                            Store_Gsm = TRUE;
                            Apply_Interpolation = TRUE;
                            break;
#endif
#if defined(__MP3_CODEC__)
                        case SMP_PACK_MP3:
                            Store_Mp3 = TRUE;
                            Apply_Interpolation = TRUE;
                            break;
#endif
                        case SMP_PACK_ADPCM:
                            Store_ADPCM = TRUE;
                            Apply_Interpolation = TRUE;
                            break;

                        case SMP_PACK_8BIT:
                            Store_8Bit = TRUE;
                            Apply_Interpolation = TRUE;
                            break;

                        case SMP_PACK_WAVPACK:
                            Store_WavPack = TRUE;
                            Apply_Interpolation = TRUE;
                            break;
                    }
                    Uint32 Real_Len = Sample_Length[swrite][slwrite];
                    Uint32 Calc_Len = Sample_Length[swrite][slwrite];
                    Uint32 Loop_Start = LoopStart[swrite][slwrite];
                    Uint32 Loop_End = LoopEnd[swrite][slwrite];
                    Uint32 iSmp;

                    short *Smp_Dats = NULL;

                    if(Apply_Interpolation)
                    {
                        Loop_Start /= 2;
                        Loop_Start *= 2;

                        Loop_End /= 2;
                        Loop_End *= 2;

                        Calc_Len /= 2;
                        Calc_Len *= 2;
                    }

                    // Don't write past the looping point
                    if(LoopType[swrite][slwrite])
                    {
                        Calc_Len = Loop_End + 2;
                    }

                    Write_Mod_Data(&Basenote[swrite][slwrite], sizeof(char), 1, in);

                    Write_Mod_Data(&Loop_Start, sizeof(int), 1, in);
                    Write_Mod_Data(&Loop_End, sizeof(int), 1, in);
                    switch(LoopType[swrite][slwrite])
                    {
                        case SMP_LOOP_FORWARD:
                            Store_Loop_Forward = TRUE;
                            break;
                        case SMP_LOOP_PINGPONG:
                            Store_Loop_PingPong = TRUE;
                            break;
                    }
                    Write_Mod_Data(&LoopType[swrite][slwrite], sizeof(char), 1, in);
                    
                    Write_Mod_Data(&Calc_Len, sizeof(int), 1, in);
                    Write_Mod_Data(&Finetune[swrite][slwrite], sizeof(char), 1, in);
                    Write_Mod_Data(&Sample_Amplify[swrite][slwrite], sizeof(float), 1, in);
                    Write_Mod_Data(&FDecay[swrite][slwrite], sizeof(float), 1, in);

                    if(Apply_Interpolation)
                    {
                        Calc_Len /= 2;
                        Smp_Dats = (short *) malloc(Real_Len * 2 + 8);
                        memset(Smp_Dats, 0, Real_Len * 2 + 8);
                        // Halve the sample
                        short *Sample = Get_WaveForm(swrite, 0, slwrite);
                        for(iSmp = 0; iSmp < Calc_Len; iSmp++)
                        {
                            Smp_Dats[iSmp] = *(Sample + (iSmp * 2));
                        }
                        Pack_Sample(in,
                                    Smp_Dats,
                                    Calc_Len,
                                    SampleCompression[swrite],
                                    SampleCompression[swrite] == SMP_PACK_MP3 ?
                                            Type_Mp3_BitRate[Mp3_BitRate[swrite]] :
                                            Type_At3_BitRate[At3_BitRate[swrite]]
                                   );
                    }
                    else
                    {
                        Pack_Sample(in,
                                    Get_WaveForm(swrite, 0, slwrite),
                                    Calc_Len,
                                    SampleCompression[swrite],
                                    SampleCompression[swrite] == SMP_PACK_MP3 ?
                                            Type_Mp3_BitRate[Mp3_BitRate[swrite]] :
                                            Type_At3_BitRate[At3_BitRate[swrite]]
                                   );
                    }
                    
                    // Stereo mode ?
                    Write_Mod_Data(&Sample_Channels[swrite][slwrite], sizeof(char), 1, in);
                    if(Sample_Channels[swrite][slwrite] == 2)
                    {
                        if(Apply_Interpolation)
                        {
                            // Halve the sample
                            short *Sample = Get_WaveForm(swrite, 1, slwrite);
                            for(iSmp = 0; iSmp < Calc_Len; iSmp++)
                            {
                                Smp_Dats[iSmp] = *(Sample + (iSmp * 2));
                            }
                            Pack_Sample(in,
                                        Smp_Dats,
                                        Calc_Len,
                                        SampleCompression[swrite],
                                        SampleCompression[swrite] == SMP_PACK_MP3 ?
                                                Type_Mp3_BitRate[Mp3_BitRate[swrite]] :
                                                Type_At3_BitRate[At3_BitRate[swrite]]
                                       );
                        }
                        else
                        {
                            Pack_Sample(in,
                                        Get_WaveForm(swrite, 1, slwrite),
                                        Calc_Len,
                                        SampleCompression[swrite],
                                        SampleCompression[swrite] == SMP_PACK_MP3 ?
                                                Type_Mp3_BitRate[Mp3_BitRate[swrite]] :
                                                Type_At3_BitRate[At3_BitRate[swrite]]
                                       );
                        }
                    }
                    free(Smp_Dats);
                }   // Exist Sample
            }
        }
    }

    Save_Constant("PTK_INSTRUMENTS", Store_Instruments);
    Save_Constant("PTK_LOOP_FORWARD", Store_Loop_Forward);
    Save_Constant("PTK_LOOP_PINGPONG", Store_Loop_PingPong);

    Save_Constant("PTK_SYNTH", Store_Synth);

    Save_Constant("PTK_SYNTH_PHASE1", Store_Synth_Phase_Osc_1);
    Save_Constant("PTK_SYNTH_PHASE2", Store_Synth_Phase_Osc_2);

    Save_Constant("PTK_SYNTH_LFO_1", Store_Synth_Lfo_1);
    Save_Constant("PTK_SYNTH_LFO_2", Store_Synth_Lfo_2);

    Save_Constant("PTK_SYNTH_ENV_1", Store_Synth_Env_1);
    Save_Constant("PTK_SYNTH_ENV_2", Store_Synth_Env_2);

    Save_Constant("PTK_SYNTH_LFO_1_PITCH", Store_Synth_Lfo_1_Pitch);
    Save_Constant("PTK_SYNTH_LFO_2_PITCH", Store_Synth_Lfo_2_Pitch);

    Save_Constant("PTK_SYNTH_ENV_1_PITCH", Store_Synth_Env_1_Pitch);
    Save_Constant("PTK_SYNTH_ENV_2_PITCH", Store_Synth_Env_2_Pitch);

    Save_Constant("PTK_SYNTH_PITCH", Store_Synth_Lfo_1_Pitch | Store_Synth_Env_1_Pitch |
                                     Store_Synth_Lfo_2_Pitch | Store_Synth_Env_2_Pitch
                 );

    Save_Constant("PTK_SYNTH_DISTO", Store_Synth_Disto);
    Save_Constant("PTK_SYNTH_OSC_2", Store_Synth_Osc_2);
    Save_Constant("PTK_SYNTH_OSC_3", Store_Synth_Osc_3);
    Save_Constant("PTK_SYNTH_FILTER", Store_Synth_Filter);
    Save_Constant("PTK_SYNTH_FILTER_LO", Store_Synth_Filter_Lo);
    Save_Constant("PTK_SYNTH_FILTER_HI", Store_Synth_Filter_Hi);
    Save_Constant("PTK_SYNTH_FILTER_MOOG_LO", Store_Synth_Filter_Moog_Lo);
    Save_Constant("PTK_SYNTH_FILTER_MOOG_BAND", Store_Synth_Filter_Moog_Band);

    Save_Constant("PTK_SYNTH_SIN", Store_Synth_Sin);
    Save_Constant("PTK_SYNTH_SAW", Store_Synth_Saw);
    Save_Constant("PTK_SYNTH_PULSE", Store_Synth_Pulse);
    Save_Constant("PTK_SYNTH_WHITE", Store_Synth_WhiteNoise);
    Save_Constant("PTK_SYNTH_TRI", Store_Synth_Tri);

    //Save_Constant("PTK_WAVEFORM", Store_Instr_Waveform_Osc_1 | Store_Instr_Waveform_Osc_2);
    //Save_Constant("PTK_WAVEFORM_OSC_1", Store_Instr_Waveform_Osc_1);
    //Save_Constant("PTK_WAVEFORM_OSC_2", Store_Instr_Waveform_Osc_2);

    Save_Constant("PTK_GSM", Store_Gsm);
    Save_Constant("PTK_MP3", Store_Mp3);
    Save_Constant("PTK_ADPCM", Store_ADPCM);
    Save_Constant("PTK_AT3", Store_At3);
    Save_Constant("PTK_8BIT", Store_8Bit);
    Save_Constant("PTK_WAVPACK", Store_WavPack);

    Write_Mod_Data(&compressor, sizeof(char), 1, in);

    for(twrite = 0; twrite < Song_Tracks; twrite++)
    {
        if(!Track_Is_Muted(twrite))
        {
            if(ICut[twrite] > 0.0078125f) ICut[twrite] = 0.0078125f;
            if(ICut[twrite] < 0.00006103515625f) ICut[twrite] = 0.00006103515625f;
            Write_Mod_Data(&TCut[twrite], sizeof(float), 1, in);
            Write_Mod_Data(&ICut[twrite], sizeof(float), 1, in);
            Write_Mod_Data(&TPan[twrite], sizeof(float), 1, in);
            Write_Mod_Data(&FType[twrite], sizeof(int), 1, in);
            // One of them must be != 4
            if(FType[twrite] != 4) Store_TrackFilters = TRUE;

            switch(FType[twrite])
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    Store_Filter_LoHiBand = TRUE;
                    break;
                case 4:
                    break;
                case 5:
                    Store_Filter_Lo24 = TRUE;
                    break;
                case 6:
                    Store_Filter_Lo48 = TRUE;
                    break;
                case 7:
                    Store_Filter_Lp24 = TRUE;
                    break;
                case 8:
                    Store_Filter_AModM = TRUE;
                    break;
                case 9:
                    Store_Filter_AModS = TRUE;
                    break;
                case 10:
                    Store_Filter_SingleM = TRUE;
                    break;
                case 11:
                    Store_Filter_SingleS = TRUE;
                    break;
                case 12:
                    Store_Filter_Eqm15 = TRUE;
                    break;
                case 13:
                    Store_Filter_Eqm6 = TRUE;
                    break;
                case 14:
                    Store_Filter_Eqp6 = TRUE;
                    break;
                case 15:
                    Store_Filter_Eqp15 = TRUE;
                    break;
                case 16:
                    Store_Filter_Delta = TRUE;
                    break;
                case 17:
                    Store_Filter_DistL = TRUE;
                    break;
                case 18:
                    Store_Filter_DistM = TRUE;
                    break;
                case 19:
                    Store_Filter_DistH = TRUE;
                    break;
                case 20:
                    Store_Filter_Dist = TRUE;
                    break;
                case 21:
                    Store_Filter_Hp12M = TRUE;
                    break;
                case 22:
                    Store_Filter_Hp12S = TRUE;
                    break;
                case 23:
                    Store_Filter_Hp24M = TRUE;
                    break;
            }
            Write_Mod_Data(&FRez[twrite], sizeof(int), 1, in);
            Write_Mod_Data(&DThreshold[twrite], sizeof(float), 1, in);
            Write_Mod_Data(&DClamp[twrite], sizeof(float), 1, in);
            if(compressor)
            {
                Write_Mod_Data(&DSend[twrite], sizeof(float), 1, in);
            }
            Write_Mod_Data(&CSend[twrite], sizeof(int), 1, in);
            Write_Mod_Data(&Channels_Polyphony[twrite], sizeof(char), 1, in);
        }
    }

    // Writing mod properties
    Write_Mod_Data(&c_threshold, sizeof(int), 1, in);

    Write_Mod_Data(&Beats_Per_Min, sizeof(int), 1, in);
    Write_Mod_Data(&Ticks_Per_Beat, sizeof(int), 1, in);
    if(mas_vol < 0.01f) mas_vol = 0.01f;
    if(mas_vol > 1.0f) mas_vol = 1.0f;
    Write_Mod_Data(&mas_vol, sizeof(float), 1, in);

    float threshold = mas_comp_threshold_Master;
    float ratio = mas_comp_ratio_Master;

    if(threshold < 0.01f) threshold = 0.01f;
    if(threshold > 100.0f) threshold = 100.0f;
    if(ratio < 0.01f) ratio = 0.01f;
    if(ratio > 100.0f) ratio = 100.0f;

    if(ratio > 0.01f)
    {
        threshold *= 0.001f;
        ratio *= 0.01f;
        Comp_Flag_Main = TRUE;
        Write_Mod_Data(&Comp_Flag_Main, sizeof(char), 1, in);
        Write_Mod_Data(&threshold, sizeof(float), 1, in);
        Write_Mod_Data(&ratio, sizeof(float), 1, in);
        Save_Constant("PTK_LIMITER_MASTER", TRUE);
    }
    else
    {
        Write_Mod_Data(&Comp_Flag_Main, sizeof(char), 1, in);
        Save_Constant("PTK_LIMITER_MASTER", FALSE);
    }

    for(i = 0; i < Song_Tracks; i++)
    {
        // At least 1 track is compressed
        if(Compress_Track[i])
        {
            Comp_Flag_Tracks = TRUE;
            break;
        }
    }
    if(Comp_Flag_Tracks)
    {
        Save_Constant("PTK_LIMITER_TRACKS", TRUE);
    }
    else
    {
        Save_Constant("PTK_LIMITER_TRACKS", FALSE);
    }
    Write_Mod_Data(&Comp_Flag_Tracks, sizeof(char), 1, in);

    if(Comp_Flag_Tracks)
    {
        for(i = 0; i < Song_Tracks; i++)
        {
            if(!Track_Is_Muted(i))
            {
                threshold = mas_comp_threshold_Track[i];
                if(threshold < 0.01f) threshold = 0.01f;
                if(threshold > 100.0f) threshold = 100.0f;
                threshold *= 0.001f;
                Write_Mod_Data(&threshold, sizeof(float), 1, in);
            }
        }
        for(i = 0; i < Song_Tracks; i++)
        {
            if(!Track_Is_Muted(i))
            {
                ratio = mas_comp_ratio_Track[i];
                if(ratio < 0.01f) ratio = 0.01f;
                if(ratio > 100.0f) ratio = 100.0f;
                ratio *= 0.01f;
                Write_Mod_Data(&ratio, sizeof(float), 1, in);
            }
        }
        for(i = 0; i < Song_Tracks; i++)
        {
            if(!Track_Is_Muted(i))
            {
                Write_Mod_Data(&Compress_Track[i], sizeof(char), 1, in);
            }
        }
    }

    Write_Mod_Data(&Feedback, sizeof(float), 1, in);
    
    if(compressor)
    {
        Save_Reverb_Data(Write_Mod_Data, Write_Mod_Data, in);
    }

    Write_Mod_Data(&lchorus_delay, sizeof(int), 1, in);
    Write_Mod_Data(&rchorus_delay, sizeof(int), 1, in);
    Write_Mod_Data(&lchorus_feedback, sizeof(float), 1, in);
    Write_Mod_Data(&rchorus_feedback, sizeof(float), 1, in);

    Write_Mod_Data(&shuffle_amount, sizeof(int), 1, in);

    Save_Constant("PTK_TRACKFILTERS", Store_TrackFilters);

    Save_Constant("PTK_FILTER_LOHIBAND", Store_Filter_LoHiBand);
    Save_Constant("PTK_FILTER_LO24", Store_Filter_Lo24);
    Save_Constant("PTK_FILTER_LO48", Store_Filter_Lo48);
    Save_Constant("PTK_FILTER_LP24", Store_Filter_Lp24);
    Save_Constant("PTK_FILTER_AMODM", Store_Filter_AModM);
    Save_Constant("PTK_FILTER_AMODS", Store_Filter_AModS);
    Save_Constant("PTK_FILTER_SINGLEM", Store_Filter_SingleM);
    Save_Constant("PTK_FILTER_SINGLES", Store_Filter_SingleS);
    Save_Constant("PTK_FILTER_EQM15", Store_Filter_Eqm15);
    Save_Constant("PTK_FILTER_EQM6", Store_Filter_Eqm6);
    Save_Constant("PTK_FILTER_EQP6", Store_Filter_Eqp6);
    Save_Constant("PTK_FILTER_EQP15", Store_Filter_Eqp15);
    Save_Constant("PTK_FILTER_DELTA", Store_Filter_Delta);
    Save_Constant("PTK_FILTER_DISTL", Store_Filter_DistL);
    Save_Constant("PTK_FILTER_DISTM", Store_Filter_DistM);
    Save_Constant("PTK_FILTER_DISTH", Store_Filter_DistH);
    Save_Constant("PTK_FILTER_DIST", Store_Filter_Dist);
    Save_Constant("PTK_FILTER_HP12M", Store_Filter_Hp12M);
    Save_Constant("PTK_FILTER_HP12S", Store_Filter_Hp12S);
    Save_Constant("PTK_FILTER_HP24M", Store_Filter_Hp24M);

    Save_Constant("PTK_PROC_FILTER", Store_Filter_LoHiBand);
    Save_Constant("PTK_PROC_FILTER2P", Store_Filter_Lo24 | Store_Filter_Lo48 | Store_Filter_Lp24);
    Save_Constant("PTK_PROC_FILTER2P24D", Store_Filter_Lo48 | Store_Filter_Lp24);
    Save_Constant("PTK_PROC_FILTERRINGMOD", Store_Filter_AModM | Store_Filter_AModS);
    Save_Constant("PTK_PROC_FILTERRINGMODSTEREO", Store_Filter_AModS);
    Save_Constant("PTK_PROC_FILTERWATER", Store_Filter_SingleM | Store_Filter_SingleS);
    Save_Constant("PTK_PROC_FILTERWATERSTEREO", Store_Filter_SingleS);
    Save_Constant("PTK_PROC_FILTERBELLSHAPED", Store_Filter_Eqm15 | Store_Filter_Eqm6 | Store_Filter_Eqp6 | Store_Filter_Eqp15);
    Save_Constant("PTK_PROC_FILTERDELTA", Store_Filter_Delta);
    Save_Constant("PTK_PROC_FILTERINT2P", Store_Filter_DistL | Store_Filter_DistM | Store_Filter_DistH | Store_Filter_Dist);
    Save_Constant("PTK_PROC_FILTERHP", Store_Filter_Hp12M | Store_Filter_Hp12S | Store_Filter_Hp24M);
    Save_Constant("PTK_PROC_FILTERHP2", Store_Filter_Hp12S | Store_Filter_Hp24M);

    Save_Constant("PTK_SHUFFLE", shuffle_amount != 0 ? TRUE : FALSE || Store_FX_Shuffle);
    
    Save_Constant("PTK_COMPRESSOR", compressor);

    for(int tps_pos = 0; tps_pos < Song_Length; tps_pos++)
    {
        for(tps_trk = 0; tps_trk < Song_Tracks; tps_trk++)
        {
            if(!Track_Is_Muted(tps_trk))
            {
                Write_Mod_Data(&Chan_Active_State[tps_pos][tps_trk], sizeof(char), 1, in);
            }
        }
    }

    for(twrite = 0; twrite < Song_Tracks; twrite++)
    {
        if(!Track_Is_Muted(twrite))
        {
            Write_Mod_Data(&LFO_ON[twrite], sizeof(char), 1, in);
            if(LFO_ON[twrite])
            {
                Store_LFO = TRUE;
                Write_Mod_Data(&LFO_RATE[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&LFO_AMPL_FILTER[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&LFO_AMPL_VOLUME[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&LFO_AMPL_PANNING[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&LFO_RATE_SCALE[twrite], sizeof(float), 1, in);
            }
        }
    }
    Save_Constant("PTK_LFO", Store_LFO);

    for(twrite = 0; twrite < Song_Tracks; twrite++)
    {
        if(!Track_Is_Muted(twrite))
        {
            Write_Mod_Data(&FLANGER_ON[twrite], sizeof(char), 1, in);
            if(FLANGER_ON[twrite])
            {
                Store_Flanger = TRUE;
                Write_Mod_Data(&FLANGER_AMOUNT[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&FLANGER_DEPHASE[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&FLANGER_RATE[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&FLANGER_AMPL[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&FLANGER_FEEDBACK[twrite], sizeof(float), 1, in);
                Write_Mod_Data(&FLANGER_DELAY[twrite], sizeof(int), 1, in);
            }
        }
    }
    Save_Constant("PTK_FLANGER", Store_Flanger);

    for(tps_trk = 0; tps_trk < Song_Tracks; tps_trk++)
    {
        if(!Track_Is_Muted(tps_trk))
        {
            if(Disclap[tps_trk])
            {
                Store_Disclap = TRUE;
            }
        }
    }
    Save_Constant("PTK_DISCLAP", Store_Disclap);

    Save_Constant("PTK_TRACK_VOLUME", Store_Track_Volume);

    Save_Constant("PTK_TRACK_EQ", Store_Track_Eq);

    for(tps_trk = 0; tps_trk < Song_Tracks; tps_trk++)
    {
        if(!Track_Is_Muted(tps_trk))
        {
            Write_Mod_Data(&Disclap[tps_trk], sizeof(char), 1, in);
        }
    }

    Write_Mod_Data(&Reverb_Filter_Cutoff, sizeof(float), 1, in);
    Write_Mod_Data(&Reverb_Filter_Resonance, sizeof(float), 1, in);
    Write_Mod_Data(&Reverb_Stereo_Amount, sizeof(char), 1, in);
    Write_Mod_Data(&Reverb_Damp, sizeof(float), 1, in);

    Write_Mod_Data(&Store_303_1, sizeof(char), 1, in);
    if(Store_303_1)
    {
        Write_Mod_Data(&tb303[0].selectedpattern, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].tune, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].cutoff, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].resonance, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].envmod, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].decay, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].accent, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].waveform, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[0].scale, sizeof(char), 1, in);
        Write_Mod_Data(tb303[0].patternlength, sizeof(char), 32, in);
        Write_Mod_Data(tb303[0].tone, sizeof(char), 32 * 16, in);
        Write_Mod_Data(tb303[0].flag, sizeof(struct flag303), 32 * 16, in);
    }

    Write_Mod_Data(&Store_303_2, sizeof(char), 1, in);
    if(Store_303_2)
    {
        Write_Mod_Data(&tb303[1].selectedpattern, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].tune, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].cutoff, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].resonance, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].envmod, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].decay, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].accent, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].waveform, sizeof(char), 1, in);
        Write_Mod_Data(&tb303[1].scale, sizeof(char), 1, in);
        Write_Mod_Data(tb303[1].patternlength, sizeof(char), 32, in);
        Write_Mod_Data(tb303[1].tone, sizeof(char), 32 * 16, in);
        Write_Mod_Data(tb303[1].flag, sizeof(struct flag303), 32 * 16, in);
    }
    
    if(Store_303_1) Write_Mod_Data(&tb303engine[0].tbVolume, sizeof(float), 1, in);
    if(Store_303_2) Write_Mod_Data(&tb303engine[1].tbVolume, sizeof(float), 1, in);

    free(New_RawPatterns);

    if(Out_FX) fclose(Out_FX);
    if(Out_constants) fclose(Out_constants);

    return(TRUE);
}
