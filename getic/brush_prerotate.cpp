//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "MainFrm.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "brush.h"
#include "basecont.h"
#include "compiler.h"
#include "Motion.h"
#include "z_ed2Doc.h"
#include "BigTerrain.h"
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
[B----B-----C----B]      vvector of brushes
| \   | \   |
BC R  BC R  BC           R - resultinh brush by imediate aplyed cut
BC - list ao associated cuts or brushes
*/

int Brush::GID=0;

//--|Brush::Brush|------------------------------------------------------------------------
Brush::Brush()
{
    _s        = V1;
    _selected = FALSE;
    _copyed   = FALSE;
    _polys.reserve(16);
    _pCutBrshes.reserve(16);
    _pSldBrushes.reserve(16);
    _flags    = 0;
    _pMotion  = 0;
    _pResult  = this;
    _group    = 0;
    _flags    = BRSH_DIRTY|BRSH_NEW;
    _props    = MODEL_MAT_BRICK;
    _polySec  = 0;
    _thikness = 0;
    _nTrigers = 0;
    _intask   = 0;
    _unicID   = GID++;
    _pUsrData = 0;
    _stprintf(_name, "%d",_unicID);
};

//--|Brush::~Brush|-----------------------------------------------------------------------
Brush::~Brush()
{
    if(_intask==1)
    {
        _intask = 0;
        while(_intask > 0)
        {
            Sleep(64);
        }
    }
    _polys.clear();
    DeleteResultPtr();
}

//--| Brush& b|------------------------------------------------------------------------
Brush::Brush(const Brush& b)
{
    _intask = 0;
    Equal(b);
    if(b._pResult != &b)    // if b has a result
    {
        _pResult  = new Brush(*b._pResult);
        strcpy(_pResult->_name, b._name);
    }
    else
    {
        _pResult  = this;
    }
}

//--|  Brush::operator=|------------------------------------------------------------------
Brush&  Brush::operator=(const Brush& b)
{
    if(this == &b)
        return *this;
    Equal(b);
    DeleteResultPtr();
    if(b._pResult != &b)    // if b has a result
    {
        _pResult  = new Brush(*b._pResult);
        strcpy(_pResult->_name, b._name);
    }
    return *this;
}

//--|  Brush::Equal|----------------------------------------------------------------------
void  Brush::Equal(const Brush& b, BOOL polys)
{
    if(GNewUID)
    {
        _unicID    = GID++;
        TCHAR name[64];
        _tcscpy(name, b._name);
        StripDigits(name);
        if(strlen(name))
            _stprintf(_name, "%s%d", name, _unicID);
        else
            _stprintf(_name, "%d", _unicID);

    }
    else
    {
        _unicID    = b._unicID;
        _tcscpy(_name, b._name);
    }
    _t         = b._t;
    _r         = b._r;
    _s         = b._s;
    _mt        = b._mt;
    _selected  = b._selected;
    _pUsrData  = b._pUsrData;
    _copyed    = b._copyed;
    _physicsA  = b._physicsA;
    _rotSteps  = b._rotSteps;
    _physicsD  = b._physicsD;
    _box       = b._box;
    _flags     = b._flags;
    _props     = b._props;
    _group     = b._group;
    _polySec   = b._polySec;
    _thikness  = b._thikness;
    _cf        = b._cf;
    _pMotion   = b._pMotion; // just point same as previous
    _nTrigers  = 0;    // don't copyes triggers
    // paste take care of this

    
    _polys.clear();
    if(!polys)
        return;
    Polys::iterator p  = ((Brush&)b)._polys.begin();
    Polys::iterator e  = ((Brush&)b)._polys.end();
    for(;p!=e;p++)
    {
        Poly pn = *p;
        pn._pBrush = this;
        _polys.push_back(pn);
    }
}
void Brush::MakeTm()
{
}

//--| Brush::GetTxPolysCopy|--------------------------------------------------------------
void Brush::GetTxPolysCopy(Polys& pl)
{
    Poly pnew;
    pl.clear();
    
    FOREACH(Polys, _polys, p)
    {
        pnew.Clear();
        pnew.CopyProps(*p);
        
        FOREACH(vvector<Vtx2>,p->_vtci,vtci)
        {
            Vtx2 v = *vtci;
            pnew._vtci.push_back(v);
        }
        pnew.Recalc();
        pl.push_back(pnew);
    }
}

//--| Brush::GetTxReversedPolysCopy|------------------------------------------------------
void Brush::GetTxReversedPolysCopy(Polys& pl)
{
    pl.clear();
    pl.reserve(_polys.size());
    
    int count  = _polys.size();
    
    FOREACH(Polys,_polys,p)
    {
        Poly pnew;
        
        pnew.CopyProps(*p);
        
        vvector<Vtx2>::reverse_iterator b = p->_vtci.rbegin();
        vvector<Vtx2>::reverse_iterator e = p->_vtci.rend();
        for(;b!=e;b++)
        {
            Vtx2 v = *b;
            pnew._vtci.push_back(v);
        }
        pnew.Recalc();
        //  pnew._c=-pnew._c;
        
        pl.push_back(pnew);
    }
}

//--|  Brush::operator<<|-----------------------------------------------------------------
Brush&  Brush::operator<<(vvector<Poly>& pls)
{
    Poly pn;
    Polys::iterator p  = pls.begin();
    Polys::iterator e  = pls.end();
    for(;p!=e;p++)
    {
        pn = *p;
        pn._pBrush = this;
        _polys.push_back(pn);
    }
    Dirty(1);
    return *this;
}

//--|  Brush::operator<<|-----------------------------------------------------------------
Brush&  Brush::operator<<(const Brush& b)
{
    Poly pn;
    Polys::iterator p  = ((Brush&)b)._polys.begin();
    Polys::iterator e  = ((Brush&)b)._polys.end();
    for(;p!=e;p++)
    {
        pn = *p;
        pn._pBrush = this;
        _polys.push_back(pn);
    }
    Dirty(1);
    return *this;
}

//--|   Brush::operator<<|----------------------------------------------------------------
Brush&   Brush::operator<<(Poly& p)
{
    _box.Union(p._box);
    _polys.push_back(p);
    p._pBrush = this;
    return *this;
}


//--| Brush::GetTrMatrix|-----------------------------------------------------------------
M4& Brush::GetTrMatrix()
{
    return _mt;
}

//--|    Brush::GlTransform|--------------------------------------------------------------
void    Brush::GlTransform()
{
    glTranslatef(_t.x,_t.y,_t.z);
    glRotatef(_r.z,0,0,1);
    glRotatef(_r.y,0,1,0);
    glRotatef(_r.x,1,0,0);
    glScalef(_s.x,_s.y,_s.z);
}

//--|    Brush::Clear|--------------------------------------------------------------------
void    Brush::Clear()
{
    _polys.clear();
    _box.Reset();
    _t        = V0;
    _s        = V1;
    _r        = V0;
    _selected = FALSE;
    _copyed   = FALSE;
    
    _pCutBrshes.clear();
    _pSldBrushes.clear();
    DeleteResultPtr();
    _mt.identity();
}

//--| Brush::GetPrimitive|----------------------------------------------------------------
Brush& Brush::GetPrimitive()
{
    #ifdef _NO_PERM
        return GetWorldPos();
    #else//
    return *this;
    #endif //
}

//--| Brush::GetResult|-------------------------------------------------------------------
Brush& Brush::GetResult()
{
    #ifdef _NO_PERM
        return _pResult->GetWorldPos();
    #else//
        return *_pResult;
    #endif //
}

//--|    Brush::Recalc|-------------------------------------------------------------------
void    Brush::Recalc(BOOL withnormals)
{
#pragma message("optimize here")
    if(_flags & BRSH_BIGTERRAIN || IsSelection())
    {
        RecalcBoxAndNormals(0);
        if(_flags & BRSH_BIGTERRAIN)
        {
            CBigTerrain* pBt = reinterpret_cast<CBigTerrain*>(this->_pUsrData);
            if(pBt)
            {
                pBt->b_box = _box;
            }
        }
        return;
    }

DELAG:
	FOREACH(Polys, _polys, p)
	{
		if(p->_flags & POLY_DELETED || p->_vtci.size() < 3)
		{
			_polys.erase(p);
			goto DELAG;
		}
	}
    RecalcBoxAndNormals(withnormals);
   	if(_cf._tm == TM_ENV)
	{
		CalcEnvTc();
	}
    _box.Expand0(8);
    _flags &= ~BRSH_DIRTY;   
}

//--|    Brush::FlagFaces|----------------------------------------------------------------
void    Brush::FlagFaces(DWORD f)
{
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        (*b)._flags|=f;
    }
}

//--|  Brush::Mirror|---------------------------------------------------------------------
void  Brush::Mirror()
{
    V3 center = _box.GetCenter();
    V3 dif;
    
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        Poly& ps = *b;
        FOREACH(vvector<Vtx2>, ps._vtci, vt)
        {
            dif = vt->_xyz - center;
            vt->_xyz = center + dif;
        }
    }
    Recalc();
}

//--|     Brush::Reverse|-----------------------------------------------------------------
void     Brush::Reverse()
{
    _box.Reset();
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    
    for(;b!=e;b++)
    {
        Poly& ps = *b;
        ps.Reverse();
        _box.Union(ps._box);
    }
    
    
    Dirty(1);
}


//--|     Brush::AddProjection|---------------------------------------------------------------
void    Brush::AddProjection(Brush* pCutter)
{
    if(_pProjBrushes.push_unique(pCutter))
    {
        pCutter->Dirty(1);
    }
}

//--|     Brush::RemoveProjection|---------------------------------------------------------------
void    Brush::RemoveProjection(Brush* pCutter)
{
    _pProjBrushes.erase_if(pCutter);
}


//--|     Brush::AddCutter|---------------------------------------------------------------
void     Brush::AddCutter(Brush* pCutter)
{
    if(_pCutBrshes.push_unique(pCutter))
    {
        Dirty(1);
    }
}

//--|     Brush::RemoveCutter|------------------------------------------------------------
void     Brush::RemoveCutter(Brush* pCutter)
{
    if(_pCutBrshes.erase_if(pCutter))
    {
        Dirty(1);
    }
}

