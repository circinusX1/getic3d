/*
pPoly->_colorD, bump, spec
*/
// Author : Marius C.
//
//---------------------------------------------------------------------------------------

#include "stdafx.h"
#include "geticgui.h" 
#include "z-edmap.h"
#include "DlgFaceProp.h"
#include "Poly.h"
#include "DlgBar.h"
#include "z_ed2Doc.h"
#include "MainFrm.h"
#include "z_ed3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define         IS_TID(id_) (_touchedIDs.find(id_) !=  _touchedIDs.end())
static GLUquadricObj*  GluObj;

//-----------------------------------------------------------------------------
// DlgFaceProp dialog
DlgFaceProp::DlgFaceProp(CWnd* pParent /*=NULL*/)
	: CVertDlgDlg(DlgFaceProp::IDD, pParent),_pCurent(0)
{
	//{{AFX_DATA_INIT(DlgFaceProp)
	//}}AFX_DATA_INIT
	
}

//-----------------------------------------------------------------------------
void DlgFaceProp::DoDataExchange(CDataExchange* pDX)
{
	CVertDlgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgFaceProp)
	DDX_Control(pDX, CB_EFFECTS, m_cnanims);
	DDX_Control(pDX, LB_PROPS, m_lbprops);
	DDX_Control(pDX, PB_EMMISIVE, m_rs);
    DDX_Control(pDX, EF_SPEEDV, _efTanimV);
	DDX_Control(pDX, EF_SPEEDU, _efTanimU);
    DDX_Control(pDX, EF_SHINE, m_shine);
	DDX_Control(pDX, EF_STICK, m_stick);
    DDX_Control(pDX, EF_BUMP, m_bump);
	DDX_Control(pDX, EF_ALPHA, _efColor);
	//}}AFX_DATA_MAP
}

typedef LRESULT (DlgFaceProp::*DLGFP)(WPARAM,LPARAM);


BEGIN_MESSAGE_MAP(DlgFaceProp, CVertDlgDlg)
	//{{AFX_MSG_MAP(DlgFaceProp)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(CB_EFFECTS, OnSelchangeEffects)
	ON_BN_CLICKED(CK_GENSC, OnGensc)
	ON_BN_CLICKED(CK_GENSC2, OnGensc2)
	ON_BN_CLICKED(CK_GENSS, OnGenss)
	ON_BN_CLICKED(CK_GENSS2, OnGenss2)
	ON_BN_CLICKED(PB_REUV, OnResetTC)
    ON_BN_CLICKED(PB_DIFUZE,    OnDifuze)
	ON_BN_CLICKED(PB_EMMISIVE,  OnEmmisive)
    ON_BN_CLICKED(PB_SPEC,      OnSpec)
	ON_BN_CLICKED(PB_RESETSPLITS, OnResetsplits)
	//}}AFX_MSG_MAP
    ON_CONTROL(CLBN_CHKCHANGE, LB_PROPS, OnChkChange)

    ON_MESSAGE(WM_POSTINIT, (DLGFP)OnPostInit)	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
static void FCbInit(CWnd* pwnd)
{
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize       = sizeof(pfd);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 16;
    pfd.cDepthBits  = 16;
    pfd.iLayerType  = PFD_MAIN_PLANE;
    int iFormat     = ChoosePixelFormat(((CStaticGL*)pwnd)->_hdc, &pfd);
    SetPixelFormat(((CStaticGL*)pwnd)->_hdc, iFormat, &pfd);            

    ((CStaticGL*)pwnd)->m_hRC = wglCreateContext(((CStaticGL*)pwnd)->_hdc);
    
    MakeCurrent(((CStaticGL*)pwnd)->_hdc, ((CStaticGL*)pwnd)->m_hRC); 
    DOC()->SetFaceMode();

       
    

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearDepth( 1 );
    glDisable(GL_FOG);
    glDisable(GL_DITHER);
    glEnable(GL_ALPHA_TEST);
	glClearColor(0.7f,0.7f,0.7f,0.f);
    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
       
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

}

// inital material
static REAL _gd[4] = {0.3, 0.3, 0.3, 1};
static REAL _gs[4] = {0.6, 0.6, 0.6, 1};
static REAL _ge[4] = {0.0, 0.0, 0.0, 1};
static REAL _gsh   = 10.0;   

