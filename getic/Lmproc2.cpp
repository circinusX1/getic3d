#if 0

// LMProc.cpp: implementation of the LMProc class.
//

#include "stdafx.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "LMProc.h"
#include "basecont.h"
#include "Scene.h"
#include "MainFrm.h"
#include "..\\_include\\BspFIleStr.h"
#include "BspTree.h"
#include "compiler.h"
#include "LightBulb.h"
#include "BigTerrain.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////#define _ONE_TEX_FOR_DEFLMAP

/*HINTS
http://www.acm.org/crossroads/xrds3-4/raytracing.html 
http://fuzzyphoton.tripod.com
ray_color *= filter_color / (intensity_loss_of_filter_per_meter * travel_length_through_filter); 
*/

//---------------------------------------------------------------------------------------
#define MAX_LMAPSIZE    256

#ifdef _DEBUG
struct Srgb3
{
    BYTE    r;
    BYTE    g;
    BYTE    b;
};
#endif //
//---------------------------------------------------------------------------------------
struct ICLR
{
    ICLR(const CLR& tr){
        r=tr.r;
        g=tr.g;
        b=tr.b;
    }
    ICLR& operator=(const ICLR& tr){
        if(this != &tr){
            r=tr.r;
            g=tr.g;
            g=tr.b;
        }
        return *this;
    }
    ICLR& operator=(const CLR& tr){
        r=tr.r;
        g=tr.g;
        g=tr.b;
        return *this;
    }

    int r;
    int g;
    int b;
};

//---------------------------------------------------------------------------------------
static void     RemoveTerrLmapTexture(TerTree* terr)
{
    FOREACH( vvector<TerLeaf*>, terr->p_leafs, ppl)
    {
        GTexSys.RemoveTexture((*ppl)->lm_Tex);
        //(*ppl)->lm_info._lmIndex = -1;
    }
}


//---------------------------------------------------------------------------------------
static void     RemoveLmapTexture(vvector<Poly>& polys, int Idx)
{
	FOREACH(vvector<Poly>, polys, pPoly)
    {
		if(pPoly->_ilmTex.hTex>0)
        {
			GTexSys.RemoveTexture(pPoly->_ilmTex);
            if(Idx)
			{
			    pPoly->_lmInfo._lmIndex = -1;
			}
		}
		//pPoly->_polyflags &= ~POLY_PROCESSED;
	}

}

//---------------------------------------------------------------------------------------
LMProc::~LMProc()
{
    _lMaps.deleteelements();
	_lMaps.clear();
}

//---------------------------------------------------------------------------------------
BOOL LMProc::AllocMemory(vvector<CBspTree*>& bspTrees)
{
	return TRUE;
}
/*
void  LMProc::_GatherSamePolys(vvector<Poly>& all, 
                               vvector<Poly>::iterator& from, 
                               vvector<Poly*>& group, 
                               Box& box)
{
    Poly*   pRef = (Poly*)from;
    REAL    mls  = Compiler::PCompiler->_lmapMaxSize* Compiler::PCompiler->_lMapLumelSz;

    //pRef->_color = ZRED;

    for(vvector<Poly>::iterator pPoly = (from+1); pPoly != all.end(); pPoly++)
    {
        if(pPoly->_polyflags & POLY_PROCESSED)
        {
            continue;
        }
	
        
        if( group.size()<32 && pPoly->_planeIdx == pRef->_planeIdx)
        {
            Box cb = box;
            cb.Union(pPoly->_box);

            if(cb.GetMaxExtend() < mls)
            {
                box = cb;
                pPoly->_polyflags |= POLY_PROCESSED;
                group << pPoly;
            }
        }
    }
}
*/
//---------------------------------------------------------------------------------------
// collects all same plane plys and all same orientation polys up to the extend of 
// maximum light map size
void  LMProc::_GatherSamePolys(vvector<Poly>& all, 
                               vvector<Poly>::iterator& from, 
                               vvector<Poly*>& group, 
                               Box& box)
{
    Box     cb;
    Poly*   pRef = (Poly*)&(*from);
    REAL    mls  = Compiler::PCompiler->_lmapMaxSize * Compiler::PCompiler->_lMapLumelSz;


    for(vvector<Poly>::iterator pPoly = (from+1); pPoly != all.end(); pPoly++)
    {
        if(pPoly->_polyflags & POLY_PROCESSED) // already into a group
        {
            continue;
        }
	
        // try to make it square as possible we can
        cb = box;

        if(Rabs(pPoly->_c - from->_c) < 8 && Vdp(pPoly->_n, from->_n) > .9 && 
           pPoly->_pBrush == from->_pBrush)
        {
            cb.Union(pPoly->_box);
            if(cb.GetMaxExtend() < mls)
            {
                pPoly->_polyflags |= POLY_PROCESSED;
                group << &(*pPoly);
            }
        }
    }

    REAL me = cb.GetMaxExtend();
    assert(me < mls);
}
  
