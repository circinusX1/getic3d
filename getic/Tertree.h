#ifndef __TER_TREE_H__
#define __TER_TREE_H__

#include "baselib.h"
#include "TexAdapter.h"

//---------------------------------------------------------------------------------------
class NO_VT TerNode
{
public:
    TerNode(){}
    ~TerNode(){}

    int     n_idx;
    int     l_idx;
    Box     b_box;
    int     i_nodes[2];   // if index is - is an index in leafs struct
    int     i_parentNode;
    DWORD   x_flags;    // x_ for dword types
    int     d_split;    // direction split 0 on x 1 on (z/y)
};

#define LEAF_LIT    0x1
//---------------------------------------------------------------------------------------
class NO_VT TerLeaf
{
public:
    TerLeaf(){}
    ~TerLeaf();//{GTexSys.RemoveTexture(lm_Tex);}
    SIZE            s_tiles[2];     // start tile, end tile
    LmInfo          lm_info;        // in ttree lmapbuffer
    Htex            lm_Tex;         // lm texture
    Box             b_box;          // leaf box
    int             n_idx;          // node index
    int             t_idx;          // leaf index
    DWORD           x_flags;
};

//---------------------------------------------------------------------------------------
class CBigTerrain;
class TerTree  
{
public:
	TerTree();
	virtual ~TerTree();

    virtual void	Clear();
    BOOL            Compile(CBigTerrain* pbt);
    BOOL            Exist(){return p_leafs.size();}
    void            Render(BOOL d3);
    BOOL            SegmentIntersect(const V3& a, const V3& b);
protected:
    void _RenderLeaf(const TerLeaf* pLeaf, int blm, int tilemode, BOOL d3, CLR& color, int clrdiv=1);
    void _R_Render(const V3& pos, int node, BOOL d3);
    int  _R_Compile(TerNode  *pParentNode,
                          const SIZE& startSz, 
                          const SIZE& tiles, 
                          const V3& ext,
                          const V3& corner, int& depth);
    

public:
    vvector<REAL>       v_heights;
    vvector<TerLeaf*>   p_leafs;
    vvector<TerNode*>   p_nodes;
    vvector<CLRNOA>     g_light;        // gouraoud light
    static TerTree*     p_TerrTree;
    static CBigTerrain* p_Terrain;
    DWORD               x_flags;
};


#endif // !__TER_TREE_H__



/*

*/

