// z_ed2Doc.cpp : implementation of the CZ_ed2Doc class
//

#include "stdafx.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "mainfrm.h"
#include "Z_ed2View.h"
#include "Z_ed3View.h"
#include <algorithm>
#include "DlgNewBrush.h"
#include "DlgNewPref.h"
#include "DlgClassItem.h"
#include "DlgNewTerrain.h"
#include "DlgEditor2D.h"
#include "geticplug.h"
#include "geticgui.h"
#include "DlgNewItem.h"
#include "DlgBigTerrain.h"
#include "LightBulb.h"

#ifndef APSTUDIO_INVOKED
    #define APSTUDIO_INVOKED
#endif //
#ifdef APSTUDIO_READONLY_SYMBOLS
    #undef APSTUDIO_READONLY_SYMBOLS
#endif // 

//---------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
static int	GID=0;

static V3 GetViewsDims()
{

    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);

	V3	boxdim = V3(  pTZ->_vport.right-pTX->_vport.left,
					  pTX->_vport.bottom-pTZ->_vport.top,
			          pTX->_vport.right-pTX->_vport.left);
	boxdim *= .3f;

    return boxdim;
}
//---------------------------------------------------------------------------------------
// set the flags from setting dialog into the brush
void CZ_ed2Doc::ApplyFlags(Brush* pBrush)
{
    _docDirty = 1;
    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);
	z_ed3View*  pT3 = VIEW3D(0,0);

    pBrush->Move(V3(pTY->_shiftx, pTZ->_shifty, -pTY->_shifty));
	pBrush->Recalc();

    if(_cachedSett._splitter)
    {
        pBrush->_brushflags = BRSH_SPLITTER;
    }
    else if(_cachedSett._zone)
    {
        pBrush->_brushflags = BRSH_ZONE;
    }
    else
    {
        if(_cachedSett._isSolid) // cut
        {
		    pBrush->_brushflags |= BRSH_SOLID;
		    if(_cachedSett._detail)
		    {
			    pBrush->_brushflags |= BRSH_DETAIL;
                if(_cachedSett._detached)
                {
                    pBrush->_brushflags |= BRSH_DETACHED;
                }
		    }
        }
        else
	    {
		    pBrush->FlagFaces(POLY_FROM_CUTB);
		    if(_cachedSett._cutAll)
		    {
			    pBrush->_brushflags |= BRSH_CUTALL;
		    }
		    pBrush->_brushflags &= ~BRSH_DETAIL;
            pBrush->_brushflags &= ~BRSH_DETACHED;
	    }
    }


	if(_cachedSett._reversed)	
	{
        pBrush->Reverse();

        if(_cachedSett._skyDom)
        {
            pBrush->_brushprops |= MODEL_IS_SKYDOM;
        }
	}
	pBrush->_group = _curCreatGroup;
    pBrush->_brushflags |= BRSH_NEW;


    if(GDetEnabled && (pBrush->_brushflags & BRSH_SOLID))
    {
        pBrush->Recalc();
        V3 ex = pBrush->_box.GetExtends();
        if(ex.getmax() < GMaxDetDim && 
           ex.getmin() < GMinDetDim)
        {
            pBrush->_brushflags |= BRSH_DETAIL;   
            if(_cachedSett._detached)
            {
                pBrush->_brushflags |= BRSH_DETACHED;
            }

        }
        else
        {
            pBrush->_brushflags &= ~BRSH_DETAIL;
            pBrush->_brushflags &= ~BRSH_DETACHED;
        }

        if(pBrush->_polys.size() > (UINT)GMinDetPolys)
        {
            pBrush->_brushflags |= BRSH_DETAIL;    
        }
    }


	pBrush->Reverse();

	//
	// gens lmap tex cooord and stores initial texture coords
	//
    if(pBrush->_cf._tm == TM_LIN)
    {
        pBrush->ResetTexCoords(-1);	//FOR LIGHT MAP
    }
    pBrush->ResetTexCoords(TX_LM);	//FOR LIGHT MAP

	FOREACH(vvector<Poly>,pBrush->_polys,pPoly)
	{
		Poly& poly = *pPoly;

        if(poly.GetHtex(GUtex) <= 0)
        {
    		poly.SetTex(BARDLG()->GetCurSelTexture(), GUtex , BARDLG()->GettexGenFlag());
        }
        FOREACH(vvector<Vtx2>,poly._vtci,pvtci)
        {
            pvtci->_rgb         = ZWHITE;   
            pvtci->_uv[TX_1]    = pvtci->_uv[TX_0];
            pvtci->_uv[TX_2]    = pvtci->_uv[TX_0];
        }
	}

	
 	_pSelBrushes.push_back(pBrush); //place in sel brushes witht eh new flag set
	Invalidate();
	pT3->SetFocus();

	_selmode		= SEL_BRUSH;
	_bShowCsg		= 0;
    _compviewmode   = C_NA;

    _maxUsedGrp     = tmax(_maxUsedGrp, _curCreatGroup);
    SaveTempFile();
    _brmode = BR_SCALE;
}


