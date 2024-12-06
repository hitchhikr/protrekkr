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

// ------------------------------------------------------
// Includes
#include "include/samples_ops.h"

#if defined(__AROS__) || defined(__MORPHOS__)
#include <stdint.h>
#define int32 int32_t
#endif

// ------------------------------------------------------
// Variables
int cur_sample_buffer;
short *Sample_Back[4][2];
int Sample_Back_Channels[4];
int Sample_Back_Size[4];
extern short *Player_WL[MAX_TRACKS][MAX_POLYPHONY];
extern short *Player_WR[MAX_TRACKS][MAX_POLYPHONY];
extern unsigned int Player_LS[MAX_TRACKS][MAX_POLYPHONY];
extern unsigned int Player_LE[MAX_TRACKS][MAX_POLYPHONY];
extern unsigned int Player_LL[MAX_TRACKS][MAX_POLYPHONY];
extern unsigned int Player_NS[MAX_TRACKS][MAX_POLYPHONY];
extern char Player_LT[MAX_TRACKS][MAX_POLYPHONY];
extern char Player_LW[MAX_TRACKS][MAX_POLYPHONY];
extern s_access sp_Position[MAX_TRACKS][MAX_POLYPHONY];

// ------------------------------------------------------
// Rotate a selection to the left by a given amount
int Sample_Rotate_Left(int32 range_start, int32 range_end, int amount)
{
    int32 i;
    int j;
    short sample1;
    short sample2;
    char nc;
    long shiftsize = (range_end - range_start);
 
    if(shiftsize)
    {
        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

        for(j = 0; j < amount; j++)
        {
            sample1 = RawSamples[Current_Instrument][0][Current_Instrument_Split][range_start];
            if(nc == 2)
            {
                sample2 = RawSamples[Current_Instrument][1][Current_Instrument_Split][range_start];
            }
 
            // Shift it
            for(i = range_start; i < range_end - 1; i++)
            {
                RawSamples[Current_Instrument][0][Current_Instrument_Split][i] = RawSamples[Current_Instrument][0][Current_Instrument_Split][i + 1];
                if(nc == 2)
                {
                    RawSamples[Current_Instrument][1][Current_Instrument_Split][i] = RawSamples[Current_Instrument][1][Current_Instrument_Split][i + 1];
                }
            }
            RawSamples[Current_Instrument][0][Current_Instrument_Split][i] = sample1;
            if(nc == 2)
            {
                RawSamples[Current_Instrument][1][Current_Instrument_Split][i] = sample2;
            }
        }
        Status_Box("Shift Left Done.");
        return 1;
    }
    return 0;
}

// ------------------------------------------------------
// Rotate a selection to the right by a given amount
int Sample_Rotate_Right(int32 range_start, int32 range_end, int amount)
{
    int32 i;
    int j;
    short sample1;
    short sample2;
    char nc;
    long shiftsize = (range_end - range_start);
 
    if(shiftsize)
    {
        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

        for(j = 0; j < amount; j++)
        {
            sample1 = RawSamples[Current_Instrument][0][Current_Instrument_Split][range_end - 1];
            if(nc == 2)
            {
                sample2 = RawSamples[Current_Instrument][1][Current_Instrument_Split][range_end - 1];
            }
 
            // Shift it
            for(i = range_end - 2; i >= range_start; i--)
            {
                RawSamples[Current_Instrument][0][Current_Instrument_Split][i + 1] = RawSamples[Current_Instrument][0][Current_Instrument_Split][i];
                if(nc == 2)
                {
                    RawSamples[Current_Instrument][1][Current_Instrument_Split][i + 1] = RawSamples[Current_Instrument][1][Current_Instrument_Split][i];
                }
            }
            RawSamples[Current_Instrument][0][Current_Instrument_Split][range_start] = sample1;
            if(nc == 2)
            {
                RawSamples[Current_Instrument][1][Current_Instrument_Split][range_start] = sample2;
            }
        }
        Status_Box("Shift Right Done.");
        return 1;
    }
    return 0;
}

