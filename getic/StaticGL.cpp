// @@@@@@@@@@@@@
// 
// StaticGL.cpp : implementation file
//

#include "stdafx.h"
#include "texadapter.h"
#include "z-edmap.h"
#include "TexRef.h"
#include "StaticGL.h" 
#include "z_ed2Doc.h"
#include "dlgbar.h"
#include "z_ed3View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticGL

static REAL _Depth = 1;
static REAL _Fov   = 90;

BEGIN_MESSAGE_MAP(CStaticGL, CWnd)
	//{{AFX_MSG_MAP(CStaticGL)
	ON_LBN_SELCHANGE(LB_PREFS, OnSelchangePrefs)
	ON_LBN_DBLCLK(LB_PREFS, OnDblclkPrefs)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
    ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStaticGL message handlers
static BOOL BOnCreate=FALSE;
void CStaticGL::OnSelchangePrefs() 
{
	// TODO: Add your control notification handler code here
	
}

void CStaticGL::OnDblclkPrefs() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CStaticGL::CreateWnd(CWnd* parent, UINT isctrl)
{
    BOnCreate = TRUE;
    RECT rt={0,0,1,1};
    CWnd* hwPrev = 0;//
    if(isctrl)
    {
        hwPrev = parent->GetDlgItem(isctrl)->GetNextWindow(GW_HWNDPREV);
        parent->GetDlgItem(isctrl)->GetWindowRect(&rt);
        parent->ScreenToClient(&rt);
        parent->GetDlgItem(isctrl)->ShowWindow(SW_HIDE);
        parent->GetDlgItem(isctrl)->DestroyWindow();
    }
    BOOL b = Create(theApp._cName.c_str(), "", WS_CHILD, rt, parent, isctrl+9876);
    if(isctrl)
    {
	    SetWindowPos(hwPrev,0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
        SetDlgCtrlID(isctrl);
    }
    _backgrnd[0]=.0;
    _backgrnd[1]=.0;
    _backgrnd[2]=.0;
	return b;
}


int CStaticGL::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    _hdc = ::GetDC(m_hWnd);   
    if(_pcbinit)
        _pcbinit(this);
    else
    {
        if(!DOC()->InitGL(3,_hdc, m_hRC))
        {
            PostQuitMessage(0);
            return -1;
        }

        z_ed3View*  pT3 = (z_ed3View *)FRM()->m_wndSplitter.GetPane(0,0);
        wglShareLists(pT3->m_hRC, m_hRC);
        DOC()->SetFaceMode();
    }

    _zoom    = .3f;                 //initial zoom
    _shifty  = 0.0f;
    _shiftx  = 0.0f;
    _shiftz  = 0.0f; 
    _moving  = 0;
    _scaling = 0;
    _nMode   = V_3DFRONT;
	_pushMode= 0; 
    _vt      = 'z';
    _bHide   = FALSE;   
    _vireframe = 0;
    _lighting  = 0;
    _cam.SetFov(2*PIPE6); 
    _cam.SetFar(25600.0);//250m
	_cam.SetAngles(-PIPE6,-PIPE6,0);
	_cam._pos = V3(3200.0f,3200.0f,1600.0f);
    _cam.Euler2Pos();
	

	return 0;
}


void CStaticGL::InitialUpdate()
{
    MakeCurrent(_hdc, m_hRC); 
	_tbartex = _tbtex;//hTex.hTex

    REAL    as = (REAL)400/(REAL)300;

    _bhght    = tan(PIPE4)/8;
	REAL butx = 1.0f/(REAL)BUTS;

	for(int i=0;i<BUTS;i++)
	{
		_toolbar[i].MakeSheet(V3(_bhght, _bhght,0),1,1);
	

		FOREACH(Polys, _toolbar[i]._polys, pp)
		{
            Poly * p =&(*pp);
			p->_polyprops&=~FACE_SHOWBACK;
		
			p->SetHtex(_tbartex, TX_0);

			p->_texcoord.scalex = butx;
			p->_texcoord.shiftx = butx*i;
			p->_texcoord.shifty = 0; 
			p->_texcoord.scaley = 1;

			p->ApplyNewTexCoord();

			
		
		    if(_pushMode==i)
		    {
			    p->_colorD =CLR(255,255,255);
		    }
		    else
		    {
			    p->_colorD =CLR(192,192,192);
		    }

            if(_vireframe && BUTS==i)
            {
			    FOREACH(Polys, _toolbar[i]._polys, p)
			    {
				    p->_colorD =CLR(255  ,255,255);
			    }
            }
        }

	}

    BOnCreate = FALSE;
}

