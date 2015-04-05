#if !defined(AFX_BOXITEMS_H__510DD407_E3F0_47C8_B567_FEDFAD10511E__INCLUDED_)
#define AFX_BOXITEMS_H__510DD407_E3F0_47C8_B567_FEDFAD10511E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoxItems.h : header file
//
#pragma warning (disable: 4786)
/////////////////////////////////////////////////////////////////////////////
// CBoxItems window

class CBoxItems : public CComboBox
{
// Construction
public:
	CBoxItems();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoxItems)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBoxItems();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBoxItems)
	afx_msg void OnCloseup();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOXITEMS_H__510DD407_E3F0_47C8_B567_FEDFAD10511E__INCLUDED_)
