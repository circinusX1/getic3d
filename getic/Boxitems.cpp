//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z-edmap.h"
#include "BoxItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBoxItems::CBoxItems()
{
}

CBoxItems::~CBoxItems()
{
}


BEGIN_MESSAGE_MAP(CBoxItems, CComboBox)
//{{AFX_MSG_MAP(CBoxItems)
ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CBoxItems::OnCloseup()
{
    // TODO: Add your control notification handler code here
    
}
