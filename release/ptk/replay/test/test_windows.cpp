/*
   How to use:

   1. Copy the .ptp and the Ptk_Properties.h file into the "Replay" directory.
   2. Edit the module.asm file for visual C or module.s for GCC
      and replace the ../YOUR_MODULE.PTP by the filename of your module file.
   3. Compile the replay routine (which will be fine tuned for your module).
   4. Compile this test example.

   Note: LATENCY should be increased if the cpu is extensively used for other purposes
         to avoid lags and stuttering.
*/

#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <stdio.h>

#include "../lib/include/ptkreplay.h"

#if defined(__GCC__)
#define MODULE PTK_MODULE
#else
#define MODULE _PTK_MODULE
#endif

#ifdef __cplusplus
extern "C"
#endif
{
    extern unsigned int MODULE;
#ifdef __cplusplus
}
#endif

// Initialize with 20 milliseconds of latency
#define LATENCY 20

int main(void)
{
    // Init the sound driver and the various tables
    if(!Ptk_InitDriver(GetConsoleWindow(), LATENCY)) return(0);

    // Load it
    if(!Ptk_InitModule((unsigned char *) &MODULE, 0))
    {
        Ptk_ReleaseDriver();
        return(0);
    }

    // Start playing it
    Ptk_Play();

    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        printf("   :   ");
        printf("\r");
        printf("%.2d:%.2d", Ptk_GetPosition(), Ptk_GetRow());
        printf("\r");

        Sleep(10);
    }

    Ptk_Stop();
    Ptk_ReleaseDriver();

    ExitProcess(0);
}
