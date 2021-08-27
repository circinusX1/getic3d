/////////////////////////////////////////////////////////////////////////////
// This ListCtrlDemo is done by Marlon Tang. You can distribute,
// EXCEPT: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.

// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Please send bug reports, bug fixes, enhancements, requests, etc., and
// Fell free to e-mail me at marlont@worldnet.att.net
//
// Special thanks for Matt Gullett to give me this idea
//
///////////////////////a//////////////////////////////////////////////////////

// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "PrpListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PrpListControl

PrpListControl::PrpListControl()
{
    m_pDlg          = 0;
    m_pfooNoty      = 0;
	m_MyEdit		= NULL;
	m_MyComBoBox	= NULL;
}

PrpListControl::~PrpListControl()
{
	if (m_MyEdit)
		delete m_MyEdit;
	if (m_MyComBoBox)
		delete m_MyComBoBox;
	m_Bitmap_DC.SelectObject(m_pBitmap);
	m_Bitmap_DC.DeleteDC();
}

BEGIN_MESSAGE_MAP(PrpListControl, CListCtrl)
	//{{AFX_MSG_MAP(PrpListControl)
    ON_WM_MEASUREITEM_REFLECT()
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    
    ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_MESSAGE_NOTIFY,	(LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))(OnUserMessageNotify))
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PrpListControl message handlers
void PrpListControl::PreSubclassWindow()
{

    CFont* pf = GetFont();
    LOGFONT lf; pf->GetLogFont(&lf);
    lf.lfWeight = 200;
    lf.lfWeight +=1;
    m_boldFont.CreateFontIndirect(&lf);

    CListCtrl::PreSubclassWindow();
}


void PrpListControl::OnDestroy() 
{
	DeleteAllItems();	
	CListCtrl::OnDestroy();
    m_boldFont.DeleteObject();
}

LRESULT PrpListControl::OnSetFont(WPARAM wParam, LPARAM)
{
	LRESULT res =  Default();

	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
	return res;
}

void PrpListControl::MeasureItem ( LPMEASUREITEMSTRUCT lpms )
{
    CFont* pf = GetFont();
    LOGFONT lf;
    pf->GetLogFont(&lf);
    lpms->itemHeight = (int)(abs(lf.lfHeight) * 1.8);
}



