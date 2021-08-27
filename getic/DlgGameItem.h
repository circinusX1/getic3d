#if !defined(AFX_DLGGAMEITEM_H__6A072E9B_CC02_43AB_AA1E_2E920A9FB8B9__INCLUDED_)
#define AFX_DLGGAMEITEM_H__6A072E9B_CC02_43AB_AA1E_2E920A9FB8B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGameItem.h : header file
// For item Game items Catheory and Game Item Props
#include "Prplb.h"
#include "UiEnhancer.h"
/////////////////////////////////////////////////////////////////////////////
// DlgGameItem dialog
class ItemCat;
class SceItem;
class DlgGameItem : public CBASEDLG
{
// Construction
public:
	DlgGameItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgGameItem)
	enum { IDD = DLG_I_GAMEITM };
	CPrpLb	m_lb;
	//}}AFX_DATA
    void     Show(BOOL b);
    virtual BOOL    IsDlgVisible(){return IsWindowVisible();}
    void    Update(SceItem* pItem,int selCount=1);
    void    Retreive(SceItem* pItem,int selCount=1);
    
    // use same dialog for cath and props
    void    Update(ItemCat* pItem);
    void    Retreive(ItemCat* pItem);
    BOOL    NotyFoo(CLbDlg* pDlg);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGameItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGameItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeProps();
	afx_msg void OnDblclkPropscat();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, size_t nStatus);
	//}}AFX_MSG
	LRESULT OnRepaint(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	int			_reroute;		
    ItemCat*    _curCat;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGAMEITEM_H__6A072E9B_CC02_43AB_AA1E_2E920A9FB8B9__INCLUDED_)
