#ifndef __POLY_H__
#define __POLY_H__

//---------------------------------------------------------------------------------------


#include "basecont.h"
#include "TexAdapter.h"
#include "BaseEntity.h"
#include "..\\_include\\BspFIleStr.h"

// Structures passed between plugins and main application
//---------------------------------------------------------------------------------------
// internals
#define	POLY_FROM_CUTB	    0x1
#define	SPLITTER_POLY	    0x2
#define	POLY_DELNAILED      0x4
#define	POLY_DELETED	    0x8
//gap
#define POLY_PORTAL0        0x20
#define POLY_PORTAL1        0x40
#define	POLY_PROCESSED		0x80
#define POLY_HIDDEN         0x100
//
#define POLY_SEP_LMAP       0x400       // trat lmap individually for this poly 
#define POLY_DIRTY          0x800
#define POLY_EXTRUDED       0x1000      // allows rotation and moving cose any vertex of
#define POLY_TERRAIN        0x2000      // allows rotation and moving cose any vertex of
#define POLY_NOPROJ         0x4000
#define POLY_INSERTED       0x10000

//---------------------------------------------------------------------------------------
INLN BOOL IsOrtogonal(const V3& v)
{
	return v.x==1 ||v.y==1||v.z==1||v.x==-1||v.y==-1||v.z==-1;
}

//---------------------------------------------------------------------------------------
// holds the texture coord transformation applyed in the editor only
struct TexCoords
{
	TexCoords& operator=(const TexCoords& tc){
        if(this != &tc){
            ::memcpy(this,&tc,sizeof(TexCoords));
        }
		return *this;
	}
	bool operator==(const TexCoords& tc){
		return !memcmp(this,&tc,sizeof(TexCoords));
	}

	int 	rotate;
    REAL	scalex;
    REAL	scaley;
    REAL	shiftx;
    REAL	shifty;
    long	flipV;
	long	flipH;
	REAL	b;
	REAL	c;
	REAL	d;
	REAL	e;
};

//---------------------------------------------------------------------------------------
#define	SIZE_0	    0x0
#define	SIZE_N	    0x1
#define	SIZE_S	    0x2
#define	SIZE_E	    0x4
#define	SIZE_W	    0x8
#define	SIZE_MOVE	0x10

//---------------------------------------------------------------------------------------
#define	SIZE_SW	(SIZE_S|SIZE_W)
#define	SIZE_SE	(SIZE_S|SIZE_E)
#define	SIZE_NW	(SIZE_N|SIZE_W)
#define	SIZE_NE	(SIZE_N|SIZE_E)
#define SAFE_TIDX 3     // 3th tex stage
//---------------------------------------------------------------------------------------
// extended vertex that allows selection. reuses the _cmd because all will be POLYGONS
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
class Poly;
class Vtx2 : public Vtx
{
public:
    Vtx2(){_pPoly=0;_extra1=0;}
	Vtx2(V3& v){_xyz=v;_pPoly=0;_extra1=0;}
    Vtx2(const Vtx2& v):Vtx(v){
		_tcCrea = v._tcCrea;
		_pPoly  = v._pPoly;
        _extra1 = v._extra1;
	}
    ~Vtx2(){
        ;
    }
    INLN  Vtx2& operator = (const Vtx2& r){ 
        if(this != &r){
                _xyz = r._xyz;
                _nrm = r._nrm;
	            _uv[0] = r._uv[0];
                _uv[1] = r._uv[1];
                _uv[2] = r._uv[2];
                _uv[3] = r._uv[3];
		        _rgb   = r._rgb  ;
		        _user  = r._user ;	 

            _tcCrea = r._tcCrea;
		    _pPoly  = r._pPoly;
            _extra1 = r._extra1;
        }
        return *this;
	}
    void  Select(Poly* pB){_pPoly = pB;}
    BOOL  IsSelected(){return _pPoly!=0;}
    
    int		_index;		// just for final save is kept here
    UV		_tcCrea;
	Poly*	_pPoly;		// !=0 whenb vertex is selected
	DWORD	_extra1;
};
typedef vvector<Vtx2>             Vertexes;
typedef vvector<Vtx2*>            PVertexes;
typedef vvector<Vtx2>::iterator   PVertex;
typedef vvector<Vtx2*>::iterator  PPVertex;

