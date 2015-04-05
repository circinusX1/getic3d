#ifndef __DLG_NERWBRUSH_H__
#define __DLG_NERWBRUSH_H__
//----------------------------------------------------------------------------------------
// DlgNewBrush.h : header file
#include "UiEnhancer.h"
#include "brush.h"
#include "resource.h"


//----------------------------------------------------------------------------------------
// CDlgNewBrush dialog
struct SNewBrush
{
	SNewBrush()
	{
		_tm = TM_LIN;
	}
    BYTE    _vstrips;
    BYTE    _radstrips;
    V3      _boxdim;
    REAL    _thikness;
    TEX_MAP _tm;

    BOOL    _isSolid    : 1;
    BOOL    _detail  : 1;
    BOOL    _reversed   : 1;
    BOOL	_cutAll     : 1;
    BOOL	_splitter   : 1;
    BOOL    _zone       : 1;
    BOOL    _skyDom     : 1;
    BOOL    _detached   : 1;

};


class CDlgNewBrush : public CBASEDLG
{
// Construction
public:
	CDlgNewBrush(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNewBrush)
	enum { IDD = DLG_NEWBRUSH };
	CColorButon	m_o;
	CColorButon	m_c;
	CSpinButtonCtrl	m_splr;
	CSpinButtonCtrl	m_splv;
	CEdit	m_radstrips;
	CEdit	m_vstrips;
	CEdit	m_thikness;
	CEdit	_height;
	CEdit	_thick;
    CEdit	_width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewBrush)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewBrush)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnShell();
	afx_msg void OnAutodetail();
	afx_msg void OnSplitable();
	afx_msg void OnSkydom();
	afx_msg void OnReverse();
	afx_msg void OnSplitter();
	afx_msg void OnZone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
    SNewBrush   _brshSett;
    BOOL        _hideZone;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWBRUSH_H__F654A883_00E5_4CA1_B956_20F5C2531DAF__INCLUDED_)
