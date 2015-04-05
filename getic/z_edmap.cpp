// z_ed2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "_config.h"
#include "z-edmap.h"
#include "MainFrm.h"
#include "z_ed2Doc.h"
#include "z_ed2View.h"
#include "texsys.h"
#ifdef COM_WAY
    #include <initguid.h>
    #include "IGetic.h"
    #include "Z_edmap_i.c"
#else
    #include "..\\_include\\geticplug.h"
#endif
#include "StaticGL.h"
#include "ToolFrame.h"
#include "Crc.h"
#include "geticplug.h"
#include "Drawout1.h"
#include "ExceptionLog.h"
#include "shlobj.h"
#include "../np_xmoge/_xmoge.h"
#include "../np_xmoge/_xmoge_i.c"  



#ifndef _INC_COMDEF
#pragma message("warning: Build gbtx project first...")
#endif //

static void CheckGbtx();

//#include "..\\stacktracelib\\StackTrace.h" 

#ifndef APSTUDIO_INVOKED
    #define APSTUDIO_INVOKED
#endif //
#ifdef APSTUDIO_READONLY_SYMBOLS
    #undef APSTUDIO_READONLY_SYMBOLS
#endif // 

#include "geticgui.h"
#include "aboutdlg.h"

UINT	ThrID;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void _RegisterShellFileTypes(LPCTSTR path);
int GRuns = 40;
BOOL _Ginitializing ;
extern HICON   __icos[4];
//------------------------------------  ---------------------------------------------------
// CZ_ed2App
BEGIN_MESSAGE_MAP(CZ_ed2App, CWinApp)
	//{{AFX_MSG_MAP(CZ_ed2App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(MN_SHOWCOMPILER, OnShowcompiler)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
    ON_COMMAND_RANGE(_PLUG_MSG0, _PLUG_MSG0+200, OnPlugInsCmd)
END_MESSAGE_MAP()

void CZ_ed2App::OnFileOpen()
{
	REDIR();
	_chdir(theApp.DocsDir());
	CWinApp::OnFileOpen();
}

//---------------------------------------------------------------------------------------
// CZ_ed2App construction
CZ_ed2App::CZ_ed2App():_pToolFrame(0),_bToolOn(0),_msgBoxOn(0)
{
    memset(&_rtMakeTbar,0,sizeof(_rtMakeTbar));
    memset(&_rtSelectTbar,0,sizeof(_rtSelectTbar));
    memset(&_rtMoveTbar,0,sizeof(_rtMoveTbar));
    memset(&_rtAlignTbar,0,sizeof(_rtAlignTbar));
    memset(&_rtViewsTbar,0,sizeof(_rtViewsTbar));
    memset(&_rtCsgTbar,0,sizeof(_rtCsgTbar));
    memset(&_rtBuildTbar,0,sizeof(_rtBuildTbar));

    _dwpMakeTbar    = AFX_IDW_DOCKBAR_TOP; 
    _dwpSelectTbar  = AFX_IDW_DOCKBAR_TOP; 
    _dwpMoveTbar    = AFX_IDW_DOCKBAR_TOP; 
    _dwpAlignTbar   = AFX_IDW_DOCKBAR_RIGHT; 
    _dwpViewsTbar   = AFX_IDW_DOCKBAR_BOTTOM; 
    _dwpCsgTbar     = AFX_IDW_DOCKBAR_BOTTOM; 
    _dwpBuildTbar   = AFX_IDW_DOCKBAR_TOP; 

    _bShowMakeTbar = 1;
    _bShowSelectTbar = 1;
    _bShowMoveTbar = 1;
    _bShowAlignTbar = 0;
    _bShowViewsTbar = 0;
    _bShowCsgTbar = 0;
    _bShowBuildTbar = 1;
    _bShowCurvesbar =0;

    _rectMiniTool.left=0;
    _rectMiniTool.right=0;
    _rectMiniTool.top=0;
    _rectMiniTool.bottom=0;
	_returnVal = TRUE;
    p_wndFocused = 0;
    _runs         = 0;
    ::memset(_keys,0,sizeof(_keys));
}

//---------------------------------------------------------------------------------------
CZ_ed2App::~CZ_ed2App()
{

}


class CMyCommandLineInfo :public CCommandLineInfo 
{
public:
    void ParseParam(const char* pszParam,BOOL bFlag, BOOL bLast)
    {
	    if (bFlag)
	    {
		    USES_CONVERSION;
		    ParseParamFlag(T2CA(pszParam));
	    }
	    else
		    ParseParamNotFlag(pszParam);
    	ParseLast(bLast);
    }

};
//-------;--------------------------------------------------------------------------------
// 
CZ_ed2App theApp;

//---------------------------------------------------------------------------------------
// 
static LPCTSTR m_pszProfileNameOLD;
static char   sz_PROFILE[256];
static char   sz_NOAPPDATA[256] = "GETIC.CFG";
BOOL CZ_ed2App::InitInstance()
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER) Win32FaultHandler);
   
    _Ginitializing=1;

#ifdef _DEBUG
    OBJ_DMP(Texture);
    OBJ_DMP(Texs);
    OBJ_DMP(NoTexCopy);
    OBJ_DMP(SmartTex);
    OBJ_DMP(TexRef);
    OBJ_DMP(TexSys);
    OBJ_DMP(Htex);
    OBJ_DMP(Vtx2);
    OBJ_DMP(Poly);
    OBJ_DMP(Brush);
    OBJ_DMP(TexCarrier);
#endif //
	// SetRegistryKey(_T("GETIC_3D_400"));

    m_pszProfileNameOLD = this->m_pszProfileName;
    
    if(S_OK == SHGetFolderPath(0, CSIDL_COMMON_APPDATA, 0, SHGFP_TYPE_DEFAULT, sz_PROFILE))
    {
        ::strcat(sz_PROFILE,"\\Getic3D");
        ::_mkdir(sz_PROFILE);
        ::strcat(sz_PROFILE,MKSTR("\\GETIC%d.CFG",VER_SION));
        this->m_pszProfileName = sz_PROFILE;
    }
    else
    {
        this->m_pszProfileName = sz_NOAPPDATA;
    }

    CreatePaths();

    if(!Dbg_ResolveCurDir())
    {
        return 0;
    }

    LPTSTR lpCmdLine = GetCommandLine(); 
#ifndef DEBUG
   // TestKey();
