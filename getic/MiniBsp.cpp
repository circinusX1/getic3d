// MiniBsp.cpp: implementation of the MiniBsp class.
//
//------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "z-edmap.h"
#include "Poly.h"
#include "z_ed2Doc.h"
#include "basecont.h"
#include "MiniBsp.h"
#include "Mainfrm.h"
#include "Compiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
    #define _CATCHE(v_) while(0); if(0)
#else
    #define _CATCHE(v_)  catch(v_)
#endif //

//------------------------------------------------------------------------------------------
static  int			Gdepth          = 0; //compiler does for globals
static  int			GMaxDepth       = 0;
static  int			GLoops          = 0;
static  int			GPolys          = 0;
Poly*   MiniBsp::_pWorkingPoly[2]   = {0};
typedef enum _EX_BSP{EX_TOODEEP=1, EX_TONORROR, EX_FRONTBACK}EX_BSP;

//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
CMiniNode* CMiniNode::Back()
{
    if(_nodeidx>0 && _nodeidx[0]>=0)
        return _pBsp->_nodesPtr[_nodeidx[0]];
    return 0;
}

//------------------------------------------------------------------------------------------
CMiniNode* CMiniNode::Front()
{
    if(_nodeidx[1]>=0)
        return _pBsp->_nodesPtr[_nodeidx[1]];
    return 0;
}

//------------------------------------------------------------------------------------------
CMiniNode* CMiniNode::Parent()
{
	if(_idxParent>=0)
        return _pBsp->_nodesPtr[_idxParent];
    return 0;
}

//------------------------------------------------------------------------------------------

struct S_SortByNormal
{
    bool operator()( Poly& a,  Poly& b) const
    {
        if(a._n.isortonormal() && !b._n.isortonormal())
            return 1;
        return 0;
    }
};

//------------------------------------------------------------------------------------------
struct S_SortByAreea 
{
    bool operator()( Poly& a,  Poly& b) const
    {
        return a.GetAreea() > b.GetAreea();
    }
};

struct S_SortBySplitIdx 
{
    bool operator()( Poly& a,  Poly& b) const
    {
        return (DWORD)a._use2splitidx < (DWORD)b._use2splitidx;
    }
};


//------------------------------------------------------------------------------------------
BOOL MiniBsp::Compile(vvector<Poly>& polys, DWORD brFlags, BOOL bLog, BOOL btextConvex)
{
    Clear(); 
    PolyList		loco;
	BOOL			buildDummy  = FALSE;
    _treeflags        = brFlags;
    Gdepth        = 0;
    GLoops        = 0;
	_reachedDepth = 0;
    GMaxDepth     = polys.size() * 2; // limit here by the splits and etc
    _bremES		  = bLog;
    int             idx = 0;
    CWaitCursor     cwc;
    
	_btextconvex   = btextConvex;

    if(IsMainModel())
    {
        PolyList		secondpart;

        FOREACH(vvector<Poly>, polys, pp)
        {
		    pp->_polyflags &= ~SPLITTER_POLY;
            pp->_idxSplitter    =  -1;          // store here the  node idx who split this
            pp->_idxParent      =  idx++;       // make all point to themselfs
		    pp->_fragments		= 0;            // no fragments

            if(pp->_polyprops & FACE_SPLITTER)
            {
                loco.push_back(*pp);
            }
            else
            {
                secondpart.push_back(*pp);
            }
        }
        loco.sort((greater<Poly>&)S_SortBySplitIdx());

        if(COMP()->_bPolySortCrit==1 )
        {
            secondpart.sort((greater<Poly>&)S_SortByAreea());					   // sort by area	
        }
        else if(COMP()->_bPolySortCrit==2)
        {
            secondpart.sort((greater<Poly>&)S_SortByNormal());		
        }
        append(loco, secondpart);

    }
    else
    {
        FOREACH(vvector<Poly>, polys, pp)
        {
		    pp->_polyflags &= ~SPLITTER_POLY;
            pp->_idxSplitter    =  -1;          // store here the  node idx who split this
            pp->_idxParent      =  idx++;       // make all point to themselfs
		    pp->_fragments		= 0;            // no fragments
            loco.push_back(*pp);
        }
        loco.sort((greater<Poly>&)S_SortByNormal());		
    }
    MiniBsp::_pWorkingPoly[0] = 0;         //debug purposes
    MiniBsp::_pWorkingPoly[1] = 0;         //debug purposes

    _pInPolys  = &polys;				   // hold a pointer to input data set	
    _bComplErr = FALSE;
    _dlgWaitOn = 0;
    _timeStart = GetTickCount();
    BuildPlaneArray(loco, brFlags );
    MakeRoot();                         // sdd default root node                    
    UpdateNodeBB(Root(), polys);
    V3 center = Root()->_bbox.GetCenter();
    if(_balance > 20) _balance=20;
    if(_balance < 2)  _balance=2;

    
    CMiniNode*  pNode = _nodesPtr[0];
	pNode->_planeIdx = 0;
	
    _nSafeStack = 0;


    try
    {
        if(brFlags & BRSH_CSGTERRAIN)
        {
            TRACEX("trerr csg\r\n");
        }
        if(brFlags &  BRSH_DONTBSP)
        {
            BuildDummyBSP(polys);
        }
        else if(brFlags & BRSH_CSGTERRAIN && Compiler::PCompiler->_bAABBTreeCsgTer)
		{
			R_TerrainCompile(0, loco);
		}
		else
        {
			R_Compile(0, loco);
        }
    }
	catch(EX_BSP e)
	{
        e;
		Clear();
		if(_dlgWaitOn)
			END_WAIT("");

        if(ThrID == GetCurrentThreadId())
            AfxMessageBox("Error Compiling BSP");

		LOGERR(E_BSP, center, MKSTR("Error BSP1022: Cannot decide front/back :%d. Try to change tolerance (See Global Settings)", Gdepth));
        Compiler::PCompiler->_errDetected=1;

#ifndef _DEBUG
		if(e == EX_FRONTBACK)
		{
			if(GForceBSP)
				buildDummy = GForceBSP;
			else
				return FALSE;
		}
		else
#endif //
			return FALSE;

	}
    catch(...)
    {
		Clear();

		if(_dlgWaitOn)
			END_WAIT("");

        LOGERR(E_BSP, center, "Exception BSP0000: Unexpected. Try to adjust the specified Brush)");
        Compiler::PCompiler->_errDetected=1;
        return FALSE;
    }

    if(buildDummy)
    {
        MakeRoot();                                      
        UpdateNodeBB(Root(), polys);
        BuildDummyBSP(polys);
    }

	if(_dlgWaitOn)
		END_WAIT("");


    FOREACH(vvector<CMiniNode*>, _nodesPtr, ppnode)
    {
        assert((*ppnode)->_planeIdx!=-1);
    }
	return TRUE;
}


