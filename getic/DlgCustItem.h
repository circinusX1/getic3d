#ifndef __CUST_ITEM_H__
#define __CUST_ITEM_H__



#include "ColorButon.h"
#include "DlgCustItem.h"
/////////////////////////////////////////////////////////////////////////////
// DlgCustItem dialog

class DlgCustItem : public CBASEDLG
{
// Construction
public:
	DlgCustItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCustItem)
	enum { IDD = DLG_CUSTITEM };
	//}}AFX_DATA

    CGListCtrl	m_lv;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCustItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCustItem)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeItemcat();
	afx_msg void OnOk();
	virtual void OnCancel();
    LRESULT OnItemChanged(WPARAM w,LPARAM l);
    void OnAddItem();
    void OnDelItem();
    void OnMoveDown();
    void OnMoveUp();
    //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    void UpdateStruct();

    CFont   _hf;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // 
