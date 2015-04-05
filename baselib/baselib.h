// --------------------------------------------------------------------------------------
// Copyright (c) 2000-2005 Zalsoft Inc
// Copyright: zalsoft Inc
// --------------------------------------------------------------------------------------
#ifndef __BASELIB_H__
#define __BASELIB_H__
#pragma pack(push, 8)

#define _USE_BASEG

#ifdef _CFG_WIN32_WIN
#define OS_DEFINED 
#ifndef WINDOWS
#define WINDOWS
#endif //
#include "WIN_WIN\\os.h"
#endif

#ifdef _CFG_GNU_WIN
#define OS_DEFINED
#ifndef WINDOWSG
#define WINDOWSG
#endif //
#include "GNU_WIN\\os.h"
#endif

#ifdef _CFG_GNU_LINUX
#define OS_DEFINED
#include "GNU_LINUX/os.h"
#endif


#ifndef OS_DEFINED
#pragma message ("error: _CFG_GNU_LINUX, _CFG_GNU_WIN or _CFG_WIN32_WIN Not defined")
#endif //

#pragma warning (disable: 4267)
#pragma warning (disable: 4018)
#pragma warning (disable: 4786)
#pragma warning (disable: 4006)
#pragma warning (disable: 4244)
#pragma warning (disable: 4996)



//---------------------------------------------------------------------------------------
#ifdef _BDL_PREC
typedef	double REAL;
#else
typedef	float REAL;
#pragma warning(disable: 4305)
#endif //

//---------------------------------------------------------------------------------------
#ifdef _USRDLL
#define EX_IN __declspec(dllexport)
#else
#define EX_IN __declspec(dllimport)
#endif

//---------------------------------------------------------------------------------------
#define VIRT            // dummy macro for virtual function
#define ABSTRACT        // dummy macro for abstract classes
#define STATIC          // dummy macro for static funtions
#define THRFN           // dummy macro for function on TLS
#define _SCOPE

//---------------------------------------------------------------------------------------
#define	NO_FLAG(f_,v_)	(!(f_&v_))
#define	HAS_FLAG(f_,v_)	(f_&v_)
#define	REM_FLAG(f_,v_)	(f_&=~v_)
#define	ADD_FLAG(f_,v_)	(f_|=v_)
#define SET_FLAG(f_,v_)	(f_=v_)

//---------------------------------------------------------------------------------------
#define DELPA(p_)        {delete[] p_; p_=0;}
#define DELP(p_)         {delete p_; p_=0;}
#define THROW_IF(v_)    if(v_)throw v_

//---------------------------------------------------------------------------------------
typedef long              ERR;
#define HANDELED          1
#define DEFAULT           0
#define SUCCESS           0
#define FAILURE           (-1)
//---------------------------------------------------------------------------------------
typedef ERR (*Proc)(unsigned int, long wP, long lP);
typedef ERR (*Proc1)(unsigned int, long wP);
typedef ERR (*Proc2)(unsigned int);
typedef ERR (*Proc3)(void*);

//---------------------------------------------------------------------------------------
#define EPSILONBIG				0.0625F
#define EPSILON					0.0009765625F 
#define EPSMED                  0.00390625
#define INFINIT					999999999.0
#define PI						3.14159265358900                // PI in radians
#define DOIPI					6.28318530717900	            // 2 pi
#define PIPE2					1.57079632679400                // halfpi
#define PIPE4					0.78539816339700                // quad of  pi
#define PIPE6					0.52359877559800                // sixth of pi
#define ONEDG                   0.0174532925199F                // one grade
#define G2R(a)					(a*0.0174532925F)
#define R2G(a)					(a*57.295779513F)
#define SWITCHI(_x,_y)			((_x)^=(_y)^=(_x)^=(_y))                        // swithes 2 integers
#define ROLLG(a)				{if(a<0) a=360; else if (a > 360) a = 0;}       // roll's the angle
#define ROLLR(a)				{if(a<0) a=DOIPI; else if (a > DOIPI) a = 0.00;}
#define ROLL180(a)				{if(a<-180.00) a=180.00; else if (a > 180.00) a = -180.00;}
#define BLOCKRANGE(a)			if(a < - PIPE2-EPSILON) a =- (PIPE2-EPSILON); else if (a > PIPE2-EPSILON) a = PIPE2-EPSILON;  // blocks the range of angle to straight up/down
#define CLAMPVAL(_v_,m_,M_)     {if(_v_< m_) _v_= m_; else if (_v_> M_)  _v_=M_;}
#define ROLLPI(a)				{if(a < -(PI) ) a = (PI); else if (a > (PI)) a = -(PI);}
#define ROLL2PI(a)				{if(a < 0) a = DOIPI; else if (a > DOIPI) a = 0;}
#define RRND()		            ((REAL)rand()*(1.0f /(REAL)RAND_MAX))  // Floating point random number generator ( 0 -> 1)
#define SGN(a)                  ((a)<0?-1:((a)?1:0))
#define PZERO(t_)                memset(t_,0,sizeof(*t_));
//---------------------------------------------------------------------------------------


template <class T>T tmin(T a, T b)  {return a < b ? a : b;}
template <class T>T tmax(T a, T b)  {return a >= b ? a : b;}

#define FRAND(a,b) (REAL)((REAL)(rand()%a)/b)
#define FRANDPREC(nLow, nHigh) ( ( (1.0f / ( (rand() % 100) + 1) ) * ((nHigh) - (nLow ) ) ) + (nLow) )
#define IRAND(low,high) (rand() % ( (high) - (low) + 1) + (low) )


INLN BOOL IsPowOf2(int value){ // if error here you did not define one of _CFG_**** (see top)
    return (value&(value-1))==0;
}
INLN int ToPow2(int value, int sv=0x1){
    while(value>>=1){
        sv<<=1;
    }
    return sv;
}

INLN REAL Tanr(REAL gr){
    return tanf(gr);
}

INLN REAL Sinr(REAL gr){
    return sinf(gr);
}

INLN REAL Cosr(REAL gr){
    return cosf(gr);
}

INLN REAL Sina(REAL gr){
    ROLLG(gr); return Sinr(gr);
}

INLN REAL Cosa(REAL gr){
    ROLLG(gr); return Cosr(gr);
}

INLN NAKED_ long R2I (REAL f){
#ifdef WINDOWS
    static int i;
    ASM_ fld dword ptr [esp+4]
        ASM_ fistp i
        ASM_ mov eax, i
        ASM_ ret
#else
        return (long)f;
#endif //
}
INLN int Randval(int iMin, int iMax)
{
    return((rand() % (abs(iMax-iMin)+1))+iMin);
}

INLN REAL  Randval(REAL fMin, REAL fMax)
{
    if (fMin == fMax) return(fMin);
    REAL fRandom = (REAL)rand() / (REAL)RAND_MAX;
    return((fRandom * (REAL)fabs(fMax-fMin))+fMin);
}

INLN REAL Rabs (REAL f){
    int i = *(int*) &f;
    i &= 0x7FFFFFFF;
    return *(REAL*)&i;
}

INLN REAL Frand(){

    static unsigned int x = /* QfRandomInt() */ 1234567;
    static unsigned int jflone = 0x40000000;
    static unsigned int jflmask = 0x007fffff;

    x = x * 1664525 + 1013904223;
    unsigned int t = jflone | (jflmask & x);
    return (*(REAL*)&t) - 3.0f;
}

inline BOOL  IsZero(REAL a , REAL b=EPSILONBIG){
    if(a==0)return TRUE;
    return ( a > -b && a < b );
}

INLN REAL Asinr (REAL val) {
    if ( -1.0 < val ) {
        if ( val < 1.0 ) {
            return REAL (asin(val));
        } else {
            return -PIPE2;
        }
    } else {
        return PIPE2;
    }
}

inline REAL ATanr (REAL val) {
    return REAL(atan(val));
}

//---------------------------------------------------------------------------------------
struct V3I{int x,y,z;};
typedef  REAL    AV3[4];
class NO_VT V3
{
public:
    REAL x, y, z;
    V3():x(0.0),y(0.0),z(0.0) {};
    V3(REAL inX, REAL inY, REAL inZ): x(inX), y(inY), z(inZ) {}
    V3(const V3 &v): x(v.x), y(v.y), z(v.z) {}
    V3(const REAL f): x(f), y(f), z(f) {}
    V3(const REAL *f): x(*f), y(*(f+1)), z(*(f+2)) {}
    void RawPaste(REAL* pr){*pr =x;	*(pr+1) = y;*(pr+2) = z;}
    void RawCopy(REAL* pr){	x = *pr;y = *(pr+1);z = *(pr+2);}
    operator REAL*(){return &x;}
    INLN  V3& operator+=(const V3 &v){x += v.x; y += v.y; z += v.z;	return *this;}
    INLN  V3& operator+=(const REAL r){	x += r; y += r; z += r;	return *this;}
    INLN  V3& operator-=(const V3 &v){x -= v.x ;y -= v.y; z -= v.z;return *this;}
    INLN  V3& operator=(const V3 &v){x = v.x ;y = v.y; z = v.z;return *this;}
    //INLN  V3& operator=(const REAL* pf){x=(*pf); y=(*(pf+1)); z=(*(pf+2)); return *this;}
    INLN  void negate(){x=-x;y=-y;z=-z;};
    INLN  V3& operator*=(REAL f) {x *= f;y *= f;z *= f;	return *this;}
    INLN  V3& operator/=(REAL f) {x /= f;y /= f;z /= f;	return *this;}
    INLN  V3& operator/=(int f) {x /= (REAL)f;y /= (REAL)f;z /= (REAL)f;return *this;}
    INLN  V3& operator*=(const V3& v){	x *= v.x;y *= v.y;z *= v.z;return *this;}
    REAL getmin()const{return tmin((tmin(x,y)),z);}
    REAL getmax()const{return tmax((tmax(x,y)),z);}
    INLN void reset(){	memset(this,0,sizeof(*this));}
    INLN void domin(V3& v){	x = tmin(x, v.x);y = tmin(y, v.y);z = tmin(z, v.z);	}
    INLN  void domax(V3& v){x = tmax(x, v.x);y = tmax(y, v.y);z = tmax(z, v.z);	}
    INLN void addscaled(V3& v, REAL d){	x+=v.x*d;	y+=v.y*d;	z+=v.z*d;}
    friend INLN  BOOL operator==(const V3 &a, const V3 &b) {return(IsZero(a.x-b.x) && IsZero(a.y-b.y) &&  IsZero(a.z-b.z));	}
    friend INLN  BOOL operator!=(const V3 &a, const V3 &b) {return((a.x != b.x) || (a.y != b.y) || (a.z != b.z));}
    friend INLN  V3 operator-(const V3 &a){	return V3(-a.x, -a.y, -a.z);}
    friend INLN  V3 operator+(const V3& a, const V3& b) {V3 ret(a);ret += b;return ret;}
    friend INLN  V3 operator-(const V3 &a, const V3 &b) {V3 ret(a); ret -= b; return ret;}
    friend INLN  V3 operator*(const V3 &v, REAL f) {return V3(f * v.x, f * v.y, f * v.z);}
    friend INLN  V3 operator*(REAL f, const V3 &v) {return V3(f * v.x, f * v.y, f * v.z);}
    friend INLN  V3 operator/(const V3 &v, REAL f) {return V3(v.x / f, v.y / f, v.z / f);}
    friend INLN  V3  operator/(const V3 &v, const V3 &f) {return V3(v.x / f.x, v.y / f.y, v.z / f.z);}
    friend INLN  V3  operator*(const V3 &v, const V3 &f) {return V3(v.x * f.x, v.y * f.y, v.z * f.z);}
    friend INLN  REAL vdist(const V3& p1,const  V3& p2){V3 tmp(p1);	tmp-=p2;return tmp.len();}
    friend INLN  REAL vdist2(const V3& p1,const  V3& p2){V3 tmp(p1);	tmp-=p2;	return tmp.len2();}
    INLN  REAL vdp(const V3& b){  return x*b.x + y*b.y + z*b.z; }
    INLN  V3&  vcp(const V3 &b){setval(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);return *this;}
    INLN  void setval(REAL xx, REAL yy, REAL zz) {x = xx; y = yy; z = zz;}
    INLN  void setval(REAL xx[3]) {x = xx[0]; y = xx[1]; z = xx[2];}
    INLN  void squeeze(REAL val)	{if (x > -val && x < val)x = 0.00;if (y > -val && y < val)y = 0.00;if (z > -val && z < val)	z = 0.00;}
    INLN  void setval(REAL xx) {x = xx; y = xx; z = xx;	}
    INLN  BOOL Valid(){	return fabs(x) != INFINIT;}
    INLN  void setval(V3& v) {x = v.x; y = v.y; z = v.z;}
    INLN void interpolate(V3& v1, V3& v2, REAL ammnt=.5f){
        x = v1.x+ (v2.x-v1.x)*ammnt;
        y = v1.y+ (v2.y-v1.y)*ammnt;
        z = v1.z+ (v2.z-v1.z)*ammnt;
    }
    INLN REAL len() const {return(REAL) sqrt(x*x + y*y + z*z);	}
    INLN REAL len2() const{return(x*x + y*y + z*z);}
    INLN BOOL isnot1()const{return (x!=1 || y!=1 || z!=1);	}
    INLN BOOL is0(REAL eps=EPSILON)	const{return Rabs(x) < eps && Rabs(y) < eps && Rabs(z) < eps;}
    BOOL isnot0()const{return Rabs(x)>EPSILON || Rabs(y)>EPSILON || Rabs(z)>EPSILON;}
    INLN BOOL isortonormal()const{
        if (y > .999 || y< -.999) return 1;
        if (x > .999 || x< -.999) return 1;
        if (z > .999 || z< -.999) return 1;
        return 0;
    }
    REAL normalize(){
        REAL m = len();
        if (m > 0.0) m = 1.0 / m;
        else m = 0.0;x *= m;
        y *= m; z *= m;
        return m;
    }
    INLN  V3 &norm(){	REAL m = len();
    if (m > 0.0) m = 1.0 / m;
    else m = 0.0;x *= m;
    y *= m; z *= m;
    return *this;
    }
    INLN void cross(const V3 &v1,const V3 &v2) {
        x = v1.y * v2.z - v1.z * v2.y;
        y = v1.z * v2.x - v1.x * v2.z;
        z = v1.x * v2.y - v1.y * v2.x;
    }
    void SetLength(REAL l)
    {
        REAL len = sqrt(x*x + y*y + z*z);    
        x *= l/len;
        y *= l/len;
        z *= l/len;
    } 
};
//---------------------------------------------------------------------------------------
extern V3 VX;
extern V3 VZ;
extern V3 VY;
extern V3 V0;
extern V3 V1;