//------------------------------------------------------------------------------------------
void MiniBsp::R_TerrainCompile(int nodeIdx, PolyList& polys)
{
	static      int Deepth = 0;
	PolyList	    frontPolys; 
	PolyList	    backPolys;
	CMiniNode*		pNode = _nodesPtr[nodeIdx];
	Deepth++;
	//
	// build splitter plane
	//
#ifdef _DEBUG
	int tst = polys.size();	
#endif //#ifdef _DEBUG

    FindCsgTerrSplitterPlane(polys, pNode);
	PartitionCSGTerrain(pNode, polys, frontPolys, backPolys);

    if(frontPolys.size())
	{
		if(0==backPolys.size() /*|| AllSpliters(frontPolys)*/)
		{
			CMiniNode* fn			 = CreateNode(NODE_LEAF);
			pNode->_nodeidx[N_FRONT] = _nodesPtr.size();
			fn->_polyIdx			 = _polys.size();
			fn->_nPolys				 =  frontPolys.size();
            fn->_idxParent           =  pNode->_idxNodeThis;
			AddNode(fn);

            
            FOREACH(PolyList, frontPolys, pp)
            {
                pp->_planeIdxFinal = fn->_leafIdx; // for lightmaps poly gathering
                if(pp->_polyprops & FACE_OCCLUDER)
                    fn->_nodeflags |= LEAF_HAS_OCCLUDERS;
                _polys.push_back(*pp);
            }

			UpdateNodeBB(fn, frontPolys);
			frontPolys.clear();
		}
        else                                            // continue
		{
			CMiniNode*  fn = CreateNode(0);
			pNode->_nodeidx[N_FRONT]=_nodesPtr.size();
            fn->_idxParent = pNode->_idxNodeThis;
			AddNode(fn);
			R_TerrainCompile(pNode->_nodeidx[N_FRONT], frontPolys);
		}
	}
	if(backPolys.size())	                        // make a solid leaf
	{

        if(_CheckSafeStack(backPolys.size()))
        {
            throw EX_FRONTBACK;
        }

		CMiniNode* bn = CreateNode(0);
		pNode->_nodeidx[N_BACK] = _nodesPtr.size();
		bn->_idxParent = pNode->_idxNodeThis;
		AddNode(bn);
		R_TerrainCompile(pNode->_nodeidx[N_BACK], backPolys);
	}
	else                                            // continue
	{
		CMiniNode* bn = CreateNode(NODE_LEAF|NODE_SOLID);
		pNode->_nodeidx[N_BACK]=_nodesPtr.size();
        bn->_idxParent = pNode->_idxNodeThis;
		AddNode(bn);
	}
    Gdepth--;
}


//------------------------------------------------------------------------------------------
#pragma inline_recursion(on)
#pragma inline_depth(80)

