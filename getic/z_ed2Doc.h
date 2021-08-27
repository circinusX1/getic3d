// z_ed2Doc.h : interface of the CZ_ed2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __ED2_DOC_H__
#define __ED2_DOC_H__

#include <list>
#include "zedtypes.h"
#include "Scene.h"
#include "camera.h"
#include "z_ed2View.h"
#include "DlgNewBrush.h"
#include "DlgNewBrush.h"
#include "Bsptree.h"
#include "Compiler.h"
#include "Motion.h"
#include "DialogEvalDets.h"
#include "Undo.h"

class SceItem;
class CLightBulb;
class CZ_ed2View;
class z_ed3View;
class IGeticPlug;


#define BIT_SPECULAR    0x4
#define BIT_DIFFUSE     0x2
#define BIT_AMBIENT     0x1

//---------------------------------------------------------------------------------------
class CZ_ed2Doc : public CDocument
{
public: // create from serialization only
	CZ_ed2Doc();
	DECLARE_DYNCREATE(CZ_ed2Doc)

// Attributes
public:
    static CZ_ed2Doc*   PDoc;
// Operations
public:
    LRESULT OnCompileDone(WPARAM,LPARAM);
    void Repos20();
	void EndEditor();
    BOOL StartEditor();
    BOOL InitGL(int i3d,HDC hDC, HGLRC& hrc);
    void SetShift(CZ_ed2View* pSrc);
    void SetZoom(CZ_ed2View* pSrc);
	void ReposCamLight(size_t enableBits, int availLght, REAL offset=128.0, CLR cdif=ZBLACK);
    void DisableAll(BOOL bd);
	void RayIntersectBrush(V3& s, V3& end,_EL_SEL sel);
    void RayIntersectItems(V3& s, V3& end);
    void SetErrorPoint(V3& v){_errPoint = v;};
    void SetActiveWiew(CWnd* pWnd){_pActiveView = pWnd;};
	size_t SetFaceMode(BOOL resetBlends=FALSE);
	void  SetLightMode(BOOL inv=TRUE);
    void  SbarShowCurMode();
    void  MoveCamera(V3& p);
    
