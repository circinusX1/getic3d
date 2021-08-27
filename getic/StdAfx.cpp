// stdafx.cpp : source file that includes just the standard includes
//	z_ed2.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "z-edmap.h"
#include "UiEnhancer.h"
#include "Crc.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "../../_lib/extern/glaux.lib")
#pragma comment(lib, "../_lib/extern/jpg.lib")

#ifdef _DEBUG
    #pragma comment(lib, "../_lib/baselibd.lib")
#else
    #pragma comment(lib, "../_lib/baselib.lib")
#endif //


extern BOOL _Ginitializing;
BOOL    GFrustCull   = 0;
BOOL    BShowLCirc   = 1;
BOOL    BShowTrPolys = 1;
size_t   PaintCounter = 0;
BOOL    GNewUID     = 1; 
REAL	GEpsilon	= .5f;
BOOL    GDetEnabled = FALSE;
int     GMinDetDim  = 64;    
int     GMaxDetDim  = 100;
int     GCullTerrCell = 0;
BOOL    BShowCb      = 0;
BOOL    GDisableGrpCSG=1;
BOOL    GKeepProjections=1;
int     GMinDetPolys= 32; 
REAL    GGridSnap   = 32; 
BOOL	GBackFace	= TRUE;  
BOOL	GLight		= FALSE;
BOOL    GShowCuts   = FALSE;
REAL    GMeters     = 1.0f;
BOOL	GForceBSP	= FALSE;
BOOL    GCarving    = FALSE;
BOOL	GUpdateImm  = FALSE;
BOOL    GAutoSave   = 1;
size_t    GUtex       = 0;  
size_t    GAnimCompilation = FALSE;
BOOL    GSelTexFace = 1;
BOOL    BrushTask   = 1;
BOOL    BVxNormals  = 0;
BOOL    CompileTask = 0;//CK_TASK
BOOL    CSGTask     = 0;
BOOL    GNormals    = 0;
int     TerLeafCelTiles = 16;
int     TerrTiles       = 64;
int     SnapToXY      = 16;

RGBA	CLR_NORMAL	= {182,182,182,255};
RGBA	CLR_DARK	= {96,96,96,255};
RGBA	CLR_CUT		= {180,0,0,255};
RGBA	CLR_WHITE	= {255,255,255,255};
RGBA	CLR_SELECT	= {162,255,182,192};
RGBA	CLR_SELECTDIMED	= {142,235,162,128};
RGBA	CLR_SELBOX	= {0,64,200,222};
RGBA	CLR_SELBOX_TCH = {0,96,255,255};
RGBA	CLR_CAMSTART={0,0,255,255};
RGBA	CLR_CAMEND  ={0,0,64,255};
RGBA	CLR_CAMFRUST={0,0,255,255};
RGBA	CLR_SCRSURFACE={0,0,255,75};
RGBA	CLR_PRJDBRUSH={192,100,0,128};
RGBA	CLR_CUT3DBRUSH={255,0,0,64};
RGBA	CLR_CUT3DBRUSHREV={255,128,0,54};
RGBA	CLR_PORTALS={0,0,128,200};
RGBA	CLR_PORTALS1={0,128,0,200};
RGBA	CLR_PORTALSLINE={255,255,192,255};
RGBA	CLR_BLOOR={255,255,255,128};
RGBA	CLR_DETAIL={128,128,0,255};
RGBA	CLR_TERRAIN={84,72,60,255};
RGBA	CLR_DETAILTRIG={192,255,0,255};
RGBA	CLR_SKYDOM={0,200,200,255};
RGBA	CLR_LIGHTBULB={64,0,0,255};
RGBA	CLR_MOTION={46,150,220,255};
RGBA	CLR_TRIGGER={250,84,84,255};
RGBA	CLR_FACEDETAIL={192,192,100,255};
RGBA    CLR_NEW_BRITEM={255,147,149,255};
RGBA    CLR_ZONE={160,60,160,255};
RGBA    CLR_GRIDF		= {52,52,54,255};
RGBA    CLR_FINEGRIDF	= {42,42,42,255};
RGBA    CLR_GRIDD		= {38,38,38,255};   
RGBA    CLR_FINEGRIDD	= {28,28,28,255};
RGBA    CLR_AXES_X		= {111,64,64,255};
RGBA    CLR_AXES_Y		= {64,111,64,255};
RGBA    CLR_AXES_Z		= {64,64,111,255};




