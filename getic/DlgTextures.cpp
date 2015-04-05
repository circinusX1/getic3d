// DlgTextures.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgTextures.h"
#include "texsys.h"
#include "MainFrm.h"
#include "z_ed2Doc.h"
#include "basecont.h"
#include "DlgBar.h"
#include "Scene.h"
#include "geticgui.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CDlgTextures::CDlgTextures(CWnd* pParent /*=NULL*/)
	: CVertDlgDlg(CDlgTextures::IDD, pParent),_texIter(-1)
{
	//{{AFX_DATA_INIT(CDlgTextures)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _paneNo = 0;
    _bpp = 0;
    _sz.cx=_sz.cy=0;
    _name = "*";
    _initalized = FALSE;
	_edChanged  = FALSE;
    _dirty      = FALSE;  
    _bSkyDom    = FALSE;
	_showPick   = TRUE;
    _barNoty    = TRUE;
    _resetTex   = FALSE;
}

void CDlgTextures::Dirty()
{
	//BARDLG()->Dirty();
	//_dirty=1;
}

void CDlgTextures::DoDataExchange(CDataExchange* pDX)
{
	CVertDlgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTextures)
	DDX_Control(pDX, IDC_TAB1, _tabdirs);
	DDX_Control(pDX, PB_DELTEX, m_dt);
	DDX_Control(pDX, PB_SELTBT, m_sss);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	
	DDX_Control(pDX, IDC_LIST1, m_lf);
	DDX_Control(pDX, IDC_EDIT5, m_escalex);
	DDX_Control(pDX, IDC_EDIT4, m_escaley);
	DDX_Control(pDX, IDC_EDIT3, m_eshiftx);
	DDX_Control(pDX, IDC_EDIT2, m_eshifty);
	DDX_Control(pDX, IDC_SPIN4, m_scalex);
	DDX_Control(pDX, IDC_SPIN3, m_scaley);
	DDX_Control(pDX, IDC_SPIN2, m_shiftx);
	DDX_Control(pDX, IDC_SPIN1, m_shifty);
	DDX_Control(pDX, ST_BPP, m_bpp);
	DDX_Control(pDX, ST_SIZE, m_size);
	DDX_Control(pDX, ST_NAME, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTextures, CVertDlgDlg)
	//{{AFX_MSG_MAP(CDlgTextures)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpinShiftV)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpinShiftH)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpinScaleV)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, OnDeltaposSpinScaleH)
	ON_BN_CLICKED(PB_ROTATE, OnRotate)
	ON_BN_CLICKED(PB_FLIPH, OnFliph)
	ON_BN_CLICKED(PB_FLIPV, OnFlipv)
	ON_BN_CLICKED(PB_SELTBT, OnSeltbt)
	ON_CBN_SELCHANGE(CB_TTARGET, OnSelchangeTk)
	ON_CBN_SELCHANGE(CB_TF, OnSelchangeTf)
	ON_CBN_SELCHANGE(CB_LAYOUT, OnSelchangeTgen)
	ON_BN_CLICKED(PB_DELTEX, OnDeltex)
	ON_CBN_CLOSEUP(IDC_TAB1, OnCloseupTab1)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTextures message handlers

void CDlgTextures::OnCancel() 
{

}

//select path search
void CDlgTextures::OnButton1() 
{
    if(_barNoty)
        BARDLG()->BlockRetreive(1);

    
	if(_showPick)
    {
		LPCTSTR pStr = BrowseForFolder("Select texture Search Directory",  theApp.TexDir());
        if(pStr)
        {
            theApp.TexDir(pStr);
            SetDlgItemText(ST_TEXTUREFLD, theApp.TexDir());
        }
    }
    BuildFileList();
    if(_texFiles.size())
    {
        _texIter = 0;
        m_lf.SetCurSel(0);
    }
    ShowPicture(_texIter);
	_showPick = TRUE;

    if(_barNoty)
        BARDLG()->BlockRetreive(0);
}