//------------------------------------------------------------------------------------------
BOOL MiniBsp::_FormConvex(PolyList& frontPolys)
{

    if(_btextconvex)
	{
        FOREACH(PolyList, frontPolys, ppoly1)
        {
            FOREACH(PolyList, frontPolys, ppoly2)
            {
                if(ppoly1 != ppoly2)
                {
                    Plane& plane2 = _planes[ppoly2->_planeIdx];
                    if(ON_BACK == ppoly1->Classify(plane2))
                    {
                        return FALSE;
                    }
                }
            }
        }
		return TRUE;
	}
    return FALSE;
}

//------------------------------------------------------------------------------------------

void MiniBsp::R_Compile(int nodeIdx, PolyList& polys)
{

    if(++Gdepth > GMaxDepth )       
	{
		throw(EX_FRONTBACK);
	}
    _reachedDepth = tmax(_reachedDepth, Gdepth);

    if(ThrID == GetCurrentThreadId() && _timeStart != 0)
    {
        if(GetTickCount() - _timeStart > 4000)
        {
            _timeStart=0;
            _dlgWaitOn = 1;
            START_WAIT("Please Wait");
        }
    }

    if(_dlgWaitOn)
    {
        KEEP_WAIT(MKSTR("Compiling Bsp: n:%d.  d:%d",nodeIdx, Gdepth));
    }

	PolyList	    frontPolys; // minimize mem fragmentation
	PolyList	    backPolys;
	CMiniNode*		pNode = _nodesPtr[nodeIdx];

	int splitters = 	Partition(pNode, polys, frontPolys, backPolys);

    //
    // fix partitioning due to floating point error
    //
    if(0 == splitters && backPolys.size())
    {
        append(frontPolys, backPolys);
        backPolys.clear();
    }
    
    if(frontPolys.size())
	{
		if(splitters ==  0 || _FormConvex(frontPolys) || AllSpliters(frontPolys))           // make a leaf of 1 polygon
		{
			CMiniNode* fn			 = CreateNode(NODE_LEAF);
			pNode->_nodeidx[N_FRONT] = _nodesPtr.size();
			fn->_polyIdx			 = _polys.size();
			fn->_nPolys				 =  frontPolys.size();
            fn->_idxParent           =  pNode->_idxNodeThis;
            fn->_planeIdx            =  pNode->_planeIdx;
            fn->_planeIdxFinal       = Gdepth;  // keep depth here
			AddNode(fn);

			FOREACH(PolyList, frontPolys, pp)
            {
                pp->_planeIdxFinal = fn->_leafIdx; // for lightmaps poly gathering
				if(pp->_polyprops & FACE_OCCLUDER)
                    fn->_nodeflags |= LEAF_HAS_OCCLUDERS;
                _polys.push_back(*pp);
            }

            UpdateNodeBB(fn, frontPolys);
			frontPolys.clear();
		}
        else                                            // continue
		{
			CMiniNode*  fn = CreateNode(0);
			pNode->_nodeidx[N_FRONT]=_nodesPtr.size();
            fn->_idxParent = pNode->_idxNodeThis;
            fn->_planeIdxFinal       = Gdepth;  // keep depth here
            AddNode(fn);

            UpdateNodeBB(fn, frontPolys);
			
            R_Compile(pNode->_nodeidx[N_FRONT], frontPolys);
		}
	}

    if(backPolys.size())	                        // make a solid leaf
	{
		CMiniNode* bn = CreateNode(0);
        pNode->_nodeidx[N_BACK] = _nodesPtr.size();
        bn->_idxParent = pNode->_idxNodeThis;
        bn->_planeIdxFinal       = Gdepth;  // keep depth here
		AddNode(bn);
        UpdateNodeBB(bn, backPolys);
		R_Compile(pNode->_nodeidx[N_BACK], backPolys);
	}
	else
	{
		CMiniNode* bn = CreateNode(NODE_LEAF|NODE_SOLID);
		pNode->_nodeidx[N_BACK]=_nodesPtr.size();
        bn->_idxParent = pNode->_idxNodeThis;
        bn->_planeIdx  =  pNode->_planeIdx;
        bn->_bbox._max.setval(0.0); bn->_bbox._min.setval(0.0);
        bn->_planeIdxFinal       = Gdepth; // keep depth here
		AddNode(bn);
	}
    Gdepth--;
}

