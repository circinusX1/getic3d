#if !defined(AFX_DLGNEWTERRAIN_H__AFDF87F7_3EEB_44DF_9DA5_E984BD1CF083__INCLUDED_)
#define AFX_DLGNEWTERRAIN_H__AFDF87F7_3EEB_44DF_9DA5_E984BD1CF083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewTerrain.h : header file
//
#include "texlbox.h"
#include "Staticgl.h"
#include "DlgTextures.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNewTerrain dialog

class Brush;
class CDlgNewTerrain : public CBASEDLG
{
// Construction
public:
	CDlgNewTerrain(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDlgNewTerrain)
	enum { IDD = DLG_NEWTERIAN };
	CColorButon	m_c;
	CColorButon	m_g;
	CColorButon	m_o;
    CStaticGL*   _pStaticGL;
	//}}AFX_DATA
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewTerrain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	CreateMesh();
	int		GetVal (BYTE* pBuff, int x, int y);
	void	PutVal (BYTE* pBuff, int x, int y, BYTE value);
	// Generated message map functions
	//{{AFX_MSG(CDlgNewTerrain)
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
    int                 _dimX;
    int                 _dimY;
public:
    Brush*              _pWorkingBrush;
    CDlgTextures        _dlgtex;
    BOOL                b_big;
    BOOL                _dwmode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWTERRAIN_H__AFDF87F7_3EEB_44DF_9DA5_E984BD1CF083__INCLUDED_)
