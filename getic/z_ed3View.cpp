//---------------------------------------------------------------------------------------
// Author Marius Chincisan:
// Started Sept 2003
// z_ed3View.cpp : implementation file

//---------------------------------------------------------------------------------------
#include "stdafx.h"
#include "texadapter.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "z_ed3View.h"
#include "camera.h"
#include "MainFrm.h"

//---------------------------------------------------------------------------------------
z_ed3View* z_ed3View::P3DView;

//---------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(z_ed3View, CView)

//---------------------------------------------------------------------------------------
z_ed3View::z_ed3View()
{
    _rotating           = FALSE;
    _fov                = 64.0f;
    _fnear              = 4.0f;
    _ffar               = SCENE()._si.camFar;
    z_ed3View::P3DView  = this;
	_capture            = 0;
	_scrPoint.x         = 0;
    _downKey            = 0;
    _blockKeyCall       = 0;
    _blockMouseCall     = 0;

}

//---------------------------------------------------------------------------------------
z_ed3View::~z_ed3View()
{
}

//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(z_ed3View, CView)
	//{{AFX_MSG_MAP(z_ed3View)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_KEYDOWN()
    ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
    ON_MESSAGE(WM_POSTINIT, OnPostInit)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
// z_ed3View drawing
void z_ed3View::OnDraw(CDC* pDC)
{
}

//---------------------------------------------------------------------------------------
// z_ed3View diagnostics

#ifdef _DEBUG
void z_ed3View::AssertValid() const
{
	CView::AssertValid();
}

void z_ed3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

//---------------------------------------------------------------------------------------
void z_ed3View::OnPaint() 
{
	CPaintDC dc(this); 
	DrawInHrc(dc.m_hDC);
}

//---------------------------------------------------------------------------------------    
void z_ed3View::DrawInHrc(HDC hdc, BOOL update)
{
    if(FRM()->m_bshutdown) return;

	MakeCurrent(hdc, m_hRC); 

    Ta.Enable();

    GetClientRect(&_rt);
    if(_rt.right && _rt.bottom)
    {
        Project3D(_rt.right, _rt.bottom);	
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        GLPaint();
        if(DOC()->_glAddSwapHintRectWIN)
            (DOC()->_glAddSwapHintRectWIN)(0,0,_rt.right,_rt.bottom);
        SwapBuffers(hdc);
        if(update)
            glFlush();
    }
	MakeCurrent(hdc, 0); 
}

//---------------------------------------------------------------------------------------
void  z_ed3View::Project3D(int iWidth, int iHeight)
{
	MakeCurrent(_hdc, m_hRC); 

    if(iHeight == 0)
        iHeight = 1;

    if(DOC()->b_whitebk)
    {
        glClearColor(.8f, .8f, .8f, 0.0f);
    }
    else
    {
        if(DOC()->_bShowCsg)
            glClearColor(0.11f, 0.11f, 0.11f, 0.0f);
        else
            glClearColor(.02f, .02f, .04f, 0.0f);
    }
    REAL farCam = _ffar;
/*
    if(SCENE()._si.fogFar)
        farCam = SCENE()._si.fogFar*_ffar;
*/
    
    glViewport(0, 0, iWidth, iHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_fov,(GLfloat)iWidth/(GLfloat)iHeight, _fnear, farCam);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//	MakeCurrent(_hdc, 0); 
}

//---------------------------------------------------------------------------------------
BOOL z_ed3View::PreCreateWindow(CREATESTRUCT& cs) 
{
//    cs.lpszClass = theApp._cNameViews.c_str();

    cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
                                      LoadCursor(0,IDC_CROSS),
                                     (HBRUSH)0,0);

	return CView::PreCreateWindow(cs);
}

//---------------------------------------------------------------------------------------
int z_ed3View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    _hdc = ::GetDC(m_hWnd);   
    if(!DOC()->InitGL(3,_hdc, m_hRC))
    {
        PostQuitMessage(0);
        return -1;
    }
	
    glPolygonMode(GL_BACK,GL_FILL);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

    BuildFont(_hdc);
	MakeCurrent(0, 0); 
    ++DOC()->_viewCount;
/*
    MEM_START(a,"a");
    do{
        V3      v(1.0,1.0,1.0);
        Brush   b;
        Brush   b1;
        b.MakeSheet(v,1,1);

        Brush   b2 = b;
        b1 = b;

    }while(0);
    MEM_END(a);
*/

	return 0;
}

