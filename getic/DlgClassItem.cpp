// DlgClassItem.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z_ed2Doc.h"
#include "z-edmap.h"
#include "DlgClassItem.h"
#include "DlgCustItem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgClassItem dialog


DlgClassItem::DlgClassItem(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgClassItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgClassItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgClassItem::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgClassItem)
	DDX_Control(pDX, PB_BROWSE, m_browse);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, LB_ITEMCAT, m_lb);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgClassItem, CBASEDLG)
	//{{AFX_MSG_MAP(DlgClassItem)
	ON_WM_DESTROY()
	ON_BN_CLICKED(PB_BROWSE, OnBrowse)
	ON_LBN_SELCHANGE(LB_ITEMCAT, OnSelchangeItemcat)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgClassItem message handlers

void DlgClassItem::OnDestroy() 
{
	CBASEDLG::OnDestroy();
}
 
BOOL DlgClassItem::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();

    _gameitemDlg.BlockBarUpdate();
    _gameitemDlg.SetItmType(ITM_GAME);
    _gameitemDlg.Create( DLG_I_GAMEITM, this);


    RECT rt;
	GetDlgItem(ST_PLACE_DLG)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	_gameitemDlg.MoveWindow(rt.left,rt.top,
				rt.right-rt.left, 
		    	rt.bottom-rt.top,1);

    _gameitemDlg.ShowWindow(SW_SHOW);

    if(PopulateFromCats())
    {
        SetDlgItemText(EF_FILEEXE,theApp.XsdCatItmFile());
    }
	return TRUE;  
	              
}

void DlgClassItem::OnBrowse() 
{
    
    char szFilters[] = _T(" Def Files (*.idf)|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("idf"), "*.idf", OFN_HIDEREADONLY, szFilters);
	
	dlg.m_ofn.lpstrTitle = _T("Select Item Definition File"); 

	if (theApp.DoFileDialog(dlg) != IDOK) 
	{
		REDIR();
        return;
	}
	REDIR();
    SetDlgItemText(EF_FILEEXE, dlg.m_ofn.lpstrFile);

    m_lb.ResetContent();
	DOC()->SaveTempFile();

    if(DOC()->_scene.SearchCats(dlg.m_ofn.lpstrFile))
    {
        if(PopulateFromCats())
        {
            theApp.XsdCatItmFile(dlg.m_ofn.lpstrFile);
            SetDlgItemText(EF_FILEEXE,theApp.XsdCatItmFile());
        }
        
        BOOL found = FALSE;
        // see if the scene has cats with the same name
        vvector<SceItem*>* pvitems = DOC()->_scene.GetItems();
        vvector<ItemCat>*  pcats = DOC()->_scene.GetCats();

        FOREACH(vvector<ItemCat>, (*pcats), pc)
        {
            ItemCat& ic = *pc;

            FOREACH(vvector<SceItem*>, (*pvitems), ppi)
            {
                SceItem* pI = *ppi;
                if(pI->_item == ITM_GAME)
                {
                    if(!strncmp(pI->_catname, ic._catname, tmin(strlen(ic._catname),strlen(pI->_catname))))
                    {
                        DOC()->SelectItem(pI);
                        found = 1;
                    }
                }
            }
        }
        if(found)
        {
            AfxMessageBox("You have to revisit all custom items because \n"
                          "the new definition file may changed their layout", MB_OK|MB_ICONINFORMATION);
        }
    }
}


int     DlgClassItem::PopulateFromCats()
{
    static char* s[]={  "Integer",
                        "Real 32 bit",
                        "Color BYTE[4]",
                        "Texture Coordinates UV",
                        "Point Coordinates XYZ",
                        "String 32 Characters"};

    m_lb.ResetContent();
	Scene&  scene = DOC()->_scene;
    if(scene.GetCats()->size())
    {
        int                 index;
        vvector<ItemCat>*   pcats = scene.GetCats();

        FOREACH(vvector<ItemCat>, (*pcats), pcat)
        {
            index = m_lb.AddString(pcat->_catname);
        }
    }
    return m_lb.GetCount();
}

void DlgClassItem::OnSelchangeItemcat() 
{
    int     cbSel = m_lb.GetCurSel();

    vvector<ItemCat>*   pcats = DOC()->_scene.GetCats();
    _gameitemDlg.Update(&pcats->at(cbSel));
    	
}

void DlgClassItem::OnOk() 
{
	
}

void DlgClassItem::OnCancel() 
{
	CBASEDLG::OnCancel();
}
