// BigTerrain.cpp: implementation of the CBigTerrain class.
//
//---------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z_edmap.h"
#include "BigTerrain.h"
#include "z_ed2View.h"
#include "z_ed3View.h"
#include "z_ed2Doc.h"

//---------------------------------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------------------
void CBigTerrain::Serialize(FileWrap* pfv, 
                            const vvector<Texture>& texNames, 
                            map<int,int>& texassoc)
{

    pfv->Serialize(n_xtiles);
    pfv->Serialize(n_ztiles);
    pfv->Serialize(b_box);
    pfv->Serialize(n_stage);
    pfv->Serialize(t_anim[0]);
    pfv->Serialize(t_anim[1]);

    pfv->Serialize(m_dummy[0]);
    pfv->Serialize(m_dummy[1]);
    pfv->Serialize(m_dummy[2]);
    pfv->Serialize(m_dummy[3]);
    pfv->Serialize(m_dummy[4]);
    pfv->Serialize(m_dummy[5]);
    pfv->Serialize(m_dummy[6]);
    pfv->Serialize(m_dummy[7]);

    if(pfv->_store)
    {
        int nVxes = v_vxes.Size();
        pfv->Write(nVxes);
        for(int i=0;i<nVxes;i++)
        {
            pfv->Write(v_vxes[i]);
        }
    }
    else
    {
        BYTE hm;
        int nVxes;
        pfv->Read(nVxes);
        v_vxes.ObjReserve(nVxes);
        for(int i=0; i<nVxes ;i++)
        {
            pfv->Read(hm);
            v_vxes[i] = hm;
        }
    }    
}

//---------------------------------------------------------------------------------------
void  CBigTerrain::Render(BOOL d3)
{
    if(!n_xtiles || 0==p_dummyBrush) return;
    V3              ex = b_box.GetExtends();
    REAL            xS = (ex.x) / (REAL)n_xtiles;
    REAL            zS = (ex.z) / (REAL)n_ztiles;
    V3              vWalk1,vWalk2, vmin = b_box._min;
    BYTE            lc[4];
    UV              tc0,tc1;
    DWORD           texAstage = 1<<GUtex;
    BOOL            untt[4] = {Flags()&TERR_UTT0,
                               Flags()&TERR_UTT1,
                               Flags()&TERR_UTT2,
                               Flags()&TERR_UTT3};


    if(p_dummyBrush->IsSelected())
        glColor4ubv(CLR_WHITE);
    else
    {
        if(d3==0)
            glColor4ubv(CLR_TERRAIN);
        else
            glColor4ubv(CLR_NORMAL);
    }

    if(d3)
    {
        Ta.Reset();
        Ta.Bind(Texs(), GUtex);
    }

    for(int x=0 ; x < n_xtiles; x++)
    {
        if(d3)
        {
            glBegin(GL_TRIANGLE_STRIP);
        }
        else
            glBegin(GL_LINE_STRIP);

        vWalk1.x = vmin.x + x * xS;
        vWalk2.x = vmin.x + (x+1) * xS;

        for(int z=0 ; z < n_ztiles; z++)
        {
            
            vWalk1.z = vmin.z + z * zS;
            vWalk1.y = *(element(&v_vxes, x, z, n_xtiles));
            vWalk1.y /= 255.0;
            vWalk1.y *= ex.y;
            vWalk1.y += vmin.y;

            vWalk2.z = vWalk1.z;
            vWalk2.y = *(element(&v_vxes, (x+1), z, n_xtiles));
            vWalk2.y /= 255.0;
            vWalk2.y *= ex.y;
            vWalk2.y += vmin.y;

            if(GCullTerrCell && DOC()->_cam.IsCulled(&vWalk1) && DOC()->_cam.IsCulled(&vWalk2))
            {
                continue;
            }
            
            if(d3==0 && !p_dummyBrush->IsSelected())
            {
                lc[0] = CLR_TERRAIN[0] + ((vWalk1.y-b_box._min.z) / b_box.GetExtends().z * 64);
                lc[1] = CLR_TERRAIN[1] + ((vWalk1.y-b_box._min.z) / b_box.GetExtends().z * 64);
                lc[2] = CLR_TERRAIN[2] + ((vWalk1.y-b_box._min.z) / b_box.GetExtends().z * 64);
                lc[3] = CLR_TERRAIN[3];
                glColor4ubv(lc);
            }

            if(untt[GUtex])    
            {
                tc0.setval((vWalk1.x-vmin.x)/ex.x, 1-(vWalk1.z-vmin.z)/ex.z);
            }
            else
            {
                if((z&1)==0)
                    tc0.setval(0,0);
                else
                    tc0.setval(0,1);
            }

            if(untt[GUtex])    
            {
                tc1.setval((vWalk2.x-vmin.x)/ex.x, 1-(vWalk2.z-vmin.z)/ex.z);
            }
            else
            {
            if((z&1)==0)
                tc1.setval(1,0);
            else
                tc1.setval(1,1);
            }
            if(d3 && (n_stage & texAstage))
            {
                AnimateSurfaceTexture(tc0, tc1);
            }

            glTexCoord2fv((REAL*)tc0);
            glVertex3fv((REAL*)vWalk1);
            glTexCoord2fv((REAL*)tc1);
            glVertex3fv((REAL*)vWalk2);
                        
        }
        glEnd();
    }
}

