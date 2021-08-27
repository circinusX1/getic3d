// PrpLb.cpp : implementation file
//


#include "stdafx.h"
#include "z-edmap.h"
#include "PrpLb.h"
 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrpLb

CPrpLb::CPrpLb()
{
    _dlgXDim = 140;
    _dlgYDim = 25;
    _labelRaport = 0;
}

CPrpLb::~CPrpLb()
{
}


BEGIN_MESSAGE_MAP(CPrpLb, CListBox)
	//{{AFX_MSG_MAP(CPrpLb)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_CONTROL_REFLECT(LBN_SETFOCUS, OnSetfocus)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_MESSAGE(LB_SETCURSEL,OnSetCurSel)
    ON_MESSAGE(WM_REDRAW_DLG, OnRedraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrpLb message handlers
CWnd* PGDlg;

void CPrpLb::PreSubclassWindow() 
{
	CListBox::PreSubclassWindow();
    _curSel = GetCurSel();
    PGDlg = GetParent();
    _invDlg = 0;
}

void CPrpLb::MeasureItem(LPMEASUREITEMSTRUCT pMi) 
{
    pMi->itemHeight = _dlgYDim-1;
}

BOOL CPrpLb::OnEraseBkgnd(CDC* pDC) 
{
    BOOL b = CListBox::OnEraseBkgnd(pDC);
    SeeRects();
    return b ;
}


void CPrpLb::DrawItem(LPDRAWITEMSTRUCT pDraw) 
{
    if(_invDlg)
        return;

    char tOut[128]={0};
    int  crOldBack,crOldText;

    if((pDraw->rcItem.bottom - pDraw->rcItem.top)< 4)
        return;
    
    CDC* pDC = CDC::FromHandle( pDraw->hDC );
    COLORREF hl = theApp._color[2];
    COLORREF sh = theApp._color[0];

    pDraw->rcItem.right -= _dlgXDim;
    GetText(pDraw->itemID ,tOut);
    if(0==tOut[0])return;

    if (pDraw->itemState & ODS_SELECTED )  // up button
    	crOldBack = pDC->SetBkColor(theApp._color[0]);
    else
        crOldBack = pDC->SetBkColor(theApp._color[1]);

	crOldText = pDC->SetTextColor(theApp._txtcolor);
    
    pDC->ExtTextOut(pDraw->rcItem.left+1,
                    pDraw->rcItem.top+4,
                    ETO_OPAQUE, &pDraw->rcItem,tOut, strlen(tOut), NULL );// fast then Fill Rect
    
    pDC->SetTextColor( crOldText );
    pDC->SetBkColor( crOldBack );

    CLbDlg* pDlg = (CLbDlg*)pDraw->itemData;

    _invDlg = 1;
    pDlg->Invalidate();
    _invDlg = 0;


}


void CPrpLb::OnPaint()
{
}


LRESULT CPrpLb::OnRedraw(WPARAM w, LPARAM l)
{
    SeeRects();
    return 0;
}


CLbDlg* CPrpLb::AddItem(char* str, CLbDlg* lbDlg, size_t nID)
{
    int idx = AddString(str);
    
    lbDlg->Create(nID,this);
    lbDlg->ShowWindow(SW_SHOW);

    SetItemData(idx,(size_t)lbDlg);
    lbDlg->SetIndex(idx);
    strcpy(lbDlg->_propName,str);

    RECT rt;    
    RECT rtl;    
    lbDlg->GetClientRect(&rt);
    GetClientRect(&rtl);

    if(_labelRaport==0)
    {
        _dlgXDim = rt.right;
       
    }
    else
    {
        _dlgXDim = (int)((float)rtl.right/(float)_labelRaport);

    }
     _dlgYDim = rt.bottom;

    SeeRects();
	_dlgAsoc[str] = lbDlg;
    return lbDlg;
}

CLbDlg* CPrpLb::AddItem(char* str, size_t nID)
{
    CLbDlg* pNewDlg = 0;
    int idx = AddString(str);

    switch(nID)
    {
        case DL_CBSEL:
            pNewDlg =  new DlbCB;
            break;
        case DL_CHECKS:
            pNewDlg =  new DlbCKs;
            break;
        case DL_COLOR:
            pNewDlg =  new DlbCLR;
            break;
        case DL_EDIT:
            pNewDlg =  new DlbEDIT;
            break;
        case DL_EDSPIN:
            pNewDlg =  new DlbEDSpin;
            break;
        case DL_EDXYZ:
            pNewDlg =  new DlbXyz;
            break;
        case DL_RANGE:
            pNewDlg =  new DlbRANGE; 
            break;
        case DL_UV:
            pNewDlg =  new DlbUV;
            break;
        case DL_FILEPCK:
            pNewDlg =  new DlbFILE;
            break;
        case DL_RGB1:
            pNewDlg =  new DlbRB;
            break;

    }
    pNewDlg->Create(nID,this);
    pNewDlg->ShowWindow(SW_SHOW);

    //pNewDlg->SetWindowPos(this,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

    SetItemData(idx,(size_t)pNewDlg);
    strcpy(pNewDlg->_propName,str);
    pNewDlg->SetIndex(idx);

    
   
    
    RECT rtl;    
    GetClientRect(&rtl);

    pNewDlg->GetClientRect(&pNewDlg->_initalRect);
	pNewDlg->_initalRect.bottom+=4;
    SetItemHeight( idx, pNewDlg->_initalRect.bottom);

    if(_labelRaport==0)
    {
        _dlgXDim = pNewDlg->_initalRect.right;
        _dlgYDim = pNewDlg->_initalRect.bottom-1;
    }
    else
    {
        int labl = (rtl.right * (_labelRaport))/100;
        _dlgXDim = rtl.right - labl;

    }
    _dlgYDim = pNewDlg->_initalRect.bottom-1;


    //SeeRects();
	_dlgAsoc[str] = pNewDlg;
    return pNewDlg;
}

void CPrpLb::OnDestroy() 
{
	CListBox::OnDestroy();
    int iCnt =  this->GetCount();
    for(int i=0;i<iCnt;i++)
    {
        CLbDlg* pDlg =  (CLbDlg*)this->GetItemData(i);
        pDlg->DestroyWindow();
        delete pDlg;
    }
}

void CPrpLb::Clean() 
{
    int iCnt =  this->GetCount();
    for(int i=0;i<iCnt;i++)
    {
        CLbDlg* pDlg =  (CLbDlg*)this->GetItemData(i);
        pDlg->DestroyWindow();
        delete pDlg;
    }
    ResetContent();
}


void CPrpLb::OnVScroll(size_t nSBCode, size_t nPos, CScrollBar* pScrollBar) 
{
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
    SeeRects();
}


void    CPrpLb::SeeRects() //CListBox
{
    int iCnt =  this->GetCount();

    if( 0 == iCnt)
        return;
    
    int iTop = GetTopIndex();
    int iiH  = 0;
    RECT rt;
    

    for(int i=0; i<iCnt; i++)
    {
        if(i >= iTop)
        {
        
            CLbDlg* pDlg =  (CLbDlg*)this->GetItemData(i);
            GetItemRect( i, &rt) ;

            pDlg->MoveWindow(rt.right-_dlgXDim+1, rt.top+1, _dlgXDim, rt.bottom-rt.top-2, 1);
            iiH  += rt.bottom;
            pDlg->Invalidate();
            pDlg->UpdateWindow();

            ////DelayUpdateWindow(pDlg->m_hWnd);
        }
    }
    Invalidate();
    UpdateWindow();
}

LRESULT CPrpLb::WindowProc(size_t message, WPARAM wParam, LPARAM lParam)
{
    /*
    if(message > LB_MSGMAX || message < LB_ADDSTRING)
    {
        CWnd*   pWnd = this;
        int     relation = GW_CHILD|GW_HWNDFIRST;
        while(pWnd = pWnd->GetWindow(relation))
        {
            
            pWnd->PostMessage(message, wParam, lParam);
            relation = GW_HWNDNEXT;
        }
    }
    */

    return CListBox::WindowProc(message, wParam, lParam);
}

BOOL CPrpLb::PreTranslateMessage(MSG* pMsg) 
{
    /*
    if(pMsg == WM_PAINT)
    {
        CWnd*   pWnd = this;
        int     relation = GW_CHILD|GW_HWNDFIRST;
        while(pWnd = pWnd->GetWindow(relation))
        {
            relation = GW_HWNDNEXT;
        }
    }
    */

	return CListBox::PreTranslateMessage(pMsg);
}


CLbDlg::CLbDlg(size_t iDD, CWnd* pParent)
	: CBASEDLG(iDD, pParent)
{
	//{{AFX_DATA_INIT(CLbDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _lockNoty = 0;
    _index = -1;
    _fileFilter = "*.*";
    _initalPath = "";
    _fileExt    = ".*";
    _onlyFile   = FALSE;
    _bFileOpen = FALSE;
    _ckNumVis  = 1;
    _curValueX = 0;
    _spMinX    = 0; 
    _spMaxX    = 100;
    _curValueY = 0;
    _spMinY    = 0; 
    _spMaxY    = 100;
    _curValueZ = 0;
    _spMinZ    = 0; 
    _spMaxZ    = 100;
    _spStep     = 1; 
    
    _selected  = FALSE;
    _visible   = TRUE;
    _pNoty      = 0;
}


void CLbDlg::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLbDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLbDlg, CBASEDLG)
	//{{AFX_MSG_MAP(CLbDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLbDlg message handlers



