/**
    File:   irender.h
    Author: Marius Chincisan
    Desc:   Interfaces definition for rendering DLL's
    History:
*/
#ifndef __IRENDER_H__
#define __IRENDER_H__

#include "../baselib/baseutils.h"
#include "../baselib/basecont.h"

//---------------------------------------------------------------------------------------
#define CB_WNDMSG               0x1
//---------------------------------------------------------------------------------------
// Used to respond and manage the texture on main thread when used form different threads
#define WM_DELETE_TEXTURE       WM_USER+1988
#define WM_STORE_TEXTURE        WM_USER+1987
#define WM_OPENGLNOTY           WM_USER+1986

//---------------------------------------------------------------------------------------
// object runtime flags
#define OBJ_LOADIDENTITY        0x1                         
#define OBJ_ROTATE              0x2
#define OBJ_TRANSLATE           0x4
#define OBJ_SCALE               0x8
#define OBJ_ROTATE_EX           0x10
#define OBJ_BB_CYL              0x20
#define OBJ_BB_SPH              0x40
#define OBJ_BB_ALL              0x80
#define OBJ_HIDE	            0x100
#define OBJ_SHOW_ALLWAYS        0x200
#define OBJ_INVIEW				0x400
#define OBJ_LOCAL_INPUT         0x800
#define OBJ_NOCOLLIDE			0x1000
#define OBJ_USE_SOFT_TR		    0x8000	    //software trnasformation	
#define OBJ_INPVS               0x10000
#define OBJ_BILLBOARD           (OBJ_BB_CYL|OBJ_BB_SPH|OBJ_BB_ALL)
#define OBJ_TRANSFORM			(OBJ_ROTATE_EX|OBJ_SCALE|OBJ_ROTATE|OBJ_BB_ALL|OBJ_BB_SPH|OBJ_BB_CYL|OBJ_LOADIDENTITY|OBJ_TRANSLATE)
#define OBJ_TRX                 (OBJ_TRANSLATE|OBJ_ROTATE_EX)
#define OBJ_TR                  (OBJ_TRANSLATE|OBJ_ROTATE)
#define OBJ_TS                  (OBJ_TRANSLATE|OBJ_SCALE)
#define OBJ_TRXS                (OBJ_TRANSLATE|OBJ_ROTATE_EX|OBJ_SCALE)

//--DRAW STYLES-------------------------------------------------------------------------

#define DS_TEX_CNT_MASK         0xF          
#define DS_BLEND_DECAL          0x10
#define DS_BLEND_HITMARK        0x20    
#define DS_BLEND_LIGHT          0x40
#define DS_BLEND_HALLO          0x80
#define DS_BLEND_MASK           0x100
#define DS_SHOW_BACK            0x200
#define DS_DEEPTEST_OFF         0x400
#define DS_COLOR			    0x800
#define DS_NODEEPTEST           0x1000
#define DS_TEXT_BIT 		    0x2000		


#define GET_TEXSTAGE_BITS(x_)   (x_ & 0xF)

//----------------------------------------------------------------------------------------
#if 0
      combiners    ***X****   texture stages (1 2 4 8) as VX_TX1
#endif //0
//----------------------------------------------------------------------------------------
// howwhat
#define VX_COLOR                0x00010000   
#define VX_NORMAL               0x00020000
#define VX_BINORM               0x00040000
#define VX_TANGENT              0x00080000
// gap
#define VTXLATERS               0x00F00000  //texture layers bits
#define VX_TX1                  0x00100000
#define VX_TX2                  0x00200000
#define VX_TX3                  0x00400000
#define VX_TX4                  0x00800000

//---------------------------------------------------------------------------------------
#define FACE_TRANSP             0x1000000
#define FACE_BMASK              0x2000000
#define FACE_MIRRORS            0x4000000

#define ALL_FACES               0xF000000
//---------------------------------------------------------------------------------------
#define REND_PRIM(x_)           (x_ & 0x3F)

