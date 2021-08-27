#ifndef __ZED3DV_H__
#define __ZED3DV_H__
//---------------------------------------------------------------------------------------
//
//   #pragma warning (disable: 4786)

//#include "basearrays.h"
#include "MMove.h"
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
// #include <gl/Glaux.h>
#include "zedtypes.h"

//---------------------------------------------------------------------------------------
// z_ed3View view
class CZ_ed2Doc;
class z_ed3View : public CView
{
protected:
	z_ed3View();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(z_ed3View)

    void  Project3D(int iWidth, int iHeight);
// Attributes
public:
	void RedrawDC(BOOL update=0);

// Operations
public:
    void    GlAfterPaint();
    void ReleaseRDI();
	void Redraw();
    void DrawDirty(int i){_dd=i;}
    void TextOut(V3& v, const char* text, CLR& clr);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(z_ed3View)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
    void GLPaint();
// Implementation
protected:
	virtual ~z_ed3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    
	// Generated message map functions
protected:
	//{{AFX_MSG(z_ed3View)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnKeyDown(size_t nChar, size_t nRepCnt, size_t nFlags);
    afx_msg void OnKeyUp(size_t nChar, size_t nRepCnt, size_t nFlags) ;
	afx_msg void OnMouseMove(size_t nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(size_t nFlags, CPoint point);
	afx_msg void OnLButtonDown(size_t nFlags, CPoint point);
	afx_msg void OnLButtonUp(size_t nFlags, CPoint point);
	afx_msg void OnSize(size_t nType, int cx, int cy);
	afx_msg void OnRButtonDown(size_t nFlags, CPoint point);
	afx_msg void OnRButtonUp(size_t nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(size_t nFlags, short zDelta, CPoint pt);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
    LRESULT OnPostInit(WPARAM pb1, LPARAM pb2);
    
    void    BuildFont(HDC hDC);

	void	DrawInHrc(HDC hdc, BOOL update=0);
    void    HideTheCursor (void);
    void    ShowTheCursor (void);
    void    Rollover(CPoint pt);
    

	DECLARE_MESSAGE_MAP()
    FRECT   _vport;
    BOOL    _active;
	MMove   _mm;
    BOOL    _rotating;

public:
    HDC     _hdc;
    HGLRC   m_hRC;
    BOOL    _dd;    //draw dirty. can be redrawned
    RECT    _rt;
    REAL    _fov;
    REAL    _fnear;
    REAL    _ffar;
    int     _font1;
    HFONT   _hf;
	int		_capture;
    POINT  _scrPoint;
    RECT    _prevrt;
    static z_ed3View* P3DView;
    int     _downKey;
    BOOL    _blockKeyCall;
    BOOL    _blockMouseCall;
};

//---------------------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Z_ED3VIEW_H__21E16863_9BBC_40BF_BEF3_17D6CEC43D72__INCLUDED_)
