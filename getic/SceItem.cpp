#include "stdafx.h"
#include "TexAdapter.h"
#include "MainFrm.h"
#include "Poly.h"
#include "SceItem.h"
#include "ScriptItem.h"
#include "StartPlayItem.h"
#include "SoundItem.h"
#include "z_ed2Doc.h"
#include "LightBulb.h"
#include "TriggerItem.h"
#include "GameItem.h"
#include "SoundItem.h"

//-----------------------------------------------------------------------------
Vtx    SceItem::_Vtci[4];
BOOL    SceItem::_BInitalized = FALSE;
long    SceItem::_GItemID     = 0; // cd


ItemSig IS[]={
    {"."},
	{"L"},
    {"T"},
    {"S"},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"."},
    {"U"},
    {"X"},
};



SceItem::SceItem():_item(ITM_NONE),_props(0),_flags(0),_colorD(ZWHITE)
{
	_leafIdx     = -1;
    _selected    = FALSE;
    if(_BInitalized == FALSE)
    {
        _BInitalized = TRUE;
        // on yoz facing to us
//        _Vtci[0] = Vtx(V3(-16, 16,0), UV(1,1), ZWHITE,0);
//        _Vtci[1] = Vtx(V3( 16, 16,0), UV(1,0), ZWHITE,0);
//        _Vtci[2] = Vtx(V3( 16,-16,0), UV(0,0), ZWHITE,0);
//        _Vtci[3] = Vtx(V3(-16,-16,0), UV(0,1), ZWHITE,0);


        _Vtci[0] = Vtx(V3(-16, 16,0), UV(0,1), ZWHITE,0);
        _Vtci[1] = Vtx(V3( 16, 16,0), UV(1,1), ZWHITE,0);
        _Vtci[2] = Vtx(V3( 16,-16,0), UV(1,0), ZWHITE,0);
        _Vtci[3] = Vtx(V3(-16,-16,0), UV(0,0), ZWHITE,0);

    }
    _group    = 0;
    _id       = _GItemID++;
    _s        = V1;
    _euler    = V0;
    _zonearea= -1;
    sprintf(_name,"%s",MKSTR("Item%d",_id));
}

SceItem::SceItem(const SceItem& r)
{
    if(GNewUID)
    {
        _id    = _GItemID++;
         char name[64];
        strcpy(name,r._name);
        StripDigits(name);
        if(strlen(name))
            sprintf(_name, "%s%d", name, _id);
        else
            sprintf(_name, "%d", _id);

    }
    else
    {
        _id    = r._id;
        strcpy(_name,r._name);
    }
    strcpy(_catname,r._catname);
    _item       = r._item    ;
    _props      = r._props   ;
    _flags      = r._flags   ;
    _selected   = r._selected;
    _colorD     = r._colorD  ;
    _zonearea   = r._zonearea;
    _t          = r._t       ;
    _s          = r._s       ;
    _euler      = r._euler   ;
    _tl         = r._tl      ;
    _sl         = r._sl      ;
    _dirl       = r._dirl    ;	
    _group      = r._group   ;
    _leafIdx    = r._leafIdx ; 
    _combine    = r._combine;
    _textures   = r._textures;

}

SceItem& SceItem::operator=(const SceItem& r)
{
    if(this!=&r)
    {
        if(GNewUID)
        {
            _id    = _GItemID++;
             char name[64];
            strcpy(name,r._name);
            StripDigits(name);
            if(strlen(name))
                sprintf(_name, "%s%d", name, _id);
            else
                sprintf(_name, "%d", _id);

        }
        else
        {
            _id    = r._id;
            strcpy(_name,r._name);
        }
        strcpy(_catname,r._catname);
        _item       = r._item    ;
        _props      = r._props   ;
        _flags      = r._flags   ;
        _selected   = r._selected;
        _colorD     = r._colorD  ;
        _zonearea   = r._zonearea;
        _t          = r._t       ;
        _s          = r._s       ;
        _euler      = r._euler   ;
        _tl         = r._tl      ;
        _sl         = r._sl      ;
        _dirl       = r._dirl    ;	
        _group      = r._group   ;
        _leafIdx    = r._leafIdx ; 
        _combine    = r._combine;
        _textures   = r._textures;

    }
    return *this;
}


void    SceItem::GlDraw2D(CZ_ed2View* pV)
{
	char ax = pV->_vt;

    if(_flags & BRSH_NEW)
        glColor4ubv(CLR_NEW_BRITEM);
    else
    {
        if(!IsSelected())
            glColor4ub(_colorD.r, _colorD.g, _colorD.b, 255);
        else
            glColor4ubv(CLR_SELECT);
    }
    
    glPushMatrix();
    glTranslatef(_t.x,_t.y,_t.z);
    char s[32];
    
    if(_item == ITM_GAME)
    {
        SCopy(s, (char*)_catname, sizeof(s)-1);
        if(!IsSelected())
            s[3]=0;
    }
    else
    {
        strcpy(s, IS[_item].text);
    }

    
    if(_flags & BRSH_NEW)
    {
        CLR color(CLR_NEW_BRITEM[0],CLR_NEW_BRITEM[1],CLR_NEW_BRITEM[2]);
        DOC()->PrintOut(V0, pV->_vt, "Hit Enter To Commit", color);
    }
    else
    {
        if(DOC()->_selmode== SEL_NONE)
            DOC()->PrintOut(V0, pV->_vt, ".", _colorD);
        else
            DOC()->PrintOut(V0, pV->_vt, s, _colorD);
    }

    glPopMatrix();

    if(!IsSelected())
        glColor4ub(_colorD.r/3, _colorD.g/3, _colorD.b/3,255);
    else
        glColor4ubv(CLR_WHITE);
	

}


