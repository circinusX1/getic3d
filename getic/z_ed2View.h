//---------------------------------------------------------------------------------------
// z_ed2View.h : interface of the CZ_ed2View class
//
//---------------------------------------------------------------------------------------

#ifndef __ZED2VIEW_H__
#define __ZED2VIEW_H__

#pragma warning (disable: 4251)
#pragma warning (disable: 4786)

#include <stdio.h>
#include <gl/gl.h>			
#include <gl/glu.h>			
#include <gl/glaux.h>	
#include "mmove.h"
#include "zedtypes.h"
#include "SceItem.h"
#include "DlgBar.h"
#include "UiEnhancer.h"
#include "MyStatusBar.h"

//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
struct MState;   //cached motion state taken on mousemove not to do same thing again
class CZ_ed2Doc;
class Brush;

class CZ_ed2View : public CView
{
protected: // create from serialization only
	CZ_ed2View();
	DECLARE_DYNCREATE(CZ_ed2View)

// Attributes
public:
	CZ_ed2Doc* GetDocument();
    void ReleaseRDI();

// Operations
public:
	BOOL IsBoxInViewPort(Box& box);
    BOOL IsVxInViewPort(V3& vx);
	void Redraw();
    void Reinit();
    void    GlAfterPaint();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZ_ed2View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZ_ed2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    void RedrawDC(BOOL update=0);
protected:
    void Project2D(int iWidth, int iHeight);
    void GLPaint(RECT&);
	void DoSelection(CPoint&  point);
// Generated message map functions
protected:
	//{{AFX_MSG(CZ_ed2View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg LRESULT OnUsrMsgRepaint(WPARAM,LPARAM); 
    LRESULT OnPostInit(WPARAM pb1, LPARAM pb2);
	DECLARE_MESSAGE_MAP()

    void    SetCursorText(POINT& ct, LPCTSTR str)    ;
    void    MouseWorldMove(REAL mxw, REAL  myw,CPoint& pt);
	void	DrawInHrc(HDC, BOOL update=0);
    void	DrawView(HDC hdc, BOOL update=0);
    void    ScrollView(CPoint& pt);
    void    MoveItem(CPoint& pt);
    void    RotateItem(CPoint& pt);
    void    ScaleItem(CPoint& pt);
    void    MoveBrush(CPoint& pt);
    void    MoveVertex(CPoint& pt);
	void    MovePoly(CPoint& pt);
    void    ScalePoly(CPoint& pt);
    void    RotatePoly(CPoint& pt);

	void    RaiseBrush(CPoint& pt);
    void    ScaleBrush(CPoint& pt,REAL,REAL);
    void    RotateBrush(CPoint& pt);

    BOOL    MannageCursor(CPoint point,BOOL sel);
    void    DrawGrid(RECT& rt);
    void    DrawTopGrid(RECT& rt);
    void    DrawFrontGrid(RECT& rt);
    void    DrawSideGrid(RECT& rt);
    REAL    Wnd2World(REAL coord, char dir);
    int     World2Wnd(REAL coord, char dir);
    void    CheckCoordRestrains();
	void    ResolveCurInBox(Box& acumBox,int& Mx,int& My,int& mx,int& my);
    void    HideTheCursor (void);
    void    ShowTheCursor (void);
    void    MouseMove(UINT nFlags, CPoint point, BOOL fromUp=FALSE);
    void    SnapPoint(CPoint& point);
    void    Rollover(CPoint pt);
    void    GetSelCenter(Poly** pPoly, int count);
    void    GetSelCenter(SceItem** it, int count);
    void    GetSelCenter(Brush** it, int count);
    BOOL    GetSelectionItems(POINT& ptDown);
    V3      GetMouseRotAngle(CPoint& point);
    V3      _GetTranslation(const CPoint& pt);
    int     _MoveBrushMotion(CMotion* pM, V3& t);
    void    Add4Update(Brush* pb);//{_dirty = M_MOVED; pb->Dirty(1); _dirtyBrushes << pb;)
    void    _GetScaleValues(CPoint& pt, V3& s, V3& t);
    void    _DrawLines(int , 	
                                    REAL , 
	                                REAL ,   
	                                REAL ,     
	                                REAL , 
	                                int  );

public:

    C_TOOLBAR    m_TolTipBar;
    C_REBAR      m_wndReBar;

    HGLRC   m_hRC;
    HDC     _hdc;
    char    _vt;
    REAL    _zoom;
    REAL    _shifty;
    REAL    _shiftx;
    RECT    _rt;
    MMove   _mm;
    FRECT   _vport;
    BOOL    _active;
	FPOINT	_prepdn;
    int	    _dirty;
    _BR_MODE  _accmode;
    BOOL    _selBox;
    BOOL    _dd;    //draw dirty. can be redrawned
    DWORD	_scaleDir;
	DWORD	_canMove;
	DWORD	_canMoveDown;
    DWORD	_scaleDirDown;
	int		_depl;			//limits the move on one axis to allow alig on move scale rot
    POINT   _scrPoint;
	V3	    _cccGrd;
    REAL    _grades;
    RECT    _prevrt;
    POINT   _wmouse;
    POINT   _textmouse;
    CString cs_ct;
    REAL    _viewrot;
    
    V3      _selCenter;
    V3      _ptDown;
    V3      _arccircle[3];

    vvector<Brush*> _dirtyBrushes;
};

//---------------------------------------------------------------------------------------
#ifndef _DEBUG  // debug version in z_ed2View.cpp
inline CZ_ed2Doc* CZ_ed2View::GetDocument()
   { return (CZ_ed2Doc*)m_pDocument; }
#endif

//---------------------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Z_ED2VIEW_H__31355EF4_3613_4344_B483_F4C7DDE3C745__INCLUDED_)
