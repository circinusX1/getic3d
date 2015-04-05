# Microsoft Developer Studio Generated NMAKE File, Based on z_edmap.dsp
!IF "$(CFG)" == ""
CFG=z_edmap - Win32 Debug
!MESSAGE No configuration specified. Defaulting to z_edmap - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "z_edmap - Win32 Release" && "$(CFG)" != "z_edmap - Win32 Debug" && "$(CFG)" != "z_edmap - Win32 ReleaseFree"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "z_edmap.mak" CFG="z_edmap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "z_edmap - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "z_edmap - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "z_edmap - Win32 ReleaseFree" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "z_edmap - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\_bin\getic.exe" "$(OUTDIR)\z_edmap.pch"


CLEAN :
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\BaseItemsDlgs.obj"
	-@erase "$(INTDIR)\brush.obj"
	-@erase "$(INTDIR)\BspTree.obj"
	-@erase "$(INTDIR)\bTbarCombo.obj"
	-@erase "$(INTDIR)\BzipCompressor.obj"
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\CCrystalEditView.obj"
	-@erase "$(INTDIR)\CCrystalTextBuffer.obj"
	-@erase "$(INTDIR)\CCrystalTextView.obj"
	-@erase "$(INTDIR)\CCrystalTextView2.obj"
	-@erase "$(INTDIR)\CEditReplaceDlg.obj"
	-@erase "$(INTDIR)\CFindTextDlg.obj"
	-@erase "$(INTDIR)\ColorButon.obj"
	-@erase "$(INTDIR)\Compiler.obj"
	-@erase "$(INTDIR)\CompilerOutDlg.obj"
	-@erase "$(INTDIR)\CompilerStream.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\cplusplus.obj"
	-@erase "$(INTDIR)\CppCompiler.obj"
	-@erase "$(INTDIR)\Crc.obj"
	-@erase "$(INTDIR)\Curve_Class.obj"
	-@erase "$(INTDIR)\CustItemCat.obj"
	-@erase "$(INTDIR)\DialogEvalDets.obj"
	-@erase "$(INTDIR)\DirTree.obj"
	-@erase "$(INTDIR)\DlgBar.obj"
	-@erase "$(INTDIR)\DlgBarLevExpl.obj"
	-@erase "$(INTDIR)\DlgBarScript.obj"
	-@erase "$(INTDIR)\DlgBrshProp.obj"
	-@erase "$(INTDIR)\DlgClassItem.obj"
	-@erase "$(INTDIR)\DlgCompile.obj"
	-@erase "$(INTDIR)\DlgCompilerPath.obj"
	-@erase "$(INTDIR)\DlgCompProgress.obj"
	-@erase "$(INTDIR)\DlgConsole.obj"
	-@erase "$(INTDIR)\DlgCpxPrimnames.obj"
	-@erase "$(INTDIR)\DlgEditor2D.obj"
	-@erase "$(INTDIR)\DlgFaceProp.obj"
	-@erase "$(INTDIR)\DlgGameItem.obj"
	-@erase "$(INTDIR)\DlgGs.obj"
	-@erase "$(INTDIR)\DlgItems.obj"
	-@erase "$(INTDIR)\DlgLightItem.obj"
	-@erase "$(INTDIR)\DlgMotionState.obj"
	-@erase "$(INTDIR)\DlgMurrChs.obj"
	-@erase "$(INTDIR)\DlgNewBrush.obj"
	-@erase "$(INTDIR)\DlgNewItem.obj"
	-@erase "$(INTDIR)\DlgNewPref.obj"
	-@erase "$(INTDIR)\DlgNewTerrain.obj"
	-@erase "$(INTDIR)\DlgPreferences.obj"
	-@erase "$(INTDIR)\DlgScene.obj"
	-@erase "$(INTDIR)\DlgScripts.obj"
	-@erase "$(INTDIR)\DlgSounds.obj"
	-@erase "$(INTDIR)\DlgTexMody.obj"
	-@erase "$(INTDIR)\DlgTexRefs.obj"
	-@erase "$(INTDIR)\DlgTextures.obj"
	-@erase "$(INTDIR)\DlgTriggerItem.obj"
	-@erase "$(INTDIR)\Drawout1.obj"
	-@erase "$(INTDIR)\FixSplitter.obj"
	-@erase "$(INTDIR)\Frustrum.obj"
	-@erase "$(INTDIR)\GameItem.obj"
	-@erase "$(INTDIR)\GeoUtils.obj"
	-@erase "$(INTDIR)\GListCtrl.obj"
	-@erase "$(INTDIR)\LbEditable.obj"
	-@erase "$(INTDIR)\LboxItem.obj"
	-@erase "$(INTDIR)\LightBulb.obj"
	-@erase "$(INTDIR)\LMProc.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MaskEdit.obj"
	-@erase "$(INTDIR)\MiniBsp.obj"
	-@erase "$(INTDIR)\Motion.obj"
	-@erase "$(INTDIR)\MyStatusBar.obj"
	-@erase "$(INTDIR)\Poly.obj"
	-@erase "$(INTDIR)\PortalMerge.obj"
	-@erase "$(INTDIR)\PortalPRC.obj"
	-@erase "$(INTDIR)\ProgressDlg.obj"
	-@erase "$(INTDIR)\PrpLb.obj"
	-@erase "$(INTDIR)\PVSPrc.obj"
	-@erase "$(INTDIR)\RepLBox.obj"
	-@erase "$(INTDIR)\rfHillTerrain.obj"
	-@erase "$(INTDIR)\SampleDoc.obj"
	-@erase "$(INTDIR)\SampleView.obj"
	-@erase "$(INTDIR)\SceItem.obj"
	-@erase "$(INTDIR)\Scene.obj"
	-@erase "$(INTDIR)\ScriptItem.obj"
	-@erase "$(INTDIR)\Scripts.obj"
	-@erase "$(INTDIR)\SkyDom.obj"
	-@erase "$(INTDIR)\SplinePath.obj"
	-@erase "$(INTDIR)\StaticGL.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TabDlgSel.obj"
	-@erase "$(INTDIR)\TexAdapter.obj"
	-@erase "$(INTDIR)\TexLbox.obj"
	-@erase "$(INTDIR)\TexRef.obj"
	-@erase "$(INTDIR)\texsys.obj"
	-@erase "$(INTDIR)\ToolFrame.obj"
	-@erase "$(INTDIR)\TreeScene.obj"
	-@erase "$(INTDIR)\TriggerItem.obj"
	-@erase "$(INTDIR)\UiEnhancer.obj"
	-@erase "$(INTDIR)\Undo.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VertDlgDlg.obj"
	-@erase "$(INTDIR)\XUnzip.obj"
	-@erase "$(INTDIR)\XZip.obj"
	-@erase "$(INTDIR)\z-edmapkey.obj"
	-@erase "$(INTDIR)\z_ed2Doc.obj"
	-@erase "$(INTDIR)\z_ed2DocAlign.obj"
	-@erase "$(INTDIR)\z_ed2DocDraw.obj"
	-@erase "$(INTDIR)\z_ed2DocMk.obj"
	-@erase "$(INTDIR)\z_ed2DocProc.obj"
	-@erase "$(INTDIR)\z_ed2DocSave.obj"
	-@erase "$(INTDIR)\z_ed2DocSave_Old.obj"
	-@erase "$(INTDIR)\z_ed2View.obj"
	-@erase "$(INTDIR)\z_ed3View.obj"
	-@erase "$(INTDIR)\z_edmap.obj"
	-@erase "$(INTDIR)\z_edmap.pch"
	-@erase "$(INTDIR)\z_edmap.res"
	-@erase "$(INTDIR)\Zone.obj"
	-@erase "..\_bin\getic.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /Od /Ob2 /I "..\z-utils" /I "..\z-engine\\" /I "..\baselib\\" /I "..\baselib" /I "..\_include" /I "..\BzLib\\" /D "NDEBUG" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\z_edmap.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\z_edmap.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\getic.pdb" /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrt.lib" /out:"..\_bin\getic.exe" /libpath:"..\lib\\" 
