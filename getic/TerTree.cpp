// TerTree.cpp: implementation of the TerTree class.
//
//---------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "texsys.h"
#include "TerTree.h"
#include "BigTerrain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//---------------------------------------------------------------------------------------
extern  Htex            _wLmap;
TerTree*     TerTree::p_TerrTree;
CBigTerrain* TerTree::p_Terrain;

//---------------------------------------------------------------------------------------
TerTree::TerTree()
{
    p_TerrTree = this;
    g_light.reserve(128*128*3);
    v_heights.reserve(128*128);
    p_leafs.reserve(32);
    p_leafs.reserve(64);
}

//---------------------------------------------------------------------------------------
TerTree::~TerTree()
{
    this->Clear();
}

//---------------------------------------------------------------------------------------
void	TerTree::Clear()
{
    v_heights.clear();
    p_leafs.deleteelements();
    p_nodes.deleteelements();
    g_light.clear();
}

//---------------------------------------------------------------------------------------
BOOL  TerTree::Compile(CBigTerrain* pbt)
{
    this->Clear();
    x_flags = 0;

    //tansfer props for owner brush poly to terrain
    this->x_flags=pbt->Flags();

    size_t            tiles   = {pbt->n_xtiles, pbt->n_ztiles};
    BYTE*           vxes    = &pbt->v_vxes;
    V3              corner  = pbt->b_box._min;
    V3              ext     = pbt->b_box._max - pbt->b_box._min;
    p_Terrain               = pbt;
    TerNode        *pRoot   = new TerNode();
    
    pRoot->i_parentNode = -1;
    pRoot->x_flags = 0;

    pRoot->n_idx = p_nodes.size(); 
    pRoot->l_idx = -1;
    p_nodes << pRoot;

    REAL    xS = (ext.x) / (REAL)tiles.cx;
    REAL    zS = (ext.z) / (REAL)tiles.cy;
    V3      vcur; 
    // scale and pos 'Y' them acording to world coord
    for(int x=0; x < tiles.cx; x++)
    {
        vcur.x = corner.x + (x * xS);
        for(int z=0; z < tiles.cy; z++)
        {
            vcur.z = corner.z + (z * zS);
            vcur.y = *(element(vxes, z, x, tiles.cx));
            // alredy scalled
            vcur.y /= 255.0;
            vcur.y *= ext.y;
            vcur.y += corner.y;
            pRoot->b_box.AddPoint(vcur);
            v_heights << vcur.y;
        }
    }
    int depth = -1;
    size_t    startSz = {0, 0};
    _R_Compile(pRoot, startSz, tiles, ext, corner, depth);
    return 1;
}

