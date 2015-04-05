#if !defined(AFX_DLGEDITOR2D_H__A7FC44BF_3E29_4BD5_9425_0C7AACC6049A__INCLUDED_)
#define AFX_DLGEDITOR2D_H__A7FC44BF_3E29_4BD5_9425_0C7AACC6049A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditor2D.h : header file
//
#include "geticgui.h"
#include "camera.h"
#include "Curve_Class.h"

/////////////////////////////////////////////////////////////////////////////
/*
We goona hold here 3 objects. A shape whos extruded along a path making 
up a brush
A shape retangular, or a closed bicubic spline
        rectangular     - no of points  (point position and direction)
        spline-closed   - points                      
                         steps
A Path  rectangulr
        spline
Resulting Mesh
*/

// structure for holding data for close shape and path
struct  PathShape
{
    PathShape():_selp(0),nPoints(0),nSteps(0){
    }
    int nPoints;    // points
    int nSteps;     // iterations between points

    void Clear(){
        _points.clear();
        _pathspl.clear();       // spline path
    }
    vvector<VNT>    _points;
    vvector<VNT>    _pathspl;   // spline path
    int             _selp;      // selection point index   
};

/////////////////////////////////////////////////////////////////////////////
class CStaticGL;
class DlgEditor2D : public CBASEDLG
{
public:
    typedef enum _MK_STEPS{STEP_MKSHAPE=0, STEP_MKPATH, STEP_MKBRUSH} MK_STEPS;

    struct    StepState  
    {
        MK_STEPS    _step;
        V_MODE      _viewMode;
        char        _vproj;
    };

// Construction
public:
	DlgEditor2D(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgEditor2D)
	enum { IDD = DLG_EDITOR2D };
	CStatic	m_stout;
	CColorButon	m_clr;
	CStatic	m_curs;
	CColorButon	m_p;
	CColorButon	m_n;
	CColorButon	m_k;
	CColorButon	m_c;
	//}}AFX_DATA
	afx_msg void OnSize(UINT nType, int cx, int cy);
    BOOL OnRButtonUp(UINT nFlags, CPoint& point, V3& wp);
    BOOL OnRButtonDown(UINT nFlags, CPoint& point, V3& wp); 
    BOOL OnMouseMove(UINT nFlags, CPoint& point, V3& wp);
    BOOL OnSetCursor(UINT nFlags, CPoint& point, V3 wp);
    BOOL OnLButtonUp(UINT nFlags, CPoint& point, V3& wp);
    BOOL OnLButtonDown(UINT nFlags, CPoint& point, V3& wp);
    void CreateBrush();
    void EvaluateStep();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEditor2D)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void MakePath();
    void MakeShape();
    void MakeSpline();
    void OnMake() ;

	// Generated message map functions
	//{{AFX_MSG(DlgEditor2D)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	afx_msg void OnPaint();
	afx_msg void OnPrev();
	afx_msg void OnNext();
	afx_msg void OnClear();
	afx_msg void OnSpline();
	afx_msg void OnRect();
	afx_msg void OnChangeCpcts();
	afx_msg void OnChangeStpcts();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNurb();
	afx_msg void OnChangeXYZ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    CStaticGL*   _pStaticGL;
    char        _vt;
    MK_STEPS    _step;    
    PathShape   _shape;
    PathShape   _path;
    PathShape*  _pWorking;
    Brush       _brush;
    MMove       _mm;
    BOOL        _pagechange;
    VNT*        _plastsel;    
    BOOL        _blockupdate;

    StepState   _sstates[3];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITOR2D_H__A7FC44BF_3E29_4BD5_9425_0C7AACC6049A__INCLUDED_)