BOOL CDlgTextures::OnInitDialog() 
{
	CVertDlgDlg::OnInitDialog();
    Enable(1);

    //_tabdirs.SubclassDlgItem(IDC_TAB1,this);
    m_lf._pParent = this;
    
    m_scalex.SetRange(1,1000);  // /10
    m_scaley.SetRange(1,1000);  // /10
    m_shiftx.SetRange(-1000,1000);  // /10
    m_shifty.SetRange(-1000,1000);  // /10

    m_scalex.SetPos(10);
    m_scaley.SetPos(10);
    m_shiftx.SetPos(0);
    m_shifty.SetPos(0);
    m_escalex.SetLimitText(6);
    m_escaley.SetLimitText(6);
    m_eshiftx.SetLimitText(6);
    m_eshifty.SetLimitText(6);
    int nFkags = m_nFlags;
    m_escalex.SetWindowText("1.00");
    m_escaley.SetWindowText("1.00");
    m_eshiftx.SetWindowText("0.00");
    m_eshifty.SetWindowText("0.00");

    _btnTexRot.AutoLoad(PB_ROTATE, this);
    _btnTexSh.AutoLoad(PB_FLIPH, this);
    _btnTexSv.AutoLoad(PB_FLIPV, this);

    ((CComboBox*)GetDlgItem(CB_TTARGET))->SetCurSel(0);
    ((CComboBox*)GetDlgItem(CB_TF))->SetCurSel(0);
    ((CComboBox*)GetDlgItem(CB_LAYOUT))->SetCurSel(0);


    EnableTexControls(0);
    _initalized=TRUE;
	_showPick = FALSE;

    SetDlgItemText(ST_TEXTUREFLD, theApp.TexDir());

	PostMessage(WM_COMMAND,IDC_BUTTON1);
    m_nFlags =nFkags;
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTextures::OnDestroy() 
{
    AfxGetApp()->WriteProfileString( _T("DIR"), _T("IMAGES") , theApp.TexDir());
	CVertDlgDlg::OnDestroy();
	
}


void CDlgTextures::BuildFileList() 
{
    CWaitCursor cwc; // if not var is excluded in release(1);
    if(_barNoty)
        START_SPLASH();
	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	
	

    TexHandler      th;
    DWORD           ct = GetTickCount();
	REDIR();
    tstring         fullpth = theApp.TexDir();
	CDirChange	    cd(theApp.TexDir());//chdir
    

	m_lf.ResetContent();
    _texFiles.clear();
	_tabdirs.ResetContent();
    cd.UseObj();
	TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,NULL,32,0,NULL};
    
	_tabdirs.AddString("Resources");
    _tabdirs.AddString("/");



	if((hfFind=FindFirstFile(_T("*.*"), &fDATA)) != INVALID_HANDLE_VALUE)
	{
        if(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && fDATA.cFileName[0]!='.')
		{
			_tabdirs.AddString (fDATA.cFileName);
		}
			
		while(FindNextFile(hfFind, &fDATA)!=0)
		{
            if(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && fDATA.cFileName[0]!='.')
			{
				_tabdirs.AddString (fDATA.cFileName);
			}
		}
	}
	FindClose(hfFind);  


    KEEP_WAIT("      Getic 3.00 Full Version. ");
    Sleep(1000);

    if(_barNoty)
    {
        BARDLG()->ShowWindow(SW_SHOW);
        END_WAIT("");
    }
    //OnCloseupTab1() ;

    
    FRM()->PostMessage(WM_SPLASHGONE);
}


