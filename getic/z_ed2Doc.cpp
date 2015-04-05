// z_ed2Doc.cpp : implementation of the CZ_ed2Doc class
//

#include "stdafx.h"
#include "_config.h"
#include "texadapter.h"
#include "DlgTexMody.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "mainfrm.h"
#include "Z_ed2View.h"
#include "Z_ed3View.h"
#include <algorithm>
#include "LightBulb.h"
#include "DlgNewPref.h"
#include "Motion.h"
#include "DlgTexRefs.h"
#include "TriggerItem.h"
#include "geticgui.h"
#include "versionmacro.h"

//



/////////////////////////////////////////////////////////////////////////////
CZ_ed2Doc*   CZ_ed2Doc::PDoc;
static int Gsec;

IMPLEMENT_DYNCREATE(CZ_ed2Doc, CDocument)

BEGIN_MESSAGE_MAP(CZ_ed2Doc, CDocument)
	//{{AFX_MSG_MAP(CZ_ed2Doc)
	ON_UPDATE_COMMAND_UI(ID_TB_LOCKXYZ, OnUpdateTbLockxyz)
    ON_UPDATE_COMMAND_UI(ID_BUROTATE, OnUpdateBurotate)
	ON_UPDATE_COMMAND_UI(ID_BUMOVE, OnUpdateBumove)
	ON_UPDATE_COMMAND_UI(ID_BUSCALE, OnUpdateBUScale)
	ON_UPDATE_COMMAND_UI(ID_BUSHCAM, OnUpdateBushcam)
	ON_UPDATE_COMMAND_UI(ID_BUSNAPGRID, OnUpdateBusnapgrid)
	ON_UPDATE_COMMAND_UI(ID_BUTSELBRUSH, OnUpdateButselbrush)
	ON_UPDATE_COMMAND_UI(ID_BUFACESEL, OnUpdateBufacesel)
	ON_UPDATE_COMMAND_UI(ID_BUWIREFRAME, OnUpdateBuwireframe)
	ON_UPDATE_COMMAND_UI(ID_BUSHOWBACK, OnUpdateBushowback)
	ON_COMMAND(ID_TB_LOCKXYZ, OnTbLockxyz)
	ON_COMMAND(ID_MK_BOX, OnMkBox)
	ON_COMMAND(ID_BUROTATE, OnBurotate)
	ON_COMMAND(ID_BUMOVE, OnBumove)
	ON_COMMAND(ID_BUSCALE, OnBUScale)
	ON_COMMAND(ID_BUSHCAM, OnBushcam)
	ON_COMMAND(ID_BUSNAPGRID, OnBusnapgrid)
	ON_COMMAND(ID_BUREVERSE, OnBureverse)
	ON_COMMAND(ID_BUTSELBRUSH, OnButselbrush)
	ON_COMMAND(ID_BUFACESEL, OnBufacesel)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_BUWIREFRAME, OnBuwireframe)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_BUSHOWBACK, OnBushowback)
	ON_COMMAND(ID_BUCSG, DoHSR)
	ON_COMMAND(ID_BUSHOWCSG, OnBushowcsg)
    ON_UPDATE_COMMAND_UI(ID_BUSHOWCSG, OnUpdateBushowcsg)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_TAB_NEXTSEL, OnTabNextsel)
	ON_COMMAND(ID_BURISELOW, OnBuriselow)
	ON_UPDATE_COMMAND_UI(ID_BURISELOW, OnUpdateBuriselow)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(MN_CUT, OnCut)
	ON_UPDATE_COMMAND_UI(MN_CUT, OnUpdateCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_DELETE, OnUpdateDelete)
	ON_COMMAND(ID_BUCYL, MkCylinder)
	ON_COMMAND(MN_ESCAPE, OnEscape)
	ON_UPDATE_COMMAND_UI(ID_MIRROR, OnUpdateMirror)
	ON_COMMAND(ID_MIRROR, OnMirror)
	ON_COMMAND(MN_ENTER, OnEnter)
	ON_UPDATE_COMMAND_UI(MN_ENTER, OnUpdateEnter)
	ON_COMMAND(TBB_XALIGN, OnXalign)
	ON_UPDATE_COMMAND_UI(TBB_XALIGN, OnUpdateXalign)
    ON_COMMAND(TBB_YALIGN, OnYalign)
    ON_UPDATE_COMMAND_UI(TBB_YALIGN, OnUpdateYalign)
	ON_COMMAND(TBB_ZALIGN, OnZalign)
    ON_UPDATE_COMMAND_UI(TBB_ZALIGN, OnUpdateZalign)
	ON_COMMAND(TBB_XINTER, OnXinter)
    ON_UPDATE_COMMAND_UI(TBB_XINTER, OnUpdateXinter)
	ON_COMMAND(TBB_YINTER, OnYinter)
    ON_UPDATE_COMMAND_UI(TBB_YINTER, OnUpdateYinter)
	ON_COMMAND(TBB_ZINTER, OnZinter)
    ON_UPDATE_COMMAND_UI(TBB_ZINTER, OnUpdateZinter)
	ON_COMMAND(TBB_XDIM, OnXdim)
    ON_UPDATE_COMMAND_UI(TBB_XDIM, OnUpdateXdim)
	ON_COMMAND(TBB_YDIM, OnYdim)
    ON_UPDATE_COMMAND_UI(TBB_YDIM, OnUpdateYdim)
	ON_COMMAND(TBB_ZDIM, OnZdim)
	ON_UPDATE_COMMAND_UI(TBB_ZDIM, OnUpdateZdim)
	ON_COMMAND(TBB_XYZDIM, OnXyzdim)
	ON_UPDATE_COMMAND_UI(TBB_XYZDIM, OnUpdateXyzdim)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_BUCONE, OnBucone)
	ON_COMMAND(ID_BUSTAIRS, OnBustairs)
	ON_COMMAND(ID_TAB_PREVSEL, OnTabPrevsel)
	ON_COMMAND(ID_SELECTALL, OnSelectall)
	ON_COMMAND(ID_TAB_SELNEXT, OnTabSelnext)
	ON_COMMAND(ID_TAB_SELPREV, OnTabSelprev)
	ON_COMMAND(KEY_BSPCURSEL, OnCompbsp)
	ON_COMMAND(ID_BUCOMPILE, OnBucompile)
	ON_UPDATE_COMMAND_UI(ID_BUCOMPILE, OnUpdateBucompile)
	ON_COMMAND(ID_VIEW_PORTALS, OnViewPortals)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PORTALS, OnUpdateOnViewPortals)
	ON_COMMAND(ID_VIEW_COLORED_LEAFS, OnViewColorLeafs)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLORED_LEAFS, OnUpdateOnViewColorLeafs)

    ON_COMMAND(MN_WHITE, OnViewWhite)
    ON_UPDATE_COMMAND_UI(MN_WHITE, OnViewWhiteUpdate)

    ON_COMMAND(MN_CLRBYFAR, OnViewByFar)
    ON_UPDATE_COMMAND_UI(MN_CLRBYFAR, OnViewByFarUpdate)

	ON_COMMAND(ID_SELSAMEASSELFACE, OnSelsameasselface)
	ON_COMMAND(ID_ALIGN_RIGHT_HIT, OnAlignRightHit)
	ON_COMMAND(ID_ALIGH_LEFTHIT, OnAlighLefthit)
	ON_COMMAND(ID_ALIGH_BOTHIT, OnAlighBothit)
	ON_COMMAND(ID_ALIGH_TOPHIT, OnAlighTophit)
	ON_COMMAND(MN_SELECTGROUP, OnSelectgroup)
	ON_COMMAND(MN_SELCURID, OnSelcurid)
	ON_UPDATE_COMMAND_UI(MN_SELCURID, OnUpdateSelcurid)
	ON_UPDATE_COMMAND_UI(MN_SELECTGROUP, OnUpdateSelectgroup)
	ON_UPDATE_COMMAND_UI(ID_SELSAMEASSELFACE, OnUpdateSelsameasselface)
    
	ON_UPDATE_COMMAND_UI(ID_SELECTALL, OnUpdateSelectall)
	ON_UPDATE_COMMAND_UI(ID_BUREVERSE, OnUpdateBureverse)
	ON_UPDATE_COMMAND_UI(ID_BUCSG, OnUpdateBucsg)
	ON_COMMAND(TBB_LR, OnLr)
	ON_UPDATE_COMMAND_UI(TBB_LR, OnUpdateLr)
	ON_COMMAND(TBB_TB, OnTb)
	ON_UPDATE_COMMAND_UI(TBB_TB, OnUpdateTb)
	ON_COMMAND(TBB_FB, OnFb)
	ON_UPDATE_COMMAND_UI(TBB_FB, OnUpdateFb)
	ON_COMMAND(ID_BUSHEET, OnBusheet)
	ON_COMMAND(ID_MATCHWIDTH, OnMatchwidth)
	ON_COMMAND(MN_SELITEM, OnSelitem)
    ON_UPDATE_COMMAND_UI(MN_SELITEM, OnUpdateSelitem)
	ON_COMMAND(MN_VIEW_FULLCOMP, OnViewFullBSP)
    ON_UPDATE_COMMAND_UI(MN_VIEW_FULLCOMP, OnUpdateOnViewFullBSP)
	ON_COMMAND(MN_INSITEM, OnInsitem)
	ON_COMMAND(MN_ADD_MODEL, OnAddModel)
	ON_UPDATE_COMMAND_UI(MN_ADD_MODEL, OnUpdateAddModel)
	ON_COMMAND(MN_INSPREF, OnInspref)
    ON_UPDATE_COMMAND_UI(MN_INSITEM, OnUpdateInsertMesh)
	ON_COMMAND(ID_BUSPHERE, OnBusphere)
	ON_COMMAND(KEY_BSPCURSEL, OnBspcursel)
	ON_COMMAND(MN_WPNCFG, OnWpncfg)
	ON_COMMAND(MN_COMPILEROPTIONS, OnCompileroptions)
	ON_UPDATE_COMMAND_UI(MN_COMPILEROPTIONS, OnUpdateCompileroptions)
	ON_COMMAND(TBB_SELVERTEX, OnSelvertex)
	ON_UPDATE_COMMAND_UI(TBB_SELVERTEX, OnUpdateSelvertex)
	ON_COMMAND(MN_TEXREFDLG, OnTexrefdlg)
	ON_COMMAND(MN_SHOWSELBBOX, OnShowselbbox)
	ON_COMMAND(MN_TERIAN, OnTerian)
	ON_UPDATE_COMMAND_UI(MN_TERIAN, OnUpdateTerian)
	ON_COMMAND(MN_2DSHAPE, On2dshape)
	ON_UPDATE_COMMAND_UI(MN_2DSHAPE, OnUpdate2dshape)
	ON_COMMAND(MN_RECLMAPS, OnReclmaps)
	ON_UPDATE_COMMAND_UI(MN_RECLMAPS, OnUpdateReclmaps)
	ON_COMMAND(ID_FILE_MERGE, OnFileMerge)
	ON_UPDATE_COMMAND_UI(PB_PARSKIN, OnUpdateParskin)
	ON_COMMAND(PB_PARSKIN, OnParskin)
	ON_COMMAND(MN_TRIANGULATE, OnTriangulate)
    ON_COMMAND(MN_TRIFAN, OnTriFan)
    ON_UPDATE_COMMAND_UI(MN_TRIFAN, OnUpdateTriFan)
	ON_COMMAND(MN_SAMETEXFACE, OnSametexface)
	ON_COMMAND(MN_SAMENORMFACE, OnSamenormface)
	ON_COMMAND(MN_INSGAMEITEM, OnInsgameitem)
	ON_UPDATE_COMMAND_UI(MN_INSGAMEITEM, OnUpdateInsgameitem)
	ON_COMMAND(TBB_SPLINE, OnSpline)
	ON_UPDATE_COMMAND_UI(TBB_SPLINE, OnUpdateSpline)
	ON_COMMAND(TBB_CLOSEDSPLINE, OnClosedspline)
	ON_UPDATE_COMMAND_UI(TBB_CLOSEDSPLINE, OnUpdateClosedspline)
	ON_COMMAND(MN_XOY, OnXoy)
	ON_UPDATE_COMMAND_UI(MN_XOY, OnUpdateXoy)
	ON_COMMAND(MN_XOZ, OnXoz)
	ON_UPDATE_COMMAND_UI(MN_XOZ, OnUpdateXoz)
	ON_COMMAND(MN_ZOY, OnZoy)
	ON_UPDATE_COMMAND_UI(MN_ZOY, OnUpdateZoy)
	ON_COMMAND(MN_EXTRUDE, OnExtrude)
	ON_UPDATE_COMMAND_UI(MN_EXTRUDE, OnUpdateExtrude)
	ON_COMMAND(MN_MERGE, OnMerge)
	ON_UPDATE_COMMAND_UI(MN_MERGE, OnUpdateMerge)
	ON_COMMAND(MN_PROJ_BRUSH, OnProjBrush)
	ON_UPDATE_COMMAND_UI(MN_PROJ_BRUSH, OnUpdateProjBrush)
	ON_COMMAND(MN_PERMCUT, OnPermcut)
	ON_UPDATE_COMMAND_UI(MN_PERMCUT, OnUpdatePermcut)
	ON_COMMAND(MN_UNDO, OnUndo)
	ON_UPDATE_COMMAND_UI(MN_UNDO, OnUpdateUndo)
	ON_UPDATE_COMMAND_UI(MN_TERVOL, OnUpdateTervol)
	ON_COMMAND(MN_TERVOL, OnTervol)
	ON_COMMAND(MN_MAN_TEX, OnManTex)
	ON_UPDATE_COMMAND_UI(MN_MAN_TEX, OnUpdateManTex)
	ON_COMMAND(MN_TO_XML, OnToXml)
	ON_UPDATE_COMMAND_UI(MN_TO_XML, OnUpdateToXml)
	ON_COMMAND(MN_EXTRACTFACES, OnExtractfaces)
	ON_UPDATE_COMMAND_UI(MN_EXTRACTFACES, OnUpdateExtractfaces)
	ON_COMMAND(MN_EVALDETS, OnEvaldets)
	ON_UPDATE_COMMAND_UI(MN_EVALDETS, OnUpdateEvaldets)
	ON_COMMAND(MN_HOME, OnHome)
	ON_COMMAND(MN_CREATESB, OnCreatesb)
	ON_UPDATE_COMMAND_UI(MN_CREATESB, OnUpdateCreatesb)
	ON_COMMAND(MN_LOADLASTBACKIP, OnLoadlastbackip)
	ON_UPDATE_COMMAND_UI(MN_LOADLASTBACKIP, OnUpdateLoadlastbackip)
	ON_COMMAND(MN_SHOWGRID, OnShowgrid)
	ON_UPDATE_COMMAND_UI(MN_SHOWGRID, OnUpdateShowgrid)
	ON_COMMAND(MN_WBCK, OnWbck)
	ON_COMMAND(MN_TRPOLYS, OnTrpolys)
	ON_COMMAND(MN_REPOINT, OnRepoint)
	ON_UPDATE_COMMAND_UI(MN_REPOINT, OnUpdateRepoint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(MN_ALBYBOX, OnAlbybox)
	ON_UPDATE_COMMAND_UI(MN_ALBYBOX, OnUpdateAlbybox)
	ON_COMMAND(TB_ZONES, OnZones)
	ON_UPDATE_COMMAND_UI(TB_ZONES, OnUpdateZones)
	ON_COMMAND(ID_VIEW_LEAF_BY_LEAF, OnViewLeafByLeaf)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LEAF_BY_LEAF, OnUpdateViewLeafByLeaf)
	ON_COMMAND(ID_VIEW_LIGHTMAPS, OnViewLightmaps)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIGHTMAPS, OnUpdateViewLightmaps)
	ON_COMMAND(ID_VIEW_GRAPH, OnViewGraph)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPH, OnUpdateViewGraph)
	ON_COMMAND(ID_CAM_LSPOT, OnCamLspot)
	ON_COMMAND(ID_FILE_SAVESEL_AS, OnFileSaveselAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVESEL_AS, OnUpdateFileSaveselAs)
	ON_COMMAND(MN_INSFACE, OnInsface)
	ON_UPDATE_COMMAND_UI(MN_INSFACE, OnUpdateInsface)
	ON_COMMAND(MN_CUTWITHBRUSH, OnCutwithbrush) 
	ON_UPDATE_COMMAND_UI(MN_CUTWITHBRUSH, OnUpdateCutwithbrush)
    ON_UPDATE_COMMAND_UI(MN_TRIANGULATE, OnUpdateTriangulate)
	ON_COMMAND(MN_TERIANBIG, OnTerianbig)
	ON_UPDATE_COMMAND_UI(MN_TERIANBIG, OnUpdateTerianbig)
	ON_COMMAND(MN_SNAPVXES, OnSnapvxes)
	ON_UPDATE_COMMAND_UI(MN_SNAPVXES, OnUpdateSnapvxes)
	ON_COMMAND(MN_INSVX, OnInsvx)
	ON_UPDATE_COMMAND_UI(MN_INSVX, OnUpdateInsvx)
	ON_COMMAND(MN_PAINTIMER, OnPaintimer)
	ON_UPDATE_COMMAND_UI(PB_COMBO, OnUpdateCombo)
	ON_UPDATE_COMMAND_UI(PB_COMBOTS, OnUpdateCombots)
	ON_COMMAND(MN_SELEDGE, OnSeledge)
	ON_UPDATE_COMMAND_UI(MN_SELEDGE, OnUpdateSeledge)
	ON_COMMAND(MN_REPOINTBRSH, OnRepointbrsh)
	ON_UPDATE_COMMAND_UI(MN_REPOINTBRSH, OnUpdateRepointbrsh)
	ON_COMMAND(MN_IMPP, OnImpp)
	ON_UPDATE_COMMAND_UI(MN_IMPP, OnUpdateImpp)
	ON_COMMAND(MN_EXPP, OnExpp)
	ON_UPDATE_COMMAND_UI(MN_EXPP, OnUpdateExpp)
	ON_COMMAND(MN_LEAFS, OnLeafs)
	ON_UPDATE_COMMAND_UI(MN_LEAFS, OnUpdateLeafs)
	ON_COMMAND(MN_SELNONDETBRUSHES, OnSelnondetbrushes)
	ON_COMMAND(MN_DETBRUSHES, OnDetbrushes)
	ON_UPDATE_COMMAND_UI(MN_INSPREF, OnUpdateInsertMesh)
	ON_UPDATE_COMMAND_UI(ID_MK_BOX, OnUpdateInsertMesh)
	ON_UPDATE_COMMAND_UI(ID_BUCYL, OnUpdateInsertMesh)
	ON_UPDATE_COMMAND_UI(ID_BUCONE, OnUpdateInsertMesh)
	ON_UPDATE_COMMAND_UI(ID_BUSTAIRS, OnUpdateInsertMesh)
	ON_UPDATE_COMMAND_UI(ID_BUSHEET, OnUpdateInsertMesh)
	ON_UPDATE_COMMAND_UI(ID_BUSPHERE, OnUpdateInsertMesh)
	ON_COMMAND(TB_COLOR, OnColor)
	ON_UPDATE_COMMAND_UI(TB_COLOR, OnUpdateColor)
	//}}AFX_MSG_MAP
  

END_MESSAGE_MAP()



CZ_ed2Doc::CZ_ed2Doc()
{
	CZ_ed2Doc::PDoc=this;
    _frmTimer     = 0;
    _complBreaking = FALSE;
    _blocksel      = FALSE;
    _lastFocusView = 0;
    _clearing = FALSE;
    _hrc = 0;
    _curgroup      = -1; //all
    _maxUsedGrp    = 0;
    _curCreatGroup = 0;
	_cmdMsg		   = 0;
    GUtex      = 0;
    _userBool  = FALSE;
    _allselected = 0;
    _tildselect  = 0;
	//
    _undodirt       = 0; 
    _docDirty       = 0;
    _alignment      = FALSE;
    _brmodePrev     = BR_VIEW;
    _brmode         = BR_VIEW;
    _leafNo         = -1;
    _compiling      = FALSE;
    _closing        = FALSE;
    _selBoxByTouch  = FALSE;
    _cachedSett._skyDom = 0;
    _cachedSett._isSolid    =1;
    _cachedSett._radstrips  =-1;
    _cachedSett._reversed   =0;
    _cachedSett._detail  =0;
    _cachedSett._thikness   =0;
    _cachedSett._vstrips    =-1;
	_cachedSett._tm         = TM_LIN;
    _pActiveView        = 0;
    _exViewMode	        =  0;
	_vmodeback          = FALSE;	
    _selmode            = SEL_NONE;
    _compviewmode       = C_NA;
    _lastItmSel         = 0;
    _viewChanged        = 0;
    _selShowSelBox      = 0;
    _viewCount          = 0; 
	b_showgrid          = 1;
    b_whitebk           = 0;
    _glAddSwapHintRectWIN = 0;
    _use2splitidx       = -1;
    _maxUsedGrp       = 0;
    Brush::GID        = 0;
    _use2splitidx       = -1;
    SceItem::_GItemID = 0;

    _camSpot            = 0; 
}

CZ_ed2Doc::~CZ_ed2Doc()
{
	CZ_ed2Doc::PDoc=0;
} //OCR_SIZEALL


