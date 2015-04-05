// DlgBSetTop.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgBSetTop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgBSetTop dialog


DlgBSetTop::DlgBSetTop(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgBSetTop::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgBSetTop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgBSetTop::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgBSetTop)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgBSetTop, CBASEDLG)
	//{{AFX_MSG_MAP(DlgBSetTop)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgBSetTop message handlers

BOOL DlgBSetTop::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	
    ::memset(_pChilds,0,sizeof(_pChilds));

    m_tab._pParent=this;
    
    _dbp1.Create(DLG_B_SET,this);
    m_tab.AddDialog(&_dbp1, "Flags", 0, ST_PLACEDLG);
    _pChilds[0] = &_dbp1;

    long l;
    m_tab.OnSelchangeDlgs(0,&l);
	return TRUE;  
	              
}

void DlgBSetTop::OnDestroy() 
{
	CBASEDLG::OnDestroy();
}


void DlgBSetTop::UpdateMotionProps(Brush* pB, CMotion* pM, BOOL bRefresh)
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->UpdateMotionProps(pB, pM, bRefresh);
            break;
        }
    }
}

void DlgBSetTop::Update(Brush* pB, int selCount)
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->Update(pB, selCount);
            break;
        }
    }

}

void DlgBSetTop::Retreive(Brush* pB, int selCount)
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->Retreive(pB, selCount);
            break;
        }
    }

}

void DlgBSetTop::Activate(int ia)
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->Activate(ia);
            break;
        }
    }
}

void DlgBSetTop::Dirty()
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->Dirty();
            break;
        }
    }

}

void DlgBSetTop::Clean()
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->Clean();
            break;
        }
    }

}

void DlgBSetTop::OnItemSelected(int iSel)
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->OnItemSelected(iSel);
            break;
        }
    }

}

void DlgBSetTop::OnItemDoubleClicked(int iSel)
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->OnItemDoubleClicked(iSel);
            break;
        }
    }
}

void DlgBSetTop::OnCommand()
{
    for(int  i=0;_pChilds[i];i++)
    {
        if(_pChilds[i]->IsVisible())
        {
            _pChilds[i]->OnCommand();
            break;
        }
    }
}

