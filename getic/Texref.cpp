#include "stdafx.h"
#include "z-edmap.h"
#include "Texref.h"
#include "MainFrm.h"
#include "z_ed3View.h"
#include "z_ed2Doc.h"
#include "z_ed2View.h"
#include "extern/glext.h"
#include "extern/wglext.h"

//   #pragma warning (disable: 4786)
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef __HTEX
    typedef long HTEX;
    #define __HTEX
#endif //


BOOL TexRef::_MakeCurrent=1;
Htex TexRef::_defTex;
int SmartTex::_TexCopy = 1; 

static struct Bppform
{
    int bpp;
    int gl1;
    int gl2;
} GBpp[5] = 
{
	{0, -1          ,         -1  },
    {1, GL_LUMINANCE, GL_LUMINANCE},
    {2, -1          ,         -1  },
    {3, GL_RGB,  GL_UNSIGNED_BYTE },
    {4, GL_RGBA, GL_UNSIGNED_BYTE }
};

void    MakeCurrent(char c)
{
    ASSERT(ThrID == GetCurrentThreadId());
	

	switch(c)
	{
		case '3':
			::wglMakeCurrent(z_ed3View::P3DView->_hdc, z_ed3View::P3DView->m_hRC);
			break;
		case 'x':
			::wglMakeCurrent(VIEW2D(1,0)->_hdc, VIEW2D(1,0)->m_hRC);
			break;
		case 'y':
			::wglMakeCurrent(VIEW2D(1,1)->_hdc, VIEW2D(1,1)->m_hRC);
			break;
		case 'z':
			::wglMakeCurrent(VIEW2D(0,1)->_hdc, VIEW2D(0,1)->m_hRC);
			break;
	}
}

void    MakeCurrent(HDC hdc, HGLRC hrc)
{
	if(ThrID != GetCurrentThreadId())
	{
		return;
	}

    if(hdc==0)
        ::wglMakeCurrent(z_ed3View::P3DView->_hdc, z_ed3View::P3DView->m_hRC);
    else
        ::wglMakeCurrent(hdc, hrc);
}


void	TexRef::RemoveTex(Htex* it, int i)
{
    ASSERT(ThrID == GetCurrentThreadId());
    if(ThrID != GetCurrentThreadId())
    {
        //AfxGetMainWnd()->SendMessage(WM_REMOVETEX, it);
        return;
    }
    if(_MakeCurrent)
	    ::wglMakeCurrent(z_ed3View::P3DView->_hdc, z_ed3View::P3DView->m_hRC); 
	if(*it > 0 && glIsTexture(*it))
    {
        glDeleteTextures(1,(size_t*)&it->hTex);
    }
}



Htex TexRef::GlGenTex(int x, int y, int bpp, BYTE* pBuff, size_t flags)
{
    ASSERT(ThrID == GetCurrentThreadId());
	if(ThrID != GetCurrentThreadId())
		return TexRef::_defTex;
    if(_MakeCurrent)
        ::wglMakeCurrent(z_ed3View::P3DView->_hdc, z_ed3View::P3DView->m_hRC); 
    Htex itex     =  _GlGenTex(x, y, bpp, pBuff, flags);
    itex.glTarget =  flags;
    if(itex > 32000)
    {
        Beep(1000,200);
        SBT(0,"Too many textures!");
    }
    return itex;
}

void	TexRef::_RemoveTex(Htex& it)
{
    ASSERT(ThrID == GetCurrentThreadId());
	if(ThrID != GetCurrentThreadId())
		return;


	if(it > 0 && glIsTexture(it))
    {
		glDeleteTextures(1,(size_t*)&it);
    }
}


//--------------------------------------------------------------------------------------------
void Local_LoadBytes(BYTE* pD, BYTE* pS, int sx, int sy, int x, int y, int lS)
{
	BYTE* pDest = pD;
	for(int j=0;j<y;j++)
	{
		for(int i=0; i < x; i++)
		{
			*pDest = pS[sx+i + (sy +j)*lS];
			++pDest;
		}
	}
}


