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

#ifndef _SAMPLES_OPS_H_
#define _SAMPLES_OPS_H_

// ------------------------------------------------------
// Includes
#include "../../include/variables.h"
#include "../../ui/include/misc_draw.h"

// ------------------------------------------------------
// Variables
extern int cur_sample_buffer;
extern int Sample_Back_Size[4];

// ------------------------------------------------------
// Functions
int Sample_Rotate_Left(int32 range_start, int32 range_end, int amount);
int Sample_Rotate_Right(int32 range_start, int32 range_end, int amount);
int Sample_Reverse(int32 range_start, int32 range_end);
int Sample_Crop(int32 range_start, int32 range_end);
int Sample_Copy(int32 range_start, int32 range_end);
int Sample_Paste(int32 range_start);
int Sample_Cut(int32 range_start, int32 range_end, int do_copy);
void Sample_DC_Adjust(int32 range_start, int32 range_end);
void Sample_Maximize(int32 range_start, int32 range_end);
void Sample_Zeroize(int32 range_start, int32 range_end);
void Sample_FadeIn(int32 range_start, int32 range_end);
void Sample_FadeOut(int32 range_start, int32 range_end);
void Sample_Half(int32 range_start, int32 range_end);
int Sample_Duplicate(int32 range_start, int32 range_end);
int Sample_InsertZero(int32 range_start, int32 range_end);

#endif
