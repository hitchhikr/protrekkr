// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2021 Franck Charlet.
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
#include "../include/variables.h"
#include "include/patterns_blocks.h"
#include "include/editor_pattern.h"

#if !defined(__WINAMP__)

// ------------------------------------------------------
// Variables
extern char Global_Patterns_Font;

unsigned char *Spec_BuffBlock;
unsigned char *BuffBlock[NBR_COPY_BLOCKS];
int Buff_Full[NBR_COPY_BLOCKS];
int block_start_track_nibble[NBR_COPY_BLOCKS];
int block_end_track_nibble[NBR_COPY_BLOCKS];
int b_buff_xsize[NBR_COPY_BLOCKS];
int b_buff_ysize[NBR_COPY_BLOCKS];
int start_buff_nibble[NBR_COPY_BLOCKS];
int block_in_selection[NBR_COPY_BLOCKS];
int block_start_track[NBR_COPY_BLOCKS];
int block_end_track[NBR_COPY_BLOCKS];
int block_start[NBR_COPY_BLOCKS];
int block_end[NBR_COPY_BLOCKS];
int swap_block_start_track[NBR_COPY_BLOCKS];
int swap_block_end_track[NBR_COPY_BLOCKS];
int swap_block_start[NBR_COPY_BLOCKS];
int swap_block_end[NBR_COPY_BLOCKS];

char Buff_MultiNotes[NBR_COPY_BLOCKS][MAX_TRACKS];
char Buff_Effects[NBR_COPY_BLOCKS][MAX_TRACKS];
int Curr_Buff_Block;

COLUMN_TYPE table_compatibilities[] =
{
    EFFECTLO, EFFECT2LO, EFFECT3LO, EFFECT4LO,
    EFFECT2LO, EFFECTLO, EFFECT3LO, EFFECT4LO,
    EFFECT3LO, EFFECT2LO, EFFECTLO, EFFECT4LO,
    EFFECT4LO, EFFECT2LO, EFFECT3LO, EFFECTLO,

    EFFECTDATLO, EFFECT2DATLO, EFFECT3DATLO, EFFECT4DATLO,
    EFFECT2DATLO, EFFECTDATLO, EFFECT3DATLO, EFFECT4DATLO,
    EFFECT3DATLO, EFFECT2DATLO, EFFECTDATLO, EFFECT4DATLO,
    EFFECT4DATLO, EFFECT2DATLO, EFFECT3DATLO, EFFECTDATLO,

    EFFECTHI, EFFECT2HI, EFFECT3HI, EFFECT4HI,
    EFFECT2HI, EFFECTHI, EFFECT3HI, EFFECT4HI,
    EFFECT3HI, EFFECT2HI, EFFECTHI, EFFECT4HI,
    EFFECT4HI, EFFECT2HI, EFFECT3HI, EFFECTHI,

    EFFECTDATHI, EFFECT2DATHI, EFFECT3DATHI, EFFECT4DATHI,
    EFFECT2DATHI, EFFECTDATHI, EFFECT3DATHI, EFFECT4DATHI,
    EFFECT3DATHI, EFFECT2DATHI, EFFECTDATHI, EFFECT4DATHI,
    EFFECT4DATHI, EFFECT2DATHI, EFFECT3DATHI, EFFECTDATHI
};
extern float Default_Pan[MAX_TRACKS];

// ------------------------------------------------------
// Clear the data of a copy buffer
void Init_Current_Buff(int i)
{
    int j;

    Buff_Full[i] = FALSE;
    swap_block_start[i] = -1;
    swap_block_end[i] = -1;
    swap_block_start_track[i] = -1;
    swap_block_end_track[i] = -1;
    block_start_track_nibble[i] = -1;
    block_end_track_nibble[i] = -1;
    b_buff_xsize[i] = 0;
    b_buff_ysize[i] = 0;
    start_buff_nibble[i] = 0;
    block_start_track[i] = -1;
    block_end_track[i] = -1;
    block_in_selection[i] = FALSE;
    block_start[i] = 0;
    block_end[i] = 0;
    for(j = 0; j < MAX_TRACKS; j++)
    {
        Buff_MultiNotes[i][j] = 0;
        Buff_Effects[i][j] = 0;
    }
}

// ------------------------------------------------------
// Copy the data of a buffer into another block
void Copy_Buff(int dst, int src)
{
    Buff_Full[dst] = Buff_Full[src];
    swap_block_start[dst] = swap_block_start[src];
    swap_block_end[dst] = swap_block_end[src];
    swap_block_start_track[dst] = swap_block_start_track[src];
    swap_block_end_track[dst] = swap_block_end_track[src];
    block_start_track_nibble[dst] = block_start_track_nibble[src];
    block_end_track_nibble[dst] = block_end_track_nibble[src];
    b_buff_xsize[dst] = b_buff_xsize[src];
    b_buff_ysize[dst] = b_buff_ysize[src];
    start_buff_nibble[dst] = start_buff_nibble[src];
    block_start_track[dst] = block_start_track[src];
    block_end_track[dst] = block_end_track[src];
    block_in_selection[dst] = block_in_selection[src];
    block_start[dst] = block_start[src];
    block_end[dst] = block_end[src];
}

// ------------------------------------------------------
// Init the blocks datas and buffers
int Init_Block_Work(void)
{
    int i;

    for(i = 0; i < NBR_COPY_BLOCKS; i++)
    {
        BuffBlock[i] = (unsigned char *) malloc(PATTERN_LEN);
        if(!BuffBlock[i]) return(FALSE);
        Init_Current_Buff(i);
        Clear_Buff(i);
    }

    Curr_Buff_Block = 0;
    return(TRUE);
}

// ------------------------------------------------------
// Clear the copy block
void Clear_Buff(int Idx)
{
    int i;
    int ipcut;

    for(ipcut = 0; ipcut < PATTERN_LEN; ipcut += PATTERN_BYTES)
    {
        for(i = 0; i < MAX_POLYPHONY; i++)
        {
            *(BuffBlock[Idx] + ipcut + PATTERN_NOTE1 + (i * 2)) = 121;
            *(BuffBlock[Idx] + ipcut + PATTERN_INSTR1 + (i * 2)) = 255;
        }        
        *(BuffBlock[Idx] + ipcut + PATTERN_VOLUME) = 255;
        *(BuffBlock[Idx] + ipcut + PATTERN_PANNING) = 255;
        *(BuffBlock[Idx] + ipcut + PATTERN_FX) = 0;
        *(BuffBlock[Idx] + ipcut + PATTERN_FXDATA) = 0;
        *(BuffBlock[Idx] + ipcut + PATTERN_FX2) = 0;
        *(BuffBlock[Idx] + ipcut + PATTERN_FXDATA2) = 0;
        *(BuffBlock[Idx] + ipcut + PATTERN_FX3) = 0;
        *(BuffBlock[Idx] + ipcut + PATTERN_FXDATA3) = 0;
        *(BuffBlock[Idx] + ipcut + PATTERN_FX4) = 0;
        *(BuffBlock[Idx] + ipcut + PATTERN_FXDATA4) = 0;
    }
}

// ------------------------------------------------------
// Start the block marking stuff
void Mark_Block_Start(int start_nibble, int start_track, int start_line)
{
    swap_block_start_track[Curr_Buff_Block] = (start_nibble +
                                               Get_Track_Nibble_Start(Channels_MultiNotes, Channels_Effects, start_track))
                                               + start_track;
    swap_block_end_track[Curr_Buff_Block] = swap_block_start_track[Curr_Buff_Block];
    swap_block_start[Curr_Buff_Block] = start_line;
    swap_block_end[Curr_Buff_Block] = swap_block_start[Curr_Buff_Block];

    if(swap_block_end_track[Curr_Buff_Block] < swap_block_start_track[Curr_Buff_Block])
    {
        swap_block_end_track[Curr_Buff_Block] = swap_block_start_track[Curr_Buff_Block];
    }
    if(swap_block_end[Curr_Buff_Block] < swap_block_start[Curr_Buff_Block])
    {
        swap_block_end[Curr_Buff_Block] = swap_block_start[Curr_Buff_Block];
    }

    if(swap_block_end_track[Curr_Buff_Block] < 0) swap_block_end_track[Curr_Buff_Block] = 0;
    block_start_track[Curr_Buff_Block] = swap_block_start_track[Curr_Buff_Block];
    block_end_track[Curr_Buff_Block] = swap_block_end_track[Curr_Buff_Block];
    block_start[Curr_Buff_Block] = swap_block_start[Curr_Buff_Block];
    block_end[Curr_Buff_Block] = swap_block_end[Curr_Buff_Block];
    block_in_selection[Curr_Buff_Block] = TRUE;
    Actupated(0);
}