#endif //

	if(_returnVal==0)
    {
        return 0;
    }
    HINSTANCE hRes = NULL;
    hRes= LoadLibrary("geticgui.dll");
    if(hRes)
        AfxSetResourceHandle(hRes);
    else
    {
        AfxMessageBox("Cannot load: 'geticgui.dll'", MB_ICONHAND|MB_OK);
        return FALSE;
    }

    CheckGbtx();
    _me.Init();


    
    ThrID = GetCurrentThreadId();

    if(AKEY(VK_SPACE))
    {
        FileWrap fh;
        TexHandler th;

        th.LoadThisFile("C:\\texture.bmp",0);
        fh.Open("C:\\texture.rgb","wb");
        fh.Write(th.Buffer(), th.n_size);
        fh.Close();

        fh.Open("C:\\texture.txt","wb");
        for(int i=0;i<th.n_size;i++)
        {
            unsigned char ch = th.Buffer()[i];
            fprintf(fh._pf, "0x%02X, " ,ch);
        }
        fh.Close();

        
        th.Reset();
        return 0;
    }

    _colexpic[0]=LoadIcon(ICO_COLAPSED);
    _colexpic[1]=LoadIcon(ICO_EXPANDED);

    __icos[0] = AfxGetApp()->LoadIcon(ICO_ANI_1);
    __icos[1] = AfxGetApp()->LoadIcon(ICO_ANI_2);
    __icos[2] = AfxGetApp()->LoadIcon(ICO_ANI_3);
    __icos[3] = AfxGetApp()->LoadIcon(ICO_ANI_4);


    RegisterGLWindows();

    _butBmp.LoadBitmap("BMP_BUTT"); 
    _slpEditBitmap.LoadBitmap(SPLASH_EDIT);
    _txtEditBitmap.LoadBitmap(SPLASH_EDIT1);


	_msgBoxBitmap.LoadBitmap(IDB_ABOUTBOX);

    _slpEditBrush.CreatePatternBrush(&_slpEditBitmap);
    _txtEditBrush.CreatePatternBrush(&_txtEditBitmap);


    //AfxEnableControlContainer();
	//AfxInitRichEdit();	
#ifdef _AFXDLL
	Enable3dControls();			
#else
	Enable3dControlsStatic();	
#endif

    TexSys::_GNoRefs = TRUE; // no reference count on the tex handler

    _txtcolor = GetSysColor(COLOR_WINDOWTEXT);

	COLORREF cs = GetSysColor(COLOR_BTNFACE);
	BYTE rr0 =	GetRValue(cs); 
	BYTE gg0 =	GetGValue(cs); 
	BYTE bb0 =	GetBValue(cs); 

	BYTE rr1 =	GetRValue(cs); if(rr1 < 230) rr1+=24; else rr1=245;
	BYTE gg1 =	GetGValue(cs); if(gg1 < 230) gg1+=24; else gg1=245;
	BYTE bb1 =	GetBValue(cs); if(bb1 < 230) bb1+=24; else bb1=245;

	BYTE rr2 =	GetRValue(cs); if(rr2 < 200) rr2+=42; else rr2=255;
	BYTE gg2 =	GetGValue(cs); if(gg2 < 200) gg2+=42; else gg2=255;
	BYTE bb2 =	GetBValue(cs); if(bb2 < 200) bb2+=42; else bb2=255;
                                                           
    _color[0] = RGB(rr0, gg0, bb0);
    _color[1] = RGB(rr1, gg1, bb1);
    _color[2] = RGB(rr2, gg2, bb2);
    _color[3] = RGB(0, 0, 0);


//    _color[0] = RGB((rr-60),(gg-60),(bb-60));
//    _color[1] = RGB(rr,gg,bb);
//    _color[2] = RGB(tmin(255,(rr+40)),tmin(255,(gg+40)),tmin(255,(bb+40)));

    _brush[0].CreateSolidBrush(_color[0]);
    _brush[1].CreateSolidBrush(_color[1]);
    _brush[2].CreateSolidBrush(_color[2]);
    _brush[3].CreateSolidBrush(_color[3]);

	SetDialogBkColor(_color[1]);

	
	LoadStdProfileSettings();  
    LoadCustomProfileSettings();

    _compilingCur        = AfxGetApp()->LoadCursor(IDR_COMPILING);
    _cursors[CUR_SCROLL] = AfxGetApp()->LoadCursor(IDC_SCROLL);
    _cursors[CUR_MOVE]   = AfxGetApp()->LoadCursor(IDC_CMOVE);
    _cursors[CUR_SCALE]  = AfxGetApp()->LoadCursor(IDC_CSCALE);
    _cursors[CUR_ROTATE] = AfxGetApp()->LoadCursor(IDC_CROTATE);  
    _cursors[CUR_NS]     = AfxGetApp()->LoadStandardCursor(IDC_SIZENS); //IDC_ARROW
    _cursors[CUR_EW]     = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
    _cursors[CUR_NWSE]   = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
    _cursors[CUR_NESW]   = AfxGetApp()->LoadStandardCursor(IDC_SIZENESW);


	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME,
	                                      RUNTIME_CLASS(CZ_ed2Doc), // how stupid runtime 'type'
	                                      RUNTIME_CLASS(CMainFrame),       
	                                      RUNTIME_CLASS(CZ_ed2View));

    _h3dEditAccel     = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_3DEDITACCEL));
    _hScriptEditAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_SCREDITOR_ACCEL));
       
    AddDocTemplate(pDocTemplate);

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	CMyCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}

    cmdInfo.m_nShellCommand=CCommandLineInfo::FileNew;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


    _cmdPlugIn = _PLUG_MSG0;

    _Ginitializing=0;    
#ifndef _DEBUG
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
#endif //
	m_pMainWnd->UpdateWindow();

    FindPlugIns();

	if(_returnVal==0)
		PostQuitMessage(0);


	return _returnVal;
}


//---------------------------------------------------------------------------------------
// App command to run the dialog
void CZ_ed2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//---------------------------------------------------------------------------------------
BOOL    CZ_ed2App::Dbg_ResolveCurDir()
{
    char sPath[_MAX_PATH];
    _getcwd(sPath,_MAX_PATH);

    _homeDir   = GetProfileString( _T("DIR"), _T("HOME"),_T(""));
    if(_homeDir.size() && _homeDir.find("_bin")!=-1)
    {
        if(_homeDir != sPath)
        {
            //change home-dir
            if(strstr(sPath,"_bin"))
            {
        	    _curDir  = sPath;
                _instDir = sPath; 
                _homeDir = sPath;
                WriteProfileString( _T("DIR"), _T("HOME"), sPath);
                _RegisterShellFileTypes(sPath);
            }
            
            else
            {
#ifndef _DEBUG
                AfxMessageBox(MKSTR("Working Directory Changed To:\n %s \n Working directory has to be named '_bin'",_homeDir.c_str()), MB_OK|MB_ICONINFORMATION);
#endif //
                chdir(_homeDir.c_str());
                _RegisterShellFileTypes(_homeDir.c_str());
    	        _curDir  = _homeDir;
                _instDir = _homeDir;
            }
        }
        else
        {
    	    _curDir  = _homeDir;
            _instDir = _homeDir;
        }
        return 1;
    }
    if(!strstr(sPath,"\\_bin"))
    {
        _unlink(sz_PROFILE);
        AfxMessageBox("Getic cannot run properly from a shortcut !", MB_OK|MB_ICONINFORMATION);
        
        return 0;
    }
	_curDir  = sPath;
    _instDir = _curDir;
    if(_homeDir.size()==0)
    {
        //AfxMessageBox(MKSTR("Home Directory was set to:\n %s \n For setting see:  My Settings->Preferences", _homeDir.c_str()), MB_OK|MB_ICONINFORMATION);
        _homeDir = _curDir;
    }
    WriteProfileString( _T("DIR"), _T("HOME"), _homeDir.c_str());
    _RegisterShellFileTypes(sPath);
    return 1;
}

