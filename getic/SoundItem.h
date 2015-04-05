// LightBulb.h: interface for the CSoundItem class.
//
#ifndef __SOUND_ITEM_H__
#define __SOUND_ITEM_H__

#include "SceItem.h"

class CSoundItem  : public SceItem
{
public:
	CSoundItem();
    CSoundItem(const CSoundItem& r);
    CSoundItem& operator=(const CSoundItem& r);
	virtual ~CSoundItem();
    Htex& GetTexture();
    OVERWRITE void Serialize(FileWrap* pfv,const vvector<Texture>& texNames, map<int,int>& texassoc);
    virtual void GlDraw(z_ed3View* ax){
        SceItem::GlDraw(ax);
    }
    void    Play();
    virtual void    GlDraw2D(CZ_ed2View* ax);
    virtual Box    GetWBox(){
        Box box = GetBox();
        box._min+=_t;
        box._max+=_t;
        return box;
    }
    virtual Box    GetBox(){
        Box box(V3(-_radius*.707,-_radius*.707,-_radius*.707),
                 V3(_radius*.707,_radius*.707,_radius*.707));
        return box;
    }
    virtual void    Scale(V3& s, V3 t, REAL sc=1.0){
        REAL length = s.len();
        if(s.x<0||s.y<0||s.z<0)
            length=-length;
        _radius += length;
        if(_radius < 2)
            _radius=2;
    }
    virtual int  Size(){return sizeof(*this);}

    REAL    _specAngle;
    REAL    _volume;
    REAL    _radius;
    string  _soundfile;
};

#endif // !defined(AFX_LIGHTBULB_H__AB671A26_16BA_4D98_96A6_2A5E1631C930__INCLUDED_)
