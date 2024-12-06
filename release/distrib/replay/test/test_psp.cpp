/*
   How to use:

   1. Copy the .ptp and the Ptk_Properties.h file into the "Replay" directory.
   2. Edit the module.asm file for visual C or module.s for gcc
      and replace the ../YOUR_MODULE.PTP by the filename of your module file.
   3. Compile the replay routine (which will be fine tuned for your module).
   4. Compile this test example.

   Note: LATENCY should be increased if the cpu is extensively used for other purposes
         to avoid lags and stuttering.
*/

#include <pspkernel.h>
#include <pspctrl.h>
#include <psppower.h>

#include "../lib/include/ptkreplay.h"

PSP_MODULE_INFO("H", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define STACK_SIZE (1024 * 4)

SceCtrlData Ctrl_Buf;

#ifdef __cplusplus
extern "C"
#endif
{
    extern unsigned int _PTK_MODULE;
    int sceKernelPowerTick(int type);
#ifdef __cplusplus
}
#endif

// If the PSP isn't fast enough to render a module,
// this value won't save anything anyway.
#define LATENCY 120

int main_thread(SceSize args, void *argp)
{
    scePowerSetClockFrequency(333, 333, 166);

    if(Ptk_InitDriver(LATENCY))
    {
        // Load it
        if(Ptk_InitModule((unsigned char *) &_PTK_MODULE, 0))
        {
            // Start playing it
            Ptk_Play();

            // Quit with the home button
	        while(1)
            {
                sceCtrlPeekBufferPositive(&Ctrl_Buf, 1);
                sceKernelDcacheWritebackAll();
                if(Ctrl_Buf.Buttons & PSP_CTRL_HOME) break;
  
                sceKernelPowerTick(6);
            }
            Ptk_Stop();
        }
        Ptk_ReleaseDriver();
    }
    sceKernelExitGame();
}

// ---------------------------------------------------------------
// Program entry point
extern "C" int module_start(SceSize args, void *argp)
{
    sceKernelStartThread(sceKernelCreateThread("Ptk", (int (*)(SceSize, void*)) (((unsigned int) main_thread) & 0x7fffffff), 10, STACK_SIZE, PSP_THREAD_ATTR_VFPU, 0), 0, 0);
	return 0;
}
