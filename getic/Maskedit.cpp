// MaskEdit.cpp: implementation of the CMaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "MaskEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

WNDPROC CMaskEdit::_op;

void CMaskEdit::Init()
{
    WNDCLASS wc  = {0};

    static char pszClassName[] = "G_EDIT";
    GetClassInfo(NULL, "EDIT", (LPWNDCLASS)&wc);

    _iStdButtonWndExtra = wc.cbWndExtra;   // Save this in a global.
    wc.cbWndExtra    += 128;
    wc.lpszClassName = pszClassName;
    _op              = wc.lpfnWndProc;
    wc.lpfnWndProc   = WndProc;
    
   BOOL br =  RegisterClass((LPWNDCLASS)&wc);

}

CMaskEdit::~CMaskEdit()
{
    UnregisterClass("GETIC_EDIT",AfxGetApp()->m_hInstance);
}

LRESULT CALLBACK CMaskEdit::WndProc(HWND hWnd, size_t uMsg, WPARAM wP, LPARAM lP)
{
    return (CMaskEdit::_op)(hWnd, uMsg, wP, lP);

}




