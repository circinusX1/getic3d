//---------------------------------------------------------------------------------------
// DlgBar.h : header file
#ifndef __DLG_BAR_H__
#define __DLG_BAR_H__

#pragma warning (disable: 4786)
#include "uienhancer.h"
#include "DlgFaceProp.h"
#include "DlgBrshProp.h"
#include "DlgTextures.h"    
#include "TabDlgSel.h"
#include "DlgItems.h"
#include "DlgSoundItem.h"
#include "DlgTriggerItem.h"
#include "DlgStartPlayItem.h"
#include "DlgScene.h"
#include "DlgLightItem.h"
#include "DlgGameItem.h"
#include "DlgMaterial.h"
#include "resource.h"
//---------------------------------------------------------------------------------------
// CDlgBar dialog
#ifdef _BAR_DLGPANE
    #define BASE_CLS    CFormView
#else
    #define BASE_CLS    CBASEDLG //modeless
#endif //

#define WM_REDRAWCHILDREN	(WM_USER+1238)
//---------------------------------------------------------------------------------------

class CMotion;

class CDlgBar : public BASE_CLS
{
// Construction
public:
	DECLARE_DYNCREATE(CDlgBar)

    static CDlgBar* GBarDlg;
	CDlgBar(CWnd* pParent = NULL);   // standard constructor

	virtual void OnInitialUpdate();

// Dialog Data
	//{{AFX_DATA(CDlgBar)
	enum { IDD = IDR_MAINFRAME }; 
	CStatic	_stMsg;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBar)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

    void    OnOK(){}
    void    OnCancel();
    void    ActivateTexturesDlg(BOOL bShow);
    void    ShowItemDlg(SceItem* pItem);
	char*   GetCurSelTexture();
    void    ShowSelItmDlg(int sel);
    LRESULT HandleInitDialog2(WPARAM, LPARAM);
    void    OnBrushesSelected();
    void    OnItemsSelected();
    void    OnFacesSelected();
    DWORD   GettexGenFlag();
    void    ResetBrushes();
    void	BlockRetreive(BOOL blk){_blkRetreive = blk;}
	BOOL	IsRetreiveBlocked(){return (_blkRetreive || _initDlg);}
	void    RefreshItemsList();
    void    ShowItemProps(DWORD itmtype);
	void	AddItem(SceItem* pItem);
	void    DelItem(SceItem* pItem);
    void    OnItemPropsChanged(SceItem* pItem);
    void    ScanCreationFlags();
    void    OnItemSelected(SceItem* pItem, int sel=1);
    void    UpdateItmProps(SceItem* pItem);
    void    UpdateMotionProps(Brush*, CMotion* pM);
	void	ForceApply(){Dirty(); OnApply();};
    void    MakeVisible(CBASEDLG* pDlg);
    void    DeselectItems(BOOL notyFrame);
    void    HideAll();
    void    Rescan();
    void    Refresh();
    void    ResetDlgsContent();
    void    Clean(){
            _dirty=0;
    		if(IsWindow(m_hWnd))
            {
	    		GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
                GetDlgItem(ST_MSG)->SetWindowText("Ready");
            }

    };
    void    Dirty(){
        if(IsRetreiveBlocked())
            return;
        if(_dirty==0)
        {
            _dirty = 1;
    		if(IsWindow(m_hWnd))
            {
	    		GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
	            GetDlgItem(ST_MSG)->SetWindowText("Press Enter To Commit");
            }
            
        }
	}
    afx_msg void OnApply();
    void ShowBrushesProperties(Brush** ppBrushes, int count);
    void ShowFacesProperties(Poly** ppPolys, int count);
	LRESULT RedrawCHildren(WPARAM,LPARAM);//WM_REDRAWCHILDREN
    // Implementation
protected:
    void    DisableDlg(CBASEDLG* pd);
    void    EnableDlg(CBASEDLG* pd);

    void    CommonInitializer();
    virtual void OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler );
    virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlgBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
    LRESULT OnPostInit(LPARAM, WPARAM) ;
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    LRESULT OnKeyTab(WPARAM,LPARAM);
	void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    vvector<CBASEDLG*>   _pDlgs;
    BOOL            _dirty;
public:
    TabDlgSel       _tb;        // tab control
    CBaseDlg*  _pItemBase;  // ptr to current 
    CBaseDlg*  _pItemBasePrev;  // ptr to current Prev blinking issue
    CBASEDLG*       _pItemBaseB;

    CDlgTextures    _dlgtex;    
    //DlgSounds       _dlgSnd;    
    DlgItems        _dlgItem;   
    //DlgScripts      _dlgScript; 
    CDlgScene       _dlgScene;  
    
    // down side dialogs
    DlgSoundItem     _dlgSoundItem; 
    DlgLightItem     _dlgLightItem; 
    DlgTriggerItem   _dlgTrigger;   
    DlgFaceProp      _dlgFp;
    DlgBrshProp      _dlgBp;
    DlgGameItem      _dlgGameItem;
    DlgStartPlayItem _dlgStartPlayItem;


    int             _tabSelIdx;
	BOOL			_blkRetreive;
    CBrush          _dirtyBrush;
    BOOL            _initDlg;
    CColorButon     _applyBut;
    BOOL            _blockDocEvents;
};

#define BARDLG()    CDlgBar::GBarDlg


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBAR_H__4A80DAFF_63A0_4F67_B523_3B3D7185D52B__INCLUDED_)
