// PortalPRC1.cpp: implementation of the CPortalPRC class.
//
//---------------------------------------------------------------------------------------

#include "stdafx.h"
#include "texadapter.h"
#include "z-edmap.h"
#include "PortalPRC.h"
#include "BspTree.h"
#include "scene.h"
#include "z_ed3View.h"
#include "compiler.h"
#include "DlgCompProgress.h"
#include "Mainfrm.h"
#include "z_ed2Doc.h"

//---------------------------------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------------------
struct Edge
{
    V3  edge[2];
    Edge(V3& a, V3& b){edge[0]=a;edge[1]=b;}
    Edge(){}
    Edge(const Edge& r){
        edge[0]=r.edge[0];
        edge[1]=r.edge[1];
    }
    Edge& operator = (const Edge& r){
        edge[0]=r.edge[0];
        edge[1]=r.edge[1];
        return *this;
    }
    BOOL operator == (const Edge& r){
        return (edge[0]==r.edge[0] && 
            edge[1]==r.edge[1]);
    }
};

// #define __MCO
//---------------------------------------------------------------------------------------
Box GetVxesBbox(vvector<V3>& vxes)
{
    Box b;
    FOREACH(vvector<V3>, vxes, pv)
        b.AddPoint(*pv);
    return b;
}

//---------------------------------------------------------------------------------------
BOOL    R_SearchLeaf(vvector<CMiniNode*>& nodes, CMiniNode* pStartNode, CMiniNode* p2Find)
{
    if(pStartNode->IsLeaf())
    {
        return (pStartNode == p2Find);
    }
    
    if(R_SearchLeaf(nodes, nodes[pStartNode->_nodeidx[0]], p2Find))
        return TRUE;
    if(R_SearchLeaf(nodes, nodes[pStartNode->_nodeidx[1]], p2Find))
        return TRUE;
    return FALSE;
}

//---------------------------------------------------------------------------------------
CMiniNode* R_FindCommonParent(vvector<CMiniNode*>& nodes,  
                              CMiniNode* pNotGo,
                              CMiniNode* pLUpper, CMiniNode* pSec)
{
    CMiniNode* p2Go;
    
    if(pNotGo == pLUpper->Front())
        p2Go = pLUpper->Back();
    else
        p2Go = pLUpper->Front();
    
    if(R_SearchLeaf(nodes, p2Go, pSec))
        return pLUpper;
    if(pLUpper->_idxNodeThis == 0)
    {
        if(R_SearchLeaf(nodes, pLUpper, pSec))
            return pLUpper;
    }
    return R_FindCommonParent(nodes,  pLUpper, nodes[pLUpper->_idxParent], pSec);
}






CMiniNode* FindCommonParent(vvector<CMiniNode*>& nodes,  CMiniNode* pLa, CMiniNode* pLb)
{
    return R_FindCommonParent(nodes,  pLa, nodes[pLa->_idxParent], pLb);
}

//---------------------------------------------------------------------------------------
void    Portal::Split(Plane& plane, Portal& a, Portal& b)
{
    a.CopyProps(*this);
    b.CopyProps(*this);
    
    
    if((_n == plane._n && _c == plane._c) ||
        (_n == -plane._n && _c == -plane._c) )
    {
#pragma message("see this")
        a._vxes = _vxes;
        b._vxes = _vxes;
        return;
    }
    
    V3  iv;
    V3 itxB = *_vxes.begin();
    V3 itxA = _vxes.back();
    
    REAL    fB;
    REAL	fA = plane.DistTo(itxA);
    
    FOREACH(vvector<V3>, _vxes, vxI)
    {
        itxB = *vxI;
        fB   = plane.DistTo(itxB);
        if(fB > GEpsilon)
        {
            if(fA < -GEpsilon)
            {
                REAL   t = -fA /(fB - fA);
                iv.interpolate(itxA,itxB,t);
                a._vxes << iv;
                b._vxes << iv;
            }
            a._vxes<<itxB;
        }
        else if(fB < -GEpsilon)
        {
            if(fA > GEpsilon)
            {
                REAL t = -fA /(fB - fA);           // t of segment
                iv.interpolate(itxA,itxB,t);
                a._vxes <<iv;
                b._vxes <<iv;
            }
            b._vxes<<itxB;
        }
        else
        {
            a._vxes << itxB;
            b._vxes << itxB;
        }
        itxA = itxB;
        fA   = fB;
    }
    if(b._vxes.size()<3){
        b._vxes.clear();
        assert(a._vxes.size() == _vxes.size());
    }
    if(a._vxes.size()<3){
        a._vxes.clear();
        assert(b._vxes.size() == _vxes.size());
    }
    
}

