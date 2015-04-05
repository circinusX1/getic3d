
#ifndef __SCEITEM_H__
#define __SCEITEM_H__

#include "stdafx.h"     // for GBackface
#include <gl/gl.h>			
#include <gl/glu.h>			
#include <gl/glaux.h>	
     
#include "..\\_include\\BspFIleStr.h"
#include "baseutils.h"
#include "TexRef.h"
#include "TexAdapter.h"
#include "BaseEntity.h"

#define OVERWRITE

//-----------------------------------------------------------------------------
// base item class for editor
class CSoundItem;
class CZ_ed2View;
class z_ed3View;
struct Texture;

struct ItemSig
{
	char  text[8];
};

extern ItemSig IS[];

class SceItem : public TexCarrier, public BaseEntity, public CanProps
{
public: 
    SceItem();
    SceItem(const SceItem& r);
    virtual ~SceItem();
    SceItem& operator=(const SceItem& r);
    static  SceItem* Create(int type);
    void    DeSelect(){_selected=FALSE;}
    void    Select(){
        _selected=TRUE;
    }
    BOOL    IsSelected(){
        return _selected;
    }
    virtual void Serialize(FileWrap* pfv, const vvector<Texture>& texNames, map<int,int>& texassoc);

	virtual void InitCommit(){};
    virtual void Commit(){};
    virtual void GlDraw(z_ed3View* pV);
    virtual void GlDrawSel(z_ed3View* pV);
	BOOL	IsDirty(){return _flags & BRSH_DIRTY;};
	void	Dirty(BOOL b){
        if(b) 
            _flags|=BRSH_DIRTY; 
        else 
            _flags&=~BRSH_DIRTY;
    };
    virtual void    SetPos(V3& pos){
        _t=pos;
    }
    virtual Htex& GetTexture();
    virtual void    GlDraw2D(CZ_ed2View* ax);
    virtual void    Move(V3& t){
        _tl = _t;
        _t += t;
    }
    virtual void    Scale(V3& s, V3 t, REAL sc=1.0){
        _s += s;
        _sl = _s;
    }
    virtual void    Rotate(V3& r, BOOL bao=FALSE);
    virtual void UndoLastTr(){
    }
    virtual int     Size(){return sizeof(*this);}
    virtual Box    GetBox(){
        Box box(V3(-32,-32,-32),V3(32,32,32));
        return box;
    }
    virtual Box    GetWBox(){
            Box box(V3(-32,-32,-32),V3(32,32,32));
            box._min += _t;
            box._max += _t;
           // box._min *= _s;
           // box._max *= _s;
            return box;
    };

public:
    EITEM           _item;
    char            _catname[16];
    int             _id;
    DWORD           _props;
    DWORD           _flags;
	BOOL			_selected;
    CLR             _colorD;
    DWORD           _zonearea;
    V3              _t;
    V3              _s;
    V3              _euler;
    M4              _m;

    V3              _tl;
    V3              _sl;
    V3              _dirl;	// last direction

    long            _group;
	long			_leafIdx;

    static  Vtx     _Vtci[4];
    static  BOOL    _BInitalized;
    static  long    _GItemID;
};



#endif //