// 2D3DExtrude.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "staticgl.h"
#include "z_ed2Doc.h"
#include "mainfrm.h"

#include "2D3DExtrude.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
static C2D3DExtrude* PThis;

/////////////////////////////////////////////////////////////////////////////
// C2D3DExtrude message handlers
static void SizeHere(CWnd* pWnd, int iWidth, int iHeight)
{
    PThis->OnSize(0, iWidth, iHeight) ;
}

static BOOL SetCursorHere(UINT msg, V_ACTION act,  CPoint& p, V3& wp) 
{
    if(!PThis || !PThis->_pStaticGL)
        return FALSE;
    if(PThis->_pStaticGL->_nMode==V_3DFRONT||PThis->_pStaticGL->_nMode == V_3D)
        return FALSE;
    
    return PThis->OnSetCursor((UINT)act, p, wp);
}


static BOOL    MouseHere(UINT msg, V_ACTION act,  CPoint& p, V3& wp)
{
    switch(msg)
    {
        case WM_LBUTTONDOWN:
            return PThis->OnLButtonDown((UINT)act, p, wp);
        case WM_LBUTTONUP:
            return PThis->OnLButtonUp((UINT)act, p, wp);
        case WM_RBUTTONDOWN:
            return PThis->OnRButtonDown((UINT)act, p, wp);
        case WM_RBUTTONUP:
            return PThis->OnRButtonUp((UINT)act, p, wp);
        case WM_MOUSEMOVE:
            return PThis->OnMouseMove((UINT)act, p, wp);
    }
    return 0;
}

static void    PaintHere(CWnd* pWnd , V_MODE mode)
{
	if(PThis->_pStaticGL->_nMode==V_3D) 
    {
        if(PThis->_brush._polys.size())
        {
            BOOL            b = 0;
            vvector<Poly*>  pDummy(8);
            DOC()->DrawSolidBrush(PThis->_brush, 
                                  pDummy,
                                  pDummy,
                                  pDummy,
                                  pDummy,
                                  pDummy, 
                                  b);

        }
    }
    else if(PThis->_pStaticGL->_vt == 'y')
    {

        glColor3ubv(CLR_MOTION);
        glBegin(GL_LINE_STRIP);

        for(int i=0; i<PThis->_path.size(); i++)
        {
            const V3& vtn = PThis->_path[i];
            glVertex3f(vtn.x, vtn.y, vtn.z);
        }
        glEnd();

        glColor3ubv(CLR_DARK);
        glPointSize(4.0f);
        glBegin(GL_POINTS);
        for(i=0; i<PThis->_path.size(); i++)
        {
            const V3& vtn = PThis->_path[i];
            if(PThis->_selp>=0 && PThis->_selp==i)
                glColor3ubv(CLR_SELECT);
            else
                glColor3ubv(CLR_DARK);
            glVertex3f(vtn.x, vtn.y, vtn.z);
        }
        glEnd(); 
        glPointSize(1.0f);
        glColor3ubv(CLR_NORMAL);
    }
    
}


C2D3DExtrude::C2D3DExtrude(CWnd* pParent /*=NULL*/)
	: CBASEDLG(C2D3DExtrude::IDD, pParent)
{
	//{{AFX_DATA_INIT(C2D3DExtrude)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    PThis = this;
    _pStaticGL= 0;
    _selp     = -1;

}


void C2D3DExtrude::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C2D3DExtrude)
	DDX_Control(pDX, IDOK, m_cncl);
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, PB_EXTRUDE, m_b2);
	DDX_Control(pDX, PB_CLEAR, m_b1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C2D3DExtrude, CBASEDLG)
	//{{AFX_MSG_MAP(C2D3DExtrude)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(PB_CLEAR, OnClear)
	ON_BN_CLICKED(CK_HALLO, OnHallo)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C2D3DExtrude message handlers

void C2D3DExtrude::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

void C2D3DExtrude::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBASEDLG::OnCancel();
}

