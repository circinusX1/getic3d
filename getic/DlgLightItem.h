#ifndef __DLG_LIGHTITEM_H__
#define __DLG_LIGHTITEM_H__

#include "basecont.h"
#include "uienhancer.h"
#include "ColorButon.h"
#include "VertDlgDlg.h"
#include "TexLbox.h"


// DlgLightItem dialog
class CLightBulb;
class DlgLightItem : public CVertDlgDlg, public CarryTex
{
// Construction
public:
	DlgLightItem(CWnd* pParent = NULL);   // standard constructor
    void     Show(BOOL b);
// Dialog Data
	//{{AFX_DATA(DlgLightItem)
	enum { IDD = DLG_I_LIGHT };
	CColorButon	_pbColor;
    CColorButon	_pbColorSpec;
	//}}AFX_DATA
    void    Update(SceItem* pItem,int selCount=1);
    void    Retreive(SceItem* pItem,int selCount=1);
    virtual BOOL    IsDlgVisible(){return IsWindowVisible();}
    void    OnAction();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgLightItem)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgLightItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnColor();
	afx_msg void OnSpot();
	afx_msg void OnHallo();
	afx_msg void OnChangeFilter();
	afx_msg void OnDblclkTex();
	afx_msg void OnDlight();
	afx_msg void OnSpec();
	afx_msg void OnQuadratic();
	//}}AFX_MSG
	afx_msg void OnRadioButtons(UINT iCmd);
	DECLARE_MESSAGE_MAP()
    BOOL        _filterChanged;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __DLG_LIGHTITEM_H__
