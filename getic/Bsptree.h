// Author: Marius Chincisan: 2003
//---------------------------------------------------------------------------------------
#ifndef __BSPTREE_H__
#define __BSPTREE_H__
//   #pragma warning (disable: 4786)
//---------------------------------------------------------------------------------------


#include "Poly.h"
#include "basecont.h"
#include "Minibsp.h"


//---------------------------------------------------------------------------------------
class SceItem;
class CLeaf : public CMiniNode
{
public:
    CLeaf(MiniBsp* pBsp, size_t dw=0):CMiniNode(pBsp, dw),
            _pNext(0),
            _pvsIdx(-1),
            _portIdxes(8),
            _iModIndex(8),
            _iItems(8)
            
    {
        _zonearea=(-1);
    }
    virtual ~CLeaf(){_portIdxes.clear();_iModIndex.clear();}
    void     AddPortalIdx(int idx){
        _portIdxes.push_back(idx);
    };
    vvector<int>&   GetPortalIdxes(){return _portIdxes;}

    CLeaf*          _pNext;
	int				_pvsIdx;
    vvector<int>    _iModIndex;          // linked module(bsp) index from compiler bsp's
    vvector<int>    _portIdxes;          // in the port proceesor
    vvector<int>    _iItems;
};

//---------------------------------------------------------------------------------------
// caries the colided point properties
class CBspTree;
struct CollPack
{
    CollPack(){
        ::memset(this,0,sizeof(*this));
        _dist = INFINIT;
    }
    CollPack& operator=(const CollPack& rc){
        if(this != &rc){
            ::memcpy(this, &rc,sizeof(*this));
        }
        return  *this;
    }
    V3          _ip;        //  ip 
    int         _nLeaf;     //  hit leaf
    Plane*      _pPlane;    //  hit Plane
    Poly*       _pPoly;     //  hit Polygon
    CBspTree*   _pTree;     //  collided tree
    REAL        _dist;
};

//---------------------------------------------------------------------------------------
class CZ_ed2View;
class z_ed3View;
class PortalPRC;
class CBspTree  : public MiniBsp
{
public:
	BOOL BuildPortals();
	CBspTree();
	virtual ~CBspTree();
    virtual void Clear();                       // virtual from base 
	virtual BOOL Compile(vvector<Poly>& polys, size_t brFlags, BOOL bremES=FALSE, BOOL bTextCvx=FALSE); // virtual from base
    CLeaf*  GetLeaf(size_t idx){ASSERT(idx < _leafs.size()); return _leafs[idx];}
    void    Draw3DBSPTree(z_ed3View* pV, int& leaf,int curLeaf, size_t mode);
	void    Draw2GlTree(CZ_ed2View*,size_t,int,BOOL);
    void    LinkDetailModel(CBspTree* pModelTree, BOOL bAtLeaf=TRUE);
	int		R_FindLeaf(int nodeIdx,V3& pos);
    int     GetLeafsCount(){return _leafs.size();}
    vvector<CLeaf*>& GetLeafs() {return _leafs;}
    void    SetMainModel(){_treeprops |= BRSH_MAINBSP;}
    //
    int     AddUniquePlane(vvector<Plane>&  allPlanes, map<int,int>& plIdxMap);
    void    ReindexPlanes(map<int,int>& pidxes);
    int     CollectPNL(vvector<Poly*>&  allPolys, vvector<CMiniNode*>& allNodes, vvector<CLeaf*>& allLeafs);
    BOOL    SegmentIntersect(V3& a, V3& b, BOOL bSubModels, CollPack& coll);
	void    LinkItem(SceItem* pItem, BOOL atLeaf);
	void	ConcaveConvex();
    virtual void    Finalize();
protected:
    // drawing
    void    Draw_Graph(z_ed3View* pV);
    void    DrawPolys(Poly* pLeaf,int ncount, size_t mode,  CLR* lfc=0,
                                                               vvector<Poly*>* transPolys=0,
                                                               vvector <Poly*>* backDraw=0,
                                                               vvector <Poly*>* mirrors=0,
                                                               vvector <Poly*>* masked=0,
                                                               vvector <Poly*>* guround=0);
    void    Draw_BSP_WithLmapsAndTextures(z_ed3View* pV, int& leaf, int curLeaf, size_t mode);
    void    Draw_SelectedLeafByLeaf(z_ed3View* pV, int& leaf, int curLeaf,size_t mode);
    void    DoVisByLeafs();
    void    PatchPolys(vvector<Poly>&, vvector<Poly>&);
    BOOL    SegmentIntersectPolygons(Poly* pStart, int nCount, V3& a, V3& b, CollPack& col,BOOL faces=1);
    BOOL    SegmentIntersectConvexLeaf(CMiniNode* pNode, V3& a, V3& b, CollPack& col);
    BOOL    R_SegmentIntersect(V3& a, V3& b, int node, CollPack& coll);

	virtual void UpdateNodeBB(CMiniNode* pNode, list<Poly>& polys);
    virtual void UpdateNodeBB(CMiniNode* pNode, vvector<Poly>& polys);
    void    R_LinkDetailModel(int node, CBspTree* pModelTree);
	void    R_LinkItem(int node, SceItem* pItem);
    void    R_DrawGraph(int node, V3 pos, int prevDir, z_ed3View* pV);
    CMiniNode*  CreateNode(size_t dw){
        if(dw & NODE_SOLID)
            return MiniBsp::CreateNode(dw);
        return new CLeaf(this, dw);
    }
    void   AddNode(CMiniNode* pNn){
        MiniBsp::AddNode(pNn);
        if(pNn->IsEmptyLeaf())
        {
            pNn->_leafIdx = _leafs.size();
            _leafs << (CLeaf*)pNn;
        }
    }
    void    R_CreateLeafClusters(int nNode);
public:    
    int                 _thisIdx;
	PortalPRC*		    _prtProc;
    vvector<CLeaf*>     _leafs;
    vvector<CBspTree*>  _pSubModels;
    vvector<int>        _sounds;
    V3                  _rotSteps;
	int				    _itemStartIdx;
    int                 _motionsIdx;
    BOOL                _patchPolys;
    size_t               _zoneareea;
private:
    BOOL                _userFlag;
    int                 _clusterCnt;
};




#endif // !defined(AFX_BSPTREE_H__C358667F_7226_4799_BB82_E27CC4CA51B3__INCLUDED_)
