// MiniBsp.h: interface for the MiniBsp class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __MINIBSPX_H__
#define	__MINIBSPX_H__

#include <list>


#include "Brush.h"
#include "bspfilestr.h"

class MiniBsp ; 
class CMiniNode  
{
public:
    CMiniNode(MiniBsp* pBsp, size_t dw=0)
    {
        _cluster      = -1;
        _idxNodeThis  = -1;
		_nodeidx[0]	  = -1;
		_nodeidx[1]	  = -1;
        _planeIdx     = -1;
        _nPolys       = 0;
        _polyIdx      = -1;
	    _nodeflags    = dw;
        _pBsp         = pBsp;
		_leafIdx	  = -1;	
		_idxParent	  = -1;
        _planeIdxFinal =-1;
        _origPolyIndex = -1;
        _zonearea      = -1;
        _selected	   = 0;	 
        _content       = 0;
    }
    virtual ~CMiniNode(){}
    
    CMiniNode* Back();
    CMiniNode* Front();
	CMiniNode* Parent();
    int     FontIdx(){return _nodeidx[1];}
    int     BackIdx(){return _nodeidx[0];}
    BOOL	IsEmptyLeaf()  {return  (_nodeflags & NODE_LEAF) && !(_nodeflags & NODE_SOLID);}
	BOOL	IsLeaf()  {
        return  (_nodeflags & NODE_LEAF);
    }
	BOOL	IsSolid() {
        return (_nodeflags & NODE_SOLID);
    }
    BOOL	IsSpace() {return !(_nodeflags & NODE_SOLID) && (_nodeflags & NODE_LEAF);}
	BOOL	IsNode()  {
        return !(_nodeflags & NODE_LEAF);
    }
    INLN    Plane&  GetPlane()const;//{
    INLN    Poly*   GetPolys(int& count);
    INLN    Poly&   GetOrigPoly();
    int     AddUniqueVertexes(vvector<V3>& globPool);

public:
    int         _idxNodeThis;  // this idx in bsp array
    int         _idxParent;    // this idx in bsp array
    int		    _nodeidx[2];   // front back
    CLR         _ilum;         // leaf ilum
    int         _planeIdx;     // spliter plane index in bsp plane arrray
	int			_leafIdx;
    int         _planeIdxFinal;    // spliter plane index in bsp global plane arrray
    int         _origPolyIndex;    // polygon index just in case;
	size_t	    _nodeflags;			
    int		    _nPolys;       // leaf polys
    int		    _polyIdx;      // start poly leaf index
    int         _cluster;
    MiniBsp*    _pBsp; 
    Box         _bbox;
    size_t       _zonearea;
    size_t       _flags;
    size_t       _selected;
	size_t       _content;
};



struct	CompileRcs
{
	int				noode;
	list<Poly>		polys;
};


