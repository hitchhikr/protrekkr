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
#include "include/aiff.h"

AIFFFile::AIFFFile()
{
    file = NULL;
    Base_Note = 0;
    SustainLoop.PlayMode = NoLooping;
    Use_Floats = 0;
    Loop_Start = 0;
    Loop_End = 0;
}

AIFFFile::~AIFFFile()
{
    Close();
}

unsigned long AIFFFile::FourCC(const char *ChunkName)
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
// Look for a chunk inside the file
// Return it's length (with file pointing to it's data) or 0
int AIFFFile::SeekChunk(const char *ChunkName)
{
    int Chunk;
    int Chunk_To_Find_Lo;
    int Chunk_To_Find = FourCC(ChunkName);
    int i;
    int size;

    i = 0;
    Chunk_To_Find_Lo = tolower(Chunk_To_Find & 0xff);
    Chunk_To_Find_Lo |= tolower((Chunk_To_Find >> 8) & 0xff) << 8;
    Chunk_To_Find_Lo |= tolower((Chunk_To_Find >> 16) & 0xff) << 16;
    Chunk_To_Find_Lo |= tolower((Chunk_To_Find >> 24) & 0xff) << 24;

    Seek(i);
    while(!feof(file))
    {
        Chunk = 0;
        Seek(i);
        Read(&Chunk, 4);
        if(Chunk == Chunk_To_Find ||
           Chunk == Chunk_To_Find_Lo)
        {
            Read(&size, 4);
            return(Mot_Swap_32(size));
        }
        // Skip the data part to speed up the process
        if(Chunk == SoundDataID)
        {
            Read(&size, 4);
            size = Mot_Swap_32(size);
            i += size + 4 + 4 - 1;
        }
        i++;
    }
    return(0);
}

int AIFFFile::Open(const char *Filename)
{
    int chunk_size;
    int Padding;
    int Phony_Byte;
    short Phony_Short;

    file = fopen(Filename, "rb");
    if(file)
    {
        // Those compression schemes are not supported
        chunk_size = SeekChunk("ALAW");
        if(chunk_size) return 0;
        chunk_size = SeekChunk("ULAW");
        if(chunk_size) return 0;
        chunk_size = SeekChunk("G722");
        if(chunk_size) return 0;
        chunk_size = SeekChunk("G726");
        if(chunk_size) return 0;
        chunk_size = SeekChunk("G728");
        if(chunk_size) return 0;
        chunk_size = SeekChunk("GSM ");
        if(chunk_size) return 0;

        chunk_size = SeekChunk("COMM");
        if(chunk_size)
        {
            Read(&CommDat.numChannels, sizeof(short));
            CommDat.numChannels = Mot_Swap_16(CommDat.numChannels);
            Read(&CommDat.numSampleFrames, sizeof(unsigned long));
            CommDat.numSampleFrames = Mot_Swap_32(CommDat.numSampleFrames);
            Read(&CommDat.sampleSize, sizeof(short));
            CommDat.sampleSize = Mot_Swap_16(CommDat.sampleSize);

            chunk_size = SeekChunk("INST");
            // (Not mandatory)
            if(chunk_size)
            {
                Read(&Base_Note, sizeof(char));
                Read(&Phony_Byte, sizeof(char));            // detune
                Read(&Phony_Byte, sizeof(char));            // lowNote
                Read(&Phony_Byte, sizeof(char));            // highNote
                Read(&Phony_Byte, sizeof(char));            // lowVelocity
                Read(&Phony_Byte, sizeof(char));            // highVelocity
                Read(&Phony_Short, sizeof(short));          // gain
                Read(&SustainLoop, sizeof(Loop));           // sustainLoop
                SustainLoop.PlayMode = Mot_Swap_16(SustainLoop.PlayMode);
                SustainLoop.beginLoop = Mot_Swap_16(SustainLoop.beginLoop);
                SustainLoop.endLoop = Mot_Swap_16(SustainLoop.endLoop);
                if(SustainLoop.beginLoop < SustainLoop.endLoop)
                {
                    // Find loop points
                    Loop_Start = Get_Marker(SustainLoop.beginLoop);
                    Loop_End = Get_Marker(SustainLoop.endLoop);
                    // Messed up data
                    if(Loop_Start >= Loop_End)
                    {
                        SustainLoop.PlayMode = NoLooping;
                    }
                }
                else
                {
                    // Doc specifies that begin must be smaller
                    // otherwise there's no loop
                    SustainLoop.PlayMode = NoLooping;
                }
            }

            chunk_size = SeekChunk("FL32");
            if(chunk_size) Use_Floats = 1;
            chunk_size = SeekChunk("FL64");
            if(chunk_size) Use_Floats = 1;

            chunk_size = SeekChunk("SSND");
            if(chunk_size)
            {
                // Dummy reads
                Read(&Padding, sizeof(unsigned long));
                Read(&Block_Size, sizeof(unsigned long));
                Seek(CurrentFilePosition() + Padding);
                // File pos now points on waveform data
                return 1;
            }
        }
    }
    return 0;
}

long AIFFFile::CurrentFilePosition()
{
    return ftell(file);
}

