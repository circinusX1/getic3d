////////////////////////////////////////////////////////////////////////////
//	File:		CCrystalTextBuffer.h
//	Version:	1.0.0.0
//	Created:	29-Dec-1998
//
//	Author:		Stcherbatchenko Andrei
//	E-mail:		windfall@gmx.de
//
//	Interface of the CCrystalTextBuffer class, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCRYSTALTEXTBUFFER_H__AD7F2F49_6CB3_11D2_8C32_0080ADB86836__INCLUDED_)
#define AFX_CCRYSTALTEXTBUFFER_H__AD7F2F49_6CB3_11D2_8C32_0080ADB86836__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//   #pragma warning (disable: 4786)
#include "cedefs.h"
#include "CCrystalTextView.h"

#ifndef __AFXTEMPL_H__
#pragma message("Include <afxtempl.h> in your stdafx.h to avoid this message")
#include <afxtempl.h>
#endif

#define UNDO_DESCRIP_BUF        32

enum LINEFLAGS
{
	LF_BOOKMARK_FIRST			= 0x00000001L,
	LF_EXECUTION				= 0x00010000L,
	LF_BREAKPOINT				= 0x00020000L,
	LF_COMPILATION_ERROR		= 0x00040000L,
	LF_BOOKMARKS	        	= 0x00080000L,
	LF_INVALID_BREAKPOINT       = 0x00100000L
};

#define LF_BOOKMARK(id)		(LF_BOOKMARK_FIRST << id)

enum CRLFSTYLE
{
	CRLF_STYLE_AUTOMATIC	= -1,
	CRLF_STYLE_DOS			= 0,
	CRLF_STYLE_UNIX			= 1,
	CRLF_STYLE_MAC			= 2
};

enum
{
	CE_ACTION_UNKNOWN		= 0,
	CE_ACTION_PASTE			= 1,
	CE_ACTION_DELSEL		= 2,
	CE_ACTION_CUT			= 3,
	CE_ACTION_TYPING		= 4,
	CE_ACTION_BACKSPACE		= 5,
	CE_ACTION_INDENT		= 6,
	CE_ACTION_DRAGDROP		= 7,
	CE_ACTION_REPLACE		= 8,
	CE_ACTION_DELETE		= 9,
	CE_ACTION_AUTOINDENT	= 10
	//	...
	//	Expandable: user actions allowed
};


/////////////////////////////////////////////////////////////////////////////
// CUpdateContext class

class CUpdateContext
{
public:
	virtual void RecalcPoint(CPoint &ptPoint) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// CCrystalTextBuffer command target

class CRYSEDIT_CLASS_DECL CCrystalTextBuffer : public CCmdTarget
{
	DECLARE_DYNCREATE(CCrystalTextBuffer)

private:
	BOOL m_bInit;
	BOOL m_bReadOnly;
	BOOL m_bModified;
	int m_nCRLFMode;
	BOOL m_bCreateBackupFile;
	int m_nUndoBufSize;
	int FindLineWithFlag(size_t dwFlag);

public:
#pragma pack(push, 1)
	//	Nested class declarations
	struct SLineInfo
	{
		char	*m_pcLine;
		int		m_nLength, m_nMax;
		size_t	m_dwFlags;

		SLineInfo() { memset(this, 0, sizeof(SLineInfo)); };
	};

	enum
	{
		UNDO_INSERT			= 0x0001,
		UNDO_BEGINGROUP		= 0x0100
	};

	//	[JRT] Support For Descriptions On Undo/Redo Actions
	struct SUndoRecord
	{
		size_t	m_dwFlags;

		CPoint	m_ptStartPos, m_ptEndPos;			//	Block of text participating
		int		m_nAction;							//	For information only: action type

	private:
		//	char	*m_pcText;
		//	Since in most cases we have 1 character here, 
		//	we should invent a better way. Note: 2 * sizeof(WORD) <= sizeof(char*)
		//
		//	Here we will use the following trick: on Win32 platforms high-order word
		//	of any pointer will be != 0. So we can store 1 character strings without
		//	allocating memory.
		//

		union
		{
			char	*m_pszText;		//	For cases when we have > 1 character strings
			char	m_szText[2];	//	For single-character strings
		};

	public:
		//	constructor/destructor for this struct
		SUndoRecord() { memset(this, 0, sizeof(SUndoRecord)); };

		void SetText(const char* pszText);
		void FreeText();

		const char* GetText() const
		{
			if (HIWORD((size_t) m_pszText) != 0)
				return m_pszText;
			return m_szText;
		};
	};

#pragma pack(pop)

	class CInsertContext : public CUpdateContext
	{
	public:
		CPoint m_ptStart, m_ptEnd;
		virtual void RecalcPoint(CPoint &ptPoint);
	};

