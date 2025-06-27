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
#include "include/editor_synth.h"
#include "include/editor_instrument.h"
#include "../support/include/timer.h"
#include "../files/include/files.h"
#include "../ui/include/requesters.h"

// ------------------------------------------------------
// Variables
extern REQUESTER Overwrite_Requester;

extern Synth_Parameters PARASynth[128];
extern int play_pattern;
extern int reset_carriers;
int csynth_slv_OSC_1;
int csynth_slv_OSC_2;
int csynth_slv_VCF;
int csynth_slv_LFO_1;
int csynth_slv_LFO_2;
int csynth_slv_ENV_1;
int csynth_slv_ENV_2;
int csynth_slv_Misc;
int temp_osc_3_interval;

int Allow_All;
int Allow_Button;
int Allow_Button_Pushed;
int Allow_Phase_Distortion_OSC_1 = TRUE;
int Allow_Phase_Distortion_OSC_2 = TRUE;
   
void Actualize_Pattern_Ed(void);
void Actualize_Synth_Params_Sliders(void);
void Actualize_Sequencer(void);
void Renew_Sample_Ed(void);
void Center_SynthParam_OSC_1(void);
void Center_SynthParam_OSC_2(void);
void Center_SynthParam_VCF(void);
void Center_SynthParam_LFO_1(void);
void Center_SynthParam_LFO_2(void);
void Center_SynthParam_ENV_1(void);
void Center_SynthParam_ENV_2(void);
void Center_SynthParam_Misc(void);
void Rand_OSC_1();
void Rand_OSC_2();
void Rand_VCF();
void Rand_LFO_1();
void Rand_LFO_2();
void Rand_ENV_1();
void Rand_ENV_2();
void Rand_Misc();
void CParcha_OSC_1(int cpar);
void CParcha_OSC_2(int cpar);
void CParcha_VCF(int cpar);
void CParcha_LFO_1(int cpar);
void CParcha_LFO_2(int cpar);
void CParcha_ENV_1(int cpar);
void CParcha_ENV_2(int cpar);
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

int Tbl_Synth_OSC_1[] =
{
    1, /* Osc1_PD */
    7, /* ENV_1_Attack */
    8, /* ENV_1_Decay */
    9, /* ENV_1_Sustain */
    10, /* ENV_1_Release */
    49, /* OSC_3 volume */
    49, /* OSC_3 interval */
};
int Pos_Tbl_Synth_OSC_1;
int Size_Tbl_Synth_OSC_1 = (sizeof(Tbl_Synth_OSC_1) / sizeof(int)) - 1;
char Names_Tbl_Synth_OSC_1[(sizeof(Tbl_Synth_OSC_1) / sizeof(int))][32];

int Tbl_Synth_OSC_2[] =
{
    2, /* Osc2_PD */
    3, /* Osc2_Detune */
    4, /* Osc2_Finetune */
    11, /* ENV_2_Attack */
    12, /* ENV_2_Decay */
    13, /* ENV_2_Sustain */
    14, /* ENV_2_Release */
};
int Pos_Tbl_Synth_OSC_2;
int Size_Tbl_Synth_OSC_2 = (sizeof(Tbl_Synth_OSC_2) / sizeof(int)) - 1;
char Names_Tbl_Synth_OSC_2[(sizeof(Tbl_Synth_OSC_2) / sizeof(int))][32];

int Tbl_Synth_VCF[] =
{
    5, /* VCF Cutoff */
    6, /* VCF_Resonance */
};
int Pos_Tbl_Synth_VCF;
int Size_Tbl_Synth_VCF = (sizeof(Tbl_Synth_VCF) / sizeof(int)) - 1;
char Names_Tbl_Synth_VCF[(sizeof(Tbl_Synth_VCF) / sizeof(int))][32];

int Tbl_Synth_LFO_1[] =
{
    15, /* lfo_1_Period */
    17, /* lfo_1->osc_1 pd */
    18, /* lfo_1->osc_2 pd */
    19, /* lfo_1->osc_1 pitch */
    20, /* lfo_1->osc_1 pitch */
    21, /* lfo_1->osc_1 volume */
    22, /* lfo_1->osc_2 volume */
    23, /* lfo_1->vcf cutoff */
    24, /* lfo_1->vcf resonance */
    24, /* lfo_1->disto */
    25, /* lfo_1_attack */
    26, /* lfo_1_decay */
    27, /* lfo_1_sustain */
    28, /* lfo_1_release */
};
int Pos_Tbl_Synth_LFO_1;
int Size_Tbl_Synth_LFO_1 = (sizeof(Tbl_Synth_LFO_1) / sizeof(int)) - 1;
char Names_Tbl_Synth_LFO_1[(sizeof(Tbl_Synth_LFO_1) / sizeof(int))][32];

int Tbl_Synth_LFO_2[] =
{
    16, /* lfo_2_Period */
    25, /* lfo_2->osc_1 pd */
    26, /* lfo_2->osc_2 pd */
    27, /* lfo_2->osc_1 pitch */
    28, /* lfo_2->osc_2 pitch */
    29, /* lfo_2->osc_1 volume */
    30, /* lfo_2->osc_2 volume */
    31, /* lfo_2->vcf cutoff */
    32, /* lfo_2->vcf resonance */
    32, /* lfo_2->disto */
    25, /* lfo_2_attack */
    26, /* lfo_2_decay */
    27, /* lfo_2_sustain */
    28, /* lfo_2_release */
};
int Pos_Tbl_Synth_LFO_2;
int Size_Tbl_Synth_LFO_2 = (sizeof(Tbl_Synth_LFO_2) / sizeof(int)) - 1;
char Names_Tbl_Synth_LFO_2[(sizeof(Tbl_Synth_LFO_2) / sizeof(int))][32];

int Tbl_Synth_ENV_1[] =
{
    33, /* Env1->osc_1 pd */
    34, /* Env1->osc_2 pd */
    35, /* Env1->osc_1 pitch */
    36, /* Env1->osc_2 pitch */
    37, /* Env1->osc_1 volume */
    38, /* Env1->osc_2 volume */
    39, /* Env1->vcf cutoff */
    40, /* Env1->vcf resonance */
    40, /* Env1->disto */
};
int Pos_Tbl_Synth_ENV_1;
int Size_Tbl_Synth_ENV_1 = (sizeof(Tbl_Synth_ENV_1) / sizeof(int)) - 1;
char Names_Tbl_Synth_ENV_1[(sizeof(Tbl_Synth_ENV_1) / sizeof(int))][32];

int Tbl_Synth_ENV_2[] =
{
    41, /* Env2->osc_1 pd */
    42, /* Env2->osc_2 pd */
    43, /* Env2->osc_1 pitch */
    44, /* Env2->osc_2 pitch */
    45, /* Env2->osc_1 volume */
    46, /* Env2->osc_2 volume */
    47, /* Env2->vcf cutoff */
    48, /* Env2->vcf resonance */
    48, /* Env2->disto */
};
int Pos_Tbl_Synth_ENV_2;
int Size_Tbl_Synth_ENV_2 = (sizeof(Tbl_Synth_ENV_2) / sizeof(int)) - 1;
char Names_Tbl_Synth_ENV_2[(sizeof(Tbl_Synth_ENV_2) / sizeof(int))][32];

int Tbl_Synth_Misc[] =
{
    50, /* Misc. glide */
    51, /* Misc. Global volume */
    52, /* Misc. Distortion */
};
int Pos_Tbl_Synth_Misc;
int Size_Tbl_Synth_Misc = (sizeof(Tbl_Synth_Misc) / sizeof(int)) - 1;
char Names_Tbl_Synth_Misc[(sizeof(Tbl_Synth_Misc) / sizeof(int))][32];

