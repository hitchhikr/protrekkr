// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2024 Franck Charlet.
// All rights reserved.
//
// This file is :
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

#include "include/samples_unpack.h"
#include "include/wavpack.h"

#if defined(PTK_WAVPACK)

const char nbits_table [] =
{
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,     // 0 - 15
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,     // 16 - 31
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,     // 32 - 47
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,     // 48 - 63
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,     // 64 - 79
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,     // 80 - 95
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,     // 96 - 111
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,     // 112 - 127
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,     // 128 - 143
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,     // 144 - 159
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,     // 160 - 175
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,     // 176 - 191
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,     // 192 - 207
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,     // 208 - 223
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,     // 224 - 239
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8      // 240 - 255
};

static const uchar log2_table [] =
{
    0x00, 0x01, 0x03, 0x04, 0x06, 0x07, 0x09, 0x0a, 0x0b, 0x0d, 0x0e, 0x10, 0x11, 0x12, 0x14, 0x15,
    0x16, 0x18, 0x19, 0x1a, 0x1c, 0x1d, 0x1e, 0x20, 0x21, 0x22, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2a,
    0x2c, 0x2d, 0x2e, 0x2f, 0x31, 0x32, 0x33, 0x34, 0x36, 0x37, 0x38, 0x39, 0x3b, 0x3c, 0x3d, 0x3e,
    0x3f, 0x41, 0x42, 0x43, 0x44, 0x45, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
    0x52, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63,
    0x64, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x74, 0x75,
    0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85,
    0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
    0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb2,
    0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc0,
    0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcb, 0xcc, 0xcd, 0xce,
    0xcf, 0xd0, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd8, 0xd9, 0xda, 0xdb,
    0xdc, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe4, 0xe5, 0xe6, 0xe7, 0xe7,
    0xe8, 0xe9, 0xea, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xee, 0xef, 0xf0, 0xf1, 0xf1, 0xf2, 0xf3, 0xf4,
    0xf4, 0xf5, 0xf6, 0xf7, 0xf7, 0xf8, 0xf9, 0xf9, 0xfa, 0xfb, 0xfc, 0xfc, 0xfd, 0xfe, 0xff, 0xff
};

static const uchar exp2_table [] =
{
    0x00, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0a, 0x0b,
    0x0b, 0x0c, 0x0d, 0x0e, 0x0e, 0x0f, 0x10, 0x10, 0x11, 0x12, 0x13, 0x13, 0x14, 0x15, 0x16, 0x16,
    0x17, 0x18, 0x19, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1d, 0x1e, 0x1f, 0x20, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x24, 0x25, 0x26, 0x27, 0x28, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3a, 0x3b, 0x3c, 0x3d,
    0x3e, 0x3f, 0x40, 0x41, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x48, 0x49, 0x4a, 0x4b,
    0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
    0x5b, 0x5c, 0x5d, 0x5e, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87, 0x88, 0x89, 0x8a,
    0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
    0x9c, 0x9d, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad,
    0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
    0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc8, 0xc9, 0xca, 0xcb, 0xcd, 0xce, 0xcf, 0xd0, 0xd2, 0xd3, 0xd4,
    0xd6, 0xd7, 0xd8, 0xd9, 0xdb, 0xdc, 0xdd, 0xde, 0xe0, 0xe1, 0xe2, 0xe4, 0xe5, 0xe6, 0xe8, 0xe9,
    0xea, 0xec, 0xed, 0xee, 0xf0, 0xf1, 0xf2, 0xf4, 0xf5, 0xf6, 0xf8, 0xf9, 0xfa, 0xfc, 0xfd, 0xff
};

static const char ones_count_table [] =
{
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,7,
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,8
};

///////////////////////////// executable code ////////////////////////////////

static int mylog2(uint32_t avalue);

int read_entropy_vars_depack(WavpackStream *wps, WavpackMetadata *wpmd)
{
    uchar *byteptr = (uchar *) wpmd->data;

    if (wpmd->byte_length != 6)
        return FALSE;

    wps->w.c [0].median [0] = exp2s_depack(byteptr [0] + (byteptr [1] << 8));
    wps->w.c [0].median [1] = exp2s_depack(byteptr [2] + (byteptr [3] << 8));
    wps->w.c [0].median [2] = exp2s_depack(byteptr [4] + (byteptr [5] << 8));
    return TRUE;
}

