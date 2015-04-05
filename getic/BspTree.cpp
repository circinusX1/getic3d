//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "scene.h"
#include "MainFrm.h"
#include "bsptree.h"
#include <gl/glu.h>
#include <assert.h>
#include "z_ed3View.h"
#include "Mainfrm.h"
#include "PortalPrc.h"
#include "z_ed2Doc.h"
#include "texadapter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define     BAD_GEOMERTY    6
#define     BOGUS_VALUE     524288
static int Sign = 0;
//--| Box GetVxesBbox|--------------------------------------------------------------------
static Box GetVxesBbox(vvector<Vtx2>& pcts)
{
    Box b;
    
    FOREACH(vvector<Vtx2>, pcts, pp)
    {
        b.AddPoint(pp->_xyz);
    }
    return b;
}


//--| void SetPvsBit |--------------------------------------------------------------------
static void SetPvsBit (BYTE *pVisArray, int nDestLeaf)
{
    pVisArray[nDestLeaf>>3] |= (1<<(nDestLeaf&7));
}

//--| BOOL GetPvsBit |--------------------------------------------------------------------
static BOOL GetPvsBit (BYTE *pVisArray, int pDestLeaf)
{
    return pVisArray[pDestLeaf>>3] & (1<<(pDestLeaf&7));
}


#define PON_FRONT    1
#define PON_BACK  2
#define PON_BOTH  (PON_FRONT|PON_BACK)
#define PON_FACE  4

//--|   DWORD ClassifyBox|----------------------------------------------------------------
static   DWORD ClassifyBox(Plane& plan, Box& box)
{
    V3   corners[2];
    GetCornersOfPlane(box._min, box._max, plan._n, corners);
    REAL d1 = plan.DistTo(corners[0]);
    REAL d2 = plan.DistTo(corners[1]);
    DWORD side = 0;
    if (d1 >= 0)
       side |= PON_FRONT;
    if (d2 < 0)
       side |= PON_BACK;
    return side;
}

CBspTree::CBspTree():_prtProc(0),_motionsIdx(-1),_patchPolys(FALSE),_zoneareea(-1)
{
}

//--|    CBspTree::Clear|-----------------------------------------------------------------
void    CBspTree::Clear()
{
    _sounds.clear();
    _pSubModels.clear();
    _leafs.clear();
    MiniBsp::Clear();
}

//--|CBspTree::~CBspTree|-----------------------------------------------------------------
CBspTree::~CBspTree()
{
    Clear();
}

/**
Compile the bsp
*/
//--| CBspTree::Compile|------------------------------------------------------------------
BOOL CBspTree::Compile(vvector<Poly>& polys, DWORD brFlags, BOOL bLog, BOOL btextConvex) // virtual from base
{
    
    BOOL b = FALSE;
    if(polys.size()==0)
        return TRUE;
    _TRY{

        if(IsMainModel() && 1)
        {
            vvector<Poly>   patchedPolys;
            PatchPolys(polys, patchedPolys);//_bSnapMainBsp
            b =  MiniBsp::Compile(patchedPolys, brFlags, bLog, btextConvex);
        }
        else
        {
            b =  MiniBsp::Compile(polys, brFlags, bLog, btextConvex);
        }
    }
    _CATCHX()
    {
        AfxMessageBox("Patch Exception. Please Disable Polygon Patching For The Time Being",MB_OK|MB_ICONHAND);
        return FALSE;
    }
    
    if(b == FALSE)
    {
        LOG(E_BSP,"Error BSP1023: Bad geometry detected. Please review the geometry");
        return FALSE;
    }
    
    if(bLog)
    {
        LOG(E_BSP,"BSP: Ouput summary results for BSP %s: ",_name);
        LOG(E_BSP, "BSP: Polygons: %d, Planes: %d, Nodes: %d, Leafs: %d", _polys.size(),
           _planes.size(),
           _nodesPtr.size(),
           _leafs.size());
        /*
        FOREACH(vvector<CLeaf*>, _leafs, pp)
        {
            LOG(E_BSP, "-Leaf: %03d has %03d polyons\r\n",(*pp)->_leafIdx,(*pp)->_nPolys);
        }
        */
            
    }
    Finalize();
    return b;
}

//--| CBspTree::UpdateNodeBB|-------------------------------------------------------------
void CBspTree::UpdateNodeBB(CMiniNode* pNode, vvector<Poly>& polys)
{
	MiniBsp::UpdateNodeBB(pNode, polys);
}

//--| CBspTree::UpdateNodeBB|-------------------------------------------------------------
void CBspTree::UpdateNodeBB(CMiniNode* pNode, list<Poly>& polys)
{
	MiniBsp::UpdateNodeBB(pNode, polys);  
}

//--|    CBspTree::LinkItem|--------------------------------------------------------------
void    CBspTree::LinkItem(SceItem* pItem, BOOL atLeaf)
{
    R_LinkItem(0, pItem);
}

//--|    CBspTree::R_LinkItem|------------------------------------------------------------
void    CBspTree::R_LinkItem(int node, SceItem* pItem)
{
    CMiniNode* pNode = _nodesPtr[node];
    
    if(pNode->IsLeaf()) // leaf
    {
        if(!pNode->IsSolid())
        {
            pItem->_leafIdx = pNode->_leafIdx;
        }
        return;
    }
    
    int side = pNode->GetPlane().GetSide(pItem->_t);
    
    if(side > 0 && pNode->_nodeidx[N_FRONT]>=0) // && item out of BSP
        R_LinkItem(pNode->_nodeidx[N_FRONT], pItem);
    else if( pNode->_nodeidx[N_FRONT]>=0)       // && item out of BSP
        R_LinkItem(pNode->_nodeidx[N_BACK], pItem);
}


