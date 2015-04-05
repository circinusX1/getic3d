// GListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "GListCtrl.h"
#include "DlgBrshProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGListCtrl

CGListCtrl::CGListCtrl():_curSel(-1),_updating(FALSE)
{
}

CGListCtrl::~CGListCtrl()
{
}


BEGIN_MESSAGE_MAP(CGListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CGListCtrl)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGListCtrl message handlers

void CGListCtrl::PreSubclassWindow() 
{
    SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
    SetBkColor( theApp._color[1]);
    SetTextColor( RGB(0,0,0));

#ifdef _SKINNY    
    CHeaderCtrl* ph = GetHeaderCtrl();
    //ph->ModifyStyle(0,HDF_OWNERDRAW);
    _hdrCtrl.SubclassWindow(ph->m_hWnd);
#endif //_SKINNY


	CListCtrl::PreSubclassWindow();
}


void CGListCtrl::SetHeadersNames(LPCSTR pFirst,...)
{
    _updating=1;
    int i=0;
    

    LV_COLUMN   lvcol ={LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH,
        LVCFMT_LEFT,NULL,0,0};
    va_list marker;
    va_start( marker, pFirst );
    LPSTR    pF = (char*)pFirst;
    while(  pF)
    {
        lvcol.pszText   = pF;
        lvcol.iSubItem  = i++;
        lvcol.cx        = strlen(pF)*8+16;
        InsertColumn(i,&lvcol);
        pF = va_arg( marker,char* );
    }
    va_end( marker );
    _updating=0;

#ifdef _SKINNY
    for(i=0;i<_hdrCtrl.GetItemCount( );i++)
    {
        HD_ITEM hditem;
        hditem.mask = HDI_FORMAT;
	    _hdrCtrl.GetItem( i, &hditem );
	    hditem.fmt |= HDF_OWNERDRAW;
	    _hdrCtrl.SetItem( i, &hditem );
    }
#endif //#ifdef _SKINNY
}

int CGListCtrl::LstSetItem(int iItem, LPCSTR pFirst,...)
{
	_updating=1;
    LV_ITEM     lvi;
    int         pos =  iItem;
    int         i=0;
    
    va_list marker;
    va_start( marker, pFirst );
    LPSTR    pF = (char*)pFirst;
    while(  pF)
    {
        lvi.mask     = LVIF_TEXT;
        lvi.iImage   = 0;
        lvi.lParam   = 0;
        lvi.iItem	 = pos;
        lvi.iSubItem = i;
        lvi.pszText  = pF;
        SetItem(&lvi);
        
        pF = va_arg( marker,char* );
        if(++i >((CHeaderCtrl*)GetDlgItem(0))->GetItemCount())
        {
            // items exceed the columns number
            ASSERT(0);
            break;
        }
        
    }
	_curSel=0;
	_updating=0;
    return  GetItemCount();
}

int CGListCtrl::AddNewItem(LPCSTR pFirst,...)
{
	_updating=1;
    LV_ITEM     lvi;
    int         pos =  GetItemCount();
    int         i=0;
    
    va_list marker;
    va_start( marker, pFirst );
    LPSTR    pF = (char*)pFirst;
    while(  pF)
    {
        lvi.mask     = LVIF_TEXT;
        lvi.iImage   = 0;
        lvi.lParam   = 0;
        lvi.iItem	 = pos;
        lvi.iSubItem = i;
        lvi.pszText  = pF;
        if(!i)
          InsertItem(&lvi);
        else
            SetItem(&lvi);
        
        pF = va_arg( marker,char* );
        if(++i >((CHeaderCtrl*)GetDlgItem(0))->GetItemCount())
        {
            // items exceed the columns number
            ASSERT(0);
            break;
        }
        
    }
	_curSel=0;
	_updating=0;
    return  GetItemCount();
}


void CGListCtrl::SetCurSel(int sel)
{
    if(_curSel>=0)
        SetItemState(_curSel, ~(LVIS_SELECTED|LVIS_FOCUSED), LVIS_SELECTED|LVIS_FOCUSED);
    if(sel>=0)
        SetItemState(sel, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    _curSel = sel;
}


void CGListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(_updating)return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    _curSel = pNMListView->iItem;

    GetParent()->SendMessage(WM_ITEMSEL,_curSel,(LPARAM)this);
	*pResult = 0;
}

void CGListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(_updating)return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


    _curSel = pNMListView->iItem;
    GetParent()->SendMessage(WM_ITEMSEL2CLICK,_curSel,(LPARAM)this);
	
	*pResult = 0;
}

void CGListCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
    
	*pResult = 0;
}
