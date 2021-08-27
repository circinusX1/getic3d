// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "_config.h"
#include "z-edmap.h"
#include "MainFrm.h"
#include "Z_ed3View.h"
#include "Z_ed2View.h"
#include "DlgGs.h"
#include "compiler.h"
#include "z_ed2Doc.h"
#include "ToolFrame.h"

BOOL GetOpenCommand(const char* regPath, CString& command);

BOOL            BbautoClose = 0;
static int      BARWIDTH  =  260;
static char     Sbt[255];
//---------------------------------------------------------------------------------------
size_t            UTimer;
Htex		    _selTex;
Htex		    _dumptex;
Htex			_lbulbtex;
Htex			_itmqtex;
Htex			_sndtex;
Htex			_startPosTex;
Htex			_tbtex;  
Htex            _wLmap;
Htex            _ambLmap;
HWND            _Hmain;// = m_hWNd;

//---------------------------------------------------------------------------------------
CMainFrame* CMainFrame::PFrame;
int		    CMainFrame::_font1;
HFONT	    CMainFrame::_hf;
CZ_ed2View* Ptz;
CZ_ed2View* Ptx;
CZ_ed2View* Pty;
z_ed3View*  PT3;

void	MakeToolbarImageList(int			inBitmapID,
							 size_t			nButtons,
							 CImageList&	outImageList,
							 CToolBarCtrl& barCtrl, int what);

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_MESSAGE(WM_COMPILEDONE,OnCompileDone)
    ON_MESSAGE(WM_USR_REPAINT, OnPostRepaint)
    ON_MESSAGE(WM_SBARNOTY, OnSbarNoty)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(MN_GLOBALS, OnGlobals)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SETTINGS_PREFERENCES, OnSettingsPreferences)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_COMMAND(MN_CONSOLE, OnConsole)
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
    ON_COMMAND_EX(TB_SLINETTIP, OnBarCheck)
    ON_COMMAND_EX(TB_MAKE, OnBarCheck)
	ON_COMMAND_EX(TB_SELECT, OnBarCheck)
	ON_COMMAND_EX(TB_MOVE, OnBarCheck)
	ON_COMMAND_EX(TB_ALIGN, OnBarCheck)
	ON_COMMAND_EX(TB_VIEWS, OnBarCheck)
	ON_COMMAND_EX(TB_CSG, OnBarCheck)
	ON_COMMAND_EX(TB_BUILD, OnBarCheck)
    ON_COMMAND_EX(TB_QUADRAICS, OnBarCheck)
    ON_UPDATE_COMMAND_UI(TB_SLINETTIP, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_MAKE, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_SELECT, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_MOVE, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_ALIGN, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_VIEWS, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_CSG, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_BUILD, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(TB_QUADRAICS, OnUpdateControlBarMenu)
	ON_COMMAND(PB_REFR, OnRefr)
	ON_UPDATE_COMMAND_UI(PB_REFR, OnUpdateRefr)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_SPLASHGONE,OnSplashGone)
	ON_MESSAGE(WM_POSTINIT,OnPostInit)
	ON_MESSAGE(WM_TEXTURE,OnTextureGen)
	ON_MESSAGE(WM_REMOVETEX,OnTextureRem)
    ON_MESSAGE(WM_SBT_MSG,OnTSBTMsg)
    ON_MESSAGE(WM_VIEW_DESTROY,OnViewDestroy)
    ON_MESSAGE(WM_GOTOZALSOFT,OnZalsoft)
END_MESSAGE_MAP()

static size_t indicators[] =
{
	ID_SEPARATOR,           
    SB_CAMPOS,
    SB_MOUSEPOS,
    SB_2DPOS,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame():m_bshutdown(0),m_wndSplitterS(4)
{
    b_TolTipBar = 0;
    b_viewsup = FALSE;
	CMainFrame::PFrame = this;
    _dlgBarHidden = AfxGetApp()->GetProfileInt("GDLG","BARSHOW",0);	
}

CMainFrame::~CMainFrame()
{
    TRACE("~CMainFrame() After this we have access volation");
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	    
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    _Hmain = m_hWnd;
    CenterWindow();

    size_t defStyle = TBSTYLE_FLAT|TBSTYLE_TRANSPARENT;
    size_t styl  = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM;

	//UTimer = SetTimer(SPASH_TIMER, 60*1000*10, 0);

    
	if (!m_TolTipBar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_SLINETTIP) ||
		!m_TolTipBar.LoadToolBar(TB_SLINETTIP))
	{
		return -1;      // fail to create
	}


	if (!m_MakeTbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_MAKE) ||
		!m_MakeTbar.LoadToolBar(TB_MAKE))
	{
		return -1;      // fail to create
	}
    
    if (!m_SelectTbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_SELECT)  ||
		!m_SelectTbar.LoadToolBar(TB_SELECT))
	{
		return -1;      // fail to create
	}

    if (!m_MoveTbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_MOVE)  ||
		!m_MoveTbar.LoadToolBar(TB_MOVE))
	{
		return -1;      // fail to create
	}
    
	if (!m_AlignTbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_ALIGN)  ||
		!m_AlignTbar.LoadToolBar(TB_ALIGN))
	{
		return -1;      // fail to create
	}

	if (!m_ViewsTbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_VIEWS)  ||
		!m_ViewsTbar.LoadToolBar(TB_VIEWS))
	{
		return -1;      // fail to create
	}
    
	if (!m_CsgTbar.CreateEx(this, defStyle,styl,CRect(0,0,0,0), TB_CSG) ||
		!m_CsgTbar.LoadToolBar(TB_CSG))
	{
		return -1;      // fail to create
	}

    if (!m_BuildTbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_BUILD)  ||
		!m_BuildTbar.LoadToolBar(TB_BUILD))
	{
		return -1;      // fail to create
	}

    if (!m_Curvesbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), TB_QUADRAICS)  ||
		!m_Curvesbar.LoadToolBar(TB_QUADRAICS))
	{
		return -1;      // fail to create
	}

    //CToolBar

    
	MakeToolbarImageList (TB_ALIGN_24E            ,13,	il_ALIGN_24[0],		m_AlignTbar.GetToolBarCtrl(),0);
	MakeToolbarImageList (TB_BUILD_24E            ,5,	il_BUILD_24[0],		m_BuildTbar.GetToolBarCtrl(),0);
	MakeToolbarImageList (TB_CSG_24E              ,3,	il_CSG_24[0],		m_CsgTbar.GetToolBarCtrl(),0);
	MakeToolbarImageList (TB_MAKE_24E             ,13,	il_MAKE_24[0],      m_MakeTbar.GetToolBarCtrl(),0);
	MakeToolbarImageList (TB_MOVE_24E             ,5,	il_MOVE_24[0],      m_MoveTbar.GetToolBarCtrl(),0);
	MakeToolbarImageList (TB_SELECT_24E           ,5,	il_SELECT_24[0],    m_SelectTbar.GetToolBarCtrl(),0);
	MakeToolbarImageList (TB_VIEWS_24E            ,6,	il_VIEWS_24[0],		m_ViewsTbar.GetToolBarCtrl(),0);
    MakeToolbarImageList (TB_QUADRAICS_24E        ,2,   il_QUADRAICS_24[0],	m_Curvesbar.GetToolBarCtrl(),0);
