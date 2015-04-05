//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------
// Author: Marius Chincisan: 2003
//---------------------------------------------------------------------------------------
#include "stdafx.h"
#include "z-edmap.h"
#include "Compiler.h"
#include "Scene.h"
#include "z_ed2Doc.h"
#include "BspTree.h"
#include "z_ed3View.h"     
#include "DlgCompProgress.h"
#include "MainFrm.h"
#include "TriggerItem.h"

//---------------------------------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------------------
Compiler* Compiler::PCompiler;

static int GCOunter;

//---------------------------------------------------------------------------------------
class Cwc
{
public:
    Cwc(){}
    ~Cwc(){}
    void Step(LPCTSTR stp){
        if(ThrID != GetCurrentThreadId())
        {
            if(stp[0]!='-')
            {
                SBT(0, MKSTR("+%s Please Wait !", stp));
            }
        }
    }
};


//---------------------------------------------------------------------------------------
Compiler::Compiler()
{
    _errDetected       = 0;
    _done              = FALSE;
    _bEsr              = TRUE;
    _bPvsMethod        = 1;
    _bGeticBSP         = 1;//AfxGetApp()->GetProfileInt("BSP","GETICBSP",1);
    _bSnapMainBsp      = 0;
    _bZones            = TRUE; 
    _bPvsMethod        = 1;
    _lmapIntens        = 4; 
    _bMergeNarrowLeafs = 0;
    _narowLeaf         = 16;  /*cm*/
    _bAutoDetailSmall  = FALSE;
    _bAutoDetailPolys  = FALSE;
    _removeOnePolyLeaf = FALSE;
    _portalCutFlag     = CUT_PORTAL_BY_NODE;
    _break             = 0;
    _bCompressPvs      = FALSE;
    _bAABBTreeCsgTer   =  FALSE;
    _bSaveImages       = FALSE;
    _maxFacesLeaf      = 0;
	_bSaveCats         = FALSE;
    _bSavePortals      = TRUE;
    _bPolySortCrit     = 0;
    _bSaveSkyDom       = TRUE;
    _bSaveBigTerrain   = TRUE;
    _bSaveBigTerrainBSP= TRUE;
	_bExportFile	   = FALSE;	
    
    _bAvoidNonOrt      = FALSE;
    _bUseUserPriority  = FALSE;

    _bConvexEarly      = 0;
    _bLightMaps        = TRUE;
    _lmpowof2          = TRUE;
    _lmConst          = FALSE;
    _b3SampleLM        = TRUE;
    _inThread          = FALSE;
    _bSaveFinalBSP     = FALSE;
    _szipit            = TRUE;
    _bLmPatch          = FALSE;
    _finalBSpFileName  = "";
    _exBspPlugin       = "";
    _balance           = 6;
    _curStep           = 0;
    _percentage        = 0;
    PCompiler          = this;
    _lmapMaxSize       = 16;
    _lMapLumelSz       = 64.0f;

};

//---------------------------------------------------------------------------------------
BOOL Compiler::Compile(Scene& scene)
{
#ifdef _DEBUG
    DOC()->_testPoints.clear();
#endif //

    LOG(E_COMP, "Compiling BSP level...");

    DOC()->OnEscape();
    DOC()->DeselectAll();

    _errDetected=0;
	if(theApp._returnVal==0)
    {
		PostQuitMessage(0);
        return FALSE;
    }

    char   docName[_MAX_PATH];
    ::_tcscpy(docName, DOC()->GetTitle());
    PathHandler ph(docName);
	Break(0);

    _finalBSpFileName = theApp.BspsDir();
    _finalBSpFileName += ph.JustFile();
    _finalBSpFileName += ".gbt";
    
	
    Clear();
    scene.ClearBrushes();
    _pScene  = &scene;
    _lmProc.NTC_Clear();
    _break        = 0;
    _zonecount    = 0;
    _errDetected  = 0;
    FOREACH(PBrushes, (*DOC()->_scene.GetPrimitives()), ppb)
    {
        if((*ppb)->IsZone())
        {
            (*ppb)->ZoneIdx()  = _zonecount;
            (*ppb)->VisData()  = 1<<_zonecount; // zone can se itself
            _zones[_zonecount] = (*ppb);
            ++_zonecount;
        }
    }
    _timeStart = GetTickCount();
    return Start();
}

//---------------------------------------------------------------------------------------
// see wich trigger points to this brush. Update triger index to this BSP witch is the
// brush bsp
void Compiler::EstablishTrigerLinks(Brush* pBrush, int trrIndex)
{
    vvector<SceItem*>* pItems = _pScene->GetItems();
    FOREACH(vvector<SceItem*>, (*pItems), ppItem)
    {
        if((*ppItem)->_item != ITM_TRIGER)continue;
        if( ((TriggerItem*)(*ppItem))->_pBrush ==  pBrush)
        {
            ((TriggerItem*)(*ppItem))->_brIndex = trrIndex;
            break;
        }
    }
}

//---------------------------------------------------------------------------------------
int Compiler::_CheckBrushes(BOOL hasTerrain)
{
    _curStep = 0;
AGAIN:
    FOREACH(PBrushes, (*_pScene->GetPrimitives()), ppBrsh)
    {
        if((*ppBrsh)->_polys.size() == 0)
        {
            DOC()->DeleteBrush((*ppBrsh));
            goto AGAIN;
        }
        
        if((*ppBrsh)->IsSelection() || 
           ((*ppBrsh)->_brushflags & BRSH_BIGTERRAIN))
            continue;

        if(!((*ppBrsh)->_brushflags & BRSH_DETAIL))
        {
            _curStep = 1;
        }
        
        if((*ppBrsh)->_brushflags & BRSH_DISABLED && !((*ppBrsh)->_brushflags & BRSH_BIGTERRAIN))
        {
            DOC()->SelectBrush((*ppBrsh)); // recut these
        }
    }
    return _curStep;
}

void Compiler::_RecutSelectedBrushes()
{

    int nCount;
    Brush** ppBrushes = DOC()->GetSelBrushes(nCount);
    if(nCount)
    {
        UINT res = AfxMessageBox(MKSTR("There are '%d' Brushe(s) disabled (Greyed). \r\n"
            "Do you like to enable them for the Compilation ?",nCount), MB_ICONQUESTION|MB_YESNOCANCEL);
        if(res == IDYES)
        {
            DOC()->Recut();
            DOC()->DeselectAll();
        }
        else if(res==IDCANCEL)
        {
            DOC()->DeselectAll();
            LOG(E_COMP, "Info: User Abort. You can enable disabled brushes manually");
            throw E_COMP;
        }
    }
}

