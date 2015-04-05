// Camera.h: interface for the Camera class.
//The code has strict tutorial purposes ans shall be used only at home
//Author: Marius Chincisan (Zalsoft Inc. 2000-2002)				
//							
// Started: Sept 2001
//---------------------------------------------------------------------------------------
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "Frustrum.h"
#include "baselibhdrs.h"

//---------------------------------------------------------------------------------------
class Poly;
class Camera : public Pos
{
public:
	Camera();
    Camera(V3& eye, REAL a = 0.0f, REAL e = 0.0f, REAL r = 0.0f);
	virtual ~Camera(){};
//ICamera
	void	PosCamera(V3& pos, V3& dir, V3& up){
        _pos = pos;    
		_fwd = dir;
		_up  = up;         // get to these
	}
	void	GetCamera(V3& pos, V3& dir, V3& up){
		pos = _pos;
		dir = _fwd;
		up  = _up;
	}
	const M4	GetViewMtx(REAL dTime);
    void		SetFar(REAL ffar){_frustrum._farZ = ffar; }
	void		At(V3& v){_fwd = (v-_pos).norm();}
	BOOL		IsCulled(const V3* aabbMin, const V3* aabbMax){return _frustrum.IsCulled(aabbMin, aabbMax);}
    BOOL		IsCulled(const V3* point){return _frustrum.IsCulled(point);}
	REAL		GetFov(){return _fov ;}
	void		SetFov(REAL fov)   {
		_fov = fov;	
	}
    void        AddOccluder(const Poly& p);
    void        UpdateFrustrumPlanes(){_frustrum.UpdateFrustrumPlanes(_pos, _fwd, _up, _right); }
private:
    Frustrum            _frustrum;
    REAL		        _fov;           // save here prev frustrum props
    REAL		        _prevfov;
	REAL		        _fowSaved;
	REAL		        _prevrs;
};


#endif // !__CAMERA_H__