static void FCbPreparePaint(CWnd* pdc, V_MODE mode, int init)
{
    if(init){}
    else
    {
    }
}

static Poly* PCurent;
static void FCbPaint(CWnd* pdc, V_MODE mode)
{
    
    REAL r[4] = {0.1, 0.1, 0.1, 1};
    REAL p[4] = {0,0,0,0};

    glColor4ubv(CLR_WHITE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL) ;
    glEnable(GL_LIGHT0);
    
    if(PCurent)
    {
        REAL e[4] = {PCurent->_colorE.r/255.0, PCurent->_colorE.g/255.0, PCurent->_colorE.b/255.0, 1};
        REAL d[4] = {PCurent->_colorD.r/255.0, PCurent->_colorD.g/255.0, PCurent->_colorD.b/255.0, 1};
        REAL s[4] = {PCurent->_colorS.r/255.0, PCurent->_colorS.g/255.0, PCurent->_colorS.b/255.0, 1};

        glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
        glMaterialfv(GL_FRONT, GL_SPECULAR, s);
        glMaterialfv(GL_FRONT, GL_EMISSION, e);
        glMateriali(GL_FRONT, GL_SHININESS, PCurent->_shiness);
    }
    else
    {
        REAL e[4] = {0,0,0,1};
        REAL d[4] = {1.0,1.0,1.0, 1};
        REAL s[4] = {1.0,1.0,1.0, 1};

        glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
        glMaterialfv(GL_FRONT, GL_SPECULAR, s);
        glMaterialfv(GL_FRONT, GL_EMISSION, e);
        glMateriali(GL_FRONT, GL_SHININESS, 100);
    }

    float position[4] = {-150.0, 150.0, 150.0,1/*non-directional*/};
    REAL a[4]         = {.2,.2,.2,1};
    REAL dif[4]       = {0.4, 0.4, 0.4, 1.0};
    REAL spec[4]      = {0.9, 0.9, 0.9, 1.0};
    REAL linearAttenuation    = .00006;

	glLightfv(GL_LIGHT0, GL_POSITION, (float *) position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  a);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, &(linearAttenuation));

    if(PCurent)
    {
        Ta.Bind(PCurent->GetHtex(0), GEN_TEX_GEN_ST_SPH);
    }

    glEnable(GL_NORMALIZE);
    gluSphere(GluObj, 128.0, 24, 24);
    glPushMatrix();
            glTranslatef(150.0, -150.0, -150.0);
            gluSphere(GluObj, 32.0, 12, 12);
    glPopMatrix();
    glDisable(GL_NORMALIZE);

    
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL) ;
    glDisable(GL_LIGHT0);

}

LRESULT DlgFaceProp::OnPostInit(LPARAM, WPARAM) 
{
    z_ed3View*  pT3 = (z_ed3View *)FRM()->m_wndSplitter.GetPane(0,0);
    ////wglShareLists(pT3->m_hRC, _glWnd.m_hRC);
    DOC()->SetFaceMode();

    VIEW2D(1,1)->PostMessage(WM_POSTINIT);
    VIEW2D(0,1)->PostMessage(WM_POSTINIT);
    VIEW2D(1,0)->PostMessage(WM_POSTINIT);
    VIEW3D(0,0)->PostMessage(WM_POSTINIT);

    
    return 0;
}   


