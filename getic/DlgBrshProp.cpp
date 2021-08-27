// Author : Marius C.
//---------------------------------------------------------------------------------------
#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "DlgBrshProp.h"
#include "DlgBar.h"
#include "Brush.h"
#include "Motion.h"
#include "z_ed2doc.h"
#include "DlgMotionState.h"

//---------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
DlgBrshProp::DlgBrshProp(CWnd* pParent /*=NULL*/)
	: CVertDlgDlg(DlgBrshProp::IDD, pParent),_pCurBrush(0),
                                        _scrollPage(1),_selCount(0),_paneNo(0)
{
	//{{AFX_DATA_INIT(DlgBrshProp)
	//}}AFX_DATA_INIT
}


void DlgBrshProp::DoDataExchange(CDataExchange* pDX)
{
	CVertDlgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgBrshProp)
	DDX_Control(pDX, PB_ADD, m_badd);
//	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, T3_Z, m_taz);
	DDX_Control(pDX, T3_X, m_tax);
	DDX_Control(pDX, CB_GROUP, _cbGroup);
	DDX_Control(pDX, EF_NAME, _efName);
	DDX_Control(pDX, IDC_LIST1, _lcMove);
	DDX_Control(pDX, EF_STEPSS, _efSteps);
	DDX_Control(pDX, CB_CONTENT, _cbContent);
	//}}AFX_DATA_MAP
     
}


BEGIN_MESSAGE_MAP(DlgBrshProp, CVertDlgDlg)
	//{{AFX_MSG_MAP(DlgBrshProp)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(CK_DETAIL, OnDetail)
	ON_BN_CLICKED(CK_DYNAMIC, OnDynamic)
	ON_BN_CLICKED(CK_CUT, OnCut)
	//ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_CLOSEUP(CB_CONTENT, OnCloseupContent)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(PB_ADD, OnAdd)
	ON_BN_CLICKED(CK_SKYDOM, OnSkydom)
	ON_BN_CLICKED(CK_NOCOLL, OnNocoll)
	ON_BN_CLICKED(CK_ROTATE, OnRotate)
	ON_CBN_CLOSEUP(CB_GROUP, OnCloseupGroup)
	ON_BN_CLICKED(PB_P2, OnP2)
	ON_BN_CLICKED(PB_P1, OnP1)
	ON_BN_CLICKED(PB_P3, OnP3)
	ON_BN_CLICKED(PB_P32, OnP32)
	ON_BN_CLICKED(CK_TRIANGS, OnTriangs)
	ON_BN_CLICKED(CK_NOBSP, OnNobsp)
	ON_BN_CLICKED(CK_DISABLE, OnDisable)
	ON_BN_CLICKED(PB_SCRIPT, OnScript)
	ON_BN_CLICKED(CK_CUTALL, OnCutall)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ITEMSEL2CLICK,OnItemDoubleClicked)
	ON_MESSAGE(WM_ITEMSEL,OnItemSelected)

END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
BOOL DlgBrshProp::OnInitDialog()  
{
    _updatingCtrls = TRUE;

	CVertDlgDlg::OnInitDialog();
    _efSteps.SetLimitText(2);
    _efName.SetLimitText(15);
    m_taz.SetLimitText(6);
    m_tax.SetLimitText(6);
    CheckRadioButton(RB_TSHIFT,RB_TVIBR, RB_TSHIFT);	
    
    
    _lcMove.SetHeadersNames("Node","  X ","  Y ","  Z ","T-Off","T-On",0);

    CheckDlgButton(CK_GOR, BST_CHECKED);

    for(int i=0;i<32;i++)
        _cbGroup.AddString(MKSTR("%d",i));

    CheckDlgButton(CK_L0,BST_CHECKED);
    

	return TRUE;  
}

