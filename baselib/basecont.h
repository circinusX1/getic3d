// --------------------------------------------------------------------------------------
// Copyright (c) 2000-2005 Zalsoft Inc
// Copyright: zalsoft Inc
// --------------------------------------------------------------------------------------

#ifndef __BASECONT_H__
#define __BASECONT_H__

#include "baselib.h"
#include "baseutils.h"

//------------------------------------------------------------------------------------
/*
#define _XIOSBASE_
#define _XLOCNUM_
#define _STREAMBUF_
#define _IOS_
#define _OSTREAM_
#define _ISTREAM_
#define _IOSTREAM_
*/
//------------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <deque>
using namespace std;

//------------------------------------------------------------------------------------

typedef int (*PFNSORT)(const void* e1, const void* e2);


class ArrEl{
public:
    int _index;
};

template <class T, int sizesz = 1024>
class PtrArray
{
public:
	PtrArray():_elems(0){}
	~PtrArray(){}
	void ClearPtr(){ while(--_elems>=0){delete t[_elems]; t[_elems]=0;}_elems=0;}
	//T& operator[](int idx){return t[idx];}
    T operator[](int idx)const{return t[idx];}
	T At(int idx)const{return t[idx];}
	void Set(T& tt, int idx){assert(idx < _elems);t[idx] = tt;tt->_index =idx;}
	T Push(T el){if(_elems < sizesz){el->_index = _elems;t[_elems] = el; return t[_elems++];}return 0;}
    T Pop(){return t[--_elems];}
	T* GetBuff(){return t;}
	void Remove(T pt){
		assert(_elems>0);
		int index = pt->_index;
		if(index != _elems-1)
		{
			t[index] = t[_elems-1];
			t[index]->_index = index;
		}
		_elems--;
	}

	void RemoveIdx(int idx){
		assert(_elems>0);
		if(idx != _elems-1)
		{
			t[idx] = t[_elems-1];
			t[idx]->_index = idx;
		}
		_elems--;
	}
	void Reset(){
		_elems=0;
	}
    INLN int Count()const {
		return _elems;
	}
	int ElemSize(){
		return sizeof(T);
	}
    void FullSort(PFNSORT sfn){
        qsort((void*)t, _elems, ElemSize(), sfn);
    }
    BOOL PartialSort(PFNSORT sfn){
        for(int i=1; i<_elems; i++){
            if(sfn(t[i-1], t[i]))
            {
                //SWITCHI((size_t)t[i-1],(size_t)t[i]);
                T t = t[i-1];
                t[i-1]=t[i];
                t[i]=t;
                return TRUE;
            }
        }
        return FALSE;
    }
protected:
	T	t[sizesz];
	int _elems;
};

//------------------------------------------------------------------------------------
template <class T, int sizesz = 128>
class NO_VT RawArray
{
public:
	RawArray():_elems(0){}
	~RawArray(){DeleteElements();}
	void DeleteElements(){}
	T& operator[](int idx){return t[idx];}
	T& At(int idx){ return t[idx];}
	int     Set(T& tt , int idx){assert(idx < _elems); t[idx] = tt;}
    int     Push(T el){if(_elems < sizesz) {t[_elems++] = el; return (_elems-1);} return -1;}
    T&      Pop(){return t[--_elems];}
    T&      Last(){assert(_elems);return t[_elems-1];}
    T&      NextLast(){return t[_elems];}
	T*      GetBuff(){return t;}
	void Remove(int idx){assert(_elems);t[idx] = t[_elems-1];_elems--;}
	void Remove(T& e){for(int i=0;i<_elems;i++){if(t[i] == e){Remove(i);return;}}}
    void Reset(){_elems=0;}
    INLN int Size(){return _elems;}
	int ElemSize(){return sizeof(T);}
    void Increment(){++_elems;}
protected:
	T	t[sizesz];
	int _elems;
};


//------------------------------------------------------------------------------------
template <class T> class Pool
{
public:
	static void CreatePool(UINT cap, BOOL threadSafe=0)
	{
        if(0 != threadSafe)_cs = new mutex();
        else               _cs = 0;
		_pvect = new vector<T*>();
		_pvect->reserve(cap);
		_pbhead = (T*) ::malloc(cap * sizeof(T));
        _nCapacity = cap;
		T* pw	= _pbhead;
		for(UINT i=0; i< cap; i++){
			_pvect->push_back(pw++);
		}
	}

	static void DestroyPool(){
		if(0==_pvect)return;
		_pvect->clear();
		delete _pvect;
		::free((void*)_pbhead);
		_pbhead=0;
        delete _cs;
	}

	void* operator new(size_t sz)
	{
		if(T::_pvect->size() > 0)
        {
            AutoLock q(_cs);
			int szisz = _pvect->size();
			T* pvb = (T*)_pvect->back();
			_pvect->pop_back();
			// szisz = _pvect->size();
			++Pool<T>::_inUse;
			return (void*)(pvb);
		}
        return 0;
	}

	void operator delete(void* pv){
        AutoLock q(_cs);
        --Pool<T>::_inUse;
		_pvect->push_back((T*)pv);
        // int szisz = _pvect->size();
	}
    static int Capacity(){
        return _nCapacity;
    }
    static int Size(){
        return _inUse;
    }
	static T*			_pbhead;
	static vector<T*>*	_pvect;
	static UINT		    _nCapacity;
	static UINT		    _inUse;
    static mutex       *_cs;
};

