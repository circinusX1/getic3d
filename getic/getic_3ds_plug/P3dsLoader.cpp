// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************

#include "stdafx.h"
#include "stdio.h"
#include "baselib.h"
#include "basecont.h"
#include "P3dsLoader.h"

//---------------------------------------------------------------------------------------
INLN char*	StripChar(char* psz, char ch){
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

//---------------------------------------------------------------------------------------
long P3dsLoader::ExportFile(IGeticEditor* pe, char*,char* bsFileName, const Plg_Scene* pBrush)
{
    return -1;
}

//---------------------------------------------------------------------------------------
long P3dsLoader::GetMenuStringAndType(char* bsFileName, DWORD* type)
{
    _tcscpy(bsFileName,"3Ds Files,3ds");
    *type = PLUG_IMPORTER;
    return 0;
}

//---------------------------------------------------------------------------------------
long P3dsLoader::ReleaseScene(Plg_Scene* pScene)
{
    RELEASE_SCENE(pScene);
    return 0;
}

//---------------------------------------------------------------------------------------
long P3dsLoader::ImportFile(IGeticEditor* pe, char*,char* bsFileName, Plg_Scene** pScene)
{
    D3Ds_Scene  s;

    if(0==_Load3Ds(bsFileName, s))
    {
        //
        // transfer it to the scene
        //
        //1 scene, k brushes, j polys , t vertexes
	    *pScene = new Plg_Scene;
        memset(*pScene, 0 , sizeof(Plg_Scene));

	    (*pScene)->flags      = 0;
	    (*pScene)->nBrushes   = s._brushes.size();
	    (*pScene)->pBrushes   = new Plg_Brush[s._brushes.size()];
        ::memset((*pScene)->pBrushes, 0, sizeof(Plg_Brush) * s._brushes.size());
	    for(int i=0;i<s._brushes.size();i++){
		    D3Ds_Brush* model = s._brushes[i];
		    (*pScene)->pBrushes[i].nPolys   = model->polys.size();
		    (*pScene)->pBrushes[i].pPolys = new Plg_Poly[model->polys.size()];
            ::memset((*pScene)->pBrushes[i].pPolys,  0, sizeof(Plg_Poly) * model->polys.size());
		    (*pScene)->pBrushes[i].flags  = BRSH_SOLID;
		    for(int j=0;j<model->polys.size();j++){
			    D3dsPoly& d3dspoly = model->polys[j];
			    
			    (*pScene)->pBrushes[i].pPolys[j].nvXes = 3;
			    (*pScene)->pBrushes[i].pPolys[j].vXes = new Vtx[3];
			    (*pScene)->pBrushes[i].pPolys[j].color = CLR(255,255,255);

			    for(int k=0;k<3;k++)
			    {
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k]._xyz.x = s.vtxes[d3dspoly.a[k]].x*8;
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k]._xyz.y = s.vtxes[d3dspoly.a[k]].y*8;
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k]._xyz.z = s.vtxes[d3dspoly.a[k]].z*8;

				    (*pScene)->pBrushes[i].pPolys[j].vXes[k]._uv[0].u = s.texxes[d3dspoly.a[k]].u;
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k]._uv[1].v = s.texxes[d3dspoly.a[k]].v;
			    }
		    }
	    }
        
        s._brushes.deleteelements();

        return NO_ERROR;
    }
    return -1;
}

//---------------------------------------------------------------------------------------
long P3dsLoader::_Load3Ds(char* fileName, D3Ds_Scene&  s)
{
	int             i;                     
	FILE*           pfile;                
	unsigned short  chunkid;            
	unsigned int    chunklen;        
	unsigned char   lenchar;                
	unsigned short  quantity;                 

	if ((pfile=fopen (fileName, "rb"))== NULL) 
        return -1; 

    D3Ds_Brush* b = 0;// = new D3Ds_Brush();
    V3          v;
    UV          t;

	while (!feof(pfile)) 
	{
		fread (&chunkid, 2, 1, pfile); //Read the chunk header
		TRACEX("ChunkID: %x\n",chunkid); 
		fread (&chunklen, 4, 1, pfile); //Read the lenght of the chunk
		TRACEX("ChunkLenght: %x\n",chunklen);

        if(feof(pfile))
            break;
		switch (chunkid)
        {
			case 0x4d4d: 
			break;    
			case 0x3d3d:
			break;
			case 0x4000: 
				i=0;
                b = new D3Ds_Brush();
				do
				{
					fread (&lenchar, 1, 1, pfile);
                    b->name[i]=lenchar;
					i++;
                }while(lenchar != '\0' && i<20);
                
                s._brushes.push_back(b);

			break;
			case 0x4100:
			break;
			case 0x4110: 
				fread (&quantity, sizeof (unsigned short), 1, pfile);
                for (i=0; i<quantity; i++)
                {
					fread (&v.x, sizeof(float), 1, pfile);
                    fread (&v.y, sizeof(float), 1, pfile);
					fread (&v.z, sizeof(float), 1, pfile);
                    s.vtxes.push_back(v);
				}
				break;
			case 0x4120:
                {
				    fread (&quantity, sizeof (unsigned short), 1, pfile);
                    b->polys.reserve(quantity);
                    D3dsPoly    p;

                    for (i=0; i<quantity; i++)
                    {
					    fread (&p.a[0], sizeof (unsigned short), 1, pfile);
					    fread (&p.a[1], sizeof (unsigned short), 1, pfile);
					    fread (&p.a[2], sizeof (unsigned short), 1, pfile);
					    fread (&p.flags, sizeof (unsigned short), 1, pfile);
                        b->polys.push_back(p);
				    }
                }
                break;
			case 0x4140:
				fread (&quantity, sizeof (unsigned short), 1, pfile);
                
                
				for (i=0; i<quantity; i++)
				{
					fread (&t.u, sizeof (float), 1, pfile);
                    fread (&t.v, sizeof (float), 1, pfile);

                    s.texxes.push_back(t);
				}
                break;
			default:
				 fseek(pfile, chunklen-6, SEEK_CUR);
        } 
	}
	fclose (pfile); // Closes the file stream
    return NO_ERROR;
}