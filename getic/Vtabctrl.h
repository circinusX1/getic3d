#if !defined(AFX_VTABCTRL_H__70B9CC57_F46E_46EA_BD3A_F3FBE4C4F819__INCLUDED_)
#define AFX_VTABCTRL_H__70B9CC57_F46E_46EA_BD3A_F3FBE4C4F819__INCLUDED_

#include <string>
#include <vector>
using namespace std;

class CVTabCtrl : public CWnd
{
// Construction
public:
	CVTabCtrl();

// Attributes
public:
    BOOL    Create(UINT ID, CWnd* pParent);
    void AddChildren(CDialog* pDlg, UINT ID, LPCTSTR pTitle=0);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVTabCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    static BOOL     _registered;
    static int      _instances;
    static HCURSOR  _hHandCursor;
    void    RecalcScroll(int yView);
    void    Reposition(int nRestore=0);

    struct  SDlg
    {
        CDialog* _pD;
        CRect    _rdlg;
        CRect    _rbtn;
        BOOL     _expnd;
        string   _name;
        CButton  _but;
    };

    int             _tmpCreaY;
    int             _viewportY;
    vector<SDlg*>   _dialogs;
    CRect           _wndRect;
    int             _yScrollDn;
    CRect           _scrlInval;
    CPoint          _ptDn;
    BOOL            _scrDn;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VTABCTRL_H__70B9CC57_F46E_46EA_BD3A_F3FBE4C4F819__INCLUDED_)
