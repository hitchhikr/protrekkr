// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2022 Franck Charlet.
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
void Read_Synth_Params(int (*Read_Function)(void *, int ,int, FILE *),
                       int (*Read_Function_Swap)(void *, int ,int, FILE *),
                       FILE *in,
                       int idx,
                       int read_disto,
                       int read_lfo_adsr,
                       int new_version,
                       int Env_Modulation,
                       int New_Env,
                       int Ntk_Beta,
                       int Combine)
{
    if(!new_version)
    {
        if(read_disto && read_lfo_adsr)
        {
            Read_Function(&PARASynth[idx], sizeof(SynthParameters), 1, in);
        }
        else
        {
            if(read_disto)
            {
                Read_Function(&PARASynth[idx], sizeof(SynthParameters) - 32, 1, in);
            }
            else
            {
                if(Ntk_Beta)
                {
                    Read_Function(&PARASynth[idx].presetname, sizeof(char), 20, in);
        
                    Read_Function(&PARASynth[idx].osc1_waveform, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].osc2_waveform, sizeof(char), 1, in);
        
                    Read_Function_Swap(&PARASynth[idx].osc1_pw, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].osc2_pw, sizeof(int), 1, in);
        
                    Read_Function(&PARASynth[idx].osc2_detune, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].osc2_finetune, sizeof(char), 1, in);
        
                    Read_Function(&PARASynth[idx].vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].vcf_resonance, sizeof(char), 1, in);
                    char phony;
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].vcf_type, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
    
                    Read_Function_Swap(&PARASynth[idx].env1_attack, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env1_decay, sizeof(int), 1, in);
                    Read_Function(&PARASynth[idx].env1_sustain, sizeof(char), 1, in);   //44
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
    
                    Read_Function_Swap(&PARASynth[idx].env1_release, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env2_attack, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env2_decay, sizeof(int), 1, in);
                    Read_Function(&PARASynth[idx].env2_sustain, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function_Swap(&PARASynth[idx].env2_release, sizeof(int), 1, in);//61
    
                    Read_Function_Swap(&PARASynth[idx].lfo1_period, sizeof(int), 1, in);
                    Read_Function_Swap(&PARASynth[idx].lfo2_period, sizeof(int), 1, in);

                    Read_Function(&PARASynth[idx].lfo1_osc1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo1_osc2_pw, sizeof(char), 1, in);
                    PARASynth[idx].lfo1_osc1_pitch = 0x40;
                    PARASynth[idx].lfo1_osc2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].lfo1_osc1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo1_osc2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo1_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo1_vcf_resonance, sizeof(char), 1, in);

                    Read_Function(&PARASynth[idx].lfo2_osc1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo2_osc2_pw, sizeof(char), 1, in);
                    PARASynth[idx].lfo2_osc1_pitch = 0x40;
                    PARASynth[idx].lfo2_osc2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].lfo2_osc1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo2_osc2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo2_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].lfo2_vcf_resonance, sizeof(char), 1, in);//81
                    Read_Function(&PARASynth[idx].env1_osc1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env1_osc2_pw, sizeof(char), 1, in);
                    PARASynth[idx].env1_osc1_pitch = 0x40;
                    PARASynth[idx].env1_osc2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].env1_osc1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env1_osc2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env1_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env1_vcf_resonance, sizeof(char), 1, in);

                    Read_Function(&PARASynth[idx].env2_osc1_pw, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env2_osc2_pw, sizeof(char), 1, in);
                    PARASynth[idx].env2_osc1_pitch = 0x40;
                    PARASynth[idx].env2_osc2_pitch = 0x40;
                    Read_Function(&PARASynth[idx].env2_osc1_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env2_osc2_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env2_vcf_cutoff, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].env2_vcf_resonance, sizeof(char), 1, in);

                    Read_Function(&PARASynth[idx].osc3_volume, sizeof(char), 1, in);
                    Read_Function(&PARASynth[idx].osc3_switch, sizeof(char), 1, in);//96
                    Read_Function(&phony, sizeof(char), 1, in);
                    Read_Function(&phony, sizeof(char), 1, in);
                    PARASynth[idx].ptc_glide = 0;
                    PARASynth[idx].glb_volume = 127;
                }
                else
                {
                    Read_Function(&PARASynth[idx], sizeof(SynthParameters) - 4 - 32, 1, in);
                }
            }
        }

        PARASynth[idx].osc1_pw = Swap_32(PARASynth[idx].osc1_pw);
        PARASynth[idx].osc2_pw = Swap_32(PARASynth[idx].osc2_pw);

        PARASynth[idx].env1_attack = Swap_32(PARASynth[idx].env1_attack);
        PARASynth[idx].env1_decay = Swap_32(PARASynth[idx].env1_decay);
        PARASynth[idx].env1_release = Swap_32(PARASynth[idx].env1_release);
    
        PARASynth[idx].env2_attack = Swap_32(PARASynth[idx].env2_attack);
        PARASynth[idx].env2_decay = Swap_32(PARASynth[idx].env2_decay);
        PARASynth[idx].env2_release = Swap_32(PARASynth[idx].env2_release);
    
        PARASynth[idx].lfo1_period = Swap_32(PARASynth[idx].lfo1_period);
        PARASynth[idx].lfo2_period = Swap_32(PARASynth[idx].lfo2_period);

        if(read_lfo_adsr)
        {
            PARASynth[idx].lfo1_attack = Swap_32(PARASynth[idx].lfo1_attack);
            PARASynth[idx].lfo1_decay = Swap_32(PARASynth[idx].lfo1_decay);
            PARASynth[idx].lfo1_release = Swap_32(PARASynth[idx].lfo1_release);

            PARASynth[idx].lfo2_attack = Swap_32(PARASynth[idx].lfo2_attack);
            PARASynth[idx].lfo2_decay = Swap_32(PARASynth[idx].lfo2_decay);
            PARASynth[idx].lfo2_release = Swap_32(PARASynth[idx].lfo2_release);
        }
    }
    else
    {
        Read_Function(&PARASynth[idx].presetname, sizeof(char), 20, in);
        
        Read_Function(&PARASynth[idx].osc1_waveform, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].osc2_waveform, sizeof(char), 1, in);
        
        Read_Function_Swap(&PARASynth[idx].osc1_pw, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].osc2_pw, sizeof(int), 1, in);
        
        Read_Function(&PARASynth[idx].osc2_detune, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].osc2_finetune, sizeof(char), 1, in);
        
        Read_Function(&PARASynth[idx].vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].vcf_resonance, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].vcf_type, sizeof(char), 1, in);
    
        Read_Function_Swap(&PARASynth[idx].env1_attack, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].env1_decay, sizeof(int), 1, in);
        Read_Function(&PARASynth[idx].env1_sustain, sizeof(char), 1, in);
        Read_Function_Swap(&PARASynth[idx].env1_release, sizeof(int), 1, in);
    
        Read_Function_Swap(&PARASynth[idx].env2_attack, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].env2_decay, sizeof(int), 1, in);
        Read_Function(&PARASynth[idx].env2_sustain, sizeof(char), 1, in);
        Read_Function_Swap(&PARASynth[idx].env2_release, sizeof(int), 1, in);
    
        Read_Function_Swap(&PARASynth[idx].lfo1_period, sizeof(int), 1, in);
        Read_Function_Swap(&PARASynth[idx].lfo2_period, sizeof(int), 1, in);

        Read_Function(&PARASynth[idx].lfo1_osc1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo1_osc2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo1_osc1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo1_osc2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo1_osc1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo1_osc2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo1_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo1_vcf_resonance, sizeof(char), 1, in);

        Read_Function(&PARASynth[idx].lfo2_osc1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo2_osc2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo2_osc1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo2_osc2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo2_osc1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo2_osc2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo2_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].lfo2_vcf_resonance, sizeof(char), 1, in);

        Read_Function(&PARASynth[idx].env1_osc1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env1_osc2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env1_osc1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env1_osc2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env1_osc1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env1_osc2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env1_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env1_vcf_resonance, sizeof(char), 1, in);

        Read_Function(&PARASynth[idx].env2_osc1_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env2_osc2_pw, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env2_osc1_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env2_osc2_pitch, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env2_osc1_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env2_osc2_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env2_vcf_cutoff, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].env2_vcf_resonance, sizeof(char), 1, in);

        Read_Function(&PARASynth[idx].osc3_volume, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].osc3_switch, sizeof(char), 1, in);

        Read_Function(&PARASynth[idx].ptc_glide, sizeof(char), 1, in);
        Read_Function(&PARASynth[idx].glb_volume, sizeof(char), 1, in);

        if(read_disto)
        {
            Read_Function(&PARASynth[idx].disto, sizeof(char), 1, in);
        }

        if(read_lfo_adsr)
        {
            Read_Function_Swap(&PARASynth[idx].lfo1_attack, sizeof(int), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo1_decay, sizeof(int), 1, in);
            Read_Function(&PARASynth[idx].lfo1_sustain, sizeof(char), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo1_release, sizeof(int), 1, in);

            Read_Function_Swap(&PARASynth[idx].lfo2_attack, sizeof(int), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo2_decay, sizeof(int), 1, in);
            Read_Function(&PARASynth[idx].lfo2_sustain, sizeof(char), 1, in);
            Read_Function_Swap(&PARASynth[idx].lfo2_release, sizeof(int), 1, in);
        }

        if(Combine) Read_Function(&PARASynth[idx].osc_combine, sizeof(char), 1, in);
    }

    if(!Env_Modulation)
    {
        char Swap;

        Swap = PARASynth[idx].env1_osc2_volume;
        PARASynth[idx].env1_osc2_volume = PARASynth[idx].env2_osc1_volume;
        PARASynth[idx].env2_osc1_volume = Swap;
    }

    if(!New_Env)
    {
        // In old versions a 64 value meant ENV = 1.0
        // In newer ones it's a normal value (0.0)
        if(PARASynth[idx].env1_osc1_volume == 64) PARASynth[idx].env1_osc1_volume = 127;
        if(PARASynth[idx].env1_osc2_volume == 64) PARASynth[idx].env1_osc2_volume = 127;
        if(PARASynth[idx].env2_osc1_volume == 64) PARASynth[idx].env2_osc1_volume = 127;
        if(PARASynth[idx].env2_osc2_volume == 64) PARASynth[idx].env2_osc2_volume = 127;
    }
}

