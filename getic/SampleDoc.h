// SampleDoc.h : interface of the CSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEDOC_H__B1B69ED1_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
#define AFX_SAMPLEDOC_H__B1B69ED1_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CCrystalTextBuffer.h"
class CSampleView;
class CSampleDoc : public CDocument
{
public: // create from serialization only
	CSampleDoc();
    virtual ~CSampleDoc()
    {
		m_xTextBuffer.FreeAll();
        TRACE("~CSampleDoc()\r\n");
    }
	DECLARE_DYNCREATE(CSampleDoc)

// Attributes
public:
	void ReportSaveLoadException(LPCTSTR lpszPathName,
				CException* e, BOOL bSaving, UINT nIDPDefault);

	class CSampleTextBuffer : public CCrystalTextBuffer
	{
	private:
		CSampleDoc *m_pOwnerDoc;
	public:
		CSampleTextBuffer(CSampleDoc *pDoc) { m_pOwnerDoc = pDoc; };
		virtual void SetModified(BOOL bModified = TRUE)
		{ 
			m_pOwnerDoc->SetModifiedFlag(bModified); 
		};
	};
	void InsertLine(LPCTSTR pszLine, BOOL ro=FALSE);

	CSampleTextBuffer	m_xTextBuffer;
	LOGFONT				m_lf;
	DWORD				m_scrCookie;
// Operations
public:
	static			CSampleDoc*	PDoc;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL
    void UpdateAllViews();//{
    
// Implementation
public:
    CSampleView*        _pView;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    BOOL DoSaveFile(LPCTSTR fileName);
// Generated message map functions
protected:
	//{{AFX_MSG(CSampleDoc)
	afx_msg void OnSaveInEditor();
	afx_msg void OnUpdateSaveInEditor(CCmdUI* pCmdUI);
	//}}AFX_MSG
    void OnBuild() ;
    void OnUpdateBuild(CCmdUI* pCmdUI) ;
    void OnTemplate() ;

	DECLARE_MESSAGE_MAP()
    int    _compileErr;    
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEDOC_H__B1B69ED1_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
