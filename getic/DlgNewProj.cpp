// DlgNewProj.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "geticgui.h"
#include "DlgNewProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DlgNewProj::DlgNewProj(CWnd* pParent /*=NULL*/)
	: CDialog(DlgNewProj::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgNewProj)
	m_prjName = _T("");
	m_location = _T("");
	//}}AFX_DATA_INIT
}


void DlgNewProj::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgNewProj)
	DDX_Text(pDX, IDC_EDIT1, m_prjName);
	DDX_Text(pDX, IDC_EDIT6, m_location);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgNewProj, CDialog)
	//{{AFX_MSG_MAP(DlgNewProj)
	ON_NOTIFY(LVN_ITEMCHANGED, LV_PJTYPE, OnItemchangedPjtype)
	ON_NOTIFY(LVN_ITEMCHANGING, LV_PJTYPE, OnItemchangingPjtype)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_EN_KILLFOCUS(IDC_EDIT6, OnKillfocusEdit6)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(PB_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgNewProj message handlers

BOOL DlgNewProj::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    TCHAR cd[PATH_MAX]={0} ;

    GetCurrentDirectory(PATH_MAX, cd);
    m_location = cd;
    m_prjName = "";
    GetDlgItem(IDOK)->EnableWindow(0);
    UpdateData(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgNewProj::OnItemchangedPjtype(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DlgNewProj::OnItemchangingPjtype(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DlgNewProj::OnChangeEdit1() 
{
    TCHAR ct[128] = {0};

    int len = GetDlgItemText(IDC_EDIT1, ct,127);
	GetDlgItem(IDOK)->EnableWindow(len>1);
}

void DlgNewProj::OnKillfocusEdit1() 
{
	// TODO: Add your control notification handler code here
	
}

void DlgNewProj::OnChangeEdit6() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void DlgNewProj::OnKillfocusEdit6() 
{
	// TODO: Add your control notification handler code here
	
}

void DlgNewProj::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DlgNewProj::OnOk() 
{
    UpdateData(1);
    TCHAR prjName[128] = {0};
    strcpy(prjName, const char*(m_prjName));
    TCHAR* pDot = _tcschr(prjName,',');
    if(pDot)
        *pDot = _T('\0');
    _tcscat(prjName,".ggf");


    CDialog::OnOK();	
}

void DlgNewProj::OnBrowse() 
{
    const char* pscDest = BrowseForFolder("Select Output Directory",theApp.DocsDir());
    if(pscDest)
    {
        m_location = pscDest; 
        UpdateData(0);
    }
}


