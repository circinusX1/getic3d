///////////////////////////////////////////////////////////////////////////////
// File: LbEditable.cpp  
//
//  Last Changed:18-04-1997  by COM
//
//                                                               Zal 17 APR 97 
//  This File Is downloaded from                 toronto.planeteer.com/~zalmoxe
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "LbEditable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CLbEditable, CListBox)
//{{AFX_MSG_MAP(CLbEditable)
ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
ON_WM_DESTROY()
ON_WM_KILLFOCUS()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
CLbEditable::CLbEditable():_pParent(0),_withKeys(0),_force(0)
{
    
}

CLbEditable::~CLbEditable()
{
}



////////////////////////////////////////////////////////////////////////////////
void CLbEditable::DrawItem(DRAWITEMSTRUCT* pDraw)
{
    CString cLocal;
    RECT rWork;
    int  crOldBack,crOldText;
    
    CDC* pDC = CDC::FromHandle( pDraw->hDC );

    ::CopyRect(&rWork, &pDraw->rcItem);
	int wide = rWork.right-rWork.left;
	int height = rWork.bottom-rWork.top;

	CLbItem* pItem = (CLbItem*)pDraw->itemData;
	if(pItem==0)
		return;

    COLORREF   ccc = theApp._color[1];
    BYTE* pbc = (BYTE*)&ccc;

    pbc[0]*=.9;
    pbc[1]*=.9;
    pbc[2]*=.9;

    crOldBack = pDC->SetBkColor(ccc);

	rWork.right /=2; 
	rWork.right -=1;
    

    if(!_withKeys || !(pDraw->itemState & ODS_SELECTED))
    {
        pDC->ExtTextOut(rWork.left+2,
                    rWork.top,
                    ETO_OPAQUE, &rWork,"", 0, NULL ); 

        crOldText = pDC->SetTextColor(theApp._txtcolor);

	    pDC->TextOut(pDraw->rcItem.left+2,	
				     pDraw->rcItem.top+2, 
				     pItem->_prop.c_str(), pItem->_prop.length());

	    rWork.right+=1; 
	    rWork.right*=2; 
    }

    if (!(pDraw->itemState & ODS_SELECTED)) 
    {

        rWork.left+=wide/2;

        crOldBack = pDC->SetBkColor(theApp._color[1]);
        pDC->ExtTextOut(rWork.left ,
                        rWork.top,
                        ETO_OPAQUE, &rWork,"", 0, NULL );// fast then Fill Rect
		crOldText = pDC->SetTextColor(theApp._txtcolor);

		pDC->TextOut(pDraw->rcItem.left+wide/2,	
						pDraw->rcItem.top, 
						pItem->_curval.c_str(), pItem->_curval.length());

    }

    if (pDraw->itemState & ODS_SELECTED )  // up button
        ((CDC*)CDC::FromHandle(pDraw->hDC))->Draw3dRect(&pDraw->rcItem,
                                theApp._color[1],theApp._color[2]);
    else
        ((CDC*)CDC::FromHandle(pDraw->hDC))->Draw3dRect(&pDraw->rcItem,
                                theApp._color[2],theApp._color[1]);

    // Return the device context to its original state.
    pDC->SetTextColor( crOldText );
    pDC->SetBkColor( crOldBack );

}

////////////////////////////////////////////////////////////////////////////////
void CLbEditable::PreSubclassWindow()
{
    RECT rt = {0};
    CWnd*     pParent;
    
    // Change the style from resource to be  LBS_OWNERDRAWFIXED|LBS_HASSTRINGS
    ASSERT(::GetWindowLong(m_hWnd,GWL_STYLE) & (LBS_OWNERDRAWFIXED|LBS_HASSTRINGS) );
    
    CListBox::PreSubclassWindow();
    
    // create the edit that'a a child in the listbox
    _edit.Create( WS_CHILD|WS_BORDER|ES_MULTILINE|ES_RIGHT,rt,this,1001);
    _editkey.Create( WS_CHILD|WS_BORDER|ES_MULTILINE|ES_LEFT,rt,this,1000);
    if(pParent = GetParent())
    {
        _edit.SetFont(pParent->GetFont());
        _edit.SetLimitText(15);

        _editkey.SetFont(pParent->GetFont());
        _editkey.SetLimitText(7);
    }
    _edit._pParent = this;
    _editkey._pParent = this;

	_combo.Create( WS_CHILD|CBS_DROPDOWNLIST | CBS_SORT | 
                    WS_VSCROLL ,rt,this,1002);
    if(pParent = GetParent())
    {
        _combo.SetFont(pParent->GetFont());
    }
    _combo._pParent = this;
    
}