void CDlgTextures::PopSubDirText(LPCTSTR subDir)
{
    int             idx;
	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	
    tstring         fullpth;
    tstring		    fullFname;	
	
    REDIR();
    if(!_stricmp(subDir, "/"))
    {
        fullpth = theApp.TexDir();
        if(fullpth[fullpth.length()-1]!='\\')
            fullpth +="\\";
    }
    else if(!_stricmp(subDir, "(Res)"))
    {
        fullpth = theApp.HomeDir() ;
        fullpth +=  "\\res\\";
    }
    else
    {
        fullpth = theApp.TexDir();
        if(fullpth[fullpth.length()-1]!='\\')
            fullpth   += "\\";
        fullpth   += subDir;
	    fullpth   += "\\";
    }

    
    CDirChange	    cd(fullpth.c_str()); 
    SBT(0, fullpth.c_str());
    SetDlgItemText(ST_TEXTUREFLD, MKSTR("Textures From:\n%s",cd.CurDir()));
    
    m_lf.SetRedraw(0);
	m_lf.ResetContent();
    _texFiles.clear();
    cd.UseObj();
	if((hfFind=FindFirstFile(_T("*.*"), &fDATA)) != INVALID_HANDLE_VALUE)
	{
		if(!(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
            char* pext = _tcschr(fDATA.cFileName,'.');
            if(pext)pext++;

            if(pext &&  (!stricmp(pext,"bmp") || !stricmp(pext,"tga") || !stricmp(pext,"jpg") ))
            {
                fullFname = fullpth;
			    fullFname += fDATA.cFileName;
                _texFiles.push_back(fullFname);
    			idx = m_lf.AddString(fDATA.cFileName);
                if(_barNoty)
                    KEEP_WAIT(fDATA.cFileName);
            }
		} 
			
		while(FindNextFile(hfFind, &fDATA)!=0)
		{
			if(!(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
                char*pext = _tcschr(fDATA.cFileName,'.');
                if(pext)pext++;
                if(pext &&  (!stricmp(pext,"bmp") || !stricmp(pext,"tga") || !stricmp(pext,"jpg") ))
                {
        
			       fullFname = fullpth;
				    fullFname += fDATA.cFileName;
	               _texFiles.push_back(fullFname);
	                idx = m_lf.AddString(fDATA.cFileName);
                    if(_barNoty)
                        KEEP_WAIT(fDATA.cFileName);
                }
            }
		}
	}
	FindClose(hfFind);  

    m_lf.SetRedraw(1);
    if(_texFiles.size())
    {
        m_lf.SetCurSel(0);
    }
    m_lf.Invalidate();
}



int	 CDlgTextures::GetCurSel(char* pTex)
{
	int sel = m_lf.GetCurSel();
	m_lf.GetText(sel,pTex);
	return sel;
}

void CDlgTextures::ShowPicture(int index)
{
    m_bpp.SetWindowText(MKSTR("%d",_bpp));
    m_size.SetWindowText(MKSTR("%d %d",_sz.cx, _sz.cy));
    m_name.SetWindowText(_name.c_str());
}


void CDlgTextures::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

int CDlgTextures::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CVertDlgDlg::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetCurrentDirectory(_MAX_PATH,_cwd);
	return 0;
}


void CDlgTextures::OnDblclkList1() 
{
#ifdef _FREE
    if(GUtex==1)
    {
        SBT(0, "Cannot set texture over Light Map stage");
        MessageBeep((UINT)-1);
        return ;
    }
#endif //

    if(m_lf.GetCount())
    {
	    OnApplytex(); 
    }
}


void CDlgTextures::OnSelchangeList1() 
{
    if(m_lf.GetCount())
    {
	    _texIter = m_lf.GetCurSel();
	    ShowPicture	(_texIter);


		char lbText[128];

		m_lf.GetText(_texIter, lbText);
        DWORD dwCreaTexMode = m_lf.GetItemData(_texIter);
		Texture* ptex = GTexSys.GetTempTexture(lbText, dwCreaTexMode);
		if(ptex)
		{
		    // se up the props in the second pane
		    DWORD  creaMode  = TGET_TARGET(dwCreaTexMode);
		    DWORD  filMode   = TGET_FILTER(dwCreaTexMode);
            DWORD  fwrpMode  = TGET_WRAP(dwCreaTexMode);

            switch(creaMode)
            {
                default:      
                case GEN_TEX_2D_MAP:	
                    ((CComboBox*)GetDlgItem(CB_TTARGET))->SetCurSel(0);
                    break;
                case GEN_TEX_CUBE_MAP:
                    ((CComboBox*)GetDlgItem(CB_TTARGET))->SetCurSel(1);
                    break;
            }

        
            if(filMode & GEN_TEX_MM_TRILINEAR)
                ((CComboBox*)GetDlgItem(CB_TF))->SetCurSel(2);
            else if(filMode & GEN_TEX_MM_BILINEAR)
                ((CComboBox*)GetDlgItem(CB_TF))->SetCurSel(1);
            else
                ((CComboBox*)GetDlgItem(CB_TF))->SetCurSel(0);

            if(fwrpMode & GEN_TEX_HAS_CUBE_T)
                ((CComboBox*)GetDlgItem(CB_LAYOUT))->SetCurSel(1);
            else if(fwrpMode & GEN_TEX_HAS_CUBE_M)
                ((CComboBox*)GetDlgItem(CB_LAYOUT))->SetCurSel(2);
            else
                ((CComboBox*)GetDlgItem(CB_LAYOUT))->SetCurSel(0);
        }

    }
}

BOOL CDlgTextures::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if(!_initalized)
        return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

    if(_barNoty)
    {
		if(!BARDLG()->IsRetreiveBlocked())
		{
			if(nCode == EN_CHANGE )
			{
				_edChanged = TRUE;
				Dirty();
			}
            if(nCode == EN_KILLFOCUS)
            {
                if(!BARDLG()->IsRetreiveBlocked())
                {
                    OnOk() ;
                }
            }

		}
    }
    else
    {
		if(nCode == EN_CHANGE )
		{
			_edChanged = TRUE;
			Dirty();
		}
        if(nCode == EN_KILLFOCUS)
        {
            if(!BARDLG()->IsRetreiveBlocked())
            {
                OnOk() ;
            }
        }

    }
	return CVertDlgDlg::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CDlgTextures::SetTexCoords(TexCoords& texCoord)
{
    _texCoord   =   texCoord;

    m_escalex.SetWindowText(MKSTR("%2.4f", texCoord.scalex));
    m_escaley.SetWindowText(MKSTR("%2.4f", texCoord.scaley));
    m_eshiftx.SetWindowText(MKSTR("%2.4f", texCoord.shiftx));
    m_eshifty.SetWindowText(MKSTR("%2.4f", texCoord.shifty));

    m_scalex.SetPos(10*texCoord.scalex);
    m_scaley.SetPos(10*texCoord.scaley);
    m_shiftx.SetPos(10*texCoord.shiftx);
    m_shifty.SetPos(10*texCoord.shifty);

};


void CDlgTextures::OnOk() 
{
    if(GUtex==1)
    {
        SBT(0, "Cannot set texture over Light Map stage");
        MessageBeep((UINT)-1);
        return ;
    }

	OnApplytex(); 
}

void CDlgTextures::OnApllytex() 
{
	//OnApplytex();
}

UINT CDlgTextures::OnGetDlgCode() 
{
	return DLGC_WANTTAB ;
	return CVertDlgDlg::OnGetDlgCode();
}

void CDlgTextures::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_TAB)
	{
		if(AKEY(VK_SHIFT))
			DOC()->OnTabPrevsel();
		else
			DOC()->OnTabNextsel();
	}

	CVertDlgDlg::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDlgTextures::OnChangeEdit3() 
{
	char text[64];
    m_eshiftx.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.shiftx);
    m_shiftx.SetPos(10*_texCoord.shiftx);
}