void CLbDlg::OnLButtonDown(size_t nFlags, CPoint point) 
{
	CBASEDLG::OnLButtonDown(nFlags, point);
}

void CLbDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CBASEDLG::OnSetFocus(pOldWnd);
	

}

BOOL CLbDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_LBUTTONDOWN)
    {
        CPrpLb* pP = (CPrpLb*)GetParent();
        BOOL   outSide ;
        
        CPoint pr (pMsg->pt.x, pMsg->pt.y);
        pP->ScreenToClient(&pr);
        int itm = pP->ItemFromPoint( pr, outSide);
        if(!outSide)
        {
            pP->PostMessage(LB_SETCURSEL,itm);
            OnSelect();
        }
    }
    return CBASEDLG::PreTranslateMessage(pMsg);
}

BOOL CLbDlg::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLbDlg::Activate(BOOL b)
{
    if(b)
    {
        ShowWindow(SW_SHOW);
        CWnd* p = GetParent();
        ((CPrpLb*)GetParent())->SetItemHeight( _index, _initalRect.bottom);
        _visible = TRUE;
        ((CPrpLb*)GetParent())->PostMessage(WM_KEYDOWN,VK_DOWN,0);//
        ((CPrpLb*)GetParent())->PostMessage(WM_KEYUP,VK_DOWN,0);//

        ((CPrpLb*)GetParent())->PostMessage(WM_KEYDOWN,VK_UP,0);//
        ((CPrpLb*)GetParent())->PostMessage(WM_KEYUP,VK_UP,0);//

    }
    else
    {
        ShowWindow(SW_HIDE);
        ((CPrpLb*)GetParent())->SetItemHeight( _index, 1);
        _visible =FALSE;
    }
    ((CPrpLb*)GetParent())->Invalidate();

}


