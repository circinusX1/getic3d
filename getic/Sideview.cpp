// SideView.cpp : implementation file
//

#include "stdafx.h"
#include "ZED.h"
#include "SideView.h"
#include "ZEDDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSideView

IMPLEMENT_DYNCREATE(CSideView, CView)

CSideView::CSideView()
{
}

CSideView::~CSideView()
{
}


BEGIN_MESSAGE_MAP(CSideView, CView)
	//{{AFX_MSG_MAP(CSideView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSideView drawing

void CSideView::OnDraw(CDC* pDC)
{
	CZEDDoc* pDoc = (CZEDDoc*)GetDocument();
	pDC->TextOut(0,0,"Side View");
    pDoc->DrawFocusRect(this,pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CSideView diagnostics

#ifdef _DEBUG
void CSideView::AssertValid() const
{
	CView::AssertValid();
}

void CSideView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSideView message handlers

BOOL CSideView::PreCreateWindow(CREATESTRUCT& cs) 
{
    BOOL b = CView::PreCreateWindow(cs);
    return RegisterNewWindow(this, cs);
}