//---------------------------------------------------------------------------------------
UINT Compiler::MainFoo(HANDLE hStop)
{
    int Gtask = BrushTask;
    //save lmpatch do not aplly it for ESR just for last BSP
    try
    {
        Cwc c;

        BrushTask = 0;

        _inThread       = 1;
        BOOL hasTerrain = SCENE().Terrain().Exist();
        PBrushes* pBrushes = _pScene->GetBrushes();

        if(_pScene->GetPrimitives()->size()==0)
        {
            LOG(E_COMP, "Error: the level has not brushes left to compile. They may be wiped out during CSG !");
            throw E_COMP;
        }

        _curStep        = 0;
        _percentage     = 0;
        _done           = FALSE;
        

        int hasMain = _CheckBrushes(hasTerrain);    

        if(_curStep==0 && !hasTerrain)
        {
            DOC()->DeselectAll();
            LOG(E_COMP, "Error: CMPL0001. The Scene does not have a Main Brush (All Brushes are details) !");
            throw E_COMP;
        }
        
        _RecutSelectedBrushes();
        
        if(hasTerrain)
            CompileTerrain();

        c.Step("HSR");
        //  HSR Step 1
        _curStep++;
        _percentage = 0;
        if(!PerformHSR( DOC()->_scene.GetPrimitives()))
        {
            LOG(E_COMP, "Error: HSR0001. Error Performing Hidden Surface Removal !. Check if the level is sealed !");
            throw E_COMP;
        }
        if(pBrushes->size() == 0)
        {
            LOG(E_COMP, "Error: HSR0001. Scene has no brushes to build ! They may have been wiped out by CSG");
            throw E_COMP;
        }
        
        //
        
        if(hasMain)
        {
            c.Step("ESR");
            _curStep++;
            _percentage = 0;
            if(_bEsr)
            {
                if(!PerformESR(pBrushes) || _break)
                {
                    throw E_HSR;
                }
            }
        }
        
        //
        // reserve bsps as many brushes we have
        //
        _bTrees.reserve(pBrushes->size());
        CBspTree* pMainModel = 0;
        CBspTree* pSkyModel  = 0;
        
        //
        // step 3 compiling prepared geometry
        //
        c.Step("BSP Step 1");
        _curStep++;
        _percentage = 0;
        int nCount = 0;
        V3  bbex;
        
        //
        // places the main brush on pos 0 and sky on last
        //
        c.Step("Sorting");
        SortBrushes(*pBrushes);
        c.Step("BSP Compiling");
        FOREACH(PBrushes, (*pBrushes), ppBrsh)
        {
            
            c.Step("-");
            
            LOG(E_BSP,"BSP: Compiling Brush '%s'. ", (*ppBrsh)->_name);
                        
            if(_break){
                throw E_BSP;
            }
            if((*ppBrsh)->IsZone())
            {
                continue;
            }
            
            Brush* pBrush = *ppBrsh;
            pBrush->Dirty(1);
            pBrush->Recalc(1/*with normals*/);
            

            bbex = pBrush->_box.GetExtends();
            if(bbex.x < 16 && bbex.y < 16 && bbex.z < 16)
            {
                LOG(E_BSP,"BSP: Brush: '%s' is to small. Brush minimum size is 16 units. Brush ignored", (*ppBrsh)->_name);
                continue;
            }

            PROG(nCount);
            nCount++;
            
            CBspTree* pTree    = new  CBspTree();
            if(0==pTree)
            {
                LOG(E_BSP,"BSP: Out of memory", (*ppBrsh)->_name);
                throw E_BSP;
            }

            #pragma message ("copy brush props into tree props")
            
            _tcscpy(pTree->_name, pBrush->_name);

            pTree->_balance     = _balance;
            pTree->_treeflags   = pBrush->_brushflags;
            pTree->_treeprops   = pBrush->_brushprops ;
            pTree->_patchPolys  = Compiler::PCompiler->_bLmPatch;
            pTree->_motionsIdx  = DOC()->_scene.GetIndex(pBrush->_pMotion);
            if(pTree->_motionsIdx>=0)
            {
                pTree->_treeprops|=MODEL_DYNAMIC;
            }
            else
            {
                pTree->_treeprops&=~MODEL_DYNAMIC;
            }
            pTree->_thisIdx     = _bTrees.size();
            
            c.Step("-");
            this->_bConvexEarly = 0; //portal fails
            BOOL  success = pTree->Compile(pBrush->_polys, pBrush->_brushflags,TRUE, this->_bConvexEarly);
            if(!success)
            {
                DOC()->SelectBrush(pBrush);
                DOC()->MoveCamera(pBrush->_box.GetCenter());
                DOC()->Invalidate();
                delete pTree;
                throw E_BSP;
            }
            pTree->_treeprops   = pBrush->_brushprops ;       //for patching and materials (0xFF)
            pTree->_rotSteps    = pBrush->_rotSteps;
            // add on
            
            if( pBrush->_brushflags & BRSH_DETAIL || 
                pBrush->_brushflags & BRSH_CSGTERRAIN)
            {
                pTree->_treeprops  |= MODEL_IS_DETAIL;
                
                if(pBrush->_brushflags & BRSH_CSGTERRAIN)
                    pTree->_treeprops  |= MODEL_IS_TERRAIN;

                if(pBrush->_brushflags |= BRSH_TRIANGLES)
                    pTree->_treeprops  |= MODEL_TRIANGLES;

                if(pBrush->_brushflags |= BRSH_DONTBSP)
                    pTree->_treeprops  |= MODEL_IS_FLAT;

            }
            else if(pBrush->_brushprops & MODEL_IS_SKYDOM)
            {
                ASSERT(0 == pSkyModel);
                pSkyModel = pTree;
            }
            else
            {
                ASSERT(pMainModel  == 0);
                pMainModel         = pTree;
                pTree->_treeprops  |=  MODEL_IS_MAINBSP;
            }
            if(pBrush->_brushflags & BRSH_HASTRIG)
            {
                EstablishTrigerLinks(pBrush , pTree->_thisIdx);
            }
            _bTrees << pTree;
        }//each brush
        

        if((_bPvsMethod != 0 || _bZones) && pMainModel && !hasTerrain)
        {
            c.Step("Portals");
            _portPrc._mergeLeafs = _bMergeNarrowLeafs;
            if( _portPrc.Process(*pMainModel,1))
            {
                if(_pvsPrc.Process(pMainModel, &_portPrc))
                {
                    pMainModel->_prtProc = &_portPrc;
                }
                else
                {
                    throw E_PVS;
                }
            }
            else
            {
                throw E_PORT;
            }
            
            if(_bZones)
            {
                AutomateZoneVisibility(pMainModel);
                if(_bPvsMethod == 0)
                {
                    _portPrc.Clear();
                    _pvsPrc.Clear();
                }
            }
            else
                _ManuaIntersectZones();
        }
        else  
            _ManuaIntersectZones();

        LinkDetailsToMainBsp();
        
        if(_bLightMaps)
        {
            if(FALSE == PerformLmapCalculation(pMainModel))
            {
                throw E_LMAPS;
            }
        }
        
        // reduce trees. See if tree is concave or convex. reduce the leafs to 1
        // place a flag in the tree and collect all polys in that leaf
        c.Step("Concave Convex");
        FOREACH(vvector<CBspTree*>, _bTrees, ppTree)
        {
            (*ppTree)->ConcaveConvex();
        }
        _done = TRUE;
        PROG(-1);
        _inThread  = 0;
    }
    catch(EXCOMP& ierr)
    {
        NOTIFY_FRAME(WM_USR_REPAINT,0,0);
        BrushTask=Gtask;
        NOTIFY_FRAME(WM_COMPILEDONE,ierr,0);
        PROG(-1);
        return E_COMPILER;
    }
#ifndef _DEBUG
    catch(...)
    {
        NOTIFY_FRAME(WM_USR_REPAINT,0,0);
        BrushTask=Gtask;
        NOTIFY_FRAME(WM_COMPILEDONE,E_COMPILER,0);
        PROG(-1);
        return E_COMPILER;
    }
#endif //
    NOTIFY_FRAME(WM_USR_REPAINT,0,0);
    BrushTask=Gtask;
    DWORD tsecs = (GetTickCount()-_timeStart)/1000;
    int   tmin = tsecs/60;
    int   secs = tsecs%60; 
    LOG(E_BSP,"BSP: Compilation Time: %d tmin %d sec", tmin, secs);
    NOTIFY_FRAME(WM_COMPILEDONE,0,0);
    PROG(-1);
    return 0;
}


