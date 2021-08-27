/*
// SoundCarrier.cpp: implementation of the SoundCarrier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "SoundItem.h"
#include "scene.h"
#include "SSCarrier.h"
#include "z_ed2Doc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


SoundCarrier::~SoundCarrier()
{
    FOREACH(vvector<int>,_pSoundsIdxes,pps)
    {
        DOC()->_scene.GetSound(*pps)->Release();
    }
}


BOOL	SoundCarrier::AddSound(int iidx)
{
	if(_pSoundsIdxes.findelement(iidx)==_pSoundsIdxes.end())
	{
        DOC()->_scene.GetSound(iidx)->AddRef();
		_pSoundsIdxes << iidx;
		return TRUE;
	}
	return FALSE;
}

int		SoundCarrier::GetSound(int i)
{
	if(_pSoundsIdxes.size()>(size_t)i)
		return _pSoundsIdxes[i];
	return 0;
}


void	SoundCarrier::RemoveSound(int index)
{
	vvector<int>::iterator fi= _pSoundsIdxes.findelement(index);
	if(fi!=_pSoundsIdxes.end())
	{
		DOC()->_scene.GetSound(*fi)->Release();
		_pSoundsIdxes.erase(fi);
	}
}

  */