//--|    CBspTree::LinkDetailModel|-------------------------------------------------------
void    CBspTree::LinkDetailModel(CBspTree* pModelTree, BOOL atLeaf)
{
    _pSubModels << pModelTree;
    if(atLeaf)
        R_LinkDetailModel(0, pModelTree);
}

//--|    CBspTree::R_LinkDetailModel|-----------------------------------------------------
void    CBspTree::R_LinkDetailModel(int node, CBspTree* pModelTree)
{
    CMiniNode* pNode = _nodesPtr[node];
    
    if(pNode->IsLeaf()) // leaf
    {
        if(!pNode->IsSolid())
        {
            ((CLeaf*)pNode)->_iModIndex.push_back(pModelTree->_thisIdx);
        }
        return;
    }
    
    Box& box = pModelTree->Root()->_bbox;
    
    DWORD rp = ClassifyBox(pNode->GetPlane(), box);
    
    if(rp & PON_FRONT && pNode->_nodeidx[N_FRONT]!=-1) // if model get's out
      R_LinkDetailModel(pNode->_nodeidx[N_FRONT], pModelTree);
    
    if(rp & PON_BACK && pNode->_nodeidx[N_BACK]!=-1)    
      R_LinkDetailModel(pNode->_nodeidx[N_BACK], pModelTree);
    
}


//--|  CBspTree::R_FindLeaf|--------------------------------------------------------------
int  CBspTree::R_FindLeaf(int nodeidx, V3& pos)
{
	if(nodeidx == -1)
		return -1;
    CMiniNode* pNode = _nodesPtr[nodeidx];
    
    if(pNode->IsLeaf()) // leaf
    {
        if(!pNode->IsSolid())
        {
            return ((CLeaf*)pNode)->_leafIdx;
        }
        return -1;
    }
    
    int side = pNode->GetPlane().GetSide(pos);
    
    if(side > 0)
        return R_FindLeaf(pNode->_nodeidx[N_FRONT], pos);
    else
        return R_FindLeaf(pNode->_nodeidx[N_BACK], pos);
}


//--|   CBspTree::AddUniquePlane|---------------------------------------------------------
int   CBspTree::AddUniquePlane(vvector<Plane>&  allPlanes, map<int,int>& reindexMap)
{
    int nIndex = 0;
    FOREACH(vvector<Plane>, _planes, pPlane)
    {
        Plane& rPlane = *pPlane;
        
        vvector<Plane>::iterator fp = allPlanes.findelement(rPlane);
        
        if(fp == allPlanes.end())
        {
            reindexMap[nIndex] = allPlanes.size();
            allPlanes << rPlane;
        }else
        {
            reindexMap[nIndex] = fp - allPlanes.begin();
        }
        nIndex++;
    }
    return 0;
}

//--|    CBspTree::ReindexPlanes|---------------------------------------------------------
void    CBspTree::ReindexPlanes(map<int,int>& pidxes)
{
    FOREACH(vvector<CMiniNode*>, _nodesPtr,ppNode)
    {
        CMiniNode* pNode = *ppNode;
        if(pNode->_planeIdx>=0)
          pNode->_planeIdxFinal = pidxes[pNode->_planeIdx];
        else
            pNode->_planeIdxFinal = -1; // DUMMY-TREE
    }
    
    FOREACH(vvector<Poly>,  _polys, pPoly)
    {
        Poly& rP = *pPoly;
        if(rP._planeIdx>=0)
          rP._planeIdxFinal = pidxes[rP._planeIdx];
        else
            rP._planeIdxFinal = -1;// DUMMY-TREE
    }
    
    FOREACH(vvector<CLeaf*>, _leafs, ppLeaf)
    {
        CLeaf* pLeaf = *ppLeaf;
        if(pLeaf->_planeIdx>=0)
          pLeaf->_planeIdxFinal = pidxes[pLeaf->_planeIdx];
        else
            pLeaf->_planeIdxFinal = -1;// DUMMY-TREE
    }
    
    if(_prtProc)
    {
        FOREACH(vvector<Portal>, _prtProc->_portals, pPort)
        {
            Portal& rP = *pPort;
            if(rP._planeIdx>=0)
              rP._planeIdxFinal = pidxes[rP._planeIdx];
            else
                rP._planeIdxFinal = -1;// DUMMY-TREE
        }
    }
    
}

//--|  CBspTree::CollectPNL|--------------------------------------------------------------

struct S_SortByTexture
{
    /*
    bool operator()(Poly* p1, Poly* p2)
    {
        LONG texA[4]    = { p1->_textures[0] > 0 ? p1->_textures[0] * (1000000000000) : 0,
                            p1->_textures[1] > 0 ? p1->_textures[0] * (100000000) : 0,
                            p1->_textures[2] > 0 ? p1->_textures[0] * (10000) : 0,
                            p1->_textures[3] > 0 ? p1->_textures[0] * (1) : 0};
        LONG   texB[4]  = { p2->_textures[0] > 0 ? p2->_textures[0] * (1000000000000) : 0,
                            p2->_textures[1] > 0 ? p2->_textures[0] * (100000000) : 0,
                            p2->_textures[2] > 0 ? p2->_textures[0] * (10000) : 0,
                            p2->_textures[3] > 0 ? p2->_textures[0] * (1) : 0};
        return (texA < texB);
    }
    */

};

