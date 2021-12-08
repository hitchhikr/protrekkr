/*
   This file is part of the Buzé base Buzz-library. 
   
   Please refer to LICENSE.TXT for details regarding usage.
*/

/*===========================================================================

      riff.h  -  Don Cross, April 1993.

      RIFF file format classes.
      See Chapter 8 of "Multimedia Programmer's Reference" in
      the Microsoft Windows SDK.

      See also:
          ..\source\riff.cpp
          ddc.h

===========================================================================*/

#ifndef _RIFF_H_
#define _RIFF_H_

#include "ddc.h"

#include "../../../../release/distrib/replay/lib/include/endianness.h"

#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_PCM 1
#define WAVE_FORMAT_EXTENSIBLE 0xfffe

typedef unsigned int UINT32;
typedef int INT32;

#if defined(__WIN32__) && !defined(__GCC__)
typedef unsigned __int64 Uint64;
#else
#if defined(__LINUX__) && !defined(__FREEBSD__)
//typedef unsigned long long uint64_t;
typedef uint64_t Uint64;
#else
typedef unsigned long long Uint64;
#endif
#endif

#if !defined(__GCC__)
#pragma pack(push)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

unsigned long FourCC(const char *ChunkName);

enum RiffFileMode
{
    RFM_UNKNOWN,      // undefined type (can use to mean "N/A" or "not open")
    RFM_WRITE,        // open for write
    RFM_READ          // open for read
};


struct RiffChunkHeader
{
    UINT32 ckID;       // Four-character chunk ID
    UINT32 ckSize;     // Length of data in chunk
};

class RiffFile
{
   RiffChunkHeader riff_header;      // header for whole file

    protected:

        RiffFileMode fmode;     // current file I/O mode
        FILE *file;             // I/O stream to use
        DDCRET Seek(long offset);

    public:

        RiffFile();
        ~RiffFile();

        RiffFileMode CurrentFileMode() const
        {
            return fmode;
        }

        DDCRET Open(const char *Filename, RiffFileMode NewMode);
        DDCRET Write(const void *Data, unsigned NumBytes);
        DDCRET Read(void *Data, unsigned NumBytes);
        DDCRET Expect(const void *Data, unsigned NumBytes);
        DDCRET Skip(unsigned NumBytes);
        DDCRET Close();

        int SeekChunk(const char *ChunkName);

        long CurrentFilePosition() const;

        DDCRET Backpatch(long FileOffset,
                         const void *Data,
                         unsigned NumBytes
                        );
};

typedef struct
{
    UINT16   wFormatTag;        /* format type */
    UINT16   nChannels;         /* number of channels (i.e. mono, stereo...) */
    UINT32   nSamplesPerSec;    /* sample rate */
    UINT32   nAvgBytesPerSec;   /* for buffer estimation */
    UINT16   nBlockAlign;       /* block size of data */
    UINT16   wBitsPerSample;    /* Number of bits per sample of mono data */
    UINT16   cbSize;            /* The count in bytes of the size of
                                  extra information (after cbSize) */
} _WAVEFORMATEX;

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;
#endif /* GUID_DEFINED */

typedef struct
{
    union
    {
        UINT16 wValidBitsPerSample; /* bits of precision */
        UINT16 wSamplesPerBlock;   /* valid if wBitsPerSample==0 */
        UINT16 wReserved;         /* If neither applies, set to zero. */
    } Samples;
    UINT32 dwChannelMask; /* which channels are present in stream */
    GUID SubFormat;
} _WAVEFORMATEXTENSIBLE, *_PWAVEFORMATEXTENSIBLE;

struct WaveFormat_ChunkData
{
    UINT16 wFormatTag;       // Format category (PCM=1)
    UINT16 nChannels;        // Number of channels (mono=1, stereo=2)
    UINT32 nSamplesPerSec;   // Sampling rate [Hz]
    UINT32 nAvgBytesPerSec;
    UINT16 nBlockAlign;
    UINT16 nBitsPerSample;

    void Config(UINT32   NewSamplingRate = 44100,
                UINT16   NewBitsPerSample = 16,
                UINT16   NewNumChannels = 2
               )
    {
        nSamplesPerSec  = NewSamplingRate;
        nChannels       = NewNumChannels;
        nBitsPerSample  = NewBitsPerSample;
        nAvgBytesPerSec = (nChannels * nSamplesPerSec * nBitsPerSample) / 8;
        nBlockAlign     = (nChannels * nBitsPerSample) / 8;
    }

    WaveFormat_ChunkData()
    {
        wFormatTag = WAVE_FORMAT_PCM;
        Config();
    }
};

struct WaveFormat_Chunk
{
    RiffChunkHeader header;
    WaveFormat_ChunkData data;

