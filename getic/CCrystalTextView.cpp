//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "z-edmap.h"
#include <malloc.h>
#include <tchar.h>
#include "editcmd.h"
#include "editreg.h"
#include "CCrystalTextView.h"
#include "CCrystalTextBuffer.h"
#include "CFindTextDlg.h"

#ifndef __AFXPRIV_H__
#pragma message("Include <afxpriv.h> in your stdafx.h to avoid this message")
#include <afxpriv.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_CHARACTER    _T('\xBB')
#define SPACE_CHARACTER    _T('\x95')
#define DEFAULT_PRINT_MARGIN  1000    // 10 millimeters

#define SMOOTH_SCROLL_FACTOR  6



IMPLEMENT_DYNCREATE(CCrystalTextView, CView)

HINSTANCE CCrystalTextView::s_hResourceInst = 0;

BEGIN_MESSAGE_MAP(CCrystalTextView, CView)
//{{AFX_MSG_MAP(CCrystalTextView)
ON_WM_DESTROY()
ON_WM_ERASEBKGND()
ON_WM_SIZE()
ON_WM_VSCROLL()
ON_WM_SETCURSOR()
ON_WM_LBUTTONDOWN()
ON_WM_SETFOCUS()
ON_WM_HSCROLL()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_TIMER()
ON_WM_KILLFOCUS()
ON_WM_LBUTTONDBLCLK()
ON_COMMAND(ID_SC_COPY, OnEditCopy)
ON_UPDATE_COMMAND_UI(ID_SC_COPY, OnUpdateEditCopy)
ON_COMMAND(ID_SC_SELECT_ALL, OnEditSelectAll)
ON_UPDATE_COMMAND_UI(ID_SC_SELECT_ALL, OnUpdateEditSelectAll)
ON_WM_RBUTTONDOWN()
ON_WM_SYSCOLORCHANGE()
ON_WM_CREATE()
ON_COMMAND(ID_SC_FIND, OnEditFind)
ON_COMMAND(ID_SC_REPEAT, OnEditRepeat)
ON_UPDATE_COMMAND_UI(ID_SC_REPEAT, OnUpdateEditRepeat)
ON_COMMAND(ID_SC_FIND_PREVIOUS, OnEditFindPrevious)
ON_UPDATE_COMMAND_UI(ID_SC_FIND_PREVIOUS, OnUpdateEditFindPrevious)
//}}AFX_MSG_MAP
ON_COMMAND(ID_SC_CHAR_LEFT, OnCharLeft)
ON_COMMAND(ID_SC_EXT_CHAR_LEFT, OnExtCharLeft)
ON_COMMAND(ID_SC_CHAR_RIGHT, OnCharRight)
ON_COMMAND(ID_SC_EXT_CHAR_RIGHT, OnExtCharRight)
ON_COMMAND(ID_SC_WORD_LEFT, OnWordLeft)
ON_COMMAND(ID_SC_EXT_WORD_LEFT, OnExtWordLeft)
ON_COMMAND(ID_SC_WORD_RIGHT, OnWordRight)
ON_COMMAND(ID_SC_EXT_WORD_RIGHT, OnExtWordRight)
ON_COMMAND(ID_SC_LINE_UP, OnLineUp)
ON_COMMAND(ID_SC_EXT_LINE_UP, OnExtLineUp)
ON_COMMAND(ID_SC_LINE_DOWN, OnLineDown)
ON_COMMAND(ID_SC_EXT_LINE_DOWN, OnExtLineDown)
ON_COMMAND(ID_SC_SCROLL_UP, ScrollUp)
ON_COMMAND(ID_SC_SCROLL_DOWN, ScrollDown)
ON_COMMAND(ID_SC_PAGE_UP, OnPageUp)
ON_COMMAND(ID_SC_EXT_PAGE_UP, OnExtPageUp)
ON_COMMAND(ID_SC_PAGE_DOWN, OnPageDown)
ON_COMMAND(ID_SC_EXT_PAGE_DOWN, OnExtPageDown)
ON_COMMAND(ID_SC_LINE_END, OnLineEnd)
ON_COMMAND(ID_SC_EXT_LINE_END, OnExtLineEnd)
ON_COMMAND(ID_SC_HOME, OnHome)
ON_COMMAND(ID_SC_EXT_HOME, OnExtHome)
ON_COMMAND(ID_SC_TEXT_BEGIN, OnTextBegin)
ON_COMMAND(ID_SC_EXT_TEXT_BEGIN, OnExtTextBegin)
ON_COMMAND(ID_SC_TEXT_END, OnTextEnd)
ON_COMMAND(ID_SC_EXT_TEXT_END, OnExtTextEnd)
ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
ON_UPDATE_COMMAND_UI(ID_SC_INDICATOR_CRLF, OnUpdateIndicatorCRLF)
ON_UPDATE_COMMAND_UI(ID_SC_INDICATOR_POSITION, OnUpdateIndicatorPosition)
ON_COMMAND_RANGE(ID_SC_TOGGLE_BOOKMARK0, ID_SC_TOGGLE_BOOKMARK9, OnToggleBookmark)
ON_COMMAND_RANGE(ID_SC_GO_BOOKMARK0, ID_SC_GO_BOOKMARK9, OnGoBookmark)
ON_COMMAND(ID_SC_CLEAR_BOOKMARKS, OnClearBookmarks)
ON_COMMAND(ID_SC_TOGGLE_BOOKMARK,     OnToggleBookmark)
ON_COMMAND(ID_SC_GOTO_NEXT_BOOKMARK,  OnNextBookmark)
ON_COMMAND(ID_SC_GOTO_PREV_BOOKMARK,  OnPrevBookmark)
ON_COMMAND(ID_SC_CLEAR_ALL_BOOKMARKS, OnClearAllBookmarks)
ON_UPDATE_COMMAND_UI(ID_SC_GOTO_NEXT_BOOKMARK,  OnUpdateNextBookmark)
ON_UPDATE_COMMAND_UI(ID_SC_GOTO_PREV_BOOKMARK,  OnUpdatePrevBookmark)
ON_UPDATE_COMMAND_UI(ID_SC_CLEAR_ALL_BOOKMARKS, OnUpdateClearAllBookmarks)
END_MESSAGE_MAP()

#define EXPAND_PRIMITIVE(impl, func) \
void CCrystalTextView::On##func() { impl(FALSE); } \
void CCrystalTextView::OnExt##func() { impl(TRUE); }
EXPAND_PRIMITIVE(MoveLeft, CharLeft)
EXPAND_PRIMITIVE(MoveRight, CharRight)
EXPAND_PRIMITIVE(MoveWordLeft, WordLeft)
EXPAND_PRIMITIVE(MoveWordRight, WordRight)
EXPAND_PRIMITIVE(MoveUp, LineUp)
EXPAND_PRIMITIVE(MoveDown, LineDown)
EXPAND_PRIMITIVE(MovePgUp, PageUp)
EXPAND_PRIMITIVE(MovePgDn, PageDown)
EXPAND_PRIMITIVE(MoveHome, Home)
EXPAND_PRIMITIVE(MoveEnd, LineEnd)
EXPAND_PRIMITIVE(MoveCtrlHome, TextBegin)
EXPAND_PRIMITIVE(MoveCtrlEnd, TextEnd)
#undef EXPAND_PRIMITIVE



//--|CCrystalTextView::CCrystalTextView|--------------------------------------------------
CCrystalTextView::CCrystalTextView()
{
    AFX_ZERO_INIT_OBJECT(CView);
    m_bSelMargin = TRUE;
    
    m_pIcons =0;
    m_pdwParseCookies=0;
    m_pnActualLineLength=0;
    m_apFonts[0]=m_apFonts[1]=m_apFonts[2]=m_apFonts[3]=0;
    
    ResetView();
    
}

//--|CCrystalTextView::~CCrystalTextView|-------------------------------------------------
CCrystalTextView::~CCrystalTextView()
{
    ASSERT(m_hAccel == 0);
    ASSERT(m_pCacheBitmap == 0);
    ASSERT(m_pTextBuffer == 0);  // Must be correctly detached
    if (m_pszLastFindWhat != 0)
       free(m_pszLastFindWhat);
    if (m_pdwParseCookies != 0)
       delete[] m_pdwParseCookies;
    
    if (m_pnActualLineLength != 0)
       delete[] m_pnActualLineLength;
}

