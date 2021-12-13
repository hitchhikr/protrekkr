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
#include "../include/ptk.h"
#include "include/draw_primitives.h"

// ------------------------------------------------------
// Constants
#define SHADOW_FACTOR 3.4f

// ------------------------------------------------------
// Variables
int max_colors_logo;
int max_colors_303;
SDL_Surface *Temp_PFONT;
SDL_Surface *Temp_LARGEPFONT;
SDL_Surface *Temp_SMALLPFONT;
SDL_Surface *Temp_NOTEPFONT;
SDL_Surface *Temp_NOTELARGEPFONT;
SDL_Surface *Temp_NOTESMALLPFONT;
int Beveled = 1;
char Use_Shadows = TRUE;

int max_colors_Pointer;
int curr_tab_highlight;

int Nbr_Letters;
int Font_Height = 11;
int Font_Height_Small = 7;
char *Font_Ascii =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    //           é        è    ç   à         ù    µ                   
    "0123456789&\351\"'(-\350_\347\340)=*+^$\371%\265,;:!?./<>@#[]|\\\001\002\003\004\005\006\007\010\011\012\013\014\015"
    //A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z  
    "\216\217\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237\240\241\242\243\244\245\246\247"
    //0   1   2   3   4   5   6   7   8   9
    "\360\361\362\363\364\365\366\367\370\372"
    ///   (   )   :
    "\373\374\375\376"
    //.   -   |>  ||  Wave
    "\250\251\253\254\255"
    " ";
int Font_Pos[256];
int Font_Size[256];

char Accidental = FALSE;

int Accidental_Table[] =
{
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
     12,  12,  13,  13,  14,  15,  15,  16,  16,  10,  10,  11,
      0,   0
};

int Accidental_Table_b[] =
{
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
     12,  13,  13,  14,  14,  15,  16,  16,  10,  10,  11,  11,
      0,   0
};

char *Notes[] =
{
    "C-0", "C#0", "D-0", "D#0", "E-0", "F-0", "F#0", "G-0", "G#0", "A-0", "A#0", "B-0",
    "C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1", "G-1", "G#1", "A-1", "A#1", "B-1",
    "C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2", "G-2", "G#2", "A-2", "A#2", "B-2",
    "C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3", "G-3", "G#3", "A-3", "A#3", "B-3",
    "C-4", "C#4", "D-4", "D#4", "E-4", "F-4", "F#4", "G-4", "G#4", "A-4", "A#4", "B-4",
    "C-5", "C#5", "D-5", "D#5", "E-5", "F-5", "F#5", "G-5", "G#5", "A-5", "A#5", "B-5",
    "C-6", "C#6", "D-6", "D#6", "E-6", "F-6", "F#6", "G-6", "G#6", "A-6", "A#6", "B-6",
    "C-7", "C#7", "D-7", "D#7", "E-7", "F-7", "F#7", "G-7", "G#7", "A-7", "A#7", "B-7",
    "C-8", "C#8", "D-8", "D#8", "E-8", "F-8", "F#8", "G-8", "G#8", "A-8", "A#8", "B-8",
    "C-9", "C#9", "D-9", "D#9", "E-9", "F-9", "F#9", "G-9", "G#9", "A-9", "A#9", "B-9",
    "off"
};

char *Notes_b[] =
{
    "C-0", "Db0", "D-0", "Eb0", "E-0", "F-0", "Gb0", "G-0", "Ab0", "A-0", "Bb0", "B-0",
    "C-1", "Db1", "D-1", "Eb1", "E-1", "F-1", "Gb1", "G-1", "Ab1", "A-1", "Bb1", "B-1",
    "C-2", "Db2", "D-2", "Eb2", "E-2", "F-2", "Gb2", "G-2", "Ab2", "A-2", "Bb2", "B-2",
    "C-3", "Db3", "D-3", "Eb3", "E-3", "F-3", "Gb3", "G-3", "Ab3", "A-3", "Bb3", "B-3",
    "C-4", "Db4", "D-4", "Eb4", "E-4", "F-4", "Gb4", "G-4", "Ab4", "A-4", "Bb4", "B-4",
    "C-5", "Db5", "D-5", "Eb5", "E-5", "F-5", "Gb5", "G-5", "Ab5", "A-5", "Bb5", "B-5",
    "C-6", "Db6", "D-6", "Eb6", "E-6", "F-6", "Gb6", "G-6", "Ab6", "A-6", "Bb6", "B-6",
    "C-7", "Db7", "D-7", "Eb7", "E-7", "F-7", "Gb7", "G-7", "Ab7", "A-7", "Bb7", "B-7",
    "C-8", "Db8", "D-8", "Eb8", "E-8", "F-8", "Gb8", "G-8", "Ab8", "A-8", "Bb8", "B-8",
    "C-9", "Db9", "D-9", "Eb9", "E-9", "F-9", "Gb9", "G-9", "Ab9", "A-9", "Bb9", "B-9",
    "off"
};

char *DecChar[] =
{
    "000", "001", "002", "003", "004", "005", "006", "007", "008", "009",
    "010", "011", "012", "013", "014", "015", "016", "017", "018", "019",
    "020", "021", "022", "023", "024", "025", "026", "027", "028", "029",
    "030", "031", "032", "033", "034", "035", "036", "037", "038", "039",
    "040", "041", "042", "043", "044", "045", "046", "047", "048", "049",
    "050", "051", "052", "053", "054", "055", "056", "057", "058", "059",
    "060", "061", "062", "063", "064", "065", "066", "067", "068", "069",
    "070", "071", "072", "073", "074", "075", "076", "077", "078", "079",
    "080", "081", "082", "083", "084", "085", "086", "087", "088", "089",
    "090", "091", "092", "093", "094", "095", "096", "097", "098", "099",
    "100", "101", "102", "103", "104", "105", "106", "107", "108", "109",
    "110", "111", "112", "113", "114", "115", "116", "117", "118", "119",
    "120", "121", "122", "123", "124", "125", "126", "127", "128", "129",
    "130", "131", "132", "133", "134", "135", "136", "137", "138", "139",
    "140", "141", "142", "143", "144", "145", "146", "147", "148", "149",
    "150", "151", "152", "153", "154", "155", "156", "157", "158", "159",
    "160", "161", "162", "163", "164", "165", "166", "167", "168", "169",
    "170", "171", "172", "173", "174", "175", "176", "177", "178", "179",
    "180", "181", "182", "183", "184", "185", "186", "187", "188", "189",
    "190", "191", "192", "193", "194", "195", "196", "197", "198", "199",
    "200", "201", "202", "203", "204", "205", "206", "207", "208", "209",
    "210", "211", "212", "213", "214", "215", "216", "217", "218", "219",
    "220", "221", "222", "223", "224", "225", "226", "227", "228", "229",
    "230", "231", "232", "233", "234", "235", "236", "237", "238", "239",
    "240", "241", "242", "243", "244", "245", "246", "247", "248", "249",
    "250", "251", "252", "253", "254", "255", "256"
};

char *DecChar2[] =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99",
};

char *HexTab[] =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
};

char *HexTab_NoZero[] =
{
    " 0", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", " A", " B", " C", " D", " E", " F",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
};

SDL_Color Ptk_Palette[256 * 2];
SDL_Color Palette_303[256];
SDL_Color Palette_Pointer[256];
SDL_Color Palette_Logo[256];

char *Labels_Palette[] =
{
    "Background",
    "Foreground",
    "Buttons",
    "Pushed Buttons",
    "Sliders",
    "Vumeter",
    "Vumtr. peak / Caret",
    "Scopes / Samples",
    "Font",
    "Pattern Lo Backgnd",
    "Pattern Lo Foregnd",
    "Pattern Hi Backgnd",
    "Pattern Hi Foregnd",
    "Pattern Sel Backgnd",
    "Pattern Sel Foregnd",
    "Note Lo Foregnd",
    "Note Hi Foregnd",
    "Note Sel Foregnd",
    "Track Off / Mute",
    "Track On / Play"
};

// Real colors
int Idx_Palette[] =
{
    1,
    3,
    6,
    9,
    12,
    14,
    15,
    16,
    17,
    
    19,
    20,
    21,
    22,
    23,
    24,
    
    26,
    28,
    30,
    
    31,
    32
};

int Default_Beveled1 = 1;
SDL_Color Default_Palette1[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 1

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
    { 0x4a, 0x58, 0x6c, 0x00 },      // 3
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0x3c, 0x62, 0x88, 0x00 },      // 6
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x00, 0x34, 0x66, 0x00 },      // 9
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x40, 0x4c, 0x7c, 0x00 },      // 12
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0x10, 0xea, 0xb0, 0x00 },      // 14 vumeter
    { 0xff, 0xc8, 0x7e, 0x00 },      // 15 vumeter peak / Caret
    { 0x88, 0xd0, 0xff, 0x00 },      // 16 scopes / samples

    { 0xdc, 0xf2, 0xff, 0x00 },      // 17 Font hi
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 19 Pattern lo background
    { 0x68, 0x8c, 0xac, 0x00 },      // 20 Pattern lo foreground

    { 0x06, 0x1c, 0x28, 0x00 },      // 21 Pattern hi background
    { 0xa0, 0xbe, 0xe4, 0x00 },      // 22 Pattern hi foreground

    { 0x30, 0x24, 0x34, 0x00 },      // 23 Pattern sel background
    { 0xdc, 0xf2, 0xff, 0x00 },      // 24 Pattern sel foreground

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0xe0, 0xb6, 0x80, 0x00 },      // 26 Note lo foreground

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0xff, 0xd4, 0xb6, 0x00 },      // 28 Note hi foreground

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
    { 0xea, 0xf0, 0xff, 0x00 },      // 30 Note sel foreground

    { 0xff, 0x4a, 0x00, 0x00 },      // 31 mute
    { 0x00, 0x92, 0xb4, 0x00 },      // 32 play

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black
};

