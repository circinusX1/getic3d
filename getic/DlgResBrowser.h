#if !defined(AFX_DLGRESBROWSER_H__17CA3A9A_D70C_4650_B0E9_2728835F1E90__INCLUDED_)
#define AFX_DLGRESBROWSER_H__17CA3A9A_D70C_4650_B0E9_2728835F1E90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgResBrowser.h : header file
//
#include "DirTree.h"
#include "Drawout1.h"
#include "TexLbox.h"
#include "ColorButon.h"
/////////////////////////////////////////////////////////////////////////////
// DlgResBrowser dialog

class DlgResBrowser : public CDialog, public CarryTex
{
// Construction
public:
	DlgResBrowser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgResBrowser)
	enum { IDD = DLG_TEXBROWSER };
	CColorButon	m_ss;
	CColorButon	m_cc;
	CTexLbox	m_texlb;
	CDirTree	m_tree;
	//}}AFX_DATA
    CDrawOut    w_draw;    


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgResBrowser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


    void  GetTextureName(int idx, LPSTR pName){
        m_texlb.GetText(idx ,pName);
    }
    const char*   GetTextureFullName(int i){
        if(i==-1)i = m_texlb.GetCurSel();
        return i>=0 ? _texFiles[i].c_str() : "";
    }

// Implementation
protected:
    void PopSubDirText(LPCTSTR subDir);


	// Generated message map functions
	//{{AFX_MSG(DlgResBrowser)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelect();
	afx_msg LRESULT OnSelchangedDirs(WPARAM,LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    vvector<tstring>  _texFiles;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESBROWSER_H__17CA3A9A_D70C_4650_B0E9_2728835F1E90__INCLUDED_)
