/*
   How to use:

   1. Copy the .ptp and the Ptk_Properties.h file into the "Replay" directory.
   2. Edit the module.asm file for visual C or module.s for MingW
      and replace the ../YOUR_MODULE.PTP by the filename of your module file.
   3. Compile the replay routine (which will be fine tuned for your module).
   4. Compile this test example.

   Note: LATENCY should be increased if the cpu is extensively used for other purposes
         to avoid lags and stuttering.
*/

#include <libraries/dos.h>
#include <proto/exec.h>

#include "../lib/include/ptkreplay.h"

#ifdef __cplusplus
extern "C"
#endif
{
    extern unsigned int _PTK_MODULE;
#ifdef __cplusplus
}
#endif

// Initialize with 20 milliseconds of latency
#define LATENCY 20

int main(void)
{
    if(!Ptk_InitDriver(LATENCY))
    {
        Ptk_ReleaseDriver();
        return(0);
    }
    // Load it
    if(!Ptk_InitModule((unsigned char *) &_PTK_MODULE, 0))
    {
        Ptk_ReleaseDriver();
        return(0);
    }
    // Start playing it
    Ptk_Play();

    // Ctrl+C to quit
    while(1)
    {
        if(Wait(SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
        {
            break;
        }
    }

    Ptk_Stop();
    Ptk_ReleaseDriver();
    return(0);
}