void     Brush::AppendPrim(Brush& brush)
{
    FOREACH(Polys, brush._polys, pp)
    {
        pp->_pBrush=this;
        _polys<<*pp;
    }
    Dirty(1);
}

//--|     Brush::AddBrush|----------------------------------------------------------------
void     Brush::AddBrush(Brush* pB)
{
    _pSldBrushes.push_unique(pB);
}

//--|     Brush::RemoveBrush|-------------------------------------------------------------
void     Brush::RemoveBrush(Brush* pB)
{
    _pSldBrushes.erase_if(pB);
}


//--| Brush::RecalcTM|--------------------------------------------------------------------
void Brush::RecalcTM()
{
    _mt  =  MTranslate(_t.x,_t.y,_t.z);
    _mt  *= MRadRotate(_r.x,_r.y,_r.z);
    _mt  *= MScale(_s.x,_s.y,_s.z);
}

//--| Brush::Move|------------------------------------------------------------------------
void Brush::Move(V3& v)
{
    _t += v;
#ifdef _NO_PERM
        RecalcTM();
#else
    M4 ml = MTranslate(v.x,v.y,v.z);
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        Poly& ps = *b;
        ps.Transform(ml);
    }
#endif //

    Dirty(1);
}

//--|    Brush::LimitScaleVector|---------------------------------------------------------
BOOL    Brush::LimitScaleVector(V3& v, REAL zoom)
{
    Box nBox(_box);
    if(_thikness==0)
        _thikness = 16;
    REAL mindim = (_thikness * 3);

    V3   ex1 =  nBox.GetExtends();

    nBox._max.x += v.x;
    nBox._max.y += v.y;
    nBox._max.z += v.z;

    V3   ex2 =  nBox.GetExtends();

    if(ex2.x < 8 || ex2.y < 8 || ex2.z < 8)
    {
        v = 0.0;
        return -1;
    }
    
    if(ex2.x < ex1.x || ex2.y < ex1.y || ex2.z < ex1.z)
    {
        if(ex2.x < mindim || ex2.y < mindim ||ex2.z < mindim)
        {
            v = 0.0;
            return 1;
        }
    }
    return 0;
}

//--|    Brush::RescalePolys|-------------------------------------------------------------
void    Brush::RescalePolys(int s, int e, V3& vv3s, V3& tv, Box& nBox, Box& ob)
{
    nBox._max.x += vv3s.x;
    nBox._max.y += vv3s.y;
    nBox._max.z += vv3s.z;
    

    V3 den = (ob._max-ob._min); 
    if(den.x==0)den.x=1;
    if(den.y==0)den.y=1;
    if(den.z==0)den.z=1;        // fix .#IND
    V3 rap = (nBox._max-nBox._min)/den;
    
    if(s==0){
        _s *= rap;
        _t += tv;
    }

    for(int i=s; i< e;i++)
    {
        Vertexes::iterator vb  = _polys[i]._vtci.begin();
        Vertexes::iterator ve  = _polys[i]._vtci.end();
        for(;vb!=ve;vb++)
        {
            //recalc each point relative to new box
            V3& vx = vb->_xyz;
            vx     = ob._min + (vx-ob._min) * rap;
            vx     += tv;
        }
    }
}

// apllyes the scale in place
void Brush::ScaleTr(V3& v, V3& t)
{
    for(UINT i=0; i< _polys.size(); i++)
    {
        Vertexes::iterator vb  = _polys[i]._vtci.begin();
        Vertexes::iterator ve  = _polys[i]._vtci.end();
        for(;vb!=ve;vb++)
        {
            //recalc each point relative to new box
            V3& vx = vb->_xyz;
            //vx     += t;
            vx     *= v;
            
        }
    }
}

//--| Brush::Scale|-----------------------------------------------------------------------
// calcs the vscale on the box reports
void Brush::Scale(V3& s, V3& t, REAL zoom)
{
    if(_polySec)
    {
        Box nBox;
        Box nBox1;
        
        for(int i=0; i< _polySec;i++)
        {
            nBox.Union(_polys[i]._box);
        }
        nBox.Expand0(16);
        
        // if(nBox.GetExtends().x<GEpsilon || nBox.GetExtends().y<GEpsilon || nBox.GetExtends().z<GEpsilon)
        //  return;
        
        nBox1 = nBox;
        RescalePolys(0, _polySec, s, t, nBox, nBox1);
       
        nBox.Reset();
        for( i=_polySec; i< _polys.size();i++)
        {
            nBox.Union(_polys[i]._box);
        }
        nBox1 = nBox;
        RescalePolys(_polySec, _polys.size(), s, t ,nBox, nBox1);
        
    }
    else
    {
        Box nBox(_box);
        Box nBox1(_box);
        
        //  if(nBox.GetExtends().x<GEpsilon || nBox.GetExtends().y<GEpsilon || nBox.GetExtends().z<GEpsilon)
        //   return;
        
        RescalePolys(0, _polys.size(), s, t ,nBox, nBox1);
    }
    Dirty(1);
}

//--| Brush::Raise|-----------------------------------------------------------------------
void Brush::Raise(V3& v, REAL zoom)
{
    _f += v;
#ifdef _NO_PERM
        
#else
        Shift(v);
#endif //
    Dirty(1);
}

//--| Brush::Shift|-----------------------------------------------------------------------
void Brush::Shift(V3& v)
{
    // v is the translation far point
    V3 ex = _box.GetExtends();
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        Poly& ps = *b;
        
        Vertexes::iterator vb  = ps._vtci.begin();
        Vertexes::iterator ve  = ps._vtci.end();
        for(;vb!=ve;vb++)
        {
            //recalc each point relative to new box
            
            V3& vx   = vb->_xyz;
            V3 rap  = (vx-_box._min) / ex;
            
            vx.y     +=  v.x * rap.x;
            vx.z     +=  v.y * rap.y;
            vx.x     +=  v.z * rap.z;
        }
    }
    
}

//--| Brush::Rotate|----------------------------------------------------------------------
void Brush::Rotate(V3& v, BOOL aroundOrigin/*= 0*/)
{
    M4 m;
    _r += v;
    V3   prev = _r;

    ROLL2PI(_r.y);
    ROLL2PI(_r.x);
    ROLL2PI(_r.z);

    if(v.y) m  *= MRotAround(VY, v.y);
    if(v.x) m *= MRotAround(VX, v.x);
    if(v.z) m  *= MRotAround(VZ, v.z);

    V3 at = _box.GetCenter();

    if(aroundOrigin)
    {
        V3 rp = at;
        m.v3rotate(rp);
        Move(rp-at);
    }
    else
    {
    
        Polys::iterator b  = _polys.begin();
        Polys::iterator e  = _polys.end();
        for(;b!=e;b++)
        {
            Poly& ps = *b;
    
            Vertexes::iterator vb  = ps._vtci.begin();
            Vertexes::iterator ve  = ps._vtci.end();
            for(;vb!=ve;vb++)
            {
                vb->_xyz-=at;   //translate back in 0
                m.v3rotate(vb->_xyz); //roate
                vb->_xyz+=at;   //translate back on position
            }
        }
    }
    Dirty(1);
}

//--|    Brush::ApplyCuts|----------------------------------------------------------------
BOOL    Brush::ApplyCuts()
{
    if(IsSelection()) return 0;

    Dirty(0);
 
    // if(_flags & BRSH_CSGTERRAIN)
    //  return 1;
    if(_pCutBrshes.size())
    {
        MiniBsp cutsBsp;
        MiniBsp brshBsp;
        Brush   cpXToCut;
        Brush   interBrsh;
        Brush   allCuts;        //all cuts
        Box     cutBox;
        
        if(_polys.size() > 4096)
        {
            AfxMessageBox(MKSTR("Cannot Cut. There are '%d' (too many) polygons to cut. ",_polys.size()),MB_OK);
            return FALSE;
        }
        
        if(_polys.size() > 2048)
        {
            if(IDNO == AfxMessageBox(MKSTR("There are '%d polygons to Cut. \r\n"
                                            " This can take up 2-3 minuttes. Would you like to cut ?\r\n\r\n\r\n"
                                            "If you select No the brush is flagged 'Disabled' \r\n"
                                            "A Disabled Brush would not be cut while touching any cut brushes \r\n"
                                            "You can Enable the brush from Brush Properties Dialog Box"
                                            
                                            ,_polys.size()),MB_OK|MB_YESNO))
            {
                _flags |= BRSH_DISABLED;
                return FALSE;
            }
        }

        //
        // make pResult
        //
        cpXToCut.Equal(*this,FALSE);  // make a this copy
        
        interBrsh._flags = _flags;
        interBrsh._group = _group;
        interBrsh._props = _props;
        
        //
        // gather together all cuts of this brush
        //
        Compiler::Union(_pCutBrshes, allCuts);
        
        //
        // calc all cut brushes bbox
        //
        vvector<Brush*>::iterator ppCut  =  _pCutBrshes.begin();
        vvector<Brush*>::iterator ppCutE =  _pCutBrshes.end();
        for(;ppCut!=ppCutE;ppCut++)
        {
            cutBox.Union((*ppCut)->_box);
        }
        cutBox.Expand(32.f);
        
        if(allCuts._polys.size() > 4096)
        {
            AfxMessageBox(MKSTR("Cannot Cut. There are '%d' (too many) faces TO CUT. ",allCuts._polys.size()),
                         MB_OK);
            return FALSE;
        }
        //
        // compile all the cut brushes
        //
        if(!cutsBsp.Compile(allCuts._polys,0))
        {
            return FALSE;
        }
        //
        // compile from only touched polys
        //
        if(GCarving)
        {
            
            FOREACH(vvector<Poly>, _polys, pp)
            {
                pp->_flags|=POLY_DIRTY;
                cpXToCut._polys<<*pp;
            }
        }
        else
        {
            FOREACH(vvector<Poly>, _polys, pp)
            {
                if(pp->_box.IsTouchesBox(cutBox))
                {
                    pp->_flags|=POLY_DIRTY;
                    cpXToCut._polys<<*pp;
                }else
                {
                    pp->_flags&=~POLY_DIRTY;
                }
            }
            if(cpXToCut._polys.size() > 4096)
            {
                AfxMessageBox(MKSTR("Cannot Cut. There are '%d' (too many) faces to cut. ",allCuts._polys.size()),
                             MB_OK);
                return FALSE;
            }
        }
        
        
        if(cpXToCut._polys.size())
        {
            if(!brshBsp.Compile(cpXToCut._polys,0))
            {
                return FALSE;
            }
            
            // cut out this->polys ending in the solid cut brush
            cutsBsp.ClipBrPolys(cpXToCut._polys, interBrsh._polys,1,1,1);
            
            // clip inverted final brush with this bsp
            brshBsp._addedPolysBox.Reset(); // collect cut out polys to reshape the cut
            // to this dimension only
            allCuts.Reverse();
            brshBsp.ClipBrPolys(allCuts._polys, interBrsh._polys,1,0,0);
            if(interBrsh._polys.size())
            {
                // store the decupated area bbox of the brush. reajust this brush
                // when Ctrl+M(atch) is pressed
                FOREACH(PBrushes, _pCutBrshes, ppCB)
                {
                    (*ppCB)->_cbox = brshBsp._addedPolysBox; //cut bounding box
                    
                    if((*ppCB)->_cbox._max.x != INFINIT)
                      (*ppCB)->_thikness = cpXToCut._thikness;
                    else
                        (*ppCB)->_thikness = 0;
                    
                }
                
                AlocateResultPtr();
                _pResult->Clear();
                _pResult->Equal(interBrsh);

                FOREACH(vvector<Poly>, _polys, pp)
                {
                    if(!(pp->_flags&POLY_DIRTY))
                    {
                        _pResult->_polys << *pp;
                    }
                }
            }
        }
        else
        {
            DeleteResultPtr();
        }
    }
	
    return TRUE;
}