//---------------------------------------------------------------------------------------
int  TerTree::_R_Compile(TerNode  *pParentNode,
                          const size_t& startSz, 
                          const size_t& tiles, 
                          const V3& ext,
                          const V3& corner, 
                          int& depth)
{
    ++depth;

    int     x,z;
    size_t    tileDim;
    size_t    tileSF; //tiles start and and for front
    size_t    tileEF;
    size_t    tileSB; //tiles start and and for end
    size_t    tileEB;
    V3      lext = ext;

    if((depth & 1) == 0)    // x 
    {
        lext.x /= 2.0;
        tileDim.cx = (tiles.cx)/2;
        tileDim.cy = tiles.cy;
        pParentNode->d_split = 0;

        tileSF.cx = startSz.cx + tileDim.cx; 
        tileSF.cy = startSz.cy; 

        tileEF.cx = tileSF.cx + tileDim.cx;
        tileEF.cy = tileSF.cy + tileDim.cy;

        tileSB.cx = startSz.cx; 
        tileSB.cy = startSz.cy;  

        tileEB.cx = startSz.cx + tileDim.cx;        
        tileEB.cy = startSz.cy + tileDim.cy;


    }
    else
    {
        lext.z /= 2.0;
        tileDim.cx = tiles.cx;
        tileDim.cy = (tiles.cy)/2;
        pParentNode->d_split = 1;

        tileSF.cx = startSz.cx; 
        tileSF.cy = startSz.cy + tileDim.cy; 

        tileEF.cx = tileSF.cx + tileDim.cx;
        tileEF.cy = tileSF.cy + tileDim.cy;

        tileSB.cx = startSz.cx; 
        tileSB.cy = startSz.cy;  

        tileEB.cx = startSz.cx + tileDim.cx;        
        tileEB.cy = startSz.cy + tileDim.cy;


    
    }
        
    TerNode* pFront = new TerNode();
    pFront->i_parentNode = pParentNode->n_idx;
    pFront->n_idx = p_nodes.size();
    pFront->l_idx = -1;

    pParentNode->i_nodes[N_FRONT] = pFront->n_idx;
    p_nodes << pFront;

    REAL    xS = (lext.x) / (REAL)(tileDim.cx);
    REAL    zS = (lext.z) / (REAL)(tileDim.cy);
    V3      vcur; 

    for(x=tileSF.cx; x < tileEF.cx; x++)
    {
        vcur.x = corner.x + (x * xS);
        for(z=tileSF.cy; z < tileEF.cy; z++)
        {
            vcur.z = corner.z + (z * zS);
            vcur.y = *(element(&v_heights[0], x, z, tiles.cx));
            pFront->b_box.AddPoint(vcur);
        }
    }
    pFront->b_box.Expand(1024);

    
    

    TerNode* pBack = new TerNode();
    pBack->i_parentNode = pParentNode->n_idx;
    pBack->n_idx = p_nodes.size();
    pBack->l_idx = -1;
    pParentNode->i_nodes[N_BACK] = pBack->n_idx;
    p_nodes << pBack;

    for(x=tileSB.cx; x < tileEB.cx; x++)
    {
        vcur.x = corner.x + (x * xS);
        for(z=tileSB.cy; z < tileEB.cy; z++)
        {
            vcur.z = corner.z + (z * zS);
            vcur.y = *(element(&v_heights[0], x, z, tiles.cx));
            pBack->b_box.AddPoint(vcur);
        }
    }
    pBack->b_box.Expand(1024);

    if(tileDim.cx > TerLeafCelTiles || tileDim.cy > TerLeafCelTiles)
    {
        _R_Compile(pFront, tileSF, tileDim, lext, corner, depth);
        _R_Compile(pBack, tileSB, tileDim, lext, corner, depth);
    }
    else
    {
        //make 2 leafs on front and on back on node indexes
        TerLeaf* pTF    = new TerLeaf(); //front
        pFront->l_idx   = p_leafs.size();
        pTF->t_idx      = pFront->l_idx;
        pTF->n_idx      = pFront->n_idx;
        pTF->s_tiles[0] = tileSF;
        pTF->s_tiles[1] = tileEF;
        this->p_leafs << pTF;

        //add vxes
        
        TerLeaf* pTB = new TerLeaf(); // back
        pBack->l_idx = p_leafs.size();
        pTB->t_idx = pBack->l_idx;
        pTB->n_idx = pBack->n_idx;
        pTB->s_tiles[0] = tileSB;
        pTB->s_tiles[1] = tileEB;
        this->p_leafs << pTB;

    }

    --depth;
    return depth;
}

//---------------------------------------------------------------------------------------
TerLeaf::~TerLeaf()
{
    GTexSys.RemoveTexture(lm_Tex);
}

//---------------------------------------------------------------------------------------
void   TerTree::Render(BOOL d3)
{
    const V3& pos = DOC()->_cam._pos;

    if(x_flags & TERR_FULLBRGHT)
    {
        glColor4ubv(CLR_WHITE);
    }
    _R_Render(pos, 0, d3);
}