LINK32_OBJS= \
	"$(INTDIR)\BspTree.obj" \
	"$(INTDIR)\Compiler.obj" \
	"$(INTDIR)\CompilerStream.obj" \
	"$(INTDIR)\LMProc.obj" \
	"$(INTDIR)\MiniBsp.obj" \
	"$(INTDIR)\PortalMerge.obj" \
	"$(INTDIR)\PortalPRC.obj" \
	"$(INTDIR)\PVSPrc.obj" \
	"$(INTDIR)\bTbarCombo.obj" \
	"$(INTDIR)\BzipCompressor.obj" \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\ColorButon.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\Crc.obj" \
	"$(INTDIR)\Curve_Class.obj" \
	"$(INTDIR)\DirTree.obj" \
	"$(INTDIR)\DlgConsole.obj" \
	"$(INTDIR)\Drawout1.obj" \
	"$(INTDIR)\FixSplitter.obj" \
	"$(INTDIR)\Frustrum.obj" \
	"$(INTDIR)\GeoUtils.obj" \
	"$(INTDIR)\GListCtrl.obj" \
	"$(INTDIR)\LbEditable.obj" \
	"$(INTDIR)\LboxItem.obj" \
	"$(INTDIR)\MaskEdit.obj" \
	"$(INTDIR)\PrpLb.obj" \
	"$(INTDIR)\RepLBox.obj" \
	"$(INTDIR)\StaticGL.obj" \
	"$(INTDIR)\TexAdapter.obj" \
	"$(INTDIR)\TexLbox.obj" \
	"$(INTDIR)\TexRef.obj" \
	"$(INTDIR)\texsys.obj" \
	"$(INTDIR)\TreeScene.obj" \
	"$(INTDIR)\Undo.obj" \
	"$(INTDIR)\VertDlgDlg.obj" \
	"$(INTDIR)\XUnzip.obj" \
	"$(INTDIR)\XZip.obj" \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\DialogEvalDets.obj" \
	"$(INTDIR)\DlgClassItem.obj" \
	"$(INTDIR)\DlgCompile.obj" \
	"$(INTDIR)\DlgCpxPrimnames.obj" \
	"$(INTDIR)\DlgEditor2D.obj" \
	"$(INTDIR)\DlgGs.obj" \
	"$(INTDIR)\DlgMotionState.obj" \
	"$(INTDIR)\DlgMurrChs.obj" \
	"$(INTDIR)\DlgNewBrush.obj" \
	"$(INTDIR)\DlgNewItem.obj" \
	"$(INTDIR)\DlgNewPref.obj" \
	"$(INTDIR)\DlgNewTerrain.obj" \
	"$(INTDIR)\DlgPreferences.obj" \
	"$(INTDIR)\DlgTexMody.obj" \
	"$(INTDIR)\DlgTexRefs.obj" \
	"$(INTDIR)\BaseItemsDlgs.obj" \
	"$(INTDIR)\DlgBar.obj" \
	"$(INTDIR)\DlgBarLevExpl.obj" \
	"$(INTDIR)\DlgBrshProp.obj" \
	"$(INTDIR)\DlgCompProgress.obj" \
	"$(INTDIR)\DlgFaceProp.obj" \
	"$(INTDIR)\DlgGameItem.obj" \
	"$(INTDIR)\DlgItems.obj" \
	"$(INTDIR)\DlgLightItem.obj" \
	"$(INTDIR)\DlgScene.obj" \
	"$(INTDIR)\DlgScripts.obj" \
	"$(INTDIR)\DlgSounds.obj" \
	"$(INTDIR)\DlgTextures.obj" \
	"$(INTDIR)\DlgTriggerItem.obj" \
	"$(INTDIR)\ProgressDlg.obj" \
	"$(INTDIR)\TabDlgSel.obj" \
	"$(INTDIR)\brush.obj" \
	"$(INTDIR)\CustItemCat.obj" \
	"$(INTDIR)\GameItem.obj" \
	"$(INTDIR)\LightBulb.obj" \
	"$(INTDIR)\Motion.obj" \
	"$(INTDIR)\Poly.obj" \
	"$(INTDIR)\SceItem.obj" \
	"$(INTDIR)\Scene.obj" \
	"$(INTDIR)\ScriptItem.obj" \
	"$(INTDIR)\SkyDom.obj" \
	"$(INTDIR)\SplinePath.obj" \
	"$(INTDIR)\TriggerItem.obj" \
	"$(INTDIR)\Zone.obj" \
	"$(INTDIR)\z_ed2View.obj" \
	"$(INTDIR)\z_ed3View.obj" \
	"$(INTDIR)\z_ed2Doc.obj" \
	"$(INTDIR)\z_ed2DocAlign.obj" \
	"$(INTDIR)\z_ed2DocDraw.obj" \
	"$(INTDIR)\z_ed2DocMk.obj" \
	"$(INTDIR)\z_ed2DocProc.obj" \
	"$(INTDIR)\z_ed2DocSave.obj" \
	"$(INTDIR)\z_ed2DocSave_Old.obj" \
	"$(INTDIR)\Scripts.obj" \
	"$(INTDIR)\CCrystalEditView.obj" \
	"$(INTDIR)\CCrystalTextBuffer.obj" \
	"$(INTDIR)\CCrystalTextView.obj" \
	"$(INTDIR)\CCrystalTextView2.obj" \
	"$(INTDIR)\CEditReplaceDlg.obj" \
	"$(INTDIR)\CFindTextDlg.obj" \
	"$(INTDIR)\CompilerOutDlg.obj" \
	"$(INTDIR)\cplusplus.obj" \
	"$(INTDIR)\CppCompiler.obj" \
	"$(INTDIR)\DlgBarScript.obj" \
	"$(INTDIR)\DlgCompilerPath.obj" \
	"$(INTDIR)\SampleDoc.obj" \
	"$(INTDIR)\SampleView.obj" \
	"$(INTDIR)\ToolFrame.obj" \
	"$(INTDIR)\rfHillTerrain.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyStatusBar.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UiEnhancer.obj" \
	"$(INTDIR)\z-edmapkey.obj" \
	"$(INTDIR)\z_edmap.obj" \
	"$(INTDIR)\z_edmap.res"

"..\_bin\getic.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\_bin\geticd.exe" "$(OUTDIR)\z_edmap.pch" "$(OUTDIR)\z_edmap.bsc"


