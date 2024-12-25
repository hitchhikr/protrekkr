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

#ifndef _FILES_H_
#define _FILES_H_

// ------------------------------------------------------
// Includes
#include <stdio.h>
#include "../../include/variables.h"

#if defined(__AROS__) || defined(__MORPHOS__)
#include <stdint.h>
#include <string.h>
#define int32 int32_t
#define int64 int64_t
#endif

// ------------------------------------------------------
// Constants
#define SAVE_WRITE 0
#define SAVE_CALCLEN 1
#define SAVE_WRITEMEM 2

#define LOAD_READ FALSE
#define LOAD_READMEM TRUE

#if defined(__WIN32__)
#define SLASH "\\"
#else
#define SLASH "/"
#endif

// ------------------------------------------------------
// Variables
extern int Final_Mod_Length;
extern int mt_tags[16];
extern char mt_channels[16];
extern char Dir_Mods[MAX_PATH];
extern char Dir_Instrs[MAX_PATH];
extern char Dir_Presets[MAX_PATH];
extern char Dir_Reverbs[MAX_PATH];
extern char Dir_MidiCfg[MAX_PATH];
extern char Dir_Patterns[MAX_PATH];
extern char Dir_Samples[MAX_PATH];
extern char Use_Cubic;
extern char Paste_Across;
extern int wait_AutoSave;
extern gear303 tb303engine[2];
extern para303 tb303[2];

#if defined(BZR2)
struct CustomFile
{
    CustomFile(const uint8_t *data, size_t size) : buf(data), maxSize(size) {}
    const uint8_t* buf;
    size_t maxSize;
    size_t pos = 0;

    int Read(void *dst, size_t size, size_t count)
    {
        auto newPos = pos + size * count;
        if(newPos > maxSize)
        {
            if(pos < maxSize)
            {
                memcpy(dst, buf + pos, maxSize - pos);
                memset(reinterpret_cast<char *>(dst) + maxSize - pos, 0, newPos - maxSize);
                count = (maxSize - pos) / size;
            }
            else
            {
                memset(dst, 0, size * count);
                count = 0;
            }
            newPos = maxSize;
        }
        else
        {
            memcpy(dst, buf + pos, size * count);
        }
        pos = newPos;
        return int(count);
    }
};
#endif

// ------------------------------------------------------
// Functions
#if !defined(BZR2)
int Load_Ptk(char *FileName);
#else
int Load_Ptk(CustomFile customFile);
#endif

int Save_Ptk(char *FileName, int NewFormat, int Simulate, Uint8 *Memory);
void Backup_Module(char *FileName);
int Pack_Module(char *FileName);
int Test_Mod(void);
void Load_Synth(char *FileName);
void Load_Inst(char *FileName);
void Save_Inst(void);
void Save_Synth(void);
void Song_Stop(void);
void Free_Samples(void);
Uint8 *Pack_Data(Uint8 *Memory, int *Len);
Uint8 *Depack_Data(Uint8 *Memory, int Size, int size_out);
#if !defined(BZR2)
int Get_File_Size(FILE *Handle);
#else
int Get_File_Size(CustomFile &Handle);
#endif
int Calc_Length(void);
void Reset_Song_Length(void);
void Clear_Input(void);
int File_Exist(char *Format, char *Directory, char *FileName);
int File_Exist_Req(char *Format, char *Directory, char *FileName);
void Init_Tracker_Context_After_ModLoad(void);

#if !defined(BZR2)
int Read_Data(void *value, int size, int amount, FILE *handle);
int Read_Data_Swap(void *value, int size, int amount, FILE *handle);
#else
int Read_Data(void *value, int size, int amount, CustomFile &handle);
int Read_Data_Swap(void *value, int size, int amount, CustomFile &handle);
#endif

int Write_Data(void *value, int size, int amount, FILE *handle);
int Write_Data_Swap(void *value, int size, int amount, FILE *handle);
void Swap_Sample(short *buffer, int sample, int bank);
void rtrim_string(char *string, int maxlen);
void Write_Unpacked_Sample(int (*Write_Function)(void *, int ,int, FILE *),
                           FILE *in, int sample, int bank);
int Write_Mod_Data(void *Datas, int Unit, int Length, FILE *Handle);
int Write_Mod_Data_Swap(void *Datas, int Unit, int Length, FILE *Handle);
short *Get_WaveForm(int Instr_Nbr, int Channel, int Split);
void Pack_Sample(FILE *FileHandle, short *Sample, int Size, char Pack_Type, int BitRate);
int Fix_Codec(int Scheme);

#endif