//---------------------------------------------------------------------------------------
INLN  BOOL  IsZero(V3& a, REAL e=EPSILONBIG){
    if(a.x<-e || a.x>e)return 0;
    if(a.y<-e || a.y>e)return 0;
    if(a.z<-e || a.z>e)return 0;
    return 1;
}

//---------------------------------------------------------------------------------------
INLN   V3 Vcp(const V3 &a, const V3 &b){
    return V3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

//---------------------------------------------------------------------------------------
INLN  V3 Getnormal(const V3& p0,const  V3& p1,const  V3& p2){
    V3 v = Vcp(p0-p1, p2-p1);
    v.norm();
    return v;
}

//------------------------------------------------------------------------------------
// Normalises the input vector and returns the vector normalised
INLN  V3 Vnrm(const V3 &a)
{
    V3 ret(a.x,a.y,a.z);
    return ret.norm();
}

//---------------------------------------------------------------------------------------
INLN  REAL Vdp(const V3 &a, const V3 &b)
{
    return (REAL)(a.x*b.x + a.y*b.y + a.z*b.z);
}

//---------------------------------------------------------------------------------------
INLN  void Vswp(V3 &a, V3 &b)
{
    V3 tmp(a);    a = b;    b = tmp;
}

//---------------------------------------------------------------------------------------
class NO_VT CLRNOA
{
public:
    CLRNOA():r(255),g(255),b(255){};
    INLN  operator BYTE*(){	return (BYTE*)&r; }
    BYTE r,g,b;
};

//---------------------------------------------------------------------------------------
typedef  BYTE    ACLR[4];
class NO_VT CLR
{
public:
    BYTE r,g,b,a;
    CLR():r(255),g(255),b(255),a(255){};
    CLR(BYTE rgb):r(rgb),g(rgb),b(rgb),a(rgb){};
    CLR(ACLR rgb):r(rgb[0]),g(rgb[1]),b(rgb[2]),a(rgb[3]){};
    CLR(const CLR& rgb):r(rgb.r),g(rgb.g),b(rgb.b),a(rgb.a){}
    CLR(BYTE rr, BYTE gg, BYTE bb, BYTE aa){r = rr;    g = gg;    b = bb;    a = aa;}
    CLR(BYTE rr, BYTE gg, BYTE bb){	r = rr;    g = gg;    b = bb;    a = 255;}
    CLR (DWORD dwclr){ a=255; r= (dwclr>>16)&0xff; g= (dwclr>>8)&0xff; b= (dwclr)&0xff;}
    operator DWORD(){DWORD drv; drv = (r<<24)|(g<<16)|b; return drv;}
    INLN  operator BYTE*(){	return (BYTE*)&r; }
    INLN void black(){	memset(this,0,sizeof(*this));}
    INLN void white(){	memset(this,255,sizeof(*this));}
    INLN void grey(){	memset(this,128,sizeof(*this));}
    INLN void lgrey(){	memset(this,192,sizeof(*this));}
    INLN CLR& operator =(DWORD dwclr){ a=255; r= (dwclr>>24)&0xf; g= (dwclr>>16)&0xf; b= (dwclr>>8)&0xf; return *this;}
    INLN CLR& operator =(const CLR& rr){	r = rr.r;    g = rr.g;    b = rr.b;    a = rr.a;	return *this;}
    INLN CLR& operator =(const CLRNOA& rr){	r = rr.r;    g = rr.g;    b = rr.b;    a = 255;	return *this;}
    INLN  CLR& operator -=(CLR& rgb){r-=rgb.r;g-=rgb.g;b-=rgb.b;return *this;}
    BOOL  isnot0(){	if(r!=0)return TRUE;if(g!=0)return TRUE;if(b!=0)return TRUE;return FALSE;}
    INLN  CLR& operator +=(CLR& rgb){r+=rgb.r; g+=rgb.g; b+=rgb.b; return *this;}
    INLN  CLR& operator +=(BYTE rgb){r+=rgb; g+=rgb; b+=rgb; return *this;}
    INLN  CLR& operator *=(CLR& rgb){r*=rgb.r; g*=rgb.g; b*=rgb.b; return *this;}
    INLN  void setval(BYTE rr, BYTE gg, BYTE bb){r=rr;g=gg;b=bb;a=255;}
    INLN  void setval(const BYTE* by){r=by[0];g=by[1];b=by[2];a=by[3];}
    INLN  int Sum()const{return (r+g+b);}
    INLN  CLR& operator *=(BYTE f) {r *= f;g *= f; b *= f;return *this;}
    friend INLN  CLR  operator-(CLR& a, CLR& b ){CLR vr(a);vr-=b;return vr;}
    friend INLN  CLR  operator+(CLR& a, CLR& b ){CLR vr(a);vr+=b;return vr;}
    friend INLN  CLR operator / (const CLR& v, BYTE f) {return CLR(v.r / f, v.g / f, v.b / f);}
    INLN void operator =(BYTE f){r = f; g = f;  b = f;  a = f;}
    INLN BOOL operator !=(CLR& vrgb){return (r!=vrgb.r || g!=vrgb.g || b!=vrgb.b || a!=vrgb.a);}
    INLN BOOL operator ==(const CLR& vrgb){return (r==vrgb.r && g==vrgb.g && b==vrgb.b && a==vrgb.a);}

    INLN  void interpolate(const CLR& c1, const CLR& c2, REAL ammnt=.5f){
        r = c1.r + (c2.r-c1.r) * ammnt;
        g = c1.g + (c2.g-c1.g) * ammnt;
        b = c1.b + (c2.b-c1.b) * ammnt;
        a = c1.a + (c2.a-c1.a) * ammnt;
    }
    INLN  void ClampUp(CLR& cc){if (r>cc.r)r=cc.r;if (g>cc.g)g=cc.g;if (b>cc.b)b=cc.b;}
    INLN  void Scale(CLR& clr){r/=clr.r;g/=clr.g;b/=clr.b;}

};

//---------------------------------------------------------------------------------------
extern CLR ZBLACK;
extern CLR ZGREY;
extern CLR ZLIGHTGRAY;
extern CLR ZMEDIUMGRAY;
extern CLR ZRED;
extern CLR ZGREEN;
extern CLR ZYELLOW;
extern CLR ZBLUE;
extern CLR ZWHITE;
extern CLR ZCYAN;
extern CLR ZREDLIGHT;
extern CLR ZBLUELIGHT;
extern CLR ZGREENLIGHT;

struct UVI
{
    int u;
    int v;
};
//---------------------------------------------------------------------------------------
typedef  REAL  AUV[2];
struct UV
{
    REAL u,v;
    UV():u(0.00),v(0.00) {};
    UV(REAL inX, REAL inY): u(inX), v(inY){}
    UV(const UV &v): u(v.u), v(v.v) {}
    UV(const REAL f): u(f), v(f) {}
    UV(const REAL *f): u(*f), v(*(f+1)) {}
    INLN  operator REAL*(){return (REAL*)&u;}
    INLN  UV& operator += (const UV &ufval) {u += ufval.u; v += ufval.v;	return *this;}
    INLN  UV& operator -= (const UV &ufval) {u -= ufval.u; v -= ufval.v;	return *this;}
    INLN  UV& operator *= (REAL f) {u *= f; v *= f;	return *this;}
    INLN  UV& operator *= (UV f) {u *= f.u; v *= f.v;	return *this;}
    INLN  UV& operator /= (REAL f) {u /= f; v /= f;	return *this;}
    INLN  UV& operator /= (UV& uv) {u /= uv.u; v /= uv.v;	return *this;}
    INLN  void setval(REAL xx) {u = xx; v = xx;}
    INLN void reset(){	memset(this,0,sizeof(*this));}
    INLN  void setval(REAL uu, REAL vv) {u = uu; v = vv;}
    INLN  void setval(UV& xx) {u = xx.u; v = xx.v;}
    INLN  void oneover() {u = 1.00/u; v = 1.00/v;}
    INLN  void domin(UV& t){u = tmin(u,t.u);v = tmin(v,t.v);}
    INLN void domax(UV& t){u = tmax(u,t.u);v = tmax(v,t.v);}
    INLN void interpolate(UV& t1, UV& t2, REAL ammnt=.5f){u = t1.u + (t2.u-t1.u)*ammnt;v = t1.v + (t2.v-t1.v)*ammnt;}
    friend INLN  BOOL operator == (const UV &a, const UV &b) {return((a.u == b.u) && (a.v == b.v));}
    friend INLN  BOOL operator != (const UV &a, const UV &b) {return((a.u != b.u) || (a.v != b.v));}
    friend INLN  UV  operator - (const UV &a) {return UV(-a.u, -a.v);}
    friend INLN  UV  operator + (const UV &a, const UV &b) {UV ret(a);ret += b;return ret;}
    friend INLN  UV  operator - (const UV &a, const UV &b) {UV ret(a);ret -= b;return ret;}
    friend INLN  UV operator * (const UV &v, REAL f) {return UV(f * v.u, f * v.v);}
    friend INLN  UV operator * (REAL f, const UV &v) {return UV(f * v.u, f * v.v);}
    friend INLN  UV operator / (const UV &v, REAL f) {return UV(v.u / f, v.v / f);}
    friend INLN  UV  operator / (const UV &v, const UV &f) {return UV(v.u / f.u, v.v / f.v);}
    friend INLN  UV  operator * (const UV &v, const UV &f) {return UV(v.u * f.u, v.v * f.v);}
};

//---------------------------------------------------------------------------------------
typedef  REAL  AVtx[16];
struct Vtx
{
    V3		_xyz;    // 12 ...
    V3		_nrm;    // 12 ...
    UV		_uv[4];	 // 32 .. .. .. ..
    CLR		_rgb;	 // 4  .
    long	_user;	 // 4 .

    Vtx():_user(0){}
    Vtx(const V3& pct,const UV& ufval){	_xyz = pct; 	_uv[0]	= ufval;	_rgb	= ZWHITE;  _user	= 0;}
    Vtx(const V3& pct, const UV& ufval,const  CLR& rgb, long c ){_xyz = pct;	_uv[0]	= ufval; _rgb= rgb;	_user	= c;}
    Vtx(const AVtx& r){memcpy(this,&r,sizeof(*this));}
    Vtx(const Vtx& r)	{
        _xyz = r._xyz;
        _nrm = r._nrm;
        _uv[0] = r._uv[0];
        _uv[1] = r._uv[1];
        _uv[2] = r._uv[2];
        _uv[3] = r._uv[3];
        _rgb   = r._rgb  ;
        _user  = r._user ;	 }
    INLN  Vtx& operator = (const Vtx& r){
        if(this != & r){
            _xyz = r._xyz;
            _nrm = r._nrm;
            _uv[0] = r._uv[0];
            _uv[1] = r._uv[1];
            _uv[2] = r._uv[2];
            _uv[3] = r._uv[3];
            _rgb   = r._rgb  ;
            _user  = r._user ;
        }
        return *this;
    }
    INLN void reset(){	memset(this,0,sizeof(*this));}
    void interpolate(Vtx& v1, Vtx& v2, REAL ammnt=.5f){
        _user	= v1._user;
        _xyz.interpolate(v1._xyz, v2._xyz, ammnt);
        _nrm.interpolate(v1._xyz, v2._xyz, ammnt);
        _uv[0].interpolate(v1._uv[0],v2._uv[0],ammnt);
        _uv[1].interpolate(v1._uv[1],v2._uv[1],ammnt);
        _uv[2].interpolate(v1._uv[1],v2._uv[1],ammnt);
        _uv[3].interpolate(v1._uv[1],v2._uv[1],ammnt);
        _rgb = v1._rgb;
    }
    BOOL operator ==(const Vtx& r){return _xyz	== r._xyz;}
    int size(){return sizeof(*this);}
};//  64 bytes (pow of 2) should stay like this


typedef  REAL  AM4[16];

class NO_VT M4
{
public:
    REAL   _m[16];

public:
    M4()     {identity();};
    M4(V3 v) {translate(v);};
    M4(const V3 &axis,REAL angle)              {rotate(axis,angle);}
    INLN  M4(const M4 &m) {
        _m[0] = m._m[0]; _m[4] = m._m[4]; _m[8] = m._m[8]; _m[12] = m._m[12];
        _m[1] = m._m[1]; _m[5] = m._m[5]; _m[9] = m._m[9]; _m[13] = m._m[13];
        _m[2] = m._m[2]; _m[6] = m._m[6]; _m[10] = m._m[10]; _m[14] = m._m[14];
        _m[3] = m._m[3]; _m[7] = m._m[7]; _m[11] = m._m[11]; _m[15] = m._m[15];
    }
    M4& operator=(const M4& m) {
        if(this != &m){
            _m[0] = m._m[0]; _m[4] = m._m[4]; _m[8] = m._m[8]; _m[12] = m._m[12];
            _m[1] = m._m[1]; _m[5] = m._m[5]; _m[9] = m._m[9]; _m[13] = m._m[13];
            _m[2] = m._m[2]; _m[6] = m._m[6]; _m[10] = m._m[10]; _m[14] = m._m[14];
            _m[3] = m._m[3]; _m[7] = m._m[7]; _m[11] = m._m[11]; _m[15] = m._m[15];
        }
        return *this;
    }
    operator REAL*() { return (REAL*)_m; }
    operator const REAL*() const { return (REAL*)_m; }
    M4 operator+(const M4 &m) const
    {
        M4 ret;
        ret[0] = _m[0] + m[0]; ret[4] = _m[4] + m[4]; ret[8] = _m[8] + m[8]; ret[12] = _m[12] + m[12];
        ret[1] = _m[1] + m[1]; ret[5] = _m[5] + m[5]; ret[9] = _m[9] + m[9]; ret[13] = _m[13] + m[13];
        ret[2] = _m[2] + m[2]; ret[6] = _m[6] + m[6]; ret[10] = _m[10] + m[10]; ret[14] = _m[14] + m[14];
        ret[3] = _m[3] + m[3]; ret[7] = _m[7] + m[7]; ret[11] = _m[11] + m[11]; ret[15] = _m[15] + m[15];
        return ret;
    }
    V3 operator*(const V3 &v) const {
        V3 ret;
        ret[0] = _m[0] * v.x + _m[4] * v.y + _m[8] * v.z + _m[12];
        ret[1] = _m[1] * v.x + _m[5] * v.y + _m[9] * v.z + _m[13];
        ret[2] = _m[2] * v.x + _m[6] * v.y + _m[10] * v.z + _m[14];
        return ret;
    }
    M4 operator*(REAL f) const {
        M4 ret;
        ret._m[0] = _m[0] * f; ret._m[4] = _m[4] * f; ret._m[8] = _m[8] * f; ret._m[12] = _m[12] * f;
        ret._m[1] = _m[1] * f; ret._m[5] = _m[5] * f; ret._m[9] = _m[9] * f; ret._m[13] = _m[13] * f;
        ret._m[2] = _m[2] * f; ret._m[6] = _m[6] * f; ret._m[10] = _m[10] * f; ret._m[14] = _m[14] * f;
        ret._m[3] = _m[3] * f; ret._m[7] = _m[7] * f; ret._m[11] = _m[11] * f; ret._m[15] = _m[15] * f;
        return ret;
    }
    M4 operator*(const M4 &m) const {
        M4 ret;
        ret._m[0] = _m[0] * m._m[0] + _m[4] * m._m[1] + _m[8] * m._m[2] + _m[12] * m._m[3];
        ret._m[1] = _m[1] * m._m[0] + _m[5] * m._m[1] + _m[9] * m._m[2] + _m[13] * m._m[3];
        ret._m[2] = _m[2] * m._m[0] + _m[6] * m._m[1] + _m[10] * m._m[2] + _m[14] * m._m[3];
        ret._m[3] = _m[3] * m._m[0] + _m[7] * m._m[1] + _m[11] * m._m[2] + _m[15] * m._m[3];
        ret._m[4] = _m[0] * m._m[4] + _m[4] * m._m[5] + _m[8] * m._m[6] + _m[12] * m._m[7];
        ret._m[5] = _m[1] * m._m[4] + _m[5] * m._m[5] + _m[9] * m._m[6] + _m[13] * m._m[7];
        ret._m[6] = _m[2] * m._m[4] + _m[6] * m._m[5] + _m[10] * m._m[6] + _m[14] * m._m[7];
        ret._m[7] = _m[3] * m._m[4] + _m[7] * m._m[5] + _m[11] * m._m[6] + _m[15] * m._m[7];
        ret._m[8] = _m[0] * m._m[8] + _m[4] * m._m[9] + _m[8] * m._m[10] + _m[12] * m._m[11];
        ret._m[9] = _m[1] * m._m[8] + _m[5] * m._m[9] + _m[9] * m._m[10] + _m[13] * m._m[11];
        ret._m[10] = _m[2] * m._m[8] + _m[6] * m._m[9] + _m[10] * m._m[10] + _m[14] * m._m[11];
        ret._m[11] = _m[3] * m._m[8] + _m[7] * m._m[9] + _m[11] * m._m[10] + _m[15] * m._m[11];
        ret._m[12] = _m[0] * m._m[12] + _m[4] * m._m[13] + _m[8] * m._m[14] + _m[12] * m._m[15];
        ret._m[13] = _m[1] * m._m[12] + _m[5] * m._m[13] + _m[9] * m._m[14] + _m[13] * m._m[15];
        ret._m[14] = _m[2] * m._m[12] + _m[6] * m._m[13] + _m[10] * m._m[14] + _m[14] * m._m[15];
        ret._m[15] = _m[3] * m._m[12] + _m[7] * m._m[13] + _m[11] * m._m[14] + _m[15] * m._m[15];
        return ret;
    }

    M4 &operator*=(REAL f)     { return *this = *this * f; }
    M4 &operator*=(const M4 &m) { return *this = *this * m; }
    M4 &operator+=(const M4 &m) { return *this = *this + m; }

    void orthonormalize() {
        V3 x(_m[0],_m[1],_m[2]);
        V3 y(_m[3],_m[4],_m[5]);
        V3 z;
        x.norm();
        z.cross(x,y);
        z.norm();
        y.cross(z,x);
        y.norm();
        _m[0] = x.x; _m[3] = y.x; _m[6] = z.x;
        _m[1] = x.y; _m[4] = y.y; _m[7] = z.y;
        _m[2] = x.z; _m[5] = y.z; _m[8] = z.z;
    }

    INLN void v3transposeTransform(V3& i)
    {
        V3 r;
        i.x -= _m[12];
        i.y -= _m[13];
        i.z -= _m[14];

        r.x = i.x * _m[0] + i.y * _m[1] + i.z * _m[2] ;
        r.y = i.x * _m[4] + i.y * _m[5] + i.z * _m[6] ;
        r.z = i.x * _m[8] + i.y * _m[9] + i.z * _m[10] ;
        i=r;
    }

    INLN void v3transform(V3& v)
    {
        v3translate(v);
        v3rotate(v);
        /*
        V3 r;
        r.x = v.x * _m[0] + v.y * _m[4] + v.z * _m[8]  + _m[12];
        r.y = v.x * _m[1] + v.y * _m[5] + v.z * _m[9]  + _m[13];
        r.z = v.x * _m[2] + v.y * _m[6] + v.z * _m[10] + _m[14];
        v=r;
        */
    }
    INLN void v3rotate(V3& v)
    {
        V3 r;
        r.x = v.x * _m[0] + v.y * _m[4] + v.z * _m[8] ;
        r.y = v.x * _m[1] + v.y * _m[5] + v.z * _m[9] ;
        r.z = v.x * _m[2] + v.y * _m[6] + v.z * _m[10] ;
        v=r;
    }

    INLN void v3translate(V3& ret)
    {
        ret.x = ret.x + _m[12];
        ret.y = ret.y + _m[13];
        ret.z = ret.z + _m[14];
    }

    INLN void v3scale(V3& ret)
    {
        ret.x = ret.x * _m[0];
        ret.y = ret.y * _m[5];
        ret.z = ret.z * _m[10];
    }

    INLN void identity() {
        _m[0] = 1.0; _m[4] = 0.0; _m[8] = 0.0; _m[12] = 0.0;
        _m[1] = 0.0; _m[5] = 1.0; _m[9] = 0.0; _m[13] = 0.0;
        _m[2] = 0.0; _m[6] = 0.0; _m[10] = 1.0; _m[14] = 0.0;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }

    INLN void transp(){
        M4 ret;
        ret._m[0] = _m[0]; ret._m[4] = _m[1]; ret._m[8] = _m[2]; ret._m[12] = _m[3];
        ret._m[1] = _m[4]; ret._m[5] = _m[5]; ret._m[9] = _m[6]; ret._m[13] = _m[7];
        ret._m[2] = _m[8]; ret._m[6] = _m[9]; ret._m[10] = _m[10]; ret._m[14] = _m[11];
        ret._m[3] = _m[12]; ret._m[7] = _m[13]; ret._m[11] = _m[14]; ret._m[15] = _m[15];
        *this = ret;
    }

    REAL det() const {
        REAL det;
        det = _m[0] * _m[5] * _m[10];
        det += _m[4] * _m[9] * _m[2];
        det += _m[8] * _m[1] * _m[6];
        det -= _m[8] * _m[5] * _m[2];
        det -= _m[4] * _m[1] * _m[10];
        det -= _m[0] * _m[9] * _m[6];
        return det;
    }
    M4 inverse() const {
        M4 ret;
        REAL idet = 1.0f / det();
        ret._m[0] =  (_m[5] * _m[10] - _m[9] * _m[6]) * idet;
        ret._m[1] = -(_m[1] * _m[10] - _m[9] * _m[2]) * idet;
        ret._m[2] =  (_m[1] * _m[6] - _m[5] * _m[2]) * idet;
        ret._m[3] = 0.0;
        ret._m[4] = -(_m[4] * _m[10] - _m[8] * _m[6]) * idet;
        ret._m[5] =  (_m[0] * _m[10] - _m[8] * _m[2]) * idet;
        ret._m[6] = -(_m[0] * _m[6] - _m[4] * _m[2]) * idet;
        ret._m[7] = 0.0;
        ret._m[8] =  (_m[4] * _m[9] - _m[8] * _m[5]) * idet;
        ret._m[9] = -(_m[0] * _m[9] - _m[8] * _m[1]) * idet;
        ret._m[10] =  (_m[0] * _m[5] - _m[4] * _m[1]) * idet;
        ret._m[11] = 0.0;
        ret._m[12] = -(_m[12] * ret._m[0] + _m[13] * ret._m[4] + _m[14] * ret._m[8]);
        ret._m[13] = -(_m[12] * ret._m[1] + _m[13] * ret._m[5] + _m[14] * ret._m[9]);
        ret._m[14] = -(_m[12] * ret._m[2] + _m[13] * ret._m[6] + _m[14] * ret._m[10]);
        ret._m[15] = 1.0;
        return ret;
    }

    void rotate(const V3 &axis, REAL angle) {
        REAL rad = G2R(angle);
        REAL c = Cosr(rad);
        REAL s = Sinr(rad);
        V3 v = axis;
        v.norm();
        REAL xx = v.x * v.x;
        REAL yy = v.y * v.y;
        REAL zz = v.z * v.z;
        REAL xy = v.x * v.y;
        REAL yz = v.y * v.z;
        REAL zx = v.z * v.x;
        REAL xs = v.x * s;
        REAL ys = v.y * s;
        REAL zs = v.z * s;
        _m[0] = (1.0f - c) * xx + c; _m[4] = (1.0f - c) * xy - zs; _m[8] = (1.0f - c) * zx + ys; _m[12] = 0.0;
        _m[1] = (1.0f - c) * xy + zs; _m[5] = (1.0f - c) * yy + c; _m[9] = (1.0f - c) * yz - xs; _m[13] = 0.0;
        _m[2] = (1.0f - c) * zx - ys; _m[6] = (1.0f - c) * yz + xs; _m[10] = (1.0f - c) * zz + c; _m[14] = 0.0;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }
    void rotatex(REAL angle) {
        REAL rad = G2R(angle);
        REAL c = Cosr(rad);
        REAL s = Sinr(rad);
        _m[0] = 1.0; _m[4] = 0.0; _m[8] = 0.0; _m[12] = 0.0;
        _m[1] = 0.0; _m[5] = c; _m[9] = -s; _m[13] = 0.0;
        _m[2] = 0.0; _m[6] = s; _m[10] = c; _m[14] = 0.0;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }
    void rotatey(REAL angle) {
        REAL rad = G2R(angle);
        REAL c = Cosr(rad);
        REAL s = Sinr(rad);
        _m[0] = c; _m[4] = 0.0; _m[8] = s; _m[12] = 0.0;
        _m[1] = 0.0; _m[5] = 1.0; _m[9] = 0.0; _m[13] = 0.0;
        _m[2] = -s; _m[6] = 0.0; _m[10] = c; _m[14] = 0.0;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }
    void rotatez(REAL angle) {
        REAL rad = G2R(angle);
        REAL c = Cosr(rad);
        REAL s = Sinr(rad);
        _m[0] = c; _m[4] = -s; _m[8] = 0.0; _m[12] = 0.0;
        _m[1] = s; _m[5] = c; _m[9] = 0.0; _m[13] = 0.0;
        _m[2] = 0.0; _m[6] = 0.0; _m[10] = 1.0; _m[14] = 0.0;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }
    void scale(const V3 &v) {
        _m[0] = v.x; _m[4] = 0.0; _m[8] = 0.0; _m[12] = 0.0;
        _m[1] = 0.0; _m[5] = v.y; _m[9] = 0.0; _m[13] = 0.0;
        _m[2] = 0.0; _m[6] = 0.0; _m[10] = v.z; _m[14] = 0.0;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }
    void translate(const V3 &v) {
        _m[0] = 1.0; _m[4] = 0.0; _m[8] = 0.0; _m[12] = v.x;
        _m[1] = 0.0; _m[5] = 1.0; _m[9] = 0.0; _m[13] = v.y;
        _m[2] = 0.0; _m[6] = 0.0; _m[10] = 1.0; _m[14] = v.z;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }
    void reflect(const V3 &norm, REAL c) {
        REAL x = norm.x;
        REAL y = norm.y;
        REAL z = norm.z;
        REAL x2 = x * 2.0f;
        REAL y2 = y * 2.0f;
        REAL z2 = z * 2.0f;
        _m[0] = 1.0f - x * x2; _m[4] = -y * x2; _m[8] = -z * x2; _m[12] = -c * x2;
        _m[1] = -x * y2; _m[5] = 1.0f - y * y2; _m[9] = -z * y2; _m[13] = -c * y2;
        _m[2] = -x * z2; _m[6] = -y * z2; _m[10] = 1.0f - z * z2; _m[14] = -c * z2;
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = 0.0; _m[15] = 1.0;
    }

    void perspective(REAL fov,REAL aspect,REAL znear,REAL zfar) {
        REAL y = tan(fov * PI / 360.0f);
        REAL x = y * aspect;
        _m[0] = 1.0f / x; _m[4] = 0.0; _m[8] = 0.0; _m[12] = 0.0;
        _m[1] = 0.0; _m[5] = 1.0f / y; _m[9] = 0.0; _m[13] = 0.0;
        _m[2] = 0.0; _m[6] = 0.0; _m[10] = -(zfar + znear) / (zfar - znear); _m[14] = -(2.0f * zfar * znear) / (zfar - znear);
        _m[3] = 0.0; _m[7] = 0.0; _m[11] = -1.0; _m[15] = 0.0;
    }
    void look_to(const V3 &eye,const V3 &at,const V3 &up) {
        V3 x,y,z;
        M4 m0,m1;
        z = eye - at;
        z.norm();
        x.cross(up,z);
        x.norm();
        y.cross(z,x);
        y.norm();
        m0._m[0] = x.x; m0._m[4] = x.y; m0._m[8] = x.z; m0._m[12] = 0.0;
        m0._m[1] = y.x; m0._m[5] = y.y; m0._m[9] = y.z; m0._m[13] = 0.0;
        m0._m[2] = z.x; m0._m[6] = z.y; m0._m[10] = z.z; m0._m[14] = 0.0;
        m0._m[3] = 0.0; m0._m[7] = 0.0; m0._m[11] = 0.0; m0._m[15] = 1.0;
        m1.translate(-eye);
        *this = m0 * m1;
    }
    M4 rotation() const {
        M4 ret;
        ret[0] = _m[0]; ret[4] = _m[4]; ret[8] = _m[8]; ret[12] = 0;
        ret[1] = _m[1]; ret[5] = _m[5]; ret[9] = _m[9]; ret[13] = 0;
        ret[2] = _m[2]; ret[6] = _m[6]; ret[10] = _m[10]; ret[14] = 0;
        ret[3] = 0; ret[7] = 0; ret[11] = 0; ret[15] = 1;
        return ret;
    }
    M4 transpose() const {
        M4 ret;
        ret[0] = _m[0]; ret[4] = _m[1]; ret[8] = _m[2]; ret[12] = _m[3];
        ret[1] = _m[4]; ret[5] = _m[5]; ret[9] = _m[6]; ret[13] = _m[7];
        ret[2] = _m[8]; ret[6] = _m[9]; ret[10] = _m[10]; ret[14] = _m[11];
        ret[3] = _m[12]; ret[7] = _m[13]; ret[11] = _m[14]; ret[15] = _m[15];
        return ret;
    }
    M4 transpose_rotation() const {
        M4 ret;
        ret[0] = _m[0]; ret[4] = _m[1]; ret[8] = _m[2]; ret[12] = _m[12];
        ret[1] = _m[4]; ret[5] = _m[5]; ret[9] = _m[6]; ret[13] = _m[13];
        ret[2] = _m[8]; ret[6] = _m[9]; ret[10] = _m[10]; ret[14] = _m[14];
        ret[3] = _m[3]; ret[7] = _m[7]; ret[14] = _m[14]; ret[15] = _m[15];
        return ret;
    }

    void rotateradians(REAL dAngleX, REAL dAngleY, REAL dAngleZ)
    {
	    REAL cr = cos( dAngleX );
	    REAL sr = sin( dAngleX );
	    REAL cp = cos( dAngleY );
	    REAL sp = sin( dAngleY );
	    REAL cy = cos( dAngleZ );
	    REAL sy = sin( dAngleZ );

	    _m[0] = ( REAL )( cp*cy );
	    _m[1] = ( REAL )( cp*sy );
	    _m[2] = ( REAL )( -sp );

	    REAL srsp = sr*sp;
	    REAL crsp = cr*sp;

	    _m[4] = ( REAL )( srsp*cy-cr*sy );
	    _m[5] = ( REAL )( srsp*sy+cr*cy );
	    _m[6] = ( REAL )( sr*cp );

	    _m[8] = ( REAL )( crsp*cy+sr*sy );
	    _m[9] = ( REAL )( crsp*sy-sr*cy );
	    _m[10] = ( REAL )( cr*cp );

    }

    void multiply (M4 *matrix)
    {
	    float newMatrix[16];
	    const float *m1 = _m, *m2 = matrix->_m;

	    newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
	    newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
	    newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
	    newMatrix[3] = 0;

	    newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
	    newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
	    newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
	    newMatrix[7] = 0;

	    newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
	    newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
	    newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
	    newMatrix[11] = 0;

	    newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
	    newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
	    newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
	    newMatrix[15] = 1;

	    for (int i=0; i<16; i++)
		    this->_m[i] = newMatrix[i];
    }
	void  inversetranslatevect(V3& vct)
	{
		vct.x = vct.x-_m[12];
		vct.y = vct.y-_m[13];
		vct.z = vct.z-_m[14];
	}

	void inverserotatevect(V3&  vct)
	{
		V3 vec(vct.x*_m[0]+vct.y*_m[1]+vct.z*_m[2],
		       vct.x*_m[4]+vct.y*_m[5]+vct.z*_m[6],
		       vct.x*_m[8]+vct.y*_m[9]+vct.z*_m[10]);

		vct = vec;
	}

};

class NO_VT Quat
{
public:

    Quat() : x(0), y(0), z(0), w(1) { }
    Quat(const V3 &dir,REAL angle) {
        set(dir,angle);
    }
    Quat(REAL x,REAL y,REAL z,REAL angle) {
        set(x,y,z,angle);
    }
    Quat(const M4 &m) {
        REAL trace = m[0] + m[4] + m[8];
        if(trace > 0.0) {
            REAL s = sqrt(trace + 1.0f);
            q[3] = 0.5f * s;
            s = 0.5f / s;
            q[0] = (m[5] - m[7]) * s;
            q[1] = (m[6] - m[2]) * s;
            q[2] = (m[1] - m[3]) * s;
        } else {
            static int next[3] = { 1, 2, 0 };
            int i = 0;
            if(m[4] > m[0]) i = 1;
            if(m[8] > m[3 * i + i]) i = 2;
            int j = next[i];
            int k = next[j];
            REAL s = sqrt(m[3 * i + i] - m[3 * j + j] - m[3 * k + k] + 1.0f);
            q[i] = 0.5f * s;
            if(s != 0) s = 0.5f / s;
            q[3] = (m[3 * j + k] - m[3 * k + j]) * s;
            q[j] = (m[3 * i + j] + m[3 * j + i]) * s;
            q[k] = (m[3 * i + k] + m[3 * k + i]) * s;
        }
    }
    Quat& operator =(const Quat& q){x=q.x ;y=q.y ;z=q.z ;w=q.w ; return *this;}
    operator REAL*() { return (REAL*)&x; }
    operator const REAL*() const { return (REAL*)&x; }

    REAL &operator[](int i) { return ((REAL*)&x)[i]; }
    const REAL operator[](int i) const { return ((REAL*)&x)[i]; }

    Quat operator*(const Quat &q) const {
        Quat ret;
        ret.x = w * q.x + x * q.x + y * q.z - z * q.y;
        ret.y = w * q.y + y * q.w + z * q.x - x * q.z;
        ret.z = w * q.z + z * q.w + x * q.y - y * q.x;
        ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
        return ret;
    }

    void set(const V3 &dir,REAL angle) {
        REAL length = dir.len();
        if(length != 0.0) {
            length = 1.0f / length;
            REAL sinangle = Sinr(G2R(angle)/ 2.0f);
            x = dir.x * length * sinangle;
            y = dir.y * length * sinangle;
            z = dir.z * length * sinangle;
            w = Cosr(G2R(angle)/ 2.0f);
        } else {
            x = y = z = 0.0;
            w = 1.0;
        }
    }
    void set(REAL x,REAL y,REAL z,REAL angle) {
        set(V3(x,y,z),angle);
    }

    void slerp(const Quat &q0,const Quat &q1,REAL t) {
        REAL k0,k1,cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
        Quat q;
        if(cosomega < 0.0) {
            cosomega = -cosomega;
            q.x = -q1.x;
            q.y = -q1.y;
            q.z = -q1.z;
            q.w = -q1.w;
        } else {
            q.x = q1.x;
            q.y = q1.y;
            q.z = q1.z;
            q.w = q1.w;
        }
        if(1.0 - cosomega > 1e-6) {
            REAL omega = acos(cosomega);
            REAL sinomega = Sinr(omega);
            k0 = Sinr((1.0f - t) * omega) / sinomega;
            k1 = Sinr(t * omega) / sinomega;
        } else {
            k0 = 1.0f - t;
            k1 = t;
        }
        x = q0.x * k0 + q.x * k1;
        y = q0.y * k0 + q.y * k1;
        z = q0.z * k0 + q.z * k1;
        w = q0.w * k0 + q.w * k1;
    }

    M4 to_matrix() const {
        M4 ret;
        REAL x2 = x + x;
        REAL y2 = y + y;
        REAL z2 = z + z;
        REAL xx = x * x2;
        REAL yy = y * y2;
        REAL zz = z * z2;
        REAL xy = x * y2;
        REAL yz = y * z2;
        REAL xz = z * x2;
        REAL wx = w * x2;
        REAL wy = w * y2;
        REAL wz = w * z2;
        ret[0] = 1.0f - (yy + zz); ret[3] = xy - wz; ret[6] = xz + wy;
        ret[1] = xy + wz; ret[4] = 1.0f - (xx + zz); ret[7] = yz - wx;
        ret[2] = xz - wy; ret[5] = yz + wx; ret[8] = 1.0f - (xx + yy);
        return ret;
    }

    union {
        struct {
            REAL x,y,z,w;
        };
        REAL q[4];
    };
};


//---------------------------------------------------------------------------------------
inline  M4 MDirsRotate(V3& f, V3& u, V3& s)
{
    M4 mret;
    mret._m[8]=f.x;mret._m[9]=f.y;mret._m[10]=f.x;
    mret._m[4]=u.x;mret._m[5]=u.y;mret._m[6]=u.x;
    mret._m[0]=s.x;mret._m[1]=s.y;mret._m[2]=s.z;
    return mret;
}

//---------------------------------------------------------------------------------------
inline  M4 MEulerRotate(REAL a, REAL b, REAL c)
{
    M4 mret;

    REAL sa = Sinr(a);
    REAL sb = Sinr(b);
    REAL sy = Sinr(c);
    REAL ca = Cosr(a);
    REAL cb = Cosr(b);
    REAL cy = Cosr(c);

    mret._m[0] = ca*cb*cy-sa*sy; mret._m[1] = -ca*cb*sy-sa*cy; mret._m[2] = ca*sy;
    mret._m[4] = sa*cb*cy-ca*sy; mret._m[5] = -sa*cb*sy+ca*cy; mret._m[6] = sa*sy;
    mret._m[8] = -sb*cy;         mret._m[9] = sb*sy;           mret._m[10] = cb;
    return mret;
}


//---------------------------------------------------------------------------------------
inline M4 MRadRotate(REAL radx, REAL rady, REAL radz)
{
    REAL sinA, cosA;
    M4	 mret;

    if(radz != 0.0)
    {
        M4		mrz;
        sinA = (REAL)Sinr(radz);
        cosA = (REAL)Cosr(radz);
        mrz._m[0] =  cosA; mrz._m[4] = -sinA; mrz._m[8] =  0.0;
        mrz._m[1] =  sinA; mrz._m[5] =  cosA; mrz._m[9] =  0.0;
        mret*=mrz;
    }
    if(rady != 0.0)
    {
        M4		mry;
        sinA = (REAL)Sinr(rady);
        cosA = (REAL)Cosr(rady);
        mry._m[0] =  cosA;  mry._m[8] =  sinA;
        mry._m[2] = -sinA;  mry._m[10] =  cosA;
        mret*=mry;
    }

    if(radx != 0.0)
    {
        M4		mrx;
        sinA = (REAL)Sinr(radx);
        cosA = (REAL)Cosr(radx);
        mrx._m[5] =  cosA;  mrx._m[9] = -sinA;
        mrx._m[6] =  sinA;  mrx._m[10] =  cosA;
        mret*=mrx;
    }
    return mret;
}


//---------------------------------------------------------------------------------------
inline M4 MRotAround(const V3 &axis, REAL rad)
{
    V3 nrm = axis;
    M4 mret;
    nrm.norm();
    REAL sinA = (REAL)Sinr(rad);
    REAL cosA = (REAL)Cosr(rad);
    REAL invCosA = 1.0 - cosA;
    REAL x = nrm.x;
    REAL y = nrm.y;
    REAL z = nrm.z;
    REAL xSq = x * x;
    REAL ySq = y * y;
    REAL zSq = z * z;

    mret._m[0] = (invCosA * xSq) + (cosA);
    mret._m[4] = (invCosA * x * y) - (sinA * z );
    mret._m[8] = (invCosA * x * z) + (sinA * y );
    mret._m[1] = (invCosA * x * y) + (sinA * z);
    mret._m[5] = (invCosA * ySq) + (cosA);
    mret._m[9] = (invCosA * y * z) - (sinA * x);
    mret._m[2] = (invCosA * x * z) - (sinA * y);
    mret._m[6] = (invCosA * y * z) + (sinA * x);
    mret._m[10] = (invCosA * zSq) + (cosA);
    return mret;

}

//---------------------------------------------------------------------------------------
inline  M4 MTranslate(REAL x, REAL y, REAL z)
{
    M4 mret;
    mret._m[12] = x; mret._m[13] = y; mret._m[14] = z;
    return mret;
}

//---------------------------------------------------------------------------------------
inline  M4 MScale(REAL x, REAL y, REAL z, REAL w = 1.0)
{
    M4 mret;
    mret._m[0] = x; mret._m[5] = y; mret._m[10] = z; mret._m[15] = w;
    return mret;
}

//---------------------------------------------------------------------------------------
inline M4 MLook(const V3& eye,const  V3& fwd,const  V3& up,const  V3& side)
{
    M4 mret;
    mret._m[0] = side.x;    mret._m[4] = side.y;    mret._m[8] = side.z;   mret._m[12] = 0.0;
    mret._m[1] = up.x;      mret._m[5] = up.y;      mret._m[9] = up.z;     mret._m[13] = 0.0;
    mret._m[2] = -fwd.x;    mret._m[6] = -fwd.y;    mret._m[10] = -fwd.z;   mret._m[14] = 0.0;
    mret._m[3] = 0.0;       mret._m[7] = 0.0;       mret._m[11] = 0.0;      mret._m[15] = 1.0;
    mret *= MTranslate(-eye.x, -eye.y, -eye.z);
    return mret;
}

//---------------------------------------------------------------------------------------
inline void GetAnglesFromVector( V3 &vec, REAL &yawAng, REAL &pitchAng ,REAL& roll)
{
    yawAng = atan2(vec.x, vec.y) ;
    roll   = atan2(vec.z, vec.y);

    if( yawAng < 0.0 )
    {
        yawAng += DOIPI;
    }
    if( roll < 0.0 )
    {
        roll += DOIPI;
    }


    if( fabs(vec.x) > fabs(vec.y) )
    {
        pitchAng = atan2( fabs(vec.z), fabs(vec.x) );
    }
    else
    {
        pitchAng = atan2( fabs(vec.z), fabs(vec.y) );
    }

    if( vec.z < 0.0 )
    {
        pitchAng = -pitchAng;
    }
    pitchAng = (pitchAng);
    yawAng   = (yawAng);
    roll     = (roll);
}


//---------------------------------------------------------------------------------------
#define	PLANE_X						0
#define	PLANE_Y						1
#define	PLANE_Z						2
#define	PLANE_ANYX					3
#define	PLANE_ANYY					4
#define	PLANE_ANYZ					5
#define	PLANE_ANY					6

//---------------------------------------------------------------------------------------
typedef  REAL  APlane[4];
class Plane
{
public:
    Plane():_c(0.0),_t(PLANE_ANY){};
    Plane& operator=(const Plane& pl){_n = pl._n; _c = pl._c;_t =pl._t;	return *this;};
    BOOL operator ==(const Plane& p){if(IsZero(p._c - _c)){	return IsZero( Vdp(_n , p._n)-1.0,  EPSILON);}	return FALSE;}
    Plane(const V3& vn, const REAL c):_n(vn),_c(c),_t(PLANE_ANY){}
    Plane(const V3& vn, const V3& pt):_n(vn),_t(PLANE_ANY){	_c = Vdp(_n, pt);}
    Plane(const V3& p0, const V3& p1, const V3& p2):_t(PLANE_ANY){CalcNormal(p0, p1, p2);}
    Plane(REAL a, REAL b , REAL c, REAL d) :_t(PLANE_ANY){_n = V3(a,b,c); 	_c = d;	}
    ~Plane(){}
    INLN int GetSide(const V3& pct)const{REAL r =  DistTo(pct);	if(r>EPSILON)	return 1;else if(r<-EPSILON)return -1;	return 0;}
    INLN REAL DistTo(const V3& pct)const{
        REAL r = (Vdp(pct, _n) - _c);
        return r;
    }
    INLN REAL DistTo(REAL a, REAL b, REAL c)const{return (Vdp(V3(a,b,c), _n) - _c);	}
    INLN REAL operator-( V3& pt){return DistTo(pt);}
    INLN void Set(const V3& vn, const V3& pct){_n = vn; _c  = Vdp(_n, pct);_t = PLANE_ANY;}
    INLN void Set(const V3& vn, REAL cnst){	_n = vn; _c=cnst;_t = PLANE_ANY;}
    INLN void Reverse(){	_n.negate();_c=-_c;	}
    INLN void CalcNormal(const V3& p0, const  V3& p1, const  V3& p2){
        V3 vV1 = p0-p1;
        V3 vV2 = p0-p2;
        _n = Vcp(vV1, vV2);	_n.norm();
        _c = Vdp(p0,_n);
    }
    INLN char GetApproxAlignment()	{if (Rabs(_n.x) > Rabs(_n.y) && Rabs(_n.x) > Rabs(_n.z))return 'x';	if (Rabs(_n.y) > Rabs(_n.x) && Rabs(_n.y) > Rabs(_n.z))		return 'y';	return 'z';	}
    INLN REAL FastDistTo(const V3& pct)const{
        switch (_t&0xF){
        case PLANE_X: if(_n.x<0) return -(pct.x - (-_c)); return (pct.x - _c);
        case PLANE_Y: if(_n.y<0) return -(pct.y - (-_c)); return (pct.y - _c);
        case PLANE_Z: if(_n.z<0) return -(pct.z - (-_c)); return (pct.z - _c);
        }
        return (Vdp(pct, _n) - _c);
    }
    void TypeIt(){
        _t = PLANE_ANY;
        if(IsZero(Rabs(_n.x)-1.0))		_t = PLANE_X;
        else if(IsZero(Rabs(_n.y)-1.0))	_t = PLANE_Y;
        else if(IsZero(Rabs(_n.z)-1.0))	_t = PLANE_Z;
    }
    INLN BOOL RayIntersect( const V3& vpos, const V3& vdir, V3& ip)const{
        REAL dp = Vdp(_n, vdir);
        if(dp  >= 0.0)//away from plane
        {
            return FALSE;
        }
        ip = vpos - ((vdir * DistTo(vpos)) / dp);
        return TRUE;
    }
    BOOL GetIP ( const Plane &a, const Plane &b, V3 &v )
    {
        V3   cp    = Vcp(a._n, b._n);
        REAL denom = Vdp(_n, cp);

        if ( fabs ( denom ) < 1e-5 )
        {
            return FALSE;
        }
        V3  va = Vcp( a._n, b._n ) * -_c;
        V3  vb = Vcp(b._n, _n )    * a._c ;
        V3  vc = Vcp(_n, a._n )    * b._c ;
        v = ( va - vb- vc) / denom;
        return TRUE;
    }

    /*
    int GetSide(V3& center, V3& aex)
    {
    V3  vx(aex.x,0,0);
    V3  vy(0,aex.y,0);
    V3  vz(0,0,aex.z);
    return GetSide(center, vx, vy, vz);
    }

      int GetSide(V3& center, V3& ax, V3& ay, V3& az)
      {
      REAL d2p =  DistTo(center);
      REAL cd  =  Vdp(ax,_n) + Vdp(ay,_n) + Vdp(az,_n);
      if(d2p > cd)return 1;
      else if(d2p < cd)return -1;
      return 0;
      }
    */

public:
    V3		_n;
    REAL	_c;
    unsigned short	_t;
    unsigned short	_u;
    
};

//---------------------------------------------------------------------------------------
typedef  REAL  ASphere[4];
class   NO_VT Sphere
{
public:
    Sphere(){}
    Sphere(V3& center, REAL r):_c(center), _r(r){}
    BOOL IsTouchingSphere( const Sphere& other){	return vdist(_c,other._c) < (other._r + _r);}
    BOOL GetSegIPoint(const V3& start, const V3& dir, V3& rip){
        V3&    center = _c;
        REAL&  r = _r;
        V3 kDif = start - center;
        REAL fA = dir.len2();
        REAL fB = kDif.vdp(dir);
        REAL fC = kDif.len2() - r*r;
        REAL afT[2];
        REAL fDiscr = fB*fB - fA*fC;
        if ( fDiscr < 0.0 )	{
            return FALSE;
        }
        else if ( fDiscr > 0.0 ){
            REAL fRoot = (REAL)sqrt(fDiscr);
            REAL fInvA = 1.0/fA;
            afT[0] = (-fB - fRoot)*fInvA;
            afT[1] = (-fB + fRoot)*fInvA;
        }else{
            afT[0] = -fB/fA;
        }
        rip  = start + dir*afT[0];
        return TRUE;
    }
    REAL DistToRay(const V3& origin, const V3& dir)	{
        V3 xv(_c);
        REAL& r = _r;
        xv -= origin;
        REAL lx = xv.len();
        REAL dp = xv.vdp(dir);
        REAL d = r*r - (lx*lx - dp*dp);
        if(d < 0) return INFINIT;
        return dp - (REAL)sqrt(d);

    }
    BOOL FindRayIP(const V3& start,const V3& dir, V3& ip);
    BOOL DistToSphere(const Sphere& other, REAL* d=0){
        V3& oc = (V3&)other._c;
        REAL di = (_r + other._r) - vdist(_c, oc);
        if(d)
            *d = di;
        return d < 0;
    }
public:
    V3     _c;
    REAL   _r;
};

//---------------------------------------------------------------------------------------
typedef  REAL  ABox[6];
class   NO_VT Box
{
public:
    Box():_min(INFINIT),_max(-INFINIT){}
    Box(V3& m, V3& M4):_min(INFINIT),_max(-INFINIT){AddPoint(m);	AddPoint(M4);}
    Box(const Box& box):_min(box._min),_max(box._max){}
    Box(const Box& box, V3& a, V3& b, REAL ex=0):_min(box._min),_max(box._max){MakeMoveBox(a,b,ex);}
    Box(const Box& box, V3& a):_min(box._min),_max(box._max){Move(a);}
    Box(REAL rad):_min(-rad),_max(rad){}
    void Reset(){_min =INFINIT;_max=-INFINIT;}
    Box& operator=(const Box& box){	_min=box._min;	_max=box._max;	return *this;}
    Box& operator+=(V3& pos) {_min+=pos;_max+=pos;	return *this;}
    Box& operator+=(REAL val){_max+=val; _min+=(-val);	return *this;}
    Box& operator*=(REAL val) {_max*=val;_min*=val;	return *this;}
    void Union(const Box& b){
        _max.x = tmax(_max.x, b._max.x);			_max.y = tmax(_max.y, b._max.y);
        _max.z = tmax(_max.z, b._max.z);			_min.x = tmin(_min.x, b._min.x);
        _min.y = tmin(_min.y, b._min.y);			_min.z = tmin(_min.z, b._min.z);
    }
    void AddPoint(V3& a){
        _max.x = tmax(_max.x, a.x);			_max.y = tmax(_max.y, a.y);
        _max.z = tmax(_max.z, a.z);			_min.x = tmin(_min.x, a.x);
        _min.y = tmin(_min.y, a.y);			_min.z = tmin(_min.z, a.z);
    }

    void MakeMoveBbox( V3& start,  V3& end, REAL ex=0.0)
    {
        for (int i=0 ; i<3 ; i++)
        {
            if (end[i] > start[i])
            {
                _min[i] += start[i]  - ex;
                _max[i] += end[i]    + ex;
            }
            else
            {
                _min[i] += end[i]   - ex;
                _max[i] += start[i] + ex;
            }
        }
    }
    REAL GetRadius(){return GetMaxExtend()/2;}

    void FlatIt(UV* puv){
        REAL mx = _max.x-_min.x;
        REAL my = _max.y-_min.y;
        REAL mz = _max.z-_min.z;
        REAL me =  tmin(tmin(mx,my),mz);
        if(me == mx){
            _max.x = _min.x =0;
            puv[0] = UV(_min.z,_min.y);
            puv[1] = UV(_max.z,_max.y);
        }
        else if(me == my){
            _max.y = _min.y =0;
            puv[0] = UV(_min.x,_min.z);
            puv[1] = UV(_max.x,_max.z);
        }
        else if(me == mz){
            _max.z = _min.z =0;
            puv[0] = UV(_min.x,_min.y);
            puv[1] = UV(_max.x,_max.y);
        }
    }
    char GetMinimAx(){
        REAL mx = _max.x-_min.x;
        REAL my = _max.y-_min.y;
        REAL mz = _max.z-_min.z;
        REAL me =  tmin(tmin(mx,my),mz);
        if(me == mx)		return 'x';
        else if(me == my)	return 'y';
        else if(me == mz)	return 'z';
        return 0;
    }

    REAL GetMinExtend()	{
        REAL mx = _max.x-_min.x;
        REAL my = _max.y-_min.y;
        REAL mz = _max.z-_min.z;
        return tmin(tmin(mx,my),mz);
    }

    REAL GetMaxExtend()const{
        REAL mx = _max.x-_min.x;
        REAL my = _max.y-_min.y;
        REAL mz = _max.z-_min.z;
        return tmax(tmax(mx,my),mz);
    }
    REAL DistTo(const Plane& p)
    {
        const V3& ext = GetExtends();
        return Rabs(ext.x*p._n.x)+Rabs(ext.y*p._n.y)+Rabs(ext.z*p._n.z);
    }
    V3 GetExtends()const	{	return (_max-_min);	}
    void Transform(M4& m){
        m.v3transform(_max);
        m.v3transform(_min);
    }
    V3  GetCenter()const	{return (_min + _max)/2;}

   	BOOL ContainBox(const Box& pct){
        return  ContainPoint(pct._max) &&
            ContainPoint(pct._min);
    }

    BOOL ContainPoint(const V3& pct)const{
        return  (pct.x>_min.x && pct.x<_max.x &&
            pct.y>_min.y && pct.y<_max.y &&
            pct.z>_min.z && pct.z<_max.z );
    }

    void Expand0(REAL ammnt = .1f, REAL epsil=EPSILONBIG)
    {
        V3 ex = _max-_min;
        if(IsZero(ex.x, epsil)){
            _max.x+=ammnt;
            _min.x-=ammnt;
        }
        if(IsZero(ex.y,epsil)){
            _max.y+=ammnt;
            _min.y-=ammnt;
        }
        if(IsZero(ex.z,epsil)){//AUG 04
            _max.z+=ammnt;
            _min.z-=ammnt;
        }
    }

    void Expand(REAL val){
        _min-=val;
        _max+=val;
    }

    BOOL IsTouchesSphere(const Sphere& other){
        const  REAL&  rad = other._r;
        const  V3&    pos = other._c;

        if (pos.x+rad >= _min.x && pos.x-rad <= _max.x)
        if (pos.y+rad >= _min.y && pos.y-rad <= _max.y)
        if (pos.z+rad >= _min.z && pos.z-rad <= _max.z)
            return TRUE;
        return FALSE;
    }

     BOOL IsTouchesSphere(V3& vpos, REAL rrad)
     {
        if (vpos.x+rrad >= _min.x && vpos.x-rrad <= _max.x)
        if (vpos.y+rrad >= _min.y && vpos.y-rrad <= _max.y)
        if (vpos.z+rrad >= _min.z && vpos.z-rrad <= _max.z)
            return TRUE;
        return FALSE;
    }
  
    BOOL IsTouchesBox(const Box& other)
    {
        if(_max.x < other._min.x)				return FALSE;
        if(_min.x > other._max.x)				return FALSE;
        if(_max.z < other._min.z)				return FALSE;
        if(_min.z > other._max.z)				return FALSE;
        if(_max.y < other._min.y)				return FALSE;
        if(_min.y > other._max.y)				return FALSE;
        return TRUE;
    }

    void	FromSegment(const V3& a, const V3& b){
        _min.x = tmin(a.x,b.x);
        _min.y = tmin(a.y,b.y);
        _min.z = tmin(a.z,b.z);
        _max.x = tmax(a.x,b.x);
        _max.y = tmax(a.y,b.y);
        _max.z = tmax(a.z,b.z);
    }

    REAL PlaneShift(Plane& plan)
    {
        REAL& r = plan._c;
        V3&	  pn = plan._n;

        if (pn.x > 0)
            r -= pn.x * _min.x;
        else
            r -= pn.x * _max.x;

        if (pn.y > 0)
            r -= pn.y * _min.y;
        else
            r -= pn.y * _max.y;

        if (pn.z > 0)
            r -= pn.z * _min.z;
        else
            r -= pn.z * _max.z;
        return 0;
    }

    BOOL PointInside(V3& pct)
    {
        return ((pct.x >= _min.x) && (pct.x <= _max.x) &&
            (pct.y >= _min.y) && (pct.y <= _max.y) &&
            (pct.z >= _min.z) && (pct.z <= _max.z));
    }

    void    GetFace(V3* pV, char face)//'u,d,f,b,e,w'
    {
        // return faces facing outwards
        switch(face)
        {
        case 'U':
            pV[0] = V3(_min.x, _max.y, _min.z);
            pV[1] = V3(_max.x, _max.y, _min.z);
            pV[2] = V3(_max.x, _max.y, _max.z);
            pV[3] = V3(_min.x, _max.y, _max.z);
            break;
        case 'D':
            pV[3] = V3(_min.x, _min.y, _min.z);
            pV[2] = V3(_max.x, _min.y, _min.z);
            pV[1] = V3(_max.x, _min.y, _max.z);
            pV[0] = V3(_min.x, _min.y, _max.z);
            break;
        case 'F':
            pV[0] = V3(_min.x, _max.y, _max.z);
            pV[1] = V3(_max.x, _max.y, _max.z);
            pV[2] = V3(_max.x, _min.y, _max.z);
            pV[3] = V3(_min.x, _min.y, _max.z);
            break;
        case 'B':
            pV[3] = V3(_min.x, _max.y, _min.z);
            pV[2] = V3(_max.x, _max.y, _min.z);
            pV[1] = V3(_max.x, _min.y, _min.z);
            pV[0] = V3(_min.x, _min.y, _min.z);
            break;
        case 'L':
            pV[0] = V3(_min.x, _max.y, _min.z);
            pV[1] = V3(_min.x, _max.y, _max.z);
            pV[2] = V3(_min.x, _min.y, _max.z);
            pV[3] = V3(_min.x, _min.y, _min.z);
            break;
        case 'R':
            pV[3] = V3(_max.x, _max.y, _min.z);
            pV[2] = V3(_max.x, _max.y, _max.z);
            pV[1] = V3(_max.x, _min.y, _max.z);
            pV[0] = V3(_max.x, _min.y, _min.z);
            break;
        default: break;
        }
    }

    void Move(V3& pos)
    {
        _min+=pos;_max+=pos;
    }

    void MakeMoveBox(V3& start, V3& end, REAL ex=0.0)
    {
        for (int i=0 ; i<3 ; i++)
        {
            if (end[i] > start[i])
            {
                _min[i] += start[i]  - ex;
                _max[i] += end[i]    + ex;
            }else
            {
                _min[i] += end[i]   - ex;
                _max[i] += start[i] + ex;
            }
        }
    }
    V3* GetCorners(V3* pVx)
    {
        pVx[0].setval(_min);
        pVx[1].setval(_min.x,_min.y,_max.z);
        pVx[2].setval(_max.x,_min.y,_max.z);
        pVx[3].setval(_max.x,_min.y,_min.z);

        pVx[4].setval(_min.x,_max.y,_min.z);
        pVx[5].setval(_max.x,_max.y,_min.z);
        pVx[6].setval(_max);
        pVx[7].setval(_max.x,_min.y,_max.z);
        return _norms;
    }

    Plane* GetPlanes()
    {
        static Plane planes[6];

        planes[0]._n = VY;
        planes[0]._c = _max.y;

        planes[1]._n = -VY;
        planes[1]._c = _min.y;

        planes[2]._n = VX;
        planes[2]._c = _max.x;

        planes[3]._n = -VX;
        planes[3]._c = _min.x;

        planes[4]._n = VZ;
        planes[4]._c = _max.z;

        planes[5]._n = -VZ;
        planes[5]._c = _min.z;

        return planes;

    }

    REAL Volume(){
        V3 extend = _max-_min;
        return Vdp(extend, extend);
    }

public:
    V3	        _min;
    V3	        _max;
    static V3   _norms[8];
};


//---------------------------------------------------------------------------------------
INLN void    GetCornersOfPlane(V3& mins, V3&  maxs, V3& normal, V3* corners)
{
    for (REG int i=0 ; i<3 ; i++)
    {
        if (normal[i] < 0)
        {
            corners[0][i] = mins[i];
            corners[1][i] = maxs[i];
        }
        else
        {
            corners[1][i] = mins[i];
            corners[0][i] = maxs[i];
        }
    }
}

//--------------------------------------------------------------------------------------------
inline V3	GetMinAx(V3& normal)
{
    V3	rv;
    if( Rabs(normal.y) > Rabs(normal.z) )
    {
        if( Rabs(normal.z) < Rabs(normal.x) )
            rv.z = 1.f;
        else
            rv.x = 1.f;
    }
    else
    {
        if (Rabs(normal.y )<= Rabs(normal.x) )
            rv.y = 1;
        else
            rv.x = 1;
    }
    return rv;
}

//--------------------------------------------------------------------------------------------
inline V3	GetMaxAx(V3& normal)
{
    V3	rv = normal;

    rv.x = Rabs(rv.x);
    rv.y = Rabs(rv.y);
    rv.z = Rabs(rv.z);

    if(rv.x == 1)
        return V3(1,0,0);
    if(rv.y == 1)
        return V3(0,1,0);
    if(rv.z == 1)
        return V3(0,0,1);

    if(rv.x > rv.y && rv.x > rv.z)
        return V3(1,0,0);
    else if(rv.y > rv.x && rv.y > rv.z)
        return V3(0,1,0);
    return V3(0,0,1);
}

INLN void	ReflectVector(V3& who, V3& normal, REAL attn, V3& newSpeed)
{
    newSpeed = 2.0f * (normal * Vdp(-who, normal)) +  who;
    newSpeed*=attn;
}




INLN void ClipVector(V3& speed, V3& normal, REAL bounce, V3& newSpeed)
{
    REAL    reflect = Vdp(speed, normal) ;
    newSpeed = bounce * (speed - normal * reflect);
    if(Rabs(newSpeed.x)<.2f)newSpeed.x=0;
    if(Rabs(newSpeed.y)<.2f)newSpeed.y=0;
    if(Rabs(newSpeed.z)<.2f)newSpeed.z=0;
}

INLN void MakeDir(V3& av, V3& bv, V3& dir){
    dir = bv;
    dir -=av;
    dir.norm();
}
INLN void TweakPlaneDistance(V3& pos, V3& norm){
    pos  += (norm * .3f);
}
INLN void HackVertBilboard(V3& campos, V3& pos, V3& dir){
    dir	    =  campos;
    dir	    -= pos;
    dir.y	=  0.0f;
    dir.norm();
}


INLN REAL DistPointSeg(const V3& start, const V3& end, const V3& point)
{
    V3 diff = point - start;
    V3	dend = end-start;
    REAL fT = Vdp(diff, dend);

    if ( fT <= 0.0 )
    {
        fT = 0.0;
    }
    else
    {
        REAL len2 = dend.len2();
        if ( fT >= len2)
        {
            fT = 1.0;
            diff -= dend;
        }
        else
        {
            fT /= len2;
            diff -= (dend * fT);
        }
    }
    return  diff.len();
}

INLN REAL DistPointSeg(const V3& start, const V3& end, const V3& point, V3& ponLine)
{
    V3 diff = point - start;
    V3	dend = end-start;
    REAL fT = Vdp(diff, dend);

    if ( fT <= 0.0 )
    {
        fT = 0.0;
        ponLine = start;
    }
    else
    {
        REAL len2 = dend.len2();
        if ( fT >= len2)
        {
            fT = 1.0;
            diff -= dend;
        }
        else
        {
            fT /= len2;
            diff -= (dend * fT);
        }
        ponLine = start + (end-start)*fT;
    }
    return  diff.len();
}


//----------------------------------------------------------------------------------------
INLN REAL DistRayPlane(V3& rOrigin, V3& rDir, V3& pOnPlan, V3& pNormal)
{
    REAL denom = Vdp(pNormal,rDir);
    if (denom == 0)
        return INFINIT;
    REAL da = - (Vdp(pNormal,pOnPlan));
    REAL numer = Vdp(pNormal,rOrigin) + da;
    return -(numer / denom);
}

//----------------------------------------------------------------------------------------
INLN V3 ClosestLinePointToPoint(V3& lstart, V3& lend, V3& point)
{
    static V3 vret;
    // Determine t (the length of the V3 from ‘lstart’ to ‘point’)
    vret = point;
    vret -= lstart;
    V3 xv = lend;
    xv-=lstart;

    REAL lenxv = xv.len();
    xv.norm();

    REAL t = Vdp(xv,vret);

    // Check to see if ‘t’ is beyond the extents of the line segment
    if (t < 0.0f)
    {
        vret.setval(lstart);
        return vret;
    }
    if (t > lenxv)
    {
        vret.setval(lend);
        return vret;
    }
    // construct to point a+bt
    vret = lstart;
    vret += xv*t;
    return vret;
}

INLN const REAL S_PushPlane(Plane& plan, V3& ex, BOOL upward=1)
{
	const V3& normal = plan._n;
    if(upward)
    {
        plan._c += (normal.x) * ex.x;
  	    plan._c += (normal.y) * ex.y;
        plan._c += (normal.z) * ex.z;
    }
    else
    {
        plan._c -= normal.x * ex.x;
    	plan._c -= normal.y * ex.y;
        plan._c -= normal.z * ex.z;
    }
    return 0;
}

INLN const REAL ExtendsToPlane(const V3& v, const Plane& p)
{
    return Rabs(v.x*p._n.x)+Rabs(v.y*p._n.y)+Rabs(v.z*p._n.z);
}

//---------------------------------------------------------------------------------------
#define FOREACH(_xtype, _coll, it_begin)    if(_coll.size()) \
                                            for(_xtype::iterator it_begin = _coll.begin(); it_begin!=_coll.end();it_begin++)




//---------------------------------------------------------------------------------------
typedef  REAL  APos[12];
class NO_VT Pos 
{
public:
    V3		_right;
    V3		_up;
    V3		_fwd;
    V3		_pos;
    V3      _euler;
    REAL    _rot;
    V3      _scale;
    int     _index;
    INLN 	void	MoveUp(REAL r){_pos   += r * _up;}
    INLN 	void	MoveSide(REAL r){_pos += r * _right;}
    INLN 	void	MoveFwd(REAL r){_pos  += r * _fwd;}
    INLN    void    Rotate(REAL a, REAL e, REAL r)
    {
        _euler.y += a;	
	    _euler.x -= e;	
	    _euler.z += r;
	    BLOCKRANGE(_euler.x);
        Euler2Pos();
    }
 
    INLN void	SetAngles(REAL a, REAL e, REAL r){
        ROLLPI(a);
        BLOCKRANGE(e);
        _euler.y=a; _euler.x=e; _euler.z=r;
        Euler2Pos();
    }

    INLN    M4	    LookAt(){return MLook(_pos, _fwd, _up, _right);}
    operator M4*()const{return ((M4*)this);}
    Pos& operator=(const Pos& p){
        if(&p==this)return *this;
        _right = p._right;
        _up    = p._up;
        _fwd   = p._fwd;
        _pos   = p._pos;
        _euler = p._euler;
        _rot   = p._rot;
        return *this;
    }
    void Euler2Pos(){
	    REAL CosrAz = Cosr(_euler.y);
	    REAL CosrEl = Cosr(_euler.x);
	    REAL SinrAz = Sinr(_euler.y);
	    REAL SinrEl = Sinr(_euler.x);
	    REAL CosrRl = Cosr(_euler.z);
	    REAL SinrRl = Sinr(_euler.z);
	    _fwd.x = SinrAz * CosrEl;
	    _fwd.y = SinrEl;
	    _fwd.z = CosrEl * -CosrAz;
	    _up.x = -CosrAz * SinrRl - SinrAz * SinrEl * CosrRl;
	    _up.y = CosrEl * CosrRl;
	    _up.z = -SinrAz * SinrRl - SinrEl * CosrRl * -CosrAz;
	    _right = Vcp(_fwd, _up);
    }

    INLN void Dir2Euler(BOOL toDeg){
        REAL dy2 = (REAL)sqrt(1-_fwd.y*_fwd.y);
        _euler.x = (REAL)Sinr(_fwd.y);
        _euler.z = (REAL)Cosr(_up.y  / dy2);
        _euler.y = (REAL)Sinr(_fwd.x / dy2);	  // az
        ROLLPI(_euler.y);
        if(toDeg)GetEulerDegrees();
    }

    virtual void MoveAndRot(int* pr, REAL m, REAL r){
        MoveFwd(pr[2]*m);
        MoveSide(pr[0]*m);
        MoveUp(pr[1]*m);
        Rotate( pr[3] * r , pr[4] * r, pr[5] * r);
    }
    INLN V3 GetEulerDegrees(int b=0){
        V3 ret(_euler.x,_euler.y, _euler.z);
        if(1 == b){
            ret.y = 180.0-_euler.y;
        }
        else if(_fwd.z < 0.f){
            ret.y = 180.0-_euler.y;
        }
        return ret;
    }
};


//---------------------------------------------------------------------------------------

#pragma pack(pop)



#endif // __BASELIB_H__

/*
//---------------------------------------------------------------------------------------
class NO_VT Ortho
{
public:
    V3		_wpos;
    V3		_ldir;
    V3		_lup;
    V3		_lside;
    V3	    _euler;
    V3      _grsped;
    V3      _vel;

    REAL	_rotAngle;
    V3      _rotPivot;
    V3		_scalation;
    V3	    _pwpos;
public:
    virtual ~Ortho(){}
    Ortho(REAL *pos){
        _wpos.setval(pos[0],pos[1],pos[2]);
        SetAngles(pos[3], pos[4], pos[5]);
    }
    Ortho(){
        Reset();
    }
    INLN void Reset(){
        _pwpos.reset();
        _wpos.reset();
        _ldir.setval(V3(0,0,-1));
        _lup.setval(V3(0,1,0));
        _lside.setval(V3(1,0,0));
        _rotAngle  = 0.0;
        _scalation = 1.0;
    }

    void CopyAngles(Ortho* pos){
        _euler.y = pos->_euler.y;
        _euler.x = pos->_euler.x;
        _euler.z = pos->_euler.z;
    }

    INLN void GetRawPos(REAL* pv){
        pv[0] = _wpos.x;
        pv[1] = _wpos.y;
        pv[2] = _wpos.z;
        pv[3] = _euler.y;
        pv[4] = _euler.x;
        pv[5] = _euler.z;

    }
    INLN void SetRawPos(REAL* pv){
        _wpos.x = pv[0];
        _wpos.y = pv[1];
        _wpos.z = pv[2];
        _euler.y = pv[3];
        _euler.x = pv[4];
        _euler.z = pv[5];
    }
    INLN void	SetAngles(REAL a, REAL e, REAL r){
        ROLLPI(a);
        BLOCKRANGE(e);
        _euler.y=a; _euler.x=e; _euler.z=r;
        Euler2Dir();
    }
    INLN void BlockVertMovement(){
        _lup.setval(0.0);
        _ldir.y = 0;
        _ldir.norm();
    }
    INLN void MoveAndRot(REAL* pr){
        MoveFwd(pr[2]);
        MoveSide(pr[0]);
        MoveUp(pr[1]);
        Rotate( pr[3],pr[4], pr[5]);
    }
    INLN void MoveAndRot(REAL f, REAL s, REAL u, REAL a, REAL e, REAL r)	{
        if(u)MoveUp(u);
        if(s)MoveSide(s);
        if(f)MoveFwd(f);
        Rotate( a, e, r);
    }
    INLN void Rotate(REAL a, REAL e, REAL r)
    {
        _euler.y += a;
        _euler.x -= e;
        _euler.z += r;
        BLOCKRANGE(_euler.x);
        Euler2Dir();
    }
    INLN void Copy(Ortho* pr){
        _wpos      = pr->_wpos;
        _ldir      = pr->_ldir;
        _lup       = pr->_lup;
        _lside	   = pr->_lside;
        _euler.y  = pr->_euler.y;
        _euler.x  = pr->_euler.x;
        _euler.z  = pr->_euler.z;
        _rotAngle  = pr->_rotAngle;
    }

    INLN 	void	MoveUp(REAL r){
        _wpos += r*_lup;
    }
    INLN 	void	MoveSide(REAL r){
        _wpos += r*_lside;
    }
    INLN 	void	MoveFwd(REAL r){
        _wpos += r*_ldir;
    }
    INLN M4	MakeLookAtMatrix(){
        return MLook(_wpos, _ldir, _lup, _lside);
    }
    INLN void EqualDirs(Ortho* pos){
        _wpos = pos->_wpos;
        _ldir = pos->_ldir;
        _lside = pos->_lside;
    }
    INLN void EqualAngles(Ortho* pos){
        _euler.y = pos->_euler.y;
        _euler.x = pos->_euler.x;
        _euler.z = pos->_euler.z;
    }

    INLN void Dir2Euler(BOOL toDeg){
        REAL dy2 = (REAL)sqrt(1-_ldir.y*_ldir.y);
        _euler.x = (REAL)Sinr(_ldir.y);
        _euler.z = (REAL)Cosr(_lup.y  / dy2);
        _euler.y = (REAL)Sinr(_ldir.x / dy2);	  // az
        ROLLPI(_euler.y);
        if(toDeg)GetEulerDegrees();
    }

    INLN void Euler2Dir(){
        REAL CosrAz = Cosr(_euler.y);
        REAL CosrEl = Cosr(_euler.x);
        REAL SinrAz = Sinr(_euler.y);
        REAL SinrEl = Sinr(_euler.x);
        REAL CosrRl = Cosr(_euler.z);
        REAL SinrRl = Sinr(_euler.z);
        _ldir.x = SinrAz * CosrEl;
        _ldir.y = SinrEl;
        _ldir.z = CosrEl * -CosrAz;
        _lup.x = -CosrAz * SinrRl - SinrAz * SinrEl * CosrRl;
        _lup.y = CosrEl * CosrRl;
        _lup.z = -SinrAz * SinrRl - SinrEl * CosrRl * -CosrAz;
        _lside = Vcp(_ldir, _lup);
    }

    INLN M4 BuildTrMatrix(){
        M4 trmat;
        if(_wpos.isnot0())
            trmat *= MTranslate(_wpos.x,_wpos.y,_wpos.z);          // translate to desired pos
        if(_scalation.x!=1.0)
            trmat *= MScale(_scalation.x,_scalation.y,_scalation.z);     // scale it
        if(_rotAngle)
            trmat *= MRotAround(_rotPivot, G2R(_rotAngle));            // rotate around _rotPivot if _rotAngle
        if(_euler.isnot0())
            trmat *= MRadRotate(_euler.x,_euler.y,_euler.z);          // rotate
        return trmat;
    }
    INLN V3 GetEulerDegrees(int b=0){
        V3 ret(_euler.x,_euler.y, _euler.z);
        if(1 == b){
            ret.y = 180.0-_euler.y;
        }
        else if(_ldir.z < 0.f){
            ret.y = 180.0-_euler.y;
        }
        return ret;
    }
};
*/
