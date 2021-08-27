// z_ed2Doc.cpp : implementation of the CZ_ed2Doc class
//

#include "stdafx.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "mainfrm.h"
#include "Z_ed2View.h"
#include "Z_ed3View.h"
#include "DlgMurrChs.h"

#include <algorithm>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
BOOL    GSuported=0;

//---------------------------------------------------------------------------------------
Box   CZ_ed2Doc::GetReferenceBrushBox()
{
    DeleteUncomittedNewBrushes();

    // do alignment by selection box
    _selBox.Recalc();
    
    if(IsSelBoxOn())                                
        return _selBox._box;
    
    if(_pSelBrushes.size())
        return _pSelBrushes.back()->_box;
    return _pSelItems.back()->GetBox();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnXalign() 
{
    SaveTempFile();
    if(_pSelBrushes.size())
    {
        V3 diff ;
        V3 xc = GetReferenceBrushBox().GetCenter();

        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            V3 diff = xc - (*ppb)->_box.GetCenter();
            diff.y  = 0;
            diff.z  = 0;
            (*ppb)->_t = diff;      // keep th last translation here to alow (not exact by floating) a repos if canceled

            (*ppb)->Move(diff);
            (*ppb)->Recalc();
        }
        Recut();
    }
    if(_pSelItems.size())
    {
        V3 xc = _pSelItems[0]->_t;

        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
            (*ppItem)->_t.x = xc.x;
        }
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateXalign(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }
	pCmdUI->Enable(_pSelBrushes.size()>1 || _pSelItems.size()>1);	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnYalign() 
{
    SaveTempFile();
	if(_pSelBrushes.size())
    {
        V3 xc = GetReferenceBrushBox().GetCenter();
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            V3 diff = xc - (*ppb)->_box.GetCenter();
            diff.x  = 0;
            diff.z  = 0;
            (*ppb)->_t = diff;// keep th last translation here to alow (not exact by floating) a repos if canceled
            (*ppb)->Move(diff);
            (*ppb)->Recalc();
        }
        Recut();
    }

    if(_pSelItems.size())
    {
        V3 xc = _pSelItems[0]->_t;

        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
            (*ppItem)->_t.y = xc.y;
        }
    }
    Invalidate();

}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateYalign(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

	pCmdUI->Enable(_pSelBrushes.size()>1 || _pSelItems.size()>1);	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnZalign() 
{
    SaveTempFile();
    if(_pSelBrushes.size())
    {
        V3 xc = GetReferenceBrushBox().GetCenter();
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            V3 diff = xc - (*ppb)->_box.GetCenter();
            diff.x  = 0;
            diff.y  = 0;
            (*ppb)->_t = diff;// keep th last translation here to alow (not exact by floating) a repos if canceled
            (*ppb)->Move(diff);
            (*ppb)->Recalc();
        }
        Recut();
    }
    if(_pSelItems.size())
    {
        V3 xc = _pSelItems[0]->_t;
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
            (*ppItem)->_t.z = xc.z;
        }
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateZalign(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 || _pSelItems.size()>1);	
}


//---------------------------------------------------------------------------------------
static Box GetSelBrushesBox(PBrushes& selbrushes)
{
    Box box;
    FOREACH(PBrushes, selbrushes, ppb)
    {
        box.Union((*ppb)->_box);
    }
    return box;
}

//---------------------------------------------------------------------------------------
static V3    GetFilledSpace(PBrushes& selbrushes)
{
    V3 allDims;
    FOREACH(PBrushes, selbrushes, ppb)
    {
        allDims+=(*ppb)->_box.GetExtends();
    }
    return allDims;
}
//---------------------------------------------------------------------------------------
// sorts brush by x displacement
int SortByX(const void* b1, const void* b2)
{
    REAL displ = ((Brush*)b1)->_box.GetCenter().x-((Brush*)b2)->_box.GetCenter().x;
    if(displ > 0)
        return 1;
    else if(displ < 0)
        return -1;
    return 0;
}

//---------------------------------------------------------------------------------------
// sorts brush by x displacement
int SortByY(const void* b1, const void* b2)
{
    REAL displ = ((Brush*)b1)->_box.GetCenter().y-((Brush*)b2)->_box.GetCenter().y;
    if(displ > 0)
        return 1;
    else if(displ < 0)
        return -1;
    return 0;

}