void CStaticGL::OnDestroy() 
{
    //KillTimer(334);
    _tbartex.Reset();
    MakeCurrent(_hdc, m_hRC); 
    //wglDeleteContext(m_hRC);
    ::ReleaseDC(m_hWnd,_hdc);
    MakeCurrent(0,0);

	CWnd::OnDestroy();
}

    
void CStaticGL::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    GLPaint();
}


void CStaticGL::OnMouseMove(UINT nFlags, CPoint pt) 
{
    REAL mxw = Wnd2World(pt.x, 'h');
    REAL myw = Wnd2World(pt.y, 'v');
    _curPos = Wnd2World2(pt.x, pt.y);
    switch(_nMode)
    {
        case V_3DFRONT:
            if(_mm._rdown)
            {
                int dz = pt.y-_mm._pt.y;
                _dist -= dz*10;
                _mm._pt = pt;
                if(_dist>0)
                    _dist=0;
                if(_dist<-8000)
                    _dist=-8000;
                GLPaint();
            }else if(_mm._ldown)
            {
                REAL dx = (REAL)(pt.x-_mm._pt.x)/1.0f;
                REAL dy = (REAL)(pt.y-_mm._pt.y)/1.0f;
                _angle.x+=dy;
                _angle.y+=dx;
                _mm._pt = pt;
                GLPaint();
            }
            break;
        case V_3D:
            {
                V3      scrPt;
                CPoint  ptl = _mm._pt - pt;
                _mm._pt   = pt; //due to SetCursor Pos
              

                if(_mm._rdown || _mm._ldown)
                {
		            if(!AKEY(VK_LBUTTON))
			            _mm._ldown=0;

		            if(!AKEY(VK_RBUTTON))
			            _mm._rdown=0;
                }
                if(_mm._ldown)
                {
                    if(abs(ptl.x) > 0|| 
                       abs(ptl.y) > 0)
                    {
                        if(_mm._rdown)
                        {
                            _cam.MoveUp(ptl.y*2.f);
    			            _cam.MoveSide(-ptl.x*2.f);
	                    }
			            else
			            {
				            _cam.Rotate(-ptl.x/128.f,-ptl.y/128.f,0);
    		            }
                    }
                    GLPaint();
	            }
                else if(_mm._rdown)
                {

                    _cam.MoveFwd(ptl.y*4.f);

                    GLPaint();
                }
                SBT(1,MKSTR("%03.2f %03.2f z:%03.2f (%1.3f)",	_cam._pos.x/GMeters,
																		_cam._pos.y/GMeters,
																		_cam._pos.z/GMeters,
																		_zoom));

            }
            break;
        case V_2D:

            if(_mm._ldown)  // shift
            {
                CPoint ptl = _mm._pt - pt;
                _mm._pt    = pt;
                V3          shift;

                switch(_vt)
                {
                    case 'x':
                        shift.z += (REAL)ptl.x / _zoom;
                        shift.y -= (REAL)ptl.y / _zoom;
                        break;
                    case 'y':
                        shift.x += (REAL)ptl.x / _zoom;
                        shift.z -= (REAL)ptl.y / _zoom;
                        break;
                    case 'z':
                        shift.x += (REAL)ptl.x / _zoom;
                        shift.y -= (REAL)ptl.y / _zoom;
                        break;
                }

                if(_pcbmouse)
                {
                    // if we handle the shift dont shift anymore
                    if(!(_pcbmouse)(WM_MOUSEMOVE, A_SHIFTVIEW, pt, shift))
                    {
                        _shiftx += shift.x;
                        _shifty += shift.y;
                        _shiftz += shift.z;
                        
                    }
                }
            }
            else
            if(_mm._rdown)  //scale
            {
                CPoint ptl = _mm._pt - pt;
                _mm._pt   = pt;

                if(_zoom<.01)
                    _zoom += (REAL)ptl.y/8256.0;
                else if(_zoom<.1)
                    _zoom += (REAL)ptl.y/1256.0;
                else
                    _zoom += (REAL)ptl.y/256.0;

	            CLAMPVAL(_zoom,.002, 8.f);
            }
            GLPaint();
            break;
    }
	CWnd::OnMouseMove(nFlags, pt);
}