//---------------------------------------------------------------------------------------
#define HAS_TEX_0(x_)           (x_ & VX_TX1)
#define HAS_TEX_LM(x_)          (x_ & VX_TX2)
#define HAS_TEX_DET(x_)         (x_ & VX_TX3)
#define HAS_TEX_AUX(x_)         (x_ & VX_TX4)

//---------------------------------------------------------------------------------------
// BLENDS
#define DS_BLEND 			    (DS_BLEND_HITMARK|DS_BLEND_LIGHT|DS_BLEND_HALLO)
#define	DS_BL				    (DS_BLEND_LIGHT|DS_DEEPTEST_OFF)
#define	DS_HL				    (DS_BLEND_HALLO|DS_DEEPTEST_OFF)
#define	DS_HLB				    (DS_BLEND_HALLO|DS_DEEPTEST_OFF|DS_SHOW_BACK)


//----------------------------------------------------------------------------------------
#if 0
      HOWWAT    ---Y--XX   Y=genST XX=how
#endif //0
//----------------------------------------------------------------------------------------

//---TEXTURE_TARGET----------------------------------------------------------------------
//target
#define	TGET_TARGET(dw_)                (dw_&0xF)
#define GEN_TEX_2D_MAP			        0x0             // default
#define GEN_TEX_CUBE_MAP	            0x1		   
#define GEN_TEX_3D_MAP			        0x2		  

//---------------------------------------------------------------------------------------
// filter
#define	TGET_FILTER(dw_)                (dw_&0xF0)
#define	GEN_TEX_MM_LINEAR		        0x00            // default
#define	GEN_TEX_MM_BILINEAR		        0x10	
#define	GEN_TEX_MM_TRILINEAR	        0x20
#define	GEN_TEX_MM_NONE	                0x40
//gap
//---------------------------------------------------------------------------------------
// target 2
#define	TGET_WRAP(dw_)                  (dw_&0xF00)
#define GEN_REPEAT                      0x000           // default
#define GEN_CLAMP                       0x100   
#define GEN_CLAMP_TO_BORDER		        0x200
#define GEN_TEX_HAS_CUBE_T			    0x300	// 6 images T shaped
#define GEN_TEX_HAS_CUBE_M			    0x400   // 6 images on 2 rows by 3

//gap
//---------------------------------------------------------------------------------------
#define	TGET_GENST(dw_)                 (dw_&0xF000)
#define GEN_TEX_GEN_NA                  0x0000         // default no generation
#define	GEN_TEX_GEN_ST_SPH              0x1000         
#define	GEN_TEX_GEN_ST_CUBE	            0x2000

