#ifndef __TEXREF2_H__
#define __TEXREF2_H__

#include "baselib.h"
#include "texsys.h"
//#pragma warning (disable: 4786)

//---------------------------------------------------------------------------------------
#define TEX_CAPACITY    32768
#define WM_REMOVETEX    (WM_USER+1007)

//---------------------------------------------------------------------------------------
class TexRef  : public TexSys
{
public:
    TexRef():TexSys(TexRef::GlGenTex, TexRef::RemoveTex){
		_texmap = new size_t[TEX_CAPACITY+1];
        ::memset(_texmap,0,sizeof(size_t)*(TEX_CAPACITY));
    };
    ~TexRef(){
		TexSys::Clean();
		delete[]_texmap;
	};
public:
	static void  Clear(BOOL b=0);
    static void	 _RemoveTex(Htex& it);
    static Htex  _GlGenTex(int x, int y, int bpp, BYTE* pBuff, size_t flags);
    static Htex	 GlGenTex(int x, int y, int bpp, BYTE* pBuff, size_t flags);
    static void	 RemoveTex(Htex* it, int);
    static Htex  LoadRCTexture(const char* id,int,int,BYTE* pwantbuff=0);
    static BYTE* LoadRCTextureBuffer(const char* id,int,int);
    static Htex  LoadFile(char* pszFileName, size_t flags);
    static Htex  LoadThisFile(const char* pszFileName, size_t flags);
    static void  GlLmapMode(BOOL start);
    static void  GlDecalMode(BOOL start);
    static void  GlHalloMode(BOOL start);
    static void  GlDetailMode(BOOL start);
	static Htex	_defTex;
    static BOOL _MakeCurrent;
    size_t*       _texmap;    
};


//---------------------------------------------------------------------------------------
// implements fast per/object reference count to minimize hits on TexSys
class SmartTex
{
public:
    static   int _TexCopy;
    explicit SmartTex();
             SmartTex (const SmartTex& r);
    virtual  ~SmartTex();
    SmartTex& operator=(const SmartTex& r);
    SmartTex& operator=(const Htex& ht);
    operator    size_t(){return _hTex;}
    bool operator==(const SmartTex& r){return (r._hTex.hTex == _hTex.hTex);};
    void        operator--();
    const       char* GetTexName();
	void        AddRef();
	SmartTex&   Assign(const char* p, size_t flags);
	void        Reset();
    BOOL        Valid(){return _hTex.hTex > 0;};
    operator    Htex&(){return _hTex;};
private:
    SmartTex& operator = (const char* p){return Assign(p, 0);};
    void Clear();
private:
    Htex     _hTex;
    size_t*    _pRefs;
};


//---------------------------------------------------------------------------------------
// texture indexex
enum {TX_0 = 0, TX_LM, TX_1, TX_2};         //tex index 
//---------------------------------------------------------------------------------------
// apply flags, gen flags, texture stage flags
struct Texs
{
    Texs(){memset(_texApply,0,sizeof(_texApply));}
    virtual ~Texs(){
    }
    Texs(const Texs& r){
        _texts[0] = r._texts[0];
        _texts[1] = r._texts[1];
        _texts[2] = r._texts[2];
        _texts[3] = r._texts[3];
        _texts[4] = r._texts[4];
        ::memcpy(_texApply, r._texApply, sizeof(_texApply));
    }

    Texs& operator=(const Texs& r){
        if(&r != this)
        {
            _texts[0] = r._texts[0];
            _texts[1] = r._texts[1];
            _texts[2] = r._texts[2];
            _texts[3] = r._texts[3];
            _texts[4] = r._texts[4];
            ::memcpy(_texApply, r._texApply, sizeof(_texApply));
        }
        return *this;
    }
    SmartTex& operator[](size_t i){return _texts[i];}
    void     GetTextureInts(WORD *texts, size_t* applyes){
        for(int i=0;i<4;i++)*(texts+i) = (WORD)(size_t)_texts[i];
        memcpy(applyes, _texApply, sizeof(size_t)*4);
    }
    SmartTex    _texts[5];
    size_t       _texApply[5];		// target, genST, envMode
};

//---------------------------------------------------------------------------------------
void    MakeCurrent(HDC hdc, HGLRC hrc);
void    MakeCurrent(char);
class NO_VT NoTexCopy{
public:
    NoTexCopy(){
        SmartTex::_TexCopy=0;
    }
    ~NoTexCopy(){
        SmartTex::_TexCopy=1;
    }
};


#endif // !__TEXREF_H__
