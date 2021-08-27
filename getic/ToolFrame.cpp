// ToolFrame.cpp : implementation file
//

#include    "stdafx.h"
#include    "z-edmap.h"
#include    "ToolFrame.h"
#include    "SampleView.h"
#include	"SampleDoc.h"
#include	"DlgCompilerPath.h"
#include	"z_ed2Doc.h"
#include	"DlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCompilerOutDlg* PBarDlg = 0;
// CToolFrame

IMPLEMENT_DYNCREATE(CToolFrame, CMiniFrameWnd)

CToolFrame::CToolFrame()
{
}

CToolFrame::~CToolFrame()
{
    PBarDlg = 0;
}


BEGIN_MESSAGE_MAP(CToolFrame, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CToolFrame)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SC_NEW, OnScNew)
	ON_COMMAND(ID_SC_OPEN, OnScOpen)
	ON_COMMAND(ID_SC_SAVE, OnScSave)
	ON_UPDATE_COMMAND_UI(ID_SC_SAVE, OnUpdateScSave)
	ON_COMMAND(PB_SETTINGS, OnSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolFrame message handlers

BOOL	CToolFrame::CreateTextView()//CView
{


    size_t defStyle = TBSTYLE_FLAT;
    size_t styl  = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP;

	//SetTimer(SPASH_TIMER, 4000,0);

	if (!m_Editbar.CreateEx(this,defStyle,styl,CRect(0,0,0,0), IDR_SINTACTICEF) ||
		!m_Editbar.LoadToolBar(IDR_SINTACTICEF))
	{
		return -1;      // fail to create
	}
    size_t tbAttr = CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY;
	m_Editbar.SetBarStyle(m_Editbar.GetBarStyle() |tbAttr);    

	_pDoc = new  CSampleDoc();
	_pDoc->m_bAutoDelete=FALSE;
	_pView = new CSampleView();
    _pDoc->_pView = _pView;
	
    CCreateContext  context; 
	RECT rt;
    context.m_pCurrentDoc = (CDocument*)_pDoc;

	_pView->Create(0,0,WS_CHILD|WS_VISIBLE,rt,this,AFX_IDW_PANE_FIRST, 
                            &context);
    OnScNew() ;
	_pView->ShowWindow(SW_SHOW);
	_pView->OnInitialUpdate();

    _dlgLeft._pToolFrame = this;
    _dlgLeft.Create(this, DLG_SCRIPT_BAR, CBRS_ALIGN_LEFT, AFX_IDW_DIALOGBAR);

    _dlgBar.Create(this, IDD_DIALOG_CPP, CBRS_ALIGN_BOTTOM, AFX_IDW_DIALOGBAR+1);
    PBarDlg = &_dlgBar;
   
    RecalcLayout();
	return 1;
}

BOOL CToolFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CMiniFrameWnd::PreCreateWindow(cs);
}

void CToolFrame::OnDestroy() 
{
	_pView->DestroyWindow();
	delete _pDoc;
    theApp._bToolOn=FALSE;
    GetWindowRect(&theApp._rectMiniTool);
	CMiniFrameWnd::OnDestroy();
	//BARDLG()->_dlgScript.Refresh();
}

void CToolFrame::PostNcDestroy() 
{
	CMiniFrameWnd::PostNcDestroy();
	theApp._pToolFrame = 0;
}

void CToolFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMiniFrameWnd::OnSetFocus(pOldWnd);
    theApp._bToolOn=TRUE;

}
//-----------------------------------------------------------------------------
void CToolFrame::OnScNew() 
{
    CSampleDoc* _pDoc = _pView->GetDocument();
    if(_pDoc)
    {
        _pDoc->SaveModified();
        _pDoc->OnNewDocument();

    }
}

void CToolFrame::OnScOpen()
{
	CSampleDoc* _pDoc = _pView->GetDocument();

    LPTSTR  lpszPathName = NULL;
    char    szFilters[] = _T("Script file(*.cg+)|*.cg+|All Files (*.*)|*.*||");

    CString newName = "*.cg+";

    CFileDialog dlg(1, _T("cg+"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    dlg.m_ofn.lpstrTitle      = _T("Select Script File");
    dlg.m_ofn.lpstrInitialDir = theApp.ScriptsDir();
    dlg.m_ofn.lpstrFile       = newName.GetBuffer(PATH_MAX);
    int res = theApp.DoFileDialog(dlg); 
    newName.ReleaseBuffer();
    if(res!=IDOK)
        return ;   

    PathHandler ph(dlg.m_ofn.lpstrFile);
    theApp.ScriptsDir(ph.Path());

    if(_pDoc->OnOpenDocument(newName))
    {
        SetTitle(newName);
    }
    REDIR(); 
    
	char title[255];
	strcpy(title,_pDoc->GetTitle());
    SendMessage(WM_SETTEXT,0,(long)title);
}


void CToolFrame::OnScSave() 
{
	int nChars = 0; 

    CSampleDoc* _pDoc = _pView->GetDocument();
    if(_pDoc)
    {
		nChars = _pDoc->m_xTextBuffer.GetLineCount();
		if(nChars>0)
		{
			nChars += _pDoc->m_xTextBuffer.GetLineLength(0);
		}
		if(nChars>1)
		{
			_pDoc->SaveModified();
		}
    }
    if(nChars>1)
	{
		CString tlName = _pDoc->GetTitle();
		_dlgLeft.AttachItemScriptFile(tlName);
	}
	else
	{
		_dlgLeft.AttachItemScriptFile(""); //delete script for given item
	}
}


void CToolFrame::OnUpdateScSave(CCmdUI* pCmdUI) 
{
    CSampleDoc* _pDoc = _pView->GetDocument();
    pCmdUI->Enable(_pDoc->IsModified());
}

void CToolFrame::OnSettings() 
{
    CDlgCompilerPath().DoModal();	
}

void    CToolFrame::CanChangeItem()
{
	if (_pDoc->IsModified())
	{
		if(IDYES == AfxMessageBox(MKSTR("Save '%s' script",_pDoc->GetTitle()), MB_ICONQUESTION|MB_YESNO))
		{
			OnScSave();
		}
	}
}

void    CToolFrame::LoadSciptFile(const char* doc, 
								  const char* itm, 
								  const char* szScriptFname)
{
	REDIR();
	if(szScriptFname[0]==0)
	{
		OnScNew();
		char newDoc[PATH_MAX];
		PathHandler ph(doc);
		
		sprintf(newDoc,"scripts\\%s_%s",ph.File(),itm);
		StripSpaces(newDoc);
		_tcscat(newDoc,".cg+");

		SetTitle(newDoc);
        SendMessage(WM_SETTEXT,0,(long)newDoc);
		return;
	}

    if(_pDoc->OnOpenDocument(szScriptFname))
    {
        SetTitle(szScriptFname);
    }
	else
	{
		OnScNew() ;
		SetTitle(MKSTR("%s_%s.cg+",doc,itm));
	}

}


void	CToolFrame::SetTitle(const char* psz)
{
	SendMessage(WM_SETTEXT,0,(long)psz);
	_pDoc->SetTitle(psz);

}

void	CToolFrame::AddScriptString(const char* psz)
{
}


void	CToolFrame::SelectScriptString(const char* psz)
{
}

void	CToolFrame::FindScriptFunction(const char* psz)
{
}

