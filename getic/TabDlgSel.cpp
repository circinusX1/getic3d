// TabDlgSel.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "TabDlgSel.h"
#include "Dlgbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TabDlgSel

TabDlgSel::TabDlgSel()
{
}

TabDlgSel::~TabDlgSel()
{
}


BEGIN_MESSAGE_MAP(TabDlgSel, C_TABCTRL)
	//{{AFX_MSG_MAP(TabDlgSel)
	ON_NOTIFY(TCN_SELCHANGE, TAB_DLGS, OnSelchangeDlgs)
	ON_NOTIFY(TCN_SELCHANGING, TAB_DLGS, OnSelchangingDlgs)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TabDlgSel message handlers

void TabDlgSel::OnSelchangeDlgs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char text[32];
	TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,text,32,0,NULL};
	GetItem(GetCurSel(), &tci);

    CBASEDLG* pDlg = ((CBASEDLG*)tci.lParam);

    RECT rt; 
    _pParent->GetDlgItem(_rects[pDlg])->GetWindowRect(&rt);
    _pParent->ScreenToClient(&rt);

	RECT rd ; pDlg->GetClientRect(&rd);


    pDlg->ShowWindow(SW_SHOW);
    pDlg->MoveWindow(rt.left,rt.top,rt.right-rt.left, 
                     tmin(rd.bottom, rt.bottom-rt.top-32),1);
    BARDLG()->ShowSelItmDlg(GetCurSel());
    pDlg->ClearDirtyIDs();
    

	*pResult = 0;	
    theApp.p_wndFocused = 0;
}

void TabDlgSel::OnSelchangingDlgs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char text[32];
	TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,text,32,0,NULL};
	GetItem(GetCurSel(), &tci);
    CBASEDLG* pDlg = ((CBASEDLG*)tci.lParam);

    BARDLG()->OnApply();

    pDlg->ClearDirtyIDs();
	pDlg->ShowWindow(SW_HIDE);
	*pResult = 0;

    BARDLG()->HideAll();
}


void TabDlgSel::AddDialog(CBASEDLG * pDlg,LPCSTR pTitle,int iMage, UINT placeID)
{
    if(pDlg && pDlg->m_hWnd)
    {
	    TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,NULL,32,0,NULL};
	    tci.pszText = (LPSTR)pTitle;
	    tci.iImage	= iMage;
	    tci.lParam  = (LPARAM)pDlg;
        _rects[pDlg] = placeID;
        InsertItem(GetItemCount(),&tci);
    }
}

