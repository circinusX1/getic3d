// z_ed2Doc.cpp : implementation of the CZ_ed2Doc class
//

#include "stdafx.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "mainfrm.h"
#include "Z_ed2View.h"
#include "Z_ed3View.h"

#include <algorithm>
#include "BspTree.h"
#include "DlgCompile.h"

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
// make csg
void CZ_ed2Doc::DoHSR() 
{
    SaveTempFile();

    _scene.ClearBrushes();
	if(AKEY(VK_CONTROL)) // MERGES BRUSHES
	{
		
		
		Brush* pResultSceneBrsh = new Brush();

		pResultSceneBrsh->_brushflags=BRSH_SOLID;

		PBrushes* pBrushes =  _scene.GetPrimitives();
		if( 0 == pBrushes->size())
			return ;

		FOREACH(PBrushes, (*pBrushes), ppBrsh)
		{
			FOREACH(vvector<Poly>,(*ppBrsh)->_polys, pPoly)
			{
				pResultSceneBrsh->_polys << *pPoly;
			}
		}

		pResultSceneBrsh->Recalc();

		if(pResultSceneBrsh->_polys.size())
			_scene.AddBrush(pResultSceneBrsh);
		else
			delete pResultSceneBrsh;
	}
	else
	{
		size_t dw = GetTickCount();
		_compiler.PerformHSR(_scene.GetPrimitives());
		dw = GetTickCount()-dw;
		SBT(0,MKSTR("%d polygons in %d ms", _compiler._polysStats, dw));
	}
	/*
    #pragma message("see thsi save infile later ????")
    if(_scene.GetBrushes()->size())
    {
        CString cs = GetTitle();
        cs+=".cg";
        SaveInFile(cs,TRUE);
	    END_WAIT("");
    }
	*/
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnCompileroptions() 
{

    char   docName[PATH_MAX];
    ::strcpy(docName, DOC()->GetTitle());
    PathHandler ph(docName);

    _compiler._finalBSpFileName = theApp.BspsDir();
    _compiler._finalBSpFileName += ph.JustFile();
    _compiler._finalBSpFileName += ".gbt";

    CDlgCompile d;

    _compiler.FromRegistry(1);

    d._ckSave       = _compiler._bSaveFinalBSP;
    d._szipit       = _compiler._szipit;
    d._fileName     = _compiler._finalBSpFileName;
    d._bSaveImages  = _compiler._bSaveImages ;
    d._lmapIntens  = _compiler._lmapIntens ;
    d._maxFacesLeaf = _compiler._maxFacesLeaf ;
    d._bSavePortals = _compiler._bSavePortals ;
    d._bPolySortCrit = _compiler._bPolySortCrit;
    d._bMergeNarrowLeafs = _compiler._bMergeNarrowLeafs;
    d._bSaveSkyDom  = _compiler._bSaveSkyDom ;
    d._bLightMaps   = _compiler._bLightMaps; 
    d._lmpowof2     = _compiler._lmpowof2; 
    d._lmConst     = _compiler._lmConst;
    d._bConvexEarly = _compiler._bConvexEarly;
    d._b3SampleLM   = _compiler._b3SampleLM; 

    d._bSaveBigTerrainBSP = _compiler._bSaveBigTerrainBSP;
    d._bSaveBigTerrain    = _compiler._bSaveBigTerrain;


//    d._bAvoidNonOrt =   _bAvoidNonOrt;
    //d._bUseUserPriority =_bUseUserPriority;


    d._bSnapMainBsp     = _compiler._bSnapMainBsp;
    d._bPvsMethod     = _compiler._bPvsMethod;
    d._bGeticBSP    = _compiler._bGeticBSP;
    d._bZones     = _compiler._bZones;
    d._bCompressPvs= _compiler._bCompressPvs;
    d._bAABBTreeCsgTer= _compiler._bAABBTreeCsgTer;


    d._bRemopl      = _compiler._removeOnePolyLeaf;
    d._portalCutFlag = _compiler._portalCutFlag;
    d._balance      = _compiler._balance;
    d._bExportFile   = _compiler._bExportFile;
    d._lmapMaxSize       = _compiler._lmapMaxSize;
    d._bLmPatch          = _compiler._bLmPatch;
    d._lMapLumelSz       = _compiler._lMapLumelSz;


    if(d.DoModal() == IDOK)
    {
        BARDLG()->OnApply();

        OnEscape();
        _compiler._bSaveFinalBSP    = d._ckSave;
        _compiler._finalBSpFileName = d._fileName;
        _compiler._szipit       = d._szipit;
        _compiler._bSaveImages  = d._bSaveImages;
        _compiler._lmapIntens    = d._lmapIntens;
        _compiler._maxFacesLeaf = d._maxFacesLeaf;
        _compiler._bSavePortals = d._bSavePortals;
        _compiler._bPolySortCrit = d._bPolySortCrit;
        _compiler._bMergeNarrowLeafs =d._bMergeNarrowLeafs; 
        _compiler._bSaveSkyDom  = d._bSaveSkyDom;

        _compiler._bSaveBigTerrainBSP = d._bSaveBigTerrainBSP;
        _compiler._bSaveBigTerrain    = d._bSaveBigTerrain;

//        _compiler._bAvoidNonOrt =   d._bAvoidNonOrt;
//        _compiler._bUseUserPriority = d._bUseUserPriority;

        _compiler._bConvexEarly = 0;//d._bConvexEarly;
        _compiler._bSnapMainBsp = d._bSnapMainBsp;
        _compiler._bPvsMethod     = d._bPvsMethod;
        _compiler._bGeticBSP    = d._bGeticBSP;
        _compiler._bZones        = d._bZones;

        _compiler._bCompressPvs = d._bCompressPvs;
        _compiler._bAABBTreeCsgTer = d._bAABBTreeCsgTer;

        _compiler._removeOnePolyLeaf = d._bRemopl;
        _compiler._portalCutFlag = d._portalCutFlag;
        _compiler._balance           = d._balance;
        _compiler._bExportFile   = d._bExportFile;

        _compiler._lmpowof2     = d._lmpowof2; 
        _compiler._lmConst     = d._lmConst;
         _compiler._lmapMaxSize = d._lmapMaxSize;
         _compiler._bLmPatch    = d._bLmPatch;
         _compiler._lMapLumelSz = d._lMapLumelSz;
         _compiler._b3SampleLM  = d._b3SampleLM; 

        
        _compiler.FromRegistry(0);
    }
	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnBucompile() 
{
    if(_compiling == 0)
    {
        _compiling = TRUE;
        SaveTempFile();
        OnFileSave() ;
        // up all buttons view modes selections  etc
        BARDLG()->OnApply();

        _compviewmode = C_NA;
        _brmode       = BR_VIEW;
        _bSnapGrid    = FALSE;
        _selmode      = SEL_NONE;
        _vmodefire    = FALSE;
        _vmodeback    = FALSE;
        _bShowCsg     = FALSE;
        _compviewmode  = C_NA;
        _debugPolys.clear();

        LOG(I_DONE,"+");
        DisableAll(TRUE);
        FRM()->ShowProgressDlg(TRUE);
        FRM()->Invalidate();
        FRM()->UpdateWindow();
        _compiler.FromRegistry(1);
        _compiler.Compile(_scene);
    }
    else
    {
        CWaitCursor cwc; // if not var is excluded in release(1);
        _complBreaking = TRUE;
        _compiler.Break();
        START_WAIT("Please Wait. Breaking Compilation...");
    }
}

//---------------------------------------------------------------------------------------
LRESULT CZ_ed2Doc::OnCompileDone(WPARAM wErr, LPARAM pStr)
{
    PROG(-1);
    if(_complBreaking)
    {
        END_WAIT("");
    }
    if(0 ==_compiling)
    {
        SBT(0, "Ready...");
        return 0;
    }
    _compiling     = 0;
    _complBreaking = 0;

    _compiler.Stop();
    Sleep(32);
    OnEscape();
    
    CZ_ed2Doc::DisableAll(FALSE);
	BOOL bchb = _compiler.HasBreaked();// 
	_compiler.Break(0);

    if(bchb)
	{
        CWaitCursor cwc; // if not var is excluded in release(0);
	}

    if(wErr !=0 )
    {
        if(bchb)
        {
            LOG(0,"! Incomplette compilation. User break. ");
        }
        else
        {
            LOG(0,"!! Error compiling level ");
        }
		_compiler.Clear();
        return 0;
    }
    _compiler.CommitChanges();
	
	LOG(0,"BSP Details:--------------------------------------------------------");
    int totPolys = 0;
    FOREACH(vvector<CBspTree*>, _compiler._bTrees, pBtree)
    {   
		LOG(0,"BSP: '%s', Nodes :  %d, Polygons: %d, Leafs: %d", (*pBtree)->_name,
																(*pBtree)->_nodesPtr.size(),
																(*pBtree)->_polys.size(),
																(*pBtree)->_leafs.size());

        totPolys+=(*pBtree)->_polys.size();
    }

	LOG(0,"Compilation Details:------------------------------------------------");
    LOG(0,"Portals: %d, Leafs: %d, PVS: %d b, BSP's: %d",_compiler._portPrc._portals.size(),
															_compiler._pvsPrc._nleafs,
															_compiler._pvsPrc._cPvss,
															_compiler._bTrees.size());

    LOG(0,"Scene Details:------------------------------------------------------");
    LOG(0,"Number of Polygons: %d",totPolys);
    LOG(0,"Number of Textures: %d",GTexSys.size());
    
    SaveFinalBSP();

    if(_compiler._errDetected)
    {
        LOGERR(E_BSP,V0,"There were some warnings detected. Scroll up to ckeck ! ");
    }
    _compiler._errDetected=0;

    if(pStr)
    {
        Brush* pBrush = (Brush* )pStr;
        DOC()->SelectBrush(pBrush);
        DOC()->MoveCamera(pBrush->_box.GetCenter());
        DOC()->Invalidate();
    }

    
    return 0;
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateBucompile(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(_scene.GetPrimitives()->size()!=0);
    pCmdUI->SetCheck(_compiling);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateCompileroptions(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_compiling);
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnBspcursel() 
{
    int nCount;
    Brush** b = GetSelBrushes(nCount);
    for(int i=0;i<nCount;i++,b++)
    {
		MiniBsp test;
		BOOL    result;
		
		test._treeprops = (*b)->_brushprops;
		result = test.Compile((*b)->_polys, test._treeprops);
		if(result)
			SBT(0,MKSTR("P: %d / %d D: %d N: %d",(*b)->_polys.size(), test._polys.size(), test._reachedDepth, test._nodesPtr.size() ));	
		else
			AfxMessageBox("Failed to build BSP");
    }
}


void CZ_ed2Doc::OnReclmaps() 
{
    CWaitCursor cwc ; 

    _compviewmode = C_NA;
    _brmode       = BR_VIEW;
    _bSnapGrid    = FALSE;
    _selmode      = SEL_NONE;
	_vmodefire    = FALSE;
	_vmodeback    = FALSE;
    _bShowCsg     = FALSE;
    _compviewmode  = C_NA;


	BARDLG()->OnApply();

	
    _compiler.DoLightMaps();
    SaveFinalBSP();
	
	
}

void CZ_ed2Doc::OnUpdateReclmaps(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_compiler.Done()!=0);

}

void CZ_ed2Doc::SaveFinalBSP()
{
    if(!_compiler._finalBSpFileName.IsEmpty())
    {
        if(_compiler._exBspPlugin.GetLength())
        {
            _compiler.ExportBSP(_compiler._finalBSpFileName);
        }
        else
        if(_compiler.Save(_compiler._finalBSpFileName))
        {
            if(_compiler._bExportFile)
            {
                _compiler.Export(_compiler._finalBSpFileName);
            }
            LOG(0,"File: '%s' saved Succesfully",_compiler._finalBSpFileName);
        }
        else
        {
            LOG(0,"Error:  saving '%s' ",_compiler._finalBSpFileName);
        }

    }
    else
    {
        LOG(0,"Error:  saving '%s' ",_compiler._finalBSpFileName);
    }
}


