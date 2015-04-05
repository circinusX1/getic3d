// DlgMotionState.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgMotionState.h"
#include "Motion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgMotionState dialog
DlgMotionState::DlgMotionState(MState* pMState)
	: CBASEDLG(DlgMotionState::IDD, 0),_pMotionState(pMState)
{
}

DlgMotionState::DlgMotionState(CWnd* pParent /*=NULL*/)
	: CBASEDLG(DlgMotionState::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgMotionState)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgMotionState::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgMotionState)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgMotionState, CBASEDLG)
	//{{AFX_MSG_MAP(DlgMotionState)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgMotionState message handlers


BOOL DlgMotionState::OnInitDialog() 
{
	CBASEDLG::OnInitDialog();
	
    Sct(this, EF_X,_pMotionState->_pos.x);
    Sct(this, EF_Y,_pMotionState->_pos.y);
    Sct(this, EF_Z,_pMotionState->_pos.z);
    Sct(this, EF_A, R2G(_pMotionState->_euler.x));
    Sct(this, EF_E, R2G(_pMotionState->_euler.y));
    Sct(this, EF_R, R2G(_pMotionState->_euler.z));
    Sct(this, EF_TIME,_pMotionState->_trTimes[0]);      // node time
    Sct(this, EF_TIME2,_pMotionState->_trTimes[1]);     // walk time to next node
	return TRUE;  
}

void DlgMotionState::OnDestroy() 
{
	CBASEDLG::OnDestroy();
}

void DlgMotionState::OnOk() 
{
    _pMotionState->_pos.x = ParseREAL(this, EF_X);
    _pMotionState->_pos.y = ParseREAL(this, EF_Y);
    _pMotionState->_pos.z = ParseREAL(this, EF_Z);
    _pMotionState->_euler.x = R2G(ParseREAL(this, EF_A));
    _pMotionState->_euler.y = R2G(ParseREAL(this, EF_E));
    _pMotionState->_euler.z = R2G(ParseREAL(this, EF_R));
    BLOCKRANGE(_pMotionState->_euler.x);
    BLOCKRANGE(_pMotionState->_euler.z);
    ROLLPI(_pMotionState->_euler.y);    


    _pMotionState->_trTimes[0] = ParseREAL(this, EF_TIME);
    _pMotionState->_trTimes[1] = ParseREAL(this, EF_TIME2);
    CBASEDLG::OnOK();
}

void DlgMotionState::OnCancel() 
{
	CBASEDLG::OnCancel();
}

BOOL DlgMotionState::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if(nCode == EN_CHANGE)
    {
        if(!GetDlgItem(IDOK)->IsWindowEnabled())
            GetDlgItem(IDOK)->EnableWindow(1);
    }
	return CBASEDLG::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
