#if !defined(AFX_DLGCOMPILERPATH_H__8A72B771_04FC_4403_8D95_EEEE04D6E33E__INCLUDED_)
#define AFX_DLGCOMPILERPATH_H__8A72B771_04FC_4403_8D95_EEEE04D6E33E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCompilerPath.h : header file
//
#include "uienhancer.h"
#include "ColorButon.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCompilerPath dialog

class CDlgCompilerPath : public CBASEDLG
{
// Construction
public:
	CDlgCompilerPath(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCompilerPath)
	enum { IDD = DLG_COMPILERSETT };
	CColorButon	m_3;
	CColorButon	m_2;
	CColorButon	m_1;
	CEdit	_efPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCompilerPath)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCompilerPath)
	afx_msg void OnSelpath();
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMPILERPATH_H__8A72B771_04FC_4403_8D95_EEEE04D6E33E__INCLUDED_)