CLEAN :
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\AboutDlg.sbr"
	-@erase "$(INTDIR)\BaseItemsDlgs.obj"
	-@erase "$(INTDIR)\BaseItemsDlgs.sbr"
	-@erase "$(INTDIR)\brush.obj"
	-@erase "$(INTDIR)\brush.sbr"
	-@erase "$(INTDIR)\BspTree.obj"
	-@erase "$(INTDIR)\BspTree.sbr"
	-@erase "$(INTDIR)\bTbarCombo.obj"
	-@erase "$(INTDIR)\bTbarCombo.sbr"
	-@erase "$(INTDIR)\BzipCompressor.obj"
	-@erase "$(INTDIR)\BzipCompressor.sbr"
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\camera.sbr"
	-@erase "$(INTDIR)\CCrystalEditView.obj"
	-@erase "$(INTDIR)\CCrystalEditView.sbr"
	-@erase "$(INTDIR)\CCrystalTextBuffer.obj"
	-@erase "$(INTDIR)\CCrystalTextBuffer.sbr"
	-@erase "$(INTDIR)\CCrystalTextView.obj"
	-@erase "$(INTDIR)\CCrystalTextView.sbr"
	-@erase "$(INTDIR)\CCrystalTextView2.obj"
	-@erase "$(INTDIR)\CCrystalTextView2.sbr"
	-@erase "$(INTDIR)\CEditReplaceDlg.obj"
	-@erase "$(INTDIR)\CEditReplaceDlg.sbr"
	-@erase "$(INTDIR)\CFindTextDlg.obj"
	-@erase "$(INTDIR)\CFindTextDlg.sbr"
	-@erase "$(INTDIR)\ColorButon.obj"
	-@erase "$(INTDIR)\ColorButon.sbr"
	-@erase "$(INTDIR)\Compiler.obj"
	-@erase "$(INTDIR)\Compiler.sbr"
	-@erase "$(INTDIR)\CompilerOutDlg.obj"
	-@erase "$(INTDIR)\CompilerOutDlg.sbr"
	-@erase "$(INTDIR)\CompilerStream.obj"
	-@erase "$(INTDIR)\CompilerStream.sbr"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\CoolDialogBar.sbr"
	-@erase "$(INTDIR)\cplusplus.obj"
	-@erase "$(INTDIR)\cplusplus.sbr"
	-@erase "$(INTDIR)\CppCompiler.obj"
	-@erase "$(INTDIR)\CppCompiler.sbr"
	-@erase "$(INTDIR)\Crc.obj"
	-@erase "$(INTDIR)\Crc.sbr"
	-@erase "$(INTDIR)\Curve_Class.obj"
	-@erase "$(INTDIR)\Curve_Class.sbr"
	-@erase "$(INTDIR)\CustItemCat.obj"
	-@erase "$(INTDIR)\CustItemCat.sbr"
	-@erase "$(INTDIR)\DialogEvalDets.obj"
	-@erase "$(INTDIR)\DialogEvalDets.sbr"
	-@erase "$(INTDIR)\DirTree.obj"
	-@erase "$(INTDIR)\DirTree.sbr"
	-@erase "$(INTDIR)\DlgBar.obj"
	-@erase "$(INTDIR)\DlgBar.sbr"
	-@erase "$(INTDIR)\DlgBarLevExpl.obj"
	-@erase "$(INTDIR)\DlgBarLevExpl.sbr"
	-@erase "$(INTDIR)\DlgBarScript.obj"
	-@erase "$(INTDIR)\DlgBarScript.sbr"
	-@erase "$(INTDIR)\DlgBrshProp.obj"
	-@erase "$(INTDIR)\DlgBrshProp.sbr"
	-@erase "$(INTDIR)\DlgClassItem.obj"
	-@erase "$(INTDIR)\DlgClassItem.sbr"
	-@erase "$(INTDIR)\DlgCompile.obj"
	-@erase "$(INTDIR)\DlgCompile.sbr"
	-@erase "$(INTDIR)\DlgCompilerPath.obj"
	-@erase "$(INTDIR)\DlgCompilerPath.sbr"
	-@erase "$(INTDIR)\DlgCompProgress.obj"
	-@erase "$(INTDIR)\DlgCompProgress.sbr"
	-@erase "$(INTDIR)\DlgConsole.obj"
	-@erase "$(INTDIR)\DlgConsole.sbr"
	-@erase "$(INTDIR)\DlgCpxPrimnames.obj"
	-@erase "$(INTDIR)\DlgCpxPrimnames.sbr"
	-@erase "$(INTDIR)\DlgEditor2D.obj"
	-@erase "$(INTDIR)\DlgEditor2D.sbr"
	-@erase "$(INTDIR)\DlgFaceProp.obj"
	-@erase "$(INTDIR)\DlgFaceProp.sbr"
	-@erase "$(INTDIR)\DlgGameItem.obj"
	-@erase "$(INTDIR)\DlgGameItem.sbr"
	-@erase "$(INTDIR)\DlgGs.obj"
	-@erase "$(INTDIR)\DlgGs.sbr"
	-@erase "$(INTDIR)\DlgItems.obj"
	-@erase "$(INTDIR)\DlgItems.sbr"
	-@erase "$(INTDIR)\DlgLightItem.obj"
	-@erase "$(INTDIR)\DlgLightItem.sbr"
	-@erase "$(INTDIR)\DlgMotionState.obj"
	-@erase "$(INTDIR)\DlgMotionState.sbr"
	-@erase "$(INTDIR)\DlgMurrChs.obj"
	-@erase "$(INTDIR)\DlgMurrChs.sbr"
	-@erase "$(INTDIR)\DlgNewBrush.obj"
	-@erase "$(INTDIR)\DlgNewBrush.sbr"
	-@erase "$(INTDIR)\DlgNewItem.obj"
	-@erase "$(INTDIR)\DlgNewItem.sbr"
	-@erase "$(INTDIR)\DlgNewPref.obj"
	-@erase "$(INTDIR)\DlgNewPref.sbr"
	-@erase "$(INTDIR)\DlgNewTerrain.obj"
	-@erase "$(INTDIR)\DlgNewTerrain.sbr"
	-@erase "$(INTDIR)\DlgPreferences.obj"
	-@erase "$(INTDIR)\DlgPreferences.sbr"
	-@erase "$(INTDIR)\DlgScene.obj"
	-@erase "$(INTDIR)\DlgScene.sbr"
	-@erase "$(INTDIR)\DlgScripts.obj"
	-@erase "$(INTDIR)\DlgScripts.sbr"
	-@erase "$(INTDIR)\DlgSounds.obj"
	-@erase "$(INTDIR)\DlgSounds.sbr"
	-@erase "$(INTDIR)\DlgTexMody.obj"
	-@erase "$(INTDIR)\DlgTexMody.sbr"
	-@erase "$(INTDIR)\DlgTexRefs.obj"
	-@erase "$(INTDIR)\DlgTexRefs.sbr"
	-@erase "$(INTDIR)\DlgTextures.obj"
	-@erase "$(INTDIR)\DlgTextures.sbr"
	-@erase "$(INTDIR)\DlgTriggerItem.obj"
	-@erase "$(INTDIR)\DlgTriggerItem.sbr"
	-@erase "$(INTDIR)\Drawout1.obj"
	-@erase "$(INTDIR)\Drawout1.sbr"
	-@erase "$(INTDIR)\FixSplitter.obj"
	-@erase "$(INTDIR)\FixSplitter.sbr"
	-@erase "$(INTDIR)\Frustrum.obj"
	-@erase "$(INTDIR)\Frustrum.sbr"
	-@erase "$(INTDIR)\GameItem.obj"
	-@erase "$(INTDIR)\GameItem.sbr"
	-@erase "$(INTDIR)\GeoUtils.obj"
	-@erase "$(INTDIR)\GeoUtils.sbr"
	-@erase "$(INTDIR)\GListCtrl.obj"
	-@erase "$(INTDIR)\GListCtrl.sbr"
	-@erase "$(INTDIR)\LbEditable.obj"
	-@erase "$(INTDIR)\LbEditable.sbr"
	-@erase "$(INTDIR)\LboxItem.obj"
	-@erase "$(INTDIR)\LboxItem.sbr"
	-@erase "$(INTDIR)\LightBulb.obj"
	-@erase "$(INTDIR)\LightBulb.sbr"
	-@erase "$(INTDIR)\LMProc.obj"
	-@erase "$(INTDIR)\LMProc.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MaskEdit.obj"
	-@erase "$(INTDIR)\MaskEdit.sbr"
	-@erase "$(INTDIR)\MiniBsp.obj"
	-@erase "$(INTDIR)\MiniBsp.sbr"
	-@erase "$(INTDIR)\Motion.obj"
	-@erase "$(INTDIR)\Motion.sbr"
	-@erase "$(INTDIR)\MyStatusBar.obj"
	-@erase "$(INTDIR)\MyStatusBar.sbr"
	-@erase "$(INTDIR)\Poly.obj"
	-@erase "$(INTDIR)\Poly.sbr"
	-@erase "$(INTDIR)\PortalMerge.obj"
	-@erase "$(INTDIR)\PortalMerge.sbr"
	-@erase "$(INTDIR)\PortalPRC.obj"
	-@erase "$(INTDIR)\PortalPRC.sbr"
	-@erase "$(INTDIR)\ProgressDlg.obj"
	-@erase "$(INTDIR)\ProgressDlg.sbr"
	-@erase "$(INTDIR)\PrpLb.obj"
	-@erase "$(INTDIR)\PrpLb.sbr"
	-@erase "$(INTDIR)\PVSPrc.obj"
	-@erase "$(INTDIR)\PVSPrc.sbr"
	-@erase "$(INTDIR)\RepLBox.obj"
	-@erase "$(INTDIR)\RepLBox.sbr"
	-@erase "$(INTDIR)\rfHillTerrain.obj"
	-@erase "$(INTDIR)\rfHillTerrain.sbr"
	-@erase "$(INTDIR)\SampleDoc.obj"
	-@erase "$(INTDIR)\SampleDoc.sbr"
	-@erase "$(INTDIR)\SampleView.obj"
	-@erase "$(INTDIR)\SampleView.sbr"
	-@erase "$(INTDIR)\SceItem.obj"
	-@erase "$(INTDIR)\SceItem.sbr"
	-@erase "$(INTDIR)\Scene.obj"
	-@erase "$(INTDIR)\Scene.sbr"
	-@erase "$(INTDIR)\ScriptItem.obj"
	-@erase "$(INTDIR)\ScriptItem.sbr"
	-@erase "$(INTDIR)\Scripts.obj"
	-@erase "$(INTDIR)\Scripts.sbr"
	-@erase "$(INTDIR)\SkyDom.obj"
	-@erase "$(INTDIR)\SkyDom.sbr"
	-@erase "$(INTDIR)\SplinePath.obj"
	-@erase "$(INTDIR)\SplinePath.sbr"
	-@erase "$(INTDIR)\StaticGL.obj"
	-@erase "$(INTDIR)\StaticGL.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TabDlgSel.obj"
	-@erase "$(INTDIR)\TabDlgSel.sbr"
	-@erase "$(INTDIR)\TexAdapter.obj"
	-@erase "$(INTDIR)\TexAdapter.sbr"
	-@erase "$(INTDIR)\TexLbox.obj"
	-@erase "$(INTDIR)\TexLbox.sbr"
	-@erase "$(INTDIR)\TexRef.obj"
	-@erase "$(INTDIR)\TexRef.sbr"
	-@erase "$(INTDIR)\texsys.obj"
	-@erase "$(INTDIR)\texsys.sbr"
	-@erase "$(INTDIR)\ToolFrame.obj"
	-@erase "$(INTDIR)\ToolFrame.sbr"
	-@erase "$(INTDIR)\TreeScene.obj"
	-@erase "$(INTDIR)\TreeScene.sbr"
	-@erase "$(INTDIR)\TriggerItem.obj"
	-@erase "$(INTDIR)\TriggerItem.sbr"
	-@erase "$(INTDIR)\UiEnhancer.obj"
	-@erase "$(INTDIR)\UiEnhancer.sbr"
	-@erase "$(INTDIR)\Undo.obj"
	-@erase "$(INTDIR)\Undo.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VertDlgDlg.obj"
	-@erase "$(INTDIR)\VertDlgDlg.sbr"
	-@erase "$(INTDIR)\XUnzip.obj"
	-@erase "$(INTDIR)\XUnzip.sbr"
	-@erase "$(INTDIR)\XZip.obj"
	-@erase "$(INTDIR)\XZip.sbr"
	-@erase "$(INTDIR)\z-edmapkey.obj"
	-@erase "$(INTDIR)\z-edmapkey.sbr"
	-@erase "$(INTDIR)\z_ed2Doc.obj"
	-@erase "$(INTDIR)\z_ed2Doc.sbr"
	-@erase "$(INTDIR)\z_ed2DocAlign.obj"
	-@erase "$(INTDIR)\z_ed2DocAlign.sbr"
	-@erase "$(INTDIR)\z_ed2DocDraw.obj"
	-@erase "$(INTDIR)\z_ed2DocDraw.sbr"
	-@erase "$(INTDIR)\z_ed2DocMk.obj"
	-@erase "$(INTDIR)\z_ed2DocMk.sbr"
	-@erase "$(INTDIR)\z_ed2DocProc.obj"
	-@erase "$(INTDIR)\z_ed2DocProc.sbr"
	-@erase "$(INTDIR)\z_ed2DocSave.obj"
	-@erase "$(INTDIR)\z_ed2DocSave.sbr"
	-@erase "$(INTDIR)\z_ed2DocSave_Old.obj"
	-@erase "$(INTDIR)\z_ed2DocSave_Old.sbr"
	-@erase "$(INTDIR)\z_ed2View.obj"
	-@erase "$(INTDIR)\z_ed2View.sbr"
	-@erase "$(INTDIR)\z_ed3View.obj"
	-@erase "$(INTDIR)\z_ed3View.sbr"
	-@erase "$(INTDIR)\z_edmap.obj"
	-@erase "$(INTDIR)\z_edmap.pch"
	-@erase "$(INTDIR)\z_edmap.res"
	-@erase "$(INTDIR)\z_edmap.sbr"
	-@erase "$(INTDIR)\Zone.obj"
	-@erase "$(INTDIR)\Zone.sbr"
	-@erase "$(OUTDIR)\geticd.pdb"
	-@erase "$(OUTDIR)\z_edmap.bsc"
	-@erase "..\_bin\geticd.exe"
	-@erase "..\_bin\geticd.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\baselib" /I "..\BzLib\\" /I "..\_include" /D "_DEBUG" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\z_edmap.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\z_edmap.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\BspTree.sbr" \
	"$(INTDIR)\Compiler.sbr" \
	"$(INTDIR)\CompilerStream.sbr" \
	"$(INTDIR)\LMProc.sbr" \
	"$(INTDIR)\MiniBsp.sbr" \
	"$(INTDIR)\PortalMerge.sbr" \
	"$(INTDIR)\PortalPRC.sbr" \
	"$(INTDIR)\PVSPrc.sbr" \
	"$(INTDIR)\bTbarCombo.sbr" \
	"$(INTDIR)\BzipCompressor.sbr" \
	"$(INTDIR)\camera.sbr" \
	"$(INTDIR)\ColorButon.sbr" \
	"$(INTDIR)\CoolDialogBar.sbr" \
	"$(INTDIR)\Crc.sbr" \
	"$(INTDIR)\Curve_Class.sbr" \
	"$(INTDIR)\DirTree.sbr" \
	"$(INTDIR)\DlgConsole.sbr" \
	"$(INTDIR)\Drawout1.sbr" \
	"$(INTDIR)\FixSplitter.sbr" \
	"$(INTDIR)\Frustrum.sbr" \
	"$(INTDIR)\GeoUtils.sbr" \
	"$(INTDIR)\GListCtrl.sbr" \
	"$(INTDIR)\LbEditable.sbr" \
	"$(INTDIR)\LboxItem.sbr" \
	"$(INTDIR)\MaskEdit.sbr" \
	"$(INTDIR)\PrpLb.sbr" \
	"$(INTDIR)\RepLBox.sbr" \
	"$(INTDIR)\StaticGL.sbr" \
	"$(INTDIR)\TexAdapter.sbr" \
	"$(INTDIR)\TexLbox.sbr" \
	"$(INTDIR)\TexRef.sbr" \
	"$(INTDIR)\texsys.sbr" \
	"$(INTDIR)\TreeScene.sbr" \
	"$(INTDIR)\Undo.sbr" \
	"$(INTDIR)\VertDlgDlg.sbr" \
	"$(INTDIR)\XUnzip.sbr" \
	"$(INTDIR)\XZip.sbr" \
	"$(INTDIR)\AboutDlg.sbr" \
	"$(INTDIR)\DialogEvalDets.sbr" \
	"$(INTDIR)\DlgClassItem.sbr" \
	"$(INTDIR)\DlgCompile.sbr" \
	"$(INTDIR)\DlgCpxPrimnames.sbr" \
	"$(INTDIR)\DlgEditor2D.sbr" \
	"$(INTDIR)\DlgGs.sbr" \
	"$(INTDIR)\DlgMotionState.sbr" \
	"$(INTDIR)\DlgMurrChs.sbr" \
	"$(INTDIR)\DlgNewBrush.sbr" \
	"$(INTDIR)\DlgNewItem.sbr" \
	"$(INTDIR)\DlgNewPref.sbr" \
	"$(INTDIR)\DlgNewTerrain.sbr" \
	"$(INTDIR)\DlgPreferences.sbr" \
	"$(INTDIR)\DlgTexMody.sbr" \
	"$(INTDIR)\DlgTexRefs.sbr" \
	"$(INTDIR)\BaseItemsDlgs.sbr" \
	"$(INTDIR)\DlgBar.sbr" \
	"$(INTDIR)\DlgBarLevExpl.sbr" \
	"$(INTDIR)\DlgBrshProp.sbr" \
	"$(INTDIR)\DlgCompProgress.sbr" \
	"$(INTDIR)\DlgFaceProp.sbr" \
	"$(INTDIR)\DlgGameItem.sbr" \
	"$(INTDIR)\DlgItems.sbr" \
	"$(INTDIR)\DlgLightItem.sbr" \
	"$(INTDIR)\DlgScene.sbr" \
	"$(INTDIR)\DlgScripts.sbr" \
	"$(INTDIR)\DlgSounds.sbr" \
	"$(INTDIR)\DlgTextures.sbr" \
	"$(INTDIR)\DlgTriggerItem.sbr" \
	"$(INTDIR)\ProgressDlg.sbr" \
	"$(INTDIR)\TabDlgSel.sbr" \
	"$(INTDIR)\brush.sbr" \
	"$(INTDIR)\CustItemCat.sbr" \
	"$(INTDIR)\GameItem.sbr" \
	"$(INTDIR)\LightBulb.sbr" \
	"$(INTDIR)\Motion.sbr" \
	"$(INTDIR)\Poly.sbr" \
	"$(INTDIR)\SceItem.sbr" \
	"$(INTDIR)\Scene.sbr" \
	"$(INTDIR)\ScriptItem.sbr" \
	"$(INTDIR)\SkyDom.sbr" \
	"$(INTDIR)\SplinePath.sbr" \
	"$(INTDIR)\TriggerItem.sbr" \
	"$(INTDIR)\Zone.sbr" \
	"$(INTDIR)\z_ed2View.sbr" \
	"$(INTDIR)\z_ed3View.sbr" \
	"$(INTDIR)\z_ed2Doc.sbr" \
	"$(INTDIR)\z_ed2DocAlign.sbr" \
	"$(INTDIR)\z_ed2DocDraw.sbr" \
	"$(INTDIR)\z_ed2DocMk.sbr" \
	"$(INTDIR)\z_ed2DocProc.sbr" \
	"$(INTDIR)\z_ed2DocSave.sbr" \
	"$(INTDIR)\z_ed2DocSave_Old.sbr" \
	"$(INTDIR)\Scripts.sbr" \
	"$(INTDIR)\CCrystalEditView.sbr" \
	"$(INTDIR)\CCrystalTextBuffer.sbr" \
	"$(INTDIR)\CCrystalTextView.sbr" \
	"$(INTDIR)\CCrystalTextView2.sbr" \
	"$(INTDIR)\CEditReplaceDlg.sbr" \
	"$(INTDIR)\CFindTextDlg.sbr" \
	"$(INTDIR)\CompilerOutDlg.sbr" \
	"$(INTDIR)\cplusplus.sbr" \
	"$(INTDIR)\CppCompiler.sbr" \
	"$(INTDIR)\DlgBarScript.sbr" \
	"$(INTDIR)\DlgCompilerPath.sbr" \
	"$(INTDIR)\SampleDoc.sbr" \
	"$(INTDIR)\SampleView.sbr" \
	"$(INTDIR)\ToolFrame.sbr" \
	"$(INTDIR)\rfHillTerrain.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MyStatusBar.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\UiEnhancer.sbr" \
	"$(INTDIR)\z-edmapkey.sbr" \
	"$(INTDIR)\z_edmap.sbr"

