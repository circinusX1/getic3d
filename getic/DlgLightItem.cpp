// DlgLightItem.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "DlgLightItem.h"
#include "DlgBar.h"
#include "LightBulb.h"
#include "z_ed2Doc.h"
#include "zedtypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgLightItem dialog


DlgLightItem::DlgLightItem(CWnd* pParent /*=NULL*/)
	: CVertDlgDlg(DlgLightItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgLightItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgLightItem::DoDataExchange(CDataExchange* pDX)
{
	CVertDlgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgLightItem)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgLightItem, CVertDlgDlg)
	//{{AFX_MSG_MAP(DlgLightItem)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(PB_COLOR, OnColor)
	ON_BN_CLICKED(CK_SPOT, OnSpot)
	ON_BN_CLICKED(CK_HALLO, OnHallo)
	ON_BN_CLICKED(CK_DLIGHT, OnDlight)
	ON_BN_CLICKED(PB_SPEC, OnSpec)
	ON_BN_CLICKED(CK_QUADRATIC, OnQuadratic)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(RB_PULSEI, RB_NORMALI, OnRadioButtons)
END_MESSAGE_MAP()

//CK_LPATCH
/////////////////////////////////////////////////////////////////////////////
// DlgLightItem message handlers

BOOL DlgLightItem::OnInitDialog() 
{
    _updatingCtrls = TRUE;
	CVertDlgDlg::OnInitDialog();

    _pbColor.SubclassDlgItem(PB_COLOR, this);
    _pbColorSpec.SubclassDlgItem(PB_SPEC, this);

    _pbColor.SetColor(RGB(255,255,255));
    _pbColorSpec.SetColor(RGB(0,0,0));

    ((CEdit*)GetDlgItem(EF_RADIUS))->SetLimitText(8);
    ((CEdit*)GetDlgItem(EF_INTENSITY))->SetLimitText(8);
    ((CEdit*)GetDlgItem(EF_SPOTANGLE))->SetLimitText(8);
    ((CEdit*)GetDlgItem(EF_SDX))->SetLimitText(8);
    ((CEdit*)GetDlgItem(EF_SDY))->SetLimitText(8);
    ((CEdit*)GetDlgItem(EF_SDZ))->SetLimitText(8);
    ((CEdit*)GetDlgItem(EF_MINHALLORADIUS))->SetLimitText(8);
    ((CEdit*)GetDlgItem(EF_MAXHALLORADIUS))->SetLimitText(8);

    ((CEdit*)GetDlgItem(EF_RADIUS))->SetWindowText(MKSTR("%2.2f", 800.f));
    ((CEdit*)GetDlgItem(EF_INTENSITY))->SetWindowText("500");
    ((CEdit*)GetDlgItem(EF_SPOTANGLE))->SetWindowText("0");
    ((CEdit*)GetDlgItem(EF_SDX))->SetWindowText("-90");
    ((CEdit*)GetDlgItem(EF_SDY))->SetWindowText("0.0");
    ((CEdit*)GetDlgItem(EF_SDZ))->SetWindowText("0.0");
    ((CEdit*)GetDlgItem(EF_MINHALLORADIUS))->SetWindowText("1.0");
    ((CEdit*)GetDlgItem(EF_MAXHALLORADIUS))->SetWindowText("32.0");

    ((CEdit*)GetDlgItem(EF_MINDIST))->SetWindowText("100.0");
    ((CEdit*)GetDlgItem(EF_MAXDIST))->SetWindowText("1600.0");

	
	OnDlight() ;
	OnHallo() ;
	OnSpot() ;

    _updatingCtrls = FALSE;
	return TRUE;  
}

void DlgLightItem::OnCancel() 
{
}

void DlgLightItem::OnDestroy() 
{
	CVertDlgDlg::OnDestroy();
}

void DlgLightItem::OnOk() 
{
    BARDLG()->OnApply();
}

void DlgLightItem::OnSpec() 
{
	CColorDialog    d( _pbColorSpec._color, CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal()==IDOK)
    {
        _pbColorSpec.SetColor(d.GetColor());
        _pbColorSpec.Invalidate();

		if(!_updatingCtrls)
		{
			Dirty();
			BARDLG()->OnApply();
		}
        OnAction() ;
    }
}


void DlgLightItem::OnColor() 
{
	CColorDialog    d( _pbColor._color, CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal()==IDOK)
    {
        _pbColor.SetColor(d.GetColor());
        _pbColor.Invalidate();

		if(!_updatingCtrls)
		{
			Dirty();
			BARDLG()->OnApply();
		}
        OnAction() ;
    }
}

