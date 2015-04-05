// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************

#include "stdafx.h"
#include "stdio.h"
#include "baselib.h"
#include "baseutils.h"
#include "basecont.h"
#include "PqmapLoader.h"
#include <algorithm>

//----------------------------------------------------------------------------------
bool Parser::_doubleopen = false;
//----------------------------------------------------------------------------------
const REAL	__epsilon = 1e-5;	

//---------------------------------------------------------------------------------------
struct QLevel
{
    vvector<QEntity*>   _entities;
    vvector<string>     _texnames;

    // adds a unique tex and returns the index into this array
    int  AddTex(const char* n){
        vvector<string>::iterator fi  = find(_texnames.begin(), _texnames.end(), n);
        if(fi != _texnames.end())
            return fi-_texnames.begin();
        _texnames<<n;
        return _texnames.size()-1;
    }
        

}__QLevel;


//---------------------------------------------------------------------------------------
// this may not be properly handeled. See UT calc tex coord how is done
void CalcTexCoord(QPoly& qp, UV* shift)
{
    UV			        maxuv[2];
    Box                 box ;
    vvector<UV>&        uvs    = qp._uvs;

    FOREACH(vvector<V3>, qp._vxes, pvx){
        box.AddPoint(*pvx);
    }
    box.FlatIt(maxuv);

    for(int i=0; i < qp._vxes.size() ;i++)
    {
        V3& v  = qp._vxes[i];
        UV t;

        switch(box.GetMinimAx())
        {
            case 'x':   //z,y
                t.v = (v.y-maxuv[0].v) / (maxuv[1].v-maxuv[0].v);
                t.u = (v.z-maxuv[0].u) / (maxuv[1].u-maxuv[0].u);
                break;
            case 'y':   //x,z
                t.u = (v.x-maxuv[0].u) / (maxuv[1].u-maxuv[0].u);
                t.v = (v.z-maxuv[0].v) / (maxuv[1].v-maxuv[0].v);
                break;
            case 'z':   //x,y
                t.u = (v.x-maxuv[0].u) / (maxuv[1].u-maxuv[0].u);
                t.v = (v.y-maxuv[0].v) / (maxuv[1].v-maxuv[0].v);
                break;
        }
		t.v +=shift->u;
		t.u +=shift->v;
        qp._uvs << t;
    }
}

//---------------------------------------------------------------------------------------
// splits by plane all 'p' and populates a and b
void    Split(Plane& plane, vvector<V3>& p, vvector<V3>& a, vvector<V3>& b)
{
	V3  iv;
    V3 itxB = *p.begin();
    V3 itxA = p.back();

	REAL    fB;
    REAL	fA = plane.DistTo(itxA);
    
    FOREACH(vvector<V3>, p, vxI)
    {
        itxB = *vxI;
        fB   = plane.DistTo(itxB);
        if(fB > 0.01)
        {
            if(fA < -0.01f)
            {
                REAL   t = -fA /(fB - fA);
				iv.interpolate(itxA,itxB,t);
                a << iv;
                b << iv;
            }
            a<<itxB;
        }
        else if(fB < -0.01f)
        {
            if(fA > 0.01f)
            {
                REAL t = -fA /(fB - fA);           // t of segment
				iv.interpolate(itxA,itxB,t);
                a << iv;
                b << iv;
            }
            b <<itxB;
        }
		else
		{
			a << itxB;
            b << itxB;

		}
        itxA = itxB;
        fA   = fB;
    }
}

//---------------------------------------------------------------------------------------
// from plane p builds a huge polygon 'qp'
void FromPlane(const Plane& p, QPoly& qp, V3* center)
{
    // get to see what wax we are using for rotation cp
	V3		ax = GetMinAx((V3&)p._n);  
    ax.norm();

    // to corner vectors;
	V3		uDr  = Vcp(ax,  p._n);		   
	V3		vDr  = Vcp(uDr, p._n);

    uDr		*= 36635.0;
	vDr		*= 36635.0;
    REAL vdp = Vdp(p._n, ax);
    V3&  c = *center;

    qp._vxes <<  c+(uDr-vDr);
	qp._vxes <<  c+(uDr+vDr);
	qp._vxes <<  c-(uDr-vDr);	
	qp._vxes <<  c-(uDr+vDr);
}