void PrpListControl::DrawItem(LPDRAWITEMSTRUCT pdis)
{
	CMyLParam*	MyLParam	= GetMyLParam(pdis->itemID);
    
    if(0==MyLParam)        return;

	CDC*		pDC			= CDC::FromHandle(pdis->hDC);
	void*		MyVoid		= NULL;
	CWnd*		pWnd		= NULL;

    if(MyLParam->GetControlType() == HEADER)
    {
        CRect rt;
        this->GetItemRect(pdis->itemID,&rt, LVIR_BOUNDS);
        pDC->FillRect(rt, &theApp._brush[2]);
        pDC->Draw3dRect(rt, theApp._color[2], theApp._color[0]);

        //CFont* pof = pDC->SelectObject(&m_boldFont);
	    pDC->DrawText(CListCtrl::GetItemText(pdis->itemID,0), rt, 
					    DT_SINGLELINE | DT_LEFT | DT_VCENTER);
        //pDC->SelectObject(&pof);
    }
    else
    {


	    CRect		rcItem      = GetCellRect(pdis->itemID, 0);

        InflateRect(&rcItem,-1,-1);
        CBrush*         pob = 0;
        
        pDC->FillRect(rcItem, &theApp._brush[2]);

        if(pdis->itemState &  ODS_SELECTED)
        {
            pDC->FillRect(rcItem, &theApp._brush[1]);
            pDC->Draw3dRect(rcItem, theApp._color[0], theApp._color[2]);
        }
        else
        {
            pDC->FillRect(rcItem, &theApp._brush[2]);
            pDC->Draw3dRect(rcItem, theApp._color[2], theApp._color[0]);
        } 
        
        
        
        rcItem.left	+= 2;
	    rcItem.top	+= 1;
	    pDC->DrawText(CListCtrl::GetItemText(pdis->itemID,0), rcItem, 
					    DT_SINGLELINE | DT_LEFT | DT_VCENTER);
    		
	    rcItem	= GetCellRect(pdis->itemID, 1);
    	
        InflateRect(&rcItem,-1,-1);
        
        //pDC->FillRect(rcItem, &theApp._brush[2]);

        if(pdis->itemState &  ODS_SELECTED)
        {
            pDC->Draw3dRect(rcItem, theApp._color[0], theApp._color[2]);
        }
        else
        {
            pDC->Draw3dRect(rcItem, theApp._color[2], theApp._color[0]);
        }

	    rcItem.left		+= 2;
	    rcItem.top		+= 1;
	    rcItem.bottom	-= 1;
	    if (NO_CONTROL	!= MyLParam->GetControlType())
		    rcItem.right -= rcItem.Height();
	    
        if (MyLParam->GetOPType() == FPSPROPERTYITEMTYPE_COLOR)
		    pDC->FillSolidRect(rcItem, (COLORREF)MyLParam->GetData());
	    else
		    pDC->DrawText(FindText(pdis->itemID, MyLParam), rcItem, 
						        DT_SINGLELINE | DT_LEFT | DT_VCENTER);


	    rcItem.left		= rcItem.right - 1;
	    rcItem.top		-= 1;
	    rcItem.right	+= rcItem.Height();

        int offset = (rcItem.Height()-16) / 2;
        

	    switch (MyLParam->GetControlType())
	    {	//	Can not use Bitblt here because rcItem.Height() are different
		    //	for different screen resolution and Font size, but my bitmap
		    //	are always the same size.
		    case 	SPIN_CONTROL:
			    pDC->StretchBlt(offset+rcItem.left,offset+rcItem.top,
							    16,16,
							    &m_Bitmap_DC,
							    SPIN_BITMP*16,0,
							    16,16,
							    SRCCOPY);
		        break;
		    case 	BUTTON_CONTROL:
			    pDC->StretchBlt(offset+rcItem.left,offset+rcItem.top,
							    16,16,
							    &m_Bitmap_DC,
							    BUTTON_UP_BITMAP*16,0,
							    16,16,
							    SRCCOPY);
		        break;
            case CB_CONTROL:
			    pDC->StretchBlt(offset+rcItem.left,offset+rcItem.top,
							    16,16,
							    &m_Bitmap_DC,
							    COMBO_BITMAP*16,0,
							    16,16,
							    SRCCOPY);
                break;
            case CK_BOX:
                {
                    int imul = MyLParam->GetData() ? 1 : 0;
			        pDC->StretchBlt(offset+rcItem.left,offset+rcItem.top,
							    16,16,
							    &m_Bitmap_DC,
                                ((imul + CHECK_BITMAP)*16),0,
							    16,16,
							    SRCCOPY);

                    

                }
		    case 	NO_CONTROL:
		    default:
		    break;
	    }
    }


}

void PrpListControl::Initial(CString Title, int	TitleWidth,
						  CString Data,	 int	DataWidth)
{
	CListCtrl::InsertColumn( 0, Title,	LVCFMT_LEFT, TitleWidth);
	CListCtrl::InsertColumn( 1, Data,	LVCFMT_LEFT, DataWidth);
	CBitmap			MyBitmap;
	MyBitmap.LoadBitmap(IDB_BITMAP);
    m_Bitmap_DC.CreateCompatibleDC(NULL);            
	m_pBitmap	= m_Bitmap_DC.SelectObject(&MyBitmap);
	MyBitmap.DeleteObject();
}

BOOL PrpListControl::AddItem(CString		Information, 
						  CString			Data, 
						  OPERATION_TYPE	Operation_type,
						  void*				InputStringList)
{
	if ((Information.IsEmpty())||
		(FPSPROPERTYITEMTYPE_UNKNOW==Operation_type))
		return FALSE;
	
	LV_ITEM lv;
	memset(&lv, 0, sizeof(lv));
	lv.state		= 0;
    lv.iImage		= 0;
	
	lv.iItem		= CListCtrl::GetItemCount();
	lv.iSubItem		= 0;
	lv.mask			= LVIF_TEXT|LVIF_PARAM;
	lv.lParam		= (size_t) SetRelatedInfo(Operation_type, InputStringList, Data);
	lv.pszText		= (LPTSTR)(const char*)Information;
	lv.cchTextMax	= Information.GetLength();
	int	index		= CListCtrl::InsertItem(&lv);

	lv.iItem		= index;
	lv.iSubItem		= 1;
	lv.mask			= LVIF_TEXT;
	lv.lParam		= NULL;
	lv.pszText		= (LPTSTR)(const char*)Data;
	lv.cchTextMax	= Data.GetLength();
	CListCtrl::SetItem(&lv);
    SetFont(GetFont());
	return TRUE;
}

