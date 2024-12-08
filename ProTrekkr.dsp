# Microsoft Developer Studio Project File - Name="ProTrekkr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ProTrekkr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProTrekkr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProTrekkr.mak" CFG="ProTrekkr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProTrekkr - Win32 release" (based on "Win32 (x86) Application")
!MESSAGE "ProTrekkr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ProTrekkr - Win32 release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "release"
# PROP Intermediate_Dir "release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "__WIN32__" /D "NDEBUG" /D "_WINDOWS" /D "__WINDOWS_MM__" /D "__MOT_SWAP__" /D "__MP3_CODEC__" /D "__AT3_CODEC__" /D "__GSM_CODEC__" /D "__ADPCM_CODEC__" /D "__TRUESPEECH_CODEC__" /Fr /FD /Gs /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib sdl.lib dsound.lib msacm32.lib zlib.lib winmm.lib advapi32.lib sdl_draw.lib shlwapi.lib sdlmain.lib msvcrt.lib libcp.lib opengl32.lib /nologo /subsystem:windows /pdb:none /machine:I386 /nodefaultlib /out:"release/distrib/ptk_win.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "ProTrekkr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /ML /W3 /Gm /GX /ZI /Od /D "__WIN32__" /D "_DEBUG" /D "_WINDOWS-" /D "__WINDOWS_MM__" /D "__MOT_SWAP__" /D "__MP3_CODEC__" /D "__AT3_CODEC__" /D "__GSM_CODEC__" /D "__ADPCM_CODEC__" /D "__TRUESPEECH_CODEC__" /FR /FD /Zm500 /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib sdl.lib sdl_draw.lib dsound.lib msacm32.lib winmm.lib advapi32.lib zlib.lib shlwapi.lib sdlmain.lib msvcrt.lib libcp.lib opengl32.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /nodefaultlib /out:"release/distrib/ptk_win.exe"

!ENDIF 

# Begin Target

# Name "ProTrekkr - Win32 release"
# Name "ProTrekkr - Win32 Debug"
# Begin Group "startup"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "startup include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\support\include\main.h
# End Source File
# Begin Source File

SOURCE=.\src\support\include\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\support\include\timer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\support\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\support\project.ico
# End Source File
# Begin Source File

SOURCE=.\src\support\project_16x16.ico
# End Source File
# Begin Source File

SOURCE=.\src\support\resources.rc
# End Source File
# Begin Source File

SOURCE=.\src\support\timer.cpp
# End Source File
# End Group
# Begin Group "ProTrekkr"

# PROP Default_Filter ""
# Begin Group "ProTrekkr include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\include\ptk.h
# End Source File
# Begin Source File

SOURCE=.\src\include\variables.h
# End Source File
# Begin Source File

SOURCE=.\src\include\version.h
# End Source File
# End Group
# Begin Group "editors"

# PROP Default_Filter ""
# Begin Group "editors include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\editors\include\editor_303.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_diskio.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_fx_setup.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_instrument.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_midi.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_pattern.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_reverb.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_sample.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_sequencer.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_setup.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_synth.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_track.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_track_fx.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\patterns_blocks.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\editors\editor_303.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_diskio.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_fx_setup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_instrument.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_midi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_reverb.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_sample.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_sequencer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_setup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_synth.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_track.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_track_fx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\patterns_blocks.cpp
# End Source File
# End Group
# Begin Group "replay"

# PROP Default_Filter ""
# Begin Group "replay include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\cubic_spline.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\endianness.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\ptk_def_properties.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\ptkreplay.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\replay.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\samples_unpack.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\spline.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\synth.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\tb_303.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\wavpack.h
# End Source File
# End Group
# Begin Group "sounddriver"

# PROP Default_Filter ""
# Begin Group "sounddriver include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\release\distrib\replay\lib\sounddriver\include\sounddriver_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\release\distrib\replay\lib\sounddriver\sounddriver_windows.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\release\distrib\replay\lib\endianness.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\replay.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\samples_unpack.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\spline.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\synth.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\tb_303.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\wavpack.cpp
# End Source File
# End Group
# Begin Group "ui"

# PROP Default_Filter ""
# Begin Group "ui include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ui\include\draw_primitives.h
# End Source File
# Begin Source File

SOURCE=.\src\UI\include\misc_draw.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\include\requesters.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\ui\draw_primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\misc_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\requesters.cpp
# End Source File
# End Group
# Begin Group "files"

# PROP Default_Filter ""
# Begin Group "files include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\files\include\303s.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\config.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\files.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\files_list.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\insts.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\mods.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\patterns.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\ptps.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\reverbs.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\synths.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\files\303s.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\config.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\files.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\files_list.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\insts.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\midi_cfg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\mods.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\patterns.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\ptps.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\reverbs.cpp
# End Source File
# Begin Source File

SOURCE=.\src\files\synths.cpp
# End Source File
# End Group
# Begin Group "midi"

# PROP Default_Filter ""
# Begin Group "midi include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\midi\include\midi.h
# End Source File
# Begin Source File

SOURCE=.\src\midi\include\midi_dispatch.h
# End Source File
# Begin Source File

SOURCE=.\src\midi\include\rterror.h
# End Source File
# Begin Source File

SOURCE=.\src\midi\include\rtmidi.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\midi\midi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\midi\midi_dispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\src\midi\rtmidi.cpp
# End Source File
# End Group
# Begin Group "samples"

# PROP Default_Filter ""
# Begin Group "aiff"

# PROP Default_Filter ""
# Begin Group "aiff include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\samples\aiff\include\aiff.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\samples\aiff\aiff.cpp
# End Source File
# End Group
# Begin Group "wav"

# PROP Default_Filter ""
# Begin Group "wav include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\samples\wav\include\ddc.h
# End Source File
# Begin Source File

SOURCE=.\src\samples\wav\include\riff.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\samples\wav\riff.cpp
# End Source File
# End Group
# Begin Group "samples include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\samples\include\samples_ops.h
# End Source File
# Begin Source File

SOURCE=.\src\samples\include\samples_pack.h
# End Source File
# End Group
# Begin Group "wavpack"

# PROP Default_Filter ""
# Begin Group "wavpack include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\samples\wavpack\include\wplocal.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\samples\wavpack\bits.cpp
# End Source File
# Begin Source File

SOURCE=.\src\samples\wavpack\pack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\samples\wavpack\tinypack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\samples\wavpack\words.cpp
# End Source File
# Begin Source File

SOURCE=.\src\samples\wavpack\wputils.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\samples\samples_ops.cpp
# End Source File
# Begin Source File

SOURCE=.\src\samples\samples_pack.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\release\distrib\changes.txt
# End Source File
# Begin Source File

SOURCE=.\release\distrib\license.txt
# End Source File
# Begin Source File

SOURCE=.\src\ptk.cpp
# End Source File
# End Group
# End Target
# End Project