// ------------------------------------------------------
// Validate the block marking stuff
void Mark_Block_End(int end_nibble, int start_track, int start_line, int Modif)
{
    int swap_value;

    end_nibble += Get_Track_Nibble_Start(Channels_MultiNotes, Channels_Effects, start_track);

    if(Modif & BLOCK_MARK_TRACKS)
    {
        swap_value = end_nibble + start_track;
        if(swap_block_start_track[Curr_Buff_Block] >= swap_value)
        {
            block_start_track[Curr_Buff_Block] = swap_value;
            block_end_track[Curr_Buff_Block] = swap_block_start_track[Curr_Buff_Block];
        }
        else
        {
            block_end_track[Curr_Buff_Block] = swap_value;
            block_start_track[Curr_Buff_Block] = swap_block_start_track[Curr_Buff_Block];
        }
        if(block_end_track[Curr_Buff_Block] < 0) block_end_track[Curr_Buff_Block] = 0;
        if(block_start_track[Curr_Buff_Block] < 0) block_start_track[Curr_Buff_Block] = 0;
    }
    if(Modif & BLOCK_MARK_ROWS)
    {
        swap_value = start_line;

        if(swap_block_start[Curr_Buff_Block] >= swap_value)
        {
            block_start[Curr_Buff_Block] = swap_value;
            block_end[Curr_Buff_Block] = swap_block_start[Curr_Buff_Block];
        }
        else
        {
            block_end[Curr_Buff_Block] = swap_value;
            block_start[Curr_Buff_Block] = swap_block_start[Curr_Buff_Block];
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Blocks routines support stuff
int Get_Pattern_Column(int Position, int xbc, int ybc)
{
   return(*(RawPatterns + (pSequence[Position] * PATTERN_LEN) +
           (ybc * PATTERN_ROW_LEN) + (Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, xbc) * PATTERN_BYTES) +
                                      Get_Byte_From_Column(Channels_MultiNotes, Channels_Effects, xbc)));
}

void Set_Pattern_Column(int Position, int xbc, int ybc, int Data)
{
    *(RawPatterns + (pSequence[Position] * PATTERN_LEN) +
     (ybc * PATTERN_ROW_LEN) + (Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, xbc) * PATTERN_BYTES) +
                                Get_Byte_From_Column(Channels_MultiNotes, Channels_Effects, xbc)) = Data;
}

void Set_Buff_Column(int Position, int xbc, int ybc, int Data)
{
    Buff_Full[Curr_Buff_Block] = TRUE;
    *(BuffBlock[Curr_Buff_Block] + (ybc * PATTERN_ROW_LEN) +
     (Get_Track_From_Nibble(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], xbc) * PATTERN_BYTES) +
      Get_Byte_From_Column(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], xbc)) = Data;
}

int Get_Buff_Column(int Position, int xbc, int ybc)
{
    return(*(BuffBlock[Curr_Buff_Block] + (ybc * PATTERN_ROW_LEN) +
           (Get_Track_From_Nibble(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], xbc) * PATTERN_BYTES) +
            Get_Byte_From_Column(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], xbc)));
}

// ------------------------------------------------------
// Read a byte from the given pattern
int Read_Pattern_Column(int Position, int xbc, int ybc)
{
    COLUMN_TYPE type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
    switch(type)
    {
        case NOTE:
            return(Get_Pattern_Column(Position, xbc, ybc));
        case INSTRHI:
        case VOLUMEHI:
        case PANNINGHI:
        case EFFECTHI:
        case EFFECTDATHI:
        case EFFECT2HI:
        case EFFECT2DATHI:
        case EFFECT3HI:
        case EFFECT3DATHI:
        case EFFECT4HI:
        case EFFECT4DATHI:
            return(Get_Pattern_Column(Position, xbc, ybc) & 0xf0);
        case INSTRLO:
        case VOLUMELO:
        case PANNINGLO:
        case EFFECTLO:
        case EFFECTDATLO:
        case EFFECT2LO:
        case EFFECT2DATLO:
        case EFFECT3LO:
        case EFFECT3DATLO:
        case EFFECT4LO:
        case EFFECT4DATLO:
            return(Get_Pattern_Column(Position, xbc, ybc) & 0xf);
        default:
            // Something went awfully wrong if we're reaching this point
            return(0);
    }
}

// ------------------------------------------------------
// Read a note from the given pattern
int Read_Pattern_Note(int Position)
{
    int start_x = Get_Track_Nibble_Start(Channels_MultiNotes, Channels_Effects, Track_Under_Caret) + Column_Under_Caret + Track_Under_Caret;

    COLUMN_TYPE type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, start_x);
    switch(type)
    {
        case NOTE:
            return(Get_Pattern_Column(Position, start_x, Pattern_Line));
        default:
            return(-1);
    }
}

// ------------------------------------------------------
// Write a byte in the given pattern
void Write_Pattern_Column(int Position, int xbc, int ybc, int datas)
{
    int datas_nibble;
    COLUMN_TYPE type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);

    switch(type)
    {
        case NOTE:
            Set_Pattern_Column(Position, xbc, ybc, datas);
            break;
        case INSTRHI:
        case VOLUMEHI:
        case PANNINGHI:
        case EFFECTHI:
        case EFFECTDATHI:
        case EFFECT2HI:
        case EFFECT2DATHI:
        case EFFECT3HI:
        case EFFECT3DATHI:
        case EFFECT4HI:
        case EFFECT4DATHI:
            datas_nibble = Get_Pattern_Column(Position, xbc, ybc);
            datas_nibble &= 0x0f;
            datas_nibble |= datas;
            Set_Pattern_Column(Position, xbc, ybc, datas_nibble);
            break;
        case INSTRLO:
        case VOLUMELO:
        case PANNINGLO:
        case EFFECTLO:
        case EFFECTDATLO:
        case EFFECT2LO:
        case EFFECT2DATLO:
        case EFFECT3LO:
        case EFFECT3DATLO:
        case EFFECT4LO:
        case EFFECT4DATLO:
            datas_nibble = Get_Pattern_Column(Position, xbc, ybc);
            datas_nibble &= 0xf0;
            datas_nibble |= datas;
            Set_Pattern_Column(Position, xbc, ybc, datas_nibble);
            break;
        default:
            break;
    }
}

// ------------------------------------------------------
// Read a byte from the copy buffer
int Read_Buff_Column(int Position, int xbc, int ybc)
{
    COLUMN_TYPE type = Get_Column_Type(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], xbc);
    switch(type)
    {
        case NOTE:
            return(Get_Buff_Column(Position, xbc, ybc));
        case INSTRHI:
        case VOLUMEHI:
        case PANNINGHI:
        case EFFECTHI:
        case EFFECTDATHI:
        case EFFECT2HI:
        case EFFECT2DATHI:
        case EFFECT3HI:
        case EFFECT3DATHI:
        case EFFECT4HI:
        case EFFECT4DATHI:
            return(Get_Buff_Column(Position, xbc, ybc) & 0xf0);
        case INSTRLO:
        case VOLUMELO:
        case PANNINGLO:
        case EFFECTLO:
        case EFFECTDATLO:
        case EFFECT2LO:
        case EFFECT2DATLO:
        case EFFECT3LO:
        case EFFECT3DATLO:
        case EFFECT4LO:
        case EFFECT4DATLO:
            return(Get_Buff_Column(Position, xbc, ybc) & 0xf);
        default:
            // Something went awfully wrong if we're reaching this point
            return(0);
    }
}

// ------------------------------------------------------
// Write a byte in the copy buffer
void Write_Buff_Column(int Position, int xbc, int ybc, int datas)
{
    int datas_nibble;
    COLUMN_TYPE type = Get_Column_Type(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], xbc);

    switch(type)
    {
        case NOTE:
            Set_Buff_Column(Position, xbc, ybc, datas);
            break;
        case INSTRHI:
        case VOLUMEHI:
        case PANNINGHI:
        case EFFECTHI:
        case EFFECTDATHI:
        case EFFECT2HI:
        case EFFECT2DATHI:
        case EFFECT3HI:
        case EFFECT3DATHI:
        case EFFECT4HI:
        case EFFECT4DATHI:
            datas_nibble = Get_Buff_Column(Position, xbc, ybc);
            datas_nibble &= 0x0f;
            datas_nibble |= datas;
            Set_Buff_Column(Position, xbc, ybc, datas_nibble);
            break;
        case INSTRLO:
        case VOLUMELO:
        case PANNINGLO:
        case EFFECTLO:
        case EFFECTDATLO:
        case EFFECT2LO:
        case EFFECT2DATLO:
        case EFFECT3LO:
        case EFFECT3DATLO:
        case EFFECT4LO:
        case EFFECT4DATLO:
            datas_nibble = Get_Buff_Column(Position, xbc, ybc);
            datas_nibble &= 0xf0;
            datas_nibble |= datas;
            Set_Buff_Column(Position, xbc, ybc, datas_nibble);
            break;
        default:
            break;
    }
}

// ------------------------------------------------------
// Delete a selected block
int Delete_Selection(int Position)
{
    COLUMN_TYPE type;
    int data;

    if(block_start_track[Curr_Buff_Block] != -1 &&
       block_end_track[Curr_Buff_Block] != -1)
    {
        // Delete the entire selection
        for(int ybc = block_start[Curr_Buff_Block]; ybc < block_end[Curr_Buff_Block] + 1; ybc++)
        {
            for(int xbc = block_start_track[Curr_Buff_Block]; xbc < block_end_track[Curr_Buff_Block] + 1; xbc++)
            {
                type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
                switch(type)
                {
                    case NOTE:
                        Set_Pattern_Column(Position, xbc, ybc, 121);
                        break;
                    case INSTRHI:
                    case PANNINGHI:
                    case VOLUMEHI:
                        data = Get_Pattern_Column(Position, xbc, ybc);
                        if(data != 0xff)
                        {
                            data &= 0xf;
                            if(!data)
                            {
                                data = 0xff;
                            }
                            Set_Pattern_Column(Position, xbc, ybc, data);
                        }
                        break;
                    case INSTRLO:
                    case VOLUMELO:
                    case PANNINGLO:
                        data = Get_Pattern_Column(Position, xbc, ybc);
                        if(data != 0xff)
                        {
                            data &= 0xf0;
                            if(!data)
                            {
                                data = 0xff;
                            }
                            Set_Pattern_Column(Position, xbc, ybc, data);
                        }
                        break;
                    case EFFECTLO:
                    case EFFECTDATLO:
                    case EFFECTHI:
                    case EFFECTDATHI:
                    case EFFECT2LO:
                    case EFFECT2DATLO:
                    case EFFECT2HI:
                    case EFFECT2DATHI:
                    case EFFECT3LO:
                    case EFFECT3DATLO:
                    case EFFECT3HI:
                    case EFFECT3DATHI:
                    case EFFECT4LO:
                    case EFFECT4DATLO:
                    case EFFECT4HI:
                    case EFFECT4DATHI:
                        Set_Pattern_Column(Position, xbc, ybc, 0);
                        break;
                }
            }
        }
        return(TRUE);
    }
    return(FALSE);
}

