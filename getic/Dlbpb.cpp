//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "DirTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDirTree::CDirTree():_updating(FALSE)
{
}

CDirTree::~CDirTree()
{
}


BEGIN_MESSAGE_MAP(CDirTree, CTreeCtrl)
//{{AFX_MSG_MAP(CDirTree)
ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//--| CDirTree::PreSubclassWindow|--------------------------------------------------------
void CDirTree::PreSubclassWindow()
{
    HIMAGELIST      hImageList;
    SHFILEINFO      sfi;
    
    if ( hImageList = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\",
       0,
       &sfi,
       sizeof(SHFILEINFO),
       SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
    {
        ::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) TVSIL_NORMAL,
                     (LPARAM)hImageList);
    }
    
    CTreeCtrl::PreSubclassWindow();
    FillControls();
    
}


//--| CDirTree::FillControls|-------------------------------------------------------------
void CDirTree::FillControls()
{
    
    LPSHELLFOLDER   lpsf=NULL;
    LPITEMIDLIST    lpi=NULL;
    HRESULT         hr;
    TV_SORTCB       tvscb;
    
    // Get a pointer to the desktop folder.
    hr=SHGetDesktopFolder(&lpsf);
    
    if (SUCCEEDED(hr))
    {
        // Initialize the tree view control to be empty.
        DeleteAllItems();
        
        // Fill in the tree view control from the root.
        FillTreeCtrl(lpsf, NULL, TVI_ROOT);
        
        // Release the folder pointer.
        lpsf->Release();
    }
    
    // Sort the items in the tree view control.
    tvscb.hParent     = TVI_ROOT;
    tvscb.lpfnCompare = TreeViewCompareProc;
    tvscb.lParam      = 0;
    ::SendMessage(m_hWnd,TVM_SORTCHILDRENCB, (WPARAM)0, (LPARAM)&tvscb);
    
    
    
}

//--| CALLBACK CDirTree::TreeViewCompareProc|---------------------------------------------
int CALLBACK CDirTree::TreeViewCompareProc(LPARAM lparam1,
                                          LPARAM lparam2,
                                          LPARAM lparamSort)
{
    LPTVITEMDATA lptvid1=(LPTVITEMDATA)lparam1;
    LPTVITEMDATA lptvid2=(LPTVITEMDATA)lparam2;
    HRESULT   hr;
    hr = lptvid1->lpsfParent->CompareIDs(0,lptvid1->lpi,lptvid2->lpi);
    if (FAILED(hr))
       return 0;
    return (hr);
}

