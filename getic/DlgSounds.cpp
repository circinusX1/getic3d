// DlgSounds.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "DlgSounds.h"
#include "z_ed2Doc.h"
#include "SoundItem.h"
#include "DlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define 	WM_ITEMSEL2CLICK	(WM_USER+1236)
#define     WM_ITEMSEL			(WM_USER+1237)


DlgSounds::DlgSounds(CWnd* pParent /*=NULL*/):CBASEDLG(DlgSounds::IDD, pParent),_dirty(0),_ncursel(-1)
{
	//{{AFX_DATA_INIT(DlgSounds)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgSounds::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgSounds)
	DDX_Control(pDX, EF_VOL, m_efv);
	DDX_Control(pDX, EF_RAD, m_efr);
	DDX_Control(pDX, IDC_LISTASSOC, m_lasoc);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgSounds, CDialog)
	//{{AFX_MSG_MAP(DlgSounds)
	ON_WM_DESTROY()
	ON_BN_CLICKED(PB_PLAY, OnPlay)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(EF_RAD, OnChangeRad)
	ON_EN_CHANGE(EF_VOL, OnChangeVol)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ITEMSEL,OnItemSelected)
END_MESSAGE_MAP()


BOOL DlgSounds::OnInitDialog() 
{
	CDialog::OnInitDialog();
    m_list.SetHeadersNames("No","File Name", "Volume", "Radius", "Ref", 0);
    m_lasoc.SetHeadersNames("Type", "Name                 ",  0);
    m_efv.SetLimitText(5);
	m_efr.SetLimitText(5);
	return TRUE;  
}



void DlgSounds::OnDestroy() 
{
	CDialog::OnDestroy();
}


// like texture.
void DlgSounds::BuildFileList() 
{
  	m_list.DeleteAllItems();
    vvector<CSoundItem*>* psnds = SCENE().GetSounds();
    int nIndex =0;
    FOREACH(vvector<CSoundItem*>, (*psnds), ppsnd)
    {
        CSoundItem* psndi = *ppsnd;
        m_list.AddNewItem(MKSTR("%d",nIndex++),
                          psndi->_filename.c_str(), 
                          MKSTR("%.2f",psndi->_volume), 
                          MKSTR("%.2f",psndi->_radius),
                          MKSTR("%d",psndi->GetRefs()),0);
    }
}
	
LRESULT DlgSounds::OnItemSelected(WPARAM iItem, LPARAM lp) 
{
	if(lp == (LPARAM)&m_list)
	{
		_ncursel            = iItem;
		CString       cstxt = m_list.GetItemText(iItem,0);
		CSoundItem*   psnd  = SCENE().GetSound(::_ttoi(cstxt));
    
		Sct(this, EF_VOL, psnd->_volume);
		Sct(this, EF_RAD, psnd->_radius);

		PopulateAssociations(::_ttoi(cstxt));
	}

	return 0L;
}

void DlgSounds::OnPlay() 
{
	
}


void DlgSounds::OnOk() 
{
    CString       cstxt = m_list.GetItemText(_ncursel,0);
    CSoundItem*   psnd  = SCENE().GetSound(::_ttoi(cstxt));

    psnd->_volume = ParseREAL(this,EF_VOL);
    psnd->_radius = ParseREAL(this,EF_RAD);


    m_list.SetItem(_ncursel, 2, LVIF_TEXT, MKSTR("%.2f",psnd->_volume),0, 0, 0, 0);
    m_list.SetItem(_ncursel, 3, LVIF_TEXT, MKSTR("%.2f",psnd->_radius),0, 0, 0, 0);

	BARDLG()->Clean();
    
}


void DlgSounds::OnShowWindow(BOOL bShow, size_t nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(!bShow && _dirty && _ncursel>=0)
    {
        OnOk();
    }
    if(bShow)
    {
        BuildFileList() ;
		m_list.Invalidate();
    }

    BARDLG()->Clean();
}

void DlgSounds::OnChangeRad() 
{
	BARDLG()->Dirty();
}

void DlgSounds::OnChangeVol() 
{
    BARDLG()->Dirty();
}


void DlgSounds::PopulateAssociations(int index)
{
}