//---------------------------------------------------------------------------------------
// makes a box by setings dialog
void CZ_ed2Doc::OnMkBox() 
{
	//OnFileSave();
    OnEscape() ;                    //clear any initiated action
    SaveTempFile();

    _cachedSett._boxdim = GetViewsDims();

    _newBrshDlg._brshSett=_cachedSett;
    _newBrshDlg._hideZone=FALSE;
    int rez = _newBrshDlg.DoModal();

	if(rez==IDCANCEL)
		return;

    _cachedSett = _newBrshDlg._brshSett;
    _selmode = SEL_BRUSH;
	
	Brush*    pBrush   = new Brush();

	pBrush->MakeCube(_cachedSett._boxdim, _cachedSett._tm);

	if(_cachedSett._thikness != 0)
	{
		pBrush->StartSecGrp(_cachedSett._thikness);

	    V3 intBox       = _cachedSett._boxdim - _cachedSett._thikness;
	    Brush	 b2;
	    b2.MakeCube(intBox, _cachedSett._tm);
	    b2.Reverse();
	    FOREACH(Polys,b2._polys,pp)
        {
            pp->_pBrush = pBrush;
		    pBrush->_polys.push_back(*pp);
        }
    }
    ApplyFlags(pBrush);

    pBrush->_cf._tm      = _cachedSett._tm;
    pBrush->_cf._brushflags   = pBrush->_brushflags;
    pBrush->_cf._hstrips = 0;
    pBrush->_cf._vstrips = 0;
    pBrush->_cf._thick   = _cachedSett._thikness;
    pBrush->_cf._t    = E_CUBE;

}