//------------------------------------------------------------------------------------------
// returns 
int  MiniBsp::FindPLane(Plane& p)
{
    vvector<Plane>::iterator fp = _planes.findelement(p);
    if(fp == _planes.end())
    {
        int index = _planes.size();
        _planes << p;
        return index;
    }
    return fp - _planes.begin();
}
//------------------------------------------------------------------------------------------
// Select the best splitter index in plSpliter list. walks in polys and pick most
// suitable splitter
int MiniBsp::GetBestSplitter(PolyList& polys, Poly* pWantPoly)
{

    if(_pSplPlanes.size())
    {
        int index = _planes.size();
        _planes << *_pSplPlanes.front();
        _pSplPlanes.erase(_pSplPlanes.begin());
        return index;
    }


    int                     splitCnt    = 0;
	int		                retval		= -1;
	DWORD	                bestscore	= INFINIT;
    PolyList::iterator    bpIt        = polys.end();
    int                     maxbacks    = 0;
    int                     maxfronts   = 0;

	FOREACH(PolyList, polys, pPoly1)
	{
		DWORD	score		= 0; 
		DWORD	boths		= 0;
        DWORD   planars     = 0;
		DWORD	backs		= 0;
		DWORD	fronts		= 0;
		Poly&	polySpliter = *pPoly1;

        MiniBsp::_pWorkingPoly[0] = &polySpliter; //debug purposes

		if(!polySpliter.IsSplitter())//_polyflags & SPLITTER_POLY
        {
            if(polySpliter._polyprops & FACE_SPLITTER)
            {
		        polySpliter.SetSplitter();		// mark it as a splitter poly
		        return polySpliter._planeIdx;
            }

		    FOREACH(PolyList, polys, pPoly2)
		    {
			    REL_POS   whereIs;
			    Poly& poly = *pPoly2;

                if(pPoly2 == pPoly1)
                    continue;

                MiniBsp::_pWorkingPoly[1] = &poly; //debug purposes

			    whereIs = poly.Classify(polySpliter);
			    switch(whereIs)
			    {
				    case ON_PLANE:
                        ++planars;
					    break;
				    case ON_FRONT:
					    ++fronts;
					    break;
				    case ON_BACK:
					    ++backs;
					    break;
				    case ON_SPLIT:
					    ++boths;
					    break;
			    }
		    }
            

		    score = abs( long((fronts-backs)+(boths*_balance)) );
		    if(score < bestscore)
		    {
		        bestscore = score;
                bpIt  = pPoly1;
		    }
            splitCnt++;

            maxfronts = tmax((ULONG)maxfronts, (ULONG)fronts);
            maxbacks =  tmax((ULONG)maxbacks,  (ULONG)backs);


        }//if
	}

    // try to balance the detail close to it's root
    // if polygons are still more than _maxFacesLeaf
    // split them in 2 by an ortogonal plane
    if( (_treeflags & BRSH_DETAIL) && 
        Compiler::PCompiler->_maxFacesLeaf!=0)
    {
        if(polys.size() > (UINT)Compiler::PCompiler->_maxFacesLeaf)
        {
            if((UINT)maxbacks >= polys.size()-1) // still convex
            {
                Plane& plane = GetCSGTerOptimSpliterPlane(polys, 0);
                return FindPLane(plane);
            }
        }
    }

	if(bpIt != polys.end())
	{
#ifdef _DEBUG
        const char* tn = bpIt->GetTexs()[GUtex].GetTexName();
#endif //
        Poly& rpoly = (*bpIt);
		Poly* poly = &rpoly;
		poly->SetSplitter();		// mark it as a splitter poly

        //
        // mark all polys having same plane as splitters
        // couse we already psrtition on these
        //
        FOREACH(PolyList, polys, pPoly3)
        {
            if(!pPoly3->IsSplitter())
            {
                if(pPoly3->_planeIdx == poly->_planeIdx)
                    pPoly3->SetSplitter();
            }
        }
		return poly->_planeIdx;
	}

#ifdef __TEST_BSP
    if(0 == splitCnt)
    {
        TRACE("something wrong %d. cannot det front back", GLoops);
        ASSERT(0);
    }
#endif //__FORCE_BSP

    _bComplErr        = TRUE;
	return polys.begin()->_planeIdx; // let creation of a back node, then all will be pushed on front
}

//------------------------------------------------------------------------------------------
void MiniBsp::Trace(int idx)
{
	CMiniNode* pNode = _nodesPtr[idx];
	if(pNode->_nodeflags & NODE_LEAF)
	{
		if(pNode->_nodeflags & NODE_SOLID)
		{
			TRACE("p:%d->n:%d[X]", pNode->_idxParent, pNode->_idxNodeThis);
		}
		else
		{
			TRACE("p:%d->n:%d / l:%d) [polys:%d]",
                    pNode->_idxParent,
                    pNode->_idxNodeThis,
                    pNode->_leafIdx, 
                    pNode->_nPolys);
		}
		TRACE("\r\n");
	}
	if(pNode->_nodeidx[1]>=0)
	{
		TRACE("{(%d)>(%d)}",idx,pNode->_nodeidx[1]);
		Trace(pNode->_nodeidx[1]);
	}
	if(pNode->_nodeidx[0]>=0)
	{
        if(pNode->_nodeidx[1]==-1)
            TRACE("-??-");

		TRACE("{(%d)<(%d)}",idx,pNode->_nodeidx[0]);
		Trace(pNode->_nodeidx[0]);


	}
	TRACE("-");
}


