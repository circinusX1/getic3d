#if !defined(AFX_DLGPREFERENCES_H__531B1B6F_3A49_4559_B039_0AD8676CBB44__INCLUDED_)
#define AFX_DLGPREFERENCES_H__531B1B6F_3A49_4559_B039_0AD8676CBB44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPreferences.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgPreferences dialog

class DlgPreferences : public CDialog
{
// Construction
public:
	DlgPreferences(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgPreferences)
	enum { IDD = DLG_PREFS };
	CButton	m_11;
	CButton	m_10;
	CButton	m_9;
	CButton	m_8;
	CButton	m_7;
	CButton	m_6;
	CButton	m_5;
	CButton	m_4;
	CButton	m_3;
	CButton	m_2;
	CButton	m_1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgPreferences)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPREFERENCES_H__531B1B6F_3A49_4559_B039_0AD8676CBB44__INCLUDED_)
