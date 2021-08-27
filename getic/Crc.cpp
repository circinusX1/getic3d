//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------
#include "stdafx.h"
#include "z-edmap.h"
#include "Crc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//--| poly|------------------------------------------------------------------------
CCrc::CCrc(size_t poly):polynomial(poly)
{
    int i, j;
    size_t crc_accum;
    for ( i = 0;  i < 256;  i++ )
    {
        crc_accum = ( (size_t) i << 24 );
        for ( j = 0;  j < 8;  j++ )
        {
            if ( crc_accum & 0x80000000 )
               crc_accum =( crc_accum << 1 ) ^ polynomial;
            else
                crc_accum = ( crc_accum << 1 );
        }
        crc_table[i] = crc_accum;
    }
}

//--|CCrc::~CCrc|------------------------------------------------------------------------
CCrc::~CCrc()
{
    
}


//--| CCrc::update_crc|-------------------------------------------------------------------
size_t CCrc::update_crc(size_t crc_accum, BYTE *data_blk_ptr, int data_blk_size)
{
    int i, j;
    for ( j = 0;  j < data_blk_size;  j++ )
    {
        i = ( (int) ( crc_accum >> 24) ^ *data_blk_ptr++ ) & 0xff;
        crc_accum = ( crc_accum << 8 ) ^ crc_table[i];
    }
    return crc_accum;
}