//---------------------------------------------------------------------------------------
// from all brush planes build all brush polygons
void  FromPlanes(vvector<Plane*>& planes,
                 vvector<V3*>&    centers,
                 vvector<UV*>     shifts,  
                 vvector<int>     texIdxes,
                 BOOL             front, 
                 vvector<QPoly>&  outPolys)
{
    int jidx = 0;
    FOREACH(vvector<Plane*>, planes, pplane)
    {
        QPoly qp;
        
        FromPlane(*(*pplane), qp, centers[jidx]);
        qp._texIdx = texIdxes[jidx];

        // see if builded polygon plane faces as original 
        // polygon which was build from
        {
            Plane pOfPoly;
            pOfPoly.CalcNormal(qp._vxes[0], qp._vxes[1], qp._vxes[2]);
            if(IsZero(Vdp(pOfPoly._n, (*pplane)->_n)), __epsilon)// poly has to be reversed
            {
                qp._vxes.reverse();
            }
        }

        // cut it with all other polygons
        FOREACH(vvector<Plane*>, planes, pplane2)
        {
            if(pplane2 == pplane)
                continue;
            vvector<V3> a;
            vvector<V3> b;

            Split(**pplane2, qp._vxes, a, b);  
            qp._vxes.clear();

            if(front)
            {
                qp._vxes = a;
            }
            else
            {
                qp._vxes = b;
            }
        }
        if(qp._vxes.size() >= 3)
        {
            // calc if normals are the same
            CalcTexCoord(qp, shifts[jidx]);
            outPolys << qp;
        }
        ++jidx;
    }
}

//---------------------------------------------------------------------------------------
void GatherBrushes(vvector<QBrush*>& allBrushes, vvector<QEntity*>&  allEntities)
{
    FOREACH(vvector<QEntity*>, __QLevel._entities, ppEntity)
    {
        if((*ppEntity)->_classname== "\"worldspawn\"")
        {
            FOREACH(vvector<QBrush*>, (*ppEntity)->_brushes, ppBrush)
            {
                allBrushes << (*ppBrush);
            }
        }

        if((*ppEntity)->_classname== "\"light\"")
        {
            allEntities << *ppEntity;
        }
    }
}

