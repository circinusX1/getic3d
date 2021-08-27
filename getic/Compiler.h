#ifndef __CCOMPILER_H__
#define __CCOMPILER_H__

#include "baseutils.h"
#include "basethread.h"
#include "Brush.h"
#include "Bsptree.h"
#include "PortalPrc.h"
#include "Pvsprc.h"
#include "Flowpath.h"
#include "basethread.h"
#include "Lmproc.h"
#include "Lmproc2.h"
#include "bsphook.h"
#include "Tertree.h"

//---------------------------------------------------------------------------------------
#define REND_BSP        0x1
#define REND_PORTALS    0x2
enum {
    CUT_PORTAL_BY_NODE,
    CUT_PORTAL_BY_PARENTS,
    CUT_PORTAL_BY_LEAFS,
};

//---------------------------------------------------------------------------------------
struct SBytes
{
    typedef enum _SType{S_IMAGE, S_SOUND, S_SCRIPT, S_UNKNOWN}SType;
    SBytes():ptr(0){}
    ~SBytes(){delete[] ptr;}
    BYTE*   ptr;
    long    size;
    long    offset;
    SType   stype;
};

//---------------------------------------------------------------------------------------
class z_ed3View;
class Scene;
class CDlgCompProgress;
class Compiler  : public OsThread
{
public:
    Compiler();
    virtual ~Compiler(){Clear();};
    
    BOOL     Compile(Scene& scene);
    void     CommitChanges();
    BOOL     Save(const char* pFIleName);
    BOOL     ExportBSP(const char* pFIleName);
    BOOL     PerformHSR(PBrushes* pBrushes );
    void     Clear();
    BOOL     Done(){return _done;}    
    void     Render(z_ed3View* pV, size_t what);    
    static   BOOL Union(PBrushes& pPrimitives, Brush& brush, BOOL sgow=0);
    void     Break(long st=1){_break=st;};
    long     HasBreaked(){return _break;}
    void     FromRegistry(BOOL bRead);
    void     Export(const char* bspFile);
	size_t	 DoLightMaps();
    BOOL     CompileTerrain();
    Brush   *GetZone(Box& box, int* pindex=0);
    Brush   *GetZone(V3& vx);

    IMPLEMENT_TP(Compiler, MainFoo);

private:
    void    _ManuaIntersectZones();
    int     _CheckBrushes(BOOL hasTerrain);

    void    _RecutSelectedBrushes();

    BOOL    PerformESR(PBrushes* pbrushes);
    void    ReevaluateBrush(Brush* pBrush);
    void    R_FloodBSP(CBspTree&  tmpTree, PortalPRC& tmpPPrc, int iLeaf, BYTE* wetBytes);
    void    LinkDetailsToMainBsp();
    void    LinkDetailsToMainSky();
    void    SortBrushes(PBrushes& pBrushes);
    BOOL    PerformLmapCalculation(CBspTree* pMainModel);
    
