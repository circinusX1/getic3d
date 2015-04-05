#if !defined(AFX_GLISTCTRL_H__8DD421F2_B242_4C73_B157_06069ABF9E96__INCLUDED_)
#define AFX_GLISTCTRL_H__8DD421F2_B242_4C73_B157_06069ABF9E96__INCLUDED_

#include "UiEnhancer.h"

// GListCtrl.h : header file
//
#define 	WM_ITEMSEL2CLICK	(WM_USER+1236)
#define     WM_ITEMSEL			(WM_USER+1237)

/////////////////////////////////////////////////////////////////////////////
// CGListCtrl window

class CGListCtrl : public CListCtrl
{
// Construction
public:
	CGListCtrl();

// Attributes
public:
    int  GetCurSel(){return _curSel;}
    void SetCurSel(int sel);
    void SetHeadersNames(LPCSTR pFirst,...);
    int  AddNewItem(LPCSTR pFirst,...);
    int  LstSetItem(int iItem, LPCSTR pFirst,...);
    void Block(BOOL up=1){_updating=up;}
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGListCtrl)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    int			    _curSel;
	BOOL		    _updating;
    C_HEADERCTRL    _hdrCtrl; //CHeaderCtrl
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLISTCTRL_H__8DD421F2_B242_4C73_B157_06069ABF9E96__INCLUDED_)