BOOL PrpListControl::AddItem(CString			Information, 
						  int				Data, 
						  OPERATION_TYPE	Operation_type,
						  void*				InputStringList)
{
	char	temp[50];
	return AddItem (Information, itoa(Data, temp,10),
					Operation_type,InputStringList);
}

BOOL PrpListControl::AddItem(CString			Information, 
						  long				Data, 
						  OPERATION_TYPE	Operation_type,
						  void*				InputStringList)
{
	char	temp[50];
	return AddItem (Information, ltoa(Data, temp, 10),
					Operation_type,InputStringList);
}

CRect PrpListControl::GetCellRect(int nItem, int nColumn)
{
	static CRect rectItem;
	static int nWidth = 0;
	static int cx = 0;

	// Get the rectangle to output the string
	if (0 == nColumn)
	{
		CListCtrl::GetItemRect(nItem, rectItem, LVIR_LABEL);
		rectItem.left	-= 2;
		nWidth = rectItem.Width();
		cx = 0;
	}
	else
	{
		LV_COLUMN column;
		column.mask = LVCF_WIDTH;        // Set the mask to only return the width

		CListCtrl::GetColumn(nColumn - 1, &column); // Get column width information.
		cx += column.cx;                 // Increment for multiple column widths.

		CListCtrl::GetColumn(nColumn, &column);
		nWidth = column.cx;
	}

	CRect rectCell = rectItem;
	rectCell.left = rectItem.left + cx;
	rectCell.right = rectCell.left + nWidth;
	return rectCell;
}

CString PrpListControl::FindText(int Index, CMyLParam*	MyLParam)
{
	CString		DataString;
	switch (MyLParam->GetOPType())
	{
		case FPSPROPERTYITEMTYPE_FONT:
			DataString	= ((LOGFONT*)MyLParam->GetData())->lfFaceName;
		break;
		case FPSPROPERTYITEMTYPE_BOOL:
			if (MyLParam->GetData())
				DataString	= "TRUE";
			else
				DataString	= "false";
		break;
		default:
			DataString = CListCtrl::GetItemText(Index,1);
		break;
	}
	return DataString;
}

void PrpListControl::MyOutText(CString	MyString, CDC* pDC, CRect* rect)
{
	CRect	MyRect	= rect;
	////pDC->Rectangle(MyRect);
	MyRect.left	+= 2;
	MyRect.top	+= 1;
	pDC->DrawText(MyString, MyRect, 
				DT_SINGLELINE | DT_LEFT | DT_VCENTER);
}

void* PrpListControl::SetRelatedInfo(OPERATION_TYPE Operation_type, 
								  void*			InputStringList,
								  CString		Data)
{
	CMyLParam*	MyLParam	= new CMyLParam;
	CStringList*			MyStringList;
	void*		pReturnVoid	= NULL;
	LOGFONT*	MyFont;
	CRect		rect(0,0,0,0);
	switch (Operation_type)
	{
		case FPSPROPERTYITEMTYPE_INTEGER:			// CMyEditBox
		case FPSPROPERTYITEMTYPE_LONG:				// CMyEditBox
			MyLParam->SetInfo(Operation_type, SPIN_CONTROL);
		break;
		case FPSPROPERTYITEMTYPE_FONT:				// Button
			MyFont	= new LOGFONT;
			FontPassing((LOGFONT*)InputStringList, MyFont);
			MyLParam->SetInfo(Operation_type, BUTTON_CONTROL, MyFont);
		break;
		case FPSPROPERTYITEMTYPE_FILE:				// Button
			MyLParam->SetInfo(Operation_type, BUTTON_CONTROL);
		break;
		case FPSPROPERTYITEMTYPE_COLOR:				// Button
			MyLParam->SetInfo(Operation_type, BUTTON_CONTROL, InputStringList);
		break;
		case FPSPROPERTYITEMTYPE_COMBOBOX:			// CComboBox
			MyStringList	= new CStringList;
			DataPassing((CStringList*)InputStringList, MyStringList);
			MyLParam->SetInfo(Operation_type, CB_CONTROL, (void*)MyStringList);
		break;
		case FPSPROPERTYITEMTYPE_TEXT:				// CMyEditBox
		case FPSPROPERTYITEMTYPE_UNKNOW:				// ERROR
			MyLParam->SetInfo(Operation_type, NO_CONTROL);
		break;
		case FPSPROPERTYITEMTYPE_BOOL:				// DoubleClick
			MyLParam->SetInfo(Operation_type, CK_BOX, InputStringList);
		break;
		case FPSPROPERTYITEMTYPE_HEADER:				// DoubleClick
			MyLParam->SetInfo(Operation_type, HEADER);
		break;
        
		default:
		break;
	}
	return (void*) MyLParam;
}

