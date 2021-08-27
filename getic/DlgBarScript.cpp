// DlgBarScript.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "geticgui.h"
#include "DlgBarScript.h"
#include "z_ed2Doc.h"
#include "ToolFrame.h"
#include "CCrystalTextView.h"
#include "CCrystalTextBuffer.h"
#include "SampleDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// DlgBarScript dialog


DlgBarScript::DlgBarScript(CWnd* pParent /*=NULL*/)
	: CDialogBar(),_bLockUpdate(TRUE),_hSelected(0)
{
	//{{AFX_DATA_INIT(DlgBarScript)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgBarScript::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgBarScript)
	DDX_Control(pDX, LB_EVENTS, _lbEvents);
	DDX_Control(pDX, TR_SCENE, _scTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgBarScript, CDialogBar)
    
	//{{AFX_MSG_MAP(DlgBarScript)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_NOTIFY(TVN_SELCHANGED, TR_SCENE, OnSelchangedScene)
	ON_NOTIFY(TVN_SELCHANGING, TR_SCENE, OnSelchangingScene)
	ON_NOTIFY(TVN_BEGINLABELEDIT, TR_SCENE, OnBeginlabeleditScene)
	ON_NOTIFY(TVN_ENDLABELEDIT, TR_SCENE, OnEndlabeleditScene)
	ON_NOTIFY(NM_RETURN, TR_SCENE, OnReturnScene)
	ON_BN_CLICKED(PB_CANCEL, OnCancel)
	ON_BN_CLICKED(PB_OK, OnOk)
	ON_WM_GETDLGCODE()
	ON_LBN_DBLCLK(LB_EVENTS, OnDblclkEvents)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_INITDIALOG, HandleInitDialog2)
END_MESSAGE_MAP()


// DlgBarScript message handlers

BOOL DlgBarScript::OnInitDialog() 
{
	return TRUE;  
	              
}

LRESULT DlgBarScript::HandleInitDialog2(WPARAM w, LPARAM l)
{
    LRESULT lr =  HandleInitDialog(w,l);
    _scTab.SubclassDlgItem(TR_SCENE,this);
	_lbEvents.SubclassDlgItem(LB_EVENTS,this);

    _scTab.SetBkColor(theApp._color[1]);
    //_treeImg.Create();

    PopulateScenTabCtrl();

    _bLockUpdate=FALSE;
    return  lr;
}


void DlgBarScript::OnDestroy() 
{
	ClearListBox();
	CDialogBar::OnDestroy();
}


void DlgBarScript::OnCancel() 
{
}

void DlgBarScript::OnOk() 
{
	
}


void DlgBarScript::PopulateScenTabCtrl()
{
    // matches the item enumeration.'-'means disabled

    const char *names[33] = {"-",
                            "Lights",     
                            "Triggers",     
                            "-Brushes",
                            "-ITM_GAP51", 
                            "-ITM_GAP61",
	                        "-ITM_GAP71",
                            "-ITM_GAP81",
                            "-ITM_GAP91",
                            "-ITM_GAP10",
                            "-ITM_GAP11",
                            "-ITM_GAP12",
                            "-ITM_GAP13",
                            "-ITM_GAP14",
                            "-ITM_GAP15",
                            "-ITM_GAP16",
                            "-ITM_GAP17",
                            "-ITM_GAP18",
                            "-ITM_GAP19",
                            "-ITM_GAP20",
                            "-ITM_GAP21",
                            "-ITM_GAP22",
                            "-ITM_GAP23",
                            "-ITM_GAP24",
                            "-ITM_GAP25",
                            "-ITM_GAP26",
                            "-ITM_GAP27",
                            "-ITM_GAP28",
                            "-ITM_GAP29",
                            "-ITM_GAP30",	  
                            "Game","-"};

    //_scTab
    Scene&   scene     = DOC()->_scene;
	CString	 sceneTitl = DOC()->GetTitle();
	char	 szt[PATH_MAX];

	strcpy(szt, sceneTitl);
	PathHandler	ph(szt);

    
    HTREEITEM htrR = _scTab.GetRootItem();
    HTREEITEM htrS = _scTab.InsertItem(ph.File(),htrR);
	_scTab.SetItemData(htrS, (size_t) &DOC()->_scene);	//scene//Scene
	

    HTREEITEM htrCat[32] = {0};  

    HTREEITEM htrI = _scTab.InsertItem("Items",htrS);
    for(int i=0; i < 32; i++)
    {
        if(names[i][0]!='-')
        {
            htrCat[i] = _scTab.InsertItem(names[i],htrI);
        }
    }
}

void DlgBarScript::OnSelchangedScene(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_bLockUpdate)
        return;

    _pCurItem = 0;
    _pCurBrush = 0;
    _pCurPoly = 0;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    CString     in           = _scTab.GetItemText(pNMTreeView->itemNew.hItem);
    if(in[0]=='I')
    {
        _pCurItem = (SceItem*)_scTab.GetItemData(pNMTreeView->itemNew.hItem);
        ClearListBox();
        if(_pCurItem)
        {
            ShowOptionsForScript();
            _hSelected = pNMTreeView->itemNew.hItem;
        }
    }else
    if(in[0]=='B')
    {
        _pCurBrush = (Brush*)_scTab.GetItemData(pNMTreeView->itemNew.hItem);
        ClearListBox();
        if(_pCurBrush)
        {
            ShowOptionsForScript();
            _hSelected = pNMTreeView->itemNew.hItem;
        }
    }

	*pResult = 0;
}

