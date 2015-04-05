//---------------------------------------------------------------------------------------

#ifndef __BRUSH_H__
#define __BRUSH_H__

//---------------------------------------------------------------------------------------
#pragma warning (disable: 4786)
#include "poly.h"
#include "minibsp.h"
#include "BaseEntity.h"
#include "..\\_include\\BspFIleStr.h"


//--------------------------------------------------------------------------------------
typedef enum _TEX_MAP
{
    TM_LIN,
    TM_CYL,
    TM_SPH,
	TM_ENV,
}TEX_MAP;

#define BRSH_CLEAN (BRSH_DIRTY|BRSH_NEW)
//--------------------------------------------------------------------------------------
typedef enum _P_TYPE{  
                E_CUBE=1, 
                E_CONE, 
                E_CYL, 
                E_SPHERE, 
                E_SHEET,
                E_STAIRS,
                E_TERRAIN,
                E_PREFAB}P_TYPE;
// struct Iniaial flags of this brush creation
struct  CreaFlags
{
    REAL    _thick;
    DWORD   _brushflags;
    int     _vstrips;
    int     _hstrips;
    TEX_MAP _tm;
    P_TYPE  _t;
};

//---------------------------------------------------------------------------------------
class	Brush  ;
typedef vvector<Brush*>            PBrushes;
typedef vvector<Brush*>::iterator  PPBrush;

//---------------------------------------------------------------------------------------
class CMotion ; 
class Scene;
class Brush  : public BaseEntity, public CanProps
{

public:
	Brush*   _pResult;
    Brush*   _pUnrotated;
	V3		 _t;
	V3		 _r;
	V3		 _s;
	V3		 _f;	//shift
    M4       _mt;
	int		 _unicID;
	Polys	 _polys;
	Box      _box;
    BOOL     _selected;
	BOOL	 _copyed;
    DWORD    _brushflags;
    DWORD    _brushprops;   
    V3       _physicsA;         // stickness, rejection etc...
    V3       _rotSteps;         // rotation 
    V3       _physicsD;
    CMotion* _pMotion;        // index to dynaics movement equation (details only)

    PBrushes  _pCutBrshes;
	PBrushes  _pSldBrushes;
    PBrushes  _pProjBrushes;
	DWORD	  _polySec;
    int       _group;            // group them by user design time
	REAL	  _thikness;         
    TEX_MAP   _texmap;    	
    CreaFlags _cf;
	Box       _cbox;			 // box of decupated area from a solid. Updated ech cut
    
