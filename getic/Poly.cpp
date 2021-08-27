//---------------------------------------------------------------------------------------
#include "stdafx.h"
#include "z-edmap.h"
#include "Poly.h"
#include "scene.h"
#include "z_ed2Doc.h"


V3     Poly::__Max = V3(0,0,0);

//---------------------------------------------------------------------------------------
Poly::Poly()
{
    _vtci.reserve(4);
    _selected        = 0;
    _polyflags           = 0;
    _use2splitidx   = -1;
    _idxSplitter     = -1;
    _idxParent       = -1;
	_pBrush		     = 0;
    _planeIdx        = -1;
    _planeIdxFinal   = -1;
	_texcoord.rotate = 0;
	_texcoord.scalex = 1;
	_texcoord.scaley = 1;
	_texcoord.shiftx = 0;
	_texcoord.shifty = 0;
	_texcoord.flipH  = 0;
	_texcoord.flipV  = 0;
    _polyprops       = 0;
    _pUsrData        = 0;
    _polyflags2      = 0;
    _colorS          = ZGREY;
    _colorD          = ZWHITE;
    _colorE          = ZBLACK;
    _shiness         = 96;
    _bump            = 1;
    _friction        = 0;
    _area            = 0;

}

//---------------------------------------------------------------------------------------
void Poly::CalcNormal()
{
    ASSERT(_vtci.size()>=3);
    if(_vtci.size())
    {
	    GCalcNormal(this, _vtci[0]._xyz,_vtci[1]._xyz,_vtci[2]._xyz);
    }
    else
    {
        _vtci.clear();
    }
}


//---------------------------------------------------------------------------------------
Poly& Poly::operator<<(Vtx2& v)
{
    v._pPoly=this;
	_vtci.push_back(v);
	if(_vtci.size() > 3)
		CalcNormal();
    return *this;
}

//---------------------------------------------------------------------------------------
BOOL Poly::operator == (const Poly& p)
{
	if      (_n    == p._n &&
			 _c    == p._c &&
			 _vtci.size()  == p._vtci.size())
    {
        return !memcmp(&_vtci[0], &p._vtci[0], sizeof(Vtx2)*_vtci.size());
    }
    return FALSE;

}

//---------------------------------------------------------------------------------------
void    Poly::Create(int points, Vtx2* pPoints, Brush* pBrush)
{
    _pBrush = pBrush;
    _vtci.clear();
    
    if(points < 3)
        return;
    // each poly caties a bbox for texture calculation
    _box.Reset();
    for(int i=0; i < points;i++)
    {
        _vtci.push_back(pPoints[i]);
        _box.AddPoint(pPoints[i]._xyz);
    }
    CalcNormal();
	SortVertexes();
}

//---------------------------------------------------------------------------------------
void Poly::Create(int points, V3* pPoints, Brush* pBrush)
{
    _pBrush = pBrush;
    _vtci.clear();
    V3* pWalk = pPoints;

    if(points < 3)
        return;
    
    // see the maximum extend of the poly
    _box.Reset();

    for(int i=0; i < points;i++)
    {
        Vtx2 v;
        v._user = 0;
        v._rgb = CLR(255,255,255);
        v._xyz = *(pWalk++);
        _vtci.push_back(v);
        _box.AddPoint(v._xyz);
    }
    CalcNormal();
	SortVertexes();
	
}

//---------------------------------------------------------------------------------------
BOOL Poly::ContainPoint(V3& pi)
{
	V3	v1; 
	V3	v2;
	
    REAL sum = DOIPI;
	
    Vtx2&  lastVx = *(_vtci.end()-1);
	
	v1.setval(lastVx._xyz);
	v1 -= pi;
	v1.norm();
    
    PVertex b = _vtci.begin();
    PVertex e = _vtci.end();
    for(;b!=e;b++)
    {
        Vtx2& wv = *b;
        v2.setval(wv._xyz);
        
        if(vdist(v1, v2) < 1.f) // no less than 4 cm
            return FALSE;

        v2 -= pi;
        v2.norm();
        sum -= (REAL)acos(Vdp(v1, v2));
        v1 = v2;
    }
	return IsZero(sum);
}

