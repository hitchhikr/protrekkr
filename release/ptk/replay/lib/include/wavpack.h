// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2026 Franck Charlet.
// All rights reserved.
//
// This file is:
// Copyright (c) 1998-2007 Conifer Software.
// All right reserved.
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

#ifndef _WAVPACK_H_
#define _WAVPACK_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if !defined(__PSP__)
    #if !defined(__AMIGAOS4__)
        #include <memory.h>
    #endif
#endif

typedef struct
{
    UINT32 ckSize;
    UINT32 total_samples;
    UINT32 block_index;
    UINT32 block_samples;
    UINT32 flags;
} WavpackHeader;

#define INT32_DATA      0x100   // special extended int handling

#define INITIAL_BLOCK   0x800   // initial block of multichannel segment
#define FINAL_BLOCK     0x1000  // final block of multichannel segment

#define SHIFT_LSB       13
#define SHIFT_MASK      (0x1fL << SHIFT_LSB)

#define MAG_LSB         18
#define MAG_MASK        (0x1fL << MAG_LSB)

#define SRATE_LSB       23
#define SRATE_MASK      (0xfL << SRATE_LSB)

#define IGNORED_FLAGS   0x18000000      // reserved, but ignore if encountered
#define UNKNOWN_FLAGS   0x80000000      // also reserved, but refuse decode if
                                        //  encountered

#define MIN_STREAM_VERS     0x402       // lowest stream version we'll decode
#define MAX_STREAM_VERS     0x410       // highest stream version we'll decode

#define LIMIT_ONES 16   // maximum consecutive 1s sent for "div" data

#define SLS 8
#define SLO ((1 << (SLS - 1)))

// these control the time constant of the 3 median level breakpoints
#define DIV0 128        // 5/7 of samples
#define DIV1 64         // 10/49 of samples
#define DIV2 32         // 20/343 of samples

#define GET_MED(med) (((c->median [med]) >> 4) + 1)

#define INC_MED0() (c->median [0] += ((c->median [0] + DIV0) / DIV0) * 5)
#define DEC_MED0() (c->median [0] -= ((c->median [0] + (DIV0-2)) / DIV0) * 2)
#define INC_MED1() (c->median [1] += ((c->median [1] + DIV1) / DIV1) * 5)
#define DEC_MED1() (c->median [1] -= ((c->median [1] + (DIV1-2)) / DIV1) * 2)
#define INC_MED2() (c->median [2] += ((c->median [2] + DIV2) / DIV2) * 5)
#define DEC_MED2() (c->median [2] -= ((c->median [2] + (DIV2-2)) / DIV2) * 2)

#define count_bits(av) ((av) < (1 << 8) ? nbits_table [av] : ((av) < (1L << 16) ? nbits_table [(av) >> 8] + 8 : ((av) < (1L << 24) ? nbits_table [(av) >> 16] + 16 : nbits_table [(av) >> 24] + 24)))

typedef struct
{
    INT32 byte_length;
    void *data;
    UINT8 id;
} WavpackMetadata;

#define ID_CONFIG_BLOCK         0x20
#define ID_ODD_SIZE             0x40
#define ID_LARGE                0x80

#define ID_DECORR_TERMS         0x1
#define ID_DECORR_WEIGHTS       0x2
#define ID_DECORR_SAMPLES       0x3
#define ID_ENTROPY_VARS         0x4
#define ID_HYBRID_PROFILE       0x5
#define ID_WV_BITSTREAM         0x6

typedef struct {
    UINT32 flags;
} WavpackConfig;

typedef struct bs
{
    UINT8 *buf, *end, *ptr;
    void (*wrap)(struct bs *bs);
    UINT32 file_bytes, sr;
    int error, bc;
} Bitstream;

#define MAX_NTERMS 16
#define MAX_TERM 8

struct decorr_pass
{
    short term, delta, weight_A, weight_B;
    INT32 samples_A [MAX_TERM], samples_B [MAX_TERM];
};

struct entropy_data
{
    UINT32 median [3], slow_level, error_limit;
};

struct words_data
{
    UINT32 bitrate_delta [2], bitrate_acc [2];
    UINT32 pend_data, holding_one, zeros_acc;
    int holding_zero, pend_count;
    struct entropy_data c [2];
};

