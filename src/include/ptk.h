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

#ifndef _MAIN_H_
#define _MAIN_H_

// ------------------------------------------------------
// Includes
#if !defined(BZR2)
#include "../support/include/main.h"
#else
#include <cstdio>
#endif

#include "../samples/wav/include/riff.h"
#include "../samples/aiff/include/aiff.h"
#include "../include/variables.h"

#if !defined(BZR2)
#include "../ui/include/misc_draw.h"
#include "../ui/include/draw_primitives.h"
#include "../ui/include/requesters.h"
#endif

#include "../files/include/files.h"
#include "../files/include/files_list.h"
#include "../files/include/mods.h"
#include "../files/include/303s.h"
#include "../files/include/reverbs.h"
#include "../files/include/midi_cfg.h"
#include "../files/include/patterns.h"

#if !defined(BZR2)
#include "../editors/include/editor_setup.h"
#include "../editors/include/editor_midi.h"
#include "../editors/include/editor_303.h"
#include "../editors/include/editor_fx_setup.h"
#include "../editors/include/editor_synth.h"
#include "../editors/include/editor_track_fx.h"
#include "../editors/include/editor_diskio.h"
#include "../editors/include/editor_reverb.h"
#include "../editors/include/editor_sequencer.h"
#include "../editors/include/editor_track.h"
#include "../editors/include/editor_instrument.h"
#include "../editors/include/editor_pattern.h"
#include "../editors/include/editor_sample.h"
#include "../editors/include/patterns_blocks.h"

#include "../support/include/timer.h"

#include "../midi/include/midi.h"
#endif

#include "../../release/distrib/replay/lib/include/endianness.h"
#include "../../release/distrib/replay/lib/include/replay.h"

#include <string>
#include <list>
#include <sstream>
#include <time.h>

using namespace std;

class skin_file
{
    public:
        const char *name;
};

extern list <skin_file> skin_files;
#if !defined(BZR2)
extern REQUESTER Exit_Requester;
#endif

// ------------------------------------------------------
// Structures
typedef struct
{
    int Channel;
    int Sub_Channel;
    int Note;
} JAZZ_KEY, *LPJAZZ_KEY;

// ------------------------------------------------------
// Variables
#if !defined(BZR2)
extern SDL_Surface *Main_Screen;
extern MOUSE Mouse;
#endif

#if defined(__WIN32__)
#include <mmsystem.h>
extern HWND Main_Window;
#endif

#if defined(__WIN32__)
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#if defined(__WIN32__)
#define GETCWD _getcwd
#define CHDIR _chdir
#define GETDRIVE _getdrive
#else
#define GETCWD getcwd
#define CHDIR chdir
#define FA_DIREC 0x10
#define A_SUBDIR FA_DIREC
#define _A_SUBDIR FA_DIREC
#endif
#define _A_FILE 0
#define _A_SEP -1

// ------------------------------------------------------
// Functions
int Init_Context(void);
void Destroy_Context(void);
int Screen_Update(void);
void STDCALL Mixer(Uint8 *Buffer, Uint32 Len);
#if !defined(BZR2)
SDL_Surface *Load_Skin_Picture(char *name);
#endif
LPJAZZ_KEY Get_Jazz_Key_Off(JAZZ_KEY Pool[MAX_TRACKS][MAX_POLYPHONY], int Note);
int Discard_Key_Note_Off(JAZZ_KEY Pool[MAX_TRACKS][MAX_POLYPHONY], int Channel, int Sub_Channel);
void Set_Default_Channels_Polyphony(void);
void Clear_Midi_Channels_Pool(void);
void Send_Note(int Note, int Raw_Note, int One_Channel);
void Note_Jazz(int track, int note, float volume);
void Note_Jazz_Off(int note);
void Display_Beat_Time(void);
int Read_Pattern_Note(int Position);
void Set_Font_Normal(void);
void Set_Font_Double(void);
void Display_Patterns_Sizes_Status(void);
void Display_Patterns_Sliders_Status(void);
int Get_Nbr_Synths(void);
int Get_Instrs_Data(int *samp_size);

#endif