//---------------------------------------------------------------------------------------
void TerTree::_RenderLeaf(const TerLeaf* pLeaf, 
                          int bLmtexCo, // 1 default for stage,  
                          int texLayer, // 
                          BOOL d3, 
                          CLR& color,
                          int clrdiv)
{
    if(0==pLeaf)return;
    if(0 == clrdiv)clrdiv=1; //div by 0

    int   x,z;
    V3    v0,v1;
    const size_t& s   = pLeaf->s_tiles[0];
          size_t  e   = pLeaf->s_tiles[1];
    const size_t  t   = {p_Terrain->n_xtiles,p_Terrain->n_ztiles};
    const V3&   c   = p_Terrain->b_box._min;
    const V3&   ex  = p_Terrain->b_box.GetExtends();
    const REAL  xS  = (ex.x) / (REAL)p_Terrain->n_xtiles;
    const REAL  zS  = (ex.z) / (REAL)p_Terrain->n_ztiles;
    const BOOL  bG  = g_light.size()!=0;
    CLR         cL;
    CLRNOA      clrNoA;
    UV          tc0,tc1;
    UV          lMsz(TerLeafCelTiles*xS, TerLeafCelTiles*zS);
    REAL        fx,fx1,fz;
    size_t       texLayerBits = (1<<texLayer);
    BOOL        untt[4] = {x_flags & TERR_UTT0,
                           x_flags & TERR_UTT1,
                           x_flags & TERR_UTT2,
                           x_flags & TERR_UTT3};

    color.r /= clrdiv;
    color.g /= clrdiv;
    color.b /= clrdiv;
    glColor4ubv((BYTE*)color);

    for(x=s.cx; x < e.cx; x++)
    {
        if(d3)
            glBegin(GL_TRIANGLE_STRIP);
        else
            glBegin(GL_LINE_STRIP);

        fx   = x * xS;
        fx1  = (x+1) * xS;
        v0.x = c.x + (fx);
        v1.x = c.x + (fx1);
        
        for(z=s.cy; z <= e.cy; z++)
        { 
            if(z==p_Terrain->n_ztiles)
                break;
            fz = z * zS;
            v0.z = c.z + (fz);
            v0.y = *(element(&v_heights[0], x, z, t.cx));
            v1.z = v0.z;
            v1.y = *(element(&v_heights[0], (x+1), z, t.cx));

            if(d3)
            {
                if(bG)
                {
                    clrNoA = *(element(&g_light[0], x, z, t.cx));
                    clrNoA.r /= clrdiv;
                    clrNoA.g /= clrdiv;
                    clrNoA.b /= clrdiv;
                    glColor3ubv(clrNoA);
                }
                if(untt[texLayer])
                {
                     if(bLmtexCo)
                     {
                         if(lMsz.v)lMsz.v=EPSILON;
                         if(lMsz.u)lMsz.u=EPSILON;
                         tc0.setval(fx/lMsz.u , 1-fz/(lMsz.v));
                     }
                     else
                     {
                         tc0.setval((v0.x-c.x)/ex.x , 1-(v0.z-c.z)/ex.z);
                     }
                }
                else
                {
                    if(bLmtexCo && texLayer==1)
                    {
                        if(lMsz.v)lMsz.v=EPSILON;
                        if(lMsz.u)lMsz.u=EPSILON;
                        tc0.setval(fx/lMsz.u , 1-fz/lMsz.v);
                    }
                    else
                    {
                        if((z&1)==0)
                            tc0.setval(0,0);
                        else
                            tc0.setval(0,1);
                    }
                }
            }

            
            if(d3)
            {
                if(bG)
                {
                    clrNoA = *(element(&g_light[0], x+1, z, t.cx));
                    clrNoA.r /= clrdiv;
                    clrNoA.g /= clrdiv;
                    clrNoA.b /= clrdiv;
                    glColor3ubv(clrNoA);

                }
                if(untt[texLayer])
                {
                    if(bLmtexCo)
                    {
                        if(0==lMsz.u)lMsz.u=EPSILON;
                        if(0==lMsz.v)lMsz.v=EPSILON;
                        tc1.setval(fx1/lMsz.u , 1-fz/lMsz.v);
                    }
                    else
                    {
                        tc1.setval((v1.x-c.x)/ex.x , 1-(v1.z-c.z)/ex.z);
                    }
                }
                else
                {
                    if(bLmtexCo && texLayer==1)
                    {
                        if(0==lMsz.u)lMsz.u=EPSILON;
                        if(0==lMsz.v)lMsz.v=EPSILON;
                        tc1.setval(fx1/lMsz.u , 1-fz/lMsz.v);
                    }
                    else
                    {
                        if((z&1)==0)
                            tc1.setval(1,0);
                        else
                            tc1.setval(1,1);
                    }
                }
            }
            if(texLayerBits & p_Terrain->n_stage)
            {
                p_Terrain->AnimateSurfaceTexture(tc0, tc1);
            }

            glTexCoord2fv((REAL*)tc0);
            glVertex3fv((REAL*)v0);

            glTexCoord2fv((REAL*)tc1);
            glVertex3fv((REAL*)v1);
        }
        glEnd();
    }
}