void PrpListControl::DataPassing(CStringList*	InputStringList, 
							  CStringList*	MyStringList)
{
	if (InputStringList)
	{
		POSITION	pos	= InputStringList->GetHeadPosition();
		while (pos)
			MyStringList->AddTail(InputStringList->GetNext(pos));
	}
}

void PrpListControl::FontPassing(LOGFONT*	InputStringList, 
							  LOGFONT*	MyFont)
{
	MyFont->lfHeight		= InputStringList->lfHeight;   
	MyFont->lfWidth			= InputStringList->lfWidth;   
	MyFont->lfEscapement	= InputStringList->lfEscapement;   
	MyFont->lfOrientation	= InputStringList->lfOrientation;
	MyFont->lfWeight		= InputStringList->lfWeight;   
	MyFont->lfItalic		= InputStringList->lfItalic;   
	MyFont->lfUnderline		= InputStringList->lfUnderline;   
	MyFont->lfStrikeOut		= InputStringList->lfStrikeOut;
	MyFont->lfCharSet		= InputStringList->lfCharSet;   
	MyFont->lfOutPrecision	= InputStringList->lfOutPrecision;   
	MyFont->lfClipPrecision	= InputStringList->lfClipPrecision;
	MyFont->lfQuality		= InputStringList->lfQuality;   
	MyFont->lfPitchAndFamily= InputStringList->lfPitchAndFamily;   
	lstrcpy(MyFont->lfFaceName, (const char*) InputStringList->lfFaceName);
}

BOOL PrpListControl::DeleteAllItems()
{
	while (CListCtrl::GetItemCount())
		DeleteItem(0);
	return TRUE;
}

BOOL PrpListControl::DeleteItem( int nItem )
{
	CMyLParam*	MyLParam	= GetMyLParam(nItem);
	void*		MyVoid		= MyLParam->GetData();
	LOGFONT*	MyFont;
	if (NULL	!= MyVoid)
	{
		CStringList*		MyStringList		= NULL;
		switch (MyLParam->GetOPType())
		{
			case FPSPROPERTYITEMTYPE_FONT:				// Button
				MyFont		=	(LOGFONT*)MyLParam->GetData();
				delete MyFont;
			break;
			case FPSPROPERTYITEMTYPE_COMBOBOX:			// CComboBox
				MyStringList =	(CStringList*)MyVoid;
				delete MyStringList;
			break;
			case FPSPROPERTYITEMTYPE_COLOR:				// Button
			case FPSPROPERTYITEMTYPE_FILE:				// Button
			case FPSPROPERTYITEMTYPE_INTEGER:			// CMyEditBox
			case FPSPROPERTYITEMTYPE_LONG:				// CMyEditBox
			case FPSPROPERTYITEMTYPE_TEXT:				// CMyEditBox
			case FPSPROPERTYITEMTYPE_UNKNOW:				// ERROR
			case FPSPROPERTYITEMTYPE_BOOL:				// DoubleClick
			break;
			default:
			break;
		}
	}
	delete	MyLParam;	
	CListCtrl::DeleteItem(nItem);
	return TRUE;
}

CMyLParam*	PrpListControl::GetMyLParam(int Index)
{
	LVITEM	Item;
	Item.mask		= LVIF_PARAM;
	Item.iItem		= Index;
	Item.iSubItem	= 0;
	CListCtrl::GetItem(&Item);
	return (CMyLParam*)Item.lParam;
}

