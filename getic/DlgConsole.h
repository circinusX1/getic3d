#if !defined(AFX_DLGCONSOLE_H__CB4A24E4_DCAA_447E_AEC8_E751A46AA13A__INCLUDED_)
#define AFX_DLGCONSOLE_H__CB4A24E4_DCAA_447E_AEC8_E751A46AA13A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConsole.h : header file
//
#include "UiEnhancer.h"
#include "ColorButon.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgConsole dialog

class CDlgConsole : public CBASEDLG
{
// Construction
public:
	CDlgConsole(CWnd* pParent = NULL);   // standard constructor

    ~CDlgConsole()
    {
        TRACE("~CDlgConsole");
    }

// Dialog Data
	//{{AFX_DATA(CDlgConsole)
	enum { IDD = DLG_CONSOLE };
	CColorButon	m_r;
	CColorButon	m_c;
	CEdit	_out;
	CEdit	_console;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConsole)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConsole)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRun();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    //CFont _hf;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONSOLE_H__CB4A24E4_DCAA_447E_AEC8_E751A46AA13A__INCLUDED_)
