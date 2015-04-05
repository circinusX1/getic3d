// DlgScene.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "z-edmap.h"
#include "DlgScene.h"
#include "DlgBar.h"
#include "z_ed2Doc.h"
#include "BaseEntity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgScene dialog


CDlgScene::CDlgScene(CWnd* pParent /*=NULL*/)
	: CVertDlgDlg(CDlgScene::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgScene)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    p_be = 0;
}


void CDlgScene::DoDataExchange(CDataExchange* pDX)
{
	CVertDlgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgScene)
	DDX_Control(pDX, PB_LOGO, m_l);
	DDX_Control(pDX, PB_DELLOGO, m_bx);
	DDX_Control(pDX, EF_DETAILS, m_det);
	DDX_Control(pDX, PB_REFRESH, m_bow);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, LB_SCPROPS, _prpList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScene, CVertDlgDlg)
	//{{AFX_MSG_MAP(CDlgScene)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_BN_CLICKED(PB_REFRESH, OnRefresh)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
    ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, OnSelchangingTree1)
	ON_EN_KILLFOCUS(EF_DETAILS, OnKillfocusDetails)
	ON_BN_CLICKED(PB_LOGO, OnLogo)
	ON_BN_CLICKED(PB_DELLOGO, OnDellogo)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_DIRTY, OnDirty)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScene message handlers
// LEV_QUEST, LEV_TEAM, LEV_MULTITEAM
BOOL CDlgScene::OnInitDialog() 
{
    _paintzone.CreateWnd(this, ST_SPLASH);

    CVertDlgDlg::OnInitDialog();
   _dirty = FALSE;

    m_det.SetLimitText(8912);

    RECT rt;
    _prpList.GetClientRect(&rt);
    int cxs = GetSystemMetrics(SM_CXHSCROLL);



    //_prpList.SetNotyFunction(this, CDlgScene::NotyFoo);
    _prpList.Initial("Property", (2*rt.right/3)-2, "Value", (rt.right/3)-cxs);

    this->Update();

    Enable(1);
    _oldMode = SEL_NONE;
    return TRUE;  
}

BOOL CDlgScene::NotyFoo(PrpListControl* pList, int index)
{

    return 0;
}



void CDlgScene::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CVertDlgDlg::OnShowWindow(bShow, nStatus);
    if(bShow)
    {
        _oldMode = DOC()->_selmode;
        Update();
        Invalidate();
    }
    else
    {
        Dirty();
        Retreive();
        BARDLG()->OnApply();	
        DOC()->SetSelMode(_oldMode);
    }
}




