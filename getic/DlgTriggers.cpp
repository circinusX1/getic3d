// DlgTriggers.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgTriggers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgTriggers dialog


DlgTriggers::DlgTriggers(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTriggers::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgTriggers)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgTriggers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgTriggers)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgTriggers, CDialog)
	//{{AFX_MSG_MAP(DlgTriggers)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgTriggers message handlers