"$(OUTDIR)\z_edmap.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\geticd.pdb" /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrt.lib" /out:"..\_bin\geticd.exe" /pdbtype:sept /libpath:"..\lib\\" 
LINK32_OBJS= \
	"$(INTDIR)\BspTree.obj" \
	"$(INTDIR)\Compiler.obj" \
	"$(INTDIR)\CompilerStream.obj" \
	"$(INTDIR)\LMProc.obj" \
	"$(INTDIR)\MiniBsp.obj" \
	"$(INTDIR)\PortalMerge.obj" \
	"$(INTDIR)\PortalPRC.obj" \
	"$(INTDIR)\PVSPrc.obj" \
	"$(INTDIR)\bTbarCombo.obj" \
	"$(INTDIR)\BzipCompressor.obj" \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\ColorButon.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\Crc.obj" \
	"$(INTDIR)\Curve_Class.obj" \
	"$(INTDIR)\DirTree.obj" \
	"$(INTDIR)\DlgConsole.obj" \
	"$(INTDIR)\Drawout1.obj" \
	"$(INTDIR)\FixSplitter.obj" \
	"$(INTDIR)\Frustrum.obj" \
	"$(INTDIR)\GeoUtils.obj" \
	"$(INTDIR)\GListCtrl.obj" \
	"$(INTDIR)\LbEditable.obj" \
	"$(INTDIR)\LboxItem.obj" \
	"$(INTDIR)\MaskEdit.obj" \
	"$(INTDIR)\PrpLb.obj" \
	"$(INTDIR)\RepLBox.obj" \
	"$(INTDIR)\StaticGL.obj" \
	"$(INTDIR)\TexAdapter.obj" \
	"$(INTDIR)\TexLbox.obj" \
	"$(INTDIR)\TexRef.obj" \
	"$(INTDIR)\texsys.obj" \
	"$(INTDIR)\TreeScene.obj" \
	"$(INTDIR)\Undo.obj" \
	"$(INTDIR)\VertDlgDlg.obj" \
	"$(INTDIR)\XUnzip.obj" \
	"$(INTDIR)\XZip.obj" \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\DialogEvalDets.obj" \
	"$(INTDIR)\DlgClassItem.obj" \
	"$(INTDIR)\DlgCompile.obj" \
	"$(INTDIR)\DlgCpxPrimnames.obj" \
	"$(INTDIR)\DlgEditor2D.obj" \
	"$(INTDIR)\DlgGs.obj" \
	"$(INTDIR)\DlgMotionState.obj" \
	"$(INTDIR)\DlgMurrChs.obj" \
	"$(INTDIR)\DlgNewBrush.obj" \
	"$(INTDIR)\DlgNewItem.obj" \
	"$(INTDIR)\DlgNewPref.obj" \
	"$(INTDIR)\DlgNewTerrain.obj" \
	"$(INTDIR)\DlgPreferences.obj" \
	"$(INTDIR)\DlgTexMody.obj" \
	"$(INTDIR)\DlgTexRefs.obj" \
	"$(INTDIR)\BaseItemsDlgs.obj" \
	"$(INTDIR)\DlgBar.obj" \
	"$(INTDIR)\DlgBarLevExpl.obj" \
	"$(INTDIR)\DlgBrshProp.obj" \
	"$(INTDIR)\DlgCompProgress.obj" \
	"$(INTDIR)\DlgFaceProp.obj" \
	"$(INTDIR)\DlgGameItem.obj" \
	"$(INTDIR)\DlgItems.obj" \
	"$(INTDIR)\DlgLightItem.obj" \
	"$(INTDIR)\DlgScene.obj" \
	"$(INTDIR)\DlgScripts.obj" \
	"$(INTDIR)\DlgSounds.obj" \
	"$(INTDIR)\DlgTextures.obj" \
	"$(INTDIR)\DlgTriggerItem.obj" \
	"$(INTDIR)\ProgressDlg.obj" \
	"$(INTDIR)\TabDlgSel.obj" \
	"$(INTDIR)\brush.obj" \
	"$(INTDIR)\CustItemCat.obj" \
	"$(INTDIR)\GameItem.obj" \
	"$(INTDIR)\LightBulb.obj" \
	"$(INTDIR)\Motion.obj" \
	"$(INTDIR)\Poly.obj" \
	"$(INTDIR)\SceItem.obj" \
	"$(INTDIR)\Scene.obj" \
	"$(INTDIR)\ScriptItem.obj" \
	"$(INTDIR)\SkyDom.obj" \
	"$(INTDIR)\SplinePath.obj" \
	"$(INTDIR)\TriggerItem.obj" \
	"$(INTDIR)\Zone.obj" \
	"$(INTDIR)\z_ed2View.obj" \
	"$(INTDIR)\z_ed3View.obj" \
	"$(INTDIR)\z_ed2Doc.obj" \
	"$(INTDIR)\z_ed2DocAlign.obj" \
	"$(INTDIR)\z_ed2DocDraw.obj" \
	"$(INTDIR)\z_ed2DocMk.obj" \
	"$(INTDIR)\z_ed2DocProc.obj" \
	"$(INTDIR)\z_ed2DocSave.obj" \
	"$(INTDIR)\z_ed2DocSave_Old.obj" \
	"$(INTDIR)\Scripts.obj" \
	"$(INTDIR)\CCrystalEditView.obj" \
	"$(INTDIR)\CCrystalTextBuffer.obj" \
	"$(INTDIR)\CCrystalTextView.obj" \
	"$(INTDIR)\CCrystalTextView2.obj" \
	"$(INTDIR)\CEditReplaceDlg.obj" \
	"$(INTDIR)\CFindTextDlg.obj" \
	"$(INTDIR)\CompilerOutDlg.obj" \
	"$(INTDIR)\cplusplus.obj" \
	"$(INTDIR)\CppCompiler.obj" \
	"$(INTDIR)\DlgBarScript.obj" \
	"$(INTDIR)\DlgCompilerPath.obj" \
	"$(INTDIR)\SampleDoc.obj" \
	"$(INTDIR)\SampleView.obj" \
	"$(INTDIR)\ToolFrame.obj" \
	"$(INTDIR)\rfHillTerrain.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyStatusBar.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UiEnhancer.obj" \
	"$(INTDIR)\z-edmapkey.obj" \
	"$(INTDIR)\z_edmap.obj" \
	"$(INTDIR)\z_edmap.res"

