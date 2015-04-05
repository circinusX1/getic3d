// GeticProject.h: interface for the GeticProject class.
//

#ifndef __GETIC_PROJECT__
#define __GETIC_PROJECT__

#include "baselib.h"

class GeticProject  
{
public:
	GeticProject();
	virtual ~GeticProject();
    void     CreateProject(LPCTSTR fullPrjName);

private:
    tstring     _prjpath;
    tstring     _respath;
    tstring     _binpath;
    tstring     _geomfile;
};

#endif //__GETIC_PROJECT__