//---------------------------------------------------------------------------------------
// CZ_ed2App message handlers
int CZ_ed2App::ExitInstance() 
{
    _brushtask.Stop();

    for(int i=0;i<32;i++)
	    END_WAIT("");


    DestroyCursor(_compilingCur        );
    DestroyCursor(_cursors[CUR_SCROLL] );
    DestroyCursor(_cursors[CUR_MOVE]   );
    DestroyCursor(_cursors[CUR_SCALE]  );
    DestroyCursor(_cursors[CUR_ROTATE] );
    DestroyCursor(_cursors[CUR_NS]     );
    DestroyCursor(_cursors[CUR_EW]     );
    DestroyCursor(_cursors[CUR_NWSE]   );
    DestroyCursor(_cursors[CUR_NESW]   );


    DestroyAcceleratorTable(_hScriptEditAccel);
    DestroyAcceleratorTable(_h3dEditAccel);

    _butBmp.DeleteObject();
    _msgBoxBitmap.DeleteObject();
    
    _slpEditBrush.DeleteObject();
    _slpEditBitmap.DeleteObject();

    _txtEditBitmap.DeleteObject();
    _txtEditBrush.DeleteObject();

    DestroyIcon(_colexpic[0]);
    DestroyIcon(_colexpic[1]);


    WriteCustomProfileSettings();
   
#ifdef COM_WAY
	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
		CoUninitialize();
	}
#endif //

    ::UnregisterClass(_cName.c_str() ,m_hInstance);
    ::UnregisterClass("Getic_View2d3d",m_hInstance);
    UnSuperClassAll();

    this->m_pszProfileName = m_pszProfileNameOLD;
	return CWinApp::ExitInstance();
}

//---------------------------------------------------------------------------------------
void CZ_ed2App::CreatePaths()
{
    //preserve curent wdir
	REDIR();  

    int dirs = GetProfileInt("MAIN", "FIRSTRUN",  0);    
    if(0 == dirs)
    {
        AfxMessageBox("Getic will create under curent working directory few folders !",MB_OK|MB_ICONINFORMATION);
        AfxMessageBox("complled bsp maps are generated in compiled folder",MB_OK|MB_ICONINFORMATION);
        AfxMessageBox( MKSTR("Configuration File Location: %s", this->m_pszProfileName), MB_OK|MB_ICONINFORMATION);
        mkdir("temp");
        mkdir("res");
        mkdir("compiled");
        mkdir("textures");
        mkdir("server_levels");
        mkdir("export");

        char cd[_MAX_PATH];
        _getcwd(cd,_MAX_PATH);
        WriteProfileString( _T("DIR"), _T("HOME"),cd);
        WriteProfileInt("MAIN", "FIRSTRUN",1);
    }
}

#ifdef COM_WAY

//---------------------------------------------------------------------------------------	
CZedmapModule _Module;

//---------------------------------------------------------------------------------------
BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_IGetic, CIGetic)
//OBJECT_ENTRY(CLSID_GeticEditor, CGeticEditor)
END_OBJECT_MAP()

//---------------------------------------------------------------------------------------
LONG CZedmapModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

//---------------------------------------------------------------------------------------
LONG CZedmapModule::Lock()
{
	AfxOleLockApp();
	return 1;
}

//---------------------------------------------------------------------------------------
LPCTSTR CZedmapModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}


