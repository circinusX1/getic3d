// z_ed2View.cpp : implementation of the CZ_ed2View class
// Author Marius Chincisan:
// Started Sept 2003
//---------------------------------------------------------------------------------------

#include "stdafx.h"
#include "texadapter.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "z_ed2View.h"
#include "z_ed3View.h"
#include "mainfrm.h"
#include "basecont.h"
#include "zedtypes.h"

//---------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
// structure for handing cursor at brush edges
struct SHCUR
{
    HCURSOR hc;
    DWORD   scDir;
};


//---------------------------------------------------------------------------------------
// colors for grid and axes

//---------------------------------------------------------------------------------------
// MFC crap
IMPLEMENT_DYNCREATE(CZ_ed2View, CView)
BEGIN_MESSAGE_MAP(CZ_ed2View, CView)
	//{{AFX_MSG_MAP(CZ_ed2View)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SHOWWINDOW()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
    ON_MESSAGE(WM_USR_REPAINT,OnUsrMsgRepaint)
    ON_MESSAGE(WM_POSTINIT, OnPostInit)	
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
// CZ_ed2View construction/destruction
CZ_ed2View::CZ_ed2View()
{
    Reinit();
}

void CZ_ed2View::Reinit()
{
    _zoom   = 0.4f;                 //initial zoom
    _shifty = 0.0f;
    _shiftx = 0.0f;
    _selBox = FALSE;
    _active = FALSE;
    _canMove= 1;
	_scrPoint.x=0;
	_scrPoint.y=0;
    _selCenter  = V0;
    _ptDown     = V0;
}

//---------------------------------------------------------------------------------------
CZ_ed2View::~CZ_ed2View()
{
}

//---------------------------------------------------------------------------------------
// make up our class
BOOL CZ_ed2View::PreCreateWindow(CREATESTRUCT& cs)
{
	/*
    cs.lpszClass = theApp._cNameViews.c_str();
*/

     cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
								        LoadCursor(0,IDC_CROSS),
							            (HBRUSH)0,0);

    return CView::PreCreateWindow(cs);
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::OnDraw(CDC* pDC)
{
}

//---------------------------------------------------------------------------------------
BOOL CZ_ed2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CZ_ed2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CZ_ed2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

//---------------------------------------------------------------------------------------
#ifdef _DEBUG
void CZ_ed2View::AssertValid() const
{
	CView::AssertValid();
}
void CZ_ed2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CZ_ed2Doc* CZ_ed2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZ_ed2Doc)));
	return (CZ_ed2Doc*)m_pDocument;
}
#endif //_DEBUG