//---------------------------------------------------------------------------------------
long PqmapLoader::PostProcess(Plg_Scene** pScene)
{
    vvector<QBrush*>    allBrushes;
    vvector<QEntity*>   allEntities;

    GatherBrushes(allBrushes, allEntities);

    // allocate space for scene and for all brushes
    *pScene = new Plg_Scene;
	::memset(*pScene, 0, sizeof(Plg_Scene));
    (*pScene)->nBrushes = allBrushes.size();
    (*pScene)->pBrushes = new Plg_Brush[allBrushes.size()];

    (*pScene)->nTextures = __QLevel._texnames.size();
    (*pScene)->pTextures = new Plg_Texture[__QLevel._texnames.size()];

    // store textures here
    for(int t = 0; t < __QLevel._texnames.size(); t++)
    {
        (*pScene)->pTextures[t].target = 0; // GL_TEXTURE2D
        ::strcpy((*pScene)->pTextures[t].filename, __QLevel._texnames[t].c_str());
    }

    if(allEntities.size())
    {

        (*pScene)->nItems = allEntities.size();
        (*pScene)->pItems = new Plg_Item[(*pScene)->nItems];
        
        int index = 0;
        FOREACH(vvector<QEntity*>,  allEntities, ppEntity)
        {
            Plg_Item& itm = (*pScene)->pItems[index];
            
            

            itm.classType=CLASS_IS_LIGHT;
            strcpy(itm.classname, (*ppEntity)->_classname.c_str());
            itm.position = (*ppEntity)->_pos;

            itm._U._Light.radius     = (REAL)(*ppEntity)->_intensity * 15.0;
            itm._U._Light.intensity  = (*ppEntity)->_intensity ;
            itm._U._Light.color[0]   = 255;
            itm._U._Light.color[1]   = 255;
            itm._U._Light.color[2]   = 255;
            
            ++index;
        }
    }

    // for each q-brush
    int indexBr = 0;
    FOREACH(vvector<QBrush*>,  allBrushes, ppBrush)
    {
        vvector<Plane*>     planes;  
        vvector<QPoly>      polys;
        vvector<V3*>        centers;  
        vvector<UV*>        shifts;  
        vvector<int>        texIdxes;  

        Plg_Brush&          plgBrush =  (*pScene)->pBrushes[indexBr];
        QBrush*             pBrush   = *ppBrush;
        int                 indexp   = 0;
        
        //
        // for each qface line collect planes 
        //
        FOREACH(vvector<QBrushFace*>,  pBrush->_pbrshlines, ppBrLine)
        {
            QBrushFace* pLine = *ppBrLine;
            planes   << &pLine->_plane;
            centers  << &pLine->_center;
            shifts   << &pLine->_shft;
            texIdxes <<  pLine->_indexTex;
        }

        //
        // all brush faces, get out brush polygons
        //
        FromPlanes(planes, centers, shifts, texIdxes, 0, polys);   

        //
        // load them in the plgBrush
        //
        if(polys.size() > 4)
        {
            plgBrush.nPolys = polys.size();
            plgBrush.pPolys = new Plg_Poly[polys.size()];
            // alloc vertexes space        
            FOREACH(vvector<QPoly>, polys, ppPoly)
            {
                plgBrush.pPolys[indexp].nvXes = ppPoly->_vxes.size();
                plgBrush.pPolys[indexp].vXes  = new Vtx[ppPoly->_vxes.size()];
                ++indexp;
            }

            //
            // store info in plg brush
            //
            plgBrush.flags = pBrush->_pParent->_flags;
			plgBrush.name[0] = '\0';

            indexp = 0;
            FOREACH(vvector<QPoly>, polys, ppPoly)
            {
                plgBrush.pPolys[indexp].texIdx[2]   = -1;
                plgBrush.pPolys[indexp].texIdx[1]   = -1;
                plgBrush.pPolys[indexp].texIdx[0]   = ppPoly->_texIdx; // index in texture previously saved
                
                plgBrush.pPolys[indexp].texapply[0] = 0;
                plgBrush.pPolys[indexp].texapply[1] = 0;
                plgBrush.pPolys[indexp].texapply[2] = 0;

                plgBrush.pPolys[indexp].combine     = 0x1; // one texture has been set
                plgBrush.pPolys[indexp].flags       = 0;
                
                int idxv = 0;
                FOREACH(vvector<V3>, ppPoly->_vxes, pVx)
                {
                    plgBrush.pPolys[indexp].vXes[idxv]._xyz   = *pVx;
                    plgBrush.pPolys[indexp].vXes[idxv]._uv[0] = ppPoly->_uvs[idxv];
                    ++idxv;
                }
                ++indexp;
            }
        }
        ++indexBr;
    }


    return 0;
}

//---------------------------------------------------------------------------------------
long PqmapLoader::GetVersion()
{
    return 1;
}

//---------------------------------------------------------------------------------------
static BOOL GetCloseUpPoly(Plg_Poly* pPolys, int count , Plg_Poly& p, V3& vx)
{
    // find adiacent polygon which 
    return FALSE;
}