void CDlgTextures::OnChangeEdit2() 
{
	char text[64];
    m_eshifty.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.shifty);
    m_shifty.SetPos(10*_texCoord.shifty);
}

void CDlgTextures::OnChangeEdit5() 
{
	char text[64];
    m_escalex.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.scalex);
	m_scalex.SetPos(10*_texCoord.scalex);
}

void CDlgTextures::OnChangeEdit4() 
{
	char text[64];
    m_escaley.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.scaley);
    m_scaley.SetPos(10*_texCoord.scaley);

}

void CDlgTextures::OnChangeEdit6() 
{
/*
    char text[64];
*/
}

void CDlgTextures::OnDeltaposSpinShiftV(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int ip = pNMUpDown->iPos;

	m_eshifty.SetWindowText(MKSTR("%2.4f",ip/10.));
	OnTexCoordChanged();	
	*pResult = 0;
}

void CDlgTextures::OnDeltaposSpinShiftH(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int ip = pNMUpDown->iPos;
    m_eshiftx.SetWindowText(MKSTR("%2.4f",ip/10.));
	OnTexCoordChanged();	
	*pResult = 0;
}

void CDlgTextures::OnDeltaposSpinScaleV(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int ip = pNMUpDown->iPos;
    m_escaley.SetWindowText(MKSTR("%2.4f",ip/10.));
	OnTexCoordChanged();	
	*pResult = 0;
}

void CDlgTextures::OnDeltaposSpinScaleH(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int ip = pNMUpDown->iPos;
    m_escalex.SetWindowText(MKSTR("%2.4f",ip/10.));
    OnTexCoordChanged();	
	*pResult = 0;
}


