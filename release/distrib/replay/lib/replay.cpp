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
#include "include/replay.h"
#include "include/endianness.h"

#if defined(__PSP__)
#include <stdlib.h>
#include <string.h>
#endif

#if defined(__AROS__) || defined(__HAIKU__)
#define int32 int32_t
#endif

#if !defined(__STAND_ALONE__)
#include "../../../../src/midi/include/midi.h"
#include "../../../../src/include/variables.h"
#include "../../../../src/include/ptk.h"
#endif

#if defined(BZR2)
#include <cstdio>
#endif

// ------------------------------------------------------
// Variables
#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    extern SDL_sem *thread_sema;
#endif

void Compute_Stereo_Quick(int channel);

int SamplesPerTick;

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
int SamplesPerTick_Midi;
#endif
#endif

float SQRT[1025];   // Sqrt float-precalculated table.
float Left_Buffer[16 * 1024];
float Right_Buffer[16 * 1024];

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    int Beats_Per_Min = 125;
    int Ticks_Per_Beat = 4;
    float Feedback = 0.88f;
    int c_threshold = 50;
#else
    int Beats_Per_Min;
    int Ticks_Per_Beat;
    float Feedback;
    int c_threshold;
#endif

#if defined(PTK_FX_AUTOFADEMODE)
    char FADEMODE[MAX_TRACKS];    // 0 - Off, 1- In, 2 - Out;
    float FADECOEF[MAX_TRACKS];
#endif

#if defined(PTK_SYNTH)
CSynth Synthesizer[MAX_TRACKS][MAX_POLYPHONY];
#endif

float Player_FD[MAX_TRACKS];
INSTR_SCHEDULE Instrument_Schedule_Dat[MAX_TRACKS][MAX_POLYPHONY];
char sp_channelsample[MAX_TRACKS][MAX_POLYPHONY];
char sp_channelnote[MAX_TRACKS][MAX_POLYPHONY];
char sp_split[MAX_TRACKS][MAX_POLYPHONY];
int Chan_Mute_State[MAX_TRACKS];   // 0->Normal 1->Muted
float oldspawn[MAX_TRACKS];
float roldspawn[MAX_TRACKS];
char LFO_ON[MAX_TRACKS];

int Delay_Sound_Buffer;
int Cur_Delay_Sound_Buffer;

VISUAL_DELAY_DAT Delays_Pos_Sound_Buffer[512];
int PosInTick_Delay;

#if defined(PTK_LFO)
    float LFO_RATE[MAX_TRACKS];
    float LFO_RATE_SCALE[MAX_TRACKS];
    float LFO_AMPL_FILTER[MAX_TRACKS];
    float LFO_AMPL_VOLUME[MAX_TRACKS];
    float LFO_AMPL_PANNING[MAX_TRACKS];
    float LFO_CARRIER_FILTER[MAX_TRACKS];
    float LFO_CARRIER_VOLUME[MAX_TRACKS];
    float LFO_CARRIER_PANNING[MAX_TRACKS];
#endif

char FLANGER_ON[MAX_TRACKS];

#if defined(PTK_FLANGER)
    float FLANGER_AMOUNT[MAX_TRACKS];
    float FLANGER_DEPHASE[MAX_TRACKS];
    float FLANGER_RATE[MAX_TRACKS];
    float FLANGER_AMPL[MAX_TRACKS];
    float FLANGER_GR[MAX_TRACKS];
    float FLANGER_FEEDBACK[MAX_TRACKS];
    int FLANGER_DELAY[MAX_TRACKS];
    int FLANGER_OFFSET[MAX_TRACKS];
    float FLANGER_OFFSET2[MAX_TRACKS];
    float FLANGER_OFFSET1[MAX_TRACKS];
    float FLANGE_LEFTBUFFER[MAX_TRACKS][16400];
    float FLANGE_RIGHTBUFFER[MAX_TRACKS][16400];
#endif

float sp_Cvol[MAX_TRACKS][MAX_POLYPHONY];
float sp_Cvol_Ramp[MAX_TRACKS][MAX_POLYPHONY];
float sp_Cvol_Ramp_Dest[MAX_TRACKS][MAX_POLYPHONY];
float sp_Cvol_Synth[MAX_TRACKS][MAX_POLYPHONY];
float sp_Cvol_Synth_Ramp[MAX_TRACKS][MAX_POLYPHONY];
float sp_Cvol_Synth_Ramp_Dest[MAX_TRACKS][MAX_POLYPHONY];
float sp_Tvol[MAX_TRACKS][MAX_POLYPHONY];
float sp_Tvol_Synth[MAX_TRACKS][MAX_POLYPHONY];
float sp_Tvol_Mod[MAX_TRACKS];
float DSend[MAX_TRACKS];   
int CSend[MAX_TRACKS];
float Vstep1[MAX_TRACKS][MAX_POLYPHONY];

#if defined(PTK_FX_TRANCEGLIDER)
    float Glide_Step[MAX_TRACKS];
#endif

EQSTATE EqDat[MAX_TRACKS];

#if !defined(__STAND_ALONE__)
float Default_Pan[MAX_TRACKS] =
{
     0.4f,  0.6f,
     0.3f,  0.7f,
     0.2f,  0.8f,
    0.15f, 0.85f,
    0.35f, 0.65f,
    0.45f, 0.55f,
    0.25f, 0.75f,
     0.1f,  0.9f
};
#endif

float TPan[MAX_TRACKS];
float old_TPan[MAX_TRACKS];
int old_note[MAX_TRACKS][MAX_POLYPHONY];

s_access sp_Position[MAX_TRACKS][MAX_POLYPHONY];

#if defined(PTK_SYNTH)
s_access sp_Position_osc_1[MAX_TRACKS][MAX_POLYPHONY];
s_access sp_Position_osc_2[MAX_TRACKS][MAX_POLYPHONY];

#if defined(PTK_SYNTH_OSC_3)
    s_access sp_Position_osc_3[MAX_TRACKS][MAX_POLYPHONY];
#endif
#endif

#if defined(PTK_TRACKFILTERS)
float CCut[MAX_TRACKS];
#endif

char Use_Cubic = 0;

float TCut[MAX_TRACKS];
float ICut[MAX_TRACKS];
float LVol[MAX_TRACKS];
float Old_LVol[MAX_TRACKS];
float RVol[MAX_TRACKS];
float Old_RVol[MAX_TRACKS];
int FType[MAX_TRACKS];
int FRez[MAX_TRACKS];
int ChorType;
int ChorRez;
float DThreshold[MAX_TRACKS];
char Disclap[MAX_TRACKS];
float DClamp[MAX_TRACKS];
float CCoef[MAX_TRACKS];
float lbuff_chorus[131072];
float rbuff_chorus[131072];
#if defined(PTK_FILTER_LOHIBAND)
float coef[5];
float Final_coef[5];
float coef_chorus[5];
float coeftab[5][128][128][4];
#endif

#if defined(__PSP__)
volatile int Song_Playing;
#else
int Song_Playing;
#endif

int New_Instrument[MAX_TRACKS];
int Pos_Segue[MAX_TRACKS];
float Segue_Volume[MAX_TRACKS];
float Segue_SamplesL[MAX_TRACKS];
float Segue_SamplesR[MAX_TRACKS];

#if defined(PTK_303)
    gear303 tb303engine[2];
    para303 tb303[2];
    unsigned char track3031;
    unsigned char track3032;
#endif

float left_float;
float right_float;
int pos_round_float_history[MAX_TRACKS];
float left_float_history[MAX_TRACKS][32];
float right_float_history[MAX_TRACKS][32];
float left_float_render;
float right_float_render;
float left_chorus;
float right_chorus;
float left_reverb;
float right_reverb;
float delay_left_final;
float delay_right_final;
#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
int PosInTick_Midi;
#endif
#endif
int PosInTick;

#if !defined(__STAND_ALONE__)
    char rawrender;
    char rawrender_32float;
    char rawrender_multi;
    char rawrender_target;
    int rawrender_range;
    int rawrender_from;
    int rawrender_to;
#endif

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    float mas_vol = 1.0f;
#else
    float mas_vol;
#endif

float local_mas_vol;
float local_curr_mas_vol;
volatile float local_curr_ramp_vol;
volatile float local_ramp_vol;

int Song_Playing_Pattern;

#if !defined(__WINAMP__)
#if !defined(__STAND_ALONE__)
    int done;
    int trigger_metronome;
    int metronome_latency;
    int metronome_rows_counter;
    int metronome_magnify;
    int metronome_internal_counter_int;

// RAW waveform used for the metronome
#define Metronome_Dats_Size 4688/2
unsigned char Metronome_Dats[] =
{
	0x40,0x00,0x30,0x00,0x70,0xfe,0x0e,0xfa,0x58,0xf3,0x42,0xec,0xc0,0xe7,0x80,0xe8,
	0x8c,0xef,0x6c,0xfb,0xc6,0x08,0xd2,0x13,0xc0,0x19,0xce,0x19,0x2a,0x15,0x2e,0x0e,
	0x38,0x07,0xdc,0x01,0xdc,0xfe,0x2a,0xfe,0x60,0xff,0x1e,0x02,0x42,0x06,0xce,0x0b,
	0xb8,0x12,0x50,0x1a,0xec,0x20,0x1e,0x24,0x6a,0x21,0x70,0x17,0x40,0x07,0x58,0xf4,
	0x98,0xe3,0x8a,0xd9,0x6a,0xd8,0x4e,0xdf,0xda,0xea,0xb0,0xf6,0x28,0xff,0x4a,0x02,
	0xfa,0xff,0x78,0xf9,0xb8,0xf0,0xf6,0xe7,0x5e,0xe1,0x6a,0xde,0x74,0xdf,0x7c,0xe3,
	0x66,0xe8,0x22,0xec,0xd4,0xed,0x4a,0xee,0x9a,0xef,0xe4,0xf3,0xcc,0xfb,0x12,0x06,
	0x3a,0x10,0xa4,0x17,0x10,0x1b,0x0a,0x1b,0x54,0x19,0xdc,0x17,0xba,0x17,0xf0,0x18,
	0x9c,0x1a,0xa2,0x1b,0x6a,0x1b,0x20,0x1a,0xaa,0x18,0x5a,0x18,0x40,0x1a,0x78,0x1e,
	0xc8,0x23,0xb0,0x27,0x0c,0x27,0x9e,0x1f,0x36,0x11,0x2c,0xfe,0xfa,0xea,0x54,0xdc,
	0x6c,0xd5,0xd2,0xd6,0x5a,0xde,0x3e,0xe8,0xbc,0xf0,0x4a,0xf5,0x08,0xf5,0x8e,0xf0,
	0x5c,0xe9,0x6c,0xe1,0xac,0xda,0x9a,0xd6,0xfc,0xd5,0x7c,0xd8,0xc2,0xdc,0x36,0xe1,
	0xc4,0xe4,0x78,0xe7,0x88,0xea,0x52,0xef,0xa6,0xf6,0xfc,0xff,0x7a,0x09,0x2e,0x11,
	0xf8,0x15,0xfa,0x17,0x82,0x18,0x22,0x19,0xd6,0x1a,0x94,0x1d,0x62,0x20,0x1a,0x22,
	0x20,0x22,0xa2,0x20,0xa4,0x1e,0x9a,0x1d,0xaa,0x1e,0x28,0x22,0x2a,0x27,0x7e,0x2b,
	0x74,0x2c,0xc6,0x27,0xa6,0x1c,0x80,0x0c,0x7c,0xfa,0x80,0xea,0xe4,0xdf,0x0e,0xdc,
	0x44,0xde,0x2a,0xe4,0xc6,0xea,0x90,0xef,0x1c,0xf1,0x10,0xef,0x14,0xea,0x70,0xe3,
	0xae,0xdc,0x54,0xd7,0x72,0xd4,0x42,0xd4,0x46,0xd6,0x80,0xd9,0x02,0xdd,0x9e,0xe0,
	0xd0,0xe4,0x7e,0xea,0x2a,0xf2,0x4a,0xfb,0x94,0x04,0x74,0x0c,0xbe,0x11,0x6e,0x14,
	0x8c,0x15,0x7a,0x16,0x58,0x18,0x5c,0x1b,0xd4,0x1e,0xc0,0x21,0x46,0x23,0x2a,0x23,
	0x1e,0x22,0x3a,0x21,0xa2,0x21,0xe6,0x23,0x7e,0x27,0xe0,0x2a,0xde,0x2b,0x6e,0x28,
	0xa2,0x1f,0x28,0x12,0x42,0x02,0x34,0xf3,0xe8,0xe7,0x18,0xe2,0xc2,0xe1,0x40,0xe5,
	0x22,0xea,0x1a,0xee,0x98,0xef,0x26,0xee,0x4c,0xea,0x04,0xe5,0x6e,0xdf,0x9c,0xda,
	0x46,0xd7,0x08,0xd6,0x52,0xd7,0x2c,0xdb,0x58,0xe1,0x22,0xe9,0x6a,0xf1,0xec,0xf8,
	0x7a,0xfe,0x88,0x01,0x46,0x02,0x7e,0x01,0x80,0x00,0xa6,0x00,0xd4,0x02,0x4c,0x07,
	0x8c,0x0d,0x62,0x14,0x76,0x1a,0x90,0x1e,0x04,0x20,0x0e,0x1f,0x8a,0x1c,0x96,0x19,
	0x46,0x17,0x0a,0x16,0xaa,0x15,0x94,0x15,0x16,0x15,0xce,0x13,0xba,0x11,0x12,0x0f,
	0x20,0x0c,0xf0,0x08,0x6e,0x05,0x9e,0x01,0x98,0xfd,0xae,0xf9,0x64,0xf6,0x06,0xf4,
	0xa0,0xf2,0x04,0xf2,0xaa,0xf1,0x12,0xf1,0xec,0xef,0x1e,0xee,0xec,0xeb,0xf0,0xe9,
	0xbe,0xe8,0xee,0xe8,0xc6,0xea,0x08,0xee,0x04,0xf2,0xc8,0xf5,0x7e,0xf8,0xb6,0xf9,
	0x8a,0xf9,0xb4,0xf8,0x20,0xf8,0xa0,0xf8,0xa0,0xfa,0x0c,0xfe,0x46,0x02,0x96,0x06,
	0x3e,0x0a,0xbc,0x0c,0x08,0x0e,0x68,0x0e,0x48,0x0e,0x2a,0x0e,0x56,0x0e,0xc4,0x0e,
	0x40,0x0f,0x7e,0x0f,0x4c,0x0f,0xa6,0x0e,0xb8,0x0d,0xb8,0x0c,0xbc,0x0b,0xae,0x0a,
	0x58,0x09,0x8e,0x07,0x4a,0x05,0xc8,0x02,0x58,0x00,0x4a,0xfe,0xd2,0xfc,0xd6,0xfb,
	0x0e,0xfb,0x32,0xfa,0xf8,0xf8,0x48,0xf7,0x52,0xf5,0x60,0xf3,0xe0,0xf1,0x36,0xf1,
	0x96,0xf1,0xf4,0xf2,0x06,0xf5,0x48,0xf7,0x2a,0xf9,0x46,0xfa,0x7a,0xfa,0xfc,0xf9,
	0x40,0xf9,0xdc,0xf8,0x48,0xf9,0xb2,0xfa,0x00,0xfd,0xca,0xff,0x86,0x02,0xca,0x04,
	0x62,0x06,0x48,0x07,0xbc,0x07,0x00,0x08,0x36,0x08,0x68,0x08,0x80,0x08,0x5c,0x08,
	0xfe,0x07,0x84,0x07,0x26,0x07,0x12,0x07,0x56,0x07,0xcc,0x07,0x32,0x08,0x38,0x08,
	0xae,0x07,0x8c,0x06,0xfa,0x04,0x46,0x03,0xb2,0x01,0x66,0x00,0x70,0xff,0xae,0xfe,
	0xf4,0xfd,0x2a,0xfd,0x42,0xfc,0x4a,0xfb,0x6c,0xfa,0xbc,0xf9,0x4e,0xf9,0x24,0xf9,
	0x2a,0xf9,0x3e,0xf9,0x44,0xf9,0x22,0xf9,0xd8,0xf8,0x7e,0xf8,0x42,0xf8,0x5e,0xf8,
	0xf4,0xf8,0x06,0xfa,0x6e,0xfb,0xe0,0xfc,0x1e,0xfe,0x06,0xff,0xa2,0xff,0x26,0x00,
	0xd6,0x00,0xd0,0x01,0x18,0x03,0x78,0x04,0xac,0x05,0x7c,0x06,0xd0,0x06,0xbe,0x06,
	0x7e,0x06,0x56,0x06,0x70,0x06,0xdc,0x06,0x7a,0x07,0x0c,0x08,0x5a,0x08,0x34,0x08,
	0x98,0x07,0xa0,0x06,0x78,0x05,0x50,0x04,0x3a,0x03,0x34,0x02,0x34,0x01,0x26,0x00,
	0x08,0xff,0xe2,0xfd,0xca,0xfc,0xd0,0xfb,0x06,0xfb,0x68,0xfa,0xea,0xf9,0x76,0xf9,
	0xfa,0xf8,0x70,0xf8,0xde,0xf7,0x62,0xf7,0x22,0xf7,0x44,0xf7,0xda,0xf7,0xda,0xf8,
	0x22,0xfa,0x76,0xfb,0xa2,0xfc,0x8a,0xfd,0x30,0xfe,0xbc,0xfe,0x60,0xff,0x4c,0x00,
	0x96,0x01,0x20,0x03,0xb0,0x04,0xf4,0x05,0xb0,0x06,0xd2,0x06,0x78,0x06,0xe4,0x05,
	0x64,0x05,0x2c,0x05,0x48,0x05,0x9c,0x05,0xf6,0x05,0x2a,0x06,0x1e,0x06,0xd2,0x05,
	0x5a,0x05,0xd0,0x04,0x40,0x04,0xae,0x03,0x0c,0x03,0x4a,0x02,0x60,0x01,0x4c,0x00,
	0x1e,0xff,0xea,0xfd,0xd0,0xfc,0xe4,0xfb,0x36,0xfb,0xc6,0xfa,0x86,0xfa,0x66,0xfa,
	0x52,0xfa,0x38,0xfa,0x16,0xfa,0xea,0xf9,0xc2,0xf9,0xb0,0xf9,0xc4,0xf9,0x02,0xfa,
	0x68,0xfa,0xf0,0xfa,0x92,0xfb,0x58,0xfc,0x50,0xfd,0x94,0xfe,0x20,0x00,0xde,0x01,
	0x94,0x03,0xf4,0x04,0xc2,0x05,0xe4,0x05,0x76,0x05,0xc0,0x04,0x1c,0x04,0xd0,0x03,
	0x04,0x04,0x9e,0x04,0x6a,0x05,0x18,0x06,0x6e,0x06,0x4c,0x06,0xba,0x05,0xdc,0x04,
	0xe0,0x03,0xf2,0x02,0x2a,0x02,0x84,0x01,0xee,0x00,0x56,0x00,0xae,0xff,0xec,0xfe,
	0x1a,0xfe,0x44,0xfd,0x76,0xfc,0xbe,0xfb,0x26,0xfb,0xb2,0xfa,0x64,0xfa,0x36,0xfa,
	0x1c,0xfa,0x0c,0xfa,0x02,0xfa,0x02,0xfa,0x1a,0xfa,0x52,0xfa,0xac,0xfa,0x22,0xfb,
	0xae,0xfb,0x52,0xfc,0x1e,0xfd,0x28,0xfe,0x82,0xff,0x20,0x01,0xe0,0x02,0x82,0x04,
	0xc8,0x05,0x80,0x06,0xac,0x06,0x68,0x06,0xf0,0x05,0x8a,0x05,0x60,0x05,0x80,0x05,
	0xd2,0x05,0x24,0x06,0x3c,0x06,0xf0,0x05,0x36,0x05,0x1e,0x04,0xd2,0x02,0x84,0x01,
	0x5c,0x00,0x6e,0xff,0xbc,0xfe,0x36,0xfe,0xce,0xfd,0x6a,0xfd,0x02,0xfd,0x88,0xfc,
	0xfc,0xfb,0x68,0xfb,0xda,0xfa,0x6c,0xfa,0x30,0xfa,0x2c,0xfa,0x5c,0xfa,0xb8,0xfa,
	0x28,0xfb,0xa4,0xfb,0x24,0xfc,0xa2,0xfc,0x1e,0xfd,0x90,0xfd,0xf8,0xfd,0x5c,0xfe,
	0xcc,0xfe,0x58,0xff,0x0c,0x00,0xee,0x00,0xe8,0x01,0xe4,0x02,0xc0,0x03,0x70,0x04,
	0xea,0x04,0x36,0x05,0x60,0x05,0x72,0x05,0x72,0x05,0x5e,0x05,0x32,0x05,0xe2,0x04,
	0x6a,0x04,0xc6,0x03,0x00,0x03,0x30,0x02,0x66,0x01,0xb8,0x00,0x2a,0x00,0xb6,0xff,
	0x48,0xff,0xd6,0xfe,0x52,0xfe,0xc4,0xfd,0x2a,0xfd,0x92,0xfc,0x04,0xfc,0x8c,0xfb,
	0x3a,0xfb,0x1c,0xfb,0x3a,0xfb,0x8e,0xfb,0x0c,0xfc,0x98,0xfc,0x1e,0xfd,0x8e,0xfd,
	0xe2,0xfd,0x24,0xfe,0x5c,0xfe,0x9a,0xfe,0xe8,0xfe,0x52,0xff,0xda,0xff,0x7a,0x00,
	0x20,0x01,0xb6,0x01,0x2a,0x02,0x70,0x02,0x92,0x02,0xa4,0x02,0xba,0x02,0xe8,0x02,
	0x2e,0x03,0x7e,0x03,0xc8,0x03,0xf8,0x03,0xfc,0x03,0xcc,0x03,0x64,0x03,0xc6,0x02,
	0x02,0x02,0x2e,0x01,0x60,0x00,0xac,0xff,0x18,0xff,0xa6,0xfe,0x56,0xfe,0x1c,0xfe,
	0xfa,0xfd,0xe0,0xfd,0xc0,0xfd,0x90,0xfd,0x4a,0xfd,0xf6,0xfc,0xa6,0xfc,0x76,0xfc,
	0x7a,0xfc,0xba,0xfc,0x24,0xfd,0xa0,0xfd,0x16,0xfe,0x76,0xfe,0xbc,0xfe,0xec,0xfe,
	0x1a,0xff,0x50,0xff,0x9e,0xff,0x06,0x00,0x8a,0x00,0x1a,0x01,0xa0,0x01,0xfe,0x01,
	0x26,0x02,0x12,0x02,0xd8,0x01,0x90,0x01,0x5c,0x01,0x58,0x01,0x86,0x01,0xe2,0x01,
	0x54,0x02,0xc0,0x02,0x08,0x03,0x10,0x03,0xce,0x02,0x44,0x02,0x8c,0x01,0xc4,0x00,
	0x10,0x00,0x8a,0xff,0x34,0xff,0x0c,0xff,0x08,0xff,0x18,0xff,0x2c,0xff,0x34,0xff,
	0x1e,0xff,0xe0,0xfe,0x74,0xfe,0xee,0xfd,0x68,0xfd,0x06,0xfd,0xe2,0xfc,0x02,0xfd,
	0x5a,0xfd,0xd0,0xfd,0x48,0xfe,0xae,0xfe,0xf6,0xfe,0x24,0xff,0x40,0xff,0x58,0xff,
	0x7c,0xff,0xb8,0xff,0x0e,0x00,0x7a,0x00,0xe8,0x00,0x42,0x01,0x76,0x01,0x7e,0x01,
	0x5c,0x01,0x26,0x01,0xea,0x00,0xba,0x00,0xa6,0x00,0xb2,0x00,0xda,0x00,0x16,0x01,
	0x52,0x01,0x7c,0x01,0x82,0x01,0x60,0x01,0x1c,0x01,0xc6,0x00,0x70,0x00,0x28,0x00,
	0xee,0xff,0xc4,0xff,0xa6,0xff,0x9c,0xff,0x9e,0xff,0xa6,0xff,0xa8,0xff,0x92,0xff,
	0x5a,0xff,0x0a,0xff,0xbc,0xfe,0x8c,0xfe,0x8e,0xfe,0xc4,0xfe,0x18,0xff,0x64,0xff,
	0x86,0xff,0x70,0xff,0x2a,0xff,0xce,0xfe,0x7c,0xfe,0x52,0xfe,0x68,0xfe,0xbc,0xfe,
	0x3c,0xff,0xd8,0xff,0x6c,0x00,0xe2,0x00,0x26,0x01,0x34,0x01,0x0e,0x01,0xc4,0x00,
	0x62,0x00,0x00,0x00,0xb2,0xff,0x88,0xff,0x92,0xff,0xd2,0xff,0x3a,0x00,0xb8,0x00,
	0x2e,0x01,0x7e,0x01,0x9e,0x01,0x8a,0x01,0x48,0x01,0xe6,0x00,0x74,0x00,0x02,0x00,
	0xa6,0xff,0x70,0xff,0x70,0xff,0xa0,0xff,0xec,0xff,0x36,0x00,0x66,0x00,0x6a,0x00,
	0x4a,0x00,0x14,0x00,0xe2,0xff,0xb8,0xff,0x92,0xff,0x64,0xff,0x22,0xff,0xca,0xfe,
	0x6a,0xfe,0x16,0xfe,0xe4,0xfd,0xe4,0xfd,0x12,0xfe,0x6c,0xfe,0xde,0xfe,0x54,0xff,
	0xba,0xff,0x00,0x00,0x1e,0x00,0x1c,0x00,0x04,0x00,0xe2,0xff,0xc2,0xff,0xb0,0xff,
	0xb6,0xff,0xdc,0xff,0x20,0x00,0x84,0x00,0xfc,0x00,0x74,0x01,0xd8,0x01,0x14,0x02,
	0x24,0x02,0x0c,0x02,0xd8,0x01,0x92,0x01,0x44,0x01,0xf6,0x00,0xac,0x00,0x70,0x00,
	0x48,0x00,0x3a,0x00,0x3e,0x00,0x48,0x00,0x46,0x00,0x2e,0x00,0x00,0x00,0xc4,0xff,
	0x86,0xff,0x4c,0xff,0x1c,0xff,0xec,0xfe,0xb8,0xfe,0x7c,0xfe,0x38,0xfe,0xf6,0xfd,
	0xbe,0xfd,0x9a,0xfd,0x92,0xfd,0xb0,0xfd,0xf6,0xfd,0x5e,0xfe,0xe4,0xfe,0x72,0xff,
	0xf8,0xff,0x62,0x00,0xa8,0x00,0xc8,0x00,0xca,0x00,0xb8,0x00,0xa4,0x00,0x98,0x00,
	0xa0,0x00,0xc0,0x00,0xf8,0x00,0x42,0x01,0x98,0x01,0xec,0x01,0x34,0x02,0x62,0x02,
	0x68,0x02,0x42,0x02,0xec,0x01,0x6a,0x01,0xca,0x00,0x1e,0x00,0x7c,0xff,0xf4,0xfe,
	0x98,0xfe,0x6e,0xfe,0x78,0xfe,0xa4,0xfe,0xe6,0xfe,0x28,0xff,0x5a,0xff,0x6c,0xff,
	0x5a,0xff,0x26,0xff,0xd8,0xfe,0x7c,0xfe,0x1c,0xfe,0xca,0xfd,0x90,0xfd,0x7c,0xfd,
	0x98,0xfd,0xe2,0xfd,0x5c,0xfe,0xfa,0xfe,0xaa,0xff,0x54,0x00,0xe4,0x00,0x48,0x01,
	0x76,0x01,0x76,0x01,0x54,0x01,0x22,0x01,0xf6,0x00,0xe2,0x00,0xec,0x00,0x1a,0x01,
	0x60,0x01,0xba,0x01,0x18,0x02,0x6e,0x02,0xae,0x02,0xcc,0x02,0xc0,0x02,0x88,0x02,
	0x24,0x02,0x9e,0x01,0xfe,0x00,0x54,0x00,0xb0,0xff,0x1e,0xff,0xa8,0xfe,0x56,0xfe,
	0x28,0xfe,0x18,0xfe,0x1c,0xfe,0x24,0xfe,0x24,0xfe,0x18,0xfe,0x02,0xfe,0xe2,0xfd,
	0xbe,0xfd,0x98,0xfd,0x70,0xfd,0x4c,0xfd,0x32,0xfd,0x2c,0xfd,0x48,0xfd,0x8c,0xfd,
	0xf8,0xfd,0x8a,0xfe,0x2e,0xff,0xde,0xff,0x82,0x00,0x14,0x01,0x86,0x01,0xd6,0x01,
	0x02,0x02,0x12,0x02,0x0e,0x02,0x06,0x02,0x00,0x02,0x04,0x02,0x1a,0x02,0x3e,0x02,
	0x72,0x02,0xb0,0x02,0xec,0x02,0x18,0x03,0x26,0x03,0x0a,0x03,0xbe,0x02,0x46,0x02,
	0xa8,0x01,0xf4,0x00,0x3a,0x00,0x8c,0xff,0xf2,0xfe,0x7a,0xfe,0x22,0xfe,0xea,0xfd,
	0xc6,0xfd,0xac,0xfd,0x94,0xfd,0x78,0xfd,0x5c,0xfd,0x40,0xfd,0x2a,0xfd,0x1a,0xfd,
	0x10,0xfd,0x0e,0xfd,0x12,0xfd,0x26,0xfd,0x50,0xfd,0x94,0xfd,0xf2,0xfd,0x66,0xfe,
	0xea,0xfe,0x7a,0xff,0x0a,0x00,0xa0,0x00,0x2e,0x01,0xae,0x01,0x14,0x02,0x5c,0x02,
	0x80,0x02,0x86,0x02,0x78,0x02,0x64,0x02,0x54,0x02,0x4e,0x02,0x56,0x02,0x68,0x02,
	0x80,0x02,0x94,0x02,0x96,0x02,0x80,0x02,0x4a,0x02,0xf0,0x01,0x7c,0x01,0xfa,0x00,
	0x72,0x00,0xf0,0xff,0x78,0xff,0x0e,0xff,0xb4,0xfe,0x70,0xfe,0x40,0xfe,0x20,0xfe,
	0x0a,0xfe,0xf2,0xfd,0xd4,0xfd,0xb0,0xfd,0x8c,0xfd,0x6e,0xfd,0x5c,0xfd,0x5c,0xfd,
	0x74,0xfd,0xa4,0xfd,0xea,0xfd,0x42,0xfe,0x9c,0xfe,0xec,0xfe,0x28,0xff,0x4e,0xff,
	0x70,0xff,0x9e,0xff,0xe4,0xff,0x46,0x00,0xb8,0x00,0x2a,0x01,0x8a,0x01,0xcc,0x01,
	0xf6,0x01,0x0e,0x02,0x1e,0x02,0x2c,0x02,0x36,0x02,0x3c,0x02,0x3e,0x02,0x38,0x02,
	0x2c,0x02,0x1c,0x02,0x0a,0x02,0xf4,0x01,0xd8,0x01,0xb8,0x01,0x8e,0x01,0x54,0x01,
	0x08,0x01,0xac,0x00,0x40,0x00,0xd2,0xff,0x64,0xff,0x02,0xff,0xb0,0xfe,0x70,0xfe,
	0x40,0xfe,0x1c,0xfe,0x04,0xfe,0xf2,0xfd,0xea,0xfd,0xe8,0xfd,0xee,0xfd,0xf4,0xfd,
	0xfe,0xfd,0x06,0xfe,0x0c,0xfe,0x12,0xfe,0x18,0xfe,0x2a,0xfe,0x4c,0xfe,0x8a,0xfe,
	0xe6,0xfe,0x60,0xff,0xea,0xff,0x74,0x00,0xea,0x00,0x42,0x01,0x72,0x01,0x82,0x01,
	0x80,0x01,0x76,0x01,0x74,0x01,0x82,0x01,0xa0,0x01,0xce,0x01,0x06,0x02,0x3c,0x02,
	0x66,0x02,0x78,0x02,0x6c,0x02,0x3e,0x02,0xf6,0x01,0x9e,0x01,0x3e,0x01,0xe2,0x00,
	0x8c,0x00,0x3e,0x00,0xfe,0xff,0xc8,0xff,0x9e,0xff,0x7a,0xff,0x56,0xff,0x2a,0xff,
	0xf2,0xfe,0xb0,0xfe,0x6c,0xfe,0x32,0xfe,0x08,0xfe,0xf4,0xfd,0xf4,0xfd,0x04,0xfe,
	0x1c,0xfe,0x36,0xfe,0x4e,0xfe,0x60,0xfe,0x70,0xfe,0x84,0xfe,0xa2,0xfe,0xd2,0xfe,
	0x18,0xff,0x6a,0xff,0xc6,0xff,0x1e,0x00,0x72,0x00,0xba,0x00,0xf8,0x00,0x2a,0x01,
	0x4e,0x01,0x64,0x01,0x72,0x01,0x82,0x01,0x98,0x01,0xbc,0x01,0xee,0x01,0x1e,0x02,
	0x40,0x02,0x46,0x02,0x2e,0x02,0xfa,0x01,0xb2,0x01,0x62,0x01,0x14,0x01,0xcc,0x00,
	0x90,0x00,0x5e,0x00,0x30,0x00,0x06,0x00,0xd8,0xff,0xa2,0xff,0x66,0xff,0x28,0xff,
	0xee,0xfe,0xba,0xfe,0x8e,0xfe,0x68,0xfe,0x44,0xfe,0x2a,0xfe,0x1c,0xfe,0x22,0xfe,
	0x38,0xfe,0x58,0xfe,0x7c,0xfe,0x98,0xfe,0xac,0xfe,0xbc,0xfe,0xd4,0xfe,0xf8,0xfe,
	0x2e,0xff,0x70,0xff,0xba,0xff,0x06,0x00,0x50,0x00,0x96,0x00,0xd2,0x00,0x02,0x01,
	0x26,0x01,0x40,0x01,0x5e,0x01,0x84,0x01,0xba,0x01,0xf4,0x01,0x24,0x02,0x3c,0x02,
	0x32,0x02,0x0a,0x02,0xca,0x01,0x86,0x01,0x48,0x01,0x0e,0x01,0xde,0x00,0xae,0x00,
	0x7a,0x00,0x40,0x00,0x08,0x00,0xd2,0xff,0xa2,0xff,0x7c,0xff,0x5e,0xff,0x42,0xff,
	0x20,0xff,0xee,0xfe,0xae,0xfe,0x6a,0xfe,0x30,0xfe,0x12,0xfe,0x16,0xfe,0x3a,0xfe,
	0x6e,0xfe,0xa2,0xfe,0xd2,0xfe,0xf6,0xfe,0x1a,0xff,0x40,0xff,0x6a,0xff,0x96,0xff,
	0xc0,0xff,0xe2,0xff,0xfe,0xff,0x16,0x00,0x34,0x00,0x58,0x00,0x84,0x00,0xc0,0x00,
	0x06,0x01,0x58,0x01,0xa6,0x01,0xe6,0x01,0x08,0x02,0x00,0x02,0xd6,0x01,0x96,0x01,
	0x58,0x01,0x28,0x01,0x0e,0x01,0x04,0x01,0x02,0x01,0xfe,0x00,0xf0,0x00,0xd8,0x00,
	0xb4,0x00,0x86,0x00,0x50,0x00,0x12,0x00,0xd0,0xff,0x84,0xff,0x38,0xff,0xf0,0xfe,
	0xb6,0xfe,0x96,0xfe,0x98,0xfe,0xb4,0xfe,0xde,0xfe,0x04,0xff,0x16,0xff,0x0e,0xff,
	0xf6,0xfe,0xdc,0xfe,0xd2,0xfe,0xde,0xfe,0xf8,0xfe,0x1a,0xff,0x3a,0xff,0x54,0xff,
	0x6e,0xff,0x8c,0xff,0xae,0xff,0xd6,0xff,0xfe,0xff,0x28,0x00,0x58,0x00,0x92,0x00,
	0xd6,0x00,0x1a,0x01,0x50,0x01,0x6a,0x01,0x62,0x01,0x3c,0x01,0x06,0x01,0xd4,0x00,
	0xb8,0x00,0xba,0x00,0xd6,0x00,0x04,0x01,0x32,0x01,0x56,0x01,0x64,0x01,0x54,0x01,
	0x28,0x01,0xe2,0x00,0x8a,0x00,0x26,0x00,0xc2,0xff,0x64,0xff,0x1c,0xff,0xf8,0xfe,
	0xfc,0xfe,0x28,0xff,0x6c,0xff,0xb2,0xff,0xdc,0xff,0xdc,0xff,0xaa,0xff,0x54,0xff,
	0xf4,0xfe,0xa0,0xfe,0x6e,0xfe,0x6a,0xfe,0x8c,0xfe,0xcc,0xfe,0x1c,0xff,0x6e,0xff,
	0xb2,0xff,0xe6,0xff,0x02,0x00,0x12,0x00,0x1a,0x00,0x24,0x00,0x36,0x00,0x4e,0x00,
	0x68,0x00,0x80,0x00,0x8e,0x00,0x96,0x00,0xa2,0x00,0xb6,0x00,0xd4,0x00,0xf8,0x00,
	0x18,0x01,0x2c,0x01,0x30,0x01,0x26,0x01,0x12,0x01,0xf8,0x00,0xde,0x00,0xc0,0x00,
	0x9a,0x00,0x6c,0x00,0x34,0x00,0xfa,0xff,0xc2,0xff,0x94,0xff,0x7a,0xff,0x76,0xff,
	0x86,0xff,0x9c,0xff,0xaa,0xff,0xa2,0xff,0x80,0xff,0x46,0xff,0x02,0xff,0xca,0xfe,
	0xac,0xfe,0xb0,0xfe,0xd4,0xfe,0x0e,0xff,0x52,0xff,0x90,0xff,0xbe,0xff,0xd8,0xff,
	0xe2,0xff,0xe8,0xff,0xf4,0xff,0x12,0x00,0x48,0x00,0x8c,0x00,0xd0,0x00,0x02,0x01,
	0x18,0x01,0x12,0x01,0xf6,0x00,0xd4,0x00,0xb6,0x00,0xa8,0x00,0xa8,0x00,0xb4,0x00,
	0xc6,0x00,0xd6,0x00,0xdc,0x00,0xd4,0x00,0xb8,0x00,0x84,0x00,0x40,0x00,0xf4,0xff,
	0xa4,0xff,0x5a,0xff,0x1a,0xff,0xee,0xfe,0xd8,0xfe,0xe2,0xfe,0x08,0xff,0x42,0xff,
	0x84,0xff,0xb4,0xff,0xc2,0xff,0xa6,0xff,0x6c,0xff,0x26,0xff,0xec,0xfe,0xd4,0xfe,
	0xe4,0xfe,0x1e,0xff,0x76,0xff,0xda,0xff,0x36,0x00,0x78,0x00,0x98,0x00,0x92,0x00,
	0x72,0x00,0x4a,0x00,0x32,0x00,0x38,0x00,0x5c,0x00,0x94,0x00,0xd0,0x00,0xfc,0x00,
	0x10,0x01,0x0e,0x01,0xfa,0x00,0xda,0x00,0xb6,0x00,0x94,0x00,0x74,0x00,0x58,0x00,
	0x3e,0x00,0x24,0x00,0x06,0x00,0xe8,0xff,0xc6,0xff,0xaa,0xff,0x98,0xff,0x8c,0xff,
	0x7e,0xff,0x66,0xff,0x42,0xff,0x1e,0xff,0x08,0xff,0x0c,0xff,0x30,0xff,0x62,0xff,
	0x90,0xff,0xa8,0xff,0xa4,0xff,0x8e,0xff,0x7a,0xff,0x76,0xff,0x8e,0xff,0xc0,0xff,
	0x00,0x00,0x44,0x00,0x84,0x00,0xb4,0x00,0xca,0x00,0xc4,0x00,0xa8,0x00,0x80,0x00,
	0x60,0x00,0x56,0x00,0x60,0x00,0x7c,0x00,0x9a,0x00,0xae,0x00,0xb0,0x00,0xa6,0x00,
	0x96,0x00,0x84,0x00,0x6c,0x00,0x4e,0x00,0x26,0x00,0xfa,0xff,0xcc,0xff,0xa4,0xff,
	0x84,0xff,0x6c,0xff,0x5c,0xff,0x54,0xff,0x58,0xff,0x60,0xff,0x68,0xff,0x6a,0xff,
	0x64,0xff,0x5e,0xff,0x5e,0xff,0x6a,0xff,0x7e,0xff,0x94,0xff,0xa4,0xff,0xa4,0xff,
	0x98,0xff,0x8e,0xff,0x8e,0xff,0xa0,0xff,0xc6,0xff,0xfa,0xff,0x34,0x00,0x6c,0x00,
	0x9c,0x00,0xb8,0x00,0xc0,0x00,0xae,0x00,0x8a,0x00,0x5e,0x00,0x3c,0x00,0x2e,0x00,
	0x3a,0x00,0x5a,0x00,0x82,0x00,0xa4,0x00,0xbc,0x00,0xc6,0x00,0xc2,0x00,0xb2,0x00,
	0x94,0x00,0x6c,0x00,0x38,0x00,0x02,0x00,0xd0,0xff,0xa6,0xff,0x88,0xff,0x76,0xff,
	0x72,0xff,0x7a,0xff,0x84,0xff,0x8c,0xff,0x8e,0xff,0x84,0xff,0x72,0xff,0x62,0xff,
	0x5c,0xff,0x66,0xff,0x80,0xff,0xa0,0xff,0xb8,0xff,0xc4,0xff,0xc6,0xff,0xc2,0xff,
	0xc2,0xff,0xca,0xff,0xda,0xff,0xea,0xff,0xf8,0xff,0x04,0x00,0x14,0x00,0x28,0x00,
	0x40,0x00,0x50,0x00,0x56,0x00,0x4a,0x00,0x30,0x00,0x12,0x00,0xf8,0xff,0xe8,0xff,
	0xe8,0xff,0xf8,0xff,0x14,0x00,0x3c,0x00,0x6e,0x00,0x9e,0x00,0xc4,0x00,0xd4,0x00,
	0xca,0x00,0xa8,0x00,0x74,0x00,0x3a,0x00,0x06,0x00,0xe0,0xff,0xca,0xff,0xc8,0xff,
	0xd8,0xff,0xf2,0xff,0x0a,0x00,0x1c,0x00,0x20,0x00,0x14,0x00,0xfe,0xff,0xde,0xff,
	0xba,0xff,0x98,0xff,0x76,0xff,0x5a,0xff,0x4a,0xff,0x4e,0xff,0x68,0xff,0x90,0xff,
	0xc0,0xff,0xe4,0xff,0xf6,0xff,0xf2,0xff,0xe0,0xff,0xca,0xff,0xbc,0xff,0xbc,0xff,
	0xca,0xff,0xe4,0xff,0xfe,0xff,0x16,0x00,0x28,0x00,0x2e,0x00,0x26,0x00,0x14,0x00,
	0xfe,0xff,0xee,0xff,0xee,0xff,0x00,0x00,0x24,0x00,0x56,0x00,0x88,0x00,0xb2,0x00,
	0xca,0x00,0xcc,0x00,0xb4,0x00,0x82,0x00,0x40,0x00,0xfc,0xff,0xbe,0xff,0x94,0xff,
	0x86,0xff,0x96,0xff,0xb8,0xff,0xe6,0xff,0x0e,0x00,0x2a,0x00,0x30,0x00,0x16,0x00,
	0xe4,0xff,0x9a,0xff,0x4e,0xff,0x0e,0xff,0xee,0xfe,0xf0,0xfe,0x0e,0xff,0x42,0xff,
	0x7c,0xff,0xb0,0xff,0xd8,0xff,0xf4,0xff,0x02,0x00,0x06,0x00,0x02,0x00,0xfa,0xff,
	0xf4,0xff,0xf2,0xff,0xfa,0xff,0x0a,0x00,0x20,0x00,0x3e,0x00,0x5a,0x00,0x74,0x00,
	0x8c,0x00,0x9a,0x00,0xa4,0x00,0xa8,0x00,0xa8,0x00,0xaa,0x00,0xaa,0x00,0xa6,0x00,
	0x9a,0x00,0x80,0x00,0x58,0x00,0x26,0x00,0xf2,0xff,0xc6,0xff,0xaa,0xff,0xa2,0xff,
	0xa8,0xff,0xb8,0xff,0xca,0xff,0xd8,0xff,0xda,0xff,0xce,0xff,0xb4,0xff,0x8e,0xff,
	0x64,0xff,0x3e,0xff,0x26,0xff,0x20,0xff,0x2e,0xff,0x4c,0xff,0x70,0xff,0x96,0xff,
	0xb8,0xff,0xd4,0xff,0xe8,0xff,0xf4,0xff,0xfc,0xff,0xfc,0xff,0xfa,0xff,0xfa,0xff,
	0x00,0x00,0x12,0x00,0x2a,0x00,0x48,0x00,0x64,0x00,0x7a,0x00,0x86,0x00,0x8e,0x00,
	0x90,0x00,0x92,0x00,0x96,0x00,0xa0,0x00,0xb0,0x00,0xbe,0x00,0xc6,0x00,0xc2,0x00,
	0xaa,0x00,0x80,0x00,0x4a,0x00,0x12,0x00,0xe2,0xff,0xc2,0xff,0xb2,0xff,0xb0,0xff,
	0xb4,0xff,0xb8,0xff,0xb8,0xff,0xac,0xff,0x94,0xff,0x70,0xff,0x46,0xff,0x1e,0xff,
	0x00,0xff,0xf4,0xfe,0xfc,0xfe,0x12,0xff,0x34,0xff,0x5a,0xff,0x7e,0xff,0xa2,0xff,
	0xc2,0xff,0xdc,0xff,0xf0,0xff,0xfa,0xff,0xfe,0xff,0x00,0x00,0x0a,0x00,0x20,0x00,
	0x48,0x00,0x7a,0x00,0xae,0x00,0xd8,0x00,0xf2,0x00,0xf4,0x00,0xe2,0x00,0xc0,0x00,
	0x9c,0x00,0x82,0x00,0x76,0x00,0x7c,0x00,0x86,0x00,0x90,0x00,0x8e,0x00,0x7c,0x00,
	0x5c,0x00,0x32,0x00,0x08,0x00,0xe2,0xff,0xc0,0xff,0xa4,0xff,0x8e,0xff,0x7a,0xff,
	0x68,0xff,0x54,0xff,0x3e,0xff,0x26,0xff,0x12,0xff,0x04,0xff,0x04,0xff,0x0e,0xff,
	0x24,0xff,0x40,0xff,0x60,0xff,0x80,0xff,0xa2,0xff,0xc0,0xff,0xdc,0xff,0xec,0xff,
	0xf4,0xff,0xee,0xff,0xe8,0xff,0xe8,0xff,0xf8,0xff,0x1c,0x00,0x58,0x00,0xa2,0x00,
	0xf2,0x00,0x3c,0x01,0x6e,0x01,0x80,0x01,0x6a,0x01,0x34,0x01,0xea,0x00,0x9e,0x00,
	0x62,0x00,0x40,0x00,0x3a,0x00,0x48,0x00,0x5c,0x00,0x70,0x00,0x76,0x00,0x70,0x00,
	0x58,0x00,0x2e,0x00,0xf4,0xff,0xaa,0xff,0x5c,0xff,0x12,0xff,0xd4,0xfe,0xa8,0xfe,
	0x96,0xfe,0x9c,0xfe,0xba,0xfe,0xea,0xfe,0x20,0xff,0x58,0xff,0x80,0xff,0x9a,0xff,
	0xa2,0xff,0xa4,0xff,0xa4,0xff,0xae,0xff,0xc0,0xff,0xda,0xff,0xf2,0xff,0x04,0x00,
	0x12,0x00,0x24,0x00,0x3c,0x00,0x60,0x00,0x94,0x00,0xd0,0x00,0x0e,0x01,0x40,0x01,
	0x5e,0x01,0x60,0x01,0x44,0x01,0x0e,0x01,0xc8,0x00,0x7e,0x00,0x40,0x00,0x14,0x00,
	0xfe,0xff,0xfe,0xff,0x0c,0x00,0x26,0x00,0x40,0x00,0x52,0x00,0x54,0x00,0x3c,0x00,
	0x0a,0x00,0xc2,0xff,0x68,0xff,0x0e,0xff,0xc0,0xfe,0x8a,0xfe,0x72,0xfe,0x7e,0xfe,
	0xac,0xfe,0xf2,0xfe,0x46,0xff,0x98,0xff,0xda,0xff,0x00,0x00,0x06,0x00,0xf6,0xff,
	0xdc,0xff,0xc8,0xff,0xc6,0xff,0xd4,0xff,0xee,0xff,0x0c,0x00,0x2a,0x00,0x46,0x00,
	0x64,0x00,0x86,0x00,0xae,0x00,0xd0,0x00,0xea,0x00,0xf6,0x00,0xf2,0x00,0xe0,0x00,
	0xc0,0x00,0x98,0x00,0x6a,0x00,0x3a,0x00,0x0a,0x00,0xe4,0xff,0xc6,0xff,0xb6,0xff,
	0xb4,0xff,0xc0,0xff,0xd2,0xff,0xec,0xff,0x02,0x00,0x10,0x00,0x0a,0x00,0xee,0xff,
	0xb6,0xff,0x72,0xff,0x30,0xff,0xfc,0xfe,0xe4,0xfe,0xec,0xfe,0x12,0xff,0x50,0xff,
	0x9e,0xff,0xee,0xff,0x34,0x00,0x64,0x00,0x7a,0x00,0x74,0x00,0x5c,0x00,0x40,0x00,
	0x28,0x00,0x1c,0x00,0x1c,0x00,0x24,0x00,0x34,0x00,0x48,0x00,0x5e,0x00,0x76,0x00,
	0x8a,0x00,0x96,0x00,0x96,0x00,0x88,0x00,0x72,0x00,0x56,0x00,0x3a,0x00,0x1e,0x00,
	0x02,0x00,0xe4,0xff,0xc4,0xff,0xa4,0xff,0x8c,0xff,0x80,0xff,0x80,0xff,0x90,0xff,
	0xa8,0xff,0xc6,0xff,0xe2,0xff,0xf6,0xff,0xfe,0xff,0xf6,0xff,0xe0,0xff,0xc0,0xff,
	0xa0,0xff,0x86,0xff,0x7e,0xff,0x86,0xff,0xa4,0xff,0xd2,0xff,0x0c,0x00,0x4a,0x00,
	0x84,0x00,0xae,0x00,0xbe,0x00,0xb2,0x00,0x92,0x00,0x6a,0x00,0x42,0x00,0x2a,0x00,
	0x20,0x00,0x24,0x00,0x2c,0x00,0x34,0x00,0x34,0x00,0x2e,0x00,0x24,0x00,0x18,0x00,
	0x0a,0x00,0xfe,0xff,0xf0,0xff,0xe2,0xff,0xd6,0xff,0xcc,0xff,0xc2,0xff,0xb6,0xff,
	0xa6,0xff,0x92,0xff,0x7c,0xff,0x6a,0xff,0x62,0xff,0x6c,0xff,0x8a,0xff,0xb4,0xff,
	0xe6,0xff,0x12,0x00,0x36,0x00,0x46,0x00,0x40,0x00,0x2e,0x00,0x14,0x00,0xfe,0xff,
	0xee,0xff,0xea,0xff,0xf6,0xff,0x0e,0x00,0x34,0x00,0x60,0x00,0x90,0x00,0xba,0x00,
	0xd4,0x00,0xd8,0x00,0xc4,0x00,0x9e,0x00,0x6a,0x00,0x34,0x00,0x08,0x00,0xec,0xff,
	0xe0,0xff,0xdc,0xff,0xe0,0xff,0xe2,0xff,0xe0,0xff,0xd6,0xff,0xc4,0xff,0xac,0xff,
	0x90,0xff,0x76,0xff,0x62,0xff,0x58,0xff,0x5c,0xff,0x6a,0xff,0x7c,0xff,0x8a,0xff,
	0x92,0xff,0x94,0xff,0x94,0xff,0xa0,0xff,0xb8,0xff,0xe2,0xff,0x12,0x00,0x48,0x00,
	0x72,0x00,0x8e,0x00,0x98,0x00,0x94,0x00,0x8c,0x00,0x82,0x00,0x7c,0x00,0x7c,0x00,
	0x80,0x00,0x88,0x00,0x92,0x00,0x9a,0x00,0x9e,0x00,0x9e,0x00,0x98,0x00,0x8e,0x00,
	0x7c,0x00,0x62,0x00,0x40,0x00,0x1c,0x00,0xf8,0xff,0xd4,0xff,0xb6,0xff,0xa0,0xff,
	0x8e,0xff,0x82,0xff,0x7a,0xff,0x76,0xff,0x74,0xff,0x72,0xff,0x6e,0xff,0x68,0xff,
	0x60,0xff,0x5c,0xff,0x60,0xff,0x6e,0xff,0x82,0xff,0x9c,0xff,0xb2,0xff,0xc4,0xff,
	0xd6,0xff,0xe8,0xff,0x00,0x00,0x20,0x00,0x44,0x00,0x66,0x00,0x80,0x00,0x8e,0x00,
	0x8e,0x00,0x84,0x00,0x74,0x00,0x68,0x00,0x60,0x00,0x64,0x00,0x76,0x00,0x8c,0x00,
	0xa2,0x00,0xb0,0x00,0xae,0x00,0x98,0x00,0x74,0x00,0x46,0x00,0x18,0x00,0xf2,0xff,
	0xd6,0xff,0xc6,0xff,0xc2,0xff,0xc6,0xff,0xca,0xff,0xca,0xff,0xc4,0xff,0xb4,0xff,
	0x9e,0xff,0x86,0xff,0x70,0xff,0x5e,0xff,0x50,0xff,0x4a,0xff,0x4a,0xff,0x56,0xff,
	0x6c,0xff,0x8e,0xff,0xb6,0xff,0xdc,0xff,0xfa,0xff,0x08,0x00,0x0c,0x00,0x0a,0x00,
	0x0a,0x00,0x16,0x00,0x2e,0x00,0x52,0x00,0x7a,0x00,0x9e,0x00,0xb4,0x00,0xba,0x00,
	0xb0,0x00,0x9a,0x00,0x82,0x00,0x72,0x00,0x6e,0x00,0x78,0x00,0x8c,0x00,0x9c,0x00,
	0xa2,0x00,0x96,0x00,0x7a,0x00,0x4c,0x00,0x1c,0x00,0xf0,0xff,0xcc,0xff,0xb6,0xff,
	0xae,0xff,0xac,0xff,0xb0,0xff,0xb2,0xff,0xb0,0xff,0xa4,0xff,0x94,0xff,0x7e,0xff,
	0x68,0xff,0x54,0xff,0x48,0xff,0x3e,0xff,0x3e,0xff,0x46,0xff,0x5c,0xff,0x80,0xff,
	0xb0,0xff,0xe6,0xff,0x14,0x00,0x36,0x00,0x44,0x00,0x42,0x00,0x36,0x00,0x2a,0x00,
	0x28,0x00,0x30,0x00,0x46,0x00,0x60,0x00,0x7e,0x00,0x96,0x00,0xa4,0x00,0xa8,0x00,
	0xa2,0x00,0x96,0x00,0x88,0x00,0x7e,0x00,0x74,0x00,0x6c,0x00,0x60,0x00,0x4e,0x00,
	0x36,0x00,0x18,0x00,0xfe,0xff,0xe6,0xff,0xd4,0xff,0xca,0xff,0xc4,0xff,0xc4,0xff,
	0xc2,0xff,0xc2,0xff,0xbe,0xff,0xb4,0xff,0xa4,0xff,0x92,0xff,0x80,0xff,0x72,0xff,
	0x70,0xff,0x7a,0xff,0x8a,0xff,0xa0,0xff,0xb8,0xff,0xd0,0xff,0xe6,0xff,0xfc,0xff,
	0x0e,0x00,0x20,0x00,0x30,0x00,0x3c,0x00,0x42,0x00,0x44,0x00,0x46,0x00,0x4a,0x00,
	0x52,0x00,0x5e,0x00,0x6a,0x00,0x70,0x00,0x6e,0x00,0x62,0x00,0x52,0x00,0x40,0x00,
	0x36,0x00,0x34,0x00,0x3a,0x00,0x46,0x00,0x4e,0x00,0x4e,0x00,0x42,0x00,0x2a,0x00,
	0x0a,0x00,0xec,0xff,0xd0,0xff,0xbc,0xff,0xb8,0xff,0xc0,0xff,0xd4,0xff,0xe8,0xff,
	0xf6,0xff,0xfa,0xff,0xf2,0xff,0xde,0xff,0xc6,0xff,0xaa,0xff,0x96,0xff,0x86,0xff,
	0x84,0xff,0x8e,0xff,0xa6,0xff,0xc8,0xff,0xec,0xff,0x0c,0x00,0x22,0x00,0x2c,0x00,
	0x2a,0x00,0x1e,0x00,0x12,0x00,0x06,0x00,0x00,0x00,0x04,0x00,0x14,0x00,0x30,0x00,
	0x56,0x00,0x7a,0x00,0x92,0x00,0x96,0x00,0x86,0x00,0x6a,0x00,0x4a,0x00,0x2e,0x00,
	0x1e,0x00,0x1a,0x00,0x1a,0x00,0x1e,0x00,0x20,0x00,0x22,0x00,0x1e,0x00,0x16,0x00,
	0x04,0x00,0xee,0xff,0xd4,0xff,0xbc,0xff,0xac,0xff,0xa6,0xff,0xa6,0xff,0xa8,0xff,
	0xa6,0xff,0xa0,0xff,0x98,0xff,0x90,0xff,0x8e,0xff,0x94,0xff,0xa2,0xff,0xba,0xff,
	0xd8,0xff,0xf6,0xff,0x10,0x00,0x22,0x00,0x28,0x00,0x24,0x00,0x1c,0x00,0x16,0x00,
	0x14,0x00,0x18,0x00,0x1e,0x00,0x26,0x00,0x30,0x00,0x42,0x00,0x5a,0x00,0x76,0x00,
	0x8e,0x00,0x94,0x00,0x84,0x00,0x62,0x00,0x36,0x00,0x0a,0x00,0xf2,0xff,0xe8,0xff,
	0xf0,0xff,0x00,0x00,0x14,0x00,0x20,0x00,0x26,0x00,0x1e,0x00,0x0c,0x00,0xf8,0xff,
	0xe2,0xff,0xd0,0xff,0xc6,0xff,0xc4,0xff,0xc4,0xff,0xc4,0xff,0xc0,0xff,0xb6,0xff,
	0xa8,0xff,0x9a,0xff,0x8e,0xff,0x88,0xff,0x8c,0xff,0x98,0xff,0xb2,0xff,0xd2,0xff,
	0xf0,0xff,0x06,0x00,0x12,0x00,0x10,0x00,0x06,0x00,0xfc,0xff,0xf2,0xff,0xee,0xff,
	0xf2,0xff,0xfa,0xff,0x0a,0x00,0x24,0x00,0x48,0x00,0x6e,0x00,0x90,0x00,0xa2,0x00,
	0x9e,0x00,0x82,0x00,0x5a,0x00,0x2e,0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x10,0x00,
	0x26,0x00,0x38,0x00,0x40,0x00,0x3c,0x00,0x2c,0x00,0x16,0x00,0xfc,0xff,0xe4,0xff,
};
#endif