BOOL CZ_ed2Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    Poly::__Max=V3(0,0,0);
    if(_scene.GetPrimitives()->size() && _docDirty)
    {
        UINT retval = AfxMessageBox("Do you want to Save the current Getic scene file ? ", MB_YESNOCANCEL|MB_ICONQUESTION);
        if(retval==IDCANCEL)
            return 0;
        if(retval==IDYES)
        {
            OnFileSave();
        }
    }

    _closing = FALSE;
    InitDocVars();
	return TRUE;
}

BOOL CZ_ed2Doc::OnNewDocument()
{
    Poly::__Max=V3(0,0,0);
    if(_scene.GetPrimitives()->size() && _docDirty)
    {
        UINT retval = AfxMessageBox("Do you want to Save the current Getic scene file ?", MB_YESNOCANCEL|MB_ICONQUESTION);
        if(retval==IDCANCEL)
            return 0;
        if(retval==IDYES)
        {
            OnFileSave();
        }
    }
	if (!CDocument::OnNewDocument())
		return FALSE;
    InitDocVars();
    SceItem::_GItemID = 0;
    Brush::GID        = 0;

    if(!_scene.SearchCats(theApp.XsdCatItmFile()))
    {
        theApp.XsdCatItmFile("");

    }
    return 1;
}


void CZ_ed2Doc::InitDocVars()
{
#ifdef _FREE
    GLight = 0;
#endif //
    DeselectAll();
    _scene.Clear();

    _undo.Clear();
    _undodirt       = 0;
    _docDirty       = 0;

    _use2splitidx = -1;
    _closing      = FALSE;
    _gridSize     = 40000;           
    _gridStep     = 20;
    _xyz_lock     = 1;
    _bDrawCamRay  = TRUE;
    _brmode       = BR_VIEW;
    _brmodePrev     = BR_VIEW;
    _compviewmode = C_NA;
    _bSnapGrid = FALSE;
    _selmode   = SEL_NONE;

	_vmodefire = FALSE;
	_vmodeback = FALSE;
    _bShowCsg  = FALSE;
    _maxUsedGrp   = 0;
    Brush::GID        = 0;
    SceItem::_GItemID = 0;
    _use2splitidx       = -1;
    _cam.SetFov(2*PIPE6); 
	_lvupdate = GetTickCount();
    _compiler._finalBSpFileName="";

    _pClipboard.clear();
    _pClipboardItm.clear();
    _undo.Clear();

    _cam.SetFar(24800.0);//250m
	_cam.SetAngles(-PIPE4,-PIPE6,0);
	_cam.Euler2Pos();
	_cam._pos = V3(1600.0f,1000.0f,1600.0f);

    _viewChanged = 6;
    theApp.CleanTexMan();
    
    if(VIEW2D(1,0))
        VIEW2D(1,0)->Reinit();
    if(VIEW2D(0,1))
	    VIEW2D(0,1)->Reinit();
    if(VIEW2D(1,1))
        VIEW2D(1,1)->Reinit();

    _selBox.Clear();
	_selBox.MakeCube(V3(1000, 1000, 1000));
    _selBox._brushflags = BRSH_SELBOX;
    _selBox.Recalc();
    TexHandler::SetSearchPath(0);

    return ;
}



/////////////////////////////////////////////////////////////////////////////
// CZ_ed2Doc serialization

void CZ_ed2Doc::Serialize(CArchive& ar)
{

}

/////////////////////////////////////////////////////////////////////////////
// CZ_ed2Doc diagnostics

#ifdef _DEBUG
void CZ_ed2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CZ_ed2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZ_ed2Doc commands

void CZ_ed2Doc::OnTbLockxyz() 
{
	_xyz_lock = !_xyz_lock;
	

	CZ_ed2View* pTX = VIEW2D(1,0);
	CZ_ed2View* pTY = VIEW2D(0,1);
	CZ_ed2View* pTZ = VIEW2D(1,1);

    pTX->Reinit();
    pTY->Reinit();
    pTZ->Reinit();
/*    
    SetShift(pSrc);
    SetZoom(pSrc);
*/
    Invalidate();
}

void CZ_ed2Doc::OnUpdateTbLockxyz(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->SetCheck(_xyz_lock);
}


void CZ_ed2Doc::SetShift(CZ_ed2View* pSrc)
{
	CZ_ed2View* pTX = VIEW2D(1,0);
	CZ_ed2View* pTY = VIEW2D(0,1);
	CZ_ed2View* pTZ = VIEW2D(1,1);

    switch(pSrc->_vt)
    {
        case 'x':                 // xYZ        // SIDE
            pTY->_shifty = -pSrc->_shiftx;
            pTZ->_shifty = pSrc->_shifty;
            _shift.y = -pSrc->_shifty;
            _shift.z = pSrc->_shiftx;
            break;
        case 'y':               // XyZ          // TOP
            pTX->_shiftx = -pSrc->_shifty;  // side
            pTZ->_shiftx = pSrc->_shiftx;              // front
            _shift.z = pSrc->_shifty;
            _shift.x = pSrc->_shiftx;
            break;
        case 'z':               // XYz          // FRONT
            pTX->_shifty = pSrc->_shifty;
            pTY->_shiftx = pSrc->_shiftx;               // top
            _shift.x = pSrc->_shiftx;
            _shift.y = pSrc->_shifty;
            break;
    }
    
}


void CZ_ed2Doc::SetZoom(CZ_ed2View* pSrc)
{
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTX = VIEW2D(1,0);
    CZ_ed2View* pTZ = VIEW2D(1,1);
    pTY->_zoom = pSrc->_zoom;
    pTX->_zoom = pSrc->_zoom;
    pTZ->_zoom = pSrc->_zoom;
    _zoom = pTZ->_zoom;
}


void CZ_ed2Doc::Invalidate(BOOL update) 
{
    if(ThrID != GetCurrentThreadId())
        return;

    CZ_ed2View* pTX   = VIEW2D(1,0);
    CZ_ed2View* pTY   = VIEW2D(0,1);
    CZ_ed2View* pTZ   = VIEW2D(1,1);
    z_ed3View* pT3D   = VIEW3D(0,0);
    if(pTX && pT3D)
    {
        pTX->RedrawDC(update);
        pTZ->RedrawDC(update);
        pTY->RedrawDC(update);
        pT3D->RedrawDC(update);
    }
}

void    CZ_ed2Doc::Invalidate3D(BOOL fromMM) 
{
    if(ThrID != GetCurrentThreadId())
        return;

    if(fromMM)
	{
		if(GetTickCount() - _lvupdate < 32) 
		{
			_lvupdate = GetTickCount();
			return;
		}
	}
	z_ed3View* pTXYZ = VIEW3D(0,0);
	pTXYZ->RedrawDC();

}


void CZ_ed2Doc::OnBuriselow() 
{
    if(_brmode!=BR_RISE)
        _brmode = BR_RISE;
    else
        _brmode = BR_VIEW;
	
}

void CZ_ed2Doc::OnUpdateBuriselow(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}


void CZ_ed2Doc::OnBurotate() 
{
    if(_brmode == BR_ROTATE)
    {
        _brmode = BR_VIEW;
        return;
    }
    _brmode = BR_ROTATE;
}

void CZ_ed2Doc::OnUpdateBurotate(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling ||IsSelBoxOn() || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable((_pSelBrushes.size() || _pSelItems.size()) || _pSelPolys.size()==1);
    pCmdUI->SetCheck(_brmode == BR_ROTATE);
}


void CZ_ed2Doc::OnBumove() 
{
    if(_brmode == BR_MOVE)
    {
        _brmode = BR_VIEW;
        return;
    }
    _brmode = BR_MOVE;
}

void CZ_ed2Doc::OnUpdateBumove(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_pSelBrushes.size() || _pSelItems.size() || _pSelVertexes.size()|| 
			       _pSelPolys.size()==1 || _selBox._brushflags & BRSH_SELVIS);
	pCmdUI->SetCheck(_brmode == BR_MOVE);

    SBAR().SetSelection(_pSelItems.size() || _pSelPolys.size() || _pSelBrushes.size() || _pSelVertexes.size());
	
}

void CZ_ed2Doc::OnBUScale() 
{
    if(_brmode == BR_SCALE)
    {
        _brmode = BR_VIEW;
        return;
    }
    _brmode = BR_SCALE;
}


void CZ_ed2Doc::OnUpdateBUScale(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable((_pSelBrushes.size()  || 
                    _pSelItems.size()) || _pSelPolys.size()==1||
                    _selBox._brushflags & BRSH_SELVIS);
    pCmdUI->SetCheck(_brmode == BR_SCALE);
}


void    CZ_ed2Doc::DoSelection2D(REAL x, REAL y, CZ_ed2View* pV, _EL_SEL sel)
{
	V3	 start = _cam._pos;
	V3	 end   = _cam._pos;

	
	switch(pV->_vt)
	{
		case 'x':
			start.y = y;
			start.z = x;
			if (_cam._fwd.x>0)
			{
				end.x   += 32768.f;
			}
			else
			{
				end.x -= 32768.f;
			}
			break;
		case 'y':
			start.x = x;
			start.z = y;

			if (_cam._fwd.y>0)
			{
				end.y += 32768.f;
			}
			else
			{
				end.y -= 32768.f;
			}
			break;
		case 'z':
			start.x = x;
			start.y = y;
			if (_cam._fwd.z>0)
			{
				end.z += 32768.f;
			}
			else
			{
				end.z -= 32768.f;
			}
			break;
	}
	RayIntersectBrush(start,end,sel);
}

void    CZ_ed2Doc::DoSelection3D(CPoint& point, z_ed3View* pV, _EL_SEL sel)
{

    RECT& rt = pV->_rt; 
    if(rt.bottom==0)rt.bottom=1;
    V3  scrPt;
    Make3dCursorPos(pV, point, rt, scrPt);


    V3  start = _cam._pos;                 // build the seg
    V3  vdir  = (scrPt - start).norm();
    V3  end   = start + vdir * 32768.f;     // 64 meters long

    if(_compviewmode==C_FULL)
    {
        /*
        if(SEL_VERTEX == _selmode || SEL_FACE == _selmode)
        {
            _compiler.RayIntersect(start, vdir, end, _selmode);
        }
        */

    }
    else
    {
        if( SEL_BRUSH  == _selmode|| 
            SEL_FACE   == _selmode || 
            SEL_VERTEX == _selmode|| 
            SEL_EDGE   == _selmode)
	        RayIntersectBrush(start, end, sel);
        else if(_selmode == SEL_ITEMS)
            RayIntersectItems(start, end);
    }
}

void CZ_ed2Doc::RayIntersectItems(V3& s, V3& vend)
{
    int       itmSel = -1;
    M4        tm;
    V3        ipct;
    REAL      bestDist = 999999999.0;
    SceItem*  pItemFnd = 0;
    V3        vdir =   vend-s;
    V3        ovj2Cam;  

    vdir.norm();

    _cam.Euler2Pos();
    vvector<SceItem*>*  pTemsPtr = GetItems();

    FOREACH(vvector<SceItem*>, (*pTemsPtr), ppItem)
    {
        SceItem*  pItem  = *ppItem;

        // reject any out of +-45 degrees
        ovj2Cam = pItem->_t - s;
        ovj2Cam.norm();
        if(Vdp(ovj2Cam, _cam._fwd) < .707)
            continue;

        // buld a plane there facig toward us
        Plane pthere(-_cam._fwd, pItem->_t);

        if(pthere.RayIntersect(s, vdir, ipct))
        {   

            REAL vd = vdist(ipct, pItem->_t) ;
            if(vd < pItem->GetWBox().GetExtends().len())
            {
                REAL dtoip = vdist(ipct, s);
                if(dtoip < bestDist)
                {
                    bestDist = dtoip;
                    pItemFnd = pItem;
                    _ip[0]   = ipct;
                    _ip[1]   = pthere._n;
                }
            }
        }
    }

    if(pItemFnd)
    {
        if(AKEY(VK_CONTROL) ) // select cumulative multiple selection
        {
            if(pItemFnd->IsSelected())
            {
                DeselectItem(pItemFnd);
            }
            else
            {
                SelectItem(pItemFnd);
            }

        }
        else                                     // exclusive selection
        {
            DeselectItems();
            SelectItem(pItemFnd);
        }
        Invalidate();
/*
        // update the Bar Dlg with the first selected item
        vvector<SceItem*>*  pTemsPtr = GetItems();
        FOREACH(vvector<SceItem*>, (*pTemsPtr), ppItem)
        {
            if((*ppItem)->IsSelected())
            {
                BARDLG()->OnItemSelected((*ppItem));
                break;
            }
        }
*/

    }
}

void    CZ_ed2Doc::DeselectItem(SceItem* pItemFnd, BOOL bNoty)
{
    pItemFnd->DeSelect();
    if(_pSelItems.erase_if(pItemFnd))
    {
        if(bNoty)BARDLG()->OnItemSelected(pItemFnd,0);
    }

    if(_pSelItems.size() == 0)
    {
        _brmode  = BR_VIEW;
    }
}

BOOL    CZ_ed2Doc::SelectItem(SceItem* pItemFnd, BOOL notyBar)
{
    if(_pSelItems.push_unique(pItemFnd))
    {
        pItemFnd->Select();
        if(_bSnapGrid)
        {
            int x = pItemFnd->_t.x; x/=SnapToXY; x*=SnapToXY;
            int y = pItemFnd->_t.x; y/=SnapToXY; y*=SnapToXY;
            int z = pItemFnd->_t.y; z/=SnapToXY; z*=SnapToXY;
            pItemFnd->_t.x = x;
            pItemFnd->_t.y = y;
            pItemFnd->_t.z = z;
        }

        if(notyBar)
        {
            HWND hwnd = ::GetFocus();
            BARDLG()->OnItemSelected(pItemFnd);
            if(hwnd != ::GetFocus())
            {
                ::SetFocus(hwnd);
            }
            
        }
        return 1;
    }
    return 0;
}

void CZ_ed2Doc::DeselectItems(BOOL notyBar)
{
    FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
	{
        (*ppItem)->DeSelect();
		if((*ppItem)->_flags & BRSH_NEW)
        {
			delete (*ppItem);
        }
	}
    if(notyBar)
        BARDLG()->DeselectItems(0);
    _pSelItems.clear();
}

void CZ_ed2Doc::RayIntersectBrush(V3& s, V3& end, _EL_SEL sel)
{
    int brSel,faceSel,vxSel;

    // see if ve've intersect some first seen brush or first seen face
    if(BrushIntersect(s, end, brSel, faceSel, vxSel))
    {
        Poly* ps;
        switch(sel)
        {
            default:
            case SEL_NONE:
                break;
            case SEL_BRUSH:
                SelectBrush(brSel, faceSel);
                BARDLG()->OnBrushesSelected();
                break;
            case SEL_FACE:
                {
                    Brush* b = GetForm(brSel);             
                    if(b && b->IsSpecial())
                        break;
                }
                ps = SelectFace(brSel, faceSel);
                if(ps && AKEY(VK_MENU))
                {
                    AddVertex(ps, _ip[0]);
                    DeselectAll();
                }
                else
                {
                    BARDLG()->OnFacesSelected();
                }
                break;
            case SEL_VERTEX:
                SelectVertex(brSel, faceSel, vxSel);
                break;
            case SEL_EDGE:
                SelectEdge(brSel, faceSel, vxSel);
                break;

        }
    }
}



void    CZ_ed2Doc::Make3dCursorPos(z_ed3View* pV, CPoint& point, RECT rt, V3& out)
{
    if(rt.bottom==0) rt.bottom=1; 

    double rr = (double)rt.right /2.0;
    double rb = (double)rt.bottom/2.0;

    //transpofrm the cursorpos by screen center
    double    ptxpC  = ((double)point.x - rr) / rr;
    double    ptypC  = (rb - (double)point.y) / rb;   //inverty

    // make the fov
    double aspect     = rr/rb;

    double halfScrV   = tan(G2R((double)pV->_fov/2.0)) * (double)pV->_fnear;
	double halfScrH   = halfScrV * aspect;
    
    _cam.Euler2Pos();

    // calc the on screeen pos
    double scrX = ptxpC * halfScrH;
    double scrY = ptypC * halfScrV;

//    TRACE("%2.2f %2.2f \r\n", scrX, scrY);

	_frustDots[0] = _cam._pos;
	_frustDots[1] = _cam._pos + _cam._fwd*pV->_fnear + (_cam._up * halfScrV)  +   _cam._right *  (-halfScrH);
	_frustDots[2] = _cam._pos + _cam._fwd*pV->_fnear + (_cam._up * halfScrV)  +   _cam._right *  (halfScrH);
	_frustDots[3] = _cam._pos + _cam._fwd*pV->_fnear + (_cam._up * (-halfScrV))  + _cam._right *  (halfScrH);
	_frustDots[4] = _cam._pos + _cam._fwd*pV->_fnear + (_cam._up * (-halfScrV))  + _cam._right *  (-halfScrH);

    V3  pointer =  _cam._pos + (_cam._fwd * pV->_fnear) + (_cam._up * scrY) + (_cam._right * scrX);
	_camray[2] = pointer;
	_camray[0] =_cam._pos;

    if(AKEY(VK_LBUTTON)) //getAsyncKeyState
    {
	    _camray[1] = (pointer-_cam._pos);
	    _camray[1].norm();
	    _camray[1] = _cam._pos + _camray[1] * 6400.f;
    }


    out = pointer;

	
    
}

BOOL    CZ_ed2Doc::BrushIntersect(V3& vstart, V3& vend, 
                                  int& bsIdx, 
                                  int& psIdx,
                                  int& vxIdx)
{
    V3      a,b,d,cp;
	REAL    dist  = (999999999.f);
    int     bIdx = 0;

    bsIdx   = -1;
    psIdx   = -1;
    vxIdx   = -1;
    a		= vstart;
    b		= vend; 
    cp		= vstart;//_cam._pos;
    d		= (b-a).norm();
	Box	box;
	BOOL	bstartInSolid = FALSE;
    V3      ip;
    
	_cam.Euler2Pos();
	Plane	camPlane(_cam._fwd, _cam._pos+_cam._right);

    PBrushes* rbs = this->GetForms();

	// if cut select the far face
    FOREACH(PBrushes, (*rbs), ppB)
    {
        Brush& rb = (**ppB);

        if(_curgroup>=0)
        {
            if(_curgroup != rb._group)
                continue;
        }

        if(_cam.IsCulled(&rb._box._min,&rb._box._max))
           continue;

		box		  = rb._box;
        box.Expand(8.f);
        

		if(!rb.IsSolid() && box.ContainPoint(a))
			bstartInSolid=TRUE;
		else
			bstartInSolid=FALSE;

		Polys& polys = rb._polys;

 
        FOREACH(Polys, polys, pB)
        {
            Poly& pl = *pB;

            if(_cam.IsCulled(&pl._box._min,&pl._box._max))
                continue;
			
			// shooting face
			// normally we shoot from front face DP>0
			REAL shf  = Vdp(d, pl._n);	
								
			if(bstartInSolid)		//we invert the shf. we shoot from behind faces
				shf=-shf;

            /*
            // select back faces from behind too
			if(shf > 0 && !(pl._props & FACE_SHOWBACK))
				continue;
            */

			if(camPlane.GetSide(pl._box.GetCenter())<0)
				continue;

			if(pl.RayIntersect(a, d, ip) )
            {
				if(camPlane.GetSide(ip)<0)
					continue;

                if(pl.ContainPoint(ip) && box.ContainPoint(ip))
                {
                    REAL d2   = vdist2(ip, cp);
                    if(d2 < dist )
                    {
                        bsIdx    = ppB - rbs->begin();
                        psIdx    = pB  - rb._polys.begin();
                        dist     = d2;
                        _ip[0]   = ip;
						_ip[1]   = pl._n;
                    }
                }
            }
        }
    }

    //
    // selects the closest vertx on selected face
    //
    if(psIdx>=0 &&  (_selmode == SEL_VERTEX ||
                     _selmode == SEL_EDGE))
    {   
        REAL   dst;
        REAL   closest = INFINIT;
        Brush* pSelB   = (*rbs)[bsIdx];
        Poly&  poly    = pSelB->_polys[psIdx];
        
        FOREACH(vvector<Vtx2>, poly._vtci, pV)
        {
            dst = vdist2(pV->_xyz, _ip[0]);
            if(dst < closest)
            {
                closest = dst;
                vxIdx   = pV - poly._vtci.begin();
            }
        }
    }
    return (bsIdx >= 0);
}


void CZ_ed2Doc::OnBushcam() 
{
	_bDrawCamRay = !_bDrawCamRay;
	
}

void CZ_ed2Doc::OnUpdateBushcam(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->SetCheck(_bDrawCamRay);
}

void CZ_ed2Doc::OnBusnapgrid() 
{
	_bSnapGrid=!_bSnapGrid;
	
}

void CZ_ed2Doc::OnUpdateBusnapgrid(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->SetCheck(_bSnapGrid);
}


void CZ_ed2Doc::OnSelitem() 
{
    DeselectAll();

    _brmode       = BR_VIEW;
    _compviewmode = C_NA;

    if(_selmode == SEL_ITEMS)
    {
        _selmode = SEL_NONE;

        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

        return;
    }
	_selmode = SEL_ITEMS;
    Invalidate();
}

//SEL_NONE, SEL_BRUSH, SEL_FACE, SEL_VERTEX
void CZ_ed2Doc::OnButselbrush() 
{
    DeselectAll();
    _brmode       = BR_VIEW;

    _compviewmode = C_NA;
    if(_selmode == SEL_BRUSH)
    {
        _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

        Invalidate();
        return;
    }
	_selmode = SEL_BRUSH;
    Invalidate();
}

