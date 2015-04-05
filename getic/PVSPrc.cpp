// PVSPrc.cpp: implementation of the CPVSPrc class.
//
//---------------------------------------------------------------------------------------

#include "stdafx.h"
#include "baseutils.h"
#include "z-edmap.h"
#include "PVSPrc.h"
#include "BspTree.h"
#include "PortalPrc.h"
#include "Mainfrm.h"
#include "Compiler.h"

//---------------------------------------------------------------------------------------
static int CompressLeafPVS(BYTE* pDest,BYTE* pLeafPvs, int bytesPvs);

int AllocBitsRoundByLong(int nSetCount)
{
    int rval =   (nSetCount + 7) / 8;    // bytes
    rval = (rval * 3 + 3) & 0xFFFFFFFC;  // round by mod4
    return rval;
}


//---------------------------------------------------------------------------------------
BOOL    PVSPrc::Process(CBspTree* pT, PortalPRC* pP)
{
    BOOL bret=TRUE;
    LOG(E_PVS,"PVS: Processing Potentially View Sets");
    // get temp pointers
    _pBspTree = pT;
    _pPortPrc = pP;

    Clear();
	_nleafs  = pT->GetLeafsCount();
    
    if(pP->HasPortals())
    {
        vvector<Portal>& rPortals = pP->GetPortals();
        if(bret = InitalPortalVis(rPortals))
        {
		    GaterLeafsVis();
            CompressPVS();
        }
        
        //// DoVisByLeafs();
    }
    else
    {
        assert(1 == _nleafs);
		if(0==_nleafs)
			return FALSE;
        if(_pvs) delete[] _pvs; _pvs = 0;        
        _cPvss  = 4;
        _pvs    = new BYTE[_cPvss];
		memset(_pvs,255,_cPvss);
        FOREACH(vvector<CLeaf*>, _pBspTree->_leafs, ppLeaf)
        {
            (*ppLeaf)->_pvsIdx = 0;
        }
    }
	if(0 == bret)
		LOG(E_PVS,"Error: PVS1022. Internal Error");
	else
		LOG(E_PVS,"PVS: Potentially View Sets '%s' successful build for '%d' leafs.",pT->_name, _pBspTree->_leafs.size());
    return bret;
}

//---------------------------------------------------------------------------------------
BOOL    CompletteOnBack(Plane& pA, Portal& rP)
{
    int count = rP._vxes.size();
    FOREACH(vvector<V3>, rP._vxes, pvx)
    {
        if(pA.GetSide(*pvx) == 1)
            break;
        --count;
    }
    return (0==count);
}

//---------------------------------------------------------------------------------------
BOOL    CompletteOnFront(Plane& pA, Portal& rP)
{
    int count = rP._vxes.size();
    FOREACH(vvector<V3>, rP._vxes, pvx)
    {
        if(pA.GetSide(*pvx) == -1)
            break;
        --count;
    }
    return (0==count);
}


//---------------------------------------------------------------------------------------
BOOL    PVSPrc::InitalPortalVis(vvector<Portal>& rPortals)
{
    BOOL    bRetVal= TRUE;
    BYTE*   pVis   = new BYTE[rPortals.size()];
    int     nCount = 0;
    int     nTotal = rPortals.size();
    int     percent;

    ASSERT(pVis);
    _TRY
    {

        FOREACH(vvector<Portal>, rPortals, portItA)
        {
            Portal&     prtA = *portItA;
            PortalData* pD  = new PortalData(_nleafs);

            _portVs << pD;                                    // this prtA pvs
            ::memset(pVis, 0, rPortals.size());               // reset the array for this prtA        
            int nCount2 = 0;
            FOREACH(vvector<Portal>, rPortals, portItB)
            {
                Portal& prtB = *portItB;
                if(portItB == portItA)
                    continue;

                if(CompletteOnBack(prtA, prtB))    // complette on back we dont see this
                    continue;                       // so leave his byte on 0
            
                if(CompletteOnFront(prtB, prtA))  // are we complette on front 
                    continue;                           

                pVis[prtB._idxThis] = 1;                // portal vis flag <<<<<<<<<<<<

            }
            // as long we roll and we have temp vis sets we do here the visibile sets 
            // I can stop here for very rough visibility
            pD->_possibleVisCount = 0;
            if(Compiler::PCompiler->_bPvsMethod==2)
            {
                PerformPVS(prtA, rPortals, pVis);
            }
            R_PortalFlood(prtA, pVis, prtA._sideLIdx[1]);   // flod in neghbour leaf
            if (Compiler::PCompiler->HasBreaked())
		    {
			    bRetVal = FALSE;
			    break;
		    }
            percent = (100*nCount++) / (1+nTotal);
            PROG(percent);
        }
    }
    _CATCHX()
    {
        bRetVal = FALSE;
    }
    delete []pVis;
    return bRetVal;
}

