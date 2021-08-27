/////////////////////////////////////////////////////////////////////////////
// This ListCtrlDemo is done by Marlon Tang. You can distribute,
// EXCEPT: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.

// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Please send bug reports, bug fixes, enhancements, requests, etc., and
// Fell free to e-mail me at marlont@worldnet.att.net
//
// Special thanks for Matt Gullett to give me this idea
//

#if !defined(AFX_MYLISTCTRL_H__470DF1C6_9672_11D2_9F6C_0000C0A3B9F2__INCLUDED_)
#define AFX_MYLISTCTRL_H__470DF1C6_9672_11D2_9F6C_0000C0A3B9F2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window
class   PrpListControl;
typedef BOOL    (CDialog::*PFCHANGE)(PrpListControl*, int sel);

#define WM_USER_MESSAGE_NOTIFY				WM_USER + 1
#define	MY_EDIT_UNIT						3000
#define MY_COMBOBOX_UINT					3001
typedef enum
{
	FPSPROPERTYITEMTYPE_UNKNOW,				// ERROR
	FPSPROPERTYITEMTYPE_TEXT,				// CMyEditBox
	FPSPROPERTYITEMTYPE_INTEGER,			// CMyEditBox
	FPSPROPERTYITEMTYPE_LONG,				// CMyEditBox
	FPSPROPERTYITEMTYPE_FONT,				// Button
	FPSPROPERTYITEMTYPE_COLOR,				// Button
	FPSPROPERTYITEMTYPE_FILE,				// Button
	FPSPROPERTYITEMTYPE_COMBOBOX,			// CComboBox
	FPSPROPERTYITEMTYPE_BOOL,				// DoubleClick
    FPSPROPERTYITEMTYPE_HEADER,				// DoubleClick
}	OPERATION_TYPE;

typedef enum
{
	NO_CONTROL = 0,
	SPIN_CONTROL,
	BUTTON_CONTROL,
    CB_CONTROL,
    CK_BOX,
    HEADER,
}	CONTROL_TYPE;

typedef enum
{
	SPIN_BITMP = 0,
	BUTTON_DOWN_BITMAP,
	BUTTON_UP_BITMAP,
    COMBO_BITMAP,
    CHECK_BITMAP,
}	BITMAP_LOCATION;

/////////////////////////////////////////////////////////////////////////////
// class CMyLParam

class CMyLParam  
{
public:
	CMyLParam()
	{
		m_Operation_Type	= FPSPROPERTYITEMTYPE_UNKNOW;
		m_Control_Type		= NO_CONTROL;
		m_MyData			= NULL;
	}
	virtual ~CMyLParam()
	{
	}
	void	SetInfo(OPERATION_TYPE	Operation_Type,
					CONTROL_TYPE Control_Type, 
					void* MyData = NULL)
			{
				m_Operation_Type	= Operation_Type;
				m_Control_Type		= Control_Type;
				m_MyData			= MyData;
			}
	CONTROL_TYPE	GetControlType()
			{return m_Control_Type;}
	OPERATION_TYPE	GetOPType()
			{return m_Operation_Type;}
	void	SetData(void* Mydata)
			{m_MyData	= Mydata;}
	void*	GetData()
			{return m_MyData;}
protected:
	void*			m_MyData;
	OPERATION_TYPE	m_Operation_Type;
	CONTROL_TYPE	m_Control_Type;
};

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window
class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();
	virtual ~CMyEdit();
	BOOL Create(CWnd* pParentWnd, CRect rect, int Index);
	
// Attributes
public:
	BOOL	m_bESC;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	int		m_Index	;
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox window