SceItem::~SceItem()
{
}

Htex& SceItem::GetTexture()
{
    Htex& rv = GetHtex(0);
	 if(rv) return rv;
    return  _itmqtex;
}


void SceItem::GlDraw(z_ed3View* pV)
{
    int i;
    CLR cd  = _colorD;

    cd.a    = 254;
    glColor4ubv(cd);
    REAL modelview[16];

    glPushMatrix();
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR); 

    Ta.Bind(GetTexture(), 0);
    glTranslatef(_t.x,_t.y,_t.z);
    
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	V3 xUp(modelview[1],modelview[5],modelview[9]);
    for( i=0; i<3; i++ ){
        for(int j=0; j<3; j++ ){
            if ( i==j )
               modelview[i*4+j] = 1;
            else
                modelview[i*4+j] = 0.0f;
        }
    }
    glLoadMatrixf(modelview);

    glBegin(GL_POLYGON);

    for(i=0;i<4;i++)
    {
        glTexCoord2f(_Vtci[i]._uv[GUtex].u, _Vtci[i]._uv[GUtex].v);
        glVertex3f(_Vtci[i]._xyz.x, _Vtci[i]._xyz.y, _Vtci[i]._xyz.z);
    }

    glEnd();
    
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glPopMatrix();


}

void SceItem::GlDrawSel(z_ed3View* pV)
{
	int i;
    REAL modelview[16]; 
    glPushMatrix();
    glTranslatef(_t.x,_t.y,_t.z);

    Ta.Disable();
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    for( i=0; i<3; i++ ){
        for(int j=0; j<3; j++ ){
            if ( i==j )
               modelview[i*4+j] = 1.2f; //scale a bit
            else
                modelview[i*4+j] = 0.0f;
        }
    }
    glLoadMatrixf(modelview);

    glBegin(GL_LINE_LOOP);
    for( i=0;i<4;i++)
        glVertex3f(_Vtci[i]._xyz.x, _Vtci[i]._xyz.y, _Vtci[i]._xyz.z);
    glEnd();
    Ta.Restore();
    glPopMatrix();

    CLR sc = _colorD;
    _colorD = ZWHITE;
    GlDraw(pV);
    _colorD = sc;

    DOC()->DrawOrtho(_t, V0, 40.0, .64);
    DOC()->DrawOrtho(_t, _euler, 64, 1);

}

void    SceItem::Rotate(V3& r, BOOL bao)
{
    if(r.x== -1 && r.y==-1 && r.z==-1)
    {
        _euler.reset();
        _m.identity();
    }
    else
    {
        _euler += r;

        ROLL2PI(_euler.y);
        ROLL2PI(_euler.x);
        ROLL2PI(_euler.z);

        _m.identity();
        if(_euler.y)  _m  *= MRotAround(VY, _euler.y);
        if(_euler.x)  _m  *= MRotAround(VX, _euler.x);
        if(_euler.z)  _m  *= MRotAround(VZ, _euler.z);
    }
}


/*static */ 
SceItem* SceItem::Create(int type)
{
    switch(type)
    {
        case ITM_LIGTBULB:
            return new CLightBulb();
        case ITM_TRIGER:
            return new TriggerItem();
        case ITM_GAME:
            return new GameItem();
        case ITM_SNDSOURCE:
            return new CSoundItem();
        case ITM_STARTPLAY:
            return new CStartPlayItem();
        default:
            break;
    }
    return 0;
}

void SceItem::Serialize(FileWrap* pfw, 
                        const vvector<Texture>& texNames,
                        map<int,int>& texassoc)
{
    pfw->Serialize(_item);
    _catname[15] = 0;
    pfw->Serialize(_catname,16);
    _catname[15] = 0;
    _name[15] = 0;
    pfw->Serialize(_name,16);
    _name[15] = 0;
    pfw->Serialize(_id);
    pfw->Serialize(_props);
    pfw->Serialize(_flags);
	pfw->Serialize(_selected);
    pfw->Serialize(_colorD);
    pfw->Serialize(_zonearea);
    pfw->Serialize(_t);
    pfw->Serialize(_s);
    pfw->Serialize(_euler);
    pfw->Serialize(_group);

    if(pfw->_store)
    {
        size_t dwTmp;
        for(int k=0;k<4;k++)
        {
            map<int, int>::iterator fi = texassoc.find((int)this->GetHtex(k));
            if(fi != texassoc.end())
            {
                pfw->Write((int&) texassoc[this->GetHtex(k)]);
            }
            else
            {
                int  it = -1;
                pfw->Write((int&) it);
            }
            dwTmp = this->Apply(k);
            pfw->Write(dwTmp);
        }
        dwTmp = this->Combine();
        pfw->Write(dwTmp);
    }
    else
    {
        int      tmpVal;
        size_t    dwTmpVal;
    
        for(int k=0; k<4; k++)
        {
            pfw->Read(tmpVal);
            pfw->Read(dwTmpVal);

            if(tmpVal >=0 && (size_t)tmpVal < texNames.size())
            {
                this->SetApply(dwTmpVal,k);
                this->SetTex((char*)texNames[tmpVal].filename, k, texNames[tmpVal].hTex.glTarget);

                if(TexHandler::GDefaultHtexture == this->GetHtex(k))
                {
                    theApp._errors.push_back(texNames[tmpVal].filename);
                }
            }
        }
        size_t combine;
        pfw->Read(combine);//this->_combine);
        this->SetCombine(combine);
        SetPos(_t);
        SceItem::_GItemID = tmax(SceItem::_GItemID,(long)_id);
    }

}