void CDlgTextures::SelectFaceTexture(const char* psztexture)
{
    char loco[_MAX_PATH];
    _tcscpy(loco, psztexture);
    if(_tcschr(loco,'\\') || _tcschr(loco,'/') )
    {
        char fn[_MAX_PATH];
        char ext[_MAX_PATH];
        char dc[_MAX_PATH];

        _tsplitpath(psztexture,dc,dc,fn,ext);
        _tcscpy(loco, fn);
        _tcscat(loco,ext);
    }
    GetDlgItem(ST_NAME)->SetWindowText(loco);
}

void CDlgTextures::EnableTexControls(BOOL b)
{
    GetDlgItem(IDC_SPIN1)->EnableWindow(b);
    GetDlgItem(IDC_EDIT2)->EnableWindow(b);
    GetDlgItem(IDC_EDIT4)->EnableWindow(b);
    GetDlgItem(IDC_SPIN3)->EnableWindow(b);
    GetDlgItem(IDC_EDIT3)->EnableWindow(b);
    GetDlgItem(IDC_SPIN2)->EnableWindow(b);
    GetDlgItem(IDC_EDIT5)->EnableWindow(b);
    GetDlgItem(IDC_SPIN4)->EnableWindow(b);
    GetDlgItem(PB_ROTATE)->EnableWindow(b);
    GetDlgItem(PB_FLIPH)->EnableWindow(b);
    GetDlgItem(PB_FLIPV)->EnableWindow(b);
}

void CDlgTextures::OnFliph() 
{
    Fliptexture('H');
}

void CDlgTextures::OnFlipv() 
{
	Fliptexture('V');
}

void CDlgTextures::OnRotate() 
{
    _bSkyDom ? DomOnRotate() : DocOnRotate() ;
}


void CDlgTextures::Fliptexture(char cd)
{
    _bSkyDom ? DomFliptexture(cd) :DocFliptexture(cd);
}

void CDlgTextures::OnTexCoordChanged()
{
    _bSkyDom ? DomOnTexCoordChanged(): DocOnTexCoordChanged();
}

BOOL CDlgTextures::OnApplytex() 
{
#ifdef _FREE
    if(GUtex==1)
    {
        SBT(0, "Cannot set texture over Light Map stage");
        MessageBeep((UINT)-1);
        return FALSE;
    }
#endif //

    CWnd* pWnd = GetParent();
    if(pWnd)
    {
        if(pWnd->SendMessage(WM_HANDLE_TEXSEL, m_lf.GetCurSel(),(long)_name.c_str()))
        {
            return TRUE;
        }
    }
    return DocOnApplytex();
}

BOOL CDlgTextures::SetTexture(Poly& poly, int& pV)
{
    return DocSetTexture(poly, pV);
}

BOOL CDlgTextures::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    if(wParam >= 25000 && wParam < 25032)
    {
        SelectSubPath(wParam - 25000);
    }
	return CVertDlgDlg::OnCommand(wParam, lParam);
}

void CDlgTextures::OnSeltbt() 
{
	char tn[128];
	GetDlgItem(ST_NAME)->GetWindowText(tn,128);	
    SelectSubPath(0);
	m_lf.SelectString(-1,tn);
}

BOOL CDlgTextures::DocSetTexture(SceItem& item, int& pV)
{
	if(_copyt) 
        DOC()->DuplicateFile(_name.c_str(), 1);
	_copyt = FALSE;

    #pragma message("check item apply texture mode")
    int cs = m_lf.GetCurSel();
    int id = m_lf.GetItemData(cs);

    if(_resetTex)
    {
        item.SetTex("");
    }
    else
    {
        PathHandler ph(_name.c_str()) ;
        item.SetTex(ph.File(), GUtex, GettexGenFlag());
    }

	return TRUE;
}


BOOL CDlgTextures::DocSetTexture(Poly& poly, int& pV)
{
	if(_copyt) 
        DOC()->DuplicateFile(_name.c_str(), 1);
	_copyt = FALSE;

	#pragma message("check poly apply texture mode")
    int cs = m_lf.GetCurSel();

    int id = ((CComboBox*)GetDlgItem(CB_LAYOUT))->GetCurSel();
    if(_resetTex)
    {
        poly.SetTex("");
        poly._polyprops &= ~FACE_CUSTLM;
    }
    else
    {
        PathHandler ph(_name.c_str());
        CDirChange  cdc(ph.Path());

        poly.SetTex(ph.File(), GUtex, GettexGenFlag());


        if(GUtex==1)
        {
            if(poly.GetHtex(GUtex) != 0)
            {
                poly._polyprops |= FACE_CUSTLM;
                _texwasSet=1;
            }
            else
                poly._polyprops &= ~FACE_CUSTLM;
        }
    }
	return TRUE;
}

