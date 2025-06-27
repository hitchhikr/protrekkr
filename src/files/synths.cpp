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
#include "include/synths.h"

// ------------------------------------------------------
// Load the data of a synth instrument
// (The new version (v4) use correct data aligment)
#if !defined(BZR2)
void Read_Synth_Params(int (*Read_Function)(void *, int, int, FILE *),
                       int (*Read_Function_Swap)(void *, int, int, FILE *),
                       FILE *in,
#else
void Read_Synth_Params(int (*Read_Function)(void *, int, int, CustomFile &),
                       int (*Read_Function_Swap)(void *, int, int, CustomFile &),
                       CustomFile &in,
#endif
                       int idx,
                       int read_disto,
                       int read_lfo_adsr,
                       int new_version,
                       int Env_Modulation,
                       int New_Env,
                       int Ntk_Beta,
                       int Combine,
                       int Var_Disto,
                       int Sync,
                       int Osc_3_Interval
                      )
{
    if(!new_version)
    {
        if(read_disto && read_lfo_adsr)
        {
            Read_Function(&PARASynth[idx], sizeof(Synth_Parameters) - 4 - 2, 1, in);
            PARASynth[idx].disto /= 2;
            PARASynth[idx].disto += 64;
            PARASynth[idx].glb_volume = 0x7f;
        }
        else
        {
            if(read_disto)
            {
                Read_Function(&PARASynth[idx], sizeof(Synth_Parameters) - 32 - 4 - 2, 1, in);
            }
            else
            {
                if(Ntk_Beta)
                {
                    char phony;
                    Read_Function(&PARASynth[idx].Preset_Name, sizeof(char), 20, in);
        
                    Read_Function(&PARASynth[idx].osc_1_waveform, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].osc_2_waveform, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
        
                    Read_Function_Swap(&PARASynth[idx].osc_1_pw, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].osc_2_pw, sizeof(int), 1, in);

                    Read_Function(&PARASynth[idx].osc_2_detune, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].osc_2_finetune, sizeof(char), 1, in);
        
                    Read_Function(&PARASynth[idx].vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].vcf_resonance, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].vcf_type, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
    
                    Read_Function_Swap(&PARASynth[idx].env_1_attack, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env_1_decay, sizeof(int), 1, in);

                    Read_Function(&PARASynth[idx].env_1_sustain, sizeof(char), 1, in);   //44
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
    
                    Read_Function_Swap(&PARASynth[idx].env_1_release, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env_2_attack, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env_2_decay, sizeof(int), 1, in);

                    Read_Function(&PARASynth[idx].env_2_sustain, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env_2_release, sizeof(int), 1, in);//61
    
                    Read_Function_Swap(&PARASynth[idx].lfo_1_period, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].lfo_2_period, sizeof(int), 1, in);

                    Read_Function(&PARASynth[idx].lfo_1_osc_1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_1_osc_2_pw, sizeof(char), 1, in);
                    PARASynth[idx].lfo_1_osc_1_pitch = 0x40;
                    PARASynth[idx].lfo_1_osc_2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].lfo_1_osc_1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_1_osc_2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_1_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_1_vcf_resonance, sizeof(char), 1, in);

                    Read_Function(&PARASynth[idx].lfo_2_osc_1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_2_osc_2_pw, sizeof(char), 1, in);
                    PARASynth[idx].lfo_2_osc_1_pitch = 0x40;
                    PARASynth[idx].lfo_2_osc_2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].lfo_2_osc_1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_2_osc_2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_2_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo_2_vcf_resonance, sizeof(char), 1, in); //81

                    Read_Function(&PARASynth[idx].env_1_osc_1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_1_osc_2_pw, sizeof(char), 1, in);
                    PARASynth[idx].env_1_osc_1_pitch = 0x40;
                    PARASynth[idx].env_1_osc_2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].env_1_osc_1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_1_osc_2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_1_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_1_vcf_resonance, sizeof(char), 1, in);

                    Read_Function(&PARASynth[idx].env_2_osc_1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_2_osc_2_pw, sizeof(char), 1, in);
                    PARASynth[idx].env_2_osc_1_pitch = 0x40;
                    PARASynth[idx].env_2_osc_2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].env_2_osc_1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_2_osc_2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_2_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env_2_vcf_resonance, sizeof(char), 1, in);

                    Read_Function(&PARASynth[idx].osc_3_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].osc_3_switch, sizeof(char), 1, in);//96
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    PARASynth[idx].ptc_glide = 0;
                    PARASynth[idx].glb_volume = 127;
                }
                else
                {
                    Read_Function(&PARASynth[idx], sizeof(Synth_Parameters) - 4 - 32 - 4 - 2, 1, in);
                }
            }
        }

        PARASynth[idx].osc_1_pw = Swap_32(PARASynth[idx].osc_1_pw);
        PARASynth[idx].osc_2_pw = Swap_32(PARASynth[idx].osc_2_pw);

        PARASynth[idx].env_1_attack = Swap_32(PARASynth[idx].env_1_attack);
        PARASynth[idx].env_1_decay = Swap_32(PARASynth[idx].env_1_decay);
        PARASynth[idx].env_1_release = Swap_32(PARASynth[idx].env_1_release);
    
        PARASynth[idx].env_2_attack = Swap_32(PARASynth[idx].env_2_attack);
        PARASynth[idx].env_2_decay = Swap_32(PARASynth[idx].env_2_decay);
        PARASynth[idx].env_2_release = Swap_32(PARASynth[idx].env_2_release);
    
        PARASynth[idx].lfo_1_period = Swap_32(PARASynth[idx].lfo_1_period);
        PARASynth[idx].lfo_2_period = Swap_32(PARASynth[idx].lfo_2_period);

        if(read_lfo_adsr)
        {
            PARASynth[idx].lfo_1_attack = Swap_32(PARASynth[idx].lfo_1_attack);
            PARASynth[idx].lfo_1_decay = Swap_32(PARASynth[idx].lfo_1_decay);
            PARASynth[idx].lfo_1_release = Swap_32(PARASynth[idx].lfo_1_release);

            PARASynth[idx].lfo_2_attack = Swap_32(PARASynth[idx].lfo_2_attack);
            PARASynth[idx].lfo_2_decay = Swap_32(PARASynth[idx].lfo_2_decay);
            PARASynth[idx].lfo_2_release = Swap_32(PARASynth[idx].lfo_2_release);
        }
    }
    else
    {
        Read_Function(&PARASynth[idx].Preset_Name, sizeof(char), 20, in);
        
        Read_Function(&PARASynth[idx].osc_1_waveform, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].osc_2_waveform, sizeof(char), 1, in);
        
        Read_Function_Swap(&PARASynth[idx].osc_1_pw, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].osc_2_pw, sizeof(int), 1, in);
        
        Read_Function(&PARASynth[idx].osc_2_detune, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].osc_2_finetune, sizeof(char), 1, in);
        
        Read_Function(&PARASynth[idx].vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].vcf_resonance, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].vcf_type, sizeof(char), 1, in);
    
        Read_Function_Swap(&PARASynth[idx].env_1_attack, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].env_1_decay, sizeof(int), 1, in);
        Read_Function(&PARASynth[idx].env_1_sustain, sizeof(char), 1, in);
        Read_Function_Swap(&PARASynth[idx].env_1_release, sizeof(int), 1, in);
    
        Read_Function_Swap(&PARASynth[idx].env_2_attack, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].env_2_decay, sizeof(int), 1, in);
        Read_Function(&PARASynth[idx].env_2_sustain, sizeof(char), 1, in);
        Read_Function_Swap(&PARASynth[idx].env_2_release, sizeof(int), 1, in);
    
        Read_Function_Swap(&PARASynth[idx].lfo_1_period, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].lfo_2_period, sizeof(int), 1, in);

        Read_Function(&PARASynth[idx].lfo_1_osc_1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_1_osc_2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_1_osc_1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_1_osc_2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_1_osc_1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_1_osc_2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_1_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_1_vcf_resonance, sizeof(char), 1, in);
        if(Var_Disto)
        {
            Read_Function(&PARASynth[idx].lfo_1_disto, sizeof(char), 1, in);
        }
        Read_Function(&PARASynth[idx].lfo_2_osc_1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_2_osc_2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_2_osc_1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_2_osc_2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_2_osc_1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_2_osc_2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_2_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo_2_vcf_resonance, sizeof(char), 1, in);
        if(Var_Disto)
        {
            Read_Function(&PARASynth[idx].lfo_2_disto, sizeof(char), 1, in);
        }
        Read_Function(&PARASynth[idx].env_1_osc_1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_1_osc_2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_1_osc_1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_1_osc_2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_1_osc_1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_1_osc_2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_1_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_1_vcf_resonance, sizeof(char), 1, in);
        if(Var_Disto)
        {
            Read_Function(&PARASynth[idx].env_1_disto, sizeof(char), 1, in);
        }
        Read_Function(&PARASynth[idx].env_2_osc_1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_2_osc_2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_2_osc_1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_2_osc_2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_2_osc_1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_2_osc_2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_2_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env_2_vcf_resonance, sizeof(char), 1, in);
        
        if(Var_Disto)
        {
            Read_Function(&PARASynth[idx].env_2_disto, sizeof(char), 1, in);
        }
        Read_Function(&PARASynth[idx].osc_3_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].osc_3_switch, sizeof(char), 1, in);

        Read_Function(&PARASynth[idx].ptc_glide, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].glb_volume, sizeof(char), 1, in);

        if(read_disto)
        {
            Read_Function(&PARASynth[idx].disto, sizeof(char), 1, in);
        }
        if(!Var_Disto)
        {
            // New disto can now be positive or negative
            PARASynth[idx].disto /= 2;
            PARASynth[idx].disto += 64;
        }

        if(read_lfo_adsr)
        {
            Read_Function_Swap(&PARASynth[idx].lfo_1_attack, sizeof(int), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo_1_decay, sizeof(int), 1, in);
            Read_Function(&PARASynth[idx].lfo_1_sustain, sizeof(char), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo_1_release, sizeof(int), 1, in);

            Read_Function_Swap(&PARASynth[idx].lfo_2_attack, sizeof(int), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo_2_decay, sizeof(int), 1, in);
            Read_Function(&PARASynth[idx].lfo_2_sustain, sizeof(char), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo_2_release, sizeof(int), 1, in);
        }

        if(Combine)
        {
            Read_Function(&PARASynth[idx].osc_combine, sizeof(char), 1, in);
        }
        if(Sync)
        {
            Read_Function(&PARASynth[idx].osc_sync, sizeof(char), 1, in);
        }

        if(Osc_3_Interval)
        {
            Read_Function(&PARASynth[idx].osc_3_interval, sizeof(char), 1, in);
        }

    }

    if(!Env_Modulation)
    {
        char Swap;

        Swap = PARASynth[idx].env_1_osc_2_volume;
        PARASynth[idx].env_1_osc_2_volume = PARASynth[idx].env_2_osc_1_volume;
        PARASynth[idx].env_2_osc_1_volume = Swap;
    }

    if(!New_Env)
    {
        // In old versions a 64 value meant ENV = 1.0
        // In newer ones it's a normal value (0.0)
        if(PARASynth[idx].env_1_osc_1_volume == 64) PARASynth[idx].env_1_osc_1_volume = 127;
        if(PARASynth[idx].env_1_osc_2_volume == 64) PARASynth[idx].env_1_osc_2_volume = 127;
        if(PARASynth[idx].env_2_osc_1_volume == 64) PARASynth[idx].env_2_osc_1_volume = 127;
        if(PARASynth[idx].env_2_osc_2_volume == 64) PARASynth[idx].env_2_osc_2_volume = 127;
    }

    if(!Sync)
    {
        // Remove those
        if(PARASynth[idx].osc_1_waveform == WAVEFORM_PINK)
        {
            PARASynth[idx].osc_1_waveform = WAVEFORM_WHITE;
        }
        if(PARASynth[idx].osc_2_waveform == WAVEFORM_PINK)
        {
            PARASynth[idx].osc_2_waveform = WAVEFORM_WHITE;
        }
    }
}