//---------------------------------------------------------------------------------------
long PqmapLoader::ExportFile(IGeticEditor* pe, char* installDir, char* bsFileName, const Plg_Scene* pScene)
{
    int         bn = 0;
    FileWrap    fw;
    char        texname[64];
    char        cd[256];

    _getcwd(cd,256);


    string mapname = bsFileName;

    int idx = mapname.find(".");
    if(idx !=-1)
    {
        mapname = mapname.substr(0,idx);
        PathHandler        ph(mapname.c_str());
        _mapname = ph.JustFile();
    }

    mkdir(_mapname.c_str());


    if(!fw.Open(bsFileName, "wb"))
        return -1;
        
    // expand textures in a readable variable
    Plg_Texture* ptrTotextures = pScene->pTextures;
    V3  center;
    
    fprintf(fw._pf, "{\r\n");
    fprintf(fw._pf, "\"message\" \"%s\"\r\n", "scene");
    fprintf(fw._pf,"\"classname\" \"worldspawn\"\r\n");
    fprintf(fw._pf,"\"_color\" \"1 1 1\"\r\n"); // poly caries the color
    for(int i=0; i< pScene->nBrushes;i++)
    {
        Plg_Brush& b = pScene->pBrushes[i];

        if(b.flags & BRSH_CUT)

        for(int j=0; j< b.nPolys;j++)
        {
            Plg_Poly& p = b.pPolys[j];
            for(int x=0;x<p.nvXes;x++)
                p.vXes[x]._xyz/=2;
        }
	    for(int j=0; j< b.nPolys;j++)
	    {
            fprintf(fw._pf,"// brush %d\r\n", ++bn); // write a negative value for cut brush
            fprintf(fw._pf, "{\r\n");

            Plg_Poly& p = b.pPolys[j];

            
//            if(!GetWindingVertex(b.pPolys, b.nPolys, p, center))
            {
                center.reset();
                REAL exback = 8;
                
                
                for(int x=0; x < p.nvXes; x++)
                {
                    center+=p.vXes[x]._xyz;
                }
                center/=p.nvXes;
                
                

                Plane plan(p.vXes[0]._xyz, p.vXes[1]._xyz, p.vXes[2]._xyz);
                center += plan._n * (exback);
            }


            // put first 3 points of the poly. 
            // scale world by 4
            
            if(p.texIdx[0] < pScene->nTextures)
            {
                ::sprintf(texname,"%s/%s",_mapname.c_str(),pScene->pTextures[p.texIdx[0]].filename);

                
                CopyFile(MKSTR("%s/ged_textures/%s", installDir, pScene->pTextures[p.texIdx[0]].filename), texname,0);
            }
            else
            {
                ::strcpy(texname, "NULL");
            }
            
            fprintf(fw._pf, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s %f %f %f %f %f %f %f %f\r\n", //// %f %f
                            p.vXes[0]._xyz.x, p.vXes[0]._xyz.z, p.vXes[0]._xyz.y,
                            p.vXes[2]._xyz.x, p.vXes[2]._xyz.z, p.vXes[2]._xyz.y,
                            p.vXes[1]._xyz.x, p.vXes[1]._xyz.z, p.vXes[1]._xyz.y,
                            texname,
                            p.texax[0].x, p.texax[0].y, p.texax[0].z, p.texShift.u,
                            p.texax[1].x, p.texax[1].y, p.texax[1].z, p.texShift.v);                 //texScale._u, texScale._v)

            V3 a,b,c;
            for(int x=0;x<p.nvXes;x++)
            {
                a = p.vXes[x]._xyz;
                b = p.vXes[(x+1)%p.nvXes]._xyz;
                c = center;
                


                fprintf(fw._pf, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s %f %f %f %f %f %f %f %f\r\n", //// %f %f
                            a.x, a.z, a.y,
                            b.x, b.z, b.y,
                            c.x, c.z, c.y,
                            texname,
                            p.texax[0].x, p.texax[0].y, p.texax[0].z, p.texShift.u,
                            p.texax[1].x, p.texax[1].y, p.texax[1].z, p.texShift.v);                       //texScale._u, texScale._v)

            }
 



            fprintf(fw._pf, "}\r\n");

	    }
        
    }
    fprintf(fw._pf, "}\r\n");
    return 0;
}

//---------------------------------------------------------------------------------------
long PqmapLoader::GetMenuStringAndType(char* bsFileName, DWORD* type)
{
    //large enough
    _tcscpy(bsFileName,"Quake map files,map");
    *type = PLUG_IMPORTER|PLUG_EXPORTER;
    return 0;
}


//---------------------------------------------------------------------------------------
long PqmapLoader::ImportFile(IGeticEditor* pe, char* installDir, char* bsFileName, Plg_Scene** pScene)
{
    FileWrap fw;

    if(!fw.Open(bsFileName, "rb"))
        return -1;

    Parser  parser(&fw);
    while(parser.Step()>=0);
    fw.Close();

    return PostProcess(pScene);
}