//---------------------------------------------------------------------------------------
void    PortalPRC::Clear()
{
    _testExterior = FALSE;
    _portals.clear();
}

//---------------------------------------------------------------------------------------
// build the new leafs tha will hold additional flags. we do not copy the
BOOL    PortalPRC::Process(CBspTree& tree, BOOL testExterior)
{
    LOG(E_PORT,"PRT: Processing portals");
    Clear();                        // clears the 'testExterior'
    _testExterior = testExterior;
    _pTree = & tree;
    if(BuildPortals())
    {
        if(0)
            MergeNarowLeafs();
        LOG(E_PORT,"PRT: %d portals were generated ",_portals.size()/2);
        return TRUE;
    }
    return FALSE;
}

//---------------------------------------------------------------------------------------
// builds initial portals that separate the leafs
BOOL PortalPRC::BuildPortals()
{
    int                  nCount = 0;    
    BOOL                 bret  = TRUE;
    vvector<Portal>      portals;
    vvector<CMiniNode*>& treeNodes = _pTree->GetNodes();
    int                  totNodes = treeNodes.size();
    int                  percent;
    _TRY
    {
        _portals.clear();
        FOREACH(vvector<CMiniNode*>, treeNodes, ppnode)
        {
            percent =  (100*nCount++)/ (totNodes + 1);
            PROG(percent);
            
            CMiniNode*	pNode = (*ppnode);
            
            DOC()->SetErrorPoint(pNode->_bbox.GetCenter());
            
            if(pNode->_nodeflags & NODE_LEAF)
                continue;
            
            if(pNode->Back()->IsSolid())
                continue;
            
            Portal            pPort(this);
            vvector<Portal>	  prtFragments;				
            if(CalculateInitialPortal(pPort, pNode))
            {
                R_ClipPortal(0, pPort, prtFragments);
                if(prtFragments.size())
                    append(portals, prtFragments);
            }
            
#pragma message("inital portal is zero")
            ////else
            ////	return FALSE;
            
            if (Compiler::PCompiler->HasBreaked())
                return FALSE;
        }
        if(portals.size()==0)
            return 1;
        
        if(bret = CleanObsolettePortals(portals))
        {
            ReduceVertexes(portals);
            _portals.reserve(portals.size()*2);
            bret = DuplicatePortals(portals, _portals);
        }
        
        
        if(bret==1 && _testExterior==1)
        {
            TestExteriorPortals();
            bret = 1;
            //bret = ReFlood();
        }
        
        

    }
    _CATCHX()
    {

        if(0==bret){
            LOGERR(E_PORT, V0, "Exception: Cannot Build Portals on given geometry");
            Compiler::PCompiler->_errDetected=1;
            bret =FALSE;
        }
    }
    
    DOC()->SetErrorPoint(V0);
    
    return bret ;
}

