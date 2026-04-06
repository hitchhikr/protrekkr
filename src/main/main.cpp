// ------------------------------------------------------
// Protrekkr
// Based on Juan Antonio Arguelles Rius's NoiseTrekker.
//
// Copyright (C) 2008-2026 Franck Charlet.
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

#if defined(__AROS__)
#include <cstdlib>
#define SDL_strcasecmp strcasecmp
#elif defined(__MORPHOS__)
#include <cstdlib>
#define SDL_strcasecmp strcasecmp
#else
#define NEED_SDL_GETENV
#endif

#include <string.h>
#include "include/main.h"
#include "include/timer.h"
#include "include/resource.h"

#include "../files/include/files.h"
#include "../files/include/config.h"
#include "../ui/include/misc_draw.h"

#if defined(__WIN32__)
#include <windows.h>
#if !defined(__GCC__)
#include <shlwapi.h>
#endif
#endif

#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
#include <mach-o/dyld.h>
#endif

#if defined(__HAIKU__)
#include <libgen.h>
#endif

#if defined(__AMIGAOS4__) || defined(__AROS__) || defined(__MORPHOS__)

#if defined(__AMIGAOS4__)
#if defined(__CROSS__)
#ifdef __cplusplus
extern "C"
{
#endif
void __eabi(void)
{ 
}
#ifdef __cplusplus
}
#endif
#endif
#endif

const char *AMIGA_VERSION = "\0$VER: " TITLE " " VER_VER "." VER_REV "." VER_REVSMALL "\0";
#endif

// ------------------------------------------------------
// Types
typedef struct
{
    int code;
    int trans;
} KEYCORE, LPKEYCORE;

// ------------------------------------------------------
// Variables
#if defined(__WIN32__)
HWND Main_Win32_Win;
SDL_SysWMinfo WMInfo;
#endif
SDL_DisplayMode Current_Screen_Mode;
SDL_DisplayMode Screen_Modes[1024];
SDL_Window *Main_Window;
SDL_Renderer *Main_Renderer;
PTK_SURFACE *Main_Screen;
SDL_Texture *Main_Texture;
SDL_GLContext Main_Context;
SDL_Event Events[MAX_EVENTS];
SDL_sem *thread_semaphore;
int window_shown = FALSE;

REQUESTER Title_Requester =
{
    "",
    NULL,
    &LOGOPIC, 5
};

extern int file_loaded;

int Orig_Screen_Width;
int Orig_Screen_Height;
int Startup_Width;
int Startup_Height;
int Burn_Title = FALSE;
int global_argc;
char global_argv[MAX_PATH];
int delay_ms = 10;

int Cur_Screen_Mode = -1;
int Max_Screen_Mode = 0;

MOUSE Mouse;
int Current_Keys = 0;
unsigned int Keys[SDL_NUM_SCANCODES + 1];
unsigned int Keys_Raw_Off[SDL_NUM_SCANCODES + 1];
unsigned int Keys_Raw[SDL_NUM_SCANCODES + 1];
int Keyboard_Nbr_Events;
int Keyboard_Events[256];
int Keyboard_Notes_Type[256];
int Keyboard_Notes_Bound[256];
//int Key_Unicode;
char FullScreen = FALSE;
int Cur_Left = SDL_WINDOWPOS_CENTERED;
int Cur_Top = SDL_WINDOWPOS_CENTERED;
int Cur_Width = SCREEN_WIDTH;
int Cur_Height = SCREEN_HEIGHT;
int Windowed_Width = SCREEN_WIDTH;
int Windowed_Height = SCREEN_HEIGHT;
int FullScreen_Width = SCREEN_WIDTH;
int FullScreen_Height = SCREEN_HEIGHT;
int Save_Cur_Width = -1;
int Save_Cur_Height = -1;
int First_Time;
int do_resize = FALSE;
char AutoSave;
char Window_Title[256];
extern int gui_pushed;
int Env_Change;
int key_on = 0;
float delay_refresh;
float delay_refresh2;

char *ExePath;
extern char AutoReload;
extern char SplashScreen;
char Last_Used_Ptk[MAX_PATH];

extern int Enter_Notification;
extern int Enter_Notified;
extern int Enter_Notified_Play_Pattern;
extern int RShift_Notification;
extern int RShift_Notified;

int Nbr_Keyboards;
int Keyboard_Idx;
char Keyboard_Labels[256][256];
char Keyboard_FileNames[256][256];

const char *Keyboards;
char Keyboard_Name[MAX_PATH];
char Keyboards_FileName[MAX_PATH];