void CLbDlg::Apply()
{
	/*
    if(_visible)
    {
        RECT rt; GetClientRect(&rt);
        ((CListBox*)GetParent())->SetItemHeight( _index, rt.bottom);
    }
    else
    {
        ((CListBox*)GetParent())->SetItemHeight( _index, 1);
    }
	*/
    
}

void CLbDlg::Noty()
{
    if(_lockNoty)
        return;
    if(_pNoty)
    {
        CWnd*    pWnd = GetParent();
        CBASEDLG* pDlg = (CBASEDLG*)pWnd->GetParent();//->PostMessage(WM_DLGEVENT, nID, (LONG)this);
        if(pDlg)
            (pDlg->*_pNoty)(this);
    }
    DelayUpdateWindow(m_hWnd);
}

int CLbDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBASEDLG::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CLbDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CBASEDLG::OnKillFocus(pNewWnd);
    OnDeselect();
}

/////////////////////////////////////////////////////////////////////////////
// DlbXyz dialog


DlbXyz::DlbXyz(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbXyz::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbXyz)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _lockNoty = FALSE;
}


void DlbXyz::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbXyz)
	DDX_Control(pDX, S_Z, _sp3);
	DDX_Control(pDX, S_Y, _sp2);
	DDX_Control(pDX, S_X, _sp1);
	DDX_Control(pDX, E_Z, _e3);
	DDX_Control(pDX, E_Y, _e2);
	DDX_Control(pDX, E_X, _e1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbXyz, CBASEDLG)
	//{{AFX_MSG_MAP(DlbXyz)
	ON_EN_CHANGE(E_X, OnChangeX)
	ON_EN_CHANGE(E_Y, OnChangeY)
	ON_EN_CHANGE(E_Z, OnChangeZ)
	ON_NOTIFY(UDN_DELTAPOS, S_X, OnDeltaposX)
	ON_NOTIFY(UDN_DELTAPOS, S_Y, OnDeltaposY)
	ON_NOTIFY(UDN_DELTAPOS, S_Z, OnDeltaposZ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbXyz message handlers

void    DlbXyz::OnDeselect()
{
}

void    DlbXyz::OnSelect() 
{
}

void   DlbXyz::Apply()
{
    _lockNoty = 1;

    _sp1.SetRange32(_spMinX * 100, _spMaxX * 100);
    _sp1.SetPos(_curValueX*100);
    _e1.SetWindowText(MKSTR("%.2f",_curValueX));

    _sp2.SetRange32(_spMinY * 100, _spMaxY * 100);
    _sp2.SetPos(_curValueY*100);
    _e2.SetWindowText(MKSTR("%.2f",_curValueY));

    _sp3.SetRange32(_spMinZ * 100, _spMaxZ * 100);
    _sp3.SetPos(_curValueZ*100);
    _e3.SetWindowText(MKSTR("%.2f",_curValueZ));

    _lockNoty = 0;

    CLbDlg::Apply();

}

void DlbXyz::OnChangeX() 
{
    if(_lockNoty) return;
    _lockNoty            = 1;

    _curValueX = ParseREAL(this, E_X);
    _sp1.SetPos(_curValueX*100);

    _lockNoty            = 0;

}

void DlbXyz::OnChangeY() 
{
     if(_lockNoty) return;
    _lockNoty            = 1;
    _curValueY = ParseREAL(this, E_Y);
    _sp2.SetPos(_curValueY*100);

    _lockNoty            = 0;
    

}

void DlbXyz::OnChangeZ() 
{
    if(_lockNoty) return;
    _lockNoty            = 1;

    _curValueZ = ParseREAL(this, E_Z);
    _sp3.SetPos(_curValueZ*100);

    _lockNoty            = 0;

}

void DlbXyz::OnDeltaposX(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_lockNoty) return;
    _lockNoty            = 1;

    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    int sp     = pNMUpDown->iPos;
    _curValueX = (float)sp/100.f;
    _e1.SetWindowText(MKSTR("%.2f",_curValueX));


	*pResult = 0;
    _lockNoty = 0;
}