//---------------------------------------------------------------------------------------
BOOL LMProc::Calculate(Scene* pScene, vvector<CBspTree*>&  bTrees)
{
    vvector<CLightBulb*>	staticLights(128);
    vvector<Poly*>			polyGroupGoround(512);
    _pScene      = pScene;
    Compiler::PCompiler->_lmConst = 1;

    if(ThrID == GetCurrentThreadId())
        START_WAIT("Please Wait...");

    vvector<SceItem*>* pItems = _pScene->GetItems();
    FOREACH(vvector<SceItem*>, (*pItems), pItem)
    {
        if((*pItem)->_item==ITM_LIGTBULB)
        {
            staticLights << (CLightBulb*)(*pItem);
        }
    }

    int totpolys = 0;
    int curPolys = 0;
    FOREACH(vvector<CBspTree*>,bTrees, ppT)
    {
        
        if((*ppT)->_treeprops & MODEL_IS_SKYDOM)
        {
            continue;
        }
        FOREACH(vvector<Poly>, (*ppT)->_polys, pPoly)
        {
            pPoly->_polyflags &= ~POLY_PROCESSED;

            if(pPoly->_polyprops & FACE_MIRROR      || 
               pPoly->_polyprops & FACE_TRANSPARENT ||
               pPoly->_polyprops & FACE_FULLBRIGHT  ||
               pPoly->_polyprops & FACE_CUSTLM)
            {
                pPoly->_polyflags |= POLY_PROCESSED;
                continue;
            }

            if(pPoly->_polyprops & FACE_GSHADE || 
               pPoly->_pBrush->_brushprops & BRSH_CSGTERRAIN )
            {
                pPoly->_polyflags |= POLY_PROCESSED;
                polyGroupGoround  << &(*pPoly);
                continue;
            }
            ++totpolys;
        }
    }
    

    _FOREACH(vvector<CBspTree*>,bTrees, ppT)
    {
        if (Compiler::PCompiler->HasBreaked())
        {
            FOREACH(vvector<CLightBulb*>, staticLights, pSl)
            {
                if((*pSl)->_props & LIGHT_TEMPORAY)
                    delete (*pSl);
            }
            NOTIFY_FRAME(WM_COMPILEDONE,E_LMAPS,0);
            return 0;
        }

        if(FALSE == Calculate(_pScene, bTrees[0], *ppT, staticLights))
        {
            if(ThrID == GetCurrentThreadId())
                END_WAIT("");
            PROG(-1);
            return FALSE;
        }
    }
	if(polyGroupGoround.size())
    {
		GouraudShade(polyGroupGoround, staticLights);
    }


    if(Compiler::PCompiler->_terrTree.Exist())
    {
         CalculateTtLm(Compiler::PCompiler->_terrTree, staticLights);
    }

    FOREACH(vvector<CLightBulb*>, staticLights, pSl)
    {
        if((*pSl)->_props & LIGHT_TEMPORAY)
            delete (*pSl);
    }
	PROG(-1);
    if(ThrID == GetCurrentThreadId())
        END_WAIT("");
    PROG(-1);
    return TRUE;
}