//---------------------------------------------------------------------------------------
int CZ_ed2View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    _accmode = BR_VIEW;
    _hdc = ::GetDC(m_hWnd);   
    if(!DOC()->InitGL(2,_hdc, m_hRC))
    {
        return -1;
    }
    ++DOC()->_viewCount;
	return 0;
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::ReleaseRDI()
{
    MakeCurrent(_hdc, m_hRC); 
    Ta.Disable();
	TexRef::Clear(1);
 	//wglDeleteContext(m_hRC);
    ::ReleaseDC(m_hWnd,_hdc);
	_hdc  = 0;
	m_hRC = 0;
    MakeCurrent(0,0); 
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::OnDestroy() 
{
    CView::OnDestroy();
    --DOC()->_viewCount;
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::RedrawDC(BOOL update)
{
    if(m_hWnd)
	    DrawInHrc(_hdc,update);
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::OnPaint() 
{
    CPaintDC dc(this); 
	DrawView(dc.m_hDC);
}

void	CZ_ed2View::DrawView(HDC hdc, BOOL update)
{
    if(AKEY(VK_SPACE))
        DOC()->Invalidate();
    else
        DrawInHrc(hdc, update);
}

//---------------------------------------------------------------------------------------
void	CZ_ed2View::DrawInHrc(HDC hdc, BOOL update)
{
	if(!hdc || FRM()->m_bshutdown)
		return;
    MakeCurrent(hdc, m_hRC); 
    Ta.Disable();
    if(_rt.right && _rt.bottom)
    {
        Project2D(_rt.right, _rt.bottom);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
	        GLPaint(_rt);
		glEnable(GL_DEPTH_TEST);
        DOC()->PrintCoords(_rt, _vport, V0, _vt, _active,cs_ct,&_textmouse);

        if(DOC()->_glAddSwapHintRectWIN)
            (DOC()->_glAddSwapHintRectWIN)(0,0,_rt.right,_rt.bottom);
		SwapBuffers(hdc); 
    }
	MakeCurrent(hdc, 0); 
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	GetClientRect(&_rt);
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::Project2D(int iWidth, int iHeight)
{
	MakeCurrent(_hdc, m_hRC); 
    if(iHeight == 0)
    {
        iHeight = 1;
    }

    glViewport(0, 0, iWidth, iHeight);
    if(DOC()->b_whitebk !=0 )
    {
        if(DOC()->b_whitebk==1)
            _active ? glClearColor(1.f, 1.f, 1.f, 0.0f) : glClearColor(.9f, .9f, .9f, 0.0f) ;
        else
            glClearColor(1.f, 1.f, 1.f, 0.0f);

    }
    else 
    {
        if(DOC()->_bShowCsg)
            glClearColor(0.11f, 0.11f, 0.11f, 0.0f);
        else
	    {
		    _active ? glClearColor(.0f, .0f, .0f, 0.0f) : glClearColor(.0800f, .0800f, .0800f, 0.0f) ;
	    }
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    _vport.left   = -(((REAL)iWidth * 0.5)   / _zoom)   + _shiftx;
    _vport.right  =  (((REAL)iWidth * 0.5)   / _zoom)   + _shiftx;
    _vport.top    = -(((REAL)iHeight * 0.5)  / _zoom)   + _shifty;
    _vport.bottom  = (((REAL)iHeight * 0.5)  / _zoom)   + _shifty;
    glOrtho(_vport.left, _vport.right, _vport.top , _vport.bottom, -131072, 131072);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//	MakeCurrent(_hdc, 0); 
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::GLPaint(RECT& rt)
{
    glPushMatrix();
    {
        Ta.Disable();
		if(DOC()->b_showgrid)
			DrawGrid(rt);
        DOC()->Draw2DScene(this);
    }
    glPopMatrix();
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::DrawGrid(RECT& rt)
{
    switch(_vt)
    {
        case 'y':
                DrawTopGrid(rt);
                break;
        case 'x':
                DrawSideGrid(rt);
                break;
        case 'z':
                DrawFrontGrid(rt);
                break;
    }
}


void CZ_ed2View::OnLButtonDown(UINT nFlags, CPoint point) 
{
    BOOL    rep = 0;

	SetFocus();
    if(GetCapture()!= this)
    {
        SetCapture();
    }
	_cccGrd.reset();
    _mm._ldown = 1;
    _mm._pt    = point;
	_mm._ptd   = point;


    GetCursorPos(&_scrPoint);
    _scaleDirDown = _scaleDir;
	_canMoveDown  = _canMove;
    CheckCoordRestrains();

    // create a bounding selection box to allow copy move delete mirroring
    if(DOC()->OnStartMoveRotScale())
    {
        rep = GetSelectionItems(point);
        _arccircle[0] = V0;
        _arccircle[1] = V0;
        _arccircle[2] = V0;
    }

    if(0 == _canMoveDown || _accmode == BR_VIEW)
    {
        HideTheCursor(); 
    }
    
	CView::OnLButtonDown(nFlags, point);
    _dirty = FALSE;

    DOC()->Invalidate(rep);
}

void CZ_ed2View::OnLButtonUp(UINT nFlags, CPoint point) 
{
    _scaleDirDown = 0;
	_canMoveDown  = 0;
	_mm._ldown    = 0;
	_depl         = 0;

    if(GetCapture()== this)
        ReleaseCapture();

    SetCursorText(_wmouse,0);
    ShowTheCursor(); 
    if(_accmode == BR_VIEW)
    {
		if(_scrPoint.x!=0)
			SetCursorPos(_scrPoint.x, _scrPoint.y);
    }
        
    if(DOC()->_selBox._brushflags & BRSH_SELVIS)
    {
     	CView::OnLButtonUp(nFlags, point);
        MannageCursor(point,0);
        DOC()->SelectByBox();
        DOC()->Invalidate();
        return;
    }

    if(_dirty)
    {
        DOC()->OnFinishMoveRotScale(_dirty, &_dirtyBrushes);
    }

	if(DOC()->_selmode != SEL_NONE && DOC()->_blocksel==FALSE)
	{
		CPoint pdiff = point - _mm._ptd;
		if(abs(pdiff.x) < 2 && abs(pdiff.y) < 2)
		{
            if(AKEY(VK_SHIFT) )
            {
			    DoSelection(point);
            }
		}
	}
    
 	CView::OnLButtonUp(nFlags, point);
    MannageCursor(point,0);
    _dirty = FALSE;
    _dirtyBrushes.clear();
    DOC()->Invalidate();
}


void    CZ_ed2View::ScrollView(CPoint& pt)
{
    _shiftx += (REAL)pt.x / _zoom;
    _shifty -= (REAL)pt.y / _zoom;

    if(DOC()->_xyz_lock)
    {
        DOC()->SetShift(this);
    }
    else
    {
        switch(_vt)
        {
            case 'x':
                SBT(3, MKSTR("2D: x=%04.2f z=%04.2f * %2.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
                break;
            case 'y':
                SBT(3, MKSTR("2D: z=%04.2f y=%04.2f * %2.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
               break;
            case 'z':
                SBT(3 , MKSTR("2D: x=%04.2f y=%04.2f * %2.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
                break;
        }
    }
    DOC()->Invalidate();
}


void    CZ_ed2View::RaiseBrush(CPoint& pt)
{
	if(0 == _canMoveDown)
	{
		ScrollView(pt);
	}
	else
    {
        V3     t;
        switch(_vt)
        {
            case 'x':   // side view
                switch(_scaleDir)
                {
                    case SIZE_N:
                        break;
                    case SIZE_S:
                        break;
                    case SIZE_E:
                        break;
                    case SIZE_W:
                        break;
                }
                break;
            case 'y':   //top view
                switch(_scaleDir)
                {
                    case SIZE_N:
                        break;
                    case SIZE_S:
                        break;
                    case SIZE_E:
                        break;
                    case SIZE_W:
                        break;
                }
               break;
            case 'z': // front view
                switch(_scaleDir)
                {
                    case SIZE_N:
                        break;
                    case SIZE_S:
                        break;
                    case SIZE_E:
                        break;
                    case SIZE_W:
                        break;
                }
                break;
        }

        
        int     count;
        Brush** b = DOC()->GetSelBrushes(count);
        if(count)
        {
            //DOC()->OnStartMoveRotScale(ITM_ROTATED, b, count);
            for(int i=0;i<count;i++,b++)
            {
			    (*b)->Raise(t, _zoom);
            }
		    DrawView(_hdc);
            _dirty = M_SHIFTED;
        }
    }
}


void    CZ_ed2View::MoveBrush(CPoint& pt)
{
	if(0 == _canMoveDown)
	{
		ScrollView(pt);
        return;
	}
    V3  t = _GetTranslation(pt);
    //DOC()->RoundToNear(t);

    if(DOC()->IsSelBoxOn())
    {
        DOC()->_selBox.Move(t);
        DOC()->_selBox.Recalc();
        DrawView(_hdc);
    }
    else
    {
        int         count;
        Brush** b = DOC()->GetSelBrushes(count);

		if(_depl==1) t.x=0;
		if(_depl==2) t.y=0;
		if(_depl==3) t.z=0;

        if(1== count && (*b)->HasMotion() && !(*b)->IsDisabled())
        {
            count = _MoveBrushMotion((*b)->_pMotion, t);
        }
        V3  c;
        //DOC()->OnStartMoveRotScale(M_MOVED, b, count);
        for(int i=0; i < count; i++, b++)
        {
            if((*b)->IsDisabled())
                continue;

            (*b)->Move(t);
            (*b)->Recalc();
            c =(*b)->_box.GetCenter();
            SetCursorText(_wmouse, MKSTR("x:%4.2f y:%4.2f z:%4.2f", c.x/GMeters, c.y/GMeters, c.z/GMeters));
        }
        _dirty = M_MOVED;
    }
	DrawView(_hdc);
}

void    CZ_ed2View::MoveVertex(CPoint& pt)
{
    int i;
	if(0 == _canMoveDown)
	{
		ScrollView(pt);
        return;
	}
    V3  t = _GetTranslation(pt);
    //DOC()->RoundToNear(t);
    
    int    count  = 0;
    Vtx2** ppvx   = DOC()->GetSelVxes(count);
    Brush* pBrush = (*ppvx)->_pPoly->_pBrush;
    if(!pBrush)
        return;
    
    for( i = 0; i < count; i++, ppvx++)
    {
        if((*ppvx)->_pPoly->_pBrush != pBrush)
        {
            Beep(600,20);
            SBT(0,"Vertexes should be on the same brush");
            return; 
        }
    }
    if(!(pBrush->_brushflags & BRSH_TRIANGLES))        
    {
        Beep(600,20);
        SBT(0,"Cannot move while brush is not triangulated");
        return;
    }

    //DOC()->OnStartMoveRotScale(ITM_ROTATED, &pBrush, 1);

    while(--i>=0)
    {
        --ppvx;
        (*ppvx)->_xyz += t;       
    }
	DrawView(_hdc);
    Add4Update(pBrush);
}

void    CZ_ed2View::MovePoly(CPoint& pt)
{
    if(0 == _canMoveDown)
	{
		ScrollView(pt);
        return;
	}
    V3  t = _GetTranslation(pt);
    //DOC()->RoundToNear(t);

    vvector<V3*>       toMove(32);
    int     count    = 0;
    Poly**  pppoly   = DOC()->GetSelPolys(count);
    if(count !=1 )
    {
        SBT(0,"Cannot move more than one polygon");
        Beep(600,20);
        return;
    }
    Poly* pPoly  = *pppoly;

    Brush* pBrush = pPoly->_pBrush;
    if(pBrush==0)
    {
        SBT(0,"Error selection parent brush");
        Beep(600,20);
        return;
    }
    //DOC()->OnStartMoveRotScale(M_MOVED, &pBrush, count);

    FOREACH(vvector<Vtx2>, pPoly->_vtci, pvtcis)
	{
        toMove << &pvtcis->_xyz;

        // search this vx in the brush and add it to the toMove;
        FOREACH(vvector<Poly>, pBrush->_polys, ppoly)
        {
            if(&(*ppoly) == pPoly)
                continue;
            FOREACH(vvector<Vtx2>, ppoly->_vtci, potherv)
            {
                if(::IsZero(potherv->_xyz-pvtcis->_xyz,EPSMED))
                {
                    toMove << &potherv->_xyz;
                }
            }
        }
	}

	if(_depl==1) t.x=0;
	if(_depl==2) t.y=0;
	if(_depl==3) t.z=0;

    if(AKEY('N')||AKEY('n'))
    {
        FOREACH(vvector<V3*>, toMove, ppv3)
        {
            (*ppv3)->x += (pPoly->_n.x * (t.x+t.y+t.z));
            (*ppv3)->y += (pPoly->_n.y * (t.x+t.y+t.z));
            (*ppv3)->z += (pPoly->_n.z * (t.x+t.y+t.z));
        }
    }
    else
    {
        FOREACH(vvector<V3*>, toMove, ppv3)
        {
            (*ppv3)->x+=t.x;
            (*ppv3)->y+=t.y;
            (*ppv3)->z+=t.z;
        }
    }
    this->Add4Update(pBrush);
    DrawView(_hdc);
}

void    CZ_ed2View::MoveItem(CPoint& pt)
{
	if(0 == _canMoveDown)
	{
		ScrollView(pt);
        return;
	}
    V3  t = _GetTranslation(pt);
    //DOC()->RoundToNear(t);
    
    if(DOC()->IsSelBoxOn())
    {
        DOC()->_selBox.Move(t);
        DOC()->_selBox.Recalc();
    }
    else
    {
		int         count;
		SceItem**   it = DOC()->GetSelItems(count);

		if(_depl==1) t.x=0;
		if(_depl==2) t.y=0;
		if(_depl==3) t.z=0;

        if(count)
        {
            //DOC()->OnStartMoveRotScale(M_MOVED, it, count);
            for(int i=0;i<count;i++,it++)
            {
				(*it)->Move(t);
            }
        }
    }
    DrawView(_hdc);
}

void    CZ_ed2View::ScaleItem(CPoint& pt)
{
    V3 s,t;
	if(0 == _scaleDirDown)
	{
		ScrollView(pt);
		return;
	}
    if(_scaleDirDown == SIZE_MOVE)
    {
        MoveItem(pt);
        return;
    }
    _GetScaleValues(pt, s,t);
    //DOC()->RoundToNear(s);
    //DOC()->RoundToNear(t);

    if(DOC()->IsSelBoxOn())
    {
        DOC()->_selBox.Scale(s,t, _zoom);
        DOC()->_selBox.Recalc();
        V3 ex = DOC()->_selBox._box.GetExtends();
        SBT(3,MKSTR("w:%4.2f h:%4.2f d:%4.2f", ex.x/GMeters, ex.y/GMeters, ex.z/GMeters));
    }
    else
    {
        if(s.x==0)t.x=0;
        if(s.y==0)t.y=0;
        if(s.z==0)t.z=0;

		int count;
		SceItem** it = DOC()->GetSelItems(count);
        if(count)
        {
            //DOC()->OnStartMoveRotScale(M_MOVED, it, count);

            REAL radius = 0;
            for(int i=0;i<count;i++,it++)
            {
				(*it)->Scale(s,t,_zoom);
            }
            DrawView(_hdc);
			_dirty = ITM_SCALED;
        }
    }
}

void    CZ_ed2View::ScaleBrush(CPoint& pt, REAL mxw, REAL myz)
{
    V3 s,t;
	if(0 == _scaleDirDown)
	{
		ScrollView(pt);
		return;
	}
    if(_scaleDirDown == SIZE_MOVE)
    {
        MoveBrush(pt);
        return;
    }
    _GetScaleValues(pt, s,t);
    //DOC()->RoundToNear(s);
    //DOC()->RoundToNear(t);

    if(DOC()->IsSelBoxOn())
    {
        DOC()->_selBox.Scale(s,t, _zoom);
        DOC()->_selBox.Recalc();
        DrawView(_hdc);

        V3 ex = DOC()->_selBox._box.GetExtends();
        SBT(3,MKSTR("w:%4.2f h:%4.2f d:%4.2f", ex.x/GMeters, ex.y/GMeters, ex.z/GMeters));
        switch(_vt)
        {
            case 'x':
                SetCursorText(_wmouse, MKSTR("h:%4.2f d:%4.2f", ex.y/GMeters, ex.z/GMeters));
                break;
            case 'y':
                SetCursorText(_wmouse, MKSTR("w:%4.2f d:%4.2f", ex.x/GMeters, ex.z/GMeters));
                break;
            case 'z':
                SetCursorText(_wmouse, MKSTR("w:%4.2f h:%4.2f", ex.x/GMeters, ex.y/GMeters));
                break;
        }
        DOC()->Invalidate();
    }
    else
    {
        Box box;
        int count;
        Brush** b = DOC()->GetSelBrushes(count);
        if(count)
        {
            //DOC()->OnStartMoveRotScale(M_SCALED, b, count);
            for(int i=0;i<count;i++,b++)
            {
                if((*b)->IsDisabled())
                    continue;
                if(s.x==0)t.x=0;
                if(s.y==0)t.y=0;
                if(s.z==0)t.z=0;
        
			    (*b)->Scale(s,t,_zoom);
                (*b)->Recalc();
                box.Union((*b)->_box); // for sbar text
            }
            DrawView(_hdc);
            V3 ex = box.GetExtends();
            SBT(3,MKSTR("w:%4.2f h:%4.2f d:%4.2f", ex.x/GMeters, ex.y/GMeters, ex.z/GMeters));
            switch(_vt)
            {
                case 'x':
                    SetCursorText(_wmouse, MKSTR("h:%4.2f d:%4.2f", ex.y/GMeters, ex.z/GMeters));
                    break;
                case 'y':
                    SetCursorText(_wmouse, MKSTR("w:%4.2f d:%4.2f", ex.x/GMeters, ex.z/GMeters));
                    break;
                case 'z':
                    SetCursorText(_wmouse, MKSTR("w:%4.2f h:%4.2f", ex.x/GMeters, ex.y/GMeters));
                    break;
            }
            _dirty = M_SCALED;
    	    if(GUpdateImm)
	    		DOC()->Invalidate3D(1) ;
        }
    }
}

void    CZ_ed2View::ScalePoly(CPoint& pt)
{
    V3 s,t;
	if(0 == _scaleDirDown)
	{
		ScrollView(pt);
		return;
	}
    if(_scaleDirDown == SIZE_MOVE)
    {
        MoveBrush(pt);
        return;
    }
    _GetScaleValues(pt, s,t);
    //DOC()->RoundToNear(s);
    //DOC()->RoundToNear(t);

    vvector<V3*>       toMove(32);
    int     count    = 0;
    Poly**  pppoly   = DOC()->GetSelPolys(count);
    if(count !=1 )
    {
        SBT(0,"Cannot resize more than one polygon");
        Beep(600,20);
        return;
    }

    Poly* pPoly  = *pppoly;
    if(!(pPoly->_polyflags & POLY_EXTRUDED))
    {
        Beep(600,20);
        SBT(0,"Cannot scale non-extruded polygon");
        return;
    }

    Brush* pBrush = pPoly->_pBrush;
    V3     center; 

    //DOC()->OnStartMoveRotScale(M_SCALED, &pBrush, 1);

    FOREACH(vvector<Vtx2>, pPoly->_vtci, pvtcis)
	{
        toMove << &pvtcis->_xyz;
        center += pvtcis->_xyz;
        // search this vx in the brush and add it to the toMove;
        FOREACH(vvector<Poly>, pBrush->_polys, ppoly)
        {
            if(&(*ppoly)== pPoly)
                continue;
            FOREACH(vvector<Vtx2>, ppoly->_vtci, potherv)
            {
                if(potherv->_xyz == pvtcis->_xyz)
                    toMove << &potherv->_xyz;
            }
        }
	}
    center /= (REAL)pPoly->_vtci.size();
    Box b;
    Box b2;
    FOREACH(vvector<V3*>, toMove, ppv3){
        b.AddPoint(**ppv3);
        b2.AddPoint(**ppv3);
    }

    b2._max.x += s.x;
    b2._max.y += s.y;
    b2._max.z += s.z;

    V3 den = b._max-b._min; 
    if(den.x==0)den.x=1;
    if(den.y==0)den.y=1;
    if(den.z==0)den.z=1; // fix .#IND
    V3 rap = (b2._max-b2._min)/den;
    FOREACH(vvector<V3*>, toMove, ppv3){
        V3& vx  = **ppv3;
        vx      = b._min + (vx-b._min) * rap;
        vx      += t;
    }
    this->Add4Update(pBrush);
	DrawView(_hdc);
}

void    CZ_ed2View::RotateItem(CPoint& pt)
{
	if(0 == _canMoveDown)
	{
		ScrollView(pt);
        return;
	}
	int count;
	SceItem** it = DOC()->GetSelItems(count);
    if(count)
    {
        GetSelCenter(it, count);
        V3      r = GetMouseRotAngle(pt);
        _cccGrd += r;
        SBT(0, MKSTR("%2.2f° %2.2f° %2.2f°",R2G(_cccGrd.x), R2G(_cccGrd.y), R2G(_cccGrd.z)));
        //DOC()->OnStartMoveRotScale(ITM_ROTATED, it, count);

        BOOL aroundOrigin = AKEY(VK_MENU);
        for(int i=0;i<count;i++,it++)
        {
			(*it)->Rotate(r, aroundOrigin);
        }
        DrawView(_hdc);
        _dirty = ITM_ROTATED;
    }
}

void    CZ_ed2View::RotateBrush(CPoint& pt)
{
	if(0 == _canMoveDown)
	{
		ScrollView(pt);
        return;
	}
    int count;
    Brush** b = DOC()->GetSelBrushes(count);
    BOOL bao = AKEY(VK_MENU);
    //MERGE>


    if(count)
    {
        V3   r  = GetMouseRotAngle(pt);
        _cccGrd += r;
        ROLL2PI(_cccGrd.x);
        ROLL2PI(_cccGrd.y);
        ROLL2PI(_cccGrd.z);
        SBT(0, MKSTR("%2.2f° %2.2f° %2.2f°",R2G(_cccGrd.x), R2G(_cccGrd.y), R2G(_cccGrd.z)));
        //DOC()->OnStartMoveRotScale(ITM_ROTATED, b, count);
        for(int i=0;i<count;i++,b++)
        {
            if((*b)->IsDisabled())
                continue;
			(*b)->Rotate(r, bao);
            (*b)->Recalc();
        }
		DrawView(_hdc);
        _dirty = M_ROTATED;
    }
}

void    CZ_ed2View::RotatePoly(CPoint& pt)
{
	if(0 == _canMoveDown)
	{
		ScrollView(pt);
        return;
	}
    V3   r        = GetMouseRotAngle(pt);
    int  count    = 0;
    if(AKEY('T') || AKEY('t'))
    {
        Poly**  pppoly   = DOC()->GetSelPolys(count);
        for(int i=0;i<count;i++,pppoly++)
        {
            (*pppoly)->RotateTCRad(R2G((r.x+r.y+r.z)), 0);
        }
    }
    else
    {
        vvector<V3*>       toMove(32);
        
        Poly**  pppoly   = DOC()->GetSelPolys(count);
        if(count !=1)
        {
            Beep(600,20);
            SBT(0,"Cannot rotate more than one polygon");
            return;
        }

        Poly* pPoly   = *pppoly;
        Brush* pBrush = pPoly->_pBrush;
        Box   b;
/*
        if(!(pBrush->_brushflags & BRSH_TRIANGLES))
        {
            Beep(600,20);
            SBT(0,"Cannot rotate while brush is not triangulated");
            return;
        }
*/

        //DOC()->OnStartMoveRotScale(ITM_ROTATED, &pBrush, 1);

    	FOREACH(vvector<Vtx2>, pPoly->_vtci, pvtcis)
		{
            toMove << &pvtcis->_xyz;
            b.AddPoint(pvtcis->_xyz);
            // search this vx in the brush and add it to the toMove;
            FOREACH(vvector<Poly>, pBrush->_polys, ppoly)
            {
                if(&(*ppoly) == pPoly)
                    continue;
                FOREACH(vvector<Vtx2>, ppoly->_vtci, potherv)
                {
                    if(potherv->_xyz == pvtcis->_xyz)
                        toMove << &potherv->_xyz;
                }
            }
		}
        
        _cccGrd += r;
        ROLL2PI(_cccGrd.x);
        ROLL2PI(_cccGrd.y);
        ROLL2PI(_cccGrd.z);
        SBT(0, MKSTR("%2.2f° %2.2f° %2.2f°",R2G(_cccGrd.x), R2G(_cccGrd.y), R2G(_cccGrd.z)));
        
        M4 mr;
        if(DOC()->_selEdges.size()) //rot around the edge
        {
            PolyEdge& pe = DOC()->_selEdges[0];
            REAL rotangle = tmax(r.x,r.y);
                 rotangle = tmax(rotangle,r.z);
            V3   edge = (pe._edge[1]->_xyz - pe._edge[0]->_xyz);
            // rotate around edge
            mr = MRotAround(edge, rotangle);
        }
        else
        {
            mr = MRadRotate(r.x,r.y,r.z);
            FOREACH(vvector<V3*>, toMove, ppv3)
            {
                V3& rv = *(*ppv3);
                rv-=b.GetCenter();
                mr.v3rotate(**ppv3);
                rv+=b.GetCenter();
            }
        }
        DrawView(_hdc);
        this->Add4Update(pBrush);
    }
}

void CZ_ed2View::OnMouseMove(UINT nFlags, CPoint point) 
{
    CView::OnMouseMove(nFlags, point);

    REAL fx = Wnd2World(point.x, 'h');
    REAL fy = Wnd2World(point.y, 'v');
    
    MouseWorldMove(fx, fy, point);
    _wmouse.x = point.x+16;
    _wmouse.y = _rt.bottom-point.y+16;

}

void CZ_ed2View::MouseWorldMove(REAL mxw, REAL  myw, CPoint& point)
{
    if(_accmode == BR_MOVE)
    {
        switch(_vt)
        {
            case 'x':
                SBT(3,MKSTR("M: z:%04.2f y:%04.2f z:%1.4f", mxw/GMeters, myw/GMeters, _zoom));
                break;
            case 'y':
                SBT(3,MKSTR("M: x:%04.2f z:%04.2f z:%1.4f", mxw/GMeters, myw/GMeters, _zoom));
               break;
            case 'z':
                SBT(3,MKSTR("M: x:%04.2f y:%04.2f z:%1.4f", mxw/GMeters, myw/GMeters, _zoom));
                break;
        }
    }

    DWORD   selMode = DOC()->_selmode;
    _accmode = DOC()->_brmode;
    if(_mm._ldown)
    {
        CPoint pt = _mm._pt - point;
        
        
       _mm._pt   = point;
        
        if(AKEY(VK_SHIFT))
        {
            ScrollView(pt);
        }
        else
        {
            CheckCoordRestrains();
            switch(_accmode)//BR_VIEW
            {
                case BR_VIEW:
                    ScrollView(pt);
                    break;
                case BR_MOVE:
					if(DOC()->_selBox._brushflags & BRSH_SELVIS)
					{
						MoveBrush(pt);
					}
					else
					{
						if(selMode == SEL_BRUSH)
                        {
							MoveBrush(pt);
                            if(DOC()->_allselected)
                                MoveItem(pt);
                        }
						else if(SEL_ITEMS == selMode )
                        {
							MoveItem(pt);
                        }
						else if(SEL_VERTEX == selMode)
                        {
							MoveVertex(pt);
                        }
						else if(SEL_FACE == selMode)
						{
							MovePoly(pt);
						}
					}
                    break;
                case BR_SCALE:
                    if(selMode == SEL_BRUSH || 
                        (DOC()->_selBox._brushflags & BRSH_SELVIS))
                        ScaleBrush(pt, mxw, myw);
                    else if(SEL_ITEMS == selMode )
                        ScaleItem(pt);
					else if(SEL_FACE == selMode)
						ScalePoly(pt);
                    break;
                case BR_ROTATE:
                    if(selMode == SEL_BRUSH)
                        RotateBrush(pt);
                    else if(SEL_ITEMS == selMode )
                        RotateItem(pt);
					else if(SEL_FACE == selMode)
						RotatePoly(pt);
                    break;
		        case BR_RISE:
    		        RaiseBrush(pt);
                    break;
            }
        }
		if(GUpdateImm)
			DOC()->Invalidate3D(1) ;
    }
    else if (_mm._rdown)
    {
        CPoint pt = _mm._pt - point;
        _mm._pt   = point;

        if(_zoom<.01)
            _zoom += (REAL)pt.y/8256.0;
        else if(_zoom<.1)
            _zoom += (REAL)pt.y/1256.0;
        else
            _zoom += (REAL)pt.y/256.0;

	    CLAMPVAL(_zoom,.002,8.f);

        if(DOC()->_xyz_lock)
        {
            DOC()->SetZoom(this);
            DOC()->Invalidate() ;
        }
        else
        {
            Invalidate();
        }
    }
    else
    {
        _scaleDir = 0;
        ClientToScreen(&point);
        if(GetCapture() == this )
        {
            ReleaseCapture();
        }
    }
    if(_mm._ldown || _mm._rdown)
    {
        switch(_vt)
        {
            case 'x':
                SBT(1, MKSTR("2D: x=%04.2f z=%04.2f * %1.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
                
                break;
            case 'y':
                SBT(1, MKSTR("2D: z=%04.2f y=%04.2f * %1.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
               break;
            case 'z':
                SBT(1, MKSTR("2D: x=%04.2f y=%04.2f * %1.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
                break;
        }        
        
        if(0 == _canMoveDown || _accmode == BR_VIEW)
        {
            Rollover(point);
        }
    }
	CRect lr = _rt;
	lr.InflateRect(-4,-4);
}



BOOL CZ_ed2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	REAL dz = .01f;

	if(AKEY(VK_SHIFT) )
	{
		dz=.5f;
	}

    if(zDelta > 0)
	    _zoom+=dz;
    else
        _zoom-=dz;

	CLAMPVAL(_zoom,.002,8.f);

    if(DOC()->_xyz_lock)
    {
        DOC()->SetZoom(this);
    }
    else
    {
        switch(_vt)
        {
            case 'x':
                SBT(1, MKSTR("(x=%04.2f z=%04.2f) * %1.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
                break;
            case 'y':
                SBT(1, MKSTR("(z=%04.2f y=%04.2f) * %1.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
               break;
            case 'z':
                SBT(1, MKSTR("(x=%04.2f y=%04.2f) * %1.4f", _shiftx/GMeters, _shifty/GMeters, _zoom));
                break;
        }
        
    }
    DOC()->Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


int CZ_ed2View::World2Wnd(REAL coord, char dir) 
{
    int wpos;

	if (dir == 'h')
    {
		wpos = (int)(_zoom * (coord - _shiftx));
    }
	else
		if (_vt != 'y')
			wpos = (int)(_zoom * (coord - _shifty));
        else
			wpos = (int)(_zoom * (coord + _shifty));
			
	if (dir == 'h')
    	wpos += (int)(0.5 * _rt.right);
	else
	{
		if (_vt != 'y')
    		wpos *= -1;
		wpos += (int)(0.5 * _rt.bottom);
	}
	return wpos;
}

REAL    CZ_ed2View::Wnd2World(REAL coord, char dir)
{
	int      itemp = 0;
	REAL     fWp = 0.0;

    if(dir=='h')
    {
        itemp = (int)(coord + (-0.5 * _rt.right));
		fWp = itemp / _zoom + _shiftx;
    }
    else
    {
        if(_vt != 'y')
        {
			itemp = (int)(-1 * (coord + (-0.5 * _rt.bottom)));
			fWp = itemp / _zoom + _shifty;
        }
        else
        {
			itemp = (int)(coord + (-0.5 * _rt.bottom));
			fWp = itemp / _zoom - _shifty;

        }
    }
    return fWp;
}


BOOL CZ_ed2View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    switch(_accmode)
    {
        case BR_VIEW:
            SetCursor(theApp._cursors[CUR_SCROLL]);
            return 1;
        case BR_MOVE:
            {
                _canMove = 0;
                CPoint pt;
                GetCursorPos(&pt);
                ScreenToClient(&pt);
                if(MannageCursor(pt,0)){
					SetCursor(theApp._cursors[CUR_MOVE]);
					return 1;
				}
            }
            SetCursor(theApp._cursors[CUR_SCROLL]);
            return 1;
        case BR_SCALE:
            {
                _scaleDir = 0;
                CPoint pt;
                GetCursorPos(&pt);
                ScreenToClient(&pt);
                if(MannageCursor(pt,1)){
                    return 1;
				}
            }
            SetCursor(theApp._cursors[CUR_SCROLL]);
            return 1; 
        case BR_ROTATE:
            {
                //if(_vt=='z')return 1;
                _canMove = 0;
                CPoint pt;
                GetCursorPos(&pt);
                ScreenToClient(&pt);
                if(MannageCursor(pt,0))
				{
					SetCursor(theApp._cursors[CUR_ROTATE]);
					return 1;
				}
            }
			SetCursor(theApp._cursors[CUR_SCROLL]);
            return 1;
        case BR_RISE:
            {
                //_canMove = 1;
                CPoint pt;
                GetCursorPos(&pt);
                ScreenToClient(&pt);
                if(MannageCursor(pt,2))
				{
					return 1;
				}
            }
			SetCursor(theApp._cursors[CUR_SCROLL]);
            return 1;

    }
	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CZ_ed2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    BOOL    moveKey=0;
    CPoint  point(0,0);


   

    switch(nChar)
    {
		case VK_DOWN:
            moveKey=1;
            point.y=-1;
			break;
		case VK_RIGHT:
            point.x=-1;
            moveKey=1;
			break;
		case VK_LEFT:
            point.x=1;
            moveKey=1;
			break;
		case VK_UP:
            point.y=1;
            moveKey=1;
            break;
        case '1':
            //DOC()->OnBumove();
			_accmode=BR_MOVE;
            break;
        case '2':
			_accmode=BR_SCALE;
            //DOC()->OnBUScale();
            break;
        case '3':
            if(_vt=='z')break;
			_accmode=BR_ROTATE;
            //DOC()->OnBurotate();
            break;
        case '0':
			_accmode=BR_VIEW;
            //DOC()->SetViewMode();
            break;
    }


    if(moveKey)
    {
        POINT   pt;

        GetCursorPos(&pt);
        ScreenToClient(&pt);

        REAL mxw = Wnd2World(pt.x, 'h');
        REAL myw = Wnd2World(pt.y, 'v');

        _scaleDirDown=1;
        _canMoveDown=1;
        switch(_accmode)
        {
            case BR_VIEW:
                ScrollView(point);
                break;
            case BR_MOVE:
                if(DOC()->_selmode == SEL_BRUSH)
				{
                    MoveBrush(point);
                    MoveItem(point);
				}
                else if(DOC()->_selmode == SEL_ITEMS)
                {
                    MoveItem(point);
                }
                break;
            case BR_SCALE:
                if(DOC()->_selmode == SEL_BRUSH)
                    ScaleBrush(point, mxw, myw);
                else if(DOC()->_selmode == SEL_ITEMS)
                    ScaleItem(point);
                break;
            case BR_ROTATE:
                //if(_vt=='z')break;
                if(DOC()->_selmode == SEL_BRUSH)
                    RotateBrush(point);
                else if(DOC()->_selmode == SEL_ITEMS)
                    RotateItem(point);
                break;
		    case BR_RISE:
    		    RaiseBrush(point);
                break;
        }
        _scaleDirDown=0;
        _canMoveDown=0;
		if(GUpdateImm)
			DOC()->Invalidate3D(1) ;
    }

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//_vport.left
BOOL CZ_ed2View::IsBoxInViewPort(Box& box)
{
	return TRUE;
	switch(_vt)
	{
		case 'x':
			if(box._max.z < _vport.left)
				return FALSE;
			if(box._min.z > _vport.right)
				return FALSE;
			if(box._min.y > _vport.bottom)
				return FALSE;
			if(box._max.y < _vport.top)
				return FALSE;
			break;
		case 'y':
    		if(box._max.x < _vport.left)
				return FALSE;
			if(box._min.x > _vport.right)
				return FALSE;
			if(box._min.z > _vport.bottom)
				return FALSE;
			if(box._max.z < _vport.top)
				return FALSE;
			break;
		case 'z':
			if(box._max.x < _vport.left)
				return FALSE;
			if(box._min.x > _vport.right)
				return FALSE;
			if(box._min.y > -_vport.top)
				return FALSE;
			if(box._max.y < -_vport.bottom)
				return FALSE;
			break;
	}
	return TRUE;
}

BOOL CZ_ed2View::IsVxInViewPort(V3& vx)
{
	return TRUE;
	switch(_vt)
	{
		case 'x':
			if(vx.z < _vport.left)
				return FALSE;
			if(vx.y < _vport.top)
				return FALSE;
			break;
		case 'y':
    		if(vx.x < _vport.left)
				return FALSE;
			if(vx.z < _vport.top)
				return FALSE;
			break;
		case 'z':
			if(vx.x < _vport.left)
				return FALSE;
			if(vx.y < -_vport.bottom)
				return FALSE;
			break;
	}
	return TRUE;
}

void CZ_ed2View::DoSelection(CPoint&  point)
{
    REAL mxw = Wnd2World(point.x, 'h');
    REAL myw = Wnd2World(point.y, 'v');

	_EL_SEL sel = DOC()->_selmode;
        
    if(SEL_NONE!=sel)
    {
        DOC()->DoSelection2D(mxw,myw,this, sel);
    }
    DOC()->Invalidate();	
}


BOOL    CZ_ed2View::MannageCursor(CPoint point, BOOL set)
{
    
    int  count = 0;

 

    if(_accmode == BR_VIEW ) 
        return 0;

    Brush** ppb = DOC()->GetSelBrushes(count);

    if(0 == count && 
       DOC()->_pSelItems.size()== 0 && 
       DOC()->_pSelVertexes.size()==0 && 
       DOC()->_pSelPolys.size()==0)     // AUG 04
    {
        if(!DOC()->IsSelBoxOn())
            return 0;
    }

	Box acumBox;
    int  Mx,My,mx,my;


    ResolveCurInBox(acumBox,Mx,My,mx,my);
	
    int hs = abs((Mx-mx)/3 + 1);
    int vs = abs((My-my)/3 + 1);
    SHCUR sScaleCurs[3][3] =     
    {
        {{theApp._cursors[CUR_NWSE], SIZE_N|SIZE_W},  {theApp._cursors[CUR_NS],SIZE_N}     , {theApp._cursors[CUR_NESW], SIZE_N|SIZE_E}},
        {{theApp._cursors[CUR_EW],   SIZE_W},         {theApp._cursors[CUR_SCALE], SIZE_MOVE/*SIZE_0*/} , {theApp._cursors[CUR_EW],   SIZE_E}},
        {{theApp._cursors[CUR_NESW], SIZE_S|SIZE_W},  {theApp._cursors[CUR_NS],SIZE_S}     , {theApp._cursors[CUR_NWSE], SIZE_S|SIZE_E}},
    };

    SHCUR sRiseCurs[3][3] =     
    {
        {{theApp._cursors[CUR_MOVE], SIZE_MOVE|SIZE_W},  {theApp._cursors[CUR_EW],SIZE_N}     , {theApp._cursors[CUR_MOVE], SIZE_MOVE}},
        {{theApp._cursors[CUR_NS],   SIZE_W},         {theApp._cursors[CUR_SCALE], SIZE_MOVE} , {theApp._cursors[CUR_NS],   SIZE_E}},
        {{theApp._cursors[CUR_MOVE], SIZE_MOVE},  {theApp._cursors[CUR_EW],SIZE_S}     , {theApp._cursors[CUR_MOVE], SIZE_MOVE}},
    };

    

    if(_vt == 'x')
    {
        DWORD t;
        t = sScaleCurs[0][0].scDir;
        sScaleCurs[0][0].scDir=sScaleCurs[2][0].scDir;

        sScaleCurs[2][0].scDir=t;
        t = sScaleCurs[0][1].scDir;

        sScaleCurs[0][1].scDir=sScaleCurs[2][1].scDir;
        sScaleCurs[2][1].scDir=t;
        t = sScaleCurs[0][2].scDir;

        sScaleCurs[0][2].scDir=sScaleCurs[2][2].scDir;
        sScaleCurs[2][2].scDir=t;
    }

	if(!set)
	{
		CRect   rne(mx, my, Mx, My);
		if(rne.PtInRect(point))
		{
			_canMove=TRUE;
			return TRUE;
		}
	}
	else
	{
		int i=0,j=0;
		for(int xPos = mx; xPos < Mx; xPos+=hs)
		{
			j=0;
			for(int yPos = my; yPos < My; yPos+=vs)
			{
				CRect   rne;
				switch(_vt)
				{
				case 'x':
						rne.left   = xPos;
						rne.top    = yPos;
						rne.right  = xPos+hs;
						rne.bottom = yPos+vs;

						break;
				case 'y':
						rne.left   = xPos;
						rne.top    = yPos;
						rne.right  = xPos+hs;
						rne.bottom = yPos+vs;
						break;
				case 'z':
						rne.left   = xPos;
						rne.top    = yPos;
						rne.right  = xPos+hs;
						rne.bottom = yPos+vs;
						break;
				}
	
/*
				CDC* pdc=GetDC();
				rne.InflateRect(-1,-1);
				pdc->Rectangle(rne);
				ReleaseDC(pdc);
*/
				if(rne.PtInRect(point))
				{
					_scaleDir = sScaleCurs[j][i].scDir;

                    if(2 == set) //rise flag is 2
                    {
                        SetCursor(sRiseCurs[j][i].hc);
                    }
                    else
                    {
        				SetCursor(sScaleCurs[j][i].hc);
                    }
					return TRUE;
				}
				j++;
			}
			i++;
		}
	}
    return FALSE;

}



void CZ_ed2View::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	if(GetCapture()!= this)
		SetCapture();
	
    _mm._rdown = 1;
    _mm._pt = point;
	_mm._ptd   = point; //FIX IN GETIC
	GetCursorPos(&_scrPoint); //FIX IN GETIC
    HideTheCursor(); 

    /****
    CMenu       cm;
    cm.LoadMenu(IDR_MENU1);// ADD,DELETE

    if(DOC()->HasSomeSelected())
    {
        CMenu       cm,*cs;
        cm.LoadMenu(IDR_MENU1);// ADD,DELETE
        cs = cm.GetSubMenu(0);
        POINT pt = {point.x,point.y};
        ClientToScreen(&pt);
        cs->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_LEFTBUTTON ,pt.x,pt.y,this);
        cm.DestroyMenu();
    }
    */
	CView::OnRButtonDown(nFlags, point);
}

void CZ_ed2View::OnRButtonUp(UINT nFlags, CPoint point) 
{
	_mm._rdown = 0;


	if(GetCapture() == this)
		ReleaseCapture();


	if(_scrPoint.x!=0)
		SetCursorPos(_scrPoint.x, _scrPoint.y);
	ShowTheCursor();     
	CView::OnRButtonUp(nFlags, point);
}


void CZ_ed2View::ResolveCurInBox(Box& acumBox,int& Mx,int& My,int& mx,int& my)
{
	int                 count   = 0;
	int                 count1  = 0;
    int                 count2  = 0;
    int                 count3  = 0;    // AUG 04
    Brush**             ppb     = DOC()->GetSelBrushes(count);
	SceItem**			ppi		= DOC()->GetSelItems(count1);
    Vtx2**              ppvx    = DOC()->GetSelVxes(count2);
    Poly**              ppply   = DOC()->GetSelPolys(count3);

        
    if(DOC()->IsSelBoxOn())
    {
        acumBox = DOC()->_selBox._box;
    }
    else  if(count)
    {

        if(count == 1 && ((*ppb))->HasMotion())    
        {
            CMotion* pM = (*ppb)->_pMotion;
            if(pM->GetSelNode() > 0) 
            {
                MState*    pS = pM->GetSelState();

                acumBox.AddPoint(pS->_pos);
                acumBox.Expand(32);
                
                //pS->_pos 
                
                count = 0;  // no brush bbox for cursors
            }
        }

	    for(int i=0;i<count;i++,ppb++)
	    {
            if((*ppb)->IsDisabled())
                continue;

		    acumBox.Union((*ppb)->_box);
	    }
    }
    else if(count1)
    {
        for(int i=0;i<count1;i++,ppi++)
        {
            Box box =  (*ppi)->GetWBox(); 
            acumBox.Union(box);
        }
    }else if(count2)
    {
        for(int i=0;i<count2;i++,ppvx++)
        {
            // 16 box aroud the vx
            V3& vx   =  (*ppvx)->_xyz;
            Box box  =  Box(vx+V3(32,32,32),vx+V3(-32,-32,-32)); 
            acumBox.Union(box);
        }
    }else if(count3)
    {
        for(int i=0;i<count3;i++,ppply++)
        {
            // face box
            acumBox.Union((*ppply)->_box);
        }
    }

    acumBox.Expand0(32, 30);

    switch(_vt)
    {
        case 'x':	//-z Y
			Mx = World2Wnd(acumBox._max.z,'h');
            mx = World2Wnd(acumBox._min.z,'h');

            my = World2Wnd(acumBox._max.y,'v');
            My = World2Wnd(acumBox._min.y,'v');
            break;

        case 'y':	//X
    		Mx = World2Wnd(acumBox._max.x,'h');
            mx = World2Wnd(acumBox._min.x,'h');

            My = World2Wnd(acumBox._max.z,'v');
            my = World2Wnd(acumBox._min.z,'v');
           break;

        case 'z':
			Mx = World2Wnd(acumBox._max.x,'h');
            mx = World2Wnd(acumBox._min.x,'h');

            my = World2Wnd(acumBox._max.y,'v');
            My = World2Wnd(acumBox._min.y,'v');
            break;
    }
    
    int dx =  (Mx-mx)/6;
    int dy =  (My-my)/6;

    Mx += dx;    
    mx -= dx;
    My += dy;
    my -= dy;

}

LRESULT CZ_ed2View::OnUsrMsgRepaint(WPARAM pb1, LPARAM pb2)
{
    return 0;
}



void CZ_ed2View::OnSetFocus(CWnd* pOldWnd) 
{
    DOC()->_lastFocusView = this;
	CView::OnSetFocus(pOldWnd);
    _active = TRUE;	
    DOC()->SetActiveWiew(this);
    DrawView(_hdc);
    theApp.p_wndFocused=this;
}

void CZ_ed2View::OnKillFocus(CWnd* pNewWnd) 
{
    SetCursorText(_wmouse, 0);
	DrawView(_hdc);
    CView::OnKillFocus(pNewWnd);
    DOC()->SetActiveWiew(pNewWnd);
    _active = FALSE;	
    _dirtyBrushes.clear();
    theApp.p_wndFocused=0;
}


void CZ_ed2View::CheckCoordRestrains()
{
    _depl = 0;
    // allow the moving scaling onl;y on the coord that has been pressed
    switch(_vt)
    {
        case 'x':       // can block Z or Y (SIDE)
            if(AKEY('X')) _depl = 2;
	        if(AKEY('Y')) _depl = 3;
            break;
        case 'y':       // can block Z or X (TOP)
            if(AKEY('X'))_depl=3;
	        if(AKEY('Y'))_depl=1;
            break;
        case 'z':       // can block X or Y (FRONT)
            if(AKEY('X'))_depl=2;
	        if(AKEY('Y'))_depl=1;
            break;
    }
}

void CZ_ed2View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

    #pragma message("dont forget. doubleclick rotates 90 degrees and a key pressed by pi/key")

    V3      t,r,s;
    REAL    piDiv = 2.0;

    for(int i=0;i<9;i++)
    {
        if(AKEY(i+'0'))
        { 
            piDiv  = (REAL)i;
            DOC()->SbarShowCurMode();
            break;
        }
    }

    if(AKEY(VK_SHIFT))
        piDiv=-piDiv;

    switch(_accmode)
    {
        case BR_MOVE:
            break;
        case BR_ROTATE:
            if(piDiv==0)
            {
                r.x = -1;
                r.y = -1;
                r.z = -1;
            }
            else
            {
                switch(_vt)
                {
                    case 'x':
                        r.x = PI/piDiv;
                        break;
                    case 'y':
                        r.y = PI/piDiv;
                        break;
                    case 'z':
                        r.z = PI/piDiv;
                        break;
                }
            }
            {
                BOOL baor = AKEY(VK_MENU);
                int  count;
                Brush** b = DOC()->GetSelBrushes(count);

                //MERGE>
                /*
                if(1== count && (*b)->HasMotion())
                {
                    if(!(*b)->IsDisabled())
                    {
                        CMotion* pM = (*b)->_pMotion;
                        int nNode;
                        if((nNode = pM->GetSelNode()) >= 0) // mode sel node
                        {
                            //extend all nodes in this direction
                            MState*    pS = pM->GetSelState();
                            V3         nP = pS->_dir + r;
                            pS->_dir    = nP;
                            if(nNode != 0)
                                count         = 0;    // don't rot the brush
                        }
                    }
                }
                */

                //MERGE<


                if(count)
                {
                    for(int i=0;i<count;i++,b++)
                    {
			            (*b)->Rotate(r,baor);
                        (*b)->Recalc();
                    }
		            DrawView(_hdc);
                    _dirty = M_ROTATED;
                }

				SceItem** it = DOC()->GetSelItems(count);
                if(count)
                {
                    for(int i=0;i<count;i++,it++)
                    {
						(*it)->Rotate(r,baor);
                    }
                    DrawView(_hdc);
                    _dirty = ITM_ROTATED;
                }
            }
            break;
        case BR_RISE:
            switch(_vt)
            {
                case 'x':
                    break;
                case 'y':
                    break;
                case 'z':
                    break;
            }
            break;
        default:
            /*
            {
                int r,c;
                switch(_vt)
                {
                    case 'x':
                        r=1;
                        c=0;
                        break;
                    case 'y':
                        r=0;
                        c=1;
                        break;
                    case 'z':
                        r=1;
                        c=1;
                        break;
                }
                if(!IsRectEmpty(&_prevrt)) 
                {
                    FRM()->m_wndSplitter.SetRowInfo(r, _prevrt.bottom, 1);
                    FRM()->m_wndSplitter.SetColumnInfo(c, _prevrt.right, 1);
                    
                    ::memset(&_prevrt,0,sizeof(RECT));
                }
                else
                {
                    int dummy;
                    RECT rt;
                    GetParent()->GetClientRect(&rt);

                    GetClientRect(&_prevrt);
                    _prevrt.right+=2;
                    _prevrt.bottom+=2;

                    FRM()->m_wndSplitter.GetRowInfo(r, (int&)_prevrt.bottom, dummy);
                    FRM()->m_wndSplitter.GetColumnInfo(c, (int&)_prevrt.right, dummy);
                    
                    FRM()->m_wndSplitter.SetRowInfo(r, rt.bottom, 1);
                    FRM()->m_wndSplitter.SetColumnInfo(c, rt.right, 1);
                    
                }
        		FRM()->m_wndSplitter.RecalcLayout();

            }
            */
        break;
    }

	CView::OnLButtonDblClk(nFlags, point);
}

void CZ_ed2View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    DOC()->SbarShowCurMode();
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CZ_ed2View::HideTheCursor (void)
{
	while (ShowCursor (FALSE) >= 0);
}

void CZ_ed2View::ShowTheCursor (void)
{
	while (ShowCursor (TRUE) < 0);
}

void CZ_ed2View::SnapPoint(CPoint& point)
{

}

void    CZ_ed2View::Rollover(CPoint pt)
{
    ClientToScreen(&pt);
    if(WindowFromPoint(pt) != GetCapture())
    {
		if(_scrPoint.x!=0)
			SetCursorPos(_scrPoint.x, _scrPoint.y);
        _mm._pt    = _mm._ptd;
    }
}

void    CZ_ed2View::SetCursorText(POINT& pt, LPCTSTR str)
{
    _textmouse = pt;
    if(str && *str)
        cs_ct = str;
    else
        cs_ct ="";
}


V3   CZ_ed2View::GetMouseRotAngle(CPoint& pt)
{
 /*
    V3 r;
    switch(_vt)
    {
        case 'x':
              r.x = pt.x/128.f;
              r.x = pt.y/128.f;
              break;

        case 'y':
              r.y = pt.x/128.f;
              r.y = pt.y/128.f;
              break;
        case 'z':
              r.z = pt.x/128.f;
              r.z = pt.y/128.f;
              break;
    }
    
	if(AKEY(VK_SPACE))
	{
		if(r.x)
			r.x/=32.f;
		if(r.y)
			r.y/=32.f;
		if(r.z)
			r.z/=32.f;
	}
    return r;

*/
    V3   mult;
    V3   vCenter = _selCenter;
    V3   v3down;
    V3   v3cur;
    V3   vPdt;
    

    REAL dsx = Wnd2World(_mm._ptd.x, 'h');
    REAL dsy = Wnd2World(_mm._ptd.y, 'v');

    REAL sx = Wnd2World(_mm._pt.x, 'h');
    REAL sy = Wnd2World(_mm._pt.y, 'v');
    REAL fx = Wnd2World(_mm._pt.x + pt.x, 'h');
    REAL fy = Wnd2World(_mm._pt.y + pt.y, 'v');

    switch(_vt)
    {
        case 'x':

            vPdt.x = vCenter.x;
            vPdt.z = dsx;
            vPdt.y = dsy;

            v3down.x = vCenter.x;
            v3down.z = sx;
            v3down.y = sy;

            v3cur.x = vCenter.x;
            v3cur.z = fx;
            v3cur.y = fy;
            mult = VX;
            _grades = (_cccGrd.x);
            break;

        case 'y':
            vPdt.y = vCenter.y;
            vPdt.x = dsx;
            vPdt.z = dsy;

            
            v3down.y = vCenter.y;
            v3down.x = sx;
            v3down.z = sy;

            v3cur.y = vCenter.y;
            v3cur.x = fx;
            v3cur.z = fy;
            mult = VY;
            _grades = (_cccGrd.y);
           break;

        case 'z':
            vPdt.z = vCenter.z;
            vPdt.x = dsx;
            vPdt.y = dsy;

            v3down.z = vCenter.z;
            v3down.x = sx;
            v3down.y = sy;

            v3cur.z = vCenter.z;
            v3cur.x = fx;
            v3cur.y = fy;
            mult = VZ;
            _grades = (_cccGrd.z);
            break;
    }
    

    V3 di = (v3down - vCenter).norm();
    V3 dc = (v3cur  - vCenter).norm();

    _arccircle[0] = vCenter;
    _arccircle[1] = vCenter + (vPdt-vCenter).norm() * 128.0/_zoom;
    _arccircle[2] = vCenter + dc * 128.0/_zoom;

    
    
    REAL cosang = Vdp(di, dc);
    mult *= acos(cosang);
 
    // see the sign
    V3 perp = Vcp(di, dc).norm();
    REAL r = perp.x ? perp.x : (perp.y?perp.y:perp.z);
    if(r)
    {
        int br = r > 0 ? -1 : 1;
    
        switch(_vt)
        {
            case 'x':
                mult*=br;
                break;
            case 'y':
                mult*=br;
                break;
            case 'z':
                mult*=br;
                break;
        }    
        return mult;
    }
    return V0;
}

void CZ_ed2View::GlAfterPaint()
{
    
    if(_accmode==BR_ROTATE && _mm._ldown )
    {
        
        glBegin(GL_LINES);
            glColor4ub(255,255,255,255);
            
            glVertex3fv((REAL*)_arccircle[0]);
            glVertex3fv((REAL*)_arccircle[1]);

            glColor4ub(255,255,0,255);
            
            glVertex3fv((REAL*)_arccircle[0]);
            glVertex3fv((REAL*)_arccircle[2]);
        glEnd();
        
        SetCursorText(_wmouse, 
                        MKSTR("%4.2f °", R2G(_grades)));
    }
    
}

BOOL CZ_ed2View::GetSelectionItems(POINT& point)
{
    SceItem**   pItems;
    Poly**      pPolys;
    Brush**     pBrushes;
    Vtx2**      pvxes;
    int         ncount;
    
    if(_canMoveDown)
    {

        REAL fx = Wnd2World(point.x, 'h');
        REAL fy = Wnd2World(point.y, 'v');

        switch(_vt)
        {
            case 'x':
                _ptDown.z = fx;
                _ptDown.y = fy;
                break;
            case 'y':
                _ptDown.x = fx;
                _ptDown.z = fy;
               break;
            case 'z':
                _ptDown.x = fx;
                _ptDown.y = fy;
                break;
        }

        pItems = DOC()->GetSelItems(ncount);
        if(ncount)
        {
            GetSelCenter(pItems, ncount);
            MoveItem(CPoint(0,0));
            return 1;
        }

        pPolys = DOC()->GetSelPolys(ncount);
        if(ncount)
        {
            GetSelCenter(pPolys, ncount);
            MovePoly(CPoint(0,0));
            return 1;
        }

        pBrushes = DOC()->GetSelBrushes(ncount);
        if(ncount)
        {
            GetSelCenter(pBrushes, ncount);
            MoveBrush(CPoint(0,0));
            return 1;
        }

        pvxes = DOC()->GetSelVxes(ncount);
        if(ncount)
        {
            GetSelCenter(pBrushes, ncount);
            MoveVertex(CPoint(0,0));
            return 1;
        }
     }
    return 0;
}

void CZ_ed2View::GetSelCenter(Poly** pPoly, int count)
{
    _selCenter.reset();
    REAL fnc = count;
    while(--count>=0)
        _selCenter+=(pPoly[count]->_box.GetCenter());
    _selCenter/=fnc;
}

void CZ_ed2View::GetSelCenter(SceItem** it, int count)
{
    _selCenter.reset();
    REAL fnc = count;
    while(--count>=0)
        _selCenter+=it[count]->_t;
    _selCenter/=fnc;

}

void CZ_ed2View::GetSelCenter(Brush** it, int count)
{
    _selCenter.reset();
    REAL fnc = count;
    while(--count>=0)
        _selCenter+=(it[count]->_box.GetCenter());
    _selCenter/=fnc;
}

LRESULT CZ_ed2View::OnPostInit(WPARAM pb1, LPARAM pb2)
{

    m_wndReBar.Create(this);
    DWORD defStyle = TBSTYLE_FLAT|TBSTYLE_TRANSPARENT;
    DWORD styl  = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM;

	//UTimer = SetTimer(SPASH_TIMER, 60*1000*10, 0);

    
	if (!m_TolTipBar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_SLINETTIP) ||
		!m_TolTipBar.LoadToolBar(TB_SLINETTIP))
	{
		return -1;      // fail to create
	}
    DWORD tbAttr = CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY;
    m_TolTipBar.SetBarStyle(m_TolTipBar.GetBarStyle() |tbAttr);
    m_wndReBar.AddBar(&m_TolTipBar);
    return 0;
}

V3 CZ_ed2View::_GetTranslation(const CPoint& pt)
{
    V3 t;
    switch(_vt)
    {
        case 'x':
            t.z = -(REAL)pt.x / _zoom;
            t.y = (REAL)pt.y / _zoom;
            break;
        case 'y':
            t.x = -(REAL)pt.x / _zoom;
            t.z = -(REAL)pt.y / _zoom;
           break;
        case 'z':
            t.x = -(REAL)pt.x / _zoom;
            t.y = (REAL)pt.y / _zoom;
            break;
    }
    return t;
}

int    CZ_ed2View::_MoveBrushMotion( CMotion* pM, V3& t)
{
    int nNode;
    if((nNode = pM->GetSelNode()) > 0) // mode sel node
    {
        //extend all nodes in this direction
        MState*    pS = pM->GetSelState();
        V3         nP = pS->_pos + t;

        if(AKEY(VK_CONTROL))
        {
            // eqidist all points from start to here
            MState*    pSHead = pM->GetState(0);
            V3      calc;
    
            for(int i=1 ; i < nNode; i++)
            {
                double  stp = (double)i/(double)nNode;
                calc = pSHead->_pos + ((nP-pSHead->_pos)*stp);
                pM->GetState(i)->_pos = calc;
            }
        }
        pS->_pos = nP;
        return 0;
    }
    return 1;
}

void     CZ_ed2View::_GetScaleValues( CPoint& pt, V3& s, V3& t)
{
    //prelilinari elimination by cursor shape
    if(_scaleDirDown == SIZE_N || _scaleDirDown == SIZE_S)
        pt.x=0;       
    if(_scaleDirDown == SIZE_E || _scaleDirDown == SIZE_W)
        pt.y=0;

    if(AKEY('J') || AKEY('j'))
    {
        if(pt.y)
            pt.x=pt.y;
        else
            pt.y=pt.x;
    }

    switch(_vt)
    {
        case 'y':	//X
		    s.x = -(REAL)pt.x / _zoom;
		    s.z = -(REAL)pt.y / _zoom;

		    switch(_scaleDirDown)
		    {
			    case SIZE_NE:	
			    case SIZE_N:
				    s.z=-s.z;
				    t.z=-s.z;
				    break;
			    case SIZE_SW:
			    case SIZE_W:
				    s.x=-s.x;
				    t.x=-s.x;
				    break;
			    case SIZE_NW:
				    s.z=-s.z;
				    t.z=-s.z;
				    s.x=-s.x;
				    t.x=-s.x;
				    break;
		    }
            break;

    case 'x':	//-z Y
            s.z = -(REAL)pt.x / _zoom;
            s.y = (REAL)pt.y  / _zoom;

		    switch(_scaleDirDown)
		    {
			    case SIZE_NE:	
			    case SIZE_N:
				    s.y=-s.y;
				    t.y=-s.y;
				    break;
			    case SIZE_SW:
			    case SIZE_W:
				    s.z=-s.z;
				    t.z=-s.z;
				    break;
			    case SIZE_NW:
				    s.y=-s.y;
				    t.y=-s.y;
				    s.z=-s.z;
				    t.z=-s.z;
				    break;
		    }
            break;

        case 'z':
            s.x = -(REAL)pt.x / _zoom;
            s.y = (REAL)pt.y  / _zoom;

		    switch(_scaleDirDown)
		    {
			    case SIZE_SW:
				    s.x=-s.x;
				    t.x=-s.x;
				    s.y=-s.y;
				    t.y=-s.y;
				    break;
			    case SIZE_NW:
			    case SIZE_W:
				    s.x=-s.x;
				    t.x=-s.x;
				    break;
			    case SIZE_SE:
			    case SIZE_S:
				    s.y=-s.y;
				    t.y=-s.y;
				    break;
		    }
            break;
    }
}

void     CZ_ed2View::_DrawLines(int hgsz, 	
                                GLfloat Left, 
	                            GLfloat Right,   
	                            GLfloat Top,     
	                            GLfloat Bottom , 
	                            int     wp)
{
    if(_zoom>.04)
    {
        _active ? glColor4ubv(CLR_FINEGRIDF) : glColor4ubv(CLR_FINEGRIDD);
	    glBegin(GL_LINES);
   	    for(int vDec = 0; vDec <= DOC()->_gridSize; vDec += DOC()->_gridStep)
   	    {
		    if (vDec % (DOC()->_gridStep * 10) && vDec != hgsz)
		    {
			    wp = hgsz - vDec;
			    if (wp > (int)Left && wp < (int)Right)
			    {
        		    glVertex3f(wp, 0.0, Top);
        		    glVertex3f(wp, 0.0, Bottom);
			    }
			    if (wp < (int)Bottom && wp > (int)Top)
			    {
        		    glVertex3f(Left, 0.0, wp);
        		    glVertex3f(Right, 0.0, wp);
			    }
		    }
        }
    	glEnd();
    }

    _active ?   glColor4ubv(CLR_GRIDF):glColor4ubv(CLR_GRIDD);
    glBegin(GL_LINES);
    for(int vDec = 0; vDec <= DOC()->_gridSize; vDec += (DOC()->_gridStep * 10))
    {
		if (vDec != hgsz)
		{
			wp = hgsz - vDec;
			if (wp >= (int)Left && wp <= (int)Right)
			{
        		glVertex3f(wp, 0.0, Top);
        		glVertex3f(wp, 0.0, Bottom);
			}
			if (wp <= (int)Bottom && wp >= (int)Top)
			{
        		glVertex3f(Left, 0.0, wp);
        		glVertex3f(Right, 0.0, wp);
			}
		}
    }
    glEnd();

    glBegin(GL_LINES);
        
        glColor4ubv(CLR_AXES_X);
        glVertex3f((int)Left, 0, 0);
        glVertex3f((int)Right, 0, 0);

        glColor4ubv(CLR_AXES_Z);
        glVertex3f(0.0, 0.0, Top);
        glVertex3f(0.0, 0.0, Bottom);


    glEnd();
}

void    CZ_ed2View::Add4Update(Brush* pb)
{
    _dirty = M_MOVED; 
    pb->Dirty(1); 
    _dirtyBrushes << pb;
}


//---------------------------------------------------------------------------------------
void CZ_ed2View::DrawFrontGrid(RECT& rt)
{
	int hssz = DOC()->_gridSize / 2;

	REAL Left    = Wnd2World(rt.left,  'h');
	REAL Right   = Wnd2World(rt.right, 'h');
	REAL Top     = Wnd2World(rt.top,   'v');
	REAL Bottom  = Wnd2World(rt.bottom,'v');
	int  wp;

	if (Left < -hssz)		Left   = -hssz;
	if (Right > hssz)		Right  = hssz;
	if (Top > hssz)		    Top    = hssz;
	if (Bottom < -hssz)		Bottom = -hssz;

    glPushMatrix();
    if(_zoom>.04)       // shows the fine geid if zoom is less then .4
	{
        _active ? glColor4ubv(CLR_FINEGRIDF) : glColor4ubv(CLR_FINEGRIDD);
    	glBegin(GL_LINES);
	    for(int decVal = 0; decVal <= DOC()->_gridSize; decVal += DOC()->_gridStep)
    	{
			if (decVal % (DOC()->_gridStep * 10) && decVal != hssz)
			{
				wp = hssz - decVal;
				if (wp > (int)Left && wp < (int)Right)
				{
        			glVertex3f(wp, Top, 0.0);
        			glVertex3f(wp, Bottom, 0.0);
				}

				if (wp > (int)Bottom && wp < (int)Top)
				{
        			glVertex3f(Left, wp, 0.0);
        			glVertex3f(Right, wp, 0.0);
				}
			}
    	}
	    glEnd();
	}

    // shows different grid color on current active view

    _active ?   glColor4ubv(CLR_GRIDF):glColor4ubv(CLR_GRIDD);
    
	glBegin(GL_LINES);
    for(int decVal = 0; decVal <= DOC()->_gridSize; decVal += (DOC()->_gridStep * 10))
    {
		wp = hssz - decVal;
		if (decVal != hssz)
		{
			if (wp >= (int)Left && wp <= (int)Right)
			{
    	    	glVertex3f(wp, Top, 0.0);
        		glVertex3f(wp, Bottom, 0.0);
			}

			if (wp >= (int)Bottom && wp <= (int)Top)
			{
        		glVertex3f(Left, wp, 0.0);
        		glVertex3f(Right, wp, 0.0);
			}
		}
    }
    glEnd();

    glBegin(GL_LINES);
		// Horizontal lines
        glColor4ubv(CLR_AXES_X);
        glVertex3f(Left, 0.0, 0.0);
        glVertex3f(Right, 0.0, 0.0);
		// Vertical lines
        glColor4ubv(CLR_AXES_Y);
        glVertex3f(0.0, Top, 0.0);
        glVertex3f(0.0, Bottom, 0.0);

    glEnd();
    glPopMatrix();
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::DrawTopGrid(RECT& rt)
{
	int hgsz = DOC()->_gridSize / 2;

	GLfloat Left    = Wnd2World(rt.left, 'h');
	GLfloat Right   = Wnd2World(rt.right, 'h');
	GLfloat Top     = Wnd2World(rt.top, 'v');
	GLfloat Bottom  = Wnd2World(rt.bottom, 'v');
	int     wp;

	if (Left < -hgsz)		Left = -hgsz;
	if (Right > hgsz)		Right = hgsz;
	if (Top < -hgsz)		Top = -hgsz;
	if (Bottom > hgsz)		Bottom = hgsz;

    glPushMatrix();
    glRotatef(90, 1, 0, 0);

    if(_zoom>.04)
    {
        _active ? glColor4ubv(CLR_FINEGRIDF) : glColor4ubv(CLR_FINEGRIDD);
	    glBegin(GL_LINES);
   	    for(int vDec = 0; vDec <= DOC()->_gridSize; vDec += DOC()->_gridStep)
   	    {
		    if (vDec % (DOC()->_gridStep * 10) && vDec != hgsz)
		    {
			    wp = hgsz - vDec;
			    if (wp > (int)Left && wp < (int)Right)
			    {
        		    glVertex3f(wp, 0.0, Top);
        		    glVertex3f(wp, 0.0, Bottom);
			    }
			    if (wp < (int)Bottom && wp > (int)Top)
			    {
        		    glVertex3f(Left, 0.0, wp);
        		    glVertex3f(Right, 0.0, wp);
			    }
		    }
        }
    	glEnd();
    }

    _active ?   glColor4ubv(CLR_GRIDF):glColor4ubv(CLR_GRIDD);
    glBegin(GL_LINES);
    for(int vDec = 0; vDec <= DOC()->_gridSize; vDec += (DOC()->_gridStep * 10))
    {
		if (vDec != hgsz)
		{
			wp = hgsz - vDec;
			if (wp >= (int)Left && wp <= (int)Right)
			{
        		glVertex3f(wp, 0.0, Top);
        		glVertex3f(wp, 0.0, Bottom);
			}
			if (wp <= (int)Bottom && wp >= (int)Top)
			{
        		glVertex3f(Left, 0.0, wp);
        		glVertex3f(Right, 0.0, wp);
			}
		}
    }
    glEnd();

    glBegin(GL_LINES);
        
        glColor4ubv(CLR_AXES_X);
        glVertex3f((int)Left, 0, 0);
        glVertex3f((int)Right, 0, 0);

        glColor4ubv(CLR_AXES_Z);
        glVertex3f(0.0, 0.0, Top);
        glVertex3f(0.0, 0.0, Bottom);


    glEnd();

    glPopMatrix();
}

//---------------------------------------------------------------------------------------
void CZ_ed2View::DrawSideGrid(RECT& rt)
{
   	int hdsz = DOC()->_gridSize / 2;

	GLfloat Left = Wnd2World(rt.left, 'h');
	GLfloat Right = Wnd2World(rt.right, 'h');
	GLfloat Top = Wnd2World(rt.top, 'v');
	GLfloat Bottom = Wnd2World(rt.bottom, 'v');
	int wp;

	if (Left < -hdsz)
		Left = -hdsz;
	if (Right > hdsz)
		Right = hdsz;
	if (Top > hdsz)
		Top = hdsz;
	if (Bottom < -hdsz)
		Bottom = -hdsz;


    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    if(_zoom>.04)
    {
        _active ? glColor4ubv(CLR_FINEGRIDF) : glColor4ubv(CLR_FINEGRIDD);

        glBegin(GL_LINES);
        for(int fd= 0; fd<= DOC()->_gridSize; fd+= DOC()->_gridStep)
        {
	        if (fd% (DOC()->_gridStep * 10) && fd!= hdsz)
	        {
		        wp = hdsz - fd;
		        if (wp > (int)Left && wp < (int)Right)
		        {
    		        glVertex3f(0.0, Top, wp);
    	   	        glVertex3f(0.0, Bottom, wp);
		        }
		        if (wp > (int)Bottom && wp < (int)Top)
		        {
       		        glVertex3f(0.0, wp, Left);
       		        glVertex3f(0.0, wp, Right);
		        }
	        }
        }
        glEnd();
    }

    _active ?   glColor4ubv(CLR_GRIDF):glColor4ubv(CLR_GRIDD);

    glBegin(GL_LINES);
    for( int fd= 0; fd<= DOC()->_gridSize; fd+= (DOC()->_gridStep * 10))
    {
		if (fd!= hdsz)
		{
			wp = hdsz - fd;
			if (wp >= (int)Left && wp <= (int)Right)
			{
				// Vertical lines
    		    glVertex3f(0.0, Top, wp);
        		glVertex3f(0.0, Bottom, wp);
			}
			if (wp >= (int)Bottom && wp <= (int)Top)
			{
				// Horizontal lines
        		glVertex3f(0.0, wp, Left);
        		glVertex3f(0.0, wp, Right);
			}
		}
    }
    glEnd();


    glBegin(GL_LINES);
		// Horizontal lines
        glColor4ubv(CLR_AXES_Z);
        glVertex3f(0.0, 0.0, Left);
        glVertex3f(0.0, 0.0, Right);

    // Vertical lines
        glColor4ubv(CLR_AXES_Y);
        glVertex3f(0.0, Top, 0.0);
        glVertex3f(0.0, Bottom, 0.0);

    glEnd();

    glPopMatrix();

}
