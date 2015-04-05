// Curve_Class.cpp: implementation of the CCurve_Class class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "Curve_Class.h"




BOOL    CCurve_Class::Compute(const vvector<VNT>& inpoints, 
                              int curveOrder, 
                              vvector<VNT>& outpoints)
{
    _tmax = 0;
    if(curveOrder > 5) curveOrder = 5;

    REAL*   kt = new REAL[inpoints.size()+curveOrder+1];
    REAL*   px = new REAL[inpoints.size()+1];
    REAL*   py = new REAL[inpoints.size()+1];
    REAL*   pz = new REAL[inpoints.size()+1];
    REAL*   ps = new REAL[inpoints.size()+curveOrder+1];

    int m;
    for( m=0; m < inpoints.size(); m++)
    {
        px[m] = inpoints[m].point.x;
        py[m] = inpoints[m].point.y;
        pz[m] = inpoints[m].point.z;
    }
    _TRY
    {
        if(_t == CURV_SPLINE)
        {
            SPL_Knots(m, curveOrder, px, py, pz, kt);
            SPL_Curve(px, py, pz, kt, m,curveOrder, outpoints);
        }
        else
        {
            NUR_Knots(m,curveOrder,px,py,pz, ps, kt);
            NUR_Curve(px,py,pz,kt, m, curveOrder, outpoints);        
        }
    }_CATCHX()
    {
        outpoints.clear();
    }
    delete[] kt;
    delete[] px;
    delete[] py;
    delete[] pz;
    delete[] ps;
    return 1;
}

void CCurve_Class::SPL_Knots(int n, int curveOrder, REAL *px, REAL *py, REAL *pz, REAL* kt)
{   
    int i;
    for(i=0 ;i<=n+curveOrder ; i++)
    {
        if( i>curveOrder-1 )  /* is xi non multiple zero ? */
            if( i<n+1 )  /* is xi non multiple high end? */
                if(*(px+i-curveOrder) == *(px+i-curveOrder+1) &&
                    *(py+i-curveOrder) == *(py+i-curveOrder+1) &&
                    *(pz+i-curveOrder) == *(pz+i-curveOrder+1) ) /* repeated vertex ? */
                    kt[i] = kt[i-1] ;   /* then keep prev kt */
                else   /* non repeated vertex i-curveOrder ? */
                {  _tmax++;
                    kt[i] = kt[i-1] + 1.;
                /* increment kt value */
                }
                else if(i==n+1)  /* first end kt constant */
                {  _tmax++;
                    kt[n+1] = kt[n] + 1.0;
                }
                else kt[i] = kt[i-1];  /* equal end SPL_Knots */
                else kt[i] = 0.;    /* is starting equal zero kt */
    }   /* end of kt assignment loop for i*/
}

/*  B-spline SPL_Basics recursive computation given
n+1 vertices and order curveOrder for interpolant t */
REAL CCurve_Class::SPL_Basics(int i, int curveOrder, REAL t, REAL* kt)
{
    REAL a,b,c,d,e,f;  int j,m;
    j = i+1; m=curveOrder-1;
    if( curveOrder<1 || t<0. || t > _tmax) return 0.0;
    if( curveOrder == 1)   /* order 1 */
        if( t>=kt[i] && t<=kt[i+1])  return 1.0;
        else return 0.0;
        /* Now all curveOrder > 1 */
        a =  t - kt[i];
        b =  kt[i+curveOrder-1]-kt[i];
        c =  kt[i+curveOrder] - t;
        d =  kt[i+curveOrder] - kt[i+1];
        e = SPL_Basics(i,m,t,kt);
        f = SPL_Basics(j,m,t,kt);
        if(b==0. && d==0.)
            if(e==0. && f==0.)  /* only case true */
                return 0.0;  else
            {     TRACEX("error\n");
            return 0.0;  }
            if(b==0. && a*e==0.)
                return c*f/d;
            if(d==0. && c*f==0.)
                return a*e/b;
            return e*a/b + f*c/d;
}    /* end of SPL_Basics computation */

void CCurve_Class::SPL_Curve( REAL *rx, REAL *ry, REAL *rz, REAL* kt ,int n, int curveOrder, vvector<VNT>& outpoints)
{
    REAL t,b,bottom;
    int i;
    VNT v;

    if(curveOrder>n+1)
    {
        TRACEX("Order of curve must be <= number of defining pts\n");
        throw 1;
    }
    for(t=0.1; t<=_tmax; t+=_tmax/20.)
    {
        v.point =V0;
        bottom=0.0;
        for(i=0;i<=n-1;i++)
        {
            b = SPL_Basics(i,curveOrder,t, kt) * 1;
            bottom += b;
            v.point.x += *(rx+i) * b;
            v.point.y += *(ry+i) * b;
            v.point.z += *(rz+i) * b;
        }
        if(bottom==0.0)
        {
            v.point=V0;
            throw 1;
        }
        outpoints<< v;
    }
}