void CStaticGL::OnLButtonDown(UINT nFlags, CPoint pt) 
{
    if(GetCapture()!=this)
        SetCapture();

	if(TestButtons(pt,1))
		return;
    _mm._ldown = 1;
    _mm._pt    = pt;
    _mm._ptd   = pt;

    if(_pcbmouse)
    {
        _curPos = Wnd2World2(pt.x, pt.y);
        (_pcbmouse)(WM_LBUTTONDOWN, A_NONE, pt, _curPos);
    }
    
    CWnd::OnLButtonDown(nFlags, pt);
    GetParent()->SetFocus();
}

void CStaticGL::OnLButtonUp(UINT nFlags, CPoint pt) 
{
    if(GetCapture()==this)
        ReleaseCapture();
    if(TestButtons(pt,0)){
        SetCursor(LoadCursor(0,IDC_CROSS));
		return;
    }
    _mm._ldown = 0;

    _curPos = Wnd2World2(pt.x, pt.y);
    if(_pcbmouse)
    {
        (_pcbmouse)(WM_LBUTTONUP, A_SHIFTVIEW, pt, _curPos);
    }
    

	CWnd::OnLButtonUp(nFlags, pt);
}

void CStaticGL::OnRButtonDown(UINT nFlags, CPoint pt) 
{
    if(GetCapture()!=this)
        SetCapture();
    _mm._rdown = 1;
    _mm._pt = pt;
    _mm._ptd   = pt;
    if(_pcbmouse)
    {
        _curPos = Wnd2World2(pt.x, pt.y);
        (_pcbmouse)(WM_RBUTTONDOWN, A_NONE, pt, _curPos);
    }
	CWnd::OnRButtonDown(nFlags, pt);
    GetParent()->SetFocus();
}

void CStaticGL::OnRButtonUp(UINT nFlags, CPoint pt) 
{
    if(GetCapture()==this)
        ReleaseCapture();
    _mm._rdown = 0;
    if(_pcbmouse)
    {
        _curPos = Wnd2World2(pt.x, pt.y);
        (_pcbmouse)(WM_RBUTTONUP, A_NONE, pt, _curPos);
    }
	CWnd::OnRButtonUp(nFlags, pt);
}

void CStaticGL::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
}

BOOL CStaticGL::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam==VK_TAB)
			BARDLG()->PostMessage(WM_KEYTAB);		
		else if(GetParent())
			GetParent()->PostMessage(WM_KEYDOWN, pMsg->wParam,0);		
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CStaticGL::OnMButtonDown(UINT nFlags, CPoint pt) 
{
	
	CWnd::OnMButtonDown(nFlags, pt);
}

void CStaticGL::OnMButtonUp(UINT nFlags, CPoint pt) 
{
	CWnd::OnMButtonUp(nFlags, pt);
}