BOOL	PortalPRC::CalculateInitialPortal(Portal& portal, CMiniNode* pNode)
{
    Plane&  plane  = pNode->GetPlane();
    V3&		normal = plane._n;
    Box	bb         = _pTree->Root()->_bbox;//pNode->_bbox;				    	// bbox
    
    V3		bbCenter = bb.GetCenter();					// bbox center
    REAL	d2plan   = -plane.DistTo(bbCenter);			// dist to plane
    V3		portOrig = bbCenter + (normal * d2plan);	// plane center id bb center projeccted on plane
    V3		ax       = GetMinAx(plane._n);    		// get to see what wax we are using for rotation cp
    ax.norm();
    
    // build a polygon from plane
    V3		bbEx = (bb._max - bbCenter);				// bbox extends
    V3		uDr  = Vcp(ax, normal);						// to corner vectors;
    V3		vDr  = Vcp(uDr, normal);
    
    uDr		*= bbEx.getmax() ;
    vDr		*= bbEx.getmax() ;
    
    REAL vdp = Vdp(normal, ax);
    
    // enlarge the bogus polygon
    if(!IsZero(vdp,EPSMED))
    {
        uDr *= 8.0;
        vDr *= 8.0;
    }
    
    V3		corners[4];									// the 4 corners
    corners[0] = portOrig + (uDr-vDr);
    corners[1] = portOrig + (uDr+vDr);
    corners[2] = portOrig - (uDr-vDr);					// switch this to be able to rotate it
    corners[3] = portOrig - (uDr+vDr);
    
    // populate the portal structure
    portal._planeIdx = pNode->_planeIdx;
    portal._flags    = PORT_INITAL;
    
    portal._vxes.clear();
    for(int i=0; i < 4; i++)
        portal._vxes << corners[i];
    
    // see if bnew generated portal rely on the same plane it was generated (due)
    // 2 floating point errors
    GCalcNormal(&portal, portal._vxes[0],portal._vxes[1], portal._vxes[2]);
    if(0==portal._vxes.size())
        return FALSE;
    
    // cut initial portal with BSP bounding box
    CutPortalWithNodeBox(portal, _pTree->Root()->_bbox, 0);
    if(portal._vxes.size() == 0)
        return TRUE;
    
    switch(Compiler::PCompiler->_portalCutFlag)
    {
        case CUT_PORTAL_BY_NODE:
            {
                CMiniNode* pNodeP = pNode;//->Parent();
                CutPortalWithNodeBox(portal, pNodeP->_bbox);
            }
            break;
        
        case CUT_PORTAL_BY_PARENTS:
            CutPortalWithParents(portal, pNode);
            break;
        
        case CUT_PORTAL_BY_LEAFS:
            R_CutPortalWithLeafs(portal, pNode);
            break;
    }
    
    portal._n  = normal;
    portal._c  = plane._c;
    portal._planeIdxFinal = pNode->_planeIdxFinal; // store the depth
    return portal._vxes.size() >= 3;
    
}

//---------------------------------------------------------------------------------------
// csg this portal. add all between leafs in fragments.
void	PortalPRC::R_ClipPortalWithTree(int nodeIdx, Portal& pPortal, vvector<Portal>& portals)
{
    if(nodeIdx<0)
        return;
}

//---------------------------------------------------------------------------------------
void	PortalPRC::ClipWithLeafSides(CLeaf* pLeaf, Portal& portal)
{
    if(0)
    {
        set<int>     sides;
        Portal       ap((const Portal&)portal), bp;
    
        for(int i=pLeaf->_polyIdx; i < pLeaf->_polyIdx + pLeaf->_nPolys; i++)
        {
            Poly& poly = _pTree->_polys[i];
        
            if(sides.find(poly._planeIdx) == sides.end())
            {
                sides.insert(poly._planeIdx);
                portal.Split(poly, ap, bp);
                portal = ap;
                ap.Clear();
                bp.Clear();
            }
        }
    }
}

