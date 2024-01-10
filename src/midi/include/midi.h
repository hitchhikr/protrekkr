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

#ifndef _MIDI_H_
#define _MIDI_H_

#if !defined(__NO_MIDI__)

// ------------------------------------------------------
// Includes
#if defined(__WIN32__)
#include <Windows.h>
#include <mmsystem.h>
#endif

// ------------------------------------------------------
// Variables
extern signed char n_midioutdevices;
extern signed char n_midiindevices;

extern int midiin_changed;
extern int midiout_changed;

// ------------------------------------------------------
// Functions
void Midi_AllNotesOff(void);
void Midi_Reset(void);

void Midi_GetAll(void);
void Midi_FreeAll(void);

void Midi_InitIn(void);
void Midi_CloseIn(void);
void Midi_InitOut(void);
void Midi_CloseOut(void);

void Midi_NoteOff(int channel, int note);
void Midi_Send(int nbr_track, int eff_dat, int row_dat);

char *Midi_GetInName(void);
char *Midi_GetOutName(void);

#endif

#endif
