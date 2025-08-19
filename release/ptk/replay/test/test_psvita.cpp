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

#include <psp2/kernel/threadmgr.h>
#include "../lib/include/ptkreplay.h"

#ifdef __cplusplus
extern "C"
#endif
{
    extern unsigned int _PTK_MODULE;
#ifdef __cplusplus
}
#endif

// If the VITA isn't fast enough to render a module,
// this value won't save anything anyway.
#define LATENCY 40

extern "C" void _start() __attribute__ ((weak, alias("module_start")));
extern "C" int module_start(SceSize args, void *argp)
{
    if(Ptk_InitDriver(LATENCY))
    {
        // Load it
        if(Ptk_InitModule((unsigned char *) &_PTK_MODULE, 0))
        {
            // Start playing it
            Ptk_Play();
	        while(1)
            {
                sceKernelDelayThread(10);
            }
            Ptk_Stop();
        }
        Ptk_ReleaseDriver();
    }
    return 0;
}
