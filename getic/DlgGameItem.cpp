// DlgGameItem.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "GameItem.h"
#include "DlgGameItem.h"
#include "CustItemCat.h"
#include "DlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgGameItem dialog


DlgGameItem::DlgGameItem(CWnd* pParent /*=NULL*/)
: CBASEDLG(DlgGameItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGameItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgGameItem::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGameItem)
	DDX_Control(pDX, LB_PROPSCAT, m_lb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGameItem, CBASEDLG)
	//{{AFX_MSG_MAP(DlgGameItem)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(LB_PROPSCAT, OnSelchangeProps)
	ON_LBN_DBLCLK(LB_PROPSCAT, OnDblclkPropscat)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REDRAWCHILDREN,OnRepaint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGameItem message handlers

BOOL DlgGameItem::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	return TRUE;  
}

void DlgGameItem::OnDestroy() 
{
	CBASEDLG::OnDestroy();
	
}

void    DlgGameItem::Update(SceItem* pItem, int selCount)
{
    if(pItem->_item != ITM_GAME)
        return;

    _updatingCtrls = TRUE;
    _curCat     = 0;
    m_lb.Clean();

    
    GameItem* pGi = (GameItem*)pItem;
    CLbDlg* pDlg = 0;

    _pCurItem = pItem;


    _curCat = 0;

    SetDlgItemText(ST_CATNAME,pGi->_catname);


    FOREACH(vvector<NameTypeValue>,pGi->_data, ppcp)
    {
        NameTypeValue& rTypeVal = *(ppcp);
        pDlg = 0;
        switch(ppcp->_type)
        {
            case V_TEXT:
                pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
                pDlg->_e1.SetWindowText((char*)rTypeVal._value);
                break;
            case V_FILENAME:
                pDlg = m_lb.AddItem(rTypeVal._name,DL_FILEPCK);
                pDlg->_szFile     = rTypeVal._value;
                pDlg->_bFileOpen  = 1;
                pDlg->_fileExt    = "*";
                pDlg->_fileFilter = "Files (*.*)|*.*|All Files (*.*)|*.*||";
                pDlg->Apply();
                break;
            case V_INT:
			    {
				    int val;
				    rTypeVal.GetValue(val);
				    pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
				    pDlg->_e1.SetWindowText(MKSTR("%d",val));
			    }
                break;
            case V_REAL:
			    {
				    REAL r;
				    rTypeVal.GetValue(r);
				    pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
				    pDlg->_e1.SetWindowText(MKSTR("%3.3f",r));
			    }
                break;
            case V_UV:
			    {
				    UV r;
				    rTypeVal.GetValue(r);
				    pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
				    pDlg->_e1.SetWindowText(MKSTR("%3.3f,%3.3f",r.u,r.v));
			    }
                break;
            case V_V3:
			    {
				    V3 r;
				    rTypeVal.GetValue(r);
				    pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
				    pDlg->_e1.SetWindowText(MKSTR("%3.3f,%3.3f,%3.3f",r.x,r.y,r.z));
			    }
                break;

            case V_CLR:
			    {
				    CLR	c;
				    rTypeVal.GetValue(c);
				    pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
				    pDlg->_e1.SetWindowText(MKSTR("%d,%d,%d,%d",c.r,c.g,c.b,c.a));
			    }
                break;
        }
        if(pDlg)
        {
              pDlg->_pNoty =  (PFNOTY)&DlgGameItem::NotyFoo;
        }
    }
    _updatingCtrls = FALSE;

//    m_lb.SetCurSel(0);
    m_lb.PostMessage(WM_KEYDOWN,VK_DOWN,0);
    m_lb.PostMessage(WM_KEYUP,VK_DOWN,0);

    //m_lb.OnSelchange() 
}


void    DlgGameItem::Retreive(SceItem* pItem, int selCount)
{
    if(pItem->_item != ITM_GAME)
        return;

    if(!_dirty || m_lb.GetCount()==0)return;
    if(_updatingCtrls)return;
	m_lb.SetCurSel(-1);
    CLbDlg* pDlg;

	if(pItem->_item != ITM_GAME)
		return;

    GameItem* pGi = (GameItem*)pItem;

    FOREACH(vvector<NameTypeValue>,pGi->_data, ppcp)
    {
        NameTypeValue& rTypeVal = *(ppcp);

        pDlg = m_lb.GetCurItemSel(rTypeVal._name);
        ASSERT(pDlg);
        switch(ppcp->_type)
        {
            case V_TEXT:
                pDlg->GetDlgItemText(E_1, (char*)rTypeVal._value, 32);
                break;
            case V_FILENAME:
                strcpy((char*)rTypeVal._value,pDlg->_szFile);
                break;
            case V_INT:
                {
                    char value[32];
                    pDlg->GetDlgItemText(E_1, value, 32);
                    int iv = atoi(value);
                    rTypeVal.SetValue(iv);
                }
                break;
            case V_REAL:
                {
                    char value[32];
                    pDlg->GetDlgItemText(E_1, value, 32);
                    REAL iv = atof(value);
                    rTypeVal.SetValue(iv);
                }
                break;
            case V_CLR:
                {
                    CLR clr = ParseCLR(pDlg, E_1);
                    rTypeVal.SetValue(clr);
                }
                break;
            case V_UV:
			    {
                    UV uv = ParseUV(pDlg, E_1);
                    rTypeVal.SetValue(uv);			    
                }
                break;
            case V_V3:
			    {
                    V3 v3 = ParseV3(pDlg, E_1);
                    rTypeVal.SetValue(v3);			    

			    }
                break;

        }
    }
}


