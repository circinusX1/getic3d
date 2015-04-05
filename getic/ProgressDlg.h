// ProgressDlg.h: interface for the ProgressDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PROGRESS_DLG___
#define __PROGRESS_DLG___
#pragma warning (disable: 4786)
#include "geticgui.h"

class ProgressDlg  
{
public:
	ProgressDlg ()
	{
		hDialog = 0;
		event = CreateEvent(0,0,0,0);
	}
	~ProgressDlg ()
	{
		CloseHandle(event);
	}

	typedef UINT (*THREADPROC)(LPVOID pVoid);
	
		
// ITheDlg
public:
	void	Show(int action, LPCSTR text, UINT nDlgID = DLG_WAIT);

	static	BOOL		_visible;
	static	DWORD		timeStart;
	static	HWND		hDialog;
	static	HANDLE		event;
	char				tstring[128];
	int					m_reffs;
	CRITICAL_SECTION	cs;

	static	ProgressDlg	GWaitDlg;
};

#define	START_SPLASH()		ProgressDlg::GWaitDlg.Show(1,"",DLG_SPLASH)
#define	START_WAIT(text_)	ProgressDlg::GWaitDlg.Show(1,text_)
#define	KEEP_WAIT(text_)	ProgressDlg::GWaitDlg.Show(2,text_)
#define	END_WAIT(text_)		ProgressDlg::GWaitDlg.Show(0,text_)



#endif // !__PROGRESS_DLG___
