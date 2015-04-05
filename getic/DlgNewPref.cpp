// DlgNewPref.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "MainFrm.h"
#include "DlgNewPref.h"
#include "z_ed3View.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgNewPref dialog


DlgNewPref::DlgNewPref(CWnd* pParent /*=NULL*/)
: CBASEDLG(DlgNewPref::IDD, pParent),_pStaticGL(0)
{
	//{{AFX_DATA_INIT(DlgNewPref)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DlgNewPref::~DlgNewPref()
{
    delete _pStaticGL->_pBrush;
    _pStaticGL->_pBrush = 0;

}

void DlgNewPref::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgNewPref)
	DDX_Control(pDX, IDOK, m_o);
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, IDC_TREE1, _tree);
	DDX_Control(pDX, LB_PREFS, lb_prefs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgNewPref, CBASEDLG)
	//{{AFX_MSG_MAP(DlgNewPref)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(LB_PREFS, OnSelchangePrefs)
	ON_LBN_DBLCLK(LB_PREFS, OnDblclkPrefs)
	ON_BN_CLICKED(RB_CUT, OnCut)
	ON_BN_CLICKED(RB_CUTALL, OnCutall)
	ON_BN_CLICKED(CK_SETDETAIL, OnSplitable)
	ON_BN_CLICKED(RB_SOLID, OnSolid)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_SELCHANGED, OnTreeChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgNewPref message handlers

BOOL DlgNewPref::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();

    theApp.DoWaitCursor(1);
	
	_pStaticGL=FRM()->GetGLWidnow(this, IDC_STATICGL);
    _pStaticGL->_pBrush=0;
    _pStaticGL->EnableLighting(1);
    int count=0;

	REDIR();
	CDirChange cd(theApp.PrefsDir());
    SBT(0, theApp.PrefsDir());
    
    cd.UseObj();
    _curPath = cd.CurDir();
    _tree.PreSelectFolder(cd.CurDir());
	CheckDlgButton(CK_SETDETAIL, BST_CHECKED);

    CheckRadioButton(RB_SOLID, RB_CUTALL,  RB_SOLID);

	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	

 
	if((hfFind=FindFirstFile("*.gpm", &fDATA)) != INVALID_HANDLE_VALUE)
	{
		if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			lb_prefs.AddString(fDATA.cFileName);
            count++;
		
			while(FindNextFile(hfFind, &fDATA)!=0)
			{
				if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				{
					lb_prefs.AddString(fDATA.cFileName);
                    count++;
				}
			}
		}
	}
	FindClose(hfFind);  

    GetDlgItem(IDOK)->EnableWindow(_pStaticGL->_pBrush != 0);
	theApp.DoWaitCursor(0);
	return TRUE;  
}

LRESULT DlgNewPref::OnTreeChanged(WPARAM, LPARAM lp)
{

	REDIR();
	CDirChange cd((const char*)lp);

    SBT(0,cd.CurDir());
    _curPath = (const char*)cd.CurDir();
    cd.UseObj();

    

    lb_prefs.ResetContent();

    delete _pStaticGL->_pBrush;
    _pStaticGL->_pBrush = 0;
    _texNames.clear();


	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	
 
	if((hfFind=FindFirstFile("*.gpm", &fDATA)) != INVALID_HANDLE_VALUE)
	{
		if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			lb_prefs.AddString(fDATA.cFileName);
		
			while(FindNextFile(hfFind, &fDATA)!=0)
			{
				if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				{
					lb_prefs.AddString(fDATA.cFileName);
				}
			}
		}
	}
	FindClose(hfFind);  
    return 0l;
}

void DlgNewPref::OnDestroy() 
{
    _pStaticGL->EnableLighting(0);
    FRM()->GetGLWidnow(0,0);
	CBASEDLG::OnDestroy();
}