//    TB_QUADRAICS_24E

	MakeToolbarImageList (TB_ALIGN_24D            ,13,il_ALIGN_24[1],	m_AlignTbar.GetToolBarCtrl(),1);
	MakeToolbarImageList (TB_BUILD_24D            ,5,il_BUILD_24[1],	m_BuildTbar.GetToolBarCtrl(),1);
	MakeToolbarImageList (TB_CSG_24D              ,3,il_CSG_24[1],		m_CsgTbar.GetToolBarCtrl(),1);
	MakeToolbarImageList (TB_MAKE_24D             ,13,il_MAKE_24[1],     m_MakeTbar.GetToolBarCtrl(),1);
	MakeToolbarImageList (TB_MOVE_24D             ,5,il_MOVE_24[1],      m_MoveTbar.GetToolBarCtrl(),1);
	MakeToolbarImageList (TB_SELECT_24D           ,5,il_SELECT_24[1],    m_SelectTbar.GetToolBarCtrl(),1);
	MakeToolbarImageList (TB_VIEWS_24D            ,6,il_VIEWS_24[1],	m_ViewsTbar.GetToolBarCtrl(),1);
    MakeToolbarImageList (TB_QUADRAICS_24D        ,2,il_QUADRAICS_24[1],	    m_Curvesbar.GetToolBarCtrl(),1);


   	MakeToolbarImageList (TB_ALIGN_24R            ,13,il_ALIGN_24[2],	 m_AlignTbar.GetToolBarCtrl(),2);
	MakeToolbarImageList (TB_BUILD_24R            ,5,il_BUILD_24[2],	 m_BuildTbar.GetToolBarCtrl(),2);
	MakeToolbarImageList (TB_CSG_24R              ,3,il_CSG_24[2],		 m_CsgTbar.GetToolBarCtrl(),2);
	MakeToolbarImageList (TB_MAKE_24R             ,13,il_MAKE_24[2],     m_MakeTbar.GetToolBarCtrl(),2);
	MakeToolbarImageList (TB_MOVE_24R             ,5,il_MOVE_24[2],      m_MoveTbar.GetToolBarCtrl(),2);
	MakeToolbarImageList (TB_SELECT_24R           ,5,il_SELECT_24[2],    m_SelectTbar.GetToolBarCtrl(),2);
	MakeToolbarImageList (TB_VIEWS_24R            ,6,il_VIEWS_24[2],	 m_ViewsTbar.GetToolBarCtrl(),2);
    MakeToolbarImageList (TB_QUADRAICS_24R        ,2,il_QUADRAICS_24[2], m_Curvesbar.GetToolBarCtrl(),2);

    
    _menu.Attach(GetMenu()->m_hMenu);
	m_wndReBar.Create(this);

    size_t sbs = WS_CHILD|WS_VISIBLE|CBRS_BOTTOM;
    sbs &= ~SBARS_SIZEGRIP;

	if (!m_wndStatusBar.Create(this,sbs)  ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(size_t)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
    CStatusBarCtrl& sb =   m_wndStatusBar.GetStatusBarCtrl();
    sb.SetBkColor(theApp._color[1]);
    sb.ModifyStyle(SBARS_SIZEGRIP, 0);

    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 320);
    m_wndStatusBar.SetPaneInfo(1, SB_CAMPOS,    SBPS_NORMAL, 180) ;
    m_wndStatusBar.SetPaneInfo(2, SB_MOUSEPOS,  SBPS_NORMAL, 180);
    m_wndStatusBar.SetPaneInfo(3, SB_2DPOS,     SBPS_NORMAL, 180);

    _cfont.CreateFont(-16,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DRAFT_QUALITY,DEFAULT_PITCH,"arial"	);	
    m_wndStatusBar.SetFont(&_cfont);


    size_t tbAttr = CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY;

    m_TolTipBar.SetBarStyle(m_TolTipBar.GetBarStyle() |tbAttr);
	m_MakeTbar.SetBarStyle(m_MakeTbar.GetBarStyle() |tbAttr);
    m_SelectTbar.SetBarStyle(m_SelectTbar.GetBarStyle() |tbAttr);
    m_MoveTbar.SetBarStyle(m_MoveTbar.GetBarStyle() |tbAttr);
    m_AlignTbar.SetBarStyle(m_AlignTbar.GetBarStyle() |tbAttr);
    m_ViewsTbar.SetBarStyle(m_ViewsTbar.GetBarStyle() |tbAttr);
    m_CsgTbar.SetBarStyle(m_CsgTbar.GetBarStyle() |tbAttr);
    m_BuildTbar.SetBarStyle(m_BuildTbar.GetBarStyle() |tbAttr);
    m_Curvesbar.SetBarStyle(m_Curvesbar.GetBarStyle() |tbAttr);
    //
	// Init values
	//
    
    int index=0;
    RECT rc;
    while (m_MakeTbar.GetItemID(index) != PB_COMBO)
       index++;
    m_MakeTbar.SetButtonInfo(index, PB_COMBO, TBBS_SEPARATOR, 140);
    m_MakeTbar.GetItemRect(index, &rc);
    rc.top		+= 2;
    rc.bottom	+= 200;
    size_t dwStyle = WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL |CBS_DROPDOWNLIST | CBS_HASSTRINGS;
    if (!m_combobox.Create(dwStyle, rc, &m_MakeTbar, 1011))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }
    CFont* pf = m_MakeTbar.GetFont();
    m_combobox.SetFont(&_cfont,1);

    for(int i=-1;i<32;i++)
    {
        if(i==-1)
            m_combobox.AddString(MKSTR("%d View All",i));
        else
            m_combobox.AddString(MKSTR("%d View Group ",i));
    }
    m_combobox.SetCurSel(0);

    ++index;
    m_MakeTbar.SetButtonInfo(index, PB_COMBO, TBBS_SEPARATOR, 100);
    m_MakeTbar.GetItemRect(index, &rc);
    rc.top		+= 2;
    rc.bottom	+= 200;
    if (!m_comboboxts.Create(dwStyle, rc, &m_MakeTbar, 1012))
    {
        TRACE0("Failed to create combo-box\n");
        return FALSE;
    }

    m_comboboxts.SetFont(&_cfont,1);
    //for(i=0;i<4;i++)
    {
        int idx = m_comboboxts.AddString(" Primary");
		m_comboboxts.SetItemData(idx, 0);

        idx = m_comboboxts.AddString(" *Light Map");
		m_comboboxts.SetItemData(idx, 1);

		idx = m_comboboxts.AddString(" Detail");
		m_comboboxts.SetItemData(idx, 2);
		
        idx = m_comboboxts.AddString(" Secondary");
		m_comboboxts.SetItemData(idx, 3);
    }
    m_comboboxts.SetCurSel(0);


    m_TolTipBar.SetButtonInfo(0, ST_SEP, TBBS_SEPARATOR, 100);
    m_MakeTbar.GetItemRect(index, &rc);
    rc.top		-= 3;
    rc.bottom	-= 3;
    rc.right	+= 200;
    m_toltip.Create("Tooltip",WS_VISIBLE|WS_CHILD|WS_BORDER, rc, &m_TolTipBar, 1013);//CStatic
    m_toltip.SetFont(&_cfont,1);


	m_wndReBar.AddBar(&m_MakeTbar);
	m_wndReBar.AddBar(&m_SelectTbar);
	m_wndReBar.AddBar(&m_MoveTbar);
	m_wndReBar.AddBar(&m_AlignTbar);
	m_wndReBar.AddBar(&m_CsgTbar);
	m_wndReBar.AddBar(&m_BuildTbar);
	m_wndReBar.AddBar(&m_ViewsTbar);
    m_wndReBar.AddBar(&m_Curvesbar);
    m_wndReBar.AddBar(&m_TolTipBar);
    
    
	ShowControlBar(&m_TolTipBar,b_TolTipBar,0);	
    ShowControlBar(&m_MakeTbar,theApp._bShowMakeTbar,0);
    ShowControlBar(&m_SelectTbar,theApp._bShowSelectTbar,0);
    ShowControlBar(&m_MoveTbar,theApp._bShowMoveTbar,0);
    ShowControlBar(&m_AlignTbar,theApp._bShowAlignTbar,0);
    ShowControlBar(&m_ViewsTbar,theApp._bShowViewsTbar,0);
    ShowControlBar(&m_CsgTbar,theApp._bShowCsgTbar,0);
    ShowControlBar(&m_BuildTbar,theApp._bShowBuildTbar,0);
    ShowControlBar(&m_Curvesbar,theApp._bShowCurvesbar,0);
    

    if(m_TolTipBar.GetToolBarCtrl().GetToolTips())
        m_TolTipBar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_AlignTbar.GetToolBarCtrl().GetToolTips())
        m_AlignTbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_BuildTbar.GetToolBarCtrl().GetToolTips())
        m_BuildTbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_CsgTbar.GetToolBarCtrl().GetToolTips())
        m_CsgTbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_MakeTbar.GetToolBarCtrl().GetToolTips())
        m_MakeTbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_MoveTbar.GetToolBarCtrl().GetToolTips())
        m_MoveTbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_SelectTbar.GetToolBarCtrl().GetToolTips())
        m_SelectTbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_ViewsTbar.GetToolBarCtrl().GetToolTips())
        m_ViewsTbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);
    if(m_Curvesbar.GetToolBarCtrl().GetToolTips())
        m_Curvesbar.GetToolBarCtrl().GetToolTips()->SetDelayTime(0);


    RecalcLayout();

	PostMessage(WM_POSTINIT,0,0);
    PostMessage(WM_TIMER, UTimer+1) ;


	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	RECT rt;
    GetClientRect(&rt);

