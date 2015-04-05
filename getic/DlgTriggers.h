#if !defined(AFX_DLGTRIGGERS_H__4CC78DD1_7B93_4E8F_93CC_0FC59A5FB0DF__INCLUDED_)
#define AFX_DLGTRIGGERS_H__4CC78DD1_7B93_4E8F_93CC_0FC59A5FB0DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTriggers.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgTriggers dialog

class DlgTriggers : public CBASEDLG
{
// Construction
public:
	DlgTriggers(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgTriggers)
	enum { IDD = DLG_TRIGERS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

    void    Activate(int ia){};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgTriggers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgTriggers)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRIGGERS_H__4CC78DD1_7B93_4E8F_93CC_0FC59A5FB0DF__INCLUDED_)