//---------------------------------------------------------------------------------------
void CBigTerrain::AnimateSurfaceTexture(UV& tc0, UV& tc1)
{
    DWORD animbits = n_stage & 0xF0000000;
    if(animbits == TERR_TA_SHIFT)
    {
        UV depl(t_anim[0].u * PaintCounter, t_anim[0].v * PaintCounter);
        tc0 += depl;
        tc1 += depl;
    }
    else if(animbits == TERR_TA_OSC) //trmo
    {
        UV depl(t_anim[0].u*Sinr((REAL)PaintCounter/16.0), t_anim[0].v*Cosr((REAL)PaintCounter/16.0));
        tc0 += depl;
        tc1 += depl;
    }
}




//---------------------------------------------------------------------------------------
BOOL  CBigTerrain::Create(const BYTE* hmap, const V3& start, const V3& size, 
                          int xt, int yt)
{
    Clear(1);

	p_dummyBrush  = new Brush();
	p_dummyBrush->MakeCube((V3&)size);
	p_dummyBrush->Recalc();
    p_dummyBrush->_brushflags    = BRSH_BIGTERRAIN|TERR_GOR|TERR_UTT0;
    p_dummyBrush->_pUsrData = this;
    p_dummyBrush->Reverse();
    p_dummyBrush->_pUsrData=this;
    strcpy(p_dummyBrush->_name,"terrain");
    

    n_xtiles     = xt;
    n_ztiles     = yt;
    b_box._min   = start;
    b_box._max   = start + size;
    BYTE* pwhmap = (BYTE*)hmap;
    int k=0;

    BYTE  bmin=255;  
    BYTE  bmax=0;  

    v_vxes.ObjReserve((n_xtiles+1)*(n_ztiles+1));
    for(int x=0 ; x <= n_xtiles; x++)
    {
        for(int z=0 ; z <= n_ztiles; z++)
        {
            bmin = tmin(*pwhmap,bmin);
            bmax = tmax(*pwhmap,bmax);
            v_vxes[k++] = *pwhmap++;
        }
    }
    return 1;
}

//---------------------------------------------------------------------------------------
void  CBigTerrain::Clear(BOOL delB)
{
    if(delB && p_dummyBrush)
        DOC()->DeleteBrush(p_dummyBrush,0);
    p_dummyBrush = 0;
    v_vxes.Destroy();
    n_xtiles=0;
    n_ztiles=0;
}

//---------------------------------------------------------------------------------------
CBigTerrain& CBigTerrain::operator = (const CBigTerrain& cbt)
{
    if(&cbt != this)
    {
        Clear(1);
        if( ((CBigTerrain&)cbt).Exist() )
        {
            p_dummyBrush = new Brush(*cbt.p_dummyBrush);
            p_dummyBrush->_pUsrData=this;
        }
        v_vxes       = cbt.v_vxes;
        n_xtiles     = cbt.n_xtiles;
        n_ztiles     = cbt.n_ztiles;
        b_box        = cbt.b_box;
        n_stage      = cbt.n_stage;
        t_anim[0]    = cbt.t_anim[0];
        t_anim[1]    = cbt.t_anim[1];
        memcpy(m_dummy,cbt.m_dummy,sizeof(m_dummy));
        _clearing=0;
    }
    return *this;
}

//---------------------------------------------------------------------------------------
CBigTerrain::CBigTerrain(const CBigTerrain& cbt )
{
    if( ((CBigTerrain&)cbt).Exist())
    {
        p_dummyBrush = new Brush(*cbt.p_dummyBrush);
        p_dummyBrush->_pUsrData=this;
    }
    v_vxes       = cbt.v_vxes;
    n_xtiles     = cbt.n_xtiles;
    n_ztiles     = cbt.n_ztiles;
    b_box        = cbt.b_box;
    n_stage      = cbt.n_stage;
    t_anim[0]    = cbt.t_anim[0];
    t_anim[1]    = cbt.t_anim[1];
    memcpy(m_dummy,cbt.m_dummy,sizeof(m_dummy));
    _clearing = 0;
}


/*
    anim textures u and v is shift
*/