//--| CDirTree::FillTreeCtrl |------------------------------------------------------------
void CDirTree::FillTreeCtrl (LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent)
{
    TV_ITEM         tvi;            // tree view item
    TV_INSERTSTRUCT tvins;          // tree view insert structure
    HTREEITEM       hPrev = NULL;   // previous item added
    LPSHELLFOLDER   lpsf2=NULL;
    LPENUMIDLIST    lpe=NULL;
    LPITEMIDLIST    lpi=NULL, lpiTemp=NULL, lpifqThisItem;
    LPTVITEMDATA    lptvid=NULL;
    LPMALLOC        lpMalloc=NULL;
    ULONG           ulFetched;
    size_t            uCount=0;
    HRESULT         hr;
    TCHAR           szBuff[256];
    HWND            hwnd=::GetParent(m_hWnd);
    
    // Allocate a shell memory object.
    hr=::SHGetMalloc(&lpMalloc);
    if (FAILED(hr))
       return;
    
    
    if (SUCCEEDED(hr))
    {
        // Get the IEnumIDList object for the given folder.
        hr=lpsf->EnumObjects(hwnd, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &lpe);
        
        if (SUCCEEDED(hr))
        {
            // Enumerate throught the list of folder and nonfolder objects.
            while (S_OK==lpe->Next(1, &lpi, &ulFetched))
            {
                // Create a fully qualified path to the current item.
                // The SH* functions take a fully qualified path PIDL,
                // while the interface member functions take a
                // relative path PIDL.
                ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER;
                
                // Determine what type of object you have.
                lpsf->GetAttributesOf (
                                      1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
                
                if (ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FOLDER))
                {
                    // You need this next if statement to
                    // avoid adding objects that are not real
                    // folders to the tree. Some objects can
                    // have subfolders, but aren't real folders.
                    if (ulAttrs & SFGAO_FOLDER)
                    {
                        tvi.mask= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
                        
                        if (ulAttrs & SFGAO_HASSUBFOLDER)
                        {
                            // This item has subfolders, so put a plus sign in
                            // the tree view control. The first time the user clicks
                            // the item, you should populate the sub-folders.
                            tvi.cChildren=1;
                            tvi.mask |= TVIF_CHILDREN;
                        }
                        
                        // Get some memory for the ITEMDATA structure.
                        lptvid = (LPTVITEMDATA)lpMalloc->Alloc(sizeof(TVITEMDATA));
                        if (!lptvid)
                           goto Done;  // Error - could not allocate memory.
                        
                        // Now get the friendly name to
                        // put in the tree view control.
                        if (!GetName(lpsf, lpi, SHGDN_NORMAL, szBuff))
                           goto Done; // Error - could not get friendly name
                        
                        tvi.pszText    = szBuff;
                        tvi.cchTextMax = MAX_PATH;
                        
                        lpifqThisItem=ConcatPidls(lpifq, lpi);
                        
                        // Now make a copy of the ITEMIDLIST
                        lptvid->lpi=CopyITEMID(lpMalloc, lpi);
                        
                        GetNormalAndSelectedIcons(lpifqThisItem, &tvi);
                        
                        lptvid->lpsfParent=lpsf;    // store the parent folder's SF
                        lpsf->AddRef();
                        
                        lptvid->lpifq=ConcatPidls(lpifq, lpi);
                        
                        tvi.lParam = (LPARAM)lptvid;
                        
                        // Populate the tree view insert structure.
                        // The item is the one filled above.
                        // Insert it after the last item inserted at this level.
                        // Indicate this is a root entry.
                        tvins.item         = tvi;
                        tvins.hInsertAfter = TVI_FIRST;
                        tvins.hParent      = hParent;
                        
                        // Add the item to the tree.
                        hPrev = InsertItem(&tvins);
                    }
                    // Free the task allocator for this item.
                    lpMalloc->Free(lpifqThisItem);
                    lpifqThisItem=0;
                }
                
                lpMalloc->Free(lpi);  // free PIDL the shell gave you
                lpi=0;
            }
        }
        
    }
    else
        return;
    
    Done:
        if (lpe)
        lpe->Release();
    
    // The following two if statements will be TRUE only if you got here
    // on an error condition from the goto statement.  Otherwise, free
    // this memory at the end of the while loop above.
    if (lpi && lpMalloc)
       lpMalloc->Free(lpi);
    if (lpifqThisItem && lpMalloc)
       lpMalloc->Free(lpifqThisItem);
    
    if (lpMalloc)
       lpMalloc->Release();
}


//--| CDirTree::GetName |-----------------------------------------------------------------
BOOL CDirTree::GetName (LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, size_t dwFlags, LPSTR lpFriendlyName)
{
    BOOL   bSuccess=TRUE;
    STRRET str;
    
    if (NOERROR==lpsf->GetDisplayNameOf(lpi,dwFlags, &str))
    {
        switch (str.uType)
        {
            case STRRET_WSTR:
                WideCharToMultiByte(
                CP_ACP,                 // code page
            0,                 // dwFlags
            str.pOleStr,            // lpWideCharStr
            -1,                     // cchWideCharStr
            lpFriendlyName,         // lpMultiByteStr
            128, // cchMultiByte
            NULL,                   // lpDefaultChar
            NULL);                  // lpUsedDefaultChar
            break;
            
            case STRRET_OFFSET:
                strcpy(lpFriendlyName, (LPSTR)lpi+str.uOffset);
            break;
            
            case STRRET_CSTR:
                strcpy(lpFriendlyName, (LPSTR)str.cStr);
            break;
            
            default:
                bSuccess = FALSE;
            break;
        }
    }
    else
        bSuccess = FALSE;
    
    return bSuccess;
    
}
//}}AFX