//gap
//---------------------------------------------------------------------------------------
#define	TGET_ENV(dw_)                   (dw_&0xFF0000)
//pnames
#define TPN_TEXTURE_ENV_MODE             0x010000  
#define TPN_TEXTURE_ENV_COLOR            0x020000                  
#define TPN_COMBINE_EXT                  0x030000  
#define TPN_COMBINE_RGB_EXT              0x040000  
#define TPN_COMBINE_ALPHA_EXT            0x050000  
#define TPN_RGB_SCALE_EXT                0x060000  
#define TPN_ADD_SIGNED_EXT               0x070000  
#define TPN_INTERPOLATE_EXT              0x080000  
#define TPN_CONSTANT_EXT                 0x090000  
#define TPN_PRIMARY_COLOR_EXT            0x0A0000  
#define TPN_PREVIOUS_EXT                 0x0B0000  
#define TPN_SOURCE0_RGB_EXT              0x0C0000  
#define TPN_SOURCE1_RGB_EXT              0x0D0000  
#define TPN_SOURCE2_RGB_EXT              0x0E0000  
#define TPN_SOURCE0_ALPHA_EXT            0x0F0000  
#define TPN_SOURCE1_ALPHA_EXT            0x100000  
#define TPN_SOURCE2_ALPHA_EXT            0x110000  
#define TPN_OPERAND0_RGB_EXT             0x120000  
#define TPN_OPERAND1_RGB_EXT             0x130000  
#define TPN_OPERAND2_RGB_EXT             0x140000  
#define TPN_OPERAND0_ALPHA_EXT           0x150000  
#define TPN_OPERAND1_ALPHA_EXT           0x160000  
#define TPN_OPERAND2_ALPHA_EXT           0x170000  
// p-params
#define TPP_MODULATE                    0x800000
#define TPP_DECAL                       0x810000
#define TPP_BLEND                       0x820000 
#define TPP_REPLACE                     0x830000
#define TPP_COMBINE_EXT                 0x840000
#define TPP_COMBINE_RGB_EXT             0x850000
#define TPP_COMBINE_ALPHA_EXT           0x860000
#define TPP_RGB_SCALE_EXT               0x870000
#define TPP_ADD_SIGNED_EXT              0x880000
#define TPP_INTERPOLATE_EXT             0x890000
#define TPP_CONSTANT_EXT                0x8A0000
#define TPP_PRIMARY_COLOR_EXT           0x8B0000
#define TPP_PREVIOUS_EXT                0x8C0000
#define TPP_SOURCE0_RGB_EXT             0x8D0000
#define TPP_SOURCE1_RGB_EXT             0x8E0000
#define TPP_SOURCE2_RGB_EXT             0x8F0000
#define TPP_SOURCE0_ALPHA_EXT           0x910000
#define TPP_SOURCE1_ALPHA_EXT           0x920000
#define TPP_SOURCE2_ALPHA_EXT           0x930000
#define TPP_OPERAND0_RGB_EXT            0x940000
#define TPP_OPERAND1_RGB_EXT            0x950000
#define TPP_OPERAND2_RGB_EXT            0x960000
#define TPP_OPERAND0_ALPHA_EXT          0x970000
#define TPP_OPERAND1_ALPHA_EXT          0x980000
#define TPP_OPERAND2_ALPHA_EXT          0x990000
//gap

//---------------------------------------------------------------------------------------
#define TEX_ALPHABW                     0x80000000


//---------------------------------------------------------------------------------------
#define TEX_PRIMARY(flt_)			     (flt_|GEN_TEX_2D_MAP|TPP_REPLACE)
#define TEX_LIGHTMAP(flt_)			     (flt_|GEN_TEX_2D_MAP|TPP_MODULATE)
#define TEX_CUBEMAP(flt_,wrap_)			 (flt_|wrap_|GEN_TEX_CUBE_MAP|TPP_REPLACE)
#define TEX_CUBELMAP(flt_,wrap_)		 (flt_|wrap_|GEN_TEX_CUBE_MAP|TPP_MODULATE)

// dummy predefined textures-------------------------------------------------------------
#define TEX_LOWREZ      			     (GEN_TEX_MM_LINEAR|GEN_TEX_2D_MAP|TPP_REPLACE)
#define TEX_NORMAL      			     (GEN_TEX_MM_TRILINEAR|GEN_TEX_2D_MAP|TPP_REPLACE)
#define TEX_NORMAL_LM     			     (GEN_TEX_MM_TRILINEAR|GEN_TEX_2D_MAP|TPP_MODULATE|GEN_CLAMP)

//---Contexts ---------------------------------------------------------------------------
// Htex flags are the creation flags for the texture. The apply flags are caried out by 
// each item/face
#ifndef __HTEX
    typedef UINT HTEX;
    #define __HTEX
#endif //

//--------------------------------------------------------------------------------------
// texture handler. texture id handler
typedef  DWORD    $Htex[4];
class  NO_VT Htex     
{
public:
    Htex():hTex(0),glTarget(0),envMode(0),genST(0){}
    ~Htex(){}
    Htex(const Htex& r):hTex(r.hTex),glTarget(r.glTarget),envMode(r.envMode),genST(r.genST){}
	BOOL  operator==(const Htex& r){return hTex==r.hTex;};
    Htex& operator=(const Htex& r){if(this!=&r) {hTex=r.hTex; glTarget=r.glTarget; envMode = r.envMode; genST=r.genST;} return *this; }
	operator UINT(){return hTex; };
    void    Clear(){hTex=0;glTarget=0;envMode=0;genST=0;}
    HTEX  hTex;                             // texture unique ID
    DWORD glTarget;							// set by render to GL_TEXTURe_XX	
    DWORD envMode;                          // additional data not used
    DWORD genST;                            // align to 16 bytes
};