//--|    Brush::UpdateLinks|--------------------------------------------------------------
void    Brush::UpdateSolidsTouches()
{
AGAIN2:
    vvector<Brush*>::iterator ppCut  =  _pSldBrushes.begin();
    vvector<Brush*>::iterator ppCutE =  _pSldBrushes.end();
    for(; ppCut!=ppCutE; ppCut++)
    {
        if(!_box.IsTouchesBox((*ppCut)->_box))
        {
            (*ppCut)->RemoveBrush(this);
            RemoveBrush((*ppCut));
            goto AGAIN2;
        }
    }
}

//--|    Brush::UpdateLinks|--------------------------------------------------------------
void    Brush::UpdateCutsTouches()
{
AGAIN:
    vvector<Brush*>::iterator ppCut  =  _pCutBrshes.begin();
    vvector<Brush*>::iterator ppCutE =  _pCutBrshes.end();
    for(; ppCut!=ppCutE; ppCut++)
    {
        if(!_box.IsTouchesBox((*ppCut)->_box))
        {
            (*ppCut)->RemoveBrush(this);
            RemoveCutter((*ppCut));
            goto AGAIN;
        }
    }
    if(_pCutBrshes.size()==0)
    {
        if(_pResult!=this)
                delete _pResult;
        _pResult = this;
    }

    // projection btush ewmoves it's solids
}

void	Brush::DeletePolys()
{
BEG:
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
	{
		if(b->_flags & POLY_DELETED)
		{
			_polys.erase(b);
			goto BEG;
		}
	}
}

//--| Brush::Deketeface|------------------------------------------------------------------
void Brush::Deleteface(Poly* p)
{
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
	{
		if(b == p)
		{
			p->_flags|=POLY_DELETED;
			break;
		}
	}
    Dirty(1);
}

//--| Brush::DeleteResultPtr|-------------------------------------------------------------
void Brush::DeleteResultPtr()
{
    if(_pResult != this)
        delete _pResult;
    _pResult = this;
}

//--| Brush::AlocateResultPtr|------------------------------------------------------------
void Brush::AlocateResultPtr()
{
    if(_pResult != this)
        delete _pResult;
    _pResult = new Brush();
    ::_tcscpy(_pResult->_name, _name);
}

//--|    Brush::Replan|-------------------------------------------------------------------
void    Brush::Replan()
{
    
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        Poly& ps = *b;
        Plane pl;
        
        Vertexes::iterator vb  = ps._vtci.begin();
        Vertexes::iterator ve  = ps._vtci.end();
        
        V3&  v1 = vb->_xyz;
        vb++;
        V3&  v2 = vb->_xyz;
        vb++;
        
        for(;vb!=ve;vb++)
        {
            V3& v3 = vb->_xyz;
            
            GCalcNormal(&pl, v1, v2, v3);
            
            if(!IsZero(pl._n - ps._n,EPSMED))
            //if(!IsZero (Vdp (pl._n , ps._n)-1.0f))
            {
                V3 s = v3+ps._n;
                ps.RayIntersect(s,-ps._n,v3); //replan the point
            }
        }
    }
}

/*
0   1
3   2
*/

//--|      Brush::MakeCube|---------------------------------------------------------------
void      Brush::MakeCube(V3& dims, TEX_MAP tm)
{
    _cf._tm = tm;
    
    Poly p;
    
    V3  v[8];
    V3  fv[4];
    
    v[0]  =V3(-(dims.x/2), (dims.y/2), -(dims.z/2));
    v[1]  =V3(-(dims.x/2), (dims.y/2), (dims.z/2));
    v[2]  =V3((dims.x/2), (dims.y/2), (dims.z/2));
    v[3]  =V3((dims.x/2), (dims.y/2), -(dims.z/2));
    v[4]  =V3(-(dims.x/2), -(dims.y/2), -(dims.z/2));
    v[5]  =V3((dims.x/2), -(dims.y/2), -(dims.z/2));
    v[6]  =V3((dims.x/2), -(dims.y/2), (dims.z/2));
    v[7]  =V3(-(dims.x/2), -(dims.y/2), (dims.z/2));
    
    //TOP
    
    fv[0] = v[0];
    fv[1] = v[1];
    fv[2] = v[2];
    fv[3] = v[3];
    p.Create(4, fv, this);
    
    if(tm == TM_SPH)
    {
        fv[0].norm();
        fv[1].norm();
        fv[2].norm();
        fv[3].norm();
        
        p._vtci[0]._uv[TX_0] = UV(((fv[0].x/PI)+.50), ((-fv[0].z/PI)+.50));
        p._vtci[1]._uv[TX_0] = UV(((fv[1].x/PI)+.50), ((-fv[1].z/PI)+.50));
        p._vtci[2]._uv[TX_0] = UV(((fv[2].x/PI)+.50), ((-fv[2].z/PI)+.50));
        p._vtci[3]._uv[TX_0] = UV(((fv[3].x/PI)+.50), ((-fv[3].z/PI)+.50));
    }
    else
    {
        p.CalcTexCoord(1,0);
        p.FlipTC('H');
    }
    *(this) << p;
    
    //BOTTOM
    
    fv[0] = v[4];
    fv[1] = v[5];
    fv[2] = v[6];
    fv[3] = v[7];
    p.Create(4, fv, this);
    if(tm == TM_SPH)
    {
        fv[0].norm();
        fv[1].norm();
        fv[2].norm();
        fv[3].norm();
        
        p._vtci[0]._uv[TX_0] = UV(((fv[0].x/PI)+.50), ((-fv[0].z/PI)+.50));
        p._vtci[1]._uv[TX_0] = UV(((fv[1].x/PI)+.50), ((-fv[1].z/PI)+.50));
        p._vtci[2]._uv[TX_0] = UV(((fv[2].x/PI)+.50), ((-fv[2].z/PI)+.50));
        p._vtci[3]._uv[TX_0] = UV(((fv[3].x/PI)+.50), ((-fv[3].z/PI)+.50));
    }
    else
    {
        p.CalcTexCoord(1,0);
        p.FlipTC('H');
    }
    *(this) << p;
    
    //FRONT
    fv[0] = v[1];
    fv[1] = v[7];
    fv[2] = v[6];
    fv[3] = v[2];
    p.Create(4, fv, this);
    if(tm == TM_SPH)
    {
        fv[0].norm();
        fv[1].norm();
        fv[2].norm();
        fv[3].norm();
        
        p._vtci[0]._uv[TX_0] = UV(((fv[0].x/PI)+.50), ((-fv[0].z/PI)+.50));
        p._vtci[1]._uv[TX_0] = UV(((fv[1].x/PI)+.50), ((-fv[1].z/PI)+.50));
        p._vtci[2]._uv[TX_0] = UV(((fv[2].x/PI)+.50), ((-fv[2].z/PI)+.50));
        p._vtci[3]._uv[TX_0] = UV(((fv[3].x/PI)+.50), ((-fv[3].z/PI)+.50));
    }
    else
        p.CalcTexCoord(1,0);
    *(this) << p;
    
    //BACK
    fv[0] = v[0];
    fv[1] = v[3];
    fv[2] = v[5];
    fv[3] = v[4];
    p.Create(4, fv,this);
    if(tm == TM_SPH)
    {
        fv[0].norm();
        fv[1].norm();
        fv[2].norm();
        fv[3].norm();
        
        p._vtci[0]._uv[TX_0] = UV(((fv[0].x/PI)+.50), ((-fv[0].z/PI)+.50));
        p._vtci[1]._uv[TX_0] = UV(((fv[1].x/PI)+.50), ((-fv[1].z/PI)+.50));
        p._vtci[2]._uv[TX_0] = UV(((fv[2].x/PI)+.50), ((-fv[2].z/PI)+.50));
        p._vtci[3]._uv[TX_0] = UV(((fv[3].x/PI)+.50), ((-fv[3].z/PI)+.50));
    }
    else
    {
        p.CalcTexCoord(1,0);
        p.FlipTC('H');
    }
    *(this) << p;
    
    //LEFT
    fv[0] = v[0];
    fv[1] = v[4];
    fv[2] = v[7];
    fv[3] = v[1];
    p.Create(4, fv, this);
    if(tm == TM_SPH)
    {
        fv[0].norm();
        fv[1].norm();
        fv[2].norm();
        fv[3].norm();
        
        p._vtci[0]._uv[TX_0] = UV(((fv[0].x/PI)+.50), ((-fv[0].z/PI)+.50));
        p._vtci[1]._uv[TX_0] = UV(((fv[1].x/PI)+.50), ((-fv[1].z/PI)+.50));
        p._vtci[2]._uv[TX_0] = UV(((fv[2].x/PI)+.50), ((-fv[2].z/PI)+.50));
        p._vtci[3]._uv[TX_0] = UV(((fv[3].x/PI)+.50), ((-fv[3].z/PI)+.50));
    }
    else
        p.CalcTexCoord(1,0);
    *(this) << p;
    

    //RIGHT

    fv[0] = v[3];
    fv[1] = v[2];
    fv[2] = v[6];
    fv[3] = v[5];
    p.Create(4, fv, this);
    if(tm == TM_SPH)
    {
        fv[0].norm();
        fv[1].norm();
        fv[2].norm();
        fv[3].norm();
        
        p._vtci[0]._uv[TX_0] = UV(((fv[0].x/PI)+.50), ((-fv[0].z/PI)+.50));
        p._vtci[1]._uv[TX_0] = UV(((fv[1].x/PI)+.50), ((-fv[1].z/PI)+.50));
        p._vtci[2]._uv[TX_0] = UV(((fv[2].x/PI)+.50), ((-fv[2].z/PI)+.50));
        p._vtci[3]._uv[TX_0] = UV(((fv[3].x/PI)+.50), ((-fv[3].z/PI)+.50));
    }
    else
    {
        p.CalcTexCoord(1,0);
        p.FlipTC('H');

    }
    *(this) << p;
    

	Recalc();
    Dirty(1);
    
}

