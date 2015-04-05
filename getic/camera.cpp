//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------
#include "stdafx.h"
#include "camera.h"
#include "Poly.h"

//--| pos, REAL yaw, REAL pich, REAL rol|-------------------------------------------------
Camera::Camera(V3& pos, REAL yaw, REAL pich, REAL rol)
{
    SetAngles(yaw, pich, rol);
    _pos          = pos;
    _prevfov      = 0;
    _prevrs       = 0;
	_fov		  = 64;
}

//--|Camera::Camera|----------------------------------------------------------------------
Camera::Camera()
{
    _prevrs = 0;
	_fov    = 64;
}

//--| M4 Camera::GetViewMtx|--------------------------------------------------------------
const M4 Camera::GetViewMtx(REAL dTime)
{
    return MLook(_pos, _fwd, _up, _right);
}

//--| void Camera::AddOccluder|--------------------------------------------------------------
void Camera::AddOccluder(const Poly& p)
{
    return;
    const V3* cur;
    const V3* prev = &p._vtci.back()._xyz;
    vvector<Vtx2>::const_iterator b = p._vtci.begin();
    vvector<Vtx2>::const_iterator  e = p._vtci.end();
    _frustrum.AddFrustrumPlan(p._vtci[2]._xyz,p._vtci[1]._xyz,p._vtci[0]._xyz);
    for(; b != e; b++)
    {
        cur = &b->_xyz;
        _frustrum.AddFrustrumPlan(_pos,*cur,*prev);
        prev = cur;
    }
    
}