//------------------------------------------------------------------------------------------
// collects all planes and make polys indicating to this planes. The node tree
// is gonna keep instead a whole poly as it's splitter only a plane
void	MiniBsp::BuildPlaneArray(PolyList& polys, DWORD flags)
{
    ASSERT(_planes.size()==0);
    
    // init the splitting planes
    FOREACH(PolyList, polys, pPoly)
    {

        pPoly->Recalc();

        Plane   plane(pPoly->_n, pPoly->_c);
        pPoly->_planeIdx = -1;

        vvector<Plane>::iterator fp = _planes.findelement(plane);

        if(fp == _planes.end())
        {
            pPoly->_planeIdx = _planes.size();
            _planes << plane;
        }
        else
        {
            pPoly->_planeIdx = fp - _planes.begin();
        }
        ASSERT(pPoly->_planeIdx != -1);
    }
}

//------------------------------------------------------------------------------------------
// clips polySoup with this tree. the remaining fragments are added to outPoly
//
void    MiniBsp::ClipBrPolys(vvector<Poly>& polySoup, 
                          vvector<Poly>&    outPlys, 
                          BOOL bRemCoplan,  // remove any coplanars poly
                          BOOL bRemSolid,   // removes the splits that end up in solid space
                          BOOL bremoveUs)   // removes any unnecesarely splits after operation
{
    int idx = 0;

    _timeStart = GetTickCount();
	FOREACH(vvector<Poly>, polySoup, pp)    // prepare for unnecesarely aplit repair anyway
    {
		pp->_polyflags          &=  ~POLY_DELETED;
        pp->_idxSplitter    =  -1;          // store here the  node idx who split this
        pp->_idxParent      =  idx++;       // make all point to themselfs
		pp->_fragments		= 0;            // no fragments
    }
    _pInPolys   = &polySoup;                // hold a pointer to input data set
    _bRemUS		= bremoveUs;                // store removing status her instead passing them recurs..
	_bRemCoplan = bRemCoplan;
	_bClipSolid = bRemSolid;

    _dlgWaitOn = 0;
    _timeStart = GetTickCount();

	R_ClipPolyList(0, polySoup, outPlys);

	if(bremoveUs)
	{
		RepairSplits(polySoup, outPlys);
	}
	if(_dlgWaitOn)
		END_WAIT("");

    _pdroppedpolys = 0;
}                                                