// ------------------------------------------------------
// Insert a line into a selected block
void Insert_Selection(int Cur_Track, int Position)
{
    COLUMN_TYPE type;
    int data;

    if(block_start_track[Curr_Buff_Block] != -1 && block_end_track[Curr_Buff_Block] != -1)
    {
        for(int ybc = block_end[Curr_Buff_Block] - 1; ybc > block_start[Curr_Buff_Block] - 1; ybc--)
        {
            for(int xbc = block_start_track[Curr_Buff_Block]; xbc < block_end_track[Curr_Buff_Block] + 1; xbc++)
            {
                type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
                switch(type)
                {
                    case NOTE:
                        Set_Pattern_Column(Position, xbc, ybc + 1, Get_Pattern_Column(Position, xbc, ybc));
                        Set_Pattern_Column(Position, xbc, ybc, 121);
                        break;
                    case INSTRHI:
                    case PANNINGHI:
                    case VOLUMEHI:
                        data = Get_Pattern_Column(Position, xbc, ybc) & 0xf0;
                        data |= Get_Pattern_Column(Position, xbc, ybc + 1) & 0xf;
                        Set_Pattern_Column(Position, xbc, ybc + 1, data);
                        if(xbc < block_end_track[Curr_Buff_Block])
                        {
                            // Both selected
                            Set_Pattern_Column(Position, xbc, ybc, 0xf0 | (Get_Pattern_Column(Position, xbc, ybc) & 0xf));
                        }
                        else
                        {
                            Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf));
                        }
                        break;

                    case INSTRLO:
                    case VOLUMELO:
                    case PANNINGLO:
                        data = Get_Pattern_Column(Position, xbc, ybc) & 0xf;
                        data |= Get_Pattern_Column(Position, xbc, ybc + 1) & 0xf0;
                        Set_Pattern_Column(Position, xbc, ybc + 1, data);
                        if(xbc > block_start_track[Curr_Buff_Block])
                        {
                            // Both selected
                            Set_Pattern_Column(Position, xbc, ybc, 0xf | (Get_Pattern_Column(Position, xbc, ybc) & 0xf0));
                        }
                        else
                        {
                            Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf0));
                        }
                        break;

                    case EFFECTHI:
                    case EFFECTDATHI:
                    case EFFECT2HI:
                    case EFFECT2DATHI:
                    case EFFECT3HI:
                    case EFFECT3DATHI:
                    case EFFECT4HI:
                    case EFFECT4DATHI:
                        data = Get_Pattern_Column(Position, xbc, ybc) & 0xf0;
                        data |= Get_Pattern_Column(Position, xbc, ybc + 1) & 0xf;
                        Set_Pattern_Column(Position, xbc, ybc + 1, data);
                        Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf));
                        break;

                    case EFFECTLO:
                    case EFFECTDATLO:
                    case EFFECT2LO:
                    case EFFECT2DATLO:
                    case EFFECT3LO:
                    case EFFECT3DATLO:
                    case EFFECT4LO:
                    case EFFECT4DATLO:
                        data = Get_Pattern_Column(Position, xbc, ybc) & 0xf;
                        data |= Get_Pattern_Column(Position, xbc, ybc + 1) & 0xf0;
                        Set_Pattern_Column(Position, xbc, ybc + 1, data);
                        Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf0));
                        break;
                }
            }
        }
        Actupated(0);
    }
    else
    {
        Insert_Track_Line(Cur_Track, Position);
    }
}

// ------------------------------------------------------
// Remove a line from a selected block
void Remove_Selection(int Cur_Track, int Position)
{
    COLUMN_TYPE type;
    int data;

    if(block_start_track[Curr_Buff_Block] != -1 && block_end_track[Curr_Buff_Block] != -1)
    {
        if(Pattern_Line > block_start[Curr_Buff_Block])
        {
            Pattern_Line--;
            for(int ybc = block_start[Curr_Buff_Block] + 1; ybc < block_end[Curr_Buff_Block] + 1; ybc++)
            {
                for(int xbc = block_start_track[Curr_Buff_Block]; xbc < block_end_track[Curr_Buff_Block] + 1; xbc++)
                {
                    type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
                    switch(type)
                    {
                        case NOTE:
                            Set_Pattern_Column(Position, xbc, ybc - 1, Get_Pattern_Column(Position, xbc, ybc));
                            Set_Pattern_Column(Position, xbc, ybc, 121);
                            break;
                        case INSTRHI:
                        case PANNINGHI:
                        case VOLUMEHI:
                            data = Get_Pattern_Column(Position, xbc, ybc) & 0xf0;
                            data |= Get_Pattern_Column(Position, xbc, ybc - 1) & 0xf;
                            Set_Pattern_Column(Position, xbc, ybc - 1, data);
                            if(xbc < block_end_track[Curr_Buff_Block])
                            {
                                // Both selected
                                Set_Pattern_Column(Position, xbc, ybc, 0xf0 | (Get_Pattern_Column(Position, xbc, ybc) & 0xf));
                            }
                            else
                            {
                                Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf));
                            }
                            break;

                        case INSTRLO:
                        case VOLUMELO:
                        case PANNINGLO:
                            data = Get_Pattern_Column(Position, xbc, ybc) & 0xf;
                            data |= Get_Pattern_Column(Position, xbc, ybc - 1) & 0xf0;
                            Set_Pattern_Column(Position, xbc, ybc - 1, data);
                            if(xbc > block_start_track[Curr_Buff_Block])
                            {
                                // Both selected
                                Set_Pattern_Column(Position, xbc, ybc, 0xf | (Get_Pattern_Column(Position, xbc, ybc) & 0xf0));
                            }
                            else
                            {
                                Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf0));
                            }
                            break;

                        case EFFECTHI:
                        case EFFECTDATHI:
                        case EFFECT2HI:
                        case EFFECT2DATHI:
                        case EFFECT3HI:
                        case EFFECT3DATHI:
                        case EFFECT4HI:
                        case EFFECT4DATHI:
                            data = Get_Pattern_Column(Position, xbc, ybc) & 0xf0;
                            data |= Get_Pattern_Column(Position, xbc, ybc - 1) & 0xf;
                            Set_Pattern_Column(Position, xbc, ybc - 1, data);
                            Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf));
                            break;

                        case EFFECTLO:
                        case EFFECTDATLO:
                        case EFFECT2LO:
                        case EFFECT2DATLO:
                        case EFFECT3LO:
                        case EFFECT3DATLO:
                        case EFFECT4LO:
                        case EFFECT4DATLO:
                            data = Get_Pattern_Column(Position, xbc, ybc) & 0xf;
                            data |= Get_Pattern_Column(Position, xbc, ybc - 1) & 0xf0;
                            Set_Pattern_Column(Position, xbc, ybc - 1, data);
                            Set_Pattern_Column(Position, xbc, ybc, (Get_Pattern_Column(Position, xbc, ybc) & 0xf0));
                            break;
                    }
                }
            }
            Actupated(0);
        }
    }
    else
    {
        if(Pattern_Line)
        {
            Pattern_Line--;
            Remove_Track_Line(Cur_Track, Position);
        }
    }
}

// ------------------------------------------------------
// Copy a selected block into a secondary buffer
void Copy_Selection_To_Buffer(int Position)
{
    int axbc;
    int aybc;
    int relative_track;

    block_start_track_nibble[Curr_Buff_Block] = block_start_track[Curr_Buff_Block];
    block_end_track_nibble[Curr_Buff_Block] = block_end_track[Curr_Buff_Block];
    aybc = 0;

    Clear_Buff(Curr_Buff_Block);
    memset(Buff_MultiNotes[Curr_Buff_Block], 0, MAX_TRACKS);
    memset(Buff_Effects[Curr_Buff_Block], 0, MAX_TRACKS);
    start_buff_nibble[Curr_Buff_Block] = Get_Track_Relative_Column(Channels_MultiNotes, Channels_Effects, block_start_track_nibble[Curr_Buff_Block]);
    for(int ybc = block_start[Curr_Buff_Block]; ybc < block_end[Curr_Buff_Block] + 1; ybc++)
    {
        // Make sure we start to copy on the first track (but not necessary on the first column)
        axbc = start_buff_nibble[Curr_Buff_Block];
        relative_track = Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, block_start_track_nibble[Curr_Buff_Block]);
        for(int xbc = block_start_track_nibble[Curr_Buff_Block]; xbc < block_end_track_nibble[Curr_Buff_Block] + 1; xbc++)
        {
            Buff_MultiNotes[Curr_Buff_Block][Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, xbc) - relative_track] =
                                             (Get_Max_Nibble_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, xbc) -
                                              EXTRA_NIBBLE_DAT -
                                              (Channels_Effects[Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, xbc)] * 4)) / 3;

            Buff_Effects[Curr_Buff_Block][Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, xbc) - relative_track] =
                                         Get_Max_Nibble_Effects_From_Nibble(Channels_MultiNotes, Channels_Effects, xbc) / 4;

            Write_Buff_Column(Position, axbc, aybc, Read_Pattern_Column(Position, xbc, ybc));
            axbc++;
        }
        aybc++;
    }
}

