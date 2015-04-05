// MainFrm.h : interface of the CMainFrame class
//
//---------------------------------------------------------------------------------------

#ifndef __MAINFRAME_G_H__
#define __MAINFRAME_G_H__

//---------------------------------------------------------------------------------------
#include "DlgBar.h"
#include "StaticGL.h"
#include "FixSplitter.h"
#include "DlgCompProgress.h"
#include "bTbarCombo.h"
#include "DlgConsole.h"
#include "UiEnhancer.h"
#include "MyStatusBar.h"
#include "DlgLeafs.h"

//---------------------------------------------------------------------------------------
#define WM_COMPILEDONE  WM_USER+1001
#define WM_USR_REPAINT  WM_USER+1002
#define WM_POSTINIT		WM_USER+1006
#define WM_TEXTURE		WM_USER+1007
#define WM_SBT_MSG      WM_USER+1008
#define WM_VIEW_DESTROY WM_USER+1009
#define WM_GOTOZALSOFT  WM_USER+1010
#define WM_SPLASHGONE	WM_USER+1011

#define SPASH_TIMER		336		

//---------------------------------------------------------------------------------------
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:

public:
    CFixSplitter m_wndSplitterS;
	CFixSplitter m_wndSplitter;
// Operations
public:
    static CMainFrame* PFrame;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
    CStaticGL*  GetGLWidnow(CWnd* pNewParent, UINT CtlId);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    CDlgCompProgress* ShowProgressDlg(BOOL bShow);

    //_dlgleafs
    void    DestroyLeafExplorer();
    void    CreateLeafExplorer();


	CMyStatusBar  m_wndStatusBar;
	BOOL         b_TolTipBar;   
    C_TOOLBAR    m_TolTipBar;
    C_TOOLBAR    m_MakeTbar;
    C_TOOLBAR    m_SelectTbar;
    C_TOOLBAR    m_MoveTbar;
    C_TOOLBAR    m_AlignTbar;
    C_TOOLBAR    m_ViewsTbar;
    C_TOOLBAR    m_CsgTbar;
    C_TOOLBAR    m_BuildTbar;
    C_TOOLBAR    m_Curvesbar;
#ifndef _BAR_DLGPANE
    CDlgBar     m_barDlg;
#endif //

    //CCoolDialogBar       m_wndDialogBar;
	C_REBAR             m_wndReBar;
    CFont				_cfont;	
    CbTbarCombo         m_combobox;
    CbTbarCombo         m_comboboxts;
    CStatic             m_toltip;
    CDlgCompProgress	_dlgProgress;
    CDlgConsole			_console;
    CStatic            _titles[2];

	afx_msg void OnBushowdlg();
// Generated message map functions
protected:
    LRESULT OnCompileDone(WPARAM,LPARAM);
    LRESULT OnPostRepaint(WPARAM,LPARAM);
	LRESULT OnPostInit(WPARAM,LPARAM);
	LRESULT OnTextureGen(WPARAM,LPARAM);
	LRESULT OnTextureRem(WPARAM,LPARAM);
    LRESULT OnTSBTMsg(WPARAM w,LPARAM l);
    LRESULT OnSplashGone(WPARAM w,LPARAM l);
    LRESULT OnSbarNoty(WPARAM kc, LPARAM down);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnGlobals();
	afx_msg void OnClose();
	afx_msg void OnSettingsPreferences();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnConsole();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnRefr();
	afx_msg void OnUpdateRefr(CCmdUI* pCmdUI);
	//}}AFX_MSG
    virtual HACCEL GetDefaultAccelerator();
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
    afx_msg LRESULT OnViewDestroy(WPARAM, LPARAM);
    afx_msg LRESULT OnZalsoft(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

	CImageList il_ALIGN_24[3];		
	CImageList il_BUILD_24[3];		
	CImageList il_CSG_24[3];		
	CImageList il_MAKE_24[3];      
	CImageList il_MOVE_24[3];      
	CImageList il_QUADRAICS_24[3]; 
	CImageList il_SELECT_24[3];    
	CImageList il_VIEWS_24[3];


    int        m_col1;
    int        m_row1;
    int        m_col2;
    int        m_row2;
	
    CStaticGL  m_Gl; 
    C_Menu     _menu;
public:
    static int			_font1;
	static HFONT		_hf;
	CBrush				_brush;
	COLORREF			_cbk;
    BOOL	            m_bshutdown;
    BOOL                _dlgBarHidden;
    BOOL                b_viewsup;
    CDlgLeafs           _dlgleafs;
};

extern 	Htex		    _startPosTex;
extern 	Htex		    _selTex;
extern 	Htex		    _dumptex;
extern 	Htex			_lbulbtex;
extern 	Htex			_itmqtex;
extern 	Htex			_sndtex;
extern 	Htex			_tbtex;
extern  Htex            _wLmap;
extern  Htex            _ambLmap;
extern  HWND            _Hmain;


#define FRM()   CMainFrame::PFrame
extern UINT     ThrID;

void  SBT(int nPane, LPCTSTR pTxt);
#define SBAR()  CMainFrame::PFrame->m_wndStatusBar

#define CLRLOG() CMainFrame::PFrame->_dlgProgress.Clear()
#define LOG      CMainFrame::PFrame->_dlgProgress.Log
#define LOGERR   CMainFrame::PFrame->_dlgProgress.LogPos
#define PROG     CMainFrame::PFrame->_dlgProgress.Progress



// opearation id's for copiling phase
typedef enum _EXCOMP{
    E_COMP  = 1  ,
    E_HSR      ,
    E_ESR      ,
    E_BSP      ,
    E_PORT     ,
    E_PVS      ,
    E_TJU      ,
    E_LMAPS    ,
    E_COMPILER,
    I_DONE,
}EXCOMP;

extern      CZ_ed2View* Ptz;
extern  	CZ_ed2View* Ptx;
extern  	CZ_ed2View* Pty;
extern      z_ed3View*  PT3;

inline CZ_ed2View* VIEW2D(int x, int y) 
{
    if(x==0 && y==1)
        return Ptz; 
    if(x==1 && y==0)
        return Ptx;
    return Pty;
}

inline z_ed3View* VIEW3D(int x, int y) {return PT3;}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__MAINFRAME_G_H__
