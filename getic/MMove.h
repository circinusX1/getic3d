// MMove.h: interface for the MMove class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MOVE_H__
#define __MOVE_H__

enum {M_MOVED=1, M_SCALED, M_ROTATED, M_SHIFTED, M_DELETED, ITM_MOVED, ITM_SCALED, ITM_ROTATED, ITM_DELETED, F_EXTRUDED, OBJ_CHANGED};

class MMove  
{
public:
    MMove():_ldown(0),_rdown(0),_mdown(0){};
    virtual ~MMove(){};

    float   _prev[3];
    float   _cur[3];
    CPoint  _pt;
	CPoint  _ptd;
    BOOL    _ldown;
    BOOL    _rdown;
    BOOL    _mdown;
    V3      _wpdown;
};

#endif // __MOVE_H__