int read_hybrid_profile_depack(WavpackStream *wps, WavpackMetadata *wpmd)
{
    uchar *byteptr = (uchar *) wpmd->data;
    uchar *endptr = byteptr + wpmd->byte_length;

    wps->w.c [0].slow_level = exp2s_depack(byteptr [0] + (byteptr [1] << 8));
    byteptr += 2;

    wps->w.bitrate_acc [0] = (int32_t)(byteptr [0] + (byteptr [1] << 8)) << 16;
    byteptr += 2;

    if (byteptr < endptr)
    {
        wps->w.bitrate_delta [0] = exp2s_depack((short)(byteptr [0] + (byteptr [1] << 8)));
        byteptr += 2;

        if (byteptr < endptr)
        {
            return FALSE;
        }
    }
    else
        wps->w.bitrate_delta [0] = wps->w.bitrate_delta [1] = 0;

    return TRUE;
}

void update_error_limit(struct words_data *w, uint32_t flags)
{
    int bitrate_0 = (w->bitrate_acc [0] += w->bitrate_delta [0]) >> 16;

    int slow_log_0 = (w->c [0].slow_level + SLO) >> SLS;

    if (slow_log_0 - bitrate_0 > -0x100)
    {
        w->c [0].error_limit = exp2s_depack(slow_log_0 - bitrate_0 + 0x100);
    }
    else
    {
        w->c [0].error_limit = 0;
    }
}

static uint32_t read_code(Bitstream *bs, uint32_t maxcode);

int32_t get_words (int32_t *buffer, int nsamples, uint32_t flags,
                   struct words_data *w, Bitstream *bs)
{
    struct entropy_data *c = w->c;
    int csamples;

    for (csamples = 0; csamples < nsamples; ++csamples)
    {
        uint32_t ones_count, low, mid, high;

        if (!(w->c [0].median [0] & ~1) && !w->holding_zero && !w->holding_one && !(w->c [1].median [0] & ~1)) {
            uint32_t mask;
            int cbits;

            if (w->zeros_acc) {
                if (--w->zeros_acc) {
                    c->slow_level -= (c->slow_level + SLO) >> SLS;
                    *buffer++ = 0;
                    continue;
                }
            }
            else {
                for (cbits = 0; cbits < 33 && getbit (bs); ++cbits);

                if (cbits == 33)
                    break;

                if (cbits < 2)
                    w->zeros_acc = cbits;
                else {
                    for (mask = 1, w->zeros_acc = 0; --cbits; mask <<= 1)
                        if (getbit (bs))
                            w->zeros_acc |= mask;

                    w->zeros_acc |= mask;
                }

                if (w->zeros_acc) {
                    c->slow_level -= (c->slow_level + SLO) >> SLS;
                    CLEAR (w->c [0].median);
                    CLEAR (w->c [1].median);
                    *buffer++ = 0;
                    continue;
                }
            }
        }

        if (w->holding_zero)
            ones_count = w->holding_zero = 0;
        else {
            int next8;

            if (bs->bc < 8) {
                if (++(bs->ptr) == bs->end)
                    bs->wrap (bs);

                next8 = (bs->sr |= *(bs->ptr) << bs->bc) & 0xff;
                bs->bc += 8;
            }
            else
                next8 = bs->sr & 0xff;

            if (next8 == 0xff) {
                bs->bc -= 8;
                bs->sr >>= 8;

                for (ones_count = 8; ones_count < (LIMIT_ONES + 1) && getbit (bs); ++ones_count);

                if (ones_count == (LIMIT_ONES + 1))
                    break;

                if (ones_count == LIMIT_ONES)
                {
                    uint32_t mask;
                    int cbits;

                    for (cbits = 0; cbits < 33 && getbit (bs); ++cbits);

                    if (cbits == 33)
                        break;

                    if (cbits < 2)
                        ones_count = cbits;
                    else {
                        for (mask = 1, ones_count = 0; --cbits; mask <<= 1)
                            if (getbit (bs))
                                ones_count |= mask;

                        ones_count |= mask;
                    }

                    ones_count += LIMIT_ONES;
                }
            }
            else {
                bs->bc -= (ones_count = ones_count_table [next8]) + 1;
                bs->sr >>= ones_count + 1;
            }

            if (w->holding_one) {
                w->holding_one = ones_count & 1;
                ones_count = (ones_count >> 1) + 1;
            }
            else {
                w->holding_one = ones_count & 1;
                ones_count >>= 1;
            }

            w->holding_zero = ~w->holding_one & 1;
        }

        if ((1 || !(csamples & 1)))
        {
            update_error_limit (w, flags);
        }
        if (ones_count == 0)
        {
            low = 0;
            high = GET_MED (0) - 1;
            DEC_MED0 ();
        }
        else
        {
            low = GET_MED (0);
            INC_MED0 ();

            if (ones_count == 1) {
                high = low + GET_MED (1) - 1;
                DEC_MED1 ();
            }
            else {
                low += GET_MED (1);
                INC_MED1 ();

                if (ones_count == 2) {
                    high = low + GET_MED (2) - 1;
                    DEC_MED2 ();
                }
                else {
                    low += (ones_count - 2) * GET_MED (2);
                    high = low + GET_MED (2) - 1;
                    INC_MED2 ();
                }
            }
        }

        mid = (high + low + 1) >> 1;

        if (!c->error_limit)
            mid = read_code (bs, high - low) + low;
        else while (high - low > c->error_limit) {
            if (getbit (bs))
                mid = (high + (low = mid) + 1) >> 1;
            else
                mid = ((high = mid - 1) + low + 1) >> 1;
        }

        *buffer++ = getbit (bs) ? ~mid : mid;

        c->slow_level = c->slow_level - ((c->slow_level + SLO) >> SLS) + mylog2 (mid);
    }