//---------------------------------------------------------------------------------------
void z_ed3View::ReleaseRDI()
{
    ////(HFONT)SelectObject(_hdc, 0); 
    ::DeleteObject(_hf);

    MakeCurrent(_hdc, m_hRC); 
    
	glDeleteLists(_font1,   96);
	TexRef::Clear(1);
    
    ::ReleaseDC(m_hWnd,_hdc);
    MakeCurrent(0,0); 
   // wglDeleteContext(m_hRC);
    
	
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnDestroy() 
{
    Ta.Disable();
	CView::OnDestroy();
    --DOC()->_viewCount;
    
}

//---------------------------------------------------------------------------------------
void z_ed3View::GLPaint()
{
    
    glPushMatrix();
        Camera& cam = DOC()->_cam;
	    const M4&		mt = cam.GetViewMtx(0);
        cam.UpdateFrustrumPlanes();
	    glColor3f(1.0, 1.0f, 1.0);
	    glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
#ifdef _BDL_PREC
		glLoadMatrixd((REAL*)&mt);
#else
        glLoadMatrixf((REAL*)&mt);
#endif
        DOC()->Draw3DScene(this);

    glPopMatrix();
}


//---------------------------------------------------------------------------------------
void z_ed3View::OnMove(int x, int y) 
{
	CView::OnMove(x, y);
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if(_downKey == nChar)
        return;

    _blockKeyCall=1;
    OnKeyUp(_downKey, nRepCnt, nFlags);
    _blockKeyCall=0;

    _downKey = nChar;

    if(nChar == ('i') || nChar == ('I'))
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        _blockMouseCall=1;
        OnLButtonDown(0, pt) ;
        _blockMouseCall=0;
        _downKey =  nChar;
        return ;
    }
    else if(nChar == ('p') || nChar == ('P'))
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        _blockMouseCall=1;
        OnRButtonDown(0, pt) ;
        _blockMouseCall=0;
        _downKey =  nChar;
        return ;
    }
    else if(nChar == ('o') || nChar == ('O'))
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        _blockMouseCall=1;
        OnLButtonDown(0, pt) ;
        OnRButtonDown(0, pt) ;
        _blockMouseCall=0;
        _downKey =  nChar;
        return ;
    }

    if(nChar == (VK_SHIFT) || nChar == (VK_CONTROL))
        DOC()->Invalidate();

    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if(nChar == ('i') || nChar == ('I'))
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        _blockMouseCall=1;
        OnLButtonUp(0, pt) ;
        _blockMouseCall=0;
        _downKey =  0;
        return ;
    }
    else if(nChar == ('p') || nChar == ('P'))
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        _blockMouseCall=1;
        OnRButtonUp(0, pt) ;
        _blockMouseCall=0;
        _downKey =  0;
        return ;
    }
    else if(nChar == ('o') || nChar == ('O'))
    {
        CPoint pt;
        GetCursorPos(&pt);
        ScreenToClient(&pt);
        _blockMouseCall=1;
        OnLButtonUp(0, pt) ;
        OnRButtonUp(0, pt) ;
        _blockMouseCall=0;
        _downKey =  0;
        return ;
    }
    if(!_blockKeyCall)
        CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//---------------------------------------------------------------------------------------
