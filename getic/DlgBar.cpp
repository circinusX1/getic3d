//---------------------------------------------------------------------------------------
// DlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "mainFrm.h"
#include "DlgBar.h"
#include "z_ed2Doc.h"

//---------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgBar*    CDlgBar::GBarDlg;

IMPLEMENT_DYNCREATE(CDlgBar, BASE_CLS)

typedef LRESULT (CDlgBar::*PFNDB)(WPARAM,LPARAM);
//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDlgBar, BASE_CLS)
//{{AFX_MSG_MAP(CDlgBar)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON3, OnApply)
	ON_WM_DESTROY()
//}}AFX_MSG_MAP
//    ON_MESSAGE(WM_INITDIALOG, HandleInitDialog2)
    ON_MESSAGE(WM_KEYTAB, OnKeyTab)
	ON_MESSAGE( WM_REDRAWCHILDREN, RedrawCHildren)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
//    ON_WM_VSCROLL()
    ON_MESSAGE(WM_POSTINIT,(PFNDB)OnPostInit)	
END_MESSAGE_MAP()


HBRUSH CDlgBar::OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor)
{
    pDC->SetBkColor(theApp._color[1]);
	return theApp._brush[1];
}

//---------------------------------------------------------------------------------------
// CDlgBar dialog
CDlgBar::CDlgBar(CWnd* pParent /*=NULL*/): BASE_CLS(IDR_MAINFRAME),_tabSelIdx(0),_dirty(0),_blkRetreive(0) 
{
	//{{AFX_DATA_INIT(CDlgBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    CDlgBar::GBarDlg = this;
    _pItemBase      = 0;
    _pItemBasePrev  = 0;
    _pItemBaseB     = 0;
}

//---------------------------------------------------------------------------------------
void CDlgBar::ActivateTexturesDlg(BOOL bShow)
{

}

void CDlgBar::CommonInitializer()
{
	static int B = FALSE;

	if(B==TRUE)
		return ;

    _initDlg=TRUE;
    _tb.SubclassDlgItem(TAB_DLGS, this);
    _dlgtex.Create( DLG_TEX, this);
    //_dlgSnd.Create( DLG_SOUNDS, this);
    _dlgItem.Create( DLG_ITEMS, this);
    //_dlgScript.Create( DLG_SCRIPT, this);
    _dlgScene.Create( DLG_SCENE, this);

    _dlgFp.Create(DLG_F_SET,     this);
    _dlgBp.Create(DLG_B_SET,     this);


    // in items tab
    _applyBut.SubclassDlgItem(IDC_BUTTON3, this);    

    _tb._pParent = this;
	_tb.AddDialog(&_dlgtex,"Textures",0, IDC_STATICF);
    _tb.AddDialog(&_dlgBp,"Brush",0, IDC_STATICF);
    _tb.AddDialog(&_dlgFp,"Surface",0, IDC_STATICF);
    //_tb.AddDialog(&_dlgSnd, "Sounds", 0,IDC_STATICF);
    _tb.AddDialog(&_dlgItem, "Items", 0,IDC_STATICF);
    //_tb.AddDialog(&_dlgScript, "Scripts", 0,IDC_STATICF);
    _tb.AddDialog(&_dlgScene, "Scene", 0,IDC_STATICF);
    long l;
    _tb.OnSelchangeDlgs(0,&l);
    _tabSelIdx = 0;
    _dirty = 0;
    _initDlg=0;


    RECT rt;
	GetDlgItem(IDC_STATICF2)->GetWindowRect(&rt);
	ScreenToClient(&rt);

	_dlgLightItem.Create( DLG_I_LIGHT, this);
    _dlgLightItem.SetItmType(ITM_LIGTBULB);

    _dlgSoundItem.Create( DLG_I_SOUND, this);
    _dlgSoundItem.SetItmType(ITM_SNDSOURCE);
    

    _dlgTrigger.Create( DLG_I_TRIGER, this); ////<NEW_ITEM>
    _dlgTrigger.SetItmType( ITM_TRIGER);

    _dlgGameItem.Create(DLG_I_GAMEITM, this);
    _dlgGameItem.SetItmType(ITM_GAME);

    _dlgStartPlayItem.Create(DLG_I_STARTPLAY, this);
    _dlgStartPlayItem.SetItmType(ITM_STARTPLAY);


    
    _dlgSoundItem.MoveWindow(rt.left,rt.top,
                      rt.right-rt.left, 
		        	  rt.bottom-rt.top,0);

	_dlgLightItem.MoveWindow(rt.left,rt.top,
                      rt.right-rt.left, 
		        	  rt.bottom-rt.top,0);
    
	_dlgTrigger.MoveWindow(rt.left,rt.top,
                      rt.right-rt.left, 
		        	  rt.bottom-rt.top,0);

    _dlgStartPlayItem.MoveWindow(rt.left,rt.top,
                      rt.right-rt.left, 
		        	  rt.bottom-rt.top,0);

    _dlgGameItem.MoveWindow(rt.left,rt.top,
                            rt.right-rt.left, 
		        	    	rt.bottom-rt.top,0);

	B=TRUE;

}