// ------------------------------------------------------
// Paste a block into a pattern
void Paste_Selection_From_Buffer(int Position, int Go_Across)
{
    int xbc;
    int ybc;
    int axbc;
    int expanded = 0;
    // Dest start
    int start_x = Get_Track_Nibble_Start(Channels_MultiNotes, Channels_Effects, Track_Under_Caret) + Column_Under_Caret + Track_Under_Caret;
    int byte;
    COLUMN_TYPE type_src;
    COLUMN_TYPE type_dst;
    int old_byte;
    int max_columns = Get_Max_Nibble_All_Tracks();
    int start_buff_x;
    int expand = FALSE;
    int max_src;
    int max_dst;
    int save_start_x;
    int start_track;

    // Compensate
    max_src = Get_Max_Nibble_Track_From_Nibble(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], start_buff_nibble[Curr_Buff_Block]);
    max_dst = Get_Max_Nibble_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, start_x);
    axbc = Get_Track_Relative_Column(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], start_buff_nibble[Curr_Buff_Block]);
    xbc = Get_Track_Relative_Column(Channels_MultiNotes, Channels_Effects, start_x);
    if(max_src < max_dst)
    {
        if((xbc + max_src) < max_dst)
        {
            expand = TRUE;
        }
    }
    else
    {
        if(max_src == max_dst)
        {
            if(axbc > xbc)
            {
                expand = TRUE;
            }
        }
        save_start_x = start_x;
        start_track = Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, start_x);
        while(Get_Column_Type(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], axbc) != Get_Column_Type(Channels_MultiNotes, Channels_Effects, start_x))
        {
            start_x++;
        }
        if(start_track != Get_Track_From_Nibble(Channels_MultiNotes, Channels_Effects, start_x))
        {
            start_x = save_start_x;
        }
    }

    ybc = Pattern_Line;
    for(int aybc = 0; aybc < b_buff_ysize[Curr_Buff_Block]; aybc++)
    {
        axbc = Get_Track_Relative_Column(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], start_buff_nibble[Curr_Buff_Block]);
        start_buff_x = axbc;

        if(Go_Across && ybc >= patternLines[pSequence[Position]])
        {
            if(Position < (Song_Length - 1))
            {
                if(pSequence[Position] == pSequence[Position + 1])
                {
                    break;
                }
                else
                {
                    ybc = 0;
                    Position++;
                }
            }
            else
            {
                break;
            }
        }
        xbc = start_x;
        while(xbc < start_x + b_buff_xsize[Curr_Buff_Block] + expanded)
        {
            if(ybc < patternLines[pSequence[Position]])
            {
                if(xbc < max_columns)
                {
Continue_Copy:
                    // Only copy similar data
                    type_src = Get_Column_Type(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], axbc);
                    type_dst = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
                    if(Buff_Full[Curr_Buff_Block])
                    {
                        if(type_src == type_dst || Are_Columns_Compatible(type_src, type_dst))
                        {
                            // We need to check if we're on an odd byte for the instrument/volume or panning
                            // and see if the byte was 0xff if that's the case we need to put 0 in the upper nibble
                            byte = Read_Buff_Column(Position, axbc, aybc);
                            switch(type_dst)
                            {
                                case INSTRHI:
                                case VOLUMEHI:
                                case PANNINGHI:
                                    old_byte = Read_Pattern_Column(Position, xbc, ybc);
                                    old_byte |= Read_Pattern_Column(Position, xbc + 1, ybc);
                                    if(old_byte == 0xff && byte != 0xf0)
                                    {
                                        Write_Pattern_Column(Position, xbc + 1, ybc, 0);
                                    }
                                    break;

                                case INSTRLO:
                                case VOLUMELO:
                                case PANNINGLO:
                                    // Case where the user didn't selected HI bits
                                    old_byte = Read_Pattern_Column(Position, xbc - 1, ybc);
                                    old_byte |= Read_Pattern_Column(Position, xbc, ybc);
                                    if(start_buff_x == axbc)
                                    {
                                        if(old_byte == 0xff) Write_Pattern_Column(Position, xbc - 1, ybc, 0);
                                    }
                            }
                            Write_Pattern_Column(Position, xbc, ybc, byte);
                        }
                        else
                        {
                            if(expand)
                            {
                                do
                                {
                                    xbc++;
                                    expanded++;
                                    if(xbc >= start_x + b_buff_xsize[Curr_Buff_Block] + expanded)
                                    {
                                        goto Stop_Col;
                                    }
                                }
                                while(Get_Column_Type(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], axbc) != Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc));
                            }
                            else
                            {
                                do
                                {
                                    axbc++;
                                    if(axbc >= start_buff_x + b_buff_xsize[Curr_Buff_Block])
                                    {
                                        goto Stop_Col;
                                    }
                                }
                                while(Get_Column_Type(Buff_MultiNotes[Curr_Buff_Block], Buff_Effects[Curr_Buff_Block], axbc) != Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc));
                            }
                            goto Continue_Copy;
                        }
                    }
                }
            }
            axbc++;
            if(axbc >= start_buff_x + b_buff_xsize[Curr_Buff_Block]) break;
            xbc++;
        }
Stop_Col:
        ybc++;
    }
}

// ------------------------------------------------------
// Cut a selected block
void Cut_Selection(int Position)
{
    Copy_Selection_To_Buffer(Position);
    if(is_editing) Delete_Selection(Position);
    Calc_selection();
    Unselect_Selection();
    Actupated(0);
    Draw_Blocks_Buffers_Status();
}

// ------------------------------------------------------
// Copy a selected block
void Copy_Selection(int Position)
{
    Copy_Selection_To_Buffer(Position);
    Calc_selection();
    Actupated(0);
    Draw_Blocks_Buffers_Status();
}

// ------------------------------------------------------
// Paste a block into a pattern
void Paste_Block(int Position, int Go_Across, int Refresh)
{
    Paste_Selection_From_Buffer(Position, Go_Across);
    if(Refresh) Actupated(0);
}

// ------------------------------------------------------
// Select a complete track
SELECTION Select_Track(int Track)
{
    int i;
    SELECTION Cur_Sel;

    // Default selection
    Cur_Sel.y_start = 0;
    Cur_Sel.y_end = patternLines[pSequence[Song_Position]] - 1;
    Cur_Sel.x_start = 0;
    for(i = 0; i < Track; i++)
    {
        Cur_Sel.x_start += Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, i);
    }
    Cur_Sel.x_end = Cur_Sel.x_start + (Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, Track) - 1);
    return(Cur_Sel);
}

// ------------------------------------------------------
// Load a selection structure (and auto select a complete track if nothing was selected)
SELECTION Get_Real_Selection(int Default)
{
    SELECTION Cur_Sel;
    Cur_Sel.y_start = block_start[Curr_Buff_Block];
    Cur_Sel.y_end = block_end[Curr_Buff_Block];
    Cur_Sel.x_start = block_start_track[Curr_Buff_Block];
    Cur_Sel.x_end = block_end_track[Curr_Buff_Block];
    if(Default)
    {
        if(!(block_end_track[Curr_Buff_Block] - block_start_track[Curr_Buff_Block]) || !(block_end[Curr_Buff_Block] - block_start[Curr_Buff_Block]))
        {
            Cur_Sel = Select_Track(Track_Under_Caret);
        }
    }
    return(Cur_Sel);
}

