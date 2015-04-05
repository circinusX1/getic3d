#if !defined(__ZONE_H__)
#define __ZONE_H__

#include "baselib.h"

class CAreaZone  
{
public:
	CAreaZone();
    virtual ~CAreaZone(){};
    void    Render(CView* pv, char view);
    void    Select(BOOL b){
        (_selected = b) ? _color = ZWHITE : _color = CLR(CLR_ZONE);
    }
private:

    int             _idxThis;
    vvector<Box>    _boxes;
    CAreaZone*      _zones[32]; //vis zones
    BOOL            _linedrawn;
    CLR             _color;
    BOOL            _selected;
};



#endif // 
