//@@@@@@@@@@@@@@@@@@@@@@@@@
#if !defined(AFX_STATICGL_H__93624CAF_5EA2_45FE_BAE9_B2A4B696BA81__INCLUDED_)
#define AFX_STATICGL_H__93624CAF_5EA2_45FE_BAE9_B2A4B696BA81__INCLUDED_


#include <gl/gl.h>			
#include <gl/glu.h>			
#include <gl/glaux.h>	
#include "mmove.h"	
#include "camera.h"	
#include "brush.h"	
#include "zedtypes.h"

enum V_MODE{
    V_3DFRONT,
    V_3D,
    V_2D,
};
enum V_ACTION
{
    A_NONE  =   -1,
    A_SHIFTVIEW,
    A_SIZEVIEW,
};

#define BUTS         8
//
typedef void (*CbSize)(CWnd* pwnd, int x, int y);
typedef void (*CbPrepPaint)(CWnd* pdc, V_MODE mode, BOOL init);
typedef void (*CbPaint)(CWnd* pdc, V_MODE mode);
typedef void (*CbInit)(CWnd* pwnd);
typedef BOOL (*CbMouse)(UINT msg, V_ACTION m, CPoint& pt, V3& xyz) ;


// CStaticGL window
class Brush;
class CStaticGL : public CWnd
{
public:

// Construction
public:
    CStaticGL(){
        _pBrush = 0;
        _dist  = -800;
        _bsel   = FALSE;
        _pcbsz    = 0;
        _pcbpaint = 0;
        _pcbpreppaint = 0;
        _pcbinit  = 0;
        _pcbmouse = 0;
        _pcbsetcursor = 0;
        b_showGrid  = 0;
    };

    ~CStaticGL()
    {
        TRACE("~CStaticGL()");
    }
    int     GetViewIndex(){ return (int)(_vt-'x');};
    void    InitialUpdate();
    BOOL    CreateWnd(CWnd* parent, UINT isctrl);
    void    GLPaint();
    void    UseSelection(BOOL b= TRUE){_bsel=b;}
    REAL    Wnd2World(REAL coord, char dir);
    V3      Wnd2World2(int x, int y );
    void    DrawGrids();
    void    DrawViewModes();
    void    SelectTextCtx();
    void    DeSelectTextCtx();
    void    PrintOut(V3& v, const char* text, CLR& clr=ZLIGHTGRAY);
    void    PrintOut(int x, int y, const char* text, CLR& clr=ZLIGHTGRAY);
    void    HideButtons(BOOL bHide){_bHide=bHide; GLPaint();};
    void    UpdateButSels();
    void    GlMakeCurrent(){MakeCurrent(_hdc, m_hRC);} 
    void    EnableLighting(BOOL be){_lighting = be;}
    void    SetMode(V_MODE vm, char c){
        SetViewport(c); 
        _nMode = vm; 
        UpdateButSels();
    };
    void    SetViewport(char c){
        switch(c)
        {
            case 'x':
                _nMode = V_2D;
                _pushMode=2;
                _vt = c; 
                break;
            case 'y':
                _nMode = V_2D;
                _pushMode=3;
                _vt = c; 
                break;
            case 'z':
                _nMode = V_2D;
                _pushMode=4;
                _vt = c; 
                break;
            default:
                _pushMode=0;
                _vt = '*'; 
                break;
        }
        GLPaint();
    }
// Attributes
public:
    
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticGL)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
    Brush*  _pBrush;
    HDC     _hdc;
    HGLRC   m_hRC;

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticGL)
	afx_msg void OnSelchangePrefs();
	afx_msg void OnDblclkPrefs();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void Perspective(int iWidth, int iHeight) ;
	BOOL TestButtons(CPoint& pt, BOOL bGoDown);
    
    V3      _angle;
    UINT    _timer;
    BOOL    _bsel;
public:
    REAL    _dist;
    CbSize      _pcbsz;
    CbPaint     _pcbpaint;
    CbPrepPaint _pcbpreppaint;
    CbInit      _pcbinit;
    CbMouse     _pcbmouse;
    CbMouse     _pcbsetcursor;
    BOOL        _bHide;
    V3          _curPos;
    FRECT       _vport;
    RECT        _rt;
    REAL        _zoom;
    REAL        _shifty2;
    REAL        _shiftx2;

    REAL        _shifty;
    REAL        _shiftx;
    REAL        _shiftz;	
    BOOL        _moving;
    BOOL        _scaling;
    BOOL        _vireframe;
    MMove       _mm;
    char        _vt;
    V_MODE      _nMode;
    V_ACTION    _nAction;
    Camera      _cam;
	Brush		_toolbar[BUTS];
	REAL		_bhght;
	int			_selMode;
	int			_pushMode;
	SmartTex	_tbartex;//"IDR_GLTBAR"
    BOOL        b_showGrid;
    RGBA         _backgrnd;
    BOOL        _lighting;
    //"IDR_GLTBAR"
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICGL_H__93624CAF_5EA2_45FE_BAE9_B2A4B696BA81__INCLUDED_)
