# Microsoft Developer Studio Project File - Name="baselib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=baselib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "baselib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "baselib.mak" CFG="baselib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "baselib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "baselib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "baselib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Od /Ob1 /I "..\_include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_CFG_WIN32_WIN" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\_lib\baselib6.lib"

!ELSEIF  "$(CFG)" == "baselib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\_include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_CFG_WIN32_WIN" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\_lib\baselib6d.lib"

!ENDIF 

# Begin Target

# Name "baselib - Win32 Release"
# Name "baselib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "OS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\WIN_WIN\InetFtpHttp.cpp
# End Source File
# Begin Source File

SOURCE=..\WIN_WIN\InetFtpHttp.h
# End Source File
# Begin Source File

SOURCE=..\WIN_WIN\os.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\baselib.cpp
# End Source File
# Begin Source File

SOURCE=..\BzipCompressor.cpp
# End Source File
# Begin Source File

SOURCE=..\sock.cpp
# End Source File
# Begin Source File

SOURCE=..\XUnzip.cpp
# End Source File
# Begin Source File

SOURCE=..\XZip.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\basecont.h
# End Source File
# Begin Source File

SOURCE=..\baselib.h
# End Source File
# Begin Source File

SOURCE=..\basethread.h
# End Source File
# Begin Source File

SOURCE=..\baseutils.h
# End Source File
# Begin Source File

SOURCE=..\BzipCompressor.h
# End Source File
# Begin Source File

SOURCE=..\sock.h
# End Source File
# Begin Source File

SOURCE=..\XUnzip.h
# End Source File
# Begin Source File

SOURCE=..\XZip.h
# End Source File
# End Target
# End Project