"..\_bin\geticd.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"

OUTDIR=.\z_edmap___Win32_ReleaseFree
INTDIR=.\z_edmap___Win32_ReleaseFree
# Begin Custom Macros
OutDir=.\z_edmap___Win32_ReleaseFree
# End Custom Macros

ALL : "..\_bin\getic_free.exe" "$(OUTDIR)\z_edmap.pch"


CLEAN :
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\BaseItemsDlgs.obj"
	-@erase "$(INTDIR)\brush.obj"
	-@erase "$(INTDIR)\BspTree.obj"
	-@erase "$(INTDIR)\bTbarCombo.obj"
	-@erase "$(INTDIR)\BzipCompressor.obj"
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\CCrystalEditView.obj"
	-@erase "$(INTDIR)\CCrystalTextBuffer.obj"
	-@erase "$(INTDIR)\CCrystalTextView.obj"
	-@erase "$(INTDIR)\CCrystalTextView2.obj"
	-@erase "$(INTDIR)\CEditReplaceDlg.obj"
	-@erase "$(INTDIR)\CFindTextDlg.obj"
	-@erase "$(INTDIR)\ColorButon.obj"
	-@erase "$(INTDIR)\Compiler.obj"
	-@erase "$(INTDIR)\CompilerOutDlg.obj"
	-@erase "$(INTDIR)\CompilerStream.obj"
	-@erase "$(INTDIR)\CoolDialogBar.obj"
	-@erase "$(INTDIR)\cplusplus.obj"
	-@erase "$(INTDIR)\CppCompiler.obj"
	-@erase "$(INTDIR)\Crc.obj"
	-@erase "$(INTDIR)\Curve_Class.obj"
	-@erase "$(INTDIR)\CustItemCat.obj"
	-@erase "$(INTDIR)\DialogEvalDets.obj"
	-@erase "$(INTDIR)\DirTree.obj"
	-@erase "$(INTDIR)\DlgBar.obj"
	-@erase "$(INTDIR)\DlgBarLevExpl.obj"
	-@erase "$(INTDIR)\DlgBarScript.obj"
	-@erase "$(INTDIR)\DlgBrshProp.obj"
	-@erase "$(INTDIR)\DlgClassItem.obj"
	-@erase "$(INTDIR)\DlgCompile.obj"
	-@erase "$(INTDIR)\DlgCompilerPath.obj"
	-@erase "$(INTDIR)\DlgCompProgress.obj"
	-@erase "$(INTDIR)\DlgConsole.obj"
	-@erase "$(INTDIR)\DlgCpxPrimnames.obj"
	-@erase "$(INTDIR)\DlgEditor2D.obj"
	-@erase "$(INTDIR)\DlgFaceProp.obj"
	-@erase "$(INTDIR)\DlgGameItem.obj"
	-@erase "$(INTDIR)\DlgGs.obj"
	-@erase "$(INTDIR)\DlgItems.obj"
	-@erase "$(INTDIR)\DlgLightItem.obj"
	-@erase "$(INTDIR)\DlgMotionState.obj"
	-@erase "$(INTDIR)\DlgMurrChs.obj"
	-@erase "$(INTDIR)\DlgNewBrush.obj"
	-@erase "$(INTDIR)\DlgNewItem.obj"
	-@erase "$(INTDIR)\DlgNewPref.obj"
	-@erase "$(INTDIR)\DlgNewTerrain.obj"
	-@erase "$(INTDIR)\DlgPreferences.obj"
	-@erase "$(INTDIR)\DlgScene.obj"
	-@erase "$(INTDIR)\DlgScripts.obj"
	-@erase "$(INTDIR)\DlgSounds.obj"
	-@erase "$(INTDIR)\DlgTexMody.obj"
	-@erase "$(INTDIR)\DlgTexRefs.obj"
	-@erase "$(INTDIR)\DlgTextures.obj"
	-@erase "$(INTDIR)\DlgTriggerItem.obj"
	-@erase "$(INTDIR)\Drawout1.obj"
	-@erase "$(INTDIR)\FixSplitter.obj"
	-@erase "$(INTDIR)\Frustrum.obj"
	-@erase "$(INTDIR)\GameItem.obj"
	-@erase "$(INTDIR)\GeoUtils.obj"
	-@erase "$(INTDIR)\GListCtrl.obj"
	-@erase "$(INTDIR)\LbEditable.obj"
	-@erase "$(INTDIR)\LboxItem.obj"
	-@erase "$(INTDIR)\LightBulb.obj"
	-@erase "$(INTDIR)\LMProc.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MaskEdit.obj"
	-@erase "$(INTDIR)\MiniBsp.obj"
	-@erase "$(INTDIR)\Motion.obj"
	-@erase "$(INTDIR)\MyStatusBar.obj"
	-@erase "$(INTDIR)\Poly.obj"
	-@erase "$(INTDIR)\PortalMerge.obj"
	-@erase "$(INTDIR)\PortalPRC.obj"
	-@erase "$(INTDIR)\ProgressDlg.obj"
	-@erase "$(INTDIR)\PrpLb.obj"
	-@erase "$(INTDIR)\PVSPrc.obj"
	-@erase "$(INTDIR)\RepLBox.obj"
	-@erase "$(INTDIR)\rfHillTerrain.obj"
	-@erase "$(INTDIR)\SampleDoc.obj"
	-@erase "$(INTDIR)\SampleView.obj"
	-@erase "$(INTDIR)\SceItem.obj"
	-@erase "$(INTDIR)\Scene.obj"
	-@erase "$(INTDIR)\ScriptItem.obj"
	-@erase "$(INTDIR)\Scripts.obj"
	-@erase "$(INTDIR)\SkyDom.obj"
	-@erase "$(INTDIR)\SplinePath.obj"
	-@erase "$(INTDIR)\StaticGL.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TabDlgSel.obj"
	-@erase "$(INTDIR)\TexAdapter.obj"
	-@erase "$(INTDIR)\TexLbox.obj"
	-@erase "$(INTDIR)\TexRef.obj"
	-@erase "$(INTDIR)\texsys.obj"
	-@erase "$(INTDIR)\ToolFrame.obj"
	-@erase "$(INTDIR)\TreeScene.obj"
	-@erase "$(INTDIR)\TriggerItem.obj"
	-@erase "$(INTDIR)\UiEnhancer.obj"
	-@erase "$(INTDIR)\Undo.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VertDlgDlg.obj"
	-@erase "$(INTDIR)\XUnzip.obj"
	-@erase "$(INTDIR)\XZip.obj"
	-@erase "$(INTDIR)\z-edmapkey.obj"
	-@erase "$(INTDIR)\z_ed2Doc.obj"
	-@erase "$(INTDIR)\z_ed2DocAlign.obj"
	-@erase "$(INTDIR)\z_ed2DocDraw.obj"
	-@erase "$(INTDIR)\z_ed2DocMk.obj"
	-@erase "$(INTDIR)\z_ed2DocProc.obj"
	-@erase "$(INTDIR)\z_ed2DocSave.obj"
	-@erase "$(INTDIR)\z_ed2DocSave_Old.obj"
	-@erase "$(INTDIR)\z_ed2View.obj"
	-@erase "$(INTDIR)\z_ed3View.obj"
	-@erase "$(INTDIR)\z_edmap.obj"
	-@erase "$(INTDIR)\z_edmap.pch"
	-@erase "$(INTDIR)\z_edmap.res"
	-@erase "$(INTDIR)\Zone.obj"
	-@erase "..\_bin\getic_free.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /Od /Ob2 /I "..\z-utils" /I "..\z-engine\\" /I "..\baselib\\" /I "..\baselib" /I "..\_include" /I "..\BzLib\\" /D "NDEBUG" /D "_FREE" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\z_edmap.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\z_edmap.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\getic_free.pdb" /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrt.lib" /out:"..\_bin\getic_free.exe" /libpath:"..\lib\\" 