void CZ_ed2Doc::OnSeledge() 
{
    DeselectAll();
    _brmode       = BR_VIEW;
    _compviewmode = C_NA;
    if(_selmode == SEL_EDGE)
    {
        _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

        Invalidate();
        return;
    }
	_selmode = SEL_EDGE;
    Invalidate();
}

void CZ_ed2Doc::OnUpdateSeledge(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA || (_scene.GetPrimitives()->size()==0))
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->SetCheck(_selmode == SEL_EDGE);
	
}


void CZ_ed2Doc::OnSelvertex() 
{
    DeselectAll();
    _brmode       = BR_VIEW;
    _compviewmode = C_NA;
    if(_selmode == SEL_VERTEX)
    {
        _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

        Invalidate();
        return;
    }
	_selmode = SEL_VERTEX;
    Invalidate();
}


void CZ_ed2Doc::OnBufacesel() 
{
    DeselectAll();
    _brmode       = BR_VIEW;
    _compviewmode = C_NA;
    if(_selmode == SEL_FACE)
    {
        _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

        Invalidate();
        return;
    }
	_selmode = SEL_FACE;
    Invalidate();
}

void CZ_ed2Doc::OnUpdateButselbrush(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA || (_scene.GetPrimitives()->size()==0))
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(!(_selBox._brushflags & BRSH_SELVIS));

	pCmdUI->SetCheck(_selmode == SEL_BRUSH);
}

void CZ_ed2Doc::OnUpdateSelitem(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode != C_NA ||(_scene.GetItems()->size()==0))
    {
        pCmdUI->Enable(0);
        return;
    }
    pCmdUI->Enable(1);
	pCmdUI->SetCheck(_selmode == SEL_ITEMS);
}


void CZ_ed2Doc::OnUpdateSelvertex(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA || (_scene.GetPrimitives()->size()==0))
    {
        //_compviewmode==C_FULL
        pCmdUI->Enable(0);
        return;
    }
    pCmdUI->Enable(1);
    pCmdUI->SetCheck(_selmode == SEL_VERTEX);
}


void CZ_ed2Doc::OnUpdateBufacesel(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA || (_scene.GetPrimitives()->size()==0))
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(!(_selBox._brushflags & BRSH_SELVIS));
    pCmdUI->SetCheck(_selmode == SEL_FACE);
}

//allways select the primitives
void CZ_ed2Doc::SelectBrush(int brSel, int faceSel)
{
    if(AKEY(VK_CONTROL) ) // select cumulative multiple selection
    {
        Brush* b = GetForm(brSel);
        if(!b->IsSelected())
            SelectBrush(b);
        else
            DeSelectBrush(b);
    }
    else                                     // exclusive selection
    {
        DeselectBrushes();
        SelectBrush(GetForm(brSel));
    }
}


void CZ_ed2Doc::SelectAllBrushVertexes(Brush& b, Vtx2& x)
{
	FOREACH(vvector<Poly>, b._polys,pp)
	{
		FOREACH(vvector<Vtx2>, pp->_vtci, pv)
		{
			if(IsZero(x._xyz-pv->_xyz,EPSMED))
			{
				SelectVertex(&(*pv), &(*pp));
			}
		}
	}
}

void CZ_ed2Doc::DeselectVertex(Vtx2* pv)
{
	if(_pSelVertexes.erase_if(pv))
	{
		if(pv->_pPoly)
            DeSelectFace(pv->_pPoly);
		pv->Select(0);
	}

}

void CZ_ed2Doc::SelectVertex(Vtx2* pv, Poly* pPoly)
{
    _pSelVertexes.push_unique(pv);
	SelectFace(pPoly);
	pv->Select(pPoly);
}

void CZ_ed2Doc::SelectVertex(int brSel, int nPoly, int vxSel)
{
    try{
        if(!AKEY(VK_CONTROL) )   // acumulate selections
        {
            DeselectVertexes();
        }
        Brush* b = GetForm(brSel);
        if(0==b)       
            return;

        // sel vertex blocked on already rotated brush
        if((b->_r.isnot0()))                     
        {
            Beep(600,20);
            SBT(0,"Cannot select a vertex on rotated brush");
            return;
        }

        if((UINT)nPoly >= b->_polys.size())       return;
                Poly& p = b->_polys[nPoly];
        if((UINT)vxSel >= p._vtci.size())         return;
            Vtx2& x = p._vtci[vxSel]; 

	    SelectAllBrushVertexes(*b, x);
    }
    catch(...){
        OnEscape();
    }
}



void CZ_ed2Doc::SelectEdge(int brSel, int nPoly, int vxSel)
{
    _selEdges.clear();

    PolyEdge edge;
    //find the closest line to ip and select that edge (2 vertexex)
    //_ip[0]
    Brush* b      = GetForm(brSel);
    edge._poly    = &b->_polys[nPoly];
    edge._edge[0] = &edge._poly->_vtci[vxSel]; 
    int vxSel2    = ((vxSel+1) % edge._poly->_vtci.size());
    edge._edge[1] = &edge._poly->_vtci[vxSel2]; 
    _selEdges.push_back(edge);

    SelectFace(brSel, nPoly);

}


// allways select the primitives
Poly* CZ_ed2Doc::SelectFace(int brSel, int faceSel)
{
    Poly* poly = 0;
    if(AKEY(VK_CONTROL) ) // acumulate selections
    {
        Brush* b = GetForm(brSel);             
        Poly&  p = b->_polys[faceSel];
        if(! p.IsSelected())
        {
            SelectFace(&p);
            poly = &p;
        }
        else
            DeSelectFace(&p);
    }
    else                                    // exclusive selection
    {
        Brush* b = GetForm(brSel);             
        DeselectPolys();
        if(faceSel >= 0 && faceSel < (int)b->_polys.size())
        {
		    Brush* b = GetForm(brSel);
            if(b)
            {
                Poly&  p = b->_polys[faceSel];
                SelectFace(&p, b);
                poly = &p;
            }
        }
    }
    return poly;
}

void    CZ_ed2Doc::DeselectBrushes()
{
    FOREACH(PBrushes,  _pSelBrushes,a)
    {
        (*a)->DeSelect();
        (*a)->Dirty(0);
        if((*a)->_brushflags & BRSH_NEW)
            delete *a;
    }
   _pSelBrushes.clear();
}//Vtx2

void    CZ_ed2Doc::DeselectPolys(BOOL desel)
{
    if(desel)
    {
        FOREACH(PPolys,  _pSelPolys, a)
            (*a)->DeSelect();
    }
   _pSelPolys.clear();
}

void    CZ_ed2Doc::DeselectVertexes()
{
	DeselectPolys();
	FOREACH(vvector<Vtx2*>,_pSelVertexes,ppv)
		(*ppv)->Select(0);
    _pSelVertexes.clear();
}

void    CZ_ed2Doc::DeselectAll(BOOL killDets)
{
    if(killDets)
		_devdet.DestroyWindow();

    DeselectPolys();
    DeselectBrushes();
    DeselectItems();
    DeselectVertexes(); 
    _selEdges.clear();

    if(!_closing)
    {
        if(_selmode == SEL_BRUSH || _selmode == SEL_FACE)
        {
            BARDLG()->OnBrushesSelected();
            BARDLG()->OnFacesSelected();
        }
        else if(_selmode == SEL_ITEMS)
        {
            BARDLG()->OnItemSelected(0,0);
        }
        BARDLG()->Rescan();
        if(!_clearing)
            Invalidate();
    }
    
}


void CZ_ed2Doc::OnFileSave() 
{
#ifdef _TRIAL_ 
    error;
    return;
#else //
    if(GetTitle() == "NoName")
    {
        OnFileSaveAs();
    }
    else
    {
        _lasterror.erase();

        try{
            DeselectAll();
            SaveInFile(GetTitle());
            _docDirty = 0;
        }
        catch(int& i)
        {
            i;
            AfxMessageBox(_lasterror.c_str(), MB_OK|MB_ICONWARNING);
            _lasterror.erase();
        }
        catch(...)
        {
            AfxMessageBox("Unknown Exception while Saving", MB_OK|MB_ICONHAND);
        }
    }
#endif
}


