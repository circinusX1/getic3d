#ifndef _ZT___
#define _ZT___

#pragma warning (disable: 4786)
#include "baselib.h"


struct FPOINT
{
    REAL x;
    REAL y;

};

struct FRECT
{
    REAL left;
    REAL top;
    REAL right;
    REAL bottom;
};


#define C_NA    0 
#define C_FULL  0x1 
#define C_PORT  0x2 
#define C_LEAFS 0x4 
#define C_LBYL  0x8 
#define C_LMAP  0x10
#define C_GRAPH 0x20 
#define C_ZONES 0x40
#define C_WHITE 0x80
#define C_BYFAR 0x100


typedef enum _BR_MODE{BR_VIEW, BR_MOVE, BR_SCALE, BR_ROTATE, BR_RISE}BR_MODE;
typedef enum _EL_SEL{SEL_NONE, SEL_BRUSH, SEL_FACE, SEL_VERTEX, SEL_EDGE, SEL_ITEMS, SEL_BR_ITEM}EL_SEL;


enum
{
    CUR_NONE=0,
    CUR_SCROLL,
    CUR_MOVE,
    CUR_SCALE,
    CUR_ROTATE,

    CUR_NS,
    CUR_EW,
    CUR_NWSE,
    CUR_NESW,
};


#endif