//---------------------------------------------------------------------------------------
// makes 2 boxes one insode of other by confing settings
void CZ_ed2Doc::OnMkShellBox(V3& vdim) 
{
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::MkCylinder() 
{
//	OnFileSave();
    OnEscape() ;    //cancel any action because we've begin a new one
    if(_cachedSett._radstrips<0)
        _cachedSett._radstrips=16;
    if(_cachedSett._vstrips<0)
        _cachedSett._vstrips=1;

    _cachedSett._boxdim = GetViewsDims();
    
    _newBrshDlg._brshSett = _cachedSett;
    _newBrshDlg._hideZone=TRUE;
    if(IDCANCEL == _newBrshDlg.DoModal())
        return;
    _cachedSett = _newBrshDlg._brshSett;
    _selmode = SEL_BRUSH;

    if(_cachedSett._radstrips<3  || _cachedSett._radstrips>32 )
	{
        AfxMessageBox("Cylinder Radial Strips are between 3 and 32",MB_OK|MB_ICONINFORMATION);
        return;
    }

    
    REAL    rad        = tmin(_cachedSett._boxdim.x,_cachedSett._boxdim.z) / 2;
    REAL    height     = _cachedSett._boxdim.y; 

    Brush* pBrush = new Brush();
    pBrush->MakeCylinder(rad, height, _cachedSett._radstrips, _cachedSett._vstrips, _cachedSett._tm);

	if(_cachedSett._thikness != 0)
	{
		pBrush->StartSecGrp(_cachedSett._thikness);

    	Brush tb;
        tb.MakeCylinder(rad-_cachedSett._thikness, 
                        height-_cachedSett._thikness, _cachedSett._radstrips, _cachedSett._vstrips, _cachedSett._tm);
	    

	    tb.Reverse();
	    FOREACH(Polys,tb._polys,pp)
        {
            pp->_pBrush = pBrush;
		    pBrush->_polys.push_back(*pp);
        }
	}
    ApplyFlags(pBrush);

    pBrush->_cf._tm      = _cachedSett._tm;
    pBrush->_cf._brushflags   = pBrush->_brushflags;
    pBrush->_cf._hstrips = _cachedSett._radstrips;
    pBrush->_cf._vstrips = _cachedSett._vstrips;
    pBrush->_cf._thick   = _cachedSett._thikness;
    pBrush->_cf._t    = E_CYL;


}


void CZ_ed2Doc::OnBucone() 
{
	//OnFileSave();
    OnEscape() ;    //cancel any action because we've begin a new one
    if(_cachedSett._radstrips<0)
        _cachedSett._radstrips=16;
    if(_cachedSett._vstrips<0)
        _cachedSett._vstrips=1;
    
    _cachedSett._boxdim = GetViewsDims();
    
    _newBrshDlg._brshSett = _cachedSett;
    _newBrshDlg._hideZone=TRUE;
    if(IDCANCEL == _newBrshDlg.DoModal())
        return;
    _cachedSett = _newBrshDlg._brshSett;
    _selmode = SEL_BRUSH;

    if(_cachedSett._radstrips<3 ||_cachedSett._radstrips > 32 )
	{
        AfxMessageBox("Cone Radial Strips Bands are between 3 and 32",MB_OK|MB_ICONINFORMATION);
        return;
    }

   
    REAL    rad        = tmin(_cachedSett._boxdim.x,_cachedSett._boxdim.z) / 2;
    REAL    height     = _cachedSett._boxdim.y; 

    Brush* pBrush = new Brush();
    	
    pBrush->MakeCone(rad, height, _cachedSett._radstrips, _cachedSett._vstrips, _cachedSett._tm);
	if(_cachedSett._thikness != 0)
	{
		pBrush->StartSecGrp(_cachedSett._thikness);

    	Brush tb;
        tb.MakeCone(rad-_cachedSett._thikness, 
                        height-_cachedSett._thikness, _cachedSett._radstrips, _cachedSett._vstrips, _cachedSett._tm);
	    

	    tb.Reverse();
	    FOREACH(Polys,tb._polys,pp)
        {
            pp->_pBrush = pBrush;
		    pBrush->_polys.push_back(*pp);
        }
	}
    ApplyFlags(pBrush);

    pBrush->_cf._tm      = _cachedSett._tm;
    pBrush->_cf._brushflags   = pBrush->_brushflags;
    pBrush->_cf._hstrips = _cachedSett._radstrips;
    pBrush->_cf._vstrips = _cachedSett._vstrips;
    pBrush->_cf._thick   = _cachedSett._thikness;
    pBrush->_cf._t    = E_CONE;

}

void CZ_ed2Doc::OnBusphere() 
{
   OnEscape() ;    //cancel any action because we've begin a new one
    if(_cachedSett._radstrips<=0)
        _cachedSett._radstrips=16;
    if(_cachedSett._vstrips<=0)
        _cachedSett._vstrips=16;

   _cachedSett._boxdim = GetViewsDims();
    _newBrshDlg._brshSett = _cachedSett;
    _newBrshDlg._hideZone=TRUE;
    if(IDCANCEL == _newBrshDlg.DoModal())
        return;
    _cachedSett = _newBrshDlg._brshSett;
    _selmode = SEL_BRUSH;

#ifndef DEBUG
    if(_cachedSett._radstrips < 4 || _cachedSett._vstrips < 4 || 
        _cachedSett._radstrips>18 || _cachedSett._vstrips>18){
        AfxMessageBox("Sphere Vertical/Horizontal Strips  are between 4 and 18", MB_OK|MB_ICONINFORMATION);
        return;
    }
#endif//

    Brush* pBrush = new Brush();
    REAL    rad   = tmin(_cachedSett._boxdim.x,_cachedSett._boxdim.z) / 2;

    pBrush->MakeSphere(rad, _cachedSett._radstrips,_cachedSett._vstrips, _cachedSett._tm);
    
    
    if(_cachedSett._thikness != 0)
	{
		pBrush->StartSecGrp(_cachedSett._thikness);

    	Brush tb;
        tb.MakeSphere(rad-_cachedSett._thikness,  _cachedSett._vstrips,_cachedSett._radstrips, _cachedSett._tm);
	    

	    tb.Reverse();
	    FOREACH(Polys,tb._polys,pp)
        {
            pp->_pBrush = pBrush;
		    pBrush->_polys.push_back(*pp);
        }
	}
    
    ApplyFlags(pBrush);
    
    pBrush->_cf._tm      = _cachedSett._tm;
    pBrush->_cf._brushflags   = pBrush->_brushflags;
    pBrush->_cf._hstrips = _cachedSett._radstrips;
    pBrush->_cf._vstrips = _cachedSett._vstrips;
    pBrush->_cf._thick   = _cachedSett._thikness;
    pBrush->_cf._t    = E_SPHERE;

}


void CZ_ed2Doc::OnBustairs() 
{
   OnEscape() ;    //cancel any action because we've begin a new one
    if(_cachedSett._radstrips<0)
        _cachedSett._radstrips=8;
    if(_cachedSett._vstrips<0)
        _cachedSett._vstrips=1;
    
    _cachedSett._boxdim = GetViewsDims();
    
    _newBrshDlg._brshSett = _cachedSett;
    _newBrshDlg._hideZone=TRUE;
    if(IDCANCEL == _newBrshDlg.DoModal())
        return;
    _cachedSett = _newBrshDlg._brshSett;
    _selmode = SEL_BRUSH;

    if(_cachedSett._vstrips < 2 || _cachedSett._vstrips>32)
	{
        AfxMessageBox("Stair Case Vertical Strips  are between 2 and 32",MB_OK|MB_ICONINFORMATION);
        return;
    }


    Brush* pBrush = new Brush();

    
    pBrush->MakeStairs(_cachedSett._boxdim, _cachedSett._radstrips, _cachedSett._vstrips, _cachedSett._thikness, _cachedSett._tm);
    ApplyFlags(pBrush);
    pBrush->_brushflags |= BRSH_DETAIL;
    
    pBrush->_cf._tm      = _cachedSett._tm;
    pBrush->_cf._brushflags   = pBrush->_brushflags;
    pBrush->_cf._hstrips = _cachedSett._radstrips;
    pBrush->_cf._vstrips = _cachedSett._vstrips;
    pBrush->_cf._thick   = _cachedSett._thikness;
    pBrush->_cf._t       = E_STAIRS;
}




void CZ_ed2Doc::OnBusheet() 
{
   OnEscape() ;    //cancel any action because we've begin a new one
    if(_cachedSett._radstrips<0)
        _cachedSett._radstrips=1;
    if(_cachedSett._vstrips<0)
        _cachedSett._vstrips=1;
    
    _cachedSett._boxdim = GetViewsDims();
    _newBrshDlg._brshSett = _cachedSett;
    _newBrshDlg._hideZone=TRUE;
    if(IDCANCEL == _newBrshDlg.DoModal())
        return;
    _cachedSett = _newBrshDlg._brshSett;
    _selmode = SEL_BRUSH;

    if(_cachedSett._radstrips<1 || _cachedSett._vstrips < 1 || 
        _cachedSett._radstrips>32 ){
        AfxMessageBox("Sheet Vertical/Horizontal Strips are between 1 and 32",MB_OK|MB_ICONINFORMATION);
        return;
    }

    Brush* pBrush = new Brush();
    pBrush->MakeSheet(_cachedSett._boxdim, _cachedSett._vstrips, _cachedSett._radstrips);
	pBrush->_brushflags |= BRSH_DETAIL;
    pBrush->Rotate(V3(PIPE2,0,PI));//IsZero
    ApplyFlags(pBrush);
    pBrush->_cf._tm      = _cachedSett._tm;
    pBrush->_cf._brushflags   = pBrush->_brushflags;
    pBrush->_cf._hstrips = _cachedSett._radstrips;
    pBrush->_cf._vstrips = _cachedSett._vstrips;
    pBrush->_cf._thick   = _cachedSett._thikness;
    pBrush->_cf._t    = E_STAIRS;
    _pLastMakedBrush = pBrush;
}


void CZ_ed2Doc::OnInspref() 
{
    OnEscape();
    DlgNewPref  dn;

    if(dn.DoModal() == IDCANCEL)
        return;

    Brush* pBrush = dn.GetBrush();
	_cachedSett._splitter = 0;
    ApplyFlags(pBrush);

	pBrush->Reverse();


    pBrush->_cf._tm      = _cachedSett._tm;
    pBrush->_cf._brushflags   = pBrush->_brushflags;
    pBrush->_cf._hstrips = 0;
    pBrush->_cf._vstrips = 0;
    pBrush->_cf._thick   = _cachedSett._thikness;
    pBrush->_cf._t    = E_PREFAB;

	Invalidate();

    _brmode= BR_MOVE;
    _docDirty = 1;

}


void CZ_ed2Doc::OnUpdateInsertMesh(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(TRUE);
}

void CZ_ed2Doc::OnUpdateAddModel(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(1);
}


void CZ_ed2Doc::OnTerian() 
{
    OnEscape();
    // special dialog box for terian
    CDlgNewTerrain  ter;
    if(IDOK!=ter.DoModal())
    {
        return;
    }
    ASSERT(ter._pWorkingBrush);

    FOREACH(vvector<Poly>,ter._pWorkingBrush->_polys,pPoly)
	{
		Poly& poly = *pPoly;

        if(poly.GetHtex(GUtex)<= 0)
        {
    		poly.SetTex(BARDLG()->GetCurSelTexture(), GUtex, BARDLG()->GettexGenFlag());
        }

        FOREACH(vvector<Vtx2>,poly._vtci,pvtci)
            pvtci->_rgb = ZWHITE;////_scene._si.defLumin;

	}

    ter._pWorkingBrush->_group = _curCreatGroup;
    ter._pWorkingBrush->_brushflags |= BRSH_NEW;
    _maxUsedGrp  = tmax(_maxUsedGrp, _curCreatGroup);
    //ter._pWorkingBrush->Rotate(V3(0,0,PI));
    
    SelectBrush(ter._pWorkingBrush);

    z_ed3View*  pT3 = VIEW3D(0,0);
	pT3->SetFocus();

    _brmode= BR_MOVE;
    Invalidate(1);

    _docDirty = 1;

}

void CZ_ed2Doc::OnUpdateTerian(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(TRUE);

}

void CZ_ed2Doc::OnTerianbig() 
{
    OnEscape();
    CDlgBigTerrain  d;
    CBigTerrain&    terr = SCENE().Terrain();
    d._terrain = terr;
    if(d.DoModal()==IDOK)
    {
        
        terr = d._terrain;

        if(terr.p_dummyBrush)
        {
            SCENE().AddPrimitive(terr.p_dummyBrush);
            Invalidate();
            SelectBrush(terr.p_dummyBrush);            
            _brmode     = BR_SCALE;
            _selmode	= SEL_BRUSH;
        }

    }
}

void CZ_ed2Doc::OnUpdateTerianbig(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(TRUE);	
}

void CZ_ed2Doc::On2dshape() 
{
}

void CZ_ed2Doc::OnUpdate2dshape(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(0);
}


void     CZ_ed2Doc::InvokePlugIn(const char* dllname, int ncmd)
{
    char szFilters[MAX_PATH];// = _T(" Ms3dFile (*.ms3d)|*.ms3d|All Files (*.*)|*.*||");
    REDIR();

    SaveTempFile();
    _TRY
    {
        PlugInDll<IGeticPlug> plDll(MKSTR("plugins\\%s",dllname));	
        if(plDll.IsValid())
        {
            char   menuName[128];
        
            DWORD   typo;
            plDll->GetMenuStringAndType(menuName,&typo);
            if(_tcschr(menuName,_T(',')))
            {
                char*  pFName = _tcstok(menuName,",");   
                char*  pExt   = _tcstok(0,",");   ;   
            
                if((ncmd & 0x1)==0)
                {
                    if(!AKEY(VK_CONTROL))
                        AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_FILE_NEW,0);
                }

	            _stprintf(szFilters, _T("%s (*.%s)|*.%s|All Files (*.*)|*.*||"),pFName,pExt,pExt);
                CFileDialog dlg(((ncmd & 0x1)==0) ? TRUE : FALSE, MKSTR(".%s",pExt), 0, OFN_HIDEREADONLY, szFilters);
                dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();
		        if (theApp.DoFileDialog(dlg) != IDOK) 
		        {
			        REDIR();
                    return;
		        }
                PathHandler ph(dlg.m_ofn.lpstrFile);
                theApp.DocsDir(ph.Path());

                

                if(typo & PLUG_IMPORTER)
                    START_WAIT("Importing File");
                else
                    START_WAIT("Exporting File");

                AfxGetMainWnd()->Invalidate();
                AfxGetMainWnd()->UpdateWindow();
                _TRY
                {
				    if(typo & PLUG_IMPORTER && (ncmd & 0x1)==0)
				    {
					    ImportFile(plDll, dlg.m_ofn.lpstrFile);
				    }
				    else if ( (ncmd & 0x1) == 1)
					    ExportFile(plDll, dlg.m_ofn.lpstrFile);
                }
                _CATCHX()
                {
				    END_WAIT("");
				    AfxMessageBox(MKSTR("%s PlugIn Error: %d.",GetLastError(), dllname),MB_OK|MB_ICONWARNING);
                }
			    END_WAIT("");
                
                PathHandler ph2(dlg.m_ofn.lpstrFile); 
			    theApp.DocsDir(ph2.Path());
			    
                REDIR();

                _TRY{
                    theApp._gEditor.Clear();
                }_CATCHX()
                {
                    AfxMessageBox("Exception: Please don't delete texture buffers!",MB_OK|MB_ICONWARNING);
                }
            }
            else{
                AfxMessageBox(MKSTR("'%s' menu string of '%s' Is Not Well Formated ",menuName,dllname),MB_OK|MB_ICONWARNING);
            }
            plDll.Free();

            VIEW3D(0,0)->SetFocus();
            OnEnter();
            OnEscape();
        }
        else
        {
            AfxMessageBox(MKSTR("'%s' Plug-In Error. Cannot Find Exported Functions",dllname),MB_OK|MB_ICONWARNING);
        }
    }
    _CATCHX()
    {
        AfxMessageBox(MKSTR("'%s' Plug-In Error. ",dllname),MB_OK|MB_ICONWARNING);
    }
}