//---------------------------------------------------------------------------------------
void Poly::Reverse()
{
    Vertexes   rev;
    
    rev.reserve(_vtci.size());

    _box.Reset();
    Vertexes::reverse_iterator b = _vtci.rbegin();
    Vertexes::reverse_iterator e = _vtci.rend();
    for(;b!=e;b++)
    {
        rev.push_back(*b);
        _box.AddPoint((*b)._xyz);
    }
    _vtci = rev;
    CalcNormal();
}

//---------------------------------------------------------------------------------------
Poly::Poly(const Poly& p)
{
	this->operator=(p);
}

//---------------------------------------------------------------------------------------
Poly& Poly::operator=(const Poly& p)
{
    if(this != &p)
	{
        _vtci.clear();
        CopyProps((Poly&)p);
        FOREACH(vvector<Vtx2>, ((Poly&)p)._vtci, vxI)
        {
            _vtci << *vxI;
        }

        FOREACH(vvector<Vtx2>, _vtci, vxI)
        {
            vxI->_pPoly = this;
        }
    }
    return *this;
}

//---------------------------------------------------------------------------------------
// no vertexes
void	Poly::CopyProps(Poly& p)
{
   
    
    _n          = p._n;
    _t          = p._t;
    _c          = p._c;
    _box        = p._box;
    _pBrush     = p._pBrush; 
    this->GetTexs() = p.GetTexs();              
    
    _nonProjTex = p._nonProjTex;
	_selected   = p._selected;
    _polyflags      = p._polyflags;
    _polyprops      = p._polyprops;
    _texcoord   = p._texcoord;
    _lmInfo     = p._lmInfo;
    _idxParent  = p._idxParent;
    _use2splitidx = p._use2splitidx;
    _idxSplitter= p._idxSplitter;
    _planeIdx   = p._planeIdx;
    _tanimA   = p._tanimA;
    _tanimB   = p._tanimB;
    _texV1   = p._texV1;
    _ilmTex   = p._ilmTex;
    _texV2   = p._texV2;
    _pUsrData    = p._pUsrData;
    _polyflags2  = p._polyflags2;
    _colorS  = p._colorS;
    _colorD  = p._colorD;
    _colorE  = p._colorE;
    _shiness = p._shiness;
    _bump     = p._bump;
    _friction = p._friction;
    _combine  = p._combine;
    _textures = p._textures;
}

//---------------------------------------------------------------------------------------
void    Poly::Recalc()
{
	_box.Reset();
	PVertex b = _vtci.begin();
    PVertex e = _vtci.end();
    for(;b!=e;b++)
    {
        _box.AddPoint((*b)._xyz);

        Poly::__Max.x = Rabs(tmax((*b)._xyz.x,Poly::__Max.x));
        Poly::__Max.y = Rabs(tmax((*b)._xyz.y,Poly::__Max.y));
        Poly::__Max.z = Rabs(tmax((*b)._xyz.z,Poly::__Max.z));
    }
	CalcNormal();
    GetAreea();
}

//---------------------------------------------------------------------------------------
Poly::~Poly()
{
    _vtci.clear();	    // vertexex array
    GTexSys.RemoveTexture(_ilmTex);
}

//---------------------------------------------------------------------------------------
void  Poly::Clear()
{
//    --_texture0;
    _box.Reset();
    _vtci.clear();
    _n = V0;
    _c = 0;
    _t = 0;
}

//---------------------------------------------------------------------------------------
void   Poly::GetWorldPos(M4& m, Poly& pr)
{
    pr.Clear();
    pr = *this;
    pr.Transform(m);
    
}

//---------------------------------------------------------------------------------------
void Poly::Transform(M4 m)
{
    _box.Reset();

    PVertex b = _vtci.begin();
    PVertex e = _vtci.end();
    for(;b!=e;b++)
    {
        Vtx2& v = *b;
        m.v3transform(v._xyz);
        _box.AddPoint(v._xyz);
    }
    CalcNormal();
}