// ------------------------------------------------------
// Swap the data of a selection
int Sample_Reverse(int32 range_start, int32 range_end)
{
    int32 i;
    short sample;
    char nc;
    long p_s;
    long reversesize = (range_end - range_start) / 2;
 
    if(reversesize)
    {
        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];
        p_s = range_end - 1;

        // Reverse it
        for(i = range_start; i < (range_start + reversesize); i++)
        {
            sample = RawSamples[Current_Instrument][0][Current_Instrument_Split][p_s];
            RawSamples[Current_Instrument][0][Current_Instrument_Split][p_s] = RawSamples[Current_Instrument][0][Current_Instrument_Split][i];
            RawSamples[Current_Instrument][0][Current_Instrument_Split][i] = sample;
            if(nc == 2)
            {
                sample = RawSamples[Current_Instrument][1][Current_Instrument_Split][p_s];
                RawSamples[Current_Instrument][1][Current_Instrument_Split][p_s] = RawSamples[Current_Instrument][1][Current_Instrument_Split][i];
                RawSamples[Current_Instrument][1][Current_Instrument_Split][i] = sample;
            }
            p_s--;
        }
        Status_Box("Reverse Done.");
        return 1;
    }
    return 0;
}

// ------------------------------------------------------
// Convert a selection into a whole sample
int Sample_Crop(int32 range_start, int32 range_end)
{
    int32 i;
    short *NewBuffer[2];
    char nc;
    long p_s;
    long cropsize = (range_end - range_start);

    if(cropsize)
    {
        Lock_Audio_Thread();
        Stop_Current_Instrument();

        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

        NewBuffer[0] = (short *) malloc(cropsize * 2 + 8);
        if(!NewBuffer[0])
        {
            Unlock_Audio_Thread();
            Status_Box("Not enough memory.");
            return 0;
        }
        memset(NewBuffer[0], 0, cropsize * 2 + 8);
        if(nc == 2)
        {
            NewBuffer[1] = (short *) malloc(cropsize * 2 + 8);
            if(!NewBuffer[1])
            {
                free(NewBuffer[0]);
                Unlock_Audio_Thread();
                Status_Box("Not enough memory.");
                return 0;
            }
            memset(NewBuffer[1], 0, cropsize * 2 + 8);
        }

        p_s = 0;

        // Copy the selection
        for(i = range_start; i < range_end; i++)
        {
            *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
            if(nc == 2)
            {
                *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            }
            p_s++;
        }

        // Set the new buffer as current sample
        if(RawSamples[Current_Instrument][0][Current_Instrument_Split])
        {
            free(RawSamples[Current_Instrument][0][Current_Instrument_Split]);
        }
        RawSamples[Current_Instrument][0][Current_Instrument_Split] = NewBuffer[0];
        Player_WL[Current_Instrument][Current_Instrument_Split] = NewBuffer[0];
        if(nc == 2)
        {
            if(RawSamples[Current_Instrument][1][Current_Instrument_Split])
            {
                free(RawSamples[Current_Instrument][1][Current_Instrument_Split]);
            }
            RawSamples[Current_Instrument][1][Current_Instrument_Split] = NewBuffer[1];
            Player_WR[Current_Instrument][Current_Instrument_Split] = NewBuffer[1];
        }
        Sample_Length[Current_Instrument][Current_Instrument_Split] = cropsize;
        Recalculate_Sample_Size(Current_Instrument, Current_Instrument_Split);

        Unlock_Audio_Thread();
        Status_Box("Crop Done.");
        return 1;
    }
    return 0;
}