//---------------------------------------------------------------------------------------
void    Compiler::ReevaluateBrush(Brush* pBrush)
{
    LOG(E_HSR,"HSR: Evaluating detail brushes '%s'",pBrush->_name);
    
    if(pBrush->_brushprops & MODEL_IS_SKYDOM)
    return;
    
    if(_bAutoDetailSmall)
    {
        V3 ex = pBrush->_box.GetExtends();
        if(ex.getmin() < 30.f && ex.getmax() < 200.0)
        pBrush->_brushflags |= BRSH_DETAIL;
    }
    
    if(_bAutoDetailPolys)
    {
        if(pBrush->_polys.size() > 32)
        pBrush->_brushflags |= BRSH_DETAIL;
    }
}

//---------------------------------------------------------------------------------------
BOOL    Compiler::CompileTerrain()
{
    _terrTree.Compile(&SCENE().Terrain());
     return 1;
}

//---------------------------------------------------------------------------------------
// union all brushes and delete any poly ennding up in solid space
BOOL    Compiler::PerformHSR(PBrushes* pBrushes )
{
    BOOL brv = FALSE;
    _polysStats = 0;
    _cutsStats  = 0;
    
    if( 0 == pBrushes->size())
        return brv;
    
    LOG(E_HSR,"HSR: Performing hidden surface removal");
    
    PBrushes    brshSolids(256);
    PBrushes    brshSkydoms(64);
    
    CWaitCursor cwc;
    //
    // clear prev output brushes
    //
    int nCount = 0;
    
    //scene.ClearBrushes();
    
    FOREACH(PBrushes, (*pBrushes), ppBrsh)
    {
        Brush* pBrushO =  (*ppBrsh);
        Brush* pBrush  =  pBrushO->_pResult;

        if((pBrush)->IsSelection() || (pBrush->_brushflags & BRSH_BIGTERRAIN) )
              continue;

        
        #pragma message("auto details based on number of polys and minimum extend")
        /*
        if(_bAutoDetailSmall || _bAutoDetailPolys)
        {
            ReevaluateBrush(pBrush); // preserve the skydoms
        }
        */
        
        // not touch detail brushes. add them to scene as they are
        if(pBrush->_brushflags & BRSH_DETAIL || pBrush->_brushflags & BRSH_CSGTERRAIN)
        {
            Brush* pResultSceneBrsh = new Brush(*pBrush);

            if(pBrush->_brushflags & BRSH_DETAIL)
            {
                CMotion* pMotion = pBrush->_pMotion ? pBrush->_pMotion : pBrushO->_pMotion;
                pResultSceneBrsh->_pMotion = pMotion;
            }


            DOC()->_scene.AddBrush(pResultSceneBrsh);
            brv = TRUE;
            continue;
        }
        
        // collect all skydom brushes
        if(pBrush->_brushprops & MODEL_IS_SKYDOM)
        {
            pBrush->Recalc();
            brshSkydoms.push_back(pBrush);
            continue;
        }
        
        if(pBrush->_brushflags & BRSH_SOLID)
        {
            pBrush->Recalc();
            brshSolids.push_back(pBrush);
        }
    }
    
    if(brshSolids.size() != 0)
    {
        Brush* pResultSceneBrsh = new Brush();

        pResultSceneBrsh->_brushflags = brshSolids[0]->_brushflags | BRSH_SOLID;
        
        if(brshSolids.size())
        {
            if(Union(brshSolids, *pResultSceneBrsh,1))
            {
                strcpy(pResultSceneBrsh->_name, brshSolids[0]->_name);
                DOC()->_scene.AddBrush(pResultSceneBrsh);
                brv = TRUE;
            }
            else
            {
                delete pResultSceneBrsh;
                pResultSceneBrsh = 0;
                return FALSE;
            }
        }
        else
        {
            delete pResultSceneBrsh;
            pResultSceneBrsh = 0;
            return FALSE;
        }
    }
    
    if(brshSkydoms.size() != 0)
    {
        Brush* pResultSceneBrsh = new Brush();
        pResultSceneBrsh->_brushflags = BRSH_SOLID;
        pResultSceneBrsh->_brushprops = MODEL_IS_SKYDOM;
        
        if(Union(brshSkydoms, *pResultSceneBrsh,1))
        {
            DOC()->_scene.AddBrush(pResultSceneBrsh);
            brv = TRUE;
        }
        else{
            delete pResultSceneBrsh;
            pResultSceneBrsh = 0;
            return FALSE;
        }
    }
    
    cwc.Restore();
    
    LOG(E_HSR,"HSR: Successful");
    return brv ;
}