    void*     _pUsrData;
	int		  _nZoneNo;
    LONG      _intask;

public:
    Brush();
	Brush(const Brush& b);
    virtual ~Brush();
    // make them
    void    FindNormal(Vtx2& tv, Polys& polys);
    BOOL    CopyDiametral(const Brush& b);
    void    MakeStairs(V3& dims, int radstrips, int vstrips, REAL thikness, TEX_MAP tm = TM_LIN);
    void    MakeCone(REAL rad, REAL height, int radstrips, int vstrips, TEX_MAP tm = TM_LIN);
    void    MakeCylinder(REAL rad, REAL height, int radstrips, int vstrips, TEX_MAP tm = TM_LIN);
    void    MakeCube(V3& dims, TEX_MAP tm = TM_LIN);
    void    MakeSheet(V3& dims, int vstrips, int hstrips, TEX_MAP tm = TM_LIN);
    void    MakeSphere(REAL dims, int vstrips, int hstrips, TEX_MAP tm = TM_LIN);
    void    MakeTerrain(V3& meshDim, int dimX, int dimY, BYTE* pBuffer);
    void    CalctexCoord(int idx);
    void    ModifyTexCoord(int idx);
    void    CalcVxNormals();
    int     CalcVxTask();
    REAL    GetVolume();
	void	StartSecGrp(REAL t){_polySec = _polys.size();_thikness=t;}
    Brush&  operator<<(const Brush& b);
    Brush&  operator=(const Brush& b);
    Brush&  operator<<(Poly& p);
    Brush&  operator<<(vvector<Poly>& p);
    void    RotateTCOneStep();
    Brush&  GetWorldPos();
	Brush&	GetResult();
	Brush&	GetPrimitive();
    BOOL    IsDisabled(){return _brushflags & BRSH_DISABLED;};
    void    RecalcRotation();
    void    Reinit();
    void    GlTransform();
    void    ResetTexCoords(int ti=-1);
    void    TriangulateDeplanetizedFaces();
	void    Snap2grid(REAL gs,BOOL m);
	void	SBoxTr(Box& newBox);
    void    Reverse();
    void    HalloUnHallo();
    M4&     GetTrMatrix();
    void    DeSelect(){_selected=FALSE;}
    void    Select(){_selected=TRUE;}
    BOOL    IsSelected(){return _selected;}
    BOOL    IsSpecial();
	void	MakeTm();
    void    Recalc(BOOL withnormals=FALSE);
    void    Replan();
	void	GetTxPolysCopy(Polys& pl);
	void	GetTxReversedPolysCopy(Polys& pl);
    void    Clear();
    void    Mirror();
    BOOL    IsSealed();
    void    Seal();
    void    AddCutter(Brush* pCutter);
    void    RemoveCutter(Brush* pCutter);
    void    AddBrush(Brush* pBrush);
    void    AppendPrim(Brush& brush);
    void    RemoveBrush(Brush* pBrush);
    void    AddProjection(Brush* pCutter);
    void    RemoveProjection(Brush* pCutter);
    BOOL    ApplyCuts();
    BOOL    Split(Brush& splBrush, Brush& frontB, Brush& backB);
	void    Equal(const Brush& b,BOOL polys=TRUE);
	void	Move(V3& v);
	void	Scale(V3& v, V3& t,REAL zoom);
    void	ScaleTr(V3& v, V3& t);
	void	Raise(V3& v, REAL zoom);
    BOOL    LimitScaleVector(V3& v,REAL);
	void	Rotate(V3& v, BOOL aroundOrigin=FALSE);
	void	SetPos(V3& center);
	void	SetSize(Box& extends);
	void    FlagFaces(DWORD f);
	void	Deleteface(Poly* p);
    BOOL    HasPolys(){return _polys.size()>0;}
	BOOL    IsSolid(){return _brushflags & BRSH_SOLID;}
    BOOL    IsTouched(){return this != _pResult;}
	void	DeletePolys();
    BOOL    HasFlag(DWORD f);
    BOOL    IsCut();
    BOOL    IsProjector();
    BOOL    IsSplitter();
    BOOL    IsZone();
	BOOL    IsSelection();
    BOOL	IsDirty(){return _brushflags & BRSH_DIRTY;};
	void	Dirty(BOOL b){if(b) _brushflags|=BRSH_DIRTY; else  _brushflags&=~BRSH_DIRTY;};
    BOOL    HasMotion(){return _pMotion!=0;}
    Poly&   FirstPoly(){return _polys[0];}
    void    TriFanIt();	
    int     Triangulate();	
    void    CopyPolysFrom(Brush& br);
	Poly&	GetTerrainPoly(int x, int y, BOOL tridx);
    void    AddPoly(Poly& p);
    void    AddPolys(vvector<Poly>& polys);
    void    ErasePoly(Poly& p);
    void    UpdateCutsTouches();
	void    UpdateSolidsTouches();
    void    UndoLastTr();
    void    ExplodeInConvexBrushes(PBrushes& pResults);
    BOOL    IsConvex();
    int     SPlitByPlane(Plane& p0, Brush* pNewA, Brush* pNewB);
    int    &ZoneIdx(){return _nZoneNo;} // fo zone hold here this index
    DWORD  &VisData(){return _polySec;} // fo zone hold here this index
    BOOL    RecalcBoxAndNormals(BOOL withnormals);
    int     GetPolyCount(DWORD flags, DWORD props = (DWORD)-1);
    Poly *  GetNextPoly(class Poly *);
    Poly *  GetPrevPoly(class Poly *);
private:
	void	StoreInitiaTexCoord();
    void    RescalePolys(int s, int e, V3& sc, V3& tr, Box& b,Box& b1);
	void	CalcEnvTc(int stage = 0);
    void    R_Explode(PBrushes& pResults);
	
public:
	static int GID;
    
private:
    void    AlocateResultPtr();
	void	DeleteResultPtr();
	void	RecalcTM();
	void	Shift(V3& v);
	
};


#endif //