// The currently used keyboard
// code,trans,base
KEYCORE Keyboard[] =
{
    { 0, 0x03 },     // Do# / C#
    { 0, 0x04 },     // Re# / D#
    { 0, 0x06 },     // Fa# / F#
    { 0, 0x07 },     // Sol# / G#
    { 0, 0x08 },     // La# / A#
    { 0, 0x0a },     // Do# / C#
    { 0, 0x0b },     // Re# / D#
    { 0, 0x0d },     // Fa# / F#

    { 0, 0x10 },     // Do / C
    { 0, 0x11 },     // Re / D
    { 0, 0x12 },     // Mi / E
    { 0, 0x13 },     // Fa / F
    { 0, 0x14 },     // Sol / G
    { 0, 0x15 },     // La / A
    { 0, 0x16 },     // Si / B
    { 0, 0x17 },     // Do / C
    { 0, 0x18 },     // Re / D
    { 0, 0x19 },     // Mi / E
    { 0, 0x1a },     // Fa / F
    { 0, 0x1b },     // Sol / G

    { 0, 0x02 },     // Off
    { 0, 0x1f },     // Do# / etc.
    { 0, 0x20 },     // Re#
    { 0, 0x22 },     // Fa#
    { 0, 0x23 },     // Sol#
    { 0, 0x24 },     // La#
    { 0, 0x26 },     // Do#
    { 0, 0x27 },     // Re#

    { 0, 0x2c },     // Do
    { 0, 0x2d },     // Re
    { 0, 0x2e },     // Mi
    { 0, 0x2f },     // Fa
    { 0, 0x30 },     // Sol
    { 0, 0x31 },     // La
    { 0, 0x32 },     // Si
    { 0, 0x33 },     // Do
    { 0, 0x34 },     // Re
    { 0, 0x35 },     // Mi
};

// English keyboard
KEYCORE Default_Keyboard[] =
{
    { '2', 0x03 },     // Do#
    { '3', 0x04 },     // Re#
    { '5', 0x06 },     // Fa#
    { '6', 0x07 },     // Sol#
    { '8', 0x08 },     // La#
    { '9', 0x0a },     // Do#
    { '0', 0x0b },     // Re#
    { '-', 0x0d },     // Fa#

    { 'q', 0x10 },     // Do
    { 'w', 0x11 },     // Re
    { 'e', 0x12 },     // Mi
    { 'r', 0x13 },     // Fa
    { 't', 0x14 },     // Sol
    { 'y', 0x15 },     // La
    { 'u', 0x16 },     // Si
    { 'i', 0x17 },     // Do
    { 'o', 0x18 },     // Re
    { 'p', 0x19 },     // Mi
    { '[', 0x1a },     // Fa
    { ']', 0x1b },     // Sol

    { 'a', 0x02 },     // Off
    { 's', 0x1f },     // Do#
    { 'd', 0x20 },     // Re#
    { 'g', 0x22 },     // Fa#
    { 'h', 0x23 },     // Sol#
    { 'j', 0x24 },     // La#
    { 'l', 0x26 },     // Do#
    { ';', 0x27 },     // Re#

    { 'z', 0x2c },     // Do
    { 'x', 0x2d },     // Re
    { 'c', 0x2e },     // Mi
    { 'v', 0x2f },     // Fa
    { 'b', 0x30 },     // Sol
    { 'n', 0x31 },     // La
    { 'm', 0x32 },     // Si
    { ',', 0x33 },     // Do
    { '.', 0x34 },     // Re
    { '/', 0x35 },     // Mi
};

LPKEYCORE *Cur_Keyboard = Default_Keyboard;

// ------------------------------------------------------
// Functions
int Open_Window(int Width, int Height);
void Switch_FullScreen();

int Get_LShift(void)
{
    if(SDL_GetModState() & KMOD_LSHIFT) return(TRUE);
    return(FALSE);
}

int Get_RShift(void)
{
    if(SDL_GetModState() & KMOD_RSHIFT) return(TRUE);
    return(FALSE);
}

int Get_Shift(void)
{
    if((SDL_GetModState() & KMOD_LSHIFT) | (SDL_GetModState() & KMOD_RSHIFT)) return(TRUE);
    return(FALSE);
}

int Get_Caps(void)
{
    if(SDL_GetModState() & KMOD_CAPS) return(TRUE);
    return(FALSE);
}

int Get_LAlt(void)
{
    if(SDL_GetModState() & KMOD_LALT) return(TRUE);
    return(FALSE);
}

int Get_RAlt(void)
{
    if(SDL_GetModState() & KMOD_RALT) return(TRUE);
    return(FALSE);
}

int Get_LCtrl(void)
{
    if(SDL_GetModState() & KMOD_LCTRL) return(TRUE);
    return(FALSE);
}

int Get_RCtrl(void)
{
    if(SDL_GetModState() & KMOD_RCTRL) return(TRUE);
    return(FALSE);
}

// ------------------------------------------------------
// SDL scancode doesn't make it, so we have to blast
int Translate_Locale_Key(int Key)
{
    int i;

    for(i = 0; i < (sizeof(Default_Keyboard) / sizeof(KEYCORE)); i++)
    {
        // apparently, windows doesn't care about that localization
        if(Cur_Keyboard[i].code == Key)
        {
            return(Cur_Keyboard[i].trans);
        }
    }
    return(0);
}

// ------------------------------------------------------
// Get the full name of the currently selected keyboard
char *Get_Keyboard_Label(void)
{
    return(Keyboard_Labels[Keyboard_Idx]);
}

// ------------------------------------------------------
// Get the filename of the currently selected keyboard
char *Get_Keyboard_FileName(void)
{
    return(Keyboard_FileNames[Keyboard_Idx]);
}

