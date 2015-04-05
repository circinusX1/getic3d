// SkyDom.cpp: implementation of the SkyDom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "SkyDom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SkyDom::SkyDom():_shape(SKY_CUBE)
{
}

SkyDom::~SkyDom()
{
}


void    SkyDom::Make(int shape)
{
    Clear();
    _shape = shape;
    switch(_shape)
    {
        case SKY_CUBE:
            _brush.MakeCube(V3(100,100,100));
            break;
        case SKY_CONE:
            _brush.MakeCone(100,100,4,0);
            break;
        case SKY_CYL:
            _brush.MakeCylinder(100,100,8,0);
            break;
        case SKY_SPHERE:
            break;
    }
}

