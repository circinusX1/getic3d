#if !defined(AFX_TOOLFRAME_H__57D871A5_AF9E_424B_A1AF_8F347406AA9E__INCLUDED_)
#define AFX_TOOLFRAME_H__57D871A5_AF9E_424B_A1AF_8F347406AA9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolFrame.h : header file
#include "CompilerOutDlg.h"
#include "DlgBarScript.h"
/////////////////////////////////////////////////////////////////////////////
// CToolFrame frame
class CSampleView;
class CSampleDoc;
class CToolFrame : public CMiniFrameWnd
{
	friend class DlgBarScript;
public:	
	DECLARE_DYNCREATE(CToolFrame)

	CToolFrame();           // protected constructor used by dynamic creation

// Attributes
public:

	BOOL	CreateTextView();//_textEdit
    void    LoadSciptFile(const char* doc, const char* itm, const char* text);
	void    CanChangeItem();
	void	SetTitle(const char* psz);


    void    AddScriptString(const char* );
    void    SelectScriptString(const char* );
    void    FindScriptFunction(const char* );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	virtual ~CToolFrame();

	// Generated message map functions
	//{{AFX_MSG(CToolFrame)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnScNew();
	afx_msg void OnScOpen();
	afx_msg void OnScSave();
	afx_msg void OnUpdateScSave(CCmdUI* pCmdUI);
	afx_msg void OnSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CSampleView*		_pView;	
    CToolBar            m_Editbar;
    CCompilerOutDlg	    _dlgBar;
    CSampleDoc*			_pDoc;
    DlgBarScript        _dlgLeft;
};

extern CCompilerOutDlg* PBarDlg;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLFRAME_H__57D871A5_AF9E_424B_A1AF_8F347406AA9E__INCLUDED_)
