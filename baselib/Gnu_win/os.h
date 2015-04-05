#ifndef __OSSS_H__
#define __OSSS_H__

#ifndef _MT
    #define _MT
#endif //

#include <windows.h>
#include <winsock.h>
#include <process.h>
#pragma comment(lib,"Wsock32.lib")
#include <direct.h>
#include <stdio.h>
#include <tchar.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//-----------------------------------------------------------------------------
#define TYPENAME    typename
#define INLN        inline
#define REG         register
#define NO_VT
#define VIRT
#define NAKED_
#define ASM_        asm
#define OUT_
#define OVERWRITE


//-----------------------------------------------------------------------------
INLN void    ge_sleep(DWORD milisecs){Sleep(milisecs);}
INLN void    ge_idle(){Sleep(8);}
INLN DWORD   ge_gettick(){return GetTickCount();}
INLN long    ge_error(){return GetLastError();}
INLN void    ge_error(long err){SetLastError(err);}

//-----------------------------------------------------------------------------
class mutex
{
    mutable CRITICAL_SECTION _cs;
public:
    mutex(){
        InitializeCriticalSection(&_cs);
    }
    virtual ~mutex(){
        DeleteCriticalSection(&_cs);
    }
    const int Lock() const{
        EnterCriticalSection(&_cs);
        return 0;
    }
    int TryLock() const
    {
        #if(_WIN32_WINNT >= 0x0400)
            return (TryEnterCriticalSection(&_cs) ? 0 : 1);
        #else
            EnterCriticalSection(&_cs);
            return 0;
        #endif
    }
    int Unlock() const{ LeaveCriticalSection(&_cs); return 0; }
};

//-----------------------------------------------------------------------------
class semaphore
{
    HANDLE _sem;
public:
    semaphore( int init = 0 ){
        _sem = CreateSemaphore(0,init,0x7fffffff,0);
    }
    virtual ~semaphore(){ CloseHandle(_sem); }
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
class OsThread
{
public:
    typedef unsigned long THANDLE;
    OsThread(){
        _bstop   = 1;
        _hthread = 0;
    }

    ~OsThread(){
        Stop();
    }

    int     Start()
    {
        _mutex.Lock();
        _bstop   = 0;
        _hthread = _beginthread(SFoo, 8912, (void*)this);
        _mutex.Unlock();
        ge_sleep(100);
        _start.Wait();
        return 0;
    }

    void IssueStop()
    {
        _mutex.Lock();
        _bstop = 1;
        _mutex.Unlock();
    }
    void    Stop()
    {
        _mutex.Lock();
        _bstop = 1;
        _mutex.Unlock();
        ge_sleep(100);
        Join();
    }

    BOOL  BStop()
    {
        return _bstop;
    }

    long  IsStopped(DWORD dws)
    {
        ge_sleep(dws);
        BOOL bret ;
        _mutex.Lock();
        bret = _bstop;
        _mutex.Unlock();
        return bret;
    }
    int Join(){
        return WaitForSingleObject((void*)_hthread,INFINITE);
    }

    int Kill(){
       int rv = TerminateThread((void*)_hthread, 0);
       _hthread = 0;

       return rv;
    }
    int Detach(){}
    virtual void ThreadFoo(){};

private:
    mutex       _mutex;
    semaphore   _start;
    long        _bstop;
    THANDLE     _hthread;
    static void SFoo(void* pData){
        OsThread* pT = reinterpret_cast<OsThread*>(pData);
        pT->_start.Notify();
        pT->ThreadFoo();
    }
};

//-----------------------------------------------------------------------------
class AutoLock
{
public:
    AutoLock(mutex* mutex):_mutex(mutex){if(_mutex)_mutex->Lock();}
    ~AutoLock(){if(_mutex)_mutex->Unlock();}
private:
    mutex* _mutex;
};

#define _PLATFORM_ "WINDOWS GNU"
#endif // !__OS_H__
