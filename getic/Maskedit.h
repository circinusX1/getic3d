// MaskEdit.h: interface for the CMaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASKEDIT_H__2CA041FA_005A_4E42_8A89_F71E3C304A72__INCLUDED_)
#define AFX_MASKEDIT_H__2CA041FA_005A_4E42_8A89_F71E3C304A72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaskEdit  
{
public:
    CMaskEdit(){};
	virtual ~CMaskEdit();
    void Init();


    int         _iStdButtonWndExtra;

    static WNDPROC _op;
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP);
};

#endif // !defined(AFX_MASKEDIT_H__2CA041FA_005A_4E42_8A89_F71E3C304A72__INCLUDED_)