    WaveFormat_Chunk()
    {
        header.ckID = FourCC("fmt ");
        header.ckSize = Swap_32(sizeof(WaveFormat_ChunkData));
    }

    dBOOLEAN VerifyValidity()
    {
        return header.ckID == FourCC("fmt ") &&
               (data.nChannels == 1 || data.nChannels == 2) &&
                data.nAvgBytesPerSec == (data.nChannels *
                                         data.nSamplesPerSec *
                                         data.nBitsPerSample) / 8 &&
                data.nBlockAlign == (data.nChannels * data.nBitsPerSample) / 8 ||
                data.nBitsPerSample == 12;
    }
};

struct WaveSmpl_ChunkData
{
   UINT32 Manufacturer;        // 0
   UINT32 Product;             // 4
   UINT32 Sample_Period;       // 8
   UINT32 MIDI_Unity_Note;     // 12
   UINT32 MIDI_Pitch_Fraction; // 16
   UINT32 SMPTE_Format;        // 20
   UINT32 SMPTE_Offset;        // 24
   UINT32 Num_Sample_Loops;    // 28
   UINT32 Sampler_Data;        // 32 0
   UINT32 Cue_Point_ID;        // 36 4
   UINT32 Type;                // 40 8
   UINT32 Start;               // 44 12
   UINT32 End;                 // 48 16
   UINT32 Fraction;            // 52 20
   UINT32 Play_Count;          // 56 24

   void Config(UINT32 New_Num_Sample_Loops = 0,
               UINT32 New_Start = 0,
               UINT32 New_End = 0
              )
    {
        Num_Sample_Loops = New_Num_Sample_Loops;
        Start = New_Start;
        End = New_End;
    }

    WaveSmpl_ChunkData()
    {
        Num_Sample_Loops = 0;
        Start = 0;
        End = 0;
        Config();
    }
};

struct WaveSmpl_Chunk
{
    RiffChunkHeader header;
    WaveSmpl_ChunkData data;

    WaveSmpl_Chunk()
    {
        header.ckID = FourCC("smpl");
        header.ckSize = Swap_32(sizeof(WaveSmpl_ChunkData));
    }
};

#define MAX_WAVE_CHANNELS 2

struct WaveFileSample
{
    INT32 chan[MAX_WAVE_CHANNELS];
};

class WaveFile: private RiffFile
{
        WaveFormat_Chunk wave_format;
        WaveSmpl_Chunk wave_Smpl;
        RiffChunkHeader pcm_data;
        long pcm_data_offset;  // offset of 'pcm_data' in output file
        UINT32 num_samples;

    public:

        WaveFile();

        DDCRET OpenForWrite(const char  *Filename,
                            UINT32 SamplingRate = 44100,
                            UINT16 BitsPerSample = 16,
                            UINT16 NumChannels = 2
                           );

        DDCRET OpenForRead(const char *Filename);

        DDCRET SeekToSample(unsigned long SampleIndex);

        DDCRET WriteData(const void *data, UINT32 numData);

        DDCRET WriteData(const INT32 *data, UINT32 numData);

        // The following work only with 16-bit audio
        DDCRET WriteData(const INT16 *data, UINT32 numData);

        // The following work only with 8-bit audio
        DDCRET WriteData(const UINT8 *data, UINT32 numData);

        DDCRET WriteMonoSample(INT16 ChannelData);
        DDCRET WriteStereoSample(INT16 LeftChannelData, INT16 RightChannelData);
        DDCRET WriteStereoFloatSample(float LeftSample, float RightSample);

        DDCRET ReadMonoSample(INT16 *ChannelData);
        DDCRET ReadStereoSample(INT16 *LeftSampleData, INT16 *RightSampleData);

        DDCRET Close();

        UINT32   SamplingRate() const;    // [Hz]
        UINT16   BitsPerSample() const;
        UINT16   NumChannels() const;
        UINT32   NumSamples() const;
        UINT16   SampleFormat() const; // one of WAVE_FORMAT_IEEE_FLOAT, WAVE_FORMAT_PCM

        UINT32   LoopType() const;
        UINT32   LoopStart() const;
        UINT32   LoopEnd() const;

       // Open for write using another wave file's parameters...

        DDCRET OpenForWrite(const char *Filename,
                             WaveFile &OtherWave
                           )
        {
            return OpenForWrite(Filename,
                                OtherWave.SamplingRate(),
                                OtherWave.BitsPerSample(),
                                OtherWave.NumChannels()
                               );
        }

        long CurrentFilePosition() const
        {
            return RiffFile::CurrentFilePosition();
        }

    private:

        int Flip12;

        void IntToFloat(int *Dest, int Source);
        void Int64ToDouble(Uint64 *Dest, Uint64 Source);
        int FloatToInt(int *Source);
};

#pragma pack(pop)

#endif /* __DDC_RIFF_H */
