// MyStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "MyStatusBar.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define SB_PREV_CMD       (_PLUG_MSG0+300)
#define SB_NEXT_CMD       (SB_PREV_CMD+1)


#define SB_ROT_30         (SB_PREV_CMD+2)
#define SB_ROT_45         (SB_PREV_CMD+3)
#define SB_ROT_60         (SB_PREV_CMD+4)
#define SB_AROT_30        (SB_PREV_CMD+5)
#define SB_AROT_45        (SB_PREV_CMD+6)
#define SB_AROT_60        (SB_PREV_CMD+7)

#define SB_LAST_CMD       (SB_PREV_CMD+32)

#define SB_BACKUP_OPEN    (SB_PREV_CMD+64)

CMyStatusBar::CMyStatusBar()
{
}

CMyStatusBar::~CMyStatusBar()
{
}


BEGIN_MESSAGE_MAP(CMyStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CMyStatusBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
int CMyStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void    CMyStatusBar::SetSelection(int selEntities){}