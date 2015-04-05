// TreeScene.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "TreeScene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TreeScene

TreeScene::TreeScene()
{
}

TreeScene::~TreeScene()
{
}


BEGIN_MESSAGE_MAP(TreeScene, CTreeCtrl)
	//{{AFX_MSG_MAP(TreeScene)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TreeScene message handlers


void TreeScene::PreSubclassWindow() 
{
    i_ml.Create(BMP_SCENE,9,0,theApp._color[1]);
	CTreeCtrl::PreSubclassWindow();
    SetBkColor(theApp._color[1]);
    SetTextColor(RGB(0,0,0));	
    SetImageList(&i_ml,TVSIL_NORMAL);

}
