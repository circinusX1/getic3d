// Motion.cpp: implementation of the CMotion class.
//

#include "stdafx.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "Motion.h"
#include "Brush.h"
#include <gl/gl.h>			
#include <gl/glu.h>			
#include <gl/glaux.h>	

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



BOOL    CMotion::Create()
{
//    int             _iNodes;
//    BOOL            _bLoop;
//    vvector<MState> _states;

    V3  pos = _pBrush->_box.GetCenter();

    for(int i=0; i<_iNodes; i++)
    {
        MState  ms;
        ms._trTimes[0]=0;
        ms._trTimes[1]=1000;
        ms._pMotion = this;
        ms._pos     = pos;
        ms._euler   = V0;
        _states << ms;

        pos += VY * 64.0f;
    }
    return 1;
}


//MERGE//>
void   CMotion::GlDraw()
{
    _bLoop ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP) ;
    int i;
    // draw  path
    for( i=0; i<_iNodes; i++)
    {
        glVertex3fv((REAL*)_states[i]._pos);
    }
    glEnd();

    // draw points
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for(i=0; i<_iNodes; i++)
    {
        if(_selNode == i)
        {
            glColor4ubv(CLR_SELECT);
        }

        glVertex3fv((REAL*)_states[i]._pos);

        if(_selNode == i)
        {
            glColor4ubv(CLR_MOTION);
        }

    }
    glEnd();
    glPointSize(1.0f);

    // draw ortho of each point
    for(i=0; i<_iNodes; i++)
    {
        if(_selNode == i)
            DOC()->DrawOrtho(_states[i]._pos, VZ, 16.0, .5);
        {
            V3 euler;
            DOC()->DrawOrtho(_states[i]._pos, _states[i]._euler, 32.0, _selNode == i ? 1.0 : .7);
        }
    }
    // draw orthos
}
//MERGE//<


void CMotion::RefreshPosition()
{
    V3 depl = _pBrush->_box.GetCenter() - _center; 
    _center = _pBrush->_box.GetCenter();
    for(int i=0; i<_iNodes; i++)
    {
        _states[i]._pos += depl;
    }
}