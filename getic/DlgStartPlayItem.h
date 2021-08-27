#ifndef _DLG_STARTPLAY_ITEM_H__
#define _DLG_STARTPLAY_ITEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStartPlayItem.h : header file
//
#include "basecont.h"
#include "UiEnhancer.h"
#include "ColorButon.h"
#include "VertDlgDlg.h"
#include "Texlbox.h"

/////////////////////////////////////////////////////////////////////////////
// DlgStartPlayItem dialog

class DlgStartPlayItem : public CVertDlgDlg, public CarryTex
{
// Construction
public:
	DlgStartPlayItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgStartPlayItem)
	enum { IDD = DLG_I_STARTPLAY };
	//}}AFX_DATA

    void    Update(SceItem* pItem,int selCount=1);
    void    Retreive(SceItem* pItem,int selCount=1);
    virtual BOOL    IsDlgVisible(){return IsWindowVisible();}
    void     Show(BOOL b);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgStartPlayItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void OnAction() ;

	// Generated message map functions
	//{{AFX_MSG(DlgStartPlayItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgStartPlayItem_H__F3CA0B94_0508_473E_A5FC_009B00BF3923__INCLUDED_)