//-----------------------------------------------------------------------------
void    DlgBrshProp::Update(Brush* pBrush, int selCount)
{
    if(!m_hWnd )
    {
        return;
    }

    _pCurBrush = pBrush;
    if(0 == pBrush)
    {
        ClearDirtyIDs();
        _selCount      = 0;
        Enable(0);
        DisableAllCtrls();
        _lcMove.DeleteAllItems();        
        return ;
    }
    Enable(1);
    
    selCount       =  DOC()->_pSelBrushes.size();
    _selCount      = selCount;
    _updatingCtrls = TRUE;

    size_t& cflags = pBrush->_brushflags;
    size_t& cprops = pBrush->_brushprops;
    BOOL   enable=1;

    if((cflags & BRSH_BIGTERRAIN) && _selCount==1)
        enable=0;

    char tw[4];
    CWnd*   pWnd = this;
    int     relation = GW_CHILD|GW_HWNDFIRST;
    while(pWnd = pWnd->GetWindow(relation))
    {
        size_t iD = pWnd->GetDlgCtrlID();
        pWnd->GetWindowText(tw, 3);

        relation = GW_HWNDNEXT;
        if(iD == CB_GROUP)
            continue;
        if(iD == EF_NAME)
            continue;
        if(iD == CK_TERRAIN)
            continue;
        if(iD == EF_XC)
            continue;
        if(iD == EF_YC)
            continue;
        if(iD == EF_ZC)
            continue;
        if(iD == EF_W)
            continue;
        if(iD == EF_H)
            continue;
        if(iD == EF_D)
            continue;

        if(tw[0]=='+' || tw[0]=='-')
		{
			pWnd->EnableWindow(1);
            continue;
		}
        pWnd->EnableWindow(enable);
    }


    if((cflags  & BRSH_BIGTERRAIN) && _selCount==1)
    {
        if(SCENE().Terrain().Exist())
        {
            CBigTerrain& bt = SCENE().Terrain();
            Texs&    t  = bt.Texs();

            EnableGroup(ST_BIGTERR,1);
            EnableGroup(ST_DETAIL,0);
            EnableGroup(ST_SKIDOM,0);
        

            CK_(CK_NOTILE,  (cflags & TERR_UTT0));
            CK_(CK_NOTILE1, (cflags & TERR_UTT1));
            CK_(CK_NOTILE2, (cflags & TERR_UTT2));
            CK_(CK_NOTILE4, (cflags & TERR_UTT3));
            //CK_(CK_GOR,     (cflags & TERR_GOR));
            CK_(CK_FULLBR,  (cflags & TERR_FULLBRGHT));
            CK_(CK_NOLM,    (cflags & TERR_NOLIGHT ));
            CK_(CK_NOCOLLIDE,    (cflags & TERR_NOCOLIDE ));

            EN_(CK_NOTILE,  (int)t[0]);        
            EN_(CK_NOTILE1, (int)t[1]);        
            EN_(CK_NOTILE2, (int)t[2]);        
            EN_(CK_NOTILE4, (int)t[3]);        


            if(bt.n_stage & TERR_TA_SHIFT)
                CheckRadioButton(RB_TSHIFT ,RB_TVIBR, RB_TSHIFT);
            else
            if(bt.n_stage & TERR_TA_OSC)
                CheckRadioButton(RB_TSHIFT ,RB_TVIBR, RB_TVIBR);

            CK_(CK_L0, EV_(0x1 & bt.n_stage));
            CK_(CK_L1, EV_(0x2 & bt.n_stage));
            CK_(CK_L2, EV_(0x4 & bt.n_stage));
            CK_(CK_L3, EV_(0x8 & bt.n_stage));

            m_taz.SetWindowText(MKSTR("%1.4f", bt.t_anim[0].u));
            m_tax.SetWindowText(MKSTR("%1.4f", bt.t_anim[0].v));
        }
    }
    else
    {
        EnableGroup(ST_BIGTERR,0);
    }


    _efName.SetWindowText(pBrush->_name);

    Box& bbox = pBrush->_box;
    Sct(this, EF_XC, bbox.GetCenter().x/GMeters);
    Sct(this, EF_YC, bbox.GetCenter().y/GMeters);
    Sct(this, EF_ZC, bbox.GetCenter().z/GMeters);

    Sct(this, EF_W, bbox.GetExtends().x/GMeters);
    Sct(this, EF_H, bbox.GetExtends().y/GMeters);
    Sct(this, EF_D, bbox.GetExtends().z/GMeters);

    GetDlgItem(EF_XC)->EnableWindow(selCount==1);
    GetDlgItem(EF_YC)->EnableWindow(selCount==1);
    GetDlgItem(EF_ZC)->EnableWindow(selCount==1);

    GetDlgItem(EF_H)->EnableWindow(selCount==1);
    GetDlgItem(EF_D)->EnableWindow(selCount==1);
    GetDlgItem(EF_W)->EnableWindow(selCount==1);
    GetDlgItem(EF_NAME)->EnableWindow(selCount==1);
    GetDlgItem(CB_GROUP)->EnableWindow(selCount>=1);



     _cbGroup.SelectString(-1, MKSTR("%d",pBrush->_group));
    if(cflags & BRSH_WIREBOX) 
    {
        _updatingCtrls=0;
        return;
    }

	EN_(CK_DETAIL,1);
	EN_(CK_SKYDOM,1);
    CK_(CK_DISABLE, (cflags & BRSH_DISABLED));
   

    if(!(cflags & BRSH_SOLID)) 
    {
        EN_(CK_CUTALL,1);
        if(cflags & BRSH_CUTALL)
        {
            CK_(CK_CUTALL, 1);
        }
        else
        {
            CK_(CK_CUT, 1);
        }
        OnCut();

        if(pBrush->_pSldBrushes.size() || selCount!=1)
            _cbGroup.EnableWindow(0);
        else
            _cbGroup.EnableWindow(1);
        OnDynamic();
        

        _updatingCtrls = FALSE;
        return;
    }
    CK_(CK_CUTALL, 0);
    CK_(CK_CUT, 0);
    EN_(CK_CUTALL,0);
    EN_(CK_CUT,0);
    EN_(CK_TERRAIN,0);
    EN_(CK_TRIANGS,0);
    EN_(CK_SINGLETON,0);
    CK_(CK_TERRAIN, EV_(cflags & BRSH_CSGTERRAIN));
    CK_(CK_DETAIL,EV_(cflags & BRSH_DETAIL));
    CK_(CK_NOCOLL,  EV_(cprops & MODEL_NOCOLIDE) );
    CK_(CK_DYNAMIC, EV_(cprops & MODEL_DYNAMIC) );


    OnDetail();
    if((cflags & BRSH_DETAIL))
    {

        EN_(CK_NOBSP,1);
        CK_(CK_TRIANGS, EV_(cflags & BRSH_TRIANGLES));
        CK_(CK_NOBSP,   EV_(cflags & BRSH_DONTBSP));

        CK_(CK_CYLBB, EV_(cprops & MODEL_IS_CYLBB));     
        CK_(CK_SPHBB, EV_(cprops & MODEL_IS_SPHBB));   

        CK_(CK_DETACHED, EV_(cflags & BRSH_DETACHED));   
        OnDynamic();
        int isel = (cprops & MODEL_CONTENT_MASK)>>4;
        _cbContent.SetCurSel(isel);

        if(_pCurBrush->_pMotion != 0)
        {
            CMotion* pM = _pCurBrush->_pMotion;
			UpdateMotionProps(_pCurBrush, pM, TRUE);

			GetDlgItem(EF_STEPSS)->EnableWindow(1);
            GetDlgItem(CK_LOOP)->EnableWindow(1);
			GetDlgItem(PB_ADD)->EnableWindow(1);
            _lcMove.EnableWindow(1);

            SetDlgItemInt(EF_STEPSS, pM->GetNodesCount());
            CK_(CK_LOOP, (pM->IsLoop() & 0x1 ));
        }
        else
        {

            _lcMove.DeleteAllItems();
			GetDlgItem(EF_STEPSS)->EnableWindow(0);
            GetDlgItem(CK_LOOP)->EnableWindow(0);
			GetDlgItem(PB_ADD)->EnableWindow(0);
            _lcMove.EnableWindow(0);
			
			CK_(CK_DYNAMIC, 0);
			CK_(CK_LOOP, 0);
			cprops &= ~MODEL_DYNAMIC; 
			OnDynamic();
        }
    }
    else
    {
        OnDynamic();

        EN_(CK_NOBSP,0);
        CK_(CK_NOBSP,0);
    }

	CK_(CK_SKYDOM,EV_(cprops & MODEL_IS_SKYDOM));
    CK_(CK_ROTATE, EV_(cflags & BRSH_ROTATES));
	OnSkydom();
	
    if((cprops & MODEL_IS_SKYDOM))
    {
        Sct(this, EF_XR, _pCurBrush->_rotSteps.x);
        Sct(this, EF_YR, _pCurBrush->_rotSteps.y);
        Sct(this, EF_ZR, _pCurBrush->_rotSteps.z);
    }

    ((CEdit*)GetDlgItem(EF_XC))->SetModify(0);
    ((CEdit*)GetDlgItem(EF_YC))->SetModify(0);
    ((CEdit*)GetDlgItem(EF_ZC))->SetModify(0);

    ((CEdit*)GetDlgItem(EF_W))->SetModify(0);
    ((CEdit*)GetDlgItem(EF_H))->SetModify(0);
    ((CEdit*)GetDlgItem(EF_D))->SetModify(0);
    _efName.SetModify(0);

    if(GUtex!=1)
    {
        if(_pCurBrush->_cf._tm==TM_LIN) 
        {
            CheckDlgButton(RB_LIN, BST_CHECKED);
            CheckDlgButton(RB_SPH, BST_UNCHECKED);
        }
        else
        {
            CheckDlgButton(RB_SPH, BST_CHECKED);
            CheckDlgButton(RB_LIN, BST_UNCHECKED);
        }
    }
    else
    {
        CheckDlgButton(RB_LIN, BST_UNCHECKED);
        CheckDlgButton(RB_SPH, BST_UNCHECKED);
    }
    
    _updatingCtrls = FALSE;
    ClearDirtyIDs();
}