//--| Brush::MakeCylinder|----------------------------------------------------------------
void Brush::MakeCylinder(REAL rad, REAL height, int radstrips, int yDimension, TEX_MAP tm)
{
    Brush     upper;
    Brush     lower;
    Brush     side;
    // create a cylinder standing on ZOX.
    double   arcstep = DOIPI/radstrips;
    Vtx2    upPoly[36];
    Vtx2    dnPoly[36];
    Vtx2    vSide[4];
    double   arcPrev=0.0;
    Poly     pu,pd,ps;
    double   uplimit = DOIPI + arcstep/2.0;
    double   tuside = 0;
    double   tustep = 1.0/radstrips;
    V3   prevUpDn[2];
    
    _cf._tm = tm;
    // create top cover. bottom and side in one shot
    int nSec = 0;
    for(double arc = arcstep; arc < uplimit; )
    {
        
        //up and down covers
        upPoly[nSec]._xyz.y   = height/2.0;
        upPoly[nSec]._xyz.x   = sin(arcPrev)*rad;
        upPoly[nSec]._xyz.z   = cos(arcPrev)*rad;
        
        upPoly[nSec]._uv[TX_0].u = .5+sin(arcPrev)/2.0;
        upPoly[nSec]._uv[TX_0].v = .5+cos(arcPrev)/2.0;
        
        
        dnPoly[nSec]._xyz   = upPoly[nSec]._xyz;
        dnPoly[nSec]._xyz.y = -height/2;
        dnPoly[nSec]._uv[TX_0] = upPoly[nSec]._uv[TX_0];
        
        
        if(nSec > 0)
        {
            vSide[0]._xyz = upPoly[nSec]._xyz;
            vSide[1]._xyz = upPoly[nSec-1]._xyz;
            vSide[2]._xyz = dnPoly[nSec-1]._xyz;
            vSide[3]._xyz = dnPoly[nSec]._xyz;
            
            switch(tm)
            {
                default:
                    case TM_LIN:
                    vSide[0]._uv[TX_0] = UV(1,0);
                    vSide[1]._uv[TX_0] = UV(0,0);
                    vSide[2]._uv[TX_0] = UV(0,1);
                    vSide[3]._uv[TX_0] = UV(1,1);
                break;
                case TM_CYL:
                    vSide[0]._uv[TX_0] = UV(tuside   ,0);
                    vSide[1]._uv[TX_0] = UV(tuside-tustep,    0);
                    vSide[2]._uv[TX_0] = UV(tuside-tustep,    1);
                    vSide[3]._uv[TX_0] = UV(tuside    ,1);
                break;
                case TM_SPH:
                {
                    V3 vn[4];
                    
                    vn[0] = vSide[0]._xyz;
                    vn[1] = vSide[1]._xyz;
                    vn[2] = vSide[2]._xyz;
                    vn[3] = vSide[3]._xyz;
                    
                    vn[0].norm();
                    vn[1].norm();
                    vn[2].norm();
                    vn[3].norm();
                    
                    vSide[0]._uv[TX_0] = UV(((vn[0].x/PI)+.50), ((-vn[0].z/PI)+.50));
                    vSide[1]._uv[TX_0] = UV(((vn[1].x/PI)+.50), ((-vn[1].z/PI)+.50));
                    vSide[2]._uv[TX_0] = UV(((vn[2].x/PI)+.50), ((-vn[2].z/PI)+.50));
                    vSide[3]._uv[TX_0] = UV(((vn[3].x/PI)+.50), ((-vn[3].z/PI)+.50));
                }
                break;
                
                
            }
            
            vSide[0]._uv[TX_LM] = UV(1,0);
            vSide[1]._uv[TX_LM] = UV(0,0);
            vSide[2]._uv[TX_LM] = UV(0,1);
            vSide[3]._uv[TX_LM] = UV(1,1);
            
            
            ps.Create(4,vSide,this);
            side   << ps;
        }
        
        arcPrev =  arc;
        arc     += arcstep;
        tuside  += tustep;
        nSec++;
    }
    
    vSide[0]._xyz = upPoly[0]._xyz;
    vSide[1]._xyz = upPoly[nSec-1]._xyz;
    vSide[2]._xyz = dnPoly[nSec-1]._xyz;
    vSide[3]._xyz = dnPoly[0]._xyz;
    
    switch(tm)
    {
        default:
            case TM_LIN:
            vSide[0]._uv[TX_0] = UV(1,0);
        vSide[1]._uv[TX_0] = UV(0,0);
        vSide[2]._uv[TX_0] = UV(0,1);
        vSide[3]._uv[TX_0] = UV(1,1);
        break;
        case TM_CYL:
            vSide[0]._uv[TX_0] = UV(1,0);
        vSide[1]._uv[TX_0] = UV(tuside-tustep     ,0);
        vSide[2]._uv[TX_0] = UV(tuside-tustep     ,1);
        vSide[3]._uv[TX_0] = UV(1,1);
        break;
        case TM_SPH:
        {
            V3 vn[4];
            
            vn[0] = vSide[0]._xyz;
            vn[1] = vSide[1]._xyz;
            vn[2] = vSide[2]._xyz;
            vn[3] = vSide[3]._xyz;
            
            vn[0].norm();
            vn[1].norm();
            vn[2].norm();
            vn[3].norm();
            
            vSide[0]._uv[TX_0] = UV(((vn[0].x/PI)+.50), ((-vn[0].z/PI)+.50));
            vSide[1]._uv[TX_0] = UV(((vn[1].x/PI)+.50), ((-vn[1].z/PI)+.50));
            vSide[2]._uv[TX_0] = UV(((vn[2].x/PI)+.50), ((-vn[2].z/PI)+.50));
            vSide[3]._uv[TX_0] = UV(((vn[3].x/PI)+.50), ((-vn[3].z/PI)+.50));
        }
        break;
        
    }
    vSide[0]._uv[TX_LM] = UV(1,0);
    vSide[1]._uv[TX_LM] = UV(0,0);
    vSide[2]._uv[TX_LM] = UV(0,1);
    vSide[3]._uv[TX_LM] = UV(1,1);
    
    
    ps.Create(4,vSide,this);
    side   << ps;
    
    pu.Create(nSec,upPoly,this);
    pd.Create(nSec,dnPoly,this);
    upper<<pu;
    lower<<pd;
    
    lower.Reverse();


    if(!CopyDiametral(side))
    {
        (*this) << side;
    }
    (*this) << upper;
    (*this) << lower;
    
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        if(tm == TM_CYL)
          (*b).FlipTC('V');
        else if(tm == TM_LIN)
               (*b).CalcTexCoord(1,0);
    }

    Recalc();
    Dirty(1);
}