#ifdef _BAR_DLGPANE // bardlg in pane

    m_wndSplitterS.CreateStatic(this, 1, 2);

    m_wndSplitter.CreateStatic(&m_wndSplitterS, 2, 2, WS_CHILD | WS_VISIBLE | WS_BORDER, 
                              m_wndSplitterS.IdFromRowCol(0, 1));



    CDC ScreenDC;
    VERIFY(ScreenDC.CreateIC(_T("DISPLAY"), NULL, NULL, NULL));
    const int nLogDPIX = ScreenDC.GetDeviceCaps(LOGPIXELSX);
    const int nLogDPIY = ScreenDC.GetDeviceCaps(LOGPIXELSY);

    int obw = BARWIDTH ;
    if (nLogDPIX == 96 && nLogDPIY == 96)
    {
        ;
    }
    else
    {
        if (nLogDPIX == 120 && nLogDPIY == 120)
        {
            REAL bw = (REAL)BARWIDTH;
            bw *= (REAL)(120.0/96.0);
            BARWIDTH = bw + 8;
        }
        else
        {
            REAL bw = (REAL)BARWIDTH;
            bw *= (REAL)(nLogDPIX/96.0);
            BARWIDTH = bw + 8;
        }
    }
    if(BARWIDTH < obw )
        BARWIDTH=obw;


    if (!m_wndSplitterS.CreateView(0, 0, RUNTIME_CLASS(CDlgBar), CSize(BARWIDTH, 0), pContext))
	{
		return FALSE;
	}
    rt.right-=BARWIDTH;
    rt.bottom -= 24;