void CDlgScene::Retreive()
{
    if(!_dirty)
        return;

    OPERATION_TYPE	op_type;
	CString			title;
	CString			strData;
	void*			dataPointer	= NULL;
	int	            totalItem	= _prpList.GetItemCount() + 1;
    int             sel = 0;

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    strcpy(SCENE()._si.author, strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    strcpy(SCENE()._si.comments, strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.levelType = 0;

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.maxActors = atoi(strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.maxFrags = atoi(strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.maxTime = atoi(strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    COLORREF crf = (COLORREF)dataPointer;
    SCENE()._si.defLumin.r = GetRValue(crf);
    SCENE()._si.defLumin.g = GetGValue(crf);
    SCENE()._si.defLumin.b = GetBValue(crf);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.gravAcc = atoi(strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.camFar = (REAL)atoi(strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.camFov = (REAL)atoi(strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    crf = (COLORREF)dataPointer;
    SCENE()._si.fogColor.r = GetRValue(crf);
    SCENE()._si.fogColor.g = GetGValue(crf);
    SCENE()._si.fogColor.b = GetBValue(crf);


    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.fogFar= (REAL)atof(strData);
                            
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.fogNear = (REAL)atof(strData);

    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    SCENE()._si.fogDens = atof(strData) / 100000.0;


    OnKillfocusDetails();
    DOC()->RecreateAmbLmap();
	_dirty=FALSE;
}



void CDlgScene::OnOk() 
{
    Dirty();
    BARDLG()->OnApply();	
}

void CDlgScene::OnCancel() 
{
}

void CDlgScene::Dirty()
{
    _dirty = TRUE;
    BARDLG()->Dirty();
}


void CDlgScene::OnKillFocus(CWnd* pNewWnd) 
{
	CVertDlgDlg::OnKillFocus(pNewWnd);
    Dirty();
    Retreive();	
    BARDLG()->OnApply();	
}

void CDlgScene::OnSetFocus(CWnd* pOldWnd) 
{
	CVertDlgDlg::OnSetFocus(pOldWnd);
    Dirty();
	Retreive();	
	BARDLG()->OnApply();	
	
}


void CDlgScene::Update()
{
    if(!_dirty)
    {
        PopTreeScene();    
        return;
    }
        _prpList.DeleteAllItems();
    
	_prpList.AddItem("Author",	    
							SCENE()._si.author,								
							FPSPROPERTYITEMTYPE_TEXT);		

	_prpList.AddItem("Comments",	    
							SCENE()._si.comments,								
							FPSPROPERTYITEMTYPE_TEXT);		
    char szLev[3][32] = {
        "Single",
        "Quest",
        "Team",
    };
    CStringList		strList;
    strList.RemoveAll();
    strList.AddTail(szLev[0]);
    strList.AddTail(szLev[1]);
    strList.AddTail(szLev[2]);
    _prpList.AddItem("Level Type",			
                        SCENE()._si.levelType, //_bPvsMethod
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    

    _prpList.AddItem("Players",	    
							SCENE()._si.maxActors,								
							FPSPROPERTYITEMTYPE_INTEGER);		

    _prpList.AddItem("Frags",	    
							SCENE()._si.maxFrags,								
							FPSPROPERTYITEMTYPE_INTEGER);		

    _prpList.AddItem("Time",	    
							(int)SCENE()._si.maxTime,								
							FPSPROPERTYITEMTYPE_INTEGER);		

    COLORREF crf = RGB(SCENE()._si.defLumin.r,SCENE()._si.defLumin.g,SCENE()._si.defLumin.b);
	_prpList.AddItem("Default Light",			
							"",							
							FPSPROPERTYITEMTYPE_COLOR,	
							(void*) crf);

	_prpList.AddItem("Gravitation",			
							(int)SCENE()._si.gravAcc,	
							FPSPROPERTYITEMTYPE_INTEGER);


	_prpList.AddItem("Far Cut Plane",			
							(int)SCENE()._si.camFar,	
							FPSPROPERTYITEMTYPE_INTEGER);

	_prpList.AddItem("Camera FOV",			
							(int)SCENE()._si.camFov,	
							FPSPROPERTYITEMTYPE_INTEGER);

    COLORREF crff = RGB(SCENE()._si.fogColor.r,SCENE()._si.fogColor.g,SCENE()._si.fogColor.b);
	_prpList.AddItem("Fog Color",			
							"",	
							FPSPROPERTYITEMTYPE_COLOR,
                            (void*)crff);

	_prpList.AddItem("Fog Far",			
							MKSTR("%.2f",SCENE()._si.fogFar),	
							FPSPROPERTYITEMTYPE_TEXT);
                            
	_prpList.AddItem("Fog Near",			
							MKSTR("%.2f",SCENE()._si.fogNear),	
							FPSPROPERTYITEMTYPE_TEXT);

    _prpList.AddItem("Fog Density",			
							MKSTR("%.2f",SCENE()._si.fogDens*100000.0),	
							FPSPROPERTYITEMTYPE_TEXT);


    PopTreeScene();    
	_dirty=FALSE;


#ifdef _FREE
	GetDlgItem(LB_SCPROPS)->EnableWindow(0);
	GetDlgItem(IDC_TREE1)->EnableWindow(0);
	SetDlgItemText(ST_MSG,"Getic 1.20.02.Free Feature Disabled");
#endif

}


void CDlgScene::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

}

void CDlgScene::OnRefresh() 
{
    Update();	
}


void CDlgScene::PopTreeScene()
{
    m_tree.DeleteAllItems();
    
    Scene&    scene = SCENE();
    HTREEITEM hRoot  = m_tree.InsertItem(DOC()->GetTitle());
    HTREEITEM htmp;
    HTREEITEM htmps; 
    int       j = 0;

    //brushes
    FOREACH(PBrushes, (*scene.GetPrimitives()), ppbb)
	{
		Brush* pB =*ppbb;

        htmp = m_tree.InsertItem(MKSTR("_B:%s",pB->_name), 0, 0, hRoot);
        m_tree.SetItemData(htmp, (DWORD)pB);

        //polys
        vvector<Poly>::iterator pb = pB->_polys.begin();
        vvector<Poly>::iterator pe = pB->_polys.end();
        for(;pb!=pe;pb++)
        {
            htmps = m_tree.InsertItem(MKSTR("_F:%d",j++),1,1, htmp);
            m_tree.SetItemData(htmps, (DWORD)(&(*pb)));
        }

        if(pB->_brushflags & BRSH_HASTRIG)
        {
            m_tree.InsertItem("_T",4,4, htmp);
        }
        if(pB->_pMotion)
        {
            htmps = m_tree.InsertItem("_M",3,3, htmp);
            m_tree.SetItemData(htmps, (DWORD)pB->_pMotion);
        }
        
        if(pB->_pCutBrshes.size())
        {
            vvector<Brush*>::iterator ppCut  =  pB->_pCutBrshes.begin();
            vvector<Brush*>::iterator ppCutE =  pB->_pCutBrshes.end();
            for(;ppCut!=ppCutE;ppCut++)
            {
                m_tree.InsertItem(MKSTR("_B:%s",(*ppCut)->_name),2,2, htmp);
                m_tree.SetItemData(htmps, (DWORD)(*ppCut));
            }
        }
    }


    //items
    FOREACH(vvector<SceItem*>, (*scene.GetItems()), pItemPtr)
    {
        SceItem* pItem = *pItemPtr;

        htmps = m_tree.InsertItem(MKSTR("_I:%s",pItem->_name), hRoot);
        m_tree.SetItemData(htmps, (DWORD)pItem);
    }
}


void CDlgScene::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}


void CDlgScene::OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    OnKillfocusDetails();

}

void CDlgScene::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    DOC()->DeselectAll();

    HTREEITEM   sitem = pNMTreeView->itemNew.hItem;
    DWORD       ptr   = m_tree.GetItemData(sitem);
    CString     cs    = m_tree.GetItemText(sitem);
    
    V3      center;
    Box*    pb = 0;
    Box     ib;
    p_be  = 0;
    
    if(cs.Find("_B")!=-1)
    {
        DOC()->SetSelMode(SEL_BRUSH);
        DOC()->SelectBrush((Brush*)ptr);
        center= ((Brush*)ptr)->_box.GetCenter();
        pb = &((Brush*)ptr)->_box;
        p_be = reinterpret_cast<BaseEntity*>(ptr);
    }
    else if(cs.Find("_F")!=-1)
    {
        DOC()->SetSelMode(SEL_FACE);
        DOC()->SelectFace((Poly*)ptr);
        pb = &((Poly*)ptr)->_box;
        center= ((Poly*)ptr)->_box.GetCenter();

        
        
        HTREEITEM   parsitem = m_tree.GetParentItem(sitem);
        DWORD       ptr   = m_tree.GetItemData(parsitem);
        if(ptr)
            p_be  = reinterpret_cast<BaseEntity*>(ptr);
    }
    else if(cs.Find("_I")!=-1)
    {
        DOC()->SetSelMode(SEL_ITEMS);
        DOC()->SelectItem((SceItem*)ptr);
        center= ((SceItem*)ptr)->_t;
        ib.AddPoint(center-V3(128,128,128));
        ib.AddPoint(center+V3(128,128,128));
        pb    = &ib;
        p_be  = reinterpret_cast<BaseEntity*>(ptr);
    }
    else
        DOC()->SetSelMode(SEL_NONE);

    if(pb)    
    {
        Box     lb = *pb;
        Camera& cam = DOC()->_cam;
        lb.Expand(128);
        cam._pos = lb._max;
        cam._fwd = center - lb._max;
        cam._fwd.norm();

        map<string ,string>&  descs = SCENE().Descriptions();
        map<string ,string>::iterator fid = descs.find(p_be->_name);
        if(fid != descs.end())
        {
            m_det.SetWindowText(fid->second.c_str());
        }
        else
        {
            m_det.SetWindowText("");
        }
        DOC()->Invalidate(1);
    }
    else
        m_det.SetWindowText("");
	*pResult = 0;
}

void CDlgScene::OnKillfocusDetails() 
{
    if(p_be == 0)
    {
        HTREEITEM   sitem = m_tree.GetSelectedItem();
        if(!sitem)
            return;
        DWORD       ptr   = m_tree.GetItemData(sitem);
        CString     cs    = m_tree.GetItemText(sitem);
        if(cs.Find("_B")!=-1 || cs.Find("_I")!=-1)
        {
            p_be  = reinterpret_cast<BaseEntity*>(ptr);
        }else if(cs.Find("_F")!=-1)
        {
            HTREEITEM   parsitem = m_tree.GetParentItem(sitem);
            ptr   = m_tree.GetItemData(parsitem);
            if(ptr) p_be  = reinterpret_cast<BaseEntity*>(ptr);
        }
    }
    if(p_be)
    {
        map<string ,string>&  descs = SCENE().Descriptions();
        CString cs; m_det.GetWindowText(cs);
        descs[p_be->_name] = (LPCTSTR)cs;
    }
}

void CDlgScene::PreSubclassWindow() 
{

	CVertDlgDlg::PreSubclassWindow();
}

BOOL CDlgScene::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message==WM_KEYDOWN)
    {
        if(pMsg->wParam==VK_RETURN)
        {
            OnOk() ;
        }
    }

	return CVertDlgDlg::PreTranslateMessage(pMsg);
}

void CDlgScene::OnLogo() 
{
    char szFilters[] = _T("Bitmap Files (*.bmp)|*.bmp|"
                          "Jpeg Files (*.jpg)|*.jpg|"  
                          "Targa Files (*.tga)|*.tga||");  

    // Prompt the user for a filename
    CFileDialog dlg(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters);
    dlg.m_ofn.lpstrTitle = _T("Select File");
    dlg.m_ofn.lpstrInitialDir = theApp.TexDir();
    // Show the dialog

    if (theApp.DoFileDialog(dlg) != IDOK) return;   
    PathHandler ph(dlg.m_ofn.lpstrFile);

    REDIR();
    _tcscpy(SCENE()._si.sceneLogoFile, ph.File());

    DOC()->DuplicateFile(DOC()->ResFile(SCENE()._si.sceneLogoFile), TRUE);
    

    _paintzone._th.Reset();
    _paintzone._th.LoadFile(DOC()->ResFile(SCENE()._si.sceneLogoFile), 0);
	_paintzone.Repaint();
    
}

void CDlgScene::OnDellogo() 
{
    SCENE()._si.sceneLogoFile[0]=0;
}

void CDlgScene::OnExpanded()
{
    if(_prpList.IsWindowVisible())
    {
        Invalidate();
    }
}

