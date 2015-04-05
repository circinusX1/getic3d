#if !defined(AFX_DLGSOUNDITEM_H__F3CA0B94_0508_473E_A5FC_009B00BF3923__INCLUDED_)
#define AFX_DLGSOUNDITEM_H__F3CA0B94_0508_473E_A5FC_009B00BF3923__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSoundItem.h : header file
//
#include "basecont.h"
#include "uienhancer.h"
#include "ColorButon.h"
#include "VertDlgDlg.h"
#include "TexLbox.h"

/////////////////////////////////////////////////////////////////////////////
// DlgSoundItem dialog

class DlgSoundItem : public CVertDlgDlg, public CarryTex
{
// Construction
public:
	DlgSoundItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgSoundItem)
	enum { IDD = DLG_I_SOUND };
	CColorButon	m_cbb;
	CColorButon	m_cb;
	CComboBox	m_cba;
	//}}AFX_DATA

    void    Update(SceItem* pItem,int selCount=1);
    void    Retreive(SceItem* pItem,int selCount=1);
    virtual BOOL    IsDlgVisible(){return IsWindowVisible();}
    void     Show(BOOL b);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgSoundItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void OnAction() ;

	// Generated message map functions
	//{{AFX_MSG(DlgSoundItem)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSpot();
	afx_msg void OnAmbient();
	afx_msg void OnPlay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOUNDITEM_H__F3CA0B94_0508_473E_A5FC_009B00BF3923__INCLUDED_)