void z_ed3View::RedrawDC(BOOL update)
{
	DrawInHrc(_hdc,update);
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnMouseMove(UINT nFlags, CPoint point) 
{
    V3      scrPt;
    CPoint  pt = _mm._pt - point;
    

    if(_mm._rdown || _mm._ldown)
    {
        _mm._pt    = point; 
        //SetCursorPos(_mm._pt.x, _mm._pt.y);

		if(AKEY('L'))
		{
			DrawInHrc(_hdc);
			CView::OnMouseMove(nFlags, point);
			REAL cf = _fov + pt.y;
			
			if(cf < 35) cf=35;
			if(cf>140)  cf=140;

			_fov = cf ;
			DOC()->_cam.SetFov(_fov);

			SBT(3,MKSTR("FOV :%03.0f", _fov));
			return;
		}

/*
		if(!AKEY(VK_LBUTTON))
			_mm._ldown=0;

		if(!AKEY(VK_RBUTTON))
			_mm._rdown=0;
*/
    }

    DOC()->Make3dCursorPos(this, point, _rt, scrPt);


    if(_mm._ldown)
    {
        
        if(abs(pt.x) > 0|| 
           abs(pt.y) > 0)
        {
            if(_mm._rdown)
            {
                if(AKEY(VK_SPACE))
				{
                    DOC()->_cam.MoveUp(pt.y*64.f); 
				}
                else
				{
                    DOC()->_cam.MoveUp(pt.y*2.f);
				}

				if(AKEY(VK_SPACE))
				{
					DOC()->_cam.MoveSide(-pt.x*64.f);    	
				}
				else
				{
					DOC()->_cam.MoveSide(-pt.x*2.f);
				}
            }
			else
			{
				if(AKEY(VK_SHIFT))
				{
					V3		cr ;
					int		nC = 0;
					Brush** ppb = DOC()->GetSelBrushes(nC);
					if(nC){
						cr = (*ppb)->_box.GetCenter();
					}
					REAL dst = -vdist(DOC()->_cam._pos , cr);
					DOC()->_cam._pos = cr;
					DOC()->_cam.Rotate(-pt.x/128.f,-pt.y/128.f,0);
					DOC()->_cam._pos = cr + (DOC()->_cam._fwd)*dst;


				}
				else
				{
					DOC()->_cam.Rotate(-pt.x/128.f,-pt.y/128.f,0);
				}
    			
			}
			_rotating = TRUE;
        }
		
        SBT(2,MKSTR("CAM: %04.2f %04.2f z:%04.2f",	DOC()->_cam._pos.x/GMeters,
															DOC()->_cam._pos.y/GMeters,
															DOC()->_cam._pos.z/GMeters));
		if(DOC()->_compviewmode != C_NA)
		{
			DOC()->Invalidate(); //allow to see the full BSP
		}
		else
			DrawInHrc(_hdc);

	}
    else if(_mm._rdown )
    {
        
        if(AKEY(VK_SPACE) )
            DOC()->_cam.MoveFwd(pt.y*64.f);
        else
            DOC()->_cam.MoveFwd(pt.y*4.f);
	
		SBT(2,MKSTR("CAM: %04.2f %04.2f z:%04.2f",DOC()->_cam._pos.x/GMeters,
														  DOC()->_cam._pos.y/GMeters,
														  DOC()->_cam._pos.z/GMeters));
        _rotating = TRUE;
		if(DOC()->_compviewmode != C_NA)
		{
			DOC()->Invalidate();
		}
		else
			DrawInHrc(_hdc);

    }
    
    if(_mm._rdown || _mm._ldown)
        Rollover(point);
    
	CView::OnMouseMove(nFlags, point);
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if(AKEY(VK_SPACE))
    {
        int r=0,c=0;
        if(!IsRectEmpty(&_prevrt)) 
        {
            FRM()->m_wndSplitter.SetColumnInfo(c, _prevrt.right, 0);
            FRM()->m_wndSplitter.SetRowInfo(r, _prevrt.bottom, 0);
            ::memset(&_prevrt,0,sizeof(RECT));
        }
        else
        {
            int dummy;
            RECT rt;
            GetParent()->GetClientRect(&rt);
            GetClientRect(&_prevrt);

            FRM()->m_wndSplitter.GetColumnInfo(c, (int&)_prevrt.right, dummy);
            FRM()->m_wndSplitter.GetRowInfo(r, (int&)_prevrt.bottom, dummy);

            FRM()->m_wndSplitter.SetColumnInfo(c, rt.right, 1);
            FRM()->m_wndSplitter.SetRowInfo(r, rt.bottom, 1);
        }
        FRM()->m_wndSplitter.RecalcLayout();
    }
	CView::OnLButtonDblClk(nFlags, point);
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	_mm._ldown = 0;

	if(_mm._rdown==0)
	{
		if(GetCapture() == this)
			ReleaseCapture();
		if(_scrPoint.x!=0)
			SetCursorPos(_scrPoint.x, _scrPoint.y);
		ShowTheCursor();
	}

    if(!_rotating )
    {
        _EL_SEL sel = DOC()->_selmode;
        if(SEL_NONE!=sel && DOC()->_blocksel==FALSE)
        {
            DOC()->DoSelection3D(point, this, sel);
        }
    }
	DOC()->Invalidate();
    if(!_blockMouseCall)
	    CView::OnLButtonUp(nFlags, point);
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
    MakeCurrent(_hdc, m_hRC); 
    Project3D(cx, cy);
    GetClientRect(&_rt);
    if(_rt.bottom==0)_rt.bottom=1;	
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnLButtonDown(UINT nFlags, CPoint point) 
{
    GetCursorPos(&_scrPoint);
    if(GetCapture()!= this)
	    SetCapture();
    HideTheCursor();

    _rotating  = FALSE;
    _mm._ldown = 1;
    _mm._pt    = point;
    _mm._ptd   = point;   
    
	CView::OnLButtonDown(nFlags, point);

    V3 dummy;
    DOC()->Make3dCursorPos(this, point, _rt, dummy);

}


//---------------------------------------------------------------------------------------
void z_ed3View::OnRButtonDown(UINT nFlags, CPoint point) 
{
    GetCursorPos(&_scrPoint);
    if(GetCapture()!= this)
	    SetCapture();
	GetCursorPos(&_scrPoint);
    HideTheCursor();
    
	_mm._rdown = 1;
    _rotating  = _mm._ldown;
    _mm._pt    = point;
    _mm._ptd   = point;

	
    CView::OnRButtonDown(nFlags, point);
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(_mm._ldown==0)
	{
		if(GetCapture() == this)
			ReleaseCapture();

		if(_scrPoint.x!=0)
			SetCursorPos(_scrPoint.x, _scrPoint.y);
		ShowTheCursor();
	}
    _mm._rdown=FALSE;
    DOC()->Invalidate();

    if(!_blockMouseCall)
	    CView::OnRButtonUp(nFlags, point);
}

//---------------------------------------------------------------------------------------
BOOL z_ed3View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	/*
    if(AKEY(VK_SPACE) )
        DOC()->_cam.MoveSide(zDelta*8);    	
    else
        DOC()->_cam.MoveSide(zDelta);
    */
    DrawInHrc(_hdc);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

//---------------------------------------------------------------------------------------
void z_ed3View::BuildFont(HDC hDC)
{ 
    MakeCurrent(_hdc, m_hRC); 
    _font1 = glGenLists(96); 
    _hf    = CreateFont( -14, 0, 0, 0, FW_MEDIUM, FALSE,
                                FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
                                CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                                FF_DONTCARE | DEFAULT_PITCH, "Verdana" );
    HFONT hf = (HFONT)SelectObject(_hdc, _hf); 
    wglUseFontBitmaps(_hdc, 32, 96, _font1); 
    SelectObject(_hdc, hf); 
}

//---------------------------------------------------------------------------------------
void z_ed3View::TextOut(V3& v, const char* text, CLR& clr)
{ 
    glEnable(GL_BLEND);
    glColor3ub(clr.r,clr.g,clr.b);
    
	glPushMatrix();
    {
        glRasterPos3f(v.x,v.y,v.z);
        glPushAttrib(GL_LIST_BIT); 
        glListBase( _font1-32 );
        glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );
    }
    glPopMatrix();
    glDisable(GL_BLEND);
}

void z_ed3View::HideTheCursor (void)
{
	while (ShowCursor (FALSE) >= 0);
}

void z_ed3View::ShowTheCursor (void)
{
	while (ShowCursor (TRUE) < 0);
}

void    z_ed3View::Rollover(CPoint pt)
{
    ClientToScreen(&pt);
    if(WindowFromPoint(pt) != GetCapture())
    {
		if(_scrPoint.x!=0)
			SetCursorPos(_scrPoint.x, _scrPoint.y);
        _mm._pt    = _mm._ptd;
    }
}



#if  0
/*
Framing ANimation Texs
*/
void FrameTexture();

/*
Fire Runtime Texs
*/
void GenFiretexTure();

/*
Blobs Runtime Texs
*/
void GenBlobTexture();



#endif //0

void z_ed3View::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
    theApp.p_wndFocused=this;
}

//---------------------------------------------------------------------------------------
void z_ed3View::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	if(GetCapture() == this)
		ReleaseCapture();
    _mm._ldown = 0;	
    _mm._rdown = 0;
    theApp.p_wndFocused=0;
}


LRESULT z_ed3View::OnPostInit(WPARAM pb1, LPARAM pb2)
{
    return 0;
}

void    z_ed3View::GlAfterPaint()
{


}