#else
    extern int done;
#endif

int Subicounter;

#if defined(PTK_MP3)
char Mp3_BitRate[MAX_INSTRS];
int Type_Mp3_BitRate[] =
{
    64, 88, 96, 128, 160, 192
};
#endif

#if defined(PTK_AT3)
char At3_BitRate[MAX_INSTRS];
int Type_At3_BitRate[] =
{
    66, 105, 132
};
#endif

#if defined(PTK_FX_PATTERNBREAK) || defined(PTK_FX_POSJUMP)
// 255 when no jump or yes on patbreak < 128 = line to jump.
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    int Patbreak_Line = 255;
#else
    int Patbreak_Line;
#endif
#endif

#if defined(PTK_FX_POSJUMP)
int PosJump = -1;
#endif

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    char Song_Tracks = 6;
#else
    char Song_Tracks;
#endif

char Channels_Polyphony[MAX_TRACKS];
char Channels_MultiNotes[MAX_TRACKS];
char Channels_Effects[MAX_TRACKS];

unsigned char pSequence[256];

int Pattern_Line;
int Song_Position;
int Song_Position_Visual;
int Pattern_Line_Visual;

int pl_note[MAX_POLYPHONY];
int pl_sample[MAX_POLYPHONY];
int old_pl_sample[MAX_TRACKS][MAX_POLYPHONY];
int pl_vol_row;
int pl_pan_row;
unsigned char *RawPatterns;

int pl_eff_row[MAX_FX];
int pl_dat_row[MAX_FX];
int glide[MAX_TRACKS];
float Sample_Vol[MAX_INSTRS];
unsigned int SubCounter;
unsigned int SamplesPerSub;
int shuffle_amount;

#if defined(PTK_SHUFFLE)
int shufflestep;
int shuffleswitch;
#endif

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    unsigned char Song_Length = 1;
#else
    unsigned char Song_Length;
#endif

#if defined(PTK_FX_REVERSE)
    int Reverse_Switch[MAX_TRACKS];
    int Reserve_Dat[MAX_TRACKS];
#endif

#if defined(PTK_FX_VIBRATO)
    float Vstep_vib[MAX_TRACKS][MAX_POLYPHONY];
    int Vibrato_Switch[MAX_TRACKS];
    float Vibrato_BaseNote[MAX_TRACKS][MAX_POLYPHONY];
    int Vibcounter[MAX_TRACKS];
#endif

#if defined(PTK_FX_ARPEGGIO)
    float Vstep_arp[MAX_TRACKS][MAX_POLYPHONY];
    int Arpeggio_Switch[MAX_TRACKS];
    float Arpeggio_BaseNote[MAX_TRACKS][MAX_POLYPHONY];
#endif

#if defined(PTK_FX_PATTERNLOOP)
int repeat_loop_pos;
int repeat_loop_counter;
int repeat_loop_counter_in;
#endif

short patternLines[MAX_ROWS];
char grown;
float Curr_Signal_L[MAX_POLYPHONY];
float Curr_Signal_R[MAX_POLYPHONY];
float All_Signal_L;
float All_Signal_R;
unsigned int Current_Pointer;

#if defined(PTK_SYNTH)
char Synth_Was[MAX_TRACKS][MAX_POLYPHONY];
#endif

short *Player_WL[MAX_TRACKS][MAX_POLYPHONY];
short *Player_WR[MAX_TRACKS][MAX_POLYPHONY];
float Player_Ampli[MAX_TRACKS][MAX_POLYPHONY];
char Player_SC[MAX_TRACKS][MAX_POLYPHONY];
char Player_LT[MAX_TRACKS][MAX_POLYPHONY];
char Player_LW[MAX_TRACKS][MAX_POLYPHONY];
unsigned int Player_LS[MAX_TRACKS][MAX_POLYPHONY];
unsigned int Player_LE[MAX_TRACKS][MAX_POLYPHONY];
unsigned int Player_LL[MAX_TRACKS][MAX_POLYPHONY];
unsigned int Player_NS[MAX_TRACKS][MAX_POLYPHONY];

#if defined(PTK_LIMITER_MASTER) || defined(PTK_LIMITER_TRACKS)
#define MAS_COMPRESSOR_SECONDS 0.1f
#define MAS_COMPRESSOR_SIZE (int) (MAS_COMPRESSOR_SECONDS * MIX_RATE)
    int mas_comp_pos_rms_buffer;
#endif

#if defined(PTK_LIMITER_TRACKS)
    float mas_comp_bufferL_Track[MAX_TRACKS][MAS_COMPRESSOR_SIZE];
    float mas_comp_bufferR_Track[MAX_TRACKS][MAS_COMPRESSOR_SIZE];
    char Compress_Track[MAX_TRACKS];
    float rms_sumL_Track[MAX_TRACKS];
    float rms_sumR_Track[MAX_TRACKS];
    float mas_envL_Track[MAX_TRACKS];
    float mas_envR_Track[MAX_TRACKS];
    float mas_comp_ratio_Track[MAX_TRACKS];
    float mas_comp_threshold_Track[MAX_TRACKS];
    float mas_threshold_Track[MAX_TRACKS];
    float mas_ratio_Track[MAX_TRACKS];
#endif

    float Track_Volume[MAX_TRACKS];
    char Track_Surround[MAX_TRACKS];
    char Track_Denoise[MAX_TRACKS];

#if defined(PTK_LIMITER_MASTER)
    float mas_comp_bufferL_Master[MAS_COMPRESSOR_SIZE];
    float mas_comp_bufferR_Master[MAS_COMPRESSOR_SIZE];
    char Compress_Master;
    float rms_sumL_Master;
    float rms_sumR_Master;
    float mas_envL_Master;
    float mas_envR_Master;
    float mas_comp_ratio_Master;
    float mas_comp_threshold_Master;
    float mas_threshold_Master;
    float mas_ratio_Master;
#endif

char Chan_Active_State[256][MAX_TRACKS];

#if !defined(__STAND_ALONE__)
    char Chan_History_State[256][MAX_TRACKS];
#endif

int gco;
int ChorCut;
float ramper[MAX_TRACKS];
char Basenote[MAX_INSTRS][16];
char SampleType[MAX_INSTRS][16];
char Finetune[MAX_INSTRS][16];

#if !defined(__STAND_ALONE__) && !(__WINAMP__)
char SamplesSwap[MAX_INSTRS];
short *RawSamples_Swap[MAX_INSTRS][2][16];
#endif

unsigned char Synth_Prg[128];

#if defined(PTK_SYNTH)
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
Synth_Parameters PARASynth[128];
#else
SYNTH_DATA PARASynth[128];
#endif
#endif

char LoopType[MAX_INSTRS][MAX_INSTRS_SPLITS];
Uint32 LoopStart[MAX_INSTRS][MAX_INSTRS_SPLITS];
Uint32 LoopEnd[MAX_INSTRS][MAX_INSTRS_SPLITS];
Uint32 Sample_Length[MAX_INSTRS][MAX_INSTRS_SPLITS];
Uint32 Sample_Length_Packed[MAX_INSTRS][MAX_INSTRS_SPLITS];
char Beat_Sync[MAX_INSTRS];
short Beat_Lines[MAX_INSTRS];
float sp_Step[MAX_TRACKS][MAX_POLYPHONY];
float Sample_Amplify[MAX_INSTRS][MAX_INSTRS_SPLITS];
char Sample_Channels[MAX_INSTRS][MAX_INSTRS_SPLITS];
float FDecay[MAX_INSTRS][MAX_INSTRS_SPLITS];
short *RawSamples[MAX_INSTRS][2][MAX_INSTRS_SPLITS];

#if defined(PTK_COMPRESSOR)
    int currentCounter;
    int delayedCounterL[10];
    int delayedCounterR[10];
    float reverb_threshold_delay[] =
    {
        44.1f, 50.1f, 60.1f, 70.1f, 73.1f, 79.1f, 64.0f, 55.0f, 20.0f, 32.0f
    };
    float allBuffer_L[10][5760];
    float allBuffer_R[10][5760];
    float delay_left_buffer[MAX_COMB_FILTERS][100000];
    float delay_right_buffer[MAX_COMB_FILTERS][100000];
    float left_buffer[MAX_COMB_FILTERS][100000];
    float right_buffer[MAX_COMB_FILTERS][100000];
    int counters_L[MAX_COMB_FILTERS];
    int counters_R[MAX_COMB_FILTERS];
    rFilter LFP_L;
    rFilter LFP_R;
    char num_echoes;
    int delays[MAX_COMB_FILTERS];       // delays for the comb filters
    float decays[MAX_COMB_FILTERS];
#endif // PTK_COMPRESSOR

float buf024[2][MAX_TRACKS];
float buf124[2][MAX_TRACKS];
float buf0[2][MAX_TRACKS];
float buf1[2][MAX_TRACKS];
float fx1[2][MAX_TRACKS];
float fx2[2][MAX_TRACKS];
float fy1[2][MAX_TRACKS];
float fy2[2][MAX_TRACKS];
float Final_fx1[2];
float Final_fx2[2];
float Final_fy1[2];
float Final_fy2[2];
float fx1_chorus[2];
float fx2_chorus[2];
float fy1_chorus[2];
float fy2_chorus[2];
float xi0[2][MAX_TRACKS];
float xi1[2][MAX_TRACKS];
float xi2[2][MAX_TRACKS];

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    float lchorus_feedback = 0.6f;
    float rchorus_feedback = 0.5f;
    int lchorus_delay = 10584;
    int rchorus_delay = 15876;
    unsigned int lchorus_counter = MIX_RATE;
    unsigned int rchorus_counter = MIX_RATE;
    unsigned int lchorus_counter2 = MIX_RATE - lchorus_delay;
    unsigned int rchorus_counter2 = MIX_RATE - rchorus_delay;
#else
    float lchorus_feedback;
    float rchorus_feedback;
    int lchorus_delay;
    int rchorus_delay;
    unsigned int lchorus_counter;
    unsigned int rchorus_counter;
    unsigned int lchorus_counter2;
    unsigned int rchorus_counter2;
#endif

// 0-->Off 1-->On
char compressor;

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    float Reverb_Filter_Cutoff = 0.08f;
    float Reverb_Filter_Resonance = 0.5f;
    unsigned char Reverb_Stereo_Amount = 63;
    float Reverb_Damp = 1.0f;
#else
    float Reverb_Filter_Cutoff;
    float Reverb_Filter_Resonance;
    unsigned char Reverb_Stereo_Amount;
    float Reverb_Damp;
#endif

int Reserved_Sub_Channels[MAX_TRACKS][MAX_POLYPHONY];
int Note_Sub_Channels[MAX_TRACKS][MAX_POLYPHONY];

#if defined(PTK_INSTRUMENTS)
int sp_Stage[MAX_TRACKS][MAX_POLYPHONY];
#endif

int Cut_Stage[MAX_TRACKS][MAX_POLYPHONY];
int Synth_Really_Used[MAX_TRACKS][MAX_POLYPHONY];
int Glide_Stage[MAX_TRACKS][MAX_POLYPHONY];

#if defined(PTK_SYNTH)
int sp_Stage2[MAX_TRACKS][MAX_POLYPHONY];
int sp_Stage3[MAX_TRACKS][MAX_POLYPHONY];
#endif

char SampleCompression[MAX_INSTRS];
int delay_time;

#if defined(PTK_FLANGER)
    float Flanger_sbuf0L[MAX_TRACKS];
    float Flanger_sbuf1L[MAX_TRACKS];
    float Flanger_sbuf0R[MAX_TRACKS];
    float Flanger_sbuf1R[MAX_TRACKS];
#endif

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    int DelayType = 1;
#endif

#if !defined(__STAND_ALONE__)
    int L_MaxLevel;
    int R_MaxLevel;
    extern int Chan_Midi_Prg[MAX_TRACKS];
    float *Scope_Dats[MAX_TRACKS];
    float *VuMeters_Dats_L[MAX_TRACKS];
    float *VuMeters_Dats_R[MAX_TRACKS];
    float VuMeters_Level_Dats_L[MAX_TRACKS];
    float VuMeters_Level_Dats_R[MAX_TRACKS];
    float *Scope_Dats_LeftRight[2];
    int pos_scope;
    int pos_scope_latency;
    extern signed char c_midiin;
    extern signed char c_midiout;
    int play_pattern;
    int reset_carriers;
    int Midiprg[128];
    int LastProgram[MAX_TRACKS];
    int wait_level;
    char nameins[128][20];
    char SampleName[128][16][64];
    unsigned char nPatterns = 1;
    extern char sr_isrecording;
    extern int32 sed_range_start;
    extern int32 sed_range_end;
#else
    unsigned char nPatterns;
#endif

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
extern int Midi_Notes_History[MAX_TRACKS][256];
extern int Midi_Current_Notes[MAX_TRACKS][MAX_POLYPHONY];
#endif
#endif

#if !defined(__STAND_ALONE__)
void Clear_Midi_Channels_Pool(void);
#endif

// ------------------------------------------------------
// Functions
float Apply_Lfo_To_Filter(float value, int channel);
float Apply_Lfo_To_Volume(int channel);
float Apply_Lfo_To_Panning(float value, int channel);
void ComputeCoefs(int freq, int r, int t);
void Record_Delay_Event();

#if defined(PTK_FX_TICK0)
    void Do_Effects_Tick_0(void);
#endif
#if defined(PTK_FX_PATTERNLOOP)
    void Do_Pattern_Loop(int track);
#endif

void Do_Effects_Ticks_X(void);
float Filter_Chorus(int stereo, float x);
float Filter(int stereo, float x, char i);
float filter2p(int stereo, int ch, float input, float f, float q);
float filter2px(int stereo, int ch, float input, float f, float q);
float filter2p24d(int stereo, int ch, float input, float f, float q);
float filterRingMod(int stereo, int ch, float input, float f, float q);
float filterRingModStereo(int stereo, int ch, float input);
float filterWater(int stereo, int ch, float input, float f);
float filterWaterStereo(int stereo, int ch, float input, float f);
float filterBellShaped(int stereo, int ch, float input, float f, float q, float g);
float filterDelta(int stereo, int ch, float input, float f, float q);
float int_filter2p(int stereo, int ch, float input, float f, float q, float q2);
float filterhp(int stereo, int ch, float input, float f, float q);
float filterhp2(int stereo, int ch, float input, float f, float q);

#if defined(PTK_303)
    void live303(int pltr_eff_row, int pltr_dat_row);
    void Fire303(unsigned char number, int unit);
    void Go303(void);