void      CZ_ed2Doc::ExportFile(PlugInDll<IGeticPlug>& plgDll, const char* pszName)
{
	Plg_Scene       scene;
    map<int,int>    texids;
    int             cntTex = 0;

    // store the whole stuff.
    PBrushes* pbRsh = _scene.GetPrimitives();
    scene.nBrushes  = pbRsh->size();
    scene.pBrushes  = new Plg_Brush[scene.nBrushes];
	
    scene.nTextures = Inl_GetTexCount();
    scene.pTextures = new Plg_Texture[scene.nTextures];

    // texture stuff
    TexSys::iterator bt = GTexSys.begin();
    TexSys::iterator et = GTexSys.end();
    
    for(;bt != et; bt++)
    {
        Texture& tex      = (*bt).second;
		if(!strncmp(tex.filename, "@L",2))
			continue;
        
        texids[(int)tex.hTex] = cntTex;         // map index with texid
        
        ::_tcscpy(scene.pTextures[cntTex].filename, tex.filename);
        scene.pTextures[cntTex].target = tex.hTex.glTarget;
        ++cntTex;
    }

    int brIdx = 0;
    FOREACH(PBrushes, (*pbRsh), ppbb)
    {
		Brush* pB = &(*ppbb)->GetResult();
		Brush* pB0 = *ppbb;

		scene.pBrushes[brIdx].pPolys = 0;
		scene.pBrushes[brIdx].nPolys = 0;
		
        //Brush* pB = (*ppbb);
        if(pB0->IsCut())
		{
			++brIdx;
			continue;
		}

		scene.pBrushes[brIdx].flags = pB0->_cf._brushflags;

		if(pB==pB0)
			scene.pBrushes[brIdx].flags |= BRSH_UNCUT;
        else
            scene.pBrushes[brIdx].flags &= ~BRSH_UNCUT;


        SCopy((char*)scene.pBrushes[brIdx].name, pB->_name, sizeof(scene.pBrushes[brIdx].name));
        scene.pBrushes[brIdx].nPolys = pB->_polys.size();
        scene.pBrushes[brIdx].pPolys = new Plg_Poly[scene.pBrushes[brIdx].nPolys];

        // store polygons
        int idxPoly = 0;
        FOREACH(vvector<Poly>,pB->_polys,pPoly)
	    {
		    Poly&       poly = *pPoly;
            Plg_Poly&   plPl = scene.pBrushes[brIdx].pPolys[idxPoly];

            plPl.flags     = poly._polyflags;
            plPl.combine   = poly.Combine();
            poly.BuildTexAxes(plPl.texax[0], plPl.texax[1]);
            plPl.texIdx[0] = texids[poly.GetHtex(0)];
            plPl.texScale  = UV(poly._texcoord.scalex, poly._texcoord.scaley);
            plPl.texShift  = UV(poly._texcoord.shiftx, poly._texcoord.shifty); 
            plPl.nvXes     = poly._vtci.size();
            plPl.vXes      = new Vtx[plPl.nvXes];
            
            int idxvx = 0;
            FOREACH(vvector<Vtx2>, poly._vtci, pvtci)
            {
                plPl.vXes[idxvx] = *pvtci;
                ++idxvx;
            }
            ++idxPoly;
	    }

        ++brIdx;
    }

    _TRY
    {
        plgDll->ExportFile(&theApp._gEditor, (char*)theApp.InstallDir(), (char*)pszName, &scene);

        if(scene.pTextures)
            delete[] scene.pTextures;

		for(int i=0; i< scene.nBrushes;i++)
		{
            Plg_Brush& b = scene.pBrushes[i];
			for(int j=0; j< b.nPolys;j++)
			{
                Plg_Poly& p = b.pPolys[j];
                delete[] p.vXes;
			}
			delete[]b.pPolys;
		}
        delete[] scene.pBrushes;
    }
    _CATCHX()
    {
        AfxMessageBox(MKSTR("Error:%d in '%s'",GetLastError(), plgDll),MB_OK|MB_ICONWARNING);
    }
    
}