//--|    Brush::MakeCone|-----------------------------------------------------------------
void    Brush::MakeCone(REAL rad, REAL height, int radstrips, int yDimension, TEX_MAP tm)
{
    Brush     baseBr;
    Brush     side;
    // create a cylinder standing on ZOX.
    double   arcstep = DOIPI/radstrips;
    Vtx2     basePoly[36];
    Vtx2     vSide[3];
    double   arcPrev=0.0;
    Poly     pu,pd,ps;
    double   uplimit = DOIPI + arcstep/2.0;
    double   tuside = 0;
    double   tustep = 1.0/radstrips;
    V3       prevUpDn[2];
    V3       topVct(0,height/2.0f,0);
    // create top cover. bottom and side in one shot
    
    _cf._tm = tm;
    
    int nSec = 0;
    for(double arc = arcstep; arc < uplimit; )
    {
        
        //up and down covers
        basePoly[nSec]._xyz.y   = -height/2.0;
        basePoly[nSec]._xyz.x   = sin(arcPrev)*rad;
        basePoly[nSec]._xyz.z   = cos(arcPrev)*rad;
        
        basePoly[nSec]._uv[TX_0].u = .5+sin(arcPrev)/2.0;
        basePoly[nSec]._uv[TX_0].v = .5+cos(arcPrev)/2.0;
        
        
        if(nSec > 0)
        {
            vSide[0]._xyz = basePoly[nSec-1]._xyz;
            vSide[1]._xyz = basePoly[nSec]._xyz;
            vSide[2]._xyz = topVct;
            
            
            switch(tm)
            {
                default:
                case TM_LIN:
                    vSide[0]._uv[TX_0] = UV(0.f, 0);
                    vSide[1]._uv[TX_0] = UV(0.f, 1);
                    vSide[2]._uv[TX_0] = UV(1.f, .5f);
                break;
                case TM_CYL:
                    vSide[0]._uv[TX_0] = UV(0.f, tuside-tustep);
                    vSide[1]._uv[TX_0] = UV(0.f, tuside);
                    vSide[2]._uv[TX_0] = UV(1.f, tuside-(tustep/2.f));
                break;
                case TM_SPH:
                {
                    V3 vn[3];
                    
                    vn[0] = vSide[0]._xyz;
                    vn[1] = vSide[1]._xyz;
                    vn[2] = vSide[2]._xyz;
                    
                    
                    vn[0].norm();
                    vn[1].norm();
                    vn[2].norm();
                    
                    vSide[0]._uv[TX_0] = UV(((vn[0].x/PI)+.50), ((-vn[0].z/PI)+.50));
                    vSide[1]._uv[TX_0] = UV(((vn[1].x/PI)+.50), ((-vn[1].z/PI)+.50));
                    vSide[2]._uv[TX_0] = UV(((vn[2].x/PI)+.50), ((-vn[2].z/PI)+.50));
                }
                break;
            }
            
            vSide[0]._uv[TX_LM] = UV(0.f, 0);
            vSide[1]._uv[TX_LM] = UV(0.f, 1);
            vSide[2]._uv[TX_LM] = UV(1.f, .5f);
            
            ps.Create(3,vSide,this);
            side   << ps;
        }
        
        arcPrev =  arc;
        arc     += arcstep;
        tuside  += tustep;
        nSec++;
    }
    
    vSide[1]._xyz = basePoly[0]._xyz;
    vSide[0]._xyz = basePoly[nSec-1]._xyz;
    vSide[2]._xyz = topVct;
    
    switch(tm)
    {
        default:
            case TM_LIN:
            vSide[0]._uv[TX_0] = UV(0.f, 0);
            vSide[1]._uv[TX_0] = UV(0.f, 1);
            vSide[2]._uv[TX_0] = UV(1.f, .5f);
        break;
        case TM_CYL:
            vSide[0]._uv[TX_0] = UV(0.f, tuside-tustep);
            vSide[1]._uv[TX_0] = UV(0.f, 1);
            vSide[2]._uv[TX_0] = UV(1.f, tuside-(tustep/2.f));
        break;
        
        case TM_SPH:
        {
            V3 vn[3];
            
            vn[0] = vSide[0]._xyz;
            vn[1] = vSide[1]._xyz;
            vn[2] = vSide[2]._xyz;
            
            vn[0].norm();
            vn[1].norm();
            vn[2].norm();
            
            vSide[0]._uv[TX_0] = UV(((vn[0].x/PI)+.50), ((-vn[0].z/PI)+.50));
            vSide[1]._uv[TX_0] = UV(((vn[1].x/PI)+.50), ((-vn[1].z/PI)+.50));
            vSide[2]._uv[TX_0] = UV(((vn[2].x/PI)+.50), ((-vn[2].z/PI)+.50));
        }
        break;
        
    }
    vSide[0]._uv[TX_LM] = UV(0.f, 0);
    vSide[1]._uv[TX_LM] = UV(0.f, 1);
    vSide[2]._uv[TX_LM] = UV(1.f, .5f);
    
    ps.Create(3,vSide,this);
    side   << ps;
    
    pu.Create(nSec,basePoly,this);
    baseBr<<pu;
    
    baseBr.Reverse();
    //// 
    
    if(!CopyDiametral(side))
    {
        (*this)<<side;
    }
    (*this)<<baseBr;
    
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        if(tm == TM_CYL)
          (*b).FlipTC('V');
        else
            (*b).CalcTexCoord(1,0);
    }

	Recalc();
    Dirty(1);
}

void    Brush::CalctexCoord(int idx)
{
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        (*b).CalcTexCoord(1,idx);
    }
    Recalc();
}

//--|    Brush::MakeStairs|---------------------------------------------------------------
void    Brush::MakeStairs(V3&   dims,                   // bbox extend
                         int   radstrips,
                         int   yDimension,
                         REAL  sThick,
                         TEX_MAP tm)
{
    // yDimension are number if stairs
    // xDimension is ignored
    // thikness is the thikness of a stair, if 0 the stair is full
    // build the stair rising ito the screen
    double      stepY   = dims.y / (double)yDimension;
    double      stepZ   = -(dims.z-yDimension) / (double)yDimension;//allow union
    V3          nextOrg = -dims/2;
    
    _cf._tm = tm;
    
    Brush       bt[64];
    PBrushes    pbses;
    
    /*  ////????
    if(sThick > 0)
    {
        if(sThick < stepY)
          sThick = stepY+stepY/2; //allow union
        
    }
    */
    // build each stair as a full solid box
    for(int i=0; i < yDimension; i++)
    {
        if(sThick > 0)
          bt[i].MakeCube(V3(dims.x, sThick, stepZ));
        else
            bt[i].MakeCube(V3(dims.x, stepY*(i+1), stepZ));
        
        // if(GBackFace)
        //bt[i].Reverse();
        
        bt[i].Move(nextOrg);
        pbses << &bt[i];
        nextOrg +=  V3(0,stepY/2.0,stepZ);
    }
    // union them
    Compiler::Union(pbses,*this);
    Reverse();
    Recalc();
    Dirty(1);
}

//--|    Brush::MakeSphere|---------------------------------------------------------------
void    Brush::MakeSphere(REAL fRadius, int hBands, int vBands, TEX_MAP tm)
{
    REAL            deltaZ  = (2.0 * fRadius)/(vBands-1);
    vvector<V3>     vv3s;
    V3              vx[4];
    Brush           b1;
    int             hb,vb;
    Poly            p;

    double          tuside = 0;
    double          tustep = 1.0/hBands;
    double          tvstep = 1.0/vBands;
    double          tvside = tvstep;
    
    vv3s << V3(0,-fRadius, 0);
    
    for(REAL fZ=(-fRadius)+deltaZ/2.0; fZ<(fRadius-deltaZ/2.0+deltaZ/4.0); fZ+=deltaZ)
    {
        REAL rr = sqrt(fRadius*fRadius - fZ*fZ);
        REAL dt = (REAL)(DOIPI / (REAL)hBands);
        
        for(REAL ft=0; ft < DOIPI-(dt*.5);ft+=dt)
        {
            //vv3s << V3(sinf(ft) * rr, cosf(ft) * rr, -fZ);
            vv3s << V3(sinf(ft) * rr, fZ, cosf(ft) * rr);
        }
    }
    vv3s << V3(0,fRadius,0);

    tuside = 0;
    for( vb=0;vb < hBands;vb++)
    {
        
        vx[0] =  vv3s[0];
        vx[1] =  vv3s[((1 + vb) % hBands) + 1];
        vx[2] =  vv3s[(vb % hBands)+1];
        
        p.Create(3, vx, this);
        
        if(tm == TM_SPH)
        {
            vx[0].norm();
            vx[1].norm();
            vx[2].norm();
            
            
            p._vtci[0]._uv[TX_0] = UV(-((vx[0].x/PI)+.50), ((vx[0].y/PI)+.50));
            p._vtci[1]._uv[TX_0] = UV(-((vx[2].x/PI)+.50), ((vx[2].y/PI)+.50));
            p._vtci[2]._uv[TX_0] = UV(-((vx[1].x/PI)+.50), ((vx[1].y/PI)+.50));
            
        }else if(tm == TM_CYL)
        {
            p._vtci[0]._uv[TX_0] = UV(tuside+tustep/2,0);
            p._vtci[1]._uv[TX_0] = UV(tuside, tvside);
            p._vtci[2]._uv[TX_0] = UV(tuside+tustep, tvside);
        }
        
        
        (*this) << p;
        p.Clear();
        tuside +=tustep;
    }
    
    
    for( hb=1;hb < (vBands-1);hb++)
    {
        tuside = 0;
        for(vb=0;vb < hBands;vb++)
        {
            vx[0] = vv3s[(((hb-1)*hBands)+1)+vb];
            vx[1] = vv3s[(((hb-1)*hBands)+1)+((vb+1)%hBands)];
            vx[2] = vv3s[((hb*hBands)+1)+((vb+1)%hBands)];
            vx[3] = vv3s[((hb*hBands)+1)+vb];
            p.Create(4, vx, this);
            
            if(tm == TM_SPH)
            {
                
                vx[0].norm();
                vx[1].norm();
                vx[2].norm();
                vx[3].norm();
                
                p._vtci[0]._uv[TX_0] = UV(-((vx[0].x/PI)+.50), ((vx[0].y/PI)+.50));
                p._vtci[1]._uv[TX_0] = UV(-((vx[1].x/PI)+.50), ((vx[1].y/PI)+.50));
                p._vtci[2]._uv[TX_0] = UV(-((vx[2].x/PI)+.50), ((vx[2].y/PI)+.50));
                p._vtci[3]._uv[TX_0] = UV(-((vx[3].x/PI)+.50), ((vx[3].y/PI)+.50));
                
            }else if(tm == TM_CYL)
            {
                p._vtci[0]._uv[TX_0] = UV(tuside,        tvside);
                p._vtci[1]._uv[TX_0] = UV(tuside+tustep, tvside);
                p._vtci[2]._uv[TX_0] = UV(tuside+tustep, tvside+tvstep);
                p._vtci[3]._uv[TX_0] = UV(tuside,        tvside+tvstep);

            }
            
            (*this) << p;
            p.Clear();
            tuside +=tustep;
        }
        tvside += tvstep;
    }
    
    tuside = 0;
    for(vb=0;vb < hBands;vb++)
    {
        vx[0] =vv3s[1+((vBands-2)*hBands)+vb];
        vx[1] =vv3s[1+((vBands-2)*hBands)+((vb+1)%hBands)];
        vx[2] =vv3s[((vBands-1)*hBands)+1];
        p.Create(3, vx, this);
        
        if(tm == TM_SPH)
        {
            vx[0].norm();
            vx[1].norm();
            vx[2].norm();
            
            
            p._vtci[0]._uv[TX_0] = UV(-((vx[0].x/PI)+.50), ((vx[0].y/PI)+.50));
            p._vtci[1]._uv[TX_0] = UV(-((vx[1].x/PI)+.50), ((vx[1].y/PI)+.50));
            p._vtci[2]._uv[TX_0] = UV(-((vx[2].x/PI)+.50), ((vx[2].y/PI)+.50));
            
        }else if(tm == TM_CYL)
        {
            p._vtci[0]._uv[TX_0] = UV(tuside, 1-tvstep);
            p._vtci[1]._uv[TX_0] = UV(tuside+tustep, 1-tvstep);
            p._vtci[2]._uv[TX_0] = UV(tuside+tustep/2,1);
        }
        
        (*this) << p;
        p.Clear();
        tuside +=tustep;
    }
    
    // Rotate(V3(PIPE2,0,0));
    Recalc();
    
    if(tm == TM_SPH||tm == TM_CYL)
        GenTexCoord(1);
    else
        GenTexCoord(0);
    Dirty(1);
}