//---------------------------------------------------------------------------------------
void    PVSPrc::R_PortalFlood(Portal& srcP, BYTE* pVis, int nLeaf)
{
    if(_portVs[srcP._idxThis]->_portArrPvs[nLeaf] == 1) //<<<<leaf vis bit
        return;

	_portVs[srcP._idxThis]->_portArrPvs.Set(nLeaf);
    _portVs[srcP._idxThis]->_possibleVisCount++;       // count how many portals this prtA can flood
    CLeaf*          pToLeaf = _pPortPrc->GetLeaf(nLeaf);
    vvector<int>&   iporrts = pToLeaf->GetPortalIdxes();

    FOREACH(vvector<int>, iporrts, pip)   
    {
        #pragma message("unlimit limit here the flood")
        if(0 == pVis[*pip])                             // dont flood in this prtA
            continue;

        Portal& prtA = _pPortPrc->GetPortal(*pip);
        R_PortalFlood(srcP, pVis, prtA._sideLIdx[1]);
    }
}

//---------------------------------------------------------------------------------------
void    PVSPrc::PerformPVS(Portal& portA, vvector<Portal>& rPortals, BYTE* prevPvs)
{
    FOREACH(vvector<Portal>, rPortals, portItB)
    {
        Portal& prtB = *portItB;
        if(prtB == portA)
            continue;

        if(prevPvs[prtB._idxThis] == 0)        
            continue;
        
        if(!PortalSeesPortal(portA,prtB))
            prevPvs[prtB._idxThis]=0;
    }
}

//---------------------------------------------------------------------------------------
void    PVSPrc::CompressPVS()
{
}

//---------------------------------------------------------------------------------------
void    PVSPrc::GaterLeafsVis()
{
    int     dummy;
	if(_pvs) delete[] _pvs; _pvs = 0;
    int      nPerLeaf    = AllocBitsRoundByLong(_nleafs);
    _cPvss  = _nleafs * nPerLeaf;                           // all leafs        
	_pvs    = new BYTE[_cPvss];

    LOG(E_PVS,"PVS: PVS data has '%d' bytes ",_cPvss);

	::memset(_pvs, 0, _cPvss);                              // serooo them
    BYTE*    pWalkPvsPtr       = _pvs;                            // head of oll bytes
    
    CBitArray pPerLeafPvs(_nleafs)  ; 


	FOREACH(vvector<CLeaf*>, _pBspTree->_leafs, ppLeaf)
	{
		CLeaf*          pLeaf = (*ppLeaf);
        vvector<int>&   pidxs = pLeaf->GetPortalIdxes();

        pPerLeafPvs.Reset();
        pLeaf->_pvsIdx  = pWalkPvsPtr - _pvs;

        pPerLeafPvs.Set((*ppLeaf)->_leafIdx);

        FOREACH(vvector<int>,pidxs,pidx)
        {
            Portal& portal = _pPortPrc->GetPortal(*pidx);
            PortalData* pD = _portVs[portal._idxThis];
            pPerLeafPvs.Merge(pD->_portArrPvs);
        }
		assert(pPerLeafPvs.Size() == nPerLeaf);
        if(Compiler::PCompiler->_bCompressPvs)
        {
            pWalkPvsPtr += CompressLeafPVS( pWalkPvsPtr, pPerLeafPvs.Buffer(dummy) ,nPerLeaf);
        }
        else
        {
            ::memcpy(pWalkPvsPtr, pPerLeafPvs.Buffer(dummy), nPerLeaf);
            pWalkPvsPtr += nPerLeaf;
        }
	}
}



