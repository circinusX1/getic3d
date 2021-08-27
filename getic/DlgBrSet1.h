#if !defined(AFX_DLGBRSET1_H__FA0A1503_34D6_4C29_B667_E11EBD234622__INCLUDED_)
#define AFX_DLGBRSET1_H__FA0A1503_34D6_4C29_B667_E11EBD234622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBrSet1.h : header file
//
#include "DlgBrshProp.h"
/////////////////////////////////////////////////////////////////////////////
// DlgBrSet1 dialog

class DlgBrSet1 : public CBASEDLG, public BrshRequire
{
// Construction
public:
	DlgBrSet1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgBrSet1)
	enum { IDD = DLG_B_SET1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

    void    UpdateMotionProps(Brush*, CMotion* pM, BOOL bRefresh = FALSE);
    void    Update(Brush* pBrush, int selCount=1);
    void    Retreive(Brush* pBrush, int selCount=1);
    void    Activate(int ia);
    void    Dirty();
    void    Clean();
    void    OnItemSelected(int iSel);
    void    OnItemDoubleClicked(int iSel);
	void    OnCommand();
    BOOL    IsVisible(){return IsWindowVisible();};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgBrSet1)
	public:
	virtual BOOL OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgBrSet1)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, size_t nStatus);
	afx_msg void OnCloseupGroup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBRSET1_H__FA0A1503_34D6_4C29_B667_E11EBD234622__INCLUDED_)