static void __CalcTexCoord(Poly& p, V3 tax[2], UV& shift, UV& scale, SIZE& bmpSz, int layer)
{
    V3  texO = (shift.u * tax[0]) / tax[0].len();
        texO -= (shift.u * tax[1]) / tax[1].len();

    // fix passed in scalation
    if(scale.u == 0) scale.u = 1;
    if(scale.v == 0) scale.v = 1;

    V3  ss = tax[0] / (REAL)bmpSz.cx; // xbitmap
    V3  tt = tax[1] / (REAL)bmpSz.cy; //ybitmap
    REAL scaleS = ss.len();
    REAL scaleT = tt.len();
    ss /= scaleS;
    tt /= scaleT;
    
    for(UINT i=0; i < p._vtci.size() ;i++)
    {
        V3& v  = p._vtci[i]._xyz;
        UV& t  = p._vtci[i]._uv[layer];
        t.u = Vdp((p._vtci[i]._xyz - texO) , ss) * scaleS * scale.u;
        t.v = Vdp((p._vtci[i]._xyz - texO) , tt) * scaleT * scale.v;
    }
}


void      CZ_ed2Doc::ImportFile(PlugInDll<IGeticPlug>& plgDll, const char* pszdllname)
{

	Plg_Scene* pScene = 0;
	char	   texture[_MAX_PATH];
    int         k;

    CString resDir = theApp.HomeDir();
    resDir += "\\res\\";
    Box     wholething;

    InitDocVars();
    _cachedSett._splitter   = 0;
    _cachedSett._isSolid    = 1;
	_cachedSett._reversed   = 0;	
	_cachedSett._cutAll     = 0;
    _cachedSett._skyDom     = 0;
    _cachedSett._splitter   = 0;
    _cachedSett._zone       = 0;
    _cachedSett._detached   = 0;
    _cachedSett._zone       = 0;
    _cachedSett._detail     = 0;
    _cachedSett._reversed   = 0;


    if(0== plgDll->ImportFile(&theApp._gEditor, (char*)theApp.InstallDir(), (char*)pszdllname, &pScene) && pScene)
    {

		for(k=0;k<pScene->nBrushes;k++)
		{

			///KEEP_WAIT(MKSTR("Processing Brushes %d", k));

			Plg_Brush*	pBrush = &pScene->pBrushes[k];
			Brush*      pB     = new Brush();
			

			for(int i=0; i< pBrush->nPolys;i++)
			{
				Poly    p;            
				for(int j=0; j < pBrush->pPolys[i].nvXes; j++)
				{
					Vtx2   vtc; 
                    vtc._xyz    = pBrush->pPolys[i].vXes[j]._xyz;
                    vtc._nrm    = pBrush->pPolys[i].vXes[j]._nrm;
                    vtc._pPoly  = &p;
                    int maxtex  = 1;

                    wholething.AddPoint(vtc._xyz);

                    for(int ts = 0; ts < maxtex; ts++)
                    {
						if(pBrush->pPolys[i].vXes[j]._uv[ts].u != NAF)
						{
							vtc._uv[ts]  = pBrush->pPolys[i].vXes[j]._uv[ts];
						}
                    }
					p._vtci.push_back(vtc);
				}

				p._pBrush     = pB;
            	p._colorD     = CLR(pBrush->pPolys[i].color);
				p._polyflags  = 0;
				p._polyprops  = 0;


                int ito = 0;
                for(int t=0; t<4; t++)                    
                {
                    if(t==1)//loght map
                        continue;

                    if(pBrush->pPolys[i].texIdx[t]==-1)
                        continue;


                    DWORD tmpVal = pBrush->pPolys[i].texapply[t];

                    p.SetApply(tmpVal,ito);
                    p.SetCombine(pBrush->pPolys[i].combine);

                    if(pBrush->pPolys[i].texIdx[t] >= 0 && pScene->nTextures > pBrush->pPolys[i].texIdx[t])
                    {
						DWORD target = pScene->pTextures[pBrush->pPolys[i].texIdx[t]].target;
                        

                        char* pFileName = (char*)pScene->pTextures[pBrush->pPolys[i].texIdx[t]].filename;
                        if(pFileName[0] != 0)
                        {
                            SCopy(texture, pFileName, sizeof(texture));
                            
                        
                            CFileFind   finder;
                            BOOL        bWorking = 0;
                            

                            if(strchr(texture,'.'))
                                bWorking=finder.FindFile( pFileName);
                            else
                            {
                                if(bWorking=finder.FindFile( MKSTR("%s.jpg",pFileName)))
                                {
                                    SCopy(texture, (char*)MKSTR("%s.jpg",pFileName), sizeof(texture));
                                }
                                else if (bWorking=finder.FindFile( MKSTR("%s.tga",pFileName)))
                                {
                                    SCopy(texture, (char*)MKSTR("%s.tga",pFileName), sizeof(texture));
                                }
                                else if(bWorking=finder.FindFile( MKSTR("%s.bmp",pFileName)))
                                {
                                    SCopy(texture, (char*)MKSTR("%s.bmp",pFileName), sizeof(texture));
                                }
                            }
                        
                        
                            if(bWorking)
                            {
                                finder.Close();

                                PathHandler ph(texture);

                                CopyFile(texture, MKSTR("%s%s",resDir, ph.File()), 0);
                                p.SetTex(ph.File(), ito);
                            }
                            
                        }
                    }
                    ito++;    
                }//4 tex stages

				pB->_polys.push_back(p);
			}

            // for the commit
            _cachedSett._zone = 0;
			if(pBrush->flags & BRSH_SOLID)
			{
				_cachedSett._isSolid = 1;
				_cachedSett._cutAll  = 0;
			}
			else
			{
				_cachedSett._isSolid = 0;
				_cachedSett._cutAll  = 1;
			}
			_cachedSett._splitter=0;
			_cachedSett._skyDom=0;

			CommitBrush(pB);
			SCopy(pB->_name, pBrush->name, sizeof(pB->_name));
			pB->_group = 0;
		}

        for(k=0;k<pScene->nItems;k++)
        {
            Plg_Item*	pItem = &pScene->pItems[k];

            CLightBulb* pCurItem = new CLightBulb();
            

            pCurItem->_t           = pItem->position;
            pCurItem->_colorD.r    = pItem->_U._Light.color[0];
            pCurItem->_colorD.g    = pItem->_U._Light.color[1];
            pCurItem->_colorD.b    = pItem->_U._Light.color[2];
            pCurItem->_radius      = pItem->_U._Light.radius;
            pCurItem->_lmIntensity =  pItem->_U._Light.intensity;

          	pCurItem->_flags|= BRSH_NEW;
            pCurItem->_group = _curCreatGroup;

            DOC()->AddItem(pCurItem);
            DOC()->OnEnter();

        }

        //plgDll->ReleaseScene(pScene);
    }

    wholething.Expand(256);
    

    Brush*    pBrush   = new Brush();
	pBrush->MakeCube(wholething.GetExtends(), _cachedSett._tm);
    pBrush->Move(wholething.GetCenter());
    pBrush->_brushflags = BRSH_SOLID|BRSH_NEW;
    pBrush->ResetTexCoords();	
 	pBrush->Recalc();   
    pBrush->_cf._tm             = _cachedSett._tm;
    pBrush->_cf._brushflags     = BRSH_SOLID;
    pBrush->_cf._hstrips        = 0;
    pBrush->_cf._vstrips        = 0;
    pBrush->_cf._thick          = 0;
    pBrush->_cf._t              = E_CUBE;

    _pSelBrushes.push_back(pBrush); //place in sel brushes witht eh new flag set

    _selmode = SEL_NONE;


    _docDirty = 0;
}


