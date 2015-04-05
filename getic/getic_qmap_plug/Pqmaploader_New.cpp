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
#include "PqmapLoader.h"
#include <algorithm>

//----------------------------------------------------------------------------------
const REAL	__epsilon = 1e-5;	
#define     HAS_FRONTS  0x1
#define     HAS_BACKS   0x2
#define     HAS_BOTH    0x3

//---------------------------------------------------------------------------------------
struct QLevel
{
    vvector<QEntity*>   _entities;          // quake level entities
    vvector<string>     _texnames;          // texture names

    // adds a unique tex and returns the index into this array. polygons hold to this index
    int  AddTex(const char* n){
        vvector<string>::iterator fi  = find(_texnames.begin(), _texnames.end(), n);
        if(fi != _texnames.end())
            return fi-_texnames.begin();
        _texnames<<n;
        return _texnames.size()-1;
    }
}__QLevel;


//---------------------------------------------------------------------------------------
// this may not be properly handeled. See UT calc tex coord how is done or search for 
// quaketools and take a look.
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
// Splits by plane all 'p' and populates a and b
void    Split(Plane& plane, vvector<V3>& p, vvector<V3>& a, vvector<V3>& b)
{
	V3      iv;
    V3      itxB = *p.begin();
    V3      itxA = p.back();
	REAL    fB;
    REAL	fA   = plane.DistTo(itxA);
    
    FOREACH(vvector<V3>, p, vxI){
        itxB = *vxI;
        fB   = plane.DistTo(itxB);
        if(fB > __epsilon){
            if(fA < -__epsilon){
                REAL   t = -fA /(fB - fA);
				iv.interpolate(itxA,itxB,t);
                a << iv;
                b << iv;
            }
            a<<itxB;
        }
        else if(fB < -__epsilon){
            if(fA > __epsilon){
                REAL t = -fA /(fB - fA);          
				iv.interpolate(itxA,itxB,t);
                a << iv;
                b << iv;
            }
            b <<itxB;
        }
		else{
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
	V3		ax = GetMajorAxes((V3&)p._n);  
    ax.norm();

    // to corner vectors;
	V3		uDr  = Vcp(ax,  p._n);		   
	V3		vDr  = Vcp(uDr, p._n);

    uDr		*= 32768.0;             // huge one ()
	vDr		*= 32768.0;
    REAL vdp = Vdp(p._n, ax);
    V3&  c   = *center;

    qp._vxes <<  c+(uDr-vDr);       // build corner points
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

    FOREACH(vvector<Plane*>, planes, pplane){

        QPoly qp;                                       // polygon we build

        FromPlane(*(*pplane), qp, centers[jidx]);
        qp._texIdx = texIdxes[jidx];                    // find tex index
        // see if the polygon plane faces as original plane
        do{
            Plane pOfPoly;
            pOfPoly.CalcNormal(qp._vxes[0], qp._vxes[1], qp._vxes[2]);
            if(IsZero(Vdp(pOfPoly._n, (*pplane)->_n)), __epsilon)
            {
                qp._vxes.reverse();
            }
        }while(0);

        // cut it with all other polygons excluding this one
        FOREACH(vvector<Plane*>, planes, pplane2){
            if(pplane2 == pplane)
                continue;
            vvector<V3> a;
            vvector<V3> b;
            Split(**pplane2, qp._vxes, a, b);  
            qp._vxes.clear();

            if(front){
                qp._vxes = a;
            }
            else{
                qp._vxes = b;
            }
        }

        if(qp._vxes.size() >= 3){                   // should have minim 3 vertexes
            CalcTexCoord(qp, shifts[jidx]);
            outPolys << qp;
        }
        ++jidx;
    }
}

//---------------------------------------------------------------------------------------
void GatherBrushes(vvector<QBrush*>& allBrushes)
{
    FOREACH(vvector<QEntity*>, __QLevel._entities, ppEntity){
        if((*ppEntity)->_classname== "\"worldspawn\""){
            FOREACH(vvector<QBrush*>, (*ppEntity)->_brushes, ppBrush){
                allBrushes << (*ppBrush);
            }
        }
    }
}

//---------------------------------------------------------------------------------------
long PqmapLoader::PostProcess(Plg_Scene** pScene)
{
    vvector<QBrush*>    allBrushes;

    GatherBrushes(allBrushes);

    // allocate space for scene and for all brushes
    *pScene = new Plg_Scene;
	::memset(*pScene, 0, sizeof(Plg_Scene));        // DO NOT FORGET TO SET ALL SCENE MEMBERS TO NULL
    (*pScene)->nBrushes  = allBrushes.size();
    (*pScene)->pBrushes  = new Plg_Brush[allBrushes.size()];
    (*pScene)->nTextures = __QLevel._texnames.size();
    (*pScene)->pTextures = new Plg_Texture[__QLevel._texnames.size()];

    // store textures 
    for(int t = 0; t < __QLevel._texnames.size(); t++){
        (*pScene)->pTextures[t].target = 0; // GL_TEXTURE2D        (see irender.h)
        ::strcpy((*pScene)->pTextures[t].filename, __QLevel._texnames[t].c_str());
    }

    // store brushes
    int indexBr = 0;
    FOREACH(vvector<QBrush*>,  allBrushes, ppBrush){
        vvector<Plane*>     planes;  
        vvector<QPoly>      polys;
        vvector<V3*>        centers;  
        vvector<UV*>        shifts;  
        vvector<int>        texIdxes;  

        Plg_Brush&          plgBrush =  (*pScene)->pBrushes[indexBr];
        QBrush*             pBrush   = *ppBrush;
        int                 indexp   = 0;
        
        // for each qface line collect planes 
        FOREACH(vvector<QBrushFace*>,  pBrush->_pbrshlines, ppBrLine){
            QBrushFace* pLine = *ppBrLine;
            planes   << &pLine->_plane;
            centers  << &pLine->_center;
            shifts   << &pLine->_shft;
            texIdxes <<  pLine->_indexTex;
        }

        // all brush faces, get out brush polygons
        FromPlanes(planes, centers, shifts, texIdxes, 0, polys);   
        // load them in the plgBrush
        if(polys.size() > 4)    // if we have more than 4 polys into the scen 
        {                       // we may have a convex region
            plgBrush.nPolys = polys.size();
            plgBrush.pPolys = new Plg_Poly[polys.size()];
            ::memset(plgBrush.pPolys, 0, sizeof(polys.size())); // DONT FORGET !!!!
            // alloc vertexes space        
            FOREACH(vvector<QPoly>, polys, ppPoly)
            {
                plgBrush.pPolys[indexp].nvXes = ppPoly->_vxes.size();
                plgBrush.pPolys[indexp].vXes  = new Vtx[ppPoly->_vxes.size()];
                ++indexp;
            }
            // store info in plg brush
            plgBrush.flags = pBrush->_pParent->_flags;

            indexp = 0;
            _FOREACH(vvector<QPoly>, polys, ppPoly){
                plgBrush.pPolys[indexp].texIdx[2]   = -1;   // NO TEXTURE SET INDEX TO -1
                plgBrush.pPolys[indexp].texIdx[1]   = -1;   // NO TEXTURE SET INDEX TO -1
                plgBrush.pPolys[indexp].texIdx[0]   = ppPoly->_texIdx; // index in texture previously saved
                
                plgBrush.pPolys[indexp].texapply[0] = 0;    // for how to apply the texture
                plgBrush.pPolys[indexp].texapply[1] = 0;    // see irender.h
                plgBrush.pPolys[indexp].texapply[2] = 0;

                plgBrush.pPolys[indexp].combine     = 0x1; // LOWER BYTE INDICATES HOW MANY TEXTURES WERE SET
                plgBrush.pPolys[indexp].flags       = 0;   // no special flags for the polygon couse I dont know them  
                                                           // SEE POLYGON FLAGS IN THE bspfilestr.h
                int idxv = 0;
                FOREACH(vvector<V3>, ppPoly->_vxes, pVx){
                    plgBrush.pPolys[indexp].vXes[idxv]._xyz   = *pVx;
                    plgBrush.pPolys[indexp].vXes[idxv]._uv[0] = ppPoly->_uvs[idxv];
                    ++idxv;
                }
                ++indexp;
            }
        }
        ++indexBr;
    }
    return 0;   // no error
}

//---------------------------------------------------------------------------------------
long PqmapLoader::GetVersion()
{
    return 1;
}

//---------------------------------------------------------------------------------------
// Exporter. Brushes comming in are not convex, therefore they have to be split and capped
// back in order to have them convex as qmap file wants them. This works only if
// the scene you are exporting is build with no cuts. (just by adding convex)
long PqmapLoader::ExportFile(IGeticEditor* pe, TCHAR* bsFileName, const Plg_Scene* pScene)
{
    FileWrap    fw;
    char        texname[64];

    if(!fw.Open(bsFileName, "wb"))
        return -1;
        
    // expand textures in a readable variable
    Plg_Texture* ptrTotextures = pScene->pTextures;
    
    fprintf(fw._pf, "{\r\n");
    fprintf(fw._pf, "\"message\" \"%s\"\r\n", "scene");
    fprintf(fw._pf,"\"classname\" \"worldspawn\"\r\n");
    fprintf(fw._pf,"\"_color\" \"1 1 1\"\r\n"); // poly caries the color
    for(int i=0; i< pScene->nBrushes;i++)
    {
        Plg_Brush& b = pScene->pBrushes[i];

        //// vvector<QBrCvx>  cvxbr; 
        //// MakeConvex(b, cvxbr); // later on

        FOREACH(vvector<Plg_Brush*>,  fragments, ppPlgBrush)
        {
            Plg_Brush& bf = *(*ppPlgBrush);

            fprintf(fw._pf,"// brush %d\r\n", i+1); 
            fprintf(fw._pf, "{\r\n");
        
	        for(int j=0; j< bf.nPolys;j++)
	        {
                Plg_Poly& p = bf.pPolys[j];
                // put first 3 points of the poly. 
                // scale world by 4
            
                p.vXes[0]._xyz/=4.0f;
                p.vXes[1]._xyz/=4.0f;
                p.vXes[2]._xyz/=4.0f;

                if(p.texIdx[0] < pScene->nTextures)
                {
                    ::strcpy(texname, pScene->pTextures[p.texIdx[0]].filename);
                }
                else
                {
                    ::strcpy(texname, "none");
                }
                ::fprintf(fw._pf, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s %f %f %f %f %f %f %f %f\r\n", //// %f %f
                          p.vXes[0]._xyz.x, p.vXes[0]._xyz.z, p.vXes[0]._xyz.y,
                          p.vXes[2]._xyz.x, p.vXes[2]._xyz.z, p.vXes[2]._xyz.y,
                          p.vXes[1]._xyz.x, p.vXes[1]._xyz.z, p.vXes[1]._xyz.y,
                          texname,
                          p.texax[0].x, p.texax[0].y, p.texax[0].z, p.texShift.u,
                          p.texax[1].x, p.texax[1].y, p.texax[1].z, p.texShift.v);                 //texScale._u, texScale._v)

	        }
            ::fprintf(fw._pf, "}\r\n");
            delete  (*ppPlgBrush);
        }
        fragments.clear();
    }
    ::fprintf(fw._pf, "}\r\n");
    return 0;
}

//---------------------------------------------------------------------------------------
void PqmapLoader::MakeConvex(Plg_Brush& bf, vvector<Plg_Brush*> outs)
{
    QBrCvx           inbrush;
    // use this for storage convenience. 
    // move them and pass the mem managemebt to stl.
    for(int j=0; j < bf.nPolys;j++){
        Plg_Poly& p  = bf.pPolys[j];
        QPoly     qp;  
        qp._texIdx = bf.pPolys[j].texIdx[0];
        qp._flags  = bf.flags;

        for(int v=0; v < p.nvXes;v++){
            qp._vxes << p.vXes[v]._xyz;
            qp._uvs  << p.vXes[v]._uv[0];
        }
        cvxbr._flags = bf.flags;
        cvxbr._polys << qp;
    }
    MakeConvex(inbrush, cvxbr);
}

//---------------------------------------------------------------------------------------
int PqmapLoader::IsConvex(QBrCvx& inbrush, QBrCvx& a, QBrCvx& b, QPoly* splPoly)
{
    FOREACH(vvector<QPoly>, inbrush._polys, pp)
    {
        Plane pl(pp->_vxes[0], pp->_vxes[1], pp->_vxes[2]);

        FOREACH(vvector<QPoly>, inbrush._polys, pp2)
        {
            if(pp2 == pp) continue;

            QPoly  pa;
            QPoly  pb;

            pa._texIdx = pp2->._texIdx;

            Split(pl, pp2->_vxes, pa._vxes, pb._vxes);

            if(pa._vxes.size()>=3)
                a._polys << pa;

            if(pb._vxes.size()>=3)
                b._polys << pb;
        }

        if(b._polys.size() && a._polys.size())
        {
            pp->_flags |= POLY_IS_SPLITTER;
            splPoly = pp;
            return HAS_BOTH;
        }
    }
    return b._polys.size() ? HAS_BACKS : HAS_FRONTS;
}

//---------------------------------------------------------------------------------------
void PqmapLoader::MakeConvex(QBrCvx inbrush, vvector<QBrCvx>& cvx)
{
    QBrCvx a;
    QBrCvx b;
    QPoly* pSlitter;

    DWORD dwRez = IsConvex(inbrush, a, b, pSlitter);
    switch(dwRez)
    {
        case HAS_BOTH:
            MakeConvex(a, cvx);
            MakeConvex(b, cvx);
            break;
        case HAS_BACKS:
            cvx << b;
            break;
        case HAS_FRONTS:
            //discard these
            break;
    }
}

//---------------------------------------------------------------------------------------
// pseudo code how to split the brush
/*
        allocate out convex brushes
        for each face plane
        classify all brush vertexes.
        if only front vertexex 
            is concave error
        if only back vertexes 
            is convex store in out convex brushes and return
        if back and front vertexes
        split brush by this face plane 
        seal both fragment brush { build a bogus poly on this splitter 
                                   cut it with all faces that have been split or touched
                                   add remainig polygon to both fragments. reverse one 
                                   face acordingly}
        repeat for front and back fragments
        save the out convex brushes
*/


//---------------------------------------------------------------------------------------
long PqmapLoader::GetMenuStringAndType(TCHAR* bsFileName, DWORD* type)
{
    _tcscpy(bsFileName,"Quake map files,map");
    *type = PLUG_IMPORTER|PLUG_EXPORTER;
    return 0;
}

//---------------------------------------------------------------------------------------
long PqmapLoader::ImportFile(IGeticEditor* pe, TCHAR* bsFileName, Plg_Scene** pScene)
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
    RELEASE_SCENE(pScene);
    return NO_ERROR;
}

//---------------------------------------------------------------------------------------
int Parser::Parse(vvector<string>& tokens)
{
    map<string, PfHandler>::iterator fi = _classes.find(tokens[0]);
    if(fi != _classes.end()){
        return (this->*_classes[tokens[0]])(tokens);
    }
    return 1;
}

//---------------------------------------------------------------------------------------
int Parser::BaseParse(const char* pline)
{
    vvector<string> tokens;
    Explode(pline, tokens,' ');
    if(tokens.size()){
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
    while(pSubStr = strtok(pLocal, pTok)){
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
    if(_classname== "\"worldspawn\"")
    {
        TRACEX("New Brush\r\n");
        QBrush* pBrush = new QBrush(_pfw, this);
        while(pBrush->Step() > 0);
    }
    return 1;
}

//---------------------------------------------------------------------------------------
int QEntity::Handle_KeyVal(vvector<string>& tokens)
{
    if(tokens.size() == 0)
        return 1;
    string values;

    for(int i=1; i<tokens.size(); ++i){
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

    for(int i=1; i<tokens.size(); ++i){
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
    if(_classname== "\"worldspawn\""){
        TRACEX("Store entity brush in scene\r\n");
        this->_flags = BRSH_SOLID;
        __QLevel._entities << this;
    }else{
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
    if(this->_pbrshlines.size() &&_pParent->_classname=="\"worldspawn\""){
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
    
    FOREACH(vvector<string>, tokens, pstring){
        ::strcat(originalLine, pstring->c_str());
        ::strcat(originalLine, " ");
    }

    int spacecount = tokens.size();
    if(spacecount > 22){ // new file
        ::sscanf(originalLine, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s %f %f %f %f %f %f %f %f" , &a,&b,&c,&d,&e,&f,&g,&h,&i, name, &u0, &u1, &u2, &u3, &v0, &v1, &v2, &v3);
    }
    else{               // never tested (old file)
        ::sscanf(originalLine, "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %s %f %f %f" , &a,&b,&c,&d,&e,&f,&g,&h,&i, name, &u0, &u1, &u2);
    }
    // scale the world by 4
    V3  v11(a,c,b);
    V3  v22(d,f,e);
    V3  v33(g,i,h);
    v11*=4.0f; v22*=4.0f; v33*=4.0f;

    _center = V3(0,0,0);
    _center += v11;
    _center += v22;
    _center += v33;
    _center /= 3.0f;    // from wich we spawn the polygon

    _plane.CalcNormal (v11,v22,v33);

    // reject same planes in this brush
    FOREACH(vvector<QBrushFace*>,  _pParent->_pbrshlines, ppBrLine)
    {
        QBrushFace* pLine = *ppBrLine;
        if(_plane == pLine->_plane){
            return 1;
        }
    }
    _OK = TRUE;

    // save texture information
    _indexTex = __QLevel.AddTex(name);
    _ax[0]  = V3(u0, u1, u2);
    _shft.u = u3;
    _ax[1]  = V3(v0, v1, v2);
    _shft.v = v3;
    
    return 1;
}




