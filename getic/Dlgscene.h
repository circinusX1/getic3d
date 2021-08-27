#ifndef __DLGSCENE_H__
#define __DLGSCENE_H__
//   #pragma warning (disable: 4786)
#include "bspfilestr.h"
#include "zedtypes.h"
#include "TreeScene.h"
#include "ColorButon.h"
#include "Drawout1.h"
#include "VertDlgDlg.h"
#include "PrpListCtrl.h"

//---------------------------------------------------------------------------------------
class BaseEntity;
class CDlgScene : public CVertDlgDlg
{
// Construction
public:
	void Retreive();
    void Update();
	CDlgScene(CWnd* pParent = NULL);   // standard constructor
    virtual void OnExpanded();
// Dialog Data
	//{{AFX_DATA(CDlgScene)
	enum { IDD = DLG_SCENE };
	CColorButon	        m_l;
	CColorButon	        m_bx;
	CEdit	            m_det;
	CColorButon	        m_bow;
	TreeScene	        m_tree;
	PrpListControl	    _prpList;
	//}}AFX_DATA
    CDrawOut    _paintzone; 

    void    Activate(int ia){};
    void    Dirty();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScene)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScene)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, size_t nStatus);
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg void OnRefresh();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnKillfocusDetails();
	afx_msg void OnLogo();
	afx_msg void OnDellogo();
	//}}AFX_MSG
    void PopTreeScene();
	BOOL NotyFoo(PrpListControl* pList, int index);
    LRESULT OnDirty(WPARAM w,LPARAM l){Dirty();return 0;}
	DECLARE_MESSAGE_MAP()
    BaseEntity*     p_be;    
    EL_SEL          _oldMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCENE_H__C4C6473D_1D08_4A05_9AC7_5CB3849F910B__INCLUDED_)