//---------------------------------------------------------------------------------------
void    Poly::Split(Plane& plane, Poly& a, Poly& b, BOOL bAnyway)
{
    a.CopyProps(*this);
    b.CopyProps(*this);

	Vtx2  iv;
    Vtx2 itxB = *_vtci.begin();
    Vtx2 itxA = _vtci.back();

	REAL    fB;
    REAL	fA = plane.DistTo(itxA._xyz);
	//REAL	fA = plane.GetSide(itxA._xyz);
    
    FOREACH(vvector<Vtx2>, _vtci, vxI)
    {
        itxB = *vxI;
        fB   = plane.DistTo(itxB._xyz);
        if(fB > GEpsilon)
        {
            if(fA < -GEpsilon)
            {
                REAL   t = -fA /(fB - fA);
				iv.interpolate(itxA,itxB,t);
                a << iv;
                b << iv;
            }
            a<<itxB;
        }
        else if(fB < -GEpsilon)
        {
            if(fA > GEpsilon)
            {
                REAL t = -fA /(fB - fA);           // t of segment
				iv.interpolate(itxA,itxB,t);
                a<<iv;
                b <<iv;
            }
            b <<itxB;
        }
		else
		{
			a << itxB;
            b << itxB;

		}
        itxA = itxB;
        fA   = fB;
    }

    if(!bAnyway)
    {

        if(b._vtci.size() == _vtci.size() && a._vtci.size()<3)
            a.Clear();
        if(a._vtci.size() == _vtci.size() && b._vtci.size()<3)
            b.Clear();

#ifdef _DEBUG
	    assert(a._vtci.size() >= 3 || a._vtci.size() == 0);
	    assert(b._vtci.size() >= 3 || b._vtci.size() == 0);
#endif //_DEBUG


        if(a._vtci.size()<3)
            a.Clear();
        if(b._vtci.size()<3)
            b.Clear();

    }

    if(a._vtci.size()>=3)
        a.Recalc();

    if(b._vtci.size()>=3)
        b.Recalc();

}

//---------------------------------------------------------------------------------------
void   Poly::Clip(Poly& plane, Poly& pout)
{

    pout._vtci.clear();
    pout._box.Reset();

    pout.CopyProps(*this);

	Vtx2  iv;
    Vtx2& itxB = *_vtci.begin();
    Vtx2& itxA = _vtci.back();

	REAL    fB;
	int     sB;
    REAL	fA = plane.DistTo(itxA._xyz);
	int     sA = plane.Classify(itxA._xyz);
    
    FOREACH(vvector<Vtx2>, _vtci, vxI)
    {
        itxB = *vxI;
        fB   = plane.DistTo(itxB._xyz);
		sB   = plane.Classify(itxB._xyz);

        if(sB > 0)
        {
            if(sA < 0)
            {
                REAL   t = -fA /(fB - fA);
				iv.interpolate(itxA,itxB,t);
                pout << iv;
            }
            pout<<itxB;
        }
        else if(sB < 0)
        {
            if(sA > 0)
            {
                REAL t = -fA /(fB - fA);           // t of segment
				iv.interpolate(itxA,itxB,t);
                pout<<iv;
            }
        }
		else
		{
			pout << itxB;
		}
        itxA = itxB;
        fA   = fB;
    }
	pout.Recalc();
    assert(pout._vtci.size() >= 3);
}

int     Poly::Classify(V3& point)
{
	V3 vdir = point - _vtci[0]._xyz;
	REAL d = Vdp(vdir, _n);

	if (d < -GEpsilon)
		return -1;
	else
		if (d > GEpsilon)
			return 1;
	return 0;
}

// this against plane
REL_POS Poly::Classify(Plane& plane)
{
	int		fronts  = 0, backs = 0, coinciss = 0;
	int		vxes	= _vtci.size();
	REAL	rdp;
	
	FOREACH(vvector<Vtx2>, _vtci, ppct)
	{
		rdp = plane.DistTo(ppct->_xyz);

		if(rdp > GEpsilon)
		{
			fronts++;
		}
		else if(rdp < -GEpsilon)
		{
			backs++;
		}
		else{
			coinciss++;
			backs++;
			fronts++;
		}
	}
	if (coinciss == vxes) 
	{
		return ON_PLANE;
	}
	if (fronts == vxes) 
	{
		return ON_FRONT;
	}
	if (backs  == vxes) 
	{
		return ON_BACK;
	}
	return ON_SPLIT;
}