    return csamples;
}

static uint32_t read_code (Bitstream *bs, uint32_t maxcode)
{
    int bitcount = count_bits (maxcode);
    uint32_t extras = (1L << bitcount) - maxcode - 1, code;

    if (!bitcount)
        return 0;

    getbits (&code, bitcount - 1, bs);
    code &= (1L << (bitcount - 1)) - 1;

    if (code >= extras)
    {
        code = (code << 1) - extras;
        if (getbit (bs))
            ++code;
    }

    return code;
}

static int mylog2 (uint32_t avalue)
{
    int dbits;
    if ((avalue += avalue >> 9) < (1 << 8))
    {
        dbits = nbits_table [avalue];
        return (dbits << 8) + log2_table [(avalue << (9 - dbits)) & 0xff];
    }
    else
    {
        if (avalue < (1L << 16))
            dbits = nbits_table [avalue >> 8] + 8;
        else if (avalue < (1L << 24))
            dbits = nbits_table [avalue >> 16] + 16;
        else
            dbits = nbits_table [avalue >> 24] + 24;

        return (dbits << 8) + log2_table [(avalue >> (dbits - 9)) & 0xff];
    }
}

int32_t exp2s_depack (int log)
{
    uint32_t value;

    if (log < 0)
        return -exp2s_depack(-log);

    value = exp2_table [log & 0xff] | 0x100;

    if ((log >>= 8) <= 9)
        return value >> (9 - log);
    else
        return value << (log - 9);
}

int restore_weight_depack(signed char weight)
{
    int result;

    if ((result = (int) weight << 3) > 0)
    {
        result += (result + 64) >> 7;
    }
    return result;
}

static void bs_read (Bitstream *bs);

void bs_open_read(Bitstream *bs,
                  uchar *buffer_start,
                  uchar *buffer_end,
                  uint32_t file_bytes)
{
    CLEAR(*bs);
    bs->buf = buffer_start;
    bs->end = buffer_end;
    bs->ptr = bs->end - 1;
    bs->file_bytes = file_bytes;
    bs->wrap = bs_read;
}

