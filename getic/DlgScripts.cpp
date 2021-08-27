// DlgScripts.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgScripts.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgScripts dialog


DlgScripts::DlgScripts(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgScripts::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgScripts)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgScripts::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgScripts)
	DDX_Control(pDX, IDC_LIST1, _list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgScripts, CBASEDLG)
	//{{AFX_MSG_MAP(DlgScripts)
	ON_BN_CLICKED(PB_NEW, OnNew)
	ON_BN_CLICKED(PB_DEL, OnDel)
	ON_BN_CLICKED(PB_EDIT, OnEdit)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ITEMSEL, OnItemSelected)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgScripts message handlers


void DlgScripts::OnNew() 
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,MN_SHOWCOMPILER);
}

void DlgScripts::OnDel() 
{
	CString		cst;
	int			iSel = _list.GetCurSel();
	vvector<ScriptItem*>* ps = DOC()->_scene.GetScripts();
    
    FOREACH(vvector<ScriptItem*>, (*ps) ,ppScr)
    {
		ScriptItem* pSi = *ppScr;
		cst = _list.GetItemText(iSel,0);
		if(::atol(cst) == (size_t)pSi)
		{

			SCENE().DelScript(pSi);
			_list.DeleteItem(iSel);
			break;
		}
    }
}

void DlgScripts::OnEdit() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MN_SHOWCOMPILER);
}


BOOL DlgScripts::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	_list.SetHeadersNames("","Name                ","Object   ",0);
	Refresh();
	return 1;
}

BOOL DlgScripts::Refresh()
{
	_list.DeleteAllItems();
	vvector<ScriptItem*>* ps = DOC()->_scene.GetScripts();
    
    FOREACH(vvector<ScriptItem*>, (*ps) ,ppScr)
    {
		ScriptItem* pSi = *ppScr;
		PathHandler ph(pSi->_script.c_str());
		_list.AddNewItem(MKSTR("%d",pSi),ph.File(),pSi->_name,0);
    }
	Invalidate();
	return TRUE;  
}

void DlgScripts::OnShowWindow(BOOL bShow, size_t nStatus) 
{
	CBASEDLG::OnShowWindow(bShow, nStatus);
	if(bShow)
		Refresh();
	
}

LRESULT DlgScripts::OnItemSelected(WPARAM itm, LPARAM lst)
{
	return 0;
}