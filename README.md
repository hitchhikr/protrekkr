ProTrekkr is a tracker program combining a software synthesizer together with
a traditional samples tracker which can (mainly) be used to create electronic music
(like psytrance, trance goa, hard acid, IDM, chip, techno, jungle, etc.)
targeting small sized intros, demos or games.

It’s a heavily modified version of the old NoiseTrekker 2 which was made by
Juan Antonio Arguelles Rius aka Arguru.

Currently the tracker is available on Windows, Linux, FreeBSD, NetBSD, Amiga OS4, AROS, MorphOS, Mac OS (ppc, x86, ia64 & arm64) and Haiku OS.<br><br>
Standalone replay routines are available for most of these Operating Systems and also for the PlayStation Portable & PlayStation Vita.<br>

![alt text](ptk.png)
---
> [!CAUTION]
> **The tracker itself requires AT LEAST a 1,5ghz processor to run properly.**<br><br>
**This is a FREEWARE tool, this means that you can use it for whatever you want and for free.**
**There's NO GUARANTEE that this program will perform well or even run on your computer.**
---
In order to compile the tracker, use the provided makefile for your target architecture (if available).

Example:

make -f makefile.linux

The executable will be created in the release/distrib directory.

Notice that you'll need SDL 1 (only to compile the tracker, the replay routines themselves have no dependencies).<br>
> [!CAUTION]
> **When compiling for Linux or Mac OS: Use libsdl1.2 and NOT libsdl1.2-compat.**<br><br>
> **On Linux use libsdl1.2-dev and on Mac OS use libsdl from Mac Ports.**<br><br>
> **If your Linux distribution doesn't include SDL 1.2 then try your luck here:<br>https://github.com/libsdl-org/SDL-1.2**<br><br>
> **"ptk_vx.x.x_macosx_ia64_ogl.zip" may be problematic on some 64 bit versions of Mac OS (ulterior to Catalina), on those you should use "ptk_vx.x.x_macosx_ia64.zip" instead.**
---
So far, ProTrekkr was used in these intros or demos (the ones i'm aware of, at least) :<br><br>
<a href="http://www.pouet.net/prod.php?which=51129" target=_parent>Nebula by Sqny</a><br>
<a href="http://www.pouet.net/prod.php?which=51286" target=_parent>Kaivo by Clrsrc</a><br>
<a href="http://www.pouet.net/prod.php?which=52876" target=_parent>Kaolin beach2009 by Kaolin</a><br>
<a href="http://www.pouet.net/prod.php?which=53473" target=_parent>Genesis cracktro by Genesis & Nooon</a><br>
<a href="http://www.pouet.net/prod.php?which=53496" target=_parent>Arjttdoasw by Mayhem</a><br>
<a href="http://www.pouet.net/prod.php?which=53897" target=_parent>Ediacara by Traction</a><br>
<a href="http://www.pouet.net/prod.php?which=54430" target=_parent>Hatred crackmo.short by Hatred & Neural</a><br>
<a href="http://www.pouet.net/prod.php?which=54589" target=_parent>Primitive by Brainstorm</a><br>
<a href="http://www.pouet.net/prod.php?which=55302" target=_parent>Mozog by Mayhem</a><br>
<a href="http://www.pouet.net/prod.php?which=55505" target=_parent>Buenzli 19 invitation by clrsrc</a><br>
<br>Also some sampled tune from the tracker has been used in the end part of this Atari ST demo :<br><br>
<a href="http://www.pouet.net/prod.php?which=50486" target=_parent>Morphonic lab.VI by checkpoint</a><br>
<br>And a module done by BitArts (Norbert Röcher) with the tracker (in MP3 format): <a href="https://hitchhikr.net/bitarts%20-%20nightflight.mp3" target=_parent>NightFlight</a>
<br><br>Some more music also produced with the tracker available on SoundCloud :<br><br>
<a href="https://soundcloud.com/defraq79/distorted-reality" target=_parent>distorted reality</a><br>
<a href="https://soundcloud.com/defraq79/sign" target=_parent>sign</a><br>
<a href="https://soundcloud.com/defraq79/back-to-moon" target=_parent>back to moon</a><br>
<a href="https://soundcloud.com/defraq79/starburst" target=_parent>starburst</a><br>
<br>A video recording of another module done with the tracker: <a href="https://www.youtube.com/watch?v=N5Hr4uiPAZk" target=_parent>Crutokk by Vojta Nedved</a>
<br>Another video recording of yet another module done with the tracker: <a href="https://www.youtube.com/watch?v=XMvkCSP4eEs" target=_parent>Horse Heads and Gray Biscuits by Man With No Alias</a>