// ------------------------------------------------------
// Save the data of a synth instrument
void Write_Synth_Params(int (*Write_Function)(void *, int ,int, FILE *),
                        int (*Write_Function_Swap)(void *, int ,int, FILE *),
                        FILE *in,
                        int idx)
{
    Write_Function(&PARASynth[idx].Preset_Name, sizeof(char), 20, in);

    Write_Function(&PARASynth[idx].osc_1_waveform, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc_2_waveform, sizeof(char), 1, in);

    Write_Function_Swap(&PARASynth[idx].osc_1_pw, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].osc_2_pw, sizeof(int), 1, in);

    Write_Function(&PARASynth[idx].osc_2_detune, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc_2_finetune, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].vcf_resonance, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].vcf_type, sizeof(char), 1, in);

    Write_Function_Swap(&PARASynth[idx].env_1_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].env_1_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].env_1_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].env_1_release, sizeof(int), 1, in);

    Write_Function_Swap(&PARASynth[idx].env_2_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].env_2_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].env_2_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].env_2_release, sizeof(int), 1, in);

    Write_Function_Swap(&PARASynth[idx].lfo_1_period, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo_2_period, sizeof(int), 1, in);

    Write_Function(&PARASynth[idx].lfo_1_osc_1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_osc_2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_osc_1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_osc_2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_osc_1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_osc_2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_vcf_resonance, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_disto, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].lfo_2_osc_1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_osc_2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_osc_1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_osc_2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_osc_1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_osc_2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_vcf_resonance, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_disto, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].env_1_osc_1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_osc_2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_osc_1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_osc_2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_osc_1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_osc_2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_vcf_resonance, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_1_disto, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].env_2_osc_1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_osc_2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_osc_1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_osc_2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_osc_1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_osc_2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_vcf_resonance, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env_2_disto, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].osc_3_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc_3_switch, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].ptc_glide, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].glb_volume, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].disto, sizeof(char), 1, in);

    Write_Function_Swap(&PARASynth[idx].lfo_1_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo_1_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].lfo_1_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo_1_release, sizeof(int), 1, in);

    Write_Function_Swap(&PARASynth[idx].lfo_2_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo_2_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].lfo_2_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo_2_release, sizeof(int), 1, in);

    Write_Function(&PARASynth[idx].osc_combine, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc_sync, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc_3_interval, sizeof(char), 1, in);
}

