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
!MESSAGE "ProTrekkr - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ProTrekkr - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ProTrekkr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
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
# ADD LINK32 kernel32.lib user32.lib sdl.lib dsound.lib msacm32.lib zlib.lib winmm.lib advapi32.lib sdl_draw.lib shlwapi.lib tinyxml.lib sdlmain.lib msvcrt.lib libcp.lib /nologo /subsystem:windows /pdb:none /machine:I386 /nodefaultlib /out:"release/distrib/ptk_win.exe"
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
# ADD CPP /nologo /ML /W3 /Gm /GX /ZI /Od /D "__WIN32__" /D "_DEBUG" /D "_WINDOWS" /D "__WINDOWS_MM__" /D "__MOT_SWAP__" /D "__MP3_CODEC__" /D "__AT3_CODEC__" /D "__GSM_CODEC__" /D "__ADPCM_CODEC__" /D "__TRUESPEECH_CODEC__" /FR /FD /Zm500 /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib sdl.lib sdl_draw.lib dsound.lib msacm32.lib winmm.lib advapi32.lib zlib.lib shlwapi.lib tinyxml.lib sdlmain.lib msvcrt.lib libcp.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /nodefaultlib /out:"release/distrib/ptk_win.exe"

!ENDIF 

# Begin Target

# Name "ProTrekkr - Win32 Release"
# Name "ProTrekkr - Win32 Debug"
# Begin Group "SDL"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "SDL Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\support\include\main.h
# End Source File
# Begin Source File

SOURCE=.\src\Support\include\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\Support\include\timer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\support\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Support\Project.ico
# End Source File
# Begin Source File

SOURCE=.\src\support\Project_16x16.ico
# End Source File
# Begin Source File

SOURCE=.\src\Support\Resources.rc
# End Source File
# Begin Source File

SOURCE=.\src\Support\timer.cpp
# End Source File
# End Group
# Begin Group "ProTrekkr"

# PROP Default_Filter ""
# Begin Group "ProTrekkr Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\include\ptk.h
# End Source File
# Begin Source File

SOURCE=.\src\include\Variables.h
# End Source File
# Begin Source File

SOURCE=.\src\include\version.h
# End Source File
# End Group
# Begin Group "Editors"

# PROP Default_Filter ""
# Begin Group "Editors Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\Editors\include\Editor_303.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_DiskIO.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Fx_Setup.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Instrument.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_midi.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Pattern.h
# End Source File
# Begin Source File

SOURCE=.\src\editors\include\editor_reverb.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Sample.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Sequencer.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Setup.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Synth.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Track.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Editor_Track_Fx.h
# End Source File
# Begin Source File

SOURCE=.\src\Editors\include\Patterns_Blocks.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\Editors\Editor_303.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_DiskIO.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Fx_Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Instrument.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_midi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\src\editors\editor_reverb.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Sample.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Sequencer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Synth.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Track.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Editor_Track_Fx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editors\Patterns_Blocks.cpp
# End Source File
# End Group
# Begin Group "Replay"

# PROP Default_Filter ""
# Begin Group "Replay Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\include\Cubic_Spline.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\endianness.h
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\include\Ptk_Def_Properties.h
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\include\PtkReplay.h
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\include\Replay.h
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\include\Samples_Unpack.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\spline.h
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\include\Synth.h
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\include\TB_303.h
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\include\wavpack.h
# End Source File
# End Group
# Begin Group "SoundDriver"

# PROP Default_Filter ""
# Begin Group "SoundDriver Include"

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

SOURCE=.\Release\Distrib\Replay\lib\Replay.cpp
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\Samples_Unpack.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\spline.cpp
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\Synth.cpp
# End Source File
# Begin Source File

SOURCE=.\Release\Distrib\Replay\lib\TB_303.cpp
# End Source File
# Begin Source File

SOURCE=.\release\distrib\replay\lib\wavpack.cpp
# End Source File
# End Group
# Begin Group "UI"

# PROP Default_Filter ""
# Begin Group "UI Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ui\include\draw_primitives.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\include\gadgets.h
# End Source File
# Begin Source File

SOURCE=.\src\UI\include\Misc_Draw.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\include\requesters.h
# End Source File
# Begin Source File

SOURCE=.\src\UI\include\xml.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\ui\draw_primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\gadgets.cpp
# End Source File
# Begin Source File

SOURCE=.\src\UI\Misc_Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\requesters.cpp
# End Source File
# Begin Source File

SOURCE=.\src\UI\xml.cpp
# End Source File
# End Group
# Begin Group "Files"

# PROP Default_Filter ""
# Begin Group "Files Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\files\include\303s.h
# End Source File
# Begin Source File

SOURCE=.\src\files\include\config.h
# End Source File
# Begin Source File

SOURCE=.\src\Files\include\Files.h
# End Source File
# Begin Source File

SOURCE=.\src\Files\include\Files_List.h
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

SOURCE=.\src\Files\Files.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Files\Files_List.cpp
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
# Begin Group "Midi"

# PROP Default_Filter ""
# Begin Group "Midi Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\midi\include\midi.h
# End Source File
# Begin Source File

SOURCE=.\src\midi\include\midi_dispatch.h
# End Source File
# Begin Source File

SOURCE=.\src\midi\include\RtError.h
# End Source File
# Begin Source File

SOURCE=.\src\midi\include\RtMidi.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\midi\midi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\midi\midi_dispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\src\midi\RtMidi.cpp
# End Source File
# End Group
# Begin Group "Samples"

# PROP Default_Filter ""
# Begin Group "Aiff"

# PROP Default_Filter ""
# Begin Group "Aiff Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\samples\aiff\include\aiff.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\samples\aiff\aiff.cpp
# End Source File
# End Group
# Begin Group "Wav"

# PROP Default_Filter ""
# Begin Group "Wav Include"

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
# Begin Group "Samples Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\samples\include\samples_ops.h
# End Source File
# Begin Source File

SOURCE=.\src\samples\include\samples_pack.h
# End Source File
# End Group
# Begin Group "Wavpack"

# PROP Default_Filter ""
# Begin Group "Wavpack Include"

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

SOURCE=.\release\distrib\history.txt
# End Source File
# Begin Source File

SOURCE=.\src\ptk.cpp
# End Source File
# End Group
# End Target
# End Project
