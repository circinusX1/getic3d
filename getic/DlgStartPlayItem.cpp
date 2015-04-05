// DlgStartPlayItem.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "geticgui.h"
#include "SceItem.h"
#include "SoundItem.h"
#include "DlgStartPlayItem.h"
#include "DlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgStartPlayItem dialog
#define         IS_TID(id_) (_touchedIDs.find(id_) !=  _touchedIDs.end())

DlgStartPlayItem::DlgStartPlayItem(CWnd* pParent /*=NULL*/)
	: CVertDlgDlg(DlgStartPlayItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgStartPlayItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgStartPlayItem::DoDataExchange(CDataExchange* pDX)
{
	CVertDlgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgStartPlayItem)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgStartPlayItem, CVertDlgDlg)
	//{{AFX_MSG_MAP(DlgStartPlayItem)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL DlgStartPlayItem::OnInitDialog() 
{
	CVertDlgDlg::OnInitDialog();
    Enable(1);
	return TRUE;  
	              
}

void DlgStartPlayItem::OnDestroy() 
{
	CVertDlgDlg::OnDestroy();
}


void    DlgStartPlayItem::Update(SceItem* pItemG, int selCount)
{
    _updatingCtrls = FALSE;
    ClearDirtyIDs();
}

//-----------------------------------------------------------------------------
void    DlgStartPlayItem::Retreive(SceItem* pItemG, int selCount)
{
    CSoundItem* pItem = (CSoundItem*)pItemG;
}


void     DlgStartPlayItem::Show(BOOL b)
{
    ShowWindow(b ? SW_SHOW : SW_HIDE);
    if(b)
    {
        DelayUpdateWindow(m_hWnd);
    }
};



void DlgStartPlayItem::OnAction() 
{
    if(	BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}

