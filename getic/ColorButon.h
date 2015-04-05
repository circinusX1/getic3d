#if !defined(AFX_COLORBUTON_H__4B784C5B_D533_4BCA_9177_23B8E1295D24__INCLUDED_)
#define AFX_COLORBUTON_H__4B784C5B_D533_4BCA_9177_23B8E1295D24__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// CColorButon window

class CColorButon : public CButton
{
// Construction
public:
	CColorButon();

// Attributes
public:
    COLORREF    _color;
    CBrush      _brush;
    void    SetColor(COLORREF    color);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButon)
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CColorButon();
	virtual void PreSubclassWindow();
	// Generated message map functions
protected:
    virtual void DrawItem(LPDRAWITEMSTRUCT pDraw);
	//{{AFX_MSG(CColorButon)
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    CString m_tip;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTON_H__4B784C5B_D533_4BCA_9177_23B8E1295D24__INCLUDED_)