//--| CCrystalTextView::PreCreateWindow|--------------------------------------------------
BOOL CCrystalTextView::PreCreateWindow(CREATESTRUCT& cs)
{
    CWnd *pParentWnd = CWnd::FromHandlePermanent(cs.hwndParent);
    if (pParentWnd == 0 || ! pParentWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
    {
        // View must always create its own scrollbars,
        // if only it's not used within splitter
        cs.style |= (WS_HSCROLL | WS_VSCROLL);
    }
    cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
    return CView::PreCreateWindow(cs);
}



//--| CCrystalTextView::GetSelection|-----------------------------------------------------
void CCrystalTextView::GetSelection(CPoint &ptStart, CPoint &ptEnd)
{
    PrepareSelBounds();
    ptStart = m_ptDrawSelStart;
    ptEnd = m_ptDrawSelEnd;
}

//--| *CCrystalTextView::LocateTextBuffer|------------------------------------------------
CCrystalTextBuffer *CCrystalTextView::LocateTextBuffer()
{
    return 0;
}

//--| CCrystalTextView::GetLineActualLength|----------------------------------------------
int CCrystalTextView::GetLineActualLength(int nLineIndex)
{
    int nLineCount = GetLineCount();
    ASSERT(nLineCount > 0);
    ASSERT(nLineIndex >= 0 && nLineIndex < nLineCount);
    if (m_pnActualLineLength == 0)
    {
        m_pnActualLineLength = new int[nLineCount];
        memset(m_pnActualLineLength, 0xff, sizeof(int) * nLineCount);
        m_nActualLengthArraySize = nLineCount;
    }
    
    if (m_pnActualLineLength[nLineIndex] >= 0)
       return m_pnActualLineLength[nLineIndex];
    
    // Actual line length is not determined yet, let's calculate a little
    int nActualLength = 0;
    int nLength = GetLineLength(nLineIndex);
    if (nLength > 0)
    {
        LPCTSTR pszLine = GetLineChars(nLineIndex);
        LPTSTR pszChars = (LPTSTR) _alloca(sizeof(char) * (nLength + 1));
        memcpy(pszChars, pszLine, sizeof(char) * nLength);
        pszChars[nLength] = 0;
        LPTSTR pszCurrent = pszChars;
        
        int nTabSize = GetTabSize();
        for (;;)
        {
            #ifdef _UNICODE
                LPTSTR psz = wcschr(pszCurrent, L'\t');
            #else
                LPTSTR psz = strchr(pszCurrent, '\t');
            #endif
                if (psz == 0)
            {
                nActualLength += (pszChars + nLength - pszCurrent);
                break;
            }
            
            nActualLength += (psz - pszCurrent);
            nActualLength += (nTabSize - nActualLength % nTabSize);
            pszCurrent = psz + 1;
        }
    }
    
    m_pnActualLineLength[nLineIndex] = nActualLength;
    return nActualLength;
}

//--| CCrystalTextView::ScrollToChar|-----------------------------------------------------
void CCrystalTextView::ScrollToChar(int nNewOffsetChar, BOOL bNoSmoothScroll /*= FALSE*/, BOOL bTrackScrollBar /*= TRUE*/)
{
    // For now, ignoring bNoSmoothScroll and m_bSmoothScroll
    if (m_nOffsetChar != nNewOffsetChar)
    {
        int nScrollChars = m_nOffsetChar - nNewOffsetChar;
        m_nOffsetChar = nNewOffsetChar;
        CRect rcScroll;
        GetClientRect(&rcScroll);
        rcScroll.left += GetMarginWidth();
        ScrollWindow(nScrollChars * GetCharWidth(), 0, &rcScroll, &rcScroll);
        UpdateWindow();
        if (bTrackScrollBar)
           RecalcHorzScrollBar(TRUE);
    }
}

//--| CCrystalTextView::ScrollToLine|-----------------------------------------------------
void CCrystalTextView::ScrollToLine(int nNewTopLine, BOOL bNoSmoothScroll /*= FALSE*/, BOOL bTrackScrollBar /*= TRUE*/)
{
    if (m_nTopLine != nNewTopLine)
    {
        if (bNoSmoothScroll || ! m_bSmoothScroll)
        {
            int nScrollLines = m_nTopLine - nNewTopLine;
            m_nTopLine = nNewTopLine;
            ScrollWindow(0, nScrollLines * GetLineHeight());
            UpdateWindow();
            if (bTrackScrollBar)
               RecalcVertScrollBar(TRUE);
        }
        else
        {
            // Do smooth scrolling
            int nLineHeight = GetLineHeight();
            if (m_nTopLine > nNewTopLine)
            {
                int nIncrement = (m_nTopLine - nNewTopLine) / SMOOTH_SCROLL_FACTOR + 1;
                while (m_nTopLine != nNewTopLine)
                {
                    int nTopLine = m_nTopLine - nIncrement;
                    if (nTopLine < nNewTopLine)
                       nTopLine = nNewTopLine;
                    int nScrollLines = nTopLine - m_nTopLine;
                    m_nTopLine = nTopLine;
                    ScrollWindow(0, - nLineHeight * nScrollLines);
                    UpdateWindow();
                    if (bTrackScrollBar)
                       RecalcVertScrollBar(TRUE);
                }
            }
            else
            {
                int nIncrement = (nNewTopLine - m_nTopLine) / SMOOTH_SCROLL_FACTOR + 1;
                while (m_nTopLine != nNewTopLine)
                {
                    int nTopLine = m_nTopLine + nIncrement;
                    if (nTopLine > nNewTopLine)
                       nTopLine = nNewTopLine;
                    int nScrollLines = nTopLine - m_nTopLine;
                    m_nTopLine = nTopLine;
                    ScrollWindow(0, - nLineHeight * nScrollLines);
                    UpdateWindow();
                    if (bTrackScrollBar)
                       RecalcVertScrollBar(TRUE);
                }
            }
        }
    }
}

//--| CCrystalTextView::ExpandChars|------------------------------------------------------
void CCrystalTextView::ExpandChars(LPCTSTR pszChars, int nOffset, int nCount, CString &line)
{
    int I;
    if (nCount <= 0)
    {
        line = _T("");
        return;
    }
    
    int nTabSize = GetTabSize();
    
    int nActualOffset = 0;
    for ( I = 0; I < nOffset; I ++)
    {
        if (pszChars[I] == _T('\t'))
           nActualOffset += (nTabSize - nActualOffset % nTabSize);
        else
            nActualOffset ++;
    }
    
    pszChars += nOffset;
    int nLength = nCount;
    
    int nTabCount = 0;
    for (I = 0; I < nLength; I ++)
    {
        if (pszChars[I] == _T('\t'))
           nTabCount ++;
    }
    
    LPTSTR pszBuf = line.GetBuffer(nLength + nTabCount * (nTabSize - 1) + 1);
    int nCurPos = 0;
    if (nTabCount > 0 || m_bViewTabs)
    {
        for (I = 0; I < nLength; I ++)
        {
            if (pszChars[I] == _T('\t'))
            {
                int nSpaces = nTabSize - (nActualOffset + nCurPos) % nTabSize;
                if (m_bViewTabs)
                {
                    pszBuf[nCurPos ++] = TAB_CHARACTER;
                    nSpaces --;
                }
                while (nSpaces > 0)
                {
                    pszBuf[nCurPos ++] = _T(' ');
                    nSpaces --;
                }
            }
            else
            {
                if (pszChars[I] == _T(' ') && m_bViewTabs)
                   pszBuf[nCurPos] = SPACE_CHARACTER;
                else
                    pszBuf[nCurPos] = pszChars[I];
                nCurPos ++;
            }
        }
    }
    else
    {
        memcpy(pszBuf, pszChars, sizeof(char) * nLength);
        nCurPos = nLength;
    }
    pszBuf[nCurPos] = 0;
    line.ReleaseBuffer();
}

//--| CCrystalTextView::DrawLineHelperImpl|-----------------------------------------------
void CCrystalTextView::DrawLineHelperImpl(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip,
                                         LPCTSTR pszChars, int nOffset, int nCount)
{
    ASSERT(nCount >= 0);
    if (nCount > 0)
    {
        CString line;
        ExpandChars(pszChars, nOffset, nCount, line);
        int nWidth = rcClip.right - ptOrigin.x;
        if (nWidth > 0)
        {
            int nCharWidth = GetCharWidth();
            int nCount = line.GetLength();
            int nCountFit = nWidth / nCharWidth + 1;
            if (nCount > nCountFit)
               nCount = nCountFit;
            #ifdef _DEBUG
            //CSize sz = pdc->GetTextExtent(line, nCount);
            //ASSERT(sz.cx == m_nCharWidth * nCount);
            #endif
            /*
            CRect rcBounds = rcClip;
            rcBounds.left = ptOrigin.x;
            rcBounds.right = rcBounds.left + GetCharWidth() * nCount;
            pdc->ExtTextOut(rcBounds.left, rcBounds.top, ETO_OPAQUE, &rcBounds, 0, 0, 0);
            */
                VERIFY(pdc->ExtTextOut(ptOrigin.x, ptOrigin.y, ETO_CLIPPED, &rcClip, line, nCount, 0));
        }
        ptOrigin.x += GetCharWidth() * line.GetLength();
    }
}

//--| CCrystalTextView::DrawLineHelper|---------------------------------------------------
void CCrystalTextView::DrawLineHelper(CDC *pdc, CPoint &ptOrigin, const CRect &rcClip, int nColorIndex,
                                     LPCTSTR pszChars, int nOffset, int nCount, CPoint ptTextPos)
{
    if (nCount > 0)
    {
        if (m_bFocused || m_bShowInactiveSelection)
        {
            int nSelBegin = 0, nSelEnd = 0;
            if (m_ptDrawSelStart.y > ptTextPos.y)
            {
                nSelBegin = nCount;
            }
            else
                if (m_ptDrawSelStart.y == ptTextPos.y)
            {
                nSelBegin = m_ptDrawSelStart.x - ptTextPos.x;
                if (nSelBegin < 0)
                   nSelBegin = 0;
                if (nSelBegin > nCount)
                   nSelBegin = nCount;
            }
            if (m_ptDrawSelEnd.y > ptTextPos.y)
            {
                nSelEnd = nCount;
            }
            else
                if (m_ptDrawSelEnd.y == ptTextPos.y)
            {
                nSelEnd = m_ptDrawSelEnd.x - ptTextPos.x;
                if (nSelEnd < 0)
                   nSelEnd = 0;
                if (nSelEnd > nCount)
                   nSelEnd = nCount;
            }
            
            ASSERT(nSelBegin >= 0 && nSelBegin <= nCount);
            ASSERT(nSelEnd >= 0 && nSelEnd <= nCount);
            ASSERT(nSelBegin <= nSelEnd);
            
            // Draw part of the text before selection
            if (nSelBegin > 0)
            {
                DrawLineHelperImpl(pdc, ptOrigin, rcClip, pszChars, nOffset, nSelBegin);
            }
            if (nSelBegin < nSelEnd)
            {
                COLORREF crOldBk = pdc->SetBkColor(GetColor(COLORINDEX_SELBKGND));
                COLORREF crOldText = pdc->SetTextColor(GetColor(COLORINDEX_SELTEXT));
                DrawLineHelperImpl(pdc, ptOrigin, rcClip, pszChars, nOffset + nSelBegin, nSelEnd - nSelBegin);
                pdc->SetBkColor(crOldBk);
                pdc->SetTextColor(crOldText);
            }
            if (nSelEnd < nCount)
            {
                DrawLineHelperImpl(pdc, ptOrigin, rcClip, pszChars, nOffset + nSelEnd, nCount - nSelEnd);
            }
        }
        else
        {
            DrawLineHelperImpl(pdc, ptOrigin, rcClip, pszChars, nOffset, nCount);
        }
    }
}

//--| CCrystalTextView::GetLineColors|----------------------------------------------------
void CCrystalTextView::GetLineColors(int nLineIndex, COLORREF &crBkgnd,
                                    COLORREF &crText, BOOL &bDrawWhitespace)
{
    DWORD dwLineFlags = GetLineFlags(nLineIndex);
    bDrawWhitespace = TRUE;
    crText = RGB(255, 255, 255);
    if (dwLineFlags & LF_EXECUTION)
    {
        crBkgnd = RGB(0, 128, 0);
        return;
    }
    if (dwLineFlags & LF_BREAKPOINT)
    {
        crBkgnd = RGB(255, 0, 0);
        return;
    }
    if (dwLineFlags & LF_INVALID_BREAKPOINT)
    {
        crBkgnd = RGB(128, 128, 0);
        return;
    }
    crBkgnd = CLR_NONE;
    crText = CLR_NONE;
    bDrawWhitespace = FALSE;
}

//--| CCrystalTextView::GetParseCookie|---------------------------------------------------
DWORD CCrystalTextView::GetParseCookie(int nLineIndex)
{
    int nLineCount = GetLineCount();
    if (m_pdwParseCookies == 0)
    {
        m_nParseArraySize = nLineCount;
        m_pdwParseCookies = new DWORD[nLineCount+4];
        memset(m_pdwParseCookies, 0xff, nLineCount * sizeof(DWORD));
    }
    
    if (nLineIndex < 0)
       return 0;
    if (m_pdwParseCookies[nLineIndex] != (DWORD) -1)
       return m_pdwParseCookies[nLineIndex];
    
    int L = nLineIndex;
    while (L >= 0 && m_pdwParseCookies[L] == (DWORD) -1)
          L --;
    L ++;
    
    int nBlocks;
    while (L <= nLineIndex)
    {
        DWORD dwCookie = 0;
        if (L > 0)
           dwCookie = m_pdwParseCookies[L - 1];
        ASSERT(dwCookie != (DWORD) -1);
        m_pdwParseCookies[L] = ParseLine(dwCookie, L, 0, nBlocks);
        ASSERT(m_pdwParseCookies[L] != (DWORD) -1);
        L ++;
    }
    
    return m_pdwParseCookies[nLineIndex];
}

//--| CCrystalTextView::DrawSingleLine|---------------------------------------------------
void CCrystalTextView::DrawSingleLine(CDC *pdc, const CRect &rc, int nLineIndex)
{
    ASSERT(nLineIndex >= -1 && nLineIndex < GetLineCount());
    
    if (nLineIndex == -1)
    {
        // Draw line beyond the text
        pdc->FillSolidRect(rc, GetColor(COLORINDEX_WHITESPACE));
        return;
    }
    
    // Acquire the background color for the current line
    BOOL bDrawWhitespace = FALSE;
    COLORREF crBkgnd, crText;
    GetLineColors(nLineIndex, crBkgnd, crText, bDrawWhitespace);
    if (crBkgnd == CLR_NONE)
       crBkgnd = GetColor(COLORINDEX_BKGND);
    
    int nLength = GetLineLength(nLineIndex);
    if (nLength == 0)
    {
        // Draw the empty line
        CRect rect = rc;
        if ((m_bFocused || m_bShowInactiveSelection) && IsInsideSelBlock(CPoint(0, nLineIndex)))
        {
            pdc->FillSolidRect(rect.left, rect.top, GetCharWidth(), rect.Height(), GetColor(COLORINDEX_SELBKGND));
            rect.left += GetCharWidth();
        }
        pdc->FillSolidRect(rect, bDrawWhitespace ? crBkgnd : GetColor(COLORINDEX_WHITESPACE));
        return;
    }
    
    // Parse the line
    LPCTSTR pszChars = GetLineChars(nLineIndex);
    DWORD dwCookie = GetParseCookie(nLineIndex - 1);
    TEXTBLOCK *pBuf = (TEXTBLOCK *) _alloca(sizeof(TEXTBLOCK) * nLength * 3);
    int nBlocks = 0;
    
    m_pdwParseCookies[nLineIndex] = ParseLine(dwCookie, nLineIndex, pBuf, nBlocks);
    ASSERT(m_pdwParseCookies[nLineIndex] != (DWORD) -1);
    
    // Draw the line text
    CPoint origin(rc.left - m_nOffsetChar * GetCharWidth(), rc.top);
    pdc->SetBkColor(crBkgnd);
    if (crText != CLR_NONE)
       pdc->SetTextColor(crText);
    BOOL bColorSet = FALSE;
    
    if (nBlocks > 0)
    {
        ASSERT(pBuf[0].m_nCharPos >= 0 && pBuf[0].m_nCharPos <= nLength);
        if (crText == CLR_NONE)
           pdc->SetTextColor(GetColor(COLORINDEX_NORMALTEXT));
        pdc->SelectObject(GetFont(GetItalic(COLORINDEX_NORMALTEXT), GetBold(COLORINDEX_NORMALTEXT)));
        DrawLineHelper(pdc, origin, rc, COLORINDEX_NORMALTEXT, pszChars, 0, pBuf[0].m_nCharPos, CPoint(0, nLineIndex));
        for (int I = 0; I < nBlocks - 1; I ++)
        {
            ASSERT(pBuf[I].m_nCharPos >= 0 && pBuf[I].m_nCharPos <= nLength);
            if (crText == CLR_NONE)
               pdc->SetTextColor(GetColor(pBuf[I].m_nColorIndex));
            pdc->SelectObject(GetFont(GetItalic(pBuf[I].m_nColorIndex), GetBold(pBuf[I].m_nColorIndex)));
            DrawLineHelper(pdc, origin, rc, pBuf[I].m_nColorIndex, pszChars,
                          pBuf[I].m_nCharPos, pBuf[I + 1].m_nCharPos - pBuf[I].m_nCharPos,
                          CPoint(pBuf[I].m_nCharPos, nLineIndex));
        }
        ASSERT(pBuf[nBlocks - 1].m_nCharPos >= 0 && pBuf[nBlocks - 1].m_nCharPos <= nLength);
        if (crText == CLR_NONE)
           pdc->SetTextColor(GetColor(pBuf[nBlocks - 1].m_nColorIndex));
        pdc->SelectObject(GetFont(GetItalic(pBuf[nBlocks - 1].m_nColorIndex),
                         GetBold(pBuf[nBlocks - 1].m_nColorIndex)));
        DrawLineHelper(pdc, origin, rc, pBuf[nBlocks - 1].m_nColorIndex, pszChars,
                      pBuf[nBlocks - 1].m_nCharPos, nLength - pBuf[nBlocks - 1].m_nCharPos,
                      CPoint(pBuf[nBlocks - 1].m_nCharPos, nLineIndex));
    }
    else
    {
        if (crText == CLR_NONE)
           pdc->SetTextColor(GetColor(COLORINDEX_NORMALTEXT));
        pdc->SelectObject(GetFont(GetItalic(COLORINDEX_NORMALTEXT), GetBold(COLORINDEX_NORMALTEXT)));
        DrawLineHelper(pdc, origin, rc, COLORINDEX_NORMALTEXT, pszChars, 0, nLength, CPoint(0, nLineIndex));
    }
    
    // Draw whitespaces to the left of the text
    CRect frect = rc;
    if (origin.x > frect.left)
       frect.left = origin.x;
    if (frect.right > frect.left)
    {
        if ((m_bFocused || m_bShowInactiveSelection) && IsInsideSelBlock(CPoint(nLength, nLineIndex)))
        {
            pdc->FillSolidRect(frect.left, frect.top, GetCharWidth(), frect.Height(),
                              GetColor(COLORINDEX_SELBKGND));
            frect.left += GetCharWidth();
        }
        if (frect.right > frect.left)
           pdc->FillSolidRect(frect, bDrawWhitespace ? crBkgnd : GetColor(COLORINDEX_WHITESPACE));
    }
}

//--| CCrystalTextView::GetColor|---------------------------------------------------------
COLORREF CCrystalTextView::GetColor(int nColorIndex)
{
    switch (nColorIndex)
    {
        case COLORINDEX_WHITESPACE:
            case COLORINDEX_BKGND:
            return RGB(0,0,0);
        case COLORINDEX_NORMALTEXT:
            return RGB(192,255,192);
        case COLORINDEX_SELMARGIN:
            return RGB(32,32,32);
        case COLORINDEX_PREPROCESSOR:
            return RGB(0, 128, 192);
        case COLORINDEX_COMMENT:
            return RGB(192, 192, 192);
        // [JRT]: Enabled Support For Numbers...
        case COLORINDEX_NUMBER:
            return RGB(255, 255, 0x00);
        // [JRT]: Support For C/C++ Operators
        case COLORINDEX_OPERATOR:
            return RGB(255,255,0);//;RGB(0x00, 0x00, 0x00);
        case COLORINDEX_KEYWORD:
            return RGB(0, 255, 255);
        case COLORINDEX_SELBKGND:
            return RGB(255, 255, 255);
        case COLORINDEX_SELTEXT:
            return RGB(0, 0, 0);
    }
    return RGB(255, 200, 0);
}

//--| CCrystalTextView::GetLineFlags|-----------------------------------------------------
DWORD CCrystalTextView::GetLineFlags(int nLineIndex)
{
    if (m_pTextBuffer == 0)
       return 0;
    return m_pTextBuffer->GetLineFlags(nLineIndex);
}

//--| CCrystalTextView::DrawMargin|-------------------------------------------------------
void CCrystalTextView::DrawMargin(CDC *pdc, const CRect &rect, int nLineIndex)
{
    if (! m_bSelMargin)
    {
        pdc->FillSolidRect(rect, GetColor(COLORINDEX_BKGND));
        return;
    }
    
    pdc->FillSolidRect(rect, GetColor(COLORINDEX_SELMARGIN));
    
    int nImageIndex = -1;
    if (nLineIndex >= 0)
    {
        DWORD dwLineFlags = GetLineFlags(nLineIndex);
        static const DWORD adwFlags[] =
        {
            LF_EXECUTION,
                LF_BREAKPOINT,
                LF_COMPILATION_ERROR,
                LF_BOOKMARK(1),
                LF_BOOKMARK(2),
                LF_BOOKMARK(3),
                LF_BOOKMARK(4),
                LF_BOOKMARK(5),
                LF_BOOKMARK(6),
                LF_BOOKMARK(7),
                LF_BOOKMARK(8),
                LF_BOOKMARK(9),
                LF_BOOKMARK(0),
                LF_BOOKMARKS,
                LF_INVALID_BREAKPOINT
            };
        for (int I = 0; I <= sizeof(adwFlags) / sizeof(adwFlags[0]); I ++)
        {
            if ((dwLineFlags & adwFlags[I]) != 0)
            {
                nImageIndex = I;
                break;
            }
        }
    }
    
    if (nImageIndex >= 0)
    {
        if (m_pIcons == 0)
        {
            m_pIcons = new CImageList;
            VERIFY(m_pIcons->Create(IDR_MARGIN_ICONS, 16, 16, RGB(255, 255, 255)));
        }
        CPoint pt(rect.left + 2, rect.top + (rect.Height() - 16) / 2);
        VERIFY(m_pIcons->Draw(pdc, nImageIndex, pt, ILD_TRANSPARENT));
    }
}

//--| CCrystalTextView::IsInsideSelBlock|-------------------------------------------------
BOOL CCrystalTextView::IsInsideSelBlock(CPoint ptTextPos)
{
    ASSERT_VALIDTEXTPOS(ptTextPos);
    if (ptTextPos.y < m_ptDrawSelStart.y)
       return FALSE;
    if (ptTextPos.y > m_ptDrawSelEnd.y)
       return FALSE;
    if (ptTextPos.y < m_ptDrawSelEnd.y && ptTextPos.y > m_ptDrawSelStart.y)
       return TRUE;
    if (m_ptDrawSelStart.y < m_ptDrawSelEnd.y)
    {
        if (ptTextPos.y == m_ptDrawSelEnd.y)
           return ptTextPos.x < m_ptDrawSelEnd.x;
        ASSERT(ptTextPos.y == m_ptDrawSelStart.y);
        return ptTextPos.x >= m_ptDrawSelStart.x;
    }
    ASSERT(m_ptDrawSelStart.y == m_ptDrawSelEnd.y);
    return ptTextPos.x >= m_ptDrawSelStart.x && ptTextPos.x < m_ptDrawSelEnd.x;
}

//--| CCrystalTextView::IsInsideSelection|------------------------------------------------
BOOL CCrystalTextView::IsInsideSelection(const CPoint &ptTextPos)
{
    PrepareSelBounds();
    return IsInsideSelBlock(ptTextPos);
}

//--| CCrystalTextView::PrepareSelBounds|-------------------------------------------------
void CCrystalTextView::PrepareSelBounds()
{
    if (m_ptSelStart.y < m_ptSelEnd.y ||
       (m_ptSelStart.y == m_ptSelEnd.y && m_ptSelStart.x < m_ptSelEnd.x))
    {
        m_ptDrawSelStart = m_ptSelStart;
        m_ptDrawSelEnd = m_ptSelEnd;
    }
    else
    {
        m_ptDrawSelStart = m_ptSelEnd;
        m_ptDrawSelEnd = m_ptSelStart;
    }
}

//--| CCrystalTextView::OnDraw|-----------------------------------------------------------
void CCrystalTextView::OnDraw(CDC *pdc)
{
    CRect rcClient;
    GetClientRect(rcClient);
    
    int nLineCount = GetLineCount();
    int nLineHeight = GetLineHeight();
    PrepareSelBounds();
    
    CDC cacheDC;
    VERIFY(cacheDC.CreateCompatibleDC(pdc));
    if (m_pCacheBitmap == 0)
    {
        m_pCacheBitmap = new CBitmap;
        VERIFY(m_pCacheBitmap->CreateCompatibleBitmap(pdc, rcClient.Width(), nLineHeight));
    }
    CBitmap *pOldBitmap = cacheDC.SelectObject(m_pCacheBitmap);
    
    CRect rcLine;
    rcLine = rcClient;
    rcLine.bottom = rcLine.top + nLineHeight;
    CRect rcCacheMargin(0, 0, GetMarginWidth(), nLineHeight);
    CRect rcCacheLine(GetMarginWidth(), 0, rcLine.Width(), nLineHeight);
    
    int nCurrentLine = m_nTopLine;
    while (rcLine.top < rcClient.bottom)
    {
        if (nCurrentLine < nLineCount)
        {
            DrawMargin(&cacheDC, rcCacheMargin, nCurrentLine);
            DrawSingleLine(&cacheDC, rcCacheLine, nCurrentLine);
        }
        else
        {
            DrawMargin(&cacheDC, rcCacheMargin, -1);
            DrawSingleLine(&cacheDC, rcCacheLine, -1);
        }
        
        VERIFY(pdc->BitBlt(rcLine.left, rcLine.top, rcLine.Width(), rcLine.Height(), &cacheDC, 0, 0, SRCCOPY));
        
        nCurrentLine ++;
        rcLine.OffsetRect(0, nLineHeight);
    }
    
    cacheDC.SelectObject(pOldBitmap);
    cacheDC.DeleteDC();
}

//--| CCrystalTextView::ResetView|--------------------------------------------------------
void CCrystalTextView::ResetView()
{
    m_nTopLine = 0;
    m_nOffsetChar = 0;
    m_nLineHeight = -1;
    m_nCharWidth = -1;
    m_nTabSize = 4;
    m_nMaxLineLength = -1;
    m_nScreenLines = -1;
    m_nScreenChars = -1;
    m_nIdealCharPos = -1;
    m_ptAnchor.x = 0;
    m_ptAnchor.y = 0;
    if (m_pIcons != 0)
    {
        delete m_pIcons;
        m_pIcons = 0;
    }
    for (int I = 0; I < 4; I ++)
    {
        if (m_apFonts[I] != 0)
        {
            m_apFonts[I]->DeleteObject();
            delete m_apFonts[I];
            m_apFonts[I] = 0;
        }
    }
    if (m_pdwParseCookies != 0)
    {
        delete[] m_pdwParseCookies;
        m_pdwParseCookies = 0;
    }
    if (m_pnActualLineLength != 0)
    {
        delete[] m_pnActualLineLength;
        m_pnActualLineLength = 0;
    }
    m_nParseArraySize = 0;
    m_nActualLengthArraySize = 0;
    m_ptCursorPos.x = 0;
    m_ptCursorPos.y = 0;
    m_ptSelStart = m_ptSelEnd = m_ptCursorPos;
    m_bDragSelection = FALSE;
    m_bVertScrollBarLocked = FALSE;
    m_bHorzScrollBarLocked = FALSE;
    if (::IsWindow(m_hWnd))
       UpdateCaret();
    m_bLastSearch = FALSE;
    m_bShowInactiveSelection = FALSE;
    m_bPrintHeader = FALSE;
    m_bPrintFooter = TRUE;
    
    m_bBookmarkExist  = FALSE; // More bookmarks
    m_bMultipleSearch = FALSE; // More search
}

//--|    CCrystalTextView::ReserCursors|--------------------------------------------------
void    CCrystalTextView::ReserCursors()
{
    SetCaretPos(CPoint(0,0));
    m_ptCursorPos.x=m_ptCursorPos.y=0;
    //OnClearAllBookmarks();
}

//--| CCrystalTextView::UpdateCaret|------------------------------------------------------
void CCrystalTextView::UpdateCaret()
{
    ASSERT_VALIDTEXTPOS(m_ptCursorPos);
    if (m_bFocused && ! m_bCursorHidden &&
       CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x) >= m_nOffsetChar)
    {
        CreateSolidCaret(2, GetLineHeight());
        SetCaretPos(TextToClient(m_ptCursorPos));
        ShowCaret();
    }
    else
    {
        HideCaret();
    }
}

