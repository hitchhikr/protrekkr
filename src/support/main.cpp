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
#include "../include/ptk.h"

#if defined(__AROS__)
#include <cstdlib>
#define SDL_putenv putenv
#define SDL_strcasecmp strcasecmp
#elif defined(__MORPHOS__)
#include <cstdlib>
#define SDL_strcasecmp strcasecmp
#else
#define NEED_SDL_GETENV
#endif

#include <SDL/SDL_types.h>
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
#include <exec/libraries.h>
#include <proto/graphics.h>
#include <graphics/rastport.h>
struct Library *GfxBase;
struct GraphicsIFace *IGraphics;
#endif

const char *AMIGA_VERSION = "\0$VER: " TITLE " " VER_VER "." VER_REV "." VER_REVSMALL "\0";
#endif

// ------------------------------------------------------
// Types
typedef struct
{
    int code;
    int trans;
    int base;
} KEYCORE, LPKEYCORE;

// ------------------------------------------------------
// Variables
#if defined(__WIN32__)
HWND Main_Window;
#define SDL_NEED C_LINKAGE
#else
#define SDL_NEED
#endif

REQUESTER Title_Requester =
{
    "",
    NULL,
    &LOGOPIC, 5
};

const SDL_VideoInfo *Screen_Info;
int Orig_Screen_Width;
int Orig_Screen_Height;
int Startup_Width;
int Startup_Height;
int Resize_Width;
int Resize_Height;
int Burn_Title = FALSE;
int global_argc;
char global_argv[MAX_PATH];
int delay_ms = 0;

SDL_Surface *Main_Screen;

#if defined(__WIN32__)
SDL_SysWMinfo WMInfo;
#endif

MOUSE Mouse;
unsigned short Keys[SDLK_LAST];
unsigned short Keys_Sym[SDLK_LAST];
unsigned short Keys_Raw_Off[512];
unsigned short Keys_Raw_Repeat[512];
unsigned short Keys_Raw[512];
unsigned short Keys_Unicode[512];
int Keyboard_Nbr_Events;
int Keyboard_Events[256];
int Keyboard_Notes_Type[256];
int Keyboard_Notes_Bound[256];
int Key_Unicode;
char FullScreen = FALSE;
int Cur_Left = -1;
int Cur_Top = -1;
int Cur_Width = SCREEN_WIDTH;
int Cur_Height = SCREEN_HEIGHT;
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

extern int Nbr_Update_Rects;
extern SDL_Rect Update_Stack[UPDATE_STACK_SIZE];

char *ExePath;
extern char AutoReload;
extern char SplashScreen;
char Last_Used_Ptk[MAX_PATH];

SDL_Event Events[MAX_EVENTS];

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
KEYCORE Keyboard[] =
{
    {  0, 0x03, '2' },     // Do# / C#
    {  0, 0x04, '3' },     // Re# / D#
    {  0, 0x06, '5' },     // Fa# / F#
    {  0, 0x07, '6' },     // Sol# / G#
    {  0, 0x08, '7' },     // La# / A#
    {  0, 0x0a, '9' },     // Do# / C#
    {  0, 0x0b, '0' },     // Re# / D#
    {  0, 0x0d, '=' },     // Fa# / F#

    {  0, 0x10, 'q' },     // Do / C
    {  0, 0x11, 'w' },     // Re / D
    {  0, 0x12, 'e' },     // Mi / E
    {  0, 0x13, 'r' },     // Fa / F
    {  0, 0x14, 't' },     // Sol / G
    {  0, 0x15, 'y' },     // La / A
    {  0, 0x16, 'u' },     // Si / B
    {  0, 0x17, 'i' },     // Do / C
    {  0, 0x18, 'o' },     // Re / D
    {  0, 0x19, 'p' },     // Mi / E
    {  0, 0x1a, '[' },     // Fa / F
    {  0, 0x1b, ']' },     // Sol / G
    {  0, 0x1f, 's' },     // Do# / etc.
    {  0, 0x20, 'd' },     // Re#
    {  0, 0x22, 'g' },     // Fa#
    {  0, 0x23, 'h' },     // Sol#
    {  0, 0x24, 'j' },     // La#
    {  0, 0x26, 'l' },     // Do#
    {  0, 0x27, ';' },     // Re#

    {  0, 0x2c, 'z' },     // Do
    {  0, 0x2d, 'x' },     // Re
    {  0, 0x2e, 'c' },     // Mi
    {  0, 0x2f, 'v' },     // Fa
    {  0, 0x30, 'b' },     // Sol
    {  0, 0x31, 'n' },     // La
    {  0, 0x32, 'm' },     // Si
    {  0, 0x33, ',' },     // Do
    {  0, 0x34, '.' },     // Re
    {  0, 0x35, '/' },     // Mi
};

