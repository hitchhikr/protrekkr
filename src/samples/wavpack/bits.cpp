////////////////////////////////////////////////////////////////////////////
//                           **** WAVPACK ****                            //
//                  Hybrid Lossless Wavefile Compressor                   //
//              Copyright (c) 1998 - 2007 Conifer Software.               //
//                          All Rights Reserved.                          //
//      Distributed under the BSD Software License (see license.txt)      //
////////////////////////////////////////////////////////////////////////////

// bits.c

// This module provides utilities to support the BitStream structure which is
// used to read and write all WavPack audio data streams. It also contains a
// wrapper for the stream I/O functions and a set of functions dealing with
// endian-ness, both for enhancing portability. Finally, a debug wrapper for
// the malloc() system is provided.

#include "include/wplocal.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////// Bitstream functions ////////////////////////////////

// Open the specified BitStream using the specified buffer pointers. It is
// assumed that enough buffer space has been allocated for all data that will
// be written, otherwise an error will be generated.

static void bs_write (Bitstream *bs);

void bs_open_write (Bitstream *bs, uchar *buffer_start, uchar *buffer_end)
{
    bs->error = bs->sr = bs->bc = 0;
    bs->ptr = bs->buf = buffer_start;
    bs->end = buffer_end;
    bs->wrap = bs_write;
}

// This function is only called from the putbit() and putbits() macros when
// the buffer is full, which is now flagged as an error.

static void bs_write (Bitstream *bs)
{
    bs->ptr = bs->buf;
    bs->error = 1;
}

// This function calculates the approximate number of bytes remaining in the
// bitstream buffer and can be used as an early-warning of an impending overflow.

uint32_t bs_remain_write (Bitstream *bs)
{
    if (bs->error)
        return (uint32_t) -1;

    return bs->end - bs->ptr;
}

// This function forces a flushing write of the specified BitStream, and
// returns the total number of bytes written into the buffer.

uint32_t bs_close_write (Bitstream *bs)
{
    uint32_t bytes_written;

    if (bs->error)
        return (uint32_t) -1;

    while (bs->bc || ((bs->ptr - bs->buf) & 1))
    {
        putbit_1(bs);
    }
    bytes_written = bs->ptr - bs->buf;
    CLEAR (*bs);
    return bytes_written;
}
