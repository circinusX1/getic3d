// DlgItems.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgItems.h"
#include "DlgBar.h"
#include "LightBulb.h"
#include "z_ed2Doc.h"
#include "zedtypes.h"
#include "MainFrm.h"
#include "ToolFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////



DlgItems::DlgItems(CWnd* pParent /*=NULL*/)
    : CBASEDLG(DlgItems::IDD, pParent),_pCurrent(0)
{
	//{{AFX_DATA_INIT(DlgItems)
	//}}AFX_DATA_INIT
    _blockDocUpdate = FALSE;
}


void DlgItems::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgItems)
	DDX_Control(pDX, CB_GROUP, _cbGroup);
	DDX_Control(pDX, IDC_LIST1, _lbItems);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgItems, CBASEDLG)
	//{{AFX_MSG_MAP(DlgItems)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_SHOWWINDOW()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_CBN_CLOSEUP(CB_GROUP, OnCloseupGroup)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgItems message handlers


BOOL DlgItems::OnInitDialog() 
{
    _updatingCtrls=1;
	CBASEDLG::OnInitDialog();
    RefreshItemsList();
    for(int i=0;i<32;i++)
        _cbGroup.AddString(MKSTR("%d",i));

    ((CEdit*)GetDlgItem(EF_NAME))->SetLimitText(15);
	return TRUE;  
}

void DlgItems::OnOk() 
{
    BARDLG()->OnApply();
}

void DlgItems::OnCancel() 
{
    DOC()->DeselectItems();
    DOC()->Invalidate();
}

void    DlgItems::Update(SceItem* pItem, int selCount)
{
    if(!m_hWnd )
    {
        return;
    }
    if(selCount==0)
    {
        ClearDirtyIDs();
        return;
    }

    _pCurItem = pItem;
    _updatingCtrls=1;
    _cbGroup.SelectString(-1, MKSTR("%d",pItem->_group));

	Sct(this, EF_POSX, pItem->_t.x);
    Sct(this, EF_POSY, pItem->_t.y);
    Sct(this, EF_POSZ, pItem->_t.z);
    GetDlgItem(EF_CATNAME)->SetWindowText(pItem->_catname);
    GetDlgItem(EF_NAME)->SetWindowText(pItem->_name);
    _updatingCtrls = 0;
    ClearDirtyIDs();
 
}


void    DlgItems::Retreive(SceItem* pItem, int selCount)
{
    if(!_dirty)    return;
    if(!IsWindowVisible())
    {
        return;
    }


    if(_touchedIDs.find(EF_POSX) != _touchedIDs.end() ||
        _touchedIDs.find(EF_POSY) != _touchedIDs.end()||
        _touchedIDs.find(EF_POSZ) != _touchedIDs.end())
    {
        V3 np;
	    np.x = ParseREAL(this, EF_POSX);
        np.y = ParseREAL(this, EF_POSY);
        np.z = ParseREAL(this, EF_POSZ);

        np -=  pItem->_t;
	    pItem->Move(np);
    }

    if(_touchedIDs.find(CB_GROUP) != _touchedIDs.end())
    {
        char   szTemp[32];
        _cbGroup.GetLBText(_cbGroup.GetCurSel(), szTemp);
        pItem->_group = ::_ttoi(szTemp);
    }
    if(selCount==1)
    {
        GetDlgItem(EF_NAME)->GetWindowText(pItem->_name,16);
    }
}

void    DlgItems::Activate(int ia)
{
}

void DlgItems::OnShowWindow(BOOL bShow, size_t nStatus) 
{
	CBASEDLG::OnShowWindow(bShow, nStatus);
	Activate(bShow);
	if(_pCurrent)
	{
		_pCurrent->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}

    if(bShow)
    {
        RefreshItemsList();

        int      count;
        SceItem** pSi = DOC()->GetSelItems(count);
        if(count)
        {
            Update(*pSi, count);
        }

    }


}

void    DlgItems::Dirty()
{
    BARDLG()->Dirty();
    _dirty = 1;
}


void    DlgItems::DelItem(SceItem* pItem)
{
    int     cbCnt = _lbItems.GetCount();
    for(int i=0; i < cbCnt; i++)
    {
        if(pItem == (SceItem*)_lbItems.GetItemData(i))
        {
            _lbItems.DeleteString(i);
            ////_itemstates.erase(i);
            break;
        }
    }
}

void    DlgItems::OnItemPropsChanged(SceItem* pItem)
{
    // show here common properties as pos, flags
    Sct(this, EF_POSX, pItem->_t.x);
    Sct(this, EF_POSY, pItem->_t.y);
    Sct(this, EF_POSZ, pItem->_t.z);

    BARDLG()->UpdateItmProps(pItem);
    // show in detail dlg the specific props
    
}
 
