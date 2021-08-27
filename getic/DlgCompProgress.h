#ifndef __DLG_PROG_H__
#define __DLG_PROG_H__

#include "resource.h"
#include "Replbox.h"
#include "ColorButon.h"
#include "UiEnhancer.h"
//----------------------------------------------------------------------------
// CDlgCompProgress dialog
#define WM_SETCTLTEXT       WM_USER+1003
#define PBM_SETTEXTCOLOR    WM_USER + 1004
#define PBM_SETSHOWTEXT     WM_USER + 1005

/////////////////////////////////////////////////////////////////////////////
//class CDlgCompProgress : public CBASEDLG -> CDialogBar
class CDlgCompProgress : public CBASEDLG
{
// Construction
public:
	CDlgCompProgress(CWnd* pParent = NULL);   // standard constructor


    void    Progress(int pos=-1);
    void    LogPos(size_t flag, V3& pos, const char* lpszFormat,...);
    void    Log(size_t flag, const char*,...);
    void    Clear()
    {
       if(!_onScreen)
            return;
       if(!::IsWindow(m_hWnd))
           return;
       	_repLB.ResetContent();
        m_stp.SetPos(0);
    }
// Dialog Data
	//{{AFX_DATA(CDlgCompProgress)
	enum { IDD = DLG_COMPILERPROG };
	CRepLBox	_repLB;
    CProgressCtrl	m_stp;
   	//}}AFX_DATA
    CColorButon	m_b1;
    CColorButon	m_b2;


    LRESULT HandleInitDialog2(WPARAM, LPARAM);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCompProgress)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual void OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler ){};
	// Generated message map functions
	//{{AFX_MSG(CDlgCompProgress)
	afx_msg void OnSize(size_t nType, int cx, int cy);
	afx_msg void OnCancel();
    afx_msg void OnClear();
	afx_msg void OnCloseBTN();
    void        OnCheckCheck();
	afx_msg void OnDestroy() ;
    afx_msg void OnDblclkLB();
    afx_msg void OnSize();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    BOOL            _onScreen;

public:
    int                 n_timer;
    map<int,V3>         _mapidPoint;
    static     char    _progTxt[128];
    static     int      _progValue;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !__DLG_PROG_H__
