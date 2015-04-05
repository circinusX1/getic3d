// Scene.h: interface for the Scene class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SCENE_H__CB8FF316_9C3D_4B74_94F6_B504E5AC18A5__INCLUDED_)
#define AFX_SCENE_H__CB8FF316_9C3D_4B74_94F6_B504E5AC18A5__INCLUDED_




#include <gl/gl.h>			
#include <gl/glu.h>			
#include <gl/glaux.h>	
#include "z-edmap.h"
#include "poly.h"
#include "brush.h"
#include "texsys.h"
#include "SceItem.h"
#include "SkyDom.h"
#include "..\\_include\\BspFIleStr.h"
#include "Motion.h"
#include "ScriptItem.h"
#include "SoundItem.h"
#include "CustItemCat.h"    /// item categories
#include "BigTerrain.h"

//---------------------------------------------------------------------------------------
class Scene  : public SceItem
{
public:
    Scene(){
        Initialize();
    }

    void Initialize()
    {
        _tcscpy(_si.author,"John Doe");
        _tcscpy(_si.comments,"Getic 2.3");
        _si.levelType = LEV_QUEST;
        _si.maxActors = 8;
        _si.maxFrags  = 100;
        _si.maxTime   = 15;
        _si.defLumin  = CLR(128,128,128);
        _si.gravAcc   = 9.81;
        _si.camFar    = 65535;
        _si.camFov    = 64.0;
        _si.sceSpeed  = 2.0;
        _si.sceFlags  = 0;
        _si.fogNear   = 0;
        _si.fogFar     = 0;
        _si.fogColor   = ZWHITE;
        _si.fogDens    = 0;
        _si.fogFallOff =0;

        _tcscpy(_name, "scene");
        //_si.camOrient1
        //_si.camOrient2

    };
    virtual ~Scene(){
		Clear();
    }

    void Clear(){
        // items first. they may release owned brushes
        Initialize();
        ClearItems();
        ClearBrushes();
        ClearMotions();
		ClearScripts();
        ClearSounds();
        ClearCats();
        ClearPrimitives();
        _terrain.Clear();
        _descs.clear();
        _errstring="";
    }
    void ClearScripts(){_scripts.deleteelements();}
    void ClearSounds(){_sounds.deleteelements();}
    void ClearItems(){_items.deleteelements();}
    void ClearMotions(){_motions.deleteelements();}
	void ClearPrimitives();
    void ClearBrushes();
	int	 GetPolyCnt();
    PBrushes*   GetPrimitives(){return &_primitives;};
    void        AddBrush(Brush* pB){_brushes <<(pB);}
    PBrushes*   GetBrushes(){return &_brushes;};
	void        AddPrimitive(Brush* pB){pB->_brushflags &= ~BRSH_NEW;_primitives << (pB);}
    Brush*      GetPrimitive(int brSel){if(brSel < (int)_primitives.size()) return _primitives[brSel];return 0;}
    Brush*      GetBrush(int brSel){if(brSel < (int)_brushes.size()) return _brushes[brSel];return 0;}
    Brush*      GetBrushByID(int brushID){FOREACH(PBrushes, _primitives, ppb){if((*ppb)->_unicID == brushID){return *ppb;}}return 0;}
    int         AddItem(SceItem* pi){int sz = _items.size();_items << pi;return sz;}
    void        DelItem(SceItem* pi){
        vvector<SceItem*>::iterator fi = _items.findelement(pi);
        if(fi != _items.end()){_items.erase(fi);delete pi;}
    }
    SceItem*  GetItem(int idx){return (idx < (int)_items.size()) ? _items[idx] : 0;}
    vvector<SceItem*>*   GetItems(){return &_items;}
    SceItem*  GetItemByID(int itemID){
        FOREACH(vvector<SceItem*>, _items, ppi){
            if((*ppi)->_id == itemID){
                return *ppi;
            }
        }
        return 0;
    }

   int GetItemsByType(SceItem** ppArray, UINT type, int imax)
   {
       int retval = 0;
        FOREACH(vvector<SceItem*>, _items, ppi){
            if((*ppi)->_item==ITM_LIGTBULB){
                ppArray[retval++] = (*ppi);
                if(retval >= imax) 
                    break;
            }
        }
        return retval;
   }

