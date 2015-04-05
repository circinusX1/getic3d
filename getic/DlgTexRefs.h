#if !defined(AFX_DLGTEXREFS_H__FFCD36C2_235F_423F_B523_410EAB3F7451__INCLUDED_)
#define AFX_DLGTEXREFS_H__FFCD36C2_235F_423F_B523_410EAB3F7451__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTexRefs.h : header file
//
#include "GListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTexRefs dialog

class CDlgTexRefs : public CBASEDLG
{
// Construction
public:
	CDlgTexRefs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTexRefs)
	enum { IDD = DLG_TEXREF };
	CColorButon	m_c;
	CGListCtrl	m_lv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTexRefs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTexRefs)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickTexrefs(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    BOOL PreTranslateMessage(MSG* pMsg);
    LRESULT OnItemChanged(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
    int _iSel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEXREFS_H__FFCD36C2_235F_423F_B523_410EAB3F7451__INCLUDED_)
