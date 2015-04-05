#if !defined(AFX_DLGGAMECATS_H__6A396A10_DB38_46E6_AEA8_CB51B057404C__INCLUDED_)
#define AFX_DLGGAMECATS_H__6A396A10_DB38_46E6_AEA8_CB51B057404C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgClassItem.h : header file
//
#include "LboxItem.h"
#include "ColorButon.h"
#include "DlgGameItem.h"
/////////////////////////////////////////////////////////////////////////////
// DlgClassItem dialog

class DlgClassItem : public CBASEDLG
{
// Construction
public:
	DlgClassItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgClassItem)
	enum { IDD = DLG_CLASS_ITEM };
	CColorButon	m_browse;
	CColorButon	m_cancel;
	CLboxItem	m_lb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgClassItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int     PopulateFromCats();


	// Generated message map functions
	//{{AFX_MSG(DlgClassItem)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	afx_msg void OnSelchangeItemcat();
	afx_msg void OnOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    DlgGameItem         _gameitemDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGAMECATS_H__6A396A10_DB38_46E6_AEA8_CB51B057404C__INCLUDED_)
