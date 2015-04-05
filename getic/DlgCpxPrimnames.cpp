// DlgCpxPrimnames.cpp : implementation file
//

#include "stdafx.h"
#include "scene.h"
#include "z_ed2doc.h"
#include "z-edmap.h"
#include "DlgCpxPrimnames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgCpxPrimnames dialog


DlgCpxPrimnames::DlgCpxPrimnames(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgCpxPrimnames::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgCpxPrimnames)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgCpxPrimnames::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCpxPrimnames)
	DDX_Control(pDX, IDOK3, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, TR_BROWSE, _dirtree);
	DDX_Control(pDX, IDC_LIST1, _lbModels);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgCpxPrimnames, CBASEDLG)
	//{{AFX_MSG_MAP(DlgCpxPrimnames)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK3, OnOk3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCpxPrimnames message handlers

BOOL DlgCpxPrimnames::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();

    theApp.DoWaitCursor(1);

    _lbModels._edit.SetLimitText(15);

    _dirtree.PreSelectFolder(theApp.PrefsDir());
    _curPath = theApp.PrefsDir();

	PBrushes* pBrushes = DOC()->_scene.GetBrushes();
    int iCount = 0;
    FOREACH(PBrushes, (*pBrushes), ppCpxBrush)
    {
        Brush* pBrush = *ppCpxBrush;

        _lbModels.InsertItem(new CLbItem(CLbItem::CTEXT,MKSTR("Brush %d", iCount), pBrush->_name));
        iCount++;
    }
	
    theApp.DoWaitCursor(0);
	return TRUE;  
}


void DlgCpxPrimnames::OnCancel()  
{
	// TODO: Add extra cleanup here
	
	CBASEDLG::OnCancel();
}

int DlgCpxPrimnames::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBASEDLG::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}


void DlgCpxPrimnames::OnOk3() 
{
    _lbModels.KillFOcus(1);

	PBrushes* pBrushes = DOC()->_scene.GetBrushes();

    int iCount = 0;
    char   prop[32];

    FOREACH(PBrushes, (*pBrushes), ppCpxBrush)
    {
        Brush* pBrush = *ppCpxBrush;
        
        _stprintf(prop,"Brush %d", iCount);
        _tcscpy( pBrush->_name, _lbModels.GetProp( prop) );
        iCount++;
    }
    _curPath = _dirtree.GetItemPath();
	theApp.PrefsDir(_curPath.c_str());
    CBASEDLG::OnOK();
	
}

void DlgCpxPrimnames::OnOk() 
{
    _dirtree.PostMessage(WM_KEYDOWN,VK_RIGHT);
    _dirtree.PostMessage(WM_KEYUP,VK_RIGHT);
}