//---------------------------------------------------------------------------------------
// unit represent lumels/meter
BOOL LMProc::Calculate(Scene* pScene, CBspTree* pTreeMain, CBspTree* pTree, vvector<CLightBulb*>& staticLights)
{
    // bitmap size
    if(Compiler::PCompiler->_lmapMaxSize < 2)
        Compiler::PCompiler->_lmapMaxSize = 2;
    else if(Compiler::PCompiler->_lmapMaxSize > 128)
        Compiler::PCompiler->_lmapMaxSize = 128;

    // pLumel in units
    if(Compiler::PCompiler->_lMapLumelSz < 16)
        Compiler::PCompiler->_lMapLumelSz = 16;
    else if(Compiler::PCompiler->_lMapLumelSz > 1024)
        Compiler::PCompiler->_lMapLumelSz = 1024;

	vvector<Poly*> polyGroup(32);   // per (model/BSP)

	_pMainTree   = pTreeMain;
    _pTree       = pTree;

    RemoveLmapTexture(_pTree->_polys,1);
    RemoveTerrLmapTexture(&Compiler::PCompiler->_terrTree);


////#ifdef _ONE_TEX_FOR_DEFLMAP
	//
    // default utouched surface light map
	//
    LmSizeBuff*  lmDef = new LmSizeBuff(Compiler::PCompiler->_lmapMaxSize,
                                        Compiler::PCompiler->_lmapMaxSize,
                                        Compiler::PCompiler->_lmapMaxSize,
                                        Compiler::PCompiler->_lmapMaxSize,
                                        3);
	
	BYTE*   pHead  = lmDef->pBuff;
	for(int i=0; i<Compiler::PCompiler->_lmapMaxSize * Compiler::PCompiler->_lmapMaxSize; i++)
    {
		*pHead++ =_pScene->_si.defLumin.r;
		*pHead++ =_pScene->_si.defLumin.g;
		*pHead++ =_pScene->_si.defLumin.b;
    }
	_lMaps << lmDef;	// store dfault ilumination light map on pos 0
////#endif //

	// group together coplanar and iterconected polys for building a coommon light map on them
	// use the USER_FLAG to mark them - lightmapped
	// Clean the user flag first

    int     totPolys        = _pTree->_polys.size();
    int     remMainPolys    = totPolys;
    int     percent;
    Box     accBox;
	_totpolys = totPolys;
	_curPolys = 0;


    FOREACH(vvector<Poly>, _pTree->_polys, pPoly)
    {
        ++_curPolys;
        percent = (_curPolys*100)/_totpolys;
        PROG(percent);

        if(pPoly->_polyflags & POLY_PROCESSED ) /// q?
        {
            continue;
        }

        accBox.Reset();
        if(pPoly->_polyflags & POLY_SEP_LMAP)  // was split because was too big to acomodate a lmap
        {
		    pPoly->_polyflags |= POLY_PROCESSED;
		    polyGroup    << &(*pPoly);
            accBox    =   pPoly->_box;
        }
        else
        {
        
            pPoly->_polyflags |= POLY_PROCESSED;
            polyGroup  << &(*pPoly);
            accBox.Reset();
            accBox    =   pPoly->_box;
            _GatherSamePolys(_pTree->_polys, pPoly, polyGroup, accBox);
        }

		if(polyGroup.size())
        {
			CalcLMapOnPolys(polyGroup, staticLights);
            polyGroup.clear();
        }

        if (Compiler::PCompiler->HasBreaked())
        {
            return FALSE;
        }
    }
    return TRUE;
}

//---------------------------------------------------------------------------------------    
static void Smooth(BYTE* pLMap, SIZE& sz)
{
  
}

//---------------------------------------------------------------------------------------
//no thread call dont call this from thread
void LMProc::NTC_Clear()
{
    ASSERT(ThrID == GetCurrentThreadId());

	_lMaps.deleteelements();
	_lMaps.clear();
    _totpolys = 0;
}