//---------------------------------------------------------------------------------------
long PqmapLoader::ReleaseScene(Plg_Scene* pScene)
{
    //RELEASE_SCENE(pScene);
    return NO_ERROR;
}

//---------------------------------------------------------------------------------------
int Parser::Parse(vvector<string>& tokens)
{
    map<string, PfHandler>::iterator fi = _classes.find(tokens[0]);
    if(fi != _classes.end())
    {
        return (this->*_classes[tokens[0]])(tokens);
    }
    return 1;
}

//---------------------------------------------------------------------------------------
int Parser::BaseParse(const char* pline)
{
    vvector<string> tokens;
    Explode(pline, tokens,' ');
    if(tokens.size())
    {
        TRACEX(pline);
        TRACEX("\r\n");
        append(_tokenspersect, tokens);
        return Parse(tokens);
    }
    return -1;
}

//---------------------------------------------------------------------------------------
void Parser::Explode(const char* pline, vvector<string>& tokens, char token)
{
    char localline[256]= {0};

    ::strcpy(localline, pline);
    char*   pLocal = localline; 
    char    pTok[2]= {token,'\0'};
    char*   pSubStr;
    while(pSubStr = strtok(pLocal, pTok))
    {
        pLocal = 0;
        tokens << pSubStr; 
    }
}

//---------------------------------------------------------------------------------------
int Parser::Handle_Open(vvector<string>& tokens)
{
    _tokenspersect.clear();
    _pEntity = new QEntity(_pfw);
    TRACEX("New Entity\r\n");
    while(_pEntity->Step()>0);
    return 1;
}

//---------------------------------------------------------------------------------------
int Parser::Step()
{
    bool bok = false;
    char line[255]={0};
    char* pline = line;
    char* pend;  //= pline+strlen(line);
    do{
        pline = line;
        _pfw->ReadLine(line, 255);
        if(feof(_pfw->_pf))
            return -1;
        // remove triling crlf
        pend = pline+strlen(line)-1;
        while(*pend=='\n'||*pend=='\r')
            --pend;
        *(pend+1)='\0';
        // remove start spaces
        while(*pline==' '||*pline=='\t')
            ++pline;
        if(T()==_BRUSH)
        {
            bok =  !(pline[0]=='(' || pline[0]==')' || pline[0]=='}');
            if(pline[0]=='{')
                _doubleopen                 = true;
            if(pline[0]=='}')
            {
                if(_doubleopen)
                {
                    _doubleopen=false;
                    bok=true;
                }
                else
                    bok=false;

            }
        }
    }while(bok);

    if(strlen(pline))
        return BaseParse(pline);
    return 0;
}

//---------------------------------------------------------------------------------------
int    QEntity::Handle_Open(vvector<string>& tokens)
{
    if(_classname== "\"worldspawn\"")
    {
        TRACEX("New Brush\r\n");
        QBrush* pBrush = new QBrush(_pfw, this);
        while(pBrush->Step() > 0)
		{
			;
		}
    }
    
    return 1;
}

int QEntity::Handle_Light(vvector<string>& tokens)
{
    if(tokens.size()>=2)
    {
        char* pc = (char*)tokens[1].c_str();
        if(*pc=='\"')pc++;
        _intensity = atoi(pc);
    }
    return 1;
}

//---------------------------------------------------------------------------------------
int QEntity::Handle_KeyVal(vvector<string>& tokens)
{
    if(tokens.size() == 0)
        return 1;

    string values;
    for(int i=1; i<tokens.size(); ++i)
    {
        values += tokens[i];
        values += " ";
    }
    if(values.length())
        _keyvals[tokens[0]] = values;
    return 1;
}

//---------------------------------------------------------------------------------------
int    QEntity::Handle_Comment(vvector<string>& tokens)
{
    string values;
    for(int i=1; i<tokens.size(); ++i)
    {
        values += tokens[i];
        values += " ";
    }
    if(values.length())
        _keyvals[tokens[0]] = values;

    return 1;
}