int Default_Beveled2 = 1;
SDL_Color Default_Palette2[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0xff, 0xf4, 0xde, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
	{ 0xff, 0xce, 0x9c, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0xff, 0xde, 0xb8, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x88, 0xb8, 0xf2, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0xa2, 0xba, 0xd8, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0x30, 0x60, 0x32, 0x00 },
    { 0xcc, 0x82, 0x6c, 0x00 },
    { 0x24, 0x38, 0x64, 0x00 },

    { 0x06, 0x0e, 0x28, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0xff, 0xf4, 0xde, 0x00 },
    { 0x00, 0x28, 0x54, 0x00 },

    { 0xff, 0xe0, 0xbe, 0x00 },
    { 0x00, 0x28, 0x54, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // Pattern sel foreground
    { 0x00, 0x28, 0x54, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0x7e, 0x2e, 0x56, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0x7e, 0x2e, 0x56, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
	{ 0x00, 0x1c, 0x40, 0x00 },

    { 0xff, 0x54, 0x28, 0x00 },
    { 0x5c, 0x9e, 0xff, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

int Default_Beveled3 = 0;
SDL_Color Default_Palette3[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x00, 0x00, 0x00, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
	{ 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0x30, 0x66, 0x1a, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x10, 0x38, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x00, 0x1c, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0xa6, 0xea, 0x32, 0x00 },
    { 0xff, 0x30, 0x00, 0x00 },
    { 0x64, 0xea, 0x24, 0x00 },

    { 0xe6, 0xff, 0xbc, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x00, 0x00, 0x00, 0x00 },
    { 0x4a, 0xc2, 0x3a, 0x00 },

    { 0x00, 0x26, 0x00, 0x00 },
    { 0x4e, 0xff, 0x32, 0x00 },

    { 0x34, 0x00, 0x00, 0x00 },
    { 0xba, 0xff, 0x98, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0xc4, 0xff, 0x88, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0xc4, 0xff, 0x88, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
	{ 0xce, 0xff, 0x9a, 0x00 },

    { 0x16, 0x4e, 0x00, 0x00 },
    { 0x46, 0xaa, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

int Default_Beveled4 = 1;
SDL_Color Default_Palette4[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x34, 0x2e, 0x4c, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
	{ 0x56, 0x5e, 0x62, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0x3c, 0x40, 0x44, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x1c, 0x1e, 0x20, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0xd6, 0xe8, 0xfa, 0x00 },
    { 0xff, 0x8c, 0x5e, 0x00 },
    { 0xd4, 0xbc, 0x86, 0x00 },

    { 0xe2, 0xf2, 0xff, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x2c, 0x2c, 0x36, 0x00 },
    { 0xd8, 0xe6, 0xee, 0x00 },

    { 0x20, 0x20, 0x2a, 0x00 },
    { 0xb0, 0xbc, 0xc4, 0x00 },

    { 0x10, 0x10, 0x1a, 0x00 },
    { 0xff, 0x8c, 0x5e, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0xa6, 0xe6, 0xff, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0xff, 0xe6, 0xa6, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
	{ 0xff, 0x8c, 0x5e, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },
    { 0x92, 0x9e, 0xa4, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

int Default_Beveled5 = 1;
SDL_Color Default_Palette5[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x00, 0x0A, 0x24, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
    { 0x00, 0x28, 0x6a, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0x00, 0x4e, 0x98, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x16, 0x66, 0x50, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x00, 0x30, 0x76, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0x66, 0xa0, 0xfa, 0x00 },
    { 0x26, 0xb6, 0xff, 0x00 },
    { 0x4e, 0x7e, 0xff, 0x00 },

    { 0xb0, 0xce, 0xff, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x00, 0x16, 0x32, 0x00 },
    { 0x46, 0x90, 0xee, 0x00 },

    { 0x00, 0x0A, 0x24, 0x00 },
    { 0x94, 0xb4, 0xff, 0x00 },

    { 0x0a, 0x32, 0x72, 0x00 },
    { 0x00, 0xa8, 0xff, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0x88, 0xc2, 0xee, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0x90, 0xb2, 0xff, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
    { 0xa0, 0xe8, 0xff, 0x00 },

    { 0x00, 0x08, 0xa6, 0x00 },
    { 0x42, 0x66, 0xd2, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

int Default_Beveled6 = 0;
SDL_Color Default_Palette6[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x1c, 0x00, 0x00, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
	{ 0x24, 0x06, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0xc0, 0x2c, 0x0e, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x68, 0x14, 0x16, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x66, 0x18, 0x14, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0x9c, 0x36, 0x00, 0x00 },
    { 0xff, 0x40, 0x00, 0x00 },
    { 0xff, 0xb6, 0x54, 0x00 },

    { 0xff, 0xe0, 0x9a, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x10, 0x00, 0x00, 0x00 },
    { 0xff, 0x92, 0x36, 0x00 },

    { 0x20, 0x00, 0x00, 0x00 },
    { 0xee, 0x74, 0x3a, 0x00 },

    { 0x00, 0x14, 0x14, 0x00 },
    { 0xff, 0xff, 0x94, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0xff, 0xa6, 0x64, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0xff, 0x7a, 0x2a, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
	{ 0xff, 0xe8, 0x9a, 0x00 },

    { 0xce, 0x00, 0x00, 0x00 },
    { 0xff, 0x74, 0x00, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

int Default_Beveled7 = 2;
SDL_Color Default_Palette7[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x96, 0xa6, 0xb6, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
	{ 0x96, 0xa6, 0xb6, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0x78, 0x86, 0x94, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0xf4, 0xe4, 0x5e, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x5a, 0x66, 0x76, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0xff, 0xff, 0xff, 0x00 },
    { 0x30, 0x60, 0xc8, 0x00 },
    { 0x00, 0x10, 0x20, 0x00 },

    { 0x00, 0x08, 0x16, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x96, 0xa6, 0xb6, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },

    { 0x8a, 0x8c, 0x8e, 0x00 },
    { 0x44, 0x32, 0x4a, 0x00 },

    { 0x58, 0x68, 0x78, 0x00 },
    { 0xe4, 0xf4, 0xff, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0x12, 0x1e, 0x2e, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0x20, 0x10, 0x32, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
	{ 0xda, 0xf0, 0xff, 0x00 },

    { 0x22, 0x86, 0x50, 0x00 },
    { 0x7a, 0xca, 0x9c, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

int Default_Beveled8 = 2;
SDL_Color Default_Palette8[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x46, 0xaa, 0xc0, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
	{ 0x32, 0x96, 0xac, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0x84, 0x14, 0x22, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x16, 0x00, 0x32, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x00, 0x30, 0x44, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0xb2, 0xf6, 0xae, 0x00 },      // vu-meter
    { 0x00, 0x00, 0x00, 0x00 },      // vu-meter peak/caret
    { 0xff, 0xe8, 0xa4, 0x00 },

    { 0xf2, 0xfe, 0xff, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x60, 0xbc, 0xe4, 0x00 },
    { 0x00, 0x22, 0x4a, 0x00 },

    { 0x4c, 0xae, 0xc8, 0x00 },
    { 0x00, 0x4e, 0x5c, 0x00 },

    { 0x66, 0xbe, 0xd0, 0x00 },
    { 0xc2, 0xf4, 0xff, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0x00, 0x38, 0x42, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0x5e, 0x10, 0x32, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
	{ 0xcc, 0xfe, 0xff, 0x00 },

    { 0x1c, 0x68, 0x80, 0x00 },
    { 0x6a, 0xde, 0xe6, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

int Default_Beveled9 = 0;
SDL_Color Default_Palette9[] =
{
    { 0x00, 0x00, 0x00, 0x00 },      // 0 lists/samples/vumeters background/sequencer (calculated)

    { 0x00, 0x00, 0x00, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 2 static highlight (calculated)
	{ 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 4 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 5 interactive parts highlight (calculated)
    { 0x36, 0x24, 0x1c, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 7 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 8 pushed button highlight (calculated)
    { 0x54, 0x32, 0x30, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 10 (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 11 slider highlight (calculated)
    { 0x20, 0x14, 0x0e, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 13 (calculated)

    { 0x58, 0xc6, 0x22, 0x00 },
    { 0x5e, 0x9e, 0xd4, 0x00 },
    { 0x24, 0xac, 0x52, 0x00 },

    { 0xff, 0xf6, 0xee, 0x00 },
    { 0x00, 0x00, 0x00, 0x00 },      // 18 Font lo (calculated)

    { 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xee, 0xc2, 0x00 },

    { 0x16, 0x0e, 0x00, 0x00 },
    { 0xff, 0xf0, 0xd4, 0x00 },

    { 0x36, 0x00, 0x10, 0x00 },
    { 0xff, 0xff, 0xff, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 25 Note lo background (calculated)
    { 0xcc, 0xaa, 0x1e, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 27 Note hi background (calculated)
    { 0xc0, 0xac, 0x1e, 0x00 },

    { 0x00, 0x00, 0x00, 0x00 },      // 29 Note sel background (calculated)
	{ 0xff, 0xff, 0xc4, 0x00 },

    { 0x6a, 0x3a, 0x00, 0x00 },
    { 0xa2, 0x8a, 0x54, 0x00 },

    { 0xff, 0xff, 0xff, 0x00 },      // 33 mute/play highlight (fixed)

    { 0x00, 0x00, 0x00, 0x00 },      // 34 Shadow Pattern lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 35 Shadow Pattern lo foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 36 Shadow Pattern hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 37 Shadow Pattern hi foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 38 Shadow Pattern sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 39 Shadow Pattern sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 40 Shadow Note lo background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 41 Shadow Note lo foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 42 Shadow Note hi background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 43 Shadow Note hi foreground (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 44 Shadow Note sel background (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 45 Shadow Note sel foreground (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 46 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 47 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 48 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 49 RGB double inverted buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 50 RGB double inverted buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 51 RGB double inverted buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 52 RGB double buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 53 RGB double buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 54 RGB double buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 55 RGB double input buttons highlight (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 56 RGB double input buttons (calculated)
    { 0x00, 0x00, 0x00, 0x00 },      // 57 RGB double input buttons shadow (calculated)

    { 0x00, 0x00, 0x00, 0x00 },      // 58 Phony always black (fixed)
};

LONGRGB Phony_Palette[sizeof(Default_Palette2) / sizeof(SDL_Color)];

SDL_Surface *POINTER;
unsigned char *Pointer_BackBuf;
int bare_color_idx;

// ------------------------------------------------------
// Functions
void (*Letter_Function)(int x, int y, char ltr, int ys, int y2) = Letter;
SDL_Surface *Note_Surface;
SDL_Surface *Note_Alt_Surface;

void Blit_note(int x, int y, int note, int y1, int y2, int size, int acc);

void out_decchar(int x, int y, int number, char smith)
{
    PrintString(x, y, USE_FONT, DecChar[number]);
}

void Get_Phony_Palette(void)
{
    int i;

    for(i = 0; i < sizeof(Default_Palette2) / sizeof(SDL_Color); i++)
    {
        Phony_Palette[i].r = Ptk_Palette[i].r;
        Phony_Palette[i].g = Ptk_Palette[i].g;
        Phony_Palette[i].b = Ptk_Palette[i].b;
    }
}

void Set_Phony_Palette(void)
{
    int i;
    int ComponentR;
    int ComponentG;
    int ComponentB;
    int ComponentR2;
    int ComponentG2;
    int ComponentB2;

    for(i = 0; i < sizeof(Default_Palette2) / sizeof(SDL_Color); i++)
    {
        switch(i)
        {
            case COL_STATIC_HI:
            case COL_HI:
            case COL_PUSHED_HI:
            case COL_SLIDER_HI:
                ComponentR = Phony_Palette[i + 1].r;
                ComponentG = Phony_Palette[i + 1].g;
                ComponentB = Phony_Palette[i + 1].b;
                switch(Beveled)
                {
                    case 0:
                    case 1:
                        ComponentR += 0x18;
                        ComponentG += 0x18;
                        ComponentB += 0x18;
                        if(ComponentR > 0xff) ComponentR = 0xff;
                        if(ComponentG > 0xff) ComponentG = 0xff;
                        if(ComponentB > 0xff) ComponentB = 0xff;
                        break;
                    case 2:
                        ComponentR -= 0x18;
                        ComponentG -= 0x18;
                        ComponentB -= 0x18;
                        if(ComponentR < 0) ComponentR = 0;
                        if(ComponentG < 0) ComponentG = 0;
                        if(ComponentB < 0) ComponentB = 0;
                        break;
                }
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                break;

            case COL_STATIC_LO:
            case COL_LO:
            case COL_PUSHED_LO:
            case COL_SLIDER_LO:
                ComponentR = Phony_Palette[i - 1].r;
                ComponentG = Phony_Palette[i - 1].g;
                ComponentB = Phony_Palette[i - 1].b;
                switch(Beveled)
                {
                    case 0:
                        ComponentR += 0x18;
                        ComponentG += 0x18;
                        ComponentB += 0x18;
                        if(ComponentR > 0xff) ComponentR = 0xff;
                        if(ComponentG > 0xff) ComponentG = 0xff;
                        if(ComponentB > 0xff) ComponentB = 0xff;
                        break;
                    case 1:
                    case 2:
                        ComponentR -= 0x18;
                        ComponentG -= 0x18;
                        ComponentB -= 0x18;
                        if(ComponentR < 0) ComponentR = 0;
                        if(ComponentG < 0) ComponentG = 0;
                        if(ComponentB < 0) ComponentB = 0;
                        break;
                }
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                break;

            // The font
            case COL_FONT_HI:
                ComponentR = Phony_Palette[i].r;
                ComponentG = Phony_Palette[i].g;
                ComponentB = Phony_Palette[i].b;
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;

                // Set the shadow font
                if(ComponentR >= 128) ComponentR = 128 + (ComponentR >> 3);
                else ComponentR = (128 - (ComponentR >> 3));
                if(ComponentG >= 128) ComponentG = 128 + (ComponentG >> 3);
                else ComponentG = (128 - (ComponentG >> 3));
                if(ComponentB >= 128) ComponentB = 128 + (ComponentB >> 3);
                else ComponentB = (128 - (ComponentB >> 3));

                if(ComponentR < 0) ComponentR = 0;
                if(ComponentG < 0) ComponentG = 0;
                if(ComponentB < 0) ComponentB = 0;
                if(ComponentR > 0xff) ComponentR = 0xff;
                if(ComponentG > 0xff) ComponentG = 0xff;
                if(ComponentB > 0xff) ComponentB = 0xff;
                Ptk_Palette[i + 1].r = ComponentR;
                Ptk_Palette[i + 1].g = ComponentG;
                Ptk_Palette[i + 1].b = ComponentB;
                Phony_Palette[i + 1].r = ComponentR;
                Phony_Palette[i + 1].g = ComponentG;
                Phony_Palette[i + 1].b = ComponentB;
                break;

            case COL_PATTERN_LO_BACK:
            case COL_PATTERN_SEL_BACK:
                ComponentR = Phony_Palette[i].r;
                ComponentG = Phony_Palette[i].g;
                ComponentB = Phony_Palette[i].b;
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                Ptk_Palette[i + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK)].r = ComponentR;
                Ptk_Palette[i + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK)].g = ComponentG;
                Ptk_Palette[i + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK)].b = ComponentB;
                Phony_Palette[i + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK)].r = ComponentR;
                Phony_Palette[i + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK)].g = ComponentG;
                Phony_Palette[i + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK)].b = ComponentB;
                break;

            case COL_PATTERN_HI_BACK:
                ComponentR = Phony_Palette[i].r;
                ComponentG = Phony_Palette[i].g;
                ComponentB = Phony_Palette[i].b;
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                ComponentR2 = Phony_Palette[COL_PATTERN_LO_BACK].r;
                ComponentG2 = Phony_Palette[COL_PATTERN_LO_BACK].g;
                ComponentB2 = Phony_Palette[COL_PATTERN_LO_BACK].b;
                ComponentR = (ComponentR - ComponentR2);
                ComponentG = (ComponentG - ComponentG2);
                ComponentB = (ComponentB - ComponentB2);
                if(ComponentR > 0) ComponentR = Phony_Palette[i].r - (int) (abs(ComponentR) * 2.0f / SHADOW_FACTOR);
                else ComponentR = Phony_Palette[i].r + (int) (abs(ComponentR) * 2.0f / SHADOW_FACTOR);
                if(ComponentG > 0) ComponentG = Phony_Palette[i].g - (int) (abs(ComponentG) * 2.0f / SHADOW_FACTOR);
                else ComponentG = Phony_Palette[i].g + (int) (abs(ComponentG) * 2.0f / SHADOW_FACTOR);
                if(ComponentB > 0) ComponentB = Phony_Palette[i].b - (int) (abs(ComponentB) * 2.0f / SHADOW_FACTOR);
                else ComponentB = Phony_Palette[i].b + (int) (abs(ComponentB) * 2.0f / SHADOW_FACTOR);
                Ptk_Palette[i + (COL_PATTERN_HI_BACK_SHADOW - COL_PATTERN_HI_BACK)].r = ComponentR;
                Ptk_Palette[i + (COL_PATTERN_HI_BACK_SHADOW - COL_PATTERN_HI_BACK)].g = ComponentG;
                Ptk_Palette[i + (COL_PATTERN_HI_BACK_SHADOW - COL_PATTERN_HI_BACK)].b = ComponentB;
                Phony_Palette[i + (COL_PATTERN_HI_BACK_SHADOW - COL_PATTERN_HI_BACK)].r = ComponentR;
                Phony_Palette[i + (COL_PATTERN_HI_BACK_SHADOW - COL_PATTERN_HI_BACK)].g = ComponentG;
                Phony_Palette[i + (COL_PATTERN_HI_BACK_SHADOW - COL_PATTERN_HI_BACK)].b = ComponentB;
                break;

            case COL_PATTERN_HI_FORE:
            case COL_NOTE_HI_FORE:
                ComponentR = Phony_Palette[i].r;
                ComponentG = Phony_Palette[i].g;
                ComponentB = Phony_Palette[i].b;
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                ComponentR2 = Phony_Palette[COL_PATTERN_HI_BACK].r;
                ComponentG2 = Phony_Palette[COL_PATTERN_HI_BACK].g;
                ComponentB2 = Phony_Palette[COL_PATTERN_HI_BACK].b;
                ComponentR = (ComponentR - ComponentR2);
                ComponentG = (ComponentG - ComponentG2);
                ComponentB = (ComponentB - ComponentB2);
                if(ComponentR > 0) ComponentR = Phony_Palette[i].r - (int) (abs(ComponentR) * 2.0f / SHADOW_FACTOR);
                else ComponentR = Phony_Palette[i].r + (int) (abs(ComponentR) * 2.0f / SHADOW_FACTOR);
                if(ComponentG > 0) ComponentG = Phony_Palette[i].g - (int) (abs(ComponentG) * 2.0f / SHADOW_FACTOR);
                else ComponentG = Phony_Palette[i].g + (int) (abs(ComponentG) * 2.0f / SHADOW_FACTOR);
                if(ComponentB > 0) ComponentB = Phony_Palette[i].b - (int) (abs(ComponentB) * 2.0f / SHADOW_FACTOR);
                else ComponentB = Phony_Palette[i].b + (int) (abs(ComponentB) * 2.0f / SHADOW_FACTOR);
                Ptk_Palette[i + (COL_PATTERN_HI_FORE_SHADOW - COL_PATTERN_HI_FORE)].r = ComponentR;
                Ptk_Palette[i + (COL_PATTERN_HI_FORE_SHADOW - COL_PATTERN_HI_FORE)].g = ComponentG;
                Ptk_Palette[i + (COL_PATTERN_HI_FORE_SHADOW - COL_PATTERN_HI_FORE)].b = ComponentB;
                Phony_Palette[i + (COL_PATTERN_HI_FORE_SHADOW - COL_PATTERN_HI_FORE)].r = ComponentR;
                Phony_Palette[i + (COL_PATTERN_HI_FORE_SHADOW - COL_PATTERN_HI_FORE)].g = ComponentG;
                Phony_Palette[i + (COL_PATTERN_HI_FORE_SHADOW - COL_PATTERN_HI_FORE)].b = ComponentB;
                break;

            case COL_PATTERN_LO_FORE:
            case COL_NOTE_LO_FORE:
                ComponentR = Phony_Palette[i].r;
                ComponentG = Phony_Palette[i].g;
                ComponentB = Phony_Palette[i].b;
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                ComponentR2 = Phony_Palette[COL_PATTERN_LO_BACK].r;
                ComponentG2 = Phony_Palette[COL_PATTERN_LO_BACK].g;
                ComponentB2 = Phony_Palette[COL_PATTERN_LO_BACK].b;
                ComponentR = (ComponentR - ComponentR2);
                ComponentG = (ComponentG - ComponentG2);
                ComponentB = (ComponentB - ComponentB2);
                if(ComponentR > 0) ComponentR = Phony_Palette[i].r - (int) (abs(ComponentR) * 2.0f / SHADOW_FACTOR);
                else ComponentR = Phony_Palette[i].r + (int) (abs(ComponentR) * 2.0f / SHADOW_FACTOR);
                if(ComponentG > 0) ComponentG = Phony_Palette[i].g - (int) (abs(ComponentG) * 2.0f / SHADOW_FACTOR);
                else ComponentG = Phony_Palette[i].g + (int) (abs(ComponentG) * 2.0f / SHADOW_FACTOR);
                if(ComponentB > 0) ComponentB = Phony_Palette[i].b - (int) (abs(ComponentB) * 2.0f / SHADOW_FACTOR);
                else ComponentB = Phony_Palette[i].b + (int) (abs(ComponentB) * 2.0f / SHADOW_FACTOR);
                Ptk_Palette[i + (COL_PATTERN_LO_FORE_SHADOW - COL_PATTERN_LO_FORE)].r = ComponentR;
                Ptk_Palette[i + (COL_PATTERN_LO_FORE_SHADOW - COL_PATTERN_LO_FORE)].g = ComponentG;
                Ptk_Palette[i + (COL_PATTERN_LO_FORE_SHADOW - COL_PATTERN_LO_FORE)].b = ComponentB;
                Phony_Palette[i + (COL_PATTERN_LO_FORE_SHADOW - COL_PATTERN_LO_FORE)].r = ComponentR;
                Phony_Palette[i + (COL_PATTERN_LO_FORE_SHADOW - COL_PATTERN_LO_FORE)].g = ComponentG;
                Phony_Palette[i + (COL_PATTERN_LO_FORE_SHADOW - COL_PATTERN_LO_FORE)].b = ComponentB;
                break;

            case COL_MUTE:
                Ptk_Palette[i].r = Phony_Palette[i].r;
                Ptk_Palette[i].g = Phony_Palette[i].g;
                Ptk_Palette[i].b = Phony_Palette[i].b;
                break;

            case COL_PLAY:
                Ptk_Palette[i].r = Phony_Palette[i].r;
                Ptk_Palette[i].g = Phony_Palette[i].g;
                Ptk_Palette[i].b = Phony_Palette[i].b;
                break;

            case COL_DOUBLE_HI:
            case COL_DOUBLE_MED:
            case COL_DOUBLE_LO:
                ComponentR = Ptk_Palette[(i - COL_DOUBLE_HI) + COL_HI].r;
                ComponentG = Ptk_Palette[(i - COL_DOUBLE_HI) + COL_HI].g;
                ComponentB = Ptk_Palette[(i - COL_DOUBLE_HI) + COL_HI].b;
                ComponentR -= 0x20;
                ComponentB += 0x20;
                if(ComponentR < 0) ComponentR = 0;
                if(ComponentB > 0xff) ComponentB = 0xff;

                ComponentR += 0x15;
                ComponentG += 0x15;
                ComponentB += 0x15;
                if(ComponentR > 0xff) ComponentR = 0xff;
                if(ComponentG > 0xff) ComponentG = 0xff;
                if(ComponentB > 0xff) ComponentB = 0xff;

                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                break;

            case COL_DOUBLE_PUSHED_HI:
            case COL_DOUBLE_PUSHED_MED:
            case COL_DOUBLE_PUSHED_LO:
                ComponentR = Ptk_Palette[(i - COL_DOUBLE_PUSHED_HI) + COL_PUSHED_HI].r;
                ComponentG = Ptk_Palette[(i - COL_DOUBLE_PUSHED_HI) + COL_PUSHED_HI].g;
                ComponentB = Ptk_Palette[(i - COL_DOUBLE_PUSHED_HI) + COL_PUSHED_HI].b;
                ComponentR -= 0x20;
                ComponentB += 0x20;
                if(ComponentR < 0) ComponentR = 0;
                if(ComponentB > 0xff) ComponentB = 0xff;

                ComponentR += 0x15;
                ComponentG += 0x15;
                ComponentB += 0x15;
                if(ComponentR > 0xff) ComponentR = 0xff;
                if(ComponentG > 0xff) ComponentG = 0xff;
                if(ComponentB > 0xff) ComponentB = 0xff;
    
                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                break;

            case COL_INPUT_HI:
            case COL_INPUT_MED:
            case COL_INPUT_LO:
                ComponentR = Ptk_Palette[(i - COL_INPUT_HI) + COL_HI].r;
                ComponentG = Ptk_Palette[(i - COL_INPUT_HI) + COL_HI].g;
                ComponentB = Ptk_Palette[(i - COL_INPUT_HI) + COL_HI].b;
                ComponentG -= 0x20;
                ComponentB += 0x20;
                if(ComponentG < 0) ComponentG = 0;
                if(ComponentB > 0xff) ComponentB = 0xff;

                ComponentR += 0x15;
                ComponentG += 0x15;
                ComponentB += 0x15;
                if(ComponentR > 0xff) ComponentR = 0xff;
                if(ComponentG > 0xff) ComponentG = 0xff;
                if(ComponentB > 0xff) ComponentB = 0xff;

                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                break;

            case COL_INPUT_PUSHED_HI:
            case COL_INPUT_PUSHED_MED:
            case COL_INPUT_PUSHED_LO:
                ComponentR = Ptk_Palette[(i - COL_INPUT_PUSHED_HI) + COL_PUSHED_HI].r;
                ComponentG = Ptk_Palette[(i - COL_INPUT_PUSHED_HI) + COL_PUSHED_HI].g;
                ComponentB = Ptk_Palette[(i - COL_INPUT_PUSHED_HI) + COL_PUSHED_HI].b;
                ComponentG -= 0x20;
                ComponentB += 0x20;
                if(ComponentG < 0) ComponentG = 0;
                if(ComponentB > 0xff) ComponentB = 0xff;

                ComponentR += 0x15;
                ComponentG += 0x15;
                ComponentB += 0x15;
                if(ComponentR > 0xff) ComponentR = 0xff;
                if(ComponentG > 0xff) ComponentG = 0xff;
                if(ComponentB > 0xff) ComponentB = 0xff;

                Ptk_Palette[i].r = ComponentR;
                Ptk_Palette[i].g = ComponentG;
                Ptk_Palette[i].b = ComponentB;
                break;

            default:
                Ptk_Palette[i].r = Phony_Palette[i].r;
                Ptk_Palette[i].g = Phony_Palette[i].g;
                Ptk_Palette[i].b = Phony_Palette[i].b;
                break;
        }
    }
}

void value_box(int x, int y, int val, int flags)
{
    Gui_Draw_Button_Box(x, y, 16, 16, "\03", flags);
    Gui_Draw_Button_Box(x + 18, y, 24, 16, HexTab[val], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(x + 44, y, 16, 16, "\04", flags);
}

void value_box_format(int x, int y, int val, int flags, char *format)
{
    char String[64];
    sprintf(String, format, val);
    Gui_Draw_Button_Box(x, y, 16, 16, "\03", flags);
    Gui_Draw_Button_Box(x + 18, y, 24, 16, String, BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(x + 44, y, 16, 16, "\04", flags);
}

void Gui_Draw_Arrows_Number_Box(int x, int y, int val, int flags)
{
    Gui_Draw_Button_Box(x, y, 16, 16, "\03", flags);
    Gui_Draw_Button_Box(x + 18, y, 24, 16, DecChar[val], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(x + 44, y, 16, 16, "\04", flags);
}

void Gui_Draw_Arrows_Number_Box2(int x, int y, int val, int flags)
{
    Gui_Draw_Button_Box(x, y, 16, 16, "\03", flags);
    Gui_Draw_Button_Box(x + 18, y, 24, 16, DecChar2[val], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(x + 44, y, 16, 16, "\04", flags);
}

void value_box4(int x, int y, int val)
{
    Gui_Draw_Button_Box(x, y, 24, 16, HexTab[val], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
}

void value_box3(int x, int y, char val, int flags)
{
    Gui_Draw_Button_Box(x, y, 16, 16, "\03", flags);
    Gui_Draw_Button_Box(x + 44, y, 16, 16, "\04", flags);
    if(Accidental)
    {
        Gui_Draw_Button_Box(x + 18, y, 24, 16, Notes_b[val], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    }
    else
    {
        Gui_Draw_Button_Box(x + 18, y, 24, 16, Notes[val], BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    }
}

void outlong_small(int x, int y, int value, int mode, int size, int flags)
{
    char xstr[40];

    switch(mode)
    {
        case 0: sprintf(xstr, "%d", value); break;
        case 1: sprintf(xstr, "%d%%", value); break;
        case 2: sprintf(xstr, "%d ms.", value); break;
        case 3: sprintf(xstr, "%dhz", value); break;
        case 5: sprintf(xstr, "%d K.", value); break;
        case 6: sprintf(xstr, "%d Degr.", value); break;
        case 10: sprintf(xstr, "S: %d", value); break;
        case 11: sprintf(xstr, "E: %d", value); break;
        case 12: sprintf(xstr, "L: %d", value); break;
    }
    Gui_Draw_Button_Box(x, y, size - 1, 16, xstr, flags);
}

void outlong(int x, int y, int cant, int mode)
{
    char xstr[40];

    switch(mode)
    {
        case 0: sprintf(xstr, "%d", cant); break;
        case 1: sprintf(xstr, "%d%%", cant); break;
        case 2: sprintf(xstr, "%d ms.", cant); break;
        case 3: sprintf(xstr, "%d Hz.", cant); break;
        case 5: sprintf(xstr, "%d K.", cant); break;
        case 6: sprintf(xstr, "%d Degr.", cant); break;
        case 7:
            sprintf(xstr, "%.07d Bytes", cant);
            Gui_Draw_Button_Box(x, y, 80, 16, xstr, BUTTON_NORMAL | BUTTON_DISABLED);
            break;
        case 10: sprintf(xstr, "S: %d", cant); break;
        case 11: sprintf(xstr, "E: %d", cant); break;
        case 12: sprintf(xstr, "L: %d", cant); break;
    }
    if(mode != 7) Gui_Draw_Button_Box(x, y, 60, 16, xstr, BUTTON_NORMAL | BUTTON_DISABLED);
}

void outfloat(int x, int y, float cant, int mode)
{
    char xstr[40];

    switch (mode)
    {
        case 0: sprintf(xstr, "%.3f", cant); break;
        case 1: sprintf(xstr, "%.2f%%", cant); break;
        case 2: sprintf(xstr, "%.3f ms.", cant); break;
        case 3: sprintf(xstr, "%.3f Hz.", cant); break;
        case 5: sprintf(xstr, "%.3f K.", cant); break;
        case 8: sprintf(xstr, "%.1f Tk.", cant); break;
    }

    Gui_Draw_Button_Box(x, y, 60, 16, xstr, BUTTON_NORMAL | BUTTON_DISABLED);
}

void outfloat_small(int x, int y, float cant, int mode, int size, int flags)
{
    char xstr[40];

    switch (mode)
    {
        case 0: sprintf(xstr, "%.3f", cant); break;
        case 1: sprintf(xstr, "%.2f%%", cant); break;
        case 2: sprintf(xstr, "%.3f ms.", cant); break;
        case 3: sprintf(xstr, "%.3f Hz", cant); break;
        case 5: sprintf(xstr, "%.3f K.", cant); break;
        case 8: sprintf(xstr, "%.1f Tk", cant); break;
    }

    Gui_Draw_Button_Box(x, y, size - 1, 16, xstr, flags);
}

// ------------------------------------------------------
// Draw the editors buttons bar
void Draw_Editors_Bar(int Highlight)
{
    int Highlight_Tab[] =
    {
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL,
        BUTTON_NORMAL
    };
    
    if(Highlight == -1)
    {
        SetColor(COL_BLACK);
        bjbox(0, Cur_Height - 172, Cur_Width, 19);
        Highlight = curr_tab_highlight;
    }
    if(Highlight != USER_SCREEN_LARGE_PATTERN)
    {
        if(Patterns_Lines_Offset)
        {
            SetColor(COL_BLACK);
            bjbox(0, Cur_Height - 172, Cur_Width, 19);
        }
        Highlight_Tab[Highlight] = BUTTON_PUSHED;
        Large_Patterns = FALSE;
        Set_Pattern_Size();
        Draw_Pattern_Right_Stuff();
        Actupated(0);
    }
    else
    {
        userscreen = USER_SCREEN_LARGE_PATTERN;
        SetColor(COL_BLACK);
        bjbox(0, (Cur_Height - 172) + Patterns_Lines_Offset, Cur_Width, 19);
        Highlight_Tab[USER_SCREEN_LARGE_PATTERN] = BUTTON_PUSHED;
        Draw_Pattern_Right_Stuff();
        Actupated(0);
    }
    
    if(Patterns_Lines_Offset == 0)
    {
        Gui_Draw_Button_Box(0, (Cur_Height - 171) + Patterns_Lines_Offset, 18, 16, "\010", Highlight_Tab[11] | BUTTON_TEXT_CENTERED);
    }
    else
    {
        Gui_Draw_Button_Box(0, (Cur_Height - 171) + Patterns_Lines_Offset, 18, 16, "\007", Highlight_Tab[11] | BUTTON_TEXT_CENTERED);
    }
    Gui_Draw_Button_Box(20, (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Sequencer", Highlight_Tab[4]);
    Gui_Draw_Button_Box(20 + TAB_LARG, (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Instrument", Highlight_Tab[2] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 2), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Synth", Highlight_Tab[6] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 3), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Sample Ed.", Highlight_Tab[8] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 4), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "303 Units", Highlight_Tab[9] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 5), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Track (I)", Highlight_Tab[1] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 6), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Track (II)", Highlight_Tab[7] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 7), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "FX Setup", Highlight_Tab[3] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 8), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Reverb", Highlight_Tab[10] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 9), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Disk IO", Highlight_Tab[0] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 10), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "UI Setup", Highlight_Tab[5] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(20 + (TAB_LARG * 11), (Cur_Height - 171) + Patterns_Lines_Offset, TAB_LARG - 2, 16, "Midi Setup", Highlight_Tab[12] | BUTTON_TEXT_CENTERED);

    if(Highlight != USER_SCREEN_LARGE_PATTERN)
    {
        curr_tab_highlight = Highlight;
    }
}

// ------------------------------------------------------
// Refresh the whole interface context
void Refresh_UI_Context(void)
{
    switch(userscreen)
    {
        case USER_SCREEN_REVERB_EDIT:
            Draw_Reverb_Ed();
            break;
        case USER_SCREEN_DISKIO_EDIT:
            Draw_DiskIO_Ed();
            break;
        case USER_SCREEN_TB303_EDIT:
            Draw_303_Ed();
            break;
        case USER_SCREEN_INSTRUMENT_EDIT:
            Draw_Instrument_Ed();
            break;
        case USER_SCREEN_SEQUENCER:
            Draw_Sequencer_Ed();
            break;
        case USER_SCREEN_TRACK_EDIT:
            Draw_Track_Ed();
            break;
        case USER_SCREEN_SETUP_EDIT:
            Draw_Master_Ed();
            break;
        case USER_SCREEN_SETUP_MIDI:
            Draw_Midi_Ed();
            break;
        case USER_SCREEN_SYNTH_EDIT:
            Draw_Synth_Ed();
            break;
        case USER_SCREEN_SAMPLE_EDIT:
            Draw_Sample_Ed();
            break;
        case USER_SCREEN_FX_SETUP_EDIT:
            Draw_Fx_Ed();
            break;
        case USER_SCREEN_TRACK_FX_EDIT:
            Draw_Track_Fx_Ed();
            break;
        case USER_SCREEN_LARGE_PATTERN:
            Draw_Editors_Bar(USER_SCREEN_LARGE_PATTERN);
            break;
    }
    seditor = 0;
    
    Actualize_Midi_Ed(0);
    Actualize_Reverb_Ed(0);
    Actualize_DiskIO_Ed(0);
    Actualize_303_Ed(0);
    Actualize_Instrument_Ed(0, 0);
    Actualize_Sequencer();
    Display_Master_Comp();
    Display_Master_Volume();
    Display_Shuffle();
    Actualize_Master(0);
    Actualize_Master(4);

    Actualize_Seq_Ed(0);
    Actualize_Track_Ed(0);
    Actualize_Master_Ed(0);
    Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
    Actualize_Sample_Ed(0);
    Actualize_Fx_Ed(0);
    Actualize_Track_Fx_Ed(0);
    Actualize_Patterned();
    Actualize_Instruments_Synths_List(0);
    Display_Beat_Time();
}

// ------------------------------------------------------
// Print a text on the screen
void Print_String(char *str, int x, int y, int size_x, int flags)
{
    if(flags & BUTTON_TEXT_CENTERED)
    {
        x += (size_x - Get_Size_Text(str)) / 2;
    }
    PrintString(x, y, USE_FONT, (char *) str);
}

// ------------------------------------------------------
// Draw a clickable box with a text
void Gui_Draw_Button_Box(int x, int y, int sx, int sy, const char *str, int flags)
{
    int x2 = x + sx;
    int y2 = y + sy;
    int y_center;
    int Col_Idx = (flags & BUTTON_RIGHT_MOUSE) ? 4: 0;
    Col_Idx |= (flags & BUTTON_INPUT) ? 8: 0;

    if(sx <= 0) return;
    if(sy <= 0) return;
    
    int Colors_Norm[] =
    {
        COL_MED, COL_HI, COL_LO, 0, 
        COL_DOUBLE_MED, COL_DOUBLE_HI, COL_DOUBLE_LO, 0,
        COL_INPUT_MED, COL_INPUT_HI, COL_INPUT_LO, 0
    };
    int Colors_Pushed[] =
    {
        COL_PUSHED_MED, COL_PUSHED_HI, COL_PUSHED_LO, 0,
        COL_DOUBLE_PUSHED_MED, COL_DOUBLE_PUSHED_HI, COL_DOUBLE_PUSHED_LO, 0,
        COL_INPUT_PUSHED_MED, COL_INPUT_PUSHED_HI, COL_INPUT_PUSHED_LO, 0 
    };

    if(!(flags & BUTTON_NO_BORDER))
    {
        if(flags & BUTTON_NORMAL)
        {
            if(flags & BUTTON_DISABLED)
            {
                SetColor(COL_STATIC_MED);
                Fillrect(x, y, x2, y2 + 1);
                SetColor(COL_STATIC_HI);
                DrawLine(x, y, x2 - 1, y);
                DrawLine(x, y, x, y2);
                SetColor(COL_STATIC_LO);
                DrawLine(x, y2, x2, y2);
                DrawLine(x2, y, x2, y2);
            }
            else
            {
                SetColor(Colors_Norm[Col_Idx]);
                Fillrect(x, y, x2, y2 + 1);
                SetColor(Colors_Norm[Col_Idx + 1]);
                DrawLine(x, y, x2 - 1, y);
                DrawLine(x, y, x, y2);
                SetColor(Colors_Norm[Col_Idx + 2]);
                DrawLine(x, y2, x2, y2);
                DrawLine(x2, y, x2, y2);
            }
        }
        else
        {
            if(flags & BUTTON_DISABLED)
            {
                SetColor(COL_STATIC_MED);
                Fillrect(x, y, x2, y2 + 1);
                SetColor(COL_STATIC_LO);
                DrawLine(x, y, x2 - 1, y);
                DrawLine(x, y, x, y2);
                SetColor(COL_STATIC_HI);
                DrawLine(x, y2, x2, y2);
                DrawLine(x2, y, x2, y2);
            }
            else
            {
                SetColor(Colors_Pushed[Col_Idx]);
                Fillrect(x, y, x2, y2 + 1);
                SetColor(Colors_Pushed[Col_Idx + 2]);
                DrawLine(x, y, x2 - 1, y);
                DrawLine(x, y, x, y2);
                SetColor(Colors_Pushed[Col_Idx + 1]);
                DrawLine(x, y2, x2, y2);
                DrawLine(x2, y, x2, y2);
            }
        }
    }

    if(flags & BUTTON_TEXT_VTOP)
    {
        y_center = 2;
    }
    else
    {
        if(flags & BUTTON_SMALL_FONT)
        {
            y_center = (((sy - 2) - Font_Height_Small) / 2);
        }
        else
        {
            y_center = ((sy - Font_Height) / 2);
        }
    }

    if(flags & BUTTON_TEXT_CENTERED)
    {
        x += ((sx + 1) - Get_Size_Text((char *) str)) / 2;
        PrintString(x, y + y_center, flags & BUTTON_LOW_FONT ? USE_FONT_LOW : USE_FONT, (char *) str);
    }
    else
    {
        PrintString(x + 4, y + y_center, flags & BUTTON_LOW_FONT ? USE_FONT_LOW : USE_FONT, (char *) str);
    }
}

// ------------------------------------------------------
// Draw a clear box
void Gui_Clear_Array(int x, int y, int sx, int sy)
{
    SetColor(COL_STATIC_MED);
    bjbox(x, y, sx, sy);
}

// ------------------------------------------------------
// Draw a flat background box with a text
void Gui_Draw_Flat_Box(const char *str)
{
    SetColor(COL_STATIC_MED);

    Fillrect(2, (Cur_Height - 150), Cur_Width - 6, (Cur_Height - 24));
    PrintString(4, (Cur_Height - 151), USE_FONT, (char *) str);
}

// ------------------------------------------------------
// Draw a rectangle
void bjbox(int x, int y, int sx, int sy)
{
    Fillrect(x, y, x + sx, y + sy);
}

// ------------------------------------------------------
// Display a status message at the bottom of the screen
void Status_Box(char const *str)
{
    Gui_Draw_Button_Box(0, CONSOLE_HEIGHT - 21, fsize, 18, str, BUTTON_NORMAL | BUTTON_DISABLED);
}
      
// ------------------------------------------------------
// Display a horizontal slider
void Realslider_Size(int x, int y, int size, int val, int Enabled)
{   y--;
    if(Enabled)
    {
        SetColor(COL_STATIC_LO);
        bjbox(x + 2, y + 1, size + 17, 17);
        SetColor(COL_STATIC_HI);
        bjbox(x + 3, y + 2, size + 16, 16);
        SetColor(COL_INPUT_MED);
        bjbox(x + 3, y + 2, val + 15, 15);
        SetColor(COL_SLIDER_MED);
        bjbox(x + 4 + val, y + 2, size - val + 14, 15);
    }
    else
    {
        SetColor(COL_STATIC_LO);
        bjbox(x + 2, y + 1, size + 17, 17);
        SetColor(COL_STATIC_HI);
        bjbox(x + 3, y + 2, size + 16, 16);
        SetColor(COL_STATIC_MED);
        bjbox(x + 3, y + 2, size + 15, 15);
    }
    if(Enabled)
    {
        Gui_Draw_Button_Box(x + 4 + val, y + 2, 13, 14, "", BUTTON_NORMAL);
    }
    else
    {
        Gui_Draw_Button_Box(x + 4 + val, y + 2, 13, 14, "", BUTTON_NORMAL | BUTTON_DISABLED);
    }
}

void Realslider(int x, int y, int val, int Enabled)
{
    Realslider_Size(x, y, 128, val, Enabled);
}

void Realslider2(int x, int y, int val, int Enabled)
{
    Realslider_Size(x, y, 64, val, Enabled);
}

// ------------------------------------------------------
// Get the center position of a slider
int Slider_Get_Center(unsigned int Size, unsigned int Maximum, int Pixels)
{
    // Normalize
    float ratio = (float) Maximum / (float) Size;
    int caret_size = (int) ((float) Pixels / ratio);
    if(caret_size == Pixels) caret_size--;
    return(caret_size);
}

// ------------------------------------------------------
// Calculate the caret size of a slider
float Slider_Calc_Size(int displayed, int maximum, int size)
{
    float caret_size;
    float ratio;

    if(maximum < displayed) displayed = maximum;
    ratio = (float) maximum / (float) displayed;
    caret_size = ((float) size / (float) ratio);
    return(caret_size);
}

// ------------------------------------------------------
// Calculate the caret position of a slider
float Slider_Calc_Pos(int displayed, int maximum, int size, int value)
{
    float Pos_Slider;
    float ratio;

    if(maximum < displayed) displayed = maximum;
    ratio = (float) maximum / (float) displayed;
    if(maximum == displayed) Pos_Slider = 0.0f;
    else Pos_Slider = (float) value * ((((float) size) - ((float) (size) / (float) ratio)) / ((float) maximum - (float) displayed));
    return(Pos_Slider);
}

// ------------------------------------------------------
// Display a horizontal slider
void Realslider_Horiz(int x, int y, int value, int displayed, int maximum, int size, int enable)
{
    float caret_size;
    float Pos_slider;

    size -= 2;

    Pos_slider = Slider_Calc_Pos(displayed, maximum, size, value);

    caret_size = Slider_Calc_Size(displayed, maximum, size);

    if((caret_size + Pos_slider) > (size - 1)) caret_size -= (caret_size + Pos_slider) - (size - 1);
   
    if(enable) SetColor(COL_SLIDER_LO);
    else SetColor(COL_STATIC_LO);
    bjbox(x, y, size + 2, 16 + 1);
    if(enable) SetColor(COL_SLIDER_HI);
    else SetColor(COL_STATIC_HI);
    bjbox(x + 1, y + 1, size + 1, 16);
    if(enable) SetColor(COL_SLIDER_MED);
    else SetColor(COL_STATIC_MED);
    bjbox(x + 1, y + 1, size, 16 - 1);

    Gui_Draw_Button_Box(x + 1 + (int) Pos_slider, y + 1, (int) caret_size, 16 - 2, "", BUTTON_NORMAL | (enable ? 0 : BUTTON_DISABLED));
}

// ------------------------------------------------------
// Display a vertical slider
void Realslider_Vert(int x,
                     int y,
                     int value,
                     int displayed,
                     int maximum,
                     int size,
                     int enable,
                     int ext_color)
{
    float caret_size;
    float Pos_slider;
    float slide_max_on;

    Pos_slider = Slider_Calc_Pos(displayed, maximum, size, value);

    size -= 2;

    caret_size = Slider_Calc_Size(displayed, maximum, size);

    if((caret_size + Pos_slider) > size) caret_size -= (caret_size + Pos_slider) - size;

    if(enable)
    {
        SetColor(COL_STATIC_LO);
        bjbox(x, y, 16 + 1, size + 2);
        SetColor(COL_STATIC_HI);
        bjbox(x + 1, y + 1, 16, size + 1);
        SetColor(COL_SLIDER_MED);
        bjbox(x + 1, y + 1, 16 - 1, size);
        SetColor(COL_INPUT_MED);
        slide_max_on = ceil((float) size - Pos_slider );
        bjbox(x + 1, y + 1 + Pos_slider, 16 - 1, slide_max_on);
    }
    else
    {
        SetColor(COL_STATIC_LO);
        bjbox(x, y, 16 + 1, size + 2);
        SetColor(COL_STATIC_HI);
        bjbox(x + 1, y + 1, 16, size + 1);
        SetColor(COL_STATIC_MED);
        bjbox(x + 1, y + 1, 16 - 1, size);
    }    
    Gui_Draw_Button_Box(x + 1, y + 1 + (int) Pos_slider, 16 - 2, (int) caret_size, "", BUTTON_NORMAL | (enable ? 0 : BUTTON_DISABLED));
}

// ------------------------------------------------------
// dest x
// dest y
// letter
// src y
// length y
void Letter(int x, int y, char ltr, int ys, int y2)
{
    switch(ltr)
    {
        case 0: Copy(Temp_PFONT, x, y, 72, ys, 79, y2); break;
        case 1: Copy(Temp_PFONT, x, y, 80, ys, 87, y2); break;
        case 2: Copy(Temp_PFONT, x, y, 88, ys, 95, y2); break;
        case 3: Copy(Temp_PFONT, x, y, 96, ys, 103, y2); break;
        case 4: Copy(Temp_PFONT, x, y, 104, ys, 111, y2); break;
        case 5: Copy(Temp_PFONT, x, y, 112, ys, 119, y2); break;
        case 6: Copy(Temp_PFONT, x, y, 120, ys, 127, y2); break;
        case 7: Copy(Temp_PFONT, x, y, 128, ys, 135, y2); break;
        case 8: Copy(Temp_PFONT, x, y, 136, ys, 143, y2); break;
        case 9: Copy(Temp_PFONT, x, y, 144, ys, 151, y2); break;
        case 10: Copy(Temp_PFONT, x, y, 0, ys, 7, y2); break; // A
        case 11: Copy(Temp_PFONT, x, y, 8, ys, 15, y2); break;// B
        case 12: Copy(Temp_PFONT, x, y, 16, ys, 23, y2); break;// C
        case 13: Copy(Temp_PFONT, x, y, 24, ys, 31, y2); break;// D
        case 14: Copy(Temp_PFONT, x, y, 32, ys, 39, y2); break;// E
        case 15: Copy(Temp_PFONT, x, y, 40, ys, 47, y2); break;// F
        case 16: Copy(Temp_PFONT, x, y, 48, ys, 55, y2); break;// G
        case 17: Copy(Temp_PFONT, x, y, 64, ys, 71, y2); break; // #
        case 18: Copy(Temp_PFONT, x, y, 176, ys, 183, y2); break; // -
        case 19: Copy(Temp_PFONT, x, y, 152, ys, 175, y2); break; // Off
        case 20: Copy(Temp_PFONT, x, y, 56, ys, 63, y2); break; // Blank
        case 21: Copy(Temp_PFONT, x, y, 184, ys, 191, y2); break; // .

        case 23: Copy(Temp_PFONT, x, y, 56, 64, 56 + 26, 64 + 6); break; // ON
        case 24: Copy(Temp_PFONT, x, y, 84, 64, 84 + 26, 64 + 6); break; // OFF
        case 25: Copy(Temp_PFONT, x, y,  0, 64,  0 + 26, 64 + 6); break; // MUTE
        case 26: Copy(Temp_PFONT, x, y, 28, 64, 28 + 26, 64 + 6); break; // PLAY

        case 27: Copy(Temp_PFONT, x, y, 193, 64, 193 + 14, 64 + 6); break; // ZOOM ON
        case 28: Copy(Temp_PFONT, x, y, 221, 64, 221 + 14, 64 + 6); break; // ZOOM OFF

        case 29: Copy(Temp_PFONT, x, y, 56, ys, 59, y2); break; // Blank (4 pixels)
        case 30: Copy(Temp_PFONT, x, y, 56, ys, 57, y2); break; // Blank (2 pixels)

        case 31: Copy(Temp_PFONT, x, y, 111, 64, 111 + 4, 64 + 6); break; // FX ARROW LO BACK
        case 32: Copy(Temp_PFONT, x, y, 138, 64, 138 + 4, 64 + 6); break; // FX ARROW HI BACK
        case 33: Copy(Temp_PFONT, x, y, 165, 64, 165 + 4, 64 + 6); break; // FX ARROW SEL BACK

        case 34: Copy(Temp_PFONT, x, y, 56, ys, 56, y2); break; // Blank (1 pixels)

        case 35: Copy(Temp_PFONT, x, y, 249, 64, 249 + 6, 64 + 6); break; // REDUCE NOTES ON
        case 36: Copy(Temp_PFONT, x, y, 257, 64, 257 + 6, 64 + 6); break; // EXPAND NOTES ON
        case 37: Copy(Temp_PFONT, x, y, 277, 64, 277 + 6, 64 + 6); break; // REDUCE NOTES OFF
        case 38: Copy(Temp_PFONT, x, y, 285, 64, 285 + 6, 64 + 6); break; // EXPAND NOTES OFF
    }
}

// ------------------------------------------------------
// dest x
// dest y
// letter
// src y
// length y
void Note_Letter(int x, int y, char ltr, int ys, int y2)
{
    switch(ltr)
    {
        case 0: Copy(Note_Surface, x, y, 72, ys, 79, y2); break; // 0
        case 1: Copy(Note_Surface, x, y, 80, ys, 87, y2); break; // 1
        case 2: Copy(Note_Surface, x, y, 88, ys, 95, y2); break; // 2
        case 3: Copy(Note_Surface, x, y, 96, ys, 103, y2); break; // 3
        case 4: Copy(Note_Surface, x, y, 104, ys, 111, y2); break; // 4
        case 5: Copy(Note_Surface, x, y, 112, ys, 119, y2); break; // 5
        case 6: Copy(Note_Surface, x, y, 120, ys, 127, y2); break; // 6
        case 7: Copy(Note_Surface, x, y, 128, ys, 135, y2); break; // 7
        case 8: Copy(Note_Surface, x, y, 136, ys, 143, y2); break; // 8
        case 9: Copy(Note_Surface, x, y, 144, ys, 151, y2); break; // 9
        case 10: Copy(Note_Surface, x, y, 0, ys, 7, y2); break; // A
        case 11: Copy(Note_Surface, x, y, 8, ys, 15, y2); break;// B
        case 12: Copy(Note_Surface, x, y, 16, ys, 23, y2); break;// C
        case 13: Copy(Note_Surface, x, y, 24, ys, 31, y2); break;// D
        case 14: Copy(Note_Surface, x, y, 32, ys, 39, y2); break;// E
        case 15: Copy(Note_Surface, x, y, 40, ys, 47, y2); break;// F
        case 16: Copy(Note_Surface, x, y, 48, ys, 55, y2); break;// G
        case 17:
        {
            if(Accidental)
            {
                Copy(Note_Surface, x, y, 193, ys, 193 + 7, y2); break; // b
            }
            else
            {
                Copy(Note_Surface, x, y, 64, ys, 71, y2); break; // #
            }
        }
        case 18: Copy(Note_Surface, x, y, 176, ys, 183, y2); break; // -
        case 19: Copy(Note_Surface, x, y, 152, ys, 175, y2); break; // Off
        case 20: Copy(Note_Surface, x, y, 56, ys, 63, y2); break; // Blank
        case 21: Copy(Note_Surface, x, y, 184, ys, 191, y2); break; // .

        case 23: Copy(Temp_PFONT, x, y, 56, 64, 56 + 26, 64 + 6); break; // ON
        case 24: Copy(Temp_PFONT, x, y, 84, 64, 84 + 26, 64 + 6); break; // OFF
        case 25: Copy(Temp_PFONT, x, y,  0, 64,  0 + 26, 64 + 6); break; // MUTE
        case 26: Copy(Temp_PFONT, x, y, 28, 64, 28 + 26, 64 + 6); break; // PLAY

        case 27: Copy(Temp_PFONT, x, y, 193, 64, 193 + 14, 64 + 6); break; // ZOOM ON
        case 28: Copy(Temp_PFONT, x, y, 221, 64, 221 + 14, 64 + 6); break; // ZOOM OFF

        case 29: Copy(Temp_PFONT, x, y, 56, ys, 59, y2); break; // Blank (4 pixels)
        case 30: Copy(Temp_PFONT, x, y, 56, ys, 57, y2); break; // Blank (2 pixels)

        case 31: Copy(Temp_PFONT, x, y, 111, 64, 111 + 4, 64 + 6); break; // FX ARROW LO BACK
        case 32: Copy(Temp_PFONT, x, y, 138, 64, 138 + 4, 64 + 6); break; // FX ARROW HI BACK
        case 33: Copy(Temp_PFONT, x, y, 165, 64, 165 + 4, 64 + 6); break; // FX ARROW SEL BACK

        case 35: Copy(Temp_PFONT, x, y, 249, 64, 249 + 6, 64 + 6); break; // REDUCE NOTES ON
        case 36: Copy(Temp_PFONT, x, y, 257, 64, 257 + 6, 64 + 6); break; // EXPAND NOTES ON
        case 37: Copy(Temp_PFONT, x, y, 277, 64, 277 + 6, 64 + 6); break; // REDUCE NOTES OFF
        case 38: Copy(Temp_PFONT, x, y, 285, 64, 285 + 6, 64 + 6); break; // EXPAND NOTES OFF
    }
}

// ------------------------------------------------------
void Large_Letter(int x, int y, char ltr, int ys, int y2)
{
    switch(ltr)
    {
        case 0: Copy(Temp_LARGEPFONT, x, y, 100, ys, 100 + 11, y2); break;
        case 1: Copy(Temp_LARGEPFONT, x, y, 111, ys, 111 + 11, y2); break;
        case 2: Copy(Temp_LARGEPFONT, x, y, 122, ys, 122 + 11, y2); break;
        case 3: Copy(Temp_LARGEPFONT, x, y, 133, ys, 133 + 11, y2); break;
        case 4: Copy(Temp_LARGEPFONT, x, y, 144, ys, 144 + 11, y2); break;
        case 5: Copy(Temp_LARGEPFONT, x, y, 155, ys, 155 + 11, y2); break;
        case 6: Copy(Temp_LARGEPFONT, x, y, 166, ys, 166 + 11, y2); break;
        case 7: Copy(Temp_LARGEPFONT, x, y, 177, ys, 177 + 11, y2); break;
        case 8: Copy(Temp_LARGEPFONT, x, y, 188, ys, 188 + 11, y2); break;
        case 9: Copy(Temp_LARGEPFONT, x, y, 199, ys, 199 + 11, y2); break;

        case 10: Copy(Temp_LARGEPFONT, x, y, 0, ys, 11, y2); break; // A
        case 11: Copy(Temp_LARGEPFONT, x, y, 11, ys, 11 + 11, y2); break;// B
        case 12: Copy(Temp_LARGEPFONT, x, y, 22, ys, 22 + 11, y2); break;// C
        case 13: Copy(Temp_LARGEPFONT, x, y, 33, ys, 33 + 11, y2); break;// D
        case 14: Copy(Temp_LARGEPFONT, x, y, 44, ys, 44 + 11, y2); break;// E
        case 15: Copy(Temp_LARGEPFONT, x, y, 55, ys, 55 + 11, y2); break;// F
        case 16: Copy(Temp_LARGEPFONT, x, y, 66, ys, 66 + 11, y2); break;// G
        
        case 17: Copy(Temp_LARGEPFONT, x, y, 88, ys, 88 + 11, y2); break; // #
        case 18: Copy(Temp_LARGEPFONT, x, y, 243, ys, 243 + 11, y2); break; // -
        case 19: Copy(Temp_LARGEPFONT, x, y, 210, ys, 210 + 34, y2); break; // Off
        case 20: Copy(Temp_LARGEPFONT, x, y, 77, ys, 77 + 11, y2); break; // Blank
        case 21: Copy(Temp_LARGEPFONT, x, y, 254, ys, 254 + 11, y2); break; // .

        case 23: Copy(Temp_PFONT, x, y, 56, 64, 56 + 26, 64 + 6); break; // ON
        case 24: Copy(Temp_PFONT, x, y, 84, 64, 84 + 26, 64 + 6); break; // OFF
        case 25: Copy(Temp_PFONT, x, y,  0, 64,  0 + 26, 64 + 6); break; // MUTE
        case 26: Copy(Temp_PFONT, x, y, 28, 64, 28 + 26, 64 + 6); break; // PLAY

        case 27: Copy(Temp_PFONT, x, y, 193, 64, 193 + 14, 64 + 6); break; // ZOOM ON
        case 28: Copy(Temp_PFONT, x, y, 221, 64, 221 + 14, 64 + 6); break; // ZOOM OFF

        case 29: Copy(Temp_PFONT, x, y, 56, ys, 59, y2); break; // Blank (4 pixels)
        case 30: Copy(Temp_PFONT, x, y, 56, ys, 57, y2); break; // Blank (2 pixels)

        case 31: Copy(Temp_PFONT, x, y, 111, 64, 111 + 4, 64 + 6); break; // FX ARROW LO BACK
        case 32: Copy(Temp_PFONT, x, y, 138, 64, 138 + 4, 64 + 6); break; // FX ARROW HI BACK
        case 33: Copy(Temp_PFONT, x, y, 165, 64, 165 + 4, 64 + 6); break; // FX ARROW SEL BACK

        case 35: Copy(Temp_PFONT, x, y, 249, 64, 249 + 6, 64 + 6); break; // REDUCE NOTES ON
        case 36: Copy(Temp_PFONT, x, y, 257, 64, 257 + 6, 64 + 6); break; // EXPAND NOTES ON
        case 37: Copy(Temp_PFONT, x, y, 277, 64, 277 + 6, 64 + 6); break; // REDUCE NOTES OFF
        case 38: Copy(Temp_PFONT, x, y, 285, 64, 285 + 6, 64 + 6); break; // EXPAND NOTES OFF
    }
}

// ------------------------------------------------------
void Note_Large_Letter(int x, int y, char ltr, int ys, int y2)
{
    switch(ltr)
    {
        case 0: Copy(Note_Surface, x, y, 100, ys, 100 + 11, y2); break;
        case 1: Copy(Note_Surface, x, y, 111, ys, 111 + 11, y2); break;
        case 2: Copy(Note_Surface, x, y, 122, ys, 122 + 11, y2); break;
        case 3: Copy(Note_Surface, x, y, 133, ys, 133 + 11, y2); break;
        case 4: Copy(Note_Surface, x, y, 144, ys, 144 + 11, y2); break;
        case 5: Copy(Note_Surface, x, y, 155, ys, 155 + 11, y2); break;
        case 6: Copy(Note_Surface, x, y, 166, ys, 166 + 11, y2); break;
        case 7: Copy(Note_Surface, x, y, 177, ys, 177 + 11, y2); break;
        case 8: Copy(Note_Surface, x, y, 188, ys, 188 + 11, y2); break;
        case 9: Copy(Note_Surface, x, y, 199, ys, 199 + 11, y2); break;

        case 10: Copy(Note_Surface, x, y, 0, ys, 11, y2); break; // A
        case 11: Copy(Note_Surface, x, y, 11, ys, 11 + 11, y2); break;// B
        case 12: Copy(Note_Surface, x, y, 22, ys, 22 + 11, y2); break;// C
        case 13: Copy(Note_Surface, x, y, 33, ys, 33 + 11, y2); break;// D
        case 14: Copy(Note_Surface, x, y, 44, ys, 44 + 11, y2); break;// E
        case 15: Copy(Note_Surface, x, y, 55, ys, 55 + 11, y2); break;// F
        case 16: Copy(Note_Surface, x, y, 66, ys, 66 + 11, y2); break;// G
        
        case 17:
        {
            if(Accidental)
            {
                Copy(Note_Surface, x, y, 269, ys, 269 + 11, y2); break; // b
            }
            else
            {
                Copy(Note_Surface, x, y, 88, ys, 88 + 11, y2); break; // #
            }
        }
        case 18: Copy(Note_Surface, x, y, 243, ys, 243 + 11, y2); break; // -
        case 19: Copy(Note_Surface, x, y, 210, ys, 210 + 34, y2); break; // Off
        case 20: Copy(Note_Surface, x, y, 77, ys, 77 + 11, y2); break; // Blank
        case 21: Copy(Note_Surface, x, y, 254, ys, 254 + 11, y2); break; // .

        case 23: Copy(Temp_PFONT, x, y, 56, 64, 56 + 26, 64 + 6); break; // ON
        case 24: Copy(Temp_PFONT, x, y, 84, 64, 84 + 26, 64 + 6); break; // OFF
        case 25: Copy(Temp_PFONT, x, y,  0, 64,  0 + 26, 64 + 6); break; // MUTE
        case 26: Copy(Temp_PFONT, x, y, 28, 64, 28 + 26, 64 + 6); break; // PLAY

        case 27: Copy(Temp_PFONT, x, y, 193, 64, 193 + 14, 64 + 6); break; // ZOOM ON
        case 28: Copy(Temp_PFONT, x, y, 221, 64, 221 + 14, 64 + 6); break; // ZOOM OFF

        case 29: Copy(Temp_PFONT, x, y, 56, ys, 59, y2); break; // Blank (4 pixels)
        case 30: Copy(Temp_PFONT, x, y, 56, ys, 57, y2); break; // Blank (2 pixels)

        case 31: Copy(Temp_PFONT, x, y, 111, 64, 111 + 4, 64 + 6); break; // FX ARROW LO BACK
        case 32: Copy(Temp_PFONT, x, y, 138, 64, 138 + 4, 64 + 6); break; // FX ARROW HI BACK
        case 33: Copy(Temp_PFONT, x, y, 165, 64, 165 + 4, 64 + 6); break; // FX ARROW SEL BACK

        case 35: Copy(Temp_PFONT, x, y, 249, 56, 249 + 6, 56 + 6); break; // REDUCE NOTES ON
        case 36: Copy(Temp_PFONT, x, y, 257, 64, 257 + 6, 64 + 6); break; // EXPAND NOTES ON
        case 37: Copy(Temp_PFONT, x, y, 277, 64, 277 + 6, 64 + 6); break; // REDUCE NOTES OFF
        case 38: Copy(Temp_PFONT, x, y, 285, 64, 285 + 6, 64 + 6); break; // EXPAND NOTES OFF
    }
}

// ------------------------------------------------------
void Small_Letter(int x, int y, char ltr, int ys, int y2)
{
    switch(ltr)
    {
        case 0: Copy(Temp_SMALLPFONT, x, y, 54, ys, 54 + 5, y2); break; // 0
        case 1: Copy(Temp_SMALLPFONT, x, y, 60, ys, 60 + 5, y2); break; // 1
        case 2: Copy(Temp_SMALLPFONT, x, y, 66, ys, 66 + 5, y2); break; // 2
        case 3: Copy(Temp_SMALLPFONT, x, y, 72, ys, 72 + 5, y2); break; // 3
        case 4: Copy(Temp_SMALLPFONT, x, y, 78, ys, 78 + 5, y2); break; // 4
        case 5: Copy(Temp_SMALLPFONT, x, y, 84, ys, 84 + 5, y2); break; // 5
        case 6: Copy(Temp_SMALLPFONT, x, y, 90, ys, 90 + 5, y2); break; // 6
        case 7: Copy(Temp_SMALLPFONT, x, y, 96, ys, 96 + 5, y2); break; // 7
        case 8: Copy(Temp_SMALLPFONT, x, y, 102, ys, 102 + 5, y2); break; // 8
        case 9: Copy(Temp_SMALLPFONT, x, y, 108, ys, 108 + 5, y2); break; // 9
        
        case 10: Copy(Temp_SMALLPFONT, x, y, 0, ys, 5, y2); break; // A
        case 11: Copy(Temp_SMALLPFONT, x, y, 6, ys, 6 + 5, y2); break;// B
        case 12: Copy(Temp_SMALLPFONT, x, y, 12, ys, 12 + 5, y2); break;// C
        case 13: Copy(Temp_SMALLPFONT, x, y, 18, ys, 18 + 5, y2); break;// D
        case 14: Copy(Temp_SMALLPFONT, x, y, 24, ys, 24 + 5, y2); break;// E
        case 15: Copy(Temp_SMALLPFONT, x, y, 30, ys, 30 + 5, y2); break;// F
        case 16: Copy(Temp_SMALLPFONT, x, y, 36, ys, 36 + 5, y2); break;// G
        
        case 17: Copy(Temp_SMALLPFONT, x, y, 48, ys, 48 + 5, y2); break; // #
        case 18: Copy(Temp_SMALLPFONT, x, y, 132, ys, 132 + 5, y2); break; // -
        case 19: Copy(Temp_SMALLPFONT, x, y, 114, ys, 114 + 17, y2); break; // Off
        case 20: Copy(Temp_SMALLPFONT, x, y, 42, ys, 42 + 5, y2); break; // Blank
        case 21: Copy(Temp_SMALLPFONT, x, y, 138, ys, 138 + 5, y2); break; // .

        case 23: Copy(Temp_PFONT, x, y, 56, 64, 56 + 26, 64 + 6); break; // ON
        case 24: Copy(Temp_PFONT, x, y, 84, 64, 84 + 26, 64 + 6); break; // OFF
        case 25: Copy(Temp_PFONT, x, y,  0, 64,  0 + 26, 64 + 6); break; // MUTE
        case 26: Copy(Temp_PFONT, x, y, 28, 64, 28 + 26, 64 + 6); break; // PLAY

        case 27: Copy(Temp_PFONT, x, y, 193, 64, 193 + 14, 64 + 6); break; // ZOOM ON
        case 28: Copy(Temp_PFONT, x, y, 221, 64, 221 + 14, 64 + 6); break; // ZOOM OFF

        case 29: Copy(Temp_PFONT, x, y, 56, ys, 59, y2); break; // Blank (4 pixels)
        case 30: Copy(Temp_PFONT, x, y, 56, ys, 57, y2); break; // Blank (2 pixels)

        case 31: Copy(Temp_PFONT, x, y, 111, 64, 111 + 4, 64 + 6); break; // FX ARROW LO BACK
        case 32: Copy(Temp_PFONT, x, y, 138, 64, 138 + 4, 64 + 6); break; // FX ARROW HI BACK
        case 33: Copy(Temp_PFONT, x, y, 165, 64, 165 + 4, 64 + 6); break; // FX ARROW SEL BACK

        case 35: Copy(Temp_PFONT, x, y, 249, 64, 249 + 6, 64 + 6); break; // REDUCE NOTES ON
        case 36: Copy(Temp_PFONT, x, y, 257, 64, 257 + 6, 64 + 6); break; // EXPAND NOTES ON
        case 37: Copy(Temp_PFONT, x, y, 277, 64, 277 + 6, 64 + 6); break; // REDUCE NOTES OFF
        case 38: Copy(Temp_PFONT, x, y, 285, 64, 285 + 6, 64 + 6); break; // EXPAND NOTES OFF
    }
}

// ------------------------------------------------------
void Note_Small_Letter(int x, int y, char ltr, int ys, int y2)
{
    switch(ltr)
    {
        case 0: Copy(Note_Surface, x, y, 54, ys, 54 + 5, y2); break; // 0
        case 1: Copy(Note_Surface, x, y, 60, ys, 60 + 5, y2); break; // 1
        case 2: Copy(Note_Surface, x, y, 66, ys, 66 + 5, y2); break; // 2
        case 3: Copy(Note_Surface, x, y, 72, ys, 72 + 5, y2); break; // 3
        case 4: Copy(Note_Surface, x, y, 78, ys, 78 + 5, y2); break; // 4
        case 5: Copy(Note_Surface, x, y, 84, ys, 84 + 5, y2); break; // 5
        case 6: Copy(Note_Surface, x, y, 90, ys, 90 + 5, y2); break; // 6
        case 7: Copy(Note_Surface, x, y, 96, ys, 96 + 5, y2); break; // 7
        case 8: Copy(Note_Surface, x, y, 102, ys, 102 + 5, y2); break; // 8
        case 9: Copy(Note_Surface, x, y, 108, ys, 108 + 5, y2); break; // 9
        
        case 10: Copy(Note_Surface, x, y, 0, ys, 5, y2); break; // A
        case 11: Copy(Note_Surface, x, y, 6, ys, 6 + 5, y2); break;// B
        case 12: Copy(Note_Surface, x, y, 12, ys, 12 + 5, y2); break;// C
        case 13: Copy(Note_Surface, x, y, 18, ys, 18 + 5, y2); break;// D
        case 14: Copy(Note_Surface, x, y, 24, ys, 24 + 5, y2); break;// E
        case 15: Copy(Note_Surface, x, y, 30, ys, 30 + 5, y2); break;// F
        case 16: Copy(Note_Surface, x, y, 36, ys, 36 + 5, y2); break;// G
        
        case 17: 
        {
            if(Accidental)
            {
                Copy(Note_Surface, x, y, 145, ys, 145 + 5, y2); break; // b
            }
            else
            {
                Copy(Note_Surface, x, y, 48, ys, 48 + 5, y2); break; // #
            }
        }
        case 18: Copy(Note_Surface, x, y, 132, ys, 132 + 5, y2); break; // -
        case 19: Copy(Note_Surface, x, y, 114, ys, 114 + 17, y2); break; // Off
        
        case 20: Copy(Note_Surface, x, y, 42, ys, 42 + 5, y2); break; // Blank
        case 21: Copy(Note_Surface, x, y, 138, ys, 138 + 5, y2); break; // .

        case 23: Copy(Temp_NOTEPFONT, x, y,  56, 64, 56 + 26, 64 + 6); break; // ON
        case 24: Copy(Temp_NOTEPFONT, x, y,  84, 64, 84 + 26, 64 + 6); break; // OFF
        case 25: Copy(Temp_NOTEPFONT, x, y,   0, 64,  0 + 26, 64 + 6); break; // MUTE
        case 26: Copy(Temp_NOTEPFONT, x, y,  28, 64, 28 + 26, 64 + 6); break; // PLAY

        case 27: Copy(Temp_NOTEPFONT, x, y,  193, 64, 193 + 14, 64 + 6); break; // ZOOM ON
        case 28: Copy(Temp_NOTEPFONT, x, y,  221, 64, 221 + 14, 64 + 6); break; // ZOOM OFF

        case 29: Copy(Temp_NOTEPFONT, x, y, 56, ys, 59, y2); break; // Blank (4 pixels)
        case 30: Copy(Temp_NOTEPFONT, x, y, 56, ys, 57, y2); break; // Blank (2 pixels)

        case 31: Copy(Temp_NOTEPFONT, x, y, 111, 64, 111 + 4, 64 + 6); break; // FX ARROW LO BACK
        case 32: Copy(Temp_NOTEPFONT, x, y, 138, 64, 138 + 4, 64 + 6); break; // FX ARROW HI BACK
        case 33: Copy(Temp_NOTEPFONT, x, y, 165, 64, 165 + 4, 64 + 6); break; // FX ARROW SEL BACK

        case 35: Copy(Temp_NOTEPFONT, x, y, 249, 64, 249 + 6, 64 + 6); break; // REDUCE NOTES ON
        case 36: Copy(Temp_NOTEPFONT, x, y, 257, 64, 257 + 6, 64 + 6); break; // EXPAND NOTES ON
        case 37: Copy(Temp_NOTEPFONT, x, y, 277, 64, 277 + 6, 64 + 6); break; // REDUCE NOTES OFF
        case 38: Copy(Temp_NOTEPFONT, x, y, 285, 64, 285 + 6, 64 + 6); break; // EXPAND NOTES OFF
    }
}

void blitnote(int x, int y, int note, int y1, int y2)
{
    Letter_Function = Note_Letter;
    Note_Surface = Temp_NOTEPFONT;
    Note_Alt_Surface = Temp_PFONT;
    if(Accidental) Blit_note(x, y, note, y1, y2, 8, Accidental_Table_b[note]);
    else Blit_note(x, y, note, y1, y2, 8, Accidental_Table[note]);
}

void blitlargenote(int x, int y, int note, int y1, int y2)
{
    Letter_Function = Note_Large_Letter;
    Note_Surface = Temp_NOTELARGEPFONT;
    Note_Alt_Surface = Temp_LARGEPFONT;
    if(Accidental) Blit_note(x, y, note, y1, y2, 11, Accidental_Table_b[note]);
    else Blit_note(x, y, note, y1, y2, 11, Accidental_Table[note]);
}

void blitsmallnote(int x, int y, int note, int y1, int y2)
{
    Letter_Function = Note_Small_Letter;
    Note_Alt_Surface = Temp_SMALLPFONT;
    Note_Surface = Temp_NOTESMALLPFONT;
    if(Accidental) Blit_note(x, y, note, y1, y2, 6, Accidental_Table_b[note]);
    else Blit_note(x, y, note, y1, y2, 6, Accidental_Table[note]);
}

void Blit_note(int x, int y, int note, int y1, int y2, int size, int acc)
{
    int sizex2 = size * 2;
    switch(note)
    {
        case 120:
            Letter_Function(x, y, 19, y1, y2);
            break;

        case 121:
            Note_Surface = Note_Alt_Surface;
            Letter_Function(x, y, 18, y1, y2);
            Letter_Function(x + size, y, 18, y1, y2);
            Letter_Function(x + sizex2, y, 18, y1, y2);
            break;

        case 0: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 1: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 2: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 3: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 4: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 5: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 6: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 7: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 8: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 9: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 10: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 11: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 0, y1, y2); break;
        case 12: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 13: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 14: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 15: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 16: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 17: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 18: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 19: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 20: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 21: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 22: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 23: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 1, y1, y2); break;
        case 24: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 25: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 26: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 27: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 28: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 29: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 30: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 31: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 32: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 33: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 34: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 35: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 2, y1, y2); break;
        case 36: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 37: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 38: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 39: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 40: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 41: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 42: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 43: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 44: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 45: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 46: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 47: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 3, y1, y2); break;
        case 48: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 49: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 50: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 51: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 52: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 53: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 54: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 55: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 56: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 57: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 58: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 59: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 4, y1, y2); break;
        case 60: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 61: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 62: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 63: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 64: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 65: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 66: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 67: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 68: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 69: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 70: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 71: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 5, y1, y2); break;
        case 72: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 73: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 74: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 75: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 76: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 77: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 78: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 79: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 80: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 81: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 82: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 83: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 6, y1, y2); break;
        case 84: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 85: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 86: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 87: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 88: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 89: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 90: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 91: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 92: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 93: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 94: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 95: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 7, y1, y2); break;
        case 96: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 97: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 98: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 99: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 100: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 101: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 102: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 103: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 104: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 105: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 106: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 107: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 8, y1, y2); break;
        case 108: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 109: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 110: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 111: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 112: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 113: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 114: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 115: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 116: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 117: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 118: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 17, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
        case 119: Letter_Function(x, y, acc, y1, y2); Letter_Function(x + size, y, 18, y1, y2); Letter_Function(x + sizex2, y, 9, y1, y2); break;
    }
}

// ------------------------------------------------------
// Display a digit
void out_nibble(int x, int y, int Font_Type, int number)
{
    switch(number)
    {
        case 0: PrintString(x, y, Font_Type, "0"); break;
        case 1: PrintString(x, y, Font_Type, "1"); break;
        case 2: PrintString(x, y, Font_Type, "2"); break;
        case 3: PrintString(x, y, Font_Type, "3"); break;
        case 4: PrintString(x, y, Font_Type, "4"); break;
        case 5: PrintString(x, y, Font_Type, "5"); break;
        case 6: PrintString(x, y, Font_Type, "6"); break;
        case 7: PrintString(x, y, Font_Type, "7"); break;
        case 8: PrintString(x, y, Font_Type, "8"); break;
        case 9: PrintString(x, y, Font_Type, "9"); break;
        case 10: PrintString(x, y, Font_Type, "A"); break;
        case 11: PrintString(x, y, Font_Type, "B"); break;
        case 12: PrintString(x, y, Font_Type, "C"); break;
        case 13: PrintString(x, y, Font_Type, "D"); break;
        case 14: PrintString(x, y, Font_Type, "E"); break;
        case 15: PrintString(x, y, Font_Type, "F"); break;
    }
}

// -----------------------------------------------------
// Return the ordinal position of a given letter
int Get_Char_Position(char *Ascii_Letters, int Max_Letters, char Letter)
{
    int i;

    for(i = 0; i < Max_Letters; i++) {
        if(Ascii_Letters[i] == Letter) return(i);
    }
    return(Max_Letters - 1);
}

// ------------------------------------------------------
// Get the size of a text in pixels
int Get_Size_Text(char *String)
{
    int Idx;
    int len;
    int i;

    len = 0;
    for(i = 0; String[i]; i++)
    {
        Idx = Get_Char_Position(Font_Ascii, Nbr_Letters, String[i]);
        len += Font_Size[Idx];
    }
    return(len - 1);
}

// ------------------------------------------------------
// Return the height of the font
int Get_Font_Height(void)
{
    return(Font_Height);
}

// ------------------------------------------------------
// Load a .bmp picture into a SDL surface
SDL_Surface *Load_Picture(char *FileName)
{
    return(SDL_LoadBMP(FileName));
}

// ------------------------------------------------------
// Create the font datas
int Create_Font_Datas(char *FontName)
{
    FILE *hFont;
    int Data;
    int i;
    int Pos;

    Nbr_Letters = strlen(Font_Ascii);

    hFont = fopen(FontName, "r");
    if(!hFont) return(FALSE);
    Pos = 0;
    for(i = 0; i < 256; i++)
    {
        Font_Pos[i] = Pos;
        fscanf(hFont, "%d", &Data);
        Font_Size[i] = Data;
        Pos += Data;
    }
    fclose(hFont);
    return(TRUE);
}

void Set_Channel_State_Pic(int x, int color, int inv_color)
{
    int Surface_offset;
    int i;
    int j;
    unsigned char *Pix;

    while(SDL_LockSurface(Temp_PFONT) < 0);

    Pix = (unsigned char *) Temp_PFONT->pixels;

    Pix += (64 * Temp_PFONT->pitch) + x;
    for(j = 0; j < 7; j++)
    {
        for(i = 0; i < 27; i++)
        {
            Surface_offset = (j * Temp_PFONT->pitch) + i;
            if(Pix[Surface_offset])
            {
                Pix[Surface_offset] = color;
            }
            else
            {
                Pix[Surface_offset] = inv_color;
            }
        }
    }

    SDL_UnlockSurface(Temp_PFONT);
}

// ------------------------------------------------------
// Create the complete font set to display the patterns
void Create_Pattern_font(SDL_Surface *Dest, int offset,
                         int Lo_Fore,
                         int Sel_Fore,
                         int Hi_Fore)
{
    unsigned char *Pix;
    unsigned char *Pix3;
    unsigned char *Pix2;
    int Surface_offset;
    int Surface_offset_Dest;
    int i;
    int j;

    // Create the pattern font
    Copy_To_Surface(PFONT, Dest, 0, 0, 0, offset, 316, offset + 8);

    // Set the base colors
    while(SDL_LockSurface(Dest) < 0);

    Pix = (unsigned char *) Dest->pixels;
    Pix3 = Pix + (87 * Dest->pitch);

    for(j = 0; j < 8; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            if(Pix[Surface_offset])
            {
                Pix[Surface_offset] = Lo_Fore;
                Pix3[Surface_offset] = Lo_Fore + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
            }
            else
            {
                Pix[Surface_offset] = COL_PATTERN_LO_BACK;
                Pix3[Surface_offset] = COL_PATTERN_LO_BACK + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
            }
        }
    }

    // Invert
    Pix2 = Pix;
    Pix2 += (8 * Dest->pitch);
    Pix3 = Pix2 + (87 * Dest->pitch);
    for(j = 0; j < 8; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            if(Pix[Surface_offset] == Lo_Fore)
            {
                Pix2[Surface_offset] = COL_PATTERN_LO_BACK;
                Pix3[Surface_offset] = COL_PATTERN_LO_BACK + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
            }
            else
            {
                Pix2[Surface_offset] = Lo_Fore;
                Pix3[Surface_offset] = Lo_Fore + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
            }
        }
    }

    SDL_UnlockSurface(Dest);

    // Blank line
    Copy_To_Surface(PFONT, Dest, 0, 16, 0, 7, 316, 7 + 1);

    while(SDL_LockSurface(Dest) < 0);

    Pix2 = Pix;
    Pix2 += (16 * Dest->pitch);

    // Big one
    for(j = 0; j < 8; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            Surface_offset_Dest = ((j * 2) * Dest->pitch) + i;
            Pix2[Surface_offset_Dest] = Pix[Surface_offset] + 4;
            Pix2[Surface_offset_Dest + Dest->pitch] = Pix2[Surface_offset_Dest];
        }
    }

    // Big one inverted
    Pix2 = (unsigned char *) Dest->pixels;
    Pix2 += (32 * Dest->pitch);
    Pix = (unsigned char *) Dest->pixels;
    Pix += (16 * Dest->pitch);
    for(j = 0; j < 16; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            if(Pix[Surface_offset] == COL_PATTERN_SEL_BACK)
            {
                Pix2[Surface_offset] = Sel_Fore;
            }
            else
            {
                Pix2[Surface_offset] = COL_PATTERN_SEL_BACK;
            }
        }
    }

    // Big one under the caret
    Pix2 = (unsigned char *) Dest->pixels;
    Pix2 += (48 * Dest->pitch);
    Pix = (unsigned char *) Dest->pixels;
    Pix += (16 * Dest->pitch);
    for(j = 0; j < 16; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            if(Pix[Surface_offset] == COL_PATTERN_SEL_BACK)
            {
                Pix2[Surface_offset] = COL_VUMETERPEAK;
            }
            else
            {
                Pix2[Surface_offset] = COL_PATTERN_LO_BACK;
            }
        }
    }

    // Highlight inverted
    Pix = (unsigned char *) Dest->pixels;
    Pix2 = Pix;
    Pix2 += (71 * Dest->pitch);
    Pix3 = Pix2 + (87 * Dest->pitch);

    for(j = 0; j < 8; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            Pix2[Surface_offset] = Pix[Surface_offset] + 2;
            Pix3[Surface_offset] = Pix[Surface_offset] + 2 + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
        }
    }

    // (invert the previous one)
    Pix = (unsigned char *) Dest->pixels;
    Pix += (71 * Dest->pitch);
    Pix2 = Pix + (8 * Dest->pitch);
    Pix3 = Pix2 + (87 * Dest->pitch);
    for(j = 0; j < 8; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            if(Pix[Surface_offset] == Hi_Fore)
            {
                Pix2[Surface_offset] = COL_PATTERN_HI_BACK;
                Pix3[Surface_offset] = COL_PATTERN_HI_BACK + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
            }
            else
            {
                Pix2[Surface_offset] = Hi_Fore;
                Pix3[Surface_offset] = Hi_Fore + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
            }
        }
    }

    // Highlight
    Pix = (unsigned char *) Dest->pixels;
    Pix2 = Pix;
    Pix2 += (71 * Dest->pitch);
    Pix3 = Pix2 + (87 * Dest->pitch);

    for(j = 0; j < 8; j++)
    {
        for(i = 0; i < Dest->w; i++)
        {
            Surface_offset = (j * Dest->pitch) + i;
            Pix2[Surface_offset] = Pix[Surface_offset] + 2;
            Pix3[Surface_offset] = Pix[Surface_offset] + 2 + (COL_PATTERN_LO_BACK_SHADOW - COL_PATTERN_LO_BACK);
        }
    }

    SDL_UnlockSurface(Dest);

    // Markers arrows
    Copy_To_Surface(PFONT, Dest, 0, 64, 0, 8, 316, 8 + 7);
}

// ------------------------------------------------------
// Set the main palette with a default one
void Restore_Default_Palette(SDL_Color *Def, int DefBevel)
{
    int i;
    for(i = 0; i < sizeof(Default_Palette2) / sizeof(SDL_Color); i++)
    {
        Ptk_Palette[i].r = Def[i].r;
        Ptk_Palette[i].g = Def[i].g;
        Ptk_Palette[i].b = Def[i].b;
        Ptk_Palette[i].unused = Def[i].unused;
    }
    Beveled = DefBevel;
}

// ------------------------------------------------------
// Remap the pictures colors for our usage
int Set_Pictures_Colors(void)
{
    int i;
    unsigned char *Pix;

    SDL_Palette *Pic_Palette;
    int min_idx = sizeof(Default_Palette2) / sizeof(SDL_Color);

    Pix = (unsigned char *) FONT->pixels;
    for(i = 0; i < FONT->w * FONT->h; i++)
    {
        if(Pix[i]) Pix[i] = COL_FONT_HI;
    }

    Pix = (unsigned char *) FONT_LOW->pixels;
    for(i = 0; i < FONT_LOW->w * FONT_LOW->h; i++)
    {
        if(Pix[i]) Pix[i] = COL_FONT_LO;
    }
   
    bare_color_idx = min_idx;

    Pix = (unsigned char *) SKIN303->pixels;
    max_colors_303 = 0;
    for(i = 0; i < SKIN303->w * SKIN303->h; i++)
    {
        if(Pix[i] > max_colors_303) max_colors_303 = Pix[i];
        Pix[i] += min_idx;
    }
    max_colors_303++;

    Pix = (unsigned char *) LOGOPIC->pixels;
    max_colors_logo = 0;
    for(i = 0; i < LOGOPIC->w * LOGOPIC->h; i++)
    {
        if(Pix[i] > max_colors_logo) max_colors_logo = Pix[i];
        Pix[i] += min_idx;
    }
    max_colors_logo++;

    Pic_Palette = SKIN303->format->palette;
    for(i = 0; i < max_colors_303; i++)
    {
        Palette_303[i].r = Pic_Palette->colors[i].r;
        Palette_303[i].g = Pic_Palette->colors[i].g;
        Palette_303[i].b = Pic_Palette->colors[i].b;
        Palette_303[i].unused = Pic_Palette->colors[i].unused;
    }

    Pic_Palette = POINTER->format->palette;
    for(i = 0; i < max_colors_303; i++)
    {
        Palette_Pointer[i].r = Pic_Palette->colors[i].r;
        Palette_Pointer[i].g = Pic_Palette->colors[i].g;
        Palette_Pointer[i].b = Pic_Palette->colors[i].b;
        Palette_Pointer[i].unused = Pic_Palette->colors[i].unused;
    }

    Pic_Palette = LOGOPIC->format->palette;
    for(i = 0; i < max_colors_logo; i++)
    {
        Palette_Logo[i].r = Pic_Palette->colors[i].r;
        Palette_Logo[i].g = Pic_Palette->colors[i].g;
        Palette_Logo[i].b = Pic_Palette->colors[i].b;
        Palette_Logo[i].unused = Pic_Palette->colors[i].unused;
    }

    // Remap the colors of the pointer
    Pix = (unsigned char *) POINTER->pixels;
    max_colors_Pointer = 0;
    for(i = 0; i < POINTER->w * POINTER->h; i++)
    {
        if(Pix[i] > max_colors_Pointer) max_colors_Pointer = Pix[i];
        if(Pix[i]) Pix[i] += min_idx + max_colors_303;
    }
    max_colors_Pointer++;

    Temp_PFONT = SDL_AllocSurface(SDL_SWSURFACE, 316, 87 * 2, 8, 0, 0, 0, 0xff);
    Temp_LARGEPFONT = SDL_AllocSurface(SDL_SWSURFACE, 316, 87 * 2, 8, 0, 0, 0, 0xff);
    Temp_SMALLPFONT = SDL_AllocSurface(SDL_SWSURFACE, 316, 87 * 2, 8, 0, 0, 0, 0xff);
    Temp_NOTEPFONT = SDL_AllocSurface(SDL_SWSURFACE, 316, 87 * 2, 8, 0, 0, 0, 0xff);
    Temp_NOTELARGEPFONT = SDL_AllocSurface(SDL_SWSURFACE, 316, 87 * 2, 8, 0, 0, 0, 0xff);
    Temp_NOTESMALLPFONT = SDL_AllocSurface(SDL_SWSURFACE, 316, 87 * 2, 8, 0, 0, 0, 0xff);

    Pointer_BackBuf = (unsigned char *) malloc(POINTER->pitch * POINTER->h * sizeof(unsigned char));
    memset(Pointer_BackBuf, 0, POINTER->pitch * POINTER->h * sizeof(unsigned char));

    Set_Logo_Palette();
    Get_Phony_Palette();
    Set_Phony_Palette();
    Ptk_Palette[0].r = 0;
    Ptk_Palette[0].g = 0;
    Ptk_Palette[0].b = 0;
    Init_UI();

    Create_Pattern_font(Temp_PFONT, 0, COL_PATTERN_LO_FORE, COL_PATTERN_SEL_FORE, COL_PATTERN_HI_FORE);
    Create_Pattern_font(Temp_LARGEPFONT, 15, COL_PATTERN_LO_FORE, COL_PATTERN_SEL_FORE, COL_PATTERN_HI_FORE);
    Create_Pattern_font(Temp_SMALLPFONT, 23, COL_PATTERN_LO_FORE, COL_PATTERN_SEL_FORE, COL_PATTERN_HI_FORE);
    Create_Pattern_font(Temp_NOTEPFONT, 0, COL_NOTE_LO_FORE, COL_NOTE_SEL_FORE, COL_NOTE_HI_FORE);
    Create_Pattern_font(Temp_NOTELARGEPFONT, 15, COL_NOTE_LO_FORE, COL_NOTE_SEL_FORE, COL_NOTE_HI_FORE);
    Create_Pattern_font(Temp_NOTESMALLPFONT, 23, COL_NOTE_LO_FORE, COL_NOTE_SEL_FORE, COL_NOTE_HI_FORE);

    // Create the channels status
    Set_Channel_State_Pic(0, COL_MUTE, COL_MUTE_PLAY_INVERT);
    Set_Channel_State_Pic(28, COL_PLAY, COL_MUTE_PLAY_INVERT);
    Set_Channel_State_Pic(56, COL_PLAY, COL_MUTE_PLAY_INVERT);
    Set_Channel_State_Pic(84, COL_MUTE, COL_MUTE_PLAY_INVERT);
    Set_Channel_State_Pic(111, COL_MUTE, COL_PATTERN_LO_BACK);
    Set_Channel_State_Pic(138, COL_MUTE, COL_PATTERN_HI_BACK);
    Set_Channel_State_Pic(165, COL_MUTE, COL_PATTERN_SEL_BACK);
    Set_Channel_State_Pic(193, COL_PLAY, COL_MUTE_PLAY_INVERT);
    Set_Channel_State_Pic(221, COL_MUTE, COL_MUTE_PLAY_INVERT);
    Set_Channel_State_Pic(249, COL_PLAY, COL_MUTE_PLAY_INVERT);
    Set_Channel_State_Pic(277, COL_MUTE, COL_MUTE_PLAY_INVERT);

    return(TRUE);
}

void Set_Main_Palette(void)
{
    int i;

Wait_Palette:
    if(SKIN303 == NULL) goto Wait_Palette;

    for(i = 0; i < max_colors_303; i++)
    {
        Ptk_Palette[i + bare_color_idx].r = Palette_303[i].r;
        Ptk_Palette[i + bare_color_idx].g = Palette_303[i].g;
        Ptk_Palette[i + bare_color_idx].b = Palette_303[i].b;
        Ptk_Palette[i + bare_color_idx].unused = Palette_303[i].unused;
    }

    for(i = 0; i < max_colors_Pointer; i++)
    {
        Ptk_Palette[i + bare_color_idx + max_colors_303].r = Palette_Pointer[i].r;
        Ptk_Palette[i + bare_color_idx + max_colors_303].g = Palette_Pointer[i].g;
        Ptk_Palette[i + bare_color_idx + max_colors_303].b = Palette_Pointer[i].b;
        Ptk_Palette[i + bare_color_idx + max_colors_303].unused = Palette_Pointer[i].unused;
    }
}

void Set_Logo_Palette(void)
{
    int i;

    for(i = 0; i < max_colors_logo; i++)
    {
        Ptk_Palette[i + bare_color_idx].r = Palette_Logo[i].r;
        Ptk_Palette[i + bare_color_idx].g = Palette_Logo[i].g;
        Ptk_Palette[i + bare_color_idx].b = Palette_Logo[i].b;
        Ptk_Palette[i + bare_color_idx].unused = Palette_Logo[i].unused;
    }
}

// ------------------------------------------------------
// Set the screen palette
void Refresh_Palette(void)
{
    UISetPalette(Ptk_Palette, 256);
}

// ------------------------------------------------------
// Init the user interface
void Init_UI(void)
{
    Refresh_Palette();
}

// ------------------------------------------------------
// Free the allocated resources
void Destroy_UI(void)
{
    if(Pointer_BackBuf) free(Pointer_BackBuf);
}