void DlbXyz::OnDeltaposY(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_lockNoty) return;
    _lockNoty            = 1;

    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    int sp     = pNMUpDown->iPos;
    _curValueY = (float)sp/100.f;
    _e2.SetWindowText(MKSTR("%.2f",_curValueY));

    *pResult = 0;
    _lockNoty = 0;
}

void DlbXyz::OnDeltaposZ(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_lockNoty) return;
    _lockNoty            = 1;

    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    int sp     = pNMUpDown->iPos;
    _curValueZ = (float)sp/100.f;
    _e3.SetWindowText(MKSTR("%.2f",_curValueZ));

    *pResult = 0;
    _lockNoty = 0;
}

/////////////////////////////////////////////////////////////////////////////
// DlbUV dialog


DlbUV::DlbUV(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbUV::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbUV)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlbUV::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbUV)
	DDX_Control(pDX, IDC_SPIN1, _sp1);
    DDX_Control(pDX, IDC_SPIN2, _sp2);
	DDX_Control(pDX, E_U, _e1);
    DDX_Control(pDX, E_V, _e2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbUV, CLbDlg)
	//{{AFX_MSG_MAP(DlbUV)
	ON_EN_CHANGE(E_U, OnChangeU)
	ON_EN_CHANGE(E_V, OnChangeV)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbUV message handlers


void    DlbUV::OnDeselect()
{
}

void    DlbUV::OnSelect() 
{
}

