// z-edmap.h : main header file for the Z_ED2 application
//

#ifndef __ABOUT_DLG_H__
#define __ABOUT_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"       // main symbols
#include "Maskedit.h"       // main symbols
#include "basecont.h"
#include "Dlgbar.h"
#include "geticgui.h"
#include "Staticgl.h"


//---------------------------------------------------------------------------------------
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	C_Static	m_st;
	CColorButon	m_od1;
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, size_t nHitTest, size_t message);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(size_t nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    CStaticGL   *_pStaticGL;
public:
    size_t        m_timer;
    Htex        m_htAbout;
    Htex        m_htNoise;
    BYTE*       p_texBuff;
    size_t       dw_uptime;
};


#endif //__ABOUT_DLG_H__