void    DlgLightItem::Update(SceItem* pItemG, int selCount)
{
    if(pItemG->_item != _itmType)
        return;

     if(pItemG==0)
     {
         Enable(0);
         return;
     }
     Enable(1);

    _updatingCtrls = TRUE;
    
    _pCurItem = pItemG;
    CLightBulb* pItem = (CLightBulb*)pItemG;

    Sct(this, EF_RADIUS , pItem->_radius);
    Sct(this, EF_INTENSITY , pItem->_lmIntensity);
    
    _pbColor.SetColor(RGB(pItem->_colorD.r, pItem->_colorD.g, pItem->_colorD.b));
    _pbColor.Invalidate();

    _pbColorSpec.SetColor(RGB(pItem->_colorS.r, pItem->_colorS.g, pItem->_colorS.b));
    _pbColorSpec.Invalidate();


    CK_(CK_SPOT, EV_(pItem->_props & LIGHT_SPOT));
    CK_(CK_QUADRATIC, EV_(pItem->_atten[0]==2));

    
    Sct(this, EF_SPOTANGLE, pItem->_specAngle);
    Sct(this, EF_SDX, R2G(pItem->_euler.x));
    Sct(this, EF_SDY, R2G(pItem->_euler.y));
    Sct(this, EF_SDZ, R2G(pItem->_euler.z));
    OnSpot();
    
    CK_(CK_HALLO, EV_(pItem->_props & LIGHT_HASHALLO));
    Sct(this, EF_MINHALLORADIUS, pItem->_halloRadMin);
    Sct(this, EF_MAXHALLORADIUS, pItem->_halloRadMax);
    Sct(this, EF_MINDIST, pItem->_minDist);
    Sct(this, EF_MAXDIST, pItem->_maxDist);


    OnHallo();
    

    CK_ (CK_DLIGHT, EV_(pItem->_props & LIGHT_HASDYNA));
    OnDlight();
    if(pItem->_props & LIGHT_HASDYNA)
    {
        if(LIGHT_DY_PULSE  & pItem->_props)
            CheckRadioButton(RB_PULSEI, RB_NORMALI, RB_PULSEI);
        else if(LIGHT_DY_RAND   & pItem->_props)
            CheckRadioButton(RB_PULSEI, RB_NORMALI, RB_RANDOMI);
        else if(LIGHT_DY_WAVY   & pItem->_props)
            CheckRadioButton(RB_PULSEI, RB_NORMALI, RB_VAWYI);
        else if(LIGHT_DY_FLASHY & pItem->_props)
            CheckRadioButton(RB_PULSEI, RB_NORMALI, RB_FLASHYI);
        else 
            CheckRadioButton(RB_PULSEI, RB_NORMALI, RB_NORMALI);
    }
    OnDlight();
    _updatingCtrls = FALSE;
    ClearDirtyIDs();
}

