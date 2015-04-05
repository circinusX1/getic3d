//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z-edmap.h"
#include "DirTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char* DupString( LPTSTR szData )
{
	TRACE( _T("DupString( %s )\n"), szData );
	//Assume a NULL = empty string
	char * szRet = NULL;
	int nLength	  = 0;
	if( szData )
		nLength = _tcslen( szData )+1;
	if( nLength > 0 )
	{
		szRet = new char[nLength];
		ASSERT( szRet );
		_tcscpy( szRet, szData );
	}
	return szRet;
}


CString WindowName( CString sName )
{
	CString sRet;
	//Scan all charactors to determine if their are any lower case items
	for( int n = 0; n < sName.GetLength(); n++ )
	{
		char ch = sName[n];
		if ((ch >= 'a') && (ch <= 'z') )
			return sName;
	}
	sName.MakeLower();
	if( sName.GetLength() > 0 )
	{
		CString sFirstChar = sName[0];
		sFirstChar.MakeUpper();
		sName = sFirstChar + sName.Mid( 1 );
	}
	return sName;
}

bool ParsPath( CString sPath, int nLevel, CString *const psDirName )
{
	ASSERT( psDirName );
	//find first match
	int nStart = 0;									//Location in sPath to search from
	while( nLevel )
	{
		nStart = sPath.Find( _T('\\'), nStart );
		if( nStart < 0 )
			return false;
		nLevel--;
		nStart++;
	}
	//We now have the start point to find the end.
	int nEnd = sPath.Find( _T('\\'), nStart );
	if( nEnd < 0 )
		*psDirName = sPath.Mid( nStart );
	else
		*psDirName = sPath.Mid( nStart, nEnd-nStart);
	return true;
}


CDirTree::CDirTree()
{
    _frompop=0;
}

CDirTree::~CDirTree()
{
}


BEGIN_MESSAGE_MAP(CDirTree, CTreeCtrl)
    //{{AFX_MSG_MAP(CDirTree)
    ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
    ON_WM_CREATE()
    ON_WM_DELETEITEM()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CDirTree::PreSubclassWindow() 
{
    /*
	HIMAGELIST himlNormal;
	SHFILEINFO sfi;

	himlNormal = (HIMAGELIST) SHGetFileInfo ((LPCSTR) "C:\\", 0, &sfi, sizeof (SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	if (himlNormal)
	{
		::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)himlNormal);
	}
/*
    if( !m_ImageListTree.Create( IDB_FILE_TYPES, 17, 1, RGB( 255, 255, 255 ) ) )
		TRACE( _T("Image list creation fault") );
	SetImageList( &m_ImageListTree, TVSIL_NORMAL );
*/
	for( char cLetter = _T('A'); cLetter <= _T('Z'); cLetter++ )
	{
		CString sDrive = cLetter;
		sDrive += _T(":");						
		UINT nType = GetDriveType( sDrive + _T("\\") );
		if( DRIVE_REMOVABLE <= nType && nType <= DRIVE_RAMDISK )
			InsertItem2( TVI_ROOT, sDrive, nType );
	}
    m_hSelected	=TVI_ROOT;
	this->SetBkColor(RGB(255,255,255));

	CTreeCtrl::PreSubclassWindow();
}

HTREEITEM CDirTree::FindItem(LPCTSTR lpszText, HTREEITEM hItem)
{
	HTREEITEM htiSelected = hItem ? hItem : GetSelectedItem();
	HTREEITEM htiCurrent = GetNextItemEx(htiSelected);
	
	if(htiCurrent == NULL)
	{
		htiCurrent = GetRootItem();
	}

	while(htiCurrent && htiCurrent != htiSelected)
	{
		CString strItemText = GetItemText(htiCurrent);

		if (strItemText.Compare(lpszText) == 0)
		{
			// found string
			return htiCurrent;
		}

		// get next
		htiCurrent = GetNextItemEx(htiCurrent);
		if(htiCurrent == NULL && htiSelected != NULL)
		{
			htiCurrent = GetRootItem();
		}
	}
	return NULL;
}

HTREEITEM CDirTree::GetNextItemEx(HTREEITEM hItem)
{
	HTREEITEM hti;

	if(ItemHasChildren(hItem))
	{
		// return first child
		return GetChildItem(hItem);           
	}
	else
	{
		// return next sibling item
        // Go up the tree to find a parent's sibling if needed.
        while((hti = GetNextSiblingItem(hItem)) == NULL)
		{
			if((hItem = GetParentItem(hItem)) == NULL)
				return NULL;
		}
	}
    return hti;
}



int CDirTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL    CDirTree::PreSelectFolder(LPCSTR pszFolder)
{
    _frompop=1;
	int nLevel = 0;
	CString   sDirName;
    
    if(!strstr(pszFolder,":\\"))
    {
        char cd[_MAX_PATH];
        _getcwd(cd, _MAX_PATH);
        m_sPath = cd;
        m_sPath += "\\";
        m_sPath +=pszFolder;
    }
    else
        m_sPath = pszFolder;



	HTREEITEM hCurrent = TVI_ROOT;					//Current item in the list being expanded.
	while( ParsPath( m_sPath, nLevel, &sDirName ) )
	{
		//Network or Drive expand
		//Search for the matching tree item and expand
		HTREEITEM hItem = this->GetChildItem( hCurrent );	
		while( hItem )
		{
			if( sDirName.CompareNoCase( this->GetItemText( hItem ) ) == 0 )
			{
				hCurrent = hItem;
				this->Expand( hCurrent, TVE_EXPAND );
				break;
			}
			hItem = this->GetNextSiblingItem( hItem );
		}
		nLevel++;
	}
    m_hSelected	=hCurrent;
    m_sPath = GetItemPath(m_hSelected);
    _frompop=0;
    return 0;
}

