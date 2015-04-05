// GeoUtils.cpp: implementation of the GeoUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "GeoUtils.h"

V3* __GetCone(const V3& center, REAL h, REAL r, int& nverts )
{
    static int  snverts = 0;
    static V3   cirpct[32];

    if(snverts)
    {
        nverts = snverts;
        return cirpct;
    }

    REAL step = 0;
    cirpct[0] = V0;
    int i;
    for( i = 0; i<12; i++)
    {
        cirpct[1+i].z = 0;
        cirpct[1+i].x = r * sinf(step);
        cirpct[1+i].y = r * cosf(step);
        step+=PIPE6;
    }
    cirpct[1+i] = cirpct[1];
    cirpct[0].z += h;
    nverts = 14;
    snverts = nverts;
    return cirpct;
}