LINK32_OBJS= \
	"$(INTDIR)\BspTree.obj" \
	"$(INTDIR)\Compiler.obj" \
	"$(INTDIR)\CompilerStream.obj" \
	"$(INTDIR)\LMProc.obj" \
	"$(INTDIR)\MiniBsp.obj" \
	"$(INTDIR)\PortalMerge.obj" \
	"$(INTDIR)\PortalPRC.obj" \
	"$(INTDIR)\PVSPrc.obj" \
	"$(INTDIR)\bTbarCombo.obj" \
	"$(INTDIR)\BzipCompressor.obj" \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\ColorButon.obj" \
	"$(INTDIR)\CoolDialogBar.obj" \
	"$(INTDIR)\Crc.obj" \
	"$(INTDIR)\Curve_Class.obj" \
	"$(INTDIR)\DirTree.obj" \
	"$(INTDIR)\DlgConsole.obj" \
	"$(INTDIR)\Drawout1.obj" \
	"$(INTDIR)\FixSplitter.obj" \
	"$(INTDIR)\Frustrum.obj" \
	"$(INTDIR)\GeoUtils.obj" \
	"$(INTDIR)\GListCtrl.obj" \
	"$(INTDIR)\LbEditable.obj" \
	"$(INTDIR)\LboxItem.obj" \
	"$(INTDIR)\MaskEdit.obj" \
	"$(INTDIR)\PrpLb.obj" \
	"$(INTDIR)\RepLBox.obj" \
	"$(INTDIR)\StaticGL.obj" \
	"$(INTDIR)\TexAdapter.obj" \
	"$(INTDIR)\TexLbox.obj" \
	"$(INTDIR)\TexRef.obj" \
	"$(INTDIR)\texsys.obj" \
	"$(INTDIR)\TreeScene.obj" \
	"$(INTDIR)\Undo.obj" \
	"$(INTDIR)\VertDlgDlg.obj" \
	"$(INTDIR)\XUnzip.obj" \
	"$(INTDIR)\XZip.obj" \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\DialogEvalDets.obj" \
	"$(INTDIR)\DlgClassItem.obj" \
	"$(INTDIR)\DlgCompile.obj" \
	"$(INTDIR)\DlgCpxPrimnames.obj" \
	"$(INTDIR)\DlgEditor2D.obj" \
	"$(INTDIR)\DlgGs.obj" \
	"$(INTDIR)\DlgMotionState.obj" \
	"$(INTDIR)\DlgMurrChs.obj" \
	"$(INTDIR)\DlgNewBrush.obj" \
	"$(INTDIR)\DlgNewItem.obj" \
	"$(INTDIR)\DlgNewPref.obj" \
	"$(INTDIR)\DlgNewTerrain.obj" \
	"$(INTDIR)\DlgPreferences.obj" \
	"$(INTDIR)\DlgTexMody.obj" \
	"$(INTDIR)\DlgTexRefs.obj" \
	"$(INTDIR)\BaseItemsDlgs.obj" \
	"$(INTDIR)\DlgBar.obj" \
	"$(INTDIR)\DlgBarLevExpl.obj" \
	"$(INTDIR)\DlgBrshProp.obj" \
	"$(INTDIR)\DlgCompProgress.obj" \
	"$(INTDIR)\DlgFaceProp.obj" \
	"$(INTDIR)\DlgGameItem.obj" \
	"$(INTDIR)\DlgItems.obj" \
	"$(INTDIR)\DlgLightItem.obj" \
	"$(INTDIR)\DlgScene.obj" \
	"$(INTDIR)\DlgScripts.obj" \
	"$(INTDIR)\DlgSounds.obj" \
	"$(INTDIR)\DlgTextures.obj" \
	"$(INTDIR)\DlgTriggerItem.obj" \
	"$(INTDIR)\ProgressDlg.obj" \
	"$(INTDIR)\TabDlgSel.obj" \
	"$(INTDIR)\brush.obj" \
	"$(INTDIR)\CustItemCat.obj" \
	"$(INTDIR)\GameItem.obj" \
	"$(INTDIR)\LightBulb.obj" \
	"$(INTDIR)\Motion.obj" \
	"$(INTDIR)\Poly.obj" \
	"$(INTDIR)\SceItem.obj" \
	"$(INTDIR)\Scene.obj" \
	"$(INTDIR)\ScriptItem.obj" \
	"$(INTDIR)\SkyDom.obj" \
	"$(INTDIR)\SplinePath.obj" \
	"$(INTDIR)\TriggerItem.obj" \
	"$(INTDIR)\Zone.obj" \
	"$(INTDIR)\z_ed2View.obj" \
	"$(INTDIR)\z_ed3View.obj" \
	"$(INTDIR)\z_ed2Doc.obj" \
	"$(INTDIR)\z_ed2DocAlign.obj" \
	"$(INTDIR)\z_ed2DocDraw.obj" \
	"$(INTDIR)\z_ed2DocMk.obj" \
	"$(INTDIR)\z_ed2DocProc.obj" \
	"$(INTDIR)\z_ed2DocSave.obj" \
	"$(INTDIR)\z_ed2DocSave_Old.obj" \
	"$(INTDIR)\Scripts.obj" \
	"$(INTDIR)\CCrystalEditView.obj" \
	"$(INTDIR)\CCrystalTextBuffer.obj" \
	"$(INTDIR)\CCrystalTextView.obj" \
	"$(INTDIR)\CCrystalTextView2.obj" \
	"$(INTDIR)\CEditReplaceDlg.obj" \
	"$(INTDIR)\CFindTextDlg.obj" \
	"$(INTDIR)\CompilerOutDlg.obj" \
	"$(INTDIR)\cplusplus.obj" \
	"$(INTDIR)\CppCompiler.obj" \
	"$(INTDIR)\DlgBarScript.obj" \
	"$(INTDIR)\DlgCompilerPath.obj" \
	"$(INTDIR)\SampleDoc.obj" \
	"$(INTDIR)\SampleView.obj" \
	"$(INTDIR)\ToolFrame.obj" \
	"$(INTDIR)\rfHillTerrain.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MyStatusBar.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\UiEnhancer.obj" \
	"$(INTDIR)\z-edmapkey.obj" \
	"$(INTDIR)\z_edmap.obj" \
	"$(INTDIR)\z_edmap.res"

