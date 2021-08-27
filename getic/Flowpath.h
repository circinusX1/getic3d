// FlowPath.h: interface for the CFlowPath class.
//
//---------------------------------------------------------------------------------------
#ifndef __FLOW_PATH_H__
#define __FLOW_PATH_H__
//   #pragma warning (disable: 4786)

#include "basecont.h"

//---------------------------------------------------------------------------------------
// defines a graph of search path. A Node can flow in any other nodes trough a link
struct  FlowNode
{
    FlowNode():_leafIdx(-1){
        ::memset(_pNearNodes,0,sizeof(_pNearNodes));
    }
    
    int     _pNearNodes[32];
    V3      _point;
    int     _leafIdx;
};

//---------------------------------------------------------------------------------------
class CFlowPath  
{
public:
    CFlowPath(){};
    virtual ~CFlowPath(){};

    
private:
    vvector<FlowNode>   _flow;
};

#endif // !__FLOW_PATH_H__