//---RENDER_STYLE------------------------------------------------------------------------
#define	USE_CCW			    0x1
#define	USE_CW				0x2
#define	SHOW_FRONT			0x4
#define	SHOW_BACK			0x8
#define	SHOW_BOTH			0x10

//---------------------------------------------------------------------------------------

#define LIGHT_POINT         1
#define LIGHT_DIRECT        2
#define LIGHT_SPOT          4

//hardware light
struct RenderLight 
{
    bool operator==(const RenderLight& r){return _colorDiffuse.r==r._colorDiffuse.r;}
    DWORD   _flags;
    V3      _pos;
    V3      _direction;
    REAL    _radius;
    CLR     _colorAmbient;                 
    CLR     _colorDiffuse;
    CLR     _colorSpecular;
    REAL    _intens;
    REAL    _cutOff;
    REAL    _fallOff;
    REAL    _constAttenuation;
    REAL    _linearAttenuation;
    REAL    _quadratAttenuation;
	long	_userdata;
};

//---------------------------------------------------------------------------------------
// hardware material
struct RenderMaterial 
{
    bool operator==(const  RenderMaterial& r){return _colorDiffuse.r==r._colorDiffuse.r;}
    DWORD   _flags;
    CLR     _colorAmbient;                 
    CLR     _colorDiffuse;
    CLR     _colorEmmisive;
    CLR     _colorSpecular;
    REAL    _shiness;
};

//---------------------------------------------------------------------------------------
//hardware fog
struct RenderFog
{
    REAL    _gnear;
    REAL    _gfar;
    REAL    _dens;
    CLR     _clr;
    DWORD   _flag;
    Plane   _clip;
};


//---------------------------------------------------------------------------------------
// Renderer generic flags. We pass in our array of gPrims with these indexes and the
// plugin DLL is responsable to copy it's internal flags on specific offsets.
enum
{
	ZERO = 0,
	Z_TEXTURE_2D, 
	Z_TEXTURE_3D, 
	Z_TEXTURE_CUBE_MAP,
    Z_FRONT_AND_BACK,
	Z_LINE,
    Z_FILL,
    Z_CULL_FACE,
    Z_FRONT,
    Z_SHININESS,
    Z_BACK,//10
	Z_BLEND,
	Z_POINTS,       //draw mode
	Z_LINES,
	Z_LINE_STRIP,
	Z_LINE_LOOP,
	Z_TRIANGLES,
	Z_TRIANGLE_STRIP,
	Z_TRIANGLE_FAN,
	Z_QUADS,
	Z_QUAD_STRIP,//20
	Z_POLYGON,
	Z_ZERO,         //alpha
	Z_ONE,
	Z_SRC_COLOR,
	Z_ONE_MINUS_SRC_COLOR,
	Z_SRC_ALPHA,
	Z_ONE_MINUS_SRC_ALPHA,
	Z_DST_ALPHA,
	Z_ONE_MINUS_DST_ALPHA,
	Z_DST_COLOR,
	Z_ONE_MINUS_DST_COLOR,
	Z_SRC_ALPHA_SATURATE,
    Z_TEXTURE_GEN_S,
    Z_TEXTURE_GEN_T,
    Z_TEXTURE_GEN_R,
    Z_TEXTURE_GEN_Q,
    Z_TEXTURE_MAG_FILTER,
    Z_TEXTURE_MIN_FILTER,
    Z_TEXTURE_WRAP_S,
    Z_TEXTURE_WRAP_T,
    Z_TEXTURE_ENV_MODE,
    Z_TEXTURE_ENV_COLOR,
    Z_TEXTURE_ENV,
    Z_CLAMP,
    Z_REPEAT,
    Z_AMBIENT,
    Z_DIFFUSE,
    Z_SPECULAR,
    Z_EMISSION,
    Z_LAST_ITEM
};