void   DlbUV::Apply()
{
    _lockNoty = 1;

    _sp1.SetRange32(_spMinX * 100, _spMaxX * 100);
    _sp1.SetPos(_curValueX*100);
    _e1.SetWindowText(MKSTR("%.2f",_curValueX));

    _sp2.SetRange32(_spMinY * 100, _spMaxY * 100);
    _sp2.SetPos(_curValueY*100);
    _e2.SetWindowText(MKSTR("%.2f",_curValueY));

    _lockNoty = 0;
    CLbDlg::Apply();
}

void DlbUV::OnChangeU() 
{

    if(_lockNoty) return;
    _lockNoty            = 1;

    _curValueX = ParseREAL(this, E_U);
    _sp1.SetPos(_curValueX*100);

    _lockNoty            = 0;
	
}

void DlbUV::OnChangeV() 
{
    if(_lockNoty) return;
    _lockNoty            = 1;
    _curValueY = ParseREAL(this, E_V);
    _sp2.SetPos(_curValueY*100);

    _lockNoty            = 0;

}

void DlbUV::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_lockNoty) return;
    _lockNoty            = 1;

    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    int sp     = pNMUpDown->iPos;
    _curValueX = (float)sp/100.f;
    _e1.SetWindowText(MKSTR("%.2f",_curValueX));


	*pResult = 0;
    _lockNoty = 0;
}

void DlbUV::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_lockNoty) return;
    _lockNoty            = 1;

    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    int sp     = pNMUpDown->iPos;
    _curValueY = (float)sp/100.f;
    _e2.SetWindowText(MKSTR("%.2f",_curValueY));

    *pResult = 0;
    _lockNoty = 0;
}

/////////////////////////////////////////////////////////////////////////////
// DlbRANGE dialog


DlbRANGE::DlbRANGE(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbRANGE::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbRANGE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _lockNoty = FALSE;
}


void DlbRANGE::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbRANGE)
	DDX_Control(pDX, S_1, _s1);
	DDX_Control(pDX, E_1, _e1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbRANGE, CLbDlg)
	//{{AFX_MSG_MAP(DlbRANGE)
	ON_NOTIFY(NM_RELEASEDCAPTURE, S_1, OnReleasedcapture1)
	ON_EN_CHANGE(E_1, OnChange1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbRANGE message handlers

void    DlbRANGE::OnDeselect()
{
}

void    DlbRANGE::OnSelect() 
{
}

void   DlbRANGE::Apply()
{
    _lockNoty = 1;
    _s1.SetRange((int)_spMinX,(int)_spMaxX);
    _s1.SetPos((int)_curValueX);
    SetDlgItemInt(E_1, (int)_curValueX);
    CLbDlg::Apply();
    _lockNoty = 0;
}

void DlbRANGE::OnReleasedcapture1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(TRUE == _lockNoty )
        return;
    _lockNoty = 1;
    _curValueX = (REAL)_s1.GetPos();
    SetDlgItemInt(E_1, (int)_curValueX );
	*pResult = 0;
    _lockNoty = 0;
    Noty();
}

void DlbRANGE::OnChange1() 
{
    if(TRUE == _lockNoty )
        return;
    _lockNoty = 1;

    int rv = GetDlgItemInt(E_1);
    _curValueX = rv;
    _s1.SetPos((int)_curValueX);
    _lockNoty = 0;
    Noty();
}


/////////////////////////////////////////////////////////////////////////////
// DlbFILE dialog


DlbFILE::DlbFILE(CWnd* pParent /*=NULL*/)
: CLbDlg(DlbFILE::IDD, pParent),_bchanging(0)
{
	//{{AFX_DATA_INIT(DlbFILE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    
}


void DlbFILE::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbFILE)
	DDX_Control(pDX, IDC_BUTTON1, m_ob1);
	DDX_Control(pDX, E_1, _e1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbFILE, CLbDlg)
	//{{AFX_MSG_MAP(DlbFILE)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(E_1, OnChange1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbFILE message handlers

void    DlbFILE::OnDeselect()
{
}

void    DlbFILE::OnSelect() 
{
}


