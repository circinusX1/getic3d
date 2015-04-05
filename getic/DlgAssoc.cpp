// DlgLeafs.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgAssoc.h"

CDlgAssoc::CDlgAssoc(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgAssoc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAssoc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAssoc::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAssoc)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAssoc, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgAssoc)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, OnOk)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAssoc message handlers

BOOL CDlgAssoc::OnInitDialog() 
{
    CBASEDLG::OnInitDialog();
    CComboBox* pcb = (CComboBox*)GetDlgItem(CB_TYPE);
    pcb->AddString("Integer");
    pcb->AddString("Real 32 bit");
    pcb->AddString("Color BYTE[4]");
    pcb->AddString("Texture Coordinates UV");
    pcb->AddString("Point Coordinates XYZ");
    pcb->AddString("String 32 Characters" );

    ((CEdit*)GetDlgItem(EF_NAME))->SetLimitText(16);

    if(!s_name.IsEmpty())
    {
        SetDlgItemText(EF_NAME, s_name);
        pcb->SetCurSel(e_type-1);
    }
    /*
    V_INT,
    V_REAL,
    V_CLR,
    C_UV,
    V_V3,
    V_STR,
    */

	return TRUE;  
	              
}
void CDlgAssoc::OnDestroy() 
{
}

void CDlgAssoc::OnCancel() 
{
    CBASEDLG::OnCancel();
}

void CDlgAssoc::OnOk() 
{
    CComboBox* pcb = (CComboBox*)GetDlgItem(CB_TYPE);
     GetDlgItemText(EF_NAME,s_name);
    if(!s_name.IsEmpty() && pcb->GetCurSel()!=0)
    {
       
        e_type =(pcb->GetCurSel()+1);
        EndDialog(IDOK);
        return;
    }
    CDlgAssoc::OnCancel() ;
}

