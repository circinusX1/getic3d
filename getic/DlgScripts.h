#if !defined(AFX_DLGSCRIPTS_H__3690A87F_E3BA_4C28_854C_28773E0CB0D3__INCLUDED_)
#define AFX_DLGSCRIPTS_H__3690A87F_E3BA_4C28_854C_28773E0CB0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScripts.h : header file
//
#pragma warning (disable: 4786)
#include "GListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// DlgScripts dialog

class DlgScripts : public CBASEDLG
{
// Construction
public:
	DlgScripts(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgScripts)
	enum { IDD = DLG_SCRIPT };
	CGListCtrl	_list;
	//}}AFX_DATA

    void    Activate(int ia){};
	BOOL	Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgScripts)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgScripts)
	afx_msg void OnNew();
	afx_msg void OnDel();
	afx_msg void OnEdit();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	LRESULT OnItemSelected(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCRIPTS_H__3690A87F_E3BA_4C28_854C_28773E0CB0D3__INCLUDED_)
