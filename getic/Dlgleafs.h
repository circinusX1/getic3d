#if !defined(AFX_DLGLEAFS_H__002BF4A5_4308_44D8_A3D8_5F2C04FAB050__INCLUDED_)
#define AFX_DLGLEAFS_H__002BF4A5_4308_44D8_A3D8_5F2C04FAB050__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLeafs.h : header file
//
#include "GListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgLeafs dialog
class CMiniNode;
class CDlgLeafs : public CBASEDLG
{
// Construction
public:
	CDlgLeafs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLeafs)
	enum { IDD = DLG_LEAFS };
	CComboBox	m_cbc;
	CColorButon	m_c;
	CGListCtrl	m_lv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLeafs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLeafs)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickTexrefs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnSelchangeContent();
	//}}AFX_MSG
    LRESULT OnItemChanged(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
    int _iSel;
    CMiniNode* _prevLeaf;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLEAFS_H__002BF4A5_4308_44D8_A3D8_5F2C04FAB050__INCLUDED_)
