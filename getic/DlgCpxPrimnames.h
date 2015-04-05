#if !defined(AFX_DLGCPXPRIMNAMES_H__C9E41245_99E3_4050_9123_FE147B9C2589__INCLUDED_)
#define AFX_DLGCPXPRIMNAMES_H__C9E41245_99E3_4050_9123_FE147B9C2589__INCLUDED_

#include "Lbeditable.h"
#include "ColorButon.h"
#include "dirtree.h"

/////////////////////////////////////////////////////////////////////////////
// DlgCpxPrimnames dialog

class DlgCpxPrimnames : public CBASEDLG
{
// Construction
public:
	DlgCpxPrimnames(CWnd* pParent = NULL);   // standard constructor

    tstring         _curPath;
// Dialog Data
	//{{AFX_DATA(DlgCpxPrimnames)
	enum { IDD = DLG_CMODELSAVE };
	CColorButon	m_ok;
	CColorButon	m_cancel;
	CDirTree	_dirtree;
	CLbEditable	_lbModels;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCpxPrimnames)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCpxPrimnames)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOk3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCPXPRIMNAMES_H__C9E41245_99E3_4050_9123_FE147B9C2589__INCLUDED_)