void    CStaticGL::GLPaint()
{
    RECT rt;
    GetClientRect(&rt);

    if(FRM()->m_bshutdown)return;
    
    MakeCurrent(_hdc, m_hRC); 
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    Perspective(rt.right, rt.bottom); // perspective
    DOC()->SetFaceMode();

    if(_pcbpreppaint)
        _pcbpreppaint(this ,_nMode, 1);
 
    glPushMatrix();
    glEnable(GL_BLEND);

    glColor3f(1.0, 1.0f, 1.0);

    switch(_nMode)
    {
        case V_3DFRONT:
            glTranslatef(0,0,_dist);
	        glRotatef(_angle.y,0,1,0);
	        glRotatef(_angle.x,1,0,0);
            if(b_showGrid)
                DrawGrids();
            break;
        case V_3D:
            {
	            const M4&		mt = _cam.GetViewMtx(0);
                _cam.UpdateFrustrumPlanes();
                glLoadMatrixf((REAL*)&mt);
                if(b_showGrid)
                    DrawGrids();
            }
            break;
        case V_2D:
            Ta.Disable();
            if(b_showGrid)
                DrawGrids();
            switch(_vt)
            {
                case 'x':
                    glRotatef(90,0,1,0);
                    break;
                case 'y':
                    glRotatef(90,1,0,0);
                    break;
                case 'z':
                    break;
            }
            break;
    }
    glDisable(GL_BLEND);    
    if(_pcbpaint)
    {
        _pcbpaint(this ,_nMode);
    }
    else
    if(_pBrush)
	{
        BOOL    globalLight ;

        if(_vireframe)
        {
            Ta.Disable();
            glEnable(GL_CULL_FACE);
		    glFrontFace(GL_CCW);
		    glCullFace(GL_FRONT);
	        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            Ta.Enable();
        }
        if(0)//_lighting)
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);
            DOC()->ReposCamLight(BIT_AMBIENT|BIT_SPECULAR, 0);
            globalLight = GLight;
            GLight = 1;
        }

        vvector<Poly*>  pDummy(8);
        DOC()->DrawSolidBrush(*_pBrush, pDummy,pDummy,pDummy,pDummy,pDummy, _bsel);

        if(_vireframe)
        {
            glEnable(GL_CULL_FACE);
		    glFrontFace(GL_CCW);
		    glCullFace(GL_FRONT);
	        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if(0)//_lighting)
        {
            glDisable(GL_NORMALIZE);
            glDisable(GL_LIGHTING);
            GLight = globalLight;
        }

	}

    glDisable(GL_BLEND);    

    if(V_2D == _nMode)
    {
        if(b_showGrid)
            DOC()->PrintCoords(_rt, _vport, _curPos, _vt, 0,0,0);
    }

    glPopMatrix();


    //draw the view modes
    if(_bHide == FALSE)
        DrawViewModes();

    if(_pcbpreppaint)
        _pcbpreppaint(this ,_nMode, 0);

    if(DOC()->_glAddSwapHintRectWIN)
        (DOC()->_glAddSwapHintRectWIN)(0,0,_rt.right,_rt.bottom);
    SwapBuffers(_hdc);
    //GdiFlush();
}

/*
void CStaticGL::OnTimer(UINT nIDEvent) 
{
    CWnd::OnTimer(nIDEvent);
}
*/

BOOL CStaticGL::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if(_pcbsetcursor)
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        V3 tp = Wnd2World2(pt.x, pt.y);

        if( (_pcbsetcursor)(WM_MOUSEMOVE, A_NONE, pt, _curPos))
            return 1; // cursor has been set
    }
    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CStaticGL::OnSize(UINT nType, int iWidth, int iHeight) 
{
	CWnd::OnSize(nType, iWidth, iHeight);
    MakeCurrent(_hdc, m_hRC); 
    if(iHeight == 0)
        iHeight = 1;
    if(_pcbsz)
    {
        _pcbsz(this, iWidth, iHeight);
        return;
    }
    Perspective(iWidth,iHeight);
    GetClientRect(&_rt);
}

void CStaticGL::Perspective(int iWidth, int iHeight) 
{
    switch(_nMode)
    {
        case V_3DFRONT:
            {
                glClearColor(_backgrnd[0], _backgrnd[1], _backgrnd[2], 0.0f);
                glViewport(0, 0, iWidth, iHeight);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(_Fov,(GLfloat)iWidth/(GLfloat)iHeight, _Depth, 35535);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
            }
            break;
        case V_3D:
            {
                glViewport(0, 0, iWidth, iHeight);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(_Fov,(GLfloat)iWidth/(GLfloat)iHeight, _Depth, 35535);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
            }
            break;
        case V_2D:
                glViewport(0, 0, iWidth, iHeight);
                glClearColor(_backgrnd[0], _backgrnd[1], _backgrnd[2], 0.0f);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();

                switch(_vt)
                {
                        case 'x':
                            _vport.left   = -(((REAL)iWidth * 0.5)   / _zoom)   + _shiftz;
                            _vport.right  =  (((REAL)iWidth * 0.5)   / _zoom)   + _shiftz;
                            _vport.top    = -(((REAL)iHeight * 0.5)  / _zoom)   + _shifty;
                            _vport.bottom  = (((REAL)iHeight * 0.5)  / _zoom)   + _shifty;
                            break;
                        case 'y':
                            _vport.left   = -(((REAL)iWidth * 0.5)   / _zoom)   + _shiftx;
                            _vport.right  =  (((REAL)iWidth * 0.5)   / _zoom)   + _shiftx;
                            _vport.top    = -(((REAL)iHeight * 0.5)  / _zoom)   + _shiftz;
                            _vport.bottom  = (((REAL)iHeight * 0.5)  / _zoom)   + _shiftz;
                            break;
                        case 'z':
                            _vport.left   = -(((REAL)iWidth * 0.5)   / _zoom)   + _shiftx;
                            _vport.right  =  (((REAL)iWidth * 0.5)   / _zoom)   + _shiftx;
                            _vport.top    = -(((REAL)iHeight * 0.5)  / _zoom)   + _shifty;
                            _vport.bottom  = (((REAL)iHeight * 0.5)  / _zoom)   + _shifty;
                            break;
                }

                glOrtho(_vport.left, _vport.right, _vport.top , _vport.bottom, -32768, 32768);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
            break;
    }
   
}

