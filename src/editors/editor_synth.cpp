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
#include "include/editor_synth.h"
#include "include/editor_instrument.h"
#include "../support/include/timer.h"
#include "../files/include/files.h"
#include "../ui/include/requesters.h"

// ------------------------------------------------------
// Variables
extern REQUESTER Overwrite_Requester;

extern SynthParameters PARASynth[128];
extern int plx;
int csynth_slv_OSC1;
int csynth_slv_OSC2;
int csynth_slv_VCF;
int csynth_slv_LFO1;
int csynth_slv_LFO2;
int csynth_slv_ENV1;
int csynth_slv_ENV2;
int csynth_slv_Misc;

int Allow_All;
int Allow_Button;
int Allow_Button_Pushed;
int Allow_Phase_Distortion_OSC1 = TRUE;
int Allow_Phase_Distortion_OSC2 = TRUE;
   
void Actualize_Patterned(void);
void Actualize_SynthParSlider(void);
void Actualize_Sequencer(void);
void Renew_Sample_Ed(void);
void Center_SynthParam_OSC1(void);
void Center_SynthParam_OSC2(void);
void Center_SynthParam_VCF(void);
void Center_SynthParam_LFO1(void);
void Center_SynthParam_LFO2(void);
void Center_SynthParam_ENV1(void);
void Center_SynthParam_ENV2(void);
void Center_SynthParam_Misc(void);
void Rand_OSC1();
void Rand_OSC2();
void Rand_VCF();
void Rand_LFO1();
void Rand_LFO2();
void Rand_ENV1();
void Rand_ENV2();
void Rand_Misc();
void CParcha_OSC1(int cpar);
void CParcha_OSC2(int cpar);
void CParcha_VCF(int cpar);
void CParcha_LFO1(int cpar);
void CParcha_LFO2(int cpar);
void CParcha_ENV1(int cpar);
void CParcha_ENV2(int cpar);
void CParcha_Misc(int cpar);

// 2 = no filter by legacy
int filter_vcf_type_plus[] =
{
    1, 3, 2, 4, 2
};

int filter_vcf_type_minus[] =
{
    0, 0, 4, 1, 3
};

int Tbl_Synth_OSC1[] =
{
    1, /* Osc1_PD */
    7, /* ENV1_Attack */
    8, /* ENV1_Decay */
    9, /* ENV1_Sustain */
    10, /* ENV1_Release */
    49, /* OSC3 volume */
};
int Pos_Tbl_Synth_OSC1;
int Size_Tbl_Synth_OSC1 = (sizeof(Tbl_Synth_OSC1) / sizeof(int)) - 1;
char Names_Tbl_Synth_OSC1[(sizeof(Tbl_Synth_OSC1) / sizeof(int))][24];

int Tbl_Synth_OSC2[] =
{
    2, /* Osc2_PD */
    3, /* Osc2_Detune */
    4, /* Osc2_Finetune */
    11, /* ENV2_Attack */
    12, /* ENV2_Decay */
    13, /* ENV2_Sustain */
    14, /* ENV2_Release */
};
int Pos_Tbl_Synth_OSC2;
int Size_Tbl_Synth_OSC2 = (sizeof(Tbl_Synth_OSC2) / sizeof(int)) - 1;
char Names_Tbl_Synth_OSC2[(sizeof(Tbl_Synth_OSC2) / sizeof(int))][24];

int Tbl_Synth_VCF[] =
{
    5, /* VCF Cutoff */
    6, /* VCF_Resonance */
};
int Pos_Tbl_Synth_VCF;
int Size_Tbl_Synth_VCF = (sizeof(Tbl_Synth_VCF) / sizeof(int)) - 1;
char Names_Tbl_Synth_VCF[(sizeof(Tbl_Synth_VCF) / sizeof(int))][24];

int Tbl_Synth_LFO1[] =
{
    15, /* LFO1_Period */
    17, /* Lfo1->osc1 pd */
    18, /* Lfo1->osc2 pd */
    19, /* Lfo1->osc1 pitch */
    20, /* Lfo1->osc1 pitch */
    21, /* Lfo1->osc1 volume */
    22, /* Lfo1->osc2 volume */
    23, /* Lfo1->vcf cutoff */
    24, /* Lfo1->vcf resonance */
    25, /* Lfo1_attack */
    26, /* Lfo1_decay */
    27, /* Lfo1_sustain */
    28, /* Lfo1_release */
};
int Pos_Tbl_Synth_LFO1;
int Size_Tbl_Synth_LFO1 = (sizeof(Tbl_Synth_LFO1) / sizeof(int)) - 1;
char Names_Tbl_Synth_LFO1[(sizeof(Tbl_Synth_LFO1) / sizeof(int))][24];

int Tbl_Synth_LFO2[] =
{
    16, /* LFO2_Period */
    25, /* Lfo2->osc1 pd */
    26, /* Lfo2->osc2 pd */
    27, /* Lfo2->osc1 pitch */
    28, /* Lfo2->osc2 pitch */
    29, /* Lfo2->osc1 volume */
    30, /* Lfo2->osc2 volume */
    31, /* Lfo2->vcf cutoff */
    32, /* Lfo2->vcf resonance */
    25, /* Lfo2_attack */
    26, /* Lfo2_decay */
    27, /* Lfo2_sustain */
    28, /* Lfo2_release */
};
int Pos_Tbl_Synth_LFO2;
int Size_Tbl_Synth_LFO2 = (sizeof(Tbl_Synth_LFO2) / sizeof(int)) - 1;
char Names_Tbl_Synth_LFO2[(sizeof(Tbl_Synth_LFO2) / sizeof(int))][24];

int Tbl_Synth_ENV1[] =
{
    33, /* Env1->osc1 pd */
    34, /* Env1->osc2 pd */
    35, /* Env1->osc1 pitch */
    36, /* Env1->osc2 pitch */
    37, /* Env1->osc1 volume */
    38, /* Env1->osc2 volume */
    39, /* Env1->vcf cutoff */
    40, /* Env1->vcf resonance */
};
int Pos_Tbl_Synth_ENV1;
int Size_Tbl_Synth_ENV1 = (sizeof(Tbl_Synth_ENV1) / sizeof(int)) - 1;
char Names_Tbl_Synth_ENV1[(sizeof(Tbl_Synth_ENV1) / sizeof(int))][24];

int Tbl_Synth_ENV2[] =
{
    41, /* Env2->osc1 pd */
    42, /* Env2->osc2 pd */
    43, /* Env2->osc1 pitch */
    44, /* Env2->osc2 pitch */
    45, /* Env2->osc1 volume */
    46, /* Env2->osc2 volume */
    47, /* Env2->vcf cutoff */
    48, /* Env2->vcf resonance */
};
int Pos_Tbl_Synth_ENV2;
int Size_Tbl_Synth_ENV2 = (sizeof(Tbl_Synth_ENV2) / sizeof(int)) - 1;
char Names_Tbl_Synth_ENV2[(sizeof(Tbl_Synth_ENV2) / sizeof(int))][24];

int Tbl_Synth_Misc[] =
{
    50, /* Misc. glide */
    51, /* Misc. Global volume */
    52, /* Misc. Distortion */
};
int Pos_Tbl_Synth_Misc;
int Size_Tbl_Synth_Misc = (sizeof(Tbl_Synth_Misc) / sizeof(int)) - 1;
char Names_Tbl_Synth_Misc[(sizeof(Tbl_Synth_Misc) / sizeof(int))][24];

