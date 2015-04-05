// Frame2.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "Frame2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrame2

IMPLEMENT_DYNCREATE(CFrame2, CMiniFrameWnd)

CFrame2::CFrame2()
{
}

CFrame2::~CFrame2()
{
}


BEGIN_MESSAGE_MAP(CFrame2, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CFrame2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrame2 message handlers

void CFrame2::PreSubclassWindow() 
{
    

	CMiniFrameWnd::PreSubclassWindow();
}