//------------------------------------------------------------------------------------------
// recurvsive function that carves the polySoup->(carve)->outLst
void MiniBsp::R_ClipPolyList(int idxNode, 
                            vvector<Poly>& polySoup, 
                            vvector<Poly>& outLst)
{
    if(idxNode<0)   
    {               // I expect having this couse the 
                    // polys we clip may be over BSP last partition planes
                    // and here we allways in the solid
        // ASSERT(0);
        if(_bClipSolid) 
        {                        // solid space
            if(_pdroppedpolys)
            {
        		FOREACH(vvector<Poly>, polySoup, pL) 
		    		_pdroppedpolys->push_back(*pL);
            }
            return;
		}
		FOREACH(vvector<Poly>, polySoup, pL) //keep bounding allBox of removed geom
				_addedPolysBox.Union(pL->_box);
        append(outLst, polySoup);   
        return;
    }

    REAL        fTemp;
	CMiniNode*	pNode = _nodesPtr[idxNode];

    if(ThrID == GetCurrentThreadId() && _timeStart != 0)
    {
        if(GetTickCount() - _timeStart > 4096)
        {
            _timeStart=0;
            _dlgWaitOn = 1;
            START_WAIT("Please Wait");
        }
    }

	if(_dlgWaitOn)
    {
        KEEP_WAIT(MKSTR("Clipping Polygon: %d ",idxNode));
    }

    if(pNode->IsLeaf())                             // polys landen in the leaf
    {                                               // take the decision
        if(pNode->IsSolid())
        {
            if(_bClipSolid) 
            {                        // solid space
                if(_pdroppedpolys)
                {
        			FOREACH(vvector<Poly>, polySoup, pL) 
		    			_pdroppedpolys->push_back(*pL);
                }
                return;
			}
			FOREACH(vvector<Poly>, polySoup, pL) //keep bounding allBox of removed geom
					_addedPolysBox.Union(pL->_box);
            append(outLst, polySoup);   
        }
        else
        {
            if(!_bClipSolid)                        // empty space
			{
                if(_pdroppedpolys)
                {
        			FOREACH(vvector<Poly>, polySoup, pL) 
		    			_pdroppedpolys->push_back(*pL);
                }
                return;
			}
			FOREACH(vvector<Poly>, polySoup, pL) //keep bounding allBox of removed geom
					_addedPolysBox.Union(pL->_box);
            append(outLst, polySoup);   
        }
        return;
    }

    vvector<Poly>    backPolys(polySoup.size());    // minimize mem fragmentation
    vvector<Poly>    frontPolys(polySoup.size());

    FOREACH(vvector<Poly>, polySoup, ppSpPoly)      // for each poly in soup
    {
        Poly& pSpPoly = *ppSpPoly;
        REL_POS rp1   = pSpPoly.Classify(pNode->GetPlane());
    
        switch (rp1)
	    {
		    case ON_PLANE:
                fTemp = Vdp(pSpPoly._n, pNode->GetPlane()._n)-1.f;
                if(IsZero(fTemp))   // poly and splitter are on same plane
                {
                    if(_bRemCoplan) // removing coplanar pass in back list
                    {
                          backPolys.push_back(pSpPoly);
                    }
                    else            // keeping coplanar, pass in front list
                    {
                         frontPolys.push_back(pSpPoly);
                    }
                }
                else               // on same plane but facing oppozite, pass back anyway
                {
                    // remove opozite plane same polys (cut poly overlaps on regular poly)
                    if(_bClipSolid == FALSE)
                    {
                        frontPolys.push_back(pSpPoly);
                    }
                    else
                    {
                        backPolys.push_back(pSpPoly);
                    }
                }
			    break;
            case ON_FRONT:
                frontPolys.push_back(pSpPoly);
			    break;
            case ON_BACK:
			    backPolys.push_back(pSpPoly);
			    break;
            case ON_SPLIT:
                switch(_cutflag)
                {
                    default:
                    case CUT_POLYS:
                        {
                            Poly fp;
                            Poly bp;
                    
                            // the initial parent polygon keep track of how many times was it split
                            // notice the firt splut is 2 then  only one
                            // this is done only for removing unnecesarely splits
					        if(_pInPolys->at(pSpPoly._idxParent)._fragments == 0)
						        _pInPolys->at(pSpPoly._idxParent)._fragments+=2;
					        else
						        _pInPolys->at(pSpPoly._idxParent)._fragments++;

                            pSpPoly.Split(pNode->GetPlane(),fp,bp);

                            // make thiese fragments remembering their original parent
					        fp._idxParent = pSpPoly._idxParent;
					        bp._idxParent = pSpPoly._idxParent;
					        // and thei node that cut their parent
					        fp._idxSplitter = idxNode;
					        bp._idxSplitter = idxNode;
                            // mark parnt poly as deleted
					       // pSpPoly._flags |= POLY_DELETED;
                            frontPolys.push_back(fp);
                            backPolys.push_back(bp);
                        }
                        break;
                    case PART_ON_FRONT:
                        frontPolys.push_back(pSpPoly);
                        break;
                }
			    break;
        } 
    }

    if(backPolys.size() && pNode->_nodeidx[N_BACK]!=-1)        //send back fragments on back
    {
		R_ClipPolyList(pNode->_nodeidx[N_BACK], backPolys, outLst);   
    }
    if(frontPolys.size() && pNode->_nodeidx[N_FRONT]!=-1)       //send front fragments on front
    {
		R_ClipPolyList(pNode->_nodeidx[N_FRONT], frontPolys, outLst); 
    }
    return;
}

//------------------------------------------------------------------------------------------
// helping foo: collects in outs all fragments that their parent are the same
void collect(int index, vvector<Poly*>& fragments, vvector<Poly>& outs)
{
	FOREACH(vvector<Poly>, outs,ppop)
	{
        Poly& rp = *ppop;
		if(ppop->_idxParent==-1)
			continue;
		if(ppop->_idxParent!=index)
			continue;
		fragments.push_back(&rp);
	}
}

//------------------------------------------------------------------------------------------
// walks in fragments and check if all have survived. if the case replace them with the 
// original parent. this can be improved keeping all parents durring clipping process
// but this is enough
void	MiniBsp::RepairSplits(vvector<Poly>& ins , vvector<Poly>& outs)
{
	FOREACH(vvector<Poly>, ins ,pip)
	{
		if(pip->_fragments)
		{
			vvector<Poly*>	fragments;
			collect(pip->_idxParent, fragments, outs); // get fragments for pip
		
			if(fragments.size() == pip->_fragments)    // see if all survived
			{
				FOREACH(vvector<Poly*>,fragments,ppp)   
				{
					(*ppp)->_polyflags|=POLY_DELNAILED;    // mark them deleted
				}
                //pip->_flags &= ~POLY_DELETED;
				outs.push_back(*pip);                  // add parent
			}
		}
	}
    

	FOREACH(vvector<Poly>, outs, ppo)                 // delete the marked ones
	{
        //ppo->_flags &= ~POLY_DELETED;
		if(ppo->_polyflags & POLY_DELNAILED)
		{
			ppo = outs.erase(ppo);
            if(ppo!=outs.begin())
                --ppo;
            if(outs.size()==0)
                break;
		}
	}
}