// ------------------------------------------------------
// Interpolate a selected effects column
void Interpolate_Block(int Position)
{
    int startvalue[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int endvalue[10] =   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int ranlen;
    int cran;
    int tran;
    int start_value;
    int end_value;
    int xbc;
    int ybc;
    COLUMN_TYPE type;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);

    for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
    {
        type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
        switch(type)
        {
            case VOLUMEHI:
            case VOLUMELO:
                startvalue[0] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                endvalue[0] |= Read_Pattern_Column(Position, xbc, Sel.y_end);
                break;

            case PANNINGHI:
            case PANNINGLO:
                startvalue[1] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                endvalue[1] |= Read_Pattern_Column(Position, xbc, Sel.y_end);
                break;

            case EFFECTHI:
            case EFFECTLO:
                startvalue[2] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                break;

            case EFFECT2HI:
            case EFFECT2LO:
                startvalue[3] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                break;

            case EFFECT3HI:
            case EFFECT3LO:
                startvalue[4] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                break;

            case EFFECT4HI:
            case EFFECT4LO:
                startvalue[5] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                break;

            case EFFECTDATHI:
            case EFFECTDATLO:
                startvalue[6] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                endvalue[6] |= Read_Pattern_Column(Position, xbc, Sel.y_end);
                break;
            case EFFECT2DATHI:
            case EFFECT2DATLO:
                startvalue[7] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                endvalue[7] |= Read_Pattern_Column(Position, xbc, Sel.y_end);
                break;
            case EFFECT3DATHI:
            case EFFECT3DATLO:
                startvalue[8] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                endvalue[8] |= Read_Pattern_Column(Position, xbc, Sel.y_end);
                break;
            case EFFECT4DATHI:
            case EFFECT4DATLO:
                startvalue[9] |= Read_Pattern_Column(Position, xbc, Sel.y_start);
                endvalue[9] |= Read_Pattern_Column(Position, xbc, Sel.y_end);
                break;
        }
    }

    for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
    {
        type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
        switch(type)
        {
            case VOLUMEHI:
            case VOLUMELO:
                start_value = startvalue[0];
                end_value = endvalue[0];
                break;
            
            case PANNINGHI:
            case PANNINGLO:
                start_value = startvalue[1];
                end_value = endvalue[1];
                break;

            case EFFECTHI:
            case EFFECTLO:
                start_value = startvalue[2];
                end_value = startvalue[2];
                break;

            case EFFECT2HI:
            case EFFECT2LO:
                start_value = startvalue[3];
                end_value = startvalue[3];
                break;

            case EFFECT3HI:
            case EFFECT3LO:
                start_value = startvalue[4];
                end_value = startvalue[4];
                break;

            case EFFECT4HI:
            case EFFECT4LO:
                start_value = startvalue[5];
                end_value = startvalue[5];
                break;

            case EFFECTDATHI:
            case EFFECTDATLO:
                start_value = startvalue[6];
                end_value = endvalue[6];
                break;
            case EFFECT2DATHI:
            case EFFECT2DATLO:
                start_value = startvalue[7];
                end_value = endvalue[7];
                break;
            case EFFECT3DATHI:
            case EFFECT3DATLO:
                start_value = startvalue[8];
                end_value = endvalue[8];
                break;
            case EFFECT4DATHI:
            case EFFECT4DATLO:
                start_value = startvalue[9];
                end_value = endvalue[9];
                break;
        }

        if(start_value != 0xff || end_value != 0xff)
        {
            switch(type)
            {

                case VOLUMEHI:
                case VOLUMELO:
                case PANNINGHI:
                case PANNINGLO:
                    if(start_value == 0xff) start_value = 0;
                    if(end_value == 0xff) end_value = 0;

                    // No break

                case EFFECTHI:
                case EFFECTLO:
                case EFFECT2HI:
                case EFFECT2LO:
                case EFFECT3HI:
                case EFFECT3LO:
                case EFFECT4HI:
                case EFFECT4LO:

                case EFFECTDATHI:
                case EFFECTDATLO:
                case EFFECT2DATHI:
                case EFFECT2DATLO:
                case EFFECT3DATHI:
                case EFFECT3DATLO:
                case EFFECT4DATHI:
                case EFFECT4DATLO:
                    if(start_value != 0xff || end_value != 0xff)
                    {
                        ranlen = Sel.y_end - Sel.y_start;
                        if(ranlen == 0) ranlen = 1;
                        cran = 0;
                        tran = end_value - start_value;
                        for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
                        {
                            if(xbc < max_columns && ybc < MAX_ROWS)
                            {
                                int c_val = (cran * tran) / ranlen;
                                Write_Pattern_Column(Position, xbc, ybc, start_value + c_val);
                                cran++;
                            }
                        }
                    }
                    break;
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Randomize a selected effects column
void Randomize_Block(int Position)
{
    int ybc;
    int xbc;
    int value;
    COLUMN_TYPE type;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);

    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                type = Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc);
                switch(type)
                {
                    case VOLUMEHI:
                    case VOLUMELO:
                        Write_Pattern_Column(Position, xbc, ybc, (rand() & 0x7f));
                        break;

                    case PANNINGHI:
                        value = (rand() & 0x7f) & 0xf0;
                        if(value > 0x80) value = 0x80;
                        if((Read_Pattern_Column(Position, xbc + 1, ybc) & 0xf) > 0 &&
                                                value == 0x80)
                        {
                            value = 0x80;
                        }
                        Write_Pattern_Column(Position, xbc, ybc, value & 0xf0);
                        break;

                    case PANNINGLO:
                        value = rand() & 0x7f;
                        if((Read_Pattern_Column(Position, xbc - 1, ybc) & 0xf0) > 0x80)
                        {
                            value = 0;
                        }
                        Write_Pattern_Column(Position, xbc, ybc, value & 0xf);
                        break;

                    case EFFECTDATHI:
                    case EFFECTDATLO:
                    case EFFECT2DATHI:
                    case EFFECT2DATLO:
                    case EFFECT3DATHI:
                    case EFFECT3DATLO:
                    case EFFECT4DATHI:
                    case EFFECT4DATLO:
                        Write_Pattern_Column(Position, xbc, ybc, (rand() & 0xff));
                        break;
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a block to 1 semitone higher
void Semitone_Up_Block(int Position)
{
    int ybc;
    int xbc;
    int note;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);
    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    note = Read_Pattern_Column(Position, xbc, ybc);
                    if(note < 120)
                    {
                        note++;
                        if(note > 119) note = 119;
                    }
                    Write_Pattern_Column(Position, xbc, ybc, note);
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a block to 1 semitone lower
void Semitone_Down_Block(int Position)
{
    int ybc;
    int xbc;
    int note;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);
    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    note = Read_Pattern_Column(Position, xbc, ybc);
                    if(note < 120)
                    {
                        note--;
                        if(note < 0) note = 0;
                    }
                    Write_Pattern_Column(Position, xbc, ybc, note);
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a block to 1 octave higher
void Octave_Up_Block(int Position)
{
    int ybc;
    int xbc;
    int note;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);
    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    note = Read_Pattern_Column(Position, xbc, ybc);
                    if(note < 120)
                    {
                        note += 12;
                        if(note > 119) continue;
                    }
                    Write_Pattern_Column(Position, xbc, ybc, note);
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a block to 1 octave lower
void Octave_Down_Block(int Position)
{
    int ybc;
    int xbc;
    int note;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);
    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    note = Read_Pattern_Column(Position, xbc, ybc);
                    if(note < 120)
                    {
                        note -= 12;
                        if(note < 0) continue;
                    }
                    Write_Pattern_Column(Position, xbc, ybc, note);
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a block to 1 semitone higher for the current instrument
void Instrument_Semitone_Up_Block(int Position)
{
    Instrument_Semitone_Up_Sel(Position, Get_Real_Selection(TRUE), 1, Current_Instrument);
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a given selection to 1 semitone higher for the current instrument
void Instrument_Semitone_Up_Sel(int Position, SELECTION Sel, int Amount, int Instr)
{
    int ybc;
    int xbc;
    int note;
    int instrument;
    int max_columns = Get_Max_Nibble_All_Tracks();

    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    instrument = Read_Pattern_Column(Position, xbc + 1, ybc);
                    instrument |= Read_Pattern_Column(Position, xbc + 2, ybc);
                    if(instrument == Instr)
                    {
                        note = Read_Pattern_Column(Position, xbc, ybc);
                        if(note < 120)
                        {
                            note += Amount;
                            if(note > 119) note = 119;
                        }
                        Write_Pattern_Column(Position, xbc, ybc, note);
                    }
                }
            }
        }
    }
}

// ------------------------------------------------------
// Transpose a block to 1 semitone lower for the current instrument
void Instrument_Semitone_Down_Block(int Position)
{
    Instrument_Semitone_Down_Sel(Position, Get_Real_Selection(TRUE), 1, Current_Instrument);
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a given selection to 1 semitone lower for the current instrument
void Instrument_Semitone_Down_Sel(int Position, SELECTION Sel, int Amount, int Instr)
{
    int ybc;
    int xbc;
    int note;
    int instrument;
    int max_columns = Get_Max_Nibble_All_Tracks();

    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    instrument = Read_Pattern_Column(Position, xbc + 1, ybc);
                    instrument |= Read_Pattern_Column(Position, xbc + 2, ybc);
                    if(instrument == Instr)
                    {
                        note = Read_Pattern_Column(Position, xbc, ybc);
                        if(note < 120)
                        {
                            note -= Amount;
                            if(note < 0) note = 0;
                        }
                        Write_Pattern_Column(Position, xbc, ybc, note);
                    }
                }
            }
        }
    }
}

// ------------------------------------------------------
// Transpose a block to 1 octave higher for the current instrument
void Instrument_Octave_Up_Block(int Position)
{
    int ybc;
    int xbc;
    int note;
    int instrument;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);
    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    instrument = Read_Pattern_Column(Position, xbc + 1, ybc);
                    instrument |= Read_Pattern_Column(Position, xbc + 2, ybc);
                    if(instrument == Current_Instrument)
                    {
                        note = Read_Pattern_Column(Position, xbc, ybc);
                        if(note < 120)
                        {
                            note += 12;
                            if(note > 119) continue;
                        }
                        Write_Pattern_Column(Position, xbc, ybc, note);
                    }
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Transpose a block to 1 octave lower for the current instrument
void Instrument_Octave_Down_Block(int Position)
{
    int ybc;
    int xbc;
    int note;
    int instrument;
    int max_columns = Get_Max_Nibble_All_Tracks();

    SELECTION Sel = Get_Real_Selection(TRUE);
    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                if(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc) == NOTE)
                {
                    instrument = Read_Pattern_Column(Position, xbc + 1, ybc);
                    instrument |= Read_Pattern_Column(Position, xbc + 2, ybc);
                    if(instrument == Current_Instrument)
                    {
                        note = Read_Pattern_Column(Position, xbc, ybc);
                        if(note < 120)
                        {
                            note -= 12;
                            if(note < 0) continue;
                        }
                        Write_Pattern_Column(Position, xbc, ybc, note);
                    }
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Remap an instrument
void Instrument_Remap_Sel(int Position, SELECTION Sel, int From, int To)
{
    int ybc;
    int xbc;
    int instrument;
    int max_columns = Get_Max_Nibble_All_Tracks();

    for(ybc = Sel.y_start; ybc <= Sel.y_end; ybc++)
    {
        for(xbc = Sel.x_start; xbc <= Sel.x_end; xbc++)
        {
            if(xbc < max_columns && ybc < MAX_ROWS)
            {
                switch(Get_Column_Type(Channels_MultiNotes, Channels_Effects, xbc))
                {
                    case INSTRHI:
                        instrument = Read_Pattern_Column(Position, xbc, ybc);
                        instrument |= Read_Pattern_Column(Position, xbc + 1, ybc);
                        if(instrument == From)
                        {
                            Write_Pattern_Column(Position, xbc, ybc, To);
                            Write_Pattern_Column(Position, xbc + 1, ybc, To);
                        }
                        break;

                    case INSTRLO:
                        instrument = Read_Pattern_Column(Position, xbc - 1, ybc);
                        instrument |= Read_Pattern_Column(Position, xbc, ybc);
                        if(instrument == From)
                        {
                            Write_Pattern_Column(Position, xbc - 1, ybc, To);
                            Write_Pattern_Column(Position, xbc, ybc, To);
                        }
                        break;
                }
            }
        }
    }
    Actupated(0);
}

// ------------------------------------------------------
// Select a complete track
void Select_Track_Block(void)
{
    int nlines;

    if(!Song_Playing)
    {
        Mark_Block_Start(0, Track_Under_Caret, 0);
        nlines = patternLines[pSequence[Song_Position]];
        Mark_Block_End(Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, Track_Under_Caret) - 1,
                       Track_Under_Caret,
                       nlines,
                       BLOCK_MARK_TRACKS | BLOCK_MARK_ROWS);
    }
}