// ------------------------------------------------------
// Copy part of a sample
int Sample_Copy(int32 range_start, int32 range_end)
{
    int i;
    short *dest_mono;
    short *dest_stereo;
    long copysize = (range_end - range_start);
    char nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

    if(copysize)
    {
        // Free both back buffers
        if(Sample_Back[cur_sample_buffer][0])
        {
            free(Sample_Back[cur_sample_buffer][0]);
        }
        if(Sample_Back[cur_sample_buffer][1])
        {
            free(Sample_Back[cur_sample_buffer][1]);
        }
        Sample_Back[cur_sample_buffer][0] = NULL;
        Sample_Back[cur_sample_buffer][1] = NULL;

        // Copy the data into the back buffer
        Sample_Back[cur_sample_buffer][0] = (short *) malloc(copysize * 2 + 8);
        if(!Sample_Back[cur_sample_buffer][0])
        {
            Status_Box("Not enough memory.");
            return 0;
        }
        memset(Sample_Back[cur_sample_buffer][0], 0, copysize * 2 + 8);
        if(nc == 2)
        {
            Sample_Back[cur_sample_buffer][1] = (short *) malloc(copysize * 2 + 8);
            if(!Sample_Back[cur_sample_buffer][1])
            {
                free(Sample_Back[cur_sample_buffer][0]);
                Status_Box("Not enough memory.");
                return 0;
            }
            memset(Sample_Back[cur_sample_buffer][1], 0, copysize * 2 + 8);
        }
        Sample_Back_Channels[cur_sample_buffer] = nc;
        Sample_Back_Size[cur_sample_buffer] = copysize;

        // Copy the selection into the back buffer
        dest_mono = Sample_Back[cur_sample_buffer][0];
        dest_stereo = Sample_Back[cur_sample_buffer][1];
        for(i = range_start; i < range_end; i++)
        {
            *dest_mono++ = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
            if(nc == 2)
            {
                *dest_stereo++ = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            }
        }
        Status_Box("Copy Done.");
        return 1;
    }
    return 0;
}