//-----------------------------------------------------------------------------
BOOL DlgFaceProp::OnInitDialog() 
{
    _updatingCtrls = TRUE;

    /*
    _glWnd._pcbinit  = FCbInit;
    _glWnd._pcbpaint = FCbPaint;
    _glWnd._pcbpreppaint = FCbPreparePaint;
    _glWnd.b_showGrid = 0;
	_glWnd.CreateWnd(this, ST_GLCTL); // shares the list
	_glWnd.ShowWindow(SW_SHOW);
	_glWnd._dist=-190;
    _glWnd._backgrnd[0]= .7;
    _glWnd._backgrnd[1]= .7;
    _glWnd._backgrnd[2]= .7;

    
    GluObj = gluNewQuadric();
    gluQuadricNormals(GluObj ,  GLU_SMOOTH);
    gluQuadricOrientation(GluObj, GLU_INSIDE);
    gluQuadricTexture(GluObj, GL_TRUE);
    gluQuadricDrawStyle(GluObj,GLU_FILL);
    */


    _ctrlt0.CreateWnd(this,ST_L0);
    _ctrlt1.CreateWnd(this,ST_L1);
    _ctrlt2.CreateWnd(this,ST_L2);
    _ctrlt3.CreateWnd(this,ST_L3);

    //_glWnd._nMode=V_3D;
    
    CVertDlgDlg::OnInitDialog();

    _efTanimU.SetLimitText(8);
    _efTanimV.SetLimitText(8);
    _efTanimU.SetWindowText("0.1");
    _efTanimV.SetWindowText("0.1");


    m_rspl.SubclassDlgItem( PB_RESETSPLITS,  this);
	m_s.SubclassDlgItem(PB_SPEC, this);
    m_reuv.SubclassDlgItem(PB_REUV, this);
    m_d.SubclassDlgItem(PB_DIFUZE, this);

	m_stick.SetLimitText(8);
	m_bump.SetLimitText(8);
	m_stick.SetWindowText("0.00");
	m_bump.SetWindowText("1.00");

    int idx = m_lbprops.AddString("Transparent");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Mirror");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Show Back");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("No Lights");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Gouroud Light");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Full Bright");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Bump");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Hallo");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Masked Black");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Projective");
    // m_lbprops.SetItemData(idx, );


    idx = m_lbprops.AddString("Occluder");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Disable Collision");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Splitter");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Patch");
    // m_lbprops.SetItemData(idx, );
    m_lbprops.SetCheck(idx,1);
    //m_lbprops.Enable(idx,0); //not used allways patch
    

    idx = m_lbprops.AddString("No Texture");
    // m_lbprops.SetItemData(idx, );

    idx = m_lbprops.AddString("Custom Light Map");
    // m_lbprops.SetItemData(idx, );


	_updatingCtrls = FALSE;
    _last2usesplitidx = 0;
	_updatingCtrls = FALSE;
    ClearDirtyIDs();
    return TRUE;
}

//-----------------------------------------------------------------------------
void DlgFaceProp::OnOk() 
{
    BARDLG()->OnApply();
}

//-----------------------------------------------------------------------------
void DlgFaceProp::OnCancel() 
{
    DOC()->DeselectPolys();
    DOC()->Invalidate();
}

//-----------------------------------------------------------------------------
void    DlgFaceProp::Dirty()
{
    if(_updatingCtrls)return;
    BARDLG()->Dirty();
    _dirty = TRUE;
}

//-----------------------------------------------------------------------------
void    DlgFaceProp::EnableAll()
{
    CWnd*   pWnd = this;
    int     relation = GW_CHILD|GW_HWNDFIRST;
    while(pWnd = pWnd->GetWindow(relation))
    {
        pWnd->EnableWindow(1);
        relation = GW_HWNDNEXT;
    }

}