// ------------------------------------------------------
// Select a complete pattern
void Select_Pattern_Block(void)
{
    int nlines;

    if(!Song_Playing)
    {
        Mark_Block_Start(0, 0, 0);
        nlines = patternLines[pSequence[Song_Position]];
        Mark_Block_End(Get_Track_Nibble_Start(Channels_MultiNotes, Channels_Effects, Track_Under_Caret) - 1,
                       Songtracks,
                       nlines,
                       BLOCK_MARK_TRACKS | BLOCK_MARK_ROWS);
    }
}

// ------------------------------------------------------
// Select a note/instrument columns
int Table_Select_Notes[] =
{
    0, 0, 0,
    3, 3, 3,
    6, 6, 6,
    9, 9, 9,
    12, 12, 12,
    15, 15, 15,
    18, 18, 18,
    21, 21, 21,
    24, 24, 24,
    27, 27, 27,
    30, 30, 30,
    33, 33, 33,
    36, 36, 36,
    39, 39, 39,
    42, 42, 42,
    45, 45, 45
};

void Select_Note_Block(void)
{
    int nlines;
    int i;
    int column_to_select;

    if(!Song_Playing)
    {
        for(i = 0; i < Channels_MultiNotes[Track_Under_Caret] * 3; i++)
        {
            if(Column_Under_Caret == i)
            {
                column_to_select = Table_Select_Notes[i];
                Mark_Block_Start(column_to_select, Track_Under_Caret, 0);
                nlines = patternLines[pSequence[Song_Position]];
                Mark_Block_End(column_to_select + 2,
                               Track_Under_Caret,
                               nlines,
                               BLOCK_MARK_TRACKS | BLOCK_MARK_ROWS);
            }
        }
    }
}

// ------------------------------------------------------
// Select all note/instrument columns of a track
void Select_All_Notes_Block(void)
{
    int nlines;

    if(!Song_Playing)
    {
        Mark_Block_Start(0, Track_Under_Caret, 0);
        nlines = patternLines[pSequence[Song_Position]];
        Mark_Block_End(Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, Track_Under_Caret) - 1 -
                       EXTRA_NIBBLE_DAT - (Channels_Effects[Track_Under_Caret] * 4),
                       Track_Under_Caret,
                       nlines,
                       BLOCK_MARK_TRACKS | BLOCK_MARK_ROWS);
    }
}

// ------------------------------------------------------
// Calculate the selected range
void Calc_selection(void)
{
    b_buff_xsize[Curr_Buff_Block] = (block_end_track_nibble[Curr_Buff_Block] - block_start_track_nibble[Curr_Buff_Block]) + 1;
    b_buff_ysize[Curr_Buff_Block] = (block_end[Curr_Buff_Block] - block_start[Curr_Buff_Block]) + 1;
}

// ------------------------------------------------------
// Unselect the range
void Unselect_Selection(void)
{
    // Save the size of the copied block
    block_start[Curr_Buff_Block] = 0;
    block_end[Curr_Buff_Block] = 0;
    block_start_track[Curr_Buff_Block] = -1;
    block_end_track[Curr_Buff_Block] = -1;
    block_in_selection[Curr_Buff_Block] = FALSE;
}

// ------------------------------------------------------
// Select a block via the keyboard
void Select_Block_Keyboard(int Type)
{
    if(!Song_Playing)
    {
        if(Get_LShift())
        {
            if(block_in_selection[Curr_Buff_Block] == FALSE) Mark_Block_Start(Column_Under_Caret, Track_Under_Caret, Pattern_Line);
            Mark_Block_End(Column_Under_Caret, Track_Under_Caret, Pattern_Line, Type);
        }
        else
        {
            Unselect_Selection();
        }
    }
}

// ------------------------------------------------------
// Insert a line in a pattern
void Insert_Pattern_Line(int Position)
{
    int i;

    for(i = 0; i < Songtracks; i++)
    {
        Insert_Track_Line(i, Position);
    }
}

// ------------------------------------------------------
// Insert a line in a track
void Insert_Track_Line(int track, int Position)
{
    int xoffseted;
    int i;

    for(int interval = (MAX_ROWS - 2); interval > Pattern_Line - 1; interval--)
    {
        xoffseted = Get_Pattern_Offset(pSequence[Position], track, interval);

        for(i = 0; i < MAX_POLYPHONY; i++)
        {
            *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_NOTE1 + (i * 2)) = *(RawPatterns + xoffseted + PATTERN_NOTE1 + (i * 2));
            *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_INSTR1 + (i * 2)) = *(RawPatterns + xoffseted + PATTERN_INSTR1 + (i * 2));
        }
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_VOLUME) = *(RawPatterns + xoffseted + PATTERN_VOLUME);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_PANNING) = *(RawPatterns + xoffseted + PATTERN_PANNING);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FX) = *(RawPatterns + xoffseted + PATTERN_FX);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FXDATA) = *(RawPatterns + xoffseted + PATTERN_FXDATA);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FX2) = *(RawPatterns + xoffseted + PATTERN_FX2);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FXDATA2) = *(RawPatterns + xoffseted + PATTERN_FXDATA2);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FX3) = *(RawPatterns + xoffseted + PATTERN_FX3);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FXDATA3) = *(RawPatterns + xoffseted + PATTERN_FXDATA3);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FX4) = *(RawPatterns + xoffseted + PATTERN_FX4);
        *(RawPatterns + xoffseted + PATTERN_ROW_LEN + PATTERN_FXDATA4) = *(RawPatterns + xoffseted + PATTERN_FXDATA4);
    }
    xoffseted = Get_Pattern_Offset(pSequence[Position], track, Pattern_Line);
 
    Clear_Track_Data(xoffseted);
    Actupated(0);
}

// ------------------------------------------------------
// Insert a line in a pattern
void Remove_Pattern_Line(int Position)
{
    int i;

    for(i = 0; i < Songtracks; i++)
    {
        Remove_Track_Line(i, Position);
    }
}

// ------------------------------------------------------
// Remove a line from a track
void Remove_Track_Line(int track, int Position)
{
    int xoffseted;
    int xoffseted2;
    int i;

    for(int interval = Pattern_Line + 1; interval < MAX_ROWS; interval++)
    {
        xoffseted = Get_Pattern_Offset(pSequence[Position], track, interval);
        xoffseted2 = Get_Pattern_Offset(pSequence[Position], track, interval) - PATTERN_ROW_LEN;
        
        for(i = 0; i < MAX_POLYPHONY; i++)
        {
            *(RawPatterns + xoffseted2 + PATTERN_NOTE1 + (i * 2)) = *(RawPatterns + xoffseted + PATTERN_NOTE1 + (i * 2));
            *(RawPatterns + xoffseted2 + PATTERN_INSTR1 + (i * 2)) = *(RawPatterns + xoffseted + PATTERN_INSTR1 + (i * 2));
        }
        *(RawPatterns + xoffseted2 + PATTERN_VOLUME) = *(RawPatterns + xoffseted + PATTERN_VOLUME);
        *(RawPatterns + xoffseted2 + PATTERN_PANNING) = *(RawPatterns + xoffseted + PATTERN_PANNING);
        *(RawPatterns + xoffseted2 + PATTERN_FX) = *(RawPatterns + xoffseted + PATTERN_FX);
        *(RawPatterns + xoffseted2 + PATTERN_FXDATA) = *(RawPatterns + xoffseted + PATTERN_FXDATA);
        *(RawPatterns + xoffseted2 + PATTERN_FX2) = *(RawPatterns + xoffseted + PATTERN_FX2);
        *(RawPatterns + xoffseted2 + PATTERN_FXDATA2) = *(RawPatterns + xoffseted + PATTERN_FXDATA2);
        *(RawPatterns + xoffseted2 + PATTERN_FX3) = *(RawPatterns + xoffseted + PATTERN_FX3);
        *(RawPatterns + xoffseted2 + PATTERN_FXDATA3) = *(RawPatterns + xoffseted + PATTERN_FXDATA3);
        *(RawPatterns + xoffseted2 + PATTERN_FX4) = *(RawPatterns + xoffseted + PATTERN_FX4);
        *(RawPatterns + xoffseted2 + PATTERN_FXDATA4) = *(RawPatterns + xoffseted + PATTERN_FXDATA4);
    }
    xoffseted = Get_Pattern_Offset(pSequence[Position], track, 0) + (PATTERN_LEN - PATTERN_ROW_LEN);

    Clear_Track_Data(xoffseted);

    Actupated(0);
}
#endif // __WINAMP__

// ------------------------------------------------------
// Clear all patterns
void Clear_Patterns_Pool(void)
{
    for(int i = 0; i < PATTERN_POOL_SIZE; i += PATTERN_BYTES)
    {
        Clear_Track_Data(i);
    }

#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
    Reset_Patterns_Zoom();
#endif

}

