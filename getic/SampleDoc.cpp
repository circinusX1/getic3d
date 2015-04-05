// SampleDoc.cpp : implementation of the CSampleDoc class
//

#include "stdafx.h"
#include "z-edmap.h"
#include "SampleDoc.h"
#include "SampleView.h"
#include "CppCompiler.h"
#include "ToolFrame.h"
#include "CompilerOutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc
CSampleDoc*	CSampleDoc::PDoc;



IMPLEMENT_DYNCREATE(CSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CSampleDoc, CDocument)
	//{{AFX_MSG_MAP(CSampleDoc)
	ON_COMMAND(PB_BUILD, OnBuild)
	ON_UPDATE_COMMAND_UI(PB_BUILD, OnUpdateBuild)
	ON_COMMAND(PB_SAVE, OnSaveInEditor)
	ON_UPDATE_COMMAND_UI(PB_SAVE, OnUpdateSaveInEditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc construction/destruction

#pragma warning(disable:4355)
CSampleDoc::CSampleDoc() : m_xTextBuffer(this),_compileErr(-1)
{
	// TODO: add one-time construction code here

	//	Initialize LOGFONT structure
	memset(&m_lf, 0, sizeof(m_lf));
	m_lf.lfWeight = FW_NORMAL;
	m_lf.lfCharSet = ANSI_CHARSET;
	m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy(m_lf.lfFaceName, "FixedSys");

	CSampleDoc::PDoc = this;
}


BOOL CSampleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_xTextBuffer.InitNew();

    _pView->Invalidate();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSampleDoc serialization

void CSampleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc diagnostics

#ifdef _DEBUG
void CSampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc commands

void CSampleDoc::DeleteContents() 
{
    _pView->ReserCursors();
    m_xTextBuffer.FreeAll();
    CDocument::DeleteContents();
}

BOOL CSampleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	BOOL b = m_xTextBuffer.LoadFromFile(lpszPathName);
    _pView->Invalidate();
    return TRUE;
}

BOOL CSampleDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	m_xTextBuffer.SaveToFile(lpszPathName,CRLF_STYLE_AUTOMATIC, TRUE,FALSE);
	SetModifiedFlag(FALSE); 
	return TRUE;	//	Note - we didn't call inherited member!
}

void CSampleDoc::OnBuild() 
{
    CWaitCursor cwc; // if not var is excluded in release(0);
}

void CSampleDoc::OnUpdateBuild(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CCrystalTextView::PView->GetLineCount()>1);
	
}

void CSampleDoc::OnTemplate() 
{
	SetModifiedFlag(FALSE); 
}

void CSampleDoc::OnSaveInEditor() 
{
	CString cs;
	CSampleDoc::PDoc->m_xTextBuffer.GetText(0,0,-1,-1,cs) ;
}

void CSampleDoc::OnUpdateSaveInEditor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_compileErr==0);
}

void CSampleDoc::OnCloseDocument() 
{
	CDocument::OnCloseDocument();
}

BOOL CSampleDoc::SaveModified() 
{
	if (!IsModified())
		return TRUE;        
    int rv= IDYES;
	CString cs = GetTitle();
	if (cs.IsEmpty())
	{
        cs = "Untitled.cg+";
	}
	if(IDYES==rv)
    {    
        return DoSaveFile(cs);
    }
    if(IDNO==rv)
        return TRUE;
    return FALSE;
}


BOOL CSampleDoc::DoSaveFile(LPCTSTR fileName)
{
    return 0;
}

void CSampleDoc::UpdateAllViews()
{
    if(_pView)
        _pView->Invalidate();
}

void CSampleDoc::InsertLine(LPCTSTR pszLine, BOOL ro)
{
	m_xTextBuffer.InsertLine(pszLine);
	_pView->UpdateTokens();
};


void CSampleDoc::ReportSaveLoadException(LPCTSTR lpszPathName,
	CException* e, BOOL bSaving, UINT nIDPDefault)
{
	AfxMessageBox("Error opening document. Please check the path", MB_ICONEXCLAMATION);
}