V3  CStaticGL::Wnd2World2(int x, int y )
{
    V3   rez;
//    REAL mxw = Wnd2World(x, 'h');
//    REAL myw = Wnd2World(y, 'v');
	int      itempX = 0;
    int      itempY = 0;
	REAL     fWp = 0.0;
    RECT     rt;
    GetClientRect(&rt);

    switch(_vt)
    {
        case 'x':
            itempX = (int)(x + (-0.5 * rt.right));
            itempY = (int)(-1 * (y + (-0.5 * rt.bottom)));
            rez.z = itempX / _zoom + _shiftz;
            rez.y = itempY / _zoom + _shifty;
            break;
        case 'y':
            itempX = (int)(x + (-0.5 * rt.right));
            itempY = (int)(y + (-0.5 * rt.bottom));
            rez.x = itempX / _zoom + _shiftx;
            rez.z = itempY / _zoom - _shiftz;
            break;
        case 'z':
            itempX = (int)(x + (-0.5 * rt.right));
            itempY = (int)(-1 * (y + (-0.5 * rt.bottom)));

            rez.x = itempX / _zoom + _shiftx;
            rez.y = itempY / _zoom + _shifty;
            break;
    }
    return rez;
}

REAL    CStaticGL::Wnd2World(REAL coord, char dir)
{
	int      itemp = 0;
	REAL     fWp = 0.0;
    RECT     rt;
    GetClientRect(&rt);

    if(dir=='h')
    {
        itemp = (int)(coord + (-0.5 * rt.right));
		fWp = itemp / _zoom + _shiftx;
    }
    else
    {
        if(_vt != 'y')
        {
			itemp = (int)(-1 * (coord + (-0.5 * rt.bottom)));
			fWp = itemp / _zoom + _shifty;
        }
        else
        {
			itemp = (int)(coord + (-0.5 * rt.bottom));
			fWp = itemp / _zoom - _shifty;
        }
    }
    return fWp;
}


void CStaticGL::DrawGrids()
{
    int _gridSize     = 10001;
    int _gridStep     = 20;
    REAL r;

    glEnable(GL_BLEND);

    switch(_nMode)
    {
        case V_3DFRONT:
            DOC()->DrawAxis(V3(0,0,0), 8000);
            break;
        case V_3D:
            DOC()->DrawAxis(V3(0,0,0), 8000);
            break;
        case V_2D:
           
             glColor4ub(255,0,0,255);

            glBegin(GL_LINES);
                glVertex3f(-_gridSize/2,0,0);
                glVertex3f(_gridSize/2,0,0);

                glVertex3f(0,-_gridSize/2,0);
                glVertex3f(0,_gridSize/2,0);
            glEnd();

            glColor4ub(53,53,53,64);
            REAL gs = _gridStep*10;
            glBegin(GL_LINES);
            for( r = -_gridSize/2; r<= _gridSize/2; r+=gs)
            {
                glVertex3f(r,-_gridSize/2,0);
                glVertex3f(r,_gridSize/2,0);
            }

            for(r = -_gridSize/2; r<= _gridSize/2; r+=gs)
            {
                glVertex3f(-_gridSize/2,r,0);
                glVertex3f(_gridSize/2,r,0);
            }
            glEnd();
        break;
    }
    glDisable(GL_BLEND);
}


