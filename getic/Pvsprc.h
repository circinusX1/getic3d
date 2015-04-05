// PVSPrc.h: interface for the CPVSPrc class.
// By: Marius Chincisan
//---------------------------------------------------------------------------------------

#ifndef __PVS_PRC__
#define __PVS_PRC__

//---------------------------------------------------------------------------------------
#include "basecont.h"
#include "BitArray.h"

//---------------------------------------------------------------------------------------
class CBspTree;
class CLeaf;
class Portal;
class PortalPRC;

//---------------------------------------------------------------------------------------
// paralel array of data assocuated to each portal. The portal index idxThis will
// be used to point tin this array
struct PortalData
{
    PortalData(int sz):_portArrPvs(sz)
	{
        _possibleVisCount = 0;
        _size   = _portArrPvs.Size();
    }
    ~PortalData(){}
    
    int			_size;
    int			_possibleVisCount;
    CBitArray   _portArrPvs;
};

//---------------------------------------------------------------------------------------

class PVSPrc  
{
public:
    PVSPrc():_nleafs(0),_pvs(0),_cPvss(0){}
    virtual ~PVSPrc(){
        Clear();
    };
    
    void    Clear();
    BOOL    Process(CBspTree* pT, PortalPRC* pP);

private:
    BOOL    InitalPortalVis(vvector<Portal>& rPortals);
    void    R_PortalFlood(Portal& portal, BYTE* pVis, int nLeaf);   // flod in neghbour leaf
    void    PerformPVS(Portal& pOrigin, vvector<Portal>& rPortals, BYTE* prevPvs);
    void    CompressPVS();
	void    GaterLeafsVis();
    BOOL    PortalSeesPortal(Portal& a, Portal& b);
    void    DoVisByLeafs();
private:
    CBspTree*   _pBspTree;
    PortalPRC*  _pPortPrc;
public:
    int                   _nleafs;
    vvector<PortalData*>  _portVs;
public:
	BYTE*				  _pvs;
	int					  _cPvss;
};

#endif // !defined(AFX_PVSPRC_H__AC44DB74_60E2_4611_97C6_B7AB50079388__INCLUDED_)
