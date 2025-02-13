// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2025 Franck Charlet.
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
#include "include/ptk.h"

// ------------------------------------------------------
// Constants
// This is a nasty hack: we should have a specific ASCII buffer
// instead of using the unicode one.
#if !defined(__MACOSX_PPC__) && !defined(__MACOSX_X86__)
#define UNICODE_OFFSET1 0x20
#define UNICODE_OFFSET2 0x60
#else
#define UNICODE_OFFSET1 0
#define UNICODE_OFFSET2 0
#endif

// ------------------------------------------------------
// Variables
#if defined(__MACOSX_PPC__)
SystemSoundActionID WavActionID;
#endif

extern SDL_sem *thread_sema;

extern char SplashScreen;

extern int metronome_rows_counter;
extern int metronome_magnify;

extern int Song_Playing_Pattern;

extern short *Player_WL[MAX_TRACKS][MAX_POLYPHONY];
extern short *Player_WR[MAX_TRACKS][MAX_POLYPHONY];

extern VISUAL_DELAY_DAT Delays_Pos_Sound_Buffer[512];
extern int Delay_Sound_Buffer;
extern int Cur_Delay_Sound_Buffer;
extern int Subicounter;
extern int pattern_double;
extern int pattern_sliders;
extern int pattern_sliders_numbers;
extern int RShift_Notification;
extern int RShift_Notified;
int Enter_Notification;
int Enter_Notified;
int Enter_Notified_Play_Pattern;
SDL_sem *thread_sema;

#if defined(PTK_SHUFFLE)
extern int shufflestep;
extern int shuffleswitch;
#endif

extern char Use_Cubic;
extern char Paste_Across;

extern int32 sed_real_range_start;
extern int32 sed_real_range_end;

unsigned char sl3 = 0;

extern int pos_scope;
extern int pos_scope_latency;

int can_modify_song = 0;

int file_loaded = FALSE;
int Burned_Title = FALSE;
extern int global_argc;
extern char global_argv[MAX_PATH];
extern char AutoReload;
extern char Last_Used_Ptk[MAX_PATH];

#if defined(__USE_OPENGL__)
extern unsigned int *RGBTexture;
#endif

#if defined(__AMIGAOS4__)
extern struct Library *GfxBase;
extern struct GraphicsIFace *IGraphics;
#endif

extern float sp_Tvol_Mod[MAX_TRACKS];

int CONSOLE_WIDTH;
int CHANNELS_WIDTH;
int TRACKS_WIDTH;
int CONSOLE_HEIGHT;
int CONSOLE_HEIGHT2;
int fluzy = -1;
int Scopish = SCOPE_ZONE_MOD_DIR;
char Scopish_LeftRight = FALSE;
char Visible_Columns = 0;
int rs_coef = 32768;

int gui_lx = -1;
int gui_ly = -1;
int gui_lxs = -1;
int gui_lys = -1;
int gui_pushed = 0;
char teac = 0;
int liveparam = 0;
int livevalue = 0;
char trkchan = TRUE;
int pos_space = 0;
int multifactor = 4;
char seditor = 0;
int Done_Tip = FALSE;
char tipoftheday[256];
char Current_Instrument_Split = 0;

int player_pos = -1;
int xew = 0;
char sas = FALSE;
int flagger = 0;
int ltretnote = 0;
int ltretnote_raw = 0;
int ltretvalue = 0;
int retletter[256];
int tretletter = 0;
int posletter = 0;
int last_index = -1;
int gui_action = GUI_CMD_NOP;
int gui_action_external = 0;
int gui_action_external_303 = 0;
int gui_action_metronome = GUI_CMD_NOP;
int Column_Under_Caret = 0;
int Track_Under_Caret = 0;
int gui_track = 0;
int xoffseted;
float gr = 0;
float synthsignal = 0;

char userscreen = USER_SCREEN_DISKIO_EDIT;
int c_r_release = 0;
int c_l_release = 0;

char name[MAX_PATH];
char synthname[MAX_PATH];
char instrname[MAX_PATH];
char name303[MAX_PATH];
char namerev[MAX_PATH];
char namemidicfg[MAX_PATH];
char artist[20];
char style[20];

int Current_Instrument = 0;
int restx = 0;
int resty = 0;
int fsize = 0;
char draw_sampled_wave = FALSE;
char draw_sampled_wave2 = FALSE;
char draw_sampled_wave3 = FALSE;

int redraw_everything = FALSE;
char boing = FALSE;
int LastPedRow = -1;
char po_ctrl2 = TRUE;
char po_alt = TRUE;
char po_alt2 = TRUE;
int Chan_Midi_Prg[MAX_TRACKS];

int sp_TickCounter = 0;
int LVColor = 0;
int RVColor = 0;
int player_line = 0;
char actuloop = 0;
int namesize = 8;

int c_l_tvol = 32768;
int c_r_tvol = 32768;
int c_l_cvol = 32768;
int c_r_cvol = 32768;

int snamesel = INPUT_NONE;

SDL_Surface *PFONT;
SDL_Surface *PFONT_DOUBLE;
SDL_Surface *FONT;
SDL_Surface *FONT_LOW;

SDL_TimerID Timer;
Uint32 Timer_CallBack(Uint32 interval, void *param);
Uint32 (*Timer_Ptr)(Uint32 interval, void *param) = &Timer_CallBack;
extern int Status_Box_Wait;
extern int Status_Box_Wait_Done;

extern s_access sp_Position[MAX_TRACKS][MAX_POLYPHONY];

extern int done;
extern float local_curr_mas_vol;

int MouseWheel_Multiplier = 1;
char Rows_Decimal = FALSE;
char See_Prev_Next_Pattern = FALSE;

char cur_input_name[1024];

SDL_Surface *LOGOPIC;
int wait_title;
int display_title = 0;

int key_record_first_time;
double key_ticks;
int old_key_Pattern_Line;
Uint32 Alloc_midi_Channels[MAX_TRACKS][MAX_POLYPHONY];
int Record_Keys[37];
int Record_Keys_State[37];

void Mouse_Sliders_Master_Shuffle(void);
int Calc_Length(void);

extern int Ticks_Synchro_Left;
extern int Ticks_Synchro_Right;
extern int Continuous_Scroll;

void Draw_Scope(void);
void Draw_Scope_Files_Button(void);
void Display_Tracks_To_Render(void);
void Mute_Track(int track_to_solo);
void Display_RShift_Status(void);

JAZZ_KEY Sub_Channels_Jazz[MAX_TRACKS][MAX_POLYPHONY];

char Jazz_Edit = FALSE;

int wait_AutoSave;
extern char AutoSave;
int Values_AutoSave[] =
{
    0,
    1 * 60,
    2 * 60,
    4 * 60,
    8 * 60,
    10 * 60,
    15 * 60,
    30 * 60
};

char Prog_Path[MAX_PATH];

int Table_Right_Tab_Notes[] =
{
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
    3, 2, 1,
};

int Table_Left_Tab_Notes[] =
{
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
    3, 1, 2,
};

int gui_thread_action = FALSE;
int gui_thread_can_act = FALSE;
int gui_bpm_action = FALSE;

int Keyboard_Events_Channels[256];
JAZZ_KEY Sub_Channels_Note_Off[MAX_TRACKS][MAX_POLYPHONY];
int Nbr_Sub_Note_Off;

REQUESTER_BUTTON Requester_Btn_Cancel =
{
    NULL,
    "Cancel",
    BUTTON_CANCEL,
    SDLK_c
};

REQUESTER_BUTTON Requester_Btn_303 =
{
    &Requester_Btn_Cancel,
    "303 Units",
    0,
    SDLK_u
};

REQUESTER_BUTTON Requester_Btn_Synths =
{
    &Requester_Btn_303,
    "Synths",
    0,
    SDLK_s
};

REQUESTER_BUTTON Requester_Btn_Instruments =
{
    &Requester_Btn_Synths,
    "Instruments",
    0,
    SDLK_i
};

REQUESTER_BUTTON Requester_Btn_Patterns =
{
    &Requester_Btn_Instruments,
    "Patterns",
    0,
    SDLK_p
};

REQUESTER_BUTTON Requester_Btn_All =
{
    &Requester_Btn_Patterns,
    "All",
    BUTTON_DEFAULT,
    SDLK_a
};

REQUESTER_BUTTON Requester_Btn_No =
{
    NULL,
    "No",
    BUTTON_CANCEL,
    SDLK_n
};

REQUESTER_BUTTON Requester_Btn_Yes =
{
    &Requester_Btn_No,
    "Yes",
    BUTTON_DEFAULT,
    SDLK_y
};

REQUESTER_BUTTON Requester_Btn_Synth =
{
    &Requester_Btn_Cancel,
    "Synth",
    0,
    SDLK_s
};

REQUESTER_BUTTON Requester_Btn_Split =
{
    &Requester_Btn_Synth,
    "Split",
    0,
    SDLK_p
};

REQUESTER_BUTTON Requester_Btn_Instrument =
{
    &Requester_Btn_Split,
    "Instrument",
    0,
    SDLK_i
};

REQUESTER_BUTTON Requester_Btn_All_Delete =
{
    &Requester_Btn_Instrument,
    "All",
    BUTTON_DEFAULT,
    SDLK_a
};

REQUESTER_BUTTON Requester_Btn_Ok =
{
    NULL,
    "Ok",
    BUTTON_DEFAULT,
    SDLK_ESCAPE
};

extern REQUESTER Title_Requester;

REQUESTER Exit_Requester =
{
    "Do you really want to quit ?",
    &Requester_Btn_Yes,
    NULL, 0
};

REQUESTER Overwrite_Requester =
{
    "",
    &Requester_Btn_Yes,
    NULL, 0
};

REQUESTER Zzaapp_Requester =
{
    "What do you want to Zzaapp ?",
    &Requester_Btn_All,
    NULL, 0
};

REQUESTER Delete_Requester =
{
    "What do you want to delete ?",
    &Requester_Btn_All_Delete,
    NULL, 0
};

REQUESTER Helper_Requester =
{
    NULL,
    &Requester_Btn_Ok,
    NULL, 0
};
char Chords_Major[64];
char Chords_Minor[64];
char Chords_Sus4[64];
char Chords_Sus2[64];
char Chords_Dim[64];
char Chords_Aug[64];
char Chords_Maj7[64];
char Chords_Min7[64];
char Chords_Add2[64];
char Chords[512];

char OverWrite_Name[1024];

int ZzaappOMatic;

// ------------------------------------------------------
// Load a skin picture
SDL_Surface *Load_Skin_Picture(char *name)
{
    char filepath[MAX_PATH];
    char error[256];
    SDL_Surface *surface;

#if defined(__WIN32__)
    sprintf(filepath, "skins\\%s", name);
#else
    sprintf(filepath, "skins/%s", name);
#endif

    surface = Load_Picture(filepath);
    if(surface == NULL)
    {
        sprintf(error, "Can't load %s file.", name);
        Message_Error(error);
        return(NULL);
    }
    return(surface);
}

// ------------------------------------------------------
// Load the font letters sizes according
int Load_Font_Data(char *name)
{
    char filepath[MAX_PATH];
    char error[256];

#if defined(__WIN32__)
    sprintf(filepath, "skins\\%s", name);
#else
    sprintf(filepath, "skins/%s", name);
#endif
    if(!Create_Font_Data(filepath))
    {
        sprintf(error, "Can't load %s file.", name);
        Message_Error(error);
        return(FALSE);
    }
    return(TRUE);
}

// ------------------------------------------------------
// Load the necessary data and initialize the interface
int Init_Context(void)
{
    srand(time(0));

    switch(rand() % 13)
    {
        case 0: sprintf(tipoftheday, "Tip Of The Hour: Pressing CTRL+I Will Interpolate Effect Value On A Marked Block."); break;
        case 1: sprintf(tipoftheday, "Tip Of The Hour: The Right Mouse Button Will Have A Secondary Action On Most Buttons."); break;
        case 2: sprintf(tipoftheday, "Tip Of The Hour: Don't Set Excessive Track Reverb Send Values, To Get Better Quality."); break;
        case 3: sprintf(tipoftheday, "Tip Of The Hour: When Recording, Number Of Positions Will Grow Automatically When Necessary."); break;
        case 4: sprintf(tipoftheday, "Tip Of The Hour: Remember: MIDI Is Not Audio, Realtime Fx Will Not Affect Midi Sound."); break;
        case 5: sprintf(tipoftheday, "Tip Of The Hour: On Lower CPUs, You Can Renderize Patterns To Wav, And Use Them As Samples Without Any Loss Of Quality."); break;
        case 6: sprintf(tipoftheday, "Tip Of The Hour: Volume Note-Cut Ccommand 'Fx' Is Very Useful To Avoid Sample-clicking."); break;
        case 7: sprintf(tipoftheday, "Tip Of The Hour: Left-Clicking On Pattern Editor Channels Numbers Will Mute/Unmute Any Track (Right Clicking Will Solo It)."); break;
        case 8: sprintf(tipoftheday, "Tip Of The Hour: Pattern Command '16xx' Will Reset The Filter LFO Of The Track. No Parameter Required."); break;
        case 9: sprintf(tipoftheday, "Tip Of The Hour: Use '90' Command In The Panning Column To Change Midi Controllers Values."); break;
        case 10: sprintf(tipoftheday, "Tip Of The Hour: Increase Latency Time If Sound Is Distorted."); break;
        case 11: sprintf(tipoftheday, "Tip Of The Hour: Pressing Right Mouse Button On Most Arrows Buttons (\03\04) Will Speed Operation Up."); break;
        default: sprintf(tipoftheday, "Tip Of The Hour: See 'manual.pdf' For More Infos About Help And Pattern Commands."); break;
    }

    L_MaxLevel = 0;
    R_MaxLevel = 0;

    sprintf(Selection_Name, "Untitled");
    sprintf(Reverb_Name, "Untitled");
    sprintf(Midi_Name, "Untitled");
    sprintf(name, "Untitled");
    sprintf(artist, "Somebody");
    sprintf(style, "Anything goes");

    thread_sema = SDL_CreateSemaphore(1);

    namesize = 8;
    Init_Synth_Params_Names();

    Clear_Files_List();

#if !defined(__NO_MIDI__)
    midiin_changed = 2;
    midiout_changed = 2;

    Midi_InitIn();
    Midi_InitOut();
#endif

    GETCWD(Prog_Path, MAX_PATH);

    Read_SMPT();

    CHDIR(Prog_Path);

    if(!Init_Block_Work())
    {
        return(FALSE);
    }

#if !defined(__NO_MIDI__)
    Midi_Reset();
#endif

    if(!Alloc_Patterns_Pool())
    {
        Message_Error("Can't allocate room for patterns.");
        return(FALSE);
    }

    LOGOPIC = Load_Skin_Picture("neural.bmp");
    if(!LOGOPIC) return(FALSE);
    SKIN303 = Load_Skin_Picture("303.bmp");
    if(!SKIN303) return(FALSE);
    PFONT = Load_Skin_Picture("pattern_font.bmp");
    if(!PFONT) return(FALSE);
    PFONT_DOUBLE = Load_Skin_Picture("pattern_font_double.bmp");
    if(!PFONT_DOUBLE) return(FALSE);
    FONT = Load_Skin_Picture("font.bmp");
    if(!FONT) return(FALSE);
    FONT_LOW = Load_Skin_Picture("font.bmp");
    if(!FONT_LOW) return(FALSE);

    if(!Load_Font_Data("font_data.txt"))
    {
        return(FALSE);
    }

    SDL_SetColorKey(FONT, SDL_SRCCOLORKEY, 0);
    SDL_SetColorKey(FONT_LOW, SDL_SRCCOLORKEY, 0);

    // Player initialization
#if defined(__WIN32__)
    if(!Ptk_InitDriver(Main_Window, AUDIO_Milliseconds))
#else
    if(!Ptk_InitDriver(AUDIO_Milliseconds))
#endif
    {
        return(FALSE);
    }

    Set_Default_Channels_Polyphony();
    Init_Sample_Bank();
    Pre_Song_Init();
    Post_Song_Init();

    // Old preset by default
    Load_Old_Reverb_Presets(0);
    Initreverb();

    Timer = SDL_AddTimer(1000, Timer_Ptr, NULL);

    return(TRUE);
}

// ------------------------------------------------------
// Release the allocated resources
extern int volatile AUDIO_Acknowledge;
void Destroy_Context(void)
{
    int i;

    if(Timer)
    {
        while(SDL_RemoveTimer(Timer) == FALSE)
        {
        };
    }
    AUDIO_Acknowledge = TRUE;

    Ptk_ReleaseDriver();

#if !defined(__NO_MIDI__)
    // Close any opened midi devices on any exit
    Midi_CloseIn();
    Midi_CloseOut();
#endif

#if !defined(__NO_MIDI__)
    // Free the devices enumeration
    Midi_FreeAll();
#endif

    Free_Samples();

    Destroy_UI();

    for(i = 0; i < MAX_TRACKS; i++)
    {  
        // ---
        if(Scope_Dats[i])
        {
            free(Scope_Dats[i]);
        }
        Scope_Dats[i] = NULL;

        // ---
        if(VuMeters_Dats_L[i])
        {
            free(VuMeters_Dats_L[i]);
        }
        VuMeters_Dats_L[i] = NULL;
        
        // ---
        if(VuMeters_Dats_R[i])
        {
            free(VuMeters_Dats_R[i]);
        }
        VuMeters_Dats_R[i] = NULL;
    }
    if(Scope_Dats_LeftRight[0])
    {
        free(Scope_Dats_LeftRight[0]);
    }
    Scope_Dats_LeftRight[0] = NULL;
    if(Scope_Dats_LeftRight[1])
    {
        free(Scope_Dats_LeftRight[1]);
    }
    Scope_Dats_LeftRight[1] = NULL;

    // Freeing Allocated Patterns
    if(RawPatterns)
    {
        free(RawPatterns);
    }
    RawPatterns = NULL;

    if(thread_sema)
    {
        SDL_DestroySemaphore(thread_sema);
    }

#if defined(__AMIGAOS4__)
    if(IGraphics)
    {
        IExec->DropInterface((struct Interface*) IGraphics);
    }
    if(GfxBase)
    {
        IExec->CloseLibrary(GfxBase);
    }
#endif

    SDL_Quit();
}

// ------------------------------------------------------
// Strictly for Mac OS X
#if defined(__MACOSX_PPC__)
OSStatus CompletionRoutine(SystemSoundActionID inAction, void *UserDat)
{
    SystemSoundRemoveActionID(inAction);
    return(noErr);
}
#endif

