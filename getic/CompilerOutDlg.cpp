//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z-edmap.h"
#include "resource.h"
#include "CompilerOutDlg.h"
#include "CCrystalTextView.h"
#include "CCrystalTextBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CCompilerOutDlg::CCompilerOutDlg(CWnd* pParent /*=NULL*/)
//--| CDialogBar|------------------------------------------------------------------------
: CDialogBar()
{
    //{{AFX_DATA_INIT(CCompilerOutDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

/*
//--| CCompilerOutDlg::DoDataExchange|----------------------------------------------------
void CCompilerOutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogBar::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCompilerOutDlg)
	DDX_Control(pDX, PC_CLOSE, m_close);
	DDX_Control(pDX, PB_CANCEL, m_pbprout);
    DDX_Control(pDX, LB_OUT, _lbout);
	//}}AFX_DATA_MAP
}
*/


BEGIN_MESSAGE_MAP(CCompilerOutDlg, CDialogBar)
//{{AFX_MSG_MAP(CCompilerOutDlg)
ON_BN_CLICKED(IDOK, OnOk)
ON_WM_SIZE()
ON_WM_DESTROY()
ON_LBN_DBLCLK(LB_OUT, OnDblclkOut)
	ON_BN_CLICKED(PC_CLOSE, OnClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
ON_MESSAGE(WM_INITDIALOG, HandleInitDialog2)
END_MESSAGE_MAP()


void CCompilerOutDlg::OnCancel()
{
}

void CCompilerOutDlg::OnOk()
{
}

//--| CCompilerOutDlg::OnSize|------------------------------------------------------------
void CCompilerOutDlg::OnSize(size_t nType, int cx, int cy)
{
    CDialogBar::OnSize(nType, cx, cy);
    
    if(_lbout.m_hWnd)
      _lbout.MoveWindow(3,3,cx-6,cy-6);
}

//--| CCompilerOutDlg::HandleInitDialog2|-------------------------------------------------
LRESULT CCompilerOutDlg::HandleInitDialog2(WPARAM w, LPARAM l)
{
    LRESULT lr =  HandleInitDialog(w,l);
    
    _lbout.SubclassDlgItem(LB_OUT,this);
    _lbout.SetFont(CCrystalTextView::PView->GetFont(1,0));
    
    return lr;
}

//--| CCompilerOutDlg::OnDestroy|---------------------------------------------------------
void CCompilerOutDlg::OnDestroy()
{
    CDialogBar::OnDestroy();
    
    // TODO: Add your message handler code here
    
}


//--| CCompilerOutDlg::OnDblclkOut|-------------------------------------------------------
void CCompilerOutDlg::OnDblclkOut()
{
    char sstring[512];
    int idx = _lbout.GetCurSel();
    _lbout.GetText(idx,sstring);
    
    char* pLineNo = _tcschr(sstring,_T(':'));
    
    CCrystalTextView::PView->GetBuffer()->RemoveAllFlags(LF_COMPILATION_ERROR);
    
    
    
    if(pLineNo)
    {
        long nLine = ::_ttoi(pLineNo+1);
        if(nLine>=1)
        {
            CCrystalTextView::PView->GetBuffer()->SetLineFlag(nLine-1, LF_COMPILATION_ERROR,1);
            CCrystalTextView::PView->EnsureVisible(CPoint(0,nLine-1));
        }
    }
    
}


//--| CCompilerOutDlg::AddText|-----------------------------------------------------------
void CCompilerOutDlg::AddText(const char* p)
{
    int i = _lbout.AddString(p);
    _lbout.SetCurSel(i);
    
}


void CCompilerOutDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	
}

HBRUSH CCompilerOutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor) 
{
	HBRUSH hbr = CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return theApp._brush[1];
}