//--| CCrystalTextView::GetTabSize|-------------------------------------------------------
int CCrystalTextView::GetTabSize()
{
    ASSERT(m_nTabSize >= 0 && m_nTabSize <= 64);
    return m_nTabSize;
}

//--| CCrystalTextView::SetTabSize|-------------------------------------------------------
void CCrystalTextView::SetTabSize(int nTabSize)
{
    ASSERT(nTabSize >= 0 && nTabSize <= 64);
    if (m_nTabSize != nTabSize)
    {
        m_nTabSize = nTabSize;
        if (m_pnActualLineLength != 0)
        {
            delete[] m_pnActualLineLength;
            m_pnActualLineLength = 0;
        }
        m_nActualLengthArraySize = 0;
        m_nMaxLineLength = -1;
        RecalcHorzScrollBar();
        Invalidate();
        UpdateCaret();
    }
}

//--| *CCrystalTextView::GetFont|---------------------------------------------------------
CFont *CCrystalTextView::GetFont(BOOL bItalic /*= FALSE*/, BOOL bBold /*= FALSE*/)
{
    int nIndex = 0;
    if (bBold)
       nIndex |= 1;
    if (bItalic)
       nIndex |= 2;
    
    if (m_apFonts[nIndex] == 0)
    {
        m_apFonts[nIndex] = new CFont;
        m_lfBaseFont.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
        m_lfBaseFont.lfItalic = (BYTE) bItalic;
        if (! m_apFonts[nIndex]->CreateFontIndirect(&m_lfBaseFont))
        {
            delete m_apFonts[nIndex];
            m_apFonts[nIndex] = 0;
            return CView::GetFont();
        }
        
    }
    return m_apFonts[nIndex];
}

//--| CCrystalTextView::CalcLineCharDim|--------------------------------------------------
void CCrystalTextView::CalcLineCharDim()
{
    CDC *pdc = GetDC();
    CFont *pOldFont = pdc->SelectObject(GetFont());
    CSize szCharExt = pdc->GetTextExtent(_T("X"));
    m_nLineHeight = szCharExt.cy;
    if (m_nLineHeight < 1)
       m_nLineHeight = 1;
    m_nCharWidth = szCharExt.cx;
    /*
        TEXTMETRIC tm;
    if (pdc->GetTextMetrics(&tm))
       m_nCharWidth -= tm.tmOverhang;
    */
        pdc->SelectObject(pOldFont);
    ReleaseDC(pdc);
}

//--| CCrystalTextView::GetLineHeight|----------------------------------------------------
int CCrystalTextView::GetLineHeight()
{
    if (m_nLineHeight == -1)
       CalcLineCharDim();
    return m_nLineHeight;
}

//--| CCrystalTextView::GetCharWidth|-----------------------------------------------------
int CCrystalTextView::GetCharWidth()
{
    if (m_nCharWidth == -1)
       CalcLineCharDim();
    return m_nCharWidth;
}