// ------------------------------------------------------
// Paste a back buffer into a sample
int Sample_Paste(int32 range_start)
{
    int32 i;
    short *NewBuffer[2];
    char nc;
    long p_s;
    long cutsize = Sample_Back_Size[cur_sample_buffer];
    long newsize = Sample_Length[Current_Instrument][Current_Instrument_Split] + cutsize;

    if(cutsize)
    {
        Lock_Audio_Thread();
        Stop_Current_Instrument();

        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

        // Allocate the destination buffer(s)
        // (We need to clear the second one as the back buffer may not be stereo).
        NewBuffer[0] = (short *) malloc(newsize * 2 + 8);
        if(!NewBuffer[0])
        {
            Unlock_Audio_Thread();
            Status_Box("Not enough memory.");
            return 0;
        }
        memset(NewBuffer[0], 0, newsize * 2 + 8);

        if(nc == 2)
        {
            NewBuffer[1] = (short *) malloc(newsize * 2 + 8);
            if(!NewBuffer[1])
            {
                free(NewBuffer[0]);
                Unlock_Audio_Thread();
                Status_Box("Not enough memory.");
                return 0;
            }
            memset(NewBuffer[1], 0, newsize * 2 + 8);
        }

        p_s = 0;
        if(range_start > 0)
        {
            // Copy the original data into the new buffer
            for(i = 0; i < range_start; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        // Copy the back buffer(s) now
        for(i = 0; i < cutsize; i++)
        {
            *(NewBuffer[0] + p_s) = *(Sample_Back[cur_sample_buffer][0] + i);
            if(Sample_Back_Size[cur_sample_buffer] == 2 && nc == 2)
            {
                *(NewBuffer[1] + p_s) = *(Sample_Back[cur_sample_buffer][1] + i);
            }
            p_s++;
        }

        if((Sample_Length[Current_Instrument][Current_Instrument_Split] - range_start) > 0)
        {
            // Add the rest of the original data
            for(i = range_start; i < (int32) Sample_Length[Current_Instrument][Current_Instrument_Split]; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        // Set the new buffer as current sample
        if(RawSamples[Current_Instrument][0][Current_Instrument_Split])
        {
            free(RawSamples[Current_Instrument][0][Current_Instrument_Split]);
        }
        RawSamples[Current_Instrument][0][Current_Instrument_Split] = NewBuffer[0];
        Player_WL[Current_Instrument][Current_Instrument_Split] = NewBuffer[0];
        if(nc == 2)
        {
            if(RawSamples[Current_Instrument][1][Current_Instrument_Split])
            {
                free(RawSamples[Current_Instrument][1][Current_Instrument_Split]);
            }
            RawSamples[Current_Instrument][1][Current_Instrument_Split] = NewBuffer[1];
            Player_WR[Current_Instrument][Current_Instrument_Split] = NewBuffer[1];
        }
        Sample_Length[Current_Instrument][Current_Instrument_Split] = newsize;
        Recalculate_Sample_Size(Current_Instrument, Current_Instrument_Split);

        Unlock_Audio_Thread();
        Status_Box("Paste Done.");
        return 1;
    }
    return 0;
}

// ------------------------------------------------------
// Cut part of a sample
int Sample_Cut(int32 range_start, int32 range_end, int do_copy)
{
    int32 i;
    short *NewBuffer[2];
    char nc;
    long p_s;
    long cutsize = (range_end - range_start);
    long newsize = Sample_Length[Current_Instrument][Current_Instrument_Split] - cutsize;

    if(newsize)
    {
        Lock_Audio_Thread();
        Stop_Current_Instrument();

        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

        // Allocate the wav with the minus the block to cut
        NewBuffer[0] = (short *) malloc(newsize * 2 + 8);
        if(!NewBuffer[0]) return 0;
        memset(NewBuffer[0], 0, newsize * 2 + 8);
        if(nc == 2)
        {
            NewBuffer[1] = (short *) malloc(newsize * 2 + 8);
            if(!NewBuffer[1])
            {
                free(NewBuffer[0]);
                Unlock_Audio_Thread();
                Status_Box("Not enough memory.");
                return 0;
            }
            memset(NewBuffer[1], 0, newsize * 2 + 8);
        }

        if(do_copy)
        {
            if(!Sample_Copy(range_start, range_end))
            {
                free(NewBuffer[1]);
                free(NewBuffer[0]);
                Unlock_Audio_Thread();
                Status_Box("Not enough memory.");
                return 0;
            }
        }

        p_s = 0;
        if(range_start > 0)
        {
            // Copy the data located before the range start
            for(i = 0; i < range_start; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        if((Sample_Length[Current_Instrument][Current_Instrument_Split] - range_end) > 0)
        {
            // Add the data located after the range end
            for(i = range_end; i < (int32) Sample_Length[Current_Instrument][Current_Instrument_Split]; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        // Set the new buffer as current sample
        if(RawSamples[Current_Instrument][0][Current_Instrument_Split])
        {
            free(RawSamples[Current_Instrument][0][Current_Instrument_Split]);
        }
        RawSamples[Current_Instrument][0][Current_Instrument_Split] = NewBuffer[0];
        Player_WL[Current_Instrument][Current_Instrument_Split] = NewBuffer[0];
        if(nc == 2)
        {
            if(RawSamples[Current_Instrument][1][Current_Instrument_Split])
            {
                free(RawSamples[Current_Instrument][1][Current_Instrument_Split]);
            }
            RawSamples[Current_Instrument][1][Current_Instrument_Split] = NewBuffer[1];
            Player_WR[Current_Instrument][Current_Instrument_Split] = NewBuffer[1];
        }
        Sample_Length[Current_Instrument][Current_Instrument_Split] = newsize;
        Recalculate_Sample_Size(Current_Instrument, Current_Instrument_Split);

        Unlock_Audio_Thread();
        Status_Box("Cut Done.");
        return 1;
    }
    else
    {
        if(do_copy)
        {
            Status_Box("You Cannot Cut Entire Sample, Use 'Delete' On Instrument Instead.");
        }
        else
        {
            Status_Box("You Cannot Zap Entire Sample, Use 'Delete' On Instrument Instead.");
        }
        return 0;
    }
}

// ------------------------------------------------------
// Adjust DC of a sample
void Sample_DC_Adjust(int32 range_start, int32 range_end)
{
    int32 i;
    char nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];
    float l_shift = 0;
    float r_shift = 0;

    for(i = range_start; i < range_end + 1; i++)
    {
        l_shift += *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
        if(nc == 2)
        {
            r_shift += *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
        }
    }

    l_shift /= (range_end + 1) - range_start;
    r_shift /= (range_end + 1) - range_start;

    for(i = range_start; i < range_end + 1; i++)
    {
        float bleak = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
        bleak -= l_shift;

        if(bleak > 32767)
        {
            bleak = 32767;
        }
        if(bleak < -32767)
        {
            bleak = -32767;
        }
        *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i) = (short) bleak;

        if(nc == 2)
        {
            bleak = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            bleak -= r_shift;

            if(bleak > 32767)
            {
                bleak = 32767;
            }
            if(bleak < -32767)
            {
                bleak = -32767;
            }
            *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i) = (short) bleak;
        }
    }

    draw_sampled_wave = TRUE;
    Status_Box("DC Adjust Done.");
}

// ------------------------------------------------------
// Maximize a sample
void Sample_Maximize(int32 range_start, int32 range_end)
{
    int32 i;
    char nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];
    float l_shift = 0;

    for(i = range_start; i < range_end + 1; i++)
    {
        if(abs(*(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i)) > l_shift)
        {
            l_shift = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
        }
        if(nc == 2)
        {
            if(abs(*(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i)) > l_shift)
            {
                l_shift = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            }
        }
    }

    l_shift = 32768.0f / l_shift;

    for(i = range_start; i < range_end + 1; i++)
    {
        float bleak = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
        bleak *= l_shift;

        if(bleak > 32767)
        {
            bleak = 32767;
        }
        if(bleak < -32767)
        {
            bleak = -32767;
        }
        *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i) = (short) bleak;

        if(nc == 2)
        {
            bleak = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            bleak *= l_shift;

            if(bleak > 32767)
            {
                bleak = 32767;
            }
            if(bleak < -32767)
            {
                bleak = -32767;
            }
            *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i) = (short) bleak;
        }
    }

    draw_sampled_wave = TRUE;
    Status_Box("Maximize Done.");
}

// ------------------------------------------------------
// Zeroize a sample
void Sample_Zeroize(int32 range_start, int32 range_end)
{
    int32 i;
    char nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

    for(i = range_start; i < range_end + 1; i++)
    {
        *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i) = 0;
        if(nc == 2)
        {
            *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i) = 0;
        }
    }

    draw_sampled_wave = TRUE;
    Status_Box("Zero Done.");
}

