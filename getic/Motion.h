// Motion.h: interface for the CMotion class.
//
//---------------------------------------------------------------------------------------
#ifndef __MOTION_H__
#define __MOTION_H__


#include "basecont.h"
#include "Brush.h"

//---------------------------------------------------------------------------------------
// motion state
class CMotion;
struct MState
{
    MState(){
        _pos   = V0;
        _euler = V0;
        _trTimes[0] = 1000;
		_trTimes[1] = 1000;
    }

    REAL        _trTimes[2];    // times transitions in this node and in transition
    V3          _trRots[2];     // rotationa appyed durring times
    CMotion*    _pMotion;       // parent motion
    V3          _pos;           // position
    V3          _euler;         // orientation
};


//---------------------------------------------------------------------------------------
// descriobes motion of detail brushes
class Brush;
class NO_VT CMotion  
{
public:
    friend class CZ_ed2Doc;    
    // loop circular list, loop=0 double likked list forward and back
    CMotion(Brush* pBrush,  int iNodes, BOOL bLoop):_pBrush(pBrush),_iNodes(iNodes),_bLoop(bLoop){
        if(iNodes)
        {
            _states.reserve(iNodes);
            Create();
            _center  = pBrush->_box.GetCenter();
        }
        _selNode = 1; // cannot select joint node
    }
    int             GetNodesCount(){return _states.size();}
    MState*         GetState(int i){return &_states[i];}
    BOOL            Create();
    void            GlDraw();
    BOOL            IsLoop(){return _bLoop;}
    void            RefreshPosition();
    void            SelectNode(int idx){_selNode = idx;}
    int             GetSelNode(){return _selNode ;}
    MState*         GetSelState(){return &_states[_selNode] ;};
    V3&             GetCenter(){return _center;}
    Brush*          GetBrush(){return _pBrush;}

    int             _iNodes;
    BOOL            _bLoop;
    vvector<MState> _states;
    Brush*          _pBrush;
    V3              _center;
    int             _selNode;
};

#endif // __MOTION_H__