//--|  CBspTree::CollectPNL|--------------------------------------------------------------
int  CBspTree::CollectPNL(vvector<Poly*>&        allPolys,
                          vvector<CMiniNode*>&   allNodes,
                          vvector<CLeaf*>&       allLeafs)
{
    _polyStartIdx = allPolys.size();
    _nodeStartIdx = allNodes.size();
    _leafStartIdx = allLeafs.size();
    
    // collect polys, nodes and leafs. when save add the index with the start indexa as long
    // they are ofset in th allXXx collection
    FOREACH(vvector<Poly>,  _polys, pPoly)
    {
        Poly& rP = *pPoly;
        allPolys << &rP;
    }
    
    FOREACH(vvector<CMiniNode*>, _nodesPtr,ppNode)
    {
        CMiniNode* pNode = *ppNode;
        allNodes << pNode;
    }
    
    FOREACH(vvector<CLeaf*>, _leafs, ppLeaf)
    {
        CLeaf* pLeaf = *ppLeaf;
        allLeafs << pLeaf;

        // sort polys by texture in each leaf
        vvector<Poly*>::iterator t0S = allPolys.begin() + pLeaf->_polyIdx + _polyStartIdx;
        vvector<Poly*>::iterator t0E = allPolys.begin() + pLeaf->_polyIdx + _polyStartIdx + pLeaf->_nPolys;

        //std::sort(t0S, t0E, S_SortByTexture);
    }
    return 0;
}

//--|    CBspTree::SegmentIntersect|------------------------------------------------------
BOOL    CBspTree::SegmentIntersect(V3& a, V3& b, BOOL bSubModels, CollPack& coll)
{
    Box       segBox;
    REAL       dTmp;
    
    if(_treeprops & MODEL_NOCOLIDE)
    {
        return FALSE;
    }

  
    _userFlag  = FALSE; // collpack set
    V3   iP    = b;  // keep this for submitting it to models
    coll._ip   = b;  // last ip is the end. keep this
    coll._dist = INFINITE;
    coll._pTree=0;
    BOOL bRet = R_SegmentIntersect(a, b, 0, coll);
    //
    // we always go forward. This Segitersect is not only for light
    //
    if(/*!bRet &&  */bSubModels && _pSubModels.size())
    {
        CollPack   best;
        
        FOREACH(vvector<CBspTree*>, _pSubModels, ppSubBsp)
        {
            CBspTree* pBsp = (*ppSubBsp);
            
            //
            // ignore dynamic models for now
            //
            if(pBsp->_treeprops & MODEL_DYNAMIC)
                continue;

			if(pBsp->_treeprops & MODEL_NOCOLIDE)
                continue;

			if(pBsp->_treeprops & MODEL_HIDDEN)
				continue;

			if(pBsp->_treeprops & MODEL_IS_SKYDOM)
				continue;

			if(GET_CONTENT(pBsp->_treeprops) == CONT_PLASMA || 
               GET_CONTENT(pBsp->_treeprops) == CONT_GAS)
				continue;


            
            segBox.FromSegment(a,iP);                          // do a fast seg box intersection
            if(segBox.IsTouchesBox(pBsp->GetNode(0)->_bbox))
            {
                if(!pBsp->SegmentIntersect(a, iP, 0/*no submodels*/, best))
                {
                    continue;
                }
                //
                // keep the best ip
                //
                bRet = TRUE;
                dTmp = vdist(best._ip, a);
                if(dTmp < coll._dist)
                {
                    coll       = best;
                    coll._dist = dTmp;
                    coll._pTree = pBsp;
                }
            }
        }
    }
    return bRet ;
}

