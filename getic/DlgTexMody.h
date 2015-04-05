#if !defined(AFX_DLGTEXMODY_H__CA27AF28_27AE_4055_892E_31335E503A4C__INCLUDED_)
#define AFX_DLGTEXMODY_H__CA27AF28_27AE_4055_892E_31335E503A4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTexMody.h : header file
//
#include "geticgui.h"
#include "MMove.h"
#include "DrawOut1.h"



class DlgTexMody : public CDialog
{
// Construction
public:
    friend class CDrawOut;
	DlgTexMody(CWnd* pParent = NULL);   // standard constructor
    void Paint(CPaintDC& dc) ;
// Dialog Data
	//{{AFX_DATA(DlgTexMody)
	enum { IDD = DLG_TEX_MODY };
	CDrawOut	m_static;
	CSliderCtrl	m_i;
	CSliderCtrl	m_g;
	CSliderCtrl	m_l;
	CSliderCtrl	m_c;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgTexMody)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgTexMody)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSave();
	afx_msg void OnSaveas();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBw();
	afx_msg void OnBw2();
	afx_msg void OnMask();
	afx_msg void OnNegative();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnReload();
	//}}AFX_MSG
    /**
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    */

    DECLARE_MESSAGE_MAP()

    TexHandler      _th;
    BOOL            _loaded;

    int             _gamma[3];
    int             _luminance[3];
    int             _copntrast[3];
    int             _comp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEXMODY_H__CA27AF28_27AE_4055_892E_31335E503A4C__INCLUDED_)
