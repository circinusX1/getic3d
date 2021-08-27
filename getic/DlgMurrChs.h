#if !defined(AFX_DLGMURRCHS_H__E3A97B10_3E09_4F0D_96A7_80BD619E8B07__INCLUDED_)
#define AFX_DLGMURRCHS_H__E3A97B10_3E09_4F0D_96A7_80BD619E8B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMurrChs.h : header file
//
#include "geticgui.h"
/////////////////////////////////////////////////////////////////////////////
// DlgMurrChs dialog

class DlgMurrChs : public CDialog
{
// Construction
public:
	DlgMurrChs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgMurrChs)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgMurrChs)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgMurrChs)
	virtual void OnCancel();
	afx_msg void OnChar(size_t nChar, size_t nRepCnt, size_t nFlags);
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMURRCHS_H__E3A97B10_3E09_4F0D_96A7_80BD619E8B07__INCLUDED_)