int    DlgItems::OnItemSelected(SceItem* pItem, int sel)
{   
    int     cbCnt = _lbItems.GetCount();

    if(pItem==0) //deselect all
    {
        if(!_blockDocUpdate)
        {
            for(int i = 0; i < cbCnt; i++)
            {
                if(_lbItems.GetSel(i))
                {
                    
                    {
                        _lbItems.SetSel(i, 0); 
                    }
                }
            }
        }
        return 0;
    }

    int selitems = 0;
    //select/deselect icoming item
    for(int i=0; i < cbCnt; i++)
    {
        if(pItem == (SceItem*)_lbItems.GetItemData(i))
        {
            if(!_blockDocUpdate)
            {
                _lbItems.SetSel(i, sel); 
                _itemstates[i]=sel;
            }

            if(sel)
            {
                BARDLG()->UpdateItmProps(pItem);
            }
            if(_lbItems.GetSel(i))
            {
                ++selitems;
            }
        }
    }
    return selitems;
}


void DlgItems::OnDblclkList1() 
{
}



void DlgItems::OnCloseupGroup() 
{
    if(!_updatingCtrls)
    {
        Dirty();
        BARDLG()->OnApply();
    }
}

void    DlgItems::RefreshItemsList()
{
    int   idx;
    
    _lbItems.ResetContent();
    vvector<SceItem*>* pItems = DOC()->_scene.GetItems();
    FOREACH(vvector<SceItem*>, (*pItems), ppItem)
    {
        SceItem* pItem =  *ppItem;

        idx = _lbItems.AddString(pItem->_name);
        UpdateItem(pItem, idx);
    }

    SceItem *pItem;
    int     cbCnt = _lbItems.GetCount();
    for(int i=0; i < cbCnt; i++)
    {
        if(pItem = (SceItem*)_lbItems.GetItemData(i))
        {
            if(pItem->IsSelected())
            {
                _lbItems.SetSel(i,1);
            }
        }
    }
}

void    DlgItems::AddItem(SceItem* pItem)
{
	int idx = _lbItems.AddString(pItem->_name);
////    _itemstates[idx] = 0;
    UpdateItem(pItem, idx);
}

void    DlgItems::UpdateItem(SceItem* pItem, int idx)
{
      _lbItems.SetItemData(idx, (size_t)pItem);
}

void DlgItems::OnSelchangeList1() 
{
    
    if(DOC()->_compviewmode != C_NA)
    {
        return;
    }

    if(DOC()->_selmode != SEL_ITEMS)
    {
        DOC()->OnEscape();
        DOC()->_selmode = SEL_ITEMS;
    }
    
    _blockDocUpdate            = TRUE;
/*
    int cs = _lbItems.GetCurSel();
    if(_lbItems.GetSel(cs))
    {
        _lbItems.SetSel(cs,0);
    }
*/

    SceItem *pItem,*pItemlast  = 0;
    int     cbCnt = _lbItems.GetCount();
    DOC()->DeselectItems(0);

    for(int  i=0; i < cbCnt; i++)
    {
        if(pItem = (SceItem*)_lbItems.GetItemData(i))
        {
            BOOL bsel = _lbItems.GetSel(i);
            if(bsel)
            {
                DOC()->SelectItem(pItem, FALSE);
                if(pItemlast==0)
                    pItemlast = pItem;
            }
        }
    }
    if(pItemlast)
    {
        //

        int still = _lbItems.GetSelCount();//_dlgItem.OnItemSelected(pItem, sel);
        BARDLG()->ShowItemProps(pItemlast->_item);
    	if(IsWindowVisible() && _pCurrent)
		{
            if(this->IsNotSameTypeItemsSelected())
            {
                still=0;
            }
            BARDLG()->_pItemBase->Show(still!=0);//hide if 2 different items
		}
        BARDLG()->UpdateItmProps(pItemlast);
    }
	else
	{
        BARDLG()->ShowItemProps(-1);
		if(IsWindowVisible() && _pCurrent && pItem && BARDLG()->_pItemBase)
		{
            if(pItem->_item!=BARDLG()->_pItemBase->ItemType())
            {
			    BARDLG()->_pItemBase->Show(0);
            }
		}
        BARDLG()->_pItemBase = 0;
        BARDLG()->_pItemBaseB= 0;
		this->_pCurrent = 0;
	}
    
    _blockDocUpdate =  FALSE;
    DOC()->Invalidate();
}


BOOL    DlgItems::IsNotSameTypeItemsSelected()
{
    SceItem* pItem;
    int      cbCnt  = _lbItems.GetCount();
    size_t    t = 0;

    for(int  i=0; i < cbCnt; i++)
    {
        if(_lbItems.GetSel(i))
        {
            if(pItem = (SceItem*)_lbItems.GetItemData(i))
            {
                if(0==t)
                {
                    t = pItem->_item;
                }
                else if(t != pItem->_item)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}