void Draw_Synth_Ed(void)
{
    Draw_Editors_Bar(USER_SCREEN_SYNTH_EDIT);

    Gui_Draw_Button_Box(0, (Cur_Height - 153), fsize, 130, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
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
            int Synth_Prg_check = Synth_Prg[Current_Instrument];
            if(Synth_Prg_check > 130) Synth_Prg[Current_Instrument] = 130;
            
            switch(Synth_Prg[Current_Instrument])
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
                    value_box(228, (Cur_Height - 150), (Synth_Prg[Current_Instrument] - 2), BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
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
           gode == UPDATE_SYNTH_ED_OSC_1_WAVEFORM ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            Gui_Draw_Button_Box(24, (Cur_Height - 95) - 1, 23, 16, "sin", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(49, (Cur_Height - 95) - 1, 23, 16, "saw", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(74, (Cur_Height - 95) - 1, 23, 16, "pul", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(99, (Cur_Height - 95) - 1, 23, 16, "whit", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(124, (Cur_Height - 95) - 1, 23, 16, "tri", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(149, (Cur_Height - 95) - 1, 23, 16, "instr", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(174, (Cur_Height - 95) - 1, 23, 16, "off", Allow_Button | BUTTON_TEXT_CENTERED);
            switch(PARASynth[Current_Instrument].osc_1_waveform)
            {
                case 0:Gui_Draw_Button_Box(24, (Cur_Height - 95) - 1, 23, 16, "sin", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 1:Gui_Draw_Button_Box(49, (Cur_Height - 95) - 1, 23, 16, "saw", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 2:Gui_Draw_Button_Box(74, (Cur_Height - 95) - 1, 23, 16, "pul", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 3:Gui_Draw_Button_Box(99, (Cur_Height - 95) - 1, 23, 16, "whit", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 6:Gui_Draw_Button_Box(124, (Cur_Height - 95) - 1, 23, 16, "tri", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 5:Gui_Draw_Button_Box(149, (Cur_Height - 95) - 1, 23, 16, "instr", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 4:Gui_Draw_Button_Box(174, (Cur_Height - 95) - 1, 23, 16, "off", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
            }
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_OSC_2_WAVEFORM ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            Gui_Draw_Button_Box(24, (Cur_Height - 41) - 1, 23, 16, "sin", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(49, (Cur_Height - 41) - 1, 23, 16, "saw", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(74, (Cur_Height - 41) - 1, 23, 16, "pul", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(99, (Cur_Height - 41) - 1, 23, 16, "whit", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(124, (Cur_Height - 41) - 1, 23, 16, "tri", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(149, (Cur_Height - 41) - 1, 23, 16, "instr", Allow_Button | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(174, (Cur_Height - 41) - 1, 23, 16, "off", Allow_Button | BUTTON_TEXT_CENTERED);
            switch(PARASynth[Current_Instrument].osc_2_waveform)
            {
                case 0: Gui_Draw_Button_Box(24, (Cur_Height - 41) - 1, 23, 16, "sin", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 1: Gui_Draw_Button_Box(49, (Cur_Height - 41) - 1, 23, 16, "saw", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 2: Gui_Draw_Button_Box(74, (Cur_Height - 41) - 1, 23, 16, "pul", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 3: Gui_Draw_Button_Box(99, (Cur_Height - 41) - 1, 23, 16, "whit", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
                case 6: Gui_Draw_Button_Box(124, (Cur_Height - 41) - 1, 23, 16, "tri", Allow_Button_Pushed | BUTTON_TEXT_CENTERED); break;
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
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_CHANGE_NAME ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            char tcp[30];
            sprintf(tcp, "%s_", PARASynth[Current_Instrument].Preset_Name);

            if(snamesel == INPUT_SYNTH_NAME)
            {
                Gui_Draw_Button_Box(592, (Cur_Height - 150), 164, 16, tcp, BUTTON_PUSHED | BUTTON_INPUT);
            }
            else
            {
                Gui_Draw_Button_Box(592, (Cur_Height - 150), 164, 16, PARASynth[Current_Instrument].Preset_Name, Allow_Button | BUTTON_INPUT);
            }
            Actualize_Instruments_Synths_List(1);
        }

        if(gode == UPDATE_SYNTH_ED_VALIDATE_PARAMS)
        {
            CParcha_OSC_1(csynth_slv_OSC_1);
            CParcha_OSC_2(csynth_slv_OSC_2);
            CParcha_VCF(csynth_slv_VCF);
            CParcha_LFO_1(csynth_slv_LFO_1);
            CParcha_LFO_2(csynth_slv_LFO_2);
            CParcha_ENV_1(csynth_slv_ENV_1);
            CParcha_ENV_2(csynth_slv_ENV_2);
            CParcha_Misc(csynth_slv_Misc);
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_OSC_1_PARAMS)
        {
            if(Pos_Tbl_Synth_OSC_1 < 0) Pos_Tbl_Synth_OSC_1 = 0;
            if(Pos_Tbl_Synth_OSC_1 > Size_Tbl_Synth_OSC_1) Pos_Tbl_Synth_OSC_1 = Size_Tbl_Synth_OSC_1;
            Gui_Draw_Arrows_Number_Box2(62, (Cur_Height - 131) - 1, Pos_Tbl_Synth_OSC_1 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(62 + 62, (Cur_Height - 131) - 1, 128, 16, Names_Tbl_Synth_OSC_1[Pos_Tbl_Synth_OSC_1], BUTTON_NORMAL | BUTTON_DISABLED);

            if(Allow_Phase_Distortion_OSC_1 || Pos_Tbl_Synth_OSC_1 != 0)
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
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_OSC_2_PARAMS)
        {
            if(Pos_Tbl_Synth_OSC_2 < 0) Pos_Tbl_Synth_OSC_2 = 0;
            if(Pos_Tbl_Synth_OSC_2 > Size_Tbl_Synth_OSC_2) Pos_Tbl_Synth_OSC_2 = Size_Tbl_Synth_OSC_2;
            Gui_Draw_Arrows_Number_Box2(62, (Cur_Height - 77) - 1, Pos_Tbl_Synth_OSC_2 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(62 + 62, (Cur_Height - 77) - 1, 128, 16, Names_Tbl_Synth_OSC_2[Pos_Tbl_Synth_OSC_2], BUTTON_NORMAL | BUTTON_DISABLED);

            if(Allow_Phase_Distortion_OSC_2 || Pos_Tbl_Synth_OSC_2 != 0)
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

            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_VCF_PARAMS)
        {
            if(Pos_Tbl_Synth_VCF < 0) Pos_Tbl_Synth_VCF = 0;
            if(Pos_Tbl_Synth_VCF > Size_Tbl_Synth_VCF) Pos_Tbl_Synth_VCF = Size_Tbl_Synth_VCF;
            Gui_Draw_Arrows_Number_Box2(331, (Cur_Height - 131) - 1, Pos_Tbl_Synth_VCF + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(331 + 62, (Cur_Height - 131) - 1, 111, 16, Names_Tbl_Synth_VCF[Pos_Tbl_Synth_VCF], BUTTON_NORMAL | BUTTON_DISABLED);
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_LFO_1_PARAMS)
        {
            if(Pos_Tbl_Synth_LFO_1 < 0) Pos_Tbl_Synth_LFO_1 = 0;
            if(Pos_Tbl_Synth_LFO_1 > Size_Tbl_Synth_LFO_1) Pos_Tbl_Synth_LFO_1 = Size_Tbl_Synth_LFO_1;
            Gui_Draw_Arrows_Number_Box2(331, (Cur_Height - 95) - 1, Pos_Tbl_Synth_LFO_1 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(331 + 62, (Cur_Height - 95) - 1, 111, 16, Names_Tbl_Synth_LFO_1[Pos_Tbl_Synth_LFO_1], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_1 == 1) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_1 == 2))
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
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_LFO_2_PARAMS)
        {
            if(Pos_Tbl_Synth_LFO_2 < 0) Pos_Tbl_Synth_LFO_2 = 0;
            if(Pos_Tbl_Synth_LFO_2 > Size_Tbl_Synth_LFO_2) Pos_Tbl_Synth_LFO_2 = Size_Tbl_Synth_LFO_2;
            Gui_Draw_Arrows_Number_Box2(331, (Cur_Height - 59) - 1, Pos_Tbl_Synth_LFO_2 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(331 + 62, (Cur_Height - 59) - 1, 111, 16, Names_Tbl_Synth_LFO_2[Pos_Tbl_Synth_LFO_2], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_2 == 1) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_2 == 2))
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
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_ENV_1_PARAMS)
        {
            if(Pos_Tbl_Synth_ENV_1 < 0) Pos_Tbl_Synth_ENV_1 = 0;
            if(Pos_Tbl_Synth_ENV_1 > Size_Tbl_Synth_ENV_1) Pos_Tbl_Synth_ENV_1 = Size_Tbl_Synth_ENV_1;
            Gui_Draw_Arrows_Number_Box2(583, (Cur_Height - 131) - 1, Pos_Tbl_Synth_ENV_1 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(583 + 62, (Cur_Height - 131) - 1, 111, 16, Names_Tbl_Synth_ENV_1[Pos_Tbl_Synth_ENV_1], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_1 == 0) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_1 == 1))
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
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_ENV_2_PARAMS)
        {
            if(Pos_Tbl_Synth_ENV_2 < 0) Pos_Tbl_Synth_ENV_2 = 0;
            if(Pos_Tbl_Synth_ENV_2 > Size_Tbl_Synth_ENV_2) Pos_Tbl_Synth_ENV_2 = Size_Tbl_Synth_ENV_2;
            Gui_Draw_Arrows_Number_Box2(583, (Cur_Height - 95) - 1, Pos_Tbl_Synth_ENV_2 + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(583 + 62, (Cur_Height - 95) - 1, 111, 16, Names_Tbl_Synth_ENV_2[Pos_Tbl_Synth_ENV_2], BUTTON_NORMAL | BUTTON_DISABLED);

            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_2 == 0) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_2 == 1))
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
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL || gode == UPDATE_SYNTH_ED_Misc_PARAMS)
        {
            if(Pos_Tbl_Synth_Misc < 0) Pos_Tbl_Synth_Misc = 0;
            if(Pos_Tbl_Synth_Misc > Size_Tbl_Synth_Misc) Pos_Tbl_Synth_Misc = Size_Tbl_Synth_Misc;
            Gui_Draw_Arrows_Number_Box2(583, (Cur_Height - 59) - 1, Pos_Tbl_Synth_Misc + 1, Allow_Button | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            Gui_Draw_Button_Box(583 + 62, (Cur_Height - 59) - 1, 111, 16, Names_Tbl_Synth_Misc[Pos_Tbl_Synth_Misc], BUTTON_NORMAL | BUTTON_DISABLED);

            if((Allow_Phase_Distortion_OSC_1 || Allow_Phase_Distortion_OSC_2) ||
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
            Actualize_Synth_Params_Sliders();
        }

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_OSC_3_SWITCH ||
           gode == UPDATE_SYNTH_ED_VALUES)
        {
            if(PARASynth[Current_Instrument].osc_3_switch)
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

        if(gode == UPDATE_SYNTH_ED_ALL ||
           gode == UPDATE_SYNTH_ED_SYNC)
        {
            if(PARASynth[Current_Instrument].osc_sync)
            {
                Gui_Draw_Button_Box(208, (Cur_Height - 41) - 1, 30, 16, "Sync", Allow_Button_Pushed | BUTTON_TEXT_CENTERED);
            }
            else
            {
                Gui_Draw_Button_Box(208, (Cur_Height - 41) - 1, 30, 16, "Sync", Allow_Button | BUTTON_TEXT_CENTERED);
            }

        }
    }
}

void Check_Slider(int *Value, int x, int y)
{
    if(Check_Mouse(x - 2, y, 148, 16))
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
        if(Allow_Phase_Distortion_OSC_1 || Pos_Tbl_Synth_OSC_1 != 0)
        {
            Check_Slider(&csynth_slv_OSC_1, 43, (Cur_Height - 113) - 1);
        }
        if(Allow_Phase_Distortion_OSC_2 || Pos_Tbl_Synth_OSC_2 != 0)
        {
            Check_Slider(&csynth_slv_OSC_2, 43, (Cur_Height - 59) - 1);
        }

        Check_Slider(&csynth_slv_VCF, 313, (Cur_Height - 113) - 1);

        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_1 == 1) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_1 == 2))
        {
        }
        else
        {
            Check_Slider(&csynth_slv_LFO_1, 313, (Cur_Height - 77) - 1);
        }

        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_2 == 1) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_2 == 2))
        {
        }
        else
        {
            Check_Slider(&csynth_slv_LFO_2, 313, (Cur_Height - 41) - 1);
        }

        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_1 == 0) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_1 == 1)) {
        }
        else
        {
            Check_Slider(&csynth_slv_ENV_1, 565, (Cur_Height - 113) - 1);
        }

        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_2 == 0) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_2 == 1))
        {
        }
        else
        {
            Check_Slider(&csynth_slv_ENV_2, 565, (Cur_Height - 77) - 1);
        }

        if((Allow_Phase_Distortion_OSC_1 || Allow_Phase_Distortion_OSC_2) ||
            Pos_Tbl_Synth_Misc > 0)
        {
            Check_Slider(&csynth_slv_Misc, 565, (Cur_Height - 41) - 1);
        }
    }
}

void Check_Sliders_Arrows(int value_step)
{
    // OSC_1 Sliders Arrows
    if(Check_Mouse(24, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC_1 || Pos_Tbl_Synth_OSC_1 != 0)
        {
            csynth_slv_OSC_1 -= value_step;
            if(csynth_slv_OSC_1 < 0) csynth_slv_OSC_1 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    if(Check_Mouse(190, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC_1 || Pos_Tbl_Synth_OSC_1 != 0)
        {
            csynth_slv_OSC_1 += value_step;
            if(csynth_slv_OSC_1 > 128) csynth_slv_OSC_1 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // OSC_2 Sliders Arrows
    if(Check_Mouse(24, (Cur_Height - 59) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC_2 || Pos_Tbl_Synth_OSC_2 != 0)
        {
            csynth_slv_OSC_2 -= value_step;
            if(csynth_slv_OSC_2 < 0) csynth_slv_OSC_2 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    if(Check_Mouse(190, (Cur_Height - 59) - 1, 16, 16) == 1)
    {
        if(Allow_Phase_Distortion_OSC_2 || Pos_Tbl_Synth_OSC_2 != 0)
        {
            csynth_slv_OSC_2 += value_step;
            if(csynth_slv_OSC_2 > 128) csynth_slv_OSC_2 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // VCF Sliders Arrows
    if(Check_Mouse(294, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        csynth_slv_VCF -= value_step;
        if(csynth_slv_VCF < 0) csynth_slv_VCF = 0;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
    }
    if(Check_Mouse(460, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        csynth_slv_VCF += value_step;
        if(csynth_slv_VCF > 128) csynth_slv_VCF = 128;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
    }

    // LFO_1 Sliders Arrows
    if(Check_Mouse(294, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_1 == 1) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_1 == 2))
        {
        }
        else
        {
            csynth_slv_LFO_1 -= value_step;
            if(csynth_slv_LFO_1 < 0) csynth_slv_LFO_1 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    if(Check_Mouse(460, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_1 == 1) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_1 == 2))
        {
        }
        else
        {
            csynth_slv_LFO_1 += value_step;
            if(csynth_slv_LFO_1 > 128) csynth_slv_LFO_1 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // LFO_2 Sliders Arrows
    if(Check_Mouse(294, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_2 == 1) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_2 == 2))
        {
        }
        else
        {
            csynth_slv_LFO_2 -= value_step;
            if(csynth_slv_LFO_2 < 0) csynth_slv_LFO_2 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    
    if(Check_Mouse(460, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_2 == 1) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_2 == 2))
        {
        }
        else
        {
            csynth_slv_LFO_2 += value_step;
            if(csynth_slv_LFO_2 > 128) csynth_slv_LFO_2 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // ENV_1 Sliders Arrows
    if(Check_Mouse(546, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_1 == 0) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_1 == 1))
        {
        }
        else
        {
            csynth_slv_ENV_1 -= value_step;
            if(csynth_slv_ENV_1 < 0) csynth_slv_ENV_1 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    if(Check_Mouse(712, (Cur_Height - 113) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_1 == 0) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_1 == 1))
        {
        }
        else
        {
            csynth_slv_ENV_1 += value_step;
            if(csynth_slv_ENV_1 > 128) csynth_slv_ENV_1 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // ENV_2 Sliders Arrows
    if(Check_Mouse(546, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_2 == 0) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_2 == 1))
        {
        }
        else
        {
            csynth_slv_ENV_2 -= value_step;
            if(csynth_slv_ENV_2 < 0) csynth_slv_ENV_2 = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    
    if(Check_Mouse(712, (Cur_Height - 77) - 1, 16, 16) == 1)
    {
        if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_2 == 0) ||
           !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_2 == 1))
        {
        }
        else
        {
            csynth_slv_ENV_2 += value_step;
            if(csynth_slv_ENV_2 > 128) csynth_slv_ENV_2 = 128;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }

    // Misc. Sliders Arrows
    if(Check_Mouse(546, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if((Allow_Phase_Distortion_OSC_1 || Allow_Phase_Distortion_OSC_2) ||
            Pos_Tbl_Synth_Misc > 0)
        {
            csynth_slv_Misc -= value_step;
            if(csynth_slv_Misc < 0) csynth_slv_Misc = 0;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VALIDATE_PARAMS;
        }
    }
    
    if(Check_Mouse(712, (Cur_Height - 41) - 1, 16, 16) == 1)
    {
        if((Allow_Phase_Distortion_OSC_1 || Allow_Phase_Distortion_OSC_2) ||
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
    // Select OSC_1 parameter
    if(Check_Mouse(62, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_OSC_1 > 0)
    {
        Pos_Tbl_Synth_OSC_1 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC_1_PARAMS;
    }
    if(Check_Mouse(62 + 44, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_OSC_1 < Size_Tbl_Synth_OSC_1)
    {
        Pos_Tbl_Synth_OSC_1 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC_1_PARAMS;
    }  

    // Select OSC_2 parameter
    if(Check_Mouse(62, (Cur_Height - 77) - 1, 16, 16) && Pos_Tbl_Synth_OSC_2 > 0)
    {
        Pos_Tbl_Synth_OSC_2 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC_2_PARAMS;
    }
    if(Check_Mouse(62 + 44, (Cur_Height - 77) - 1, 16, 16) && Pos_Tbl_Synth_OSC_2 < Size_Tbl_Synth_OSC_2)
    {
        Pos_Tbl_Synth_OSC_2 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_OSC_2_PARAMS;
    }  

    // Select VCF parameter
    if(Check_Mouse(331, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_VCF > 0)
    {
        Pos_Tbl_Synth_VCF -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VCF_PARAMS;
    }
    if(Check_Mouse(331 + 44, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_VCF < Size_Tbl_Synth_VCF)
    {
        Pos_Tbl_Synth_VCF += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_VCF_PARAMS;
    }  

    // Select LFO_1 parameter
    if(Check_Mouse(331, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_LFO_1 > 0)
    {
        Pos_Tbl_Synth_LFO_1 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO_1_PARAMS;
    }
    if(Check_Mouse(331 + 44, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_LFO_1 < Size_Tbl_Synth_LFO_1)
    {
        Pos_Tbl_Synth_LFO_1 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO_1_PARAMS;
    }  

    // Select LFO_2 parameter
    if(Check_Mouse(331, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_LFO_2 > 0)
    {
        Pos_Tbl_Synth_LFO_2 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO_2_PARAMS;
    }
    if(Check_Mouse(331 + 44, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_LFO_2 < Size_Tbl_Synth_LFO_2)
    {
        Pos_Tbl_Synth_LFO_2 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_LFO_2_PARAMS;
    }

    // Select ENV_1 parameter
    if(Check_Mouse(583, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_ENV_1 > 0)
    {
        Pos_Tbl_Synth_ENV_1 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV_1_PARAMS;
    }
    if(Check_Mouse(583 + 44, (Cur_Height - 131) - 1, 16, 16) && Pos_Tbl_Synth_ENV_1 < Size_Tbl_Synth_ENV_1)
    {
        Pos_Tbl_Synth_ENV_1 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV_1_PARAMS;
    }  

    // Select ENV_2 parameter
    if(Check_Mouse(583, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_ENV_2 > 0)
    {
        Pos_Tbl_Synth_ENV_2 -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV_2_PARAMS;
    }
    if(Check_Mouse(583 + 44, (Cur_Height - 95) - 1, 16, 16) && Pos_Tbl_Synth_ENV_2 < Size_Tbl_Synth_ENV_2)
    {
        Pos_Tbl_Synth_ENV_2 += Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_ENV_2_PARAMS;
    }  

    // Select Misc parameter
    if(Check_Mouse(583, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_Misc > 0)
    {
        Pos_Tbl_Synth_Misc -= Value_Step;
        gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        teac = UPDATE_SYNTH_ED_Misc_PARAMS;
    }
    if(Check_Mouse(583 + 44, (Cur_Height - 59) - 1, 16, 16) && Pos_Tbl_Synth_Misc < Size_Tbl_Synth_Misc)
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

        if(Check_Mouse(228, (Cur_Height - 150), 17, 16))
        {
            if(Synth_Prg[Current_Instrument] >= 10)
            {
                Synth_Prg[Current_Instrument] -= 10;
            }
            else
            {
                Synth_Prg[Current_Instrument] = 0;
            }
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(228 + 44, (Cur_Height - 150), 17, 16))
        {
            if(Synth_Prg[Current_Instrument] <= (129 - 10))
            {
                Synth_Prg[Current_Instrument] += 10;
            }
            else
            {
                Synth_Prg[Current_Instrument] = 129;
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
        // Select
        if(Check_Mouse(228, (Cur_Height - 150), 17, 16))
        {
            if(Synth_Prg[Current_Instrument])
            {
                Synth_Prg[Current_Instrument]--;
            }
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(228 + 44, (Cur_Height - 150), 17, 16))
        {
            if(Synth_Prg[Current_Instrument] < 129)
            {
                Synth_Prg[Current_Instrument]++;
            }
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
    }

    if(userscreen == USER_SCREEN_SYNTH_EDIT && Allow_All)
    {
        // Save the data
        if(Check_Mouse(758, (Cur_Height - 150), 34, 16))
        {
            if(File_Exist_Req("%s" SLASH "%s.pts", Dir_Presets, PARASynth[Current_Instrument].Preset_Name))
            {
                Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_SYNTH, NULL, TRUE);
            }
            else
            {
                gui_action = GUI_CMD_SAVE_SYNTH;
            }
        }

        // Start synth name input
        if(Check_Mouse(592, (Cur_Height - 150), 164, 16) && snamesel == INPUT_NONE)
        {
            snamesel = INPUT_SYNTH_NAME;
            strcpy(cur_input_name, PARASynth[Current_Instrument].Preset_Name);
            namesize = 0;
            sprintf(PARASynth[Current_Instrument].Preset_Name, "");
            teac = UPDATE_SYNTH_CHANGE_NAME;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }

        if(Check_Mouse(254, (Cur_Height - 131) - 1, 34, 16)) Rand_OSC_1();
        if(Check_Mouse(254, (Cur_Height - 77) - 1, 34, 16)) Rand_OSC_2();
        if(Check_Mouse(506, (Cur_Height - 131) - 1, 34, 16)) Rand_VCF();
        if(Check_Mouse(506, (Cur_Height - 95) - 1, 34, 16)) Rand_LFO_1();
        if(Check_Mouse(506, (Cur_Height - 59) - 1, 34, 16)) Rand_LFO_2();
        if(Check_Mouse(755, (Cur_Height - 131) - 1, 34, 16)) Rand_ENV_1();
        if(Check_Mouse(755, (Cur_Height - 95) - 1, 34, 16)) Rand_ENV_2();
        if(Check_Mouse(755, (Cur_Height - 59) - 1, 34, 16)) Rand_Misc();

        Check_Sliders_Arrows(1);

        Check_Parameters_Arrows(1);

        // Center OSC_1
        if(Check_Mouse(272, (Cur_Height - 113) - 1, 16, 16))
        {
            if(Allow_Phase_Distortion_OSC_1 || Pos_Tbl_Synth_OSC_1 != 0)
            {
                Center_SynthParam_OSC_1();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_OSC_1_PARAMS;
            }
        }  

        // Center OSC_2
        if(Check_Mouse(272, (Cur_Height - 59) - 1, 16, 16))
        {
            if(Allow_Phase_Distortion_OSC_2 || Pos_Tbl_Synth_OSC_2 != 0)
            {
                Center_SynthParam_OSC_2();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_OSC_2_PARAMS;
            }
        }  

        // Center VCF
        if(Check_Mouse(524, (Cur_Height - 113) - 1, 16, 16))
        {
            Center_SynthParam_VCF();
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            teac = UPDATE_SYNTH_ED_VCF_PARAMS;
        }  

        // Center LFO_1
        if(Check_Mouse(524, (Cur_Height - 77) - 1, 16, 16))
        {
            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_1 == 1) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_1 == 2))
            {
            }
            else
            {
                Center_SynthParam_LFO_1();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_LFO_1_PARAMS;
            }
        }  

        // Center LFO_2
        if(Check_Mouse(524, (Cur_Height - 41) - 1, 16, 16))
        {
            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_LFO_2 == 1) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_LFO_2 == 2))
            {
            }
            else
            {
                Center_SynthParam_LFO_2();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_LFO_2_PARAMS;
            }
        }  

        // Center ENV_1
        if(Check_Mouse(776, (Cur_Height - 113) - 1, 16, 16))
        {
            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_1 == 0) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_1 == 1))
            {
            }
            else
            {
                Center_SynthParam_ENV_1();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_ENV_1_PARAMS;
            }
        }  

        // Center ENV_2
        if(Check_Mouse(776, (Cur_Height - 77) - 1, 16, 16))
        {
            if(!Allow_Phase_Distortion_OSC_1 && (Pos_Tbl_Synth_ENV_2 == 0) ||
               !Allow_Phase_Distortion_OSC_2 && (Pos_Tbl_Synth_ENV_2 == 1))
            {
            }
            else
            {
                Center_SynthParam_ENV_2();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_ENV_2_PARAMS;
            }
        }  

        // Center Misc
        if(Check_Mouse(776, (Cur_Height - 41) - 1, 16, 16))
        {
            if((Allow_Phase_Distortion_OSC_1 || Allow_Phase_Distortion_OSC_2) ||
               Pos_Tbl_Synth_Misc > 0)
            {
                Center_SynthParam_Misc();
                gui_action = GUI_CMD_UPDATE_SYNTH_ED;
                teac = UPDATE_SYNTH_ED_Misc_PARAMS;
            }
        }  

        // OSC_1 Waveform
        if(Check_Mouse(24, (Cur_Height - 95) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_1_waveform = 0;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(49, (Cur_Height - 95) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_1_waveform = 1;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(74, (Cur_Height - 95) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_1_waveform = 2;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(99, (Cur_Height - 95) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_1_waveform = 3;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(124, (Cur_Height - 95) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_1_waveform = 6;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(149, (Cur_Height - 95) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_1_waveform = 5;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(174, (Cur_Height - 95) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_1_waveform = 4;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }

        // OSC_2 Waveform
        if(Check_Mouse(24, (Cur_Height - 41) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_2_waveform = 0;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(49, (Cur_Height - 41) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_2_waveform = 1;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(74, (Cur_Height - 41) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_2_waveform = 2;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(99, (Cur_Height - 41) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_2_waveform = 3;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(124, (Cur_Height - 41) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_2_waveform = 6;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(149, (Cur_Height - 41) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_2_waveform = 5;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(174, (Cur_Height - 41) - 1, 23, 16))
        {
            PARASynth[Current_Instrument].osc_2_waveform = 4;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }

        // Oscillators combiner
        if(Check_Mouse(4, (Cur_Height - 95) - 1 + (18 * 0), 17, 16))
        {
            PARASynth[Current_Instrument].osc_combine = COMBINE_ADD;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(4, (Cur_Height - 95) - 1 + (18 * 1), 17, 16))
        {
            PARASynth[Current_Instrument].osc_combine = COMBINE_SUB;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(4, (Cur_Height - 95) - 1 + (18 * 2), 17, 16))
        {
            PARASynth[Current_Instrument].osc_combine = COMBINE_MUL;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(4, (Cur_Height - 95) - 1 + (18 * 3), 17, 16))
        {
            PARASynth[Current_Instrument].osc_combine = COMBINE_DIV;
            teac = UPDATE_SYNTH_ED_ALL;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }

        // Sub oscillator switch
        if(Check_Mouse(237, (Cur_Height - 95) - 1, 24, 16))
        {
            PARASynth[Current_Instrument].osc_3_switch = TRUE;
            teac = UPDATE_SYNTH_ED_OSC_3_SWITCH;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(264, (Cur_Height - 95) - 1, 24, 16))
        {
            PARASynth[Current_Instrument].osc_3_switch = FALSE;
            teac = UPDATE_SYNTH_ED_OSC_3_SWITCH;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }

        // VCF Type
        if(Check_Mouse(349, (Cur_Height - 150), 17, 16) && PARASynth[Current_Instrument].vcf_type > 0)
        {
            PARASynth[Current_Instrument].vcf_type = filter_vcf_type_minus[PARASynth[Current_Instrument].vcf_type];
            teac = UPDATE_SYNTH_ED_VCF_TYPE;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }
        if(Check_Mouse(349 + 84, (Cur_Height - 150), 17, 16) && PARASynth[Current_Instrument].vcf_type <= 4)
        {
            PARASynth[Current_Instrument].vcf_type = filter_vcf_type_plus[PARASynth[Current_Instrument].vcf_type];
            teac = UPDATE_SYNTH_ED_VCF_TYPE;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
        }

        // OSC sync
        if(Check_Mouse(208, (Cur_Height - 41), 31, 16))
        {
            PARASynth[Current_Instrument].osc_sync ^= TRUE;
            teac = UPDATE_SYNTH_ED_SYNC;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;

        }   
    }
}

void Actualize_Synth_Params_Sliders(void)
{

    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    // -----------------------------
    switch(Pos_Tbl_Synth_OSC_1)
    {
        case 0: /* Osc1_PD */
            if(Allow_Phase_Distortion_OSC_1)
            {
                Real_Slider(41, (Cur_Height - 113) - 1, Cur_SynthParam->osc_1_pw / 4, Allow_All);
            }
            else
            {
                Real_Slider(41, (Cur_Height - 113) - 1, Cur_SynthParam->osc_1_pw / 4, FALSE);
            }
            if(Cur_SynthParam->osc_1_pw - 256)
            {
                outfloat_small(208, (Cur_Height - 113) - 1, (((float) Cur_SynthParam->osc_1_pw - 256) / 256.0f) * 100.0f, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(208, (Cur_Height - 113) - 1, 62, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);
            }
            csynth_slv_OSC_1 = Cur_SynthParam->osc_1_pw / 4;
            break;

        case 1: /* ENV_1_Attack */
            Real_Slider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_attack / 512, Allow_All);
            Print_Long_Small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env_1_attack * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_attack / 512;
            break;

        case 2: /* ENV_1_Decay */
            Real_Slider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_decay / 512, Allow_All);
            Print_Long_Small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env_1_decay * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_decay / 512;
            break;

        case 3: /* ENV_1_Sustain */
            Real_Slider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_sustain, Allow_All);
            Print_Long_Small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env_1_sustain * 100) / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_sustain;
            break;

        case 4: /* ENV_1_Release */
            Real_Slider(41, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_release / 512, Allow_All);
            Print_Long_Small(208, (Cur_Height - 113) - 1, (Cur_SynthParam->env_1_release * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_release / 512;
            break;

        case 5: /* OSC_3 volume */
            Real_Slider(41, (Cur_Height - 113) - 1, Cur_SynthParam->osc_3_volume, Allow_All);
            outfloat_small(208, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->osc_3_volume - 64.0f) * 1.5625f, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_1 = Cur_SynthParam->osc_3_volume;
            break;

        case 6: /* OSC_3 interval */
            Real_Slider(41, (Cur_Height - 113) - 1, (int) ((float) Cur_SynthParam->osc_3_interval / 12.0f * 128.0f), Allow_All);
            Print_Long_Small(208, (Cur_Height - 113) - 1, Cur_SynthParam->osc_3_interval, 13, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_1 = temp_osc_3_interval;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_OSC_2)
    {
        case 0: /* Osc2_PD */
            if(Allow_Phase_Distortion_OSC_2)
            {
                Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc_2_pw / 4, Allow_All);
            }
            else
            {
                Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc_2_pw / 4, FALSE);
            }
            if(Cur_SynthParam->osc_2_pw - 256)
            {
                outfloat_small(208, (Cur_Height - 59) - 1, (((float) Cur_SynthParam->osc_2_pw - 256) / 256.0f) * 100.0f, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(208, (Cur_Height - 59) - 1, 62, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_OSC_2 = Cur_SynthParam->osc_2_pw / 4;
            break;

        case 1: /* Osc2_Detune */
            Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc_2_detune, Allow_All);
            Print_Long_Small(208, (Cur_Height - 59) - 1, Cur_SynthParam->osc_2_detune - 64, 0, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_2 = Cur_SynthParam->osc_2_detune;
            break;

        case 2: /* Osc2_Finetune */
            Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->osc_2_finetune, Allow_All);
            Print_Long_Small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->osc_2_finetune * 100) / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_2 = Cur_SynthParam->osc_2_finetune;
            break;

        case 3: /* ENV_2_Attack */
            Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env_2_attack / 512, Allow_All);
            Print_Long_Small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env_2_attack * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_attack / 512;
            break;

        case 4: /* ENV_2_Decay */
            Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env_2_decay / 512, Allow_All);
            Print_Long_Small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env_2_decay * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_decay / 512;
            break;

        case 5: /* ENV_2_Sustain */
            Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env_2_sustain, Allow_All);
            Print_Long_Small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env_2_sustain * 100) / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_sustain;
            break;

        case 6: /* ENV_2_Release */
            Real_Slider(41, (Cur_Height - 59) - 1, Cur_SynthParam->env_2_release / 512, Allow_All);
            Print_Long_Small(208, (Cur_Height - 59) - 1, (Cur_SynthParam->env_2_release * 100) / 512 / 128, 1, 63, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_release / 512;
            break;
    // -----------------------------
    }

    // -----------------------------
    switch(Pos_Tbl_Synth_VCF)
    {
        case 0: /* VCF Cutoff */
            Real_Slider(311, (Cur_Height - 113) - 1, Cur_SynthParam->vcf_cutoff, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 113) - 1, Cur_SynthParam->vcf_cutoff * 172, 3, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_VCF = Cur_SynthParam->vcf_cutoff;
            break;

        case 1: /* VCF_Resonance */
            Real_Slider(311, (Cur_Height - 113) - 1, Cur_SynthParam->vcf_resonance, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 113) - 1, (Cur_SynthParam->vcf_resonance * 100) / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_VCF = Cur_SynthParam->vcf_resonance;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_LFO_1)
    {
        case 0: /* LFO_1_Period */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_period, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, (float) Cur_SynthParam->lfo_1_period * 2, 8, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_period;
            break;

        case 1: /* lfo_1->osc_1 pd */
            if(Allow_Phase_Distortion_OSC_1)
            {
                Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_1_pw, Allow_All);
            }
            else
            {
                Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_1_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_osc_1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_1_pw;
            break;

        case 2: /* lfo_1->osc_2 pd */
            if(Allow_Phase_Distortion_OSC_2)
            {
                Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_2_pw, Allow_All);
            }
            else
            {
                Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_2_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_osc_2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_2_pw;
            break;

        case 3: /* lfo_1->osc_1 pitch */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_1_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_osc_1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_1_pitch;
            break;

        case 4: /* lfo_1->osc_1 pitch */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_2_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_osc_2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_2_pitch;
            break;

        case 5: /* lfo_1->osc_1 volume */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_1_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_osc_1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_1_volume;
            break;

        case 6: /* lfo_1->osc_2 volume */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_osc_2_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_osc_2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_2_volume;
            break;

        case 7: /* lfo_1->vcf cutoff */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_vcf_cutoff, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_vcf_cutoff;
            break;

        case 8: /* lfo_1->vcf resonance */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_vcf_resonance, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_vcf_resonance;
            break;

        case 9: /* lfo_1->distortion */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_disto, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->lfo_1_disto - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_disto;
            break;

        case 10: /* LFO_1_Attack */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_attack / 512, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo_1_attack * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_attack / 512;
            break;

        case 11: /* LFO_1_Decay */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_decay / 512, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo_1_decay * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_decay / 512;
            break;

        case 12: /* LFO_1_Sustain */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_sustain, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo_1_sustain * 100) / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_sustain;
            break;

        case 13: /* LFO_1_Release */
            Real_Slider(311, (Cur_Height - 77) - 1, Cur_SynthParam->lfo_1_release / 512, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 77) - 1, (Cur_SynthParam->lfo_1_release * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_release / 512;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_LFO_2)
    {
        case 0: /* LFO_2_Period */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_period, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, (float) Cur_SynthParam->lfo_2_period * 2, 8, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_period;
            break;

        case 1: /* lfo_2->osc_1 pd */
            if(Allow_Phase_Distortion_OSC_1)
            {
                Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_1_pw, Allow_All);
            }
            else
            {
                Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_1_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_osc_1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_1_pw;
            break;

        case 2: /* lfo_2->osc_2 pd */
            if(Allow_Phase_Distortion_OSC_2)
            {
                Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_2_pw, Allow_All);
            }
            else
            {
                Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_2_pw, FALSE);
            }
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_osc_2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_2_pw;
            break;

        case 3: /* lfo_2->osc_1 pitch */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_1_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_osc_1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_1_pitch;
            break;

        case 4: /* lfo_2->osc_2 pitch */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_2_pitch, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_osc_2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_2_pitch;
            break;

        case 5: /* lfo_2->osc_1 volume */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_1_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_osc_1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_1_volume;
            break;

        case 6: /* lfo_2->osc_2 volume */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_osc_2_volume, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_osc_2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_2_volume;
            break;

        case 7: /* lfo_2->vcf cutoff */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_vcf_cutoff, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_vcf_cutoff;
            break;

        case 8: /* lfo_2->vcf resonance */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_vcf_resonance, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_vcf_resonance;
            break;

        case 9: /* lfo_2->distortion */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_disto, Allow_All);
            outfloat_small(311 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->lfo_2_disto - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_disto;
            break;

        case 10: /* LFO_2_Attack */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_attack / 512, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo_2_attack * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_attack / 512;
            break;

        case 11: /* LFO_2_Decay */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_decay / 512, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo_2_decay * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_decay / 512;
            break;

        case 12: /* LFO_2_Sustain */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_sustain, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo_2_sustain * 100) / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_sustain;
            break;

        case 13: /* LFO_2_Release */
            Real_Slider(311, (Cur_Height - 41) - 1, Cur_SynthParam->lfo_2_release / 512, Allow_All);
            Print_Long_Small(311 + 167, (Cur_Height - 41) - 1, (Cur_SynthParam->lfo_2_release * 100) / 512 / 128, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_release / 512;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_ENV_1)
    {
        case 0: /* Env1->osc_1 pd */
            if(Allow_Phase_Distortion_OSC_1)
            {
                Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_1_pw, Allow_All);
            }
            else
            {
                Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_1_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_osc_1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_1_pw;
            break;

        case 1: /* Env1->osc_2 pd */
            if(Allow_Phase_Distortion_OSC_2)
            {
                Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_2_pw, Allow_All);
            }
            else
            {
                Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_2_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_osc_2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_2_pw;
            break;

        case 2: /* Env1->osc_1 pitch */
            Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_1_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_osc_1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_1_pitch;
            break;

        case 3: /* Env1->osc_2 pitch */
            Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_2_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_osc_2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_2_pitch;
            break;

        case 4: /* Env1->osc_1 volume */
            Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_1_volume, Allow_All);
            if(Cur_SynthParam->env_1_osc_1_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_osc_1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 113) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_1_volume;
            break;

        case 5: /* Env1->osc_2 volume */
            Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_osc_2_volume, Allow_All);
            if(Cur_SynthParam->env_1_osc_2_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_osc_2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 113) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_2_volume;
            break;

        case 6: /* Env1->vcf cutoff */
            Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_vcf_cutoff, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_vcf_cutoff;
            break;

        case 7: /* Env1->vcf resonance */
            Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_vcf_resonance, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_vcf_resonance;
            break;

        case 8: /* Env1->distortion */
            Real_Slider(563, (Cur_Height - 113) - 1, Cur_SynthParam->env_1_disto, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 113) - 1, ((float) Cur_SynthParam->env_1_disto - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_disto;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_ENV_2)
    {
        case 0: /* Env2->osc_1 pw */
            if(Allow_Phase_Distortion_OSC_1)
            {
                Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_1_pw, Allow_All);
            }
            else
            {
                Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_1_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_osc_1_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_1_pw;
            break;

        case 1: /* Env2->osc_2 pw */
            if(Allow_Phase_Distortion_OSC_2)
            {
                Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_2_pw, Allow_All);
            }
            else
            {
                Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_2_pw, FALSE);
            }
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_osc_2_pw - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_2_pw;
            break;

        case 2: /* Env2->osc_1 pitch */
            Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_1_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_osc_1_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_1_pitch;
            break;

        case 3: /* Env2->osc_2 pitch */
            Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_2_pitch, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_osc_2_pitch - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_2_pitch;
            break;

        case 4: /* Env2->osc_1 volume */
            Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_1_volume, Allow_All);
            if(Cur_SynthParam->env_2_osc_1_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_osc_1_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 77) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_1_volume;
            break;

        case 5: /* Env2->osc_2 volume */
            Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_osc_2_volume, Allow_All);
            if(Cur_SynthParam->env_2_osc_2_volume != 64)
            {
                outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_osc_2_volume - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            }
            else
            {
                Gui_Draw_Button_Box(563 + 167, (Cur_Height - 77) - 1, 44, 16, "off", BUTTON_NORMAL | BUTTON_DISABLED);           
            }
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_2_volume;
            break;

        case 6: /* Env2->vcf cutoff */
            Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_vcf_cutoff, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_vcf_cutoff - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_vcf_cutoff;
            break;

        case 7: /* Env2->vcf resonance */
            Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_vcf_resonance, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_vcf_resonance - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_vcf_resonance;
            break;

        case 8: /* Env2->distortion */
            Real_Slider(563, (Cur_Height - 77) - 1, Cur_SynthParam->env_2_disto, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 77) - 1, ((float) Cur_SynthParam->env_2_disto - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_disto;
            break;
    }
    // -----------------------------

    // -----------------------------
    switch(Pos_Tbl_Synth_Misc)
    {
        case 0: /* Misc. glide */
            if((Allow_Phase_Distortion_OSC_1 || Allow_Phase_Distortion_OSC_2) ||
               Pos_Tbl_Synth_Misc > 0)
            {
                Real_Slider(563, (Cur_Height - 41) - 1, Cur_SynthParam->ptc_glide, Allow_All);
            }
            else
            {
                Real_Slider(563, (Cur_Height - 41) - 1, Cur_SynthParam->ptc_glide, FALSE);
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
            Real_Slider(563, (Cur_Height - 41) - 1, Cur_SynthParam->glb_volume, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 41) - 1, ((float) Cur_SynthParam->glb_volume) * 0.78125f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_Misc = Cur_SynthParam->glb_volume;
            break;

        case 2: /* Misc. Distortion */
            Real_Slider(563, (Cur_Height - 41) - 1, Cur_SynthParam->disto, Allow_All);
            outfloat_small(563 + 167, (Cur_Height - 41) - 1, (((float) Cur_SynthParam->disto) - 64.0f) * 1.5625f, 1, 45, BUTTON_NORMAL | BUTTON_DISABLED);
            csynth_slv_Misc = Cur_SynthParam->disto;
            break;
    }
    // -----------------------------
}

void Center_SynthParam_OSC_1(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_OSC_1)
    {
        case 0: /* Osc1_PD */
            Cur_SynthParam->osc_1_pw = 64 * 4;
            csynth_slv_OSC_1 = Cur_SynthParam->osc_1_pw / 4;
            break;

        case 1: /* ENV_1_Attack */
            Cur_SynthParam->env_1_attack = 64 * 512;
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_attack / 512;
            break;

        case 2: /* ENV_1_Decay */
            Cur_SynthParam->env_1_decay = 64 * 512;
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_decay / 512;
            break;

        case 3: /* ENV_1_Sustain */
            Cur_SynthParam->env_1_sustain = 64;
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_sustain;
            break;

        case 4: /* ENV_1_Release */
            Cur_SynthParam->env_1_release = 64 * 512;
            csynth_slv_OSC_1 = Cur_SynthParam->env_1_release / 512;
            break;

        case 5: /* OSC_3 volume */
            Cur_SynthParam->osc_3_volume = 64;
            csynth_slv_OSC_1 = Cur_SynthParam->osc_3_volume;
            break;

        case 6: /* OSC_3 interval */
            temp_osc_3_interval = 64;
            Cur_SynthParam->osc_3_interval = (int) (((float) temp_osc_3_interval / 128.0f) * 12.0f);
            if(Cur_SynthParam->osc_3_interval < 0) Cur_SynthParam->osc_3_interval = 0;
            if(Cur_SynthParam->osc_3_interval > 12) Cur_SynthParam->osc_3_interval = 12;
            csynth_slv_OSC_1 = temp_osc_3_interval;
            break;

    }
}

void Center_SynthParam_OSC_2(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_OSC_2)
    {
        case 0: /* Osc2_PD */
            Cur_SynthParam->osc_2_pw = 64 * 4;
            csynth_slv_OSC_2 = Cur_SynthParam->osc_2_pw / 4;
            break;

        case 1: /* Osc2_Detune */
            Cur_SynthParam->osc_2_detune = 64;
            csynth_slv_OSC_2 = Cur_SynthParam->osc_2_detune;
            break;

        case 2: /* Osc2_Finetune */
            Cur_SynthParam->osc_2_finetune = 64;
            csynth_slv_OSC_2 = Cur_SynthParam->osc_2_finetune;
            break;

        case 3: /* ENV_2_Attack */
            Cur_SynthParam->env_2_attack = 64 * 512;
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_attack / 512;
            break;

        case 4: /* ENV_2_Decay */
            Cur_SynthParam->env_2_decay = 64 * 512;
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_decay / 512;
            break;

        case 5: /* ENV_2_Sustain */
            Cur_SynthParam->env_2_sustain = 64;
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_sustain;
            break;

        case 6: /* ENV_2_Release */
            Cur_SynthParam->env_2_release = 64 * 512;
            csynth_slv_OSC_2 = Cur_SynthParam->env_2_release / 512;
            break;
    }
}

void Center_SynthParam_VCF(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

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

void Center_SynthParam_LFO_1(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_LFO_1)
    {
        case 0: /* LFO_1_Period */
            Cur_SynthParam->lfo_1_period = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_period;
            break;

        case 1: /* lfo_1->osc_1 pd */
            Cur_SynthParam->lfo_1_osc_1_pw = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_1_pw;
            break;

        case 2: /* lfo_1->osc_2 pd */
            Cur_SynthParam->lfo_1_osc_2_pw = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_2_pw;
            break;

        case 3: /* lfo_1->osc_1 pitch */
            Cur_SynthParam->lfo_1_osc_1_pitch = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_1_pitch;
            break;

        case 4: /* lfo_1->osc_1 pitch */
            Cur_SynthParam->lfo_1_osc_2_pitch = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_2_pitch;
            break;

        case 5: /* lfo_1->osc_1 volume */
            Cur_SynthParam->lfo_1_osc_1_volume = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_1_volume;
            break;

        case 6: /* lfo_1->osc_2 volume */
            Cur_SynthParam->lfo_1_osc_2_volume = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_osc_2_volume;
            break;

        case 7: /* lfo_1->vcf cutoff */
            Cur_SynthParam->lfo_1_vcf_cutoff = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_vcf_cutoff;
            break;

        case 8: /* lfo_1->vcf resonance */
            Cur_SynthParam->lfo_1_vcf_resonance = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_vcf_resonance;
            break;

        case 9: /* lfo_1->distortion */
            Cur_SynthParam->lfo_1_disto = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_disto;
            break;

        case 10: /* LFO_1_Attack */
            Cur_SynthParam->lfo_1_attack = 64 * 512;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_attack / 512;
            break;

        case 11: /* LFO_1_Decay */
            Cur_SynthParam->lfo_1_decay = 64 * 512;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_decay / 512;
            break;

        case 12: /* LFO_1_Sustain */
            Cur_SynthParam->lfo_1_sustain = 64;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_sustain;
            break;

        case 13: /* LFO_1_Release */
            Cur_SynthParam->lfo_1_release = 64 * 512;
            csynth_slv_LFO_1 = Cur_SynthParam->lfo_1_release / 512;
            break;
    }
}

void Center_SynthParam_LFO_2(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_LFO_2)
    {
        case 0: /* LFO_2_Period */
            Cur_SynthParam->lfo_2_period = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_period;
            break;

        case 1: /* lfo_2->osc_1 pd */
            Cur_SynthParam->lfo_2_osc_1_pw = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_1_pw;
            break;

        case 2: /* lfo_2->osc_2 pd */
            Cur_SynthParam->lfo_2_osc_2_pw = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_2_pw;
            break;

        case 3: /* lfo_2->osc_1 pitch */
            Cur_SynthParam->lfo_2_osc_1_pitch = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_1_pitch;
            break;

        case 4: /* lfo_2->osc_2 pitch */
            Cur_SynthParam->lfo_2_osc_2_pitch = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_2_pitch;
            break;

        case 5: /* lfo_2->osc_1 volume */
            Cur_SynthParam->lfo_2_osc_1_volume = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_1_volume;
            break;

        case 6: /* lfo_2->osc_2 volume */
            Cur_SynthParam->lfo_2_osc_2_volume = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_osc_2_volume;
            break;

        case 7: /* lfo_2->vcf cutoff */
            Cur_SynthParam->lfo_2_vcf_cutoff = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_vcf_cutoff;
            break;

        case 8: /* lfo_2->vcf resonance */
            Cur_SynthParam->lfo_2_vcf_resonance = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_vcf_resonance;
            break;

        case 9: /* lfo_2->distortion */
            Cur_SynthParam->lfo_2_disto = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_disto;
            break;

        case 10: /* LFO_2_Attack */
            Cur_SynthParam->lfo_2_attack = 64 * 512;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_attack / 512;
            break;

        case 11: /* LFO_2_Decay */
            Cur_SynthParam->lfo_2_decay = 64 * 512;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_decay / 512;
            break;

        case 12: /* LFO_2_Sustain */
            Cur_SynthParam->lfo_2_sustain = 64;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_sustain;
            break;

        case 13: /* LFO_2_Release */
            Cur_SynthParam->lfo_2_release = 64 * 512;
            csynth_slv_LFO_2 = Cur_SynthParam->lfo_2_release / 512;
            break;
    }
}

void Center_SynthParam_ENV_1(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_ENV_1)
    {
        case 0: /* Env1->osc_1 pd */
            Cur_SynthParam->env_1_osc_1_pw = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_1_pw;
            break;

        case 1: /* Env1->osc_2 pd */
            Cur_SynthParam->env_1_osc_2_pw = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_2_pw;
            break;

        case 2: /* Env1->osc_1 pitch */
            Cur_SynthParam->env_1_osc_1_pitch = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_1_pitch;
            break;

        case 3: /* Env1->osc_2 pitch */
            Cur_SynthParam->env_1_osc_2_pitch = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_2_pitch;
            break;

        case 4: /* Env1->osc_1 volume */
            Cur_SynthParam->env_1_osc_1_volume = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_1_volume;
            break;

        case 5: /* Env1->osc_2 volume */
            Cur_SynthParam->env_1_osc_2_volume = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_osc_2_volume;
            break;

        case 6: /* Env1->vcf cutoff */
            Cur_SynthParam->env_1_vcf_cutoff = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_vcf_cutoff;
            break;

        case 7: /* Env1->vcf resonance */
            Cur_SynthParam->env_1_vcf_resonance = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_vcf_resonance;
            break;

        case 8: /* Env1->distortion */
            Cur_SynthParam->env_1_disto = 64;
            csynth_slv_ENV_1 = Cur_SynthParam->env_1_disto;
            break;
    }
}

void Center_SynthParam_ENV_2(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

    switch(Pos_Tbl_Synth_ENV_2)
    {
        case 0: /* Env2->osc_1 pd */
            Cur_SynthParam->env_2_osc_1_pw = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_1_pw;
            break;

        case 1: /* Env2->osc_2 pd */
            Cur_SynthParam->env_2_osc_2_pw = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_2_pw;
            break;

        case 2: /* Env2->osc_1 pitch */
            Cur_SynthParam->env_2_osc_1_pitch = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_1_pitch;
            break;

        case 3: /* Env2->osc_2 pitch */
            Cur_SynthParam->env_2_osc_2_pitch = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_2_pitch;
            break;

        case 4: /* Env2->osc_1 volume */
            Cur_SynthParam->env_2_osc_1_volume = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_1_volume;
            break;

        case 5: /* Env2->osc_2 volume */
            Cur_SynthParam->env_2_osc_2_volume = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_osc_2_volume;
            break;

        case 6: /* Env2->vcf cutoff */
            Cur_SynthParam->env_2_vcf_cutoff = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_vcf_cutoff;
            break;

        case 7: /* Env2->vcf resonance */
            Cur_SynthParam->env_2_vcf_resonance = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_vcf_resonance;
            break;

        case 8: /* Env2->distortion */
            Cur_SynthParam->env_2_disto = 64;
            csynth_slv_ENV_2 = Cur_SynthParam->env_2_disto;
            break;

    }
}

void Center_SynthParam_Misc(void)
{
    Synth_Parameters *Cur_SynthParam = &PARASynth[Current_Instrument];

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

void CParcha_OSC_1(int cpar)
{
    switch(Pos_Tbl_Synth_OSC_1)
    {
        case 0: /* Osc1_PD */
            PARASynth[Current_Instrument].osc_1_pw = cpar * 4;
            break;

        case 1: /* ENV_1_Attack */
            PARASynth[Current_Instrument].env_1_attack = cpar * 512;
            break;

        case 2: /* ENV_1_Decay */
            PARASynth[Current_Instrument].env_1_decay = cpar * 512;
            break;

        case 3: /* ENV_1_Sustain */
            PARASynth[Current_Instrument].env_1_sustain = cpar;
            break;

        case 4: /* ENV_1_Release */
            PARASynth[Current_Instrument].env_1_release = cpar * 512;
            break;

        case 5: /* OSC_3 volume */
            PARASynth[Current_Instrument].osc_3_volume = cpar;
            break;

        case 6: /* OSC_3 interval */
            temp_osc_3_interval = cpar;
            PARASynth[Current_Instrument].osc_3_interval = (int) (((float) temp_osc_3_interval / 128.0f) * 12.0f);
            if(PARASynth[Current_Instrument].osc_3_interval < 0) PARASynth[Current_Instrument].osc_3_interval = 0;
            if(PARASynth[Current_Instrument].osc_3_interval > 12) PARASynth[Current_Instrument].osc_3_interval = 12;
            break;
    }
}

void CParcha_OSC_2(int cpar)
{
    switch(Pos_Tbl_Synth_OSC_2)
    {
        case 0: /* Osc2_PD */
            PARASynth[Current_Instrument].osc_2_pw = cpar * 4;
            break;

        case 1: /* Osc2_Detune */
            PARASynth[Current_Instrument].osc_2_detune = cpar;
            break;

        case 2: /* Osc2_Finetune */
            PARASynth[Current_Instrument].osc_2_finetune = cpar;
            break;

        case 3: /* ENV_2_Attack */
            PARASynth[Current_Instrument].env_2_attack = cpar * 512;
            break;

        case 4: /* ENV_2_Decay */
            PARASynth[Current_Instrument].env_2_decay = cpar * 512;
            break;

        case 5: /* ENV_2_Sustain */
            PARASynth[Current_Instrument].env_2_sustain = cpar;
            break;

        case 6: /* ENV_2_Release */
            PARASynth[Current_Instrument].env_2_release = cpar * 512;
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

void CParcha_LFO_1(int cpar)
{
    switch(Pos_Tbl_Synth_LFO_1)
    {
        case 0: /* lfo_1_Period */
            PARASynth[Current_Instrument].lfo_1_period = cpar;
            break;

        case 1: /* lfo_1->osc_1 pd */
            PARASynth[Current_Instrument].lfo_1_osc_1_pw = cpar;
            break;

        case 2: /* lfo_1->osc_2 pd */
            PARASynth[Current_Instrument].lfo_1_osc_2_pw = cpar;
            break;

        case 3: /* lfo_1->osc_1 pitch */
            PARASynth[Current_Instrument].lfo_1_osc_1_pitch = cpar;
            break;

        case 4: /* lfo_1->osc_1 pitch */
            PARASynth[Current_Instrument].lfo_1_osc_2_pitch = cpar;
            break;

        case 5: /* lfo_1->osc_1 volume */
            PARASynth[Current_Instrument].lfo_1_osc_1_volume = cpar;
            break;

        case 6: /* lfo_1->osc_2 volume */
            PARASynth[Current_Instrument].lfo_1_osc_2_volume = cpar;
            break;

        case 7: /* lfo_1->vcf cutoff */
            PARASynth[Current_Instrument].lfo_1_vcf_cutoff = cpar;
            break;

        case 8: /* lfo_1->vcf resonance */
            PARASynth[Current_Instrument].lfo_1_vcf_resonance = cpar;
            break;

        case 9: /* lfo_1->distortion */
            PARASynth[Current_Instrument].lfo_1_disto = cpar;
            break;

        case 10: /* LFO_1_Attack */
            PARASynth[Current_Instrument].lfo_1_attack = cpar * 512;
            break;

        case 11: /* LFO_1_Decay */
            PARASynth[Current_Instrument].lfo_1_decay = cpar * 512;
            break;

        case 12: /* LFO_1_Sustain */
            PARASynth[Current_Instrument].lfo_1_sustain = cpar;
            break;

        case 13: /* LFO_1_Release */
            PARASynth[Current_Instrument].lfo_1_release = cpar * 512;
            break;
    }
}

void CParcha_LFO_2(int cpar)
{
    switch(Pos_Tbl_Synth_LFO_2)
    {
        case 0: /* LFO_2_Period */
            PARASynth[Current_Instrument].lfo_2_period = cpar;
            break;

        case 1: /* lfo_2->osc_1 pd */
            PARASynth[Current_Instrument].lfo_2_osc_1_pw = cpar;
            break;

        case 2: /* lfo_2->osc_2 pd */
            PARASynth[Current_Instrument].lfo_2_osc_2_pw = cpar;
            break;

        case 3: /* lfo_2->osc_1 pitch */
            PARASynth[Current_Instrument].lfo_2_osc_1_pitch = cpar;
            break;

        case 4: /* lfo_2->osc_2 pitch */
            PARASynth[Current_Instrument].lfo_2_osc_2_pitch = cpar;
            break;

        case 5: /* lfo_2->osc_1 volume */
            PARASynth[Current_Instrument].lfo_2_osc_1_volume = cpar;
            break;

        case 6: /* lfo_2->osc_2 volume */
            PARASynth[Current_Instrument].lfo_2_osc_2_volume = cpar;
            break;

        case 7: /* lfo_2->vcf cutoff */
            PARASynth[Current_Instrument].lfo_2_vcf_cutoff = cpar;
            break;

        case 8: /* lfo_2->vcf resonance */
            PARASynth[Current_Instrument].lfo_2_vcf_resonance = cpar;
            break;

        case 9: /* lfo_2->distortion */
            PARASynth[Current_Instrument].lfo_2_disto = cpar;
            break;

        case 10: /* LFO_1_Attack */
            PARASynth[Current_Instrument].lfo_2_attack = cpar * 512;
            break;

        case 11: /* LFO_1_Decay */
            PARASynth[Current_Instrument].lfo_2_decay = cpar * 512;
            break;

        case 12: /* LFO_1_Sustain */
            PARASynth[Current_Instrument].lfo_2_sustain = cpar;
            break;

        case 13: /* LFO_1_Release */
            PARASynth[Current_Instrument].lfo_2_release = cpar * 512;
            break;
    }
}

void CParcha_ENV_1(int cpar)
{
    switch(Pos_Tbl_Synth_ENV_1)
    {
        case 0: /* Env1->osc_1 pd */
            PARASynth[Current_Instrument].env_1_osc_1_pw = cpar;
            break;

        case 1: /* Env1->osc_2 pd */
            PARASynth[Current_Instrument].env_1_osc_2_pw = cpar;
            break;

        case 2: /* Env1->osc_1 pitch */
            PARASynth[Current_Instrument].env_1_osc_1_pitch = cpar;
            break;

        case 3: /* Env1->osc_2 pitch */
            PARASynth[Current_Instrument].env_1_osc_2_pitch = cpar;
            break;

        case 4: /* Env1->osc_1 volume */
            PARASynth[Current_Instrument].env_1_osc_1_volume = cpar;
            break;

        case 5: /* Env1->osc_2 volume */
            PARASynth[Current_Instrument].env_1_osc_2_volume = cpar;
            break;

        case 6: /* Env1->vcf cutoff */
            PARASynth[Current_Instrument].env_1_vcf_cutoff = cpar;
            break;

        case 7: /* Env1->vcf resonance */
            PARASynth[Current_Instrument].env_1_vcf_resonance = cpar;
            break;

        case 8: /* Env1->distortion */
            PARASynth[Current_Instrument].env_1_disto = cpar;
            break;
    }
}

void CParcha_ENV_2(int cpar)
{
    switch(Pos_Tbl_Synth_ENV_2)
    {
        case 0: /* Env2->osc_1 pd */
            PARASynth[Current_Instrument].env_2_osc_1_pw = cpar;
            break;

        case 1: /* Env2->osc_2 pd */
            PARASynth[Current_Instrument].env_2_osc_2_pw = cpar;
            break;

        case 2: /* Env2->osc_1 pitch */
            PARASynth[Current_Instrument].env_2_osc_1_pitch = cpar;
            break;

        case 3: /* Env2->osc_2 pitch */
            PARASynth[Current_Instrument].env_2_osc_2_pitch = cpar;
            break;

        case 4: /* Env2->osc_1 volume */
            PARASynth[Current_Instrument].env_2_osc_1_volume = cpar;
            break;

        case 5: /* Env2->osc_2 volume */
            PARASynth[Current_Instrument].env_2_osc_2_volume = cpar;
            break;

        case 6: /* Env2->vcf cutoff */
            PARASynth[Current_Instrument].env_2_vcf_cutoff = cpar;
            break;

        case 7: /* Env2->vcf resonance */
            PARASynth[Current_Instrument].env_2_vcf_resonance = cpar;
            break;

        case 8: /* Env2->distortion */
            PARASynth[Current_Instrument].env_2_disto = cpar;
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

void Rand_OSC_1()
{
    PARASynth[Current_Instrument].osc_1_waveform = rand() % 7;
    if(Allow_Phase_Distortion_OSC_1)
    {
        PARASynth[Current_Instrument].osc_1_pw = rand() & 0x1ff;
    }
    PARASynth[Current_Instrument].env_1_attack = rand() & 0xffff;
    PARASynth[Current_Instrument].env_1_decay = rand() & 0xffff;
    PARASynth[Current_Instrument].env_1_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].env_1_release = rand() & 0xffff;
    PARASynth[Current_Instrument].osc_3_volume = rand() & 0x7f;
    temp_osc_3_interval = (rand() & 0x7f);
    PARASynth[Current_Instrument].osc_3_interval = (int) (((float) temp_osc_3_interval / 128.0f) * 12.0f);
    if(PARASynth[Current_Instrument].osc_3_interval < 0) PARASynth[Current_Instrument].osc_3_interval = 0;
    if(PARASynth[Current_Instrument].osc_3_interval > 12) PARASynth[Current_Instrument].osc_3_interval = 12;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_OSC_2()
{
    PARASynth[Current_Instrument].osc_2_waveform = rand() % 7;
    if(Allow_Phase_Distortion_OSC_2)
    {
        PARASynth[Current_Instrument].osc_2_pw = rand() & 0x1ff;
    }
    PARASynth[Current_Instrument].osc_2_finetune = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_attack = rand()  & 0xffff;
    PARASynth[Current_Instrument].env_2_decay = rand()  & 0xffff;
    PARASynth[Current_Instrument].env_2_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_release = rand() & 0xffff;
    PARASynth[Current_Instrument].osc_2_detune = rand() & 0x7f;

    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_VCF()
{
    PARASynth[Current_Instrument].vcf_type = rand() % 5;
    PARASynth[Current_Instrument].vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].vcf_resonance = rand() & 0x7f;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_LFO_1()
{
    PARASynth[Current_Instrument].lfo_1_period = rand() & 0x7f;
    if(Allow_Phase_Distortion_OSC_1)
    {
        PARASynth[Current_Instrument].lfo_1_osc_1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC_2)
    {
        PARASynth[Current_Instrument].lfo_1_osc_2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].lfo_1_osc_1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_1_osc_2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_1_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_1_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_1_disto = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_1_osc_1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_1_osc_2_pitch = rand() & 0x7f;

    PARASynth[Current_Instrument].lfo_1_attack = rand() & 0xffff;
    PARASynth[Current_Instrument].lfo_1_decay = rand() & 0xffff;
    PARASynth[Current_Instrument].lfo_1_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_1_release = rand() & 0xffff;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_LFO_2()
{
    PARASynth[Current_Instrument].lfo_2_period = rand() & 0x7f;
    if(Allow_Phase_Distortion_OSC_1)
    {
        PARASynth[Current_Instrument].lfo_2_osc_1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC_2)
    {
        PARASynth[Current_Instrument].lfo_2_osc_2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].lfo_2_osc_1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_2_osc_2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_2_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_2_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_2_disto = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_2_osc_1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_2_osc_2_pitch = rand() & 0x7f;

    PARASynth[Current_Instrument].lfo_2_attack = rand() & 0xffff;
    PARASynth[Current_Instrument].lfo_2_decay = rand() & 0xffff;
    PARASynth[Current_Instrument].lfo_2_sustain = rand() & 0x7f;
    PARASynth[Current_Instrument].lfo_2_release = rand() & 0xffff;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_ENV_1()
{
    if(Allow_Phase_Distortion_OSC_1)
    {
        PARASynth[Current_Instrument].env_1_osc_1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC_2)
    {
        PARASynth[Current_Instrument].env_1_osc_2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].env_1_osc_1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env_1_osc_2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env_1_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].env_1_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].env_1_disto = rand() & 0x7f;
    PARASynth[Current_Instrument].env_1_osc_1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].env_1_osc_2_pitch = rand() & 0x7f;

    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_ENV_2()
{
    if(Allow_Phase_Distortion_OSC_1)
    {
        PARASynth[Current_Instrument].env_2_osc_1_pw = rand() & 0x7f;
    }
    if(Allow_Phase_Distortion_OSC_2)
    {
        PARASynth[Current_Instrument].env_2_osc_2_pw = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].env_2_osc_1_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_osc_2_volume = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_vcf_cutoff = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_vcf_resonance = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_disto = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_osc_1_pitch = rand() & 0x7f;
    PARASynth[Current_Instrument].env_2_osc_2_pitch = rand() & 0x7f;

    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Rand_Misc()
{
    if(Allow_Phase_Distortion_OSC_1 || Allow_Phase_Distortion_OSC_2)
    {
        PARASynth[Current_Instrument].ptc_glide = rand() & 0x7f;
    }
    PARASynth[Current_Instrument].glb_volume = 100;
    PARASynth[Current_Instrument].disto = rand() & 0x7f;
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_VALUES);
}

void Init_Synth_Params_Names(void)
{
    sprintf(Names_Tbl_Synth_OSC_1[0], "Pulse Phase");
    sprintf(Names_Tbl_Synth_OSC_1[1], "Attack");
    sprintf(Names_Tbl_Synth_OSC_1[2], "Decay");
    sprintf(Names_Tbl_Synth_OSC_1[3], "Sustain Level");
    sprintf(Names_Tbl_Synth_OSC_1[4], "Release");
    sprintf(Names_Tbl_Synth_OSC_1[5], "Sub-Oscillator Volume");
    sprintf(Names_Tbl_Synth_OSC_1[6], "Sub-Oscillator Interval");

    sprintf(Names_Tbl_Synth_OSC_2[0], "Pulse Phase");
    sprintf(Names_Tbl_Synth_OSC_2[1], "Detune");
    sprintf(Names_Tbl_Synth_OSC_2[2], "Finetune");
    sprintf(Names_Tbl_Synth_OSC_2[3], "Attack");
    sprintf(Names_Tbl_Synth_OSC_2[4], "Decay");
    sprintf(Names_Tbl_Synth_OSC_2[5], "Sustain Level");
    sprintf(Names_Tbl_Synth_OSC_2[6], "Release");

    sprintf(Names_Tbl_Synth_VCF[0], "CutOff");
    sprintf(Names_Tbl_Synth_VCF[1], "Resonance");

    sprintf(Names_Tbl_Synth_LFO_1[0], "Period");
    sprintf(Names_Tbl_Synth_LFO_1[1], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO_1[2], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO_1[3], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_LFO_1[4], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_LFO_1[5], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_LFO_1[6], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_LFO_1[7], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_LFO_1[8], "-> Vcf Resonance");
    sprintf(Names_Tbl_Synth_LFO_1[9], "-> Distortion");
    sprintf(Names_Tbl_Synth_LFO_1[10], "Attack");
    sprintf(Names_Tbl_Synth_LFO_1[11], "Decay");
    sprintf(Names_Tbl_Synth_LFO_1[12], "Sustain Level");
    sprintf(Names_Tbl_Synth_LFO_1[13], "Release");

    sprintf(Names_Tbl_Synth_LFO_2[0], "Period");
    sprintf(Names_Tbl_Synth_LFO_2[1], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO_2[2], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_LFO_2[3], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_LFO_2[4], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_LFO_2[5], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_LFO_2[6], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_LFO_2[7], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_LFO_2[8], "-> Vcf Resonance");
    sprintf(Names_Tbl_Synth_LFO_2[9], "-> Distortion");
    sprintf(Names_Tbl_Synth_LFO_2[10], "Attack");
    sprintf(Names_Tbl_Synth_LFO_2[11], "Decay");
    sprintf(Names_Tbl_Synth_LFO_2[12], "Sustain Level");
    sprintf(Names_Tbl_Synth_LFO_2[13], "Release");

    sprintf(Names_Tbl_Synth_ENV_1[0], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV_1[1], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV_1[2], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_ENV_1[3], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_ENV_1[4], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_ENV_1[5], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_ENV_1[6], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_ENV_1[7], "-> Vcf Resonance");
    sprintf(Names_Tbl_Synth_ENV_1[8], "-> Distortion");

    sprintf(Names_Tbl_Synth_ENV_2[0], "-> Osc1 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV_2[1], "-> Osc2 Pulse Phase");
    sprintf(Names_Tbl_Synth_ENV_2[2], "-> Osc1 Pitch");
    sprintf(Names_Tbl_Synth_ENV_2[3], "-> Osc2 Pitch");
    sprintf(Names_Tbl_Synth_ENV_2[4], "-> Osc1 Volume");
    sprintf(Names_Tbl_Synth_ENV_2[5], "-> Osc2 Volume");
    sprintf(Names_Tbl_Synth_ENV_2[6], "-> Vcf CutOff");
    sprintf(Names_Tbl_Synth_ENV_2[7], "-> Vcf Resonance");
    sprintf(Names_Tbl_Synth_ENV_2[8], "-> Distortion");

    sprintf(Names_Tbl_Synth_Misc[0], "Portamento Glide");
    sprintf(Names_Tbl_Synth_Misc[1], "Final [global] Volume");
    sprintf(Names_Tbl_Synth_Misc[2], "Wave Distortion");
}