//---------------------------------------------------------------------------------------
// We pass this in to configure the initial state of the rendering system
struct RndStruct
{
    RndStruct()
    {
		retainedDraw  = FALSE;
        hInstance	  = 0;
        fNear		  = 8.0;
        fFar		  = 131072.0;
        hFov		  = 64.0;
		xPolyMode     = USE_CW|SHOW_BACK;
		wndStyle	  = 0;
		wndParent	  = 0;
        pfDepthBits   = 32;
        pfStencilBits = 1;
        bFullScreen	  = FALSE;
        bgColor       = 0;
        clipCursor    = 0;
        hideCurosr    = 1;
        bsetCapture    = 0;
		dmDisplayFrequency=60;
        dmPelsWidth   = 640; 
		dmPelsHeight  =	480;
		dmBitsPerPel  = 16;

        bIsActivated = FALSE;
        fWidth	   = 0.0;
        fHeight    = 0.0;
        nWidth	   = 0;
        nHeight	   = 0;
        dwVpStyle  = 3; //clsoe and maximize button
        szError[0] = 0;        
    }

	void*		    wndParent;
	DWORD          	wndStyle;
    void*	    hInstance;
	BOOL		retainedDraw;
    
    REAL		fNear;
    REAL		fFar;
    REAL		hFov;
    
    
    
    DWORD       bgColor;
    int         dmPelsWidth; 
    int         dmPelsHeight;
    int         dmBitsPerPel;
    int         dmDisplayFrequency;

	DWORD		xPolyMode;
	BOOL		bFullScreen;
    int         pfDepthBits;
    int         pfStencilBits;

    REAL		fAspect;
    REAL		fWidth;
    REAL		fHeight;
    int			nWidth;
    int			nHeight;
    DWORD       dwVpStyle;
    BOOL		bIsActivated;

    BOOL        bsetCapture;
    BOOL        hideCurosr;
    
    BOOL        clipCursor;


    char        szError[64];

};


//---------------------------------------------------------------------------------------
// array of vtxes
struct RenderVx
{
    RenderVx():_v(0),_n(0),_b(0),_c(0),_sz(0){_u[0]=_u[1]=_u[2]=_u[3]=0;}
    RenderVx(int sz, DWORD what):_v(0),_n(0),_b(0),_c(0),_sz(0){
        _u[0]=_u[1]=_u[2]=_u[3]=0;
        _what = what;
        _sz = sz;
        _v  = new V3[sz];
        if(what & VX_TX1)       _u[0] = new UV[sz];
        if(what & VX_TX2)       _u[1] = new UV[sz];
        if(what & VX_TX3)       _u[2] = new UV[sz];
        if(what & VX_TX4)       _u[3] = new UV[sz];
        if(what & VX_NORMAL)    _n  = new V3[sz];    
        if(what & VX_BINORM)    _b  = new V3[sz];    
        if(what & VX_COLOR)     _c  = new CLR[sz];
    }

    ~RenderVx(){
        delete[] _v;
        delete[] _n;
        delete[] _b;
        delete[] _c;
        delete[] _u[0];
        delete[] _u[1];
        delete[] _u[2];
        delete[] _u[3];
    }
    V3*  getv(int idx){return &_v[idx];}
    UV*  getu(int idx, int layer){ return &_u[layer][idx];}
    V3*  getn(int idx){return &_n[idx];}
    V3*  getb(int idx){return &_b[idx];}
    CLR* getc(int idx){return &_c[idx];}
    
    void  setv(int idx, const V3* v){_v[idx] = *v;}
    void  setu(int idx, const UV* u, int layer){ _u[layer][idx]=*u;}
    void  setn(int idx, const V3* v){_n[idx]=*v;}
    void  setb(int idx, const V3* v){_b[idx]=*v;}
    void  setc(int idx, const CLR* c){_c[idx]=*c;}

    V3* _v;
    V3* _n;
    V3* _b;
    CLR* _c;
    int _sz;
    DWORD _what;
    UV* _u[4];

};