//-----------------------------------------------------------------------------
void    DlgBrshProp::Retreive(Brush* pBrush, int selCount)
{

    if(_dirty == FALSE) 
        return;
    if(!IsWindowVisible())
    {
        return;
    }


    _pCurBrush = pBrush;

    // flags
    char   szTemp[32];
    size_t&  cflags = pBrush->_brushflags;
    size_t&  cprops = pBrush->_brushprops;
    selCount       =  DOC()->_pSelBrushes.size();

    if(_touchedIDs.find(EF_NAME) !=  _touchedIDs.end() && selCount==1)
    {
        _efName.GetWindowText(pBrush->_name,16);
    }

    if((cflags & BRSH_BIGTERRAIN) && _selCount==1)
    {
        IS_K(CK_NOTILE)   ? cflags  |= TERR_UTT0 : cflags &= ~TERR_UTT0; 
        IS_K(CK_NOTILE1)  ? cflags  |= TERR_UTT1 : cflags &= ~TERR_UTT1; 
        IS_K(CK_NOTILE2)  ? cflags  |= TERR_UTT2 : cflags &= ~TERR_UTT2; 
        IS_K(CK_NOTILE4)  ? cflags  |= TERR_UTT3 : cflags &= ~TERR_UTT3; 
        //hack
        //IS_K(CK_GOR)    ? cflags  |= TERR_GOR : cflags &= ~TERR_GOR; 
        cflags  |= TERR_GOR ;
        //CheckDlgButton(CK_GOR, BST_CHECKED);

        IS_K(CK_FULLBR) ? cflags  |= TERR_FULLBRGHT : cflags &= ~TERR_FULLBRGHT; 
        IS_K(CK_NOLM)   ? cflags  |= TERR_NOLIGHT : cflags &= ~TERR_NOLIGHT; 
        IS_K(CK_NOCOLLIDE)   ? cflags  |= TERR_NOCOLIDE : cflags &= ~TERR_NOCOLIDE; 
        //CK_NOLM         TERR_NOLIGHT

        CBigTerrain& bt = SCENE().Terrain();

        IS_K(CK_L0) ? bt.n_stage |= 0x1 : bt.n_stage &= ~0x1;
        IS_K(CK_L1) ? bt.n_stage |= 0x2 : bt.n_stage &= ~0x2;
        IS_K(CK_L2) ? bt.n_stage |= 0x4 : bt.n_stage &= ~0x4;
        IS_K(CK_L3) ? bt.n_stage |= 0x8 : bt.n_stage &= ~0x8;


        bt.t_anim[0].u     = ParseREAL(this, T3_X);
        bt.t_anim[0].v     = ParseREAL(this, T3_Z);


        IS_K(RB_TSHIFT)  ? bt.n_stage  |= TERR_TA_SHIFT : bt.n_stage &= ~TERR_TA_SHIFT; 
        IS_K(RB_TVIBR)   ? bt.n_stage  |= TERR_TA_OSC : bt.n_stage &= ~TERR_TA_OSC; 

    }    
    

    if(selCount==1)
    { //cannot move all selected in one place
        
        if(_touchedIDs.find(EF_XC) !=  _touchedIDs.end() ||
           _touchedIDs.find(EF_YC) !=  _touchedIDs.end() ||
           _touchedIDs.find(EF_ZC) !=  _touchedIDs.end() )
          {
                V3 vc;
                vc.x = ParseREAL(this,EF_XC)*GMeters;
                vc.y = ParseREAL(this,EF_YC)*GMeters;
                vc.z = ParseREAL(this,EF_ZC)*GMeters;
                _pCurBrush->SetPos(vc);
                _pCurBrush->Recalc();
          }
    }

     if(_touchedIDs.find(EF_W) !=  _touchedIDs.end() ||
        _touchedIDs.find(EF_H) !=  _touchedIDs.end() ||
        _touchedIDs.find(EF_D) !=  _touchedIDs.end() )
    {
        V3 vcN;
        vcN.x  = (ParseREAL(this, EF_W)/2) * GMeters;
        if(vcN.x < 8)vcN.x=8;
        vcN.y  = (ParseREAL(this, EF_H)/2) * GMeters;
        if(vcN.y < 8)vcN.y=8;
        vcN.z  = (ParseREAL(this, EF_D)/2) * GMeters;
        if(vcN.z < 8)vcN.z=8;
     
        _pCurBrush->SetSize(Box(-vcN,vcN));
        _pCurBrush->Recalc();
    }

     if(_touchedIDs.find(CB_GROUP) !=  _touchedIDs.end())
    {
        _cbGroup.GetLBText(_cbGroup.GetCurSel(), szTemp);
        pBrush->_group = ::_ttoi(szTemp);
    }


    if(cflags & BRSH_WIREBOX)
        return;


    if(!(cflags & BRSH_SOLID)) // cut brush
    {
        if(_touchedIDs.find(CK_CUTALL) !=  _touchedIDs.end())
        {
            if(IS_K(CK_CUTALL))
            {
                cflags |= BRSH_CUTALL;
            }
            else
            {
                cflags &= ~BRSH_CUTALL;
            }
         }
        return;
     }

    // solid brush down here

    if(_touchedIDs.find(CK_DISABLE) !=  _touchedIDs.end())
    {
        if(IS_K(CK_DISABLE))
            cflags |= BRSH_DISABLED;
        else
            cflags &= ~BRSH_DISABLED;
    }

    if(_touchedIDs.find(CK_DETAIL) !=  _touchedIDs.end())
    {
        IS_K(CK_DETAIL)  ? cflags |= BRSH_DETAIL : cflags &= ~BRSH_DETAIL;
    }
    if(_touchedIDs.find(CK_TERRAIN) !=  _touchedIDs.end())
    {
        IS_K(CK_TERRAIN) ? cflags |= BRSH_CSGTERRAIN : cflags &= ~BRSH_CSGTERRAIN;
    }
    
    
    
    if(cflags & BRSH_DETAIL)
    {
        
        if(_touchedIDs.find(CK_NOCOLL) !=  _touchedIDs.end())
            IS_K(CK_NOCOLL)  ? cprops |= MODEL_NOCOLIDE : cprops &= ~MODEL_NOCOLIDE ;

        if(_touchedIDs.find(CK_DYNAMIC) !=  _touchedIDs.end())
            IS_K(CK_DYNAMIC) ? cprops |= MODEL_DYNAMIC  : cprops &= ~MODEL_DYNAMIC  ;

        if(_touchedIDs.find(CK_NOBSP) !=  _touchedIDs.end())
            IS_K(CK_NOBSP)   ? cflags |= BRSH_DONTBSP   : cflags &= ~BRSH_DONTBSP   ;

        if(_touchedIDs.find(CK_CYLBB) !=  _touchedIDs.end())
            IS_K(CK_CYLBB)   ? cprops |= MODEL_IS_CYLBB   : cprops &= ~MODEL_IS_CYLBB   ;

        if(_touchedIDs.find(CK_SPHBB) !=  _touchedIDs.end())
            IS_K(CK_SPHBB)   ? cprops |= MODEL_IS_SPHBB   : cprops &= ~MODEL_IS_SPHBB   ;

        if(_touchedIDs.find(CK_DETACHED) !=  _touchedIDs.end())
            IS_K(CK_DETACHED)   ? cflags |= BRSH_DETACHED   : cflags &= ~BRSH_DETACHED   ;
   

        if(1/*_touchedIDs.find(CB_CONTENT) !=  _touchedIDs.end()*/)
        {
            cprops &= ~MODEL_CONTENT_MASK;

            switch(_cbContent.GetCurSel())
            {
                case 0:
                default:
                    cprops |= MODEL_MAT_SOLID;
                    break;
                case 1:
                    cprops |= MODEL_MAT_WATER;
                    break;
                case 2:
                    cprops |= MODEL_MAT_LAVA;
                    break;
                case 3:
                    cprops |= MODEL_MAT_BRICK;
                    break;
                case 4:
                    cprops |= MODEL_MAT_WOOD;
                    break;
                case 5:
                    cprops |= MODEL_MAT_METAL;
                    break;
                case 6:
                    cprops |= MODEL_MAT_ICE;
                    break;
                case 7:
                    cprops |= MODEL_MAT_GLASS;
                    break;
                case 8:
                    cprops |= MODEL_MAT_DIRT;
                    break;
                case 9:
                    cprops |= MODEL_MAT_GELA;
                    break;
                case 10:
                    cprops |= MODEL_MAT_PLASMA;
                    break;
                case 11:
                    cprops |= MODEL_MAT_GAS;
                    break;
                case 12:
                    cprops |= MODEL_MAT_FOG;
                    break;

            }
        }

    }

    if(!(cflags & BRSH_DETAIL) || !(cprops & MODEL_DYNAMIC))
    {
        if(_pCurBrush->_pMotion)
        {
            DOC()->DeleteBrushMotion(_pCurBrush);
        }
    }

    if(_touchedIDs.find(CK_SKYDOM) !=  _touchedIDs.end())
    {
        IS_K(CK_SKYDOM) ? cprops |= MODEL_IS_SKYDOM : cprops &= ~MODEL_IS_SKYDOM ;

        if(cprops & MODEL_IS_SKYDOM )
        {
            IS_K(CK_ROTATE) ? cflags |= BRSH_ROTATES : cflags &= ~BRSH_ROTATES;

            if(cflags & BRSH_ROTATES)
            {
                _pCurBrush->_rotSteps.x = ParseREAL(this,EF_XR);
                _pCurBrush->_rotSteps.y = ParseREAL(this,EF_YR);
                _pCurBrush->_rotSteps.z = ParseREAL(this,EF_ZR);
            }
            else
            {
                _pCurBrush->_rotSteps = V0;
            }
        }
    }

    if(_touchedIDs.find((size_t)RB_LIN)!=  _touchedIDs.end() || 
        _touchedIDs.find((size_t)RB_SPH)!=  _touchedIDs.end())
    {
        FOREACH(PBrushes,  DOC()->_pSelBrushes, ppb)
        {
            if(IsDlgButtonChecked(RB_LIN))
            {
                (*ppb)->_cf._tm = TM_LIN;
            }
            else
            {
                (*ppb)->_cf._tm = TM_SPH;
            }
            (*ppb)->ModifyTexCoord(GUtex);
        }
        DOC()->Recut();
    }
}