//---------------------------------------------------------------------------------------

BOOL    Compiler::Union(PBrushes& pPrimitives, Brush& brush, BOOL sgow)
{
    Polys   firstPolys;
    Polys   pSplitters;
    Polys   splittedPolys;
    Polys   outPolys;
    int     bClipIt  = FALSE;
    Brush*  pB1      = NULL;
    Brush*  pB2      = NULL;
    
    int counter = {0};
    
    int         cntInPolys  = 0;
    int         cntOutPolys = 0;
    int         maxsteps = pPrimitives.size()*pPrimitives.size();

    
    FOREACH(PBrushes, pPrimitives, ppPrim)
    {
        bClipIt = FALSE;
        pB1     = *ppPrim;

        if(pB1->IsSelection()|| (pB1->_brushflags & BRSH_BIGTERRAIN) )
        {
            continue;
        }

        pB1->GetTxPolysCopy(firstPolys);    // get translated polygons
        cntInPolys+=firstPolys.size();

        FOREACH(PBrushes, pPrimitives, ppPrim1)
        {
            if((*ppPrim1)->IsSelection()|| ((*ppPrim1)->_brushflags & BRSH_BIGTERRAIN))
            {
                continue;
            }
		
            (*ppPrim1)->Recalc();
            (*ppPrim)->Recalc();

            pB2 = *ppPrim1;
            if (pB1 == pB2)
            {
                bClipIt = TRUE;
                ++counter;
                continue;
            }
            
            if(!pB1->_box.IsTouchesBox(pB2->_box))
            {
                continue;
                ++counter;
            }

            // animation by selection
        	if((counter & 0xF) == 0xF)
			{
				
                if(GAnimCompilation)
                {
                    NOTIFY_FRAME(WM_USR_REPAINT,(WPARAM)pB1,(LPARAM)pB2);
                }
			

				int percentage = (counter*100)/(maxsteps+1);
				if(PCompiler->_inThread==0)//LOG
				{
					KEEP_WAIT(MKSTR("Performing Constructive Solid Geometry %d%%", percentage));
				}
				else
				{
				    PROG(percentage);
				}
			}
                
            do //scope
            {
                MiniBsp bsp;
                pB2->GetTxPolysCopy(pSplitters);

                if(!bsp.Compile(pSplitters,0,FALSE))
                {
                    return FALSE;
                }

                bsp.ClipBrPolys(firstPolys, splittedPolys, bClipIt,1,1);
                firstPolys.clear();
                firstPolys = splittedPolys;
                pSplitters.clear();
                splittedPolys.clear();
                
            }while(0);
            
            if(GAnimCompilation)
            {
                NOTIFY_FRAME(WM_USR_REPAINT,0,0);
                Sleep(0);
            }
			++counter;
        }
        append(outPolys, firstPolys);
        firstPolys.clear();
    }
    
    if (outPolys.size())
    {
        FOREACH(Polys, outPolys, op)
        {
            op->Recalc();
            op->_pBrush = &brush;
            brush._polys.push_back(*op);
            cntOutPolys++;
        }
        brush.Recalc();
        brush._brushflags |= BRSH_SOLID;
    }
    PROG(-1);
    SBT(0,"+Ready");
    Compiler::PCompiler->_polysStats=cntInPolys;
    return TRUE;
}


//---------------------------------------------------------------------------------------
void  Compiler::Render(z_ed3View* pV, DWORD what)
{
    if(!_done)
    {
        pV ? pV->TextOut(V0,"There is nothing to render",ZWHITE):0;
    }
    
    if(what & REND_BSP && _terrTree.Exist())
    {
        _terrTree.Render(pV!=0);
    }
    if((what & REND_PORTALS) && pV)
    {
        _portPrc.Render(pV);
    }
    
}

//---------------------------------------------------------------------------------------
int FindLeaf(V3& pct, CBspTree&  tmpTree)
{
    CMiniNode* pNode = tmpTree.Root();
    
    while(pNode->IsNode())
    {
        int side = pNode->GetPlane().GetSide(pct);
        
        if(side >= 0)
        pNode = pNode->Front();
        else if(side < 0)
        pNode = pNode->Back();
        
    }
    return pNode->_leafIdx;
    
}

