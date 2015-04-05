// P3dsLoader.h: interface for the P3dsLoader class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_)
#define AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_

#include "baselib.h"
#include "basecont.h"
#include "..\\_include\\geticplug.h"


//-------------------------- 3DS-------------------------------------------------

struct D3dsPoly
{
    unsigned short a[3];
    short flags;
};

//-------------------------- Object-------------------------------------------------
struct D3Ds_Brush
{
    D3Ds_Brush()
    {
        ::memset(name,0,sizeof(name));
    }

	char                name[32];
    vvector<D3dsPoly>    polys;
};

//-------------------------- Mojects-------------------------------------------------
struct D3Ds_Scene
{
    D3Ds_Scene()
    {
        _brushes.reserve(64);
        vtxes.reserve(65535);
        texxes.reserve(65535);
    }
    vvector<D3Ds_Brush*>  _brushes;
    vvector<V3>          vtxes;
    vvector<UV>          texxes;

};

//-------------------------- Loader-------------------------------------------------
class P3dsLoader  : public IGeticPlug
{
public:
    P3dsLoader(){};
    virtual ~P3dsLoader(){};
	long _stdcall ImportFile(TCHAR* bsFileName, Plg_Scene** ppBrush) ;
	long _stdcall ExportFile(TCHAR* bsFileName, Plg_Scene* pBrush);
	long _stdcall GetMenuStringAndType(TCHAR* bsFileName,DWORD* type);
    long _stdcall ReleaseScene(Plg_Scene* ppBrush);
    long _stdcall GetVersion(){return 1;}
private:

    long _Load3Ds(TCHAR* fileName, D3Ds_Scene&  b);

};

#endif // !defined(AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_)