#endif

float Cutoff(int v);
float Resonance(float v);
float Bandwidth(int v);
void Reverb_work(void);

void Initreverb(void);

#if defined(PTK_FLANGER)
    __inline float Filter_FlangerL(int track, float input);
    __inline float Filter_FlangerR(int track, float input);
#endif

volatile int Done_Reset;
void Reset_Values(void);

// -----------------------------------------------------------------------------
// Return the absolute value of a floating point
// -----------------------------------------------------------------------------
float absf(float x)
{
    *(unsigned int *) &x &= 0x7fffffff;
    return(x);
}

#define DENORMAL 1

#if DENORMAL
__inline float denormal(float sample)
{
    unsigned int isample;
    *(unsigned int *) &isample = *(unsigned int *) &sample;

    int abs_mantissa = isample & 0x007FFFFF;
    int biased_exponent = isample & 0x7F800000;
    if(biased_exponent == 0 && abs_mantissa != 0) 
    {
        return 0;
    }
    return sample;
}
#endif

#if defined(USE_FASTPOW)
void ToFloat(int *dest, int val)
{
    *dest = val;
}

#if defined(__GCC__) && !(__MACOSX_X86__) && !(__LINUX__)  && !(__HAIKU__)
static __inline__ float FastFloor(float f)
{
    float b, c, d, e, g, h, t;

    c = (f >= 0.0f) ? -8388608.0f: 8388608.0f;
    b = absf(f);
    d = f - c;
    e = b - 8388608.0f;
#if defined(__LINUX__)
    __asm__("" : "+mf" (d));
#else
#if defined(__AROS__)
    __asm__("" : "+d" (d));
#else
    __asm__("" : "+f" (d));
#endif
#endif
    d = d + c;
    g = f - d;
    h = (g >= 0.0f) ? 0.0f: 1.0f;
    t = d - h;
    return (e >= 0.0f) ? f: t;
}
#endif

#if defined(__PSP__)
float FastPow2(float x)
{
    float result;

    __asm__ volatile(
    "mtv      %1, S000\n"
    "vexp2.s  S000, S000\n"
    "mfv      %0, S000\n"
    : "=r"(result) : "r"(x));
    return result;
}
#else
float FastPow2(float i)
{
    float x;
#if defined(__GCC__) && !(__MACOSX_X86__) && !(__LINUX__) && !(__HAIKU__)
    float y = i - FastFloor(i);
#else
    float y = i - floorf(i);
#endif
    y = (y - y * y) * 0.33971f;
    x = i + 127 - y;
    x *= (1 << 23);
    ToFloat((int *) &x, (int) x);
    return x;
}
#endif

float FastLog(float i)
{
    float x;
    float y;
    x = (float) (*(int *) &i);
    x *= 1.0f / (1 << 23);
    x = x - 127;
#if defined(__GCC__) && !(__MACOSX_X86__) && !(__LINUX__) && !(__HAIKU__)
    y = x - FastFloor(x);
#else
    y = x - floorf(x);
#endif
    y = (y - y * y) * 0.346607f;
    return x + y;
}
float FastPow(float a, float b)
{
    return FastPow2(b * FastLog(a));
}
#endif

// ------------------------------------------------------
// Audio mixer
#if !defined(BZR2)
void STDCALL Mixer(Uint8 *Buffer, Uint32 Len)
#else
Uint32 STDCALL Mixer(Uint8 *Buffer, Uint32 Len)
#endif
{

#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__) || defined(BZR2)
    float *pSamples_flt = (float *) Buffer;
#endif

    short *pSamples = (short *) Buffer;
    int i;

#if !defined(__STAND_ALONE__)
    float clamp_left_value;
    float clamp_right_value;
#endif

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    if(thread_sema)
    {
        SDL_SemWait(thread_sema);
    }
#endif

#if defined(BZR2)
    Uint32 numSamples = Len;
#endif

#if !defined(__STAND_ALONE__)
    if(!rawrender && Buffer)
    {
#endif

#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
        for(i = Len - 1; i >= 0; i -= 8)
#elif defined(BZR2)
        for(; numSamples; --numSamples)
#else
        for(i = Len - 1; i >= 0; i -= 4)
#endif
        {

#if defined(BZR2)
            if(!Song_Playing)
            {
                break;
            }
            if(done)
            {
                if(numSamples == Len)
                {
                    done = FALSE;
                }
                break;
            }
#endif

            Get_Player_Values();

#if !defined(__STAND_ALONE__)
            // Gather data for the scopes and the vumeters
            clamp_left_value = left_float * 32767.0f;
            clamp_right_value = right_float * 32767.0f;
#endif

#if !defined(__WINAMP__)
#if !defined(__STAND_ALONE__) 
            // Add the metronome at the end of the audio chain
            if(trigger_metronome)
            {
                short Left_Dat;
                short Right_Dat;
                Left_Dat = (Metronome_Dats[(metronome_internal_counter_int * 2) + 1] << 8) |
                           (Metronome_Dats[(metronome_internal_counter_int * 2)] & 0xff);
                Right_Dat = Left_Dat;

                // ([1.0..-1.0f])
                left_float += (float) (Left_Dat) / 32767.0f;
                right_float += (float) (Right_Dat) / 32767.0f;
                
                metronome_internal_counter_int++;
                if((metronome_internal_counter_int) == Metronome_Dats_Size)
                {
                    metronome_internal_counter_int = 0;
                    trigger_metronome = FALSE;
                    metronome_latency = TRUE;
                }
            }
#endif
#endif

#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__) || defined(BZR2)
            *pSamples_flt++ = left_float;
            *pSamples_flt++ = right_float;
#else
            *pSamples++ = (short) (left_float * 32767.0f);
            *pSamples++ = (short) (right_float * 32767.0f);
#endif

#if !defined(__STAND_ALONE__)
            // Pre-record
            Scope_Dats_LeftRight[0][pos_scope] = clamp_left_value;
            Scope_Dats_LeftRight[1][pos_scope] = clamp_right_value;

            clamp_left_value = absf(Scope_Dats_LeftRight[0][pos_scope_latency]);
            clamp_right_value = absf(Scope_Dats_LeftRight[1][pos_scope_latency]);
            if(clamp_left_value > L_MaxLevel) L_MaxLevel = (int) clamp_left_value;
            if(clamp_right_value > R_MaxLevel) R_MaxLevel = (int) clamp_right_value;
            wait_level++;
            if(wait_level > 127)
            {
                wait_level = 0;
                L_MaxLevel -= 128;
                R_MaxLevel -= 128;
                if(L_MaxLevel < 0) L_MaxLevel = 0;
                if(R_MaxLevel < 0) R_MaxLevel = 0;
            }

            pos_scope++;
            if(pos_scope >= (AUDIO_Latency / 2)) pos_scope = 0;
            pos_scope_latency = pos_scope - (AUDIO_Latency / 4);
            if(pos_scope_latency < 0) pos_scope_latency = (AUDIO_Latency / 2) + pos_scope_latency;
            if(pos_scope_latency < 0) pos_scope_latency = 0;
#endif
        }

        if(local_curr_ramp_vol <= 0.0f)
        {
            Reset_Values();
        }

#if !defined(__STAND_ALONE__)
    } //RawRender
#endif

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    if(thread_sema)
    {
        SDL_SemPost(thread_sema);
    }
#endif

#if defined(BZR2)
    return Len - numSamples;
#endif

}

// ------------------------------------------------------
// Init the replayer driver
#if !defined(__WINAMP__)

