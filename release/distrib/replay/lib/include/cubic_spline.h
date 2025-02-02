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

#ifndef _CUBIC_SPLINE_H_
#define _CUBIC_SPLINE_H_

// ------------------------------------------------------
// Functions
inline float Cubic_Work(float ym, float input, float y1, float y2, unsigned int res)
{
    // 0..1.0f
    float x = (float) (((double) res) / 4294967296.0f);
  
    // -0.5..0.5
    float z = x - 0.5f;
    float even1 = y1 + input;
    float odd1 = y1 - input;
    float even2 = y2 + ym;
    float odd2 = y2 - ym;

    float c0 = even1 * 0.45868f + even2 * 0.04131f;
    float c1 = odd1 * 0.48068f + odd2 * 0.17577f;
    float c2 = even1 * -0.24618f + even2 * 0.24614f;
    float c3 = odd1 * -0.36030f + odd2 * 0.10174f;
    return(((c3 * z + c2) * z + c1) * z + c0);
}

#endif
