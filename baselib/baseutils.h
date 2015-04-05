// --------------------------------------------------------------------------------------
// Copyright (c) 2000-2005 Zalsoft Inc
// Copyright: zalsoft Inc
// --------------------------------------------------------------------------------------

#ifndef __BASE_UTILS_H__
#define __BASE_UTILS_H__

#include "baselib.h"
#ifdef _CFG_WIN32_WIN
        #include <crtdbg.h>
#endif //

//---------------------------------------------------------------------------------------
#ifdef _UNICODE
    #pragma message ("building unicode version of the libray")
#endif//

#ifdef _DEBUG
#ifdef WINDOWS
        INLN void _cdecl TRACEX(LPCSTR lpszFormat, ...)
	    {
		    static CHAR szBuffer[1024];

            va_list args;
		    va_start(args, lpszFormat);
            unsigned int nBuf = ::_vsnprintf(szBuffer, sizeof(szBuffer) / sizeof(CHAR), lpszFormat, args);
#ifdef _CONSOLE
            printf(szBuffer);
#else
            ::OutputDebugStringA(szBuffer);
#endif //
		    va_end(args);
	    }
#else
        #define TRACEX printf
#endif
    #else //
        #ifdef WIN32
            #define TRACEX	 
        #else
            #ifdef _CONSOLE
                #define TRACEX printf
            #else
                INLN TRACEX(LPCTSTR lpszFormat, ...){}
            #endif //
        #endif

    #endif //

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
#define DECLARE_TIME()      static LARGE_INTEGER _Start;\
                                static LARGE_INTEGER _Freq;\
                                static LARGE_INTEGER _End;\
                                QueryPerformanceFrequency(&_Freq);\
                                QueryPerformanceCounter(&_Start)
#define WAIT_HERE(msec)     QueryPerformanceCounter(&_End);\
                            while( (_End.QuadPart - _Start.QuadPart)*1000 / _Freq.QuadPart < msec)\
							{\
								 QueryPerformanceCounter(&_End);\
								 Sleep(0);\
							}
#define TRACETIME(msec)     QueryPerformanceCounter(&_End);\
                            TRACEX(_T("%s %d ms\r\n"),msec ,(_End.QuadPart - _Start.QuadPart)*1000 / _Freq.QuadPart)
#define READ_CLOCK()        QueryPerformanceCounter(&_Start)