class CMyComboBox : public CComboBox
{
// Construction
public:
	CMyComboBox();
	virtual ~CMyComboBox();
	BOOL Create(CWnd* pParentWnd, CRect rect, int Index);
	
// Attributes
public:
	BOOL	m_bESC;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	int		m_Index	;
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyComboBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// PrpListControl window

class PrpListControl : public CListCtrl
{
// Construction
public:
	PrpListControl();
	virtual ~PrpListControl();

// Attributes
public:
    virtual void PreSubclassWindow();
// Operations
public:
	void	Initial(CString Title,	int	TitleWidth,
					CString Data,	int	DataWidth);
	BOOL	AddItem(CString			Information, 
					CString			Data, 
					OPERATION_TYPE	Operation_type,
					void*			InputStringList = NULL);
	BOOL	AddItem(CString			Information, 
					int				Data, 
					OPERATION_TYPE	Operation_type,
					void*			InputStringList = NULL);
	BOOL	AddItem(CString			Information, 
					long			Data, 
					OPERATION_TYPE	Operation_type,
					void*			InputStringList = NULL);
	BOOL	DeleteAllItems();
	BOOL	DeleteItem(int nItem );
	// Find Information From Index of ListCtrl
	BOOL	GetData(int				Index,
					OPERATION_TYPE&	Op_type,
					CString&		Title,
					CString&		Data,
					void*&			DataPointer);
	// Find Information From Title of ListCtrl
	// If there are 2 exect titles in ListCtrl, this function always
	// return first  data information.
	BOOL	FindData(CString			Title,
					 int&				Index,
					 OPERATION_TYPE&	Op_type,
					 CString&			Data,
					 void*&				DataPointer);
	// Transfer data from InputStringList to MyFont
	void	FontPassing(LOGFONT*	InputStringList, 
						LOGFONT*	MyFont);
	BOOL	GetTitle(int	Index, CString& Title);
    void    SetNotyFunction(CDialog* pDlg, PFCHANGE  pfooNoty){m_pDlg = pDlg ;  m_pfooNoty = pfooNoty;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PrpListControl)
	//}}AFX_VIRTUAL

protected:
    CFont           m_boldFont;
    CDialog*        m_pDlg;
    PFCHANGE        m_pfooNoty;
	CMyEdit*		m_MyEdit;
	CMyComboBox*	m_MyComBoBox;
	CDC				m_Bitmap_DC;
	CBitmap*		m_pBitmap;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);


// override all default ClistCtrl Function
	CImageList* GetImageList(int nImageList) const{return NULL;}
	CImageList* SetImageList(CImageList* pImageList, int nImageListType){return NULL;}
	BOOL GetItem(LV_ITEM* pItem) const{return TRUE;}
	BOOL SetItem(const LV_ITEM* pItem){return TRUE;}
	BOOL SetItem(int nItem, int nSubItem, size_t nMask, const char* lpszItem,
		int nImage, size_t nState, size_t nStateMask, LPARAM lParam){return TRUE;}
	size_t GetCallbackMask() const{return 1;}
	BOOL SetCallbackMask(size_t nMask){return TRUE;}
	int GetNextItem(int nItem, int nFlags) const{return TRUE;}
	BOOL SetItemPosition(int nItem, POINT pt){return TRUE;}
	BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const{return TRUE;}
	int GetStringWidth(const char* lpsz) const{return 1;}
	CEdit* GetEditControl() const{return NULL;}
	BOOL GetColumn(int nCol, LV_COLUMN* pColumn) const{return TRUE;}
	BOOL SetColumn(int nCol, const LV_COLUMN* pColumn){return TRUE;}
	int GetColumnWidth(int nCol) const{return 1;}
	