//---------------------------------------------------------------------------------------
// filter the portal into this BSP and collect surviving fragments
void	PortalPRC::R_ClipPortal(int nodeIdx, Portal& portal, 
                                vvector<Portal>& portals)
{
    
    if(nodeIdx<0)
        return;
    
    CMiniNode*  pNode   = _pTree->GetNode(nodeIdx);
    Plane&  plan		= pNode->GetPlane();
    REL_POS rp1			= portal.Classify(plan);
    
    switch (rp1)
    {
    case ON_PLANE:
        {
            vvector<Portal>   tmpPorts(8);  //  alloc 8 ptrs
            
            if(pNode->Front())
            {
                if(pNode->Front()->IsLeaf()) // send on front to be clipped 
                {
                    CLeaf* pLeaf = (CLeaf*)pNode->Front();
                    
                    // clip with leaf sides
                    ClipWithLeafSides(pLeaf, portal);
                    if(portal._vxes.size() >= 3)
                    {
                        portal.AddSideLeafIdx( pLeaf->_leafIdx);
                        tmpPorts.push_back(portal);
                    }
                }
                else
                {
                    R_ClipPortal(pNode->FontIdx(), portal, tmpPorts);
                }
            }
            
            if(tmpPorts.size())  // what have survived on front send on back
            {
                if(pNode->Back())
                {
                    if(pNode->Back()->IsLeaf())
                    {
                        append(portals, tmpPorts);
                        return;
                    }
                    
                    FOREACH(vvector<Portal>, tmpPorts, rp)
                    {	
                        R_ClipPortal(pNode->BackIdx(), (*rp), portals);
                    }
                }
            }
        }
        break;
    case ON_FRONT:
        if(!pNode->Front())
            break;
        if(pNode->Front()->IsNode())
        {
            R_ClipPortal(pNode->FontIdx(), portal, portals);
        }
        else
        {
            CLeaf* pLeaf = (CLeaf*)pNode->Front();
            ClipWithLeafSides(pLeaf, portal);
            if(portal._vxes.size()>=3)
            {
                portal.AddSideLeafIdx(pLeaf->_leafIdx);
                portals.push_back(portal);
            }
        }
        break;
    case ON_BACK:
        if(pNode->Back() && pNode->Back()->IsNode())
        {
            R_ClipPortal(pNode->BackIdx(), portal, portals);
        }
        break;
    case ON_SPLIT:
        {
            Portal pFp(this);
            Portal pBp(this);
            
            portal.Split(plan, pFp, pBp);
            portal._flags |= POLY_DELETED;
            if(pNode->Front())
            {
                if(pNode->Front()->IsNode())
                {
                    R_ClipPortal(pNode->FontIdx(), pFp, portals);
                }
                else
                {
                    CLeaf* pLeaf = (CLeaf*)pNode->Front();
                    ClipWithLeafSides(pLeaf, portal);
                    if(portal._vxes.size()>=3)
                    {
                        pFp.AddSideLeafIdx(pLeaf->_leafIdx);
                        portals.push_back(pFp);
                    }
                }
            }
            if(pNode->Back() && pNode->Back()->IsNode())
            {
                R_ClipPortal(pNode->BackIdx(),  pBp, portals);
            }
        }
        break;
    } 
    return;
    
}

//---------------------------------------------------------------------------------------
// the process builds duplicates. so eliminate them
BOOL	PortalPRC::CleanObsolettePortals(vvector<Portal>& prtFragments)
{
    int idx = 0;
AG:
    FOREACH(vvector<Portal>, prtFragments, pport)
    {
        if (Compiler::PCompiler->HasBreaked())
            return FALSE;
        
        Portal& portal = *pport;
        int        sz  = portal._sideLIdx.size();

        if(portal._flags & PORT_PROCESSED)
            continue;
        portal._flags|=PORT_PROCESSED;
        
        // erase any portal was split
        if(portal._flags & POLY_DELETED)
        {
            pport = prtFragments.erase(pport)-1;
            goto AG;
        }
        //erase any portal that does not separate 2 leafs
        if(portal._sideLIdx.size() != 2)
        {
            prtFragments.erase(pport);
            goto AG;
        }
        
        if(portal._vxes.size() < 3)
        {
            pport = prtFragments.erase(pport);
            goto AG;
        }
    }
    CleanProcFlag(prtFragments);
    
AG2:
    // erase any portals build on same 3d space
    FOREACH(vvector<Portal>, prtFragments, pport2)
    {
        if(pport2->_flags & PORT_PROCESSED)
            continue;
        pport2->_flags|=PORT_PROCESSED;

        if(PortalHasDuplicate(pport2, prtFragments))
        {
            prtFragments.erase(pport2);
            goto AG2;
        }
        if (Compiler::PCompiler->HasBreaked())
            return FALSE;

        pport2->AdjustVxOrder();

    }
    CleanProcFlag(prtFragments);
    
    return TRUE;
}

