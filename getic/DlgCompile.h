#if !defined(AFX_DLGCOMPILE_H__A18E5168_CD3F_46F3_9FD8_8ACF844A33D4__INCLUDED_)
#define AFX_DLGCOMPILE_H__A18E5168_CD3F_46F3_9FD8_8ACF844A33D4__INCLUDED_

#include "prplb.h"
#include "PrpListCtrl.h"
#include "UiEnhancer.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCompile dialog

class CDlgCompile : public CBASEDLG
{
// Construction
public:
	CDlgCompile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCompile)
	enum { IDD = DLG_COMPILESET };
	C_TABCTRL	m_tb;
	CColorButon	m_2;
	CColorButon	m_1;
	
    PrpListControl _prpList;
	//}}AFX_DATA

	BOOL	_ckESR;
    BOOL _bCompressPvs;
    BOOL _bAABBTreeCsgTer;

    int     _bPvsMethod;
    BOOL    _bGeticBSP;//         = AfxGetApp()->GetProfileInt("BSP","GETICBSP",1);
    BOOL    _bSnapMainBsp;
    BOOL    _bZones;
    BOOL	_ckSave;
    BOOL    _szipit;
    BOOL    _bMergeNarrowLeafs;
    BOOL    _portalCutFlag;
	BOOL	_bSavePortals;
    BOOL    _bPolySortCrit;
	BOOL	_bSaveSkyDom;
    BOOL    _bSaveBigTerrainBSP;
    BOOL    _bSaveBigTerrain;
	BOOL	_bSaveImages;
    long    _maxFacesLeaf ;
    int     _lmapIntens;

	BOOL	_bLightMaps;
    BOOL    _lmpowof2;
    BOOL    _lmConst;
    BOOL    _bConvexEarly;
    BOOL    _b3SampleLM;
	BOOL	_bRemopl;
	BOOL	_bExportFile;
	BOOL	_ckREMS1;
    CString _fileName;
    int     _balance;
    int     _lmapMaxSize;
    REAL    _lMapLumelSz;
    BOOL    _bLmPatch;
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCompile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//    BOOL    LmapSzDlgNoty(CLbDlg* pDlg);
//    BOOL    LumSzDlgNoty(CLbDlg* pDlg);
    BOOL    LmTweak(CLbDlg* pDlg);
    BOOL    SortByArea(CLbDlg* pDlg);
    BOOL    SortByNormal(CLbDlg* pDlg);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCompile)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    
	DECLARE_MESSAGE_MAP()
        
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMPILE_H__A18E5168_CD3F_46F3_9FD8_8ACF844A33D4__INCLUDED_)
