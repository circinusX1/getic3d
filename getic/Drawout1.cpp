// Drawout1.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "Drawout1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDrawOut*    CDrawOut::p_DrawBig = 0;

BEGIN_MESSAGE_MAP(CDrawOut, CWnd)
	//{{AFX_MSG_MAP(CDrawOut)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CDrawOut::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CDrawOut::PreSubclassWindow() 
{
   // ModifyStyle(0, BS_OWNERDRAW);
	CWnd::PreSubclassWindow();
}

void CDrawOut::OnLButtonUp(UINT nFlags, CPoint point) 
{
	_mmove._ldown = 0;
	CWnd::OnLButtonUp(nFlags, point);
}

void CDrawOut::OnLButtonDown(UINT nFlags, CPoint point) 
{
    _mmove._ptd   = point;
    _mmove._ldown = 1;
	CWnd::OnLButtonDown(nFlags, point);
}

void CDrawOut::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(_mmove._ldown)
    {
        _scroll.cx	= point.x - _mmove._ptd.x;
        _scroll.cy	= point.y - _mmove._ptd.y;

       if(_scroll.cx > _maxscroll.cx)
            _scroll.cx = _maxscroll.cx;
       if(_scroll.cy > _maxscroll.cy)
            _scroll.cy = _maxscroll.cy;

        if(_scroll.cx < -_maxscroll.cx)
            _scroll.cx = -_maxscroll.cx;
        if(_scroll.cy < -_maxscroll.cy)
            _scroll.cy = -_maxscroll.cy;

            


        Invalidate();
    }

	CWnd::OnMouseMove(nFlags, point);
}


void CDrawOut::Repaint(CDC* pDC)
{
    BOOL release = 0;
    if(0 == pDC)
    {
        release=1;
        pDC=GetDC();
    }
    
    CRect          rt ;
    GetClientRect(&rt);

    if(_th.Buffer())
        _pth=&_th;
    
    if(_pth && _pth->Buffer())
    {

        _scroll.cx = 0;
        _scroll.cy = 0;
        _maxscroll.cx = _pth->n_x - rt.right;
        _maxscroll.cy = _pth->n_y - rt.bottom;


     //   HDC              hdc    = lpDI->hDC;
        HDC              hMemDC = CreateCompatibleDC(pDC->m_hDC);
        BITMAPINFOHEADER bi     = {0};

        bi.biSize        = sizeof(BITMAPINFOHEADER);
        bi.biHeight      = _pth->n_y; 
        bi.biWidth       = _pth->n_x;
        bi.biPlanes      = 1;
        bi.biCompression =  BI_RGB;
        bi.biBitCount    =  24;
        if(_pth==&_th)
            _pth->SwapRB();
        HBITMAP hBitmap = CreateDIBitmap(pDC->m_hDC, 
                                         &bi, 
                                         CBM_INIT, 
                                         _pth->Buffer(), 
                                         (BITMAPINFO*)&bi, 
                                         DIB_RGB_COLORS);
        HBITMAP ob = (HBITMAP)SelectObject(hMemDC, hBitmap);
    /*
        BitBlt( dc.m_hDC,
                rt.left+_scroll.cx, 
                rt.top+_scroll.cy , 
                tmin(_th.n_x, rt.right), 
                tmin(_th.n_y, rt.bottom),
		        hMemDC,
                0,
                0,
                SRCCOPY);
    */

        StretchBlt( pDC->m_hDC,rt.left+1, 
                        rt.top + 1, 
                        rt.right-rt.left-1, 
					    rt.bottom-rt.top-2,
					    hMemDC,0,0,_pth->n_x ,_pth->n_y, SRCCOPY);

        ::SelectObject(hMemDC, ob);
        ::DeleteObject(hBitmap) ;
        ::DeleteDC(hMemDC);        

    }
    else if(_lmsb.pBuff)
    {
        HDC              hdc    = pDC->m_hDC;
        HDC              hMemDC = CreateCompatibleDC(hdc);
        BITMAPINFOHEADER bi     = {0};

        bi.biSize        = sizeof(BITMAPINFOHEADER);
        bi.biHeight      = _lmsb.sz.cy &  0xFFFF; 
        bi.biWidth       = _lmsb.sz.cx & 0xFFFF;
        bi.biPlanes      = 1;
        bi.biCompression = BI_RGB;
        bi.biBitCount    = 24;
        HBITMAP hBitmap = CreateDIBitmap(hdc,    
                                &bi, 
                                CBM_INIT, 
                                _lmsb.pBuff, 
                                (BITMAPINFO*)&bi, 
                                DIB_RGB_COLORS);
        HBITMAP ob = (HBITMAP)SelectObject(hMemDC, hBitmap);
        StretchBlt( hdc,rt.left+1, 
                    rt.top + 1, 
                    rt.right-rt.left-1, 
				    rt.bottom-rt.top-2,
				    hMemDC,0,0,_lmsb.sz.cx  & 0xFFFF, _lmsb.sz.cy  & 0xFFFF, SRCCOPY);        
        ::SelectObject(hMemDC, ob);
        ::DeleteObject(hBitmap) ;
        ::DeleteDC(hMemDC);
    }else
    {
        pDC->Rectangle(&rt);
    }

	if(release)
        ReleaseDC(pDC);

}

