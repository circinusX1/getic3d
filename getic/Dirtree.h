#if !defined(AFX_DIRTREE_H__25C401C3_89C9_11D2_822A_00600815B1D4__INCLUDED_)
#define AFX_DIRTREE_H__25C401C3_89C9_11D2_822A_00600815B1D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DirTree.h : header file
//
#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////
// CDirTree window

#define	WM_SELCHANGED	WM_USER+1234

typedef struct tagID
{
   LPSHELLFOLDER lpsfParent;
   LPITEMIDLIST  lpi;
   LPITEMIDLIST  lpifq;
} TVITEMDATA, *LPTVITEMDATA;



class CDirTree : public CTreeCtrl
{
// Construction
public:
	CDirTree();

// Attributes
public:
    HTREEITEM   FindItem(const char* lpszText, HTREEITEM hItem);
    HTREEITEM   GetNextItemEx(HTREEITEM hItem);
	CString     GetItemPath( HTREEITEM hItem = 0);
	BOOL        PopulateTree( CString &sPath, HTREEITEM hParent );
    HTREEITEM   InsertItem2( HTREEITEM hParent, 
                            CString &sText, 
                            int iImage, 
                            int iImageSelected =-1 );

public:
	CString		m_sPath;							//Highlighted path

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirTree)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL



// Implementation
public:
	BOOL    PreSelectFolder(LPCSTR pszFolder);
	virtual ~CDirTree();
protected:
	//{{AFX_MSG(CDirTree)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
    void OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult) ;
	DECLARE_MESSAGE_MAP()
	
    //Attributes
private:
	CImageList	m_ImageListTree;					//Item in the Tree image list				
    HTREEITEM   m_hSelected;
    BOOL        _frompop;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRTREE_H__25C401C3_89C9_11D2_822A_00600815B1D4__INCLUDED_)
