// DlgNewItem.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "mainfrm.h"
#include "z_ed2View.h"
#include "DlgNewItem.h" //
#include "bspfilestr.h"
#include "LightBulb.h"
#include "z_ed2Doc.h"
#include "zedtypes.h"
#include "dlgbar.h"
#include "TriggerItem.h"
#include "GameItem.h"
#include "SoundItem.h"
#include "StartPlayItem.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// DlgNewItem dialog


DlgNewItem::DlgNewItem(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgNewItem::IDD, pParent),_pCurItem(0)
{
	//{{AFX_DATA_INIT(DlgNewItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgNewItem::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgNewItem)
	DDX_Control(pDX, IDOK, m_o);
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, LB_ITEMS, _lbItems);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgNewItem, CBASEDLG)
	//{{AFX_MSG_MAP(DlgNewItem)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_LBN_SELCHANGE(LB_ITEMS, OnSelchangeItems)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgNewItem message handlers

BOOL DlgNewItem::OnInitDialog() 
{
    --SceItem::_GItemID;
	CBASEDLG::OnInitDialog();

	
	int is = _lbItems.AddString(_T("Light Bulb"));
    _lbItems.SetItemData(is, (size_t)ITM_LIGTBULB);


    is = _lbItems.AddString(_T("Trigger"));
    _lbItems.SetItemData(is, (size_t) ITM_TRIGER);


    is = _lbItems.AddString(_T("Sound Source"));
    _lbItems.SetItemData(is, (size_t) ITM_SNDSOURCE);


    is = _lbItems.AddString(_T("Actor Start Play"));
    _lbItems.SetItemData(is, (size_t) ITM_STARTPLAY);



    // add game items if some have been found
    if(DOC()->_scene.HasCats())
    {
        vvector<ItemCat>* pcats = DOC()->_scene.GetCats();
        FOREACH(vvector<ItemCat>, (*pcats), pcat)
        {
            is = _lbItems.AddString(pcat->_catname);
            _lbItems.SetItemData(is, (size_t) ITM_GAME);
        }
    }

    

    ////<NEW_ITEM>
	_dlgSoundItem.BlockBarUpdate();
    _dlgSoundItem.SetItmType(ITM_SNDSOURCE);
	_dlgSoundItem.Create( DLG_I_SOUND, this);


	_dlgLightItem.BlockBarUpdate();
    _dlgLightItem.SetItmType(ITM_LIGTBULB);
	_dlgLightItem.Create( DLG_I_LIGHT, this);

    _dlgTriggerItem.BlockBarUpdate();
    _dlgTriggerItem.SetItmType(ITM_TRIGER);
    _dlgTriggerItem.Create( DLG_I_TRIGER, this);

    _dlgGameItem.BlockBarUpdate();
    _dlgGameItem.SetItmType(ITM_GAME);
    _dlgGameItem.Create( DLG_I_GAMEITM, this);
    
    _dlgStartItem.BlockBarUpdate();
    _dlgStartItem.SetItmType(ITM_STARTPLAY);
    _dlgStartItem.Create( DLG_I_STARTPLAY, this);



	CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);
	

	Sct(this, EF_POSX,  pTY->_shiftx);
    Sct(this, EF_POSY,  pTZ->_shifty);
    Sct(this, EF_POSZ, -pTY->_shifty);
	
    _pCurDlg = 0;
    _lbItems.SetCurSel(DOC()->_lastItmSel);
    OnSelchangeItems() ;
	return TRUE;  // return TRUE unless you set the focus to a control
}


void DlgNewItem::OnDestroy() 
{
	_dlgLightItem.DestroyWindow();
    _dlgTriggerItem.DestroyWindow();
    _dlgGameItem.DestroyWindow();
  	_dlgSoundItem.DestroyWindow();
    _dlgStartItem.DestroyWindow();

	CBASEDLG::OnDestroy();
}

void DlgNewItem::UpdateFromCat(GameItem* pitem)
{
    pitem->_data.clear();
    ItemCat* pcat = DOC()->_scene.GetCat(pitem->_catname);
    if(pcat)
    {
        pitem->_data = pcat->_objprops;
        // from now on we gong to change 'pitem->_data'
    }
}


void DlgNewItem::UpdateItemPosition(SceItem* pItem)
{
    V3 pos;
    pos.x = ParseREAL(this, EF_POSX);
    pos.y = ParseREAL(this, EF_POSY);
    pos.z = ParseREAL(this, EF_POSZ);

    pItem->SetPos(pos);
}

