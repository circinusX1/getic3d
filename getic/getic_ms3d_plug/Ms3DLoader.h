// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************

#if !defined(AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_)
#define AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_

#include "baselib.h"
#define HELP_ME
#include "..\\_include\\geticplug.h"

//-------------------------- MSD-------------------------------------------------
typedef struct MS3D_HEADER_TYP
{
		char id[10];				//The ID number of the model (always "MS3D000000")
		int  version;				//The version number of the model
} MS3D_HEADER, *MS3D_HEADER_PTR;

typedef struct MS3D_VERTEX_TYP
{
	unsigned char flags;
	unsigned char refCount;
	char boneID;				//The bone ID (used for skeletal animation)
	float vertex[3];
} MS3D_VERTEX, *MS3D_VERTEX_PTR;

typedef struct MS3D_TRIANGLE_TYP
{
	unsigned short flags;
	unsigned short vertexIndices[3];
	float vertexNormals[3][3];
	float u[3];
	float v[3];
	unsigned char smoothingGroup;
	unsigned char groupIndex;
} MS3D_TRIANGLE, *MS3D_TRIANGLE_PTR;

typedef struct MS3D_GROUP_TYP
{
	unsigned char	flags;
	char			name[32];
	unsigned short	numTriangles;
	unsigned short*	triangleIndices;
	char			materialIndex;
} MS3D_GROUP, *MS3D_GROUP_PTR;

class Ms3DLoader  : public IGeticPlug
{
public:
    Ms3DLoader(){};
    virtual ~Ms3DLoader(){};

	long _stdcall ImportFile(IGeticEditor* pe, char*,char* bsFileName, Plg_Scene** ppBrush) ;
	long _stdcall ExportFile(IGeticEditor* pe, char*,char* bsFileName, const Plg_Scene* pBrush);
    long _stdcall ReleaseScene(Plg_Scene* ppBrush);
    long _stdcall GetMenuStringAndType(char* bsFileName,DWORD* type);
    long _stdcall ExportFile(char* bsFileName, const Plg_Scene* ppScene);
    long _stdcall GetVersion(){return 1;};

};

#endif // !defined(AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_)