// ------------------------------------------------------
// Fade a sample in
void Sample_FadeIn(int32 range_start, int32 range_end)
{
    int i;
    char nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];
    float c_vol = 0.0f;
    float const coef_vol = 1.0f / ((range_end + 1) - range_start);

    for(i = range_start; i < range_end + 1; i++)
    {
        float bleak = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
        bleak *= c_vol;
        if(bleak > 32767)
        {
            bleak = 32767;
        }
        if(bleak < -32767)
        {
            bleak = -32767;
        }
        *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i) = (short) bleak;

        if(nc == 2)
        {
            bleak = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            bleak *= c_vol;

            if(bleak > 32767)
            {
                bleak = 32767;
            }
            if(bleak < -32767)
            {
                bleak = -32767;
            }
            *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i) = (short) bleak;
        }
        c_vol += coef_vol;
    }

    draw_sampled_wave = TRUE;
    Status_Box("Fade In Done.");
}

// ------------------------------------------------------
// Fade a sample out
void Sample_FadeOut(int32 range_start, int32 range_end)
{
    int32 i;
    char nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];
    float c_vol = 1.0f;
    float const coef_vol = 1.0f / ((range_end + 1) - range_start);

    for(i = range_start; i < range_end + 1; i++)
    {
        float bleak = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
        bleak *= c_vol;
        if(bleak > 32767)
        {
            bleak = 32767;
        }
        if(bleak < -32767)
        {
            bleak = -32767;
        }
        *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i) = (short) bleak;

        if(nc == 2)
        {
            bleak = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            bleak *= c_vol;

            if(bleak > 32767)
            {
                bleak = 32767;
            }
            if(bleak < -32767)
            {
                bleak = -32767;
            }
            *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i) = (short) bleak;
        }
        c_vol -= coef_vol;
    }

    draw_sampled_wave = TRUE;
    Status_Box("Fade Out Done.");
}

// ------------------------------------------------------
// Half a sample
void Sample_Half(int32 range_start, int32 range_end)
{
    int32 i;
    char nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];
    float c_vol = 0.5f;

    for(i = range_start; i < range_end + 1; i++)
    {
        float bleak = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
        bleak *= c_vol;

        *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i) = (short) bleak;
        if(nc == 2)
        {
            bleak = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            bleak *= c_vol;
            *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i) = (short) bleak;
        }
    }

    draw_sampled_wave = TRUE;
    Status_Box("Half Done.");
}

