# Microsoft Developer Studio Project File - Name="ptkreplay" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ptkreplay - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ptkreplay.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ptkreplay.mak" CFG="ptkreplay - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ptkreplay - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ptkreplay - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ptkreplay - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Gz /W3 /O1 /D "__WIN32__" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__STAND_ALONE__" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\ptkreplay.lib"

!ELSEIF  "$(CFG)" == "ptkreplay - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_LIB" /D "__WIN32__" /D "_DEBUG" /D "_MBCS" /D "__STAND_ALONE__" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\ptkreplay.lib"

!ENDIF 

# Begin Target

# Name "ptkreplay - Win32 Release"
# Name "ptkreplay - Win32 Debug"
# Begin Group "source files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\replay.cpp
# End Source File
# Begin Source File

SOURCE=.\samples_unpack.cpp
# End Source File
# Begin Source File

SOURCE=.\sounddriver\sounddriver_windows.cpp
# End Source File
# Begin Source File

SOURCE=.\spline.cpp
# End Source File
# Begin Source File

SOURCE=.\synth.cpp
# End Source File
# Begin Source File

SOURCE=.\tb_303.cpp
# End Source File
# Begin Source File

SOURCE=.\wavpack.cpp
# End Source File
# End Group
# Begin Group "header files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\cubic_spline.h
# End Source File
# Begin Source File

SOURCE=..\ptk_properties.h
# End Source File
# Begin Source File

SOURCE=.\include\replay.h
# End Source File
# Begin Source File

SOURCE=.\include\samples_unpack.h
# End Source File
# Begin Source File

SOURCE=.\sounddriver\include\sounddriver_windows.h
# End Source File
# Begin Source File

SOURCE=.\include\spline.h
# End Source File
# Begin Source File

SOURCE=.\include\synth.h
# End Source File
# Begin Source File

SOURCE=.\include\tb_303.h
# End Source File
# Begin Source File

SOURCE=.\include\wavpack.h
# End Source File
# End Group
# End Target
# End Project
