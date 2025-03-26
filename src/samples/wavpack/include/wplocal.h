////////////////////////////////////////////////////////////////////////////
//                           **** WAVPACK ****                            //
//                  Hybrid Lossless Wavefile Compressor                   //
//              Copyright (c) 1998 - 2007 Conifer Software.               //
//                          All Rights Reserved.                          //
//      Distributed under the BSD Software License (see license.txt)      //
////////////////////////////////////////////////////////////////////////////

// wplocal.h

#ifndef WPLOCAL_H
#define WPLOCAL_H

#if defined(WIN32)
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif

#include <sys/types.h>

// This header file contains all the definitions required by WavPack.

#ifdef __BORLANDC__
typedef unsigned long uint32_t;
typedef long int32_t;
#elif defined(_WIN32) && !defined(__MINGW32__)
#include <stdlib.h>
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef __int32 int32_t;
#else
#include <inttypes.h>
#endif

typedef unsigned char   uchar;

#if !defined(__GNUC__) || defined(WIN32)
typedef unsigned short  ushort;
typedef unsigned int    uint;
#endif

#include <stdio.h>

#define FALSE 0
#define TRUE 1

////////////////////////////// WavPack Header /////////////////////////////////

// Note that this is the ONLY structure that is written to (or read from)
// WavPack 4.0 files, and is the preamble to every block in both the .wv
// and .wvc files.

typedef struct {
    uint32_t ckSize;
    uint32_t total_samples;
    uint32_t block_index;
    uint32_t block_samples;
    uint32_t flags;
} WavpackHeader;

// or-values for "flags"

#define BYTES_STORED    3       // 1-4 bytes/sample
#define HYBRID_FLAG     8       // hybrid mode
#define HYBRID_SHAPE    0x40    // noise shape (hybrid mode only)

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

#define CUR_STREAM_VERS     0x405       // stream version we are writing now


//////////////////////////// WavPack Metadata /////////////////////////////////

// This is an internal representation of metadata.

typedef struct {
    uchar temp_data [64];
    int32_t byte_length;
    void *data;
    uchar id;
} WavpackMetadata;

#define ID_UNIQUE               0x3f
#define ID_CONFIG_BLOCK         0x20
#define ID_ODD_SIZE             0x40
#define ID_LARGE                0x80

#define ID_DECORR_TERMS         0x1
#define ID_DECORR_WEIGHTS       0x2
#define ID_DECORR_SAMPLES       0x3
#define ID_ENTROPY_VARS         0x4
#define ID_HYBRID_PROFILE       0x5
#define ID_WV_BITSTREAM         0x6

///////////////////////// WavPack Configuration ///////////////////////////////

// This external structure is used during encode to provide configuration to
// the encoding engine.

typedef struct {
    int bitrate, shaping_weight;
    int bits_per_sample, bytes_per_sample, num_channels;
    int32_t block_samples, sample_rate;
    uint32_t flags;
} WavpackConfig;

#define CONFIG_HYBRID_FLAG      8       // hybrid mode
#define CONFIG_HYBRID_SHAPE     0x40    // noise shape (hybrid mode only)

#define CONFIG_FAST_FLAG        0x200   // fast mode
#define CONFIG_HIGH_FLAG        0x800   // high quality mode
#define CONFIG_VERY_HIGH_FLAG   0x1000  // very high
#define CONFIG_SHAPE_OVERRIDE   0x8000  // shaping mode specified
#define CONFIG_LOSSY_MODE       0x1000000 // obsolete (for information)
#define CONFIG_EXTRA_MODE       0x2000000 // extra processing mode
#define CONFIG_SKIP_WVX         0x4000000 // no wvx stream w/ floats & big ints
#define CONFIG_MD5_CHECKSUM     0x8000000 // compute & store MD5 signature
#define CONFIG_OPTIMIZE_MONO    0x80000000 // optimize for mono streams posing as stereo

//////////////////////////////// WavPack Stream ///////////////////////////////

// This internal structure contains everything required to handle a WavPack
// "stream", which is defined as a stereo or mono stream of audio samples. For
// multichannel audio several of these would be required. Each stream contains
// pointers to hold a complete allocated block of WavPack data, although it's
// possible to decode WavPack blocks without buffering an entire block.

typedef struct bs {
    uchar *buf, *end, *ptr;
    void (*wrap)(struct bs *bs);
    int error, bc;
    uint32_t sr;
} Bitstream;

#define MAX_STREAMS 1
#define MAX_NTERMS 16
#define MAX_TERM 8

struct decorr_pass {
    int term, delta, weight_A, weight_B;
    int32_t samples_A [MAX_TERM], samples_B [MAX_TERM];
    int32_t aweight_A, aweight_B;
};

