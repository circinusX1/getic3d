// DlgBigTerrain.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "MainFrm.h"
#include "DlgBigTerrain.h"
#include "DlgBar.h"
#include "Brush.h"
#include "z_ed2Doc.h"
#include "z_ed2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgBigTerrain* PDlg;
//---------------------------------------------------------------------------------------
CDlgBigTerrain::CDlgBigTerrain(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgBigTerrain::IDD, pParent),_pStaticGL(0)
{
	//{{AFX_DATA_INIT(CDlgBigTerrain)
	//}}AFX_DATA_INIT
    PDlg = this;
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBigTerrain)
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, PB_GENTERIAN, m_g);
	DDX_Control(pDX, IDOK, m_o);
	//}}AFX_DATA_MAP
}

//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDlgBigTerrain, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgBigTerrain)
	ON_WM_DESTROY()
	ON_BN_CLICKED(PB_GENTERIAN, OnGen)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_HANDLE_TEXSEL, HandelTextureSel)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
static void FCbPaint(CWnd* pdc, V_MODE mode)
{
    if(PDlg)
        PDlg->_terrain.Render();
}

//---------------------------------------------------------------------------------------
BOOL CDlgBigTerrain::OnInitDialog() 
{
    TerrTiles       = 64;
    TerLeafCelTiles = 16;
    CBASEDLG::OnInitDialog();
    _untiledTex[0] = 0;
    _untiledTex[1] = 0;
    _untiledTex[2] = 0;
    _untiledTex[3] = 0;
    _pStaticGL = FRM()->GetGLWidnow(this, IDC_STATICGL);
    _pStaticGL->_pcbpaint     = FCbPaint;


    DOC()->_viewChanged = 3;
    _dlgtex.Create( DLG_TEX, this);
    RECT rt; 
    GetDlgItem(ST_PLACE)->GetWindowRect(&rt);
    ScreenToClient(&rt);
    _dlgtex.ShowWindow(SW_SHOW);
    _dlgtex.MoveWindow(rt.left,rt.top,rt.right-rt.left, 
                     rt.bottom-rt.top,1);
    _dlgtex._barNoty=FALSE;
    GetDlgItem(CK_NOTILE)->EnableWindow(1);

    CheckDlgButton(CK_NOTILE, BST_CHECKED);
    GetDlgItem(CK_NOTILE1)->EnableWindow(1);
    GetDlgItem(CK_NOTILE2)->EnableWindow(1);
    GetDlgItem(CK_NOTILE3)->EnableWindow(1);
    _dlgtex.GetDlgItem(FRM_TEXMAP)->EnableWindow(0);
    _dlgtex.PostMessage(WM_COMMAND, ST_TEXFRM) ;
    _updatingCtrls = 0;
	return TRUE;  
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::OnDestroy() 
{
    DOC()->_viewChanged = 3;
    _pStaticGL->_pBrush = 0;
    FRM()->GetGLWidnow(0, IDC_STATICGL);
	CBASEDLG::OnDestroy();
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::OnGen() 
{
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::OnOk() 
{
    CBASEDLG::OnOK();
    _untiledTex[0] = IsDlgButtonChecked(CK_NOTILE);
    _untiledTex[1] = IsDlgButtonChecked(CK_NOTILE1);
    _untiledTex[2] = IsDlgButtonChecked(CK_NOTILE2);
    _untiledTex[3] = IsDlgButtonChecked(CK_NOTILE3);

    
    _untiledTex[0] ? _terrain.Flags() |= TERR_UTT0:0;                       // transfer
    _untiledTex[1] ? _terrain.Flags() |= TERR_UTT1:0;
    _untiledTex[2] ? _terrain.Flags() |= TERR_UTT2:0;
    _untiledTex[3] ? _terrain.Flags() |= TERR_UTT3:0;


}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::OnCancel() 
{
	CBASEDLG::OnCancel();
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::OnPaint() 
{
    CPaintDC dc(this); 
    DOC()->SetFaceMode();
    _pStaticGL->GLPaint();	
}

//---------------------------------------------------------------------------------------
int CDlgBigTerrain::GetVal (BYTE* pBuff, int x, int y)
{
    return *(pBuff + (y * _dimX + x));
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::PutVal (BYTE* pBuff, int x, int y, BYTE value)
{
    ASSERT((y * _dimX + x) < _dimX*_dimY);
    *(pBuff + (y * _dimX + x)) = value;
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::OnSetFocus(CWnd* pOldWnd) 
{
	CBASEDLG::OnSetFocus(pOldWnd);
}

//---------------------------------------------------------------------------------------
LRESULT CDlgBigTerrain::HandelTextureSel(WPARAM csl, LPARAM texName)
{
    TexHandler  th;
    CWaitCursor cwc;

    int nTiles = TerrTiles+1;

    if(th.LoadFile((char*)texName,0))
    {
        BYTE* heightBuff =  new  BYTE[nTiles*nTiles];
        if(heightBuff)
        {
	        BYTE* pw = new BYTE[nTiles*nTiles*3];
            memset(pw,128,nTiles*nTiles*3);
            if(pw)
            {
                th.Resize(th.n_bpp, th.Buffer(), th.n_x , th.n_y , pw, TerrTiles, TerrTiles); 
                _dimX  = _dimY   = TerrTiles;
                th.n_x = th.n_y  = TerrTiles;

                int j=0;
                for(int y=0; y < TerrTiles; y++)
                {
                    for(int x=0; x < TerrTiles; x++)
                    {
                        PutVal (heightBuff, x, (TerrTiles-1)-y, (pw[j]+pw[j+1]+pw[j+2])/3);
                        j+=3;
                    }
                }
                
                delete[]pw; 
                _ReplaceTerrain(heightBuff);
            }
            delete[] heightBuff;
        }
    }
    return 1L;
}

//---------------------------------------------------------------------------------------
static V3 GetViewsDims()
{
    
    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);
	V3	boxdim = V3(  pTZ->_vport.right-pTX->_vport.left,
					  pTX->_vport.bottom-pTZ->_vport.top,
			          pTX->_vport.right-pTX->_vport.left);
    
    return V3(6400,3200,6400);//boxdim;
}

//---------------------------------------------------------------------------------------
void CDlgBigTerrain::_ReplaceTerrain(BYTE* heightBuff)
{

    if(0 == heightBuff)
    {
        GetDlgItem(IDOK)->EnableWindow(0);
    }
    else
    {
        V3 vd    = GetViewsDims();
        V3 start = -(vd/2.0);
        
        _terrain.Create(heightBuff, start, vd, TerrTiles, TerrTiles); 
        GetDlgItem(IDOK)->EnableWindow(1);
    }
    _pStaticGL->Invalidate();
}
//---------------------------------------------------------------------------------------