#endif //#ifdef COM_WAY
//---------------------------------------------------------------------------------------
void CZ_ed2App::LoadCustomProfileSettings()
{
    

    GFrustCull = Str2Int(GetProfileString("GDLG","FRUSTCUL"),0);
    BShowLCirc= Str2Int(GetProfileString("GDLG","SHOWLC"),1);
    BShowTrPolys= Str2Int(GetProfileString("GDLG","SHOWTP"),1);
    GEpsilon    = Str2Real(GetProfileString("GDLG","EPSIL"),.5);
    GGridSnap   = Str2Int(GetProfileString("GDLG","SNAP"),32);
    GDetEnabled = Str2Int(GetProfileString("GDLG","DET"),0);
    GMinDetDim  = Str2Int(GetProfileString("GDLG","DETMINDIM"),64);  
    GMaxDetDim  = Str2Int(GetProfileString("GDLG","DETMAXDIM"),100);
    GCullTerrCell= Str2Int(GetProfileString("GDLG","CULLTERRAIN"),1);
    BShowCb      = Str2Int(GetProfileString("GDLG","SHOWCB"),0);
    GDisableGrpCSG = Str2Int(GetProfileString("GDLG","NOCSGBROUPS"),1);
    GKeepProjections = Str2Int(GetProfileString("GDLG","KEEPPROJECTION"),0);
    GMinDetPolys= Str2Int(GetProfileString("GDLG","DETMINPOLY"),32);
    GBackFace   = Str2Int(GetProfileString("GDLG","GBACKFACE"),32);
    GLight      = Str2Int(GetProfileString("GDLG","GLIGHT"),0);
    GShowCuts   = Str2Int(GetProfileString("GDLG","GCUTS"),0);
    GMeters     = Str2Real(GetProfileString("GDLG","CUNITS"),1);
	GForceBSP	= Str2Int(GetProfileString("GDLG","FORCEBSP"),0);
    GCarving    = Str2Int(GetProfileString("GDLG","BYCARVING"),0);
	GUpdateImm	= Str2Int(GetProfileString("GDLG","3DLIVEUPDATE"),1);
    GAutoSave   = Str2Int(GetProfileString("GDLG","AUTOSAVE"),0);
    GAnimCompilation  = Str2Int(GetProfileString("GDLG","ANIMATEC"),0);
    GSelTexFace  = Str2Int(GetProfileString("GDLG","FACESELTEX"),0);
    BrushTask  = Str2Int(GetProfileString("GDLG","BRUSHTASK"),0);
    TerLeafCelTiles = Str2Int(GetProfileString("GDLG","TTLEAFTILES"),16);
    TerrTiles       = Str2Int(GetProfileString("GDLG","TERTILES"),128);
    SnapToXY        = Str2Int(GetProfileString("GDLG","SNAPTOXY"),32);

    if(SnapToXY < 8) SnapToXY = 8;
    if(SnapToXY > 256) SnapToXY = 256;

    if(TerLeafCelTiles>16 || TerLeafCelTiles < 8)
    {
        TerLeafCelTiles=16;
    }
    if(!IsPowOf2(TerLeafCelTiles))
    {
        TerLeafCelTiles=16;
    }
    if(TerrTiles>128 || TerrTiles < 32)
    {
        TerrTiles=64;
    }
    if(TerrTiles < TerLeafCelTiles)
    {
        TerLeafCelTiles = 16;
        TerrTiles       = 64;
    }
    if(!IsPowOf2(TerrTiles))
    {
        TerrTiles=64;
    }



#ifdef _FREE
GLight = 0;
GAutoSave = 0;
#endif //

    

    _docsDir    = GetProfileString( _T("DIR"), _T("DOCS"),_T("maps\\"));   
	if(_docsDir.length() == 0)_docsDir = _T("maps\\");
    
	_bspDir     = GetProfileString( _T("DIR"), _T("BSPS"),_T("compiled\\"));
	if(_bspDir.length() == 0)_bspDir = _T("compiled\\");
    
	_texDir     = GetProfileString( _T("DIR"), _T("TEXTURES"),_T("res\\"));
	if(_texDir.length() == 0)_texDir = _T("res\\"); 
    
	_sndDir     = GetProfileString( _T("DIR"), _T("SOUNDS"),_T("res\\"));
	if(_sndDir.length() == 0)_sndDir = _T("res\\");

    _prefsDir   = GetProfileString( _T("DIR"), _T("PREFS"),_T("res\\"));
	if(_prefsDir.length() == 0)_prefsDir = _T("res\\");

    _scriptsDir = GetProfileString( _T("DIR"), _T("SCRIPTS"),_T("res\\"));
	if(_scriptsDir.length() == 0)_scriptsDir = _T("res\\");

    _serverDir  = GetProfileString( _T("DIR"), _T("SERVER"),_T("export\\"));
	if(_serverDir.length() == 0)_serverDir = _T("export\\");

    _catFile    = GetProfileString( _T("FILE"), _T("ITMCAT"),_T("default.xsd"));
	if(_catFile.length() == 0)_catFile = _T("default.xsd");


     _dwpMakeTbar   = GetProfileInt("TOOLBARS", "MAKE_BAR_DOCK",  AFX_IDW_DOCKBAR_TOP   );
     _dwpSelectTbar = GetProfileInt("TOOLBARS", "SEL_BAR_DOCK",   AFX_IDW_DOCKBAR_TOP   );
     _dwpMoveTbar   = GetProfileInt("TOOLBARS", "MOVE_BAR_DOCK",  AFX_IDW_DOCKBAR_TOP   );
     _dwpAlignTbar  = GetProfileInt("TOOLBARS", "ALIGN_BAR_DOCK", AFX_IDW_DOCKBAR_RIGHT );
     _dwpViewsTbar  = GetProfileInt("TOOLBARS", "VIEWS_BAR_DOCK", AFX_IDW_DOCKBAR_BOTTOM);
     _dwpCsgTbar    = GetProfileInt("TOOLBARS", "CSG_BAR_DOCK",   AFX_IDW_DOCKBAR_BOTTOM);
     _dwpBuildTbar  = GetProfileInt("TOOLBARS", "BUILD_BAR_DOCK", AFX_IDW_DOCKBAR_TOP    );

     UINT retreived = sizeof(RECT);
     BYTE* pBuff =0;

     GetProfileBinary("TOOLBARS", "CSG_BAR_L",     &pBuff, &retreived);
     if(pBuff)
         ::memcpy(&_rtViewsTbar, pBuff, sizeof(RECT));
     delete[] pBuff; pBuff=0;

    GetProfileBinary("TOOLBARS", "VIEWS_BAR_L",   &pBuff, &retreived);
    if(pBuff)
        ::memcpy(&_rtViewsTbar, pBuff, sizeof(RECT));
     delete[] pBuff; pBuff=0;

    GetProfileBinary("TOOLBARS", "ALIGN_BAR_L",   &pBuff, &retreived);
    if(pBuff)
        ::memcpy(&_rtAlignTbar, pBuff, sizeof(RECT));
     delete[] pBuff; pBuff=0;

    GetProfileBinary("TOOLBARS", "MOVE_BAR_L",    &pBuff, &retreived);
    if(pBuff)
        ::memcpy(&_rtMoveTbar, pBuff, sizeof(RECT));
     delete[] pBuff; pBuff=0;

    GetProfileBinary("TOOLBARS", "SEL_BAR_L",     &pBuff, &retreived);
    if(pBuff)
        ::memcpy(&_rtSelectTbar, pBuff, sizeof(RECT));
     delete[] pBuff; pBuff=0;

    GetProfileBinary("TOOLBARS", "MAKE_BAR_L",    &pBuff, &retreived);
    if(pBuff)
        ::memcpy(&_rtMakeTbar, pBuff, sizeof(RECT));
     delete[] pBuff; pBuff=0;

    GetProfileBinary("TOOLBARS", "BUILD_BAR_L",   &pBuff, &retreived);
    if(pBuff)
        ::memcpy(&_rtBuildTbar, pBuff, sizeof(RECT));
     delete[] pBuff; pBuff=0;


    _bShowMakeTbar  = GetProfileInt("TOOLBARS", "MAKE_BAR_VIS",  1);
    _bShowSelectTbar= GetProfileInt("TOOLBARS", "SEL_BAR_VIS",   1);
    _bShowMoveTbar  = GetProfileInt("TOOLBARS", "MOVE_BAR_VIS",  1);
    _bShowAlignTbar = GetProfileInt("TOOLBARS", "ALIGN_BAR_VIS", 0);
    _bShowViewsTbar = GetProfileInt("TOOLBARS", "VIEWS_BAR_VIS", 0);
    _bShowCsgTbar   = GetProfileInt("TOOLBARS", "CSG_BAR_VIS",   0);
    _bShowBuildTbar = GetProfileInt("TOOLBARS", "BUILD_BAR_VIS", 1);
    _bShowCurvesbar = GetProfileInt("TOOLBARS", "BUILD_CURVE_VIS", 0);

}