BOOL CDlgTextures::DocOnApplytex() 
{
    if(IsDlgButtonChecked(CK_INGNORE))
        return FALSE;

    // brushes
    int cnt;
    Brush** ppb =  DOC()->GetSelBrushes(cnt);
	_copyt = TRUE;
    _texwasSet=0;

    DWORD gtf = GettexGenFlag();

    if(TGET_TARGET(gtf) && (TGET_WRAP(gtf) == GEN_TEX_HAS_CUBE_T ||
                            TGET_WRAP(gtf) == GEN_TEX_HAS_CUBE_M))
    {
        if(cnt==1 && (*ppb)->_polys.size()==6)
        {
            //apply cube texture
            forall((*ppb)->_polys, this, &CDlgTextures::SetTexture,cnt);

        }
    }
    else
    {
        while(cnt-->0)
        {
            forall((*ppb)->_polys, this, &CDlgTextures::SetTexture,cnt);

            if((*ppb)->HasFlag(BRSH_BIGTERRAIN) && GUtex==1)
            {
                if(_texwasSet)
                {
                    SCENE().Terrain().Flags() |= TERR_CUSTLM;
                }
                else
                {
                    SCENE().Terrain().Flags() &= ~TERR_CUSTLM;
                }
                continue;
            }
            else
            if((*ppb)->IsProjector())
            {
                DOC()->ProjectTexture(*(*ppb));
            }
            ppb++;
        }
    }
	_copyt = TRUE;

    // polys

	map<Brush*,int>	uniquerush;	

    Poly** ppp = DOC()->GetSelPolys(cnt);
    while(cnt-->0)
    {
		uniquerush[(*ppp)->_pBrush]=1;
		SetTexture(**ppp , cnt);
		ppp++;
    }

	map<Brush*,int>::iterator ubb = uniquerush.begin();
	map<Brush*,int>::iterator ube = uniquerush.end();
	for(;ubb!=ube;ubb++)
	{
		(*ubb).first->Select();
        DOC()->_pSelBrushes.push_back((*ubb).first);
	}

	DOC()->Recut();

	if(uniquerush.size())
		DOC()->DeselectBrushes();

    SceItem** ppItem = DOC()->GetSelItems(cnt);
    while(cnt-->0)
    {
        DocSetTexture(**ppItem, cnt);
		ppItem++;
    }


	if(_edChanged)
		OnTexCoordChanged();
	_edChanged=FALSE;

	DOC()->Invalidate();
    return TRUE;

}
void CDlgTextures::DocOnTexCoordChanged()
{
	char	text[32];
	int		cnt;

    m_escalex.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.scalex);
    m_escaley.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.scaley);
    m_eshiftx.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.shiftx);
    m_eshifty.GetWindowText(text,32);sscanf(text, "%f", &_texCoord.shifty);

	map<Brush*,int>	uniquerush;	

	_copyt = TRUE;
    Poly** ppp = DOC()->GetSelPolys(cnt);
    while(cnt-->0)
    {

		(*ppp)->_texcoord.scalex = _texCoord.scalex;
		(*ppp)->_texcoord.scaley = _texCoord.scaley;
		(*ppp)->_texcoord.shiftx = _texCoord.shiftx;
		(*ppp)->_texcoord.shifty = _texCoord.shifty;

		(*ppp)->ApplyNewTexCoord();
		uniquerush[(*ppp)->_pBrush]=1;
        ppp++;
    }

	map<Brush*,int>::iterator ubb = uniquerush.begin();
	map<Brush*,int>::iterator ube = uniquerush.end();
	for(;ubb!=ube;ubb++)
	{
		(*ubb).first->Select();
        DOC()->_pSelBrushes.push_back((*ubb).first);
	}

	DOC()->Recut();
	DOC()->DeselectBrushes();
	DOC()->Invalidate();
}
void CDlgTextures::DocFliptexture(char cd)
{
    map<Brush*,int>	uniquerush;	
   //reverse tcoord only for tha poly

    int cnt;
    Poly** ppp = DOC()->GetSelPolys(cnt);
    while(cnt-->0)
    {
		(*ppp)->FlipTC(cd);
		uniquerush[(*ppp)->_pBrush]=1;
        ppp++;
    }
    // select these brushes to allow update  on them
	map<Brush*,int>::iterator ubb = uniquerush.begin();
	map<Brush*,int>::iterator ube = uniquerush.end();
	for(;ubb!=ube;ubb++)
	{
		(*ubb).first->Select();
        DOC()->_pSelBrushes.push_back((*ubb).first);
	}
	DOC()->Recut();
	DOC()->DeselectBrushes();
	DOC()->Invalidate();

}
void CDlgTextures::DocOnRotate() 
{
    // keep unique brush if more polys are seletced
    #pragma message("use set where appropriate not map")
	map<Brush*,int>	uniquerush;	

    int cnt=0;
    Poly** ppp = DOC()->GetSelPolys(cnt);
    while(cnt-->0)
    {
		(*ppp)->RotateTC(0,0);
		uniquerush[(*ppp)->_pBrush]=1;
        ppp++;
    }

    // select these brushes to allow update  on them
	map<Brush*,int>::iterator ubb = uniquerush.begin();
	map<Brush*,int>::iterator ube = uniquerush.end();
	for(;ubb!=ube;ubb++)
	{
		(*ubb).first->Select();
        DOC()->_pSelBrushes.push_back((*ubb).first);
	}
	DOC()->Recut();
	DOC()->DeselectBrushes();
	DOC()->Invalidate();

}

