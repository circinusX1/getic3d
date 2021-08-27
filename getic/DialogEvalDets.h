#if !defined(AFX_DIALOGEVALDETS_H__F326E442_D6C0_48E1_93A0_938F4F7A6254__INCLUDED_)
#define AFX_DIALOGEVALDETS_H__F326E442_D6C0_48E1_93A0_938F4F7A6254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogEvalDets.h : header file
//

#include "basecont.h"

class CDialogEvalDets : public CDialog
{
// Construction
public:
	CDialogEvalDets(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogEvalDets)
	enum { IDD = DLG_AUTODETAILS };
	CColorButon	m_ca;
	CColorButon	m_ok;
	CSliderCtrl	m_slide;
	//}}AFX_DATA
    BOOL _onScreen;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEvalDets)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void    DetalizeSelectedFaces();
    void    DetalizeSelectedBrushes();
    void    MarkBrushes();
    void    MarkFaces();

	// Generated message map functions
	//{{AFX_MSG(CDialogEvalDets)
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnDestroy();
	afx_msg void OnFaces();
	afx_msg void OnBrushes();
	afx_msg void OnBypolis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    vvector<Poly*>   _polys;
    vvector<Brush*>  _brushes;
    vvector<size_t>   _flags;
    size_t             _rbchecked;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEVALDETS_H__F326E442_D6C0_48E1_93A0_938F4F7A6254__INCLUDED_)
