// z_ed2.h : main header file for the Z_ED2 application
//

#if !defined(AFX_Z_ED2_H__E1604506_B2FF_4294_90AB_56AD88B6A276__INCLUDED_)
#define AFX_Z_ED2_H__E1604506_B2FF_4294_90AB_56AD88B6A276__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"       // main symbols

#define WM_REDRAW   WM_USER+1000
/////////////////////////////////////////////////////////////////////////////
// CZ_ed2App:
// See z_ed2.cpp for the implementation of this class
//

class CZ_ed2App : public CWinApp
{
public:
	CZ_ed2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZ_ed2App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int DoMessageBox(const char* lpszPrompt, size_t nType, size_t nIDPrompt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CZ_ed2App)
	afx_msg void OnAppAbout();
	afx_msg void OnShowcompiler();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void Dbg_ResolveCurDir();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Z_ED2_H__E1604506_B2FF_4294_90AB_56AD88B6A276__INCLUDED_)