// English keyboard
KEYCORE Default_Keyboard[] =
{
    {  '2', 0x03, '2' },     // Do#
    {  '3', 0x04, '3' },     // Re#
    {  '5', 0x06, '5' },     // Fa#
    {  '6', 0x07, '6' },     // Sol#
    {  '8', 0x08, '7' },     // La#
    {  '9', 0x0a, '9' },     // Do#
    {  '0', 0x0b, '0' },     // Re#
    {  '-', 0x0d, '-' },     // Fa#

    {  'q', 0x10, 'q' },     // Do
    {  'w', 0x11, 'w' },     // Re
    {  'e', 0x12, 'e' },     // Mi
    {  'r', 0x13, 'r' },     // Fa
    {  't', 0x14, 't' },     // Sol
    {  'y', 0x15, 'y' },     // La
    {  'u', 0x16, 'u' },     // Si
    {  'i', 0x17, 'i' },     // Do
    {  'o', 0x18, 'o' },     // Re
    {  'p', 0x19, 'p' },     // Mi
    {  '[', 0x1a, '[' },     // Fa
    {  ']', 0x1b, ']' },     // Sol

    {  's', 0x1f, 's' },     // Do#
    {  'd', 0x20, 'd' },     // Re#
    {  'g', 0x22, 'g' },     // Fa#
    {  'h', 0x23, 'h' },     // Sol#
    {  'j', 0x24, 'j' },     // La#
    {  'l', 0x26, 'l' },     // Do#
    {  ';', 0x27, ';' },     // Re#

    {  'z', 0x2c, 'z' },     // Do
    {  'x', 0x2d, 'x' },     // Re
    {  'c', 0x2e, 'c' },     // Mi
    {  'v', 0x2f, 'v' },     // Fa
    {  'b', 0x30, 'b' },     // Sol
    {  'n', 0x31, 'n' },     // La
    {  'm', 0x32, 'm' },     // Si
    {  ',', 0x33, ',' },     // Do
    {  '.', 0x34, '.' },     // Re
    {  '/', 0x35, '/' },     // Mi
};

LPKEYCORE *Cur_Keyboard = Default_Keyboard;

