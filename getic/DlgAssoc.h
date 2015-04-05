#ifndef __DLG_ASSOC_H__
#define __DLG_ASSOC_H__

// DlgLeafs.h : header file
#include "UiEnhancer.h"
#include "LbEditable.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAssoc dialog
class CDlgAssoc : public CBASEDLG
{
// Construction
public:
	CDlgAssoc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAssoc)
	enum { IDD = DLG_ASSOC };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAssoc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAssoc)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    CString s_name;
    int     e_type;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLEAFS_H__002BF4A5_4308_44D8_A3D8_5F2C04FAB050__INCLUDED_)