BOOL    Poly::IntersectsPoly(Poly& p2)
{
    V3   edge[2];
    REAL d2p[2];

    // thiis poly edges
    edge[0] = _vtci.back()._xyz;
    FOREACH(vvector<Vtx2>, _vtci, vxI)
    {
        edge[1] = vxI->_xyz;
        //dist to p2
        d2p[0] = p2.DistTo(edge[0]);
        d2p[1] = p2.DistTo(edge[1]);

        if(d2p[0] * d2p[1] < 0) //they are on the oposite dirs of the plane
                                // we may have a intersection
        {
            V3   ip;
            V3   vray = edge[1]-edge[0];
            vray.norm();
            if(p2.RayIntersect(edge[0],vray,ip))
            {
                if(p2.ContainPoint(ip))
                {
                    return TRUE;
                }
            }
        }
        edge[0]=edge[1];
    }
    return FALSE;
}

void	Poly::ApplyNewTexCoord()
{
    size_t i;
    for( i=0; i < _vtci.size();i++)
    {
        UV& t   = _vtci[i]._uv[GUtex];
		UV& it  = _vtci[i]._tcCrea;

		t.u = it.u * _texcoord.scalex;
		t.v = it.v * _texcoord.scaley;
		t.u += _texcoord.shiftx;
		t.v += _texcoord.shifty;
    }

	//
	// reaply the transformations
	//
	if(_texcoord.flipH)	FlipTC('H');
	if(_texcoord.flipV)	FlipTC('V');
	for( i=0; i< _texcoord.rotate; i++) 
		RotateTC(0,0);

    _texcoord.flipH		= 0;
	_texcoord.flipV		= 0;
	_texcoord.rotate	= 0;

}


void    Poly::RotateTC(int step, int ti)
{
    if(!step)
        _texcoord.rotate++;

    _texcoord.rotate %= _vtci.size();
    
    if(0 == _texcoord.rotate)
        return;

    // roll them
    for(int i=0; i< _texcoord.rotate; i++)
    {
        UV prev = _vtci.back()._uv[ti];
        UV tmp;

        for(size_t j=0; j < _vtci.size(); j++)
        {
            UV& t1  = _vtci[j]._uv[ti];
            tmp = _vtci[j]._uv[ti];
            _vtci[j]._uv[ti] = prev;
            prev=tmp;
        }
    }

}

//---------------------------------------------------------------------------------------
// flips tex coord vertically or horizontally
void    Poly::FlipTC(char cd /*V or H*/)
{
    //calc bbox of the texture. use a 3box (only xy)
    Box box;
    FOREACH(vvector<Vtx2>,_vtci, pvtci)
    {
        Vtx2& rvtci = *pvtci;
        box.AddPoint(V3(rvtci._uv[GUtex].u,rvtci._uv[GUtex].v,0));
    }
    V3& centre = box.GetCenter();

    //calc the bbox of the texture
    FOREACH(vvector<Vtx2>,_vtci, pvtci)
    {
        Vtx2& rvtci = *pvtci;
        UV& tc = rvtci._uv[GUtex];
        tc -= UV(centre.x,centre.y); // shift that centre to make it 0 based
        if(cd=='H')                  // swap   
        {
            tc.u=-tc.u;
        }
        else
        {
            tc.v=-tc.v;
        }
        tc += UV(centre.x,centre.y); //shift back
    }
	if(cd == 'H')
		_texcoord.flipH = !_texcoord.flipH;
	else
		_texcoord.flipV = !_texcoord.flipV;
}

//---------------------------------------------------------------------------------------
// preserves texture coordinates
void	Poly::SortVertexes()
{
	return;
    int i;
	V3	vmin(INFINIT,INFINIT,INFINIT);
	
	V3	vxes[64];

	FOREACH(vvector<Vtx2>,_vtci, pvtci)
	{
		vmin.x = tmin(vmin.x, pvtci->_xyz.x);
		vmin.y = tmin(vmin.y, pvtci->_xyz.y);
		vmin.z = tmin(vmin.z, pvtci->_xyz.z);
	}

	int vCnt = _vtci.size();
	
	for( i=0; i < vCnt ; i++)
	{
		if(_vtci[i]._xyz == vmin)
		{
			if(i==0)
				return;
			for(int j=0; j < vCnt; j++)
			{
				vxes[j] = _vtci[i]._xyz;
				if(++i == vCnt)
					i=0;
			}
			break;
		}
	}

	i=0;
	FOREACH(vvector<Vtx2>,_vtci, pvtci)
	{
		pvtci->_xyz = vxes[i++];
	}

}

