#if !defined(AFX_DLGBRSET2_H__DEC21C37_35D7_466C_AB5B_4D44D2F6A2AE__INCLUDED_)
#define AFX_DLGBRSET2_H__DEC21C37_35D7_466C_AB5B_4D44D2F6A2AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBrSet2.h : header file
//
#include "DlgBrshProp.h"
/////////////////////////////////////////////////////////////////////////////
// DlgBrSet2 dialog

class DlgBrSet2 : public CBASEDLG, public BrshRequire
{
// Construction
public:
	DlgBrSet2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgBrSet2)
	enum { IDD = DLG_B_SET2 };
	CGListCtrl	    _lcMove;
	CEdit	        _efSteps;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
    BOOL            _dirty;

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
	//{{AFX_VIRTUAL(DlgBrSet2)
	public:
	virtual BOOL OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgBrSet2)
	afx_msg void OnOk();
	virtual void OnCancel();
    afx_msg void OnDynamic();
    afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, size_t nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    Brush*      _pCurBrush;
    BOOL        _updatingCtrls;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBRSET2_H__DEC21C37_35D7_466C_AB5B_4D44D2F6A2AE__INCLUDED_)