void   DlbFILE::Apply()
{
    _e1.SetWindowText(_szFile);
    CLbDlg::Apply();
}
void DlbFILE::OnButton1() 
{
    
    CFileDialog dlg(_bFileOpen, _fileExt, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _fileFilter);
    dlg.m_ofn.lpstrInitialDir = _initalPath;
    dlg.m_ofn.lpstrTitle = _T("Getic");
    if (theApp.DoFileDialog(dlg) != IDOK) return;   
    _bchanging = 1;
    if(_onlyFile)
    {
        PathHandler ph(dlg.m_ofn.lpstrFile);
        _szFile         = ph.File();
        _initalPath     = ph.Path();
        _e1.SetWindowText(ph.File());
        Noty();	
    }
    else
    {
        PathHandler ph(dlg.m_ofn.lpstrFile);
        _szFile         = dlg.m_ofn.lpstrFile;
        _initalPath     = ph.Path();
        _e1.SetWindowText(dlg.m_ofn.lpstrFile);
        Noty();	
    }
    _bchanging = 0;
    
}

void DlbFILE::OnChange1() 
{
    if(_bchanging)
        return;
    char fn[PATH_MAX];

    GetDlgItemText(E_1, fn, PATH_MAX);
    _szFile         = fn;
}


void DlbFILE::OnDestroy() 
{
	CLbDlg::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
// DlbEDSpin dialog


DlbEDSpin::DlbEDSpin(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbEDSpin::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbEDSpin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _lockNoty = FALSE;
}


void DlbEDSpin::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbEDSpin)
	DDX_Control(pDX, SP_1, _sp1);
	DDX_Control(pDX, E_1, _e1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbEDSpin, CLbDlg)
	//{{AFX_MSG_MAP(DlbEDSpin)
	ON_NOTIFY(UDN_DELTAPOS, SP_1, OnDeltapos1)
	ON_EN_CHANGE(E_1, OnChange1)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbEDSpin message handlers

void    DlbEDSpin::OnDeselect()
{
}

void    DlbEDSpin::OnSelect() 
{
}

void   DlbEDSpin::Apply()
{
    // tmax 2 de places
    
    _lockNoty = 1;
    _sp1.SetRange32(_spMinX * 100, _spMaxX * 100);
    _sp1.SetPos(_curValueX*100);
    _e1.SetWindowText(MKSTR("%.2f",_curValueX));
    _prevVal = _sp1.GetPos();
    _lockNoty = 0;
    CLbDlg::Apply();
} 

void DlbEDSpin::OnDeltapos1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_lockNoty) return;
    _lockNoty            = 1;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    
    int sp     = pNMUpDown->iPos;
    _curValueX = (float)sp/100.f;
    _e1.SetWindowText(MKSTR("%.2f",_curValueX));
	*pResult = 0;
    _lockNoty = 0;
}

void DlbEDSpin::OnChange1() 
{
    if(_lockNoty) return;
    _lockNoty = 1;
    _curValueX = ParseREAL(this, E_1);
    _sp1.SetPos(_curValueX*100);
    _lockNoty = 0;
}

void DlbEDSpin::OnVScroll(size_t nSBCode, size_t nPos, CScrollBar* pScrollBar) 
{
	
	CLbDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////////////////////////////////////
// DlbEDIT dialog


DlbEDIT::DlbEDIT(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbEDIT::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbEDIT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlbEDIT::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbEDIT)
	DDX_Control(pDX, E_1, _e1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbEDIT, CLbDlg)
	//{{AFX_MSG_MAP(DlbEDIT)
	ON_EN_CHANGE(E_1, OnChange1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbEDIT message handlers

void    DlbEDIT::OnDeselect()
{
}

void    DlbEDIT::OnSelect() 
{
}


void   DlbEDIT::Apply()
{
    CLbDlg::Apply();
}


/////////////////////////////////////////////////////////////////////////////
// DlbCLR dialog


DlbCLR::DlbCLR(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbCLR::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbCLR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlbCLR::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbCLR)
	DDX_Control(pDX, IDC_BUTTON2, m_bc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbCLR, CLbDlg)
	//{{AFX_MSG_MAP(DlbCLR)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbCLR message handlers

void    DlbCLR::OnDeselect()
{
}

void    DlbCLR::OnSelect() 
{
}

HBRUSH DlbCLR::OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor) 
{
	HBRUSH hbr = CLbDlg::OnCtlColor(pDC, pWnd, nCtlColor);
    if(nCtlColor == CTLCOLOR_STATIC )
    {
        pDC->SetBkColor(_color);
    }
	return hbr;
}

void DlbCLR::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    RECT rect; 
    GetClientRect(&rect);
    InflateRect(&rect,-1,-2);
    rect.left+=50;

    COLORREF hl = theApp._color[2];
    COLORREF sh = theApp._color[0];

    if(_brush.m_hObject)
        dc.FillRect(&rect,&_brush);
    
    SetDlgItemText(S_1, MKSTR("%d.%d.%d", GetRValue(_color),GetGValue(_color), GetBValue(_color)));
    
    dc.Draw3dRect(&rect, hl,sh);


}