// ------------------------------------------------------
// Update the world
int Screen_Update(void)
{
    int FineTune_Value;
    int i;

#if defined(__MACOSX_PPC__)
    FSRef soundFileRef;
#endif

    if(Burned_Title && !file_loaded)
    {
        if(global_argc != 1)
        {
            Load_File(0, global_argv);
        }
        else
        {
            // Nope: check if auto reload was turned on
            if(AutoReload)
            {
                Load_File(0, Last_Used_Ptk);
            }
        }
        file_loaded = TRUE;
    }

    if(Status_Box_Wait_Done == 1)
    {
        Status_Box("Feeling Groovy...", TRUE);
        Status_Box_Wait_Done = 2;
    }

    redraw_everything = FALSE;
    if(Env_Change)
    {
        redraw_everything = TRUE;
        Env_Change = FALSE;
    }

    for(i = 0; i < Channels_Polyphony[Track_Under_Caret]; i++)
    {
        if(sp_Stage[Track_Under_Caret][i] == PLAYING_SAMPLE &&
		   Current_Instrument == sp_channelsample[Track_Under_Caret][i] &&
		   Current_Instrument_Split == sp_split[Track_Under_Caret][i])
        {
            draw_sampled_wave2 = TRUE;
            boing = TRUE;
            break;
        }
        else if(boing)
        {
            boing = FALSE;
            draw_sampled_wave3 = TRUE;
            break;
        }
    }

    switch(Scopish)
    {
        case SCOPE_ZONE_VUMETERS:
            Draw_VuMeters();
            break;

        case SCOPE_ZONE_SCOPE:
            Draw_Scope();
            break;
    }

    // Sample ed. stuff
    Draw_Wave_Data();

    if(actuloop)
    {
        Display_Fine_Loop_Data();
    }

    if(gui_action_metronome == GUI_CMD_FLASH_METRONOME_ON)
    {
        SetColor(COL_VUMETERPEAK);
        Fillrect(73, 83, 73 + 15, 83 + 15);
    }
    if(gui_action_metronome == GUI_CMD_FLASH_METRONOME_OFF)
    {
        Gui_Draw_Button_Box(72, 82, 16, 16, NULL, BUTTON_DISABLED);
    }
    gui_action_metronome = GUI_CMD_NOP;

    if(gui_action != GUI_CMD_NONE)
    { // There are some for me today.....:)

        // Files list slider
        if(gui_action == GUI_CMD_SET_FILES_LIST_SLIDER)
        {
            lt_ykar[Scopish] = Mouse.y - 72;
            Actualize_Files_List(0);
        }

        // File selection
        if(gui_action == GUI_CMD_SET_FILES_LIST_SELECT_FILE)
        {
            if(lt_items[Scopish])
            {
                int broadcast = lt_index[Scopish] + (Mouse.y - 43) / (Font_Height + 1);
                if(Get_FileType(broadcast) != _A_SEP)
                {
                    last_index = -1;
                    if(broadcast != lt_curr[Scopish])
                    {
                        lt_curr[Scopish] = broadcast;
                        Actualize_Files_List(1);
                    }
                    else
                    {
                        switch(Get_FileType(lt_curr[Scopish]))
                        {
                            case _A_FILE:
                                Load_File(Current_Instrument, Get_FileName(lt_curr[Scopish]));
                                break;
                            case _A_SUBDIR:
                                Set_Current_Dir();
                                Read_SMPT();
                                Actualize_Files_List(0);
                                break;
                        }
                    }
                }
            }
        }

        // Play a .wav
        if(gui_action == GUI_CMD_SET_FILES_LIST_PLAY_WAV)
        {
            if(lt_items[Scopish])
            {
                int broadcast = lt_index[Scopish] + (Mouse.y - 43) / 12;
                last_index = -1;
                lt_curr[Scopish] = broadcast;
                switch(Get_FileType(lt_curr[Scopish]))
                {
                    case _A_FILE:
                        Actualize_Files_List(1);
#if defined(__WIN32__)
                        PlaySound(Get_FileName(lt_curr[Scopish]), NULL, SND_FILENAME | SND_ASYNC);
#endif
#if defined(__MACOSX_PPC__)
                        if(FSPathMakeRef((Uint8 *) Get_FileName(lt_curr[Scopish]), &soundFileRef, NULL) == noErr)
                        {
                            SystemSoundGetActionID(&soundFileRef, &WavActionID);
                            SystemSoundSetCompletionRoutine(WavActionID,
                                                            NULL,
                                                            NULL,
                                                            &CompletionRoutine,
                                                            NULL);
                            AlertSoundPlayCustomSound(WavActionID);
                        }
#endif
#if defined(__LINUX__)
                        FILE *fptr;
                        char temp[1024];

                        sprintf(temp, "aplay %s & > /dev/null", Get_FileName(lt_curr[Scopish]));
                        fptr = popen(temp, "r");
                        pclose(fptr);
#endif
#if defined(__AROS__) || defined(__AMIGAOS4__) || defined(__MORPHOS__)
                        // TODO
#endif

                        break;
                }
            }
        }

        // Instruments/synths list slider
        if(gui_action == GUI_CMD_SET_INSTR_SYNTH_LIST_SLIDER)
        {
            Instrs_ykar = Mouse.y - 72;
            Actualize_Instruments_Synths_List(0);
        }

        // Select an instrument/synth
        if(gui_action == GUI_CMD_SET_INSTR_SYNTH_LIST_SELECT)
        {
            Current_Instrument = Instrs_index + (Mouse.y - 43) / 12;
            Actualize_Instruments_Synths_List(1);
            Actualize_Pattern_Ed();
            Refresh_Sample();
            Renew_Sample_Ed();
            Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        }

        // --- Sequence ---------------------------------------

        if(gui_action == GUI_CMD_PREVIOUS_POSITION)
        {
            Goto_Song_Position(Song_Position - 1);
        }

        if(gui_action == GUI_CMD_NEXT_POSITION)
        {
            Goto_Song_Position(Song_Position + 1);
        }

        if(gui_action == GUI_CMD_SET_PATTERN_LENGTH)
        {
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_PREV_PATT)
        {
            pSequence[Song_Position]--;
            Bound_Patt_Pos();
            Actualize_Sequencer();
            Update_Pattern(0);
            Seq_Bound_Pattern(Song_Position);
        }

        if(gui_action == GUI_CMD_NEXT_PATT)
        {
            pSequence[Song_Position]++;
            Bound_Patt_Pos();
            Actualize_Sequencer();
            Update_Pattern(0);
            Seq_Bound_Pattern(Song_Position);
        }

        if(gui_action == GUI_CMD_GOTO_PREVIOUS_ROW)
        {
            Goto_Previous_Row(TRUE);
        }

        if(gui_action == GUI_CMD_GOTO_NEXT_ROW)
        {
            Goto_Next_Row(TRUE);
        }

        if(gui_action == GUI_CMD_REDUCE_SONG_LENGTH)
        {
            Song_Length--;
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_INCREASE_SONG_LENGTH)
        {
            Song_Length++;
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_UPDATE_SEQUENCER)
        {
            Actualize_Sequencer();
        }

        if(gui_action == GUI_CMD_SWITCH_TRACK_STATUS && can_modify_song)
        {
            Toggle_Track_On_Off_Status(Ext_Pos_Switch, Ext_Track_Switch);
            Actualize_Sequencer();
        }

        if(gui_action == GUI_CMD_UPDATE_PATTERN)
        {
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_SET_FOCUS_TRACK_EXTERNAL)
        {
            Update_Pattern(0);
            Actualize_Track_Ed(0);
            Actualize_Track_Fx_Ed(0);
        }

        if(gui_action == GUI_CMD_SET_FOCUS_TRACK)
        {
            Actualize_Track_Ed(0);
            Actualize_Track_Fx_Ed(0);
        }

        if(gui_action == GUI_CMD_LOWER_OCTAVE)
        {
            Current_Octave--;
            Actualize_Pattern_Ed();
        }

        if(gui_action == GUI_CMD_HIGHER_OCTAVE)
        {
            Current_Octave++;
            Actualize_Pattern_Ed();
        }

        if(gui_action == GUI_CMD_GET_HELP)
        {
            // Retrieve the note under the caret
            int note = Read_Pattern_Note(Get_Song_Position());
            if(note != -1)
            {
                if(note != NO_NOTE && note != NOTE_OFF)
                {
                    // Display the list of chords
                    char ascii_chord[16];
                    char ascii_note1[16];
                    char ascii_note2[16];
                    char ascii_note3[16];
                    char ascii_note4[16];
                    int octave;
                    int octave1;
                    int octave2;
                    int octave3;
                    int octave4;
                    int real_note;
                    real_note = Get_Note_Ascii(note, ascii_chord, &octave, FALSE);

                    Get_Note_Ascii(note, ascii_note1, &octave1, TRUE);

                    Get_Note_Ascii(note + 4, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 7, ascii_note3, &octave3, TRUE);
                    sprintf(Chords_Major, "%s Major:    %s%d   %s%d   %s%d", ascii_chord,
                                                                ascii_note1, octave1,
                                                                ascii_note2, octave2,
                                                                ascii_note3, octave3
                           );
                    Get_Note_Ascii(note + 3, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 7, ascii_note3, &octave3, TRUE);
                    sprintf(Chords_Minor, "%s Minor:    %s%d   %s%d   %s%d", ascii_chord,
                                                                ascii_note1, octave1,
                                                                ascii_note2, octave2,
                                                                ascii_note3, octave3
                           );
                    Get_Note_Ascii(note + 5, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 7, ascii_note3, &octave3, TRUE);
                    sprintf(Chords_Sus4, "%s Sus4:    %s%d   %s%d   %s%d", ascii_chord,
                                                              ascii_note1, octave1,
                                                              ascii_note2, octave2,
                                                              ascii_note3, octave3
                           );
                    Get_Note_Ascii(note + 2, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 4, ascii_note3, &octave3, TRUE);
                    Get_Note_Ascii(note + 7, ascii_note4, &octave4, TRUE);
                    sprintf(Chords_Sus2, "%s Sus2:    %s%d   %s%d   %s%d   %s%d", ascii_chord,
                                                                 ascii_note1, octave1,
                                                                 ascii_note2, octave2,
                                                                 ascii_note3, octave3,
                                                                 ascii_note4, octave4
                           );
                    Get_Note_Ascii(note + 3, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 6, ascii_note3, &octave3, TRUE);
                    sprintf(Chords_Dim, "%s Dim:    %s%d   %s%d   %s%d", ascii_chord,
                                                            ascii_note1, octave1,
                                                            ascii_note2, octave2,
                                                            ascii_note3, octave3
                           );
                    Get_Note_Ascii(note + 4, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 6, ascii_note3, &octave3, TRUE);
                    sprintf(Chords_Aug, "%s Aug:    %s%d   %s%d   %s%d", ascii_chord,
                                                            ascii_note1, octave1,
                                                            ascii_note2, octave2,
                                                            ascii_note3, octave3
                           );
                    Get_Note_Ascii(note + 4, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 7, ascii_note3, &octave3, TRUE);
                    Get_Note_Ascii(note + 11, ascii_note4, &octave4, TRUE);
                    sprintf(Chords_Maj7, "%s Maj 7:    %s%d   %s%d   %s%d   %s%d", ascii_chord,
                                                                  ascii_note1, octave1,
                                                                  ascii_note2, octave2,
                                                                  ascii_note3, octave3,
                                                                  ascii_note4, octave4
                           );
                    Get_Note_Ascii(note + 3, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 7, ascii_note3, &octave3, TRUE);
                    Get_Note_Ascii(note + 11, ascii_note4, &octave4, TRUE);
                    sprintf(Chords_Min7, "%s Min7:    %s%d   %s%d   %s%d   %s%d", ascii_chord,
                                                                 ascii_note1, octave1,
                                                                 ascii_note2, octave2,
                                                                 ascii_note3, octave3,
                                                                 ascii_note4, octave4
                           );

                    Get_Note_Ascii(note + 2, ascii_note2, &octave2, TRUE);
                    Get_Note_Ascii(note + 4, ascii_note3, &octave3, TRUE);
                    Get_Note_Ascii(note + 7, ascii_note4, &octave4, TRUE);
                    sprintf(Chords_Add2, "%s Add2:    %s%d   %s%d   %s%d   %s%d", ascii_chord,
                                                                 ascii_note1, octave1,
                                                                 ascii_note2, octave2,
                                                                 ascii_note3, octave3,
                                                                 ascii_note4, octave4
                           );

                    sprintf(Chords, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",
                                    Chords_Major,
                                    Chords_Minor,
                                    Chords_Sus4,
                                    Chords_Sus2,
                                    Chords_Dim,
                                    Chords_Aug,
                                    Chords_Maj7,
                                    Chords_Min7,
                                    Chords_Add2);
                    Display_Requester(&Helper_Requester, GUI_CMD_NOP, Chords, FALSE);
                }
            }
        }

        if(gui_action == GUI_CMD_DECREASE_STEP_ADD)
        {
            Current_Edit_Steps--;
            if(Current_Edit_Steps < 0) Current_Edit_Steps = 0;
            Actualize_Pattern_Ed();
        }

        if(gui_action == GUI_CMD_INCREASE_STEP_ADD)
        {
            Current_Edit_Steps++;
            if(Current_Edit_Steps > 16) Current_Edit_Steps = 16;
            Actualize_Pattern_Ed();
        }

        if(gui_action == GUI_CMD_SELECT_INSTR_EXTERNAL)
        {
            Clear_Input();
            Actualize_Pattern_Ed();
            Refresh_Sample();
            Renew_Sample_Ed();
            Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        }

        if(gui_action == GUI_CMD_PREV_INSTR)
        {
            Current_Instrument--;
            Clear_Input();
            Actualize_Pattern_Ed();
            Refresh_Sample();
            Renew_Sample_Ed();
            Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        }

        if(gui_action == GUI_CMD_NEXT_INSTR)
        {
            Current_Instrument++;
            Clear_Input();
            Actualize_Pattern_Ed();
            Refresh_Sample();
            Renew_Sample_Ed();
            Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        }

        if(gui_action == GUI_CMD_SWITCH_TRACK_MUTE_STATE)
        {
            int tmp_track = Get_Track_Over_Mouse(Mouse.x, NULL, FALSE);

            if(Chan_Mute_State[tmp_track] == FALSE) Chan_Mute_State[tmp_track] = TRUE;
            else Chan_Mute_State[tmp_track] = FALSE;

            if(userscreen == USER_SCREEN_TRACK_EDIT) Actualize_Track_Ed(10);
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_SWITCH_TRACK_EXPAND_STATE)
        {
            Change_Track_Zoom(Get_Track_Over_Mouse(Mouse.x, NULL, FALSE), TRUE);
            Update_Pattern(0);
            Display_Patterns_Sizes_Status();
        }

        if(gui_action == GUI_CMD_SWITCH_TRACK_REDUCE_STATE)
        {
            Change_Track_Zoom(Get_Track_Over_Mouse(Mouse.x, NULL, FALSE), FALSE);
            Update_Pattern(0);
            Display_Patterns_Sizes_Status();
        }

        if(gui_action == GUI_CMD_REDUCE_TRACK_NOTES)
        {
            Track_Sub_Notes(Get_Track_Over_Mouse(Mouse.x, NULL, FALSE), 1);
            Update_Pattern(0);
            Actualize_Track_Ed(14);
        }

        if(gui_action == GUI_CMD_EXPAND_TRACK_NOTES)
        {
            Track_Add_Notes(Get_Track_Over_Mouse(Mouse.x, NULL, FALSE), 1);
            Update_Pattern(0);
            Actualize_Track_Ed(14);
        }

        if(gui_action == GUI_CMD_REDUCE_TRACK_EFFECTS)
        {
            Track_Sub_Effects(Get_Track_Over_Mouse(Mouse.x, NULL, FALSE), 1);
            Update_Pattern(0);
            Actualize_Track_Ed(16);
        }

        if(gui_action == GUI_CMD_EXPAND_TRACK_EFFECTS)
        {
            Track_Add_Effects(Get_Track_Over_Mouse(Mouse.x, NULL, FALSE), 1);
            Update_Pattern(0);
            Actualize_Track_Ed(16);
        }

        if(gui_action == GUI_CMD_PLAY_SONG)
        {
            Song_Play();
        }

        if(gui_action == GUI_CMD_STOP_SONG)
        {
            Song_Stop();
            is_recording = 0;
            is_recording_2 = 0;
            Nbr_Sub_Note_Off = 0;
            is_editing = FALSE;
            Notify_Edit(TRUE);
        }

        if(gui_action == GUI_CMD_RECORD_303)
        {
            Start_Rec();
        }

        if(gui_action == GUI_CMD_EDIT_MODE)
        {
            is_recording = 0;
            is_editing ^= TRUE;
            is_recording_2 = 0;
            Nbr_Sub_Note_Off = 0;
            Song_Stop();
            Update_Pattern(0);
            Notify_Edit(TRUE);
        }

        if(gui_action == GUI_CMD_RECORD_MODE)
        {
            Song_Stop();
            is_recording ^= 1;
            is_recording_2 = 0;
            Nbr_Sub_Note_Off = 0;
            is_editing = FALSE;
            key_record_first_time = TRUE;
            Clear_Midi_Channels_Pool();
            Update_Pattern(0);
            Notify_Edit(TRUE);
        }

        if(gui_action == GUI_CMD_CHANGE_BPM_TICKS_NBR)
        {
            Actualize_Master(teac);
            Display_Beat_Time();
        }

        if(gui_action == GUI_CMD_CHANGE_TRACKS_NBR ||
           gui_action == GUI_CMD_INSERT_TRACK ||
           gui_action == GUI_CMD_DELETE_TRACK)
        {
            Actualize_Master(teac);
            Update_Pattern(0);
            Draw_Scope();
            Display_Tracks_To_Render();
            Actualize_Track_Ed(0);
            Actualize_Track_Fx_Ed(0);
            Display_Patterns_Sizes_Status();
        }

        if(gui_action == GUI_CMD_SET_TRACK_CUTOFF_FREQ)
        {
            TCut[Track_Under_Caret] = float(Mouse.x - 88);
            Actualize_Track_Ed(1);
            liveparam = LIVE_PARAM_TRACK_CUTOFF;
            livevalue = (Mouse.x - 88) * 2;
        }

        if(gui_action == GUI_CMD_SET_TRACK_RESONANCE)
        {
            FRez[Track_Under_Caret] = Mouse.x - 88;
            Actualize_Track_Ed(2);
            liveparam = LIVE_PARAM_TRACK_RESONANCE;
            livevalue = (Mouse.x - 88) * 2;
        }

        if(gui_action == GUI_CMD_SET_TRACK_INERTIA)
        {
            ICut[Track_Under_Caret] = (Mouse.x - 88.0f) * 0.00006103515625f;
            Actualize_Track_Ed(3);
        }

        if(gui_action == GUI_CMD_UPDATE_TRACK_ED)
        {
            Actualize_Track_Ed(teac);
        }

        if(gui_action == GUI_CMD_SET_TRACK_THRESHOLD)
        {
            DThreshold[Track_Under_Caret] = float((Mouse.x - 318) * 256);
            Actualize_Track_Ed(7);
            liveparam = LIVE_PARAM_TRACK_THRESHOLD;
            livevalue = (Mouse.x - 318) * 2;
        }

        if(gui_action == GUI_CMD_SET_TRACK_CLAMP)
        {
            DClamp[Track_Under_Caret] = float((Mouse.x - 318) * 256);
            Actualize_Track_Ed(8);
            liveparam = LIVE_PARAM_TRACK_CLAMP;
            livevalue = (Mouse.x - 318) * 2;
        }

        if(gui_action == GUI_CMD_SET_TRACK_REVERB_SEND)
        {
            DSend[Track_Under_Caret] = float(((float) Mouse.x - 318) / 128.0f);
            Actualize_Track_Ed(5);
            liveparam = LIVE_PARAM_TRACK_REVERB_SEND;
            livevalue = (Mouse.x - 318) * 2;
        }

        if(gui_action == GUI_CMD_SET_TRACK_PANNING)
        {
            TPan[Track_Under_Caret] = ((float) Mouse.x - 318) / 128.0f;
            Actualize_Track_Ed(9);
            liveparam = LIVE_PARAM_TRACK_PANNING;
            livevalue = Mouse.x - 318;
        }

        // Tabs select
        if(gui_action == GUI_CMD_SELECT_LARGE_PATTERNS)
        {
            if(!Large_Patterns)
            {
                Large_Patterns = TRUE;
                Set_Pattern_Size();
                Draw_Pattern_Right_Stuff();
                Update_Pattern(0);
                Draw_Editors_Bar(USER_SCREEN_LARGE_PATTERN);
            }
            else
            {
                userscreen = curr_tab_highlight;
                Large_Patterns = FALSE;
                Set_Pattern_Size();
                Draw_Pattern_Right_Stuff();
                Update_Pattern(0);
                Draw_Editors_Bar(-1);
                Refresh_UI_Context();
            }
        }

        if(gui_action == GUI_CMD_SELECT_DISKIO_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_DISKIO_EDIT;
            Draw_DiskIO_Ed();
            Actualize_DiskIO_Ed(0);
        }

        if(gui_action == GUI_CMD_SELECT_REVERB_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_REVERB_EDIT;
            Draw_Reverb_Ed();
            Actualize_Reverb_Ed(0);
        }

        if(gui_action == GUI_CMD_SELECT_TRACK_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_TRACK_EDIT;
            Draw_Track_Ed();
            Actualize_Track_Ed(0);
        }

        if(gui_action == GUI_CMD_SELECT_INSTRUMENT_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_INSTRUMENT_EDIT;
            Draw_Instrument_Ed();
            Actualize_Instrument_Ed(2, 0);
        }

        if(gui_action == GUI_CMD_SELECT_FX_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_FX_SETUP_EDIT;
            Draw_Fx_Ed();
            Actualize_Fx_Ed(teac);
        }

        if(gui_action == GUI_CMD_SELECT_SEQUENCER)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_SEQUENCER;
            Draw_Sequencer_Ed();
            Actualize_Seq_Ed(0);
        }

        if(gui_action == GUI_CMD_SELECT_SCREEN_SETUP_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_SETUP_EDIT;
            Draw_Master_Ed();
            Actualize_Master_Ed(0);
        }

        if(gui_action == GUI_CMD_SELECT_MIDI_SETUP)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_SETUP_MIDI;
            Draw_Midi_Ed();
            Actualize_Midi_Ed(0);
        }

        if(gui_action == GUI_CMD_SELECT_SYNTH_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_SYNTH_EDIT;
            Draw_Synth_Ed();
            Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        }

        if(gui_action == GUI_CMD_SELECT_TRACK_FX_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_TRACK_FX_EDIT;
            Draw_Track_Fx_Ed();
            Actualize_Track_Fx_Ed(teac);
        }

        if(gui_action == GUI_CMD_SELECT_SAMPLE_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_SAMPLE_EDIT;
            Draw_Sample_Ed();
            Actualize_Sample_Ed(0);
        }

        if(gui_action == GUI_CMD_SELECT_TB303_EDIT)
        {
            retletter[71] = TRUE;
            Actualize_Input();
            retletter[71] = FALSE;
            userscreen = USER_SCREEN_TB303_EDIT;
            Draw_303_Ed();
        }

        if(gui_action == GUI_CMD_EXPORT_WHOLE_WAV ||
           gui_action == GUI_CMD_EXPORT_SEL_WAV ||
           gui_action == GUI_CMD_EXPORT_LOOP_WAV)
        {
            char buffer[64];
            char wav_filename[MAX_PATH];

            switch(gui_action)
            {
                case GUI_CMD_EXPORT_SEL_WAV:
                    Status_Box("Writing Wav Header And Selected Sample Data Range...", TRUE);
                    break;

                case GUI_CMD_EXPORT_WHOLE_WAV:
                    Status_Box("Writing Wav Header And Whole Sample Data...", TRUE);
                    break;

                case GUI_CMD_EXPORT_LOOP_WAV:
                    Status_Box("Writing Wav Header And Sample Data Loop...", TRUE);
                    break;
            }

            WaveFile RF;

            if(strlen(SampleName[Current_Instrument][Current_Instrument_Split]))
            {
                sprintf(wav_filename, "%s" SLASH "%s", Dir_Samples, SampleName[Current_Instrument][Current_Instrument_Split]);
            }
            else
            {
                sprintf(wav_filename, "%s" SLASH "Untitled.wav", Dir_Samples);
            }

            RF.OpenForWrite(wav_filename,
                            44100,
                            16,
                            Sample_Channels[Current_Instrument][Current_Instrument_Split]);

            char t_stereo;

            if(Sample_Channels[Current_Instrument][Current_Instrument_Split] == 1) t_stereo = FALSE;
            else t_stereo = TRUE;

            Uint32 woff = 0;
            Uint32 wend = 0;

            short *eSamples = RawSamples[Current_Instrument][0][Current_Instrument_Split];
            short *erSamples = RawSamples[Current_Instrument][1][Current_Instrument_Split];

            switch(gui_action)
            {
                case GUI_CMD_EXPORT_SEL_WAV:
                    woff = sed_real_range_start;
                    wend = sed_real_range_end;
                    eSamples += woff;
                    erSamples += woff;
                    break;

                case GUI_CMD_EXPORT_WHOLE_WAV:
                    woff = 0;
                    wend = Sample_Length[Current_Instrument][Current_Instrument_Split];
                    break;

                case GUI_CMD_EXPORT_LOOP_WAV:
                    woff = LoopStart[Current_Instrument][Current_Instrument_Split];
                    wend = LoopEnd[Current_Instrument][Current_Instrument_Split];
                    eSamples += woff;
                    erSamples += woff;
                    break;
            }

            while(woff < wend)
            {
                if(t_stereo) RF.WriteStereoSample(*eSamples++, *erSamples++);
                else RF.WriteMonoSample(*eSamples++);
                woff++;
            }

            // Write the looping info
            if(gui_action != GUI_CMD_EXPORT_SEL_WAV)
            {
                if(LoopType[Current_Instrument][Current_Instrument_Split])
                {
                    RiffChunkHeader header;
                    WaveSmpl_ChunkData data;

                    header.ckID = FourCC("smpl");
                    header.ckSize = 0x3c;

                    memset(&data, 0, sizeof(data));
                    data.Num_Sample_Loops = 1;
                    if(gui_action == GUI_CMD_EXPORT_LOOP_WAV)
                    {
                        // Whole sample is the loop
                        data.Start = 0;
                        data.End = wend;
                    }
                    else
                    {
                        data.Start = LoopStart[Current_Instrument][Current_Instrument_Split];
                        data.End = LoopEnd[Current_Instrument][Current_Instrument_Split];
                    }

                    header.ckSize = Swap_32(header.ckSize);

                    data.Num_Sample_Loops = Swap_32(data.Num_Sample_Loops);
                    data.Start = Swap_32(data.Start);
                    data.End = Swap_32(data.End);

                    RF.WriteData((void *) &header, sizeof(header));
                    RF.WriteData((void *) &data, sizeof(data));
                }
            }
            RF.Close();
            if(strlen(SampleName[Current_Instrument][Current_Instrument_Split])) sprintf(buffer, "File '%s' Saved.", SampleName[Current_Instrument][Current_Instrument_Split]);
            else sprintf(buffer, "File 'Untitled.wav' Saved.");
            Status_Box(buffer, TRUE);

            Read_SMPT();
            last_index = -1;
            Actualize_Files_List(0);
        }

        if(gui_action == GUI_CMD_SAVE_INSTRUMENT)
        {
            if(snamesel == INPUT_NONE) Save_Inst();
        }

        if(gui_action == GUI_CMD_MODULE_INFOS)
        {
            ShowInfo();
        }

        if(gui_action == GUI_CMD_REFRESH_TB303_PARAMS)
        {
            Actualize_303_Ed(teac);
        }

        if(gui_action == GUI_CMD_REFRESH_TB303_VOLUME_EXTERNAL)
        {
            Refresh_303_Unit(Refresh_Unit, 15);
        }

        if(gui_action == GUI_CMD_SAVE_303_PATTERN)
        {
            if(snamesel == INPUT_NONE) Save_303();
        }
        if(gui_action == GUI_CMD_SAVE_REVERB)
        {
            if(snamesel == INPUT_NONE) Save_Reverb();
        }

        if(gui_action == GUI_CMD_SAVE_MIDI_CFG)
        {
            if(snamesel == INPUT_NONE) Save_Midi_Cfg();
        }

        if(gui_action == GUI_CMD_SAVE_PATTERN)
        {
            if(snamesel == INPUT_NONE) Save_Pattern();
        }

        if(gui_action == GUI_CMD_FILELIST_SCROLL)
        {
            Actualize_Files_List(1);
        }

        if(gui_action == GUI_CMD_INSTR_SYNTH_SCROLL)
        {
            Actualize_Instruments_Synths_List(1);
        }

        if(gui_action == GUI_CMD_UPDATE_LOOP_EDITOR_ED)
        {
            // 1 = loop end
            Actualize_Instrument_Ed(1, teac);
        }

        if(gui_action == GUI_CMD_SAVE_MODULE)
        {
            if(snamesel == INPUT_NONE) Pack_Module(name);
        }

        // Save a .ptp module
        if(gui_action == GUI_CMD_SAVE_FINAL)
        {
            if(snamesel == INPUT_NONE) Save_Ptk(name, TRUE, SAVE_WRITE, NULL);
        }

        // Calculate the approximate size of a .ptp module
        if(gui_action == GUI_CMD_CALC_FINAL)
        {
            Final_Mod_Length = Test_Mod();
            Print_Long(254, (Cur_Height - 94), Final_Mod_Length, 7);
        }

        if(gui_action == GUI_CMD_SET_INSTRUMENT_AMPLI)
        {
            Sample_Amplify[Current_Instrument][Current_Instrument_Split] = float((Mouse.x - 436) / 32.0f);
            Actualize_Instrument_Ed(0, 1);
        }

        if(gui_action == GUI_CMD_SET_INSTRUMENT_FINETUNE)
        {
            FineTune_Value = ((Mouse.x - 436) - 64) * 2;
            if(FineTune_Value > 127) FineTune_Value = 127;
            if(FineTune_Value < -127) FineTune_Value = -127;
            Finetune[Current_Instrument][Current_Instrument_Split] = FineTune_Value;
            Actualize_Instrument_Ed(0, 2);
        }

        if(gui_action == GUI_CMD_SET_INSTRUMENT_DECAY)
        {
            FDecay[Current_Instrument][Current_Instrument_Split] = float(Mouse.x - 62) / 8192.0f;
            Actualize_Instrument_Ed(0, 3);
        }

        if(gui_action == GUI_CMD_UPDATE_INSTRUMENT_ED)
        {
            // 0 loop start
            Actualize_Instrument_Ed(0, teac);
        }

        if(gui_action == GUI_CMD_UPDATE_DISKIO_ED)
        {
            Actualize_DiskIO_Ed(teac);
        }

        if(gui_action == GUI_CMD_UPDATE_REVERB_ED)
        {
            Actualize_Reverb_Ed(teac);
        }

        if(gui_action == GUI_CMD_UPDATE_MIDI_ED)
        {
            Actualize_Midi_Ed(teac);
        }

        if(gui_action == GUI_CMD_NEW_MODULE)
        {
            New_Mod();
        }

        if(gui_action == GUI_CMD_UPDATE_SEQ_ED)
        {
            Actualize_Seq_Ed(0);
        }

        if(gui_action == GUI_CMD_UPDATE_FX_ED)
        {
            Actualize_Fx_Ed(teac);
        }
        
        if(gui_action == GUI_CMD_UPDATE_SETUP_ED)
        {
            Actualize_Master_Ed(teac);
        }

        if(gui_action == GUI_CMD_UPDATE_SYNTH_ED)
        {
            Actualize_Synth_Ed(teac);
        }

        if(gui_action == GUI_CMD_MIDI_NOTE_OFF_1_TRACK)
        {
            Status_Box("Notes Off Command Sent To This Track.", TRUE);
        }

        if(gui_action == GUI_CMD_MIDI_NOTE_OFF_ALL_TRACKS)
        {
            Status_Box("Notes Off Command Sent To All Tracks.", TRUE);
        }

        if(gui_action == GUI_CMD_UPDATE_TRACK_FX_ED)
        {
            Actualize_Track_Fx_Ed(teac);
        }

        if(gui_action == GUI_CMD_UPDATE_MIDI_303_ED)
        {
            Actualize_303_Ed(teac);
        }

        if(gui_action == GUI_CMD_PREVIOUS_16_INSTR)
        {
            Current_Instrument -= 16;
            Actualize_Pattern_Ed();
            Refresh_Sample();
            Renew_Sample_Ed();
            Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        }

        if(gui_action == GUI_CMD_NEXT_16_INSTR)
        {
            Current_Instrument += 16;
            Actualize_Pattern_Ed();
            Refresh_Sample();
            Renew_Sample_Ed();
            Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        }

        if(gui_action == GUI_CMD_UPDATE_PATTERN_ED)
        {
            Actualize_Pattern_Ed();
        }

        if(gui_action == GUI_CMD_GOTO_START_POSITION)
        {
            Song_Position = 0;
            Bound_Patt_Pos();
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_GOTO_END_POSITION)
        {
            Song_Position = Song_Length - 1;
            Bound_Patt_Pos();
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_REDUCE_POSITIONS_10)
        {
            Song_Position -= 10;
            Bound_Patt_Pos();
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_INCREASE_POSITIONS_10)
        {
            Song_Position += 10;
            Bound_Patt_Pos();
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_REDUCE_PATTERNS_10)
        {
            if(pSequence[Song_Position] > 9) pSequence[Song_Position] -= 10;
            else pSequence[Song_Position] = 0;
            Bound_Patt_Pos();
            Seq_Bound_Pattern(Song_Position);
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_INCREASE_PATTERNS_10)
        {
            if(pSequence[Song_Position] < 118) pSequence[Song_Position] += 10;
            else pSequence[Song_Position] = 127;
            Bound_Patt_Pos();
            Seq_Bound_Pattern(Song_Position);
            Actualize_Sequencer();
            Update_Pattern(0);
        }

        if(gui_action == GUI_CMD_RENDER_WAV)
        {
            if(snamesel == INPUT_NONE)
            {
                Wav_Renderizer();
            }
        }

        if(gui_action == GUI_CMD_TIMED_REFRESH_SEQUENCER)
        {
            Actualize_Sequencer();
            player_pos = Get_Song_Position();
        }

        if(gui_action == GUI_CMD_DELETE_INSTRUMENT)
        {
            Clear_Input();
            Delete_Instrument();
        }

        if(gui_action == GUI_CMD_SAVE_SYNTH)
        {
            if(snamesel == INPUT_NONE) Save_Synth();
        }

        if(gui_action == GUI_CMD_PATTERNS_POOL_EXHAUSTED)
        {
            Status_Box("Maximum Number Of Patterns Reached.", TRUE);
        }

        if(gui_action == GUI_CMD_REFRESH_SAMPLE_ED)
        {
            Actualize_Sample_Ed(teac);
        }

        if(gui_action == GUI_CMD_EXIT)
        {
            Display_Requester(&Exit_Requester, GUI_CMD_NOP, NULL, TRUE);
        }

        gui_action = GUI_CMD_NOP;
    }

    // Draw the main windows layout
    if(redraw_everything)
    {
        last_index = -1;
        // Box around the vu-meters
        Gui_Draw_Button_Box(MIN_VUMETER - 4, 6, (MAX_VUMETER - MIN_VUMETER) + 6, 16, NULL, BUTTON_NORMAL | BUTTON_DISABLED);

        Display_Master_Comp();
        Display_Master_Volume();
        Display_Shuffle();

        Draw_Scope_Files_Button();

        if(!Done_Tip)
        {
            Status_Box(tipoftheday, FALSE);
            Done_Tip = TRUE;
        }
        else
        {
            Status_Box(NULL, FALSE);
        }

        Gui_Draw_Button_Box(0, 6, 16, 16, "\011", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);

        Gui_Draw_Button_Box(0, 180, fsize, 2, NULL, BUTTON_NORMAL | BUTTON_DISABLED);

        Gui_Draw_Button_Box(0, 24, 96, 78, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
        
        // ||
        Gui_Draw_Button_Box(8, 46, 80, 16, "\254", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        Notify_Play();
        Start_Rec();

        Gui_Draw_Button_Box(98, 24, 156, 78, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(106, 28, 80, 16, "Position", BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(106, 46, 80, 16, "Pattern", BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(106, 64, 80, 16, "Song Length", BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(106, 82, 80, 16, "Pattern Lines", BUTTON_NORMAL | BUTTON_DISABLED);

        Gui_Draw_Button_Box(256, 24, 136, 78, NULL, BUTTON_NORMAL | BUTTON_DISABLED);

        Gui_Draw_Button_Box(262, 28, 38, 16, "Tracks", BUTTON_NORMAL | BUTTON_DISABLED);

        Gui_Draw_Button_Box(262, 46, 60, 16, "Beats/Min.", BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(262, 64, 60, 16, "Ticks/Beat", BUTTON_NORMAL | BUTTON_DISABLED);
        Display_Beat_Time();
        Notify_Edit(FALSE);

        Gui_Draw_Button_Box(0, 104, 392, 42, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(8, 108, 80, 16, "Instrument", BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(320, 108, 28, 16, "Del", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        Gui_Draw_Button_Box(8, 126, 80, 16, "Step Add", BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(166, 126, 90, 16, "Keyboard Octave", BUTTON_NORMAL | BUTTON_DISABLED);

        // Patterns sizes
        Display_Patterns_Sizes_Status();

        Gui_Draw_Button_Box(0, 148, 392, 30, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
        Gui_Draw_Button_Box(8, 152, 61, 10, S_ E_ L_ DOT_ SPC_ T_ R_ A_ C_ K_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);
        Gui_Draw_Button_Box(8, 164, 61, 10, S_ E_ L_ DOT_ SPC_ N_ O_ T_ E_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);

        Gui_Draw_Button_Box(8 + 63, 152, 61, 10, C_ U_ T_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);
        Gui_Draw_Button_Box(8 + 63, 164, 61, 10, C_ O_ P_ Y_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);

        Gui_Draw_Button_Box(8 + (63 * 2), 152, 61, 10, P_ A_ S_ T_ E_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);
        Gui_Draw_Button_Box(8 + (63 * 2), 164, 61, 10, D_ E_ L_ E_ T_ E_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);

        Gui_Draw_Button_Box(8 + (63 * 3), 152, 61, 10, I_ N_ T_ E_ R_ P_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);

        Gui_Draw_Button_Box(8 + (63 * 3), 164, 30, 10, R_ A_ N_ D_ , BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);
        Gui_Draw_Button_Box(8 + (63 * 3) + 32, 164, 29, 10, F_ I_ L_ L_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);

        Gui_Draw_Button_Box(8 + (63 * 4), 152, 61, 10, S_ E_ M_ I_ TIR_ T_ O_ N_ E_ SPC_ U_ P_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);
        Gui_Draw_Button_Box(8 + (63 * 4), 164, 61, 10, S_ E_ M_ I_ TIR_ T_ O_ N_ E_ SPC_ D_ N_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);

        Gui_Draw_Button_Box(8 + (63 * 5), 152, 61, 10, O_ C_ T_ A_ V_ E_ SPC_ U_ P_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);
        Gui_Draw_Button_Box(8 + (63 * 5), 164, 61, 10, O_ C_ T_ A_ V_ E_ SPC_ D_ N_, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | BUTTON_SMALL_FONT);

        Refresh_UI_Context();

        Actualize_Files_List(0);

        // Vu meters background
        SetColor(COL_BLACK);
        Fillrect(MIN_VUMETER - 1, 9, MAX_VUMETER, 20);
        SetColor(COL_BACKGROUND);
        Fillrect(MIN_VUMETER, 10, MAX_VUMETER - 1, 19);

        Draw_Pattern_Right_Stuff();
        Update_Pattern(0);
        redraw_everything = FALSE;
        Push_Update_Rect(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT);
    }
    
    int Lt_vu = (int) (MIN_VUMETER + (((float) L_MaxLevel / 32767.0f) * LARG_VUMETER));
    int Rt_vu = (int) (MIN_VUMETER + (((float) R_MaxLevel / 32767.0f) * LARG_VUMETER));
    int Lt_vu_Peak = Lt_vu;
    int Rt_vu_Peak = Rt_vu;

    if(Lt_vu > MIN_PEAK)
    {
        Lt_vu = MIN_PEAK;
    }
    if(Rt_vu > MIN_PEAK)
    {
        Rt_vu = MIN_PEAK;
    }
    if(Lt_vu_Peak > MAX_VUMETER - 2)
    {
        Lt_vu_Peak = MAX_VUMETER - 2;
    }
    if(Rt_vu_Peak > MAX_VUMETER - 2)
    {
        Rt_vu_Peak = MAX_VUMETER - 2;
    }
    
    // Clear
    SetColor(COL_BACKGROUND);
    Fillrect(Lt_vu_Peak, 10, MAX_VUMETER - 1, 14);
    Fillrect(Rt_vu_Peak, 15, MAX_VUMETER - 1, 19);

    // Draw the vu meters
    for(i = MIN_VUMETER; i < Lt_vu; i += 2)
    {
        DrawVLine(i, 10, 13, COL_VUMETER);
    }
    for(i = MIN_VUMETER; i < Rt_vu; i += 2)
    {
        DrawVLine(i, 15, 18, COL_VUMETER);
    }
    for(i = Lt_vu + 1; i < Lt_vu_Peak; i += 2)
    {
        DrawVLine(i, 10, 13, COL_VUMETERPEAK);
    }
    for(i = Rt_vu + 1; i < Rt_vu_Peak; i += 2)
    {
        DrawVLine(i, 15, 18, COL_VUMETERPEAK);
    }

    Push_Update_Rect(MIN_VUMETER, 10, Rt_vu_Peak, 18);

    if(gui_thread_action && gui_thread_can_act)
    {
        gui_thread_action = FALSE;
        Update_Pattern(0);
    }
    if(gui_bpm_action)
    {
        gui_bpm_action = FALSE;
        Display_Beat_Time();
        Actualize_Master(2);
        Display_Shuffle();
    }

    if(Song_Playing && Pattern_Line_Visual != player_line)
    {
        // --------------------------------------
        // Updates coming from the replay
        if(!sr_isrecording)
        {
            if(gui_action_external & GUI_UPDATE_EXTERNAL_REVERB_FILTER)
            {
                Actualize_Fx_Ed(9);
                Actualize_Fx_Ed(14);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_REVERB_FILTER;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_REVERB_DAMP)
            {
                Actualize_Fx_Ed(15);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_REVERB_DAMP;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_COMPRESSOR)
            {
                Actualize_Track_Fx_Ed(12);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_COMPRESSOR;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_SEND_TO_REVERB)
            {
                Actualize_Track_Ed(5);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_SEND_TO_REVERB;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_DISTO_THRESHOLD)
            {
                Actualize_Track_Ed(7);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_DISTO_THRESHOLD;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_DISTO_CLAMP)
            {
                Actualize_Track_Ed(8);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_DISTO_CLAMP;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_FILTER_CUTOFF)
            {
                Actualize_Track_Ed(1);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_FILTER_CUTOFF;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_FILTER_RESONANCE)
            {
                Actualize_Track_Ed(2);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_FILTER_RESONANCE;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_FILTER_TYPE)
            {
                Actualize_Track_Ed(4);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_FILTER_TYPE;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_FLANGER)
            {
                Actualize_Track_Fx_Ed(11);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_FLANGER;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_LFO)
            {
                Actualize_Track_Fx_Ed(15);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_LFO;
            }

            if(gui_action_external & GUI_UPDATE_EXTERNAL_SET_PANNING)
            {
                Actualize_Track_Ed(9);
                gui_action_external &= ~GUI_UPDATE_EXTERNAL_SET_PANNING;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_1_TUNE)
            {
                Refresh_303_Unit(0, 3);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_1_TUNE;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_2_TUNE)
            {
                Refresh_303_Unit(1, 3);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_2_TUNE;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_1_CUTOFF)
            {
                Refresh_303_Unit(0, 4);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_1_CUTOFF;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_2_CUTOFF)
            {
                Refresh_303_Unit(1, 4);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_2_CUTOFF;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_1_RESONANCE)
            {
                Refresh_303_Unit(0, 5);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_1_RESONANCE;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_2_RESONANCE)
            {
                Refresh_303_Unit(1, 5);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_2_RESONANCE;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_1_ENVMOD)
            {
                Refresh_303_Unit(0, 6);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_1_ENVMOD;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_2_ENVMOD)
            {
                Refresh_303_Unit(1, 6);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_2_ENVMOD;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_1_DECAY)
            {
                Refresh_303_Unit(0, 7);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_1_DECAY;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_2_DECAY)
            {
                Refresh_303_Unit(1, 7);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_2_DECAY;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_1_ACCENT)
            {
                Refresh_303_Unit(0, 8);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_1_ACCENT;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_2_ACCENT)
            {
                Refresh_303_Unit(1, 8);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_2_ACCENT;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_1_SCALE)
            {
                Refresh_303_Unit(0, 19);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_1_SCALE;
            }

            if(gui_action_external_303 & GUI_UPDATE_EXTERNAL_303_2_SCALE)
            {
                Refresh_303_Unit(1, 19);
                gui_action_external_303 &= ~GUI_UPDATE_EXTERNAL_303_2_SCALE;
            }

        }

        // --------------------------------------
        Update_Pattern(0);
        player_line = Pattern_Line_Visual;
    }

    // Checking for mouse and keyboard events ------------------------------------
    if(!In_Requester)
    {
        Mouse_Handler();
        Keyboard_Handler();
    }
    else
    {
        Mouse_Handler_Requester();
        Keyboard_Handler_Requester();
    }

    Display_RShift_Status();

    // Refresh the sequencer each time the song position is different
    if(Song_Playing)
    {
        if(player_pos != Song_Position_Visual)
        {
            gui_action = GUI_CMD_TIMED_REFRESH_SEQUENCER;
        }
    }
    if(SplashScreen)
    {
        Check_Requester(&Title_Requester);
    }

    if(Check_Requester(&Overwrite_Requester) == 2)
    {
        gui_action = GUI_CMD_NOP;
    }

    switch(Check_Requester(&Zzaapp_Requester))
    {
        case 1:
            // All
            ZzaappOMatic = ZZAAPP_ALL;
            break;

        case 2:
            // Patterns
            ZzaappOMatic = ZZAAPP_PATTERNS;
            break;

        case 3:
            // Instruments
            ZzaappOMatic = ZZAAPP_INSTRUMENTS;
            break;

        case 4:
            // Synths
            ZzaappOMatic = ZZAAPP_SYNTHS;
            break;

        case 5:
            // 303
            ZzaappOMatic = ZZAAPP_303;
            break;

        case 6:
            // Cancel
            gui_action = GUI_CMD_NOP;
            break;

    }

    switch(Check_Requester(&Delete_Requester))
    {
        case 1:
            // All
            ZzaappOMatic = ZZAAPP_ALL;
            break;

        case 2:
            // Instrument
            ZzaappOMatic = ZZAAPP_INSTRUMENTS;
            break;

        case 3:
            // Split
            ZzaappOMatic = ZZAAPP_SPLIT;
            break;

        case 4:
            // Synth
            ZzaappOMatic = ZZAAPP_SYNTHS;
            break;

        case 5:
            gui_action = GUI_CMD_NOP;
            break;

    }

    Check_Requester(&Helper_Requester);
    
    if(Check_Requester(&Exit_Requester) == 1)
    {
        Song_Stop();
        Status_Box("Seppuku...", FALSE);
        return(FALSE);
    }

    return(TRUE);
}

// ------------------------------------------------------
// Attempt to load any supported file format
void Load_File(int Freeindex, const char *str)
{
    long Datalen = 0;
    int ld0 = 0;
    int ld1 = 0;
    int ld2 = 0;
    int ld3 = 0;
    int Freeindex2 = 0;
    FILE *in;
    WaveFile Wav_File;
    AIFFFile AIFF_File;
    float save_sample_vol;

    int rate = 0;
    int bits = 0;
    short *csamples, *csamples2;
    char st_type = 0;
    short inx = 0;
    int fmtchklen = 0;
    int digibooster = FALSE;

    const char *FileName = str;

    if(FileName != NULL && (in = fopen(FileName, "rb")) != NULL)
    {
        char extension[10];
        int extension_AIFF[3];
        char extension_digi[21];
        unsigned extension_New;
        int modext;
        char found_mod;
        int i;
        //char rebext[5];

        fseek(in, 1080, SEEK_SET);
        fread(&modext, sizeof(char), 4, in);

        fseek(in, 0, SEEK_SET);
        fread(extension, sizeof(char), 9, in);

        fseek(in, 0, SEEK_SET);
        fread(&extension_New, sizeof(char), 4, in);

        fseek(in, 0, SEEK_SET);
        fread(&extension_AIFF, sizeof(char), 12, in);

        memset(extension_digi, 0, sizeof(extension_digi));
        fseek(in, 0, SEEK_SET);
        fread(&extension_digi, sizeof(char), 20, in);

        // Look for sound/noise/star/protracker module
        found_mod = 0;
        for(i = 0; i < sizeof(mt_tags) / sizeof(int); i++)
        {
            if(Swap_32(mt_tags[i]) == modext)
            {
                found_mod = mt_channels[i];
                break;
            }
        }
        
        // Look for a digibooster v1.x module
        if(strcmp(extension_digi, "DIGI Booster module") == 0)
        {
            digibooster = TRUE;
            // Retrieve the number of channels
            fseek(in, 25, SEEK_SET);
            fread(&found_mod, sizeof(char), 1, in);
        }

        if(found_mod)
        {
            sprintf(name, "%s", FileName);
            // name / number of channels
            Song_Stop();
            AUDIO_Stop();
            Lock_Audio_Thread();
            Load_Amiga_Mod(name, FileName, found_mod, digibooster);
            Renew_Sample_Ed();
            fclose(in);
            gui_action = GUI_CMD_NONE;
            Actualize_DiskIO_Ed(0);
            Unlock_Audio_Thread();
            AUDIO_Play();
            return;
        }

        //fseek(in, 8, SEEK_SET);
        //fread(rebext, sizeof(char), 4, in);

        if(/*strcmp(extension, "TWNNINS0") == 0 ||
           strcmp(extension, "TWNNINS1") == 0 ||
           strcmp(extension, "TWNNINS2") == 0 ||
           strcmp(extension, "TWNNINS3") == 0 ||
           strcmp(extension, "TWNNINS4") == 0 ||
           strcmp(extension, "TWNNINS5") == 0 ||
           strcmp(extension, "TWNNINS6") == 0 ||
           strcmp(extension, "TWNNINS7") == 0 ||*/
           strcmp(extension, "TWNNINS8") == 0 ||
           strcmp(extension, "TWNNINS9") == 0 ||
           strcmp(extension, "TWNNINS9") == 0 ||
           strcmp(extension, "PROTINSA") == 0 ||
           strcmp(extension, "PROTINSB") == 0 ||
           strcmp(extension, "PROTINSC") == 0 ||
           strcmp(extension, "PROTINSD") == 0)
        {
            sprintf(instrname, "%s", FileName);
            Lock_Audio_Thread();
            Stop_Current_Instrument();
            Load_Inst(instrname);
            Renew_Sample_Ed();
            Unlock_Audio_Thread();
        }
        else if(/*strcmp(extension, "TWNNSNG1") == 0 ||   // For salvage purpose
                strcmp(extension, "TWNNSNG2") == 0 ||
                strcmp(extension, "TWNNSNG3") == 0 ||
                strcmp(extension, "TWNNSNG4") == 0 ||
                strcmp(extension, "TWNNSNG5") == 0 ||
                strcmp(extension, "TWNNSNG6") == 0 ||
                strcmp(extension, "TWNNSNG7") == 0 ||
                strcmp(extension, "TWNNSNG8") == 0 ||
                strcmp(extension, "TWNNSNG9") == 0 ||
                strcmp(extension, "TWNNSNGA") == 0 ||
                strcmp(extension, "TWNNSNGB") == 0 ||
                strcmp(extension, "TWNNSNGC") == 0 ||
                strcmp(extension, "TWNNSNGD") == 0 ||
                strcmp(extension, "TWNNSNGE") == 0 ||
                strcmp(extension, "TWNNSNGF") == 0 ||
                strcmp(extension, "TWNNSNGG") == 0 ||
                strcmp(extension, "TWNNSNGH") == 0 ||
                strcmp(extension, "TWNNSNGI") == 0 ||
                strcmp(extension, "TWNNSNGJ") == 0 ||*/
                strcmp(extension, "PROTREKK") == 0 ||
                strcmp(extension, "PROTREKL") == 0 ||
                strcmp(extension, "PROTREKM") == 0 ||
                strcmp(extension, "PROTREKN") == 0 ||
                strcmp(extension, "PROTREKO") == 0 ||
                strcmp(extension, "PROTREKP") == 0 ||
                strcmp(extension, "PROTREKQ") == 0 ||
                strcmp(extension, "PROTREKR") == 0 ||
                strcmp(extension, "PROTREKS") == 0 ||
                strcmp(extension, "PROTREKT") == 0 ||
                strcmp(extension, "PROTREKU") == 0 ||
                strcmp(extension, "PROTREKV") == 0)
        {
            sprintf(name, "%s", FileName);
            Song_Stop();
            AUDIO_Stop();
            Lock_Audio_Thread();
            Load_Ptk(name);
            Renew_Sample_Ed();
            Unlock_Audio_Thread();
            AUDIO_Play();
        }
        else if(strcmp(extension, "TWNNSYN0") == 0 ||
                strcmp(extension, "TWNNSYN1") == 0 ||
                strcmp(extension, "TWNNSYN2") == 0 ||
                strcmp(extension, "TWNNSYN3") == 0 ||
                strcmp(extension, "TWNNSYN4") == 0 ||
                strcmp(extension, "TWNNSYN5") == 0 ||
                strcmp(extension, "TWNNSYN6") == 0 ||
                strcmp(extension, "TWNNSYN7") == 0 ||
                strcmp(extension, "PROTSYN8") == 0)
        {
            sprintf(synthname, "%s", FileName);
            Lock_Audio_Thread();
            Stop_Current_Instrument();
            Load_Synth(synthname);
            Unlock_Audio_Thread();
        }
        else if(strcmp(extension, "TWNN3031") == 0 ||
                strcmp(extension, "PROT3031") == 0)
        {
            sprintf(name303, "%s", FileName);
            Lock_Audio_Thread();
            Load_303(name303);
            Unlock_Audio_Thread();
        }
        else if(strcmp(extension, "TWNNREV1") == 0 ||
                strcmp(extension, "PROTREV1") == 0 ||
                strcmp(extension, "PROTREV2") == 0)
        {
            sprintf(namerev, "%s", FileName);
            Lock_Audio_Thread();
            Load_Reverb(namerev);
            Unlock_Audio_Thread();
        }
        else if(strcmp(extension, "PROTMID1") == 0)
        {
            sprintf(namemidicfg, "%s", FileName);
            Lock_Audio_Thread();
            Load_Midi_Cfg(namemidicfg);
            Unlock_Audio_Thread();
        }
        else if((strcmp(extension, "TWNNBLK1") == 0) ||
                (strcmp(extension, "PROTBLK2") == 0)
               )
        {
            sprintf(namerev, "%s", FileName);
            Lock_Audio_Thread();
            Load_Pattern(namerev);
            Unlock_Audio_Thread();
        }
        else
        {
            if((extension_AIFF[0] == FormID &&
                extension_AIFF[2] == AiffID) ||
               (extension_AIFF[0] == FormID &&
                extension_AIFF[2] == AifcID))
            {
                Status_Box("Attempting To Load An Audio IFF File...", TRUE);
                if(AIFF_File.Open(FileName))
                {
                    int bits = AIFF_File.BitsPerSample();
                    int channels = AIFF_File.NumChannels();

                    if(channels != 1 && channels != 2)
                    {
                        Status_Box(TITLE " Can Only Load Mono Or Stereo Samples.", TRUE);
                    }
                    else
                    {
                        if(bits != 8 &&
                           bits != 12 &&
                           bits != 16 &&
                           bits != 24 &&
                           bits != 32 &&
                           bits != 64)
                        {
                            Status_Box(TITLE " Can Only Load 8, 12, 16, 24, 32 Or 64 Bit Samples.", TRUE);
                        }
                        else
                        {
                            save_sample_vol = 1.0f;
                            if(Get_Number_Of_Splits(Freeindex)) save_sample_vol = Sample_Vol[Freeindex];
                            sp_Position[Freeindex][Current_Instrument_Split].absolu = 0;
                            Lock_Audio_Thread();
                            Stop_Current_Instrument();
                            switch(channels)
                            {
                                case 1:
                                    Allocate_Wave(Freeindex, Current_Instrument_Split, AIFF_File.NumSamples(), 1, TRUE, NULL, NULL);
                                    csamples = RawSamples[Freeindex][0][Current_Instrument_Split];
                                    for(i = 0; i < AIFF_File.NumSamples(); i++)
                                    {
                                        AIFF_File.ReadMonoSample(&csamples[i]);
                                    }
                                    break;

                                case 2:
                                    Allocate_Wave(Freeindex, Current_Instrument_Split, AIFF_File.NumSamples(), 2, TRUE, NULL, NULL);
                                    csamples = RawSamples[Freeindex][0][Current_Instrument_Split];
                                    csamples2 = RawSamples[Freeindex][1][Current_Instrument_Split];
                                    for(i = 0; i < AIFF_File.NumSamples(); i++)
                                    {
                                        AIFF_File.ReadStereoSample(&csamples[i], &csamples2[i]);
                                    }
                                    break;
                            }
                            LoopStart[Freeindex][Current_Instrument_Split] = 0;
                            LoopEnd[Freeindex][Current_Instrument_Split] = 0;
                            LoopType[Freeindex][Current_Instrument_Split] = SMP_LOOP_NONE;
                            Basenote[Freeindex][Current_Instrument_Split] = DEFAULT_BASE_NOTE;
                            if(Get_Number_Of_Splits(Freeindex) == 1)
                            {
                                Sample_Vol[Freeindex] = save_sample_vol;
                            }

                            if(AIFF_File.BaseNote())
                            {
                                Basenote[Freeindex][Current_Instrument_Split] = AIFF_File.BaseNote();
                            }

                            if(AIFF_File.LoopType())
                            {
                                LoopType[Freeindex][Current_Instrument_Split] = AIFF_File.LoopType();
                                LoopStart[Freeindex][Current_Instrument_Split] = AIFF_File.LoopStart();
                                LoopEnd[Freeindex][Current_Instrument_Split] = AIFF_File.LoopEnd();
                            }

                            sprintf(SampleName[Freeindex][Current_Instrument_Split], "%s", FileName);
                            Actualize_Pattern_Ed();
                            Actualize_Instrument_Ed(2, 0);
                            Renew_Sample_Ed();
                            switch(bits)
                            {
                                case 64:
                                    Status_Box("64 Bit Audio IFF PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 32:
                                    Status_Box("32 Bit Audio IFF PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 24:
                                    Status_Box("24 Bit Audio IFF PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 12:
                                    Status_Box("12 Bit Audio IFF PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 8:
                                    Status_Box("8 Bit Audio IFF PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                default:
                                    Status_Box("16 Bit Audio IFF PCM Loaded.", TRUE);
                                    break;
                            }
                            Unlock_Audio_Thread();
                        }
                    }
                    AIFF_File.Close();
                    Status_Box("File Loaded Successfully.", TRUE);
                }
                else
                {
                    Status_Box("Corrupted Or Unsupported Audio IFF File.", TRUE);
                }
            }
            else
            {
                Status_Box("Attempting to Load A RIFF File...", TRUE);

                // We need the length
                if(Wav_File.OpenForRead(FileName) == DDC_SUCCESS)
                {
                    int bits = Wav_File.BitsPerSample();
                    int channels = Wav_File.NumChannels();
                    if(channels != 1 && channels != 2)
                    {
                        Status_Box(TITLE " Can Only Load Mono Or Stereo Samples.", TRUE);
                    }
                    else
                    {
                        if(bits != 8 &&
                           bits != 12 &&
                           bits != 16 &&
                           bits != 24 &&
                           bits != 32 &&
                           bits != 64)
                        {
                            Status_Box(TITLE " Can Only Load 8, 12, 16, 24, 32 Or 64 Bit Samples.", TRUE);
                        }
                        else
                        {
                            save_sample_vol = 1.0f;
                            if(Get_Number_Of_Splits(Freeindex)) save_sample_vol = Sample_Vol[Freeindex];
                            sp_Position[Freeindex][Current_Instrument_Split].absolu = 0;
                            Lock_Audio_Thread();
                            Stop_Current_Instrument();
                            switch(channels)
                            {
                                case 1:
                                    Allocate_Wave(Freeindex, Current_Instrument_Split, Wav_File.NumSamples(), 1, TRUE, NULL, NULL);
                                    csamples = RawSamples[Freeindex][0][Current_Instrument_Split];
                                    for(i = 0; i < Wav_File.NumSamples(); i++)
                                    {
                                        Wav_File.ReadMonoSample(&csamples[i]);
                                    }
                                    break;

                                case 2:
                                    Allocate_Wave(Freeindex, Current_Instrument_Split, Wav_File.NumSamples(), 2, TRUE, NULL, NULL);
                                    csamples = RawSamples[Freeindex][0][Current_Instrument_Split];
                                    csamples2 = RawSamples[Freeindex][1][Current_Instrument_Split];
                                    for(i = 0; i < Wav_File.NumSamples(); i++)
                                    {
                                        Wav_File.ReadStereoSample(&csamples[i], &csamples2[i]);
                                    }
                                    break;
                            }
                            LoopStart[Freeindex][Current_Instrument_Split] = 0;
                            LoopEnd[Freeindex][Current_Instrument_Split] = 0;
                            LoopType[Freeindex][Current_Instrument_Split] = SMP_LOOP_NONE;
                            Basenote[Freeindex][Current_Instrument_Split] = DEFAULT_BASE_NOTE;
                            if(Get_Number_Of_Splits(Freeindex) == 1)
                            {
                                Sample_Vol[Freeindex] = save_sample_vol;
                            }

                            if(Wav_File.LoopType())
                            {
                                LoopType[Freeindex][Current_Instrument_Split] = Wav_File.LoopType();
                                LoopStart[Freeindex][Current_Instrument_Split] = Wav_File.LoopStart();
                                LoopEnd[Freeindex][Current_Instrument_Split] = Wav_File.LoopEnd();
                            }

                            sprintf(SampleName[Freeindex][Current_Instrument_Split], "%s", FileName);
                            Actualize_Pattern_Ed();
                            Actualize_Instrument_Ed(2, 0);
                            Renew_Sample_Ed();
                            switch(bits)
                            {
                                case 64:
                                    Status_Box("64 Bit WAV PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 32:
                                    Status_Box("32 Bit WAV PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 24:
                                    Status_Box("24 Bit WAV PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 12:
                                    Status_Box("12 Bit WAV PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                case 8:
                                    Status_Box("8 Bit WAV PCM Converted Into 16 Bit Format.", TRUE);
                                    break;
                                default:
                                    Status_Box("16 Bit WAV PCM Loaded.", TRUE);
                                    break;
                            }
                            Unlock_Audio_Thread();
                        }
                    }
                    Wav_File.Close();
                    Status_Box("File Loaded Successfully.", TRUE);
                }
                else
                {
                    Status_Box("Invalid File Format. I Only Accept '.wav' '.aiff' '.aifc' '.ptk' '.pti' '.303' '.pts' '.ppb' '.prv' '.mod' '.dbm' Or '.ft' Files.", TRUE);
                }
            }
        }
        fclose(in);
    }
    else
    {
        Status_Box("File Loading Error. (Possible Cause: File Not Found)", TRUE);
    }
    gui_action = GUI_CMD_NONE;
    Actualize_DiskIO_Ed(0);
}

// ------------------------------------------------------
// Function to search for a free wave on board
int Get_Free_Wave(void)
{
    int inico = 0;
    int exiter = 0;
    int valuer = -1;

    do
    {
        if(SampleType[inico] == 0)
        {
            exiter = 1;
            valuer = inico;
        }
        inico++;
    }
    while(exiter == 0);

    return valuer;
}

// ------------------------------------------------------
// Start replaying
void Song_Play(void)
{
    Ptk_Stop();
    SubCounter = 0;
    PosInTick = 0;
    PosInTick_Delay = 0;
    liveparam = 0;
    livevalue = 0;
    player_pos = -1;

    Post_Song_Init();
    Ptk_Play();

    Notify_Play();
}

// ------------------------------------------------------
// Display live recording status
// "start_rec" got it ? haha :(
void Start_Rec(void)
{
    liveparam = 0;
    livevalue = 0;
    if(sr_isrecording) Gui_Draw_Button_Box(8, 64, 80, 16, "Live Rec: On", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
    else Gui_Draw_Button_Box(8, 64, 80, 16, "Live Rec: Off", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
}

// ------------------------------------------------------
// Display editing status
void Notify_Edit(int After_Song_Stop)
{
    if(is_editing && !is_recording)
    {
        Gui_Draw_Button_Box(8, 82, 62, 16, "Editing...", BUTTON_PUSHED | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
    }
    if(is_recording)
    {
        Gui_Draw_Button_Box(8, 82, 62, 16, "Recording...", BUTTON_PUSHED | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
    }
    if(!is_editing && !is_recording)
    {
        Gui_Draw_Button_Box(8, 82, 62, 16, "Edit/Rec.", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
    }
    Gui_Draw_Button_Box(72, 82, 16, 16, NULL, BUTTON_DISABLED);
    Switch_Cmd_Playing(After_Song_Stop);
    Actualize_Sequencer();
    gui_action_metronome = GUI_CMD_FLASH_METRONOME_OFF;
}

// ------------------------------------------------------
// Switch the command that can't be modified during playing
void Switch_Cmd_Playing(int After_Song_Stop)
{
    int Was_Song_Playing;
    can_modify_song = 0;
    if(is_editing)
    {
        can_modify_song = BUTTON_DISABLED;
        if(is_recording)
        {
            can_modify_song = 0;
        }
    }
    
    Was_Song_Playing = Song_Playing;
    // Fix synchro problem...
    if(After_Song_Stop)
    {
        Was_Song_Playing = FALSE;
    }

    Gui_Draw_Button_Box(302, 28, 9, 16, I_, BUTTON_NORMAL | (can_modify_song ^ BUTTON_DISABLED) | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);
    Gui_Draw_Button_Box(313, 28, 9, 16, D_, BUTTON_NORMAL | (can_modify_song ^ BUTTON_DISABLED) | BUTTON_TEXT_CENTERED | BUTTON_SMALL_FONT);
    Gui_Draw_Arrows_Number_Box(324, 46, Beats_Per_Min, BUTTON_NORMAL | (!Was_Song_Playing ? 0 : BUTTON_DISABLED) | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    Gui_Draw_Arrows_Number_Box2(324, 64, Ticks_Per_Beat, BUTTON_NORMAL | (!Was_Song_Playing ? 0 : BUTTON_DISABLED) | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
    Gui_Draw_Arrows_Number_Box2(324, 28, Song_Tracks, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
    Gui_Draw_Arrows_Number_Box2(324, 28, Song_Tracks, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
}
            
// ------------------------------------------------------
// Display playing status
void Notify_Play(void)
{
    if(Song_Playing)
    {
        if(!play_pattern)
        {
            Gui_Draw_Button_Box(8, 28, 39, 16, "\04", BUTTON_PUSHED | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(49, 28, 39, 16, "\253", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
            Switch_Cmd_Playing(FALSE);
            Status_Box("Playing Song...", TRUE);
        }
        else
        {
            Gui_Draw_Button_Box(8, 28, 39, 16, "\04", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(49, 28, 39, 16, "\253", BUTTON_PUSHED | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
            Switch_Cmd_Playing(FALSE);
            Status_Box("Playing Pattern...", TRUE);
        }
    }
    else
    {
        Gui_Draw_Button_Box(8, 28, 39, 16, "\04", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
        Gui_Draw_Button_Box(49, 28, 39, 16, "\253", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
    }
}

// ------------------------------------------------------
// Stop replaying
void Song_Stop(void)
{
    Song_Playing = FALSE;
    Ptk_Stop();
    Gui_Draw_Button_Box(8, 28, 39, 16, "\04", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(49, 28, 39, 16, "\253", BUTTON_NORMAL | BUTTON_RIGHT_MOUSE | BUTTON_TEXT_CENTERED);
    Status_Box("Feeling Groovy.", TRUE);
    Song_Position = Song_Position_Visual;
    Pattern_Line = Pattern_Line_Visual;
    Actualize_Master(5);
    Update_Pattern(0);
}

// ------------------------------------------------------
// Create a new song
void New_Mod(void)
{
    int i;
    int Old_Prg;

    Clear_Input();
    Song_Stop();

#if !defined(__NO_MIDI__)
    Midi_Reset();
#endif

    Lock_Audio_Thread();
    Stop_Current_Instrument();

    if(ZzaappOMatic == ZZAAPP_ALL || ZzaappOMatic == ZZAAPP_INSTRUMENTS)
    {
        Free_Samples();
        Current_Instrument = 0;
        seditor = 0;
        Final_Mod_Length = 0;
        Actualize_Master(0);
        for(i = 0; i < MAX_INSTRS; i++)
        {
            Old_Prg = Synthprg[i];
            Kill_Instrument(i, TRUE);
            Synthprg[i] = Old_Prg;
            sprintf(nameins[i], "Untitled");
            if((Synthprg[i] - 2) == i)
            {
                Synthprg[i] = 1;
            }
        }
        Renew_Sample_Ed();
    }

    if(ZzaappOMatic == ZZAAPP_ALL || ZzaappOMatic == ZZAAPP_SYNTHS)
    {
        for(int ini = 0; ini < MAX_TRACKS; ini++)
        {
            for(i = 0; i < MAX_POLYPHONY; i++)
            {
                Synthesizer[ini][i].Reset();
            }
        }
        for(i = 0; i < MAX_INSTRS; i++)
        {
            Reset_Synth_Parameters(&PARASynth[i]);
            Synthprg[i] = 0;
        }
        Final_Mod_Length = 0;
        Actualize_Master(0);
        Current_Instrument = 0;
    }

    if(ZzaappOMatic == ZZAAPP_ALL || ZzaappOMatic == ZZAAPP_PATTERNS)
    {
        Song_Tracks = 6;
        for(int api = 0; api < MAX_ROWS; api++)
        {
            patternLines[api] = DEFAULT_PATTERN_LEN;
        }
        Clear_Patterns_Pool();
        nPatterns = 1;
        Set_Default_Channels_Polyphony();
        for(int inico = 0; inico < 256; inico++)
        {
            pSequence[inico] = 0;
            for(int inico2 = 0; inico2 < MAX_TRACKS; inico2++)
            {
                Chan_Active_State[inico][inico2] = TRUE;
                Chan_History_State[inico][inico2] = FALSE;
            }
        }

        Song_Length = 1;
        Track_Under_Caret = 0;
        Column_Under_Caret = 0;
        Pattern_Line = 0;
        Pattern_Line_Visual = 0;
        player_line = 0;
        Song_Position = 0;
        Song_Position_Visual = 0;
        rawrender_range = FALSE;
        rawrender_target = RENDER_TO_FILE;
        rawrender_from = 0;
        rawrender_to = 0;
        Final_Mod_Length = 0;
        Actualize_Sequencer();
        Actualize_Pattern_Ed();
        Actualize_Master(0);
        Update_Pattern(0);
        Reset_Tracks_To_Render();
        Display_Patterns_Sizes_Status();
    }

    if(ZzaappOMatic == ZZAAPP_ALL || ZzaappOMatic == ZZAAPP_303)
    {
        Reset_303_Parameters(&tb303[0]);
        Reset_303_Parameters(&tb303[1]);
        tb303engine[0].reset();
        tb303engine[1].reset();
        track3031 = 255;
        track3032 = 255;
    }

    if(ZzaappOMatic == ZZAAPP_ALL)
    {
        Use_Cubic = CUBIC_INT;
        sprintf(Selection_Name, "Untitled");
        sprintf(Reverb_Name, "Untitled");
        sprintf(Midi_Name, "Untitled");
        sprintf(name, "Untitled");
        sprintf(artist, "Somebody");
        sprintf(style, "Anything Goes");
        namesize = 8;
        Beats_Per_Min = 125;
        Ticks_Per_Beat = 4;
        DelayType = 1;
        mas_vol = 1.0f;
        Reverb_Filter_Cutoff = 0.08f;
        Reverb_Filter_Resonance = 0.5f;
        Reverb_Stereo_Amount = 63;
        Reverb_Damp = 1.0f;

        for(i = 0; i < MAX_TRACKS; i++)
        {
            mas_comp_threshold_Track[i] = 100.0f;
            mas_comp_ratio_Track[i] = 0.0f;
        }
        lchorus_feedback = 0.6f;
        rchorus_feedback = 0.5f;
        lchorus_delay = 10584;
        rchorus_delay = 15876;
        lchorus_counter = MIX_RATE;
        rchorus_counter = MIX_RATE;
        lchorus_counter2 = MIX_RATE - lchorus_delay;
        rchorus_counter2 = MIX_RATE - rchorus_delay;
        compressor = FALSE;
        c_threshold = 32;
        delay_time = 0;

        // Old preset by default
        Load_Old_Reverb_Presets(DelayType);
        Initreverb();
        for(int spl = 0; spl < MAX_TRACKS; spl++)
        {
            CCoef[spl] = float((float) CSend[spl] / 127.0f);
        }
        Pre_Song_Init();
    }

    Post_Song_Init();

    SubCounter = 0;
    PosInTick = 0;
    PosInTick_Delay = 0;

    allow_save = TRUE;

    Actualize_DiskIO_Ed(0);

    Reset_Song_Length();
    Display_Song_Length();

    Draw_Scope();

    Status_Box("Zzaapp Done.", TRUE);

    Refresh_UI_Context();

    Unlock_Audio_Thread();
}

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

// ------------------------------------------------------
// Change a name
char *table_newletter[71] =
{
    NULL, // 0
    "%sa",
    "%sb",
    "%sc",
    "%sd",
    "%se",
    "%sf",
    "%sg",
    "%sh",
    "%si",
    "%sj", // 10
    "%sk",
    "%sl",
    "%sm",
    "%sn",
    "%so",
    "%sp",
    "%sq",
    "%sr",
    "%ss",
    "%st", // 20
    "%su",
    "%sv",
    "%sw",
    "%sx",
    "%sy",
    "%sz",
    "%s0",
    "%s1",
    "%s2",
    "%s3",
    "%s4",
    "%s5",
    "%s6",
    "%s7",
    "%s8",
    "%s9",
    NULL, // 37
    "%s ", // 38
    NULL, // 39
    "%s.", // 40
    "%sA",
    "%sB",
    "%sC",
    "%sD",
    "%sE",
    "%sF",
    "%sG",
    "%sH",
    "%sI",
    "%sJ", // 50
    "%sK",
    "%sL",
    "%sM",
    "%sN",
    "%sO",
    "%sP",
    "%sQ",
    "%sR",
    "%sS",
    "%sT", // 60
    "%sU",
    "%sV",
    "%sW",
    "%sX",
    "%sY",
    "%sZ",
    "%s,", // 67
    NULL,  // 68
    "%s-", // 69
    NULL   // 70
};

// ------------------------------------------------------
// Change a letter in a string
void Actualize_Name(int *newletter, char *nam)
{
    int i;

    // Cancel input
    if(newletter[71])
    {
        newletter[71] = FALSE;
        strcpy(nam, cur_input_name);
        snamesel = INPUT_NONE;
        Keyboard_Nbr_Events = 0;
        return;
    }
    // Valid input
    if(newletter[39] && namesize > 0)
    {
        newletter[39] = FALSE;
        snamesel = INPUT_NONE;
        Keyboard_Nbr_Events = 0;
        return;
    }

    for(i = 0; i < 71; i++)
    {
        if(i == 37)
        {
            // Backspace
            if(newletter[i])
            {
                if(namesize > 0)
                {
                    nam[strlen(nam) - 1] = '\0';
                    if(namesize > 0) namesize--;
                }
                newletter[i] = FALSE;
            }
        }
        else
        {
            if(newletter[i])
            {
                if(namesize < 19)
                {
                    if(table_newletter[i])
                    {
                        sprintf(nam, table_newletter[i], nam);
                        namesize++;
                    }
                }
                newletter[i] = FALSE;
            }
        }
    }
}

// ------------------------------------------------------
// Render the song as a .wav file (or collection of files).
int Must_Render_Track(int Idx)
{
    if(rawrender_multi &&
       rawrender_target == RENDER_TO_FILE)
    {
        if(Tracks_To_Render[Idx])
        {
            return FALSE;
        }
    }
    return TRUE;
}

void Wav_Renderizer()
{
    play_pattern = FALSE;
    char buffer[MAX_PATH];
    char buffer_name[MAX_PATH];
    int Save_Chan_Mute_State[MAX_TRACKS];
    int i;
    int j;
    int max_tracks_to_render;
    int Max_Position;
    WaveFile RF[MAX_TRACKS];
    short *Sample_Buffer[2];
    int Pos_In_Memory;
    int Mem_Buffer_Size;
    int Stereo = 0;
    int do_multi;
    float save_sample_vol;
    long filesize = 0;
    int rendered_track = 0;

    if(rawrender_multi &&
       rawrender_target == RENDER_TO_FILE)
    {
        max_tracks_to_render = Song_Tracks;
        do_multi = TRUE;
        for(i = 0; i < MAX_TRACKS; i++)
        {
            Save_Chan_Mute_State[i] = Chan_Mute_State[i];
            Chan_Mute_State[i] = TRUE;
        }
    }
    else
    {
        max_tracks_to_render = 1;
        do_multi = FALSE;
        Song_Stop();
    }

    for(j = 0; j < max_tracks_to_render; j++)
    {
        if(Must_Render_Track(j))
        {
            rendered_track++;
            if(do_multi)
            {
                Song_Stop();
                sprintf(buffer_name, "%s_%x.wav", name, j);
            }
            else
            {
                sprintf(buffer_name, "%s.wav", name);
            }

            switch(rawrender_target)
            {
                case RENDER_TO_FILE:
                    if(RF[j].OpenForWrite(buffer_name, 44100, rawrender_32float ? 32 : 16, 2) != DDC_SUCCESS)
                    {
                        sprintf(buffer, "Can't Open '%s' File.", buffer_name);
                        Status_Box(buffer, TRUE);
                        return;
                    }
                    if(rawrender_range)
                    {
                        sprintf(buffer, "Rendering Selection To '%s' File. Please Wait...", buffer_name);
                    }
                    else
                    {
                        sprintf(buffer, "Rendering Module To '%s' File. Please Wait...", buffer_name);
                    }
                    break;

                case RENDER_TO_MONO:
                case RENDER_TO_STEREO:
                    if(rawrender_range)
                    {
                        sprintf(buffer, "Rendering Selection To Instrument %d (Split %d). Please Wait...",
                                Current_Instrument,
                                Current_Instrument_Split);
                    }
                    else
                    {
                        sprintf(buffer, "Rendering Module To Instrument %d (Split %d). Please Wait...",
                                Current_Instrument,
                                Current_Instrument_Split);
                    }
                    break;
            }
            Status_Box(buffer, TRUE);

            rawrender = TRUE;

            Pattern_Line = 0;
            if(rawrender_range)
            {
                Song_Position = rawrender_from;
                Max_Position = rawrender_to + 1;
            }
            else
            {
                Song_Position = 0;
                Max_Position = Song_Length;
            }

            filesize = 0;
            done = FALSE;

            if(!do_multi)
            {
                for(i = 0; i < MAX_TRACKS; i++)
                {
                    Save_Chan_Mute_State[i] = Chan_Mute_State[i];
                    Chan_Mute_State[i] = Tracks_To_Render[i];
                }
            }
            else
            {
                Chan_Mute_State[j] = FALSE;
            }
            reset_carriers = TRUE;

            switch(rawrender_target)
            {
                case RENDER_TO_FILE:
                    Song_Play();
                    while(Song_Position < Max_Position && done == FALSE)
                    {
                        Get_Player_Values();
                        if(rawrender_32float)
                        {
                            // [-1.0..1.0]
                            RF[j].WriteStereoFloatSample(left_float_render, right_float_render);
                            filesize += 8;
                        }
                        else
                        {
                            RF[j].WriteStereoSample(left_value, right_value);
                            filesize += 4;
                        }
                    }
                    RF[j].Close();
                    break;

                case RENDER_TO_STEREO:
                    Stereo = 1;

                case RENDER_TO_MONO:
                    Mem_Buffer_Size = AUDIO_Latency * 64;
                    Sample_Buffer[0] = (short *) malloc(Mem_Buffer_Size * 2);
                    memset(Sample_Buffer[0], 0, Mem_Buffer_Size * 2);
                    Sample_Buffer[1] = NULL;
                    if(Sample_Buffer[0])
                    {
                        if(Stereo)
                        {
                            Sample_Buffer[1] = (short *) malloc(Mem_Buffer_Size * 2);
                            if(!Sample_Buffer[1])
                            {
                                free(Sample_Buffer[0]);
                                goto Stop_WavRender;
                            }
                            memset(Sample_Buffer[1], 0, Mem_Buffer_Size * 2);
                        }
                        Pos_In_Memory = 0;
                        Song_Play();
                        while(Song_Position < Max_Position && done == FALSE)
                        {
                            if(!Mem_Buffer_Size)
                            {
                                // Increase the size of our buffer
                                Mem_Buffer_Size = (AUDIO_Latency * 64);
                                Sample_Buffer[0] = (short *) realloc(Sample_Buffer[0], (Pos_In_Memory + Mem_Buffer_Size) * 2);
                                if(Stereo)
                                {
                                    Sample_Buffer[1] = (short *) realloc(Sample_Buffer[1], (Pos_In_Memory + Mem_Buffer_Size) * 2);
                                }
                            }
                            Get_Player_Values();
                            if(Stereo)
                            {
                                Sample_Buffer[0][Pos_In_Memory] = left_float_render * 32767.0f;
                                Sample_Buffer[1][Pos_In_Memory] = right_float_render * 32767.0f;
                                filesize += 4;
                            }
                            else
                            {
                                // Mix both channels
                                Sample_Buffer[0][Pos_In_Memory] = ((left_float_render * 0.5f) +
                                                                   (right_float_render * 0.5f)) * 32767.0f;
                                filesize += 2;
                            }
                            Pos_In_Memory++;
                            Mem_Buffer_Size--;
                        }
                        save_sample_vol = 1.0f;
                        if(Get_Number_Of_Splits(Current_Instrument)) save_sample_vol = Sample_Vol[Current_Instrument];

                        Allocate_Wave(Current_Instrument, Current_Instrument_Split,
                                     Pos_In_Memory, (Stereo + 1), TRUE,
                                     NULL, NULL);

                        // Fixup the buffer with it's real size
                        memcpy(RawSamples[Current_Instrument][0][Current_Instrument_Split], 
                               Sample_Buffer[0], (Pos_In_Memory * 2));
                        if(Stereo)
                        {
                            memcpy(RawSamples[Current_Instrument][1][Current_Instrument_Split], 
                                   Sample_Buffer[1], (Pos_In_Memory * 2));
                            free(Sample_Buffer[1]);
                        }
                        free(Sample_Buffer[0]);
                        LoopStart[Current_Instrument][Current_Instrument_Split] = 0;
                        LoopEnd[Current_Instrument][Current_Instrument_Split] = 0;
                        LoopType[Current_Instrument][Current_Instrument_Split] = SMP_LOOP_NONE;
                        Basenote[Current_Instrument][Current_Instrument_Split] = DEFAULT_BASE_NOTE;
                        Renew_Sample_Ed();
                        if(Get_Number_Of_Splits(Current_Instrument) == 1)
                        {
                            Sample_Vol[Current_Instrument] = save_sample_vol;
                        }
                    }
                    break;
            }
            if(do_multi)
            {
                Chan_Mute_State[j] = TRUE;
            }
        }
    }

Stop_WavRender:
    Song_Stop();
    reset_carriers = TRUE;

    Post_Song_Init();
    Song_Playing = FALSE;

    if(!do_multi)
    {
        for(i = 0; i < MAX_TRACKS; i++)
        {
            Chan_Mute_State[i] = Save_Chan_Mute_State[i];
        }
    }
    else
    {
        for(i = 0; i < MAX_TRACKS; i++)
        {
            Chan_Mute_State[i] = Save_Chan_Mute_State[i];
        }
    }

    int minutes;
    int seconds;

    switch(rawrender_target)
    {
        case RENDER_TO_FILE:
            minutes = filesize / 10584000;
            seconds = (filesize - minutes * 10584000) / 176400;
            if(do_multi)
            {
                sprintf(buffer, "Wav Rendering Finished. File Size: %.2f Megabytes Per File (Total: %.2f Megabytes). Playback Time: %d'%d''.",
                                float(filesize / 1048576.0f), float(filesize / 1048576.0f) * rendered_track, minutes, seconds);
            }
            else
            {
                sprintf(buffer, "Wav Rendering Finished. File Size: %.2f Megabytes. Playback Time: %d'%d''.",
                                float(filesize / 1048576.0f), minutes, seconds);
            }
            break;
        case RENDER_TO_STEREO:
            minutes = filesize / 10584000;
            seconds = (filesize - minutes * 10584000) / 176400;
            sprintf(buffer, "Wav Rendering Finished. Waveform Size: %.2f Megabytes. Playback Time: %d'%d''.",
                            float(filesize / 1048576.0f), minutes, seconds);
            break;
        default:
            minutes = (filesize / 10584000) * 3;
            seconds = ((filesize - minutes * 10584000) / 176400) * 3;
            sprintf(buffer, "Wav Rendering Finished. Waveform Size: %.2f Megabytes. Playback Time: %d'%d''.",
                            float(filesize / 1048576.0f), minutes, seconds);
            break;
    }

    // Return to the start as all the values will be trashed anyway.
    Pattern_Line = 0;
    Song_Position = 0;
    Song_Position_Visual = 0;
    Pattern_Line_Visual = 0;
    Actualize_DiskIO_Ed(0);
    rawrender = FALSE;

    last_index = -1;
    Read_SMPT();
    Actualize_Files_List(0);

    Status_Box(buffer, TRUE);
    Update_Pattern(0);
}

// ------------------------------------------------------
// Delete an instrument
void Delete_Instrument(void)
{
    int i;
    int Old_Prg;

    Lock_Audio_Thread();
    Stop_Current_Instrument();

    if(ZzaappOMatic == ZZAAPP_ALL || ZzaappOMatic == ZZAAPP_SYNTHS)
    {
        for(int ini = 0; ini < MAX_TRACKS; ini++)
        {
            for(i = 0; i < MAX_POLYPHONY; i++)
            {
                Synthesizer[ini][i].Reset();
            }
        }
        Reset_Synth_Parameters(&PARASynth[Current_Instrument]);
        Synthprg[Current_Instrument] = 0;
        Actualize_Master(0);
        Final_Mod_Length = 0;
        Actualize_Synth_Ed(UPDATE_SYNTH_ED_ALL);
        Status_Box("Synth Deleted.", TRUE);
    }

    if(ZzaappOMatic == ZZAAPP_SPLIT)
    {
        seditor = 0;
        Final_Mod_Length = 0;
        Actualize_Master(0);
        Old_Prg = Synthprg[Current_Instrument];
        Kill_Instrument(Current_Instrument, FALSE);
        Synthprg[Current_Instrument] = Old_Prg;
        sprintf(nameins[Current_Instrument], "Untitled");
        if((Synthprg[Current_Instrument] - 2) == Current_Instrument)
        {
            Synthprg[Current_Instrument] = 1;
        }
        Renew_Sample_Ed();
        Status_Box("Instrument Deleted.", TRUE);
        Refresh_Sample();
        Actualize_Master(0);
    }

    if(ZzaappOMatic == ZZAAPP_ALL || ZzaappOMatic == ZZAAPP_INSTRUMENTS)
    {
        seditor = 0;
        Final_Mod_Length = 0;
        Actualize_Master(0);
        Old_Prg = Synthprg[Current_Instrument];
        Kill_Instrument(Current_Instrument, TRUE);
        Synthprg[Current_Instrument] = Old_Prg;
        sprintf(nameins[Current_Instrument], "Untitled");
        if((Synthprg[Current_Instrument] - 2) == Current_Instrument)
        {
            Synthprg[Current_Instrument] = 1;
        }
        Renew_Sample_Ed();
        Status_Box("Instrument Deleted.", TRUE);
        Refresh_Sample();
        Actualize_Master(0);
    }
    Actualize_Instruments_Synths_List(0);
    Unlock_Audio_Thread();
}

void Stop_Current_Instrument(void)
{
    int i;
    int u;

    Current_Pointer[0] = 0;
    Current_Pointer[1] = 0;
    Current_Pointer[2] = 0;
    Current_Pointer[3] = 0;
    for(u = 0; u < MAX_TRACKS; u++)
    {
        for(i = 0; i < MAX_POLYPHONY; i++)
        {
            if(sp_channelsample[u][i] == Current_Instrument)
            {
                sp_Stage[u][i] = PLAYING_NOSAMPLE;
                sp_Position[u][i].absolu = 0;
            }
        }
        Player_FD[u] = 0;
        ramper[u] = 0;
    }
}

// ------------------------------------------------------
// Refresh the view of a sample
void Refresh_Sample(void)
{
    seditor = 0;
    Current_Instrument_Split = 0;
    if(userscreen == USER_SCREEN_INSTRUMENT_EDIT)
    {
        Draw_Instrument_Ed();
        Actualize_Instrument_Ed(2, 0);
    }
}

// ------------------------------------------------------
// Reset the channels polyphonies  to their default state
void Set_Default_Channels_Polyphony(void)
{
    int i;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        Channels_Polyphony[i] = DEFAULT_POLYPHONY;
    }
}

// ------------------------------------------------------
// Return the number of synths
int Get_Nbr_Synths(void)
{
    int i;
    int nbr_synth = 0;

    for(i = 0; i < MAX_INSTRS; i++)
    {
        if(Synthprg[i])
        {
            nbr_synth++;
        }
    }
    return nbr_synth;
}

// ------------------------------------------------------
// Return the number of instruments
int Get_Instrs_Data(int *samp_size)
{
    int nbr_samp = 0;

    if(samp_size != NULL)
    {
        *samp_size = 0;
    }

    for(int pp = 0; pp < MAX_INSTRS; pp++)
    {
        for(int z = 0; z < MAX_INSTRS_SPLITS; z++)
        {
            if(SampleType[pp][z] != 0)
            {
                if(samp_size != NULL)
                {
                    *samp_size += Sample_Channels[pp][z] * Sample_Length[pp][z];
                }
                nbr_samp++;
            }
        }
    }
    return nbr_samp;
}

// ------------------------------------------------------
// Show song informations (summing instruments and patterns)
void ShowInfo(void)
{
    char tmp[256];
    int patt_size;
    int nbr_samp;
    int nbr_synth;
    int samp_size;

    patt_size = nPatterns * PATTERN_LEN;
    nbr_synth = Get_Nbr_Synths();
    nbr_samp = Get_Instrs_Data(&samp_size);

    sprintf(tmp, "%d Instruments (%d Bytes), "
                 "%d Synths, "
                 "%d Patterns (%d Bytes).",
                 nbr_samp, samp_size,
                 nbr_synth, 
                 nPatterns, patt_size);
    Status_Box(tmp, TRUE);
}

// ------------------------------------------------------
// Function called each second mainly for autosaving purposes
Uint32 Timer_CallBack(Uint32 interval, void *param)
{
    if(Req_TimeOut)
    {
        Req_Timer++;
        if(Req_Timer > Req_TimeOut) Req_Pressed_Button = 1;
    }

    if(!Status_Box_Wait && Status_Box_Wait_Done == 0)
    {
        Status_Box_Wait_Done = 1;
    }
    if(Status_Box_Wait)
    {
        Status_Box_Wait--;
    }

    // Don't save during a requester
    if(AutoSave && Current_Requester == NULL)
    {
        wait_AutoSave++;
        // Autosave module
        if(wait_AutoSave > Values_AutoSave[AutoSave])
        {
            Pack_Module(name);
        }
    }
    return(interval);
}

// ------------------------------------------------------
// Validate or cancel an user input
void Actualize_Input(void)
{
    switch(snamesel)
    {
        // Module name
        case INPUT_MODULE_NAME:
            Actualize_Name(retletter, name);
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
            break;

        // Instrument name
        case INPUT_INSTRUMENT_NAME:
            Actualize_Name(retletter, nameins[Current_Instrument]);
            gui_action = GUI_CMD_UPDATE_PATTERN_ED;
            break;

        // Synth name
        case INPUT_SYNTH_NAME:
            Actualize_Name(retletter, PARASynth[Current_Instrument].Preset_Name);
            teac = UPDATE_SYNTH_CHANGE_NAME;
            gui_action = GUI_CMD_UPDATE_SYNTH_ED;
            break;

        // Module artist
        case INPUT_MODULE_ARTIST:
            Actualize_Name(retletter, artist);
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
            break;

        // Module style
        case INPUT_MODULE_STYLE:
            Actualize_Name(retletter, style);
            gui_action = GUI_CMD_UPDATE_DISKIO_ED;
            break;

        // 303 pattern
        case INPUT_303_PATTERN:
            Actualize_Name(retletter, tb303[sl3].pattern_name[tb303[sl3].selectedpattern]);
            teac = 18;
            gui_action = GUI_CMD_UPDATE_MIDI_303_ED;
            break;

        // Reverb
        case INPUT_REVERB_NAME:
            Actualize_Name(retletter, Reverb_Name);
            teac = UPDATE_REVERB_ED_CHANGE_NAME;
            gui_action = GUI_CMD_UPDATE_REVERB_ED;
            break;

        // Selection
        case INPUT_SELECTION_NAME:
            Actualize_Name(retletter, Selection_Name);
            teac = 3;
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
            break;

        // Reverb
        case INPUT_MIDI_NAME:
            Actualize_Name(retletter, Midi_Name);
            teac = UPDATE_MIDI_ED_CHANGE_NAME;
            gui_action = GUI_CMD_UPDATE_MIDI_ED;
            break;

    }
}

// ------------------------------------------------------
// Return the channel and sub channel of a played note
LPJAZZ_KEY Get_Jazz_Key_Off(JAZZ_KEY Pool[MAX_TRACKS][MAX_POLYPHONY], int Note)
{
    int i;
    int j;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        for(j = 0; j < MAX_POLYPHONY; j++)
        {
            if(Pool[i][j].Note == Note)
            {
                return(&Pool[i][j]);
            }
        }
    }
    return(NULL);
}

// ------------------------------------------------------
// Return the channel and sub channel of a played note
int Discard_Key_Note_Off(JAZZ_KEY Pool[MAX_TRACKS][MAX_POLYPHONY], int Channel, int Sub_Channel)
{
    int i;
    int j;
    int discarded = 0;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        for(j = 0; j < MAX_POLYPHONY; j++)
        {
            if(Pool[i][j].Channel == Channel && Pool[i][j].Sub_Channel == Sub_Channel)
            {
                Pool[i][j].Sub_Channel = 0;
                Pool[i][j].Channel = 0;
                Pool[i][j].Note = 0;
                discarded++;
            }
        }
    }
    return(discarded);
}

int Raw_Keys_to_Send[] =
{
    0x10,
    3,
    0x11,
    4,
    0x12,
    0x13,
    6,
    0x14,
    7,
    0x15,
    8,
    0x16,
    0x17,
    0xa,
    0x18,
    0xb,
    0x19,
    0x1a,
    0xd,
    0x1b,
    0x2c,
    0x1f,
    0x2d,
    0x20,
    0x2e,
    0x2f,
    0x22,
    0x30,
    0x23,
    0x31,
    0x24,
    0x32,
    0x33,
    0x26,
    0x34,
    0x27,
    0x35
};

// ------------------------------------------------------
// KeyBoard ShortCut Handler
void Keyboard_Handler(void)
{
    int Cur_Position = Get_Song_Position();
    int Done_Value;
    int data;
    int validating_input = FALSE;

    // Exit tracker
    if(Get_LAlt() && !Get_LCtrl() && Keys[SDLK_F4]) gui_action = GUI_CMD_EXIT;
    // Lower octave
    if(!Get_LShift() && !Get_LCtrl() && Keys[SDLK_F1]) gui_action = GUI_CMD_LOWER_OCTAVE;
    // Higher octave
    if(!Get_LShift() && !Get_LCtrl() && Keys[SDLK_F2]) gui_action = GUI_CMD_HIGHER_OCTAVE;
    // Helper
    if(!Get_LShift() && !Get_LCtrl() && Keys[SDLK_F3]) gui_action = GUI_CMD_GET_HELP;

    if(Get_LCtrl() && !Get_LShift() && Keys[SDLK_F2]) gui_action = GUI_CMD_DECREASE_STEP_ADD;
    if(Get_LCtrl() && !Get_LShift() && Keys[SDLK_F4]) gui_action = GUI_CMD_INCREASE_STEP_ADD;

    if(Get_LShift() && !Get_LCtrl())
    {
        if(Keys[SDLK_ESCAPE])
        {
            gui_action = GUI_CMD_SELECT_LARGE_PATTERNS;
        }
        if(Keys[SDLK_F1])
        {
            gui_action = GUI_CMD_SELECT_SEQUENCER;
        }
        if(Keys[SDLK_F2])
        {
            gui_action = GUI_CMD_SELECT_INSTRUMENT_EDIT;
        }
        if(Keys[SDLK_F3])
        {
            gui_action = GUI_CMD_SELECT_SYNTH_EDIT;
        }
        if(Keys[SDLK_F4])
        {
            gui_action = GUI_CMD_SELECT_SAMPLE_EDIT;
        }
        if(Keys[SDLK_F5])
        {
            gui_action = GUI_CMD_SELECT_TB303_EDIT;
        }
        if(Keys[SDLK_F6])
        {
            gui_action = GUI_CMD_SELECT_TRACK_EDIT;
        }
        if(Keys[SDLK_F7])
        {
            gui_action = GUI_CMD_SELECT_TRACK_FX_EDIT;
        }
        if(Keys[SDLK_F8])
        {
            gui_action = GUI_CMD_SELECT_FX_EDIT;
        }
        if(Keys[SDLK_F9])
        {
            gui_action = GUI_CMD_SELECT_REVERB_EDIT;
        }
        if(Keys[SDLK_F10])
        {
            gui_action = GUI_CMD_SELECT_DISKIO_EDIT;
        }
        if(Keys[SDLK_F11])
        {
            gui_action = GUI_CMD_SELECT_SCREEN_SETUP_EDIT;
        }
        if(Keys[SDLK_F12])
        {
            gui_action = GUI_CMD_SELECT_MIDI_SETUP;
        }
    }

    // Previous track
    if(Get_LShift() && !Get_LCtrl() && Keys[SDLK_TAB])
    {
        Unselect_Selection();
        Track_Under_Caret--;
        Column_Under_Caret = 0;
        Update_Pattern(0);
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }

    // Next track
    if(!Get_LShift() && !Get_LCtrl() && Keys[SDLK_TAB])
    {
        Unselect_Selection();
        Track_Under_Caret++;
        if(Track_Under_Caret >= Song_Tracks)
        {
            Track_Under_Caret = 0;
        }
        Column_Under_Caret = 0;
        Update_Pattern(0);
        gui_action = GUI_CMD_SET_FOCUS_TRACK;
    }

    // Next note
#if defined(__WIN32__)
    if(!Get_LShift() && Get_LCtrl() && Keys[SDLK_TAB] && !Key_Unicode)
#endif
#if defined(__LINUX__)
    if(!Get_LShift() && Get_LCtrl() && Keys[SDLK_TAB] && Keys_Sym[SDLK_TAB])
#else
    if(!Get_LShift() && Get_LCtrl() && Keys[SDLK_TAB])
#endif
    {
        Unselect_Selection();
        Column_Under_Caret += Table_Right_Tab_Notes[Column_Under_Caret];
        if(Column_Under_Caret >= (Channels_MultiNotes[Track_Under_Caret] * 3)) Column_Under_Caret = 0;
        Update_Pattern(0);
    }

    // Previous note
    if(Get_LShift() && Get_LCtrl() && Keys[SDLK_TAB])
    {
        Unselect_Selection();
        Column_Under_Caret -= Table_Left_Tab_Notes[Column_Under_Caret];
        if(Column_Under_Caret < 0) Column_Under_Caret = (Channels_MultiNotes[Track_Under_Caret] * 3) - 3;
        Update_Pattern(0);
    }

    if(!is_recording)
    {
        if(!Get_LShift())
        {
            // Jump to row 0
            if(Keys[SDLK_F5])
            {
                if(Pattern_Line != 0) Goto_Row(0);
            }

            // Jump to row 16
            if(Keys[SDLK_F6])
            {
                int line = patternLines[pSequence[Cur_Position]] / 4;
                if(line > patternLines[pSequence[Cur_Position]] - 1) line = patternLines[pSequence[Cur_Position]] - 1;
                if(Pattern_Line != line) Goto_Row(line);
            }

            // Jump to row 32
            if(Keys[SDLK_F7])
            {
                int line = patternLines[pSequence[Cur_Position]] / 4;
                line *= 2;
                if(line > patternLines[pSequence[Cur_Position]] - 1) line = patternLines[pSequence[Cur_Position]] - 1;
                if(Pattern_Line != line) Goto_Row(line);
            }

            // Jump to row 48
            if(Keys[SDLK_F8])
            {
                int line = patternLines[pSequence[Cur_Position]] / 4;
                line *= 3;
                if(line > patternLines[pSequence[Cur_Position]] - 1) line = patternLines[pSequence[Cur_Position]] - 1;
                if(Pattern_Line != line) Goto_Row(line);
            }

            // Jump to row 63
            if(Keys[SDLK_F9])
            {
                int line = patternLines[pSequence[Cur_Position]] / 4;
                line *= 4;
                if(line > patternLines[pSequence[Cur_Position]] - 1) line = patternLines[pSequence[Cur_Position]] - 1;
                if(Pattern_Line != line) Goto_Row(line);
            }
        }

        // Top left of pattern
        if(Keys[SDLK_HOME])
        {
            Goto_Top_Left();
        }

        // Bottom right of pattern
        if(Keys[SDLK_END])
        {
            Goto_Bottom_Right();
        }

        // Previous column or previous track
        if(Keys[SDLK_LEFT] && !Get_LCtrl() && !Get_LAlt() && !Get_RShift())
        {
            Goto_Previous_Column();
        }

        // Next column or next track
        if(Keys[SDLK_RIGHT] && !Get_LCtrl() && !Get_LAlt() && !Get_RShift())
        {
            Goto_Next_Column();
        }

        // Decrease slider value by 1
        if(Keys[SDLK_RIGHT] && !Get_LCtrl() && !Get_LAlt() && Get_RShift() && pattern_sliders && can_modify_song)
        {
            Set_Slider_Value(-1);
        }

        // Increase slider value by 1
        if(Keys[SDLK_LEFT] && !Get_LCtrl() && !Get_LAlt() && Get_RShift() && pattern_sliders && can_modify_song)
        {
            Set_Slider_Value(1);
        }

        // Decrease slider value by 10
        if(Keys[SDLK_DOWN] && !Get_LCtrl() && !Get_LAlt() && Get_RShift() && pattern_sliders && can_modify_song)
        {
            Set_Slider_Value(-10);
        }

        // Increase slider value by 10
        if(Keys[SDLK_UP] && !Get_LCtrl() && !Get_LAlt() && Get_RShift() && pattern_sliders && can_modify_song)
        {
            Set_Slider_Value(10);
        }

        // Previous row
        if(Keys[SDLK_UP] && !Song_Playing && !Get_RShift())
        {
            Goto_Previous_Row(TRUE);
        }

        // Next row
        if(Keys[SDLK_DOWN] && !Song_Playing && !Get_RShift())
        {
            Goto_Next_Row(TRUE);
        }

        // Previous page (16 rows)
        if(Keys[SDLK_PAGEUP] && !Song_Playing)
        {
            Goto_Previous_Page(TRUE);
        }

        // Next page (16 rows)
        if(Keys[SDLK_PAGEDOWN] && !Song_Playing)
        {
            Goto_Next_Page(TRUE);
        }

        // Previous pattern
        if((Keys_Sym[SDLK_KP_MINUS]) && pSequence[Cur_Position] > 0 && can_modify_song)
        {
            gui_action = GUI_CMD_PREV_PATT;
        }

        // Next pattern
        if((Keys_Sym[SDLK_KP_PLUS]) && pSequence[Cur_Position] < 254 && can_modify_song)
        {
            gui_action = GUI_CMD_NEXT_PATT;
        }

        if(Keys[SDLK_INSERT] && can_modify_song && !Get_LCtrl())
        {
            // INSERT
            if(Get_LShift())
            {
                Insert_Pattern_Line(Cur_Position);
            }
            else
            {
                Insert_Selection(Track_Under_Caret, Cur_Position);
            }
        }

        if(snamesel == INPUT_NONE)
        {
            if(Keys[SDLK_BACKSPACE] && can_modify_song  && !Get_LCtrl())
            {
                // BACKSPACE
                if(Get_LShift())
                {
                    if(Pattern_Line)
                    {
                        Pattern_Line--;
                        Remove_Pattern_Line(Cur_Position);
                    }
                }
                else
                {
                    Remove_Selection(Track_Under_Caret, Cur_Position);
                }
            }
        }
    }

    // Lower octave
    if((Keys_Sym[SDLK_KP_DIVIDE]))
    {
        gui_action = GUI_CMD_LOWER_OCTAVE;
        Keys_Sym[SDLK_KP_DIVIDE] = FALSE;
    }
    // Upper octave
    if((Keys_Sym[SDLK_KP_MULTIPLY]))
    {
        gui_action = GUI_CMD_HIGHER_OCTAVE;
        Keys_Sym[SDLK_KP_MULTIPLY] = FALSE;
    }

    int retnote = NOTE_OFF;
    int retnote_raw = NOTE_OFF;
    int retvalue = NOTE_OFF;
    int reelletter = FALSE;

    // -------------------------------------------
    // Editing a name
    if(snamesel > INPUT_NONE)
    {
        if(Keys[SDLK_a])
        {
            retletter[1] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_b])
        {
            retletter[2] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_c])
        {
            retletter[3] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_d])
        {
            retletter[4] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_e])
        {
            retletter[5] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_f])
        {
            retletter[6] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_g])
        {
            retletter[7] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_h])
        {
            retletter[8] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_i])
        {
            retletter[9] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_j])
        {
            retletter[10] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_k])
        {
            retletter[11] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_l])
        {
            retletter[12] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_m])
        {
            retletter[13] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_n])
        {
            retletter[14] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_o])
        {
            retletter[15] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_p])
        {
            retletter[16] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_q])
        {
            retletter[17] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_r])
        {
            retletter[18] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_s])
        {
            retletter[19] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_t])
        {
            retletter[20] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_u])
        {
            retletter[21] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_v])
        {
            retletter[22] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_w])
        {
            retletter[23] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_x])
        {
            retletter[24] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_y])
        {
            retletter[25] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_z])
        {
            retletter[26] = TRUE;
            reelletter = TRUE;
        }

        if(Keys['A'])
        {
            retletter[41] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['B'])
        {
            retletter[42] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['C'])
        {
            retletter[43] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['D'])
        {
            retletter[44] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['E'])
        {
            retletter[45] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['F'])
        {
            retletter[46] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['G'])
        {
            retletter[47] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['H'])
        {
            retletter[48] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['I'])
        {
            retletter[49] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['J'])
        {
            retletter[50] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['K'])
        {
            retletter[51] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['L'])
        {
            retletter[52] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['M'])
        {
            retletter[53] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['N'])
        {
            retletter[54] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['O'])
        {
            retletter[55] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['P'])
        {
            retletter[56] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['Q'])
        {
            retletter[57] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['R'])
        {
            retletter[58] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['S'])
        {
            retletter[59] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['T'])
        {
            retletter[60] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['U'])
        {
            retletter[61] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['V'])
        {
            retletter[62] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['W'])
        {
            retletter[63] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['X'])
        {
            retletter[64] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['Y'])
        {
            retletter[65] = TRUE;
            reelletter = TRUE;
        }
        if(Keys['Z'])
        {
            retletter[66] = TRUE;
            reelletter = TRUE;
        }

        if(Keys[SDLK_KP0])
        {
            retletter[27] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['0'])
        {
            retletter[27] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP1])
        {
            retletter[28] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['1'])
        {
            retletter[28] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP2])
        {
            retletter[29] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['2'])
        {
            retletter[29] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP3])
        {
            retletter[30] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['3'])
        {
            retletter[30] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP4])
        {
            retletter[31] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['4'])
        {
            retletter[31] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP5])
        {
            retletter[32] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['5'])
        {
            retletter[32] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP6])
        {
            retletter[33] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['6'])
        {
            retletter[33] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP7])
        {
            retletter[34] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['7'])
        {
            retletter[34] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP8])
        {
            retletter[35] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['8'])
        {
            retletter[35] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_KP9])
        {
            retletter[36] = TRUE;
            reelletter = TRUE;
        }
        if(Keys_Unicode['9'])
        {
            retletter[36] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_BACKSPACE])
        {
            retletter[37] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_SPACE])
        {
            retletter[38] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_RETURN])
        {
            retletter[39] = TRUE;
            reelletter = TRUE;
            validating_input = TRUE;
        }
        if(Keys[SDLK_KP_ENTER])
        {
            retletter[39] = TRUE;
            reelletter = TRUE;
            validating_input = TRUE;
        }
        if(Keys[SDLK_PERIOD])
        {
            retletter[40] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_COMMA])
        {
            retletter[67] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_SLASH])
        {
            retletter[68] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_MINUS])
        {
            retletter[69] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_EQUALS])
        {
            retletter[70] = TRUE;
            reelletter = TRUE;
        }
        if(Keys[SDLK_ESCAPE])
        {
            retletter[71] = TRUE;
            reelletter = TRUE;
        }
    }

    if(reelletter)
    {
        Actualize_Input();
    }
    // -------------------------------------------

    int Shift = 0;
    if(Get_Caps())
    {
        if(!Get_LShift() && !Get_RShift())
        {
            Shift = UNICODE_OFFSET1;
        }
    }
    else
    {
        if(Get_LShift() || Get_RShift())
        {
            Shift = UNICODE_OFFSET1;
        }
    }

    if(snamesel == INPUT_NONE && !reelletter && !Get_LCtrl())
    {
        // Data columns
        if(Keys_Unicode[SDLK_0]) retvalue = 0;
        if(Keys[SDLK_KP0]) retvalue = 0;
        if(Keys_Unicode[SDLK_1]) retvalue = 1;
        if(Keys[SDLK_KP1]) retvalue = 1;
        if(Keys_Unicode[SDLK_2]) retvalue = 2;
        if(Keys[SDLK_KP2]) retvalue = 2;
        if(Keys_Unicode[SDLK_3]) retvalue = 3;
        if(Keys[SDLK_KP3]) retvalue = 3;
        if(Keys_Unicode[SDLK_4]) retvalue = 4;
        if(Keys[SDLK_KP4]) retvalue = 4;
        if(Keys_Unicode[SDLK_5]) retvalue = 5;
        if(Keys[SDLK_KP5]) retvalue = 5;
        if(Keys_Unicode[SDLK_6]) retvalue = 6;
        if(Keys[SDLK_KP6]) retvalue = 6;
        if(Keys_Unicode[SDLK_7]) retvalue = 7;
        if(Keys[SDLK_KP7]) retvalue = 7;
        if(Keys_Unicode[SDLK_8]) retvalue = 8;
        if(Keys[SDLK_KP8]) retvalue = 8;
        if(Keys_Unicode[SDLK_9]) retvalue = 9;
        if(Keys[SDLK_KP9]) retvalue = 9;
        if(Keys[SDLK_a - Shift])
		{
            retvalue = 10;
        }
        if(Keys[SDLK_b - Shift]) retvalue = 11;
        if(Keys[SDLK_c - Shift]) retvalue = 12;
        if(Keys[SDLK_d - Shift]) retvalue = 13;
        if(Keys[SDLK_e - Shift]) retvalue = 14;
        if(Keys[SDLK_f - Shift]) retvalue = 15;
        if(Keys[SDLK_DELETE]) retvalue = 16;
    }

    // ------------------------------------------
    if(!Keys[SDLK_MENU] && !Get_LAlt() && !Get_LCtrl() && !Get_LShift() && snamesel == INPUT_NONE && !reelletter)
    {
        // Key jazz release
        if(Keys_Raw_Off[0x10]) { Record_Keys[0] = (12 + 1) | 0x80;  Keys_Raw_Off[0x10] = FALSE; Keys_Raw[0x10] = FALSE; }
        if(Keys_Raw_Off[3])    { Record_Keys[1] = (13 + 1) | 0x80;  Keys_Raw_Off[3] = FALSE; Keys_Raw[3] = FALSE; }
        if(Keys_Raw_Off[0x11]) { Record_Keys[2] = (14 + 1) | 0x80;  Keys_Raw_Off[0x11] = FALSE; Keys_Raw[0x11] = FALSE; }
        if(Keys_Raw_Off[4])    { Record_Keys[3] = (15 + 1) | 0x80;  Keys_Raw_Off[4] = FALSE; Keys_Raw[4] = FALSE; }
        if(Keys_Raw_Off[0x12]) { Record_Keys[4] = (16 + 1) | 0x80;  Keys_Raw_Off[0x12] = FALSE; Keys_Raw[0x12] = FALSE; }
        if(Keys_Raw_Off[0x13]) { Record_Keys[5] = (17 + 1) | 0x80;  Keys_Raw_Off[0x13] = FALSE; Keys_Raw[0x13] = FALSE; }
        if(Keys_Raw_Off[6])    { Record_Keys[6] = (18 + 1) | 0x80;  Keys_Raw_Off[6] = FALSE; Keys_Raw[6] = FALSE; }
        if(Keys_Raw_Off[0x14]) { Record_Keys[7] = (19 + 1) | 0x80;  Keys_Raw_Off[0x14] = FALSE; Keys_Raw[0x14] = FALSE; }
        if(Keys_Raw_Off[7])    { Record_Keys[8] = (20 + 1) | 0x80;  Keys_Raw_Off[7] = FALSE; Keys_Raw[7] = FALSE; }
        if(Keys_Raw_Off[0x15]) { Record_Keys[9] = (21 + 1) | 0x80;  Keys_Raw_Off[0x15] = FALSE; Keys_Raw[0x15] = FALSE; }
        if(Keys_Raw_Off[8])    { Record_Keys[10] = (22 + 1) | 0x80; Keys_Raw_Off[8] = FALSE; Keys_Raw[8] = FALSE; }
        if(Keys_Raw_Off[0x16]) { Record_Keys[11] = (23 + 1) | 0x80; Keys_Raw_Off[0x16] = FALSE; Keys_Raw[0x16] = FALSE; }
        if(Keys_Raw_Off[0x17]) { Record_Keys[12] = (24 + 1) | 0x80; Keys_Raw_Off[0x17] = FALSE; Keys_Raw[0x17] = FALSE; }
        if(Keys_Raw_Off[0xa])  { Record_Keys[13] = (25 + 1) | 0x80; Keys_Raw_Off[0xa] = FALSE; Keys_Raw[0xa] = FALSE; }
        if(Keys_Raw_Off[0x18]) { Record_Keys[14] = (26 + 1) | 0x80; Keys_Raw_Off[0x18] = FALSE; Keys_Raw[0x18] = FALSE; }
        if(Keys_Raw_Off[0xb])  { Record_Keys[15] = (27 + 1) | 0x80; Keys_Raw_Off[0xb] = FALSE; Keys_Raw[0xb] = FALSE; }
        if(Keys_Raw_Off[0x19]) { Record_Keys[16] = (28 + 1) | 0x80; Keys_Raw_Off[0x19] = FALSE; Keys_Raw[0x19] = FALSE; }
        if(Keys_Raw_Off[0x1a]) { Record_Keys[17] = (29 + 1) | 0x80; Keys_Raw_Off[0x1a] = FALSE; Keys_Raw[0x1a] = FALSE; }
        if(Keys_Raw_Off[0xd])  { Record_Keys[18] = (30 + 1) | 0x80; Keys_Raw_Off[0xd] = FALSE; Keys_Raw[0xd] = FALSE; }
        if(Keys_Raw_Off[0x1b]) { Record_Keys[19] = (31 + 1) | 0x80; Keys_Raw_Off[0x1b] = FALSE; Keys_Raw[0x1b] = FALSE; }
        if(Keys_Raw_Off[0x2c]) { Record_Keys[20] = (0 + 1) | 0x80;  Keys_Raw_Off[0x2c] = FALSE; Keys_Raw[0x2c] = FALSE; }
        if(Keys_Raw_Off[0x1f]) { Record_Keys[21] = (1 + 1) | 0x80;  Keys_Raw_Off[0x1f] = FALSE; Keys_Raw[0x1f] = FALSE; }
        if(Keys_Raw_Off[0x2d]) { Record_Keys[22] = (2 + 1) | 0x80;  Keys_Raw_Off[0x2d] = FALSE; Keys_Raw[0x2d] = FALSE; }
        if(Keys_Raw_Off[0x20]) { Record_Keys[23] = (3 + 1) | 0x80;  Keys_Raw_Off[0x20] = FALSE; Keys_Raw[0x20] = FALSE; }
        if(Keys_Raw_Off[0x2e]) { Record_Keys[24] = (4 + 1) | 0x80;  Keys_Raw_Off[0x2e] = FALSE; Keys_Raw[0x2e] = FALSE; }
        if(Keys_Raw_Off[0x2f]) { Record_Keys[25] = (5 + 1) | 0x80;  Keys_Raw_Off[0x2f] = FALSE; Keys_Raw[0x2f] = FALSE; }
        if(Keys_Raw_Off[0x22]) { Record_Keys[26] = (6 + 1) | 0x80;  Keys_Raw_Off[0x22] = FALSE; Keys_Raw[0x22] = FALSE; }
        if(Keys_Raw_Off[0x30]) { Record_Keys[27] = (7 + 1) | 0x80;  Keys_Raw_Off[0x30] = FALSE; Keys_Raw[0x30] = FALSE; }
        if(Keys_Raw_Off[0x23]) { Record_Keys[28] = (8 + 1) | 0x80;  Keys_Raw_Off[0x23] = FALSE; Keys_Raw[0x23] = FALSE; }
        if(Keys_Raw_Off[0x31]) { Record_Keys[29] = (9 + 1) | 0x80;  Keys_Raw_Off[0x31] = FALSE; Keys_Raw[0x31] = FALSE; }
        if(Keys_Raw_Off[0x24]) { Record_Keys[30] = (10 + 1) | 0x80; Keys_Raw_Off[0x24] = FALSE; Keys_Raw[0x24] = FALSE; }
        if(Keys_Raw_Off[0x32]) { Record_Keys[31] = (11 + 1) | 0x80; Keys_Raw_Off[0x32] = FALSE; Keys_Raw[0x32] = FALSE; }
        if(Keys_Raw_Off[0x33]) { Record_Keys[32] = (12 + 1) | 0x80; Keys_Raw_Off[0x33] = FALSE; Keys_Raw[0x33] = FALSE; }
        if(Keys_Raw_Off[0x26]) { Record_Keys[33] = (13 + 1) | 0x80; Keys_Raw_Off[0x26] = FALSE; Keys_Raw[0x26] = FALSE; }
        if(Keys_Raw_Off[0x34]) { Record_Keys[34] = (14 + 1) | 0x80; Keys_Raw_Off[0x34] = FALSE; Keys_Raw[0x34] = FALSE; }
        if(Keys_Raw_Off[0x27]) { Record_Keys[35] = (15 + 1) | 0x80; Keys_Raw_Off[0x27] = FALSE; Keys_Raw[0x27] = FALSE; }
        if(Keys_Raw_Off[0x35]) { Record_Keys[36] = (16 + 1) | 0x80; Keys_Raw_Off[0x35] = FALSE; Keys_Raw[0x35] = FALSE; }

        if(Keys[SDLK_DELETE])
        {
            retnote = -1;
            retnote_raw = -1;
        }
        if(Keys[SDLK_INSERT])
        {
            retnote = -2;
            retnote_raw = -2;
        }
        if(Keys[SDLK_RSHIFT])
        {
            retnote = -3;
            retnote_raw = -3;
        }

        // Key jazz
        if(Keys_Raw[0x10]) { retnote_raw = 12; Record_Keys[0] = 12 + 1; }
        if(Keys_Raw[3])    { retnote_raw = 13; Record_Keys[1] = 13 + 1; }
        if(Keys_Raw[0x11]) { retnote_raw = 14; Record_Keys[2] = 14 + 1; }
        if(Keys_Raw[4])    { retnote_raw = 15; Record_Keys[3] = 15 + 1; }
        if(Keys_Raw[0x12]) { retnote_raw = 16; Record_Keys[4] = 16 + 1; }
        if(Keys_Raw[0x13]) { retnote_raw = 17; Record_Keys[5] = 17 + 1; }
        if(Keys_Raw[6])    { retnote_raw = 18; Record_Keys[6] = 18 + 1; }
        if(Keys_Raw[0x14]) { retnote_raw = 19; Record_Keys[7] = 19 + 1; }
        if(Keys_Raw[7])    { retnote_raw = 20; Record_Keys[8] = 20 + 1; }
        if(Keys_Raw[0x15]) { retnote_raw = 21; Record_Keys[9] = 21 + 1; }
        if(Keys_Raw[8])    { retnote_raw = 22; Record_Keys[10] = 22 + 1; }
        if(Keys_Raw[0x16]) { retnote_raw = 23; Record_Keys[11] = 23 + 1; }
        if(Keys_Raw[0x17]) { retnote_raw = 24; Record_Keys[12] = 24 + 1; }
        if(Keys_Raw[0xa])  { retnote_raw = 25; Record_Keys[13] = 25 + 1; }
        if(Keys_Raw[0x18]) { retnote_raw = 26; Record_Keys[14] = 26 + 1; }
        if(Keys_Raw[0xb])  { retnote_raw = 27; Record_Keys[15] = 27 + 1; }
        if(Keys_Raw[0x19]) { retnote_raw = 28; Record_Keys[16] = 28 + 1; }
        if(Keys_Raw[0x1a]) { retnote_raw = 29; Record_Keys[17] = 29 + 1; }
        if(Keys_Raw[0xd])  { retnote_raw = 30; Record_Keys[18] = 30 + 1; }
        if(Keys_Raw[0x1b]) { retnote_raw = 31; Record_Keys[19] = 31 + 1; }
        if(Keys_Raw[0x2c]) { retnote_raw = 0; Record_Keys[20] = 0 + 1; }
        if(Keys_Raw[0x1f]) { retnote_raw = 1; Record_Keys[21] = 1 + 1; }
        if(Keys_Raw[0x2d]) { retnote_raw = 2; Record_Keys[22] = 2 + 1; }
        if(Keys_Raw[0x20]) { retnote_raw = 3; Record_Keys[23] = 3 + 1; }
        if(Keys_Raw[0x2e]) { retnote_raw = 4; Record_Keys[24] = 4 + 1; }
        if(Keys_Raw[0x2f]) { retnote_raw = 5; Record_Keys[25] = 5 + 1; }
        if(Keys_Raw[0x22]) { retnote_raw = 6; Record_Keys[26] = 6 + 1; }
        if(Keys_Raw[0x30]) { retnote_raw = 7; Record_Keys[27] = 7 + 1; }
        if(Keys_Raw[0x23]) { retnote_raw = 8; Record_Keys[28] = 8 + 1; }
        if(Keys_Raw[0x31]) { retnote_raw = 9; Record_Keys[29] = 9 + 1; }
        if(Keys_Raw[0x24]) { retnote_raw = 10; Record_Keys[30] = 10 + 1; }
        if(Keys_Raw[0x32]) { retnote_raw = 11; Record_Keys[31] = 11 + 1; }
        if(Keys_Raw[0x33]) { retnote_raw = 12; Record_Keys[32] = 12 + 1; }
        if(Keys_Raw[0x26]) { retnote_raw = 13; Record_Keys[33] = 13 + 1; }
        if(Keys_Raw[0x34]) { retnote_raw = 14; Record_Keys[34] = 14 + 1; }
        if(Keys_Raw[0x27]) { retnote_raw = 15; Record_Keys[35] = 15 + 1; }
        if(Keys_Raw[0x35]) { retnote_raw = 16; Record_Keys[36] = 16 + 1; }

        int i;

        if(is_recording)
        {
            // Start the real recording
            if(!is_recording_2 && (retnote_raw < NOTE_OFF && retnote_raw > 0) && key_record_first_time)
            {
                // Start recording
                is_recording_2 = 1;
                Nbr_Sub_Note_Off = 0;
                is_record_key = FALSE;
                is_editing = TRUE;
                L_MaxLevel = 0;
                R_MaxLevel = 0;
                Pattern_Line_Visual = Pattern_Line;
                key_record_first_time = FALSE;
                old_key_Pattern_Line = Pattern_Line;
                Notify_Edit(FALSE);
                Clear_Midi_Channels_Pool();
                player_pos = -1;
                metronome_rows_counter = 0;
                Post_Song_Init();
                Ptk_Play();
            }
        }

        int Record_Key_Pressed = FALSE;
        for(i = 0; i < 37; i++)
        {
            if(Record_Keys[i])
            {
                Record_Key_Pressed = TRUE;
            }
        }

        if(Record_Key_Pressed)
        {
            for(i = 0; i < 37; i++)
            {
                if(Record_Keys[i])
                {
                    int tmp_note = ((Record_Keys[i] & 0x7f) - 1) + Current_Octave * 12;
                    if(tmp_note > NOTE_MAX) tmp_note = NOTE_MAX;

                    if(!(Record_Keys[i] & 0x80))
                    {
                        // Note on
                        if(Record_Keys_State[i] == FALSE)
                        {
                            Record_Keys_State[i] = TRUE;

                            // Write it
                            if(is_recording_2)
                            {
                                Send_Note(Raw_Keys_to_Send[i], FALSE, FALSE);
                            }

                            // Play it
                            Note_Jazz(Track_Under_Caret, tmp_note, 1.0f);
                        }
                    }
                    else
                    {
                        // Note off
                        if(Record_Keys_State[i] == TRUE)
                        {
                            Record_Keys_State[i] = FALSE;
                            Record_Keys[i] = 0;

                            // Do not reach that point
                            if(is_recording_2)
                            {
                                Send_Note(Raw_Keys_to_Send[i] | 0x80, FALSE, FALSE);
                            }

                            Note_Jazz_Off(tmp_note);
                        }
                    }
                }
            }
        }
    }

    // Turn edit mode on/off
    if(Keys[SDLK_SPACE] && snamesel == INPUT_NONE && pos_space == 1)
    {
        if(Get_LShift())
        {
            gui_action = GUI_CMD_RECORD_MODE;
        }
        else
        {
            gui_action = GUI_CMD_EDIT_MODE;
        }
    }

    if(!Get_LAlt() && !Get_RAlt() && snamesel == INPUT_NONE && !validating_input && !is_recording)
    {
        if(Enter_Notification)
        {
            if(Enter_Notified)
            {
                po_ctrl2 = FALSE;
                Enter_Notified = FALSE;
                play_pattern = FALSE;
                reset_carriers = FALSE;
                if(Enter_Notified_Play_Pattern)
                {
                    play_pattern = TRUE;
                }
                gui_action = GUI_CMD_PLAY_SONG;
            }
        }
        else
        {
            if(Enter_Notified)
            {
                Enter_Notified = FALSE;
                is_recording = 0;
                is_recording_2 = 0;
                Nbr_Sub_Note_Off = 0;
                Song_Stop();
            }
        }
    }

    if(pos_space == 0 && !Keys[SDLK_SPACE])
    {
        pos_space = 1;
    }

    if(!is_recording)
    {
        // Play song
        if(Keys[SDLK_RCTRL] && snamesel == INPUT_NONE && po_ctrl2)
        {
            play_pattern = FALSE;
            po_ctrl2 = FALSE;
            reset_carriers = FALSE;
            if(!Get_LShift())
            {
                reset_carriers = TRUE;
                Pattern_Line = 0;
            }
            gui_action = GUI_CMD_PLAY_SONG;
        }
        if(!Keys[SDLK_RCTRL] && !po_ctrl2) po_ctrl2 = TRUE;

        if(Keys[SDLK_RALT] && snamesel == INPUT_NONE && po_alt2)
        {
            play_pattern = TRUE;
            po_alt2 = FALSE;
            reset_carriers = FALSE;
            if(!Get_LShift())
            {
                reset_carriers = TRUE;
                Pattern_Line = 0;
            }
            gui_action = GUI_CMD_PLAY_SONG;
        }
        if(!Keys[SDLK_RALT] && !po_alt2) po_alt2 = TRUE;

        // Play song
        if(Get_LAlt() && snamesel == INPUT_NONE && !po_alt)
        {
            po_alt = TRUE;
        }
        if(!Get_LAlt() && po_alt) po_alt = FALSE;

        if(Key_Unicode)
        {
            if(Get_LShift() && !Get_LCtrl())
            {
                if(Keys[SDLK_m - UNICODE_OFFSET1])
                {
                    if(Chan_Mute_State[Track_Under_Caret] == FALSE) Chan_Mute_State[Track_Under_Caret] = TRUE;
                    else Chan_Mute_State[Track_Under_Caret] = FALSE;
                    if(userscreen == USER_SCREEN_TRACK_EDIT) Actualize_Track_Ed(10);
                    Update_Pattern(0);
                }

                // Display instruments list
                if(Keys[SDLK_i - UNICODE_OFFSET1])
                {
                    Scopish = SCOPE_ZONE_INSTR_LIST;
                    Draw_Scope_Files_Button();
                }

                // Display synths list
                if(Keys[SDLK_s - UNICODE_OFFSET1])
                {
                    Scopish = SCOPE_ZONE_SYNTH_LIST;
                    Draw_Scope_Files_Button();
                }
            }
        }

        if(Get_LCtrl())
        {
            if(!Get_LAlt() && !Get_LShift())
            {
                // Select the copy buffer
                if(Keys[SDLK_1] || Keys[SDLK_KP1])
                {
                    Curr_Buff_Block = 0;
                    Draw_Pattern_Right_Stuff();
                    Update_Pattern(0);
                }
                if(Keys[SDLK_2] || Keys[SDLK_KP2])
                {
                    Curr_Buff_Block = 1;
                    Draw_Pattern_Right_Stuff();
                    Update_Pattern(0);
                }
                if(Keys[SDLK_3] || Keys[SDLK_KP3])
                {
                    Curr_Buff_Block = 2;
                    Draw_Pattern_Right_Stuff();
                    Update_Pattern(0);
                }
                if(Keys[SDLK_4] || Keys[SDLK_KP4])
                {
                    Curr_Buff_Block = 3;
                    Draw_Pattern_Right_Stuff();
                    Update_Pattern(0);
                }

                if(Keys[SDLK_LEFT])
                {
                    if(pSequence[Cur_Position] > 0 && can_modify_song)
                    {
                        gui_action = GUI_CMD_PREV_PATT;
                    }
                    Keys[SDLK_LEFT] = FALSE;
                }
                if(Keys[SDLK_RIGHT])
                {
                    if(pSequence[Cur_Position] < 254 && can_modify_song)
                    {
                        gui_action = GUI_CMD_NEXT_PATT;
                    }
                    Keys[SDLK_RIGHT] = FALSE;
                }
            }

            if(Key_Unicode)
            {
                if(Get_LShift())
                {
                    // Solo track
                    if(Keys[SDLK_m - UNICODE_OFFSET2])
                    {
                        Mute_Track(Track_Under_Caret);
                        // Will unmute the correct track
                        if(Chan_Mute_State[Track_Under_Caret] == FALSE) Chan_Mute_State[Track_Under_Caret] = TRUE;
                        else Chan_Mute_State[Track_Under_Caret] = FALSE;
                        if(userscreen == USER_SCREEN_TRACK_EDIT) Actualize_Track_Ed(10);
                        Update_Pattern(0);
                    }

                    // Fill the values of a selected block with step add
                    if(Keys[SDLK_f - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Fill_Block(Cur_Position, Current_Edit_Steps);
                    }

                    // Randomize the values of a selected block with step add
                    if(Keys[SDLK_r - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Randomize_Block(Cur_Position, Current_Edit_Steps);

                    }
                    // Transpose the selection 1 semitone higher
                    if(Keys[SDLK_u - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Instrument_Semitone_Up_Block(Cur_Position);
                    }

                    // Transpose the selection 1 semitone lower
                    if(Keys[SDLK_d - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Instrument_Semitone_Down_Block(Cur_Position);
                    }

                    // Transpose the selection 1 octave higher
                    if(Keys[SDLK_k - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Instrument_Octave_Up_Block(Cur_Position);
                    }

                    // Transpose the selection 1 octave lower
                    if(Keys[SDLK_l - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Instrument_Octave_Down_Block(Cur_Position);
                    }

                    // Select the current track or the current pattern
                    if(Keys[SDLK_a - UNICODE_OFFSET2])
                    {
                        Select_Pattern_Block();
                    }
                }
                else
                {
                    // Save
                    if(Keys[SDLK_s - UNICODE_OFFSET2])
                    {
                        if(File_Exist_Req("%s" SLASH "%s.ptk", Dir_Mods, name))
                        {
                            Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_MODULE, NULL, TRUE);
                        }
                        else
                        {
                            gui_action = GUI_CMD_SAVE_MODULE;
                        }
                    }

                    // Cut selected block
                    if(Keys[SDLK_x - UNICODE_OFFSET2] &&
                       block_start_track[Curr_Buff_Block] != -1 &&
                       block_end_track[Curr_Buff_Block] != -1 && can_modify_song)
                    {
                        Cut_Selection(Cur_Position);
                    }

                    // Copy selected block
                    if(Keys[SDLK_c - UNICODE_OFFSET2] &&
                       block_start_track[Curr_Buff_Block] != -1 &&
                       block_end_track[Curr_Buff_Block] != -1)
                    {
                        Copy_Selection(Cur_Position);
                    }

                    // Interpolate the values of a selected block
                    if(Keys[SDLK_i - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Interpolate_Block(Cur_Position);
                    }

                    // Randomize the values of a selected block
                    if(Keys[SDLK_r - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Randomize_Block(Cur_Position, 1);
                    }

                    // Fill the values of a selected block
                    if(Keys[SDLK_f - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Fill_Block(Cur_Position, 1);
                    }

                    // Save a selected block or a whole pattern
                    if(Keys[SDLK_w - UNICODE_OFFSET2])
                    {
                        if(File_Exist_Req("%s" SLASH "%s.ppb", Dir_Patterns, Selection_Name))
                        {
                            Display_Requester(&Overwrite_Requester, GUI_CMD_SAVE_PATTERN, NULL, TRUE);
                        }
                        else
                        {
                            gui_action = GUI_CMD_SAVE_PATTERN;
                        }
                    }

                    // Paste the block buffer into a pattern
                    if(Keys[SDLK_v - UNICODE_OFFSET2] &&
					   block_start_track_nibble[Curr_Buff_Block] != -1 &&
					   block_end_track_nibble[Curr_Buff_Block] != -1 &&
					   can_modify_song)
                    {
                        Paste_Block(Cur_Position, Paste_Across, TRUE);
                    }

                    // Transpose the selection 1 semitone higher
                    if(Keys[SDLK_u - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Semitone_Up_Block(Cur_Position);
                    }

                    // Transpose the selection 1 semitone lower
                    if(Keys[SDLK_d - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Semitone_Down_Block(Cur_Position);
                    }

                    // Transpose the selection 1 octave higher
                    if(Keys[SDLK_k - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Octave_Up_Block(Cur_Position);
                    }

                    // Transpose the selection 1 octave lower
                    if(Keys[SDLK_l - UNICODE_OFFSET2] && can_modify_song)
                    {
                        Reset_Transpose_Block();
                        Octave_Down_Block(Cur_Position);
                    }

                    // Select the current track or the current pattern
                    if(Keys[SDLK_a - UNICODE_OFFSET2])
                    {
                        Select_Track_Block();
                    }
                }
            }
        }

        if(Get_LAlt())
        {
            if(Get_LCtrl())
            {
                // Previous position in sequence
                if(Keys[SDLK_LEFT])
                {
                    gui_action = GUI_CMD_PREVIOUS_POSITION;
                    Keys[SDLK_LEFT] = FALSE;
                }

                // Next position in sequence
                if(Keys[SDLK_RIGHT])
                {
                    gui_action = GUI_CMD_NEXT_POSITION;
                    Keys[SDLK_RIGHT] = FALSE;
                }
            }
            else
            {
                // Previous instrument
                if(Keys[SDLK_LEFT])
                {
                    gui_action = GUI_CMD_PREV_INSTR;
                    Keys[SDLK_LEFT] = FALSE;
                }

                // Next instrument
                if(Keys[SDLK_RIGHT])
                {
                    gui_action = GUI_CMD_NEXT_INSTR;
                    Keys[SDLK_RIGHT] = FALSE;
                }

                // Select the current note/instrument
                if(Keys[SDLK_a])
                {
                    Select_Note_Block();
                }

                // Select the current track or the current pattern
                if(Keys[SDLK_a - UNICODE_OFFSET1])
                {
                    if(Get_LShift()) Select_All_Notes_Block();
                }

            }
        }

        Done_Value = FALSE;
        if(!Get_LAlt())
        {
            if(retvalue != NOTE_OFF && is_editing)
            {
                int ped_cell;
                int i;
                int j = (Channels_MultiNotes[Track_Under_Caret] - 1) * 3;

                // Instrument
                for(i = 0; i < Channels_MultiNotes[Track_Under_Caret]; i++)
                {
                    if(Column_Under_Caret == 1 + (i * 3))
                    {
                        i++;
                        break;
                    }
                }
                i--;

                // Odd
                if(Column_Under_Caret == (1 + (i * 3)) ||
                   Column_Under_Caret == (3 + j) ||
                   Column_Under_Caret == (5 + j) ||
                   Column_Under_Caret == (7 + j) ||
                   Column_Under_Caret == (9 + j) ||
                   Column_Under_Caret == (11 + j) ||
                   Column_Under_Caret == (13 + j) ||
                   Column_Under_Caret == (15 + j) ||
                   Column_Under_Caret == (17 + j) ||
                   Column_Under_Caret == (19 + j) ||
                   Column_Under_Caret == (21 + j))
                {
                    ped_cell = PATTERN_INSTR1 + (i * 2);                                // instrument
                    if(Column_Under_Caret == (3 + j)) ped_cell = PATTERN_VOLUME;       	// volume
                    if(Column_Under_Caret == (5 + j)) ped_cell = PATTERN_PANNING;      	// panning
                    if(Column_Under_Caret == (7 + j)) ped_cell = PATTERN_FX;           	// fx
                    if(Column_Under_Caret == (9 + j)) ped_cell = PATTERN_FXDATA;       	// fx data
                    if(Column_Under_Caret == (11 + j)) ped_cell = PATTERN_FX2;         	// fx 2
                    if(Column_Under_Caret == (13 + j)) ped_cell = PATTERN_FXDATA2;     	// fx 2 data
                    if(Column_Under_Caret == (15 + j)) ped_cell = PATTERN_FX3;          // fx 3
                    if(Column_Under_Caret == (17 + j)) ped_cell = PATTERN_FXDATA3;     	// fx 3 data
                    if(Column_Under_Caret == (19 + j)) ped_cell = PATTERN_FX4;         	// fx 4
                    if(Column_Under_Caret == (21 + j)) ped_cell = PATTERN_FXDATA4;     	// fx 4 data
                    
                    switch(ped_cell)
                    {
                        case PATTERN_FXDATA:
                        case PATTERN_FXDATA2:
                        case PATTERN_FXDATA3:
                        case PATTERN_FXDATA4:
                            data = Get_Column_Data_With_Track(Channels_MultiNotes, Channels_Effects, Cur_Position,
                                                              Track_Under_Caret, Column_Under_Caret - 2, Pattern_Line);
                            if(data == 0)
                            {
                                Done_Value = TRUE;
                                goto Cannot_Modify_Odd;
                            }
                            break;
                    }

                    ltretvalue = retvalue;
                    xoffseted = (Track_Under_Caret * PATTERN_BYTES) + (Pattern_Line * PATTERN_ROW_LEN) + ped_cell;

                    int oldval = *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted);

                    if(retvalue < 16)
                    {
                        if(oldval == 255 && Column_Under_Caret == (1 + (i * 3))) oldval = 0;
                        if(oldval == 255 && Column_Under_Caret == (3 + j)) oldval = 0;
                        if(oldval == 255 && Column_Under_Caret == (5 + j)) oldval = 0;
                        oldval = (oldval & 0xf) + (retvalue << 4);
                        *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = oldval;

                        // Max panning
                        if(oldval != 255 && Column_Under_Caret == (5 + j) &&
                           *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) != 0x90)
                        {
                            if(oldval != 255 && Column_Under_Caret == (5 + j) &&
                            *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 0x80)
                            {
                                *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 0x80;
                            }
                        }

                        // Max instrument
                        if(oldval != 255 && Column_Under_Caret == (1 + (i * 3)) &&
                           *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 0x7f)
                        {
                            *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 0x7f;
                        }

                        if(!is_recording) if(!Song_Playing)
                        {
                            Pattern_Line += Current_Edit_Steps;
                            if(!Current_Edit_Steps)
                            {
                                Goto_Next_Column();
                                Done_Value = TRUE;
                            }
                        }
                    }
                    else
                    {
                        if(!Delete_Selection(Cur_Position))
                        {
                            oldval = 0;
                            if(Column_Under_Caret == (1 + (i * 3))) oldval = 255;
                            if(Column_Under_Caret == (3 + j)) oldval = 255;
                            if(Column_Under_Caret == (5 + j)) oldval = 255;
                            *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = oldval;

                            // Max panning
                            if(oldval != 255 && Column_Under_Caret == (5 + j) &&
                               *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) != 0x90)
                            {
                                if(oldval != 255 && Column_Under_Caret == (5 + j) &&
                                   *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 0x80)
                                {
                                    *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 0x80;
                                }
                            }

                            // Max instrument
                            if(oldval != 255 && Column_Under_Caret == (1 + (i * 3)) &&
                               *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 0x7f)
                            {
                                *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 0x7f;
                            }

                            if(!is_recording) if(!Song_Playing)
                            {
                                Pattern_Line += Current_Edit_Steps;
                                if(!Current_Edit_Steps)
                                {
                                    Goto_Next_Column();
                                    Done_Value = TRUE;
                                }
                            }
                        }
                    }
                    Update_Pattern(0);
                }
Cannot_Modify_Odd:

                if(!Done_Value)
                {
                    for(i = 0; i < Channels_MultiNotes[Track_Under_Caret]; i++)
                    {
                        if(Column_Under_Caret == 2 +(i * 3))
                        {
                            i++;
                            break;
                        }
                    }
                    i--;
                    // Even
                    if(Column_Under_Caret == (2 + (i * 3)) ||
                       Column_Under_Caret == (4 + j) ||
                       Column_Under_Caret == (6 + j) ||
                       Column_Under_Caret == (8 + j) ||
                       Column_Under_Caret == (10 + j) ||
                       Column_Under_Caret == (12 + j) ||
                       Column_Under_Caret == (14 + j) ||
                       Column_Under_Caret == (16 + j) ||
                       Column_Under_Caret == (18 + j) ||
                       Column_Under_Caret == (20 + j) ||
                       Column_Under_Caret == (22 + j))
                    {
                        ped_cell = PATTERN_INSTR1 + (i * 2);
                        if(Column_Under_Caret == (4 + j)) ped_cell = PATTERN_VOLUME;
                        if(Column_Under_Caret == (6 + j)) ped_cell = PATTERN_PANNING;
                        if(Column_Under_Caret == (8 + j)) ped_cell = PATTERN_FX;
                        if(Column_Under_Caret == (10 + j)) ped_cell = PATTERN_FXDATA;
                        if(Column_Under_Caret == (12 + j)) ped_cell = PATTERN_FX2;
                        if(Column_Under_Caret == (14 + j)) ped_cell = PATTERN_FXDATA2;
                        if(Column_Under_Caret == (16 + j)) ped_cell = PATTERN_FX3;
                        if(Column_Under_Caret == (18 + j)) ped_cell = PATTERN_FXDATA3;
                        if(Column_Under_Caret == (20 + j)) ped_cell = PATTERN_FX4;
                        if(Column_Under_Caret == (22 + j)) ped_cell = PATTERN_FXDATA4;

                        ltretvalue = retvalue;
                        xoffseted = (Track_Under_Caret * PATTERN_BYTES) + (Pattern_Line * PATTERN_ROW_LEN) + ped_cell;
                        int oldval = *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted);

                            switch(ped_cell)
                            {
                                case PATTERN_FXDATA:
                                case PATTERN_FXDATA2:
                                case PATTERN_FXDATA3:
                                case PATTERN_FXDATA4:
                                    data = Get_Column_Data_With_Track(Channels_MultiNotes, Channels_Effects, Cur_Position,
                                                                      Track_Under_Caret, Column_Under_Caret - 3, Pattern_Line);
                                    if(data == 0)
                                    {
                                        Done_Value = TRUE;
                                        goto Cannot_Modify_Even;
                                    }
                                    break;
                            }

                        if(retvalue < 16)
                        {
                            if(oldval == 255 && Column_Under_Caret == (2 + (i * 3))) oldval = 0;
                            if(oldval == 255 && Column_Under_Caret == (4 + j)) oldval = 0;
                            if(oldval == 255 && Column_Under_Caret == (6 + j)) oldval = 0;
                            oldval = (oldval & 0xf0) + retvalue;
                            *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = oldval;

                            // Max panning
                            if(oldval != 255 && Column_Under_Caret == (6 + j) &&
                               *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) != 0x90)
                            {
                                if(oldval != 255 && Column_Under_Caret == (6 + j) &&
                                   *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 0x80)
                                {
                                    *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 0x80;
                                }
                            }

                            // Max instrument
                            if(oldval != 255 && Column_Under_Caret == (2 + (i * 3)) &&
                               *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 127)
                            {
                                *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 127;
                            }

                            if(!is_recording) if(!Song_Playing)
                            {
                                Pattern_Line += Current_Edit_Steps;
                                if(!Current_Edit_Steps)
                                {
                                    Goto_Next_Column();
                                }
                            }
                        }
                        else
                        {
                            if(!Delete_Selection(Cur_Position))
                            {
                                oldval = 0;
                                if(Column_Under_Caret == (2 + (i * 3))) oldval = 255;
                                if(Column_Under_Caret == (4 + j)) oldval = 255;
                                if(Column_Under_Caret == (6 + j)) oldval = 255;
                                *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = oldval;

                                // Max panning
                                if(oldval != 255 && Column_Under_Caret == (6 + j) &&
                                   *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) != 0x90)
                                {
                                    if(oldval != 255 && Column_Under_Caret == (6 + j) &&
                                       *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 0x80)
                                    {
                                        *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 0x80;
                                    }
                                }

                                // Max instrument
                                if(oldval != 255 && Column_Under_Caret == (2 + (i * 3)) &&
                                   *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) > 0x7f)
                                {
                                    *(RawPatterns + pSequence[Cur_Position] * PATTERN_LEN + xoffseted) = 0x7f;
                                }

                                if(!is_recording) if(!Song_Playing)
                                {
                                    Pattern_Line += Current_Edit_Steps;
                                    if(!Current_Edit_Steps)
                                    {
                                        Goto_Next_Column();
                                    }
                                }
                            }
                        }
                        Update_Pattern(0);
                    }
                }
Cannot_Modify_Even:;
            }
        }
    }

    // --------------------------------------------
    // Enter one or several notes (used for midi and record as well as simple editing)
    if(!Get_LAlt() && !Get_LCtrl() && !Get_LShift() && snamesel == INPUT_NONE && !reelletter)
    {
        int go_note = FALSE;

        int i;
        for(i = 0; i < Keyboard_Nbr_Events; i++)
        {
            int Idx = Keyboard_Events[i];

            if(!Keyboard_Notes_Type[i])
            {
                // Key editing
                switch(Idx & 0x7f)
                {
                    case 0x10:
                    case 3:
                    case 0x11:
                    case 4:
                    case 0x12:
                    case 0x13:
                    case 6:
                    case 0x14:
                    case 7:
                    case 0x15:
                    case 8:
                    case 0x16:
                    case 0x17:
                    case 0xa:
                    case 0x18:
                    case 0xb:
                    case 0x19:
                    case 0x1a:
                    case 0xd:
                    case 0x1b:
                    case 0x2c:
                    case 0x1f:
                    case 0x2d:
                    case 0x20:
                    case 0x2e:
                    case 0x2f:
                    case 0x22:
                    case 0x30:
                    case 0x23:
                    case 0x31:
                    case 0x24:
                    case 0x32:
                    case 0x33:
                    case 0x26:
                    case 0x34:
                    case 0x27:
                    case 0x35:
                        go_note = TRUE;
                        break;
                }
            }
            else
            {
                go_note = TRUE;
            }
        }
        // Write a note
        if(go_note)
        {
            int column = 0;
            int in_note = FALSE;

            // Check if this is a legal note column
            for(i = 0; i < Channels_MultiNotes[Track_Under_Caret]; i++)
            {
                if(Column_Under_Caret == (i * 3))
                {
                    in_note = TRUE;
                    break;
                }
                column += 2;
            }

            if(in_note)
            {
                int i;
                int move_down = FALSE;
                for(i = 0; i < Keyboard_Nbr_Events; i++)
                {
                    int Idx = Keyboard_Events[i];
                    int pos;
                    int tmp_note;
                    int line;

                    if(!Keyboard_Notes_Type[i])
                    {
                        // Key editing
                        switch(Idx & 0x7f)
                        {
                            case 0x10: { retnote_raw = 12; break; }
                            case 3:    { retnote_raw = 13; break; }
                            case 0x11: { retnote_raw = 14; break; }
                            case 4:    { retnote_raw = 15; break; }
                            case 0x12: { retnote_raw = 16; break; }
                            case 0x13: { retnote_raw = 17; break; }
                            case 6:    { retnote_raw = 18; break; }
                            case 0x14: { retnote_raw = 19; break; }
                            case 7:    { retnote_raw = 20; break; }
                            case 0x15: { retnote_raw = 21; break; }
                            case 8:    { retnote_raw = 22; break; }
                            case 0x16: { retnote_raw = 23; break; }
                            case 0x17: { retnote_raw = 24; break; }
                            case 0xa:  { retnote_raw = 25; break; }
                            case 0x18: { retnote_raw = 26; break; }
                            case 0xb:  { retnote_raw = 27; break; }
                            case 0x19: { retnote_raw = 28; break; }
                            case 0x1a: { retnote_raw = 29; break; }
                            case 0xd:  { retnote_raw = 30; break; }
                            case 0x1b: { retnote_raw = 31; break; }
                            case 0x2c: { retnote_raw = 0; break; }
                            case 0x1f: { retnote_raw = 1; break; }
                            case 0x2d: { retnote_raw = 2; break; }
                            case 0x20: { retnote_raw = 3; break; }
                            case 0x2e: { retnote_raw = 4; break; }
                            case 0x2f: { retnote_raw = 5; break; }
                            case 0x22: { retnote_raw = 6; break; }
                            case 0x30: { retnote_raw = 7; break; }
                            case 0x23: { retnote_raw = 8; break; }
                            case 0x31: { retnote_raw = 9; break; }
                            case 0x24: { retnote_raw = 10; break; }
                            case 0x32: { retnote_raw = 11; break; }
                            case 0x33: { retnote_raw = 12; break; }
                            case 0x26: { retnote_raw = 13; break; }
                            case 0x34: { retnote_raw = 14; break; }
                            case 0x27: { retnote_raw = 15; break; }
                            case 0x35: { retnote_raw = 16; break; }
                            default:
                                goto No_Key;
                        }
                        tmp_note = retnote_raw + (Current_Octave * 12);
                        if(tmp_note > NOTE_MAX) tmp_note = NOTE_MAX;
                    }
                    else
                    {
                        tmp_note = Idx & 0x7f;
                    }

                    line = Pattern_Line;
                    if(is_recording_2 || Song_Playing) line = Pattern_Line_Visual;

                    if(!(Idx & 0x80))
                    {
                        // Note
                        if(retnote > -1)
                        {
                            xoffseted = Get_Pattern_Offset(pSequence[Cur_Position], Track_Under_Caret, line);

                            // Select the sub channel
                            pos = i;

                            if(is_recording_2 || is_editing)
                            {
                                // Store the note column where it was entered
                                Sub_Channels_Note_Off[Track_Under_Caret][Nbr_Sub_Note_Off].Note = ((tmp_note + 1) << 8);
                                Sub_Channels_Note_Off[Track_Under_Caret][Nbr_Sub_Note_Off].Channel = Track_Under_Caret;
                                if(Keyboard_Notes_Bound[i])
                                {
                                    pos = (Column_Under_Caret / 3);
                                }
                                else
                                {
                                    pos = (Column_Under_Caret / 3) + Nbr_Sub_Note_Off;
                                }
                                Sub_Channels_Note_Off[Track_Under_Caret][Nbr_Sub_Note_Off].Sub_Channel = pos;

                                if(Nbr_Sub_Note_Off < (Channels_MultiNotes[Track_Under_Caret] - 1))
                                {
                                    Nbr_Sub_Note_Off++;
                                }
                                else
                                {
                                    Nbr_Sub_Note_Off = 0;
                                }
                            }

                            if(pos > (Channels_MultiNotes[Track_Under_Caret] - 1))
                            {
                                pos -= Channels_MultiNotes[Track_Under_Caret];
                            }
                            xoffseted += (pos * 2);

                            if(is_editing)
                            {
                                *(RawPatterns + xoffseted + PATTERN_NOTE1) = tmp_note;
                                *(RawPatterns + xoffseted + PATTERN_INSTR1) = Current_Instrument;
                            }
                        }
                        move_down = TRUE;
                    }
                    else
                    {
                        // Note off
                        if(is_recording_2 || is_editing)
                        {
                            LPJAZZ_KEY Channel = Get_Jazz_Key_Off(Sub_Channels_Note_Off, (tmp_note + 1) << 8);
                            if(Channel)
                            {
                                if(is_recording_2)
                                {
                                    xoffseted = Get_Pattern_Offset(pSequence[Cur_Position], Channel->Channel, line);
                                    pos = Channel->Sub_Channel;
                                    if(pos > (Channels_MultiNotes[Track_Under_Caret] - 1))
                                    {
                                        pos -= Channels_MultiNotes[Track_Under_Caret];
                                    }
                                    xoffseted += (pos * 2);
                                    *(RawPatterns + xoffseted + PATTERN_NOTE1) = NOTE_OFF;
                                    *(RawPatterns + xoffseted + PATTERN_INSTR1) = NO_INSTR;
                                }
                                // Now discard any remaining note off for that channel/sub channel
                                Nbr_Sub_Note_Off -= Discard_Key_Note_Off(Sub_Channels_Note_Off, Channel->Channel, Channel->Sub_Channel);
                                if(Nbr_Sub_Note_Off < 0) Nbr_Sub_Note_Off = 0;
                            }
                        }
                        if(is_recording_2)
                        {
                            move_down = TRUE;
                        }
                    }
No_Key:;
                }
                Keyboard_Nbr_Events = 0;
                if(is_editing)
                {
                    if(!Song_Playing)
                    {
                        if(move_down)
                        {
                            Pattern_Line += Current_Edit_Steps;
                            if(!Current_Edit_Steps)
                            {
                                Goto_Next_Column();
                            }
                        }
                    }
                }
                if(is_recording_2)
                {
                    if(old_key_Pattern_Line != Pattern_Line)
                    {
                        Actualize_Sequencer();
                    }
                    old_key_Pattern_Line = Pattern_Line;
                }
                Update_Pattern(0);
            }
        }

        // Note off (RSHIFT)
        if(retnote == -3 && is_editing && !is_recording_2 && !pattern_sliders)
        {
            int column = 0;
            int in_note = FALSE;
            int i;

            // Check if this is a legal note column
            for(i = 0; i < Channels_MultiNotes[Track_Under_Caret]; i++)
            {
                if(Column_Under_Caret == (i * 3))
                {
                    in_note = TRUE;
                    break;
                }
                column += 2;
            }

            if(in_note)
            {
                xoffseted = Get_Pattern_Offset(pSequence[Cur_Position], Track_Under_Caret, Pattern_Line);

                *(RawPatterns + xoffseted + PATTERN_NOTE1 + column) = NOTE_OFF;
                *(RawPatterns + xoffseted + PATTERN_INSTR1 + column) = NO_INSTR;

                if(!is_recording) if(!Song_Playing)
                {
                    Pattern_Line += Current_Edit_Steps;
                    if(!Current_Edit_Steps)
                    {
                        Goto_Next_Column();
                    }
                }
                Update_Pattern(0);
            }
        }

        // Delete a note
        if(retnote == -1 && is_editing && !is_recording_2)
        {
            int column = 0;
            int in_note = FALSE;
            int i;

            // Check if this is a legal note column
            for(i = 0; i < Channels_MultiNotes[Track_Under_Caret]; i++)
            {
                if(Column_Under_Caret == (i * 3))
                {
                    in_note = TRUE;
                    break;
                }
                column += 2;
            }

            if(in_note)
            {
                xoffseted = Get_Pattern_Offset(pSequence[Cur_Position], Track_Under_Caret, Pattern_Line);

                if(!Delete_Selection(Cur_Position))
                {
                    // Delete the note under the caret
                    *(RawPatterns + xoffseted + PATTERN_NOTE1 + column) = NO_NOTE;
                    *(RawPatterns + xoffseted + PATTERN_INSTR1 + column) = NO_INSTR;

                    if(!is_recording) if(!Song_Playing)
                    {
                        Pattern_Line += Current_Edit_Steps;
                        if(!Current_Edit_Steps)
                        {
                            Goto_Next_Column();
                        }
                    }
                }
                Update_Pattern(0);
            }
        }
    }
}

// ------------------------------------------------------
// Retrieve mouse data
char Check_Mouse_No_Button(int x, int y, int xs, int ys)
{
    if(Mouse.x > x && Mouse.x < x + xs && Mouse.y > y && Mouse.y < y + ys + 1)
    {
        return(1);
    }
    return(0);
}

char Check_Mouse(int x, int y, int xs, int ys)
{
    if(!gui_pushed && Mouse.x > x && Mouse.x < x + xs && Mouse.y > y && Mouse.y < y + ys + 1)
    {
        gui_lx = x;
        gui_ly = y;
        gui_lxs = xs;
        gui_lys = ys;
        gui_pushed = Mouse.button_oneshot;
        fluzy = -1;
        return(1);
    }
    if(gui_pushed && x == gui_lx && y == gui_ly && xs == gui_lxs && ys == gui_lys)
    {
        return(1);
    }
    return(0);
}

// ------------------------------------------------------
// Mouse Handler
void Mouse_Handler(void)
{
    int Cur_Position = Get_Song_Position();
    int i;

    if(gui_action == GUI_CMD_NOP) gui_action = GUI_CMD_NONE;

    teac = 0;

    // mouse wheel up
    if(Mouse.wheel == 1)
    {
        // Scroll the pattern
        Mouse_Wheel_Pattern_Ed(-MouseWheel_Multiplier, !Song_Playing && !is_recording);

        // Scroll the files list
        switch(Scopish)
        {
            case SCOPE_ZONE_INSTR_LIST:
            case SCOPE_ZONE_SYNTH_LIST:

                // Scroll the instruments/synths lists
                if(Check_Mouse(393, 41, (Cur_Width - 394), 136) == 1)
                {
                    Instrs_index--;
                    gui_action = GUI_CMD_INSTR_SYNTH_SCROLL;
                }
                break;

            case SCOPE_ZONE_MOD_DIR:
            case SCOPE_ZONE_INSTR_DIR:
            case SCOPE_ZONE_PRESET_DIR:
            case SCOPE_ZONE_REVERB_DIR:
            case SCOPE_ZONE_MIDICFG_DIR:
            case SCOPE_ZONE_PATTERN_DIR:
            case SCOPE_ZONE_SAMPLE_DIR:

                // Scroll the files lists
                if(Check_Mouse(393, 41, (Cur_Width - 394), 136) == 1)
                {
                    lt_index[Scopish]--;
                    gui_action = GUI_CMD_FILELIST_SCROLL;
                }
                break;
        }

        // Scroll the sequences
        if(userscreen == USER_SCREEN_SEQUENCER)
        {
            if(Check_Mouse(257, (Cur_Height - 134), 26, 90) ||
               Check_Mouse(89, (Cur_Height - 134), 26, 90) ||
               Check_Mouse(120, (Cur_Height - 134), 132, 90))
            {
                gui_action = GUI_CMD_PREVIOUS_POSITION;
            }
        }

        // Scroll the knobs
        Mouse_Wheel_303_Ed(MouseWheel_Multiplier);
        Mouse_Wheel_Sample_Ed(MouseWheel_Multiplier);
    }

    // mouse wheel down
    if(Mouse.wheel == -1)
    {
        // Scroll the pattern
        Mouse_Wheel_Pattern_Ed(MouseWheel_Multiplier, !Song_Playing && !is_recording);

        switch(Scopish)
        {
            case SCOPE_ZONE_INSTR_LIST:
            case SCOPE_ZONE_SYNTH_LIST:

                // Scroll the instruments/synths lists
                if(Check_Mouse(393, 41, (Cur_Width - 394), 136) == 1)
                {
                    Instrs_index++;
                    gui_action = GUI_CMD_INSTR_SYNTH_SCROLL;
                }
                break;

            case SCOPE_ZONE_MOD_DIR:
            case SCOPE_ZONE_INSTR_DIR:
            case SCOPE_ZONE_PRESET_DIR:
            case SCOPE_ZONE_REVERB_DIR:
            case SCOPE_ZONE_MIDICFG_DIR:
            case SCOPE_ZONE_PATTERN_DIR:
            case SCOPE_ZONE_SAMPLE_DIR:

                // Scroll the files lists
                if(Check_Mouse(393, 41, (Cur_Width - 394), 136) == 1)
                {
                    lt_index[Scopish]++;
                    gui_action = GUI_CMD_FILELIST_SCROLL;
                }
                break;
        }

        // Scroll the sequences
        if(userscreen == USER_SCREEN_SEQUENCER)
        {
            if(Check_Mouse(257, (Cur_Height - 134), 26, 90) ||
               Check_Mouse(89, (Cur_Height - 134), 26, 90) ||
               Check_Mouse(120, (Cur_Height - 134), 132, 90))
            {
                gui_action = GUI_CMD_NEXT_POSITION;
            }
        }

        // Scroll the knobs
        Mouse_Wheel_303_Ed(-MouseWheel_Multiplier);
        Mouse_Wheel_Sample_Ed(-MouseWheel_Multiplier);
    }

    if(Mouse.button & MOUSE_LEFT_BUTTON)
    {
        switch(Scopish)
        {
            case SCOPE_ZONE_INSTR_LIST:
            case SCOPE_ZONE_SYNTH_LIST:
                if(Check_Mouse(Cur_Width - 17, 59, 16, 103 + 1)) gui_action = GUI_CMD_SET_INSTR_SYNTH_LIST_SLIDER;
                break;

            case SCOPE_ZONE_MOD_DIR:
            case SCOPE_ZONE_INSTR_DIR:
            case SCOPE_ZONE_PRESET_DIR:
            case SCOPE_ZONE_REVERB_DIR:
            case SCOPE_ZONE_MIDICFG_DIR:
            case SCOPE_ZONE_PATTERN_DIR:
            case SCOPE_ZONE_SAMPLE_DIR:
                if(Check_Mouse(Cur_Width - 17, 59, 16, 103 + 1)) gui_action = GUI_CMD_SET_FILES_LIST_SLIDER;
                break;
        }

        Mouse_Left_Repeat_Instrument_Ed();
        Mouse_Sliders_303_Ed();
        Mouse_Sliders_Sample_Ed();
        Mouse_Sliders_Instrument_Ed();
        Mouse_Sliders_Master_Ed();
        Mouse_Sliders_Synth_Ed();
        Mouse_Sliders_Track_Fx_Ed();
        Mouse_Sliders_Fx_Ed();
        Mouse_Sliders_Track_Ed();
        Mouse_Sliders_Master_Shuffle();
        Mouse_Sliders_Pattern_Ed();
        Mouse_Sliders_Reverb_Ed();
    }
    else
    {
        // leftbutton
        sas = FALSE;
    }

    // Check Zones for GUI clicks -----------------------------------
    
    Mouse_Reverb_Ed();

    if(Mouse.button_oneshot & MOUSE_LEFT_BUTTON)
    {
        // Modules dir.
        if(Check_Mouse(Cur_Width - 126, 24, 18, 16))
        {
            Scopish = SCOPE_ZONE_MOD_DIR;
            Draw_Scope_Files_Button();
        }

        // Instruments dir.
        if(Check_Mouse(Cur_Width - 108, 24, 18, 16))
        {
            Scopish = SCOPE_ZONE_INSTR_DIR;
            Draw_Scope_Files_Button();
        }

        // Samples dir.
        if(Check_Mouse(Cur_Width - 90, 24, 18, 16))
        {
            Scopish = SCOPE_ZONE_SAMPLE_DIR;
            Draw_Scope_Files_Button();
        }

        // Presets dir.
        if(Check_Mouse(Cur_Width - 72, 24, 18, 16))
        {
            Scopish = SCOPE_ZONE_PRESET_DIR;
            Draw_Scope_Files_Button();
        }

        // Reverbs dir.
        if(Check_Mouse(Cur_Width - 54, 24, 18, 16))
        {
            Scopish = SCOPE_ZONE_REVERB_DIR;
            Draw_Scope_Files_Button();
        }

        // Patterns dir.
        if(Check_Mouse(Cur_Width - 36, 24, 18, 16))
        {
            Scopish = SCOPE_ZONE_PATTERN_DIR;
            Draw_Scope_Files_Button();
        }

        // Midi dir.
        if(Check_Mouse(Cur_Width - 18, 24, 18, 16))
        {
            Scopish = SCOPE_ZONE_MIDICFG_DIR;
            Draw_Scope_Files_Button();
        }

        // Tracks vumeters.
        if(Check_Mouse(Cur_Width - 72, 6, 18, 16))
        {
            Scopish = SCOPE_ZONE_VUMETERS;
            Draw_Scope_Files_Button();
        }

        // Tracks scopes.
        if(Check_Mouse(Cur_Width - 54, 6, 18, 16))
        {
            if(Scopish != SCOPE_ZONE_SCOPE)
            {
                Scopish_LeftRight = FALSE;
                Scopish = SCOPE_ZONE_SCOPE;
                Draw_Scope_Files_Button();
            }
            if(Scopish == SCOPE_ZONE_SCOPE && Scopish_LeftRight == TRUE)
            {
                Scopish_LeftRight = FALSE;
                Scopish = SCOPE_ZONE_SCOPE;
                Draw_Scope_Files_Button();
            }
        }

        // Instruments list
        if(Check_Mouse(Cur_Width - 36, 6, 18, 16))
        {
            Scopish = SCOPE_ZONE_INSTR_LIST;
            Draw_Scope_Files_Button();
        }

        // Synths list
        if(Check_Mouse(MAX_PATT_SCREEN_X + 1, 6, 18, 16))
        {
            Scopish = SCOPE_ZONE_SYNTH_LIST;
            Draw_Scope_Files_Button();
        }

        Mouse_Left_303_Ed();
        Mouse_Left_Sample_Ed();

        // Change current instrument name
        if(Check_Mouse(90, 108, 166, 16) && snamesel == INPUT_NONE)
        {
            snamesel = INPUT_INSTRUMENT_NAME;
            strcpy(cur_input_name, nameins[Current_Instrument]);
            sprintf(nameins[Current_Instrument], "");
            namesize = 0;
            gui_action = GUI_CMD_UPDATE_PATTERN_ED;
        }

        switch(Scopish)
        {
            case SCOPE_ZONE_INSTR_LIST:
            case SCOPE_ZONE_SYNTH_LIST:
                Files_List_Move(1);

                // Select
                if(Check_Mouse(393, 43, Cur_Width - 412, 133))
                {
                    gui_action = GUI_CMD_SET_INSTR_SYNTH_LIST_SELECT;
                }
                break;

            case SCOPE_ZONE_MOD_DIR:
            case SCOPE_ZONE_INSTR_DIR:
            case SCOPE_ZONE_PRESET_DIR:
            case SCOPE_ZONE_REVERB_DIR:
            case SCOPE_ZONE_PATTERN_DIR:
            case SCOPE_ZONE_MIDICFG_DIR:
            case SCOPE_ZONE_SAMPLE_DIR:
                Files_List_Move(1);

                // Select a file
                if(Check_Mouse(393, 43, Cur_Width - 412, 133))
                {
                    gui_action = GUI_CMD_SET_FILES_LIST_SELECT_FILE;
                }
                break;
        }

        if(Check_Mouse(258, 126, 16, 16)) gui_action = GUI_CMD_LOWER_OCTAVE;
        if(Check_Mouse(302, 126, 16, 16)) gui_action = GUI_CMD_HIGHER_OCTAVE;
        if(Check_Mouse(90, 126, 16, 16)) gui_action = GUI_CMD_DECREASE_STEP_ADD;
        if(Check_Mouse(134, 126, 16, 16)) gui_action = GUI_CMD_INCREASE_STEP_ADD;
        if(Check_Mouse(258, 108, 16, 16)) gui_action = GUI_CMD_PREV_INSTR;
        if(Check_Mouse(302, 108, 16, 16)) gui_action = GUI_CMD_NEXT_INSTR;

        // --- Player --------------------------------------------

        // Play song from top
        if(Check_Mouse(8, 28, 39, 16))
        {
            play_pattern = FALSE;
            reset_carriers = TRUE;
            Pattern_Line = 0;
            gui_action = GUI_CMD_PLAY_SONG;
        }
        // Play pattern from top
        if(Check_Mouse(49, 28, 39, 16))
        {
            play_pattern = TRUE;
            reset_carriers = TRUE;
            Pattern_Line = 0;
            gui_action = GUI_CMD_PLAY_SONG;
        }
        // Stop the playing song
        if(Check_Mouse(8, 46, 80, 16))
        {
            gui_action = GUI_CMD_STOP_SONG;
        }

        if(Check_Mouse(8, 64, 80, 16))
        {
            sr_isrecording ^= 1;
            gui_action = GUI_CMD_RECORD_303;
        }

        if(Check_Mouse(8, 82, 80, 16))
        {
            gui_action = GUI_CMD_EDIT_MODE;
        }

        // --- Sequence -----------------------------------------

        if(Check_Mouse(188, 28, 16, 16))
        {
            gui_action = GUI_CMD_PREVIOUS_POSITION;
        }
        if(Check_Mouse(232, 28, 16, 16))
        {
            gui_action = GUI_CMD_NEXT_POSITION;
        }
        if(Check_Mouse(188, 46, 16, 16) && pSequence[Cur_Position] > 0 && can_modify_song)
        {
            gui_action = GUI_CMD_PREV_PATT;
        }
        if(Check_Mouse(232, 46, 16, 16) && pSequence[Cur_Position] < 254 && can_modify_song)
        {
            gui_action = GUI_CMD_NEXT_PATT;
        }
        if(Check_Mouse(188, 64, 16, 16) && Song_Length > 1 && can_modify_song)
        {
            gui_action = GUI_CMD_REDUCE_SONG_LENGTH;
        }
        if(Check_Mouse(232, 64, 16, 16) && Song_Length < 255 && can_modify_song)
        {
            gui_action = GUI_CMD_INCREASE_SONG_LENGTH;
        }

        // Decrease the number of lines for this pattern
        if(Check_Mouse(188, 82, 16, 16) && patternLines[pSequence[Cur_Position]] > 1 && can_modify_song)
        {
            patternLines[pSequence[Cur_Position]]--;
            if(Pattern_Line >= patternLines[pSequence[Cur_Position]]) Pattern_Line = patternLines[pSequence[Cur_Position]] - 1;
            gui_action = GUI_CMD_SET_PATTERN_LENGTH;
        }
        // Increase the number of lines for this pattern
        if(Check_Mouse(232, 82, 16, 16) && patternLines[pSequence[Cur_Position]] < 128 && can_modify_song)
        {
            patternLines[pSequence[Cur_Position]]++;
            gui_action = GUI_CMD_SET_PATTERN_LENGTH;
        }

        // --- Song Settings --------------------------------------

        // Reduce the number of tracks
        if(Check_Mouse(324, 28, 16, 16) && Song_Tracks > 1 && can_modify_song)
        {
            Song_Tracks--;
            if(Song_Tracks < 1) Song_Tracks = 1;
            gui_action = GUI_CMD_CHANGE_TRACKS_NBR;
            teac = 4;
        }
        // Increase the number of tracks
        if(Check_Mouse(368, 28, 16, 16) && Song_Tracks < 16 && can_modify_song)
        {
            Song_Tracks++;
            if(Song_Tracks > 16) Song_Tracks = 16;
            gui_action = GUI_CMD_CHANGE_TRACKS_NBR;
            teac = 4;
        }

        // Delete the current track
        if(Check_Mouse(313, 28, 9, 16) && Song_Tracks >= 1 && !Song_Playing && can_modify_song)
        {
            Song_Tracks--;
            if(Song_Tracks < 1)
            {
                Song_Tracks = 1;
                // Just clear it
                for(i = 0; i < MAX_PATTERNS; i++)
                {
                    Reset_Pattern(i, Track_Under_Caret);
                }
                Reset_Track(Track_Under_Caret);
            }
            else
            {
                Delete_Track();
            }
            gui_action = GUI_CMD_DELETE_TRACK;
            teac = 4;
        }
        // Insert a track at current position
        if(Check_Mouse(302, 28, 9, 16) && Song_Tracks < 16 && !Song_Playing && can_modify_song)
        {
            Song_Tracks++;
            if(Song_Tracks > 16)
            {
                Song_Tracks = 16;
            }
            else
            {
                Insert_Track();
                gui_action = GUI_CMD_INSERT_TRACK;
                teac = 4;
            }
        }
        
        // Reduce the number of BPM
        if(Check_Mouse(324, 46, 16, 16) && !Song_Playing)
        {
            Beats_Per_Min--;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 1;
        }

        // Increase the number of BPM
        if(Check_Mouse(368, 46, 16, 16) && !Song_Playing)
        {
            Beats_Per_Min++;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 1;
        }

        // Decrease the number of TPB
        if(Check_Mouse(324, 64, 16, 16) && !Song_Playing)
        {
            Ticks_Per_Beat--;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 2;
        }

        // Increase the number of TPB
        if(Check_Mouse(368, 64, 16, 16) && !Song_Playing)
        {
            Ticks_Per_Beat++;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 2;
        }

        // Delete instrument
        if(Check_Mouse(320, 108, 28, 16))
        {
            Display_Requester(&Delete_Requester, GUI_CMD_DELETE_INSTRUMENT, NULL, TRUE);
        }

        // Zoom'em down
        if(Check_Mouse(332, 126, 16, 16))
        {
            for(i = 0; i < Song_Tracks; i++)
            {
                switch(Get_Track_Zoom(i))
                {
                    case TRACK_MEDIUM:
                        Set_Track_Zoom(i, TRACK_SMALL);
                        break;

                    case TRACK_LARGE:
                        Set_Track_Zoom(i, TRACK_MEDIUM);
                        break;
                }
            }
            Update_Pattern(0);
            Set_Track_Slider(gui_track);
            Display_Patterns_Sizes_Status();
        }
        // Zoom'em up
        if(Check_Mouse(332 + (18 * 1), 126, 16, 16))
        {
            for(i = 0; i < Song_Tracks; i++)
            {
                switch(Get_Track_Zoom(i))
                {
                    case TRACK_SMALL:
                        Set_Track_Zoom(i, TRACK_MEDIUM);
                        break;

                    case TRACK_MEDIUM:
                        Set_Track_Zoom(i, TRACK_LARGE);
                        break;
                }
            }
            Update_Pattern(0);
            Set_Track_Slider(gui_track);
            Display_Patterns_Sizes_Status();
        }
        // Double the font
        if(Check_Mouse(332 + (18 * 2), 126, 16, 16))
        {
            if(pattern_double)
            {
                Set_Font_Normal();
            }
            else
            {
                Set_Font_Double();
            }
            if(Large_Patterns)
            {
                Set_Pattern_Size();
            }
            else
            {
                Set_Pattern_Size();
            }
            if(pattern_double)
            {
                Gui_Draw_Button_Box(332 + (18 * 2), 126, 16, 16, "\017", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Display_Patterns_Sliders_Status();
            }
            else
            {
                Gui_Draw_Button_Box(332 + (18 * 2), 126, 16, 16, "\016", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
                Display_Patterns_Sliders_Status();
            }
            Update_Pattern(0);
        }

        // Switch patterns sliders
        if(Check_Mouse(350, 108, 16, 16))
        {
            pattern_sliders ^= TRUE;
            Display_Patterns_Sliders_Status();
            Update_Pattern(0);
        }

        // Switch patterns sliders
        if(Check_Mouse(368, 108, 16, 16) && pattern_double)
        {
            pattern_sliders_numbers ^= TRUE;
            Display_Patterns_Sliders_Status();
            Update_Pattern(0);
        }

        // Select track
        if(Check_Mouse(8, 152, 61, 10))
        {
            Select_Track_Block();
        }
        // Select note
        if(Check_Mouse(8, 164, 61, 10))
        {
            Select_Note_Block();
        }
        // Cut
        if(Check_Mouse(8 + 63, 152, 61, 10))
        {
            if(block_start_track[Curr_Buff_Block] != -1 &&
               block_end_track[Curr_Buff_Block] != -1)
            {
                Cut_Selection(Cur_Position);
            }
        }
        // Copy
        if(Check_Mouse(8 + 63, 164, 61, 10))
        {
            if(block_start_track[Curr_Buff_Block] != -1 &&
               block_end_track[Curr_Buff_Block] != -1)
            {
                Copy_Selection(Cur_Position);
            }
        }
        // Paste
        if(Check_Mouse(8 + (63 * 2), 152, 61, 10))
        {
            if(block_start_track_nibble[Curr_Buff_Block] != -1 &&
               block_end_track_nibble[Curr_Buff_Block] != -1 &&
               can_modify_song)
            {
                Paste_Block(Cur_Position, Paste_Across, TRUE);
            }
        }
        // Delete
        if(Check_Mouse(8 + (63 * 2), 164, 61, 10))
        {
            if(can_modify_song)
            {
                Delete_Selection(Cur_Position);
                Update_Pattern(0);
            }
        }
        // Interpolate
        if(Check_Mouse(8 + (63 * 3), 152, 61, 10))
        {
            if(can_modify_song)
            {
                Interpolate_Block(Cur_Position);
            }
        }
        // Randomize
        if(Check_Mouse(8 + (63 * 3), 164, 31, 10))
        {
            if(can_modify_song)
            {
                Randomize_Block(Cur_Position, 1);
            }
        }
        // Fill
        if(Check_Mouse(8 + (63 * 3) + 31, 164, 31, 10))
        {
            if(can_modify_song)
            {
                Fill_Block(Cur_Position, 1);
            }
        }
        // Semitone up
        if(Check_Mouse(8 + (63 * 4), 152, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Semitone_Up_Block(Cur_Position);
            }
        }
        // Semitone down
        if(Check_Mouse(8 + (63 * 4), 164, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Semitone_Down_Block(Cur_Position);
            }
        }
        // Octave up
        if(Check_Mouse(8 + (63 * 5), 152, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Octave_Up_Block(Cur_Position);
            }
        }
        // Octave down
        if(Check_Mouse(8 + (63 * 5), 164, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Octave_Down_Block(Cur_Position);
            }
        }

        // Switch small / large patterns
        int Add_Offset = Patterns_Lines_Offset;
        if(Check_Mouse(0, (Cur_Height - 171) + Add_Offset, 18, 16))
        {
            gui_action = GUI_CMD_SELECT_LARGE_PATTERNS;
        }

        // Exit
        if(Check_Mouse(0, 6, 16, 16))
        {
            gui_action = GUI_CMD_EXIT;
        }

        if(Check_Mouse(20, (Cur_Height - 171) + Add_Offset, 62, 16) && (userscreen != USER_SCREEN_SEQUENCER || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_SEQUENCER;
        if(Check_Mouse(20 + (TAB_LARG * 1), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_INSTRUMENT_EDIT || Patterns_Lines_Offset))
        {
            gui_action = GUI_CMD_SELECT_INSTRUMENT_EDIT;
            seditor = 0;
        }
        if(Check_Mouse(20 + (TAB_LARG * 2), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_SYNTH_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_SYNTH_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 3), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_SAMPLE_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_SAMPLE_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 4), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_TB303_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_TB303_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 5), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_TRACK_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_TRACK_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 6), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_TRACK_FX_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_TRACK_FX_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 7), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_FX_SETUP_EDIT || Patterns_Lines_Offset))
        {
            gui_action = GUI_CMD_SELECT_FX_EDIT;
            teac = 0;
        }
        if(Check_Mouse(20 + (TAB_LARG * 8), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_REVERB_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_REVERB_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 9), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_DISKIO_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_DISKIO_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 10), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_SETUP_EDIT || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_SCREEN_SETUP_EDIT;
        if(Check_Mouse(20 + (TAB_LARG * 11), (Cur_Height - 171) + Add_Offset, TAB_LARG - 2, 16) && (userscreen != USER_SCREEN_SETUP_MIDI || Patterns_Lines_Offset)) gui_action = GUI_CMD_SELECT_MIDI_SETUP;

        Mouse_Left_Track_Fx_Ed();
        Mouse_Left_Sequencer_Ed();
        Mouse_Left_Reverb_Ed();
        Mouse_Left_Midi_Ed();
        Mouse_Left_DiskIO_Ed();
        Mouse_Left_Track_Ed();
        Mouse_Left_Instrument_Ed();
        Mouse_Left_Fx_Ed();
        Mouse_Left_Synth_Ed();
        Mouse_Left_Master_Ed();
        Mouse_Left_Pattern_Ed();
    }

    // Middle mouse button
    if(Mouse.button_oneshot & MOUSE_MIDDLE_BUTTON)
    {
        Mouse_Middle_Sample_Ed();
    }

    // Right mouse button
    if(Mouse.button_oneshot & MOUSE_RIGHT_BUTTON)
    {
        Reset_Pattern_Scrolling_Horiz();
        Mouse_Right_Pattern_Ed();

        if(Check_Mouse(188, 82, 16, 16) && can_modify_song)
        {
            int ltp = patternLines[pSequence[Cur_Position]];
            ltp -= 8;
            if(ltp < 1) ltp = 1;
            patternLines[pSequence[Cur_Position]] = ltp;
            if(Pattern_Line >= patternLines[pSequence[Cur_Position]]) Pattern_Line = patternLines[pSequence[Cur_Position]] - 1;
            gui_action = GUI_CMD_SET_PATTERN_LENGTH;
        }

        if(Check_Mouse(232, 82, 16, 16) && can_modify_song)
        {
            int ltp = patternLines[pSequence[Cur_Position]];
            ltp += 8;
            if(ltp > 128) ltp = 128;
            patternLines[pSequence[Cur_Position]] = ltp;
            gui_action = GUI_CMD_SET_PATTERN_LENGTH;
        }

        if(Check_Mouse(258, 108, 16, 16) == 1)
        {
            gui_action = GUI_CMD_PREVIOUS_16_INSTR;
        }
        if(Check_Mouse(302, 108, 16, 16) == 1)
        {
            gui_action = GUI_CMD_NEXT_16_INSTR;
        }

        // Play song from current position
        if(Check_Mouse(8, 28, 39, 16))
        {
            play_pattern = FALSE;
            reset_carriers = FALSE;
            gui_action = GUI_CMD_PLAY_SONG;
        }
        // Play pattern from current position
        if(Check_Mouse(49, 28, 39, 16))
        {
            play_pattern = TRUE;
            reset_carriers = FALSE;
            gui_action = GUI_CMD_PLAY_SONG;
        }

        switch(Scopish)
        {
            case SCOPE_ZONE_INSTR_LIST:
            case SCOPE_ZONE_SYNTH_LIST:
                Files_List_Move(10);
                break;

            case SCOPE_ZONE_MOD_DIR:
            case SCOPE_ZONE_INSTR_DIR:
            case SCOPE_ZONE_PRESET_DIR:
            case SCOPE_ZONE_REVERB_DIR:
            case SCOPE_ZONE_PATTERN_DIR:
            case SCOPE_ZONE_MIDICFG_DIR:
            case SCOPE_ZONE_SAMPLE_DIR:
                Files_List_Move(10);
                break;
        }

        // Reduce the number of tracks
        if(Check_Mouse(324, 28, 16, 16) && Song_Tracks > 1 && can_modify_song)
        {
            Song_Tracks -= 5;
            if(Song_Tracks < 1) Song_Tracks = 1;
            gui_action = GUI_CMD_CHANGE_TRACKS_NBR;
            teac = 4;
        }
        // Increase the number of tracks
        if(Check_Mouse(368, 28, 16, 16) && Song_Tracks < 16 && can_modify_song)
        {
            Song_Tracks += 5;
            if(Song_Tracks > 16) Song_Tracks = 16;
            gui_action = GUI_CMD_CHANGE_TRACKS_NBR;
            teac = 4;
        }

        // Reduce the number of BPM by 16
        if(Check_Mouse(324, 46, 16, 16) && !Song_Playing)
        {
            Beats_Per_Min -= 16;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 1;
        }
        // Increase the number of BPM by 16
        if(Check_Mouse(368, 46, 16, 16) && !Song_Playing)
        {
            Beats_Per_Min += 16;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 1;
        }

        // Decrease the number of TPB
        if(Check_Mouse(324, 64, 16, 16) && !Song_Playing)
        {
            Ticks_Per_Beat -= 10;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 2;
        }

        // Increase the number of TPB
        if(Check_Mouse(368, 64, 16, 16) && !Song_Playing)
        {
            Ticks_Per_Beat += 10;
            gui_action = GUI_CMD_CHANGE_BPM_TICKS_NBR;
            teac = 2;
        }

        // Song_Length - 10
        if(Check_Mouse(188, 64, 16, 16) == 1 && Song_Length != 1 && can_modify_song)
        {
            int tLength = Song_Length;
            tLength -= 10;
            if(tLength < 1) tLength = 1;
            Song_Length = tLength;
            Update_Pattern(0);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }
        // Song_Length + 10
        if(Check_Mouse(232, 64, 16, 16) == 1 && Song_Length != 255 && can_modify_song)
        {
            int tLength = Song_Length;
            tLength += 10;
            if(tLength > 255) tLength = 255;
            Song_Length = tLength;
            Update_Pattern(0);
            gui_action = GUI_CMD_UPDATE_SEQUENCER;
        }

        // -10 positions
        if(Check_Mouse(188, 28, 16, 16))
        {
            gui_action = GUI_CMD_REDUCE_POSITIONS_10;
        }
        // +10 positions
        if(Check_Mouse(232, 28, 16, 16))
        {
            gui_action = GUI_CMD_INCREASE_POSITIONS_10;
        }

        // -10 patterns
        if(Check_Mouse(188, 46, 16, 16) && can_modify_song)
        {
            gui_action = GUI_CMD_REDUCE_PATTERNS_10;
        }
        // +10 patterns
        if(Check_Mouse(232, 46, 16, 16) && can_modify_song)
        {
            gui_action = GUI_CMD_INCREASE_PATTERNS_10;
        }

        // Record mode
        if(Check_Mouse(8, 82, 80, 16))
        {
            gui_action = GUI_CMD_RECORD_MODE;
        }

        // Select pattern
        if(Check_Mouse(8, 152, 61, 10))
        {
            Select_Pattern_Block();
        }
        // Select notes
        if(Check_Mouse(8, 164, 61, 10))
        {
            Select_All_Notes_Block();
        }

        // Semitone up
        if(Check_Mouse(8 + (63 * 4), 152, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Instrument_Semitone_Up_Block(Cur_Position);
            }
        }
        // Semitone down
        if(Check_Mouse(8 + (63 * 4), 164, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Instrument_Semitone_Down_Block(Cur_Position);
            }
        }
        // Instrument octave up
        if(Check_Mouse(8 + (63 * 5), 152, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Instrument_Octave_Up_Block(Cur_Position);
            }
        }
        // Instrument octave down
        if(Check_Mouse(8 + (63 * 5), 164, 61, 10))
        {
            if(can_modify_song)
            {
                Reset_Transpose_Block();
                Instrument_Octave_Down_Block(Cur_Position);
            }
        }

        // Fill with step add.
        if(Check_Mouse(8 + (63 * 3) + 31, 164, 31, 10))
        {
            if(can_modify_song)
            {
                Fill_Block(Cur_Position, Current_Edit_Steps);
            }
        }

        // Randomize with step add
        if(Check_Mouse(8 + (63 * 3), 164, 31, 10))
        {
            if(can_modify_song)
            {
                Randomize_Block(Cur_Position, Current_Edit_Steps);
            }
        }

        // Stereo scopes.
        if(Check_Mouse(Cur_Width - 54, 6, 18, 16))
        {
            if(Scopish != SCOPE_ZONE_SCOPE)
            {
                Scopish_LeftRight = TRUE;
                Scopish = SCOPE_ZONE_SCOPE;
                Draw_Scope_Files_Button();
            }
            if(Scopish == SCOPE_ZONE_SCOPE && Scopish_LeftRight == FALSE)
            {
                Scopish_LeftRight = TRUE;
                Scopish = SCOPE_ZONE_SCOPE;
                Draw_Scope_Files_Button();
            }
        }

        Mouse_Right_Fx_Ed();
        Mouse_Right_303_Ed();
        Mouse_Right_Reverb_Ed();
        Mouse_Right_Midi_Ed();
        Mouse_Right_DiskIO_Ed();
        Mouse_Right_Sample_Ed();
        Mouse_Right_Track_Ed();
        Mouse_Right_Instrument_Ed();
        Mouse_Right_Sequencer_Ed();
        Mouse_Right_Synth_Ed();
        Mouse_Right_Master_Ed();

        // Play a .wav
        switch(Scopish)
        {
            case SCOPE_ZONE_INSTR_LIST:
            case SCOPE_ZONE_SYNTH_LIST:
                break;

            case SCOPE_ZONE_MOD_DIR:
            case SCOPE_ZONE_INSTR_DIR:
            case SCOPE_ZONE_PRESET_DIR:
            case SCOPE_ZONE_REVERB_DIR:
            case SCOPE_ZONE_PATTERN_DIR:
            case SCOPE_ZONE_MIDICFG_DIR:
            case SCOPE_ZONE_SAMPLE_DIR:

                // Play the file
                if(Check_Mouse(393, 43, Cur_Width - 412, 133))
                {
                    gui_action = GUI_CMD_SET_FILES_LIST_PLAY_WAV;
                }
                break;
        }


    } // RIGHT MOUSE

    if(Mouse.button & MOUSE_RIGHT_BUTTON)
    {
        Mouse_Sliders_Right_Pattern_Ed();
        Mouse_Sliders_Right_Instrument_Ed();
        Mouse_Sliders_Right_Reverb_Ed();
    }
}

// ------------------------------------------------------
// Search the first free slot in the sequences pool
int Search_Free_Pattern(void)
{
    int i;
    int j;
    int found;

    for(i = 0; i < Song_Length; i++)
    {
        found = FALSE;
        for(j = 0; j < 128; j++)
        {
            if(pSequence[j] == i)
            {
                found = TRUE;
                break;
            }
        }
        if(!found) return(i);
    }
    return(-1);
}

// ------------------------------------------------------
// Next pattern/position
int Next_Line_Pattern_Auto(int *position, int lines, int *line)
{
    int new_pattern;
    int max_value;

    if(*line >= lines)
    {
        // Normal end of pattern
        *line = *line - lines;
        *position += 1;
        if(*position >= Song_Length - 1)
        {
            Song_Length++;
            max_value = Song_Length;
            if(max_value > 255)
            {
                Song_Length = 255;
                Song_Position = 0;
            }
            // Alloc a new pattern position
            new_pattern = Search_Free_Pattern();
            if(new_pattern > -1) pSequence[*position] = new_pattern;
        }
    }
    return(*position);
}

// ------------------------------------------------------
// Return the track associated with the midi channel
int Get_Midi_Channel(int midi_channel)
{
    int i;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        if(midi_channel == Chan_Midi_Prg[i])
        {
            return(i);
        }
    }
    return(-1);
}

// ------------------------------------------------------
// Return the first track which isn't allocated in the midi channels pool
int Get_Free_Midi_Sub_Channel(int track)
{
    int i;
    for(i = 0; i < MAX_POLYPHONY; i++)
    {
        if(Alloc_midi_Channels[track][i] == 0) return(i);
    }
    return(-1);
}

// ------------------------------------------------------
// Search & return the midi channel corresponding to the given data
int Search_Corresponding_Midi_Sub_Channel(int track, Uint32 Data)
{
    int i;

    for(i = 0; i < MAX_TRACKS; i++)
    {
        if(Alloc_midi_Channels[track][i] == (Data & 0x00ff00))
        {
            return(i);
        }
    }
    return(-1);
}

// ------------------------------------------------------
// Display the current beat time in milliseconds
void Display_Beat_Time(void)
{
    char String[64];
    sprintf(String, "1/4 Beat Time: %d ms.", (int) (15000.0f / (float) Beats_Per_Min));
    Gui_Clear_Array(268, 86, 120, 16);
    Print_String(268, 86, USE_FONT, String);
}

// ------------------------------------------------------
// Display master compressor
void Display_Master_Comp(void)
{
    char string[64];

    Gui_Draw_Button_Box(159, 6, 54, 16, "Threshold", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Real_Slider_Size(159 + 54, 6, 50, (int) (mas_comp_threshold_Master * 0.5f), TRUE);
    if(mas_comp_ratio_Master <= 0.01f)
    {
        sprintf(string, "Off");
    }
    else
    {
        sprintf(string, "%d%%", (int) mas_comp_threshold_Master);
    }
    Print_String(string, 159 + 54, 8, 67, BUTTON_TEXT_CENTERED);

    Gui_Draw_Button_Box(283, 6, 41, 16, "Ratio", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Real_Slider_Size(283 + 41, 6, 50, (int) (mas_comp_ratio_Master * 0.5f), TRUE);
    if(mas_comp_ratio_Master <= 0.01f)
    {
        sprintf(string, "Off");
    }
    else
    {
        sprintf(string, "%d%%", (int) mas_comp_ratio_Master);
    }
    Print_String(string, 283 + 41, 8, 67, BUTTON_TEXT_CENTERED);
}

// ------------------------------------------------------
// Display master volume
void Display_Master_Volume(void)
{
    char String[64];

    if(mas_vol < 0.01f) mas_vol = 0.01f;
    if(mas_vol > 1.0f) mas_vol = 1.0f;
    Gui_Draw_Button_Box(394, 6, 28, 16, "M.Vol", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    Real_Slider(394 + 28, 6, (int) (mas_vol * 128.0f), TRUE);
    sprintf(String, "%d%%", (int) (mas_vol * 100.0f));
    Print_String(String, 394 + 28, 8, 145, BUTTON_TEXT_CENTERED);
}

// ------------------------------------------------------
// Display the shuffle amount
void Display_Shuffle(void)
{
    char string[64];

    if(shuffle_amount > 100) shuffle_amount = 100;
    if(shuffle_amount < 0) shuffle_amount = 0;
    Gui_Draw_Button_Box(570, 6, 38, 16, "Shuffle", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);

    Real_Slider_Size(570 + 38, 6, 100, shuffle_amount, TRUE);
    sprintf(string, "%d%%", shuffle_amount);
    Print_String(string, 570 + 38, 8, 116, BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(728, 6, Cur_Width - 802, 16, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
}

// ------------------------------------------------------
// Handle the mouse event of the top bar
void Mouse_Sliders_Master_Shuffle(void)
{
    // Compressor threshold
    if(Check_Mouse(213, 6, 67, 18))
    {
        Mas_Compressor_Set_Variables_Master((Mouse.x - 223.0f) * 2.0f, mas_comp_ratio_Master);
        Display_Master_Comp();
    }

    // Compressor ratio
    if(Check_Mouse(324, 6, 67, 18))
    {
        Mas_Compressor_Set_Variables_Master(mas_comp_threshold_Master, (Mouse.x - 334.0f) * 2.0f);
        Display_Master_Comp();
    }

    // Master volume
    if(Check_Mouse(422, 6, 148, 18))
    {
        // Normalized
        mas_vol = (Mouse.x - 432.0f) / 128.0f;
        Display_Master_Volume();
    }

    // Shuffle
    if(Check_Mouse(586 + 24, 6, 120, 18))
    {
        shuffle_amount = (int) ((Mouse.x - (586 + 24 + 10)));
        Display_Shuffle();
    }
}

void Actualize_Master(char gode)
{
    if(gode == 0 || gode == 1 || gode == 2)
    {
        if(Beats_Per_Min < 20) Beats_Per_Min = 20;
        if(Beats_Per_Min > 255) Beats_Per_Min = 255;
        Switch_Cmd_Playing(FALSE);
    }

    if(gode == 0 || gode == 2)
    {
        if(Ticks_Per_Beat < 1) Ticks_Per_Beat = 1;
        if(Ticks_Per_Beat > 32) Ticks_Per_Beat = 32;
        if((Ticks_Per_Beat + 1) < Ticks_Synchro_Left)
        {
            Ticks_Synchro_Left = Ticks_Per_Beat + 1;
            Actualize_Fx_Ed(10);
            Actualize_Fx_Ed(11);
        }

        if((Ticks_Per_Beat + 1) < Ticks_Synchro_Right)
        {
            Ticks_Synchro_Right = Ticks_Per_Beat + 1;
            Actualize_Fx_Ed(10);
            Actualize_Fx_Ed(11);
        }
        Switch_Cmd_Playing(FALSE);
    }

    SamplesPerTick = (int) ((60 * MIX_RATE) / (Beats_Per_Min * Ticks_Per_Beat));
    float SamplesPerBeat = (float) MIX_RATE / (((float) Beats_Per_Min * 4) / 60);
    SamplesPerSub = SamplesPerTick / 6;

    if(gode == 5)
    {
        Switch_Cmd_Playing(FALSE);
    }

    if(gode == 0)
    {
        Gui_Draw_Arrows_Number_Box2(324, 28, Song_Tracks, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
    }

    if(gode == 4)
    {
        Gui_Draw_Arrows_Number_Box2(324, 28, Song_Tracks, BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE | (can_modify_song ^ BUTTON_DISABLED));
        Actualize_Seq_Ed(0);
        Update_Pattern(0);
    }

    if(userscreen == USER_SCREEN_SETUP_EDIT) Actualize_Master_Ed(3);
}

// ------------------------------------------------------
// Draw the stereo scope
typedef struct
{
    int nbr;
    int offset;
} POS_SCOPE, *LPPOS_SCOPE;

typedef struct
{
    int x_pos;
    int x_div;
    int x_mul;
    int y_pos;
    int y_large;
    int x_max;
} DAT_POS_SCOPE, *LPDAT_POS_SCOPE;

POS_SCOPE Scope_Table[] =
{
    { 0, 0 },       // 0
    { 1, 0 },       // 1
    { 2, 1 },       // 2
    { 3, 3 },       // 3
    { 4, 6 },       // 4
    { 5, 10 },      // 5

    { 6, 15 },      // 3/3
    { 7, 21 },      // 4/3
    { 8, 28 },      // 4/4
    { 9, 36 },      // 5/4
    { 10, 45 },     // 5/5
    { 11, 55 },     // 4/4/3
    { 12, 66 },     // 4/4/4
    { 13, 78 },     // 5/4/4
    { 14, 91 },     // 5/5/4
    { 15, 105 },    // 5/5/5
    { 16, 120 },    // 4/4/4/4
};

DAT_POS_SCOPE Scope_Table_Dats[] =
{
    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 1 },                     // 0 1

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 2 },                     // 1 2
    { 394, 2, 1, 135 / 2, 135 / 2, 2 },                     // 2

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 3 },                     // 3 3
    { 394, 3, 1, 135 / 2, 135 / 2, 3 },                     // 4
    { 394, 3, 2, 135 / 2, 135 / 2, 3 },                     // 5

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 4 },                     // 6 4
    { 394, 4, 1, 135 / 2, 135 / 2, 4 },                     // 7 
    { 394, 4, 2, 135 / 2, 135 / 2, 4 },                     // 8
    { 394, 4, 3, 135 / 2, 135 / 2, 4 },                     // 9

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 5 },                     // 10 5
    { 394, 5, 1, 135 / 2, 135 / 2, 5 },                     // 11
    { 394, 5, 2, 135 / 2, 135 / 2, 5 },                     // 12
    { 394, 5, 3, 135 / 2, 135 / 2, 5 },                     // 13
    { 394, 5, 4, 135 / 2, 135 / 2, 5 },                     // 14

    // ---
    { 394, 0, 0, 135 / 4, 135 / 4, 3 },                     // 15 3/3
    { 394, 3, 1, 135 / 4, 135 / 4, 3 },                     // 16
    { 394, 3, 2, 135 / 4, 135 / 4, 3 },                     // 17
    { 394, 0, 0, (135 / 2) + (135 / 4) + 1, 135 / 4, 3 },   // 18
    { 394, 3, 1, (135 / 2) + (135 / 4) + 1, 135 / 4, 3 },   // 19
    { 394, 3, 2, (135 / 2) + (135 / 4) + 1, 135 / 4, 3 },   // 20

    // ---
    { 394, 0, 0, 135 / 4, 135 / 4, 4 },                     // 21 4/3
    { 394, 4, 1, 135 / 4, 135 / 4, 4 },                     // 22
    { 394, 4, 2, 135 / 4, 135 / 4, 4 },                     // 23
    { 394, 4, 3, 135 / 4, 135 / 4, 4 },                     // 24
    { 394, 0, 0, (135 / 2) + (135 / 4) + 1, 135 / 4, 3 },   // 25
    { 394, 3, 1, (135 / 2) + (135 / 4) + 1, 135 / 4, 3 },   // 26
    { 394, 3, 2, (135 / 2) + (135 / 4) + 1, 135 / 4, 3 },   // 27

    // ---
    { 394, 0, 0, 135 / 4, 135 / 4, 4 },                     // 28 4/4
    { 394, 4, 1, 135 / 4, 135 / 4, 4 },                     // 29
    { 394, 4, 2, 135 / 4, 135 / 4, 4 },                     // 30
    { 394, 4, 3, 135 / 4, 135 / 4, 4 },                     // 31
    { 394, 0, 0, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 32
    { 394, 4, 1, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 33
    { 394, 4, 2, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 34
    { 394, 4, 3, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 35

    // ---
    { 394, 0, 0, 135 / 4, 135 / 4, 5 },                     // 36 5/4
    { 394, 5, 1, 135 / 4, 135 / 4, 5 },                     // 37
    { 394, 5, 2, 135 / 4, 135 / 4, 5 },                     // 38
    { 394, 5, 3, 135 / 4, 135 / 4, 5 },                     // 39
    { 394, 5, 4, 135 / 4, 135 / 4, 5 },                     // 40
    { 394, 0, 0, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 41
    { 394, 4, 1, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 42
    { 394, 4, 2, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 43
    { 394, 4, 3, (135 / 2) + (135 / 4) + 1, 135 / 4, 4 },   // 44

    // ---
    { 394, 0, 0, 135 / 4, 135 / 4, 5 },                     // 45 5/5
    { 394, 5, 1, 135 / 4, 135 / 4, 5 },                     // 46
    { 394, 5, 2, 135 / 4, 135 / 4, 5 },                     // 47
    { 394, 5, 3, 135 / 4, 135 / 4, 5 },                     // 48
    { 394, 5, 4, 135 / 4, 135 / 4, 5 },                     // 49
    { 394, 0, 0, (135 / 2) + (135 / 4) + 1, 135 / 4, 5 },   // 50
    { 394, 5, 1, (135 / 2) + (135 / 4) + 1, 135 / 4, 5 },   // 51
    { 394, 5, 2, (135 / 2) + (135 / 4) + 1, 135 / 4, 5 },   // 52
    { 394, 5, 3, (135 / 2) + (135 / 4) + 1, 135 / 4, 5 },   // 53
    { 394, 5, 4, (135 / 2) + (135 / 4) + 1, 135 / 4, 5 },   // 54

    // ---
    { 394, 0, 0, 135 / 6, 135 / 6, 4 },                     // 55 4/4/3
    { 394, 4, 1, 135 / 6, 135 / 6, 4 },                     // 56
    { 394, 4, 2, 135 / 6, 135 / 6, 4 },                     // 57
    { 394, 4, 3, 135 / 6, 135 / 6, 4 },                     // 58
    { 394, 0, 0, (135 / 2) + 1, 135 / 6, 4 },               // 59
    { 394, 4, 1, (135 / 2) + 1, 135 / 6, 4 },               // 60
    { 394, 4, 2, (135 / 2) + 1, 135 / 6, 4 },               // 61
    { 394, 4, 3, (135 / 2) + 1, 135 / 6, 4 },               // 62
    { 394, 0, 0, (135 / 2) + (135 / 3), 135 / 6, 3 },       // 63
    { 394, 3, 1, (135 / 2) + (135 / 3), 135 / 6, 3 },       // 64
    { 394, 3, 2, (135 / 2) + (135 / 3), 135 / 6, 3 },       // 65

    // ---
    { 394, 0, 0, 135 / 6, 135 / 6, 4 },                     // 66 4/4/4 
    { 394, 4, 1, 135 / 6, 135 / 6, 4 },                     // 67
    { 394, 4, 2, 135 / 6, 135 / 6, 4 },                     // 68
    { 394, 4, 3, 135 / 6, 135 / 6, 4 },                     // 69
    { 394, 0, 0, (135 / 2) + 1, 135 / 6, 4 },               // 70
    { 394, 4, 1, (135 / 2) + 1, 135 / 6, 4 },               // 71
    { 394, 4, 2, (135 / 2) + 1, 135 / 6, 4 },               // 72
    { 394, 4, 3, (135 / 2) + 1, 135 / 6, 4 },               // 73
    { 394, 0, 0, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 74
    { 394, 4, 1, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 75
    { 394, 4, 2, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 76
    { 394, 4, 3, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 77

    // ---
    { 394, 0, 0, 135 / 6, 135 / 6, 5 },                     // 78 5/4/4
    { 394, 5, 1, 135 / 6, 135 / 6, 5 },                     // 79
    { 394, 5, 2, 135 / 6, 135 / 6, 5 },                     // 80
    { 394, 5, 3, 135 / 6, 135 / 6, 5 },                     // 81
    { 394, 5, 4, 135 / 6, 135 / 6, 5 },                     // 82
    { 394, 0, 0, (135 / 2) + 1, 135 / 6, 4 },               // 83
    { 394, 4, 1, (135 / 2) + 1, 135 / 6, 4 },               // 84
    { 394, 4, 2, (135 / 2) + 1, 135 / 6, 4 },               // 85
    { 394, 4, 3, (135 / 2) + 1, 135 / 6, 4 },               // 86
    { 394, 0, 0, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 87
    { 394, 4, 1, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 88
    { 394, 4, 2, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 89
    { 394, 4, 3, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 90

    // ---
    { 394, 0, 0, 135 / 6, 135 / 6, 5 },                     // 91 5/5/4
    { 394, 5, 1, 135 / 6, 135 / 6, 5 },                     // 92
    { 394, 5, 2, 135 / 6, 135 / 6, 5 },                     // 93
    { 394, 5, 3, 135 / 6, 135 / 6, 5 },                     // 94
    { 394, 5, 4, 135 / 6, 135 / 6, 5 },                     // 95
    { 394, 0, 0, (135 / 2) + 1, 135 / 6, 5 },               // 96
    { 394, 5, 1, (135 / 2) + 1, 135 / 6, 5 },               // 97
    { 394, 5, 2, (135 / 2) + 1, 135 / 6, 5 },               // 98
    { 394, 5, 3, (135 / 2) + 1, 135 / 6, 5 },               // 99
    { 394, 5, 4, (135 / 2) + 1, 135 / 6, 5 },               // 100
    { 394, 0, 0, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 101
    { 394, 4, 1, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 102
    { 394, 4, 2, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 103
    { 394, 4, 3, (135 / 2) + (135 / 3), 135 / 6, 4 },       // 104

    // ---
    { 394, 0, 0, 135 / 6, 135 / 6, 5 },                     // 105 5/5/5
    { 394, 5, 1, 135 / 6, 135 / 6, 5 },                     // 106
    { 394, 5, 2, 135 / 6, 135 / 6, 5 },                     // 107
    { 394, 5, 3, 135 / 6, 135 / 6, 5 },                     // 108
    { 394, 5, 4, 135 / 6, 135 / 6, 5 },                     // 109
    { 394, 0, 0, (135 / 2) + 1, 135 / 6, 5 },               // 110
    { 394, 5, 1, (135 / 2) + 1, 135 / 6, 5 },               // 111
    { 394, 5, 2, (135 / 2) + 1, 135 / 6, 5 },               // 112
    { 394, 5, 3, (135 / 2) + 1, 135 / 6, 5 },               // 113
    { 394, 5, 4, (135 / 2) + 1, 135 / 6, 5 },               // 114
    { 394, 0, 0, (135 / 2) + (135 / 3), 135 / 6, 5 },       // 115
    { 394, 5, 1, (135 / 2) + (135 / 3), 135 / 6, 5 },       // 116
    { 394, 5, 2, (135 / 2) + (135 / 3), 135 / 6, 5 },       // 117
    { 394, 5, 3, (135 / 2) + (135 / 3), 135 / 6, 5 },       // 118
    { 394, 5, 4, (135 / 2) + (135 / 3), 135 / 6, 5 },       // 119

    // ---
    { 394, 0, 0, 135 / 8, 135 / 8, 4 },                     // 120 4/4/4/4
    { 394, 4, 1, 135 / 8, 135 / 8, 4 },                     // 121
    { 394, 4, 2, 135 / 8, 135 / 8, 4 },                     // 122
    { 394, 4, 3, 135 / 8, 135 / 8, 4 },                     // 123
    { 394, 0, 0, (135 / 3) + 4, 135 / 8, 4 },               // 124
    { 394, 4, 1, (135 / 3) + 4, 135 / 8, 4 },               // 125
    { 394, 4, 2, (135 / 3) + 4, 135 / 8, 4 },               // 126
    { 394, 4, 3, (135 / 3) + 4, 135 / 8, 4 },               // 127
    { 394, 0, 0, (135 / 3) + (135 / 4) + 3, 135 / 8, 4 },   // 128
    { 394, 4, 1, (135 / 3) + (135 / 4) + 3, 135 / 8, 4 },   // 129
    { 394, 4, 2, (135 / 3) + (135 / 4) + 3, 135 / 8, 4 },   // 130
    { 394, 4, 3, (135 / 3) + (135 / 4) + 3, 135 / 8, 4 },   // 131
    { 394, 0, 0, (135 / 3) + (135 / 2) + 2, 135 / 8, 4 },   // 132
    { 394, 4, 1, (135 / 3) + (135 / 2) + 2, 135 / 8, 4 },   // 133
    { 394, 4, 2, (135 / 3) + (135 / 2) + 2, 135 / 8, 4 },   // 134
    { 394, 4, 3, (135 / 3) + (135 / 2) + 2, 135 / 8, 4 },   // 135
};

char *table_channels_scopes[] =
{
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    // ---
    "L",
    "R",
};

POS_SCOPE VuMeters_Table[] =
{
    { 0, 0 },       // 0
    { 1, 0 },       // 1
    { 2, 1 },       // 2
    { 3, 3 },       // 3
    { 4, 6 },       // 4
    { 5, 10 },      // 5

    { 6, 15 },      // 6
    { 7, 21 },      // 7
    { 8, 28 },      // 8
    { 9, 36 },      // 9
    { 10, 45 },     // 10
    { 11, 55 },     // 11
    { 12, 66 },     // 12
    { 13, 78 },     // 13
    { 14, 91 },     // 14
    { 15, 105 },    // 15
    { 16, 120 },    // 16
};

DAT_POS_SCOPE VuMeters_Table_Dats[] =
{
    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 1 },                     // 0 1

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 2 },                     // 1 2
    { 394, 2, 1, 135 / 2, 135 / 2, 2 },                     // 2

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 3 },                     // 3 3
    { 394, 3, 1, 135 / 2, 135 / 2, 3 },                     // 4
    { 394, 3, 2, 135 / 2, 135 / 2, 3 },                     // 5

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 4 },                     // 6 4
    { 394, 4, 1, 135 / 2, 135 / 2, 4 },                     // 7 
    { 394, 4, 2, 135 / 2, 135 / 2, 4 },                     // 8
    { 394, 4, 3, 135 / 2, 135 / 2, 4 },                     // 9

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 5 },                     // 10 5
    { 394, 5, 1, 135 / 2, 135 / 2, 5 },                     // 11
    { 394, 5, 2, 135 / 2, 135 / 2, 5 },                     // 12
    { 394, 5, 3, 135 / 2, 135 / 2, 5 },                     // 13
    { 394, 5, 4, 135 / 2, 135 / 2, 5 },                     // 14

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 6 },                     // 15 6
    { 394, 6, 1, 135 / 2, 135 / 2, 6 },                     // 16
    { 394, 6, 2, 135 / 2, 135 / 2, 6 },                     // 17
    { 394, 6, 3, 135 / 2, 135 / 2, 6 },                     // 18
    { 394, 6, 4, 135 / 2, 135 / 2, 6 },                     // 19
    { 394, 6, 5, 135 / 2, 135 / 2, 6 },                     // 20

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 7 },                     // 21 7
    { 394, 7, 1, 135 / 2, 135 / 2, 7 },                     // 22
    { 394, 7, 2, 135 / 2, 135 / 2, 7 },                     // 23
    { 394, 7, 3, 135 / 2, 135 / 2, 7 },                     // 24
    { 394, 7, 4, 135 / 2, 135 / 2, 7 },                     // 25
    { 394, 7, 5, 135 / 2, 135 / 2, 7 },                     // 26
    { 394, 7, 6, 135 / 2, 135 / 2, 7 },                     // 27

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 8 },                     // 28 8
    { 394, 8, 1, 135 / 2, 135 / 2, 8 },                     // 29
    { 394, 8, 2, 135 / 2, 135 / 2, 8 },                     // 30
    { 394, 8, 3, 135 / 2, 135 / 2, 8 },                     // 31
    { 394, 8, 4, 135 / 2, 135 / 2, 8 },                     // 32
    { 394, 8, 5, 135 / 2, 135 / 2, 8 },                     // 33
    { 394, 8, 6, 135 / 2, 135 / 2, 8 },                     // 32
    { 394, 8, 7, 135 / 2, 135 / 2, 8 },                     // 35

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 9 },                     // 36 9
    { 394, 9, 1, 135 / 2, 135 / 2, 9 },                     // 37
    { 394, 9, 2, 135 / 2, 135 / 2, 9 },                     // 38
    { 394, 9, 3, 135 / 2, 135 / 2, 9 },                     // 39
    { 394, 9, 4, 135 / 2, 135 / 2, 9 },                     // 40
    { 394, 9, 5, 135 / 2, 135 / 2, 9 },                     // 41
    { 394, 9, 6, 135 / 2, 135 / 2, 9 },                     // 42
    { 394, 9, 7, 135 / 2, 135 / 2, 9 },                     // 43
    { 394, 9, 8, 135 / 2, 135 / 2, 9 },                     // 44

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 10 },                    // 45 10
    { 394, 10, 1, 135 / 2, 135 / 2, 10 },                   // 46
    { 394, 10, 2, 135 / 2, 135 / 2, 10 },                   // 47
    { 394, 10, 3, 135 / 2, 135 / 2, 10 },                   // 48
    { 394, 10, 4, 135 / 2, 135 / 2, 10 },                   // 49
    { 394, 10, 5, 135 / 2, 135 / 2, 10 },                   // 50
    { 394, 10, 6, 135 / 2, 135 / 2, 10 },                   // 51
    { 394, 10, 7, 135 / 2, 135 / 2, 10 },                   // 52
    { 394, 10, 8, 135 / 2, 135 / 2, 10 },                   // 53
    { 394, 10, 9, 135 / 2, 135 / 2, 10 },                   // 54

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 11 },                    // 55 11
    { 394, 11, 1, 135 / 2, 135 / 2, 11 },                   // 56
    { 394, 11, 2, 135 / 2, 135 / 2, 11 },                   // 57
    { 394, 11, 3, 135 / 2, 135 / 2, 11 },                   // 58
    { 394, 11, 4, 135 / 2, 135 / 2, 11 },                   // 59
    { 394, 11, 5, 135 / 2, 135 / 2, 11 },                   // 60
    { 394, 11, 6, 135 / 2, 135 / 2, 11 },                   // 61
    { 394, 11, 7, 135 / 2, 135 / 2, 11 },                   // 62
    { 394, 11, 8, 135 / 2, 135 / 2, 11 },                   // 63
    { 394, 11, 9, 135 / 2, 135 / 2, 11 },                   // 64
    { 394, 11, 10, 135 / 2, 135 / 2, 11 },                  // 65

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 12 },                    // 66 12
    { 394, 12, 1, 135 / 2, 135 / 2, 12 },                   // 67
    { 394, 12, 2, 135 / 2, 135 / 2, 12 },                   // 68
    { 394, 12, 3, 135 / 2, 135 / 2, 12 },                   // 69
    { 394, 12, 4, 135 / 2, 135 / 2, 12 },                   // 70
    { 394, 12, 5, 135 / 2, 135 / 2, 12 },                   // 71
    { 394, 12, 6, 135 / 2, 135 / 2, 12 },                   // 72
    { 394, 12, 7, 135 / 2, 135 / 2, 12 },                   // 73
    { 394, 12, 8, 135 / 2, 135 / 2, 12 },                   // 74
    { 394, 12, 9, 135 / 2, 135 / 2, 12 },                   // 75
    { 394, 12, 10, 135 / 2, 135 / 2, 12 },                  // 76
    { 394, 12, 11, 135 / 2, 135 / 2, 12 },                  // 77

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 13 },                    // 78 13
    { 394, 13, 1, 135 / 2, 135 / 2, 13 },                   // 79
    { 394, 13, 2, 135 / 2, 135 / 2, 13 },                   // 80
    { 394, 13, 3, 135 / 2, 135 / 2, 13 },                   // 81
    { 394, 13, 4, 135 / 2, 135 / 2, 13 },                   // 82
    { 394, 13, 5, 135 / 2, 135 / 2, 13 },                   // 83
    { 394, 13, 6, 135 / 2, 135 / 2, 13 },                   // 84
    { 394, 13, 7, 135 / 2, 135 / 2, 13 },                   // 85
    { 394, 13, 8, 135 / 2, 135 / 2, 13 },                   // 86
    { 394, 13, 9, 135 / 2, 135 / 2, 13 },                   // 87
    { 394, 13, 10, 135 / 2, 135 / 2, 13 },                  // 88
    { 394, 13, 11, 135 / 2, 135 / 2, 13 },                  // 89
    { 394, 13, 12, 135 / 2, 135 / 2, 13 },                  // 90

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 14 },                    // 91 14
    { 394, 14, 1, 135 / 2, 135 / 2, 14 },                   // 92
    { 394, 14, 2, 135 / 2, 135 / 2, 14 },                   // 93
    { 394, 14, 3, 135 / 2, 135 / 2, 14 },                   // 94
    { 394, 14, 4, 135 / 2, 135 / 2, 14 },                   // 95
    { 394, 14, 5, 135 / 2, 135 / 2, 14 },                   // 96
    { 394, 14, 6, 135 / 2, 135 / 2, 14 },                   // 97
    { 394, 14, 7, 135 / 2, 135 / 2, 14 },                   // 98
    { 394, 14, 8, 135 / 2, 135 / 2, 14 },                   // 99
    { 394, 14, 9, 135 / 2, 135 / 2, 14 },                   // 100
    { 394, 14, 10, 135 / 2, 135 / 2, 14 },                  // 101
    { 394, 14, 11, 135 / 2, 135 / 2, 14 },                  // 102
    { 394, 14, 12, 135 / 2, 135 / 2, 14 },                  // 103
    { 394, 14, 13, 135 / 2, 135 / 2, 14 },                  // 104

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 15 },                    // 105 15
    { 394, 15, 1, 135 / 2, 135 / 2, 15 },                   // 106
    { 394, 15, 2, 135 / 2, 135 / 2, 15 },                   // 107
    { 394, 15, 3, 135 / 2, 135 / 2, 15 },                   // 108
    { 394, 15, 4, 135 / 2, 135 / 2, 15 },                   // 109
    { 394, 15, 5, 135 / 2, 135 / 2, 15 },                   // 110
    { 394, 15, 6, 135 / 2, 135 / 2, 15 },                   // 111
    { 394, 15, 7, 135 / 2, 135 / 2, 15 },                   // 112
    { 394, 15, 8, 135 / 2, 135 / 2, 15 },                   // 113
    { 394, 15, 9, 135 / 2, 135 / 2, 15 },                   // 114
    { 394, 15, 10, 135 / 2, 135 / 2, 15 },                  // 115
    { 394, 15, 11, 135 / 2, 135 / 2, 15 },                  // 116
    { 394, 15, 12, 135 / 2, 135 / 2, 15 },                  // 117
    { 394, 15, 13, 135 / 2, 135 / 2, 15 },                  // 118
    { 394, 15, 14, 135 / 2, 135 / 2, 15 },                  // 119

    // ---
    { 394, 0, 0, 135 / 2, 135 / 2, 16 },                    // 120 16
    { 394, 16, 1, 135 / 2, 135 / 2, 16 },                   // 121
    { 394, 16, 2, 135 / 2, 135 / 2, 16 },                   // 122
    { 394, 16, 3, 135 / 2, 135 / 2, 16 },                   // 123
    { 394, 16, 4, 135 / 2, 135 / 2, 16 },                   // 124
    { 394, 16, 5, 135 / 2, 135 / 2, 16 },                   // 125
    { 394, 16, 6, 135 / 2, 135 / 2, 16 },                   // 126
    { 394, 16, 7, 135 / 2, 135 / 2, 16 },                   // 127
    { 394, 16, 8, 135 / 2, 135 / 2, 16 },                   // 128
    { 394, 16, 9, 135 / 2, 135 / 2, 16 },                   // 129
    { 394, 16, 10, 135 / 2, 135 / 2, 16 },                  // 130
    { 394, 16, 11, 135 / 2, 135 / 2, 16 },                  // 131
    { 394, 16, 12, 135 / 2, 135 / 2, 16 },                  // 132
    { 394, 16, 13, 135 / 2, 135 / 2, 16 },                  // 133
    { 394, 16, 14, 135 / 2, 135 / 2, 16 },                  // 134
    { 394, 16, 15, 135 / 2, 135 / 2, 16 },                  // 135
};

void Draw_Scope(void)
{
    int i;
    int pixel_color = COL_SCOPESSAMPLES;
    float pos_in_line;
    float data;
    int x_max;
    int nibble_pos;
    int offset_scope = pos_scope_latency;
    LPDAT_POS_SCOPE ptrTbl_Dat;
    int scope_pos = Get_Song_Position();
    int active_channel;
    int cur_pos_x = 394;
    int max_right = Cur_Width - 395;

    if(offset_scope < 0) offset_scope = 0;
    if(offset_scope > (AUDIO_Latency / 2) - 1) offset_scope = (AUDIO_Latency / 2) - 1;

    if(Scopish == SCOPE_ZONE_SCOPE)
    {
        SetColor(COL_BACKGROUND);
        Fillrect(394, 42, Cur_Width - 1, 179);

        cur_pos_x = 0;
        if(Scopish_LeftRight)
        {
            // Left / Right
            ptrTbl_Dat = &Scope_Table_Dats[Scope_Table[2].offset];
            for(i = 0; i < Scope_Table[2].nbr; i++)
            {
                if(ptrTbl_Dat->x_div) nibble_pos = ((max_right / ptrTbl_Dat->x_div) * ptrTbl_Dat->x_mul);
                else
                {
                    nibble_pos = 0;
                    cur_pos_x = 394;
                }
                Print_String(cur_pos_x + 4,
                             44 + (ptrTbl_Dat->y_pos - ptrTbl_Dat->y_large),
                             USE_FONT,
                             table_channels_scopes[i + MAX_TRACKS]);
                x_max = max_right / ptrTbl_Dat->x_max;
                if(nibble_pos)
                {
                    x_max += max_right % ptrTbl_Dat->x_max;
                }
                for(int s = 0; s < x_max; s++)
                {
                    pos_in_line = ((float) s) / (float) x_max;
                    data = (Scope_Dats_LeftRight[i][offset_scope + (int) (pos_in_line * 128)]) / 32767.0f;
                    if(data < -1.0f) data = -1.0f;
                    if(data > 1.0f) data = 1.0f;
                    int y = 42 + ptrTbl_Dat->y_pos + (int) (data * ptrTbl_Dat->y_large);
                    DrawPixel(cur_pos_x, y, pixel_color);
                    cur_pos_x++;
                }
                ptrTbl_Dat++;
                pixel_color = COL_SCOPESSAMPLES;
            }
        }
        else
        {
            // Tracks
            ptrTbl_Dat = &Scope_Table_Dats[Scope_Table[Song_Tracks].offset];
            for(i = 0; i < Scope_Table[Song_Tracks].nbr; i++)
            {
                if(ptrTbl_Dat->x_div) nibble_pos = ((max_right / ptrTbl_Dat->x_div) * ptrTbl_Dat->x_mul);
                else
                {
                    nibble_pos = 0;
                    cur_pos_x = 394;
                }
                // Print the number of the track
                if(Chan_Active_State[scope_pos][i])
                {
                    Print_String(cur_pos_x + 4, 44 + (ptrTbl_Dat->y_pos - ptrTbl_Dat->y_large),
                                 USE_FONT,
                                 table_channels_scopes[i]);
                    active_channel = TRUE;
                }
                else
                {
                    Print_String(cur_pos_x + 4,
                                 44 + (ptrTbl_Dat->y_pos - ptrTbl_Dat->y_large),
                                 USE_FONT_LOW,
                                 table_channels_scopes[i]);
                    active_channel = FALSE;
                }
                x_max = max_right / ptrTbl_Dat->x_max;
                if(!nibble_pos)
                {
                    x_max += max_right % ptrTbl_Dat->x_max;
                }
                for(int s = 0; s < x_max; s++)
                {
                    // [0..1]
                    pos_in_line = ((float) s) / (float) x_max;
                    data = Scope_Dats[i][offset_scope + (int) (pos_in_line * 128)];
                    if(data < -1.0f)
                    {
                        data = -1.0f;
                    }
                    if(data > 1.0f)
                    {
                        data = 1.0f;
                    }
                    int y = 42 + ptrTbl_Dat->y_pos + (int) (data * (ptrTbl_Dat->y_large));
                    DrawPixel(cur_pos_x, y, pixel_color);
                    cur_pos_x++;
                }
                ptrTbl_Dat++;
                pixel_color = COL_SCOPESSAMPLES;
            }
        }
    }
}

int Init_Scopes_VuMeters_Buffers(void)
{
    int i;

    pos_scope = 0;
    pos_scope_latency = 0;
    for(i = 0; i < MAX_TRACKS; i++)
    {  
        // ---
        if(Scope_Dats[i])
        {
            free(Scope_Dats[i]);
        }
        Scope_Dats[i] = (float *) malloc(((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
        if(!Scope_Dats[i])
        {
            return(FALSE);
        }
        memset(Scope_Dats[i], 0, ((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));

        // ---
        if(VuMeters_Dats_L[i])
        {
            free(VuMeters_Dats_L[i]);
        }
        VuMeters_Dats_L[i] = (float *) malloc(((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
        if(!VuMeters_Dats_L[i])
        {
            return(FALSE);
        }
        memset(VuMeters_Dats_L[i], 0, ((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));

        // ---
        if(VuMeters_Dats_R[i])
        {
            free(VuMeters_Dats_R[i]);
        }
        VuMeters_Dats_R[i] = (float *) malloc(((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
        if(!VuMeters_Dats_R[i])
        {
            return(FALSE);
        }
        memset(VuMeters_Dats_R[i], 0, ((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
    }

    if(Scope_Dats_LeftRight[0])
    {
        free(Scope_Dats_LeftRight[0]);
    }
    if(Scope_Dats_LeftRight[1])
    {
        free(Scope_Dats_LeftRight[1]);
    }
    Scope_Dats_LeftRight[0] = (float *) malloc(((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
    if(!Scope_Dats_LeftRight[0])
    {
        return(FALSE);
    }
    Scope_Dats_LeftRight[1] = (float *) malloc(((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
    if(!Scope_Dats_LeftRight[1])
    {
        return(FALSE);
    }
    memset(Scope_Dats_LeftRight[0], 0, ((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
    memset(Scope_Dats_LeftRight[1], 0, ((AUDIO_Latency + 1) + (512 * 4)) * 2 * sizeof(float));
    return(TRUE);
}

// ------------------------------------------------------
// Draw the vumeters
void Draw_VuMeters(void)
{
    int i;
    int j;
    LPDAT_POS_SCOPE ptrTbl_Dat;
    float data;
    float MaxLevel_L;
    float MaxLevel_R;
    int i_MaxLevel_L;
    int i_MaxLevel_R;
    int bottom_line;
    int peak_line;
    int offset_scope = pos_scope_latency;
    int x_max;
    int x_max_half;
    int x_max_half_start;
    int spread_remainder;
    int nibble_pos;
    int active_channel;
    int max_right = Cur_Width - 394;
    int cur_pos_x = 394;
    int pixel_color = COL_SCOPESSAMPLES;
    int scope_pos = Get_Song_Position();
    int start_pos_x;

    SetColor(COL_BACKGROUND);
    Fillrect(394, 42, Cur_Width - 1, 179);

    // Tracks
    ptrTbl_Dat = &VuMeters_Table_Dats[VuMeters_Table[Song_Tracks].offset];
    spread_remainder = 0;
    for(i = 0; i < VuMeters_Table[Song_Tracks].nbr; i++)
    {
        if(ptrTbl_Dat->x_div)
        {
            nibble_pos = ((max_right / ptrTbl_Dat->x_div) * ptrTbl_Dat->x_mul);
        }
        else
        {
            nibble_pos = 0;
            cur_pos_x = 394;
        }
        // Print the number of the track
        if(Chan_Active_State[scope_pos][i])
        {
            Print_String(cur_pos_x + 4, 44 + (ptrTbl_Dat->y_pos - ptrTbl_Dat->y_large),
                         USE_FONT,
                         table_channels_scopes[i]);
            active_channel = TRUE;
        }
        else
        {
            Print_String(cur_pos_x + 4, 44 + (ptrTbl_Dat->y_pos - ptrTbl_Dat->y_large),
                         USE_FONT_LOW,
                         table_channels_scopes[i]);
            active_channel = FALSE;
        }
        x_max = max_right / ptrTbl_Dat->x_max;
        if(!nibble_pos)
        {
            spread_remainder = (max_right % ptrTbl_Dat->x_max) / VuMeters_Table[Song_Tracks].nbr;
        }
        MaxLevel_L = 0.0f;  
        MaxLevel_R = 0.0f;
        start_pos_x = cur_pos_x;
        x_max_half = x_max >> 2;
        x_max_half_start = x_max_half;

        data = VuMeters_Level_Dats_L[i];
        if(data > 1.0f)
        {
            data = 1.0f;
        }
        if(data > MaxLevel_L)
        {
            MaxLevel_L = data;
        }

        data = VuMeters_Level_Dats_R[i];
        if(data > 1.0f)
        {
            data = 1.0f;
        }
        if(data > MaxLevel_R)
        {
            MaxLevel_R = data;
        }

        MaxLevel_R *= (float) (ptrTbl_Dat->y_large << 1);
        MaxLevel_L *= (float) (ptrTbl_Dat->y_large << 1);
        peak_line = (ptrTbl_Dat->y_large << 1) - (ptrTbl_Dat->y_large >> 1);
        i_MaxLevel_L = MaxLevel_L;
        i_MaxLevel_R = MaxLevel_R;
        bottom_line = 44 + (ptrTbl_Dat->y_pos + ptrTbl_Dat->y_large);
        if(i_MaxLevel_L > bottom_line)
        {
            i_MaxLevel_L = bottom_line;
        }
        if(i_MaxLevel_R > bottom_line)
        {
            i_MaxLevel_R = bottom_line;
        }

        for(j = 0; j < i_MaxLevel_L + 1; j += 2)
        {
            if(j > peak_line)
            {
                DrawHLine(bottom_line - j, start_pos_x + x_max_half_start - 2, start_pos_x + x_max_half_start + x_max_half - 1, COL_VUMETERPEAK);
            }
            else
            {
                DrawHLine(bottom_line - j, start_pos_x  + x_max_half_start - 2, start_pos_x  + x_max_half_start + x_max_half  - 1, COL_VUMETER);
            }
        }

        for(j = 0; j < i_MaxLevel_R + 1; j += 2)
        {
            if(j > peak_line)
            {
                DrawHLine(bottom_line - j, start_pos_x + x_max_half_start + x_max_half + 1, start_pos_x  + x_max_half_start + x_max_half + x_max_half + 2, COL_VUMETERPEAK);
            }
            else
            {
                DrawHLine(bottom_line - j, start_pos_x + x_max_half_start + x_max_half + 1, start_pos_x  + x_max_half_start + x_max_half + x_max_half + 2, COL_VUMETER);
            }
        }
        x_max += spread_remainder;
        cur_pos_x += x_max;
        ptrTbl_Dat++;
        pixel_color = COL_SCOPESSAMPLES;
    }
}

void Display_Dirs_Icons(int Idx)
{
    int Tab_Highlight[] =
    {
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_PUSHED, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_PUSHED,
        BUTTON_PUSHED, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_PUSHED, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_PUSHED, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_PUSHED, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_PUSHED, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_PUSHED, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL,
        BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_NORMAL, BUTTON_PUSHED, BUTTON_NORMAL, BUTTON_NORMAL
    };

    Idx *= 9;

    Gui_Draw_Button_Box(Cur_Width - 126, 24, 16, 16, "M", Tab_Highlight[0 + Idx] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(Cur_Width - 108, 24, 16, 16, "I", Tab_Highlight[1 + Idx] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(Cur_Width - 90, 24, 16, 16, "S", Tab_Highlight[2 + Idx] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(Cur_Width - 72, 24, 16, 16, "P", Tab_Highlight[3 + Idx] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(Cur_Width - 54, 24, 16, 16, "R", Tab_Highlight[4 + Idx] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(Cur_Width - 36, 24, 16, 16, "B", Tab_Highlight[5 + Idx] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(Cur_Width - 18, 24, 16, 16, "A", Tab_Highlight[6 + Idx] | BUTTON_TEXT_CENTERED);
    
    Gui_Draw_Button_Box(Cur_Width - 36, 6, 16, 16, "In", Tab_Highlight[7 + Idx] | BUTTON_TEXT_CENTERED);
    Gui_Draw_Button_Box(Cur_Width - 18, 6, 16, 16, "Sy", Tab_Highlight[8 + Idx] | BUTTON_TEXT_CENTERED);
}

void Draw_Scope_Files_Button(void)
{
    switch(Scopish)
    {
        case SCOPE_ZONE_VUMETERS:
            Draw_VuMeters();
            Gui_Draw_Button_Box(394, 24, Cur_Width - 522, 16, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
            Gui_Draw_Button_Box(Cur_Width - 72, 6, 16, 16, "\215", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(Cur_Width - 54, 6, 16, 16, "\255", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);
            
            Display_Dirs_Icons(0);
            break;

        case SCOPE_ZONE_SCOPE:
            Draw_Scope();
            Gui_Draw_Button_Box(394, 24, Cur_Width - 522, 16, NULL, BUTTON_NORMAL | BUTTON_DISABLED);
            Gui_Draw_Button_Box(Cur_Width - 72, 6, 16, 16, "\215", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(Cur_Width - 54, 6, 16, 16, "\255", BUTTON_PUSHED | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);

            Display_Dirs_Icons(0);
            break;

        case SCOPE_ZONE_INSTR_LIST:
            Actualize_Instruments_Synths_List(0);
            Gui_Draw_Button_Box(Cur_Width - 72, 6, 16, 16, "\215", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(Cur_Width - 54, 6, 16, 16, "\255", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);

            Display_Dirs_Icons(1);
            break;

        case SCOPE_ZONE_SYNTH_LIST:
            Actualize_Instruments_Synths_List(0);
            Gui_Draw_Button_Box(Cur_Width - 72, 6, 16, 16, "\215", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(Cur_Width - 54, 6, 16, 16, "\255", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);

            Display_Dirs_Icons(2);
            break;

        case SCOPE_ZONE_MOD_DIR:
        case SCOPE_ZONE_INSTR_DIR:
        case SCOPE_ZONE_PRESET_DIR:
        case SCOPE_ZONE_REVERB_DIR:
        case SCOPE_ZONE_PATTERN_DIR:
        case SCOPE_ZONE_MIDICFG_DIR:
        case SCOPE_ZONE_SAMPLE_DIR:

            Read_SMPT();
            Dump_Files_List(395, 41);
            Actualize_Files_List(0);
            Gui_Draw_Button_Box(Cur_Width - 72, 6, 16, 16, "\215", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
            Gui_Draw_Button_Box(Cur_Width - 54, 6, 16, 16, "\255", BUTTON_NORMAL | BUTTON_TEXT_CENTERED | BUTTON_RIGHT_MOUSE);

            switch(Scopish)
            {
                case SCOPE_ZONE_MOD_DIR:
                    Display_Dirs_Icons(3);
                    break;

                case SCOPE_ZONE_INSTR_DIR:
                    Display_Dirs_Icons(4);
                    break;

                case SCOPE_ZONE_SAMPLE_DIR:
                    Display_Dirs_Icons(5);
                    break;

                case SCOPE_ZONE_PRESET_DIR:
                    Display_Dirs_Icons(6);
                    break;

                case SCOPE_ZONE_REVERB_DIR:
                    Display_Dirs_Icons(7);
                    break;

                case SCOPE_ZONE_PATTERN_DIR:
                    Display_Dirs_Icons(8);
                    break;

                case SCOPE_ZONE_MIDICFG_DIR:
                    Display_Dirs_Icons(9);
                    break;
            }
            break;
    }
}

// ------------------------------------------------------
// Free all allocated midi channels and sub channels
void Clear_Midi_Channels_Pool(void)
{
    int i;
    int j;

    for(j = 0; j < MAX_POLYPHONY; j++)
    {
        for(i = 0; i < MAX_TRACKS; i++)
        {
            Alloc_midi_Channels[i][j] = 0;
        }
    }
}

// ------------------------------------------------------
// Schedule a note
void Send_Note(int Note, int Raw_Note, int One_Channel)
{
    // Provision to send
    Keyboard_Events[Keyboard_Nbr_Events] = Note;
    Keyboard_Notes_Type[Keyboard_Nbr_Events] = Raw_Note;
    Keyboard_Notes_Bound[Keyboard_Nbr_Events] = One_Channel;
    Keyboard_Nbr_Events++;
    if(Keyboard_Nbr_Events >= 256) Keyboard_Nbr_Events = 0;
}

// ------------------------------------------------------
// Jazz a note man
void Note_Jazz(int track, int note, float volume)
{
    // Play the note
    int Sub_Channel = Get_Free_Sub_Channel(track, Channels_Polyphony[track]);

    if(Sub_Channel == -1)
    {
        // We didn't have enough channel
        Sub_Channel = 0;
    }
    Sub_Channels_Jazz[track][Sub_Channel].Note = ((note + 1) << 8);
    Sub_Channels_Jazz[track][Sub_Channel].Channel = track;
    Sub_Channels_Jazz[track][Sub_Channel].Sub_Channel = Sub_Channel;
    local_mas_vol = 1.0f;
    local_ramp_vol = 1.0f;

    if(Jazz_Edit || is_recording_2 || !is_editing)
    {
        sp_Tvol_Mod[track] = 1.0f;
        Schedule_Instrument(track,
                            Sub_Channel,
                            note,
                            Current_Instrument,
                            0,
                            0,
                            !is_recording,
                            -(Sub_Channel + 1),
                            volume,
                            0,
                            0);
    }
}

// ------------------------------------------------------
// Jazz a note off man
void Note_Jazz_Off(int note)
{
    // Play a note off
    LPJAZZ_KEY Channel = Get_Jazz_Key_Off(Sub_Channels_Jazz, (note + 1) << 8);
    if(Channel)
    {
        if(Jazz_Edit || is_recording_2 || !is_editing)
        {
            Synthesizer[Channel->Channel][Channel->Sub_Channel].Note_Off();
            if(sp_Stage[Channel->Channel][Channel->Sub_Channel] == PLAYING_SAMPLE)
            {
                sp_Stage[Channel->Channel][Channel->Sub_Channel] = PLAYING_SAMPLE_NOTEOFF;
            }
            // That's a possibility
            Note_Off_303(Channel->Channel);
        }
        Channel->Note = 0;
        Channel->Channel = 0;
        Channel->Sub_Channel = 0;
    }

#if !defined(__STAND_ALONE__)
#if !defined(__NO_MIDI__)
    if(Jazz_Edit || is_recording_2 || !is_editing)
    {
        Midi_Note_Off(Track_Under_Caret, note);
    }
#endif
#endif

}

// ------------------------------------------------------
// Lock & unlock the audio thread so we can perform modifications
void Lock_Audio_Thread(void)
{
    if(thread_sema)
    {
        SDL_SemWait(thread_sema);
    }
}

void Unlock_Audio_Thread(void)
{
    if(thread_sema)
    {
        SDL_SemPost(thread_sema);
    }
}

// ------------------------------------------------------
// The status of the patterns zoom
void Display_Patterns_Sizes_Status(void)
{
    int i;
    int all_same = BUTTON_DISABLED;

    for(i = 0; i < Song_Tracks; i++)
    {
        if(Get_Track_Zoom(i) != TRACK_SMALL)
        {
            all_same = 0;
            break;
        }
    }
    Gui_Draw_Button_Box(332, 126, 16, 16, "\003", BUTTON_NORMAL | all_same | BUTTON_TEXT_CENTERED);

    all_same = BUTTON_DISABLED;
    for(i = 0; i < Song_Tracks; i++)
    {
        if(Get_Track_Zoom(i) != TRACK_LARGE)
        {
            all_same = 0;
            break;
        }
    }
    Gui_Draw_Button_Box(332 + 18, 126, 16, 16, "\004", BUTTON_NORMAL | all_same | BUTTON_TEXT_CENTERED);

    if(pattern_double)
    {
        Gui_Draw_Button_Box(332 + (18 * 2), 126, 16, 16, "\017", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    }
    else
    {
        Gui_Draw_Button_Box(332 + (18 * 2), 126, 16, 16, "\016", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    }
    Display_Patterns_Sliders_Status();
}

// ------------------------------------------------------
// The status of the patterns sliders
void Display_Patterns_Sliders_Status(void)
{
    if(pattern_double && pattern_sliders)
    {
        if(pattern_sliders_numbers)
        {
            Gui_Draw_Button_Box(368, 108, 16, 16, "00", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
        }
        else
        {
            Gui_Draw_Button_Box(368, 108, 16, 16, "00", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
        }
    }
    else
    {
        Gui_Draw_Button_Box(368, 108, 16, 16, "00", BUTTON_NORMAL | BUTTON_DISABLED | BUTTON_TEXT_CENTERED);
    }
    if(pattern_sliders)
    {
        Gui_Draw_Button_Box(350, 108, 16, 16, "Sli", BUTTON_PUSHED | BUTTON_TEXT_CENTERED);
    }
    else
    {
        Gui_Draw_Button_Box(350, 108, 16, 16, "Sli", BUTTON_NORMAL | BUTTON_TEXT_CENTERED);
    }
    Gui_Draw_Button_Box(320, 126, 10, 16, NULL, BUTTON_DISABLED);
    Sanitize_Sliders_Block();
}

// ------------------------------------------------------
// The status of the right shift key in edit mode
void Display_RShift_Status(void)
{
    if(is_editing && pattern_sliders)
    {
        if(RShift_Notification)
        {
            if(RShift_Notified)
            {
                SetColor(COL_VUMETERPEAK);
                Fillrect(320 + 1, 126 + 1, 320 + 1 + 9, 126  + 1 + 15);
                RShift_Notified = FALSE;
            }
        }
        else
        {
            if(RShift_Notified)
            {
                Gui_Draw_Button_Box(320, 126, 10, 16, NULL, BUTTON_DISABLED);
                RShift_Notified = FALSE;
            }
        }
    }
}