//---------------------------------------------------------------------------------------
int    QEntity::Handle_Close(vvector<string>& tokens)
{
    if(_classname== "\"worldspawn\"")
    {
        TRACEX("Store entity brush in scene\r\n");
        this->_flags = BRSH_SOLID;
        __QLevel._entities << this;
    }
    else 
    {
        TRACEX("Store entity in scene\r\n");
        __QLevel._entities << this;
    }
    return 0;
}

//---------------------------------------------------------------------------------------
int QBrush::Handle_Line(vvector<string>& tokens)
{
    QBrushFace* pLine = new QBrushFace(_pfw, this);
    pLine->Handle_Line(tokens);
    if(pLine->_OK)
        _pbrshlines << pLine;
    return 1;
}

//---------------------------------------------------------------------------------------
int QBrush::Handle_Close(vvector<string>& tokens)
{
    if(this->_pbrshlines.size() &&_pParent->_classname=="\"worldspawn\"")
    {
        TRACEX("store Qbrush in entity");
        _pParent->_brushes << this;
    }
    return 0;    // end brush
}


//---------------------------------------------------------------------------------------
int QBrushFace::Handle_Line(vvector<string>& tokens)
{
    float   a,b,c,d,e,f,g,h,i;
    float   u0=0,u1=0,u2=0,u3=0,v0=0,v1=0,v2=0,v3=0;
    char    name[32];
    char    originalLine[255]={0};

    if(_pParent->_pParent->_classname != "\"worldspawn\"")
        return 1;
    
    FOREACH(vvector<string>, tokens, pstring)
    {
        ::strcat(originalLine, pstring->c_str());
        ::strcat(originalLine, " ");
    }

    int spacecount = tokens.size();
    
    if(spacecount == 28) // new file
    {
        //( 1744 656 112 ) ( 1872 656 112 ) ( 1744 656 240 ) ( ( -0.00391 0.00000 -6.81640 ) ( 0.00000 0.00391 -0.56196 ) ) base_wall/basewall03
        ::sscanf(originalLine, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) ( ( %f %f %f ) ( %f %f %f ) ) %s" , 
                                 &a, &b,&c,    &d,&e,&f,    &g,&h,&i,     &u0,&u1,&u2, &v0, &v1, &v2, name);
    }
    else
    if(spacecount > 22) // new file
    {
        //( 93.750000 -91.500000 65.250000 ) ( -93.750000 91.500000 65.250000 ) ( 93.750000 91.500000 65.250000 ) qqq/bankmarble_offsq.jpg 0.000000 0.000000 1.000000 1.000000 1.000000 0.000000 0.000000 1.000000

        ::sscanf(originalLine, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s %f %f %f %f %f %f %f %f" , &a,&b,&c,&d,&e,&f,&g,&h,&i, name, &u0, &u1, &u2, &u3, &v0, &v1, &v2, &v3);
    }
    else                // old file
    {
        ::sscanf(originalLine, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s %f %f %f" , &a,&b,&c,&d,&e,&f,&g,&h,&i, name, &u0, &u1, &u2);
    }

    // scale the world by 4
    V3  v11(a,c,b);
    V3  v22(d,f,e);
    V3  v33(g,i,h);

    v11*=3.0f; v22*=3.0f; v33*=3.0f;

    _center = V3(0,0,0);
    _center += v11;
    _center += v22;
    _center += v33;
    _center /= 3.0f;    // from wich we spawn the polygon

    _plane.CalcNormal (v11,v22,v33);

    // see if exitent plane is coplanar to already existent plane
    FOREACH(vvector<QBrushFace*>,  _pParent->_pbrshlines, ppBrLine)
    {
        QBrushFace* pLine = *ppBrLine;
        if(_plane == pLine->_plane)
        {
            return 1;
        }
    }
    _OK = TRUE;
    
    _indexTex = __QLevel.AddTex(name);
    _ax[0]  = V3(u0, u1, u2);
    _shft.u = u3;
    
    _ax[1]  = V3(v0, v1, v2);
    _shft.v = v3;
    
    return 1;
}