//---------------------------------------------------------------------------------------
int CDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (BASE_CLS::OnCreate(lpCreateStruct) == -1)
		return -1;
    //_dirtyBrush.CreateSolidBrush(GetSysColor(COLOR_ACTIVECAPTION));

	return 0;
}

void CDlgBar::OnInitialUpdate() 
{
#ifdef _BAR_DLGPANE
	BASE_CLS::OnInitialUpdate();
#endif //#ifdef _BAR_DLGPANE
    CommonInitializer();
}


BOOL CDlgBar::OnInitDialog()
{
#ifndef _BAR_DLGPANE
    CBASEDLG::OnInitDialog();
#endif //
    CommonInitializer();
    return 1;
}

//---------------------------------------------------------------------------------------
LRESULT CDlgBar::HandleInitDialog2(WPARAM w, LPARAM l)
{
    LRESULT lr =  HandleInitDialog(w,l);

    CommonInitializer();
    return lr;
}

//---------------------------------------------------------------------------------------
void CDlgBar::OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler )
{
    //enables the controls
}

//---------------------------------------------------------------------------------------
void    CDlgBar::ShowItemProps(size_t itmtype)
{
    RECT rt;
	GetDlgItem(IDC_STATICF2)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if(_pItemBase && itmtype!=_pItemBase->ItemType())
	{
		_pItemBase->Show(0);
	}
    _pItemBase = 0;
	_dlgItem._pCurrent = 0;

    switch(itmtype)
    {
        case ITM_LIGTBULB:
            _pItemBase = &_dlgLightItem;
            _pItemBaseB = &_dlgLightItem;
			_dlgItem._pCurrent =&_dlgLightItem;
            break;
        case ITM_TRIGER:
            _pItemBase = &_dlgTrigger;
            _pItemBaseB = &_dlgTrigger;
			_dlgItem._pCurrent =&_dlgTrigger;
            break;
        case ITM_STARTPLAY:
            _pItemBase = &_dlgStartPlayItem;
            _pItemBaseB = &_dlgStartPlayItem;
			_dlgItem._pCurrent =&_dlgStartPlayItem;
            break;

        case ITM_GAME:
            _pItemBase = &_dlgGameItem;
            _pItemBaseB = &_dlgGameItem;
			_dlgItem._pCurrent =&_dlgGameItem;
			break;
        case ITM_SNDSOURCE:
            _pItemBase = &_dlgSoundItem;
            _pItemBaseB = &_dlgSoundItem;
			_dlgItem._pCurrent =&_dlgSoundItem;
			break;

    }
		
}

//---------------------------------------------------------------------------------------
void    CDlgBar::ShowSelItmDlg(int sel)
{
}


//---------------------------------------------------------------------------------------
void    CDlgBar::OnBrushesSelected()
{
    if(!::IsWindow(m_hWnd))
        return;
	BlockRetreive(1);
    int nCount;
    Brush** ppBrushes = DOC()->GetSelBrushes(nCount);


    if(nCount  )
	{
		ShowBrushesProperties(ppBrushes, nCount);
    }
    else
    {
        _dlgBp.Update(0);        
    }
	BlockRetreive(0);
}


void    CDlgBar::OnItemsSelected()
{
	OnItemSelected(0,0);
}

//---------------------------------------------------------------------------------------
void    CDlgBar::OnFacesSelected()
{
    if(!::IsWindow(m_hWnd))
        return;
    BlockRetreive(1);
    int nCount;
    Poly** ppPolys = DOC()->GetSelPolys(nCount);

    if(nCount )
	{
		ShowFacesProperties(ppPolys, nCount);
    }
    else
    {
        UV uv;CLR clr;
        _dlgFp.Update(0);
    }
	BlockRetreive(0);
}