//---------------------------------------------------------------------------------------
void TerTree::_R_Render(const V3& pos, int node, BOOL d3)
{
    const TerNode*  pNode = p_nodes[node];
    const size_t     dmode = DOC()->_compviewmode;// & C_LMAP
    Texs&           tex   = p_Terrain->Texs();
    CLR             rgbc  = ZWHITE ;
    
    if(d3)
    {
        if(DOC()->_cam.IsCulled(&pNode->b_box._min, &pNode->b_box._max))
            return;
        rgbc.setval(CLR_WHITE);
    }
    else
    {
        if(DOC()->_cam.IsCulled(&pNode->b_box._min, &pNode->b_box._max))
        {
            rgbc.setval(CLR_NORMAL);
        }
        else
        {
            rgbc.setval(CLR_WHITE);
        }
    }
    //assert(pNode->l_idx < p_leafs.size());
    if(pNode->l_idx >= 0 && pNode->l_idx < p_leafs.size()) 
    {
        
        TerLeaf* pLeaf = p_leafs[pNode->l_idx];
        if(d3)
        {
            if(dmode & C_LMAP)   // show light maps only
            {
                if(this->x_flags & TERR_CUSTLM)
                {
                    Ta.Bind(tex[1], tex._texApply[1]);        // custom texture
                    _RenderLeaf(pLeaf, 0, 1, d3, rgbc);   // render time mode tex
                }
                else
                {
                    if(x_flags & TERR_FULLBRGHT)    //  TERR_NOLIGHT is taken car e in lmap proc
	    	             Ta.Bind(_wLmap, 0);
    	            else 
		                Ta.Bind(pLeaf->lm_Tex);
                    _RenderLeaf(pLeaf, 1, 1, d3, rgbc);   //render lmap and lmap coords
                }
            }
            else
            {
                BOOL binded=FALSE;
                
                for(int ts=0; ts < 4; ++ts)
                {
                    binded=FALSE;
                    switch(ts)
                    {
                        case 0:
                            Ta.Bind(tex[ts], tex._texApply[ts]);
                            _RenderLeaf(pLeaf, 0, ts, d3, rgbc);   
                            break;
                        case 1:
                            if(this->x_flags & TERR_CUSTLM && tex[1])
                            {
                                Ta.Bind(tex[1], tex._texApply[ts]);
                                binded=1;
                            }
                            else 
                            {
                                if(x_flags & TERR_FULLBRGHT)    //  TERR_NOLIGHT is taken car e in lmap proc
                                {
	    	                        Ta.Bind(_wLmap, 0);
                                    binded=1;
                                }
    	                        else if(pLeaf->lm_Tex)
                                {
		                            Ta.Bind(pLeaf->lm_Tex, 0);
                                    binded=1;
                                }
                            }
                            if(binded)
                            {
                                TexRef::GlLmapMode(1);
                                _RenderLeaf(pLeaf, 0, ts, d3, rgbc);   
                                TexRef::GlLmapMode(0);
                            }
                            break;
                        case 2:
                        case 3:
                            if(Ta.Bind(tex[ts], tex._texApply[ts]))
                            {
                                TexRef::GlDetailMode(1);
                                _RenderLeaf(pLeaf, 0, ts, d3, rgbc, 1);   
                                TexRef::GlDetailMode(0);
                            }
                            break;
                    }
                }
            }
        }
        else
        {
            _RenderLeaf(pLeaf, 0, 0, d3, rgbc);
        }
    }
    else
    {
        if(pNode->i_nodes[0]>=0)
            _R_Render(pos, pNode->i_nodes[0], d3);

        if(pNode->i_nodes[1]>=0)
            _R_Render(pos, pNode->i_nodes[1], d3);
    }
}


//---------------------------------------------------------------------------------------
BOOL TerTree::SegmentIntersect(const V3& a, const V3& b)
{
    


    const REAL  slen = (b-a).len();// a + (b-a)*t
    const V3    ba   = b-a;
    const V3&   ex   = p_Terrain->b_box.GetExtends();
    const REAL  step = 1.0 / p_Terrain->n_xtiles;
    V3          np;    // new point
    V3          pp   = a;
    

    for(REAL f = step; f < 1.00000; f+=step)
    {
        np = a + ba * f; // see if height is below surface
                         // then collide prev this with cel tringles

        

        pp = np;
    }
    // go from b to a in tiledims
    return 0;
}


