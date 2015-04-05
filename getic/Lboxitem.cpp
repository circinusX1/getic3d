// LboxItem.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "LboxItem.h"
#include "SceItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLboxItem

CLboxItem::CLboxItem()
{
}

CLboxItem::~CLboxItem()
{
}


BEGIN_MESSAGE_MAP(CLboxItem, CListBox)
	//{{AFX_MSG_MAP(CLboxItem)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLboxItem message handlers

void CLboxItem::DrawItem(LPDRAWITEMSTRUCT pDraw) 
{
    char tOut[128]={0};
    int  crOldBack,crOldText;
    
    CDC* pDC = CDC::FromHandle( pDraw->hDC );
    COLORREF hl = theApp._color[2];
    COLORREF sh = theApp._color[0];

    GetText(pDraw->itemID ,tOut);
    if(0==tOut[0])return;

    if (pDraw->itemState & ODS_SELECTED )  // up button
    {
      //  ((CDC*)CDC::FromHandle(pDraw->hDC))->Draw3dRect(&pDraw->rcItem,
      //                          sh,hl);
        crOldBack = pDC->SetBkColor(theApp._color[0]);
        crOldText = pDC->SetTextColor(RGB(255,255,255));
    }
    else
    {
        //((CDC*)CDC::FromHandle(pDraw->hDC))->Draw3dRect(&pDraw->rcItem,
//                                hl,sh);
                       
        crOldText = pDC->SetTextColor(theApp._txtcolor);
        crOldBack = pDC->SetBkColor(theApp._color[1]);
    }

	//crOldBack = pDC->SetBkColor(theApp._color[1]);
	
    pDC->ExtTextOut(0, 0, ETO_OPAQUE, &pDraw->rcItem,tOut, 0, NULL );// fast then Fill Rect

    if(pDraw->itemData > 65535)
    {
        _TRY
        {
                SceItem* pItem = (SceItem*)pDraw->itemData;
                POINT pt = {pDraw->rcItem.left+4, pDraw->rcItem.top};
                _imageList.Draw(pDC, (int)pItem->_item, pt, ILD_NORMAL);

        }_CATCHX()
        {
        }
    }
    else if(pDraw->itemData>0&&pDraw->itemData<32)
    {
        POINT pt = {pDraw->rcItem.left+4, pDraw->rcItem.top};
        _imageList.Draw(pDC, (int)pDraw->itemData, pt, ILD_NORMAL);
    }
    pDC->TextOut(pDraw->rcItem.left+24,	
  			     pDraw->rcItem.top+2, 
 			     tOut,strlen(tOut));
    
    pDC->SetTextColor( crOldText );
    pDC->SetBkColor( crOldBack );
}

void CLboxItem::PreSubclassWindow() 
{
	CListBox::PreSubclassWindow();

    _imageList.Create( IDB_ITEM_TYPES, 16, 1, RGB(255, 255, 255) );
}

void CLboxItem::OnDestroy() 
{
	CListBox::OnDestroy();
	_imageList.DeleteImageList();
	
}