typedef struct {
    WavpackHeader wphdr;

    uchar *blockbuff, *blockend;

    int bits, num_terms, shift, lossy_block;
    uint32_t sample_index, crc, crc_x;
    Bitstream wvbits, wvcbits;

    struct {
        int32_t shaping_acc [2], shaping_delta [2], error [2];
    } dc;

    struct decorr_pass decorr_passes [MAX_NTERMS];

    struct {
        uint32_t bitrate_delta [2], bitrate_acc [2];
        uint32_t median [3] [2], slow_level [2], error_limit [2];
        uint32_t pend_data, holding_one, zeros_acc;
        int holding_zero, pend_count;
    } w;
} WavpackStream;

/////////////////////////////// WavPack Context ///////////////////////////////

// This internal structure holds everything required to encode or decode WavPack
// files. It is recommended that direct access to this structure be minimized
// and the provided utilities used instead.

typedef int (*WavpackBlockOutput)(void *wpc, void *data, int32_t bcount);

typedef struct WavpackContext
{
    WavpackConfig config;

    WavpackBlockOutput blockout;
    char *Dest;
    uint32_t crc;

    int wvc_flag;
    int lossy_blocks;
    uint32_t block_samples;
    uint32_t acc_samples;
    uint32_t filelen;
    uint32_t total_samples;

    WavpackStream stream;
    char error_message[80];
} WavpackContext;

//////////////////////// function prototypes and macros //////////////////////

#define CLEAR(destin) memset (&destin, 0, sizeof (destin));

// These macros implement the weight application and update operations
// that are at the heart of the decorrelation loops. Note that when there
// are several alternative versions of the same macro (marked with PERFCOND)
// then the versions are functionally equivalent with respect to WavPack
// decoding and the user should choose the one that provides the best
// performance.

#if 1   // PERFCOND
#define apply_weight_i(weight, sample) ((weight * sample + 512) >> 10)
#else
#define apply_weight_i(weight, sample) ((((weight * sample) >> 8) + 2) >> 2)
#endif

#define apply_weight_f(weight, sample) (((((sample & 0xffffL) * weight) >> 9) + (((sample & ~0xffffL) >> 9) * weight) + 1) >> 1)

#if 1   // PERFCOND
#define apply_weight(weight, sample) (sample != (short) sample ? apply_weight_f (weight, sample) : apply_weight_i (weight, sample))
#else
#define apply_weight(weight, sample) ((int32_t)((weight * (int64_t) sample + 512) >> 10))
#endif

#if 0   // PERFCOND
#define update_weight(weight, delta, source, result) if (source && result) { int32_t s = (int32_t) (source ^ result) >> 31; weight = (delta ^ s) + (weight - s); }
#elif 1
#define update_weight(weight, delta, source, result) if (source && result) weight += (((source ^ result) >> 30) | 1) * delta
#else
#define update_weight(weight, delta, source, result) if (source && result) (source ^ result) < 0 ? (weight -= delta) : (weight += delta)
#endif

#define update_weight_clip(weight, delta, source, result) if (source && result && ((source ^ result) < 0 ? (weight -= delta) < -1024 : (weight += delta) > 1024)) weight = weight < 0 ? -1024 : 1024

// bits.c

void bs_open_write (Bitstream *bs, uchar *buffer_start, uchar *buffer_end);
uint32_t bs_remain_write (Bitstream *bs);
uint32_t bs_close_write (Bitstream *bs);

#define bs_is_open(bs) ((bs)->ptr != NULL)

#define putbit(bit, bs) { if (bit) (bs)->sr |= (1L << (bs)->bc); if (++((bs)->bc) == 8) { *((bs)->ptr) = (unsigned char) (bs)->sr; (bs)->sr = (bs)->bc = 0; if (++((bs)->ptr) == (bs)->end) (bs)->wrap (bs); }}

#define putbit_0(bs) { if (++((bs)->bc) == 8) { *((bs)->ptr) = (unsigned char) (bs)->sr; (bs)->sr = (bs)->bc = 0; if (++((bs)->ptr) == (bs)->end) (bs)->wrap (bs); }}

#define putbit_1(bs) { (bs)->sr |= (1L << (bs)->bc); if (++((bs)->bc) == 8) { *((bs)->ptr) = (unsigned char) (bs)->sr; (bs)->sr = (bs)->bc = 0; if (++((bs)->ptr) == (bs)->end) (bs)->wrap (bs); }}

