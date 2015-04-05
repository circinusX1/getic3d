// LightBulb.h: interface for the CLightBulb class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __LIGHT_BULB_H__
#define __LIGHT_BULB_H__

#include "SceItem.h"

class CLightBulb  : public SceItem
{
public:
	CLightBulb();
    CLightBulb(const CLightBulb& r);
    CLightBulb& operator=(const CLightBulb& r);
	virtual ~CLightBulb();
    OVERWRITE void Serialize(FileWrap* pfv, const vvector<Texture>& texNames, map<int,int>& texassoc);
    virtual void GlDraw(z_ed3View* ax){
        SceItem::GlDraw(ax);
    }
    virtual Htex& GetTexture();
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

    //      light map calc
    REAL    _specAngle;
    REAL    _lmIntensity;
    REAL    _radius;
    
    // hallo image
    REAL    _halloRadMin;
    REAL    _halloRadMax;

    REAL    _minDist;
    REAL    _maxDist;
	CLR		_colorE;	// emissive
	CLR		_colorS;	// specular
	int	    _atten[3];	// for the 3 colors
};

#endif // !defined(AFX_LIGHTBULB_H__AB671A26_16BA_4D98_96A6_2A5E1631C930__INCLUDED_)
