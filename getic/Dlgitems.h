#if !defined(AFX_DLGITEMS_H__111F8863_BDED_48A2_94C1_8DE527020479__INCLUDED_)
#define AFX_DLGITEMS_H__111F8863_BDED_48A2_94C1_8DE527020479__INCLUDED_
#pragma warning (disable: 4786)
#include "uienhancer.h"
#include "LboxItem.h"

/////////////////////////////////////////////////////////////////////////////
// DlgItems dialog
class SceItem;
class DlgItems : public CBASEDLG
{
// Construction
public:
	DlgItems(CWnd* pParent = NULL);   // standard constructor

    void    Update(SceItem* pItem,int selCount=1);
    void    Retreive(SceItem* pItem,int selCount=1);
    void    DelItem(SceItem* pItem);
	void    AddItem(SceItem* pItem);
    void    RefreshItemsList();
    void    Activate(int ia);
    void    Dirty();
    void    ClenContent(){_lbItems.ResetContent();};
    void    OnItemPropsChanged(SceItem* pItem);
    int     OnItemSelected(SceItem* pItem, int sel=1); 
    BOOL    IsNotSameTypeItemsSelected();
    CDialog*	_pCurrent;
// Dialog Data
	//{{AFX_DATA(DlgItems)
	enum { IDD = DLG_ITEMS };
	CComboBox	_cbGroup;
	CLboxItem	_lbItems;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgItems)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
    void    UpdateItem(SceItem* pItem, int idx);

	// Generated message map functions
	//{{AFX_MSG(DlgItems)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDblclkList1();
	afx_msg void OnCloseupGroup();
	afx_msg void OnSelchangeList1();
	afx_msg void OnScript();
	afx_msg void OnCkscript();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    map<int,int> _itemstates;
    BOOL        _blockDocUpdate;
    UINT        _firstItmSelType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGITEMS_H__111F8863_BDED_48A2_94C1_8DE527020479__INCLUDED_)
