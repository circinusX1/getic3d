#if !defined(AFX_DLGTEXTURES_H__539C1423_A878_43F7_90B3_335CA9C88729__INCLUDED_)
#define AFX_DLGTEXTURES_H__539C1423_A878_43F7_90B3_335CA9C88729__INCLUDED_


#include "uienhancer.h"
#include "scene.h"
#include "VertDlgDlg.h"
#include "SceItem.h"
#include "TexLbox.h"
#include <string>
#include <vector>
using namespace std;

#define WM_HANDLE_TEXSEL    (WM_USER+2000)

class CDlgTextures : public CVertDlgDlg, public CarryTex
{
// Construction
public:
	CDlgTextures(CWnd* pParent = NULL);   // standard constructor
    DWORD GettexGenFlag();
	int	 GetCurSel(char* pTex);
    BOOL SetTexture(Poly& poly, int& pV);
    void SetTexCoords(TexCoords& texCoord);
    void SelectFaceTexture(const char* psztexture);
	BOOL OnApplytex();
    void EnableTexControls(BOOL);
	void Activate(int ia){};
    void SkyDomOn(BOOL b)   {_bSkyDom = b;}
    void  GetTextureName(int idx, LPSTR pName){
        m_lf.GetText(idx ,pName);
    }
    const char*   GetTextureFullName(int i){
        if(i==-1)i = m_lf.GetCurSel();
        return i>=0 ? _texFiles[i].c_str() : "";
    }
    void     GetTextureSubPaths(vvector<tstring>& r);
    void     SelectSubPath(int i);
    void     OnAction();
    int GetTexCount(){return _texFiles.size();}
// Dialog Data
	//{{AFX_DATA(CDlgTextures)
	enum { IDD = DLG_TEX };
	CComboBox	_tabdirs;
	CColorButon	m_dt;
	CColorButon	m_prev;
	CColorButon	m_next;
	CColorButon	m_sss;
	CColorButon	m_b1;
	
	CTexLbox	m_lf;
	CEdit	m_escalex;
	CEdit	m_escaley;
	CEdit	m_eshiftx;
	CEdit	m_eshifty;
	CEdit	m_eshift;
	CSpinButtonCtrl	m_scalex;
	CSpinButtonCtrl	m_scaley;
	CSpinButtonCtrl	m_shiftx;
	CSpinButtonCtrl	m_shifty;
	CStatic	m_bpp;
	CStatic	m_size;
	CStatic	m_name;
	CStatic	m_path;
	//}}AFX_DATA
    CBitmapButton   _btnTexRot;
    CBitmapButton   _btnTexSv;
    CBitmapButton   _btnTexSh;
    BOOL            _barNoty;
   

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTextures)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
    
// Implementation
protected:
	void OnTexCoordChanged();
    void BuildFileList();
    void ShowPicture(int index);
    void Fliptexture(char cd);
	void Dirty();
	void PopSubDirText(LPCTSTR subDir);
    BOOL DocSetTexture(SceItem& item, int& pV);
    BOOL DocSetTexture(Poly& poly, int& pV);
    BOOL DocOnApplytex() ;
    void DocOnTexCoordChanged();
    void DocFliptexture(char cd);
    void DocOnRotate() ;

    BOOL DomSetTexture(Poly& poly, int& pV);
    BOOL DomOnApplytex() ;
    void DomOnTexCoordChanged();
    void DomFliptexture(char cd);
    void DomOnRotate() ;


	// Generated message map functions
	//{{AFX_MSG(CDlgTextures)
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclkList1();
	afx_msg void OnSelchangeList1();
	afx_msg void OnApllytex();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnDeltaposSpinShiftV(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinShiftH(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinScaleV(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinScaleH(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRotate();
	afx_msg void OnFliph();
	afx_msg void OnFlipv();
	afx_msg void OnSeltbt();
	afx_msg void OnIngnore();
	afx_msg void OnSelchangeTk();
	afx_msg void OnSelchangeTf();
	afx_msg void OnSelchangeTgen();
	afx_msg void OnDeltex();
	afx_msg void OnCloseupTab1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    BOOL             _bSkyDom;   
    vvector<tstring>  _texFiles;
    int             _texIter;
    BOOL            _initalized;
    TexCoords       _texCoord;
	BOOL			_copyt;
	BOOL			_edChanged;
	char			_cwd[_MAX_PATH];
	BOOL			_showPick;
    int             _paneNo;
    BOOL            _resetTex;
    BOOL            _texwasSet;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEXTURES_H__539C1423_A878_43F7_90B3_335CA9C88729__INCLUDED_)