Htex	TexRef::_GlGenTex(int x, int y, int bpp, BYTE* pBuff, size_t flags)
{
	Htex    hTex;
    size_t   texTarget;
    BOOL    ok=0;
    try{

        ::glGenTextures(1,(size_t*)&hTex.hTex);

        if(0==hTex.hTex)
        {
            int le = GetLastError();
            return hTex;
        }
        hTex.glTarget = flags;

        switch(TGET_TARGET(flags))
        {
		    case GEN_TEX_2D_MAP:
                glBindTexture(GL_TEXTURE_2D, hTex.hTex);
                texTarget = GL_TEXTURE_2D;
                break;
		    case GEN_TEX_CUBE_MAP:
                glBindTexture(GL_TEXTURE_CUBE_MAP, hTex.hTex); // pause for now
                texTarget = GL_TEXTURE_CUBE_MAP;
                break;
		    case GEN_TEX_3D_MAP: // disabled
                glBindTexture(GL_TEXTURE_2D, hTex.hTex);
                texTarget = GL_TEXTURE_2D;
                break;
        }

	    // decompose the on bitmap (see if 6 bitmaps are there)
        if(TGET_WRAP(flags) == GEN_TEX_HAS_CUBE_T)
	    {
		    BYTE* lb = new BYTE[(x*y*bpp)/12];

		    Local_LoadBytes(lb, pBuff, x/4, 0, x/4, y/3, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/4, y/3, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);

		    Local_LoadBytes(lb, pBuff, 0, y/3, x/4, y/3, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/4, y/3, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, x/4, y/3, x/4, y/3, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/4, y/3, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, 2*x/4, y/3, x/4, y/3, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/4, y/3, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, 3*x/4, y/3, x/4, y/3, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/4, y/3, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, x/4, 2*y/3, x/4, y/3, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/4, y/3, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);
		    delete[] lb;

		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            ok=1;

	    }
	    if(TGET_WRAP(flags) == GEN_TEX_HAS_CUBE_M)
	    {
		    BYTE* lb = new BYTE[(x*y*bpp)/6];

		    Local_LoadBytes(lb, pBuff, 0, 0, x/3, y/2, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/3, y/2, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);

		    Local_LoadBytes(lb, pBuff, x/3, 0, x/3, y/2, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/3, y/2, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, 2*x/3, 0, x/3, y/2, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/3, y/2, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, 0, y/2, x/3, y/2, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/3, y/2, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, x/3, y/2, x/3, y/2, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/3, y/2, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);


		    Local_LoadBytes(lb, pBuff, 2*x/3, y/2, x/3, y/2, x);
		    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
						    0, GL_RGB, x/3, y/2, 0, GL_RGB, GL_UNSIGNED_BYTE, lb);
		    delete[] lb;
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            ok=1;

	    }

        if(TGET_WRAP(flags) == GEN_CLAMP)
        {
		    glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );	
		    glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );	
	    }
	    else
        {
		    glTexParameteri(texTarget,GL_TEXTURE_WRAP_S,GL_REPEAT);		
		    glTexParameteri(texTarget,GL_TEXTURE_WRAP_T,GL_REPEAT);		
	    }
        switch(TGET_FILTER(flags))
        {
            case GEN_TEX_MM_NONE:
                glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                break;
            case GEN_TEX_MM_LINEAR: //lin
                glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                break;
            default:
            case GEN_TEX_MM_BILINEAR: // 2 lin
                glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                break;
            case GEN_TEX_MM_TRILINEAR: // 3 lin
                glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                break;
        }

        if(!ok)
        {
            glTexImage2D(texTarget,0,bpp,x,y,0,GBpp[bpp].gl1, GBpp[bpp].gl2,pBuff);
	        gluBuild2DMipmaps(texTarget,bpp,x,y,GBpp[bpp].gl1, GBpp[bpp].gl2,pBuff);
        }
    }catch(...){}
	return hTex;
}

