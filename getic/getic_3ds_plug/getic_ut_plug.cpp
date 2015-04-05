// getic_ms3d_plug.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "P3dsLoader.h"



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
	    return new P3dsLoader(); 
    }

    EX_IN  void	DestroyInstance(IGeticPlug* pr)
    {
	    delete pr;
    }

    const char* GetCatName(void)
    {
        return "getic plugin, 3ds";
    }

};

