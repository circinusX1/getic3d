// **************************************************************************************
// Copyright (c) 2000-2005 Zalsoft Inc
// www.zalsoft.com
// sales@zalsoft.com
// You are licensed to use this file on a single computer only. 
// **************************************************************************************
#if !defined(AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_)
#define AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_

#include "baselib.h"
#include "..\\_include\\geticplug.h"

//-------------------------- MSD-------------------------------------------------
struct UrScene;
struct UtPoly;
class UTLoader  : public IGeticPlug
{
public:
    UTLoader(){};
    virtual ~UTLoader(){};
	long _stdcall ImportFile(IGeticEditor* pe,char*, char* bsFileName, Plg_Scene** ppBrush) ;
	long _stdcall ExportFile(IGeticEditor* pe, char*,char* bsFileName, const Plg_Scene* pBrush);
	long _stdcall GetMenuStringAndType(char* bsFileName,DWORD* type);
    long _stdcall ReleaseScene(Plg_Scene* ppBrush);
    long _stdcall GetVersion(){return 1;};

protected:
    BOOL HandlePolygon(UrScene& s, UtPoly& p, FILE* f, V3& location, V3& rotation);
    void CalcTexCoord(UrScene& s, UtPoly& p);
    void HandleBrush(FILE* f, UrScene& s, V3& location, V3 rotation, BOOL csgadd);

    IGeticEditor* _pe;
};

#endif // !defined(AFX_MS3DLOADER_H__6D400CE1_D6D8_4ABA_B6B3_8DB6D8A2EF58__INCLUDED_)