//--| CCrystalTextView::GetMaxLineLength|-------------------------------------------------
int CCrystalTextView::GetMaxLineLength()
{
    if (m_nMaxLineLength == -1)
    {
        m_nMaxLineLength = 0;
        int nLineCount = GetLineCount();
        for (int I = 0; I < nLineCount; I ++)
        {
            int nActualLength = GetLineActualLength(I);
            if (m_nMaxLineLength < nActualLength)
               m_nMaxLineLength = nActualLength;
        }
    }
    return m_nMaxLineLength;
}

//--| *CCrystalTextView::GetSiblingView|--------------------------------------------------
CCrystalTextView *CCrystalTextView::GetSiblingView(int nRow, int nCol)
{
    CSplitterWnd *pSplitter = GetParentSplitter(this, FALSE);
    if (pSplitter == 0)
       return 0;
    CWnd *pWnd = CWnd::FromHandlePermanent(
                                          ::GetDlgItem(pSplitter->m_hWnd, pSplitter->IdFromRowCol(nRow, nCol)));
    if (pWnd == 0 || ! pWnd->IsKindOf(RUNTIME_CLASS(CCrystalTextView)))
       return 0;
    return (CCrystalTextView *) pWnd;
}

//--| CCrystalTextView::OnInitialUpdate|--------------------------------------------------
void CCrystalTextView::OnInitialUpdate()
{
    CView::OnInitialUpdate();
    
    AttachToBuffer(0);
    
    CSplitterWnd *pSplitter = GetParentSplitter(this, FALSE);
    if (pSplitter != 0)
    {
        // See CSplitterWnd::IdFromRowCol() implementation
        int nRow = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) / 16;
        int nCol = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) % 16;
        ASSERT(nRow >= 0 && nRow < pSplitter->GetRowCount());
        ASSERT(nCol >= 0 && nCol < pSplitter->GetColumnCount());
        
        if (nRow > 0)
        {
            CCrystalTextView *pSiblingView = GetSiblingView(0, nCol);
            if (pSiblingView != 0 && pSiblingView != this)
            {
                m_nOffsetChar = pSiblingView->m_nOffsetChar;
                ASSERT(m_nOffsetChar >= 0 && m_nOffsetChar <= GetMaxLineLength());
            }
        }
        
        if (nCol > 0)
        {
            CCrystalTextView *pSiblingView = GetSiblingView(nRow, 0);
            if (pSiblingView != 0 && pSiblingView != this)
            {
                m_nTopLine = pSiblingView->m_nTopLine;
                ASSERT(m_nTopLine >= 0 && m_nTopLine < GetLineCount());
            }
        }
    }
}



//--| CCrystalTextView::OnPrepareDC|------------------------------------------------------
void CCrystalTextView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
    CView::OnPrepareDC(pDC, pInfo);
    
    if (pInfo != 0)
    {
        pInfo->m_bContinuePrinting = TRUE;
        if (m_pnPages != 0 && (int) pInfo->m_nCurPage > m_nPrintPages)
           pInfo->m_bContinuePrinting = FALSE;
    }
}

//--| CCrystalTextView::OnPreparePrinting|------------------------------------------------
BOOL CCrystalTextView::OnPreparePrinting(CPrintInfo* pInfo)
{
    return DoPreparePrinting(pInfo);
}

//--| CCrystalTextView::PrintLineHeight|--------------------------------------------------
int CCrystalTextView::PrintLineHeight(CDC *pdc, int nLine)
{
    ASSERT(nLine >= 0 && nLine < GetLineCount());
    ASSERT(m_nPrintLineHeight > 0);
    int nLength = GetLineLength(nLine);
    if (nLength == 0)
       return m_nPrintLineHeight;
    
    CString line;
    LPCTSTR pszChars = GetLineChars(nLine);
    ExpandChars(pszChars, 0, nLength, line);
    CRect rcPrintArea = m_rcPrintArea;
    pdc->DrawText(line, &rcPrintArea, DT_LEFT | DT_NOPREFIX | DT_TOP | DT_WORDBREAK | DT_CALCRECT);
    return rcPrintArea.Height();
}

//--| CCrystalTextView::GetPrintHeaderText|-----------------------------------------------
void CCrystalTextView::GetPrintHeaderText(int nPageNum, CString &text)
{
    ASSERT(m_bPrintHeader);
    text = _T("");
}

//--| CCrystalTextView::GetPrintFooterText|-----------------------------------------------
void CCrystalTextView::GetPrintFooterText(int nPageNum, CString &text)
{
    ASSERT(m_bPrintFooter);
    text.Format(_T("Page %d/%d"), nPageNum, m_nPrintPages);
}

//--| CCrystalTextView::PrintHeader|------------------------------------------------------
void CCrystalTextView::PrintHeader(CDC *pdc, int nPageNum)
{
    CRect rcHeader = m_rcPrintArea;
    rcHeader.bottom = rcHeader.top;
    rcHeader.top -= (m_nPrintLineHeight + m_nPrintLineHeight / 2);
    
    CString text;
    GetPrintHeaderText(nPageNum, text);
    if (! text.IsEmpty())
       pdc->DrawText(text, &rcHeader, DT_CENTER | DT_NOPREFIX | DT_TOP | DT_SINGLELINE);
}

//--| CCrystalTextView::PrintFooter|------------------------------------------------------
void CCrystalTextView::PrintFooter(CDC *pdc, int nPageNum)
{
    CRect rcFooter = m_rcPrintArea;
    rcFooter.top = rcFooter.bottom;
    rcFooter.bottom += (m_nPrintLineHeight + m_nPrintLineHeight / 2);
    
    CString text;
    GetPrintFooterText(nPageNum, text);
    if (! text.IsEmpty())
       pdc->DrawText(text, &rcFooter, DT_CENTER | DT_NOPREFIX | DT_BOTTOM | DT_SINGLELINE);
}

//--| CCrystalTextView::RecalcPageLayouts|------------------------------------------------
void CCrystalTextView::RecalcPageLayouts(CDC *pdc, CPrintInfo *pInfo)
{
    m_ptPageArea = pInfo->m_rectDraw;
    m_ptPageArea.NormalizeRect();
    
    m_nPrintLineHeight = pdc->GetTextExtent(_T("X")).cy;
    
    m_rcPrintArea = m_ptPageArea;
    CSize szTopLeft, szBottomRight;
    CWinApp *pApp = AfxGetApp();
    ASSERT(pApp != 0);
    szTopLeft.cx = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_LEFT, DEFAULT_PRINT_MARGIN);
    szBottomRight.cx = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_RIGHT, DEFAULT_PRINT_MARGIN);
    szTopLeft.cy = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_TOP, DEFAULT_PRINT_MARGIN);
    szBottomRight.cy = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_BOTTOM, DEFAULT_PRINT_MARGIN);
    pdc->HIMETRICtoLP(&szTopLeft);
    pdc->HIMETRICtoLP(&szBottomRight);
    m_rcPrintArea.left += szTopLeft.cx;
    m_rcPrintArea.right -= szBottomRight.cx;
    m_rcPrintArea.top += szTopLeft.cy;
    m_rcPrintArea.bottom -= szBottomRight.cy;
    if (m_bPrintHeader)
       m_rcPrintArea.top += m_nPrintLineHeight + m_nPrintLineHeight / 2;
    if (m_bPrintFooter)
       m_rcPrintArea.bottom += m_nPrintLineHeight + m_nPrintLineHeight / 2;
    
    int nLimit = 32;
    m_nPrintPages = 1;
    m_pnPages = new int[nLimit];
    m_pnPages[0] = 0;
    
    int nLineCount = GetLineCount();
    int nLine = 1;
    int y = m_rcPrintArea.top + PrintLineHeight(pdc, 0);
    while (nLine < nLineCount)
    {
        int nHeight = PrintLineHeight(pdc, nLine);
        if (y + nHeight <= m_rcPrintArea.bottom)
        {
            y += nHeight;
        }
        else
        {
            ASSERT(nLimit >= m_nPrintPages);
            if (nLimit <= m_nPrintPages)
            {
                nLimit += 32;
                int *pnNewPages = new int[nLimit];
                memcpy(pnNewPages, m_pnPages, sizeof(int) * m_nPrintPages);
                delete[] m_pnPages;
                m_pnPages = pnNewPages;
            }
            ASSERT(nLimit > m_nPrintPages);
            m_pnPages[m_nPrintPages ++] = nLine;
            y = m_rcPrintArea.top + nHeight;
        }
        nLine ++;
    }
}

//--| CCrystalTextView::OnBeginPrinting|--------------------------------------------------
void CCrystalTextView::OnBeginPrinting(CDC *pdc, CPrintInfo *pInfo)
{
    ASSERT(m_pnPages == 0);
    ASSERT(m_pPrintFont == 0);
    CFont *pDisplayFont = GetFont();
    
    LOGFONT lf;
    pDisplayFont->GetLogFont(&lf);
    
    CDC *pDisplayDC = GetDC();
    lf.lfHeight = MulDiv(lf.lfHeight, pdc->GetDeviceCaps(LOGPIXELSY), pDisplayDC->GetDeviceCaps(LOGPIXELSY) * 2);
    lf.lfWidth = MulDiv(lf.lfWidth, pdc->GetDeviceCaps(LOGPIXELSX), pDisplayDC->GetDeviceCaps(LOGPIXELSX) * 2);
    ReleaseDC(pDisplayDC);
    
    m_pPrintFont = new CFont;
    if (! m_pPrintFont->CreateFontIndirect(&lf))
    {
        delete m_pPrintFont;
        m_pPrintFont = 0;
        return;
    }
    
    pdc->SelectObject(m_pPrintFont);
}

//--| CCrystalTextView::OnEndPrinting|----------------------------------------------------
void CCrystalTextView::OnEndPrinting(CDC *pdc, CPrintInfo *pInfo)
{
    if (m_pPrintFont != 0)
    {
        delete m_pPrintFont;
        m_pPrintFont = 0;
    }
    if (m_pnPages != 0)
    {
        delete m_pnPages;
        m_pnPages = 0;
    }
    m_nPrintPages = 0;
    m_nPrintLineHeight = 0;
}

//--| CCrystalTextView::OnPrint|----------------------------------------------------------
void CCrystalTextView::OnPrint(CDC* pdc, CPrintInfo* pInfo)
{
    if (m_pnPages == 0)
    {
        RecalcPageLayouts(pdc, pInfo);
        ASSERT(m_pnPages != 0);
    }
    
    ASSERT(pInfo->m_nCurPage >= 1 && (int) pInfo->m_nCurPage <= m_nPrintPages);
    int nLine = m_pnPages[pInfo->m_nCurPage - 1];
    int nEndLine = GetLineCount();
    if ((int) pInfo->m_nCurPage < m_nPrintPages)
       nEndLine = m_pnPages[pInfo->m_nCurPage];
    TRACE(_T("Printing page %d of %d, lines %d - %d\n"), pInfo->m_nCurPage, m_nPrintPages,
         nLine, nEndLine - 1);
    
    if (m_bPrintHeader)
       PrintHeader(pdc, pInfo->m_nCurPage);
    if (m_bPrintFooter)
       PrintFooter(pdc, pInfo->m_nCurPage);
    
    int y = m_rcPrintArea.top;
    for (; nLine < nEndLine; nLine ++)
    {
        int nLineLength = GetLineLength(nLine);
        if (nLineLength == 0)
        {
            y += m_nPrintLineHeight;
            continue;
        }
        
        CRect rcPrintRect = m_rcPrintArea;
        rcPrintRect.top = y;
        LPCTSTR pszChars = GetLineChars(nLine);
        CString line;
        ExpandChars(pszChars, 0, nLineLength, line);
        y += pdc->DrawText(line, &rcPrintRect, DT_LEFT | DT_NOPREFIX | DT_TOP | DT_WORDBREAK);
    }
}



//--| CCrystalTextView::GetLineCount|-----------------------------------------------------
int CCrystalTextView::GetLineCount()
{
    if (m_pTextBuffer == 0)
       return 1;  // Single empty line
    int nLineCount = m_pTextBuffer->GetLineCount();
    ASSERT(nLineCount > 0);
    return nLineCount;
}

//--| CCrystalTextView::GetLineLength|----------------------------------------------------
int CCrystalTextView::GetLineLength(int nLineIndex)
{
    if (m_pTextBuffer == 0)
       return 0;
    return m_pTextBuffer->GetLineLength(nLineIndex);
}

//--| CCrystalTextView::GetLineChars|-----------------------------------------------------
LPCTSTR CCrystalTextView::GetLineChars(int nLineIndex)
{
    if (m_pTextBuffer == 0)
       return 0;
    return m_pTextBuffer->GetLineChars(nLineIndex);
}