static void bs_read(Bitstream *bs)
{
    if (bs->file_bytes)
    {
        uint32_t bytes_read, bytes_to_read = bs->end - bs->buf;
        if (bytes_to_read > bs->file_bytes)
        {
            bytes_to_read = bs->file_bytes;
        }
        bytes_read = read_bytes(bs->buf, bytes_to_read);
        if (bytes_read)
        {
            bs->end = bs->buf + bytes_read;
            bs->file_bytes -= bytes_read;
        }
        else
        {
            memset (bs->buf, -1, bs->end - bs->buf);
            bs->error = 1;
        }
    }
    else
    {
        bs->error = 1;
    }
    if (bs->error)
    {
        memset (bs->buf, -1, bs->end - bs->buf);
    }
    bs->ptr = bs->buf;
}

int read_metadata_buff (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    uchar tchar;

    if (!read_bytes(&wpmd->id, 1) || !read_bytes(&tchar, 1))
    {
        return FALSE;
    }
    wpmd->byte_length = tchar << 1;

    if (wpmd->id & ID_LARGE)
    {
        wpmd->id &= ~ID_LARGE;

        if (!read_bytes(&tchar, 1))
        {
            return FALSE;
        }
        wpmd->byte_length += (int32_t) tchar << 9; 

        if (!read_bytes(&tchar, 1))
        {
            return FALSE;
        }
        wpmd->byte_length += (int32_t) tchar << 17;
    }

    if (wpmd->id & ID_ODD_SIZE)
    {
        wpmd->id &= ~ID_ODD_SIZE;
        wpmd->byte_length--;
    }

    if (wpmd->byte_length && wpmd->byte_length <= sizeof (wpc->read_buffer))
    {
        uint32_t bytes_to_read = wpmd->byte_length + (wpmd->byte_length & 1);

        if (read_bytes(wpc->read_buffer, bytes_to_read) != (int32_t) bytes_to_read)
        {
            wpmd->data = NULL;
            return FALSE;
        }
        wpmd->data = wpc->read_buffer;
    }
    else
    {
        wpmd->data = NULL;
    }
    return TRUE;
}

int process_metadata(WavpackContext *wpc, WavpackMetadata *wpmd)
{
    WavpackStream *wps = &wpc->stream;

    switch (wpmd->id)
    {
        case ID_DECORR_TERMS:
            return read_decorr_terms(wps, wpmd);
        case ID_DECORR_WEIGHTS:
            return read_decorr_weights(wps, wpmd);
        case ID_DECORR_SAMPLES:
            return read_decorr_samples(wps, wpmd);
        case ID_ENTROPY_VARS:
            return read_entropy_vars_depack(wps, wpmd);
        case ID_HYBRID_PROFILE:
            return read_hybrid_profile_depack(wps, wpmd);
        case ID_CONFIG_BLOCK:
            return read_config_info(wpc, wpmd);
        case ID_WV_BITSTREAM:
            return init_wv_bitstream(wpc, wpmd);
        default:
            return 0;
    }
}

int unpack_init (WavpackContext *wpc)
{
    WavpackStream *wps = &wpc->stream;
    WavpackMetadata wpmd;

    if (wps->wphdr.block_samples && wps->wphdr.block_index != (uint32_t) -1)
    {
        wps->sample_index = wps->wphdr.block_index;
    }
    wps->mute_error = FALSE;
    CLEAR (wps->wvbits);
    CLEAR (wps->decorr_passes);
    CLEAR (wps->w);

    while (read_metadata_buff(wpc, &wpmd))
    {
        if (!process_metadata(wpc, &wpmd))
        {
            return FALSE;
        }
        if (wpmd.id == ID_WV_BITSTREAM) break;
    }
    if (wps->wphdr.block_samples && !bs_is_open (&wps->wvbits))
    {
        return FALSE;
    }
    return TRUE;
}

int init_wv_bitstream (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    WavpackStream *wps = &wpc->stream;

    if (wpmd->data)
    {
        bs_open_read(&wps->wvbits,
                      (uchar *) wpmd->data,
                      (unsigned char *) wpmd->data + wpmd->byte_length,
                      0);
    }
    else
    if (wpmd->byte_length)
    {
        bs_open_read (&wps->wvbits,
                      wpc->read_buffer,
                      wpc->read_buffer + sizeof (wpc->read_buffer),
                      wpmd->byte_length + (wpmd->byte_length & 1)
                     );
    }
    return TRUE;
}