//---------------------------------------------------------------------------------------
// start on a leaf and perform a flood fill on the tree. wet all leafs we can go in from
// this by entering a portal
BOOL  Compiler::PerformESR(PBrushes* pbrushes)
{
    int         dummy;
    CBspTree    tmpTree;
    PortalPRC   tmpPPrc;
    int         inPolys  = 0;
    int         outPolys = 0;
    Brush*      pBrush   = 0;
    
    LOG(E_ESR," ESR: Performing Exterior Surface Removal");
    
    FOREACH(PBrushes, (*pbrushes), ppBrsh)
    {
        pBrush = *ppBrsh;
        if( pBrush->_brushprops & MODEL_IS_SKYDOM)
            continue;
        
        if( pBrush->_brushflags & BRSH_DETAIL)
            continue;

        if( pBrush->_brushflags & BRSH_CSGTERRAIN)
            continue;

        if( pBrush->IsSelection()|| ((pBrush)->_brushflags & BRSH_BIGTERRAIN))
            continue;
        break;
    }
    
    // ESR only on main tree
    if(pBrush->_brushflags & BRSH_CSGTERRAIN || pBrush->IsSelection() || ((pBrush)->_brushflags & BRSH_BIGTERRAIN))
    {
        return TRUE;
    }
    
    ::_tcscpy(tmpTree._name, pBrush->_name);
    if(0==tmpTree.Compile(pBrush->_polys, pBrush->_brushflags, FALSE))
        return 0;
    
    int savedFlag = Compiler::PCompiler->_portalCutFlag;
    tmpPPrc._testExterior = 0;
    Compiler::PCompiler->_portalCutFlag = savedFlag;
    if(0==tmpPPrc.Process(tmpTree))
    {
        Compiler::PCompiler->_portalCutFlag = savedFlag;
        return FALSE;
    }
    Compiler::PCompiler->_portalCutFlag = savedFlag;
    //
    // reset del flags
    //
    FOREACH(vvector<Poly>, tmpTree.GetPolys(), ppoly)
    {
        ppoly->_polyflags &= ~POLY_DELETED;
        inPolys++;
    }
    
    
    // AUG 04
    // find a leaf around center
    //
    int      kCount  = 128;
    V3       vp      = V0;
    int      ileaf   = -1;
    int      j       = 0;
    ileaf = tmpTree.R_FindLeaf(0, vp);
    
    //
    // get first leaf center
    //
    if(-1 == ileaf)
    {
        LOGERR(E_ESR, V0,"Error: Center of the map must be empty!");
        _errDetected=1;
        return FALSE;
    }
   

    
    BYTE*  byAlloc = new BYTE[tmpTree.GetLeafsCount()]; //alloc bits for each leaf
    
    if(0 == byAlloc)return FALSE;
    ::memset(byAlloc,0,tmpTree.GetLeafsCount());
    

    _TRY{

        GCOunter = tmpTree._leafs.size();
        PROG(0);
        (0);
        R_FloodBSP(tmpTree, tmpPPrc, ileaf, byAlloc);
        PROG(100);
        
        //
        // delete unvisitted polys
        //
        vvector<CLeaf*>&     iLfsPtr = tmpTree.GetLeafs();
        int j = 0;
        FOREACH(vvector<CLeaf*>, iLfsPtr, ppLeaf)
        {
            CLeaf*  pLeaf  = *ppLeaf;
            int     nCount = 0;
            Poly*   pHead  = pLeaf->GetPolys(nCount);
            
            if(byAlloc[j]!=0)
            {
                j++;
                continue;
            }
            
            for(int i=0; i<nCount;i++)
            {
                pHead->_polyflags |= POLY_DELETED;
                pHead++;
            }
            j++;
        }
    }
    _CATCHX()
    {
        delete[] byAlloc;
        return FALSE;
    }
    delete[] byAlloc;
    
    if(_removeOnePolyLeaf) //remove leaking leafs
    {
        FOREACH(vvector<Portal>, tmpPPrc.GetPortals(), pPortal)
        {
            // if both leafs are 1 poly remove their polys
            int    nfpp = pPortal->BackLeaf()->_nPolys;
            int    nbpp = pPortal->FrontLeaf()->_nPolys;
            int    nfpc = pPortal->FrontLeaf()->_portIdxes.size();
            int    nbpc = pPortal->BackLeaf()->_portIdxes.size();
            
            if( nfpp ==1 && nbpp==1)
            {
                pPortal->BackLeaf()->GetPolys(dummy)->_polyflags |= POLY_DELETED;
                pPortal->FrontLeaf()->GetPolys(dummy)->_polyflags |= POLY_DELETED;
            }
        }
    }
    else
    {
        
    }
    
    //
    // place polys back into the brush
    //
    pBrush->_polys.clear();
    
    FOREACH(vvector<Poly>, tmpTree.GetPolys(), prpoly)
    {
        if(prpoly->_polyflags & POLY_DELETED)
        continue;
        
        //
        // clean splitter flags for next processing
        //
        prpoly->_polyflags &= ~SPLITTER_POLY;
        pBrush->_polys << (*prpoly);
    }
    
    // count the remaining polygons. if none thare means there is a hole
    // in the solid
    tmpTree.Clear();
    tmpPPrc.Clear();
    
    if(pBrush->_polys.size()==0 )
    {
        LOGERR(E_ESR, V0, MKSTR("Error: Leaf '%d' is Leaking outside.",ileaf));
        _errDetected=1;
        return FALSE;
    }
    pBrush->Recalc();
    outPolys = pBrush->_polys.size();
    
    LOG(0, MKSTR(" ESR: %d polygons outside map were deleted",inPolys - outPolys));
    return TRUE;
}

//---------------------------------------------------------------------------------------
// flood all leafs recusrivelly and mark the leafs visited in the array.
// anu non touched leafs
void  Compiler::R_FloodBSP(CBspTree&  tmpTree, PortalPRC& tmpPPrc, int iLeaf, BYTE* wetBytes)
{
    wetBytes[iLeaf] = 1;
    
    CLeaf*          pLeaf = tmpTree.GetLeaf(iLeaf);
    vvector<int>&   portalsIdxes = pLeaf->GetPortalIdxes();
    int             percentage   = (100* (++_percentage))/GCOunter;
    
    FOREACH(vvector<int>,  portalsIdxes, ppidx)
    {
        Portal&  portal =  tmpPPrc.GetPortal(*ppidx);
        CLeaf*   pNleaf = portal.FrontLeaf();
        
        if(wetBytes[pNleaf->_leafIdx] == 0)
        {
            V3 ex = pNleaf->_bbox.GetExtends();
            R_FloodBSP(tmpTree, tmpPPrc, pNleaf->_leafIdx, wetBytes);
        }
    }
}

//---------------------------------------------------------------------------------------
void    Compiler::LinkDetailsToMainSky()
{
    vvector<CBspTree*>::iterator itBegin =  _bTrees.begin();
    vvector<CBspTree*>::iterator itEnd   =  _bTrees.end();
    CBspTree* pMaster = _bTrees.back();
    
    ASSERT(pMaster->_treeprops & MODEL_IS_SKYDOM);
    ASSERT(!(pMaster->_treeprops & MODEL_IS_DETAIL));
    
    int cnt = 0;
    for(;itBegin != itEnd; itBegin++)
    {
        if( ((*itBegin)->_treeprops & MODEL_IS_SKYDOM) &&
        ((*itBegin)->_treeprops & MODEL_IS_DETAIL))
        {
            pMaster->LinkDetailModel(*itBegin, FALSE);
        }
        cnt++;
    }
}