void    Poly::DeSelect()
{
    _selected=FALSE;
    Vertexes::iterator b = _vtci.begin();
    Vertexes::iterator e = _vtci.end();
    for(;b!=e;b++)
        b->Select(0);
}

void    Poly::Select()
{
    _selected=TRUE;
    Vertexes::iterator b = _vtci.begin();
    Vertexes::iterator e = _vtci.end();
    for(;b!=e;b++)
        b->Select(this);
}

Poly&    Poly::operator<<(V3& v){
	Vtx2 vt;
	vt._xyz = v;
	(*this)<<vt;
    return *this;
};

BOOL Poly::IsInnerPoint(V3& pi)
{
    // build planes by 2 points and normal. see if the point is in front of all planes
    int side = 0;
    Box exb (_box);
    exb.Expand0(8);
	if(exb.ContainPoint(pi))
	{
		
        V3 itxB;
        V3 itxA = _vtci.back()._xyz;
        V3 point3;

        FOREACH(vvector<Vtx2>, _vtci, vxI)
        {
            itxB = vxI->_xyz;

            point3 = itxB;
            point3 += (_n*32.0f);
            
            do{
                Plane   pl(itxA , itxB , point3);
/*
                if(!added)
                {
                    Poly p;
			        p._vtci << Vtx2(itxA);
   			        p._vtci << Vtx2(itxB);
                    p._vtci << Vtx2(point3);
			        DOC()->AddDbgPoly(p);
                    DOC()->_errPoint=V3(1,1,1);
                }
*/

                side += pl.GetSide(pi);
            }while(0);
            itxA = itxB;
        }

    }

    return abs(side) == _vtci.size();

}

void Poly::ReApplyTexture(size_t crFlags)
{
/****
    const char* szt0 = _textures[GUtex].GetTexName();
    if(0==szt0) 
        return;
    char   styn[64];

    ::strcpy(styn,szt0);

    if(_polyprops & FACE_BLACKMASK) 
    {
        TexHandler th;
        if(th.LoadFile(MKSTR("res\\%s", (styn)),0))
        {
            th.AlphaIt(0);
            GTexSys.GenTexture(MKSTR("&%s",(styn)),
                               th.n_x ,
                               th.n_y, 
                               th.n_bpp, 
                               th.Buffer(),
                               crFlags);
            
            _textures[GUtex].Assign("&%s",(styn),0);
        }
    }
    else 
    {
        // do the black mask alpha
        TexHandler th;
        if(styn[0]='&')
            strcpy(styn,styn+1);
        if(th.LoadFile(MKSTR("res\\%s", styn),0))
        {
            
            GTexSys.GenTexture(MKSTR("%s",
                                   szt0),
                                   th.n_x ,
                                   th.n_y, 
                                   th.n_bpp, 
                                   th.Buffer(),
                                   TEX_NORMAL);
            _textures[GUtex].Assign((char*)MKSTR("%s",styn),0); 
        }

    }
*/
}



BOOL    Poly::IsDeplanetized()
{
    if(_vtci.size()==3)
        return FALSE;

    GCalcNormal(this, _vtci[0]._xyz, _vtci[1]._xyz, _vtci[2]._xyz);

    V3  v0 = _vtci[0]._xyz;
    for(size_t i=1; i < _vtci.size()-1; i++)
    {
        V3 n = GCalcNormal(_vtci[1+i]._xyz,_vtci[i]._xyz, v0);
        if(n == _n)
            continue;
        return TRUE;
    }
    return FALSE;
}


V3      Poly::GetCenter()
{
    V3 ret;
    for(size_t i=0; i < _vtci.size(); ++i)
    {
        ret+=_vtci[i]._xyz;
    }
    ret/= (REAL)_vtci.size();
    return ret;
}

REAL    Poly::GetAreea()
{
    if(_area>0)
        return _area;
	_area = 0.0;
    V3      a, b, c;
	for (size_t i=2 ; i < _vtci.size() ; i++)
	{
        a = _vtci[i-1]._xyz - _vtci[0]._xyz;
        b = _vtci[i  ]._xyz - _vtci[0]._xyz;
        c = Vcp(a, b);
		_area += 0.5 * c.len();
	}
	return (float)_area;    
	
}