#else
    rt.bottom -= 25;
   
    m_barDlg.Create(IDR_MAINFRAMEMDS, 0);
    m_wndSplitter.CreateStatic(this, 2, 2);

#endif //


	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(z_ed3View), CSize(rt.right/2, rt.bottom/2), pContext))
	{
		return FALSE;
	}


	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CZ_ed2View), CSize(rt.right/2, rt.bottom/2), pContext))
	{
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CZ_ed2View), CSize(rt.right/2, rt.bottom/2), pContext))
	{
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(1, 1,
		RUNTIME_CLASS(CZ_ed2View), CSize(rt.right/2, rt.bottom/2), pContext))
	{
		return FALSE;
    }

    CZ_ed2View* ptx = (CZ_ed2View *)m_wndSplitter.GetPane(1,0);
	CZ_ed2View* pty = (CZ_ed2View *)m_wndSplitter.GetPane(0,1);
	CZ_ed2View* ptz = (CZ_ed2View *)m_wndSplitter.GetPane(1,1);

    size_t defStyle = TBSTYLE_FLAT;
    size_t styl  = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM;
	

    ptx->_vt = 'x';
    ptx->_viewrot=90;
    
    pty->_vt = 'y';
    pty->_viewrot=90;

    ptz->_vt = 'z';
    ptz->_viewrot=0;
	
	return TRUE;
}
   	


LRESULT CMainFrame::OnTextureRem(WPARAM w,LPARAM l)
{
	GTexSys.RemoveTex((Htex*)&w,1);
	return 0;
}


LRESULT CMainFrame::OnTextureGen(WPARAM w,LPARAM l)
{
	MakeCurrent((HDC) w, (HGLRC) l);
	return 0;
}

LRESULT CMainFrame::OnSplashGone(WPARAM w,LPARAM l)
{
    #pragma message ("I: running it from association the name comes between double quotes")    
    if(theApp.m_lpCmdLine && *theApp.m_lpCmdLine )
    {
        CString loco = theApp.m_lpCmdLine;
        loco.Replace("\"","");
        DOC()->ReadFromFile(loco);
    }
    return 0;
}

