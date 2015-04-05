// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************

#include "stdafx.h"
#include "Ms3DLoader.h"



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


extern "C"
{
    EX_IN IGeticPlug*	CreateInstance()
    {
	    return new Ms3DLoader(); 
    }

    EX_IN  void	DestroyInstance(IGeticPlug* pr)
    {
	    delete pr;
    }

    const char* GetCatName(void)
    {
        return "getic plugin, ms3d";
    }

};