//-----------------------------------------------------------------------------
// updates the controls with selected face properties
void    DlgFaceProp::Update(Poly* pPoly, int selCont)
{
    if(!m_hWnd )
        return;


    PCurent = _pCurent  =  pPoly;
    if(pPoly == 0)
    {
        ClearDirtyIDs();
        //Reset();
        //Enable(0);
        //DisableAllCtrls();
        return;
    }
    Enable(1);
    
    _updatingCtrls = TRUE;

    selCont = DOC()->_pSelPolys.size();
    if(selCont>1)
        SetDlgItemText(EF_NOTE,"Shows last polygon flags");
    else
        SetDlgItemText(EF_NOTE,"");
    SetDlgItemText(EF_AREA, MKSTR("%.2f",pPoly->_area));
    int i;
    for( i=0; i < m_lbprops.GetCount(); i++)
    {
        m_lbprops.SetCheck(i, pPoly->_polyprops & (1<<i) ? 1 : 0);
    }
    m_lbprops.Enable(i-1,0); //light  map is informative

    DWORD anims = pPoly->_polyprops>>16;
    m_cnanims.SetCurSel(0);
    if(anims)
    {
        for(i=0;i<16;i++)
        {
            if(anims & 0x1)
            {
                m_cnanims.SetCurSel(i+1);
                break;
            }
            anims>>=1;
        }
    }
 
    CK_(CK_GENSC,EV_(pPoly->_polyprops & FACE_TG_ENVCUBE));
    CK_(CK_GENSS,EV_(pPoly->_polyprops & FACE_TG_ENVSHPERE));
    CK_(CK_GENSC2,EV_(pPoly->_polyprops & FACE_TG_ENVCUBE2));
    CK_(CK_GENSS2,EV_(pPoly->_polyprops & FACE_TG_ENVSHPERE2));

    if(FACE_SPLITTER & pPoly->_polyprops )
        SetDlgItemInt(ST_LIDXSP, pPoly->_use2splitidx);
    else
        SetDlgItemText(ST_LIDXSP, "");

    Sct(this, EF_NORM, pPoly->_c);
    Sct(this, EF_CONST, pPoly->_n);
    
    _efColor.SetWindowText(MKSTR("%d", pPoly->_colorD.a));
    m_d.SetColor(RGB(pPoly->_colorD.r,pPoly->_colorD.g,pPoly->_colorD.b));
    m_d.Invalidate();

    _gd[0] = (REAL)pPoly->_colorD.r/255.0;
    _gd[1] = (REAL)pPoly->_colorD.g/255.0;
    _gd[2] = (REAL)pPoly->_colorD.b/255.0;
    
    _gs[0] = (REAL)pPoly->_colorS.r/255.0;
    _gs[1] = (REAL)pPoly->_colorS.g/255.0;
    _gs[2] = (REAL)pPoly->_colorS.b/255.0;

    _ge[0] = (REAL)pPoly->_colorE.r/255.0;
    _ge[1] = (REAL)pPoly->_colorE.g/255.0;
    _ge[2] = (REAL)pPoly->_colorE.b/255.0;

    _gsh   = pPoly->_shiness;   
    ////_glWnd.Invalidate();
    Sct(this, EF_STICK, pPoly->_friction);
    Sct(this, EF_BUMP, pPoly->_bump);
    Sct(this, EF_SHINE, pPoly->_shiness);
    if(pPoly->_shiness>128)pPoly->_shiness=128;
    UpdateTextureStage(pPoly, selCont, 1);


    if(pPoly->_polyprops & FACE_TAMASK)
    {
        if(pPoly->_polyprops & FACE_TA_FRAMING)
        {
            _efTanimU.SetWindowText(MKSTR("%d", (int)pPoly->_tanimA.u));
            _efTanimV.SetWindowText(MKSTR("%d", (int)pPoly->_tanimA.v));
        }
        else
        {
            _efTanimU.SetWindowText(MKSTR("%1.4f", pPoly->_tanimA.u));
            _efTanimV.SetWindowText(MKSTR("%1.4f", pPoly->_tanimA.v));
        }
    }
    else
    {
        _efTanimU.SetWindowText(MKSTR("%d", (int)0));
        _efTanimV.SetWindowText(MKSTR("%d", (int)0));
    }

    _ctrlt1.Repaint();
    _ctrlt0.Repaint();
    _ctrlt3.Repaint();
    _ctrlt2.Repaint();


    _updatingCtrls = FALSE;	
    ClearDirtyIDs();	
}

//-----------------------------------------------------------------------------
void DlgFaceProp::OnChkChange() 
{
   // OnCmdMsg(-LB_PROPS, CN_COMMAND, 0, 0) ;
    OnAction();
    _touchedIDs.insert(LB_PROPS & 0xFFFF);
}