	BOOL GetViewRect(LPRECT lpRect) const{return TRUE;}
	int GetTopIndex() const{return 1;}
	int GetCountPerPage() const{return 1;}
	BOOL GetOrigin(LPPOINT lpPoint) const{return TRUE;}
	BOOL SetItemState(int nItem, LV_ITEM* pItem){return TRUE;}
	BOOL SetItemState(int nItem, size_t nState, size_t nMask){return TRUE;}
	size_t GetItemState(int nItem, size_t nMask) const{return 1;}
	CString GetItemText(int nItem, int nSubItem) const{return "";}
	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const{return 1;}
	BOOL SetItemText(int nItem, int nSubItem, const char* lpszText){return TRUE;}
	void SetItemCount(int nItems);
	BOOL SetItemData(int nItem, size_t dwData){return TRUE;}
	size_t GetItemData(int nItem) const;
	size_t GetSelectedCount() const{return 1;}

// Operations
	int InsertItem(const LV_ITEM* pItem){return 1;}
	int InsertItem(int nItem, const char* lpszItem){return 1;}
	int InsertItem(int nItem, const char* lpszItem, int nImage){return 1;}
	int FindItem(LV_FINDINFO* pFindInfo, int nStart = -1) const{return 1;}
	int HitTest(LV_HITTESTINFO* pHitTestInfo) const{return 1;}
	int HitTest(CPoint pt, size_t* pFlags = NULL) const{return 1;}
	BOOL EnsureVisible(int nItem, BOOL bPartialOK){return TRUE;}
	BOOL Scroll(CSize size){return TRUE;}
	BOOL RedrawItems(int nFirst, int nLast){return TRUE;}
	BOOL Arrange(size_t nCode){return TRUE;}
	CEdit* EditLabel(int nItem){return NULL;}
	int InsertColumn(int nCol, const LV_COLUMN* pColumn){return 1;}
	int InsertColumn(int nCol, const char* lpszColumnHeading,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1){return 1;}
	BOOL DeleteColumn(int nCol){return TRUE;}
	CImageList* CreateDragImage(int nItem, LPPOINT lpPoint){return NULL;}
	BOOL Update(int nItem){return TRUE;}
	BOOL SortItems(PFNLVCOMPARE pfnCompare, size_t dwData){return TRUE;}
	BOOL GetItemRect(int nItem, LPRECT lpRect, size_t nCode) const{return TRUE;}


/*	// Functions do not override
	BOOL SetColumnWidth(int nCol, int cx);
	COLORREF GetTextColor() const;
	BOOL SetTextColor(COLORREF cr);
	COLORREF GetTextBkColor() const;
	BOOL SetTextBkColor(COLORREF cr);
*/
	// My Own Function
	CMyLParam*	GetMyLParam(int Index);
	CRect	GetCellRect(int nItem, int nColumn);
	void	MyOutText(CString	MyString, CDC* pDC, CRect* rect);
	CString	FindText(int Index, CMyLParam*	MyLParam);
	void*	SetRelatedInfo( OPERATION_TYPE	Operation_type, 
							void*			InputStringList,
							CString			MyData);
	void	DataPassing(CStringList*	InputStringList, 
						CStringList*	MyStringList);

	void	OperateBOOL(int Index, CMyLParam*&	MyLParam);
	void	OperateFont(int	Index, CRect rcItem, CMyLParam*&		MyLParam);
	void	OperateColor(int	Index, CRect rcItem, CMyLParam*&	MyLParam);
	void	OperateSpinInt(int	Index, BOOL	Incerasing);
	void	OperateSpinLong(int	Index, BOOL	Incerasing);
	void	OperateFile(int	Index, CRect rcItem);
	void	OperateEditBox (int Index, CRect rcItem, CMyLParam*	MyLParam);
	void	OperateComboBox(int Index, CRect rcItem, CMyLParam*	MyLParam);

	void	UpdateEditBox (int	Index);
	void	UpdateComboBox(int	Index);

	void	DrawDownButton(CRect&	rcItem);
	// Generated message map functions
protected:
	//{{AFX_MSG(PrpListControl)
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    BOOL OnEraseBkgnd(CDC* pDC) ;
    LRESULT OnUserMessageNotify(LPARAM nID, WPARAM Index);
    LRESULT OnSetFont(WPARAM wParam, LPARAM);
    void MeasureItem ( LPMEASUREITEMSTRUCT lpms );
    

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__470DF1C6_9672_11D2_9F6C_0000C0A3B9F2__INCLUDED_)