#define putbits(value, nbits, bs) { (bs)->sr |= (int32_t)(value) << (bs)->bc; if (((bs)->bc += (nbits)) >= 8) do { *((bs)->ptr) = (unsigned char) (bs)->sr; (bs)->sr >>= 8; if (((bs)->bc -= 8) > 24) (bs)->sr |= ((value) >> ((nbits) - (bs)->bc)); if (++((bs)->ptr) == (bs)->end) (bs)->wrap (bs); } while ((bs)->bc >= 8); }

// pack.c

void pack_init (WavpackContext *wpc);
int pack_start_block (WavpackContext *wpc);
uint32_t pack_samples (WavpackContext *wpc, int32_t *buffer, uint32_t sample_count);
int pack_finish_block (WavpackContext *wpc);

// words.c stuff

void init_words (WavpackStream *wps);
void word_set_bitrate (WavpackStream *wps);
void write_entropy_vars (WavpackStream *wps, WavpackMetadata *wpmd);
void write_hybrid_profile (WavpackStream *wps, WavpackMetadata *wpmd);
int read_entropy_vars (WavpackStream *wps, WavpackMetadata *wpmd);
int read_hybrid_profile (WavpackStream *wps, WavpackMetadata *wpmd);
int32_t FASTCALL send_word (WavpackStream *wps, int32_t value, int chan);
int32_t FASTCALL get_word (WavpackStream *wps, int chan, int32_t *correction);
int32_t FASTCALL get_word_lossless (WavpackStream *wps, int chan);
void flush_word (WavpackStream *wps);
int32_t nosend_word (WavpackStream *wps, int32_t value, int chan);
void scan_word (WavpackStream *wps, int32_t *samples, uint32_t num_samples, int dir);

int log2s (int32_t value);
int32_t exp2s (int log);
uint32_t log2buffer (int32_t *samples, uint32_t num_samples, int limit);

signed char store_weight (int weight);
int restore_weight (signed char weight);

#define WORD_EOF (1L << 31)

// wputils.c

int WavpackGetVersion (WavpackContext *wpc);
uint32_t WavpackUnpackSamples (WavpackContext *wpc, int32_t *buffer, uint32_t samples);
uint32_t WavpackGetNumSamples (WavpackContext *wpc);
uint32_t WavpackGetSampleIndex (WavpackContext *wpc);
int WavpackGetNumErrors (WavpackContext *wpc);
int WavpackLossyBlocks (WavpackContext *wpc);
int WavpackSeekSample (WavpackContext *wpc, uint32_t sample);
WavpackContext *WavpackCloseFile (WavpackContext *wpc);
uint32_t WavpackGetSampleRate (WavpackContext *wpc);
int WavpackGetBitsPerSample (WavpackContext *wpc);
int WavpackGetBytesPerSample (WavpackContext *wpc);
int WavpackGetNumChannels (WavpackContext *wpc);
int WavpackGetReducedChannels (WavpackContext *wpc);
int WavpackGetMD5Sum (WavpackContext *wpc, uchar data [16]);
uint32_t WavpackGetWrapperBytes (WavpackContext *wpc);
uchar *WavpackGetWrapperData (WavpackContext *wpc);
void WavpackFreeWrapper (WavpackContext *wpc);
double WavpackGetProgress (WavpackContext *wpc);
uint32_t WavpackGetFileSize (WavpackContext *wpc);
double WavpackGetRatio (WavpackContext *wpc);
double WavpackGetAverageBitrate (WavpackContext *wpc, int count_wvc);
double WavpackGetInstantBitrate (WavpackContext *wpc);
int WavpackGetNumTagItems (WavpackContext *wpc);
int WavpackGetTagItem (WavpackContext *wpc, const char *item, char *value, int size);
int WavpackGetTagItemIndexed (WavpackContext *wpc, int index, char *item, int size);
int WavpackAppendTagItem (WavpackContext *wpc, const char *item, const char *value, int vsize);
int WavpackDeleteTagItem (WavpackContext *wpc, const char *item);
int WavpackWriteTag (WavpackContext *wpc);

int WavpackSetConfiguration (WavpackContext *wpc, WavpackConfig *config, uint32_t total_samples);
int WavpackAddWrapper (WavpackContext *wpc, void *data, uint32_t bcount);
int WavpackStoreMD5Sum (WavpackContext *wpc, uchar data [16]);
int WavpackPackInit (WavpackContext *wpc);
int WavpackPackSamples (WavpackContext *wpc, int32_t *sample_buffer, uint32_t sample_count);
int WavpackFlushSamples (WavpackContext *wpc);
void WavpackUpdateNumSamples (WavpackContext *wpc, void *first_block);
void *WavpackGetWrapperLocation (void *first_block, uint32_t *size);

#endif
