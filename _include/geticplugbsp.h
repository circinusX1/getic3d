// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************
#ifndef _GETIC_PLUGBSP_H__
#define _GETIC_PLUGBSP_H__

#include <windows.h>
#include <tchar.h>
#include "bspfilestr.h"

typedef enum _SEEK_POS{
    FSEEK_CUR ,
    FSEEK_END ,
    FSEEK_SET,
}SEEK_POS;

// File hook for intercepring the BSP saving data. This allows you to save the data
// in your own format.

class IBSPFileHook
{
public:
    IBSPFileHook(){}
    virtual ~IBSPFileHook(){}


    virtual long   _stdcall read( void *buffer, size_t size, size_t count)=0;
    virtual size_t _stdcall write( const void *buffer, size_t size, size_t count)=0;
    virtual char*  _stdcall gets( char *string, int n)=0;
    virtual int    _stdcall seek (long offset, SEEK_POS origin )=0;
    virtual long   _stdcall tell ()=0;
    virtual int    _stdcall close()=0;
    virtual long   _stdcall wopen(const wchar_t *filename, const wchar_t *mode)=0;
    virtual long   _stdcall aopen(const char *filename, const char *mode)=0;
    virtual int    _stdcall eof( )=0;
    virtual void   _stdcall start_section(const char* sectionName)=0;

};

#endif 