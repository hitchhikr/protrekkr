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

#include <pspkernel.h>
#include <pspctrl.h>
#include <psppower.h>

#include "../lib/include/ptkreplay.h"

PSP_MODULE_INFO("", PSP_MODULE_KERNEL, 1, 1);

SceCtrlData Ctrl_Buf;

extern "C"
{
    extern unsigned int _PTK_MODULE;
    void _start(void);
    int sceKernelPowerTick(int type);
}

// If the PSP isn't fast enough to render a module,
// this value won't save anything anyway.
#define LATENCY 180

void ptk_start(void)
{
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

void (*m)(void);
void _start(void)
{
    // Give us some boost
    scePowerSetClockFrequency(333, 333, 166);
    // Enable access to all the coprocessors and also obtain access to the media engine
    asm(
        "mfc0 $v0, $12\n"
        "or $v0, 0b11110000000000000000000000000000\n"
        "and $v0, 0b11111111111111111111111111100111\n"
        "mtc0 $v0, $12\n"
    );
    m = (void (*)()) ((u32) ptk_start | 0x80000000);
    m();
}