//-----------------------------------------------------------------------------
void DlgBrshProp::OnDetail() 
{
	if(_pCurBrush==0)		return;

    BOOL b = IS_K(CK_DETAIL);

    if(!_updatingCtrls || FALSE ==b)
    {
        if(_pCurBrush->_pMotion )
        {
            if(IDNO == AfxMessageBox("All motions will be deleted. \nDo you wish to continue",
                                    MB_YESNO|MB_ICONQUESTION))
            {
                return;
            }
        }
    }
    
    EN_(CK_NOCOLL,b);
    EN_(CK_DYNAMIC,b);
    EN_(CB_CONTENT,b);
    EN_(CK_NOBSP,b);
    EN_(CK_CYLBB,b);
    EN_(CK_SPHBB,b);
    EN_(CK_DETACHED,b);


	if(b==FALSE){
		CK_(CK_NOCOLL,0);
        CK_(CK_DYNAMIC,0);
		CK_(CK_NOCOLL,0);
        CK_(CK_DETACHED,0);
        CK_(CK_CYLBB,0);
        CK_(CK_SPHBB,0);
        CK_(CK_NOBSP,0);
        CK_(CB_CONTENT,0);
	}        

    OnDynamic();
}

//-----------------------------------------------------------------------------
void DlgBrshProp::OnDynamic() 
{
	if(_pCurBrush==0)		return;
    BOOL b = IS_K(CK_DYNAMIC); 
    EN_(IDC_LIST1,b);
    EN_(CK_LOOP,b);
    EN_(ST_STEPS,b);
    EN_(EF_STEPSS,b);
    EN_(PB_ADD,b);

	if(FALSE == b)
	{
		if(_pCurBrush->_pMotion)
        {
			DOC()->DeleteBrushMotion(_pCurBrush);
			_lcMove.DeleteAllItems();
		}
	}

    if(!_pCurBrush->_pMotion)
        _lcMove.DeleteAllItems();

    OnAction();
}

