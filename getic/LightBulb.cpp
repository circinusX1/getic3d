// LightBulb.cpp: implementation of the CLightBulb class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "texadapter.h"
#include "MainFrm.h"
#include "z-edmap.h"
#include "z_ed2Doc.h"
#include "LightBulb.h"
#include "z_ed2View.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightBulb::CLightBulb():SceItem()
{
    _euler          = V3(G2R(-90),0,0);                 // spheric no spot
    _specAngle      = 0;                   // no angle 
    _lmIntensity    = 50.0;               
    _radius         = 200;                 // in cm 
    _colorD         = ZWHITE;   // diffuze
	_colorE			= ZWHITE;	// emmisive
	_colorS			= ZWHITE;	// spec
	_atten[0]		= 1;	// for the 3 colors
	_atten[1]		= 1;	// for the 3 colors
	_atten[2]		= 1;	// for the 3 colors
    _halloRadMin    = 32;   // cm
    _halloRadMax    = 400;  // cm
    _minDist		= 100;
    _maxDist		= 1600;
    _item           = ITM_LIGTBULB;
    strcpy(_catname ,       "ITM_LIGTBULB") ;
	GetTexs()[0]    = _lbulbtex;
    sprintf(_name,"Light%d",_id);
}

CLightBulb::CLightBulb(const CLightBulb& r):SceItem((const SceItem&)r)
{
    _specAngle   = r._specAngle  ;
    _lmIntensity = r._lmIntensity;
    _radius      = r._radius     ;
    _halloRadMin = r._halloRadMin;
    _halloRadMax = r._halloRadMax;
	_colorE      = r._colorE;
	_colorS      = r._colorS;
	_atten[0]    = r._atten[0];
    _atten[1]    = r._atten[1];
    _atten[2]    = r._atten[2];
    _minDist     = r._minDist;
    _maxDist     = r._maxDist;
}

CLightBulb& CLightBulb::operator=(const CLightBulb& r)
{
    if(this != &r)
    {
        this->SceItem::operator=((const SceItem&)(r));
        _specAngle   = r._specAngle  ;
        _lmIntensity = r._lmIntensity;
        _radius      = r._radius     ;
        _halloRadMin = r._halloRadMin;
        _halloRadMax = r._halloRadMax;
	    _colorE      = r._colorE;
	    _colorS      = r._colorS;
	    _atten[0]    = r._atten[0];
        _atten[1]    = r._atten[1];
        _atten[2]    = r._atten[2];
        _minDist     = r._minDist;
        _maxDist     = r._maxDist;

    }
    return *this;
}

CLightBulb::~CLightBulb()
{
    
}


