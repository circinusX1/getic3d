/**
    File:    isound.h
    Author:  Marius Chincisan
    Desc:    Interface definition for sound plugins DLL's
    History:
*/

#ifndef __Isound_H__
#define __Isound_H__

#include "../baselib/baseutils.h"
#include "../baselib/basecont.h"


//---------------------------------------------------------------------------------------
// One sound sample
class NO_VT Itape
{
public:
	Itape(){}
	virtual ~Itape(){}
public: 
	virtual BOOL D3Sound()=0;
	virtual void D3Sound(BOOL b)=0;
    virtual void D3Params(REAL minDistance, REAL maxDistance)=0;
    virtual void D3Pos(REAL x, REAL y, REAL z)=0;
	virtual void SetVolume(REAL x)=0;
	virtual void AddRef()=0;
	virtual int  Release()=0;
};

//---------------------------------------------------------------------------------------
// The sound player. PLay tapes.
class NO_VT Isound
{
public:
	virtual BOOL	Create(HWND hwnd, const TCHAR* srcPath, int maxSounds)=0;
	virtual void	Destroy()=0;
	virtual Itape*	CreateTape(char *filename, BOOL is3DSound)=0;
	virtual void	DestroyTape(Itape* p)=0;
	virtual void	PlayTape(const Itape *audio, DWORD numRepeats)=0;
	virtual void	StopTape(const Itape *audio)=0;
	virtual void	SetListenerPos(REAL cameraX, REAL cameraY, REAL cameraZ)=0;
	virtual void	SetListenerRolloff(REAL rolloff)=0;
	virtual void	SetListenerOrientation(REAL forwardX, REAL forwardY, REAL forwardZ, REAL topX, REAL topY, REAL topZ)=0;
};



#endif // __Isound_H__


