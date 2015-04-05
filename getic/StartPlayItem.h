// LightBulb.h: interface for the CStartPlayItem class.
//
#ifndef __STARTPLAY_ITEM_H__
#define __STARTPLAY_ITEM_H__

#include "SceItem.h"

class CStartPlayItem  : public SceItem
{
public:
	CStartPlayItem();
    CStartPlayItem(const CStartPlayItem& r);
    CStartPlayItem& operator=(const CStartPlayItem& r);
	virtual ~CStartPlayItem();
    Htex& GetTexture();
    OVERWRITE void Serialize(FileWrap* pfv,const vvector<Texture>& texNames, map<int,int>& texassoc);
    virtual void GlDraw(z_ed3View* ax){
        SceItem::GlDraw(ax);
    }
    virtual void    GlDraw2D(CZ_ed2View* ax);
    virtual Box    GetWBox(){
        Box box = GetBox();
        box._min+=_t;
        box._max+=_t;
        return box;
    }
    virtual int  Size(){return sizeof(*this);}

    string  _team;
};

#endif // !defined(AFX_LIGHTBULB_H__AB671A26_16BA_4D98_96A6_2A5E1631C930__INCLUDED_)