//show categories props for given object class-cat
void    DlgGameItem::Update(ItemCat* pItem)
{
    _updatingCtrls = TRUE;
    _curCat = pItem;

    _pCurItem = 0;
    m_lb.Clean();

    SetDlgItemText(ST_CATNAME,_curCat->_catname);

    vvector<NameTypeValue>* pshowList = 0;

    pshowList = &_curCat->_classprops;
    CLbDlg* pDlg;
    FOREACH(vvector<NameTypeValue>, (*pshowList), ppcp)
    {
        NameTypeValue& rTypeVal = *ppcp;
        switch(ppcp->_type)
        {
            case V_TEXT:
                pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
                pDlg->_e1.SetWindowText((char*)rTypeVal._value);
                break;
            case V_FILENAME:
                pDlg = m_lb.AddItem(rTypeVal._name,DL_FILEPCK);
                pDlg->_szFile     = rTypeVal._value;
                pDlg->_bFileOpen  = 1;
                pDlg->_fileExt    = "*";
                pDlg->_fileFilter = "Files (*.*)|*.*|All Files (*.*)|*.*||";
                pDlg->Apply();
                break;
            case V_INT:
				{
					int val;
					rTypeVal.GetValue(val);
					pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
					pDlg->_e1.SetWindowText(MKSTR("%d",val));
				}
                break;
            case V_REAL:
				{
					REAL r;
					rTypeVal.GetValue(r);
					pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
					pDlg->_e1.SetWindowText(MKSTR("%3.2f",r));
				}
                break;
            case V_CLR:
				{
					CLR	c;
					rTypeVal.GetValue(c);
					pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
					pDlg->_e1.SetWindowText(MKSTR("%d,%d,%d,%d",c.r,c.g,c.b,c.a));
				}
                break;

             case V_UV:
				{
					UV	c;
					rTypeVal.GetValue(c);
					pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
					pDlg->_e1.SetWindowText(MKSTR("%2.3f,%2.3f",c.u,c.v));
				}
                break;
             case V_V3:
				{
					V3	c;
					rTypeVal.GetValue(c);
					pDlg = m_lb.AddItem(rTypeVal._name,DL_EDIT);
					pDlg->_e1.SetWindowText(MKSTR("%3.3f,%3.3f,%3.3f",c.x,c.y,c.z));
				}
                break;

        }
        if(pDlg)
        {
            pDlg->_pNoty =  (PFNOTY)&DlgGameItem::NotyFoo;
        }
        pDlg = 0;
    }
	m_lb.EnableWindow(0);
    _updatingCtrls = FALSE;
   
}


void    DlgGameItem::Retreive(ItemCat* pItem)
{
    if(_updatingCtrls)return;
	m_lb.SetCurSel(-1);
    CLbDlg* pDlg;
    FOREACH(vvector<NameTypeValue>, pItem->_classprops, ppcp)
    {
        NameTypeValue& rTypeVal = *ppcp;

        pDlg = m_lb.GetCurItemSel(rTypeVal._name);

        switch(ppcp->_type)
        {
            case V_TEXT:
                pDlg->GetDlgItemText(E_1, (char*)rTypeVal._value, 32);
                break;
            case V_FILENAME:
                strcpy((char*)rTypeVal._value,pDlg->_szFile);
                break;
            case V_INT:
                {
                    char value[32];
                    pDlg->GetDlgItemText(E_1, value, 32);
                    int iv = atoi(value);
                    rTypeVal.SetValue(iv);
                }
                break;
            case V_REAL:
                {
                    char value[32];
                    pDlg->GetDlgItemText(E_1, value, 32);
                    REAL iv = atof(value);
                    rTypeVal.SetValue(iv);
                }
                break;
            case V_CLR:
                {
                    CLR clr = ParseCLR(pDlg, E_1);
                    rTypeVal.SetValue(clr);
                }
            case V_UV:
                {
                    UV clr = ParseUV(pDlg, E_1);
                    rTypeVal.SetValue(clr);
                }
                break;
            case V_V3:
                {
                    V3 clr = ParseV3(pDlg, E_1);
                    rTypeVal.SetValue(clr);
                }
                break;
        }
    }

}


BOOL DlgGameItem::NotyFoo(CLbDlg* pDlg)
{
    Dirty();
	return TRUE;
}

void DlgGameItem::OnSelchangeProps() 
{
	
}

void DlgGameItem::OnDblclkPropscat() 
{
	
}

void DlgGameItem::OnKillFocus(CWnd* pNewWnd) 
{
	CBASEDLG::OnKillFocus(pNewWnd);

    m_lb.SetCurSel(-1);
    

    Dirty();
    if(_pCurItem)
	    Retreive(_pCurItem);	
	
}

void DlgGameItem::OnOk() 
{
    BARDLG()->OnApply();
}

void DlgGameItem::OnCancel() 
{
}

void DlgGameItem::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if(!bShow)
		BARDLG()->OnApply();
	CBASEDLG::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		_reroute=8;
		PostMessage(WM_REDRAWCHILDREN);
	}
}

LRESULT DlgGameItem::OnRepaint(WPARAM,LPARAM)
{
	while(--_reroute>0)
		PostMessage(WM_REDRAWCHILDREN);

	Invalidate();
	GetDlgItem(LB_PROPSCAT)->Invalidate();
	return 0;
}

void     DlgGameItem::Show(BOOL b)
{
    ShowWindow(b ? SW_SHOW : SW_HIDE);
    if(b)
    {
        DelayUpdateWindow(this->m_lb.m_hWnd);
        DelayUpdateWindow(this->m_hWnd);
    }
};