//-----------------------------------------------------------------------------
// gets trom controls nw properties for the selected face/faces
void    DlgFaceProp::Retreive(Poly* pPoly, int selCont) 
{
    if(_dirty == FALSE || !IsWindowVisible()) 
        return;

    BOOL pchanged = 0;
    BOOL changed = 0;
    selCont = DOC()->_pSelPolys.size();

    if(IS_TID(LB_PROPS))
    {
        for(int i=0; i < m_lbprops.GetCount(); i++)
        {
            if(m_lbprops.GetCheck(i))
            {
                pPoly->_polyprops |= (1<<i);
            }
            else
            {
                pPoly->_polyprops &=~ (1<<i);
            }
        }

        if(pPoly->_polyprops | FACE_TRANSPARENT || 
           pPoly->_polyprops | FACE_BLACKMASK)
        {
            if(pPoly->_colorD.a == 255){
                pPoly->_colorD.a = 192;
            }
        }
        changed = 1;
    }

    if(selCont==1)
    {
        if(pPoly->_polyprops & FACE_SPLITTER)
        {
            pPoly->_use2splitidx = _last2usesplitidx;
            ++_last2usesplitidx;
        }
    }


    if(IS_TID(CK_GENSC) ||
       IS_TID(CK_GENSS) ||
       IS_TID(CK_GENSC2)||
       IS_TID(CK_GENSS2))
    {
        IS_K(CK_GENSC) ? pPoly->_polyprops |= FACE_TG_ENVCUBE   : pPoly->_polyprops &= ~FACE_TG_ENVCUBE;
        IS_K(CK_GENSS) ? pPoly->_polyprops |= FACE_TG_ENVSHPERE : pPoly->_polyprops &= ~FACE_TG_ENVSHPERE;
        IS_K(CK_GENSC2) ? pPoly->_polyprops |= FACE_TG_ENVCUBE2   : pPoly->_polyprops &= ~FACE_TG_ENVCUBE2;
        IS_K(CK_GENSS2) ? pPoly->_polyprops |= FACE_TG_ENVSHPERE2 : pPoly->_polyprops &= ~FACE_TG_ENVSHPERE2;

        Texs& textures = pPoly->GetTexs();

        textures._texApply[0] &= ~GEN_TEX_GEN_ST_CUBE;
        textures._texApply[0] &= ~GEN_TEX_GEN_ST_SPH;
        textures._texApply[2] &= ~GEN_TEX_GEN_ST_CUBE;
        textures._texApply[2] &= ~GEN_TEX_GEN_ST_SPH;

        if(pPoly->_polyprops & FACE_TG_ENVCUBE)
            textures._texApply[0]|=GEN_TEX_GEN_ST_CUBE;
        else if(pPoly->_polyprops & FACE_TG_ENVSHPERE)
            textures._texApply[0]|=GEN_TEX_GEN_ST_SPH;

        if(pPoly->_polyprops & FACE_TG_ENVCUBE2)
            textures._texApply[2]|=GEN_TEX_GEN_ST_CUBE;
        else if(pPoly->_polyprops & FACE_TG_ENVSHPERE2)
            textures._texApply[2]|=GEN_TEX_GEN_ST_SPH;

        changed = 1;
        pchanged =1;
    }

    if(IS_TID(CB_EFFECTS) || IS_TID(EF_SPEEDU) || IS_TID(EF_SPEEDV))
    {
        int anims = m_cnanims.GetCurSel();
        DWORD props = 0;
        pPoly->_polyprops &= 0x0000FFFF;

        pchanged =1;

        if(anims>0)
        {
            pPoly->_polyprops |= (1 << ((anims-1)+16));
            
        }
        if(pPoly->_polyprops & FACE_TAMASK)
        {
            pPoly->_polyflags2 |= (1<<GUtex);        

            if(pPoly->_polyprops & FACE_TA_FRAMING || pPoly->_polyprops & FACE_TG_RANDOM)
            {
                pPoly->_tanimA.u = GetDlgItemInt(EF_SPEEDU);
                pPoly->_tanimA.v = GetDlgItemInt(EF_SPEEDV);
            }
            else
            {
                pPoly->_tanimA.u = ParseREAL(this,EF_SPEEDU);
                pPoly->_tanimA.v = ParseREAL(this,EF_SPEEDV);
            }
            pPoly->_tanimB = UV(0,0); // hold inital tanims
        }
        else
            pPoly->_polyflags2 &= ~(1<<GUtex);

        changed = 1;
    }

   

     
    if(IS_TID(PB_COLOR))
    {
        pPoly->_colorD.r = GetRValue(_pbColor._color); 
        pPoly->_colorD.g = GetGValue(_pbColor._color);
        pPoly->_colorD.b = GetBValue(_pbColor._color);
        changed = 1;
    }
    if(IS_TID(EF_ALPHA))
    {
        pPoly->_colorD.a = GetDlgItemInt(EF_ALPHA);
        if(pPoly->_colorD.a < 255){
            pPoly->_polyprops |= FACE_TRANSPARENT;
        }
        changed = 1;
        pchanged =1;
    }
    if(IS_TID(PB_DIFUZE))
    {
        pPoly->_colorD.r = GetRValue(m_d._color); 
        pPoly->_colorD.g = GetGValue(m_d._color); 
        pPoly->_colorD.b = GetBValue(m_d._color); 

        pPoly->_colorD.r = pPoly->_colorD.r ;
        pPoly->_colorD.g = pPoly->_colorD.g ;
        pPoly->_colorD.b = pPoly->_colorD.b ;

        pPoly->_colorS.r = pPoly->_colorD.r/2;
        pPoly->_colorS.g = pPoly->_colorD.g/2;
        pPoly->_colorS.b = pPoly->_colorD.b/2;

        pPoly->_colorE.r = pPoly->_colorD.r/8;
        pPoly->_colorE.g = pPoly->_colorD.g/8;
        pPoly->_colorE.b = pPoly->_colorD.b/8;
        changed = 1;
    }

    if(IS_TID(EF_SHINE))
    {
        pPoly->_shiness  = ParseREAL(this, EF_SHINE);
        changed = 1;
    }

    _gd[0] = (REAL)pPoly->_colorD.r/255.0;
    _gd[1] = (REAL)pPoly->_colorD.g/255.0;
    _gd[2] = (REAL)pPoly->_colorD.b/255.0;

    _gs[0] = (REAL)pPoly->_colorS.r/255.0;
    _gs[1] = (REAL)pPoly->_colorS.g/255.0;
    _gs[2] = (REAL)pPoly->_colorS.b/255.0;

    _ge[0] = (REAL)pPoly->_colorE.r/255.0;
    _ge[1] = (REAL)pPoly->_colorE.g/255.0;
    _ge[2] = (REAL)pPoly->_colorE.b/255.0;

    _gsh   = pPoly->_shiness;   
    ////_glWnd.Invalidate();

    if(IS_TID(EF_STICK))
    {
        pPoly->_friction = ParseREAL(this, EF_STICK);
        changed = 1;
    }

    if(IS_TID(EF_BUMP))
    {
        pPoly->_bump     = ParseREAL(this, EF_BUMP);
        changed = 1;
    }
    // texture stages
    UpdateTextureStage(pPoly, selCont, 0);

    if(changed)
    {
        pPoly->_pBrush->ApplyCuts();
        
    }
    if(pchanged)
    {
        int i;
        for( i=0; i < m_lbprops.GetCount(); i++)
        {
            m_lbprops.SetCheck(i, pPoly->_polyprops & (1<<i) ? 1 : 0);
        }
    }
}



