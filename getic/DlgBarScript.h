#if !defined(AFX_DLGBARSCRIPT_H__41564C61_3106_49D6_B366_D71C0A4B6308__INCLUDED_)
#define AFX_DLGBARSCRIPT_H__41564C61_3106_49D6_B366_D71C0A4B6308__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBarScript.h : header file
//
#include "geticgui.h"
#include "TreeScene.h"
/////////////////////////////////////////////////////////////////////////////
// DlgBarScript dialog
class SceItem;
class Brush;
class Poly;
class CToolFrame;
class DlgBarScript : public CDialogBar
{
public:

	// assoc with item data
	struct	Event
	{
		char    fooProto[128];
        char    msgNName[32];
        DWORD   msg;
	};



// Construction
public:
	DlgBarScript(CWnd* pParent = NULL);   // standard constructor
    void        AttachItemScriptFile(LPCTSTR fName);


    CToolFrame*     _pToolFrame;
// Dialog Data
	//{{AFX_DATA(DlgBarScript)
	enum { IDD = DLG_SCRIPT_BAR };
	CListBox	_lbEvents;
	TreeScene	_scTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgBarScript)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    LRESULT HandleInitDialog2(WPARAM, LPARAM);

// Implementation
protected:
    void	PopulateScenTabCtrl();
    BOOL	ShowOptionsForScript();
	void    PopulateFromFile(const char* szTmplFname);
	void	ClearListBox();
	// Generated message map functions
	//{{AFX_MSG(DlgBarScript)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnSelchangedScene(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingScene(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditScene(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditScene(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturnScene(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkEvents();
	//}}AFX_MSG
    
	DECLARE_MESSAGE_MAP()
    BOOL        _bLockUpdate;
    HTREEITEM   _hSelected;
    CImageList  _treeImg;
    Brush*      _pCurBrush;
    Poly*       _pCurPoly;
    SceItem*    _pCurItem;        
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBARSCRIPT_H__41564C61_3106_49D6_B366_D71C0A4B6308__INCLUDED_)