void    CStaticGL::DrawViewModes()
{
    RECT    rt;
    GetClientRect(&rt);
    REAL    as = (REAL)rt.right/(REAL)rt.bottom;
    REAL    dy = tan(PIPE4);
    REAL    dx = tan(PIPE4)*as;
	REAL	offset=_bhght/2;

    glViewport(0, 0, rt.right, rt.bottom);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_Fov,(GLfloat)rt.right/(GLfloat)rt.bottom, _Depth, 8000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Ta.Enable();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	vvector<Poly*>  pDummy(8);
   
	for(int i=0;i<BUTS;i++)
	{
	    glPushMatrix();
		glTranslatef(-dx + offset,-dy+_bhght/2,-1.0001);
        BOOL dmw=DOC()->_vmodefire;

		DOC()->DrawSolidBrush(_toolbar[i], pDummy,pDummy,pDummy,pDummy,pDummy, _bsel);        
		
        glPopMatrix();
		
		offset+=_bhght;
		offset+=_bhght/10;
	}


}

void    CStaticGL::UpdateButSels()
{
  if(_bHide)
        return ;

	int     itemp  = 0;

    for(int i=0;i<BUTS;i++)
	{
		if(_pushMode == i)
		{
			FOREACH(Polys, _toolbar[i]._polys, p)
			{
				p->_colorD = CLR(220,220,220);
			}
		}
		else
		{
			FOREACH(Polys, _toolbar[i]._polys, p)
			{
				p->_colorD = CLR(192,192,192);
			}
		}
	}
}

BOOL    CStaticGL::TestButtons(CPoint& pt, BOOL bGoDown)
{
    if(_bHide)
        return FALSE;
	RECT	rt;
    GetClientRect(&rt);

    REAL    as     = (REAL)rt.right/(REAL)rt.bottom;
    REAL    dy     = tan(PIPE4);
    REAL    dx     = tan(PIPE4)*as;
	REAL	offset =_bhght/2;
	REAL	cx     = 0;
	REAL	cy     = 0;
	int     itemp  = 0;

	REAL    cury   = -((REAL)(pt.y*2)/(REAL)rt.bottom)+1;

	if(cury > -dy+_bhght)
		return 0;

	REAL    curx   =  (((REAL)(pt.x*2)/(REAL)rt.right)-1)*as;
	BOOL	update = FALSE;
    int     ppus = _pushMode;

    for(int i=0;i<BUTS;i++)
	{
		cx = (offset-dx); 
		if(curx > cx-_bhght/2 && curx < cx+_bhght/2)
		{
			FOREACH(Polys, _toolbar[i]._polys, p)
			{
                if(bGoDown)
				    p->_colorD = CLR(255,255,255);
                else
                    p->_colorD = CLR(220,220,220);
			}
			update    = TRUE;
			_pushMode = i;
		}
		else
		{
			FOREACH(Polys, _toolbar[i]._polys, p)
			{
				p->_colorD = CLR(192,192,192);
			}
		}
		offset+=_bhght;
		offset+=_bhght/10;
	}

    if(bGoDown)
    {
        GLPaint();
        return update;
    }

	switch(_pushMode )
	{
		case 0:
			_nMode=V_3DFRONT;
			break;
		case 1:
			_nMode=V_3D;
			break;
		case 2:
			_nMode=V_2D;
			_vt='x';
			break;
		case 3:
			_nMode=V_2D;
			_vt='y';
			break;
		case 4:
			_nMode=V_2D;
			_vt='z';
			break;
		case 5:
            _vireframe=!_vireframe;
            _pushMode = ppus;
			break;
	}

    if(_vireframe )
    {
		FOREACH(Polys, _toolbar[5]._polys, p)
		{
			p->_colorD =CLR(255,255,255);
		}
    }
    else
    {
        FOREACH(Polys, _toolbar[5]._polys, p)
		{
			p->_colorD =CLR(192,192,192);
		}

    }


	if(update)
	{
		GLPaint();
	}
	return update;
}