#if defined(__WIN32__)
int STDCALL Ptk_InitDriver(HWND hWnd, int milliseconds)
{
    AUDIO_Milliseconds = milliseconds;
#else
int STDCALL Ptk_InitDriver(int milliseconds)
{
    AUDIO_Milliseconds = milliseconds;
#endif

#else
int STDCALL Ptk_InitDriver(void)
{
#endif // !defined(__WINAMP__)

    int i;

#if defined(PTK_SYNTH)
    // Create the stock waveforms
    float incr = 1.0f / 360.0f;
    float stop = 1.0f;
    float x;

#if defined(PTK_SYNTH_SAW)
    unsigned short temp_saw;
#endif

#if defined(PTK_SYNTH_SIN)
    short *wav_sin = STOCK_SIN;
    memset(STOCK_SIN, 0, sizeof(STOCK_SIN));
#endif

#if defined(PTK_SYNTH_SAW)
    short *wav_saw = STOCK_SAW;
    memset(STOCK_SAW, 0, sizeof(STOCK_SAW));
#endif

#if defined(PTK_SYNTH_PULSE)
    short *wav_pul = STOCK_PULSE;
    memset(STOCK_PULSE, 0, sizeof(STOCK_PULSE));
#endif

#if defined(PTK_SYNTH_TRI)
    int tri_carrier;
    int tri_carrier_step;
    short *wav_tri = STOCK_TRI;
    tri_carrier = 0;
    tri_carrier_step = 1;
    memset(STOCK_TRI, 0, sizeof(STOCK_TRI));
#endif

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
#if defined(PTK_LIMITER_MASTER)
    Mas_Compressor_Set_Variables_Master(100.0f, 0.0f);
#endif
#if defined(PTK_LIMITER_TRACKS)
    for(i = 0; i < MAX_TRACKS; i++)
    {
        Mas_Compressor_Set_Variables_Track(i, 100.0f, 0.0f);
    }
#endif
#endif


    SIZE_WAVEFORMS = 0;
    for(x = 0; x < 360.0f; x += 1.0f)
    {
        float value = x * 0.0174532f;

#if defined(PTK_SYNTH_PULSE)
        if(sinf(value) < 0.0f) *wav_pul++ = 32767;
        else *wav_pul++ = -32767;
#endif

#if defined(PTK_SYNTH_SAW)
        // There's a problem with fmodf->signed short in mingw here
        temp_saw = (unsigned short) ((fmodf(x / 360.0f, 64.0f) * 32767.0f) * 2.0f);
        *wav_saw++ = (short) (((float) (short) temp_saw));
#endif

#if defined(PTK_SYNTH_TRI)
        if((((int) x) % 180) == 0)
        {
            tri_carrier_step = -tri_carrier_step;
        }
        *wav_tri++ = (short) (((((float) tri_carrier / 180.0f) + 0.5f) * 32767.0f) * 2.0f);
        tri_carrier += tri_carrier_step;
#endif

#if defined(PTK_SYNTH_SIN)
        *wav_sin++ = (short) (sinf(value) * 32767.0f);
#endif

        SIZE_WAVEFORMS++;
    }

#if defined(PTK_SYNTH_WHITE)
    short *wav_wit = STOCK_WHITE;
    int carrier;
    
    memset(STOCK_WHITE, 0, sizeof(STOCK_WHITE));
    incr = 1.0f / fMIX_RATE;
    stop = 2.0f;
    carrier = 0;

    for(x = 0; x < (stop - incr / 2); x += incr)
    {
        *wav_wit++ = (short) (rand() - 16384) * 2;
    }
#endif

#endif // PTK_SYNTH

    // Initializing work SINETABLE
    for(i = 0; i < 360; i++)
    {
        SIN[i] = (float) sinf(i * 0.0174532f);
    }

    // Initializing work panning
    for(i = 0; i < 1025; i++)
    {
        SQRT[i] = (float) sqrtf(i / 1024.0f);
    }

#if !defined(__WINAMP__)

#if defined(__WIN32__)
    if(!AUDIO_Init_Driver(hWnd, &Mixer))
#else
    if(!AUDIO_Init_Driver(&Mixer))
#endif
    {
        return(FALSE);
    }

#if !defined(__STAND_ALONE__)
    if(!Init_Scopes_VuMeters_Buffers()) return(FALSE);
#endif

    AUDIO_Play();

#else  // !defined(__WINAMP__)

    Pre_Song_Init();

#endif // defined(__WINAMP__)

    return(TRUE);
}

// ------------------------------------------------------
// Load a module
#if defined(__STAND_ALONE__)
Uint8 *Cur_Module;

// ------------------------------------------------------
// Retrieve data from the ptp mod
void Mod_Dat_Read(void *Dest, int size)
{
    memcpy(Dest, Cur_Module, size);
    Cur_Module += size;
}

#if defined(PTK_INSTRUMENTS)
short *Unpack_Sample(int Dest_Length, char Pack_Type, int BitRate)
{
    int Packed_Length;
    short *Dest_Buffer;
    Uint8 *Packed_Read_Buffer;

    Mod_Dat_Read(&Packed_Length, sizeof(int));
    if(Packed_Length == -1)
    {
        // Sample wasn't packed

#if defined(__PSVITA__)
        Packed_Read_Buffer = (Uint8 *) PSVITA_malloc(Dest_Length * 2 + 8);
#else
        Packed_Read_Buffer = (Uint8 *) malloc(Dest_Length * 2 + 8);
        memset(Packed_Read_Buffer, 0, Dest_Length * 2 + 8);
#endif

        Mod_Dat_Read(Packed_Read_Buffer, sizeof(char) * (Dest_Length * 2));
        return((short *) Packed_Read_Buffer);
    }
    else
    {

#if defined(__PSVITA__)
        Packed_Read_Buffer = (Uint8 *) PSVITA_malloc(Packed_Length);
#else
        Packed_Read_Buffer = (Uint8 *) malloc(Packed_Length);
#endif

        // Read the packer buffer
        Mod_Dat_Read(Packed_Read_Buffer, sizeof(char) * Packed_Length);

#if defined(__PSVITA__)
        Dest_Buffer = (short *) PSVITA_malloc(Dest_Length * 2 + 8);
#else
        Dest_Buffer = (short *) malloc(Dest_Length * 2 + 8);
        memset(Dest_Buffer, 0, Dest_Length * 2 + 8);
#endif

#if defined(PTK_AT3) || defined(PTK_GSM) || defined(PTK_MP3) || \
    defined(PTK_ADPCM) || defined(PTK_8BIT) || \
    defined(PTK_WAVPACK)

        switch(Pack_Type)
        {

#if defined(__PSP__)
#if defined(PTK_AT3)
            case SMP_PACK_AT3:
                Unpack_AT3(Packed_Read_Buffer, Dest_Buffer, Packed_Length, Dest_Length, BitRate);
                break;
#endif
#endif

#if defined(PTK_GSM)
            case SMP_PACK_GSM:
                Unpack_GSM(Packed_Read_Buffer, Dest_Buffer, Packed_Length, Dest_Length);
                break;
#endif

#if defined(PTK_MP3)
            case SMP_PACK_MP3:
                Unpack_MP3(Packed_Read_Buffer, Dest_Buffer, Packed_Length, Dest_Length, BitRate);
                break;
#endif

#if defined(PTK_ADPCM)
            case SMP_PACK_ADPCM:
                Unpack_ADPCM(Packed_Read_Buffer, Dest_Buffer, Packed_Length, Dest_Length);
                break;
#endif

#if defined(PTK_8BIT)
            case SMP_PACK_8BIT:
                Unpack_8Bit(Packed_Read_Buffer, Dest_Buffer, Packed_Length, Dest_Length);
                break;
#endif

#if defined(PTK_WAVPACK)
            case SMP_PACK_WAVPACK:
                Unpack_WavPack(Packed_Read_Buffer, Dest_Buffer, Packed_Length, Dest_Length);
                break;
#endif

        }
#endif

#if defined(__PSVITA__)
        PSVITA_free(Packed_Read_Buffer);
#else
        free(Packed_Read_Buffer);
#endif

        return(Dest_Buffer);
    }
}
#endif // PTK_INSTRUMENTS

int PTKEXPORT Ptk_InitModule(Uint8 *Module, int start_position)
{
    Uint32 *dwModule = (Uint32 *) Module;
    Cur_Module = Module;

    int i;
    int j;
    int k;
    unsigned char *TmpPatterns;
    unsigned char *TmpPatterns_Tracks;
    unsigned char *TmpPatterns_Notes;
    unsigned char *TmpPatterns_Rows;
    int tps_trk;
    int nbr_instr;
    int twrite;

    // .ptp modules aren't portable from big endian platform to little endian ones
    // (so that header will be saved as PRTK on little endian platforms and as KTRP on the other ones)
    if(dwModule[0] == 'KTRP')
    {
        Cur_Module += 4;

        Pre_Song_Init();

        Mod_Dat_Read(&nPatterns, sizeof(char));
        Mod_Dat_Read(&Song_Tracks, sizeof(char));
        Mod_Dat_Read(&Song_Length, sizeof(char));

        Mod_Dat_Read(&Use_Cubic, sizeof(char));

        Mod_Dat_Read(pSequence, sizeof(char) * Song_Length);

        // Patterns lines
        for(i = 0; i < nPatterns; i++)
        {
            patternLines[i] = 0;
            Mod_Dat_Read(&patternLines[i], sizeof(char));
            patternLines[i] = Swap_16(patternLines[i]);
        }

        // Allocated the necessary room for the patterns
        int max_lines = (PATTERN_LEN * nPatterns);

#if defined(__PSVITA__)
        // Free the patterns block
        if(RawPatterns) PSVITA_free(RawPatterns);
#else
        // Free the patterns block
        if(RawPatterns) free(RawPatterns);
#endif

#if defined(__PSP__)
        RawPatterns = (unsigned char *) AUDIO_malloc_64(&max_lines);
#else
#if defined(__PSVITA__)
        RawPatterns = (unsigned char *) PSVITA_malloc(max_lines);
#else
        RawPatterns = (unsigned char *) malloc(max_lines);
#endif
#endif

        if(!RawPatterns) return(FALSE);

        // Multi notes
        Mod_Dat_Read(Channels_MultiNotes, sizeof(char) * Song_Tracks);

        // Multi fx
        Mod_Dat_Read(Channels_Effects, sizeof(char) * Song_Tracks);

        // Individual volumes
        Mod_Dat_Read(Track_Volume, sizeof(float) * Song_Tracks);

        // Surround effect
        Mod_Dat_Read(Track_Surround, sizeof(char) * Song_Tracks);

        // Eq parameters
        for(i = 0; i < Song_Tracks; i++)
        {
            Mod_Dat_Read(&EqDat[i].lg, sizeof(float));
            Mod_Dat_Read(&EqDat[i].mg, sizeof(float));
            Mod_Dat_Read(&EqDat[i].hg, sizeof(float));
        }

        // Surround effect
        Mod_Dat_Read(Track_Denoise, sizeof(char) * Song_Tracks);

        TmpPatterns = RawPatterns;
        for(int pwrite = 0; pwrite < nPatterns; pwrite++)
        {
            TmpPatterns_Rows = TmpPatterns + (pwrite * PATTERN_LEN);
            for(i = 0; i < PATTERN_BYTES; i++)
            {   // Bytes / track
                for(k = 0; k < Song_Tracks; k++)
                {   // Tracks
                    TmpPatterns_Tracks = TmpPatterns_Rows + (k * PATTERN_BYTES);
                    for(j = 0; j < patternLines[pwrite]; j++)
                    {   // Rows
                        TmpPatterns_Notes = TmpPatterns_Tracks + (j * PATTERN_ROW_LEN);
                        Mod_Dat_Read(TmpPatterns_Notes + i, sizeof(char));
                    }
                }
            }
        }

        Mod_Dat_Read(&nbr_instr, sizeof(int));

        for(int swrite = 0; swrite < nbr_instr; swrite++)
        {
            Mod_Dat_Read(&Synth_Prg[swrite], sizeof(char));

            Mod_Dat_Read(&Beat_Sync[swrite], sizeof(char));
            Mod_Dat_Read(&Beat_Lines[swrite], sizeof(short));
            Mod_Dat_Read(&Sample_Vol[swrite], sizeof(float));

#if defined(PTK_SYNTH)
            if(Synth_Prg[swrite])
            {
                Mod_Dat_Read(&PARASynth[swrite], sizeof(SYNTH_DATA));
            }
#endif

            // Compression type
            Mod_Dat_Read(&SampleCompression[swrite], sizeof(char));

#if defined(PTK_MP3) || defined(PTK_AT3)
            switch(SampleCompression[swrite])
            {

#if defined(PTK_MP3)
                case SMP_PACK_MP3:
                    Mod_Dat_Read(&Mp3_BitRate[swrite], sizeof(char));
                    break;
#endif

#if defined(PTK_AT3)
                case SMP_PACK_AT3:
                    Mod_Dat_Read(&At3_BitRate[swrite], sizeof(char));
                    break;
#endif

            }
#endif
            for(int slwrite = 0; slwrite < MAX_INSTRS_SPLITS; slwrite++)
            {
                Mod_Dat_Read(&SampleType[swrite][slwrite], sizeof(char));
                
#if defined(PTK_INSTRUMENTS)
                if(SampleType[swrite][slwrite])
                {
                    int Apply_Interpolation;
                    Uint32 Save_Len;
                    Uint32 iSmp;
                    short Sample1;
                    short Sample2;
                    short *Sample_Buffer = NULL;
                    short *Sample_Dest_Buffer;

                    // No samples names in packed modules
                    Mod_Dat_Read(&Basenote[swrite][slwrite], sizeof(char));
                    Mod_Dat_Read(&LoopStart[swrite][slwrite], sizeof(int));
                    Mod_Dat_Read(&LoopEnd[swrite][slwrite], sizeof(int));
                    Mod_Dat_Read(&LoopType[swrite][slwrite], sizeof(char));
                    Mod_Dat_Read(&Sample_Length[swrite][slwrite], sizeof(int));
                    Mod_Dat_Read(&Finetune[swrite][slwrite], sizeof(char));
                    Mod_Dat_Read(&Sample_Amplify[swrite][slwrite], sizeof(float));
                    Mod_Dat_Read(&FDecay[swrite][slwrite], sizeof(float));
                    Save_Len = Sample_Length[swrite][slwrite];

                    Apply_Interpolation = SampleCompression[swrite] == SMP_PACK_NONE ? FALSE : TRUE;

                    if(Apply_Interpolation)
                    {
                        Save_Len /= 2;
                        Sample_Buffer = Unpack_Sample(Save_Len,
                                                      SampleCompression[swrite],
                                                      SampleCompression[swrite] == SMP_PACK_MP3 ?
#if defined(PTK_MP3)
                                                      Type_Mp3_BitRate[Mp3_BitRate[swrite]]
#else
                                                      0
#endif
                                                      :

#if defined(PTK_AT3)
                                                      Type_At3_BitRate[At3_BitRate[swrite]]
#else
                                                      0
#endif
                                                     );

#if defined(__PSVITA__)
                        Sample_Dest_Buffer = (short *) PSVITA_malloc((Save_Len * 2 * sizeof(short)) + 8);
#else
                        Sample_Dest_Buffer = (short *) malloc((Save_Len * 2 * sizeof(short)) + 8);
                        memset(Sample_Dest_Buffer, 0, (Save_Len * 2 * sizeof(short)) + 8);
#endif

                        // Interpolate samples
                        for(iSmp = 0; iSmp < Save_Len; iSmp++)
                        {
                            Sample1 = Sample_Buffer[iSmp];
                            Sample2 = Sample_Buffer[iSmp + 1];
                            if(iSmp == Save_Len - 1)
                            {
                                if(LoopType[swrite][slwrite])
                                {
                                    Sample2 = Sample_Dest_Buffer[LoopStart[swrite][slwrite]];
                                }
                                else
                                {
                                    Sample2 = Sample_Buffer[iSmp];
                                }
                            }
                            Sample_Dest_Buffer[(iSmp * 2)] = Sample1;
                            Sample_Dest_Buffer[(iSmp * 2) + 1] = Sample1 + ((Sample2 - Sample1) / 2);
                        }
                        RawSamples[swrite][0][slwrite] = Sample_Dest_Buffer;
                    }
                    else
                    {
                        RawSamples[swrite][0][slwrite] = Unpack_Sample(Save_Len,
                                                                       SampleCompression[swrite],
                                                                       SampleCompression[swrite] == SMP_PACK_MP3 ?
#if defined(PTK_MP3)
                                                                            Type_Mp3_BitRate[Mp3_BitRate[swrite]]
#else
                                                                            0
#endif
                                                                            :
#if defined(PTK_AT3)
                                                                            Type_At3_BitRate[At3_BitRate[swrite]]
#else
                                                                            0
#endif
                                                                      );
                    }

                    // Stereo flag
                    Mod_Dat_Read(&Sample_Channels[swrite][slwrite], sizeof(char));
                    if(Sample_Channels[swrite][slwrite] == 2)
                    {
                        if(Apply_Interpolation)
                        {
                            Sample_Buffer = Unpack_Sample(Save_Len,
                                                          SampleCompression[swrite],
                                                          SampleCompression[swrite] == SMP_PACK_MP3 ?
#if defined(PTK_MP3)
                                                                Type_Mp3_BitRate[Mp3_BitRate[swrite]]
#else
                                                                0
#endif
                                                                :
#if defined(PTK_AT3)
                                                                Type_At3_BitRate[At3_BitRate[swrite]]
#else
                                                                0
#endif
                                                         );

#if defined(__PSVITA__)
                            Sample_Dest_Buffer = (short *) PSVITA_malloc((Save_Len * 2 * sizeof(short)) + 8);
#else
                            Sample_Dest_Buffer = (short *) malloc((Save_Len * 2 * sizeof(short)) + 8);
                            memset(Sample_Dest_Buffer, 0, (Save_Len * 2 * sizeof(short)) + 8);
#endif

                            for(iSmp = 0; iSmp < Save_Len; iSmp++)
                            {
                                Sample1 = Sample_Buffer[iSmp];
                                Sample2 = Sample_Buffer[iSmp + 1];
                                if(LoopType[swrite][slwrite])
                                {
                                    Sample2 = Sample_Dest_Buffer[LoopStart[swrite][slwrite]];
                                }
                                else
                                {
                                    Sample2 = Sample_Buffer[iSmp];
                                }
                                Sample_Dest_Buffer[(iSmp * 2)] = Sample1;
                                Sample_Dest_Buffer[(iSmp * 2) + 1] = Sample1 + ((Sample2 - Sample1) / 2);
                            }
                            RawSamples[swrite][1][slwrite] = Sample_Dest_Buffer;
                        }
                        else
                        {
                            RawSamples[swrite][1][slwrite] = Unpack_Sample(Save_Len,
                                                                           SampleCompression[swrite],
                                                                           SampleCompression[swrite] == SMP_PACK_MP3 ?
#if defined(PTK_MP3)
                                                                                Type_Mp3_BitRate[Mp3_BitRate[swrite]]
#else
                                                                                0
#endif
                                                                                :
#if defined(PTK_AT3)
                                                                                Type_At3_BitRate[At3_BitRate[swrite]]
#else
                                                                                0
#endif
                                                                          );
                        }
                    }

#if defined(__PSVITA__)
                    if(Sample_Buffer) PSVITA_free(Sample_Buffer);
#else
                    if(Sample_Buffer) free(Sample_Buffer);
#endif

                }// Exist Sample
#endif // PTK_INSTRUMENTS

            }
        }

        // Reading mod properties
        Mod_Dat_Read(&compressor, sizeof(char));

        // Reading Track Properties
        for(twrite = 0; twrite < Song_Tracks; twrite++)
        {
            Mod_Dat_Read(&TCut[twrite], sizeof(float));
            Mod_Dat_Read(&ICut[twrite], sizeof(float));
            Mod_Dat_Read(&TPan[twrite], sizeof(float));
            Compute_Stereo(twrite);
            Fix_Stereo(twrite);

            Mod_Dat_Read(&FType[twrite], sizeof(int));
            Mod_Dat_Read(&FRez[twrite], sizeof(int));
            Mod_Dat_Read(&DThreshold[twrite], sizeof(float));
            Mod_Dat_Read(&DClamp[twrite], sizeof(float));

#if defined(PTK_COMPRESSOR)
            if(compressor)
            {
                Mod_Dat_Read(&DSend[twrite], sizeof(float));
            }
#endif

            Mod_Dat_Read(&CSend[twrite], sizeof(int));
            Mod_Dat_Read(&Channels_Polyphony[twrite], sizeof(char));
        }

        Mod_Dat_Read(&c_threshold, sizeof(int));
        Mod_Dat_Read(&Beats_Per_Min, sizeof(int));
        Mod_Dat_Read(&Ticks_Per_Beat, sizeof(int));
        Mod_Dat_Read(&mas_vol, sizeof(float));

        char Comp_Flag;
        Mod_Dat_Read(&Comp_Flag, sizeof(char));

#if defined(PTK_LIMITER_MASTER)
        // Master compressor
        if(Comp_Flag)
        {
            Mod_Dat_Read(&mas_threshold_Master, sizeof(float));
            Mod_Dat_Read(&mas_ratio_Master, sizeof(float));
        }
#endif

        // Tracks compressors
        Mod_Dat_Read(&Comp_Flag, sizeof(char));
#if defined(PTK_LIMITER_TRACKS)
        if(Comp_Flag)
        {
            Mod_Dat_Read(&mas_threshold_Track, sizeof(float) * Song_Tracks);
            Mod_Dat_Read(&mas_ratio_Track, sizeof(float) * Song_Tracks);
            Mod_Dat_Read(&Compress_Track, sizeof(char) * Song_Tracks);
        }
#endif

        Mod_Dat_Read(&Feedback, sizeof(float));

#if defined(PTK_COMPRESSOR)
        if(compressor)
        {
            Mod_Dat_Read(&num_echoes, sizeof(char));

            for(i = 0; i < num_echoes; i++)
            {
                Mod_Dat_Read(&delays[i], sizeof(int));
            }
            for(i = 0; i < num_echoes; i++)
            {
                Mod_Dat_Read(&decays[i], sizeof(float));
            }
        }
#endif

        Mod_Dat_Read(&lchorus_delay, sizeof(int));
        Mod_Dat_Read(&rchorus_delay, sizeof(int));
        Mod_Dat_Read(&lchorus_feedback, sizeof(float));
        Mod_Dat_Read(&rchorus_feedback, sizeof(float));

        Mod_Dat_Read(&shuffle_amount, sizeof(int));

        // Reading track part sequence
        for(int tps_pos = 0; tps_pos < Song_Length; tps_pos++)
        {
            for(tps_trk = 0; tps_trk < Song_Tracks; tps_trk++)
            {
                Mod_Dat_Read(&Chan_Active_State[tps_pos][tps_trk], sizeof(char));
            }
        }

        for(int spl = 0; spl < Song_Tracks; spl++)
        {
            CCoef[spl] = float((float) CSend[spl] / 127.0f);
        }

        for(twrite = 0; twrite < Song_Tracks; twrite++)
        {
            Mod_Dat_Read(&LFO_ON[twrite], sizeof(char));

#if defined(PTK_LFO)
            if(LFO_ON[twrite])
            {
                Mod_Dat_Read(&LFO_RATE[twrite], sizeof(float));
                Mod_Dat_Read(&LFO_AMPL_FILTER[twrite], sizeof(float));
                Mod_Dat_Read(&LFO_AMPL_VOLUME[twrite], sizeof(float));
                Mod_Dat_Read(&LFO_AMPL_PANNING[twrite], sizeof(float));
                Mod_Dat_Read(&LFO_RATE_SCALE[twrite], sizeof(float));

            }
#endif
        }

        for(twrite = 0; twrite < Song_Tracks; twrite++)
        {
            Mod_Dat_Read(&FLANGER_ON[twrite], sizeof(char));

#if defined(PTK_FLANGER)
            if(FLANGER_ON[twrite])
            {
                Mod_Dat_Read(&FLANGER_AMOUNT[twrite], sizeof(float));
                Mod_Dat_Read(&FLANGER_DEPHASE[twrite], sizeof(float));
                Mod_Dat_Read(&FLANGER_RATE[twrite], sizeof(float));
                Mod_Dat_Read(&FLANGER_AMPL[twrite], sizeof(float));
                Mod_Dat_Read(&FLANGER_FEEDBACK[twrite], sizeof(float));
                Mod_Dat_Read(&FLANGER_DELAY[twrite], sizeof(int));
                FLANGER_OFFSET[twrite] = 8192;
                FLANGER_OFFSET2[twrite] = float(FLANGER_OFFSET[twrite] - FLANGER_DELAY[twrite]);
                FLANGER_OFFSET1[twrite] = float(FLANGER_OFFSET[twrite] - FLANGER_DELAY[twrite]);
            }
#endif

        }

        for(tps_trk = 0; tps_trk < Song_Tracks; tps_trk++)
        {
            Mod_Dat_Read(&Disclap[tps_trk], sizeof(char));
        }

        Mod_Dat_Read(&Reverb_Filter_Cutoff, sizeof(float));
        Mod_Dat_Read(&Reverb_Filter_Resonance, sizeof(float));
        Mod_Dat_Read(&Reverb_Stereo_Amount, sizeof(char));
        Mod_Dat_Read(&Reverb_Damp, sizeof(float));

        Mod_Dat_Read(&ChorType, sizeof(int));
        Mod_Dat_Read(&ChorCut, sizeof(int));
        Mod_Dat_Read(&ChorRez, sizeof(int));

        char tb303_1_enabled;
        char tb303_2_enabled;
        // Read the 303 data
        Mod_Dat_Read(&tb303_1_enabled, sizeof(char));

#if defined(PTK_303)
        tb303[0].enabled = tb303_1_enabled;
        if(tb303_1_enabled)
        {
            Mod_Dat_Read(&tb303[0].selectedpattern, sizeof(para303) - sizeof(char));
        }
#endif

        Mod_Dat_Read(&tb303_2_enabled, sizeof(char));

#if defined(PTK_303)
        tb303[1].enabled = tb303_2_enabled;
        if(tb303_2_enabled)
        {
            Mod_Dat_Read(&tb303[1].selectedpattern, sizeof(para303) - sizeof(char));
        }

        if(tb303_1_enabled) Mod_Dat_Read(&tb303engine[0].tbVolume, sizeof(float));
        if(tb303_2_enabled) Mod_Dat_Read(&tb303engine[1].tbVolume, sizeof(float));
#endif

        Song_Position = start_position;
        Post_Song_Init();
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}
#endif  // __STAND_ALONE__

// ------------------------------------------------------
// Release the replayer driver
void PTKEXPORT Ptk_ReleaseDriver(void)
{
#if !defined(__WINAMP__)
    AUDIO_Stop_Driver();
#endif

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
    // Free the patterns block
#if defined(__PSVITA__)
    if(RawPatterns) PSVITA_free(RawPatterns);
#else
    if(RawPatterns) free(RawPatterns);
#endif
    RawPatterns = NULL;
#endif

}

// ------------------------------------------------------
// Retrieve the current position in the song
int PTKEXPORT Ptk_GetRow(void)
{
    return(Pattern_Line_Visual);
}
int PTKEXPORT Ptk_GetPosition(void)
{
    return(Song_Position_Visual);
}

// ------------------------------------------------------
// Set the current position in the song
void PTKEXPORT Ptk_SetPosition(int new_position)
{
    if(new_position >= Song_Length) new_position = Song_Length - 1;
    if(new_position < 0) new_position = 0;

    Song_Position = new_position;
    Pattern_Line = 0;

#if defined(PTK_FX_PATTERNLOOP)
    // No repeat loop
    repeat_loop_pos = 0;
    repeat_loop_counter = 0;
    repeat_loop_counter_in = 0;
#endif

}

// ------------------------------------------------------
// Reset some values before starting playing
void Reset_Values(void)
{
    int i;
    int j;

    if(!Done_Reset)
    {
        Song_Playing = FALSE;

#if defined(PTK_FX_PATTERNLOOP)
        repeat_loop_pos = 0;
        repeat_loop_counter = 0;
        repeat_loop_counter_in = 0;
#endif
        
        lchorus_counter = MIX_RATE;
        rchorus_counter = MIX_RATE;
        lchorus_counter2 = MIX_RATE - lchorus_delay;
        rchorus_counter2 = MIX_RATE - rchorus_delay;

        memset(pos_round_float_history, 0, sizeof(pos_round_float_history));
        memset(left_float_history, 0, sizeof(left_float_history));
        memset(right_float_history, 0, sizeof(right_float_history));

        for(i = 0; i < 131072; i++)
        {
            lbuff_chorus[i] = 0.0f;
            rbuff_chorus[i] = 0.0f;
        }

        Reset_Chorus_Filters();

#if defined(PTK_LIMITER_MASTER)
        mas_comp_threshold_Master = 100.0f;
        mas_comp_ratio_Master = 0;
        rms_sumL_Master = 0;
        rms_sumR_Master = 0;
        mas_envL_Master = 0;
        mas_envR_Master = 0;
        for(i = 0; i < MAS_COMPRESSOR_SIZE; i++)
        {
            mas_comp_bufferL_Master[i] = 0;
            mas_comp_bufferR_Master[i] = 0;
        }
#endif

#if defined(PTK_COMPRESSOR)
        Initreverb();
#endif

        for(i = 0; i < MAX_TRACKS; i++)
        {
            Reset_Filters(i);

            glide[i] = 0;

#if defined(PTK_TRACKFILTERS)
            CCut[i] = 0.0f;
#endif
            ramper[i] = 0;
            New_Instrument[i] = 0;
            Pos_Segue[i] = 0;
            Segue_Volume[i] = 0;
            Segue_SamplesL[i] = 0;
            Segue_SamplesR[i] = 0;

#if defined(PTK_FLANGER)
            Flanger_sbuf0L[i] = 0;
            Flanger_sbuf1L[i] = 0;
            Flanger_sbuf0R[i] = 0;
            Flanger_sbuf1R[i] = 0;
#endif

#if defined(PTK_LIMITER_TRACKS)
            rms_sumL_Track[i] = 0;
            rms_sumR_Track[i] = 0;
            mas_envL_Track[i] = 0;
            mas_envR_Track[i] = 0;
            for(j = 0; j < MAS_COMPRESSOR_SIZE; j++)
            {
                mas_comp_bufferL_Track[i][j] = 0;
                mas_comp_bufferR_Track[i][j] = 0;
            }
#endif

            for(j = 0; j < MAX_POLYPHONY; j++)
            {
                Reserved_Sub_Channels[i][j] = -1;
                Note_Sub_Channels[i][j] = -1;

#if defined(PTK_FX_ARPEGGIO)
                Arpeggio_BaseNote[i][j] = 0;
                Vstep_arp[i][j] = 0;
#endif

#if defined(PTK_FX_VIBRATO)
                Vibrato_BaseNote[i][j] = 0;
                Vstep_vib[i][j] = 0;
#endif

#if defined(PTK_SYNTH)
                Synth_Was[i][j] = 0;
#endif

                Player_WL[i][j] = 0;
                Player_WR[i][j] = 0;
                Player_Ampli[i][j] = 0;
                Player_SC[i][j] = 0;
                Player_LT[i][j] = 0;
                Player_LW[i][j] = 0;
                Player_LS[i][j] = 0;
                Player_LE[i][j] = 0;
                Player_LL[i][j] = 0;
                Player_NS[i][j] = 0;

                sp_Step[i][j] = 0;

                sp_Cvol[i][j] = 0.0f;
                sp_Cvol_Ramp[i][j] = 0.0f;
                sp_Cvol_Ramp_Dest[i][j] = 10.0f;
                sp_Cvol_Synth[i][j] = 0.0f;
                sp_Cvol_Synth_Ramp[i][j] = 0.0f;
                sp_Cvol_Synth_Ramp_Dest[i][j] = 10.0f;

                sp_channelsample[i][j] = -1;
                sp_channelnote[i][j] = 120;
                sp_split[i][j] = 0;

                sp_Tvol[i][j] = 0.0f;
                sp_Tvol_Synth[i][j] = 0.0f;

#if defined(PTK_SYNTH)
                Synthesizer[i][j].Reset();
                sp_Stage2[i][j] = PLAYING_NOSAMPLE;
                sp_Stage3[i][j] = PLAYING_NOSAMPLE;
#endif

#if defined(PTK_INSTRUMENTS)
                sp_Stage[i][j] = PLAYING_NOSAMPLE;
#endif

                Reserved_Sub_Channels[i][j] = -1;
                Note_Sub_Channels[i][j] = -1;
                sp_channelsample[i][j] = -1;
                sp_channelnote[i][j] = 120;
                
                Cut_Stage[i][j] = FALSE;
                Glide_Stage[i][j] = FALSE;

#if defined(PTK_INSTRUMENTS)
                sp_Stage[i][j] = PLAYING_NOSAMPLE;
#endif

#if defined(PTK_SYNTH)
                sp_Stage2[i][j] = PLAYING_NOSAMPLE;
                sp_Stage3[i][j] = PLAYING_NOSAMPLE;
#endif

                sp_Position[i][j].int_pos = 0;
                sp_Position[i][j].flt_pos = 0;

#if defined(PTK_SYNTH)
                sp_Position_osc_1[i][j].int_pos = 0;
                sp_Position_osc_1[i][j].flt_pos = 0;
                sp_Position_osc_2[i][j].int_pos = 0;
                sp_Position_osc_2[i][j].flt_pos = 0;

#if defined(PTK_SYNTH_OSC_3)
                sp_Position_osc_3[i][j].int_pos = 0;
                sp_Position_osc_3[i][j].flt_pos = 0;
#endif
#endif

                old_note[i][j] = 0;
                Vstep1[i][j] = 0;
            
#if defined(PTK_FX_ARPEGGIO)
                Arpeggio_BaseNote[i][j] = 0;
                Vstep_arp[i][j] = 0;
#endif

#if defined(PTK_FX_VIBRATO)
                Vibrato_BaseNote[i][j] = 0;
                Vstep_vib[i][j] = 0;
#endif

                Cut_Stage[i][j] = FALSE;
                Glide_Stage[i][j] = FALSE;

#if defined(PTK_INSTRUMENTS)
                sp_Tvol[i][j] = 0.0f;
                sp_Cvol[i][j] = 0.0f;
                sp_Cvol_Ramp[i][j] = 0.0f;
                sp_Cvol_Ramp_Dest[i][j] = 10.0f;
#endif

#if defined(PTK_SYNTH)
                sp_Tvol_Synth[i][j] = 0.0f;
                sp_Cvol_Synth[i][j] = 0.0f;
                sp_Cvol_Synth_Ramp[i][j] = 0.0f;
                sp_Cvol_Synth_Ramp_Dest[i][j] = 10.0f;
#endif

            }

#if defined(PTK_FX_ARPEGGIO)
            Arpeggio_Switch[i] = 0;
#endif

#if defined(PTK_FX_VIBRATO)
            Vibrato_Switch[i] = 0;
#endif
        
#if defined(PTK_FX_AUTOFADEMODE)
            FADEMODE[i] = 0;
            FADECOEF[i] = 0.0f;
#endif
       
            sp_Tvol_Mod[i] = 1.0f;

#if defined(PTK_FX_REVERSE)
            Reverse_Switch[i] = 0;
            Reserve_Dat[i] = 0;
#endif

            old_TPan[i] = TPan[i];
            sp_Tvol_Mod[i] = 1.0f;

            Player_FD[i] = 0.0f;

            oldspawn[i] = 0;
            roldspawn[i] = 0;

#if defined(PTK_TRACK_EQ)
            EqDat[i].f1p0[0] = 0.0f;
            EqDat[i].f1p0[1] = 0.0f;
        
            EqDat[i].f1p1[0] = 0.0f;
            EqDat[i].f1p1[1] = 0.0f;
        
            EqDat[i].f1p2[0] = 0.0f;
            EqDat[i].f1p2[1] = 0.0f;
        
            EqDat[i].f1p3[0] = 0.0f;
            EqDat[i].f1p3[1] = 0.0f;
        
            EqDat[i].f2p0[0] = 0.0f;
            EqDat[i].f2p0[1] = 0.0f;

            EqDat[i].f2p1[0] = 0.0f;
            EqDat[i].f2p1[1] = 0.0f;

            EqDat[i].f2p2[0] = 0.0f;
            EqDat[i].f2p2[1] = 0.0f;
        
            EqDat[i].f2p3[0] = 0.0f;
            EqDat[i].f2p3[1] = 0.0f;

            EqDat[i].sdm1[0] = 0.0f;
            EqDat[i].sdm1[1] = 0.0f;

            EqDat[i].sdm2[0] = 0.0f;
            EqDat[i].sdm2[1] = 0.0f;

            EqDat[i].sdm3[0] = 0.0f;
            EqDat[i].sdm3[1] = 0.0f;
#endif

#if defined(PTK_FX_TRANCEGLIDER)
            Glide_Step[i] = 0;
#endif

#if defined(PTK_LFO)
#if !defined(__STAND_ALONE__)
            if(reset_carriers)
            {
#endif
                LFO_CARRIER_FILTER[i] = 0.0f;
                LFO_CARRIER_VOLUME[i] = 0.0f;
                LFO_CARRIER_PANNING[i] = 0.0f;
#if !defined(__STAND_ALONE__)
            }
#endif
#endif

#if defined(PTK_FLANGER)
            FLANGER_OFFSET2[i] = float(FLANGER_OFFSET[i] - FLANGER_DELAY[i]);
            FLANGER_OFFSET1[i] = float(FLANGER_OFFSET[i] - FLANGER_DELAY[i]);  
            for(int ini2 = 0; ini2 < 16400; ini2++)
            {
                FLANGE_LEFTBUFFER[i][ini2] = 0.0f;
                FLANGE_RIGHTBUFFER[i][ini2] = 0.0f;
            }
#endif
        }

        SubCounter = 0;
        Subicounter = 0;

        lchorus_counter = MIX_RATE;
        rchorus_counter = MIX_RATE;
        lchorus_counter2 = MIX_RATE - lchorus_delay;
        rchorus_counter2 = MIX_RATE - rchorus_delay;

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
        Midi_AllNotesOff();
#endif
        // Clear all midi channels
        Clear_Midi_Channels_Pool();
#endif

        for(i = 0; i < 256; i++)
        {
            Delays_Pos_Sound_Buffer[i].Line = 0;
            Delays_Pos_Sound_Buffer[i].Pos = 0;
            Delays_Pos_Sound_Buffer[i].SamplesPerTick = 0;

#if defined(PTK_SHUFFLE)
            Delays_Pos_Sound_Buffer[i].shufflestep = 0;
#endif

        }

#if defined(PTK_SHUFFLE)
        Update_Shuffle();
#endif

        local_ramp_vol = 1.0f;
        local_curr_ramp_vol = 0.0f;
    
        local_mas_vol = 1.0f;
        local_curr_mas_vol = 0.0f;

        // Start as the last known position
        for(int spl = 0; spl < MAX_TRACKS; spl++)
        {
            CCoef[spl] = float((float) CSend[spl] / 127.0f);
            Compute_Stereo(spl);
            Fix_Stereo(spl);
        }

        Calc_Tempo();
        PosInTick = 0;

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
        PosInTick_Midi = SamplesPerTick_Midi;
#endif
#endif

        PosInTick_Delay = 0;
        Delay_Sound_Buffer = 0;
        Cur_Delay_Sound_Buffer = 0;

#if defined(PTK_SHUFFLE)
        shufflestep = 0;
        shuffleswitch = -1;
#endif

#if defined(PTK_303)
        
        tb303engine[0].reset();
        tb303engine[1].reset();

        track3031 = 255;
        track3032 = 255;
#endif

        Pattern_Line_Visual = Pattern_Line;
        Song_Position_Visual = Song_Position;

        AUDIO_ResetTimer();

#if defined(__PSP__)
        volatile int *ptr_Done_Reset = (int *) (((int) &Done_Reset) | 0x40000000);
        *ptr_Done_Reset = TRUE;
#else
        Done_Reset = TRUE;
#endif

    }
}

// ------------------------------------------------------
// Start replaying
void PTKEXPORT Ptk_Play(void)
{

#if !defined(__STAND_ALONE__)
    int i;
    int j;

    for(j = 0; j < 256; j++)
    {
        for(i = 0; i < MAX_TRACKS; i++)
        {
            Chan_History_State[j][i] = FALSE;
        }
    }
#endif

    Done_Reset = FALSE;

    local_mas_vol = 1.0f;
    local_curr_mas_vol = 0.0f;

    local_ramp_vol = 1.0f;
    local_curr_ramp_vol = 0.0f;

    Reset_Values();
    Done_Reset = FALSE;

#if !defined(__STAND_ALONE__)
    L_MaxLevel = 0;
    R_MaxLevel = 0;
#endif

// Send stop notification
#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
    if(!Song_Playing)
    {
        if(reset_carriers)
        {
            Midi_Send(0xfa, 0, 0);
        }
        else
        {
            Midi_Send(0xfb, 0, 0);
        }
    }
#endif
#endif

    Song_Playing = TRUE;

#if defined(__PSP__)
    sceKernelDcacheWritebackInvalidateAll();	
#endif
}

// ------------------------------------------------------
// Stop replaying
extern int AUDIO_Play_Flag;

void PTKEXPORT Ptk_Stop(void)
{

#if defined(__PSP__)
    // Thanks to MIPS, that machine really sucks
    volatile int *ptr_Done_Reset = (int *) (((int) &Done_Reset));
    *ptr_Done_Reset = FALSE;
    volatile float *ptr_local_ramp_vol = (float *) (((int) &local_ramp_vol));
    volatile float *ptr_local_curr_ramp_vol = (float *) (((int) &local_curr_ramp_vol));
    sceKernelDcacheWritebackInvalidateAll();
    while(*ptr_Done_Reset == FALSE && AUDIO_Play_Flag && *ptr_local_curr_ramp_vol != 0.0f)
    {
        *ptr_local_ramp_vol = 0.0f;
    }

#else

    Done_Reset = FALSE;
    while(Done_Reset == FALSE && AUDIO_Play_Flag && local_curr_ramp_vol != 0.0f)
    {
        local_ramp_vol = 0.0f;

#if defined(__MACOSX_PPC__) || defined(__LINUX__) || defined(__AROS__) || defined(__AMIGAOS4__) || defined(__MORPHOS__)
        usleep(10);
#endif
#if defined(__WIN32__)
        Sleep(10);
#endif

#if !defined(__STAND_ALONE__)
        if(local_curr_ramp_vol == 1.0f || rawrender)
        {
            break;
        }
#endif

    }
#endif

}

// ------------------------------------------------------
// Init replay variables before loading a module
void Pre_Song_Init(void)
{
    int i;


#if !defined(__STAND_ALONE__) || defined(BZR2)
    sprintf(artist, "Somebody");
    sprintf(style, "Anything Goes");
#endif

    shuffle_amount = 0;
    
    for(int ini = 0; ini < MAX_TRACKS; ini++)
    {
        for(i = 0; i < MAX_POLYPHONY; i++)
        {

#if defined(PTK_SYNTH)
            Synthesizer[ini][i].Reset();
#endif
            Chan_Active_State[ini][i] = TRUE;
        }

        Track_Volume[ini] = 1.0f;
        Track_Surround[ini] = FALSE;
        Track_Denoise[ini] = 0;

#if defined(PTK_TRACK_EQ)
        Init_Equ(&EqDat[ini]);
#endif

        Channels_Polyphony[ini] = 1;
        Channels_MultiNotes[ini] = 1;
        Channels_Effects[ini] = 1;

        Reset_Filters(ini);

#if !defined(__STAND_ALONE__)
        Chan_Midi_Prg[ini] = ini;
#endif

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
        Chan_Mute_State[ini] = FALSE;
#endif

#if defined(PTK_LFO)
        LFO_ON[ini] = 0;
        LFO_RATE[ini] = 0.0001f;
        LFO_RATE_SCALE[ini] = 1.0f;
        LFO_AMPL_FILTER[ini] = 0.0f;
        LFO_AMPL_VOLUME[ini] = 0.0f;
        LFO_AMPL_PANNING[ini] = 0.0f;
        LFO_CARRIER_FILTER[ini] = 0.0f;
        LFO_CARRIER_VOLUME[ini] = 0.0f;
        LFO_CARRIER_PANNING[ini] = 0.0f;
#endif

#if !defined(__STAND_ALONE__)
        // Default parameters usde inside the tracker
        FLANGER_AMOUNT[ini] = -0.8f;
        FLANGER_DEPHASE[ini] = 0.0174532f;
        FLANGER_ON[ini] = 0;
        FLANGER_RATE[ini] = 0.0068125f / 57.29578f;
        FLANGER_AMPL[ini] = 0.001f;
        FLANGER_GR[ini] = 0;
        FLANGER_FEEDBACK[ini] = -0.51f;
        FLANGER_DELAY[ini] = 176;
        FLANGER_OFFSET[ini] = 8192;

        TPan[ini] = Default_Pan[ini];
        old_TPan[ini] = TPan[ini];
        TCut[ini] = 126.0f;
        ICut[ini] = 0.0039062f;
        FType[ini] = 4;
        FRez[ini] = 64;
        DThreshold[ini] = 32767;
        DClamp[ini] = 32767;
        Disclap[ini] = 0;
#endif

        ramper[ini] = 0;

#if !defined(__STAND_ALONE__)
        DSend[ini] = 0;
        CSend[ini] = 0;
#endif
    }

    Reset_Chorus_Filters();
    ChorType = 4;
    ChorRez = 64;

#if defined(PTK_303)
    tb303engine[0].reset();
    tb303engine[1].reset();
#endif

    for(i = 0; i < MAX_INSTRS; i++)
    {
        Sample_Vol[i] = 1.0f;
    }

#if defined(PTK_LIMITER_MASTER) || defined(PTK_LIMITER_TRACKS)
    mas_comp_pos_rms_buffer = 0;
#endif

#if defined(PTK_LIMITER_MASTER)
    mas_comp_threshold_Master = 100.0f;
    mas_comp_ratio_Master = 0;
#endif

    Reverb_Filter_Resonance = 0.2f;
    Reverb_Damp = 1.0f;

#if defined(PTK_LIMITER_TRACKS)
    int j;

    for(j = 0; j < MAX_TRACKS; j++)
    {
        mas_comp_threshold_Track[j] = 100.0f;
        mas_comp_ratio_Track[j] = 0;
    }
#endif

#if defined(PTK_FILTER_LOHIBAND)
    for(int cutt = 0; cutt < 128; cutt++)
    {
        for(int rezz = 0; rezz < 128; rezz++)
        {
            for(int typp = 0; typp < 4; typp++)
            {
                ComputeCoefs(cutt, rezz, typp);
                coeftab[0][cutt][rezz][typp] = coef[0];      
                coeftab[1][cutt][rezz][typp] = coef[1];
                coeftab[2][cutt][rezz][typp] = coef[2];
                coeftab[3][cutt][rezz][typp] = coef[3];
                coeftab[4][cutt][rezz][typp] = coef[4];
            }
        }
    }
#endif

#if defined(PTK_INSTRUMENTS)
    Free_Samples();
#endif

}

// ------------------------------------------------------
// Set the tempo
void Calc_Tempo(void)
{
    SamplesPerTick = (int) (((60 * MIX_RATE) / Beats_Per_Min) / Ticks_Per_Beat);

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
    SamplesPerTick_Midi = (int) (((60 * MIX_RATE) / Beats_Per_Min) / 24.0f);
#endif
#endif

    SamplesPerSub = SamplesPerTick / 6;
}

// ------------------------------------------------------
// Init the replayer data
void Post_Song_Init(void)
{
    Reset_Values();
    Song_Playing_Pattern = FALSE;
}

// ------------------------------------------------------
// Record and set the visual patterns lines and song positions
void Record_Delay_Event()
{
    Cur_Delay_Sound_Buffer++;
    if(Cur_Delay_Sound_Buffer >= 512) Cur_Delay_Sound_Buffer = 0;

    // Record a complete sequence for latency calibration
#if defined(PTK_SHUFFLE)
    Delays_Pos_Sound_Buffer[Cur_Delay_Sound_Buffer].shufflestep = shufflestep;
#endif

    Delays_Pos_Sound_Buffer[Cur_Delay_Sound_Buffer].SamplesPerTick = SamplesPerTick;
    Delays_Pos_Sound_Buffer[Cur_Delay_Sound_Buffer].Line = Pattern_Line;
    Delays_Pos_Sound_Buffer[Cur_Delay_Sound_Buffer].Pos = Song_Position;
}

// ------------------------------------------------------
// Record and set the visual patterns lines and song positions
void Proc_Next_Visual_Line()
{
    PosInTick_Delay++;

#if defined(PTK_SHUFFLE)
    if(PosInTick_Delay > Delays_Pos_Sound_Buffer[Delay_Sound_Buffer].SamplesPerTick +
                         Delays_Pos_Sound_Buffer[Delay_Sound_Buffer].shufflestep)
    {
#else
    if(PosInTick_Delay > Delays_Pos_Sound_Buffer[Delay_Sound_Buffer].SamplesPerTick)
    {
#endif
        PosInTick_Delay = 0;

        Delay_Sound_Buffer++;
        if(Delay_Sound_Buffer >= 512) Delay_Sound_Buffer = 0;
        Pattern_Line_Visual = Delays_Pos_Sound_Buffer[Delay_Sound_Buffer].Line;
        Song_Position_Visual = Delays_Pos_Sound_Buffer[Delay_Sound_Buffer].Pos;

#if !defined(__WINAMP__)
#if !defined(__STAND_ALONE__) 
        gui_action_metronome = GUI_CMD_FLASH_METRONOME_OFF;
        if(metronome_latency)
        {
            gui_action_metronome = GUI_CMD_FLASH_METRONOME_ON;
            metronome_latency = FALSE;
        }
#endif
#endif

    }
}

// ------------------------------------------------------
// Main Player Routine
void Sp_Player(void)
{
#if defined(PTK_INSTRUMENTS)
    unsigned int res_dec;
#endif

#if defined(PTK_303)
    float Signal_303 = 0.0f;
    int fired_303_1;
    int fired_303_2;
    int trigger_note_off;
#endif

    char gotsome;
    int c;
    int i;
    int j;
    int ct;

#if defined(PTK_SYNTH) || defined(PTK_INSTRUMENTS)
    float dest_volume;
#endif

    int toffset;
    int free_sub_channel;

#if defined(PTK_FX_SETVOLUME)
    int no_fx3;
#endif

    int Glide_Synth[MAX_POLYPHONY];

#if defined(PTK_TRACKFILTERS)
    float realcut;
#endif

#if defined(PTK_COMPRESSOR)
    delay_left_final = 0.0f;
    delay_right_final = 0.0f;
#endif

    if(Song_Playing)
    {
        if(PosInTick == 0)
        {

#if defined(PTK_FX_TICK0)
            Do_Effects_Tick_0();
#endif
            Subicounter = 0;

#if defined(PTK_FX_PATTERNBREAK)
            Patbreak_Line = 255;
#endif

#if defined(PTK_303)
            fired_303_1 = FALSE;
            fired_303_2 = FALSE;

            for(ct = 0; ct < Song_Tracks; ct++)
            {
                int efactor = Get_Pattern_Offset(pSequence[Song_Position], ct, Pattern_Line);
                
                // Was a note off (always available even if channels are turned off)
                trigger_note_off = FALSE;
                for(i = 0; i < Channels_MultiNotes[ct]; i++)
                {
                    pl_note[i] = *(RawPatterns + efactor + PATTERN_NOTE1 + (i * 2));
                    if(pl_note[i] == 120)
                    {
                        trigger_note_off = TRUE;
                    }
                }

                // Store the effects
                for(i = 0; i < Channels_Effects[ct]; i++)
                {
                    pl_eff_row[i] = *(RawPatterns + efactor + PATTERN_FX + (i * 2));
                    pl_dat_row[i] = *(RawPatterns + efactor + PATTERN_FXDATA + (i * 2));

#if !defined(__STAND_ALONE__)
                    // Check if the user is recording 303 effects
                    // In that case we don't read the row data
                    if(!sr_isrecording)
#endif
                    {
                        live303(pl_eff_row[i], pl_dat_row[i]);
                    }

                    // 303 are always available
                    // since they aren't "really" bounded to any track
                    if(pl_eff_row[i] == 0x31)
                    {
                        track3031 = ct;
                        Fire303(pl_dat_row[i], 0);
                        fired_303_1 = TRUE;
                    }
                    if(pl_eff_row[i] == 0x32)
                    {
                        track3032 = ct;
                        Fire303(pl_dat_row[i], 1);
                        fired_303_2 = TRUE;
                   }
                }

#if defined(PTK_303)
                // There was a note off on any notes slot and no 303 was fired,
                // see if a 303 is running on that track
                if(trigger_note_off && (!fired_303_1 || !fired_303_2))
                {
                    Note_Off_303(ct);
                }
#endif

            }

            Go303();

#endif

            for(ct = 0; ct < Song_Tracks; ct++)
            {
                int efactor = Get_Pattern_Offset(pSequence[Song_Position], ct, Pattern_Line);
                
                // Store the notes & instruments numbers
                for(i = 0; i < Channels_MultiNotes[ct]; i++)
                {
                    pl_note[i] = *(RawPatterns + efactor + PATTERN_NOTE1 + (i * 2));
                    pl_sample[i] = *(RawPatterns + efactor + PATTERN_INSTR1 + (i * 2));
                    if(pl_sample[i] != 255)
                    {
                        old_pl_sample[ct][i] = pl_sample[i];
                    }
                }

                pl_vol_row = *(RawPatterns + efactor + PATTERN_VOLUME);
                pl_pan_row = *(RawPatterns + efactor + PATTERN_PANNING);
                
                for(i = 0; i < Channels_Effects[ct]; i++)
                {
                    pl_eff_row[i] = *(RawPatterns + efactor + PATTERN_FX + (i * 2));
                    pl_dat_row[i] = *(RawPatterns + efactor + PATTERN_FXDATA + (i * 2));
                }

#if defined(PTK_VOLUME_COLUMN) || defined(PTK_FX_SETVOLUME)
                for(i = 0; i < Channels_Effects[ct]; i++)
                {
                    if(pl_vol_row <= 64 || pl_eff_row[i] == 3)
#endif
                    {
                        sp_Tvol_Mod[ct] = 1.0f;
#if defined(PTK_VOLUME_COLUMN) || defined(PTK_FX_SETVOLUME)
                        break;
#endif
                    }

#if defined(PTK_VOLUME_COLUMN) || defined(PTK_FX_SETVOLUME)
                }
#endif

#if defined(PTK_VOLUME_COLUMN)
                if(pl_vol_row <= 64)
                {
                    sp_Tvol_Mod[ct] *= (float) pl_vol_row * 0.015625f;
                }
#endif

#if defined(PTK_FX_SETVOLUME)
                for(i = 0; i < Channels_Effects[ct]; i++)
                {
                    // Modulated by effect 3
                    if(pl_eff_row[i] == 3)
                    {
                        sp_Tvol_Mod[ct] *= (float) pl_dat_row[i] * 0.0039062f;
                    }
                }
#endif

                if(pl_pan_row <= 128)
                {
                    TPan[ct] = (float) pl_pan_row * 0.0078125f; 

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_SET_PANNING;
                    }
#endif

                }

                // Don't check those fx if the channel isn't active
                if(Chan_Active_State[Song_Position][ct])
                {

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
                    // No controller command for inactive tracks
                    for(i = 0; i < Channels_Effects[ct]; i++)
                    {
                        if((pl_pan_row == 0x90 && pl_eff_row[i] < 128) && c_midiout != -1)
                        {
                            Midi_Send(0xb0 + Chan_Midi_Prg[ct], pl_eff_row[i], pl_dat_row[i]);
                        }

                        if((pl_eff_row[i] == 0x80 && pl_dat_row[i] < 128) && c_midiout != -1)
                        {
                            Midi_Send(0xb0 + Chan_Midi_Prg[ct], 0, pl_dat_row[i]);
                        }
                    }
#endif
#endif

#if defined(PTK_FX_PATTERNBREAK)
                    for(i = 0; i < Channels_Effects[ct]; i++)
                    {
                        if(pl_eff_row[i] == 0xd && pl_dat_row[i] < MAX_ROWS)
                        {
                            Patbreak_Line = pl_dat_row[i];
                        }
                    }
#endif

#if defined(PTK_FX_POSJUMP)
                    for(i = 0; i < Channels_Effects[ct]; i++)
                    {
                        if(pl_eff_row[i] == 0x1f)
                        {
                            if(Patbreak_Line >= MAX_ROWS) Patbreak_Line = 0;
                            PosJump = pl_dat_row[i];
                        }
                    }
#endif
                }

                // Those 2 will only be used when triggering new notes
                toffset = 0;
                glide[ct] = 0;
                for(i = 0; i < Channels_Effects[ct]; i++)
                {
                    if(pl_eff_row[i] == 0x9)
                    {
                        toffset = pl_dat_row[i];
                    }
                    else
                    {
                        if(pl_eff_row[i] == 0x5)
                        {
                            glide[ct] = 1;
                        }
                    }
                }

                // Clear glide infos
                for(i = 0; i < Channels_MultiNotes[ct]; i++)
                {
                    Glide_Synth[i] = -1;
                }

                // Send notes off to the synth & midi
                // before triggering any new note
                for(i = 0; i < Channels_MultiNotes[ct]; i++)
                {
                    if(pl_note[i] < 120 && Note_Sub_Channels[ct][i] != -1)
                    {
                        j = Reserved_Sub_Channels[ct][i];

#if defined(PTK_INSTRUMENTS)
                        // Get the virtual channel it was playing on and remove it
                        if(sp_Stage[ct][j] == PLAYING_SAMPLE)
                        {
                            sp_Stage[ct][j] = PLAYING_SAMPLE_NOTEOFF;
                        }
#endif
#if defined(PTK_SYNTH)
                        if(!glide)
                        {
                            Synthesizer[ct][j].Note_Off();
                            sp_Stage[ct][j] = PLAYING_SAMPLE_NOTEOFF;
                        }
                        else
                        {
                            Glide_Synth[i] = j;
                        }
#endif
#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
                        if(Midi_Current_Notes[Chan_Midi_Prg[ct]][j])
                        {
                            Midi_Note_Off(ct, Midi_Current_Notes[Chan_Midi_Prg[ct]][j]);
                            Midi_Current_Notes[Chan_Midi_Prg[ct]][j] = 0;
                        }
#endif
#endif
                    }
                }

                // New note
                for(i = 0; i < Channels_MultiNotes[ct]; i++)
                {
                    if(Chan_Active_State[Song_Position][ct])
                    {
                        // A note or no note with an instrument
                        if(pl_note[i] < 120 || (pl_note[i] > 120 && pl_sample[i] != 255))
                        {
                            if(!glide)
                            {
                                free_sub_channel = Get_Free_Sub_Channel(ct, Channels_Polyphony[ct]);
                            }
                            else
                            {
                                free_sub_channel = Glide_Synth[i];
                            }
                            if(free_sub_channel == -1) free_sub_channel = i;

                            // Mark it as playing
                            Note_Sub_Channels[ct][i] = i;
                            Reserved_Sub_Channels[ct][i] = free_sub_channel;

#if defined(PTK_VOLUME_COLUMN)
                            // Need to set the instrument volume
                            if(pl_vol_row > 64)
#endif
                            {

#if defined(PTK_FX_SETVOLUME)
                                // Look for a 03 fx
                                no_fx3 = FALSE;
                                for(j = 0; j < Channels_Effects[ct]; j++)
                                {
                                    if(pl_eff_row[j] == 3)
                                    {
                                        no_fx3 = TRUE;
                                        break;
                                    }
                                }

                                // Found a 03 fx ?
                                if(!no_fx3)
#endif
                                {
                                    // Nope: set default ratio
                                    if(pl_sample[i] != 255)
                                    {
                                        sp_Tvol_Mod[ct] = 1.0f;
                                    }
                                }
                            }

                            // Start to play it with the specified volume
                            Schedule_Instrument(ct,
                                                free_sub_channel,           // From Channels_Polyphony not Channels_MultiNotes
                                                pl_note[i],
                                                pl_sample[i],
                                                toffset,
                                                glide[ct],
                                                FALSE, i + 1,
                                                1.0f,
                                                Song_Position,
                                                Pattern_Line);
                        }
                    }
                }

                // Was a note off (always available even if channels are turned off)
                for(i = 0; i < Channels_MultiNotes[ct]; i++)
                {
                    if(pl_note[i] == 120)
                    {
                        if(Note_Sub_Channels[ct][i] != -1)
                        {
                            j = Reserved_Sub_Channels[ct][i];

#if defined(PTK_INSTRUMENTS)
                            if(sp_Stage[ct][j] == PLAYING_SAMPLE)
                            {
                                sp_Stage[ct][j] = PLAYING_SAMPLE_NOTEOFF;
                            }
#endif

#if defined(PTK_SYNTH)
                            Synthesizer[ct][j].Note_Off();
                            sp_Stage[ct][j] = PLAYING_SAMPLE_NOTEOFF;
#endif
                            Reserved_Sub_Channels[ct][i] = -1;
                            Note_Sub_Channels[ct][i] = -1;

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
                            if(Midi_Current_Notes[Chan_Midi_Prg[ct]][i])
                            {
                                Midi_Note_Off(ct, Midi_Current_Notes[Chan_Midi_Prg[ct]][i]);
                                Midi_Current_Notes[Chan_Midi_Prg[ct]][i] = 0;
                            }
#endif
#endif
                        }


                    }
                }

#if defined(PTK_FX_PATTERNLOOP)
                if(Chan_Active_State[Song_Position][ct])
                {
                    Do_Pattern_Loop(ct);
                }
#endif


            } // Channels loop

        } // Pos in tick == 0

        // ---------------------------------------

        if(!SubCounter)
        {
            Do_Effects_Ticks_X();
        }

        // Record at tick 0 but wait for some effects (like the speed change)
        // to be processed (SubCounter is resetted at the same time as PosInTick)
        if(PosInTick == 0)
        {
            Record_Delay_Event();
        }

        SubCounter++;

        if(SubCounter > SamplesPerSub - 1)
        {
            SubCounter = 0;
            Subicounter++;
        }

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
        PosInTick_Midi++;
        if(PosInTick_Midi > SamplesPerTick_Midi)
        {
            // Send midi clock notification
            Midi_Send(0xf8, 0, 0);
            PosInTick_Midi = 0;
        }
#endif
#endif

        PosInTick++;

#if defined(PTK_SHUFFLE)
        if(PosInTick > (SamplesPerTick + shufflestep))
        {
            shuffleswitch = -shuffleswitch;

            Update_Shuffle();
#else
        if(PosInTick > (SamplesPerTick))
        {
#endif
            SubCounter = 0;
            PosInTick = 0;

            // Time to trigger the metronome
#if !defined(__WINAMP__)
#if !defined(__STAND_ALONE__) 
            if(metronome_magnify && is_recording_2)
            {
                metronome_rows_counter++;
                if(metronome_rows_counter >= metronome_magnify)
                {
                    metronome_rows_counter = 0;
                    metronome_internal_counter_int = 0;
                    trigger_metronome = TRUE;
                }
            }
#endif
#endif

            // ------------------------------
            // Pattern movements

#if defined(PTK_FX_PATTERNLOOP)
            // Check if we're in a loop
            if(repeat_loop_counter_in)
            {
                Pattern_Line -= repeat_loop_pos;
                if(Pattern_Line < 0) Pattern_Line = 0;
                repeat_loop_counter_in = 0;
            }
            else
#endif
            {

#if defined(PTK_FX_PATTERNBREAK)
                if(Patbreak_Line > 127)
                {
                    Pattern_Line++;
                }
                else
                {
                    // Break the pattern
#if !defined(__STAND_ALONE__)
                    if(is_recording_2)
                    {
                        Next_Line_Pattern_Auto(&Song_Position, Patbreak_Line, &Pattern_Line);
                    }
                    else
#endif
                    {

#if !defined(__STAND_ALONE__)
                        if(!play_pattern)            // Playing a pattern or not ?
#endif
                        {

                            // Position jump and pattern break can be combined
#if defined(PTK_FX_POSJUMP)
                            if(PosJump >= 0)
                            {
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
                                if(PosJump <= Song_Position)
                                {
                                    done = TRUE;
                                }
#endif
                                Song_Position = PosJump;
                                PosJump = -1;
                            }
                            else
                            {
                                Song_Position++;
                            }
#else
                            Song_Position++;
#endif
                        }
                    }

                    // Pattern break specified line
                    Pattern_Line = Patbreak_Line;

#if !defined(__STAND_ALONE__)
                    if(!is_recording_2)
#endif
                    {
                        if(Song_Position >= Song_Length)
                        {
                            Song_Position = 0;
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
                            done = TRUE;
#endif
                        }
                    }

#if !defined(__STAND_ALONE__)
                    for(i = 0; i < MAX_TRACKS; i++)
                    {
                        Chan_History_State[Song_Position][i] = FALSE;
                    }
#endif

#if defined(PTK_FX_PATTERNLOOP)
                    // Loops between patterns wouldn't make sense
                    repeat_loop_pos = 0;
                    repeat_loop_counter = 0;
                    repeat_loop_counter_in = 0;
#endif
                }
#else
                Pattern_Line++;
#endif  // PTK_FX_PATTERNBREAK

                // Normal end of pattern
                if(Pattern_Line == patternLines[pSequence[Song_Position]])
                {

#if !defined(__STAND_ALONE__)
                    if(is_recording_2)
                    {
                        Next_Line_Pattern_Auto(&Song_Position, patternLines[pSequence[Song_Position]], &Pattern_Line);
                    }
                    else
#endif
                    {

#if !defined(__STAND_ALONE__)
                        if(!play_pattern)
#endif
                        {

#if defined(PTK_FX_POSJUMP)
                            if(PosJump > 0)
                            {

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
                                if(PosJump <= Song_Position)
                                {
                                    done = TRUE;
                                }
#endif
                                Song_Position = PosJump;
                                PosJump = -1;
                            }
                            else
                            {
                                Song_Position++;
                            }
#else
                            Song_Position++;
#endif
                        }
                    }

                    Pattern_Line = 0;

#if !defined(__STAND_ALONE__)
                    if(!is_recording_2)
#endif
                    {
                        if(Song_Position >= Song_Length)
                        {
                            Song_Position = 0;
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
                            done = TRUE;
#endif
                        }
                    }

#if !defined(__STAND_ALONE__)
                    for(i = 0; i < MAX_TRACKS; i++)
                    {
                        Chan_History_State[Song_Position][i] = FALSE;
                    }
#endif

#if defined(PTK_FX_PATTERNLOOP)
                    repeat_loop_pos = 0;
				    repeat_loop_counter = 0;
                    repeat_loop_counter_in = 0;
#endif
                }
            }
        }

        // Replay the recorded song sequence with the sound card latency delay
        if(Song_Playing_Pattern)
        {
            Proc_Next_Visual_Line();
        }
    }

    // -------------------------------------------
    // Process the data, this is the huge loop
    // -------------------------------------------
    for(c = 0; c < Song_Tracks; c++)
    {

#if defined(PTK_FX_VIBRATO)
        Vibcounter[c]++;
#endif

        grown = FALSE;
        gotsome = FALSE;

        All_Signal_L = 0;
        All_Signal_R = 0;

        for(i = 0; i < Channels_Polyphony[c]; i++)
        {
            Curr_Signal_L[i] = 0.0f;
            Curr_Signal_R[i] = 0.0f;

#if defined(PTK_INSTRUMENTS) || defined(PTK_SYNTH)

            // A new note has been scheduled ?
            if(Cut_Stage[c][i])
            {
                // Note Stop
                if(sp_Cvol[c][i] <= 0.0f &&
                   sp_Cvol_Synth[c][i] <= 0.0f)
                {
                    Cut_Stage[c][i] = FALSE;
                    Play_Instrument(c, i);
                }
            }
            else
            {
                // Continue an instrument
                if(Glide_Stage[c][i])
                {
                    Glide_Stage[c][i] = FALSE;
                    Play_Instrument(c, i);
                }
            }

            // ----------------------------------
            // Handle samples
            if(sp_Stage[c][i] == PLAYING_SAMPLE ||
               sp_Stage[c][i] == PLAYING_SAMPLE_NOTEOFF ||
               Cut_Stage[c][i])
            {

#if defined(PTK_SYNTH)

                // Synth bypassing
                if(!Synth_Was[c][i]) goto ByPass_Wav;

                if((Synthesizer[c][i].Data.OSC_1_WAVEFORM != WAVEFORM_WAV &&
                    Synthesizer[c][i].Data.OSC_2_WAVEFORM != WAVEFORM_WAV))
                {
ByPass_Wav:
#endif
                    if(sp_Stage[c][i] == PLAYING_SAMPLE_NOTEOFF && sp_Cvol[c][i] <= 0.02f)
                    {
                        sp_Stage[c][i] = PLAYING_NOSAMPLE;
                    }
                    if(Cut_Stage[c][i])
                    {
                        // Volume ramping
                        if(sp_Cvol[c][i] > 0.0f)
                        {
                            if(sp_Cvol_Ramp_Dest[c][i] != 0.0f)
                            {
                                sp_Cvol_Ramp[c][i] = (sp_Cvol[c][i] - 0.0f) / 48.0f;
                                sp_Cvol_Ramp_Dest[c][i] = 0.0f;
                            }
                            sp_Cvol[c][i] -= sp_Cvol_Ramp[c][i];
                            if(sp_Cvol[c][i] <= 0.0f)
                            {
                                sp_Cvol[c][i] = 0.0f;
                                sp_Cvol_Ramp_Dest[c][i] = 10.0f;
                            }
                        }
                    }
                    else
                    {
                        if(sp_Stage[c][i] == PLAYING_SAMPLE_NOTEOFF)
                        {
                            // Note Stop
                            sp_Tvol[c][i] = 0.0f;
                            sp_Cvol_Ramp_Dest[c][i] = 10.0f;
                        }

                        dest_volume = sp_Tvol[c][i] * sp_Tvol_Mod[c];
                        // Volume ramping
                        if(sp_Cvol[c][i] != dest_volume)
                        {
                            if(sp_Cvol[c][i] > dest_volume)
                            {
                                if(sp_Cvol_Ramp_Dest[c][i] != dest_volume)
                                {
                                    sp_Cvol_Ramp[c][i] = (sp_Cvol[c][i] - dest_volume) / 48.0f;
                                    sp_Cvol_Ramp_Dest[c][i] = dest_volume;
                                }
                                sp_Cvol[c][i] -= sp_Cvol_Ramp[c][i];
                                if(sp_Cvol[c][i] <= dest_volume)
                                {
                                    sp_Cvol[c][i] = dest_volume;
                                    sp_Cvol_Ramp_Dest[c][i] = 10.0f;
                                }
                            }
                            else
                            {
                                if(sp_Cvol_Ramp_Dest[c][i] != dest_volume)
                                {
                                    sp_Cvol_Ramp[c][i] = (dest_volume - sp_Cvol[c][i]) / 48.0f;
                                    sp_Cvol_Ramp_Dest[c][i] = dest_volume;
                                }
                                sp_Cvol[c][i] += sp_Cvol_Ramp[c][i];
                                if(sp_Cvol[c][i] >= dest_volume)
                                {
                                    sp_Cvol[c][i] = dest_volume;
                                    sp_Cvol_Ramp_Dest[c][i] = 10.0f;
                                }
                            }
                            if(sp_Cvol[c][i] > 1.0f)
                            {
                                sp_Cvol[c][i] = 1.0f;
                            }
                            if(sp_Cvol[c][i] < 0.0f)
                            {
                                sp_Cvol[c][i] = 0.0f;
                            }
                        }
                        if(sp_Stage[c][i] == PLAYING_NOSAMPLE)
                        {
                            if(sp_Cvol[c][i] < 0.02f)
                            {
                                sp_Cvol[c][i] = 0.0f;
                                sp_Position[c][i].flt_pos = 0;
                                sp_Position[c][i].int_pos = 0;
                            }
                        }

                    }

                    res_dec = sp_Position[c][i].int_pos;

                    // We had some signal (on any channel)
                    gotsome = TRUE;

                    Set_Carrier_Boundaries(sp_Position[c][i].int_pos,
                                           &Current_Pointer,
                                           Player_LT[c][i],
                                           Player_LW[c][i],
                                           Player_NS[c][i],
                                           Player_LE[c][i],
                                           Player_LS[c][i]
                                          );

                    // Get sample data
                    if(Player_WL[c][i])
                    {
                        Curr_Signal_L[i] = (float) *(Player_WL[c][i] + Current_Pointer) * sp_Cvol[c][i] * Player_Ampli[c][i];
                        // Is it stereo sample ?
                        if(Player_SC[c][i] == 2)
                        {
                            grown = TRUE;
                            Curr_Signal_R[i] = (float) *(Player_WR[c][i] + Current_Pointer) * sp_Cvol[c][i] * Player_Ampli[c][i];
                        }
                    }

                    // End of Interpolation algo
                    // check the carrier against looping infos
                    if(Player_LW[c][i] == SMP_LOOPING_BACKWARD)
                    {
                        if((int) sp_Position[c][i].int_pos > 0)
                        {
                            sp_Position[c][i].flt_pos -= Vstep1[c][i];
                            sp_Position[c][i].int_pos = (int32) sp_Position[c][i].flt_pos;
                        }
                    }
                    else
                    {
                        sp_Position[c][i].flt_pos += Vstep1[c][i];
                        sp_Position[c][i].int_pos = (int32) sp_Position[c][i].flt_pos;
                    }

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                    switch(Player_LT[c][i])
                    {
#if defined(PTK_LOOP_FORWARD)
                        case SMP_LOOP_FORWARD:
                            if(Player_LW[c][i] == SMP_LOOPING_FORWARD)
                            {
                                if(sp_Position[c][i].int_pos >= (int) Player_LE[c][i])
                                {
                                    sp_Position[c][i].int_pos = Player_LS[c][i];
                                    sp_Position[c][i].flt_pos = (float) Player_LS[c][i];
                                }
                            }
                            else
                            {
                                if((int) sp_Position[c][i].int_pos <= (int) Player_LS[c][i])
                                {
                                    sp_Position[c][i].int_pos = Player_LE[c][i];
                                    sp_Position[c][i].flt_pos = (float) Player_LE[c][i];
                                }
                            }
                            break;
#endif
#if defined(PTK_LOOP_PINGPONG)
                        case SMP_LOOP_PINGPONG:
                            if(Player_LW[c][i] == SMP_LOOPING_FORWARD)
                            {
                                if(sp_Position[c][i].int_pos >= (int) Player_LE[c][i])
                                {
                                    sp_Position[c][i].int_pos = Player_LE[c][i];
                                    sp_Position[c][i].flt_pos = (float) Player_LE[c][i];
                                    Player_LW[c][i] = SMP_LOOPING_BACKWARD;
                                }
                            }
                            else
                            {
                                if((int) sp_Position[c][i].int_pos <= (int) Player_LS[c][i])
                                {
                                    Player_LW[c][i] = SMP_LOOPING_FORWARD;
                                    sp_Position[c][i].int_pos = Player_LS[c][i];
                                    sp_Position[c][i].flt_pos = (float) Player_LS[c][i];
                                }
                            }
                            break;
#endif
                        case SMP_LOOP_NONE:
#endif // defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                            if(Player_LW[c][i] == SMP_LOOPING_FORWARD)
                            {
                                if(sp_Position[c][i].int_pos >= (int) Player_NS[c][i])
                                {
                                    sp_Position[c][i].int_pos = Player_NS[c][i];
                                    sp_Position[c][i].flt_pos = (float) Player_NS[c][i];
                                    sp_Stage[c][i] = PLAYING_NOSAMPLE;
                                }
                            }
                            else
                            {
                                if((int) sp_Position[c][i].int_pos <= 0)
                                {
                                    sp_Position[c][i].int_pos = 0;
                                    sp_Position[c][i].flt_pos = 0;
                                    sp_Stage[c][i] = PLAYING_NOSAMPLE;
                                }
                            }
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
                            break;
                    }
#endif

#if defined(PTK_SYNTH)
                }
#endif

            }
#endif // defined(PTK_INSTRUMENTS) || defined(PTK_SYNTH)

#if defined(PTK_SYNTH)
            // --------------------------------------
            // Handle the synth part
            if(Synthesizer[c][i].ENV_1_STAGE ||
               Synthesizer[c][i].ENV_2_STAGE ||
               Cut_Stage[c][i] && Synth_Really_Used[c][i]
              )
            {
                if(Synthesizer[c][i].ENV_1_STAGE == SYNTH_RELEASE && 
                   Synthesizer[c][i].ENV_2_STAGE == SYNTH_RELEASE && 
                   sp_Cvol_Synth[c][i] <= 0.02f)
                {
                    Synthesizer[c][i].ENV_1_STAGE = PLAYING_NOSAMPLE;
                    Synthesizer[c][i].ENV_2_STAGE = PLAYING_NOSAMPLE;
                    sp_Stage[c][i] = PLAYING_NOSAMPLE;
                    sp_Stage2[c][i] = PLAYING_NOSAMPLE;
                    sp_Stage3[c][i] = PLAYING_NOSAMPLE;
                }
                if(Cut_Stage[c][i])
                {
                    // Volume ramping
                    if(sp_Cvol_Synth[c][i] > 0.0f)
                    {
                        if(sp_Cvol_Synth_Ramp_Dest[c][i] != 0.0f)
                        {
                            sp_Cvol_Synth_Ramp[c][i] = (sp_Cvol_Synth[c][i] - 0.0f) / 48.0f;
                            sp_Cvol_Synth_Ramp_Dest[c][i] = 0.0f;
                        }
                        sp_Cvol_Synth[c][i] -= sp_Cvol_Synth_Ramp[c][i];
                        if(sp_Cvol_Synth[c][i] < 0.0f)
                        {
                            sp_Cvol_Synth[c][i] = 0.0f;
                        }
                    }
                }
                else
                {
                    dest_volume = sp_Tvol_Synth[c][i] * sp_Tvol_Mod[c];
                    // Perform the volume ramping now if it hasn't been done before
                    if(sp_Cvol_Synth[c][i] != dest_volume)
                    {
                        if(sp_Cvol_Synth[c][i] > dest_volume)
                        {
                            if(sp_Cvol_Synth_Ramp_Dest[c][i] != dest_volume)
                            {
                                sp_Cvol_Synth_Ramp[c][i] = (sp_Cvol_Synth[c][i] - dest_volume) / 48.0f;
                                sp_Cvol_Synth_Ramp_Dest[c][i] = dest_volume;
                            }
                            sp_Cvol_Synth[c][i] -= sp_Cvol_Synth_Ramp[c][i];
                            if(sp_Cvol_Synth[c][i] < dest_volume)
                            {
                                sp_Cvol_Synth[c][i] = dest_volume;
                            }
                        }
                        else
                        {
                            if(sp_Cvol_Synth_Ramp_Dest[c][i] != dest_volume)
                            {
                                sp_Cvol_Synth_Ramp[c][i] = (dest_volume - sp_Cvol_Synth[c][i]) / 48.0f;
                                sp_Cvol_Synth_Ramp_Dest[c][i] = dest_volume;
                            }
                            sp_Cvol_Synth[c][i] += sp_Cvol_Synth_Ramp[c][i];
                            if(sp_Cvol_Synth[c][i] > dest_volume)
                            {
                                sp_Cvol_Synth[c][i] = dest_volume;
                            }
                        }
                        if(sp_Cvol_Synth[c][i] > 1.0f)
                        {
                            sp_Cvol_Synth[c][i] = 1.0f;
                        }
                        if(sp_Cvol_Synth[c][i] < 0.0f)
                        {
                            sp_Cvol_Synth[c][i] = 0.0f;
                        }
                    }
                }

                Curr_Signal_L[i] += Synthesizer[c][i].GetSample(Player_WL[c][i],
                                                                Player_WR[c][i],
                                                                Player_SC[c][i],
                                                                Player_LT[c][i],
                                                                Player_LT[c][i] > SMP_LOOP_NONE ? Player_LE[c][i]: Player_NS[c][i],
                                                                Player_LT[c][i] > SMP_LOOP_NONE ? Player_LL[c][i]: 0,
                                                                &Curr_Signal_R[i],
                                                                sp_Cvol_Synth[c][i],
                                                                &sp_Stage2[c][i],
                                                                &sp_Stage3[c][i],
                                                                &sp_Position_osc_1[c][i],
                                                                &sp_Position_osc_2[c][i],
#if defined(PTK_SYNTH_OSC_3)
                                                                &sp_Position_osc_3[c][i],
#endif
                                                                Vstep1[c][i],
                                                                Player_Ampli[c][i]
                                                               );

                if((Synthesizer[c][i].Data.OSC_1_WAVEFORM == WAVEFORM_WAV ||
                    Synthesizer[c][i].Data.OSC_2_WAVEFORM == WAVEFORM_WAV
                   )
                  )
                {
                    // It was a stereo signal
                    if(Player_SC[c][i] == 2) grown = TRUE;
                }

                gotsome = TRUE;
            }
#endif // PTK_SYNTH

            // Gather the signals of all the sub channels

            if(gotsome)
            {

#if DENORMAL
                Curr_Signal_L[i] = denormal(Curr_Signal_L[i]);
                if(grown)
                {
                    Curr_Signal_R[i] = denormal(Curr_Signal_R[i]);
                }
#endif
                All_Signal_L += Curr_Signal_L[i];
                if(grown)
                {
                    All_Signal_R += Curr_Signal_R[i];
                }
            }
        } // Channels_Polyphony[c]

#if defined(PTK_303)
        if(track3031 == c && Chan_Active_State[Song_Position][c])
        {
            Signal_303 = tb303engine[0].tbGetSample(&tb303[0]);
            All_Signal_L += Signal_303;
            if(grown)
            {
                All_Signal_R += Signal_303;
            }
            gotsome = TRUE;
        }
        if(track3032 == c && Chan_Active_State[Song_Position][c])
        {
            Signal_303 = tb303engine[1].tbGetSample(&tb303[1]);
            All_Signal_L += Signal_303;
            if(grown)
            {
                All_Signal_R += Signal_303;
            }
            gotsome = TRUE;
        }
#endif

        // Send a note off if the channel is being turned off
        if(!Chan_Active_State[Song_Position][c]

#if !defined(__STAND_ALONE__)
           && !Chan_History_State[Song_Position][c]
#endif

          )
        {

#if !defined(__STAND_ALONE__)
            Chan_History_State[Song_Position][c] = TRUE;
#endif

            // We send a note off to all sub channels
            for(i = 0; i < Channels_Polyphony[c]; i++)
            {

#if defined(PTK_INSTRUMENTS)
                if(sp_Stage[c][i] == PLAYING_SAMPLE)
                {
                    sp_Stage[c][i] = PLAYING_SAMPLE_NOTEOFF;
                    sp_Cvol_Ramp_Dest[c][i] = 10.0f;
                }
#endif

#if defined(PTK_SYNTH)
                Synthesizer[c][i].Note_Off();
#endif
            }

#if defined(PTK_303)
            Note_Off_303(c);
#endif

#if !defined(__STAND_ALONE__)
    #if !defined(__NO_MIDI__)
            Midi_Note_Off(c, -1);
    #endif
#endif

        }

        // A rather clumsy cross fading to avoid the most outrageous clicks
        // (i also tried with splines but didn't hear any difference)
        if(New_Instrument[c] && Channels_Polyphony[c] == 1)
        {
            All_Signal_L = (All_Signal_L * (1.0f - Segue_Volume[c])) + (Segue_SamplesL[c] * Segue_Volume[c]);
            if(grown)
            {
                All_Signal_R = (All_Signal_R * (1.0f - Segue_Volume[c])) + (Segue_SamplesR[c] * Segue_Volume[c]);
            }
            Pos_Segue[c]++;
            Segue_Volume[c] -= 1.0f / 127.0f;
            if(Pos_Segue[c] >= 128)
            {
                New_Instrument[c] = FALSE;
            }
        }
        else
        {
            // Store the transition
            Segue_SamplesL[c] = All_Signal_L;
            if(grown)
            {
                Segue_SamplesR[c] = All_Signal_R;
            }

        }

#if DENORMAL
        All_Signal_L = denormal(All_Signal_L);
        if(grown)
        {
            All_Signal_R = denormal(All_Signal_R);
        }
#endif

        // -----------------------------------------------

#if defined(PTK_TRACKFILTERS)
        if(FType[c] != 4)
        {   // Track filter activated
            float dfi = TCut[c] - CCut[c];

            if(dfi < -1.0f || dfi > 1.0f)
            {
                CCut[c] += dfi * ICut[c];
            }
            realcut = Apply_Lfo_To_Filter(CCut[c] - ramper[c], c);
            ramper[c] += Player_FD[c] * realcut * 0.015625f;
            gco = (int) realcut;
        }
#endif

        if(gotsome)
        {

#if defined(PTK_TRACKFILTERS)
            if(FType[c] != 4)
            {   // Track filter activated

#if defined(PTK_FILTER_LOHIBAND)
                if(FType[c] < 4)
                {
                    coef[0] = coeftab[0][gco][FRez[c]][FType[c]];
                    coef[1] = coeftab[1][gco][FRez[c]][FType[c]];
                    coef[2] = coeftab[2][gco][FRez[c]][FType[c]];
                    coef[3] = coeftab[3][gco][FRez[c]][FType[c]];
                    coef[4] = coeftab[4][gco][FRez[c]][FType[c]];

                    All_Signal_L = Filter(0, All_Signal_L + 1.0f, c);
                    if(grown)
                    {
                        All_Signal_R = Filter(1, All_Signal_R + 1.0f, c);
                    }
                }
                else
#endif
                {
                    switch(FType[c])
                    {

#if defined(PTK_FILTER_LO24)
                        case 5:
                            All_Signal_L = filter2p(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filter2p(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                            }
                            break;
#endif

#if defined(PTK_FILTER_LO48)
                        case 6:
                            All_Signal_L = filter2p(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            All_Signal_L = filter2p24d(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filter2p(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                                All_Signal_R = filter2p24d(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                            }
                            break;
#endif

#if defined(PTK_FILTER_LP24)
                        case 7:
                            All_Signal_L = filter2p(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filter2p24d(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                            }
                            break;
#endif

#if defined(PTK_FILTER_AMODM)
                        case 8:
                            All_Signal_L = filterRingMod(0, c, All_Signal_L, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filterRingMod(1, c, All_Signal_R, realcut, (float) FRez[c]);
                            }
                            break;
#endif

#if defined(PTK_FILTER_AMODS)
                        case 9:
                            if(grown)
                            {
                                All_Signal_L = filterRingMod(0, c, All_Signal_L, realcut, (float) FRez[c]);
                                All_Signal_R = filterRingModStereo(0, c, All_Signal_R);
                            }
                            else
                            {
                                All_Signal_R = All_Signal_L;
                                All_Signal_L = filterRingMod(0, c, All_Signal_L, realcut, (float) FRez[c]);
                                All_Signal_R = filterRingModStereo(0, c, All_Signal_R);
                                grown = TRUE;
                            }
                            break;
#endif

#if defined(PTK_FILTER_SINGLEM)
                        case 10:
                            All_Signal_L = filterWater(0, c, All_Signal_L, realcut);
                            if(grown)
                            {
                                All_Signal_R = filterWater(1, c, All_Signal_R, realcut);
                            }
                            break;
#endif

#if defined(PTK_FILTER_SINGLES)
                        case 11:
                            All_Signal_L = filterWater(0, c, All_Signal_L, realcut);
                            if(grown)
                            {
                                All_Signal_R = filterWaterStereo(1, c, All_Signal_R, realcut);
                            }
                            break;
#endif

#if defined(PTK_FILTER_EQM15)
                        case 12:
                            All_Signal_L = filterBellShaped(0, c, All_Signal_L, realcut, (float) FRez[c], -15);
                            if(grown)
                            {
                                All_Signal_R = filterBellShaped(1, c, All_Signal_R, realcut, (float) FRez[c], -15);
                            }
                            break;
#endif

#if defined(PTK_FILTER_EQM6)
                        case 13:
                            All_Signal_L = filterBellShaped(0, c, All_Signal_L, realcut, (float) FRez[c], -6);
                            if(grown)
                            {
                                All_Signal_R = filterBellShaped(1, c, All_Signal_R, realcut, (float) FRez[c], -6);
                            }
                            break;
#endif

#if defined(PTK_FILTER_EQP6)
                        case 14:
                            All_Signal_L = filterBellShaped(0, c, All_Signal_L, realcut, (float) FRez[c], 6);
                            if(grown)
                            {
                                All_Signal_R = filterBellShaped(1, c, All_Signal_R, realcut, (float) FRez[c], 6);
                            }
                            break;
#endif

#if defined(PTK_FILTER_EQP15)
                        case 15:
                            All_Signal_L = filterBellShaped(0, c, All_Signal_L, realcut, (float) FRez[c], 15);
                            if(grown)
                            {
                                All_Signal_R = filterBellShaped(1, c, All_Signal_R, realcut, (float) FRez[c], 15);
                            }
                            break;
#endif

#if defined(PTK_FILTER_DELTA)
                        case 16:
                            All_Signal_L = filterDelta(0, c, All_Signal_L, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filterDelta(1, c, All_Signal_R, realcut, (float) FRez[c]);
                            }
                            break;
#endif

#if defined(PTK_FILTER_DISTL)
                        case 17:
                            All_Signal_L = int_filter2p(0, c, All_Signal_L, realcut, (float) FRez[c], 0.25f);
                            if(grown)
                            {
                                All_Signal_R = int_filter2p(1, c, All_Signal_R, realcut, (float) FRez[c], 0.25f);
                            }
                            break;
#endif

#if defined(PTK_FILTER_DISTM)
                        case 18:
                            All_Signal_L = int_filter2p(0, c, All_Signal_L, realcut, (float) FRez[c], 0.56f);
                            if(grown)
                            {
                                All_Signal_R = int_filter2p(1, c, All_Signal_R, realcut, (float) FRez[c], 0.56f);
                            }
                            break;
#endif

#if defined(PTK_FILTER_DISTH)
                        case 19:
                            All_Signal_L = int_filter2p(0, c, All_Signal_L, realcut, (float) FRez[c], 0.78f);
                            if(grown)
                            {
                                All_Signal_R = int_filter2p(1, c, All_Signal_R, realcut, (float) FRez[c], 0.78f);
                            }
                            break;
#endif

#if defined(PTK_FILTER_DIST)
                        case 20:
                            All_Signal_L = int_filter2p(0, c, All_Signal_L, realcut, (float) FRez[c], 0.96f);
                            if(grown)
                            {
                                All_Signal_R = int_filter2p(1, c, All_Signal_R, realcut, (float) FRez[c], 0.96f);
                            }
                            break;
#endif

#if defined(PTK_FILTER_HP12M)
                        case 21:
                            All_Signal_L = filterhp(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filterhp(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                            }
                            break;
#endif

#if defined(PTK_FILTER_HP12S)
                        case 22:
                            All_Signal_L = filterhp(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filterhp2(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                            }
                            break;
#endif

#if defined(PTK_FILTER_HP24M)
                        case 23:
                            All_Signal_L = filterhp(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            All_Signal_L = filterhp2(0, c, All_Signal_L + 1.0f, realcut, (float) FRez[c]);
                            if(grown)
                            {
                                All_Signal_R = filterhp(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                                All_Signal_R = filterhp2(1, c, All_Signal_R + 1.0f, realcut, (float) FRez[c]);
                            }
                            break;
#endif

                    } //SWITCHCASE [FILTERS]
                }

            } // Filter end
#endif // PTK_TRACKFILTERS

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
            if(Chan_Mute_State[c])
            {
                All_Signal_L = 0;
                All_Signal_R = 0;
            }
#endif

            // Dry Send
#if defined(PTK_DISCLAP)
            if(Disclap[c])
            {
                // Distortion
                if(All_Signal_L > DThreshold[c]) All_Signal_L = DClamp[c];
                else if(All_Signal_L < -DThreshold[c]) All_Signal_L = -DClamp[c];

                if(grown)
                {
                    if(All_Signal_R > DThreshold[c]) All_Signal_R = DClamp[c];
                    else if(All_Signal_R < -DThreshold[c]) All_Signal_R = -DClamp[c];
                }
            }
#endif

#if DENORMAL
            All_Signal_L = denormal(All_Signal_L);
#endif

            // Duplicate the mono signal if necessary
            if(!grown)
            {
                All_Signal_R = All_Signal_L;
            }
            else
            {

#if DENORMAL
                All_Signal_R = denormal(All_Signal_R);
#endif

            }

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
            if(Chan_Mute_State[c])
            {
                All_Signal_L = 0;
                All_Signal_R = 0;
            }
#endif
        }   // gotsome

#if defined(PTK_FLANGER)
        // 32-Bit HQ Interpolated System Flanger
        if(FLANGER_ON[c])
        {
            FLANGE_LEFTBUFFER[c][FLANGER_OFFSET[c]] = All_Signal_L *
                                                      FLANGER_AMOUNT[c] +
                                                      oldspawn[c] *
                                                      FLANGER_FEEDBACK[c];
            FLANGE_RIGHTBUFFER[c][FLANGER_OFFSET[c]] = All_Signal_R *
                                                       FLANGER_AMOUNT[c] +
                                                       roldspawn[c] *
                                                       FLANGER_FEEDBACK[c];

#if DENORMAL
            FLANGE_LEFTBUFFER[c][FLANGER_OFFSET[c]] = denormal(FLANGE_LEFTBUFFER[c][FLANGER_OFFSET[c]]);
            FLANGE_RIGHTBUFFER[c][FLANGER_OFFSET[c]] = denormal(FLANGE_RIGHTBUFFER[c][FLANGER_OFFSET[c]]);
#endif
            
            float fstep1;
            float fstep2;
            float gr_value = FLANGER_GR[c] / 6.283185f;
            float de_value = FLANGER_GR[c] + FLANGER_DEPHASE[c];
            if(de_value >= 6.283185f)
            {
                de_value -= 6.283185f;
            }
            de_value = ((de_value / 6.283185f));

#if DENORMAL
            gr_value = denormal(gr_value);
            de_value = denormal(de_value);
#endif

            fstep1 = POWF2(SIN[(int) (gr_value * 359.0f)] * FLANGER_AMPL[c]);
            fstep2 = POWF2(SIN[(int) (de_value * 359.0f)] * FLANGER_AMPL[c]);
            
            FLANGER_OFFSET2[c] += fstep1;
            FLANGER_OFFSET1[c] += fstep2;

            if(FLANGER_OFFSET2[c] >= 16384.0f) FLANGER_OFFSET2[c] -= 16384.0f;
            if(FLANGER_OFFSET1[c] >= 16384.0f) FLANGER_OFFSET1[c] -= 16384.0f;
            if(FLANGER_OFFSET2[c] < 0.0f) FLANGER_OFFSET2[c] += 16384.0f;
            if(FLANGER_OFFSET1[c] < 0.0f) FLANGER_OFFSET1[c] += 16384.0f;

            oldspawn[c] = FLANGE_LEFTBUFFER[c][(int) (FLANGER_OFFSET2[c])];
            roldspawn[c] = FLANGE_RIGHTBUFFER[c][(int) (FLANGER_OFFSET1[c])];

            All_Signal_L += Filter_FlangerL(c, ++oldspawn[c]);
            All_Signal_R += Filter_FlangerR(c, ++roldspawn[c]);

#if DENORMAL
            All_Signal_L = denormal(All_Signal_L);
            All_Signal_R = denormal(All_Signal_R);
#endif
            if(++FLANGER_OFFSET[c] >= 16384) FLANGER_OFFSET[c] -= 16384;
            FLANGER_GR[c] += FLANGER_RATE[c];

            if(FLANGER_GR[c] >= 6.283185f)
            {
                FLANGER_GR[c] -= 6.283185f;
                FLANGER_OFFSET2[c] = float(FLANGER_OFFSET[c] - FLANGER_DELAY[c]);
                FLANGER_OFFSET1[c] = float(FLANGER_OFFSET[c] - FLANGER_DELAY[c]);
            }
        }
#endif

#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
        if(Chan_Mute_State[c])
        {
            All_Signal_L = 0;
            All_Signal_R = 0;
        }
#endif

#if defined(PTK_LIMITER_TRACKS)
        // Compress the track signal
        if(Compress_Track[c])
        {
            All_Signal_L = Mas_Compressor_Track(c,
                                                All_Signal_L / 32767.0f,
                                                &rms_sumL_Track[c],
                                                mas_comp_bufferL_Track[c],
                                                &mas_envL_Track[c]) * 32767.0f;
            All_Signal_R = Mas_Compressor_Track(c,
                                                All_Signal_R / 32767.0f,
                                                &rms_sumR_Track[c],
                                                mas_comp_bufferR_Track[c],
                                                &mas_envR_Track[c]) * 32767.0f;
        }
#endif

#if defined(PTK_TRACK_EQ)
        if(EqDat[c].lg != 1.0f ||
           EqDat[c].mg != 1.0f ||
           EqDat[c].hg != 1.0f)
        {
            All_Signal_L = Do_Equ(&EqDat[c], All_Signal_L, 0);
            All_Signal_R = Do_Equ(&EqDat[c], All_Signal_R, 1);
        }
#endif

        if(Track_Surround[c])
        {
            All_Signal_R = -All_Signal_R;
        }

#if DENORMAL
        All_Signal_L = denormal(All_Signal_L);
        All_Signal_R = denormal(All_Signal_R);
#endif

        Compute_Stereo_Quick(c);

        All_Signal_L *= LVol[c];
        All_Signal_R *= RVol[c];

        // Soft panning
        if(LVol[c] != Old_LVol[c])
        {
            if(LVol[c] > Old_LVol[c])
            {
                LVol[c] -= 0.001f;
                if(LVol[c] < Old_LVol[c]) LVol[c] = Old_LVol[c];
            }
            else
            {
                LVol[c] += 0.001f;
                if(LVol[c] > Old_LVol[c]) LVol[c] = Old_LVol[c];
            }
        }

        if(RVol[c] != Old_RVol[c])
        {
            if(RVol[c] > Old_RVol[c])
            {
                RVol[c] -= 0.001f;
                if(RVol[c] < Old_RVol[c]) RVol[c] = Old_RVol[c];
            }
            else
            {
                RVol[c] += 0.001f;
                if(RVol[c] > Old_RVol[c]) RVol[c] = Old_RVol[c];
            }
        }

#if defined(PTK_LFO)
        All_Signal_L *= Apply_Lfo_To_Volume(c);
        All_Signal_R *= Apply_Lfo_To_Volume(c);
#endif

#if defined(PTK_TRACK_VOLUME)
        All_Signal_L *= Track_Volume[c];
        All_Signal_R *= Track_Volume[c];
#endif

        if(Track_Denoise[c])
        {
            float old_l_float = 0.0f;
            float old_r_float = 0.0f;
            float accum_l;
            float accum_r;
            float denoiser_coeff;
            int k;
            int pos_round;
            int denoise_mask;
            int denoise_size;
            int Table_Denoise[] = { 0, 4, 8, 16, 32 };
            float Table_Denoise_Coeffs[] = { 1.0f, 1.0f / 4, 1.0f / 8, 1.0f / 16, 1.0f / 32 };
            old_l_float = All_Signal_L;
            old_r_float = All_Signal_R;
            denoise_size = Table_Denoise[Track_Denoise[c]];
            denoiser_coeff = Table_Denoise_Coeffs[Track_Denoise[c]];
            denoise_mask = denoise_size - 1;
            accum_l = 0.0f;
            accum_r = 0.0f;
            for(k = 0; k < denoise_size; k++)
            {
                pos_round = (pos_round_float_history[c] - k) & denoise_mask;
                accum_l += left_float_history[c][pos_round];
                accum_r += right_float_history[c][pos_round];
            }
            All_Signal_L = accum_l;
            All_Signal_R = accum_r;
            left_float_history[c][pos_round_float_history[c]] = old_l_float * denoiser_coeff;
            right_float_history[c][pos_round_float_history[c]] = old_r_float * denoiser_coeff;
            pos_round_float_history[c]++;
            pos_round_float_history[c] &= denoise_mask;
        }

#if DENORMAL
        All_Signal_L = denormal(All_Signal_L);
        All_Signal_R = denormal(All_Signal_R);
#endif

        // Store to global signals
        left_float += All_Signal_L;
        right_float += All_Signal_R;

#if defined(PTK_COMPRESSOR)
        // Sending to delay
        float const DS = DSend[c];

        if(DS > 0.008f)
        {
            delay_left_final += All_Signal_L * DS;
            delay_right_final += All_Signal_R * DS;
        }
#endif

        // Sending to chorus
        float const DC = CCoef[c];

        if(DC > 0.008f)
        {
            left_chorus += All_Signal_L * DC;
            right_chorus += All_Signal_R * DC;
        }

        // Store the data for the tracks scopes
#if !defined(__STAND_ALONE__)
        if(!Chan_Mute_State[c])
        {
#if DENORMAL
            VuMeters_Dats_L[c][pos_scope] = denormal(All_Signal_L / 32767.0f);
            VuMeters_Dats_R[c][pos_scope] = denormal(All_Signal_R / 32767.0f);
#else
            VuMeters_Dats_L[c][pos_scope] = All_Signal_L / 32767.0f;
            VuMeters_Dats_R[c][pos_scope] = All_Signal_R / 32767.0f;
#endif
        }
        else
        {
            VuMeters_Dats_L[c][pos_scope] = 0.0f;
            VuMeters_Dats_R[c][pos_scope] = 0.0f;
        }
#endif
    } // Song_Tracks
}

// ------------------------------------------------------
// Look for a free sub channel
int Get_Free_Sub_Channel(int channel, int polyphony)
{
    int i;
    int oldest;
    int age_value;

    for(i = 0; i < polyphony; i++)
    {
        if(
#if defined(PTK_INSTRUMENTS)
           sp_Stage[channel][i] == PLAYING_NOSAMPLE 
#else
           TRUE
#endif
#if defined(PTK_SYNTH)
           && sp_Stage2[channel][i] == PLAYING_NOSAMPLE 
           && sp_Stage3[channel][i] == PLAYING_NOSAMPLE
#endif
          )
        {
            if(!Cut_Stage[channel][i])
            {
                return(i);
            }
        }
    }

    for(i = 0; i < polyphony; i++)
    {
        if(!Cut_Stage[channel][i])
        {
            if(
#if defined(PTK_INSTRUMENTS)
               sp_Stage[channel][i] == PLAYING_SAMPLE_NOTEOFF
#else
               TRUE
#endif
#if defined(PTK_SYNTH)
               && sp_Stage2[channel][i] == PLAYING_SAMPLE_NOTEOFF
               && sp_Stage3[channel][i] == PLAYING_SAMPLE_NOTEOFF
#endif
              )
            {
                return(i);
            }
        }
    }

    // Take the oldest playing one
    oldest = 0;
    age_value = Instrument_Schedule_Dat[channel][0].age;
    for(i = 1; i < polyphony; i++)
    {
        if(Instrument_Schedule_Dat[channel][i].age < age_value)
        {
            oldest = i;
        }
    }

    // None found
    return(oldest);
}

// ------------------------------------------------------
// Record an instrument for playing it later
// (We use this to avoid immediately chaning the instrument
//  during volume ramping as the previous one is still being played).
void Schedule_Instrument(int channel,
                         int sub_channel,
                         int inote,
                         int sample,
                         unsigned int offset,
                         int glide,
                         int Play_Selection,
                         int midi_sub_channel,
                         float vol,
                         int Pos,
                         int Row)
{
    int Cur_Position = Song_Position;
    if(Chan_Active_State[Cur_Position][channel])
    {
        if((Channels_MultiNotes[channel] - 1) <= sub_channel)
        {
            if(sample == 255)
            {
                sample = old_pl_sample[channel][midi_sub_channel - 1];
            }

        }
        // Nothing is already playing so play it directly
        old_note[channel][sub_channel] = inote;
        Instrument_Schedule_Dat[channel][sub_channel].start_backward = FALSE;
        Instrument_Schedule_Dat[channel][sub_channel].inote = inote;
        Instrument_Schedule_Dat[channel][sub_channel].sample = sample;
        Instrument_Schedule_Dat[channel][sub_channel].vol = Sample_Vol[sample] * vol;

#if defined(PTK_SYNTH)
#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
        Instrument_Schedule_Dat[channel][sub_channel].vol_synth = PARASynth[sample].GLB_VOLUME * vol;
#else
        Instrument_Schedule_Dat[channel][sub_channel].vol_synth = (PARASynth[sample].glb_volume * 0.0078125f) * vol;
#endif
#endif

        Instrument_Schedule_Dat[channel][sub_channel].offset = offset;
        Instrument_Schedule_Dat[channel][sub_channel].glide = glide;
        Instrument_Schedule_Dat[channel][sub_channel].Play_Selection = Play_Selection;
        Instrument_Schedule_Dat[channel][sub_channel].midi_sub_channel = midi_sub_channel;
        Instrument_Schedule_Dat[channel][sub_channel].age = AUDIO_GetSamples();

        sp_Cvol_Ramp_Dest[channel][sub_channel] = 10.0f;

        if(!glide)
        {
            Cut_Stage[channel][sub_channel] = TRUE;
        }
        else
        {
            Glide_Stage[channel][sub_channel] = TRUE;
        }
    }
}

// ------------------------------------------------------
// Play a waveform
void Play_Instrument(int channel, int sub_channel)
{
    int inote;
    int sample;
    float vol;
    float vol_synth;
    float channel_vol;
    unsigned int offset;
    int glide;
    int Play_Selection;
    int midi_sub_channel;

    int Cur_Position;
    int note2;

#if defined(PTK_INSTRUMENTS)
    float note;
#endif

    int associated_sample;
    int no_retrig_adsr = FALSE;
    int no_retrig_note = FALSE;

    channel_vol = sp_Tvol_Mod[channel];
    Cur_Position = Song_Position;

    // Check if the channel have to be played
    if(Chan_Active_State[Cur_Position][channel])
    {
        // Retrieve the scheduled data
        inote = Instrument_Schedule_Dat[channel][sub_channel].inote;
        sample = Instrument_Schedule_Dat[channel][sub_channel].sample;
        vol = Instrument_Schedule_Dat[channel][sub_channel].vol;
        vol_synth = Instrument_Schedule_Dat[channel][sub_channel].vol_synth;
        offset = Instrument_Schedule_Dat[channel][sub_channel].offset;
        glide = Instrument_Schedule_Dat[channel][sub_channel].glide;
        Play_Selection = Instrument_Schedule_Dat[channel][sub_channel].Play_Selection;
        midi_sub_channel = Instrument_Schedule_Dat[channel][sub_channel].midi_sub_channel;

        // There was no note specified
        if(inote > 120)
        {
            inote = (unsigned char) sp_channelnote[channel][sub_channel];
            no_retrig_note = TRUE;
        }

        // Empty row ? Take the current one.
        if(sample == 255)
        {
            sample = (unsigned char) sp_channelsample[channel][sub_channel];
            // retrieve it's volume
            vol = sp_Tvol[channel][sub_channel];
            vol_synth = sp_Tvol_Synth[channel][sub_channel];
            if(glide)
            {
                no_retrig_adsr = TRUE;
            }
        }

#if defined(PTK_SYNTH)
        switch(Synth_Prg[sample])
        {
            case SYNTH_WAVE_OFF:          // synth off
            case SYNTH_WAVE_CURRENT:      // Current waveform selected
                associated_sample = sample;
                break;
            default:                      // Any other waveform selected
                associated_sample = Synth_Prg[sample] - 2;
                break;
        }
#else
        associated_sample = sample;
#endif

        int split = 0;

        if(associated_sample != 255)
        {

#if defined(PTK_INSTRUMENTS)
            for(int revo = 0; revo < MAX_INSTRS_SPLITS; revo++)
            {
                if(inote >= Basenote[associated_sample][revo] &&
                   SampleType[associated_sample][revo] != 0)
                {
                    split = revo;
                }
            }
#endif

#if defined(PTK_SYNTH)
            if(!no_retrig_note)
            {
                if(Synthesizer[channel][sub_channel].Data.OSC_1_WAVEFORM == WAVEFORM_WAV)
                {
                    if(!glide)
                    {
                        sp_Position[channel][sub_channel].int_pos = 0;
                        sp_Position[channel][sub_channel].flt_pos = 0;
                        sp_Position_osc_1[channel][sub_channel].int_pos = 0;
                        sp_Position_osc_1[channel][sub_channel].flt_pos = 0;

#if defined(PTK_SYNTH_OSC_3)
                        sp_Position_osc_3[channel][sub_channel].int_pos = 0;
                        sp_Position_osc_3[channel][sub_channel].flt_pos = 0;
#endif

                    }

                }
                if(Synthesizer[channel][sub_channel].Data.OSC_2_WAVEFORM == WAVEFORM_WAV)
                {
                    if(!glide)
                    {
                        sp_Position[channel][sub_channel].int_pos = 0;
                        sp_Position[channel][sub_channel].flt_pos = 0;
                        sp_Position_osc_2[channel][sub_channel].int_pos = 0;
                        sp_Position_osc_2[channel][sub_channel].flt_pos = 0;
                    }
                }
            }
#endif

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
            int mnote = inote;
#endif
#endif

            // Nullify the glide if the instrument is different
            if(sample != sp_channelsample[channel][sub_channel])
            {
                glide = 0;
            }

            note2 = inote - DEFAULT_BASE_NOTE;

#if defined(PTK_INSTRUMENTS)
            note = (float) inote;
            note -= Basenote[associated_sample][split];
            note += float((float) Finetune[associated_sample][split] * 0.0078125f);
#endif

#if defined(PTK_SYNTH)
            if(!no_retrig_adsr && !no_retrig_note)
            {
                if(!glide)
                {
                    Synthesizer[channel][sub_channel].Data.OSC_1_WAVEFORM = WAVEFORM_NONE;
                    Synthesizer[channel][sub_channel].Data.OSC_2_WAVEFORM = WAVEFORM_NONE;
                    // No synth playing
                    if(!Synthesizer[channel][sub_channel].ENV_1_STAGE && 
                       !Synthesizer[channel][sub_channel].ENV_1_STAGE)
                    {
                        Synth_Really_Used[channel][sub_channel] = 0;
                    }
                    else
                    {
                        if(Synth_Prg[sample])
                        {
                            // Schedule a release if it a synth
                            Synthesizer[channel][sub_channel].ENV_1_STAGE = SYNTH_RELEASE;
                            Synthesizer[channel][sub_channel].ENV_2_STAGE = SYNTH_RELEASE;
                        }
                    }
                }
                if(Synth_Prg[sample])
                {

#if defined(__STAND_ALONE__) && !defined(__WINAMP__)
                    Synthesizer[channel][sub_channel].ChangeParameters(&PARASynth[sample]);
#else
                    Synthesizer[channel][sub_channel].ChangeParameters(PARASynth[sample]);
#endif

                    Synthesizer[channel][sub_channel].NoteOn(note2,
                                                             vol,
                                                             LoopType[associated_sample][split],
                                                             LoopType[associated_sample][split] > SMP_LOOP_NONE ? LoopEnd[associated_sample][split]: (Sample_Length[associated_sample][split] - 2),
                                                             LoopEnd[associated_sample][split] - LoopStart[associated_sample][split]
#if defined(PTK_INSTRUMENTS)
                                                             ,note
#endif
                                                             ,glide
                                                             );
                    Synth_Really_Used[channel][sub_channel] = Synth_Prg[associated_sample];
                }
            }
#endif

            // Fix a bug as this can also be used for synth
            // which isn't correct
            Player_SC[channel][sub_channel] = 0;

            // Store the specified volume
            sp_Tvol[channel][sub_channel] = vol;
            sp_Cvol_Ramp_Dest[channel][sub_channel] = 10.0f;
            sp_Tvol_Synth[channel][sub_channel] = vol_synth;
            sp_Cvol_Synth_Ramp_Dest[channel][sub_channel] = 10.0f;

            float spreadnote = ((float) POWF2(note2 / 12.0f));

#if defined(PTK_FX_ARPEGGIO)
            Vstep_arp[channel][sub_channel] = spreadnote;
            Arpeggio_BaseNote[channel][sub_channel] = (float) note2;
#endif

#if defined(PTK_FX_VIBRATO)
            Vstep_vib[channel][sub_channel] = spreadnote;
            Vibrato_BaseNote[channel][sub_channel] = (float) note2;
#endif

            if(!no_retrig_note)
            {
                if(glide)
                {
                    sp_Step[channel][sub_channel] = spreadnote;
                }
                else
                {
                    Vstep1[channel][sub_channel] = spreadnote;
                    sp_Step[channel][sub_channel] = spreadnote;
                }
            }

#if defined(PTK_INSTRUMENTS)
            sp_Stage[channel][sub_channel] = PLAYING_NOSAMPLE;
#endif

#if defined(PTK_SYNTH)
            if(Synth_Prg[sample] == SYNTH_WAVE_OFF)
            {
                sp_Stage2[channel][sub_channel] = PLAYING_NOSAMPLE;
                sp_Stage3[channel][sub_channel] = PLAYING_NOSAMPLE;
            }
            else
            {
                sp_Stage2[channel][sub_channel] = PLAYING_STOCK;
                sp_Stage3[channel][sub_channel] = PLAYING_STOCK;
            }
#endif
            sp_channelsample[channel][sub_channel] = sample;
            sp_channelnote[channel][sub_channel] = inote;

#if defined(PTK_INSTRUMENTS)
            if(SampleType[associated_sample][split])
            {

#if defined(PTK_SYNTH)
                if(Synth_Prg[sample])
                {
                    // Synth + sample together if both OSCs are != wav
                    if(Synthesizer[channel][sub_channel].Data.OSC_1_WAVEFORM != WAVEFORM_WAV &&
                       Synthesizer[channel][sub_channel].Data.OSC_2_WAVEFORM != WAVEFORM_WAV)
                    {
                        sp_Stage[channel][sub_channel] = PLAYING_SAMPLE;
                    }
                }
                else
                {
#endif
                    // Only sample
                    sp_Stage[channel][sub_channel] = PLAYING_SAMPLE;

#if defined(PTK_SYNTH)
                    sp_Stage2[channel][sub_channel] = PLAYING_NOSAMPLE;
                    sp_Stage3[channel][sub_channel] = PLAYING_NOSAMPLE;
#endif

#if defined(PTK_SYNTH)
                }
#endif

#if defined(PTK_FX_ARPEGGIO)
                Arpeggio_BaseNote[channel][sub_channel] = note;
#endif

#if defined(PTK_FX_VIBRATO)
                Vibrato_BaseNote[channel][sub_channel] = note;
#endif

                if(Beat_Sync[associated_sample])
                {
                    float spreadnote = ((float) (Sample_Length[associated_sample][split]) / ((float) Beat_Lines[associated_sample] * (float) SamplesPerTick));

#if defined(PTK_FX_ARPEGGIO)
                    Vstep_arp[channel][sub_channel] = spreadnote;
#endif

#if defined(PTK_FX_VIBRATO)
                    Vstep_vib[channel][sub_channel] = spreadnote;
#endif

                    if(!no_retrig_note)
                    {
                        Vstep1[channel][sub_channel] = spreadnote;
                        sp_Step[channel][sub_channel] = spreadnote;
                    }
                }
                else
                {
                    float spreadnote = ((float) POWF2(note / 12.0f));

#if defined(PTK_FX_ARPEGGIO)
                    Vstep_arp[channel][sub_channel] = spreadnote;
#endif

#if defined(PTK_FX_VIBRATO)
                    Vstep_vib[channel][sub_channel] = spreadnote;
#endif

                    if(!no_retrig_note)
                    {
                        if(glide)
                        {
                            sp_Step[channel][sub_channel] = spreadnote;
                        }
                        else
                        {
                            Vstep1[channel][sub_channel] = spreadnote;
                            sp_Step[channel][sub_channel] = spreadnote;
                        }
                    }
                }

#if defined(PTK_SYNTH)
                Synth_Was[channel][sub_channel] = Synth_Prg[sample];
#endif
                sp_split[channel][sub_channel] = split;

                // Player Pointer Assignment

#if !defined(__STAND_ALONE__)
                Uint32 Sel_Start;
                Uint32 Sel_End;
            
                // Only play the selection
                if(userscreen == USER_SCREEN_SAMPLE_EDIT &&
                   sed_range_start != sed_range_end && Play_Selection)
                {
                    Sel_Start = sed_range_start;
                    Sel_End = sed_range_end;
                    if(sed_range_start > sed_range_end)
                    {
                        Sel_End = sed_range_start;
                        Sel_Start = sed_range_end;
                    }
                    Player_LS[channel][sub_channel] = Sel_Start;
                    Player_LE[channel][sub_channel] = Sel_End;
                    if(!no_retrig_note)
                    {
                        sp_Position[channel][sub_channel].int_pos = Sel_Start;
                        sp_Position[channel][sub_channel].flt_pos = Sel_Start;
                    }
                    Player_NS[channel][sub_channel] = Sel_End;
                    if(!glide)
                    {
                        if(!no_retrig_note)
                        {
                            sp_Position[channel][sub_channel].int_pos += offset << 8;
                            sp_Position[channel][sub_channel].flt_pos += offset << 8;
                        }
                    }
                }
                else
                {
                    Player_LS[channel][sub_channel] = LoopStart[associated_sample][split];
                    Player_LE[channel][sub_channel] = LoopEnd[associated_sample][split];
                    Player_NS[channel][sub_channel] = Sample_Length[associated_sample][split];
                    if(!glide)
                    {
                        if(!no_retrig_note)
                        {
                            sp_Position[channel][sub_channel].int_pos = offset << 8;
                            sp_Position[channel][sub_channel].flt_pos = offset << 8;
                        }
                    }
                }
#else
                Player_LS[channel][sub_channel] = LoopStart[associated_sample][split];
                Player_LE[channel][sub_channel] = LoopEnd[associated_sample][split];
                Player_NS[channel][sub_channel] = Sample_Length[associated_sample][split];
                if(!glide)
                {
                    if(!no_retrig_note)
                    {
                        sp_Position[channel][sub_channel].int_pos = offset << 8;
                        sp_Position[channel][sub_channel].flt_pos = (float) (offset << 8);
                    }
                }
#endif
                Player_LL[channel][sub_channel] = Player_LE[channel][sub_channel] - Player_LS[channel][sub_channel];

                Player_Ampli[channel][sub_channel] = Sample_Amplify[associated_sample][split];
                Player_LT[channel][sub_channel] = LoopType[associated_sample][split];
                if(!no_retrig_note)
                {
                    Player_LW[channel][sub_channel] = SMP_LOOPING_FORWARD;
                }
                else
                {
                    // It may already be playing backward so don't change it in that case
                    if(Player_LT[channel][sub_channel] != SMP_LOOP_PINGPONG)
                    {
                        Player_LW[channel][sub_channel] = SMP_LOOPING_FORWARD;
                    }
                }
                Player_SC[channel][sub_channel] = Sample_Channels[associated_sample][split];

                // I know this isn't exactly correct but using a sub channel for this
                // would mean that we'd have to maintain 1 filters state per sub channel which would be insane.
                Player_FD[channel] = FDecay[associated_sample][split];

                Player_WL[channel][sub_channel] = RawSamples[associated_sample][0][split];
                if(Sample_Channels[associated_sample][split] == 2)
                {
                    Player_WR[channel][sub_channel] = RawSamples[associated_sample][1][split];
                }
                if(!glide)
                {
                    ramper[channel] = 0;
                }
            }
            else
#endif // PTK_INSTRUMENTS

            {
                Player_WL[channel][sub_channel] = 0;
                Player_WR[channel][sub_channel] = 0;
                Player_Ampli[channel][sub_channel] = 1.0f;
                if(!glide)
                {
                    ramper[channel] = 0;
                    if(!no_retrig_note)
                    {
                        sp_Position[channel][sub_channel].int_pos = offset << 8;
                        sp_Position[channel][sub_channel].flt_pos = (float) (offset << 8);
                    }
                }
            }

            // Sample is out of range
            // (synths can have Sample_Length = 0)
            if(Sample_Length[associated_sample][split])
            {
                if((int) sp_Position[channel][sub_channel].int_pos >= (int) Sample_Length[associated_sample][split])
                {
                    if(LoopType[associated_sample][split])
                    {
                        // Don't cross the boundaries of the loop data
                        sp_Position[channel][sub_channel].int_pos = Player_LE[channel][sub_channel];
                        sp_Position[channel][sub_channel].flt_pos = (float) Player_LE[channel][sub_channel];
                    }
                    else
                    {

#if defined(PTK_INSTRUMENTS)
                        sp_Stage[channel][sub_channel] = PLAYING_NOSAMPLE;
#endif

#if defined(PTK_SYNTH)
                        sp_Stage2[channel][sub_channel] = PLAYING_NOSAMPLE;
                        sp_Stage3[channel][sub_channel] = PLAYING_NOSAMPLE;
#endif
                    }
                }
            }

#if defined(PTK_SYNTH)
            if(!no_retrig_adsr && !no_retrig_note && !glide)
            {
                sp_Position_osc_1[channel][sub_channel] = sp_Position[channel][sub_channel];
                sp_Position_osc_2[channel][sub_channel] = sp_Position[channel][sub_channel];

#if defined(PTK_SYNTH_OSC_3)
                sp_Position_osc_3[channel][sub_channel] = sp_Position[channel][sub_channel];
#endif
            }
#endif

            Pos_Segue[channel] = 0;
            Segue_Volume[channel] = 1.0f;
            New_Instrument[channel] = TRUE;

            // Check if we must start playing it backward
            if(!glide)
            {
#if defined(PTK_SYNTH)
                Synthesizer[channel][sub_channel].ENV_1_LOOP_BACKWARD = FALSE;
                Synthesizer[channel][sub_channel].ENV_3_LOOP_BACKWARD = FALSE;
                Synthesizer[channel][sub_channel].ENV_2_LOOP_BACKWARD = FALSE;
#endif
                Player_LW[channel][sub_channel] = SMP_LOOPING_FORWARD;
            }
            if(Instrument_Schedule_Dat[channel][sub_channel].start_backward)
            {
                if(!glide)
                {
#if defined(PTK_SYNTH)
                    Synthesizer[channel][sub_channel].ENV_1_LOOP_BACKWARD = TRUE;
                    Synthesizer[channel][sub_channel].ENV_3_LOOP_BACKWARD = TRUE;
                    Synthesizer[channel][sub_channel].ENV_2_LOOP_BACKWARD = TRUE;
#endif
                    Player_LW[channel][sub_channel] = SMP_LOOPING_BACKWARD;
                }

                int Max_Loop = Player_NS[channel][sub_channel];
                // No loop: go to the end of the sample
                if((int) Player_LE[channel][sub_channel] < Max_Loop)
                {
                    Max_Loop = Player_LE[channel][sub_channel];
                }
                sp_Position[channel][sub_channel].int_pos = Max_Loop;
                sp_Position[channel][sub_channel].flt_pos = (float) Max_Loop;
#if defined(PTK_SYNTH)
                if(Synthesizer[channel][sub_channel].Data.OSC_1_WAVEFORM == WAVEFORM_WAV)
                {
                    sp_Position_osc_1[channel][sub_channel].int_pos = Max_Loop;
                    sp_Position_osc_1[channel][sub_channel].flt_pos = (float) Max_Loop;
#if defined(PTK_SYNTH_OSC_3)
                    sp_Position_osc_3[channel][sub_channel].int_pos = Max_Loop;
                    sp_Position_osc_3[channel][sub_channel].flt_pos = (float) Max_Loop;
#endif
                }
                if(Synthesizer[channel][sub_channel].Data.OSC_2_WAVEFORM == WAVEFORM_WAV)
                {
                    sp_Position_osc_2[channel][sub_channel].int_pos = Max_Loop;
                    sp_Position_osc_2[channel][sub_channel].flt_pos = (float) Max_Loop;
                }
#endif
            }

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
            if(Chan_Mute_State[channel] == FALSE &&
               c_midiout != -1 &&
               Midiprg[associated_sample] != -1)
            {
                // Remove the previous note
                if(midi_sub_channel >= 1 && Midi_Current_Notes[Chan_Midi_Prg[channel]][midi_sub_channel - 1])
                {
                    Midi_Note_Off(channel, Midi_Current_Notes[Chan_Midi_Prg[channel]][midi_sub_channel - 1]);
                    Midi_Current_Notes[Chan_Midi_Prg[channel]][midi_sub_channel - 1] = 0;
                }

                // Set the midi program if it was modified
                if(LastProgram[Chan_Midi_Prg[channel]] != Midiprg[associated_sample])
                {
                    Midi_Send(0xb0 + Chan_Midi_Prg[channel], 0, (Midiprg[associated_sample] & 0x7f) * 2);
                    Midi_Send(0xc0 + Chan_Midi_Prg[channel], Midiprg[associated_sample] & 0x7f, 127 * 2);
                    LastProgram[Chan_Midi_Prg[channel]] = Midiprg[associated_sample];
                }

                // Send the note to the midi device
                float veloc = vol * channel_vol * mas_vol * local_mas_vol * local_ramp_vol * Track_Volume[channel];

                Midi_Send(0x90 + Chan_Midi_Prg[channel], mnote, (int) (veloc * 127) * 2);
                if(midi_sub_channel < 0) Midi_Current_Notes[Chan_Midi_Prg[channel]][(-midi_sub_channel) - 1] = mnote;
                else Midi_Current_Notes[Chan_Midi_Prg[channel]][midi_sub_channel - 1] = mnote;
            }
#endif // __NO_MIDI
#endif // __STAND_ALONE__

        }
    }
}

// ------------------------------------------------------
// Handle patterns effects
#if defined(PTK_FX_TICK0)
void Do_Effects_Tick_0(void)
{

#if defined(PTK_FX_ARPEGGIO) || defined(PTK_FX_VIBRATO) || defined(PTK_FX_REVERSE) || defined(PTK_SHUFFLE) || \
    defined(PTK_FX_SETREVCUTO) || defined(PTK_FX_SETREVRESO) || defined(PTK_FX_SETREVDAMP) || defined(PTK_LIMITER_TRACKS) || \
    defined(PTK_FX_SETBPM) || defined(PTK_FX_SETSPEED)

    int i;
    int j;
    int pltr_eff_row[MAX_FX];
    int pltr_dat_row[MAX_FX];

    for(int trackef = 0; trackef < Song_Tracks; trackef++)
    {
        int tefactor = Get_Pattern_Offset(pSequence[Song_Position], trackef, Pattern_Line);

        for(j = 0; j < Channels_Effects[trackef]; j++)
        {
            pltr_eff_row[j] = *(RawPatterns + tefactor + PATTERN_FX + (j * 2));
            pltr_dat_row[j] = *(RawPatterns + tefactor + PATTERN_FXDATA + (j * 2));

            switch(pltr_eff_row[j])
            {

#if defined(PTK_FX_ARPEGGIO)
                // $1b arpeggio switch on/off
                case 0x1b:
                    Arpeggio_Switch[trackef] = pltr_dat_row[j];
                    if(!pltr_dat_row[j])
                    {
                        for(i = 0; i < Channels_Polyphony[trackef]; i++)
                        {
                            Vstep1[trackef][i] = Vstep_arp[trackef][i];
                        }
                    }
                    break;
#endif

#if defined(PTK_FX_VIBRATO)
                // $1d vibrato switch on/off
                case 0x1d:
                    Vibcounter[trackef] = 0;
                    Vibrato_Switch[trackef] = pltr_dat_row[j];
                    if(!pltr_dat_row[j])
                    {
                        for(i = 0; i < Channels_Polyphony[trackef]; i++)
                        {
                            Vstep1[trackef][i] = Vstep_vib[trackef][i];
                        }
                    }
                    break;
#endif

#if defined(PTK_FX_REVERSE)
                case 0x1e:
                    Reverse_Switch[trackef] = TRUE;
                    Reserve_Dat[trackef] = pltr_dat_row[j];
                    break;
#endif

#if defined(PTK_SHUFFLE)
                case 0x25:
                    shuffle_amount = (int) ((float) pltr_dat_row[j] * 0.39216f);
                    Update_Shuffle();
                    break;
#endif

#if defined(PTK_FX_SETREVCUTO)
                case 0x26:
                    Reverb_Filter_Cutoff = pltr_dat_row[j] / 255.0f * 0.99f;
                    if(Reverb_Filter_Cutoff < 0.02f) Reverb_Filter_Cutoff = 0.02f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_REVERB_FILTER;
                    }
#endif

                    break;
#endif

#if defined(PTK_FX_SETREVRESO)
                case 0x27:
                    Reverb_Filter_Resonance = pltr_dat_row[j] / 255.0f * 0.99f;
                    if(Reverb_Filter_Resonance < 0.02f) Reverb_Filter_Resonance = 0.02f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_REVERB_FILTER;
                    }
#endif

                    break;
#endif

#if defined(PTK_LIMITER_TRACKS)
                case 0x29:
                    Compress_Track[trackef] = pltr_dat_row[j] & TRUE;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_COMPRESSOR;
                    }
#endif

                    break;
#endif

#if defined(PTK_LIMITER_TRACKS)
                case 0x2a:
                    Mas_Compressor_Set_Variables_Track(trackef, pltr_dat_row[j] / 255.0f * 100.0f, mas_comp_ratio_Track[trackef]);

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_COMPRESSOR;
                    }
#endif

                    break;
#endif

#if defined(PTK_LIMITER_TRACKS)
                case 0x2b:
                    Mas_Compressor_Set_Variables_Track(trackef, mas_threshold_Track[trackef], pltr_dat_row[j] / 255.0f * 100.0f);

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_COMPRESSOR;
                    }
#endif

                    break;
#endif

#if defined(PTK_FX_SETREVDAMP)
                case 0x2c:
                    Reverb_Damp = pltr_dat_row[j] / 255.0f * 1.0f;
                    if(Reverb_Damp < 0.02f) Reverb_Damp = 0.02f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_REVERB_DAMP;
                    }
#endif

                    break;
#endif

#if defined(PTK_LFO)
#if defined(PTK_FX_RESETFILTERLFO)
                // $16 Set channel filter lfo carrier value
                case 0x16:
                    LFO_CARRIER_FILTER[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 359.0f;
                    LFO_CARRIER_VOLUME[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 359.0f;
                    LFO_CARRIER_PANNING[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 359.0f;
                    break;
#endif
#endif

#if defined(PTK_LFO)
#if defined(PTK_FX_SETLFORATE)
                // $43 Set channel lfo frequency value
                case 0x43:
                    LFO_RATE[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 0.0078125f;
                    if(LFO_RATE[trackef] < 0.0001f) LFO_RATE[trackef] = 0.0001f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_LFO;
                    }
#endif
                    
                    break;
#endif
#endif

#if defined(PTK_LFO)
#if defined(PTK_FX_SETLFOSCALE)
                // $44 Set channel lfo scale value
                case 0x44:
                    LFO_RATE_SCALE[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 15.0f + 1.0f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_LFO;
                    }
#endif
                    
                    break;
#endif
#endif

#if defined(PTK_LFO)
#if defined(PTK_FX_SETFILTERLFO)
                // $45 Set channel filter lfo value
                case 0x45:
                    LFO_AMPL_FILTER[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 128.0f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_LFO;
                    }
#endif

                    break;
#endif
#endif

#if defined(PTK_LFO)
#if defined(PTK_FX_SETVOLUMELFO)
                // $46 Set channel volume lfo value
                case 0x46:
                    LFO_AMPL_VOLUME[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 128.0f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_LFO;
                    }
#endif
                    
                    break;
#endif
#endif

#if defined(PTK_LFO)
#if defined(PTK_FX_SETPANNINGLFO)
                // $47 Set channel panning lfo value
                case 0x47:
                    LFO_AMPL_PANNING[trackef] = ((float) pltr_dat_row[j] / 255.0f) * 128.0f;

#if !defined(__STAND_ALONE__)
                    if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                    {
                        gui_action_external |= GUI_UPDATE_EXTERNAL_LFO;
                    }
#endif
                    
                    break;
#endif
#endif

#if defined(PTK_FX_SETBPM)
                // $f0 Set BPM
                case 0xf0:
                    if(pltr_dat_row[j] >= 20)
                    {
                        Beats_Per_Min = (int) pltr_dat_row[j];
                        Calc_Tempo();
                    }
                    break;
#endif

#if defined(PTK_FX_SETSPEED)
                // $0f Set speed
                case 0x0f:
                    if(pltr_dat_row[j] == 0)
                    {
                        Song_Playing = FALSE;
#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
                        gui_action = GUI_CMD_STOP_SONG;
#endif
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
                        done = TRUE;
#endif
                        break;
                    }
                    Ticks_Per_Beat = (int) pltr_dat_row[j];
                    if(Ticks_Per_Beat < 1) Ticks_Per_Beat = 1;
                    if(Ticks_Per_Beat > 32) Ticks_Per_Beat = 32;
                    Calc_Tempo();

#if defined(PTK_SHUFFLE)
                    Update_Shuffle();
#endif

                    break;
#endif

            }
        }
    }      

#endif  // defined(PTK_FX_ARPEGGIO) || defined(PTK_FX_VIBRATO) || defined(PTK_FX_REVERSE) etc.

}

#if defined(PTK_FX_PATTERNLOOP)
void Do_Pattern_Loop(int track)
{
    int j;
    int pltr_eff_row[MAX_FX];
    int pltr_dat_row[MAX_FX];

    int tefactor = Get_Pattern_Offset(pSequence[Song_Position], track, Pattern_Line);

    for(j = 0; j < Channels_Effects[track]; j++)
    {
        pltr_eff_row[j] = *(RawPatterns + tefactor + PATTERN_FX + (j * 2));
        pltr_dat_row[j] = *(RawPatterns + tefactor + PATTERN_FXDATA + (j * 2));

        switch(pltr_eff_row[j])
        {
            // $06 Pattern loop
            case 0x6:
                if(!pltr_dat_row[j])
                {
                    if(repeat_loop_counter == 0)
                    {
                        repeat_loop_pos = Pattern_Line;
                    }
                }
                else
                {
                    if(repeat_loop_pos != -1)
                    {
                        if(repeat_loop_counter == 0)
                        {
                            repeat_loop_counter = ((int) pltr_dat_row[j]);
                            repeat_loop_pos = (Pattern_Line - repeat_loop_pos);
                            repeat_loop_counter_in = 1;
                        }
                        else
                        {
                            // count down
                            repeat_loop_counter--;
                            if(!repeat_loop_counter)
                            {
                                repeat_loop_pos = -1;
                                repeat_loop_counter_in = 0;
                            }
                            else
                            {
                                repeat_loop_counter_in = 1;
                            }
                        }
                    }
                } 
                break;
        }
    }
}
#endif // PTK_FX_PATTERNLOOP

#endif // PTK_FX_TICK0

// ------------------------------------------------------
// Process ticks X effects
void Do_Effects_Ticks_X(void)
{
    int i;

#if defined(PTK_FX_NOTERETRIGGER)
    int j;
#endif

    int k;
    int pltr_note[MAX_POLYPHONY];
    int pltr_sample[MAX_POLYPHONY];

#if defined(PTK_FX_0) || defined(PTK_FX_X)
    int pltr_eff_row[MAX_FX];
#endif

#if defined(PTK_FX_0) || defined(PTK_FX_X)
    int pltr_dat_row[MAX_FX];
#endif

    for(int trackef = 0; trackef < Song_Tracks; trackef++)
    {
        int tefactor = Get_Pattern_Offset(pSequence[Song_Position], trackef, Pattern_Line);

        // Get the notes for this track
        for(i = 0; i < Channels_MultiNotes[trackef]; i++)
        {
            pltr_note[i] = *(RawPatterns + tefactor + PATTERN_NOTE1 + (i * 2));
            pltr_sample[i] = *(RawPatterns + tefactor + PATTERN_INSTR1 + (i * 2));
        }

        unsigned char pltr_vol_row = *(RawPatterns + tefactor + PATTERN_VOLUME);

#if defined(PTK_FX_0) || defined(PTK_FX_X)
        for(i = 0; i < Channels_Effects[trackef]; i++)
        {
            pltr_eff_row[i] = *(RawPatterns + tefactor + PATTERN_FX + (i * 2));
            pltr_dat_row[i] = *(RawPatterns + tefactor + PATTERN_FXDATA + (i * 2));
        }
#endif

#if defined(PTK_FX_AUTOFADEMODE)
        // Autofade routine
        switch(FADEMODE[trackef])
        {
            case 1:
                sp_Tvol_Mod[trackef] += FADECOEF[trackef];

                if(sp_Tvol_Mod[trackef] > 1.0f)
                {
                    sp_Tvol_Mod[trackef] = 1.0f;
                    FADEMODE[trackef] = 0;
                }
                break;

            case 2:
                sp_Tvol_Mod[trackef] -= FADECOEF[trackef];

                if(sp_Tvol_Mod[trackef] < 0.0f)
                {   
                    sp_Tvol_Mod[trackef] = 0.0f;
                    FADEMODE[trackef] = 0;
                }
                break;
        }
#endif

#if defined(PTK_FX_NOTECUT)
            if((pltr_vol_row & 0xf0) == 0xf0)
            {   // Note Cut: Fx
                unsigned char kinder = pltr_vol_row & 0xf;
                if(Subicounter == kinder)
                {
                    for(i = 0; i < Channels_Polyphony[trackef]; i++)
                    {

#if defined(PTK_INSTRUMENTS)
                        if(sp_Stage[trackef][i] == PLAYING_SAMPLE)
                        {
                            if(FType[trackef] == 4)
                            {
                                sp_Stage[trackef][i] = PLAYING_SAMPLE_NOTEOFF;
                            }
                            else
                            {
                                sp_Tvol_Mod[trackef] = 0.001f;
                            }
                        }
#endif

#if defined(PTK_SYNTH)
                        Synthesizer[trackef][i].Note_Off();
#endif

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
                        if(Midi_Current_Notes[Chan_Midi_Prg[trackef]][i])
                        {
                            Midi_Note_Off(trackef, Midi_Current_Notes[Chan_Midi_Prg[trackef]][i]);
                            Midi_Current_Notes[Chan_Midi_Prg[trackef]][i] = 0;
                        }
#endif
#endif
                    }
                }
            }
#endif

        // Effects
        for(k = 0; k < Channels_Effects[trackef]; k++)
        {

#if defined(PTK_FX_FINEVOLUMESLIDEUP) || defined(PTK_FX_FINEVOLUMESLIDEDOWN) || \
    defined(PTK_FX_FINEPITCHUP) || defined(PTK_FX_FINEPITCHDOWN) || \
    defined(PTK_FX_SENDTODELAYCOMMAND) || defined(PTK_FX_SENDTOREVERBCOMMAND) || \
    defined(PTK_FX_SETDISTORTIONTHRESHOLD) || defined(PTK_FX_SETDISTORTIONCLAMP) || \
    defined(PTK_FX_SETCUTOFF) || defined(PTK_FX_SETFILTERRESONANCE) || defined(PTK_FX_SWITCHFLANGER) || \
    defined(PTK_FX_TRACK_FILTER_LFO) || defined(PTK_FX_SETFILTERTYPE) || \
    defined(PTK_FX_SETCHORUSFILTERTYPE) || defined(PTK_FX_SETCHORUSCUTOFF) || defined(PTK_FX_SETCHORUSRESONANCE) || \
    defined(PTK_FX_SETDENOISE)

            // Only at tick 0 but after instruments data
            if(PosInTick == 0)
            {
                switch(pltr_eff_row[k])
                {

#if defined(PTK_FX_FINEVOLUMESLIDEUP)
                    // $20 fine volume slide up
                    case 0x20:
                        sp_Tvol_Mod[trackef] += pltr_dat_row[k] * 0.0039062f;
                        if(sp_Tvol_Mod[trackef] > 1.0f) sp_Tvol_Mod[trackef] = 1.0f;
                        break;
#endif

#if defined(PTK_FX_FINEVOLUMESLIDEDOWN)
                    // $20 fine volume slide down
                    case 0x21:
                        sp_Tvol_Mod[trackef] -= pltr_dat_row[k] * 0.0039062f;
                        if(sp_Tvol_Mod[trackef] < 0.0f) sp_Tvol_Mod[trackef] = 0.0f;
                        break;
#endif

#if defined(PTK_FX_FINEPITCHUP)
                    // $22 fine pitch up
                    case 0x22:

                        for(i = 0; i < Channels_Polyphony[trackef]; i++)
                        {
                            if(Vstep1[trackef][i] < 5.66029f)
                            {
                                Vstep1[trackef][i] += ((float) pltr_dat_row[k]) / 2048.0f;
                            }
                        }
                        break;
#endif

#if defined(PTK_FX_FINEPITCHDOWN)
                    // $23 fine pitch down
                    case 0x23:
                        for(i = 0; i < Channels_Polyphony[trackef]; i++)
                        {
                            if(Vstep1[trackef][i] > 0.00586236f)
                            {
                                Vstep1[trackef][i] -= ((float) pltr_dat_row[k]) / 2048.0f;
                            }
                        }
                        break;
#endif

#if defined(PTK_FX_SENDTODELAYCOMMAND)
                    // $10 Send to delay Command
                    case 0x10:
                        CCoef[trackef] = (float) pltr_dat_row[k] / 255.0f;
                        break;
#endif

#if defined(PTK_FX_SENDTOREVERBCOMMAND)
                    // $11 Send to reverb Command
                    case 0x11:
                        DSend[trackef] = (float) pltr_dat_row[k] / 255.0f;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_SEND_TO_REVERB;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SETDISTORTIONTHRESHOLD)
                    // $12 Set distortion Threshold
                    case 0x12:
                        DThreshold[trackef] = (float) pltr_dat_row[k] * 128.0f;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_DISTO_THRESHOLD;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SETDISTORTIONCLAMP)
                    // $13 Set distortion clamp
                    case 0x13: 
                        DClamp[trackef] = (float) pltr_dat_row[k] * 128.0f;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_DISTO_CLAMP;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SETCUTOFF)
                    // $08 Set filter cutoff
                    case 0x8:
                        TCut[trackef] = (float) pltr_dat_row[k] / 2.0f;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_FILTER_CUTOFF;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SETFILTERRESONANCE)
                    // $14 Set filter resonance
                    case 0x14:
                        FRez[trackef] = (int) (pltr_dat_row[k] / 2);

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_FILTER_RESONANCE;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SWITCHFLANGER)
                    // $24 Switch flanger on/off
                    case 0x24:
                        FLANGER_ON[trackef] = (int) pltr_dat_row[k] & 1;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_FLANGER;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_TRACK_FILTER_LFO)
                    // $28 Switch track filter on/off
                    case 0x28:
                        LFO_ON[trackef] = (int) pltr_dat_row[k] & 1;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_FX_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_LFO;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SETFILTERTYPE)
                    // $15 Set filter Type
                    case 0x15:
                        if(pltr_dat_row[k] <= MAX_FILTER)
                        {
                            FType[trackef] = (int) pltr_dat_row[k];

#if !defined(__STAND_ALONE__)
                            if(userscreen == USER_SCREEN_TRACK_EDIT)
                            {
                                gui_action_external |= GUI_UPDATE_EXTERNAL_FILTER_TYPE;
                            }
#endif
                        
                        }
                        break;
#endif

#if defined(PTK_FX_SETCHORUSFILTERTYPE)
                    // $2d Set chorus filter Type
                    case 0x2d:
                        if(pltr_dat_row[k] <= MAX_CHORUS_FILTER)
                        {
                            ChorType = (int) pltr_dat_row[k];

#if !defined(__STAND_ALONE__)
                            if(userscreen == USER_SCREEN_TRACK_EDIT)
                            {
                                gui_action_external |= GUI_UPDATE_EXTERNAL_CHORUS_FILTER_TYPE;
                            }
#endif
                        
                        }
                        break;
#endif

#if defined(PTK_FX_SETCHORUSCUTOFF)
                    // $2e Set chorus filter cutoff
                    case 0x2e:
                        ChorCut = (int) (pltr_dat_row[k] / 2);

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_CHORUS_FILTER_CUTOFF;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SETCHORUSRESONANCE)
                    // $2f Set chorus filter resonance
                    case 0x2f:
                        ChorRez = (int) (pltr_dat_row[k] / 2);

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_FX_SETUP_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_CHORUS_FILTER_RESONANCE;
                        }
#endif

                        break;
#endif

#if defined(PTK_FX_SETDENOISE)
                    // $30 Set chorus filter resonance
                    case 0x30:
                        if(pltr_dat_row[k] > 4)
                        {
                            Track_Denoise[trackef] = 4;
                        }
                        else
                        {
                            Track_Denoise[trackef] = pltr_dat_row[k];
                        }

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_DENOISE;
                        }
#endif

                        break;
#endif




                } // pltr_eff_row[k]

            } // PosInTick == 0

#endif

            // At all ticks

#if defined(PTK_FX_0)
            switch(pltr_eff_row[k])
            {

#if defined(PTK_FX_PITCHUP)
                // $01 Pitch Up
                case 0x1:

                    for(i = 0; i < Channels_Polyphony[trackef]; i++)
                    {
                        if(Vstep1[trackef][i] < 5.66029f)
                        {
                            Vstep1[trackef][i] += ((float) pltr_dat_row[k]) / 2048.0f;
                        }
                    }
            
                    break;
#endif  // PTK_FX_PITCHUP

#if defined(PTK_FX_PITCHDOWN)
                // $02 Pitch Down
                case 0x2:

                    for(i = 0; i < Channels_Polyphony[trackef]; i++)
                    {
                        if(Vstep1[trackef][i] > 0.00586236f)
                        {
                            Vstep1[trackef][i] -= ((float) pltr_dat_row[k]) / 2048.0f;
                        }
                    }
                    break;
#endif

#if defined(PTK_FX_TRANCESLICER)
                // $04 Trance slicer
                case 0x4:
                    if(Subicounter >= pltr_dat_row[k]) sp_Tvol_Mod[trackef] = 0;
                    break;
#endif

#if defined(PTK_FX_TRANCEGLIDER)
                // $05 Glider
                case 0x5:
                    if(pltr_dat_row[k])
                    {
                        Glide_Step[trackef] = ((float) pltr_dat_row[k]) / 2048.0f;
                    }

                    for(i = 0; i < Channels_Polyphony[trackef]; i++)
                    {
                        if(Vstep1[trackef][i] < sp_Step[trackef][i])
                        {
                            Vstep1[trackef][i] += Glide_Step[trackef];
                            if(Vstep1[trackef][i] > sp_Step[trackef][i])
                            {
                                Vstep1[trackef][i] = sp_Step[trackef][i];
                            }
                        }
                        else
                        {
                            if(Vstep1[trackef][i] > sp_Step[trackef][i])
                            {
                                Vstep1[trackef][i] -= Glide_Step[trackef];
                                if(Vstep1[trackef][i] < sp_Step[trackef][i])
                                {
                                    Vstep1[trackef][i] = sp_Step[trackef][i];
                                }
                            }
                        }
                    }
                    break;
#endif

            } // SWITCH CASES

#endif  // PTK_FX_0

#if defined(PTK_FX_X)
            switch(pltr_eff_row[k])
            {

#if !defined(__STAND_ALONE__)
                // $07 FX
                case 0x7:
                    // ... nothing ...
                    break;
#endif

#if defined(PTK_FX_SETRANDOMCUTOFF)
                // $0a SetRandomCutOff
                case 0xa:
                    if(Subicounter == 0)
                    {
                        TCut[trackef] = float((float) (rand() & 0xff) * ((float) (pltr_dat_row[k] / 255.0f)));

                        if(TCut[trackef] < 1) TCut[trackef] = 1;
                        if(TCut[trackef] > 127) TCut[trackef] = 127;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_FILTER_CUTOFF;
                        }
#endif
                    
                    }
                    break;
#endif

#if defined(PTK_FX_SLIDEUPCUTOFF)
                // $0b SlideUpCutOff 
                case 0xb:
                    if(Subicounter == 0)
                    {
                        TCut[trackef] += pltr_dat_row[k];
                        if(TCut[trackef] > 127) TCut[trackef] = 127;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_FILTER_CUTOFF;
                        }
#endif

                    }
                    break;
#endif

#if defined(PTK_FX_SLIDEDOWNCUTOFF)
                // $0c SlideDownCutOff 
                case 0xc:
                    if(Subicounter == 0)
                    {
                        TCut[trackef] -= pltr_dat_row[k];
                        if(TCut[trackef] < 1) TCut[trackef] = 1;

#if !defined(__STAND_ALONE__)
                        if(userscreen == USER_SCREEN_TRACK_EDIT)
                        {
                            gui_action_external |= GUI_UPDATE_EXTERNAL_FILTER_CUTOFF;
                        }
#endif

                    }
                    break;
#endif

#if defined(PTK_FX_NOTERETRIGGER)
                // $0e Note retrigger
                case 0xe: 

                    int free_sub_channel;
                
                    // No new note
                    for(i = 0; i < Channels_Polyphony[trackef]; i++)
                    {
                        if(pltr_note[i] == 121) pltr_note[i] = old_note[trackef][i];
                        old_note[trackef][i] = pltr_note[i];
                    }

                    if(pltr_dat_row[k] > 0 && (Subicounter % pltr_dat_row[k]) == 0)
                    {
                        for(i = 0; i < Channels_MultiNotes[trackef]; i++)
                        {
                            if(pl_note[i] < 120 && Note_Sub_Channels[trackef][i] != -1)
                            {
                                j = Reserved_Sub_Channels[trackef][i];

#if defined(PTK_INSTRUMENTS)
                                // Get the virtual channel it was playing on and remove it
                                if(sp_Stage[trackef][j] == PLAYING_SAMPLE)
                                {
                                    sp_Stage[trackef][j] = PLAYING_SAMPLE_NOTEOFF;
                                }
#endif

#if defined(PTK_SYNTH)
                                Synthesizer[trackef][j].Note_Off();
#endif

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
                                if(Midi_Current_Notes[Chan_Midi_Prg[trackef]][j])
                                {
                                    Midi_Note_Off(trackef, Midi_Current_Notes[Chan_Midi_Prg[trackef]][j]);
                                    Midi_Current_Notes[Chan_Midi_Prg[trackef]][j] = 0;
                                }
#endif
#endif
                            }
                        }

                        for(i = 0; i < Channels_MultiNotes[trackef]; i++)
                        {
                            free_sub_channel = Get_Free_Sub_Channel(trackef, Channels_Polyphony[trackef]);
                            if(free_sub_channel == -1) free_sub_channel = i;

                            // Mark it as playing
                            Note_Sub_Channels[trackef][i] = i;
                            Reserved_Sub_Channels[trackef][i] = free_sub_channel;
                            Schedule_Instrument(trackef,
                                                free_sub_channel,
                                                pltr_note[i],
                                                pltr_sample[i],
                                                0, 0,
                                                FALSE,
                                                i + 1,
                                                1.0f,
                                                Song_Position,
                                                Pattern_Line);
                        }
                    }
                    break;
#endif

#if defined(PTK_FX_AUTOFADEIN)
                // $17 Auto fade in xx ticks
                case 0x17:
                    if(pltr_dat_row[k] > 0)
                    {
                        FADECOEF[trackef] = 0.1666667f / (float) pltr_dat_row[k];
                        FADEMODE[trackef] = 1;
                    }
                    else
                    {
                        FADEMODE[trackef] = 0;
                    }
                    break;
#endif

#if defined(PTK_FX_AUTOFADEOUT)
                // $18 Auto fade out xx ticks
                case 0x18:
                    if(pltr_dat_row[k] > 0)
                    {
                        FADECOEF[trackef] = 0.1666667f / (float) pltr_dat_row[k];
                    }
                    else
                    {
                        FADEMODE[trackef] = 0;
                    }
                    break;
#endif

#if defined(PTK_FX_VOLUMESLIDEUP)
                // $19 Volume slide up
                case 0x19:
                    sp_Tvol_Mod[trackef] += pltr_dat_row[k] * 0.0039062f;
                    if(sp_Tvol_Mod[trackef] > 1.0f) sp_Tvol_Mod[trackef] = 1.0f;
                    break;
#endif

#if defined(PTK_FX_VOLUMESLIDEDOWN)
                // $1a Volume slide down
                case 0x1a: 
                    sp_Tvol_Mod[trackef] -= pltr_dat_row[k] * 0.0039062f;
                    if(sp_Tvol_Mod[trackef] < 0.0f) sp_Tvol_Mod[trackef] = 0.0f;
                    break;
#endif

#if defined(PTK_FX_SETGLOBALVOLUME)
                // $1a Set global volume
                case 0x1c:
                    local_mas_vol = pltr_dat_row[k] / 255.0f;
                    break;
#endif

        }

#endif  // PTK_FX_X

        } // Channels_Effects[trackef]