// ------------------------------------------------------
// Init the data of a note
void Clear_Track_Data(int offset)
{
    int i;

    for(i = 0; i < MAX_POLYPHONY; i++)
    {
        *(RawPatterns + offset + PATTERN_NOTE1 + (i * 2)) = 121;
        *(RawPatterns + offset + PATTERN_INSTR1 + (i * 2)) = 255;
    }
    *(RawPatterns + offset + PATTERN_VOLUME) = 255;
    *(RawPatterns + offset + PATTERN_PANNING) = 255;
    *(RawPatterns + offset + PATTERN_FX) = 0;
    *(RawPatterns + offset + PATTERN_FXDATA) = 0;
    *(RawPatterns + offset + PATTERN_FX2) = 0;
    *(RawPatterns + offset + PATTERN_FXDATA2) = 0;
    *(RawPatterns + offset + PATTERN_FX3) = 0;
    *(RawPatterns + offset + PATTERN_FXDATA3) = 0;
    *(RawPatterns + offset + PATTERN_FX4) = 0;
    *(RawPatterns + offset + PATTERN_FXDATA4) = 0;
}

// ------------------------------------------------------
// Reset the patterns pool
int Alloc_Patterns_Pool(void)
{
    for(int api = 0; api < MAX_ROWS; api++)
    {
        patternLines[api] = DEFAULT_PATTERN_LEN;
    }

    nPatterns = 1;

    if((RawPatterns = (unsigned char *) malloc(PATTERN_POOL_SIZE)) != NULL)
    {
        Clear_Patterns_Pool();
        return TRUE;
    }
    return FALSE;
}

// ------------------------------------------------------
// Return the number of nibbles in a track
int Get_Max_Nibble_Track(char *Buffer_Multinotes, char *Buffer_Effects, int track)
{
    return((Buffer_Multinotes[track] * 3) +
            EXTRA_NIBBLE_DAT + (Buffer_Effects[track] * 4));
}

// ------------------------------------------------------
// Obtain a track from a nibble and return it's number of nibbles
int Get_Max_Nibble_Track_From_Nibble(char *Buffer_MultiNotes, char *Buffer_Effects, int nibble)
{
    return((Buffer_MultiNotes[Get_Track_From_Nibble(Buffer_MultiNotes, Buffer_Effects, nibble)] * 3) +
            EXTRA_NIBBLE_DAT +
            (Buffer_Effects[Get_Track_From_Nibble(Buffer_MultiNotes, Buffer_Effects, nibble)] * 4));
}

// ------------------------------------------------------
// Obtain a track from a nibble and return it's number of effects nibbles
int Get_Max_Nibble_Effects_From_Nibble(char *Buffer_MultiNotes, char *Buffer_Effects, int nibble)
{
    return(Buffer_Effects[Get_Track_From_Nibble(Buffer_MultiNotes, Buffer_Effects, nibble)] * 4);
}

// ------------------------------------------------------
// Return the real nibble from where a track is starting
int Get_Track_Nibble_Start(char *Buffer_Multinotes, char *Buffer_Effects, int track)
{
    int i;
    int column = 0;

    for(i = 0; i < track; i++)
    {
        column += Get_Max_Nibble_Track(Buffer_Multinotes, Buffer_Effects, i) - 1;
    }
    return(column);
}

// ------------------------------------------------------
// Return the index of a track from a nibble/Column
int Get_Track_From_Nibble(char *Buffer_MultiNotes, char *Buffer_Effects, int nibble)
{
    int i;
    int min_nibble = 0;
    int max_nibble = 0;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        max_nibble += Get_Max_Nibble_Track(Buffer_MultiNotes, Buffer_Effects, i);
        if(nibble >= min_nibble && nibble < max_nibble)
        {
            return(i);
        }
        min_nibble = max_nibble;
    }
    return(0);
}

// ------------------------------------------------------
// Return the byte index from a given column
int Get_Byte_From_Column(char *Buffer_MultiNotes, char *Buffer_Effects, int column)
{
    int max_notes = (Get_Max_Nibble_Track_From_Nibble(Buffer_MultiNotes, Buffer_Effects, column) -
                     EXTRA_NIBBLE_DAT - (Buffer_Effects[Get_Track_From_Nibble(Buffer_MultiNotes, Buffer_Effects, column)] * 4)) / 3;
    int byte = 0;
    int i;

    column = Get_Track_Relative_Column(Buffer_MultiNotes, Buffer_Effects, column);

    for(i = 0; i < max_notes; i++)
    {
        if(column == (i * 3)) return(PATTERN_NOTE1 + (i * 2));
        if(column == ((i * 3) + 1)) return(PATTERN_INSTR1 + (i * 2));
        if(column == ((i * 3) + 2)) return(PATTERN_INSTR1 + (i * 2));
    }
    i--;

    // Volume
    if(column == ((i * 3) + 3)) return(PATTERN_VOLUME);
    if(column == ((i * 3) + 4)) return(PATTERN_VOLUME);

    // Panning
    if(column == ((i * 3) + 5)) return(PATTERN_PANNING);
    if(column == ((i * 3) + 6)) return(PATTERN_PANNING);

    // Fx
    if(column == ((i * 3) + 7)) return(PATTERN_FX);
    if(column == ((i * 3) + 8)) return(PATTERN_FX);

    // Fx data
    if(column == ((i * 3) + 9)) return(PATTERN_FXDATA);
    if(column == ((i * 3) + 10)) return(PATTERN_FXDATA);

    // Fx 2
    if(column == ((i * 3) + 11)) return(PATTERN_FX2);
    if(column == ((i * 3) + 12)) return(PATTERN_FX2);

    // Fx 2 data
    if(column == ((i * 3) + 13)) return(PATTERN_FXDATA2);
    if(column == ((i * 3) + 14)) return(PATTERN_FXDATA2);

    // Fx 3
    if(column == ((i * 3) + 15)) return(PATTERN_FX3);
    if(column == ((i * 3) + 16)) return(PATTERN_FX3);

    // Fx 3 data
    if(column == ((i * 3) + 17)) return(PATTERN_FXDATA3);
    if(column == ((i * 3) + 18)) return(PATTERN_FXDATA3);

    // Fx 4
    if(column == ((i * 3) + 19)) return(PATTERN_FX4);
    if(column == ((i * 3) + 20)) return(PATTERN_FX4);

    // Fx 4 data
    if(column == ((i * 3) + 21)) return(PATTERN_FXDATA4);
    if(column == ((i * 3) + 22)) return(PATTERN_FXDATA4);

    return(byte);
}

// ------------------------------------------------------
// Return the byte index from a given column
COLUMN_TYPE Get_Column_Type(char *Buffer_MultiNotes, char *Buffer_Effects, int column)
{
    int track = Get_Track_From_Nibble(Buffer_MultiNotes, Buffer_Effects, column);
    int i;
    int notes = (Get_Max_Nibble_Track(Buffer_MultiNotes, Buffer_Effects, track) -
                 EXTRA_NIBBLE_DAT - 
                 (Buffer_Effects[track] * 4)) / 3;

    column = Get_Track_Relative_Column(Buffer_MultiNotes, Buffer_Effects, column);

    for(i = 0; i < notes; i++)
    {
        if(column == (i * 3)) return(NOTE);
        if(column == ((i * 3) + 1)) return(INSTRHI);
        if(column == ((i * 3) + 2)) return(INSTRLO);
    }
    i--;
    if(column == ((i * 3) + 3)) return(VOLUMEHI);
    if(column == ((i * 3) + 4)) return(VOLUMELO);
    if(column == ((i * 3) + 5)) return(PANNINGHI);
    if(column == ((i * 3) + 6)) return(PANNINGLO);
    if(column == ((i * 3) + 7)) return(EFFECTHI);
    if(column == ((i * 3) + 8)) return(EFFECTLO);
    if(column == ((i * 3) + 9)) return(EFFECTDATHI);
    if(column == ((i * 3) + 10)) return(EFFECTDATLO);
    if(column == ((i * 3) + 11)) return(EFFECT2HI);
    if(column == ((i * 3) + 12)) return(EFFECT2LO);
    if(column == ((i * 3) + 13)) return(EFFECT2DATHI);
    if(column == ((i * 3) + 14)) return(EFFECT2DATLO);
    if(column == ((i * 3) + 15)) return(EFFECT3HI);
    if(column == ((i * 3) + 16)) return(EFFECT3LO);
    if(column == ((i * 3) + 17)) return(EFFECT3DATHI);
    if(column == ((i * 3) + 18)) return(EFFECT3DATLO);
    if(column == ((i * 3) + 19)) return(EFFECT4HI);
    if(column == ((i * 3) + 20)) return(EFFECT4LO);
    if(column == ((i * 3) + 21)) return(EFFECT4DATHI);
    if(column == ((i * 3) + 22)) return(EFFECT4DATLO);
    return(NOTE);
}

// ------------------------------------------------------
// Return the relative index of a global column
int Get_Track_Relative_Column(char *Buffer_MultiNotes, char *Buffer_Effects, int column)
{
    int track = Get_Track_From_Nibble(Buffer_MultiNotes, Buffer_Effects, column);
    int i;
    int min_nibble = 0;

    for(i = 0; i < track; i++)
    {
        min_nibble += Get_Max_Nibble_Track(Buffer_MultiNotes, Buffer_Effects, i);
    }
    column -= min_nibble;
    return(column);
}

// ------------------------------------------------------
// Return the number of nibbles in a track
int Get_Max_Nibble_All_Tracks(void)
{
    int i;
    int max_columns = 0;

    for(i = 0; i < Songtracks; i++)
    {
        max_columns += Get_Max_Nibble_Track(Channels_MultiNotes, Channels_Effects, i);
    }
    return(max_columns);
}

