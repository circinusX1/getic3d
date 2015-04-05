#if !defined(AFX_LBOXITEM_H__D17FEEC0_7FB3_48D4_841F_209AE38A0E7D__INCLUDED_)
#define AFX_LBOXITEM_H__D17FEEC0_7FB3_48D4_841F_209AE38A0E7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LboxItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLboxItem window

class CLboxItem : public CListBox
{
// Construction
public:
	CLboxItem();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLboxItem)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLboxItem();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLboxItem)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    CImageList   _imageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LBOXITEM_H__D17FEEC0_7FB3_48D4_841F_209AE38A0E7D__INCLUDED_)