//---------------------------------------------------------------------------------------
void CZ_ed2App::WriteCustomProfileSettings()
{
    WriteProfileString("GDLG","FRUSTCUL",MKSTR("%d",GFrustCull));
    WriteProfileString("GDLG","SHOWLC",MKSTR("%d",BShowLCirc));
    WriteProfileString("GDLG","SHOWTP",MKSTR("%d",BShowTrPolys));
    WriteProfileString("GDLG","EPSIL",MKSTR("%f",GEpsilon));
    WriteProfileString("GDLG","SNAP",MKSTR("%d",GGridSnap));
    WriteProfileString("GDLG","DET",MKSTR("%d",GDetEnabled));
    WriteProfileString("GDLG","DETMINDIM",MKSTR("%d",GMinDetDim));  
    WriteProfileString("GDLG","DETMAXDIM",MKSTR("%d",GMaxDetDim));
    WriteProfileString("GDLG","CULLTERRAIN",MKSTR("%d",GCullTerrCell));
    WriteProfileString("GDLG","SHOWCB",MKSTR("%d",BShowCb));

    WriteProfileString("GDLG","NOCSGBROUPS",MKSTR("%d",GDisableGrpCSG));
    WriteProfileString("GDLG","KEEPPROJECTIONS",MKSTR("%d",GKeepProjections));

    WriteProfileString("GDLG","DETMINPOLY",MKSTR("%d",GMinDetPolys));
    WriteProfileString("GDLG","GBACKFACE",MKSTR("%d",GBackFace));
    WriteProfileString("GDLG","GLIGHT",MKSTR("%d",GLight));
    WriteProfileString("GDLG","GCUTS",MKSTR("%d",GShowCuts));
    WriteProfileString("GDLG","CUNITS",MKSTR("%2.4f",GMeters));
	WriteProfileString("GDLG","FORCEBSP",MKSTR("%d",GForceBSP));
    WriteProfileString("GDLG","BYCARVING",MKSTR("%d",GCarving));
	WriteProfileString("GDLG","3DLIVEUPDATE",MKSTR("%d",GUpdateImm));
    WriteProfileString("GDLG","AUTOSAVE",MKSTR("%d",GAutoSave));
    WriteProfileString("GDLG","ANIMATEC",MKSTR("%d",GAnimCompilation));
    WriteProfileString("GDLG","FACESELTEX",MKSTR("%d",GSelTexFace));
    WriteProfileString("GDLG","BRUSHTASK",MKSTR("%d",BrushTask  ));
    WriteProfileString("GDLG","TTLEAFTILES",MKSTR("%d",TerLeafCelTiles));
    WriteProfileString("GDLG","TERTILES",MKSTR("%d",TerrTiles));
    WriteProfileString("GDLG","SNAPTOXY",MKSTR("%d",SnapToXY));
    

    
    

    WriteProfileString( _T("DIR"), _T("DOCS"),    _docsDir.c_str());
    WriteProfileString( _T("DIR"), _T("BSPS"),    _bspDir.c_str());
    WriteProfileString( _T("DIR"), _T("TEXTURES"),_texDir.c_str());
    WriteProfileString( _T("DIR"), _T("SOUNDS"),  _sndDir.c_str());
    WriteProfileString( _T("DIR"), _T("PREFS"),   _prefsDir.c_str());
    WriteProfileString( _T("DIR"), _T("SCRIPTS"), _scriptsDir.c_str());
    WriteProfileString( _T("DIR"), _T("COMPILER"), _compilerDir.c_str());
    WriteProfileString( _T("DIR"), _T("SERVER"), _serverDir.c_str());
    WriteProfileString( _T("FILE"), _T("ITMCAT"), _catFile.c_str());

    WriteProfileInt("TOOLBARS", "MAKE_BAR_DOCK",    _dwpMakeTbar   );
    WriteProfileInt("TOOLBARS", "SEL_BAR_DOCK",     _dwpSelectTbar );
    WriteProfileInt("TOOLBARS", "MOVE_BAR_DOCK",    _dwpMoveTbar   );
    WriteProfileInt("TOOLBARS", "ALIGN_BAR_DOCK",   _dwpAlignTbar  );
    WriteProfileInt("TOOLBARS", "VIEWS_BAR_DOCK",   _dwpViewsTbar  );
    WriteProfileInt("TOOLBARS", "CSG_BAR_DOCK",     _dwpCsgTbar    );
    WriteProfileInt("TOOLBARS", "BUILD_BAR_DOCK",   _dwpBuildTbar  );

    WriteProfileBinary("TOOLBARS", "CSG_BAR_L",   (BYTE*)&_rtCsgTbar, sizeof(RECT));
    WriteProfileBinary("TOOLBARS", "VIEWS_BAR_L", (BYTE*)&_rtViewsTbar, sizeof(RECT));
    WriteProfileBinary("TOOLBARS", "ALIGN_BAR_L", (BYTE*)&_rtAlignTbar, sizeof(RECT));
    WriteProfileBinary("TOOLBARS", "MOVE_BAR_L",  (BYTE*)&_rtMoveTbar, sizeof(RECT));
    WriteProfileBinary("TOOLBARS", "SEL_BAR_L",   (BYTE*)&_rtSelectTbar, sizeof(RECT));
    WriteProfileBinary("TOOLBARS", "MAKE_BAR_L",  (BYTE*)&_rtMakeTbar, sizeof(RECT));
    WriteProfileBinary("TOOLBARS", "BUILD_BAR_L", (BYTE*)&_rtBuildTbar, sizeof(RECT));


    WriteProfileInt("TOOLBARS", "MAKE_BAR_VIS",   _bShowMakeTbar );
    WriteProfileInt("TOOLBARS", "SEL_BAR_VIS",    _bShowSelectTbar );
    WriteProfileInt("TOOLBARS", "MOVE_BAR_VIS",   _bShowMoveTbar );
    WriteProfileInt("TOOLBARS", "ALIGN_BAR_VIS",  _bShowAlignTbar );
    WriteProfileInt("TOOLBARS", "VIEWS_BAR_VIS",  _bShowViewsTbar );
    WriteProfileInt("TOOLBARS", "CSG_BAR_VIS",    _bShowCsgTbar );
    WriteProfileInt("TOOLBARS", "BUILD_BAR_VIS",  _bShowBuildTbar );
    WriteProfileInt("TOOLBARS", "BUILD_CURVE_VIS",  _bShowCurvesbar );

/*
    WriteProfileInt("CLRPREF", "CLR_NORMAL",CLR_NORMAL);
    WriteProfileInt("CLRPREF","CLR_DARK",CLR_DARK)
    WriteProfileInt("CLRPREF","CLR_CUT",CLR_CUT)
    WriteProfileInt("CLRPREF","CLR_WHITE",CLR_WHITE)
    WriteProfileInt("CLRPREF","CLR_SELECT",CLR_SELECT);
    WriteProfileInt("CLRPREF","CLR_SELECTDIMED",CLR_SELECTDIMED);
    WriteProfileInt("CLRPREF","CLR_SELBOX",CLR_SELBOX);
    WriteProfileInt("CLRPREF","CLR_SELBOX_TCH",CLR_SELBOX_TCH);
    WriteProfileInt("CLRPREF","CLR_CAMSTART",CLR_CAMSTART);
    WriteProfileInt("CLRPREF","CLR_CAMEND",CLR_CAMEND);
    WriteProfileInt("CLRPREF","CLR_CAMFRUST",CLR_CAMFRUST);
    WriteProfileInt("CLRPREF","CLR_SCRSURFACE",CLR_SCRSURFACE);
    WriteProfileInt("CLRPREF","CLR_CUT3DBRUSH",CLR_CUT3DBRUSH);
    WriteProfileInt("CLRPREF","CLR_PRJDBRUSH",CLR_PRJDBRUSH);
    WriteProfileInt("CLRPREF","CLR_BLOOR",CLR_BLOOR);
    WriteProfileInt("CLRPREF","CLR_CUT3DBRUSHREV", CLR_CUT3DBRUSHREV);
    WriteProfileInt("CLRPREF","CLR_PORTALS", CLR_PORTALS);
    WriteProfileInt("CLRPREF","CLR_PORTALS1", CLR_PORTALS1);
    WriteProfileInt("CLRPREF","CLR_PORTALSLINE", CLR_PORTALSLINE);
    WriteProfileInt("CLRPREF","CLR_DETAIL", CLR_DETAIL);
    WriteProfileInt("CLRPREF","CLR_TERRAIN", CLR_TERRAIN);
    WriteProfileInt("CLRPREF","CLR_DETAILTRIG", CLR_DETAILTRIG);
    WriteProfileInt("CLRPREF","CLR_LIGHTBULB", CLR_LIGHTBULB);
    WriteProfileInt("CLRPREF","CLR_SKYDOM", CLR_SKYDOM);
    WriteProfileInt("CLRPREF","CLR_MOTION", CLR_MOTION);
    WriteProfileInt("CLRPREF","CLR_TRIGGER", CLR_TRIGGER);
    WriteProfileInt("CLRPREF","CLR_FACEDETAIL", CLR_FACEDETAIL);
*/
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
#include "atlbase.h"
//---------------------------------------------------------------------------------------
#ifdef COM_WAY
void CZ_ed2App::FindPlugIns()
{
 
    CComPtr<ICatInformation>    ci;
    HRESULT hr = ci.CoCreateInstance(CLSID_StdComponentCategoriesMgr);
    if(FAILED(hr)) return;
    CComPtr<IEnumCLSID> spEci;

    int          cid = 1;
    ULONG        reg = 1;
    CATID        guidCAT[1];
    guidCAT[0] = CatGuid;
    
    hr = ci->EnumClassesOfCategories(cid, guidCAT, 0, 0,  &spEci);

    if(SUCCEEDED(hr))
    {
        ULONG nRet      = 0;
        
        CLSID rgclsid;
        do 
        {
            ULONG cActual = 0;
            hr   = spEci->Next(1, &rgclsid, &cActual);

            if(SUCCEEDED(hr))
            {
                IGeticPlug* plg;
                hr = ::CoCreateInstance(rgclsid, 0, CLSCTX_INPROC_SERVER, IID_IGeticPlug, (void**)&plg);

                CComBSTR    bs;
                EPLUGTYPE   et;
                hr = plg->GetMenuStringAndType(&bs, &et);
            }

        }while(hr == S_OK);
    }
}

#include "PlugInCat.h"
void CZ_ed2App::RegisterUnregisterServer(BOOL breg)
{

    PlugInCat   pg;
    if(!breg)
    {
        pg.DeleteCategory();
        _Module.UpdateRegistryFromResource(IDR_ZEDMAP, FALSE);
        _Module.UnregisterServer(TRUE); 
        return;
    }
    _Module.UpdateRegistryFromResource(IDR_ZEDMAP, TRUE);
    _Module.RegisterServer(TRUE);

}

#else
void CZ_ed2App::FindPlugIns()
{
    REDIR();
    CDirChange      to("plugins\\");
	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	

    to.UseObj();

	if((hfFind=FindFirstFile("*.dll", &fDATA)) != INVALID_HANDLE_VALUE)
	{
		if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
	        //
            CheckPlug(fDATA.cFileName);
		}
		while(FindNextFile(hfFind, &fDATA)!=0)
		{
			if(fDATA.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
               // fDATA.cFileName
                CheckPlug(fDATA.cFileName);
            }
		}
	}
	FindClose(hfFind);  
}

