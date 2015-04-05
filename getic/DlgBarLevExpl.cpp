// DlgBarLevExpl.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgBarLevExpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBarLevExpl dialog


CDlgBarLevExpl::CDlgBarLevExpl(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgBarLevExpl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBarLevExpl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBarLevExpl::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarLevExpl)
	DDX_Control(pDX, IDC_LIST1, _list);
	DDX_Control(pDX, IDC_TREE1, _tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBarLevExpl, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgBarLevExpl)
	ON_BN_CLICKED(PB_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBarLevExpl message handlers

BOOL CDlgBarLevExpl::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	return TRUE;  
}


void CDlgBarLevExpl::OnRefresh() 
{
	
}