//--| CCrystalTextView::AttachToBuffer|---------------------------------------------------
void CCrystalTextView::AttachToBuffer(CCrystalTextBuffer *pBuf /*= 0*/)
{
    if (m_pTextBuffer != 0)
       m_pTextBuffer->RemoveView(this);
    if (pBuf == 0)
    {
        pBuf = LocateTextBuffer();
        // ...
    }
    m_pTextBuffer = pBuf;
    if (m_pTextBuffer != 0)
       m_pTextBuffer->AddView(this);
    ResetView();
    
    // Init scrollbars
    CScrollBar *pVertScrollBarCtrl = GetScrollBarCtrl(SB_VERT);
    if (pVertScrollBarCtrl != 0)
       pVertScrollBarCtrl->EnableScrollBar(GetScreenLines() >= GetLineCount() ?
       ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
    CScrollBar *pHorzScrollBarCtrl = GetScrollBarCtrl(SB_HORZ);
    if (pHorzScrollBarCtrl != 0)
       pHorzScrollBarCtrl->EnableScrollBar(GetScreenChars() >= GetMaxLineLength() ?
       ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
    
    // Update scrollbars
    RecalcVertScrollBar();
    RecalcHorzScrollBar();
}

//--| CCrystalTextView::DetachFromBuffer|-------------------------------------------------
void CCrystalTextView::DetachFromBuffer()
{
    if (m_pTextBuffer != 0)
    {
        m_pTextBuffer->RemoveView(this);
        m_pTextBuffer = 0;
        ResetView();
    }
}

//--| CCrystalTextView::GetScreenLines|---------------------------------------------------
int CCrystalTextView::GetScreenLines()
{
    if (m_nScreenLines == -1)
    {
        CRect rect;
        GetClientRect(&rect);
        m_nScreenLines = rect.Height() / GetLineHeight();
    }
    return m_nScreenLines;
}

//--| CCrystalTextView::GetItalic|--------------------------------------------------------
BOOL CCrystalTextView::GetItalic(int nColorIndex)
{
    return FALSE;
}

//--| CCrystalTextView::GetBold|----------------------------------------------------------
BOOL CCrystalTextView::GetBold(int nColorIndex)
{
    return FALSE;
}

//--| CCrystalTextView::GetScreenChars|---------------------------------------------------
int CCrystalTextView::GetScreenChars()
{
    if (m_nScreenChars == -1)
    {
        CRect rect;
        GetClientRect(&rect);
        m_nScreenChars = (rect.Width() - GetMarginWidth()) / GetCharWidth();
    }
    return m_nScreenChars;
}

//--| CCrystalTextView::OnDestroy|--------------------------------------------------------
void CCrystalTextView::OnDestroy()
{
    DetachFromBuffer();
    m_hAccel = 0;
    
    CView::OnDestroy();
    
    for (int I = 0; I < 4; I ++)
    {
        if (m_apFonts[I] != 0)
        {
            m_apFonts[I]->DeleteObject();
            delete m_apFonts[I];
            m_apFonts[I] = 0;
        }
    }
    if (m_pCacheBitmap != 0)
    {
        delete m_pCacheBitmap;
        m_pCacheBitmap = 0;
    }
}

//--| CCrystalTextView::OnEraseBkgnd|-----------------------------------------------------
BOOL CCrystalTextView::OnEraseBkgnd(CDC *pdc)
{
    return 0;
}

//--| CCrystalTextView::OnSize|-----------------------------------------------------------
void CCrystalTextView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    
    if (m_pCacheBitmap != 0)
    {
        m_pCacheBitmap->DeleteObject();
        delete m_pCacheBitmap;
        m_pCacheBitmap = 0;
    }
    m_nScreenLines = -1;
    m_nScreenChars = -1;
    RecalcVertScrollBar();
    RecalcHorzScrollBar();
}

//--| CCrystalTextView::UpdateSiblingScrollPos|-------------------------------------------
void CCrystalTextView::UpdateSiblingScrollPos(BOOL bHorz)
{
    CSplitterWnd *pSplitterWnd = GetParentSplitter(this, FALSE);
    if (pSplitterWnd != 0)
    {
        // See CSplitterWnd::IdFromRowCol() implementation for details
        int nCurrentRow = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) / 16;
        int nCurrentCol = (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) % 16;
        ASSERT(nCurrentRow >= 0 && nCurrentRow < pSplitterWnd->GetRowCount());
        ASSERT(nCurrentCol >= 0 && nCurrentCol < pSplitterWnd->GetColumnCount());
        
        if (bHorz)
        {
            int nCols = pSplitterWnd->GetColumnCount();
            for (int nCol = 0; nCol < nCols; nCol ++)
            {
                if (nCol != nCurrentCol) // We don't need to update ourselves
                {
                    CCrystalTextView *pSiblingView = GetSiblingView(nCurrentRow, nCol);
                    if (pSiblingView != 0)
                       pSiblingView->OnUpdateSibling(this, TRUE);
                }
            }
        }
        else
        {
            int nRows = pSplitterWnd->GetRowCount();
            for (int nRow = 0; nRow < nRows; nRow ++)
            {
                if (nRow != nCurrentRow) // We don't need to update ourselves
                {
                    CCrystalTextView *pSiblingView = GetSiblingView(nRow, nCurrentCol);
                    if (pSiblingView != 0)
                       pSiblingView->OnUpdateSibling(this, FALSE);
                }
            }
        }
    }
}

//--| CCrystalTextView::OnUpdateSibling|--------------------------------------------------
void CCrystalTextView::OnUpdateSibling(CCrystalTextView *pUpdateSource, BOOL bHorz)
{
    if (pUpdateSource != this)
    {
        ASSERT(pUpdateSource != 0);
        ASSERT_KINDOF(CCrystalTextView, pUpdateSource);
        if (bHorz)
        {
            ASSERT(pUpdateSource->m_nTopLine >= 0);
            ASSERT(pUpdateSource->m_nTopLine < GetLineCount());
            if (pUpdateSource->m_nTopLine != m_nTopLine)
            {
                ScrollToLine(pUpdateSource->m_nTopLine, TRUE, FALSE);
                UpdateCaret();
            }
        }
        else
        {
            ASSERT(pUpdateSource->m_nOffsetChar >= 0);
            ASSERT(pUpdateSource->m_nOffsetChar < GetMaxLineLength());
            if (pUpdateSource->m_nOffsetChar != m_nOffsetChar)
            {
                ScrollToChar(pUpdateSource->m_nOffsetChar, TRUE, FALSE);
                UpdateCaret();
            }
        }
    }
}

//--| CCrystalTextView::RecalcVertScrollBar|----------------------------------------------
void CCrystalTextView::RecalcVertScrollBar(BOOL bPositionOnly /*= FALSE*/)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    if (bPositionOnly)
    {
        si.fMask = SIF_POS;
        si.nPos = m_nTopLine;
    }
    else
    {
        if (GetScreenLines() >= GetLineCount() && m_nTopLine > 0)
        {
            m_nTopLine = 0;
            Invalidate();
            UpdateCaret();
        }
        si.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_POS | SIF_RANGE;
        si.nMin = 0;
        si.nMax = GetLineCount() - 1;
        si.nPage = GetScreenLines();
        si.nPos = m_nTopLine;
    }
    VERIFY(SetScrollInfo(SB_VERT, &si));
}

//--| CCrystalTextView::OnVScroll|--------------------------------------------------------
void CCrystalTextView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CView::OnVScroll(nSBCode, nPos, pScrollBar);
    
    // Note we cannot use nPos because of its 16-bit nature
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    VERIFY(GetScrollInfo(SB_VERT, &si));
    
    int nPageLines = GetScreenLines();
    int nLineCount = GetLineCount();
    
    int nNewTopLine;
    BOOL bDisableSmooth = TRUE;
    switch (nSBCode)
    {
        case SB_TOP:
            nNewTopLine = 0;
        bDisableSmooth = FALSE;
        break;
        case SB_BOTTOM:
            nNewTopLine = nLineCount - nPageLines + 1;
        bDisableSmooth = FALSE;
        break;
        case SB_LINEUP:
            nNewTopLine = m_nTopLine - 1;
        break;
        case SB_LINEDOWN:
            nNewTopLine = m_nTopLine + 1;
        break;
        case SB_PAGEUP:
            nNewTopLine = m_nTopLine - si.nPage + 1;
        bDisableSmooth = FALSE;
        break;
        case SB_PAGEDOWN:
            nNewTopLine = m_nTopLine + si.nPage - 1;
        bDisableSmooth = FALSE;
        break;
        case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
            nNewTopLine = si.nTrackPos;
        break;
        default:
            return;
    }
    
    if (nNewTopLine < 0)
       nNewTopLine = 0;
    if (nNewTopLine >= nLineCount)
       nNewTopLine = nLineCount - 1;
    ScrollToLine(nNewTopLine, bDisableSmooth);
}

//--| CCrystalTextView::RecalcHorzScrollBar|----------------------------------------------
void CCrystalTextView::RecalcHorzScrollBar(BOOL bPositionOnly /*= FALSE*/)
{
    // Again, we cannot use nPos because it's 16-bit
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    if (bPositionOnly)
    {
        si.fMask = SIF_POS;
        si.nPos = m_nOffsetChar;
    }
    else
    {
        if (GetScreenChars() >= GetMaxLineLength() && m_nOffsetChar > 0)
        {
            m_nOffsetChar = 0;
            Invalidate();
            UpdateCaret();
        }
        si.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_POS | SIF_RANGE;
        si.nMin = 0;
        si.nMax = GetMaxLineLength() - 1;
        si.nPage = GetScreenChars();
        si.nPos = m_nOffsetChar;
    }
    VERIFY(SetScrollInfo(SB_HORZ, &si));
}

//--| CCrystalTextView::OnHScroll|--------------------------------------------------------
void CCrystalTextView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CView::OnHScroll(nSBCode, nPos, pScrollBar);
    
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    VERIFY(GetScrollInfo(SB_HORZ, &si));
    
    int nPageChars = GetScreenChars();
    int nMaxLineLength = GetMaxLineLength();
    
    int nNewOffset;
    switch (nSBCode)
    {
        case SB_LEFT:
            nNewOffset = 0;
        break;
        case SB_BOTTOM:
            nNewOffset = nMaxLineLength - nPageChars + 1;
        break;
        case SB_LINEUP:
            nNewOffset = m_nOffsetChar - 1;
        break;
        case SB_LINEDOWN:
            nNewOffset = m_nOffsetChar + 1;
        break;
        case SB_PAGEUP:
            nNewOffset = m_nOffsetChar - si.nPage + 1;
        break;
        case SB_PAGEDOWN:
            nNewOffset = m_nOffsetChar + si.nPage - 1;
        break;
        case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
            nNewOffset = si.nTrackPos;
        break;
        default:
            return;
    }
    
    if (nNewOffset >= nMaxLineLength)
       nNewOffset = nMaxLineLength - 1;
    if (nNewOffset < 0)
       nNewOffset = 0;
    ScrollToChar(nNewOffset, TRUE);
    UpdateCaret();
}

//--| CCrystalTextView::OnSetCursor|------------------------------------------------------
BOOL CCrystalTextView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT)
    {
        CPoint pt;
        ::GetCursorPos(&pt);
        ScreenToClient(&pt);
        if (pt.x < GetMarginWidth())
        {
            ::SetCursor(::LoadCursor(GetResourceHandle(), MAKEINTRESOURCE(IDR_MARGIN_CURSOR)));
        }
        else
        {
            CPoint ptText = ClientToText(pt);
            PrepareSelBounds();
            if (IsInsideSelBlock(ptText))
            {
                // [JRT]: Support For Disabling Drag and Drop...
                if (!m_bDisableDragAndDrop)    // If Drag And Drop Not Disabled
                ::SetCursor(::LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW))); // Set To Arrow Cursor
            }
            else
                ::SetCursor(::LoadCursor(0, MAKEINTRESOURCE(IDC_IBEAM)));
        }
        return TRUE;
    }
    return CView::OnSetCursor(pWnd, nHitTest, message);
}

//--| CCrystalTextView::ClientToText|-----------------------------------------------------
CPoint CCrystalTextView::ClientToText(const CPoint &point)
{
    int nLineCount = GetLineCount();
    
    CPoint pt;
    pt.y = m_nTopLine + point.y / GetLineHeight();
    if (pt.y >= nLineCount)
       pt.y = nLineCount - 1;
    if (pt.y < 0)
       pt.y = 0;
    
    int nLength = 0;
    LPCTSTR pszLine = 0;
    if (pt.y >= 0 && pt.y < nLineCount)
    {
        nLength = GetLineLength(pt.y);
        pszLine = GetLineChars(pt.y);
    }
    
    int nPos = m_nOffsetChar + (point.x - GetMarginWidth()) / GetCharWidth();
    if (nPos < 0)
       nPos = 0;
    
    int nIndex = 0, nCurPos = 0;
    int nTabSize = GetTabSize();
    while (nIndex < nLength)
    {
        if (pszLine[nIndex] == _T('\t'))
           nCurPos += (nTabSize - nCurPos % nTabSize);
        else
            nCurPos ++;
        
        if (nCurPos > nPos)
           break;
        
        nIndex ++;
    }
    
    ASSERT(nIndex >= 0 && nIndex <= nLength);
    pt.x = nIndex;
    return pt;
}

#ifdef _DEBUG
//--| CCrystalTextView::AssertValidTextPos|-----------------------------------------------
void CCrystalTextView::AssertValidTextPos(const CPoint &point)
{
    if (GetLineCount() > 0)
    {
        ASSERT(m_nTopLine >= 0 && m_nOffsetChar >= 0);
        ASSERT(point.y >= 0 && point.y < GetLineCount());
        ASSERT(point.x >= 0 && point.x <= GetLineLength(point.y));
    }
}
#endif

//--| CCrystalTextView::TextToClient|-----------------------------------------------------
CPoint CCrystalTextView::TextToClient(const CPoint &point)
{
    ASSERT_VALIDTEXTPOS(point);
    int nLength = GetLineLength(point.y);
    LPCTSTR pszLine = GetLineChars(point.y);
    
    CPoint pt;
    pt.y = (point.y - m_nTopLine) * GetLineHeight();
    pt.x = 0;
    int nTabSize = GetTabSize();
    for (int nIndex = 0; nIndex < point.x; nIndex ++)
    {
        if (pszLine[nIndex] == _T('\t'))
           pt.x += (nTabSize - pt.x % nTabSize);
        else
            pt.x ++;
    }
    
    pt.x = (pt.x - m_nOffsetChar) * GetCharWidth() + GetMarginWidth();
    return pt;
}