// ------------------------------------------------------
// Load a keyboard definition file
void Load_Keyboard_Def(char *FileName)
{
    char KbData[64];
    FILE *KbFile;
    int j;
    int Key_Value;
    char *KbDataEnd;
    char KbFileName[MAX_PATH];
    char Keyboard_Label[256];

#if defined(__WIN32__)
    strcpy(KbFileName, ExePath);
    strcat(KbFileName, "\\skins\\");
#else
    strcpy(KbFileName, ExePath);
    strcat(KbFileName, "/skins/");
#endif

    strcat(KbFileName, FileName);

    // Current keyboard filename (saved in ptk.cfg)
    sprintf(Keyboard_Name, "%s", FileName);

    KbFile = fopen(KbFileName, "r");
    if(KbFile)
    {
        fscanf(KbFile, "%s", &Keyboard_Label);
        j = 0;
        while(!feof(KbFile) && j < (sizeof(Default_Keyboard) / sizeof(KEYCORE)))
        {
            // Retrieve the data and store it
            fscanf(KbFile, "%s", &KbData);
            if(KbData[0] == '0' && (KbData[1] == 'x' || KbData[1] == 'X'))
            {
                // Hexadecimal data
                Key_Value = strtol(KbData, &KbDataEnd, 16);
            }
            else
            {
                // Plain ASCII
                Key_Value = KbData[0];
            }
            Keyboard[j].code = Key_Value;
            j++;
        }
        fclose(KbFile);
    }
}

// ------------------------------------------------------
// Retrieve the video infos
void Get_Vid_Infos()
{
    int display_count = SDL_GetNumVideoDisplays();
    Max_Screen_Mode = 0;

    for (int display_index = 0; display_index <= display_count; display_index++)
    {
        int modes_count = SDL_GetNumDisplayModes(display_index);
        for (int mode_index = 0; mode_index <= modes_count; mode_index++)
        {
            SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
            if (SDL_GetDisplayMode(display_index, mode_index, &mode) == 0)
            {
                if(Current_Screen_Mode.refresh_rate == mode.refresh_rate)
                {
                    memcpy(&Screen_Modes[Max_Screen_Mode], &mode, sizeof(SDL_DisplayMode));
                    Max_Screen_Mode++;
                }
            }
        }
    }
}

// ------------------------------------------------------
// Retrieve a screen dimensions
SDL_DisplayMode *Get_Screen_Rect(int mode_index)
{
    return &Screen_Modes[mode_index];
}

// ------------------------------------------------------
// Retrieve the position of the window
void Get_Window_Pos(void)
{
    SDL_GetWindowPosition(Main_Window, &Cur_Left, &Cur_Top);
}

// ------------------------------------------------------
// Set the position of the window
void Set_Window_Pos(void)
{
    SDL_SetWindowPosition(Main_Window, Cur_Left, Cur_Top);
}

// ------------------------------------------------------
// Use a thrad semaphore to lock & unlock the audio thread so we can perform modifications
void Lock_Audio_Thread(void)
{
    if(thread_semaphore)
    {
        SDL_SemWait(thread_semaphore);
    }
}

void Unlock_Audio_Thread(void)
{
    if(thread_semaphore)
    {
        SDL_SemPost(thread_semaphore);
    }
}

void Create_Semaphore(void)
{
    thread_semaphore = SDL_CreateSemaphore(1);
}

void Destroy_Semaphore(void)
{
    if(thread_semaphore)
    {
        SDL_DestroySemaphore(thread_semaphore);
    }
}

