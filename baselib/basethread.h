/**
# Copyright (C) 2006-2014 Chincisan Octavian-Marius(mariuschincisan@gmail.com) - coinscode.com - N/A
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

#ifndef __OS_H__
#define __OS_H__

#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <inttypes.h>

#ifdef MA_UNICODE
#   include <wchar.h>
#   define _ttfopen      _wfopen
#   define _ttstrchr     wcschr
#   define _ttstrstr     wcsstr
#   define _ttsnprintf   wsnprintf
#   define _ttatoi       _wtoi
#   define _ttstrtok     wcstok
#   define _ttgetenv     _wgetenv
#   define _ttsystem     _wsystem
#   define _ttasctime    _wasctime
#   define _ttremove     _wremove
#   define _ttrename     _wrename
#else
#   define _ttfopen      fopen
#   define _ttstrchr     strchr
#   define _ttstrstr     strstr
#   define _ttsnprintf   snprintf
#   define _ttatoi       atoi
#   define _ttstrtok     strtok
#   define _ttgetenv     getenv
#   define _ttsystem     system
#   define _ttasctime    asctime
#   define _ttremove     remove
#   define _ttrename     rename
#endif

#define UNUS(x_)   (void)(x_)

typedef unsigned long ulong;
typedef unsigned long THANDLE;
typedef const char kchar;


//-----------------------------------------------------------------------------
class mutex;
class condition
{
public:
    condition()
    {
        pthread_cond_init(&_cond, NULL);
        pthread_mutex_init(&_mutex  ,NULL);
    }
    ~condition()
    {
        pthread_cond_signal(&_cond);
        pthread_mutex_unlock(&_mutex);

        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(&_mutex);
    }
    void lock()
    {
        pthread_mutex_lock(&_mutex);
    }
    void signal()
    {
        pthread_cond_signal(&_cond);
    }
    void broadcast()
    {
        pthread_cond_broadcast(&_cond);
    };

    void wait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }
private:

    pthread_cond_t _cond;
    pthread_mutex_t _mutex;
};
//-------;----------------------------------------------------------------------
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

    virtual ~mutex()
    {
        pthread_mutex_unlock(&_mut);
        pthread_mutex_destroy(&_mut);
    }

    int mlock() const
    {
        int err = pthread_mutex_lock(&_mut);
        return err;
    }

    int try_lock() const
    {
        int err =pthread_mutex_trylock(&_mut);
        return err;
    }

    int munlock() const
    {
        int err =pthread_mutex_unlock(&_mut);
        return err;
    }
};

//-----------------------------------------------------------------------------
class semaphore
{
    sem_t _sem;

public:
    semaphore( int init = 0 )
    {
        sem_init(&_sem,0,init);
    }

    virtual ~semaphore()
    {
        sem_destroy(&_sem);
    }

    int swait(int to=-1) const
    {
        UNUS(to);
        sem_wait((sem_t *)&_sem);
        return 1;
    }

    int stry_wait(int to=-1) const
    {
        UNUS(to);
        return (sem_trywait((sem_t *)&_sem)?errno:0);
    }

    int snotify() const
    {
        return (sem_post((sem_t *)&_sem) ? errno : 0);
    }

    int value() const
    {
        int val = -1;
        sem_getvalue((sem_t *)&_sem,&val);
        return val;
    }
    void reset( int init = 0 )
    {
        sem_destroy(&_sem);
        sem_init(&_sem,0,init);
    }
};


//-----------------------------------------------------------------------------
class AutoLock
{
public:
    AutoLock(mutex* m):_mutex(m)
    {
         _mutex->mlock();
    }
    AutoLock(const mutex* m):_mutex((mutex*)m)
    {
         _mutex->mlock();
    }
    ~AutoLock()
    {
        _mutex->munlock();
    }
private:
    mutex* _mutex;
};

//-----------------------------------------------------------------------------
class Mvi1Wr
{
    pthread_rwlock_t       rwlock;
public:
    Mvi1Wr():rwlock(PTHREAD_RWLOCK_INITIALIZER)
    {

    }
    ~Mvi1Wr()
    {
        pthread_rwlock_destroy(&rwlock);
    }

    void writing()
    {
        int rc = pthread_rwlock_wrlock(&rwlock);
        assert(0 ==rc);
    }

    void done()
    {
         pthread_rwlock_unlock(&rwlock);
    }

    int viewing()
    {
        int k = 65535;//avoid dead infinitte
        int rc = pthread_rwlock_tryrdlock(&rwlock);
        while(rc == EBUSY && k--)
        {
           usleep(0x1FF);
           rc = pthread_rwlock_tryrdlock(&rwlock);
        }
        return rc;
    }


    void viewed(int rc)
    {
        if(rc==0)
            pthread_rwlock_unlock(&rwlock);
    }

    class Read
    {
        const Mvi1Wr& _mv;
        int rc;

    public:

        Read(const Mvi1Wr &v):_mv(v)
        {
            rc = ((Mvi1Wr &)v).viewing();
        }
        ~Read()
        {
            ((Mvi1Wr&)_mv).viewed(rc);
        }
    };

private:
    mutex       _mv, _m, _mp;
    u_int32_t   _viewers;

};

//-----------------------------------------------------------------------------
class OsThread
{
public:
    OsThread()
    {
        _bstop   = 1;
        _hthread = 0;
        _joined  = false;
        _init = -1;
    }

    virtual ~OsThread()
    {
        if(!_stopped)
        {
            stop_thread();
        }

        if(_joined && _hthread)
        {
            //printf("\ndetaching thread %p \n", _hthread);//mem leak fix valgrind
            pthread_detach(_hthread);
        }
        if(_init==0)
            pthread_attr_destroy(&_attr);
        _hthread = 0;

    }
    virtual void _post_thread_foo()
    {
        ;
    }
    virtual bool _pre_thread_foo()
    {
        return true;
    }
    virtual int  start_thread()
    {
        _bstop   = 0;

        _init=pthread_attr_init(&_attr);
        pthread_attr_setdetachstate(&_attr, PTHREAD_CREATE_JOINABLE);

        if (pthread_create(&_hthread, &_attr, SFoo, this) != 0)
        {
            pthread_attr_destroy(&_attr);
            return errno;
        }
        pthread_attr_destroy(&_attr);
        usleep(1000);
        _start.swait();
        return 0;
    }

    virtual void signal_to_stop()
    {
        _bstop = 1;
    }
    virtual void    stop_thread()
    {
        if(!_stopped)
        {
            _bstop = 1;
            usleep(100000);
            t_join();
        }

    }

    bool  is_stopped()
    {
        return _bstop;
    }

    int t_join()
    {
        if(!_joined)
        {
            _joined = true;
            //printf("\njoining thread %p \n", _hthread);//mem leak fix valgrind
            return pthread_join(_hthread,0);
        }
        return 0;
    }

    int kill()
    {
        return 0;
    }
    int detach()
    {
        return 0;
    }

    void set_prio(int boost)
    {
        if(_hthread)
        {
            // pthread_setschedprio(_hthread, 32);
        }
    }
protected:
    virtual void thread_main() {};
    int         _bstop;
private:
    int         _init;
    mutex       _mutex;
    semaphore   _start;
    pthread_attr_t  _attr;
    THANDLE     _hthread;
    int         _stopped;
    bool        _joined;
    static void* SFoo(void* pData)
    {
        OsThread* pT = reinterpret_cast<OsThread*>(pData);
        pT->_stopped = 0;
        pT->_start.snotify();
        if(pT-> _pre_thread_foo())
        {
            pT->thread_main();
            pT->_stopped = 1;
            pT->_post_thread_foo();
            // no member access couse can be deleted
        }
        return 0;
    }
};


template < typename T, class P>struct AutoCall
{
    AutoCall(T f, P p):_f(f),_p(p) {}
    ~AutoCall()
    {
        _f(_p);
    }
    T _f;
    P _p;
};

#define _PLATFORM_ "LINUX"
#endif // !__OS_H__

