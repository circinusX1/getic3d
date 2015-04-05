// z-edmap.h : main header file for the Z_ED2 application
//

#if !defined(AFX_Z_ED2_H__E1604506_B2FF_4294_90AB_56AD88B6A276__INCLUDED_)
#define AFX_Z_ED2_H__E1604506_B2FF_4294_90AB_56AD88B6A276__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"       // main symbols
#include <deque>
#include "geticplugbsp.h"
#include "maskedit.h"       // main symbols
#include "basecont.h"
#include "basethread.h"
#include "atlbase.h"       // main symbols
#ifdef COM_WAY
#include "Z_edmap.h"
#endif //COM_WAY
#include "..\\_include\\geticplug.h"
#include "tasks.h"
#include "brush.h"

#define WM_REDRAW   (WM_USER+1000)
#define WM_DIRTY    (WM_USER+1001)
#define WM_KEYTAB   (WM_USER+1002)
#define WM_SBARNOTY (WM_USER+1003)
//-----------------------------------------------------------------------------
// bridge from external plugins and editor
class GeticEditor : public IGeticEditor
{
public:
    long _stdcall GetTextureInfo(_Plg_Texture* pt);
    void  Clear();
    map<string, _Plg_Texture>  _gc;
};

//---------------------------------------------------------------------------------------
// CZ_ed2App:
// See z_ed2.cpp for the implementation of this class
//

class CToolFrame;
class CZ_ed2App : public CWinApp
{
public:
	CZ_ed2App();
    ~CZ_ed2App();
    
    
    void DocsDir(const TCHAR* psz)        
    {
        _docsDir=psz;
    }
    void BspsDir(const TCHAR* psz)        
    {
        _bspDir=psz;
    }
    void TexDir(const TCHAR* psz)         
    {
        _texDir=psz;
    }
    void SndDir(const TCHAR* psz)         
    {
        _sndDir=psz;
    }
    void PrefsDir(const TCHAR* psz)       
    {
        _prefsDir=psz;
    }
    void HomeDir(const TCHAR* hd);
    void ScriptsDir(const TCHAR* psz)     {_scriptsDir=psz;}
    void CompilerDir(const TCHAR* psz)    {_compilerDir=psz;}
    void ServerDir(const TCHAR* psz)      {_serverDir=psz;}
    void XsdCatItmFile(const TCHAR* psz)  {_catFile=psz;}
    void StartDir(const TCHAR* psz)       {_curDir = psz;}


    const TCHAR* InstallDir(){return _instDir.c_str();}
	const TCHAR* StartDir(){return _curDir.c_str();}
    const TCHAR* DocsDir(){return _docsDir.c_str();}
    const TCHAR* HomeDir(){return _homeDir.c_str();}
    
    const TCHAR* BspsDir(){return _bspDir.c_str();}
    const TCHAR* TexDir(){return _texDir.c_str();}
    const TCHAR* SndDir(){return _sndDir.c_str();}
    const TCHAR* PrefsDir(){return _prefsDir.c_str();}
    const TCHAR* ServerDir()  {return _serverDir.c_str();}
    const TCHAR* ScriptsDir(){return _scriptsDir.c_str();}
    const TCHAR* CompilerDir(){return _compilerDir.c_str();}
    const TCHAR* XsdCatItmFile(){return _catFile.c_str();}

    void LoadCustomProfileSettings();
    void WriteCustomProfileSettings();


	void  ReDir(){
        ::_chdir(_curDir.c_str());
    }
    CToolFrame*	GetCompilerFrame(){return _pToolFrame;}
    void CleanTexMan();
    HACCEL  GetAccel(){
        if(!_bToolOn)
            return _h3dEditAccel;
        return _hScriptEditAccel;
    }
    int DoFileDialog(CFileDialog& d);
    void StampDll(int runs);
	void PumpMessage(UINT msg, UINT tout=-1);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZ_ed2App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CZ_ed2App)
	afx_msg void OnAppAbout();
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	void OnFileOpen();

	void OnShowcompiler() ;
    BOOL PreTranslateMessage(MSG* pMsg) ;
	DECLARE_MESSAGE_MAP()


    void CreatePaths();
    BOOL Dbg_ResolveCurDir();
    void FindPlugIns();
	int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
    BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo); 
    void OnPlugInsCmd(UINT cMd);