void DlgNewPref::OnOk() 
{
	
    if(_pStaticGL->_pBrush)
    {
        DOC()->_cachedSett._isSolid = 0;
        _pStaticGL->_pBrush->_brushflags = BRSH_NEW;

        if(IsDlgButtonChecked(RB_SOLID))
        {
            DOC()->_cachedSett._isSolid = 1;
            _pStaticGL->_pBrush->_brushflags |= BRSH_SOLID;
        }
        else if(IsDlgButtonChecked(RB_CUT))
        {
            _pStaticGL->_pBrush->_brushflags &= ~BRSH_SOLID;
        }
        else if(IsDlgButtonChecked(RB_CUTALL))
        {
            DOC()->_cachedSett._cutAll = 1;
            _pStaticGL->_pBrush->_brushflags &= ~BRSH_SOLID;
            _pStaticGL->_pBrush->_brushflags |= BRSH_CUTALL;
        }
        if(IsDlgButtonChecked(CK_REVERSE))
        {
            DOC()->_cachedSett._reversed = 1;
        }

        if(IsDlgButtonChecked(CK_SETDETAIL))
        {
            DOC()->_cachedSett._detail = 1;
            _pStaticGL->_pBrush->_brushflags |= BRSH_DETAIL;
        }
        _pStaticGL->_pBrush->_brushflags &= ~BRSH_ZONE;
		_pStaticGL->_pBrush->_brushflags &= ~BRSH_SPLITTER;
        
        DOC()->_cachedSett._zone = 0;
        DOC()->_cachedSett._detached = 0;
        CBASEDLG::OnOK();
        theApp.PrefsDir(_curPath.c_str());
    }else
    {
        AfxMessageBox("Cannot Load Prefabricated Brush", MB_OK|MB_ICONHAND);
    }
}

void DlgNewPref::OnCancel() 
{
    delete _pStaticGL->_pBrush;
    _pStaticGL->_pBrush = 0;
    _texNames.clear();

	CBASEDLG::OnCancel();
}

void DlgNewPref::OnPaint() 
{
	CPaintDC dc(this); 
    _pStaticGL->GLPaint();	
}

void DlgNewPref::OnSelchangePrefs() 
{
    char        szTexName[128]={0}    ;
    char       szFname[255];
    FileWrap    fw;

	REDIR();
    CDirChange  cd(_curPath.c_str());

    SBT(0, _curPath.c_str());

    delete _pStaticGL->_pBrush;
    _pStaticGL->_pBrush = 0;
    _pStaticGL->Invalidate();

    _texNames.clear();
    cd.UseObj();

    lb_prefs.GetText(lb_prefs.GetCurSel(),szFname);
    {
        if(!fw.Open(szFname,"rb"))
            return; 
    }
    
    fw.Read(szTexName,4);
    if(!strcmp(szTexName,GeticPrimitiveFile))
    {
        fw.Close();
        GdiFlush();
        Sleep(2);
            MakeCurrent(_pStaticGL->_hdc, _pStaticGL->m_hRC);
            DOC()->LoadPrefFile(szFname, &_pStaticGL->_pBrush);
            MakeCurrent(_pStaticGL->_hdc, _pStaticGL->m_hRC);
    }
	fw.Close();
    GetDlgItem(IDOK)->EnableWindow(_pStaticGL->_pBrush != 0);
    _pStaticGL->Invalidate();
	REDIR();
}

void DlgNewPref::OnDblclkPrefs() 
{
    OnOk();
}

void DlgNewPref::OnCut() 
{
    if(IsDlgButtonChecked(RB_CUT))
    {
        CheckDlgButton(CK_SETDETAIL, BST_UNCHECKED);
        GetDlgItem(CK_SETDETAIL)->EnableWindow(0);
    }
    else
    {
        GetDlgItem(CK_SETDETAIL)->EnableWindow(1);
    }
}

void DlgNewPref::OnCutall() 
{
    if(IsDlgButtonChecked(RB_CUTALL))
    {
        CheckDlgButton(CK_SETDETAIL, BST_UNCHECKED);
        GetDlgItem(CK_SETDETAIL)->EnableWindow(0);
    }
    else
    {
        GetDlgItem(CK_SETDETAIL)->EnableWindow(1);
    }

}

void DlgNewPref::OnSplitable() 
{
	if(IsDlgButtonChecked(CK_SETDETAIL))
    {
        GetDlgItem(RB_CUTALL)->EnableWindow(0);
        GetDlgItem(RB_CUT)->EnableWindow(0);
    }
    else
    {
        GetDlgItem(RB_CUTALL)->EnableWindow(1);
        GetDlgItem(RB_CUT)->EnableWindow(1);
    }
}

void DlgNewPref::OnSolid() 
{
    if(IsDlgButtonChecked(RB_SOLID))
    {
        GetDlgItem(CK_SETDETAIL)->EnableWindow(1);
    }
    else
    {
        GetDlgItem(CK_SETDETAIL)->EnableWindow(0);
    }
}