void CZ_ed2Doc::OnUpdateParskin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(1);
}

void CZ_ed2Doc::OnParskin() 
{
    if(IDOK == DlgEditor2D().DoModal())
    {
        VIEW3D(0,0)->SetFocus();
        Invalidate();
    }
}


void CZ_ed2Doc::OnInsgameitem() 
{
    SaveTempFile();

    OnEscape();
	DlgClassItem	dni ; 
	if(dni.DoModal() == IDOK)
	{
		////BARDLG()->ShowItemDlg(dni._pCurItem);
		VIEW3D(0,0)->SetFocus();
        Invalidate();
	}
	
}

void CZ_ed2Doc::OnUpdateInsgameitem(CCmdUI* pCmdUI) 
{
    if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(TRUE);
	
	
}

// spline mode
void CZ_ed2Doc::OnSpline() 
{
    	
}

void CZ_ed2Doc::OnUpdateSpline(CCmdUI* pCmdUI) 
{
	
}

void CZ_ed2Doc::OnClosedspline() 
{
	
}

void CZ_ed2Doc::OnUpdateClosedspline(CCmdUI* pCmdUI) 
{
	
}

void CZ_ed2Doc::OnProjBrush() 
{
    OnEscape();
    
    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);
	z_ed3View*  pT3 = VIEW3D(0,0);

	Brush*    pBrush   = new Brush();

    REAL xd = (pTX->_vport.right - pTX->_vport.left) / 2.0;

	pBrush->MakeCube(V3(xd,xd,xd), TM_LIN);
    pBrush->Move(V3(pTY->_shiftx, pTZ->_shifty, -pTY->_shifty));
	pBrush->Recalc();
	pBrush->_group = _curCreatGroup;
    pBrush->_brushflags = (BRSH_NEW|BRSH_PROJECTOR);

	FOREACH(vvector<Poly>,pBrush->_polys,pPoly)
	{
		Poly& poly = *pPoly;

        if(poly.GetHtex(GUtex) <= 0)
        {
    		poly.SetTex(BARDLG()->GetCurSelTexture(),GUtex, BARDLG()->GettexGenFlag()) ;
        }
        FOREACH(vvector<Vtx2>,poly._vtci,pvtci)
            pvtci->_rgb = ZWHITE;//_scene._si.defLumin;

	}

    pBrush->Reverse();
    pBrush->ResetTexCoords();	//FOR LIGHT MAP
 	_pSelBrushes.push_back(pBrush); //place in sel brushes with the new flag set
	Invalidate();
	pT3->SetFocus();

	_selmode		= SEL_BRUSH;
	_bShowCsg		= 0;
    _compviewmode   = C_NA;
    _brmode         = BR_SCALE;

    _maxUsedGrp     = tmax(_maxUsedGrp, _curCreatGroup);

    pBrush->_cf._tm      = TM_LIN;
    pBrush->_cf._brushflags   = BRSH_PROJECTOR;
    pBrush->_cf._hstrips = 0;
    pBrush->_cf._vstrips = 0;
    pBrush->_cf._thick   = 0;
    pBrush->_cf._t       = E_CUBE;


}

