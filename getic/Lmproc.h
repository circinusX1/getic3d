// LMProc.h: interface for the LMProc class.
//
//-----------------------------------------------------------------------------
#ifndef __LMPROC_H__
#define __LMPROC_H__

#include "basecont.h"

//-----------------------------------------------------------------------------
struct LmInfo;
class Scene;
class CBspTree;
class CLightBulb;
class CLeaf;
class Poly;
class TerTree;
//-----------------------------------------------------------------------------
struct LmSizeBuff
{
    LmSizeBuff()
    {
        pBuff = 0;
        index = -1;
        lmName[0] = 0;
        flags = 0;
        sz.cx = 0;
        sz.cy = 0;
        realsz.cx = 0;
        realsz.cy = 0;

    }
    LmSizeBuff(int x, int y, int realx, int realy, int bp)
    {
        index = -1;
        lmName[0] = 0;
        flags = 0;
        sz.cx = x;
        sz.cy = y;
        realsz.cx = realx;
        realsz.cy = realy;
        bpp   = bp;
        pBuff = new BYTE[x*y*bp];
        if(!pBuff){
            sz.cx=sz.cy=0;
        }
        index=-1;       // not saved yet. if saed has the offset in bsp file buffer
    }
    LmSizeBuff(const LmSizeBuff& r){
        index = r.index;
        sz.cy = r.sz.cy;
        sz.cx = r.sz.cx;
        realsz.cx = r.realsz.cx;
        realsz.cy = r.realsz.cy;
        bpp   = r.bpp;
        flags = r.flags;
        ::strcpy(lmName, r.lmName);
        pBuff = new BYTE[sz.cx*sz.cy*bpp];
        if(pBuff){
            ::memcpy(pBuff, r.pBuff, sz.cx*sz.cy*bpp);
        }
    }
    LmSizeBuff& operator = (const LmSizeBuff& r)
    {
        if(this != &r){
            index = r.index;
            sz.cy = r.sz.cy;
            sz.cx = r.sz.cx;
            realsz.cx = r.realsz.cx;
            realsz.cy = r.realsz.cy;

            bpp   = r.bpp;
            flags = r.flags;
            ::strcpy(lmName, r.lmName);
            if(pBuff)delete[]pBuff;
            pBuff = new BYTE[sz.cx*sz.cy*bpp];
            if(pBuff){
                ::memcpy(pBuff, r.pBuff,sz.cx*sz.cy*bpp);
            }
        }
		return *this;
    }
    ~LmSizeBuff(){
        delete[] pBuff;
        pBuff = 0;
    }
    void Reset(){
        pBuff = 0;
    }
    int     index;
    size_t    sz;
    size_t    realsz;
    int     bpp;
    size_t   flags;
    BYTE*   pBuff;
    char    lmName[16];
};

//-----------------------------------------------------------------------------
class LMProc  
{
public:
	friend class Compiler;

    LMProc():_pTree(0)
    {
	    _pPoolLmap = 0;
		_szPool = 0;
	}
	virtual ~LMProc();
    void    NTC_BuildLmaps(CBspTree* pTree);//no thread call
    void    NTC_BuildLmaps(TerTree* pTree);
    void    NTC_Clear();     //no thread call   
	BOOL	AllocMemory(vvector<CBspTree*>& bspTrees);
    BOOL    Calculate(Scene* pScene, vvector<CBspTree*>&);
    BOOL    GetLmapBytes(const char* name, LmSizeBuff& lnB);
private:
    BOOL    Calculate(Scene* pScene, CBspTree* pTreeMain, CBspTree* pTree, vvector<CLightBulb*>&);
	void    CalcLMapOnPolys(vvector<Poly*>& polyGroup, vvector<CLightBulb*>&);
    void    CalcLMapOnPolysConst(vvector<Poly*>& polyGroup, vvector<CLightBulb*>& lights, BOOL bConst);

    void    GouraudShade(vvector<Poly*>& polyGroup, vvector<CLightBulb*>& lights);
	void	CalcMinMaxUVs(vvector<Poly*>& polyGroup, UV& minUV,UV& maxUV);
	void    BuildLmapEdges(REAL d2p, V3& normal, UV& minUV, UV& maxUV, V3& v0, V3& v1 ,V3& v2);
//	BOOL    R_Snd2Tree(CLightBulb* pLB, int nodeIdx)  ;
    void    IluminatePolygons(CLightBulb* pLB, CLeaf*  pLeaf);
    size_t    CalcBmpDims(const UV& edgesLength);
    BYTE*   GetPrevLumels(Poly& poly, V3* pLumels, size_t sz);
    void    StoreLumels(Poly& poly, V3* pLumels, size_t sz);
    void    CalculateTtLm(TerTree& tt, vvector<CLightBulb*>& lights);
    void    _GatherSamePolys(vvector<Poly>&, vvector<Poly>::iterator&, vvector<Poly*>&, Box&);
    int    _YolTtLight(TerTree& tt,int x, int z, const V3& vWalk1, const V3& vWalk2, const V3& vWalk3, CLRNOA* glight, int xtiles, vvector<CLightBulb*>&);
    int    _GetLumColor(V3& lumel,  V3& lumelPos, const V3& norm, vvector<CLightBulb*>&);
    int    _RoundToClosestPow2(size_t& sz, int updown);
private:
    REAL                 _lmUnit;
    Scene*               _pScene;
	CBspTree*            _pMainTree;
    CBspTree*            _pTree;
    vvector<LmSizeBuff*> _lMaps;
    int                  _totpolys;
    int                  _curPolys;

	BYTE*				_pPoolLmap;
	int					_szPool;
	int					_lastAllocIdx;
};

#endif // __LMPROC_H__