	void	EstablishTrigerLinks(Brush* pBrush, int trrIndex);
    void    ZoneAreea(SceItem*);
    void    ZoneAreea(CBspTree*, BOOL isMain);
    void    ZoneMainBSP(CBspTree*);
    void    AutomateZoneVisibility(CBspTree*);
    void    Clean();

private:    //saving
    void    WriteHeader(FlStream& fw);
    void    WriteSceneInfo(FlStream& fw);
    void    WriteSkyDomInfo(FlStream& fw);
    void    WriteScripts(FlStream& fw);
    void    WriteScriptsData(FlStream& fw);
    void    WriteImages(FlStream& fw);
    void    WriteSounds(FlStream& fw);
    void    WriteZones(FlStream& fw);
    void    KeepSecttions(size_t cs, size_t cend, BTF_Section& s, FlStream& fw);
    void    UpdateServerIndex();
	void	WriteXML(const char *xmlFname, 
                     const char* bspNameFinalZ, 
                     const char* subDir, 
                     vvector<tstring>& dependencies,
                     const vvector<size_t>& lengths);
    void    WriteGameInit(FlStream& fw);
    void    WriteLMInfoAndData(FlStream& fw);
    void    WriteVertexes(vvector<V3>& vp, FlStream& fw);
	void    WritePvs(FlStream& fw)    ;
    void    WriteBsps(map<int,int>& texids, FlStream& fw);
	void    WritePortals(FlStream& fw);
	void	WriteTexInfo(map<int,int>& texids, FlStream& fw);
	void    WriteSndInfo(map<int,int>& sndInfo, FlStream& fw);    
	void    WriteItemsInfo(map<int,int>& sndidx, map<int,int>& texids, FlStream& fw);
	void    WriteV3s(vvector<V3>& vertexPool,FlStream& fw, size_t sect);
	void    WritePlanes(vvector<Plane>& allPlanes, FlStream& fw, size_t sect);
	void    WriteNodes(vvector<CMiniNode*>& allNodes, FlStream& fw, size_t sect);
	void    WriteLeafs(vvector<CLeaf*>& allLeafs,   FlStream& fw, size_t sect);
	void    WriteModels(FlStream& fw, size_t sect);
	void    WritePolys(vvector<Poly*>& allPolys,
                             map<int,int>& texids,
                             FlStream& fw, size_t sect);
    void    WriteMotions(FlStream& fw);
	void    WritItemsCathegory(FlStream& fw);
    void    WriteTerrain(FlStream& fw, map<int,int>& texids);
    void    WriteDetails(FlStream& fw);
    void    WriteRandomBits(FlStream& fw);
    
public:
    vvector<CBspTree*>  _bTrees;
    TerTree             _terrTree;
    CFlowPath           _flowPath;
    PortalPRC           _portPrc;
    LMProc              _lmProc;

    PVSPrc              _pvsPrc;
    Scene*              _pScene;
    BOOL                _done;
    
    BOOL                _bAutoDetailSmall;
    BOOL                _bAutoDetailPolys;
    BOOL                _esrErr;
    BOOL                _removeOnePolyLeaf;
    BOOL                _bCompressPvs;

    BOOL                _bAABBTreeCsgTer;
    BOOL                _bEsr;
    int                 _bPvsMethod;      
    BOOL                _bGeticBSP;//         = AfxGetApp()->GetProfileInt("BSP","GETICBSP",1);
    BOOL                _bSnapMainBsp;
    BOOL                _bZones;
    long                _break;
    BOOL				_bSaveImages;
    int                 _lmapIntens;
    long                _maxFacesLeaf;
	BOOL				_bSaveCats;
    BOOL                _bPolySortCrit;
    BOOL				_bSavePortals;
    BOOL                _bMergeNarrowLeafs;
    BOOL				_bSaveSkyDom;
    BOOL                _bSaveBigTerrain;
    BOOL                _bSaveBigTerrainBSP;
    BOOL                _bAvoidNonOrt;
    BOOL                _bUseUserPriority;
    vvector<SBytes*>    _randombytes;

    BOOL                _bConvexEarly;
    BOOL                _bLightMaps;
    BOOL                _lmpowof2; 
    BOOL                _lmConst;
    BOOL                _b3SampleLM;
    BOOL                _bSaveFinalBSP;
    BOOL                _szipit;
    REAL                _narowLeaf;
    int                 _portalCutFlag;
	BOOL                _bExportFile;
    
    CString             _exBspPlugin;
    CString             _finalBSpFileName;
    int                 _lmapMaxSize;
    REAL                _lMapLumelSz;
    BOOL                _bLmPatch;
    int                 _balance;
    int                 _curStep;
    int                 _percentage;
    long                _inThread;
	long				_polysStats;
	long				_cutsStats;
    BOOL                _errDetected;
    Brush*              _zones[32];
    int                 _zonecount;
    size_t               _timeStart;
    
public:
    static Compiler* PCompiler;
};

//---------------------------------------------------------------------------------------
inline void NOTIFY_FRAME(size_t m, WPARAM a, LPARAM b)    
{
    extern HWND            _Hmain;// = m_hWNd;
    for(int i=0;i < 8;i++)
    {
        if(::PostMessage(_Hmain, m,a,b))
        {
            break;
        }
        Sleep(1);
    }
}


//---------------------------------------------------------------------------------------
#define COMP()          Compiler::PCompiler

#endif // !__CCOMPILER_H__
