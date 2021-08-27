//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z-edmap.h"
#include "ColorButon.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--|CColorButon::CColorButon|------------------------------------------------------------
CColorButon::CColorButon()
{
}

CColorButon::~CColorButon()
{
}


BEGIN_MESSAGE_MAP(CColorButon, CButton)
//{{AFX_MSG_MAP(CColorButon)
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--| CColorButon::DrawItem|--------------------------------------------------------------
void CColorButon::DrawItem(DRAWITEMSTRUCT* pDraw)
{
    CDC*        pdc = CDC::FromHandle(pDraw->hDC);
    POINT       pt={0};
    CString     text;
    CPen        *oldp=NULL;
    CPen        lp,dp;
    int         image = 0;
    int         offset = 0;
    COLORREF    lt = theApp._color[2];
    COLORREF    dk = theApp._color[0];
    CRect       drawr = pDraw->rcItem;

    drawr.InflateRect(-1,-1);
    
    if(_color == theApp._color[1])
    {
        lt = theApp._color[2];
        dk = theApp._color[0];
        lp.CreatePen(PS_SOLID,1,theApp._color[2]);
        dp.CreatePen(PS_SOLID,1,theApp._color[0]);
    }
    else
    {
        lt = _color;
        dk = theApp._color[0];
        lp.CreatePen(PS_SOLID,1,_color);
        dp.CreatePen(PS_SOLID,1,theApp._color[0]);
    }

    //pdc->FillRect(&drawr, &theApp._brush[1]);
    CBrush* pob = pdc->SelectObject(&theApp._brush[1]);

    if ( pDraw->itemState & ODS_SELECTED )
    {
        COLORREF temp = lt;
        lt            = dk;
        dk            = temp;
        
        oldp=(CPen*)pdc->SelectObject(&dp);
        pdc->RoundRect( &drawr, CPoint(8,8));

        ::OffsetRect(&drawr,1,1);
        pdc->SelectObject(&lp);
        pdc->RoundRect( &drawr, CPoint(8,8));
        ::OffsetRect(&drawr,-1,-1);

        pdc->SelectObject(oldp);
    }
    else
    {
        oldp=(CPen*)pdc->SelectObject(&lp);
        pdc->RoundRect( &drawr, CPoint(6,6));

        ::OffsetRect(&drawr,1,1);
        pdc->SelectObject(&dp);
        pdc->RoundRect( &drawr, CPoint(6,6));
        ::OffsetRect(&drawr,-1,-1);

        pdc->SelectObject(oldp);
    }
    
    CString cs;GetWindowText(cs);

    //
    //_butBmp.
    //pdc->FillRect(&drawr, &theApp._brush[1]);
    InflateRect(&drawr,1,1);
    pdc->SetBkColor(theApp._color[1]);

	if(IsWindowEnabled())
		pdc->SetTextColor(theApp._txtcolor);
	else
		pdc->SetTextColor(theApp._color[2]);

    int secidx = cs.Find('\n');
    drawr.top += 1;
    if(secidx!=-1)
    {
        CString left = cs.Left(secidx);
        m_tip = cs.Right(cs.GetLength()-secidx-1);
        cs = left;
    }
    if(cs[0]=='+'||cs[0]=='-')
    {
        drawr.left+=18;
        pdc->DrawText(cs.Right(cs.GetLength()-1), &drawr,DT_SINGLELINE|DT_VCENTER );
        drawr.left-=18;
        
        if(cs[0]=='+')
            pdc->DrawIcon(drawr.left+4,drawr.top+3,theApp._colexpic[0]);
        else
            pdc->DrawIcon(drawr.left+4,drawr.top+3,theApp._colexpic[1]);
        
    }
    else
    {
        pdc->DrawText(cs, &drawr,DT_CENTER|DT_SINGLELINE|DT_VCENTER );
    }
    pdc->SelectObject(pob);    
}


void CColorButon::PreSubclassWindow() //CHeaderCtrl
{
    _color = theApp._color[1];
    _brush.CreateSolidBrush(_color);
    CButton::PreSubclassWindow();
#ifdef _SKINNY
    ModifyStyle(0,BS_OWNERDRAW);
#endif //
    CRect rt;
    GetWindowRect(&rt);
    CString cst ; GetWindowText(cst);
    if(rt.Height() < 16)
    {
        this->SetWindowPos(&CWnd::wndTop, rt.left, rt.top, rt.Width(), 18, SWP_NOMOVE);
    }
}

void    CColorButon::SetColor(COLORREF    color)
{
    _brush.DeleteObject();
    _color = color;
    _brush.CreateSolidBrush(_color);

}

void CColorButon::OnDestroy() 
{
    _brush.DeleteObject();
	CButton::OnDestroy();
}

BOOL CColorButon::OnSetCursor(CWnd* pWnd, size_t nHitTest, size_t message) 
{
    if(m_tip)
        SBT(0,m_tip);
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}
    //pdc->Draw3dRect(&pDraw->rcItem,lt,dk);
    //pdc->SetBkColor(theApp._color[1]);

    // paint the bitmap
    /*
    {
        CDC		dcMemory;
        BITMAP  bmp;

        ::GetObject(theApp._butBmp, sizeof(BITMAP), &bmp);
        dcMemory.CreateCompatibleDC(pdc);
        CBitmap* ob = dcMemory.SelectObject(&theApp._butBmp);
        pdc->StretchBlt(1, 1,  
                        pDraw->rcItem.right-2,
                        bmp.bmHeight-2,
                        &dcMemory,
                        0,0,bmp.bmWidth, 
                        bmp.bmHeight, 
                        SRCCOPY);
        dcMemory.SelectObject(ob);
        dcMemory.DeleteDC();
    }
*/
