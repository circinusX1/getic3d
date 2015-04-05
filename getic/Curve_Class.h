// Curve_Class.h: interface for the CCurve_Class class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CURVE_CLASS_H__C61EDFCB_8925_404D_BD12_57659C76261A__INCLUDED_)
#define AFX_CURVE_CLASS_H__C61EDFCB_8925_404D_BD12_57659C76261A__INCLUDED_

#include "basecont.h"

struct  VNT
{
    VNT():sel(0){
    }
    BOOL   sel;
    V3  point;
    V3  normal;
};

class CCurve_Class  
{
public:
    typedef enum _CURVTYPE
    {
        CURV_SPLINE,
        CURV_NURB,

    }CURVTYPE;

public:
    CCurve_Class(CURVTYPE t){_t=t;};
    virtual ~CCurve_Class(){};
    BOOL    Compute(const vvector<VNT>& inpoints, int step, vvector<VNT>& outpoints);

private:
    void SPL_Knots(int n, int curveOrder, REAL *px, REAL *py, REAL *pz, REAL*);
    REAL SPL_Basics(int i, int curveOrder, REAL t, REAL*);
    void SPL_Curve( REAL *rx, REAL *ry, REAL *rz, REAL*, int n, int curveOrder, vvector<VNT>& outpoints);

    void NUR_Knots(int n, int curveOrder, REAL *px, REAL *py, REAL *pz, REAL* ps, REAL* kt );
    REAL NUR_Basic(int i, int curveOrder, REAL t, REAL*);
    void NUR_Curve( REAL *rx, REAL *ry, REAL *rz, REAL*, int n, int curveOrder, vvector<VNT>& outpoints);


    CURVTYPE    _t;
    REAL        _tmax;
};

void bspline(int n, int t, vvector<VNT>& input, vvector<VNT>& output, int num_output);

#endif // !defined(AFX_CURVE_CLASS_H__C61EDFCB_8925_404D_BD12_57659C76261A__INCLUDED_)
