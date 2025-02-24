/* ------------------------------------------------------

     riff.cpp  -  Don Cross, April 1993.

     Contains code for reading/writing RIFF files, including .WAV files!

     See also:
         ..\include\riff.h
         ..\include\ddc.h

     Revision history:

1993 April 12 [Don Cross]
     Started writing this code.

1994 October 6 [Don Cross]
     Added WriteData, ReadData.
     Added const modifiers to some function parameters.

1997 September 14 [Don Cross]
     Fixed a bug in WaveFile::Close.  It was calling Backpatch
     and getting confused when the the file had been opened for read.
     (Backpatch returns an error in that case, which prevented
     WaveFile::Close from calling RiffFile::Close.)

2008 April 19 [Franck Charlet]
     Added a function to seek a chunk.
     Added handling for 32 bits samples.
     Corrected the samples calculation.
     Handling for loop informations.

2009 February 6 [Franck Charlet]
     Handling for big endian platforms.
     Removed some unnecessary functions.

2009 February 13 [Franck Charlet]
     Added handling for 12, 24, 32-bit integer and 64-bit float formats.
     32 bit float samples writing.

------------------------------------------------------ */

// Standard includes
#if defined(__WIN32__)
#include <io.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DDCLIB includes
#include "include/riff.h"

unsigned long FourCC(const char *ChunkName)
{
    long retbuf = 0x20202020;   // four spaces (padding)
    char *p = ((char *) &retbuf);

    // Remember, this is Intel format!
    // The first character goes in the LSB
    for (int i = 0; i < 4 && ChunkName[i]; i++)
    {
        *p++ = ChunkName[i];
    }

    return retbuf;
}

// ------------------------------------------------------
RiffFile::RiffFile()
{
    file = 0;
    fmode = RFM_UNKNOWN;

    riff_header.ckID = FourCC("RIFF");
    riff_header.ckSize = 0;
}

RiffFile::~RiffFile()
{
    if(fmode != RFM_UNKNOWN)
    {
        Close();
    }
}

DDCRET RiffFile::Open(const char *Filename, RiffFileMode NewMode)
{
    DDCRET retcode = DDC_SUCCESS;

    if(fmode != RFM_UNKNOWN)
    {
        retcode = Close();
    }

    if(retcode == DDC_SUCCESS)
    {
        switch(NewMode)
        {
            case RFM_WRITE:
                file = fopen(Filename, "wb");
                if(file)
                {
                    // Write the RIFF header...
                    // We will have to come back later and patch it!
                    // (endianness doesn't matter here)

                    if(fwrite(&riff_header, sizeof(riff_header), 1, file) != 1)
                    {
                        fclose(file);
                        //unlink(Filename);
                        fmode = RFM_UNKNOWN;
                        file = 0;
                    }
                    else
                    {
                        fmode = RFM_WRITE;
                    }
                }
                else
                {
                    fmode = RFM_UNKNOWN;
                    retcode = DDC_FILE_ERROR;
                }
                break;

            case RFM_READ:
                file = fopen(Filename, "rb");
                if(file)
                {
                    // Try to read the RIFF header...
                    if(fread(&riff_header, sizeof(riff_header), 1, file) != 1)
                    {
                        fclose(file);
                        fmode = RFM_UNKNOWN;
                        file = 0;
                    }
                    else
                    {
                        fmode = RFM_READ;
                    }

                    riff_header.ckSize = Swap_32(riff_header.ckSize);
                }
                else
                {
                    fmode = RFM_UNKNOWN;
                    retcode = DDC_FILE_ERROR;
                }

                break;

            default:
                retcode = DDC_INVALID_CALL;
        }
    }

    return retcode;
}

DDCRET RiffFile::Write(const void *Data, unsigned NumBytes)
{
    if(fmode != RFM_WRITE)
    {
        return DDC_INVALID_CALL;
    }

    if(fwrite(Data, NumBytes, 1, file) != 1)
    {
        return DDC_FILE_ERROR;
    }

    riff_header.ckSize += NumBytes;

    return DDC_SUCCESS;
}

