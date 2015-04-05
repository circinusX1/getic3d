#ifndef __TEXADAPTER_H__
#define __TEXADAPTER_H__

#include <gl/gl.h>			
#include "extern/glext.h"	
#include "extern/glu.h"			
#include "extern/glaux.h"	
#include "TexRef.h"

extern 	Htex		_dumptex;
extern  UINT        GUtex;    
#define     FLAGS_DISABLED  (0x80000000)


//---------------------------------------------------------------------------------------
struct LmInfo
{
    LmInfo():_flags(0),_lmIndex(-1){}

    int     _lmIndex;
    SIZE    _size;
    DWORD   _flags;
    V3      _lmAxes[3];
};

//---------------------------------------------------------------------------------------
class TexCarrier 
{
public:
    TexCarrier():_combine(0){}
    ~TexCarrier(){}
    void            SetCombine(DWORD d){_combine=d;};
    DWORD           Combine(){return _combine;};
    Texs&           GetTexs(){return _textures;}
    Htex&           GetHtex(int index){return _textures._texts[index];}
    void            SetHtex(Htex& ht, int index){_textures._texts[index]=ht;}
    void            SetApply(DWORD apply, int index){_textures._texApply[index]=apply;}
    DWORD&          Apply(int index){return _textures._texApply[index];}
    Htex&           SetTex(const char* name, int stage=GUtex, DWORD flags=0){
        Htex& rv = _textures[stage].Assign(name,flags);
        if((int)rv)
            _combine |= (1<<stage);
        else
            _combine &= ~(1<<stage);
        return rv;
    }
protected:
    DWORD           _combine;
    Texs            _textures;
};

//=================================================================================
class NO_VT Tadapt
{
public:
    Tadapt():_glTarget(0),_glGen(0),_enabled(0),old_enabled(1){}

    BOOL Binds(Htex* pTexts, DWORD comb);
    BOOL Bind(const TexCarrier& pTexts, int stage=0);
    BOOL Bind(Htex& tex, DWORD flags=0);
    INLN BOOL Bind(TexCarrier* pTexts, int stage){return Bind(*pTexts, stage);}
    INLN BOOL Bind(Texs& txs, int stage=0){return Bind((Htex&)txs._texts[stage], txs._texApply[stage]);};
    INLN void Disable();
    INLN void Enable();
    INLN void Reset();
    INLN void Restore();

private:
    
    DWORD   _glTarget;
    DWORD   _glGen;
    BOOL    _enabled;
    BOOL    old_enabled;
    HTEX    _hTex;
};

extern Tadapt      Ta; 
extern UINT Targets[];


INLN void Tadapt::Enable()
{
    if(_enabled==1) return;
    old_enabled = _enabled;
    _enabled = 1;
}

INLN void Tadapt::Disable()
{
    if(0==_enabled)  return;
    old_enabled = _enabled;
    _enabled = 0;
    Reset();
}

INLN void Tadapt::Restore()
{
    _enabled = old_enabled;
}

INLN void Tadapt::Reset()
{
    glDisable(_glTarget);
    _glTarget = 0;
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    _hTex    = 0;    
}


#endif //__TEXADAPTER_H__