LRESULT CMainFrame::OnPostInit(WPARAM w,LPARAM l)
{
    Ptz = (CZ_ed2View *)m_wndSplitter.GetPane(0,1);
	Ptx = (CZ_ed2View *)m_wndSplitter.GetPane(1,0);
	Pty = (CZ_ed2View *)m_wndSplitter.GetPane(1,1);
    PT3 = (z_ed3View *)m_wndSplitter.GetPane(0,0);

    if(0 == Ptz)
    {
        PostMessage(WM_POSTINIT,0,0);
    }

    b_viewsup = 1;

    wglShareLists(PT3->m_hRC, Ptz->m_hRC);
    wglShareLists(PT3->m_hRC, Ptx->m_hRC );
    wglShareLists(PT3->m_hRC, Pty->m_hRC );
    m_Gl.CreateWnd(this, 0); // this shares the list inside
    

	TexRef::Clear(1);
    BYTE wLmap[4]    = {255,255,255,255};
	MakeCurrent('3');



   	BYTE dBits[] = {  255,192,192, 128,255,235, 209,209,255, 255,235,235, 
                      255,192,192, 235,255,235, 209,209,255, 235,128,235, 
                      255,128,192, 235,128,235, 209,209,255, 235,235,255,
                      255,192,192, 235,255,235, 209,209,128, 255,235,235};
    _dumptex  = TexRef::GlGenTex( 4, 4, 3 , dBits,0);

    /*
   	BYTE dBitsSel[4*4*3];
    memset(dBitsSel,192, 4*4*3);
    dBitsSel[0]=255;
    dBitsSel[1]=255;
    dBitsSel[2]=255;

    dBitsSel[9]=255;
    dBitsSel[10]=255;
    dBitsSel[11]=255;

    dBitsSel[36]=255;
    dBitsSel[37]=255;
    dBitsSel[38]=255;

    dBitsSel[47]=255;
    dBitsSel[46]=255;
    dBitsSel[45]=255;
*/
    BYTE dBitsSel[32*32*3];
    int line = 0;

    int g = 0;
    for(int x=0;x<32;x++)
    {
        for(int y=0;y<32;y++)
        {
            if( (((x%4)+(y%4))%2)==0 )
            {
                dBitsSel[g++]=128;
                dBitsSel[g++]=128;
                dBitsSel[g++]=128;
            }
            else
            {
                dBitsSel[g++]=0;
                dBitsSel[g++]=0;
                dBitsSel[g++]=0;
            }
        }
    }

   
    _selTex   = TexRef::GlGenTex( 32, 32, 3 , dBitsSel,GEN_TEX_MM_LINEAR);
    _sndtex	  = TexRef::LoadRCTexture("-IDR_SCE_SOUND", 128,128);
    _lbulbtex = TexRef::LoadRCTexture("-IDR_SCE_LBULB", 128,128);
    _itmqtex  = TexRef::LoadRCTexture("-ITM_CUSTOM", 128,128);
	_tbtex	  = TexRef::LoadRCTexture("-IDR_GLTBAR",256,32);
    _startPosTex  = TexRef::LoadRCTexture("-ITM_STARTPOS",32,32);



    _wLmap    = TexRef::GlGenTex( 1, 1, 3 , wLmap, 0);

    _ambLmap  = DOC()->RecreateAmbLmap();

    
    
    TexRef::_defTex = _dumptex;
    m_Gl.InitialUpdate();

//    char* pext = (char*)glGetString(GL_VERSION);
	
	_font1 = glGenLists(96); 
    _hf    = CreateFont(-11, 0, 0, 0, FW_MEDIUM, FALSE,
                        FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
                        CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                        FF_DONTCARE | DEFAULT_PITCH, "Arial" );
    SelectObject(Ptz->_hdc, _hf); 
	SelectObject(Ptx->_hdc, _hf); 
	SelectObject(Pty->_hdc, _hf); 
    wglUseFontBitmaps(Ptz->_hdc, 32, 96, _font1); 
	wglUseFontBitmaps(Ptx->_hdc, 32, 96, _font1); 
	wglUseFontBitmaps(Pty->_hdc, 32, 96, _font1); 

    DOC()->Invalidate(1);
    BARDLG()->SendMessage(WM_POSTINIT,0,0);

    if(!IsValidUL(0))
        PostMessage(WM_GOTOZALSOFT);

    return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
#ifndef _DEBUG
    cs.cx = GetSystemMetrics(SM_CXSCREEN);
    cs.cy = GetSystemMetrics(SM_CXSCREEN);
#endif //


	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style|=CS_OWNDC;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnTimer(size_t nIDEvent) 
{
#ifdef _FREE
    if((UTimer+1)== nIDEvent)
    {
        OnZalsoft(0,0);
    }
#endif//
	if(GetActiveWindow() == this)
    {
        z_ed3View* pT3D   = VIEW3D(0,0);
        pT3D->RedrawDC(0);
    }
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy() 
{
    if(!IsValidUL(0))
    {
        PostMessage(WM_GOTOZALSOFT);
        AfxMessageBox("Getic 3.0. \n Anoying version", MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
    }

    KillTimer(UTimer);
	CFrameWnd::OnDestroy();
}


void CMainFrame::OnBushowdlg() 
{
}


void CMainFrame::OnGlobals() 
{
    CDlgGs	d;
    d.DoModal();
	DOC()->SetFaceMode();
	DOC()->SetLightMode();
    DOC()->Invalidate(1);

}


void CMainFrame::OnClose() 
{
    m_wndStatusBar.SetFont(0);
    m_combobox.SetFont(0,1);
    m_comboboxts.SetFont(0,1);
    _cfont.DeleteObject();
    AfxGetApp()->WriteProfileInt("GDLG","BARSHOW",_dlgBarHidden);

    m_bshutdown = 1;
    OnViewDestroy(0,0);
	CFrameWnd::OnClose();
}

CDlgCompProgress* CMainFrame::ShowProgressDlg(BOOL bShow)
{
	if(bShow)
	{
		if(0==_dlgProgress.m_hWnd)
        {
            /*
            if(AKEY(VK_CONTROL))
            {
                _dlgProgress.Create(this, DLG_COMPILERPROG, CBRS_ALIGN_BOTTOM, AFX_IDW_DIALOGBAR);
                RecalcLayout();
            }
            else
            {
            */
                _dlgProgress.Create(DLG_COMPILERPROG1, this);
            //}
        }
	}
    else 
	{
		if(_dlgProgress.m_hWnd)
		{
            size_t id = GetDlgCtrlID();
			_dlgProgress.DestroyWindow();
			_dlgProgress.m_hWnd=0;
            //if(id == DLG_COMPILERPROG)
              //  RecalcLayout(); 
		}
	}
	
    return &_dlgProgress;
}


LRESULT CMainFrame::OnCompileDone(WPARAM w,LPARAM l)
{
    if(_dlgProgress.m_hWnd && ::IsWindow(_dlgProgress.m_hWnd))
    {
        _dlgProgress.GetDlgItem(PB_CANCEL)->EnableWindow(0);
        _dlgProgress.GetDlgItem(PC_CLOSE)->EnableWindow(1);
        if(_dlgProgress.IsDlgButtonChecked(IDC_CHECK1))
        {
            BbautoClose = 1;
            _dlgProgress.PostMessage(WM_COMMAND, PC_CLOSE);
        }

    }
    return DOC()->OnCompileDone(w,l);
}

BOOL CMainFrame::OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(m_bshutdown==0 && CZ_ed2Doc::PDoc)
	{
		DOC()->_cmdMsg	= 0;
	}

	BOOL b = CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if(CZ_ed2Doc::PDoc && m_bshutdown==0)
	{
		if(0 == DOC()->_cmdMsg)
			DOC()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	return b;
}

//---------------------------------------------------------------------------------------
// cool effect durring union
LRESULT CMainFrame::OnPostRepaint(WPARAM pbr1, LPARAM pbr2)
{
    DOC()->DeselectAll();
    if(pbr1)
    {
        ((Brush*) pbr1)->Select();
        ((Brush*) pbr2)->Select();
    }
    int view = 3;	    
    switch(--view)
    {
        case 0:
            m_wndSplitter.GetPane(1,0)->SendMessage(WM_USR_REPAINT);
            return 0;
        case 1:
	        m_wndSplitter.GetPane(0,1)->SendMessage(WM_USR_REPAINT);
            break;
        case 2:
            m_wndSplitter.GetPane(1,1)->SendMessage(WM_USR_REPAINT);
            break;
        case 3:
            m_wndSplitter.GetPane(0,0)->SendMessage(WM_USR_REPAINT);
            break;
    }
    return 0;
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(theApp._returnVal==0)
		PostQuitMessage(0);
	return CFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::OnSettingsPreferences() 
{
    	
}

void	MakeToolbarImageList(int			inBitmapID,
							 size_t			nButtons,
							 CImageList&	outImageList,
							 CToolBarCtrl& barCtrl, int what)
{
	CBitmap b;

	b.LoadBitmap(abs(inBitmapID));
	outImageList.Create( 26,27, ILC_COLOR24|ILC_MASK, nButtons, 1);
	outImageList.Add(&b, RGB(109,109,109));



	if(what==1)
		barCtrl.SetDisabledImageList(&outImageList);
	else if(what==0)
		barCtrl.SetImageList (&outImageList);
	else if(what==2)
		barCtrl.SetHotImageList (&outImageList);

    /*
	CBitmap		bm;

	bm.Attach (::LoadImage (::AfxFindResourceHandle(
		MAKEINTRESOURCE (inBitmapID), RT_BITMAP),
		MAKEINTRESOURCE (inBitmapID), IMAGE_BITMAP, 0, 0,
		(LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS )));

  */
	
/*
	outImageList.Add (&bm, (CBitmap*)NULL);

	bm.DeleteObject();
    */
}



void CMainFrame::OnSize(size_t nType, int cx, int cy) 
{
    // geep propportion on splitters
	CFrameWnd::OnSize(nType, cx, cy);
    return;//
    /**
	if(m_wndSplitter.m_hWnd)
	{
		RECT rt;

		GetClientRect(&rt);
	    
		rt.bottom-=32;

		rt.right=tmax(rt.right,2);
		rt.bottom=tmax(rt.bottom,2);

        
        //m_wndSplitterS.GetColumnInfo(0, cc, cm);

        if(_dlgBarHidden)
        {
            m_wndSplitterS.SetColumnInfo(0, 2, 0);
            rt.right-=2;
        }
        else
        {
		    m_wndSplitterS.SetColumnInfo(0, BARWIDTH, 0);
            rt.right-=BARWIDTH;
        }
        if(rt.right<2)            rt.right=2;
        if(rt.bottom<2)            rt.bottom=2;
		m_wndSplitter.SetColumnInfo(0, rt.right/2, 0);
		m_wndSplitter.SetColumnInfo(1, rt.right/2, 0);

		m_wndSplitter.SetRowInfo(0, rt.bottom/2, 0);
		m_wndSplitter.SetRowInfo(1, rt.bottom/2, 0);

		m_wndSplitterS.RecalcLayout();
		m_wndSplitter.RecalcLayout();
	}
*/
}

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	CFrameWnd::RecalcLayout(bNotify);
/*
    int cd = 0;
    m_col1=m_col2=0;
    m_row1=m_row2=0;

    m_wndSplitter.GetColumnInfo(0,m_col1,cd);
    m_wndSplitter.GetColumnInfo(1,m_col2,cd);
    m_wndSplitter.GetRowInfo(0,m_row1,cd);
    m_wndSplitter.GetRowInfo(1,m_row2,cd);
*/

}

void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
	ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);

	CControlBar* pBar = GetControlBar(pCmdUI->m_nID);
	if (pBar != NULL)
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
		return;
	}
	pCmdUI->ContinueRouting();
}


