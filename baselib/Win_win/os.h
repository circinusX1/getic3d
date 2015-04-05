//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
//============================================================================
#ifndef __OSSS_H__
#define __OSSS_H__

#ifndef _MT
    #define _MT
#endif //


#pragma warning (disable: 4996)

#include <windows.h>
#include <winsock.h>
#include <process.h>
#include <direct.h>
#include <stdio.h>
#include <tchar.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>

#pragma comment(lib,"Wsock32.lib")

//-----------------------------------------------------------------------------
#define INLN    __forceinline
#define REG
#define NO_VT   __declspec(novtable)
#define VIRT
#define NAKED_  __declspec( naked )
#define ASM_    __asm
#define nvtclass    class __declspec(novtable)
#define OUT_
#define OVERWRITE
#define TYPENAME
#define OPBS  '/'
#define BS    '\\'
#define BSS   "\\"
//-----------------------------------------------------------------------------
INLN void    ge_sleep(DWORD milisecs){Sleep(milisecs);}
INLN void    ge_idle(){Sleep(8);}
INLN DWORD   ge_gettick(){return GetTickCount();}
INLN long    ge_error(){return GetLastError();}
INLN void    ge_error(long err){SetLastError(err);}

//-----------------------------------------------------------------------------
struct mutex
{
    mutable CRITICAL_SECTION _cs;
    mutex(){
        _inout = 0;
        InitializeCriticalSection(&_cs);
    }
    ~mutex(){
        if(_inout) LeaveCriticalSection(&_cs); 
        DeleteCriticalSection(&_cs);
    }
    int Lock() {
        if(_inout)
            return 1;
        EnterCriticalSection(&_cs);
        _inout=1;
        return 0;
    }
    int TryLock() 
    {
         if(_inout)
            return 1;
        #if(_WIN32_WINNT >= 0x0400)
            if(TryEnterCriticalSection(&_cs))
            {
                _inout=1;
                return 1;
            }
            return 0;
        #else
            EnterCriticalSection(&_cs);
            _inout=1;
            return 1;
        #endif
    }
    int Unlock() { 
        if(0==_inout)
            return 1;
        _inout=0;
        LeaveCriticalSection(&_cs); 
        return 1; 
    }
    long _inout;
};

//-----------------------------------------------------------------------------
struct semaphore
{
    HANDLE _sem;
    semaphore( int init = 0 ){
        _sem = CreateSemaphore(0,init,0x7fffffff,0);
    }
    ~semaphore(){ CloseHandle(_sem); }
    int Wait(int to = INFINITE) {
        return WaitForSingleObject((HANDLE)_sem,to)==WAIT_OBJECT_0;
    }
    int TryWait(int to = INFINITE) {
        return ((WaitForSingleObject((HANDLE)_sem,to)==WAIT_OBJECT_0) ? 1 : -1);
    }
    int  Notify() const{
        return (ReleaseSemaphore((HANDLE)_sem, 1, 0) ? 0 : -1);
    }

    int  Value() { long val = -1;
        ReleaseSemaphore((HANDLE)_sem,0,&val); return val;
    }
    void Reset( int init = 0 ){
        CloseHandle(_sem);
        _sem = CreateSemaphore(0,init,0x7fffffff,0);
    }
};


//-----------------------------------------------------------------------------
#pragma warning (disable:4312)
class OsThread
{
public:
    typedef unsigned long THANDLE;
    OsThread(){
        _bstop   = 1;
        _hthread = 0;
        _stopped = 1;
    }

    virtual ~OsThread(){
        if(!_stopped)
            Stop();
    }

    virtual int     Start()
    {
        _mutex.Lock();
        _bstop   = 0;
        _hthread = (THANDLE)_beginthread(SFoo, 16384, (void*)this);
        _mutex.Unlock();
        ge_sleep(100);
        _start.Wait();
        _stopped = 0;
        return 0;
    }

    virtual void IssueStop()
    {
        _mutex.Lock();
        _bstop = 1;
        _mutex.Unlock();
    }
    virtual void    Stop()
    {
        _mutex.Lock();
        _bstop = 1;
        _mutex.Unlock();
        ge_sleep(100);
        if(StillActive())
            Join(5000);
        ge_sleep(200);
        if(StillActive())
            Kill();
        _stopped = 1;
    }

    virtual DWORD StillActive()
    {
        DWORD ex = 0;
        if(_hthread)
            GetExitCodeThread((HANDLE)_hthread,&ex);
        return (ex==STILL_ACTIVE);
    }

    

    BOOL  BStop()
    {
        return _bstop;
    }

    long  IsStopped(DWORD dws)
    {
        return _stopped;
    }
    int Join(int to = INFINITE){
        return WaitForSingleObject((HANDLE)_hthread,to);
    }

    int Kill(){
       int rv = TerminateThread((HANDLE)_hthread, 0);
       _hthread = 0;
       _stopped = 1;
       return rv;
    }
    int Detach(){}
    virtual void ThreadFoo(){};

protected:
    mutex       _mutex;
    semaphore   _start;
    long        _bstop;
    long        _stopped;
    THANDLE     _hthread;
    static void SFoo(void* pData){
        OsThread* pT = reinterpret_cast<OsThread*>(pData);
        pT->_start.Notify();
        pT->ThreadFoo();
    }
};

//-----------------------------------------------------------------------------
struct AutoLock
{
    AutoLock(mutex* mutex):_mutex(mutex){if(_mutex)_mutex->Lock();}
    ~AutoLock(){if(_mutex)_mutex->Unlock();}

    mutex* _mutex;
};

#define _PLATFORM_ "WINDOWS MS"

#endif // !__OS_H__

