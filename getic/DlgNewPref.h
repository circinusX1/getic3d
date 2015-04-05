#if !defined(AFX_DLGNEWPREF_H__A4D52713_8E0A_4F50_AD81_780830F933F1__INCLUDED_)
#define AFX_DLGNEWPREF_H__A4D52713_8E0A_4F50_AD81_780830F933F1__INCLUDED_


#include "StaticGL.h"
#include "LboxItem.h"
#include "basecont.h"
#include "dirtree.h"
/////////////////////////////////////////////////////////////////////////////
// DlgNewPref dialog
class Brush;
class DlgNewPref : public CBASEDLG
{
// Construction
public:
	DlgNewPref(CWnd* pParent = NULL);   // standard constructor
    ~DlgNewPref();

// Dialog Data
	//{{AFX_DATA(DlgNewPref)
	enum { IDD = DLG_NEWPREF };
	CColorButon	m_o;
	CColorButon	m_c;
	CDirTree	_tree;
	CLboxItem	lb_prefs;
	//}}AFX_DATA

    Brush* GetBrush(){
        Brush* pBrush = _pStaticGL->_pBrush;
        _pStaticGL->_pBrush = 0; // block deletion
        return pBrush;
    };
    CStaticGL*   _pStaticGL;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgNewPref)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void    PopulateList();
	// Generated message map functions
	//{{AFX_MSG(DlgNewPref)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnSelchangePrefs();
	afx_msg void OnDblclkPrefs();
	afx_msg void OnCut();
	afx_msg void OnCutall();
	afx_msg void OnSplitable();
	afx_msg void OnSolid();
	//}}AFX_MSG
	afx_msg LRESULT OnTreeChanged(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
    vvector<Texture>    _texNames;
    tstring             _curPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWPREF_H__A4D52713_8E0A_4F50_AD81_780830F933F1__INCLUDED_)