"..\_bin\getic_free.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("z_edmap.dep")
!INCLUDE "z_edmap.dep"
!ELSE 
!MESSAGE Warning: cannot find "z_edmap.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "z_edmap - Win32 Release" || "$(CFG)" == "z_edmap - Win32 Debug" || "$(CFG)" == "z_edmap - Win32 ReleaseFree"
SOURCE=.\BspTree.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\BspTree.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\BspTree.obj"	"$(INTDIR)\BspTree.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\BspTree.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Compiler.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Compiler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Compiler.obj"	"$(INTDIR)\Compiler.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Compiler.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CompilerStream.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CompilerStream.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CompilerStream.obj"	"$(INTDIR)\CompilerStream.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CompilerStream.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LMProc.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\LMProc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\LMProc.obj"	"$(INTDIR)\LMProc.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\LMProc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MiniBsp.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\MiniBsp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\MiniBsp.obj"	"$(INTDIR)\MiniBsp.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\MiniBsp.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PortalMerge.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\PortalMerge.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\PortalMerge.obj"	"$(INTDIR)\PortalMerge.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\PortalMerge.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PortalPRC.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\PortalPRC.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\PortalPRC.obj"	"$(INTDIR)\PortalPRC.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\PortalPRC.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PVSPrc.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\PVSPrc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\PVSPrc.obj"	"$(INTDIR)\PVSPrc.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\PVSPrc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bTbarCombo.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\bTbarCombo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\bTbarCombo.obj"	"$(INTDIR)\bTbarCombo.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\bTbarCombo.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\BzipCompressor.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\BzipCompressor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\BzipCompressor.obj"	"$(INTDIR)\BzipCompressor.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\BzipCompressor.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\camera.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\camera.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\camera.obj"	"$(INTDIR)\camera.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\camera.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ColorButon.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\ColorButon.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\ColorButon.obj"	"$(INTDIR)\ColorButon.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\ColorButon.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CoolDialogBar.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CoolDialogBar.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CoolDialogBar.obj"	"$(INTDIR)\CoolDialogBar.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CoolDialogBar.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Crc.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Crc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Crc.obj"	"$(INTDIR)\Crc.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Crc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Curve_Class.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Curve_Class.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Curve_Class.obj"	"$(INTDIR)\Curve_Class.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Curve_Class.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DirTree.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DirTree.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DirTree.obj"	"$(INTDIR)\DirTree.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DirTree.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgConsole.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgConsole.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgConsole.obj"	"$(INTDIR)\DlgConsole.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgConsole.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Drawout1.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Drawout1.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Drawout1.obj"	"$(INTDIR)\Drawout1.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Drawout1.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FixSplitter.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\FixSplitter.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\FixSplitter.obj"	"$(INTDIR)\FixSplitter.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\FixSplitter.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Frustrum.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Frustrum.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Frustrum.obj"	"$(INTDIR)\Frustrum.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Frustrum.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GeoUtils.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\GeoUtils.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\GeoUtils.obj"	"$(INTDIR)\GeoUtils.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\GeoUtils.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GListCtrl.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\GListCtrl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\GListCtrl.obj"	"$(INTDIR)\GListCtrl.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\GListCtrl.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LbEditable.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\LbEditable.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\LbEditable.obj"	"$(INTDIR)\LbEditable.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\LbEditable.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LboxItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\LboxItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\LboxItem.obj"	"$(INTDIR)\LboxItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\LboxItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MaskEdit.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\MaskEdit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\MaskEdit.obj"	"$(INTDIR)\MaskEdit.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\MaskEdit.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PrpLb.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\PrpLb.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\PrpLb.obj"	"$(INTDIR)\PrpLb.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\PrpLb.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RepLBox.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\RepLBox.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\RepLBox.obj"	"$(INTDIR)\RepLBox.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\RepLBox.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StaticGL.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\StaticGL.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\StaticGL.obj"	"$(INTDIR)\StaticGL.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\StaticGL.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TexAdapter.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\TexAdapter.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\TexAdapter.obj"	"$(INTDIR)\TexAdapter.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\TexAdapter.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TexLbox.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\TexLbox.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\TexLbox.obj"	"$(INTDIR)\TexLbox.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\TexLbox.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TexRef.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\TexRef.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\TexRef.obj"	"$(INTDIR)\TexRef.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\TexRef.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\texsys.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\texsys.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\texsys.obj"	"$(INTDIR)\texsys.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\texsys.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TreeScene.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\TreeScene.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\TreeScene.obj"	"$(INTDIR)\TreeScene.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\TreeScene.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Undo.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Undo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Undo.obj"	"$(INTDIR)\Undo.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Undo.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\VertDlgDlg.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\VertDlgDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\VertDlgDlg.obj"	"$(INTDIR)\VertDlgDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\VertDlgDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\zcompress\XUnzip.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\XUnzip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\XUnzip.obj"	"$(INTDIR)\XUnzip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\XUnzip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\zcompress\XZip.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\XZip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\XZip.obj"	"$(INTDIR)\XZip.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\XZip.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\AboutDlg.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\AboutDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\AboutDlg.obj"	"$(INTDIR)\AboutDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\AboutDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DialogEvalDets.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DialogEvalDets.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DialogEvalDets.obj"	"$(INTDIR)\DialogEvalDets.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DialogEvalDets.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgClassItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgClassItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgClassItem.obj"	"$(INTDIR)\DlgClassItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgClassItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgCompile.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgCompile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgCompile.obj"	"$(INTDIR)\DlgCompile.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgCompile.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgCpxPrimnames.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgCpxPrimnames.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgCpxPrimnames.obj"	"$(INTDIR)\DlgCpxPrimnames.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgCpxPrimnames.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgEditor2D.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgEditor2D.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgEditor2D.obj"	"$(INTDIR)\DlgEditor2D.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgEditor2D.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgGs.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgGs.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgGs.obj"	"$(INTDIR)\DlgGs.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgGs.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgMotionState.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgMotionState.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgMotionState.obj"	"$(INTDIR)\DlgMotionState.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgMotionState.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgMurrChs.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgMurrChs.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgMurrChs.obj"	"$(INTDIR)\DlgMurrChs.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgMurrChs.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgNewBrush.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgNewBrush.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgNewBrush.obj"	"$(INTDIR)\DlgNewBrush.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgNewBrush.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgNewItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgNewItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgNewItem.obj"	"$(INTDIR)\DlgNewItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgNewItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgNewPref.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgNewPref.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgNewPref.obj"	"$(INTDIR)\DlgNewPref.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgNewPref.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgNewTerrain.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgNewTerrain.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgNewTerrain.obj"	"$(INTDIR)\DlgNewTerrain.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgNewTerrain.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgPreferences.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgPreferences.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgPreferences.obj"	"$(INTDIR)\DlgPreferences.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgPreferences.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgTexMody.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgTexMody.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgTexMody.obj"	"$(INTDIR)\DlgTexMody.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgTexMody.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgTexRefs.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgTexRefs.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgTexRefs.obj"	"$(INTDIR)\DlgTexRefs.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgTexRefs.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\BaseItemsDlgs.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\BaseItemsDlgs.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\BaseItemsDlgs.obj"	"$(INTDIR)\BaseItemsDlgs.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\BaseItemsDlgs.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgBar.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgBar.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgBar.obj"	"$(INTDIR)\DlgBar.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgBar.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgBarLevExpl.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgBarLevExpl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgBarLevExpl.obj"	"$(INTDIR)\DlgBarLevExpl.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgBarLevExpl.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgBrshProp.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgBrshProp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgBrshProp.obj"	"$(INTDIR)\DlgBrshProp.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgBrshProp.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgCompProgress.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgCompProgress.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgCompProgress.obj"	"$(INTDIR)\DlgCompProgress.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgCompProgress.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgFaceProp.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgFaceProp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgFaceProp.obj"	"$(INTDIR)\DlgFaceProp.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgFaceProp.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgGameItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgGameItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgGameItem.obj"	"$(INTDIR)\DlgGameItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgGameItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgItems.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgItems.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgItems.obj"	"$(INTDIR)\DlgItems.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgItems.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgLightItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgLightItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgLightItem.obj"	"$(INTDIR)\DlgLightItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgLightItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgScene.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgScene.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgScene.obj"	"$(INTDIR)\DlgScene.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgScene.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgScripts.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgScripts.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgScripts.obj"	"$(INTDIR)\DlgScripts.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgScripts.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgSounds.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgSounds.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgSounds.obj"	"$(INTDIR)\DlgSounds.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgSounds.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgTextures.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgTextures.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgTextures.obj"	"$(INTDIR)\DlgTextures.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgTextures.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgTriggerItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgTriggerItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgTriggerItem.obj"	"$(INTDIR)\DlgTriggerItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgTriggerItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ProgressDlg.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\ProgressDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\ProgressDlg.obj"	"$(INTDIR)\ProgressDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\ProgressDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TabDlgSel.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\TabDlgSel.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\TabDlgSel.obj"	"$(INTDIR)\TabDlgSel.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\TabDlgSel.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\brush.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\brush.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\brush.obj"	"$(INTDIR)\brush.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\brush.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CustItemCat.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CustItemCat.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CustItemCat.obj"	"$(INTDIR)\CustItemCat.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CustItemCat.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GameItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\GameItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\GameItem.obj"	"$(INTDIR)\GameItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\GameItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LightBulb.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\LightBulb.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\LightBulb.obj"	"$(INTDIR)\LightBulb.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\LightBulb.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Motion.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Motion.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Motion.obj"	"$(INTDIR)\Motion.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Motion.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Poly.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Poly.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Poly.obj"	"$(INTDIR)\Poly.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Poly.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SceItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\SceItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\SceItem.obj"	"$(INTDIR)\SceItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\SceItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Scene.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Scene.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Scene.obj"	"$(INTDIR)\Scene.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Scene.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ScriptItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\ScriptItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\ScriptItem.obj"	"$(INTDIR)\ScriptItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\ScriptItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SkyDom.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\SkyDom.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\SkyDom.obj"	"$(INTDIR)\SkyDom.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\SkyDom.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SplinePath.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\SplinePath.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\SplinePath.obj"	"$(INTDIR)\SplinePath.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\SplinePath.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TriggerItem.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\TriggerItem.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\TriggerItem.obj"	"$(INTDIR)\TriggerItem.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\TriggerItem.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Zone.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Zone.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Zone.obj"	"$(INTDIR)\Zone.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Zone.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2View.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2View.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2View.obj"	"$(INTDIR)\z_ed2View.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2View.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed3View.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed3View.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed3View.obj"	"$(INTDIR)\z_ed3View.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed3View.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2Doc.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2Doc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2Doc.obj"	"$(INTDIR)\z_ed2Doc.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2Doc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2DocAlign.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2DocAlign.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2DocAlign.obj"	"$(INTDIR)\z_ed2DocAlign.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2DocAlign.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2DocDraw.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2DocDraw.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2DocDraw.obj"	"$(INTDIR)\z_ed2DocDraw.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2DocDraw.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2DocMk.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2DocMk.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2DocMk.obj"	"$(INTDIR)\z_ed2DocMk.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2DocMk.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2DocProc.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2DocProc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2DocProc.obj"	"$(INTDIR)\z_ed2DocProc.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2DocProc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2DocSave.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2DocSave.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2DocSave.obj"	"$(INTDIR)\z_ed2DocSave.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2DocSave.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\z_ed2DocSave_Old.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_ed2DocSave_Old.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_ed2DocSave_Old.obj"	"$(INTDIR)\z_ed2DocSave_Old.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_ed2DocSave_Old.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Scripts.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\Scripts.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\Scripts.obj"	"$(INTDIR)\Scripts.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\Scripts.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CCrystalEditView.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CCrystalEditView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CCrystalEditView.obj"	"$(INTDIR)\CCrystalEditView.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CCrystalEditView.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CCrystalTextBuffer.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CCrystalTextBuffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CCrystalTextBuffer.obj"	"$(INTDIR)\CCrystalTextBuffer.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CCrystalTextBuffer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CCrystalTextView.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CCrystalTextView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CCrystalTextView.obj"	"$(INTDIR)\CCrystalTextView.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CCrystalTextView.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CCrystalTextView2.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CCrystalTextView2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CCrystalTextView2.obj"	"$(INTDIR)\CCrystalTextView2.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CCrystalTextView2.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CEditReplaceDlg.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CEditReplaceDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CEditReplaceDlg.obj"	"$(INTDIR)\CEditReplaceDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CEditReplaceDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CFindTextDlg.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CFindTextDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CFindTextDlg.obj"	"$(INTDIR)\CFindTextDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CFindTextDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CompilerOutDlg.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CompilerOutDlg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CompilerOutDlg.obj"	"$(INTDIR)\CompilerOutDlg.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CompilerOutDlg.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\cplusplus.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\cplusplus.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\cplusplus.obj"	"$(INTDIR)\cplusplus.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\cplusplus.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CppCompiler.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\CppCompiler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\CppCompiler.obj"	"$(INTDIR)\CppCompiler.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\CppCompiler.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgBarScript.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgBarScript.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgBarScript.obj"	"$(INTDIR)\DlgBarScript.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgBarScript.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\DlgCompilerPath.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\DlgCompilerPath.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\DlgCompilerPath.obj"	"$(INTDIR)\DlgCompilerPath.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\DlgCompilerPath.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SampleDoc.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\SampleDoc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\SampleDoc.obj"	"$(INTDIR)\SampleDoc.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\SampleDoc.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SampleView.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\SampleView.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\SampleView.obj"	"$(INTDIR)\SampleView.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\SampleView.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ToolFrame.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\ToolFrame.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\ToolFrame.obj"	"$(INTDIR)\ToolFrame.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\ToolFrame.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rfHillTerrain.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\rfHillTerrain.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\rfHillTerrain.obj"	"$(INTDIR)\rfHillTerrain.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\rfHillTerrain.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MyStatusBar.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\MyStatusBar.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\MyStatusBar.obj"	"$(INTDIR)\MyStatusBar.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\MyStatusBar.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Od /Ob2 /I "..\z-utils" /I "..\z-engine\\" /I "..\baselib\\" /I "..\baselib" /I "..\_include" /I "..\BzLib\\" /D "NDEBUG" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /Fp"$(INTDIR)\z_edmap.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\z_edmap.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\baselib" /I "..\BzLib\\" /I "..\_include" /D "_DEBUG" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\z_edmap.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\z_edmap.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"

CPP_SWITCHES=/nologo /MT /W3 /GX /Od /Ob2 /I "..\z-utils" /I "..\z-engine\\" /I "..\baselib\\" /I "..\baselib" /I "..\_include" /I "..\BzLib\\" /D "NDEBUG" /D "_FREE" /D "_SKINNY" /D "_WINDOWS" /D "_BAR_DLGPANE" /D "WIN32" /D "_MBCS" /D "_CFG_WIN32_WIN" /Fp"$(INTDIR)\z_edmap.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\z_edmap.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\UiEnhancer.cpp

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\UiEnhancer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\UiEnhancer.obj"	"$(INTDIR)\UiEnhancer.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\UiEnhancer.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\z-edmapkey.cpp"

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z-edmapkey.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z-edmapkey.obj"	"$(INTDIR)\z-edmapkey.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z-edmapkey.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\z_edmap.cpp"

!IF  "$(CFG)" == "z_edmap - Win32 Release"


"$(INTDIR)\z_edmap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 Debug"


"$(INTDIR)\z_edmap.obj"	"$(INTDIR)\z_edmap.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "z_edmap - Win32 ReleaseFree"


"$(INTDIR)\z_edmap.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=".\z_edmap.rc"

"$(INTDIR)\z_edmap.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

