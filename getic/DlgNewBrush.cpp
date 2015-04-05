// DlgNewBrush.cpp : implementation file
//
#pragma warning (disable: 4786)
#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "DlgNewBrush.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
// CDlgNewBrush dialog


CDlgNewBrush::CDlgNewBrush(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgNewBrush::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewBrush)
	//}}AFX_DATA_INIT
    _hideZone=FALSE;
}


void CDlgNewBrush::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewBrush)
	DDX_Control(pDX, IDOK, m_o);
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, IDC_SPIN3, m_splr);
	DDX_Control(pDX, IDC_SPIN2, m_splv);
	DDX_Control(pDX, IDC_EDIT3, m_radstrips);
	DDX_Control(pDX, IDC_EDIT2, m_vstrips);
	DDX_Control(pDX, IDC_EDIT1, m_thikness);
    DDX_Control(pDX, EF_HEIGHT, _height);
    DDX_Control(pDX, EF_DEPTH, _thick);
	DDX_Control(pDX, EF_WIDTH, _width);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewBrush, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgNewBrush)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(CK_SHELL, OnShell)
	ON_BN_CLICKED(CK_SETDETAIL, OnSplitable)
	ON_BN_CLICKED(CK_SKYDOM, OnSkydom)
	ON_BN_CLICKED(CK_REVERSE, OnReverse)
	ON_BN_CLICKED(CK_SPLITTER, OnSplitter)
	ON_BN_CLICKED(CK_ZONE, OnZone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
// CDlgNewBrush message handlers

BOOL CDlgNewBrush::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();

    _brshSett._isSolid    = 1;
	_brshSett._cutAll     = 0;
    _brshSett._skyDom     = 0;
    _brshSett._splitter   = 0;
    _brshSett._zone       = 0;
    _brshSett._detached   = 0;
    _brshSett._zone       = 0;
    _brshSett._detail    = 0;
    _brshSett._reversed  = 0;

    

    DOC()->_cachedSett._detached = 0;
	DOC()->_cachedSett._zone = 0;


    m_thikness.SetLimitText(4);
    m_radstrips.SetLimitText(3);
    m_vstrips.SetLimitText(3);

    CheckRadioButton(RB_SOLID, RB_CUTALL, RB_SOLID);
    CheckRadioButton(RB_LINEAR, RB_ENVI, RB_LINEAR);

    _brshSett._radstrips=_brshSett._vstrips=16;
    SetDlgItemInt(IDC_EDIT2, _brshSett._vstrips);
    SetDlgItemInt(IDC_EDIT3, _brshSett._radstrips);

	if(_brshSett._radstrips==-1)
	{
		m_vstrips.EnableWindow(0);
		GetDlgItem(IDC_STATICRS)->EnableWindow(0);
		SetDlgItemInt(IDC_STATICRS,0);
	}
	else
		SetDlgItemInt(IDC_EDIT1,_brshSett._radstrips);

	if(_brshSett._vstrips==-1)
	{
		m_radstrips.EnableWindow(0);
		GetDlgItem(IDC_STATICVS)->EnableWindow(0);
	}
	else
		SetDlgItemInt(IDC_EDIT2,_brshSett._vstrips);

    _brshSett._thikness = 0;
	if(_brshSett._thikness)
	{
		Sct(this,IDC_EDIT1,_brshSett._thikness/GMeters);
		CheckDlgButton(CK_SHELL,BST_CHECKED);
	}

	if(_brshSett._boxdim.x<0)
		_brshSett._boxdim.x=1000;
	if(_brshSett._boxdim.y<0)
		_brshSett._boxdim.y=1000;
	if(_brshSett._boxdim.z<0)
		_brshSett._boxdim.z=1000;

	_width.SetWindowText(MKSTR("%4.2f" ,_brshSett._boxdim.x/GMeters));
    _height.SetWindowText(MKSTR("%4.2f" ,_brshSett._boxdim.y/GMeters));
    _thick.SetWindowText(MKSTR("%4.2f" ,_brshSett._boxdim.z/GMeters));
    

	OnShell() ;
    OnReverse();
    OnSkydom();

    PBrushes brushes;

    int countzones=0;
    FOREACH(PBrushes, (*DOC()->_scene.GetPrimitives()), ppb)
    {
        if((*ppb)->IsZone())
            ++countzones;
    }
    if(countzones > 31)
        _hideZone=1;

    if(_hideZone)
    {
       GetDlgItem(CK_ZONE)->EnableWindow(0);
       CheckDlgButton(CK_ZONE, BST_UNCHECKED);
    }
    return TRUE;  
}

void CDlgNewBrush::OnDestroy() 
{
	CBASEDLG::OnDestroy();
}