//-----------------------------------------------------------------------------
void DlgBrshProp::OnCut() 
{
    BOOL b = IS_K(CK_CUT);
	char	tw[3];
    CWnd*   pWnd     = this;
    int     relation = GW_CHILD|GW_HWNDFIRST;
    while(pWnd = pWnd->GetWindow(relation))
    {
        relation = GW_HWNDNEXT;
        size_t iD = pWnd->GetDlgCtrlID();
        if(iD == CB_GROUP || 
            iD == EF_NAME || 
            iD == T3_Z    || 
            iD == T3_X  || 
            iD == EF_XC || 
            iD == EF_YC || 
            iD == EF_ZC ||
            iD == EF_W  ||
            iD == EF_H  ||
            iD == EF_D )

            continue;
        

        pWnd->GetWindowText(tw, 3);
        if(tw[0]=='+' || tw[0]=='-')
		{
			pWnd->EnableWindow(1);
		}
        else
        {
            pWnd->EnableWindow(0);
        }
    }
    OnAction();
}

//-----------------------------------------------------------------------------
void DlgBrshProp::OnButton1() 
{
	if(_pCurBrush==0)		return;
    BARDLG()->ResetBrushes();
    OnAction();
}


//-----------------------------------------------------------------------------
void DlgBrshProp::OnCloseupContent() 
{
    _touchedIDs.insert(CB_CONTENT);
    OnAction();
}