//--| CDirTree::GetFullyQualPidl|---------------------------------------------------------
BOOL CDirTree::GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi,LPSTR pBuff)
{
    
    //OLECHAR szOleChar[MAX_PATH];
    //   LPSHELLFOLDER lpsfDeskTop;
    //LPITEMIDLIST  lpifq;
    //ULONG ulEaten, ulAttribs;
    //   HRESULT hr;
    
    if (!GetName(lpsf, lpi, SHGDN_FORPARSING, pBuff))
       return NULL;
    
    //hr = SHGetDesktopFolder(&lpsfDeskTop);
    
    //if (FAILED(hr))
    // return NULL;
    /*
        MultiByteToWideChar(CP_ACP,
        MB_PRECOMPOSED,
        pBuff,
        -1,
        (USHORT *)szOleChar,
    sizeof(szOleChar));
    
    hr=lpsfDeskTop->ParseDisplayName(NULL,
                                    NULL,
                                    szOleChar,
                                    &ulEaten,
                                    &lpifq,
                                    &ulAttribs);
    
    lpsfDeskTop->Release();
    
    if (FAILED(hr))
       return NULL;
    */
        return 1;//lpifq;
    
    
}

//--| CDirTree::OnItemexpanding|----------------------------------------------------------
void CDirTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    
    LPSHELLFOLDER  lpsf2=NULL;
    
    if ((pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE))
       return;
    
    LPTVITEMDATA lptvid=(LPTVITEMDATA)pNMTreeView->itemNew.lParam;
    if (lptvid)
    {
        HRESULT hr=lptvid->lpsfParent->BindToObject(lptvid->lpi,
                                                   0, IID_IShellFolder,(LPVOID *)&lpsf2);
        
        if (SUCCEEDED(hr))
        {
            FillTreeCtrl(lpsf2,
                        lptvid->lpifq,
                        pNMTreeView->itemNew.hItem);
            
            lpsf2->Release();
            
        }
        TV_SORTCB      tvscb = {0};
        
        tvscb.hParent     = pNMTreeView->itemNew.hItem;
        tvscb.lpfnCompare = TreeViewCompareProc;
        tvscb.lParam      = 0;
        
        ::SendMessage(m_hWnd,TVM_SORTCHILDRENCB, (WPARAM)0, (LPARAM)&tvscb);
    }
    *pResult = 0;
}

//--| CDirTree::CopyITEMID|---------------------------------------------------------------
LPITEMIDLIST CDirTree::CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
{
    LPITEMIDLIST lpiTemp;
    
    lpiTemp=(LPITEMIDLIST)lpMalloc->Alloc(lpi->mkid.cb+sizeof(lpi->mkid.cb));
    CopyMemory((PVOID)lpiTemp, (CONST VOID *)lpi, lpi->mkid.cb+sizeof(lpi->mkid.cb));
    
    return lpiTemp;
    
    
}

//--| CDirTree::ConcatPidls|--------------------------------------------------------------
LPITEMIDLIST CDirTree::ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
    LPITEMIDLIST pidlNew;
    size_t cb1;
    size_t cb2;
    
    if (pidl1)  //May be NULL
    cb1 = GetSize(pidl1) - sizeof(pidl1->mkid.cb);
    else
        cb1 = 0;
    
    cb2 = GetSize(pidl2);
    
    pidlNew = Create(cb1 + cb2);
    if (pidlNew)
    {
        if (pidl1)
           memcpy(pidlNew, pidl1, cb1);
        memcpy(((LPSTR)pidlNew) + cb1, pidl2, cb2);
    }
    return pidlNew;
    
}


