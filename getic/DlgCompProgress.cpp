// DlgCompProgress.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgCompProgress.h"
#include "MainFrm.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


char    CDlgCompProgress::_progTxt[128] = {0};
int      CDlgCompProgress::_progValue    = 0;
extern   HICON                          __icos[4];
extern  BOOL                            BbautoClose ;
//---------------------------------------------------------------------------------------
// CDlgCompProgress dialog
CDlgCompProgress::CDlgCompProgress(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgCompProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCompProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    n_timer = 0;
}

//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDlgCompProgress, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgCompProgress)
	ON_WM_SIZE()
	ON_BN_CLICKED(PB_CANCEL, OnCancel)
	ON_BN_CLICKED(PC_CLOSE, OnCloseBTN)
    ON_BN_CLICKED(IDC_CHECK1, OnCheckCheck)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_LBN_DBLCLK(IDC_LIST1, OnDblclkLB)
    //ON_MESSAGE(WM_INITDIALOG, HandleInitDialog2)
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
// CDlgCompProgress message handlers
BOOL CDlgCompProgress::OnInitDialog()
{
    BOOL br = CBASEDLG::OnInitDialog();
    _onScreen=TRUE;
    _repLB.SubclassDlgItem(IDC_LIST1,this);
	m_stp.SubclassDlgItem(IDC_PROGRESS1, this);
    m_stp.ShowWindow(SW_SHOW);
    m_stp.SetRange(0,100);
    m_stp.SetStep(1);
    m_b2.SubclassDlgItem(PB_CANCEL, this);    
    m_b1.SubclassDlgItem(PC_CLOSE, this);    
    CheckDlgButton(IDC_CHECK1, BbautoClose ? BST_CHECKED : BST_UNCHECKED);
    return br;
}


//---------------------------------------------------------------------------------------
// CDlgCompProgress message handlers
LRESULT CDlgCompProgress::HandleInitDialog2(WPARAM w, LPARAM l)
{
    _onScreen=TRUE;
    LRESULT lr =  HandleInitDialog(w,l);
    _repLB.SubclassDlgItem(IDC_LIST1,this);
	m_stp.SubclassDlgItem(IDC_PROGRESS1, this);
    m_stp.ShowWindow(SW_SHOW);
    m_stp.SetRange(0,100);
    m_stp.SetStep(1);
    m_b2.SubclassDlgItem(PB_CANCEL, this);    
    m_b1.SubclassDlgItem(PC_CLOSE, this);  
    CheckDlgButton(IDC_CHECK1, BbautoClose ? BST_CHECKED : BST_UNCHECKED);

    return  lr;
}


void CDlgCompProgress::OnSize(size_t nType, int cx, int cy) 
{
	CBASEDLG::OnSize(nType, cx, cy);

    if(GetDlgItem(IDC_LIST1) && GetDlgCtrlID() == DLG_COMPILERPROG)
    {
        RECT rc; 
        GetClientRect(&rc);

        GetDlgItem(IDC_LIST1)->MoveWindow(rc.left + 2,
                                          rc.top + 1,
                                          rc.right - 14,
                                          rc.bottom - 2, 1);

        m_stp.MoveWindow(rc.left   + 2,
                         rc.bottom - 14,
                         rc.right  - 14,
                         rc.bottom - 1 ,
                         1);


       // GetDlgItem(IDC_STATICL)->MoveWindow(rc.right-32, rc.top+18, 32, 32,1);

		GetDlgItem(PC_CLOSE)->MoveWindow(rc.right-13,  rc.top+1, 13, 13,1);
		GetDlgItem(PB_CANCEL)->MoveWindow(rc.right-13, rc.top+14, 13, 13,1);
    }   


}



void CDlgCompProgress::OnClear()
{
    _mapidPoint.clear();
	_repLB.ResetContent();
    DOC()->SetErrorPoint(V0);
}

void CDlgCompProgress::OnCancel() 
{
    _onScreen = 0;
    _mapidPoint.clear();
	_repLB.ResetContent();
    DOC()->SetErrorPoint(V0);
    DOC()->OnBucompile();
    Sleep(1000);
    OnCloseBTN();
}

void CDlgCompProgress::OnCheckCheck()
{
    BbautoClose = IsDlgButtonChecked(IDC_CHECK1);
}

void CDlgCompProgress::OnCloseBTN()
{
	_onScreen = FALSE;
    DOC()->_debugPolys.clear();
    _repLB.ResetContent();
    _mapidPoint.clear();
	_repLB.UnsubclassWindow();
    DOC()->SetErrorPoint(V0);    
    FRM()->ShowProgressDlg(FALSE);
}