char    GeticPrimitiveFile[]   = "GPGF";   // getic primitive geometry file
char    GeticGeometryRawFile[] = "GGRF";   // getic primitive geometry file
char    GeticGeometryBeamTree[] = "GGBT";   // getic primitive geometry file


const char* MakeBinStr(size_t dw)
{
	static char ret[512]={0};
	size_t   mask = 0x1;
	int     j = 0;
	memset(ret,0,512);
	for(int i=0; i <31; i++)
	{
		if(dw & mask)
		{
			strcat(ret, MKSTR("%x (%d) \r\n", mask,mask));
		}
		mask<<=1;
	}
	return ret;

}

/*
#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#endif
#include <atlimpl.cpp>
*/


int __stdcall BrowseCtrlCallback(HWND hwnd, size_t uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData != NULL)
	{
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
} // End of BrowseCtrlCallback

const char* BrowseForFolder(char* title,  const char* cs)
{ 
    static char    szpath[PATH_MAX];
    BOOL            success = 0;
    memset(szpath,0,sizeof(szpath));
    
    LPMALLOC lpMalloc;  // pointer to IMalloc

    if (::SHGetMalloc(&lpMalloc) == NOERROR)
    {
        char szBuffer[PATH_MAX]={0};
        char sdn[PATH_MAX];

        
        BROWSEINFO browseInfo;
        browseInfo.hwndOwner = GetActiveWindow();
        browseInfo.pidlRoot = NULL; 
        browseInfo.pszDisplayName = sdn;
        browseInfo.lpszTitle = title;
        browseInfo.ulFlags  = 0;   
        browseInfo.lpfn = BrowseCtrlCallback;
        browseInfo.lParam   = (long)cs;    

        LPITEMIDLIST lpItemIDList = ::SHBrowseForFolder(&browseInfo);

        if (lpItemIDList) 
        {
            if(::SHGetPathFromIDList(lpItemIDList, szBuffer))
            {
                if(szBuffer[0]!='\0')
                {
                    strcpy(szpath,szBuffer);
                    strcat(szpath,"\\");
                    success = 1;
                }
            }
        }
      
        lpMalloc->Free(lpItemIDList);
        lpMalloc->Release();      
    }
    if(success)
        return szpath;
    return 0;
}

static int LTP(void* wh)
{
    if(wh)
    {
        Sleep(100);
        ::PostMessage((HWND)wh,WM_REDRAW_DLG,0,0);
    }
    return 0;
}


void    DelayUpdateWindow(HWND wh)
{
    
    size_t tid;
    if(!_Ginitializing)
        CloseHandle(::CreateThread(0,1024,(LPTHREAD_START_ROUTINE)LTP,(void*)wh,0,&tid));
    //::PostMessage((HWND)wh,WM_REDRAW_DLG,0,0);
/*    
    size_t dwc = GetTickCount();
    MSG msg;
    while(GetTickCount() - dwc < 32 )
    {
	    if (PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE))
        {
	        DispatchMessage(&msg);
            Sleep(0);
        }
    }
    ::PostMessage((HWND)wh,WM_REDRAW_DLG,0,0);
*/

}