//---------------------------------------------------------------------------------------
void CDlgBar::OnApply() 
{
    if(FALSE == _dirty || _blkRetreive || !IS_WINDOW(m_hWnd))
    {
        return;
    }

    BOOL            invalCustDlg = 0;
	BOOL			bDesel = FALSE;
    map<Brush*,int>	ub;
	int				nCount;

	_dirty = FALSE;
	// parse all dialgs		
    if(_dlgFp.IsWindowVisible())
    {
         int	nCount;
         Poly** pSelPoly = DOC()->GetSelPolys(nCount);

		if(0==nCount)
		{
			Beep(600,30);
			GetDlgItem(ST_MSG)->SetWindowText("No Selection !");
            SBT(0,"Invalid Item/Brush Selection");
			return;
		}


         for(int i=0; i < nCount; i++)
         {
            _dlgFp.Retreive((*pSelPoly));
            ub[(*pSelPoly)->_pBrush]=1; 
            pSelPoly++;
         }
		_dlgFp._dirty = FALSE;

		 bDesel = TRUE;  // deselect what we select here because are faces
         _dlgFp.ClearDirtyIDs();
    }
    
    else if(_dlgBp.IsWindowVisible())
    {
        Brush** pBrsh = DOC()->GetSelBrushes(nCount);
		if(0==nCount)
		{
			Beep(600,30);
			GetDlgItem(ST_MSG)->SetWindowText("No Selection !");
            SBT(0,"Invalid Item/Brush Selection");
			return;
		}

         for(int i=0; i < nCount; i++)
         {
             _dlgBp.Retreive(*pBrsh);
             ub[*pBrsh]=1;
             pBrsh++;
         }
		 _dlgBp._dirty = FALSE;
         _dlgBp.ClearDirtyIDs();

        if(ub.size())
	    {
		    map<Brush*,int>::iterator b = ub.begin();
		    map<Brush*,int>::iterator e = ub.end();
		    for(;b!=e;b++)
		    {
			    DOC()->SelectBrush((*b).first);
		    }
            DOC()->Recut();

		    if(bDesel)
		    {
			    b = ub.begin();
			    for(;b!=e;b++)
			    {
				    DOC()->DeSelectBrush((*b).first);
			    }
		    }
	    }

    }
    else   if(_dlgItem.IsWindowVisible())
    {
        int nCount;
        SceItem** pSi = DOC()->GetSelItems(nCount);


		if(0==nCount)
		{
			Beep(600,30);
			GetDlgItem(ST_MSG)->SetWindowText("No Selection !");
            SBT(0,"Invalid Item/Brush Selection");
			return;
		}


        for(int i=0; i < nCount; i++)
        {
            _dlgItem.Retreive(*pSi, nCount);
            if(_pItemBase)
            {
				_pItemBase->Retreive((*pSi));
            }
            pSi++;
        }
		_dlgItem._dirty = FALSE;
        if(_pItemBase)
        {
		    _pItemBase->_dirty=FALSE;
        }
        if(_pItemBaseB)
            _pItemBaseB->ClearDirtyIDs();

        _dlgItem.ClearDirtyIDs();
        if(BARDLG()->_dlgGameItem.IsDlgVisible())
        {
            ::DelayUpdateWindow(_dlgGameItem.m_hWnd);
        }
    }
    else   if(_dlgScene.IsWindowVisible())
    {
        _dlgScene.Retreive();
        ::DelayUpdateWindow(_dlgScene.m_hWnd);
    }

	GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
	DOC()->Invalidate();
    Invalidate();
    GetDlgItem(ST_MSG)->SetWindowText("");
}

//---------------------------------------------------------------------------------------
// restore original state of brush
void CDlgBar::ResetBrushes()
{
    int nCount;
    Brush** pBrsh = DOC()->GetSelBrushes(nCount);
    for(int i=0; i< nCount; i++)
    {
        (*pBrsh)->Reinit();
        pBrsh++;
    }
    DOC()->Recut();
}

//---------------------------------------------------------------------------------------
void CDlgBar::ShowBrushesProperties(Brush** ppBrushes, int nCount)
{
	BlockRetreive(1);
    _dlgtex.EnableTexControls(FALSE);

    /**
    if(_dlgBp.IsWindowVisible())
    */
    if(nCount)
        _dlgBp.Update(ppBrushes[nCount-1]);

	BlockRetreive(0);
}

//---------------------------------------------------------------------------------------
void CDlgBar::ShowFacesProperties(Poly** ppPolys, int nCount)
{
	BlockRetreive(1);
    // on poly select the texture for it, otherwise ignore it
    if(nCount > 0)
    {
        Poly* pPoly = ppPolys[nCount-1];

        _dlgtex.EnableTexControls(TRUE);
        _dlgtex.SetTexCoords(pPoly->_texcoord);

        const char* textutrName = GTexSys.GetTexName(pPoly->GetHtex(GUtex));
        if(textutrName)
        {
            char loco[PATH_MAX];
            strcpy(loco, textutrName);
            char* pd =strchr(loco,'?');
            if(pd)*pd=0;
            _dlgtex.SelectFaceTexture(loco);
        }

        //if(_dlgFp.IsWindowVisible())
        {
            _dlgFp.Update(pPoly);
        }
    }
	BlockRetreive(0);
}

