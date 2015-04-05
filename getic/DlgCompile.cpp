// DlgCompile.cpp : implementation file
//

#include "stdafx.h"
#include "versionmacro.h"
#include "z-edmap.h"
#include "Compiler.h"
#include "DlgCompile.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; 
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCompile dialog


CDlgCompile::CDlgCompile(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgCompile::IDD, pParent)
{
    _bPvsMethod = 1;
    _bZones     = 0;
    //{{AFX_DATA_INIT(CDlgCompile)
	//}}AFX_DATA_INIT
}


void CDlgCompile::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCompile)
	DDX_Control(pDX, IDC_TAB1, m_tb);
	DDX_Control(pDX, IDCANCEL, m_2);
	DDX_Control(pDX, IDOK, m_1);
	//DDX_Control(pDX, LB_CO, _lbc);
    DDX_Control(pDX, LB_COPRP, _prpList);
    
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCompile, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgCompile)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
BOOL CDlgCompile::OnInitDialog() 
{
    CBASEDLG::OnInitDialog();

    RECT rt;
    _prpList.GetClientRect(&rt);
    int cxs = GetSystemMetrics(SM_CXHSCROLL);
    _prpList.Initial("Property", (2*rt.right/3)-2, "Value", (rt.right/3)-cxs);

    //_prpList.AddItem("BSP COMPILER", "", FPSPROPERTYITEMTYPE_HEADER);		    


    //1-------------------------------------------------------------------
	CStringList		strList;
    for(int i=0;i<16;i++)
	    strList.AddTail(MKSTR("%d",i));
    _prpList.AddItem("BSP Balance",			
						MKSTR("%d", _balance),
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    

    //2-------------------------------------------------------------------
    char sPsort[3][32]={"No Sorting","By Area","By Normal"};
    strList.RemoveAll();
    strList.AddTail(sPsort[0]);
    strList.AddTail(sPsort[1]);
    strList.AddTail(sPsort[2]);
    _prpList.AddItem("BSP Pre-Sort Criteria",			
                        sPsort[_bPolySortCrit],
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    

    //3-------------------------------------------------------------------
    char sPpvs[3][32]={"No PVS","Fast PVS","Full PVS (slow)"};
    strList.RemoveAll();
    strList.AddTail(sPpvs[0]);
    strList.AddTail(sPpvs[1]);
    strList.AddTail(sPpvs[2]);

    _prpList.AddItem("PVS Creation Method",			
                        sPpvs[_bPvsMethod], //_bPvsMethod
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    

    //4-------------------------------------------------------------------
	_prpList.AddItem("Generate Visibility Volumes",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bZones);		

    //5-------------------------------------------------------------------
	_prpList.AddItem("Compile CSG Terrain as AABB",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bAABBTreeCsgTer);		

    //6-------------------------------------------------------------------
	_prpList.AddItem("Remove Invalid Leafs",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bRemopl);		

    //7-------------------------------------------------------------------
    strList.RemoveAll();
    char sPcut[3][32]={"Node","Parents","Leafs"};

    strList.AddTail(sPcut[0]);
    strList.AddTail(sPcut[1]);
    strList.AddTail(sPcut[2]);

    _prpList.AddItem("Portal generation",			
						sPcut[_portalCutFlag],
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    

    //8-------------------------------------------------------------------
    _prpList.AddItem("Build Light Maps",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bLightMaps);		

    //9-------------------------------------------------------------------
    strList.RemoveAll();
    for(int i=1;i<20;i++)
    {
        strList.AddTail(MKSTR("%d", i));
    }
    _prpList.AddItem("Light Map Tweak Intensity",			
						MKSTR("%d", _lmapIntens),
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    

    //10-------------------------------------------------------------------
    _prpList.AddItem("Uniform Light Maps",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _lmConst);		

    //11-------------------------------------------------------------------
    _prpList.AddItem("Smooth Light Maps",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _b3SampleLM);		
    
    //12-------------------------------------------------------------------
    _prpList.AddItem("Patch Patchable Polygons for LM",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bLmPatch);		
    //13-------------------------------------------------------------------
    if(0==_lMapLumelSz)
        _lMapLumelSz=64.0f;
    strList.RemoveAll();
    strList.AddTail(MKSTR("%.2f", 32.00));
    strList.AddTail(MKSTR("%.2f", 64.00));
    strList.AddTail(MKSTR("%.2f", 128.00));
    strList.AddTail(MKSTR("%.2f", 256.00));
    strList.AddTail(MKSTR("%.2f", 512.00));
    _prpList.AddItem("Light Map Pel Size",			
						MKSTR("%.2f", _lMapLumelSz),
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    

    //14-------------------------------------------------------------------
    if(0==_lmapMaxSize)
        _lmapMaxSize=16;

    strList.RemoveAll();
    strList.AddTail(MKSTR("%d", 4));
    strList.AddTail(MKSTR("%d", 8));
    strList.AddTail(MKSTR("%d", 16));
    strList.AddTail(MKSTR("%d", 32));
    strList.AddTail(MKSTR("%d", 64));
    strList.AddTail(MKSTR("%d", 128));
    _prpList.AddItem("Max Light Map UV Size",			
						MKSTR("%d", _lmapMaxSize),
						FPSPROPERTYITEMTYPE_COMBOBOX,	
						(void*) &strList);		    
    
    //15-------------------------------------------------------------------
    _prpList.AddItem("Compress BSP",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _szipit);		

    //16-------------------------------------------------------------------
    _prpList.AddItem("Merge Narrow Leafs",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bMergeNarrowLeafs);		

    //17-------------------------------------------------------------------
    _prpList.AddItem("Save SkyDom",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bSaveSkyDom);		

    //18-------------------------------------------------------------------
    _prpList.AddItem("Save Big Terrain",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bSaveBigTerrain);		

 
    //19-------------------------------------------------------------------
    _prpList.AddItem("Save Portals",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bSavePortals);		
    //20-------------------------------------------------------------------
    _prpList.AddItem("Embed Resources in BSP file",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bSaveImages);		
    //21-------------------------------------------------------------------
    _prpList.AddItem("Export To Server Folder",	    
							"",								
							FPSPROPERTYITEMTYPE_BOOL,		
							(void*) _bExportFile);		


	TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,NULL,32,0,NULL};
    
	m_tb.InsertItem(0,"Getic BSP");
    m_tb.InsertItem(1,"Quake BSP");
    if(0==_bGeticBSP)
    {
        m_tb.SetCurSel(1);
        OnSelchangeTab1(0, 0) ;
    }
	return TRUE;  
}


void CDlgCompile::OnOk() 
{
    int             k;
    OPERATION_TYPE	op_type;
	CString			title;
	CString			strData;
	void*			dataPointer	= NULL;
	int	            totalItem	= _prpList.GetItemCount() + 1;

    int             sel = 0; //header

    //1
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _balance =  atoi(strData);

    //2
    char sPsort[3][32]={"No Sorting","By Area","By Normal"};
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    for(k=0; k<3;k++)
        if(strData == sPsort[k]){
            _bPolySortCrit = k;
            break;
        }

    //3
    char sPpvs[3][32]={"No PVS","Fast PVS","Full PVS (slow)"};
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    for(k=0; k<3;k++)
        if(strData == sPpvs[k]){
            _bPvsMethod = k;
            break;
        }
    
    //4
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bZones = dataPointer ? 1 : 0;


    //5
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bAABBTreeCsgTer = dataPointer ? 1 : 0;

    //6
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bRemopl = dataPointer ? 1 : 0;

	//7
    char sPcut[3][32]={"Node","Parents","Leafs"};
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    for(k=0; k<3;k++)
        if(strData == sPpvs[k]){
            _portalCutFlag = k;
            break;
        }
    //8
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bLightMaps = dataPointer ? 1 : 0;

    //9
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _lmapIntens = atoi(strData);

    //10
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _lmConst = dataPointer ? 1 : 0;

    //11
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _b3SampleLM = dataPointer ? 1 : 0;

    //12
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bLmPatch = dataPointer ? 1 : 0;

    //13
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _lMapLumelSz = atof(strData);

    //14
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _lmapMaxSize = atoi(strData);

    //15
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _szipit = dataPointer ? 1 : 0;

    //16
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bMergeNarrowLeafs = dataPointer ? 1 : 0;

    //17
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bSaveSkyDom = dataPointer ? 1 : 0;

    //18
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bSaveBigTerrain = dataPointer ? 1 : 0;


    //19
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bSavePortals = dataPointer ? 1 : 0;

    //20
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bSaveImages = dataPointer ? 1 : 0;

    //21
    _prpList.GetData(sel++,	op_type, title,	strData, dataPointer);
    _bExportFile = dataPointer ? 1 : 0;


	CBASEDLG::OnOK();	

}


void CDlgCompile::OnKillFocus(CWnd* pNewWnd) 
{
    CBASEDLG::OnKillFocus(pNewWnd);

}

HBRUSH CDlgCompile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBASEDLG::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}



void CDlgCompile::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    _prpList.ShowWindow(0==m_tb.GetCurSel() ? SW_SHOW : SW_HIDE);
    GetDlgItem(ST_QUAKE)->ShowWindow(1==m_tb.GetCurSel() ? SW_SHOW : SW_HIDE);
    pResult ? *pResult = 0 : 0;
}

void CDlgCompile::OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}