// ------------------------------------------------------
// Return 1 if a column is compatible to another according to a table
// return 0 otherwise
#if !defined(__STAND_ALONE__) && !defined(__WINAMP__)
int Are_Columns_Compatible(int type_src, int type_dst)
{
    int i;

    for(i = 0; i < sizeof(table_compatibilities) / sizeof(COLUMN_TYPE) / MAX_FX; i++)
    {
        if(type_src == table_compatibilities[i * MAX_FX] &&
           (type_dst == table_compatibilities[(i + 1) * MAX_FX] ||
            type_dst == table_compatibilities[(i + 2) * MAX_FX]||
            type_dst == table_compatibilities[(i + 3) * MAX_FX]
           )
          )
        {
            return 1;
        }
    }
    return 0;
}

// ------------------------------------------------------
// Reset a track to it's initial structure
void Reset_Track(int Position, int Track)
{
    int i;
    int offset;
    int pattern;

    pattern = pSequence[Position];

    for(i = 0 ; i < patternLines[pattern]; i++)
    {
        offset = Get_Pattern_Offset(pattern, Track, i);
        Clear_Track_Data(offset);
    }
    Channels_Polyphony[Track] = 1;
    Channels_MultiNotes[Track] = 1;
    Channels_Effects[Track] = 1;

    TPan[Track] = Default_Pan[Track];

    TCut[Track] = 126.0f;
    ICut[Track] = 0.0039062f;
    FType[Track] = 4;

    Compress_Track[Track] = FALSE;
    mas_comp_threshold_Track[Track] = 100.0f;
    mas_comp_ratio_Track[Track] = 0;
    oldspawn[Track] = 0;
    roldspawn[Track] = 0;

    Chan_Midi_Prg[Track] = Track;

    LFO_ON[Track] = 0;
    LFO_RATE[Track] = 0.0001f;
    LFO_AMPL[Track] = 0;

    FRez[Track] = 64;

    DThreshold[Track] = 32767;

    DClamp[Track] = 32767;
    Disclap[Track] = 0;

    DSend[Track] = 0;
    CSend[Track] = 0;

    Track_Volume[Track] = 1.0f;
    Track_Surround[Track] = FALSE;

    FLANGER_ON[Track] = 0;
    FLANGER_AMOUNT[Track] = -0.8f;
    FLANGER_DEPHASE[Track] = 0.0174532f;
    FLANGER_ON[Track] = 0;
    FLANGER_RATE[Track] = 0.0068125f / 57.29578f;
    FLANGER_AMPL[Track] = 0.001f;
    FLANGER_GR[Track] = 0;
    FLANGER_FEEDBACK[Track] = -0.51f;
    FLANGER_DELAY[Track] = 176;
    FLANGER_OFFSET[Track] = 8192;
    FLANGER_OFFSET2[Track] = float(FLANGER_OFFSET[Track] - FLANGER_DELAY[Track]);
    FLANGER_OFFSET1[Track] = float(FLANGER_OFFSET[Track] - FLANGER_DELAY[Track]);

    init_eq(&EqDat[Track]);

    Chan_Mute_State[Track] = FALSE;
    for(i = 0; i < Song_Length; i++)
    {
        Chan_Active_State[i][Track] = TRUE;
    }
    ComputeStereo(Track);
    FixStereo(Track);

    Set_Track_Zoom(Track, (TRACK_TYPE) Global_Patterns_Font);
}

// ------------------------------------------------------
// Copy the data & structure of a track into anoter
void Copy_Track(int Position, int Track_Src, int Track_Dst)
{
    int i;
    int j;
    int offset_src;
    int offset_dst;
    int pattern;

    pattern = pSequence[Position];

    for(i = 0 ; i < patternLines[pattern]; i++)
    {
        offset_src = Get_Pattern_Offset(pattern, Track_Src, i);
        offset_dst = Get_Pattern_Offset(pattern, Track_Dst, i);

        for(j = 0; j < MAX_POLYPHONY; j++)
        {
            *(RawPatterns + offset_dst + PATTERN_NOTE1 + (j * 2)) = *(RawPatterns + offset_src + PATTERN_NOTE1 + (j * 2));
            *(RawPatterns + offset_dst + PATTERN_INSTR1 + (j * 2)) = *(RawPatterns + offset_src + PATTERN_INSTR1 + (j * 2));
        }
        *(RawPatterns + offset_dst + PATTERN_VOLUME) = *(RawPatterns + offset_src + PATTERN_VOLUME);
        *(RawPatterns + offset_dst + PATTERN_PANNING) = *(RawPatterns + offset_src + PATTERN_PANNING);
        *(RawPatterns + offset_dst + PATTERN_FX) = *(RawPatterns + offset_src + PATTERN_FX);
        *(RawPatterns + offset_dst + PATTERN_FXDATA) = *(RawPatterns + offset_src + PATTERN_FXDATA);
        *(RawPatterns + offset_dst + PATTERN_FX2) = *(RawPatterns + offset_src + PATTERN_FX2);
        *(RawPatterns + offset_dst + PATTERN_FXDATA2) = *(RawPatterns + offset_src + PATTERN_FXDATA2);
        *(RawPatterns + offset_dst + PATTERN_FX3) = *(RawPatterns + offset_src + PATTERN_FX3);
        *(RawPatterns + offset_dst + PATTERN_FXDATA3) = *(RawPatterns + offset_src + PATTERN_FXDATA3);
        *(RawPatterns + offset_dst + PATTERN_FX4) = *(RawPatterns + offset_src + PATTERN_FX4);
        *(RawPatterns + offset_dst + PATTERN_FXDATA4) = *(RawPatterns + offset_src + PATTERN_FXDATA4);
    }
    Channels_Polyphony[Track_Dst] = Channels_Polyphony[Track_Src];
    Channels_MultiNotes[Track_Dst] = Channels_MultiNotes[Track_Src];
    Channels_Effects[Track_Dst] = Channels_Effects[Track_Src];
    Chan_Mute_State[Track_Dst] = Chan_Mute_State[Track_Src];

    Compress_Track[Track_Dst] = Compress_Track[Track_Src];
    mas_comp_threshold_Track[Track_Dst] = mas_comp_threshold_Track[Track_Src];
    mas_comp_ratio_Track[Track_Dst] = mas_comp_ratio_Track[Track_Src];

    TPan[Track_Dst] = TPan[Track_Src];
    TCut[Track_Dst] = TCut[Track_Src];
    ICut[Track_Dst] = ICut[Track_Src];
    FType[Track_Dst] = FType[Track_Src];

    oldspawn[Track_Dst] = oldspawn[Track_Src];
    roldspawn[Track_Dst] = roldspawn[Track_Src];

    Chan_Midi_Prg[Track_Dst] = Chan_Midi_Prg[Track_Src];

    FRez[Track_Dst] = FRez[Track_Src];
    
    DThreshold[Track_Dst] = DThreshold[Track_Src];
    DClamp[Track_Dst] = DClamp[Track_Src];
    Disclap[Track_Dst] = Disclap[Track_Src];
    
    DSend[Track_Dst] = DSend[Track_Src];
    CSend[Track_Dst] = CSend[Track_Src];
    
    Track_Volume[Track_Dst] = Track_Volume[Track_Src];

    Track_Surround[Track_Dst] = Track_Surround[Track_Src];

    LFO_ON[Track_Dst] = LFO_ON[Track_Src];
    LFO_RATE[Track_Dst] = LFO_RATE[Track_Src];
    LFO_AMPL[Track_Dst] = LFO_AMPL[Track_Src];

    FLANGER_ON[Track_Dst] = FLANGER_ON[Track_Src];
    FLANGER_AMOUNT[Track_Dst] = FLANGER_AMOUNT[Track_Src];
    FLANGER_DEPHASE[Track_Dst] = FLANGER_DEPHASE[Track_Src];
    FLANGER_ON[Track_Dst] = FLANGER_ON[Track_Src];
    FLANGER_RATE[Track_Dst] = FLANGER_RATE[Track_Src];
    FLANGER_AMPL[Track_Dst] = FLANGER_AMPL[Track_Src];
    FLANGER_GR[Track_Dst] = FLANGER_GR[Track_Src];
    FLANGER_FEEDBACK[Track_Dst] = FLANGER_FEEDBACK[Track_Src];
    FLANGER_DELAY[Track_Dst] = FLANGER_DELAY[Track_Src];
    FLANGER_OFFSET[Track_Dst] = FLANGER_OFFSET[Track_Src];
    FLANGER_OFFSET2[Track_Dst] = FLANGER_OFFSET2[Track_Src];
    FLANGER_OFFSET1[Track_Dst] = FLANGER_OFFSET1[Track_Src];

    memcpy(&EqDat[Track_Dst], &EqDat[Track_Src], sizeof(EQSTATE));

    for(i = 0; i < Song_Length; i++)
    {
        Chan_Active_State[i][Track_Dst] = Chan_Active_State[i][Track_Src];
    }
    ComputeStereo(Track_Dst);
    FixStereo(Track_Dst);

    /* Make sure the track still look the same */
    Set_Track_Zoom(Track_Dst, Get_Track_Zoom(Track_Src));
}

// ------------------------------------------------------
// Delete a track at current caret position
void Delete_Track(void)
{
    int i;

    for(i = Track_Under_Caret; i < Songtracks; i++)
    {
        Copy_Track(Song_Position, i + 1, i);
    }
    Reset_Track(Song_Position, Songtracks);
    Column_Under_Caret = 0;
}

// ------------------------------------------------------
// Insert a track at current caret position
void Insert_Track(void)
{
    int i;

    if(Songtracks < 16)
    {
        for(i = Songtracks - 1; i > Track_Under_Caret; i--)
        {
            Copy_Track(Song_Position, i - 1, i);
        }
        Reset_Track(Song_Position, Track_Under_Caret);
        Column_Under_Caret = 0;
    }
}
#endif
