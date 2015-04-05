#ifndef _GETIC_PLUGIN_H__
#define _GETIC_PLUGIN_H__

#include <windows.h>
#include <tchar.h>

typedef struct _Plg_Vertex
{
	float	xyz[3];
	float	nrm[3];
	float	uv[2];
	float	lm[2];
}Plg_Vertex;

typedef struct _Plg_Poly
{
	DWORD		flags;			
	BYTE		color[4];		
	Plg_Vertex*	vXes;			
	int			nCnt;			

}Plg_Poly;

typedef struct _Plg_Brush
{
	DWORD		flags;			
	Plg_Poly	*pPolys;		
	int			nCnt;			
	short		name[32];		
}Plg_Brush;

typedef struct _Plg_Plane
{
	float	normal[3];
	float	ddist;
}Plg_Plane;

#define 	PLUG_IMPORTER   1
#define 	PLUG_EXPORTER   2

struct IGeticPlug 
{
	virtual long _stdcall ImportFile(TCHAR* bsFileName, Plg_Brush** ppBrush)       = 0;
	virtual long _stdcall ExportFile(TCHAR* bsFileName, Plg_Brush* pBrush)         = 0;
	virtual long _stdcall GetMenuStringAndType(TCHAR* bsFileName,DWORD* type) = 0;
    virtual long _stdcall ReleaseBrush(Plg_Brush* ppBrush)=0;

};

#endif 