//--| CCrystalTextView::InvalidateLines|--------------------------------------------------
void CCrystalTextView::InvalidateLines(int nLine1, int nLine2, BOOL bInvalidateMargin /*= FALSE*/)
{
    bInvalidateMargin = TRUE;
    if (nLine2 == -1)
    {
        CRect rcInvalid;
        GetClientRect(&rcInvalid);
        if (! bInvalidateMargin)
           rcInvalid.left += GetMarginWidth();
        rcInvalid.top = (nLine1 - m_nTopLine) * GetLineHeight();
        InvalidateRect(&rcInvalid, FALSE);
    }
    else
    {
        if (nLine2 < nLine1)
        {
            int nTemp = nLine1;
            nLine1 = nLine2;
            nLine2 = nTemp;
        }
        CRect rcInvalid;
        GetClientRect(&rcInvalid);
        if (! bInvalidateMargin)
           rcInvalid.left += GetMarginWidth();
        rcInvalid.top = (nLine1 - m_nTopLine) * GetLineHeight();
        rcInvalid.bottom = (nLine2 - m_nTopLine + 1) * GetLineHeight();
        InvalidateRect(&rcInvalid, FALSE);
    }
}

//--| CCrystalTextView::SetSelection|-----------------------------------------------------
void CCrystalTextView::SetSelection(const CPoint &ptStart, const CPoint &ptEnd)
{
    ASSERT_VALIDTEXTPOS(ptStart);
    ASSERT_VALIDTEXTPOS(ptEnd);
    if (m_ptSelStart == ptStart)
    {
        if (m_ptSelEnd != ptEnd)
           InvalidateLines(ptEnd.y, m_ptSelEnd.y);
    }
    else
    {
        InvalidateLines(ptStart.y, ptEnd.y);
        InvalidateLines(m_ptSelStart.y, m_ptSelEnd.y);
    }
    m_ptSelStart = ptStart;
    m_ptSelEnd = ptEnd;
}

//--| CCrystalTextView::AdjustTextPoint|--------------------------------------------------
void CCrystalTextView::AdjustTextPoint(CPoint &point)
{
    point.x += GetCharWidth() / 2; //todo
}

//--| CCrystalTextView::OnSetFocus|-------------------------------------------------------
void CCrystalTextView::OnSetFocus(CWnd* pOldWnd)
{
    CView::OnSetFocus(pOldWnd);
    
    m_bFocused = TRUE;
    if (m_ptSelStart != m_ptSelEnd)
       InvalidateLines(m_ptSelStart.y, m_ptSelEnd.y);
    UpdateCaret();
}

//--| CCrystalTextView::ParseLine|--------------------------------------------------------
DWORD CCrystalTextView::ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems)
{
    return 0;
}

//--| CCrystalTextView::CalculateActualOffset|--------------------------------------------
int CCrystalTextView::CalculateActualOffset(int nLineIndex, int nCharIndex)
{
    int nLength = GetLineLength(nLineIndex);
    ASSERT(nCharIndex >= 0 && nCharIndex <= nLength);
    LPCTSTR pszChars = GetLineChars(nLineIndex);
    int nOffset = 0;
    int nTabSize = GetTabSize();
    for (int I = 0; I < nCharIndex; I ++)
    {
        if (pszChars[I] == _T('\t'))
           nOffset += (nTabSize - nOffset % nTabSize);
        else
            nOffset ++;
    }
    return nOffset;
}

//--| CCrystalTextView::ApproxActualOffset|-----------------------------------------------
int CCrystalTextView::ApproxActualOffset(int nLineIndex, int nOffset)
{
    if (nOffset == 0)
       return 0;
    
    int nLength = GetLineLength(nLineIndex);
    LPCTSTR pszChars = GetLineChars(nLineIndex);
    int nCurrentOffset = 0;
    int nTabSize = GetTabSize();
    for (int I = 0; I < nLength; I ++)
    {
        if (pszChars[I] == _T('\t'))
           nCurrentOffset += (nTabSize - nCurrentOffset % nTabSize);
        else
            nCurrentOffset ++;
        if (nCurrentOffset >= nOffset)
        {
            if (nOffset <= nCurrentOffset - nTabSize / 2)
               return I;
            return I + 1;
        }
    }
    return nLength;
}

//--| CCrystalTextView::EnsureVisible|----------------------------------------------------
void CCrystalTextView::EnsureVisible(CPoint pt)
{
    // Scroll vertically
    int nLineCount = GetLineCount();
    int nNewTopLine = m_nTopLine;
    if (pt.y >= nNewTopLine + GetScreenLines())
    {
        nNewTopLine = pt.y - GetScreenLines() + 1;
    }
    if (pt.y < nNewTopLine)
    {
        nNewTopLine = pt.y;
    }
    
    if (nNewTopLine < 0)
       nNewTopLine = 0;
    if (nNewTopLine >= nLineCount)
       nNewTopLine = nLineCount - 1;
    
    if (m_nTopLine != nNewTopLine)
    {
        ScrollToLine(nNewTopLine);
        UpdateSiblingScrollPos(TRUE);
    }
    
    // Scroll horizontally
    int nActualPos = CalculateActualOffset(pt.y, pt.x);
    int nNewOffset = m_nOffsetChar;
    if (nActualPos > nNewOffset + GetScreenChars())
    {
        nNewOffset = nActualPos - GetScreenChars();
    }
    if (nActualPos < nNewOffset)
    {
        nNewOffset = nActualPos;
    }
    
    if (nNewOffset >= GetMaxLineLength())
       nNewOffset = GetMaxLineLength() - 1;
    if (nNewOffset < 0)
       nNewOffset = 0;
    
    if (m_nOffsetChar != nNewOffset)
    {
        ScrollToChar(nNewOffset);
        UpdateCaret();
        UpdateSiblingScrollPos(FALSE);
    }
}

//--| CCrystalTextView::OnKillFocus|------------------------------------------------------
void CCrystalTextView::OnKillFocus(CWnd* pNewWnd)
{
    CView::OnKillFocus(pNewWnd);
    
    m_bFocused = FALSE;
    UpdateCaret();
    if (m_ptSelStart != m_ptSelEnd)
       InvalidateLines(m_ptSelStart.y, m_ptSelEnd.y);
    if (m_bDragSelection)
    {
        ReleaseCapture();
        KillTimer(m_nDragSelTimer);
        m_bDragSelection = FALSE;
    }
}

//--| CCrystalTextView::OnSysColorChange|-------------------------------------------------
void CCrystalTextView::OnSysColorChange()
{
    CView::OnSysColorChange();
    Invalidate();
}

//--| CCrystalTextView::GetText|----------------------------------------------------------
void CCrystalTextView::GetText(const CPoint &ptStart, const CPoint &ptEnd, CString &text)
{
    if (m_pTextBuffer != 0)
       m_pTextBuffer->GetText(ptStart.y, ptStart.x, ptEnd.y, ptEnd.x, text);
    else
        text = _T("");
}

//--| CCrystalTextView::UpdateTokens|-----------------------------------------------------
void CCrystalTextView::UpdateTokens()
{
    int nLineCount = GetLineCount();
    
    // if (m_pdwParseCookies != 0)
    if (m_nParseArraySize != nLineCount)
    {
        // Reallocate cookies array
        DWORD *pdwNewArray = new DWORD[nLineCount+4];
        memset(pdwNewArray, 0xff, sizeof(DWORD) * nLineCount);
        
        memcpy(pdwNewArray, m_pdwParseCookies, sizeof(DWORD) * m_nParseArraySize);
        
        delete[] m_pdwParseCookies;
        m_nParseArraySize = nLineCount;
        m_pdwParseCookies = pdwNewArray;
    }
    if (m_pnActualLineLength != 0)
    {
        if (m_nActualLengthArraySize != nLineCount)
        {
            // Reallocate actual length array
            int *pnNewArray = new int[nLineCount];
            if (nLineCount > 0)
               memcpy(pnNewArray, m_pnActualLineLength, sizeof(int) * nLineCount);
            
            delete[] m_pnActualLineLength;
            
            m_nActualLengthArraySize = nLineCount;
            m_pnActualLineLength = pnNewArray;
        }
        if(m_nActualLengthArraySize - nLineCount)
          memset(m_pnActualLineLength + nLineCount, 0xff,
          sizeof(DWORD) * (m_nActualLengthArraySize - nLineCount));
    }
    
}

//--| CCrystalTextView::UpdateView|-------------------------------------------------------
void CCrystalTextView::UpdateView(CCrystalTextView *pSource, CUpdateContext *pContext,
                                 DWORD dwFlags, int nLineIndex /*= -1*/)
{
    if (dwFlags & UPDATE_RESET)
    {
        ResetView();
        RecalcVertScrollBar();
        RecalcHorzScrollBar();
        return;
    }
    
    int nLineCount = GetLineCount();
    ASSERT(nLineCount > 0);
    ASSERT(nLineIndex >= -1 && nLineIndex < nLineCount);
    if ((dwFlags & UPDATE_SINGLELINE) != 0)
    {
        ASSERT(nLineIndex != -1);
        // All text below this line should be reparsed
        if (m_pdwParseCookies != 0)
        {
            ASSERT(m_nParseArraySize == nLineCount);
            memset(m_pdwParseCookies + nLineIndex, 0xff, sizeof(DWORD) * (m_nParseArraySize - nLineIndex));
        }
        // This line'th actual length must be recalculated
        if (m_pnActualLineLength != 0)
        {
            ASSERT(m_nActualLengthArraySize == nLineCount);
            m_pnActualLineLength[nLineIndex] = -1;
        }
        // Repaint the lines
        InvalidateLines(nLineIndex, -1, TRUE);
    }
    else
    {
        if (nLineIndex == -1)
           nLineIndex = 0;  // Refresh all text
        // All text below this line should be reparsed
        if (m_pdwParseCookies != 0)
        {
            if (m_nParseArraySize != nLineCount)
            {
                // Reallocate cookies array
                DWORD *pdwNewArray = new DWORD[nLineCount];
                
                if (nLineIndex > 0)
                {
                    memcpy(pdwNewArray, m_pdwParseCookies, sizeof(DWORD) * nLineIndex);
                }
                
                delete[] m_pdwParseCookies;
                
                m_nParseArraySize = nLineCount;
                m_pdwParseCookies = pdwNewArray;
            }
            memset(m_pdwParseCookies + nLineIndex, 0xff,
                  sizeof(DWORD) * (m_nParseArraySize - nLineIndex));
        }
        // Recalculate actual length for all lines below this
        if (m_pnActualLineLength != 0)
        {
            if (m_nActualLengthArraySize != nLineCount)
            {
                // Reallocate actual length array
                int *pnNewArray = new int[nLineCount];
                if (nLineIndex > 0)
                   memcpy(pnNewArray, m_pnActualLineLength, sizeof(int) * nLineIndex);
                delete[] m_pnActualLineLength;
                m_nActualLengthArraySize = nLineCount;
                m_pnActualLineLength = pnNewArray;
            }
            memset(m_pnActualLineLength + nLineIndex, 0xff, sizeof(DWORD) * (m_nActualLengthArraySize - nLineIndex));
        }
        // Repaint the lines
        InvalidateLines(nLineIndex, -1, TRUE);
    }
    
    // All those points must be recalculated and validated
    if (pContext != 0)
    {
        pContext->RecalcPoint(m_ptCursorPos);
        pContext->RecalcPoint(m_ptSelStart);
        pContext->RecalcPoint(m_ptSelEnd);
        pContext->RecalcPoint(m_ptAnchor);
        ASSERT_VALIDTEXTPOS(m_ptCursorPos);
        ASSERT_VALIDTEXTPOS(m_ptSelStart);
        ASSERT_VALIDTEXTPOS(m_ptSelEnd);
        ASSERT_VALIDTEXTPOS(m_ptAnchor);
        if (m_bDraggingText)
        {
            pContext->RecalcPoint(m_ptDraggedTextBegin);
            pContext->RecalcPoint(m_ptDraggedTextEnd);
            ASSERT_VALIDTEXTPOS(m_ptDraggedTextBegin);
            ASSERT_VALIDTEXTPOS(m_ptDraggedTextEnd);
        }
        CPoint ptTopLine(0, m_nTopLine);
        pContext->RecalcPoint(ptTopLine);
        ASSERT_VALIDTEXTPOS(ptTopLine);
        m_nTopLine = ptTopLine.y;
        UpdateCaret();
    }
    
    // Recalculate vertical scrollbar, if needed
    if ((dwFlags & UPDATE_VERTRANGE) != 0)
    {
        if (! m_bVertScrollBarLocked)
           RecalcVertScrollBar();
    }
    
    // Recalculate horizontal scrollbar, if needed
    if ((dwFlags & UPDATE_HORZRANGE) != 0)
    {
        m_nMaxLineLength = -1;
        if (! m_bHorzScrollBarLocked)
           RecalcHorzScrollBar();
    }
}

//--| CCrystalTextView::GetResourceHandle|------------------------------------------------
HINSTANCE CCrystalTextView::GetResourceHandle()
{
    #ifdef CRYSEDIT_RES_HANDLE
        return CRYSEDIT_RES_HANDLE;
    #else
        if (s_hResourceInst != 0)
        return s_hResourceInst;
    return AfxGetResourceHandle();
    #endif
}

