// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************
#ifndef __PS2D_LOADER_H__
#define __PS2D_LOADER_H__

#include "baselib.h"
#include "basecont.h"
#include "..\\_include\\geticplug.h"

//-------------------------- ------------------------------------------------------------
struct D3dsPoly
{
    unsigned short a[3];
    short flags;
};

//-------------------------- ------------------------------------------------------------
struct D3Ds_Brush
{
    D3Ds_Brush()
    {
        ::memset(name,0,sizeof(name));
    }
	char                 name[32];
    vvector<D3dsPoly>    polys;
};

//-------------------------- ------------------------------------------------------------
struct D3Ds_Scene
{
    D3Ds_Scene()
    {
        _brushes.reserve(64);
        vtxes.reserve(65535);
        texxes.reserve(65535);
    }
    vvector<D3Ds_Brush*>  _brushes;
    vvector<V3>          vtxes;
    vvector<UV>          texxes;

};

//-------------------------- Loader-------------------------------------------------
class P3dsLoader  : public IGeticPlug
{
public:
    P3dsLoader(){};
    virtual ~P3dsLoader(){};
	long _stdcall ImportFile(IGeticEditor* pe, char*,char* bsFileName, Plg_Scene** ppBrush) ;
	long _stdcall ExportFile(IGeticEditor* pe, char*,char* bsFileName, const Plg_Scene* pBrush);
	long _stdcall GetMenuStringAndType(char* bsFileName,DWORD* type);
    long _stdcall ReleaseScene(Plg_Scene* ppBrush);
    long _stdcall GetVersion(){return 1;}
private:
    long _Load3Ds(char* fileName, D3Ds_Scene&  b);
};

#endif // __PS2D_LOADER_H__




