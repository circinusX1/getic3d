// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************

#include "stdafx.h"
#include "stdio.h"
#include "baselib.h"
#include "Ms3DLoader.h"

//---------------------------------------------------------------------------------------
long Ms3DLoader::ExportFile(IGeticEditor* pe, char*,char* bsFileName, const Plg_Scene* pBrush)
{
    return -1;
}

//---------------------------------------------------------------------------------------
long Ms3DLoader::GetMenuStringAndType(char* bsFileName, DWORD* type)
{
    //large enough
    _tcscpy(bsFileName,"MS3D Files,ms3d");
    *type = PLUG_IMPORTER;
    return 0;
}

//---------------------------------------------------------------------------------------
long Ms3DLoader::ImportFile(IGeticEditor* pe, char*,char* bsFileName, Plg_Scene** ppScene)
{
    int i;
    
	FILE* f= _tfopen(bsFileName,"rb");
	if(0 == f)	
        return 0;

	
	MS3D_HEADER header = {0};

    //Read the header data in
	fread(&header.id, sizeof(char), 10, f);
	fread(&header.version, 1, sizeof(int), f);
	if(strncmp(header.id, "MS3D000000", 10)!=0)
	{
		return FALSE;
	}
	if(header.version!=3 && header.version!=4)
	{
		return false;
	}
		   
	//Read the vertex data in
	WORD numVertices = 0;
	fread(&numVertices, sizeof(unsigned short), 1, f);

	MS3D_VERTEX* vertices= new MS3D_VERTEX [numVertices];
	for( i=0; i<numVertices; i++)
	{
		fread(&vertices[i].flags,	sizeof(BYTE),  1, f);
		fread( vertices[i].vertex,	sizeof(float), 3, f);
		fread(&vertices[i].boneID,	sizeof(char),  1, f);
		fread(&vertices[i].refCount, sizeof(BYTE),  1, f);
	}

	//Read the triangle data in
	WORD numTriangles=0;
	fread(&numTriangles, sizeof(unsigned short), 1, f);
	MS3D_TRIANGLE* pTris= new MS3D_TRIANGLE [numTriangles];
	for(i=0; i<numTriangles; i++)
	{
		fread(&pTris[i].flags,			 sizeof(unsigned short), 1, f);
		fread( pTris[i].vertexIndices,	 sizeof(unsigned short), 3, f);
		fread( pTris[i].vertexNormals[0], sizeof(float),			 3, f);
		fread( pTris[i].vertexNormals[1], sizeof(float),		 	 3, f);
		fread( pTris[i].vertexNormals[2], sizeof(float),			 3, f);
		fread( pTris[i].u,				 sizeof(float),			 3, f);
		fread( pTris[i].v,				 sizeof(float),			 3, f);
		fread(&pTris[i].smoothingGroup,	 sizeof(unsigned char),  1, f);
		fread(&pTris[i].groupIndex,		 sizeof(unsigned char),  1, f);
	}

	//Read the group data in

	WORD numGroups;
	fread(&numGroups, sizeof(unsigned short), 1, f);
	MS3D_GROUP*	   pGrps= new MS3D_GROUP [numGroups];
	for(i=0; i<numGroups; i++)
	{
		fread(&pGrps[i].flags,			 sizeof(unsigned char),  1,	 f);
		fread( pGrps[i].name,			 sizeof(char),			 32, f);
		fread(&pGrps[i].numTriangles,	 sizeof(unsigned short), 1,	 f);

		pGrps[i].triangleIndices		= new unsigned short [pGrps[i].numTriangles];

		fread( pGrps[i].triangleIndices, sizeof(unsigned short), pGrps[i].numTriangles,f);
		fread(&pGrps[i].materialIndex,	 sizeof(char), 1, f);
	}
	fclose(f);


	int loop1;
	int loop2;
	int loop3;


    int vertexes = 0;
	for(loop1=0; loop1 < numGroups; loop1++ )
	{
		for(loop2=0; loop2 < pGrps[loop1].numTriangles; loop2++)
        {
            vertexes+=3;
        }
    }
    
	(*ppScene) = new Plg_Scene();
    ::memset((*ppScene), 0, sizeof(Plg_Scene));
	//(*ppScene) = new Plg_Scene;

    (*ppScene)->pBrushes    = new Plg_Brush[1];
    memset((*ppScene)->pBrushes, 0, sizeof(Plg_Brush) * 1);
	(*ppScene)->nBrushes    = 1;
	(*ppScene)->nTextures   = 0;
	(*ppScene)->pTextures   = 0;

    (*ppScene)->pBrushes[0].pPolys = 0;
    (*ppScene)->pBrushes[0].nPolys   = 0;
    int nTris = 0;

    for(loop1=0; loop1 < numGroups; loop1++ )
    {
        nTris+=pGrps[loop1].numTriangles;
    }
    (*ppScene)->pBrushes[0].pPolys = new Plg_Poly[nTris];
    ::memset( (*ppScene)->pBrushes[0].pPolys, 0, sizeof(Plg_Poly)*nTris);
    (*ppScene)->pBrushes[0].nPolys   = nTris;

    nTris = 0;
    Plg_Poly* pHead = (*ppScene)->pBrushes[0].pPolys;

	for(loop1=0; loop1 < numGroups; loop1++ )
	{
		for(loop2=0; loop2 < pGrps[loop1].numTriangles; loop2++)
		{
			int  triangleIndex		 =  pGrps[loop1].triangleIndices[loop2];
			const MS3D_TRIANGLE* tri = &pTris[triangleIndex];

            pHead->nvXes = 3;
            pHead->vXes  = new Vtx[3];
            pHead->color = CLR(255,255,255);
            ::memset(pHead->vXes,0,sizeof(Vtx)*3);

            Vtx* pVh = pHead->vXes;

			for(loop3=0; loop3 < 3; loop3++,pVh++)
			{
				int index= tri->vertexIndices[loop3];

                pVh->_xyz.x = vertices[index].vertex[0];
                pVh->_xyz.y = vertices[index].vertex[1];
                pVh->_xyz.z = vertices[index].vertex[2];

                pVh->_uv[0].u  = tri->u[loop3];
                pVh->_uv[0].v  = tri->v[loop3];

                pVh->_uv[1].u  = tri->u[loop3];
                pVh->_uv[1].v  = tri->v[loop3];

				pVh->_uv[2].u = NAF;
				pVh->_uv[2].v = NAF;
				pVh->_uv[3].u = NAF;
				pVh->_uv[4].v = NAF;
			}
            pHead++;
		}
	}

	for(i=0; i<numGroups; i++)
    {
		delete[] pGrps[i].triangleIndices;
    }
	delete[] pGrps;
	delete[] pTris;
	delete[] vertices;

    return 0;
}

//---------------------------------------------------------------------------------------
long Ms3DLoader::ReleaseScene(Plg_Scene* pScene)
{
    RELEASE_SCENE(pScene);
    return 0;
}