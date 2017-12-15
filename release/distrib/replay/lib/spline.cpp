// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2014 Franck Charlet.
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
#include "include/spline.h"
#include "include/ptk_def_properties.h"

#if defined(PTK_USE_SPLINE)
// ------------------------------------------------------
// Variables
float at[RESOLUTION];
float bt[RESOLUTION];
float ct[RESOLUTION];
float dt[RESOLUTION];

// ------------------------------------------------------
// Initialize the working table
void Spline_Init(void)
{
    for(int i = 0; i < RESOLUTION; i++)
    {
        float x = (float) i / (float) RESOLUTION;
        at[i] = -0.5f * x * x * x + x * x - 0.5f * x;
        bt[i] = 1.5f * x * x * x - 2.5f * x * x + 1.0f;
        ct[i] = -1.5f * x * x * x + 2.0f * x * x + 0.5f * x;
        dt[i] = 0.5f * x * x * x - 0.5f * x * x;
    }
}

// ------------------------------------------------------
// Work function. Where all is cooked :]
// yo = y[-1] [sample at x-1]
// y0 = y[0]  [sample at x (input)]
// y1 = y[1]  [sample at x+1]
// y2 = y[2]  [sample at x+2]

// res= distance between two neighbours sample points [y0 and y1] 
//      ,so [0...1.0]. You have to multiply this distance * RESOLUTION used
//      on the spline conversion table. [256 by default]
// If you are using 256 is assumed you are using 8 bit decimal
// fixed point offsets for resampling.
float Spline_Work(float yo, float y0, float y1, float y2, unsigned int res)
{
    res = res >> 22;
    return (at[res] * yo + bt[res] * y0 + ct[res] * y1 + dt[res] * y2);
}
#endif
