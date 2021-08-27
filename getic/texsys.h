// TexHandler.h: interface for the TexHandler class.
// This code has been released for tutorial purposes. Do not use any of this code in
// any other sofware products. 
//							
// Started: Dec 2001
//---------------------------------------------------------------------------------------
#ifndef __ZTEXTURE_H__
#define __ZTEXTURE_H__

#include <map>
#include "mfc.h"
#include "baselibhdrs.h"
#include "basecont.h"
#include "_irender.h"



#ifndef __HTEX
    typedef unsigned int HTEX;
    #define __HTEX
#endif //


//--------------[READS JPEG, TGA IMAGES FILE]--------------------------------------------
class NO_VT TexHandler
{
public: 
	TexHandler();
	virtual ~TexHandler();
    BOOL    LoadThisFile(const char *sFileName, size_t flags);
	BOOL    LoadFile(const char *sFileName, size_t flags);
	int     SaveTgaFile(const char *sFileName);
    BOOL    MakeBW();
    BOOL    MakeDot3(REAL);
	BYTE*   Buffer(){
        if(p_pBuff)
		    return p_pBuff[0];
        return 0;
	}
    void Reset()
    {
        Deallocate();
        n_x      = 0;
	    n_y      = 0;
	    n_bpp    = 0;
	    n_size   = 0;
	    b_Filter = FALSE;
	    b_MipMap = TRUE;
	    b_Ilum   = FALSE;
    }
	void MaskBW();
    static void Resize(int nBytes, BYTE* pBufIn, int wIn, 
		          int hIn, BYTE* pBufOut, int wOut, int hOut) ;
    static BYTE* AlphaIt(BYTE* pTemp, int x, int y, int& bpp);
    static void  MaskBW(BYTE* pb, int sz, int bpp);
    static void  SwapRB(BYTE* pRGB, int bpp, int cx, int cy);
    unsigned long GetImgSize(){return n_size;}
    void  AlphaIt(BYTE alpha);
    void  Invert();
    void  SaveBMP(char *filename);
    void  SwapRB();
 static void  SetSearchPath(const char* exreaPath){
        if(exreaPath)
            strcpy(_extrapath, exreaPath);
        else 
            _extrapath[0]=0;
    }
protected:	
    BOOL   _GetPixel(BYTE* pOld,  int x, int y);
    int    LoadFile2(const char *sFileName, size_t flags=0);
	BOOL   LoadJpegFile(const char *sFileName, size_t flags=0);
	BOOL   LoadTgaFile(const char *sFileName, size_t flags=0);
	BOOL   LoadBmpFile(const char *sFileName, size_t flags=0);

	BOOL   CreateBuffer(int width,int height, int deep);
	BOOL   CreateMap(REAL mapScale);
	void   Deallocate();
	BYTE** Allocate(int lines, int block);
    BOOL LoadBMP1bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
	BOOL LoadBMP8bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);
	BOOL LoadBMP4bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader);

public:
	int     n_x;
	int     n_y;
	int     n_bpp;
	int     n_size;
	BYTE**  p_pBuff;	
	BOOL    b_MipMap;
	BOOL    b_Filter; 
	BOOL    b_Ilum;
	static char _imgSubPath[255];
    static HTEX  GDefaultTexture;
    static Htex  GDefaultHtexture;
    static char _extrapath[256];
};

//-------------[TEXTURE MAPP BETWEEN FILE AND ID]----------------------------------------
struct Texture
{
	Texture():cx(0),cy(0),bpp(0),pBuff(0),refcount(0){}
	Htex        hTex;
    int         cx;
    int         cy;
    int         bpp;
    BYTE*       pBuff;
	int         refcount;
    char       filename[32];
};

//-------------[]-----------------------------------------------------------------------
typedef Htex  (*STCB)(int dx, int dy, int bpp, BYTE* pb, size_t mips);
typedef void  (*RTCB)(Htex* tex, int ncount);
//-------------------------[manages textures in system]---------------------------------
class TexSys : public map<tstring, Texture>
{
public:
    TexSys(){
        _stx = 0;
        _rtx = 0;
        _cleaned = FALSE;
    }
    TexSys(STCB scb, RTCB rtc){
        _stx = scb;
        _rtx = rtc;
        _cleaned = FALSE;
    }
    ~TexSys(){
		assert(size()==0);
	}
    Htex& GenTexture(const char* ptName, int x, int y, int bpp, BYTE* buff, size_t flags);
	Htex& AddTextureFile(const char* ptFile, size_t flags = TEX_NORMAL);
    Htex& AddTextureFileGetImage(const char* filename, BYTE** pb, int* dx, int* dy, int* bpp, size_t flags= TEX_NORMAL);
    Htex& Assign(const char* psz, size_t flags);
	const char* GetTexName(Htex& id);
    void RemoveTexture(Htex& iTex,BOOL b=TRUE);
    void RemoveTextures(Htex* iextures, int count);
	void RemoveTextureFile(const char* psz, size_t flags=TEX_NORMAL);
    void Clean();
    Texture* GetTempTexture(const char* ptName, size_t flags=TEX_NORMAL);
    Texture* GetTempTexture(Htex& tex);

public:
    STCB        _stx;
    RTCB        _rtx;
    static BOOL _GNoRefs;
    BOOL        _cleaned;
};

//-------------------------[for jpeg lbb]------------------------------------------------
typedef struct _jpegdata 
{
    BYTE *ptr;
    int    width;
    int    height;
    FILE  *output_file;
    FILE  *input_file;
    int    aritcoding;
    int    CCIR601sampling;
    int    smoothingfactor;
    int    quality;
    HWND   hWnd;
    int    ProgressMsg;
    int    status;
    int    components;
} JPEGDATA;

void JpegWrite( JPEGDATA *jpgStruct );
void JpegInfo(  JPEGDATA *jpgStruct );
void JpegRead(  JPEGDATA *jpgStruct );



#endif // !defined(AFX_ZTEXTURE_H__078F6B43_840A_4286_8A95_CB2DB6C66FE5__INCLUDED_)
