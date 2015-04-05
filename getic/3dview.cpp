//----------------------------------------------------------------------------------------
//Proprietary and Confidential.  
//Unauthorized use or duplication is strictly prohibited.
//Copyright 2004.  All rights reserved, Zalsoft Inc.
////Changes:  AUTHOR              REASON                                                   DATE
//
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ZED.h"
#include "3DView.h"
#include "3DView.h"
#include "ZEDDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(C3DView, CView)

C3DView::C3DView()
{
}

C3DView::~C3DView()
{
}


BEGIN_MESSAGE_MAP(C3DView, CView)
//{{AFX_MSG_MAP(C3DView)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//--| C3DView::OnDraw|--------------------------------------------------------------------
//    asdf adsf adf asdf adsf adsf
void C3DView::OnDraw(CDC* pDC)
{
    CZEDDoc* pDoc = (CZEDDoc*)GetDocument();
    pDC->TextOut(0,0,"3D");
    
}


#ifdef _DEBUG
//--| C3DView::AssertValid|---------------------------------------------------------------
//    asdf asd
void C3DView::AssertValid() const
{
    CView::AssertValid();
}

//--| C3DView::Dump|----------------------------------------------------------------------
//    asdf ads
void C3DView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif //_DEBUG

