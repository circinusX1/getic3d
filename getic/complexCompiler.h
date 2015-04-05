// omplexCompiler.h: interface for the ComplexCompiler class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPELX_COMPILER_H__
#define __COMPELX_COMPILER_H__

#include "brush.h"

class ComplexCompiler  
{
public:
    static BOOL    PerformESR(PBrushes* pIB, PBrushes* pOB);

private:

    static BOOL   GetTouchBrush(PBrushes* ,PBrushes& );
    static Brush* GroupPerformESR(PBrushes& );
};

#endif // !__COMPELX_COMPILER_H__
