//---------------------------------------------------------------------------------------
// DlgGs.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "DlgGs.h"
#include "z_ed2Doc.h"
#include "geticgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
// CDlgGs dialog
CDlgGs::CDlgGs(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgGs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGs)
	
	//}}AFX_DATA_INIT
}

//---------------------------------------------------------------------------------------
void CDlgGs::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGs)
	DDX_Control(pDX, PB_HOMEDIR, m_hd);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_EDIT1, m_edeps);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	//}}AFX_DATA_MAP
}

//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDlgGs, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgGs)
	ON_WM_DELETEITEM()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_EN_CHANGE(EF_MINDIM, OnChangeMindim)
	ON_EN_CHANGE(EF_MINPOLYS, OnChangeMinpolys)
	ON_EN_CHANGE(EF_MAXDIM, OnChangeMaxdim)
	ON_BN_CLICKED(CK_DETBRSH, OnCheck1) 
	ON_BN_CLICKED(CK_BACKFACE, OnBackface)
	ON_BN_CLICKED(CK_LIGHT, OnLight)
	ON_BN_CLICKED(CK_SHOWCUTS, OnShowcuts)
	ON_BN_CLICKED(CK_FORCE, OnForce)
	ON_BN_CLICKED(CK_CARVE, OnCarve)
	ON_BN_CLICKED(PB_HOMEDIR, OnHomedir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
// CDlgGs message handlers
void CDlgGs::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{

	
	CBASEDLG::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}

//---------------------------------------------------------------------------------------
void CDlgGs::OnDestroy() 
{
	CBASEDLG::OnDestroy();
}

//---------------------------------------------------------------------------------------
BOOL CDlgGs::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	REAL ge = GEpsilon;
	m_spin.SetRange(2,64);
    m_spin.SetPos(4);
    m_edeps.SetWindowText(MKSTR("%2.4f",ge));


    CheckDlgButton(CK_DETBRSH, GDetEnabled ?BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_SHOWTP, BShowTrPolys?BST_CHECKED : BST_UNCHECKED);
    //BShowTrPolys= Str2Int(GetProfileString("GDLG","SHOWTP"),0);

    SetDlgItemText(IDC_EDIT4,theApp.HomeDir());


    SetDlgItemInt(EF_MAXDIM, GMaxDetDim);
    SetDlgItemInt(EF_MINDIM, GMinDetDim);
    SetDlgItemInt(EF_MINPOLYS, GMinDetPolys);

    CheckDlgButton(CK_CULLTERR,GCullTerrCell?BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_DISGRPCSG,GDisableGrpCSG?BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_KEEPPRJ_TEX,GKeepProjections?BST_CHECKED : BST_UNCHECKED);


  

	CheckDlgButton(CK_BACKFACE, GBackFace   ? BST_CHECKED : BST_UNCHECKED); 
	CheckDlgButton(CK_LIGHT,    GLight	    ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_CARVE,    GCarving    ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(CK_CARVE,    GUpdateImm  ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_ANIM,     GAnimCompilation  ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_SELBR,    GSelTexFace  ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_TASK,     BrushTask   ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_CALCNORMALS,  BVxNormals ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_SHOWCB,   BShowCb     ? BST_CHECKED : BST_UNCHECKED);

    CheckDlgButton(CK_SHOW_FRUSTCULL, GFrustCull? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(CK_SHOWLC, BShowLCirc? BST_CHECKED : BST_UNCHECKED);


    CheckDlgButton(CK_AUTOBACKUP, GAutoSave ? BST_CHECKED : BST_UNCHECKED);
    
    OnCheck1() ;

    if(GMeters	== 1)
        CheckRadioButton(RB_CM, RB_FEET, RB_CM);
    else if(GMeters	== 100)
        CheckRadioButton(RB_CM, RB_FEET, RB_M);
    else
        CheckRadioButton(RB_CM, RB_FEET, RB_FEET);

	return TRUE;  
	              
}

//---------------------------------------------------------------------------------------
void CDlgGs::OnOk() 
{

    if(IsDlgButtonChecked(RB_CM))
        GMeters	= 1;
    else if(IsDlgButtonChecked(RB_M))
        GMeters	= 100;
    else 
        GMeters	= (2.54*12);

	if(IsDlgButtonChecked(CK_3DIMM))
		GUpdateImm=TRUE;
	else
		GUpdateImm=FALSE;

    if(IsDlgButtonChecked(CK_AUTOBACKUP))
        GAutoSave =1;
    else
        GAutoSave =0;

    GFrustCull = IsDlgButtonChecked(CK_SHOW_FRUSTCULL);
    BShowLCirc = IsDlgButtonChecked(CK_SHOWLC);

    GAnimCompilation = IsDlgButtonChecked(CK_ANIM);
    GDetEnabled = IsDlgButtonChecked(CK_DETBRSH);
    BShowTrPolys = IsDlgButtonChecked(CK_SHOWTP);
    GSelTexFace = IsDlgButtonChecked(CK_SELBR);
    BrushTask   = IsDlgButtonChecked(CK_TASK);
    BVxNormals  = IsDlgButtonChecked(CK_CALCNORMALS);
    BShowCb     = IsDlgButtonChecked(CK_SHOWCB);
    GCullTerrCell = IsDlgButtonChecked(CK_CULLTERR);
    GDisableGrpCSG  = IsDlgButtonChecked(CK_DISGRPCSG);
    GKeepProjections  = IsDlgButtonChecked(CK_KEEPPRJ_TEX);
#ifdef _FREE
    GLight = 0;
    GAutoSave = 0;
#endif //

    char psth[_MAX_PATH];
    GetDlgItemText(IDC_EDIT4,psth, _MAX_PATH);
    theApp.HomeDir(psth);

    _chdir(psth);
    _getcwd(psth, _MAX_PATH);

    if(psth[strlen(psth)-1]=='\\'||psth[strlen(psth)-1]=='/')
        psth[strlen(psth)-1]=0;

    WriteProfileString( _T("DIR"), _T("HOME"), psth);

    theApp.StartDir(MKSTR("%s\\compiled\\",psth));
    theApp.BspsDir(MKSTR("%s\\compiled\\",psth));
    theApp.DocsDir(MKSTR("%s\\maps\\",psth));
    theApp.ServerDir(MKSTR("%s\\server\\",psth));
    theApp.XsdCatItmFile(MKSTR("%s\\maps\\",psth));
    
    
 
	CBASEDLG::OnOK();
} 

//---------------------------------------------------------------------------------------
void CDlgGs::OnCancel() 
{
}

//---------------------------------------------------------------------------------------
void CDlgGs::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    int sp = pNMUpDown->iPos;
    REAL eps = 1.0/sp;
	
    m_edeps.SetWindowText(MKSTR("%2.4f",eps));
    GEpsilon = eps;
	*pResult = 0;
}

void CDlgGs::OnDeltaposMaxdim(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int sp = pNMUpDown->iPos;
    GetDlgItem(EF_MAXDIM)->SetWindowText(MKSTR("%d",sp));
    GMaxDetDim  = sp;
	*pResult = 0;
}

void CDlgGs::OnDeltaposMindim(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int sp = pNMUpDown->iPos;
    GetDlgItem(EF_MINDIM)->SetWindowText(MKSTR("%d",sp));
	GMinDetDim  = sp;
	*pResult = 0;
}

void CDlgGs::OnDeltaposMinpolys(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    int sp = pNMUpDown->iPos;
    GetDlgItem(EF_MINPOLYS)->SetWindowText(MKSTR("%d",sp));
    GMinDetPolys = sp;
	*pResult = 0;
}

void CDlgGs::OnChangeMindim() 
{
	
}

void CDlgGs::OnChangeMinpolys() 
{
}

void CDlgGs::OnChangeMaxdim() 
{
	
}

void CDlgGs::OnCheck1() 
{
    BOOL b = IsDlgButtonChecked(CK_DETBRSH);
}

void CDlgGs::OnBackface() 
{
	GBackFace = IsDlgButtonChecked(CK_BACKFACE);	
    DOC()->_viewChanged=3;    
}

void CDlgGs::OnLight() 
{
	GLight	= IsDlgButtonChecked(CK_LIGHT);
}

void CDlgGs::OnShowcuts() 
{
    GShowCuts = IsDlgButtonChecked(CK_SHOWCUTS);
}



void CDlgGs::OnForce() 
{
	GForceBSP = IsDlgButtonChecked(CK_FORCE);
	
}

void CDlgGs::OnCarve() 
{
    GCarving	= IsDlgButtonChecked(CK_CARVE);
}

void CDlgGs::OnHomedir() 
{
    
    LPCTSTR pfld = BrowseForFolder("Home Path",  theApp.HomeDir());    	
    if(pfld)
    {
        theApp.HomeDir(pfld);
        SetDlgItemText(IDC_EDIT4,theApp.HomeDir());
        WriteProfileString( _T("DIR"), _T("HOME"),theApp.HomeDir());
    }
}
