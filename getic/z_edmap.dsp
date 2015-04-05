# Microsoft Developer Studio Project File - Name="z_edmap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=z_edmap - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "z_edmap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "z_edmap.mak" CFG="z_edmap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "z_edmap - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "z_edmap - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "z_edmap - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /Ob2 /Gy /I "..\..\baselib" /I "..\..\_include" /D "NDEBUG" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcmtd.lib" /out:"..\..\_bin\getic.exe" /libpath:"..\..\lib\\"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\baselib" /I "..\..\_include" /D "_DEBUG" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /Fr /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /out:"..\..\_bin\geticd.exe" /pdbtype:sept /libpath:"..\..\lib\\"

!ENDIF 

# Begin Target

# Name "z_edmap - Win32 Release"
# Name "z_edmap - Win32 Debug"
# Begin Group "Compiler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BspTree.cpp
# End Source File
# Begin Source File

SOURCE=.\BspTree.h
# End Source File
# Begin Source File

SOURCE=.\Compiler.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler.h
# End Source File
# Begin Source File

SOURCE=.\CompilerStream.cpp
# End Source File
# Begin Source File

SOURCE=.\LMProc.cpp
# End Source File
# Begin Source File

SOURCE=.\LMProc.h
# End Source File
# Begin Source File

SOURCE=.\MiniBsp.cpp
# End Source File
# Begin Source File

SOURCE=.\MiniBsp.h
# End Source File
# Begin Source File

SOURCE=.\PortalMerge.cpp
# End Source File
# Begin Source File

SOURCE=.\PortalPRC.cpp
# End Source File
# Begin Source File

SOURCE=.\PortalPRC.h
# End Source File
# Begin Source File

SOURCE=.\PVSPrc.cpp
# End Source File
# Begin Source File

SOURCE=.\PVSPrc.h
# End Source File
# Begin Source File

SOURCE=.\TerTree.cpp
# End Source File
# Begin Source File

SOURCE=.\TerTree.h
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BitArray.h
# End Source File
# Begin Source File

SOURCE=.\bsphook.h
# End Source File
# Begin Source File

SOURCE=.\bTbarCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\bTbarCombo.h
# End Source File
# Begin Source File

SOURCE=.\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\ColorButon.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButon.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\Crc.cpp
# End Source File
# Begin Source File

SOURCE=.\Crc.h
# End Source File
# Begin Source File

SOURCE=.\Curve_Class.cpp
# End Source File
# Begin Source File

SOURCE=.\Curve_Class.h
# End Source File
# Begin Source File

SOURCE=.\DirTree.cpp
# End Source File
# Begin Source File

SOURCE=.\DirTree.h
# End Source File
# Begin Source File

SOURCE=.\DlgConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgConsole.h
# End Source File
# Begin Source File

SOURCE=.\DlgTexMody.h
# End Source File
# Begin Source File

SOURCE=.\Drawout1.cpp
# End Source File
# Begin Source File

SOURCE=.\Drawout1.h
# End Source File
# Begin Source File

SOURCE=.\FixSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\FixSplitter.h
# End Source File
# Begin Source File

SOURCE=.\Frustrum.cpp
# End Source File
# Begin Source File

SOURCE=.\Frustrum.h
# End Source File
# Begin Source File

SOURCE=.\GeoUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\GeoUtils.h
# End Source File
# Begin Source File

SOURCE=.\GListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LbEditable.cpp
# End Source File
# Begin Source File

SOURCE=.\LbEditable.h
# End Source File
# Begin Source File

SOURCE=.\LboxItem.cpp
# End Source File
# Begin Source File

SOURCE=.\LboxItem.h
# End Source File
# Begin Source File

SOURCE=.\MaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MaskEdit.h
# End Source File
# Begin Source File

SOURCE=.\MMove.h
# End Source File
# Begin Source File

SOURCE=.\PrpLb.cpp
# End Source File
# Begin Source File

SOURCE=.\PrpLb.h
# End Source File
# Begin Source File

SOURCE=.\RepLBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RepLBox.h
# End Source File
# Begin Source File

SOURCE=.\StaticGL.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticGL.h
# End Source File
# Begin Source File

SOURCE=.\Subdivider.cpp
# End Source File
# Begin Source File

SOURCE=.\Subdivider.h
# End Source File
# Begin Source File

SOURCE=.\tasks.h
# End Source File
# Begin Source File

SOURCE=.\TexAdapter.cpp
# End Source File
# Begin Source File