// ------------------------------------------------------
// Main part of the tracker interface
int main(int argc, char *argv[])
{
    SDL_KeyboardEvent *kb_evnt;

    char KbFileName[MAX_PATH];
    char KbFileNameToLoad[MAX_PATH];
    char KbData[64];
    char *KbDataEnd;
    int i;
    int j;
    int Key_Value;
//    int Uni_Trans;
    FILE *KbFile;
    FILE *AllKbsFile;
    int in_note;
    UINT32 ExePath_Size = MAX_PATH;

#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
    UINT32 Path_Length;
#endif

    SDL_SetHint(SDL_HINT_TRACKPAD_IS_TOUCH_ONLY, "1");

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0)
    {
        Message_Error((char *) SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    // Show the restrictions:
    char *NoMidi = "";

#if defined(__NO_MIDI__)
    NoMidi = "No midi";
#endif

    if(strlen(NoMidi))
    {
        sprintf(Window_Title, "%s - Build restrictions: %s", VERSION, NoMidi);
    }
    else
    {
        sprintf(Window_Title, "%s", VERSION);
    }

    ExePath = (char *) malloc(ExePath_Size + 1);
    if(ExePath == NULL)
    {
        Message_Error("Can't open alloc memory.");
        SDL_Quit();
        return EXIT_FAILURE;
    }
    memset(ExePath, 0, ExePath_Size + 1);

#if defined(__LINUX__)

    #if defined(__FREEBSD__) || defined(__NETBSD__) 
        GETCWD(ExePath, MAX_PATH);
    #else
        // Note:
        // it looks like some distros don't have /proc/self
        // Maybe a better (?) solution would be to use:
        // sprintf(ExeProc, "/proc/$d/exe", getpid());
        // readlink(ExeProc, ExePath, sizeof(ExePath));
        readlink("/proc/self/exe", ExePath, ExePath_Size);
        int exename_len = strlen(ExePath);
        while(exename_len--)
        {
            if(ExePath[exename_len] == '/')
            {
                ExePath[exename_len] = 0;
                exename_len++;
                break;
            }
        }
        CHDIR(ExePath);
    #endif

#elif defined(__HAIKU__)
    chdir(dirname(argv[0]));
    GETCWD(ExePath, MAX_PATH);

#elif defined(__AMIGAOS4__) || defined(__AROS__) || defined(__MORPHOS__)
    CHDIR("/PROGDIR/");
    GETCWD(ExePath, MAX_PATH);

#else
    #if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
        Path_Length = ExePath_Size;
        _NSGetExecutablePath(ExePath, &Path_Length);
        i = 0;
        // Remove 4 levels
        while(i < 4)
        {
            Path_Length--;
            if(ExePath[Path_Length] == '/')
            {
                ExePath[Path_Length] = 0;
                i++;
            }
        }
        CHDIR(ExePath);
    #else
        GETCWD(ExePath, MAX_PATH);
    #endif
#endif

    // Default values
#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
    sprintf(Keyboard_Name, "%s", "kben_macosx.txt");
    sprintf(Keyboard_FileNames[0], "%s", "kben_macosx.txt");
#else
    sprintf(Keyboard_Name, "%s", "kben.txt");
    sprintf(Keyboard_FileNames[0], "%s", "kben.txt");
#endif
    sprintf(Keyboard_Labels[0], "%s", "English");
    Nbr_Keyboards = 1;
    Cur_Keyboard = Default_Keyboard;

    // Set the default palette before loading the config file
    Restore_Default_Palette(Default_Palette1, Default_Beveled1);
    Load_Config();

    SDL_GetCurrentDisplayMode(0, &Current_Screen_Mode);
    Get_Vid_Infos();
    if(Cur_Screen_Mode == -1)
    {
        Orig_Screen_Width = Current_Screen_Mode.w;
        Orig_Screen_Height = Current_Screen_Mode.h;
        Startup_Width = Orig_Screen_Width;
        Startup_Height = Orig_Screen_Height;
        if(Startup_Width < SCREEN_WIDTH) Startup_Width = SCREEN_WIDTH;
        if(Startup_Height < SCREEN_HEIGHT) Startup_Height = SCREEN_HEIGHT;

        Cur_Screen_Mode = 0;
        i = 0;
        while(i < Max_Screen_Mode)
        {
            if(Screen_Modes[i].w == Current_Screen_Mode.w &&
               Screen_Modes[i].h == Current_Screen_Mode.h)
            {
                // Found it in the list: set
                Cur_Screen_Mode = i;
                break;
            }
            i++;
        }
    }
    else
    {
        Orig_Screen_Width = Screen_Modes[Cur_Screen_Mode].w;
        Orig_Screen_Height = Screen_Modes[Cur_Screen_Mode].h;
        Startup_Width = Orig_Screen_Width;
        Startup_Height = Orig_Screen_Height;
        if(Startup_Width < SCREEN_WIDTH) Startup_Width = SCREEN_WIDTH;
        if(Startup_Height < SCREEN_HEIGHT) Startup_Height = SCREEN_HEIGHT;
    }

    FullScreen_Width = Startup_Width;
    FullScreen_Height = Startup_Height;

    if(FullScreen)
    {
        Save_Cur_Width = Cur_Width;
        Save_Cur_Height = Cur_Height;
        // We start with in full screen mode
        Cur_Left = SDL_WINDOWPOS_CENTERED;
        Cur_Top = SDL_WINDOWPOS_CENTERED;
    }

    if(!strlen(Keyboard_Name))
    {
#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
        sprintf(Keyboard_Name, "%s", "kben_macosx.txt");
#else
        sprintf(Keyboard_Name, "%s", "kben.txt");
#endif
    }

    // All keyboards name
#if defined(__WIN32__)
    strcpy(Keyboards_FileName, ExePath);
    strcat(Keyboards_FileName, "\\skins\\");
#else
    strcpy(Keyboards_FileName, ExePath);
    strcat(Keyboards_FileName, "/skins/");
#endif

#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
    strcat(Keyboards_FileName, "keyboards_macosx.txt");
#else
    strcat(Keyboards_FileName, "keyboards.txt");
#endif

    AllKbsFile = fopen(Keyboards_FileName, "r");
    if(AllKbsFile != NULL)
    {
        memset(KbFileNameToLoad, 0, sizeof(KbFileNameToLoad));

        Nbr_Keyboards = 0;

        i = 0;
        while(!feof(AllKbsFile))
        {
            fscanf(AllKbsFile, "%s", &KbFileName);

#if defined(__WIN32__)
            strcpy(KbFileNameToLoad, ExePath);
            strcat(KbFileNameToLoad, "\\skins\\");
#else
            strcpy(KbFileNameToLoad, ExePath);
            strcat(KbFileNameToLoad, "/skins/");
#endif

            strcat(KbFileNameToLoad, KbFileName);

            // Load the keyboard file

            // Store it
            sprintf(Keyboard_FileNames[i], "%s", KbFileName);
            sprintf(Keyboard_Labels[i], "%s", "");

            KbFile = fopen(KbFileNameToLoad, "r");
            if(KbFile)
            {
                // Get the full name
                fscanf(KbFile, "%s", &Keyboard_Labels[i]);
                Nbr_Keyboards++;

                if(SDL_strcasecmp(KbFileName, Keyboard_Name) == 0)
                {
                    Keyboard_Idx = i;
                    // Parse it
                    j = 0;
                    Cur_Keyboard = Keyboard;
                    while(!feof(KbFile) && j < (sizeof(Default_Keyboard) / sizeof(KEYCORE)))
                    {
                        // Retrieve the data and store it
                        fscanf(KbFile, "%s", &KbData);
                        if(KbData[0] == '0' && (KbData[1] == 'x' || KbData[1] == 'X'))
                        {
                            // Hexadecimal data
                            Key_Value = strtol(KbData, &KbDataEnd, 16);
                        }
                        else
                        {
                            // Plain ASCII
                            Key_Value = (unsigned char) KbData[0];
                        }
                        Keyboard[j].code = Key_Value;
                        j++;
                    }
                }
                fclose(KbFile);
            }
            i++;
        }
        fclose(AllKbsFile);
    }

    // Avoid a possible flash
    int Save_R = Ptk_Palette[0].r;
    int Save_G = Ptk_Palette[0].g;
    int Save_B = Ptk_Palette[0].b;

    Ptk_Palette[0].r = 0;
    Ptk_Palette[0].g = 0;
    Ptk_Palette[0].b = 0;

    First_Time = TRUE;
    if(!Open_Window(Cur_Width, Cur_Height))
    {
        Message_Error((char *) SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    Ptk_Palette[0].r = Save_R;
    Ptk_Palette[0].g = Save_G;
    Ptk_Palette[0].b = Save_B;

#if !defined(__NO_MIDI__)
    // Load midi devices infos
    Midi_GetAll();
#endif

    SDL_GetMouseState((int *) &Mouse.x, (int *) &Mouse.y);

    if(!Init_Context())
    {
        Destroy_Context();
        SDL_Quit();
        Message_Error("Init_Context().");
        return EXIT_FAILURE;
    }

#if defined(__AROS__) || defined(__AMIGAOS4__) || defined(__MORPHOS__)
    char *env_var;

    env_var = getenv("PROTREKKR_MAIN_LOOP_DELAY");
    if(env_var)
    {
        delay_ms = atol(env_var);
    }
    else
    {
        delay_ms = 10;
    }
    if(delay_ms < 10)
    {
        delay_ms = 10;
    }
    if(delay_ms > 1000)
    {
        delay_ms = 1000;
    }
#endif

    global_argc = argc;

    // Check if there's an argument
    global_argv[0] = 0;
    if(argc != 1)
    {
        strcpy(global_argv, argv[1]);
    }

    do_resize = TRUE;
    SDL_SetHint(SDL_HINT_TRACKPAD_IS_TOUCH_ONLY, "1");

    while(1)
    {
        Mouse.wheel_x = 0;
        Mouse.wheel_y = 0;
        Mouse.touch_location_x = 0;
        Mouse.touch_location_y = 0;
        Mouse.zoom = 0;
        if(Mouse.button_oneshot & MOUSE_LEFT_BUTTON) Mouse.button_oneshot &= ~MOUSE_LEFT_BUTTON;
        if(Mouse.button_oneshot & MOUSE_MIDDLE_BUTTON) Mouse.button_oneshot &= ~MOUSE_MIDDLE_BUTTON;
        if(Mouse.button_oneshot & MOUSE_RIGHT_BUTTON) Mouse.button_oneshot &= ~MOUSE_RIGHT_BUTTON;
        memset(Keys, 0, sizeof(Keys));
        Current_Keys = 0;

        SDL_PumpEvents();
        int Nbr_Events = SDL_PeepEvents(Events, MAX_EVENTS, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
        int Symbol;
        int Scancode;

        for(int i = 0; i < Nbr_Events; i++)
        {
            if(In_Requester)
            {
                if(Current_Requester == NULL)
                {
                    In_Requester = FALSE;
                    memset(Keys, 0, sizeof(Keys));
                    memset(Keys_Raw, 0, sizeof(Keys_Raw));
                    memset(Keys_Raw_Off, 0, sizeof(Keys_Raw_Off));
                }
            }

            switch(Events[i].type)
            {
                case SDL_KEYDOWN:
                    Symbol = SDL_GetKeyFromScancode(Events[i].key.keysym.scancode);
                    if (Current_Keys < SDL_NUM_SCANCODES)
                    {
                        Current_Keys++;
                    }
                    Keys[Current_Keys] = Symbol;
                    
                    if(!In_Requester)
                    {
                        Scancode = Translate_Locale_Key(SDL_GetKeyFromScancode(Events[i].key.keysym.scancode));

                        Keys_Raw[Scancode] = TRUE;
                        Keys_Raw_Off[Scancode] = FALSE;

                        if(!is_recording_2 && is_editing)
                        {
                            in_note = FALSE;
                            for(j = 0; j < Channels_MultiNotes[Track_Under_Caret]; j++)
                            {
                                if(Column_Under_Caret == (j * 3))
                                {
                                    in_note = TRUE;
                                    break;
                                }
                            }
                            if(in_note)
                            {
                                if(!Get_LCtrl() && !Get_LShift() && !Get_LAlt())
                                {
                                    Send_Note(Scancode, FALSE, TRUE);
                                }
                            }
                        }

                        if(CHECK_KEY(SDLK_RETURN) || CHECK_KEY(SDLK_KP_ENTER) && Enter_Notification == FALSE &&
                            !Get_LAlt() && !Get_RAlt() && !Get_LCtrl() && !Get_RCtrl()
                           )
                        {
                            Enter_Notification = TRUE;
                            Enter_Notified = TRUE;
                            Enter_Notified_Play_Pattern = TRUE;
                            if(Get_LShift())
                            {
                                Enter_Notified_Play_Pattern = FALSE;
                            }
                        }

                        RShift_Notification = FALSE;
#if !defined(__MORPHOS__)
                        if(Get_RShift())
#else
                        if(Get_LAlt())
#endif
                        {
                            RShift_Notification = TRUE;
                            RShift_Notified = TRUE;
                        }
                    }

                    if(key_on != 2) key_on = 1;

                    if(Get_LAlt())
                    {
                        if(CHECK_KEY(SDLK_RETURN) && !Get_LCtrl() && !Get_RCtrl())
                        {
                            FullScreen ^= TRUE;
                            do_resize = TRUE;
                        }
                    }
                    break;

                case SDL_KEYUP:
                    kb_evnt = (SDL_KeyboardEvent *) &Events[i];
                    if(kb_evnt->state == SDL_RELEASED)
                    {
                        key_on = 0;
                        if(Current_Keys) Current_Keys--;
                    }

                    if(!In_Requester)
                    {
                        Scancode = Translate_Locale_Key(SDL_GetKeyFromScancode(Events[i].key.keysym.scancode));
                        
                        Keys_Raw_Off[Scancode] = TRUE;

                        if(!is_recording_2 && is_editing)
                        {
                            in_note = FALSE;
                            for(j = 0; j < Channels_MultiNotes[Track_Under_Caret]; j++)
                            {
                                if(Column_Under_Caret == (j * 3))
                                {
                                    in_note = TRUE;
                                    break;
                                }
                            }
                            if(in_note)
                            {
                                if(!Get_LCtrl() && !Get_LShift() && !Get_LAlt())
                                {
                                    Send_Note(Scancode | 0x80, FALSE, TRUE);
                                }
                            }
                        }
                        
                        if(CHECK_KEY(SDLK_RETURN) || CHECK_KEY(SDLK_KP_ENTER) || Get_LAlt() || Get_RAlt() || Get_LCtrl() || Get_RCtrl())
                        {
                        }
                        else
                        {
                            if(Enter_Notification)
                            {
                                Enter_Notification = FALSE;
                                Enter_Notified = TRUE;
                            }
                        }

#if !defined(__MORPHOS__)
                        if(Get_RShift())
#else
                        if(Get_LAlt())
#endif
                        {
                            RShift_Notification = TRUE;
                        }
                        else
                        {
                            RShift_Notification = FALSE;
                            RShift_Notified = TRUE;
                        }
                    }
                    break;

                case SDL_MULTIGESTURE:
                    if(Events[i].mgesture.numFingers == 3)
                    {
                        if(fabs(Events[i].mgesture.dDist) > 0.002)
                        {
                            Mouse.touch_location_x = (int) (Events[i].mgesture.x * Cur_Width);
                            Mouse.touch_location_y = (int) (Events[i].mgesture.y * Cur_Height);
                            if(Events[i].mgesture.dDist > 0)
                            {
                                Mouse.zoom = 1;
                            }
                            else
                            {
                                Mouse.zoom = -1;
                            }
                        }
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    if (Events[i].wheel.y != 0)
                    {
                        Mouse.wheel_y = Events[i].wheel.y;
                    }
                    if (Events[i].wheel.x != 0)
                    {
                        Mouse.wheel_x = Events[i].wheel.x;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    Mouse.x = Events[i].button.x;
                    Mouse.y = Events[i].button.y;
                    Mouse.y++;
                    switch (Events[i].button.state)
                    {
                        case SDL_PRESSED:
                            switch (Events[i].button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                    Mouse.button |= MOUSE_LEFT_BUTTON;
                                    Mouse.button_oneshot |= MOUSE_LEFT_BUTTON;
                                    break;

                                case SDL_BUTTON_MIDDLE:
                                    Mouse.button |= MOUSE_MIDDLE_BUTTON;
                                    Mouse.button_oneshot |= MOUSE_MIDDLE_BUTTON;
                                    break;

                                case SDL_BUTTON_RIGHT:
                                    Mouse.button |= MOUSE_RIGHT_BUTTON;
                                    Mouse.button_oneshot |= MOUSE_RIGHT_BUTTON;
                                    break;
                            }
                            break;

                        case SDL_RELEASED:
                            switch (Events[i].button.button)
                            {
                                case SDL_BUTTON_LEFT:
                                    Mouse.button &= ~MOUSE_LEFT_BUTTON;
                                    Mouse.button_oneshot &= ~MOUSE_LEFT_BUTTON;
                                    gui_pushed &= ~MOUSE_LEFT_BUTTON;
                                    break;
                                case SDL_BUTTON_MIDDLE:
                                    Mouse.button &= ~MOUSE_MIDDLE_BUTTON;
                                    Mouse.button_oneshot &= ~MOUSE_MIDDLE_BUTTON;
                                    gui_pushed &= ~MOUSE_MIDDLE_BUTTON;
                                    break;
                                case SDL_BUTTON_RIGHT:
                                    Mouse.button &= ~MOUSE_RIGHT_BUTTON;
                                    Mouse.button_oneshot &= ~MOUSE_RIGHT_BUTTON;
                                    gui_pushed &= ~MOUSE_RIGHT_BUTTON;
                                    break;
                            }
                            break;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    Mouse.x = Events[i].motion.x;
                    Mouse.y = Events[i].motion.y;
                    Mouse.y++;
                    break;

                case SDL_DROPFILE:
                    global_argc = 2;
                    file_loaded = FALSE;
                    strcpy(global_argv, Events[i].drop.file);
                    SDL_free(Events[i].drop.file);
                    break;
                case SDL_DROPCOMPLETE:
                    SDL_RaiseWindow(Main_Window);
                    break;

                case SDL_QUIT:
                    if(!In_Requester)
                    {
                        Display_Requester(&Exit_Requester, GUI_CMD_NOP, NULL, TRUE);
                    }
                    break;

                case SDL_RENDER_TARGETS_RESET:
                    do_resize = TRUE;
                    break;

                case SDL_WINDOWEVENT:
                    switch (Events[i].window.event)
                    {
                        case SDL_WINDOWEVENT_MOVED:
                            if (!FullScreen)
                            {
                                Cur_Left = Events[i].window.data1;
                                Cur_Top = Events[i].window.data2;
                            }
                            break;
                        case SDL_WINDOWEVENT_RESIZED:
                            // Nullify it
                            if (!FullScreen)
                            {
                                Windowed_Width = Events[i].window.data1;
                                Windowed_Height = Events[i].window.data2;
                                do_resize = TRUE;
                            }
                            break;

                        case SDL_WINDOWEVENT_RESTORED:
                        case SDL_WINDOWEVENT_EXPOSED:
//                            do_resize = TRUE;
                            break;

                        case SDL_WINDOWEVENT_ENTER:
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            memset(Keys, 0, sizeof(Keys));
                            SDL_Event event;
                            while (SDL_PollEvent(&event));
                            do_resize = TRUE;
                            break;

                        case SDL_WINDOWEVENT_LEAVE:
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                            memset(Keys, 0, sizeof(Keys));
                            break;

                    }
                    break;

                default:
                    break;
            }
        }

        if(do_resize)
        {
            Switch_FullScreen();
            do_resize = FALSE;
        }

        if(!Redraw_Screen())
        {
            break;
        }
    }

    Save_Config();

    if(ExePath)
    {
        free(ExePath);
    }

    Destroy_Context();
    SDL_Quit();
    return EXIT_SUCCESS;
}

// ------------------------------------------------------
// Display an error message somewhere
void Message_Error(char *Message)
{
    printf("Error: %s\n", Message);
}

// ------------------------------------------------------
// Flush the data onto the screen
void Flush_Screen(void)
{
    // Display the title requester once
    if(!Burn_Title && SplashScreen)
    {
        Display_Requester(&Title_Requester, GUI_CMD_REFRESH_PALETTE, NULL, TRUE);
        Burn_Title = TRUE;
    }
    if(!Burn_Title && !SplashScreen)
    {
        Burn_Title = TRUE;
        Kill_Requester();
    }
    Leave_2d_Mode();

#if !defined(__WIN32__) && !defined(__AROS__) && !defined(__AMIGAOS4__) && !defined(__MORPHOS__)
    glDrawBuffer(GL_FRONT);
    glRasterPos2f(-1.0f, -1.0f);
    glCopyPixels(0, 0, Cur_Width, Cur_Height, GL_COLOR);
    glDrawBuffer(GL_BACK);
    glFinish();
#else
    SDL_GL_SwapWindow(Main_Window);
#endif

    if(!window_shown)
    {
        // Only done at first opening to avoid
        // some nasty flashing
        SDL_ShowWindow(Main_Window);
        window_shown = TRUE;
    }
}

// ------------------------------------------------------
// Redraw the screen
int Redraw_Screen(void)
{
    Enter_2D_Mode((float) Cur_Width, (float) Cur_Height);

    if(!Screen_Update())
    {
        // Exit
        return FALSE;
    }

    Flush_Screen();

#if defined(__AMIGAOS4__) || defined(__AROS__) || defined(__MORPHOS__)
    SDL_Delay(delay_ms);
#else
    SDL_Delay(10);
#endif

    // Continue
    return TRUE;
}

// ------------------------------------------------------
// Redraw the screen quickly (this is used to update the status box)
// (We're already in 2D mode here in OpenGL mode)
void Redraw_Screen_Quick(void)
{
    Flush_Screen();
    Enter_2D_Mode((float) Cur_Width, (float) Cur_Height);
}

// ------------------------------------------------------
// Update the UI dimensions
void Reize_UI()
{
    glDrawBuffer(GL_FRONT);
    glViewport(0, 0, Cur_Width, Cur_Height);
    glDrawBuffer(GL_BACK);
    glViewport(0, 0, Cur_Width, Cur_Height);
    CONSOLE_WIDTH = Cur_Width;
    CHANNELS_WIDTH = Cur_Width - 20;
    TRACKS_WIDTH = Cur_Width - 20 - PAT_COL_NOTE;
    CONSOLE_HEIGHT = Cur_Height;
    CONSOLE_HEIGHT2 = Cur_Height;
    MAX_PATT_SCREEN_X = Cur_Width - 19;
    Set_Pattern_Size();
    restx = CONSOLE_WIDTH - 640;
    resty = CONSOLE_HEIGHT - 492;
    CONSOLE_HEIGHT2 = CONSOLE_HEIGHT - 42;
    fsize = 638 + restx;
    Visible_Columns = CONSOLE_WIDTH / 128;
}

// ------------------------------------------------------
// Swap window/fullscreen mode
void Switch_FullScreen()
{
    int Width;
    int Height;
    if (FullScreen)
    {
        if (FullScreen_Width < SCREEN_WIDTH) FullScreen_Width = SCREEN_WIDTH;
        if (FullScreen_Height < SCREEN_HEIGHT) FullScreen_Height = SCREEN_HEIGHT;
        Width = FullScreen_Width;
        Height = FullScreen_Height;
    }
    else
    {
        if (Windowed_Width < SCREEN_WIDTH) Windowed_Width = SCREEN_WIDTH;
        if (Windowed_Height < SCREEN_HEIGHT) Windowed_Height = SCREEN_HEIGHT;
        Width = Windowed_Width;
        Height = Windowed_Height;
    }
    Cur_Width = Width;
    Cur_Height = Height;
    Reize_UI();
    SDL_SetWindowSize(Main_Window, Width, Height);
    if (FullScreen)
    {
        SDL_SetWindowFullscreen(Main_Window, SDL_WINDOW_FULLSCREEN);
    }
    else
    {
        // Just in case we started in full screen mode
        SDL_SetWindowFullscreen(Main_Window, 0);
        Set_Window_Pos();
        SDL_SetWindowResizable(Main_Window, SDL_TRUE);
    }
    Env_Change = TRUE;
}

// ------------------------------------------------------
// Swap window/fullscreen mode
int Open_Window(int Width, int Height)
{
    Env_Change = TRUE;
    if (Width < SCREEN_WIDTH) Width = SCREEN_WIDTH;
    if (Height < SCREEN_HEIGHT) Height = SCREEN_HEIGHT;

    if (FullScreen)
    {
        if ((Main_Window = SDL_CreateWindow(Window_Title,
                                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            Startup_Width, Startup_Height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIDDEN)) == NULL)
        {
            return(FALSE);
        }
        Width = Startup_Width;
        Height = Startup_Height;
        if (!First_Time)
        {
            Save_Cur_Width = Cur_Width;
            Save_Cur_Height = Cur_Height;
        }
    }
    else
    {
        // Restore the old window size
        if (Save_Cur_Width != -1)
        {
            Width = Save_Cur_Width;
            Height = Save_Cur_Height;
        }
        if ((Main_Window = SDL_CreateWindow(Window_Title,
                                            Cur_Left, Cur_Top,
                                            Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN)) == NULL)
        {
            return(FALSE);
        }
        Save_Cur_Width = -1;
        Save_Cur_Height = -1;
        First_Time = FALSE;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, TRUE);
    if ((Main_Context = SDL_GL_CreateContext(Main_Window)) == NULL)
    {
        return(FALSE);
    }
    SDL_GL_SetSwapInterval(0);

    Set_Window_Pos();

    Cur_Width = Width;
    Cur_Height = Height;

    glDrawBuffer(GL_FRONT);
    glViewport(0, 0, Cur_Width, Cur_Height);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
    glShadeModel(GL_FLAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawBuffer(GL_BACK);
    glViewport(0, 0, Cur_Width, Cur_Height);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
    glShadeModel(GL_FLAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Reize_UI();

#if defined(__WIN32__)
    SDL_VERSION(&WMInfo.version);
    SDL_GetWindowWMInfo(Main_Window, &WMInfo);
    Main_Win32_Win = WMInfo.info.win.window;
    if(!FullScreen)
    {
        HICON hIcon;
        HICON hIconSmall;
        HINSTANCE ApphInstance = GetModuleHandle(0);
        hIcon = LoadIcon(ApphInstance, MAKEINTRESOURCE(IDI_ICON));
        hIconSmall = LoadIcon(ApphInstance, MAKEINTRESOURCE(IDI_ICONSMALL));
        // Set the icon of the window
        SendMessage(Main_Win32_Win, WM_SETICON, ICON_BIG, (LPARAM) hIcon);
        SendMessage(Main_Win32_Win, WM_SETICON, ICON_SMALL, (LPARAM) hIconSmall);
    }
#endif

    Get_Vid_Infos();

    return(TRUE);
}
