#ifndef __OS_H__
#define __OS_H__

#ifndef _MT
    #define _MT
#endif //

#include <winsock2.h>
#include <ws2tcpip.h>
#define   SOCKET_ERROR   -1
#define   INVALID_SOCKET -1
//typedef int socklen_t;
#include <stdarg.h>
#include <errno.h>
#include <resolv.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <asm/ioctls.h>
#include <linux/string.h>
#include <string.h>
#include <netdb.h>
//#include <tcpd.h>
#include <pthread.h>
#include <semaphore.h>

//-----------------------------------------------------------------------------
#define INFINITE     (0xEFFFFFFF);
typedef unsigned short   WORD;
typedef uint32_t         DWORD;
typedef int32_t          LONG;

typedef unsigned int     UINT;
typedef char             TCHAR;
typedef int              BOOL;
typedef unsigned char    BYTE;
#define TRUE             1
#define FALSE            0

#define  LOWORD(_x)      (_x&0xFFFF)
#define  HIWORD(_x)      ((_x&0xFFFF0000)>>16)
#define  MAKELONG(a,b)   (a|(b<<16))
#define  ASSERT          assert
#define  NO_VT
#define  afx_msg
#define  INLN            inline

//-----------------------------------------------------------------------------
void    ge_sleep(DWORD milisecs){usleep(milisecs * 1000);};
void    ge_idle(){usleep(8000);};
DWORD   ge_gettick(){
        static bool sgiInit = false;
        static int         sg_secsOffset;
        timeval     t;

        if (sgiInit == false)
        {
            sgiInit = true;
            ::gettimeofday(&t, NULL);
            sg_secsOffset = t.tv_sec;
        }
        ::gettimeofday(&t, NULL);
        DWORD secs  = t.tv_sec - sg_secsOffset;
        DWORD uSecs = t.tv_usec;
        return (secs * 1000) + (uSecs / 1000);
      }
long    ge_error(){return errno;};
void    ge_error(long err){errno=err;};

//-----------------------------------------------------------------------------
class mutex
{
    mutable pthread_mutex_t _mut;
public:
    mutex()
    {
        pthread_mutexattr_t     attr;

        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mut,&attr);
        pthread_mutexattr_destroy(&attr);
    }

    virtual ~mutex(){
        pthread_mutex_unlock(&_mut);
        pthread_mutex_destroy(&_mut); }

    int Lock() const{
        return pthread_mutex_lock(&_mut);
    }

    int TryLock() const {
        return pthread_mutex_trylock(&_mut);
    }

    int Unlock() const{
        return pthread_mutex_unlock(&_mut);
    }
};

//-----------------------------------------------------------------------------
class semaphore
{
    sem_t _sem;

public:
    semaphore( int init = 0 ){
        sem_init(&_sem,0,init); }

    virtual ~semaphore(){
        sem_destroy(&_sem);
    }

    int Wait(int to=-1) const {
        sem_wait((sem_t *)&_sem);
        return 1;
    }

    int TryWait(int to=-1) const{
        return (sem_trywait((sem_t *)&_sem)?errno:0);
    }

    int Notify() const{
        return (sem_post((sem_t *)&_sem) ? errno : 0);
    }

    int Value() const {
        int val = -1;
        sem_getvalue((sem_t *)&_sem,&val);
        return val;
    }
    void Reset( int init = 0 ){
        sem_destroy(&_sem);
        sem_init(&_sem,0,init);
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
        long stp=0;
        _mutex.Lock();
        stp = _bstop;
        _mutex.Unlock();
        if(!stp)
        {
            Stop();
            pthread_detach(_hthread);
            _hthread = 0;
        }
    }

    int     Start()
    {
        _mutex.Lock();
        _bstop   = 0;
        _mutex.Unlock();

        pthread_attr_t  attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        if (pthread_create(&_hthread, &attr, SFoo, this) != 0)
        {
            pthread_attr_destroy(&attr);
            return -1;
        }
        pthread_attr_destroy(&attr);
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
        return pthread_join(_hthread,0);
    }

    int Kill(){return 0;}
    int Detach(){return 0;}
    virtual void ThreadFoo(){};

private:
    mutex       _mutex;
    semaphore   _start;
    long        _bstop;
    pthread_t   _hthread;
    long        _stopped;
    static void* SFoo(void* pData){
        OsThread* pT = reinterpret_cast<OsThread*>(pData);
        pT->_stopped = 0;
        pT->_start.Notify();
        pT->ThreadFoo();
        pT->_stopped = 1;
        return 0;
    }
};


#endif // !__OS_H__