// ------------------------------------------------------
// Load a synth preset
#if !defined(__WINAMP__)
void Load_Synth(char *FileName)
{
    FILE *in;
    int new_version = FALSE;
    int Env_Modulation = FALSE;
    int New_Env = FALSE;
    int Combine = FALSE;
    int Disto = FALSE;
    int Sync = FALSE;
    int Osc_3_Interval = FALSE;

    in = fopen(FileName, "rb");

    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];
        fread(extension, sizeof(char), 9, in);

        switch(extension[7])
        {
            case '8':
                Osc_3_Interval = TRUE;
            case '7':
            case '6':
                Sync = TRUE;
            case '5':
                Disto = TRUE;
            case '4':
                Combine = TRUE;
            case '3':
                New_Env = TRUE;
            case '2':
                Env_Modulation = TRUE;
            case '1':
                new_version = TRUE;
            case '0':
                break;
        }

        Status_Box("Loading Synthesizer -> Structure...", TRUE);
        Reset_Synth_Parameters(&PARASynth[Current_Instrument]);

        PARASynth[Current_Instrument].disto = 64;
        PARASynth[Current_Instrument].glb_volume = 64;

        PARASynth[Current_Instrument].lfo_1_attack = 0;
        PARASynth[Current_Instrument].lfo_1_decay = 0;
        PARASynth[Current_Instrument].lfo_1_sustain = 128;
        PARASynth[Current_Instrument].lfo_1_release = 0x10000;

        PARASynth[Current_Instrument].lfo_2_attack = 0;
        PARASynth[Current_Instrument].lfo_2_decay = 0;
        PARASynth[Current_Instrument].lfo_2_sustain = 128;
        PARASynth[Current_Instrument].lfo_2_release = 0x10000;

        Read_Synth_Params(Read_Data, Read_Data_Swap, in, Current_Instrument,
                          TRUE, TRUE, new_version,
                          Env_Modulation, New_Env, FALSE, Combine, Disto,
                          Sync, Osc_3_Interval);

        // Fix some old Ntk bugs
        if(PARASynth[Current_Instrument].lfo_1_period > 128) PARASynth[Current_Instrument].lfo_1_period = 128;
        if(PARASynth[Current_Instrument].lfo_2_period > 128) PARASynth[Current_Instrument].lfo_2_period = 128;

        Synth_Prg[Current_Instrument] = SYNTH_WAVE_CURRENT;
        sprintf(nameins[Current_Instrument],PARASynth[Current_Instrument].Preset_Name);
        Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);

        Actualize_Instrument_Ed(0, 0);
        Actualize_DiskIO_Ed(0);
        Actualize_Pattern_Ed();
        Status_Box("Synthesizer Program Loaded Successfully.", TRUE);
        fclose(in);
    }
    else
    {
        Status_Box("Synthesizer Program Loading Failed. (Possible Cause: File Not Found)", TRUE);
    }
}

// ------------------------------------------------------
// Save the current synth preset
void Save_Synth(void)
{
    FILE *in;
    char Temph[MAX_PATH];
    char extension[10];

    sprintf(extension, "PROTSYN8");
    sprintf (Temph, "Saving '%s.pts' Synthesizer Program In Presets Directory...", PARASynth[Current_Instrument].Preset_Name);
    Status_Box(Temph, TRUE);

    sprintf(Temph, "%s" SLASH "%s.pts", Dir_Presets, PARASynth[Current_Instrument].Preset_Name);

    in = fopen(Temph, "wb");
    if(in != NULL)
    {
        Write_Data(extension, sizeof(char), 9, in);
        Write_Synth_Params(Write_Data, Write_Data_Swap, in, Current_Instrument);
        fclose(in);

        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Status_Box("Synthesizer Program Saved Successfully.", TRUE); 
    }
    else
    {
        Status_Box("Synthesizer Program Saving Failed.", TRUE);
    }
    Clear_Input();
}
#endif
