// DlgBrSet1.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgBrSet1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgBrSet1 dialog


DlgBrSet1::DlgBrSet1(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgBrSet1::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgBrSet1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgBrSet1::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgBrSet1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgBrSet1, CBASEDLG)
	//{{AFX_MSG_MAP(DlgBrSet1)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_CBN_CLOSEUP(CB_GROUP, OnCloseupGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgBrSet1 message handlers

BOOL DlgBrSet1::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgBrSet1::OnDestroy() 
{
	CBASEDLG::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL DlgBrSet1::OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBASEDLG::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void DlgBrSet1::OnShowWindow(BOOL bShow, size_t nStatus) 
{
	CBASEDLG::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

void DlgBrSet1::OnCloseupGroup() 
{
	// TODO: Add your control notification handler code here
	
}