//---------------------------------------------------------------------------------------
// Takes each detail bsp bsp and trow's them on the main bsp. [0]
// Updates the main bsp leaf with the detail bsp index as a link of details (_detaiIdxes).
void    Compiler::LinkDetailsToMainBsp() // LMAP DEPENDS OF IT
{
    LOG(E_COMP, "BSP: Linking detail models to main BSP)");
    vvector<CBspTree*>::iterator itBegin =  _bTrees.begin();
    vvector<CBspTree*>::iterator itEnd   =  _bTrees.end();
    CBspTree* pMaster = *itBegin++;
    
    ZoneAreea(pMaster, 1);
    //
    // link detail models
    //
    int cnt = 0;
    for(;itBegin != itEnd; itBegin++)
    {
        if((*itBegin)->_treeprops & MODEL_IS_SKYDOM) // reject sky dom detail brushes
        {
            continue;
        }
        if((*itBegin)->_treeprops & MODEL_IS_DETAIL || 
            (*itBegin)->_treeprops & MODEL_IS_TERRAIN)
        {
            ZoneAreea(*itBegin, 0);
            pMaster->LinkDetailModel(*itBegin);
        }
        cnt++;
    }
    
    //
    // link items to main bsp
    //
    vvector<SceItem*>* pItems = _pScene->GetItems();
    FOREACH(vvector<SceItem*>, (*pItems), ppItem)
    {
        ZoneAreea(*ppItem);
        pMaster->LinkItem(*ppItem,1);
    }
    LOG(E_COMP, "BSP: %d detail models linked successfully", cnt);
}

//---------------------------------------------------------------------------------------
void    Compiler::ZoneAreea(SceItem* psi)
{
    psi->_zonearea = 0;
    Brush* pZone = GetZone(psi->_t);
    if(pZone)
    {
        psi->_zonearea = pZone->VisData();
      
    }
    else
        psi->_zonearea  = 0xFFFFFFFF;
}

//---------------------------------------------------------------------------------------
void    Compiler::ZoneAreea(CBspTree* pBsp, BOOL isMain)
{
    if(isMain)
        ZoneMainBSP(pBsp);
    else
    {
        pBsp->_zoneareea = 0;
        Brush* pZone = GetZone(pBsp->_nodesPtr[0]->_bbox.GetCenter());
        if(pZone)
        {
            pBsp->_zoneareea = pZone->VisData();
        
        }
        else
            pBsp->_zoneareea = 0xFFFFFFFF;
    }
}

//---------------------------------------------------------------------------------------
void    Compiler::ZoneMainBSP(CBspTree* pTree)
{
    
    FOREACH(vvector<CMiniNode*>, pTree->_nodesPtr,ppNode)
    {
        CMiniNode* pNode = *ppNode;
        Brush   *pZone= GetZone(pNode->_bbox.GetCenter());
        pNode->_zonearea = 0;
        if(pZone)
        {
            pNode->_zonearea = pZone->VisData();
          
        }
        else 
            pNode->_zonearea = 0xFFFFFFFF;
    }
    FOREACH(vvector<CLeaf*>, pTree->_leafs, ppLeaf)
    {
        CLeaf* pLeaf = *ppLeaf;
        Brush* pZone = GetZone(pLeaf->_bbox);
        pLeaf->_zonearea = 0;
        if(pZone)
        {
            pLeaf->_zonearea = pZone->VisData();
     
        }
        else
            pLeaf->_zonearea = 0xFFFFFFFF;
    }
}

//---------------------------------------------------------------------------------------
Brush   *Compiler::GetZone(V3& vx)
{
    int i=0;
    while(_zones[i])
    {
        if(_zones[i]->_box.ContainPoint(vx))
        {
            return _zones[i];
        }
        ++i;
    }
    return 0;
}

//---------------------------------------------------------------------------------------
Brush   *Compiler::GetZone(Box& box, int* pindex)
{
    int i=0;
    while(_zones[i])
    {
        if(_zones[i]->_box.ContainBox(box))
        {
            if(pindex)
                *pindex=i;
            return _zones[i];
        }
        ++i;
    }
    return 0;
}


//---------------------------------------------------------------------------------------
// places the main bsp on position 0. and aktdom brushes on last pos (thats all)
void    Compiler::SortBrushes(PBrushes& brushes)
{
    int     skyIdx = -1;
    int     elements = brushes.size();
    Brush*  pT;
    
    for(int i=0; i<elements; i++)
    {
        if(brushes[i]->_brushflags & BRSH_DETAIL)
        continue;
        
        if(brushes[i]->_brushprops & MODEL_IS_SKYDOM)
        {
            skyIdx = i;
            continue;
        }
        if(0!=i)
        {
            pT = brushes[0];
            brushes[0] = brushes[i];
            brushes[i] = pT;
        }
    }
    //
    // places the skydom brush on last position
    //
    if(skyIdx>=0 && skyIdx != elements-1)
    {
        pT = brushes[skyIdx];
        brushes[skyIdx] = brushes[elements-1];
        brushes[elements-1] = pT;
    }
    
    brushes[0]->_brushflags = BRSH_MAINBSP;
}

//---------------------------------------------------------------------------------------
static BOOL	Local_IsLeafVisFromLeaf(vvector<CLeaf*>& leafs, int l1, int l2, BYTE* ppvs)
{
	if(l1<0||l2<0)return 1;
    BYTE* pPvs1 = &ppvs[leafs[l1]->_pvsIdx];
    return (pPvs1[l2 >> 3] & (1<<(l2 & 7)) )!=0;
}

//---------------------------------------------------------------------------------------
void    Compiler::_ManuaIntersectZones()
{
    for(int i=0;i<_zonecount;i++)
    {
        for(int j=0;j<_zonecount;j++)
        {
            if(j==i)continue;
            
            Brush* pZone1 = _zones[i];
            Brush* pZone2 = _zones[j];
            if(pZone1->_box.IsTouchesBox(pZone2->_box))
            {
                pZone1->VisData() |= (1 << j);
                pZone2->VisData() |= (1 << i);
            }
        }
    }
}

