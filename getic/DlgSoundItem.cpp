// DlgSoundItem.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "geticgui.h"
#include "SceItem.h"
#include "SoundItem.h"
#include "DlgSoundItem.h"
#include "DlgBar.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgSoundItem dialog
#define         IS_TID(id_) (_touchedIDs.find(id_) !=  _touchedIDs.end())

DlgSoundItem::DlgSoundItem(CWnd* pParent /*=NULL*/)
	: CVertDlgDlg(DlgSoundItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgSoundItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgSoundItem::DoDataExchange(CDataExchange* pDX)
{
	CVertDlgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgSoundItem)
	DDX_Control(pDX, IDC_BUTTON1, m_cbb);
	DDX_Control(pDX, PB_PLAY, m_cb);
	DDX_Control(pDX, CB_AMBIENT, m_cba);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgSoundItem, CVertDlgDlg)
	//{{AFX_MSG_MAP(DlgSoundItem)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(CK_SPOT, OnSpot)
	ON_BN_CLICKED(CK_AMBIENT, OnAmbient)
	ON_BN_CLICKED(PB_PLAY, OnPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgSoundItem message handlers

void DlgSoundItem::OnButton1() 
{
    char szFilters[] = _T("Wave (*.wav)|*.wav|"
                          "Mpeg (*.mp3)|*.mp3|"  
                          "Midi (*.midi)|*.midi|"  
                          "All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("wav"), 0, OFN_HIDEREADONLY, szFilters);

	dlg.m_ofn.lpstrTitle = _T("Select  File");
	{
        dlg.m_ofn.lpstrInitialDir = theApp.DocsDir();

		if (theApp.DoFileDialog(dlg) != IDOK) 
		{
			REDIR();
            return;
		}
        PathHandler ph(dlg.m_ofn.lpstrFile);
        
        DOC()->DuplicateFile(dlg.m_ofn.lpstrFile, 1);

        SetDlgItemText(EF_SOUND, ph.File());
        REDIR();
	    theApp.DocsDir(ph.Path());
        OnAction();
	}
}

BOOL DlgSoundItem::OnInitDialog() 
{
	CVertDlgDlg::OnInitDialog();
    Enable(1);
    m_cba.SetCurSel(0);
	return TRUE;  
	              
}

void DlgSoundItem::OnDestroy() 
{
	CVertDlgDlg::OnDestroy();
	
	
}


void    DlgSoundItem::Update(SceItem* pItemG, int selCount)
{
    if(pItemG==0)
    {
        Enable(0);
        return;
    }
    if(pItemG->_item != _itmType)
        return;

    CSoundItem* pItem = (CSoundItem*)pItemG;
    if(pItem)
        Enable(1);
    _updatingCtrls = TRUE;

    SetDlgItemText(EF_SOUND,pItem->_soundfile.c_str());
    Sct(this,EF_RADIUS,pItem->_radius);
    Sct(this,EF_INTENSITY,pItem->_volume);
    
    CK_(CK_SPOT, EV_(pItem->_props & LIGHT_SPOT));
    OnSpot();
    Sct(this, EF_SPOTANGLE, pItem->_specAngle);

    Sct(this, EF_SDX, R2G(pItem->_euler.x));
    Sct(this, EF_SDY, R2G(pItem->_euler.y));
    Sct(this, EF_SDZ, R2G(pItem->_euler.z));



    CK_(CK_COLLIDE, EV_(pItem->_props & SND_COLLIDE));
    CK_(CK_REPEAT, EV_(pItem->_props & SND_REPEAT));
    
    CK_(CK_AMBIENT, EV_(pItem->_props & SND_AMBIENT_BITS));
    m_cba.SetCurSel(((pItem->_props & SND_AMBIENT_BITS) >> 0xF));

    _updatingCtrls = FALSE;
    ClearDirtyIDs();
}

//-----------------------------------------------------------------------------
void    DlgSoundItem::Retreive(SceItem* pItemG, int selCount)
{
    if(pItemG == 0)
        return;
    CSoundItem* pItem = (CSoundItem*)pItemG;

    if(IS_TID(EF_SOUND))
    {
        char sf[128];
        GetDlgItemText(EF_SOUND, sf,128);
        pItem->_soundfile = sf;
    }
    if(IS_TID(EF_RADIUS))
        pItem->_radius = ParseREAL(this,EF_RADIUS);
    if(IS_TID(EF_INTENSITY))
        pItem->_volume = ParseREAL(this,EF_INTENSITY);
    if(IS_TID(EF_SPOTANGLE))
        pItem->_specAngle= ParseREAL(this,EF_SPOTANGLE);
    
    IS_K(CK_SPOT) ? pItem->_props |= LIGHT_SPOT: pItem->_props &= ~LIGHT_SPOT; 
    IS_K(CK_COLLIDE) ? pItem->_props |= SND_COLLIDE: pItem->_props &= ~SND_COLLIDE; 
    IS_K(CK_REPEAT) ? pItem->_props |= SND_REPEAT: pItem->_props &= ~SND_REPEAT; 

    if(IS_K(CK_AMBIENT))
    {
        pItem->_props |= (m_cba.GetCurSel()<<0xF) & SND_AMBIENT_BITS;
    }
    else
    {
        pItem->_props &= ~SND_AMBIENT_BITS;
    }
}


void DlgSoundItem::OnSpot() 
{
    EN_(IDC_STATICA, EV_(IsDlgButtonChecked(CK_SPOT)));
    EN_(IDC_STATICD, EV_(IsDlgButtonChecked(CK_SPOT)));
    EN_(EF_SPOTANGLE, EV_(IsDlgButtonChecked(CK_SPOT)));
    EN_(EF_SDX, EV_(IsDlgButtonChecked(CK_SPOT)));
    EN_(EF_SDY, EV_(IsDlgButtonChecked(CK_SPOT)));
    EN_(EF_SDZ, EV_(IsDlgButtonChecked(CK_SPOT)));
}


void DlgSoundItem::OnAmbient() 
{
    EN_(CB_AMBIENT, EV_(IsDlgButtonChecked(CK_AMBIENT)));
}

void     DlgSoundItem::Show(BOOL b)
{
    ShowWindow(b ? SW_SHOW : SW_HIDE);
    if(b)
    {
        DelayUpdateWindow(m_hWnd);
    }
};



void DlgSoundItem::OnAction() 
{
    if(	BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}

void DlgSoundItem::OnPlay() 
{
    	
}