size_t   CDlgBar::GettexGenFlag()
{
	if(::IsWindow(_dlgtex.m_hWnd) )
	{
		return _dlgtex.GettexGenFlag();
	}
	return 0;
}

//---------------------------------------------------------------------------------------
char*   CDlgBar::GetCurSelTexture()
{
	static char Sttt[64] = "EF-TEX-RGBY";
	if(::IsWindow(_dlgtex.m_hWnd) )
	{
		_dlgtex.GetCurSel(Sttt);

        if(Sttt[0]==0)
            strcpy(Sttt, "EF-TEX-RGBY");
		return Sttt;
	}
	return "*";
}

//---------------------------------------------------------------------------------------
BOOL CDlgBar::OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if(::IsWindow(_tb.m_hWnd))
        _tb.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	return BASE_CLS::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void    CDlgBar::AddItem(SceItem* pItem)
{
    BlockRetreive(1);
    if(_dlgItem && ::IsWindow(_dlgItem.m_hWnd))
    {
        _dlgItem.AddItem(pItem);
    }
    BlockRetreive(0);
}

//---------------------------------------------------------------------------------------
void    CDlgBar::DelItem(SceItem* pItem)
{
    BlockRetreive(1);
    if(_dlgItem && ::IsWindow(_dlgItem.m_hWnd))
    {
        _dlgItem.DelItem(pItem);
    }
    BlockRetreive(0);
}

//---------------------------------------------------------------------------------------
void    CDlgBar::OnItemPropsChanged(SceItem* pItem)
{
    BlockRetreive(1);
    if(_dlgItem && ::IsWindow(_dlgItem.m_hWnd))
    {
        _dlgItem.OnItemPropsChanged(pItem);
    }
    BlockRetreive(0);
}

//---------------------------------------------------------------------------------------
void    CDlgBar::ShowItemDlg(SceItem* pItem)
{
    if(_dlgItem && ::IsWindow(_dlgItem.m_hWnd))
    {
        if(pItem)
		{
            _dlgItem.OnItemSelected(pItem, 1);
        }
    }

}

void    CDlgBar::DeselectItems(BOOL notyFrame)
{
    
    if(!_dlgItem || !::IsWindow(_dlgItem.m_hWnd))
		return;
   
    BlockRetreive(1);
    
    int     cbCnt  = _dlgItem._lbItems.GetCount();

    for(int  i=0; i < cbCnt; i++)
    {
       _dlgItem._lbItems.SetSel(i,0);
    }
    if(_dlgItem.IsWindowVisible())
        _dlgItem._lbItems.Invalidate();
    BlockRetreive(0);

}

//---------------------------------------------------------------------------------------
void    CDlgBar::OnItemSelected(SceItem* pItem, int sel)
{
    if(!_dlgItem || !::IsWindow(_dlgItem.m_hWnd))
		return;
   
    BlockRetreive(1);
	if(pItem)
	{
        int still = _dlgItem.OnItemSelected(pItem, sel);
        /*if(sel)
        {
		    ShowItemProps(pItem->_item);
        }*/
		if(_dlgItem.IsWindowVisible() && _dlgItem._pCurrent)
		{
            if(_dlgItem.IsNotSameTypeItemsSelected())
            {
                still=0;
            }
            _pItemBase->Show(still!=0);//hide if 2 different items
		}
	}
	else
	{
        _dlgItem.OnItemSelected(0, 0);
        ShowItemProps(-1);
		if(_dlgItem.IsWindowVisible() && _dlgItem._pCurrent && pItem && _pItemBase)
		{
             if(pItem->_item!=_pItemBase->ItemType())
             {
			    _pItemBase->Show(0);
             }
		}
		_pItemBase = 0;
        _pItemBaseB= 0;
		_dlgItem._pCurrent = 0;
	}
	BlockRetreive(0);
}

