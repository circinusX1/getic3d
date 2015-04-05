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
#include "UTLoader.h"

#define     ASAMBLE_COMBINERS(c3_,c2_,c1_,nt_)  MAKELONG(MAKEWORD(nt_, c1_),MAKEWORD(c2_,c2_))
//---------------------------------------------------------------------------------------
// polygon flags
#define POLY_INVISIBLE        1					//1
#define POLY_MASKED           2					//10
#define POLY_TRANSLUCENT      4					//100
#define POLY_NOTSOLID         8					//1000
#define POLY_ENVIRONMENT      16				//10000
#define POLY_FORCEVIEWZONE    16				//10000	
#define POLY_SEMISOLID        32				//100000
#define POLY_MODULATED        64				//1000000
#define POLY_FAKEBACKDROP     128				//10000000
#define POLY_TWOSIDED         256				//100000000
#define POLY_AUTOUPAN         512				//1000000000
#define POLY_AUTOVPAN         1024				//10000000000
#define POLY_NOSMOOTH         2048				//100000000000
#define POLY_BIGWAVY          4096				//1000000000000
#define POLY_SPECIALPOLY      4096				//1000000000000
#define POLY_SMALLWAVY        8192				//10000000000000
#define POLY_FLAT             16384				//1000000000000000
#define POLY_LOWSHADOWDETAIL  32768				//10000000000000000
#define POLY_NOMERGE          65536				//100000000000000000
#define POLY_CLOUDWAVY        131072			//1000000000000000000
#define POLY_DIRTYSHADOWS     262144			//10000000000000000000
#define POLY_BRIGHTCORNERS    524288			//100000000000000000000
#define POLY_SPECIALLIT       1048576			//10000000000000000000000
#define POLY_GOURAUD          2097152			//100000000000000000000000
#define POLY_NOBOUNDREJECTION 2097152			//100000000000000000000000
#define POLY_UNLIT            4194304			//1000000000000000000000000
#define POLY_HIGHSHADOWDETAIL 8388608			//10000000000000000000000000
#define POLY_PORTAL           67108864			//100000000000000000000000000
#define POLY_MIRRORED         134217728			//1000000000000000000000000000
#define POLY_NOOCCLUDE        (POLY_MASKED | POLY_TRANSLUCENT | POLY_INVISIBLE | POLY_MODULATED)
#define POLY_NOSHADOWS        (POLY_UNLIT | POLY_INVISIBLE | POLY_ENVIRONMENT | POLY_FAKEBACKDROP)

//---------------------------------------------------------------------------------------
static struct
{
    int utflag;
    int geticflag;
}__flagMap[]={
    {POLY_INVISIBLE       ,FACE_TRANSPARENT},
    {POLY_MASKED          ,FACE_BLACKMASK},
    {POLY_TRANSLUCENT     ,0},
    {POLY_NOTSOLID        ,FACE_NOCOLLIDE},
    {POLY_ENVIRONMENT     ,0},
    {POLY_FORCEVIEWZONE   ,0},
    {POLY_SEMISOLID       ,0},
    {POLY_MODULATED       ,0},
    {POLY_FAKEBACKDROP    ,0},
    {POLY_TWOSIDED        ,FACE_SHOWBACK},
    {POLY_AUTOUPAN        ,0},
    {POLY_AUTOVPAN        ,0},
    {POLY_NOSMOOTH        ,0},
    {POLY_BIGWAVY         ,FACE_TA_WAVY},
    {POLY_SPECIALPOLY     ,0},
    {POLY_SMALLWAVY       ,FACE_TA_WAVY},
    {POLY_FLAT            ,0},
    {POLY_LOWSHADOWDETAIL ,0},
    {POLY_NOMERGE         ,0},
    {POLY_CLOUDWAVY       ,FACE_TA_WAVY},
    {POLY_DIRTYSHADOWS    ,0},
    {POLY_BRIGHTCORNERS   ,0},
    {POLY_SPECIALLIT      ,FACE_FULLBRIGHT},
    {POLY_GOURAUD         ,FACE_GSHADE},
    {POLY_NOBOUNDREJECTION,0},
    {POLY_UNLIT           ,FACE_NOLIGTMAP},
    {POLY_HIGHSHADOWDETAIL,0            },
    {POLY_PORTAL          ,FACE_TRANSPARENT     },    
    {POLY_MIRRORED        ,FACE_MIRROR    },  
};