void Draw_Synth_Ed(void)
{
    Draw_Editors_Bar(USER_SCREEN_SYNTH_EDIT);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, "", BUTTON_NORMAL | BUTTON_DISABLED);
    Gui_Draw_Flat_Box("Instrument Editor [Synth]");

    Gui_Draw_Button_Box(508, (Cur_Height - 150), 56, 16, "Program", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(294, (Cur_Height - 150), 53, 16, "VCF Type", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(170, (Cur_Height - 150), 56, 16, "Instrument", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box( 24, (Cur_Height - 131) - 1, 36, 16, "OSC 1", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(200, (Cur_Height - 95) - 1, 35, 16, "Sub O.", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box( 24, (Cur_Height - 77) - 1, 36, 16, "OSC 2", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(294, (Cur_Height - 131) - 1, 35, 16, "VCF", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(294, (Cur_Height - 95) - 1, 35, 16, "LFO 1", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(294, (Cur_Height - 59) - 1, 35, 16, "LFO 2", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(546, (Cur_Height - 131) - 1, 35, 16, "ENV 1", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(546, (Cur_Height - 95) - 1, 35, 16, "ENV 2", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(546, (Cur_Height - 59) - 1, 35, 16, "Misc.", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
}

void Actualize_Synth_Ed(char gode)
{
    if(userscreen == USER_SCREEN_SYNTH_EDIT)
    {
        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_CHANGE_ASSOCIATED_SAMPLE ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            int synthprg_check = Synthprg[Current_Instrument];
            if(synthprg_check > 130) Synthprg[Current_Instrument] = 130;
            
            switch(Synthprg[Current_Instrument])
            {
                case 0:
                    Allow_All = FALSE;
                    Allow_Button = BUTTON_NORMAL | BUTTON_DISABLED;
                    Allow_Button_Pushed = BUTTON_PUSHED | BUTTON_DISABLED;
                    Gui_Draw_Button_Box(228, (Cur_Height - 150), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                    Gui_Draw_Button_Box(228 + 44, (Cur_Height - 150), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                    Gui_Draw_Button_Box(228 + 18, (Cur_Height - 150), 24, 16, "Off", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                case 1:
                    Allow_All = TRUE;
                    Allow_Button = BUTTON_NORMAL;
                    Allow_Button_Pushed = BUTTON_PUSHED;
                    Gui_Draw_Button_Box(228, (Cur_Height - 150), 16, 16, "\03", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                    Gui_Draw_Button_Box(228 + 44, (Cur_Height - 150), 16, 16, "\04", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                    Gui_Draw_Button_Box(228 + 18, (Cur_Height - 150), 24, 16, "Curr", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                default:
                    Allow_All = TRUE;
                    Allow_Button = BUTTON_NORMAL;
                    Allow_Button_Pushed = BUTTON_PUSHED;
                    value_box(228, (Cur_Height - 150), (Synthprg[Current_Instrument] - 2), BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                    break;
            }
            Gui_Draw_Button_Box(758, (Cur_Height - 150), 34, 16, "Save", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(506, (Cur_Height - 131) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(524, (Cur_Height - 113) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(294, (Cur_Height - 113) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(460, (Cur_Height - 113) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Actualize_Instruments_Synths_List(1);
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_OSC1_WAVEFORM ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            Gui_Draw_Button_Box(24, (Cur_Height - 95) - 1, 23, 16, "sin", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(49, (Cur_Height - 95) - 1, 23, 16, "saw", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(74, (Cur_Height - 95) - 1, 23, 16, "pul", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(99, (Cur_Height - 95) - 1, 23, 16, "whit", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(124, (Cur_Height - 95) - 1, 23, 16, "pink", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(149, (Cur_Height - 95) - 1, 23, 16, "instr", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(174, (Cur_Height - 95) - 1, 23, 16, "off", Allow_Button | BUTTON_TEXT_CENTERED);
            switch(PARASynth[Current_Instrument].osc1_waveform)
            {
                case 0:Gui_Draw_Button_Box(24, (Cur_Height - 95) - 1, 23, 16, "sin", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 1:Gui_Draw_Button_Box(49, (Cur_Height - 95) - 1, 23, 16, "saw", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 2:Gui_Draw_Button_Box(74, (Cur_Height - 95) - 1, 23, 16, "pul", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 3:Gui_Draw_Button_Box(99, (Cur_Height - 95) - 1, 23, 16, "whit", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 6:Gui_Draw_Button_Box(124, (Cur_Height - 95) - 1, 23, 16, "pink", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 5:Gui_Draw_Button_Box(149, (Cur_Height - 95) - 1, 23, 16, "instr", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 4:Gui_Draw_Button_Box(174, (Cur_Height - 95) - 1, 23, 16, "off", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
            }
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_OSC2_WAVEFORM ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            Gui_Draw_Button_Box(24, (Cur_Height - 41) - 1, 23, 16, "sin", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(49, (Cur_Height - 41) - 1, 23, 16, "saw", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(74, (Cur_Height - 41) - 1, 23, 16, "pul", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(99, (Cur_Height - 41) - 1, 23, 16, "whit", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(124, (Cur_Height - 41) - 1, 23, 16, "pink", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(149, (Cur_Height - 41) - 1, 23, 16, "instr", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(174, (Cur_Height - 41) - 1, 23, 16, "off", Allow_Button | BUTTON_TEXT_CENTERED);
            switch(PARASynth[Current_Instrument].osc2_waveform)
            {
                case 0: Gui_Draw_Button_Box(24, (Cur_Height - 41) - 1, 23, 16, "sin", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 1: Gui_Draw_Button_Box(49, (Cur_Height - 41) - 1, 23, 16, "saw", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 2: Gui_Draw_Button_Box(74, (Cur_Height - 41) - 1, 23, 16, "pul", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 3: Gui_Draw_Button_Box(99, (Cur_Height - 41) - 1, 23, 16, "whit", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 6: Gui_Draw_Button_Box(124, (Cur_Height - 41) - 1, 23, 16, "pink", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 5: Gui_Draw_Button_Box(149, (Cur_Height - 41) - 1, 23, 16, "instr", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 4: Gui_Draw_Button_Box(174, (Cur_Height - 41) - 1, 23, 16, "off", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
            }
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_COMBINER||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 0), 17, 16, "+", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 1), 17, 16, "-", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 2), 17, 16, "*", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 3), 17, 16, "/", Allow_Button | BUTTON_TEXT_CENTERED);
            switch(PARASynth[Current_Instrument].osc_combine)
            {
                case COMBINE_ADD: Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 0), 17, 16, "+", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case COMBINE_SUB: Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 1), 17, 16, "-", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case COMBINE_MUL: Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 2), 17, 16, "*", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case COMBINE_DIV: Gui_Draw_Button_Box(4, (Cur_Height - 95) - 1 + (18 * 3), 17, 16, "/", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
            }
        }


        // Instrument number
        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_VALUES)
        {
            value_box4(566, (Cur_Height - 150), Current_Instrument);
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_CHANGE_NAME ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            char tcp[30];
            sprintf(tcp, "%s_", PARASynth[Current_Instrument].presetname);

            if(snamesel == INPUT_SYNTH_NAME)
            {
                Gui_Draw_Button_Box(592, (Cur_Height - 150), 164, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT);
            }
            else
            {
                Gui_Draw_Button_Box(592, (Cur_Height - 150), 164, 16, PARASynth[Current_Instrument].presetname, Allow_Button | BUTTON_INPUT);
            }
            Actualize_Instruments_Synths_List(1);
        }

        if(gode == UPDATE_SYNTH_ED_VALIDATE_PARAMS)
        {
            CParcha_OSC1(csynth_slv_OSC1);
            CParcha_OSC2(csynth_slv_OSC2);
            CParcha_VCF(csynth_slv_VCF);
            CParcha_LFO1(csynth_slv_LFO1);
            CParcha_LFO2(csynth_slv_LFO2);
            CParcha_ENV1(csynth_slv_ENV1);
            CParcha_ENV2(csynth_slv_ENV2);
            CParcha_Misc(csynth_slv_Misc);
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_OSC1_PARAMS)
        {
            if(Pos_Tbl_Synth_OSC1 < 0) Pos_Tbl_Synth_OSC1 = 0;
            if(Pos_Tbl_Synth_OSC1 > Size_Tbl_Synth_OSC1) Pos_Tbl_Synth_OSC1 = Size_Tbl_Synth_OSC1;
            Gui_Draw_Arrows_Number_Box2(62, (Cur_Height - 131) - 1, Pos_Tbl_Synth_OSC1 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(62 + 62, (Cur_Height - 131) - 1, 128, 16, Names_Tbl_Synth_OSC1[Pos_Tbl_Synth_OSC1], BUTTON_NORMAL | BUTTON_DISABLED);

            if(Allow_Phase_Distortion_OSC1 || Pos_Tbl_Synth_OSC1 != 0)
            {
                Gui_Draw_Button_Box(272, (Cur_Height - 113) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(254, (Cur_Height - 131) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box( 24, (Cur_Height - 113) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(190, (Cur_Height - 113) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            else
            {
                Gui_Draw_Button_Box(272, (Cur_Height - 113) - 1, 16, 16, "C", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(254, (Cur_Height - 131) - 1, 34, 16, "Rand", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box( 24, (Cur_Height - 113) - 1, 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(190, (Cur_Height - 113) - 1, 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_OSC2_PARAMS)
        {
            if(Pos_Tbl_Synth_OSC2 < 0) Pos_Tbl_Synth_OSC2 = 0;
            if(Pos_Tbl_Synth_OSC2 > Size_Tbl_Synth_OSC2) Pos_Tbl_Synth_OSC2 = Size_Tbl_Synth_OSC2;
            Gui_Draw_Arrows_Number_Box2(62, (Cur_Height - 77) - 1, Pos_Tbl_Synth_OSC2 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(62 + 62, (Cur_Height - 77) - 1, 128, 16, Names_Tbl_Synth_OSC2[Pos_Tbl_Synth_OSC2], BUTTON_NORMAL | BUTTON_DISABLED);

            if(Allow_Phase_Distortion_OSC2 || Pos_Tbl_Synth_OSC2 != 0)
            {
                Gui_Draw_Button_Box(272, (Cur_Height - 59) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(254, (Cur_Height - 77) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box( 24, (Cur_Height - 59) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(190, (Cur_Height - 59) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            else
            {
                Gui_Draw_Button_Box(272, (Cur_Height - 59) - 1, 16, 16, "C", BUTTON_NORMAL | BUTTON_DISABLED);
                Gui_Draw_Button_Box(254, (Cur_Height - 77) - 1, 34, 16, "Rand", BUTTON_NORMAL | BUTTON_DISABLED);
                Gui_Draw_Button_Box( 24, (Cur_Height - 59) - 1, 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(190, (Cur_Height - 59) - 1, 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }

            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_VCF_PARAMS)
        {
            if(Pos_Tbl_Synth_VCF < 0) Pos_Tbl_Synth_VCF = 0;
            if(Pos_Tbl_Synth_VCF > Size_Tbl_Synth_VCF) Pos_Tbl_Synth_VCF = Size_Tbl_Synth_VCF;
            Gui_Draw_Arrows_Number_Box2(331, (Cur_Height - 131) - 1, Pos_Tbl_Synth_VCF + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(331 + 62, (Cur_Height - 131) - 1, 111, 16, Names_Tbl_Synth_VCF[Pos_Tbl_Synth_VCF], BUTTON_NORMAL | BUTTON_DISABLED);
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_LFO1_PARAMS)
        {
            if(Pos_Tbl_Synth_LFO1 < 0) Pos_Tbl_Synth_LFO1 = 0;
            if(Pos_Tbl_Synth_LFO1 > Size_Tbl_Synth_LFO1) Pos_Tbl_Synth_LFO1 = Size_Tbl_Synth_LFO1;
            Gui_Draw_Arrows_Number_Box2(331, (Cur_Height - 95) - 1, Pos_Tbl_Synth_LFO1 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(331 + 62, (Cur_Height - 95) - 1, 111, 16, Names_Tbl_Synth_LFO1[Pos_Tbl_Synth_LFO1], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO1 == 1) ||
               !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO1 == 2))
            {
                Gui_Draw_Button_Box(506, (Cur_Height - 95) - 1, 34, 16, "Rand", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(524, (Cur_Height - 77) - 1, 16, 16, "C", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(294, (Cur_Height - 77) - 1, 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(460, (Cur_Height - 77) - 1, 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            else
            {
                Gui_Draw_Button_Box(506, (Cur_Height - 95) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(524, (Cur_Height - 77) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(294, (Cur_Height - 77) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(460, (Cur_Height - 77) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_LFO2_PARAMS)
        {
            if(Pos_Tbl_Synth_LFO2 < 0) Pos_Tbl_Synth_LFO2 = 0;
            if(Pos_Tbl_Synth_LFO2 > Size_Tbl_Synth_LFO2) Pos_Tbl_Synth_LFO2 = Size_Tbl_Synth_LFO2;
            Gui_Draw_Arrows_Number_Box2(331, (Cur_Height - 59) - 1, Pos_Tbl_Synth_LFO2 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(331 + 62, (Cur_Height - 59) - 1, 111, 16, Names_Tbl_Synth_LFO2[Pos_Tbl_Synth_LFO2], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO2 == 1) ||
               !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO2 == 2))
            {
                Gui_Draw_Button_Box(506, (Cur_Height - 59) - 1, 34, 16, "Rand", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(524, (Cur_Height - 41) - 1, 16, 16, "C", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(294, (Cur_Height - 41) - 1, 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(460, (Cur_Height - 41) - 1, 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            else
            {
                Gui_Draw_Button_Box(506, (Cur_Height - 59) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(524, (Cur_Height - 41) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(294, (Cur_Height - 41) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(460, (Cur_Height - 41) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_ENV1_PARAMS)
        {
            if(Pos_Tbl_Synth_ENV1 < 0) Pos_Tbl_Synth_ENV1 = 0;
            if(Pos_Tbl_Synth_ENV1 > Size_Tbl_Synth_ENV1) Pos_Tbl_Synth_ENV1 = Size_Tbl_Synth_ENV1;
            Gui_Draw_Arrows_Number_Box2(583, (Cur_Height - 131) - 1, Pos_Tbl_Synth_ENV1 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(583 + 62, (Cur_Height - 131) - 1, 111, 16, Names_Tbl_Synth_ENV1[Pos_Tbl_Synth_ENV1], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV1 == 0) ||
               !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV1 == 1))
            {
                Gui_Draw_Button_Box(758, (Cur_Height - 131) - 1, 34, 16, "Rand", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(776, (Cur_Height - 113) - 1, 16, 16, "C", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(546, (Cur_Height - 113) - 1, 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(712, (Cur_Height - 113) - 1, 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            else
            {
                Gui_Draw_Button_Box(758, (Cur_Height - 131) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(776, (Cur_Height - 113) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(546, (Cur_Height - 113) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(712, (Cur_Height - 113) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_ENV2_PARAMS)
        {
            if(Pos_Tbl_Synth_ENV2 < 0) Pos_Tbl_Synth_ENV2 = 0;
            if(Pos_Tbl_Synth_ENV2 > Size_Tbl_Synth_ENV2) Pos_Tbl_Synth_ENV2 = Size_Tbl_Synth_ENV2;
            Gui_Draw_Arrows_Number_Box2(583, (Cur_Height - 95) - 1, Pos_Tbl_Synth_ENV2 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(583 + 62, (Cur_Height - 95) - 1, 111, 16, Names_Tbl_Synth_ENV2[Pos_Tbl_Synth_ENV2], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV2 == 0) ||
               !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV2 == 1))
            {
                Gui_Draw_Button_Box(758, (Cur_Height - 95) - 1, 34, 16, "Rand", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(776, (Cur_Height - 77) - 1, 16, 16, "C", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(546, (Cur_Height - 77) - 1, 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(712, (Cur_Height - 77) - 1, 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            else
            {
                Gui_Draw_Button_Box(758, (Cur_Height - 95) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(776, (Cur_Height - 77) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(546, (Cur_Height - 77) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(712, (Cur_Height - 77) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_Misc_PARAMS)
        {
            if(Pos_Tbl_Synth_Misc < 0) Pos_Tbl_Synth_Misc = 0;
            if(Pos_Tbl_Synth_Misc > Size_Tbl_Synth_Misc) Pos_Tbl_Synth_Misc = Size_Tbl_Synth_Misc;
            Gui_Draw_Arrows_Number_Box2(583, (Cur_Height - 59) - 1, Pos_Tbl_Synth_Misc + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(583 + 62, (Cur_Height - 59) - 1, 111, 16, Names_Tbl_Synth_Misc[Pos_Tbl_Synth_Misc], BUTTON_NORMAL | BUTTON_DISABLED);

            if((Allow_Phase_Distortion_OSC1 || Allow_Phase_Distortion_OSC2) ||
                Pos_Tbl_Synth_Misc > 0)
            {
                Gui_Draw_Button_Box(758, (Cur_Height - 59) - 1, 34, 16, "Rand", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(776, (Cur_Height - 41) - 1, 16, 16, "C", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(546, (Cur_Height - 41) - 1, 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(712, (Cur_Height - 41) - 1, 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            else
            {
                Gui_Draw_Button_Box(758, (Cur_Height - 59) - 1, 34, 16, "Rand", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(776, (Cur_Height - 41) - 1, 16, 16, "C", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(546, (Cur_Height - 41) - 1, 16, 16, "\03", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
                Gui_Draw_Button_Box(712, (Cur_Height - 41) - 1, 16, 16, "\04", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            }
            Actualize_SynthParSlider();
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_OSC3_SWITCH ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            if(PARASynth[Current_Instrument].osc3_switch)
            {
                Gui_Draw_Button_Box(237, (Cur_Height - 95) - 1, 24, 16, "On", Allow_Button_Pushed | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(264, (Cur_Height - 95) - 1, 24, 16, "Off", Allow_Button | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(237, (Cur_Height - 95) - 1, 24, 16, "On", Allow_Button | BUTTON_TEXT_CENTERED);
                Gui_Draw_Button_Box(264, (Cur_Height - 95) - 1, 24, 16, "Off", Allow_Button_Pushed | BUTTON_TEXT_CENTERED);
            }
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_VCF_TYPE ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            Gui_Draw_Button_Box(349, (Cur_Height - 150), 16, 16, "\03", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(349 + 84, (Cur_Height - 150), 16, 16, "\04", Allow_Button | BUTTON_TEXT_CENTERED);
            switch(PARASynth[Current_Instrument].vcf_type)
            {
                case 0:
                    Gui_Draw_Button_Box(349 + 18, (Cur_Height - 150), 64, 16, "Lo", Allow_Button | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                case 1:
                    Gui_Draw_Button_Box(349 + 18, (Cur_Height - 150), 64, 16, "Hi", Allow_Button | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                case 2:
                    Gui_Draw_Button_Box(349 + 18, (Cur_Height - 150), 64, 16, "N/A", Allow_Button | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                case 3:
                    Gui_Draw_Button_Box(349 + 18, (Cur_Height - 150), 64, 16, "Moog Lo", Allow_Button | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
                case 4:
                    Gui_Draw_Button_Box(349 + 18, (Cur_Height - 150), 64, 16, "Moog Band", Allow_Button | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
                    break;
            }
        }
    }
}

void Check_Slider(int *Value, int x, int y)
{
    if(zcheckMouse(x - 2, y, 148, 16))
    {
        *Value = Mouse.x - ((x - 2) + 10);
        if(*Value < 0) *Value = 0;
        if(*Value > 128) *Value = 128;
        teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
    }
}

void Mouse_Sliders_Synth_Ed(void)
{
    if(userscreen == USER_SCREEN_SYNTH_EDIT && Allow_All)
    {
        if(Allow_Phase_Distortion_OSC1 || Pos_Tbl_Synth_OSC1 != 0)
        {
            Check_Slider(&csynth_slv_OSC1, 43, (Cur_Height - 113) - 1);
        }
        if(Allow_Phase_Distortion_OSC2 || Pos_Tbl_Synth_OSC2 != 0)
        {
            Check_Slider(&csynth_slv_OSC2, 43, (Cur_Height - 59) - 1);
        }

        Check_Slider(&csynth_slv_VCF, 313, (Cur_Height - 113) - 1);

        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO1 == 1) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO1 == 2))
        {
        }
        else
        {
            Check_Slider(&csynth_slv_LFO1, 313, (Cur_Height - 77) - 1);
        }

        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO2 == 1) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO2 == 2))
        {
        }
        else
        {
            Check_Slider(&csynth_slv_LFO2, 313, (Cur_Height - 41) - 1);
        }

        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV1 == 0) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV1 == 1)) {
        }
        else
        {
            Check_Slider(&csynth_slv_ENV1, 565, (Cur_Height - 113) - 1);
        }

        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV2 == 0) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV2 == 1))
        {
        }
        else
        {
            Check_Slider(&csynth_slv_ENV2, 565, (Cur_Height - 77) - 1);
        }

        if((Allow_Phase_Distortion_OSC1 || Allow_Phase_Distortion_OSC2) ||
            Pos_Tbl_Synth_Misc > 0)
        {
            Check_Slider(&csynth_slv_Misc, 565, (Cur_Height - 41) - 1);
        }
    }
}

void Check_Sliders_Arrows(int value_step)
{
    // OSC1 Sliders Arrows
    if(zcheckMouse(24, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC1 || Pos_Tbl_Synth_OSC1 != 0)
        {
            csynth_slv_OSC1 -= value_step;
            if(csynth_slv_OSC1 < 0) csynth_slv_OSC1 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    if(zcheckMouse(190, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC1 || Pos_Tbl_Synth_OSC1 != 0)
        {
            csynth_slv_OSC1 += value_step;
            if(csynth_slv_OSC1 > 128) csynth_slv_OSC1 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // OSC2 Sliders Arrows
    if(zcheckMouse(24, (Cur_Height - 59) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC2 || Pos_Tbl_Synth_OSC2 != 0)
        {
            csynth_slv_OSC2 -= value_step;
            if(csynth_slv_OSC2 < 0) csynth_slv_OSC2 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    if(zcheckMouse(190, (Cur_Height - 59) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC2 || Pos_Tbl_Synth_OSC2 != 0)
        {
            csynth_slv_OSC2 += value_step;
            if(csynth_slv_OSC2 > 128) csynth_slv_OSC2 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // VCF Sliders Arrows
    if(zcheckMouse(294, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        csynth_slv_VCF -= value_step;
        if(csynth_slv_VCF < 0) csynth_slv_VCF = 0;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
    }
    if(zcheckMouse(460, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        csynth_slv_VCF += value_step;
        if(csynth_slv_VCF > 128) csynth_slv_VCF = 128;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
    }

    // LFO1 Sliders Arrows
    if(zcheckMouse(294, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO1 == 1) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO1 == 2))
        {
        }
        else
        {
            csynth_slv_LFO1 -= value_step;
            if(csynth_slv_LFO1 < 0) csynth_slv_LFO1 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    if(zcheckMouse(460, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO1 == 1) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO1 == 2))
        {
        }
        else
        {
            csynth_slv_LFO1 += value_step;
            if(csynth_slv_LFO1 > 128) csynth_slv_LFO1 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // LFO2 Sliders Arrows
    if(zcheckMouse(294, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO2 == 1) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO2 == 2))
        {
        }
        else
        {
            csynth_slv_LFO2 -= value_step;
            if(csynth_slv_LFO2 < 0) csynth_slv_LFO2 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    
    if(zcheckMouse(460, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO2 == 1) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO2 == 2))
        {
        }
        else
        {
            csynth_slv_LFO2 += value_step;
            if(csynth_slv_LFO2 > 128) csynth_slv_LFO2 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // ENV1 Sliders Arrows
    if(zcheckMouse(546, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV1 == 0) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV1 == 1))
        {
        }
        else
        {
            csynth_slv_ENV1 -= value_step;
            if(csynth_slv_ENV1 < 0) csynth_slv_ENV1 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    if(zcheckMouse(712, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV1 == 0) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV1 == 1))
        {
        }
        else
        {
            csynth_slv_ENV1 += value_step;
            if(csynth_slv_ENV1 > 128) csynth_slv_ENV1 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // ENV2 Sliders Arrows
    if(zcheckMouse(546, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV2 == 0) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV2 == 1))
        {
        }
        else
        {
            csynth_slv_ENV2 -= value_step;
            if(csynth_slv_ENV2 < 0) csynth_slv_ENV2 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    
    if(zcheckMouse(712, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV2 == 0) ||
           !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV2 == 1))
        {
        }
        else
        {
            csynth_slv_ENV2 += value_step;
            if(csynth_slv_ENV2 > 128) csynth_slv_ENV2 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // Misc. Sliders Arrows
    if(zcheckMouse(546, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if((Allow_Phase_Distortion_OSC1 || Allow_Phase_Distortion_OSC2) ||
            Pos_Tbl_Synth_Misc > 0)
        {
            csynth_slv_Misc -= value_step;
            if(csynth_slv_Misc < 0) csynth_slv_Misc = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    
    if(zcheckMouse(712, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if((Allow_Phase_Distortion_OSC1 || Allow_Phase_Distortion_OSC2) ||
            Pos_Tbl_Synth_Misc > 0)
        {
            csynth_slv_Misc += value_step;
            if(csynth_slv_Misc > 128) csynth_slv_Misc = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
}

void Check_Parameters_Arrows(int Value_Step)
{
    // Select OSC1 parameter
    if(zcheckMouse(62, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_OSC1 > 0)
    {
        Pos_Tbl_Synth_OSC1 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC1_PARAMS;
    }
    if(zcheckMouse(62 + 44, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_OSC1 < Size_Tbl_Synth_OSC1)
    {
        Pos_Tbl_Synth_OSC1 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC1_PARAMS;
    }  

    // Select OSC2 parameter
    if(zcheckMouse(62, (Cur_Height - 77) - 1, 16, 16) && Pos_Tbl_Synth_OSC2 > 0)
    {
        Pos_Tbl_Synth_OSC2 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC2_PARAMS;
    }
    if(zcheckMouse(62 + 44, (Cur_Height - 77) - 1, 16, 16) && Pos_Tbl_Synth_OSC2 < Size_Tbl_Synth_OSC2)
    {
        Pos_Tbl_Synth_OSC2 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC2_PARAMS;
    }  

    // Select VCF parameter
    if(zcheckMouse(331, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_VCF > 0)
    {
        Pos_Tbl_Synth_VCF -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VCF_PARAMS;
    }
    if(zcheckMouse(331 + 44, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_VCF < Size_Tbl_Synth_VCF)
    {
        Pos_Tbl_Synth_VCF += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VCF_PARAMS;
    }  

    // Select LFO1 parameter
    if(zcheckMouse(331, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_LFO1 > 0)
    {
        Pos_Tbl_Synth_LFO1 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO1_PARAMS;
    }
    if(zcheckMouse(331 + 44, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_LFO1 < Size_Tbl_Synth_LFO1)
    {
        Pos_Tbl_Synth_LFO1 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO1_PARAMS;
    }  

    // Select LFO2 parameter
    if(zcheckMouse(331, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_LFO2 > 0)
    {
        Pos_Tbl_Synth_LFO2 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO2_PARAMS;
    }
    if(zcheckMouse(331 + 44, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_LFO2 < Size_Tbl_Synth_LFO2)
    {
        Pos_Tbl_Synth_LFO2 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO2_PARAMS;
    }

    // Select ENV1 parameter
    if(zcheckMouse(583, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_ENV1 > 0)
    {
        Pos_Tbl_Synth_ENV1 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV1_PARAMS;
    }
    if(zcheckMouse(583 + 44, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_ENV1 < Size_Tbl_Synth_ENV1)
    {
        Pos_Tbl_Synth_ENV1 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV1_PARAMS;
    }  

    // Select ENV2 parameter
    if(zcheckMouse(583, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_ENV2 > 0)
    {
        Pos_Tbl_Synth_ENV2 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV2_PARAMS;
    }
    if(zcheckMouse(583 + 44, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_ENV2 < Size_Tbl_Synth_ENV2)
    {
        Pos_Tbl_Synth_ENV2 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV2_PARAMS;
    }  

    // Select Misc parameter
    if(zcheckMouse(583, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_Misc > 0)
    {
        Pos_Tbl_Synth_Misc -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_Misc_PARAMS;
    }
    if(zcheckMouse(583 + 44, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_Misc < Size_Tbl_Synth_Misc)
    {
        Pos_Tbl_Synth_Misc += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_Misc_PARAMS;
    }  
}

void Mouse_Right_Synth_Ed(void)
{
    if(userscreen == USER_SCREEN_SYNTH_EDIT)
    {

        if(zcheckMouse(228, (Cur_Height - 150), 17, 16))
        {
            if(Synthprg[Current_Instrument] >= 10)
            {
                Synthprg[Current_Instrument] -= 10;
            }
            else
            {
                Synthprg[Current_Instrument] = 0;
            }
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(zcheckMouse(228 + 44, (Cur_Height - 150), 17, 16))
        {
            if(Synthprg[Current_Instrument] <= (129 - 10))
            {
                Synthprg[Current_Instrument] += 10;
            }
            else
            {
                Synthprg[Current_Instrument] = 129;
            }
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
    }

    if(userscreen == USER_SCREEN_SYNTH_EDIT && Allow_All)
    {
        Check_Sliders_Arrows(10);
        Check_Parameters_Arrows(10);
    }
}

void Mouse_Left_Synth_Ed(void)
{
    if(userscreen == USER_SCREEN_SYNTH_EDIT)
    {
        if(zcheckMouse(228, (Cur_Height - 150), 17, 16))
        {
            if(Synthprg[Current_Instrument])
            {
                Synthprg[Current_Instrument]--;
            }
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(zcheckMouse(228 + 44, (Cur_Height - 150), 17, 16))
        {
            if(Synthprg[Current_Instrument] < 129)
            {
                Synthprg[Current_Instrument]++;
            }
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        }

        if(userscreen == USER_SCREEN_SYNTH_EDIT && Allow_All)
        {
            // Save the data
            if(zcheckMouse(758, (Cur_Height - 150), 34, 16))
            {
                if(File_Exist_Req("%s" SLASH "%s.pts", Dir_Presets, PARASynth[Current_Instrument].presetname))
                {
                    Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_SYNTH, NULL, TRUE);
                }
                else
                {
                    gui_action = GUI_CMD_SAVE_SYNTH;
                }
            }

            // Start synth name input
            if(zcheckMouse(592, (Cur_Height - 150), 164, 16) && snamesel == INPUT_NONE)
            {
                snamesel = INPUT_SYNTH_NAME;
                strcpy(cur_input_name, PARASynth[Current_Instrument].presetname);
                namesize = 0;
                sprintf(PARASynth[Current_Instrument].presetname, "");
                teac = UPDATE_SYNTH_CHANGE_NAME;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }

            if(zcheckMouse(254, (Cur_Height - 131) - 1, 34, 16)) Rand_OSC1();
            if(zcheckMouse(254, (Cur_Height - 77) - 1, 34, 16)) Rand_OSC2();
            if(zcheckMouse(506, (Cur_Height - 131) - 1, 34, 16)) Rand_VCF();
            if(zcheckMouse(506, (Cur_Height - 95) - 1, 34, 16)) Rand_LFO1();
            if(zcheckMouse(506, (Cur_Height - 59) - 1, 34, 16)) Rand_LFO2();
            if(zcheckMouse(755, (Cur_Height - 131) - 1, 34, 16)) Rand_ENV1();
            if(zcheckMouse(755, (Cur_Height - 95) - 1, 34, 16)) Rand_ENV2();
            if(zcheckMouse(755, (Cur_Height - 59) - 1, 34, 16)) Rand_Misc();

            Check_Sliders_Arrows(1);

            Check_Parameters_Arrows(1);

            // Center OSC1
            if(zcheckMouse(272, (Cur_Height - 113) - 1, 16, 16))
            {
                if(Allow_Phase_Distortion_OSC1 || Pos_Tbl_Synth_OSC1 != 0)
                {
                    Center_SynthParam_OSC1();
                    gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                    teac = UPDATE_SYNTH_ED_OSC1_PARAMS;
                }
            }  

            // Center OSC2
            if(zcheckMouse(272, (Cur_Height - 59) - 1, 16, 16))
            {
                if(Allow_Phase_Distortion_OSC2 || Pos_Tbl_Synth_OSC2 != 0)
                {
                    Center_SynthParam_OSC2();
                    gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                    teac = UPDATE_SYNTH_ED_OSC2_PARAMS;
                }
            }  

            // Center VCF
            if(zcheckMouse(524, (Cur_Height - 113) - 1, 16, 16))
            {
                Center_SynthParam_VCF();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_VCF_PARAMS;
            }  

            // Center LFO1
            if(zcheckMouse(524, (Cur_Height - 77) - 1, 16, 16))
            {
                if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO1 == 1) ||
                   !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO1 == 2))
                {
                }
                else
                {
                    Center_SynthParam_LFO1();
                    gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                    teac = UPDATE_SYNTH_ED_LFO1_PARAMS;
                }
            }  

            // Center LFO2
            if(zcheckMouse(524, (Cur_Height - 41) - 1, 16, 16))
            {
                if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_LFO2 == 1) ||
                   !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_LFO2 == 2))
                {
                }
                else
                {
                    Center_SynthParam_LFO2();
                    gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                    teac = UPDATE_SYNTH_ED_LFO2_PARAMS;
                }
            }  

            // Center ENV1
            if(zcheckMouse(776, (Cur_Height - 113) - 1, 16, 16))
            {
                if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV1 == 0) ||
                   !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV1 == 1))
                {
                }
                else
                {
                    Center_SynthParam_ENV1();
                    gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                    teac = UPDATE_SYNTH_ED_ENV1_PARAMS;
                }
            }  

            // Center ENV2
            if(zcheckMouse(776, (Cur_Height - 77) - 1, 16, 16))
            {
                if(!Allow_Phase_Distortion_OSC1 && (Pos_Tbl_Synth_ENV2 == 0) ||
                   !Allow_Phase_Distortion_OSC2 && (Pos_Tbl_Synth_ENV2 == 1))
                {
                }
                else
                {
                    Center_SynthParam_ENV2();
                    gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                    teac = UPDATE_SYNTH_ED_ENV2_PARAMS;
                }
            }  

            // Center Misc
            if(zcheckMouse(776, (Cur_Height - 41) - 1, 16, 16))
            {
                if((Allow_Phase_Distortion_OSC1 || Allow_Phase_Distortion_OSC2) ||
                   Pos_Tbl_Synth_Misc > 0)
                {
                    Center_SynthParam_Misc();
                    gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                    teac = UPDATE_SYNTH_ED_Misc_PARAMS;
                }
            }  

            // OSC1 Waveform
            if(zcheckMouse(24, (Cur_Height - 95) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc1_waveform = 0;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(49, (Cur_Height - 95) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc1_waveform = 1;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(74, (Cur_Height - 95) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc1_waveform = 2;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(99, (Cur_Height - 95) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc1_waveform = 3;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(124, (Cur_Height - 95) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc1_waveform = 6;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(149, (Cur_Height - 95) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc1_waveform = 5;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(174, (Cur_Height - 95) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc1_waveform = 4;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }

            // OSC2 Waveform
            if(zcheckMouse(24, (Cur_Height - 41) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc2_waveform = 0;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(49, (Cur_Height - 41) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc2_waveform = 1;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(74, (Cur_Height - 41) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc2_waveform = 2;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(99, (Cur_Height - 41) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc2_waveform = 3;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(124, (Cur_Height - 41) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc2_waveform = 6;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(149, (Cur_Height - 41) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc2_waveform = 5;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(174, (Cur_Height - 41) - 1, 23, 16))
            {
                PARASynth[Current_Instrument].osc2_waveform = 4;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }


            // Oscillators combiner
            if(zcheckMouse(4, (Cur_Height - 95) - 1 + (18 * 0), 17, 16))
            {
                PARASynth[Current_Instrument].osc_combine = COMBINE_ADD;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(4, (Cur_Height - 95) - 1 + (18 * 1), 17, 16))
            {
                PARASynth[Current_Instrument].osc_combine = COMBINE_SUB;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(4, (Cur_Height - 95) - 1 + (18 * 2), 17, 16))
            {
                PARASynth[Current_Instrument].osc_combine = COMBINE_MUL;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(4, (Cur_Height - 95) - 1 + (18 * 3), 17, 16))
            {
                PARASynth[Current_Instrument].osc_combine = COMBINE_DIV;
                teac = UPDATE_SYNTH_ED_ALL;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }

            // Sub oscillator switch
            if(zcheckMouse(237, (Cur_Height - 95) - 1, 24, 16))
            {
                PARASynth[Current_Instrument].osc3_switch = TRUE;
                teac = UPDATE_SYNTH_ED_OSC3_SWITCH;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(264, (Cur_Height - 95) - 1, 24, 16))
            {
                PARASynth[Current_Instrument].osc3_switch = FALSE;
                teac = UPDATE_SYNTH_ED_OSC3_SWITCH;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }

            // VCF Type
            if(zcheckMouse(349, (Cur_Height - 150), 17, 16) && PARASynth[Current_Instrument].vcf_type > 0)
            {
                PARASynth[Current_Instrument].vcf_type = filter_vcf_type_minus[PARASynth[Current_Instrument].vcf_type];
                teac = 7;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
            if(zcheckMouse(349 + 84, (Cur_Height - 150), 17, 16) && PARASynth[Current_Instrument].vcf_type <= 4)
            {
                PARASynth[Current_Instrument].vcf_type = filter_vcf_type_plus[PARASynth[Current_Instrument].vcf_type];
                teac = 7;
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            }
    }
}

void Actualize_SynthParSlider(void)
{

    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    // -----------------------------
    switch(Pos_Tbl_Synth_OSC1)
    {
        case 0: /* Osc1_PD */
            if(Allow_Phase_Distortion_OSC1)
            {
                Realslider(41, (Cur_Height - 113) - 1, Cur_SynthParam->osc1_pw / 4, Allow_All);
            }
            else
            {
                Realslider(41, (Cur_Height - 113) - 1, Cur_SynthParam->osc1_pw / 4, FALSE);
            }
            if(Cur_SynthParam->osc1_pw - 256)
            {
                outfloat_small(208, (Cur_Height - 113) - 1, (((float) Cur_SynthParam->osc1_pw - 256) / 256.0f) * 100.0f, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(208, (Cur_Height - 113) - 1, 62, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);
            }
            csynth_slv_OSC1 = Cur_SynthParam->osc1_pw / 4;
            break;

        case 1: /* ENV1_Attack */
            Realslider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env1_attack / 512, Allow_All);
            outlong_small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env1_attack * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC1 = Cur_SynthParam->env1_attack / 512;
            break;

        case 2: /* ENV1_Decay */
            Realslider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env1_decay / 512, Allow_All);
            outlong_small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env1_decay * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC1 = Cur_SynthParam->env1_decay / 512;
            break;

        case 3: /* ENV1_Sustain */
            Realslider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env1_sustain, Allow_All);
            outlong_small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env1_sustain * 100) / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC1 = Cur_SynthParam->env1_sustain;
            break;

        case 4: /* ENV1_Release */
            Realslider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env1_release / 512, Allow_All);
            outlong_small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env1_release * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC1 = Cur_SynthParam->env1_release / 512;
            break;

        case 5: /* OSC3 volume */
            Realslider(41, (Cur_Height - 113) - 1, Cur_SynthParam->osc3_volume, Allow_All);
            outfloat_small(208, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->osc3_volume - 64.0f) * 1.5625f, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC1 = Cur_SynthParam->osc3_volume;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_OSC2)
    {
        case 0: /* Osc2_PD */
            if(Allow_Phase_Distortion_OSC2)
            {
                Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc2_pw / 4, Allow_All);
            }
            else
            {
                Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc2_pw / 4, FALSE);
            }
            if(Cur_SynthParam->osc2_pw - 256)
            {
                outfloat_small(208, (Cur_Height - 59) - 1, (((float) Cur_SynthParam->osc2_pw - 256) / 256.0f) * 100.0f, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(208, (Cur_Height - 59) - 1, 62, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_OSC2 = Cur_SynthParam->osc2_pw / 4;
            break;

        case 1: /* Osc2_Detune */
            Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc2_detune, Allow_All);
            outlong_small(208, (Cur_Height - 59) - 1, Cur_SynthParam->osc2_detune - 64, 0, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC2 = Cur_SynthParam->osc2_detune;
            break;

        case 2: /* Osc2_Finetune */
            Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc2_finetune, Allow_All);
            outlong_small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->osc2_finetune * 100) / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC2 = Cur_SynthParam->osc2_finetune;
            break;

        case 3: /* ENV2_Attack */
            Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env2_attack / 512, Allow_All);
            outlong_small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env2_attack * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC2 = Cur_SynthParam->env2_attack / 512;
            break;

        case 4: /* ENV2_Decay */
            Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env2_decay / 512, Allow_All);
            outlong_small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env2_decay * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC2 = Cur_SynthParam->env2_decay / 512;
            break;

        case 5: /* ENV2_Sustain */
            Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env2_sustain, Allow_All);
            outlong_small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env2_sustain * 100) / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC2 = Cur_SynthParam->env2_sustain;
            break;

        case 6: /* ENV2_Release */
            Realslider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env2_release / 512, Allow_All);
            outlong_small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env2_release * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC2 = Cur_SynthParam->env2_release / 512;
            break;
    // -----------------------------
    }

    // -----------------------------
    switch(Pos_Tbl_Synth_VCF)
    {
        case 0: /* VCF Cutoff */
            Realslider(311, (Cur_Height - 113) - 1, Cur_SynthParam->vcf_cutoff, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 113) - 1, Cur_SynthParam->vcf_cutoff * 172, 3, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_VCF = Cur_SynthParam->vcf_cutoff;
            break;

        case 1: /* VCF_Resonance */
            Realslider(311, (Cur_Height - 113) - 1, Cur_SynthParam->vcf_resonance, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 113) - 1, (Cur_SynthParam->vcf_resonance * 100) / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_VCF = Cur_SynthParam->vcf_resonance;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_LFO1)
    {
        case 0: /* LFO1_Period */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_period, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, (float) Cur_SynthParam->lfo1_period * 2, 8, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_period;
            break;

        case 1: /* Lfo1->osc1 pd */
            if(Allow_Phase_Distortion_OSC1)
            {
                Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc1_pw, Allow_All);
            }
            else
            {
                Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc1_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_osc1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc1_pw;
            break;

        case 2: /* Lfo1->osc2 pd */
            if(Allow_Phase_Distortion_OSC2)
            {
                Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc2_pw, Allow_All);
            }
            else
            {
                Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc2_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_osc2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc2_pw;
            break;

        case 3: /* Lfo1->osc1 pitch */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc1_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_osc1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc1_pitch;
            break;

        case 4: /* Lfo1->osc1 pitch */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc2_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_osc2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc2_pitch;
            break;

        case 5: /* Lfo1->osc1 volume */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc1_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_osc1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc1_volume;
            break;

        case 6: /* Lfo1->osc2 volume */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_osc2_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_osc2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc2_volume;
            break;

        case 7: /* Lfo1->vcf cutoff */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_vcf_cutoff, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_vcf_cutoff;
            break;

        case 8: /* Lfo1->vcf resonance */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_vcf_resonance, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo1_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_vcf_resonance;
            break;

        case 9: /* LFO1_Attack */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_attack / 512, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo1_attack * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_attack / 512;
            break;

        case 10: /* LFO1_Decay */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_decay / 512, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo1_decay * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_decay / 512;
            break;

        case 11: /* LFO1_Sustain */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_sustain, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo1_sustain * 100) / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_sustain;
            break;

        case 12: /* LFO1_Release */
            Realslider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo1_release / 512, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo1_release * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_release / 512;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_LFO2)
    {
        case 0: /* LFO2_Period */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_period, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, (float) Cur_SynthParam->lfo2_period * 2, 8, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_period;
            break;

        case 1: /* Lfo2->osc1 pd */
            if(Allow_Phase_Distortion_OSC1)
            {
                Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc1_pw, Allow_All);
            }
            else
            {
                Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc1_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_osc1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc1_pw;
            break;

        case 2: /* Lfo2->osc2 pd */
            if(Allow_Phase_Distortion_OSC2)
            {
                Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc2_pw, Allow_All);
            }
            else
            {
                Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc2_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_osc2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc2_pw;
            break;

        case 3: /* Lfo2->osc1 pitch */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc1_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_osc1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc1_pitch;
            break;

        case 4: /* Lfo2->osc2 pitch */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc2_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_osc2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc2_pitch;
            break;

        case 5: /* Lfo2->osc1 volume */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc1_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_osc1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc1_volume;
            break;

        case 6: /* Lfo2->osc2 volume */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_osc2_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_osc2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc2_volume;
            break;

        case 7: /* Lfo2->vcf cutoff */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_vcf_cutoff, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_vcf_cutoff;
            break;

        case 8: /* Lfo2->vcf resonance */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_vcf_resonance, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo2_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_vcf_resonance;
            break;

        case 9: /* LFO2_Attack */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_attack / 512, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo2_attack * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_attack / 512;
            break;

        case 10: /* LFO2_Decay */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_decay / 512, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo2_decay * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_decay / 512;
            break;

        case 11: /* LFO2_Sustain */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_sustain, FALSE);
            outlong_small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo2_sustain * 100) / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_sustain;
            break;

        case 12: /* LFO2_Release */
            Realslider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo2_release / 512, Allow_All);
            outlong_small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo2_release * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_release / 512;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_ENV1)
    {
        case 0: /* Env1->osc1 pd */
            if(Allow_Phase_Distortion_OSC1)
            {
                Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc1_pw, Allow_All);
            }
            else
            {
                Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc1_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_osc1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc1_pw;
            break;

        case 1: /* Env1->osc2 pd */
            if(Allow_Phase_Distortion_OSC2)
            {
                Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc2_pw, Allow_All);
            }
            else
            {
                Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc2_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_osc2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc2_pw;
            break;

        case 2: /* Env1->osc1 pitch */
            Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc1_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_osc1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc1_pitch;
            break;

        case 3: /* Env1->osc2 pitch */
            Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc2_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_osc2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc2_pitch;
            break;

        case 4: /* Env1->osc1 volume */
            Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc1_volume, Allow_All);
            if(Cur_SynthParam->env1_osc1_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_osc1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 113) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc1_volume;
            break;

        case 5: /* Env1->osc2 volume */
            Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_osc2_volume, Allow_All);
            if(Cur_SynthParam->env1_osc2_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_osc2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 113) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc2_volume;
            break;

        case 6: /* Env1->vcf cutoff */
            Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_vcf_cutoff, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV1 = Cur_SynthParam->env1_vcf_cutoff;
            break;

        case 7: /* Env1->vcf resonance */
            Realslider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env1_vcf_resonance, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env1_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV1 = Cur_SynthParam->env1_vcf_resonance;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_ENV2)
    {
        case 0: /* Env2->osc1 pw */
            if(Allow_Phase_Distortion_OSC1)
            {
                Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc1_pw, Allow_All);
            }
            else
            {
                Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc1_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_osc1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc1_pw;
            break;

        case 1: /* Env2->osc2 pw */
            if(Allow_Phase_Distortion_OSC2)
            {
                Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc2_pw, Allow_All);
            }
            else
            {
                Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc2_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_osc2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc2_pw;
            break;

        case 2: /* Env2->osc1 pitch */
            Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc1_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_osc1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc1_pitch;
            break;

        case 3: /* Env2->osc2 pitch */
            Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc2_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_osc2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc2_pitch;
            break;

        case 4: /* Env2->osc1 volume */
            Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc1_volume, Allow_All);
            if(Cur_SynthParam->env2_osc1_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_osc1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 77) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc1_volume;
            break;

        case 5: /* Env2->osc2 volume */
            Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_osc2_volume, Allow_All);
            if(Cur_SynthParam->env2_osc2_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_osc2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 77) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc2_volume;
            break;

        case 6: /* Env2->vcf cutoff */
            Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_vcf_cutoff, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV2 = Cur_SynthParam->env2_vcf_cutoff;
            break;

        case 7: /* Env2->vcf resonance */
            Realslider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env2_vcf_resonance, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env2_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV2 = Cur_SynthParam->env2_vcf_resonance;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_Misc)
    {
        case 0: /* Misc. glide */
            if((Allow_Phase_Distortion_OSC1 || Allow_Phase_Distortion_OSC2) ||
               Pos_Tbl_Synth_Misc > 0)
            {
                Realslider(563, (Cur_Height - 41) - 1, Cur_SynthParam->ptc_glide, Allow_All);
            }
            else
            {
                Realslider(563, (Cur_Height - 41) - 1, Cur_SynthParam->ptc_glide, FALSE);
            }
            if(Cur_SynthParam->ptc_glide)
            {
                outfloat_small(563 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->ptc_glide) * 0.78125f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 41) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_Misc = Cur_SynthParam->ptc_glide;
            break;

        case 1: /* Misc. Global volume */
            Realslider(563, (Cur_Height - 41) - 1, Cur_SynthParam->glb_volume, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->glb_volume) * 0.78125f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_Misc = Cur_SynthParam->glb_volume;
            break;

        case 2: /* Misc. Distortion */
            Realslider(563, (Cur_Height - 41) - 1, Cur_SynthParam->disto, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 41) - 1, (((float) Cur_SynthParam->disto)) * 0.78125f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_Misc = Cur_SynthParam->disto;
            break;
    }
    // -----------------------------
}

void Center_SynthParam_OSC1(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_OSC1)
    {
        case 0: /* Osc1_PD */
            Cur_SynthParam->osc1_pw = 64 * 4;
            csynth_slv_OSC1 = Cur_SynthParam->osc1_pw / 4;
            break;

        case 1: /* ENV1_Attack */
            Cur_SynthParam->env1_attack = 64 * 512;
            csynth_slv_OSC1 = Cur_SynthParam->env1_attack / 512;
            break;

        case 2: /* ENV1_Decay */
            Cur_SynthParam->env1_decay = 64 * 512;
            csynth_slv_OSC1 = Cur_SynthParam->env1_decay / 512;
            break;

        case 3: /* ENV1_Sustain */
            Cur_SynthParam->env1_sustain = 64;
            csynth_slv_OSC1 = Cur_SynthParam->env1_sustain;
            break;

        case 4: /* ENV1_Release */
            Cur_SynthParam->env1_release = 64 * 512;
            csynth_slv_OSC1 = Cur_SynthParam->env1_release / 512;
            break;

        case 5: /* OSC3 volume */
            Cur_SynthParam->osc3_volume = 64;
            csynth_slv_OSC1 = Cur_SynthParam->osc3_volume;
            break;
    }
}

void Center_SynthParam_OSC2(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_OSC2)
    {
        case 0: /* Osc2_PD */
            Cur_SynthParam->osc2_pw = 64 * 4;
            csynth_slv_OSC2 = Cur_SynthParam->osc2_pw / 4;
            break;

        case 1: /* Osc2_Detune */
            Cur_SynthParam->osc2_detune = 64;
            csynth_slv_OSC2 = Cur_SynthParam->osc2_detune;
            break;

        case 2: /* Osc2_Finetune */
            Cur_SynthParam->osc2_finetune = 64;
            csynth_slv_OSC2 = Cur_SynthParam->osc2_finetune;
            break;

        case 3: /* ENV2_Attack */
            Cur_SynthParam->env2_attack = 64 * 512;
            csynth_slv_OSC2 = Cur_SynthParam->env2_attack / 512;
            break;

        case 4: /* ENV2_Decay */
            Cur_SynthParam->env2_decay = 64 * 512;
            csynth_slv_OSC2 = Cur_SynthParam->env2_decay / 512;
            break;

        case 5: /* ENV2_Sustain */
            Cur_SynthParam->env2_sustain = 64;
            csynth_slv_OSC2 = Cur_SynthParam->env2_sustain;
            break;

        case 6: /* ENV2_Release */
            Cur_SynthParam->env2_release = 64 * 512;
            csynth_slv_OSC2 = Cur_SynthParam->env2_release / 512;
            break;
    }
}

void Center_SynthParam_VCF(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_VCF)
    {
        case 0: /* VCF Cutoff */
            Cur_SynthParam->vcf_cutoff = 64;
            csynth_slv_VCF = Cur_SynthParam->vcf_cutoff;
            break;

        case 1: /* VCF_Resonance */
            Cur_SynthParam->vcf_resonance = 64;
            csynth_slv_VCF = Cur_SynthParam->vcf_resonance;
            break;
    }
}

void Center_SynthParam_LFO1(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_LFO1)
    {
        case 0: /* LFO1_Period */
            Cur_SynthParam->lfo1_period = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_period;
            break;

        case 1: /* Lfo1->osc1 pd */
            Cur_SynthParam->lfo1_osc1_pw = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc1_pw;
            break;

        case 2: /* Lfo1->osc2 pd */
            Cur_SynthParam->lfo1_osc2_pw = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc2_pw;
            break;

        case 3: /* Lfo1->osc1 pitch */
            Cur_SynthParam->lfo1_osc1_pitch = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc1_pitch;
            break;

        case 4: /* Lfo1->osc1 pitch */
            Cur_SynthParam->lfo1_osc2_pitch = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc2_pitch;
            break;

        case 5: /* Lfo1->osc1 volume */
            Cur_SynthParam->lfo1_osc1_volume = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc1_volume;
            break;

        case 6: /* Lfo1->osc2 volume */
            Cur_SynthParam->lfo1_osc2_volume = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_osc2_volume;
            break;

        case 7: /* Lfo1->vcf cutoff */
            Cur_SynthParam->lfo1_vcf_cutoff = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_vcf_cutoff;
            break;

        case 8: /* Lfo1->vcf resonance */
            Cur_SynthParam->lfo1_vcf_resonance = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_vcf_resonance;
            break;

        case 9: /* LFO1_Attack */
            Cur_SynthParam->lfo1_attack = 64 * 512;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_attack / 512;
            break;

        case 10: /* LFO1_Decay */
            Cur_SynthParam->lfo1_decay = 64 * 512;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_decay / 512;
            break;

        case 11: /* LFO1_Sustain */
            Cur_SynthParam->lfo1_sustain = 64;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_sustain;
            break;

        case 12: /* LFO1_Release */
            Cur_SynthParam->lfo1_release = 64 * 512;
            csynth_slv_LFO1 = Cur_SynthParam->lfo1_release / 512;
            break;
    }
}

void Center_SynthParam_LFO2(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_LFO2)
    {
        case 0: /* LFO2_Period */
            Cur_SynthParam->lfo2_period = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_period;
            break;

        case 1: /* Lfo2->osc1 pd */
            Cur_SynthParam->lfo2_osc1_pw = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc1_pw;
            break;

        case 2: /* Lfo2->osc2 pd */
            Cur_SynthParam->lfo2_osc2_pw = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc2_pw;
            break;

        case 3: /* Lfo2->osc1 pitch */
            Cur_SynthParam->lfo2_osc1_pitch = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc1_pitch;
            break;

        case 4: /* Lfo2->osc2 pitch */
            Cur_SynthParam->lfo2_osc2_pitch = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc2_pitch;
            break;

        case 5: /* Lfo2->osc1 volume */
            Cur_SynthParam->lfo2_osc1_volume = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc1_volume;
            break;

        case 6: /* Lfo2->osc2 volume */
            Cur_SynthParam->lfo2_osc2_volume = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_osc2_volume;
            break;

        case 7: /* Lfo2->vcf cutoff */
            Cur_SynthParam->lfo2_vcf_cutoff = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_vcf_cutoff;
            break;

        case 8: /* Lfo2->vcf resonance */
            Cur_SynthParam->lfo2_vcf_resonance = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_vcf_resonance;
            break;

        case 9: /* LFO2_Attack */
            Cur_SynthParam->lfo2_attack = 64 * 512;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_attack / 512;
            break;

        case 10: /* LFO2_Decay */
            Cur_SynthParam->lfo2_decay = 64 * 512;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_decay / 512;
            break;

        case 11: /* LFO2_Sustain */
            Cur_SynthParam->lfo2_sustain = 64;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_sustain;
            break;

        case 12: /* LFO2_Release */
            Cur_SynthParam->lfo2_release = 64 * 512;
            csynth_slv_LFO2 = Cur_SynthParam->lfo2_release / 512;
            break;
    }
}

void Center_SynthParam_ENV1(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_ENV1)
    {
        case 0: /* Env1->osc1 pd */
            Cur_SynthParam->env1_osc1_pw = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc1_pw;
            break;

        case 1: /* Env1->osc2 pd */
            Cur_SynthParam->env1_osc2_pw = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc2_pw;
            break;

        case 2: /* Env1->osc1 pitch */
            Cur_SynthParam->env1_osc1_pitch = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc1_pitch;
            break;

        case 3: /* Env1->osc2 pitch */
            Cur_SynthParam->env1_osc2_pitch = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc2_pitch;
            break;

        case 4: /* Env1->osc1 volume */
            Cur_SynthParam->env1_osc1_volume = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc1_volume;
            break;

        case 5: /* Env1->osc2 volume */
            Cur_SynthParam->env1_osc2_volume = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_osc2_volume;
            break;

        case 6: /* Env1->vcf cutoff */
            Cur_SynthParam->env1_vcf_cutoff = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_vcf_cutoff;
            break;

        case 7: /* Env1->vcf resonance */
            Cur_SynthParam->env1_vcf_resonance = 64;
            csynth_slv_ENV1 = Cur_SynthParam->env1_vcf_resonance;
            break;
    }
}

void Center_SynthParam_ENV2(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_ENV2)
    {
        case 0: /* Env2->osc1 pd */
            Cur_SynthParam->env2_osc1_pw = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc1_pw;
            break;

        case 1: /* Env2->osc2 pd */
            Cur_SynthParam->env2_osc2_pw = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc2_pw;
            break;

        case 2: /* Env2->osc1 pitch */
            Cur_SynthParam->env2_osc1_pitch = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc1_pitch;
            break;

        case 3: /* Env2->osc2 pitch */
            Cur_SynthParam->env2_osc2_pitch = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc2_pitch;
            break;

        case 4: /* Env2->osc1 volume */
            Cur_SynthParam->env2_osc1_volume = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc1_volume;
            break;

        case 5: /* Env2->osc2 volume */
            Cur_SynthParam->env2_osc2_volume = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_osc2_volume;
            break;

        case 6: /* Env2->vcf cutoff */
            Cur_SynthParam->env2_vcf_cutoff = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_vcf_cutoff;
            break;

        case 7: /* Env2->vcf resonance */
            Cur_SynthParam->env2_vcf_resonance = 64;
            csynth_slv_ENV2 = Cur_SynthParam->env2_vcf_resonance;
            break;
    }
}

void Center_SynthParam_Misc(void)
{
    SynthParameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_Misc)
    {
        case 0: /* Misc. glide */
            Cur_SynthParam->ptc_glide = 64;
            csynth_slv_Misc = Cur_SynthParam->ptc_glide;
            break;

        case 1: /* Misc. Global volume */
            Cur_SynthParam->glb_volume = 64;
            csynth_slv_Misc = Cur_SynthParam->glb_volume;
            break;

        case 2: /* Misc. Distortion */
            Cur_SynthParam->disto = 64;
            csynth_slv_Misc = Cur_SynthParam->disto;
            break;
    }
}

void CParcha_OSC1(int cpar)
{
    switch(Pos_Tbl_Synth_OSC1)
    {
        case 0: /* Osc1_PD */
            PARASynth[Current_Instrument].osc1_pw = cpar * 4;
            break;

        case 1: /* ENV1_Attack */
            PARASynth[Current_Instrument].env1_attack = cpar * 512;
            break;

        case 2: /* ENV1_Decay */
            PARASynth[Current_Instrument].env1_decay = cpar * 512;
            break;

        case 3: /* ENV1_Sustain */
            PARASynth[Current_Instrument].env1_sustain = cpar;
            break;

        case 4: /* ENV1_Release */
            PARASynth[Current_Instrument].env1_release = cpar * 512;
            break;

        case 5: /* OSC3 volume */
            PARASynth[Current_Instrument].osc3_volume = cpar;
            break;
    }
}

void CParcha_OSC2(int cpar)
{
    switch(Pos_Tbl_Synth_OSC2)
    {
        case 0: /* Osc2_PD */
            PARASynth[Current_Instrument].osc2_pw = cpar * 4;
            break;

        case 1: /* Osc2_Detune */
            PARASynth[Current_Instrument].osc2_detune = cpar;
            break;

        case 2: /* Osc2_Finetune */
            PARASynth[Current_Instrument].osc2_finetune = cpar;
            break;

        case 3: /* ENV2_Attack */
            PARASynth[Current_Instrument].env2_attack = cpar * 512;
            break;

        case 4: /* ENV2_Decay */
            PARASynth[Current_Instrument].env2_decay = cpar * 512;
            break;

        case 5: /* ENV2_Sustain */
            PARASynth[Current_Instrument].env2_sustain = cpar;
            break;

        case 6: /* ENV2_Release */
            PARASynth[Current_Instrument].env2_release = cpar * 512;
            break;
    }
}

void CParcha_VCF(int cpar)
{
    switch(Pos_Tbl_Synth_VCF)
    {
        case 0: /* VCF Cutoff */
            PARASynth[Current_Instrument].vcf_cutoff = cpar;
            break;

        case 1: /* VCF_Resonance */
            PARASynth[Current_Instrument].vcf_resonance = cpar;
            break;
    }
}

void CParcha_LFO1(int cpar)
{
    switch(Pos_Tbl_Synth_LFO1)
    {
        case 0: /* Lfo1_Period */
            PARASynth[Current_Instrument].lfo1_period = cpar;
            break;

        case 1: /* Lfo1->osc1 pd */
            PARASynth[Current_Instrument].lfo1_osc1_pw = cpar;
            break;

        case 2: /* Lfo1->osc2 pd */
            PARASynth[Current_Instrument].lfo1_osc2_pw = cpar;
            break;

        case 3: /* Lfo1->osc1 pitch */
            PARASynth[Current_Instrument].lfo1_osc1_pitch = cpar;
            break;

        case 4: /* Lfo1->osc1 pitch */
            PARASynth[Current_Instrument].lfo1_osc2_pitch = cpar;
            break;

        case 5: /* Lfo1->osc1 volume */
            PARASynth[Current_Instrument].lfo1_osc1_volume = cpar;
            break;

        case 6: /* Lfo1->osc2 volume */
            PARASynth[Current_Instrument].lfo1_osc2_volume = cpar;
            break;

        case 7: /* Lfo1->vcf cutoff */
            PARASynth[Current_Instrument].lfo1_vcf_cutoff = cpar;
            break;

        case 8: /* Lfo1->vcf resonance */
            PARASynth[Current_Instrument].lfo1_vcf_resonance = cpar;
            break;

        case 9: /* LFO1_Attack */
            PARASynth[Current_Instrument].lfo1_attack = cpar * 512;
            break;

        case 10: /* LFO1_Decay */
            PARASynth[Current_Instrument].lfo1_decay = cpar * 512;
            break;

        case 11: /* LFO1_Sustain */
            PARASynth[Current_Instrument].lfo1_sustain = cpar;
            break;

        case 12: /* LFO1_Release */
            PARASynth[Current_Instrument].lfo1_release = cpar * 512;
            break;
    }
}

void CParcha_LFO2(int cpar)
{
    switch(Pos_Tbl_Synth_LFO2)
    {
        case 0: /* LFO2_Period */
            PARASynth[Current_Instrument].lfo2_period = cpar;
            break;

        case 1: /* Lfo2->osc1 pd */
            PARASynth[Current_Instrument].lfo2_osc1_pw = cpar;
            break;

        case 2: /* Lfo2->osc2 pd */
            PARASynth[Current_Instrument].lfo2_osc2_pw = cpar;
            break;

        case 3: /* Lfo2->osc1 pitch */
            PARASynth[Current_Instrument].lfo2_osc1_pitch = cpar;
            break;

        case 4: /* Lfo2->osc2 pitch */
            PARASynth[Current_Instrument].lfo2_osc2_pitch = cpar;
            break;

        case 5: /* Lfo2->osc1 volume */
            PARASynth[Current_Instrument].lfo2_osc1_volume = cpar;
            break;

        case 6: /* Lfo2->osc2 volume */
            PARASynth[Current_Instrument].lfo2_osc2_volume = cpar;
            break;

        case 7: /* Lfo2->vcf cutoff */
            PARASynth[Current_Instrument].lfo2_vcf_cutoff = cpar;
            break;

        case 8: /* Lfo2->vcf resonance */
            PARASynth[Current_Instrument].lfo2_vcf_resonance = cpar;
            break;

        case 9: /* LFO1_Attack */
            PARASynth[Current_Instrument].lfo2_attack = cpar * 512;
            break;

        case 10: /* LFO1_Decay */
            PARASynth[Current_Instrument].lfo2_decay = cpar * 512;
            break;

        case 11: /* LFO1_Sustain */
            PARASynth[Current_Instrument].lfo2_sustain = cpar;
            break;

        case 12: /* LFO1_Release */
            PARASynth[Current_Instrument].lfo2_release = cpar * 512;
            break;
    }
}

void CParcha_ENV1(int cpar)
{
    switch(Pos_Tbl_Synth_ENV1)
    {
        case 0: /* Env1->osc1 pd */
            PARASynth[Current_Instrument].env1_osc1_pw = cpar;
            break;

        case 1: /* Env1->osc2 pd */
            PARASynth[Current_Instrument].env1_osc2_pw = cpar;
            break;

        case 2: /* Env1->osc1 pitch */
            PARASynth[Current_Instrument].env1_osc1_pitch = cpar;
            break;

        case 3: /* Env1->osc2 pitch */
            PARASynth[Current_Instrument].env1_osc2_pitch = cpar;
            break;

        case 4: /* Env1->osc1 volume */
            PARASynth[Current_Instrument].env1_osc1_volume = cpar;
            break;

        case 5: /* Env1->osc2 volume */
            PARASynth[Current_Instrument].env1_osc2_volume = cpar;
            break;

        case 6: /* Env1->vcf cutoff */
            PARASynth[Current_Instrument].env1_vcf_cutoff = cpar;
            break;

        case 7: /* Env1->vcf resonance */
            PARASynth[Current_Instrument].env1_vcf_resonance = cpar;
            break;
    }
}

void CParcha_ENV2(int cpar)
{
    switch(Pos_Tbl_Synth_ENV2)
    {
        case 0: /* Env2->osc1 pd */
            PARASynth[Current_Instrument].env2_osc1_pw = cpar;
            break;

        case 1: /* Env2->osc2 pd */
            PARASynth[Current_Instrument].env2_osc2_pw = cpar;
            break;

        case 2: /* Env2->osc1 pitch */
            PARASynth[Current_Instrument].env2_osc1_pitch = cpar;
            break;

        case 3: /* Env2->osc2 pitch */
            PARASynth[Current_Instrument].env2_osc2_pitch = cpar;
            break;

        case 4: /* Env2->osc1 volume */
            PARASynth[Current_Instrument].env2_osc1_volume = cpar;
            break;

        case 5: /* Env2->osc2 volume */
            PARASynth[Current_Instrument].env2_osc2_volume = cpar;
            break;

        case 6: /* Env2->vcf cutoff */
            PARASynth[Current_Instrument].env2_vcf_cutoff = cpar;
            break;

        case 7: /* Env2->vcf resonance */
            PARASynth[Current_Instrument].env2_vcf_resonance = cpar;
            break;
    }
}

void CParcha_Misc(int cpar)
{
    switch(Pos_Tbl_Synth_Misc)
    {
        case 0: /* Misc. glide */
            PARASynth[Current_Instrument].ptc_glide = cpar;
            break;

        case 1: /* Misc. Global volume */
            PARASynth[Current_Instrument].glb_volume = cpar;
            break;

        case 2: /* Misc. Distortion */
            PARASynth[Current_Instrument].disto = cpar;
            break;
    }
}

void Rand_OSC1()
{
    PARASynth[Current_Instrument].osc1_waveform = rand() % 7;
    if(Allow_Phase_Distortion_OSC1)
    {
        PARASynth[Current_Instrument].osc1_pw = rand() & 0x1ff;
    }
    PARASynth[Current_Instrument].env1_attack = rand();
    PARASynth[Current_Instrument].env1_decay = rand();
    PARASynth[Current_Instrument].env1_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].env1_release = rand();
    PARASynth[Current_Instrument].osc3_volume = rand() & 0x7f;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_OSC2()
{
    PARASynth[Current_Instrument].osc2_waveform = rand() % 7;
    if(Allow_Phase_Distortion_OSC2)
    {
        PARASynth[Current_Instrument].osc2_pw = rand() & 0x1ff;
    }
    PARASynth[Current_Instrument].osc2_finetune = rand() & 0x7f;
    PARASynth[Current_Instrument].env2_attack = rand();
    PARASynth[Current_Instrument].env2_decay = rand();
    PARASynth[Current_Instrument].env2_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].env2_release = rand();
    PARASynth[Current_Instrument].osc2_detune = rand() & 0x7f;

    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_VCF()
{
    PARASynth[Current_Instrument].vcf_type = rand() % 5;
    PARASynth[Current_Instrument].vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].vcf_resonance = rand() & 0x7f;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_LFO1()
{
    PARASynth[Current_Instrument].lfo1_period = rand() & 0x7f;
    if(Allow_Phase_Distortion_OSC1)
    {
        PARASynth[Current_Instrument].lfo1_osc1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC2)
    {
        PARASynth[Current_Instrument].lfo1_osc2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].lfo1_osc1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo1_osc2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo1_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo1_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo1_osc1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo1_osc2_pitch = rand() & 0x7f;

    PARASynth[Current_Instrument].lfo1_attack = rand();
    PARASynth[Current_Instrument].lfo1_decay = rand();
    PARASynth[Current_Instrument].lfo1_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo1_release = rand();
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_LFO2()
{
    PARASynth[Current_Instrument].lfo2_period = rand() & 0x7f;
    if(Allow_Phase_Distortion_OSC1)
    {
        PARASynth[Current_Instrument].lfo2_osc1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC2)
    {
        PARASynth[Current_Instrument].lfo2_osc2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].lfo2_osc1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo2_osc2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo2_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo2_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo2_osc1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo2_osc2_pitch = rand() & 0x7f;

    PARASynth[Current_Instrument].lfo2_attack = rand();
    PARASynth[Current_Instrument].lfo2_decay = rand();
    PARASynth[Current_Instrument].lfo2_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo2_release = rand();
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_ENV1()
{
    if(Allow_Phase_Distortion_OSC1)
    {
        PARASynth[Current_Instrument].env1_osc1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC2)
    {
        PARASynth[Current_Instrument].env1_osc2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].env1_osc1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env1_osc2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env1_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].env1_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].env1_osc1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].env1_osc2_pitch = rand() & 0x7f;

    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_ENV2()
{
    if(Allow_Phase_Distortion_OSC1)
    {
        PARASynth[Current_Instrument].env2_osc1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC2)
    {
        PARASynth[Current_Instrument].env2_osc2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].env2_osc1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env2_osc2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env2_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].env2_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].env2_osc1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].env2_osc2_pitch = rand() & 0x7f;

    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_Misc()
{
    if(Allow_Phase_Distortion_OSC1 || Allow_Phase_Distortion_OSC2)
    {
        PARASynth[Current_Instrument].ptc_glide = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].glb_volume = 100;
    PARASynth[Current_Instrument].disto = rand() & 0x7f;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void IniCsParNames(void)
{
    sprintf(Names_Tbl_Synth_OSC1[0], "Pulse Phase");
    sprintf(Names_Tbl_Synth_OSC1[1], "Attack");
    sprintf(Names_Tbl_Synth_OSC1[2], "Decay");
    sprintf(Names_Tbl_Synth_OSC1[3], "Sustain Level");
    sprintf(Names_Tbl_Synth_OSC1[4], "Release");
    sprintf(Names_Tbl_Synth_OSC1[5], "Sub-Oscillator Volume");

    sprintf(Names_Tbl_Synth_OSC2[0], "Pulse Phase");
    sprintf(Names_Tbl_Synth_OSC2[1], "Detune");
    sprintf(Names_Tbl_Synth_OSC2[2], "Finetune");
    sprintf(Names_Tbl_Synth_OSC2[3], "Attack");
    sprintf(Names_Tbl_Synth_OSC2[4], "Decay");
    sprintf(Names_Tbl_Synth_OSC2[5], "Sustain Level");
    sprintf(Names_Tbl_Synth_OSC2[6], "Release");

    sprintf(Names_Tbl_Synth_VCF[0], "CutOff");
    sprintf(Names_Tbl_Synth_VCF[1], "Resonance");

    sprintf(Names_Tbl_Synth_LFO1[0], "Period");
    sprintf(Names_Tbl_Synth_LFO1[1], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO1[2], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO1[3], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_LFO1[4], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_LFO1[5], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_LFO1[6], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_LFO1[7], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_LFO1[8], "-> Vcf Resonance");
    sprintf(Names_Tbl_Synth_LFO1[9], "Attack");
    sprintf(Names_Tbl_Synth_LFO1[10], "Decay");
    sprintf(Names_Tbl_Synth_LFO1[11], "Sustain Level");
    sprintf(Names_Tbl_Synth_LFO1[12], "Release");

    sprintf(Names_Tbl_Synth_LFO2[0], "Period");
    sprintf(Names_Tbl_Synth_LFO2[1], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO2[2], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO2[3], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_LFO2[4], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_LFO2[5], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_LFO2[6], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_LFO2[7], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_LFO2[8], "-> Vcf Resonance");
    sprintf(Names_Tbl_Synth_LFO2[9], "Attack");
    sprintf(Names_Tbl_Synth_LFO2[10], "Decay");
    sprintf(Names_Tbl_Synth_LFO2[11], "Sustain Level");
    sprintf(Names_Tbl_Synth_LFO2[12], "Release");

    sprintf(Names_Tbl_Synth_ENV1[0], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV1[1], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV1[2], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_ENV1[3], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_ENV1[4], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_ENV1[5], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_ENV1[6], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_ENV1[7], "-> Vcf Resonance");

    sprintf(Names_Tbl_Synth_ENV2[0], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV2[1], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV2[2], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_ENV2[3], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_ENV2[4], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_ENV2[5], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_ENV2[6], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_ENV2[7], "-> Vcf Resonance");

    sprintf(Names_Tbl_Synth_Misc[0], "Portamento Glide");
    sprintf(Names_Tbl_Synth_Misc[1], "Final [global] Volume");
    sprintf(Names_Tbl_Synth_Misc[2], "Wave Distortion");
}