void CZ_ed2App::CheckPlug(const char* dllName)
{
    PlugInDll<IGeticPlug>   thePlug(dllName);
    CString                 cs;

    CMenu* pMenuM = AfxGetMainWnd()->GetMenu();
    CMenu* pMenu;
    pMenuM = pMenuM->GetSubMenu(0);
        
    if(thePlug.IsValid())
    {
        char  pszMenu[128]={0};
        DWORD  typo = 0;
        if(0==thePlug->GetMenuStringAndType(pszMenu, &typo))
        {
            if(typo & PLUG_IMPORTER && pMenuM)
            {
                pMenu = pMenuM->GetSubMenu(8);
                if(pMenu)
                {
                    
                    UINT uid = _cmdPlugIn;
                    pMenu->InsertMenu(0,MF_STRING|MF_BYPOSITION, uid, pszMenu);
                    _plugins.push_back(SPlugIn(dllName,uid));
                }
            }
            if(typo & PLUG_EXPORTER && pMenuM)
            {
                pMenu = pMenuM->GetSubMenu(9);
                if(pMenu)
                {
                    UINT uid = _cmdPlugIn+1;
                    pMenu->InsertMenu(0,MF_STRING|MF_BYPOSITION, uid, pszMenu);
                    _plugins.push_back(SPlugIn(dllName,uid));
                }
            }
            _cmdPlugIn+=2;
        }
        thePlug.Free();
    }
}




#endif //#ifndef COM_WAY



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Marius C. Hook for message box. For colloring it and ownerdraw the buttons on it
// required the bitmaps in resource as "OK*" "CANCEL*" "YES*" "NO*" ... where * is D,F,U,X,
static    FARPROC g_lpMsgBoxProc;          // Message box window procedure
static    HHOOK   g_hhookCBT;              
static    LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
static    LRESULT CALLBACK MsgBoxSubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int CZ_ed2App::DoFileDialog(CFileDialog& d)
{
   // g_hhookCBT = SetWindowsHookEx(WH_CBT, HookProc, theApp.m_hInstance, GetCurrentThreadId());
    
    int nResult = d.DoModal();
   // UnhookWindowsHookEx(g_hhookCBT);
	return nResult;
}

//---------------------- | overwrite AfxMessagbox |---------------------------------------------
int CZ_ed2App::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt) 
{
    int i;
	for(int i=0;i<8;i++)
		END_WAIT("");
    if(1)//_msgBoxOn)
    {
        return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
    }

    _msgBoxOn = 1;
    
    int		 nResult;
	for(i=0;i<8;i++)
		END_WAIT("");
    
    g_hhookCBT = SetWindowsHookEx(WH_CBT, HookProc, theApp.m_hInstance, GetCurrentThreadId());
    nResult = CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
    UnhookWindowsHookEx(g_hhookCBT);
    _msgBoxOn = 0;
	return nResult;

//    return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
}

//---------------------- | MessageBox Hook proc|-----------------------------------------------
LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
   LPCBT_CREATEWND  lpcbtcreate;
   static HWND		hwndMsgBox = NULL;
   static FARPROC	lpMsgBoxSubClassProc;
   static BOOL		bSubClass = FALSE;
   
   if (nCode < 0)
       return CallNextHookEx(g_hhookCBT, nCode, wParam, lParam); 
   
   // is coming twice here
   if (nCode == HCBT_CREATEWND){
       //  prepare in the first shoot
       lpcbtcreate = (LPCBT_CREATEWND)lParam;
       if (WC_DIALOG == lpcbtcreate->lpcs->lpszClass){
           hwndMsgBox = (HWND)wParam;
           bSubClass = TRUE;      
       }//  do the subclassing in second
       else if (bSubClass){
           g_lpMsgBoxProc       = (FARPROC)SetWindowLong(hwndMsgBox, GWL_WNDPROC,  (LONG)MsgBoxSubClassProc);
           bSubClass = FALSE;
       }
   }
   // restore the original
   else if (nCode == HCBT_DESTROYWND && (HWND)wParam == hwndMsgBox){
       SetWindowLong(hwndMsgBox, GWL_WNDPROC, (LONG)g_lpMsgBoxProc);
       hwndMsgBox = NULL;      
   }   
   return 0;          
}


//---------------------- | extends to clean up bitmaps|-----------------------------------------
class CBitmapButtonMb : public CColorButon
{
public:
    CBitmapButtonMb(){m_hWnd=0;}
    virtual ~CBitmapButtonMb(){
        Clean();
    }
    void Clean(){
    }

};

static    CBitmapButtonMb gbmpButt[16];

