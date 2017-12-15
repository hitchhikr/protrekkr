# Microsoft Developer Studio Project File - Name="plugin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=plugin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plugin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plugin.mak" CFG="plugin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "plugin - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "plugin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "plugin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "__WIN32__" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /D "__WINAMP__" /D "__STAND_ALONE__" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib ../../zlib.lib msvcrt.lib msacm32.lib dsound.lib /nologo /dll /machine:I386 /out:"Distrib\in_ptk.dll"

!ELSEIF  "$(CFG)" == "plugin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "__WIN32__" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_EXPORTS" /D "__WINAMP__" /D "__STAND_ALONE__" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib ../../zlib.lib msvcrt.lib msacm32.lib dsound.lib /nologo /dll /debug /machine:I386 /nodefaultlib /out:"C:\Program Files\XM Play\in_ptk.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "plugin - Win32 Release"
# Name "plugin - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\files\303s.cpp
# End Source File
# Begin Source File

SOURCE=..\files\files.cpp
# End Source File
# Begin Source File

SOURCE=.\MAIN.Cpp
# End Source File
# Begin Source File

SOURCE=..\editors\patterns_blocks.cpp
# End Source File
# Begin Source File

SOURCE=..\files\reverbs.cpp
# End Source File
# Begin Source File

SOURCE=..\files\synths.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\in2.h
# End Source File
# Begin Source File

SOURCE=.\out.h
# End Source File
# End Group
# Begin Group "Replay"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\cubic_spline.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\ptk_def_properties.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\ptkreplay.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\replay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\replay.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\samples_unpack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\samples_unpack.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\sounddriver\sounddriver_windows.cpp
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\sounddriver\include\sounddriver_windows.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\spline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\spline.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\synth.cpp
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\synth.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\tb_303.cpp
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\tb_303.h
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\wavpack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\release\distrib\replay\lib\include\wavpack.h
# End Source File
# End Group
# End Target
# End Project