#if defined(PTK_FX_ARPEGGIO)
        // Let's do the arpeggio
        float arpnote;

        if(Arpeggio_Switch[trackef])
        {
            for(i = 0; i < Channels_Polyphony[trackef]; i++)
            {
                switch((Subicounter % 3))
                {
                    case 0:
                        Vstep1[trackef][i] = Vstep_arp[trackef][i];
                        break;
                    case 1:
                        arpnote = ((float) POWF2(((Arpeggio_BaseNote[trackef][i] + (Arpeggio_Switch[trackef] >> 4))) / 12.0f));
                        Vstep1[trackef][i] = arpnote;
                        break;
                    case 2:
                        arpnote = ((float) POWF2(((Arpeggio_BaseNote[trackef][i] + (Arpeggio_Switch[trackef] & 0xf))) / 12.0f));
                        Vstep1[trackef][i] = arpnote;
                        break;
                }
            }
        }
#endif

#if defined(PTK_FX_VIBRATO)
        // Let's do the vibrato
        float vibnote;
        float vib_speed;
        float vib_amp;

        if(Vibrato_Switch[trackef])
        {
            vib_speed = ((float) (Vibrato_Switch[trackef] >> 4)) * (float) Vibcounter[trackef];
            vib_speed = sinf((vib_speed * 0.00045f) / (PIf * 2.0f));
            vib_amp = (float) (Vibrato_Switch[trackef] & 0xf) * 0.09f;
            vib_speed *= vib_amp;

            for(i = 0; i < Channels_Polyphony[trackef]; i++)
            {
                vibnote = ((float) POWF2(((Vibrato_BaseNote[trackef][i] + vib_speed)) / 12.0f));
                Vstep1[trackef][i] = vibnote;
            }
        }
