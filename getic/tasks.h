#ifndef __TASKS_H__
#define __TASKS_H__

#include "brush.h"
extern HWND  _Hmain;
//---------------------------------------------------------------------------------------


template <class T> class TaskClass
{
    typedef int (T::*PFCB) (void); 
    struct Spr{
        PFCB    pCall;
        T*      pThis;
    };
    T                  *_pcurent;    
    CRITICAL_SECTION    _cs;
    deque<Spr>          _tasks;
    HANDLE              _h,_s,_e;

    public:
    TaskClass(){
        DWORD dw;
        _h = ::CreateThread(0,0,(LPTHREAD_START_ROUTINE)TaskClass::TP, this, 0, &dw);
        _s = CreateEvent(0,0,0,0);
        _e = CreateEvent(0,0,0,0);
        ::InitializeCriticalSection(&_cs);
    }
    void    Stop(){
        ::SetEvent(_s);
        ::SetEvent(_e);
        ::Sleep(100);
        ::WaitForSingleObject(_h,1000);
        _tasks.clear();
        ::CloseHandle(_s);
        ::CloseHandle(_h);
        ::CloseHandle(_e);
        ::DeleteCriticalSection(&_cs);
    }
    static int TP(void* p){return ((TaskClass<T>*)p)->Main();}
    int   Main()
    {
        HANDLE  hndls[] ={_e,_s};
        Spr     t;
        while(1)
        {
            DWORD dc = ::WaitForMultipleObjects(2,hndls,0,INFINITE)-WAIT_OBJECT_0;
            if(dc==1)
                break;
            if(_tasks.size())
            {
                ::EnterCriticalSection(&_cs);
                t = _tasks.at(0);
                _tasks.pop_front();
                ::LeaveCriticalSection(&_cs);
                _TRY
                {
                    _pcurent  = t.pThis;
                    ((_pcurent)->*(t.pCall))();
                    _pcurent  = 0;
                }
                _CATCHX()
                {
                    TRACEX("EXCEPTION \r\n");
                    ASSERT(0);
                }

                ::PostMessage(_Hmain,WM_TIMER,0,0);
                if(_tasks.size())
                {
                    ::SetEvent(_e);
                }
            }
        }
        return 0;
    }
    void Add(T* pclass, PFCB call){
        if(_pcurent == pclass)
            _pcurent->_intask=0;
        Spr s;
        s.pThis = pclass;
        s.pCall = call;
        ::EnterCriticalSection(&_cs);
        _tasks.push_back(s);
        ::LeaveCriticalSection(&_cs);
        ::SetEvent(_e);
    }
};



#endif //