//------------------------------------------------------------------------------------------
// places each vertex in unique position. returns the initla vertex count
int    MiniBsp::AddUniqueVertexes(vvector<V3>& globPool)
{
    int vertexes = 0;
    FOREACH(vvector<Poly>, _polys, pPoly)
    {
        FOREACH(vvector<Vtx2>, pPoly->_vtci, pVtci)
        {
            V3& v = pVtci->_xyz;

            vvector<V3>::iterator fi = find(globPool.begin(), globPool.end(), v);
            if(fi == globPool.end())
            {
                pVtci->_index = globPool.size();
                globPool << v;
            }
            else
            {
                pVtci->_index = fi - globPool.begin();
            }
            vertexes++;
        }
    }
    return vertexes;
}

void MiniBsp::PartitionCSGTerrain(CMiniNode*  pNode, 
							      PolyList& polys, 
							      PolyList& frontPolys, 
							      PolyList& backPolys)
{
	if(Compiler::PCompiler->_break)
		throw 0;
	
	while(polys.size())
	{
		Poly& curPoly = polys.back();
	    REL_POS   whereIs = curPoly.Classify(pNode->GetPlane());
		switch(whereIs)
		{
			case ON_PLANE:
				if(IsZero(Vdp(curPoly._n,  pNode->GetPlane()._n)-1.f), EPSMED)
                {
					frontPolys.push_back(curPoly);
                }
				else
                {
					backPolys.push_back(curPoly);
                }
				break;
            case ON_SPLIT:
                switch(_cutflag)
                {
                    default:
                    case CUT_POLYS:
                        {
				            Poly	frontPoly;
				            Poly	backPoly;
                            curPoly.Split(pNode->GetPlane(), frontPoly, backPoly);

					        if(curPoly.IsSplitter())
					        {
						        frontPoly.SetSplitter();
						        backPoly.SetSplitter();
					        }

                            backPolys.push_back(backPoly);
				            frontPolys.push_back(frontPoly);
                        }
                        break;
                    case PART_ONBOTH:
                        {
                            backPolys.push_back(curPoly);
	    			        frontPolys.push_back(curPoly);
                        }
                        break;
                }
                break;
			case ON_FRONT:
				frontPolys.push_back(curPoly);
				break;
			case ON_BACK:
				backPolys.push_back(curPoly);
				break;
            default:
                break;
		}
        polys.pop_back();
	}
    UpdateNodeBB(pNode, frontPolys);
    UpdateNodeBB(pNode, backPolys);
}

int	 MiniBsp::Partition(CMiniNode* pNode, PolyList& polys, PolyList&  frontPolys, PolyList& backPolys)
{
	if(Compiler::PCompiler->_break)
		throw 0;

	pNode->_planeIdx   = GetBestSplitter(polys);

	int	 splitters    = polys.size();	      
	while(polys.size())
	{
		Poly& curPoly = polys.back();
		if(curPoly.IsSplitter())
			splitters--;

        REL_POS   whereIs = curPoly.Classify(pNode->GetPlane());
		switch(whereIs)
		{
			case ON_PLANE:
				if(IsZero(Vdp(curPoly._n,  pNode->GetPlane()._n)-1.f,EPSMED))
				{
					curPoly.SetSplitter();
					frontPolys.push_back(curPoly);
				}
				else
					backPolys.push_back(curPoly);
				break;
			case ON_FRONT:
				frontPolys.push_back(curPoly);
				break;
			case ON_BACK:
				backPolys.push_back(curPoly);
				break;
			case ON_SPLIT:
				{
                    Poly	frontPoly;
                    Poly	backPoly;

                    // used to remake original poly if has been split by bsp with no reason
                    /** NOT VIABLE AS LONG WE MASPLIT MAIN BIG GEOMETRY
                    if(_pInPolys->at(pSpPoly._idxParent)._fragments == 0)
                    _pInPolys->at(pSpPoly._idxParent)._fragments+=2;
                    else
                    _pInPolys->at(pSpPoly._idxParent)._fragments++;
                    */

                    curPoly.Split(pNode->GetPlane(), frontPoly, backPoly);
					if(curPoly.IsSplitter())
					{
						frontPoly.SetSplitter();
						backPoly.SetSplitter();
					}
                    // used to remake original poly if has been split by bsp with no reason
                    /** NOT VIABLE AS LONG WE MASPLIT MAIN BIG GEOMETRY

                    fp._idxParent = pSpPoly._idxParent;
                    bp._idxParent = pSpPoly._idxParent;
                    fp._idxSplitter = pNode->_idxNodeThis; // ! used
                    bp._idxSplitter = pNode->_idxNodeThis; // ! used

                    // later on if all fragments survived, we may rebuild original polygon
                    // and make if available from both leafs ???
                    */  
                    backPolys.push_back(backPoly);
                    frontPolys.push_back(frontPoly);
				}
				break;
            default:
                break;
		}
        polys.pop_back();
	}
	return splitters;

}

