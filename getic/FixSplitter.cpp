// FixSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "MainFrm.h"
#include "FixSplitter.h"
#include "z-edmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define         BARWIDTH    243
 
CFixSplitter::CFixSplitter(int dim)
{
    
    m_cxSplitter    = dim;     // = 1
    m_cySplitter    = dim;     // = 1
    m_cxBorderShare = 0;   // = 0
    m_cyBorderShare = 0;   // = 0
    m_cxSplitterGap = dim;     // = 1
    m_cySplitterGap = dim;     // = 1
}


CFixSplitter::~CFixSplitter()
{
}


BEGIN_MESSAGE_MAP(CFixSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CFixSplitter)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//#include  "afxext.h"
//#include <..\\src\\AFXIMPL.H>
//#include <..\\src\\mfc\\AFXIMPL.H>

void CFixSplitter::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	
//	afxData.clrBtnFace  =  theApp._color[0];
//	afxData.clrBtnHilite = theApp._color[1];
//	afxData.clrBtnShadow = theApp._color[0];	
	
	CSplitterWnd::OnPaint();
}

void CFixSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
    /*
    if(GetDlgCtrlID() == AFX_IDW_PANE_FIRST)
    {
        CWnd::OnLButtonDown(nFlags, point);
    }
    else
    */
    CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CFixSplitter::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CSplitterWnd::OnLButtonDblClk(nFlags, point);

    if(GetDlgCtrlID() == AFX_IDW_PANE_FIRST)
	{
		RECT rt;
        int  cc,cm;


        GetColumnInfo(0, cc, cm);

        if(cc<=4)
        {
		    GetClientRect(&rt);
	        rt.right  -=BARWIDTH;
		    rt.bottom -=32;
            LONG two = 2;
			rt.right=tmax(rt.right,two);
			rt.bottom=tmax(rt.bottom,two);

		    SetColumnInfo(0, BARWIDTH, 0);

		    FRM()->m_wndSplitter.SetColumnInfo(0, rt.right/2, 0);
		    FRM()->m_wndSplitter.SetColumnInfo(1, rt.right/2, 0);

		    FRM()->m_wndSplitter.SetRowInfo(0, rt.bottom/2, 0);
		    FRM()->m_wndSplitter.SetRowInfo(1, rt.bottom/2, 0);

		    RecalcLayout();
		    FRM()->m_wndSplitter.RecalcLayout();
            FRM()->_dlgBarHidden = 0;
        }
        else
        {
    		GetClientRect(&rt);
	        rt.right  -=8;
		    rt.bottom -=32;
            LONG two = 2;
			rt.right=tmax(rt.right,two);
			rt.bottom=tmax(rt.bottom,two);

		    SetColumnInfo(0, 2, 0);

		    FRM()->m_wndSplitter.SetColumnInfo(0, rt.right/2, 0);
		    FRM()->m_wndSplitter.SetColumnInfo(1, rt.right/2, 0);

		    FRM()->m_wndSplitter.SetRowInfo(0, rt.bottom/2, 0);
		    FRM()->m_wndSplitter.SetRowInfo(1, rt.bottom/2, 0);

		    RecalcLayout();
		    FRM()->m_wndSplitter.RecalcLayout();
            FRM()->_dlgBarHidden = 1;
        }

	}

}

BOOL CFixSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}

int CFixSplitter::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    _hlr[1] = AfxGetApp()->LoadCursor(IDR_R2LSPL);
    _hlr[0] = AfxGetApp()->LoadCursor(IDR_L2RSPL);
	return 0;
}

void CFixSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(GetDlgCtrlID() == AFX_IDW_PANE_FIRST )
    {
        int  cc,cm;//HTERROR 

        GetColumnInfo(0, cc, cm);
        if(cc<=4)
        {
            SetCursor(_hlr[0]);
        }
        else
        {
            SetCursor(_hlr[1]);
        }
        CWnd::OnMouseMove(nFlags, point);
        return;
    }	
	CSplitterWnd::OnMouseMove(nFlags, point);
}
