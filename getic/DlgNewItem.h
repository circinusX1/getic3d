#if !defined(AFX_DLGNEWITEM_H__6FC18961_5B8D_4EE5_B134_A9CB6D60D5C8__INCLUDED_)
#define AFX_DLGNEWITEM_H__6FC18961_5B8D_4EE5_B134_A9CB6D60D5C8__INCLUDED_

#include "DlgTriggerItem.h"
#include "DlgLightItem.h"
#include "DlgGameItem.h"
#include "DlgStartPlayItem.h"
#include "GameItem.h"
#include "LboxItem.h"
/////////////////////////////////////////////////////////////////////////////
// DlgNewItem dialog
class GameItem;
class DlgNewItem : public CBASEDLG
{
// Construction
public:
	DlgNewItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgNewItem)
	enum { IDD = DLG_NEWITEM };
	CColorButon	m_o;
	CColorButon	m_c;
	CColorButon	m_b1;
	CLboxItem	_lbItems;
	//}}AFX_DATA
	SceItem*	_pCurItem;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgNewItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgNewItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnButton1();
	afx_msg void OnSelchangeItems();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    void UpdateItemPosition(SceItem* pItem);
    void UpdateFromCat(GameItem* pitem);

    DlgSoundItem    _dlgSoundItem;
	DlgLightItem    _dlgLightItem;    
    DlgTriggerItem  _dlgTriggerItem;    
    DlgGameItem     _dlgGameItem;
    GameItem        _defgi;
    DlgStartPlayItem    _dlgStartItem;

	CDialog*	_pCurDlg;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWITEM_H__6FC18961_5B8D_4EE5_B134_A9CB6D60D5C8__INCLUDED_)