void    CLightBulb::GlDraw2D(CZ_ed2View* pV)
{
    int i;
	char ax = pV->_vt;
    SceItem::GlDraw2D(pV);

    glPushMatrix();
    Ta.Disable();
    glTranslatef(_t.x,_t.y,_t.z);
	int texIdx = GetHtex(GUtex);

    if(0==_specAngle)
	{
		switch(ax)
		{
			case 'x':
				glRotatef(-90,0,1,0);
				texIdx+=1;
				break;
			case 'y':
				glRotatef(-90,1,0,0);
				texIdx+=2;
				break;
			case 'z':
				texIdx+=3;
				break;
		}
	}
	
/*
    do{
		glColor4ubv(CLR_SELECT);
		glEnable(GL_COLOR_MATERIAL);
        Ta.BindTexture(texIdx);
        glBegin(GL_POLYGON);
        for(int i=0;i<4;++i)
        {
            glVertex3f(_Vtci[i]._xyz.x, _Vtci[i]._xyz.y, _Vtci[i]._xyz.z);
            glTexCoord2f(_Vtci[i]._uv[GUtex].u, _Vtci[i]._uv[GUtex].v);
        }

        glEnd();
    }while(0);
  */  

    int linewith = 1;

    if(_flags & BRSH_NEW)
    {
        glColor4ubv(CLR_NEW_BRITEM);
    }
    else
    {
        if(!IsSelected())
        {
            glColor4ub((_colorD.r/3)*2, (_colorD.g/3)*2, (_colorD.b/3)*2, 128);
        }
        else
        {
            glColor4ub(_colorD.r, _colorD.g, _colorD.b, 255);
        }
    }
    

    if(BShowLCirc)
    {
        if(_specAngle == 0)
        {

            REAL step = 0;
            // draw a circle from 30 x 30 degreees on xoy xoz zoy
            V3  cirpctX[12];
            V3  cirpctY[12];
            V3  cirpctZ[12];

            for( i=0;i<12;i++)
            {
                cirpctX[i].z = _radius * sinf(step);
                cirpctX[i].y = _radius * cosf(step);

                cirpctY[i].x = _radius * sinf(step);
                cirpctY[i].z = _radius * cosf(step);

                cirpctZ[i].x = _radius * sinf(step);
                cirpctZ[i].y = _radius * cosf(step);

                step+=PIPE6;
            }

            if(IsSelected())
                glLineWidth(2.0f) ;        

            glBegin(GL_LINE_LOOP);
            for(i=0;i<12;i++)
                glVertex3f(cirpctZ[i].x, cirpctZ[i].y, cirpctZ[i].z);
            glEnd();
            if(IsSelected())
                glLineWidth(1.0f) ;        

            if(IsSelected())
            {
                glColor4ubv(CLR_SELECT);
                glBegin(GL_LINES);
                for(i=0;i<12;i+=2)
                {
                    glVertex3fv((REAL*)V0);
                    glVertex3f(cirpctZ[i].x, cirpctZ[i].y, cirpctZ[i].z);
                }
                glEnd();

            }
        }
        else 
        {
            REAL deg = G2R(_specAngle);
            V3   endQuad[4];
            REAL dHV   = tanf(deg/2.0) * _radius;
            // piramide
            endQuad[0] = -VZ * _radius + VY * dHV - VX * dHV;
            endQuad[1] = -VZ * _radius + VY * dHV + VX * dHV;
            endQuad[2] = -VZ * _radius - VY * dHV + VX * dHV;
            endQuad[3] = -VZ * _radius - VY * dHV - VX * dHV;

            // already translated
    		
		    _euler.y ? glRotatef(R2G(_euler.y), 0.0,1.0,0.0):(void)0;
		    _euler.x ? glRotatef(R2G(_euler.x), 1.0,0.0,0.0):(void)0;
		    _euler.z ? glRotatef(R2G(_euler.z), 0.0,0.0,1.0):(void)0;
            
            glBegin(GL_LINES);
            for(i=0;i<4;i++)
            {
                glVertex3f(0,0,0);
                glVertex3f(endQuad[i].x,endQuad[i].y,endQuad[i].z); 
            }
            glEnd();

            glBegin(GL_LINE_LOOP);
                for(i=0;i<4;i++)glVertex3f(endQuad[i].x,endQuad[i].y,endQuad[i].z); 
            glEnd();
        }
    }

    Ta.Restore();
    glPopMatrix();

}

Htex& CLightBulb::GetTexture()
{
    Htex& ht = GetHtex(GUtex);
    if(ht)   return ht;
    return _lbulbtex;
}

void CLightBulb::Serialize(FileWrap* pfv, 
                             const vvector<Texture>& texNames, 
                             map<int,int>& texassoc)
{
    SceItem::Serialize(pfv, texNames, texassoc);

    pfv->Serialize(_specAngle);
    pfv->Serialize(_lmIntensity);
    pfv->Serialize(_radius);
    pfv->Serialize(_halloRadMin);
    pfv->Serialize(_halloRadMax);
    pfv->Serialize(_minDist);
    pfv->Serialize(_maxDist);
	pfv->Serialize(_colorE);
	pfv->Serialize(_colorS);
	pfv->Serialize(_atten, sizeof(REAL)*3);

    REAL falloff; //addif necesarelly
    pfv->Serialize(falloff);
    pfv->Serialize(falloff);
    pfv->Serialize(falloff);

}