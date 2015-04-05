#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__

#include "../baselib/baselib.h"
#include "../baselib/baseutils.h"
#include "../baselib/basecont.h"
#include "_irender.h"
#include "_isound.h"
#include "_iinput.h"
using namespace baseg;

//---------------------------------------------------------------------------------------
#ifndef WM_USER
    #define WM_USER 32768
#endif //

//---------------------------------------------------------------------------------------
// _flags
#define SM_NOMAPPING    1       // systemmode no map keys

//---------------------------------------------------------------------------------------
#define SYS_0               (40000)  // for windows
//---------------------------------------------------------------------------------------
                            // msg    wparam, lparam
#define HANDLE_SCRIPT       0 
#define SYS_CONFIGURE       (SYS_0)   // StartStruct, RndStruct
#define SYS_START           (SYS_0+1) // render struct, system data
#define SYS_TOGLESCRREN     (SYS_0+2) //screen full/windowed, sysdata  
#define SYS_ESCAPE          (SYS_0+3) //0, sysdata  
#define SYS_EXIT            (SYS_0+4) //0, sysdata
#define SYS_CLOSEAPP        (SYS_0+5) //0, sysdata
#define SYS_INPUT           (SYS_0+6)
#define SYS_UICOMMAND       (SYS_0+7) //cmd, 0
#define SYS_UIMOUSEENTER    (SYS_0+8) //cms, 1 enter 0 exit
#define SYS_RENDER_2D       (SYS_0+9) //0, paint prim plan
#define SYS_RENDER_3D       (SYS_0+10) //IRender, (long)&_sysData
#define SYS_ANIMATE         (SYS_0+11) //IRender, (long)&_sysData
#define SYS_SETERROR        (SYS_0+12) //0, sysdata


//---------------------------------------------------------------------------------------
#define SYS_BSP_READ        (SYS_0+100)
#define SYS_LOADMAP         (SYS_0+101)
#define SYS_BSPITEM_CAT     (SYS_0+102)
#define SYS_BSPITEM         (SYS_0+103)

//---------------------------------------------------------------------------------------
#define SYS_STATUSTEXT      (SYS_0+200)

//---------------------------------------------------------------------------------------
#define SYS_GAMESRVLIST      (SYS_0+300) // game server found
#define SYS_NET_REC          (SYS_0+301)
#define SYS_NET_OPENED       (SYS_0+302) 
#define SYS_NET_CLOSED       (SYS_0+302) 
#define SYS_NET_SETID        (SYS_0+303) //1 , id
#define SYS_NET_PLAYNOW      (SYS_0+304) //0 , 0
#define SYS_NET_PLAYERON     (SYS_0+305) //0 , buff
#define SYS_NET_PLAYEROFF    (SYS_0+305) //0 , buff
#define SYS_NET_POSITION     (SYS_0+306) //id, pos
#define SYS_NET_ACTION       (SYS_0+307) //id, pos
#define SYS_NET_MAPCHANGE    (SYS_0+308)   
#define SYS_NET_LOADMAP      (SYS_0+309) 
#define SYS_NET_PING         (SYS_0+310) //id, pos
#define SYS_NET_AUTH         (SYS_0+311) //id, pos
#define SYS_PROGRESS         (SYS_0+312) //id, pos
#define SYS_PROGRESSPAINT    (SYS_0+313) //0, sysdata

//return codes
//---------------------------------------------------------------------------------------
#define OS_NOMSG            0
#define OS_TOGGLESCR        0x1
#define OS_ESCAPE           0x2
#define OS_EXITAPP          0x4
#define OS_MSG              0x8

//---------------------------------------------------------------------------------------
#define RET_NOT_PROC        0x0
#define NO_IN_SPIN          0x1
#define NO_NET_SPIN         0x2
#define NO_CAM_SPIN         0x4
#define NO_RENDER           0x8
#define NO_PAINT            0x10
#define NO_SQ_CALL          0x20


//---------------------------------------------------------------------------------------
class NO_VT  ISystem;
typedef long  (*NotifyProc)(ISystem*, long, long, long);
typedef void  (*pdestruct)(void* , int size);
//---------------------------------------------------------------------------------------

struct SystemData 
{
    REAL         _fps;          
    REAL         _curtime;
    REAL         _ticktime;     
    REAL         _pauseTime;    
    DWORD        _physFrame;   
    DWORD        _drawFrame;   
    BYTE         *_pKeys;
    int          *_pMouse;
    ISystem*     _pSystem;
    RndStruct*   _pRndStruct;
    int          Key(int index){return *(_pKeys+index);}
    BYTE         MouseBut(int index){return *(_pMouse+index);}
};

//---------------------------------------------------------------------------------------

struct StartStruct
{
    StartStruct(){  
                    memset(this,0,sizeof(*this));
                    _size = sizeof(*this);
                    strcpy(_renderSel,"rendergl.dll");
                    strcpy(_inputSel, "inputdx.dll");    
                    strcpy(_audioSel,"sounddx.dll");
                    strcpy(_fontFile,"fontfile.bmp");
                    _enableScript = 1;
        }
    int           _size;                 // size of this structure
    TCHAR       _renderSel[64];        
    TCHAR       _inputSel [64]; 
    TCHAR       _audioSel [64];
    TCHAR       _fontFile [64];
    BOOL          _enableScript;
};

//---------------------------------------------------------------------------------------
#define MSG_REMOVE      0
#define MSG_NOREMOVE    1
typedef struct _SysMsg
{
    long    msg;
    long    pw;
    long    pl;
    long    time;
}SysMsg;

//---------------------------------------------------------------------------------------
typedef enum _sysMODES
{
    PLAY_MODE,
    SET_MODE,
    DOWNLOADING_MODE,
}sysMODES;


