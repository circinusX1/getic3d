// DlgUnlock.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgUnlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgUnlock dialog


DlgUnlock::DlgUnlock(CWnd* pParent /*=NULL*/)
	: CDialog(DlgUnlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgUnlock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgUnlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgUnlock)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgUnlock, CDialog)
	//{{AFX_MSG_MAP(DlgUnlock)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(PB_UNLOCK, OnUnlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgUnlock message handlers

void DlgUnlock::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DlgUnlock::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

void DlgUnlock::OnUnlock() 
{
	// TODO: Add your control notification handler code here
	
}