//---------------------------------------------------------------------------------------
void    LMProc::BuildLmapEdges(REAL pc, V3& normal, UV& minUV, UV& maxUV, V3& v0, V3& v1 ,V3& v2)
{
	V3 majorAx = GetMaxAx(normal);

    if(majorAx.x)       // zy
    {
        v0.x = -( normal.y * minUV.u + normal.z * minUV.v - pc ) / normal.x;
        v0.y = minUV.u;
        v0.z = minUV.v;

        v2.x = -( normal.y * maxUV.u + normal.z * minUV.v - pc ) / normal.x;
        v2.y = maxUV.u;
        v2.z = minUV.v;
        
        v1.x = -( normal.y * minUV.u + normal.z * maxUV.v - pc ) / normal.x;
        v1.y = minUV.u;
        v1.z = maxUV.v;
    }
    else if(majorAx.y)  // xz
    {
        v0.x = minUV.u;
        v0.y = -( normal.x * minUV.u + normal.z * minUV.v - pc ) / normal.y;
        v0.z = minUV.v;

        v2.x = maxUV.u;
        v2.y = -( normal.x * maxUV.u + normal.z * minUV.v - pc ) / normal.y;
        v2.z = minUV.v;

        v1.x = minUV.u;
        v1.y = -( normal.x * minUV.u + normal.z * maxUV.v - pc ) / normal.y;
        v1.z = maxUV.v;
    }
    else                // xy
    {
        v0.x = minUV.u;
        v0.y = minUV.v;
        v0.z = -( normal.x * minUV.u + normal.y * minUV.v - pc ) / normal.z;
        
        v2.x = maxUV.u;
        v2.y = minUV.v;
        v2.z = -( normal.x * maxUV.u + normal.y * minUV.v - pc ) / normal.z;
        
        v1.x = minUV.u;
        v1.y = maxUV.v;
        v1.z = -( normal.x * minUV.u + normal.y * maxUV.v - pc ) / normal.z;
    }
}

//---------------------------------------------------------------------------------------
// gets the minimum coord of the polys and the tmax coord of all polys on the 
// on the polys plane (UV)
void    LMProc::CalcMinMaxUVs(vvector<Poly*>& polyGroup, UV& minUV,UV& maxUV)
{
	V3 normal = polyGroup[0]->_n;
    minUV   = UV(INFINIT,INFINIT);
    maxUV   = UV(-INFINIT,-INFINIT);

	FOREACH(vvector<Poly*>, polyGroup, ppPoly)
	{
		Poly* pPoly = (*ppPoly);

#ifdef _DEBUG
       	const char* ptex = GTexSys.GetTexName(pPoly->GetHtex(GUtex));
#endif //
        pPoly->MinMaxUV(1, minUV, maxUV, &normal);
	}
}

//---------------------------------------------------------------------------------------
//no thread call.. dont call this from thread
void LMProc::NTC_BuildLmaps(CBspTree* pTree)
{
    int	lmCount = 0;

    CWaitCursor cwc;

    RemoveLmapTexture(pTree->_polys,0);

    DWORD tg = TEX_LOWREZ;
    if(Compiler::PCompiler->_b3SampleLM)
    {
        tg = TEX_NORMAL_LM;
    }

#ifdef _DEBUG
    tg = GEN_TEX_MM_NONE;
#endif //

    FOREACH(vvector<Poly>, pTree->_polys, pPoly)
    {
        if(pPoly->_lmInfo._lmIndex<0)
            continue;

        LmInfo&      plLmInf     = pPoly->_lmInfo;
        LmSizeBuff*  plmBuffer   = 0;
        plmBuffer   = _lMaps[plLmInf._lmIndex];
     
        SIZE lmsz = {plLmInf._size.cx & 0xFFFF, plLmInf._size.cy & 0xFFFF};
#ifdef _DEBUG
        Srgb3* prgb = (Srgb3*)plmBuffer->pBuff;
#endif //
            
        pPoly->_ilmTex = GTexSys.GenTexture(MKSTR("@L%d",
                                            plLmInf._lmIndex), 
                                            lmsz.cx, 
                                            lmsz.cy, 
                                            plmBuffer->bpp, 
                                            plmBuffer->pBuff, 
                                            tg);

        if(pPoly->_ilmTex > 0)
        {
            strcpy(plmBuffer->lmName, MKSTR("@L%d", plLmInf._lmIndex));
        }
    }
}