//---------------------------------------------------------------------------------------
// sorts brush by x displacement
int SortByZ(const void* b1, const void* b2)
{
    REAL displ = ((Brush*)b1)->_box.GetCenter().z-((Brush*)b2)->_box.GetCenter().z;
    if(displ > 0)
        return 1;
    else if(displ < 0)
        return -1;
    return 0;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
// Next 3 function makes the interval between brushes to have same gap on selected axes
//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnXinter() 
{
    SaveTempFile();
	if(_pSelBrushes.size())
    {
        Box   sbBox  =  GetSelBrushesBox(_pSelBrushes);       // all box
        V3     brSum  =  GetFilledSpace(_pSelBrushes);          // all filled
        REAL   gap    =  (sbBox.GetExtends().x - brSum.x)/_pSelBrushes.size();
        // now move them to leave gaps equdistant
        // sort them by x
        qsort((void*)&_pSelBrushes[0], _pSelBrushes.size(), sizeof(Brush*), SortByX);
    
        Box    firstBox;                         // prev and next boxes
        Box    curBox;                         // prev and next boxes
        FOREACH(PBrushes, _pSelBrushes, ppBrush)
        {
            if(ppBrush == _pSelBrushes.begin())     // get reference
            {
                firstBox = (*ppBrush)->_box;
                continue;
            }
            curBox = (*ppBrush)->_box;
            V3 nextCenter = firstBox.GetCenter() +  
                            firstBox.GetExtends().x/2.f + 
                            V3(gap,0,0) + 
                            curBox.GetExtends().x/2.f;
            V3 toMove     = nextCenter-curBox.GetCenter();
            toMove.z=0;
            toMove.y=0;
            (*ppBrush)->Move(toMove);
            (*ppBrush)->Recalc();
            firstBox = (*ppBrush)->_box;

        }
        Recut();
    }

    if(_pSelItems.size())
    {

        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
        }
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnYinter() 
{
    SaveTempFile();
	if(_pSelBrushes.size())
    {
        Box   sbBox  =  GetSelBrushesBox(_pSelBrushes);       // all box
        V3     brSum  =  GetFilledSpace(_pSelBrushes);          // all filled
        REAL   gap    =  (sbBox.GetExtends().y - brSum.y)/_pSelBrushes.size();
        // now move them to leave gaps equdistant
        // sort them by x
        qsort((void*)&_pSelBrushes[0], _pSelBrushes.size(), sizeof(Brush*), SortByY);
    
        Box    firstBox;                         // prev and next boxes
        Box    curBox;                         // prev and next boxes
        FOREACH(PBrushes, _pSelBrushes, ppBrush)
        {
            if(ppBrush == _pSelBrushes.begin())     // get reference
            {
                firstBox = (*ppBrush)->_box;
                continue;
            }
            curBox = (*ppBrush)->_box;
            V3 nextCenter = firstBox.GetCenter() +  
                            firstBox.GetExtends().y/2.f + 
                            V3(gap,0,0) + 
                            curBox.GetExtends().y/2.f;
            V3 toMove     = nextCenter-curBox.GetCenter();
            toMove.z=0;
            toMove.x=0;
            (*ppBrush)->Move(toMove);
            (*ppBrush)->Recalc();
            firstBox = (*ppBrush)->_box;

        }
        Recut();
    }
    if(_pSelItems.size())
    {
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
        }
    }

    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnZinter() 
{
    SaveTempFile();
	if(_pSelBrushes.size())
    {
        Box   sbBox  =  GetSelBrushesBox(_pSelBrushes);       // all box
        V3     brSum  =  GetFilledSpace(_pSelBrushes);          // all filled
        REAL   gap    =  (sbBox.GetExtends().z - brSum.z)/_pSelBrushes.size();
        // now move them to leave gaps equdistant
        // sort them by x
        qsort((void*)&_pSelBrushes[0], _pSelBrushes.size(), sizeof(Brush*), SortByZ);
    
        Box    firstBox;                         // prev and next boxes
        Box    curBox;                         // prev and next boxes
        FOREACH(PBrushes, _pSelBrushes, ppBrush)
        {
            if(ppBrush == _pSelBrushes.begin())     // get reference
            {
                firstBox = (*ppBrush)->_box;
                continue;
            }
            curBox = (*ppBrush)->_box;
            V3 nextCenter = firstBox.GetCenter() +  
                            firstBox.GetExtends().z/2.f + 
                            V3(gap,0,0) + 
                            curBox.GetExtends().z/2.f;
            V3 toMove     = nextCenter-curBox.GetCenter();
            toMove.x=0;
            toMove.y=0;
            (*ppBrush)->Move(toMove);
            (*ppBrush)->Recalc();
            firstBox = (*ppBrush)->_box;

        }
        Recut();
    }
    if(_pSelItems.size())
    {
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
        }
    }

    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateXinter(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 || _pSelItems.size()>1);	
	
}
//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateYinter(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

	pCmdUI->Enable((_selBox._brushflags & BRSH_SELVIS && _pSelBrushes.size()>1) || _pSelItems.size()>1);
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateZinter(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable((_selBox._brushflags & BRSH_SELVIS && _pSelBrushes.size()>1) || _pSelItems.size()>1);	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnXdim() 
{
    if(_pSelBrushes.size())
    {
        REAL xdim = GetReferenceBrushBox().GetExtends().x;
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            V3  vs;
            vs.x = xdim - (*ppb)->_box.GetExtends().x;
            (*ppb)->Scale(vs,V0,1);
        }
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateXdim(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 ||_pSelItems.size()>1);		
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnYdim() 
{
    if(_pSelBrushes.size())
    {
        REAL ydim = GetReferenceBrushBox().GetExtends().y;
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            V3  vs;
            vs.y = ydim - (*ppb)->_box.GetExtends().y;
            (*ppb)->Scale(vs,V0,1);
        }
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateYdim(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 ||_pSelItems.size()>1);		
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnZdim() 
{
    SaveTempFile();
    if(_pSelBrushes.size())
    {
        REAL zdim = GetReferenceBrushBox().GetExtends().z;
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            V3  vs;
            vs.z = zdim - (*ppb)->_box.GetExtends().z;
            (*ppb)->Scale(vs,V0,1);
        }
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateZdim(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 ||_pSelItems.size()>1);		
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnXyzdim() 
{
    SaveTempFile();
    if(_pSelBrushes.size())
    {
        V3 rex = GetReferenceBrushBox().GetExtends();
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            V3  vs = rex - (*ppb)->_box.GetExtends();
            (*ppb)->Scale(vs,V0,1);
        }
    }
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateXyzdim(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 ||_pSelItems.size()>1);		
}


//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::DeleteUncomittedNewBrushes()
{
    FOREACH(PBrushes,  _pSelBrushes, a)
    {
        if((*a)->_brushflags & BRSH_NEW)
        {
            delete *a;
            a = _pSelBrushes.erase(a);
            if(a != _pSelBrushes.begin())
                --a;
            if(_pSelBrushes.size()==0)
                break;
        }
    }
}


//---------------------------------------------------------------------------------------
void    CZ_ed2Doc::DeleteUncomittedNewItems()
{
	if(_pSelItems.size())
	{
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
			if((*ppItem)->_flags & BRSH_NEW)           
			{
				delete (*ppItem);
				ppItem = _pSelItems.erase(ppItem);
                if(ppItem != _pSelItems.begin())
                    ppItem--;
                if(_pSelItems.size()==0)
                    break;
			}
        }
	}
}


//---------------------------------------------------------------------------------------
// arrange more than one brus right next to end of previous brush
void CZ_ed2Doc::OnLr() 
{
    SaveTempFile();

	if(_pSelBrushes.size()==0 || _pSelItems.size()==0)
		return;

    qsort((void*)&_pSelBrushes[0], _pSelBrushes.size(), sizeof(Brush*), SortByX);
    //place them one after each other

    Box    firstBox;                         // prev and next boxes
    Box    curBox;                           // prev and next boxes
    FOREACH(PBrushes, _pSelBrushes, ppBrush)
    {
        if(ppBrush == _pSelBrushes.begin())     // get reference
        {
            firstBox = (*ppBrush)->_box;
            continue;
        }
        curBox = (*ppBrush)->_box;
        V3 nextCenter = firstBox.GetCenter() +  
                        firstBox.GetExtends().x/2.f + 
                        curBox.GetExtends().x/2.f;
        V3 toMove     = nextCenter-curBox.GetCenter();
        toMove.z=0;
        toMove.y=0;
        (*ppBrush)->Move(toMove);
        (*ppBrush)->Recalc();
        firstBox = (*ppBrush)->_box;

    }
    Recut();
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateLr(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

     pCmdUI->Enable(_pSelBrushes.size()>1 );	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnTb() 
{
    SaveTempFile();

	if(_pSelBrushes.size()==0 || _pSelItems.size()==0)
		return;
    qsort((void*)&_pSelBrushes[0], _pSelBrushes.size(), sizeof(Brush*), SortByY);


    Box    firstBox;                         // prev and next boxes
    Box    curBox;                           // prev and next boxes
    FOREACH(PBrushes, _pSelBrushes, ppBrush)
    {
        if(ppBrush == _pSelBrushes.begin())     // get reference
        {
            firstBox = (*ppBrush)->_box;
            continue;
        }
        curBox = (*ppBrush)->_box;
        V3 nextCenter = firstBox.GetCenter() +  
                        firstBox.GetExtends().y/2.f + 
                        curBox.GetExtends().y/2.f;
        V3 toMove     = nextCenter-curBox.GetCenter();
        toMove.x=0;
        toMove.z=0;
        (*ppBrush)->Move(toMove);
        (*ppBrush)->Recalc();
        firstBox = (*ppBrush)->_box;

    }
    Recut();

    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateTb(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 );	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnFb() 
{
    SaveTempFile();

	if(_pSelBrushes.size()==0 || _pSelItems.size()==0)
		return;
    qsort((void*)&_pSelBrushes[0], _pSelBrushes.size(), sizeof(Brush*), SortByZ);


    Box    firstBox;                         // prev and next boxes
    Box    curBox;                           // prev and next boxes
    FOREACH(PBrushes, _pSelBrushes, ppBrush)
    {
        if(ppBrush == _pSelBrushes.begin())     // get reference
        {
            firstBox = (*ppBrush)->_box;
            continue;
        }
        curBox = (*ppBrush)->_box;
        V3 nextCenter = firstBox.GetCenter() +  
                        firstBox.GetExtends().z/2.f + 
                        curBox.GetExtends().z/2.f;
        V3 toMove     = nextCenter-curBox.GetCenter();
        toMove.x=0;
        toMove.y=0;
        (*ppBrush)->Move(toMove);
        (*ppBrush)->Recalc();
        firstBox = (*ppBrush)->_box;

    }
    Recut();
    Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnUpdateFb(CCmdUI* pCmdUI) 
{
    if(_compiling){
        pCmdUI->Enable(0);
        return;
    }

    pCmdUI->Enable(_pSelBrushes.size()>1 );	
}

void    CZ_ed2Doc::SelectVertexes(Box& selBox, Brush* ppB)
{
    if(! (ppB->_brushflags & BRSH_BIGTERRAIN))
        return;

    //CBigTerrain* pbt = (CBigTerrain*)p_dummyBrush->_pUsrData;
    //vector<V3>  tervex;
  //  pbt->CalcVexes(tervex);
    
}

/*
if(!n_xtiles || 0==p_dummyBrush) return;
    V3              ex = b_box.GetExtends();
    REAL            xS = (ex.x) / (REAL)n_xtiles;
    REAL            zS = (ex.z) / (REAL)n_ztiles;
    V3              vWalk1,vWalk2, vmin = b_box._min;
    BYTE            lc[4];
    UV              tc0,tc1;
    size_t           texAstage = 1<<GUtex;
    BOOL            untt[4] = {Flags()&TERR_UTT0,
                               Flags()&TERR_UTT1,
                               Flags()&TERR_UTT2,
                               Flags()&TERR_UTT3};


    if(p_dummyBrush->IsSelected())
        glColor4ubv(CLR_WHITE);
    else
    {
        if(d3==0)
            glColor4ubv(CLR_TERRAIN);
        else
            glColor4ubv(CLR_NORMAL);
    }

    if(d3)
    {
        Ta.Reset();
        Ta.Bind(Texs(), GUtex);
    }

    for(int x=0 ; x < n_xtiles; x++)
    {
        if(d3)
        {
            glBegin(GL_TRIANGLE_STRIP);
        }
        else
            glBegin(GL_LINE_STRIP);

        vWalk1.x = vmin.x + x * xS;
        vWalk2.x = vmin.x + (x+1) * xS;

        for(int z=0 ; z < n_ztiles; z++)
        {
            
            vWalk1.z = vmin.z + z * zS;
            vWalk1.y = *(element(&v_vxes, x, z, n_xtiles));
            vWalk1.y /= 255.0;
            vWalk1.y *= ex.y;
            vWalk1.y += vmin.y;

            vWalk2.z = vWalk1.z;
            vWalk2.y = *(element(&v_vxes, (x+1), z, n_xtiles));
            vWalk2.y /= 255.0;
            vWalk2.y *= ex.y;
            vWalk2.y += vmin.y;

            if(GCullTerrCell && DOC()->_cam.IsCulled(&vWalk1) && DOC()->_cam.IsCulled(&vWalk2))
            {
                continue;
            }
            
            if(d3==0 && !p_dummyBrush->IsSelected())
            {
                lc[0] = CLR_TERRAIN[0] + ((vWalk1.y-b_box._min.z) / b_box.GetExtends().z * 64);
                lc[1] = CLR_TERRAIN[1] + ((vWalk1.y-b_box._min.z) / b_box.GetExtends().z * 64);
                lc[2] = CLR_TERRAIN[2] + ((vWalk1.y-b_box._min.z) / b_box.GetExtends().z * 64);
                lc[3] = CLR_TERRAIN[3];
                glColor4ubv(lc);
            }

            if(untt[GUtex])    
            {
                tc0.setval((vWalk1.x-vmin.x)/ex.x, 1-(vWalk1.z-vmin.z)/ex.z);
            }
            else
            {
                if((z&1)==0)
                    tc0.setval(0,0);
                else
                    tc0.setval(0,1);
            }

            if(untt[GUtex])    
            {
                tc1.setval((vWalk2.x-vmin.x)/ex.x, 1-(vWalk2.z-vmin.z)/ex.z);
            }
            else
            {
            if((z&1)==0)
                tc1.setval(1,0);
            else
                tc1.setval(1,1);
            }
            if(d3 && (n_stage & texAstage))
            {
                AnimateSurfaceTexture(tc0, tc1);
            }

            glTexCoord2fv((REAL*)tc0);
            glVertex3fv((REAL*)vWalk1);
            glTexCoord2fv((REAL*)tc1);
            glVertex3fv((REAL*)vWalk2);
                        
        }
        glEnd();
    }
*/

void    CZ_ed2Doc::SelectByBox()
{
    _selBox.Recalc();
    DeselectAll();

    switch(_selmode)
    {
        case SEL_BRUSH:
            {
                PBrushes* pBSs = GetPrimitives();
                FOREACH(PBrushes, (*pBSs), ppB)    
                {
                    if(_curgroup > 0 && (*ppB)->_group != _curgroup)
                        continue;

                    if((*ppB)->_brushflags & BRSH_BIGTERRAIN && _selBox._box.IsTouchesBox((*ppB)->_box))
                    {
    	                SelectVertexes(_selBox._box, *ppB);
                    }
                    else if(_selBoxByTouch)
                    {
                        if(_selBox._box.IsTouchesBox((*ppB)->_box))
                        {
			                SelectBrush(*ppB);
                        }
                    }
                    else
                    {
                        if(_selBox._box.ContainPoint((*ppB)->_box._min) && 
		                        _selBox._box.ContainPoint((*ppB)->_box._max))
                        {
			                SelectBrush(*ppB);
                        }
                    }
                }
            }
            break;
        case SEL_ITEMS:
            {
        
                vvector<SceItem*>* pItmsPtr = DOC()->GetItems();
                FOREACH(vvector<SceItem*>, (*pItmsPtr), ppItem)
                {
			        // the movement on that coord has ben blocked by X or Y key
                    if(_selBox._box.ContainPoint((*ppItem)->_t))
                    {
                        SelectItem((*ppItem));
                    }
                }
            }
            break;
        case SEL_FACE:
            {
                PBrushes* pBSs = GetPrimitives();
                FOREACH(PBrushes, (*pBSs), ppB)    
                {
                    if(_curgroup > 0 && (*ppB)->_group != _curgroup)
                        continue;

                    vvector<Poly>::iterator pPly = (*ppB)->_polys.begin();
                    vvector<Poly>::iterator be = (*ppB)->_polys.end();
                    for(; pPly != be; ++pPly)
                    {
                        if(_selBoxByTouch)
                        {
                            if(_selBox._box.IsTouchesBox(pPly->_box))
                            {
			                    SelectFace(&(*pPly));
                            }
                        }
                        else
                        {
                            if(_selBox._box.ContainPoint(pPly->_box._min) && 
		                       _selBox._box.ContainPoint(pPly->_box._max))
                            {
			                    SelectFace(&(*pPly));
                            }
                        }
                    }
                }
            }
            break;
        case SEL_VERTEX: //_selmode
            {
                DeselectVertexes();

                PBrushes* pBSs = GetPrimitives();
                FOREACH(PBrushes, (*pBSs), ppB)    
                {
                    if(_curgroup > 0 && (*ppB)->_group != _curgroup)
                        continue;

                    vvector<Poly>::iterator pPly = (*ppB)->_polys.begin();
                    vvector<Poly>::iterator be = (*ppB)->_polys.end();
                    for(; pPly != be; ++pPly)
                    {
                        FOREACH(vvector<Vtx2>, pPly->_vtci, pV)
                        {
                            if(_selBox._box.ContainPoint(pV->_xyz) )
                            {
                                SelectVertex(&(*pV), &(*pPly));
                            }
                        }
                    }
                }
            }
            break;
    }

    Invalidate();
}


void CZ_ed2Doc::OnXoy() // on xoy plane
{
    SaveTempFile();

    if(_pSelBrushes.size())
    {
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            if(_curgroup > 0 && (*ppb)->_group != _curgroup)
                continue;
            vvector<Poly>::iterator pPly = (*ppb)->_polys.begin();
            vvector<Poly>::iterator be = (*ppb)->_polys.end();
            for(; pPly != be; ++pPly)
            {
                FOREACH(vvector<Vtx2>, pPly->_vtci, pv)
                {
                    pv->_xyz.z = -pv->_xyz.z;
                }
            }
        }
        Recut();
    }

    if(_pSelItems.size())
    {
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
            (*ppItem)->_t.z     = -(*ppItem)->_t.z;
        }
    }

    Invalidate();
}

void CZ_ed2Doc::OnXoz() // xoz plane
{
    SaveTempFile();

    if(_pSelBrushes.size())
    {
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            if(_curgroup > 0 && (*ppb)->_group != _curgroup)
                continue;
            vvector<Poly>::iterator pPly = (*ppb)->_polys.begin();
            vvector<Poly>::iterator be = (*ppb)->_polys.end();
            for(; pPly != be; ++pPly)
            {
                FOREACH(vvector<Vtx2>, pPly->_vtci, pv)
                {
                    pv->_xyz.y = -pv->_xyz.y;
                }
            }
        }
        Recut();
    }
    if(_pSelItems.size())
    {
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
            (*ppItem)->_t.y = -(*ppItem)->_t.y;
        }
    }

    Invalidate();
}

void CZ_ed2Doc::OnZoy() // yoz plane
{
    SaveTempFile();

    if(_pSelBrushes.size())
    {
        FOREACH(PBrushes, _pSelBrushes, ppb)
        {
            if(_curgroup > 0 && (*ppb)->_group != _curgroup)
                continue;
            vvector<Poly>::iterator pPly = (*ppb)->_polys.begin();
            vvector<Poly>::iterator be = (*ppb)->_polys.end();
            for(; pPly != be; ++pPly)
            {
                FOREACH(vvector<Vtx2>, pPly->_vtci, pv)
                {
                    pv->_xyz.x = -pv->_xyz.x;
                }
            }
        }
        Recut();
    }

    if(_pSelItems.size())
    {
        FOREACH(vvector<SceItem*>, _pSelItems, ppItem)
        {
            (*ppItem)->_t.x = -(*ppItem)->_t.x;
        }
    }

    Invalidate();
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnMirror() 
{

     int chartyped = DlgMurrChs().DoModal();    

     OnEditCopy();
     OnEditPaste();
     switch(chartyped)
     {
         case 'X':
         case 'x':
             OnZoy();
             break;
         case 'Y':
         case 'y':
             OnXoz(); 
             break;
         case 'Z':
         case 'z':
             OnXoy(); 
             break;
     }
     Invalidate();
}


//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnAlignRightHit()
{
    
	int     nCount;
	Brush** ppb = GetSelBrushes(nCount);	
    V3      seg[2];

    // get all brushes and alighn them with ner floor in whithin 1m range
    for(int i=0;i<nCount;i++,ppb++)
    {
        Box& box = (*ppb)->_box;
        // get minimum position 
        seg[0] =  box._max;
        seg[0] += (-VX * .001);
        seg[1] =  box._max + (VX * 100.0);
    	
        int bs,fs,vs;
	    if(BrushIntersect(seg[0], seg[1], bs, fs,vs))
        {
            REAL depl = Rabs(_ip[0].x - seg[0].x);
            (*ppb)->Move(V3(-depl,0.f,0.f));
        }
    }
	Invalidate();
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnAlighLefthit() 
{
	int     nCount;
	Brush** ppb = GetSelBrushes(nCount);	
    V3      seg[2];

    // get all brushes and alighn them with ner floor in whithin 1m range
    for(int i=0;i<nCount;i++,ppb++)
    {
        Box& box = (*ppb)->_box;
        // get minimum position 
        seg[0] =  box._max;
        seg[0] += (VX * .001);
        seg[1] =  box._max + (VX * 100.0);
    	
        int bs,fs,vs;
	    if(BrushIntersect(seg[0], seg[1], bs, fs,vs))
        {
            REAL depl = Rabs(_ip[0].x - seg[0].x);
            (*ppb)->Move(V3(-depl,0.f,0.f));
        }
    }
	Invalidate();
	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnAlighTophit() 
{
	int     nCount;
	Brush** ppb = GetSelBrushes(nCount);	
    V3      seg[2];

    // get all brushes and alighn them with ner floor in whithin 1m range
    for(int i=0;i<nCount;i++,ppb++)
    {
        Box& box = (*ppb)->_box;
        // get minimum position 
        seg[0] =  box._max;
        seg[0] += (VY * .001);
        seg[1] =  box._max + (VY * 100.0);
    	
        int bs,fs,vs;
	    if(BrushIntersect(seg[0], seg[1], bs, fs,vs))
        {
            REAL depl = Rabs(_ip[0].y - seg[0].y);
            (*ppb)->Move(V3(0.f,-depl,0.f));
        }
    }
	Invalidate();
	
}

//---------------------------------------------------------------------------------------
void CZ_ed2Doc::OnAlighBothit() 
{
	int     nCount;
	Brush** ppb = GetSelBrushes(nCount);	
    V3      seg[2];

    // get all brushes and alighn them with ner floor in whithin 1m range
    for(int i=0;i<nCount;i++,ppb++)
    {
        Box& box = (*ppb)->_box;
        // get minimum position 
        seg[0] =  box._min;
        seg[1] =  seg[0];
        seg[1].y -= 64.0;
    	
        int bs,fs,vs;
	    if(BrushIntersect(seg[0], seg[1], bs, fs, vs))
        {
            REAL depl = Rabs(_ip[0].y - seg[0].y);
            (*ppb)->Move(V3(0.f,-depl,0.f));
        }
    }
	Invalidate();
}
//---------------------------------------------------------------------------------------
// get all cu nCount and width them on the wall thikness
void CZ_ed2Doc::OnMatchwidth() 
{
    SaveTempFile();

	int     nCount;
	Brush** ppb = GetSelBrushes(nCount);	
	Box	maxbox;

	for(int i=0; i<nCount; i++)
	{
		if((*ppb)->IsSolid())
			continue;
		if((*ppb)->_cbox._max.x == INFINIT)
			continue;

		(*ppb)->Recalc();
    
        //_thinkness
    
        V3 depl = (*ppb)->_cbox.GetCenter() - (*ppb)->_box.GetCenter();
        maxbox = (*ppb)->_cbox;     // cutbox. acumulated durring cuts
        maxbox.Expand(.1f);         // expand a bit
        (*ppb)->Move(depl); 
        (*ppb)->Recalc();
        (*ppb)->SetSize(maxbox);

        // move across until reach the width
        int maxAlowed = 4;
        while(!IsZero(depl.len(), EPSMED) && maxAlowed--)
        {
            (*ppb)->Recalc();
            depl = (*ppb)->_cbox.GetCenter() - (*ppb)->_box.GetCenter();
            (*ppb)->Move(depl); 
        }
		ppb++;
	}
	Recut();
	Invalidate();	
}



void CZ_ed2Doc::OnAlbybox() 
{
	V3      v  = _selBox._box.GetExtends();
    REAL    me = _selBox._box.GetMaxExtend();

    if(v.x==me)
    {
        OnZalign() ;
        OnYalign() ;
    }
    else if(v.y==me)
    {
        OnZalign() ;
        OnXalign() ;
    }
    else if(v.z==me)
    {
        OnXalign() ;
        OnYalign() ;
    }
}

void CZ_ed2Doc::OnUpdateAlbybox(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(IsSelBoxOn());
	
}
