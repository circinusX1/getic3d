#if !defined(AFX_COMPILEROUTDLG_H__7D3FD256_2F81_4CA5_9023_5F6B5F564326__INCLUDED_)
#define AFX_COMPILEROUTDLG_H__7D3FD256_2F81_4CA5_9023_5F6B5F564326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompilerOutDlg.h : header file

#include "ColorButon.h"

/////////////////////////////////////////////////////////////////////////////
// CCompilerOutDlg dialog

class CCompilerOutDlg : public CDialogBar
{
// Construction
public:
	CCompilerOutDlg(CWnd* pParent = NULL);   // standard constructor

	void	AddText(const char* );
// Dialog Data
	//{{AFX_DATA(CCompilerOutDlg)
	enum { IDD = DLG_COMPILERPROG };
	
	CColorButon	m_close;
	CColorButon	m_pbprout;
	CListBox	_lbout;
	//}}AFX_DATA

    LRESULT HandleInitDialog2(WPARAM w, LPARAM l);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompilerOutDlg)
	protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCompilerOutDlg)
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnSize(size_t nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnDblclkOut();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    CBrush  _br;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILEROUTDLG_H__7D3FD256_2F81_4CA5_9023_5F6B5F564326__INCLUDED_)