void CZ_ed2Doc::OnUpdateProjBrush(CCmdUI* pCmdUI) 
{
     if(_bShowCsg || _compiling || _compviewmode  != C_NA)
    {
        pCmdUI->Enable(FALSE);
        return;
    }
	pCmdUI->Enable(TRUE);
}

void CZ_ed2Doc::CommitBrush(Brush* pBrush)
{
    if(_cachedSett._splitter)
    {
        pBrush->_brushflags = BRSH_SPLITTER;
    }
    else if(_cachedSett._zone)
    {
        pBrush->_brushflags = BRSH_ZONE;
    }
    else
    {
        if(_cachedSett._isSolid) // cut
        {
		    pBrush->_brushflags |= BRSH_SOLID;
		    if(_cachedSett._detail)
		    {
			    pBrush->_brushflags |= BRSH_DETAIL;
		    }
        }
        else
	    {
		    pBrush->FlagFaces(POLY_FROM_CUTB);
		    if(_cachedSett._cutAll)
		    {
			    pBrush->_brushflags |= BRSH_CUTALL;
		    }
		    pBrush->_brushflags &= ~BRSH_DETAIL;
	    }
    }
	if(_cachedSett._reversed)	
	{
        pBrush->Reverse();
        if(_cachedSett._skyDom)
        {
            pBrush->_brushprops |= MODEL_IS_SKYDOM;
        }
	}
	pBrush->_group = _curCreatGroup;
    pBrush->_brushflags |= BRSH_NEW;
	pBrush->Reverse();
    pBrush->ResetTexCoords();	//FOR LIGHT MAP
 	pBrush->Recalc();   
    _pSelBrushes.push_back(pBrush); //place in sel brushes witht eh new flag set
    _selmode		= SEL_BRUSH;
	_bShowCsg		= 0;
    _compviewmode   = C_NA;
    _maxUsedGrp     = tmax(_maxUsedGrp, _curCreatGroup);
}


void CZ_ed2Doc::OnInsitem() 
{
    OnEscape();
	DlgNewItem	dni ; 
	if(dni.DoModal() == IDOK)
	{
		BARDLG()->ShowItemDlg(dni._pCurItem);
		Invalidate();
        
		BARDLG()->Dirty();
        VIEW3D(0,0)->SetFocus();
        _selmode = SEL_ITEMS;
        _brmode = BR_SCALE;
        _docDirty = 1;

	}
}