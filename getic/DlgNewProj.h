#if !defined(AFX_DLGNEWPROJ_H__CF0FE61C_385C_4530_96EF_5742C8529107__INCLUDED_)
#define AFX_DLGNEWPROJ_H__CF0FE61C_385C_4530_96EF_5742C8529107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewProj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgNewProj dialog

class DlgNewProj : public CDialog
{
// Construction
public:
	DlgNewProj(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgNewProj)
	enum { IDD = DLG_NEWPRJ };
	CString	m_prjName;
	CString	m_location;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgNewProj)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgNewProj)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedPjtype(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangingPjtype(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEdit1();
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnChangeEdit6();
	afx_msg void OnKillfocusEdit6();
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWPROJ_H__CF0FE61C_385C_4530_96EF_5742C8529107__INCLUDED_)
