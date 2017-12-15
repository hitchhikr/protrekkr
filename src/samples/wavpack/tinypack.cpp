////////////////////////////////////////////////////////////////////////////
//                           **** WAVPACK ****                            //
//                  Hybrid Lossless Wavefile Compressor                   //
//              Copyright (c) 1998 - 2007 Conifer Software.               //
//                          All Rights Reserved.                          //
//      Distributed under the BSD Software License (see license.txt)      //
////////////////////////////////////////////////////////////////////////////

// tinypack.c

// This is the main module for the demonstration WavPack command-line
// encoder using the "tiny encoder". It accepts a source WAV file, a
// destination WavPack file (.wv) and an optional WavPack correction file
// (.wvc) on the command-line. It supports all 4 encoding qualities in
// pure lossless, hybrid lossy and hybrid lossless modes. Valid input are
// mono or stereo integer WAV files with bitdepths from 8 to 24.

// This program (and the tiny encoder) do not handle placing the WAV RIFF
// header into the WavPack file. The latest version of the regular WavPack
// unpacker (4.40) and the "tiny decoder" will generate the RIFF header
// automatically on unpacking. However, older versions of the command-line
// program will complain about this and require unpacking in "raw" mode.

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>

//nclude "include/wavpack.h"
#include "include/wplocal.h"

#define CLEAR(destin) memset (&destin, 0, sizeof (destin));

/////////////////////////// local function declarations ///////////////////////

int pack_audio(short *Source, short *Dest, int Size);

#define NO_ERROR 0L
#define SOFT_ERROR 1
#define HARD_ERROR 2

static int write_block (void *wpc, void *data, int32_t length)
{
    WavpackContext *wpc_loc = (WavpackContext *) wpc;
    if (wpc_loc->Dest && data && length)
    {
        if(length / wpc_loc->config.bytes_per_sample <= wpc_loc->config.block_samples)
        {
            memcpy(wpc_loc->Dest, data, length);
            wpc_loc->Dest += length;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

// This function handles the actual audio data compression. It assumes that the
// input file is positioned at the beginning of the audio data and that the
// WavPack configuration has been set. This is where the conversion from RIFF
// little-endian standard the executing processor's format is done.

#define BIT_BUFFER_SIZE 65536   // This should be carefully chosen for the
#define INPUT_SAMPLES (sizeof (int) == 2 ? 4096 : 65536)
static uchar wv_buffer [BIT_BUFFER_SIZE];

int pack_audio(short *Source, short *Dest, int Size)
{
    uint32_t samples_remaining;
    int bytes_per_sample;
    WavpackContext wpc;
    uint32_t sample_count = 0;
    int32_t *large_buffer;
    uchar *cSource = (uchar *) Source;

    CLEAR(wpc);
    wpc.config.bitrate = (int)(2.0f * 256.0f);
    wpc.config.flags = CONFIG_VERY_HIGH_FLAG | CONFIG_HIGH_FLAG;
    wpc.config.sample_rate = 44100;
    wpc.config.bytes_per_sample = 2;
    wpc.config.bits_per_sample = 16;
    wpc.config.num_channels = 1;
    wpc.config.block_samples = Size;
    wpc.Dest = (char *) Dest;
    wpc.blockout = write_block;
    wpc.stream.blockbuff = wv_buffer;
    wpc.stream.blockend = wv_buffer + sizeof (wv_buffer);

    WavpackSetConfiguration (&wpc, &wpc.config, Size);

    large_buffer = (int32_t *) malloc(Size * wpc.config.bytes_per_sample * sizeof(int32_t));
    if(large_buffer)
    {
        WavpackPackInit (&wpc);
        bytes_per_sample = WavpackGetBytesPerSample(&wpc) * WavpackGetNumChannels(&wpc);
        samples_remaining = WavpackGetNumSamples(&wpc);

        while (1)
        {
            uint32_t bytes_to_read = 0;
            uint32_t bytes_read = 0;

            if (samples_remaining > INPUT_SAMPLES)
            {
                // Read a complete block
                bytes_to_read = INPUT_SAMPLES * bytes_per_sample;
            }
            else
            {
                // Read the remainder
                bytes_to_read = samples_remaining * bytes_per_sample;
            }
            samples_remaining -= bytes_to_read / bytes_per_sample;
            bytes_read = bytes_to_read;
            sample_count = bytes_read / bytes_per_sample;

            if (sample_count <= 0)
            {
                break;
            }
            if (sample_count)
            {
                uint32_t cnt = sample_count;
                uchar *sptr = cSource;
                int32_t *dptr = large_buffer;

                while (cnt--)
                {
#if defined(__BIG_ENDIAN__)
                    *dptr++ = sptr[1] | ((int32_t)(signed char) sptr[0] << 8);
#else
                    *dptr++ = sptr[0] | ((int32_t)(signed char) sptr[1] << 8);
#endif
                    sptr += 2;
                }
            }

            if (!WavpackPackSamples (&wpc, large_buffer, sample_count))
            {
                free(large_buffer);
                return 0;
            }
            cSource += bytes_read;
        }
        
        if (!WavpackFlushSamples (&wpc))
        {
            free(large_buffer);
            return 0;
        }

        free(large_buffer);
        return WavpackGetFileSize(&wpc);
    }
    return 0;
}
