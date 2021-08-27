// geticgui.cpp : Defines the entry point for the DLL application.
//
#include <windows.h>

/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       size_t  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
*/
struct Sec
{
    char            sig[4];
    int             payed;
    unsigned long   crc;
    SYSTEMTIME      st;
    char            sige[16];
};
static Sec sec = {"@:$",0,0,{0},"@@@@@@@@@@@@@@@"};