//--| CCrystalTextView::OnCreate|---------------------------------------------------------
int CCrystalTextView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    memset(&m_lfBaseFont, 0, sizeof(m_lfBaseFont));
    _tcscpy(m_lfBaseFont.lfFaceName, _T("FixedSys"));
    m_lfBaseFont.lfHeight = 0;
    m_lfBaseFont.lfWeight = FW_NORMAL;
    m_lfBaseFont.lfItalic = FALSE;
    m_lfBaseFont.lfCharSet = DEFAULT_CHARSET;
    m_lfBaseFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    m_lfBaseFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    m_lfBaseFont.lfQuality = DEFAULT_QUALITY;
    m_lfBaseFont.lfPitchAndFamily = DEFAULT_PITCH;
    
    if (CView::OnCreate(lpCreateStruct) == -1)
       return -1;
    
    return 0;
}

//--| CCrystalTextView::SetAnchor|--------------------------------------------------------
void CCrystalTextView::SetAnchor(const CPoint &ptNewAnchor)
{
    ASSERT_VALIDTEXTPOS(ptNewAnchor);
    m_ptAnchor = ptNewAnchor;
}

void CCrystalTextView::OnEditOperation(int nAction, LPCTSTR pszText)
{
}

//--| CCrystalTextView::PreTranslateMessage|----------------------------------------------
BOOL CCrystalTextView::PreTranslateMessage(MSG *pMsg)
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
        if (theApp._hScriptEditAccel )
        {
            if (::TranslateAccelerator(m_hWnd, theApp._hScriptEditAccel, pMsg))
               return TRUE;
        }
    }
    
    return CView::PreTranslateMessage(pMsg);
}

//--| CCrystalTextView::GetCursorPos|-----------------------------------------------------
CPoint CCrystalTextView::GetCursorPos()
{
    return m_ptCursorPos;
}

//--| CCrystalTextView::SetCursorPos|-----------------------------------------------------
void CCrystalTextView::SetCursorPos(const CPoint &ptCursorPos)
{
    ASSERT_VALIDTEXTPOS(ptCursorPos);
    m_ptCursorPos = ptCursorPos;
    m_nIdealCharPos = CalculateActualOffset(m_ptCursorPos.y, m_ptCursorPos.x);
    UpdateCaret();
}

//--| CCrystalTextView::SetSelectionMargin|-----------------------------------------------
void CCrystalTextView::SetSelectionMargin(BOOL bSelMargin)
{
    if (m_bSelMargin != bSelMargin)
    {
        m_bSelMargin = bSelMargin;
        if (::IsWindow(m_hWnd))
        {
            m_nScreenChars = -1;
            Invalidate();
            RecalcHorzScrollBar();
        }
    }
}

//--| CCrystalTextView::GetFont|----------------------------------------------------------
void CCrystalTextView::GetFont(LOGFONT &lf)
{
    lf = m_lfBaseFont;
}

//--| CCrystalTextView::SetFont|----------------------------------------------------------
void CCrystalTextView::SetFont(const LOGFONT &lf)
{
    m_lfBaseFont = lf;
    m_nScreenLines = -1;
    m_nScreenChars = -1;
    m_nCharWidth = -1;
    m_nLineHeight = -1;
    if (m_pCacheBitmap != 0)
    {
        m_pCacheBitmap->DeleteObject();
        delete m_pCacheBitmap;
        m_pCacheBitmap = 0;
    }
    for (int I = 0; I < 4; I ++)
    {
        if (m_apFonts[I] != 0)
        {
            m_apFonts[I]->DeleteObject();
            delete m_apFonts[I];
            m_apFonts[I] = 0;
        }
    }
    if (::IsWindow(m_hWnd))
    {
        RecalcVertScrollBar();
        RecalcHorzScrollBar();
        UpdateCaret();
        Invalidate();
    }
}

//--| CCrystalTextView::OnUpdateIndicatorPosition|----------------------------------------
void CCrystalTextView::OnUpdateIndicatorPosition(CCmdUI* pCmdUI)
{
    ASSERT_VALIDTEXTPOS(m_ptCursorPos);
    CString stat;
    stat.Format(_T("Ln %d, Col %d"), m_ptCursorPos.y + 1, m_ptCursorPos.x + 1);
    pCmdUI->SetText(stat);
}

//--| CCrystalTextView::OnUpdateIndicatorCRLF|--------------------------------------------
void CCrystalTextView::OnUpdateIndicatorCRLF(CCmdUI* pCmdUI)
{
    if (m_pTextBuffer != 0)
    {
        int crlfMode = m_pTextBuffer->GetCRLFMode();
        switch (crlfMode)
        {
            case CRLF_STYLE_DOS:
                pCmdUI->SetText(_T("DOS"));
            pCmdUI->Enable(TRUE);
            break;
            case CRLF_STYLE_UNIX:
                pCmdUI->SetText(_T("UNIX"));
            pCmdUI->Enable(TRUE);
            break;
            case CRLF_STYLE_MAC:
                pCmdUI->SetText(_T("MAC"));
            pCmdUI->Enable(TRUE);
            break;
            default:
                pCmdUI->SetText(0);
            pCmdUI->Enable(FALSE);
        }
    }
    else
    {
        pCmdUI->SetText(0);
        pCmdUI->Enable(FALSE);
    }
}

//--| CCrystalTextView::OnToggleBookmark|-------------------------------------------------
void CCrystalTextView::OnToggleBookmark(UINT nCmdID)
{
    int nBookmarkID = nCmdID - ID_SC_TOGGLE_BOOKMARK0;
    ASSERT(nBookmarkID >= 0 && nBookmarkID <= 9);
    if (m_pTextBuffer != 0)
    {
        DWORD dwFlags = GetLineFlags(m_ptCursorPos.y);
        DWORD dwMask = LF_BOOKMARK(nBookmarkID);
        m_pTextBuffer->SetLineFlag(m_ptCursorPos.y, dwMask, (dwFlags & dwMask) == 0);
    }
}

//--| CCrystalTextView::OnGoBookmark|-----------------------------------------------------
void CCrystalTextView::OnGoBookmark(UINT nCmdID)
{
    int nBookmarkID = nCmdID - ID_SC_GO_BOOKMARK0;
    ASSERT(nBookmarkID >= 0 && nBookmarkID <= 9);
    if (m_pTextBuffer != 0)
    {
        int nLine = m_pTextBuffer->GetLineWithFlag(LF_BOOKMARK(nBookmarkID));
        if (nLine >= 0)
        {
            CPoint pt(0, nLine);
            ASSERT_VALIDTEXTPOS(pt);
            SetCursorPos(pt);
            SetSelection(pt, pt);
            SetAnchor(pt);
            EnsureVisible(pt);
        }
    }
}

//--| CCrystalTextView::OnClearBookmarks|-------------------------------------------------
void CCrystalTextView::OnClearBookmarks()
{
    if (m_pTextBuffer != 0)
    {
        for (int nBookmarkID = 0; nBookmarkID <= 9; nBookmarkID++)
        {
            int nLine = m_pTextBuffer->GetLineWithFlag(LF_BOOKMARK(nBookmarkID));
            if (nLine >= 0)
            {
                m_pTextBuffer->SetLineFlag(nLine, LF_BOOKMARK(nBookmarkID), FALSE);
            }
        }
        
    }
}

//--| CCrystalTextView::ShowCursor|-------------------------------------------------------
void CCrystalTextView::ShowCursor()
{
    m_bCursorHidden = FALSE;
    UpdateCaret();
}

//--| CCrystalTextView::HideCursor|-------------------------------------------------------
void CCrystalTextView::HideCursor()
{
    m_bCursorHidden = TRUE;
    UpdateCaret();
}

//--| CCrystalTextView::GetDropEffect|----------------------------------------------------
DROPEFFECT CCrystalTextView::GetDropEffect()
{
    return DROPEFFECT_COPY;
}

//--| CCrystalTextView::OnDropSource|-----------------------------------------------------
void CCrystalTextView::OnDropSource(DROPEFFECT de)
{
    ASSERT(de == DROPEFFECT_COPY);
}

//--| CCrystalTextView::PrepareDragData|--------------------------------------------------
HGLOBAL CCrystalTextView::PrepareDragData()
{
    PrepareSelBounds();
    if (m_ptDrawSelStart == m_ptDrawSelEnd)
       return 0;
    
    CString text;
    GetText(m_ptDrawSelStart, m_ptDrawSelEnd, text);
    HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, lstrlen(text) + 1);
    if (hData == 0)
       return 0;
    
    LPSTR pszData = (LPSTR) ::GlobalLock(hData);
    USES_CONVERSION;
    _tcscpy(pszData, T2A(text.GetBuffer(0)));
    text.ReleaseBuffer();
    ::GlobalUnlock(hData);
    
    m_ptDraggedTextBegin = m_ptDrawSelStart;
    m_ptDraggedTextEnd = m_ptDrawSelEnd;
    return hData;
}

//--| int FindStringHelper|---------------------------------------------------------------
static int FindStringHelper(LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, BOOL bWholeWord)
{
    ASSERT(pszFindWhere != 0);
    ASSERT(pszFindWhat != 0);
    int nCur = 0;
    int nLength = lstrlen(pszFindWhat);
    for (;;)
    {
        #ifdef _UNICODE
            LPCTSTR pszPos = wcsstr(pszFindWhere, pszFindWhat);
        #else
            LPCTSTR pszPos = strstr(pszFindWhere, pszFindWhat);
        #endif
            if (pszPos == 0)
            return -1;
        if (! bWholeWord)
           return nCur + (pszPos - pszFindWhere);
        if (pszPos > pszFindWhere && (isalnum(pszPos[-1]) || pszPos[-1] == _T('_')))
        {
            nCur += (pszPos - pszFindWhere);
            pszFindWhere = pszPos + 1;
            continue;
        }
        if (isalnum(pszPos[nLength]) || pszPos[nLength] == _T('_'))
        {
            nCur += (pszPos - pszFindWhere + 1);
            pszFindWhere = pszPos + 1;
            continue;
        }
        return nCur + (pszPos - pszFindWhere);
    }
    ASSERT(FALSE);  // Unreachable
    return -1;
}

//--| CCrystalTextView::HighlightText|----------------------------------------------------
BOOL CCrystalTextView::HighlightText(const CPoint &ptStartPos, int nLength)
{
    ASSERT_VALIDTEXTPOS(ptStartPos);
    m_ptCursorPos = ptStartPos;
    m_ptCursorPos.x += nLength;
    ASSERT_VALIDTEXTPOS(m_ptCursorPos);  // Probably 'nLength' is bigger than expected...
    m_ptAnchor = m_ptCursorPos;
    SetSelection(ptStartPos, m_ptCursorPos);
    UpdateCaret();
    EnsureVisible(m_ptCursorPos);
    return TRUE;
}

//--| CCrystalTextView::FindText|---------------------------------------------------------
BOOL CCrystalTextView::FindText(LPCTSTR pszText, const CPoint &ptStartPos, DWORD dwFlags,
                               BOOL bWrapSearch, CPoint *pptFoundPos)
{
    int nLineCount = GetLineCount();
    return FindTextInBlock(pszText, ptStartPos, CPoint(0, 0),
                          CPoint(GetLineLength(nLineCount - 1), nLineCount - 1),
                          dwFlags, bWrapSearch, pptFoundPos);
}

