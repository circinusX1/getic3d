#if !defined(AFX_DRAWOUT1_H__7040D650_2ECD_439D_95A8_F568121FCDEE__INCLUDED_)
#define AFX_DRAWOUT1_H__7040D650_2ECD_439D_95A8_F568121FCDEE__INCLUDED_

#include "MMove.h"
#include "LMProc.h"
#include "LMProc2.h"
// Drawout1.h : header file
//
/////////////////////////////////////////////////////////////////////////////
class CDrawOut : public CWnd
{
// Construction
public:
    CDrawOut(){_pth=0;};
    BOOL CreateWnd(CWnd* parent, UINT isctrl);
    static void    RegisterClass(){
        WNDCLASS wndclsGL;
        ::memset(&wndclsGL, 0, sizeof(WNDCLASS));   // start with NULL defaults
        wndclsGL.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndclsGL.lpfnWndProc   = ::DefWindowProc;
        wndclsGL.hInstance     = AfxGetInstanceHandle();
        wndclsGL.hbrBackground = 0;
        wndclsGL.hCursor       = ::LoadCursor(0,IDC_ARROW);
        wndclsGL.lpszClassName = "DRAW_DC_WIN";
        BOOL b = AfxRegisterClass(&wndclsGL);
    }

    static void UnregisterClass(){
        ::UnregisterClass("DRAW_DC_WIN",AfxGetApp()->m_hInstance);
    }
// Attributes
public:
    TexHandler      *_pth;
    TexHandler      _th;
    LmSizeBuff      _lmsb ; 
// Operations
public:
    void Repaint(CDC* pDC=0);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawOut)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CDrawOut(){};
    BOOL Create(LPCTSTR cn, LPCTSTR wn, DWORD dws, const RECT& rt, CWnd* pParent, UINT id, CCreateContext* pCtx=0)
    {
	    return CreateEx(WS_EX_TOOLWINDOW, "DRAW_DC_WIN", wn,
		                dws ,
		                rt.left, rt.top,
		                rt.right - rt.left, rt.bottom - rt.top,
                        pParent ? pParent->GetSafeHwnd(): 0, (HMENU)id, (LPVOID)pCtx);
    }
    // Generated message map functions
    
	//{{AFX_MSG(CDrawOut)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	

	DECLARE_MESSAGE_MAP()
    SIZE            _scroll;    
    MMove           _mmove;
    SIZE            _maxscroll;
    static CDrawOut*    p_DrawBig;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWOUT1_H__7040D650_2ECD_439D_95A8_F568121FCDEE__INCLUDED_)
