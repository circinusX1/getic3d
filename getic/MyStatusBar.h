#if !defined(AFX_MYSTATUSBAR_H__FDAF154A_8E37_4BAC_9D36_9B11935A485B__INCLUDED_)
#define AFX_MYSTATUSBAR_H__FDAF154A_8E37_4BAC_9D36_9B11935A485B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatusBar.h : header file
//
#include "ColorButon.h"
/////////////////////////////////////////////////////////////////////////////
// CMyStatusBar window

class CMyStatusBar : public CStatusBar
{
// Construction
public:
	CMyStatusBar();

// Attributes
public:
    void    SetSelection(int selEntities);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyStatusBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyStatusBar();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyStatusBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATUSBAR_H__FDAF154A_8E37_4BAC_9D36_9B11935A485B__INCLUDED_)
