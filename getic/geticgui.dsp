# Microsoft Developer Studio Project File - Name="geticgui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=geticgui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "geticgui.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "geticgui - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETICGUI_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /I "..\baselib" /D "_WINDOWS" /D "_USRDLL" /D "GETICGUI_EXPORTS" /D "NDEBUG" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"..\..\_bin\geticgui.dll"

!ELSEIF  "$(CFG)" == "geticgui - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETICGUI_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\_lib" /I "..\baselib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GETICGUI_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\_bin\geticgui.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "geticgui - Win32 Release"
# Name "geticgui - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\geticgui.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\aboutbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\aboutmar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\act_a.bin
# End Source File
# Begin Source File

SOURCE=.\res\act_a3.bin
# End Source File
# Begin Source File

SOURCE=.\res\act_a4.bin
# End Source File
# Begin Source File

SOURCE=.\res\act_b.bin
# End Source File
# Begin Source File

SOURCE=.\res\act_c.bin
# End Source File
# Begin Source File

SOURCE=.\res\act_d.bin
# End Source File
# Begin Source File

SOURCE=.\res\act_e.bin
# End Source File
# Begin Source File

SOURCE=".\res\bin-idr_.bin"
# End Source File
# Begin Source File

SOURCE=".\res\bin-itm_.bin"
# End Source File
# Begin Source File

SOURCE=.\res\bin00001.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00002.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00003.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00004.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00005.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00006.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00007.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00008.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00009.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00010.bin
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_2d_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_3d_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_butt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_edmarkers.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_splashedit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cmove1.cur
# End Source File
# Begin Source File

SOURCE=.\res\crotate.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor4.cur
# End Source File
# Begin Source File

SOURCE=.\res\flipd1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\flipf1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\flipu1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\flipx1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ge_aboutbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ge_cross.cur
# End Source File
# Begin Source File

SOURCE=.\res\ge_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\ge_margin.cur
# End Source File
# Begin Source File

SOURCE=.\res\ge_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\ge_normal.cur
# End Source File
# Begin Source File

SOURCE=.\res\ge_rotate.cur
# End Source File
# Begin Source File

SOURCE=.\res\ge_scale.cur
# End Source File
# Begin Source File

SOURCE=.\res\getic3.ico
# End Source File
# Begin Source File

SOURCE=.\GeticEditor.rgs
# End Source File
# Begin Source File

SOURCE=.\geticgui.rc
# End Source File
# Begin Source File

SOURCE=.\GeticPlug.rgs
# End Source File
# Begin Source File

SOURCE=.\res\gscript_.bin
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico_ani_.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico_cola.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico_sct1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\icon8.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_marg.cur
# End Source File
# Begin Source File

SOURCE=.\IGetic.rgs
# End Source File
# Begin Source File

SOURCE=.\res\item_typ.bmp
# End Source File
# Begin Source File

SOURCE=.\res\itm_grenade.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_heatrock.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_kadabum.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_laser.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_lbulb.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_machinegun.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_pistol.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_remotemine.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_rocket.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_script.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_shotgun.bin
# End Source File
# Begin Source File

SOURCE=.\res\itm_snipher.bin
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogetic.ico
# End Source File
# Begin Source File

SOURCE=.\res\pb_thflip_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_thflip_f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_thflip_u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_thflip_x.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_trot_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_trot_f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_trot_u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_trot_x.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_tvflip_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_tvflip_f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_tvflip_u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pb_tvflip_x.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sce_item.bin
# End Source File
# Begin Source File

SOURCE=.\res\scr_gscript_.bin
# End Source File
# Begin Source File

SOURCE=".\res\sidr=wpn.bin"
# End Source File
# Begin Source File

SOURCE=.\res\sintacti.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash_e2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splashedit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_align.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_align24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_align_disabled24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_build.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_build24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_build_disable24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_csg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_csg24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_csg_disable24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_main256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_main257.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_make.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_make1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_make24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_make256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_make_disable24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_script_ed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_select.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_select24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_select_disable24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_sizemove.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_sizemove24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_sizemove_disable24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_splines.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_splines24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_splines24d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_view.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_view24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bpp\tb_view_disable24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texrotd1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texrotf1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texrotu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texrotu1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texrotx1.bmp
# End Source File
# Begin Source File

SOURCE=".\res\toobar-make.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar-align.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar-csg.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar-make.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar-move.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar-select.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar-sintacti.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar-views.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_aquadrat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tx_2d1.ico
# End Source File
# Begin Source File

SOURCE=.\res\tx_2d2.ico
# End Source File
# Begin Source File

SOURCE=.\res\tx_2d_1.ico
# End Source File
# Begin Source File

SOURCE=.\res\tx_2d_3.ico
# End Source File
# Begin Source File

SOURCE=.\res\tx_2d_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\tx_3d_2.ico
# End Source File
# Begin Source File

SOURCE=.\res\tx_3d_3.ico
# End Source File
# Begin Source File

SOURCE=.\res\wpn_gren.bin
# End Source File
# Begin Source File

SOURCE=".\Z-edmap.rgs"
# End Source File
# Begin Source File

SOURCE=.\res\z_ed2.ico
# End Source File
# Begin Source File

SOURCE=.\res\z_ed2Doc.ico
# End Source File
# End Group
# End Target
# End Project
