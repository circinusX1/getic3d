// DlgCompilerPath.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgCompilerPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCompilerPath dialog


CDlgCompilerPath::CDlgCompilerPath(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgCompilerPath::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCompilerPath)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCompilerPath::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCompilerPath)
	DDX_Control(pDX, PB_SELPATH, m_3);
	DDX_Control(pDX, IDOK, m_2);
	DDX_Control(pDX, IDCANCEL, m_1);
	DDX_Control(pDX, IDC_EDIT1, _efPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCompilerPath, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgCompilerPath)
	ON_BN_CLICKED(PB_SELPATH, OnSelpath)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCompilerPath message handlers

void CDlgCompilerPath::OnSelpath() 
{
    const char* pfld = BrowseForFolder("Select script directory",  "");
    if(pfld)
        _efPath.SetWindowText(pfld);
}

void CDlgCompilerPath::OnOk() 
{
    char szpath[_MAX_PATH];
    _efPath.GetWindowText(szpath,_MAX_PATH);
    theApp.CompilerDir(szpath);
    CBASEDLG::OnOK();
}

BOOL CDlgCompilerPath::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
    _efPath.SetWindowText(theApp.CompilerDir());
	return TRUE;  
}