class Compiler;
class MiniBsp  
{
public:
    typedef list<Poly>  PolyList;
    enum _CUT_FLAG{CUT_POLYS=0, PART_ON_FRONT, PART_ONBOTH, IS_HEIGHTMAP};
    enum _BSP_TYPE{BSP_LEAFY=0, BSP_TERRAIN, BSP_AABB_LEAFY, BSP_NODE};
public:
    friend class CMiniNode;
    friend class Compiler;
	MiniBsp(int expected=128){
		_polys.reserve(expected);
		_nodesPtr.reserve(expected*2);
        _bRemUS     =  FALSE;
		_bRemCoplan = TRUE;
		_bClipSolid = TRUE;
        _bremES     = FALSE;
        _polyStartIdx = 0;
        _nodeStartIdx = 0;
        _leafStartIdx = 0;
        _pdroppedpolys  = 0;
        _timeStart    = 0;
        _dlgWaitOn    = 0;
        _balance      = 6;
        _treeflags   = 0;
		_name[0]	  = '.';
		_name[1]	  = 0;
		_treeprops     = 0;
        _cutflag      = CUT_POLYS;  
        _bsptype      = BSP_LEAFY;
	};
	virtual ~MiniBsp(){
		Clear();
	};
    void            SetType(_BSP_TYPE bt, _CUT_FLAG cf ){
        _cutflag    = cf;
        _bsptype    = bt;
    }
	virtual void	Clear();
    vvector<Poly>&  GetPolys() {return _polys;};
	virtual BOOL Compile(vvector<Poly>& polys, size_t brFlags, BOOL bRemES=FALSE, BOOL bTextCvx=FALSE); // virtual from base
    int    AddUniqueVertexes(vvector<V3>& globPool);
    void    ClipBrPolys(vvector<Poly>&, vvector<Poly>& , BOOL , BOOL ,BOOL);
	CMiniNode*  Root(){return _nodesPtr[0];}//       _pRoot;
    vvector<CMiniNode*>& GetNodes(){return _nodesPtr; }
    CMiniNode* GetNode(int idx){ASSERT(idx < (int)_nodesPtr.size()); return _nodesPtr[idx];}
    Plane&     GetPlane(int iplane){ASSERT(iplane < (int)_planes.size()); return _planes[iplane];}
    Poly&      GetPoly(int idx) {ASSERT(idx < (int)_polys.size()); return _polys[idx];}
    BOOL       GetError(){return _bComplErr;}
    void       WantDroppedPolys(vvector<Poly>* polys){_pdroppedpolys = polys;}
    BOOL       IsDummy(){return _nodesPtr.size() ? (_nodesPtr[0]->_planeIdx ==-1) : TRUE;}
	virtual void    Finalize(){};
    BOOL    IsMainModel(){return _treeflags & BRSH_MAINBSP;}
protected:
    virtual void   AddNode(CMiniNode* pNn){pNn->_idxNodeThis = _nodesPtr.size(); _nodesPtr << pNn;}
    virtual void UpdateNodeBB(CMiniNode* pNode, PolyList& polys);
    virtual void UpdateNodeBB(CMiniNode* pNode, vvector<Poly>& polys);
    BOOL AllSpliters(PolyList& polyList);
    BOOL    _CheckSafeStack(int backSize);
    BOOL    BuildDummyBSP(vvector<Poly>& polys);
    int     FindPLane(Plane& p);
    void    MakeRoot(){ASSERT(_nodesPtr.size()==0); AddNode(new CMiniNode(this));};
	void	BuildPlaneArray(PolyList& polys, size_t flags);
	void    R_ClipPolyList(int,  vvector<Poly>& , vvector<Poly>&);
	void	R_Compile(int nodeIdx, PolyList&);
	void	R_TerrainCompile(int nodeIdx, PolyList&);
	int 	GetBestSplitter(PolyList& polys, Poly* pWantPoly=0);
	void	RepairSplits(vvector<Poly>& , vvector<Poly>& );
	void	Trace(int idxnode);
    virtual CMiniNode*  CreateNode(size_t dw){return new CMiniNode(this, dw);}
	int		Partition(CMiniNode* pNode, PolyList&, PolyList&,PolyList&);
	void	PartitionCSGTerrain(CMiniNode* pNode, PolyList&, PolyList&,PolyList&);
    Plane   GetCSGTerOptimSpliterPlane(PolyList& polys, int moe=1);
    void    FindCsgTerrSplitterPlane(PolyList& polys, CMiniNode* pNode);
	BOOL	_FormConvex(PolyList& frontPolys);
public:
	vvector<CMiniNode*>	_nodesPtr;
    vvector<Poly>	    _polys;
	vvector<Plane>	    _planes;
    vvector<Poly>*	    _pInPolys;
    vvector<Poly>*      _pdroppedpolys;
    vvector<Plane*>     _pSplPlanes;

    int		     _polyStartIdx;
    int          _nodeStartIdx;
    int          _leafStartIdx;

    // where the leafs are
    _CUT_FLAG    _cutflag;
    _BSP_TYPE    _bsptype;

    BOOL         _bRemUS;
	BOOL		 _bRemCoplan;
	BOOL		 _bClipSolid;
    BOOL         _bComplErr;
    BOOL         _bremES;
	Box			 _addedPolysBox;
    size_t        _timeStart;
    BOOL         _dlgWaitOn;
	int			 _reachedDepth;
    int          _balance;
	char		 _name[64];
    size_t        _treeprops;
    size_t        _treeflags;
    int          _lefmost;
	BOOL		 _btextconvex;
    int          _usersplitters;
    int          _safeStack[64];
    int          _nSafeStack;
    static       Poly*      _pWorkingPoly[2];
};

//------------------------------------------------------------------------------------------
INLN Plane& CMiniNode::GetPlane()const
{
    ASSERT(_planeIdx>=0 && _planeIdx < (int)_pBsp->_planes.size());  
	if(_planeIdx>=0 && _planeIdx < (int)_pBsp->_planes.size())
		return _pBsp->_planes[_planeIdx];
	return _pBsp->_planes[0];
};

INLN Poly*   CMiniNode::GetPolys(int& count){
    vvector<Poly>& polys = _pBsp->GetPolys();
    count = _nPolys;
    return &polys[_polyIdx];
};

INLN    Poly&   CMiniNode::GetOrigPoly()
{
    vvector<Poly>& polys = _pBsp->GetPolys();
    ASSERT(_origPolyIndex>=0);
    return polys[_origPolyIndex];
}

#endif // !__MINIBSP_H__