//--|    Brush::IsSealed|-----------------------------------------------------------------
BOOL    Brush::IsSealed()
{
    return TRUE;
}


//--|    Brush::Seal|---------------------------------------------------------------------
void    Brush::Seal()
{
    /*

    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        Vertexes::iterator vb  = b->_vtci.begin();
        Vertexes::iterator ve  = b->_vtci.end();
        for(;vb!=ve;vb++)
        {

            Polys::iterator b2 = b;
            Polys::iterator e2  = _polys.end();
            for(;b2!=e2;b2++)
            {
                if(b1 == b)
                    continue;

                Vertexes::iterator vb2  = b2->_vtci.begin();
                Vertexes::iterator ve2  = b2->_vtci.end();
                for(;vb2!=ve2;vb2++)
                {
                    if( IsZero(vb2->_xyz.x, vb->_xyz.x, .1) && 
                        IsZero(vb2->_xyz.y, vb->_xyz.y, .1) &&
                        IsZero(vb2->_xyz.z, vb->_xyz.z, .1) && 
                    {
                        vb->_xyz = vb2->_xyz;
                    }
                }
            }
        }
    }
    */
    TriangulateDeplanetizedFaces();
}


//--|  Brush::GetWorldPos|----------------------------------------------------------------
Brush&  Brush::GetWorldPos()
{
    static  Brush   locoBrsh;
    
    locoBrsh.Clear();
    locoBrsh = (*this);
    
    Polys::iterator b  = locoBrsh._polys.begin();
    Polys::iterator e  = locoBrsh._polys.end();
    for(;b!=e;b++)
    {
        Poly& ps = *b;
        ps.Transform(_mt);
    }
    locoBrsh.Shift(_f);
    locoBrsh.Recalc();
    
    return locoBrsh;
}

//--|    Brush::RotateTCOneStep|----------------------------------------------------------
void    Brush::RotateTCOneStep()
{
    Polys::iterator b  = _polys.begin();
    Polys::iterator e  = _polys.end();
    for(;b!=e;b++)
    {
        Poly& ps = *b;
        ps.RotateTC(1,0);
    }
}

//--|    Brush::Snap2grid|----------------------------------------------------------------
void    Brush::Snap2grid(REAL pgs, BOOL m)
{
    int gs   = (int) pgs;
    V3I vMin;
    V3I vMax;
    V3I mod;
    
    if(m)
    {
        V3 c = _box.GetCenter();
        V3 t;
        
        mod.x = abs((int)c.x % gs);
        mod.y = abs((int)c.y % gs);
        mod.z = abs((int)c.z % gs);
        
        if(mod.x < gs/2)
          t.x-=gs;
        else
            t.x+=gs;
        
        if(mod.y < gs/2)
          t.y-=gs;
        else
            t.y+=gs;
        
        if(mod.z < gs/2)
          t.z-=gs;
        else
            t.z+=gs;
        Move(t);
        
    }
    else
    {
        
        vMin.x = _box._min.x;
        vMin.y = _box._min.y;
        vMin.z = _box._min.z;
        
        vMax.x = _box._max.x;
        vMax.y = _box._max.y;
        vMax.z = _box._max.z;
        
        mod.x = abs(vMin.x % gs);
        mod.y = abs(vMin.y % gs);
        mod.z = abs(vMin.z % gs);
        
        if(mod.x < gs/2)
          vMin.x-=gs;
        else
            vMin.x+=gs;
        
        if(mod.y < gs/2)
          vMin.y-=gs;
        else
            vMin.y+=gs;
        
        if(mod.z < gs/2)
          vMin.z-=gs;
        else
            vMin.z+=gs;
        
        mod.x = vMax.x % gs;
        mod.y = vMax.y % gs;
        mod.z = vMax.z % gs;
        
        if(mod.x < gs/2)
          vMax.x-=gs;
        else
            vMax.x+=gs;
        
        if(mod.y < gs/2)
          vMax.y-=gs;
        else
            vMax.y+=gs;
        
        if(mod.z < gs/2)
          vMax.z-=gs;
        else
            vMax.z+=gs;
        
        Box nb;
        nb._min.x=vMin.x ;
        nb._min.y=vMin.y ;
        nb._min.z=vMin.z ;
        
        nb._max.x=vMax.x ;
        nb._max.y=vMax.y ;
        nb._max.z=vMax.z ;
        
        V3 s = nb.GetExtends()/_box.GetExtends();
        Scale(s,V0,1);
    }
}

//--|    Brush::Reinit|-------------------------------------------------------------------
void    Brush::Reinit()
{
    V3      ex  = _box.GetExtends();
    V3      pos = _box.GetCenter();
    Clear();
    
    switch(_cf._t)
    {
        case E_CUBE:
            MakeCube(ex, _cf._tm);
        break;
        case E_CONE:
            MakeCone(ex.x, ex.y, _cf._hstrips, _cf._vstrips, _cf._tm);
        break;
        case E_CYL:
            MakeCylinder(ex.x, ex.y, _cf._hstrips, _cf._vstrips, _cf._tm);
        break;
        case E_SPHERE:
            break;
        case E_SHEET:
            break;
        case E_STAIRS:
            MakeStairs(ex, _cf._hstrips, _cf._vstrips, _cf._thick,_cf._tm);
        break;
    }
}

//--|    Brush::MakeSheet|----------------------------------------------------------------
void    Brush::MakeSheet(V3& dims, int yDimension, int xDimension, TEX_MAP tm)
{
    //// make sheet allways on xoy
    Vtx2 v[4];
    REAL xstp = dims.x/(REAL)xDimension;
    REAL ystp = dims.y/(REAL)yDimension;
    REAL ys   = -dims.y/2.f;
    REAL vstp   = 1.0f/(REAL)yDimension;
    REAL us     = 0;
    REAL vs     = 1;
    
    Poly     ps;
    //build faces facing up
    for(int i=0; i < yDimension; i++)
    {
        REAL xs   = -dims.x/2.f;
        REAL ustp   = 1.0f/(REAL)xDimension;
        
        for(int j=0; j < xDimension; j++)
        {
            v[3]._xyz = V3(xs,ys,0);
            v[2]._xyz = V3(xs+xstp,ys,0);
            v[1]._xyz = V3(xs+xstp,ys+ystp,0);
            v[0]._xyz = V3(xs,ys+ystp,0);
            
            v[0]._uv[TX_0] = UV(us, vs);
            v[1]._uv[TX_0] = UV(us+ustp, vs);
            v[2]._uv[TX_0] = UV(us+ustp, vs-vstp);
            v[3]._uv[TX_0] = UV(us, vs-vstp);
            
            v[0]._uv[TX_LM] =  v[3]._uv[TX_0];
            v[1]._uv[TX_LM] =  v[2]._uv[TX_0];
            v[2]._uv[TX_LM] =  v[1]._uv[TX_0];
            v[3]._uv[TX_LM] =  v[0]._uv[TX_0];
            
            ps.Create(4,v,this);
            (*this)  << ps;
            ps.Clear();
            
            xs += xstp;
            us += ustp;
        }
        
        
        ys += ystp;
        vs -= vstp;
    }
    
    FOREACH(vvector<Poly>,_polys,p)
           p->_props |= FACE_SHOWBACK;
    
    //StoreInitiaTexCoord();
	GenTexCoord(0);
	GenTexCoord(1);

    Recalc();
    Dirty(1);
}

//--| Brush::SetPos|----------------------------------------------------------------------
void Brush::SetPos(V3& center)
{
    V3 depl = center - _box.GetCenter();
    this->Move(depl);
}

//--| Brush::SetSize|---------------------------------------------------------------------
void Brush::SetSize(Box& extends)
{
    
    V3 rap = (extends._max-extends._min)/(_box._max-_box._min);
    V3 origin = _box.GetCenter();
    
    for(UINT i=0; i< _polys.size();i++)
    {
        Vertexes::iterator vb  = _polys[i]._vtci.begin();
        Vertexes::iterator ve  = _polys[i]._vtci.end();
        for(;vb!=ve;vb++)
        {
            vb->_xyz -= origin;             // origin
            vb->_xyz *= rap;                // scale
            vb->_xyz += origin;             // repos back
        }
    }
}

//--|    Brush::GenTexCoord|--------------------------------------------------------------
void    Brush::GenTexCoord(int ti)
{
    FOREACH(Polys, _polys, pPoly)
    {
        pPoly->ReUV(ti);
    }
    StoreInitiaTexCoord();
}

//--| Brush::StoreInitiaTexCoord|---------------------------------------------------------
void Brush::StoreInitiaTexCoord()
{
    FOREACH(Polys, _polys, pPoly)
    {
        FOREACH(vvector<Vtx2>, pPoly->_vtci, pvtci)
        {
            pvtci->_tcCrea = pvtci->_uv[TX_0];
        }
        pPoly->_texcoord.rotate=0;
        pPoly->_texcoord.scalex=1.0;
        pPoly->_texcoord.scaley=1.0;
        pPoly->_texcoord.shiftx=1.0;
        pPoly->_texcoord.shifty=1.0;
        pPoly->_texcoord.flipV=0;
        pPoly->_texcoord.flipH=0;
    }
}