void CCurve_Class::NUR_Knots(int n, 
                              int curveOrder, 
                              REAL *px, REAL *py, REAL *pz, 
                              REAL* ps, REAL* kt )
{
    int i,mul=1, m=1;

    _tmax=n-curveOrder+2;
    for(i=0;i<=n-curveOrder+2;i++) ps[i]=0.;

    for(i=0 ;i<=n+curveOrder ; i++)
    {
        if( i>curveOrder-1 )  /* is xi non multiple zero ? */
            if( i<n+1 )  /* is xi non multiple high end? */
                if( px[i-curveOrder] == px[i-curveOrder+1] &&
                    py[i-curveOrder] == py[i-curveOrder+1] &&
                    pz[i-curveOrder] == pz[i-curveOrder+1] )  /* repeated vertex ? */
                {  kt[i] = kt[i-1] ;   /* then keep prev knot */
                _tmax=_tmax-ps[i-curveOrder]-m; mul++;
                } /* inhibit tmax knot */
                else   /* non repeated vertex i-curveOrder ? */
                    kt[i] = kt[i-1] + m +  ps[i-curveOrder];
                /* increment knot value */
                else if(i==n+1)  /* first end knot constant */
                    kt[n+1] = kt[n] + m + ps[n-curveOrder];
                else kt[i] = kt[i-1];  /* rest of equal end knots */
                else kt[i] = 0.;    /* is a repeated zero knot */
    }   /* end of knot assignment loop for i*/

}
/*  B-spline NUR_Basic recursive computation given
n+1 vertices and order curveOrder for interpolant t */
REAL CCurve_Class::NUR_Basic(int i, int curveOrder, REAL t, REAL* kt)
{
    REAL a,b,c,d,e,f;
    int j,m;
    j = i+1;      m=curveOrder-1;
    if( curveOrder<1 || t<=0. || t > _tmax) return 0.0;
    if( curveOrder == 1)
        if( t>=kt[i] && t<=kt[i+1])  return 1.0;
        else return 0.0;
        /* Now all curveOrder > 1 */
        a =  t - kt[i];
        b =  kt[i+curveOrder-1] - kt[i];
        c =  kt[i+curveOrder] - t;
        d =  kt[i+curveOrder] - kt[i+1];
        e = NUR_Basic(i,m,t,kt);
        f = NUR_Basic(j,m,t,kt);
        if(b==0. && d==0.)
            if(e==0. && f==0.)  /* only case true */
                return 0.0;
            if(b==0. && a*e==0.)
                return c*f/d;
            if(d==0. && c*f==0.)
                return a*e/b;
            return e*a/b + f*c/d;
}    /* end of NUR_Basic computation */

void CCurve_Class::NUR_Curve( REAL *rx, REAL *ry, REAL *rz, 
                             REAL* kt, int n, int curveOrder, vvector<VNT>& outpoints)
{
    VNT v;
    REAL t,b,bottom;

    int i;
    if(curveOrder>n+1)
        TRACEX("Order of curve must be <= number of defining pts\n");
    else
        for(t=0.1; t<=_tmax; t+=_tmax/20.)
        {
            v.point = V0;
            bottom=0.0;
            for(i=0;i<=(n-1);i++)
            {
                b = NUR_Basic(i,curveOrder,t,kt);
                bottom += b;
                v.point.x += *(rx+i) * b;
                v.point.y += *(ry+i) * b;
                v.point.z += *(rz+i) * b;
            }
            if(bottom==0.0)
            {
                TRACEX("Degenerate NUR_Basic beyond %f\n",t);
                throw 1;
            }
            outpoints << v;
        }
}

void compute_intervals(int *u, int n, int t);
void compute_point(int *u, int n, int t, double v, vvector<VNT>& control, V3 *output);
double blend(int k, int t, int *u, double v);

void bspline(int n, int t, vvector<VNT>& input, vvector<VNT>& output, int num_output)
{
  int       *u;
  double    increment,  interval;
  VNT       calcxyz;
  int       output_index;
  
  u = new int[n+t+8];
  compute_intervals(u, n, t);

  output.clear();
  increment=(double) (n-t+2)/(num_output-1);  // how much parameter goes up each time
  interval=0;
  int iCpcts = t;//PThis->GetDlgItemInt(EF_STPCTS);

  for (output_index=0; output_index<num_output; output_index++)
  {
      compute_point(u, n, t, interval, input, &calcxyz.point);
  	  if(output_index < num_output-(iCpcts+2) )
	  {
	      output.push_back(calcxyz);
	  }
      interval=interval+increment;  // increment our parameter
  }
  //output.push_back(input.back());   // put in the last point

  delete u;
}

double blend(int k, int t, int *u, double v)  // calculate the blending value
{
  double value;

  if (t==1)			// base case for the recursion
  {
    if ((u[k]<=v) && (v<u[k+1]))
      value=1;
    else
      value=0;
  }
  else
  {
    if ((u[k+t-1]==u[k]) && (u[k+t]==u[k+1]))  // check for divide by zero
      value = 0;
    else
    if (u[k+t-1]==u[k]) // if a term's denominator is zero,use just the other
      value = (u[k+t] - v) / (u[k+t] - u[k+1]) * blend(k+1, t-1, u, v);
    else
    if (u[k+t]==u[k+1])
      value = (v - u[k]) / (u[k+t-1] - u[k]) * blend(k, t-1, u, v);
    else
      value = (v - u[k]) / (u[k+t-1] - u[k]) * blend(k, t-1, u, v) +
	      (u[k+t] - v) / (u[k+t] - u[k+1]) * blend(k+1, t-1, u, v);
  }
  return value;
}

void compute_intervals(int *u, int n, int t)   // figure out the knots
{
  int j;

  for (j=0; j<=n+t; j++)
  {
    if (j<t)
      u[j]=0;
    else
    if ((t<=j) && (j<=n))
      u[j]=j-t+1;
    else
    if (j>n)
      u[j]=n-t+2;  // if n-t=-2 then we're screwed, everything goes to 0
  }
}

void compute_point(int *u, int n, int t, double v, vvector<VNT>& control, V3 *output)
{
	int k;
	double temp;

	// initialize the variables that will hold our outputted V3
	output->x=0;
	output->y=0;
	output->z=0;

	for (k=0; k<=n; k++)
	{
		temp = blend(k,t,u,v);  // same blend is used for each dimension coordinate

		output->x = output->x + (control[k].point).x * temp;
		output->y = output->y + (control[k].point).y * temp;
		output->z = output->z + (control[k].point).z * temp;
	}


}