BOOL CMainFrame::OnBarCheck(size_t nID)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
	ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);

	CControlBar* pBar = GetControlBar(nID);
	if (pBar != NULL)
	{
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CFrameWnd::OnSetFocus(pOldWnd);
    theApp._bToolOn=FALSE;
}

HACCEL CMainFrame::GetDefaultAccelerator()
{
    return theApp.GetAccel();
}


void CMainFrame::OnConsole() 
{
/*
    if(_console.m_hWnd==0)    	
    {
        _console.Create(DLG_CONSOLE, this);
        _console.ShowWindow(SW_SHOW);
    }
*/
}

CStaticGL*    CMainFrame::GetGLWidnow(CWnd* pNewParent, size_t CtlId)
{
    if(pNewParent==0)
    {
        m_Gl._pcbpreppaint=0;
        m_Gl._pcbsetcursor=0;
        m_Gl._pcbmouse=0;
        m_Gl._pcbinit=0;
        m_Gl._pcbpaint=0;
        m_Gl._pcbsz=0;
        m_Gl._zoom =0.3;
        m_Gl._bHide = 0;
        m_Gl._dist = -1090;
        m_Gl._backgrnd[0]=0;
        m_Gl._backgrnd[1]=0;
        m_Gl._backgrnd[2]=0;
        m_Gl._backgrnd[3]=255;

        m_Gl._cam.SetFov(2*PIPE6); 
        m_Gl._cam.SetFar(25600.0);//250m
	    m_Gl._cam.SetAngles(-PIPE6,-PIPE6,0);
	    m_Gl._cam._pos = V3(3200.0f,3200.0f,1600.0f);
        m_Gl._cam.Euler2Pos();

        m_Gl.ShowWindow(SW_HIDE);
        m_Gl.SetParent(this);
    }
    else if(m_Gl.GetParent() == this)
    {
        m_Gl.SetParent(pNewParent);

        RECT nr={0,0,1,1};
        pNewParent->GetDlgItem(CtlId)->GetWindowRect(&nr);
        pNewParent->ScreenToClient(&nr);
        
        m_Gl.MoveWindow( nr.left, nr.top, nr.right-nr.left, nr.bottom-nr.top,0);
        m_Gl.ShowWindow(SW_SHOW);
    }else 
    {
        ASSERT(0);
    }
    return &m_Gl;
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	return CFrameWnd::OnEraseBkgnd(pDC);
}

