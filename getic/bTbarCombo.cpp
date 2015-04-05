//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z-edmap.h"
#include "bTbarCombo.h"
#include "z_ed2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CbTbarCombo::CbTbarCombo()
{
}

CbTbarCombo::~CbTbarCombo()
{
    TRACE("~CbTbarCombo()");
}


BEGIN_MESSAGE_MAP(CbTbarCombo, CComboBox)
//{{AFX_MSG_MAP(CbTbarCombo)
ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CbTbarCombo::OnCloseup()
{
}



//--| CbTbarCombo::OnSelchange|-----------------------------------------------------------
void CbTbarCombo::OnSelchange()
{
    EL_SEL selm      = DOC()->_selmode;
    BR_MODE brmode    = DOC()->_brmode;

    DOC()->Escape(0);
    
    char str[32];
    this->GetLBText(this->GetCurSel(), str);
    int value = ::_ttoi(str);
    if(GetDlgCtrlID() == 1011)
    {
        if(value == -1)
        {
            DOC()->_curgroup  = value;       //viewable group
            DOC()->_curCreatGroup  = 0;

            DOC()->_selmode = selm;
            DOC()->_brmode = brmode;
            DOC()->Invalidate(1);
            return;
        }
    
        DOC()->_curgroup       = value;      //viewable group
        DOC()->_curCreatGroup  = value;
    }
    else
    {
        GUtex = this->GetItemData(this->GetCurSel());
    }
    DOC()->_selmode = selm;
    DOC()->_brmode = brmode;
    DOC()->Invalidate(1);
}