template <class T> T*				Pool<T>::_pbhead;
template <class T> vector<T*>*      Pool<T>::_pvect;
template <class T> UINT				Pool<T>::_nCapacity;
template <class T> UINT				Pool<T>::_inUse;
template <class T> mutex*  		    Pool<T>::_cs;



//------------------------------------------------------------------------------------
template <class T> class DPool // dynamic pool gets from heap when out of objs
{
public:
    class U : public T
    {
    public:
        U():_polled(0){};
        virtual ~U(){};
        int     _polled;
    };
public:
	static void CreatePool(UINT cap, BOOL threadSafe=0)
	{
        if(0 != threadSafe)_cs = new mutex();
        else               _cs = 0;
		_pvect = new vector<U*>();
		_pvect->reserve(cap);
		_pbhead = (U*) ::malloc(cap * sizeof(U));
        _nCapacity = cap;
		U* pw	= _pbhead;
		for(UINT i=0; i< cap; i++){
			_pvect->push_back(pw++);
		}
	}

	static void DestroyPool(){
		if(0==_pvect)return;
		_pvect->clear();
		delete _pvect;
		::free((void*)_pbhead);
		_pbhead=0;
        delete _cs;
	}

	void* operator new(size_t sz)
	{
		if(T::_pvect->size() > 0)
        {
            AutoLock q(_cs);
			int szisz = _pvect->size();
			U* pvb = (U*)_pvect->back();
			_pvect->pop_back();
			++DPool<T>::_inUse;
            ((U*)pvb)->_polled = szisz;
			return (void*)(pvb);
		}
        else
        {
            U* pu = ::new U();
            pu->_polled=-1;
            return pu;
        }
	}

	void operator delete(void* pv){
        if( ((U*)pv)->_polled !=-1)
        {
            AutoLock q(_cs);
            --DPool<T>::_inUse;
		    _pvect->push_back((U*)pv);
        }
        else
            delete pv;
	}
    static int Capacity(){
        return _nCapacity;
    }
    static int Size(){
        return _inUse;
    }
	static U*			_pbhead;
	static vector<U*>*	_pvect;
	static UINT		    _nCapacity;
	static UINT		    _inUse;
    static mutex       *_cs;
};

template <class T> typename DPool<T>::U*            DPool<T>::_pbhead;
template <class T> vector<typename DPool<T>::U*>*   DPool<T>::_pvect;
template <class T> UINT				                DPool<T>::_nCapacity;
template <class T> UINT				                DPool<T>::_inUse;
template <class T> mutex*  		                    DPool<T>::_cs;

//------------------------------------------------------------------------------------
// #pragma warning (disable:4786)
template <class X> class counted_ptr
{
public:
    explicit counted_ptr(X* p = 0) : _c(0) {if (p) _c = new cnt(p);}
    ~counted_ptr(){dec();}
    counted_ptr(const counted_ptr& r) throw(){add(r._c);}
    counted_ptr& operator=(const counted_ptr& r){if (this != &r){dec(); add(r._c);} return *this;}
    X& operator*()  const throw()   {return *_c->p;}
    X* operator->() const throw()   {return _c->p;}
private:
    struct cnt {
        cnt(X* p = 0, UINT c = 1) : p(p), c(c) {}
        X*          p;
        UINT        c;
    }* _c;
    void add(cnt* c) throw(){_c = c;if (c) ++c->c;}
    void dec(){ if (_c) {if (--_c->c == 0) {delete _c->p; delete _c;}_c = 0;}}
};

//#pragma warning (disable: 4786)