namespace baseg{

//---------------------------------------------------------------------------------------
INLN void StripEol(char* ln){
    while(ln[0] && (ln[strlen(ln)-1]=='\n' ||ln[strlen(ln)-1]=='\r'))
        ln[strlen(ln)-1]=0;
}

//---------------------------------------------------------------------------------------
template <class T> class InstCount
{
    static long _lCount;
public:
	InstCount(){
		TRACEX(_T("%d\r\n"),_lCount);
		_lCount++;
	}
	~InstCount(){
		_lCount--;
		TRACEX(_T("%d\r\n"),_lCount);
	}

};
template <class T> long              InstCount<T>::_lCount = 0;

//---------------------------------------------------------------------------------------
template <class T>BOOL _Noval(T& val)
{
    if(sizeof(val)==1)return (val == 0xFF);
    if(sizeof(val)==2)return (val == 0xFFFF);
    if(sizeof(val)==4)return (val == 0xFFFFFFFF);
    if(sizeof(val)==8)return (val == 0xFFFFFFFFFFFFFFFF);
    return 0;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
template <size_t SZ=256> class Tsstring
{
public:
    Tsstring(){_t[0]=0;}
    Tsstring(const char* o){::_tcscpy(_t, o);}
    const char* c_str(){return _t;}

private:
    char   _t[SZ];
};

typedef Tsstring<16>    S16char;     
typedef Tsstring<64>    S64char;    
typedef Tsstring<128>   S128char;    
typedef Tsstring<256>   S256char;    

//---------------------------------------------------------------------------------------
class CBitArray
{
public:
	CBitArray(int size){
		_size =     (size+ 7) / 8;
		_size = (_size * 3 + 3) & 0xFFFFFFFC;
		_storage = new BYTE[_size];
		memset(_storage, 0, _size);
	}
	virtual ~CBitArray()
	{
		delete[] _storage;
	}

    void Clear(){memset(_storage, 0, _size);}

	BOOL operator[](int i){
		return (_storage[ i>> 3 ] & (1 << ( i& 7))) != 0;
	}
	void Set(int i){
		_storage[ i >> 3 ] |=  (1 << ( i & 7 ));
	}
	void Reset(int i=0){
		if(0==i)
			::memset(_storage, 0, _size);
		else
			_storage[ i >> 3 ] &= ~(1 << ( i & 7 ));
	}
	int Size(){return _size;}
    BYTE* Buffer(int& bytes){bytes = _size; return _storage;}
    void  Merge(CBitArray& pByArr){
        int cnt;
        BYTE* pDSrc = (BYTE*)pByArr.Buffer(cnt);
        BYTE* pDDst = Buffer(cnt);
        while(--cnt>=0){
            *pDDst++ |= *pDSrc++;
        }
    }
private:
	BYTE*	_storage;
	int		_size;
};

//------------------------------------------------------------------------------
template <class T> class Allocator
{
	T*	_p;
	public:
	Allocator(int ins){
		_p = new T[ins];
		::memset(_p, 0, sizeof(T)*ins);
	}
	Allocator(){
		_p = new T;
	}
	operator T*(){return _p;}
};

//---------------------------------------------------------------------------------------
INLN int BitsRoundByLong(int nSetCount)
{
    int rval =   (nSetCount + 7) / 8;    // bytes
    rval = (rval * 3 + 3) & 0xFFFFFFFC;  // round by mod4
    return rval;
}

//---------------------------------------------------------------------------------------
class Cfms
{
public:
#ifdef _UNICODE
    Cfms(WCHAR* p,...){
		va_list args;
		va_start(args, p);
        ::_vsnwprintf(_s, sizeof(_s) / sizeof(char), p, args);
		va_end(args);
	}
	operator const WCHAR*(){return _s;}
	WCHAR _s[256];
#else
	Cfms(CHAR* p,...){
		va_list args;
		va_start(args, p);
        ::_vsnprintf(_sc, sizeof(_sc) / sizeof(CHAR), p, args);
		va_end(args);
	}
    operator const CHAR*(){return _sc;}
    CHAR _sc[256];
#endif //
};
#define	MKSTR	(const char*)Cfms

//---------------------------------------------------------------------------------------
class NO_VT FileWrap
{
public:
    int mOffset;
    FileWrap(FILE* pf=0):_pf(pf),_openhere(FALSE),_store(FALSE){};
    virtual ~FileWrap(){
        mOffset=0;
        if(_pf && _openhere)
            ::fclose(_pf);
        _pf=0;
    }

    BOOL    Open(const CHAR* file, const CHAR* how){
        char loco[256];
        strcpy(loco, file);
        mOffset=0;
#ifndef _CFG_GNU_LINUX
        // replace / with 
        char* ps = loco;
        while(*ps)
        {
            if(*ps=='/')
                *ps='\\';
            ++ps;
        }
#endif //

        _pf = ::fopen(loco, how);
        _openhere =  _pf!=0;
        _store = (how[0]==_T('w'));
        return _openhere;
    }
    void Close(){
        if(_pf)
            ::fclose(_pf);
        _pf=0;
        mOffset=0;
    }

    void Pos(int pos){
        ::fseek(_pf,pos,SEEK_SET);
        mOffset=pos;
    }

    void ReStart(){
        ::fseek(_pf,0,SEEK_SET);
        mOffset=0;
    }

    DWORD GetPos(){
        return ::ftell(_pf);
    }

    BOOL IsEof(){
        return feof(_pf);
    }

    DWORD Getlength(){
        ::fseek(_pf,0,SEEK_END);
        DWORD flength = ftell(_pf);
        ::fseek(_pf,0,SEEK_SET);
        return flength;
    }

    BOOL ReadLine(char* line, int len){
        mOffset+=len;
        return ::fgets(line,len,_pf)!=0;
    }

    template <class T> BOOL Serialize(T& t){
        if(_store) return Write(t);
        return Read(t);
    }

    template <class T> BOOL Write(T& t){
        int lenr = ::fwrite(&t,1,sizeof(T),_pf);
        mOffset+=lenr;
        return (lenr == sizeof(T));
    }

    template <class T> BOOL Read(T& t) {
        int lenr = ::fread(&t,1,sizeof(T),_pf);
        mOffset+=lenr;
        return (lenr == sizeof(T));
    }

    SIZE_T Read(void* pv, size_t len) {
        size_t lenr= ::fread(pv,1,len,_pf);
        mOffset+=lenr;
        return (lenr);
    }

    size_t Write(void* pb, size_t len) {
        mOffset+=len;
        return  ::fwrite(pb,1,len,_pf);
        
    }

    size_t Serialize(void* pv, int len){
        mOffset+=len;
        if(_store) return Write(pv,len);
        return Read(pv,len);
    }

    size_t WriteTextAsIs(const char* t){
        mOffset+=strlen(t);
		size_t l = strlen(t);
        return fwrite(t, 1, strlen(t),  _pf);
    }


    size_t WriteStr(const char* t){
		size_t l = strlen(t);
        mOffset+=strlen(t)+sizeof(short);
        ::fwrite(&l, 1, sizeof(short), _pf);
        return fwrite(t, 1, strlen(t),  _pf);
    }

    size_t ReadStr(char* t){
        short len;
        ::fread(&len, 1, sizeof(short), _pf);
        size_t rd = ::fread((void*)t, 1, len,  _pf);
        t[rd]=0;
        mOffset+=len+sizeof(short);
        return rd=len;
    }
    BOOL    _openhere;
    FILE*   _pf;
    BOOL    _store;
};

//---------------------------------------------------------------------------------------
template <class T> class NO_VT PlugInDll
{
public:
	typedef T*   (*CreateT)(void);
	typedef void (*DeleteT)(T*);
public:
	PlugInDll():_pIface(0),_hdll(0),_refs(0){
        ::memset(_dllName,0,sizeof(_dllName));
    }
    void    SetIface(T* pIface){
                    if(0 == _pIface)
                    {
                        _pIface = pIface;
                        ++_refs;
                    }
    }
	PlugInDll(const char* szDll):_pIface(0),_refs(0),_hdll(0)
	{
		_tcscpy(_dllName,szDll);
		Load(szDll);
	}
	BOOL Load(const char* szDll, BOOL create=TRUE)
	{
        BOOL rv = FALSE;
		_tcscpy(_dllName,szDll);
        if(!_hdll)
        {
		    _hdll = LoadLibrary(szDll);
		    if(_hdll)
		    {
			        CreateT	ptc = (CreateT)GetProcAddress(_hdll,_T("CreateInstance"));
			        if(ptc)
			        {
                        if(create)
                        {
    				        _pIface = (ptc)();
                            ++_refs;
                        }
                        rv = TRUE;
			        }
		    }
        }
        return rv;
	}

    void Free()
    {
        if(_hdll)_refs--;
		if(0 == _refs && _hdll)
		{
			DeleteT  pdc = (DeleteT)GetProcAddress(_hdll,"DestroyInstance");
			(pdc)(_pIface);
            _pIface = 0;
			FreeLibrary(_hdll);
            _hdll  = 0;
		}
    }

    T*  New(void)
    {
        if(_hdll)
		{
			CreateT	ptc = (CreateT)GetProcAddress(_hdll,_T("CreateInstance"));
			if(ptc)
			{
				return (T*)(ptc)();
			}
		}
        return 0;
    }

    void Delete(T* pt)
    {
		if(_hdll)
		{
			DeleteT  pdc = (DeleteT)GetProcAddress(_hdll,"DestroyInstance");
			(pdc)(pt);
		}
    }

	BOOL IsValid(){
        return _refs > 0;
    }
	virtual ~PlugInDll(){
        _refs=1;
        Free();
		TRACEX(MKSTR(_T("~PlugInDll %s \r\n"),_dllName));
	}
    T* Interface(){return _pIface;}
	T* operator->(){return _pIface;}
private:
	T*			_pIface;
	HINSTANCE	_hdll;
    int         _refs;
	char		_dllName[128];
};

//----------------------|performance timer class def|------------------------------------
class NO_VT Timer
{
public:
    Timer(){
	    _fRate	= 0; _fCnt	= 0; _fpsTo = 0.00;
        if (QueryPerformanceFrequency((LARGE_INTEGER *)&_pFreq)) {
		    QueryPerformanceCounter((LARGE_INTEGER *) &_lt);
		    _ts	 = 1.00 / _pFreq;
	    }
    }

    INLN DWORD Tick( float fLockFPS = 0.00 ) {
	    QueryPerformanceCounter((LARGE_INTEGER *)&_ct);
	    _dt = (_ct - _lt) * _ts;
        if ( fLockFPS > 0.00 ) {
            while ( _dt < (1.00 / fLockFPS))
            {
	            QueryPerformanceCounter((LARGE_INTEGER*)&_ct);
	            _dt = (_ct - _lt) * _ts;
            }
        }
	    _lt = _ct;
	    _fCnt++;
	    _fpsTo += _dt;
	    if ( _fpsTo > 1.00)
        {
		    _fRate	= _fCnt;
		    _fCnt	= 0;
		    _fpsTo	= 0.00;
	    }
        return _fRate;
    }
    REAL GetDeltaTime(){return _dt;}
    REAL GetCurentTime(){return _ct;}
private:
	REAL    _ts;
	REAL    _dt;
	REAL    _fpsTo;
    DWORD   _fRate;
	DWORD   _fCnt;
    __int64 _ct;
    __int64 _lt;
	__int64 _pFreq;
};

//-----------------------------------------------------------------------------------
class NO_VT CDirChange
{
public:
	CDirChange(const char * newDir){
        ::_tgetcwd(_olddir,_MAX_PATH);
        if(newDir)
        {
            _tcscpy(_curdir, newDir);
            if( _curdir[_tcslen(_curdir)-1] == _T('\\') ||
                _curdir[_tcslen(_curdir)-1] == _T('/'))
            {
                _curdir[_tcslen(_curdir)-1] = 0;
            }
            _rv = _tchdir(_curdir);
        }
		_tgetcwd(_curdir,_MAX_PATH);
	}
	~CDirChange(){
		_tchdir(_olddir);
	}
    const char* CurDir(){return _curdir;};
    const char* OldDir(){return _olddir;};
    char* UseObj(){
        return _curdir;
    }
    int InError(){return _rv;}
private:
	char _olddir[_MAX_PATH];
    char _curdir[_MAX_PATH];
    int   _rv;
};

//----------------------||----------------------------------------------------------------
class NO_VT  PathHandler
{
public:
    static void FixSlashes(char* fname, char bs=BS)
    {
        char* p=fname;
        while(*p){
            if(*p==OPBS) *p = BS;
            ++p;
        }
    }


    PathHandler(const char* fullPathFile)
    {
        ::_tcscpy(_szPath, fullPathFile);
        ::_tsplitpath(fullPathFile, _szDrive, _szDir, _szFName, _szExt);
    }
    const char* Path(){
        ::_stprintf(_szTemp,_T("%s%s"),_szDrive,_szDir);
        return _szTemp;
    }
    const char* File(){
        ::_stprintf(_szTemp,_T("%s%s"),_szFName,_szExt);
        return _szTemp;
    }
    const char* Ext(){
        return _szExt;
    }
    const char* Drive(){
        return _szDrive;
    }
	const char* JustFile(){
		return _szFName;
	}
private:
    char   _szDir[_MAX_PATH];
    char   _szFName[_MAX_FNAME];
    char   _szExt[_MAX_EXT];
    char   _szDrive[_MAX_DRIVE];
    char   _szPath[_MAX_PATH];
    char   _szTemp[_MAX_PATH];
};


//----------------------||----------------------------------------------------------------
template <class T>class ResetExit
{
    T&  _ref;
public:
    ResetExit(T& ref):_ref(ref){}
    ~ResetExit(){_ref=0;}
};

//----------------------||----------------------------------------------------------------
template <class T>class DeleteExit
{
    T&  _ref;
public:
    DeleteExit(T& ref):_ref(ref){}
    ~DeleteExit(){delete _ref;}
};

//----------------------|functors|-----------------------------------------------------
template <class T>void Reset(T t)
{
	::memset(&t,0,sizeof(T));
}

template <class T> void Copy(T&a , T&b)
{
    ::memcpy(&a,&b,sizeof(T));
}

INLN void SCopy(char* a , char* b, int sza)
{
    ::strncpy(a, b, sza-1);
    a[sza-1]=0;
}

template <class T>
class ObjSizeDump
{
public:
    ObjSizeDump(const char* name){
        TRACEX("%s: %d \r\n",name, sizeof(T));
    }
    virtual ~ObjSizeDump(){}
};





#define OBJ_DMP(_x_)    ObjSizeDump<_x_>   _##_x_(#_x_)


#define ITOFX(x) ((x) << 8)			    // Integer to fixed point
#define FTOFX(x) ((x) * 256)			    // Float to fixed point
#define DTOFX(x) ((x) * 256)			    // Double to fixed point
#define FXTOI(x) ((x) >> 8)			    // Fixed point to integer
#define FXTOF(x) ((float) (x) / 256)		// Fixed point to float
#define MULFX(x,y) ( ((y) * (x)) >> 8 )	// Multiply a fixed by a fixed
#define DIVFX(x,y) ((y << 8) / (x))    	// Divide a fixed by a fixed


#ifdef _CFG_WIN32_WIN
struct Cms
{
// Attributes
	enum blockUsage
	{
		freeBlock,    // memory not used
		objectBlock,  // contains a CObject derived class object
		bitBlock,     // contains ::operator new data
		crtBlock,
		ignoredBlock,
		nBlockUseMax  // total number of usages
	};

	_CrtMemState m_memState;
	size_t m_lCounts[nBlockUseMax];
	size_t m_lSizes[nBlockUseMax];
	size_t m_lHighWaterCount;
	size_t m_lTotalCount;

	Cms()
	{
		::memset(this, 0, sizeof(*this));
	}


// Operations
	void Checkpoint()
	{
    	_CrtMemCheckpoint(&m_memState);
		UpdateData();
	}

	BOOL Difference(const Cms& oldState,
					const Cms& newState)
	{

		int nResult = _CrtMemDifference(&m_memState, &oldState.m_memState, &newState.m_memState);
		UpdateData();
		return nResult != 0;
	}

	void UpdateData()
	 {
		for(int i = 0; i < nBlockUseMax; i++)
		{
			m_lCounts[i] = m_memState.lCounts[i];
			m_lSizes[i]  = m_memState.lSizes[i];
		}
		m_lHighWaterCount = m_memState.lHighWaterCount;
		m_lTotalCount = m_memState.lTotalCount;
	}


	// Output to afxDump
	void DumpStatistics() const
	{
		_CrtMemDumpStatistics(&m_memState);
	}
	void DumpAllObjectsSince() const
	{
		_CrtMemDumpAllObjectsSince(&m_memState);
	}
};

/*//////////////////////////////////////////////////////////////////////////////
	macros to count instances of a given object
	produces traces  indicating object count


   BEGIN_OBJCOUNT(CYourClass) // in cpp file add this at the top of file

   class CYourClass
   {
	   CYourClass()
	   {
			OBJPUT("My Object Name")    ; // do this for copy constructor too
	   }
	   ~CYourClass()
	   {
		   OBJGET("My Object Name")    ;
	   }

	   DECLARE_OBJCOUNT;    // add this line in your class
   };
*////////////////////////////////////////////////////////////////////////////////

#define DECLARE_OBJCOUNT\
				private:\
				static int m_ObjCount;\
#define BEGIN_OBJCOUNT(_className_)\
					int _className_::m_ObjCount=0;\
#define OBJPUT(_obj_)	do{\
							::InterlockedIncrement(&m_ObjCount);\
							ALTTRACE(_obj_, m_ObjCount);\
						}while(0);\
#define OBJGET(_obj_)		do{\
							ALTTRACE(_obj_,  m_ObjCount);\
							::InterlockedDecrement(&m_ObjCount);\
						}while(0);\

