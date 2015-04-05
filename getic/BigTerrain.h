// BigTerrain.h: interface for the CBigTerrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIGTERRAIN_H__EF7333B8_FA84_4674_8ED7_92588B8D5238__INCLUDED_)
#define AFX_BIGTERRAIN_H__EF7333B8_FA84_4674_8ED7_92588B8D5238__INCLUDED_


#include "stdafx.h"     // for GBackface
#include "..\\_include\\BspFIleStr.h"
#include "baseutils.h"
#include "TexRef.h"
#include "Brush.h"
#include "TexAdapter.h"
#include "BaseEntity.h"

class z_ed3View;
class CZ_ed2View;
class CBigTerrain 
{
public:
    CBigTerrain():n_xtiles(0),p_dummyBrush(0),n_stage(0),_clearing(0){ };
    CBigTerrain(const CBigTerrain& cbt);
    virtual ~CBigTerrain(){Clear(1);};
    void  Serialize(FileWrap* pfv, const vvector<Texture>& texNames, map<int,int>& texassoc);
    void  Render(BOOL d3=FALSE);
    void  Clear(BOOL delB=FALSE);//{v_vxes.clear();n_xtiles=0;n_ztiles=0;}
    void  Destroy(){Clear();}
    CBigTerrain& operator=(const CBigTerrain& cbt);
    BYTE* Vertextex(int& ncount){ncount = v_vxes.Size(); return &v_vxes;}
    BOOL  Exist() {return v_vxes.Size();}
    BOOL  Create(const BYTE* hmap, const V3& start, const V3& size, int xt, int yt);
    Texs&   Texs(){
        assert(p_dummyBrush);
        return p_dummyBrush->_polys[0].GetTexs();}
    DWORD&  Flags(){assert(p_dummyBrush); return  p_dummyBrush->_brushflags;}
    void AnimateSurfaceTexture(UV& tc0, UV& tc1);

public:
    UINT            n_xtiles;
    UINT            n_ztiles;
    Box             b_box;
    
    RawBuff<BYTE>   v_vxes;
    Brush*          p_dummyBrush; // used to resize and move the terrain
    DWORD           n_stage;
    UV              t_anim[2];
    DWORD           m_dummy[8];
    BOOL            _clearing;
};

#endif // !defined(AFX_BIGTERRAIN_H__EF7333B8_FA84_4674_8ED7_92588B8D5238__INCLUDED_)