//---------------------------------------------------------------------------------------
// <<XXXXXXXXXX
// check if portals are on on top of each other
BOOL	PortalPRC::PortalHasDuplicate(vvector<Portal>::iterator itrPA, 
                                      vvector<Portal>& prtFragments)
{
    int pla1 = itrPA->_sideLIdx[0];
    int pla2 = itrPA->_sideLIdx[1];
    
    FOREACH(vvector<Portal>, prtFragments, itrPB)
    {
        if(itrPB == itrPA)
            continue;
        
        assert((*itrPA)._sideLIdx.size()==2 && (*itrPB)._sideLIdx.size()==2);
        
        int plb1 = itrPB->_sideLIdx[0];
        int plb2 = itrPB->_sideLIdx[1];
        
        // test same leafs
        if((pla1 == plb1 && pla2 == plb2) || 
            (pla1 == plb2 && pla2 == plb1))
        {
        /*
        Box bpa = GetVxesBbox(itrPA->_vxes);
        Box bpb = GetVxesBbox(itrPB->_vxes);
        
          // see wich one is bigger
          REAL da = Rabs(bpa.GetExtends().len2());
          REAL db = Rabs(bpb.GetExtends().len2());
          
            if(da >= db) // let delete pA portal a because is smaller
            {
            return TRUE;
            }
            */
            // delete the portal build on upper index
            if(itrPA->_planeIdxFinal <= itrPB->_planeIdxFinal)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

//---------------------------------------------------------------------------------------
/*
BackLeaf->Portal->FrontLeaf    P->[0][1]
|  - front leaf  
- back leaf
Leaf->[portals] these portals has back leaf this 'Leaf'

  Duplication rplaces any portal makeing it 2 way portal
  [Lo]->P->[l1]
  
*/
BOOL	PortalPRC::DuplicatePortals(vvector<Portal>& portalsI, 
                                    vvector<Portal>& portalsO)
{
    V3 nodeC0;  // portal side leafs bbox centers
    V3 nodeC1;
    
    portalsO.clear();
AG:
    FOREACH(vvector<Portal>, portalsI, pPrt)
    {
        if(pPrt->_flags & PORT_PROCESSED)
            continue;
        pPrt->_flags|=PORT_PROCESSED;

        if(pPrt->_vxes.size()<3)
        {
            portalsI.erase(pPrt);
            goto AG;
        }
        ASSERT(pPrt->_sideLIdx.size() == 2);   // indexes in node structure
        
        Portal dupP = (*pPrt);
        dupP.Reverse();
        dupP._flags |= PORT_DUP;
        
        SWITCHI(dupP._sideLIdx[0] , dupP._sideLIdx[1]);
        
        int iLf0 = pPrt->_sideLIdx[0];             // see wich leaf is back and wich is front
        int iLf1 = pPrt->_sideLIdx[1];
        nodeC0   = GetLeaf(iLf0)->_bbox.GetCenter();
        nodeC1   = GetLeaf(iLf1)->_bbox.GetCenter();
        int side1;
        int side0 = pPrt->GetSide(nodeC0);
        side1 = -side0;
        if(0 == side0)                          // it may end up on portal plane
        {                                       // so take the other one
            side1 = pPrt->GetSide(nodeC1);
            //assert(side1 != 0);
            if(0==side1)
            {
                continue;
            }
            side0 = -side1;
        }
        // make [0]P[1]
        if(side0 > 0) // if leaf [0] id on front switch the order
        {
            SWITCHI(dupP._sideLIdx[0] , dupP._sideLIdx[1]);
            SWITCHI(pPrt->_sideLIdx[0] , pPrt->_sideLIdx[1]);
        }
        
        //
        // _portIdxes
        //
        pPrt->BackLeaf()->AddPortalIdx(portalsO.size());
        AddPortal(*pPrt);
        
        pPrt->FrontLeaf()->AddPortalIdx(portalsO.size());
        AddPortal(dupP);
        
        if (Compiler::PCompiler->HasBreaked())
            return FALSE;
    }

    CleanProcFlag(portalsI);
    
    return TRUE;
}


//---------------------------------------------------------------------------------------
void    PortalPRC::Render(z_ed3View* pV)
{
    if(_portals.size()==0)
        return;
    
    Ta.Disable();
    //glDisable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
    
    int ii=0;
    FOREACH(vvector<Portal>, _portals, pPrt)
    {
        if(pPrt->_flags & PORT_DUP)
        {
            glColor4ubv(CLR_PORTALS1);
        }
        else
        {
            glColor4ubv(CLR_PORTALS);
        }
        
        {
            Box b;
            glBegin(GL_POLYGON);
            FOREACH(vvector<V3>, pPrt->_vxes, pv)
            {
                b.AddPoint(*pv);
                glVertex3f(pv->x,pv->y,pv->z);
            }
            glEnd();
            if(pPrt->_flags & PORT_DUP)
            {
                pV ? pV->TextOut(b.GetCenter()+(pPrt->_n*12), MKSTR("P%d", pPrt->_idxThis), ZGREEN): 0;
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
                
            }
            else
            {
                pV ? pV->TextOut(b.GetCenter()+(pPrt->_n*12), MKSTR("P%d", pPrt->_idxThis), ZBLUE):0;
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
                
            }
            
        }
        
        {
            glColor4ubv(CLR_PORTALSLINE);
            glBegin(GL_LINE_LOOP);
            FOREACH(vvector<V3>, pPrt->_vxes, pv)
            {
                glVertex3f(pv->x,pv->y,pv->z);
            }
            glEnd();
        }
        
        
        ii++;
    }
    glColor4ubv(CLR_WHITE);
    glDisable(GL_BLEND);
    // DOC()->SetFaceMode();
    
    // render the center of each leaf
    vvector<CLeaf*>& lidx = _pTree->GetLeafs();
    FOREACH(vvector<CLeaf*>, lidx, ppLeaf)
    {
        V3& vc = (*ppLeaf)->_bbox.GetCenter();
        pV ? pV->TextOut(vc, MKSTR("L%d", (*ppLeaf)->_leafIdx), ZYELLOW): 0;
        DOC()->DrawOrtho(vc, VZ, 8.0, .3f);
    }
}
 
//--------------------------------------------------------------------------------
// clip the portal against leaf polygons
void    PortalPRC::ReduceVertexes(vvector<Portal>& portals)
{
    AG:
    // clip the portal against both leaf polygons
    FOREACH(vvector<Portal>, portals, pport)	
    {
        Portal& portal = *pport;
    
        if(portal._flags & PORT_PROCESSED)
            continue;
        portal._flags|=PORT_PROCESSED;

        if(portal._vxes.size() < 3)
        {
            portals.erase(pport);
            goto AG;
        }
    
        ClipWithLeafSides(pport->BackLeaf(), portal);
        ClipWithLeafSides(pport->FrontLeaf(), portal);
    
        if(portal._vxes.size() < 3)
        {
            portals.erase(pport);
            goto AG;
        }
    }
    CleanProcFlag(portals);
}


//--------------------------------------------------------------------------------
void PortalPRC::CutPortalWithNodeBox(Portal& portal, Box& box, BOOL reverse)
{
    Plane cutPlanes[6];
    
    if(reverse)
    {
        cutPlanes[0] = Plane(-VY,  box._max);
        cutPlanes[1] = Plane(VY,   box._min);
        cutPlanes[2] = Plane(-VX,  box._max);
        cutPlanes[3] = Plane(VX,   box._min);
        cutPlanes[4] = Plane(-VZ,  box._max);
        cutPlanes[5] = Plane(VZ,   box._min);
        
    }
    else
    {
        cutPlanes[0] = Plane(VY,box._max);
        cutPlanes[1] = Plane(-VY,box._min);
        cutPlanes[2] = Plane(VX,box._max);
        cutPlanes[3] = Plane(-VX,box._min);
        cutPlanes[4] = Plane(VZ,box._max);
        cutPlanes[5] = Plane(-VZ,box._min);
    }
    
    Portal	rp = portal;
    Portal	a(this);
    Portal	b(this);
    
    for(int i=0; i<6 && rp._vxes.size()>2; i++)
    {
        rp.Split(cutPlanes[i],a,b);
        rp = b;
        b.Clear();
        a.Clear();
    }
    portal.Clear();
    portal = rp;
}

void PortalPRC::R_CutPortalWithLeafs(Portal& portal, CMiniNode* pNode)
{
    // scan down from this node and cut with all leafs bbox
    if(pNode->IsLeaf())
    {
        Box& lbox = pNode->_bbox;
        V3   crns[8];
        REAL d2p = 0;
        int  posneg[2] = {0};
        
        lbox.GetCorners(crns);
        
        // test if leaf bbox touches this portal plane
        for(int i = 0; i < 8; i++)
        {
            d2p = portal.DistTo(crns[i]);
            if(d2p>.1)
                ++posneg[0];
            else if(d2p<-.1)
                ++posneg[1];
            else
            {
                posneg[0]=posneg[1]=1;
                break;
            }
        }
        if(posneg[0] && posneg[1])
        {
            CutPortalWithNodeBox(portal, lbox);
            return;
        }
    }
    if(pNode->Front())
        R_CutPortalWithLeafs(portal, pNode->Front());
    if(pNode->Back())
        R_CutPortalWithLeafs(portal, pNode->Back());
}

// clip portal with all planes from parent nodes
void PortalPRC::CutPortalWithParents(Portal& portal, CMiniNode* pNode)
{
    CMiniNode*  pParent = pNode;
    Portal      pFp(this);
    Portal      pBp(this);
    
    
    while((pParent = pNode->Parent()) && portal._vxes.size())
    {
        portal.Split(_pTree->_planes[pParent->_planeIdx], pFp, pBp);
        portal.Clear();
        
        if(pParent->Front() == pNode)
            portal = pFp;
        else
            portal = pBp;
        
        pFp.Clear();
        pBp.Clear();
        
        pNode    = pParent;
        pParent  = pParent->Parent();
    }
}

void  PortalPRC::R_FloodBSP(CBspTree*  tmpTree, int iLeaf, BYTE* wetLeafs)
{
    int             nCount       = 0;
    CLeaf*          pLeaf        = tmpTree->GetLeaf(iLeaf);
    vvector<int>&   portalsIdxes = pLeaf->GetPortalIdxes();
    
    Poly*   pHead  = pLeaf->GetPolys(nCount);
    for(int i=0; i<nCount;i++)
    {
        pHead->_polyflags &= ~POLY_DELETED;
        pHead++;
    }
    
    wetLeafs[iLeaf] = 1;    
    FOREACH(vvector<int>,  portalsIdxes, ppidx)
    {
        Portal&  portal = GetPortal(*ppidx);
        CLeaf*   pNleaf = portal.FrontLeaf();
        
        portal._flags &= ~POLY_DELETED;
        
        if(wetLeafs[pNleaf->_leafIdx] == 0)
        {
            V3 ex = pNleaf->_bbox.GetExtends();
            if(IsZero(ex.getmin(),EPSMED))
            {
                LOGERR(E_ESR, pNleaf->_bbox.GetCenter(),"Warning: Leaf:%d has width 0 units",pNleaf->_idxNodeThis);
                Compiler::PCompiler->_errDetected=1;
            }
            R_FloodBSP(tmpTree, pNleaf->_leafIdx, wetLeafs);
        }
    }
}



BOOL PortalPRC::ReFlood()
{
    // visit portals/leafs/polys
    BYTE*   byLeafs   = 0;  
    int     inPolys   = _pTree->GetPolys().size();
    int     inPortals = _portals.size();
    
    LOG(E_PORT,MKSTR("PRT: Reflooding BSP with %d portals and %d polygons ",inPortals,inPolys));
    
    // mark them deleted
    FOREACH(vvector<Poly>, _pTree->GetPolys(), ppoly)   
    {
        ppoly->_polyflags |= POLY_DELETED;
        ++inPolys;
    }
    
    FOREACH(vvector<Portal>, _portals, pPrt)
    {
        pPrt->_flags |=  POLY_DELETED;
    }
    
    int ileaf = _pTree->R_FindLeaf(0, V0);
    if(-1 == ileaf)
    {
        LOGERR(E_ESR, V0,"Error: Center of the map must be empty!");
        Compiler::PCompiler->_errDetected=1;
        return FALSE;
    }
    _TRY
    {

        byLeafs   = new BYTE[_pTree->GetLeafsCount()]; //alloc bits for each leaf
        if(0 == byLeafs)
            return FALSE;
        
        R_FloodBSP(_pTree, ileaf, byLeafs);
        delete[] byLeafs; 
        byLeafs = 0;
    }
    _CATCHX()
    {
        delete[] byLeafs; 
        byLeafs = 0;
        return FALSE;
    }
    //
    // delete unvisited polys and portals
    //
AG0:
    FOREACH(vvector<Poly>, _pTree->GetPolys(), ppoly)   // mark them undeleted
    {
        if(ppoly->_polyflags & POLY_DELETED)
        {
            _pTree->GetPolys().erase(ppoly);
            goto AG0;
        }
    }
    
AG1:
    FOREACH(vvector<Portal>, _portals, pPrt)
    {
        if(pPrt->_flags & PORT_PROCESSED)
            continue;
        pPrt->_flags|=PORT_PROCESSED;

        if(pPrt->_flags &  POLY_DELETED)
        {
            _portals.erase(pPrt);
            goto AG1;
        }
    }
    CleanProcFlag(_portals);
    
    inPolys   = _pTree->GetPolys().size();
    inPortals = _portals.size();
    
    LOG(E_PORT, MKSTR("PRT: %d portals and %d polygons were deleted while flooding the scene",inPortals,inPolys));
    
    if(0==inPolys)
    {
        LOG(E_PORT, "Warning: Level is leaking outside detected. (Cannot find the leaking spot)");
        
        TestExteriorPortals();
        return FALSE;
    }
    return TRUE;
    
}


BOOL    PortalPRC::TestExteriorPortals()
{
    int         counter = 0;
    int         nTotal  = _portals.size();
    V3          vStart, vEnd, vrEnd, vResult;
    CollPack    col;
    
    DOC()->_debugPolys.clear();
    
AG:
    FOREACH(vvector<Portal>, _portals, pPrt)
    {

        if(pPrt->_flags & PORT_PROCESSED)
            continue;
        pPrt->_flags|=PORT_PROCESSED;

        if(pPrt->_vxes.size()<3)
        {
            _portals.erase(pPrt);
            goto AG;
        }
        
        V3 vNormal = _pTree->_planes[pPrt->_planeIdx]._n;
        
        vStart = pPrt->_vxes[0] + (pPrt->_vxes[2]-pPrt->_vxes[0]) * .5f;
        vEnd   = vStart + (vNormal*8912.0);
        vrEnd  = vStart + (vNormal*(-8912.0));
        
        if(!_pTree->SegmentIntersect(vStart,vEnd,0, col) ||
            !_pTree->SegmentIntersect(vStart,vEnd,0, col) )
        {
            
            DOC()->_ip[0]=vStart;
            DOC()->_ip[1]=vEnd;
            
            Poly p;
            FOREACH(vvector<V3>,pPrt->_vxes, vx)
                p._vtci << Vtx2(*vx);
            
            DOC()->AddDbgPoly(p);
            
            LOGERR(E_PORT, vStart, MKSTR("Warning: Outside Portal Delected. Leak along [%.2f %.2f %.2f]<->[%.2f %.2f %.2f]. ",
                    vStart.x,vStart.y,vStart.z, vEnd.x,vEnd.y,vEnd.z));
            Compiler::PCompiler->_errDetected=1;
            return FALSE;
        }
        if (Compiler::PCompiler->HasBreaked())
            return FALSE;
    }
    CleanProcFlag(_portals);
    return TRUE;
    
}

void    PortalPRC::CleanProcFlag(vvector<Portal>&  portals)
{
    FOREACH(vvector<Portal>, portals, pp)
        pp->_flags &=~PORT_PROCESSED;
}