private:
    void TestKey();
    BOOL RegisterGLWindows();
    void UnregisterGLWindows();
    void ExportHeader();
    void SuperClassAll();
    void UnSuperClassAll();
#ifndef COM_WAY
    void CheckPlug(const TCHAR*);
#endif //
    
public:
    BYTE        _keys[256];

    vvector<string>  _errors;
    TaskClass<Brush> _brushtask;
    CWnd*       p_wndFocused;
    GeticEditor _gEditor;
    TexRef      _TexSys;
    CBrush      _brush[4];
    COLORREF    _color[4];
    COLORREF    _txtcolor;
    CBitmap     _butBmp;
    HCURSOR     _cursors[32];
    HCURSOR     _compilingCur;

    CMaskEdit   _me;
    string      _cName; //class window name
    string      _cNameViews; //class window name

    // toolbar values
    DWORD       _bShowMakeTbar;
    DWORD       _bShowSelectTbar;
    DWORD       _bShowMoveTbar;
    DWORD       _bShowAlignTbar;
    DWORD       _bShowViewsTbar;
    DWORD       _bShowCsgTbar;
    DWORD       _bShowBuildTbar;
    DWORD       _bShowCurvesbar;
	CBitmap		_msgBoxBitmap;

    CBrush      _slpEditBrush;
    CBitmap		_slpEditBitmap;
    CBitmap		_txtEditBitmap;
    CBrush      _txtEditBrush;


    RECT        _rtMakeTbar;
    RECT        _rtSelectTbar;
    RECT        _rtMoveTbar;
    RECT        _rtAlignTbar;
    RECT        _rtViewsTbar;
    RECT        _rtCsgTbar;
    RECT        _rtBuildTbar;
    RECT        _rtCurvesTbar;

    DWORD       _dwpMakeTbar;
    DWORD       _dwpSelectTbar;
    DWORD       _dwpMoveTbar;
    DWORD       _dwpAlignTbar;
    DWORD       _dwpViewsTbar;
    DWORD       _dwpCsgTbar;
    DWORD       _dwpBuildTbar;
	CToolFrame*	_pToolFrame;
    BOOL        _bToolOn;
    CRect        _rectMiniTool;	
    HACCEL      _h3dEditAccel;
    HACCEL      _hScriptEditAccel;
    UINT        _cmdPlugIn;
	BOOL		_returnVal;
    int         _runs;
    HICON       _colexpic[2];
private:
	BOOL m_bATLInited;
private:

    tstring     _instDir;
	tstring     _curDir;
    tstring     _docsDir;
    tstring     _homeDir;
    tstring     _bspDir;
    tstring     _texDir;
    tstring     _sndDir;
    tstring     _prefsDir;
    tstring     _scriptsDir;
    tstring     _compilerDir;
    tstring     _serverDir;
    tstring     _catFile;

    struct SPlugIn
    {
        SPlugIn(const TCHAR* mnname, UINT cmd):menuname(mnname),cmdid(cmd){}
        tstring menuname;
        UINT    cmdid;
    };
    vvector<SPlugIn> _plugins;
    BOOL        _msgBoxOn;
    
};


extern  CZ_ed2App theApp;
#define	REDIR()	 theApp.ReDir()	
#define	GTexSys  (theApp._TexSys)

inline int Inl_GetTexCount()
{
    int textures = 0;
    TexSys::iterator bt = GTexSys.begin();
    TexSys::iterator et = GTexSys.end();
    for(;bt != et; bt++)
    {
        Texture& tex      = (*bt).second;
		if(!strncmp(tex.filename, "@L",2))
			continue;
		++textures;
    }
    return textures;
}
/////////////////////////////////////////////////////////////////////////////

/*
    feb 25: section in bsp have theit offset and length in bytes. not yet saved in bsp
            but the stuctures are there !
    feb 26: script saving fixed  
    feb 27: gl window is not longer destroyed due the wgllist sharing.
            merging on levels. all added next level is the next group in the groups
    nov 16  tezsys moved here to prioritize the destruction
*/
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Z_ED2_H__E1604506_B2FF_4294_90AB_56AD88B6A276__INCLUDED_)
