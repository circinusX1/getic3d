// DlgNewTerrain.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "MainFrm.h"
#include "DlgNewTerrain.h"
#include "DlgBar.h"
#include "Brush.h"
#include "z_ed2Doc.h"
#include "z_ed2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TER_SZ  12

static V3 GetViewsDims()
{

    CZ_ed2View* pTX = VIEW2D(1,1);
    CZ_ed2View* pTY = VIEW2D(0,1);
    CZ_ed2View* pTZ = VIEW2D(1,0);

	V3	boxdim = V3(  pTZ->_vport.right-pTX->_vport.left,
					  pTX->_vport.bottom-pTZ->_vport.top,
			          pTX->_vport.right-pTX->_vport.left);
	boxdim *= .5f;
    return V3(1600,800,1600);//boxdim;
}
/////////////////////////////////////////////////////////////////////////////
// CDlgNewTerrain dialog


CDlgNewTerrain::CDlgNewTerrain(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgNewTerrain::IDD, pParent),_pWorkingBrush(0),_pStaticGL(0),b_big(FALSE)
{
	//{{AFX_DATA_INIT(CDlgNewTerrain)
	//}}AFX_DATA_INIT
}


void CDlgNewTerrain::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewTerrain)
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, PB_GENTERIAN, m_g);
	DDX_Control(pDX, IDOK, m_o);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewTerrain, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgNewTerrain)
	ON_WM_DESTROY()
	ON_BN_CLICKED(PB_GENTERIAN, OnGen)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_HANDLE_TEXSEL, HandelTextureSel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewTerrain message handlers


BOOL CDlgNewTerrain::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();

    _pStaticGL = FRM()->GetGLWidnow(this, IDC_STATICGL);
    DOC()->_viewChanged = 3;

    _dlgtex.Create( DLG_TEX, this);
    RECT rt; 
    GetDlgItem(ST_PLACE)->GetWindowRect(&rt);
    ScreenToClient(&rt);

    _dlgtex.ShowWindow(SW_SHOW);
    _dlgtex.MoveWindow(rt.left,rt.top,rt.right-rt.left, 
                     rt.bottom-rt.top,1);
    _dlgtex._barNoty=FALSE;
    _dlgtex.GetDlgItem(FRM_TEXMAP)->EnableWindow(0);
    _dlgtex.PostMessage(WM_COMMAND, ST_TEXFRM) ;
	return TRUE;  
}

void CDlgNewTerrain::OnDestroy() 
{
    DOC()->_viewChanged = 3;
    _pStaticGL->_pBrush = 0;

    FRM()->GetGLWidnow(0, IDC_STATICGL);
	CBASEDLG::OnDestroy();
	
}

void CDlgNewTerrain::OnGen() 
{
    CreateMesh();
}

void CDlgNewTerrain::OnOk() 
{

    CBASEDLG::OnOK();
}

void CDlgNewTerrain::OnCancel() 
{
    _pStaticGL->_pBrush = 0;
    delete _pWorkingBrush;
    _pWorkingBrush = 0;

	CBASEDLG::OnCancel();
}

void CDlgNewTerrain::CreateMesh()
{
    CWaitCursor cwc; // if not var is excluded in release(1);

   _pStaticGL->_pBrush = _pWorkingBrush;
   _pStaticGL->Invalidate();

   GetDlgItem(IDOK)->EnableWindow(0!=_pWorkingBrush);

}

void CDlgNewTerrain::OnPaint() 
{
    CPaintDC dc(this); 
    DOC()->SetFaceMode();
    _pStaticGL->GLPaint();	
}

int CDlgNewTerrain::GetVal (BYTE* pBuff, int x, int y)
{
    return *(pBuff + (y * _dimX + x));
}

void CDlgNewTerrain::PutVal (BYTE* pBuff, int x, int y, BYTE value)
{
    ASSERT((y * _dimX + x) < _dimX*_dimY);
    *(pBuff + (y * _dimX + x)) = value;
}

void CDlgNewTerrain::OnSetFocus(CWnd* pOldWnd) 
{
	CBASEDLG::OnSetFocus(pOldWnd);
}

LRESULT CDlgNewTerrain::HandelTextureSel(WPARAM csl, LPARAM texName)
{
    TexHandler  th;

    CWaitCursor cwc; // if not var is excluded in release(1);
    if(th.LoadFile((char*)texName,0))
    {
        BYTE* heightBuff =  new  BYTE[(TER_SZ+1) * (TER_SZ+ 1)];

	    BYTE* pw = new BYTE[TER_SZ*TER_SZ*3];
        {
            th.Resize(th.n_bpp, 
                    th.Buffer(), 
                    th.n_x , th.n_y ,
                    pw, 
                    TER_SZ, 
                    TER_SZ); 
        } 
        
        _dimX=_dimY=TER_SZ;
        th.n_x = th.n_y = TER_SZ;
        int j=0;
        for(int y=0; y < TER_SZ; y++)
        {
            for(int x=0; x < TER_SZ; x++)
            {
                PutVal (heightBuff, x, (TER_SZ-1)-y, (pw[j]+pw[j+1]+pw[j+2])/3);
                j+=3;
            }
        }
        
        delete[]pw; 
        delete _pWorkingBrush;

        _pWorkingBrush = new Brush();

        V3 meshDim  = GetViewsDims();

        meshDim.x = max(meshDim.x, TER_SZ * th.n_x);
        meshDim.y = max(meshDim.y, TER_SZ * th.n_y);
        meshDim.z = max(meshDim.z, TER_SZ * th.n_y);
   
        _pWorkingBrush->MakeTerrain(meshDim, th.n_x, th.n_y, heightBuff);

        delete[] heightBuff;

        _pWorkingBrush->_group = DOC()->_curCreatGroup;
        _pWorkingBrush->_brushflags |= BRSH_SOLID|BRSH_NEW|BRSH_CSGTERRAIN;
		_pWorkingBrush->ResetTexCoords();
        _pWorkingBrush->Rotate(V3(PIPE2,0,0));
		_pWorkingBrush->Reverse();

        _pWorkingBrush->_cf._tm      = TM_LIN;
        _pWorkingBrush->_cf._t       = E_TERRAIN;
        _pWorkingBrush->_cf._brushflags   = _pWorkingBrush->_brushflags;
        _pWorkingBrush->_cf._hstrips = TER_SZ;
        _pWorkingBrush->_cf._vstrips = TER_SZ;
        _pWorkingBrush->_cf._thick   = 0;


        CreateMesh();
    }

    return 1L;
}
