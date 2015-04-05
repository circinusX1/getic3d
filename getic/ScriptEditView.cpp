// ScriptEditView.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "ScriptEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView

IMPLEMENT_DYNCREATE(CScriptEditView, CView)

CScriptEditView::CScriptEditView()
{
}

CScriptEditView::~CScriptEditView()
{
}


BEGIN_MESSAGE_MAP(CScriptEditView, CView)
	//{{AFX_MSG_MAP(CScriptEditView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView drawing

void CScriptEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView diagnostics

#ifdef _DEBUG
void CScriptEditView::AssertValid() const
{
	CView::AssertValid();
}

void CScriptEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView message handlers

BOOL CScriptEditView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

BOOL CScriptEditView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return 1;//CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
