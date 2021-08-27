
#include "stdafx.h"
#include "z-edmap.h"
#include "Aboutdlg.h"
#include "VertDlgDlg.h"
#include "MainFrm.h"
#include "versionmacro.h"//#define _TRIAL
#include "z_ed2Doc.h"

BOOL GetOpenCommand(const char* regPath, CString& command);
static CAboutDlg* PAbout;
static REAL       Dst = -2000.0, J=0;
//---------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)//,m_zs("www.zalsoft.com",1)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
    PAbout = this;
    p_texBuff = 0;
    Dst = -16000.0;
    J=0;
}

//---------------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//DDX_Control(pDX, ST_BUYG, m_st);
	//}}AFX_DATA_MAP
}

//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static GLUquadricObj*  GluObj;

static void FCbPaint(CWnd* pdc, V_MODE mode)
{
    glPushMatrix();
    
    DOC()->_vmodeback=1;
    DOC()->SetFaceMode();

    Ta.Enable();
    Ta.Bind(PAbout->m_htAbout,0);

    glTranslatef(0,0,Dst);
    glRotatef(180,1,0,0);

    if(Dst || J < 380)
    {
        glRotatef(J+=8,0,1,0);
        if(J>=380)
        {
            if(Dst)
                J=0;
            else
                J=380;
        }
    }
    

    glBegin(GL_POLYGON);

        glVertex3f(-1400,600,0);
        glTexCoord2f(1,0);
        
        glVertex3f(1400,600,0);
        glTexCoord2f(1,1);

        glVertex3f(1400,-600,0);
        glTexCoord2f(0,1);

        glVertex3f(-1400,-600,0);
        glTexCoord2f(0,0);
    glEnd();

    /*
    //TexRef::GlLmapMode(1);    
    Ta.Bind(PAbout->m_htNoise,0);
    
    glBegin(GL_POLYGON);

        glVertex3f(-1400,600,0);
        glTexCoord2f(1,0);
        
        glVertex3f(1400,600,0);
        glTexCoord2f(1,1);

        glVertex3f(1400,-600,0);
        glTexCoord2f(0,1);

        glVertex3f(-1400,-600,0);
        glTexCoord2f(0,0);
    glEnd();


    TexRef::GlLmapMode(0);
*/

    DOC()->_vmodeback=0;
    DOC()->SetFaceMode();


    glPopMatrix();
}

//---------------------------------------------------------------------------------------
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    char out[512];

    if(!IsValidUL(out))
    {
        SetWindowText("Getic 3.04.02 Anoying version");
        SetDlgItemText(IDC_EDIT1, out);
    }
    else
    {
        SetWindowText("Getic 3.04.02 ");
        GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
    }
    

    _pStaticGL = FRM()->GetGLWidnow(this, IDC_STATICGL);
    _pStaticGL->_pcbpaint = FCbPaint;
    _pStaticGL->_bHide = 1;
    _pStaticGL->_dist = -600;
    _pStaticGL->_backgrnd[0]=0;
    _pStaticGL->_backgrnd[1]=0;
    _pStaticGL->_backgrnd[2]=0;
    _pStaticGL->_backgrnd[3]=255;

    m_timer = SetTimer(345,20,0);
    TexRef::_MakeCurrent = 0;
    _pStaticGL->GlMakeCurrent();

    BYTE* ptexture = new BYTE[443 * 190 * 3];
    m_htAbout = TexRef::LoadRCTexture("-ABOUTBOX", 443,190, ptexture);
    if((int)m_htAbout)
    {
        p_texBuff = new BYTE[64*32*3];
        TexHandler::Resize(3, ptexture, 443, 190, p_texBuff, 64, 32);
        m_htNoise = TexRef::GlGenTex(64, 32, 3, p_texBuff, 0);
    }
    delete[] ptexture;
    TexRef::_MakeCurrent = 1;
    dw_uptime = GetTickCount();
	return TRUE;  
}

//---------------------------------------------------------------------------------------
HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, size_t nCtlColor) 
{
    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


//---------------------------------------------------------------------------------------
BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, size_t nHitTest, size_t message) 
{
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CAboutDlg::OnDestroy() 
{
    TexRef::_MakeCurrent=0;
    _pStaticGL->GlMakeCurrent();
    TexRef::RemoveTex(&m_htAbout,0);
    TexRef::RemoveTex(&m_htNoise,0);
    TexRef::_MakeCurrent=1;
    if(m_timer)
        KillTimer(m_timer);
	CDialog::OnDestroy();
    FRM()->GetGLWidnow(0, 0);	
    delete[] p_texBuff;
	
}

void CAboutDlg::OnTimer(size_t nIDEvent) 
{
    _pStaticGL->GLPaint();
	CDialog::OnTimer(nIDEvent);
    Dst+=180;
    if(Dst>0)
        Dst=0;

    if(GetTickCount() - dw_uptime > 1000 * 30)
    {
        KillTimer(m_timer);
        //unlock message

        EndDialog(IDCANCEL);
    }
}