void CDlgCompProgress::OnDestroy() 
{
    if(n_timer)
        KillTimer(n_timer);
    n_timer=0;
}


void    CDlgCompProgress::LogPos(size_t flag, V3& pos, const char* lpszFormat,...)
{
   if(!_onScreen || !::IsWindow(m_hWnd))
        return;

    va_list args;
	va_start(args, lpszFormat);

	int  nBuf;
	char szBuffer[512];
	nBuf = _vsnprintf(szBuffer, sizeof(szBuffer) / sizeof(char), lpszFormat, args);
	assert(nBuf < sizeof(szBuffer));
    int idx = 0;
    if(_onScreen)
        idx = GetDlgItem(IDC_LIST1)->SendMessage(LB_ADDSTRING, 0, (LPARAM)(szBuffer));
    if(idx==0)
        return;
    if(szBuffer[0]=='W')    
    {
        flag |= LOG_WARNING;
        _mapidPoint[idx] = pos;
    }
    else if(szBuffer[0]=='E') 
    {
        flag |= LOG_ERROR;
        _mapidPoint[idx] = pos;

    }
    if(_onScreen)
        GetDlgItem(IDC_LIST1)->SendMessage(LB_SETITEMDATA, idx, (LPARAM)(flag));
    if(_onScreen)
        GetDlgItem(IDC_LIST1)->SendMessage(LB_SETCURSEL, idx,0);
    va_end(args);
}

void    CDlgCompProgress::Log(size_t flag, const char* lpszFormat,...)
{
    if(!_onScreen || !::IsWindow(m_hWnd))
        return;

    va_list args;
	va_start(args, lpszFormat);

	int  nBuf;
	char szBuffer[512];
	nBuf = _vsnprintf(szBuffer, sizeof(szBuffer) / sizeof(char), lpszFormat, args);
	assert(nBuf < sizeof(szBuffer));
    
    int idx = GetDlgItem(IDC_LIST1)->SendMessage(LB_ADDSTRING, 0, (LPARAM)(szBuffer));
    // build the complex flag based on passed in paramater and text

    if(szBuffer[0]=='W')    
    {
        flag |= LOG_WARNING;
    }
    else if(szBuffer[0]=='E') 
    {
        flag |= LOG_ERROR;
    }
    if(::IsWindow(m_hWnd))
    {
        GetDlgItem(IDC_LIST1)->SendMessage(LB_SETITEMDATA, idx, (LPARAM)(flag));
        GetDlgItem(IDC_LIST1)->SendMessage(LB_SETCURSEL, idx,0);
        Progress(100);
    }
    va_end(args);
}

void CDlgCompProgress::OnDblclkLB() 
{
    int idx = 	_repLB.GetCurSel();
    int index = _repLB.GetItemData(idx);

    if(idx != 0x200000 && index != 0x200000 )
    {
        V3 pos = _mapidPoint[index];
    }
}

void    CDlgCompProgress::Progress(int pos)
{
    if(!_onScreen || !m_hWnd)
        return;
    if(pos==-1)
    {
        _progValue = 0;
        if(_onScreen)
            ::PostMessage(m_stp.m_hWnd, PBM_SETPOS, 0, 0L);
        return;
    }
    if(_progValue != pos)
    {
        _progValue = pos;
        if(_onScreen)
            ::PostMessage(m_stp.m_hWnd, PBM_SETPOS, pos, 0L);
    }
}



#ifndef _MEMDC_H_
//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//
// This class implements a memory Device Context

class CMemDC : public CDC
{
public:

    // constructor sets up the memory DC
    CMemDC(CDC* pDC) : CDC()
    {
        ASSERT(pDC != NULL);

        m_pDC = pDC;
        m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();
              
        if (m_bMemDC)    // Create a Memory DC
        {
            pDC->GetClipBox(&m_rect);
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
        else        // Make a copy of the relevent parts of the current DC for printing
        {
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
    
    // Destructor copies the contents of the mem DC to the original DC
    ~CMemDC()
    {
        if (m_bMemDC) 
        {    
            // Copy the offscreen bitmap onto the screen.
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this, m_rect.left, m_rect.top, SRCCOPY);

            //Swap back the original bitmap.
            SelectObject(m_pOldBitmap);
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.
            m_hDC = m_hAttribDC = NULL;
        }
    }

    // Allow usage as a pointer
    CMemDC* operator->() {return this;}
        
    // Allow usage as a pointer
    operator CMemDC*() {return this;}

private:
    CBitmap  m_bitmap;      // Offscreen bitmap
    CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
    CDC*     m_pDC;         // Saves CDC passed in constructor
    CRect    m_rect;        // Rectangle of drawing area.
    BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
};

#endif


