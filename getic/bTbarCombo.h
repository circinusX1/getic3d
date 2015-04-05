#if !defined(AFX_BTBARCOMBO_H__5891AA0F_A761_419C_810F_D6D16B922FF4__INCLUDED_)
#define AFX_BTBARCOMBO_H__5891AA0F_A761_419C_810F_D6D16B922FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// bTbarCombo.h : header file
//
#pragma warning (disable: 4786)
/////////////////////////////////////////////////////////////////////////////
// CbTbarCombo window

class CbTbarCombo : public CComboBox
{
// Construction
public:
	CbTbarCombo();


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CbTbarCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CbTbarCombo();

	// Generated message map functions

	//{{AFX_MSG(CbTbarCombo)
	afx_msg void OnCloseup();
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTBARCOMBO_H__5891AA0F_A761_419C_810F_D6D16B922FF4__INCLUDED_)