//PSystem->_hudMode = 0;----------------------------------------------------------------
#define HUD_ARROW   0x1
#define HUD_HELP    0x2
#define HUD_TRACE   0x4
#define HUD_CONSOLE 0x8


typedef struct _EKEY
{
    int     _et;   
    int*    _pidx;
}EKEY;

//---------------------------------------------------------------------------------------
class    ISystem;
__declspec(thread) extern   ISystem* PISystem;
class    IGc;              //gabrbage collected.
class    ISystem
{
public:
    
    ISystem(){PISystem=this;}
    virtual ~ISystem(){PISystem=0;}
    
    virtual int    Run(void)=0;
    virtual int    Stop(void) = 0;
    
    virtual void   RegisterDestructor(pdestruct, void*)=0;

// access plugs functions
    virtual Irender*  Render()=0;
    virtual Isound*   Sound() = 0;
    virtual Iinput*   Input() = 0;
    virtual const     UINT*  Primitives() = 0;

    virtual void      SetMode(long ss)=0;
    virtual long      GetMode()=0;
    virtual void      TextOut(long id, const TCHAR*,...)=0;
    virtual void      SetHudMode(unsigned long ss)=0;
    virtual unsigned long   GetHudMode()=0;

    virtual void      Printf(const TCHAR*,...)=0;
    
    virtual long      Default(long, long, long )=0;
    virtual long      SendMessage(long, long, long )=0;
    virtual long      PostProc(long, long, long, long delay=0)=0;

    virtual SysMsg*   GetMessage(long )=0;
    virtual int       PumpMessage()=0;
    virtual void      Spin(REAL&)=0;
    virtual int       UpdateWindow(BOOL camtoo=0);
    virtual void      Wait(long percent)=0;

// texture functions
    virtual Htex* GenTexGetBuff(const TCHAR* , DWORD, char**, int*, int*, int*)=0;
    virtual Htex* GenTex(const TCHAR* name, int x, int y, int bpp, const char* buff, DWORD mip);
    virtual Htex* GenTexFile(const TCHAR* , DWORD)=0;
    virtual Htex* GenTexFont(REAL x, REAL y, REAL scale, const TCHAR* fname) = 0;
    virtual void  DeleteTexFont(Htex*, BOOL allrefs=0)=0;
    virtual void  DeleteTex(Htex*, BOOL allrefs=0)=0;
    virtual void  DeleteTexts(Htex*,int, BOOL allrefs=0)=0;

//sound functions
    virtual Itape* AddSoundFile(const TCHAR* pFile)=0;
	virtual void   RemoveSoundFile(const TCHAR* pFile)=0;
	virtual void   RemoveSound(Itape* pt)=0;
	virtual Itape* GetTape(const TCHAR* fname)=0;
    virtual void   PlayPrimary(const Itape* pt, DWORD mode=1)=0;
   	virtual void   StopPlay(const Itape* pt )=0;
    virtual Itape* PlayPrimaryFile(const TCHAR* fname, DWORD mode=1)=0;
	virtual void   StopPlayFile(const TCHAR* fname)=0;

//input functions
    virtual void    SetSpeeds(REAL ts, REAL rs)=0;
	virtual BOOL    IsKeyDown(long key)=0;
    virtual int *   Moves()=0;
    virtual BYTE*   Keys()=0;
    virtual BYTE    GetPressedKey()=0;
    virtual void    IgnoreInput(REAL secs)=0;


//
    virtual HINSTANCE   Hinstance()=0;
    virtual HWND        Hwnd()=0;

//
    virtual void        RegisterGc(IGc* pigc)=0;
    virtual void        UnregisterGc(IGc* pigc)=0;

};

//---------------------------------------------------------------------------------------
class ICamera : public Pos
{
public:
    ICamera (){TRACEX("ICamera()\r\n");}
    virtual ~ICamera (){TRACEX("~ICamera()\r\n");}
    virtual int         Animate(SystemData* ps=0)=0;
    virtual BOOL		CanSee(const Box& aabbMin, int i=-1)const=0;
    virtual BOOL		CanSeeCamera(const V3& center,  REAL rad)=0;
    virtual REAL        Fov()=0;
    virtual void        SetPos(const V3& eye, const V3& euler)=0;
    virtual void        SetPosMat(const M4& vmatrix)=0;
    virtual void        LookAt(const V3& eye,const  V3& up,const  V3& fwd, const V3& rght)=0;
    virtual void        SetFarAndFov(REAL rfar, REAL rfov)=0;
    virtual const M4&	ViewMatrix()=0;
    virtual void        SetUserData(void* ud)=0;
    virtual void*       GetUserData()=0;
};


//---------------------------------------------------------------------------------------
class NO_VT IGc  //gabrbage collected.
{
public:
    IGc(){TRACEX("IGc()\r\n"); PISystem->RegisterGc(this);}
    virtual ~IGc(){TRACEX("~IGc()"); PISystem->UnregisterGc(this);}
    virtual void    Clear()=0;
};


//---------------------------------------------------------------------------------------
#include <windows.h>

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {          // size is 16
    DWORD Data1;
    WORD  Data2;
    WORD  Data3;
    BYTE  Data4[8];
} GUID;
#endif //GUID_DEFINED

//---------------------------------------------------------------------------------------
INLN GUID* EngineIID()
{
    static GUID ENGINE_IID = { 0x646741ef, 0x2cc5, 0x4b68, { 0x93, 0x3d, 0xcd, 0x36, 0x51,0xfd, 0x61, 0xea } };
    return &ENGINE_IID;
}

#endif //__IENGINE_H__