void DlgNewItem::OnOk() 
{
    int     cbSel = _lbItems.GetCurSel();
    EITEM   ei = (EITEM)_lbItems.GetItemData(cbSel);
    char   catname[32];
    _lbItems.GetText(cbSel, catname);

    DOC()->_lastItmSel = cbSel;

    switch(ei)
    {
        case ITM_LIGTBULB:
            {
				_pCurItem = new CLightBulb();
                UpdateItemPosition(_pCurItem);

				_dlgLightItem.Dirty();
				_dlgLightItem.Retreive(_pCurItem);
				DOC()->AddItem(_pCurItem);
            }
            break;
        case ITM_TRIGER:
            {
                _pCurItem =  new TriggerItem();
                UpdateItemPosition(_pCurItem);
                
                _dlgTriggerItem.Dirty();
                _dlgTriggerItem.Retreive(_pCurItem);

                if(((TriggerItem*)_pCurItem)->_pBrush)
                {
                    _pCurItem->SetPos(((TriggerItem*)_pCurItem)->_pBrush->_box.GetCenter()+VY*128);
                    
                }
                _pCurItem->InitCommit();
                DOC()->AddItem(_pCurItem);
            }
            break;
        case ITM_GAME:
            {
                // dispatch is done by item category name name
                _pCurItem = new GameItem(catname);

                UpdateItemPosition(_pCurItem);

                _dlgGameItem.Dirty();
                _dlgGameItem.Retreive(&_defgi);
                ((GameItem*)_pCurItem)->_data = _defgi._data;
                _defgi._data.clear();
                _pCurItem->InitCommit();
                DOC()->AddItem(_pCurItem);
            }
            break;

        case ITM_SNDSOURCE:
            {
                _pCurItem = new CSoundItem();
                UpdateItemPosition(_pCurItem);
				_dlgSoundItem.Dirty();
				_dlgSoundItem.Retreive(_pCurItem);
				DOC()->AddItem(_pCurItem);
            }
            break;
        case ITM_STARTPLAY: //_dlgStartItem
            {
                _pCurItem = new CStartPlayItem();
                UpdateItemPosition(_pCurItem);
				_dlgStartItem.Dirty();
				_dlgStartItem.Retreive(_pCurItem);
				DOC()->AddItem(_pCurItem);

            }
            break;

   }


	CBASEDLG::OnOK();
	BARDLG()->Clean();
}

void DlgNewItem::OnCancel() 
{
	CBASEDLG::OnCancel();
}

void DlgNewItem::OnButton1() 
{
	
}

void DlgNewItem::OnSelchangeItems() 
{
    int     cbSel = _lbItems.GetCurSel();
    EITEM   ei = (EITEM)_lbItems.GetItemData(cbSel);
	RECT	rt; 

 	GetDlgItem(ST_PLACEDLG)->GetWindowRect(&rt);
	ScreenToClient(&rt);
    char   catname[32];
    _lbItems.GetText(cbSel, catname);

	if(_pCurDlg)
		_pCurDlg->ShowWindow(SW_HIDE);

    switch(ei)
    {
        case ITM_SNDSOURCE:
            {
				_dlgSoundItem.MoveWindow(rt.left,rt.top,
								  rt.right-rt.left, 
		    					  rt.bottom-rt.top,1);
				_dlgSoundItem.ShowWindow(SW_SHOW);
				_pCurDlg = &_dlgSoundItem;

            }
            break;
        case ITM_LIGTBULB:
            {
				_dlgLightItem.MoveWindow(rt.left,rt.top,
								  rt.right-rt.left, 
		    					  rt.bottom-rt.top,1);
				_dlgLightItem.ShowWindow(SW_SHOW);
				_pCurDlg = &_dlgLightItem;


            }
            break;
        case ITM_TRIGER:
            {
				_dlgTriggerItem.MoveWindow(rt.left,rt.top,
								  rt.right-rt.left, 
		    					  rt.bottom-rt.top,1);
				_dlgTriggerItem.ShowWindow(SW_SHOW);
				_pCurDlg = &_dlgTriggerItem;
            }

            break;
      case ITM_STARTPLAY:
            {
				_dlgStartItem.MoveWindow(rt.left,rt.top,
								  rt.right-rt.left, 
		    					  rt.bottom-rt.top,1);
				_dlgStartItem.ShowWindow(SW_SHOW);
				_pCurDlg = &_dlgStartItem;
            }

            break;
        case ITM_GAME:
            {
				_dlgGameItem.MoveWindow(rt.left,rt.top,
								  rt.right-rt.left, 
		    					  rt.bottom-rt.top,1);
				_dlgGameItem.ShowWindow(SW_SHOW);
				_pCurDlg = &_dlgGameItem;

                ItemCat* pCat = DOC()->_scene.GetCat(catname);
                if(pCat)
                {
                    strcpy(_defgi._catname , pCat->_catname);
                    _defgi._item    = ITM_GAME;
                    UpdateFromCat(&_defgi);
                    _dlgGameItem.Update(&_defgi);
                }
            }
            break;

    }
    _pCurDlg->Invalidate();
	
}
