// UiEnhancer.h: interface for the UiEnhancer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __UIENHANCER_H__
#define __UIENHANCER_H__

#include "ColorButon.h"
#include <set>

using namespace std;
#define     WM_REDRAW_DLG           (WM_USER+1238)
#define     WM_POST_WM_INITDIALOG   (WM_USER+1239)

class C_Rebar : public CReBar
{
// Construction
public:
    C_Rebar(){};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C_Rebar)
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~C_Rebar(){
        TRACE("~C_Rebar()");
    };

	// Generated message map functions
protected:
	//{{AFX_MSG(C_Rebar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class C_ToolBar : public CToolBar
{
// Construction
public:
    C_ToolBar(){};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C_ToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~C_ToolBar(){};

	// Generated message map functions
protected:
	//{{AFX_MSG(C_ToolBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class C_HeaderCtrl : public CHeaderCtrl
{
// Construction
public:
    C_HeaderCtrl(){};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C_HeaderCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~C_HeaderCtrl(){};
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// Generated message map functions
protected:
	//{{AFX_MSG(C_HeaderCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
class TabDlgSel;
class C_TabCtrl : public CTabCtrl
{
// Construction
public:
    friend class TabDlgSel;// for vc7
    C_TabCtrl(){};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C_TabCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~C_TabCtrl(){};
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// Generated message map functions
protected:
	//{{AFX_MSG(C_TabCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
public:
	DECLARE_MESSAGE_MAP()
};

class C_Menu : public CMenu
{
// Construction
public:
    C_Menu(){};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C_Menu)
	//}}AFX_VIRTUAL
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
//	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// Implementation
public:
    virtual ~C_Menu(){
        TRACE("~C_Menu()");
    };

	// Generated message map functions
protected:
	//{{AFX_MSG(C_Menu)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};

class C_Static : public CStatic
{
// Construction
public:
	C_Static();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C_Static)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C_Static();

	// Generated message map functions
protected:
	//{{AFX_MSG(C_Static)
	afx_msg HBRUSH CtlColor(CDC* pDC, size_t nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// for VC Stupid stupid 7------------------------------------------------------------
class DlgGameItem;
class CDlgCompilerPath;
class DlgTriggerItem;
class SceItem;

//-----------------------------------------------------------------------------------
class CBaseDlg : public CDialog
{
// Construction
public:
    friend class    DlgGameItem;
    friend class     CDlgCompilerPath;


    CBaseDlg(const char* lpszTemplateName, CWnd* pParentWnd = NULL):CDialog(lpszTemplateName, pParentWnd),_dirty(0),_updatingCtrls(1),_itmType(0),_pCurItem(0){};
	CBaseDlg(size_t nIDTemplate, CWnd* pParentWnd = NULL):CDialog(nIDTemplate, pParentWnd ),_dirty(0),_updatingCtrls(0),_itmType(0),_pCurItem(0){};
    void    DisableAllCtrls();

// Dialog Data
	//{{AFX_DATA(CBaseDlg)

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
    virtual BOOL OnInitDialog();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseDlg)

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation


	// Generated message map functions
	//{{AFX_MSG(CBaseDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor);
	afx_msg void OnNcPaint();
    afx_msg void OnShowWindow(BOOL bShow, size_t nStatus);
	//}}AFX_MSG
    LRESULT OnRedraw(WPARAM, LPARAM);
    LRESULT OnPostInitDialog(WPARAM, LPARAM);
    virtual void    Dirty();
    virtual void    OnAction(){};
    virtual BOOL    IsRetreiveBlocked();
    BOOL            OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

    void            SetItmType(size_t iType){_itmType=iType;}
    size_t           ItemType(){return _itmType;}
    virtual void    Update(SceItem* pItem,int selCount=1){
    };
    virtual void    Retreive(SceItem* pItem,int selCount=1){
    };
	virtual void	BlockBarUpdate(){_updatingCtrls=TRUE;};
    virtual BOOL    IsDlgVisible(){return FALSE;}
    virtual void    Show(BOOL){};
    virtual BOOL    CanShowDetDlg(){return 1;};    
    BOOL			_dirty;    

public:
    void            ClearDirtyIDs(){_touchedIDs.clear();};
	DECLARE_MESSAGE_MAP()

    C_Static        _allstats;
    set<size_t>       _touchedIDs;  
    BOOL            _updatingCtrls;
    size_t           _itmType;
    SceItem*        _pCurItem;

};


#ifdef _SKINNY
    typedef C_TabCtrl       C_TABCTRL;
    typedef C_HeaderCtrl    C_HEADERCTRL;
    typedef C_ToolBar       C_TOOLBAR;
    typedef C_Rebar         C_REBAR;
    typedef CBaseDlg        CBASEDLG;
#else 
    typedef CTabCtrl       C_TABCTRL;
    typedef CHeaderCtrl    C_HEADERCTRL;
    typedef CToolBar       C_TOOLBAR;
    typedef CReBar         C_REBAR;
    typedef CBaseDlg       CBASEDLG ;
#endif 





#endif // !__UIENHANCER_H__