//----------RENDERER PLUGIN INTERFACE---------------------------------------------------------
typedef int (WINAPI *PR_C)();
class ISystem;
class NO_VT Irender  
{
public:
    Irender(){TRACEX("Irender\r\n");};
	virtual ~Irender(){TRACEX("~Irender\r\n");	};

    virtual BOOL CreateRender(const ISystem* pe, RndStruct* prndStates, UINT* gRconsts, HWND useThis=0)=0;
    virtual BOOL SetSystem(const ISystem* pe)=0;
    virtual void Destroy()=0;
    virtual HDC  Hdc()=0;
    virtual HWND Hwnd()=0;
    virtual void Swap()=0;
    virtual void SwitchMode()=0;
	virtual void ClearColor(REAL, REAL, REAL, REAL)=0;
    virtual int  UseFontBitmaps(HFONT hf, int a, int b, int c)=0;
	virtual DWORD Blend(DWORD rCtx)=0;
    virtual void UnBlend(DWORD context)=0;
    virtual void UnBlendv()=0;
		
    virtual void SelectTrMattrix(const Pos* pos, DWORD what)=0;
	virtual void ReleaseTrMattrix()=0;
    	

    virtual void SetArray(const RenderVx* pxyz)=0;
	virtual void ResetArray(const RenderVx* pxyz)=0;

    virtual void SetArrayPtrs(const Vtx* pxyz, int what)=0;
	virtual void ResetArrayPtrs(int what)=0;
    virtual void DrawArrays(int nStart, int vxCnt, DWORD howWhat)=0;
   	
    virtual PR_C Gpa(const char* fn) = 0;
    virtual Htex GenTexture(const char* texFile, DWORD creaFlags)=0;
    virtual Htex ReGenTexture(const Htex& tex, int x, int y, int bpp, const BYTE* pBuff, DWORD creaFlags)=0;
	virtual Htex GenTexture(int x, int y, int bpp, const BYTE* pBuff, DWORD creaFlags)=0;
    virtual void AlterTexture(const Htex& itex, const BYTE* psub, int s, int t, int u, int v)=0;
    virtual void SetCanonicalState(void)=0;
    virtual DWORD BindAllTextures(const Htex* pTxStg, DWORD comb)=0;
    virtual DWORD BindTexture(const Htex& txStg,  int stage)=0;
	virtual void UnBindTexture(const Htex& txStg, int stage)=0;
    virtual void DisableTextures(BOOL force=FALSE)=0;    
    virtual void RemoveAllTextures()=0;
	virtual void RemoveTexture(UINT index,BOOL=TRUE)=0;
    virtual void RemoveTextures(const UINT* ptex, int count)=0;
    virtual void  SetCurrent()=0;
    virtual DWORD BindTex1(const Htex*)=0;
    virtual DWORD BindTex2(const Htex*, const Htex* )=0;
    virtual DWORD BindTex3(const Htex*, const Htex*,const Htex*  )=0;
    
