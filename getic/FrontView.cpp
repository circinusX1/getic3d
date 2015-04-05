// FrontView.cpp : implementation file
//

#include "stdafx.h"
#include "ZED.h"
#include "FrontView.h"
#include "ZEDDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrontView

IMPLEMENT_DYNCREATE(CFrontView, CView)

CFrontView::CFrontView()
{
}

CFrontView::~CFrontView()
{
}


BEGIN_MESSAGE_MAP(CFrontView, CView)
	//{{AFX_MSG_MAP(CFrontView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrontView drawing

void CFrontView::OnDraw(CDC* pDC)
{
	CZEDDoc* pDoc = (CZEDDoc*)GetDocument();

    


	pDC->TextOut(0,0,"Front View");
    pDoc->DrawFocusRect(this,pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CFrontView diagnostics

#ifdef _DEBUG
void CFrontView::AssertValid() const
{
	CView::AssertValid();
}

void CFrontView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrontView message handlers

void CFrontView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CView::OnLButtonDown(nFlags, point);
}

void CFrontView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	
	CView::OnLButtonUp(nFlags, point);
}

void CFrontView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CZEDDoc* pDoc = (CZEDDoc*)GetDocument();
	
	CView::OnMouseMove(nFlags, point);
}

BOOL CFrontView::PreCreateWindow(CREATESTRUCT& cs) 
{
    BOOL b = CView::PreCreateWindow(cs);
    return RegisterNewWindow(this, cs);
}
