// SplinePath.h: interface for the SplinePath class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SPLINE_PATH__
#define __SPLINE_PATH__

#include "basecont.h"
class SplinePath  
{
public:
	SplinePath();
    SplinePath(const SplinePath& sp){
        _points = sp._points;
    }
    void    AddPoint(V3& p){_points.push_back(p);}
    void    SetPoint(int idx, V3& p){ASSERT(idx<_points.size()); _points[idx]=p;}
    V3&     GetPoint(int idx){ASSERT(idx<_points.size()); return _points[idx];}
    void    Clear(){_points.clear();}
    int     Size(){return _points.size();}
    SplinePath& operator=(const SplinePath& sp){
        if(&sp != this)
            _points = sp._points;
        return* this;
    }
    
private:
    vvector<V3> _points;
};

#endif // !__SPLINE_PATH__