DDCRET RiffFile::Close()
{
    DDCRET retcode = DDC_SUCCESS;

    switch(fmode)
    {
        case RFM_WRITE:

            riff_header.ckSize = Swap_32(riff_header.ckSize);

            if(fflush(file) ||
               fseek(file,0,SEEK_SET) ||
               fwrite(&riff_header, sizeof(riff_header), 1, file) != 1 ||
               fclose(file)
              )
            {
                retcode = DDC_FILE_ERROR;
            }
            break;

        case RFM_READ:
            fclose(file);
            break;
    }

    file = 0;
    fmode = RFM_UNKNOWN;

    return retcode;
}

long RiffFile::CurrentFilePosition() const
{
    return ftell(file);
}

DDCRET RiffFile::Seek(long offset)
{
    fflush(file);

    DDCRET rc;

    if(fseek(file, offset, SEEK_SET))
    {
        rc = DDC_FILE_ERROR;
    }
    else
    {
        rc = DDC_SUCCESS;
    }

    return rc;
}

DDCRET RiffFile::Backpatch(long FileOffset,
                           const void *Data,
                           unsigned NumBytes)
{
    if(!file)
    {
        return DDC_INVALID_CALL;
    }

    if(fflush(file) ||
       fseek(file, FileOffset, SEEK_SET))
    {
        return DDC_FILE_ERROR;
    }

    return Write(Data, NumBytes);
}

DDCRET RiffFile::Expect(const void *Data, unsigned NumBytes)
{
    char *p = (char *) Data;

    while(NumBytes--)
    {
        if(fgetc(file) != *p++)
        {
            return DDC_FILE_ERROR;
        }
    }

    return DDC_SUCCESS;
}

DDCRET RiffFile::Read(void *Data, unsigned NumBytes)
{
    DDCRET retcode = DDC_SUCCESS;

    if(fread(Data, NumBytes, 1, file) != 1)
    {
        retcode = DDC_FILE_ERROR;
    }

    return retcode;
}

int RiffFile::SeekChunk(const char *ChunkName)
{
    int Chunk;
    int Chunk_To_Find = FourCC(ChunkName);
    int i;
    int data_size;

    i = 0;

    Seek(i);
    while(!feof(file))
    {
        Chunk = 0;
        Seek(i);
        Read(&Chunk, 4);

        if(Chunk == Chunk_To_Find)
        {

#if defined(__BIG_ENDIAN__)
            if(Chunk_To_Find == 'data')
#else
            if(Chunk_To_Find == 'atad')
#endif
            {
                Seek(i + 4);
                return(0);
            }
            Seek(i);
            return(1);
        }
        // Skip the data to speed up the process

#if defined(__BIG_ENDIAN__)
        if(Chunk == 'data')
#else
        if(Chunk == 'atad')
#endif
        {
            Read(&data_size, 4);


            data_size = Swap_32(data_size);

            i += data_size + 4 + 4 - 1;
        }
        i++;
    }
    return(0);
}