BOOL CDrawOut::CreateWnd(CWnd* parent, UINT isctrl)
{
    RECT rt={0,0,1,1};
    CWnd* hwPrev = 0;//
    if(isctrl)
    {
        hwPrev = parent->GetDlgItem(isctrl)->GetNextWindow(GW_HWNDPREV);
        parent->GetDlgItem(isctrl)->GetWindowRect(&rt);
        parent->ScreenToClient(&rt);
        parent->GetDlgItem(isctrl)->ShowWindow(SW_HIDE);
        parent->GetDlgItem(isctrl)->DestroyWindow();
    }
    BOOL b = Create("DRAW_DC_WIN", "", WS_CHILD|WS_VISIBLE, rt, parent, isctrl+9876);
    if(isctrl)
    {
	    SetDlgCtrlID(isctrl);
        SetWindowPos(hwPrev,rt.left,rt.top,rt.right,rt.bottom, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
    }
	return b;
}

void CDrawOut::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if(this == CDrawOut::p_DrawBig)
        return;

    if(!_th.Buffer())
        return;

    if(CDrawOut::p_DrawBig == 0)    
    {
        RECT rt= {0,0,_th.n_x,_th.n_y};
        CDrawOut::p_DrawBig = new CDrawOut();
        CDrawOut::p_DrawBig->Create("DRAW_DC_WIN","Texture Details",
                                    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                    rt,0,0);
    }
    if(CDrawOut::p_DrawBig && CDrawOut::p_DrawBig->m_hWnd) 
    {
        CDrawOut::p_DrawBig->MoveWindow(0,0,_th.n_x, _th.n_y);
        CDrawOut::p_DrawBig->_pth = &this->_th;
        CDrawOut::p_DrawBig->RedrawWindow();
    }
    else
    {
        delete CDrawOut::p_DrawBig;
        CDrawOut::p_DrawBig = 0;
    }


	CWnd::OnLButtonDblClk(nFlags, point);
}


void CDrawOut::OnDestroy() 
{
    if(this != CDrawOut::p_DrawBig && CDrawOut::p_DrawBig)
    {
        CDrawOut::p_DrawBig->DestroyWindow();
    }
	CWnd::OnDestroy();
}

void CDrawOut::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
    if(this == CDrawOut::p_DrawBig)
    {
        delete CDrawOut::p_DrawBig;
        CDrawOut::p_DrawBig=0;
    }

}

void CDrawOut::OnPaint() 
{
	CPaintDC dc(this); 
    Repaint(&dc);
}
