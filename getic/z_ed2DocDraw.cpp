// z_ed2Doc.cpp : implementation of the CZ_ed2Doc class
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GeoUtils.h"
#include "z-edmap.h"
#include "texsys.h"
#include "z_ed2Doc.h"
#include "mainfrm.h"
#include "Z_ed2View.h"
#include "Z_ed3View.h"
#include <algorithm>
#include "lightbulb.h"
#include "texadapter.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//-----------------------------------------------------------------------------
static void D32LocalDrawWireBrush(CZ_ed2View *pV, Brush* pbr, int mode , 
                                  BOOL bPOints, BOOL pselState, BYTE *pclr)
{
	REAL    h  = pbr->_box.GetExtends().y;
	REAL    m =  pbr->_box._min.y;
    int     index = 0;
    V3      pcenter;  
    BYTE    lc[4];
    PPoly   pprj = pbr->_polys.end()   ;

    Ta.Disable();

    PPoly   pb = pbr->_polys.begin();
    PPoly   pe = pbr->_polys.end();
    for(;pb!=pe;pb++)
    {
        Poly& pl = *pb;
        if(pbr->IsProjector() && 2 == index)
        {
            pprj = pb; // show texture only on second face of proj brush
        }

		PVertex vb = pl._vtci.begin();
        PVertex ve = pl._vtci.end();

        if(SEL_FACE   == DOC()->_selmode)
        {
            if(pl.IsSelected())
                glColor4ubv(CLR_WHITE);
            else
                glColor4ubv(pclr);
        }

        glBegin(mode);
        for(;vb!=ve;vb++)
        {
            V3& v = vb->_xyz;
            if(pbr->IsProjector() && 2 == index)
                pcenter+=v;

			if(pbr->_brushflags & BRSH_CSGTERRAIN)
			{
                if(!pbr->_selected)
                {
    				lc[0] = CLR_TERRAIN[0] + ((v.y-m) / h * 64);
	    			lc[1] = CLR_TERRAIN[1] + ((v.y-m) / h * 64);
		    		lc[2] = CLR_TERRAIN[2] + ((v.y-m) / h * 64);
			    	lc[3] = CLR_TERRAIN[3];
				    glColor4ubv(lc);
                }
			}
            if(vb->_rgb.r != 255 || 
               vb->_rgb.g != 255 || 
               vb->_rgb.b != 255)
            {
                glColor4ubv((BYTE*)vb->_rgb);
            }
            else
                glColor4ubv(pclr);
            glVertex3f(v.x,v.y,v.z);
        }
        glEnd();
        ++index;
    }

    if(pprj != pbr->_polys.end()   )
    {
        pcenter/=4.0f;
        pcenter += pprj->_n * 128.0;

        glBegin(GL_LINES);
        PVertex vb = pprj->_vtci.begin();
        PVertex ve = pprj->_vtci.end();
        for(;vb!=ve;vb++)
        {
            glVertex3f(pcenter.x, pcenter.y, pcenter.z);
            glVertex3f(vb->_xyz.x, vb->_xyz.y,vb->_xyz.z);
        }
        glEnd();
    }

    if(bPOints)
    {
        glPointSize(4.f);
            glBegin(GL_POINTS);
                pb = pbr->_polys.begin();
                pe = pbr->_polys.end();
                for(;pb!=pe;pb++)
                {
		            FOREACH(vvector<Vtx2>, (pb)->_vtci, v)
		            {
			            glVertex3f(v->_xyz.x,v->_xyz.y,v->_xyz.z);
		            }
                }
            glEnd();
        glPointSize(1.f);
    }

    if(pbr->_brushflags & BRSH_NEW && pV)    
    {
        glColor4ubv(CLR_WHITE);
        CLR color(CLR_NEW_BRITEM[0],CLR_NEW_BRITEM[1],CLR_NEW_BRITEM[2]);
        DOC()->PrintOut(pbr->_box.GetCenter(), pV->_vt, "Hit Enter", color);
    }
    Ta.Restore();
}

//-----------------------------------------------------------------------------
static void LocalDrawSelPolys(Poly** pPS , int nCount, CZ_ed2View* pV)
{
    int i;
    Ta.Disable();
    Poly** pPSCache =  pPS;
    
    for(int i=0; i < nCount; i++, pPS++)
    {
        glBegin(GL_LINE_LOOP);
        FOREACH(vvector<Vtx2>, (*pPS)->_vtci, pv)
        {
            Vtx2& v = *pv;
            glVertex3f(v._xyz.x,v._xyz.y,v._xyz.z);
        }
        glEnd();
    }

    pPS = pPSCache;  
    glPointSize(4.f);
    glBegin(GL_POINTS);
    for(i=0; i < nCount; i++, pPS++)
    {
        FOREACH(vvector<Vtx2>, (*pPS)->_vtci, pv)
        {
            Vtx2& v = *pv;
            glVertex3f(v._xyz.x,v._xyz.y,v._xyz.z);
        }
    }
    glEnd();
    glPointSize(1.f);
    Ta.Restore();
}

//-----------------------------------------------------------------------------
void  CZ_ed2Doc::LocalDrawBspTreeGraph(Compiler& compiler, int& leafNo, z_ed3View* pV)
{
    V3  vtree;

    
	int nLeaf = compiler._bTrees[0]->R_FindLeaf(0,_cam._pos);
    FOREACH(vvector<CBspTree*>, compiler._bTrees, ppBt)
    {
        glPushMatrix();
        {
            // show the tree graphic
            if(leafNo<0)
		    {
                glTranslatef(vtree.x, vtree.y, vtree.z);
			    (*ppBt)->Draw3DBSPTree(pV, leafNo, nLeaf, _compviewmode);
		    }
		    else 
            // highlight the selected current lef  _leafNo
			if(leafNo>=0 && (*ppBt)->IsMainModel())
			{
				(*ppBt)->Draw3DBSPTree(pV, leafNo, nLeaf, _compviewmode);
				if(leafNo>=0)
                {
					SBT(0,MKSTR("Leaf: %d: %d Polys, Zone: %d",leafNo, (*ppBt)->_leafs[leafNo]->_nPolys,
                                                                        (*ppBt)->_leafs[leafNo]->_zonearea));
                }
			}
        }
        glPopMatrix();
        vtree.z-=2000;
    }
    
}