int read_decorr_terms (WavpackStream *wps, WavpackMetadata *wpmd)
{
    int termcnt = wpmd->byte_length;
    uchar *byteptr = (uchar *) wpmd->data;
    struct decorr_pass *dpp;

    if (termcnt > MAX_NTERMS)
    {
        return FALSE;
    }
    wps->num_terms = termcnt;

    for (dpp = wps->decorr_passes + termcnt - 1; termcnt--; dpp--)
    {
        dpp->term = (int)(*byteptr & 0x1f) - 5;
        dpp->delta = (*byteptr++ >> 5) & 0x7;

        if (!dpp->term || dpp->term < -3 || (dpp->term > MAX_TERM && dpp->term < 17) || dpp->term > 18)
        {
            return FALSE;
        }
    }
    return TRUE;
}

int read_decorr_weights (WavpackStream *wps, WavpackMetadata *wpmd)
{
    int termcnt = wpmd->byte_length, tcount;
    signed char *byteptr = (signed char *) wpmd->data;
    struct decorr_pass *dpp;

    if (termcnt > wps->num_terms)
    {
        return FALSE;
    }
    for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++)
    {
        dpp->weight_A = dpp->weight_B = 0;
    }
    while (--dpp >= wps->decorr_passes && termcnt--)
    {
        dpp->weight_A = restore_weight_depack(*byteptr++);
    }
    return TRUE;
}

int read_decorr_samples (WavpackStream *wps, WavpackMetadata *wpmd)
{
    uchar *byteptr = (uchar *) wpmd->data;
    uchar *endptr = byteptr + wpmd->byte_length;
    struct decorr_pass *dpp;
    int tcount;

    for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++)
    {
        CLEAR (dpp->samples_A);
        CLEAR (dpp->samples_B);
    }

    while (dpp-- > wps->decorr_passes && byteptr < endptr)
    {
        if (dpp->term > MAX_TERM)
        {
            dpp->samples_A [0] = exp2s_depack((short)(byteptr [0] + (byteptr [1] << 8)));
            dpp->samples_A [1] = exp2s_depack((short)(byteptr [2] + (byteptr [3] << 8)));
            byteptr += 4;
        }
        else if (dpp->term < 0)
        {
            dpp->samples_A [0] = exp2s_depack((short)(byteptr [0] + (byteptr [1] << 8)));
            dpp->samples_B [0] = exp2s_depack((short)(byteptr [2] + (byteptr [3] << 8)));
            byteptr += 4;
        }
        else
        {
            int m = 0, cnt = dpp->term;

            while (cnt--)
            {
                dpp->samples_A [m] = exp2s_depack((short)(byteptr [0] + (byteptr [1] << 8)));
                byteptr += 2;
                m++;
            }
        }
    }
    return byteptr == endptr;
}

int read_config_info (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    int bytecnt = wpmd->byte_length;
    uchar *byteptr = (uchar *) wpmd->data;

    if (bytecnt >= 3)
    {
        wpc->config.flags &= 0xff;
        wpc->config.flags |= (int32_t) *byteptr++ << 8;
        wpc->config.flags |= (int32_t) *byteptr++ << 16;
        wpc->config.flags |= (int32_t) *byteptr << 24;
    }

    return TRUE;
}

int32_t unpack_samples (WavpackContext *wpc, int32_t *buffer, uint32_t sample_count)
{
    WavpackStream *wps = &wpc->stream;
    uint32_t flags = wps->wphdr.flags, i;
    int32_t mute_limit = (1L << ((flags & MAG_MASK) >> MAG_LSB)) + 2;
    struct decorr_pass *dpp;
    int32_t *bptr, *eptr;
    int tcount;

    if (wps->sample_index + sample_count > wps->wphdr.block_index + wps->wphdr.block_samples)
    {
        sample_count = wps->wphdr.block_index + wps->wphdr.block_samples - wps->sample_index;
    }

    if (wps->mute_error)
    {
        memset (buffer, 0, sample_count * 4);
        wps->sample_index += sample_count;
        return sample_count;
    }

    mute_limit *= 2;

    ///////////////////// handle version 4 mono data /////////////////////////

    eptr = buffer + sample_count;
    i = get_words (buffer, sample_count, flags, &wps->w, &wps->wvbits);

    for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++)
    {
        decorr_mono_pass (dpp, buffer, sample_count);
    }
    for (bptr = buffer; bptr < eptr; ++bptr)
    {
        if (labs (bptr [0]) > mute_limit)
        {
            i = bptr - buffer;
            break;
        }
    }

    if (i != sample_count)
    {
        memset (buffer, 0, sample_count * 4);
        wps->mute_error = TRUE;
        i = sample_count;
    }

    fixup_samples (wps, buffer, i);

    wps->sample_index += i;

    return i;
}