//-----------------------------------------------------------------------------
void DlgBrshProp::OnOk() 
{
    BARDLG()->OnApply();
}


//-----------------------------------------------------------------------------
void DlgBrshProp::OnShowWindow(BOOL bShow, size_t nStatus) 
{
	CVertDlgDlg::OnShowWindow(bShow, nStatus);
    if(0 == bShow){
		_pCurBrush=0;
	}
	else
	{
        int count;
        Brush** pB = DOC()->GetSelBrushes(count);
        if(count)
            Update(*pB, count);
	}
}

//-----------------------------------------------------------------------------
// path item selected
LRESULT DlgBrshProp::OnItemSelected(WPARAM isel, LPARAM lp)
{
    if(lp == (LPARAM)&_lcMove)
    {
        ASSERT(_pCurBrush->_pMotion);
        CMotion* pM = _pCurBrush->_pMotion;

        if(isel > 0)
        {
            pM->SelectNode(isel);
        }
        else
        {
            pM->SelectNode(0);
        }
        DOC()->Invalidate();
    }
	return 0L;
}

//-----------------------------------------------------------------------------
void DlgBrshProp::OnCancel() 
{
    if(GetFocus() == &_lcMove)
    {
        int isel = _lcMove.GetCurSel();
        if(isel>=0)
        {
            _lcMove.SetCurSel(0);
            CMotion* pM = _pCurBrush->_pMotion;
            pM->SelectNode(0);
        }
    }
    Clean();
    DOC()->DeselectBrushes();
    DOC()->Invalidate();
}

