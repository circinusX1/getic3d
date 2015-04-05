#if !defined(AFX_DLGTRIGGERITEM_H__ECB9F7CF_12CC_4F87_B9E9_F12B0280A083__INCLUDED_)
#define AFX_DLGTRIGGERITEM_H__ECB9F7CF_12CC_4F87_B9E9_F12B0280A083__INCLUDED_
#pragma warning (disable: 4786)

#include "uienhancer.h"
#include "ColorButon.h"

// DlgTriggerItem dialog
class TriggerItem;
class DlgTriggerItem : public CBASEDLG
{
// Construction
public:
	DlgTriggerItem(CWnd* pParent = NULL);   // standard constructor
    void     Show(BOOL b);
// Dialog Data
	//{{AFX_DATA(DlgTriggerItem)
	enum { IDD = DLG_I_TRIGER };
	CListBox	_lbScripts;
	CButton		_ckScript;
	CButton		_ckBrush;
	CButton		_ckSound;
	CListBox	_lbDetBrush;
	//}}AFX_DATA

    virtual BOOL    IsDlgVisible(){return IsWindowVisible();}
    void    Update(SceItem* pItem,int selCount=1);
    void    Retreive(SceItem* pItem,int selCount=1);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgTriggerItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    void    OnAction();
// Implementation
protected:
    void RefreshTriggerEfects();

	// Generated message map functions
	//{{AFX_MSG(DlgTriggerItem)
	virtual void OnCancel();
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkTarget();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	TriggerItem*	_pCurTrigger;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRIGGERITEM_H__ECB9F7CF_12CC_4F87_B9E9_F12B0280A083__INCLUDED_)
