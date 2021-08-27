#if !defined(AFX_DLGMOTIONSTATE_H__8303CCE0_F55F_40DA_AA32_9E794B1C2267__INCLUDED_)
#define AFX_DLGMOTIONSTATE_H__8303CCE0_F55F_40DA_AA32_9E794B1C2267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMotionState.h : header file
//
#include "UiEnhancer.h"

/////////////////////////////////////////////////////////////////////////////
// DlgMotionState dialog
struct MState;
class DlgMotionState : public CBASEDLG
{
// Construction
public:
	DlgMotionState(CWnd* pParent = NULL);   // standard constructor
    DlgMotionState(MState* pMState);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgMotionState)
	enum { IDD = DLG_MOTIONSTATE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgMotionState)
	public:
	virtual BOOL OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgMotionState)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    MState*       _pMotionState;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOTIONSTATE_H__8303CCE0_F55F_40DA_AA32_9E794B1C2267__INCLUDED_)
