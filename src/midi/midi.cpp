// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2021 Franck Charlet.
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

#if !defined(__NO_MIDI__)

// ------------------------------------------------------
// Includes
#include "../include/variables.h"
#include "../include/ptk.h"

#include "include/midi.h"
#include "include/RtMidi.h"
#include "include/midi_dispatch.h"

// ------------------------------------------------------
// Variables
extern int Nbr_Sub_NoteOff;
extern int key_record_first_time;
extern int old_key_Pattern_Line;
extern int pos_scope;
extern int pos_scope_latency;

extern int player_pos;
extern int metronome_rows_counter;

char Midi_In_Name[256];
char Midi_Out_Name[256];

int Midi_Notes_History[MAX_TRACKS][256];
int Midi_Current_Notes[MAX_TRACKS][MAX_POLYPHONY];
int Midi_Notes_History_Amount;

RtMidiIn  *midiin = 0;
RtMidiOut *midiout = 0;

int midiin_changed = 0;
int midiout_changed = 0;

int midiin_port_opened = 0;

signed char c_midiin = -1;
signed char c_midiout = -1;

signed char n_midioutdevices = 0;
signed char n_midiindevices = 0;

// ------------------------------------------------------
// Driver functions
void _Midi_Send(int nbr_track, int eff_dat, int row_dat);

// ------------------------------------------------------
// Return the instrument associated with the midi program
int Midi_GetProgram(int midi_program)
{
    int i;

    for(i = 0; i < 128; i++)
    {
        if(midi_program == Midiprg[i]) return(i);
    }
    return(-1);
}

// ------------------------------------------------------
// Handle the midi events
void Midi_CallBackIn(double deltatime,
                     std::vector< unsigned char > *message,
                     void *userData)
{
    int Midi_Channel_Number;
    int Midi_Command;
    int Midi_Datas_1;
    int Midi_Datas_2;
    int Midi_Velocity;
    int Instrument_Number; 
    int tmp_note;
    int Unknown_Message;
    Uint32 Param1 = 0;
    Uint32 Param1_Swap = 0;
    int i;
    unsigned char *ptr = (unsigned char *) &Param1;
    unsigned int nBytes = message->size();

    // From 1 to 3 (hopefully)
    for(i = 0; i < nBytes; i++)
    {
        *ptr++ = message->at(i);
    }

#if defined(__BIG_ENDIAN__)
    Param1_Swap = (Param1 & 0xff) << 24;
    Param1_Swap |= ((Param1 >> 8) & 0xff) << 16;
    Param1_Swap |= ((Param1 >> 16) & 0xff) << 8;
    Param1_Swap |= ((Param1 >> 24) & 0xff);
    Param1 = Param1_Swap;
#endif    

    Midi_Channel_Number = Param1 & 0xf;
    Midi_Command = Param1 & 0xf0;

    if(Midi_Command == 0x90 || Midi_Command == 0x80)
    {
        if(is_recording && !is_recording_2 && key_record_first_time)
        {
            // Start recording
            is_recording_2 = 1;
            Nbr_Sub_NoteOff = 0;
            is_record_key = FALSE;
            is_editing = TRUE;
            L_MaxLevel = 0;
            R_MaxLevel = 0;
            Switch_Cmd_Playing(FALSE);
            Pattern_Line_Visual = Pattern_Line;
            key_record_first_time = FALSE;
            old_key_Pattern_Line = Pattern_Line_Visual;
            Clear_Midi_Channels_Pool();
            player_pos = -1;
            metronome_rows_counter = 0;
            Post_Song_Init();
            Ptk_Play();
        }
    }

    switch(Midi_Command)
    {
        // Pitch bend (not handled yet).
        case 0xe0:
            Midi_Datas_1 = (Param1 >> 8) & 0xff;
            Midi_Datas_2 = (Param1 >> 16) & 0xff;
            break;

        // Control Change
        case 0xb0:
            Midi_Datas_1 = (Param1 >> 8) & 0xff;
            Midi_Datas_2 = (Param1 >> 16) & 0xff;
            
            Dispatch_Midi_Msg(Midi_Datas_1, Midi_Datas_2);
            break;

        // Program Change
        case 0xc0:
            Instrument_Number = Midi_GetProgram((Param1 >> 8) & 0xff);
            if(Instrument_Number > -1)
            {
                Current_Instrument = Instrument_Number;
                Clear_Input();
                Actualize_Patterned();
                RefreshSample();
                Renew_Sample_Ed();
                Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
            }
            break;

        // Note Off
        case 0x80:
            // Make sure the velocity is 0
            Param1 &= 0x00ffff;

        // Note On
        case 0x90:
            Midi_Velocity = (Param1 >> 16) & 0xff;
            if(Midi_Velocity > 0xfe) Midi_Velocity = 0xfe;
            tmp_note = (Param1 >> 8) & 0xff;
            if(tmp_note > NOTE_MAX) tmp_note = NOTE_MAX;

            if(Midi_Velocity)
            {
                if(is_recording_2 || is_editing)
                {
                    Send_Note(tmp_note, TRUE, FALSE);
                }
                if(is_recording_2 || !is_editing)
                { 
                    Note_Jazz(Track_Under_Caret, tmp_note, ((float) Midi_Velocity) / 127.0f);
                }
            }
            else
            {
                // A note off
                if(is_recording_2 || is_editing)
                {
                    Send_Note(tmp_note | 0x80, TRUE, FALSE);
                }
                if(is_recording_2 || !is_editing)
                {
                    Note_Jazz_Off(tmp_note);
                }
            }
            break;

        default:
            Unknown_Message = Param1;
            break;
    }
}

