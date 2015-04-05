/*
*/

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
        if(Rabs(pPoly->_c - from->_c) < 8 && pPoly->_planeIdx == pRef->_planeIdx)
        {
            cb.Union(pPoly->_box);
            if(cb.GetMaxExtend() < mls)
            {
                pPoly->_polyflags |= POLY_PROCESSED;
                group << &(*pPoly);
            }
        }
    }
}
  
//---------------------------------------------------------------------------------------
BOOL LMProc::Calculate(Scene* pScene, vvector<CBspTree*>&  bTrees)
{
    vvector<CLightBulb*>	staticLights(128);
    vvector<Poly*>			polyGroupGoround(512);
    _pScene      = pScene;


    if(ThrID == GetCurrentThreadId())
    {
        START_WAIT("Please Wait...");
    }

    vvector<SceItem*>* pItems = _pScene->GetItems();
    FOREACH(vvector<SceItem*>, (*pItems), pItem)
    {
        if((*pItem)->_item==ITM_LIGTBULB)
        {
            staticLights << (CLightBulb*)(*pItem);
        }
    }

    _curPolys = 0;
    _totpolys = 0;
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
               pPoly->_polyprops & FACE_NOLIGTMAP   ||
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
            ++_totpolys;
        }
    }

  
    FOREACH(vvector<CBspTree*>,bTrees, ppT)
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
            {
                END_WAIT("");
            }
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
    {
        END_WAIT("");
    }
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
    Box     accBox;

    
    FOREACH(vvector<Poly>, _pTree->_polys, pPoly)
    {
        PROG((++_curPolys*100)/(_totpolys+1));

        if(pPoly->_polyflags & POLY_PROCESSED ) /// q?
        {
            continue;
        }
        polyGroup.clear();

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
            BOOL fitLmap = accBox.GetMaxExtend() <= Compiler::PCompiler->_lMapLumelSz * Compiler::PCompiler->_lmapMaxSize && 
                                                    (polyGroup.at(0)->_polyflags & POLY_SEP_LMAP) && 
                                                    Compiler::PCompiler->_lmConst  ;
            
            CalcLMapOnPolysConst(polyGroup, staticLights, fitLmap);
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
void    LMProc::CalcMinMaxUVs(vvector<Poly*>& polyGroup, UV& minUV,UV& maxUV)
{
	V3      normal = polyGroup[0]->_n;
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
        tg = TEX_NORMAL_LM;
#ifdef _DEBUG
    //tg = GEN_TEX_MM_NONE;
#endif //

    FOREACH(vvector<Poly>, pTree->_polys, pPoly)
    {
        if(pPoly->_lmInfo._lmIndex<0)
            continue;

        LmInfo&      plLmInf     = pPoly->_lmInfo;
        LmSizeBuff*  plmBuffer   = 0;
        plmBuffer   = _lMaps[plLmInf._lmIndex];
     

        SIZE lmsz = plLmInf._size;

        lmsz.cx &= 0xFFFF;
        lmsz.cy &= 0xFFFF;

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

        SIZE lmsz = plLmInf._size;
        lmsz.cx &= 0xFFFF;
        lmsz.cy &= 0xFFFF;
        
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


SIZE  LMProc::CalcBmpDims(const UV& uv)
{
    REAL lmUnit = Compiler::PCompiler->_lMapLumelSz;
	SIZE sz;    

    sz.cx  = int(ceil(uv.u/lmUnit));
    sz.cy  = int(ceil(uv.v/lmUnit));


	if(sz.cx  > Compiler::PCompiler->_lmapMaxSize)
        sz.cx  = Compiler::PCompiler->_lmapMaxSize;

	if(sz.cy  > Compiler::PCompiler->_lmapMaxSize)
        sz.cy  = Compiler::PCompiler->_lmapMaxSize;

    if(sz.cx < 4)    sz.cx = 4;
    if(sz.cy < 4)    sz.cy = 4;

    //if(sz.cy > sz.cx)sz.cx=sz.cy; else sz.cy=sz.cx;

    return sz;
}

//---------------------------------------------------------------------------------------
void    LMProc::GouraudShade(vvector<Poly*>& polyGroup, 
                             vvector<CLightBulb*>& lights)
{
    
    int         rgb[3];
    CollPack    col;
    

    int totpolys = polyGroup.size();
    totpolys    *= lights.size();
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

            PROG((++_curPolys*100)/(1+_totpolys));
     
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
                        intens = -(lIntens) * hitAngle / (dist2Light+EPSILON);
                        break;
                    case LIGHT_DIRECT:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < .9)  continue;
                            intens = -(lIntens * hitAngle * dp2Dir)/ (dist2Light+EPSILON);
                        }
                        break;
                    case LIGHT_SPOT:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < cfSp)  continue;
                            intens = -(lIntens * hitAngle * dp2Dir)/ (dist2Light+EPSILON);
                        }
                        break;
                    case LIGHT_SURFACE:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < .00625)  continue;
                            {
                                // stair the dist value
                                int d2l = (dist2Light/32)*32;
                                intens = -(lIntens * hitAngle * dp2Dir)/(EPSILON+d2l);
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
            TexHandler::SwapRB(lnB.pBuff, lnB.bpp, lnB.sz.cx & 0xFFFF, lnB.sz.cy & 0xFFFF);
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
    
    REAL            xS  = (ex.x) / ((REAL)(pbt->n_xtiles) + 0.0001);
    REAL            zS  = (ex.z) / ((REAL)(pbt->n_ztiles) + 0.0001);
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
                intens = -(lIntens) * hitAngle / (dist2Light + EPSILON);
                break;
            case LIGHT_DIRECT:
                {
                    REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                    if(dp2Dir < .09)  continue;
                    intens = -(lIntens * hitAngle * dp2Dir)/(dist2Light + EPSILON);
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
                    intens = -(lIntens * hitAngle * dp2Dir)/ (dist2Light + EPSILON);
                }
                break;
            case LIGHT_SURFACE:
                {
                    REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                    if(dp2Dir > 0)  continue;
                    {
                        // stair the dist value
                        int d2l = (dist2Light/32)*32;
                        intens = -(lIntens * hitAngle * dp2Dir)/(d2l+EPSILON);
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

//---------------------------------------------------------------------------------------
void    LMProc::CalcLMapOnPolys(vvector<Poly*>& polyGroup, 
                                vvector<CLightBulb*>& lights)
{
	REAL	    cfSp = 0;
	UV	        minUV;
	UV	        maxUV;
	V3	        a,b,c,ip;
    V3*	        pLumel = new V3[MAX_LMAPSIZE*MAX_LMAPSIZE];
    CollPack    col;

    if(Compiler::PCompiler->_lmapMaxSize> MAX_LMAPSIZE)
    {
        return;
    }

    //
    // calc minUV amd tmax UV
    //

    //
    // build the vectors direction
    //
	Plane   plane(polyGroup[0]->_n, polyGroup[0]->_c);
	CalcMinMaxUVs(polyGroup, minUV, maxUV);
	BuildLmapEdges(plane._c, polyGroup[0]->_n, minUV , maxUV,a,b,c);
	V3   edgeU = b-a;
    V3   edgeV = c-a;

    UV deltaUV = maxUV - minUV;    
	SIZE szBmp = CalcBmpDims(deltaUV);
    this->_RoundToClosestPow2(szBmp, 0);
    
	FOREACH(vvector<Poly*>, polyGroup, ppPoly)
	{
		Poly* pPoly = (*ppPoly);
        FOREACH(vvector<Vtx2>, pPoly->_vtci, pvtci)
        {
            pvtci->_uv[TX_LM] -=  minUV;
            pvtci->_uv[TX_LM] /=  deltaUV;
        }
	}

    //
    // default utouched surface light map
    //
    int dwsz = MAX_LMAPSIZE * MAX_LMAPSIZE;
    ::memset(pLumel, 0, sizeof(pLumel));
	for(int i=0; i < dwsz; ++i)
    {
		pLumel[i].x = _pScene->_si.defLumin.r;
        pLumel[i].y = _pScene->_si.defLumin.g;
        pLumel[i].z = _pScene->_si.defLumin.b;
    }
		
    int  nLit = 0;
    REAL dist;
	FOREACH(vvector<CLightBulb*> , lights, ppLight)
	{
		CLightBulb* pLB = *ppLight;

        //
        // no back lighting
        //
		dist = plane.DistTo(pLB->_t);
		if(dist < 0)            
			continue;

        //
        // too far away
        //
        if(dist > pLB->_radius)	
            continue;

        cfSp  = cosf(G2R(pLB->_specAngle));

		int lmOffset=0;
        //
        //go on every pLumel
        // 
        REAL uDelta;
        REAL vDelta;
		for(int i=0;i<szBmp.cx; i++)
		{
            
            uDelta   = (REAL)i / ((REAL)(szBmp.cx) + 0.0001);

            V3   egUDelta = edgeU * uDelta;
			for(int j=0;j<szBmp.cy; j++,lmOffset++)
			{
                vDelta   = (REAL)j / ((REAL)szBmp.cy + 0.0001);
                
				V3   egVDelta = edgeV * vDelta;
				ICLR lColor   = pLB->_colorD;
				REAL lIntens  = pLB->_lmIntensity*4;

                if(lIntens < 1) 
                {
                    continue;
                }


				V3   lumelPos  = a;
				lumelPos	   += egUDelta;
				lumelPos	   += egVDelta;

				REAL dist2Light   = vdist(lumelPos, pLB->_t);

                if(dist2Light > pLB->_radius)	
                {
                    continue;
                }
        			
                
				if(_pMainTree->SegmentIntersect(pLB->_t, lumelPos, 1, col))
                {
                    DWORD dwp = 0;
                    if(_pMainTree != col._pTree)
                    {
                        dwp = GET_CONTENT(col._pTree->_treeprops);

                        if(!(dwp & MODEL_MAT_OPAQUE))
                        {
                            CLR&    oclr  = col._pTree->_polys.front()._colorD;

                            if(oclr.a<255)
                            {

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
                        }
                        else
                            continue;

                    }
                    /*
					else //opaque
                    {
	                    continue;
                    }
                    */

				}
//NOSEG:
				V3      dit2Lumel = (lumelPos - pLB->_t).norm();
                REAL    intens  = 0;

                V3 vldir = VZ;
                pLB->_m.v3rotate(vldir);
                

                REAL    hitAngle = Vdp(dit2Lumel, plane._n);
                if(hitAngle > 0.0)  
                    continue;

                switch((pLB->_props  & 0xF))
                {
                    case 0:
                    case LIGHT_POINT:
                        if(pLB->_atten[0]==2) // quadratic attn
                        {
                            intens = (lIntens*100) * hitAngle / ((dist2Light*dist2Light) + 0.0001);
                        }
                        else                  // linear attn
                        {
                            intens = (lIntens) * hitAngle / (dist2Light + EPSILON);
                        }
                        break;
                    case LIGHT_DIRECT:
                        {
                            // light direction
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < .09)  continue;
                            if(pLB->_atten[0]==2)
                                intens = (100*lIntens * hitAngle * dp2Dir)/ ((dist2Light*dist2Light) + EPSILON);
                            else
                                intens = (lIntens * hitAngle * dp2Dir)/ (dist2Light + EPSILON);
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
                            if(pLB->_atten[0]==2)
                                intens = (100*lIntens * hitAngle * dp2Dir)/ (EPSILON+dist2Light);
                            else
                                intens = (lIntens * hitAngle * dp2Dir)/ ((dist2Light*dist2Light) + EPSILON);
                        }
                        break;
                    case LIGHT_SURFACE:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir > 0)  continue;
                            {
                                // stair the dist value
                                int d2l = (dist2Light/32)*32;
                                intens = (lIntens * hitAngle * dp2Dir)/(EPSILON + d2l);
                            }
                        }
                        break;
                }
                intens = -intens;
                    
                

				pLumel[lmOffset].x += lColor.r * intens; //B R G
				pLumel[lmOffset].y += lColor.g * intens;
				pLumel[lmOffset].z += lColor.b * intens;

                CLAMPVAL(pLumel[lmOffset].x, 0.00000, 255.00);
                CLAMPVAL(pLumel[lmOffset].y, 0.00000, 255.00);
                CLAMPVAL(pLumel[lmOffset].z, 0.00000, 255.00);

				nLit++;
		   }
		}
	}

////#ifdef _ONE_TEX_FOR_DEFLMAP
	if(!nLit)
	{
		//
		// asign default light map
		//
		FOREACH(vvector<Poly*>, polyGroup,ppPoly)
		{
			(*ppPoly)->_lmInfo._lmIndex = 0;

    	    (*ppPoly)->_lmInfo._size.cx = szBmp.cx; // size of bitmap light map
		    (*ppPoly)->_lmInfo._size.cy = szBmp.cy;
    
			(*ppPoly)->_lmInfo._lmAxes[0] = a;
			(*ppPoly)->_lmInfo._lmAxes[1] = b;
			(*ppPoly)->_lmInfo._lmAxes[2] = c;
		}

	}
	else
	{
        LmSizeBuff*  lmPls = new LmSizeBuff (szBmp.cx, szBmp.cy, szBmp.cx, szBmp.cy, 3);
        
		int offset1 = 0;
		int offset2 = 0,i;

		for(i=0; i < szBmp.cx; i++)
		{
			for(int j=0; j < szBmp.cy; j++, offset1++)
			{
				lmPls->pBuff[offset2++] = (BYTE) pLumel[offset1].x; 
				lmPls->pBuff[offset2++] = (BYTE) pLumel[offset1].y; 
				lmPls->pBuff[offset2++] = (BYTE) pLumel[offset1].z; 
			}
		}

		if(Compiler::PCompiler->_b3SampleLM)
			Smooth(lmPls->pBuff, szBmp);

        //
        // store in all polygons the light map index
        //
		FOREACH(vvector<Poly*>, polyGroup,ppPoly)
		{
			(*ppPoly)->_lmInfo._lmIndex = _lMaps.size();
		    (*ppPoly)->_lmInfo._size.cx = szBmp.cx; // size of bitmap light map
		    (*ppPoly)->_lmInfo._size.cy = szBmp.cy;
            //
            // store polygon axex
            //
			(*ppPoly)->_lmInfo._lmAxes[0] = a;
			(*ppPoly)->_lmInfo._lmAxes[1] = b;
			(*ppPoly)->_lmInfo._lmAxes[2] = c;
		}
		_lMaps << lmPls;

	}
    delete pLumel;

}

//---------------------------------------------------------------------------------------
void    LMProc::CalcLMapOnPolysConst(vvector<Poly*>& polyGroup, 
                                     vvector<CLightBulb*>& lights, BOOL bConst)
{
	REAL	    cfSp = 0;
	UV	        minUV;
	UV	        maxUV;
	V3	        a,b,c,ip;
    V3*	        pLumel = new V3[MAX_LMAPSIZE*MAX_LMAPSIZE];
    CollPack    col;

    // calc minUV amd tmax UV
	Plane   plane(polyGroup[0]->_n, polyGroup[0]->_c);
	CalcMinMaxUVs(polyGroup, minUV, maxUV);
    //
    // build the vectors direction starting from polys origin in the UV direction 
    // along polys plane
    //
	BuildLmapEdges(plane._c, polyGroup[0]->_n, minUV , maxUV,a,b,c);
	V3   edgeU = b-a;
    V3   edgeV = c-a;
    //
    // bitmap lightmap size from 0 to 1 place a bitmap 16x16 pixels
    //
    UV   realUV   = maxUV - minUV;  
    SIZE szBmp    = CalcBmpDims(realUV); 
    this->_RoundToClosestPow2(szBmp, 1);

    UV   deltaUV  = realUV;
    SIZE szUsed   = szBmp;

    if(bConst)
    {
        UV   szLcover;

        deltaUV.u   = Compiler::PCompiler->_lMapLumelSz * szBmp.cx;
        deltaUV.v   = Compiler::PCompiler->_lMapLumelSz * szBmp.cy;
        assert(szUsed.cx <= szBmp.cx);
        assert(szUsed.cy <= szBmp.cy);
        szUsed.cx   = 1 + ceil( (realUV.u / (deltaUV.u+EPSILON)) * szBmp.cx )  ;  //extra one for mipmap
        szUsed.cy   = 1 + ceil( (realUV.v / (deltaUV.v+EPSILON)) * szBmp.cy ) ;
        CLAMPVAL(szUsed.cx, 1, szBmp.cx);
        CLAMPVAL(szUsed.cy, 1, szBmp.cy);
    }
        

	FOREACH(vvector<Poly*>, polyGroup, ppPoly)
	{
		Poly* pPoly = (*ppPoly);
        FOREACH(vvector<Vtx2>, pPoly->_vtci, pvtci)
        {
            pvtci->_uv[TX_LM] -=  minUV;
            pvtci->_uv[TX_LM] /=  deltaUV;
        }
	}

    //
    // default utouched surface light map
    //
    int dwsz = MAX_LMAPSIZE * MAX_LMAPSIZE;
    ::memset(pLumel, 0, sizeof(pLumel));
	for(int i=0; i < dwsz; ++i)
    {
		pLumel[i].x = _pScene->_si.defLumin.r;
        pLumel[i].y = _pScene->_si.defLumin.g;
        pLumel[i].z = _pScene->_si.defLumin.b;
    }

    int  nLit = 0,i,j;
    REAL dist;
	FOREACH(vvector<CLightBulb*> , lights, ppLight)
	{
		CLightBulb* pLB = *ppLight;

        //
        // no back lighting
        //
		dist = plane.DistTo(pLB->_t);
		if(dist < 0)            
			continue;

        //
        // too far away
        //
        if(dist > pLB->_radius)	
            continue;

        cfSp  = cosf(G2R(pLB->_specAngle));

		int lmOffset=0;
        //
        //go on every pLumel
        // 
        REAL uDelta;
        REAL vDelta;
		for(int i=0;i < szBmp.cy; i++)
		{
            uDelta   = (REAL)i / ((REAL)szBmp.cy + EPSILON);
            V3   egUDelta = edgeU * uDelta;

			for( j=0;j < szBmp.cx; j++,lmOffset++)
			{
           
                if(i > szUsed.cy || j  > szUsed.cx) //leave  an extra one for the mip   
                {
                    pLumel[lmOffset].x = 255; //B R G   
				    pLumel[lmOffset].y = 0;
				    pLumel[lmOffset].z = 0;
                    continue; // leave lmOffset++ to go
                    // dot save this in the file anymore
                }
           
                vDelta   = (REAL)j / ((REAL)szBmp.cx + EPSILON);
				V3   egVDelta = edgeV * vDelta;
				ICLR lColor   = pLB->_colorD;
                REAL lIntens  = pLB->_lmIntensity * Compiler::PCompiler->_lmapIntens;

                if(lIntens < 1) 
                {
                    continue;
                }

				V3   lumelPos  = a;
				lumelPos	   += egUDelta;
				lumelPos	   += egVDelta;

				REAL dist2Light   = vdist(lumelPos, pLB->_t);

                if(dist2Light > pLB->_radius)	
                {
                    continue;
                }
        			
                
				if(_pMainTree->SegmentIntersect(pLB->_t, lumelPos, 1, col))
                {
                    DWORD dwp = 0;
                    if(_pMainTree != col._pTree)
                    {
                        dwp = GET_CONTENT(col._pTree->_treeprops);

                        if(!(dwp & MODEL_MAT_OPAQUE))
                        {
                            CLR&    oclr  = col._pTree->_polys.front()._colorD;

                            if(oclr.a<255)
                            {

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
                        }
                        else
                            continue;

                    }
					else //main model allways opaque
                    {
	                    continue;
                    }
				}
//NOSEG:
				V3      dit2Lumel = (lumelPos - pLB->_t).norm();
                REAL    intens  = 0;

                V3 vldir = VZ;
                pLB->_m.v3rotate(vldir);

                REAL    hitAngle = Vdp(dit2Lumel, plane._n);
                if(hitAngle > 0.0)  
                    continue;

                switch((pLB->_props  & 0xF))
                {
                    case 0:
                    case LIGHT_POINT:
                        if(pLB->_atten[0]==2) // quadratic attn
                        {
                            intens = (lIntens*100) * hitAngle / ((dist2Light*dist2Light) +EPSILON);
                        }
                        else                  // linear attn
                        {
                            intens = (lIntens) * hitAngle / (dist2Light + EPSILON);
                        }
                        break;
                    case LIGHT_DIRECT:
                        {
                            // light direction
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir < .09)  continue;
                            if(pLB->_atten[0]==2)
                                intens = (100*lIntens * hitAngle * dp2Dir)/ (EPSILON+(dist2Light*dist2Light));
                            else
                                intens = (lIntens * hitAngle * dp2Dir)/ (EPSILON+dist2Light);
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
                            if(pLB->_atten[0]==2)
                                intens = (100*lIntens * hitAngle * dp2Dir)/ (dist2Light + EPSILON);
                            else
                                intens = (lIntens * hitAngle * dp2Dir)/ (EPSILON+(dist2Light*dist2Light));
                        }
                        break;
                    case LIGHT_SURFACE:
                        {
                            REAL    dp2Dir = Vdp(dit2Lumel, vldir);
                            if(dp2Dir > 0)  continue;
                            {
                                // stair the dist value
                                int d2l = (dist2Light/32)*32;
                                intens = (lIntens * hitAngle * dp2Dir) / (d2l + EPSILON);
                            }
                        }
                        break;
                }
                
               
                intens = -intens;

				pLumel[lmOffset].x += lColor.r * intens; //B R G
				pLumel[lmOffset].y += lColor.g * intens;
				pLumel[lmOffset].z += lColor.b * intens;

                CLAMPVAL(pLumel[lmOffset].x, 0.00000, 255.00);
                CLAMPVAL(pLumel[lmOffset].y, 0.00000, 255.00);
                CLAMPVAL(pLumel[lmOffset].z, 0.00000, 255.00);

				nLit++;
		   }
		}
	}

////#ifdef _ONE_TEX_FOR_DEFLMAP
	if(!nLit)
	{
		//
		// asign default light map
		//
		FOREACH(vvector<Poly*>, polyGroup,ppPoly)
		{
			(*ppPoly)->_lmInfo._lmIndex = 0;

            //
            //  store in high word the used bitmap size and loword the original bmp size
            //
		    (*ppPoly)->_lmInfo._size.cx = (szUsed.cx << 16) | szBmp.cx; // size of bitmap light map
		    (*ppPoly)->_lmInfo._size.cy = (szUsed.cy << 16) | szBmp.cy;


			(*ppPoly)->_lmInfo._lmAxes[0] = a;
			(*ppPoly)->_lmInfo._lmAxes[1] = b;
			(*ppPoly)->_lmInfo._lmAxes[2] = c;
		}

	}
	else
	{
        LmSizeBuff*  lmPls = new LmSizeBuff (szBmp.cx, szBmp.cy, szUsed.cx, szUsed.cy, 3);
        
		int offset1 = 0;
		int offset2 = 0;

		for(i=0; i < szBmp.cy; i++)
		{
			for(int j=0; j < szBmp.cx; j++, offset1++)
			{
				lmPls->pBuff[offset2++] = (BYTE) pLumel[offset1].x; 
				lmPls->pBuff[offset2++] = (BYTE) pLumel[offset1].y; 
				lmPls->pBuff[offset2++] = (BYTE) pLumel[offset1].z; 
			}
		}

		if(Compiler::PCompiler->_b3SampleLM)
			Smooth(lmPls->pBuff, szBmp);

        //
        // store in all polygons the light map index
        //
		FOREACH(vvector<Poly*>, polyGroup,ppPoly)
		{
			(*ppPoly)->_lmInfo._lmIndex = _lMaps.size();
		    
            (*ppPoly)->_lmInfo._size.cx = (szUsed.cx << 16) | szBmp.cx; // size of bitmap light map
		    (*ppPoly)->_lmInfo._size.cy = (szUsed.cy << 16) | szBmp.cy;

            //
            // store polygon axex
            //
			(*ppPoly)->_lmInfo._lmAxes[0] = a;
			(*ppPoly)->_lmInfo._lmAxes[1] = b;
			(*ppPoly)->_lmInfo._lmAxes[2] = c;
		}
		_lMaps << lmPls;

	}
    delete pLumel;

}