#endif

#if defined(PTK_FX_REVERSE)
        if(Reverse_Switch[trackef])
        {
            Reverse_Switch[trackef] = FALSE;
            if(Reserve_Dat[trackef])
            {
                for(i = 0; i < Channels_Polyphony[trackef]; i++)
                {
                    Instrument_Schedule_Dat[trackef][i].start_backward = TRUE;
                    if(sp_Stage[trackef][i] != PLAYING_NOSAMPLE || Cut_Stage[trackef][i])
                    {
                        Player_LW[trackef][i] = SMP_LOOPING_BACKWARD;
                    }
                    if(sp_Stage2[trackef][i] != PLAYING_NOSAMPLE || Cut_Stage[trackef][i])
                    {
                        Synthesizer[trackef][i].ENV_1_LOOP_BACKWARD = TRUE;
                        Synthesizer[trackef][i].ENV_3_LOOP_BACKWARD = TRUE;
                    }
                    if(sp_Stage3[trackef][i] != PLAYING_NOSAMPLE || Cut_Stage[trackef][i])
                    {
                        Synthesizer[trackef][i].ENV_2_LOOP_BACKWARD = TRUE;
                    }
                }
            }
            else
            {
                for(i = 0; i < Channels_Polyphony[trackef]; i++)
                {
                    Instrument_Schedule_Dat[trackef][i].start_backward = FALSE;
                    if(sp_Stage[trackef][i] != PLAYING_NOSAMPLE || Cut_Stage[trackef][i])
                    {
                        Player_LW[trackef][i] = SMP_LOOPING_FORWARD;
                    }
                    if(sp_Stage2[trackef][i] != PLAYING_NOSAMPLE || Cut_Stage[trackef][i]) 
                    {
                        Synthesizer[trackef][i].ENV_1_LOOP_BACKWARD = FALSE;
                        Synthesizer[trackef][i].ENV_3_LOOP_BACKWARD = FALSE;
                    }
                    if(sp_Stage3[trackef][i] != PLAYING_NOSAMPLE || Cut_Stage[trackef][i]) 
                    {
                        Synthesizer[trackef][i].ENV_2_LOOP_BACKWARD = FALSE;
                    }
                }
            }
        }