//-----------------------------------------------------------------------------
void CZ_ed2Doc::LocalDrawReadyBSP(Compiler&  compiler, Scene& scene, z_ed3View* pV)
{
    if(_clearing)
        return;

	int nLeaf = compiler._bTrees[0]->R_FindLeaf(0,_cam._pos);
    DWORD zoneVis = 0xFFFFFFFF;
    Brush* pz = compiler.GetZone(_cam._pos);
    if(pz) zoneVis =pz->VisData();
    FOREACH(vvector<CBspTree*>, compiler._bTrees, ppBt)
    {
        int il = -2;
        if(_compviewmode  & C_FULL)
        {
            if(0 == ((*ppBt)->_zoneareea & zoneVis))    
                continue;
        }
        (*ppBt)->Draw3DBSPTree(pV, il, nLeaf, _compviewmode);    //lmaps
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
    FOREACH(vvector<SceItem*>, (*scene.GetItems()), ppi)
    {
	    SceItem* pI = *ppi;
	    pI->GlDraw(0);
    }
    glDisable(GL_BLEND);
}

//-----------------------------------------------------------------------------
BOOL CZ_ed2Doc::InitGL(int i3d, HDC hDC, HGLRC& hrc)
{
/*?*
    PIXELFORMATDESCRIPTOR  pfd = {0}; 
    for(int i=0;i<8;i++)
    {
        DescribePixelFormat(hDC, i,  sizeof(PIXELFORMATDESCRIPTOR), &pfd); 
        if((pfd.dwFlags & (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER)) &&
           pfd.iLayerType  == PFD_MAIN_PLANE)
        {
            break;
        }
    }
*/
    
    
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize       = sizeof(pfd);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 24;
    pfd.cDepthBits  = 8;
    pfd.cStencilBits= 16;
    pfd.iLayerType  = PFD_MAIN_PLANE;
    

    int iFormat     = ChoosePixelFormat(hDC, &pfd);

    PIXELFORMATDESCRIPTOR pfd_new;
    DescribePixelFormat (hDC, iFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd_new);

    int generic_format = pfd_new.dwFlags & PFD_GENERIC_FORMAT;
    int generic_accelerated = pfd_new.dwFlags & PFD_GENERIC_ACCELERATED;

    if (generic_format && ! generic_accelerated)
    {
        AfxMessageBox("Cannot run without hardware acceleration Video Driver", MB_OK|MB_ICONHAND);
//        return 0;
    }
    SetPixelFormat(hDC, iFormat, &pfd);            

    hrc = wglCreateContext(hDC);
    
    MakeCurrent(hDC, hrc); 
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
	glClearColor(0.0f,0.0f,0.0f,0.f);
	SetFaceMode();

	if(3==i3d)
		SetLightMode(0);
	else
		glDisable(GL_LIGHTING);

    if(_extensions.empty())
    {
        _extensions = (char*)glGetString(GL_EXTENSIONS);
        _glAddSwapHintRectWIN = (PFNGLADDSWAPHINTRECTWINPROC) wglGetProcAddress("glAddSwapHintRectWIN");
        glGetIntegerv(GL_MAX_LIGHTS,&_maxLights);
    }
    return 1;
}

//-----------------------------------------------------------------------------
void  CZ_ed2Doc::SetLightMode(BOOL invalidate)
{
    MakeCurrent('3'); 
	if(!GLight)
	{
		glDisable(GL_LIGHTING);
	}
	else
	{
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(_vmodeback ? GL_FRONT_AND_BACK  : GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	    
    	if (HasExtension("_GL_EXT_separate_specular_color"))
        {
		    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
        }
        RecalcBrushes();
	}
    if(invalidate)
        Invalidate();
}

//-----------------------------------------------------------------------------
void  CZ_ed2Doc::RecalcBrushes()
{
    CWaitCursor  cwc;
    FOREACH(PBrushes, (*_scene.GetPrimitives()), ppbb)
    {
        Brush* pBrush = (*ppbb)->_pResult;
        if(pBrush->IsSolid())
        {
            pBrush->Recalc(1);
        }
    }
}

//-----------------------------------------------------------------------------
static BYTE* S_SelectColor(Brush& rb,BOOL sel=FALSE)
{
    if(sel)
    {
        glColor4ubv(CLR_SELECT);
        return CLR_SELECT;
    }
    if(rb._brushflags & BRSH_DISABLED)
    {
        glColor4ubv(CLR_DARK);
        return CLR_DARK;
    }
    if(rb._brushflags & BRSH_DETAIL)
    {
        glColor4ubv(CLR_DETAIL);
        return CLR_DETAIL;
    }
    if(rb._brushprops & MODEL_IS_SKYDOM)
    {
        glColor4ubv(CLR_SKYDOM);
        return CLR_SKYDOM;
    }
    if(rb._brushflags & BRSH_CSGTERRAIN || rb._brushflags & BRSH_BIGTERRAIN)
    {
	    glColor4ubv(CLR_TERRAIN);
        return CLR_TERRAIN;
    }
    if(rb._brushflags & BRSH_ZONE)
    {
        glColor4ubv(CLR_ZONE);
        return CLR_ZONE;
    }
    if(rb._brushflags & BRSH_SPLITTER)
    {
        glColor4ubv(CLR_CUT3DBRUSHREV);
        return CLR_CUT3DBRUSHREV;
    }
    if(rb._brushflags & BRSH_PROJECTOR)
    {
        glColor4ubv(CLR_PRJDBRUSH);
        return CLR_PRJDBRUSH;
    }
    if(rb.IsSolid())
    {
        glColor4ubv(CLR_NORMAL);
        return CLR_NORMAL;
    }
    glColor4ubv(CLR_CUT);
    return CLR_CUT;

}

//-----------------------------------------------------------------------------
void CZ_ed2Doc::Draw2DBrushes(CZ_ed2View *pV)
{
    if(_clearing)
        return;

    int           nCount;

    if(_compviewmode  & (C_FULL|C_PORT|C_LEAFS|C_LBYL|C_LMAP|C_GRAPH|C_ZONES))
    {
        _compiler.Render(0, _compviewmode|REND_BSP);

        DWORD zoneVis = 0xFFFFFFFF;
        Brush* pz = _compiler.GetZone(_cam._pos);
        if(pz) zoneVis =pz->VisData();
        BOOL visible = 1;

        FOREACH(vvector<CBspTree*>, _compiler._bTrees, ppBt)
		{
            if(_compviewmode  & C_FULL)
            {
                visible = ((*ppBt)->_zoneareea & zoneVis);    
            }
			(*ppBt)->Draw2GlTree(pV, _compviewmode, _leafNo, visible);    
		}
    }
    else if(HasForms()) // draw all
    {
		//
		// @d wireframe drawing
		//
        if(_scene.Terrain().Exist())
        {
            _scene.Terrain().Render(0);
        }

        Brush* pBr;
        glColor4ubv(CLR_NORMAL);
        if(GetForms()->size())
        {
            FOREACH(PBrushes, (*GetForms()), b)
            {
                // show result (with cuts) or (original primitive !cuts)
                if(GShowCuts)
                    pBr = &(*b)->GetResult();
                else
                    pBr = (*b);
                //rejecte selected. Last ones 
                if(pBr->IsSelected())
                    continue;

                if(_curgroup >= 0  && pBr->_group !=_curgroup)
                    continue;

                // reject out of view
		        if(!pV->IsBoxInViewPort(pBr->_box))
    		        continue;

		        BYTE* cr = S_SelectColor(*pBr);
                ::D32LocalDrawWireBrush(pV, pBr, GL_LINE_LOOP,0,0, cr);
            }
        }
    }

    // SELECTED BRUSHES
    Brush** pBS = GetSelBrushes(nCount);
    glColor4ubv(CLR_WHITE);
    for(int i=0; i < nCount; i++, pBS++)
    {
        //
		// selection not by result
        //
		Brush& rb = (**pBS);
        BYTE* pclr;
        if(_alignment)
        {
            //
            // if we align last brush will be colored slighty different
            // to show the alignment reference
            //
            if (i==nCount-1)
            {
                glColor4ubv(CLR_SELECT); 
                pclr=CLR_SELECT;
             }
            else
            {
                glColor4ubv(CLR_WHITE);
                pclr=CLR_WHITE;
            }
        }
        else
        {
			pclr=S_SelectColor(rb,TRUE);
        }
        ::D32LocalDrawWireBrush(pV, &rb, GL_LINE_LOOP, 1,0, pclr);

        if(_selShowSelBox)
            GLDrawBox(rb._box,1, CLR_PORTALSLINE);
    }

    if(_selBox._brushflags & BRSH_SELVIS)
    {
        if(_selBoxByTouch)
            glColor4ubv(CLR_SELBOX_TCH);
        else
            glColor4ubv(CLR_SELBOX);

        ::D32LocalDrawWireBrush(pV, &_selBox, GL_LINE_LOOP, false, 0, CLR_SELBOX);

    }
    
    Poly** pPS = GetSelPolys(nCount);
    if(nCount)
    {
        glColor4ubv(CLR_SELECT);
        ::LocalDrawSelPolys(pPS , nCount, pV);
    }

    //
    // sel vertexes
    //
    if(_pSelVertexes.size())
    {
		set<V3*>	uniques;
        glColor4ubv(CLR_SELECT);
        glPointSize(8.f);
        glBegin(GL_POINTS);
        FOREACH(vvector<Vtx2*>,_pSelVertexes,ppv)
        {
			if(uniques.end() == uniques.find(&(*ppv)->_xyz))
			{
				glVertex3f((*ppv)->_xyz.x,
					       (*ppv)->_xyz.y,
						   (*ppv)->_xyz.z);
				uniques.insert(&(*ppv)->_xyz);
			}
        }
        glEnd();
        glPointSize(1.f);
    }

    //
    // sel edgex
    //
    if(_selEdges.size())
    {
        glColor4ubv(CLR_SELECT);
        glLineWidth (4.0f);
        glBegin(GL_LINES);
        FOREACH(vvector<PolyEdge>, _selEdges, edge)
        {
            glVertex3fv((REAL*)edge->_edge[0]->_xyz);
            glVertex3fv((REAL*)edge->_edge[1]->_xyz);
        }
        glEnd();
        glLineWidth (1.0f);

    }


    glColor4ubv(CLR_WHITE);
    if(_selmode  == SEL_BRUSH)
    {
        DrawMotionsPaths();
    }
}

//-----------------------------------------------------------------------------
void    CZ_ed2Doc::Draw3DScene(z_ed3View* pV)
{
    if(_clearing)
        return;
++PaintCounter;
    if(_viewChanged-->0)
        SetFaceMode();
    
    glDisable(GL_BLEND);
	glPushMatrix();
    do{
		if(b_showgrid)
		{
			DrawAxis(V0, 32768);
			DrawGridLines();
		}

        if(_compviewmode != C_NA )
        {
            BOOL fe =0;
            if(_scene._si.fogDens)
            {
                REAL frgb[4];
                frgb[0]=(REAL)_scene._si.fogColor.r/255.0;
                frgb[1]=(REAL)_scene._si.fogColor.g/255.0;
                frgb[2]=(REAL)_scene._si.fogColor.b/255.0;
                frgb[3]=1.0;
                fe = 1;

                glEnable(GL_FOG);
                glFogf(GL_FOG_START,   _scene._si.fogNear);
                glFogf(GL_FOG_END,    _scene._si.fogFar);
                glFogfv(GL_FOG_COLOR,  frgb);
                glFogf(GL_FOG_DENSITY, _scene._si.fogDens);
            }
            

            if(_compviewmode & C_GRAPH)
            {
                LocalDrawBspTreeGraph(_compiler, _leafNo, pV);
            }
            else if(_compviewmode & C_LBYL)
            {
                LocalDrawBspTreeGraph(_compiler, _leafNo, pV);
                _compiler.Render(pV, _compviewmode);
            }
            else
            {
                _compiler.Render(pV, REND_BSP);
                LocalDrawReadyBSP(_compiler,_scene, pV);
                _compiler.Render(pV, _compviewmode);
                
            }
            if(fe)
            {
                glDisable(GL_FOG);
            }
        }
		else // drae scene
        {
            if(_scene.Terrain().Exist())
                _scene.Terrain().Render(1);
            Draw3DRawScene(pV);
        }
        DrawDebugPointsAndPolys();
    }while(0);
    glPopMatrix();
}

//-----------------------------------------------------------------------------
void    CZ_ed2Doc::DrawDebugPointsAndPolys()
{
    Ta.Disable();

    if(_pSelVertexes.size())
    {
		set<V3*>	uniques;
		glPointSize(8.f);
		glColor4ubv(CLR_WHITE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

        glBegin(GL_POINTS);
        FOREACH(vvector<Vtx2*>,_pSelVertexes,ppv)
        {
			if(uniques.end() == uniques.find(&(*ppv)->_xyz))
			{
				glVertex3f((*ppv)->_xyz.x,
					       (*ppv)->_xyz.y,
						   (*ppv)->_xyz.z);
				uniques.insert(&(*ppv)->_xyz);
			}
        }
        glEnd();
        glPointSize(1.f);  
    }

#ifdef _DEBUG
    glPointSize(2.f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBegin(GL_POINTS);

    glColor4ub(255,255,255,255);
    if(_testPoints.size())
    {
        FOREACH(vvector<V3>, _testPoints, pPt)
        {
            glVertex3f(pPt->x, pPt->y, pPt->z);
        }
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glPointSize(1.f);

#endif //

    if(_errPoint != V0) //V3
    {
        if(_debugPolys.size())
        {
	        glDisable(GL_CULL_FACE);
	        glEnable(GL_BLEND);
	        glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR); 

	        Ta.Disable();
	        glColor4ub(255,255,0,64);
	        FOREACH(vvector<Poly>,_debugPolys,pp)
	        {
		        Poly& rp = *pp;

		        glBegin(GL_POLYGON);
		        PVertex vb = rp._vtci.begin();
		        PVertex ve = rp._vtci.end();
		        for(;vb!=ve;vb++)
		        {
			        Vtx2& vtci = *vb;

			        glVertex3f(vtci._xyz.x,vtci._xyz.y,vtci._xyz.z);
		        }
		        glEnd();

	        }
            glDisable(GL_BLEND);
            SetFaceMode();
        }
        DrawErrorPoint();
    }

   // glEnable(GL_DEPTH_TEST);
   Ta.Restore();
}
//---------------------------------------------------------------------------------------
void  CZ_ed2Doc::Draw3DRawScene(z_ed3View* pV)
{
    if(_clearing)
        return;
    _maxMirrorsDraws=0;
    DrawTextured(pV);

    if(_selBox._brushflags & BRSH_SELVIS)
    {
        if(_selBoxByTouch)
            glColor4ubv(CLR_SELBOX_TCH);
        else
            glColor4ubv(CLR_SELBOX);

        Ta.Disable();
        ::D32LocalDrawWireBrush(0, &_selBox, GL_LINE_LOOP, FALSE, FALSE,CLR_SELBOX);
        Ta.Restore();
    }

    if(_selmode  == SEL_BRUSH)
    {
        DrawSelBrushes(pV);
        DrawMotionsPaths();
    }
    else if(_selmode==SEL_FACE || 
            _selmode==SEL_VERTEX || 
            _selmode==SEL_EDGE)
    {
        DrawSelFaces(pV);
    }
}

//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::DrawCompiledStates(z_ed3View* pV)
{
    _compiler.Render(pV,0xFF);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawTextured(z_ed3View *pV)
{
    if(_clearing || _maxMirrorsDraws > 2)   return;

    ++_maxMirrorsDraws;
    vvector<Brush*> lastones(256);
    BOOL            selOn =   (SEL_BRUSH  == _selmode || SEL_FACE   == _selmode || SEL_VERTEX == _selmode);
    
	// cut are blended so draw them at the end
    if(HasForms())    
    {
		if(GLight)
        {
            glEnable(GL_LIGHTING);
        }
        //
        // polys from brushes we draw at the very end
        //
	    vvector<Poly*> backDraw;
        vvector<Poly*> transPolys;
        vvector<Poly*> mirrors;
        vvector<Poly*> masked;         // do the mask texture trick
        vvector<Poly*> guround;       // do the mask texture trick

        if(GetForms()->size())
        {
            FOREACH(PBrushes, (*GetForms()), b)
            {
                Brush& rb = (*b)->GetResult();

                if(_curgroup >= 0 && rb._group != _curgroup)
                    continue;	

                if(rb._brushflags & BRSH_SOLID)
                {
                    if(_cam.IsCulled(&rb._box._min,&rb._box._max))
                        continue;
                    DrawSolidBrush(rb , backDraw, transPolys, mirrors, masked, guround);
                }
                else if(selOn)
                {
                    lastones << (*b); 
                }
            }
        }

        if(GLight)
            glDisable(GL_LIGHTING);

        if(selOn)
        {
            Ta.Disable();
            FOREACH(vvector<Brush*>, lastones, ppb)
            {
                if((*ppb)->IsProjector())
                {
                    DrawProjectorBrush(*(*ppb));
                }
                else if((*ppb)->IsZone())
                {
                    GLDrawBox((*ppb)->_box, 1, CLR_ZONE);
                }
                else if((*ppb)->_brushflags & BRSH_BIGTERRAIN)
                {
                    GLDrawBox((*ppb)->_box, 1, CLR_TERRAIN);
                }
                else
                {
                    DrawCutBrush(*(*ppb));
                }
            }
            Ta.Restore();
        }
        if(0==pV)
        {
            mirrors.clear();
            _DrawCamera();
        }
        DrawSpecialPolygons(pV, 0,backDraw, transPolys, mirrors, masked, guround);
    }
    DrawItems();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawItems()        
{
    FOREACH(vvector<SceItem*>, (*GetItems()), ppi)
    {
        SceItem* pI = *ppi;
        if(_curgroup >= 0 && pI->_group != _curgroup)
            continue;
        if(pI->IsSelected())
            continue;
        if(_cam.IsCulled(&pI->_t))
            continue;
        pI->GlDraw(0);
    }
    int cnt;
    SceItem** it = DOC()->GetSelItems(cnt);
    for(int i=0;i<cnt;i++,it++)
    {
        if(_curgroup >= 0 && (*it)->_group != _curgroup)
            continue;
        if(_cam.IsCulled(&(*it)->_t))
            continue;
       (*it)->GlDrawSel(0);
    }
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawProjectorBrush(Brush& rb)
{
 
    PPoly pb = rb._polys.begin();
    PPoly pe = rb._polys.end();
    int   index = 0;
    Poly* pprj = 0;
    V3    pcenter;  

    BYTE* pcrl = S_SelectColor(rb, rb.IsSelected());
    
    for(;pb!=pe;pb++,index++)
    {
        Poly& pl = *pb;

        if(2 == index )
            pprj = &(*pb);

        if(SEL_FACE   == _selmode)
        {
            if(pl.IsSelected())
                glColor4ubv(CLR_WHITE);
            else
                glColor4ubv(pcrl);
        }

        glBegin(GL_LINE_LOOP);
        PVertex vb = pl._vtci.begin();
        PVertex ve = pl._vtci.end();
        for(;vb!=ve;vb++)
        {

            Vtx2& vtci = *vb;
			glTexCoord2f(vtci._uv[GUtex].u,vtci._uv[GUtex].v);
            glVertex3f(vtci._xyz.x,vtci._xyz.y,vtci._xyz.z);
            if(2==index)
                pcenter+=vtci._xyz;
        }
        glEnd();
    }

    pcenter/=4.0f;
    pcenter += pprj->_n * 128.0;
    glBegin(GL_LINES);
    PVertex vb = pprj->_vtci.begin();
    PVertex ve = pprj->_vtci.end();
    for(;vb!=ve;vb++)
    {
        glVertex3f(pcenter.x, pcenter.y, pcenter.z);
        glVertex3f(vb->_xyz.x, vb->_xyz.y,vb->_xyz.z);
    }
    glEnd();

        
    
    int obm = _vmodeback;
    _vmodeback =1;
    SetFaceMode();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
    
    Ta.Enable();
    Ta.Bind(pprj->GetHtex(0),0);
    glBegin(GL_POLYGON);
    vb = pprj->_vtci.begin();
    ve = pprj->_vtci.end();
    for(;vb!=ve;vb++)
    {
        Vtx2& vtci = *vb;

		glTexCoord2f(vtci._uv[GUtex].u,vtci._uv[GUtex].v);
        glVertex3f(vtci._xyz.x,vtci._xyz.y,vtci._xyz.z);

        if(2==index)
            pcenter+=vtci._xyz;
    }
    glEnd();
    Ta.Restore();
    glDisable(GL_BLEND);

    _vmodeback =  obm;
    SetFaceMode();
    glColor4ubv(CLR_WHITE);
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawCutBrush(Brush& rb)
{
    Ta.Disable();
    if(BShowCb)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
    }

    // reversed brush slighty different in color
    BYTE* pclr=S_SelectColor(rb, rb.IsSelected());

    PPoly pb = rb._polys.begin();
    PPoly pe = rb._polys.end();

    for(;pb!=pe;pb++)
    {
        Poly& pl = *pb;
        if(BShowCb)
            glBegin(GL_POLYGON);
        else
            glBegin(GL_LINE_LOOP);


            if(pl.IsSelected())
                glColor4ubv(CLR_WHITE);
            else
                glColor4ubv(pclr);

            PVertex vb = pl._vtci.begin();
            PVertex ve = pl._vtci.end();
            for(;vb!=ve;vb++)
            {
                Vtx2& vtci = *vb;
				//glTexCoord2f(vtci._uv[GUtex].u,vtci._uv[GUtex].v);
                glVertex3fv((REAL*)vtci._xyz);
            }
        glEnd();
    }
    if(BShowCb)
        glDisable(GL_BLEND);

    Ta.Restore();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawSolidBrush(Brush& rb, 
                               vvector<Poly*>& backDraw,
                               vvector<Poly*>& transPolys,
                               vvector<Poly*>& mirrors,
                               vvector<Poly*>& masked,
                               vvector<Poly*>& guround,
                               BOOL usesel)
{   
    DWORD genTex  = 0;
    glColor4ubv(CLR_WHITE);
    PPoly pb = rb._polys.begin();
    PPoly pe = rb._polys.end();
    for(;pb!=pe;pb++)
    {
        Poly& pl = *pb;

        //if(pl.IsSelected())
        //    continue;


        if(pl._polyflags & POLY_HIDDEN)
        {
            pl._colorD.a = 16;
            transPolys.push_back((Poly*)&pl);
            continue;
        }

        if(pl._polyprops & FACE_MIRROR)
        {
            mirrors.push_back((Poly*)&pl);
            continue;
        }
        
		if(pl._polyprops & FACE_SHOWBACK &&
         !(pl._polyprops & FACE_BLACKMASK))
        {
			backDraw.push_back((Poly*)&pl);
            continue;
        }

        if( pl._polyprops & FACE_TRANSPARENT )
        {
            if(pl._colorD.a==0 && BShowTrPolys)
            {
                transPolys.push_back((Poly*)&pl);
            }
            else
            {
                transPolys.push_back((Poly*)&pl);
            }
            continue;
        }


        if(pl._polyprops & FACE_BLACKMASK)
        {
            masked.push_back((Poly*)&pl);
            continue;
        }

        if(pl._polyprops & FACE_GSHADE)
        {
            guround.push_back((Poly*)&pl);
            continue;
        }

        if(usesel)
        {
            if(pl.IsSelected())
                glColor4ubv(CLR_WHITE);
            else
                glColor4ubv(CLR_NORMAL);
        }
        else
            glColor4ubv((BYTE*)pl._colorD);

        if(pl._polyprops & FACE_FULLBRIGHT || pl._polyprops & FACE_NOLIGTMAP)
        {
            __noop;
        }
        else if(GLight)        
        {
            ApplyCloserLights(pl);
        }
        DrawPolygon(pl, _vmodefire);
    }
}

//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::ApplyCloserLights(const Poly& poly)
{
    CLightBulb* pL;
    REAL position[4];
    REAL dl[4];
    REAL sl[4];
    REAL e[4] = {poly._colorE.r/255.0, poly._colorE.g/255.0, poly._colorE.b/255.0, 1};
    REAL d[4] = {poly._colorD.r/255.0, poly._colorD.g/255.0, poly._colorD.b/255.0, 1};
    REAL s[4] = {poly._colorS.r/255.0, poly._colorS.g/255.0, poly._colorS.b/255.0, 1};
    
    glMaterialfv(_vmodeback ? GL_FRONT_AND_BACK  : GL_FRONT, GL_DIFFUSE, d);
    glMaterialfv(_vmodeback ? GL_FRONT_AND_BACK  : GL_FRONT, GL_SPECULAR, s);
    glMaterialfv(_vmodeback ? GL_FRONT_AND_BACK  : GL_FRONT, GL_EMISSION, e);
    glMateriali(_vmodeback  ? GL_FRONT_AND_BACK  : GL_FRONT, GL_SHININESS, poly._shiness);

    int nLightCount = 0;
    int maxLights = _maxLights;

    for(int i=0; i < _maxLights; i++)
        glDisable(GL_LIGHT0+i);

    if(_camSpot)
    {
        ReposCamLight(BIT_AMBIENT|BIT_DIFFUSE|GL_SPECULAR, nLightCount, ZGREY);
        ++nLightCount;
        --maxLights;
    }
    else
    {
        Box b;
        FOREACH(vvector<SceItem*>, (*GetItems()), ppi)
        {

            SceItem* pI = *ppi;
            if(_curgroup >= 0 && pI->_group != _curgroup)  continue;

            b._max = pI->_t+pI->_s;
            b._min = pI->_t-pI->_s;


            if(pI->_item == ITM_LIGTBULB)
            {
                if(vdist(_cam._pos, pI->_t) > 12800)
                {
                    continue;
                }
                pL = (CLightBulb*)pI;
                if(((Poly&)poly)._box.IsTouchesSphere(pL->_t, pL->_radius))
                {
                    position[0]=pL->_t.x;
                    position[1]=pL->_t.y; 
                    position[2]=pL->_t.z; 
                    position[3]=1;

                    dl[0] = (pL->_colorD.r * pL->_lmIntensity)/25500.0; if(dl[0]>1)dl[0]=1;
                    dl[1] = (pL->_colorD.g * pL->_lmIntensity)/25500.0; if(dl[1]>1)dl[1]=1;
                    dl[2] = (pL->_colorD.b * pL->_lmIntensity)/25500.0; if(dl[2]>1)dl[2]=1;
                    dl[3] = 1;

                    sl[0] = (pL->_colorS.r * pL->_lmIntensity)/25500.0; if(sl[0]>1)sl[0]=1;
                    sl[1] = (pL->_colorS.g * pL->_lmIntensity)/25500.0; if(sl[1]>1)sl[1]=1;
                    sl[2] = (pL->_colorS.b * pL->_lmIntensity)/25500.0; if(sl[2]>1)sl[2]=1;
                    sl[3] = 1;

                    glLightf(GL_LIGHT0+nLightCount,  GL_LINEAR_ATTENUATION, 1.0/(pL->_radius*3));
                
                    if(pL->_specAngle)
                    {
                        V3 vd = -VZ;
                        pL->_m.v3rotate(vd);
					    glLightfv(GL_LIGHT0+nLightCount, GL_SPOT_DIRECTION, vd);
					    glLightf(GL_LIGHT0+nLightCount,  GL_SPOT_CUTOFF, pL->_specAngle/2.0);
				    }
				    else 
                    {
                        glLightf(GL_LIGHT0+nLightCount, GL_SPOT_CUTOFF, 180.0);
                    }

                    glLightfv(GL_LIGHT0+nLightCount, GL_DIFFUSE,  dl);
                    glLightfv(GL_LIGHT0+nLightCount, GL_SPECULAR, sl);
                    glLightfv(GL_LIGHT0+nLightCount, GL_POSITION, (float *) position);
                    glEnable(GL_LIGHT0+nLightCount);
                    ++nLightCount;

                    if(nLightCount > maxLights)
                    {
                        break;
                    }
                }
            }
        }
    }
}



static void __SetFogColor(V3& v, CLR& color)
{
	float z;	/* distance of vertex from edge of fog volume */
	float f;	/* fog factor */
    V3&  cp = DOC()->_cam._pos;
	z = 0.0 - v.y;
	f = (10.0 - z) / (10.0 - 0.0);	/* linear fog: f = (end - z)/(end - start) */
	glColor4f(0.6, 0.2, 0.0, f);

}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawSpecialPolygons(z_ed3View *pV,
                                    DWORD lmaps, 
                                    vvector<Poly*>& backDraw,
                                    vvector<Poly*>& transPolys,
                                    vvector<Poly*>& mirrors,
                                    vvector<Poly*>& masked,
                                    vvector<Poly*>& guround)
{
    Poly* pPoly;

    if(transPolys.size())
    {
        glColor4ubv(CLR_WHITE);
        if(transPolys.size())
        {
            
            if(_compviewmode & C_WHITE || _compviewmode & C_BYFAR)
            {
                FOREACH(vvector<Poly*>, transPolys, ppPoly)
                {
                     pPoly = *ppPoly;
                    DrawPolyVertexes(GL_POLYGON, *pPoly, 0,  0);
                }
            }
            else
            {
                glDisable(GL_CULL_FACE);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA,GL_ONE);
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER,0);
		        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                FOREACH(vvector<Poly*>, transPolys, ppPoly)
                {
                    pPoly = *ppPoly;

                    glColor4ubv(pPoly->_colorD);
			        Ta.Bind(pPoly->GetHtex(GUtex), pPoly->Apply(GUtex));
                    DrawPolyVertexes(GL_POLYGON, *pPoly, GUtex, lmaps ? VX_COLOR : 0);
                }
                //glDisable(GL_ALPHA);		  
                glDisable(GL_BLEND);	
                glDisable(GL_ALPHA_TEST);
		        SetFaceMode();
            }

        }

    }

    if(masked.size())
    {
        // blend
		glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER,0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        
        if(masked.size())
        {
            FOREACH(vvector<Poly*>, masked, ppPoly)
            {
                pPoly = *ppPoly;
                //if(pPoly->_colorD.a==0)
                //    continue;
			    Ta.Bind(pPoly->GetHtex(GUtex), pPoly->Apply(GUtex));
                DrawPolyVertexes(GL_POLYGON, *pPoly, GUtex);
            }
        }
        //glDisable(GL_ALPHA);		  
        glDisable(GL_BLEND);	
        glDisable(GL_ALPHA_TEST);
		SetFaceMode();
    }
    

    if(guround.size())
    {
        glColor4ubv(CLR_WHITE);
        FOREACH(vvector<Poly*>, guround, ppPoly)
        {
            pPoly = *ppPoly;
			Ta.Bind(pPoly->GetHtex(GUtex), pPoly->Apply(GUtex));
            DrawPolyVertexes(GL_POLYGON, *pPoly, GUtex, lmaps ? VX_COLOR : 0);
        }
    }

	if(backDraw.size())
	{
        // blend
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        FOREACH(vvector<Poly*>, backDraw, ppPoly)
        {
            pPoly = *ppPoly;

            if(pPoly->_colorD.a==0)
                continue;

            if(pPoly->_polyprops & FACE_TRANSPARENT)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA,GL_ONE);
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER,0);

            }

            if(pPoly->_polyprops & FACE_BLACKMASK)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER,0);

            }

            glColor4ubv((BYTE*)pPoly->_colorD);

			Ta.Bind(pPoly->GetHtex(GUtex), pPoly->Apply(GUtex));
            DrawPolyVertexes(GL_POLYGON, *pPoly, GUtex);

            if(pPoly->_polyprops & FACE_TRANSPARENT || 
               pPoly->_polyprops & FACE_BLACKMASK)
            {
                //glDisable(GL_ALPHA);		  
                glDisable(GL_ALPHA_TEST);
                glDisable(GL_BLEND);
            }
        }
		SetFaceMode();
	}


    if(mirrors.size()) 
    {
        /*
        M4  mr = _cam.GetViewMtx(0);
        M4  rf;
        M4  mt;
        
        glClear (GL_STENCIL_BUFFER_BIT);

        glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE); 
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 0x1, 1); 
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

        FOREACH(vvector<Poly*>, mirrors, ppPoly)
        {
            pPoly = *ppPoly;
            DrawPolyVertexes(GL_POLYGON, *pPoly, 0, (DWORD)-1); 
        }

        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
        glDepthMask(GL_TRUE); 
        glStencilFunc(GL_EQUAL,  0x1, 0xFFFFFFFF); 
        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

        
        FOREACH(vvector<Poly*>, mirrors, ppPoly)
        {
            pPoly = *ppPoly;
            
            rf.reflect(pPoly->_n, -pPoly->_c);
            mt = mr * rf;
            
            glPushMatrix();
                glLoadMatrixf((REAL*)&mt);
                DrawTextured(0);
            glPopMatrix();
        }
        
        glDisable(GL_STENCIL_TEST);
*/


      
        
            glDisable(GL_DEPTH_TEST);
            TexRef::GlDecalMode(1);
            Ta.Bind(_dumptex, 0);
            FOREACH(vvector<Poly*>, mirrors, ppPoly)
            {
                pPoly = *ppPoly;
                glColor4ubv((pPoly)->_colorD);
                pPoly = *ppPoly;
                DrawPolyVertexes(GL_POLYGON, *pPoly, 0);
            }
            glEnable(GL_DEPTH_TEST);
            TexRef::GlDecalMode(0);
            SetFaceMode();
            //DrawTextured(0);
        
           
    }
}

//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// this draw them as wireframe in the 3D vbiew space
void CZ_ed2Doc::DrawSelBrushes(z_ed3View *pV)
{
    int     nCount;
    Brush** pBS         = DOC()->GetSelBrushes(nCount);

    glDisable(GL_DEPTH_TEST);
 
    for(int i=0; i < nCount; i++, pBS++)
    {
        //
        // wireframe shows primary primitive as defualt 
        //
        Brush* pbr = (*pBS);            //->GetResult();

        //
        // if bounding box is enabled shoes it with all cuts
        //
        if(GShowCuts)
            pbr = &(*pBS)->GetResult();

        if(_cam.IsCulled(&pbr->_box._min, &pbr->_box._max))
            continue;

        BYTE* pclr;
	   if(_alignment)
       {
            if(i==nCount-1)
            {
                if (i==nCount-1)
                {
                    glColor4ubv(CLR_SELECT);
                    pclr = CLR_SELECT;
                }
                else
                {
					glColor4ubv(CLR_WHITE);
                    pclr = CLR_WHITE;
                }
            }
        }
        else
        {
            glColor4ubv(CLR_SELECT);
            pclr = CLR_SELECT;
        }
        ::D32LocalDrawWireBrush(0, pbr, GL_LINE_LOOP, FALSE, FALSE, pclr);
    }
    glEnable(GL_DEPTH_TEST);

 }

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawSelFaces(z_ed3View *pV)
{
    int     polys,i;
    Poly**  pPI = DOC()->GetSelPolys(polys);
    Poly**  pPS = pPI;

    if(_selmode==SEL_VERTEX)
        glColor4ubv(CLR_SELECTDIMED);
    else
        glColor4ubv(CLR_SELECT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
 
    if(GSelTexFace)
    {
        TexRef::GlHalloMode(1);
        for( i=0; i < polys; i++, pPS++)
        {
            Poly& pT = (**pPS);

            if(_cam.IsCulled(&pT._box._min,&pT._box._max))
                continue;
            Ta.Bind(_selTex, 0);
            DrawPolyVertexes(GL_POLYGON, pT, GUtex);

        }
        TexRef::GlHalloMode(0);
    }

    Ta.Disable();
    pPS = pPI;
    for( i=0; i < polys; i++, pPS++)
    {
        Poly& pT = (**pPS);

        if(_cam.IsCulled(&pT._box._min,&pT._box._max))
            continue;

        DrawPolyVertexes(GL_LINE_LOOP, pT, GUtex);

        // show surfaces normals
		V3 c = pT._box.GetCenter();
		V3 e = c+pT._n*8.0;
		glBegin(GL_LINES);
			glVertex3f(c.x,c.y,c.z);
			glVertex3f(e.x,e.y,e.z);
		glEnd();

    }

    //
    // selection box
    //
    if(_selBox._brushflags & BRSH_SELVIS)
    {
        BYTE* pclr;
        if(_selBoxByTouch)
            {
                glColor4ubv(CLR_SELBOX_TCH);
                pclr= CLR_SELBOX_TCH;
            }
        else
            {
                glColor4ubv(CLR_SELBOX);
                pclr= CLR_SELBOX_TCH;
            }
        ::D32LocalDrawWireBrush(0, &_selBox, GL_LINE_LOOP, 0,0,pclr);
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    SetFaceMode();
    Ta.Restore();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawAxis(V3& org, REAL ex)
{
   Ta.Disable();
    glPushMatrix();

    glTranslatef(org.x, org.y, org.z);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); 

    glBegin(GL_LINES);
        glColor4ub(192,96,96,128);
            glVertex3f(0,0,0);
            glVertex3f(ex,.6f,0.f);

        glColor4ub(96,192,96,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,ex,0.f);

        glColor4ub(96,96,192,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,.1f,ex);

        glColor4ub(128,0,0,128);
            glVertex3f(0,0,0);
            glVertex3f(-ex,.6f,0.f);

        glColor4ub(0,128,0,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,-ex,0.f);

        glColor4ub(0,0,128,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,0.f,-ex);
    glEnd();

    glPopMatrix();
   Ta.Restore();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawGridLines()
{
    REAL r;
    if(_compviewmode  & (C_FULL|C_PORT|C_LEAFS|C_LBYL|C_LMAP|C_GRAPH|C_ZONES))
        return;

    glColor4ub(53,53,53,64);

    REAL gs = _gridStep*10;

    Ta.Disable();
    glBegin(GL_LINES);
    for( r = -_gridSize/2; r< _gridSize/2; r+=gs)
    {
        glVertex3f(r,0,-_gridSize/2);
        glVertex3f(r,0,_gridSize/2);
    }

    for(r = -_gridSize/2; r< _gridSize/2; r+=gs)
    {
        glVertex3f(-_gridSize/2,0,r);
        glVertex3f(_gridSize/2,0,r);
    }

    glEnd();

    glEnable(GL_BLEND);
    glDepthMask(0);
    glDisable(GL_DEPTH_TEST);

    
    V3 vnd = _ip[0];//
    
    if(_ip[1].len2() > 4)
        vnd = _ip[1];
    else
        vnd += _ip[0]*8;

    glBegin(GL_LINES);
        glColor4ub(100,100,0,255);
        glVertex3f(_ip[0].x,_ip[0].y,_ip[0].z);
        glColor4ub(64,64,0,255);
	    glVertex3f(vnd.x,vnd.y,vnd.z);
    glEnd();    
    glPointSize(1.f);


    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
    glDisable(GL_BLEND);
    Ta.Restore();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::ReposCamLight(DWORD lightbits, int avaliCnt, REAL offset, CLR difLight)
{

	V3 pl = _cam._pos;

    pl += _cam._right   * offset;
    pl += _cam._up     * offset;

    float position[4] = {pl.x,pl.y,pl.z,1};
    
    REAL a[4]={ _scene._si.defLumin.r/255.0, 
                _scene._si.defLumin.g/255.0, 
                _scene._si.defLumin.b/255.0, 1.0};

    REAL d[4]={difLight.r, difLight.g, difLight.b, 1.0};
    REAL s[4]={1.0, 1.0, 1.0, 1.0};
    REAL linearAttenuation       = .00001;
    float cut_off                = 360.0;

    if(lightbits | BIT_AMBIENT)
	    glLightfv(GL_LIGHT0+avaliCnt, GL_AMBIENT,  a);
    
    if(lightbits | BIT_DIFFUSE)
	    glLightfv(GL_LIGHT0+avaliCnt, GL_DIFFUSE,  d);

    if(lightbits | BIT_SPECULAR)
	    glLightfv(GL_LIGHT0+avaliCnt, GL_SPECULAR, s);

	glLightfv(GL_LIGHT0+avaliCnt, GL_POSITION, (float *) position);
    glEnable(GL_LIGHT0+avaliCnt);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawMotionsPaths()
{
	Ta.Disable();

    glDisable(GL_BLEND);
    glColor4ubv(CLR_MOTION);

    if(_pSelBrushes.size() == 1)
    {
        if(_pSelBrushes[0]->_pMotion)
        {
            _pSelBrushes[0]->_pMotion->GlDraw();
        }
    }
    Ta.Restore();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::DrawErrorPoint()
{

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(FALSE);

    glColor4ub(255,255,255,255);
    glPointSize(8.f);

    glBegin(GL_POINTS);
        glVertex3f(_errPoint.x,_errPoint.y,_errPoint.z);
    glEnd();

    glPointSize(1.f);

    glLineWidth(4.0f) ;

    DrawAxis(_errPoint, 32.0);

    glLineWidth(1.0f) ;

    glDepthMask(TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::Draw2DScene(CZ_ed2View *pV)
{
    if(_clearing)
        return;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0f, 1.0);
    glPushMatrix();
    {
        switch(pV->_vt)
        {
            case 'x':
                glRotatef(pV->_viewrot,0,1,0);
                break;
            case 'y':
                glRotatef(pV->_viewrot,1,0,0);
                break;
            case 'z':
                glRotatef(pV->_viewrot,0,0,1);
                break;
        }

        
        Draw2DBrushes(pV);
        
		if(SEL_ITEMS == _selmode )
        {
            Draw2dItems(pV);
        }



        if(_bDrawCamRay && b_showgrid)
        {
            DrawCameraRay(pV);
            DrawCamFrustrum(pV);
        }
    }
    pV->GlAfterPaint();
    glPopMatrix();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::Draw2dItems(CZ_ed2View *pV)
{
    TexRef::GlDecalMode(1);
    FOREACH(vvector<SceItem*>, (*GetItems()), ppi)
    {
        SceItem* pI = *ppi;
        if(_curgroup >= 0 && pI->_group != _curgroup)
               continue;
        if(pI->IsSelected())
            continue;
        pI->GlDraw2D(pV);
    }
    TexRef::GlDecalMode(0);

    FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
    {
        SceItem*  pItem  = *ppItem;
        if(_curgroup >= 0 && pItem->_group != _curgroup)
               continue;
		pItem->GlDraw2D(pV);
    }
}

//-----------------------------------------------------------------------------
void CZ_ed2Doc::DrawCameraRay(CZ_ed2View *pV)
{
	glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);  

    glBegin(GL_LINES);

	    glColor4ubv(CLR_CAMSTART);
	    glVertex3f(_camray[0].x,_camray[0].y,_camray[0].z);
	    glColor4ubv(CLR_CAMEND);
	    glVertex3f(_camray[1].x,_camray[1].y,_camray[1].z);

    glEnd();
	glDisable(GL_BLEND);

}

//-----------------------------------------------------------------------------
void CZ_ed2Doc::_DrawCamera()
{
    /*
    GLUquadricObj*  GluObj;
    GluObj = gluNewQuadric();
    gluQuadricNormals(GluObj ,  GLU_SMOOTH);
    gluQuadricOrientation(GluObj, GLU_INSIDE);
    gluQuadricTexture(GluObj, GL_TRUE);
    gluQuadricDrawStyle(GluObj,GLU_FILL);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glEnable(GL_LIGHTING);
    ReposCamLight(BIT_AMBIENT|BIT_DIFFUSE|BIT_SPECULAR, 0, 16.0, ZWHITE);

    V3 v =_cam.Dir2Euler(0);
    
    glTranslatef(_cam._pos.x,_cam._pos.y,_cam._pos.z);
    glRotatef(R2G(v.y),0,1,0);
    glRotatef(-R2G(v.x),1,0,0);
    glRotatef(45,0,0,1);

    glColor3f(255,0,0);
    Ta.Bind(_selTex, 0);
    gluCylinder(GluObj, 4.0, 28.0, 24, 4, 8);

    

	if(!GLight)
    {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
    }

    glPopMatrix();

    gluDeleteQuadric(GluObj);	
    */


}


//-----------------------------------------------------------------------------
void CZ_ed2Doc::DrawCamFrustrum(CZ_ed2View *pV)
{
    int i;
    if(pV->_zoom > 1.f)
	{
        
		glColor4ubv(CLR_WHITE);
		glBegin(GL_LINES);

		for( i=0;i<5;i++)
		{
			glVertex3f(_frustDots[0].x,_frustDots[0].y,_frustDots[0].z);
			glVertex3f(_frustDots[i].x,_frustDots[i].y,_frustDots[i].z);
		}
		glEnd();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
		glColor4ubv(CLR_SCRSURFACE);
		glBegin(GL_QUADS);
		for( i=1;i<5;i++)
		{
			glVertex3f(_frustDots[i].x,_frustDots[i].y,_frustDots[i].z);
		}
		glEnd();
		glDisable(GL_BLEND);

		glPointSize(4.f);
		glColor4ubv(CLR_WHITE);
		glBegin(GL_POINTS);
		glVertex3f(_camray[2].x,_camray[2].y,_camray[2].z);
		glEnd();
		glPointSize(1.f);
	}
    else
    {
        glPushMatrix();
        {
            glColor4ubv(CLR_CAMFRUST);
            REAL sc = 16.f/pV->_zoom; 

            V3  vdir;
            V3  nfd[5];
            ::memcpy(nfd,_frustDots,sizeof(_frustDots));

            for( i=1;i<5;i++)
            {
                vdir = nfd[i] - nfd[0];
                vdir.norm();
                nfd[i] = nfd[0] + vdir *  sc;
            }

            glBegin(GL_LINES);
		    for(i=0;i<5;i++)
		    {
			    glVertex3f(nfd[0].x,nfd[0].y,nfd[0].z);
			    glVertex3f(nfd[i].x,nfd[i].y,nfd[i].z);
		    }
		    glEnd();
        }
        glPopMatrix();
    }
}

//-----------------------------------------------------------------------------
void    CZ_ed2Doc::GLDrawBox(Box& box, BOOL asWire, RGBA color)
{
    Ta.Disable();
    glColor4ubv(color);

    glBegin(GL_LINE_LOOP); 
        glVertex3f(box._min.x, box._min.y, box._min.z);    
        glVertex3f(box._max.x, box._min.y, box._min.z);
        glVertex3f(box._max.x, box._min.y, box._max.z);
        glVertex3f(box._min.x, box._min.y, box._max.z);
    glEnd();

    glBegin(GL_LINE_LOOP); 
        glVertex3f(box._min.x, box._max.y, box._min.z);    
        glVertex3f(box._max.x, box._max.y, box._min.z);
        glVertex3f(box._max.x, box._max.y, box._max.z);
        glVertex3f(box._min.x, box._max.y, box._max.z);
    glEnd();

    glBegin(GL_LINES); 
        glVertex3f(box._min.x, box._max.y, box._min.z);    
        glVertex3f(box._min.x, box._min.y, box._min.z);    

        glVertex3f(box._max.x, box._max.y, box._min.z);
        glVertex3f(box._max.x, box._min.y, box._min.z);

        glVertex3f(box._max.x, box._max.y, box._max.z);
        glVertex3f(box._max.x, box._min.y, box._max.z);

        glVertex3f(box._min.x, box._max.y, box._max.z);
        glVertex3f(box._min.x, box._min.y, box._max.z);
    glEnd(); 

    glColor4ubv(CLR_WHITE);

    Ta.Restore();
}


//-----------------------------------------------------------------------------
DWORD CZ_ed2Doc::SetFaceMode(BOOL resetAll)
{
    if(resetAll)
    {
        Ta.Reset();
        glColor4ubv(CLR_WHITE);
        glDisable(GL_FOG);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE); 
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
    }

    if(_vmodeback)
    {
        glDisable(GL_CULL_FACE);
        if(_vmodefire)
	        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
	        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_FRONT);
        if(_vmodefire)
		    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
	return GL_FRONT;
}


void CZ_ed2Doc::DrawPolygon(Poly& poly, BOOL wireMode)
{
   if(poly._n.x>=0.999999f)
        TRACE("");

    
    if(_vmodefire && AKEY(VK_CONTROL))
    {
        Ta.Disable();
        DrawPolyVertexes(GL_LINE_LOOP, poly, GUtex);
        Ta.Restore();
        return;
    }
    BOOL is = poly.IsSelected();
    Ta.Bind(poly.GetHtex(GUtex), poly.Apply(GUtex));
    DrawPolyVertexes(GL_POLYGON, poly, GUtex);
    
}


void CZ_ed2Doc::PrintCoords(RECT& wrect, FRECT& vrect, V3& pct, char vt, BOOL active, LPCTSTR cs, POINT* mt)
{
    if(_clearing || !b_showgrid)
        return;

    SelectTextCtx(wrect);

    switch(vt)
    {
        case 'x':
            PrintOut(10,2,"+Z");
            PrintOut(2,10,"+Y");
            //PrintOut(2,wrect.bottom-12,"SIDE", active ?  ZWHITE : ZRED);
            PrintOut(2,wrect.bottom-12,MKSTR("SIDE %.2fx%.2f %s",
                                       (vrect.right-vrect.left)/GMeters,(vrect.bottom-vrect.top)/GMeters,
                                       GMeters==1 ? "cm" : ((GMeters==100 ? "m":"ft"))),
                                       active ? ZWHITE : ZRED); 
            //V
		    PrintOut(wrect.right/2+2, wrect.bottom-12,  MKSTR("+Y %.2f",vrect.bottom/GMeters));
		    PrintOut(wrect.right/2+2, 12,               MKSTR("-Y %.2f",vrect.top/GMeters));

            //H
            PrintOut(0,             wrect.bottom/2+4,MKSTR("-Z %.2f",vrect.left/GMeters));
            PrintOut(wrect.right-52,wrect.bottom/2+4,MKSTR("+Z %.2f",vrect.right/GMeters));

            
            if(cs && *cs)
            {
                PrintOut(mt->x,mt->y, cs, ZGREEN);
            }

            break;
        case 'y':
            PrintOut(10,2,"+X");
            PrintOut(0,10,"-Z");
            //PrintOut(2,wrect.bottom-12,"TOP",active ? ZWHITE : ZGREEN);
            PrintOut(2,wrect.bottom-12,MKSTR("TOP %.2fx%.2f %s",
                                       (vrect.right-vrect.left)/GMeters,(vrect.bottom-vrect.top)/GMeters,
                                       GMeters==1 ? "cm" : ((GMeters==100 ? "m":"ft"))),
                                       active ? ZWHITE : ZGREEN); 
            //V
		    PrintOut(wrect.right/2+2,   wrect.bottom-12,MKSTR("-Z %.2f",-vrect.bottom/GMeters));
		    PrintOut(wrect.right/2+2,   12,             MKSTR("+Z %.2f",-vrect.top/GMeters));

            //H
            PrintOut(0,             wrect.bottom/2+4,MKSTR("-X %.2f",vrect.left/GMeters));
            PrintOut(wrect.right-52,wrect.bottom/2+4,MKSTR("+X %.2f",vrect.right/GMeters));

            if(cs && *cs)
            {
                PrintOut(mt->x,mt->y, cs, ZGREEN);
            }

            break;
        case 'z':
            PrintOut(10,2,"+X");
            PrintOut(2,10,"+Y");
            //PrintOut(2,wrect.bottom-12,"FRONT",active ? ZWHITE :ZBLUE);
            PrintOut(2,wrect.bottom-12,MKSTR("FRONT %.2fx%.2f %s",
                                       (vrect.right-vrect.left)/GMeters,(vrect.bottom-vrect.top)/GMeters,
                                       GMeters==1 ? "cm" : ((GMeters==100 ? "m":"ft"))),
                                       active ? ZWHITE :ZBLUE); 
            //V
		    PrintOut(wrect.right/2,wrect.bottom-12,MKSTR("+Y %.2f",vrect.bottom/GMeters));
		    PrintOut(wrect.right/2,             12,MKSTR("-Y %.2f",vrect.top/GMeters));

            //H
            PrintOut(0,             wrect.bottom/2+4,MKSTR("-X %.2f",vrect.left/GMeters));
            PrintOut(wrect.right-52,wrect.bottom/2+4,MKSTR("+X %.2f",vrect.right/GMeters));
            
            if(cs && *cs)
            {
                PrintOut(mt->x,mt->y, cs, ZGREEN);
            }
            
            break;
    }

    if(pct.len2())
        PrintOut(wrect.right/2+70, wrect.top, MKSTR("X: %003.2f Y: %003.2f Z: %003.2f",
                                         pct.x,pct.y,pct.z),ZWHITE);

    DeSelectTextCtx();
}


void    CZ_ed2Doc::SelectTextCtx(RECT& wrect)
{
    glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(wrect.left, wrect.right, wrect.top , wrect.bottom, -32768, 32768);
        glColor3ub( 255,255,0);
    glMatrixMode(GL_MODELVIEW);
    
}

void    CZ_ed2Doc::DeSelectTextCtx()
{
    glPopMatrix();
}

void CZ_ed2Doc::PrintOut(V3& v, char vt, const char* text, CLR& clr)
{ 

    glEnable(GL_BLEND);
    glColor3ub(clr.r,clr.g,clr.b);
    glRasterPos3f(v.x,v.y,v.z);
    glPushAttrib(GL_LIST_BIT); 
        glListBase( CMainFrame::_font1-32 );
        glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );
    glPopAttrib();
    glDisable(GL_BLEND);
}

void CZ_ed2Doc::PrintOut(int x, int y, const char* text, CLR& clr)
{ 
	glColor3ub(clr.r,clr.g,clr.b);
    glRasterPos2i( x, y );
    glPushAttrib(GL_LIST_BIT); 
    glListBase( CMainFrame::_font1-32 );
    glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );
    glPopAttrib();
}

static V3 Dir2Euler(const V3& dir, const V3& up)
{
	V3 euler;
	REAL dy2 = (REAL)sqrt(1.0-dir.y*dir.y);
	euler.x = (REAL)Sinr(dir.y);
	euler.z = (REAL)Cosr(up.y  / dy2);
	euler.y = (REAL)Sinr(dir.x / dy2);	  // az
	ROLLPI(euler.y);
	return euler;
}


//MERGE//>
void    CZ_ed2Doc::DrawOrtho(const V3& pos, const V3& euler, REAL len, REAL clratn)
{
    int i;
    Ta.Disable();
    glPushMatrix();
    
    glTranslatef(pos.x, pos.y, pos.z);

    if(euler.y)glRotatef(R2G(euler.y), 0, 1, 0);
    if(euler.x)glRotatef(R2G(euler.x), 1, 0, 0);
    if(euler.z)glRotatef(R2G(euler.z), 0, 0, 1);

    V3  ex[3];
    
    glBegin(GL_LINES);
        glColor4ub(0,0,255*clratn,255);
        glVertex3fv(V0);
        ex[0] = VX*len;
        glVertex3fv(ex[0]);

        glColor4ub(0,255*clratn,0,255);
        glVertex3fv(V0);
        ex[1] = VY*len;
        glVertex3fv(ex[1]);
    
        glColor4ub(255*clratn,0,0,255);
        glVertex3fv(V0);
        ex[2] = VZ*len;
        glVertex3fv(ex[2]);

        glLineWidth(2.0);
        glColor4ub(255*clratn,255*clratn,255*clratn,255);
        glVertex3fv(V0);
        ex[2] = -VZ*len*1.4;
        glVertex3fv(ex[2]);
        glLineWidth(4.0);

    glEnd();

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // draw arrow cones;
    REAL    lh      = len/3.0;
    REAL    ld      = len/6.0;
    int     nVerts  ;
    V3*     pV3      = __GetCone(V3(0,0,len), lh, ld, nVerts);

        glPushMatrix();//Z
            glTranslatef(0, 0, len);
            glBegin(GL_TRIANGLE_FAN); 
                glColor4ub(0,0,255*clratn,255);
                glVertex3fv((REAL*)pV3[0]);    
                glColor4ub(0,0,192*clratn,255);
                for(int i=1;i<nVerts;i++)
                {
                    glVertex3fv((REAL*)pV3[i]);    
                }
            glEnd();
        glPopMatrix();

        glPushMatrix();//Z
            glTranslatef(0, 0, -(len*1.4));
            glRotatef(-179,1,0,0);
            glBegin(GL_TRIANGLE_FAN); 
                glColor4ub(255*clratn,255*clratn,255*clratn,255);
                glVertex3fv((REAL*)pV3[0]);    
                glColor4ub(192*clratn,192*clratn,192*clratn,255);
                for( i=1;i<nVerts;i++)
                {
                    glVertex3fv((REAL*)pV3[i]);    
                }
            glEnd();
        glPopMatrix();


        glPushMatrix();//Y
            glTranslatef(0, len, 0);
            glRotatef(-90,1,0,0);
            glBegin(GL_TRIANGLE_FAN); 
                glColor4ub(0,255*clratn,0,255);
                glVertex3fv((REAL*)pV3[0]);    
                glColor4ub(0,192*clratn,0,255);
                for( i=1;i<nVerts;i++)
                {
                    glVertex3fv((REAL*)pV3[i]);    
                }
            glEnd();
        glPopMatrix();


        glPushMatrix(); //X
            glTranslatef(len, 0, 0);
            glRotatef(90,0,1,0);
            glBegin(GL_TRIANGLE_FAN); 
                glColor4ub(255*clratn,0,0,255);
                glVertex3fv((REAL*)pV3[0]);    
                glColor4ub(192*clratn,0,0,255);
                for( i=1;i<nVerts;i++)
                {
                    glVertex3fv((REAL*)pV3[i]);    
                }
            glEnd();
        glPopMatrix();


    glPopMatrix();

    SetFaceMode();
    Ta.Restore();
}



static void AnimateSurfaceTexture(Poly& face, UV& tc, int ts)
{

    DWORD texBits = (face._polyflags2 & 0xF);
    if(texBits==0) texBits=1; // fix if tanim flag is set animate first stage
    if(texBits & (1 << ts))
    {
        if(face._polyprops & FACE_TA_RAD)
        {
            static int locoCnt = 1;
            UV ext((REAL)locoCnt*face._tanimA.u, (REAL)locoCnt*face._tanimA.v);
            if(tc.u < .5)
            {
                tc.u += ext.u;
                if(tc.u>.5)
                {
                    tc.u=0;
                    locoCnt=0;
                }
            }
            else
            {
                tc.u -= ext.u;
                if(tc.u < .5)
                {
                    tc.u = 1;
                    locoCnt=0;
                }
            }

            if(tc.v < .5)
            {
                tc.v += ext.v;

                if(tc.v>.5)
                {
                    tc.v=0;
                    locoCnt=0;
                }
            }
            else
            {
                tc.v -= ext.v;
                if(tc.v < .5)
                {
                    tc.v = 1;
                    locoCnt=0;
                }
            }
            locoCnt++;
            return;
        }
        if(face._polyprops & FACE_TA_RAD)
        {
            if(tc.u < .5)
            {
                tc.u += face._tanimA.u;
                if(tc.u>.5)
                    tc.u=0;
            }
            else
            {
                tc.u -= face._tanimA.v;
                if(tc.u < .5)
                    tc.u = 1;
            }

            if(tc.v < .5)
            {
                tc.v += face._tanimA.v;

                if(tc.v>.5)
                    tc.v=0;
            }
            else
            {
                tc.v -= face._tanimA.v;
                if(tc.v < .5)
                    tc.v = 1;
            }
            return;
        }

        if(face._polyprops & FACE_TA_FRAMING)
        {
            UV tcf = UV(1/face._tanimA.u, 1/face._tanimA.v);
            tc     *=  tcf;
            tcf    *= (REAL)PaintCounter;
            tcf.u-= ceil(tcf.u);
            tcf.v-= ceil(tcf.v);
            
            tc     +=  tcf;
            return;
        }

        if(face._polyprops & FACE_TA_WAVY )
        {
            UV depl(face._tanimA.u * Sinr(PaintCounter/16.0), face._tanimA.v * Sinr(PaintCounter/16.0));
            tc+=depl;
            return;
        }

        if(face._polyprops & FACE_TA_WINDY)
        {
            UV depl(face._tanimA.u * Sinr(PaintCounter/16.0), face._tanimA.v * Cosr(PaintCounter/16.0));
            tc+=depl;
            return;
        }

        if(face._polyprops & FACE_TA_SHIFT)
        {
            UV depl(face._tanimA.u * PaintCounter, face._tanimA.v * PaintCounter);
            tc+=depl;
             return;
        }

        //UV depl(FRAND(100,1000), FRAND(100,1000));
        //tc+=depl;
        return;
 
    }
} 

void CZ_ed2Doc::DrawPolyVertexes(DWORD how, Poly& poly, int texCoord, DWORD what)
{
    BYTE    farclr[4] = {255,255,255,255};
    UV      texC;
    BOOL    clrchangec = 0;
    //glColor4ubv(CLR_WHITE);
    glBegin(how);
        PVertex vb = poly._vtci.begin();
        PVertex ve = poly._vtci.end();
        for(;vb!=ve;vb++)
        {
            if(_compviewmode & C_BYFAR)
            {
                farclr[0] = BYTE((vb->_xyz.x/Poly::__Max.x)*255);
                farclr[1] = BYTE((vb->_xyz.y/Poly::__Max.y)*255);
                farclr[2] = BYTE((vb->_xyz.z/Poly::__Max.z)*255);
                glColor4ubv((BYTE*)farclr);
            }
            else if(what != 0xFFFFFFFF && !(_compviewmode & C_WHITE))
            {
                texC.setval(vb->_uv[texCoord].u,vb->_uv[texCoord].v);
                if(poly._polyprops & FACE_TAMASK)
                {
                    AnimateSurfaceTexture(poly, texC, texCoord);
                }
                glTexCoord2fv((REAL*)texC);
                if(GLight || GNormals)
    	            glNormal3f(vb->_nrm.x , vb->_nrm.y, vb->_nrm.z);
                //if(what & VX_COLOR)
                //    glColor4ub(vb->_rgb.r,vb->_rgb.g,vb->_rgb.b, vb->_rgb.a);

                if(vb->_rgb.r != 255 && vb->_rgb.g != 255 && vb->_rgb.b != 255)
                {
                    glColor4ubv((BYTE*)vb->_rgb);
                    clrchangec = 1;
                }
                else if(clrchangec)
                {
                    glColor4ubv(CLR_WHITE);
                    clrchangec = 0;
                }
            }
            glVertex3f(vb->_xyz.x, vb->_xyz.y, vb->_xyz.z);
        }
    glEnd();
}

void CZ_ed2Doc::DrawPolyLines(Poly& poly)
{
    PVertex vb,ve; 
    glBegin(GL_LINE_LOOP);
        vb = poly._vtci.begin();
        ve = poly._vtci.end();
        for(;vb!=ve;vb++)
        {
            glVertex3f(vb->_xyz.x, vb->_xyz.y, vb->_xyz.z);
        }
    glEnd();
}

void CZ_ed2Doc::DrawTerrain()
{
    _scene.Terrain().Render();
}


//MERGE//<


/* BEZIE PATCH with 9 points
void Bezier::tessellate(int L) {
    level = L;

    // The number of vertices along a side is 1 + num edges
    const int L1 = L + 1;

    vertex.resize(L1 * L1);

    // Compute the vertices
    int i;

    for (i = 0; i <= L; ++i) {
        double a = (double)i / L;
        double b = 1 - a;

        vertex[i] =
            controls[0] * (b * b) + 
            controls[3] * (2 * b * a) +
            controls[6] * (a * a);
    }

    for (i = 1; i <= L; ++i) {
        double a = (double)i / L;
        double b = 1.0 - a;

        BSPVertex temp[3];

        int j;
        for (j = 0; j < 3; ++j) {
            int k = 3 * j;
            temp[j] =
                controls[k + 0] * (b * b) + 
                controls[k + 1] * (2 * b * a) +
                controls[k + 2] * (a * a);
        }

        for(j = 0; j <= L; ++j) {
            double a = (double)j / L;
            double b = 1.0 - a;

            vertex[i * L1 + j]=
                temp[0] * (b * b) + 
                temp[1] * (2 * b * a) +
                temp[2] * (a * a);
        }
    }


    // Compute the indices
    int row;
    indexes.resize(L * (L + 1) * 2);

    for (row = 0; row < L; ++row) {
        for(int col = 0; col <= L; ++col)	{
            indexes[(row * (L + 1) + col) * 2 + 1] = row       * L1 + col;
            indexes[(row * (L + 1) + col) * 2]     = (row + 1) * L1 + col;
        }
    }

    trianglesPerRow.resize(L);
    rowIndexes.resize(L);
    for (row = 0; row < L; ++row) {
        trianglesPerRow[row] = 2 * L1;
        rowIndexes[row]      = &indexes[row * 2 * L1];
    }
    
}
 


Once constructed, this data can be rendered directly with vertex arrays: 


void Bezier::render() {
    glVertexPointer(3, GL_FLOAT,sizeof(BSPVertex), &vertex[0].position);

    glClientActiveTextureARB(GL_TEXTURE0_ARB);
    glTexCoordPointer(2, GL_FLOAT,sizeof(BSPVertex), &vertex[0].textureCoord);

    glClientActiveTextureARB(GL_TEXTURE1_ARB);
    glTexCoordPointer(2, GL_FLOAT, sizeof(BSPVertex), &vertex[0].lightmapCoord);

    glMultiDrawElementsEXT(GL_TRIANGLE_STRIP, trianglesPerRow.getCArray(),
        GL_UNSIGNED_INT, (const void **)(rowIndexes.getCArray()), patch.level);
}
 


*/

void   CZ_ed2Doc::DrawRect(Htex& texture, const Box& box)
{
    Ta.Enable();
    Ta.Bind(texture);
    
    
    glBegin(GL_POLYGON);

 	glTexCoord2f(0,0);
    glVertex3f(box._min.x, box._min.y,0);

   	glTexCoord2f(1,0);
    glVertex3f(box._max.x, box._min.y,0);


   	glTexCoord2f(1,1);
    glVertex3f(box._max.x, box._max.y,0);


  	glTexCoord2f(0,1);
    glVertex3f(box._min.x, box._max.y,0);

   
    glEnd();
    Ta.Restore();
}

