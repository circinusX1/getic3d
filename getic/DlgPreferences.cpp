// DlgPreferences.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgPreferences dialog


DlgPreferences::DlgPreferences(CWnd* pParent /*=NULL*/)
	: CDialog(DlgPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgPreferences)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgPreferences::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgPreferences)
	DDX_Control(pDX, PB_TRIGGERBRSH, m_11);
	DDX_Control(pDX, PB_TRIGGER, m_10);
	DDX_Control(pDX, PB_TERRAIN, m_9);
	DDX_Control(pDX, PB_SELECTED, m_8);
	DDX_Control(pDX, PB_SELBOX, m_7);
	DDX_Control(pDX, PB_PROJBRSH, m_6);
	DDX_Control(pDX, PB_PORTALS2, m_5);
	DDX_Control(pDX, PB_PORTALS1, m_4);
	DDX_Control(pDX, PB_MOTION, m_3);
	DDX_Control(pDX, PB_DETAIL, m_2);
	DDX_Control(pDX, PB_CUT, m_1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgPreferences, CDialog)
	//{{AFX_MSG_MAP(DlgPreferences)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgPreferences message handlers

