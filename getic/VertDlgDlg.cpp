// VertDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBar.h"
#include "VertDlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HICON	CVertDlgDlg::_handIco[2];
int		CVertDlgDlg::_instances = 0;


CVertDlgDlg::CVertDlgDlg(UINT iDD, CWnd* pParent /*=NULL*/)
	: CBASEDLG(iDD, pParent)
{
	//{{AFX_DATA_INIT(CVertDlgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVertDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVertDlgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVertDlgDlg, CBASEDLG)
	//{{AFX_MSG_MAP(CVertDlgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVertDlgDlg message handlers

BOOL CVertDlgDlg::OnInitDialog()
{
	CBASEDLG::OnInitDialog();

	if(CVertDlgDlg::_instances == 0)
	{
		CVertDlgDlg::_handIco[0] = AfxGetApp()->LoadCursor(CUR_HANDO);
		CVertDlgDlg::_handIco[1] = AfxGetApp()->LoadCursor(CUR_HANDC);
	}
	++CVertDlgDlg::_instances;
    _yScrollDn = 0;
    _scrDn = 0;    

    GetClientRect(&_wndRect);
	if(GetDlgItem(PAGE))
	{
		GetDlgItem(PAGE)->GetClientRect(&_wndRect);
        GetDlgItem(PAGE)->ShowWindow(SW_HIDE);
	}
    CreateGroups();
    Enable(0);
	return TRUE; 
}

void CVertDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CBASEDLG::OnSysCommand(nID, lParam);
}

void CVertDlgDlg::OnPaint() 
{
    CBASEDLG::OnPaint();
}

HCURSOR CVertDlgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVertDlgDlg::OnOk() 
{
	
}

void CVertDlgDlg::OnCancel() 
{
}

void CVertDlgDlg::OnDestroy() 
{
    vector<Group*>::iterator b = _groups.begin();
    vector<Group*>::iterator e = _groups.end();
    for(; b!=e; ++b)
    {
        Group* pG = (*b);
        pG->_buton.DestroyWindow();
        delete pG;
    }
	CBASEDLG::OnDestroy();
	--CVertDlgDlg::_instances;
	if(0== CVertDlgDlg::_instances)
	{
		::DestroyCursor(CVertDlgDlg::_handIco[0]);
		::DestroyCursor(CVertDlgDlg::_handIco[1]);
	}
	
}

void CVertDlgDlg::CreateGroups()
{
    char   wt[128];
    char   wc[128];

    CRect   rCtrl;
    CWnd*   pWnd = this;
    int     relation = GW_CHILD|GW_HWNDFIRST;
    while(pWnd = pWnd->GetWindow(relation))
    {
        UINT iD = pWnd->GetDlgCtrlID();

        pWnd->GetWindowText(wt, 128);
        ::GetClassName(pWnd->m_hWnd, wc, 128);
        ::CharUpper(wc);
        if(!_tcscmp(wc,"BUTTON") && (wt[0]=='-' || wt[0]=='+'))
        {
            Group* pG = new Group();

            pG->_prevSt = wt[0];
            pWnd->GetWindowRect(&pG->_grect);
            ScreenToClient(&pG->_grect);
            pG->_pCtl = pWnd;

            _groups.push_back(pG);
        }
        relation = GW_HWNDNEXT;
    }

    pWnd = this;
    relation = GW_CHILD|GW_HWNDFIRST;
    while(pWnd = pWnd->GetWindow(relation))
    {
		relation = GW_HWNDNEXT;
        UINT iD = pWnd->GetDlgCtrlID();
        pWnd->GetWindowText(wt, 128);
        if(wt[0]=='-' || wt[0]=='+')
		{
            continue;
		}

        pWnd->GetWindowRect(&rCtrl);
        ScreenToClient(&rCtrl);
        CPoint pt(rCtrl.left, rCtrl.top);
        
        vector<Group*>::iterator b = _groups.begin();
        vector<Group*>::iterator e = _groups.end();
        for(; b!=e; ++b)
        {
            Group* pG = (*b);

            if(pG->_grect.PtInRect(pt))
            {
                rCtrl.top    -= pG->_grect.top; //rel to group
                rCtrl.bottom -= pG->_grect.top;

                pG->_wnds.push_back(pWnd->m_hWnd);
                pG->_rwnds.push_back(rCtrl);
                break;
            }
        }
    }

    CRect rtn;
    vector<Group*>::iterator b = _groups.begin();
    vector<Group*>::iterator e = _groups.end();
    for(; b!=e; ++b)
    {
        Group* pG = (*b);
        UINT id = pG->_pCtl->GetDlgCtrlID();
        rtn = pG->_grect;
        rtn.bottom = rtn.top+18;
        pG->_pCtl->GetWindowText(wt, 128);
        pG->_pCtl->DestroyWindow();
        pG->_buton.Create(wt,
                          WS_VISIBLE| BS_AUTOCHECKBOX | BS_LEFT | BS_PUSHLIKE | BS_FLAT|BS_OWNERDRAW,
                          rtn, this, id);
        pG->_buton.SetFont(GetFont());
        pG->_cmdID = id;
        pG->_pCtl = &pG->_buton;
    }
    UpdateControls();
}