//---------------------- | actual procedure of subclassed dlgbox|------------------------------
LRESULT CALLBACK MsgBoxSubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static    int             buts         = 0;
    static    HWND            hWnds[16]    = {0};
    
    // firrst call after is beeing subclassed
    if(msg==WM_INITDIALOG) {
        buts = 0;
        // scan all butons and make them ownerdraw
        for(int i=0; i < 8;i++){
            HWND hb = GetDlgItem(hwnd, i);
            if(hb){
                char s[32];
#ifdef _DEBUG
                
                GetWindowText(hb,s,32);
#endif //
                GetClassName(hb,s,32);
                if(!stricmp(s,"button")){
                    SetWindowLong(hb,GWL_STYLE, GetWindowLong(hb,GWL_STYLE)|BS_OWNERDRAW);
                    if(gbmpButt[i].m_hWnd==0)
                    {
                        gbmpButt[i].SubclassDlgItem(i, CWnd::FromHandle(hwnd));
                    }
                    hWnds[buts++] = hb;
                }
                
            }
        }
    }


    // resend the message to buttons because MsgBoc def proc doesn't
    if(msg == WM_DRAWITEM){
        for(int i=0;i<8;i++){
            if(gbmpButt[i].m_hWnd)
                gbmpButt[i].SendMessage(WM_DRAWITEM,wParam,lParam);
        }
    }

    // do the background stuff
    if (msg == WM_CTLCOLORDLG || 
        msg == WM_CTLCOLORSTATIC){
        SetBkMode((HDC)wParam, TRANSPARENT);
        return (long)(HBRUSH)theApp._brush[1];
    }

    //clean up on first command because anyway the msgbox is dissmissed
    if(msg == WM_COMMAND){
        for(int i=0;i<8;i++){
            if(gbmpButt[i].m_hWnd){
                SetWindowLong(gbmpButt[i].m_hWnd, GWL_STYLE, (GetWindowLong(gbmpButt[i].m_hWnd,GWL_STYLE) & ~BS_OWNERDRAW));
                gbmpButt[i].Clean();
                gbmpButt[i].UnsubclassWindow();
                gbmpButt[i].m_hWnd = 0;
            }
        }
        EndDialog(hwnd,LOWORD(wParam));
    }
    return  CallWindowProc((WNDPROC)g_lpMsgBoxProc, hwnd, msg, wParam, lParam);
} 

void CZ_ed2App::OnShowcompiler() 
{
	if(0 == _pToolFrame)
	{
        if(_rectMiniTool.IsRectEmpty())
        {
            AfxGetMainWnd()->GetWindowRect(&_rectMiniTool);
        }
		_pToolFrame = new CToolFrame;
		_pToolFrame->Create(0,"Script Editor",WS_OVERLAPPEDWINDOW,_rectMiniTool, AfxGetMainWnd());
		_pToolFrame->ShowWindow(SW_SHOW);
		_pToolFrame->CreateTextView();
        _pToolFrame->SendMessage(WM_SETTEXT,0,(long)"Untitled.gs");
        _pToolFrame->BringWindowToTop();
	}
}

BOOL CZ_ed2App::PreTranslateMessage(MSG* pMsg) 
{
	if (_bToolOn == 0 && pMsg->message >= WM_KEYFIRST && 
						 pMsg->message <= WM_KEYLAST)
	{
		if (_h3dEditAccel != NULL)
		{
			if (::TranslateAccelerator(0, _h3dEditAccel, pMsg))
				return TRUE;
		}
        if(pMsg->message == WM_KEYDOWN)
        {
            switch(pMsg->wParam)
            {
                case VK_RETURN:
                    {
                        if(!p_wndFocused) 
                        {
                            BARDLG()->PostMessage(WM_COMMAND,IDC_BUTTON3);
                        }
                    }
                    break;
                case VK_ESCAPE:
                    {
                        if(!p_wndFocused && DOC()) 
                        {
                            AfxGetMainWnd()->PostMessage(WM_COMMAND,MN_ESCAPE);
                        }
                    }
                    break;
            }

            if(_keys[pMsg->wParam&0xFF]==0)
            {
                _keys[pMsg->wParam&0xFF] = 1;
                if(theApp.GetMainWnd())
                    theApp.GetMainWnd()->PostMessage(WM_SBARNOTY,(pMsg->wParam & 0xFF), 1);
            }
        }
        else if(pMsg->message == WM_KEYUP)
        {
            if(_keys[pMsg->wParam & 0xFF] != 0)
            {
                _keys[pMsg->wParam & 0xFF] = 0;
                if(theApp.GetMainWnd())
                    theApp.GetMainWnd()->PostMessage(WM_SBARNOTY,(pMsg->wParam & 0xFF), 0);
            }
        }

	}
	if(pMsg->message == WM_MBUTTONDOWN)
	{
		BARDLG()->BringWindowToTop();
	}
    if(WM_REMOVETEX == pMsg->message)
    {
        TexRef::RemoveTex((Htex*)&pMsg->wParam,1);
        return TRUE;
    }
	BOOL b = CWinApp::PreTranslateMessage(pMsg);
    return b;
}

void CZ_ed2App::UnregisterGLWindows()
{
    ::UnregisterClass(_cNameViews.c_str(),AfxGetInstanceHandle());
    ::UnregisterClass(_cName.c_str(),AfxGetInstanceHandle());

    CDrawOut:: UnregisterClass();
}

BOOL CZ_ed2App::RegisterGLWindows()
{

    CDrawOut::RegisterClass();

    WNDCLASS wndclsGL;
    ::memset(&wndclsGL, 0, sizeof(WNDCLASS));   // start with NULL defaults
    wndclsGL.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW|CS_CLASSDC;
    wndclsGL.lpfnWndProc   = ::DefWindowProc;
    wndclsGL.hInstance     = AfxGetInstanceHandle();
    wndclsGL.hIcon         = 0;
    wndclsGL.hbrBackground = (HBRUSH) 0;
    wndclsGL.lpszMenuName  = 0;
    wndclsGL.hCursor       = ::LoadCursor(0,IDC_CROSS);

    wndclsGL.lpszClassName = "GL_WIN_INNER";

    AfxRegisterClass(&wndclsGL);

    _cName = "GL_WIN_INNER"; //AfxRegisterWnd(CS_HREDRAW|CS_VREDRAW,0, 0);

    _cNameViews          = "Getic_View2d3d";
    WNDCLASS wndcls;
    ::memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults
    wndcls.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW|CS_CLASSDC;
    wndcls.lpfnWndProc   = ::DefWindowProc;
    wndcls.hInstance     = AfxGetInstanceHandle();
    wndcls.hIcon         = 0;
    wndcls.hbrBackground = (HBRUSH) 0;
    wndcls.lpszMenuName  = 0;
    wndcls.lpszClassName = _cNameViews.c_str();
    return AfxRegisterClass(&wndcls);

}

void CZ_ed2App::ExportHeader()
{
	HRSRC     hrsrc = FindResource(AfxGetResourceHandle(),
                                    MAKEINTRESOURCE(IDR_PLUGIN_HDR), 
                                    "GSCRIPT_HDR");
    if(hrsrc)
    {
        HGLOBAL hResLoad = LoadResource(AfxGetResourceHandle(), hrsrc);
        if(hResLoad)
        {
            void* pData = LockResource(hResLoad);
            if(pData)
            {
                FileWrap f;
                if(f.Open("geticplug.h","wb"))
                {
                    f.WriteStr((char*)pData);
                    f.Close();
                }
            }
        }
    }
    
}



BOOL CZ_ed2App::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    
	BOOL b = CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

    if(nID > _PLUG_MSG0+1)
    {
        return TRUE;
    }

    return b;
}

void CZ_ed2App::OnPlugInsCmd(UINT cmd)
{
    //store when scanning plugins assoc between cmd and plugin name dll to load it
    if(DOC())
    {
        vvector<SPlugIn>::iterator b = _plugins.begin();
        vvector<SPlugIn>::iterator e = _plugins.end();
        for(;b!=e;b++)
        {
            if(b->cmdid==cmd)
            {
                DOC()->InvokePlugIn(b->menuname.c_str(),cmd);
                break;
            }
        }
        
    }

}

void CZ_ed2App::CleanTexMan()
{
    TexRef::Clear();
}