void CZ_ed2Doc::OnFileSaveAs() 
{
#ifdef _TRIAL_
    return;
#else

	REDIR();  
    LPTSTR lpszPathName = NULL;
    char szFilters[] = _T("Geometry raw (*.grf)|*.grf|"
                          "Geometry selection (*.grs)|*.grs|"  
                          "Geometry prefab (*.gpm)|*.gpm||");  

    // Prompt the user for a filename
    CFileDialog dlg(FALSE, _T("grf"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    dlg.m_ofn.lpstrTitle = _T("Select Geometry File");
    
    dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();
    // Show the dialog

    if (theApp.DoFileDialog(dlg) != IDOK) return;   
    PathHandler ph(dlg.m_ofn.lpstrFile);
    theApp.DocsDir(ph.Path());


    try{
        SaveInFile(dlg.m_ofn.lpstrFile);
        SetTitle(dlg.m_ofn.lpstrFile);
        _docDirty=0;
    }
    catch(int& i)
    {
        i;
        AfxMessageBox(_lasterror.c_str(), MB_OK|MB_ICONWARNING);
        _lasterror.erase();
    }
    catch(...)
    {
        AfxMessageBox("Unknown Exception while Saving", MB_OK|MB_ICONHAND);
    }
	REDIR();
#endif //
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnEditPaste() 
{
    SelModeSave sms(_brmode,_selmode);

    DeselectAll();
	if(_pClipboard.size())
	{
        SBT(0, "Pasted OK.");

		FOREACH(PBrushes, _pClipboard, ppb)
		{
			Brush* pB       = *ppb;

            if(pB->_brushflags & BRSH_WIREBOX ||
                pB->_brushflags & BRSH_REF)
                continue;

			Brush* pbNew    = new Brush(*pB);
			pbNew->_pMotion = 0;
            pbNew->_brushflags |=  BRSH_NEW;
			pbNew->_brushflags &= ~BRSH_HASTRIG; //has no trigger copy 
            pbNew->Recalc();
            SelectBrush(pbNew);
		}
        Invalidate();
        return;
    }
    if(_pClipboardItm.size())
    {
        FOREACH(vvector<SceItem*>, _pClipboardItm, pps)
		{
            switch((*pps)->_item)
            {
                case ITM_LIGTBULB:
                {
                    CLightBulb* plb  = new CLightBulb(*((CLightBulb*)(*pps)));
                    plb->_flags      |= BRSH_NEW;
                    plb->_group      = _curCreatGroup;
                    SelectItem(plb);
                }
                break;
            }
		}
        Invalidate();
        return;
	}

    Beep(400,100);
    SBT(0, "Clipboard Is Empty!");
}

void CZ_ed2Doc::OnBushowback() 
{
     _viewChanged=3;    

	_vmodeback=!_vmodeback;

	Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateBushowback(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

	pCmdUI->SetCheck(_vmodeback);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnBuwireframe() 
{
	_vmodefire =! _vmodefire;
    _viewChanged = 3;

	Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateBuwireframe(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

	pCmdUI->SetCheck(_vmodefire);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DeleteContents() 
{	
    _clearing = 1;
    if(!_closing)
    {
        
 	    DeselectAll();
	    _selBox.Clear();
        _compiler.Clear();
        _scene.Clear();
        _pActiveView   = 0;
	    _exViewMode	   = 0;
	    _vmodefire	   = FALSE;
	    _vmodeback     = FALSE;
        _bShowCsg      = FALSE;
        _compviewmode  = C_NA;
        
        _maxUsedGrp       = 0;
        Brush::GID        = 0;
        _use2splitidx       = -1;
        SceItem::_GItemID = 0;
        _undo.Clear();

	    EndEditor();

        if(DOC()->_viewCount)
            Ta.Disable();
        TexRef::Clear();
	    if(_closing  == FALSE)
	    {
            BARDLG()->Rescan();
		    BARDLG()->ResetDlgsContent();
	    }
    }
    CDocument::DeleteContents();

    _clearing = 0;
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnBushowcsg() 
{
    _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

    _compviewmode = C_NA;

    _brmode       = BR_VIEW;
    _bShowCsg=!_bShowCsg;
    _viewChanged=3;
    DeselectAll();
    SetFaceMode(1);
    Invalidate();
}
 
//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateBushowcsg(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    if(_scene.GetBrushes()->size()==0)
    {
        pCmdUI->SetCheck(0);
        pCmdUI->Enable(FALSE);
    }
    else
	    pCmdUI->SetCheck(_bShowCsg);
}

//---------------------------------------------------------------------------------------
void	CZ_ed2Doc::Recut(vvector<Brush*>* extraBrushes)
{
    int         nCount = 0;
	Brush** ppb = GetSelBrushes(nCount);
    if(nCount)
    {
	    UpdateCutsCollisionList(ppb, nCount);
    }

    if(extraBrushes )
    {
        nCount = extraBrushes->size();
        if(nCount)
        {
            ppb    = &((*extraBrushes)[0]);
            UpdateCutsCollisionList(ppb, nCount);
        }
    }

    DOC()->_polycount = _scene.GetPolyCnt();
    SBT(0,MKSTR("Scene has %d Polygons", DOC()->_polycount));	

#ifdef _DEBUG    
/*
	FOREACH(PBrushes, (*_scene.GetPrimitives()), ppbb)
	{
		Brush* pB =*ppbb;

		if(pB->_flags & BRSH_SOLID)
		{

			TRACE("Brush %x (%d) has %d cuts \r\n", pB, pB->_unicID, pB->_pCutBrshes.size());
			FOREACH(PBrushes, pB->_pCutBrshes, ppc)
			{
				TRACE("Cut %x(%d), ", (*ppc), (*ppc)->_unicID);
			}
			TRACE("\r\n");

			TRACE("Brush %x (%d) has %d brushes", pB, pB->_unicID, pB->_pSldBrushes.size());
			FOREACH(PBrushes, pB->_pSldBrushes, ppc)
			{
				TRACE("Brush %x(%d), ", (*ppc), (*ppc)->_unicID);
			}

		}
		else
		{
			TRACE("Cut %x (%d) has %d brushes", pB, pB->_unicID, pB->_pSldBrushes.size());

			FOREACH(PBrushes, pB->_pSldBrushes, ppc)
			{
				TRACE("Brush %x(%d), ", (*ppc), (*ppc)->_unicID);
			}
			TRACE("\r\n");

			TRACE("Cut %x (%d) has %d cuts \r\n", pB, pB->_unicID, pB->_pCutBrshes.size());
			FOREACH(PBrushes, pB->_pCutBrshes, ppc)
			{
				TRACE("Cut %x(%d), ", (*ppc), (*ppc)->_unicID);
			}
			TRACE("\r\n");
		}
	}
    */
#endif //

}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(HasSomeSelected() );
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateCutBrush(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateCut(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(HasSomeSelected());	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_pClipboard.size()>0 || _pClipboardItm.size()>0);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateDelete(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(HasSomeSelected());	
}

//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::HideSelBox()
{
    _selBox._brushflags &= ~BRSH_SELVIS;
    Invalidate();
    SBT(0,"Edit Mode");
}
//returns the minimum projection in 2d projection views
V3   CZ_ed2Doc::GetViewXYZWidths()
{
    V3 dims;
    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,1);

    dims.x  =   fabs(pTZ->_vport.right - pTZ->_vport.left);
    REAL x2 =   fabs(pTY->_vport.right - pTY->_vport.left);
    dims.x = tmin(dims.x,x2);
    
    dims.y  =   fabs(pTZ->_vport.top   - pTZ->_vport.bottom);
    REAL y2 =   fabs(pTX->_vport.top   -  pTX->_vport.bottom);
    dims.y = tmin(dims.y,y2);

    dims.z =   fabs(pTY->_vport.top   - pTZ->_vport.bottom);
    REAL z2 =  fabs(pTX->_vport.right - pTX->_vport.left);
    dims.x = tmin(dims.x,z2);

    return dims;
}

//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::CreateSelBox(CPoint& pt)
{
    DeselectAll();
    
    _pClipboard.clear();
    _pClipboardItm.clear();
    

    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,1);

    V3 center = _selBox._box.GetCenter();
    V3 bbxyz  = _selBox._box.GetExtends();
    V3 dims   = GetViewXYZWidths();

    _selBox.Scale(dims / bbxyz, V0, 1);
    _selBox.Move(V3(pTY->_shiftx  - center.x, 
                    pTZ->_shifty  - center.y, 
                    -pTY->_shifty - center.z));
    
	_selBox.Recalc();
    _selBox._brushflags |= BRSH_SELVIS;



    Invalidate();
    SBT(0,"Selection Mode with Box");
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::Escape(BOOL resetSels)
{
    if(_selmode == SEL_NONE)    
    {
        _brmode = BR_VIEW;
    }

    VIEW2D(1,1)->_accmode = BR_VIEW;
    VIEW2D(1,0)->_accmode = BR_VIEW;
    VIEW2D(0,1)->_accmode = BR_VIEW;
    _pLastMakedBrush = 0;
    if(IsSelBoxOn())
    {
        _selBoxByTouch = FALSE;
        HideSelBox();
        return;
    }

    DeleteUncomittedNewBrushes();
	DeleteUncomittedNewItems();
    DeselectAll();
    
    _pClipboard.clear();
    _pClipboardItm.clear();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnEscape() 
{
    _allselected=0;
    Escape();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnEditCopy() 
{
    SelModeSave sms(_brmode,_selmode);

    DeleteUncomittedNewBrushes();
    DeleteUncomittedNewItems();

	if(_pSelBrushes.size())
	{
AGAIN1:
        FOREACH(PBrushes,  _pSelBrushes, ppb)
        {
            if((*ppb)->IsSelection())
            {
                _pSelBrushes.erase(ppb);
                goto AGAIN1;
            }
        }

    	_pClipboard = _pSelBrushes;
	    DeselectAll();
        SBT(0, MKSTR("%d Brushes Copyed", _pSelBrushes.size()));
	}
    else if(_pSelItems.size())
    {
        _pClipboardItm = _pSelItems;

        DeselectAll();
        SBT(0, MKSTR("%d Items Copyed", _pSelItems.size()));
    }
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnCut() 
{
    OnEditCopy() ;
    OnDelete();
}

BOOL CZ_ed2Doc::DeleteBrush(Brush* pBrush, BOOL undo)
{
    if(0 == pBrush) 
        return 0;

    if(pBrush->IsSelection() || pBrush->IsProjector() || pBrush->IsSplitter() ||
        pBrush->_brushflags & BRSH_BIGTERRAIN)
        undo=0;

    if(undo){
        _undo.Add(pBrush,M_DELETED, Gsec);
        ++Gsec;
    }

    PBrushes* pBSs = GetForms();
    FOREACH(PBrushes, (*pBSs), ppB)	// find the brush in the collection
    {
		if((*ppB)!=pBrush)			continue;

        DeleteBrushItemLinks(*ppB);
		if((*ppB)->IsSolid() )	
		{
			// dereference the cut brushes 
			FOREACH(PBrushes, pBrush->_pCutBrshes, ppCB)
			{
				// _pSelBrushes.push_back(*ppCB); 
				(*ppCB)->RemoveBrush(pBrush);
				(*ppCB)->UpdateSolidsTouches();
			}
			pBSs->erase(ppB);
            break;
		}
        if(pBrush->IsProjector())
        {
            this->CleanProjections(*pBrush, GKeepProjections);
            pBSs->erase(ppB);
            break;
        }
		else 			
		{
			FOREACH(PBrushes, (*ppB)->_pSldBrushes, ppSB)
			{
				////_pSelBrushes.push_back(*ppSB); 
				(*ppSB)->RemoveCutter(pBrush);
				(*ppSB)->UpdateCutsTouches();
                (*ppSB)->ApplyCuts();
			}
			pBSs->erase(ppB);
			break;
		}
    }
    //if(!undo)
    //{
    delete pBrush;
    return 1;
    //}
    ////return 0;
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnDelete() 
{
    if(!_userBool)
    {
        DeleteUncomittedNewBrushes();
        DeleteUncomittedNewItems();
    }
    BOOL b = FALSE;
    _pClipboard.clear();
    _pClipboardItm.clear();

    if(_selmode == SEL_BRUSH )
    {
AGAIN1:
        FOREACH(PBrushes,  _pSelBrushes, ppb)
        {
            b |= 1;
            DeleteBrush(*ppb);
            _pSelBrushes.erase(ppb);
            goto AGAIN1;
        }
        if(b)
        {
            Recut();
            DeselectAll();
        }
        Invalidate();
        return ;
    }

    if(_selmode == SEL_ITEMS)
    {
AGAIN2:
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
            b |= TRUE;
            SceItem*  pItem  = *ppItem;
            _pSelItems.erase(ppItem);
            DelItem(pItem);
            goto AGAIN2;
        }
        DeselectItems();                        // triger bar update
        if(b)
        {
            DeselectAll();
        }
    }
    
    if(_selmode == SEL_FACE)
    {
        set<Brush*> brushes;                    // touched brushes
AGAIN:
        FOREACH(PPolys,  _pSelPolys, ppp)       // selected polys
        {
            Poly& p2f =*(*ppp);
            brushes.insert(p2f._pBrush);        // store the touched brush

            _pSelPolys.erase(ppp);
            p2f._pBrush->ErasePoly(p2f);
            
            goto AGAIN;
        }
        DeselectPolys();                            // deselect them (clean the list)
        FOREACH(set<Brush*>, brushes, pb)
        {
            (*pb)->Recalc();
            SelectBrush(*pb);
            b = TRUE;
        }
    }    
    
    if(b)
    {
        Recut();
        DeselectAll();
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DoBoxSelSelection()
{
    HideSelBox();
    DeselectAll();
    // select anu brush cinside this box or touching this box
    _selBox.Recalc();
    PBrushes* pBSs = GetPrimitives();

    if(pBSs->size())
    {
        FOREACH(PBrushes, (*pBSs), ppB)    
        {
            if(_curgroup >= 0 && (*ppB)->_group != _curgroup)
                continue;

            if(_selBox._box.ContainPoint((*ppB)->_box._min) && 
		       _selBox._box.ContainPoint((*ppB)->_box._max))
            {
			    SelectBrush(*ppB);
            }
        }
        Invalidate();
    }
    
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateMirror(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size())	;
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnEnter() 
{
  //  SaveTempFile();

    BOOL    recut = TRUE;
    if(0==_pSelBrushes.size() && 0 == _pSelItems.size())
        return;
	BARDLG()->OnApply();

    PBrushes  pSelBrushes = _pSelBrushes;
    
    if(pSelBrushes.size())
	{
        SBT(0, MKSTR("%d Brushes Commited", pSelBrushes.size()) );

		FOREACH(PBrushes,  pSelBrushes, ppb)
		{
			if((*ppb)->_brushflags & BRSH_NEW)           //new brushes just move them in scene
			{
                // keep last bouding box ex for next created brush
				(*ppb)->_brushflags &= ~BRSH_NEW;
				(*ppb)->Dirty(0);
				(*ppb)->Recalc();

                if((*ppb)->IsSplitter())
                {
                    SplitBySplitterBrush(*ppb);
                    delete (*ppb);
                    recut = TRUE;
                    break; //only one can be
                }
                /*
                else if((*ppb)->IsZone())
                {
                    // add to zones
                }
                else
                {
                */
                    /**
				    if(GDetEnabled && (*ppb)->IsSolid())
				    {
					    EvaluateDetailbrsh((*ppb));
				    }
                    */
				    _scene.AddPrimitive((*ppb));
				    CameraPointTo((*ppb)->_box.GetCenter());
                //}
			}
			else if((*ppb)->IsDirty())				// moved scaled brushes, just commit them
			{
				(*ppb)->Recalc();
                /*
				if(GDetEnabled && (*ppb)->IsSolid())
				{
					EvaluateDetailbrsh((*ppb));
				}
                */
			}
		}
        if(recut )
		    Recut();
        BARDLG()->OnBrushesSelected();
	}

	if(_pSelItems.size())
	{
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
			if((*ppItem)->_flags & BRSH_NEW)           
			{
				(*ppItem)->_flags &= ~BRSH_NEW;
				
                _scene.AddItem(*ppItem);
				BARDLG()->AddItem(*ppItem);
				BARDLG()->Clean();
                BARDLG()->OnItemSelected(*ppItem);
				CameraPointTo((*ppItem)->_t);
			}
        }
        //BARDLG()->OnItemsSelected();

        SBT(0, MKSTR("%d Items Commited", _pSelItems.size()) );
	}
	Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateEnter(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
}

//---------------------------------------------------------------------------------------
BOOL    CZ_ed2Doc::SelectBrush(Brush* pB)
{
    if(_pSelBrushes.push_unique(pB))
    {
        pB->Select();

        if(_bSnapGrid)
        {
            int x = pB->_t.x; x/=SnapToXY; x*=SnapToXY;
            int y = pB->_t.x; y/=SnapToXY; y*=SnapToXY;
            int z = pB->_t.y; z/=SnapToXY; z*=SnapToXY;

            pB->_t.x = x;
            pB->_t.y = y;
            pB->_t.z = z;
        }


        if(pB->_pMotion)
        {
            pB->_pMotion->SelectNode(0);
        }
        return 1;
    }
    return 0;
}


//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::DeSelectBrush(Brush* pB)
{
    if(_pSelBrushes.erase_if(pB))
    {
		if(pB->_pMotion)
			pB->_pMotion->_selNode=0;
        pB->DeSelect();
        pB->Dirty(0);
    }
}

//---------------------------------------------------------------------------------------
BOOL    CZ_ed2Doc::SelectFace(Poly* pP, Brush* pbr)
{
    if(!pP)
        return 0;
    if(!pbr)
        pbr = pP->_pBrush;
    
    if(pbr->IsSelection() )
        return 0;

    if(_pSelPolys.push_unique(pP))
    {
        pP->Select();
        //??
        return 1;
    }
    return 0;
}

//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::DeSelectFace(Poly* pB)
{
    if(_pSelPolys.erase_if(pB))
    {
        pB->DeSelect();
    }
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnFinalRelease() 
{
	CDocument::OnFinalRelease();
}

//---------------------------------------------------------------------------------------
//void    CZ_ed2Doc::OnStartMoveRotScale(int operation, SceItem** ppItems, int count)
void    CZ_ed2Doc::OnStartMoveRotScale(int operation, vvector<SceItem*>& items)
{
    static int saequence = 0;
    if(_undodirt==0)
    {
        _undodirt=1;
        vvector<SceItem*>::iterator b =  items.begin();
        for(; b != items.end(); b++)
        {
           _undo.Add(*b, operation, Gsec);
        }
        ++Gsec;
    }
}

//---------------------------------------------------------------------------------------
//void    CZ_ed2Doc::OnStartMoveRotScale(int operation, Brush** ppBrushes, int count)
void    CZ_ed2Doc::OnStartMoveRotScale(int operation, vvector<Brush*>&  brushes)
{
    static int saequence = 0;
    if(_undodirt==0)
    {
        _undodirt=1;
        vvector<Brush*>::iterator b =  brushes.begin();
        for(; b != brushes.end(); b++)
        {
           _undo.Add(*b, operation, Gsec);
        }
        ++Gsec;
    }
}

//---------------------------------------------------------------------------------------
// reset them to initla valies to see last translation that happend
// I can undo last roattion or last translation because scale is not 
// acually a scalation
BOOL    CZ_ed2Doc::OnStartMoveRotScale()
{
    BOOL rv = 0;
    _docDirty = 1;
    if(_selmode == SEL_BRUSH || 
       _selmode == SEL_FACE || 
       _selmode == SEL_VERTEX ||
       _selmode == SEL_EDGE)
    {
        FOREACH(PBrushes,  _pSelBrushes, ppb)
        {
            (*ppb)->Dirty(0);
        }
        rv=1;
    }else if(_selmode == SEL_ITEMS ||
             _selmode == SEL_BR_ITEM)
    {
        FOREACH(vvector<SceItem*>, _pSelItems, ppi)
        {
            (*ppi)->Dirty(0);
        }
        rv=1;

    }
    _undodirt=0;

    OnStartMoveRotScale(OBJ_CHANGED, _pSelBrushes);// _pSelBrushes.size())
    OnStartMoveRotScale(OBJ_CHANGED, _pSelItems);//, _pSelBrushes.size())


    return rv;
}

//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::OnFinishMoveRotScale(int dirty, vvector<Brush*>* pBrushes)
{ 
    if(dirty == 0)
        return;

    DWORD           dw = GetTickCount();
	vvector<Brush*>	extraToAdjust;
    int             is = 0;
    int             id = 0;

    _TRY{
        if(M_SCALED == dirty || M_MOVED == dirty || M_ROTATED == dirty)
        {

            if(_pSelBrushes.size())
            {
                BARDLG()->OnBrushesSelected();  
                FOREACH(vvector<Brush*>, _pSelBrushes, ppB)
                {
                    
                    if(_bSnapGrid)
                    {
                        RoundToNear((*ppB)->_t);
                    }

                    (*ppB)->Recalc();
                    if((*ppB)->_brushflags & BRSH_NEW)           
                        continue;
                    is++;                           // det cuting
                    UpdateBrushLinks(*ppB);
                }
            }
            if(pBrushes)
            {
                append(extraToAdjust,(*pBrushes));
            }
       
            if(_pSelPolys.size()) // collect
            {
                set<Brush*>  brushes;    
                ::GetBrushes(_pSelPolys, brushes);
                append(extraToAdjust, brushes);
            }

            if(_pSelVertexes.size())// collect
            {
			    ModifyBrushes(extraToAdjust);
                is++;                           // det cuting
		    }            

            FOREACH(PBrushes, extraToAdjust, ppb)// update
            {
                (*ppb)->Dirty(1);
                (*ppb)->Recalc();
                UpdateBrushLinks(*ppb);
                is++;                           
            }

            if(is)
            {
                Recut(&extraToAdjust);
            }
        }
        else if(ITM_MOVED  == dirty || 
                ITM_SCALED == dirty || 
                ITM_ROTATED == dirty )
        {
            // update the bar dialog showing the first selected item
            int count = 0;
            vvector<SceItem*>*  pTemsPtr = GetItems();
            FOREACH(vvector<SceItem*>, (*pTemsPtr), ppItem)
            {
                if((*ppItem)->IsSelected())
                {

                    if(_bSnapGrid)
                    {
                        RoundToNear((*ppItem)->_t);
                    }

                    (*ppItem)->Commit();
                    if(0==count)
                    {
                        BARDLG()->OnItemPropsChanged((*ppItem));
		                ++count;
                    }
                }
            }
        }

        if(GLight)
        {
            z_ed3View* pT3D   = VIEW3D(0,0);
            pT3D->RedrawDC(1);
        }
        Invalidate();
    }_CATCHX()
    {
        ;
    }

    if(GetTickCount() - dw  > 2000)
    {
        SBT(0," Quite many Items to process  !");
        Beep(600,32);
    }
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnEditUndo() 
{
    _docDirty = 1;
    OnUndo();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }

	pCmdUI->Enable(1);
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnTabPrevsel() 
{
	if(_selmode == SEL_NONE)
		return;
	SelectTabBrush(-1, 0);				//walk backward
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnTabNextsel() 
{
    if(_compviewmode  != C_NA)//
    {
        _viewChanged=3;    
		if(_compviewmode & C_LBYL)
        {
			_leafNo++;
        }
		_exViewMode++;
		Invalidate();
		//(VIEW3D(0,0))->InvalidateRect(0);
        Invalidate();
        return;
    }
    if(_selmode == SEL_NONE)
		return;
    SelectTabBrush(1, 0);				//walk forwards
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnTabSelnext() 
{
    if(_selmode == SEL_NONE)    
		return;
    SelectTabBrush(1, 1);				//walk forwards
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnTabSelprev() 
{
    if(_compviewmode & C_LBYL)
    {
        _viewChanged = 3;
        _leafNo--;
        return;
    }
    if(_selmode == SEL_NONE)
		return;
   	SelectTabBrush(-1, 1);				//walk forwards
}

//---------------------------------------------------------------------------------------
void	 CZ_ed2Doc::SelectTabBrush(int direction, BOOL cumulate)
{
    HWND hwnd = ::GetFocus();

	if(_selmode == SEL_BRUSH)
	{
        if(_pSelBrushes.size())
        {

			Brush* pB;

			if(direction>0)                 // step forward
				pB = _pSelBrushes.back();   // get tail
			else
				pB = _pSelBrushes[0];       // get head

            // search the brush pos in the scene brushes
            PBrushes::iterator fb = find(_scene.GetPrimitives()->begin(), 
                                         _scene.GetPrimitives()->end(),pB);
            
            // if found
			if(fb != _scene.GetPrimitives()->end())
			{
				if(direction>0)
				{
					++fb;
					if(fb == _scene.GetPrimitives()->end())
                    {
						fb=_scene.GetPrimitives()->begin();
                    }
				}
				else
				{
					if(fb == _scene.GetPrimitives()->begin())
					{
						fb = _scene.GetPrimitives()->end();
					}
					--fb;
				}
			}
			if(cumulate==0)
			{
				DeselectBrushes();
			}
            if(fb!=_scene.GetPrimitives()->end())
            {
			    SelectBrush(*fb);
            }
            BARDLG()->OnBrushesSelected();
        }
	}
	else if(_selmode == SEL_FACE) // next face in this brush then roll
    {                             // again in this brush
        if(_pSelPolys.size())
        {
            
			Poly* pP;
			
			if(direction>0)
				pP = _pSelPolys.back();
			else
				pP = _pSelPolys[0];

            Brush* pB = pP->_pBrush;

            // find the selected poly in the brush
            vvector<Poly>::iterator pPly = pB->_polys.begin();
            vvector<Poly>::iterator be = pB->_polys.end();
            for(; pPly != be; )
            {
                Poly& rP = *pPly;
                if(&(*pPly) == pP)
                {
					if(direction > 0)
					{
						++pPly;
						if(pPly == pB->_polys.end())
						{
							pPly = pB->_polys.begin();//roll aup
						}
					}
					else
					{
						if(pPly==pB->_polys.begin())
							pPly=pB->_polys.end();
						--pPly;
					}
                    break;
                }
                pPly++;
            }
			if(!cumulate)
				DeselectPolys();
            if(pPly != pB->_polys.end())
            {
                Poly& rP = *pPly;
                SelectFace(&rP, pB);
                BARDLG()->OnFacesSelected();
            }
        }
	}
    else if(_selmode == SEL_VERTEX)
    {
        if(_pSelVertexes.size() && _pSelVertexes.size() == _pSelPolys.size())
        {
            Vtx2 *pvx   = _pSelVertexes[0]; 
            Poly* pPoly = pvx->_pPoly;

            DeselectVertexes();

            ASSERT(pPoly);
            if(pPoly)
            {
                vvector<Vtx2>::iterator pvtx;
                FOREACH(vvector<Vtx2>, pPoly->_vtci, pV)
                {
                    if(pV->_xyz == pvx->_xyz)
                    {
                        pvtx = pV;
                        break;
                    }
                }

                if(pvtx == pPoly->_vtci.end()-1)
                    pvtx=pPoly->_vtci.begin();
                else
                    pvtx++;
                SelectAllBrushVertexes(*pPoly->_pBrush, (*pvtx));
            }
            
        }
    }
    if(_selmode == SEL_EDGE)
    {
        if(_selEdges.size())
        {
            PolyEdge& curEdge = _selEdges[0];
            Poly*     pNextPoly = 0;  

            // curEdge._poly->_pBrush
            int i1 = curEdge._poly->GetVxIndex(*curEdge._edge[0]);
            int i2 = curEdge._poly->GetVxIndex(*curEdge._edge[1]);

            if(direction>0) // forward
            {
                if(i2==0) //switch to next poly in brush
                {
                    DeSelectFace(curEdge._poly);
                    curEdge._poly = curEdge._poly->_pBrush->GetNextPoly(curEdge._poly);
                    SelectFace(curEdge._poly);
                    
                    i1=0;
                    i2=1;
                }
                else
                {
                    ++i1;
                    if(++i2 >=  curEdge._poly->_vtci.size())
                        i2=0;
                }
            }
            else //backward
            {
                if(--i1==0)
                    i1=curEdge._poly->_vtci.size()-1;
                if(--i2==0)
                {
                    // switch to prev poly
                    DeSelectFace(curEdge._poly);
                    curEdge._poly = curEdge._poly->_pBrush->GetPrevPoly(curEdge._poly);
                    SelectFace(curEdge._poly);
                    i1 = curEdge._poly->_vtci.size()-2;
                    i2 = curEdge._poly->_vtci.size()-1;
                }
            }
            
            curEdge._edge[0] = &curEdge._poly->_vtci[i1];
            curEdge._edge[1] = &curEdge._poly->_vtci[i2];

        }
    }
    
    else if(_selmode == SEL_ITEMS)
    {
        if(_pSelItems.size())
        {
            vvector<SceItem*>*  pSi = _scene.GetItems();
		    SceItem*            pB;

            // get first or last selected item
		    if(direction>0)
			    pB = _pSelItems.back();
		    else
			    pB = _pSelItems[0];

            // search the item pos in the scene brushes
            vvector<SceItem*>::iterator fb = find(pSi->begin(), pSi->end(), pB);

		    if(fb != pSi->end())
		    {
			    if(direction>0)
			    {
				    ++fb;
				    if(fb == pSi->end())
					    fb = pSi->begin();
			    }
			    else
			    {
				    if(fb == pSi->begin())
				    {
					    fb = pSi->end();
				    }
				    --fb;
			    }
		    }
		    if(cumulate==0)
		    {
			    DeselectItems();
		    }
            if(fb != pSi->end())
            {
			    SelectItem(*fb);
            }
        }
    }

    if(hwnd != ::GetFocus())
    {
        ::SetFocus(hwnd);
    }

	Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnCompbsp() 
{
    SaveTempFile();

	BARDLG()->Dirty();
	BARDLG()->OnApply();

    vvector<Poly>    allpolys;

    FOREACH(PBrushes,  _pSelBrushes, psb)
    {
        Brush* pB = *psb;
        FOREACH(vvector<Poly>,pB->_polys,pp)
            allpolys.push_back(*pp);
    }

    if(allpolys.size())
    {
        Brush* pB = _pSelBrushes[0];
        MiniBsp mini;
        if(!mini.Compile(allpolys, pB->_brushflags))
        {
            AfxMessageBox("Error compiling BSP for the selected Brush", MB_OK|MB_ICONWARNING);
        }
    }
}

void CZ_ed2Doc::OnZones() 
{
    DeselectAll();
    _brmode    = BR_VIEW;
    if(_compviewmode == C_ZONES)
    {
        _compviewmode = C_NA;
        return;
    }
	_compviewmode = C_ZONES;
    Invalidate();
	
}

void CZ_ed2Doc::OnUpdateZones(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(_compiler.Done()!=0);
	pCmdUI->SetCheck(_compviewmode == C_ZONES);
}



void CZ_ed2Doc::OnViewGraph() 
{
    SetFaceMode(1);

	 DeselectAll();
    _brmode    = BR_VIEW;
    if(_compviewmode == C_GRAPH)
    {
        _compviewmode = C_NA;
        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
	    _cam._pos =_camsave[0];
        _cam._euler =_camsave[1];
	    _cam.Euler2Pos();

        Invalidate();
        return;
    }
     _camsave[0] = _cam._pos;
    _camsave[1] = _cam._euler;
    _cam.SetAngles(0,0,0);
	_cam.Euler2Pos();
    _compviewmode = C_GRAPH;
    Invalidate();
}

void CZ_ed2Doc::OnUpdateViewGraph(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_compiler.Done()!=0);
	pCmdUI->SetCheck(_compviewmode == C_GRAPH);
}


void CZ_ed2Doc::OnViewLeafByLeaf() 
{
    SetFaceMode(1);
    _selmode = SEL_NONE;
    VIEW2D(1,1)->_accmode = BR_VIEW;
    VIEW2D(1,0)->_accmode = BR_VIEW;
    VIEW2D(0,1)->_accmode = BR_VIEW;

    DeselectAll();

    _compviewmode &= ~(C_FULL|C_LMAP|C_GRAPH|C_LEAFS|C_ZONES);

    if(_compviewmode & C_LBYL)
    {
        _compviewmode &= ~C_LBYL;

        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
    }
    else
    {
        _compviewmode |= C_LBYL;
    }

    _viewChanged = 3;
    _leafNo      = 0;
    Invalidate();
}

void CZ_ed2Doc::OnUpdateViewLeafByLeaf(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_compiler.Done()!=0);
    pCmdUI->SetCheck(!!(_compviewmode & C_LBYL));
}

void CZ_ed2Doc::OnViewLightmaps() 
{

    SetFaceMode(1);

    _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

    DeselectAll();

    _compviewmode &= ~(C_FULL|C_LBYL|C_GRAPH|C_LEAFS|C_ZONES);

    if(_compviewmode & C_LMAP)
    {
        _compviewmode &= ~C_LMAP;
    }
    else
    {
        _compviewmode |= C_LMAP;

        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
    }

    _viewChanged = 3;
    _leafNo       = 0;
    
    Invalidate();
	
}

void CZ_ed2Doc::OnUpdateViewLightmaps(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_compiler.Done()!=0);
    pCmdUI->SetCheck(!!(_compviewmode & C_LMAP));	
	
}

void CZ_ed2Doc::OnViewFullBSP() 
{
    SetFaceMode(1);
    _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

    DeselectAll();
   
    _compviewmode &= ~(C_LEAFS|C_LBYL|C_LMAP|C_GRAPH|C_ZONES);

    if(_compviewmode & C_FULL)
    {
        _compviewmode &= ~C_FULL;
    }
    else
    {
        _compviewmode |= C_FULL;

        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
    }
    _viewChanged = 3;
    Invalidate();
}

void CZ_ed2Doc::OnUpdateOnViewFullBSP(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_compiler.Done()!=0);
    pCmdUI->SetCheck(!!(_compviewmode & C_FULL));
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnViewPortals() 
{
    SetFaceMode(1);

    _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

    DeselectAll();

    _compviewmode &= ~(C_GRAPH);
    if(_compviewmode & C_PORT)
    {
        _compviewmode &= ~C_PORT;
    }
    else
    {
        _compviewmode |= C_PORT;

        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
    }
    _viewChanged = 3;
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateOnViewPortals(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_compiler.Done()!=0);
    pCmdUI->SetCheck(!!(_compviewmode & C_PORT));
}

void CZ_ed2Doc::OnViewWhite()
{
    SetFaceMode(1);

    _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

    DeselectAll();
    
    if(_compviewmode & C_WHITE)
    {
        _compviewmode &= ~C_WHITE;
    }
    else
    {
        _compviewmode |= C_WHITE;

        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
    }
    _viewChanged = 3;
    Invalidate();

}

void CZ_ed2Doc::OnViewByFar()
{
    SetFaceMode(1);
    _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

    DeselectAll();
   
    if(_compviewmode & C_BYFAR)
    {
        _compviewmode &= ~C_BYFAR;
    }
    else
    {
        _compviewmode |= C_BYFAR;

        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
    }
    _viewChanged = 3;
    Invalidate();

}

void CZ_ed2Doc::OnViewByFarUpdate(CCmdUI* pCmdUI)
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_compiler.Done()!=0 && _compviewmode!=0);
    pCmdUI->SetCheck(!!(_compviewmode & C_BYFAR));
}



void CZ_ed2Doc::OnViewWhiteUpdate(CCmdUI* pCmdUI)
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_compiler.Done()!=0 && _compviewmode!=0);
    pCmdUI->SetCheck(!!(_compviewmode & C_WHITE));
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnViewColorLeafs() 
{
    SetFaceMode(1);

    _selmode = SEL_NONE;
        VIEW2D(1,1)->_accmode = BR_VIEW;
        VIEW2D(1,0)->_accmode = BR_VIEW;
        VIEW2D(0,1)->_accmode = BR_VIEW;

    DeselectAll();

    _compviewmode &= ~(C_GRAPH|C_LBYL|C_FULL|C_LMAP|C_ZONES);
    if(_compviewmode & C_LEAFS)
    {
        _compviewmode &= ~C_LEAFS;
    }
    else
    {
        _compviewmode |= C_LEAFS;

        if(::IsWindow(FRM()->_dlgleafs.m_hWnd))
        {
            FRM()->DestroyLeafExplorer();
        }
    }
    _viewChanged = 3;
    Invalidate();
}



//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateOnViewColorLeafs(CCmdUI* pCmdUI) 
{
    if(_compiling)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_compiler.Done()!=0);
    pCmdUI->SetCheck(!!(_compviewmode & C_LEAFS));
}


void CZ_ed2Doc::OnSelnondetbrushes() 
{
	if(_selmode == SEL_BRUSH )
	{
        OnEscape();
		PBrushes* rbs = _scene.GetPrimitives();
		FOREACH(PBrushes, (*rbs), ppB)
		{
            if(_curgroup>=0)
            {
                if(_curgroup != (*ppB)->_group)
                    continue;
            }
            if((*ppB)->_brushflags & BRSH_DETAIL)
                continue;

			SelectBrush(*ppB);
		}
        BARDLG()->OnBrushesSelected();
	}
	
}

void CZ_ed2Doc::OnDetbrushes() 
{
	if(_selmode == SEL_BRUSH )
	{
        OnEscape();
		PBrushes* rbs = _scene.GetPrimitives();
		FOREACH(PBrushes, (*rbs), ppB)
		{
            if(_curgroup>=0)
            {
                if(_curgroup != (*ppB)->_group)
                    continue;
            }
            if(!((*ppB)->_brushflags & BRSH_DETAIL))
                continue;

			SelectBrush(*ppB);
		}
        BARDLG()->OnBrushesSelected();
	}

}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnSelectall() 
{
	SelModeSave sms(_brmode,_selmode);

    BOOL selAll = FALSE;
    
	if(_selmode == SEL_NONE)
    {
		return;
    }
    
	if(_selmode == SEL_BRUSH )
	{
        OnEscape();
        _allselected = 1;
		PBrushes* rbs = _scene.GetPrimitives();
		FOREACH(PBrushes, (*rbs), ppB)
		{
            if(_curgroup>=0)
            {
                if(_curgroup != (*ppB)->_group)
                    continue;
            }

			SelectBrush(*ppB);
		}
        BARDLG()->OnBrushesSelected();
        
        if(AKEY('g') || AKEY('G'))
        {
            vvector<SceItem*>*  pTemsPtr = GetItems();
            FOREACH(vvector<SceItem*>, (*pTemsPtr), ppItem)
            {
                SceItem*  pItem  = *ppItem;

                if(_curgroup>=0)
                {
                    if(_curgroup != pItem->_group)
                        continue;
                }

                this->SelectItem(pItem);
            }
        }
	}
	else if(_selmode == SEL_FACE)
	{
        
        // select all faces
        if(_pSelPolys.size())
        {
			Poly*	pP = _pSelPolys[0];
			Brush*	pB = pP->_pBrush;
            OnEscape();    

            // find the selected poly in the brush
            vvector<Poly>::iterator pPly = pB->_polys.begin();
            vvector<Poly>::iterator be = pB->_polys.end();
            for(; pPly != be; pPly++)
            {
                Poly& rP = *pPly;
                //if(pP == &rP)
                //    continue;
				SelectFace(&rP, pB);
            }
        }
        BARDLG()->OnFacesSelected();
	}
	
	else if (_selmode == SEL_ITEMS )
    {
        OnEscape();    
        vvector<SceItem*>*  pTemsPtr = GetItems();
        FOREACH(vvector<SceItem*>, (*pTemsPtr), ppItem)
        {
            SceItem*  pItem  = *ppItem;

            if(_curgroup>=0)
            {
                if(_curgroup != pItem->_group)
                    continue;
            }
            this->SelectItem(pItem);
            BARDLG()->OnItemsSelected();
        }
        
    }
	Invalidate();
}

//---------------------------------------------------------------------------------------
//
//  selects all faces facing from center to this direction
//
void CZ_ed2Doc::OnSelsameasselface() 
{
    if(_pSelPolys.size())
    {
	    Poly*	pP     = _pSelPolys.back();
	    Brush*	pB     = pP->_pBrush;
        V3      bc2pc  = (pP->_box.GetCenter() - pB->_box.GetCenter()).norm();
        REAL    vndir  = Vdp(pP->_n, bc2pc);

        // find the selected poly in the brush
        vvector<Poly>::iterator pPly = pB->_polys.begin();
        vvector<Poly>::iterator be = pB->_polys.end();
        for(; pPly != be; pPly++)
        {
            Poly& rP = *pPly;
            if(pP == &rP)
                continue;

            V3      bc2pc  = (pPly->_box.GetCenter() - pB->_box.GetCenter()).norm();
            REAL    vndir2 = Vdp(pPly->_n, bc2pc);

            if(vndir2 * vndir > 0)
            {
		        SelectFace(&rP, pB);
            }
        }
    }
	Invalidate();	
}


//---------------------------------------------------------------------------------------
// reevaluates if this brush should be marked detail or not 
// baed on global settings, by size or ny number o polygons
void CZ_ed2Doc::EvaluateDetailbrsh(Brush* pb)
{
    if(pb->_brushprops & MODEL_IS_SKYDOM)
        return;

    V3 ex = pb->_box.GetExtends();
    REAL    dimM = ex.getmax();
    REAL    dimm = ex.getmin();

    if(dimm <= (REAL)GMinDetDim && dimM < (REAL)GMaxDetDim)
    {
        pb->_brushflags |= BRSH_DETAIL;
        return;
    }
    
    if(pb->_polys.size() >= (UINT)GMinDetPolys)
    {
        pb->_brushflags |= BRSH_DETAIL;
        return;
    }
}


BOOL CZ_ed2Doc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	_cmdMsg=TRUE;
    BOOL b = CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
    //SbarShowCurMode();
    return b;
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DisableAll(BOOL bd)
{
    CMenu* cm  = FRM()->GetSystemMenu(FALSE);
    if(_compiling==TRUE)
        cm->EnableMenuItem(SC_CLOSE,MF_DISABLED|MF_GRAYED);
    else
        cm->EnableMenuItem(SC_CLOSE,MF_ENABLED);
}

//---------------------------------------------------------------------------------------
// selecta ll brushes from the group as current selected brush
void CZ_ed2Doc::OnSelectgroup() 
{
	
}

//---------------------------------------------------------------------------------------
// selects all brushes having the same index as the curent selected 
void CZ_ed2Doc::OnSelcurid() 
{
	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateSelcurid(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateSelectgroup(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateSelsameasselface(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable((_pSelPolys.size()) && _selmode==SEL_FACE);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateSelectall(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_scene.GetPrimitives()->size()!=0 || _scene.GetItems()->size()!=0);
}

void CZ_ed2Doc::OnBureverse() 
{
    int nCount;
    Brush** b = GetSelBrushes(nCount);
    if(nCount)
    {
        for(int i=0;i<nCount;i++,b++)
        {
            (*b)->Reverse();
            UpdateBrushLinks(*b);
        }
        
        Recut();
    }
    else
    if(_pSelPolys.size())
    {
        set<Brush*> pbrushes;
	    FOREACH(PPolys,  _pSelPolys, a)
	    {
		    Poly   *p = (*a);
            pbrushes.insert(p->_pBrush);
            p->Reverse();
	    }
        FOREACH(set<Brush*>,pbrushes,ppb)
        {
            SelectBrush(*ppb);
            UpdateBrushLinks(*ppb);
        }
        Recut();
        DeselectBrushes();
    }
    Invalidate();
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateBureverse(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_pSelBrushes.size()||_pSelPolys.size());
	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateBucsg(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling|| _compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(_scene.GetPrimitives()->size());
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnCtx2dRotatecw30() 
{
    RotateByAngleCw(PIPE6);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnCtx2dRotatecw60() 
{
    RotateByAngleCw(PIPE6*2);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnCtx2dRotatecw90() 
{
    RotateByAngleCw(PIPE2);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnCtx2dRotatecw45() 
{
    RotateByAngleCw(PIPE4);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::RotateByAngleCw(REAL rad)
{
	/*
    V3  vrot;
    if(_pActiveView == VIEW2D(1,0))
    {
    }
    else if(_pActiveView == VIEW2D(0,1))
    {
    }
    else if(_pActiveView == VIEW2D(1,1))
    {
    }
	*/
}

BOOL CZ_ed2Doc::AddItem(SceItem* pItem)
{
    _docDirty = 1;
	pItem->_flags|= BRSH_NEW;
    pItem->_group = _curCreatGroup;
    DeselectItems();
	SelectItem(pItem);
	_selmode = SEL_ITEMS;
    return 1;
}

void CZ_ed2Doc::DelItem(SceItem* pItem)
{
    _docDirty = 1;
    _undo.Add(pItem,M_DELETED,Gsec);
    ++Gsec;
    BARDLG()->DelItem(pItem);
    _scene.DelItem(pItem);
//    delete pItem;
    Invalidate();
}


BOOL CZ_ed2Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return TRUE;
}

BOOL CZ_ed2Doc::SaveModified() 
{
	return TRUE;
}

void CZ_ed2Doc::OnCloseDocument() 
{
    _clearing = 1;
	_viewChanged = 3;
	_closing = TRUE;
	DeselectAll();
	EndEditor();
    _clearing = 0;
    CDocument::OnCloseDocument();
}

void  CZ_ed2Doc::MoveCamera(V3& ptPos)
{
    V3  ditTo = _cam._pos - ptPos;
    ditTo.norm();
    
    V3 np = ptPos + ditTo * 100;
    _cam._pos = np;
    Invalidate();

}

void CZ_ed2Doc::OnAddModel() 
{
    if(_scene.GetBrushes()->size())
    {
        SavePrefFile();
    }
}

void  CZ_ed2Doc::DeleteBrushMotion(Brush* pB)
{
    _docDirty = 1;
    if(pB->_pMotion )
    {
        CMotion* pM = pB->_pMotion;
        _scene.DelMotion(pM);
        pB->_pMotion = 0;
    }
}

BOOL  CZ_ed2Doc::AddBrushMotion(Brush* _pCurBrush, CMotion* pNewMotion)
{
    _docDirty = 1;
    SaveTempFile();

    ASSERT(_pCurBrush->_pMotion == 0);

    _pCurBrush->_pMotion = pNewMotion;
    _scene.AddMotion(pNewMotion);
    return 1;
}

void CZ_ed2Doc::UpdateDynamicObjPos(Brush* pB)
{
    
    pB->_pMotion->RefreshPosition();
    BARDLG()->UpdateMotionProps(pB, pB->_pMotion);
}


void CZ_ed2Doc::OnWpncfg() 
{

}


Brush* CZ_ed2Doc::GetLastSelBrush()
{
    int       nCount;
    Brush**   ppB =  GetSelBrushes(nCount);
    if(nCount)
    {
        return *((ppB+nCount)-1);
    }
    return 0;
}

Brush* CZ_ed2Doc::GetFirstSelBrush()
{
    int       nCount;
    Brush**   ppB =  GetSelBrushes(nCount);
    if(nCount)
    {
        return *(ppB);
    }
    return 0;

}



void CZ_ed2Doc::OnImport() 
{
    /**
    char szFilters[] = _T("Geometry raw (*.grf)|*.grf|All Files (*.*)|*.*||");

    CFileDialog dlg(_bFileOpen, "WMP", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    if (dlg.DoModal() != IDOK) return;   

    FileWrap    fw;

    if(!fw.Open(dlg.m_ofn.lpstrFile,"rb"))
        return;

    int  mBlkCnt = 1;
    int  mObjCnt = 1;
    char line[256];
    char expectedBlock[64];
    char expectedObject[64];
    
    _stprintf(expectedBlock, "block %d {", mBlkCnt);
    _stprintf(expectedObject, "block %d {", mObjCnt);

    vvector<V3>  block;
    vvector <vvector<V3> > blocks;


    while(! feof(fw._pf))
    {
    
        if(pf.ReadLine(line, 256))
        {
            if(strstr(line, expectedBlock))
            {
                mBlkCnt++;
                _stprintf(expectedBlock, "block %d {", mBlkCnt);

                ReadBlock(fw);

            }
            else
            if(strstr(line, expectedObject))
            {
                mObjCnt++;
                _stprintf(expectedObject, "block %d {", mObjCnt);
            }



        }
        if(feof(fw._pf))
            break;
    }




    fw.Close();
    */	
}


void CZ_ed2Doc::OnTexrefdlg() 
{
    CDlgTexRefs().DoModal();
	
}

void CZ_ed2Doc::DeleteBrushItemLinks(Brush* pBrush)
{
    
    //
    // motion is linked to the motions and cannot exists without brush
    //
    DeleteBrushMotion(pBrush); 
    if(pBrush->_brushflags & BRSH_HASTRIG)
    {
		DOC()->DetachTrigger(pBrush);
    }
    if(pBrush->_brushflags & BRSH_BIGTERRAIN)
    {
        CBigTerrain* pBt = (CBigTerrain*)pBrush->_pUsrData;
        pBt->Clear(0);
    }
}

//
// Brush hsa been scaled or moved. Move all associated links to it to this brush
//
void CZ_ed2Doc::UpdateBrushLinks(Brush* pBrush)
{
    //
    // move the entire motion object
    //
    if(pBrush->_pMotion )//1-2-1
    {
        UpdateDynamicObjPos(pBrush);
    }

    //
    // if the brush has a triger find that trigger. 
    // this is fast as long there are not many triggers 
    //
    if(pBrush->_brushflags & BRSH_HASTRIG)
    {
        vvector<SceItem*>*  pTemsPtr = GetItems();

        FOREACH(vvector<SceItem*>, (*pTemsPtr), ppItem)
        {
            SceItem*  pItem  = *ppItem;
            if(pItem->_item == ITM_TRIGER)
            {
                TriggerItem* pTi = (TriggerItem*)pItem;
                if(pTi->GetEfectBrush()==pBrush)
                {
                    V3& efp = pTi->EfectPoint();
                    efp = pBrush->_box.GetCenter();
                }
            }
        }
    }

}

void  CZ_ed2Doc::CameraPointTo(V3& v)
{
//	_cam.LookAt(v);

	
	

//	VIEW3D(0,0)->RedrawDC();
}
void CZ_ed2Doc::OnShowselbbox() 
{
    SelModeSave sms(_brmode,_selmode);

    _selShowSelBox	= !_selShowSelBox;
    Invalidate();
    if(_selShowSelBox)
    {
        OnFinishMoveRotScale(M_MOVED);
    }
}

void CZ_ed2Doc::EndEditor()
{
}


BOOL CZ_ed2Doc::StartEditor()
{
	return 0;
}

void CZ_ed2Doc::DetachTrigger(Brush* pB)
{
	vvector<SceItem*>*  pTemsPtr = GetItems();
	FOREACH(vvector<SceItem*>, (*pTemsPtr), ppItem)
	{
		if((*ppItem)->_item == ITM_TRIGER)
		{
			TriggerItem* pTi = (TriggerItem*)(*ppItem);
			if(pTi->_pBrush == pB)
			{
				pTi->_pBrush=0;
				return;
			}
		}
	}
}


void CZ_ed2Doc::ModifyBrushes(vvector<Brush*>& pChanged)
{
    FOREACH(vvector<Vtx2*>, _pSelVertexes, ppVx)
    {
        Poly* pPoly = (*ppVx)->_pPoly;
        if(pPoly)
            pPoly->_polyflags |= POLY_DIRTY;
    }

    FOREACH(vvector<Vtx2*>, _pSelVertexes, ppVx)
    {
        Poly* pPoly = (*ppVx)->_pPoly;
        
        if(pPoly && (pPoly->_polyflags & POLY_DIRTY))
        {
            pPoly->_polyflags &= ~POLY_DIRTY;
            pChanged.push_unique(pPoly->_pBrush);
        }
    }
}

void CZ_ed2Doc::SplitBySplitterBrush(Brush* pb)
{
    SaveTempFile();
    
    CMotion* pM = 0;


    PBrushes brushes;
    PBrushes touchBrushes;
    DeselectAll();

    FOREACH(PBrushes, (*_scene.GetPrimitives()), ppb)
    {
        if((*ppb)->IsCut())
            continue;

        if(!(*ppb)->_box.IsTouchesBox(pb->_box))
            continue;

        touchBrushes << (*ppb);
    }

    BOOL bisP  = AKEY('P');
    BOOL bisB  = AKEY('B');

    if(FALSE == (bisP|bisB))
    {
        UINT id = AfxMessageBox("Do you want to split the Brushe(s) \n (Press No To Split the Polygons) \n\n All brushes motion will be deleted", MB_ICONQUESTION|MB_YESNOCANCEL);
        if(IDYES==id)
        {
            bisB=TRUE;
            bisP=FALSE;
            this->DeleteBrushMotion(pb);
        }
        else if(IDNO==id)
        {
            bisB=FALSE;
            bisP=TRUE;
            this->DeleteBrushMotion(pb);
        }
        else
            return;
    }

    FOREACH(PBrushes, touchBrushes, ppb)
    {
        Brush*    frontB = new Brush();
        Brush*    backB  = new Brush();
        
        frontB->_brushflags = BRSH_SOLID|BRSH_NEW;
        backB->_brushflags = BRSH_SOLID|BRSH_NEW;

        // cut ppb by pb but retain both fragments
        if((*ppb)->Split(*pb, *frontB, *backB))
        {
            // clear the original brush
            (*ppb)->Clear();

            if(bisP)       // split the polygons
            {
                // make one brush with the fragments
                append(frontB->_polys, backB->_polys);
                (*ppb)->Clear();
                (*ppb)->Equal(*frontB, TRUE);
                (*ppb)->Recalc();

                (*ppb)->_brushflags &= ~BRSH_NEW;

                SelectBrush((*ppb));

                delete frontB;
                delete backB;
            }
            else if(bisB) 
            {
                DeleteBrush((*ppb));
                // retain 2 separate brushes
                if(frontB->_polys.size())
                {
                    frontB->_brushflags &= ~BRSH_NEW;
                    _scene.AddPrimitive(frontB);
                }
                else
                {
                    delete frontB;
                }

                if(backB->_polys.size())
                {
                    backB->_brushflags &= ~BRSH_NEW;
                    _scene.AddPrimitive(backB);
                }
                else
                {
                    delete backB;
                }
                OnEnter();
                //OnEscape();
            }
        }else
        {
            delete frontB;
            delete backB;
        }
    }
}


void CZ_ed2Doc::OnUpdateTriangulate(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling ||IsSelBoxOn() || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_pSelBrushes.size()!=0);
}


void CZ_ed2Doc::OnTriangulate() 
{
    FOREACH(PBrushes,  _pSelBrushes, ppBrsh)
    {
        if(!(*ppBrsh)->Triangulate())
        {
            SBT(0, "Faces: Too many or too small.");
        }
    }
    Recut();
    Invalidate();
}


void CZ_ed2Doc::OnTriFan() 
{
    FOREACH(PBrushes,  _pSelBrushes, ppBrsh)
    {
        (*ppBrsh)->TriFanIt();
    }
    Recut();
    Invalidate();
}

void CZ_ed2Doc::OnUpdateTriFan(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling ||IsSelBoxOn() || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_pSelBrushes.size()!=0);
}

void CZ_ed2Doc::OnInsvx() 
{
	
}

void CZ_ed2Doc::OnUpdateInsvx(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}


void CZ_ed2Doc::AddVertex(Poly* p, const V3& v)
{
    // add a vertex get result
    Brush* pBrush = p->_pBrush;
    vvector<Poly>   fragments;
    p->AddVertex(v, fragments);

    pBrush->Deleteface(p);
    pBrush->AddPolys(fragments);
    pBrush->Recalc();
    Recut();
}


void CZ_ed2Doc::OnSametexface() // Ctl+G
{
	if(_pSelPolys.size())
    {

	    Poly*	pP       = _pSelPolys.back();
	    Brush*	pB       = pP->_pBrush;
        UINT    sametex  = (int)pP->GetHtex(GUtex);

        if(AKEY(VK_SHIFT))//accumulates selections
        {
            FOREACH(PBrushes, (*_scene.GetPrimitives()), ppbb)
	        {
		        Brush* pB =*ppbb;

                vvector<Poly>::iterator pPly = pB->_polys.begin();
                vvector<Poly>::iterator be = pB->_polys.end();
                for(; pPly != be; pPly++)
                {
                    Poly& rP = *pPly;
                    if(pP == &rP)
                        continue;
                    if(sametex == rP.GetHtex(GUtex))
                    {
		                SelectFace(&rP, pB);
                    }
                }
            }
        }
        else
        {
             // find the selected poly in the brush
            vvector<Poly>::iterator pPly = pB->_polys.begin();
            vvector<Poly>::iterator be = pB->_polys.end();
            for(; pPly != be; pPly++)
            {
                Poly& rP = *pPly;
                if(pP == &rP)
                    continue;
                if(sametex == rP.GetHtex(GUtex))
                {
		            SelectFace(&rP, pB);
                }
            }
        }
    }
	Invalidate();		
}

void CZ_ed2Doc::OnSamenormface()	//Ctl+ H
{
	if(_pSelPolys.size())
    {
	    Poly*	pP     = _pSelPolys.back();
	    Brush*	pB     = pP->_pBrush;
        V3	   norm    = pP->_n;

        if(AKEY(VK_SHIFT))
        {
            FOREACH(PBrushes, (*_scene.GetPrimitives()), ppbb)
	        {
		        Brush* pB =*ppbb;

                vvector<Poly>::iterator pPly = pB->_polys.begin();
                vvector<Poly>::iterator be = pB->_polys.end();
                for(; pPly != be; pPly++)
                {
                    Poly& rP = *pPly;
                    if(pP == &rP)
                        continue;
                    if(norm == rP._n)
                    {
		                SelectFace(&rP, pB);
                    }
                }
            }
        }
        else
        {
            // find the selected poly in the brush
            vvector<Poly>::iterator pPly = pB->_polys.begin();
            vvector<Poly>::iterator be = pB->_polys.end();
            for(; pPly != be; pPly++)
            {
                Poly& rP = *pPly;
                if(pP == &rP)
                    continue;
                if(norm == rP._n)
                {
		            SelectFace(&rP, pB);
                }
            }
        }
    }
	Invalidate();		
}



void CZ_ed2Doc::OnUpdateXoy(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(_pSelBrushes.size());
        return;
    }
}


void CZ_ed2Doc::OnUpdateXoz(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(_pSelBrushes.size());
        return;
    }
	
}


void CZ_ed2Doc::OnUpdateZoy(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(_pSelBrushes.size());
        return;
    }
}

void CZ_ed2Doc::OnUpdateExtrude(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(_pSelPolys.size()>=1);
}



// extrude on the direction of face normals
void CZ_ed2Doc::OnExtrude() 
{
    SaveTempFile();

    if(_pSelPolys.size()==0)
        return;

    Poly    poly; 
    Poly    np;
    vvector<Poly> nps(32);
    Brush*  pBrsh;
    pBrsh = _pSelPolys[0]->_pBrush;

    // see if all polygons belong to same brush
    FOREACH(PPolys,  _pSelPolys, a)
    {
        if((*a)->_pBrush != pBrsh)
        {
            AfxMessageBox("Cannot Extrude different brushes polygons", MB_OK|MB_ICONHAND);
            return;
        }
    }

    // store original brush for undo

    // copy the selected face
    FOREACH(PPolys,  _pSelPolys, a)
    {
        poly = *(*a);
        
        // extrude vertexes, ir 'R' backward
        FOREACH(vvector<Vtx2>, poly._vtci, pV){
            if(AKEY('R'))
                pV->_xyz += poly._n * (-64);
            else
                pV->_xyz += poly._n * 64;
        }
        poly.Recalc();

        // make sideways polygons
        int  vxes = poly._vtci.size();
        for(int i=0; i < vxes; i++)
        {
            np.Clear();
            np.GetTexs() = poly.GetTexs();
            int vx0 = i % vxes;
            int vx1 = (i+1) % vxes;

            np << (*a)->_vtci[vx0];
            np << (*a)->_vtci[vx1];

            np << poly._vtci[vx1];
            np << poly._vtci[vx0];
            
            if(vxes==4)
            {
                np.ResetTexCoords(-1);
            }
			np.Recalc();
            nps.push_back(np);
        }
        // flag the poly extruded
        poly._polyflags |= POLY_EXTRUDED;
        nps.push_back(poly);
        poly.Clear();
    }

    // mark extruded polygons deleted
    FOREACH(PPolys,  _pSelPolys, a)
    {
        (*a)->_polyflags |= POLY_DELETED;
    }
    DeselectAll();

	
    // add new sideways polygons
    FOREACH(vvector<Poly>, nps, pPoly)
    {
		pPoly->_polyflags |= POLY_EXTRUDED;
        pBrsh->AddPoly(*pPoly);
    }
    nps.clear();

    // delete marked faces
AGAIN:
    FOREACH(Polys, pBrsh->_polys, pp)
    {
        if(pp->_polyflags & POLY_DELETED)
        {
            pBrsh->ErasePoly(*pp);
            goto AGAIN;
        }
    }
    pBrsh->Recalc();
    SelectBrush(pBrsh);
    UpdateBrushLinks(pBrsh);
    Recut();
    DeselectBrushes();
    Invalidate(1);
}

void CZ_ed2Doc::OnUpdateHallounhallo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_pSelBrushes.size()!=0);
}

void CZ_ed2Doc::OnHallounhallo() 
{
    FOREACH(PBrushes,  _pSelBrushes, ppb)
    {
        (*ppb)->HalloUnHallo();
    }
    Invalidate();
}

void CZ_ed2Doc::OnMerge() 
{
    SaveTempFile();
 
    if(_pSelBrushes.size()>=2)	
    {
        Brush* pba = _pSelBrushes[0];
        DeSelectBrush(pba);

        for(UINT i=0; i<_pSelBrushes.size();i++)
        {
            Brush& bb = *_pSelBrushes[i];
            pba->AppendPrim(bb);
            pba->Recalc();
        }
        OnDelete() ;
        SelectBrush(pba);
		OnEnter();
    }

}

void CZ_ed2Doc::OnUpdateMerge(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(_pSelBrushes.size()>=2);
}



void CZ_ed2Doc::CleanProjections(Brush& projBrush, BOOL onlyFlags)
{
    Box&        box = projBrush._box;

    if(projBrush.IsProjector())
    {
    AGAIN:
        FOREACH(PBrushes, projBrush._pSldBrushes, ppB)    
        {
            if(!(*ppB)->_box.IsTouchesBox(box))
            {
                // restore its polys
                vvector<Poly>::iterator pPly = (*ppB)->_polys.begin();
                vvector<Poly>::iterator be = (*ppB)->_polys.end();
                for(; pPly != be; ++pPly)
                {
                    if(pPly->_polyflags & FACE_PROJTEX)
                    {
                        pPly->_polyflags      &= ~FACE_PROJTEX;

                        if(!onlyFlags)
                        {
                            pPly->SetHtex(pPly->_nonProjTex, GUtex); 
                            FOREACH(vvector<Vtx2>, pPly->_vtci, pv)
                            {
                                pv->_uv[GUtex] = pv->_uv[SAFE_TIDX];
                            }
                        }
                        else
                        {
                            pPly->SetHtex(pPly->_nonProjTex, GUtex); 
                        }
                    }
                }
                projBrush.RemoveBrush(*ppB);
                goto AGAIN;
            }
        }
    }
    else if(projBrush.IsSolid())
    {
    AGAIN2:
        FOREACH(PBrushes, projBrush._pProjBrushes, ppB)    
        {
            if(!(*ppB)->_box.IsTouchesBox(box))
            {
                projBrush.RemoveProjection(*ppB);
                goto AGAIN2;
            }
        }

        vvector<Poly>::iterator pPly = projBrush._polys.begin();
        vvector<Poly>::iterator be = projBrush._polys.end();
        for(; pPly != be; ++pPly)
        {

            if(pPly->_polyflags & FACE_PROJTEX)
            {
                pPly->_polyflags      &= ~FACE_PROJTEX;
                if(!GKeepProjections)
                {
                    pPly->SetHtex(pPly->_nonProjTex,GUtex); 
                    FOREACH(vvector<Vtx2>, pPly->_vtci, pv)
                    {
                        pv->_uv[GUtex] = pv->_uv[SAFE_TIDX];
                    }
                }
                else
                {
                    pPly->_nonProjTex = _dumptex;
                }
            }
        }
    }
}

// texture is changed on the projector. Affects all faces where 
// us projecting to;
void CZ_ed2Doc::ProjectTexture(Brush& projBrush)
{
 
    Box&        box = projBrush._box;

    PPolys          pSelPolys;

    // CleanProjections(projBrush);
    // get out untouched brushes, and restore th polygons tex coord
    // and original texture
    FOREACH(PBrushes, projBrush._pSldBrushes, ppB)    
    {
        vvector<Poly>::iterator pPly = (*ppB)->_polys.begin();
        vvector<Poly>::iterator be = (*ppB)->_polys.end();
        for(; pPly != be; ++pPly)
        {
            if(pPly->_polyprops & POLY_NOPROJ)
                continue;
            if(pPly->_polyflags & FACE_PROJTEX)
            {
                pPly->_polyflags &= ~FACE_PROJTEX;
                pPly->SetHtex(pPly->_nonProjTex, GUtex); 
                FOREACH(vvector<Vtx2>, pPly->_vtci, pv)
                {
                    pv->_uv[GUtex] = pv->_uv[SAFE_TIDX];
                }
            }
            


            if(box.ContainPoint(pPly->_box._min) && 
   		       box.ContainPoint(pPly->_box._max))
            {
			    pSelPolys << &(*pPly);
            }
        }
     
    }    
    
    // all faces are selected
    // project them on poly 2
    if(projBrush._polys.size()<=2)
    {
        return;
    }
    Poly    proj = projBrush._polys[2];
    
    V3      o = proj._vtci[2]._xyz;
    V3      s = (proj._vtci[1]._xyz - o).norm();
    V3      t = (proj._vtci[3]._xyz - o).norm();

    UV&     tm  = proj._vtci[2]._uv[GUtex];

    M4  m = MLook(o,-proj._n, t, s);

    // project the projection poly on it's plane. (z should be 0)
    FOREACH(vvector<Vtx2>, proj._vtci, pv)
    {
        m.v3transform(pv->_xyz);
    }

    //      get tmax extends of s and t
    V3      mv = proj._vtci[0]._xyz;
    V3      tv;

    // transform all polys in this 'O'
    FOREACH(PPolys,  pSelPolys, p)    
    {
        Poly* pp   = (*p);

//        if(Vdp(pp->_n, proj._n) > 0)
//            continue;  // skip back faces

        FOREACH(vvector<Vtx2>, pp->_vtci, pv)
        {
            tv = pv->_xyz;
            m.v3transform(tv);

            pv->_uv[SAFE_TIDX] = pv->_uv[GUtex];

            pv->_uv[GUtex].u = (tv.x / mv.x) * proj._texcoord.scalex;
            pv->_uv[GUtex].v = (tv.y / mv.y) * proj._texcoord.scaley;

            pv->_uv[GUtex].u += proj._texcoord.shiftx;
            pv->_uv[GUtex].v += proj._texcoord.shifty;

        }

        pp->_polyflags      |= FACE_PROJTEX;
        pp->_nonProjTex      = pp->GetHtex(GUtex);     
        pp->SetHtex(proj.GetHtex(GUtex), GUtex);
    }
}

// here are cmming the nCount we've dragged rotate or scaled around
// see what cuts intersec what brushes
void CZ_ed2Doc::UpdateCutsCollisionList(Brush** pMovedBrushes, int nCount)
{
    map<Brush*, int>           pBrsh2Update;
    int			nBrushes     = nCount;
	Brush**     pWlkBrush	 = pMovedBrushes;
	BOOL		bDlgOn       = FALSE;
    DWORD		timeStart    = GetTickCount();
	

	// collect all brushes tha have to be updated
	while(nBrushes--)            // moved brushes
	{
		Brush* pMovedBrush = *pWlkBrush++;

        if(pMovedBrush->IsSelection())
        {
            continue;
        }
        if(pMovedBrush->_brushflags & BRSH_DISABLED)
        {
            if(ThrID == GetCurrentThreadId())
                continue;
        }

        CleanProjections(*pMovedBrush, GKeepProjections);

        // IF CUT or (PROJECTOR)
		if(pMovedBrush->IsCut() || pMovedBrush->IsProjector())
		{
			vvector<Brush*>::iterator ppb = pMovedBrush->_pSldBrushes.begin();
			vvector<Brush*>::iterator ppE = pMovedBrush->_pSldBrushes.end();

			for(;ppb!=ppE;ppb++)
			{
				pBrsh2Update[*ppb] = 1;
			};
			pMovedBrush->UpdateSolidsTouches();
		}
        else // IF SOLID
		{
			pBrsh2Update[pMovedBrush] = 1;
			pMovedBrush->UpdateCutsTouches();
		}
	}
	
	nBrushes	  = nCount;	
	pWlkBrush	  = pMovedBrushes;
    while(nBrushes--)
    {
        Brush* pMovedBrush = *pWlkBrush++;

        if(pMovedBrush->_brushflags & BRSH_DISABLED)
        {
            if(ThrID == GetCurrentThreadId())
            continue;
        }
        if(pMovedBrush->IsSelection())
            continue;
        forall2( *(GetPrimitives()), this, 
 	    	     &CZ_ed2Doc::UpdateTouches, 
				 pMovedBrush, pBrsh2Update);
    }

   
    map<Brush*,int>	toDel;	// to delete due the unsuccess BSP build

	// now cut the brushes with the cuts
	int	nCountB = 0;
    map<Brush*,int>::iterator mb =  pBrsh2Update.begin();
	map<Brush*,int>::iterator me =  pBrsh2Update.end();
	for(;mb!=me;mb++)
    {
    	//	KEEP_WAIT(MKSTR("Building minimal CSG %x",mb));		

        if((*mb).first->_brushflags & BRSH_DISABLED)
        {
            if(ThrID == GetCurrentThreadId())
                continue;
        }

        //
        // Uncomment to forbid terrain cutting
        //
		//if(!((*mb).first->_flags & BRSH_CSGTERRAIN))
        // ONLY projector has brushes
        if((*mb).first->IsProjector())
        {
            ProjectTexture(*(*mb).first);
        }
        else
		{
			if(!(*mb).first->ApplyCuts())
			{
				// delete it's cuts
				FOREACH( PBrushes, (*mb).first->_pCutBrshes, ppBrush)
					toDel[(*ppBrush)] = 1;	// keep unicity
			}
		}

		if(ThrID == GetCurrentThreadId() && timeStart != 0)
		{
			if(GetTickCount() - timeStart > 2000)
			{
				timeStart=0;
				bDlgOn = 1;
				START_WAIT("Please Wait");
			}
		}
		nCountB++;
    }
    
	//now delete 
	if(toDel.size())
	{
		if(bDlgOn)
			END_WAIT("");

		if(AfxMessageBox("The geometry is too compilated to be cut\n"
					     "Do you want to delete the cut brush ?\n"
					     "If you select 'No' Try to move out of contact the one of the brushes", MB_YESNO|MB_ICONHAND)==IDYES)
		{
			DOC()->DeselectAll();
			mb =  toDel.begin();
			me =  toDel.end();
			for(;mb!=me;mb++)
			{
				DOC()->SelectBrush((*mb).first);
			}
			AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_DELETE,0);
		}
	}

	if(bDlgOn)
		END_WAIT("");
}

// link brush with it's cutters
// br->cut is need when 3d painting to show the already cutt brush
BOOL CZ_ed2Doc::UpdateTouches(Brush* pPrim, 
                              Brush* pMovedBrush, 
							  map<Brush*, int>& accList)
{
    if(pMovedBrush == pPrim)
        return TRUE;

	if((pMovedBrush->IsCut() && pPrim->IsCut()) ||
	   (pMovedBrush->IsSolid() && pPrim->IsSolid()) ||
	   (pMovedBrush->IsSelection() || pPrim->IsSelection()) )
	   return TRUE;
 

    if(pPrim->_brushflags & BRSH_DISABLED || 
       pMovedBrush->_brushflags & BRSH_DISABLED)
    {
        if(ThrID == GetCurrentThreadId())
            return TRUE;
    }

    // only in this group
    if(GDisableGrpCSG )
    {
	    if(pMovedBrush->_group>0 && pPrim->_group>0)
	    {
		    if(pMovedBrush->_group != pPrim->_group)
			    return TRUE; 
	    }
    }
/**
	if(pMovedBrush->_flags & BRSH_CSGTERRAIN || pPrim->_flags & BRSH_CSGTERRAIN )
		return 1;
*/
    if(pPrim->_box.IsTouchesBox(pMovedBrush->_box))
    {
        if(pPrim->IsSolid())
        {
            if(pMovedBrush->IsCut())
            {
				if(pMovedBrush->_brushflags & BRSH_CUTALL)
				{
					pPrim->AddCutter(pMovedBrush);
					pMovedBrush->AddBrush(pPrim);	// keep a list for cutter whwn we move away from cutter
					accList[pPrim]=1;  
				}
				else
				if(pMovedBrush->_unicID > pPrim->_unicID)
				{
					pPrim->AddCutter(pMovedBrush);
					pMovedBrush->AddBrush(pPrim);	// keep a list for cutter whwn we move away from cutter
					accList[pPrim]=1;  
				}
            }
            else if(pMovedBrush->IsProjector() && (pMovedBrush->_unicID > pPrim->_unicID))
            {
                ///pPrim->AddBrush(pMovedBrush);
                pMovedBrush->AddBrush(pPrim);       // prim is projector->on a solid brush
                accList[pMovedBrush]=1;
            }
        }
        else if(pPrim->IsCut())
        {
            if(pMovedBrush->IsProjector())
            {
                // nothing
            }
            else
            {
		        if(pPrim->_brushflags & BRSH_CUTALL)
			    {
				    pMovedBrush->AddCutter(pPrim);
				    pPrim->AddBrush(pMovedBrush);   // keep a list for cutter whwn we move away from cutter
				    accList[pMovedBrush]=1;  
			    }
			    else
			    if(pPrim->_unicID > pMovedBrush->_unicID)
			    {

				    pMovedBrush->AddCutter(pPrim);
				    pPrim->AddBrush(pMovedBrush);   // keep a list for cutter whwn we move away from cutter
				    accList[pMovedBrush]=1;  
			    }
            }
        }
        else if(pPrim->IsProjector() && (pPrim->_unicID > pMovedBrush->_unicID))
        {
	        //pMovedBrush->AddProjection(pPrim);
	        pPrim->AddBrush(pMovedBrush);       // keep a list touched onproj brushes
	        accList[pPrim]=1;  
        }
    }
    return TRUE;
}

void  CZ_ed2Doc::SbarShowCurMode()
{
    /*
    int   i;
    char selText[128] = "";
    BOOL  found;

    switch(_brmode)
    {
    case BR_VIEW:
        _tcscpy(selText,"View Mode, ");
        break;
    case BR_MOVE:
        _tcscpy(selText,"Move Mode, ");
        break;
    case BR_SCALE:
        _tcscpy(selText,"Scale Mode, ");
        break;
    case BR_ROTATE:
        found=FALSE;    
        for(i=2; i<9; i++)
        {
            if(AKEY(i+'0'))
            {
                if(AKEY(VK_SHIFT))
                {
                    _tcscpy(selText,MKSTR("Rotates -PI/%d, ",i));
                }
                else
                {
                    _tcscpy(selText,MKSTR("Rotates +PI/%d, ",i));
                }
                found=TRUE;
                break;
            }
        }
        if(found)
            break;
        _tcscpy(selText,"Rotate Mode, ");
        break;
    case BR_RISE:
        _tcscpy(selText,"Not Used, ");
        break;
    }


    switch(_selmode)
    {
    case SEL_NONE:
        _tcscat(selText,"");
        break;
    case SEL_BRUSH:
        _tcscat(selText,"Brush Selection");
        break;

    case SEL_FACE:
        _tcscat(selText,"Face Selection");
        break;

    case SEL_VERTEX:
        _tcscat(selText,"Vertex Selection");
        break;
    case SEL_ITEMS:
        _tcscat(selText,"Item Selection");
        break;
    case SEL_BR_ITEM:
        _tcscat(selText,"N/A");
        break;

    }

    SBT(1,selText);
*/
}

// make the cuts permanent
/*
void CZ_ed2Doc::OnPermcut() 
{
    PBrushes    brshSolids(32);
    PBrushes    brshCuts(32);

    try{
        Brush* pResultSceneBrsh  = 0;
               
        DWORD flags = 0;

        // collect solids tounching each other
        FOREACH(PBrushes,  _pSelBrushes, ppb)
        {
            Brush* pBrush = (*ppb)->_pResult;
            if(pBrush->IsSolid())
            {
                brshSolids.push_back(pBrush);
                flags |= pBrush->_brushflags;
            }
        }

        // union the solids
        if(brshSolids.size() != 0)
        {
            pResultSceneBrsh = new Brush();
            pResultSceneBrsh->_flags = flags|BRSH_SOLID|BRSH_NEW;
            
            if(!Compiler::Union(brshSolids, *pResultSceneBrsh,1))
            {
                delete pResultSceneBrsh;
                pResultSceneBrsh = 0;
            }
        }

        _userBool = TRUE;
        OnDelete();
        _userBool = FALSE;
        
        SelectBrush(pResultSceneBrsh);
        Invalidate();
	    VIEW3D(0,0)->SetFocus();
		OnEnter();

    }catch(...)
    {
        AfxMessageBox("Cannot Commit Changes",MB_ICONWARNING,MB_OK);
    }
}
*/


void CZ_ed2Doc::OnPermcut() 
{
    SaveTempFile();

    set<Brush*>         brshSolids;
    vvector<Brush*>     selCopy = _pSelBrushes;
    vvector<Brush*>     newBrushes;
    int                 collsz,collsz2;
    Brush*              pResultSceneBrsh  = 0;

    DeselectAll();

    _TRY
    {
        DWORD flags = 0;

        // collect solids tounching each other
        FOREACH(PBrushes,  selCopy, ppb)
        {
            Brush* pBrush = (*ppb)->_pResult;

            if(pBrush->_brushflags & BRSH_FLAGGED)
                continue;

            if(!pBrush->IsSolid())
                continue;
            if( pBrush->_brushflags & BRSH_DETAIL)
                continue;

            flags    |= pBrush->_brushflags;
            pBrush->_brushflags |= BRSH_FLAGGED;
            brshSolids.insert(pBrush);

            collsz = brshSolids.size();
            FOREACH(PBrushes,  selCopy, ppb1)
            {
                if(ppb1 == ppb)                     continue;
                if((*ppb1)->_brushflags & BRSH_FLAGGED)  continue;
                if( pBrush->_brushflags & BRSH_DETAIL)
                    continue;


                if((*ppb1)->_box.IsTouchesBox(pBrush->_box))
                {
                    flags           |= pBrush->_brushflags;
                    (*ppb1)->_brushflags |= BRSH_FLAGGED;
                    brshSolids.insert(*ppb1);
                }
            }
            collsz2 = brshSolids.size();
            while(collsz2!= collsz) // as much there are adds
            {
                // search again in added list and see any more touching polygons
                collsz = brshSolids.size();
                set<Brush*>::iterator si = brshSolids.begin();
                set<Brush*>::iterator se = brshSolids.end();
                for(;si!=se;si++)
                {
                    FOREACH(PBrushes,  selCopy, ppb1)
                    {
                        if(*ppb1 == *si)                    continue;
                        if((*ppb1)->_brushflags & BRSH_FLAGGED)  continue;

                        if((*ppb1)->_box.IsTouchesBox((*si)->_box))
                        {
                            flags           |= pBrush->_brushflags;
                            (*ppb1)->_brushflags |= BRSH_FLAGGED;
                            brshSolids.insert(*ppb1);
                        }
                    }
                }
                collsz2 = brshSolids.size();
            }

            // now for each set build a csged brush
            if(brshSolids.size())
            {
                pResultSceneBrsh = new Brush();
                pResultSceneBrsh->_brushflags = flags|BRSH_SOLID|BRSH_NEW;
                vvector<Brush*> loco(brshSolids.size());

                set<Brush*>::iterator si = brshSolids.begin();
                set<Brush*>::iterator se = brshSolids.end();
                for(;si!=se;si++)
                {
                    loco << *si;
                }

                if(!Compiler::Union(loco, *pResultSceneBrsh,1))
                {
                    delete pResultSceneBrsh;
                    pResultSceneBrsh = 0;
                }
                else
                {
                    pResultSceneBrsh->_brushflags &= ~BRSH_FLAGGED;
                    pResultSceneBrsh->_brushflags |= BRSH_NEW;

                    newBrushes << pResultSceneBrsh;
                    pResultSceneBrsh = 0;
                }
                //SelectBrush(pResultSceneBrsh);
            }
            brshSolids.clear();
            flags = 0;
        }


        _userBool    = TRUE;
        _pSelBrushes = selCopy;
        
        OnDelete();         // delete old selected brushes
        _userBool = FALSE;

        DeselectAll();
        _pSelBrushes = newBrushes;
        OnEnter();
        Invalidate();

	    VIEW3D(0,0)->SetFocus();

        FOREACH(PBrushes,  newBrushes, ppb)
        {
            (*ppb)->_brushflags &= ~BRSH_FLAGGED;
        }

    }
    _CATCHX()
    {
        AfxMessageBox("Cannot Commit Changes",MB_ICONWARNING,MB_OK);
    }
    if(pResultSceneBrsh) 
        delete pResultSceneBrsh;
    Invalidate();
}


void CZ_ed2Doc::OnUpdatePermcut(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(_pSelBrushes.size()>=2);
}

void CZ_ed2Doc::OnUndo() 
{
    SaveTempFile();
    OnEscape();	
    _undo.Undo(this,1);
    Recut();
    DeselectAll();
    Invalidate();
}

void CZ_ed2Doc::OnUpdateUndo(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(_undo.Size());
}

void CZ_ed2Doc::OnUpdateTervol(CCmdUI* pCmdUI) 
{
	if(_pSelBrushes.size()==1 && _pSelBrushes[0]->_brushflags & BRSH_CSGTERRAIN)
        pCmdUI->Enable(1);
    else
        pCmdUI->Enable(0);
	
}

void CZ_ed2Doc::OnTervol() 
{
    
    Brush*  pSel = _pSelBrushes[0];
    Box     box  =  pSel->_box;
    V3&     pos  =  pSel->_box.GetCenter();

    // build a box 
    Brush*   pSquare = new Brush();
    box._max.y *= 2;
    box._min.y *= 2;

    pSquare->MakeCube(box.GetExtends());
    pSquare->Move(pos);
    
    SelectBrush(pSquare);
    _scene.AddPrimitive(pSquare);
    pSquare->_brushflags&=~BRSH_NEW;
    
    //OnPermcut();
    
    OnMerge(); 
    Invalidate();
    // mergeit with the terrain
    // flag it non terrain
    
}

void CZ_ed2Doc::OnManTex() 
{
    DlgTexMody d; d.DoModal();	
}

void CZ_ed2Doc::OnUpdateManTex(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}

void CZ_ed2Doc::OnToXml() 
{
    SaveTempFile();

	REDIR();
		
    LPTSTR lpszPathName = NULL;
    char szFilters[] = _T("VMRL 1.0 (*.txt)|*.txt|All Files (*.*)|*.*||");

    // Prompt the user for a filename
    CFileDialog dlg(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    dlg.m_ofn.lpstrTitle = _T("Select TEXT File");
    
    dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();
    // Show the dialog
    if (theApp.DoFileDialog(dlg) != IDOK) return;   
    PathHandler ph(dlg.m_ofn.lpstrFile);
    theApp.DocsDir(ph.Path());

    SaveInXML(dlg.m_ofn.lpstrFile); 

	REDIR();	
}

void CZ_ed2Doc::OnUpdateToXml(CCmdUI* pCmdUI) 
{
	
}

void CZ_ed2Doc::Repos20()
{
    _zoom=.04;
    _cam.SetAngles(-PIPE4,-PIPE6,0);
    _cam.Euler2Pos();
    _cam._pos = V3(1000.0f,1000.0f,1600.0f);
	
    CZ_ed2View* pTX = VIEW2D(1,0);
    pTX->_shifty = 0;
    pTX->_shiftx = 0;
    pTX->_zoom   = 0.4;


    CZ_ed2View* pTY = VIEW2D(0,1);
    pTY->_shifty = 0;
    pTY->_shiftx = 0;
    pTY->_zoom   = 0.4;

	CZ_ed2View* pTZ = VIEW2D(1,1);
    pTZ->_shifty = 0;
    pTZ->_shiftx = 0;
    pTZ->_zoom   = 0.4;
    _shift.reset();
    _zoom.setval(V1);
    Invalidate(1);
}

//CTRL+D
void CZ_ed2Doc::OnExtractfaces() 
{
	if(_pSelPolys.size()==0)
		return;

	set<Brush*>	brushes;

	Brush*    frontB = new Brush();
	
	frontB->_brushflags = BRSH_SOLID|BRSH_NEW|BRSH_DETAIL;

	FOREACH(PPolys,  _pSelPolys, a)
	{
		Poly   p = *(*a);
		p._pBrush = frontB;
        p.DeSelect();
		frontB->_polys << p;

		(*a)->_polyflags |= POLY_DELETED;
		brushes.insert((*a)->_pBrush);
	}
	_pSelPolys.clear();

	frontB->Recalc();
	_scene.AddPrimitive(frontB);
	
	for(set<Brush*>::iterator sb = brushes.begin(); sb!=brushes.end(); sb++)
	{
		(*sb)->DeletePolys();
		(*sb)->Recalc();
	}
	
	Invalidate();
	OnEnter();
}

void CZ_ed2Doc::OnUpdateExtractfaces(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_pSelPolys.size()>4)	;
}

void CZ_ed2Doc::MarkDetPolys(int percent)
{
	// mark by area sorting
	// when commit, build from all having a common ege a detail brush
	// delete polys form it own original brush
}


void CZ_ed2Doc::OnEvaldets() 
{
    SaveTempFile();
    if(_devdet.m_hWnd == 0)
    {
        DOC()->DeselectAll();

        DOC()->BlockSelections(1);
        DOC()->_selmode = SEL_FACE;
        _devdet.Create(DLG_AUTODETAILS, AfxGetMainWnd());
    }
    _devdet.ShowWindow(SW_SHOW);

}

void CZ_ed2Doc::OnUpdateEvaldets(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(!_devdet._onScreen);
}

void CZ_ed2Doc::OnHome() 
{
    _cam.SetFov(2*PIPE6); 
	_lvupdate = GetTickCount();
    _cam.SetFar(24800.0);//250m
	_cam.SetAngles(-PIPE4,-PIPE6,0);
	_cam.Euler2Pos();
	_cam._pos = V3(1000.0f,1000.0f,1600.0f);
    SBT(2,MKSTR("3D: x:%05.2f y:%05.2f z:%5.2f",_cam._pos.x,_cam._pos.y,_cam._pos.z));
    Invalidate();
}

void CZ_ed2Doc::OnCreatesb() 
{
    SelModeSave sms(_brmode,_selmode);

    CPoint ptDummy;
    
    DeselectAll();
    CreateSelBox(ptDummy);
    if(AKEY(VK_SHIFT))
        _selBoxByTouch=TRUE;
    else
        _selBoxByTouch=FALSE;

    SelectByBox();
    Invalidate();
    //_brmode     = BR_SCALE;
    //_selmode	= SEL_BRUSH;

}

void CZ_ed2Doc::OnUpdateCreatesb(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_selmode != SEL_NONE);
}

void     CZ_ed2Doc::SaveTempFile()
{
#ifdef _FREE
    return;
#endif //    
    if(!GAutoSave) return;

    char                loco[_MAX_PATH];
    char                iittaa[32];
    int                 lastID = 0;
    string              filenames[32];
    int                 stored = 0;
    
    ::strcpy(loco, GetTitle());
    PathHandler ph(loco);

    do{
        REDIR();
        mkdir("temp");
        CDirChange	    cd("temp/");

        //search backups
  	    HANDLE			    hfFind;
	    WIN32_FIND_DATA	    fDATA;	
        int                 count = 0;
        cd.UseObj();        

        for(int i=0; i<16; i++)
            filenames[i]="";

    	if((hfFind=FindFirstFile(_T("*.grf"), &fDATA)) != INVALID_HANDLE_VALUE)
	    {
		    if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
            {
                do{
                    char* tild = strchr(fDATA.cFileName,'~');
                    if(tild && strstr(fDATA.cFileName, ph.JustFile()))
                    {
                        ++tild;
                        ++stored;
                        lastID =  tmax(lastID, ::atoi(tild));
						lastID = lastID%16;
                        filenames[lastID] = fDATA.cFileName;
                    }
                }while(FindNextFile(hfFind, &fDATA)!=0);
            }

        }
        FindClose(hfFind);  
    }while(0);

    // if count roll up and rename files
    if(stored >=15 )
    {
        for(int i=0; i<16; i++)
        {
            string& fname = filenames[i];
            if(fname.length()==0) continue;
            int npos = fname.find('~')+1;
            int id   = ::atoi(fname.substr(npos,2).c_str());
            if(0 == id)
            {
                _unlink(fname.c_str());
                --lastID;
            }
            else
            {
                string newname = fname;

                ::sprintf(iittaa, "%02d", id-1);
                newname.replace(npos, 2, iittaa);

                ::CopyFile(fname.c_str(), newname.c_str(),0);
                ::_unlink(fname.c_str());
            }
        }
        // delete the 0 and rname them up
    }

    ++lastID; 
	lastID = lastID%16;
    _tempfileName =  "temp/";
    _tempfileName += "~";
    ::sprintf(iittaa, "%02d", lastID);
    _tempfileName += iittaa;
    _tempfileName += ph.File();
        
    _TRY
    {
        SaveInFile(_tempfileName);
        TestBackupFile();
        _docDirty = 0;
    }
    _CATCHX()
    {
        AfxMessageBox("Unknown Exception while Saving", MB_OK|MB_ICONHAND);
        GAutoSave=0;
    }
}

void     CZ_ed2Doc::TestBackupFile()
{   
    char iittaa[32];
    char                loco[_MAX_PATH];
    int                 lastID = 0;
    vvector<string>     filenames;
    
    ::strcpy(loco, GetTitle());
    PathHandler ph(loco);

    do{
        REDIR();
        CDirChange	    cd("temp/");

        //search backups
  	    HANDLE			    hfFind;
	    WIN32_FIND_DATA	    fDATA;	
        cd.UseObj();  

    	if((hfFind=FindFirstFile(_T("*.grf"), &fDATA)) != INVALID_HANDLE_VALUE)
	    {
		    if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
            {
                do{
                    char* tild = strchr(fDATA.cFileName,'~');
                    if(tild && strstr(fDATA.cFileName, ph.JustFile()))
                    {
                        ++tild;
                        lastID =  tmax(lastID, ::atoi(tild));
                    }
                }  while(FindNextFile(hfFind, &fDATA)!=0);
            }
        }
        FindClose(hfFind);  
    }while(0);

 //temp
    _tempfileName =  "temp/";
    _tempfileName += "~";
    ::sprintf(iittaa, "%02d", lastID);
    _tempfileName += iittaa;
    _tempfileName += ph.File();

    FileWrap fw;
    if(fw.Open(_tempfileName,"rb"))
    {
        fw.Close();
    }
    else
    {
        _tempfileName="";
    }

}

void CZ_ed2Doc::OnLoadlastbackip() 
{
    DeleteContents();
    InitDocVars();
    ReadFromFile(_tempfileName);
    SetModifiedFlag(TRUE);  
   	
}

void CZ_ed2Doc::OnUpdateLoadlastbackip(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_tempfileName.GetLength());
}

//round to near 8 cm
void     CZ_ed2Doc::RoundToNear(V3 &v, int snp)
{
    if(_bSnapGrid)
    {
        int x = v.x;
        int y = v.y;
        int z = v.z;
    
        x/=SnapToXY;
        y/=SnapToXY;
        z/=SnapToXY;

        x*=SnapToXY;
        y*=SnapToXY;
        z*=SnapToXY;

        v.x = x;
        v.y = y;
        v.z = z;
    }
}


void CZ_ed2Doc::OnSnapvxes() 
{
    FOREACH(PBrushes,  _pSelBrushes, ppb)
    {
        Brush* pb = *ppb;
        if(pb->_brushflags & BRSH_DETAIL)
            continue;
        Polys& polys = pb->_polys;
        FOREACH(Polys, polys, pB)
        {
            Poly& poly = *pB;
            FOREACH(vvector<Vtx2>, poly._vtci, pV)
            {
                RoundToNear(pV->_xyz, 16);
            }
        }
    }
    Recut();
    Invalidate();
}

void CZ_ed2Doc::OnUpdateSnapvxes(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(_pSelBrushes.size());
}


void CZ_ed2Doc::OnShowgrid() 
{
	b_showgrid=!b_showgrid;
	Invalidate();
}

void CZ_ed2Doc::OnUpdateShowgrid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

static void $Complement(BYTE* pClr)
{
    pClr[0] = 255-pClr[0];
    pClr[1] = 255-pClr[1];
    pClr[2] = 255-pClr[2];
}



void CZ_ed2Doc::OnTrpolys() 
{
	BShowTrPolys	=! BShowTrPolys;
	Invalidate();
}

void CZ_ed2Doc::OnRepoint() //join vertexes in same pos
{
    REAL mindist = 65535.0;
    REAL dist    = _selBox._box.GetMinExtend();
    Brush* refBrush = 0;

    // find first the closest distance
    FOREACH(vvector<Vtx2*>,_pSelVertexes,ppv1)
    {
        (*ppv1)->_extra1 = 0; // unaligned;
    }

	// get all selected vertexes
	FOREACH(vvector<Vtx2*>,_pSelVertexes,ppv1)
    {
        if((*ppv1)->_extra1)
            continue;
        FOREACH(vvector<Vtx2*>,_pSelVertexes,ppv2)
        {
            if(ppv1 == ppv2)
                continue;
            if((*ppv2)->_extra1)
                continue;
            if((*ppv1)->_pPoly && (*ppv2)->_pPoly)
            {
                if((*ppv1)->_pPoly->_pBrush != (*ppv2)->_pPoly->_pBrush)
                {
                    if(vdist((*ppv1)->_xyz, (*ppv2)->_xyz) < dist)
                    {
                        if(0 == refBrush)
                            refBrush = (*ppv1)->_pPoly->_pBrush;

                        if((*ppv1)->_pPoly->_pBrush == refBrush)
                        {
                            (*ppv2)->_xyz = (*ppv1)->_xyz;
                            (*ppv1)->_extra1 = 1;
                        }
                        else
                        {
                            (*ppv1)->_xyz = (*ppv2)->_xyz;
                            (*ppv2)->_extra1 = 1;
                        }
                    }
                }
            }
        }
    }

    FOREACH(vvector<Vtx2*>,_pSelVertexes,ppv1)
    {
        (*ppv1)->_extra1 = 0; // unaligned;
    }

    Invalidate();
}

void CZ_ed2Doc::OnUpdateRepoint(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(SEL_VERTEX==_selmode && IsSelBoxOn() );
}

void CZ_ed2Doc::OnRepointbrsh() 
{
    SelModeSave sms(_brmode,_selmode);

    vvector<V3> svxes;
    Brush* refBrush1 =0;
    Brush* refBrush2 =0;
    FOREACH(vvector<Vtx2*>, _pSelVertexes, ppvx)
    {
        if(svxes.push_unique((*ppvx)->_xyz))
        {
            if(refBrush1==0)
                refBrush1=(*ppvx)->_pPoly->_pBrush;
            else  if(refBrush2==0)
                refBrush2=(*ppvx)->_pPoly->_pBrush;
        }
    }

    if(refBrush1 && refBrush2 && refBrush1!=refBrush2)
    {
        V3 diff = svxes[1]-svxes[0];
        refBrush1->Dirty(1);
        refBrush1->Move(diff);
        refBrush1->Recalc();
        this->OnFinishMoveRotScale(M_MOVED);
        Invalidate();
    }
}

void CZ_ed2Doc::OnUpdateRepointbrsh(CCmdUI* pCmdUI) 
{
    if(SEL_VERTEX==_selmode)
    {
        vvector<V3> svxes;
        FOREACH(vvector<Vtx2*>, _pSelVertexes, ppvx)
        {
            svxes.push_unique((*ppvx)->_xyz);
        }
        pCmdUI->Enable(svxes.size()==2);
    }
    else
        pCmdUI->Enable(0);
	
}


void CZ_ed2Doc::OnUpdateFileSave(CCmdUI* pCmdUI)  
{
}

void CZ_ed2Doc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
}


void CZ_ed2Doc::OnCamLspot() 
{
    _camSpot=!_camSpot;
    Invalidate();
}

BOOL CZ_ed2Doc::HasExtension(LPCTSTR szex)
{
    return _extensions.find(szex)!=-1;
}

void CZ_ed2Doc::OnFileSaveselAs() 
{
	REDIR();
    LPTSTR lpszPathName = NULL;
    char szFilters[] = _T("Geometry raw (*.grs)|*.grs|All Files (*.*)|*.*||");

    // Prompt the user for a filename
    CFileDialog dlg(FALSE, _T("grf"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    dlg.m_ofn.lpstrTitle = _T("Select Geometry File");
    
    dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();
    // Show the dialog

    if (theApp.DoFileDialog(dlg) != IDOK) return;   
    PathHandler ph(dlg.m_ofn.lpstrFile);
    theApp.DocsDir(ph.Path());



    try{
        SaveInFile(dlg.m_ofn.lpstrFile, 1);
        _docDirty = 0;
    }
    catch(int& i)
    {
        i;
        AfxMessageBox(_lasterror.c_str(), MB_OK|MB_ICONWARNING);
        _lasterror.erase();
    }
    catch(...)
    {
        AfxMessageBox("Unknown Exception while Saving", MB_OK|MB_ICONHAND);
    }
	REDIR();
    	
}

void CZ_ed2Doc::OnUpdateFileSaveselAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_pSelBrushes.size());
}

void CZ_ed2Doc::OnInsface() 
{
    if(_pSelBrushes.size()==1)
    {
	    Brush* pb = _pSelBrushes.front();

        Poly p;
        V3   v[4];
        V3   &dims   = pb->_box.GetExtends();
        V3   &center = pb->_box.GetCenter();

        v[0]  = center + V3(-(dims.x/2), (dims.y/2), -(dims.z/2));
        v[1]  = center + V3(-(dims.x/2), (dims.y/2), (dims.z/2));
        v[2]  = center + V3((dims.x/2), (dims.y/2), (dims.z/2));
        v[3]  = center + V3((dims.x/2), (dims.y/2), -(dims.z/2));

        p.Create(4, v, pb);
        p.ResetTexCoords(-1);
        p.InitCalcTC(1);
        p._polyflags = POLY_EXTRUDED|POLY_INSERTED;
        p._polyprops = FACE_NOCOLLIDE|FACE_SHOWBACK;

        p._colorD   = ZWHITE;
        pb->AddPoly(p);

        DeselectAll();
        SelectFace(&pb->_polys.back(), pb);
        SetSelMode(SEL_FACE);
        Invalidate();
    }
}

void CZ_ed2Doc::OnUpdateInsface(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling ||IsSelBoxOn() || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_pSelBrushes.size()==1);
}

void CZ_ed2Doc::OnCutwithbrush() 
{
	if(_pSelPolys.size()<2)
        return;

    Poly    poly   = *(_pSelPolys.front());
    Brush*  pBrush = poly._pBrush;
    Poly    a;
    Poly    b;

    FOREACH(PPolys,  _pSelPolys, ppP)
    {
        if(ppP == _pSelPolys.begin())
            continue;

        a.CopyProps(poly);
        poly.Split((Plane&)*(*ppP),a,b); //keep front fragment
        if(a._vtci.size()>=3)
        {
            poly = a;
            b.Clear();
            a.Clear();
        }
        else
            break;
    }
     pBrush->ErasePoly(*_pSelPolys.front());
    _pSelPolys.clear();
    pBrush->AddPoly(poly);

    Invalidate();
}

void CZ_ed2Doc::OnUpdateCutwithbrush(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling ||IsSelBoxOn() || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_pSelPolys.size()>1);

}

void CZ_ed2Doc::AddBrush(Brush* pB)
{
    _scene.AddPrimitive(pB);
	Brush::GID = tmax(Brush::GID, pB->_unicID);
    _docDirty = 1;
    }

static void ClrCpy(RGBA& d, RGBA& s)
{
    memcpy(&d, &s , sizeof(RGBA));
}

void CZ_ed2Doc::OnWbck() 
{
    static RGBA	    clr_NORMAL	    [3] = { {150,150,150,255} , {32 ,32 ,32 ,255} , {11 ,11 ,11 ,255}  };
    static RGBA	    clr_DARK	    [3] = { {128,128,128,255} , {196,196,196,255} , {196,196,196,255}  };
    static RGBA	    clr_CUT		    [3] = { {180,0  ,0  ,255} , {180,0  ,0  ,255} , {180,0  ,0  ,255}  };
    static RGBA	    clr_WHITE	    [3] = { {255,255,255,255} , {0  ,0  ,0  ,255} , {0  ,0  ,0  ,255}  };
    static RGBA	    clr_SELECT	    [3] = { {193,235,137,32 } , {93 ,135,37 ,32 } , {93 ,135,37 ,32 }  };
    static RGBA	    clr_SELECTDIMED	[3] = { {163,205,107,128} , {93 ,235,37 ,32 } , {93 ,235,37 ,32 }  };
    static RGBA	    clr_SELBOX	    [3] = { {0  ,64 ,200,192} , {0  ,64 ,200,192} , {0  ,64 ,200,192}  };
    static RGBA	    clr_SELBOX_TCH  [3] = { {0  ,96 ,255,255} , {0  ,96 ,255,255} , {0  ,96 ,255,255}  };
    static RGBA	    clr_CAMSTART    [3] = { {255,0  ,0  ,255} , {54 ,0  ,0  ,255} , {54 ,0  ,0  ,255}  };
    static RGBA	    clr_CAMEND      [3] = { {64 ,0  ,0  ,255} , {255,0  ,0  ,255} , {255,0  ,0  ,255}  };
    static RGBA	    clr_CAMFRUST    [3] = { {255,0  ,0  ,255} , {128,0  ,0  ,255} , {128,0  ,0  ,255}  };
    static RGBA	    clr_SCRSURFACE  [3] = { {0  ,0  ,255,75 } , {0  ,0  ,128,75 } , {0  ,0  ,128,75 }  }; 
    static RGBA	    clr_CUT3DBRUSH  [3] = { {255,0  ,0  ,64 } , {192,0  ,0  ,64 } , {192,0  ,0  ,64 }  }; 
    static RGBA	    clr_PRJDBRUSH   [3] = { {0  ,0  ,255,64 } , {0  ,0  ,192,64 } , {0  ,0  ,192,64 }  }; 
    static RGBA	    clr_CUT3DBRUSHREV[3] ={ {0  ,255,0  ,54 } , {0  ,192,0  ,54 } , {0  ,192,0  ,54 }  }; 
    static RGBA	    clr_PORTALS     [3] = { {0  ,0  ,128,200} , {0  ,0  ,92 ,200} , {0  ,0  ,92 ,200}  };
    static RGBA	    clr_PORTALS1    [3] = { {0  ,128,0  ,200} , {0  ,92 ,0  ,200} , {0  ,92 ,0  ,200}  };
    static RGBA	    clr_PORTALSLINE [3] = { {255,255,192,255} , {0  ,0  ,192,255} , {0  ,0  ,192,255}  };
    static RGBA	    clr_BLOOR       [3] = { {255,255,255,128} , {0  ,0  ,0  ,128} , {0  ,0  ,0  ,128}  };
    static RGBA	    clr_DETAIL      [3] = { {128,128,0  ,255} , {128,128,0  ,255} , {128,128,0  ,255}  };
    static RGBA	    clr_TERRAIN     [3] = { {64 ,52 ,40 ,255} , {64 ,52 ,40 ,255} , {64 ,52 ,40 ,255}  };
    static RGBA	    clr_DETAILTRIG  [3] = { {192,255,0  ,255} , {192,64 ,0  ,255} , {192,64 ,0  ,255}  };
    static RGBA	    clr_SKYDOM      [3] = { {0  ,200,200,255} , {0  ,200,200,255} , {0  ,200,200,255}  };
    static RGBA	    clr_LIGHTBULB   [3] = { {64 ,0  ,0  ,255} , {192 ,0  ,0 ,255} , {192 ,0  ,0 ,255}  };
    static RGBA	    clr_MOTION      [3] = { {46 ,150,220,255} , {46 ,150,220,255} , {46 ,150,220,255}  };
    static RGBA	    clr_TRIGGER     [3] = { {250,84 ,84 ,255} , {250,84 ,84 ,255} , {250,84 ,84 ,255}  }; 
    static RGBA	    clr_FACEDETAIL  [3] = { {192,192,100,255} , {192,192,100,255} , {192,192,100,255}  };
    static RGBA     clr_NEW_BRITEM  [3] = { {255,147,149,255} , {192,147,149,255} , {192,147,149,255}  };
    static RGBA     clr_ZONE        [3] = { {160,60 ,160,255} , {160,60 ,160,255} , {160,60 ,160,255}  };
    static RGBA     clr_AXES_X		[3] = { {151,64 ,64 ,255} , {111,64 ,64 ,255} , {255,0  ,0 ,255}  };
    static RGBA     clr_AXES_Y		[3] = { {64 ,151,64 ,255} , {64 ,111,64 ,255} , {0  ,255,0 ,255}  };
    static RGBA     clr_AXES_Z		[3] = { {64 ,64 ,151,255} , {64 ,64 ,111,255} , {0  ,0  ,255,255}  };
    static RGBA     clr_GRIDF		[3] = { {48 ,48 ,48 ,255} , {200,200,200,255} , {190,190,190,255}  };
    static RGBA     clr_GRIDD		[3] = { {48 ,48 ,48 ,255} , {200,200,200,255} , {190,190,190,255}  };
    static RGBA     clr_FINEGRIDF	[3] = { {52 ,52 ,52 ,255} , {220,220,220,255} , {200,200,200,255}  };
    static RGBA     clr_FINEGRIDD	[3] = { {38 ,38 ,38 ,255} , {210,210,210,255} , {190,190,190,255}  };


    b_whitebk++; 
    if(b_whitebk==3)
        b_whitebk=0;


    ClrCpy( CLR_NORMAL          , clr_NORMAL       [b_whitebk]);
    ClrCpy( CLR_DARK            , clr_DARK         [b_whitebk]);
    ClrCpy( CLR_CUT             , clr_CUT          [b_whitebk]);
    ClrCpy( CLR_WHITE           , clr_WHITE        [b_whitebk]);
    ClrCpy( CLR_SELECT          , clr_SELECT       [b_whitebk]);
    ClrCpy( CLR_SELECTDIMED     , clr_SELECTDIMED  [b_whitebk]);
    ClrCpy( CLR_SELBOX          , clr_SELBOX       [b_whitebk]);
    ClrCpy( CLR_SELBOX_TCH      , clr_SELBOX_TCH   [b_whitebk]);
    ClrCpy( CLR_CAMSTART        , clr_CAMSTART     [b_whitebk]);
    ClrCpy( CLR_CAMEND          , clr_CAMEND       [b_whitebk]);
    ClrCpy( CLR_CAMFRUST        , clr_CAMFRUST     [b_whitebk]);
    ClrCpy( CLR_SCRSURFACE      , clr_SCRSURFACE   [b_whitebk]);
    ClrCpy( CLR_CUT3DBRUSH      , clr_CUT3DBRUSH   [b_whitebk]);
    ClrCpy( CLR_PRJDBRUSH       , clr_PRJDBRUSH    [b_whitebk]);
    ClrCpy( CLR_BLOOR           , clr_BLOOR        [b_whitebk]);
    ClrCpy( CLR_CUT3DBRUSHREV   , clr_CUT3DBRUSHREV[b_whitebk]);
    ClrCpy( CLR_PORTALS         , clr_PORTALS      [b_whitebk]);
    ClrCpy( CLR_PORTALS1        , clr_PORTALS1     [b_whitebk]);
    ClrCpy( CLR_PORTALSLINE     , clr_PORTALSLINE  [b_whitebk]);
    ClrCpy( CLR_DETAIL          , clr_DETAIL       [b_whitebk]);
    ClrCpy( CLR_TERRAIN         , clr_TERRAIN      [b_whitebk]);
    ClrCpy( CLR_DETAILTRIG      , clr_DETAILTRIG   [b_whitebk]);
    ClrCpy( CLR_LIGHTBULB       , clr_LIGHTBULB    [b_whitebk]);
    ClrCpy( CLR_SKYDOM          , clr_SKYDOM       [b_whitebk]);
    ClrCpy( CLR_MOTION          , clr_MOTION       [b_whitebk]);
    ClrCpy( CLR_TRIGGER         , clr_TRIGGER      [b_whitebk]);
    ClrCpy( CLR_FACEDETAIL      , clr_FACEDETAIL   [b_whitebk]);
    ClrCpy( CLR_FINEGRIDF	    , clr_FINEGRIDF	  [b_whitebk]);
    ClrCpy( CLR_GRIDF		    , clr_GRIDF		  [b_whitebk]);   
    ClrCpy( CLR_GRIDD		    , clr_GRIDD		  [b_whitebk]);   
    ClrCpy( CLR_FINEGRIDD	    , clr_FINEGRIDD	  [b_whitebk]);
    ClrCpy( CLR_AXES_X		    , clr_AXES_X		  [b_whitebk]);
    ClrCpy( CLR_AXES_Y		    , clr_AXES_Y		  [b_whitebk]);
    ClrCpy( CLR_AXES_Z		    , clr_AXES_Z		  [b_whitebk]);


    Invalidate();
}

extern UINT UTimer;
void CZ_ed2Doc::OnPaintimer() 
{
    if(UTimer)
    {
        FRM()->KillTimer(UTimer);
        UTimer = 0;
    }
    else
    {
        UTimer = FRM()->SetTimer(332,100,0);
    }    	
}

Htex    CZ_ed2Doc::RecreateAmbLmap()
{
    if(_ambLmap)
        TexRef::RemoveTex((Htex*)&_ambLmap ,1);

    BYTE* loco = new BYTE[16*16*3];
    int k=0;
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<16;j++)
        {
            loco[k++] = _scene._si.defLumin.r;
            loco[k++] = _scene._si.defLumin.g;
            loco[k++] = _scene._si.defLumin.b;
        }
    }
    Htex rv =  TexRef::GlGenTex( 16, 16, 3 , loco, 0);
    delete[] loco;
    return rv;
}



void CZ_ed2Doc::OnUpdateCombo(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA)
    {
        FRM()->m_combobox.EnableWindow(FALSE);
        return;
    }
    FRM()->m_combobox.EnableWindow(1);
}

void CZ_ed2Doc::OnUpdateCombots(CCmdUI* pCmdUI) 
{
    
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA)
    {
        FRM()->m_comboboxts.EnableWindow(FALSE);
        return;
    }
    FRM()->m_comboboxts.EnableWindow(1);
	
}

void CZ_ed2Doc::OnImpp() 
{
	REDIR();
    LPTSTR lpszPathName = NULL;
    char szFilters[] = _T("Geometry raw (*.grf)|*.grf|");  

    // Prompt the user for a filename
    CFileDialog dlg(TRUE, _T("grf"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    dlg.m_ofn.lpstrTitle = _T("Select Geometry File");
    
    dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();
    if (theApp.DoFileDialog(dlg) != IDOK) return;
    
    PathHandler phSrc(dlg.m_ofn.lpstrFile);
    
    theApp.DocsDir(phSrc.Path());

    CString     dstDir = theApp.HomeDir();
    dstDir += "\\maps\\";
    mkdir(dstDir);

    CString     dstDirRes = theApp.HomeDir();
    dstDirRes += "\\res\\";
    mkdir(dstDir);

    ::CopyFile(dlg.m_ofn.lpstrFile, MKSTR("%s%s",dstDir, phSrc.File()),0);
    
    ::G_CopyFolder(dstDirRes, MKSTR("%s%s\\",phSrc.Path(), phSrc.JustFile()), "*.*");

	
}

void CZ_ed2Doc::OnUpdateImpp(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(1);
}

void CZ_ed2Doc::OnExpp() 
{
    const char* pfld = BrowseForFolder("Select Folder",  theApp.DocsDir());
    if(0 == pfld)
        return;
	CString folderDst  = pfld;

    CString curFile    = GetTitle();
    if(curFile=="NoName")
    {
        AfxMessageBox("Cannot save an unamend file", MB_OK|MB_ICONWARNING);
        return;
    }
    theApp.DocsDir(folderDst);
    //
    // make dest res dir
    //
    PathHandler ph(curFile);

    ::mkdir(folderDst);
    CString folderDstRes = folderDst + ph.JustFile();
    folderDstRes+="_res\\";
    ::mkdir(folderDstRes);

    ::CopyFile(curFile, MKSTR("%s%s",folderDst,ph.File()),0);

    //
    // copy all textures used in this map
    //
    CString resDir = theApp.HomeDir();
    resDir += "\\res\\";
    TexSys::iterator bt = GTexSys.begin();
    TexSys::iterator et = GTexSys.end();
    for(;bt != et; bt++)
    {
        Texture& tex      = (*bt).second;
		if(!strncmp(tex.filename, "@L",2))
			continue;
        CopyFile(MKSTR("%s%s",resDir, tex.filename), MKSTR("%s%s",folderDstRes,tex.filename), 0);
    }

    //
    // copy sounds
    //
    vvector<CSoundItem*>&  sounds = *_scene.GetSounds();
    FOREACH(vvector<CSoundItem*>, sounds, ppsound)
    {
        CSoundItem* psi = *ppsound;
        CopyFile(MKSTR("%s%s",resDir, psi->_soundfile), MKSTR("%s%s",folderDstRes,psi->_soundfile), 0);
    }
    //
    // copy scripts
    //
    vvector<ScriptItem*>&  scripts = *_scene.GetScripts();
    FOREACH(vvector<ScriptItem*>, scripts, ppscript)
    {
        ScriptItem* psi = *ppscript;
        CopyFile(MKSTR("%s%s",resDir, psi->_name), MKSTR("%s%s",folderDstRes,psi->_name), 0);
    }


}

void CZ_ed2Doc::OnUpdateExpp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_scene.GetPrimitives()->empty() && _docDirty==0 && GetTitle()!="NoName");
	
}

CString CZ_ed2Doc::ResFile(const char* filenme)
{
    CString retval;
    CString cst = GetTitle();
    if(cst == "NoName")
    {
        retval = theApp.HomeDir();
        retval += "\\res\\";
        retval += filenme;
        return retval;
    }
    PathHandler ph(cst);
    retval = ph.Path();
    retval += MKSTR("%s_res\\",ph.JustFile());
    mkdir(retval);
    retval += filenme;
    return retval;
}


BOOL    CZ_ed2Doc::DuplicateFile(const char* src, BOOL over)
{
    CString resDir = theApp.HomeDir();
   
    resDir += "\\res\\";
    PathHandler ph(src);
    resDir += ph.File();

    if(resDir!=src)
        ::CopyFile(src, resDir, over);
    CString& rsf = ResFile(ph.File());
    if(rsf != src)
        ::CopyFile(src, ResFile(ph.File()), over);
    return 1;
}

void    CZ_ed2Doc::CopyResources(const char* srcFile)
{
    CWaitCursor cwc;

    PathHandler ph(srcFile);
    CString resDir = theApp.HomeDir();
    resDir += "\\res\\";
    ::G_CopyFolder(resDir, MKSTR("%s%s\\",ph.Path(),ph.JustFile()), "*.*");
}


void CZ_ed2Doc::OnLeafs() 
{
    if(!::IsWindow(FRM()->_dlgleafs.m_hWnd))
        FRM()->CreateLeafExplorer();
    else
        FRM()->DestroyLeafExplorer();
}

void CZ_ed2Doc::OnUpdateLeafs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_compviewmode & (C_LBYL|C_GRAPH));
    pCmdUI->SetCheck(::IsWindow(FRM()->_dlgleafs.m_hWnd));
}



void CZ_ed2Doc::OnUpdateColor(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || DOC()->_compviewmode != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
    pCmdUI->Enable(_pSelBrushes.size() || _pSelItems.size() || _pSelVertexes.size()|| _pSelPolys.size()==1);
    SBAR().SetSelection(_pSelItems.size() || _pSelPolys.size() || _pSelBrushes.size() || _pSelVertexes.size());
	
}

void CZ_ed2Doc::OnColor() 
{
    
    CColorDialog    d(RGB(200,200,200), CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal()==IDOK)
    {
        CWaitCursor cwc;
        COLORREF crf = d.GetColor();
        
        crf &= 0x00FEFEFE;

        if(_pSelBrushes.size())
        {
            _SeColor(crf, _pSelBrushes);
        }
        else if(_pSelItems.size())
        {
            _SeColor(crf, _pSelItems);
        }
        else if(_pSelVertexes.size())
        {
            _SeColor(crf, _pSelVertexes);
        }
        else if(_pSelPolys.size())
        {
            _SeColor(crf, _pSelPolys);
        }
        BARDLG()->Refresh();
        Invalidate();
    }	
}


void CZ_ed2Doc::_SeColor(COLORREF crf, PBrushes& pSelBrushes)
{
    FOREACH(PBrushes, _pSelBrushes, ppe)
    {
        FOREACH(vvector<Poly>, (*ppe)->_polys, pe)
        {
            FOREACH(vvector<Vtx2>, (pe)->_vtci, pv)
            {
                pv->_rgb.r = GetRValue(crf);
                pv->_rgb.g = GetGValue(crf);
                pv->_rgb.b = GetBValue(crf);
            }
        }
    }
}

void CZ_ed2Doc::_SeColor(COLORREF crf, vvector<SceItem*>& pSelItems)
{
    FOREACH(vvector<SceItem*>, pSelItems, ppe)
    {
        (*ppe)->_colorD.r = GetRValue(crf);
        (*ppe)->_colorD.g = GetGValue(crf);
        (*ppe)->_colorD.b = GetBValue(crf);
    }
}


void CZ_ed2Doc::_SeColor(COLORREF crf, vvector<Vtx2*>& pSelVertexes)
{
    FOREACH(vvector<Vtx2*>, pSelVertexes, ppe)
    {
        (*ppe)->_rgb.r = GetRValue(crf);
        (*ppe)->_rgb.g = GetGValue(crf);
        (*ppe)->_rgb.b = GetBValue(crf);
    }
}

void CZ_ed2Doc::_SeColor(COLORREF crf, PPolys& pSelPolys)
{
    FOREACH(PPolys, pSelPolys, pe)
    {
        FOREACH(vvector<Vtx2>, (*pe)->_vtci, pv)
        {
            pv->_rgb.r = GetRValue(crf);
            pv->_rgb.g = GetGValue(crf);
            pv->_rgb.b = GetBValue(crf);
        }
    }

}
