// T3dLoader.cpp: implementation of the T3dLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdio.h"
#include "..\\_include\\BspFIleStr.h"
#include "baselib.h"
#include "basecont.h"
#include "T3dLoader.h"


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


long T3dLoader::ExportFile(TCHAR* bsFileName, Plg_Scene* pBrush)
{
    return -1;
}

long T3dLoader::GetMenuStringAndType(TCHAR* bsFileName, DWORD* type)
{
    //large enough
    _tcscpy(bsFileName,"3Ds Files,3ds");
    *type = PLUG_IMPORTER;
    return 0;
}





long T3dLoader::ReleaseScene(Plg_Scene* pScene)
{
    if(pScene)
    {
		for(int j=0; j< pScene->nCount;j++)
		{
			for(int i=0; i< pScene->pBrushes[j].nCnt;i++)
			{
				delete[] pScene->pBrushes[j].pPolys[i].vXes;
			}
			delete[]pScene->pBrushes[j].pPolys;
		}
		delete[] pScene->pBrushes;
    }
    delete pScene;
    return 0;
}



long T3dLoader::ImportFile(TCHAR* bsFileName, Plg_Scene** pScene)
{
    D3Ds_Scene  s;
    if(0==_Load3Ds(bsFileName, s))
    {
        //
        // transfer it to the scene
        //
        //1 scene, k brushes, j polys , t vertexes
        
        
	    *pScene = new Plg_Scene;

	    (*pScene)->flags    = 0;//CALC_TEXCOORD1|CALC_TEXCOORD2;
	    (*pScene)->nCount   = s._brushes.size();
	    (*pScene)->pBrushes = new Plg_Brush[s._brushes.size()];

	    for(int i=0;i<s._brushes.size();i++)
	    {
		    D3Ds_Brush* model = s._brushes[i];

		    (*pScene)->pBrushes[i].nCnt   = model->polys.size();
		    (*pScene)->pBrushes[i].pPolys = new Plg_Poly[model->polys.size()];
		    (*pScene)->pBrushes[i].flags  = BRSH_SOLID;

		    for(int j=0;j<model->polys.size();j++)
		    {
			    D3dsPoly& d3dspoly = model->polys[j];
			    
			    (*pScene)->pBrushes[i].pPolys[j].nCnt = 3;
			    (*pScene)->pBrushes[i].pPolys[j].vXes = new Plg_Vertex[3];
			    (*pScene)->pBrushes[i].pPolys[j].color[0] = 255;
			    (*pScene)->pBrushes[i].pPolys[j].color[1] = 255;
			    (*pScene)->pBrushes[i].pPolys[j].color[2] = 255;
			    (*pScene)->pBrushes[i].pPolys[j].color[3] = 255;
			    

			    for(int k=0;k<3;k++)
			    {
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k].xyz[0] = s.vtxes[d3dspoly.a[k]].x;
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k].xyz[1] = s.vtxes[d3dspoly.a[k]].y;
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k].xyz[2] = s.vtxes[d3dspoly.a[k]].z;

				    (*pScene)->pBrushes[i].pPolys[j].vXes[k].uv[0] = s.texxes[d3dspoly.a[k]].u;
				    (*pScene)->pBrushes[i].pPolys[j].vXes[k].uv[1] = s.texxes[d3dspoly.a[k]].v;
			    }
		    }
	    }
        
        s._brushes.deleteelements();

        return NO_ERROR;
    }
    return -1;
}


long T3dLoader::_Load3Ds(TCHAR* fileName, D3Ds_Scene&  s)
{
	int             i;                      //Index variable
	FILE*           l_file;                 //File pointer
	unsigned short  l_chunk_id;             //Chunk identifier
	unsigned int    l_chunk_lenght;         //Chunk lenght
	unsigned char   l_char;                 //Char variable
	unsigned short  l_qty;                  //Number of elements in each chunk

	if ((l_file=fopen (fileName, "rb"))== NULL) 
        return -1; 

    D3Ds_Brush* b = 0;// = new D3Ds_Brush();
    V3          v;
    UV          t;

	while (!feof(l_file)) 
	{
		//getche(); //Insert this command for debug (to wait for keypress for each chuck reading)

		fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
		TRACEX("ChunkID: %x\n",l_chunk_id); 
		fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
		TRACEX("ChunkLenght: %x\n",l_chunk_lenght);

        if(feof(l_file))
            break;
		switch (l_chunk_id)
        {
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d 
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d: 
			break;    

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info 
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x3d3d:
			break;
			
			//--------------- EDIT_OBJECT ---------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
			case 0x4000: 
				i=0;
                b = new D3Ds_Brush();
				do
				{
					fread (&l_char, 1, 1, l_file);
                    b->name[i]=l_char;
					i++;
                }while(l_char != '\0' && i<20);
                
                s._brushes.push_back(b);

			break;

			//--------------- OBJ_TRIMESH ---------------
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4100:
			break;
			
			//--------------- TRI_VERTEXL ---------------
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Lenght: 1 x unsigned short (number of vertices) 
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
			case 0x4110: 
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                
                for (i=0; i<l_qty; i++)
                {
					fread (&v.x, sizeof(float), 1, l_file);
                    fread (&v.y, sizeof(float), 1, l_file);
					fread (&v.z, sizeof(float), 1, l_file);
                    s.vtxes.push_back(v);
				}
				break;

			//--------------- TRI_FACEL1 ----------------
			// Description: Polygons (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk Lenght: 1 x unsigned short (number of polygons) 
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub chunks
			//-------------------------------------------
			case 0x4120:
                {
				    fread (&l_qty, sizeof (unsigned short), 1, l_file);
                    b->polys.reserve(l_qty);
                    D3dsPoly    p;

                    for (i=0; i<l_qty; i++)
                    {
					    fread (&p.a[0], sizeof (unsigned short), 1, l_file);
					    fread (&p.a[1], sizeof (unsigned short), 1, l_file);
					    fread (&p.a[2], sizeof (unsigned short), 1, l_file);
					    fread (&p.flags, sizeof (unsigned short), 1, l_file);
                        b->polys.push_back(p);
				    }
                }
                break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points) 
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
			case 0x4140:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                
                
				for (i=0; i<l_qty; i++)
				{
					fread (&t.u, sizeof (float), 1, l_file);
                    fread (&t.v, sizeof (float), 1, l_file);

                    s.texxes.push_back(t);
				}
                break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
			default:
				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        } 
	}
	fclose (l_file); // Closes the file stream

    return NO_ERROR;
}