SOURCE=.\TexAdapter.h
# End Source File
# Begin Source File

SOURCE=.\TexLbox.cpp
# End Source File
# Begin Source File

SOURCE=.\TexLbox.h
# End Source File
# Begin Source File

SOURCE=.\TexRef.cpp
# End Source File
# Begin Source File

SOURCE=.\TexRef.h
# End Source File
# Begin Source File

SOURCE=.\texsys.cpp
# End Source File
# Begin Source File

SOURCE=.\texsys.h
# End Source File
# Begin Source File

SOURCE=.\TreeScene.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeScene.h
# End Source File
# Begin Source File

SOURCE=.\Undo.cpp
# End Source File
# Begin Source File

SOURCE=.\Undo.h
# End Source File
# Begin Source File

SOURCE=.\VertDlgDlg.cpp
# End Source File
# End Group
# Begin Group "Modal Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialogEvalDets.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogEvalDets.h
# End Source File
# Begin Source File

SOURCE=.\DlgBigTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBigTerrain.h
# End Source File
# Begin Source File

SOURCE=.\DlgClassItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgClassItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgCompile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCompile.h
# End Source File
# Begin Source File

SOURCE=.\DlgCpxPrimnames.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCpxPrimnames.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditor2D.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditor2D.h
# End Source File
# Begin Source File

SOURCE=.\DlgGs.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGs.h
# End Source File
# Begin Source File

SOURCE=.\DlgLeafs.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLeafs.h
# End Source File
# Begin Source File

SOURCE=.\DlgMotionState.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMotionState.h
# End Source File
# Begin Source File

SOURCE=.\DlgMurrChs.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMurrChs.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewBrush.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewPref.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewPref.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewTerrain.h
# End Source File
# Begin Source File

SOURCE=.\DlgPreferences.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPreferences.h
# End Source File
# Begin Source File

SOURCE=.\DlgResBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgResBrowser.h
# End Source File
# Begin Source File

SOURCE=.\DlgTexMody.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTexRefs.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTexRefs.h
# End Source File
# End Group
# Begin Group "Bar Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseItemsDlgs.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseItemsDlgs.h
# End Source File
# Begin Source File

SOURCE=.\DlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarLevExpl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarLevExpl.h
# End Source File
# Begin Source File

SOURCE=.\DlgBrshProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBrshProp.h
# End Source File
# Begin Source File

SOURCE=.\DlgCompProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCompProgress.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaceProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaceProp.h
# End Source File
# Begin Source File

SOURCE=.\DlgGameItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGameItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgItems.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgItems.h
# End Source File
# Begin Source File

SOURCE=.\DlgLightItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLightItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgScene.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgScene.h
# End Source File
# Begin Source File

SOURCE=.\DlgSoundItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSoundItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgStartPlayItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStartPlayItem.h
# End Source File
# Begin Source File

SOURCE=.\DlgTextures.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTextures.h
# End Source File
# Begin Source File

SOURCE=.\DlgTriggerItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTriggerItem.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabDlgSel.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDlgSel.h
# End Source File
# End Group
# Begin Group "Scene Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseEntity.h
# End Source File
# Begin Source File

SOURCE=.\BigTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\BigTerrain.h
# End Source File
# Begin Source File

SOURCE=.\brush.cpp
# End Source File
# Begin Source File

SOURCE=.\Brush.h
# End Source File
# Begin Source File

SOURCE=.\CustItemCat.cpp
# End Source File
# Begin Source File

SOURCE=.\CustItemCat.h
# End Source File
# Begin Source File

SOURCE=.\GameItem.cpp
# End Source File
# Begin Source File

SOURCE=.\GameItem.h
# End Source File
# Begin Source File

SOURCE=.\LightBulb.cpp
# End Source File
# Begin Source File

SOURCE=.\LightBulb.h
# End Source File
# Begin Source File

SOURCE=.\Motion.cpp
# End Source File
# Begin Source File

SOURCE=.\Motion.h
# End Source File
# Begin Source File

SOURCE=.\Poly.cpp
# End Source File
# Begin Source File

SOURCE=.\Poly.h
# End Source File
# Begin Source File

SOURCE=.\SceItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SceItem.h
# End Source File
# Begin Source File

SOURCE=.\Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\Scene.h
# End Source File
# Begin Source File

SOURCE=.\ScriptItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptItem.h
# End Source File
# Begin Source File

