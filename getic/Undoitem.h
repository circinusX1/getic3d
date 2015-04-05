//
//////////////////////////////////////////////////////////////////////

#ifndef _SS_CARR_H__
#define _SS_CARR_H__

#include "basecont.h"



class UndoItem
{
public:
    UndoItem(){_pThis=this;}
    virtual ~UndoItem(){_pThis=0;};
    UndoItem* _pThis;
};


#endif //