////////////////////////////////////////////////////////////////////////////////
//    FUNCTION:  CLbEditable::OnSelchange
void CLbEditable::OnSelchange()
{
	int isel = GetCurSel();

	if(_force || (_edit._index!=-1 && _edit._index != isel))
	{
        char s[128]   = {0}; 
        CLbItem* pItem = 0;
		// leave in the selection the edit text
        if(_edit._index>=0)
        {
		    pItem = (CLbItem*)GetItemData(_edit._index);
		    
            _edit.GetWindowText(s,128);
		    pItem->_curval = s;
        }

        if(pItem && _withKeys)
        {
            _editkey.GetWindowText(s,128);
	    	pItem->_prop = s;
        }
	}

	if(_combo._index!=-1 && _combo._index != isel)
	{
		CLbItem* pItem = (CLbItem*)GetItemData(_combo._index);
		int cbsel = _combo.GetCurSel();
		CString cs;
		_combo.GetLBText(cbsel,cs);
		pItem->_curval = (LPCSTR)cs;
	}
	
	CLbItem* pItem = (CLbItem*)GetItemData(isel);

    RECT rect;
    this->GetItemRect(isel, &rect);

	if(isel!=-1)
	{
        if(_withKeys)
        {
            _editkey.SetWindowText(pItem->_prop.c_str());
            _editkey._index = isel;
			_editkey.ShowWindow(SW_SHOW);
            _editkey.MoveWindow(rect.left, 
                                 rect.top,
                                 ((rect.right-rect.left)/2)-2, 
                                 rect.bottom-rect.top ,0);

        }
        

		switch(pItem->_t)
		{
			case CLbItem::CTEXT:
				_edit.SetWindowText(pItem->_curval.c_str());
				_edit._index = isel;
				_edit.ShowWindow(SW_SHOW);

                _edit.MoveWindow(rect.left + (rect.right-rect.left)/2, 
                                 rect.top,
                                 (rect.right-rect.left)/2, 
                                 rect.bottom-rect.top ,0);



				_combo.ShowWindow(SW_HIDE);
				break;
			case CLbItem::CBOX:
			case CLbItem::CBOXDIR:
				{
				    _combo.ResetContent();
				    vvector<tstring>::iterator s = pItem->_cbStrings.begin();
				    vvector<tstring>::iterator e = pItem->_cbStrings.end();
				    for(;s!=e;s++)
					    _combo.AddString(s->c_str());
				    _combo.SelectString(-1,pItem->_curval.c_str());
				    _combo._index = isel;
				    _combo.ShowWindow(SW_SHOW);
				    _edit.ShowWindow(SW_HIDE);

                    _combo.MoveWindow(rect.left + (rect.right-rect.left)/2, 
                                 rect.top,
                                 (rect.right-rect.left)/2, 
                                 rect.bottom-rect.top ,0);

                    _combo.SelectString(-1, pItem->_curval.c_str());
				}
				break;
		}
	}


	Invalidate();
}

////////////////////////////////////////////////////////////////////////////////

void CLbEditable::OnDestroy()
{
	int ic =  GetCount();
	for(int i=0;i<ic;i++)
	{
		CLbItem* pItem = (CLbItem*)GetItemData(i);
		delete pItem;
	}
    if(_edit.m_hWnd)
      _edit.DestroyWindow();
    if(_combo.m_hWnd)
      _combo.DestroyWindow();
    _editkey.DestroyWindow();
    CListBox::OnDestroy();
}



