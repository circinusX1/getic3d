// ProgressDlg.cpp: implementation of the ProgressDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "ProgressDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgressDlg 

ProgressDlg	ProgressDlg::GWaitDlg;
UINT		ThreadProc(LPVOID pVoid);


//////////////////////////////////////////////////////////////////////////////
HICON   __icos[4]={0};
HANDLE	ProgressDlg ::event;
HWND	ProgressDlg ::hDialog;
BOOL	ProgressDlg::_visible;
DWORD	ProgressDlg::timeStart;
UINT	DlgID;

BOOL DialogProc(HWND hw,UINT msg,WPARAM w,LPARAM l)
{
    static HWND hEdit;
	static int	msgs = 0;
    static int  icoidx=0;
    
	switch(msg)
	{
		case WM_INITDIALOG:
#ifdef _FREE
            ::SetWindowPos(hw, HWND_TOPMOST,0,0,0,0,SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
#endif //
            ::SetTimer(hw, 369, 60, 0);
			ProgressDlg::_visible=FALSE;
			ProgressDlg::timeStart=GetTickCount();
			ProgressDlg ::hDialog = hw;
			SetEvent (ProgressDlg ::event);
            hEdit = ::GetDlgItem(hw,EF_OUT);
			msgs = 0;
			return FALSE;
        case WM_TIMER:
            ::SendDlgItemMessage(hw, ST_ANI_ICON, STM_SETIMAGE, 
                                IMAGE_ICON, (LPARAM)((HICON)__icos[++icoidx%4]));
            return TRUE;
		case WM_SETTEXT:
			if(w==-2)
			{
				ProgressDlg::_visible=TRUE;
			}
			::SendMessage(hEdit,WM_SETTEXT,0,l);
			return TRUE;
        case WM_CTLCOLORSTATIC:
    		if(DlgID != DLG_WAIT)
            {
                ::SetBkMode((HDC)w, TRANSPARENT);
                ::SetTextColor((HDC)w,RGB(200,0,64));// theApp._txtcolor);
                return (BOOL)(HBRUSH)theApp._slpEditBrush;
            }
            break;
		case WM_DESTROY:
            ::KillTimer(hw, 369);
			ProgressDlg ::hDialog = 0;
			SetEvent (ProgressDlg ::event);
			return TRUE;
	}
	return FALSE;
}

void ProgressDlg::Show(int action, LPCSTR text, UINT nDlgID)
{
	
	if(action == -1 && hDialog)
	{
		EndDialog(hDialog,0);
		WaitForSingleObject(event,5000);
		m_reffs = 0;
		return;
	}

	if(action == 1)
	{
		DWORD dw;
		if(hDialog == 0)
		{
			_tcscpy(tstring,text);	
			DlgID = nDlgID;
			CloseHandle(::CreateThread(0,0,
						(LPTHREAD_START_ROUTINE)ThreadProc,
						(LPVOID)0,0,&dw));
			WaitForSingleObject(event,5000);

			if(hDialog)//PRIORITYMAX
			{
				::BringWindowToTop(hDialog);
				::ShowWindow(hDialog,SW_SHOW);
			}
		}
		else
			action = 2;
		m_reffs++;
	}
	if(!hDialog)
		return;

	if(action == 0)
	{
		if(m_reffs)
			if(--m_reffs == 0 && hDialog)
			{
				EndDialog(hDialog,0);
				WaitForSingleObject(event,5000);
			}
	}

	if(action == 2) //only text comes
	{
		_tcscpy(tstring,text);	
	}
	if(hDialog)
	{
		SendMessage(hDialog,WM_SETTEXT,-1,(LPARAM)tstring);
	}
    theApp.PumpMessage(4);

}


UINT	ThreadProc(LPVOID pVoid)
{

    if(ProgressDlg::hDialog == NULL)
	{
		DialogBox(AfxGetResourceHandle(),
							MAKEINTRESOURCE(DlgID),
							NULL,(DLGPROC)DialogProc);
	}
	return 1;

}