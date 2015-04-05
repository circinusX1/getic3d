#ifndef _LBPROPS_H__
#define _LBPROPS_H__

#include "resource.h"
#include "geticgui.h"
#include "UiEnhancer.h"
//---------------------------------------------------------------------------------------
#define     WM_DLGCLICK     WM_USER+1004
#define     WM_DLGEVENT     WM_USER+1005

/*
DL_CBSEL:
DL_CHECKS:
DL_COLOR:
DL_EDIT:
DL_EDSPIN:
DL_EDXYZ:
DL_RANGE:
DL_UV:
DL_FILEPCK:
*/
class CLbDlg;
typedef BOOL    (CDialog::*PFNOTY)(CLbDlg*);





class CLbDlg : public CBASEDLG
{
// Construction
public:
	CLbDlg(UINT iDD, CWnd* pParent = NULL);   // standard constructor
    void    SetIndex(int idx){_index = idx;}



    virtual void    OnDeselect() = 0;    
    virtual void    OnSelect() = 0;
    virtual void    Apply();
    void            Activate(BOOL b);
    void            Noty();
    //combo
    

    CComboBox	    _cb;    
    BOOL            _lockNoty;
    // file open save filters
    CString         _szFile;
    CString         _fileExt;
    CString         _fileFilter;
    CString         _initalPath;
    BOOL            _bFileOpen;
    BOOL            _onlyFile;
    CEdit	        _e1;
    char           _propName[64];
    // color
    COLORREF        _color;

    // chk visible
    int             _ckNumVis;
    BOOL            _visible;
    
    // edit range
    float   _curValueX;
    float   _spMinX;
    float   _spMaxX;
    float   _curValueY;
    float   _spMinY;
    float   _spMaxY;
    float   _curValueZ;
    float   _spMinZ;
    float   _spMaxZ;
    float   _spStep; 
    PFNOTY  _pNoty;

    // xyz
	BOOL    _selected;

    RECT    _initalRect;

    
// Dialog Data
	//{{AFX_DATA(CLbDlg)
	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLbDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    void OnOK(){};
    void OnCancel(){};
// Implementation
protected:
    
	// Generated message map functions
	//{{AFX_MSG(CLbDlg)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnInitDialog();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
    
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    int     _index;
    
};

//---------------------------------------------------------------------------------------
class DlbXyz : public CLbDlg
{
// Construction
public:
	DlbXyz(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbXyz)
	enum { IDD = DL_EDXYZ };
	//}}AFX_DATA

    void    OnDeselect() ;    
    void    OnSelect() ;
    void    Apply();


	CSpinButtonCtrl	_sp1;
	CSpinButtonCtrl	_sp2;
	CSpinButtonCtrl	_sp3;
	CEdit	        _e1;
	CEdit	        _e2;
	CEdit	        _e3;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbXyz)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbXyz)
	afx_msg void OnChangeX();
	afx_msg void OnChangeY();
	afx_msg void OnChangeZ();
	afx_msg void OnDeltaposX(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposY(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposZ(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//---------------------------------------------------------------------------------------
class DlbUV : public CLbDlg
{
// Construction
public:
	DlbUV(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbUV)
	enum { IDD = DL_UV };
	//}}AFX_DATA

    void    OnDeselect() ;    
    void    OnSelect() ;
    void    Apply();


	CSpinButtonCtrl	_sp1;
	CSpinButtonCtrl	_sp2;
	CEdit	        _e1;
	CEdit	        _e2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbUV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbUV)
	afx_msg void OnChangeU();
	afx_msg void OnChangeV();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//---------------------------------------------------------------------------------------

class DlbRANGE : public CLbDlg
{
// Construction
public:
	DlbRANGE(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbRANGE)
	enum { IDD = DL_RANGE };
	//}}AFX_DATA
    void    OnDeselect() ;    
    void    OnSelect() ;
    void    Apply();

    CSliderCtrl	    _s1;
    CEdit           _e1;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbRANGE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbRANGE)
	afx_msg void OnReleasedcapture1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChange1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------------------------------

class DlbFILE : public CLbDlg
{
// Construction
public:
	DlbFILE(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbFILE)
	enum { IDD = DL_FILEPCK };
	CColorButon	m_ob1;
	//}}AFX_DATA
    void    OnDeselect() ;    
    void    OnSelect() ;
    void    Apply();

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbFILE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbFILE)
	afx_msg void OnButton1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnChange1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        BOOL _bchanging;
};
//---------------------------------------------------------------------------------------
class DlbEDSpin : public CLbDlg
{
// Construction
public:
	DlbEDSpin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbEDSpin)
	enum { IDD = DL_EDSPIN };
	//}}AFX_DATA
    void    OnDeselect() ;    
    void    OnSelect() ;
    void    Apply();


	CSpinButtonCtrl	_sp1;
	CEdit	        _e1;
    int             _prevVal;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbEDSpin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbEDSpin)
	afx_msg void OnDeltapos1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChange1();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//---------------------------------------------------------------------------------------
class DlbEDIT : public CLbDlg
{
// Construction
public:
	DlbEDIT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbEDIT)
	enum { IDD = DL_EDIT };
	//}}AFX_DATA
    void    OnDeselect() ;    
    void    OnSelect() ;
    void   Apply();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbEDIT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbEDIT)
	afx_msg void OnChange1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//---------------------------------------------------------------------------------------

