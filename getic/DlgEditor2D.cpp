// @@@@@@@@@@@@@
// DlgEditor2D.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "geticgui.h"
#include "MMove.h"
#include "MainFrm.h"
#include "DlgEditor2D.h"
#include "z_ed2Doc.h"
#include "Staticgl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static DlgEditor2D* PThis;
DlgEditor2D::DlgEditor2D(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgEditor2D::IDD, pParent),_pWorking(0)
{
	//{{AFX_DATA_INIT(DlgEditor2D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    PThis = this;
    PThis->_pStaticGL=0;

    _sstates[0]._step       = STEP_MKSHAPE;
    _sstates[0]._viewMode   = V_3DFRONT;
    _sstates[0]._vproj      = '3';

    _sstates[1]._step       = STEP_MKPATH;
    _sstates[1]._viewMode   = V_2D;
    _sstates[1]._vproj      = 'y';


    _sstates[2]._step       = STEP_MKBRUSH;
    _sstates[2]._viewMode   = V_3DFRONT;
    _sstates[2]._vproj      = '3';

    _pagechange = 0;
    _plastsel   = 0;
    _blockupdate = 0;
}


void DlgEditor2D::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgEditor2D)
	DDX_Control(pDX, ST_OUT, m_stout);
	DDX_Control(pDX, PB_CLEAR, m_clr);
	DDX_Control(pDX, ST_CUR, m_curs);
	DDX_Control(pDX, PB_PREV, m_p);
	DDX_Control(pDX, PB_NEXT, m_n);
	DDX_Control(pDX, IDOK, m_k);
	DDX_Control(pDX, IDCANCEL, m_c);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgEditor2D, CBASEDLG)
	//{{AFX_MSG_MAP(DlgEditor2D)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(PB_PREV, OnPrev)
	ON_BN_CLICKED(PB_NEXT, OnNext)
	ON_BN_CLICKED(PB_CLEAR, OnClear)
	ON_BN_CLICKED(RB_SPLINE, OnSpline)
	ON_BN_CLICKED(RB_RECT, OnRect)
	ON_EN_CHANGE(EF_CPCTS, OnChangeCpcts)
	ON_EN_CHANGE(EF_STPCTS, OnChangeStpcts)
	ON_WM_ACTIVATE()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(RB_NURB, OnNurb)
	ON_EN_CHANGE(EF_X, OnChangeXYZ)
    ON_EN_CHANGE(EF_Y, OnChangeXYZ)
    ON_EN_CHANGE(EF_Z, OnChangeXYZ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgEditor2D message handlers
void SizeHere(CWnd* pWnd, int iWidth, int iHeight)
{
    PThis->OnSize(0, iWidth, iHeight) ;
}

BOOL SetCursorHere(size_t msg, V_ACTION act,  CPoint& p, V3& wp) 
{
    if(!PThis || !PThis->_pStaticGL)
        return FALSE;
    if(PThis->_pStaticGL->_nMode==V_3DFRONT||PThis->_pStaticGL->_nMode == V_3D)
        return FALSE;
    
    return PThis->OnSetCursor((size_t)act, p, wp);
}


BOOL    MouseHere(size_t msg, V_ACTION act,  CPoint& p, V3& wp)
{
    switch(msg)
    {
        case WM_LBUTTONDOWN:
            return PThis->OnLButtonDown((size_t)act, p, wp);
        case WM_LBUTTONUP:
            return PThis->OnLButtonUp((size_t)act, p, wp);
        case WM_RBUTTONDOWN:
            return PThis->OnRButtonDown((size_t)act, p, wp);
        case WM_RBUTTONUP:
            return PThis->OnRButtonUp((size_t)act, p, wp);
        case WM_MOUSEMOVE:
            return PThis->OnMouseMove((size_t)act, p, wp);
    }
    return 0;
}

void    PaintHere(CWnd* pWnd , V_MODE mode)
{

    PathShape*  ps;
    if(PThis->_step == DlgEditor2D::STEP_MKBRUSH)
    {
        if(PThis->_brush._polys.size()) 
        {
            BOOL globalLight;
            CLR  color;
            BOOL modewire;

            if(PThis->_pStaticGL->_vireframe)
            {
                modewire = DOC()->_vmodefire;
                DOC()->_vmodefire = 1;
                Ta.Disable();
                glEnable(GL_CULL_FACE);
		        glFrontFace(GL_CCW);
		        glCullFace(GL_FRONT);
	            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {

	            V3 pl = PThis->_pStaticGL->_cam._pos;
                pl += PThis->_pStaticGL->_cam._right   * 128;
                pl += PThis->_pStaticGL->_cam._up     * 128;

                float position[4] = {pl.x,pl.y,pl.z,1};
                REAL a[4]={0.7, 0.7, 0.7,  1.0};
                REAL d[4]={0.7, 0.7, 0.7, 1.0};
                REAL s[4]={0.9, 0.9, 0.9, 1.0};
                float cut_off                = 360.0;
    
                Ta.Enable();

                glEnable(GL_COLOR_MATERIAL);
                glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	    
                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glEnable(GL_NORMALIZE);

                glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
                glMaterialfv(GL_FRONT, GL_SPECULAR, s);
                glMateriali(GL_FRONT, GL_SHININESS, 80);

    
                glLightfv(GL_LIGHT0, GL_AMBIENT,  a);
	            glLightfv(GL_LIGHT0, GL_DIFFUSE,  d);
	            glLightfv(GL_LIGHT0, GL_SPECULAR, s);
	            glLightfv(GL_LIGHT0, GL_POSITION, (float *) position);
                glEnable(GL_LIGHT0);

                GNormals = 1;

                globalLight = GLight;
                GLight = 0;
            }


            BOOL            b = 0;
            vvector<Poly*>  pDummy;
            DOC()->DrawSolidBrush(PThis->_brush, 
                                  pDummy,
                                  pDummy,
                                  pDummy,
                                  pDummy,
                                  pDummy, 
                                  b);

            if(PThis->_pStaticGL->_vireframe)
            {
                DOC()->_vmodefire = modewire;
                glEnable(GL_CULL_FACE);
		        glFrontFace(GL_CCW);
		        glCullFace(GL_FRONT);
	            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            }
            else
            {
                GNormals = 0;
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHTING);
                DOC()->_scene._si.defLumin = color;
                GLight = globalLight;
            }
        }
        else
        {
            // draw shapes along the path
            ps = &PThis->_path;
            glColor3ubv(CLR_MOTION);
            glBegin(GL_LINE_STRIP);

            for(size_t i=0; i<ps->_points.size(); i++)
            {
                VNT& vtn = ps->_points[i];
                glVertex3f(vtn.point.x, vtn.point.y, vtn.point.z);
            }
            
            
            glEnd();
            glColor3ubv(CLR_NORMAL);

            if(ps->_points.size())
            {

                for(size_t i=1; i < ps->_points.size()-1; i++)
                {
                    V3  prev  = ps->_points[i-1].point;
                    V3& cur   = ps->_points[i].point;
                    V3& next  = ps->_points[i+1].point;
                    
                    V3  dir1 = cur-prev;
                    V3  dir2 = next-cur;

                    dir1.norm();
                    dir2.norm();

                    dir1+=dir2;
                    dir1.norm();

                    V3   rv   = Vcp(VZ, dir1);
                    REAL ran  = R2G(acosf(Vdp(VZ, dir1)));       // + 180;
                                        
                    glPushMatrix();
                    glTranslatef(cur.x,cur.y,cur.z);
                    if(rv.x||rv.y||rv.z)
                        glRotatef(ran, rv.x, rv.y, rv.z);
                    // draw the shape
                    PathShape*  pss = &PThis->_shape;
                    glBegin(GL_LINE_LOOP);
                    for(size_t i=0; i<pss->_points.size(); i++)
                    {
                        VNT& vtn = pss->_points[i];
                        glColor3ubv(CLR_DARK);
                        glVertex3f(vtn.point.x, vtn.point.y, vtn.point.z);
                    }
                    glEnd();

                    glPopMatrix();
                }
            }
        }
    }
    else if(PThis->_pWorking )
    {
        size_t i;
        CStaticGL* pGl = PThis->_pStaticGL;
        GLUquadricObj* go;
        go = gluNewQuadric();
        

        ps = PThis->_pWorking;
        glColor3ubv(CLR_DARK);
        //CLR color = DOC()->_scene._si.defLumin;
        //DOC()->_scene._si.defLumin = ZWHITE;
        //glPointSize(4.0f);
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        DOC()->ReposCamLight(BIT_AMBIENT|BIT_SPECULAR, 0);
        //glBegin(GL_POINTS);
        for( i=0; i<ps->_points.size(); i++)
        {
            VNT& vtn = ps->_points[i];
            if(ps->_selp && (abs(ps->_selp)-1)==i)
                glColor3ubv(CLR_SELECT);
            else
                glColor3ubv(CLR_NORMAL);
            //glVertex3f(vtn.point.x, vtn.point.y, vtn.point.z);
            glPushMatrix();
            glTranslatef(vtn.point.x, vtn.point.y, vtn.point.z);
            glScalef(.5/pGl->_zoom, .5/pGl->_zoom, .5/pGl->_zoom);
            gluSphere(go, 12.0, 12, 12);
            glPopMatrix();
        }
        //glEnd();
        //glPointSize(1.0f);
        glDisable(GL_NORMALIZE);
        glDisable(GL_LIGHTING);
        gluDeleteQuadric(go);
        //DOC()->_scene._si.defLumin = color;
        glColor3ubv(CLR_MOTION);

        //if(PThis->_bShowSpline)
        vvector<VNT>* pPoints=0;//
        if(ps->_pathspl.size())
            pPoints = &ps->_pathspl;
        else if(ps->_points.size())
            pPoints = &ps->_points;

        if(pPoints)
        {
            if(ps == &PThis->_shape)
            {
                glBegin(GL_LINE_LOOP);
                for(size_t i=0; i < pPoints->size(); i++)
                {
                    VNT& vtn = pPoints->at(i);
                    glVertex3fv(vtn.point);
                }
                glEnd();

            }
            else
            {
                glBegin(GL_LINE_STRIP);
                for(size_t i=0; i < pPoints->size(); i++)
                {
                    VNT& vtn = pPoints->at(i);
                    glVertex3fv(vtn.point);
                }
                glEnd();
            }
        }
        


        //DOC()->SelectTextCtx(pGl->_rt);
        glPushMatrix();
        glColor3ubv(CLR_NORMAL);
        CLR c(0, 255, 0, 255);
        CLR cs(0, 255, 255, 255);
        V3  offp, v=V1;
        
        
        for(i=0; i<ps->_points.size(); i++)
        {
            offp = ps->_points[i].point;
            offp.addscaled(v, 10/pGl->_zoom);
            
            if(ps->_selp && (abs(ps->_selp)-1)==i)
            {
                DOC()->PrintOut(offp, PThis->_pStaticGL->_vt, MKSTR("%d",i+1), cs);
                offp.addscaled(v, 10/pGl->_zoom);
                DOC()->PrintOut(offp, PThis->_pStaticGL->_vt, MKSTR("x=%.2f", (ps->_points[i].point.x)), cs);
                
                offp.addscaled(v, 10/pGl->_zoom);
                DOC()->PrintOut(offp, PThis->_pStaticGL->_vt, MKSTR("y=%.2f", (ps->_points[i].point.y)), cs);

                offp.addscaled(v, 10/pGl->_zoom);
                DOC()->PrintOut(offp, PThis->_pStaticGL->_vt, MKSTR("z=%.2f", (ps->_points[i].point.z)), cs);
            }
            else
                DOC()->PrintOut(offp, PThis->_pStaticGL->_vt, MKSTR("%d",i+1), c);
        }
        glPopMatrix();

        //DOC()->DeSelectTextCtx();
    }
}

BOOL DlgEditor2D::OnInitDialog() 
{
    _pagechange=1;
	CBASEDLG::OnInitDialog();
	_pStaticGL = FRM()->GetGLWidnow(this, ST_PLACE);
    
#ifdef _FREE
    ((CEdit*)GetDlgItem(EF_CPCTS))->SetLimitText(1);
    ((CEdit*)GetDlgItem(EF_STPCTS))->SetLimitText(1);
    ((CEdit*)GetDlgItem(EF_STPCTS))->SetWindowText("4");
#else

    ((CEdit*)GetDlgItem(EF_CPCTS))->SetLimitText(2);
    ((CEdit*)GetDlgItem(EF_STPCTS))->SetLimitText(2);
    ((CEdit*)GetDlgItem(EF_STPCTS))->SetWindowText("4");
#endif //

 //   _pStaticGL->_pcbsz      = SizeHere;
    _pStaticGL->_pcbsetcursor = SetCursorHere;
    _pStaticGL->_pcbmouse     = MouseHere;
    _pStaticGL->_pcbpaint     = PaintHere;

    //_pStaticGL->_zoom         = GMeter

    _pStaticGL->b_showGrid    = 1;
    _pWorking                 = 0;

    RECT rt;
    _pStaticGL->GetClientRect(&rt);
    SizeHere(_pStaticGL, rt.right, rt.bottom);
    CheckRadioButton(RB_RECT,RB_SPLINE,RB_SPLINE);
    GetDlgItem(PB_NEXT)->EnableWindow(0);
    GetDlgItem(PB_CLEAR)->EnableWindow(0);
	GetDlgItem(IDOK)->EnableWindow(0);
    _step = STEP_MKSHAPE;

    CFont *pf = this->GetFont();
    LOGFONT lf;
    pf->GetLogFont(&lf);
    strcpy(lf.lfFaceName,"Verdana");
    
    lf.lfHeight = -24;  
    lf.lfWidth  = 16;
    CFont f;
    f.CreateFontIndirect(&lf);
    m_stout.SetFont(&f);
    _pStaticGL->SetMode(V_2D,'z');

    EvaluateStep();
    _pagechange=0;
    OnSpline() ;
    return TRUE;  
}

void DlgEditor2D::OnDestroy() 
{
	CBASEDLG::OnDestroy();

    CFont *pf = m_stout.GetFont();
    DeleteObject((HFONT)(*pf));

	
    _pStaticGL->_pcbsz    = 0;
    _pStaticGL->_pcbmouse = 0;
    _pStaticGL->_pcbpaint = 0;
    _pStaticGL->b_showGrid = 0;
    _pStaticGL->HideButtons(FALSE);
	FRM()->GetGLWidnow(0, IDC_STATICGL);
}

void DlgEditor2D::OnOk() 
{
	if(PThis->_brush._polys.size())
	{
		Brush*    pBrush   = new Brush(PThis->_brush);

        DOC()->_cachedSett._detached = 0;
        DOC()->_cachedSett._zone = 0;

		pBrush->_group = DOC()->_curCreatGroup;
		pBrush->_brushflags = BRSH_NEW|BRSH_DETAIL|BRSH_SOLID;
		
		DOC()->_selmode		    = SEL_BRUSH;
		DOC()->_bShowCsg		= 0;
        DOC()->_compviewmode    = C_NA;
		DOC()->_brmode			= BR_MOVE;

		DOC()->_maxUsedGrp     = tmax(DOC()->_maxUsedGrp, DOC()->_curCreatGroup);
		DOC()->_brmode = BR_MOVE;

		pBrush->_cf._tm      = TM_LIN;
#pragma message ("why is projector ??????")
		pBrush->_cf._brushflags   = BRSH_PROJECTOR;
		pBrush->_cf._hstrips = 0;
		pBrush->_cf._vstrips = 0;
		pBrush->_cf._thick   = 0;
		pBrush->_cf._t       = E_CUBE;

 		DOC()->_pSelBrushes.push_back(pBrush); //place in sel brushes witht eh new flag set
		DOC()->Invalidate();
		
	}
	CDialog::OnOK();
}

void DlgEditor2D::OnPaint() 
{
    _pStaticGL->GLPaint();		
    CDialog::OnPaint();
}

BOOL DlgEditor2D::OnLButtonDown(size_t nFlags, CPoint& point, V3& wp) 
{
    if(_pWorking == 0)
        return 0;
   if(_pWorking->_selp < 0)
   {
       _pWorking->_selp = 1; // make it movable
   }
   _mm._ldown = 1;
   _mm._ptd   = point;
   _mm._wpdown= wp;
    return 0;
}

BOOL DlgEditor2D::OnLButtonUp(size_t nFlags, CPoint& point, V3& wp) 
{
    if(0 == _pWorking )
        return 0;
    if(_mm._ldown)
    {
       _mm._ldown = 0;

       if(abs(_mm._ptd.x-point.x) < 2 && 
          abs(_mm._ptd.y-point.y) < 2)
       {
           if(AKEY(VK_CONTROL))
           {
                switch(_step )                   
                {
                    case STEP_MKSHAPE:
                        break;
                    case STEP_MKPATH:
                        {
						    if(_pStaticGL->_nMode==V_2D)
						    {
							    VNT vnt; 
							    vnt.point  = wp;
							    vnt.normal =  V3(0,1,0);

                           
							    _pWorking->_points.push_back(vnt);
                                
                                _plastsel = &_pWorking->_points.back();
                                {
                                    _blockupdate = 1;
                                    Sct(this, EF_X, _plastsel->point.x);
                                    Sct(this, EF_Y, _plastsel->point.y);
                                    Sct(this, EF_Z, _plastsel->point.z);
                                    _blockupdate = 0;
                                }

                                _pWorking->_pathspl.clear();
                                if(!IsDlgButtonChecked(RB_RECT))
                                {
                                    MakeSpline();
                                }
                            
							    SetDlgItemInt(EF_CPCTS, _pWorking->_points.size());
							    _pStaticGL->GLPaint();
						    }
					    
                        }
                        break;
                }
           }
           else if(_plastsel)
           {
               _blockupdate = 1;
                Sct(this, EF_X, _plastsel->point.x);
                Sct(this, EF_Y, _plastsel->point.y);
                Sct(this, EF_Z, _plastsel->point.z);
                _blockupdate = 0;
           }
       }
       else if(!IsDlgButtonChecked(RB_RECT))
       {
           if(_pWorking->_selp != 0)
           {
                MakeSpline();
           }
       }
       _pWorking->_selp = 0;
    }
    return 0;
}

BOOL DlgEditor2D::OnSetCursor(size_t nFlags, CPoint& point, V3 wp) 
{
    if(_pWorking)
    {
        CStaticGL* pGl = PThis->_pStaticGL;

        _pWorking->_selp = 0;
        for(size_t i=0; i<_pWorking->_points.size(); i++)
        {
            VNT&    vtn = _pWorking->_points[i];
            V3      vmul(1,1,1);    
            
            switch(_pStaticGL->_vt)
            {
            case 'x':    // x=0
                    if(_pWorking == &_shape)
                        return FALSE;
                    vmul.x=0;
                    break;
            case 'y':    // y=0
                    if(_pWorking == &_shape)
                        return FALSE;
                    vmul.y=0;
                    break;
            case 'z':    // z=0
                    vmul.z=0;
                    break;
            }

            V3  cp = vtn.point * vmul;
            V3  tp = wp * vmul;
            V3  df = cp - tp;
            if(df.x<0) df.x=-df.x;
            if(df.y<0) df.y=-df.y;
            if(df.z<0) df.z=-df.z;
            
            if(df.len() < 8./pGl->_zoom)
            {
                _pWorking->_selp = (i+1); //offset+1 // sel became pos when lbutdown
                _plastsel = &_pWorking->_points[_pWorking->_selp-1];
                SetCursor(theApp._cursors[CUR_MOVE]);
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL DlgEditor2D::OnMouseMove(size_t nFlags, CPoint& point, V3& wp) 
{
    if(nFlags == A_SHIFTVIEW)
    {

        // wp  has shift value
        if(_pWorking && _pWorking->_selp>0)
        {
            int  pidx = _pWorking->_selp-1;
            VNT& vtn  = _pWorking->_points[pidx];
            V3   t;            
            switch(_pStaticGL->_vt)
            {
                case 'x':
                    if(_pWorking == &_shape)
                        break;
                    t.z = -wp.z;
                    t.y = -wp.y;
                    break;
                case 'y':
                    if(_pWorking == &_shape)
                        break;
                    t.x = -wp.x;
                    t.z = wp.z;
                   break;
                case 'z':
                    
                    t.x = -wp.x ;
                    t.y = -wp.y;
                    break;
            }
            vtn.point += t;
            
            // test the convexity
            /*
            if(_pWorking == &_shape)
            {
                V3 pp = _pWorking->_points.prevr(pidx).point;
                V3 np = _pWorking->_points.nextr(pidx).point;

                Plane p(pp, vtn.point, np);

                //TRACE("%2.2f %2.2f %2.2f\r\n", p._n.x, p._n.y, p._n.z);
                
                if(p._n.z>0)
                {
                    vtn.point -= t;
                }
            }
            */
            _pStaticGL->GLPaint();
            return 1;
        }
    }
    return 0;
}

BOOL DlgEditor2D::OnRButtonDown(size_t nFlags, CPoint& point, V3& wp) 
{
    if(0 == _pWorking )
        return 0;
	if(_pWorking->_selp) 
	{
		for(vvector<VNT>::iterator i = _pWorking->_points.begin();
			i != _pWorking->_points.end(); i++)
		{
			if(i-_pWorking->_points.begin() == _pWorking->_selp)
			{
				_pWorking->_points.erase(i);

                if(_plastsel == &_pWorking->_points[_pWorking->_selp-1])
                {
                    _plastsel = 0;
                    SetDlgItemText(EF_X,"");
                    SetDlgItemText(EF_Y,"");
                    SetDlgItemText(EF_Z,"");

                }
                

                _pWorking->_pathspl.clear();
                if(!IsDlgButtonChecked(RB_RECT))
                {
			        MakeSpline();
                }
				_pStaticGL->InvalidateRect(0);
				break;
			}
		}
	}

    return 0;
}

BOOL DlgEditor2D::OnRButtonUp(size_t nFlags, CPoint& point, V3& wp) 
{
    return 0;
}

void DlgEditor2D::OnSize(size_t nType, int iWidth, int iHeight) 
{
	
}

void DlgEditor2D::OnPrev() 
{
    _plastsel = 0;
    SetDlgItemText(EF_X,"");
    SetDlgItemText(EF_Y,"");
    SetDlgItemText(EF_Z,"");

    _pagechange = 1;
    _sstates[_step]._viewMode   = _pStaticGL->_nMode;
    _sstates[_step]._vproj      = _pStaticGL->_vt;

    switch(_step )
    {
        case STEP_MKSHAPE:
            break;
        case STEP_MKPATH:
            _step = STEP_MKSHAPE;
            break;
        case STEP_MKBRUSH:
            _step = STEP_MKPATH;
            break;
    }
    EvaluateStep();
    _pStaticGL->SetMode(_sstates[_step]._viewMode, _sstates[_step]._vproj);

    _pagechange = 1;
}


void DlgEditor2D::OnNext() 
{
    _plastsel = 0;

    SetDlgItemText(EF_X,"");
    SetDlgItemText(EF_Y,"");
    SetDlgItemText(EF_Z,"");

    _pagechange = 1;
    // save states on this step
    _sstates[_step]._viewMode   = _pStaticGL->_nMode;
    _sstates[_step]._vproj      = _pStaticGL->_vt;

    
    switch(_step )
    {
        case STEP_MKSHAPE:
            _step = STEP_MKPATH;
            break;
        case STEP_MKPATH:
            OnMake();
            _step = STEP_MKBRUSH;
            OnMake();
            break;
        case STEP_MKBRUSH:
            OnMake();
            break;
    }
    EvaluateStep();

    _pStaticGL->SetMode(_sstates[_step]._viewMode, _sstates[_step]._vproj);

    _pagechange = 0;
}

void DlgEditor2D::EvaluateStep()
{
    _pWorking = 0;
    switch(_step )
    {
        case STEP_MKSHAPE:
			_pWorking = &_shape;
			SetDlgItemInt(EF_CPCTS, _pWorking->_points.size());
			GetDlgItem(EF_CPCTS)->EnableWindow(1) ;
            m_n.EnableWindow(_pWorking->_points.size()>2);
            m_n.SetWindowText(">>");
            m_curs.SetWindowText("Create Closed Shape");
            m_stout.SetWindowText("Create Closed Shape");
            m_p.EnableWindow(0);


            break;
        case STEP_MKPATH:
			_pWorking = &_path;
			SetDlgItemInt(EF_CPCTS, _pWorking->_points.size());
            m_n.EnableWindow(_pWorking->_points.size()>2);
			GetDlgItem(EF_CPCTS)->EnableWindow(0) ;
            m_n.SetWindowText(">>");
            m_curs.SetWindowText("Click and Hold Ctrl on 2D View To Create Extrusion Path");
            m_stout.SetWindowText("Click and Hold Ctrl  2D View To Create Extrusion Path");
            m_p.EnableWindow(1);


            break;
        case STEP_MKBRUSH:
			GetDlgItem(EF_CPCTS)->EnableWindow(0);
            m_n.EnableWindow(_path._points.size() && _shape._points.size());
            m_n.SetWindowText("Finish");
            m_curs.SetWindowText("Press Next to Create Brush");
            _pWorking = 0;
            m_p.EnableWindow(1);
            break;
    }
}

void DlgEditor2D::OnClear() 
{
    while(_step != STEP_MKSHAPE)
        OnPrev();

    if(_pWorking)	
        _pWorking->Clear();
    _brush.Clear();
    _pStaticGL->InvalidateRect(0);
}

void DlgEditor2D::OnSpline() 
{
    GetDlgItem(IDC_STATICD)->EnableWindow(1);
    GetDlgItem(EF_STPCTS)->EnableWindow(1);
    GetDlgItem(EF_STPCTS)->EnableWindow(1);
    SetDlgItemInt(EF_STPCTS, 4);
    GetDlgItem(EF_STPCTS)->Invalidate();
    GetDlgItem(PB_NEXT)->EnableWindow(GetDlgItemInt(EF_STPCTS)>=4 && GetDlgItemInt(EF_CPCTS)>=4);

    if(_pWorking)
        MakeSpline();
    Invalidate();

}

void DlgEditor2D::OnNurb() 
{
	OnSpline() ;
}



void DlgEditor2D::OnRect() 
{
    GetDlgItem(IDC_STATICD)->EnableWindow(0);
    GetDlgItem(EF_STPCTS)->EnableWindow(0);
    GetDlgItem(PB_NEXT)->EnableWindow(GetDlgItemInt(EF_CPCTS)>=4);

    if(_pWorking)
        _pWorking->_pathspl.clear();
    Invalidate();
}

void DlgEditor2D::OnChangeCpcts() 
{
    if(_pagechange)
        return;
    if(!IsDlgButtonChecked(RB_RECT))
        GetDlgItem(PB_NEXT)->EnableWindow(GetDlgItemInt(EF_STPCTS)>=3 && GetDlgItemInt(EF_CPCTS)>=3);
    else
        GetDlgItem(PB_NEXT)->EnableWindow(GetDlgItemInt(EF_CPCTS)>=3);

    if(_step == STEP_MKSHAPE )
    {
        _pWorking->nSteps   = GetDlgItemInt(EF_STPCTS);
        _pWorking->nPoints  = GetDlgItemInt(EF_CPCTS);

        _pWorking->Clear();
        if(_pWorking->nPoints >= 2)
        {
            MakeShape();
        }
    }
    
}

void DlgEditor2D::OnChangeStpcts() 
{
    if(_pagechange)
        return;

    if(!IsDlgButtonChecked(RB_RECT))
        GetDlgItem(PB_NEXT)->EnableWindow(GetDlgItemInt(EF_STPCTS)>=4 && GetDlgItemInt(EF_CPCTS)>=4);
    else
        GetDlgItem(PB_NEXT)->EnableWindow(GetDlgItemInt(EF_CPCTS)>=4);

    if(_pWorking)
    {
        _pWorking->Clear();
        if(_pWorking->nPoints >= 2)
        {
            MakeShape();
        }
    }
}

void DlgEditor2D::OnMake() 
{
    if(_pWorking)
    {
        _pWorking->nSteps   = GetDlgItemInt(EF_STPCTS);
        _pWorking->nPoints  = GetDlgItemInt(EF_CPCTS);

        if(_pWorking->nPoints)
		{
			switch(_step )
			{
				case STEP_MKSHAPE:
					m_n.EnableWindow(_pWorking->_points.size()>2);
					break;
				case STEP_MKPATH:
					m_n.EnableWindow(_pWorking->_points.size()>2);
					break;
			}
		}
    }
    else
        CreateBrush();

    // if shape make a def circle that can be reshaped
    // if path make a line that can be curved
}

void DlgEditor2D::MakeShape()
{
    if(_pWorking==0 || _pWorking !=& _shape)
        return;
    REAL zoom  = _pStaticGL->_zoom;

    if(_pWorking->_points.size())
        return;

    // build points layin on xoy
    
    double   arcstep = DOIPI/_pWorking->nPoints;
    double   uplimit = DOIPI + arcstep/2.0;
    double   arcPrev=0.0;

    for(double arc = arcstep; arc < uplimit; )
    {
        VNT vnt; 
        vnt.point.setval(sin(arcPrev)*128.0, cos(arcPrev)*128.0, 0);
        vnt.normal =  V3(0,0,1);
        _pWorking->_points.push_back(vnt);
        arcPrev =  arc;
        arc     += arcstep;
    }

    _pWorking->_pathspl.clear();
    if(!IsDlgButtonChecked(RB_RECT))
         MakeSpline();
    _pStaticGL->Invalidate();
}


void DlgEditor2D::MakePath()
{
    if(_pWorking==0 || _pWorking !=& _path)
        return;

    _pWorking->Clear();
    REAL zoom  = _pStaticGL->_zoom;
    REAL sp    = (_pWorking->nPoints/2) * (-128);

    // spread it - 2 + along z
    for(int i=0;i<_pWorking->nPoints; i++)
    {
        VNT vnt; 
        vnt.point.setval(0, 0, sp + (128*i));
        vnt.normal =  V3(0,1,0);
        _pWorking->_points.push_back(vnt);
    }
    _pStaticGL->Invalidate();
}


void DlgEditor2D::OnActivate(size_t nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CBASEDLG::OnActivate(nState, pWndOther, bMinimized);
    if(WA_ACTIVE == nState)
    {
        InvalidateRect(0);
        UpdateWindow();
    }
}


void DlgEditor2D::CreateBrush()
{
    CWaitCursor cwc;

    if(_path._points.size()==0||_shape._points.size()==0)
        return;

    vvector<vvector<V3> >   skin;
    vvector<V3>             cap;
    // extrude the shape along the path
    

    // build the cap
    // draw shapes along the path
    PathShape*  ps  = &PThis->_path;
    PathShape*  pss = &_shape;

    vvector<VNT>* pvtn = &ps->_points;
    vvector<VNT>* pvtn2 = &pss->_points;

    if(ps->_pathspl.size())
         pvtn = &ps->_pathspl;
    else
         pvtn = &ps->_points;

    if(pss->_pathspl.size())
        pvtn2 = &pss->_pathspl;
    else
        pvtn2 = &pss->_points;
    
    V3  next,cur,prev,dir1,dir2;

    if(ps->_points.size() && pvtn->size())
    {
        for(size_t i=0; i < pvtn->size(); i++)
        {
            cur   = pvtn->at(i).point;            

            if(i>0)
                prev  = pvtn->at(i-1).point;
            else
                prev = cur;

            if(i < pvtn->size()-1)
                next  = pvtn->at(i+1).point;
            else
                next = cur;

            dir1 = cur - prev;
            dir2 = next - cur;

            dir1.norm();
            dir2.norm();

            V3  r2 = VZ;
            V3   rv;
            dir1+=dir2;
            dir1.norm();

            if(IsZero((dir1.z-1.), EPSMED))
                rv   = VY;
            else
                rv   = Vcp(r2, dir1);

            REAL ran;
            ran  = acosf(Vdp(r2, dir1));         
            REAL g = R2G(ran);
            M4  mt= MTranslate(cur.x,cur.y,cur.z);

            if(rv.x||rv.y||rv.z)
            {
                mt *= MRotAround(rv, ran);
            }

            // draw the shape
            for(size_t j=0; j < pvtn2->size(); j++)
            {
                // build the pair points skin
                V3 v = pvtn2->at(j).point;
                mt.v3transform(v);
                cap << v;
            }
            skin << cap;
            cap.clear();

        }
    }
    // we have the caps

    _brush.Clear();
    Poly    poly;
    V3      corn[4];

    if(skin.size())
    {
        for(size_t i=0; i < skin.size()-1; i++)
        {
            vvector<V3>& cap1 = skin[i];
            vvector<V3>& cap2 = skin[i+1];
        
            for(size_t j=0; j < cap1.size(); j++)
            {
                corn[0] = cap1[j];
                corn[1] = cap1.nextr(j);
                corn[2] = cap2.nextr(j);
                corn[3] = cap2[j];

                poly.Create(4, corn, &_brush);
                _brush << poly;
                poly.Clear();
            }
        }
        _brush.CalctexCoord(0);
        _brush.CalctexCoord(1);
        _brush.Recalc(1);

	    GetDlgItem(IDOK)->EnableWindow(1);
    }
}


void DlgEditor2D::OnKeyDown(size_t nChar, size_t nRepCnt, size_t nFlags) 
{

	CBASEDLG::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL DlgEditor2D::OnSetCursor(CWnd* pWnd, size_t nHitTest, size_t message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CBASEDLG::OnSetCursor(pWnd, nHitTest, message);
}

void    DlgEditor2D::MakeSpline()
{
    
    int iCpcts = GetDlgItemInt(EF_STPCTS);
    _pWorking->_pathspl.clear();
    if(iCpcts < 4)
        return;
    if(!!IsDlgButtonChecked(RB_RECT) || _pWorking->_points.size() < 4)
        return;

    vvector<VNT>    points(128);
    points = _pWorking->_points; 

    //points << _pWorking->_points.back(); 
    if(_step == STEP_MKSHAPE)
    {
        vvector<VNT>::iterator b = _pWorking->_points.begin();
        points << *b;
        points << *b;  
        points << *b;
    }
    else
    {
        points << _pWorking->_points.back(); 
        points << _pWorking->_points.back(); 
        points << _pWorking->_points.back(); 
    }
/*
    CCurve_Class::CURVTYPE t = CCurve_Class::CURV_SPLINE;
    if(IsDlgButtonChecked(RB_NURB))
        t = CCurve_Class::CURV_NURB;

    CCurve_Class c(t);
    c.Compute(points, iCpcts, _pWorking->_pathspl);
*/
    _TRY
    {
        if(iCpcts>3)iCpcts=3;//HACL
        bspline(points.size(), iCpcts, points, _pWorking->_pathspl, iCpcts * points.size());
    }
    _CATCHX()
    {
        _pWorking->_pathspl.clear();
    }
}


HBRUSH DlgEditor2D::OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor) 
{
	HBRUSH hbr = CBASEDLG::OnCtlColor(pDC, pWnd, nCtlColor);
	
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void DlgEditor2D::OnChangeXYZ() 
{
    if(_blockupdate) 
        return;
    if(_plastsel)
    {
        _plastsel->point.x = ParseREAL(this, EF_X);
        _plastsel->point.y = ParseREAL(this, EF_Y);
        _plastsel->point.z = ParseREAL(this, EF_Z);
        _pStaticGL->InvalidateRect(0);
    }
}