   SceItem*  GetItemByPtr(DWORD itemPtr){
        FOREACH(vvector<SceItem*>, _items, ppi){
            if((DWORD)(*ppi) == itemPtr){
                return *ppi;
            }
        }
        return 0;
    }

   long    GetIndex(SceItem* pi)
   {
       return _items.getindex(pi);
   }

// motions
	long	GetIndex(CMotion* pi)
	{
        return _motions.getindex(pi);
	}


    int AddMotion(CMotion* pi)
    {
        int sz = _items.size();
        _motions << pi;
        return sz;
    }

    void DelMotion(CMotion* pi){
        vvector<CMotion*>::iterator fi = _motions.findelement(pi);
        if(fi != _motions.end())
        {
            delete pi;
            _motions.erase(fi);
        }
    }
    vvector<CMotion*>*   GetMotions(){return &_motions;}
    int AddScript(ScriptItem* pi)
    {
        int sz = _scripts.size();
        _scripts << pi;
        return sz;
    }

    void DelScript(ScriptItem* pi)
	{
        vvector<ScriptItem*>::iterator fi = _scripts.findelement(pi);
        if(fi != _scripts.end())
        {
            delete pi;
            _scripts.erase(fi);
        }
    }
    
    ScriptItem*   GetScript(long index){
        return _scripts[index];
    }


    vvector<ScriptItem*>*   GetScripts(){
        return &_scripts;
    }

   long    GetIndex(ScriptItem* pi){
        vvector<ScriptItem*>::iterator fi = _scripts.findelement(pi);
        if(fi != _scripts.end())
        {
			return fi-_scripts.begin();
        }
		return -1;
    }

    int FindSoundByName(const char* psz);

    int AddSound(CSoundItem* pi)
    {
        int sz = _sounds.size();
        _sounds << pi;
        return sz;
    }

    void DelSound(CSoundItem* pi)
	{
        vvector<CSoundItem*>::iterator fi = _sounds.findelement(pi);
        if(fi != _sounds.end())
        {
            delete pi;
            _sounds.erase(fi);
        }
    }

    vvector<CSoundItem*>*   GetSounds(){
        return &_sounds;
    }

   long    GetIndex(CSoundItem* ps){
        vvector<CSoundItem*>::iterator fi = _sounds.findelement(ps);
        if(fi != _sounds.end())
        {
			return fi-_sounds.begin();
        }
		return -1;
    }

    CSoundItem*   GetSound(int idx){
        return _sounds[idx];
    }

// item cathegories managemenet (per game)
    BOOL    HasCats(){return _gamecats.size()!=0;}
    BOOL    SearchCats(const char* fileName);
    void    ClearCats();
    vvector<ItemCat>* GetCats(){return &_gamecats;}
    ItemCat* GetCat(const char* catname)
    {
        FOREACH(vvector<ItemCat>,_gamecats,pc)
        {
            if(!strcmp(pc->_catname,catname)){
                return &(*pc);
            }
        }
        return 0;
    }

    CBigTerrain&    Terrain(){return _terrain;}
    map<string ,string>& Descriptions(){ return  _descs;}
    void SetCam(const V3& pos, const V3& elr){_campos=pos; _cameuler=elr;};
private:

    BOOL    ParseSection(string& errors, vvector<char*>& classes, vvector<char*>& items);


private:
    vvector<Brush*>			_primitives;
    vvector<Brush*>			_brushes;
    vvector<SceItem*>		_items;
    vvector<CMotion*>		_motions;
	vvector<ScriptItem*>	_scripts;
    vvector<CSoundItem*>    _sounds;
    vvector<ItemCat>        _gamecats;
    CBigTerrain             _terrain;
    map<string ,string>     _descs;
    string                  _errstring;
public:
    BST_SceneInfo			_si;
    SkyDom					_dom;
    int                     _GCarving;
	int						_dummy[3];
    V3                      _campos;
    V3                      _cameuler;
};


#endif // !defined(AFX_SCENE_H__CB8FF316_9C3D_4B74_94F6_B504E5AC18A5__INCLUDED_)