//------------------------------------------------------------------------------------
template <class T> struct RawBuff
{
    T*      _ptr;
    size_t  _size;
    RawBuff():_ptr(0),_size(0){}
	RawBuff(const RawBuff& r){
		_size = r._size;
		_ptr = new T[_size];
        ::memcpy(_ptr,r._ptr,_size*sizeof(T));
	}
    ~RawBuff(){
		delete[] _ptr;
		_ptr=0;
		_size=0;
	}
	RawBuff<T>& operator= (const RawBuff<T>& r){
		Destroy();
		_size = r._size;
		_ptr = new T[_size];
        ::memcpy(_ptr,r._ptr,_size*sizeof(T));
		return *this;
	}
	void Destroy(){
		delete[] _ptr;
		_ptr=0;
		_size=0;
	}
	T* operator&(){
		return _ptr;
	}

    T& operator[](size_t i){
		return _ptr[i];
	}

    void ByteReserve(size_t sz){
		_size = sz/sizeof(T);
		_ptr = new T[_size];
       // ::memset(_ptr,0,_size);
	}
    void ObjReserve(size_t sz){
		_size = sz;
        _ptr = new T[_size];
		size_t sizeoft = sizeof(T);
       // ::memset(_ptr, 0, sizeoft);
	}
    size_t Size(){return _size;}
    size_t ByteSize(){return _size*sizeof(T);}
};

//------------------------------------------------------------------------------------
template <class T, size_t SZ> class RawRing
{
public:
    RawRing(){
        _p   = new T[SZ];
        _gp  = _pp = _cnt = 0;
    }
    ~RawRing(){
        delete[] _p;
    }
    void Put(T& t){
        _p[_pp]=t;
        _pp = ++_pp % SZ;
        if(_gp == _pp)
            _gp = ++_gp % SZ;
        else
            ++_cnt;
    }
    T* Get(){
        if(_cnt){
            T* pret = &_p[_gp];
            _gp = ++_gp % SZ;
            --_cnt;
            return pret;
        }
        return 0;
    }
    int GetTail(){return _pp;}
    int GetHead(){return _gp;}
    T*  GetAt(int i){return &_p[_gp];}
    int Size(){return _cnt;}
    void Reset(){_gp=_pp=0;_cnt=0;}
private:
    T*  _p;
    int _gp;    // get position
    int _pp;    // put position
    int _cnt;   // elems in riung
};

//------------------------------------------------------------------------------------
template <class T, class CLS, class CB, class PAR>
void forall(T& coll, CLS* pC, CB cbc, PAR& par)
{
    typename T::iterator b = coll.begin();
    typename T::iterator e = coll.end();
    for(;b!=e;b++){
		if(FALSE == ((pC)->*cbc)(*b, par))
            break;
    }
}

//------------------------------------------------------------------------------------
template <class T, class CB, class PAR>
void forall(T& coll,CB cbc,PAR* pd=0)
{
    typename T::iterator b = coll.begin();
    typename T::iterator e = coll.end();
    for(;b!=e;b++){
			if(FALSE == (cbc)(*b,pd))
                break;
    }
}

//------------------------------------------------------------------------------------
template <class T, class CLS, class CB, class PAR, class PAR1>
void forall2(T& coll, CLS* pC, CB cbc, PAR& par, PAR1& par1)
{
    typename T::iterator b = coll.begin();
    typename T::iterator e = coll.end();
    for(;b!=e;b++){
		if(FALSE == ((pC)->*cbc)(*b, par, par1))
            break;
    }
}

//------------------------------------------------------------------------------------
template <class T, class U> void append(T& a, U& b){
	FOREACH(typename U,b,pb) {
		a.push_back(*pb);
	}
}

//------------------------------------------------------------------------------------
template <class T, class P>
void appendif(T& a, T& b, P p){
    FOREACH(typename T,b,pb) {
        if((p)(*pb))
		    a.push_back(*pb);
	}
}

//------------------------------------------------------------------------------------
template <class T>void reverse(T& t){
	static T ret;
	ret.clear();
	ret =  t;
	t.clear();
	typename T::reverse_iterator b = ret.rbegin();
	typename T::reverse_iterator e = ret.rend();
    for(;b!=e;b++)
		t.push_back(*b);
}

