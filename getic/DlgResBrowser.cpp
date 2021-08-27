// DlgResBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "DlgResBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgResBrowser dialog


DlgResBrowser::DlgResBrowser(CWnd* pParent /*=NULL*/)
	: CDialog(DlgResBrowser::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgResBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_texlb._pParent=this;
    m_texlb._bitblt = 1;
}


void DlgResBrowser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgResBrowser)
	DDX_Control(pDX, PB_SELECT, m_ss);
	DDX_Control(pDX, IDCANCEL, m_cc);
	DDX_Control(pDX, IDC_LIST1, m_texlb);
	DDX_Control(pDX, TR_DIRS, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgResBrowser, CDialog)
	//{{AFX_MSG_MAP(DlgResBrowser)
	ON_WM_DESTROY()
	ON_BN_CLICKED(PB_SELECT, OnSelect)
	ON_MESSAGE(WM_SELCHANGED,OnSelchangedDirs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgResBrowser message handlers

BOOL DlgResBrowser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//w_draw.CreateWnd(this,ST_VIEW);
    
    m_tree.PreSelectFolder(theApp.TexDir());
	return TRUE;  	              
}

void DlgResBrowser::OnDestroy() 
{
	CDialog::OnDestroy();
}

void DlgResBrowser::OnSelect() 
{
	
}

LRESULT DlgResBrowser::OnSelchangedDirs(WPARAM pNMHDR, LPARAM pResult) 
{
    CString     spath = (const char*)pResult;
    PopSubDirText(spath);
	return 0;
}


void DlgResBrowser::PopSubDirText(const char* subDir)
{
    int             idx;
	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	
    tstring         fullpth= subDir;	
    tstring		    fullFname;
	
    CDirChange	    cd(fullpth.c_str()); //chdir
    
    m_texlb.SetRedraw(0);
	m_texlb.ResetContent();
    _texFiles.clear();
    cd.UseObj();
	if((hfFind=FindFirstFile(_T("*.*"), &fDATA)) != INVALID_HANDLE_VALUE)
	{
		if(!(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
            char* pext = _tcschr(fDATA.cFileName,'.');
            if(pext)pext++;

            if(pext &&  (!stricmp(pext,"bmp") || !stricmp(pext,"tga") || !stricmp(pext,"jpg") ))
            {
                fullFname = fullpth;
			    fullFname += fDATA.cFileName;
                _texFiles.push_back(fullFname);
    			idx = m_texlb.AddString(fDATA.cFileName);
            }
		} 
			
		while(FindNextFile(hfFind, &fDATA)!=0)
		{
			if(!(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
                char*pext = _tcschr(fDATA.cFileName,'.');
                if(pext)pext++;
                if(pext &&  (!stricmp(pext,"bmp") || !stricmp(pext,"tga") || !stricmp(pext,"jpg") ))
                {
        
			       fullFname = fullpth;
				    fullFname += fDATA.cFileName;
	               _texFiles.push_back(fullFname);
	                idx = m_texlb.AddString(fDATA.cFileName);
                }
            }
		}
	}
	FindClose(hfFind);  
    m_texlb.SetRedraw(1);
    m_texlb.Invalidate();
    REDIR();
}