BYTE* TexRef::LoadRCTextureBuffer(const char* szId,int sx, int sy)
{
	HRSRC  hrsrc = FindResource(AfxGetResourceHandle(),szId, "SCE_ITEM");
    if(hrsrc)
    {
        HGLOBAL hResLoad = LoadResource(AfxGetResourceHandle(), hrsrc);
        if(hResLoad)
        {
            // read the dim here
            size_t dwSize = SizeofResource(  AfxGetResourceHandle(), hrsrc);

			if(sx==sy)
			{
				sx = sqrt(float(dwSize/3.0));
				sy = sqrt(float(dwSize/3.0));
				ASSERT(sx==sy);
    			//fix . make it square if escapes 
				sx =  tmin(sx,sy);
				sy =sx;
			}
            void* pData = LockResource(hResLoad);
            if(pData)
            {
				BYTE* retBuff = new BYTE[sx * sy * 3];

                // make it upside down
                ::memcpy(retBuff, (BYTE*)pData, sx * sy * 3);
                return retBuff;
            }
        }
    }
    return 0;
}


static void  $Invert(BYTE* by, int x, int y, int bpp)
{
    BYTE* line = new BYTE[x*y*3];

	for(int i=0; i < y/2; i++ )
	{
		//flip vertical
		memcpy(&line[0], &by[i], bpp * x);
		memcpy(&by[i], &by[y-i-1], bpp * x);
		memcpy(&by[y-i-1], line, bpp * x);
	}

	delete[]	line;

}



//load a resource id bitmap and get rgb's and make a texture out of it
Htex  TexRef::LoadRCTexture(const char* szId, int sx, int sy, BYTE* pwantbuff)
{
   
	HRSRC  hrsrc = FindResource(AfxGetResourceHandle(),szId, "SCE_ITEM");
    if(hrsrc)
    {
        HGLOBAL hResLoad = LoadResource(AfxGetResourceHandle(), hrsrc);
        if(hResLoad)
        {
            // read the dim here
            size_t dwSize = SizeofResource(  AfxGetResourceHandle(), hrsrc);

			if(sx==sy)
			{
				sx = sqrt(float(dwSize/3.0));
				sy = sqrt(float(dwSize/3.0));
				ASSERT(sx==sy);
    			//fix . make it square if escapes 
				sx =  tmin(sx,sy);
				sy =sx;
			}

            void* pData = LockResource(hResLoad);
            if(pData)
            {
                if(_MakeCurrent)
				    MakeCurrent('3');
                //$Invert((BYTE*)pData,sx,sy, 3);
                if(pwantbuff){
                    memcpy(pwantbuff, pData, sx * sy* 3);
                }
				return _GlGenTex(sx, sy, 3, (BYTE*)pData, TEX_NORMAL);
            }
        }
    }
    return _defTex;
}



Htex TexRef::LoadThisFile(const char* pszFileName, size_t flags)
{
    TexHandler  th;
    if(th.LoadThisFile(pszFileName, flags))
        return _GlGenTex(th.n_x, th.n_y, th.n_bpp, th.Buffer(),  flags);
    return _defTex;
}

Htex  TexRef::LoadFile(char* pszFileName, size_t flags)
{
    TexHandler  th;
    if(th.LoadFile(pszFileName, flags))
        return _GlGenTex(th.n_x, th.n_y, th.n_bpp, th.Buffer(),  flags);
    return _defTex;
}

void   TexRef::Clear(BOOL b)
{
	GTexSys.Clean();
    ::memset(GTexSys._texmap,0,sizeof(size_t)*TEX_CAPACITY);
}

void  TexRef::GlLmapMode(BOOL start)
{
    if(start)
    {
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_EQUAL);
            glBlendFunc(GL_ZERO,GL_SRC_COLOR);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else
    {
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }
}


void  TexRef::GlDecalMode(BOOL start)
{
    if(start)
    {
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_EQUAL);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //transparency
    }
    else
    {
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }

}

void  TexRef::GlHalloMode(BOOL start)
{
    if(start)
    {
/*        
        
        size_t modes[]={     GL_ZERO,
                            GL_ONE,
                            GL_SRC_COLOR,
                            GL_ONE_MINUS_SRC_COLOR,
                            GL_DST_COLOR,
                            GL_ONE_MINUS_DST_COLOR,
                            GL_SRC_ALPHA,
                            GL_ONE_MINUS_SRC_ALPHA, 
                            GL_DST_ALPHA,
                            GL_ONE_MINUS_DST_ALPHA,  
                            GL_SRC_ALPHA_SATURATE,
                            -1,
        };
*/
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_EQUAL);
        glBlendFunc(GL_ONE, GL_ONE);
  //      glBlendFunc(modes[(int)SCENE()._si.fogNear],modes[(int)SCENE()._si.fogFar]);


    }
    else
    {
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }

}