    void  DrawSolidBrush(Brush& rb, 
                          vvector<Poly*>&,
                          vvector<Poly*>&,
                          vvector<Poly*>&,
                          vvector<Poly*>&,
                          vvector<Poly*>&,
                          BOOL usesel=FALSE);
    void DrawSpecialPolygons(z_ed3View *pV,
                             size_t lMaps,
                             vvector<Poly*>& backDraw,
                             vvector<Poly*>& transPolys,
                             vvector<Poly*>& mirrors,
                             vvector<Poly*>& masked,
                             vvector<Poly*>& guround);
    void   DrawRect(Htex& texture, const Box& box);
    Brush* GetLastSelBrush();
    Brush* GetFirstSelBrush();
    BOOL   DeleteBrush(Brush* pBrush, BOOL undo=TRUE);
    void  DeleteBrushMotion(Brush*); 
    BOOL  AddBrushMotion(Brush* _pCurBrush, CMotion* pNewMotion);
    void  AddBrush(Brush* pB);
	void  CameraPointTo(V3& v);
	void  DetachTrigger(Brush* pB);
    void  DrawGridLines();
    void    ProjectTexture(Brush& projBrush);
    void    SelectTextCtx(RECT& rt);
    void    DeSelectTextCtx();
    void    PrintOut(int x, int y, const char *fmt, CLR& clr=ZLIGHTGRAY);
	void    PrintOut(V3& v, char vt, const char* text, CLR& clr/*=ZLIGHTGRAY*/);
    void    PrintCoords(RECT& wrect, FRECT& vrect, V3& pct, char vt, BOOL active, const char* ct, POINT* mc);
    Htex    RecreateAmbLmap();
    CString ResFile(const char* filenme);
    BOOL    DuplicateFile(const char* src, BOOL over = TRUE);
    void    CopyResources(const char* srcFile);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZ_ed2Doc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual void OnFinalRelease();
	virtual BOOL OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnOpenDocument(const char* lpszPathName);
	virtual BOOL OnSaveDocument(const char* lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZ_ed2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
public:
    
	//{{AFX_MSG(CZ_ed2Doc)
	afx_msg void OnUpdateTbLockxyz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBurotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBumove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUScale(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBushcam(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBusnapgrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButselbrush(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBufacesel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuwireframe(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBushowback(CCmdUI* pCmdUI);
	afx_msg void OnTbLockxyz();
	afx_msg void OnMkBox();
	afx_msg void OnBurotate();
	afx_msg void OnBumove();
	afx_msg void OnBUScale();
	afx_msg void OnBushcam();
	afx_msg void OnBusnapgrid();
	afx_msg void OnBureverse();
	afx_msg void OnButselbrush();
	afx_msg void OnBufacesel();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnBuwireframe();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnBushowback();
	afx_msg void DoHSR();
	afx_msg void OnBushowcsg();
	afx_msg void OnUpdateBushowcsg(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnCutBrush();
	afx_msg void OnUpdateCutBrush(CCmdUI* pCmdUI);
	afx_msg void OnTabNextsel();
	afx_msg void OnBuriselow();
	afx_msg void OnUpdateBuriselow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnCut();
	afx_msg void OnUpdateCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void MkCylinder();
	afx_msg void OnEscape();
	afx_msg void OnUpdateMirror(CCmdUI* pCmdUI);
	afx_msg void OnMirror();
	afx_msg void OnEnter();
	afx_msg void OnUpdateEnter(CCmdUI* pCmdUI);
	afx_msg void OnXalign();
	afx_msg void OnUpdateXalign(CCmdUI* pCmdUI);
	afx_msg void OnYalign();
	afx_msg void OnUpdateYalign(CCmdUI* pCmdUI);
	afx_msg void OnZalign();
	afx_msg void OnUpdateZalign(CCmdUI* pCmdUI);
	afx_msg void OnXinter();
	afx_msg void OnUpdateXinter(CCmdUI* pCmdUI);
	afx_msg void OnYinter();
	afx_msg void OnUpdateYinter(CCmdUI* pCmdUI);
	afx_msg void OnZinter();
	afx_msg void OnUpdateZinter(CCmdUI* pCmdUI);
	afx_msg void OnXdim();
	afx_msg void OnUpdateXdim(CCmdUI* pCmdUI);
	afx_msg void OnYdim();
	afx_msg void OnUpdateYdim(CCmdUI* pCmdUI);
	afx_msg void OnZdim();
	afx_msg void OnUpdateZdim(CCmdUI* pCmdUI);
	afx_msg void OnXyzdim();
	afx_msg void OnUpdateXyzdim(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnBusplitable();
	afx_msg void OnUpdateBusplitable(CCmdUI* pCmdUI);
	afx_msg void OnBuredcuts();
	afx_msg void OnBucone();
	afx_msg void OnBustairs();
	afx_msg void OnTabPrevsel();
	afx_msg void OnSelectall();
	afx_msg void OnTabSelnext();
	afx_msg void OnTabSelprev();
	afx_msg void OnCompbsp();
	afx_msg void OnDelface();
	afx_msg void OnBucompile();
	afx_msg void OnUpdateBucompile(CCmdUI* pCmdUI);
	afx_msg void OnViewPortals();
    afx_msg void OnViewByFar();
    afx_msg void OnViewByFarUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOnViewPortals(CCmdUI* pCmdUI);
	afx_msg void OnViewColorLeafs();
	afx_msg void OnUpdateOnViewColorLeafs(CCmdUI* pCmdUI);
    afx_msg void OnViewWhite();
    afx_msg void OnViewWhiteUpdate(CCmdUI* pCmdUI);
	afx_msg void OnSelsameasselface();
	afx_msg void OnAlignRightHit();
	afx_msg void OnAlighLefthit();
	afx_msg void OnAlighBothit();
	afx_msg void OnAlighTophit();
	afx_msg void OnSelectgroup();
	afx_msg void OnSelcurid();
	afx_msg void OnUpdateSelcurid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectgroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelsameasselface(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBureverse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBucsg(CCmdUI* pCmdUI);
	afx_msg void OnLr();
	afx_msg void OnUpdateLr(CCmdUI* pCmdUI);
	afx_msg void OnTb();
	afx_msg void OnUpdateTb(CCmdUI* pCmdUI);
	afx_msg void OnFb();
	afx_msg void OnUpdateFb(CCmdUI* pCmdUI);
	afx_msg void OnCtx2dRotatecw30();
	afx_msg void OnCtx2dRotatecw60();
	afx_msg void OnCtx2dRotatecw90();
	afx_msg void OnCtx2dRotatecw45();
	afx_msg void OnBusheet();
	afx_msg void OnMatchwidth();
	afx_msg void OnSelitem();
	afx_msg void OnUpdateSelitem(CCmdUI* pCmdUI);
	afx_msg void OnViewFullBSP();
	afx_msg void OnUpdateOnViewFullBSP(CCmdUI* pCmdUI);
	afx_msg void OnInsitem();
	afx_msg void OnAddModel();
	afx_msg void OnUpdateAddModel(CCmdUI* pCmdUI);
	afx_msg void OnInspref();
	afx_msg void OnUpdateInsertMesh(CCmdUI* pCmdUI); 
	afx_msg void OnBusphere();
	afx_msg void OnBspcursel();
	afx_msg void OnWpncfg();
	afx_msg void OnCompileroptions();
	afx_msg void OnUpdateCompileroptions(CCmdUI* pCmdUI);
	afx_msg void OnImport();
	afx_msg void OnSelvertex();
	afx_msg void OnUpdateSelvertex(CCmdUI* pCmdUI);
	afx_msg void OnTexrefdlg();
	afx_msg void OnShowselbbox();
	afx_msg void OnTerian();
	afx_msg void OnUpdateTerian(CCmdUI* pCmdUI);
	afx_msg void On2dshape();
	afx_msg void OnUpdate2dshape(CCmdUI* pCmdUI);
	afx_msg void OnReclmaps();
	afx_msg void OnUpdateReclmaps(CCmdUI* pCmdUI);
	afx_msg void OnFileMerge();
	afx_msg void OnUpdateParskin(CCmdUI* pCmdUI);
	afx_msg void OnParskin();
	afx_msg void OnTriangulate();
    afx_msg void OnTriFan();
    afx_msg void OnUpdateTriFan(CCmdUI* pCmdUI);
	afx_msg void OnSametexface();
	afx_msg void OnSamenormface();
	afx_msg void OnInsgameitem();
	afx_msg void OnUpdateInsgameitem(CCmdUI* pCmdUI);
	afx_msg void OnSpline();
	afx_msg void OnUpdateSpline(CCmdUI* pCmdUI);
	afx_msg void OnClosedspline();
	afx_msg void OnUpdateClosedspline(CCmdUI* pCmdUI);
	afx_msg void OnXoy();
	afx_msg void OnUpdateXoy(CCmdUI* pCmdUI);
	afx_msg void OnXoz();
	afx_msg void OnUpdateXoz(CCmdUI* pCmdUI);
	afx_msg void OnZoy();
	afx_msg void OnUpdateZoy(CCmdUI* pCmdUI);
	afx_msg void OnExtrude();
	afx_msg void OnUpdateExtrude(CCmdUI* pCmdUI);
	afx_msg void OnHallounhallo();
	afx_msg void OnUpdateHallounhallo(CCmdUI* pCmdUI);
	afx_msg void OnMerge();
	afx_msg void OnUpdateMerge(CCmdUI* pCmdUI);
	afx_msg void OnProjBrush();
	afx_msg void OnUpdateProjBrush(CCmdUI* pCmdUI);
	afx_msg void OnPermcut();
	afx_msg void OnUpdatePermcut(CCmdUI* pCmdUI);
	afx_msg void OnUndo();
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTervol(CCmdUI* pCmdUI);
	afx_msg void OnTervol();
	afx_msg void OnManTex();
	afx_msg void OnUpdateManTex(CCmdUI* pCmdUI);
	afx_msg void OnToXml();
	afx_msg void OnUpdateToXml(CCmdUI* pCmdUI);
	afx_msg void OnExtractfaces();
	afx_msg void OnUpdateExtractfaces(CCmdUI* pCmdUI);
	afx_msg void OnEvaldets();
	afx_msg void OnUpdateEvaldets(CCmdUI* pCmdUI);
	afx_msg void OnHome();
	afx_msg void OnCreatesb();
	afx_msg void OnUpdateCreatesb(CCmdUI* pCmdUI);
	afx_msg void OnLoadlastbackip();
	afx_msg void OnUpdateLoadlastbackip(CCmdUI* pCmdUI);
	afx_msg void OnShowgrid();
	afx_msg void OnUpdateShowgrid(CCmdUI* pCmdUI);
	afx_msg void OnWbck();
	afx_msg void OnTrpolys();
	afx_msg void OnRepoint();
	afx_msg void OnUpdateRepoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnAlbybox();
	afx_msg void OnUpdateAlbybox(CCmdUI* pCmdUI);
	afx_msg void OnZones();
	afx_msg void OnUpdateZones(CCmdUI* pCmdUI);
	afx_msg void OnViewLeafByLeaf();
	afx_msg void OnUpdateViewLeafByLeaf(CCmdUI* pCmdUI);
	afx_msg void OnViewLightmaps();
	afx_msg void OnUpdateViewLightmaps(CCmdUI* pCmdUI);
	afx_msg void OnViewGraph();
	afx_msg void OnUpdateViewGraph(CCmdUI* pCmdUI);
	afx_msg void OnCamLspot();
	afx_msg void OnFileSaveselAs();
	afx_msg void OnUpdateFileSaveselAs(CCmdUI* pCmdUI);
	afx_msg void OnInsface();
	afx_msg void OnUpdateInsface(CCmdUI* pCmdUI);
	afx_msg void OnCutwithbrush();
	afx_msg void OnUpdateCutwithbrush(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTriangulate(CCmdUI* pCmdUI);
	afx_msg void OnTerianbig();
	afx_msg void OnUpdateTerianbig(CCmdUI* pCmdUI);
	afx_msg void OnSnapvxes();
	afx_msg void OnUpdateSnapvxes(CCmdUI* pCmdUI);
	afx_msg void OnInsvx();
	afx_msg void OnUpdateInsvx(CCmdUI* pCmdUI);
	afx_msg void OnPaintimer();
	afx_msg void OnUpdateCombo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCombots(CCmdUI* pCmdUI);
	afx_msg void OnSeledge();
	afx_msg void OnUpdateSeledge(CCmdUI* pCmdUI);
	afx_msg void OnRepointbrsh();
	afx_msg void OnUpdateRepointbrsh(CCmdUI* pCmdUI);
	afx_msg void OnImpp();
	afx_msg void OnUpdateImpp(CCmdUI* pCmdUI);
	afx_msg void OnExpp();
	afx_msg void OnUpdateExpp(CCmdUI* pCmdUI);
	afx_msg void OnLeafs();
	afx_msg void OnUpdateLeafs(CCmdUI* pCmdUI);
	afx_msg void OnSelnondetbrushes();
	afx_msg void OnDetbrushes();
	afx_msg void OnColor();
	afx_msg void OnUpdateColor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    

public:
    void     Escape(BOOL resetSels=TRUE);
    void     RoundToNear(V3 &v, int snp=16);
    void     TestBackupFile();
    void     SaveTempFile();
	//debug
    void     DrawAxis(V3& org, REAL ex);
    void     InvokePlugIn(const char* dllname, int ncmd);
    BOOL     AddItem(SceItem* pItem);
    void     DelItem(SceItem* pItem);

	void	AddDbgPoly(Poly& p){
		_debugPolys << p;
	}
    BOOL    OnStartMoveRotScale();
    //void    OnStartMoveRotScale(int operation, Brush** ppBrushes, int count);
    //void    OnStartMoveRotScale(int operation, SceItem** ppItems, int count);

    void    OnStartMoveRotScale(int operation, vvector<SceItem*>& items);
    void    OnStartMoveRotScale(int operation, vvector<Brush*>&  brushes);

    void    OnFinishMoveRotScale(int operation, vvector<Brush*>* pBrushes=0);
    BOOL    UpdateTouches(Brush* pPrim, Brush* pMovedBrush, map<Brush*, int>& accList);
    void    UpdateCutsCollisionList(Brush** ppB, int brushes );
    void    SplitBySplitterBrush(Brush* pb);
	void    DeleteUncomittedNewBrushes();
	void    DeleteUncomittedNewItems();
    void    SetViewMode(){_brmode=BR_VIEW;};
    void    GLDrawBox(Box&,BOOL, RGBA );
	void    Draw2DScene(CZ_ed2View* pV);
    void    Draw3DScene(z_ed3View* pV);
    void    Invalidate(BOOL update=FALSE) ;
	void    Invalidate3D(BOOL fromMM=FALSE) ;
    void    DoSelection3D(CPoint& point, z_ed3View* pV, _EL_SEL sel);
	void    DoSelection2D(REAL x, REAL y, CZ_ed2View* pV, _EL_SEL sel);
    void    CreateSelBox(CPoint& pt);
    void    HideSelBox();
    void    Make3dCursorPos(z_ed3View* pV,CPoint& pt, RECT rt, V3& out);
    BOOL    BrushIntersect(V3& vstart, V3& end, int&, int& , int&);
    void    DeselectAll(BOOL killDets=TRUE);
    BOOL    SelectItem(SceItem* pItemFnd, BOOL notyBar=TRUE);
    void    DeselectItem(SceItem* pItemFnd,BOOL notyBar=TRUE);
	void    DeselectBrushes();
    void    DeselectPolys(BOOL desel=1);
    void    DeselectVertexes();
    void    SelectEdge(int brSel, int nPoly, int vxSel);
	void	Recut(vvector<Brush*>* extra=0);
    void    DrawErrorPoint();
    void    DrawTerrain();
    BOOL    HasSomeSelected(){
        return (_pSelBrushes.size()!=0 ||_pSelPolys.size()!=0 || _pSelItems.size()!=0) ;
    }
    void    SelectVertexes(Box& selBox, Brush* ppB);
    void    SelectByBox();
    BOOL    SelectBrush(Brush* pB);
    void    DeSelectBrush(Brush* pB);
    BOOL    SelectFace(Poly* pB, Brush* pb=0);
    void    DeSelectFace(Poly* pB);
    void    DrawCompiledStates(z_ed3View* pV);
    void    DeselectItems(BOOL bNoty=TRUE);

    Brush** GetSelBrushes(int& count){
        count = _pSelBrushes.size();
        if(count)
            return &_pSelBrushes[0];
        return 0;
    }

    SceItem** GetSelItems(int& count){
        count = _pSelItems.size();
        if(count)
            return &_pSelItems[0];
        return 0;
    }

    Vtx2** GetSelVxes(int& count){
        count = _pSelVertexes.size();
        if(count)
            return &_pSelVertexes[0];
        return 0;
    }


    Poly** GetSelPolys(int& count){
        count = _pSelPolys.size();
        if(count)
            return &_pSelPolys[0];
        return 0;
    }

    int HasForms(){
        if(_bShowCsg)
        {
              return _scene.GetBrushes()->size();
        }
        return _scene.GetPrimitives()->size();
    }

    vvector<SceItem*>*  GetItems(){
        return _scene.GetItems();
    }

    PBrushes* GetForms(){
        if(_bShowCsg)
        {
              return _scene.GetBrushes();
        }
        return _scene.GetPrimitives();
    }
    
    Brush* GetForm(int index){
        if(_bShowCsg)
            return _scene.GetBrush(index);
        return _scene.GetPrimitive(index);
    }
    PBrushes* GetPrimitives(){
        return _scene.GetPrimitives();
    }
    BOOL      IsSelBoxOn(){
        return _selBox._brushflags & BRSH_SELVIS;
    }
	void	  SelectTabBrush(int direction, BOOL cumulate);
    void      RotateByAngleCw(REAL rad);
    void      SetSelMode(EL_SEL selmode ){_selmode = selmode ; Invalidate() ;}
    
    BOOL	  ResolveTexturesOnHRC(Brush* pB, Brush** pBRet, 
                                vvector<Texture >&  texNames,
                                vvector<Texture >*  pTexNames);
    void      ExportFile(PlugInDll<IGeticPlug>&,const char*);
    void      ImportFile(PlugInDll<IGeticPlug>&,const char*);
    void      CleanProjections(Brush& projBrush, BOOL onlyFlags=0);
	void	  LocalDrawBspTreeGraph(Compiler& compiler, int& leafNo, z_ed3View* pV);
	void	  LocalDrawReadyBSP(Compiler&  compiler, Scene& scene, z_ed3View* pV);
    BOOL      LoadPrefFile(const char* szFileName, Brush** pBRet=0);
    void      DrawPolyVertexes(size_t how, Poly& poly, BOOL gen, size_t what=0);
    void      DrawPolyLines(Poly& poly);
    void      DrawPolygon(Poly& poly, BOOL wireMode);
    void      DrawOrtho(const V3& pos, const V3& dir, REAL len, REAL clratn=1.0);
    void      BlockSelections(BOOL bl){_blocksel = bl;}
    void      ReadFromFile(const char* szFilename);
protected:
    void AddVertex(Poly* ps, const V3& v);
    void SaveInFile(const char* pSzName, BOOL prims=FALSE);
    void SavePrefFile();
    void SaveInXML(const char* pSzName, BOOL bBrshes=FALSE);
    void ReadFromBSP(const char* szFilename);
    void _BSP_ReadPolys(const FileWrap& fw, int elements);


    void SaveFinalBSP();
	void ModifyBrushes(vvector<Brush*>& pChanged);
    void RedoVxSelection(Brush* pBrush);
    void DrawDebugPointsAndPolys();
    void Draw3DRawScene(z_ed3View* pV);
    void UpdateBrushLinks(Brush* pBrush);
    void DeleteBrushItemLinks(Brush* pbrush);
    void UpdateDynamicObjPos(Brush* pb);
    void EvaluateDetailbrsh(Brush* );
    void ApplyFlags(Brush* b);
    void DoBoxSelSelection();
    void DrawCameraRay(CZ_ed2View *pV);
    void DrawProjectorBrush(Brush& rb);
    void DrawCamFrustrum(CZ_ed2View *pV);
    void SelectBrush(int brSel, int faceSel);
    Poly* SelectFace(int brSel, int faceSel);
    void SelectVertex(Vtx2*,Poly*);
	void SelectAllBrushVertexes(Brush& b, Vtx2& x);
	void DeselectVertex(Vtx2*);
	void SelectVertex(int,int,int);
	void Draw2DBrushes(CZ_ed2View *pV);
    void Draw2dItems(CZ_ed2View *pV);
    void DrawTextured(z_ed3View *pV);
    void DrawSelBrushes(z_ed3View *pV);
    void DrawSelFaces(z_ed3View *pV);
    void DrawCutBrush(Brush& rb);
    void DrawMotionsPaths();
	void OnMkShellBox(V3& vdim) ;
    Box  GetReferenceBrushBox();
    V3   GetViewXYZWidths();
    void InitDocVars();
    void ProjectTexture();
	void MarkDetPolys(int percent);
    void CommitBrush(Brush* pBrush);
    void RWCompSettings(FileWrap    &fw, BOOL save);
    void ApplyCloserLights(const Poly& poly);
    void DrawItems();
    void RecalcBrushes();
    BOOL HasExtension(const char* szex); //{"_GL_EXT_separate_specular_color")
    void TestFreeVersion(int polys);
    void _DrawCamera();
    void _SeColor(COLORREF crf, PPolys& pSelPolys);
    void _SeColor(COLORREF crf, vvector<Vtx2*>& pSelVertexes);
    void _SeColor(COLORREF crf, vvector<SceItem*>& pSelItems);
    void _SeColor(COLORREF crf, PBrushes& pSelBrushes);

public:
    size_t            _frmTimer;
    BOOL            _blocksel;	 //blocks selection when evaluate mode is on
    CWnd            *_pActiveView; 
    BR_MODE         _brmode;
    BR_MODE         _brmodePrev;
    EL_SEL          _selmode;
    EL_SEL          _prevSelmode;
    size_t           _compviewmode;
    BOOL            _bSnapGrid;
	Camera		    _cam;
    BOOL            _alignment;
    BOOL            _snapToGrid;
    int             _gridSize;
    int             _gridStep;
    CFont           _cfont;
    BOOL            _xyz_lock;
    BOOL            _selBoxByTouch;
    int             _iselbrush;
    V3              _shift;
    V3              _zoom;
    BOOL            _bDrawCamRay;
	BOOL			b_showgrid;
    int             b_whitebk;
	Scene           _scene;
	V3			    _camray[3];
	V3			    _frustDots[5];
    V3              _ip[2];
	BOOL		    _vmodefire;
	BOOL		    _vmodeback;
    int             _curgroup;
    int             _maxUsedGrp;
    int             _curCreatGroup; //current group of items we create
    Brush           _selBox;
    V3              _camsave[2];
	
    PBrushes			_pClipboard;
	vvector<SceItem*>	_pClipboardItm;

    vvector<PolyEdge>   _selEdges; 
    vvector<SceItem*>	_pSelItems;
    vvector<Vtx2*>      _pSelVertexes;
    int                 i_geomfilever;

#ifdef _DEBUG
    vvector<V3>         _testPoints;
#endif

    PBrushes            _pSelBrushes;
    PPolys          _pSelPolys;
    Compiler        _compiler;
    int             _leafNo;    
    BOOL            _compiling;     //set when we compile BSP
// child ctrls
    CDlgNewBrush    _newBrshDlg;    
    SNewBrush       _cachedSett;

	BOOL            _bShowCsg;
    BOOL            _closing;
    int             _undodirt;
    BOOL            _docDirty;
//
    vvector<Poly>	_debugPolys;
    V3              _errPoint;
	BOOL			_cmdMsg;
	int				_exViewMode;
    int             _lastItmSel;    //selection in item insert dialog
    BOOL            _viewChanged;
    POINT          _scrPoint;    
    BOOL           _doneReceived;
    BOOL           _selShowSelBox; 
    Brush*         _pLastMakedBrush;
    HGLRC          _hrc;
    BOOL           _userBool;
    BOOL           _allselected;
    tstring        _docdir;
    tstring        _lasterror;
    int            _viewCount;
	size_t		  _lvupdate;
    CDialogEvalDets	_devdet;
    BOOL            _clearing;
    CView*          _lastFocusView;
    CString        _tempfileName;
    BOOL           _complBreaking;
    size_t          _polycount;
    int            _use2splitidx;
    int            _maxLights; 
    int            _numLights;
    int            _maxMirrorsDraws;
    BOOL           _camSpot; 
    PFNGLADDSWAPHINTRECTWINPROC  _glAddSwapHintRectWIN;
    string         _extensions;
    CUndo          _undo; 
    BOOL           _tildselect; 
};

#define DOC()   CZ_ed2Doc::PDoc
#define SCENE() CZ_ed2Doc::PDoc->_scene
#define TERRIAN()  CZ_ed2Doc::PDoc->_scene.Terrain()

extern BYTE	CLR_SEL[4];
extern BYTE	CLR_NORM[4];
extern BYTE	CLR_BBOX[4];

class NO_VT GlMatrix
{
public:
    GlMatrix(){
        glPushMatrix();
    }
    ~GlMatrix(){
        glPopMatrix();
    }
};

class SelModeSave
{
public:
    SelModeSave(BR_MODE& pmode, EL_SEL& psel):_pmode(&pmode),_psel(&psel),
                                              _vmode(pmode), _vsel(psel)
    {
    }
    ~SelModeSave()
    {
        *_pmode = _vmode;
        *_psel  = _vsel;  
    }

    BR_MODE* _pmode;
    EL_SEL*  _psel;  
    BR_MODE  _vmode;
    EL_SEL   _vsel;  

};



#define DOC_VERSION2     2
#define DOC_VERSION3     3
#define DOC_VERSION4     4
#define DOC_VERSION5     200  // terrain details
#define DOC_VERSION6     201  // sound position bug
#define DOC_VERSION7     202  // early convex
#define DOC_VERSION8     203  // lmap custom intensity
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __ED2_DOC_H__
