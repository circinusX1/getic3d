#if !defined(AFX_TABDLGSEL_H__0224E6CD_BABC_4F30_87C5_1A00A9025FAC__INCLUDED_)
#define AFX_TABDLGSEL_H__0224E6CD_BABC_4F30_87C5_1A00A9025FAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDlgSel.h : header file
//
#pragma warning (disable: 4786)
#include "uienhancer.h"
/////////////////////////////////////////////////////////////////////////////
// TabDlgSel window
class CDlgBar;
class TabDlgSel : public C_TABCTRL
{
// Construction
public:
	TabDlgSel();

// Attributes
public:

// Operations
public:
	void AddDialog(CBASEDLG * pDlg, LPCSTR pTitle,int iMage, UINT iD );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TabDlgSel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TabDlgSel();

	// Generated message map functions
public:
	//{{AFX_MSG(TabDlgSel)
	afx_msg void OnSelchangeDlgs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingDlgs(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
    CDlgBar*                 _pParent;
    map<CDialog*, UINT>     _rects;   
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDLGSEL_H__0224E6CD_BABC_4F30_87C5_1A00A9025FAC__INCLUDED_)