//---------------------------------------------------------------------------------------
//no thread call.. dont call this from thread
void LMProc::NTC_BuildLmaps(TerTree* pTree)
{
    if(pTree->g_light.size())
        return;
    DWORD tg = TEX_LOWREZ;
    if(Compiler::PCompiler->_b3SampleLM)
        tg = TEX_NORMAL_LM;

#ifdef _DEBUG
    tg = GEN_TEX_MM_NONE;
#endif //

    CWaitCursor cwc;
    RemoveTerrLmapTexture(pTree);

    TerLeaf*    pLeaf;
    FOREACH(vvector<TerLeaf*>, pTree->p_leafs, ppLeaf)
    {
        pLeaf = (*ppLeaf);
        if(pLeaf->lm_info._lmIndex < 0)
            continue;
        LmInfo&      plLmInf     = pLeaf->lm_info;
        LmSizeBuff*  plmBuffer   = _lMaps[plLmInf._lmIndex];

        SIZE lmsz = {plLmInf._size.cx & 0xFFFF, plLmInf._size.cy & 0xFFFF};

        

        pLeaf->lm_Tex = GTexSys.GenTexture(MKSTR("@L%d",
                                            plLmInf._lmIndex), 
                                            lmsz.cx, 
                                            lmsz.cy, 
                                            plmBuffer->bpp, 
                                            plmBuffer->pBuff, 
                                            tg);
        if(pLeaf->lm_Tex > 0)
        {
            strcpy(plmBuffer->lmName, MKSTR("@L%d", plLmInf._lmIndex));
        }
    }
}


SIZE  LMProc::CalcBmpDims(const UV& edgesLength)
{
    SIZE sz ={  (int)(ceil( edgesLength.u / Compiler::PCompiler->_lMapLumelSz )),     
                (int)(ceil( edgesLength.v / Compiler::PCompiler->_lMapLumelSz ))  };

    CLAMPVAL(sz.cx, 4, Compiler::PCompiler->_lmapMaxSize);
    CLAMPVAL(sz.cy, 4, Compiler::PCompiler->_lmapMaxSize);
    this->_RoundToClosestPow2(sz,0);
    if(sz.cx>sz.cy)
        sz.cy=sz.cx;
    else 
        sz.cx=sz.cy;
    return sz;
}

//---------------------------------------------------------------------------------------
void    LMProc::GouraudShade(vvector<Poly*>& polyGroup, 
                             vvector<CLightBulb*>& lights)
{
    
    int         rgb[3];
    CollPack    col;
    int         percent;

    _curPolys    = 0;
    int totpolys = polyGroup.size();
    totpolys    *= lights.size();
    Sleep(200);
    PROG(0);
    
    FOREACH(vvector<Poly*>, polyGroup, ppPoly)
    {
        FOREACH(vvector<Vtx2>, (*ppPoly)->_vtci, pVtci)
        {
            pVtci->_rgb.r = _pScene->_si.defLumin.r;
			pVtci->_rgb.g = _pScene->_si.defLumin.g;
			pVtci->_rgb.b = _pScene->_si.defLumin.b;
			pVtci->_extra1=-1;
        }
    }
    

    FOREACH(vvector<CLightBulb*> , lights, ppLight)
    {
        CLightBulb* pLB = *ppLight;
        REAL lIntens    = pLB->_lmIntensity * 10;
        CLR	 lColor     = pLB->_colorD;
        V3 vldir = VZ;
        pLB->_m.v3rotate(vldir);

        if(lIntens <=0)
              continue;

        REAL cfSp  = cosf(G2R(pLB->_specAngle));

        FOREACH(vvector<Poly*>, polyGroup, ppPoly)
        {
            Poly* pPoly = *ppPoly;

            percent   = (_curPolys*100)/_totpolys;
            PROG(percent);
            ++_curPolys;

     
            if(!pPoly->_box.IsTouchesSphere(pLB->_t,pLB->_radius))
                continue;

            FOREACH(vvector<Vtx2>, pPoly->_vtci, pVtci)
            {
                REAL dist2Light   = vdist(pVtci->_xyz, pLB->_t);

                if(dist2Light > pLB->_radius || dist2Light < 0)
                    continue;

                if(_pMainTree->SegmentIntersect(pLB->_t, pVtci->_xyz, 1, col))
                    continue;

				V3      dit2Lumel = (pVtci->_xyz - pLB->_t).norm();
                REAL    intens  = 0;

                #pragma message("see this 32 factor ????")
                //REAL    hitAngle = Vdp(dit2Lumel, pPoly->_n);
				ASSERT(pVtci->_extra1>=0);

				REAL    hitAngle = Vdp(dit2Lumel, pPoly->_n);//normals[pVtci->_extra1]);
                
				if(hitAngle > 0)                // back face
                     continue;

                // should be between 0 and 1
                switch((pLB->_props  & ~LIGHT_TEMPORAY))
                {
                    case 0:
                    case LIGHT_POINT:
                        intens = -(lIntens) * hitAngle / dist2Light;
                        break;
                    case LIGHT_DIRECT:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < .9)  continue;
                            intens = -(lIntens * hitAngle * dp2Dir)/ dist2Light;
                        }
                        break;
                    case LIGHT_SPOT:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < cfSp)  continue;
                            intens = -(lIntens * hitAngle * dp2Dir)/ dist2Light;
                        }
                        break;
                    case LIGHT_SURFACE:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < .00625)  continue;
                            {
                                // stair the dist value
                                int d2l = (dist2Light/32)*32;
                                intens = -(lIntens * hitAngle * dp2Dir)/d2l;
                            }
                        }
                        break;
                }                                
                rgb[0] = pVtci->_rgb.r + (lColor.r * intens);
                rgb[1] = pVtci->_rgb.g + (lColor.g * intens);
                rgb[2] = pVtci->_rgb.b + (lColor.b * intens);

                pVtci->_rgb.r = tmin(255,rgb[0]) ;
				pVtci->_rgb.g = tmin(255,rgb[1]);
				pVtci->_rgb.b = tmin(255,rgb[2]);
            }
        }
    }
}

