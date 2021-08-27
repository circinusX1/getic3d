// DlgMurrChs.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgMurrChs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgMurrChs dialog


DlgMurrChs::DlgMurrChs(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMurrChs::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgMurrChs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgMurrChs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgMurrChs)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgMurrChs, CDialog)
	//{{AFX_MSG_MAP(DlgMurrChs)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgMurrChs message handlers

void DlgMurrChs::OnCancel() 
{
    EndDialog(0);
}


void DlgMurrChs::OnChar(size_t nChar, size_t nRepCnt, size_t nFlags) 
{
	EndDialog(nChar);
}

void DlgMurrChs::OnOk() 
{
    EndDialog(0);
}

BOOL DlgMurrChs::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message==WM_KEYDOWN)	
    {
        EndDialog(pMsg->wParam);
    }
	return CDialog::PreTranslateMessage(pMsg);
}
