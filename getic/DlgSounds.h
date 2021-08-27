#if !defined(AFX_DLGSOUNDS_H__D2416D86_445B_4EC5_A101_60F1F5F59EB6__INCLUDED_)
#define AFX_DLGSOUNDS_H__D2416D86_445B_4EC5_A101_60F1F5F59EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSounds.h : header file
//
//   #pragma warning (disable: 4786)

#include "basecont.h"
#include "GListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// DlgSounds dialog

class DlgSounds : public CBASEDLG
{
// Construction
public:
	DlgSounds(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgSounds)
	enum { IDD = DLG_SOUNDS };
	CEdit	m_efv;
	CEdit	m_efr;
	CGListCtrl	m_lasoc;
	CGListCtrl	m_list;
	//}}AFX_DATA

    void    Activate(int ia){};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgSounds)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void    BuildFileList();
    void    PopulateAssociations(int index);
	LRESULT  OnItemSelected(WPARAM iSel, LPARAM);
	// Generated message map functions
	//{{AFX_MSG(DlgSounds)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPlay();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk();
	afx_msg void OnShowWindow(BOOL bShow, size_t nStatus);
	afx_msg void OnChangeRad();
	afx_msg void OnChangeVol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    BOOL    _dirty;
    int     _ncursel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOUNDS_H__D2416D86_445B_4EC5_A101_60F1F5F59EB6__INCLUDED_)