#endif

    }// FOR LOOP ON TRACKS
}

// ------------------------------------------------------
// Reset the chorus filters
void Reset_Chorus_Filters(void)
{
    fx1_chorus[0] = 0.0f;
    fx2_chorus[0] = 0.0f;
    fy1_chorus[0] = 0.0f;
    fy2_chorus[0] = 0.0f;
    fx1_chorus[1] = 0.0f;
    fx2_chorus[1] = 0.0f;
    fy1_chorus[1] = 0.0f;
    fy2_chorus[1] = 0.0f;
}

// ------------------------------------------------------
// Reset the tracks filters
void Reset_Filters(int tr)
{
    buf024[0][tr] = 0.0f;
    buf124[0][tr] = 0.0f;
    buf0[0][tr] = 0.0f;
    buf1[0][tr] = 0.0f;
    fx1[0][tr] = 0.0f;
    fx2[0][tr] = 0.0f;
    fy1[0][tr] = 0.0f;
    fy2[0][tr] = 0.0f;
    xi0[0][tr] = 0.0f;
    xi1[0][tr] = 0.0f;
    xi2[0][tr] = 0.0f;
    
    buf024[1][tr] = 0.0f;
    buf124[1][tr] = 0.0f;
    buf0[1][tr] = 0.0f;
    buf1[1][tr] = 0.0f;
    fx1[1][tr] = 0.0f;
    fx2[1][tr] = 0.0f;
    fy1[1][tr] = 0.0f;
    fy2[1][tr] = 0.0f;
    xi0[1][tr] = 0.0f;
    xi1[1][tr] = 0.0f;
    xi2[1][tr] = 0.0f;
}

// ------------------------------------------------------
// Process track filter LFO
float Apply_Lfo_To_Filter(float value, int channel)
{

#if defined(PTK_LFO)
    if(LFO_ON[channel] == 1)
    {
        if(LFO_AMPL_FILTER[channel] != 0.0f)
        {
            value += ((SIN[(int) (LFO_CARRIER_FILTER[channel])] + 1.0f) * 0.5f) * LFO_AMPL_FILTER[channel];
        }
        LFO_CARRIER_FILTER[channel] += LFO_RATE[channel] * LFO_RATE_SCALE[channel];
        if(LFO_CARRIER_FILTER[channel] >= 360.0f) LFO_CARRIER_FILTER[channel] -= 360.0f;
    }
#endif

    if(value < 1.0f) value = 1.0f;
    if(value > 126.0f) value = 126.0f;
    return value;
}

// ------------------------------------------------------
// Process track volume LFO
#if defined(PTK_LFO)
float Apply_Lfo_To_Volume(int channel)
{
    float temp_value;
    float value = 1.0f;

    if(LFO_ON[channel] == 1)
    {
        if(LFO_AMPL_VOLUME[channel] != 0.0f)
        {
            temp_value = ((SIN[(int) (LFO_CARRIER_VOLUME[channel])] + 1.0f) * 0.5f) * LFO_AMPL_VOLUME[channel];
            temp_value /= 128.0f;
            value = 1.0f - temp_value;
        }
        LFO_CARRIER_VOLUME[channel] += LFO_RATE[channel] * LFO_RATE_SCALE[channel];
        if(LFO_CARRIER_VOLUME[channel] >= 360.0f) LFO_CARRIER_VOLUME[channel] -= 360.0f;
    }

    if(value < 0.0f) value = 0.0f;
    if(value > 1.0f) value = 1.0f;
    return value;
}
#endif

// ------------------------------------------------------
// Process track panning LFO
#if defined(PTK_LFO)
float Apply_Lfo_To_Panning(float value, int channel)
{
    float temp_value;

    // [-0.5f..0.5f]
    value -= 0.5f;
    // [-1.0f..1.0f]
    value *= 2.0f;
    temp_value = SIN[(int) (LFO_CARRIER_PANNING[channel])] * LFO_AMPL_PANNING[channel];
    temp_value /= 128.0f;
    value *= temp_value;
    // [-0.5f..0.5f]
    value *= 0.5f;
    // [0.0f..1.0f]
    value += 0.5f;

    if(value < 0.0f) value = 0.0f;
    if(value > 1.0f) value = 1.0f;
    return value;
}
#endif

// ------------------------------------------------------
// Set stereo panning only when necessary
void Compute_Stereo_Quick(int channel)
{
    float pan_value;
    int changed = FALSE;

    pan_value = TPan[channel];
    if(old_TPan[channel] != TPan[channel])
    {
        old_TPan[channel] = TPan[channel];
        changed = TRUE;
    }

#if defined(PTK_LFO)
    if(LFO_ON[channel] == 1)
    {
        // Check if not 'Off'
        if(LFO_AMPL_PANNING[channel] != 0.0f)
        {
            pan_value = Apply_Lfo_To_Panning(pan_value, channel);
            LFO_CARRIER_PANNING[channel] += LFO_RATE[channel] * LFO_RATE_SCALE[channel];
            if(LFO_CARRIER_PANNING[channel] >= 360.0f) LFO_CARRIER_PANNING[channel] -= 360.0f;
            changed = TRUE;
        }
    }
#endif

    if(changed)
    {
        Old_LVol[channel] = SQRT[(int) ((1.0f - pan_value) * 1024.0f)];
        Old_RVol[channel] = SQRT[(int) (pan_value * 1024.0f)];
    }

}

// ------------------------------------------------------
// Set stereo panning
void Compute_Stereo(int channel)
{
    Old_LVol[channel] = SQRT[(int) ((1.0f - TPan[channel]) * 1024.0f)];
    Old_RVol[channel] = SQRT[(int) (TPan[channel] * 1024.0f)];
}

// ------------------------------------------------------
// Bring stereo panning up to date
void Fix_Stereo(int channel)
{
    LVol[channel] = Old_LVol[channel];
    RVol[channel] = Old_RVol[channel];
}