Plane   MiniBsp::GetCSGTerOptimSpliterPlane(PolyList& polys, int moe)
{
    Box allBox;
	Box oneBox(V0,V3(100, 100, 100));

    //
    // find allBox of poly
    //
    FOREACH(PolyList, polys, pp){
        pp->Recalc();
        allBox.Union(pp->_box);
    }

    //
    // see wich extend is bigger and build a plane divinding biggest
    // dim in half
    //
	V3 allEx  = allBox.GetExtends();
	V3 oneEx  = oneBox.GetExtends();
    V3 ex     = allBox.GetExtends();
    V3 c      = allBox.GetCenter();

    REAL    mx = tmax(ex.x , ex.y);
    mx         = tmax(mx,    ex.z);

    
    if(ex.x  == mx)
    {
        ex.x = 1;
        ex.y = 0;
        ex.z = 0;
    }
    else if(ex.y == mx)
    {
        ex.x = 0;
        ex.y = 1;
        ex.z = 0;
    }else
    {
        ex.x = 0;
        ex.y = 0;
        ex.z = 1;
    }

    // make all of them on front to nail down the tree
    if(moe)
    {
	    oneEx     *= 8;
	    if((allEx.x <=  oneEx.x &&  
           allEx.y <= oneEx.y   && 
           allEx.z <=  oneEx.z) || polys.size() <= (UINT)moe)
        {
		    c = allBox._min; // one unit down
            c.x -= 1;
            c.y -= 1;
            c.z -= 1;
	    }
    }
    return Plane(ex,c);
}


void MiniBsp::FindCsgTerrSplitterPlane(PolyList& polys, CMiniNode* pNode)
{
    Plane& plane     = GetCSGTerOptimSpliterPlane(polys);
    pNode->_planeIdx = FindPLane(plane);

    FOREACH(PolyList, polys, pPoly3)
    {
        if(!pPoly3->IsSplitter())
        {
            if(pPoly3->_planeIdx == pNode->_planeIdx)
                pPoly3->SetSplitter();
        }
    }
}

void MiniBsp::UpdateNodeBB(CMiniNode* pNode, PolyList& polys)
{
    FOREACH(PolyList,  polys, poly)
    {
        poly->Recalc();
        pNode->_bbox.Union(poly->_box);
        Root()->_bbox.Union(poly->_box);
    }
}

void MiniBsp::UpdateNodeBB(CMiniNode* pNode, vvector<Poly>& polys)
{
    FOREACH(vvector<Poly>,  polys, poly)
    {
        poly->Recalc();
        pNode->_bbox.Union(poly->_box);
        Root()->_bbox.Union(poly->_box);
    }
}



void	MiniBsp::Clear()
{
	_nodesPtr.deleteelements();
	_planes.clear();
	_polys.clear();
	Gdepth		= 0;
	GMaxDepth	= 0;
    _lefmost    = 0;
    _treeflags  = 0;
    _treeprops  = 0;
}

BOOL MiniBsp::BuildDummyBSP(vvector<Poly>& polys)
{
    // place everithing in the root, and make root a leaf
    FOREACH(vvector<Poly>, polys, pp)
	    _polys.push_back(*pp);

    CMiniNode* rn = Root();

    rn->_nodeflags  |=  NODE_LEAF;
    rn->_polyIdx	= 0;
    rn->_nPolys		= _polys.size();
    rn->_idxParent  = -1;
    rn->_planeIdx	= 0;		// duminess
    UpdateNodeBB(rn, _polys);
    return TRUE;
}

BOOL    MiniBsp::_CheckSafeStack(int backSize)
{
    if(0 ==_nSafeStack)
    {
        _safeStack[0] = backSize;
        _nSafeStack=1;
        return FALSE;
    }
    for(int s=0; s < _nSafeStack; s++)
    {
        if(_safeStack[s] != backSize)
        {
            _safeStack[0] = backSize;
            _nSafeStack=1;
            return FALSE;
        }
    }
    _safeStack[_nSafeStack++] = backSize;
    return _nSafeStack > 32;
}



//------------------------------------------------------------------------------------------
BOOL MiniBsp::AllSpliters(PolyList& polyList)
{
    int splcount = 0;
	FOREACH(PolyList, polyList, pPoly1)
	{
		if(pPoly1->IsSplitter())
            ++splcount;
    }
    return splcount==polyList.size();
}
