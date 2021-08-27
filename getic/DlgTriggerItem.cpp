// DlgTriggerItem.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "SceItem.h"
#include "TriggerItem.h"
#include "DlgTriggerItem.h"
#include "DlgBar.h"
#include "z_ed2Doc.h"
#include "Scripts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgTriggerItem dialog


DlgTriggerItem::DlgTriggerItem(CWnd* pParent): CBASEDLG(DlgTriggerItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgTriggerItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _pCurTrigger = 0;
}


void DlgTriggerItem::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgTriggerItem)
	DDX_Control(pDX, LB_DETBRUSHES, _lbDetBrush);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgTriggerItem, CBASEDLG)
	//{{AFX_MSG_MAP(DlgTriggerItem)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_LBN_DBLCLK(LB_DETBRUSHES, OnDblclkTarget)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgTriggerItem message handlers
void DlgTriggerItem::OnCancel() 
{
}

void DlgTriggerItem::OnOk() 
{

}

BOOL DlgTriggerItem::OnInitDialog() 
{
    _updatingCtrls = 1;
	CBASEDLG::OnInitDialog();
    RefreshTriggerEfects();
    _updatingCtrls = 0;
	return TRUE;  
}

void    DlgTriggerItem::Update(SceItem* pItemG,int selCount)
{
    if(pItemG->_item != _itmType)
        return;
	RefreshTriggerEfects();
    _updatingCtrls = 1;
    

	if(pItemG->_item != ITM_TRIGER)
		return;

    TriggerItem* pTi = (TriggerItem*)pItemG;

	_pCurTrigger = pTi;

    if(pTi->GetEfectBrush())
	{
		_lbDetBrush.EnableWindow(1);
		CheckDlgButton(CK_BRUSH,BST_CHECKED);
		_lbDetBrush.SelectString(-1, pTi->GetEfectBrush()->_name);
	}
    else
	{
		_lbDetBrush.EnableWindow(0);
		CheckDlgButton(CK_BRUSH,BST_UNCHECKED);
        _lbDetBrush.SetCurSel(-1);
    }
    _updatingCtrls = 0;
}

void    DlgTriggerItem::Retreive(SceItem* pItemG,int selCount)
{
    if(pItemG->_item != _itmType)
        return;

    TriggerItem* pTi = (TriggerItem*)pItemG;
    pTi->SetEfectBrush(0);
	if(IsDlgButtonChecked(CK_BRUSH))
	{
		int iSel = _lbDetBrush.GetCurSel();
		if(iSel >=0)
		{
            Brush* pB = (Brush*)_lbDetBrush.GetItemData(iSel);
            if(!(pB->_brushflags& BRSH_HASTRIG))
            {
    			pTi->SetEfectBrush((Brush*)_lbDetBrush.GetItemData(iSel),1);
            }
		}
	}
}


void DlgTriggerItem::RefreshTriggerEfects()
{
    _lbDetBrush.ResetContent();

    PBrushes* rbs = DOC()->GetForms();
    FOREACH(PBrushes, (*rbs), ppB)
    {
        Brush& rb = (**ppB);

        //
        // reject other brushes and non detail brushes
        //
        if(DOC()->_curgroup>=0 && DOC()->_curgroup != rb._group)
            continue;
        if(!(rb._brushflags & BRSH_DETAIL))
            continue;
        
        int idx = _lbDetBrush.AddString(rb._name);
        _lbDetBrush.SetItemData(idx,(size_t)(*ppB));
    }
    _lbDetBrush.SetCurSel(-1);
}

void    DlgTriggerItem::OnAction()
{
    if(BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}

void DlgTriggerItem::OnDblclkTarget() 
{
    OnAction();
}


void DlgTriggerItem::OnShowWindow(BOOL bShow, size_t nStatus) 
{
	CBASEDLG::OnShowWindow(bShow, nStatus);
	if(0==bShow)
		_pCurTrigger = 0;	
	
}


void     DlgTriggerItem::Show(BOOL b)
{
    ShowWindow(b ? SW_SHOW : SW_HIDE);
    if(b)
    {
        DelayUpdateWindow(m_hWnd);
    }
};