void DlbCLR::OnButton2() 
{
	CColorDialog    d( _color, CC_FULLOPEN|CC_RGBINIT, AfxGetMainWnd());
    if(d.DoModal()==IDOK)
    {
        _color  = d.GetColor();
        _brush.DeleteObject();
        _brush.CreateSolidBrush(_color);
        Invalidate();
		Noty();
    }
}

void DlbCLR::OnDestroy() 
{
	CLbDlg::OnDestroy();
	_brush.DeleteObject();
    
}

BOOL DlbCLR::OnInitDialog() 
{
	CLbDlg::OnInitDialog();
 	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void   DlbCLR::Apply()
{
     _brush.DeleteObject();
    _brush.CreateSolidBrush(_color);
    CLbDlg::Apply();

}

/////////////////////////////////////////////////////////////////////////////
// DlbCKs dialog


DlbCKs::DlbCKs(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbCKs::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbCKs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlbCKs::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbCKs)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbCKs, CLbDlg)
	//{{AFX_MSG_MAP(DlbCKs)
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbCKs message handlers

BOOL DlbCKs::OnInitDialog() 
{
	CLbDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlbCKs::OnKillFocus(CWnd* pNewWnd) 
{
	CLbDlg::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}

void DlbCKs::OnDestroy() 
{
	CLbDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


void    DlbCKs::OnDeselect()
{
}

void    DlbCKs::OnSelect() 
{
}

void   DlbCKs::Apply()
{
    int iiD = CK_1;
    for(int i=0;i< 8;i++)
    {
        if(i<_ckNumVis)
            GetDlgItem(i+iiD)->ShowWindow(SW_SHOW);
        else
            GetDlgItem(i+iiD)->ShowWindow(SW_HIDE);
    }
    CLbDlg::Apply();

}

BOOL DlbCKs::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	BOOL b = CLbDlg::OnCommand(wParam, lParam);
    Noty();	
    return b;
}


DlbCB::DlbCB(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbCB::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbCB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlbCB::DoDataExchange(CDataExchange* pDX)
{
	CLbDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbCB)
	DDX_Control(pDX, IDC_COMBO1, _cb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbCB, CLbDlg)
	//{{AFX_MSG_MAP(DlbCB)
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlbCB message handlers

BOOL DlbCB::OnInitDialog() 
{
	CLbDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlbCB::OnKillFocus(CWnd* pNewWnd) 
{
	CLbDlg::OnKillFocus(pNewWnd);
}

void DlbCB::OnDestroy() 
{
	CLbDlg::OnDestroy();
}

void    DlbCB::OnDeselect()
{
}

void    DlbCB::OnSelect() 
{
}

void   DlbCB::Apply()
{
    CLbDlg::Apply();
}


void CPrpLb::OnSelchange() 
{
    CLbDlg*    pDlg ;
    if(_curSel>=0){
        pDlg = GetCurItemSel(_curSel);
        pDlg->_selected=FALSE;
        pDlg->Invalidate();

        // notify abount a click
        if(GetParent())
        {
            // lose sel
            GetParent()->PostMessage(WM_DLGCLICK, 0, (LONG)pDlg);
        }

    }
    _curSel = GetCurSel();
    if(_curSel>=0){
        pDlg = GetCurItemSel(_curSel);
        pDlg->_selected=TRUE;
        pDlg->Invalidate();
        if(GetParent())
        {
            // gain sel
            GetParent()->PostMessage(WM_DLGCLICK, 1, (LONG)pDlg);
        }
    }
    
}

LRESULT CPrpLb::OnSetCurSel(WPARAM w, LPARAM l)
{
    LRESULT lr = Default();

    OnSelchange() ;

    return lr;
}

BOOL CLbDlg::OnCmdMsg(size_t nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    CWnd* pWnd = GetParent();
	BOOL b = CBASEDLG::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
    pWnd->GetParent()->PostMessage(WM_DLGEVENT, nID, (LONG)this);
    return b;
}

void CPrpLb::OnSetfocus() 
{
    SeeRects();
}

void CPrpLb::OnKeyDown(size_t nChar, size_t nRepCnt, size_t nFlags) 
{
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
    SeeRects();
}

void DlbFILE::OnSize(size_t nType, int cx, int cy) 
{
	CLbDlg::OnSize(nType, cx, cy);
    if(GetDlgItem(E_1))
    {
        GetDlgItem(E_1)->MoveWindow(1,1,cx-40,cy-2);
        GetDlgItem(IDC_BUTTON1)->MoveWindow(cx-38,1,36,cy-2);
    }
	
}



void DlbCB::OnSelchangeCombo1() 
{
	Noty();	
}

void DlbEDIT::OnChange1() 
{
	Noty();	
}



DlbRB::DlbRB(CWnd* pParent /*=NULL*/)
	: CLbDlg(DlbRB::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlbRB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlbRB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlbRB)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlbRB, CDialog)
	//{{AFX_MSG_MAP(DlbRB)
	ON_BN_CLICKED(RB_33, On33)
	ON_BN_CLICKED(RB_22, On22)
	ON_BN_CLICKED(RB_11, On11)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void DlbRB::On33() 
{
	// TODO: Add your control notification handler code here
	
}

void DlbRB::On22() 
{
	// TODO: Add your control notification handler code here
	
}

void DlbRB::On11() 
{
	// TODO: Add your control notification handler code here
	
}

/*

void Dlg_ReportPick::_RecalcScrollPos(int starty, RECT& rtc, RECT& rt)
{
    // calc scrolling size 
    m_wndView.i_maxHeight = starty+rtc.bottom;
    m_wndView.i_curH = rtc.bottom;
    int cy = m_wndView.i_curH;
    m_wndView.i_nScrollPos = 0;

    if(starty+rtc.bottom > rt.bottom){
        int nScrollMax;
        nScrollMax = (m_wndView.i_maxHeight) - rt.bottom;
        SCROLLINFO si;
        si.cbSize  = sizeof(SCROLLINFO);
        si.fMask   = SIF_ALL; 
        si.nMin   = 0;
        si.nMax   = nScrollMax;
        si.nPage  = si.nMax/10;
        si.nPos   = 0;
        m_wndView.SetScrollInfo(SB_VERT, &si, TRUE); 
    }else{
        m_wndView.SetScrollRange(SB_VERT, 0, 0);
    }
}

void CChildView::OnVScroll(size_t nSBCode, size_t nPos, CScrollBar* pScrollBar) 
{
    int nDelta;
	int nMaxPos = i_maxHeight - i_curH;

	switch (nSBCode)
	{
	    case SB_LINEDOWN:
		    if (i_nScrollPos >= nMaxPos)
			    return;
		    nDelta = tmin(nMaxPos/100,nMaxPos-i_nScrollPos);
		    break;

	    case SB_LINEUP:
		    if (i_nScrollPos <= 0)
			    return;
		    nDelta = -tmin(nMaxPos/100,i_nScrollPos);
		    break;

        case SB_PAGEDOWN:
		    if (i_nScrollPos >= nMaxPos)
			    return;
		    nDelta = tmin(nMaxPos/10,nMaxPos-i_nScrollPos);
		    break;

	    case SB_THUMBPOSITION:
		    nDelta = (int)nPos - i_nScrollPos;
		    break;

	    case SB_PAGEUP:
		    if (i_nScrollPos <= 0)
			    return;
		    nDelta = -tmin(nMaxPos/10,i_nScrollPos);
		    break;
	    
        default:
		    return;
	}
	i_nScrollPos += nDelta;
	SetScrollPos(SB_VERT,i_nScrollPos,TRUE);
	ScrollWindow(0,-nDelta);
	CWnd ::OnVScroll(nSBCode, nPos, pScrollBar);
}


*/