// ------------------------------------------------------
// Functions
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

    for(i = 0; i < 37; i++)
    {
        // apparently, windows doesn't care about that localization
#if defined(__WIN32__)
        if(Cur_Keyboard[i].base == Key)
#else
        if(Cur_Keyboard[i].code == Key)
#endif
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
        while(!feof(KbFile) && j < 37)
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
// Main part of the tracker interface
#if defined(__WIN32__)
extern SDL_NEED int SDL_main(int argc, char *argv[])
#else
    #if defined(__LINUX__)
        int main(int argc, char *argv[])
    #else
        extern "C" int main(int argc, char *argv[])
    #endif
#endif

{
    SDL_KeyboardEvent *kb_evnt;
    char KbFileName[MAX_PATH];
    char KbFileNameToLoad[MAX_PATH];
    char KbData[64];
    char *KbDataEnd;
    int i;
    int j;
    int Key_Value;
    int Uni_Trans;
    FILE *KbFile;
    FILE *AllKbsFile;
    int in_note;
    char Win_Coords[64];
    Uint32 ExePath_Size = MAX_PATH;

#if defined(__AMIGAOS4__)
    GfxBase = IExec->OpenLibrary("graphics.library", 50);
    if(!GfxBase)
    {
        Message_Error("Can't open graphics.library.");
        exit(0);
    }
    IGraphics = (struct GraphicsIFace *) IExec->GetInterface(GfxBase, "main", 1, NULL);
    if(!IGraphics)
    {
        Message_Error("Can't obtain graphics.library interface.");
        exit(0);
    }
#endif

#if defined(__MACOSX_PPC__) || defined(__MACOSX_X86__)
    Uint32 Path_Length;
#endif

#if defined(__USE_OPENGL__)
    SDL_putenv("SDL_VIDEO_DRIVER=opengl");
#endif

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0)
    {
        Message_Error("Can't open SDL.");
        exit(0);
    }
    atexit(Destroy_Context);

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

#if defined(__USE_OPENGL__)
    strcat(Window_Title, " - (Using OpenGL)");
#endif

    SDL_WM_SetCaption(Window_Title, NULL);

    ExePath = (char *) malloc(ExePath_Size + 1);
    if(ExePath == NULL)
    {
        Message_Error("Can't open alloc memory.");
        exit(0);
    }
    memset(ExePath, 0, ExePath_Size + 1);

    Screen_Info = SDL_GetVideoInfo();
    Orig_Screen_Width = Screen_Info->current_w;
    Orig_Screen_Height = Screen_Info->current_h;
    Startup_Width = Screen_Info->current_w;
    Startup_Height = Screen_Info->current_h;
    if(Startup_Width < SCREEN_WIDTH) Startup_Width = SCREEN_WIDTH;
    if(Startup_Height < SCREEN_HEIGHT) Startup_Height = SCREEN_HEIGHT;

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
        while(Path_Length--)
        {
            if(ExePath[Path_Length] == '/')
            {
                ExePath[Path_Length] = 0;
                Path_Length++;
                break;
            }
        }

        // There's a probably a better way to do that but
        // it works fine and i want the app to be able 
        // to run from a console too.
        int Found_File = FALSE;
        strcat(ExePath, "/../");
        while(!Found_File)
        {
            CHDIR(ExePath);
            FILE *Test_File = fopen("skins/keyboards_macosx.txt", "rb");
            if(Test_File)
            {
                Found_File = TRUE;
                fclose(Test_File);
                ExePath[strlen(ExePath) - 1] = 0;
                break;
            }
            strcat(ExePath, "../");
        }
    #else
        GETCWD(ExePath, MAX_PATH);
    #endif
#endif

    // Default values
    sprintf(Keyboard_Name, "%s", "kben.txt");
    sprintf(Keyboard_FileNames[0], "%s", "kben.txt");
    sprintf(Keyboard_Labels[0], "%s", "English");
    Nbr_Keyboards = 1;
    Cur_Keyboard = Default_Keyboard;

    // Set the default palette before loading the config file
    Restore_Default_Palette(Default_Palette1, Default_Beveled1);
    Load_Config();

    if(!strlen(Keyboard_Name))
    {
        sprintf(Keyboard_Name, "%s", "kben.txt");
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
                    while(!feof(KbFile) && j < 37)
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
    if(!Switch_FullScreen(Cur_Width, Cur_Height, FALSE, FALSE))
    {
        Message_Error(SDL_GetError());
        exit(0);
    }

    if(FullScreen)
    {
        Save_Cur_Width = SCREEN_WIDTH;
        Save_Cur_Height = SCREEN_HEIGHT;
    }

    Ptk_Palette[0].r = Save_R;
    Ptk_Palette[0].g = Save_G;
    Ptk_Palette[0].b = Save_B;

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EnableUNICODE(TRUE);

#if !defined(__NO_MIDI__)
    // Load midi devices infos
    Midi_GetAll();
#endif

    SDL_GetMouseState((int *) &Mouse.x, (int *) &Mouse.y);

    if(!Init_Context())
    {
        exit(0);
    }

#if defined(__AMIGAOS4__) || defined(__AROS__) || defined(__MORPHOS__)
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

    Resize_Width = Cur_Width;
    Resize_Height = Cur_Height;
    do_resize = TRUE;

    while(1)
    {
        Mouse.wheel = 0;
        if(Mouse.button_oneshot & MOUSE_LEFT_BUTTON) Mouse.button_oneshot &= ~MOUSE_LEFT_BUTTON;
        if(Mouse.button_oneshot & MOUSE_MIDDLE_BUTTON) Mouse.button_oneshot &= ~MOUSE_MIDDLE_BUTTON;
        if(Mouse.button_oneshot & MOUSE_RIGHT_BUTTON) Mouse.button_oneshot &= ~MOUSE_RIGHT_BUTTON;

        memset(Keys, 0, sizeof(Keys));
        memset(Keys_Sym, 0, sizeof(Keys_Sym));
        memset(Keys_Unicode, 0, sizeof(Keys_Raw));

        SDL_PumpEvents();
        int Nbr_Events = SDL_PeepEvents(Events, MAX_EVENTS, SDL_GETEVENT, 
                                        SDL_EVENTMASK(SDL_KEYDOWN) | SDL_EVENTMASK(SDL_KEYUP) |
                                        SDL_EVENTMASK(SDL_MOUSEBUTTONUP) | SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN) |
                                        SDL_EVENTMASK(SDL_MOUSEMOTION) | SDL_EVENTMASK(SDL_QUIT) |
                                        SDL_EVENTMASK(SDL_VIDEORESIZE) | SDL_EVENTMASK(SDL_ACTIVEEVENT)
                                       );
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
                    memset(Keys_Sym, 0, sizeof(Keys_Sym));
                    memset(Keys_Unicode, 0, sizeof(Keys_Unicode));
                    memset(Keys_Raw, 0, sizeof(Keys_Raw));
                    memset(Keys_Raw_Off, 0, sizeof(Keys_Raw_Off));
                    memset(Keys_Raw_Repeat, 0, sizeof(Keys_Raw_Repeat));
                }
            }

            switch(Events[i].type)
            {
                case SDL_KEYDOWN:
                    Key_Unicode = Events[i].key.keysym.unicode;

                    Symbol = Events[i].key.keysym.sym;

                    Uni_Trans = Events[i].key.keysym.unicode;

                    if(Uni_Trans < 512)
                    {
                        // This is only used for the digits on all systems
                        // (especially on kb configs where they can only
                        //  be accessed by pressing shift).
                        // Otherwise it doesn't work under Mac OSX
                        Keys_Unicode[Uni_Trans] = TRUE;
                    }

#if !defined(__MACOSX_PPC__) && !defined(__MACOSX_X86__)
                    if(!Uni_Trans)
                    {
                        Uni_Trans = Symbol;
                    }
#else
                    Uni_Trans = Symbol;
#endif

                    Keys[Uni_Trans] = TRUE;

                    if(!In_Requester)
                    {
                        Scancode = Translate_Locale_Key(Symbol);

                        Keys_Raw[Scancode] = TRUE;
                        Keys_Raw_Off[Scancode] = FALSE;
                        Keys_Raw_Repeat[Scancode] = TRUE;

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

                        if((Keys[SDLK_RETURN] || Keys[SDLK_KP_ENTER]) && Enter_Notification == FALSE &&
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
                        if(Get_RShift())
                        {
                            RShift_Notification = TRUE;
                            RShift_Notified = TRUE;
                        }
                    }

                    Keys_Sym[Symbol] = TRUE;

                    if(key_on != 2) key_on = 1;

                    if(SDL_GetModState() & KMOD_LALT)
                    {
                        if(Keys[SDLK_RETURN] && !Get_LCtrl() && !Get_RCtrl())
                        {
                            FullScreen ^= TRUE;
                            Switch_FullScreen(Cur_Width, Cur_Height, TRUE, FullScreen ? FALSE : TRUE);
                        }
                    }
                    break;

                case SDL_KEYUP:
                    kb_evnt = (SDL_KeyboardEvent *) &Events[i];
                    if(kb_evnt->state == SDL_RELEASED)
                    {
                        key_on = 0;
                    }

                    if(!In_Requester)
                    {
                        // Only used for SDLK_KP_DIVIDE and SDLK_KP_MULTIPLY
                        Symbol = Events[i].key.keysym.sym;
                        Keys_Sym[Symbol] = FALSE;

                        Scancode = Translate_Locale_Key(Symbol);
                        
                        Keys_Raw_Off[Scancode] = TRUE;
                        Keys_Raw_Repeat[Scancode] = FALSE;

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
                        
                        if(Keys[SDLK_RETURN] || Keys[SDLK_KP_ENTER] || Get_LAlt() || Get_RAlt() || Get_LCtrl() || Get_RCtrl())
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
                            
                        if(Get_RShift())
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

                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    Mouse.x = Events[i].button.x;
                    Mouse.y = Events[i].button.y;

                    switch(Events[i].button.state)
                    {
                        case SDL_PRESSED:
                            switch(Events[i].button.button)
                            {
                                case SDL_MOUSE_LEFT_BUTTON:
                                    Mouse.button |= MOUSE_LEFT_BUTTON;
                                    Mouse.button_oneshot |= MOUSE_LEFT_BUTTON;
                                    break;

                                case SDL_MOUSE_MIDDLE_BUTTON:
                                    Mouse.button |= MOUSE_MIDDLE_BUTTON;
                                    Mouse.button_oneshot |= MOUSE_MIDDLE_BUTTON;
                                    break;

                                case SDL_MOUSE_RIGHT_BUTTON:
                                    Mouse.button |= MOUSE_RIGHT_BUTTON;
                                    Mouse.button_oneshot |= MOUSE_RIGHT_BUTTON;
                                    break;
                            }
                            if(Events[i].button.button == 4)
                            {
                                Mouse.wheel = 1;
                            }
                            if(Events[i].button.button == 5)
                            {
                                Mouse.wheel = -1;
                            }
                            break;

                        case SDL_RELEASED:
                            switch(Events[i].button.button)
                            {
                                case SDL_MOUSE_LEFT_BUTTON:
                                    Mouse.button &= ~MOUSE_LEFT_BUTTON;
                                    Mouse.button_oneshot &= ~MOUSE_LEFT_BUTTON;
                                    gui_pushed &= ~MOUSE_LEFT_BUTTON;
                                    break;
                                case SDL_MOUSE_MIDDLE_BUTTON:
                                    Mouse.button &= ~MOUSE_MIDDLE_BUTTON;
                                    Mouse.button_oneshot &= ~MOUSE_MIDDLE_BUTTON;
                                    gui_pushed &= ~MOUSE_MIDDLE_BUTTON;
                                    break;
                                case SDL_MOUSE_RIGHT_BUTTON:
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
                    break;

                case SDL_QUIT:
                    if(!In_Requester)
                    {
                        Display_Requester(&Exit_Requester, GUI_CMD_NOP, NULL, TRUE);
                    }
                    break;

                case SDL_GO_FULLSCREEN:
                    if(Events[i].user.code)
                    {
                        FullScreen = TRUE;
                    }
                    else
                    {
                        FullScreen = FALSE;
                    }
                    Switch_FullScreen(Cur_Width, Cur_Height, TRUE, FullScreen ? FALSE : TRUE);
                    break;

                case SDL_VIDEORESIZE:
                    // Nullify it

#ifndef __MORPHOS__
                    if(!FullScreen)
                    {
                        sprintf(Win_Coords, "SDL_VIDEO_WINDOW_POS=");
                        SDL_putenv(Win_Coords);
                    }
#endif
                    Resize_Width = Events[i].resize.w;
                    Resize_Height = Events[i].resize.h;
                    do_resize = TRUE;
                    break;

                case SDL_ACTIVEEVENT:
                    if(Events[i].active.gain)
                    {
                        memset(Keys, 0, sizeof(Keys));
                        memset(Keys_Sym, 0, sizeof(Keys_Sym));
                        SDL_Event event;
                        while(SDL_PollEvent(&event));
                    }
                    else
                    {
                        memset(Keys, 0, sizeof(Keys));
                        memset(Keys_Sym, 0, sizeof(Keys_Sym));
                    }
                    break;

                default:
                    break;
            }
        }

        if(do_resize)
        {
            Switch_FullScreen(Resize_Width, Resize_Height, TRUE, FALSE);
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
    exit(0);
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

#if !defined(__USE_OPENGL__)
    // Flush all pending blits
    if(Nbr_Update_Rects)
    {
       SDL_UpdateRects(Main_Screen, Nbr_Update_Rects, Update_Stack);
    }
    Nbr_Update_Rects = 0;
#endif

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

#if defined(__USE_OPENGL__)
    Leave_2d_Mode();

#if !defined(__WIN32__) && !defined(__AROS__)
    glDrawBuffer(GL_FRONT);
    glRasterPos2f(-1.0f, -1.0f);
    glCopyPixels(0, 0, Cur_Width, Cur_Height, GL_COLOR);
    glDrawBuffer(GL_BACK);
    glFinish();
#else
    SDL_GL_SwapBuffers();
#endif

#endif

}

// ------------------------------------------------------
// Redraw the screen
int Redraw_Screen(void)
{

#if defined(__USE_OPENGL__)
    Enter_2D_Mode(Cur_Width, Cur_Height);
#endif

    if(!Screen_Update())
    {
        return FALSE;
    }

    Flush_Screen();

#if defined(__AMIGAOS4__) || defined(__AROS__) || defined(__MORPHOS__)

#if defined(__AMIGAOS4__)
    IGraphics->WaitTOF();
#else
    SDL_Delay(delay_ms);
#endif

#else
    SDL_Delay(10);
#endif

    return TRUE;
}

// ------------------------------------------------------
// Redraw the screen quickly (this is used to update the status box)
// (We're already in 2D mode here in OpenGL mode)
void Redraw_Screen_Quick(void)
{
    Flush_Screen();

#if defined(__USE_OPENGL__)
    Enter_2D_Mode(Cur_Width, Cur_Height);
#endif

}

// ------------------------------------------------------
// Swap window/fullscreen mode
int Switch_FullScreen(int Width, int Height, int Refresh, int Force_Window_Mode)
{
    int Real_FullScreen = 0;

    if(!Force_Window_Mode)
    {
        if(Width >= Orig_Screen_Width)
        {
            Width = Orig_Screen_Width;
            Height = Orig_Screen_Height;
            FullScreen = TRUE;
        }
        else
        {
            if(Height >= Orig_Screen_Height)
            {
                Width = Orig_Screen_Width;
                Height = Orig_Screen_Height;
                FullScreen = TRUE;
            }
        }
    }

    Env_Change = TRUE;
    if(Width < SCREEN_WIDTH) Width = SCREEN_WIDTH;
    if(Height < SCREEN_HEIGHT) Height = SCREEN_HEIGHT;
    
#ifndef __MORPHOS__
    if(!FullScreen)
    {
        SDL_putenv("SDL_VIDEO_WINDOW_POS=center");
        SDL_putenv("SDL_VIDEO_CENTERED=1");
    }
#endif

#if defined(__LINUX__) || defined(__MACOSX_PPC__) || defined(__MACOSX_X86__) || defined(__WIN32__) || defined(__AROS__) || defined(__AMIGAOS4__)
    Real_FullScreen = SDL_FULLSCREEN;
#endif

#if defined(__USE_OPENGL__)

#if !defined(__LINUX__)
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);
#endif
    
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, TRUE);
    Destroy_Textures();
    if(Refresh)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
#endif

    if(FullScreen)
    {

#if defined(__USE_OPENGL__)
        if((Main_Screen = SDL_SetVideoMode(Startup_Width, Startup_Height,
                                           SCREEN_BPP, SDL_OPENGL | SDL_HWSURFACE | SDL_HWPALETTE  | SDL_NOFRAME | Real_FullScreen)) == NULL)
#else
        if((Main_Screen = SDL_SetVideoMode(Startup_Width, Startup_Height,
                                           SCREEN_BPP, SDL_SWSURFACE | SDL_HWPALETTE | SDL_NOFRAME | Real_FullScreen)) == NULL)
#endif

        {
            return(FALSE);
        }
        Width = Startup_Width;
        Height = Startup_Height;
        if(!First_Time)
        {
            Save_Cur_Width = Cur_Width;
            Save_Cur_Height = Cur_Height;
        }
    }
    else
    {
        if(Save_Cur_Width != -1)
        {
            Width = Save_Cur_Width;
            Height = Save_Cur_Height;
        }

#if defined(__USE_OPENGL__)
        if((Main_Screen = SDL_SetVideoMode(Width, Height,
                                           SCREEN_BPP, SDL_RESIZABLE | SDL_OPENGL | SDL_HWSURFACE | SDL_HWPALETTE)) == NULL)
#else
        if((Main_Screen = SDL_SetVideoMode(Width, Height,
                                           SCREEN_BPP, SDL_RESIZABLE | SDL_SWSURFACE | SDL_HWPALETTE)) == NULL)
#endif

        {
            return(FALSE);
        }
        Save_Cur_Width = -1;
        Save_Cur_Height = -1;
        First_Time = FALSE;
    }

    Cur_Width = Width;
    Cur_Height = Height;

#if defined(__USE_OPENGL__)
    glDrawBuffer(GL_FRONT);
    glViewport(0, 0, Cur_Width, Cur_Height);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
    glShadeModel(GL_FLAT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDrawBuffer(GL_BACK);
    glViewport(0, 0, Cur_Width, Cur_Height);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
    glShadeModel(GL_FLAT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#endif
    
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

    // Flush any pending rects
    Nbr_Update_Rects = 0;

#if defined(__WIN32__)
    SDL_GetWMInfo(&WMInfo);
    Main_Window = WMInfo.window;
    if(!FullScreen)
    {
        HICON hIcon;
        HICON hIconSmall;
        HINSTANCE ApphInstance = GetModuleHandle(0);
        hIcon = LoadIcon(ApphInstance, MAKEINTRESOURCE(IDI_ICON));
        hIconSmall = LoadIcon(ApphInstance, MAKEINTRESOURCE(IDI_ICONSMALL));
        // Set the icon of the window
        SendMessage(Main_Window, WM_SETICON, ICON_BIG, (LPARAM) hIcon);
        SendMessage(Main_Window, WM_SETICON, ICON_SMALL, (LPARAM) hIconSmall);
    }
#endif

    if(Refresh)
    {
        Set_Pictures_And_Palettes(FALSE);
    }

    return(TRUE);
}
