// Author : Marius C.
//---------------------------------------------------------------------------------------
#ifndef __DLG_FACE_PRP_H__
#define __DLG_FACE_PRP_H__
#pragma warning (disable: 4786)
#include "resource.h"
#include "vertdlgdlg.h"
#include "StaticGL.h"
#include "Drawout1.h"

//---------------------------------------------------------------------------------------
class Poly;
class DlgFaceProp : public CVertDlgDlg
{
// Construction
public:
	DlgFaceProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgFaceProp)
	enum { IDD = DLG_F_SET };
	CComboBox	m_cnanims;
	CCheckListBox	m_lbprops;
	CColorButon	m_rspl;
	CColorButon	m_rs;
	CColorButon	m_reuv; 
    CEdit	    _efTanimV;
    CEdit	    _efTanimU;
    CEdit	    m_shine;
	CEdit	    m_stick;
	CEdit	    m_bump;
	CColorButon	m_s;
	CColorButon	m_e;
	CColorButon	m_d;
    CColorButon	_pbColor;
	CEdit	    _efColor;
	//}}AFX_DATA

    CDrawOut	_ctrlt1;
    CDrawOut    _ctrlt0; 
    CDrawOut    _ctrlt2;
    CDrawOut    _ctrlt3;

    void    Update(Poly* p, int selCount=1);
    void    Retreive(Poly* ,int selCount=1);
    void    Activate(int ia){};
    void    Dirty();
    void    OnAction();
	void	EnableAll();
    virtual void    Reset();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgFaceProp)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void    UpdateTextureStage(Poly* pPoly, int nCount, BOOL in);
	// Generated message map functions
	//{{AFX_MSG(DlgFaceProp)
	afx_msg void OnOk();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeEffects();
	afx_msg void OnGensc();
	afx_msg void OnGensc2();
	afx_msg void OnGenss();
	afx_msg void OnGenss2();
	afx_msg void OnResetTC();
	afx_msg void OnResetsplits();
	//}}AFX_MSG
    void OnChkChange() ;
    void OnColor() ;
    void OnDifuze() ;
    void OnEmmisive() ;
    void UseSpliter() ;
    void OnSpec() ;
    LRESULT OnPostInit(LPARAM, WPARAM) ;
	DECLARE_MESSAGE_MAP()
public:
    Poly*       _pCurent;
    CStaticGL   _glWnd;
    int         _last2usesplitidx;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__DLG_FACE_PRP_H__