//--| CCrystalTextView::FindTextInBlock|--------------------------------------------------
BOOL CCrystalTextView::FindTextInBlock(LPCTSTR pszText, const CPoint &ptStartPosition,
                                      const CPoint &ptBlockBegin, const CPoint &ptBlockEnd,
                                      DWORD dwFlags, BOOL bWrapSearch, CPoint *pptFoundPos)
{
    CPoint ptCurrentPos = ptStartPosition;
    
    ASSERT(pszText != 0 && lstrlen(pszText) > 0);
    ASSERT_VALIDTEXTPOS(ptCurrentPos);
    ASSERT_VALIDTEXTPOS(ptBlockBegin);
    ASSERT_VALIDTEXTPOS(ptBlockEnd);
    ASSERT(ptBlockBegin.y < ptBlockEnd.y || ptBlockBegin.y == ptBlockEnd.y &&
          ptBlockBegin.x <= ptBlockEnd.x);
    if (ptBlockBegin == ptBlockEnd)
       return FALSE;
    
    if (ptCurrentPos.y < ptBlockBegin.y || ptCurrentPos.y == ptBlockBegin.y &&
       ptCurrentPos.x < ptBlockBegin.x)
       ptCurrentPos = ptBlockBegin;
    
    CString what = pszText;
    if ((dwFlags & FIND_MATCH_CASE) == 0)
       what.MakeUpper();
    
    if (dwFlags & FIND_DIRECTION_UP)
    {
        // Let's check if we deal with whole text.
        // At this point, we cannot search *up* in selection
        ASSERT(ptBlockBegin.x == 0 && ptBlockBegin.y == 0);
        ASSERT(ptBlockEnd.x == GetLineLength(GetLineCount() - 1) &&
              ptBlockEnd.y == GetLineCount() - 1);
        
        // Proceed as if we have whole text search.
        for (;;)
        {
            while (ptCurrentPos.y >= 0)
            {
                int nLineLength = GetLineLength(ptCurrentPos.y);
                nLineLength -= ptCurrentPos.x;
                if (nLineLength <= 0)
                {
                    ptCurrentPos.x = 0;
                    ptCurrentPos.y --;
                    continue;
                }
                
                LPCTSTR pszChars = GetLineChars(ptCurrentPos.y);
                pszChars += ptCurrentPos.x;
                
                CString line;
                _tcsncpy(line.GetBuffer(nLineLength + 1), pszChars, nLineLength + 1);
                line.ReleaseBuffer();
                if ((dwFlags & FIND_MATCH_CASE) == 0)
                   line.MakeUpper();
                
                int nPos = ::FindStringHelper(line, what, (dwFlags & FIND_WHOLE_WORD) != 0);
                if (nPos >= 0)  // Found text!
                {
                    ptCurrentPos.x += nPos;
                    *pptFoundPos = ptCurrentPos;
                    return TRUE;
                }
                
                ptCurrentPos.x = 0;
                ptCurrentPos.y --;
            }
            
            // Beginning of text reached
            if (! bWrapSearch)
               return FALSE;
            
            // Start again from the end of text
            bWrapSearch = FALSE;
            ptCurrentPos = CPoint(0, GetLineCount() - 1);
        }
    }
    else
    {
        for (;;)
        {
            while (ptCurrentPos.y <= ptBlockEnd.y)
            {
                int nLineLength = GetLineLength(ptCurrentPos.y);
                nLineLength -= ptCurrentPos.x;
                if (nLineLength <= 0)
                {
                    ptCurrentPos.x = 0;
                    ptCurrentPos.y ++;
                    continue;
                }
                
                LPCTSTR pszChars = GetLineChars(ptCurrentPos.y);
                pszChars += ptCurrentPos.x;
                
                // Prepare necessary part of line
                CString line;//strncpy
                _tcsncpy(line.GetBuffer(nLineLength + 1), pszChars, nLineLength + 1);
                line.ReleaseBuffer();
                if ((dwFlags & FIND_MATCH_CASE) == 0)
                   line.MakeUpper();
                
                // Perform search in the line
                int nPos = ::FindStringHelper(line, what, (dwFlags & FIND_WHOLE_WORD) != 0);
                if (nPos >= 0)
                {
                    ptCurrentPos.x += nPos;
                    // Check of the text found is outside the block.
                    if (ptCurrentPos.y == ptBlockEnd.y && ptCurrentPos.x >= ptBlockEnd.x)
                       break;
                    
                    *pptFoundPos = ptCurrentPos;
                    return TRUE;
                }
                
                // Go further, text was not found
                ptCurrentPos.x = 0;
                ptCurrentPos.y ++;
            }
            
            // End of text reached
            if (! bWrapSearch)
               return FALSE;
            
            // Start from the beginning
            bWrapSearch = FALSE;
            ptCurrentPos = ptBlockBegin;
        }
    }
    
    ASSERT(FALSE);  // Unreachable
    return FALSE;
}

//--| CCrystalTextView::OnEditFind|-------------------------------------------------------
void CCrystalTextView::OnEditFind()
{
    CWinApp *pApp = AfxGetApp();
    ASSERT(pApp != 0);
    
    CFindTextDlg dlg(this);
    if (m_bLastSearch)
    {
        // Get the latest search parameters
        dlg.m_bMatchCase = (m_dwLastSearchFlags & FIND_MATCH_CASE) != 0;
        dlg.m_bWholeWord = (m_dwLastSearchFlags & FIND_WHOLE_WORD) != 0;
        dlg.m_nDirection = (m_dwLastSearchFlags & FIND_DIRECTION_UP) != 0 ? 0 : 1;
        if (m_pszLastFindWhat != 0)
           dlg.m_sText = m_pszLastFindWhat;
    }
    else
    {
        // Take search parameters from registry
        dlg.m_bMatchCase = pApp->GetProfileInt(REG_FIND_SUBKEY, REG_MATCH_CASE, FALSE);
        dlg.m_bWholeWord = pApp->GetProfileInt(REG_FIND_SUBKEY, REG_WHOLE_WORD, FALSE);
        dlg.m_nDirection = 1;  // Search down
        dlg.m_sText = pApp->GetProfileString(REG_FIND_SUBKEY, REG_FIND_WHAT, _T(""));
    }
    
    // Take the current selection, if any
    if (IsSelection())
    {
        CPoint ptSelStart, ptSelEnd;
        GetSelection(ptSelStart, ptSelEnd);  if (ptSelStart.y == ptSelEnd.y)
        {
            LPCTSTR pszChars = GetLineChars(ptSelStart.y);
            int nChars = ptSelEnd.x - ptSelStart.x;
            _tcsncpy(dlg.m_sText.GetBuffer(nChars + 1), pszChars + ptSelStart.x, nChars + 1);
            dlg.m_sText.ReleaseBuffer();
        }
    }
    
    // Execute Find dialog
    dlg.m_ptCurrentPos = m_ptCursorPos;  // Search from cursor position
    m_bShowInactiveSelection = TRUE;
    dlg.DoModal();
    m_bShowInactiveSelection = FALSE;
    
    // Save search parameters for 'F3' command
    m_bLastSearch = TRUE;
    if (m_pszLastFindWhat != 0)
       free(m_pszLastFindWhat);
    #ifdef _UNICODE
        m_pszLastFindWhat = _wcsdup(dlg.m_sText);
    #else
        m_pszLastFindWhat = strdup(dlg.m_sText);
    #endif
        
    m_dwLastSearchFlags = 0;
    if (dlg.m_bMatchCase)
       m_dwLastSearchFlags |= FIND_MATCH_CASE;
    if (dlg.m_bWholeWord)
       m_dwLastSearchFlags |= FIND_WHOLE_WORD;
    if (dlg.m_nDirection == 0)
       m_dwLastSearchFlags |= FIND_DIRECTION_UP;
    
    // Save search parameters to registry
    pApp->WriteProfileInt(REG_FIND_SUBKEY, REG_MATCH_CASE, dlg.m_bMatchCase);
    pApp->WriteProfileInt(REG_FIND_SUBKEY, REG_WHOLE_WORD, dlg.m_bWholeWord);
    pApp->WriteProfileString(REG_FIND_SUBKEY, REG_FIND_WHAT, dlg.m_sText);
}

//--| CCrystalTextView::OnEditRepeat|-----------------------------------------------------
void CCrystalTextView::OnEditRepeat()
{
    if (m_bLastSearch)
    {
        CPoint ptFoundPos;
        if (! FindText(m_pszLastFindWhat, m_ptCursorPos, m_dwLastSearchFlags, TRUE, &ptFoundPos))
        {
            CString prompt;
            prompt.Format(IDS_EDIT_TEXT_NOT_FOUND, m_pszLastFindWhat);
            AfxMessageBox(prompt);
            return;
        }
        HighlightText(ptFoundPos, lstrlen(m_pszLastFindWhat));
        m_bMultipleSearch = TRUE;       // More search
    }
}

//--| CCrystalTextView::OnUpdateEditRepeat|-----------------------------------------------
void CCrystalTextView::OnUpdateEditRepeat(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bLastSearch);
}

//--| CCrystalTextView::OnEditFindPrevious|-----------------------------------------------
void CCrystalTextView::OnEditFindPrevious()
{
    DWORD dwSaveSearchFlags = m_dwLastSearchFlags;
    if ((m_dwLastSearchFlags & FIND_DIRECTION_UP) != 0)
       m_dwLastSearchFlags &= ~FIND_DIRECTION_UP;
    else
        m_dwLastSearchFlags |= FIND_DIRECTION_UP;
    OnEditRepeat();
    m_dwLastSearchFlags = dwSaveSearchFlags;
}

//--| CCrystalTextView::OnUpdateEditFindPrevious|-----------------------------------------
void CCrystalTextView::OnUpdateEditFindPrevious(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bLastSearch);
}

//--| CCrystalTextView::OnFilePageSetup|--------------------------------------------------
void CCrystalTextView::OnFilePageSetup()
{
    CWinApp *pApp = AfxGetApp();
    ASSERT(pApp != 0);
    
    CPageSetupDialog dlg;
    dlg.m_psd.Flags &= ~PSD_INTHOUSANDTHSOFINCHES;
    dlg.m_psd.Flags |= PSD_INHUNDREDTHSOFMILLIMETERS | PSD_DISABLEORIENTATION | PSD_DISABLEPAPER;
    dlg.m_psd.rtMargin.left = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_LEFT, DEFAULT_PRINT_MARGIN);
    dlg.m_psd.rtMargin.right = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_RIGHT, DEFAULT_PRINT_MARGIN);
    dlg.m_psd.rtMargin.top = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_TOP, DEFAULT_PRINT_MARGIN);
    dlg.m_psd.rtMargin.bottom = pApp->GetProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_BOTTOM, DEFAULT_PRINT_MARGIN);
    if (dlg.DoModal() == IDOK)
    {
        pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_LEFT, dlg.m_psd.rtMargin.left);
        pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_RIGHT, dlg.m_psd.rtMargin.right);
        pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_TOP, dlg.m_psd.rtMargin.top);
        pApp->WriteProfileInt(REG_PAGE_SUBKEY, REG_MARGIN_BOTTOM, dlg.m_psd.rtMargin.bottom);
    }
}

//--| CCrystalTextView::OnToggleBookmark|-------------------------------------------------
void CCrystalTextView::OnToggleBookmark()
{
    if (m_pTextBuffer != 0)
    {
        DWORD dwFlags = GetLineFlags(m_ptCursorPos.y);
        DWORD dwMask  = LF_BOOKMARKS;
        m_pTextBuffer->SetLineFlag(m_ptCursorPos.y, dwMask, (dwFlags & dwMask) == 0, FALSE);
    }
    int nLine = m_pTextBuffer->GetLineWithFlag(LF_BOOKMARKS);
    if (nLine >= 0)
       m_bBookmarkExist = TRUE;
    else
        m_bBookmarkExist = FALSE;
}

//--| CCrystalTextView::OnNextBookmark|---------------------------------------------------
void CCrystalTextView::OnNextBookmark()
{
    if (m_pTextBuffer != 0)
    {
        int nLine = m_pTextBuffer->FindNextBookmarkLine(m_ptCursorPos.y);
        if (nLine >= 0)
        {
            CPoint pt(0, nLine);
            ASSERT_VALIDTEXTPOS(pt);
            SetCursorPos(pt);
            SetSelection(pt, pt);
            SetAnchor(pt);
            EnsureVisible(pt);
        }
    }
}

//--| CCrystalTextView::OnPrevBookmark|---------------------------------------------------
void CCrystalTextView::OnPrevBookmark()
{
    if (m_pTextBuffer != 0)
    {
        int nLine = m_pTextBuffer->FindPrevBookmarkLine(m_ptCursorPos.y);
        if (nLine >= 0)
        {
            CPoint pt(0, nLine);
            ASSERT_VALIDTEXTPOS(pt);
            SetCursorPos(pt);
            SetSelection(pt, pt);
            SetAnchor(pt);
            EnsureVisible(pt);
        }
    }
}

//--| CCrystalTextView::OnClearAllBookmarks|----------------------------------------------
void CCrystalTextView::OnClearAllBookmarks()
{
    if (m_pTextBuffer != 0)
    {
        int nLineCount = GetLineCount();
        for (int I = 0; I < nLineCount; I ++)
        {
            if (m_pTextBuffer->GetLineFlags(I) & LF_BOOKMARKS)
               m_pTextBuffer->SetLineFlag(I, LF_BOOKMARKS, FALSE);
        }
        m_bBookmarkExist = FALSE;
    }
}

//--| CCrystalTextView::OnUpdateNextBookmark|---------------------------------------------
void CCrystalTextView::OnUpdateNextBookmark(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bBookmarkExist);
}

//--| CCrystalTextView::OnUpdatePrevBookmark|---------------------------------------------
void CCrystalTextView::OnUpdatePrevBookmark(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bBookmarkExist);
}

//--| CCrystalTextView::OnUpdateClearAllBookmarks|----------------------------------------
void CCrystalTextView::OnUpdateClearAllBookmarks(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bBookmarkExist);
}

//--| CCrystalTextView::GetViewTabs|------------------------------------------------------
BOOL CCrystalTextView::GetViewTabs()
{
    return m_bViewTabs;
}

//--| CCrystalTextView::SetViewTabs|------------------------------------------------------
void CCrystalTextView::SetViewTabs(BOOL bViewTabs)
{
    if (bViewTabs != m_bViewTabs)
    {
        m_bViewTabs = bViewTabs;
        if (::IsWindow(m_hWnd))
           Invalidate();
    }
}

//--| CCrystalTextView::GetSelectionMargin|-----------------------------------------------
BOOL CCrystalTextView::GetSelectionMargin()
{
    return m_bSelMargin;
}

//--| CCrystalTextView::GetMarginWidth|---------------------------------------------------
int CCrystalTextView::GetMarginWidth()
{
    return m_bSelMargin ? 20 : 1;
}

//--| CCrystalTextView::GetSmoothScroll|--------------------------------------------------
BOOL CCrystalTextView::GetSmoothScroll() const
{
    return m_bSmoothScroll;
}

//--| CCrystalTextView::SetSmoothScroll|--------------------------------------------------
void CCrystalTextView::SetSmoothScroll(BOOL bSmoothScroll)
{
    m_bSmoothScroll = bSmoothScroll;
}

//--| CCrystalTextView::GetDisableDragAndDrop|--------------------------------------------
BOOL CCrystalTextView::GetDisableDragAndDrop() const
{
    return m_bDisableDragAndDrop;
}

//--| CCrystalTextView::SetDisableDragAndDrop|--------------------------------------------
void CCrystalTextView::SetDisableDragAndDrop(BOOL bDDAD)
{
    m_bDisableDragAndDrop = bDDAD;
}
