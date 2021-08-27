// VTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "VTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CVTabCtrl::_registered =  FALSE;
int  CVTabCtrl::_instances = 0;
HCURSOR  CVTabCtrl::_hHandCursor;

CVTabCtrl::CVTabCtrl()
{
}

CVTabCtrl::~CVTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CVTabCtrl, CWnd)
	//{{AFX_MSG_MAP(CVTabCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CVTabCtrl::Create(size_t ID, CWnd* parent)
{
    if(0 == CVTabCtrl::_registered)
    {
        WNDCLASS wndCLS;
        ::memset(&wndCLS, 0, sizeof(WNDCLASS));   // start with NULL defaults
        wndCLS.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndCLS.lpfnWndProc   = ::DefWindowProc;
        wndCLS.hInstance     = AfxGetInstanceHandle();
        wndCLS.hIcon         = 0;
        wndCLS.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
        wndCLS.lpszMenuName  = 0;
        wndCLS.hCursor       = ::LoadCursor(0,IDC_ARROW);
        wndCLS.lpszClassName = "_TAB_V_";
        CVTabCtrl::_registered = AfxRegisterClass(&wndCLS);

        CVTabCtrl::_hHandCursor = ::LoadCursor(0,IDC_HAND);
    }

    RECT rt={0,0,1,1};
    parent->GetDlgItem(ID)->GetWindowRect(&rt);
    parent->ScreenToClient(&rt);
    BOOL b = CWnd::Create("_TAB_V_", "", WS_CHILD, rt, parent, 9000);
    if(b)
    {
        SetFont(parent->GetFont());
        GetClientRect(&_wndRect);
        GetClientRect(&_scrlInval);
        _scrlInval.left = _scrlInval.right-10;
        ShowWindow(SW_SHOW);
    }
    _tmpCreaY = 0;
    _yScrollDn = 0;
    _scrDn = 0;    
    ++CVTabCtrl::_instances;
    return b;
}

void CVTabCtrl::AddChildren(CDialog* pDlg, size_t ID, const char* pTtitle)
{
    CRect rw; 
    CRect rt; 
    GetClientRect(&rw);

    int hb = GetSystemMetrics(SM_CYCAPTION);
    
    if(pDlg->Create(ID, this))
    {
        TCHAR wn[128] = {0};

        if(pTtitle == 0)
            pDlg->GetWindowText(wn,128);
        else
            strcpy(wn, pTtitle);
        pDlg->ModifyStyle(WS_CAPTION|WS_VISIBLE|DS_MODALFRAME|WS_SYSMENU|DS_MODALFRAME, WS_CHILD|DS_CONTROL);
        pDlg->SetParent(this);

        SDlg* psdlg = new SDlg();

        psdlg->_pD    = pDlg;
        psdlg->_pD->GetClientRect(&psdlg->_rdlg);
        psdlg->_rdlg.right =  min(psdlg->_rdlg.right, _wndRect.right-10);
        psdlg->_expnd = FALSE;
        psdlg->_name  = "- ";
        psdlg->_name  += wn;
        pDlg->ShowWindow(SW_HIDE);
        CRect rt(0, this->_tmpCreaY, rw.right-10, hb+this->_tmpCreaY);

        psdlg->_but.Create(psdlg->_name.c_str(),
                WS_VISIBLE| BS_AUTOCHECKBOX | BS_LEFT | BS_PUSHLIKE | BS_FLAT | WS_TABSTOP,
                rt, this, this->_dialogs.size()+1000);
        psdlg->_but.SetParent(this);
        psdlg->_but.ShowWindow(SW_SHOW);
        psdlg->_but.SetFont(GetParent()->GetFont());
        psdlg->_but.GetClientRect(&psdlg->_rbtn);
        this->_dialogs.push_back(psdlg);
        this->_tmpCreaY+=hb+1;
    }

}