void decorr_mono_pass (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count)
{
    int32_t delta = dpp->delta, weight_A = dpp->weight_A;
    int32_t *bptr, *eptr = buffer + sample_count, sam_A;
    int m, k;

    switch (dpp->term)
    {
        case 17:
            for (bptr = buffer; bptr < eptr; bptr++)
            {
                sam_A = 2 * dpp->samples_A [0] - dpp->samples_A [1];
                dpp->samples_A [1] = dpp->samples_A [0];
                dpp->samples_A [0] = apply_weight (weight_A, sam_A) + bptr [0];
                update_weight (weight_A, delta, sam_A, bptr [0]);
                bptr [0] = dpp->samples_A [0];
            }

            break;

        case 18:
            for (bptr = buffer; bptr < eptr; bptr++)
            {
                sam_A = (3 * dpp->samples_A [0] - dpp->samples_A [1]) >> 1;
                dpp->samples_A [1] = dpp->samples_A [0];
                dpp->samples_A [0] = apply_weight (weight_A, sam_A) + bptr [0];
                update_weight (weight_A, delta, sam_A, bptr [0]);
                bptr [0] = dpp->samples_A [0];
            }

            break;

        default:
            for (m = 0, k = dpp->term & (MAX_TERM - 1), bptr = buffer; bptr < eptr; bptr++)
            {
                sam_A = dpp->samples_A [m];
                dpp->samples_A [k] = apply_weight (weight_A, sam_A) + bptr [0];
                update_weight (weight_A, delta, sam_A, bptr [0]);
                bptr [0] = dpp->samples_A [k];
                m = (m + 1) & (MAX_TERM - 1);
                k = (k + 1) & (MAX_TERM - 1);
            }

            if (m)
            {
                int32_t temp_samples [MAX_TERM];
                memcpy (temp_samples, dpp->samples_A, sizeof (dpp->samples_A));
                for (k = 0; k < MAX_TERM; k++, m++)
                {
                    dpp->samples_A [k] = temp_samples [m & (MAX_TERM - 1)];
                }
            }

            break;
    }

    dpp->weight_A = (short) weight_A;
}

void fixup_samples (WavpackStream *wps, int32_t *buffer, uint32_t sample_count)
{
    uint32_t flags = wps->wphdr.flags;
    int shift = (flags & SHIFT_MASK) >> SHIFT_LSB;

    int32_t min_value, max_value, min_shifted, max_shifted;

    min_shifted = (min_value = -32767 >> shift) << shift;
    max_shifted = (max_value = 32767 >> shift) << shift;

    while (sample_count--)
    {
        if (*buffer < min_value)
        {
            *buffer++ = min_shifted;
        }
        else if (*buffer > max_value)
        {
            *buffer++ = max_shifted;
        }
        else
        {
            *buffer++ <<= shift;
        }
    }
}

static WavpackContext wpc;

WavpackContext *WavpackOpenFileInput(unsigned char *Source, int Source_Size)
{
    WavpackStream *wps = &wpc.stream;
    uint32_t bcount;

    CLEAR (wpc);
    wpc.Source = Source;
    wpc.Pos_Source = 0;
    wpc.Source_Size = Source_Size;
    wpc.total_samples = (uint32_t) -1;

    // open the source file for reading and store the size
    read_bytes(&wps->wphdr, sizeof(WavpackHeader));
    bcount = sizeof(WavpackHeader);
    wpc.total_samples = wps->wphdr.total_samples;
    if(!unpack_init(&wpc)) return NULL;
    wpc.config.flags &= ~0xff;
    wpc.config.flags |= wps->wphdr.flags & 0xff;
    return &wpc;
}