void    Poly::RotateTCRad(REAL grads, int ti)
{
    M4  mrotate;
    V3  rotCenter;//
    size_t i;
    // find texture coordinate center
    vvector<V3> relstoCenter(32);
    for ( i=0 ; i < _vtci.size() ; i++)
    {
        rotCenter+=V3(_vtci[i]._uv[ti].u,_vtci[i]._uv[ti].v,0);
        relstoCenter << V3(_vtci[i]._uv[ti].u,_vtci[i]._uv[ti].v,0);
    }
    rotCenter/=(REAL)_vtci.size();

    // store in a separate array relative pos to center
    mrotate.rotate(V3(0,0,1), grads);
    for ( i=0 ; i < relstoCenter.size() ; i++)
    {
        relstoCenter[i]-=rotCenter;
        relstoCenter[i]= mrotate*relstoCenter[i];
        relstoCenter[i]+=rotCenter;

        _vtci[i]._uv[ti].u=relstoCenter[i].x;
        _vtci[i]._uv[ti].v=relstoCenter[i].y;
    }
}



//for exporting quake
void    Poly::BuildTexEdges(V3& v0, V3& v1 ,V3& v2)
{
    UV minUV, maxUV;

    MinMaxUV(0, minUV, maxUV);
	V3 minAx = GetMaxAx(_n);

    if(minAx.x)       // zy
    {
        v0.x = -( _n.y * minUV.u + _n.z * minUV.v - _c ) / _n.x;
        v0.y = minUV.u;
        v0.z = minUV.v;

        v2.x = -( _n.y * maxUV.u + _n.z * minUV.v - _c ) / _n.x;
        v2.y = maxUV.u;
        v2.z = minUV.v;
        
        v1.x = -( _n.y * minUV.u + _n.z * maxUV.v - _c ) / _n.x;
        v1.y = minUV.u;
        v1.z = maxUV.v;
    }
    else if(minAx.y)  // xz
    {
        v0.x = minUV.u;
        v0.y = -( _n.x * minUV.u + _n.z * minUV.v - _c ) / _n.y;
        v0.z = minUV.v;

        v2.x = maxUV.u;
        v2.y = -( _n.x * maxUV.u + _n.z * minUV.v - _c ) / _n.y;
        v2.z = minUV.v;

        v1.x = minUV.u;
        v1.y = -( _n.x * minUV.u + _n.z * maxUV.v - _c ) / _n.y;
        v1.z = maxUV.v;
    }
    else                // xy
    {
        v0.x = minUV.u;
        v0.y = minUV.v;
        v0.z = -( _n.x * minUV.u + _n.y * minUV.v - _c ) / _n.z;
        
        v2.x = maxUV.u;
        v2.y = minUV.v;
        v2.z = -( _n.x * maxUV.u + _n.y * minUV.v - _c ) / _n.z;
        
        v1.x = minUV.u;
        v1.y = maxUV.v;
        v1.z = -( _n.x * minUV.u + _n.y * maxUV.v - _c ) / _n.z;
    }
}

//for exporting quake
void    Poly::BuildTexAxes(V3& v0, V3& v1 )
{
    V3 a,b,c;
    BuildTexEdges(a, b, c);
    v0 = b-a;
    v1 = c-a;
    v0.norm();
    v1.norm();

}

//for exporting quake
BOOL   PolyTouchesPoly(Poly* pa,Poly* pb)
{
    PVertex b = pa->_vtci.begin();
    PVertex e = pa->_vtci.end();
    for(;b!=e;b++)
    {
        PVertex b1 = pb->_vtci.begin();
        PVertex e1 = pb->_vtci.end();
        for(;b1!=e1;b1++)
        {
            if(b1->_xyz == b->_xyz)
                return TRUE;
        }
    }
    return FALSE;
}