//-----------------------------------------------------------------------------
void    DlgBrshProp::Clean()
{
    _dirty = 0;
    BARDLG()->Clean();
}


//-----------------------------------------------------------------------------
void    DlgBrshProp::Dirty()
{
    if(BARDLG()->IsRetreiveBlocked())
        return;
    _dirty  = TRUE;
    BARDLG()->Dirty();
}

//-----------------------------------------------------------------------------
void    DlgBrshProp::UpdateMotionProps(Brush* pB, CMotion* pM, BOOL bRefresh)
{
    if(bRefresh)
    {
        int isel = _lcMove.GetCurSel();

        _lcMove.SetRedraw(0);
        _lcMove.DeleteAllItems();
        for(int i=0;i<pM->GetNodesCount();i++)
        {
            MState*    pS = pM->GetState(i);
        
            if(0 == i) // brush
            {
                _lcMove.AddNewItem(MKSTR("%s", pB->_name),
							       MKSTR("%3.2f",pB->_box.GetCenter().x/GMeters),
							       MKSTR("%3.2f",pB->_box.GetCenter().y/GMeters),
							       MKSTR("%3.2f",pB->_box.GetCenter().z/GMeters),
                                   MKSTR("%3.2f",pS->_trTimes[0]),
                                   MKSTR("%3.2f",pS->_trTimes[1]),
							       0);
            }
            else    // move path nodes
            {
                _lcMove.AddNewItem(MKSTR("Node: %d" ,i),
                                   MKSTR("%3.2f",pS->_pos.x/GMeters),
							       MKSTR("%3.2f",pS->_pos.y/GMeters),
							       MKSTR("%3.2f",pS->_pos.z/GMeters),
                                   MKSTR("%3.2f",pS->_trTimes[0]),
                                   MKSTR("%3.2f",pS->_trTimes[1]),
                                   0);
            }
        }
        _lcMove.SetCurSel(isel);
        _lcMove.SetRedraw(1);
        _lcMove.Invalidate();
    }
    else
    {
		int isel = _lcMove.GetCurSel();
        for(int i=0;i<pM->GetNodesCount();i++)
        {
            MState*    pS = pM->GetState(i);
        
            if(0 == i) // brush
            {
                _lcMove.LstSetItem(i,MKSTR("%s", pB->_name),
							       MKSTR("%3.2f",pB->_box.GetCenter().x/GMeters),
							       MKSTR("%3.2f",pB->_box.GetCenter().y/GMeters),
							       MKSTR("%3.2f",pB->_box.GetCenter().z/GMeters),
                                   MKSTR("%3.2f",pS->_trTimes[0]),
                                   MKSTR("%3.2f",pS->_trTimes[1]),
							       0);
            }
            else    // move path nodes
            {
                _lcMove.LstSetItem(i,MKSTR("Node: %d" ,i),
                                   MKSTR("%3.2f",pS->_pos.x/GMeters),
							       MKSTR("%3.2f",pS->_pos.y/GMeters),
							       MKSTR("%3.2f",pS->_pos.z/GMeters),
                                   MKSTR("%3.2f",pS->_trTimes[0]),
                                   MKSTR("%3.2f",pS->_trTimes[1]),
                                   0);
            }
        }
		_lcMove.SetCurSel(isel);

    }
}

