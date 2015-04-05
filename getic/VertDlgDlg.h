// VertDlgDlg.h : header file
//

#if !defined(AFX_VERTDLGDLG_H__FBB842B0_4021_47F4_8332_82ABFF7AFCF2__INCLUDED_)
#define AFX_VERTDLGDLG_H__FBB842B0_4021_47F4_8332_82ABFF7AFCF2__INCLUDED_


#include "ColorButon.h"
#include "uienhancer.h"
#include "ColorButon.h"
#include <vector>
#include <string>




class CVertDlgDlg : public CBASEDLG
{

   
// Construction
public:
    CVertDlgDlg(UINT idd, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVertDlgDlg)
	enum { IDD = -1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVertDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
    void    Colapse(UINT group);
    void    EnableGroup(UINT group, BOOL en);
    void    Enable(BOOL ben);
    
    virtual void OnExpanded();
// Implementation
protected:
    struct  Group
    {
        CColorButon     _buton;
        //CButton          _buton;
        CRect           _grect;
        vector<HWND>    _wnds;
        vector<CRect>   _rwnds;
        CWnd*           _pCtl;
        UINT            _cmdID;
        char            _prevSt;
    };
    vector<Group*>      _groups;

    void    CreateGroups();
    void    UpdateControls();
    virtual void    Reset();
    int             _viewportY;
    CRect           _wndRect;
    int             _yScrollDn;
    CRect           _scrlInval;
    CPoint          _ptDn;
    BOOL            _scrDn;
    
    HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVertDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    void    OnCommand();

public:
	static	HICON	_handIco[2];
	static  int		_instances;
    BOOL            _scrooled;   
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERTDLGDLG_H__FBB842B0_4021_47F4_8332_82ABFF7AFCF2__INCLUDED_)