//------------------------------------------------------------------------------------------------
INLN char*	StripChar(char* psz, char ch)
{
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

//------------------------------------------------------------------------------------------------
long UTLoader::ExportFile(IGeticEditor* pe, char*,char* bsFileName, const Plg_Scene* pBrush)
{
    _pe = pe;
    return -1;
}

//------------------------------------------------------------------------------------------------
long UTLoader::GetMenuStringAndType(char* bsFileName, DWORD* type)
{
    //large enough
    _tcscpy(bsFileName,"UT Files,t3d");
    *type = PLUG_IMPORTER;
    return 0;
}

//------------------------------------------------------------------------------------------------
struct UtPoly
{
	vvector<Vtx>	_vxes;
    DWORD           _flags;
	DWORD			_origFlags;
    int             _texIdx;
    V3              texax[2];               // texture axis         (use them to save q and ut map file)
    UV              texPan;               // shift on u and v     (use them to save q and ut map file)
    UV              texScale; 

};

//------------------------------------------------------------------------------------------------
struct UtModel
{
	DWORD			_flags;
	vvector<UtPoly> _polys;
	string			_name;
};

//------------------------------------------------------------------------------------------------
struct UrScene
{
	vvector<UtModel>	_models;
    vvector<string>     _texnames;

    // adds a unique tex and returns the index into this array
    int  AddTex(const char* n){
        vvector<string>::iterator fi  = find(_texnames.begin(), _texnames.end(), n);
        if(fi != _texnames.end())
            return fi-_texnames.begin();
        _texnames<<n;
        return _texnames.size()-1;
    }

};

//------------------------------------------------------------------------------------------------
void UTLoader::CalcTexCoord(UrScene& s, UtPoly& p)
{
    // get tex coord
    int szX = 256, szY = 256;    // dummy texture bmp size

    if(_pe)
    {
        if(p._texIdx < s._texnames.size())
        {
            Plg_Texture dummyTex;        
            string&     texname = s._texnames[p._texIdx];

            ::strcpy(dummyTex.filename, texname.c_str());
            dummyTex.flags = 0; // want the dimensions only
             if(NO_ERROR == _pe->GetTextureInfo(&dummyTex))           
            {
                szX = dummyTex.dims[0];
                szY = dummyTex.dims[1];
            }
        }
    }

    // texture origin
    V3  texO = -(p.texPan.u * p.texax[0]) / p.texax[0].len();
    texO -= (p.texPan.v     * p.texax[1]) / p.texax[1].len();

    V3   S = p.texax[0] / szX; // xbitmap
    V3   T = p.texax[1] / szY; //ybitmap
    REAL scaleS = S.len();
    REAL scaleT = T.len();
    S  /= scaleS;
    T  /= scaleT;

    for(int i=0; i < p._vxes.size() ;i++)
    {
        V3& v  = p._vxes[i]._xyz;
        UV& t  = p._vxes[i]._uv[0];

		V3	diffX = p._vxes[i]._xyz - texO;

        t.u = Vdp(diffX , (S)) * scaleS;
        t.v = Vdp(diffX , (T)) * scaleT;
    }
    //U = ((vertex - origin) dot textureU + panU)/texW 
    //V = ((vertex - origin) dot textureV + panV)/texH 
}

//------------------------------------------------------------------------------------------------
BOOL UTLoader::HandlePolygon(UrScene& s, UtPoly& p, FILE* f, V3& location, V3& rotation)
{
	char line[512];
	
	V3	origin;
	V3	normal;
	V3	texV;
	V3	texU;
    UV  pan;
	V3	corners[12];
    char* premain;
    int  vxidx = 0;

    while(1)
    {
        ::fgets(line,512,f);
        if(feof(f))
            break;
	    StripChar(line,' ');
        StripChar(line,'\t');

        if(strlen(line) < 8)
            continue;
        if(!strncmp(line, "EndPolygon",10))
            break;

        if(!strncmp(line, "Origin", 6))
        {
            premain = line + 6;
	        sscanf(premain,"%f,%f,%f",&origin.x,&origin.z,&origin.y);
            continue;
        }
        if(!strncmp(line, "Normal", 6))
        {
            premain = line + 6;
            sscanf(premain,"%f,%f,%f",&normal.x,&normal.z,&normal.y);
            continue;
        }
        if(!strncmp(line, "Pan", 3))
        {
            // pan"U=0V=20"
            premain = line + 3;
			char local[32];
			strcpy(local,premain);
			char* ps = strtok(local,"V");
			pan.u = atoi(ps+2);
			ps = strtok(0,"V");
			pan.v = atoi(ps+1);
            continue;
        }
        if(!strncmp(line, "TextureU", 8))
        {
			premain = line + 8;
            sscanf(premain,"%f,%f,%f",&texU.x,&texU.z,&texU.y);
            texU.y=-texU.y;
        }
        if(!strncmp(line, "TextureV", 8))
        {
			premain = line + 8;
            sscanf(premain,"%f,%f,%f",&texV.x,&texV.z,&texV.y);
            texV.y=-texV.y;
        }
        if(!strncmp(line, "Vertex", 6))
        {
			premain = line + 6;
            sscanf(premain,"%f,%f,%f",&corners[vxidx].x,
                                      &corners[vxidx].z,
                                      &corners[vxidx].y);
		//	corners[vxidx].z=-corners[vxidx].z;
            ++vxidx;
        }
    }

    if(vxidx < 3)
        return FALSE;
	
	// M4  we are digging here......
	M4 mt   = MTranslate(-location.x, location.z, location.y);
	//mt    *= MRadRotate(rotation.x, rotation.z, rotation.y);
	//mt    *= MRadRotate(rotation.x, rotation.z, rotation.y);
	

    p.texax[0] = texU;
    p.texax[1] = texV;
    p.texPan = pan;
	for(int j=0; j < vxidx; j++)
	{
		Vtx v;	
		v._xyz   = corners[vxidx-j-1];
		mt.v3transform(v._xyz);
		p._vxes << v;
	}
	p._vxes.reverse();
    Plane p2; 
    p2.CalcNormal(p._vxes[0]._xyz,p._vxes[1]._xyz,p._vxes[2]._xyz);
    normal.norm();
    if(IsZero(Vdp(p2._n, normal)), 0.0001)// poly has to be reversed
    {
        p._vxes.reverse();
    }
	return TRUE;
}

//---------------------------------------------------------------------------------------
void UTLoader::HandleBrush(FILE* f, UrScene& s, V3& location, V3 rotation, BOOL csgadd)
{
	char					line[512];
	map<string, UtModel>	models;
    char*					pszp;
	string					item = "def";
    

    
	while(1)
	{
		fgets(line,512,f);

        if(::strstr(line,"End Brush") || 
            ::strstr(line,"End PolyList"))
			break;
        if(feof(f))
            break;

        if(pszp = ::strstr(line,"Begin Polygon"))
		{
            UtPoly  p;
			DWORD	uflgs = 0;
            string  texture;
            DWORD   flags = 0;
            int     link  = 0;

            char locline[512]    ;
            ::strcpy(locline, pszp+13);
            char* pline = locline;
            char* ptok  = " ";
            char* pword;
            while(pword = strtok(pline, ptok))
            {
                pline = 0;
                if(!strncmp(pword,"Item",4))
                {
                    item = pword+5;
                }
                else if(!strncmp(pword,"Texture",7))
                {
                    texture = pword+8;
                }
                else if(!strncmp(pword,"Flags",5))
                {
                    uflgs = ::atoi(pword+6);

                    for(int i=0; i < sizeof(__flagMap)/sizeof(__flagMap[0]); i++)
                    {
						if(256 & uflgs)
							TRACEX("");
                        if(__flagMap[i].utflag & uflgs)
                        {
                            flags|=__flagMap[i].geticflag;
                        }
                    }
                }
                else if(!strncmp(pword,"Link",4))
                {
                    link = ::atoi(pword+5);
                }
            }

			if(HandlePolygon(s, p, f, location, rotation))
            {
                if(texture.length())
                    p._texIdx = s.AddTex(texture.c_str());
                else
                    p._texIdx = -1;

                p._flags		= flags;
				p._origFlags	= uflgs;	
                CalcTexCoord(s,p);

				if(!(p._flags & FACE_TRANSPARENT))
				{
                    // dont put them by brush name couse they dont define convex regions
					models["a"]._name  = item;
					models["a"]._flags =  csgadd ? BRSH_SOLID|BRSH_DISABLED : BRSH_CUT;
					models["a"]._polys << p;
				}

            }
		}
	}

	for(map<string, UtModel>::iterator b = models.begin(); 
		b != models.end() ; b++)
	{
		s._models.push_back(b->second);
	}
}

//---------------------------------------------------------------------------------------
long UTLoader::ImportFile(IGeticEditor* pe,  char*,char* bsFileName, Plg_Scene** pScene)
{
    _pe = pe;
    
	FILE* f= _tfopen(bsFileName,"rb");
	if(0 == f)	
        return 0;

	char	line[512];
	BOOL	csgadd=1;
	UrScene	s;
	V3		location;
	V3		rotation;
	char*   paftereq;

	while(!feof(f))
	{
		fgets(line,512,f);
		char* ps=0;

		if(ps=strstr(line,"Class=Brush"))
		{
			location.reset();
			rotation.reset();
		}
		else
		if(ps=strstr(line,"CsgOper"))
		{
			char* pop = ps+strlen("CsgOper")+1;
			if(!strncmp(pop, "CSG_Add",7))
				csgadd=1;
			else
				csgadd=1;
		}
		else
		if(strstr(line,"Location"))
		{
			paftereq = strstr(line,"X=");
			if(paftereq)
			{
				paftereq+=2;
				sscanf(paftereq,"%f",&location.x);
			}
			paftereq = strstr(line,"Y=");
			if(paftereq)
			{
				paftereq+=2;
				sscanf(paftereq,"%f",&location.y);
			}
			paftereq = strstr(line,"Z=");
			if(paftereq)
			{
				paftereq+=2;
				sscanf(paftereq,"%f",&location.z);
			}
			//(X=-1280.000000,Y=1280.000000,Z=384.000000)	
		}
		else
		if(strstr(line,"Rotation"))
		{
			//  Rotation=(Yaw=16384)
			paftereq = strstr(line,"Yaw=");
			if(paftereq)
			{
				paftereq+=4;
				sscanf(paftereq,"%f",&rotation.y);
				rotation.y = G2R(rotation.y / 182.04);
			}

			paftereq = strstr(line,"Pitch=");
			if(paftereq)
			{
				paftereq+=6;
				sscanf(paftereq,"%f",&rotation.x);
				rotation.x = G2R(rotation.x / 182.04);
			}

			paftereq = strstr(line,"Roll=");
			if(paftereq)
			{
				paftereq+=5;
				sscanf(paftereq,"%f",&rotation.z);
				rotation.z = G2R(rotation.z / 182.04);
			}
			
		}

		if(strstr(line,"Begin Brush") || strstr(line,"Begin PolyList"))
		{
			HandleBrush(f,s,location,rotation,csgadd);
		}
		if(feof(f))
			break;
	}
	fclose(f);

	// place collected scene in out param
	*pScene = new Plg_Scene;
    ::memset(*pScene, 0, sizeof(Plg_Scene));

	(*pScene)->flags    = 0; // CALC_TEXCOORD1; 
	(*pScene)->nBrushes = s._models.size();
	(*pScene)->pBrushes = new Plg_Brush[s._models.size()];

	(*pScene)->nTextures = s._texnames.size();
	(*pScene)->pTextures = new Plg_Texture[(*pScene)->nTextures];
	for(int t=0; t< s._texnames.size(); t++)
	{
		::strcpy((*pScene)->pTextures[t].filename, s._texnames[t].c_str());
		(*pScene)->pTextures[t].target = 0;
	}

	for(int i=0;i<s._models.size();i++)
	{
		UtModel& model = s._models[i];

		(*pScene)->pBrushes[i].nPolys = model._polys.size();
		(*pScene)->pBrushes[i].pPolys = new Plg_Poly[model._polys.size()];
		(*pScene)->pBrushes[i].flags  = model._flags;
		
		if(model._name.length())
			strcpy((*pScene)->pBrushes[i].name, model._name.c_str());
		else
			(*pScene)->pBrushes[i].name[0]=0;

		for(int j=0;j<model._polys.size();j++)
		{
			UtPoly& poly = 	model._polys[j];
			
            (*pScene)->pBrushes[i].pPolys[j].texIdx[0] = poly._texIdx;
            (*pScene)->pBrushes[i].pPolys[j].texIdx[1] = -1;
            (*pScene)->pBrushes[i].pPolys[j].texIdx[2] = -1;
            (*pScene)->pBrushes[i].pPolys[j].texIdx[3] = -1;

            (*pScene)->pBrushes[i].pPolys[j].combine     = ASAMBLE_COMBINERS(0,1,0,0);    //one texture no combiners
            (*pScene)->pBrushes[i].pPolys[j].texapply[0] = 0;
            (*pScene)->pBrushes[i].pPolys[j].texapply[1] = 0;
            (*pScene)->pBrushes[i].pPolys[j].texapply[2] = 0;


			(*pScene)->pBrushes[i].pPolys[j].user  = poly._origFlags;
			(*pScene)->pBrushes[i].pPolys[j].flags = poly._flags;
			(*pScene)->pBrushes[i].pPolys[j].nvXes = poly._vxes.size();
			(*pScene)->pBrushes[i].pPolys[j].vXes  = new Vtx[poly._vxes.size()];
            // global color
			(*pScene)->pBrushes[i].pPolys[j].color = CLR(255,255,255);

            int allv =poly._vxes.size()-1;
			for(int k=0;k<poly._vxes.size();k++)
			{
				(*pScene)->pBrushes[i].pPolys[j].vXes[k] = poly._vxes[allv-k];
			}
		}
	}

	return 0;
}

//---------------------------------------------------------------------------------------
long UTLoader::ReleaseScene(Plg_Scene* pScene)
{
    RELEASE_SCENE(pScene);// has to be on this heap
    return 0;
}