void C2D3DExtrude::OnClear() 
{
	// TODO: Add your control notification handler code here
	
}

void C2D3DExtrude::OnHallo() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL C2D3DExtrude::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	
	_pStaticGL = FRM()->GetGLWidnow(this, ST_PLACE);
    _pStaticGL->_pcbsetcursor = SetCursorHere;
    _pStaticGL->_pcbmouse     = MouseHere;
    _pStaticGL->_pcbpaint     = 0;//PaintHere;
    RECT rt;
    _pStaticGL->GetClientRect(&rt);
    SizeHere(_pStaticGL, rt.right, rt.bottom);
	
	return TRUE;  
	              
}

void C2D3DExtrude::OnDestroy() 
{
	CBASEDLG::OnDestroy();
	
    _pStaticGL->_pcbsz    = 0;
    _pStaticGL->_pcbmouse = 0;
    _pStaticGL->_pcbpaint = 0;
    _pStaticGL->HideButtons(FALSE);
	FRM()->GetGLWidnow(0, IDC_STATICGL);
}


BOOL C2D3DExtrude::OnLButtonDown(UINT nFlags, CPoint& point, V3& wp) 
{
   _mm._ldown = 1;
   _mm._ptd   = point;
   _mm._wpdown= wp;
    return 0;
}

BOOL C2D3DExtrude::OnLButtonUp(UINT nFlags, CPoint& point, V3& wp) 
{
    if(_mm._ldown)
    {
       _mm._ldown = 0;
       if(abs(_mm._ptd.x-point.x) < 2 && 
          abs(_mm._ptd.y-point.y) < 2 &&
		  AKEY(VK_CONTROL))
       {
			if(_pStaticGL->_nMode==V_2D &&
               _pStaticGL->_vt == 'y')
			{
				_selp = _path.size();
                _path.push_back(wp);
    			_pStaticGL->GLPaint();
			}
       }
    }
    return 0;
}

BOOL C2D3DExtrude::OnSetCursor(UINT nFlags, CPoint& point, V3 wp) 
{
    _selp = 0;
    for(int i=0; i<_path.size(); i++)
    {
        V3&    vtn = _path[i];
        V3      vmul(1,1,1);    
        
        if(_pStaticGL->_vt == 'y')
        {
            vmul.y=0;
        }

        V3  cp = vtn * vmul;
        V3  tp = wp * vmul;
        V3  df = cp - tp;
        if(df.x<0) df.x=-df.x;
        if(df.y<0) df.y=-df.y;
        if(df.z<0) df.z=-df.z;
        
        if(df.len() < 32.0){
            _selp = i; 
            SetCursor(theApp._cursors[CUR_MOVE]);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL C2D3DExtrude::OnMouseMove(UINT nFlags, CPoint& point, V3& wp) 
{
    if(nFlags == A_SHIFTVIEW)
    {
        // wp  has shift value
        if(_selp>=0)
        {
            int  pidx = _selp;
            V3& vtn  = _path[pidx];
            V3   t;            
            if('y'==_pStaticGL->_vt)
            {
                t.x = -wp.x;
                t.z = wp.z;
            }
            vtn += t;
            _pStaticGL->GLPaint();
            return 1;
        }
    }
    return 0;
}

BOOL C2D3DExtrude::OnRButtonDown(UINT nFlags, CPoint& point, V3& wp) 
{
	if(_selp>=0) 
	{
		for(vvector<V3>::iterator i = _path.begin();i != _path.end(); i++)
		{
			if(i-_path.begin() == _selp)
			{
				_path.erase(i);
				_pStaticGL->InvalidateRect(0);
				break;
			}
		}
	}

    return 0;
}

BOOL C2D3DExtrude::OnRButtonUp(UINT nFlags, CPoint& point, V3& wp) 
{
    return 0;
}


void C2D3DExtrude::OnSize(UINT nType, int cx, int cy) 
{
	CBASEDLG::OnSize(nType, cx, cy);
	
	
}
