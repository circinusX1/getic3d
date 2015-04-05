// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************
#ifndef __QMAP_GETICPLG_H__
#define __QMAP_GETICPLG_H__

#include "baselib.h"
#include "basecont.h"
#include <map>
#include <string>
#include <vector>
#include "..\\_include\\geticplug.h"

using namespace std ;

//-------------------------- map file-------------------------------------------------


//-------------------------- Loader-------------------------------------------------
class PqmapLoader  : public IGeticPlug
{
public:
    PqmapLoader(){};
    virtual ~PqmapLoader(){};
	long _stdcall ImportFile(IGeticEditor* pe, char* installDir, char* bsFileName, Plg_Scene** ppBrush) ;
	long _stdcall ExportFile(IGeticEditor* pe, char* installDir, char* bsFileName, const Plg_Scene* pBrush);
    long _stdcall ReleaseScene(Plg_Scene* ppBrush)						   ;
    long _stdcall GetMenuStringAndType(char* bsFileName,DWORD* type);
    long _stdcall ExportFile(char* bsFileName, const Plg_Scene* ppScene)  ;
    long _stdcall GetVersion();
private:
    long PostProcess(Plg_Scene** pScene);
    long PostProcess2(Plg_Scene** pScene);
    string  _mapname;
};

//-----------------------------------------------------------------------------
class    Parser;
class    QBrushFace;
class    QBrush;
class    QEntity;

//-----------------------------------------------------------------------------
typedef     bool (Parser::*PfHandler)(vvector<string>& tokens);
//-----------------------------------------------------------------------------
struct QPoly
{
    vvector<V3> _vxes;
    vvector<UV> _uvs;
    int         _texIdx;
};

//-----------------------------------------------------------------------------
class Parser
{
public:
    enum{
        _PARSER,
        _BRUSH,
        _ENTITY,
        _FACE,
    };
    Parser(FileWrap* pf):_pfw(pf){
        _pLine   = 0;
        _pBrush  = 0;
        _pEntity = 0;
        _classes["{"]  = (PfHandler)&Parser::Handle_Open;
    }
    virtual     ~Parser(){}
    int         Step();
    virtual int Parse(vvector<string>& tokens);
    virtual int T(){return _PARSER;}
private:
    void    Explode(const char* pline, vvector<string>& tokens, char token);
    int     BaseParse(const char* line);
    int     Handle_Open(vvector<string>& tokens);

protected:    
    FileWrap*                _pfw;
    QBrushFace*              _pLine;
    QBrush*                  _pBrush;
    QEntity*                 _pEntity;
    map<string, PfHandler>   _classes;
    vvector<string>          _tokenspersect;
    static bool              _doubleopen;  
    int                      _scope;
};

//-----------------------------------------------------------------------------
class QEntity;
class QBrush ;
class QBrushFace : public Parser
{
public:
    Plane _plane;
    V3    _ax[2];
    UV    _shft;
    V3    _center;
    BOOL  _OK;
    int   _indexTex;

    virtual int T(){return _FACE;}
    virtual ~QBrushFace(){}
    QBrushFace(FileWrap* pf, QBrush* pParent):Parser(pf),_pParent(pParent),_OK(0),_indexTex(-1) {}
    int Handle_Line(vvector<string>& tokens);
    QBrush* _pParent;
};

//-----------------------------------------------------------------------------
class QBrush : public Parser
{
public:
    QBrush(FileWrap* pf, QEntity* pParent):Parser(pf),_pParent(pParent)
    {
        _classes["}"]               = (PfHandler)&QBrush::Handle_Close;
        _classes["("]               = (PfHandler)&QBrush::Handle_Line;
        
    }
    virtual ~QBrush(){
        FOREACH(vvector<QBrushFace*>, _pbrshlines, ppFace){
            delete *ppFace;
        }
    }
    int Handle_Close(vvector<string>& tokens);
    int Handle_Line(vvector<string>& tokens);
    virtual int T(){return _BRUSH;}

    void Clear(){
        FOREACH(vvector<QBrushFace*>, _pbrshlines, ppl)
        {
            delete *ppl;
        }
        _pbrshlines.clear();
    }

    DWORD                   _flags;
    QEntity*                _pParent;
    vvector<QBrushFace*>    _pbrshlines;
    
};

//-----------------------------------------------------------------------------
class QEntity : public Parser
{
public:
    int                 _intensity;
    DWORD               _flags;
    V3                  _pos;
    string              _classname;
    vvector<QBrush*>    _brushes;
    map<string,string>  _keyvals;

    virtual ~QEntity(){
        FOREACH(vvector<QBrush*>, _brushes, ppb){
            delete *ppb;
        }
    }
    virtual int T(){return _ENTITY;}
    QEntity(FileWrap* pf):Parser(pf)
    {
        _classes["\"light\""]       = (PfHandler)&QEntity::Handle_Light;
        _classes["\"origin\""]      = (PfHandler)&QEntity::Handle_Origin;
        _classes["\"classname\""]   = (PfHandler)&QEntity::Handle_ClassName;
        _classes["{"]               = (PfHandler)&QEntity::Handle_Open;
        _classes["}"]               = (PfHandler)&QEntity::Handle_Close;
        _classes["/"]               = (PfHandler)&QEntity::Handle_Comment;
        _classes["\""]              = (PfHandler)&QEntity::Handle_KeyVal;
    }

    int Handle_Light(vvector<string>& tokens);
    int Handle_Open(vvector<string>& tokens);
    int Handle_Close(vvector<string>& tokens);
    int Handle_Comment(vvector<string>& tokens);
    int Handle_KeyVal(vvector<string>& tokens);
    int Handle_Origin(vvector<string>& tokens)
    {
        if(tokens.size()==4)
        {
            string loco = tokens[1];
                loco += ",";
                loco += tokens[2];
                loco += ",";
                loco += tokens[3];
            ::sscanf(loco.c_str(),"\"%f,%f,%f\"",&_pos.x, &_pos.z, &_pos.y);
        }
        else
            ::sscanf(tokens[1].c_str(),"\"%f %f %f\"",&_pos.x, &_pos.z, &_pos.y);
        _pos.x*=3.0;
        _pos.y*=3.0;
        _pos.z*=3.0;
        return 1;
    }
    int Handle_ClassName(vvector<string>& tokens)
    {
        _classname = tokens[1];
        return 1;
    }
};


//---------------------------------------------------------------------------------------
INLN char*	StripChar(char* psz, char ch)
{
	char* ps = psz;
	char* pd = psz;
	while(*ps){
		if(*ps==ch)
		{	ps++;continue;}
		*pd++=*ps++;
	}
	*pd=0;
	return psz;
}

#endif // !__QMAP_GETICPLG_H__