void CVertDlgDlg::UpdateControls()
{
    if(_groups.size()== 0)
        return;

    char   wt[128];
    int     is    = BARDLG()->GetScrollPos(SB_VERT);
    int     yView = _groups[0]->_grect.top;// - is;//_yScrollDn;
    int     viewHeight = 0;
    int     psp = _yScrollDn;   

    ScrollWindow(0,-_yScrollDn);

    CRect rt;
    vector<Group*>::iterator b = _groups.begin();
    vector<Group*>::iterator e = _groups.end();
    for(; b!=e; ++b)
    {
        Group* pG = (*b);

        pG->_pCtl->GetWindowText(wt, 128);

        pG->_pCtl->MoveWindow(pG->_grect.left, yView, pG->_grect.right-pG->_grect.left, 18);

        if(wt[0]=='-')      // expanded
        {
            vector<HWND>::iterator  wb  = pG->_wnds.begin();
            vector<HWND>::iterator  we  = pG->_wnds.end();
            vector<CRect>::iterator  rb  = pG->_rwnds.begin();
            for(; wb!=we; wb++, rb++)
            {
                ::MoveWindow((*wb),  
                             rb->left, 
                             rb->top + yView,
                             rb->right-rb->left, 
                             rb->bottom-rb->top , 1);

                ::ShowWindow((*wb), SW_SHOW); // late couse holds the old ctrl hape if any in new position
            }
            yView+=(pG->_grect.bottom -pG->_grect.top);

            if(pG->_prevSt != wt[0])
            {
                //expanded
            }
        }
        else if(wt[0]=='+') // compressed
        {
            vector<HWND>::iterator  wb  = pG->_wnds.begin();
            vector<HWND>::iterator  we  = pG->_wnds.end();
            vector<CRect>::iterator  rb  = pG->_rwnds.begin();
            for(; wb!=we; wb++, rb++)
            {
                ::ShowWindow((*wb), SW_HIDE);
            }
            yView+=18;

            if(pG->_prevSt != wt[0])
            {
                //collapsed
            }

        }    
        pG->_prevSt = wt[0]; 
    }
    viewHeight = yView - _yScrollDn;
    _viewportY = viewHeight;

	if(_viewportY <= _wndRect.bottom )    
	{
        _yScrollDn = 0;
        ScrollWindow(0,_yScrollDn);
	}
    else
    {
        ScrollWindow(0,psp);
    }
    
	Invalidate();
}

void    CVertDlgDlg::Colapse(UINT group)
{
    CString cs;
    GetDlgItem(group)->GetWindowText(cs);
    if(cs[0]=='-')
        OnCommand(group, 0) ;
}

void    CVertDlgDlg::EnableGroup(UINT group, BOOL en)
{
    vector<Group*>::iterator b = _groups.begin();
    vector<Group*>::iterator e = _groups.end();
    for(; b!=e; ++b)
    {
        Group* pG = (*b);
        if(pG->_cmdID==group)
        {
            GetDlgItem(group)->EnableWindow(en);

            vector<HWND>::iterator  wb  = pG->_wnds.begin();
            vector<HWND>::iterator  we  = pG->_wnds.end();
            for(; wb!=we; wb++)
            {
                ::EnableWindow((*wb), en);
            }
            /*
            if(en==0)
            {
                Colapse(group);
            }
            */

            break;
        }
    }
}

void    CVertDlgDlg::Enable(BOOL ben)
{
    vector<Group*>::iterator b = _groups.begin();
    vector<Group*>::iterator e = _groups.end();
    for(; b!=e; ++b)
    {
        Group* pG = (*b);
        GetDlgItem(pG->_cmdID)->EnableWindow(ben);
        //if(ben==0)
            //Colapse(pG->_cmdID);
    }

}