BOOL CDlgTextures::DomSetTexture(Poly& poly, int& pV)
{

    return 1;
}

BOOL CDlgTextures::DomOnApplytex() 
{
    return 1;
}

void CDlgTextures::DomOnTexCoordChanged()
{
}

void CDlgTextures::DomFliptexture(char cd)
{
}

void CDlgTextures::DomOnRotate() 
{
}



void     CDlgTextures::GetTextureSubPaths(vvector<tstring>& r)
{
    int icnt = _tabdirs.GetCount();
    for(int i=0;i<icnt;i++)
    {
        char   text[128];
        _tabdirs.GetLBText(i,text);
        r << text;
    }
    return;
}

void     CDlgTextures::SelectSubPath(int i)
{
    if(_tabdirs.GetCurSel() != i)
    {
        _tabdirs.SetCurSel(i);
        OnCloseupTab1();
    }
}


void CDlgTextures::OnSelchangeTk() 
{
}

void CDlgTextures::OnSelchangeTf() 
{
}

void CDlgTextures::OnSelchangeTgen() 
{
}

DWORD CDlgTextures::GettexGenFlag()
{
    DWORD gm = 0 ;
    int tt  = ((CComboBox*)GetDlgItem(CB_TTARGET))->GetCurSel();
    int tf  = ((CComboBox*)GetDlgItem(CB_TF))->GetCurSel();
    int tg  = ((CComboBox*)GetDlgItem(CB_LAYOUT))->GetCurSel();

    gm |= (tt & 0xF);
    gm |= tf ? (1<<(0x3 + tf)) : 0;
    gm |= tg ? (1<<(0x9 + tg)) : 0;


    if(IsDlgButtonChecked(CK_ALPHA) || AKEY(VK_SPACE))
    {
        gm |= TEX_ALPHABW;
        CheckDlgButton(CK_ALPHA, BST_UNCHECKED);
    }
    return gm;

}
void CDlgTextures::OnDeltex() 
{
#ifdef _FREE
    if(GUtex==1)
    {
        SBT(0, "Cannot delete texture over Light Map stage");
        MessageBeep((UINT)-1);
        return ;
    }
#endif //

    _resetTex   = TRUE;
    OnApplytex();     
    _resetTex   = FALSE;	
}


void CDlgTextures::OnCloseupTab1() 
{
    CString cs;
    int ics = _tabdirs.GetCurSel();
    if(ics>=0)
    {
	    _tabdirs.GetLBText(_tabdirs.GetCurSel(), cs);
	    PopSubDirText(cs);
    }
	
}
void     CDlgTextures::OnAction()
{
    OnOk();    
}
