#ifndef __TEXLBOX_H__
#define __TEXLBOX_H__

#include "baselib.h"

class CarryTex{
public:
	virtual const char*   GetTextureFullName(int i){ASSERT(0);return "";}
    virtual void     GetTextureSubPaths(vvector<tstring>& r){;}
    virtual void     SelectSubPath(int i){;}



	int             _bpp;
    size_t            _sz;
    tstring         _name;
};

/////////////////////////////////////////////////////////////////////////////
// CTexLbox window
class CDlgTextures;
class CTexLbox : public CListBox
{
// Construction
public:
    CTexLbox(){_bitblt=0;_pParent=0;};
    CarryTex* _pParent;
    BOOL      _bitblt;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexLbox)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CTexLbox(){};

	// Generated message map functions
protected:
	//{{AFX_MSG(CTexLbox)
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnRButtonDown(size_t nFlags, CPoint point);
	afx_msg void OnLButtonDown(size_t nFlags, CPoint point);
    afx_msg void OnSelchange() ;
	//}}AFX_MSG
    void    DrawBmp(HDC hdc, CBitmap* pBmp, int x, int y, int slice);

	DECLARE_MESSAGE_MAP()
	CPen	    _errorPen;
    CBitmap     _texkind[3];
	int			_curSel;

public:
    vvector<string> _invalides;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __TEXREF_H__
