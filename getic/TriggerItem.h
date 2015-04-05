// TriggerItem.h: interface for the TriggerItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TRIGGERITEM_H__
#define __TRIGGERITEM_H__

#include "SceItem.h"
#include "Brush.h"

class TriggerItem   : public SceItem
{
public:
	TriggerItem();
	virtual ~TriggerItem();
    TriggerItem(const TriggerItem& r);
    TriggerItem& operator=(const TriggerItem& r);

    OVERWRITE void    Serialize(FileWrap* pfv, const vvector<Texture>& texNames, map<int,int>& texassoc);
    OVERWRITE void    GlDraw2D(CZ_ed2View* ax);
    OVERWRITE void    GlDraw(z_ed3View* pV);
    OVERWRITE void    GlDrawSel(z_ed3View* pV);
    OVERWRITE int     Size(){return sizeof(*this);}
    OVERWRITE void    Move(V3& t);
    OVERWRITE void    Scale(V3& s, V3 t, REAL sc=1.0);
    OVERWRITE Box    GetBox(){
        if(_bSelEfect){
            Box befBox(V3(-32.0,-32.0,-32.0),V3(32.0,32.0,32.0));
            befBox._min+=_efectPoint;
            befBox._max+=_efectPoint;
            return befBox;
        }
        return _drawBrush._box; 
    }
    OVERWRITE Box    GetWBox(){
        _bSelEfect  = AKEY(VK_CONTROL);
        return GetBox();
    }
    void    SelectEffect(BOOL bs){_bSelEfect = bs;}
    BOOL    IsEfectSelected(){
        _bSelEfect  = AKEY(VK_CONTROL);
        return _bSelEfect ;
    }
    void    SetEfectBrush(Brush* p,BOOL bCenter=FALSE){
        //detach old brush
        if(_pBrush)
        {
			_pBrush->_brushflags &= ~BRSH_HASTRIG;
            _pBrush = 0;
        }
        if(p)
		{
            _pBrush = p;
            _pBrush->_brushflags |= BRSH_HASTRIG;
            if(bCenter)
            {
                _efectPoint   = _pBrush->_box.GetCenter();
            }
        }
		else 
		{
            if(bCenter)
			    _efectPoint   = _t + VY*64.0;
		}
    }

    virtual void Commit(){
        if(_pBrush)
        {
            _efectPoint   = _pBrush->_box.GetCenter();
        }
    };
	virtual void InitCommit(){
		Commit();
	};
    virtual void    SetPos(V3& pos){
        _drawBrush.SetPos(pos);
        _drawBrush.Recalc();
        _t = _drawBrush._box.GetCenter(); // update the t 
       _efectPoint = _t + VY * 128.0;
    }

    Brush*  GetEfectBrush(){return _pBrush;}   
    V3&     EfectPoint(){return _efectPoint;}

private:
    void    FindTargetBrush();

public:
// trigger data
    Brush*          _pBrush;		// target brush
    V3              _efectPoint;    // efect point world position    

//saving in BSP
	int             _brIndex;

//edittor spec drawinf moving data
    Brush           _drawBrush;     // just to draw the trigger bounding box
	BOOL            _bSelEfect;     // efect point is selected
};

#endif // !__TRIGGERITEM_H__
