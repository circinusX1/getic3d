// Author : Marius C.
//---------------------------------------------------------------------------------------

#ifndef __DLG_BRUSH_PROPS_H__
#define __DLG_BRUSH_PROPS_H__

#include "VertDlgDlg.h"
#include "GListCtrl.h"
#include "Prplb.h"



//---------------------------------------------------------------------------------------
class CMotion;
class Brush;
class DlgBrshProp : public CVertDlgDlg
{
// Construction
public:
	DlgBrshProp(CWnd* pParent = NULL);   

// Dialog Data
	//{{AFX_DATA(DlgBrshProp)
	enum { IDD = DLG_B_SET };
	CColorButon	m_badd;
	CColorButon	m_b1;
	CEdit	m_taidx;
	CEdit	m_taz;
	CEdit	m_tax;
	CColorButon	m_p32;
	CColorButon	m_p3;
	CColorButon	m_p2;
	CColorButon	m_p1;
	CColorButon	m_snd;
	CColorButon	m_add;
	CGListCtrl	m_snds;
	CComboBox	_cbGroup;
	CEdit	_efName;
	CGListCtrl	    _lcMove;
	CEdit	        _efSteps;
	CSpinButtonCtrl	_stSteps;
	CComboBox	    _cbDynamic;
	CComboBox	    _cbContent;
	//}}AFX_DATA

    void    UpdateMotionProps(Brush*, CMotion* pM, BOOL bRefresh = FALSE);
    void    Update(Brush* pBrush, int selCount=1);
    void    Retreive(Brush* pBrush, int selCount=1);
    void    Activate(int ia){};
    void    Dirty();
    void    Clean();
    LRESULT    OnItemSelected(WPARAM iSel, LPARAM);
    LRESULT    OnItemDoubleClicked(WPARAM iSel, LPARAM);
	void    OnAction();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgBrshProp)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgBrshProp)
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnDetail();
    afx_msg void OnDynamic();
	afx_msg void OnCut();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnCloseupContent();
	afx_msg void OnShowWindow(BOOL bShow, size_t nStatus);
	afx_msg void OnAdd();
	afx_msg void OnSkydom();
	afx_msg void OnNocoll();
	afx_msg void OnRotate();
	afx_msg void OnCloseupGroup();
	afx_msg void OnP2();
	afx_msg void OnP1();
	afx_msg void OnP3();
	afx_msg void OnP32();
	afx_msg void OnTriangs();
	afx_msg void OnNobsp();
	afx_msg void OnDisable();
	afx_msg void OnScript();
	afx_msg void OnCkscript();
	afx_msg void OnCutall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    Brush*      _pCurBrush;
	int			_scrollPage;
    int         _selCount;
    int         _paneNo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __DLG_BRUSH_PROPS_H__