//---------------------------------------------------------------------------------------
class Brush;
class __declspec (novtable) Poly : public Plane, public TexCarrier, public CanProps
{
public:
    int             _idxSplitter;   // temp durring CSG
    int             _idxParent;     // temp durring CSG
	int             _planeIdx;      // bsp plane index
    int             _planeIdxFinal; // final index BSP prior to save BSP    
	int				_fragments;     // number of fragments (CSG)
    int             _use2splitidx;
    BOOL            _selected;      // poly is selected
    Box             _box;       // poly bounding box
	TexCoords		_texcoord;  // holds  the texture coord transformation applyed in the editor only
    LmInfo          _lmInfo;    // light  map index size, flags and axes
    Htex            _ilmTex;    // used only in editor for light map index
    Htex            _nonProjTex;
    Brush*          _pBrush;     // brush this poly is on   
    Vertexes        _vtci;	    // vertexex array
    REAL            _area; 
    void*           _pUsrData;   // bsp node Idx;

    UV              _tanimA;    // texture animation UV
    UV              _tanimB;    // texture animation UV
    V3              _texV1;     // not used
    V3              _texV2;     // not used

    DWORD           _polyflags;     // polygon flags (in editor)
    DWORD           _polyflags2;    // extra flags
    DWORD           _polyprops;     // polygon properties in BspFIleStr.h
    
    CLR             _colorS;   // spec color MATERIAL
    CLR             _colorD;   // diffuse color MATERIAL
    CLR             _colorE;   // emmisive color MATERIAL
    REAL            _shiness;  // shiness
    REAL            _bump;
    REAL            _friction;
    
    
    static  int    _TexCopy; 
    static  V3     __Max;
public:
    Poly();
	~Poly();
            Poly(const Poly& p);
    bool    operator<(const Poly& p){return _area < p._area;}
    bool    operator>(const Poly& p){return _area > p._area;}
    friend bool   operator>(const Poly& p, const Poly& q){return p._area > q._area;}
    void    Clear();
    void    Create(int points, V3* pPoints, Brush* pBrush);
    void    Create(int points, Vtx2* pPoints, Brush* pBrush);
    void	Transform(M4 m);
    void    GetWorldPos(M4& m, Poly& pr);
	void	ApplyNewTexCoord();
    void    Recalc();
    BOOL    ContainPoint(V3& pi);
    void    Reverse();
    void    SnaptoPoint(int ammnt);
    void    RotateTC(int steps=0, int ti=1);
    void    RotateTCRad(REAL grads, int ti=0); 
    void    ResetTexCoords(int ti=-1);
    void    DeSelect();
    void    Select();
	void	SortVertexes();
	void	InitCalcTC(int ti=-1);
    void    CopyTc0InAll();

	void	RescaleTexCoord(BOOL bNew, int ti=0);
    void    ReApplyTexture(DWORD creaFlags);
    BOOL    IsSelected(){return _selected;}
    void    CopyProps(Poly& p);
    BOOL    IntersectsPoly(Poly& p2);
    void    Split(Plane& plane, Poly& a, Poly& b, BOOL bAllways=FALSE);
    void    Clip(Poly& plane, Poly& p);
    REL_POS Classify(Plane& plane);
	int     Classify(V3& point);
    V3      GetCenter();
    REAL    GetAreea();
	BOOL	IsSplitter(){return (_polyflags & SPLITTER_POLY);}
	void	SetSplitter(){_polyflags |= SPLITTER_POLY;}
    Poly&   operator=(const Poly& p);
	void    CalcNormal();
    void    FlipTC(char cd);
	Poly&   operator<<(Vtx2& v);
	Poly&   operator<<(V3& v);
	BOOL    operator == (const Poly& p);
    BOOL    IsInnerPoint(V3& pi);
    BOOL    IsDeplanetized();
    void    BuildTexEdges(V3& v0, V3& v1 ,V3& v2);
    void    BuildTexAxes(V3& v0, V3& v1 );
    int     TrifanIt(vvector<Poly>&   fragments);
    int     Triangulate(vvector<Poly>&   fragments);
    int     AddVertex(const V3 &vertex, vvector<Poly> &fragments);
    size_t  GetVxIndex(const Vtx2& vtx);
    void    MinMaxUV(int layer, UV& minUV,UV& maxUV, V3* vnorm = 0);
};


BOOL    PolyTouchesPoly(Poly* a,Poly* b);

INLN    int GetBrushes(vvector<Poly*>& polys, set<Brush*>& brushes)
{
    FOREACH(vvector<Poly*>, polys, pp)
    {
        brushes.insert((*pp)->_pBrush);
    }
    return brushes.size();
}

typedef vvector<Poly>             Polys;
typedef vvector<Poly>::iterator   PPoly;

typedef vvector<Poly*>             PPolys;
typedef vvector<Poly*>::iterator   PPPoly;

//--------------------------------------------------------------------------
struct PolyEdge 
{
      Vtx2  *_edge[2];  //points in 2 vxes of the poly
      Poly  *_poly;
};


#endif //__POLY_H__