#ifndef __CCRC_H__
#define __CCRC_H__

#include "baselib.h"

class CCrc  
{
public:
    CCrc(size_t poly =  0x04c11db7L);
	virtual ~CCrc();
    size_t update_crc(size_t crc_accum, BYTE *data_blk_ptr, int data_blk_size);

    size_t           polynomial;

    unsigned long   crc_table[256];

};

#endif // 
