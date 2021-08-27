// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//


#if !defined(AFX_STDAFX_H__F92295B6_8DB7_496F_A72D_4B4583B98A1F__INCLUDED_)
#define AFX_STDAFX_H__F92295B6_8DB7_496F_A72D_4B4583B98A1F__INCLUDED_

#ifndef WINVER
    #define WINVER 0x501
#endif //

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _USE_BASEG
// #define _BAR_DLGPANE     // tools dialog is on a slplitter

//   #pragma warning (disable: 4786)
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxpriv.h>
#include <afxtempl.h> 
#include <afxole.h> 
#include "progressdlg.h"
#include "Texref.h"
#include <set>
#include "baselibhdrs.h"



#define		WM_USRKEYDOWN	WM_USER+1002

//globals
extern REAL GEpsilon;

#define	AKEY(k_)	(GetAsyncKeyState(k_) & 0x8000)
typedef	BYTE		RGBA[4];

extern BOOL     Gtrial;
extern BOOL     GFrustCull;
extern BOOL     BShowLCirc;
extern BOOL     BShowCb;
extern BOOL     BShowTrPolys;
extern size_t    PaintCounter;
extern BOOL		GCullTerrCell;
extern BOOL     GDisableGrpCSG;
extern BOOL     GKeepProjections;
extern BOOL     GNewUID;
extern REAL		GGridSnap;
extern BOOL		GDetEnabled;
extern int		GMinDetDim;  
extern int		GMaxDetDim;
extern int		GMinDetPolys;    
extern BOOL		GBackFace;
extern BOOL		GLight;
extern BOOL     GShowCuts;
extern REAL     GMeters;
extern BOOL		GForceBSP;
extern BOOL     GCarving;
extern BOOL	    GUpdateImm;
extern  BOOL    GAutoSave;
extern size_t     GUtex;    
extern size_t		GAnimCompilation;
extern BOOL     GSelTexFace;
extern  BOOL    BrushTask;
extern BOOL     BVxNormals;
extern  BOOL    CompileTask;
extern  BOOL    CSGTask;
extern  BOOL    GNormals;
extern int      TerLeafCelTiles;
extern int      TerrTiles;
extern int      SnapToXY;

extern	RGBA	CLR_NORMAL;
extern	RGBA	CLR_DARK;
extern	RGBA	CLR_CUT;
extern	RGBA	CLR_WHITE;
extern	RGBA	CLR_SELECT;
extern	RGBA	CLR_SELECTDIMED;
extern	RGBA	CLR_SELBOX;
extern	RGBA	CLR_SELBOX_TCH;
extern	RGBA	CLR_CAMSTART;
extern	RGBA	CLR_CAMEND;
extern	RGBA	CLR_CAMFRUST;
extern	RGBA	CLR_SCRSURFACE;
extern	RGBA	CLR_CUT3DBRUSH;
extern  RGBA	CLR_PRJDBRUSH;
extern  RGBA	CLR_BLOOR;
extern	RGBA	CLR_CUT3DBRUSHREV;
extern	RGBA	CLR_PORTALS;
extern	RGBA	CLR_PORTALS1;
extern	RGBA	CLR_PORTALSLINE;
extern	RGBA	CLR_DETAIL;
extern	RGBA	CLR_TERRAIN;
extern	RGBA	CLR_DETAILTRIG;
extern	RGBA	CLR_LIGHTBULB;
extern	RGBA    CLR_SKYDOM;
extern	RGBA    CLR_MOTION;
extern	RGBA    CLR_TRIGGER;
extern	RGBA    CLR_FACEDETAIL;
extern	RGBA    CLR_NEW_BRITEM;
extern	RGBA    CLR_ZONE;
extern	RGBA    CLR_GRIDF		;
extern	RGBA    CLR_FINEGRIDF	;
extern	RGBA    CLR_GRIDD		;   
extern	RGBA    CLR_FINEGRIDD	;
extern	RGBA    CLR_AXES_X		;
extern	RGBA    CLR_AXES_Y		;
extern	RGBA    CLR_AXES_Z		;

extern	size_t	ThrID;

extern char    GeticPrimitiveFile[];//   = "GPGF";   // getic primitive geometry file
extern char    GeticGeometryRawFile[];// = "GGRF";   // getic primitive geometry file
extern char    GeticGeometryBeamTree[];// = "GGRF";   // getic primitive geometry file

INLN char*	StripSpaces(char* psz)
{
	char* ps = psz;
	char* pd = psz;
	while(*ps){
		if(*ps==' '||*ps=='*'||*ps==';'||*ps==':'||*ps=='\t'||*ps=='.'||*ps=='\r'||*ps=='\n')
		{	ps++;continue;}
		*pd++=*ps++;
	}
	*pd=0;
	return psz;
}

INLN char*	Replace(char* psz, char a, char b)
{
	char* ps = psz;
	char* pd = psz;
	while(*ps){
		if(*ps==a)
        {
            *pd++=b;
            ps++;
        }
        else
		    *pd++=*ps++;
	}
	*pd=0;
	return psz;
}

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

INLN char*	AnsiStripSpaces(char* psz)
{
	char* ps = psz;
	char* pd = psz;
	while(*ps){
		if(*ps==' ' ||*ps=='\t' ||*ps=='\r'||*ps=='\n')
		{	ps++;continue;}
		*pd++=*ps++;
	}
	*pd=0;
	return psz;
}



INLN char*	StripCrlf(char* psz)
{
	char* ps = psz;
	char* pd = psz;
	while(*ps){
		if(*ps=='\r'||*ps=='\n')
		{	ps++;continue;}
		*pd++=*ps++;
	}
	*pd=0;
	return psz;
}

