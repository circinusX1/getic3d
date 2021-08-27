// DlgTexRefs.cpp : implementation file
//

#include "stdafx.h"
#include "z-edmap.h"
#include "DlgTexRefs.h"
#include "Compiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTexRefs dialog
static int ks =  -1;//AKEY(VK_CONTROL);

CDlgTexRefs::CDlgTexRefs(CWnd* pParent /*=NULL*/)
	: CBASEDLG(CDlgTexRefs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTexRefs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTexRefs::DoDataExchange(CDataExchange* pDX)
{
	CBASEDLG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTexRefs)
	DDX_Control(pDX, IDCANCEL, m_c);
	DDX_Control(pDX, LC_TEXREFS, m_lv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTexRefs, CBASEDLG)
	//{{AFX_MSG_MAP(CDlgTexRefs)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, LC_TEXREFS, OnClickTexrefs)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_ITEMSEL, OnItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTexRefs message handlers

BOOL CDlgTexRefs::OnInitDialog() 
{
    char        sFullPath[PATH_MAX];
    char        sDummy[PATH_MAX];
    char        sFileName[PATH_MAX];
    char        sExt[_MAX_EXT];

    m_lv.Block();

    CBASEDLG::OnInitDialog();
	
    m_lv.SetHeadersNames("Texture Name  ","Id","Refs","Applyed","Dimension","Size (Kb)",0);

    TexSys::iterator bt = GTexSys.begin();
    TexSys::iterator et = GTexSys.end();
    size_t    totalSizes = 0;
    for(;bt != et; bt++)
    {
        Texture& tex  = (*bt).second;
        
        strcpy(sFullPath,(*bt).first.c_str());
        _tsplitpath(sFullPath,sDummy,sDummy,sFileName,sExt);
        _tcscat(sFileName,sExt);
        

        m_lv.AddNewItem(sFileName, 
                        MKSTR("%d", tex.hTex),
                        MKSTR("%d", tex.refcount),
                        MKSTR("%d", GTexSys._texmap[tex.hTex]) ,
                        MKSTR("%d*%d/%d", tex.cx,tex.cy,tex.bpp),
                        MKSTR("%.3f", (float)(tex.cx*tex.cy*tex.bpp/1024.0f)),
                        0);

        totalSizes+=tex.cx*tex.cy*tex.bpp;
    }
    _iSel  = -1;
    GetDlgItem(ST_BMPSSZ)->SetWindowText(MKSTR("%d b",totalSizes));

    m_lv.Block(0);

    ks =-1;
	return TRUE;  
}

LRESULT CDlgTexRefs::OnItemChanged(WPARAM w,LPARAM l)
{
    RECT rt;
    _iSel = w;
    GetDlgItem(IDC_STATIC1)->GetWindowRect(&rt);
    ScreenToClient(&rt);
    InvalidateRect(&rt);
    
    return 0;
}


void CDlgTexRefs::OnClickTexrefs(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    _iSel = pNMListView->iItem;
    RECT rt;
    GetDlgItem(IDC_STATIC1)->GetWindowRect(&rt);
    ScreenToClient(&rt);
    InvalidateRect(&rt);
    *pResult = 0;
}


void CDlgTexRefs::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    if(_iSel==-1)
        return;
    HBITMAP         ob, hBitmap ;
    BOOL            loaded = FALSE;
    BOOL            bLmLoaded = FALSE;
    RECT            rt;
    CString         texName = m_lv.GetItemText(_iSel,0);	
    TexHandler      th;
    LmSizeBuff      lmBuff;
    
    int dash = texName.Find('?');
    if(dash!=-1)
        texName = texName.Left(dash);

    GetDlgItem(IDC_STATIC1)->GetWindowRect(&rt);
    ScreenToClient(&rt);
    
    if(!strncmp(texName, "@L", 2))
        bLmLoaded = Compiler::PCompiler->_lmProc.GetLmapBytes(texName, lmBuff);
    else
        loaded = th.LoadThisFile(texName,0);

    BITMAPINFOHEADER bi     = {0};
    int stepX=4;
    int stepY=4;

    if(loaded)
    {
        HDC              hdc    = dc.m_hDC;
        HDC              hMemDC = CreateCompatibleDC(hdc);
        

        bi.biSize        = sizeof(BITMAPINFOHEADER);
        bi.biHeight      = th.n_y; 
        bi.biWidth       = th.n_x;
        bi.biPlanes      = 1;
        bi.biCompression = BI_RGB;
        bi.biBitCount    = 24;
        th.SwapRB();
        hBitmap = CreateDIBitmap(hdc, 
                                         &bi, 
                                         CBM_INIT, 
                                         th.Buffer(), 
                                         (BITMAPINFO*)&bi, 
                                         DIB_RGB_COLORS);
        ob = (HBITMAP)SelectObject(hMemDC, hBitmap);
        StretchBlt( hdc,rt.left+1, 
                    rt.top + 1, 
                    rt.right-rt.left-1, 
					rt.bottom-rt.top-2,
					hMemDC,0,0,th.n_x ,th.n_y, SRCCOPY);
        ::SelectObject(hMemDC, ob);
        ::DeleteObject(hBitmap) ;
        ::DeleteDC(hMemDC);
    }
    else if(bLmLoaded)
    {
        HDC              hdc    = dc.m_hDC;
        HDC              hMemDC = CreateCompatibleDC(hdc);


        bi.biSize        = sizeof(BITMAPINFOHEADER);
        bi.biHeight      = lmBuff.sz.cy & 0xFFFF; 
        bi.biWidth       = lmBuff.sz.cx & 0xFFFF;
        bi.biPlanes      = 1;
        bi.biCompression = BI_RGB;
        bi.biBitCount    = 24;

        stepX = (rt.right-rt.left-1) / bi.biWidth;
        stepY = (rt.bottom-rt.top-1) / bi.biHeight;

        hBitmap = CreateDIBitmap(hdc,    
                                &bi, 
                                CBM_INIT, 
                                lmBuff.pBuff, 
                                (BITMAPINFO*)&bi, 
                                DIB_RGB_COLORS);
        ob = (HBITMAP)SelectObject(hMemDC, hBitmap);
        StretchBlt( hdc,
                    rt.left, 
                    rt.top, 
                    stepX * bi.biWidth, 
					stepY * bi.biHeight,
					hMemDC,0,0,
                    lmBuff.sz.cx & 0xFFFF, 
                    lmBuff.sz.cy & 0xFFFF, 
                    SRCCOPY);        
        ::SelectObject(hMemDC, ob);
        ::DeleteObject(hBitmap) ;
        ::DeleteDC(hMemDC);

    }

    if(AKEY(VK_CONTROL) && bLmLoaded)
    {
        char tout[32];
       
        dc.SetBkMode(TRANSPARENT);
        int x;
        int j = 0;
        for( x=rt.left;x < rt.right; x+= stepX)
        {
            dc.MoveTo(x,rt.bottom);
            dc.LineTo(x,rt.top);
            sprintf(tout,"%d",j++);

            dc.TextOut(x+4,rt.top+1,tout,strlen(tout));

        }
        j = 0;
        for(x=rt.top;x < rt.bottom; x+= stepY)
        {
            dc.MoveTo(rt.left, x);
            dc.LineTo(rt.right,x);
            sprintf(tout,"%d",j++);

            dc.TextOut(rt.left+4 , x+1, tout,strlen(tout));
        }
    }
}


BOOL CDlgTexRefs::PreTranslateMessage(MSG* pMsg)
{
    BOOL br = CDialog::PreTranslateMessage(pMsg);
    
    if((pMsg->message == WM_SYSKEYDOWN || 
        pMsg->message == WM_KEYDOWN  ||   
        pMsg->message == WM_SYSKEYUP || 
        pMsg->message == WM_KEYUP) &&
        pMsg->wParam == VK_CONTROL)
    {
        if(ks != AKEY(VK_CONTROL))
        {
            ks = AKEY(VK_CONTROL);
            PostMessage(WM_ITEMSEL,_iSel);
        }
    }

    return br;
}