//---------------------------------------------------------------------------------------
template <class T> class vvector : public vector<T>
{
public:
	typedef typename vector<T>::iterator vit;

	vvector(){_els=0;}
	vvector(int sz){
	    _els=0;
	    this->vector<T>::reserve(sz);
	    }
	void push_back(const T& o){
		this->vector<T>::push_back(o);
		refresh();
	}
	void clear(){
		vector<T>::clear();
		_els=this->vector<T>::size();
	}
	void	 pop_back(){
		vector<T>::pop_back();
		refresh();
	}

    T& nextr(int  idx){ //rw
        if(idx==this->vector<T>::size()-1)
            return this->vector<T>::at(0);
        return this->vector<T>::at(idx+1);
    }

    T& prevr(int  idx){ //rw
        if(idx==0)
            return this->vector<T>::back();
        return this->vector<T>::at(idx-1);
    }

	vit erase( vit _P){
		vit it = this->vector<T>::erase(_P);
		refresh();
		return it;
	}
	void deleteelements(){
		FOREACH(typename vector<T>,(*this),pp)
			delete (*pp);
		clear();
        refresh();
	}
	void deleteelementsarr(){
		FOREACH(typename vector<T>,(*this),pp)
			delete[] (*pp);
		clear();
        refresh();
	}
    
    BOOL has(T& o){
        return findelement(o)!=this->vector<T>::end();
    }

    size_t getindex(T& o){
        vit it = findelement(o);
        if(it != this->vector<T>::end())
            return it-this->vector<T>::begin();
        return -1;
    }

    vit findelement(T& o){
        return find(this->vector<T>::begin(),this->vector<T>::end(),o);
    }
    vvector<T>& operator <<(const T& o){
        this->push_back(o);
        return *this;
    }
    void refresh(){
        _els=this->vector<T>::size();
    };
    void destroy()
    {
        clear();
        _Destroy(this->vector<T>::begin(), this->vector<T>::end());
    }

    int push_unique( T& o, size_t* idx=0){
        if(!this->vector<T>::size())
        {
            this->push_back(o);
            refresh();
            if(idx)*idx=0;
            return 1;
        }
        else
        {
            vit el = findelement(o);
            if( el == this->vector<T>::end()){
                if(idx)* idx=this->vector<T>::size();
                this->vector<T>::push_back(o);
                refresh();
                return 1;
            }
            if(idx)*idx=el-this->vector<T>::begin();
        }
        return 0;
    }
    int erase_if( T& o){
        vit fi = findelement(o);
        if(fi  != this->vector<T>::end()){
            this->erase(fi);
            refresh();
            return 1;
        }return 0;
    }
    void reverse()
    {
        vvector<T>   rev = *this;
        this->vector<T>::clear();
        typename vector<T>::reverse_iterator b = rev.rbegin();
        typename vector<T>::reverse_iterator e = rev.rend();
        for(;b!=e;b++)
        {
            (*this) <<(*b);
        }
    }
private:
	size_t		_els;
};

//-----------------------------------------------------------------------------
template <class T> T* element(T* table2D, int x, int y, int xM)
{
    return (table2D + (y * xM + x));
}
//-----------------------------------------------------------------------------
typedef basic_string<char> tstring;
typedef basic_string<char> t_string;

//-----------------------------------------------------------------------------
template <class T, size_t SZ>class Bucket
{
public:
    Bucket(){}
    ~Bucket(){}
    PtrArray<T>&    Array(int i){return _arrays[i];}
    int             Size(){return SZ;}

public:
    PtrArray<T>         _arrays[SZ];
};

//------------------------------------------------------------------------------
// shyncronized queue
template <class T> class cs_deque : public deque<T>
{
public:
    cs_deque(const char *pn){
        ::strcpy(_name,pn);
        _maxsz = 256;
        _actl  = 0;
    };
    virtual ~cs_deque(){
            this->deque<T>::clear();
    };

    void SetMaxSize(long ms){
        _maxsz = ms;
    }
    long    Count(){
        AutoLock q(&_cs);
         return this->deque<T>::size();
    }

    void Put(T row){
        _cs.Lock();
        this->deque<T>::push_back(row);
        _actl = this->deque<T>::size();
        _cs.Unlock();
        _hmq.Notify();
    }

    int TryGet(T* prow)
    {
        if(_cs.TryLock())
        {
            _actl = this->deque<T>::size();
		    if(_actl)
		    {
                *prow = this->deque<T>::at(0);
			    this->deque<T>::pop_front();
		    }
            _cs.Unlock();
        }
        if(_actl)
        {
            _hmq.Notify();
        }
        return _actl;
    }
    int Get(T* prow)
    {
        _cs.Lock();
        _actl = this->deque<T>::size();
		if(_actl)
		{
            *prow = this->deque<T>::at(0);
			this->deque<T>::pop_front();
		}
        _cs.Unlock();
        if(_actl)
        {
            _hmq.Notify();
        }
        return _actl;
    }

    void Lock(){_cs.Lock();}
    void Unlock(){_cs.Unlock();}
    int Pick(T* prow)
    {
        _actl = this->deque<T>::size();
		if(_actl)
		{
            *prow = this->deque<T>::at(0);
			this->deque<T>::pop_front();
		}
        return _actl;
    }
    void Clear()
    {
        this->deque<T>::clear();
    }

    mutex            _cs;
    semaphore        _hmq;
    int              _maxsz;
    int              _actl;
    char             _name[32];
};

// makes member index access of the element at position index to be able to
// access them from script engine
template <class T> class Buf
{
public:
    explicit Buf():_pt(0){}
    explicit Buf(T* p):_pt(p){}

    operator const T*()const{return _pt;}
    operator T*(){return _pt;}
    const Buf<T>&  operator =( T* p){_pt=p; return this;}
    T&  at(int idx){return *(_pt+idx);}
    const T&  operator[](int idx)const{return *(_pt+idx);}
    T&  operator[](int idx){return *(_pt+idx);}
private:
    T* _pt;
};






#endif //__BASECONT_H__

