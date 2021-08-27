#ifndef __FIX_SPLITTER_H__
#define __FIX_SPLITTER_H__

/////////////////////////////////////////////////////////////////////////////
// CFixSplitter window

class CFixSplitter : public CSplitterWnd
{
// Construction
public:
	CFixSplitter(int idim=4); 

    BOOL HasInfos() {return m_pColInfo && m_pRowInfo;};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixSplitter)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFixSplitter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFixSplitter)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(size_t nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(size_t nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, size_t nHitTest, size_t message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(size_t nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    HCURSOR _hlr[2];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXSPLITTER_H__B81FE72D_50B5_45BA_AE58_6863759E91A1__INCLUDED_)
