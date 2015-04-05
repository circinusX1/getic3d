#ifndef __REPLLBOX_H__
#define __REPLLBOX_H__
// RepLBox.h : header file
//
#define LOG_WARNING 0x100000
#define LOG_ERROR   0x200000

/////////////////////////////////////////////////////////////////////////////
// CRepLBox window

class CRepLBox : public CListBox
{
// Construction
public:
	CRepLBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRepLBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRepLBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRepLBox)
	afx_msg void OnDblclk();
	afx_msg void OnDestroy();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
    CBrush _bkBrush;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__REPLLBOX_H__