BOOL	PrpListControl::GetData(int				Index,
							 OPERATION_TYPE&	Op_type,
							 CString&			Title,
							 CString&			Data,
							 void*&				DataPointer)
{
	if (Index>=CListCtrl::GetItemCount())
		return FALSE;
	CMyLParam*	MyLParam	= GetMyLParam(Index);
	Op_type	= MyLParam->GetOPType();
	Title	= CListCtrl::GetItemText(Index,0);
	switch (Op_type)
	{
		case FPSPROPERTYITEMTYPE_INTEGER:			// CMyEditBox
		case FPSPROPERTYITEMTYPE_LONG:				// CMyEditBox
		case FPSPROPERTYITEMTYPE_TEXT:				// CMyEditBox
		case FPSPROPERTYITEMTYPE_FILE:				// Button
		case FPSPROPERTYITEMTYPE_COMBOBOX:			// CComboBox
			Data		= CListCtrl::GetItemText(Index,1);
			DataPointer	= NULL;
		break;
		case FPSPROPERTYITEMTYPE_BOOL:				// DoubleClick
			Data		= CListCtrl::GetItemText(Index,1);
			DataPointer	= MyLParam->GetData();
		break;
		case FPSPROPERTYITEMTYPE_FONT:				// Button
		case FPSPROPERTYITEMTYPE_COLOR:				// Button
			DataPointer	= MyLParam->GetData();
		break;
		case FPSPROPERTYITEMTYPE_UNKNOW:			// ERROR
			return FALSE;
		default:
		break;
	}
	return TRUE;
}

BOOL	PrpListControl::FindData(CString			Title,
							  int&				Index,
							  OPERATION_TYPE&	Op_type,
							  CString&			Data,
							  void*&			DataPointer)
{
	Index	= 0;
	BOOL	FindIt	= FALSE;
	while ((!FindIt)&&
			CListCtrl::GetItemCount()>Index)
	{
		if (CListCtrl::GetItemText(Index, 0) == Title)
			FindIt = TRUE;
		else
			Index++;
	}
	if (!FindIt)
		return FALSE;
	return GetData(Index,Op_type,Title,Data,DataPointer);
}

BOOL PrpListControl::GetTitle(int	Index, CString& Title)
{
	if (Index>CListCtrl::GetItemCount())
		return FALSE;
	Title = CListCtrl::GetItemText(Index, 0);
	return TRUE;
}

