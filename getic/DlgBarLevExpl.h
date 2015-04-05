#if !defined(AFX_DLGBARLEVEXPL_H__2AAEBF81_727C_4BF1_A6BC_2138D94C85B4__INCLUDED_)
#define AFX_DLGBARLEVEXPL_H__2AAEBF81_727C_4BF1_A6BC_2138D94C85B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBarLevExpl.h : header file
//
#pragma warning (disable: 4786)
#include "GListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBarLevExpl dialog

class CDlgBarLevExpl : public CBASEDLG
{
// Construction
public:
	CDlgBarLevExpl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBarLevExpl)
	enum { IDD = DLG_LEV_EXPLORE };
	CGListCtrl	_list;
	CTreeCtrl	_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBarLevExpl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBarLevExpl)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBARLEVEXPL_H__2AAEBF81_727C_4BF1_A6BC_2138D94C85B4__INCLUDED_)