void  TexRef::GlDetailMode(BOOL start)
{
    if(start)
    {

     
        size_t modes[]={     GL_ZERO,
                            GL_ONE,
                            GL_SRC_COLOR,
                            GL_ONE_MINUS_SRC_COLOR,
                            GL_DST_COLOR,
                            GL_ONE_MINUS_DST_COLOR,
                            GL_SRC_ALPHA,
                            GL_ONE_MINUS_SRC_ALPHA, 
                            GL_DST_ALPHA,
                            GL_ONE_MINUS_DST_ALPHA,  
                            GL_SRC_ALPHA_SATURATE,
                            -1,
        };        
        
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_EQUAL);
        glBlendFunc(774,772);


    }
    else
    {
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }

}


    
SmartTex::SmartTex():_pRefs(0){}
SmartTex::SmartTex (const SmartTex& r)
{
    if(_TexCopy==0)   return;
    if(r._hTex.hTex == TexRef::_defTex.hTex || r._hTex.hTex == 0)  
    {
        _hTex = r._hTex;
        return;
    }
    Clear();
    ++GTexSys._texmap[r._hTex.hTex];
    _hTex = r._hTex;
    _pRefs = &GTexSys._texmap[_hTex.hTex];
}

SmartTex::~SmartTex(){
    if(_TexCopy==0)return;
    Clear();
}

SmartTex& SmartTex::operator=(const SmartTex& r)
{
    if(_TexCopy==0)   return *this;
    if(r._hTex.hTex == TexRef::_defTex.hTex || r._hTex.hTex == 0)
    {
        _hTex = r._hTex;
        return *this;
    }
    if(this!=&r){
        Clear();
        ++GTexSys._texmap[r._hTex.hTex];
        _hTex = r._hTex;
        _pRefs = &GTexSys._texmap[_hTex.hTex];
    }
    return *this;
}

SmartTex& SmartTex::operator=(const Htex& ht){
    if(_TexCopy==0)return *this;
    ++GTexSys._texmap[ht.hTex];
    _hTex  = ht;
    _pRefs = &GTexSys._texmap[ht.hTex];
    return *this;
}

void SmartTex::operator--(){
    if(_TexCopy==0)return;
    Clear();
}
const char* SmartTex::GetTexName(){
    return GTexSys.GetTexName(_hTex);
}

void SmartTex::AddRef(){
    _pRefs = &GTexSys._texmap[_hTex.hTex];
    ++(*_pRefs);
}

SmartTex& SmartTex::Assign(const char* p, size_t flags){
    if(_TexCopy==0) 
        return *this;
    Clear();
    Htex   ht = GTexSys.Assign(p, flags);
    if(ht.hTex)
    {
        ASSERT(ht.hTex  < TEX_CAPACITY);
        if(ht.hTex < TEX_CAPACITY)
        {
            ++GTexSys._texmap[ht.hTex];
            _hTex     = ht;
            _pRefs    = &GTexSys._texmap[_hTex.hTex];
        }
        else
        {
            _hTex          = _dumptex; // default dummy
            _hTex.glTarget = 0;
        }
    }else
    {
        _hTex          = ht; // default dummy
        _hTex.glTarget = 0;
    }
    return *this;
}
void SmartTex::Reset(){
    _hTex.hTex = 0; 
}


void SmartTex::Clear(){
    if(_hTex.hTex == TexRef::_defTex.hTex || _hTex.hTex == 0)  return;
    if(GTexSys._texmap[_hTex.hTex]==0) return;
    --GTexSys._texmap[_hTex.hTex];
    if(GTexSys._texmap[_hTex.hTex]==0)
    {
        GTexSys.RemoveTexture(_hTex);
        _hTex.hTex     = 0;
        _hTex.glTarget = 0;
    }
}

