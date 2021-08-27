#-------------------------------------------------
#
# Project created by QtCreator 2020-09-04T15:36:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qmfcgetic
TEMPLATE = app
DEFINES += _CFG_GNU_LINUX _MFC_LINUX GNU_LINUX
INCLUDEPATH += ../../MFC
INCLUDEPATH += ../../_include
INCLUDEPATH += ../../baselib
INCLUDEPATH += ../../baselib/GNU_LINUX

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    ../../MFC/mfc.cpp \
    ../../MFC/stdafx.cpp \
    ../../baselib/baselib.cpp \
    ../../baselib/sock.cpp \
    ../../getic/Aboutdlg.cpp \
    ../../getic/Bitarray.cpp \
    ../../getic/Boxitems.cpp \
    ../../getic/BspTree.cpp \
    ../../getic/CCrystalEditView.cpp \
    ../../getic/CCrystalTextBuffer.cpp \
    ../../getic/CCrystalTextView.cpp \
    ../../getic/CCrystalTextView2.cpp \
    ../../getic/CEditReplaceDlg.cpp \
    ../../getic/CFindTextDlg.cpp \
    ../../getic/ColorButon.cpp \
    ../../getic/Compiler.cpp \
    ../../getic/CompilerOutDlg.cpp \
    ../../getic/CompilerStream.cpp \
    ../../getic/CoolDialogBar.cpp \
    ../../getic/CppCompiler.cpp \
    ../../getic/Crc.cpp \
    ../../getic/Curve_Class.cpp \
    ../../getic/CustItemCat.cpp \
    ../../getic/D2view.cpp \
    ../../getic/DialogEvalDets.cpp \
    ../../getic/Dirtree.cpp \
    ../../getic/Dlbpb.cpp \
    ../../getic/DlgAssoc.cpp \
    ../../getic/DlgBSetTop.cpp \
    ../../getic/DlgBar.cpp \
    ../../getic/DlgBarLevExpl.cpp \
    ../../getic/DlgBarScript.cpp \
    ../../getic/DlgBigTerrain.cpp \
    ../../getic/DlgBrSet1.cpp \
    ../../getic/DlgBrSet2.cpp \
    ../../getic/DlgBrshProp.cpp \
    ../../getic/DlgClassItem.cpp \
    ../../getic/DlgCompProgress.cpp \
    ../../getic/DlgCompile.cpp \
    ../../getic/DlgCompilerPath.cpp \
    ../../getic/DlgConsole.cpp \
    ../../getic/DlgCpxPrimnames.cpp \
    ../../getic/DlgCustItem.cpp \
    ../../getic/DlgEditor2D.cpp \
    ../../getic/DlgFaceProp.cpp \
    ../../getic/DlgGameItem.cpp \
    ../../getic/DlgGs.cpp \
    ../../getic/DlgLightItem.cpp \
    ../../getic/DlgMaterial.cpp \
    ../../getic/DlgMirronChose.cpp \
    ../../getic/DlgMotionState.cpp \
    ../../getic/DlgMurrChs.cpp \
    ../../getic/DlgNewBrush.cpp \
    ../../getic/DlgNewItem.cpp \
    ../../getic/DlgNewPref.cpp \
    ../../getic/DlgNewProj.cpp \
    ../../getic/DlgNewTerrain.cpp \
    ../../getic/DlgPreferences.cpp \
    ../../getic/DlgResBrowser.cpp \
    ../../getic/DlgScene.cpp \
    ../../getic/DlgScripts.cpp \
    ../../getic/DlgSoundItem.cpp \
    ../../getic/DlgSounds.cpp \
    ../../getic/DlgStartPlayItem.cpp \
    ../../getic/DlgTexRefs.cpp \
    ../../getic/DlgTextures.cpp \
    ../../getic/DlgTriggerItem.cpp \
    ../../getic/DlgTriggers.cpp \
    ../../getic/DlgUnlock.cpp \
    ../../getic/Dlgitems.cpp \
    ../../getic/Dlgleafs.cpp \
    ../../getic/Drawout.cpp \
    ../../getic/Drawout1.cpp \
    ../../getic/EditKillFocus.cpp \
    ../../getic/Esrprc.cpp \
    ../../getic/ExceptionLog.cpp \
    ../../getic/FixSplitter.cpp \
    ../../getic/Flowpath.cpp \
    ../../getic/Frame2.cpp \
    ../../getic/FrontView.cpp \
    ../../getic/Frustrum.cpp \
    ../../getic/GListCtrl.cpp \
    ../../getic/GameItem.cpp \
    ../../getic/GeoUtils.cpp \
    ../../getic/GeticProject.cpp \
    ../../getic/KCSubdivision.cpp \
    ../../getic/LMProc.cpp \
    ../../getic/LbEditable.cpp \
    ../../getic/Lboxitem.cpp \
    ../../getic/LightBulb.cpp \
    ../../getic/Lmproc2.cpp \
    ../../getic/MainFrm.cpp \
    ../../getic/Maskedit.cpp \
    ../../getic/MiniBsp.cpp \
    ../../getic/Motion.cpp \
    ../../getic/MyStatusBar.cpp \
    ../../getic/Oneitem.cpp \
    ../../getic/PVSPrc.cpp \
    ../../getic/Poly.cpp \
    ../../getic/PortalMerge.cpp \
    ../../getic/PortalPRC.cpp \
    ../../getic/PortalPRC1.cpp \
    ../../getic/ProgressDlg.cpp \
    ../../getic/PrpListCtrl.cpp \
    ../../getic/Prplb.cpp \
    ../../getic/Replbox.cpp \
    ../../getic/SSCarrier.cpp \
    ../../getic/SampleDoc.cpp \
    ../../getic/SampleView.cpp \
    ../../getic/SceItem.cpp \
    ../../getic/Scene.cpp \
    ../../getic/ScriptEditView.cpp \
    ../../getic/ScriptItem.cpp \
    ../../getic/Scripts.cpp \
    ../../getic/Sgsys.cpp \
    ../../getic/Sgsysold.cpp \
    ../../getic/Sideview.cpp \
    ../../getic/Skydom.cpp \
    ../../getic/SoundCarrier.cpp \
    ../../getic/SoundItem.cpp \
    ../../getic/SplinePath.cpp \
    ../../getic/StartPlayItem.cpp \
    ../../getic/StaticGL.cpp \
    ../../getic/StdAfx.cpp \
    ../../getic/Subdivider.cpp \
    ../../getic/TabDlgSel.cpp \
    ../../getic/TerTree.cpp \
    ../../getic/TexAdapter.cpp \
    ../../getic/Texlbox.cpp \
    ../../getic/Texref.cpp \
    ../../getic/ToolFrame.cpp \
    ../../getic/TreeScene.cpp \
    ../../getic/TriggerItem.cpp \
    ../../getic/UiEnhancer.cpp \
    ../../getic/Undo.cpp \
    ../../getic/VertDlgDlg.cpp \
    ../../getic/Vmath.cpp \
    ../../getic/Vtabctrl.cpp \
    ../../getic/Zone.cpp \
    ../../getic/bTbarCombo.cpp \
    ../../getic/brush.cpp \
    ../../getic/brush_prerotate.cpp \
    ../../getic/camera.cpp \
    ../../getic/clip.cpp \
    ../../getic/collections.cpp \
    ../../getic/cplusplus.cpp \
    ../../getic/dlgtexmody.cpp \
    ../../getic/geticgui.cpp \
    ../../getic/glsl.cpp \
    ../../getic/interpreter.cpp \
    ../../getic/rfHillTerrain.cpp \
    ../../getic/split.cpp \
    ../../getic/temp.cpp \
    ../../getic/texsys.cpp \
    ../../getic/texture.cpp \
    ../../getic/z-edmap.cpp \
    ../../getic/z-edmapkey.cpp \
    ../../getic/z_ed2Doc.cpp \
    ../../getic/z_ed2DocAlign.cpp \
    ../../getic/z_ed2DocDraw.cpp \
    ../../getic/z_ed2DocMk.cpp \
    ../../getic/z_ed2DocProc.cpp \
    ../../getic/z_ed2DocSave.cpp \
    ../../getic/z_ed2DocSave2.cpp \
    ../../getic/z_ed2DocSave_Old.cpp \
    ../../getic/z_ed2View.cpp \
    ../../getic/z_ed3View.cpp \
    ../../getic/z_edmap.cpp