	class CDeleteContext : public CUpdateContext
	{
	public:
		CPoint m_ptStart, m_ptEnd;
		virtual void RecalcPoint(CPoint &ptPoint);
	};

	//	Lines of text
	CArray <SLineInfo, SLineInfo&> m_aLines;

	//	Undo
	CArray <SUndoRecord, SUndoRecord&> m_aUndoBuf;
	int m_nUndoPosition;
	int m_nSyncPosition;
	BOOL m_bUndoGroup, m_bUndoBeginGroup;

	//	Connected views
	CList <CCrystalTextView *, CCrystalTextView *> m_lpViews;

	//	Helper methods
	void InsertLine(const char* pszLine, int nLength = -1, int nPosition = -1);
	void AppendLine(int nLineIndex, const char* pszChars, int nLength = -1);
	void AppendLine(const char* pszChars);

	//	Implementation
	BOOL InternalInsertText(CCrystalTextView *pSource, int nLine, int nPos, const char* pszText, int &nEndLine, int &nEndChar);
	BOOL InternalDeleteText(CCrystalTextView *pSource, int nStartLine, int nStartPos, int nEndLine, int nEndPos);

	//	[JRT] Support For Descriptions On Undo/Redo Actions
	void AddUndoRecord(BOOL bInsert, const CPoint &ptStartPos, const CPoint &ptEndPos, 
						const char* pszText, int nActionType = CE_ACTION_UNKNOWN);

	//	Overridable: provide action description
	virtual BOOL GetActionDescription(int nAction, CString &desc);

// Operations
public:
	//	Construction/destruction code
	CCrystalTextBuffer();
	~CCrystalTextBuffer();

	//	Basic functions
    void RemoveAllFlags(size_t dwFlag);
	BOOL InitNew(int nCrlfStyle = CRLF_STYLE_DOS);
	BOOL LoadFromFile(const char* pszFileName, int nCrlfStyle = CRLF_STYLE_AUTOMATIC);
	BOOL SaveToFile(const char* pszFileName, int nCrlfStyle , BOOL bClearModifiedFlag , BOOL bIncludeMap, const char* pEx=0);
	void FreeAll();

	//	'Dirty' flag
	virtual void SetModified(BOOL bModified = TRUE);
	BOOL IsModified() const;

	//	Connect/disconnect views
	void AddView(CCrystalTextView *pView);
	void RemoveView(CCrystalTextView *pView);

	//	Text access functions
	int GetLineCount();
	int GetLineLength(int nLine);
	LPTSTR GetLineChars(int nLine);
	size_t GetLineFlags(int nLine);
	int GetLineWithFlag(size_t dwFlag);
	void SetLineFlag(int nLine, size_t dwFlag, BOOL bSet, BOOL bRemoveFromPreviousLine = TRUE);
	void GetText(int nStartLine, int nStartChar, int nEndLine, int nEndChar, CString &text, const char* pszCRLF = NULL);

	//	Attributes
	int GetCRLFMode();
	void SetCRLFMode(int nCRLFMode);
	BOOL GetReadOnly() const;
	void SetReadOnly(BOOL bReadOnly = TRUE);

	//	Text modification functions
	BOOL InsertText(CCrystalTextView *pSource, int nLine, int nPos, const char* pszText, int &nEndLine, int &nEndChar, int nAction = CE_ACTION_UNKNOWN);
	BOOL DeleteText(CCrystalTextView *pSource, int nStartLine, int nStartPos, int nEndLine, int nEndPos, int nAction = CE_ACTION_UNKNOWN);

	//	Undo/Redo
	BOOL CanUndo();
	BOOL CanRedo();
	BOOL Undo(CPoint &ptCursorPos);
	BOOL Redo(CPoint &ptCursorPos);

	//	Undo grouping
	void BeginUndoGroup(BOOL bMergeWithPrevious = FALSE);
	void FlushUndoGroup(CCrystalTextView *pSource);

	//	Browse undo sequence
	POSITION GetUndoDescription(CString &desc, POSITION pos = NULL);
	POSITION GetRedoDescription(CString &desc, POSITION pos = NULL);

	//	Notify all connected views about changes in text
	void UpdateViews(CCrystalTextView *pSource, CUpdateContext *pContext,
					size_t dwUpdateFlags, int nLineIndex = -1);

	// More bookmarks
	int FindNextBookmarkLine(int nCurrentLine = 0);
	int FindPrevBookmarkLine(int nCurrentLine = 0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrystalTextBuffer)
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCrystalTextBuffer)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#if ! (defined(CE_FROM_DLL) || defined(CE_DLL_BUILD))
#include "CCrystalTextBuffer.inl"
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCRYSTALTEXTBUFFER_H__AD7F2F49_6CB3_11D2_8C32_0080ADB86836__INCLUDED_)