typedef struct
{
    WavpackHeader wphdr;
    Bitstream wvbits;
    struct words_data w;
    int num_terms, mute_error;
    UINT32 sample_index;
    struct decorr_pass decorr_passes[MAX_NTERMS];
} WavpackStream;

typedef struct
{
    WavpackConfig config;
    WavpackStream stream;
    UINT8 read_buffer[512];
    UINT32 total_samples;
    unsigned char *Source;
    int Pos_Source;
    int Source_Size;
} WavpackContext;

#define CLEAR(destin) memset (&destin, 0, sizeof (destin));

void bs_open_read (Bitstream *bs,
                   UINT8 *buffer_start,
                   UINT8 *buffer_end,
                   UINT32 file_bytes);

#define bs_is_open(bs) ((bs)->ptr != NULL)

#define getbit(bs) ( (((bs)->bc) ? ((bs)->bc--, (bs)->sr & 1) : (((++((bs)->ptr) != (bs)->end) ? (void) 0 : (bs)->wrap (bs)), (bs)->bc = 7, ((bs)->sr = *((bs)->ptr)) & 1)) ? ((bs)->sr >>= 1, 1) : ((bs)->sr >>= 1, 0))

#define getbits(value, nbits, bs) { while ((nbits) > (bs)->bc) { if (++((bs)->ptr) == (bs)->end) (bs)->wrap (bs); (bs)->sr |= (INT32)*((bs)->ptr) << (bs)->bc; (bs)->bc += 8; } *(value) = (bs)->sr; if ((bs)->bc > 32) { (bs)->bc -= (nbits); (bs)->sr = *((bs)->ptr) >> (8 - (bs)->bc); } else { (bs)->bc -= (nbits); (bs)->sr >>= (nbits); } }

#if defined(_DEBUG)
void native_to_little_endian (void *data, char *format);
#endif
#define apply_weight_i(weight, sample) ((weight * sample + 512) >> 10)
#define apply_weight_f(weight, sample) (((((sample & 0xffffL) * weight) >> 9) + (((sample & ~0xffffL) >> 9) * weight) + 1) >> 1)
#define apply_weight(weight, sample) (sample != (short) sample ? apply_weight_f (weight, sample) : apply_weight_i (weight, sample))

#if 0   // PERFCOND
#define update_weight(weight, delta, source, result) if (source && result) { INT32 s = (INT32) (source ^ result) >> 31; weight = (delta ^ s) + (weight - s); }
#elif 1
#define update_weight(weight, delta, source, result) if (source && result) weight += (((source ^ result) >> 30) | 1) * delta
#else
#define update_weight(weight, delta, source, result) if (source && result) (source ^ result) < 0 ? (weight -= delta) : (weight += delta)
#endif

int unpack_init (WavpackContext *wpc);
int init_wv_bitstream (WavpackContext *wpc, WavpackMetadata *wpmd);
int read_decorr_terms (WavpackStream *wps, WavpackMetadata *wpmd);
int read_decorr_weights (WavpackStream *wps, WavpackMetadata *wpmd);
int read_decorr_samples (WavpackStream *wps, WavpackMetadata *wpmd);
int read_config_info (WavpackContext *wpc, WavpackMetadata *wpmd);
INT32 unpack_samples (WavpackContext *wpc, INT32 *buffer, UINT32 sample_count);
int read_metadata_buff (WavpackContext *wpc, WavpackMetadata *wpmd);
int process_metadata (WavpackContext *wpc, WavpackMetadata *wpmd);
int read_entropy_vars (WavpackStream *wps, WavpackMetadata *wpmd);
int read_hybrid_profile (WavpackStream *wps, WavpackMetadata *wpmd);
INT32 get_words (INT32 *buffer, int nsamples, UINT32 flags,
                   struct words_data *w, Bitstream *bs);
INT32 exp2s_depack(int log);
int restore_weight_depack(signed char weight);
WavpackContext *WavpackOpenFileInput(unsigned char *Source, int Source_Size);
UINT32 WavpackUnpackSamples (WavpackContext *wpc, INT32 *buffer, UINT32 samples);
UINT32 read_next_header (WavpackHeader *wphdr);
INT32 read_bytes (void *buff, INT32 bcount);
void decorr_mono_pass (struct decorr_pass *dpp, INT32 *buffer, INT32 sample_count);
void fixup_samples (WavpackStream *wps, INT32 *buffer, UINT32 sample_count);

#endif
