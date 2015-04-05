#include "stdafx.h"
#include "z-edmap.h"
#include "RepLBox.h"
#include "MainFrm.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRepLBox

CRepLBox::CRepLBox()
{
}

CRepLBox::~CRepLBox()
{
}


BEGIN_MESSAGE_MAP(CRepLBox, CListBox)
	//{{AFX_MSG_MAP(CRepLBox)
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblclk)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRepLBox message handlers
void CRepLBox::DrawItem(LPDRAWITEMSTRUCT pDraw)
{
   
    char	lstring[512]={0};
	int		obm;
	RECT	rs;

	CopyRect(&rs,&pDraw->rcItem);
    
	obm = SetBkColor(pDraw->hDC,theApp._color[1]);
	
    if (pDraw->itemState & ODS_SELECTED )  // up button
	{
		CDC::FromHandle(pDraw->hDC)->MoveTo(rs.left,rs.bottom-2);
        CDC::FromHandle(pDraw->hDC)->LineTo(rs.right,rs.bottom-2);
	}
    else
    {
		::FillRect(pDraw->hDC,
						&rs,
						_bkBrush);
    }
    

    if(pDraw->itemData & LOG_WARNING)
	    SetTextColor(pDraw->hDC,RGB(128,64,0));
    else if (pDraw->itemData & LOG_ERROR)
        SetTextColor(pDraw->hDC,RGB(192,0,0));
    else
        SetTextColor(pDraw->hDC,theApp._txtcolor);

	GetText(pDraw->itemID,lstring);
    if(0==lstring[0])return;

	pDraw->rcItem.left+=16;
	::TextOut(pDraw->hDC,
		pDraw->rcItem.left+16,
		pDraw->rcItem.top,lstring,strlen(lstring));

}


void CRepLBox::PreSubclassWindow() 
{
    if(_bkBrush.m_hObject)
        _bkBrush.DeleteObject();
	_bkBrush.CreateSolidBrush(theApp._color[1]);
	CListBox::PreSubclassWindow();
}

void CRepLBox::OnDblclk() 
{
    int i = GetCurSel();
    map<int,V3>& errPoints = CMainFrame::PFrame->_dlgProgress._mapidPoint;
    map<int,V3>::iterator fi = errPoints.find(i);
    if(fi == errPoints.end())
        return;
    // move the camera in that position and the 3d views
    DOC()->SetErrorPoint((*fi).second);
    DOC()->MoveCamera((*fi).second);
}

void CRepLBox::OnDestroy() 
{
	
    _bkBrush.DeleteObject();

	CListBox::OnDestroy();
}



HBRUSH CRepLBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    return _bkBrush;
}
