#if !defined(AFX_TREESCENE_H__502E5860_3662_4F05_A30C_45BF1ADC0B08__INCLUDED_)
#define AFX_TREESCENE_H__502E5860_3662_4F05_A30C_45BF1ADC0B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeScene.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TreeScene window

class TreeScene : public CTreeCtrl
{
// Construction
public:
	TreeScene();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TreeScene)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TreeScene();

	// Generated message map functions
protected:
	//{{AFX_MSG(TreeScene)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    CImageList  i_ml;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREESCENE_H__502E5860_3662_4F05_A30C_45BF1ADC0B08__INCLUDED_)
