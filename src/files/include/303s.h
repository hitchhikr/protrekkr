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

#ifndef _303S_H_
#define _303S_H_

// ------------------------------------------------------
// Includes
#include "files.h"
#include "files_list.h"
#include "../../editors/include/editor_pattern.h"
#include "../../include/ptk.h"

// ------------------------------------------------------
// Functions
void Load_303(char *FileName);
void Save_303(void);
#if !defined(BZR2)
void Load_303_Data(int (*Read_Function)(void *, int, int, FILE *),
                   int (*Read_Function_Swap)(void *, int, int, FILE *),
                   FILE *in, int unit, int pattern);
#else
void Load_303_Data(int (*Read_Function)(void *, int, int, CustomFile &),
                   int (*Read_Function_Swap)(void *, int, int, CustomFile &),
                   CustomFile &in, int unit, int pattern);
#endif
void Save_303_Data(int (*Write_Function)(void *, int, int, FILE *),
                   int (*Write_Function_Swap)(void *, int, int, FILE *),
                   FILE *in, int unit, int pattern);

#endif