BOOL CVTabCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    TCHAR wt[128];

    if(wParam >= 1000 && wParam < this->_dialogs.size()+1000)	
    {
        int index = wParam-1000;

        SDlg* psdlg = this->_dialogs[index];
        if(psdlg)
        {
            if(psdlg->_expnd == FALSE && IsDlgButtonChecked(wParam))
            {
                psdlg->_expnd = TRUE; 
                GetDlgItemText(wParam,wt,128);
                if(wt[0]=='-')
                    wt[0]='+';
                SetDlgItemText(wParam,wt);

            }
            else
            if(psdlg->_expnd == TRUE && !IsDlgButtonChecked(wParam))
            {
                psdlg->_expnd = FALSE;
                GetDlgItemText(wParam,wt,128);
                if(wt[0]=='+')
                    wt[0]='-';
                SetDlgItemText(wParam,wt);

            }
        }
        Reposition(1);        
    }
	return CWnd::OnCommand(wParam, lParam);
}


void    CVTabCtrl::Reposition(int mRestore)
{
    int                         yView = _yScrollDn;
    int                         viewHeight = 0;
    vector<SDlg*>::iterator b = _dialogs.begin();
    vector<SDlg*>::iterator e = _dialogs.end();

    for(;b!=e;b++)
    {
        SDlg* pD = (*b);

        pD->_but.MoveWindow(0,yView, pD->_rbtn.right, pD->_rbtn.bottom,1);
        yView += pD->_rbtn.bottom+2;

        if(pD->_expnd)
        {
            pD->_pD->ShowWindow(SW_SHOW);
            pD->_pD->MoveWindow(0,yView, pD->_rdlg.right, pD->_rdlg.bottom,1);
            yView += pD->_rdlg.bottom+2;
        }
        else
        {
            pD->_pD->ShowWindow(SW_HIDE);
        }
    }
    viewHeight = yView - _yScrollDn;
    if(mRestore)
        RecalcScroll(viewHeight);
}


void    CVTabCtrl::RecalcScroll(int yView)
{
    _viewportY = yView;
}

void CVTabCtrl::OnLButtonDown(size_t nFlags, CPoint point) 
{
    if(_viewportY > _wndRect.bottom && _scrlInval.PtInRect(point))    
    {
        if(GetCapture()!=this)
            SetCapture();
        _scrDn = 1;
        _ptDn = point;
    }
	CWnd::OnLButtonDown(nFlags, point);
}

void CVTabCtrl::OnLButtonUp(size_t nFlags, CPoint point) 
{
    if(GetCapture()==this)
        ReleaseCapture();
    if(_scrDn)
    {
        Invalidate();
    }
    _scrDn = 0;
	CWnd::OnLButtonUp(nFlags, point);
}

void CVTabCtrl::OnMouseMove(size_t nFlags, CPoint point) 
{
    if(_scrDn)
    {
        CRect toInval = _wndRect;

        int deltay = point.y-_ptDn.y;
        _ptDn.y = point.y;

        _yScrollDn += deltay;
        if(_yScrollDn < -(_viewportY-_wndRect.bottom))
            _yScrollDn = -(_viewportY-_wndRect.bottom);
        else if(_yScrollDn > 0)
            _yScrollDn = 0;
        Reposition();
/*
        if(deltay > 0) // scrdn (view up)
        {
            toInval.bottom = toInval.top + deltay + 1;
        }
        else if(deltay < 0)
        {
            toInval.top = toInval.bottom + deltay - 1;
        }
        InvalidateRect(&toInval);
*/
    }
	CWnd::OnMouseMove(nFlags, point);
}

void CVTabCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
    --CVTabCtrl::_instances;	

    vector<SDlg*>::iterator b = _dialogs.begin();
    vector<SDlg*>::iterator e = _dialogs.end();

    for(;b!=e;b++)
    {
        SDlg* pD = (*b);

        pD->_but.DestroyWindow();
        pD->_pD->DestroyWindow();
        delete pD;
    }

    if(0 == CVTabCtrl::_instances)
    {
        DestroyCursor(CVTabCtrl::_hHandCursor);
    }
	
}

BOOL CVTabCtrl::OnSetCursor(CWnd* pWnd, size_t nHitTest, size_t message) 
{
    CPoint pt; 
    GetCursorPos(&pt);
    ScreenToClient(&pt);
    if(_viewportY > _wndRect.bottom && _scrlInval.PtInRect(pt))
    {
        SetCursor(CVTabCtrl::_hHandCursor);
        return TRUE;
    }
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