// ------------------------------------------------------
// Main mixing routine
void Get_Player_Values(void)
{
#if !defined(__STAND_ALONE__)
    int c;
#endif
    
    float left_compress;
    float right_compress;

    left_chorus = 0.0f;
    right_chorus = 0.0f;
    left_reverb = 0.0f;
    right_reverb = 0.0f;
    left_compress = 1.0f;
    right_compress = 1.0f;
    left_float = 0.0f;
    right_float = 0.0f;

#if defined(PTK_LIMITER_MASTER) || defined(PTK_LIMITER_TRACKS)
    mas_comp_pos_rms_buffer++;
    if(mas_comp_pos_rms_buffer > MAS_COMPRESSOR_SIZE - 1) mas_comp_pos_rms_buffer = 0;
#endif

    Sp_Player();

    // Wait for the audio latency before starting to play
    if(Song_Playing)
    {
        if(!Song_Playing_Pattern)
        {
            int Max_Latency = AUDIO_Latency;
            if(AUDIO_GetSamples() >= Max_Latency)
            {
                // Start from the top of the buffer
                Delay_Sound_Buffer = 0;
                PosInTick_Delay = 0;
                Song_Playing_Pattern = TRUE;
            }
        }
    }

    if(++lchorus_counter > (MIX_RATE * 2)) lchorus_counter = MIX_RATE;
    if(++rchorus_counter > (MIX_RATE * 2)) rchorus_counter = MIX_RATE;
    lbuff_chorus[lchorus_counter] = left_chorus + lbuff_chorus[lchorus_counter2] * lchorus_feedback;
    rbuff_chorus[rchorus_counter] = right_chorus + rbuff_chorus[rchorus_counter2] * rchorus_feedback;
    if(++lchorus_counter2 > (MIX_RATE * 2)) lchorus_counter2 = MIX_RATE;
    if(++rchorus_counter2 > (MIX_RATE * 2)) rchorus_counter2 = MIX_RATE;

#if DENORMAL
    lbuff_chorus[lchorus_counter2] = denormal(lbuff_chorus[lchorus_counter2]);
    rbuff_chorus[lchorus_counter2] = denormal(rbuff_chorus[lchorus_counter2]);
#endif

    float rchore = lbuff_chorus[lchorus_counter2];
    float lchore = rbuff_chorus[rchorus_counter2];

#if defined(PTK_PROC_FILTER_CHORUS)
    if(ChorType != 4)
    {
        coef_chorus[0] = coeftab[0][ChorCut][ChorRez][ChorType];
        coef_chorus[1] = coeftab[1][ChorCut][ChorRez][ChorType];
        coef_chorus[2] = coeftab[2][ChorCut][ChorRez][ChorType];
        coef_chorus[3] = coeftab[3][ChorCut][ChorRez][ChorType];
        coef_chorus[4] = coeftab[4][ChorCut][ChorRez][ChorType];

        lchore = Filter_Chorus(0, lchore + 1.0f);
        rchore = Filter_Chorus(1, rchore + 1.0f);
    }
#endif
    
    left_float += lchore;
    right_float += rchore;

#if defined(PTK_COMPRESSOR)
    Reverb_work();
    left_float += left_reverb;
    right_float += right_reverb;
#endif

    left_float /= 32767.0f;
    right_float /= 32767.0f;

#if DENORMAL
    left_float = denormal(left_float);
    right_float = denormal(right_float);
#endif
    
#if defined(PTK_LIMITER_MASTER)
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
    if(mas_comp_ratio_Master > 0.01f)
    {
#else
    if(mas_ratio_Master > 0.01f)
    {
#endif
        left_compress = Mas_Compressor_Master(left_float, &rms_sumL_Master, mas_comp_bufferL_Master, &mas_envL_Master);
        right_compress = Mas_Compressor_Master(right_float, &rms_sumR_Master, mas_comp_bufferR_Master, &mas_envR_Master);
        left_float *= left_compress; 
        right_float *= right_compress; 
    }
#endif

    left_float *= mas_vol;
    right_float *= mas_vol;

    if(local_curr_mas_vol >= local_mas_vol)
    {
        local_curr_mas_vol -= 0.003f;
        if(local_curr_mas_vol < 0.0f) local_curr_mas_vol = 0.0f;
    }
    else
    {
        local_curr_mas_vol += 0.003f;
        if(local_curr_mas_vol > 1.0f) local_curr_mas_vol = 1.0f;
    }

    left_float *= local_curr_mas_vol;
    right_float *= local_curr_mas_vol;

    if(local_curr_ramp_vol >= local_ramp_vol)
    {
        local_curr_ramp_vol -= 0.003f;
        if(local_curr_ramp_vol < 0.0f) local_curr_ramp_vol = 0.0f;
    }
    else
    {
        local_curr_ramp_vol += 0.003f;
        if(local_curr_ramp_vol > 1.0f) local_curr_ramp_vol = 1.0f;
    }

    left_float *= local_curr_ramp_vol;
    right_float *= local_curr_ramp_vol;

    if(left_float > 1.0f) left_float = 1.0f;
    if(left_float < -1.0f) left_float = -1.0f;
    if(right_float > 1.0f) right_float = 1.0f;
    if(right_float < -1.0f) right_float = -1.0f;

#if !defined(__STAND_ALONE__)
    left_float_render = left_float;
    right_float_render = right_float;
#endif

#if !defined(__STAND_ALONE__)

    // Store the data for the tracks scopes now
    for(c = 0; c < Song_Tracks; c++)
    {
        if(!Chan_Mute_State[c])
        {
            VuMeters_Dats_L[c][pos_scope] = ((((VuMeters_Dats_L[c][pos_scope]
                                            ) * left_compress
                                            ) * mas_vol
                                            ) * local_curr_mas_vol
                                            ) * local_curr_ramp_vol;

            VuMeters_Dats_R[c][pos_scope] = ((((VuMeters_Dats_R[c][pos_scope]
                                            ) * right_compress
                                            ) * mas_vol
                                            ) * local_curr_mas_vol
                                            ) * local_curr_ramp_vol;

            Scope_Dats[c][pos_scope] = (VuMeters_Dats_L[c][pos_scope] + 
                                        VuMeters_Dats_R[c][pos_scope]) * 1.2f;
            
            if(absf(VuMeters_Dats_L[c][pos_scope]) > VuMeters_Level_Dats_L[c])
            {
                VuMeters_Level_Dats_L[c] = absf(VuMeters_Dats_L[c][pos_scope]);
            }
            if(absf(VuMeters_Dats_R[c][pos_scope]) > VuMeters_Level_Dats_R[c])
            {
                VuMeters_Level_Dats_R[c] = absf(VuMeters_Dats_R[c][pos_scope]);
            }
            if(VuMeters_Level_Dats_L[c] > 1.0f)
            {
                VuMeters_Level_Dats_L[c] = 1.0f;
            }
            if(VuMeters_Level_Dats_R[c] > 1.0f)
            {
                VuMeters_Level_Dats_R[c] = 1.0f;
            }
        }
        else
        {
            VuMeters_Dats_L[c][pos_scope] = 0.0f;
            VuMeters_Dats_R[c][pos_scope] = 0.0f;
            VuMeters_Level_Dats_L[c] = 0.0f;
            VuMeters_Level_Dats_R[c] = 0.0f;
            Scope_Dats[c][pos_scope] = 0.0f;
        }
        if(VuMeters_Level_Dats_L[c] > 0.0f) 
        {
            VuMeters_Level_Dats_L[c] -= 0.000025f;
        }
        else
        {
            VuMeters_Level_Dats_L[c] = 0.0f;
        }
        if(VuMeters_Level_Dats_R[c] > 0.0f) 
        {
            VuMeters_Level_Dats_R[c] -= 0.000025f;
        }
        else
        {
            VuMeters_Level_Dats_R[c] = 0.0f;
        }
    }
#endif

}

// ------------------------------------------------------
// Precalc filters coefficients
#if defined(PTK_FILTER_LOHIBAND)
void ComputeCoefs(int freq, int r, int t)
{
    float omega = float(2 * PI * Cutoff(freq) / fMIX_RATE);
    float sn = (float) sinf(omega);
    float cs = (float) cosf(omega);
    float alpha;
    float a0;
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;

    if(t < 2) alpha = float(sn / Resonance(r * (freq + 70.0f) / (127.0f + 70.0f)));
    else alpha = float(sn * sinhf(Bandwidth(r) * omega / sn));

    switch(t)
    {
        default:
        case 0: // LP
            b0 =  (1.0f - cs) / 2.0f;
            b1 =   1.0f - cs;
            b2 =  (1.0f - cs) / 2.0f;
            a0 =   1.0f + alpha;
            a1 =  -2.0f * cs;
            a2 =   1.0f - alpha;
            break;

        case 1: // HP
            b0 =  (1.0f + cs) / 2.0f;
            b1 = -(1.0f + cs);
            b2 =  (1.0f + cs) / 2.0f;
            a0 =   1.0f + alpha;
            a1 =  -2.0f * cs;
            a2 =   1.0f - alpha;
            break;

        case 2: // BP
            b0 =   alpha;
            b1 =   0.0f;
            b2 =  -alpha;
            a0 =   1.0f + alpha;
            a1 =  -2.0f * cs;
            a2 =   1.0f - alpha;
            break;

        case 3: // BR
            b0 =   1.0f;
            b1 =  -2.0f * cs;
            b2 =   1.0f;
            a0 =   1.0f + alpha;
            a1 =  -2.0f * cs;
            a2 =   1.0f - alpha;
            break;
    }

    coef[0] = b0 / a0;
    coef[1] = b1 / a0;
    coef[2] = b2 / a0;
    coef[3] = -a1 / a0;
    coef[4] = -a2 / a0;
}
#endif

// ------------------------------------------------------
// Filters run
#if defined(PTK_PROC_FILTER_CHORUS)
float Filter_Chorus(int stereo, float x)
{
    float y;

    y = coef_chorus[0] * x +
        coef_chorus[1] * fx1_chorus[stereo] +
        coef_chorus[2] * fx2_chorus[stereo] +
        coef_chorus[3] * fy1_chorus[stereo] +
        coef_chorus[4] * fy2_chorus[stereo];
    fy2_chorus[stereo] = fy1_chorus[stereo];
    fy1_chorus[stereo] = y;
    fx2_chorus[stereo] = fx1_chorus[stereo];
    fx1_chorus[stereo] = x;
    return y;
}
#endif

#if defined(PTK_PROC_FILTER)
float Filter(int stereo, float x, char i)
{
    float y;

    y = coef[0] * x +
        coef[1] * fx1[stereo][i] +
        coef[2] * fx2[stereo][i] +
        coef[3] * fy1[stereo][i] +
        coef[4] * fy2[stereo][i];
    fy2[stereo][i] = fy1[stereo][i];
    fy1[stereo][i] = y;
    fx2[stereo][i] = fx1[stereo][i];
    fx1[stereo][i] = x;
    return y;
}
#endif

float Cutoff(int v)
{
    return POWF((v + 5.0f) / (127.0f + 5.0f), 1.7f) * 13000.0f + 30.0f;
}

float Resonance(float v)
{
    return POWF(v / 127.0f, 4.0f) * 150.0f + 0.1f;
}

float Bandwidth(int v)
{
    return POWF(v / 127.0f, 4.0f) * 4.0f + 0.1f;
}

#if defined(PTK_PROC_FILTER2P)
float filter2p(int stereo, int ch, float input, float f, float q)
{
    f *= 0.0078125f;
    q *= 0.0078125f;

    float fa = float(1.0f - f); 
    float fb = float(q * (1.0f + (1.0f / fa)));
    buf0[stereo][ch] = fa * buf0[stereo][ch] + f * (input + fb * (buf0[stereo][ch] - buf1[stereo][ch])); 
    buf1[stereo][ch] = fa * buf1[stereo][ch] + f * buf0[stereo][ch];
#if DENORMAL
    buf0[stereo][ch] = denormal(buf0[stereo][ch]);
    buf1[stereo][ch] = denormal(buf1[stereo][ch]);
#endif
    return buf1[stereo][ch];  
}
#endif

#if defined(PTK_PROC_FILTERHP2)
float filterhp2(int stereo, int ch, float input, float f, float q)
{
    f *= 0.0078125f;
    q *= 0.0078125f;

    float fa = float(1.0f - f);
    float fb = float(q * (1.0f + (1.0f / fa)));
    buf024[stereo][ch] = fa * buf024[stereo][ch] + f * (input + fb * (buf024[stereo][ch] - buf124[stereo][ch])); 
    buf124[stereo][ch] = fa * buf124[stereo][ch] + f * buf024[stereo][ch];
#if DENORMAL
    buf024[stereo][ch] = denormal(buf024[stereo][ch]);
    buf124[stereo][ch] = denormal(buf124[stereo][ch]);
#endif
    return input - buf124[stereo][ch];
}
#endif

#if defined(PTK_PROC_FILTERHP)
float filterhp(int stereo, int ch, float input, float f, float q)
{
    f *= 0.0078125f;
    q *= 0.0078125f;

    float fa = float(1.0f - f); 
    float fb = float(q * (1.0f + (1.0f / fa)));
    buf0[stereo][ch] = fa * buf0[stereo][ch] + f * (input + fb * (buf0[stereo][ch] - buf1[stereo][ch])); 
    buf1[stereo][ch] = fa * buf1[stereo][ch] + f * buf0[stereo][ch];
#if DENORMAL
    buf0[stereo][ch] = denormal(buf0[stereo][ch]);
    buf1[stereo][ch] = denormal(buf1[stereo][ch]);
#endif
    return input - buf1[stereo][ch];
}
#endif

#if defined(PTK_PROC_FILTER2P24D)
float filter2p24d(int stereo, int ch, float input, float f, float q)
{
    f *= 0.0078125f;
    q *= 0.0078125f;
    float fa = float(1.0f - f); 
    float fb = float(q * (1.0f + (1.0f / fa)));
    buf024[stereo][ch] = fa * buf024[stereo][ch] + f * (input + fb * (buf024[stereo][ch] - buf124[stereo][ch])); 
    buf124[stereo][ch] = fa * buf124[stereo][ch] + f * buf024[stereo][ch];
#if DENORMAL
    buf024[stereo][ch] = denormal(buf024[stereo][ch]);
    buf124[stereo][ch] = denormal(buf124[stereo][ch]);
#endif
    return buf124[stereo][ch];  
}
#endif

#if defined(PTK_PROC_FILTERRINGMOD)
float filterRingMod(int stereo, int ch, float input, float f, float q)
{
    q++;

    f *= 0.0078125f;
    buf0[stereo][ch] += f * (q * 0.125f);
    if(buf0[stereo][ch] >= 360.0f) buf0[stereo][ch] -= 360.0f;

    return input * SIN[(int) buf0[stereo][ch]];
}
#endif

#if defined(PTK_PROC_FILTERRINGMODSTEREO)
float filterRingModStereo(int stereo, int ch, float input)
{
    return input * cosf(buf0[stereo][ch] * 0.0174532f);
}
#endif

#if defined(PTK_PROC_FILTERWATER)
float filterWater(int stereo, int ch, float input, float f)
{
    f = 127.0f - f;
    float ad = input - buf0[stereo][ch];

    if(ad > 1.0f || ad < -1.0f) buf0[stereo][ch] += ad / f;
#if DENORMAL
    buf0[stereo][ch] = denormal(buf0[stereo][ch]);
#endif
   return buf0[stereo][ch];
}
#endif

#if defined(PTK_PROC_FILTERWATERSTEREO)
float filterWaterStereo(int stereo, int ch, float input, float f)
{
    f = 127.0f - f;
    float ad = input - buf1[stereo][ch];

    if(ad > 1.0f || ad < -1.0f) buf1[stereo][ch] += ad / f;
#if DENORMAL
    buf1[stereo][ch] = denormal(buf1[stereo][ch]);
#endif
    return buf1[stereo][ch];
}
#endif

#if defined(PTK_PROC_FILTERDELTA)
float filterDelta(int stereo, int ch, float input, float f, float q)
{
    f = 127.0f - f;
    q *= 0.007874f;

    float output = buf1[stereo][ch];
    if(buf1[stereo][ch] > 1.0f || buf1[stereo][ch] < -1.0f) buf1[stereo][ch] *= q;
#if DENORMAL
    buf1[stereo][ch] = denormal(buf1[stereo][ch]);
#endif
    buf0[stereo][ch]++;
    if(buf0[stereo][ch] >= f)
    {
        buf0[stereo][ch] = 0.0f;
        output = input;
        buf1[stereo][ch] = input;
    }
    return output;
}
#endif

/*float filterDeltaStereo(int stereo, int ch, float input, float f, float q)
{
    f = 127.0f - f;
    q *= 0.007874f;

    float output = buf124[stereo][ch];
    if(buf124[stereo][ch] > 1.0f || buf124[stereo][ch] < -1.0f) buf124[stereo][ch] *= q;

    buf024[ch]++;
    if(buf024[ch] >= f)
    {
        buf024[ch] = 0.0f;
        output = input;
        buf124[stereo][ch] = input;
    }
    return output;
}
*/

#if defined(PTK_PROC_FILTERBELLSHAPED)
float filterBellShaped(int stereo, int ch, float input, float f, float q, float g)
{
    input++;
    q *= 0.007874f;

    if(q < 0.01f) q = 0.01f;
    float freq = 320.0f + (f * 127.65625f);
    float a, b, c;
    float a0, a1, a2, b1, b2;              // filter coefficients
    float Wn, Wp;
    float gain = g / 6.6f;
    if(freq > 22100.0f) freq = 22100.0f;   // apply Nyquist frequency
    Wn = 1.0f / (6.2831853f * freq);       // freq of center
#if DENORMAL
    Wn = denormal(Wn);
#endif
    Wp = float(Wn / tanf(Wn / 88200.0f));  // prewarped frequency
#if DENORMAL
    Wp = denormal(Wp);
#endif
    a = (Wn * Wn * Wp * Wp);
    float t1 = Wn * Wp * q;
    b = (3.0f + gain) * t1;
    c = (3.0f - gain) * t1;
    t1 = a + c + 1.0f;
#if DENORMAL
    t1 = denormal(t1);
#endif
    b2 = (1.0f - c + a) / t1;
    a2 = (1.0f - b + a) / t1;
    b1 = a1 = 2.0f * (1.0f - a) / t1;
    a0 = (a + b + 1.0f) / t1;
#if DENORMAL
    b1 = denormal(b1);
    b2 = denormal(b2);
    a0 = denormal(a0);
    a1 = denormal(a1);
    a2 = denormal(a2);
#endif
    xi0[stereo][ch] = input - b1 * xi1[stereo][ch] - b2 * xi2[stereo][ch];
    float output = a0 * xi0[stereo][ch] + a1 * xi1[stereo][ch] + a2 * xi2[stereo][ch];
    xi2[stereo][ch] = xi1[stereo][ch];
    xi1[stereo][ch] = xi0[stereo][ch];
    return output;
}
#endif

#if defined(PTK_PROC_FILTERINT2P)
float int_filter2p(int stereo, int ch, float input, float f, float q, float q2)
{
    q *= 0.0787401f;
    input = filter2px(stereo, ch, input, f, q2);
    return float(32767.0f * POWF(absf(input) / 32767.0f, 1.0f - q / 11.0f));
}

float filter2px(int stereo, int ch, float input, float f, float q)
{
    f *= 0.0078125f;
    float fa = float(1.0f - f);
    float fb = float(q * (1.0f + (1.0f / fa)));
    buf0[stereo][ch] = fa * buf0[stereo][ch] + f * (input + fb * (buf0[stereo][ch] - buf1[stereo][ch]));
    buf1[stereo][ch] = fa * buf1[stereo][ch] + f * buf0[stereo][ch];
#if DENORMAL
    buf0[stereo][ch] = denormal(buf0[stereo][ch]);
    buf1[stereo][ch] = denormal(buf1[stereo][ch]);
#endif
    return buf1[stereo][ch];
}
#endif

// ------------------------------------------------------
// Record the 303 parameters changes

#if defined(PTK_303)
void live303(int pltr_eff_row, int pltr_dat_row)
{

#if !defined(__STAND_ALONE__)
    int change_303_unit = 0;
    int change_303_param = 0;
#endif

    switch(pltr_eff_row)
    {
        case 0x33:
            tb303[0].cutoff = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_1_CUTOFF;
#endif

            break;
        case 0x34:
            tb303[1].cutoff = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_2_CUTOFF;
#endif

            break;
        case 0x35:
            tb303[0].resonance = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_1_RESONANCE;
#endif

            break;
        case 0x36:
            tb303[1].resonance = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_2_RESONANCE;
#endif

            break;
        case 0x37:
            tb303[0].envmod = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_1_ENVMOD;
#endif

            break;
        case 0x38:
            tb303[1].envmod = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_2_ENVMOD;
#endif

            break;
        case 0x39:
            tb303[0].decay = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_1_DECAY;
#endif

            break;
        case 0x3a:
            tb303[1].decay = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_2_DECAY;
#endif

            break;
        case 0x3b:
            tb303[0].accent = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_1_ACCENT;
#endif

            break;
        case 0x3c:
            tb303[1].accent = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_2_ACCENT;
#endif

            break;
        case 0x3d:
            tb303[0].tune = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_1_TUNE;
#endif

            break;
        case 0x3e:
            tb303[1].tune = pltr_dat_row / 2;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_2_TUNE;
#endif

            break;
        case 0x3f: 
            if(pltr_dat_row < 1) pltr_dat_row = 1;
            if(pltr_dat_row > 16) pltr_dat_row = 16;
            tb303[0].scale = pltr_dat_row;
            tb303engine[0].tbCurMultiple = tb303[0].scale;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_1_SCALE;
#endif

            break;
        case 0x40:
            if(pltr_dat_row < 1) pltr_dat_row = 1;
            if(pltr_dat_row > 16) pltr_dat_row = 16;
            tb303[1].scale = pltr_dat_row;
            tb303engine[1].tbCurMultiple = tb303[1].scale;

#if !defined(__STAND_ALONE__)
            gui_action_external_303 = GUI_UPDATE_EXTERNAL_303_2_SCALE;
#endif

            break;
        case 0x41:
            tb303engine[0].tbTargetRealVolume = ((float) pltr_dat_row) / 255.0f;
            break;
        case 0x42:
            tb303engine[1].tbTargetRealVolume = ((float) pltr_dat_row) / 255.0f;
            break;
    }
}

void Fire303(unsigned char number, int unit)
{
    tb303engine[unit].tbLine = 0;
    tb303engine[unit].Note_Off = FALSE;

    switch(number)
    {
        case 0x00:  tb303engine[unit].tbPattern = tb303[unit].selectedpattern;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa1:  tb303engine[unit].tbPattern = 0;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa2:  tb303engine[unit].tbPattern = 1;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa3:  tb303engine[unit].tbPattern = 2;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa4:  tb303engine[unit].tbPattern = 3;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa5:  tb303engine[unit].tbPattern = 4;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa6:  tb303engine[unit].tbPattern = 5;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa7:  tb303engine[unit].tbPattern = 6;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xa8:  tb303engine[unit].tbPattern = 7;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb1:  tb303engine[unit].tbPattern = 8;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb2:  tb303engine[unit].tbPattern = 9;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb3:  tb303engine[unit].tbPattern = 10;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb4:  tb303engine[unit].tbPattern = 11;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb5:  tb303engine[unit].tbPattern = 12;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb6:  tb303engine[unit].tbPattern = 13;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb7:  tb303engine[unit].tbPattern = 14;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xb8:  tb303engine[unit].tbPattern = 15;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc1:  tb303engine[unit].tbPattern = 16;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc2:  tb303engine[unit].tbPattern = 17;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc3:  tb303engine[unit].tbPattern = 18;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc4:  tb303engine[unit].tbPattern = 19;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc5:  tb303engine[unit].tbPattern = 20;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc6:  tb303engine[unit].tbPattern = 21;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc7:  tb303engine[unit].tbPattern = 22;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xc8:  tb303engine[unit].tbPattern = 23;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd1:  tb303engine[unit].tbPattern = 24;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd2:  tb303engine[unit].tbPattern = 25;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd3:  tb303engine[unit].tbPattern = 26;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd4:  tb303engine[unit].tbPattern = 27;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd5:  tb303engine[unit].tbPattern = 28;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd6:  tb303engine[unit].tbPattern = 29;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd7:  tb303engine[unit].tbPattern = 30;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        case 0xd8:  tb303engine[unit].tbPattern = 31;
                    tb303engine[unit].RampVolume = 1.0f;
                    tb303engine[unit].tbTargetRealVolume = 1.0f;
                    tb303engine[unit].tbCurMultiple = tb303[unit].scale;
                    break;
        default:
            // No Fire
            tb303engine[unit].tbLine = 255;
            tb303engine[unit].RampVolume = 0.0f;
            tb303engine[unit].tbTargetRealVolume = 0.0f;
            break;
    }
}

void Note_Off_303(char strack)
{
    if(strack == track3031)
    {
        tb303engine[0].RampVolume = 0.0f;
        tb303engine[0].Note_Off = TRUE;
    }
    if(strack == track3032)
    { 
        tb303engine[1].RampVolume = 0.0f;
        tb303engine[1].Note_Off = TRUE;
    }
}

void Go303()
{
    if(tb303engine[0].tbPattern != 255)
    {
        tb303engine[0].tbCurMultiple++;
        if(tb303engine[0].tbCurMultiple >= tb303[0].scale)
        {
            tb303engine[0].tbCurMultiple = 0;
            if(!tb303engine[0].Note_Off)
            {
                tb303engine[0].tbNoteOn(tb303[0].tone[tb303engine[0].tbPattern][tb303engine[0].tbLine], &tb303[0]);
            }
            tb303engine[0].tbLine++;
            // End of pattern
            if(tb303engine[0].tbLine == tb303[0].patternlength[tb303engine[0].tbPattern]) tb303engine[0].tbLine = 0;
        }
    }
    if(tb303engine[1].tbPattern != 255)
    {
        tb303engine[1].tbCurMultiple++;
        if(tb303engine[1].tbCurMultiple >= tb303[1].scale)
        {
            tb303engine[1].tbCurMultiple = 0;
            if(!tb303engine[1].Note_Off)
            {
                tb303engine[1].tbNoteOn(tb303[1].tone[tb303engine[1].tbPattern][tb303engine[1].tbLine], &tb303[1]);
            }
            tb303engine[1].tbLine++;
            // End of pattern
            if(tb303engine[1].tbLine == tb303[1].patternlength[tb303engine[1].tbPattern]) tb303engine[1].tbLine = 0; 
        }
    }
}
#endif

// ------------------------------------------------------
// Bank initializer
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
void Init_Sample_Bank(void)
{
    Reset_303_Parameters(&tb303[0]);
    Reset_303_Parameters(&tb303[1]);

    for(int inico = 0; inico < 256; inico++)
    {
        // All tracks activated on entire song (1-Active 0-Inactive)
        for(int inico2 = 0; inico2 < MAX_TRACKS; inico2++)
        {
            Chan_Active_State[inico][inico2] = TRUE;
            Chan_History_State[inico][inico2] = FALSE;
        }
        pSequence[inico] = 0;

        if(inico < 128)
        {
            Beat_Sync[inico] = FALSE;
            Beat_Lines[inico] = 16;

#if !defined(__WINAMP__)
            sprintf(nameins[inico], "Untitled");
#endif

            Reset_Synth_Parameters(&PARASynth[inico]);
            Kill_Instrument(inico, TRUE);
        }
    }
}

void Kill_Instrument(int inst_nbr, int all_splits)
{
    int first_split = 0;
    int last_split = MAX_INSTRS_SPLITS;

    if(all_splits)
    {
        // Internal is the default packing scheme
        SampleCompression[inst_nbr] = SMP_PACK_WAVPACK;

#if defined(PTK_MP3)
        Mp3_BitRate[inst_nbr] = 0;
#endif

#if defined(PTK_AT3)
        At3_BitRate[inst_nbr] = 0;
#endif

#if !defined(__STAND_ALONE__)
        SamplesSwap[inst_nbr] = FALSE;
#endif

        Beat_Sync[inst_nbr] = FALSE;
        Beat_Lines[inst_nbr] = 16;

#if defined(PTK_SYNTH)
        Synth_Prg[inst_nbr] = SYNTH_WAVE_OFF;
#endif

#if !defined(__STAND_ALONE__)
        Midiprg[inst_nbr] = -1;
#endif

        Sample_Vol[inst_nbr] = 0.0f;
    }
#if !defined(__STAND_ALONE__)
    else
    {
        first_split = Current_Instrument_Split;
        last_split = Current_Instrument_Split + 1;
    }
#endif
    for(int z = first_split; z < last_split; z++)
    {
        if(RawSamples[inst_nbr][0][z])
        {
            free(RawSamples[inst_nbr][0][z]);
        }
        RawSamples[inst_nbr][0][z] = NULL;
        if(Sample_Channels[inst_nbr][z] == 2)
        {
            if(RawSamples[inst_nbr][1][z])
            {
                free(RawSamples[inst_nbr][1][z]);
            }
            RawSamples[inst_nbr][1][z] = NULL;
        }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
        if(RawSamples_Swap[inst_nbr][0][z])
        {
            free(RawSamples_Swap[inst_nbr][0][z]);
        }
        RawSamples_Swap[inst_nbr][0][z] = NULL;
        if(Sample_Channels[inst_nbr][z] == 2)
        {
            if(RawSamples_Swap[inst_nbr][1][z])
            {
                free(RawSamples_Swap[inst_nbr][1][z]);
            }
            RawSamples_Swap[inst_nbr][1][z] = NULL;
        }
#endif

        Sample_Channels[inst_nbr][z] = 0;
        SampleType[inst_nbr][z] = 0;
        LoopStart[inst_nbr][z] = 0;
        LoopEnd[inst_nbr][z] = 0;
        LoopType[inst_nbr][z] = SMP_LOOP_NONE;
        Sample_Length[inst_nbr][z] = 0;
        Finetune[inst_nbr][z] = 0;
        Sample_Amplify[inst_nbr][z] = 0.0f;
        FDecay[inst_nbr][z] = 0.0f;
        Basenote[inst_nbr][z] = DEFAULT_BASE_NOTE;

#if !defined(__STAND_ALONE__)
        sprintf(SampleName[inst_nbr][z], "Untitled");
#endif

    }
}

// ------------------------------------------------------
// Next Function: used to reset Synth_Parameters Structure
// Well, I think the default preset is not very cool, but nah!
#if defined(PTK_SYNTH)
void Reset_Synth_Parameters(Synth_Parameters *TSP)
{

#if !defined(__WINAMP__)
    sprintf(TSP->Preset_Name, "Untitled");
#endif

    TSP->osc_1_waveform = WAVEFORM_SAW;
    TSP->osc_2_waveform = WAVEFORM_PULSE;
    TSP->osc_combine = COMBINE_ADD;
    TSP->osc_sync = FALSE;
    TSP->osc_3_interval = 12;

    TSP->osc_1_pw = 256;

    TSP->osc_2_pw = 256;
    TSP->osc_2_detune = 65;
    TSP->osc_2_finetune = 0;
    
    TSP->vcf_cutoff = 64;
    TSP->vcf_resonance = 64;
    TSP->vcf_type = 0;
    
    TSP->env_1_attack = 0;
    TSP->env_1_decay = 2560;
    TSP->env_1_sustain = 20;
    TSP->env_1_release = 16384;
    
    TSP->env_2_attack = 0;
    TSP->env_2_decay = 2560;
    TSP->env_2_sustain = 20;
    TSP->env_2_release = 16384;
    
    TSP->lfo_1_period = 16;
    TSP->lfo_2_period = 16;
    
    TSP->lfo_1_osc_1_pw = 64;
    TSP->lfo_1_osc_2_pw = 64;
    TSP->lfo_1_osc_1_pitch = 64;
    TSP->lfo_1_osc_2_pitch = 64;
    TSP->lfo_1_osc_1_volume = 64;
    TSP->lfo_1_osc_2_volume = 64;   
    TSP->lfo_1_vcf_cutoff = 64;
    TSP->lfo_1_vcf_resonance = 64; 
    TSP->lfo_1_disto = 64; 

    TSP->lfo_2_osc_1_pw = 64;
    TSP->lfo_2_osc_2_pw = 64;
    TSP->lfo_2_osc_1_pitch = 64;
    TSP->lfo_2_osc_2_pitch = 64;
    TSP->lfo_2_osc_1_volume = 64;
    TSP->lfo_2_osc_2_volume = 64;   
    TSP->lfo_2_vcf_cutoff = 64;
    TSP->lfo_2_vcf_resonance = 64; 
    TSP->lfo_2_disto = 64; 

    TSP->env_1_osc_1_pw = 64;
    TSP->env_1_osc_2_pw = 64;
    TSP->env_1_osc_1_pitch = 64;
    TSP->env_1_osc_2_pitch = 64;
    TSP->env_1_osc_1_volume = 127;
    TSP->env_1_osc_2_volume = 127;   
    TSP->env_1_vcf_cutoff = 64;
    TSP->env_1_vcf_resonance = 64; 
    TSP->env_1_disto = 64; 
    
    TSP->env_2_osc_1_pw = 64;
    TSP->env_2_osc_2_pw = 64;
    TSP->env_2_osc_1_pitch = 64;
    TSP->env_2_osc_2_pitch = 64;
    TSP->env_2_osc_1_volume = 127;
    TSP->env_2_osc_2_volume = 127;
    TSP->env_2_vcf_cutoff = 64;
    TSP->env_2_vcf_resonance = 64; 
    TSP->env_2_disto = 64; 
    
    TSP->osc_3_volume = 128;
    TSP->osc_3_switch = FALSE;
    TSP->ptc_glide = 0;
    TSP->glb_volume = 128;
    TSP->disto = 64;
    TSP->lfo_1_attack = 2560;
    TSP->lfo_1_decay = 2560;
    TSP->lfo_1_sustain = 16;
    TSP->lfo_1_release = 16384;
    TSP->lfo_2_attack = 2560;
    TSP->lfo_2_decay = 2560;
    TSP->lfo_2_sustain = 16;
    TSP->lfo_2_release = 16384;
}
#endif // PTK_SYNTH
#endif // !__STAND_ALONE__ || __WINAMP__

// ------------------------------------------------------
// Free all allocated Samples
#if defined(PTK_INSTRUMENTS)
void Free_Samples(void)
{
    for(int freer = 0; freer < MAX_INSTRS; freer++)
    {
        for(char pedsplit = 0; pedsplit < MAX_INSTRS_SPLITS; pedsplit++)
        {
            if(SampleType[freer][pedsplit] != 0)
            {

#if defined(__PSVITA__)
                if(RawSamples[freer][0][pedsplit]) PSVITA_free(RawSamples[freer][0][pedsplit]);
#else
                if(RawSamples[freer][0][pedsplit]) free(RawSamples[freer][0][pedsplit]);
#endif
                RawSamples[freer][0][pedsplit] = NULL;

                if(Sample_Channels[freer][pedsplit] == 2)
                {

#if defined(__PSVITA__)
                    if(RawSamples[freer][1][pedsplit]) PSVITA_free(RawSamples[freer][1][pedsplit]);
#else
                    if(RawSamples[freer][1][pedsplit]) free(RawSamples[freer][1][pedsplit]);
#endif                    
                    RawSamples[freer][1][pedsplit] = NULL;
                    
                }
            }

#if !defined(__STAND_ALONE__)
            if(SampleType[freer][pedsplit] != 0)
            {
                if(RawSamples_Swap[freer][0][pedsplit]) free(RawSamples_Swap[freer][0][pedsplit]);
                RawSamples_Swap[freer][0][pedsplit] = NULL;
                if(Sample_Channels[freer][pedsplit] == 2)
                {
                    if(RawSamples_Swap[freer][1][pedsplit]) free(RawSamples_Swap[freer][1][pedsplit]);
                    RawSamples_Swap[freer][1][pedsplit] = NULL;
                }
            }
#endif

        }

#if !defined(__STAND_ALONE__)
        SamplesSwap[freer] = FALSE;
#endif

    }
}
#endif

// ------------------------------------------------------
// Initialize the reverb data
#if defined(PTK_COMPRESSOR)
void Initreverb(void)
{
    int i;
    int mlrw;

    for(i = 0; i < MAX_COMB_FILTERS; i++)
    {
        memset(delay_left_buffer[i], 0, 100000 * sizeof(float));
        memset(delay_right_buffer[i], 0, 100000 * sizeof(float));
        memset(left_buffer[i], 0, 100000 * sizeof(float));
        memset(right_buffer[i], 0, 100000 * sizeof(float));
    }

    for(i = 0; i < MAX_COMB_FILTERS; i++)
    {
        mlrw = 99999 - (delays[i] * 4 * 2);
        if(mlrw < 0) mlrw += 100000;
        counters_L[i] = mlrw;
        mlrw = 99999 - ((delays[i] + (Reverb_Stereo_Amount * 12)) * 4 * 2);
        if(mlrw < 0) mlrw += 100000;
        counters_R[i] = mlrw;
    }

    Init_Reverb_Filter();

    LFP_L.Reset();
    LFP_R.Reset();

}

void Init_Reverb_Filter(void)
{
    int i;

    currentCounter = 5759;

    for(i = 0; i < 10; i++)
    {
        memset(allBuffer_L[i], 0, 5760 * sizeof(float));
        memset(allBuffer_R[i], 0, 5760 * sizeof(float));
        delayedCounterL[i] = 5759 - int(c_threshold * reverb_threshold_delay[i]);
        if(delayedCounterL[i] < 0) delayedCounterL[i] += 5760;
        if(delayedCounterL[i] > 5759) delayedCounterL[i] -= 5759;
        delayedCounterR[i] = 5759 - int(c_threshold * reverb_threshold_delay[i] + ((float) Reverb_Stereo_Amount * 12));
        if(delayedCounterR[i] < 0) delayedCounterR[i] += 5760;
        if(delayedCounterR[i] > 5759) delayedCounterR[i] -= 5759;
    }
}

__inline float allpass_filter(float *Buffer, float input, int counter)
{
    float output = (-Feedback * input) + Buffer[currentCounter];
    Buffer[counter] = (output * Feedback) + input;
    return output;
}

// ------------------------------------------------------
void Reverb_work(void)
{
    int i;

    if(compressor)
    {
        float l_rout = 0.0f;
        float r_rout = 0.0f;
        float dev_l;
        float dev_r;
        float nev_l;
        float nev_r;

        // Comb filters
        for(i = 0; i < num_echoes; i++)
        {
            dev_l = (delay_left_buffer[i][counters_L[i]] * decays[i]);
            dev_r = (delay_right_buffer[i][counters_R[i]] * decays[i]);
            if(i & 1)
            {
                nev_l = dev_l - delay_left_final;
                nev_r = dev_r - delay_right_final;
            }
            else
            {
                nev_l = dev_l + delay_left_final;
                nev_r = dev_r + delay_right_final;
            }
            nev_l *= Reverb_Damp;
            nev_r *= Reverb_Damp;

            if(++counters_L[i] > 99999) counters_L[i] -= 99999;
            if(++counters_R[i] > 99999) counters_R[i] -= 99999;
            delay_left_buffer[i][counters_L[i]] = nev_l;
            delay_right_buffer[i][counters_R[i]] = nev_r;
            l_rout += nev_l;
            r_rout += nev_r;
        }

        // All pass filters
        for(i = 0; i < num_echoes; i++)
        {
            l_rout = allpass_filter(allBuffer_L[i], l_rout, delayedCounterL[i]);
            r_rout = allpass_filter(allBuffer_R[i], r_rout, delayedCounterR[i]);
            if(++delayedCounterL[i] > 5759) delayedCounterL[i] -= 5759;
            if(++delayedCounterR[i] > 5759) delayedCounterR[i] -= 5759;
        }

        // Lopass

#if DENORMAL
        LFP_L.buffy0 = denormal(LFP_L.buffy0);
        LFP_L.buffy1 = denormal(LFP_L.buffy1);
        LFP_R.buffy0 = denormal(LFP_R.buffy0);
        LFP_R.buffy1 = denormal(LFP_R.buffy1);
#endif
        l_rout = LFP_L.fWork(l_rout, Reverb_Filter_Cutoff, Reverb_Filter_Resonance);
        r_rout = LFP_R.fWork(r_rout, Reverb_Filter_Cutoff, Reverb_Filter_Resonance);

        left_reverb = l_rout;
        right_reverb = r_rout;

        // Updating current counters
        if(++currentCounter > 5759) currentCounter -= 5759;
    }
}
#endif

// ------------------------------------------------------
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
void Reset_303_Parameters(para303 *tbpars)
{
    char c;
    char d;

    tbpars->enabled = 0;
    tbpars->selectedpattern = 0;
    tbpars->tune = 64;
    tbpars->cutoff = 64;
    tbpars->resonance = 64;
    tbpars->envmod = 64;
    tbpars->decay = 64;
    tbpars->accent = 64;
    tbpars->waveform = 0;
    tbpars->scale = 1;

    for(c = 0; c < 32; c++)
    {
        tbpars->patternlength[c] = 16;

        for(d = 0; d < 16; d++)
        {
            tbpars->tone[c][d] = 0;
            tbpars->flag[c][d].slide_flag = 0;
            tbpars->flag[c][d].accent_flag = 0;
            tbpars->flag[c][d].transposeup_flag = 0;
            tbpars->flag[c][d].transposedown_flag = 0;
            tbpars->flag[c][d].pause = 1;
        }
#if !defined(__STAND_ALONE__)
        for(d = 0; d < 16; d++)
        {
            sprintf(tbpars->pattern_name[c], "Untitled");
        }
#endif
    }
}
#endif

// ------------------------------------------------------
// Filter flanger signal
#if defined(PTK_FLANGER)
__inline float Filter_FlangerL(int track, float input)
{
    float fa = 1.0f - FLANGER_LOPASS_CUTOFF; 
    float fb = float(FLANGER_LOPASS_RESONANCE * (1.0f + (1.0f / fa)));
    Flanger_sbuf0L[track] = fa * Flanger_sbuf0L[track] + FLANGER_LOPASS_CUTOFF * (input + fb * (Flanger_sbuf0L[track] - Flanger_sbuf1L[track]));
    Flanger_sbuf1L[track] = fa * Flanger_sbuf1L[track] + FLANGER_LOPASS_CUTOFF * Flanger_sbuf0L[track];

#if DENORMAL
    Flanger_sbuf0L[track] = denormal(Flanger_sbuf0L[track]);
    Flanger_sbuf1L[track] = denormal(Flanger_sbuf1L[track]);
#endif

    return(Flanger_sbuf1L[track]);
}
__inline float Filter_FlangerR(int track, float input)
{
    float fa = 1.0f - FLANGER_LOPASS_CUTOFF;
    float fb = float(FLANGER_LOPASS_RESONANCE * (1.0f + (1.0f / fa)));
    Flanger_sbuf0R[track] = fa * Flanger_sbuf0R[track] + FLANGER_LOPASS_CUTOFF * (input + fb * (Flanger_sbuf0R[track] - Flanger_sbuf1R[track]));
    Flanger_sbuf1R[track] = fa * Flanger_sbuf1R[track] + FLANGER_LOPASS_CUTOFF * Flanger_sbuf0R[track];

#if DENORMAL
    Flanger_sbuf0R[track] = denormal(Flanger_sbuf0R[track]);
    Flanger_sbuf1R[track] = denormal(Flanger_sbuf1R[track]);
#endif

    return(Flanger_sbuf1R[track]);
}
#endif

// ------------------------------------------------------
// Compressor / Limiter
#if defined(PTK_LIMITER_MASTER) || defined(PTK_LIMITER_TRACKS)
float mas_attack = 0.977579f;
float mas_release = 0.977579f;

float Do_RMS(float input, float *rms_sum, float *buffer)
{
    *rms_sum -= buffer[mas_comp_pos_rms_buffer];
    buffer[mas_comp_pos_rms_buffer] = input * input;
    *rms_sum += buffer[mas_comp_pos_rms_buffer];
    if(*rms_sum < 0.0f) *rms_sum = 0.0f;
    return(sqrtf(*rms_sum / (float) MAS_COMPRESSOR_SIZE));
}
#endif

#if defined(PTK_LIMITER_TRACKS)
void Mas_Compressor_Set_Variables_Track(int Track, float threshold, float ratio)
{
    if(threshold < 0.01f) threshold = 0.01f;
    if(threshold > 100.0f) threshold = 100.0f;
    if(ratio < 0.01f) ratio = 0.01f;
    if(ratio > 100.0f) ratio = 100.0f;
    mas_comp_threshold_Track[Track] = threshold;
    mas_comp_ratio_Track[Track] = ratio;
    mas_threshold_Track[Track] = threshold * 0.001f;
    mas_ratio_Track[Track] = ratio * 0.01f;
}

float Mas_Compressor_Track(int Track, float input, float *rms_sum, float *buffer, float *env)
{
    float gain;
    float rmsf = Do_RMS(input, rms_sum, buffer);

    float theta = rmsf > *env ? mas_attack : mas_release;
    *env = (1.0f - theta) * rmsf + theta * *env;

    gain = 1.0f;
    if(*env > mas_threshold_Track[Track])
    {
        gain = expf((LOG(mas_threshold_Track[Track]) - LOG(*env)) * mas_ratio_Track[Track]);
    }
    return input * gain;
}
#endif // PTK_LIMITER_TRACKS

#if defined(PTK_LIMITER_MASTER)
#if !defined(__STAND_ALONE__) || defined(__WINAMP__)
void Mas_Compressor_Set_Variables_Master(float threshold, float ratio)
{
    if(threshold < 0.01f) threshold = 0.01f;
    if(threshold > 100.0f) threshold = 100.0f;
    if(ratio < 0.01f) ratio = 0.01f;
    if(ratio > 100.0f) ratio = 100.0f;
    mas_comp_threshold_Master = threshold;
    mas_comp_ratio_Master = ratio;
    mas_threshold_Master = threshold * 0.001f;
    mas_ratio_Master = ratio * 0.01f;
}
#endif

float Mas_Compressor_Master(float input, float *rms_sum, float *buffer, float *env)
{
    float gain;
    float rmsf = Do_RMS(input, rms_sum, buffer);

    float theta = rmsf > *env ? mas_attack : mas_release;
    *env = (1.0f - theta) * rmsf + theta * *env;

    gain = 1.0f;
    if(*env > mas_threshold_Master)
    {
        gain = expf((LOG(mas_threshold_Master) - LOG(*env)) * mas_ratio_Master);
    }
    return gain;
}
#endif // PTK_LIMITER_MASTER

// ------------------------------------------------------
// Return an index in a pattern's module
int Get_Pattern_Offset(int pattern, int track, int row)
{
    return((track * PATTERN_BYTES) + (row * PATTERN_ROW_LEN) + (pattern * PATTERN_LEN));
}

// ------------------------------------------------------
// Calculate the boundaries of a carrier for splines calculation
void Set_Carrier_Boundaries(unsigned int Position,
                            unsigned int *Boundaries,
                            int LoopType,
                            unsigned int LoopWay,
                            unsigned int Length,
                            unsigned int LoopEnd,
                            unsigned int LoopStart
                           )
{
    *Boundaries = Position;
    Length--;
    if(LoopEnd >= Length) LoopEnd = Length;

#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
    switch(LoopType)
    {
#if defined(PTK_LOOP_FORWARD)
        case SMP_LOOP_FORWARD:
            if(LoopWay == SMP_LOOPING_FORWARD)
            {
                if(*Boundaries >= LoopEnd) *Boundaries = LoopStart;
            }
            else
            {
                if((int) *Boundaries <= (int) LoopStart) *Boundaries = LoopEnd;
            }
            break;
#endif

#if defined(PTK_LOOP_PINGPONG)
        case SMP_LOOP_PINGPONG:
            if(LoopWay == SMP_LOOPING_FORWARD)
            {
                if(*Boundaries >= LoopEnd) *Boundaries = LoopEnd;
            }
            else
            {
                if(*Boundaries >= LoopEnd) *Boundaries = LoopEnd;
                if((int) *Boundaries <= (int) LoopStart) *Boundaries = LoopStart;
            }
            break;
#endif

        case SMP_LOOP_NONE:
#endif // defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
            if(LoopWay == SMP_LOOPING_FORWARD)
            {
                if(*Boundaries >= Length) *Boundaries = Length;
            }
            else
            {
                if((int) *Boundaries <= 0) *Boundaries = 0;
            }
            
#if defined(PTK_LOOP_FORWARD) || defined(PTK_LOOP_PINGPONG)
            break;
    }
#endif
}

#if defined(PTK_TRACK_EQ)
// Public domain stuff from Neil C. / Etanza Systems
float vsa;

void Init_Equ(LPEQSTATE es)
{
    memset(es, 0, sizeof(EQSTATE));
    es->lg = 1.0f;
    es->mg = 1.0f;
    es->hg = 1.0f;
    es->lf = 2.0f * sinf(PIf * (880.0f / fMIX_RATE));
    es->hf = 2.0f * sinf(PIf * (5000.0f / fMIX_RATE));
    vsa = (float) (1.0 / 4294967295.0);

#if DENORMAL
    es->lf = denormal(es->lf);
    es->hf = denormal(es->hf);
    vsa = denormal(vsa);
#endif

}

float Do_Equ(LPEQSTATE es, float sample, int Left)
{
    float l;
    float m;
    float h;

#if DENORMAL
    es->f1p0[Left] = denormal(es->f1p0[Left]);
    es->f1p1[Left] = denormal(es->f1p1[Left]);
    es->f1p2[Left] = denormal(es->f1p2[Left]);
    es->f1p3[Left] = denormal(es->f1p3[Left]);
#endif

    es->f1p0[Left] += (es->lf * (sample - es->f1p0[Left])) + vsa;
    es->f1p1[Left] += (es->lf * (es->f1p0[Left] - es->f1p1[Left]));
    es->f1p2[Left] += (es->lf * (es->f1p1[Left] - es->f1p2[Left]));
    es->f1p3[Left] += (es->lf * (es->f1p2[Left] - es->f1p3[Left]));

#if DENORMAL
    es->f2p0[Left] = denormal(es->f2p0[Left]);
    es->f2p1[Left] = denormal(es->f2p1[Left]);
    es->f2p2[Left] = denormal(es->f2p2[Left]);
    es->f2p3[Left] = denormal(es->f2p3[Left]);
#endif
    
    l = es->f1p3[Left];
    es->f2p0[Left] += (es->hf * (sample - es->f2p0[Left])) + vsa;
    es->f2p1[Left] += (es->hf * (es->f2p0[Left] - es->f2p1[Left]));
    es->f2p2[Left] += (es->hf * (es->f2p1[Left] - es->f2p2[Left]));
    es->f2p3[Left] += (es->hf * (es->f2p2[Left] - es->f2p3[Left]));

    h = es->sdm3[Left] - es->f2p3[Left];
    m = es->sdm3[Left] - (h + l);

    l *= es->lg;
    m *= es->mg;
    h *= es->hg;

    es->sdm3[Left] = es->sdm2[Left];
    es->sdm2[Left] = es->sdm1[Left];
    es->sdm1[Left] = sample;
    return(l + m + h);
}
#endif

#if defined(PTK_SHUFFLE)
void Update_Shuffle(void)
{
    if(shuffleswitch == 1) shufflestep = -((SamplesPerTick * shuffle_amount)) / 200;
    else shufflestep = (SamplesPerTick * shuffle_amount) / 200;
}

#endif