void CDlgNewBrush::OnOk() 
{
    _brshSett._isSolid    = IsDlgButtonChecked(RB_SOLID);
	_brshSett._cutAll     = IsDlgButtonChecked(RB_CUTALL);
    _brshSett._skyDom     = IsDlgButtonChecked(CK_SKYDOM);
    _brshSett._splitter   = IsDlgButtonChecked(CK_SPLITTER);
    _brshSett._zone       = IsDlgButtonChecked(CK_ZONE);
    _brshSett._detached   = IsDlgButtonChecked(CK_DETACHED);

   
	if(IsDlgButtonChecked(CK_SHELL))
	{
		_brshSett._thikness = ParseREAL(this,IDC_EDIT1);
	}
	else
	{
		_brshSett._thikness=0;
	}

    _brshSett._detail    = IsDlgButtonChecked(CK_SETDETAIL);
    _brshSett._reversed  = IsDlgButtonChecked(CK_REVERSE);
    _brshSett._vstrips   = GetDlgItemInt(IDC_EDIT2);
    _brshSett._radstrips = GetDlgItemInt(IDC_EDIT3);

    if(IsDlgButtonChecked(RB_LINEAR))
    {
        _brshSett._tm = TM_LIN;
    }
    else if(IsDlgButtonChecked(RB_CYLINDRIC))
    {
        _brshSett._tm = TM_CYL;
    }
    else
        _brshSett._tm = TM_SPH;

    V3 prevVal = _brshSett._boxdim;

    _brshSett._boxdim.x = ParseREAL(this, EF_WIDTH);
    if(0 >= _brshSett._boxdim.x)
        _brshSett._boxdim.x = prevVal.x;

    _brshSett._boxdim.y = ParseREAL(this, EF_HEIGHT);
    if(0 >= _brshSett._boxdim.y)
        _brshSett._boxdim.y = prevVal.y;

    _brshSett._boxdim.z = ParseREAL(this, EF_DEPTH);
    if(0 >= _brshSett._boxdim.z)
        _brshSett._boxdim.z = prevVal.z;

    _brshSett._boxdim *= GMeters;

    if(IsDlgButtonChecked(CK_ZONE))
        _brshSett._thikness=0;
	_brshSett._thikness *= GMeters;

    EndDialog(IDOK);	
}

void CDlgNewBrush::OnCancel() 
{
	CBASEDLG::OnCancel();
}

void CDlgNewBrush::OnShell() 
{
    BOOL b = IsDlgButtonChecked(CK_SHELL);
    //GetDlgItem(IDC_STATICT)->EnableWindow(b);
    m_thikness.EnableWindow(b);

    if(_brshSett._thikness == 0)
        _brshSett._thikness = 64;
	Sct(this,IDC_EDIT1,_brshSett._thikness/GMeters);
} 

void CDlgNewBrush::OnAutodetail() 
{
//  BOOL b = IsDlgButtonChecked(CK_AUTODETAIL);
//  GetDlgItem(CK_SETDETAIL)->EnableWindow(!b);
}

void CDlgNewBrush::OnSplitable() 
{
	BOOL b = IsDlgButtonChecked(CK_SETDETAIL);
	if(b)
	{
		CheckRadioButton(RB_SOLID,RB_CUTALL,RB_SOLID);
	}
    else
    {
        CheckDlgButton(CK_DETACHED,BST_UNCHECKED);
    }
    GetDlgItem(CK_DETACHED)->EnableWindow(b);    
	GetDlgItem(RB_CUT)->EnableWindow(!b);
	GetDlgItem(RB_CUTALL)->EnableWindow(!b);
    GetDlgItem(CK_SKYDOM)->EnableWindow(!b);
    
    
}

void CDlgNewBrush::OnSkydom() 
{
	BOOL b = IsDlgButtonChecked(CK_SKYDOM);
	if(b)
	{
		CheckRadioButton(RB_SOLID,RB_CUTALL,RB_SOLID);

        CheckDlgButton(CK_SETDETAIL,BST_UNCHECKED);
        OnSplitable() ;
	}
	GetDlgItem(RB_CUT)->EnableWindow(!b);
	GetDlgItem(RB_CUTALL)->EnableWindow(!b);
    GetDlgItem(CK_SETDETAIL)->EnableWindow(!b);
}

void CDlgNewBrush::OnReverse() 
{
    /*
    BOOL b = IsDlgButtonChecked(CK_REVERSE);
    if(b)
    {
        GetDlgItem(CK_SKYDOM)->EnableWindow(1);
    }
    else
    {
        GetDlgItem(CK_SKYDOM)->EnableWindow(0);
        CheckDlgButton(CK_SKYDOM,BST_UNCHECKED);
        OnSkydom() ;
    }
    */
}

void CDlgNewBrush::OnSplitter() 
{
    BOOL b = IsDlgButtonChecked(CK_SPLITTER);

    CheckDlgButton(CK_ZONE, BST_UNCHECKED);
    GetDlgItem(CK_ZONE)->EnableWindow(!b);
    GetDlgItem(RB_CUT)->EnableWindow(!b);
	GetDlgItem(RB_CUTALL)->EnableWindow(!b);
    GetDlgItem(CK_SETDETAIL)->EnableWindow(!b);
    
}

void CDlgNewBrush::OnZone() 
{
    BOOL b = IsDlgButtonChecked(CK_ZONE);

    CheckDlgButton(CK_SPLITTER, BST_UNCHECKED);
    GetDlgItem(CK_SPLITTER)->EnableWindow(!b);
    GetDlgItem(RB_CUT)->EnableWindow(!b);
	GetDlgItem(RB_CUTALL)->EnableWindow(!b);
    GetDlgItem(CK_SETDETAIL)->EnableWindow(!b);
	
}
