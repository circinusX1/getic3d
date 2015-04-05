// PortalPRC1.cpp: implementation of the CPortalPRC class.
//
//---------------------------------------------------------------------------------------

#include "stdafx.h"
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
// search in lafs and if the bbox of the leaf minimum extend is t narrow, deete the leaf
// polys and attach the polys to one of the closest leafs in a leaf chain. mark then 
// that leaf's all portals that lead into
void    PortalPRC::MergeNarowLeafs()
{
    FOREACH(vvector<CLeaf*> ,_pTree->_leafs, ppl)
        (*ppl)->_nodeflags &= ~(LEAF_MERGED);


    FOREACH(vvector<Portal>,_portals, itp )
    {
        Portal& portal = *itp;

        CLeaf* pBack  = portal.BackLeaf();
        CLeaf* pFront = portal.FrontLeaf();

        REAL ex  = pBack->_bbox.GetMinExtend();
        if(ex < Compiler::PCompiler->_narowLeaf )
        {
            MergeLeafs(pBack, pFront, portal);

            itp = _portals.erase(itp)-1;    //erase this and it's duplicate
            itp = _portals.erase(itp);
            if(itp != _portals.begin())
                --itp;
            if(_portals.size()==0)
                break;

        }
    }


}


void    PortalPRC::MergeLeafs(CLeaf* pFrom, CLeaf* pInto, Portal& portal)
{
    // all deleted leaf portals indexex now points into the new leaf
    FOREACH(vvector<Portal>,_portals, itp )
    {
        if(itp->BackLeaf() == pFrom)
        {
            itp->_sideLIdx[0] = pInto->_leafIdx;
            pInto->_portIdxes.push_back(itp->_idxThis); // has more leaf leading out
        }

        if(itp->FrontLeaf() == pFrom)
        {
            itp->_sideLIdx[1] = pInto->_leafIdx;
        }
    }

    // link it 
    pInto->_pNext = pFrom;
    pInto->_bbox.Union(pFrom->_bbox);
    pFrom->_nodeflags |= LEAF_MERGED;
    pFrom->_nPolys = 0;
}
