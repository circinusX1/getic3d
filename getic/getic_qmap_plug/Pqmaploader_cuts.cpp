// PqmapLoader.cpp: implementation of the PqmapLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdio.h"
#include "..\\_include\\BspFIleStr.h"
#include "baselib.h"
#include "basecont.h"
#include "PqmapLoader.h"


//---------------------------------------------------------------------------------------
void CalcTexCoord(QPoly& qp)
{
    UV			maxuv[2];
    Box         box ;

    vvector<UV>& uvs = qp._uvs;
    FOREACH(vvector<V3>, qp._vxes, pvx)
    {
        /*
        assert(pvx->x < INFINIT);
        assert(pvx->y < INFINIT);
        assert(pvx->z < INFINIT);

        assert(pvx->x > -INFINIT);
        assert(pvx->y > -INFINIT);
        assert(pvx->z > -INFINIT);
        */

        box.AddPoint(*pvx);
    }
    box.FlatIt(maxuv);

    for(int i=0; i < qp._vxes.size() ;i++)
    {
        V3& v   = qp._vxes[i];
        UV t;//   = 

        switch(box.GetMinimAx())
        {
            case 'x':   //z,y
                t.v = (v.y-maxuv[0].v) / (maxuv[1].v-maxuv[0].v);
                t.u = (v.z-maxuv[0].u) / (maxuv[1].u-maxuv[0].u);
/*
				t.v*=_texcoord.scaley;
				t.u*=_texcoord.scalex;

				t.v+=_texcoord.shifty;
				t.u+=_texcoord.shiftx;
*/
                break;
            case 'y':   //x,z
                t.u = (v.x-maxuv[0].u) / (maxuv[1].u-maxuv[0].u);
                t.v = (v.z-maxuv[0].v) / (maxuv[1].v-maxuv[0].v);
/*
				t.u*=_texcoord.scalex;
				t.v*=_texcoord.scaley;

				t.u+=_texcoord.shiftx;
				t.v+=_texcoord.shifty;
*/
                break;
            case 'z':   //x,y
                t.u = (v.x-maxuv[0].u) / (maxuv[1].u-maxuv[0].u);
                t.v = (v.y-maxuv[0].v) / (maxuv[1].v-maxuv[0].v);
/*
				t.u*=_texcoord.scalex;
				t.v*=_texcoord.scaley;

				t.u+=_texcoord.shiftx;
				t.v+=_texcoord.shifty;
*/
                break;
        }
        qp._uvs << t;
    }
}

//---------------------------------------------------------------------------------------
void    Split(Plane& plane, vvector<V3>& p, vvector<V3>& a, vvector<V3>& b)
{

	V3  iv;
    V3 itxB = *p.begin();
    V3 itxA = p.back();

	REAL    fB;
    REAL	fA = plane.DistTo(itxA);
	//REAL	fA = plane.GetSide(itxA._xyz);
    
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
void FromPlane(const Plane& p, QPoly& qp, V3* center)
{
	V3		ax = GetMajorAxes((V3&)p._n);  // get to see what wax we are using for rotation cp
    ax.norm();

    // build a polygon from plane
	V3		uDr  = Vcp(ax,  p._n);		   // to corner vectors;
	V3		vDr  = Vcp(uDr, p._n);

    uDr		*= 36635.0;
	vDr		*= 36635.0;
    REAL vdp = Vdp(p._n, ax);
    V3&  c = *center;
	qp._vxes <<  c+(uDr-vDr);
	qp._vxes <<  c+(uDr+vDr);
	qp._vxes <<  c-(uDr-vDr);					// switch this to be able to rotate it
	qp._vxes <<  c-(uDr+vDr);
}

//---------------------------------------------------------------------------------------
void  FromPlanes(vvector<Plane*>& planes,
                 vvector<V3*>& centers,
                 BOOL front, vvector<QPoly>& outPolys)
{
    int jidx = 0;
    FOREACH(vvector<Plane*>, planes, pplane)
    {
        QPoly qp;
        
        FromPlane(**pplane, qp, centers[jidx++]);

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
        //assert(qp._vxes.size() >= 3);
        if(qp._vxes.size() >= 3)
        {
            CalcTexCoord(qp);
            outPolys << (qp);
        }
    }
}

//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
struct QLevel
{

    vvector<QEntity*>   _entities;
    vvector<string>     _texNames;
}__QLevel;

//---------------------------------------------------------------------------------------
long PqmapLoader::PostProcess(Plg_Scene** pScene)
{
    vvector<QBrush*>    allBrushes;

    // collect all brushes
    FOREACH(vvector<QEntity*>, __QLevel._entities, ppEntity)
    {
        if((*ppEntity)->_classname== "\"worldspawn\"")
        {
            FOREACH(vvector<QBrush*>, (*ppEntity)->_brushes, ppBrush)
            {
                allBrushes << (*ppBrush);
            }
        }
    }

    // allocate space for scene and for all brushes
    *pScene = new Plg_Scene;
    (*pScene)->nBrushes = allBrushes.size();
    (*pScene)->pBrushes = new Plg_Brush[allBrushes.size()];
    (*pScene)->pTextures = 0;

    // for each q-brush
    int indexBr = 0;
    FOREACH(vvector<QBrush*>,  allBrushes, ppBrush)
    {
        vvector<Plane*>     faces;  
        vvector<V3*>        centers;  
        Plg_Brush&          plgBrush =  (*pScene)->pBrushes[indexBr];
        QBrush*             pBrush   = *ppBrush;
        
        // for each qface line collect planes 
        FOREACH(vvector<QBrushFace*>,  pBrush->_pbrshlines, ppBrLine)
        {
            QBrushFace* pLine = *ppBrLine;
            faces << &pLine->_plane;
            centers << &pLine->_center;
        }

        // all brush faces, get out brush polygons
        vvector<QPoly>          polys;
        FromPlanes(faces, centers, 0, polys);   

        // load them in the plgBrush
        // alloc poly pointer
        plgBrush.nPolys = polys.size();
        plgBrush.pPolys = new Plg_Poly[polys.size()];

        // first step allocate space for vertexes
        int indexp=0;
        FOREACH(vvector<QPoly>, polys, ppPoly)
        {

            plgBrush.pPolys[indexp].nvXes = ppPoly->_vxes.size();
            plgBrush.pPolys[indexp].vXes  = new Vtx[ppPoly->_vxes.size()];
            ++indexp;
        }

        // second one p
        indexp=0;
        _FOREACH(vvector<QPoly>, polys, ppPoly)
        {
            int idxv = 0;
            FOREACH(vvector<V3>, ppPoly->_vxes, pVx)
            {
                plgBrush.pPolys[indexp].vXes[idxv]._xyz   = *pVx;

                plgBrush.pPolys[indexp].vXes[idxv]._uv[0] = ppPoly->_uvs[idxv];
                ++idxv;
            }
            ++indexp;
        }
        
        ++indexBr;
    }
    return 0;
}

//---------------------------------------------------------------------------------------
long PqmapLoader::ExportFile(TCHAR* bsFileName, const Plg_Scene* pBrush)
{
    return -1;
}

//---------------------------------------------------------------------------------------
long PqmapLoader::GetMenuStringAndType(TCHAR* bsFileName, DWORD* type)
{
    //large enough
    _tcscpy(bsFileName,"map files,map");
    *type = PLUG_IMPORTER;
    return 0;
}


//---------------------------------------------------------------------------------------
long PqmapLoader::ImportFile(TCHAR* bsFileName, Plg_Scene** pScene)
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
   
    if(pScene)
    {
        if(pScene->pTextures)
            delete[] pScene->pTextures;

		for(int i=0; i< pScene->nBrushes;i++)
		{
            Plg_Brush& b = pScene->pBrushes[i];
			for(int j=0; j< b.nPolys;j++)
			{
                Plg_Poly& p = b.pPolys[j];
                delete[] p.vXes;
			}
			delete[]b.pPolys;
		}
        delete[] pScene->pBrushes;
        delete pScene;
    }
    return 0;
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
    _pEntity = new QEntity(_pfw);
    TRACEX("New Entity\r\n");
    while(_pEntity->Step()>0);
    return 1;
}