HEADERS += \
    ../../MFC/mfc.h \
    ../../MFC/stdafx.h \
    ../../_include/_iinput.h \
    ../../_include/_irender.h \
    ../../_include/_isound.h \
    ../../_include/_isystem.h \
    ../../_include/bspfilestr.h \
    ../../_include/geticplug.h \
    ../../_include/geticplugbsp.h \
    ../../baselib/basecont.h \
    ../../baselib/baselib.h \
    ../../baselib/baselib2.h \
    ../../baselib/basethread.h \
    ../../baselib/baseutils.h \
    ../../baselib/sock.h \
    ../../getic/3dview.h \
    ../../getic/Aboutdlg.h \
    ../../getic/BaseEntity.h \
    ../../getic/BaseItemsDlgs.h \
    ../../getic/Bitarray.h \
    ../../getic/Boxitems.h \
    ../../getic/Brush.h \
    ../../getic/BspFIleStrDoc.h \
    ../../getic/Bsptree.h \
    ../../getic/CCrystalEditView.h \
    ../../getic/CCrystalTextBuffer.h \
    ../../getic/CCrystalTextView.h \
    ../../getic/CEditReplaceDlg.h \
    ../../getic/CFindTextDlg.h \
    ../../getic/ColorButon.h \
    ../../getic/Compiler.h \
    ../../getic/CompilerOutDlg.h \
    ../../getic/CoolDialogBar.h \
    ../../getic/CppCompiler.h \
    ../../getic/Crc.h \
    ../../getic/Curve_Class.h \
    ../../getic/CustItemCat.h \
    ../../getic/DialogEvalDets.h \
    ../../getic/Dirtree.h \
    ../../getic/DlgAssoc.h \
    ../../getic/DlgBSetTop.h \
    ../../getic/DlgBarLevExpl.h \
    ../../getic/DlgBarScript.h \
    ../../getic/DlgBigTerrain.h \
    ../../getic/DlgBrSet1.h \
    ../../getic/DlgBrSet2.h \
    ../../getic/DlgBrshProp.h \
    ../../getic/DlgClassItem.h \
    ../../getic/DlgCompProgress.h \
    ../../getic/DlgCompile.h \
    ../../getic/DlgCompilerPath.h \
    ../../getic/DlgConsole.h \
    ../../getic/DlgCpxPrimnames.h \
    ../../getic/DlgCustItem.h \
    ../../getic/DlgEditor2D.h \
    ../../getic/DlgFaceProp.h \
    ../../getic/DlgGameItem.h \
    ../../getic/DlgLightItem.h \
    ../../getic/DlgMaterial.h \
    ../../getic/DlgMirronChose.h \
    ../../getic/DlgMotionState.h \
    ../../getic/DlgMurrChs.h \
    ../../getic/DlgNewBrush.h \
    ../../getic/DlgNewItem.h \
    ../../getic/DlgNewPref.h \
    ../../getic/DlgNewProj.h \
    ../../getic/DlgNewTerrain.h \
    ../../getic/DlgPreferences.h \
    ../../getic/DlgResBrowser.h \
    ../../getic/DlgScripts.h \
    ../../getic/DlgSoundItem.h \
    ../../getic/DlgSounds.h \
    ../../getic/DlgStartPlayItem.h \
    ../../getic/DlgTexMody.h \
    ../../getic/DlgTexRefs.h \
    ../../getic/DlgTextures.h \
    ../../getic/DlgTriggerItem.h \
    ../../getic/DlgTriggers.h \
    ../../getic/DlgUnlock.h \
    ../../getic/Dlgbar.h \
    ../../getic/Dlggs.h \
    ../../getic/Dlgitems.h \
    ../../getic/Dlgleafs.h \
    ../../getic/Dlgscene.h \
    ../../getic/Drawout.h \
    ../../getic/Drawout1.h \
    ../../getic/EditKillFocus.h \
    ../../getic/ExceptionLog.h \
    ../../getic/FixSplitter.h \
    ../../getic/Flowpath.h \
    ../../getic/Frame2.h \
    ../../getic/Frustrum.h \
    ../../getic/GETIC_DSP.h \
    ../../getic/GListCtrl.h \
    ../../getic/Gameitem.h \
    ../../getic/Geoutils.h \
    ../../getic/GeticPlugIns.h \
    ../../getic/GeticProject.h \
    ../../getic/Igetic.h \
    ../../getic/KCSubdivision.h \
    ../../getic/LbEditable.h \
    ../../getic/Lboxitem.h \
    ../../getic/LightBulb.h \
    ../../getic/Lmproc.h \
    ../../getic/Lmproc2.h \
    ../../getic/MMove.h \
    ../../getic/MainFrm.h \
    ../../getic/Maskedit.h \
    ../../getic/Minibsp.h \
    ../../getic/Motion.h \
    ../../getic/MyStatusBar.h \
    ../../getic/PlugInCat.h \
    ../../getic/Poly.h \
    ../../getic/Portal.h \
    ../../getic/PortalPRC1.h \
    ../../getic/PortalPrc.h \
    ../../getic/ProgressDlg.h \
    ../../getic/Projector.h \
    ../../getic/PrpListCtrl.h \
    ../../getic/Prplb.h \
    ../../getic/Pvsprc.h \
    ../../getic/Replbox.h \
    ../../getic/SSCarrier.h \
    ../../getic/SampleDoc.h \
    ../../getic/SampleView.h \
    ../../getic/SceItem.h \
    ../../getic/Scene.h \
    ../../getic/ScriptEditView.h \
    ../../getic/ScriptItem.h \
    ../../getic/Scripts.h \
    ../../getic/Skydom.h \
    ../../getic/SoundCarrier.h \
    ../../getic/SoundItem.h \
    ../../getic/SplinePath.h \
    ../../getic/StartPlayItem.h \
    ../../getic/Staticgl.h \
    ../../getic/StdAfx.h \
    ../../getic/Subdivider.h \
    ../../getic/TabDlgSel.h \
    ../../getic/Tertree.h \
    ../../getic/TexAdapter.h \
    ../../getic/Texref.h \
    ../../getic/Texlbox.h \
    ../../getic/ToolFrame.h \
    ../../getic/TreeScene.h \
    ../../getic/TriggerItem.h \
    ../../getic/UiEnhancer.h \
    ../../getic/Undo.h \
    ../../getic/Undoitem.h \
    ../../getic/VertDlgDlg.h \
    ../../getic/Vmath.h \
    ../../getic/Vtabctrl.h \
    ../../getic/Z_edmap.h \
    ../../getic/Z_edmap_h.h \
    ../../getic/Zone.h \
    ../../getic/_config.h \
    ../../getic/bTbarCombo.h \
    ../../getic/baselibhdrs.h \
    ../../getic/bsphook.h \
    ../../getic/camera.h \
    ../../getic/cedefs.h \
    ../../getic/complexCompiler.h \
    ../../getic/editcmd.h \
    ../../getic/editreg.h \
    ../../getic/g-script-hdr.h \
    ../../getic/geticgui.h \
    ../../getic/geticgui.rc \
    ../../getic/glsl.h \
    ../../getic/interpreter.h \
    ../../getic/rfHillTerrain.h \
    ../../getic/script_hesder.h \
    ../../getic/scripthdr.h \
    ../../getic/tasks.h \
    ../../getic/texsys.h \
    ../../getic/texture.h \
    ../../getic/versionmacro.h \
    ../../getic/z-edmap.h \
    ../../getic/z_ed2.h \
    ../../getic/z_ed2Doc.h \
    ../../getic/z_ed2View.h \
    ../../getic/z_ed3View.h \
    ../../getic/z_edmap.rc \
    ../../getic/zedtypes.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../getic/DLG_MATERIAL.rc