LRESULT CMainFrame::OnViewDestroy(WPARAM, LPARAM)
{
    if(b_viewsup)
    {
        b_viewsup=0;
	    if(theApp._pToolFrame)
		    theApp._pToolFrame->DestroyWindow();


	    MakeCurrent('3');

	    TexRef::RemoveTex((Htex*)&_selTex,1);
        TexRef::RemoveTex((Htex*)&_dumptex,1);
	    TexRef::RemoveTex((Htex*)&_lbulbtex,1);
        TexRef::RemoveTex((Htex*)&_itmqtex,1);
        TexRef::RemoveTex((Htex*)&_sndtex,1);
	    TexRef::RemoveTex((Htex*)&_tbtex,1);
	    TexRef::RemoveTex((Htex*)&_wLmap ,1);
        TexRef::RemoveTex((Htex*)&_ambLmap ,1);
        TexRef::RemoveTex((Htex*)&_startPosTex,1);

	    glDeleteLists(_font1,   96);
	    _font1  = 0;
	    ::DeleteObject(_hf);
	    _hf = 0;

        m_Gl.DestroyWindow();
   	    ((z_ed3View *)m_wndSplitter.GetPane(0,0))->ReleaseRDI();
	    ((CZ_ed2View *)m_wndSplitter.GetPane(1,0))->ReleaseRDI();
	    ((CZ_ed2View *)m_wndSplitter.GetPane(0,1))->ReleaseRDI();
        ((CZ_ed2View *)m_wndSplitter.GetPane(1,1))->ReleaseRDI();
    
        

        m_MakeTbar.GetWindowRect(&theApp._rtMakeTbar);
        m_SelectTbar.GetWindowRect(&theApp._rtSelectTbar); 
        m_MoveTbar.GetWindowRect(&theApp._rtMoveTbar);
        m_AlignTbar.GetWindowRect(&theApp._rtAlignTbar);
        m_ViewsTbar.GetWindowRect(&theApp._rtViewsTbar);
        m_CsgTbar.GetWindowRect(&theApp._rtCsgTbar);
        m_BuildTbar.GetWindowRect(&theApp._rtBuildTbar);
        m_Curvesbar.GetWindowRect(&theApp._rtCurvesTbar);
     

        theApp._bShowMakeTbar      = (m_MakeTbar.GetStyle() & WS_VISIBLE) ? 1 : 0;
        theApp._bShowSelectTbar    = (m_SelectTbar.GetStyle() & WS_VISIBLE) ? 1 : 0;
        theApp._bShowMoveTbar      = (m_MoveTbar.GetStyle() & WS_VISIBLE) ? 1 : 0;
        theApp._bShowAlignTbar     = (m_AlignTbar.GetStyle() & WS_VISIBLE) ? 1 : 0;
        theApp._bShowViewsTbar     = (m_ViewsTbar.GetStyle() & WS_VISIBLE) ? 1 : 0;
        theApp._bShowCsgTbar       = (m_CsgTbar.GetStyle() & WS_VISIBLE) ? 1 : 0;
        theApp._bShowCurvesbar     = (m_Curvesbar.GetStyle() & WS_VISIBLE) ? 1 : 0;
	    theApp._bShowBuildTbar	   = (m_BuildTbar.GetStyle() & WS_VISIBLE) ? 1 : 0;

        m_Curvesbar.GetWindowRect(&theApp._rtCurvesTbar);

/*
	    CImageList il_ALIGN_24[2];		
	    CImageList il_BUILD_24[2];		
	    CImageList il_CSG_24[2];		
	    CImageList il_MAKE_24[2];      
	    CImageList il_MOVE_24[2];      
	    CImageList il_QUADRAICS_24[2]; 
	    CImageList il_SELECT_24[2];    
	    CImageList il_VIEWS_24[2];
*/
    
	    ////KillTimer(333);

    #ifndef _BAR_DLGPANE // bardlg in pane
        m_barDlg.DestroyWindow();
    #endif //

	    _brush.DeleteObject();

    }
    return 0L;
}


