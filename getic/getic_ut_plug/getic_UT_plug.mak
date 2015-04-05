# Microsoft Developer Studio Generated NMAKE File, Based on getic_UT_plug.dsp
!IF "$(CFG)" == ""
CFG=getic_ut_plug - Win32 Debug
!MESSAGE No configuration specified. Defaulting to getic_ut_plug - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "getic_ut_plug - Win32 Release" && "$(CFG)" != "getic_ut_plug - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "getic_UT_plug.mak" CFG="getic_ut_plug - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "getic_ut_plug - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "getic_ut_plug - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "getic_ut_plug - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\_bin\ged_plugins\getic_ut_plug.dll"


CLEAN :
	-@erase "$(INTDIR)\getic_ut_plug.obj"
	-@erase "$(INTDIR)\getic_UT_plug.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\UTLoader.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\getic_ut_plug.exp"
	-@erase "$(OUTDIR)\getic_ut_plug.lib"
	-@erase "..\_bin\ged_plugins\getic_ut_plug.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /Od /I "..\baselib" /I "..\_include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETIC_MS3D_PLUG_EXPORTS" /D "_CFG_WIN32_WIN" /Fp"$(INTDIR)\getic_UT_plug.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\getic_UT_plug.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\getic_ut_plug.pdb" /machine:I386 /out:"..\_bin\ged_plugins\getic_ut_plug.dll" /implib:"$(OUTDIR)\getic_ut_plug.lib" 
LINK32_OBJS= \
	"$(INTDIR)\getic_ut_plug.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UTLoader.obj"

"..\_bin\ged_plugins\getic_ut_plug.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "getic_ut_plug - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\_bin\ged_plugins\getic_ut_plug.dll" "$(OUTDIR)\getic_UT_plug.bsc"


CLEAN :
	-@erase "$(INTDIR)\getic_ut_plug.obj"
	-@erase "$(INTDIR)\getic_UT_plug.pch"
	-@erase "$(INTDIR)\getic_ut_plug.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\UTLoader.obj"
	-@erase "$(INTDIR)\UTLoader.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\getic_UT_plug.bsc"
	-@erase "$(OUTDIR)\getic_ut_plug.exp"
	-@erase "$(OUTDIR)\getic_ut_plug.lib"
	-@erase "$(OUTDIR)\getic_ut_plug.pdb"
	-@erase "..\_bin\ged_plugins\getic_ut_plug.dll"
	-@erase "..\_bin\ged_plugins\getic_ut_plug.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\baselib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETIC_MS3D_PLUG_EXPORTS" /D "_CFG_WIN32_WIN" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\getic_UT_plug.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\getic_UT_plug.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\getic_ut_plug.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UTLoader.sbr"

"$(OUTDIR)\getic_UT_plug.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\getic_ut_plug.pdb" /debug /machine:I386 /out:"..\_bin\ged_plugins\getic_ut_plug.dll" /implib:"$(OUTDIR)\getic_ut_plug.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\getic_ut_plug.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UTLoader.obj"

"..\_bin\ged_plugins\getic_ut_plug.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("getic_UT_plug.dep")
!INCLUDE "getic_UT_plug.dep"
!ELSE 
!MESSAGE Warning: cannot find "getic_UT_plug.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "getic_ut_plug - Win32 Release" || "$(CFG)" == "getic_ut_plug - Win32 Debug"
SOURCE=.\getic_ut_plug.cpp

!IF  "$(CFG)" == "getic_ut_plug - Win32 Release"


"$(INTDIR)\getic_ut_plug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\getic_UT_plug.pch"


!ELSEIF  "$(CFG)" == "getic_ut_plug - Win32 Debug"


"$(INTDIR)\getic_ut_plug.obj"	"$(INTDIR)\getic_ut_plug.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\getic_UT_plug.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "getic_ut_plug - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Od /I "..\baselib" /I "..\_include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETIC_MS3D_PLUG_EXPORTS" /D "_CFG_WIN32_WIN" /Fp"$(INTDIR)\getic_UT_plug.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\getic_UT_plug.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "getic_ut_plug - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\baselib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETIC_MS3D_PLUG_EXPORTS" /D "_CFG_WIN32_WIN" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\getic_UT_plug.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\getic_UT_plug.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\UTLoader.cpp

!IF  "$(CFG)" == "getic_ut_plug - Win32 Release"


"$(INTDIR)\UTLoader.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\getic_UT_plug.pch"


!ELSEIF  "$(CFG)" == "getic_ut_plug - Win32 Debug"


"$(INTDIR)\UTLoader.obj"	"$(INTDIR)\UTLoader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\getic_UT_plug.pch"


!ENDIF 


!ENDIF 