//--|    Brush::ErasePoly|----------------------------------------------------------------
void    Brush::ErasePoly(Poly& p)
{
    if(_polys.erase_if(p))
        Dirty(1);
}

void    Brush::AddPolys(vvector<Poly>& polys)
{
    FOREACH(vvector<Poly>, polys, pp)
    {
        pp->_pBrush = this;
        _polys.push_back(*pp);
    }
    Dirty(1);
}

//--|    Brush::AddPoly|------------------------------------------------------------------
void    Brush::AddPoly(Poly& p)
{
    if(_polys.push_unique(p))
    {
        p._pBrush = this;
        Dirty(1);
    }
}

//--| int GetVal |------------------------------------------------------------------------
static int GetVal (BYTE* pBuff, int maxX, int x, int y)
{
    return *(pBuff + (y * (maxX+1) + x));
}

//--| Brush::GetTerrainPoly|--------------------------------------------------------------
Poly& Brush::GetTerrainPoly(int x, int y, int tridx)
{
    return _polys[y*_cf._hstrips + x + tridx];
}


//--| Brush::MakeTerrain|-----------------------------------------------------------------
void Brush::MakeTerrain(V3& dims, int xDimension, int yDimension, BYTE* pBuffer)
{
    //// make sheet allways on xoy
    xDimension--;
    yDimension--;
    Vtx2 v[4];
    REAL xstp = dims.x/(REAL)xDimension;
    REAL ystp = dims.y/(REAL)yDimension;
    REAL ys   = -dims.y/2.f;
    REAL vstp = 1.0f/(REAL)yDimension;
    REAL us   = 0;
    REAL vs   = 1;
    
    Poly     ps;
    //build faces facing up
    for(int i=0; i < yDimension; i++)
    {
        REAL xs   = -dims.x/2.f;
        REAL ustp   = 1.0f/(REAL)xDimension;
        
        for(int j=0; j < xDimension; j++)
        {
            v[0]._xyz = V3(xs,ys,GetVal(pBuffer,xDimension,j,i));
            v[1]._xyz = V3(xs+xstp,ys,GetVal(pBuffer,xDimension,j+1,i));
            v[2]._xyz = V3(xs+xstp,ys+ystp,GetVal(pBuffer,xDimension,1+j,1+i));
            v[3]._xyz = V3(xs,ys+ystp,GetVal(pBuffer,xDimension,j,1+i));
            
            v[0]._uv[TX_0] = UV(us, vs);
            v[1]._uv[TX_0] = UV(us+ustp, vs);
            v[2]._uv[TX_0] = UV(us+ustp, vs-vstp);
            v[3]._uv[TX_0] = UV(us, vs-vstp);

            for(int k=0;k<4;++k)
            {
                v[0]._uv[k] =  v[0]._uv[TX_0];
                v[1]._uv[k] =  v[1]._uv[TX_0];
                v[2]._uv[k] =  v[2]._uv[TX_0];
                v[3]._uv[k] =  v[3]._uv[TX_0];
            }

            ps.Create(4,v,this);
            
            ps._flags |= POLY_TERRAIN;

            (*this)  << ps;
            ps.Clear();
            
            xs += xstp;
            us += ustp;
        }
        
        
        ys += ystp;
        vs -= vstp;
    }
    TriFanIt();
	Recalc();
    Dirty(1);
}


//--|    Brush::Split|--------------------------------------------------------------------
BOOL    Brush::Split(Brush& splBrush, Brush& frontB, Brush& backB)
{
    

    MiniBsp          cutsBsp;
    MiniBsp          brshBsp;
    Brush            cpx2Split;
    vvector<Poly*>   untouched;
    
    cpx2Split.Equal(*this,   FALSE);  // make a this copy
    
    if(!cutsBsp.Compile(splBrush._polys,0))
    {
        return FALSE;
    }

    FOREACH(vvector<Poly>, _polys, pp)
    {
        if(pp->_box.IsTouchesBox(splBrush._box))
        {
            cpx2Split._polys << *pp;
        }
        else
        {
            untouched.push_back(&(*pp));
        }
    }
    
    if(cpx2Split._polys.size())
    {
        if(!brshBsp.Compile(cpx2Split._polys,0))
        {
            return FALSE;
        }
        
        // cut out this->polys ending in the solid cut brush
        backB._polys.clear();
        cutsBsp.WantDroppedPolys(&backB._polys);

        if(_flags & BRSH_CSGTERRAIN)
        {
            cutsBsp._cutflag = MiniBsp::PART_ON_FRONT;
        }

        cutsBsp.ClipBrPolys(cpx2Split._polys, frontB._polys,1,1,1);

        frontB._flags = _flags|BRSH_NEW;
        frontB._group = _group;
        frontB._props = _props;
    
        backB._flags = _flags|BRSH_NEW;
        backB._group = _group;
        backB._props = _props;
        
        FOREACH(vvector<Poly*>, untouched, ppu){
            frontB._polys << *(*ppu);
        }

        backB.Recalc();
        frontB.Recalc();
    }
    return frontB._polys.size() || backB._polys.size();
}

void    Brush::TriangulateDeplanetizedFaces()
{
    V3      normal;

AGAIN1:
    FOREACH(vvector<Poly>, _polys, pp)
    {
        normal = pp->_n;
        if(pp->_flags & POLY_DELETED)
            continue;
        if(pp->IsDeplanetized())
        {
            // mark this deleted
            pp->_flags |= POLY_DELETED;
            int  nVects = pp->_vtci.size();            
            int  nOut   = (nVects-2);   // calc out polys
            Poly pout;
            
            for(int i=0;i<nOut; i++)
            {
                pout.CopyProps(*pp);
            
                pout._vtci << pp->_vtci[0];
                pout._vtci << pp->_vtci[i+1];
                pout._vtci << pp->_vtci[i+2];
                pout._pBrush = this;
                pout.Recalc();
                _polys.push_back(pout);
                goto AGAIN1;
            }
        }
    }
    // add the new ones
AGAIN:
    _FOREACH(vvector<Poly>, _polys, pp)
    {
        if(pp->_flags & POLY_DELETED){
            _polys.erase(pp);
            goto AGAIN;
        }
    }
    Recalc();
}

void    Brush::HalloUnHallo()
{   
    REAL r = min (64, _box.GetMinExtend()/3); 
    
    if(_thikness==0)
    {
        StartSecGrp(r);

        Brush b = (*this);

        b.Scale(V3(-r,0,-r), V3(r,0,r), 1);
        b.Scale(V3(-r,0,-r), V3(0,0,0), 1);
        b.Scale(V3(0,-r,0),  V3(0,r,0), 1);
        b.Scale(V3(0,-r,0),  V3(0,0,0), 1);

        b.Recalc();
        b.Reverse();

        AppendPrim(b);
    }
    else
    {
        _thikness= 0;
        
        ASSERT(_polySec == _polys.size()/2);

        for(UINT i=_polySec; i< _polys.size(); ++i)
            _polys[i]._flags |= POLY_DELETED;
AGAIN:
        FOREACH(Polys, _polys, p)
        {
            if(p->_flags & POLY_DELETED)
            {
                _polys.erase(p);
                goto AGAIN;
            }

        }
        _polySec = 0; 
    }
    Recalc();

}


REAL    Brush::GetVolume()
{
    REAL d;   
    REAL volume = 0, areea;
    V3&  corner = _polys.front()._vtci[0]._xyz;

    FOREACH(Polys, _polys, p)
	{
		d      = -(Vdp(corner, p->_n) - p->_c);
		areea  = p->GetAreea();
		volume += d * areea;
	}
	return (volume/3.0);
}

void    Brush::CalcEnvTc(int stage )
{
    UV  uvMin;
    UV  uvMax;
    
    FOREACH(Polys, _polys, pPoly)
    {
        FOREACH(vvector<Vtx2>, pPoly->_vtci, pvtci)
        {
            pvtci->_uv[stage].u = pvtci->_xyz.x * 128 + 127;
            pvtci->_uv[stage].v = pvtci->_xyz.z * 128 + 127;

            pvtci->_uv[stage].u/=_box.GetExtends().x;
            pvtci->_uv[stage].v/=_box.GetExtends().z;

            /*
			pvtci->_uv[0].norm();
			pvtci->_uv[2]=pvtci->_uv[0];
			pvtci->_uv[3]=pvtci->_uv[0];
            */
		}
 	}
}


void Brush::UndoLastTr()
{
    if(_t.len2())
    {
        Move(-_t);
    }
    if(_s.isnot1())
    {
        V3 cs = V1/_s;
        ScaleTr(cs, -_t);
    }
    if(_r.len2())
    {
        Rotate(-_r);
    }
    Recalc();
    _s.setval(1.0);
    _r.setval(0.0);
    _t.setval(0.0);
}


