# Microsoft Developer Studio Generated NMAKE File, Based on geticgui.dsp
!IF "$(CFG)" == ""
CFG=geticgui - Win32 Debug
!MESSAGE No configuration specified. Defaulting to geticgui - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "geticgui - Win32 Release" && "$(CFG)" != "geticgui - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "geticgui.mak" CFG="geticgui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "geticgui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "geticgui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "geticgui - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\_bin\geticgui.dll"


CLEAN :
	-@erase "$(INTDIR)\geticgui.obj"
	-@erase "$(INTDIR)\geticgui.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\geticgui.exp"
	-@erase "$(OUTDIR)\geticgui.lib"
	-@erase "..\_bin\geticgui.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /Od /I "..\baselib" /D "_WINDOWS" /D "_USRDLL" /D "GETICGUI_EXPORTS" /D "NDEBUG" /D "WIN32" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\geticgui.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\geticgui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\geticgui.pdb" /machine:I386 /out:"..\_bin\geticgui.dll" /implib:"$(OUTDIR)\geticgui.lib" 
LINK32_OBJS= \
	"$(INTDIR)\geticgui.obj" \
	"$(INTDIR)\geticgui.res"

"..\_bin\geticgui.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "geticgui - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\_bin\geticgui.dll"


CLEAN :
	-@erase "$(INTDIR)\geticgui.obj"
	-@erase "$(INTDIR)\geticgui.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\geticgui.exp"
	-@erase "$(OUTDIR)\geticgui.lib"
	-@erase "$(OUTDIR)\geticgui.pdb"
	-@erase "..\_bin\geticgui.dll"
	-@erase "..\_bin\geticgui.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\_lib" /I "..\baselib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETICGUI_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\geticgui.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\geticgui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\geticgui.pdb" /debug /machine:I386 /out:"..\_bin\geticgui.dll" /implib:"$(OUTDIR)\geticgui.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\geticgui.obj" \
	"$(INTDIR)\geticgui.res"

"..\_bin\geticgui.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("geticgui.dep")
!INCLUDE "geticgui.dep"
!ELSE 
!MESSAGE Warning: cannot find "geticgui.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "geticgui - Win32 Release" || "$(CFG)" == "geticgui - Win32 Debug"
SOURCE=.\geticgui.cpp

"$(INTDIR)\geticgui.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\geticgui.rc

"$(INTDIR)\geticgui.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

