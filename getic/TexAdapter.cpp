
#include "stdafx.h"
#include "TexAdapter.h"

Tadapt       Ta;  

size_t Targets[]={   GL_TEXTURE_2D,
                   GL_TEXTURE_CUBE_MAP,
                   GL_TEXTURE_2D};

 
BOOL Tadapt::Bind(Htex& tex, size_t objFlags)
{
    if(!_enabled)  return 0;
    REG BOOL  force = 0;
    if(tex.glTarget>4)tex.glTarget=0;
    size_t     tgt   = Targets[TGET_TARGET(tex.glTarget)];
    size_t     gen   = TGET_GENST(objFlags);

    if(tgt != _glTarget)
    {
        if(_glTarget)
            glDisable(_glTarget);
        _glTarget = tgt;
        force = 1;
    }

    if(gen != _glGen || force)
    {
        _glGen = gen;
        switch(gen)
        {
            case GEN_TEX_GEN_NA: // 0
	            glDisable(GL_TEXTURE_GEN_S);
	            glDisable(GL_TEXTURE_GEN_T);
                glDisable(GL_TEXTURE_GEN_R); 
                break; 
            case GEN_TEX_GEN_ST_CUBE:
                glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
                glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
                glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
                glEnable(GL_TEXTURE_GEN_R);
                break;
            case GEN_TEX_GEN_ST_SPH:
	            glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	            glEnable(GL_TEXTURE_GEN_S);
	            glEnable(GL_TEXTURE_GEN_T);
                break;        
        }
   }
    // apply envmodes
    glEnable(tgt);
    if(0==(int)tex.hTex)
    {
        glBindTexture(tgt, _dumptex.hTex);
        return FALSE;
    }
    glBindTexture(tgt, tex.hTex);
    return TRUE;
}

BOOL Tadapt::Binds(Htex* pTexts, size_t comb)
{
    /*
    REG int   stage;
    REG size_t bmask = 1;

    glActiveTextureARB(GL_TEXTURE0_ARB); 
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT); 
    Bind(pTexts[0].hTex, pTexts[0].glTarget);

    switch(comb & 0xF)
    {
        case 1:     // +N -LM -D1 -D2 -D3 
            glActiveTextureARB(GL_TEXTURE1_ARB); 
    		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT); 
	    	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_BLEND); 
            Bind(tc.GetHtex(1), tc.Apply(1));
            break;
        case 2:     // -N +LM -D1 -D2 -D3 
            glActiveTextureARB(GL_TEXTURE2_ARB); 
    		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT); 
	    	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD_SIGNED_EXT); 
            Bind(tc.GetHtex(2), tc.Apply(2));
            break;
        case 3:     // +N +LM -D1 -D2 -D3 
        case 4:     // +N -LM -D1 -D2 -D3 
            glActiveTextureARB(GL_TEXTURE3_ARB); 
    		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT); 
	    	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD_SIGNED_EXT); 
            Bind(tc.GetHtex(3), tc.Apply(3));
            break;
        case 5:     // +N -LM -D1 -D2 -D3     
        case 6:     // +N -LM -D1 -D2 -D3 
        case 7:     // +N -LM -D1 -D2 -D3 
    }
    */
    return 0;
}

BOOL Tadapt::Bind(const TexCarrier& tc, int stage)
{
    return  Bind(((TexCarrier&)tc).GetHtex(stage), ((TexCarrier&)tc).Apply(stage));
}

