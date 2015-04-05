// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************

#include "stdafx.h"
#include "PQmapLoader.h"

//----------------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{
    return TRUE;
}

//----------------------------------------------------------------------------------
extern "C"
{
    EX_IN IGeticPlug*	CreateInstance()
    {
	    return new PqmapLoader(); 
    }

    EX_IN  void	DestroyInstance(IGeticPlug* pr)
    {
	    delete pr;
    }

    const char* GetCatName(void)
    {
        return "getic plugin, map";
    }

};