void PrpListControl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	size_t	dwPos	= ::GetMessagePos();
	CPoint	point((int) LOWORD (dwPos), (int)HIWORD(dwPos));
	ScreenToClient(&point);
	int		Index	= CListCtrl::HitTest(point);
	if (LB_ERR	== Index)
		return;
	CRect	rcItem	= GetCellRect(Index, 0);
	rcItem	= GetCellRect(Index, 1);
	
	if (!rcItem.PtInRect(point))
		return;
	CMyLParam*	MyLParam	= GetMyLParam(Index);
	CString	Data;
	switch (MyLParam->GetOPType())
	{
		case FPSPROPERTYITEMTYPE_INTEGER:			// CMyEditBox
			rcItem.right -= rcItem.Height();
			if (!rcItem.PtInRect(point))
				OperateSpinInt( Index, 
								point.y > (rcItem.top + rcItem.Height() / 2));
			else
				OperateEditBox(Index, rcItem, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_LONG:				// CMyEditBox
			rcItem.right -= rcItem.Height();
			if (!rcItem.PtInRect(point))
				OperateSpinLong(Index, 
								point.y > (rcItem.top + rcItem.Height() / 2));
			else
				OperateEditBox(Index, rcItem, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_TEXT:				// CMyEditBox
			OperateEditBox(Index, rcItem, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_FILE:				// Button
			rcItem.right -= rcItem.Height();
			if (!rcItem.PtInRect(point))
				OperateFile(Index, rcItem);
			else
				OperateEditBox(Index, rcItem, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_COMBOBOX:			// CComboBox
			OperateComboBox(Index, rcItem, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_BOOL:				// DoubleClick
			OperateBOOL(Index, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_FONT:				// Button
			rcItem.right -= rcItem.Height();
			if (!rcItem.PtInRect(point))
				OperateFont(Index, rcItem, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_COLOR:				// Button
			rcItem.right -= rcItem.Height();
			if (!rcItem.PtInRect(point))
				OperateColor(Index, rcItem, MyLParam);
		break;
		case FPSPROPERTYITEMTYPE_UNKNOW:			// ERROR
		default:
		break;
	}
	*pResult = 1;
    if(m_pfooNoty)
    {
        ((m_pDlg)->*(m_pfooNoty))(this,Index);
    }
}

void PrpListControl::OperateBOOL(int Index, CMyLParam*&	MyLParam)
{
	if (MyLParam->GetData())
	{
		MyLParam->SetData((void*)0);
		CListCtrl::SetItemText(Index,1,"FALSE");
	}
	else
	{
		MyLParam->SetData((void*)1);
		CListCtrl::SetItemText(Index,1,"TRUE");
	}
}

void PrpListControl::OperateFont(int	Index, CRect rcItem, CMyLParam*&	MyLParam)
{
	DrawDownButton(rcItem);
	LOGFONT*	MyFont	= (LOGFONT*)MyLParam->GetData();
	CFontDialog dlg (MyFont);
	if (dlg.DoModal() == IDOK)
	{
		if (!MyFont)
			MyFont = new LOGFONT;
		dlg.GetCurrentFont(MyFont);
		MyLParam->SetData((void*) MyFont);
		CListCtrl::SetItemText(Index,1,MyFont->lfFaceName);
	}
}

void PrpListControl::OperateColor(int	Index, CRect rcItem, CMyLParam*&	MyLParam)
{
	DrawDownButton(rcItem);
	CColorDialog	dlg((COLORREF)MyLParam->GetData());
	if (dlg.DoModal() == IDOK)
	{
		MyLParam->SetData((void*) dlg.GetColor());
		RedrawWindow();
	}
}

void PrpListControl::OperateSpinInt(int	Index, BOOL	Incerasing)
{
	CString Data	= CListCtrl::GetItemText(Index, 1);
	int		DataInt	= atoi(Data);
	if (Incerasing)
	{
		if (32767>DataInt)
			DataInt++;
	}
	else
	{
		if (-32768 <DataInt)
			DataInt--;
	}
	char	temp[15];
	CListCtrl::SetItemText(Index,1,itoa(DataInt, temp, 10));
}

void PrpListControl::OperateSpinLong(int	Index, BOOL	Incerasing)
{
	CString Data	= CListCtrl::GetItemText(Index, 1);
	long	DataInt	= atol(Data);
	if (Incerasing)
	{
		if (2147483647	> DataInt)
			DataInt++;
	}
	else
	{
			if (-2147483647	< DataInt)
				DataInt--;
	}
	char	temp[32];
	CListCtrl::SetItemText(Index,1,ltoa(DataInt, temp, 10));
}

void PrpListControl::OperateFile(int	Index, CRect rcItem)
{
	DrawDownButton(rcItem);
	CFileDialog dlg (TRUE, 
					NULL, 
					CListCtrl::GetItemText(Index, 1),
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Access Files (*.MDB)|*.mdb|All Files (*.*)|*.*|");

	if (dlg.DoModal() == IDOK)
		CListCtrl::SetItemText(Index,1,dlg.GetPathName());
}

void PrpListControl::OperateEditBox (int			Index,
								  CRect			rcItem,
								  CMyLParam*	MyLParam)
{
	if (m_MyEdit)
		delete m_MyEdit;
	m_MyEdit = new CMyEdit;
	if (m_MyEdit->Create(this, rcItem, Index))
    {
        m_MyEdit->SetFont(GetParent()->GetFont());
        m_MyEdit->SetWindowText(CListCtrl::GetItemText(Index,1));
    }
	else
	{
        m_MyEdit->SetFont(0);
		delete m_MyEdit;
		m_MyEdit	= NULL;
	}
}

void PrpListControl::OperateComboBox(int			Index,  
								  CRect			rcItem,
								  CMyLParam*	MyLParam)
{
	CStringList*	MyStringList	= (CStringList*) MyLParam->GetData();
	if (m_MyComBoBox)
		delete m_MyComBoBox;
	m_MyComBoBox = new CMyComboBox;
	rcItem.bottom += 100;
	if (m_MyComBoBox->Create(this, rcItem, Index))
	{
        m_MyComBoBox->ModifyStyle(CBS_SORT,0);
        m_MyComBoBox->SetFont(GetParent()->GetFont());
		POSITION pos = MyStringList->GetHeadPosition();
		while (pos)
			m_MyComBoBox->AddString(MyStringList->GetNext(pos));
		Index	= m_MyComBoBox->FindStringExact(-1,CListCtrl::GetItemText(Index,1));
		if (LB_ERR != Index)
			m_MyComBoBox->SetCurSel(Index);
	}
	else
	{
        m_MyComBoBox->SetFont(0);
		delete m_MyComBoBox;
		m_MyComBoBox	= NULL;
	}
}

LRESULT PrpListControl::OnUserMessageNotify(LPARAM nID, WPARAM Index)
{
	switch (nID)
	{
		case MY_EDIT_UNIT:
			UpdateEditBox((int)Index);
		break;
		case MY_COMBOBOX_UINT:
			UpdateComboBox((int)Index);
		break;
		default:
		break;
	}
	return 0;
}

void PrpListControl::UpdateEditBox (int	Index)
{
	if (m_MyEdit)
	{
		if (!m_MyEdit->m_bESC)
		{
			CMyLParam*	MyLParam	= GetMyLParam(Index);
			CString	Data;
			char	temp[11];
			m_MyEdit->GetWindowText(Data);
			switch (MyLParam->GetOPType())
			{
				case FPSPROPERTYITEMTYPE_INTEGER:			// CMyEditBox
					Data	= itoa(atoi(Data),temp,10);
				break;
				case FPSPROPERTYITEMTYPE_LONG:				// CMyEditBox
					Data	= ltoa(atol(Data),temp,10);
				break;
				default:
				break;
			}
			CListCtrl::SetItemText(Index,1,Data);
		}
		delete	m_MyEdit;
		m_MyEdit	= NULL;
		SetFocus();
	}
}

void PrpListControl::UpdateComboBox(int	Index)
{
	if (m_MyComBoBox)
	{
		if (!m_MyComBoBox->m_bESC)
		{
			CString	Data;
			m_MyComBoBox->GetWindowText(Data);
			CListCtrl::SetItemText(Index,1,Data);
		}
		delete m_MyComBoBox;
		m_MyComBoBox	= NULL;
	}
}

void PrpListControl::DrawDownButton(CRect&	rcItem)
{
	CDC*	pDC	= GetDC();
	rcItem.left= rcItem.right;
	rcItem.right += rcItem.Height();
	pDC->StretchBlt(rcItem.left,rcItem.top,
					rcItem.Height(),rcItem.Height(),
					&m_Bitmap_DC,
					BUTTON_DOWN_BITMAP*16,0,
					16,16,
					SRCCOPY);
}

BOOL PrpListControl::OnEraseBkgnd(CDC* pDC) 
{
    RECT rt; GetClientRect(&rt);
    CBrush cb(RGB(192,192,192));
    pDC->FillRect(&rt, &cb);//&theApp._brush[1]);
    return 1;
}


/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_bESC	= FALSE;
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers
BOOL CMyEdit::Create(CWnd* pParentWnd,
					 CRect rect, 
					 int Index)
{
	m_Index	= Index;
	if (!pParentWnd)
		return FALSE;
	BOOL	returnValue	= CEdit::Create(ES_LEFT|WS_BORDER|WS_CHILD|
										WS_VISIBLE|ES_AUTOHSCROLL, 
										rect, 
										pParentWnd, 
										MY_EDIT_UNIT);
    SetLimitText(32);
	SetFocus();
	return returnValue;
}

void CMyEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	GetParent()->SendMessage(WM_USER_MESSAGE_NOTIFY, 
							 (WPARAM) MY_EDIT_UNIT, 
	     					 (LPARAM) m_Index);
}

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
				m_bESC = TRUE;
			case VK_RETURN:
				GetParent()->SetFocus();
				return 1;
			break;
			default:
			break;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox

CMyComboBox::CMyComboBox()
{
	m_bESC	= FALSE;
}

CMyComboBox::~CMyComboBox()
{
}


BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMyComboBox)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox message handlers

BOOL CMyComboBox::Create(CWnd* pParentWnd, CRect rect, int Index)
{
	m_Index			= Index;
	if (!pParentWnd)
		return FALSE;
	
	BOOL	returnValue	= CComboBox::Create(WS_VISIBLE|WS_CHILD|
											WS_VSCROLL|WS_GROUP|WS_TABSTOP|
											CBS_DROPDOWNLIST, 
											rect, 
											pParentWnd, 
											MY_COMBOBOX_UINT);
	SetFocus();
	return returnValue;
}

void CMyComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	GetParent()->SendMessage(WM_USER_MESSAGE_NOTIFY, 
							 (WPARAM) MY_COMBOBOX_UINT, 
	     					 (LPARAM) m_Index);
}

BOOL CMyComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
				m_bESC = TRUE;
			case VK_RETURN:
				GetParent()->SetFocus();
				return 1;
			break;
			default:
			break;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

