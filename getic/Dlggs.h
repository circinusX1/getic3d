#ifndef __DHGGLOBS_H__
#define __DHGGLOBS_H__

#pragma warning (disable: 4786)

#include "UiEnhancer.h"
//---------------------------------------------------------------------------------------
// CDlgGs dialog

class CDlgGs : public CBASEDLG
{
// Construction
public:
	CDlgGs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGs)
	enum { IDD = DLG_GS };
	CColorButon	m_hd;
	CColorButon	m_ok;
	CSpinButtonCtrl	m_camfar;
	CEdit	m_edeps;
	CSpinButtonCtrl	m_spin;
	int		m_intsp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGs)
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposMaxdim(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposMindim(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposMinpolys(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeMindim();
	afx_msg void OnChangeMinpolys();
	afx_msg void OnChangeMaxdim();
	afx_msg void OnCheck1();
	afx_msg void OnBackface();
	afx_msg void OnLight();
	afx_msg void OnShowcuts();
	afx_msg void OnDeltaposCamfar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDivby10();
	afx_msg void OnForce();
	afx_msg void OnCarve();
	afx_msg void OnHomedir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__DHGGLOBS_H__
