// Frustrum.h: interface for the Frustrum class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FRUSTRUM__H__
#define __FRUSTRUM__H__

class Camera;
class Frustrum
{
public:
	Frustrum():_ihulls(0){};
	virtual ~Frustrum(){};

	void		AddFrustrumPlan(const V3& a,const V3& b,const V3& c){
        if(_ihulls<32)
        {
            Plane p(a,b,c);
            AddFrustrumPlan(p);
        }
	}
	void		AddFrustrumPlan(const V3& norm, const V3& ponpl){
        if(_ihulls<32)
        {
            Plane p(norm,ponpl);
            AddFrustrumPlan(p);

        }
	}
	void		AddFrustrumPlan(const Plane& plane){
        if(_ihulls<32)
        {
            int c = _ihulls;
            while (--c>=0)
            {
                if(_hulls[c] == plane)
                    return;
            }
		    _hulls[_ihulls++] = plane;
        }
    }
    BOOL        IsCulled(const V3* point);
	BOOL		IsCulled(const V3* aabbMin, const V3* aabbMax);
    void        UpdateFrustrumPlanes(V3& pos, V3& ldir, V3& lup, V3& lside);
          

	Plane		_hulls[32];		// maxim clipping hulls
	int			_ihulls;		// number of hulls
	V3			_viewfdots[5];	// main frustrum corners
	REAL		_farZ;			// far

};

#endif // __FRUSTRUM_H__
