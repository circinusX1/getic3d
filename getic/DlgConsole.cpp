// DlgConsole.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "geticgui.h"
#include "DlgConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConsole dialog


CDlgConsole::CDlgConsole(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgConsole::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConsole)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgConsole::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConsole)
	DDX_Control(pDX, PB_RUN, m_r);
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, EF_RESPONSE, _out);
	DDX_Control(pDX, EF_CONSOLE, _console);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConsole, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgConsole)
	ON_BN_CLICKED(PB_RUN, OnRun)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConsole message handlers

void CDlgConsole::OnOK() 
{
//	CBASEDLG::OnOK();
}

void CDlgConsole::OnCancel() 
{
	// TODO: Add extra cleanup here
	CBASEDLG::OnCancel();
    DestroyWindow();
}


void CDlgConsole::OnRun() 
{
	CString cs;
    _console.GetWindowText(cs);


	
}

void CDlgConsole::OnDestroy() 
{
    //_console.SetFont(0);
    //_hf.DeleteObject();
	CBASEDLG::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CDlgConsole::OnInitDialog() 
{

	CBASEDLG::OnInitDialog();
	
/*
    _hf.CreateFont( -10, 0, 0, 0, FW_MEDIUM, FALSE,
                   FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
                   CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                   FF_DONTCARE | DEFAULT_PITCH, "FixedSys" );

    _console.SetFont(&_hf);
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
