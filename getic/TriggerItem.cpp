// TriggerItem.cpp: implementation of the TriggerItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "texadapter.h"
#include "z-edmap.h"
#include "TriggerItem.h"
#include "z_ed2Doc.h"
#include "z_ed2View.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TriggerItem::TriggerItem():SceItem(),_bSelEfect(0),_pBrush(0)
{
    _item = ITM_TRIGER;
    _drawBrush.MakeCube(V3(64.0f));    //1 m default
	_drawBrush.Reverse();

    _drawBrush.Recalc();
    _efectPoint = _t + VY * 100.0;

    _stprintf(_name,"Trigger%d",_id);
	_brIndex = -1;
}

TriggerItem::TriggerItem(const TriggerItem& r):SceItem( (const SceItem& ) (r) )
{
    _pBrush     = r._pBrush    ;
    _efectPoint = r._efectPoint;
	_brIndex    = r._brIndex   ; 
    _drawBrush  = r._drawBrush ; 
	_bSelEfect  = r._bSelEfect ;

}

TriggerItem&  TriggerItem::operator=(const TriggerItem& r)
{
    if(this != &r)
    {
        this->SceItem::operator=((const SceItem&)(r));
        _pBrush     = r._pBrush    ;
        _efectPoint = r._efectPoint;
	    _brIndex    = r._brIndex   ; 
        _drawBrush  = r._drawBrush ; 
	    _bSelEfect  = r._bSelEfect ;

    }
    return *this;
}


TriggerItem::~TriggerItem()
{
    if(_pBrush)
    {
		_pBrush->_brushflags &= ~BRSH_HASTRIG;
    }
}

void    TriggerItem::GlDraw2D(CZ_ed2View* pV)
{
    Ta.Disable();

    if(_flags & BRSH_NEW)
        glColor4ubv(CLR_NEW_BRITEM);
    else
    {
        if(!IsSelected())
            glColor4ubv(CLR_TRIGGER);
        else
            glColor4ubv(CLR_SELECT);
    }    

    PPoly pb = _drawBrush._polys.begin();
    PPoly pe = _drawBrush._polys.end();
    for(;pb!=pe;pb++)
    {
        Poly& pl = *pb;

        glBegin(GL_LINE_LOOP);
        PVertex vb = pl._vtci.begin();
        PVertex ve = pl._vtci.end();
        for(;vb!=ve;vb++)
        {
            Vtx2& vtci = *vb;
            glVertex3f(vtci._xyz.x,vtci._xyz.y,vtci._xyz.z);
        }
        glEnd();
    }


    glColor4ubv(CLR_MOTION);
    glBegin(GL_LINES);
        glVertex3f(_t.x,_t.y,_t.z);    
        glVertex3f(_efectPoint.x,_efectPoint.y,_efectPoint.z);    
    glEnd();

    glColor4ubv(CLR_WHITE);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
        glVertex3f(_efectPoint.x,_efectPoint.y,_efectPoint.z);
        glVertex3f(_t.x,_t.y,_t.z);
    glEnd();
    glPointSize(1.0f);

	if(pV){
		glPushMatrix();
		glTranslatef(_t.x,_t.y,_t.z);
		DOC()->PrintOut(V0, pV->_vt, IS[_item].text, CLR(250,84,84,255));
		glPopMatrix();
	}

}

// this is a bounding box
void    TriggerItem::GlDraw(z_ed3View* pV)
{
    GlDraw2D(0);
}

void    TriggerItem::GlDrawSel(z_ed3View* pV)
{
    GlDraw2D(0);
}

OVERWRITE void    TriggerItem::Move(V3& t)
{
	if(IsEfectSelected()){
		// move the effect point
		_efectPoint+=t;
		FindTargetBrush();
	}else{
		if(0 ==_pBrush)
		{
			_efectPoint+=t;
			FindTargetBrush();
			_drawBrush.Move(t);
			_drawBrush.Recalc();
			_t = _drawBrush._box.GetCenter(); // update the t 
		}
		else
		{
			_drawBrush.Move(t);
			_drawBrush.Recalc();
			_t = _drawBrush._box.GetCenter(); // update the t 
		}
	}
}

OVERWRITE void    TriggerItem::Scale(V3& s, V3 t, REAL sc)
{
    if(!IsEfectSelected())
    {
        _drawBrush.Scale(s, t, 1);
        _drawBrush.Recalc();
        _t = GetWBox().GetCenter();
        _s = V1;                        // don not scale as long the scale is reflected imed. in the brush
//        FindTargetBrush();
    }

}

void    TriggerItem::FindTargetBrush()
{
    //
    // clen the trigger flag (for info only)
    //
    SetEfectBrush(0);
    //
    // search in the brushes to see if effect point land on some.
    // if is contained in some brush, center it in the center of effect brush
    //
    PBrushes* rbs = DOC()->GetForms();
    Box      box ;
    FOREACH(PBrushes, (*rbs), ppB)
    {
        Brush& rb = (**ppB);

        //
        // reject other brushes and non detail brushes
        //
        if(DOC()->_curgroup>=0 && DOC()->_curgroup != rb._group)
            continue;
        if(!(rb._brushflags & BRSH_DETAIL))
            continue;
        if( rb._brushflags & BRSH_WIREBOX)
            continue;

		box = rb._box;  
        //
        // may be a surface
        //
		box.Expand0();

        if(box.ContainPoint(_efectPoint))
        {
            //
            // store the efector brush here
            //
            if(!(rb._brushflags & BRSH_HASTRIG))
            {
                SetEfectBrush(&rb);
            }
            return;
        }
    }
}


void TriggerItem::Serialize(FileWrap* pfw, 
                             const vvector<Texture>& texNames, 
                             map<int,int>& texassoc)
{
    V3    extend;
    int   ptr = -1;

    SceItem::Serialize(pfw, texNames, texassoc);
    pfw->Serialize(_efectPoint);

    if(pfw->_store)
    {
        extend = _drawBrush._box.GetExtends();
        pfw->Write(extend);
        if(_pBrush)
        {
            pfw->Write(_pBrush->_unicID);
        }
        else
        {
            pfw->Write(ptr);    //-1
        }
    }
    else
    {
        pfw->Read(extend);
        extend.x /= 2.0;
        extend.y /= 2.0;
        extend.z /= 2.0;
        _drawBrush.SetSize(Box(-extend,extend));
		_drawBrush.Recalc();

        int brushId;
        pfw->Read(brushId);
        if(brushId !=-1)
        {
            _pBrush = DOC()->_scene.GetBrushByID(brushId);
			if(_pBrush)
			{
				_pBrush->_brushflags |= BRSH_HASTRIG;
                _pBrush->_brushflags &= ~BRSH_NEW;
			}
        }
        #pragma message("is this brush new on triger")
        _flags &= ~BRSH_NEW;
    }
}