//--| CDirTree::GetNormalAndSelectedIcons |-----------------------------------------------
void CDirTree::GetNormalAndSelectedIcons ( LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
    // Don't check the return value here.
    // If IGetIcon() fails, you're in big trouble.
    lptvitem->iImage = GetIcon (lpifq,
                               SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
    
    lptvitem->iSelectedImage = GetIcon (lpifq,
                                       SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
    
    return;
    
}

//--| CDirTree::GetSize|------------------------------------------------------------------
size_t CDirTree::GetSize(LPCITEMIDLIST pidl)
{
    size_t cbTotal = 0;
    if (pidl)
    {
        cbTotal += sizeof(pidl->mkid.cb);       // Null terminator
        while (pidl->mkid.cb)
        {
            cbTotal += pidl->mkid.cb;
            pidl = Next(pidl);
        }
    }
    
    return cbTotal;
    
}

//--| CDirTree::Next|---------------------------------------------------------------------
LPITEMIDLIST CDirTree::Next(LPCITEMIDLIST pidl)
{
    LPSTR lpMem=(LPSTR)pidl;
    
    lpMem+=pidl->mkid.cb;
    
    return (LPITEMIDLIST)lpMem;
    
    
}


//--| CDirTree::Create|-------------------------------------------------------------------
LPITEMIDLIST CDirTree::Create(size_t cbSize)
{
    LPMALLOC lpMalloc;
    HRESULT  hr;
    LPITEMIDLIST pidl=NULL;
    
    hr=SHGetMalloc(&lpMalloc);
    
    if (FAILED(hr))
       return 0;
    
    pidl=(LPITEMIDLIST)lpMalloc->Alloc(cbSize);
    
    if (pidl)
       memset(pidl, 0, cbSize);      // zero-init for external task   alloc
    
    if (lpMalloc) lpMalloc->Release();
    
    return pidl;
    
    
}

//--| CDirTree::OnSelchanged|-------------------------------------------------------------
void CDirTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    
    *pResult = 0;
    if(_updating )
      return;
    
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    
    LPTVITEMDATA lptvid=(LPTVITEMDATA)pNMTreeView->itemNew.lParam;
    
    TCHAR szBuff[MAX_PATH];
    
    if(GetFullyQualPidl(lptvid->lpsfParent, lptvid->lpi,szBuff))
    {
        if(!(m_curpath == szBuff))
        {
            m_curpath = szBuff;
            GetParent()->PostMessage(WM_SELCHANGED,0,(LPARAM)(LPCSTR)m_curpath.c_str());
        }
    }
    
    *pResult = 0;
}

//--| CDirTree::PreSelectFolder|----------------------------------------------------------
BOOL CDirTree::PreSelectFolder(LPCSTR pszFolder)
{
    _updating = TRUE;
    // walk around the three
    // get the name compare to the parsed name
    TCHAR            sDir[MAX_PATH];
    TCHAR             addDir[MAX_PATH];
    TCHAR       token[3];
    LPCSTR    pToken = 0;
    HTREEITEM   htI    = TVI_ROOT;
    
    strcpy(sDir, pszFolder);
    strcpy(addDir, "");
    strcpy(token,"\\");
    
    Expand(htI,TVE_EXPAND);
    htI = GetNextItem(htI,TVGN_ROOT);
    Expand(htI,TVE_EXPAND);
    htI = GetNextItem(htI,TVGN_CHILD);
    LPSTR p = sDir;
    
    while(pToken = strtok(p,token))
    {
        _tcscat(addDir, pToken);
        _tcscat(addDir, "\\");
        if(htI && (htI = GetDirEntry(addDir,htI)))
        {
            ;
        }
        p = 0;
    }
    m_curpath = pszFolder;
    
    _updating = FALSE;
    return 1;
}


//--| CDirTree::GetDirEntry|--------------------------------------------------------------
HTREEITEM CDirTree::GetDirEntry(LPCSTR pToken,HTREEITEM hti)
{
    HTREEITEM htInitial = hti;
    LPTVITEMDATA lptvid PURE;
    TCHAR szBuff[MAX_PATH];
    
    
    do{
        
        lptvid=(LPTVITEMDATA)GetItemData(htInitial);
        GetFullyQualPidl(lptvid->lpsfParent, lptvid->lpi,szBuff);
    }while(strncmp(szBuff, pToken, strlen(szBuff)-1) &&
    (htInitial = GetNextItem(htInitial,TVGN_NEXT))&&
    htInitial != TVI_ROOT);
    if(htInitial && htInitial!=TVI_ROOT)
    {
        Expand(htInitial,TVE_EXPAND);
        SelectItem(htInitial);
        return GetNextItem(htInitial,TVGN_CHILD);
    }
    return 0;
}