//. gst   getic script template
BOOL GetOpenCommand(const char* regPath, CString& command)
{
    HKEY    hKey = NULL;

    if (::RegOpenKeyEx(HKEY_CLASSES_ROOT, regPath,0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        size_t cbData = 0;
        if (::RegQueryValueEx(hKey, NULL, NULL, NULL, NULL, &cbData) == ERROR_SUCCESS && cbData > 0)
        {
            char psz[1024] = {0};
            if (::RegQueryValueEx(hKey, NULL, NULL,NULL, (LPBYTE)psz, &cbData) ==ERROR_SUCCESS)
            {
                command = psz;
            }
        }
        ::RegCloseKey(hKey);
    }
    if (command.GetLength() > 0)
    {
        //  See if env var is in this string
        int nStart = command.Find('%');
        int nEnd   = command.Find('%', nStart+1);
        if (nStart >= 0 && nEnd >= 0)   // get it from between %% an find env var for it
        {
            CString newString;
            if (nStart != nEnd)
            {   
                newString =  command.Mid(nStart + 1, nEnd - nStart - 1);
                newString =  getenv( newString );
                newString += command.Right(command.GetLength() - nEnd-1);
                command   = newString;
            }
        }
        // leave up to exe
        nStart = command.Find(".exe");
		if(-1==nStart)
			nStart = command.Find(".EXE");
        if(nStart > 0)
        {
            command.SetAt(nStart+4, 0);
        }
        if(command[0] == '"')
        {
            command = command.Right(command.GetLength() - 1);
        }
    }
    return command.GetLength() != 0;
}

BOOL    G_CopyFolder(const char* dst, const char* src, const char* filter, BOOL recurs)
{
    REDIR();
	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	

    CDirChange      to(src);

    if(!stricmp(to.CurDir(), src))
    {
	    if((hfFind=FindFirstFile(filter, &fDATA)) != INVALID_HANDLE_VALUE)
	    {
		    if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		    {
                if(fDATA.cFileName[0]!='.')
                {
                    PathHandler ph(fDATA.cFileName);
                    ::CopyFile(fDATA.cFileName, MKSTR("%s%s",dst,ph.File()), 0);
                }
		    }
		    while(FindNextFile(hfFind, &fDATA)!=0)
		    {
			    if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			    {
                    if(fDATA.cFileName[0]!='.')
                    {
                        PathHandler ph(fDATA.cFileName);
                        ::CopyFile(fDATA.cFileName, MKSTR("%s%s",dst,ph.File()), 0);
                    }
                }
		    }
	    }
	    FindClose(hfFind);  
        to.UseObj();
    }

    return 1;    
}


size_t GetHDSerialNumber()
{
    size_t   nVolumeSerialNumber;
    size_t   nLength = 256;
	char    szVolBuffer[256];
	size_t   nVolumeNameSize = 256;
	size_t   nMaximumComponentLength;
	size_t   nFileSystemFlags;     
	char    szFileSystemNameBuffer[256];
	size_t   nFileSystemNameSize = 256;

	GetVolumeInformation("C:\\",szVolBuffer,
                                nVolumeNameSize,
                                &nVolumeSerialNumber,
                                &nMaximumComponentLength,
	                            &nFileSystemFlags, 
                                szFileSystemNameBuffer, 
                                nFileSystemNameSize );
    return nVolumeSerialNumber;
}

/*
 OpenGL 1.3 for 3D
FMod for sounds
GLF Library for fonts
ExtGL Library for handling extensions
LibJpeg for loading images
*/
BOOL SetWindowtext(const char* cstring);
BOOL IsValidUL(char* out)
{
    char cRealn[256];
    char cName[256];
    size_t dw = 255;

    GetComputerName(cName,&dw);
    CharUpper(cName);
    strcpy(cRealn, cName);

    int j = 0;
    int il= strlen(cName);
    for(int i=il;il<32;il++)
    {
        cName[il] = cName[j] + (j % 255);
        ++j;
    }
    cName[il]=0;
    size_t poly = cName[0]<<31 | cName[1]<<24 | cName[2]<<16 | cName[3];
    size_t acum = GetHDSerialNumber();
    
    char loc[256];
    sprintf(loc, "%X:%X:%s", poly, acum, cRealn);
        
    if(out) 
        strcpy(out, loc);

    return SetWindowtext(loc);
}


BOOL SetWindowtext(const char* cstring)
{
    size_t hddser;
    size_t poly;
    char  cName[128];

    sscanf(cstring,"%X:%X:%s",&poly, &hddser, cName);

    int j = 0;
    int il= strlen(cName);
    for(int i=il;il<32;il++)
    {
        cName[il] = cName[j] + (j % 255);
        ++j;
    }
    cName[il]=0;

    CCrc    crc(poly);
    size_t   dwRet = crc.update_crc(hddser, (BYTE*)cName, strlen(cName));

    size_t ulong = theApp.GetProfileInt("MAIN","UNLOCK", 0);
    return 1;//ulong == dwRet;
}