int AIFFFile::Seek(long offset)
{
    fflush(file);

    if(fseek(file, offset, SEEK_SET))
    {
        return(0);
    }
    else
    {
        return(1);
    }
}

int AIFFFile::Read(void *Data, unsigned NumBytes)
{
    return fread(Data, NumBytes, 1, file);
}

void AIFFFile::Close()
{
    if(file) fclose(file);
    file = NULL;
}

int AIFFFile::BitsPerSample()
{
    return CommDat.sampleSize;
}

int AIFFFile::NumChannels()
{
    return CommDat.numChannels;
}

unsigned long AIFFFile::LoopStart()
{
    return Loop_Start;
}

unsigned long AIFFFile::LoopEnd()
{
    return Loop_End;
}

unsigned long AIFFFile::NumSamples()
{
    return CommDat.numSampleFrames;
}

int AIFFFile::BaseNote()
{
    return Base_Note;
}

int AIFFFile::LoopType()
{
    return SustainLoop.PlayMode;
}

int AIFFFile::ReadMonoSample(short *Sample)
{
    int retcode;
    float y;
    double y64;
    unsigned long int_y;
    Uint64 int_y64;

    switch(CommDat.sampleSize)
    {
        case 8:
            unsigned char x;
            retcode = Read(&x, 1);
            *Sample = (short(x) << 8);
            break;

        case 12:
        case 16:
            retcode = Read(Sample, 2);
            *Sample = Mot_Swap_16(*Sample);
            break;

        case 24:
            int_y = 0;
            retcode = Read(&int_y, 3);
            int_y = Mot_Swap_32(int_y);
            *Sample = (short) (int_y / 65536);
            break;

        case 32:
            retcode = Read(&int_y, 4);
            int_y = Mot_Swap_32(int_y);

            if(Use_Floats)
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
            int_y64 = Mot_Swap_64(int_y64);

            Int64ToDouble((Uint64 *) &y64, int_y64);
            *Sample = (short) (y64 * 32767.0);
            break;

        default:
            retcode = 0;
    }

    return retcode;
}

int AIFFFile::ReadStereoSample(short *L, short *R)
{
    int retcode = 0;
    unsigned char x[2];
    short y[2];
    float z[2];
    double z64[2];
    long int_z[2];
    Uint64 int_z64[2];

    switch(CommDat.sampleSize)
    {
        case 8:
            retcode = Read(x, 2);
            *L = (short (x[0]) << 8);
            *R = (short (x[1]) << 8);
            break;

        case 12:
        case 16:
            retcode = Read(y, 4);

            y[0] = Mot_Swap_16(y[0]);
            y[1] = Mot_Swap_16(y[1]);

            *L = short(y[0]);
            *R = short(y[1]);
            break;

        case 24:
            int_z[0] = 0;
            int_z[1] = 0;
            retcode = Read(&int_z[0], 3);
            retcode = Read(&int_z[1], 3);
            int_z[0] = Mot_Swap_32(int_z[0]);
            int_z[1] = Mot_Swap_32(int_z[1]);
            *L = (short) (int_z[0] / 65536);
            *R = (short) (int_z[1] / 65536);
            break;

        case 32:
            retcode = Read(int_z, 8);
            int_z[0] = Mot_Swap_32(int_z[0]);
            int_z[1] = Mot_Swap_32(int_z[1]);

            if(Use_Floats)
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

            int_z64[0] = Mot_Swap_64(int_z64[0]);
            int_z64[1] = Mot_Swap_64(int_z64[1]);

            Int64ToDouble((Uint64 *) &z64[0], int_z64[0]);
            Int64ToDouble((Uint64 *) &z64[1], int_z64[1]);
            *L = (short) (z64[0] * 32767.0);
            *R = (short) (z64[1] * 32767.0);
            break;

        default:
            retcode = 0;
    }

    return retcode;
}

int AIFFFile::Get_Marker(int Marker_Id)
{
    unsigned char string_size;
    int i;

    int chunk_size = SeekChunk("MARK");
    if(chunk_size)
    {
        Read(&Markers.numMarkers, sizeof(unsigned short));
        Markers.numMarkers = Mot_Swap_16(Markers.numMarkers);
        for(i = 0 ; i < Markers.numMarkers; i++)
        {
            Read(&CurMarker.id, sizeof(unsigned short));
            CurMarker.id = Mot_Swap_16(CurMarker.id);
            Read(&CurMarker.position, sizeof(unsigned long));
            CurMarker.position = Mot_Swap_32(CurMarker.position);
            if(CurMarker.id == Marker_Id)
            {
                return(CurMarker.position);
            }
            else
            {
                Read(&string_size, sizeof(unsigned char));
                string_size++;
                Seek(CurrentFilePosition() + string_size);
            }
        }
        // Couldn't find the specified marker so disable everything
        SustainLoop.PlayMode = NoLooping;
        return(-1);
    }
    else
    {
        // Everything is broken so disable looping
        SustainLoop.PlayMode = NoLooping;
        return(-1);
    }
}

void AIFFFile::IntToFloat(int *Dest, int Source)
{
    *Dest = Source;
}

void AIFFFile::Int64ToDouble(Uint64 *Dest, Uint64 Source)
{
    *Dest = Source;
}