void DlgBarScript::OnSelchangingScene(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_bLockUpdate)
        return;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	_pToolFrame->CanChangeItem();
	*pResult = 0;
}

void DlgBarScript::OnBeginlabeleditScene(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_bLockUpdate)
        return;

	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    *pResult = 0;
    if(_pCurItem==0 || _pCurBrush == 0 )
        *pResult = 1;
}

void DlgBarScript::OnEndlabeleditScene(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(_bLockUpdate)
        return;
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    
    if(_pCurItem)
    {
        CString& cs =  _scTab.GetItemText(pTVDispInfo->item.hItem);
        strcpy(_pCurItem->_name, cs);
    }

    if(_pCurBrush)
    {
        CString& cs =  _scTab.GetItemText(pTVDispInfo->item.hItem);
        strcpy(_pCurBrush->_name, cs);
    }


    if(_pCurPoly)
    {
    }

	*pResult = 0;
}

void DlgBarScript::OnReturnScene(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

BOOL DlgBarScript::PreTranslateMessage(MSG* pMsg) 
{
	return CDialogBar::PreTranslateMessage(pMsg);
}

void        DlgBarScript::AttachItemScriptFile(const char* fName)
{
    /**
    if(_pCurItem)
    {
		ScriptItem* pSci = _pCurItem->GetScript();
		if(pSci)
			DOC()->_scene.DelScript(pSci);

		if(fName[0] == 'I')
		{
			pSci = new ScriptItem();
			if(!pSci) 
                return;

			_pCurItem->SetScript(pSci);
			pSci->_script = fName;
			DOC()->_scene.AddScript(pSci);
		}
    }
    */
}


BOOL DlgBarScript::ShowOptionsForScript()
{
    if(_pCurBrush)
    {
	    PopulateFromFile(MKSTR("res/item-%d.gst", _pCurBrush->_name));

        /*
	    ScriptItem* pSci = _pCurBrush->GetScript();
        if(pSci)
        {
            _pToolFrame->LoadSciptFile(DOC()->GetTitle(), 
                                       _pCurBrush->_name, 
                                       pSci->_script.c_str());
        }
	    else
	    {
            _pToolFrame->LoadSciptFile(DOC()->GetTitle(), 
                                       _pCurBrush->_name, 
                                       "");
	    }
        */
    }
    else
    {
	    PopulateFromFile(MKSTR("res/item-%d.gst", _pCurItem->_item));
/*  
	    ScriptItem* pSci = _pCurItem->GetScript();
        if(pSci)
        {
            _pToolFrame->LoadSciptFile(DOC()->GetTitle(), 
                                       _pCurItem->_name, 
                                       pSci->_script.c_str());
        }
	    else
	    {
            _pToolFrame->LoadSciptFile(DOC()->GetTitle(), 
                                       _pCurItem->_name, 
                                       "");
	    }
*/        
    }
	return 1;
}

void    DlgBarScript::PopulateFromFile(const char* szTmplFname)
{
    FileWrap    fw;

	
    if(!fw.Open(szTmplFname,"rb"))
        return;

    char* pTok;
    char* pTokFoo;
    char szLine[128];
    int   index;
    
    while(!feof(fw._pf))
    {
        pTokFoo=0;
        fw.ReadLine(szLine, sizeof(szLine));
		StripCrlf(szLine);
		
       
        pTok =_tcstok(szLine,";");
        if(pTok)
		{
            pTokFoo = _tcstok(0,";");
		}

        if(pTokFoo)
        {
            index = _lbEvents.AddString(szLine);
			Event* pe = new Event;

            strcpy(pe->msgNName, szLine);
			strcpy(pe->fooProto,pTokFoo);
            _lbEvents.SetItemData(index, (size_t)pe);
        }

        if(feof(fw._pf))
            break;
    }
    fw.Close();
    
}

void DlgBarScript::OnDblclkEvents() 
{
    if(0==_pCurItem)
        return;
    //
    // check if the code for current event and item is generated
    // if not generate the code, ptherwise select the code at the begining
    //
    int		index	= _lbEvents.GetCurSel();
	Event*	pe		= (Event*)_lbEvents.GetItemData(index);

	char	fDecl[128];
	char	itmName[128];

	strcpy(itmName, _pCurItem->_name);
	::StripSpaces(itmName);
	sprintf(fDecl, pe->fooProto, itmName);

	CPoint	pt(0,0);
	
	if(!CCrystalTextView::PView->FindText(fDecl, pt, 0, 0, &pt))
	{
		_pToolFrame->_pDoc->InsertLine("//-----------------------------------------------------");
        _pToolFrame->_pDoc->InsertLine(MKSTR("//%s,%s ",pe->msgNName,itmName));
		_pToolFrame->_pDoc->InsertLine(fDecl);
		_pToolFrame->_pDoc->InsertLine("{");
		_pToolFrame->_pDoc->InsertLine("");
		_pToolFrame->_pDoc->InsertLine("}");
		_pToolFrame->_pDoc->InsertLine("");
        _pToolFrame->_pDoc->SetModifiedFlag();
	}
	else
	{
		CCrystalTextView::PView->SelectLine(pt.y);
	}

	_pToolFrame->_pDoc->UpdateAllViews();

}

void DlgBarScript::ClearListBox()
{
	int nCount = _lbEvents.GetCount();
	for(int i=0;i<nCount;i++)
	{
		Event* pe = (Event*)_lbEvents.GetItemData(i);
		delete pe;

	}
    _lbEvents.ResetContent();
}


