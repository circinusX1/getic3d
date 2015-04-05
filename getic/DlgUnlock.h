#if !defined(AFX_DLGUNLOCK_H__B0A8CDD3_130D_493F_A8EA_D86817D78874__INCLUDED_)
#define AFX_DLGUNLOCK_H__B0A8CDD3_130D_493F_A8EA_D86817D78874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUnlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgUnlock dialog

class DlgUnlock : public CDialog
{
// Construction
public:
	DlgUnlock(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgUnlock)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgUnlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgUnlock)
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnUnlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUNLOCK_H__B0A8CDD3_130D_493F_A8EA_D86817D78874__INCLUDED_)