//---------------------------------------------------------------------------------------
BOOL LMProc::GetLmapBytes(const char* name, LmSizeBuff& lnB)
{
    if(0 == _lMaps.size())
        return 0;
    int index=  ::atoi(name+2/*dangerous*/);    // "@L***"

    vvector<LmSizeBuff*>::iterator b =  _lMaps.begin();
    vvector<LmSizeBuff*>::iterator e =  _lMaps.end();
    for(;b!=e ; ++b)
    {
        int il = ::atoi((*b)->lmName+2);        // "@L***"
        if(index == il){
            lnB = *(*b);
            TexHandler::SwapRB(lnB.pBuff, lnB.bpp, lnB.sz.cx, lnB.sz.cy);
            return TRUE;
        }
    }
    return FALSE;
}

//---------------------------------------------------------------------------------------
void LMProc::CalculateTtLm(TerTree& tt, vvector<CLightBulb*>& lights)
{
    CBigTerrain*    pbt = tt.p_Terrain;
    V3              ex  = pbt->b_box.GetExtends();
    REAL            xS  = (ex.x) / (REAL)pbt->n_xtiles;
    REAL            zS  = (ex.z) / (REAL)pbt->n_ztiles;
    V3              vWalk1, vWalk2, vWalk3, vmin = pbt->b_box._min;
    UINT            x,z;

    tt.g_light.clear();
    CLRNOA     rgbnoa;

    if(tt.x_flags & TERR_FULLBRGHT) // while color all
        return;
    // make rgb gouroung map
    rgbnoa.r = _pScene->_si.defLumin.r;
    rgbnoa.g = _pScene->_si.defLumin.g;
    rgbnoa.b = _pScene->_si.defLumin.b;
    for(x=0; x < pbt->v_vxes.Size();x++)
        tt.g_light << rgbnoa;
    //recal it base on lights
    int lit = 0;
    for(x=0 ; x < pbt->n_xtiles-1; x++)
    {
        vWalk1.x = vmin.x + x * xS;
        vWalk2.x = vmin.x + (x+1) * xS;

        for(z=0 ; z < pbt->n_ztiles-1; z++)
        {
            vWalk1.z = vmin.z + z * zS;
            vWalk1.y = *(element(&pbt->v_vxes, x, z, pbt->n_xtiles));
            vWalk1.y /= 255.0;
            vWalk1.y *= ex.y;
            vWalk1.y += vmin.y;

            vWalk2.z = vWalk1.z;
            vWalk2.y = *(element(&pbt->v_vxes, (x+1), z, pbt->n_xtiles));
            vWalk2.y /= 255.0;
            vWalk2.y *= ex.y;
            vWalk2.y += vmin.y;

            vWalk3.x = vWalk1.x;
            vWalk3.z = vmin.z + (z+1) * zS;
            vWalk3.y = *(element(&pbt->v_vxes, x, (z+1), pbt->n_xtiles));
            vWalk3.y /= 255.0;
            vWalk3.y *= ex.y;
            vWalk3.y += vmin.y;
            lit += _YolTtLight(tt, x, z, vWalk1, vWalk2, vWalk3, &tt.g_light[0], pbt->n_xtiles, lights);
        }
    }
    Texs&   t = pbt->Texs();

    #pragma message("fix terrain lightmap")
    return;
    // if lmap is overwritten or gouroud dont continue
    if( (tt.x_flags & TERR_GOR)     || 
        (tt.x_flags & TERR_CUSTLM)  )  //leave gouround vertex lightinf
        return;


    TerLeaf*    pLeaf;
    FOREACH(vvector<TerLeaf*>, tt.p_leafs, ppLeaf)
    {
        pLeaf = (*ppLeaf);
        LmSizeBuff*  lmPls = new LmSizeBuff (TerLeafCelTiles, TerLeafCelTiles, TerLeafCelTiles, TerLeafCelTiles, 3);
        int   offset   = 0;
        const SIZE& s  = pLeaf->s_tiles[0];
        const SIZE& e  = pLeaf->s_tiles[1];
        BOOL  leafislit= FALSE;
        for(x=s.cx; x < e.cx; x++)
        {
            for(z=s.cy; z < e.cy; z++)
            {
                CLRNOA* rgbnoa = element(&tt.g_light[0], x, z, pbt->n_xtiles);

                if(rgbnoa->r != _pScene->_si.defLumin.r ||
                   rgbnoa->g != _pScene->_si.defLumin.g ||
                   rgbnoa->b != _pScene->_si.defLumin.b )
                {
                   leafislit=TRUE;
                }
                lmPls->pBuff[offset++] = rgbnoa->r;
                lmPls->pBuff[offset++] = rgbnoa->g;
                lmPls->pBuff[offset++] = rgbnoa->b;
            }
        }
         
        // one lmap for all unlit cells
        pLeaf->lm_info._size.cx = TerLeafCelTiles;
        pLeaf->lm_info._size.cy = TerLeafCelTiles;
        if(leafislit) // if lit use calculated map
        {
            pLeaf->lm_info._lmIndex = _lMaps.size();
            _lMaps << lmPls;
        } 
        else  //place default map ()
        {
            delete lmPls;
            pLeaf->lm_info._lmIndex = 0;
        }
    }
    tt.g_light.clear();
}

