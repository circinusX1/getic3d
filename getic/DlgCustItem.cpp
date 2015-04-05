// DlgCustItem.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z_ed2Doc.h"
#include "DlgCustItem.h"
#include "DlgAssoc.h"
//#include "z-edmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgCustItem dialog


DlgCustItem::DlgCustItem(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgCustItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgCustItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgCustItem::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCustItem)
    DDX_Control(pDX, IDC_LIST1, m_lv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgCustItem, CBASEDLG)
	//{{AFX_MSG_MAP(DlgCustItem)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_ITEMSEL, OnItemChanged)
    ON_BN_CLICKED(IDC_BUTTON1, OnAddItem)
    ON_BN_CLICKED(IDC_BUTTON4, OnDelItem)
    ON_BN_CLICKED(IDC_BUTTON6, OnMoveDown)
    ON_BN_CLICKED(IDC_BUTTON7, OnMoveUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCustItem message handlers

void DlgCustItem::OnDestroy() 
{
    GetDlgItem(IDC_EDIT2)->SetFont(0);
    _hf.DeleteObject();
	CBASEDLG::OnDestroy();
}
 
BOOL DlgCustItem::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
    m_lv.SetHeadersNames("Variable Type","Variable Name",0);

    _hf.CreateFont( -10, 0, 0, 0, FW_MEDIUM, FALSE,
                   FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
                   CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                   FF_DONTCARE | DEFAULT_PITCH, "FixedSys" );
    

    GetDlgItem(IDC_EDIT2)->SetFont(&_hf);

	return TRUE;  
}


void DlgCustItem::OnOk() 
{
	
}

void DlgCustItem::OnCancel() 
{
	CBASEDLG::OnCancel();
}

LRESULT DlgCustItem::OnItemChanged(WPARAM w,LPARAM l)
{
    m_lv.GetItemText(w,0);
    m_lv.GetItemText(w,1);
    return 0;
}


void DlgCustItem::OnAddItem()
{
    static char* s[]={  "Integer",
                        "Real 32 bit",
                        "Color BYTE[4]",
                        "Texture Coordinates UV",
                        "Point Coordinates XYZ",
                        "String 32 Characters"};

    CDlgAssoc d;
    
    if(d.DoModal() == IDOK)
    {
        m_lv.AddNewItem(s[d.e_type-1],d.s_name,0);
        UpdateStruct();
    }
}

void DlgCustItem::OnDelItem()
{
}

void DlgCustItem::OnMoveDown()
{
}

void DlgCustItem::OnMoveUp()
{
}

void DlgCustItem::UpdateStruct()
{

}