//---------------------------------------------------------------------------------------
void    Compiler::AutomateZoneVisibility(CBspTree* pBsp)
{
    int                         index;
    map<int, vector<CLeaf*> >   leafsPerZone;

    FOREACH(vvector<CLeaf*>, pBsp->_leafs, ppLeaf) // assign to leafs zone index
    {
        index        = -1;  
        CLeaf* pLeaf = *ppLeaf;
        Brush* pZone = GetZone(pLeaf->_bbox, &index);
        if(pZone)
        {
            leafsPerZone[index].push_back(pLeaf);
        }
    }    
    
    // for each zone leafs test other lefas
    map<int, vector<CLeaf*> >::iterator izB = leafsPerZone.begin();
    map<int, vector<CLeaf*> >::iterator izE = leafsPerZone.end();
    for(;izB!=izE;izB++)
    {
        vector<CLeaf*>& vLeafs = (*izB).second;

        // for each leaf from this zone
        FOREACH(vector<CLeaf*>, vLeafs, ppLeaf)
        {
            // search in other zones leafs
            map<int, vector<CLeaf*> >::iterator izB2 = izB;
            map<int, vector<CLeaf*> >::iterator izE2 = leafsPerZone.end();
            izB2++;
            for(;izB2!=izE2;izB2++)
            {
                vector<CLeaf*>& vLeafs2 = (*izB2).second;

                FOREACH(vector<CLeaf*>, vLeafs2, ppLeaf2)
                {
                    if(Local_IsLeafVisFromLeaf(pBsp->_leafs, 
                                               (*ppLeaf)->_leafIdx, 
                                               (*ppLeaf2)->_leafIdx, 
                                               _pvsPrc._pvs))
                    {
                        _zones[(*izB).first]->VisData()  |= (1 << (*izB2).first);
                        _zones[(*izB2).first]->VisData() |= (1 << (*izB).first);
                        break;
                    }
                }
            }//zone i2
        }
    }//zone i1
}

//---------------------------------------------------------------------------------------
BOOL    Compiler::PerformLmapCalculation(CBspTree* pMainModel)
{
    LOG(E_HSR,"LM: processing light maps ");
    
    if(!_lmProc.AllocMemory(_bTrees))
    {
        return FALSE;
    }

    if(FALSE == _lmProc.Calculate(_pScene, _bTrees))
    {
        return FALSE;
    }
    

    /*
    FOREACH(vvector<CBspTree*>,_bTrees, ppT)
    {
        if((*ppT)->_brushprops & MODEL_IS_SKYDOM)
        {
            continue;
        }
        
        if(FALSE == _lmProc.Calculate(_pScene, _bTrees[0], *ppT))
        return FALSE;
    }
    */
    return TRUE;
}

//---------------------------------------------------------------------------------------
void Compiler::CommitChanges()
{
    CWaitCursor cwc;
    PROG(-1);
    if(_break){
        NOTIFY_FRAME(WM_COMPILEDONE,E_LMAPS,0);
        _inThread          = 0;
        return;
    }

    int nIndex = 0;
    int total       = _bTrees.size();
    int percentage;

    START_WAIT("Generating Light Map Textures...");
    FOREACH(vvector<CBspTree*>,_bTrees, ppT)
    {
        _lmProc.NTC_BuildLmaps(*ppT);
        percentage  = (100*nIndex++)/(1+total);
        PROG(percentage);

        if(_break){
            END_WAIT("");
            NOTIFY_FRAME(WM_COMPILEDONE,E_LMAPS,0);
            _inThread= 0;
        }
    }
    _lmProc.NTC_BuildLmaps(&_terrTree);
    PROG(-1);
    END_WAIT("");
    
}

//---------------------------------------------------------------------------------------
DWORD Compiler::DoLightMaps()
{
    if(_bTrees.size() )
    {
        _lmProc.NTC_Clear();
        
        if(PerformLmapCalculation(_bTrees[0]))
        {
            CommitChanges();
            return E_LMAPS;
        }
    }
    return 0;
}
//---------------------------------------------------------------------------------------
void Compiler::Clear()
{
    memset(_zones,0,sizeof(_zones));
    _zonecount     = 0;
   _done           = FALSE;


   _randombytes.deleteelements();

    FOREACH(vvector<CBspTree*>,_bTrees, ppT)
    {
    
        FOREACH(vvector<Poly>, (*ppT)->_polys, pPoly)
        {
            pPoly->_lmInfo._lmIndex = -1;
            if(pPoly->_ilmTex)
            {
                GTexSys.RemoveTexture(pPoly->_ilmTex);
            }
        }
    }
    if(_terrTree.Exist())
    {
        FOREACH( vvector<TerLeaf*>, _terrTree.p_leafs, ppl)
        {
            GTexSys.RemoveTexture((*ppl)->lm_Tex);
            (*ppl)->lm_info._lmIndex = -1;
        }
    }

    _pvsPrc.Clear();
    _bTrees.deleteelements();
    _lmProc.NTC_Clear();
    _portPrc.Clear();
    _terrTree.Clear();
    _done=FALSE;
};