//---------------------------------------------------------------------------------------
int    LMProc::_YolTtLight( TerTree& tt,
                            int x, int z, 
                            const V3& vWalk1, 
                            const V3& vWalk2, 
                            const V3& vWalk3, 
                            CLRNOA* glight, 
                            int xtiles,
                            vvector<CLightBulb*>& lights)
{
    if(tt.x_flags & TERR_NOLIGHT)  // def light all
    {
        return 0;
    }
    
    CLRNOA*     pcell = element(&glight[0], x, z, xtiles);
    int         r=pcell->r,g=pcell->g,b=pcell->b;
    V3          lumelPos = vWalk1;
    V3          va   = (vWalk3-vWalk1);  
    V3          vb   = (vWalk2-vWalk1);  
    V3          nrm = Vcp(va,vb); nrm.norm();
    V3          pLumel(r,g,b);
    
    if(_GetLumColor(pLumel, lumelPos, nrm, lights))
    {
        r = pLumel.x; CLAMPVAL(r,0,255);
        g = pLumel.y; CLAMPVAL(r,0,255);
        b = pLumel.z; CLAMPVAL(r,0,255);
        pcell->r=r;
        pcell->g=g;
        pcell->b=b;
        return 1;//lit
    }
    return 0;//ambient
}

//---------------------------------------------------------------------------------------
int  LMProc::_GetLumColor(V3& pLumel, V3& lumelPos, const V3& norm, vvector<CLightBulb*>& lights)
{
    int nlums = 0;

    if(pLumel.x>254 && pLumel.y>254 && pLumel.z>254) 
       return 0;

    CollPack    col;
    FOREACH(vvector<CLightBulb*> , lights, ppLight)
    {
        CLightBulb* pLB = *ppLight;
        REAL lIntens    = pLB->_lmIntensity * 10;
        CLR	 lColor     = pLB->_colorD;

        if(lIntens < 0.01 )                                 continue;

        REAL dist2Light   = vdist(lumelPos, pLB->_t);
		if(dist2Light > pLB->_radius || dist2Light <= 1)       continue;

		if(_pMainTree->SegmentIntersect(pLB->_t, lumelPos, 1, col))
        {
            DWORD dwp = 0;
            if(_pMainTree != col._pTree)
            {
                dwp = GET_CONTENT(col._pTree->_treeprops);
				if(!( dwp &  MODEL_MAT_OPAQUE))
                {
					goto NOSEG;
                }
            }

            //DOC()->_testPoints.push_back(col._ip);
            if(!(dwp & MODEL_MAT_OPAQUE))
            {
                CLR&    oclr  = col._pTree->_polys.front()._colorD;
                V3      oclColor(oclr.r, oclr.g, oclr.b);
                V3      rayClr(pLB->_colorD.r, pLB->_colorD.g, pLB->_colorD.b);
                REAL    oneOverAlpha = 255.0-(REAL)oclr.a;
                if(0==oneOverAlpha) oneOverAlpha = 0.00001;
                REAL    ocAlpha = (REAL)(255.0-(REAL)oclr.a)/255.0;  // 0-opaque 1-transp.

                oclColor.norm();
                rayClr.norm();
				
				SCOPE(1)
                {
                    lColor.r = 255*(oclColor.x*rayClr.x*ocAlpha);
                    lColor.g = 255*(oclColor.y*rayClr.y*ocAlpha);
                    lColor.b = 255*(oclColor.z*rayClr.z*ocAlpha);
                }
            }
			else //opaque
            {
	            continue;
            }
		}
NOSEG:

        V3 vldir = VZ;
        pLB->_m.v3rotate(vldir);

	    V3      dit2Lumel = (lumelPos - pLB->_t).norm();
        REAL    intens  = 0;
        REAL    hitAngle = Vdp(dit2Lumel, norm);
        if(hitAngle > 0.0)  continue;

        REAL cfSp  = cosf(G2R(pLB->_specAngle));

        switch((pLB->_props  & 0xF))
        {
            case 0:
            case LIGHT_POINT:
                intens = -(lIntens) * hitAngle / dist2Light;
                break;
            case LIGHT_DIRECT:
                {
                    REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                    if(dp2Dir < .09)  continue;
                    intens = -(lIntens * hitAngle * dp2Dir)/ dist2Light;
                }
                break;
            case LIGHT_SPOT:
                {
                    // conentrate intensity in given angle
                    // considering full reflective closing
                    // calc angle opening raport
                    int raposrUp = (360/pLB->_specAngle);
                    lIntens *= raposrUp;
                    REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                    if(dp2Dir < cfSp)  continue;
                    intens = -(lIntens * hitAngle * dp2Dir)/ dist2Light;
                }
                break;
            case LIGHT_SURFACE:
                {
                    REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                    if(dp2Dir > 0)  continue;
                    {
                        // stair the dist value
                        int d2l = (dist2Light/32)*32;
                        intens = -(lIntens * hitAngle * dp2Dir)/d2l;
                    }
                }
                break;
        }

		pLumel.x += lColor.r * intens; //B R G
		pLumel.y += lColor.g * intens;
		pLumel.z += lColor.b * intens;

		if(pLumel.x > 255.0) pLumel.x = 255.0;
		if(pLumel.y > 255.0) pLumel.y = 255.0;
		if(pLumel.z > 255.0) pLumel.z = 255.0;
        ++nlums;
    }
    return nlums;

}


int  LMProc::_RoundToClosestPow2(SIZE& sz, int up)
{
    int tp2,tp21;
    if(!IsPowOf2(sz.cx))
    {
        tp2 = ToPow2(sz.cx);
        tp21 = tp2 << 1;
        if((sz.cx - tp2) > (tp21-tp2)/2 || up)
            sz.cx = tp21;
        else
            sz.cx = tp2;
    }
        
    if(!IsPowOf2(sz.cy))
    {
        tp2 = ToPow2(sz.cy);
        tp21 = tp2 << 1;
        if((sz.cy - tp2) > (tp21-tp2)/2 || up)
            sz.cy = tp21;
        else
            sz.cy = tp2;
    }
    CLAMPVAL(sz.cx, 4, Compiler::PCompiler->_lmapMaxSize);
    CLAMPVAL(sz.cy, 4, Compiler::PCompiler->_lmapMaxSize);

    return 0;
}






#endif // 0