// ------------------------------------------------------
extern "C"
{

WaveFile::WaveFile()
{
    pcm_data.ckID = FourCC("data");
    pcm_data.ckSize = 0;
    num_samples = 0;
}

DDCRET WaveFile::OpenForRead(const char *Filename)
{
    UINT32 data_length;
    _WAVEFORMATEXTENSIBLE Extra_Infos;
    short Pad;

    // Verify filename parameter as best we can...
    if(!Filename)
    {
        return DDC_INVALID_CALL;
    }

    DDCRET retcode = Open(Filename, RFM_READ);

    if(retcode == DDC_SUCCESS)
    {
        retcode = Expect("WAVE", 4);
        if(retcode == DDC_SUCCESS)
        {
            if(!SeekChunk("fmt "))
            {
                retcode = DDC_FILE_ERROR;
                return retcode;
            }
            retcode = Read(&wave_format, sizeof(wave_format));

            wave_format.data.wFormatTag = Swap_16(wave_format.data.wFormatTag);
            wave_format.data.nChannels = Swap_16(wave_format.data.nChannels);
            wave_format.data.nSamplesPerSec = Swap_32(wave_format.data.nSamplesPerSec);
            wave_format.data.nAvgBytesPerSec = Swap_32(wave_format.data.nAvgBytesPerSec);
            wave_format.data.nBlockAlign = Swap_16(wave_format.data.nBlockAlign);
            wave_format.data.nBitsPerSample = Swap_16(wave_format.data.nBitsPerSample);
            if(wave_format.data.wFormatTag == WAVE_FORMAT_EXTENSIBLE)
            {
                retcode = Read(&Pad, sizeof(short));
                retcode = Read(&Extra_Infos, sizeof(_WAVEFORMATEXTENSIBLE));
                wave_format.data.wFormatTag = Swap_32(Extra_Infos.SubFormat.Data1);
                wave_format.data.nBitsPerSample = Swap_16(Extra_Infos.Samples.wValidBitsPerSample);
            }
            if(wave_format.data.nBitsPerSample == 12) wave_format.data.nBitsPerSample = 16;

            Flip12 = 0;

            if(retcode == DDC_SUCCESS &&
               !wave_format.VerifyValidity())
            {
                // This isn't standard PCM, so we don't know what it is!
                retcode = DDC_FILE_ERROR;
                return(retcode);
            }

            if(retcode == DDC_SUCCESS)
            {
                pcm_data_offset = CurrentFilePosition();

                // Figure out number of samples from
                // file size, current file position, and
                // WAVE header.

                retcode = Read(&pcm_data, sizeof(pcm_data));

                pcm_data.ckSize = Swap_32(pcm_data.ckSize);
            }

            if(SeekChunk("smpl"))
            {
                retcode = Read(&wave_Smpl, sizeof(wave_Smpl));

                wave_Smpl.header.ckSize = Swap_32(wave_Smpl.header.ckSize);

                wave_Smpl.data.Manufacturer = Swap_32(wave_Smpl.data.Manufacturer);
                wave_Smpl.data.Product = Swap_32(wave_Smpl.data.Product);
                wave_Smpl.data.Sample_Period = Swap_32(wave_Smpl.data.Sample_Period);
                wave_Smpl.data.MIDI_Unity_Note = Swap_32(wave_Smpl.data.MIDI_Unity_Note);
                wave_Smpl.data.MIDI_Pitch_Fraction = Swap_32(wave_Smpl.data.MIDI_Pitch_Fraction);
                wave_Smpl.data.SMPTE_Format = Swap_32(wave_Smpl.data.SMPTE_Format);
                wave_Smpl.data.SMPTE_Offset = Swap_32(wave_Smpl.data.SMPTE_Offset);
                wave_Smpl.data.Num_Sample_Loops = Swap_32(wave_Smpl.data.Num_Sample_Loops);
                wave_Smpl.data.Sampler_Data = Swap_32(wave_Smpl.data.Sampler_Data);
                wave_Smpl.data.Cue_Point_ID = Swap_32(wave_Smpl.data.Cue_Point_ID);
                wave_Smpl.data.Type = Swap_32(wave_Smpl.data.Type);
                wave_Smpl.data.Start = Swap_32(wave_Smpl.data.Start);
                wave_Smpl.data.End = Swap_32(wave_Smpl.data.End);
                wave_Smpl.data.Fraction = Swap_32(wave_Smpl.data.Fraction);
                wave_Smpl.data.Play_Count = Swap_32(wave_Smpl.data.Play_Count);
            }
            Seek(0);
            SeekChunk("data");

            retcode = Read(&data_length, sizeof(data_length));

            data_length = Swap_32(data_length);

            num_samples = data_length;
            num_samples /= NumChannels();
            num_samples /= (BitsPerSample() / 8);
        }
    }

    return retcode;
}

DDCRET WaveFile::OpenForWrite(const char  *Filename,
                              UINT32       SamplingRate,
                              UINT16       BitsPerSample,
                              UINT16       NumChannels)
{
    // Verify parameters...

    if(!Filename ||
       (BitsPerSample != 8 && BitsPerSample != 16 && BitsPerSample != 32) ||
        NumChannels < 1 || NumChannels > 2)
    {
        return DDC_INVALID_CALL;
    }

    wave_format.data.Config(SamplingRate, BitsPerSample, NumChannels);

    if(BitsPerSample == 32) wave_format.data.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;

    DDCRET retcode = Open(Filename, RFM_WRITE);

    if(retcode == DDC_SUCCESS)
    {
        retcode = Write("WAVE", 4);

        if(retcode == DDC_SUCCESS)
        {
            wave_format.data.wFormatTag = Swap_16(wave_format.data.wFormatTag);
            wave_format.data.nChannels = Swap_16(wave_format.data.nChannels);
            wave_format.data.nSamplesPerSec = Swap_32(wave_format.data.nSamplesPerSec);
            wave_format.data.nAvgBytesPerSec = Swap_32(wave_format.data.nAvgBytesPerSec);
            wave_format.data.nBlockAlign = Swap_16(wave_format.data.nBlockAlign);
            wave_format.data.nBitsPerSample = Swap_16(wave_format.data.nBitsPerSample);

            retcode = Write(&wave_format, sizeof(wave_format));

            wave_format.data.wFormatTag = Swap_16(wave_format.data.wFormatTag);
            wave_format.data.nChannels = Swap_16(wave_format.data.nChannels);
            wave_format.data.nSamplesPerSec = Swap_32(wave_format.data.nSamplesPerSec);
            wave_format.data.nAvgBytesPerSec = Swap_32(wave_format.data.nAvgBytesPerSec);
            wave_format.data.nBlockAlign = Swap_16(wave_format.data.nBlockAlign);
            wave_format.data.nBitsPerSample = Swap_16(wave_format.data.nBitsPerSample);

            if(retcode == DDC_SUCCESS)
            {
                pcm_data_offset = CurrentFilePosition();
                retcode = Write(&pcm_data, sizeof(pcm_data));
            }
        }
    }

    return retcode;
}

DDCRET WaveFile::Close()
{
    DDCRET rc = DDC_SUCCESS;

    if(fmode == RFM_WRITE)
    {
        pcm_data.ckSize = Swap_32(pcm_data.ckSize);

        rc = Backpatch(pcm_data_offset, &pcm_data, sizeof(pcm_data));
    }

    if(rc == DDC_SUCCESS)
    {
        rc = RiffFile::Close();
    }
    return rc;
}

DDCRET WaveFile::WriteMonoSample(INT16 SampleData)
{
    switch(wave_format.data.nBitsPerSample)
    {
        case 8:
            pcm_data.ckSize++;
            return Write(&SampleData, 1);

        case 16:
            pcm_data.ckSize += 2;

            SampleData = Swap_16(SampleData);
            return Write(&SampleData, 2);
    }
    return DDC_INVALID_CALL;
}

DDCRET WaveFile::WriteStereoSample(INT16 LeftSample,
                                   INT16 RightSample)
{
    DDCRET retcode = DDC_SUCCESS;

    switch(wave_format.data.nBitsPerSample)
    {
        case 8:
            retcode = Write(&LeftSample, 1);
            if(retcode == DDC_SUCCESS)
            {
                pcm_data.ckSize++;

                retcode = Write(&RightSample, 1);
                if(retcode == DDC_SUCCESS)
                {
                    pcm_data.ckSize++;
                }
            }
            break;

        case 16:
            LeftSample = Swap_16(LeftSample);
            RightSample = Swap_16(RightSample);

            retcode = Write(&LeftSample, 2);
            if(retcode == DDC_SUCCESS)
            {
                pcm_data.ckSize += 2;

                retcode = Write(&RightSample, 2);
                if(retcode == DDC_SUCCESS)
                {
                    pcm_data.ckSize += 2;
                }
            }
            break;

        default:
            retcode = DDC_INVALID_CALL;
    }

    return retcode;
}

DDCRET WaveFile::WriteStereoFloatSample(float LeftSample,
                                        float RightSample)
{
    DDCRET retcode = DDC_SUCCESS;
    UINT32 iSampleL;
    UINT32 iSampleR;

    switch(wave_format.data.nBitsPerSample)
    {
        case 32:
            iSampleL = FloatToInt((int *) &LeftSample);
            iSampleL = Swap_32(iSampleL);
            iSampleR = FloatToInt((int *) &RightSample);
            iSampleR = Swap_32(iSampleR);

            retcode = Write(&iSampleL, 4);
            if(retcode == DDC_SUCCESS)
            {
                pcm_data.ckSize += 4;

                retcode = Write(&iSampleR, 4);
                if(retcode == DDC_SUCCESS)
                {
                    pcm_data.ckSize += 4;
                }
            }
            break;

        default:
            retcode = DDC_INVALID_CALL;
    }

    return retcode;
}

DDCRET WaveFile::ReadMonoSample(INT16 *Sample)
{
    DDCRET retcode = DDC_SUCCESS;
    float y;
    UINT32 int_y;
    double y64;
    Uint64 int_y64;

    switch(wave_format.data.nBitsPerSample)
    {
        case 8:
            unsigned char x;
            retcode = Read(&x, 1);
            *Sample = (INT16(x) << 8) - 32768;
            break;

        case 12:
        case 16:
            retcode = Read(Sample, 2);
            *Sample = Swap_16(*Sample);
            break;

        case 24:
            int_y = 0;
            retcode = Read(&int_y, 3);
            int_y = Swap_32(int_y);

            if(int_y & 0x800000) int_y |= 0xff000000;

            *Sample = (short) (int_y / 256);
            break;

        case 32:
            retcode = Read(&int_y, 4);
            int_y = Swap_32(int_y);

            if(wave_format.data.wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
            {
                IntToFloat((int *) &y, int_y);
                *Sample = (short) (y * 32767.0f);
            }
            else
            {
                *Sample = (short) (int_y / 65536);
            }
            break;

        case 64:
            retcode = Read(&int_y64, 8);
            int_y64 = Swap_64(int_y64);

            Int64ToDouble((Uint64 *) &y64, int_y64);
            *Sample = (short) (y64 * 32767.0);
            break;

        default:
            retcode = DDC_INVALID_CALL;
    }

    return retcode;
}

DDCRET WaveFile::ReadStereoSample(INT16 *L, INT16 *R)
{
    DDCRET retcode = DDC_SUCCESS;
    UINT8 x[2];
    INT16 y[2];
    float z[2];
    double z64[2];
    INT32 int_z[2];
    Uint64 int_z64[2];

    switch(wave_format.data.nBitsPerSample)
    {
        case 8:
            retcode = Read(x, 2);
            *L = (INT16 (x[0]) << 8) - 32768;
            *R = (INT16 (x[1]) << 8) - 32768;
            break;

        case 12:
        case 16:
            retcode = Read(y, 4);

            y[0] = Swap_16(y[0]);
            y[1] = Swap_16(y[1]);

            *L = INT16(y[0]);
            *R = INT16(y[1]);
            break;

        case 24:
            int_z[0] = 0;
            int_z[1] = 0;
            retcode = Read(&int_z[0], 3);
            retcode = Read(&int_z[1], 3);

            int_z[0] = Swap_32(int_z[0]);
            int_z[1] = Swap_32(int_z[1]);

            if(int_z[0] & 0x800000) int_z[0] |= 0xff000000;
            if(int_z[1] & 0x800000) int_z[1] |= 0xff000000;

            *L = (short) (int_z[0] / 256);
            *R = (short) (int_z[1] / 256);
            break;

        case 32:
            retcode = Read(int_z, 8);

            int_z[0] = Swap_32(int_z[0]);
            int_z[1] = Swap_32(int_z[1]);

            if(wave_format.data.wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
            {
                IntToFloat((int *) &z[0], int_z[0]);
                IntToFloat((int *) &z[1], int_z[1]);
                *L = (short) (z[0] * 32767.0f);
                *R = (short) (z[1] * 32767.0f);
            }
            else
            {
                *L = (short) (int_z[0] / 65536);
                *R = (short) (int_z[1] / 65536);
            }
            break;

        case 64:
            retcode = Read(int_z64, 16);

            int_z64[0] = Swap_64(int_z64[0]);
            int_z64[1] = Swap_64(int_z64[1]);

            Int64ToDouble((Uint64 *) &z64[0], int_z64[0]);
            Int64ToDouble((Uint64 *) &z64[1], int_z64[1]);
            *L = (short) (z64[0] * 32767.0);
            *R = (short) (z64[1] * 32767.0);
            break;

        default:
            retcode = DDC_INVALID_CALL;
    }

    return retcode;
}

DDCRET WaveFile::SeekToSample(unsigned long SampleIndex)
{
    if(SampleIndex >= NumSamples())
    {
        return DDC_INVALID_CALL;
    }

    unsigned SampleSize = (BitsPerSample() + 7) / 8;

    DDCRET rc = Seek(pcm_data_offset + sizeof(pcm_data) +
                     SampleSize * NumChannels() * SampleIndex);

    return rc;
}

UINT32 WaveFile::SamplingRate()
{
    return wave_format.data.nSamplesPerSec;
}

UINT16 WaveFile::BitsPerSample()
{
    return wave_format.data.nBitsPerSample;
}

UINT16 WaveFile::NumChannels()
{
    return wave_format.data.nChannels;
}

UINT32 WaveFile::NumSamples()
{
    return num_samples;
}

// Simple or none
UINT32 WaveFile::Wave_LoopType()
{
    return wave_Smpl.data.Num_Sample_Loops != 0 ? 1 : 0;
}

UINT32 WaveFile::Wave_LoopStart()
{
    return wave_Smpl.data.Start;
}

UINT32 WaveFile::Wave_LoopEnd()
{
    return wave_Smpl.data.End;
}

// PATCH THAT

DDCRET WaveFile::WriteData_void(const void *data, UINT32 numData)
{
    UINT32 extraBytes = numData;
    return RiffFile::Write(data, extraBytes);
}

DDCRET WaveFile::WriteData_int32(const INT32 *data, UINT32 numData)
{
    UINT32 extraBytes = numData * sizeof(INT32);
    pcm_data.ckSize += extraBytes;
    return RiffFile::Write(data, extraBytes);
}

DDCRET WaveFile::WriteData_int16(const INT16 *data, UINT32 numData)
{
    UINT32 extraBytes = numData * sizeof(INT16);
    pcm_data.ckSize += extraBytes;
    return RiffFile::Write(data, extraBytes);
}

DDCRET WaveFile::WriteData_uint8(const UINT8 *data, UINT32 numData)
{
    pcm_data.ckSize += numData;
    return RiffFile::Write(data, numData);
}

void WaveFile::IntToFloat(int *Dest, int Source)
{
    *Dest = Source;
}

int WaveFile::FloatToInt(int *Source)
{
    return(*Source);
}

void WaveFile::Int64ToDouble(Uint64 *Dest, Uint64 Source)
{
    *Dest = Source;
}

}