class DlbCLR : public CLbDlg
{
// Construction
public:
	DlbCLR(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbCLR)
	enum { IDD = DL_COLOR };
	CColorButon	m_bc;
	//}}AFX_DATA
    void    OnDeselect() ;    
    void    OnSelect() ;
  void   Apply();


    // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbCLR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbCLR)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnButton2();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    CBrush _brush;
};
//---------------------------------------------------------------------------------------

class DlbCKs : public CLbDlg
{
// Construction
public:
	DlbCKs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbCKs)
	enum { IDD = DL_CHECKS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

    void    OnDeselect() ;    
    void    OnSelect() ;
    void   Apply();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbCKs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbCKs)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------------------------------


class DlbCB : public CLbDlg
{
// Construction
public:
	DlbCB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbCB)
	enum { IDD = DL_CBSEL };
	
	//}}AFX_DATA

    void    OnDeselect() ;    
    void    OnSelect() ;
    void    Apply();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbCB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbCB)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



class DlbRB :public CLbDlg
{
// Construction
public:
    
	DlbRB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlbRB)
	enum { IDD = DL_RGB1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
    void    OnDeselect(){} ;    
    void    OnSelect(){} ;
    void    Apply(){};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlbRB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlbRB)
	afx_msg void On33();
	afx_msg void On22();
	afx_msg void On11();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//---------------------------------------------------------------------------------------
class CPrpLb : public CListBox
{
// Construction
public:
	CPrpLb();
    int            _labelRaport;
// Attributes
public:
    // ad items. returns the proper DlgControl
    void       Clean() ;
    CLbDlg*    AddItem(char* str, CLbDlg* lbDlg, UINT nID);
    CLbDlg*    AddItem(char* str, UINT nID);
    CLbDlg*    GetCurItemSel(int idx){
        return (CLbDlg*)GetItemData(idx);
    }
	CLbDlg*    GetCurItemSel(const char* sidx){
		return _dlgAsoc[sidx];
	}
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrpLb)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrpLb();

   	afx_msg void OnSelchange();
    void    SeeRects();
	// Generated message map functions
protected:
	//{{AFX_MSG(CPrpLb)
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSetfocus();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg LRESULT OnSetCurSel(WPARAM, LPARAM);
    afx_msg LRESULT OnRedraw(WPARAM, LPARAM);
    afx_msg void OnPaint();
    

    int     _dlgXDim;
    int     _dlgYDim;
	DECLARE_MESSAGE_MAP()
    int     _curSel;
	map<tstring,CLbDlg*>	_dlgAsoc;
    BOOL                    _invDlg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRPLB_H__88C81D62_988F_4140_BEA4_79C614656F98__INCLUDED_)
