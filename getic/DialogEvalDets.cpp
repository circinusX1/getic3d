
#include "stdafx.h"
#include "geticgui.h"
#include "z_ed2Doc.h"
#include "scene.h"
#include "DialogEvalDets.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------------------
CDialogEvalDets::CDialogEvalDets(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEvalDets::IDD, pParent),_onScreen(0)
{
	//{{AFX_DATA_INIT(CDialogEvalDets)		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    //_polys.reserve(32768);
    _rbchecked = RB_FACES;
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEvalDets)
	DDX_Control(pDX, IDCANCEL, m_ca);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_SLIDER1, m_slide);
	//}}AFX_DATA_MAP
}


//---------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDialogEvalDets, CDialog)
	//{{AFX_MSG_MAP(CDialogEvalDets)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(RB_FACES, OnFaces)
	ON_BN_CLICKED(RB_BRUSHES, OnBrushes)
	ON_BN_CLICKED(CK_BYPOLIS, OnBypolis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------------------
static int comparepolys (const void *e1, const void *e2 ) 
{
	 Brush** pp1 = (Brush**)e1;
	 Brush** pp2 = (Brush**)e2;

	 return (*pp2)->_polys.size()- (*pp1)->_polys.size();
}

//---------------------------------------------------------------------------------------
static int comparebox   (const void *e1, const void *e2 ) 
{
	 Brush** pp1 = (Brush**)e1;
	 Brush** pp2 = (Brush**)e2;

	 return (*pp1)->_box.Volume() - (*pp2)->_box.Volume();
}

//---------------------------------------------------------------------------------------
static int compare (const void *e1, const void *e2 ) 
{
	 Poly** pp1 = (Poly**)e1;
	 Poly** pp2 = (Poly**)e2;

	 return (*pp1)->_area - (*pp2)->_area;
}

//---------------------------------------------------------------------------------------
BOOL CDialogEvalDets::OnInitDialog() 
{
	CDialog::OnInitDialog();
    _onScreen = 1;
    CheckDlgButton(CK_BYAREA, BST_CHECKED);
    CheckRadioButton(RB_FACES,RB_BRUSHES,_rbchecked);
    if(_rbchecked == RB_FACES)
        OnFaces() ;
    else
        OnBrushes(); 
    return 1;
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if(IsDlgButtonChecked(RB_BRUSHES))
    {
        MarkBrushes();
    }
    else
    {
        MarkFaces();
    }
    *pResult = 0;
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::MarkBrushes()
{
    int percent = m_slide.GetPos();
    int maxsz   = _brushes.size();
    int maxidx  = (percent*maxsz)/1000;
    int index   = 0;

    CWaitCursor cwc;
    
    DOC()->DeselectBrushes();

    Brush* pBrush;
    vvector<Brush*>::iterator ppBrush = _brushes.begin();
    for(; ppBrush != _brushes.end(); ppBrush++)
    {
        pBrush = (*ppBrush);
        if(DOC()->_selBox._brushflags & BRSH_SELVIS)
        {
			if(DOC()->_selBox._box.ContainPoint(pBrush->_box._min) && 
			   DOC()->_selBox._box.ContainPoint(pBrush->_box._max))
            {
				if(index < maxidx)
				{
					DOC()->SelectBrush(pBrush);
				}
				else
				{
					break;
				}
            }

        }
        else
        {
			if(index < maxidx)
			{
				DOC()->SelectBrush(pBrush);
			}
			else
			{
				break; 
			}
        }
		++index;
    }
    int selected = ppBrush-_brushes.begin();
    SetDlgItemText(ST_OUT, MKSTR("Selected Brushes for detailed Brushes: %d (%d%%) ", selected, percent/10));
    cwc.Restore();
    DOC()->Invalidate(1);
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::MarkFaces()
{
    int percent = m_slide.GetPos();
    int maxsz   = _polys.size();
    int maxidx  = (percent*maxsz)/1000;
    int index   = 0;
    Poly*         pp;
    REAL         area  = 0;

    CWaitCursor cwc;
    DOC()->DeselectPolys();

    vvector<Poly*>::iterator ppPoly = _polys.begin();
    for(; ppPoly != _polys.end(); ppPoly++)
    {
		pp = *ppPoly;

		if(DOC()->_selBox._brushflags & BRSH_SELVIS)
		{
			if(DOC()->_selBox._box.ContainPoint(pp->_box._min) && 
			   DOC()->_selBox._box.ContainPoint(pp->_box._max))
			{
				if(index < maxidx)
				{
					DOC()->SelectFace(pp);
				}
				else
				{
					area = pp->_area;
					break;
				}
			}
		}
		else
		{
			
			if(index < maxidx)
			{
				DOC()->SelectFace(pp);
			}
			else 
			{
				area = pp->_area;
				break;
			}
		}
        ++index;
    }

    // keep selecting close areas util bigger htan last one +2
    for(; ppPoly != _polys.end();ppPoly = ppPoly++)
    {
        pp = *ppPoly;
		if(DOC()->_selBox._brushflags & BRSH_SELVIS)
		{
			if(DOC()->_selBox._box.ContainPoint(pp->_box._min) && 
			   DOC()->_selBox._box.ContainPoint(pp->_box._max))
			{
				if(fabs(pp->_area - area) < 8.0)
					DOC()->SelectFace(pp);
				else 
					break;
			}
		}
		else
		{
			if(fabs(pp->_area - area) < 8.0)
				DOC()->SelectFace(pp);
			else 
				break;
		}
    }

    int selected = ppPoly-_polys.begin();
    SetDlgItemText(ST_OUT, MKSTR("Selected Polygons for detailed Brushes: %d (%d%%) ", selected, percent/10));
    cwc.Restore();
    DOC()->Invalidate(1);
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::OnCancel() 
{
    DOC()->DeselectAll(0);// destroys this window
	DestroyWindow();
}


//---------------------------------------------------------------------------------------
void CDialogEvalDets::OnOk() 
{
    CWaitCursor cwc;

    if(IsDlgButtonChecked(RB_BRUSHES))
    {
        DetalizeSelectedBrushes();
    }
    else
    {
        DetalizeSelectedFaces();
    }
	DestroyWindow();
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::DetalizeSelectedBrushes()
{
    FOREACH(vvector<Brush*>,  DOC()->_pSelBrushes, a)
    {
		(*a)->_brushflags |= BRSH_DETAIL;
    }

    if(IsDlgButtonChecked(CK_CSGDETS))
    {
        DOC()->OnPermcut();
    }
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::DetalizeSelectedFaces()
{

    CWaitCursor     cwc;

    set<Poly*>      detail;
    int             detsz;
	int             detsz2;
	
    // for each selected polygon
    FOREACH(vvector<Poly*>,  DOC()->_pSelPolys, a)
    {
		if((*a)->_polyflags & POLY_PROCESSED)   continue;

        (*a)->_polyflags|=POLY_PROCESSED;
        detail.insert(*a);
        

        // search touching in set
        detsz = detail.size();
        FOREACH(vvector<Poly*>, DOC()->_pSelPolys, b)
        {
            if(b==a)							continue;
            if((*b)->_polyflags & POLY_PROCESSED)   continue;

            if(::PolyTouchesPoly(*a,*b))
            {
                (*b)->_polyflags|=POLY_PROCESSED;
                detail.insert(*b);
            }
        }
		detsz2 = detail.size();
		
        while(detsz != detsz2)     //as long there are adds into the list  
        {                          // go collect more 
            detsz = detail.size();

            set<Poly*>::iterator si = detail.begin();
            set<Poly*>::iterator se = detail.end();
            for(;si!=se;si++)
            {
                FOREACH(vvector<Poly*>, DOC()->_pSelPolys, b)
                {
                    if(*si==*b)                        continue;
                    if((*b)->_polyflags & POLY_PROCESSED)  continue;

                    if(::PolyTouchesPoly(*si,*b))
                    {
                        (*b)->_polyflags|=POLY_PROCESSED;
                        detail.insert(*b);
                    }
                }
            }
            detsz2 = detail.size();
        }

        Brush* pb  = new Brush();
        pb->_brushflags = BRSH_DETAIL|BRSH_SOLID|BRSH_DIRTY;
        set<Poly*>::iterator si = detail.begin();
        set<Poly*>::iterator se = detail.end();
        for(;si!=se;si++)
        {
            (*si)->_polyflags|=POLY_PROCESSED;
            pb->_polys << *(*si);
        }
        pb->Recalc();
        DOC()->_scene.AddPrimitive(pb);
        detail.clear();
    }

    set<Brush*> toUpdate;
    FOREACH(vvector<Poly*>,  DOC()->_pSelPolys, a)
    {
        toUpdate.insert((*a)->_pBrush);
        (*a)->_pBrush->Deleteface((*a));
    }
	DOC()->DeselectPolys();

    set<Brush*>::iterator bi = toUpdate.begin();
    set<Brush*>::iterator be = toUpdate.end();
    for(;bi!=be;bi++)
    {
        (*bi)->Recalc();
    }
    
    
    DOC()->OnEnter();
    DOC()->Invalidate(1);
    DestroyWindow();
}


//---------------------------------------------------------------------------------------
void CDialogEvalDets::OnDestroy() 
{
	_polys.clear();
    DOC()->BlockSelections(0);
	CDialog::OnDestroy();
    _onScreen=0;
}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::OnFaces() 
{
	DOC()->DeselectAll(0); 

	BOOL b  = IsDlgButtonChecked(RB_FACES);
    GetDlgItem(CK_BYPOLIS)->EnableWindow(!b);
    GetDlgItem(CK_CSGDETS)->EnableWindow(!b);

    if(b)
    {
	    _polys.clear();
        int sz = DOC()->_scene.GetPrimitives()->size();

        m_slide.SetRange(0,1000);
        m_slide.SetPos(0);
        CWaitCursor cwc;

        FOREACH(PBrushes, (*DOC()->_scene.GetPrimitives()), ppbb)
        {
            Brush* pB = (*ppbb);
            FOREACH(vvector<Poly>, pB->_polys, pPoly)
            {
			    (*pPoly).Recalc();
                _polys.push_back(&(*pPoly));
            }
        }
        _TRY
        {
		    ::qsort(&_polys[0], _polys.size(), sizeof(Poly*), compare);
	    }
        _CATCHX()
	    {
		    AfxMessageBox("Error sorting polygons", MB_OK|MB_ICONHAND);
	    }
        cwc.Restore();
    }
    else
        OnBrushes() ;

}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::OnBrushes() 
{
	DOC()->DeselectAll(0);
	BOOL b  = IsDlgButtonChecked(RB_BRUSHES);
    GetDlgItem(CK_BYPOLIS)->EnableWindow(b);
    GetDlgItem(CK_CSGDETS)->EnableWindow(b);
    if(b)
    {
        _brushes.clear();

        int sz = DOC()->_scene.GetPrimitives()->size();
        _brushes.reserve(sz);
        m_slide.SetRange(0,1000);
        m_slide.SetPos(0);
        
        CWaitCursor cwc;

        FOREACH(PBrushes, (*DOC()->_scene.GetPrimitives()), ppbb)
        {
            _brushes << (*ppbb);
        }
        _TRY
        {
            if(IsDlgButtonChecked(CK_BYPOLIS))
            {
                ::qsort(&_brushes[0], _brushes.size(), sizeof(Brush*), comparepolys);
            }
            else
            {
		        ::qsort(&_brushes[0], _brushes.size(), sizeof(Brush*), comparebox);
            }
	    }
        _CATCHX()
	    {
		    AfxMessageBox("Error sorting polygons", MB_OK|MB_ICONHAND);
	    }
        cwc.Restore();
    }
    else
        OnFaces(); 

}

//---------------------------------------------------------------------------------------
void CDialogEvalDets::OnBypolis() 
{
    OnBrushes();
	DOC()->DeselectAll(0);
}