//--|    CBspTree::R_SegmentIntersect|----------------------------------------------------
BOOL    CBspTree::R_SegmentIntersect(V3& a, V3& b, int nodeIdx, CollPack& col)
{
    #pragma message("happend with lmap  128x128 and lumelsz = 8")
    if(nodeIdx < 0 || nodeIdx > BOGUS_VALUE)
    {
        return 1;
    }
    BOOL        rVal = 0;
    CMiniNode* pNode  = _nodesPtr[nodeIdx];
    if(pNode->IsLeaf())
    {
        BOOL isSolid = pNode->IsSolid();
        if(_treeprops  & MODEL_IS_TERRAIN && Compiler::PCompiler->_bAABBTreeCsgTer)
        {
            assert(pNode->_polyIdx>=0);
            if(pNode->_polyIdx<0)
            {
                return 1;
            }
            return SegmentIntersectPolygons(&_polys[pNode->_polyIdx], pNode->_nPolys, a, b, col);
        }
        
        if(_treeflags & BRSH_DONTBSP)
        {
            ASSERT(pNode->_polyIdx>=0);
            ASSERT(pNode->_nodeflags  &  NODE_LEAF);
            if(_polys.size())
                return SegmentIntersectPolygons(&_polys[0], pNode->_nPolys, a, b, col);
        }
        if(_btextconvex && !isSolid)
        {
            //return SegmentIntersectPolygons(&_polys[0], pNode->_nPolys, a, b, col);
            return SegmentIntersectConvexLeaf(pNode, a, b, col);
        }
        col._nLeaf = pNode->_leafIdx;
        col._pTree = this;
        col._ip    = a;
        return isSolid;
    }

    Plane& plane = _planes[pNode->_planeIdx];
    REAL   da    = plane.DistTo(a);
    REAL   db    = plane.DistTo(b);
    
    if(da > -EPSILON  && db > -EPSILON )
        return R_SegmentIntersect(a, b, pNode->_nodeidx[N_FRONT],col);
    else if(da < EPSILON  && db < EPSILON )
        return R_SegmentIntersect(a, b, pNode->_nodeidx[N_BACK],col);
    
    int  side   = da >= 0;
    REAL dist   = da / (da - db);
    V3  midPct  = a+(b-a) * dist;

    if(R_SegmentIntersect(a, midPct, pNode->_nodeidx[side],col))
         return TRUE;
    else if (R_SegmentIntersect(midPct, b, pNode->_nodeidx[!side],col))
    {
        if(!_userFlag )// collpack set
        {
            _userFlag   = TRUE;
            col._ip     = midPct;
            col._pPlane = &plane;
            col._pTree  = this;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL    CBspTree::SegmentIntersectConvexLeaf(CMiniNode* pNode, V3& a, V3& b, CollPack& col)
{
    BOOL        hit = 0;
    set<int>    indexes;
    REAL        d2p = INFINIT;
    Poly*       pStart = &_polys[pNode->_polyIdx];// pNode->_nPolys
    int         nCount = pNode->_nPolys;
    for(int i=0;i<nCount;i++,pStart++)
    {
        Plane& plane = _planes[pStart->_planeIdx];
        if(indexes.find(pStart->_planeIdx) == indexes.end())
        {
            REAL da = plane.DistTo(a);
            REAL db = plane.DistTo(b);
            if(db < -1.0)
            {
                REAL ratio   = da / (da - db);
                V3   midPct  = a+(b-a) * ratio;
                REAL dist    = vdist(a, midPct);
                if(dist < d2p)
                {
                    //_userFlag   = TRUE; // collfound
                    col._pPlane = &plane;       
                    col._pTree  = this;
                    col._ip     = midPct;
                    col._dist   = dist;
                    d2p         = dist;
                    hit         = 1;
                }
            }
            indexes.insert(pStart->_planeIdx);
        }
    }
    return hit;
}

//--| CBspTree::SegmentIntersectPolygons|-------------------------------------------------
BOOL CBspTree::SegmentIntersectPolygons(Poly* pStart,
                                       int nCount,
                                       V3& a,
                                       V3& b,
                                       CollPack& col, BOOL faces)
{
    BOOL            bHit = 0;
    V3 vdir = a;    vdir-=b; vdir.norm();
    Plane           plane;
    REAL            minDist = INFINIT;
    
    for(int i=0;i<nCount;i++,pStart++)
    {
        if(pStart->_planeIdx>=0) 
        {
            plane = _planes[pStart->_planeIdx];
        }
        else
        {
            assert(0);
            plane._n = pStart->_n;
            plane._c = pStart->_c;
        }

        REAL da = plane.DistTo(a);
        REAL db = plane.DistTo(b);
        
        if(da >= 0 &&  db < -2.0)
        {
            REAL ratio   = da / (da - db);
            V3   midPct  = a+(b-a) * ratio;
            REAL dist    = vdist(a, midPct);

            if(faces)
            {
                if(pStart->IsInnerPoint(midPct))
                {
                    if(dist < minDist)
                    {
                        minDist = dist;

                        if(pStart->_planeIdx>=0)
                            col._pPlane = &_planes[pStart->_planeIdx];
                        else
                            col._pPlane = 0;   // DUMMY TREE
                        col._pPoly  = pStart;
                        col._pTree  = this;
                        col._ip     = midPct;
                        bHit        = 1;
                        _userFlag   = 1;
                    }
                }
            }
            else
            {
                if(dist < minDist)
                {
                    minDist     = dist;
                    col._pPlane = &_planes[pStart->_planeIdx];
                    col._pTree  = this;
                    col._ip     = midPct;
                    bHit        = 1;
                    _userFlag   = 1;
                }
            }
        }
    }
    return bHit;
}

//--| CBspTree::ConcaveConvex|------------------------------------------------------------
void CBspTree::ConcaveConvex()
{
    return;//
    /**
    // empty convex (remove all nodes)
    if(_leafs.size()==1)
    {
        _props |= MODEL_CONVEX_EMPTY;
    }
    if(_leafs.size() == this->_polys.size())
    {
        _props |= MODEL_CONVEX_SOLID;
    }
    
    if(_props & MODEL_CONVEX_SOLID ||
      _props & MODEL_CONVEX_EMPTY)
    {
        //delete nodes
        _nodesPtr.deleteelements();
        _leafs.clear();
        
        //add new root
        AddNode(new CMiniNode(this));
        UpdateNodeBB(Root(), _polys);
        
        //
        // create front node. place all trees plygons here
        // there is no need to walk to bottom as lkong
        // all are facing in
        //
        CMiniNode* fn = CreateNode(NODE_LEAF);
        Root()->_nodeidx[N_FRONT] = _nodesPtr.size();
        fn->_polyIdx    =  0;
        fn->_nPolys     =  _polys.size();
        fn->_idxParent           =  0;
        UpdateNodeBB(fn, _polys);
        AddNode(fn);
        
        //
        // create back node witch is solid
        //
        CMiniNode* bn = CreateNode(NODE_LEAF|NODE_SOLID);
        Root()->_nodeidx[N_BACK]=_nodesPtr.size();
        bn->_idxParent = 0;
        AddNode(bn);
    }
    */
        
    
}
//PROP_PATCHES
//--| void CutByWalkingPlane|-------------------------------------------------------------
static void CutByWalkingPlane(Poly& rp, REAL treshVal, vvector<Poly>& outPolys)
{
    V3      majorAx      = GetMaxAx(rp._n);
    Box     box          = rp._box;
    REAL    coverExtends = box.GetMaxExtend() * 2.200f;

    V3      vStart  =   rp._vtci[0]._xyz;
    V3      d1      =   (rp._vtci[1]._xyz - rp._vtci[0]._xyz).norm();
    V3      d2      =   Vcp(d1, rp._n) ;
    
    /** Debug points for patching the polygons
        for(REAL x = 0 ; x < coverExtends; x+=treshVal)
    {
        for(REAL y = 0 ; y < coverExtends; y+=treshVal)
        {
            
            V3 dp = vStart + (d1*x) + (d2*y);
            
            DOC()->_testPoints << dp;
            
        }
    }
    
    */
        
    Plane           divPlane;
    vvector<Poly>   frags;
    Poly            toCut = rp;
    
    for(REAL x = 0 ; x < coverExtends; x+=treshVal)
    {
        V3      dp = vStart + (d1*x);
        Poly    pf;
        Poly    pb;
        
        //DOC()->_testPoints << dp;
        
        divPlane.Set(d1, dp);
        
        toCut.Split(divPlane, pf, pb, TRUE);
        
        if(pb._vtci.size()>=3)
        {
            pb._polyflags |= POLY_SEP_LMAP;
            frags << pb;
        }
        
        if(pf._vtci.size()>=3)
        {
            toCut.Clear();
            toCut = pf;
        }
        else
        {
            break;
        }
    }
    
    // now cut all frags
    FOREACH(vvector<Poly>, frags, p2c)
    {
        toCut = *p2c;
        
        for(REAL x = 0 ; x < coverExtends; x+=treshVal)
        {
            V3      dp = vStart + (d2*x);
            Poly    pf;
            Poly    pb;
            
            //DOC()->_testPoints << dp;
            
            divPlane.Set(d2, dp);
            
            toCut.Split(divPlane, pf, pb, TRUE);
            if(pb._vtci.size()>=3)
            {
                pb._polyflags |= POLY_SEP_LMAP;
                outPolys << pb;
            }
            if(pf._vtci.size()>=3)
            {
                toCut.Clear();
                toCut = pf;
            }
            else
            {
                break;
            }
        }
    }
}

//--|    CBspTree::PatchPolys|------------------------------------------------------------
void    CBspTree::PatchPolys(vvector<Poly>& inPolys, vvector<Poly>& outPolys)
{
    #ifdef _DEBUG
        DOC()->_testPoints.clear();
    #endif //
    
    // tmax xy dimension unpached -is tmax lmap size on that dim x  _lMapLumelSz
    REAL    maxDimPoly = Compiler::PCompiler->_lmapMaxSize * Compiler::PCompiler->_lMapLumelSz;
    
    FOREACH(vvector<Poly>, inPolys, pp)
    {
        if(Compiler::PCompiler->_bSnapMainBsp)
        {
            pp->SnaptoPoint(SnapToXY);
        }

        if(pp->_box.GetMaxExtend() > maxDimPoly && pp->_polyprops & FACE_LM_PATCH)
        {
            CutByWalkingPlane(*pp, maxDimPoly, outPolys);
            continue;
        }
        outPolys<<*pp;continue;
    }
}

static GLUquadricObj* go;

//--|    CBspTree::Draw2GlTree|------------------------------------------------------------
void    CBspTree::Draw2GlTree(CZ_ed2View* pV, DWORD mode, int leafNo, BOOL bvisible)
{
    Ta.Disable();
	int nCount = 0;
    int nLeaf = 0;
    CLR leafcolor = ZRED;
	FOREACH(vvector<CLeaf*>, _leafs, pNode)
	{
		Poly*   pPoly = (*pNode)->GetPolys(nCount);

        if(mode & (C_FULL|C_LMAP|C_GRAPH|C_ZONES))
        {
		    if(pPoly->_polyflags & POLY_HIDDEN || !bvisible)			
		    {
			    glColor4ubv(CLR_DARK);                
		    }
		    else
		    {
			    glColor4ubv(CLR_WHITE);                
		    }
        }
        else if(IsMainModel())
        {
            if(mode & C_LBYL)
            {
                if(nLeaf == leafNo)
                    glColor4ubv(CLR_WHITE);               
                else
                    glColor4ubv(CLR_DARK);                
            }
            else if(mode & C_LEAFS)
            {
                int treebits = nLeaf & 0x1F;
                
                if(treebits & 0x10)
                {
                    leafcolor.r = !!(treebits&1)*80;
                    leafcolor.g = !!(treebits&2)*80;
                    leafcolor.b = !!(treebits&4)*80;
                }
                else if(treebits & 0x8)
                {
                    leafcolor.r = !!(treebits&1)*162;
                    leafcolor.g = !!(treebits&2)*162;
                    leafcolor.b = !!(treebits&4)*162;
                }
                else
                {
                    leafcolor.r = !!(treebits&1)*250;
                    leafcolor.g = !!(treebits&2)*250;
                    leafcolor.b = !!(treebits&4)*255;
                }
                glColor4ubv(leafcolor);  
            }
        }
        else
        {
            glColor4ubv(CLR_NORMAL);               
        }

		for(int i=0;i<nCount;i++, pPoly++)
		{
			glBegin(GL_LINE_LOOP);
			PVertex vb = pPoly->_vtci.begin();
			PVertex ve = pPoly->_vtci.end();
			for(;vb!=ve;vb++)
			{
				Vtx2& vtci = *vb;
				glVertex3fv((REAL*)vtci._xyz);
			}
			glEnd();
		}
        ++nLeaf;
	}
    Ta.Restore();
}

//--|    CBspTree::Draw3DBSPTree|------------------------------------------------------------
static BOOL	Local_IsLeafVisFromLeaf(vvector<CLeaf*>& leafs, int l1, int l2, BYTE* ppvs)
{
	if(l1<0||l2<0)return 1;
    if(ppvs)
    {
        BYTE* pPvs1 = &ppvs[leafs[l1]->_pvsIdx];
        return (pPvs1[l2 >> 3] & (1<<(l2 & 7)) )!=0;
    }
    return 1;
}

//--|    CBspTree::Draw_Graph|------------------------------------------------------------
static int NDepth;
void    CBspTree::Draw_Graph(z_ed3View* pV)
{
    int node = 0 ;
    V3  pos  = V3(0.,400.,0.);
    int dir = 0;
    NDepth = 0;
    
    Ta.Disable();

    go = gluNewQuadric();
    Sign=0;
    R_DrawGraph(0, pos, dir, pV);
    if(IsMainModel())
    {
        pV->TextOut(pos, MKSTR("BSP: %d",_thisIdx) ,ZYELLOW);
    }
    else
    {
        pV->TextOut(pos, MKSTR("Model: %d",_thisIdx),ZYELLOW);
    }
    gluDeleteQuadric(go);
    Ta.Restore();
}
//--|    CBspTree::|------------------------------------------------------------
//--|    CBspTree::|------------------------------------------------------------
void    CBspTree::DrawPolys(Poly* pPoly,int ncount, DWORD mode,    CLR* leafcolor,
                                                                       vvector<Poly*>* transPolys,
                                                                       vvector <Poly*>* backDraw,
                                                                       vvector <Poly*>* mirrors,
                                                                       vvector <Poly*>* masked,
                                                                       vvector <Poly*>* guround)

{
    BOOL bind;

    for(int i=0;i<ncount;i++, pPoly++)
    {
        bind = 0;
		pPoly->_polyflags &= ~POLY_HIDDEN;

        if(pPoly->_polyprops & FACE_MIRROR)
        {
            mirrors ? mirrors->push_back((Poly*)pPoly):0;
            continue;
        }
        if(pPoly->_polyprops & FACE_SHOWBACK)
        {
            backDraw ? backDraw->push_back((Poly*)pPoly):0;
            continue;
        }
        if(pPoly->_polyprops & FACE_TRANSPARENT)
        {
            transPolys ? transPolys->push_back((Poly*)pPoly):0;
            continue;
        }

        if(pPoly->_polyprops & FACE_BLACKMASK)
        {
            masked ? masked->push_back((Poly*)pPoly):0;
        }

        if(pPoly->_polyprops & FACE_GSHADE)
        {
            guround ? guround->push_back((Poly*)pPoly):0;
            continue;
        }
        

        if(mode & C_WHITE || mode & C_BYFAR)
        {
            DOC()->DrawPolyVertexes(GL_POLYGON, *pPoly, 0);
        }
        else
        {

            if((mode & C_LEAFS) && leafcolor)
            {
                leafcolor->r-=5;
                leafcolor->b-=5;
                glColor4ubv((BYTE*)*leafcolor);
                DOC()->DrawPolyVertexes(GL_POLYGON, *pPoly, 1);
            }
            else
            {
                glColor4ubv((BYTE*)pPoly->_colorD);
                Texs& tex = pPoly->GetTexs();
                if(mode & C_LMAP)
                {
				    if(pPoly->_polyprops & FACE_FULLBRIGHT)
					    bind = Ta.Bind(_wLmap);
				    else
                    {
                        if(pPoly->_polyprops & FACE_NOLIGTMAP) 
                        {
                            __noop;
                        }
                        else
                        {
                            if(pPoly->_polyprops & FACE_CUSTLM)
                                bind=Ta.Bind(tex._texts[1], tex._texApply[1]);
                            else
    					        bind=Ta.Bind(pPoly->_ilmTex, 0);
                        }
                    }
                    if(bind)
                    {
                        DOC()->DrawPolyVertexes(GL_POLYGON, *pPoly, 1);
                    }
                }
                else
                {
                    BOOL binded;
                    
                    for(int ts=0;ts<4;++ts)
                    {
                        binded=0;
                    
                        switch(ts)
                        {
                            case 0:
                                Ta.Bind(tex._texts[0], tex._texApply[0]);
                                DOC()->DrawPolyVertexes(GL_POLYGON, *pPoly, 0);
                                break;
                            case 1:
                                if(pPoly->_polyprops & FACE_FULLBRIGHT)
                                {
				                    Ta.Bind(_wLmap, 0);
                                    binded=1;
                                }
				                else
                                {
                                    if(pPoly->_polyprops & FACE_NOLIGTMAP)
                                    {
                                        __noop;
                                    }
                                    else
                                    {
                                        if(pPoly->_polyprops & FACE_CUSTLM && pPoly->GetHtex(1))
                                        {
                                            Ta.Bind(tex,1);
                                            binded=1;
                                        }
                                        else if(pPoly->_ilmTex)
                                        {
    					                    Ta.Bind(pPoly->_ilmTex, 0);
                                            binded=1;
                                        }
                                    }
                                }
                                if(binded)
                                {
                                    TexRef::GlLmapMode(1);
                                    DOC()->DrawPolyVertexes(GL_POLYGON, *pPoly, 1);
                                    TexRef::GlLmapMode(0);
                                }
                                break;
                            case 2:
                            case 3:
                                if(Ta.Bind(tex,ts))
                                {
                                    TexRef::GlDetailMode(1);
                                    DOC()->DrawPolyVertexes(GL_POLYGON, *pPoly, ts);
                                    TexRef::GlDetailMode(0);
                                }
                                break;
                        }
                    }
                }
            }            
        }
    }

}

//--|    CBspTree::|------------------------------------------------------------
void    CBspTree::Draw_BSP_WithLmapsAndTextures(z_ed3View* pV, int& leaf, int curLeaf, DWORD mode)
{
    BOOL            frustCull = 0;
    int             nCount;
    vvector <Poly*> transPolys;         // draw these last
    vvector <Poly*> backDraw;
    vvector <Poly*> mirrors;
    vvector <Poly*> masked;
    vvector <Poly*> guround;

    glDisable(GL_BLEND);

    CLR     leafcolor = ZRED;
    BOOL    pvsHidden ;
    int     lfCount = 1;


    FOREACH(vvector<CLeaf*>, _leafs, pNode)
    if((*pNode)->_nodeflags & LEAF_HAS_OCCLUDERS)
    {
        // add them to camera antifrustrum
        Poly*   pPoly = (*pNode)->GetPolys(nCount);
        for(int i=0;i<nCount;i++, pPoly++)
        {
		    if(pPoly->_polyprops & FACE_OCCLUDER)
            {
                DOC()->_cam.AddOccluder(*pPoly);
            }
        }
    }

    // PVS the leafs
    FOREACH(vvector<CLeaf*>, _leafs, pNode)
    {
        ++lfCount;
        if(((lfCount & 0x8)==0) || ((lfCount & 0x10)==0) )++lfCount;
        if(mode & C_LEAFS && IsMainModel())
        {
            int treebits = lfCount & 0x1F;
            
            if(treebits & 0x10)
            {
                leafcolor.r = !!(treebits&1)*80;
                leafcolor.g = !!(treebits&2)*80;
                leafcolor.b = !!(treebits&4)*80;
            }
            else if(treebits & 0x8)
            {
                leafcolor.r = !!(treebits&1)*162;
                leafcolor.g = !!(treebits&2)*162;
                leafcolor.b = !!(treebits&4)*162;
            }
            else
            {
                leafcolor.r = !!(treebits&1)*250;
                leafcolor.g = !!(treebits&2)*250;
                leafcolor.b = !!(treebits&4)*255;
            }
        }

	    pvsHidden  = FALSE;

        
	    if(curLeaf >= 0 && (*pNode)->_pvsIdx >=0 && (*pNode)->_leafIdx != curLeaf)
	    {
            DWORD zonevis = _leafs[curLeaf]->_zonearea;
            DWORD nodezone = (*pNode)->_zonearea;
            if(!(nodezone & zonevis))
            {
                pvsHidden = TRUE;
            }
            else
            {
		        if(!Local_IsLeafVisFromLeaf(_leafs, (*pNode)->_leafIdx, curLeaf,Compiler::PCompiler->_pvsPrc._pvs))
                {
			        pvsHidden =TRUE;
                }
            }
            frustCull =  (GFrustCull &&   DOC()->_cam.IsCulled(&(*pNode)->_bbox._min,&(*pNode)->_bbox._max));
            // frust cull leafs and nodes
	        if(pvsHidden || frustCull)
	        {
	            Poly*   pPoly = (*pNode)->GetPolys(nCount);
		        for(int i=0;i<nCount;i++, pPoly++)
		        {
			        pPoly->_polyflags |= POLY_HIDDEN;
		        }
		        continue;
	        }

	    }

        glColor4ubv(CLR_WHITE);
        Poly*   pPoly = (*pNode)->GetPolys(nCount);
        DrawPolys(pPoly, nCount, mode,  &leafcolor,
                                        &transPolys,         // draw these last
                                        &backDraw,
                                        &mirrors,
                                        &masked,
                                        &guround);
    }
    glColor4ubv(CLR_WHITE);
    DOC()->DrawSpecialPolygons(pV,  1, backDraw, transPolys, mirrors, masked, guround);
}

//--|    CBspTree::Draw_SelectedLeafByLeaf|-----------------------------------------------
void    CBspTree::Draw_SelectedLeafByLeaf(z_ed3View* pV, int& leaf, int curLeaf, DWORD mode)
{
    CLeaf* pSelLeaf = 0;
    int    lfCount = 0;

    if(-1==curLeaf)
        curLeaf=0;


    glColor4ubv(CLR_WHITE);
    FOREACH(vvector<CLeaf*>, _leafs, pNode)
    {
        if(leaf == (*pNode)->_leafIdx)
        {
            pSelLeaf = (*pNode);
            continue;
        }
        else
        {
            glColor4ubv(CLR_DARK);
        }

        // draw selected leaf only
        int     nCount;
        Poly*   pPoly = (*pNode)->GetPolys(nCount);
        DrawPolys(pPoly, nCount, mode);
    }

    if(pSelLeaf )
    {
        glColor4ubv(CLR_WHITE);

        int     nCount;
        Poly*   pPoly = pSelLeaf->GetPolys(nCount);

        for(int i=0;i<nCount;i++)
        {
            if(!DOC()->_vmodefire)
                Ta.Bind((const TexCarrier&)*pPoly);
            DOC()->DrawPolyVertexes(GL_POLYGON, *pPoly, GUtex);
            ++pPoly;
        }
        glColor4ubv(CLR_WHITE);
    }
    if(leaf == _leafs.size())
        leaf=0;
}

//--|    CBspTree::Draw3DBSPTree|------------------------------------------------------------
void    CBspTree::Draw3DBSPTree(z_ed3View* pV, int& leaf, int curLeaf, DWORD mode)
{
    glColor4ubv(CLR_WHITE);
    DOC()->SetFaceMode();

    if(mode == C_GRAPH)
    {
        Draw_Graph(pV);
        return;        
    }
    if(mode & (C_LMAP|C_FULL|C_LEAFS)) // lmaps
    {
        CLR leafcolor = ZRED;
		if(_treeflags & BRSH_DONTBSP)
		{
			int nCount;
			vvector <Poly*> transPolys;         // draw these last
			vvector <Poly*> backDraw;
			vvector <Poly*> mirrors;
			vvector <Poly*> masked;
			vvector <Poly*> guround;

			Poly*   pPoly = Root()->GetPolys(nCount);
            DrawPolys(pPoly, nCount, mode,  &leafcolor,
                                            &transPolys,         // draw these last
                                            &backDraw,
                                            &mirrors,
                                            &masked,
                                            &guround);
			glColor4ubv(CLR_WHITE);
			DOC()->DrawSpecialPolygons(pV, 1,backDraw, transPolys, mirrors, masked, guround);
		}
		else
        {
			Draw_BSP_WithLmapsAndTextures(pV, leaf, curLeaf, mode);
        }
        return;
    }
    else  if(mode & C_LBYL)
    {
        Draw_SelectedLeafByLeaf(pV, leaf, curLeaf, mode);
    }
}

//----------------------------------------------------------------------------------------
#define DIST_SPHERE(rdt,dir) (((rdt-dir+1)*12))

//--|    CBspTree::R_DrawGraph|----------------------------------------------------------
void    CBspTree::R_DrawGraph(int node, V3 pos, int prevDir, z_ed3View* pV)
{
    ++NDepth;
    Sign=!Sign;
    
    CMiniNode* pNode = _nodesPtr[node];
    if(pNode->IsLeaf())
    {
        glPushMatrix();
        {
            glTranslatef(pos.x, pos.y, pos.z);
            if(pNode->_selected==0)
                glColor4ub(96,96,96,255);
            else
                glColor4ub(192,192,192,255);
            if(pNode->IsSolid())
            {
                if(pNode->_selected==0)
                {
                    pV->TextOut(V0, MKSTR("  x%d",node ) , CLR(255,255,0));
                    glColor4ub(128,0,0,255);
                }
                else
                {
                    pV->TextOut(V0, MKSTR("  x%d",node) , CLR(192,0,192));
                    glColor4ub(255,0,0,255);
                }
            }
            else
            {
                if(pNode->_selected==0)
                {
                    pV->TextOut(V0, MKSTR("  %d/%d ",pNode->_leafIdx, pNode->_nPolys) , ZYELLOW);
                    glColor4ub(0,128,96,255);
                }
                else
                {
                    pV->TextOut(V0, MKSTR("  %d/%d ",pNode->_leafIdx, pNode->_nPolys) , CLR(192,0,192));
                    glColor4ub(0,255,255,255);
                }
            }
            gluSphere(go, 2.0,6,6);
        }
        glPopMatrix();
        --NDepth;
        return;
    }    
    V3 lr;
    
    if(pNode->_nodeidx[0]>=0) // dir -1
    {
        lr = pos;
        int rdw = _reachedDepth*2 - 12;
        int sign = rdw%2?1:-1;
        int depl = rdw - NDepth;

        lr.y -= (rdw - NDepth) * 32;
        lr.x -= (rdw - NDepth) * 16;
        lr.z += (Sign * depl) * 24;
        
        
        if(pNode->_selected==0)
            glColor4ub(128,0,0,254);
        else
            glColor4ub(255,255,0,254);


        glBegin(GL_LINES);
        glVertex3f(pos.x,pos.y,pos.z);
        glVertex3f(lr.x,lr.y,lr.z);
        glEnd();

        if(pNode->_selected==1)
            pV->TextOut(pos, MKSTR("%d",node) , ZYELLOW);
        else
            pV->TextOut(pos, MKSTR("%d",node) , CLR(192,0,192));

        R_DrawGraph(pNode->_nodeidx[0], lr, 32 , pV);
    }
    
    if(pNode->_nodeidx[1]>=0)// - > +1
    {
        lr = pos;

        int rdw = _reachedDepth*2 - 9;
        
        int depl = rdw - NDepth;
        lr.y -= (depl) * 32;
        lr.x += (depl) * 16;
        lr.z += (-Sign * depl) * 24;
        
        if(pNode->_selected==0)
            glColor4ub(0,128,0,254);
        else
            glColor4ub(0,255,0,254);
        
        glBegin(GL_LINES);
        glVertex3f(pos.x,pos.y,pos.z);
        glVertex3f(lr.x,lr.y,lr.z);
        glEnd();

        if(pNode->_selected==1)
            pV->TextOut(pos, MKSTR("%d",node) , ZGREEN);
        else
            pV->TextOut(pos, MKSTR("%d",node) , CLR(64,64,64));

        
        R_DrawGraph(pNode->_nodeidx[1], lr, 32 , pV);
    }
    --NDepth;
}

// cluster the nodes
void    CBspTree::Finalize()
{       
    //_cluster     
    _clusterCnt = 0;
    R_CreateLeafClusters(0);
}


void    CBspTree::R_CreateLeafClusters(int nNode)
{
    if(nNode==-1)
        return;
    CMiniNode* pNode = _nodesPtr[nNode];

    if(pNode->_leafIdx==-1) //still node
    {
        R_CreateLeafClusters(pNode->_nodeidx[0]);
        R_CreateLeafClusters(pNode->_nodeidx[1]);
        return;
    }

    CLeaf* pLeaf = _leafs[pNode->_leafIdx];
    if(pLeaf->IsSolid())
        pNode->_cluster = -1;
    else
        pNode->_cluster = _clusterCnt;
    ++_clusterCnt;
}
