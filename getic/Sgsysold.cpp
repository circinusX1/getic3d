// SgSys.cpp: implementation of the CSgSys class.
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "basecont.h"
#include "z-edmap.h"
#include "scene.h"
#include "minibsp.h"
#include "sgsys.h"
//-----------------------------------------------------------------------------
void TRP(char* t, Poly& p)
{
	TRACE(t);
	FOREACH(Vertexes, p._vtci,v)
		TRACE("[%.2f, %.2f, %.2f] ",v->_xyz.x,v->_xyz.y,v->_xyz.z );
	TRACE("\r\n");
}

void    CSgSys::Add(PBrushes& pPrimitives, Brush& brush)
{

}

/*
//-----------------------------------------------------------------------------
void CSgSys::R_ClipFrontPolys(Poly p2Split, Polys& splitters, Polys& outPolys, BOOL bClipIt)
{
	Poly	front;
	Poly	back;

	Poly splitter = splitters.back();
	splitters.pop_back();

	p2Split.CalcNormal();
	splitter.CalcNormal();
    front.CopyProps(p2Split);
	back.CopyProps(p2Split);
    	
	REL_POS  rp = p2Split.Classify(splitter);

	switch (rp)
	{
		case ON_FRONT:
			outPolys.push_back(p2Split);
			return;

		case ON_BACK:
			if (splitters.size() > 0)
				R_ClipFrontPolys(p2Split, splitters, outPolys, bClipIt);
			return;

        case ON_SPLIT:
            p2Split.Split(splitter, front, back);
			if (splitters.size() > 0)
			{
				R_ClipFrontPolys(back, splitters, outPolys, bClipIt);
				if (outPolys.size())
				{
					Poly& tempPoly = outPolys.back();
					if (tempPoly == back)
					{
						outPolys.pop_back();
						outPolys.push_back(p2Split);
						return;
					}
					outPolys.push_back(front);
					return;
				}
				else
				{
					outPolys.push_back(front);
					return;
				}
			}
            else
			    outPolys.push_back(front);
			return;

  		case ON_PLANE:
			if(IsZero(Vdp(p2Split._n, splitter._n)-1))
			{
				if (!bClipIt)
				{
					outPolys.push_back(p2Split);
					return;
				}
			}	
			if (splitters.size() > 0)
				R_ClipFrontPolys(p2Split, splitters, outPolys, bClipIt);
			return;
	}
}
*/