int    Poly::Triangulate(vvector<Poly>&   polyFrg)
{
    const int &nvects = _vtci.size();

    if(nvects<=3)
        return 1;

    int        i;
    Vtx2       center;
    int        rgb[4] = {0};
    
    for( i = 0; i < nvects; i++)
    {
        center._xyz   +=  _vtci[i]._xyz;
        center._uv[0] +=  _vtci[i]._uv[0];
        center._uv[1] +=  _vtci[i]._uv[1];
        center._uv[2] +=  _vtci[i]._uv[2];
        center._uv[3] +=  _vtci[i]._uv[3];
        center._nrm   +=  center._nrm;
        rgb[0]     +=  center._rgb.r;   
        rgb[1]     +=  center._rgb.g;   
        rgb[2]     +=  center._rgb.b;   
        rgb[3]     +=  center._rgb.a;   
    }
    center._xyz   /= (REAL)nvects;
    center._uv[0] /= (REAL)nvects;
    center._uv[1] /= (REAL)nvects;
    center._uv[2] /= (REAL)nvects; 
    center._uv[3] /= (REAL)nvects;
    center._nrm   /= (REAL)nvects;
    center._nrm.normalize();
    center._rgb.r = rgb[0]/nvects;
    center._rgb.g = rgb[1]/nvects;
    center._rgb.b = rgb[2]/nvects;
    center._rgb.a = rgb[3]/nvects;


    for(i = 0; i < nvects; i++)
    {
        Poly newPoly;
        
        newPoly.CopyProps(*this);

        center._pPoly = _vtci[i]._pPoly;

        newPoly._vtci << center;
        newPoly._vtci << _vtci[i];
        newPoly._vtci << _vtci[(i+1) % nvects];


        polyFrg << newPoly;
    }
    return polyFrg.size();
}

int    Poly::TrifanIt(vvector<Poly>&   polyFrg)
{
    const int &nvects = _vtci.size();
    if(nvects > 3)
    {
        int  start  = 0;
        int  nOut   = (nvects-2); 
        for(int i = 0; i < nOut; i++)
        {
            Poly newPoly;
            
            newPoly.CopyProps(*this);
            newPoly._vtci << _vtci[start];
            newPoly._vtci << _vtci[(i+1) % nvects];
            newPoly._vtci << _vtci[(i+2) % nvects];
            polyFrg << newPoly;
        }
    }
    else
    {
        polyFrg << *this;
    }
    return polyFrg.size();
}

int     Poly::AddVertex(const V3 &vertex, vvector<Poly> &polyFrg)
{
    const int &nvects = _vtci.size();
    int        i;
    Vtx2       center;
    int        rgb[4]={0};
    
    center._xyz = vertex;
    for( i = 0; i < nvects; i++)
    {
        center._uv[0] +=  _vtci[i]._uv[0];
        center._uv[1] +=  _vtci[i]._uv[1];
        center._uv[2] +=  _vtci[i]._uv[2];
        center._uv[3] +=  _vtci[i]._uv[3];
        center._pPoly = _vtci[i]._pPoly;
        center._nrm   +=  center._nrm;
        rgb[0]     +=  center._rgb.r;   
        rgb[1]     +=  center._rgb.g;   
        rgb[2]     +=  center._rgb.b;   
        rgb[3]     +=  center._rgb.a;   
    }
    center._xyz   /= (REAL)nvects;
    center._uv[0] /= (REAL)nvects;
    center._uv[1] /= (REAL)nvects;
    center._uv[2] /= (REAL)nvects; 
    center._uv[3] /= (REAL)nvects;
    center._nrm   /= (REAL)nvects;
    center._nrm.normalize();
    center._rgb.r = rgb[0]/nvects;
    center._rgb.g = rgb[1]/nvects;
    center._rgb.b = rgb[2]/nvects;
    center._rgb.a = rgb[3]/nvects;


    for(i = 0; i < nvects; i++)
    {
        Poly newPoly;
        
        newPoly.CopyProps(*this);
        newPoly._vtci << center;
        newPoly._vtci << _vtci[i];
        newPoly._vtci << _vtci[(i+1) % nvects];
        polyFrg << newPoly;
    }
    return polyFrg.size();
}

size_t     Poly::GetVxIndex(const Vtx2& vtx)
{
    PVertex itf = std::find(_vtci.begin(), _vtci.end(), vtx);
    if(itf != _vtci.end())
        return itf-_vtci.begin();
    return -1;
}


void    Poly::SnaptoPoint(int ammnt)
{
    Vertexes::iterator b = _vtci.begin();
    Vertexes::iterator e = _vtci.end();
    for(;b!=e;b++)
    {
        int x = (int)b->_xyz.x / (int)SnapToXY ;
        int y = (int)b->_xyz.y / (int)SnapToXY;
        int z = (int)b->_xyz.z / (int)SnapToXY;

        b->_xyz.x = (REAL)(x * SnapToXY) ;
        b->_xyz.y = (REAL)(y * SnapToXY) ;
        b->_xyz.z = (REAL)(z * SnapToXY) ;
    }
    this->Recalc();
}