//--|  Brush::operator=|------------------------------------------------------------------
BOOL  Brush::CopyDiametral(const Brush& b)
{
   
    if(this == &b)
        return 0;

     int isz     = ((Brush&)b)._polys.size();

     if(isz < 6)
         return 0;

     if(isz%2 ==1 )
         return 0;

    _unicID    = GID++;
    _t         = b._t;
    _r         = b._r;
    _s         = b._s;
    _mt        = b._mt;
    _selected  = b._selected;
    _copyed    = b._copyed;
    _physicsA  = b._physicsA;
    _rotSteps  = b._rotSteps;
    _physicsD  = b._physicsD;
    _box       = b._box;
    _flags     = b._flags;
    _props     = b._props;
    _group     = b._group;
    _polySec   = b._polySec;
    _thikness  = b._thikness;
    _cf        = b._cf;
    _pMotion   = b._pMotion; // just point same as previous
    _nTrigers  = 0;    // don't copyes triggers
    // paste take care of this
    TCHAR name[64];
    _tcscpy(name, b._name);
    StripDigits(name);
    if(strlen(name))
        _stprintf(_name, "%s%d", name, _unicID);
    else
        _stprintf(_name, "%d", _unicID);
    _polys.clear();


    

    if(isz <= 8 )
    {
        Poly pn;
        int iszHalf = isz/2;
        int acc = 0,i0,i1;
        for(i0=0, i1=iszHalf; acc < isz; i0++,i1++)    
        {
            if(i0 < iszHalf)
            {
                pn = ((Brush&)b)._polys[i0];
                pn._pBrush = this;
                _polys.push_back(pn);
                acc++;
            }
            pn = ((Brush&)b)._polys[i1];
            pn._pBrush = this;
            _polys.push_back(pn);
            acc++;
        }
    }
    else 
    {
        Poly pn;
        int  iszQuad  = isz/4;
        int  iszHalf  = 2*isz/4;
        int  isz3Quad = 3*isz/4;
        int  acc = 0,i0,i1,i2,i3;
        for(i0=0, i1=iszQuad, i2=iszHalf, i3 =isz3Quad; acc<isz; acc++,i0++,i1++,i2++,i3++)
        {
            if(i0 < iszQuad)
            {
                pn = ((Brush&)b)._polys[i0];
                pn._pBrush = this;
                _polys.push_back(pn);
                acc++;
            }
            if(i2 < isz3Quad)
            {
                pn = ((Brush&)b)._polys[i2];
                pn._pBrush = this;
                _polys.push_back(pn);
                acc++;
            }
            if(i1 < iszHalf)
            {
                pn = ((Brush&)b)._polys[i1];
                pn._pBrush = this;
                _polys.push_back(pn);
                acc++;
            }
            pn = ((Brush&)b)._polys[i3];
            pn._pBrush = this;
            _polys.push_back(pn);
            acc++;
        }
    }
   
    
    DeleteResultPtr();
    if(b._pResult != &b)    // if b has a result
    {
        _pResult  = new Brush(*b._pResult);
        ::_tcscpy(_pResult->_name, b._pResult->_name);
    }
    return 1;
}


void Brush::ExplodeInConvexBrushes(PBrushes& pResults)
{
    /**
    // see if are faces facing inwards is a hallo
    // if outwards is a solid and may be a detail
    this->R_Explode(pResults);
    */
    if(IsConvex())
        return;

    // make a bsp out of it
    // store the leafs
    // calc the portals
    // store as convex region any leaf and it's portals
}

void Brush::R_Explode(PBrushes& pResults)
{
    /*
    Polys*   pSplitter = 0;
    //  flag all faces which are splitting the solis
    //  and mark them as splitters
    FOREACH(Polys, _polys, p0)
    {
        int f=0,b=0;
        FOREACH(Polys, _polys, p1)
        {
            if(p0 == p1)
                continue;
            Vertexes::iterator vb  = (p1)->_vtci.begin();
            Vertexes::iterator ve  = (p1)->_vtci.end();
            for(; vb!=ve; vb++)
            {
                if((*p0)->GetSide(vb->_xyz)>=0)
                    ++f;
                else
                    ++b;
            }
        }
        if(f && b)
        {
            pSplitter = *p0;
            break;
        }
    }

    // no splitter found. store this as a return
    if(pSplitter == 0)
    {
        pResults->push_back(this);
        return;
    }

    // get all faces who are splitting the solid and
    // split the solid in 2
    // split the brush with this poly plane
    Brush* pNewA = new Brush();
    Brush* pNewB = new Brush();

    int splOp = SPlitByPlane(pSplitter, pNewA, pNewB);

    // from splitting face make a huge poly and
    // cap both solids. Reverse the face if necesarely
    if(splOp & 1)
    {
        pNewA->CapIt(p0);
        pNewA->R_Explode(pResults);
    }
    if(splOp & 2)
    {
        pNewB->CapIt(p0);
        pNewB->R_Explode(pResults);
    }
    */
}
/*
int    Brush::CapIt(Plane* p0)
{
    // make a huge poly and add it to both ends.
    // do not take this one in next splitting by another splitter
    // finally seal all remains
}
*/

int    Brush::SPlitByPlane(Plane& p0, Brush* pNewA, Brush* pNewB)
{
    Poly a;
    Poly b;
	int rv = 0;

    FOREACH(Polys, _polys, p)
    {

        p->Split(p0, a, b);
        
        if(a._vtci.size() >=3 )
        {
            pNewA->AddPoly(a);
            rv |= 1;
        }

        if(b._vtci.size() >=3 )
        {
            pNewA->AddPoly(b);
            rv |= 2;
        }
        a.Clear();
        b.Clear();
    }
    return rv;
}

// tests if a face divides the brush
BOOL  Brush::IsConvex()
{
    int f=0,b=0;    // fronts and backs
    FOREACH(Polys, _polys, p0)
    {
        FOREACH(Polys, _polys, p1)
        {
            if(p0 == p1)
                continue;
            Vertexes::iterator vb  = (p1)->_vtci.begin();
            Vertexes::iterator ve  = (p1)->_vtci.end();
            for(; vb!=ve; vb++)
            {
                if(p0->GetSide(vb->_xyz)>=0)
                    ++f;
                else
                    ++b;
            }
        }
        if(f && b) // concave
            break;
    }
    return !(f&&b);
}

BOOL  Brush::RecalcBoxAndNormals(BOOL withnormals)
{
	do{
		_box.Reset();
		FOREACH(Polys, _polys, p)
		{
			p->_box.Reset();
			p->CalcNormal();
			p->_area = p->GetAreea();
			FOREACH(vvector<Vtx2>, p->_vtci, vt)
			{
				vt->_extra1 = 0; // used for calc normals
				vt->_nrm = (p->_n);
				p->_box.AddPoint(vt->_xyz);
				_box.AddPoint(vt->_xyz);
			}
			p->_pBrush = this;
		}
	}while(0);

    if(GLight || withnormals)
    {
        CalcVxNormals();
    }
    return 1;
}


void Brush::FindNormal(Vtx2& tv, Polys& polys)
{
    FOREACH(Polys, polys, p)
    {
        FOREACH(vvector<Vtx2>,p->_vtci, vtci)
        { 
            if(0==_intask) {_intask=-1; return ;}
            if(tv._xyz == vtci->_xyz)
            {
                tv._nrm += p->_n;
                ++tv._extra1;
            }
        }
    }
}


void    Brush::CalcVxNormals()
{
    if(BrushTask )
        theApp._brushtask.Add(this, Brush::CalcVxTask);
    else
        CalcVxTask();
}


int    Brush::CalcVxTask()
{
    // search this in scene
    BOOL    found = FALSE;
    PBrushes* rbs = DOC()->_scene.GetPrimitives();
    FOREACH(PBrushes, (*rbs), ppB)
    {
        if(*ppB==this || (*ppB)->_pResult==this)
        {
            found = TRUE;
            break;
        }
    }
    if(!found)
        return 0; 
    
    
    _intask = 1;
    FOREACH(Polys, _polys, p)
    {
        FOREACH(vvector<Vtx2>,p->_vtci, vtci)
        {
            FindNormal(*vtci, _polys);
        }
        if(0==_intask) {_intask=-1; return 0;}
    }

    _FOREACH(Polys, _polys, p)
    {
        FOREACH(vvector<Vtx2>,p->_vtci, vtci)
        {
            vtci->_nrm /= (REAL)vtci->_extra1;
            vtci->_nrm.norm();
        }
        if(0==_intask) {
            _intask=-1; 
            return 0;
        }
    }
    _intask = -1;
    return 1;
}
//-----------------------------------------------------------------------------
int     Brush::GetPolyCount(DWORD flags, DWORD props)
{
    int nCount = 0;
    FOREACH(Polys, _polys, p)
    {
        if((p->_flags & flags) && 
            (p->_props &  props))
            ++nCount;
    }
    return nCount;
}

//-----------------------------------------------------------------------------
int   Brush::Triangulate()
{
    if(_flags |= BRSH_TRIANGLES)
    {
        if(_polys.size() > 256)
        {
            Beep(400,20);
            return 0;
        }

        const Poly& first = _polys.front();
        if(vdist(first._vtci[0]._xyz, first._vtci[1]._xyz) < 16 ||
           vdist(first._vtci[0]._xyz, first._vtci[2]._xyz) < 16 ||
           vdist(first._vtci[1]._xyz, first._vtci[2]._xyz) < 16)
        {
            Beep(400,20);
            return 0;
        }
    }


    vvector<Poly>   bpolys = _polys;
    _polys.clear();
    FOREACH(vvector<Poly>, bpolys, ppoly)
    {
        vvector<Poly>   fragments;
        ppoly->Triangulate(fragments);
        AddPolys(fragments);
    }
    _flags |= BRSH_TRIANGLES;
    Recalc();
    return 1;
}

//-----------------------------------------------------------------------------
void    Brush::TriFanIt()
{
    if(_flags & BRSH_TRIANGLES)
    {
        return;
    }

    vvector<Poly>   bpolys = _polys;
    _polys.clear();
    FOREACH(vvector<Poly>, bpolys, ppoly)
    {
        vvector<Poly>   fragments;
        ppoly->TrifanIt(fragments);
        AddPolys(fragments);
    }
    _flags |= BRSH_TRIANGLES;
    Recalc();
}

BOOL    Brush::HasFlag(DWORD f){
    return (_flags&f)!=0;
}

BOOL    Brush::IsCut(){
    return (_flags == 0) || (_flags & BRSH_CUTALL) || _flags == 2;
}

BOOL    Brush::IsProjector(){
    return _flags & BRSH_PROJECTOR;
}

BOOL    Brush::IsSplitter(){
    return _flags & BRSH_SPLITTER;
}

BOOL    Brush::IsZone(){
    return _flags & BRSH_ZONE;
}

BOOL    Brush::IsSelection()
{
    return IsSplitter() || IsZone();
}

BOOL    Brush::IsSpecial()
{
    return (IsSelection() || (_flags & BRSH_BIGTERRAIN));
}

Poly *  Brush::GetNextPoly(class Poly *p)
{
    int el = _polys.findelement(*p) - _polys.begin();
    return &_polys.nextr(el);
}

Poly *  Brush::GetPrevPoly(class Poly *p)
{
    int el = _polys.findelement(*p) - _polys.begin();
    return &_polys.prevr(el);
}