// ------------------------------------------------------
// Turn all midi notes off
void Midi_AllNotesOff(void)
{
    if(c_midiout != -1)
    {
        for(int no_track = 0; no_track < MAX_TRACKS; no_track++)
        {
            Midi_NoteOff(no_track, -1);
        }
    }
}

// ------------------------------------------------------
// Reset midi out device
void Midi_Reset(void)
{
    Midi_AllNotesOff();

    for(int mreset = 0; mreset < 16; mreset++)
    {
        LastProgram[mreset] = -1;
    }
}

// ------------------------------------------------------
// Send a command to the midi out device
void Midi_Send(int nbr_track, int eff_dat, int row_dat)
{
    int i;

    if(c_midiout != -1)
    {
        if((nbr_track & 0xfff0) == 144)
        {
            for(i = 0; i < 256; i++)
            {
                // First empty channel
                if(Midi_Notes_History[nbr_track & 0xf][i] == 0)
                {
                    Midi_Notes_History[nbr_track & 0xf][i] = eff_dat + 1;
                    break;
                }
            }
        }
        _Midi_Send(nbr_track, eff_dat, row_dat);
    }
}

// ------------------------------------------------------
// Turn a midi channel off
void Midi_NoteOff(int channel, int note)
{
    int i;

    if(c_midiout != -1)
    {
        note++;
        _Midi_Send(176 + Chan_Midi_Prg[channel], 0x40, 0);
        if(note)
        {
            for(i = 0; i < 256; i++)
            {
                if(Midi_Notes_History[Chan_Midi_Prg[channel]][i] == note)
                {
                    _Midi_Send(0x80 + Chan_Midi_Prg[channel], (note - 1), 127);
                    Midi_Notes_History[Chan_Midi_Prg[channel]][i] = 0;
                    break;
                }
            }
        }
        else
        {
            for(i = 0; i < 256; i++)
            {
                _Midi_Send(0x80 + Chan_Midi_Prg[channel], (Midi_Notes_History[Chan_Midi_Prg[channel]][i] - 1), 127);
                Midi_Notes_History[Chan_Midi_Prg[channel]][i] = 0;
            }
            Midi_Notes_History_Amount = 0;
        }
    }
}