/*/////////////////////////////////////////////////////////////////////////////////
	MCCROS To CHECK MEMORY LEAKS FOR A CLASS OR PORTION OF A CODE: uses as explained


	class CYourClass
	{
		CYourClass()
		{
			MINIT0(somename);
		}


		~CYourClass()
		{

		}

		 void Foo();

		MDECLARE(somename);

	};


	void CYourClass::Foo()
	{
		MSTART(somename , "Checking meory in funtion ...");        // the same name as in MDECLARE

		  .. lot's of code

		MEND(somename); /// produces memeory dump if is any


		// you repeat this sequence

	}


  // FOR CODE OUT OF CLASS"

  SomeFunction()
  {
	  MSTART(xyz,"Checking point line xxx");

	.. lot's of code


	  MEND(xyz); /// produces memeory dump if is any
  }
*////////////////////////////////////////////////////////////////////////////////////////////////

class  MEMCHECK
{
public:
	MEMCHECK(char* pLine)
	{
		sprintf(m_line,"\r\n---------------Memory Dump For %s -----------\r\n",pLine);
		oMem.Checkpoint();
	}

	~MEMCHECK()
	{
		TRACEX(m_line);
		newMem.Checkpoint();
		if(diffMem.Difference(oMem,newMem))
		{

			TRACEX("\r\n");
			diffMem.DumpStatistics();
			TRACEX("\r\n________________________________________\r\n");
			TRACEX("\r\n Dumping now all objects\r\n");
			oMem.DumpAllObjectsSince();
			TRACEX("\r\n________________________________________\r\n");
		}
	}
	Cms   oMem,newMem,diffMem;
	char m_line[200];
};
#define MEM_START(xyz,_l_)MEMCHECK* xyz = new MEMCHECK(_l_)
#define MEM_END(xyz)      if(xyz!=NULL) delete xyz;xyz=NULL;
#define MDECLARE(xyz)     MEMCHECK* xyz
#define MINIT0(xyz)       xyz = NULL
#define MINIT(xyz)        MEMCHECK* xyz = NULL
#define MSTART(xyz,_l_)   if(xyz == NULL)xyz = new MEMCHECK(_l_)
#define MEND(xyz)         if(xyz!=NULL) delete xyz;xyz=NULL;
#else
#define MEM_START(xyz,_l_)
#define MEM_END(xyz)
#define MDECLARE(xyz)
#define MINIT0(xyz)
#define MINIT(xyz)
#define MSTART(xyz,_l_)
#define MEND(xyz)

#endif //


};//baseg

#ifdef _USE_BASEG
    using namespace baseg;
#endif //


#endif //