BEGIN_MESSAGE_MAP(CCbFromLb, CComboBox)
    //{{AFX_MSG_MAP(CCbFromLb)
    //}}AFX_MSG_MAP
    ON_WM_KEYDOWN()
    ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)

END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CEditFromLb, CEdit)
    //{{AFX_MSG_MAP(CEditFromLb)
    //}}AFX_MSG_MAP
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()



void CCbFromLb::OnCloseup()
{
    if(_pParent && _pParent->_pParent)
	_pParent->_pParent->PostMessage(WM_DIRTY,0,0);
}

void CCbFromLb::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(nChar == VK_RETURN && _pParent)
        _pParent->KillFOcus(0);
    if(_pParent && _pParent->_pParent)
        _pParent->_pParent->PostMessage(WM_DIRTY,0,0);
    CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEditFromLb::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(nChar == VK_RETURN && _pParent)
        _pParent->KillFOcus(0);
    if(_pParent && _pParent->_pParent)
        _pParent->_pParent->PostMessage(WM_DIRTY,0,0);
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CLbEditable::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight=24;
}

void CLbEditable::KillFOcus(BOOL force)
{
    _force = 1;
    OnSelchange();
    if(::IsWindow(_edit.m_hWnd))
	    _edit.ShowWindow(SW_HIDE);
    if(::IsWindow(_combo.m_hWnd))
	    _combo.ShowWindow(SW_HIDE);
    _editkey.ShowWindow(SW_HIDE);
    SetCurSel(-1);
    

}

void CLbEditable::OnKillFocus(CWnd* pNewWnd) 
{
	if(pNewWnd != &_edit && pNewWnd != &_combo && pNewWnd != &_editkey)
	{
		CListBox::OnKillFocus(pNewWnd);
        KillFOcus(0);
	}
}

void    CLbEditable::LBSetProp(const char* prop, int sel)
{
    int cnt = GetCount();
    CLbItem* pItem = 0;
    for(int i=0;i<cnt;i++)
    {
        pItem = (CLbItem*)GetItemData(i);
        if(pItem->_prop == prop)
        {
            break;
        }
        pItem=0;
    }
    if(0 == pItem)
        return;
    switch(pItem->_t)
    {
        case CLbItem::CBOX:
        case CLbItem::CBOXDIR:
             pItem->_curval = pItem->_cbStrings[sel];
            break;
    }
    Invalidate();
}

void    CLbEditable::LBSetProp(const char* prop, CLR& sel)
{
    char s[32];
    _stprintf(s,"%d,%d,%d",sel.r,sel.g,sel.b);
    LBSetProp(prop, s);
}


void    CLbEditable::LBSetProp(int idx, char* sel)
{
    CLbItem* pItem = (CLbItem*)GetItemData(idx);

    switch(pItem->_t)
    {
        case CLbItem::CTEXT:
            pItem->_curval = sel;
            break;
        case CLbItem::CBOX:
        case CLbItem::CBOXDIR:
            FOREACH(vvector<tstring>,	pItem->_cbStrings, pstr)
            {
                if(*pstr == sel)
                {
                    pItem->_curval = sel;
                    break;
                }
            }
            break;
    }
    Invalidate();
}

const char*   CLbEditable::GetProp(int idx)
{
    CLbItem* pItem = (CLbItem*)GetItemData(idx);
    return pItem->_curval.c_str();
}


void    CLbEditable::LBSetProp(const char* prop, char* sel)
{
    int cnt = GetCount();

    for(int i=0;i<cnt;i++)
    {
        CLbItem* pItem = (CLbItem*)GetItemData(i);
        if(pItem->_prop == prop)
        {
            pItem->_curval = sel;
            break;
        }
    }
    Invalidate();
}

const char*   CLbEditable::GetProp(char* idx)
{
    int cnt = GetCount();

    for(int i=0;i<cnt;i++)
    {
        CLbItem* pItem = (CLbItem*)GetItemData(i);
        if(pItem->_prop == idx)
        {
            return pItem->_curval.c_str();
        }
    }
    return 0;
}