uint32_t WavpackUnpackSamples (WavpackContext *wpc,
                               int32_t *buffer, uint32_t samples)
{
    WavpackStream *wps = &wpc->stream;
    uint32_t bcount, samples_unpacked = 0, samples_to_unpack;

    while (samples)
    {
        if (!wps->wphdr.block_samples || !(wps->wphdr.flags & INITIAL_BLOCK) ||
            wps->sample_index >= wps->wphdr.block_index + wps->wphdr.block_samples)
        {
                bcount = read_next_header(&wps->wphdr);

                if (bcount == (uint32_t) -1)
                {
                    break;
                }
                if (!wps->wphdr.block_samples || wps->sample_index == wps->wphdr.block_index)
                {
                    if (!unpack_init (wpc))
                    {
                        break;
                    }
                }
        }

        if (!wps->wphdr.block_samples || !(wps->wphdr.flags & INITIAL_BLOCK) ||
            wps->sample_index >= wps->wphdr.block_index + wps->wphdr.block_samples)
        {
                continue;
        }

        if (wps->sample_index < wps->wphdr.block_index)
        {
            samples_to_unpack = wps->wphdr.block_index - wps->sample_index;

            if (samples_to_unpack > samples)
            {
                samples_to_unpack = samples;
            }
            wps->sample_index += samples_to_unpack;
            samples_unpacked += samples_to_unpack;
            samples -= samples_to_unpack;
            while (samples_to_unpack--)
            {
                *buffer++ = 0;
            }
            continue;
        }

        samples_to_unpack = wps->wphdr.block_index + wps->wphdr.block_samples - wps->sample_index;

        if (samples_to_unpack > samples)
        {
            samples_to_unpack = samples;
        }
        unpack_samples(wpc, buffer, samples_to_unpack);

        buffer += samples_to_unpack;
        samples_unpacked += samples_to_unpack;
        samples -= samples_to_unpack;

        if (wps->sample_index == wpc->total_samples)
        {
            break;
        }
    }

    return samples_unpacked;
}

uint32_t read_next_header (WavpackHeader *wphdr)
{
    char buffer [sizeof (*wphdr)];
    char *sp = buffer + sizeof (*wphdr);
    char *ep = sp;
    uint32_t bytes_skipped = 0;
    int bleft;

    while (1)
    {
        if (sp < ep)
        {
            bleft = ep - sp;
            memcpy (buffer, sp, bleft);
        }
        else
        {
            bleft = 0;
        }
        if (read_bytes(buffer + bleft, sizeof (*wphdr) - bleft) != 
            (int32_t) sizeof (*wphdr) - bleft)
        {
            return -1;
        }
        sp = buffer;
        sp += 4;
        while (sp < ep && *sp != 'w')
        {
            sp++;
        }
        if ((bytes_skipped += sp - buffer) > 1048576L)
        {
            return -1;
        }
    }
}

int32_t read_bytes(void *buff, int32_t bcount)
{
    if(wpc.Pos_Source + bcount > wpc.Source_Size)
    {
        bcount = wpc.Source_Size - wpc.Pos_Source;
    }
    memcpy(buff, wpc.Source + wpc.Pos_Source, bcount);
    wpc.Pos_Source += bcount;
    return bcount;
}

void UnpackWavPack(Uint8 *Source, short *Dest, int Src_Size, int Dst_Size)
{
    int32_t temp;
    int32_t temp_buffer[256];
    WavpackContext *wpc;
    uchar *bDest = (uchar *) Dest;

    wpc = WavpackOpenFileInput(Source, Src_Size);
    while (1)
    {
        int32_t *buf = temp_buffer;
        memset(buf, 0, 256 * sizeof(int32_t));
        uint32_t samples_unpacked = WavpackUnpackSamples(wpc, buf, 256);
        if (samples_unpacked)
        {
            while (samples_unpacked--)
            {
#if defined(__BIG_ENDIAN__)
                *bDest++ = (uchar)((temp = *buf++) >> 8);
                *bDest++ = (uchar)(temp & 0xff);
#else
                *bDest++ = (uchar)(temp = *buf++);
                *bDest++ = (uchar)(temp >> 8);
#endif
            }
        }
        else
        {
            break;
        }
    }
}

#endif