void DlgFaceProp::OnAction() 
{
    if(BARDLG()->IsRetreiveBlocked()||_updatingCtrls)
        return ;
    Dirty();
    BARDLG()->OnApply();	
    ////_glWnd.Invalidate();
}

void    DlgFaceProp::UpdateTextureStage(Poly* pPoly, int nCount, BOOL in)
{
    _TRY{
        if(1 == nCount && in)
        {
            char* pdash;
            char* ptex = (char*)GTexSys.GetTexName(pPoly->GetHtex(0));
            pdash = strchr(ptex,'?');
            if(pdash) *pdash =0;

            SetDlgItemText(ST_TFN0, ptex);
            _ctrlt0._th.Reset();
            _ctrlt0._th.LoadFile(ptex,0);
	        _ctrlt0.Repaint();

            ptex = (char*)GTexSys.GetTexName(pPoly->GetHtex(1));
            pdash = strchr(ptex,'?');
            if(pdash) *pdash =0;

            SetDlgItemText(ST_TFN1, ptex);
            _ctrlt1._th.Reset();
            _ctrlt1._th.LoadFile(ptex,0);
	        _ctrlt1.Repaint();


            ptex = (char*)GTexSys.GetTexName(pPoly->GetHtex(2));
            pdash = strchr(ptex,'?');
            if(pdash) *pdash =0;

            SetDlgItemText(ST_TFN2, ptex);
            _ctrlt2._th.Reset();
            _ctrlt2._th.LoadFile(ptex,0);
	        _ctrlt2.Repaint();

            ptex = (char*)GTexSys.GetTexName(pPoly->GetHtex(3));
            pdash = strchr(ptex,'?');
            if(pdash) *pdash =0;

            SetDlgItemText(ST_TFN3, ptex);
            _ctrlt3._th.Reset();
            _ctrlt3._th.LoadFile(ptex,0);
	        _ctrlt3.Repaint();


        }
    }
    _CATCHX()
    {
    }
}