// ------------------------------------------------------
// Save the data of a synth instrument
void Write_Synth_Params(int (*Write_Function)(void *, int ,int, FILE *),
                        int (*Write_Function_Swap)(void *, int ,int, FILE *),
                        FILE *in,
                        int idx)
{
    Write_Function(&PARASynth[idx].presetname, sizeof(char), 20, in);

    Write_Function(&PARASynth[idx].osc1_waveform, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc2_waveform, sizeof(char), 1, in);

    Write_Function_Swap(&PARASynth[idx].osc1_pw, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].osc2_pw, sizeof(int), 1, in);

    Write_Function(&PARASynth[idx].osc2_detune, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc2_finetune, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].vcf_resonance, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].vcf_type, sizeof(char), 1, in);

    Write_Function_Swap(&PARASynth[idx].env1_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].env1_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].env1_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].env1_release, sizeof(int), 1, in);

    Write_Function_Swap(&PARASynth[idx].env2_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].env2_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].env2_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].env2_release, sizeof(int), 1, in);

    Write_Function_Swap(&PARASynth[idx].lfo1_period, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo2_period, sizeof(int), 1, in);

    Write_Function(&PARASynth[idx].lfo1_osc1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo1_osc2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo1_osc1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo1_osc2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo1_osc1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo1_osc2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo1_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo1_vcf_resonance, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].lfo2_osc1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo2_osc2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo2_osc1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo2_osc2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo2_osc1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo2_osc2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo2_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].lfo2_vcf_resonance, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].env1_osc1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env1_osc2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env1_osc1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env1_osc2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env1_osc1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env1_osc2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env1_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env1_vcf_resonance, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].env2_osc1_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env2_osc2_pw, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env2_osc1_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env2_osc2_pitch, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env2_osc1_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env2_osc2_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env2_vcf_cutoff, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].env2_vcf_resonance, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].osc3_volume, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].osc3_switch, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].ptc_glide, sizeof(char), 1, in);
    Write_Function(&PARASynth[idx].glb_volume, sizeof(char), 1, in);

    Write_Function(&PARASynth[idx].disto, sizeof(char), 1, in);

    Write_Function_Swap(&PARASynth[idx].lfo1_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo1_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].lfo1_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo1_release, sizeof(int), 1, in);

    Write_Function_Swap(&PARASynth[idx].lfo2_attack, sizeof(int), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo2_decay, sizeof(int), 1, in);
    Write_Function(&PARASynth[idx].lfo2_sustain, sizeof(char), 1, in);
    Write_Function_Swap(&PARASynth[idx].lfo2_release, sizeof(int), 1, in);

    Write_Function(&PARASynth[idx].osc_combine, sizeof(char), 1, in);
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

    in = fopen(FileName, "rb");

    if(in != NULL)
    {
        // Reading and checking extension...
        char extension[10];
        fread(extension, sizeof(char), 9, in);

        switch(extension[7])
        {
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

        Status_Box("Loading Synthesizer -> structure...");
        ResetSynthParameters(&PARASynth[Current_Instrument]);

        PARASynth[Current_Instrument].disto = 0;

        PARASynth[Current_Instrument].lfo1_attack = 0;
        PARASynth[Current_Instrument].lfo1_decay = 0;
        PARASynth[Current_Instrument].lfo1_sustain = 128;
        PARASynth[Current_Instrument].lfo1_release = 0x10000;

        PARASynth[Current_Instrument].lfo2_attack = 0;
        PARASynth[Current_Instrument].lfo2_decay = 0;
        PARASynth[Current_Instrument].lfo2_sustain = 128;
        PARASynth[Current_Instrument].lfo2_release = 0x10000;

        Read_Synth_Params(Read_Data, Read_Data_Swap, in, Current_Instrument,
                          TRUE, TRUE, new_version,
                          Env_Modulation, New_Env, FALSE, Combine);

        // Fix some old Ntk bugs
        if(PARASynth[Current_Instrument].lfo1_period > 128) PARASynth[Current_Instrument].lfo1_period = 128;
        if(PARASynth[Current_Instrument].lfo2_period > 128) PARASynth[Current_Instrument].lfo2_period = 128;

        Synthprg[Current_Instrument] = SYNTH_WAVE_CURRENT;
        sprintf(nameins[Current_Instrument],PARASynth[Current_Instrument].presetname);
        Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);

        Actualize_Instrument_Ed(0, 0);
        Actualize_DiskIO_Ed(0);
        Actualize_Patterned();
        Status_Box("Synthesizer program loaded ok.");
        fclose(in);
    }
    else
    {
        Status_Box("Synthesizer program loading failed. (Possible cause: file not found)");
    }
}

// ------------------------------------------------------
// Save the current synth preset
void Save_Synth(void)
{
    FILE *in;
    char Temph[MAX_PATH];
    char extension[10];

    sprintf(extension, "TWNNSYN4");
    sprintf (Temph, "Saving '%s.pts' synthesizer program in presets directory...", PARASynth[Current_Instrument].presetname);
    Status_Box(Temph);

    sprintf(Temph, "%s" SLASH "%s.pts", Dir_Presets, PARASynth[Current_Instrument].presetname);

    in = fopen(Temph, "wb");
    if(in != NULL)
    {
        Write_Data(extension, sizeof(char), 9, in);
        Write_Synth_Params(Write_Data, Write_Data_Swap, in, Current_Instrument);
        fclose(in);

        Read_SMPT();
        last_index = -1;
        Actualize_Files_List(0);
        Status_Box("Synthesizer program saved succesfully."); 
    }
    else
    {
        Status_Box("Synthesizer program save failed.");
    }
    Clear_Input();
}
#endif