HTREEITEM CDirTree::InsertItem2( HTREEITEM hParent, 
                                CString &sText2, 
                                int iImage, int iImageSelected)
{
	CString sText = WindowName( sText2);
	TVINSERTSTRUCT InsertStruct;
	InsertStruct.hParent		= hParent;	
	InsertStruct.hInsertAfter	= TVI_LAST;
	InsertStruct.itemex.mask	= TVIF_IMAGE | TVIF_TEXT | TVIF_CHILDREN | 
								  TVIF_SELECTEDIMAGE | TVIF_PARAM;
	InsertStruct.itemex.pszText	= sText.GetBuffer( sText.GetLength() );  sText.ReleaseBuffer();
	InsertStruct.itemex.iImage	= iImage;
	InsertStruct.itemex.cChildren= 1;
	InsertStruct.itemex.lParam	= (LPARAM)0;
	if( iImageSelected == -1 )
		InsertStruct.itemex.iSelectedImage = iImage;
	else
		InsertStruct.itemex.iSelectedImage = iImageSelected;
	return this->InsertItem( &InsertStruct );
}

BOOL CDirTree::PopulateTree( CString &sPath, HTREEITEM hParent ) 
{
	bool bGotChildren = false;						//True if a child is added.	
	CFileFind finder;
	BOOL bWorking = finder.FindFile( sPath+_T("*.*") );
	while (bWorking)   
	{
		bWorking = finder.FindNextFile();
		if( finder.IsDots() )
			continue;
		if( finder.IsDirectory() )
		{
            CString &csN = finder.GetFileName();
			InsertItem2( hParent, csN, DRIVE_NO_ROOT_DIR, DRIVE_UNKNOWN );
			bGotChildren = true;
		}
	}
	//
	//Remove the [+] if no children
	//
	if( !bGotChildren )
	{
		TVITEM item		= { 0 };
		item.mask		= TVIF_HANDLE | TVIF_CHILDREN;
		item.hItem		= hParent;
		item.cChildren	= 0;
		this->SetItem( &item );
	}
    finder.Close();
	return bGotChildren;
}

CString CDirTree::GetItemPath( HTREEITEM hItem ) 
{
    if(0==hItem || hItem==(HTREEITEM)-1 || hItem==(HTREEITEM)0xFFFF0000)
    {
        hItem= m_hSelected;
    }
	CString sRet;
	do
	{
        /*
		//End with a share name.
		NETRESOURCE *const pNetResource = (NETRESOURCE *)(this->GetItemData( hItem ) );
		if( pNetResource )
		{
			sRet = CString(pNetResource->lpRemoteName) + _T('\\')+ sRet; 		
			break;
		}
        */
		//Add the directory name to the path.
		sRet = this->GetItemText( hItem ) + _T('\\')+ sRet; 
		hItem = this->GetParentItem( hItem );
	} while( hItem );
	return sRet;
}


void CDirTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE( _T("CDirTree::OnItemexpandingTree(%p)\n"), pNMHDR );
	CWaitCursor CursorWaiting;						//Show the wait cursor while expanding
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	ASSERT( pNMTreeView );
	ASSERT( pResult );
	//Only action 2 notifications
	if( pNMTreeView->action == 2 )
	{
		//Update location display
		CString sPath = GetItemPath( pNMTreeView->itemNew.hItem );
		//Refresh children
		if( !this->GetChildItem( pNMTreeView->itemNew.hItem ) )
		{
			PopulateTree( sPath, pNMTreeView->itemNew.hItem );
			if( this->GetSelectedItem( ) != pNMTreeView->itemNew.hItem )
				this->SelectItem( pNMTreeView->itemNew.hItem );
		}
	}
	*pResult = 0;
}


void CDirTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE( _T("CDirTree::OnSelchangedTree(%p)\n"), pNMHDR );
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	ASSERT( pNMTreeView );
	ASSERT( pResult );
	CString sPath;									//Currently selected path or empty if not valid
	//Disable search on Workstation roots
	if( this->GetItemData( pNMTreeView->itemNew.hItem ))
	{
	}
	else
	{
		sPath = GetItemPath( pNMTreeView->itemNew.hItem );
	}
	*pResult = 0;
	m_hSelected	= pNMTreeView->itemNew.hItem;

    if(!_frompop)
    {
        m_sPath     = sPath; 
        GetParent()->SendMessage(WM_SELCHANGED, 0, (LPARAM)(LPCTSTR) m_sPath);
    }
}

					

void CDirTree::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	CTreeCtrl::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}

void CDirTree::OnDestroy() 
{
    m_ImageListTree.DeleteImageList();
	CTreeCtrl::OnDestroy();
}
