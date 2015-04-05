#if !defined(AFX_DLGBSETTOP_H__07480D88_F230_46CF_9383_30391C0BA617__INCLUDED_)
#define AFX_DLGBSETTOP_H__07480D88_F230_46CF_9383_30391C0BA617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBSetTop.h : header file
//
#include "TabDlgSel.h"
#include "DlgBrshProp.h"
#include "geticgui.h"
/////////////////////////////////////////////////////////////////////////////
// DlgBSetTop dialog

class DlgBSetTop : public CBASEDLG, public BrshRequire
{
// Construction
public:
	DlgBSetTop(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgBSetTop)
	enum { IDD = DLG_B_SETPAR };
	TabDlgSel	m_tab;
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
    BOOL    IsVisible(){return 1;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgBSetTop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgBSetTop)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


    DlgBrshProp     _dbp1;
    BrshRequire*    _pChilds[32];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBSETTOP_H__07480D88_F230_46CF_9383_30391C0BA617__INCLUDED_)
