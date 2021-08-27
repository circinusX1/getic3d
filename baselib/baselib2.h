// --------------------------------------------------------------------------------------
// Copyright (c) 2000-2005 Zalsoft Inc
// Copyright: zalsoft Inc
// --------------------------------------------------------------------------------------
#ifndef __BASELIB_H__
#define __BASELIB_H__


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
#error "no OS"
#endif //



//---------------------------------------------------------------------------------------
#ifdef _BDL_PREC
typedef	double REAL;
#else
typedef	float REAL;
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
    CLR (size_t dwclr){ a=255; r= (dwclr>>16)&0xff; g= (dwclr>>8)&0xff; b= (dwclr)&0xff;}
    operator size_t(){size_t drv; drv = (r<<24)|(g<<16)|b; return drv;}
    INLN  operator BYTE*(){	return (BYTE*)&r; }
    INLN void black(){	memset(this,0,sizeof(*this));}
    INLN void white(){	memset(this,255,sizeof(*this));}
    INLN void grey(){	memset(this,128,sizeof(*this));}
    INLN void lgrey(){	memset(this,192,sizeof(*this));}
    INLN CLR& operator =(size_t dwclr){ a=255; r= (dwclr>>24)&0xf; g= (dwclr>>16)&0xf; b= (dwclr>>8)&0xf; return *this;}
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
        return 