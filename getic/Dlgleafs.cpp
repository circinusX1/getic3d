// DlgLeafs.cpp : implementation file
//

#include "stdafx.h"
#include "geticgui.h"
#include "DlgLeafs.h"
#include "Compiler.h"
#include "z_ed2Doc.h"
#include "BspTree.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDlgLeafs::CDlgLeafs(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgLeafs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLeafs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    _prevLeaf = 0;
}


void CDlgLeafs::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLeafs)
	DDX_Control(pDX, CB_CONTENT, m_cbc);
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, LC_TEXREFS, m_lv);//
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLeafs, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgLeafs)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, LC_TEXREFS, OnClickTexrefs)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_CBN_SELCHANGE(CB_CONTENT, OnSelchangeContent)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_ITEMSEL, OnItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLeafs message handlers

BOOL CDlgLeafs::OnInitDialog() 
{
    m_lv.Block();

    CBASEDLG::OnInitDialog();
    m_lv.SetHeadersNames("Index","Polygons","Parent Node","Cluster","Tree Depth","Content",0);
    Compiler& compiler = DOC()->_compiler;

    if(compiler._bTrees.size())
    {
        UINT      nparents = 0;
        CBspTree* pTree = compiler._bTrees[0];

        FOREACH(vvector<CLeaf*>, pTree->_leafs, ppl)
        {
            CLeaf* pl      = *ppl;
            CMiniNode* plp = (CMiniNode*)pl;
            
            while(plp = plp->Parent())
            {
                ++nparents;
            }


            m_lv.AddNewItem(MKSTR("%d", pl->_leafIdx), 
                            MKSTR("%d", pl->_nPolys),
                            MKSTR("%d", pl->_idxParent),
                            MKSTR("%d", pl->_cluster),
                            MKSTR("%d", nparents),
                            MKSTR("%X", pl->_content),
                            
                            0);
        }
    }

    m_lv.Block(0);
    return TRUE;  
	              
}

LRESULT CDlgLeafs::OnItemChanged(WPARAM w,LPARAM l)
{
    _iSel = w;

    UINT leafIdx = ::atoi(m_lv.GetItemText(_iSel,0));
        //mark branch
    
    Compiler& compiler = DOC()->_compiler;
    CBspTree* pTree = compiler._bTrees[0];

    if(_prevLeaf)
    {
        while(_prevLeaf != pTree->Root())
        {
            _prevLeaf->_selected = 0;
            _prevLeaf = pTree->GetNode(_prevLeaf->_idxParent);
        }
        _prevLeaf=0;
    }
    if(leafIdx!=-1)
    {
        DOC()->_leafNo = leafIdx;
        CMiniNode* pNodeLeaf = pTree->GetLeaf(leafIdx);
        if(pNodeLeaf)
        {
            _prevLeaf = pNodeLeaf;
            while(pNodeLeaf != pTree->Root())
            {
                pNodeLeaf->_selected = 1;
                pNodeLeaf = pTree->GetNode(pNodeLeaf->_idxParent);
            }
            DOC()->Invalidate();
        }
        else
            _prevLeaf = 0;
    }
    return 0;
}


void CDlgLeafs::OnClickTexrefs(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    _iSel = pNMListView->iItem;
    OnItemChanged(_iSel,0);
    *pResult = 0;
}


void CDlgLeafs::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}



void CDlgLeafs::OnDestroy() 
{
    Compiler& compiler = DOC()->_compiler;
    CBspTree* pTree = compiler._bTrees[0];

    if(_prevLeaf)
    {
        while(_prevLeaf != pTree->Root())
        {
            _prevLeaf->_selected = 0;
            _prevLeaf = pTree->GetNode(_prevLeaf->_idxParent);
        }

    }
    DOC()->Invalidate();
    CBASEDLG::OnDestroy();
	
}

void CDlgLeafs::OnCancel() 
{
    FRM()->DestroyLeafExplorer();
}

void CDlgLeafs::OnOk() 
{
	FRM()->DestroyLeafExplorer();
}

void CDlgLeafs::OnSelchangeContent() 
{
    if(!_prevLeaf)
    {
        return;
    }
        
    switch(m_cbc.GetCurSel())
    {
        case 0:
        default:
            _prevLeaf->_content= MODEL_MAT_SOLID;
            break;
        case 1:
            _prevLeaf->_content = MODEL_MAT_WATER;
            break;
        case 2:
            _prevLeaf->_content = MODEL_MAT_LAVA;
            break;
        case 3:
            _prevLeaf->_content = MODEL_MAT_BRICK;
            break;
        case 4:
            _prevLeaf->_content = MODEL_MAT_WOOD;
            break;
        case 5:
            _prevLeaf->_content = MODEL_MAT_METAL;
            break;
        case 6:
            _prevLeaf->_content = MODEL_MAT_ICE;
            break;
        case 7:
            _prevLeaf->_content = MODEL_MAT_GLASS;
            break;
        case 8:
            _prevLeaf->_content = MODEL_MAT_DIRT;
            break;
        case 9:
            _prevLeaf->_content = MODEL_MAT_GELA;
            break;
        case 10:
            _prevLeaf->_content = MODEL_MAT_PLASMA;
            break;
        case 11:
            _prevLeaf->_content = MODEL_MAT_GAS;
            break;
        case 12:
            _prevLeaf->_content = MODEL_MAT_FOG;
            break;

    }
    m_lv.SetItemText(_iSel, 4, MKSTR("%X",_prevLeaf->_content));
    m_lv.Invalidate();

}