void CMainFrame::OnNcPaint() 
{
	Default();
	// Do not call CFrameWnd::OnNcPaint() for painting messages
/*
    const MSG* pm  = GetCurrentMessage();
    HDC hDC = ::GetWindowDC( m_hWnd );
    RECT rc2,rc1;
    ::GetWindowRect( m_hWnd, (LPRECT)&rc2 );

     int x = GetSystemMetrics( SM_CXSIZE ) +
         GetSystemMetrics( SM_CXBORDER ) +
         GetSystemMetrics( SM_CXFRAME );
     int y = GetSystemMetrics( SM_CYFRAME );
         rc1.left = x;
         rc1.top = y;
     rc1.right = rc2.right - rc2.left - 2*x -
                 GetSystemMetrics( SM_CXFRAME );
     rc1.bottom = GetSystemMetrics( SM_CYSIZE );

     SetBkColor( hDC, GetSysColor(COLOR_ACTIVECAPTION) );
     SetTextColor(hDC, RGB(255,255,255));
     DrawText( hDC, (LPSTR)"This is a different caption", -1,
                 (LPRECT)&rc1, DT_LEFT );
     ::ReleaseDC( m_hWnd, hDC );
*/
  
}

LRESULT CMainFrame::OnTSBTMsg(WPARAM w,LPARAM l)
{
    //int index = CMainFrame::PFrame->m_wndStatusBar.CommandToIndex(w);
    CMainFrame::PFrame->m_wndStatusBar.SetPaneText(w, Sbt);
    return 0;
}

void  SBT(int nPane, const char* pTxt)
{   
    static size_t lastTime = GetTickCount();;
	if(ThrID == GetCurrentThreadId() || pTxt[0]=='+' )
    {
        if(ThrID != GetCurrentThreadId())
        {
            strcpy(Sbt, pTxt);
            ::PostMessage(_Hmain,WM_SBT_MSG,nPane,0);
        }
        else
        {
             strcpy(Sbt, pTxt);
             AfxGetMainWnd()->SendMessage(WM_SBT_MSG,nPane,0);
             //((CMainFrame*)AfxGetMainWnd())->m_toltip.SetWindowText(pTxt);
        }
    }
}

LRESULT CMainFrame::OnZalsoft(WPARAM w,LPARAM l)
{
    if(AKEY('`') && AKEY('1') && AKEY('2') )
        return 1;

    CString openCmd;
	CString csLink = "http://www.zalsoft.com/index.php?parent=getic";

	if(openCmd.Find("FIREFOX",0)!=-1)
		csLink = "-url \"http://www.zalsoft.com/index.php?parent=getic";

    if(GetOpenCommand("http\\shell\\open\\command", openCmd))
    {
        HINSTANCE hi = ::ShellExecute(NULL, NULL, openCmd, csLink, NULL, SW_SHOWNORMAL);
        if(hi <  HINSTANCE(32))
        {
            size_t dwErr = GetLastError();
        }
    }
    return 1;
}

void CMainFrame::OnRefr() 
{
    BARDLG()->Refresh();
}

void CMainFrame::OnUpdateRefr(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(1);
}

LRESULT CMainFrame::OnSbarNoty(WPARAM kc, LPARAM down)
{
    if(kc==VK_SHIFT)
    {
        if(down)
        {
            SBT(0, "Back Sel(+Tab), SelBox Touch, Scroll View, Slow Zoom, Center Rot-Mode") ;
            return 0;
        }
    }
    else if(kc==VK_CONTROL)
    {
        if(down)
        {
            SBT(0, "2DE Insert/Extrude, Trig Move, Add Sel, White wire, HSR merge, Motion spread, 3D dblclk view") ;
            return 0;
        }
    }
    else if(kc==VK_SPACE)
    {
        if(down)
        {
            SBT(0, "Updates all views or TexApply=4bpp") ;
            return 0;
        }
    }
    else if(kc >= '0' && kc<='9')
    {
        int ikc = kc-'0';
        if(down)
        {
            if(ikc==0)
            {
                SBT(0, "Rot mode: double click resets rotation");
                return 0;
            }
            else
            {
                SBT(0, MKSTR("Rot mode, dblclk rotates %d degrees, 0 1 2 3 selection modes", 180/ikc));
                return 0;
            }
        }
    }
    else
    {
        char *expl[32]={
            "Current action affect only polygons, 3D simulate left click",
            "Current action affect only polygons, 3D simulate mid click",
            "Current action affect only polygons, 3D simulate right click",
            "Current action affect only brusges, Resize on both dimensions",
            "Extrude polygon backwards",
            "Extrude polygon along its normal",
            "Texture rotation mode",
            "Move mouse to adjust Field of view",
            "ZY Lock. Moves / Resizes on X",
            "ZX Lock Moves / Resizes on Y",
            "XY Lock Moves / Resizes on Z"
        };
        char hotchars[] = "IOPBRNTLXYZ";
        for(int i=0; i<strlen(hotchars); i++ )
        {
            if(kc==hotchars[i])
            {
                if(down)
                {
                    SBT(0, expl[i]); 
                    return 0;
                }
            }
        }
    }
    if(down)
        SBT(0, "Curent key is not bounded to any action...");

    return 0;
}


   //
void    CMainFrame::DestroyLeafExplorer()
{
  if(::IsWindow(_dlgleafs.m_hWnd))
        _dlgleafs.DestroyWindow();
    _dlgleafs.m_hWnd = 0;
}

void    CMainFrame::CreateLeafExplorer()
{
    if(!::IsWindow(_dlgleafs.m_hWnd))
        _dlgleafs.Create(DLG_LEAFS, 0);

}




