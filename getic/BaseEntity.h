//
//////////////////////////////////////////////////////////////////////

#ifndef _SS_CARR_H__
#define _SS_CARR_H__

#include "basecont.h"

class BaseEntity
{
public:
    BaseEntity(){_pThis=this;}
    virtual ~BaseEntity(){_pThis=0;};
    BaseEntity* _pThis;
    char     _name[16];
};


class CanProps
{
public:
    
};

#endif //