void    Poly::MinMaxUV(int ti, UV& minUV,UV& maxUV, V3* vnorm)
{
    V3 normal;

    if(vnorm)        normal = * vnorm;
    else             normal = this->_n;
    if(-1==ti)       ti = GUtex;
	V3               majorAx = GetMaxAx(normal);
    minUV           = UV(INFINIT,INFINIT);
    maxUV           = UV(-INFINIT,-INFINIT);

    if(majorAx.x)
    {
        FOREACH(vvector<Vtx2>, this->_vtci, pvtci)
        {
            pvtci->_uv[ti].u = pvtci->_xyz.y;
            pvtci->_uv[ti].v = pvtci->_xyz.z;
            
            // keep the tmin/tmax
            minUV.u = tmin(pvtci->_uv[ti].u,minUV.u);
            minUV.v = tmin(pvtci->_uv[ti].v,minUV.v);

            maxUV.u = tmax(pvtci->_uv[ti].u,maxUV.u);
            maxUV.v = tmax(pvtci->_uv[ti].v,maxUV.v);
        }
    }
    else if(majorAx.y)
    {
        FOREACH(vvector<Vtx2>, this->_vtci, pvtci)
        {
            pvtci->_uv[ti].u = pvtci->_xyz.x;
            pvtci->_uv[ti].v = pvtci->_xyz.z;
            // keep the tmin/tmax
            minUV.u = tmin(pvtci->_uv[ti].u,minUV.u);
            minUV.v = tmin(pvtci->_uv[ti].v,minUV.v);
            
			maxUV.u = tmax(pvtci->_uv[ti].u,maxUV.u);
            maxUV.v = tmax(pvtci->_uv[ti].v,maxUV.v);
        }
    }
    else
    {
        FOREACH(vvector<Vtx2>, this->_vtci, pvtci)
        {
            pvtci->_uv[ti].u = pvtci->_xyz.x;
            pvtci->_uv[ti].v = pvtci->_xyz.y;
            
			// keep the tmin/tmax
            minUV.u = tmin(pvtci->_uv[ti].u,minUV.u);
            minUV.v = tmin(pvtci->_uv[ti].v,minUV.v);
            
			maxUV.u = tmax(pvtci->_uv[ti].u,maxUV.u);
            maxUV.v = tmax(pvtci->_uv[ti].v,maxUV.v);
        }
    }
}

void Poly::ResetTexCoords(int ti)
{
    UV minUV;
    UV maxUV;

    if(-1==ti)   ti = GUtex;

    MinMaxUV(ti, minUV,maxUV);
    UV deltaUV = maxUV - minUV; 

    FOREACH(vvector<Vtx2>, this->_vtci, pvtci)
    {
        pvtci->_uv[ti] -=  minUV;
        pvtci->_uv[ti] /=  deltaUV;
    }


    memset(&_texcoord,0,sizeof(_texcoord));
    _texcoord.scalex = 1;
    _texcoord.scaley = 1;
}

void Poly::InitCalcTC(int ti)
{
    if(-1==ti)  ti = GUtex;

    ResetTexCoords(ti);
    FOREACH(vvector<Vtx2>, this->_vtci, pvtci)
    {
		pvtci->_uv[ti] *= _texcoord.scalex;
		pvtci->_uv[ti] *= _texcoord.scaley;
		pvtci->_uv[ti] += _texcoord.shiftx;
		pvtci->_uv[ti] += _texcoord.shifty;
    }

	if(_texcoord.flipH)	FlipTC('H');
	if(_texcoord.flipV)	FlipTC('V');
	for(int i=0; i< _texcoord.rotate; i++) 
    {
		RotateTC(0,0);
    }
    _texcoord.flipH		= 0;
	_texcoord.flipV		= 0;
	_texcoord.rotate	= 0;
}

void    Poly::CopyTc0InAll()
{
    FOREACH(vvector<Vtx2>, this->_vtci, pvtci)
    {
        //pvtci->_uv[1] = pvtci->_uv[0];
		pvtci->_uv[2] = pvtci->_uv[0];
        pvtci->_uv[3] = pvtci->_uv[0];
    }    
}