INLN char*	StripDigits(char* psz)
{
	char* ps = psz;
	char* pd = psz;
	while(*ps){
		if(isdigit(*ps))
		{	ps++;continue;}
		*pd++=*ps++;
	}
	*pd=0;
	return psz;
}

INLN float    RoundNear(float fValue, float fFactor)
{
    if(0== fFactor)fFactor=EPSILON;
     return (float)((int)((fValue + fFactor/2.0f) / fFactor) * fFactor);
}

INLN V3 GCalcNormal(const V3& p0, const  V3& p1, const  V3& p2)
{
    V3 vV1 = p0-p1; 
    V3 vV2 = p0-p2;
    V3 n = Vcp(vV1, vV2);	
    n.norm();	
    return n;
}

INLN void	GCalcNormal(Plane* p, V3& a, V3& b, V3& c)
{
	p->CalcNormal(c,b,a);
}

INLN REAL ParseREAL(CWnd* pWnd, size_t iD)
{
    char sz[64];
    REAL  ret = 0;

    if(pWnd->GetDlgItem(iD)->GetWindowText(sz,64))
        ::_stscanf(sz,"%f",&ret);
    return ret;
}

INLN V3 ParseV3(CWnd* pWnd, size_t iD)
{
    char sz[64];
    V3    ret;
    int   comma = 0;

    if(pWnd->GetDlgItem(iD)->GetWindowText(sz,64))
    {
        for(size_t i=0;i<strlen(sz);i++)
        {
            if(sz[i] == ',')
                comma++;
        }
        if(2== comma )
            ::_stscanf(sz,"%f,%f,%f",&ret.x,&ret.y,&ret.z);
    }
    return ret;
}

INLN UV ParseUV(CWnd* pWnd, size_t iD)
{
    char sz[64];
    UV    ret;
    int   comma = 0;

    if(pWnd->GetDlgItem(iD)->GetWindowText(sz,64))
    {
        for(size_t i=0;i<strlen(sz);i++)
        {
            if(sz[i] == ',')
                comma++;
        }
        if(1== comma )
            ::_stscanf(sz,"%f,%f",&ret.u,&ret.v);
    }
    return ret;
}


INLN CLR ParseCLR(CWnd* pWnd, size_t iD)
{
    char sz[64];
    CLR    ret;
    int   comma = 0;

    if(pWnd->GetDlgItem(iD)->GetWindowText(sz,64))
    {
        for(size_t i=0;i<strlen(sz);i++)
        {
            if(sz[i] == ',')
                comma++;
        }
        if(3== comma )
        {
            int r,g,b,a;
            ::_stscanf(sz,"%d,%d,%d,%d",&r,&g,&b,&a);
            ret.r=r;ret.g=g;ret.b=b;ret.a=a;

        }
    }
    return ret;
}


INLN void Sct(CWnd* pWnd, size_t iD, REAL value)
{
    CWnd* pCtl = pWnd->GetDlgItem(iD);
    pCtl->SetWindowText(MKSTR("%.2f",value));
}

INLN void Sct(CWnd* pWnd, size_t iD, V3& value)
{
    CWnd* pCtl = pWnd->GetDlgItem(iD);
    pCtl->SetWindowText(MKSTR("%.2f,%.2f,%.2f",value.x,value.y,value.z));
}

INLN void Sct(CWnd* pWnd, size_t iD, CLR& value)
{
    CWnd* pCtl = pWnd->GetDlgItem(iD);
    pCtl->SetWindowText(MKSTR("%d,%d,%d,%d",value.r,value.g,value.b,value.a));
}


INLN const char* strchrrev( char* pstr, char tok)
{
    if(pstr==0 || *pstr)
        return 0;
    char* pend = pstr + strlen(pstr)-1;
    while(pend != pstr)
    {
        if(*pstr == tok)
            return pstr;
        --pend;
    }
    return 0;
}

INLN int Str2Int(const char* psz, int def)
{
    if(psz[0])
    {
        int retval = 0;
        ::_stscanf(psz,"%d",&retval);
        return retval;
    }
    return def;
}

INLN REAL Str2Real(const char* psz, REAL def)
{
    if(psz[0])
    {
        REAL retval = 0;
        ::_stscanf(psz,"%f",&retval);
        return retval;
    }
    return def;
}

const char* MakeBinStr(size_t dw);

#define DI(v_)  GetDlgItem(v_)

#define	IS_WINDOW(w_)	(w_ && ::IsWindow(w_))
#define	IS_VISIBLE(w_)	(w_ && ::IsWindowVisible(w_))
#define IS_K            IsDlgButtonChecked
#define DI_             GetDlgItem
#define CK_             CheckDlgButton
#define EN_(u_,f_)      GetDlgItem(u_)->EnableWindow(f_)
#define	EV_(ex_)		((ex_)?BST_CHECKED : BST_UNCHECKED)

#define     SCOPE(val_)     if(val_)

BOOL    G_CopyFolder(const char* dst, const char* src, const char* filter, BOOL recurs=0);

const char* BrowseForFolder(char* title,  const char* cs);
void    DelayUpdateWindow(HWND);

#include "geticgui.h"
#ifdef _DEBUG
    #define   _TRY      do
    #define   _CATCHX()  while(0);if(0)
#else 
    #define   _TRY         try
    #define   _CATCHX()    catch(...)
#endif 


BOOL    IsValidUL(char*);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__F92295B6_8DB7_496F_A72D_4B4583B98A1F__INCLUDED_)