//---------------------------------------------------------------------------------------
void    CDlgBar::UpdateItmProps(SceItem* pItem)
{
    if(0==pItem)
        return;
    HWND    h2Update = 0;
	BlockRetreive(1);
	ShowItemProps(pItem->_item);
	_dlgItem.Update(pItem);

    switch(pItem->_item)
    {
        case ITM_LIGTBULB:
			 _dlgLightItem.Update(pItem);
             h2Update = _dlgLightItem.m_hWnd;
            break;
        case ITM_TRIGER:
            _dlgTrigger.Update(pItem);////<NEW_ITEM>
            h2Update = _dlgTrigger.m_hWnd;
            break;
       case ITM_STARTPLAY:
            _dlgStartPlayItem.Update(pItem);////<NEW_ITEM>
            h2Update = _dlgStartPlayItem.m_hWnd;
            break;
         case ITM_GAME:
            _dlgGameItem.Update(pItem);
            h2Update = _dlgGameItem.m_hWnd;
            ::DelayUpdateWindow(_dlgGameItem.m_lb.m_hWnd);
            break;
        case ITM_SNDSOURCE:
             _dlgSoundItem.Update(pItem);
             h2Update = _dlgSoundItem.m_hWnd;
			break;

    }
   //// DelayUpdateWindow(h2Update);
	BlockRetreive(0);
}


void    CDlgBar::RefreshItemsList()
{
	if(_dlgItem)
		_dlgItem.RefreshItemsList();
	Dirty();
	_dlgScene.Dirty();
	_dlgScene.Update();
}


void CDlgBar::OnDestroy() 
{
	BASE_CLS::OnDestroy();
//    _dirtyBrush.DeleteObject();	
}

void    CDlgBar::MakeVisible(CBASEDLG* pd)
{
}


void    CDlgBar::DisableDlg(CBASEDLG* pd)
{
}

void    CDlgBar::EnableDlg(CBASEDLG* pd)
{
}


void    CDlgBar::HideAll()
{
}


LRESULT CDlgBar::OnKeyTab(WPARAM w,LPARAM l)
{
    return 0L;
}

//-----------------------------------------------------------------------------
void    CDlgBar::UpdateMotionProps(Brush* pBrush, CMotion* pM)
{
    if(::IsWindow(_dlgBp.m_hWnd) /*&& _dlgBp.IsWindowVisible()*/)
    {
        _dlgBp.UpdateMotionProps(pBrush, pM);
    }
}

//-----------------------------------------------------------------------------
void CDlgBar::OnSize(size_t nType, int cx, int cy) 
{
	BASE_CLS::OnSize(nType, cx, cy);
}

//-----------------------------------------------------------------------------
void    CDlgBar::Rescan()
{
    OnBrushesSelected();
    OnFacesSelected();
    OnItemsSelected();
}

//-----------------------------------------------------------------------------
void CDlgBar::Refresh()
{
    OnBrushesSelected();
    OnFacesSelected();
    OnItemsSelected();
    RefreshItemsList();
    if(::IsWindow(_dlgScene.m_hWnd))
        _dlgScene.SendMessage(WM_COMMAND, PB_REFRESH);
}

//-----------------------------------------------------------------------------
// clen the content of bardlg here //CMenu
void CDlgBar::ResetDlgsContent()
{
    if(::IsWindow(_dlgItem.m_hWnd))
    _dlgItem.ClenContent();
}

//-----------------------------------------------------------------------------
void CDlgBar::OnVScroll(size_t nSBCode, size_t nPos, CScrollBar* pScrollBar) 
{
 	//CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CDlgBar::RedrawCHildren(WPARAM,LPARAM)
{
	Invalidate();
	_dlgItem.Invalidate();
	return 0;
}

LRESULT CDlgBar::OnPostInit(LPARAM, WPARAM) 
{
    /*
    _dlgItem.SendMessage(WM_POSTINIT)   ;
    _dlgScene.SendMessage(WM_POSTINIT)     ;
    _dlgLightItem.SendMessage(WM_POSTINIT)   ; 
    _dlgTrigger.SendMessage(WM_POSTINIT)     ; 
    _dlgStartPlayItem.SendMessage(WM_POSTINIT)     ; 
    _dlgFp.SendMessage(WM_POSTINIT)   ;
    _dlgBp.SendMessage(WM_POSTINIT)   ;
    _dlgGameItem.SendMessage(WM_POSTINIT)  ; 
*/
    return 0;
}

void    CDlgBar::OnCancel()
{
    _dlgScene.ClearDirtyIDs();
    _dlgLightItem.ClearDirtyIDs(); 
    _dlgSoundItem.ClearDirtyIDs(); 
    _dlgTrigger.ClearDirtyIDs();
    _dlgStartPlayItem.ClearDirtyIDs();
    _dlgFp.ClearDirtyIDs();
    _dlgBp.ClearDirtyIDs();
    _dlgGameItem.ClearDirtyIDs();

}