void DlgFaceProp::OnColor() 
{
	CColorDialog    d( _pbColor._color, CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal()==IDOK)
    {
        _pbColor._color  = d.GetColor();
        _pbColor.Invalidate();
		Dirty();
        BARDLG()->OnApply();
        OnAction();

        _touchedIDs.insert(_pbColor.GetDlgCtrlID());
    }
}

void DlgFaceProp::OnEmmisive() 
{
    
	CColorDialog    d( m_e._color, CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal())
    {
        _touchedIDs.insert(m_e.GetDlgCtrlID());
        m_e._color  = d.GetColor();
        m_e.Invalidate();
		Dirty();
        BARDLG()->OnApply();
        OnAction();
        
    }
}

void DlgFaceProp::OnSpec() 
{
	CColorDialog    d( m_s._color, CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal())
    {
        
        _touchedIDs.insert(m_s.GetDlgCtrlID());
        m_s._color  = d.GetColor();
        m_s.Invalidate();
		Dirty();
        BARDLG()->OnApply();
        OnAction();
        
    }
}

void DlgFaceProp::OnDifuze() 
{
	CColorDialog    d( m_d._color, CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal()==IDOK)
    {
        _touchedIDs.insert(m_d.GetDlgCtrlID());
        m_d._color  = d.GetColor();
        m_d.Invalidate();
		Dirty();
        BARDLG()->OnApply();
        OnAction();
        
    }	
}

void DlgFaceProp::UseSpliter() 
{
}

void DlgFaceProp::OnResetsplits() 
{
    _last2usesplitidx=0;
    DOC()->_use2splitidx = 0;
    PBrushes* pBrushes   =  DOC()->_scene.GetPrimitives();
    if( 0 == pBrushes->size())
    {
	    return ;
    }

    FOREACH(PBrushes, (*pBrushes), ppBrsh)
    {
	    FOREACH(vvector<Poly>,(*ppBrsh)->_polys, pPoly)
	    {
            if((*ppBrsh)->_brushflags & BRSH_DETAIL)
            {
                if(&(*pPoly) == _pCurent)
                {
                    pPoly->_polyprops &= ~FACE_SPLITTER;
                }
                continue;
            }
		    pPoly->_polyprops &= ~FACE_SPLITTER;
            pPoly->_use2splitidx = -1;
	    }
    }    	
    
}

void DlgFaceProp::OnDestroy() 
{
	CVertDlgDlg::OnDestroy();
    gluDeleteQuadric(GluObj);	
}

void DlgFaceProp::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CVertDlgDlg::OnShowWindow(bShow, nStatus);
	
    if(0 == bShow){
		PCurent=0;
	}
	else
	{
        int count;
        Poly** pPoly = DOC()->GetSelPolys(count);
        if(count)
            Update(*pPoly, count);
	}
}

void DlgFaceProp::OnSelchangeEffects() 
{
    OnAction();	
}

void    DlgFaceProp::Reset()
{
    CVertDlgDlg::Reset();
    /*
    for(int i=0; i < m_lbprops.GetCount(); i++)
    {
        m_lbprops.SetCheck(i, 0);
    }
    */
}

void DlgFaceProp::OnGensc() 
{
	OnAction();
}

void DlgFaceProp::OnGensc2() 
{
	OnAction();
	
}

void DlgFaceProp::OnGenss() 
{
    OnAction();
}

void DlgFaceProp::OnGenss2() 
{
	OnAction();
	
}

void DlgFaceProp::OnResetTC() 
{
    int count;
    Poly** pPoly = DOC()->GetSelPolys(count);
    if(count)
    {
        for(int i=0;i<count;i++,pPoly++)        
        {
            (*pPoly)->ResetTexCoords();
        }
    }
    DOC()->Invalidate();
}