//---------------------------------------------------------------------------------------
void Compiler::FromRegistry(BOOL bRead)
{
    if(bRead)
    {
        //_finalBSpFileName  = AfxGetApp()->GetProfileString("BSP","FILENAME");
        _bSaveFinalBSP     = AfxGetApp()->GetProfileInt("BSP","SAVE",0);
        _szipit            = AfxGetApp()->GetProfileInt("BSP","SZIPIT",0);
        _bSaveImages       = AfxGetApp()->GetProfileInt("BSP","SAVETEX",1);
        _lmapIntens        = AfxGetApp()->GetProfileInt("BSP","LMAPINTENS",4);
        _maxFacesLeaf      = AfxGetApp()->GetProfileInt("BSP","MAXFACES",0);
		_bSaveCats         = AfxGetApp()->GetProfileInt("BSP","SAVECATS",1);
        _bSavePortals      = AfxGetApp()->GetProfileInt("BSP","SAVEPORT",1);
        _bPolySortCrit     = AfxGetApp()->GetProfileInt("BSP","SORTCRIT",0);
        _bMergeNarrowLeafs = AfxGetApp()->GetProfileInt("BSP","LEAFMERGE",1);
        _bSaveSkyDom       = AfxGetApp()->GetProfileInt("BSP","SAVESKY",1);
        _bSaveBigTerrain   = AfxGetApp()->GetProfileInt("BSP","SAVEBIGTERRAIN",1);
        _bSaveBigTerrainBSP= AfxGetApp()->GetProfileInt("BSP","SAVEBIGTERRAINBSP",1);

        _bAvoidNonOrt      = AfxGetApp()->GetProfileInt("BSP","AVOIDNONORT",0);
        _bUseUserPriority  = AfxGetApp()->GetProfileInt("BSP","USEUSERPRIOR",0);



        _bConvexEarly      = AfxGetApp()->GetProfileInt("BSP","CONVEXEARLY",0);
        _bLightMaps        = AfxGetApp()->GetProfileInt("BSP","LMAPS",1);
        _lmpowof2          = AfxGetApp()->GetProfileInt("BSP","LMAPSPOW2",1);
        _lmConst          = AfxGetApp()->GetProfileInt("BSP","LMAASPECT",0);
        _b3SampleLM        = AfxGetApp()->GetProfileInt("BSP","LMAPS3S",1);
        _bPvsMethod         = AfxGetApp()->GetProfileInt("BSP","PVSMETH",1);
    

        _bGeticBSP         = AfxGetApp()->GetProfileInt("BSP","GETICBSP",1);
        _bSnapMainBsp      = AfxGetApp()->GetProfileInt("BSP","SNAPFULLBSP",0);
        _bZones            = AfxGetApp()->GetProfileInt("BSP","AUTOZONES",0);

        _bCompressPvs      = AfxGetApp()->GetProfileInt("BSP","COMPRESS",0);
        _bAABBTreeCsgTer   =  AfxGetApp()->GetProfileInt("BSP","CGSTERAABBTREE",0);


        _removeOnePolyLeaf = AfxGetApp()->GetProfileInt("BSP","ROPL",0);
        _portalCutFlag     = AfxGetApp()->GetProfileInt("BSP","PORTCUTBYBOX",0);
        _balance           = AfxGetApp()->GetProfileInt("BSP","BAL",6);
        _lmapMaxSize       = AfxGetApp()->GetProfileInt("BSP","LM_XY",16);
        _bLmPatch          = AfxGetApp()->GetProfileInt("BSP","LM_P",1);
		_bExportFile	   = AfxGetApp()->GetProfileInt("BSP","EX_S",0);
        _lMapLumelSz       = Str2Real(AfxGetApp()->GetProfileString("BSP","LMAP_LUMSZ","64.0"),64.0f);
        
        
        
        
        
    }
    else
    {
        //AfxGetApp()->WriteProfileString("BSP","FILENAME",_finalBSpFileName);
        AfxGetApp()->WriteProfileInt("BSP","SAVE",_bSaveFinalBSP);
        AfxGetApp()->WriteProfileInt("BSP","SZIPIT",_szipit);
        AfxGetApp()->WriteProfileInt("BSP","SAVETEX",_bSaveImages);
        AfxGetApp()->WriteProfileInt("BSP","LMAPINTENS",_lmapIntens);
        AfxGetApp()->WriteProfileInt("BSP","MAXFACES",_maxFacesLeaf);
		AfxGetApp()->WriteProfileInt("BSP","SAVECATS",_bSaveCats);
        AfxGetApp()->WriteProfileInt("BSP","SAVEPORT",_bSavePortals);
        AfxGetApp()->WriteProfileInt("BSP","SORTCRIT",_bPolySortCrit);
        AfxGetApp()->WriteProfileInt("BSP","LEAFMERGE",_bMergeNarrowLeafs);
        AfxGetApp()->WriteProfileInt("BSP","SAVESKY",_bSaveSkyDom);
        AfxGetApp()->WriteProfileInt("BSP","SAVEBIGTERRAIN",_bSaveBigTerrain);
        AfxGetApp()->WriteProfileInt("BSP","SAVEBIGTERRAINBSP",_bSaveBigTerrainBSP);

        AfxGetApp()->WriteProfileInt("BSP","CONVEXEARLY",_bConvexEarly);
        AfxGetApp()->WriteProfileInt("BSP","LMAPS",_bLightMaps);
        AfxGetApp()->WriteProfileInt("BSP","LMAPSPOW2",_lmpowof2);
        AfxGetApp()->WriteProfileInt("BSP","LMAASPECT",_lmConst);
        
        

        AfxGetApp()->WriteProfileInt("BSP","LMAPS3S",_b3SampleLM);

        AfxGetApp()->WriteProfileInt("BSP","AVOIDNONORT",_bAvoidNonOrt);
        AfxGetApp()->WriteProfileInt("BSP","USEUSERPRIOR",_bUseUserPriority);


        AfxGetApp()->WriteProfileInt("BSP","PVSMETH",_bPvsMethod);
        AfxGetApp()->WriteProfileInt("BSP","GETICBSP",_bGeticBSP);
        //_bGeticBSP         = AfxGetApp()->GetProfileInt("BSP","GETICBSP",1);

        AfxGetApp()->WriteProfileInt("BSP","SNAPFULLBSP",_bSnapMainBsp);
        AfxGetApp()->WriteProfileInt("BSP","AUTOZONES",_bZones);

        AfxGetApp()->WriteProfileInt("BSP","COMPRESS",_bCompressPvs);
        AfxGetApp()->WriteProfileInt("BSP","CGSTERAABBTREE",_bAABBTreeCsgTer);


        AfxGetApp()->WriteProfileInt("BSP","ROPL",_removeOnePolyLeaf);
        AfxGetApp()->WriteProfileInt("BSP","PORTCUTBYBOX", _portalCutFlag);
        AfxGetApp()->WriteProfileInt("BSP","BAL",_balance);
        
        AfxGetApp()->WriteProfileInt("BSP","LM_XY",_lmapMaxSize);
        AfxGetApp()->WriteProfileString("BSP","LMAP_LUMSZ",MKSTR("%3.2f",_lMapLumelSz));
        AfxGetApp()->WriteProfileInt("BSP","LM_P",_bLmPatch);
		AfxGetApp()->WriteProfileInt("BSP","EX_S",_bExportFile);
    }
}


