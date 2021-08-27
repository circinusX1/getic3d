// TexLbox.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgTextures.h"
#include "Texlbox.h"
#include "geticgui.h"
#include "DlgResBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define LEFT_MARG	6


BEGIN_MESSAGE_MAP(CTexLbox, CListBox)
	//{{AFX_MSG_MAP(CTexLbox)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
    ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTexLbox message handlers

void CTexLbox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CListBox::PreSubclassWindow();

	_errorPen.CreatePen(PS_SOLID,3,RGB(255,0,0));
    _texkind[0].LoadBitmap(BMP_2D_TEX);
    _texkind[1].LoadBitmap(BMP_3D_TEX);
    _texkind[2].LoadBitmap(BMP_CUBE_TEX);
	_curSel = -1;
    if(_pParent==0)
        _pParent=(CarryTex*)GetParent();
}

void CTexLbox::DrawItem(LPDRAWITEMSTRUCT pDraw) 
{
    RECT&   rpl = pDraw->rcItem;
    CDC*    pDC = CDC::FromHandle(pDraw->hDC);
	char	exTra[128]={0};
	char	tOut[64]={0};

	GetText(pDraw->itemID ,tOut);

    if (pDraw->itemState & ODS_SELECTED ) 
    {
        pDC->SetBkColor(RGB(255,255,255));//theApp._color[2]);
    }
    else
    {
        pDC->SetBkColor(theApp._color[2]);
    }
    
    pDC->ExtTextOut(0,0,ETO_OPAQUE, &pDraw->rcItem,"", 0, NULL );

    const char*    pszTn = 0;
    try{
        do{
        TexHandler      th;
        pszTn = _pParent->GetTextureFullName(pDraw->itemID);
        if(_invalides.findelement(string(pszTn))==_invalides.end()
            && th.LoadThisFile(pszTn,0))
        {
            HDC              hdc    = pDraw->hDC;
            HDC              hMemDC = CreateCompatibleDC(hdc);
            BITMAPINFOHEADER bi     = {0};

            bi.biSize        = sizeof(BITMAPINFOHEADER);
            bi.biHeight      = th.n_y; 
            bi.biWidth       = th.n_x;
            bi.biPlanes      = 1;
            bi.biCompression = BI_RGB;
            bi.biBitCount    = 24;
            th.SwapRB();
            HBITMAP hBitmap = CreateDIBitmap(hdc, 
                                             &bi, 
                                             CBM_INIT, 
                                             th.Buffer(), 
                                             (BITMAPINFO*)&bi, 
                                             DIB_RGB_COLORS);
            HBITMAP ob = (HBITMAP)SelectObject(hMemDC, hBitmap);
            if(_bitblt)
            {
                BitBlt(hdc,0,0,th.n_x ,th.n_y, hMemDC,0,0,SRCCOPY);
            }
            else
            {
                StretchBlt( hdc,rpl.left+LEFT_MARG, 
                            rpl.top + LEFT_MARG, 
                            rpl.right-rpl.left-LEFT_MARG*2, 
					        rpl.bottom-rpl.top-LEFT_MARG*2,
					        hMemDC,0,0,th.n_x ,th.n_y, SRCCOPY);
            }


            ::SelectObject(hMemDC, ob);
            ::DeleteObject(hBitmap) ;
            ::DeleteDC(hMemDC);

            _pParent->_bpp   = th.n_bpp;
            _pParent->_sz.cx = th.n_x;
            _pParent->_sz.cy = th.n_y;
            _pParent->_name  = pszTn;

            if(pDraw->itemID!=-1 && GetItemData(pDraw->itemID) == 0)
            {
                SetItemData(pDraw->itemID, MAKELONG(th.n_x, th.n_y));
            }

       		    
            pDC->DrawText(tOut, strlen(tOut), &pDraw->rcItem, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
        }
	    else
	    {
		    if(pDraw->itemID != -1)
		    {
			    sprintf(exTra, _T("Cannot Load '%s'"),tOut);
			    pDC->DrawText(exTra,strlen(exTra),&pDraw->rcItem, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
                _invalides << pszTn;
		    }
        }



	    if(((th.n_y & (th.n_y-1))!= 0)|| ((th.n_x & (th.n_x-1))!=0))
	    {
		    CPen* pO = (CPen*)pDC->SelectObject(&_errorPen);
		    pDC->MoveTo(CPoint(rpl.right-8,rpl.bottom-8));
		    pDC->LineTo(CPoint(rpl.right-2,rpl.bottom-2));
		    pDC->MoveTo(CPoint(rpl.right-8,rpl.bottom-2));
		    pDC->LineTo(CPoint(rpl.right-2,rpl.bottom-8));
		    pDC->SelectObject(pO);
	    }

        InflateRect(&pDraw->rcItem,-1,-1);
        pDC->Draw3dRect(&pDraw->rcItem, theApp._color[1],theApp._color[2]);
        th.Reset();
	    _curSel = pDraw->itemID;
        }while(0);
    }catch(...)
    {
        PostMessage(LB_DELETESTRING,pDraw->itemID,0);
        if(pszTn)
        {
            _invalides << pszTn;
        }
    }
}

void CTexLbox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
    if(_bitblt)
    {
        if(lpMeasureItemStruct->itemData==0)
        {
            lpMeasureItemStruct->itemHeight = 400;
        }
        else
        {
            lpMeasureItemStruct->itemHeight = HIWORD(lpMeasureItemStruct->itemData);
            lpMeasureItemStruct->itemWidth  = LOWORD(lpMeasureItemStruct->itemData);
        }
    }
    else
    {
        RECT rt; GetClientRect(&rt);
        lpMeasureItemStruct->itemHeight = rt.bottom-16;
    }
}

void CTexLbox::OnRButtonDown(size_t nFlags, CPoint point) 
{
    
    DlgResBrowser    d;   
    d.DoModal();


	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CTexLbox::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CListBox::OnCommand(wParam, lParam);
}

void    CTexLbox::DrawBmp(HDC hDc, CBitmap* pBmp, int x, int y, int slice)
{
    HDC     hMemDC  = CreateCompatibleDC(hDc);
    HBITMAP ob      = (HBITMAP)SelectObject(hMemDC, pBmp->m_hObject);

    ::BitBlt( hDc, x, y, LEFT_MARG-1, LEFT_MARG-1, hMemDC,x + (LEFT_MARG-1)*slice,0,SRCCOPY);
    ::SelectObject(hMemDC, ob);
    ::DeleteDC(hMemDC);
}

void CTexLbox::OnLButtonDown(size_t nFlags, CPoint point) 
{
    /*
	if(point.x<(LEFT_MARG-1))
    {
        char       szit[128];
        int         cs = GetCurSel();
        RECT        rt;
        
        this->GetItemRect(cs, &rt);
        this->GetText(cs, szit);
        Texture* pTex = GTexSys.GetTempTexture(szit);
        point.y-=rt.top;

        if(point.y < 25+(LEFT_MARG-1) && point.y > 25 )
        {
            SetItemData(cs, 0);
            if(pTex)
                pTex->createFlags = GEN_TEX_2D_MAP;
            InvalidateRect(&rt);
        }
        else if(point.y < 55+(LEFT_MARG-1) && point.y > 55 )
        {
            SetItemData(cs, 1);
            if(pTex)
                pTex->createFlags = GEN_TEX_3D_MAP;
            InvalidateRect(&rt);
        }
        else if(point.y < 85+(LEFT_MARG-1) && point.y > 85 )
        {
            SetItemData(cs, 2);
            if(pTex)
                pTex->createFlags = GEN_TEX_CUBE_MAP  ;  
            InvalidateRect(&rt);
        }
        else
        {
            point.y+=rt.top;
            OnRButtonDown(nFlags, point);
        }
    }
    */
	CListBox::OnLButtonDown(nFlags, point);
}




void CTexLbox::OnSelchange() 
{
    /*
    int sel = GetCurSel();	
    RECT rt; this->GetItemRect(sel, &rt);
    if(rt.top > 20)
        SetTopIndex(sel);
        */

}