SOURCE=.\SkyDom.cpp
# End Source File
# Begin Source File

SOURCE=.\SkyDom.h
# End Source File
# Begin Source File

SOURCE=.\SoundItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundItem.h
# End Source File
# Begin Source File

SOURCE=.\SplinePath.cpp
# End Source File
# Begin Source File

SOURCE=.\SplinePath.h
# End Source File
# Begin Source File

SOURCE=.\StartPlayItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StartPlayItem.h
# End Source File
# Begin Source File

SOURCE=.\TriggerItem.cpp
# End Source File
# Begin Source File

SOURCE=.\TriggerItem.h
# End Source File
# Begin Source File

SOURCE=.\Zone.cpp
# End Source File
# Begin Source File

SOURCE=.\Zone.h
# End Source File
# End Group
# Begin Group "Views "

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\z_ed2View.cpp
# End Source File
# Begin Source File

SOURCE=.\z_ed2View.h
# End Source File
# Begin Source File

SOURCE=.\z_ed3View.cpp
# End Source File
# Begin Source File

SOURCE=.\z_ed3View.h
# End Source File
# End Group
# Begin Group "Documents"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\z_ed2Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\z_ed2Doc.h
# End Source File
# Begin Source File

SOURCE=.\z_ed2DocAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\z_ed2DocDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\z_ed2DocMk.cpp
# End Source File
# Begin Source File

SOURCE=.\z_ed2DocProc.cpp
# End Source File
# Begin Source File

SOURCE=.\z_ed2DocSave.cpp
# End Source File
# End Group
# Begin Group "Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\scripthdr.h
# End Source File
# Begin Source File

SOURCE=.\Scripts.cpp
# End Source File
# Begin Source File

SOURCE=.\Scripts.h
# End Source File
# End Group
# Begin Group "SintacticEditor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CCrystalEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\CCrystalEditView.h
# End Source File
# Begin Source File

SOURCE=.\CCrystalEditView.inl
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextBuffer.h
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextBuffer.inl
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView.cpp
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView.h
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView.inl
# End Source File
# Begin Source File

SOURCE=.\CCrystalTextView2.cpp
# End Source File
# Begin Source File

SOURCE=.\cedefs.h
# End Source File
# Begin Source File

SOURCE=.\CEditReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CEditReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\CFindTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CFindTextDlg.h
# End Source File
# Begin Source File

SOURCE=.\CompilerOutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CompilerOutDlg.h
# End Source File
# Begin Source File

SOURCE=.\cplusplus.cpp
# End Source File
# Begin Source File

SOURCE=.\CppCompiler.cpp
# End Source File
# Begin Source File

SOURCE=.\CppCompiler.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarScript.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarScript.h
# End Source File
# Begin Source File

SOURCE=.\DlgCompilerPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCompilerPath.h
# End Source File
# Begin Source File

SOURCE=.\editcmd.h
# End Source File
# Begin Source File

SOURCE=.\editreg.h
# End Source File
# Begin Source File

SOURCE=.\SampleDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleDoc.h
# End Source File
# Begin Source File

SOURCE=.\SampleView.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleView.h
# End Source File
# Begin Source File

SOURCE=.\ToolFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolFrame.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\baselibhdrs.h
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\DlgUnlock.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUnlock.h
# End Source File
# Begin Source File

SOURCE=.\ExceptionLog.cpp
# End Source File
# Begin Source File

SOURCE=.\ExceptionLog.h
# End Source File
# Begin Source File

SOURCE=.\res\filetypes.bmp
# End Source File
# Begin Source File

SOURCE=.\getic3.ico
# End Source File
# Begin Source File

SOURCE=.\GeticPlug.rgs
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\IGetic.rgs
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MyStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\res\tb_make256.bmp
# End Source File
# Begin Source File

SOURCE=.\UiEnhancer.cpp
# End Source File
# Begin Source File

SOURCE=.\UiEnhancer.h
# End Source File
# Begin Source File

SOURCE=.\versionmacro.h
# End Source File
# Begin Source File

SOURCE=".\Z-edmap.rgs"
# End Source File
# Begin Source File

SOURCE=".\z-edmapkey.cpp"
# End Source File
# Begin Source File

SOURCE=".\z_edmap.cpp"
# End Source File
# Begin Source File

SOURCE=".\z_edmap.rc"
# End Source File
# End Target
# End Project
# Section z_edmap : {004E0002-0002-0041-0200-420002004400}
# 	1:10:IDR_ZEDMAP:109
# End Section
