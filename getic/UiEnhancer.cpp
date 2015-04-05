// UiEnhancer.cpp: implementation of the UiEnhancer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "UiEnhancer.h"
#include "Dlgbar.h"


BEGIN_MESSAGE_MAP(C_Rebar, CReBar)
	//{{AFX_MSG_MAP(CReBar)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReBar message handlers
BOOL C_Rebar::OnEraseBkgnd(CDC* pDC) 
{
    RECT rt; GetClientRect(&rt);
    pDC->FillRect(&rt,&theApp._brush[1]);
    return 1;
}

BEGIN_MESSAGE_MAP(C_ToolBar, CToolBar)
	//{{AFX_MSG_MAP(C_ToolBar)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL C_ToolBar::OnEraseBkgnd(CDC* pDC) 
{
    RECT rt; GetClientRect(&rt);
    pDC->FillRect(&rt,&theApp._brush[1]);
    return 1;
}


BEGIN_MESSAGE_MAP(C_HeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(C_HeaderCtrl)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void C_HeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	// TODO: Add your message handler code here and/or call default
	
    ASSERT(lpDIS->CtlType == ODT_HEADER);

    HDITEM hdi;
    char  lpBuffer[256];

    hdi.mask = HDI_TEXT;
    hdi.pszText = lpBuffer;
    hdi.cchTextMax = 256;

    GetItem(lpDIS->itemID, &hdi);

   // Draw the button frame.
   //::DrawFrameControl(lpDIS->hDC, 
   //   &lpDIS->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH);
    CDC* pdc = CDC::FromHandle(lpDIS->hDC);

   // Draw the items text using the text color red.
   COLORREF oTxtClr = pdc->SetTextColor(theApp._txtcolor);
   COLORREF oBkColor = pdc->SetBkColor(theApp._color[1]);

   pdc->DrawText(lpBuffer, strlen(lpBuffer), &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

   pdc->SetTextColor(oTxtClr);
   pdc->SetBkColor(oBkColor);
	
}

BOOL C_HeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
    RECT rt; GetClientRect(&rt);
    pDC->FillRect(&rt,&theApp._brush[1]);
    return 1;
}


BEGIN_MESSAGE_MAP(C_TabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(C_TabCtrl)
	ON_WM_ERASEBKGND()
//	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void C_TabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();
    ModifyStyle(0,TCS_OWNERDRAWFIXED);
}

void C_TabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{

    TCITEM hdi;
    char  lpBuffer[256];

    hdi.mask = TCIF_TEXT;
    hdi.pszText = lpBuffer;
    hdi.cchTextMax = 256;

    GetItem(lpDIS->itemID, &hdi);

    CDC* pdc = CDC::FromHandle(lpDIS->hDC);

   // Draw the items text using the text color red.
   COLORREF oTxtClr = pdc->SetTextColor(theApp._txtcolor);
   COLORREF oBkColor = pdc->SetBkColor(theApp._color[1]);
   pdc->DrawText(lpBuffer, strlen(lpBuffer), &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   pdc->SetTextColor(oTxtClr);
   pdc->SetBkColor(oBkColor);
	
}

BOOL C_TabCtrl::OnEraseBkgnd(CDC* pDC) 
{
    RECT rt; GetClientRect(&rt);
    pDC->FillRect(&rt,&theApp._brush[1]);
	return 1;
}


void C_Menu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
   CDC* pDC = CDC::FromHandle(lpDIS->hDC);
   COLORREF cr = (COLORREF)theApp._color[1]; // RGB in item data

   if (lpDIS->itemAction & ODA_DRAWENTIRE)
   {
      // Paint the color item in the color requested
      CBrush br(cr);
      pDC->FillRect(&lpDIS->rcItem, &br);
   }

   if ((lpDIS->itemState & ODS_SELECTED) &&
      (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
   {
      // item has been selected - hilite frame
      COLORREF crHilite = theApp._color[2];
      CBrush br(crHilite);
      pDC->FrameRect(&lpDIS->rcItem, &br);
   }

   if (!(lpDIS->itemState & ODS_SELECTED) &&
      (lpDIS->itemAction & ODA_SELECT))
   {
      // Item has been de-selected -- remove frame
      CBrush br(cr);
      pDC->FrameRect(&lpDIS->rcItem, &br);
   }

}




void CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseDlg, CDialog)
	//{{AFX_MSG_MAP(CBaseDlg)
	ON_WM_CTLCOLOR()
    ON_WM_SHOWWINDOW()
	//ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_REDRAW_DLG, OnRedraw)
    ON_MESSAGE(WM_POST_WM_INITDIALOG, OnPostInitDialog)
END_MESSAGE_MAP()

BOOL CBaseDlg::OnInitDialog() 
{
    _updatingCtrls = 1;
	CDialog::OnInitDialog();
/*
    CWnd*   pWnd     = this;
    int     relation = GW_CHILD|GW_HWNDFIRST;
    char   cn[128];
    UINT    newId    = 42000;
    while(pWnd = pWnd->GetWindow(relation))
    {
        ::GetClassName(pWnd->m_hWnd,cn,128);
        if(!_tcsicmp(cn,"STATIC"))
        {
            SetDlgCtrlID(newId++);
            _allstats.SubclassDlgItem(GetDlgCtrlID(),this);
        }
     
        relation = GW_HWNDNEXT;
    }
*/
    PostMessage(WM_POST_WM_INITDIALOG);
    return 0;
}

BOOL    CBaseDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if(nCode == EN_KILLFOCUS || 
        CN_COMMAND == nCode || 
        EN_CHANGE == nCode || 
        CBN_SELCHANGE== nCode ||
        BN_CLICKED== nCode ||
        nCode==4 || 
        nCode==9 )
	{
        if(!this->IsRetreiveBlocked())
		{
            _touchedIDs.insert(nID & 0xFFFF);
			this->Dirty();
            if(BN_CLICKED== nCode)
            {
                this->OnAction();
            }
        }
	}
    return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


LONG CBaseDlg::OnRedraw(WPARAM, LPARAM)
{
    Invalidate();
    UpdateWindow();
    return 0;
}


HBRUSH CBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    /*
	
	
	CPoint pt(0,0);

	if (this != pWnd)
	{
		// 좌표 계산
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pt.x = -(rc.left + GetSystemMetrics(SM_CXDLGFRAME) - 1) % 55;
		pt.y = -(rc.top + GetSystemMetrics(SM_CYDLGFRAME) - 1)% 53;
		
	}
    
    if(GetClassName(pWnd->m_hWnd,cn,128))
//    if(stricmp(cn,"edit"))
  //      pDC->SetBkMode(TRANSPARENT);
	// 브러시 원점 설정
	theApp._brush[1].UnrealizeObject(); 
	pDC->SetBrushOrg(pt); 
*/
    UINT id =  pWnd->GetDlgCtrlID();
    
	char cn[128];
	if(GetClassName(pWnd->m_hWnd,cn,128))
    if(!stricmp(cn,"edit"))
	{
        if(_touchedIDs.find(id) != _touchedIDs.end())
        {
            pDC->SetBkColor(RGB(0,0,0));
            pDC->SetTextColor(RGB(255,128,128));
        }
        else
        {
		    pDC->SetBkColor(RGB(0,0,0));
            pDC->SetTextColor(RGB(210,255,210));
        }
		return (HBRUSH)GetStockObject(BLACK_BRUSH);
	}

	pDC->SetBkColor(theApp._color[1]);
	return (HBRUSH)theApp._brush[1];
}