// ------------------------------------------------------
// Open the midi in device
void Midi_InitIn(void)
{
    if(midiin_changed != 0)
    {
        if(c_midiin < -1) c_midiin = n_midiindevices - 1;
        if(c_midiin == n_midiindevices) c_midiin = -1;

        if(c_midiin != -1)
        {
            Midi_CloseIn();
            try
            {
                midiin->openPort(c_midiin);
                midiin->setCallback(&Midi_CallBackIn);
                midiin->ignoreTypes(1, 1, 1);
                if(midiin_changed == 1) Status_Box("Midi In device activated.");
                midiin_port_opened = TRUE;
            }
            catch(...)
            {
                Status_Box("Midi In device failed to open.");
            }
        }
        else
        {
            Midi_CloseIn();
            if(midiin_changed == 1) Status_Box("Midi In device disconnected.");
        }
        midiin_changed = 0;
    }
}

// ------------------------------------------------------
// Close the midi in device
void Midi_CloseIn(void)
{
    if(midiin)
    {
        if(midiin_port_opened) midiin->cancelCallback();
        midiin->closePort();
    }
}

// ------------------------------------------------------
// Open the midi out device
void Midi_InitOut(void)
{
    if(midiout_changed != 0)
    {
        Midi_Reset();
        if(c_midiout < -1) c_midiout = n_midioutdevices - 1;
        if(c_midiout == n_midioutdevices) c_midiout = -1;

        if(c_midiout != -1)
        {
            Midi_CloseOut();
            try
            {
                midiout->openPort(c_midiout);
                if(midiout_changed == 1) Status_Box("Midi Out device activated.");
            }
            catch(...)
            {
                Status_Box("Midi Out device failed to open.");
            }
        }
        else
        {
            Midi_CloseOut();
            if(midiout_changed == 1) Status_Box("Midi Out device disconnected.");
        }
        midiout_changed = 0;
    }
}

// ------------------------------------------------------
// Close the midi out device
void Midi_CloseOut(void)
{
    if(midiout) midiout->closePort();
}

// ------------------------------------------------------
// Enumerate all midi in/out interfaces available
void Midi_GetAll(void)
{
    n_midiindevices = 0;
    n_midioutdevices = 0;

    midiin = new RtMidiIn();
    if(midiin)
    {
        midiout = new RtMidiOut();
        if(midiout)
        {
            n_midiindevices = midiin->getPortCount();
            n_midioutdevices = midiout->getPortCount();
        }
    }
}

// ------------------------------------------------------
// Free allocated interfaces resources
void Midi_FreeAll(void)
{
    if(midiin_port_opened) midiin->cancelCallback();
    if(midiin) delete midiin;
    if(midiout) delete midiout;
}

// ------------------------------------------------------
// Send a command to the midi out device
void _Midi_Send(int nbr_track, int eff_dat, int row_dat)
{
    std::vector<unsigned char> message;
    if(eff_dat != -1)
    {
        message.push_back(nbr_track);
        message.push_back(eff_dat);
        message.push_back(row_dat);
        if(midiout)
        {
            midiout->sendMessage(&message);
        }
    }
}

// ------------------------------------------------------
// Return the name of the current midi in device
char *Midi_GetInName(void)
{
    if(c_midiin == -1)
    {
        strcpy(Midi_Out_Name, "");
        return(Midi_Out_Name);
    }
    midiin->getPortName(c_midiin, Midi_In_Name);
    return(Midi_In_Name);
}

// ------------------------------------------------------
// Return the name of the current midi out device
char *Midi_GetOutName(void)
{
    if(c_midiout == -1)
    {
        strcpy(Midi_Out_Name, "");
        return(Midi_Out_Name);
    }
    midiout->getPortName(c_midiout, Midi_Out_Name);
    return(Midi_Out_Name);
}

#endif