BOOL CVertDlgDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    BOOL    inval=FALSE;
    vector<Group*>::iterator b = _groups.begin();
    vector<Group*>::iterator e = _groups.end();
    for(; b!=e; ++b)
    {
        Group* pG = (*b);
        if(pG->_cmdID == wParam)
        {
            char   wt[128];
            pG->_pCtl->GetWindowText(wt, 128);
            BOOL ckstate = IsDlgButtonChecked(wParam);
            if(/*ckstate && */wt[0]=='+')
            {
                wt[0]='-';
                pG->_pCtl->SetWindowText(wt);
                UpdateControls();
                inval=1;
                OnExpanded();
                break;
            }
            else if (/*!ckstate && */wt[0]=='-')
            {
                wt[0]='+';
                pG->_pCtl->SetWindowText(wt);
                UpdateControls();
                inval=1;
                break;
            }
        }
    }
    if(inval)
        Invalidate();
	return CBASEDLG::OnCommand(wParam, lParam);
}

void CVertDlgDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if(_viewportY > _wndRect.bottom )    
    {
        if(GetCapture()!=this)
            SetCapture();
        _scrDn = 1;
		SetCursor(CVertDlgDlg::_handIco[_scrDn]);
        _ptDn = point;
    }
	CBASEDLG::OnLButtonDown(nFlags, point);
    _scrooled = 0;
}

void CVertDlgDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if(GetCapture()==this)
        ReleaseCapture();
	SetCursor(CVertDlgDlg::_handIco[0]);
    if(_scrDn && _scrooled)
    {
        Invalidate();
    }
    _scrDn = 0;
    _scrooled = 0;
    CBASEDLG::OnLButtonUp(nFlags, point);
}

void CVertDlgDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(_scrDn)
    {
        CRect toInval = _wndRect;
        int deltay = point.y-_ptDn.y;
        _ptDn.y = point.y;

        int prevPos = _yScrollDn;

        _yScrollDn += deltay;
        if(_yScrollDn < -(_viewportY-_wndRect.bottom))
            _yScrollDn = -(_viewportY-_wndRect.bottom);
        else if(_yScrollDn > 0)
            _yScrollDn = 0;

        int toScrollWith = _yScrollDn - prevPos;
        ScrollWindow(0,toScrollWith);
        _scrooled = 1;
    }
	CBASEDLG::OnMouseMove(nFlags, point);
}

void CVertDlgDlg::OnKillFocus(CWnd* pNewWnd) 
{
	
}

BOOL CVertDlgDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(_viewportY > _wndRect.bottom )    
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		CRect rt;
		GetClientRect(&rt);
		if(pt.x > rt.right-12)
		{
			SetCursor(CVertDlgDlg::_handIco[_scrDn]);
			return 1;
		}
		
	}
	return CBASEDLG::OnSetCursor(pWnd, nHitTest, message);
}

HBRUSH CVertDlgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBASEDLG::OnCtlColor(pDC, pWnd, nCtlColor);


	return hbr;
}

void    CVertDlgDlg::Reset()
{
    char wt[256];
    char wc[256];
    CWnd* pWnd=this;

    int     relation = GW_CHILD|GW_HWNDFIRST;
    while(pWnd = pWnd->GetWindow(relation))
    {
        UINT iD = pWnd->GetDlgCtrlID();
        pWnd->GetWindowText(wt, 128);
        ::GetClassName(pWnd->m_hWnd, wc, 128);
        ::CharUpper(wc);
        if(!_tcscmp(wc,"BUTTON"))
        {
            if(wt[0]=='-' || wt[0]=='+')
                continue;
            CheckDlgButton(iD,BST_UNCHECKED);
        }
        if(!stricmp(wc,"STATIC"))
            continue;
        if(!stricmp(wc,"EDIT"))
        {
            SetWindowText("");
        }
        if(!stricmp(wc,"LISTBOX"))
        {
            ((CListBox*)pWnd)->SetCurSel(0);
        }
        if(!stricmp(wc,"COMBOBOX"))
        {
            ((CComboBox*)pWnd)->SetCurSel(0);
        }
        relation = GW_HWNDNEXT;
    }

}

void CVertDlgDlg::OnExpanded()
{

}


