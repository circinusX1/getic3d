// Zone.cpp: implementation of the CZone class.

#include "stdafx.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "Zone.h"


CAreaZone::CAreaZone():_idxThis(-1)
{
    ::memset(&_zones, 0, sizeof(_zones));
    _linedrawn = 0;
    _selected  = 0;
}

void CAreaZone::Render(CView* pv, char view)
{
    FOREACH(vvector<Box>, _boxes, pb)   
    {
        DOC()->GLDrawBox(*pb, 1, (BYTE*)_color);
    }
}