//-----------------------------------------------------------------------------
void DlgBrshProp::OnAdd() 
{
    if(IsDlgButtonChecked(CK_DYNAMIC))
    {
        // delete previous motion
        DOC()->DeleteBrushMotion(_pCurBrush);
        int     iNodes = GetDlgItemInt(EF_STEPSS, FALSE);
        
        if(iNodes>1)
        {
            GetDlgItem(EF_STEPSS)->EnableWindow(0);
            GetDlgItem(PB_ADD)->EnableWindow(0);

            BOOL    cLoop  = IsDlgButtonChecked(CK_LOOP);

            CMotion* pNewMotion = new CMotion(_pCurBrush, iNodes, cLoop);
            DOC()->AddBrushMotion(_pCurBrush, pNewMotion);
            UpdateMotionProps(_pCurBrush, pNewMotion,TRUE);
        }
        else
        {
            DOC()->DeleteBrushMotion(_pCurBrush);
            _lcMove.DeleteAllItems();
            CK_(CK_LOOP,0);
            _efSteps.SetWindowText("");
        }
        DOC()->Invalidate();
    }

}

//-----------------------------------------------------------------------------
LRESULT    DlgBrshProp::OnItemDoubleClicked(WPARAM iSel, LPARAM lp)
{
    if(lp == (LPARAM)&_lcMove)
    {
        ASSERT(_pCurBrush->_pMotion);
        CMotion* pM = _pCurBrush->_pMotion;
        pM->SelectNode(iSel);
        MState*  pS = pM->GetSelState();
        DlgMotionState  d(pS);
        if(IDOK == d.DoModal()) // the transfer into the 'pS' is done in dialog OnOK
        {
            DOC()->Invalidate();
            UpdateMotionProps(_pCurBrush, pM);
        }
    }
	return 0L;   
}

//---------------------------------------------------------------------------------------
void DlgBrshProp::OnSkydom() 
{
	BOOL b = IS_K(CK_SKYDOM);
    EN_(CK_ROTATE,b);
	OnRotate();
	
}

//---------------------------------------------------------------------------------------
void DlgBrshProp::OnAction() 
{
    if(BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}


void DlgBrshProp::OnNocoll() 
{
	OnAction();

}

void DlgBrshProp::OnRotate() 
{
	BOOL b = IS_K(CK_ROTATE);
    
    EN_(EF_XR,b);
    EN_(EF_YR,b);
    EN_(EF_ZR,b);

    OnAction();		
}


void DlgBrshProp::OnCloseupGroup() 
{
    _touchedIDs.insert(CB_GROUP);
    OnAction();	
}

void DlgBrshProp::OnP2() 
{
    if(_paneNo>=2) return;
	RECT rt; 
	GetDlgItem(PAGE_1)->GetClientRect(&rt);
	ScrollWindow(-rt.right,0);
	++_paneNo;
}

void DlgBrshProp::OnP1() 
{
    if(_paneNo<=0) return;
	RECT rt; 
	GetDlgItem(PAGE_1)->GetClientRect(&rt);
	ScrollWindow(rt.right,0);
    --_paneNo;		
}

void DlgBrshProp::OnP3() 
{
    if(_paneNo>=2) return;
	RECT rt; 
	GetDlgItem(PAGE_1)->GetClientRect(&rt);
	ScrollWindow(-rt.right,0);
    ++_paneNo;
}

void DlgBrshProp::OnP32() 
{
    if(_paneNo<=0) return;
	RECT rt; 
	GetDlgItem(PAGE_1)->GetClientRect(&rt);
	ScrollWindow(rt.right,0);
    --_paneNo;		
}


void DlgBrshProp::OnTriangs() 
{
}

void DlgBrshProp::OnNobsp() 
{
    if(BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}

void DlgBrshProp::OnDisable() 
{
    if(BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}


void DlgBrshProp::OnScript() 
{
    if(!_pCurBrush)return;
}

void DlgBrshProp::OnCkscript() 
{
    if(BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
}

void DlgBrshProp::OnCutall() 
{
    if(BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
	
}