int CompressLeafPVS(BYTE* pDest, BYTE* pLeafPvs, int bytesPvs)
{
    BYTE*   pDestC = pDest;
    int     repeatCnt;

    for(int i=0; i < bytesPvs ; i++)    // for each octet
    {
        *pDestC++ = pLeafPvs[i];
        if(pLeafPvs[i])  
            continue;      // all octet is !=0 do not compress

        repeatCnt = 1;
        for(i++; i < bytesPvs; i++)     // now count the 0 octets 
        {
            if(pLeafPvs[i] || repeatCnt==255) // no more than 255 zeroes in 1 octet
                break;
            else
                repeatCnt++;
        }
        *pDestC++ = repeatCnt;
        i--;                            // repos the counter
    }   
    return pDestC - pDest;
}



BOOL    PVSPrc::PortalSeesPortal(Portal& a, Portal& b)
{
    V3 dummy;

    // test center
    V3  paa;// = a._vxes[0]+ (a._vxes[1] - a._vxes[0])*.5f;
    V3  pab;// = b._vxes[0]+ (b._vxes[1] - b._vxes[0])*.5f;
    

	FOREACH(vvector<V3>, a._vxes ,pv)
		paa+=*pv;

	FOREACH(vvector<V3>, b._vxes ,pv)
		pab+=*pv;
	
	paa/=(REAL)a._vxes.size();
	pab/=(REAL)b._vxes.size();
	
	CollPack   col;
    if(!_pBspTree->SegmentIntersect(paa, pab, 0, col))
    {
        return TRUE;
    }
    
    V3 va,vb;
    FOREACH(vvector<V3>, a._vxes, pva)
    {
        FOREACH(vvector<V3>, b._vxes, pvb)
        {
            va.interpolate(*pva,paa,0.2);
            vb.interpolate(*pvb,pab,0.2);

            if(!_pBspTree->SegmentIntersect(va, vb, 0, col))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void PVSPrc::DoVisByLeafs()
{
	if(_pvs) 
        delete[] _pvs; 
    _pvs = 0;
    int      nPerLeaf    = AllocBitsRoundByLong(_nleafs);
    _cPvss  = _nleafs * nPerLeaf;                           // all leafs        
	_pvs    = new BYTE[_cPvss];
	::memset(_pvs, 0, _cPvss);                              // zeroo them
    BYTE*    pWalkPvsPtr       = _pvs;                      // head of oll bytes
    CBitArray pPerLeafPvs(_nleafs)  ; 

    // for each leaf
    FOREACH(vvector<CLeaf*>, _pBspTree->_leafs, ppLeafFrom)
    {
        
        pPerLeafPvs.Reset();

        for(vvector<CLeaf*>::iterator ppLeafTo = ppLeafFrom; ppLeafTo != _pBspTree->_leafs.end(); ppLeafTo++)
        {
            CLeaf*  pLFrom   = *ppLeafFrom;
            CLeaf*  pLTo     = *ppLeafTo;
            
            if(pLFrom == pLTo)
            {
                pPerLeafPvs.Set(pLTo->_leafIdx);
            }
            else
            {
                FOREACH(vvector<int>,  pLFrom->_portIdxes, ppFrom)
                {
                    Portal&  pFrom =  _pPortPrc->GetPortal(*ppFrom);

                    FOREACH(vvector<int>,  pLTo->_portIdxes, ppTo)
                    {
                        Portal&  pTo =  _pPortPrc->GetPortal(*ppTo);

                        if(CompletteOnBack(pFrom, pTo))    // complette on back we dont see this
                            continue;                       // so leave his byte on 0
            
                        if(CompletteOnFront(pTo, pFrom))  // are we complette on front 
                            continue;                           

                        if(Compiler::PCompiler->_bPvsMethod==2)
                        {
                            if(PortalSeesPortal(pFrom, pTo))               
                            {
                                pPerLeafPvs.Set(pLFrom->_leafIdx);
                                goto EX_PORTAL;
                            }
                        }
                        else
                        {
                            pPerLeafPvs.Set(pLFrom->_leafIdx);
                            goto EX_PORTAL;
                        }
                    }
                }
EX_PORTAL:;
            }
        }
        int dummy;
        ::memcpy(pWalkPvsPtr, pPerLeafPvs.Buffer(dummy), nPerLeaf);
        pWalkPvsPtr += nPerLeaf;
    }
}


void    PVSPrc::Clear()
{
	delete[] _pvs;  _pvs=0;
    _portVs.deleteelements();
};