void C_TabCtrl::OnNcPaint() 
{
}

void CBaseDlg::OnNcPaint() 
{
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnNcPaint() for painting messages
}



C_Static::C_Static()
{
}

C_Static::~C_Static()
{
}


BEGIN_MESSAGE_MAP(C_Static, CStatic)
	//{{AFX_MSG_MAP(C_Static)
	ON_WM_CTLCOLOR_REFLECT()
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C_Static message handlers

HBRUSH C_Static::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    pDC->SetTextColor(RGB(0,0,128));
    pDC->SetBkMode(TRANSPARENT);
	 return 0;;//(HBRUSH)GetStockObject(BLACK_BRUSH);
}

void C_Static::OnPaint() 
{
  
	CPaintDC dc(this); // device context for painting
//	CStatic::OnPaint() ;
  
    DWORD dws = GetStyle();

    if(dws&0x7)
    {
        RECT rt;
        GetClientRect(&rt);
        rt.top+=6;
        dc.Draw3dRect(&rt,theApp._color[0],theApp._color[2]);
    }

    CFont* pf = GetParent()->GetFont();

    CFont* pop = dc.SelectObject(pf);
    CString cs;
    GetWindowText(cs);
    dc.SetBkMode(TRANSPARENT);
    if(IsWindowEnabled())
        dc.SetTextColor(RGB(255,255,255));
    else
        dc.SetTextColor(theApp._color[0]);
    dc.TextOut(0,0,cs);
    dc.SelectObject(pop);

}

void C_Static::PreSubclassWindow() 
{
    	
	CStatic::PreSubclassWindow();
}

void CBaseDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CDialog::OnShowWindow(bShow, nStatus) ;
}


void    CBaseDlg::DisableAllCtrls()
{
    char    tw[128];
    CWnd*   pWnd     = this;
    int     relation = GW_CHILD|GW_HWNDFIRST;
    while(pWnd = pWnd->GetWindow(relation))
    {
        relation = GW_HWNDNEXT;
        pWnd->GetWindowText(tw, 3);
        if(tw[0]=='+' || tw[0]=='-')
		{
			pWnd->EnableWindow(1);
		}
        else
        {
            pWnd->EnableWindow(0);
        }
    }

}

BOOL CBaseDlg::IsRetreiveBlocked()
{
    return BARDLG()->IsRetreiveBlocked() || _updatingCtrls;
}

void    CBaseDlg::Dirty()
{
    _dirty = 1;
    if(!_updatingCtrls)
    {
        BARDLG()->Dirty();
    }
    
}

LRESULT CBaseDlg::OnPostInitDialog(WPARAM w, LPARAM l)
{
    _updatingCtrls = 0;
    return 0L;
}

