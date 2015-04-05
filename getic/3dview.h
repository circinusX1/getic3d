#if !defined(AFX_3DVIEW_H__4149BD24_5282_48C8_8470_E69A4615EF4F__INCLUDED_)
#define AFX_3DVIEW_H__4149BD24_5282_48C8_8470_E69A4615EF4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DView view

class C3DView : public CView
{
protected:
	C3DView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(C3DView)

// Attributes
public:

// Operations
public:
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~C3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(C3DView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEW_H__4149BD24_5282_48C8_8470_E69A4615EF4F__INCLUDED_)
