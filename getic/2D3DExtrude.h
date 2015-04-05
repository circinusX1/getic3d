#if !defined(AFX_2D3DEXTRUDE_H__CBAE6C43_A8A1_4CAF_A98D_43F98907A31C__INCLUDED_)
#define AFX_2D3DEXTRUDE_H__CBAE6C43_A8A1_4CAF_A98D_43F98907A31C__INCLUDED_

#include "uienhancer.h"
#include "geticgui.h"
#include "camera.h"
/////////////////////////////////////////////////////////////////////////////
class CStaticGL;
class C2D3DExtrude :  public CBASEDLG
{
// Construction
public:
	C2D3DExtrude(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C2D3DExtrude)
	enum { IDD = DLG_EDITOR2DEXTRUDE };
	CColorButon	m_cncl;
	CColorButon	m_c;
	CColorButon	m_b2;
	CColorButon	m_b1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C2D3DExtrude)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(C2D3DExtrude)
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnClear();
	afx_msg void OnHallo();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

    BOOL OnRButtonUp(UINT nFlags, CPoint& point, V3& wp);
    BOOL OnRButtonDown(UINT nFlags, CPoint& point, V3& wp); 
    BOOL OnMouseMove(UINT nFlags, CPoint& point, V3& wp);
    BOOL OnSetCursor(UINT nFlags, CPoint& point, V3 wp);
    BOOL OnLButtonUp(UINT nFlags, CPoint& point, V3& wp);
    BOOL OnLButtonDown(UINT nFlags, CPoint& point, V3& wp);


    CStaticGL*      _pStaticGL;
    Brush           _brush;
    MMove           _mm;
    vvector<V3>     _path;
    int             _selp;
    

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_2D3DEXTRUDE_H__CBAE6C43_A8A1_4CAF_A98D_43F98907A31C__INCLUDED_)
