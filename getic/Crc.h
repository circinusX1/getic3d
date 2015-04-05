#ifndef __CCRC_H__
#define __CCRC_H__

#include "baselib.h"

class CCrc  
{
public:
    CCrc(DWORD poly =  0x04c11db7L);
	virtual ~CCrc();
    DWORD update_crc(DWORD crc_accum, BYTE *data_blk_ptr, int data_blk_size);

    DWORD           polynomial;

    unsigned long   crc_table[256];

};

#endif // 
