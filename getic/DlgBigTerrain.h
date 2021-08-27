#ifndef __DLG_BIGTERRAIN_H__
#define __DLG_BIGTERRAIN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewTerrain.h : header file
//
#include "texlbox.h"
#include "Staticgl.h"
#include "DlgTextures.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgBigTerrain dialog

class Brush;
class CDlgBigTerrain : public CBASEDLG
{
// Construction
public:
	CDlgBigTerrain(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDlgBigTerrain)
	enum { IDD = DLG_NEWTERIAN };
	CColorButon	m_c;
	CColorButon	m_g;
	CColorButon	m_o;
    CStaticGL*   _pStaticGL;
	//}}AFX_DATA
    size_t      _untiledTex[5];//CK_NOTILE
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBigTerrain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		GetVal (BYTE* pBuff, int x, int y);
	void	PutVal (BYTE* pBuff, int x, int y, BYTE value);
	// Generated message map functions
	//{{AFX_MSG(CDlgBigTerrain)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnGen();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
    LRESULT HandelTextureSel(WPARAM,LPARAM);//WM_HANDLE_TEXSEL
	DECLARE_MESSAGE_MAP()
protected:
    void        _ReplaceTerrain(BYTE* heightBuff);
    int         _dimX;
    int         _dimY;
public:
    CDlgTextures        _dlgtex;
    BOOL                _dwmode;
    CBigTerrain         _terrain;
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__DLG_BIGTERRAIN_H__
