//-----------------------------------------------------------------------------
// GETIC SCRIPT GENERATED FILE.                                     Marius C.
// DO NOT CHANGE THIS FILE. IS BEEING CHECKED BY ITS CRC
//-----------------------------------------------------------------------------
#ifndef __GSCRIPT_HH__
#define __GSCRIPT_HH__

#include "..\\script\\headers\\engine.h"
//--------------------------------------------------------------------------------
// game messages sent to script
//------defined------------value-----wparam--------lparam--------eparam----------
typedef enum _GM_EVENTS{
    GM_NA   			=0,		
    GM_CREATE			,		
    GM_DESTROY			,	
    GM_TRIGGER			,	
    GM_CONNECT			,	
    GM_DISCONNECT		,	
    GM_CREATEITEM		,	
    GM_DESTORYITEM		,	
    GM_CREATEACTOR		,	
    GM_DESTROYTACTOR	,	
    GM_PICKITEM			,	
    GM_DROPITEM			,	
    GM_MODELPOS			,	
    GM_ATTACK			,	
    GM_ACTORFIRE		,	
    GM_ACTORDAMMAGE		,	
    GM_IMPACT			,				
    GM_ACTORPOS			,	
    GM_ONESCAPE			,	
    GM_MODECHANGED		,	
    GM_PLAYSOUND		,	
    GM_PLAYERJOIN		,	
    GM_PLAYERLEAVE		,	
    GM_ACTORMARK		,
    GM_DLLINIT          ,
    GM_DLLEXIT          ,
	GM_TEST,

}GM_EVENTS;

//--------------------------------------------------------------------------------
class Script;
typedef long (Script::*EVT_CB)(void*, void*);  //events callback
//--------------------------------------------------------------------------------

struct  MAP_TYP
{
    GM_EVENTS   ev;
    int         id;
    EVT_CB      pf;
};

extern MAP_TYP   _CmdMap[];  
//--------------------------------------------------------------------------------
class Script
{
public:

	long Dispatch(long ev, long wp, void* lp)
	{
		MAP_TYP*  pHead = _CmdMap;
		while(pHead->ev)
		{
			if(pHead->ev == ev)
			{
				if(pHead->id == wp)
				{
					return ((this)->*(pHead->pf))(lp, (void*)wp); 
				}
				return ((this)->*(pHead->pf))(lp, (void*)wp);
			}
			pHead++;
		}
		return 0;
	}
};
  

static Script* ps = 0;
//--------------------------------------------------------------------------------
extern "C"
{
	__declspec(dllexport) long Event(long ev, long wp, void* lp)
	{
		if(!ps)ps = new Script;
			return ps->Dispatch(ev, wp, lp);
		return -1;
	};
};

//-----------------------------------------------------------------------------
BOOL __stdcall DllMain (HANDLE hInst,   
                        unsigned long reason,    
                        LPVOID reserved)
{
	if(DLL_PROCESS_ATTACH == reason)
	{
		if(0==ps)
			ps = new Script;
	}
	else if(DLL_PROCESS_DETACH)
	{
		if(ps)
			delete ps;
		ps=0;
	}
    return TRUE;
};
// void (AFX_MSG_CALL CCmdTarget::*)(UINT)

//--------------------------------------------------------------------------------
#define BEGIN_EVENTS(cName_)   MAP_TYP _CmdMap[]={ 
#define ON_EVENT(ev_,foo_)	   {ev_, -1,  (EVT_CB)&foo_} ,
#define ON_ITEM(ev_,itm_,foo_) {ev_, itm_,(EVT_CB)&foo_} ,
#define END_EVENTS()           {GM_NA,-1,0}};


//-----------------------------------------------------------------------------
#endif ///_GSCRIPT_H__