//---------------------------------------------------------------------------------------
int Parser::Step()
{
    char line[255]={0};
    char* pline = line;
    char* pend;  //= pline+strlen(line);

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
    return BaseParse(pline);
}

//---------------------------------------------------------------------------------------
int    QEntity::Handle_Open(vvector<string>& tokens)
{
    // handle only brushes
    if(_classname== "\"worldspawn\"")
    {
        TRACEX("New Brush\r\n");
        QBrush* pBrush = new QBrush(_pfw, this);
        while(pBrush->Step()>0)
        {
            ;
        }
    }
    return -1;
}

//---------------------------------------------------------------------------------------
int    QEntity::Handle_Close(vvector<string>& tokens)
{
    if(_classname== "\"worldspawn\"")
    {
        TRACEX("Store entity in scene\r\n");
        __QLevel._entities << this;
    }
    return -1;
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
int QBrush::Handle_Open(vvector<string>& tokens)
{
    return _pParent->Handle_Open(tokens);
}

//---------------------------------------------------------------------------------------
int QBrush::Handle_Close(vvector<string>& tokens)
{
    TRACEX("store Qbrush in entity");
    _pParent->_brushes << this;
    return 0;       // end brush
}


//---------------------------------------------------------------------------------------
int QBrushFace::Handle_Line(vvector<string>& tokens)
{
    int     a,b,c,d,e,f,g,h,i;
    float   u0=0,u1=0,u2=0,u3=0,v0=0,v1=0,v2=0,v3=0;
    char    name[32];
    char    originalLine[255]={0};

    FOREACH(vvector<string>, tokens, pstring)
    {
        ::strcat(originalLine, pstring->c_str());
        ::strcat(originalLine, " ");
    }

    int spacecount = tokens.size();
    if(spacecount > 22)
    {
        ::sscanf(originalLine, "( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s %f %f %f %f %f %f %f %f" , &a,&b,&c,&d,&e,&f,&g,&h,&i, name, &u0, &u1, &u2, &u3, &v0, &v1, &v2, &v3);
    }
    else
    {
        ::sscanf(originalLine, "( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s %f %f %f" , &a,&b,&c,&d,&e,&f,&g,&h,&i, name, &u0, &u1, &u2);
    }

    V3  v11(a,c,b);
    V3  v22(d,f,e);
    V3  v33(g,i,h);

    v11*=6.0;
    v22*=6.0;
    v33*=6.0;

    _center = v11;
    _center += v22;
    _center += v33;
    _center /= 3.0f;

    _plane.CalcNormal (v11,v22,v33);

    FOREACH(vvector<QBrushFace*>,  _pParent->_pbrshlines, ppBrLine)
    {
        QBrushFace* pLine = *ppBrLine;
        if(_plane == pLine->_plane)
        {
            return 0;
        }
    }
    _OK = TRUE;
    ::strcpy(_texname, name);
    _u[0] = u0;
    _u[1] = u1;
    _u[2] = u2;
    _u[3] = u3;

    _v[0] = v0;
    _v[1] = v1;
    _v[2] = v2;
    _v[3] = v3;

    return 1;
}