    virtual void Render(int font, REAL x, REAL y, REAL z, const char* text, const CLR& clr)=0;
    virtual void Render(const Pos* pOp, DWORD trCtx, DWORD dCtx, const Htex* txCtx,  DWORD comb, const Vtx* pVtx, int vxCnt, DWORD how)=0;
    virtual void Render(const Pos* pOp, DWORD trCtx, const Htex* txCtx , DWORD comb,  const Vtx* pVtx, int vxCnt , DWORD how)=0;
    virtual void Render(const Pos* pOp, DWORD trCtx, const Vtx* pVtx, int vxCnt , DWORD how)=0;
    virtual void Render(DWORD dCtx, const Htex* txCtx, DWORD comb, const Vtx* pVtx, int vxCnt , DWORD how)=0;
    virtual void RenderFace(const Htex* dwctx, DWORD comb, const Vtx* pvx, int count, DWORD how)=0;
	virtual void Render(const Vtx* pVx, int first, int  vxCnt, DWORD how)=0;
    virtual void Render(const BYTE* pV, int start, int count, DWORD what)=0;
    virtual void Render(int strps, int grps, int* prims, V3* vxes, V3* norms, UV* uvs1, UV* uvs2, int* idxes)=0;
    virtual void RenderFontList(REAL x, REAL y, const char* text, int fontlist, const CLR& color)=0;
    virtual void RenderFontList3(const V3& pos, const char* text, int fontlist, const CLR& color)=0;
    virtual void TextOut(int fontlist, REAL xpos, REAL ypos, const char* text, const CLR& color)=0;
    virtual void EnableStencil(long how)=0;
    virtual void MatrixMode(int m)=0;
    virtual void RenderVertex(const Vtx* pvtci, DWORD dww=0xF)=0;
    virtual void Vertex2(REAL x, REAL y)=0;
	virtual void Vertex3(REAL x, REAL y, REAL z)=0;
	virtual void Texture2(REAL u, REAL v)=0;
    virtual void DbgLine(const V3& a,const  V3& b)=0;
	virtual void DeepTest(BOOL enable)=0;
	virtual void EnableBlend(BOOL enable)=0;
    virtual void EnableFog(REAL fnear, REAL ffar, REAL dens, const CLR& color)=0;
    virtual void BlendFunc(int a, int b)=0;
    virtual void AlphaFunc(int a, int b)=0;
    virtual void Color4(char r, char g, char b, char a)=0;
    virtual void Color(CLR& r)=0;
    virtual void SetNormal(const V3& v)=0;
    virtual void SetWndMinimizeFlag(BOOL bEnable)=0;
    virtual void RefreshModelView(REAL ffov)=0;
    virtual void Clear(int quakeTrick=0)=0;
    virtual void SetMatrix(const M4& m)=0;
    virtual void MulMatrix(const M4& m)=0;
    virtual void SetMaterial(UINT fb, UINT what, REAL r)=0;
    virtual void SetMaterials(UINT fb, UINT what, REAL* r)=0;
    virtual void SetViewMtx(const M4& m)=0;
    virtual void SetGamma(const REAL* gamma, const REAL* contrast, const int* ilum)=0;
    virtual UINT* GetPrimitives()=0;
	virtual void CleanTexCache(int i=-1)=0;
    virtual void Push()=0;
    virtual void Pop()=0;
    virtual void GetViewMatrix(REAL* m)=0;
	virtual void LoadIdentity()=0;
    virtual void PolygonMode(UINT what,UINT mode)=0;
    virtual void CullFace(UINT what)=0;
    virtual void Enable(UINT what)=0;
    virtual void Disable(UINT what)=0;
    virtual void Rotate(const V3& xyz)=0;
    virtual void RotateTo(const  V3& xyz, REAL angle)=0;
    virtual void Scale(const V3& xyz)=0;
    virtual void Translate(const V3& xyz)=0;
    virtual void Rotatef(REAL, REAL, REAL, REAL)=0;
    virtual void Scalef(REAL, REAL, REAL)=0;
    virtual void Translatef(REAL, REAL, REAL)=0;
    virtual void Begin(UINT what)=0;
    virtual void BeginPrim(UINT what)=0;
    virtual void End()=0;
	virtual UINT Compile(const Vtx* pm, int count)=0;
    virtual void NewList(int list)=0;
	virtual int  GenLists(int num)=0;
    virtual void EndNewList()=0;
    virtual void DeleteList(UINT id,UINT many=1)=0;
    virtual void CallList(UINT ID)=0;
    virtual void CallLists(UINT a, UINT b, const void* ptr)=0;// states
    virtual void EnableLighting(BOOL bEnable, DWORD c)=0;
    virtual void EnableRenderMaterialLighting(DWORD amb, BOOL b)=0;
    virtual void SetLights(const RenderLight** pl, int ncount);
    virtual int  GetMaxHwLights()=0;
    virtual void SetMaterial(const RenderMaterial& mat);
    virtual const char*   GetExtensions(DWORD b)=0;
    virtual RndStruct* GetStruct()=0;
    virtual BOOL InitShader()=0;
    virtual void SetTransform(const Pos* pos, DWORD dCtx )=0;
	virtual void ResetTransform()=0;

};


#endif //__IRENDER_H__