WNDPROC owpdll = 0;
long ButProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
        case WM_CTLCOLORMSGBOX:     
        case WM_CTLCOLOREDIT:       
        case WM_CTLCOLORLISTBOX:    
        case WM_CTLCOLORBTN:        
        case WM_CTLCOLORDLG:        
        case WM_CTLCOLORSCROLLBAR:  
        case  WM_CTLCOLORSTATIC:     
        {
            return (long)(HBRUSH)theApp._brush[1];
        }
        break;
   }

    return CallWindowProc(owpdll,hw,msg,wp,lp);
}



void CZ_ed2App::SuperClassAll()
{
    WNDCLASS    wcb;
    HINSTANCE hi = AfxGetResourceHandle();
    GetClassInfo(0,"Button",&wcb);
   
    owpdll            = wcb.lpfnWndProc;
    wcb.hInstance     = hi;
    wcb.lpszClassName = "Static";
    wcb.lpfnWndProc   = (WNDPROC)ButProc;
    BOOL b = RegisterClass(&wcb);
    if(!b)
        owpdll=0;
}


void CZ_ed2App::UnSuperClassAll()
{
    if(owpdll)
    {
        HINSTANCE hi = AfxGetResourceHandle();
        UnregisterClass("Static",hi);
    }
}

void CZ_ed2App::PumpMessage(UINT imsg, UINT tout)
{
    MSG msg;
    tout = 10;
    for(UINT i=0; i < tout ;i++)
    {
        if(PeekMessage(&msg,0,0,0,PM_REMOVE))
            DispatchMessage(&msg);
    }
}

long GeticEditor::GetTextureInfo(_Plg_Texture* pt)
{
    TexHandler      th;
    char            local[_MAX_PATH] = {0};

    pt->pBuffer     = 0;
    pt->dims[0]     = 0;
    pt->dims[1]     = 0;
    pt->bpp         = 0;

    _getcwd(local,_MAX_PATH);

    if(pt->filename[0])
    {
        map<string, _Plg_Texture>::iterator it = _gc.find(pt->filename);
        if(it != _gc.end())
        {
            pt->dims[0] = _gc[pt->filename].dims[0];
            pt->dims[1] = _gc[pt->filename].dims[1];
            pt->bpp     = _gc[pt->filename].bpp    ;
            pt->pBuffer = _gc[pt->filename].pBuffer;
         
            return 0;
        }

        if(!::strchr((const char*)pt->filename, '.'))
        {
            ::sprintf(local,"%s.tga", pt->filename);
            if(th.LoadThisFile(local,0))
            {
                pt->dims[0] = th.n_x;
                pt->dims[1] = th.n_y;
                pt->bpp     = th.n_bpp;      
                if(pt->flags & WANT_TEX_BUFEER)
                {
                    pt->pBuffer = new BYTE[th.n_x* th.n_y * th.n_bpp]; 
                    if(pt->pBuffer)
                    {
                        ::memcpy(pt->pBuffer, th.Buffer(), th.n_x* th.n_y * th.n_bpp);
                    }
                }
                _gc[pt->filename] = *pt;
                return 0;
            }
            
            ::sprintf(local,"%s.bmp", pt->filename);
            if(th.LoadThisFile(local,0))
            {
                pt->dims[0] = th.n_x;
                pt->dims[1] = th.n_y;
                pt->bpp     = th.n_bpp;      
                if(pt->flags & WANT_TEX_BUFEER)
                {
                    pt->pBuffer = new BYTE[th.n_x* th.n_y * th.n_bpp]; 
                    if(pt->pBuffer)
                    {
                        ::memcpy(pt->pBuffer, th.Buffer(), th.n_x* th.n_y * th.n_bpp);
                    }
                }
                _gc[pt->filename] = *pt;
                return 0;
            }

            ::sprintf(local,"%s.jpg", pt->filename);
            if(th.LoadThisFile(local,0))
            {
                pt->dims[0] = th.n_x;
                pt->dims[1] = th.n_y;
                pt->bpp     = th.n_bpp;      
                if(pt->flags & WANT_TEX_BUFEER)
                {
                    pt->pBuffer = new BYTE[th.n_x* th.n_y * th.n_bpp]; 
                    if(pt->pBuffer)
                    {
                        ::memcpy(pt->pBuffer, th.Buffer(), th.n_x* th.n_y * th.n_bpp);
                    }
                }
                _gc[pt->filename] = *pt;
                return 0;
            }
        }
    }
    return -1;  // error cannot load texture
}

void  GeticEditor::Clear()
{
    for(map<string, _Plg_Texture>::iterator it = _gc.begin();
        it != _gc.end(); it++)
    {
        if((it->second).pBuffer)
        {
            delete[] (it->second).pBuffer;
        }
    }
}



void CheckGbtx()
{
    CoInitialize(0);
    do{
        do{
            CComPtr<Igbtview> gbt;
            if(!SUCCEEDED(gbt.CoCreateInstance(CLSID_Cgbtview)))
            {
                WinExec("regsvr32.exe /s xmoge.dll",0);
            }
        }while(0);
    }while(0);
    CoUninitialize();
}

static  BOOL _RegSetKey(LPCTSTR lpszKey, 
                        LPCTSTR lpszValue, 
                        LPCTSTR lpszValueName=0)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(char)) != ERROR_SUCCESS)
		{
			return FALSE;
		}
		return TRUE;
	}
	HKEY hKey;

	if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
	{
		LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
			(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(char));

		if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
			return TRUE;
	}
	return FALSE;
}


void _RegisterShellFileTypes(LPCTSTR pathNoSlash)
{

	CString strTemp;
    CString csAppName        = MKSTR("%s\\moge.exe",pathNoSlash) ;
	CString icIdx;

	icIdx.Format(",%d", 4);

    CString defCl = csAppName;
    defCl         += icIdx;


    CString strFilterExt=".gs", 
            strFileTypeId="GBT.Script", 
            strFileTypeName="Script";

    strTemp.Format("%s\\DefaultIcon", (LPCTSTR)strFileTypeId);
    _RegSetKey(strTemp, csAppName+icIdx);


    LONG lSize = _MAX_PATH * 2;

	strTemp.Format("%s\\Shell", (LPCTSTR)strFilterExt);
	_RegSetKey(strTemp, "", 0);

	strTemp.Format("%s\\Shell\\open", (LPCTSTR)strFilterExt);
	_RegSetKey(strTemp, "", 0);

	strTemp.Format("%s\\Shell\\open\\command\\", (LPCTSTR)strFilterExt);
    csAppName+=" \"%1\"";
	_RegSetKey(strTemp, csAppName, 0);

	strTemp.Format("%s\\OpenWithList\\'%s'", (LPCTSTR)strFilterExt, "moge.exe");
	_RegSetKey(strTemp, "", 0);

	strTemp.Format("%s\\Def", (LPCTSTR)strFilterExt);
	_RegSetKey(strTemp, csAppName, 0);
}

void CZ_ed2App::HomeDir(const char* hd)
{
    if(hd && strlen(hd)>2)
    {
        _homeDir=hd;
    }
    else
    {
        char cd[256] = {0};
        _getcwd(cd,256);
        _homeDir = cd;
        mkdir(MKSTR("%s\\res",cd));
    }
}
