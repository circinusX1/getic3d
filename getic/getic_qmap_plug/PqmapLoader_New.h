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

//-------------------------- Loader-------------------------------------------------
class PqmapLoader  : public IGeticPlug
{
public:
    PqmapLoader(){};
    virtual ~PqmapLoader(){};
	long _stdcall ImportFile(IGeticEditor* pe, TCHAR* bsFileName, Plg_Scene** ppBrush) ;
	long _stdcall ExportFile(IGeticEditor* pe, TCHAR* bsFileName, const Plg_Scene* pBrush);
    long _stdcall ReleaseScene(Plg_Scene* ppBrush)						   ;
    long _stdcall GetMenuStringAndType(char* bsFileName,DWORD* type);
    long _stdcall ExportFile(TCHAR* bsFileName, const Plg_Scene* ppScene)  ;
    long _stdcall GetVersion();
private:
    long PostProcess(Plg_Scene** pScene);
    void MakeConvex(Plg_Brush&, vvector<Plg_Brush*>);
    void MakeConvex(QBrCvx&, vvector<QBrCvx>&);
    int  IsConvex(QBrCvx& inbrush, QBrCvx& a, QBrCvx& b, QPoly* splPoly);
};

//-----------------------------------------------------------------------------
class    Parser;
class    QBrushFace;
class    QBrush;
class    QEntity;

//-----------------------------------------------------------------------------
typedef     bool (Parser::*PfHandler)(vvector<string>& tokens);
//-----------------------------------------------------------------------------
#define     POLY_IS_SPLITTER    0x10000000
struct QPoly
{
    QPoly(){_texIdx=-1;_flag=0;}
    vvector<V3> _vxes;
    vvector<UV> _uvs;
    int         _texIdx;
    DWORD       _flags;
};

//-----------------------------------------------------------------------------
struct QBrCvx              // convex brush
{
    QBrCvx(){_flag=0;}
    vvector<QPoly> _polys;
    DWORD          _flags;
};

//-----------------------------------------------------------------------------
// parses first open brakets
class Parser
{
public:
    Parser(FileWrap* pf):_pfw(pf){
        _pLine   = 0;
        _pBrush  = 0;
        _pEntity = 0;
        _classes["{"]  = (PfHandler)Handle_Open;
    }
    virtual     ~Parser(){}
    int         Step();
    virtual int Parse(vvector<string>& tokens);

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
    int                      _scope;
};

//-----------------------------------------------------------------------------
// parses faces
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

    virtual ~QBrushFace(){}
    QBrushFace(FileWrap* pf, QBrush* pParent):Parser(pf),_pParent(pParent),_OK(0),_indexTex(-1) {}
    int Handle_Line(vvector<string>& tokens);
    QBrush* _pParent;
};

//-----------------------------------------------------------------------------
// parses brushes
class QBrush : public Parser
{
public:
    QBrush(FileWrap* pf, QEntity* pParent):Parser(pf),_pParent(pParent)
    {
        _classes["}"]               = (PfHandler)Handle_Close;
        _classes["("]               = (PfHandler)Handle_Line;
    }
    virtual ~QBrush(){
        FOREACH(vvector<QBrushFace*>, _pbrshlines, ppFace){
            delete *ppFace;
        }
    }
    int Handle_Close(vvector<string>& tokens);
    int Handle_Line(vvector<string>& tokens);

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
// pareses entities
class QEntity : public Parser
{
public:
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
    
    QEntity(FileWrap* pf):Parser(pf)
    {
        _classes["\"origin\""]      = (PfHandler)Handle_Origin;
        _classes["\"classname\""]   = (PfHandler)Handle_ClassName;
        _classes["{"]               = (PfHandler)Handle_Open;
        _classes["}"]               = (PfHandler)Handle_Close;
        _classes["/"]               = (PfHandler)Handle_Comment;
        _classes["\""]              = (PfHandler)Handle_KeyVal;
    }

    int Handle_Open(vvector<string>& tokens);
    int Handle_Close(vvector<string>& tokens);
    int Handle_Comment(vvector<string>& tokens);
    int Handle_KeyVal(vvector<string>& tokens);
    int Handle_Origin(vvector<string>& tokens)
    {
        ::sscanf(tokens[1].c_str(),"\"%f %f %f\"",&_pos.x, &_pos.y, &_pos.z);
        return 1;
    }
    int Handle_ClassName(vvector<string>& tokens)
    {
        _classname = tokens[1];
        return 1;
    }
};

//---------------------------------------------------------------------------------------
// wipes out 'ch'. returns the ptr into psz
INLN TCHAR*	StripChar(TCHAR* psz, TCHAR ch)
{
	TCHAR* ps = psz;
	TCHAR* pd = psz;
	while(*ps){
		if(*ps==ch)
		{	ps++;continue;}
		*pd++=*ps++;
	}
	*pd=0;
	return psz;
}

#endif // !__QMAP_GETICPLG_H__