// ------------------------------------------------------
// Duplicate the current selection
int Sample_Duplicate(int32 range_start, int32 range_end)
{
    int32 i;
    short *NewBuffer[2];
    char nc;
    long p_s;
    long cutsize = range_end - range_start;
    long newsize = Sample_Length[Current_Instrument][Current_Instrument_Split] + cutsize;

    if(cutsize)
    {
        Lock_Audio_Thread();
        Stop_Current_Instrument();

        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

        // Allocate the destination buffer(s)
        // (We need to clear the second one as the back buffer may not be stereo).
        NewBuffer[0] = (short *) malloc(newsize * 2 + 8);
        if(!NewBuffer[0])
        {
            Unlock_Audio_Thread();
            Status_Box("Not enough memory.");
            return 0;
        }
        memset(NewBuffer[0], 0, newsize * 2 + 8);

        if(nc == 2)
        {
            NewBuffer[1] = (short *) malloc(newsize * 2 + 8);
            if(!NewBuffer[1])
            {
                free(NewBuffer[0]);
                Unlock_Audio_Thread();
                Status_Box("Not enough memory.");
                return 0;
            }
            memset(NewBuffer[1], 0, newsize * 2 + 8);
        }

        p_s = 0;
        if(range_start > 0)
        {
            // Copy the original data into the new buffer
            for(i = 0; i < range_start; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        // Copy the selection
        for(i = range_start; i < range_end; i++)
        {
            *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
            if(nc == 2)
            {
                *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
            }
            p_s++;
        }

        if((Sample_Length[Current_Instrument][Current_Instrument_Split] - range_start) > 0)
        {
            // Add the rest of the original data
            for(i = range_start; i < (int32) Sample_Length[Current_Instrument][Current_Instrument_Split]; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        // Set the new buffer as current sample
        if(RawSamples[Current_Instrument][0][Current_Instrument_Split])
        {
            free(RawSamples[Current_Instrument][0][Current_Instrument_Split]);
        }
        RawSamples[Current_Instrument][0][Current_Instrument_Split] = NewBuffer[0];
        Player_WL[Current_Instrument][Current_Instrument_Split] = NewBuffer[0];
        if(nc == 2)
        {
            if(RawSamples[Current_Instrument][1][Current_Instrument_Split])
            {
                free(RawSamples[Current_Instrument][1][Current_Instrument_Split]);
            }
            RawSamples[Current_Instrument][1][Current_Instrument_Split] = NewBuffer[1];
            Player_WR[Current_Instrument][Current_Instrument_Split] = NewBuffer[1];
        }
        Sample_Length[Current_Instrument][Current_Instrument_Split] = newsize;
        Recalculate_Sample_Size(Current_Instrument, Current_Instrument_Split);

        Unlock_Audio_Thread();
        Status_Box("Insert Zeroes Done.");
        return 1;
    }
    return 0;
}

// ------------------------------------------------------
// Insert zeroes into a sample the length of the selection buffer
int Sample_InsertZero(int32 range_start, int32 range_end)
{
    int32 i;
    short *NewBuffer[2];
    char nc;
    long p_s;
    long cutsize = range_end - range_start;
    long newsize = Sample_Length[Current_Instrument][Current_Instrument_Split] + cutsize;

    if(cutsize)
    {
        Lock_Audio_Thread();
        Stop_Current_Instrument();

        nc = Sample_Channels[Current_Instrument][Current_Instrument_Split];

        // Allocate the destination buffer(s)
        // (We need to clear the second one as the back buffer may not be stereo).
        NewBuffer[0] = (short *) malloc(newsize * 2 + 8);
        if(!NewBuffer[0])
        {
            Unlock_Audio_Thread();
            Status_Box("Not enough memory.");
            return 0;
        }
        memset(NewBuffer[0], 0, newsize * 2 + 8);

        if(nc == 2)
        {
            NewBuffer[1] = (short *) malloc(newsize * 2 + 8);
            if(!NewBuffer[1])
            {
                free(NewBuffer[0]);
                Unlock_Audio_Thread();
                Status_Box("Not enough memory.");
                return 0;
            }
            memset(NewBuffer[1], 0, newsize * 2 + 8);
        }

        p_s = 0;
        if(range_start > 0)
        {
            // Copy the original data into the new buffer
            for(i = 0; i < range_start; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        // Skip a buffer of the selection's size
        p_s += cutsize;

        if((Sample_Length[Current_Instrument][Current_Instrument_Split] - range_start) > 0)
        {
            // Add the rest of the original data
            for(i = range_start; i < (int32) Sample_Length[Current_Instrument][Current_Instrument_Split]; i++)
            {
                *(NewBuffer[0] + p_s) = *(RawSamples[Current_Instrument][0][Current_Instrument_Split] + i);
                if(nc == 2)
                {
                    *(NewBuffer[1] + p_s) = *(RawSamples[Current_Instrument][1][Current_Instrument_Split] + i);
                }
                p_s++;
            }
        }

        // Set the new buffer as current sample
        if(RawSamples[Current_Instrument][0][Current_Instrument_Split])
        {
            free(RawSamples[Current_Instrument][0][Current_Instrument_Split]);
        }
        RawSamples[Current_Instrument][0][Current_Instrument_Split] = NewBuffer[0];
        Player_WL[Current_Instrument][Current_Instrument_Split] = NewBuffer[0];
        if(nc == 2)
        {
            if(RawSamples[Current_Instrument][1][Current_Instrument_Split])
            {
                free(RawSamples[Current_Instrument][1][Current_Instrument_Split]);
            }
            RawSamples[Current_Instrument][1][Current_Instrument_Split] = NewBuffer[1];
            Player_WR[Current_Instrument][Current_Instrument_Split] = NewBuffer[1];
        }
        Sample_Length[Current_Instrument][Current_Instrument_Split] = newsize;
        Recalculate_Sample_Size(Current_Instrument, Current_Instrument_Split);

        Unlock_Audio_Thread();
        Status_Box("Duplicate Done.");
        return 1;
    }
    return 0;
}

void Recalculate_Sample_Size(int Instrument, int Split)
{
    int c;
    int i;

    if(Player_LE[Instrument][Split] != LoopEnd[Instrument][Split])
    {
        Player_LE[Instrument][Split] = LoopEnd[Instrument][Split];
    }
    if(Player_LS[Instrument][Split] != LoopStart[Instrument][Split])
    {
        Player_LS[Instrument][Split] = LoopStart[Instrument][Split];
    }
    if(Player_LS[Instrument][Split] > Sample_Length[Instrument][Split])
    {
        Player_LS[Instrument][Split] = Sample_Length[Instrument][Split];
    }
    if(Player_LE[Instrument][Split] > Sample_Length[Instrument][Split])
    {
        Player_LE[Instrument][Split] = Sample_Length[Instrument][Split];
    }
    if(Player_LS[Instrument][Split] > Sample_Length[Instrument][Split])
    {
        Player_LS[Instrument][Split] = Sample_Length[Instrument][Split];
    }

    // Remove the loop info if it's no longer relevant
    if(Player_LS[Instrument][Split] >= Player_LE[Instrument][Split])
    {
        Player_LS[Instrument][Split] = 0;
        Player_LE[Instrument][Split] = 0;
        Player_LT[Instrument][Split] = SMP_LOOP_NONE;
        Player_LW[Instrument][Split] = SMP_LOOPING_FORWARD;
    }
    Player_LL[Instrument][Split] = Player_LE[Instrument][Split] - Player_LS[Instrument][Split];
    Player_NS[Instrument][Split] = Sample_Length[Instrument][Split];
    if(Player_LE[Instrument][Split] != LoopEnd[Instrument][Split])
    {
        LoopEnd[Instrument][Split] = Player_LE[Instrument][Split];;
    }
    if(Player_LS[Instrument][Split] != LoopStart[Instrument][Split])
    {
        LoopStart[Instrument][Split] = Player_LS[Instrument][Split];
    }
    sp_Stage[Instrument][Split] = PLAYING_NOSAMPLE;
    sp_Position[Instrument][Split].absolu = 0;

    // Now look if this instrument is being played somewhere
    for(c = 0; c < Songtracks; c++)
    {
        for(i = 0; i < Channels_Polyphony[c]; i++)
        {
            if(sp_channelsample[c][i] == Instrument)
            {
                // Set the waveform for the played split
                if(sp_split[c][i] == Split)
                {
                    Player_WL[c][i] = RawSamples[Instrument][0][Split];
                    if(Sample_Channels[Instrument][Split] == 2)
                    {
                        Player_WR[c][i] = RawSamples[Instrument][1][Split];
                    }
                }
            }
        }
    }           
}