//-----------------------------------------------------------------------------
void    DlgLightItem::Retreive(SceItem* pItemG,int selCount)
{
    if(!_dirty)return;
	
	if(pItemG->_item != _itmType)
        return;

	if(pItemG->_item != ITM_LIGTBULB)
    {
        ASSERT(0);
		return;
    }

    CLightBulb* pItem = (CLightBulb*)pItemG;
    
    if(_touchedIDs.find(EF_RADIUS) != _touchedIDs.end())
        pItem->_radius   = ParseREAL(this, EF_RADIUS);

    if(_touchedIDs.find(EF_INTENSITY) != _touchedIDs.end())
    {
        pItem->_lmIntensity = ParseREAL(this, EF_INTENSITY);
        if(pItem->_lmIntensity > 1000) 
            pItem->_lmIntensity = 1000;
        if(pItem->_lmIntensity < -1000) 
            pItem->_lmIntensity = -1000;

    }

    //if(_touchedIDs.find(PB_COLOR) != _touchedIDs.end())
    {
        pItem->_colorD.r = GetRValue(_pbColor._color); 
        pItem->_colorD.g = GetGValue(_pbColor._color);
        pItem->_colorD.b = GetBValue(_pbColor._color);

        if(!GetDlgItem(PB_SPEC)->IsWindowVisible())
        {
            pItem->_colorS.r = pItem->_colorD.r; 
            pItem->_colorS.g = pItem->_colorD.g;
            pItem->_colorS.b = pItem->_colorD.b;
        }

        pItem->_colorD.a = 255;
    }

    //if(_touchedIDs.find(PB_SPEC) != _touchedIDs.end())
    {
        pItem->_colorS.r = GetRValue(_pbColorSpec._color); 
        pItem->_colorS.g = GetGValue(_pbColorSpec._color);
        pItem->_colorS.b = GetBValue(_pbColorSpec._color);
    }
/*
    if(pItem->_colorS == ZWHITE && pItem->_colorD != ZWHITE)
    {
        
    
    }
*/
    
    if(_touchedIDs.find(CK_SPOT) != _touchedIDs.end())
    {
        if(IS_K(CK_SPOT))
        {
            pItem->_props     |= LIGHT_SPOT;
            pItem->_props     &= ~LIGHT_POINT;
        }
        else
        {
            pItem->_props     &= ~LIGHT_SPOT;
            pItem->_props     |= LIGHT_POINT;
        }
    }

    if(_touchedIDs.find(CK_QUADRATIC) != _touchedIDs.end())
    {
        pItem->_atten[0] = IsDlgButtonChecked(CK_QUADRATIC) ? 2 : 1;
    }

    if(pItem->_props     & LIGHT_SPOT)
    {
        pItem->_specAngle = ParseREAL(this, EF_SPOTANGLE);
        //pItem->_euler     = V3(-PIPE2,0,0);

    }
    else
    {
        pItem->_specAngle = 0;
    }
    
    if(_touchedIDs.find(CK_HALLO) != _touchedIDs.end())
    {
        if(IS_K(CK_HALLO))
        {
            pItem->_props      |= LIGHT_HASHALLO;
        }
        else 
        {
            pItem->_props      &= ~LIGHT_HASHALLO;
        }
    }
    
    if( pItem->_props & LIGHT_HASHALLO)
    {
        pItem->_halloRadMin = ParseREAL(this, EF_MINHALLORADIUS);
        pItem->_halloRadMax = ParseREAL(this, EF_MAXHALLORADIUS);
        pItem->_minDist = ParseREAL(this, EF_MINDIST);
        pItem->_maxDist = ParseREAL(this, EF_MAXDIST);
    }
   
    //
    // texture is applyed when double clicking and is not beeing updated here
    //
    if(_touchedIDs.find(CK_DLIGHT) != _touchedIDs.end())
    {
        if(IS_K(CK_DLIGHT))
        {
            pItem->_props  |= LIGHT_HASDYNA;
        }
        else
        {
            pItem->_props  &= ~LIGHT_HASDYNA;
        }
    }

    if(pItem->_props  & LIGHT_HASDYNA)
    {
        if(IS_K(RB_PULSEI))   
            pItem->_props  |= LIGHT_DY_PULSE; 
        else
            pItem->_props  &= ~LIGHT_DY_PULSE; 

        if(IS_K(RB_RANDOMI))  
            pItem->_props  |= LIGHT_DY_RAND;  
        else
            pItem->_props  &= ~LIGHT_DY_RAND; 

        if(IS_K(RB_VAWYI))    
            pItem->_props  |= LIGHT_DY_WAVY;  
        else
            pItem->_props  &= ~LIGHT_DY_WAVY; 

        if(IS_K(RB_FLASHYI))  
            pItem->_props  |= LIGHT_DY_FLASHY;
        else 
            pItem->_props  &= ~LIGHT_DY_FLASHY; 
    }
    if(GLight)   
        DOC()->Invalidate();
}


void DlgLightItem::OnSpot() 
{
    BOOL b = IsDlgButtonChecked(CK_SPOT);
    DI(IDC_STATICA)->EnableWindow(b);
    DI(IDC_STATICD)->EnableWindow(b);
    DI(EF_SPOTANGLE)->EnableWindow(b);
    DI(EF_SDX)->EnableWindow(b);
	DI(EF_SDY)->EnableWindow(b);
	DI(EF_SDZ)->EnableWindow(b);
	OnAction();
}

void DlgLightItem::OnHallo() 
{
	BOOL b = IsDlgButtonChecked(CK_HALLO);
    DI(IDC_STATICMR)->EnableWindow(b);
    DI(IDC_STATICR)->EnableWindow(b);
    DI(EF_MINHALLORADIUS)->EnableWindow(b);
    DI(EF_MAXHALLORADIUS)->EnableWindow(b);
    DI(EF_MINDIST)->EnableWindow(b);
    DI(EF_MAXDIST)->EnableWindow(b);
	DI(IDC_STATICMR2)->EnableWindow(b);
	DI(IDC_STATICR2)->EnableWindow(b);
		
    OnAction() ;
	
}


void DlgLightItem::OnAction() 
{
    if(	BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}

void DlgLightItem::OnRadioButtons(size_t iCmd)
{
	OnAction() ;
}

void DlgLightItem::OnDlight() 
{
	BOOL b = IsDlgButtonChecked(CK_DLIGHT);
    DI(RB_PULSEI)->EnableWindow(b);
    DI(RB_RANDOMI)->EnableWindow(b);
    DI(RB_VAWYI)->EnableWindow(b);
    DI(RB_FLASHYI)->EnableWindow(b);
    DI(RB_NORMALI)->EnableWindow(b);
    OnAction() ;
}


void     DlgLightItem::Show(BOOL b)
{
    ShowWindow(b ? SW_SHOW : SW_HIDE);
    if(b)
    {
        DelayUpdateWindow(m_hWnd);
    }
};

void DlgLightItem::OnQuadratic() 
{
	BOOL b = IsDlgButtonChecked(CK_QUADRATIC);
    OnAction() ;
}

