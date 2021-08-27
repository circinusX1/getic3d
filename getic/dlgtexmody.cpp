// DlgTexMody.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgTexMody.h"
#include "DlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgTexMody dialog
static DlgTexMody* __pDlgTex;

DlgTexMody::DlgTexMody(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTexMody::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgTexMody)
	//}}AFX_DATA_INIT
    __pDlgTex = 0;
}


void DlgTexMody::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgTexMody)
	DDX_Control(pDX, ST_PLACE, m_static);
	DDX_Control(pDX, SL_INTENS, m_i);
	DDX_Control(pDX, SL_GAMMA, m_g);
	DDX_Control(pDX, SL_LUMIN, m_l);
	DDX_Control(pDX, SL_CONTRAST, m_c);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgTexMody, CDialog)
	//{{AFX_MSG_MAP(DlgTexMody)
	ON_BN_CLICKED(PB_SAVE, OnSave)
	ON_BN_CLICKED(PB_SAVEAS, OnSaveas)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(CK_BW, OnBw)
	ON_BN_CLICKED(CK_BW2, OnBw2)
	ON_BN_CLICKED(CK_MASK, OnMask)
	ON_BN_CLICKED(CK_NEGATIVE, OnNegative)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(PB_RELOAD, OnReload)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgTexMody message handlers

void DlgTexMody::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DlgTexMody::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DlgTexMody::OnSave() 
{
	
	
}

void DlgTexMody::OnSaveas() 
{
	
	
}

BOOL DlgTexMody::OnInitDialog() 
{
	CDialog::OnInitDialog();
    TexHandler      th;

    m_g.SetRange(-32,32);
	m_l.SetRange(-32,32);
	m_c.SetRange(8,20);
    m_i.SetRange(1,128);

    m_g.SetPos(0);
    m_l.SetPos(0);
    m_c.SetPos(1);
    m_i.SetPos(1);

    _comp = -1;
    memset(_gamma,0,sizeof(_gamma));
    memset(_luminance,0,sizeof(_luminance));
    memset(_copntrast,1,sizeof(_copntrast));

    const char*    pszTn = BARDLG()->_dlgtex.GetTextureFullName(-1);
    _loaded = m_static._th.LoadThisFile(pszTn,0);

    __pDlgTex = this;
    return 1;
}
    

void DlgTexMody::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void DlgTexMody::Paint(CPaintDC& dc) 
{
    
    

}

void DlgTexMody::OnDestroy() 
{
	CDialog::OnDestroy();
	
}


void DlgTexMody::OnBw() 
{
	if(IsDlgButtonChecked(CK_BW))
	{
		m_static._th.MakeBW();
		Invalidate();
	}
	else
		OnReload();
    
}

void DlgTexMody::OnBw2() //dot 3
{
	if(IsDlgButtonChecked(CK_BW2))
	{
		m_static._th.MakeDot3(m_i.GetPos());
		Invalidate();
	}
	else
		OnReload();
	
}

void DlgTexMody::OnMask() 
{
	
}

void DlgTexMody::OnNegative() 
{
	
}

void DlgTexMody::OnRadio1() 
{
//r
    _comp=0;
}

void DlgTexMody::OnRadio2() 
{
//g	
    _comp=1;
}

void DlgTexMody::OnRadio3() 
{
//b	
    _comp=2;
}

void DlgTexMody::OnRadio4() 
{
//rgb	
    _comp=-1;
}

void DlgTexMody::OnHScroll(size_t nSBCode, size_t nPos, CScrollBar* pScrollBar) 
{

    
    
    
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void DlgTexMody::OnReload() 
{
    m_static._th.Reset();
    const char*    pszTn = BARDLG()->_dlgtex.GetTextureFullName(-1);
    _loaded = m_static._th.LoadThisFile(pszTn,0);
	Invalidate();
}

