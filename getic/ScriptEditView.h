#if !defined(AFX_SCRIPTEDITVIEW_H__F7C245F9_E145_4781_B563_740883E2800D__INCLUDED_)
#define AFX_SCRIPTEDITVIEW_H__F7C245F9_E145_4781_B563_740883E2800D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView view

class CScriptEditView : public CView
{
public:
	CScriptEditView();           // protected constructor used by dynamic creation
    virtual ~CScriptEditView();
	DECLARE_DYNCREATE(CScriptEditView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEditView)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation

	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScriptEditView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTEDITVIEW_H__F7C245F9_E145_4781_B563_740883E2800D__INCLUDED_)
