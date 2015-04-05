// SkyDom.h: interface for the SkyDom class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SKYDOM_H__2EBBE3CC_8BC6_4764_8FEC_C6E48723B361__INCLUDED_)
#define AFX_SKYDOM_H__2EBBE3CC_8BC6_4764_8FEC_C6E48723B361__INCLUDED_

#include    "brush.h"

class SkyDom  
{
public:
    enum { SKY_CUBE, SKY_CONE, SKY_CYL, SKY_SPHERE};
    enum{P_SKY,P_CLOUD, P_LAND, P_LAVAL, P_EAST, P_WEST, P_SOUTH, P_NORTH, P_SIDE};
	SkyDom();
	virtual ~SkyDom();
    void    Clear(){_brush.Clear();}
    void    Make(int shape);
    Brush&  GetBrush(){return _brush;}

private:

    DWORD       _shape;          // see enum section
    DWORD       _flags;
    V3          _rotSpeed;
    UV          _cloudSpeed;
    UV          _lavaSpeed;
    V3          _boxExt;     
    V3          _boxInt;     
    Brush       _brush;
};

#endif // !defined(AFX_SKYDOM_H__2EBBE3CC_8BC6_4764_8FEC_C6E48723B361__INCLUDED_)
